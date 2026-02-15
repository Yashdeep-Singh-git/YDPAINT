#include "include/models/Freehand.h"
#include <sstream>
#include <cmath>
#include <algorithm> // for std::max, std::min

// Constructor
Freehand::Freehand(const std::vector<QPoint>& points, std::string stroke, int width)
    : Object(0, 0, stroke, "none", width), _points(points) 
{
    // We set base x,y to 0 because a polyline is defined by absolute points, 
    // not relative to a single anchor.
    if (!points.empty()) {
        _x = points[0].x();
        _y = points[0].y();
    }
}

void Freehand::draw(QPainter &painter) {
    if (_points.empty()) return;

    // 1. Setup Pen
    QColor sColor(_stroke_color.c_str());
    QPen pen(sColor, _stroke_width);
    pen.setCapStyle(Qt::RoundCap);  // Makes ends smooth
    pen.setJoinStyle(Qt::RoundJoin); // Makes corners smooth
    painter.setPen(pen);
    
    painter.setBrush(Qt::NoBrush); // Freehand lines have no fill

    // 2. Draw all points as a single connected path
    painter.drawPolyline(_points.data(), _points.size());
}

std::string Freehand::toSVG() {
    std::stringstream out;
    out << "<polyline points=\"";
    
    // Output format: "x1,y1 x2,y2 x3,y3"
    for (size_t i = 0; i < _points.size(); ++i) {
        out << _points[i].x() << "," << _points[i].y();
        if (i < _points.size() - 1) {
            out << " "; // Space between coordinate pairs
        }
    }
    
    out << "\" stroke=\"" << _stroke_color << "\" fill=\"none\" "
        << "stroke-width=\"" << _stroke_width << "\" "
        << "stroke-linecap=\"round\" stroke-linejoin=\"round\" />";
        
    return out.str();
}

bool Freehand::isHit(const QPoint& p) {
    if (_points.size() < 2) return false;

    // Tolerance: Stroke width / 2 + Buffer (5 pixels)
    double threshold = (_stroke_width / 2.0) + 5.0;
    double thresholdSq = threshold * threshold;

    // Check distance to every segment of the polyline
    for (size_t i = 0; i < _points.size() - 1; ++i) {
        QPoint p1 = _points[i];
        QPoint p2 = _points[i+1];

        double px = p.x(); double py = p.y();
        double x1 = p1.x(); double y1 = p1.y();
        double x2 = p2.x(); double y2 = p2.y();

        double dx = x2 - x1;
        double dy = y2 - y1;

        if (dx == 0 && dy == 0) continue;

        // Project point onto line segment (same math as Line.cpp)
        double t = ((px - x1) * dx + (py - y1) * dy) / (dx * dx + dy * dy);
        t = std::max(0.0, std::min(1.0, t));

        double closestX = x1 + t * dx;
        double closestY = y1 + t * dy;

        double distSq = (px - closestX)*(px - closestX) + (py - closestY)*(py - closestY);

        if (distSq <= thresholdSq) return true;
    }

    return false;
}

void Freehand::resize(int dx , int dy){
}
QRect Freehand::getBounds() const {
}