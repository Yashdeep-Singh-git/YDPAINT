#include "include\models\Rectangle.h"
#include <string>
#include <sstream>

Rectangle::Rectangle(int x , int y , int width , int height , const std::string &stroke_color , const std::string &fill_color , int stroke_width) : 
    Object(x , y , stroke_color , fill_color , stroke_width) , _width(width) , _height(height) {}

void Rectangle::draw(Qpainter &painter){
    QColor sColor(_stroke_color.c_str()); 
    QColor fColor(_fill_color.c_str());
    QPen pen(sColor, _stroke_width);
    painter.setPen(pen);
    painter.setBrush(fColor);
    painter.drawRect(QPoint(_x, _y), _width, _height);
}

std::string  Rectangle::toSVG(){
    std::stringstream out;
    out << "<rect x=\"" << _x << "\" y=\"" << _y << "\" width=\"" << _width << "\" height=\"" << _height << "\" stroke=\"" << _stroke_color 
    << "\" fill=\"" << _fill_color << "\" stroke-width=\"" << _stroke_width << "\" />";
    return out.str();
}

bool Rectangle::isHit(Qpoint qp){
    int tx = qp.x(); int ty = qp.y();
    return (tx>=_x && ty>=_y && tx<=_x+_width && ty<=_y+_height);
}