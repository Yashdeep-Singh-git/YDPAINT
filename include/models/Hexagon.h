#ifndef HEXAGON_HPP
#define HEXAGON_HPP
#include "include/models/Object.h"

class Hexagon : public Object {
    protected:
    int _radius; double _rotation;

    public:
    Hexagon(int cx, int cy , int radius , double rotation , const std::string &stroke , const std::string &fill , int width);
    void draw(QPainter &qpainter) override;
    std::string toSVG() override;
    bool isHit(const QPoint& p) override;

    void resize(int dx , int dy) override;
    QRect getBounds() const override;
};

#endif