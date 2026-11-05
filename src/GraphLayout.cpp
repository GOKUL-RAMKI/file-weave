#include "GraphLayout.h"

#include <algorithm>
#include <cmath>
#include <cstddef>

#include "DisjointSet.h"

namespace {

constexpr double areaWidth = 1400.0;
constexpr double areaHeight = 850.0;
constexpr double packWidth = 2400.0;
constexpr double gravity = 0.1;
constexpr double gap = 40.0;
constexpr double minBox = 80.0;

struct Cluster {
    std::vector<int> nodes;
    std::vector<RelationshipEdge> edges;
    std::vector<Point> points;
    double width = 0;
    double height = 0;
};

std::vector<Point> forceLayout(int count, const std::vector<RelationshipEdge>& edges, double ideal, int iterations) {
    std::vector<Point> points(count);
    if (count <= 1) return points;

    int columns = static_cast<int>(std::ceil(std::sqrt(static_cast<double>(count))));
    int rows = (count + columns - 1) / columns;
    for (int i = 0; i < count; ++i) {
        points[i] = { (i % columns) * ideal, (i / columns) * ideal };
    }
    Point center{ (columns - 1) * ideal / 2.0, (rows - 1) * ideal / 2.0 };

    iterations = std::max(1, iterations);
    for (int step = 0; step < iterations; ++step) {
        std::vector<Point> displacement(count);

        for (int i = 0; i < count; ++i) {
            for (int j = i + 1; j < count; ++j) {
                double dx = points[i].x - points[j].x;
                double dy = points[i].y - points[j].y;
                double distance = std::max(0.01, std::sqrt(dx * dx + dy * dy));
                double force = std::min(ideal * ideal / distance, ideal);
                dx = dx / distance * force;
                dy = dy / distance * force;
                displacement[i].x += dx;
                displacement[i].y += dy;
                displacement[j].x -= dx;
                displacement[j].y -= dy;
            }
        }

        for (const RelationshipEdge& edge : edges) {
            if (edge.a < 0 || edge.b < 0 || edge.a >= count || edge.b >= count) continue;
            double dx = points[edge.a].x - points[edge.b].x;
            double dy = points[edge.a].y - points[edge.b].y;
            double distance = std::max(0.01, std::sqrt(dx * dx + dy * dy));
            double force = std::min(distance * distance / ideal, ideal);
            dx = dx / distance * force;
            dy = dy / distance * force;
            displacement[edge.a].x -= dx;
            displacement[edge.a].y -= dy;
            displacement[edge.b].x += dx;
            displacement[edge.b].y += dy;
        }

        for (int i = 0; i < count; ++i) {
            displacement[i].x += (center.x - points[i].x) * gravity;
            displacement[i].y += (center.y - points[i].y) * gravity;
        }

        double temperature = ideal * (1.0 - static_cast<double>(step) / iterations);
        for (int i = 0; i < count; ++i) {
            double length = std::sqrt(displacement[i].x * displacement[i].x + displacement[i].y * displacement[i].y);
            if (length <= 0) continue;
            double scale = std::min(length, temperature) / length;
            points[i].x += displacement[i].x * scale;
            points[i].y += displacement[i].y * scale;
        }
    }

    double minX = points[0].x, maxX = points[0].x;
    double minY = points[0].y, maxY = points[0].y;
    for (const Point& point : points) {
        minX = std::min(minX, point.x);
        maxX = std::max(maxX, point.x);
        minY = std::min(minY, point.y);
        maxY = std::max(maxY, point.y);
    }
    double offsetX = (minX + maxX) / 2.0;
    double offsetY = (minY + maxY) / 2.0;
    for (Point& point : points) {
        point.x -= offsetX;
        point.y -= offsetY;
    }

    return points;
}

std::vector<Cluster> buildClusters(int nodeCount, const std::vector<RelationshipEdge>& forest) {
    DisjointSet sets(nodeCount);
    for (const RelationshipEdge& edge : forest) {
        if (edge.a < 0 || edge.b < 0 || edge.a >= nodeCount || edge.b >= nodeCount) continue;
        sets.unite(edge.a, edge.b);
    }

    std::vector<int> rootToCluster(nodeCount, -1);
    std::vector<int> clusterOf(nodeCount, -1);
    std::vector<Cluster> clusters;

    for (int i = 0; i < nodeCount; ++i) {
        int root = sets.find(i);
        int index = rootToCluster[root];
        if (index < 0) {
            index = static_cast<int>(clusters.size());
            rootToCluster[root] = index;
            clusters.push_back({});
        }
        clusterOf[i] = index;
        clusters[index].nodes.push_back(i);
    }

    std::vector<int> localIndex(nodeCount, -1);
    for (const Cluster& cluster : clusters) {
        for (std::size_t i = 0; i < cluster.nodes.size(); ++i) {
            localIndex[cluster.nodes[i]] = static_cast<int>(i);
        }
    }

    for (const RelationshipEdge& edge : forest) {
        if (edge.a < 0 || edge.b < 0 || edge.a >= nodeCount || edge.b >= nodeCount) continue;
        int index = clusterOf[edge.a];
        if (index != clusterOf[edge.b]) continue;

        RelationshipEdge local = edge;
        local.a = localIndex[edge.a];
        local.b = localIndex[edge.b];
        clusters[index].edges.push_back(local);
    }

    return clusters;
}

}

