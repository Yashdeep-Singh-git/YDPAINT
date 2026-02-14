#include "include/models/Rectangle.h"
#include <string>
#include <sstream>
#include <QPainter>

Rectangle::Rectangle(int x , int y , int width , int height , const std::string &stroke_color , const std::string &fill_color , int stroke_width , int rx , int ry) : 
    Object(x , y , stroke_color , fill_color , stroke_width) , _width(width) , _height(height) , _rx(rx) , _ry(ry) {}

void Rectangle::draw(QPainter &painter){
    if (_stroke_color == "none"){
        painter.setPen(Qt::NoPen);
    }
    else {    
        QColor sColor(_stroke_color.c_str()); 
        QPen pen(sColor, _stroke_width);
        painter.setPen(pen);
    }
    
    if (_fill_color == "none"){
        painter.setBrush(Qt::NoBrush);
    }
    else {
        QColor fColor(_fill_color.c_str());
        painter.setBrush(fColor);
    }
    
    painter.drawRoundedRect(_x, _y , _width, _height , _rx , _ry);
}

std::string  Rectangle::toSVG(){
    std::stringstream out;
    out << "<rect x=\"" << _x << "\" y=\"" << _y << "\" width=\"" << _width << "\" height=\"" << _height << "\" rx=\"" << _rx << "\" ry=\"" << _ry << "\" stroke=\"" << _stroke_color 
    << "\" fill=\"" << _fill_color << "\" stroke-width=\"" << _stroke_width << "\" />";
    return out.str();
}

bool Rectangle::isHit(const QPoint& qp){
    int tx = qp.x(); int ty = qp.y();
    return (tx>=_x && ty>=_y && tx<=_x+_width && ty<=_y+_height);
}