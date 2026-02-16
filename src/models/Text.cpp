#include "include/models/Text.h" 
#include <sstream>
#include <QFontMetrics>

TextObj::TextObj(int x, int y, std::string text, std::string color, int fontSize)
    : Object(x, y, "none", color, 1), _text(text), _fontSize(fontSize) 
{
}

void TextObj::draw(QPainter &painter) {
    QFont font("Arial", _fontSize);
    painter.setFont(font);
    QColor color(_fill_color.c_str());
    painter.setPen(color);
    painter.drawText(_x, _y, QString::fromStdString(_text));
}

std::string TextObj::toSVG() {
    std::stringstream out;
    out << "<text x=\"" << _x << "\" y=\"" << _y << "\" font-family=\"Arial\" "<< "font-size=\"" << _fontSize << "\" "
        << "fill=\"" << _fill_color << "\">" << _text << "</text>";
    return out.str();
}

bool TextObj::isHit(const QPoint& p) {
    int estimatedWidth = _text.length() * _fontSize * 0.6;
    int estimatedHeight = _fontSize;
    bool xHit = (p.x() >= _x) && (p.x() <= _x + estimatedWidth);
    bool yHit = (p.y() >= (_y - estimatedHeight)) && (p.y() <= _y);

    return xHit && yHit;
}


void TextObj::resize(int dx, int dy) {
    int change = (std::abs(dx) > std::abs(dy)) ? dx : dy;
    _fontSize += (change / 2); 
    
    if (_fontSize < 5) _fontSize = 5;
}

QRect TextObj::getBounds() const {
    int estimatedWidth = _text.length() * _fontSize * 0.6;
    int estimatedHeight = _fontSize;
    return QRect(_x, _y - estimatedHeight, estimatedWidth, estimatedHeight);
}
std::unique_ptr<Object> TextObj::clone() const {
    return std::make_unique<TextObj>(*this);
}