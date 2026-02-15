#include "include/models/Hexagon.h"
#include <sstream>
#include <cmath> // For sin, cos, M_PI

// Constructor
Hexagon::Hexagon(int cx, int cy, int radius, double angle, const std::string &stroke, const std::string &fill, int width)
    : Object(cx, cy, stroke, fill, width), _radius(radius) , _rotation(angle)
{
}

void Hexagon::draw(QPainter &painter) {
    // 1. Setup Pen (Stroke)
    if (_stroke_color == "none") {
        painter.setPen(Qt::NoPen);
    } else {
        QColor sColor(_stroke_color.c_str());
        QPen pen(sColor, _stroke_width);
        painter.setPen(pen);
    }

    // 2. Setup Brush (Fill)
    if (_fill_color == "none") {
        painter.setBrush(Qt::NoBrush);
    } else {
        QColor fColor(_fill_color.c_str());
        painter.setBrush(fColor);
    }

    // 3. Calculate the 6 points of the Hexagon
    QPolygon polygon;
    for (int i = 0; i < 6; ++i) {
        // 60 degrees * i
        // Note: Subtracting 30 degrees (pi/6) makes it point up nicely like a "stop sign" orientation
        // purely aesthetic, you can use i * 60 if you prefer flat top
        double angle_deg = 60 * i - 30 + _rotation; 
        double angle_rad = M_PI / 180.0 * angle_deg;
        
        int px = _x + static_cast<int>(_radius * std::cos(angle_rad));
        int py = _y + static_cast<int>(_radius * std::sin(angle_rad));
        
        polygon << QPoint(px, py);
    }

    // 4. Draw
    painter.drawPolygon(polygon);
}

std::string Hexagon::toSVG() {
    std::stringstream out;
    // We stick to the custom tag as discussed, since SVG polygons are verbose
    out << "<hexagon cx=\"" << _x << "\" cy=\"" << _y << "\" rotation=\"" << _rotation << "\" r=\"" << _radius << "\" stroke=\"" << _stroke_color << "\" fill=\"" << _fill_color 
        << "\" stroke-width=\"" << _stroke_width << "\" />";
    return out.str();
}

bool Hexagon::isHit(const QPoint& p) {
    // Simple Approximation: Check if point is inside the circumcircle
    // (Distance to center <= Radius)
    long long dx = p.x() - _x; 
    long long dy = p.y() - _y;
    
    // Using 0.866 (sqrt(3)/2) approximates the "inner" boundary better
    // But full radius is usually safer for UI selection
    return (dx * dx + dy * dy <= 1LL * _radius * _radius);
}


void Hexagon::resize(int dx , int dy){
}
QRect Hexagon::getBounds() const {
}