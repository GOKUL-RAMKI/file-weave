#include "GraphView.h"

#include <algorithm>
#include <cmath>
#include <cstddef>

#include <QBrush>
#include <QColor>
#include <QFont>
#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>
#include <QGraphicsScene>
#include <QGraphicsSimpleTextItem>
#include <QPainter>
#include <QPen>
#include <QWheelEvent>

#include "GraphLayout.h"
#include "Kruskal.h"

using namespace std;

namespace {

constexpr int roleType = 0;
constexpr int roleIndex = 1;
constexpr int nodeType = 1;
constexpr int edgeType = 2;
constexpr double radius = 10.0;
constexpr double minZoom = 0.05;
constexpr double maxZoom = 8.0;
constexpr int labelLimit = 100;
constexpr double stretchLimit = 3.0;

const QColor nodeColor(180, 210, 240);
const QColor selectedNodeColor(255, 220, 120);
const QColor edgeColor(120, 120, 120);
const QColor selectedEdgeColor(200, 60, 60);

}

GraphView::GraphView(QWidget* parent) : QGraphicsView(parent), scene(new QGraphicsScene(this)) {
    setScene(scene);
    setRenderHint(QPainter::Antialiasing);
    setDragMode(QGraphicsView::ScrollHandDrag);
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    setBackgroundBrush(QColor(250, 250, 250));
    connect(scene, &QGraphicsScene::selectionChanged, this, &GraphView::handleSelection);
}

const vector<RelationshipEdge>& GraphView::forest() const {
    return forestEdges;
}

void GraphView::setGraph(const Graph& graph) {
    highlighted = nullptr;
    selectedNode = -1;
    selectedEdge = -1;
    labels.clear();
    forestEdges.clear();
    scene->clear();
    resetTransform();

    forestEdges = kruskal(static_cast<int>(graph.nodes.size()), graph.edges);
    vector<Point> points = computeLayout(static_cast<int>(graph.nodes.size()), forestEdges);

    if (graph.nodes.empty()) {
        scene->setSceneRect(0, 0, 100, 100);
        return;
    }

    double minX = points[0].x, maxX = points[0].x;
    double minY = points[0].y, maxY = points[0].y;
    for (const Point& point : points) {
        minX = min(minX, point.x);
        maxX = max(maxX, point.x);
        minY = min(minY, point.y);
        maxY = max(maxY, point.y);
    }

    double width = maxX - minX;
    double height = maxY - minY;
    double targetAspect = 1.65;
    if (viewport()->height() > 0) {
        targetAspect = clamp(static_cast<double>(viewport()->width()) / viewport()->height(), 1.0, 2.5);
    }
    if (width > 0 && height > 0) {
        double aspect = width / height;
        if (aspect < targetAspect) {
            double factor = min(targetAspect / aspect, stretchLimit);
            double centerX = (minX + maxX) / 2.0;
            for (Point& point : points) point.x = centerX + (point.x - centerX) * factor;
        } else {
            double factor = min(aspect / targetAspect, stretchLimit);
            double centerY = (minY + maxY) / 2.0;
            for (Point& point : points) point.y = centerY + (point.y - centerY) * factor;
        }
    }

    double centerX = (minX + maxX) / 2.0;
    QPen edgePen(edgeColor);
    edgePen.setWidth(2);
    for (size_t i = 0; i < forestEdges.size(); ++i) {
        const RelationshipEdge& edge = forestEdges[i];
        QGraphicsLineItem* item = scene->addLine(points[edge.a].x, points[edge.a].y,
            points[edge.b].x, points[edge.b].y, edgePen);
        item->setData(roleType, edgeType);
        item->setData(roleIndex, static_cast<int>(i));
        item->setFlag(QGraphicsItem::ItemIsSelectable);
    }

    QFont labelFont;
    labelFont.setPointSize(8);
    for (size_t i = 0; i < graph.nodes.size(); ++i) {
        QGraphicsEllipseItem* item = scene->addEllipse(points[i].x - radius, points[i].y - radius,
            radius * 2, radius * 2, QPen(QColor(60, 60, 60)), QBrush(nodeColor));
        item->setData(roleType, nodeType);
        item->setData(roleIndex, static_cast<int>(i));
        item->setFlag(QGraphicsItem::ItemIsSelectable);

        QGraphicsSimpleTextItem* label = scene->addSimpleText(
            QString::fromStdString(graph.nodes[i].name), labelFont);
        double labelX = points[i].x + radius + 4;
        if (points[i].x > centerX) {
            labelX = points[i].x - radius - 4 - label->boundingRect().width();
        }
        label->setPos(labelX, points[i].y - 8);
        label->setBrush(QColor(40, 40, 40));
        label->setAcceptedMouseButtons(Qt::NoButton);
        label->setFlag(QGraphicsItem::ItemIgnoresTransformations, true);
        labels.push_back(label);
    }

    QRectF bounds;
    for (const Point& point : points) {
        bounds = bounds.united(QRectF(point.x - radius, point.y - radius, radius * 2, radius * 2));
    }
    bounds.adjust(-40, -40, 40, 40);
    scene->setSceneRect(bounds);

    fitInView(bounds, Qt::KeepAspectRatio);
    if (transform().m11() > 2.0) {
        resetTransform();
        scale(2.0, 2.0);
        centerOn(bounds.center());
    }

    updateLabels();
}

