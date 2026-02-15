#ifndef FREEHAND_HPP
#define FREEHAND_HPP

#include "include/models/Object.h"
#include <vector>
#include <QPointF> // <--- NEW INCLUDE

class Freehand : public Object {
    // CHANGE: Use QPointF instead of QPoint
    std::vector<QPointF> _points; 

public:
    // Constructor still takes QPoint for compatibility with Canvas, 
    // but we will convert them internally.
    Freehand(const std::vector<QPoint>& points, std::string stroke, int width);
    
    void draw(QPainter &painter) override;
    std::string toSVG() override;
    bool isHit(const QPoint& p) override;
    void resize(int dx, int dy) override; 
    QRect getBounds() const override;
    void move(int dx, int dy) override;

    // Update helper to convert on the fly
    void addPoint(const QPoint& p) { 
        _points.push_back(QPointF(p)); 
    }
};

#endif