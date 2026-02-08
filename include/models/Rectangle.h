#ifndef RECTANGLE_HPP
#define RECTANGLE_HPP

#include "include\models\Object.h"

class Rectangle : public Object {
    protected:
    int _height , _width;

    public:
    Rectangle(int x , int y , int width , int height , const std::string &stroke_color , const std::string &fill_color , int stroke_width); 
    void draw(QPainter &qpainter) override;
    std::string toSVG() override;
    bool isHit(Qpoint q) override;
};

#endif