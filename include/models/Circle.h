#ifndef CIRCLE_HPP
#define CIRCLE_HPP

#include "include/models/Object.h"

class Circle : public Object {
    protected:
    int _radius;

    public:
    Circle(int cx, int cy , int radius , const std::string &stroke , const std::string &fill , int width)    
    // ~Circle() 
    void draw(Qpainter &qpainter) override;
    std::string toSVG() override;
    bool isHit(Qpoint q) override;
};

#endif