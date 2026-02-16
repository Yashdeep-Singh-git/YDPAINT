#include "include/models/Freehand.h"
#include <sstream>
#include <cmath>
#include <algorithm>
#include <limits> 

Freehand::Freehand(const std::vector<QPoint>& points, std::string stroke, int width) : Object(0, 0, stroke, "none", width) {
    for(const auto& p : points) {
        _points.push_back(QPointF(p));                   // Store points as float precision
    }
    if (!_points.empty()) {
        _x = (int)(_points[0].x());                      // Set initial anchor
        _y = (int)(_points[0].y());
    }
}

void Freehand::draw(QPainter &painter) {
    if (_points.empty()) return;
    QColor sColor(_stroke_color.c_str());
    QPen pen(sColor, _stroke_width);
    pen.setCapStyle(Qt::RoundCap);                       // Smooth line ends
    pen.setJoinStyle(Qt::RoundJoin);                     // Smooth line corners
    painter.setPen(pen);
    painter.setBrush(Qt::NoBrush);
    painter.drawPolyline(_points.data(), _points.size()); // Draw connected lines
}

std::string Freehand::toSVG() {
    std::stringstream out;
    out << "<polyline points=\"";
    for (int i = 0; i < _points.size(); i++) {
        out << _points[i].x() << "," << _points[i].y();  // Serialize point coordinates
        if (i < _points.size() - 1) out << " ";
    }
    out << "\" stroke=\"" << _stroke_color << "\" fill=\"none\" " << "stroke-width=\"" << _stroke_width << "\" " << "stroke-linecap=\"round\" stroke-linejoin=\"round\" />";
    return out.str();
}

bool Freehand::isHit(const QPoint& p) {
    if (_points.size() < 2) return false;
    double threshold = (_stroke_width / 2.0) + 5.0;      // Click tolerance radius
    double thresholdSq = threshold * threshold;
    for (size_t i = 0; i < _points.size() - 1; ++i) {    // Iterate through all segments
        QPointF p1 = _points[i]; // QPointF
        QPointF p2 = _points[i+1]; // QPointF
        double px = p.x(); double py = p.y();
        double x1 = p1.x(); double y1 = p1.y();
        double x2 = p2.x(); double y2 = p2.y();
        double dx = x2 - x1;
        double dy = y2 - y1;
        if (dx == 0 && dy == 0) continue;
        double t = ((px - x1) * dx + (py - y1) * dy) / (dx * dx + dy * dy); // Project point to line
        t = std::max(0.0, std::min(1.0, t));             // Clamp to segment [0,1]
        double closestX = x1 + t * dx;
        double closestY = y1 + t * dy;
        double distSq = (px - closestX)*(px - closestX) + (py - closestY)*(py - closestY);
        if (distSq <= thresholdSq) return true;          // Hit detected
    }
    return false;
}

void Freehand::resize(int dx, int dy) {
    if (_points.empty()) return;
    double minX = _points[0].x();
    double maxX = _points[0].x();
    double minY = _points[0].y();
    double maxY = _points[0].y();
    for (const auto &p : _points) {                      // Find current bounding box
        if (p.x() < minX) minX = p.x();
        if (p.x() > maxX) maxX = p.x();
        if (p.y() < minY) minY = p.y();
        if (p.y() > maxY) maxY = p.y();
    }
    double width = maxX - minX;
    double height = maxY - minY;
    if ((width < 1 && dx<0) || (width+dx < 0.1)) return; // Prevent collapse
    if ((height < 1 && dy<0) || (height+dy < 0.1)) return ;

    for (auto &p : _points) {
        double relativeX = (p.x() - minX) / width;       // Calculate normalized position
        double relativeY = (p.y() - minY) / height;
        p.setX(p.x() + relativeX * dx);                  // Apply proportional stretch
        p.setY(p.y() + relativeY * dy);
    }
}

QRect Freehand::getBounds() const {
    if (_points.empty()) return QRect();
    double minX = _points[0].x();
    double maxX = _points[0].x();
    double minY = _points[0].y();
    double maxY = _points[0].y();
    for (const auto &p : _points) {                      // Calculate min/max extents
        if (p.x() < minX) minX = p.x();
        if (p.x() > maxX) maxX = p.x();
        if (p.y() < minY) minY = p.y();
        if (p.y() > maxY) maxY = p.y();
    }
    return QRect((int)(minX), (int)(minY), (int)(maxX - minX), (int)(maxY - minY));
}
void Freehand::move(int dx, int dy) {
    for (auto& p : _points) {
        p.setX(p.x() + dx);                              // Shift all points
        p.setY(p.y() + dy);
    }
}
void Freehand::addPoint(const QPoint& p) { 
    _points.push_back(QPointF(p));                       // Append new point
}
std::unique_ptr<Object> Freehand::clone() const {
    return std::make_unique<Freehand>(*this);            // Deep copy
}