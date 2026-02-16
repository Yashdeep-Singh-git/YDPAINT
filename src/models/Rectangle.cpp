#include "include/models/Rectangle.h"
#include <string>
#include <sstream>
#include <QPainter>

Rectangle::Rectangle(int x , int y , int width , int height , const std::string &stroke_color , const std::string &fill_color , int stroke_width , int rx , int ry) : 
    Object(x , y , stroke_color , fill_color , stroke_width) , _width(width) , _height(height) , _rx(rx) , _ry(ry) {} // Init properties

void Rectangle::draw(QPainter &painter){
    if (_stroke_color == "none"){
        painter.setPen(Qt::NoPen);                       // Disable stroke drawing
    }
    else {    
        QColor sColor(_stroke_color.c_str()); 
        QPen pen(sColor, _stroke_width);                 // Create pen with color/width
        painter.setPen(pen);                             // Apply pen to painter
    }
    
    if (_fill_color == "none"){
        painter.setBrush(Qt::NoBrush);                   // Disable fill drawing
    }
    else {
        QColor fColor(_fill_color.c_str());
        painter.setBrush(fColor);                        // Apply fill color
    }
    painter.drawRoundedRect(_x, _y , _width, _height , _rx , _ry); // Draw rect with rounded corners
}
std::string  Rectangle::toSVG(){
    std::stringstream out;
    out << "<rect x=\"" << _x << "\" y=\"" << _y << "\" width=\"" << _width << "\" height=\"" << _height << "\" rx=\"" << _rx << "\" ry=\"" << _ry << "\" stroke=\"" << _stroke_color << "\" fill=\"" << _fill_color << "\" stroke-width=\"" << _stroke_width << "\" />";
    return out.str();                                    // Return SVG string
}
bool Rectangle::isHit(const QPoint& qp){
    int tx = qp.x(); int ty = qp.y();
    return (tx>=_x && ty>=_y && tx<=_x+_width && ty<=_y+_height); // Check AABB collision
}
void Rectangle::resize(int dx , int dy){
    _width += dx; _height += dy;                         // Adjust dimensions
    if (_width < 10) _width = 10;                        // Enforce min width
    if (_height < 10) _height = 10;                      // Enforce min height
}
QRect Rectangle::getBounds() const{
    return QRect(_x , _y , _width , _height);            // Return bounding box
}
void Rectangle::setCornerRadius(int r) {
        _rx = r;                                         // Set X radius
        _ry = r;                                         // Set Y radius
}
std::unique_ptr<Object> Rectangle::clone() const {
    return std::make_unique<Rectangle>(*this);           // Deep copy
}