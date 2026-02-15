#include "include/models/Circle.h"
#include <string>
#include <sstream>
#include <cmath>

Circle::Circle(int cx , int cy , int radius , const std::string &stroke , const std::string &fill , int width) : Object(cx,cy,stroke,fill,width), _radius(radius) {}

void Circle::draw(QPainter &painter){
    
    if (_stroke_color == "none"){
        painter.setPen(Qt::NoPen);
    }
    else{
        QColor sColor(_stroke_color.c_str()); 
        QPen pen(sColor, _stroke_width);
        painter.setPen(pen);
    }
    
    if (_fill_color == "none"){
        painter.setBrush(Qt::NoBrush);
    }
    else{
        QColor fColor(_fill_color.c_str());
        painter.setBrush(fColor);
    }

    painter.drawEllipse(QPoint(_x, _y), _radius, _radius);
}

std::string Circle::toSVG() {
    std::stringstream out;
    out << "<circle cx=\"" << _x << "\" cy=\"" << _y << "\" r=\"" << _radius << "\" stroke=\"" << _stroke_color << "\" fill=\"" << _fill_color 
    << "\" stroke-width=\"" << _stroke_width << "\" />";
    return out.str();
}

bool Circle::isHit(const QPoint& p){
    long long dx = p.x()- _x; long long dy = p.y()- _y;
    return (dx*dx+dy*dy <= 1LL*_radius*_radius);
}

void Circle::resize(int dx , int dy){
    _radius += dx;
    if (_radius < 5) _radius = 5;
}
QRect Circle::getBounds() const {
    return QRect(_x - _radius , _y - _radius , _radius*2 , _radius * 2);
}