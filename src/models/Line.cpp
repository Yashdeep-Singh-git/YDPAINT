#include "include/models/Line.h"
#include <sstream>
#include <cmath>
#include <algorithm> 

Line::Line(int x1, int y1, int x2, int y2, std::string stroke, int width) : Object(x1, y1, stroke, "none", width), _x2(x2), _y2(y2) {}

void Line::draw(QPainter &painter) {
    if (_stroke_color == "none") {
        painter.setPen(Qt::NoPen);
    } 
    else {
        QColor sColor(_stroke_color.c_str());
        QPen pen(sColor, _stroke_width);
        pen.setCapStyle(Qt::RoundCap); 
        painter.setPen(pen);
    }
    painter.drawLine(_x, _y, _x2, _y2);
}

std::string Line::toSVG() {
    std::stringstream out;
    out << "<line x1=\"" << _x << "\" y1=\"" << _y << "\" x2=\"" << _x2 << "\" y2=\"" << _y2 << "\" stroke=\"" << _stroke_color << "\" stroke-width=\"" << _stroke_width << "\" />";
    return out.str();
}

bool Line::isHit(const QPoint& p) {
    double px = p.x();
    double py = p.y();
    double x1 = _x;
    double y1 = _y;
    double x2 = _x2;
    double y2 = _y2;
    double dx = x2 - x1;
    double dy = y2 - y1;
    if (dx == 0 && dy == 0) return false; 
    double t = ((px - x1) * dx + (py - y1) * dy) / (dx * dx + dy * dy);
    t = std::max(0.0, std::min(1.0, t));
    double closestX = x1 + t * dx;
    double closestY = y1 + t * dy;
    double distSq = (px - closestX) * (px - closestX) + (py - closestY) * (py - closestY);
    double tolerance = (_stroke_width / 2.0) + 5.0; 
    return distSq <= (tolerance * tolerance);
}
void Line::resize(int dx, int dy) {
    _x2 += dx;
    _y2 += dy;
}

QRect Line::getBounds() const {
    return QRect(_x, _y, _x2 - _x, _y2 - _y).normalized();
}

void Line::move(int dx , int dy) {
    _x += dx; _y += dy;
    _x2 += dx; _y2 += dy;
}
std::unique_ptr<Object> Line::clone() const {
    return std::make_unique<Line>(*this);
}