std::vector<Point> computeLayout(int nodeCount, const std::vector<RelationshipEdge>& forest, int iterations) {
    std::vector<Point> points(nodeCount);
    if (nodeCount <= 1) return points;

    double perNodeArea = areaWidth * areaHeight / static_cast<double>(nodeCount);
    double ideal = std::sqrt(perNodeArea);
    std::vector<Cluster> clusters = buildClusters(nodeCount, forest);

    for (Cluster& cluster : clusters) {
        int count = static_cast<int>(cluster.nodes.size());
        cluster.points = forceLayout(count, cluster.edges, ideal, iterations);

        double minX = cluster.points[0].x, maxX = cluster.points[0].x;
        double minY = cluster.points[0].y, maxY = cluster.points[0].y;
        for (const Point& point : cluster.points) {
            minX = std::min(minX, point.x);
            maxX = std::max(maxX, point.x);
            minY = std::min(minY, point.y);
            maxY = std::max(maxY, point.y);
        }

        double width = maxX - minX;
        double height = maxY - minY;
        double target = std::max(minBox, std::sqrt(perNodeArea * count));
        double scale = std::max(width, height) > 0 ? target / std::max(width, height) : 1.0;
        for (Point& point : cluster.points) {
            point.x *= scale;
            point.y *= scale;
        }
        cluster.width = width * scale;
        cluster.height = height * scale;
    }

    std::vector<int> order(clusters.size());
    for (std::size_t i = 0; i < order.size(); ++i) order[i] = static_cast<int>(i);
    std::sort(order.begin(), order.end(), [&](int left, int right) {
        double leftSize = std::max(clusters[left].width, clusters[left].height);
        double rightSize = std::max(clusters[right].width, clusters[right].height);
        return leftSize > rightSize;
    });

    double cursorX = 0;
    double cursorY = 0;
    double rowHeight = 0;
    double packedWidth = 0;

    for (int index : order) {
        Cluster& cluster = clusters[index];
        double boxWidth = std::max(cluster.width, minBox);
        double boxHeight = std::max(cluster.height, minBox);

        if (cursorX > 0 && cursorX + boxWidth > packWidth) {
            cursorX = 0;
            cursorY += rowHeight + gap;
            rowHeight = 0;
        }

        double centerX = cursorX + boxWidth / 2.0;
        double centerY = cursorY + boxHeight / 2.0;
        for (std::size_t i = 0; i < cluster.nodes.size(); ++i) {
            points[cluster.nodes[i]] = { centerX + cluster.points[i].x, centerY + cluster.points[i].y };
        }

        cursorX += boxWidth + gap;
        rowHeight = std::max(rowHeight, boxHeight);
        packedWidth = std::max(packedWidth, cursorX - gap);
    }

    double packedHeight = cursorY + rowHeight;
    double scale = std::min({ 1.0, areaWidth / std::max(packedWidth, 1.0), areaHeight / std::max(packedHeight, 1.0) });
    double offsetX = (areaWidth - packedWidth * scale) / 2.0;
    double offsetY = (areaHeight - packedHeight * scale) / 2.0;
    for (Point& point : points) {
        point.x = offsetX + point.x * scale;
        point.y = offsetY + point.y * scale;
    }

    return points;
}
