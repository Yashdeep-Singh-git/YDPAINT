#ifndef TEXT_HPP
#define TEXT_HPP
#include "include/models/Object.h"
class TextObj : public Object {
    std::string _text;
    int _fontSize;
public:
    TextObj(int x, int y, std::string text, std::string color, int fontSize);
    void draw(QPainter &painter) override;
    std::string toSVG() override;
    bool isHit(const QPoint& p) override;
};

#endif