void GraphView::updateLabels() {
    bool showAll = static_cast<int>(labels.size()) <= labelLimit;
    int edgeA = -1;
    int edgeB = -1;
    if (selectedEdge >= 0 && selectedEdge < static_cast<int>(forestEdges.size())) {
        edgeA = forestEdges[selectedEdge].a;
        edgeB = forestEdges[selectedEdge].b;
    }

    for (size_t i = 0; i < labels.size(); ++i) {
        int id = static_cast<int>(i);
        labels[i]->setVisible(showAll || id == selectedNode || id == edgeA || id == edgeB);
    }
}

void GraphView::handleSelection() {
    if (highlighted) {
        int type = highlighted->data(roleType).toInt();
        if (type == nodeType) {
            if (QGraphicsEllipseItem* item = qgraphicsitem_cast<QGraphicsEllipseItem*>(highlighted)) {
                item->setBrush(nodeColor);
            }
        } else if (QGraphicsLineItem* item = qgraphicsitem_cast<QGraphicsLineItem*>(highlighted)) {
            QPen pen = item->pen();
            pen.setColor(edgeColor);
            pen.setWidth(2);
            item->setPen(pen);
        }
    }
    highlighted = nullptr;

    QList<QGraphicsItem*> selected = scene->selectedItems();
    if (selected.isEmpty()) {
        selectedNode = -1;
        selectedEdge = -1;
        emit selectionCleared();
        updateLabels();
        return;
    }

    highlighted = selected.first();
    int type = highlighted->data(roleType).toInt();
    int index = highlighted->data(roleIndex).toInt();

    if (type == nodeType) {
        if (QGraphicsEllipseItem* item = qgraphicsitem_cast<QGraphicsEllipseItem*>(highlighted)) {
            item->setBrush(selectedNodeColor);
        }
        selectedNode = index;
        selectedEdge = -1;
        emit nodeSelected(index);
    } else {
        if (QGraphicsLineItem* item = qgraphicsitem_cast<QGraphicsLineItem*>(highlighted)) {
            QPen pen = item->pen();
            pen.setColor(selectedEdgeColor);
            pen.setWidth(3);
            item->setPen(pen);
        }
        selectedNode = -1;
        selectedEdge = index;
        emit edgeSelected(index);
    }

    updateLabels();
}

void GraphView::wheelEvent(QWheelEvent* event) {
    double factor = event->angleDelta().y() > 0 ? 1.15 : 1.0 / 1.15;
    double current = transform().m11();
    if ((factor > 1.0 && current >= maxZoom) || (factor < 1.0 && current <= minZoom)) {
        event->accept();
        return;
    }
    scale(factor, factor);
    event->accept();
}
