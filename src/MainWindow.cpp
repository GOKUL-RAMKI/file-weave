#include "MainWindow.h"

#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

#include <QApplication>
#include <QDateTime>
#include <QDoubleSpinBox>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QLabel>
#include <QListWidget>
#include <QProgressBar>
#include <QPushButton>
#include <QTextBrowser>
#include <QVBoxLayout>
#include <QWidget>

#include "FileScanner.h"
#include "GraphView.h"
#include "RelationshipScorer.h"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    setWindowTitle("File Relationship Graph Visualizer");
    resize(1200, 800);

    QWidget* central = new QWidget(this);
    QHBoxLayout* layout = new QHBoxLayout(central);

    QWidget* panel = new QWidget(central);
    panel->setFixedWidth(300);
    QVBoxLayout* controls = new QVBoxLayout(panel);

    controls->addWidget(new QLabel("Folders", panel));
    directoryList = new QListWidget(panel);
    controls->addWidget(directoryList);

    QHBoxLayout* buttons = new QHBoxLayout();
    addButton = new QPushButton("Add Folder", panel);
    removeButton = new QPushButton("Remove", panel);
    buttons->addWidget(addButton);
    buttons->addWidget(removeButton);
    controls->addLayout(buttons);

    QHBoxLayout* threshold = new QHBoxLayout();
    threshold->addWidget(new QLabel("Threshold", panel));
    thresholdBox = new QDoubleSpinBox(panel);
    thresholdBox->setRange(0.0, 1.0);
    thresholdBox->setDecimals(2);
    thresholdBox->setSingleStep(0.05);
    thresholdBox->setValue(Scoring::defaultThreshold);
    threshold->addWidget(thresholdBox);
    controls->addLayout(threshold);

    scanButton = new QPushButton("Scan", panel);
    controls->addWidget(scanButton);

    progress = new QProgressBar(panel);
    progress->setRange(0, 100);
    progress->setValue(0);
    controls->addWidget(progress);

    status = new QLabel("Add one or more folders, then scan.", panel);
    status->setWordWrap(true);
    controls->addWidget(status);
    controls->addStretch(1);

    layout->addWidget(panel);

    QWidget* graphArea = new QWidget(central);
    QVBoxLayout* graphLayout = new QVBoxLayout(graphArea);
    view = new GraphView(graphArea);
    graphLayout->addWidget(view, 1);

    details = new QTextBrowser(graphArea);
    details->setMinimumHeight(140);
    details->setMaximumHeight(200);
    details->setHtml("<i>Select a node or edge to inspect it.</i>");
    graphLayout->addWidget(details);

    layout->addWidget(graphArea, 1);
    setCentralWidget(central);

    connect(addButton, &QPushButton::clicked, this, &MainWindow::addDirectory);
    connect(removeButton, &QPushButton::clicked, this, &MainWindow::removeDirectory);
    connect(scanButton, &QPushButton::clicked, this, &MainWindow::scan);
    connect(view, &GraphView::nodeSelected, this, &MainWindow::showNode);
    connect(view, &GraphView::edgeSelected, this, &MainWindow::showEdge);
}

void MainWindow::addDirectory() {
    QString dir = QFileDialog::getExistingDirectory(this, "Add Folder");
    if (dir.isEmpty()) return;
    if (!directoryList->findItems(dir, Qt::MatchExactly).isEmpty()) return;
    directoryList->addItem(dir);
}

void MainWindow::removeDirectory() {
    int row = directoryList->currentRow();
    if (row < 0) return;
    delete directoryList->takeItem(row);
}

void MainWindow::scan() {
    if (scanning) return;

    if (directoryList->count() == 0) {
        status->setText("Add at least one folder first.");
        return;
    }

    scanning = true;
    addButton->setEnabled(false);
    removeButton->setEnabled(false);
    scanButton->setEnabled(false);
    thresholdBox->setEnabled(false);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    progress->setValue(0);

    // Copy the folder list first so removing an entry during a scan cannot
    // invalidate the loop.
    std::vector<QString> folders;
    folders.reserve(directoryList->count());
    for (int i = 0; i < directoryList->count(); ++i) {
        folders.push_back(directoryList->item(i)->text());
    }

    std::vector<FileNode> files;
    std::unordered_set<std::string> seen;
    int skipped = 0;
    int total = static_cast<int>(folders.size());

    for (int i = 0; i < total; ++i) {
        const QString& folder = folders[i];
        status->setText("Scanning " + folder + "...");
        QApplication::processEvents();

        ScanResult result = scanDirectory(folder.toStdString());
        skipped += result.skipped;
        for (FileNode& file : result.files) {
            if (seen.insert(file.path).second) files.push_back(std::move(file));
        }
        progress->setValue((i + 1) * 100 / total);
    }

    status->setText(QString("Building graph for %1 files...").arg(files.size()));
    QApplication::processEvents();

    graph = buildGraph(std::move(files), thresholdBox->value());

    status->setText("Laying out graph...");
    QApplication::processEvents();

    view->setGraph(graph);
    details->setHtml("<i>Select a node or edge to inspect it.</i>");

    status->setText(QString("%1 files, %2 qualifying edges, %3 forest edges, %4 skipped")
        .arg(graph.nodes.size())
        .arg(graph.edges.size())
        .arg(view->forest().size())
        .arg(skipped));

    scanning = false;
    addButton->setEnabled(true);
    removeButton->setEnabled(true);
    scanButton->setEnabled(true);
    thresholdBox->setEnabled(true);
    QApplication::restoreOverrideCursor();
}

void MainWindow::showNode(int id) {
    if (id < 0 || id >= static_cast<int>(graph.nodes.size())) return;
    const FileNode& node = graph.nodes[id];

    QString text = "<b>" + QString::fromStdString(node.name).toHtmlEscaped() + "</b><br>";
    text += "Extension: " + (node.extension.empty() ? QString("none") : QString::fromStdString(node.extension).toHtmlEscaped()) + "<br>";
    text += "Path: " + QString::fromStdString(node.path).toHtmlEscaped() + "<br>";
    text += "Size: " + (node.hasSize ? QString::number(static_cast<qulonglong>(node.size)) + " bytes" : QString("unavailable")) + "<br>";
    text += "Modified: " + (node.hasTime
        ? QDateTime::fromSecsSinceEpoch(static_cast<qint64>(node.modified)).toString(Qt::ISODate)
        : QString("unavailable"));
    details->setHtml(text);
}

void MainWindow::showEdge(int index) {
    const std::vector<RelationshipEdge>& forest = view->forest();
    if (index < 0 || index >= static_cast<int>(forest.size())) return;
    const RelationshipEdge& edge = forest[index];

    auto name = [this](int id) {
        if (id < 0 || id >= static_cast<int>(graph.nodes.size())) return QString("?");
        return QString::fromStdString(graph.nodes[id].name).toHtmlEscaped();
    };

    QString text = QString("<b>%1 &harr; %2</b><br>").arg(name(edge.a), name(edge.b));
    text += QString("Combined score: %1<br>").arg(edge.score, 0, 'f', 3);
    text += QString("Jaccard: %1<br>").arg(edge.jaccard, 0, 'f', 3);
    text += QString("Edit similarity: %1<br>").arg(edge.edit, 0, 'f', 3);
    text += QString("Path proximity: %1<br>").arg(edge.path, 0, 'f', 3);
    text += QString("Metadata similarity: %1<br>").arg(edge.metadata, 0, 'f', 3);
    text += QString("Kruskal cost: %1").arg(edge.cost, 0, 'f', 3);
    details->setHtml(text);
}
