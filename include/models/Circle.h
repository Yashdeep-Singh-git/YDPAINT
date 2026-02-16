#ifndef CIRCLE_HPP
#define CIRCLE_HPP
#include "include/models/Object.h"

class Circle : public Object {
    protected:
    int _radius;
    public:
    Circle(int cx, int cy , int radius , const std::string &stroke , const std::string &fill , int width);
    void draw(QPainter &qpainter) override;
    std::string toSVG() override;
    bool isHit(const QPoint& p) override;
    void resize(int dx , int dy) override;
    QRect getBounds() const override;
    std::unique_ptr<Object> clone() const override ;
};

#endif