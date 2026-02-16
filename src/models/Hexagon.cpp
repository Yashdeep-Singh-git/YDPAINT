#include "include/models/Hexagon.h"
#include <sstream>
#include <cmath>

Hexagon::Hexagon(int cx, int cy, int radius, double angle, const std::string &stroke, const std::string &fill, int width)
    : Object(cx, cy, stroke, fill, width), _radius(radius) , _rotation(angle){}


void Hexagon::draw(QPainter &painter) {
    if (_stroke_color == "none") {
        painter.setPen(Qt::NoPen);
    } else {
        QColor sColor(_stroke_color.c_str());
        QPen pen(sColor, _stroke_width);
        painter.setPen(pen);
    }
    if (_fill_color == "none") {
        painter.setBrush(Qt::NoBrush);
    } else {
        QColor fColor(_fill_color.c_str());
        painter.setBrush(fColor);
    }
    QPolygon polygon;
    for (int i = 0; i < 6; ++i) {
        double angle_deg = 60 * i - 30 + _rotation; 
        double angle_rad = M_PI / 180.0 * angle_deg;
        
        int px = _x + static_cast<int>(_radius * std::cos(angle_rad));
        int py = _y + static_cast<int>(_radius * std::sin(angle_rad));
        
        polygon << QPoint(px, py);
    }

    painter.drawPolygon(polygon);
}

std::string Hexagon::toSVG() {
    std::stringstream out;
    out << "<hexagon cx=\"" << _x << "\" cy=\"" << _y << "\" rotation=\"" << _rotation << "\" r=\"" << _radius << "\" stroke=\"" << _stroke_color << "\" fill=\"" << _fill_color 
        << "\" stroke-width=\"" << _stroke_width << "\" />";
    return out.str();
}

bool Hexagon::isHit(const QPoint& p) {
    long long dx = p.x() - _x; 
    long long dy = p.y() - _y;
    
    return (dx * dx + dy * dy <= 1LL * _radius * _radius);
}


void Hexagon::resize(int dx, int dy) {
    int change = (std::abs(dx) > std::abs(dy)) ? dx : dy;
    
    _radius += change;
    if (_radius < 5) _radius = 5; 
}

QRect Hexagon::getBounds() const {
    return QRect(_x - _radius, _y - _radius, _radius * 2, _radius * 2);
}
std::unique_ptr<Object> Hexagon::clone() const {
    return std::make_unique<Hexagon>(*this);
}