#pragma once

#include <vector>

#include <QGraphicsView>

#include "GraphBuilder.h"
#include "RelationshipEdge.h"

class QGraphicsScene;
class QGraphicsItem;
class QGraphicsSimpleTextItem;

class GraphView : public QGraphicsView {
    Q_OBJECT

public:
    explicit GraphView(QWidget* parent = nullptr);

    void setGraph(const Graph& graph);
    const std::vector<RelationshipEdge>& forest() const;

signals:
    void nodeSelected(int id);
    void edgeSelected(int index);

protected:
    void wheelEvent(QWheelEvent* event) override;

private:
    void handleSelection();
    void updateLabels();

    QGraphicsScene* scene;
    std::vector<RelationshipEdge> forestEdges;
    std::vector<QGraphicsSimpleTextItem*> labels;
    QGraphicsItem* highlighted = nullptr;
    int selectedNode = -1;
    int selectedEdge = -1;
};
