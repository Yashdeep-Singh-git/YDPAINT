#include "include/models/Text.h" 
#include <sstream>
#include <QFontMetrics>

TextObj::TextObj(int x, int y, std::string text, std::string color, int fontSize) : Object(x, y, "none", color, 1), _text(text), _fontSize(fontSize) {} // Constructor

void TextObj::draw(QPainter &painter) {
    QFont font("Arial", _fontSize);
    painter.setFont(font);                                       // Set drawing font
    QColor color(_stroke_color.c_str());
    painter.setPen(color);                                       // Set text color
    painter.drawText(_x, _y, QString::fromStdString(_text));     // Render text at position
}

std::string TextObj::toSVG() {
    std::stringstream out;
    out << "<text x=\"" << _x << "\" y=\"" << _y << "\" font-family=\"Arial\" "<< "font-size=\"" << _fontSize << "\" " << "fill=\"" << _fill_color << "\">" << _text << "</text>";
    return out.str();                                            // Return SVG representation
}

bool TextObj::isHit(const QPoint& p) {
    int estimatedWidth = _text.length() * _fontSize * 0.6;       // Approximate text width
    int estimatedHeight = _fontSize;                             // Approximate text height
    bool xHit = (p.x() >= _x) && (p.x() <= _x + estimatedWidth); // Check X bounds
    bool yHit = (p.y() >= (_y - estimatedHeight)) && (p.y() <= _y); // Check Y bounds (above baseline)
    return xHit && yHit;                                         // Return intersection
}

void TextObj::resize(int dx, int dy) {
    int change = (std::abs(dx) > std::abs(dy)) ? dx : dy;        // Use dominant mouse movement
    _fontSize += (change / 2);                                   // Scale font size
    
    if (_fontSize < 5) _fontSize = 5;                            // Enforce minimum size
}

QRect TextObj::getBounds() const {
    int estimatedWidth = _text.length() * _fontSize * 0.6;       // Re-calculate width
    int estimatedHeight = _fontSize;                             // Re-calculate height
    return QRect(_x, _y - estimatedHeight, estimatedWidth, estimatedHeight); // Return bounding rect
}

std::unique_ptr<Object> TextObj::clone() const {
    return std::make_unique<TextObj>(*this);                     // Deep copy
}