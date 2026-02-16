#ifndef FREEHAND_HPP
#define FREEHAND_HPP

#include "include/models/Object.h"
#include <vector>
#include <QPointF> 

class Freehand : public Object {
    std::vector<QPointF> _points; 

public:
    Freehand(const std::vector<QPoint>& points, std::string stroke, int width);
    
    void draw(QPainter &painter) override;
    std::string toSVG() override;
    bool isHit(const QPoint& p) override;
    void resize(int dx, int dy) override; 
    QRect getBounds() const override;
    void move(int dx, int dy) override;

    void addPoint(const QPoint& p) ;
    std::unique_ptr<Object> clone() const override ;
};

#endif