#ifndef RECTANGLE_HPP
#define RECTANGLE_HPP

#include "include/models/Object.h"

class Rectangle : public Object {
    protected:
    int _height , _width , _rx , _ry;

    public:
    Rectangle(int x , int y , int width , int height , const std::string &stroke_color , const std::string &fill_color , int stroke_width , int rx = 0, int ry = 0); 
    void draw(QPainter &qpainter) override;
    std::string toSVG() override;
    bool isHit(const QPoint& q) override;

    void resize(int dx , int dy) override;
    QRect getBounds() const override;
};

#endif