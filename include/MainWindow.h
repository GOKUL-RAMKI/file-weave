#pragma once

#include <QMainWindow>

#include "GraphBuilder.h"

class QListWidget;
class QDoubleSpinBox;
class QLabel;
class QProgressBar;
class QPushButton;
class QTextBrowser;
class GraphView;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);

private:
    void addDirectory();
    void removeDirectory();
    void scan();
    void showNode(int id);
    void showEdge(int index);
    void clearInspection();
    void openSelectedFile();

    QListWidget* directoryList;
    QPushButton* addButton;
    QPushButton* removeButton;
    QPushButton* scanButton;
    QDoubleSpinBox* thresholdBox;
    QProgressBar* progress;
    QLabel* status;
    QTextBrowser* details;
    QPushButton* openFileButton;
    GraphView* view;
    Graph graph;
    int selectedNode = -1;
    bool scanning = false;
};
