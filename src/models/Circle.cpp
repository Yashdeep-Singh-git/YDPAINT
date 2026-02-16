#include "include/models/Circle.h"
#include <string>
#include <sstream>
#include <cmath>

Circle::Circle(int cx , int cy , int radius , const std::string &stroke , const std::string &fill , int width) : Object(cx,cy,stroke,fill,width), _radius(radius) {} // Constructor

void Circle::draw(QPainter &painter){
    
    if (_stroke_color == "none"){
        painter.setPen(Qt::NoPen);                       // Disable stroke drawing
    }
    else{
        QColor sColor(_stroke_color.c_str()); 
        QPen pen(sColor, _stroke_width);                 // Create pen with color/width
        painter.setPen(pen);                             // Apply pen to painter
    }
    
    if (_fill_color == "none"){
        painter.setBrush(Qt::NoBrush);                   // Disable fill drawing
    }
    else{
        QColor fColor(_fill_color.c_str());
        painter.setBrush(fColor);                        // Apply fill color
    }

    painter.drawEllipse(QPoint(_x, _y), _radius, _radius); // Draw circle geometry
}

std::string Circle::toSVG() {
    std::stringstream out;
    out << "<circle cx=\"" << _x << "\" cy=\"" << _y << "\" r=\"" << _radius << "\" stroke=\"" << _stroke_color << "\" fill=\"" << _fill_color << "\" stroke-width=\"" << _stroke_width << "\" />"; // Format SVG tag
    return out.str();                                    // Return SVG string
}

bool Circle::isHit(const QPoint& p){
    long long dx = p.x()- _x; long long dy = p.y()- _y;  // Calculate deltas
    return (dx*dx+dy*dy <= 1LL*_radius*_radius);         // Check if point inside
}

void Circle::resize(int dx , int dy){
    _radius += dx;                                       // Adjust radius by delta
    if (_radius < 5) _radius = 5;                        // Enforce minimum size
}
QRect Circle::getBounds() const {
    return QRect(_x - _radius , _y - _radius , _radius*2 , _radius * 2); // Return bounding box
}

std::unique_ptr<Object> Circle::clone() const {
        return std::make_unique<Circle>(*this);          // Create deep copy
}