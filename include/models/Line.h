#ifndef LINE_HPP
#define LINE_HPP

#include "include/models/Object.h"

class Line : public Object {
    int _x2 , _y2;
public:
    Line(int x1, int y1, int x2, int y2, std::string stroke, int width);
    void draw(QPainter &painter) override;
    std::string toSVG() override;
    bool isHit(const QPoint& p) override; // Hardest part!

    void resize(int dx , int dy) override;
    QRect getBounds() const override;
    void move(int dx , int dy) override;
    std::unique_ptr<Object> clone() const override ;
};

#endif