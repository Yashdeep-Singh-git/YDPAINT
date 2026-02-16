#include "include/models/Freehand.h"
#include <sstream>
#include <cmath>
#include <algorithm>
#include <limits> 

Freehand::Freehand(const std::vector<QPoint>& points, std::string stroke, int width)
    : Object(0, 0, stroke, "none", width) 
{
    for(const auto& p : points) {
        _points.push_back(QPointF(p));
    }

    if (!_points.empty()) {
        _x = static_cast<int>(_points[0].x());
        _y = static_cast<int>(_points[0].y());
    }
}

void Freehand::draw(QPainter &painter) {
    if (_points.empty()) return;

    QColor sColor(_stroke_color.c_str());
    QPen pen(sColor, _stroke_width);
    pen.setCapStyle(Qt::RoundCap);
    pen.setJoinStyle(Qt::RoundJoin);
    painter.setPen(pen);
    painter.setBrush(Qt::NoBrush);

    // QPainter can draw QPointF arrays directly!
    painter.drawPolyline(_points.data(), _points.size());
}

std::string Freehand::toSVG() {
    std::stringstream out;
    out << "<polyline points=\"";
    
    for (size_t i = 0; i < _points.size(); ++i) {
        // Output as plain numbers (SVG handles decimals fine)
        out << _points[i].x() << "," << _points[i].y();
        if (i < _points.size() - 1) out << " ";
    }
    
    out << "\" stroke=\"" << _stroke_color << "\" fill=\"none\" "
        << "stroke-width=\"" << _stroke_width << "\" "
        << "stroke-linecap=\"round\" stroke-linejoin=\"round\" />";
        
    return out.str();
}

bool Freehand::isHit(const QPoint& p) {
    if (_points.size() < 2) return false;

    double threshold = (_stroke_width / 2.0) + 5.0;
    double thresholdSq = threshold * threshold;

    for (size_t i = 0; i < _points.size() - 1; ++i) {
        QPointF p1 = _points[i]; // QPointF
        QPointF p2 = _points[i+1]; // QPointF

        double px = p.x(); double py = p.y();
        double x1 = p1.x(); double y1 = p1.y();
        double x2 = p2.x(); double y2 = p2.y();

        double dx = x2 - x1;
        double dy = y2 - y1;

        if (dx == 0 && dy == 0) continue;

        double t = ((px - x1) * dx + (py - y1) * dy) / (dx * dx + dy * dy);
        t = std::max(0.0, std::min(1.0, t));

        double closestX = x1 + t * dx;
        double closestY = y1 + t * dy;

        double distSq = (px - closestX)*(px - closestX) + (py - closestY)*(py - closestY);

        if (distSq <= thresholdSq) return true;
    }
    return false;
}

void Freehand::resize(int dx, int dy) {
    if (_points.empty()) return;

    double minX = _points[0].x();
    double maxX = _points[0].x();
    double minY = _points[0].y();
    double maxY = _points[0].y();

    for (const auto &p : _points) {
        if (p.x() < minX) minX = p.x();
        if (p.x() > maxX) maxX = p.x();
        if (p.y() < minY) minY = p.y();
        if (p.y() > maxY) maxY = p.y();
    }

    double width = maxX - minX;
    double height = maxY - minY;

    if ((width < 1 && dx<0) || (width+dx < 0.1)) return;
    if ((height < 1 && dy<0) || (height+dy < 0.1)) return ;

    for (auto &p : _points) {
        double relativeX = (p.x() - minX) / width;
        double relativeY = (p.y() - minY) / height;

        p.setX(p.x() + relativeX * dx);
        p.setY(p.y() + relativeY * dy);
    }
}

QRect Freehand::getBounds() const {
    if (_points.empty()) return QRect();
    double minX = std::numeric_limits<double>::max();
    double minY = std::numeric_limits<double>::max();
    double maxX = std::numeric_limits<double>::lowest();
    double maxY = std::numeric_limits<double>::lowest();

    for (const auto& p : _points) {
        if (p.x() < minX) minX = p.x();
        if (p.y() < minY) minY = p.y();
        if (p.x() > maxX) maxX = p.x();
        if (p.y() > maxY) maxY = p.y();
    }
    return QRect(static_cast<int>(minX), static_cast<int>(minY), 
                 static_cast<int>(maxX - minX), static_cast<int>(maxY - minY));
}

void Freehand::move(int dx, int dy) {
    for (auto& p : _points) {
        p.setX(p.x() + dx);
        p.setY(p.y() + dy);
    }
}
void Freehand::addPoint(const QPoint& p) { 
    _points.push_back(QPointF(p)); 
}
std::unique_ptr<Object> Freehand::clone() const {
    return std::make_unique<Freehand>(*this);
}