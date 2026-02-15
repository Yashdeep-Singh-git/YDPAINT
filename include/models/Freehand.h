#ifndef FREEHAND_HPP
#define FREEHAND_HPP
#include "include/models/Object.h"

class Freehand : public Object {
    std::vector<QPoint> _points;
public:
    Freehand(const std::vector<QPoint>& points, std::string stroke, int width);
    void draw(QPainter &painter) override;
    std::string toSVG() override;
    bool isHit(const QPoint& p) override; // Hard (check distance to all segments)
    
    // Helper to add points while drawing
    void addPoint(const QPoint& p) { _points.push_back(p); }

    void resize(int dx , int dy) override;
    QRect getBounds() const override;
};

#endif