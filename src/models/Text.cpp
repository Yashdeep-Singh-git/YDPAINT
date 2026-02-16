#include "include/models/Text.h" // Ensure file name matches your include
#include <sstream>
#include <QFontMetrics>

// Constructor
// We map the 'color' argument to 'fill' because usually text in SVG is filled, not stroked.
TextObj::TextObj(int x, int y, std::string text, std::string color, int fontSize)
    : Object(x, y, "none", color, 1), _text(text), _fontSize(fontSize) 
{
}

void TextObj::draw(QPainter &painter) {
    // 1. Setup Font
    QFont font("Arial", _fontSize);
    painter.setFont(font);

    // 2. Setup Color (Text uses Pen color in Qt)
    // Note: In our model, we stored text color in _fill_color
    QColor color(_fill_color.c_str());
    painter.setPen(color);

    // 3. Draw
    // Note: Qt draws text from the baseline-left.
    painter.drawText(_x, _y, QString::fromStdString(_text));
}

std::string TextObj::toSVG() {
    std::stringstream out;
    // SVG text structure
    out << "<text x=\"" << _x << "\" y=\"" << _y 
        << "\" font-family=\"Arial\" "
        << "font-size=\"" << _fontSize << "\" "
        << "fill=\"" << _fill_color << "\">" 
        << _text 
        << "</text>";
    return out.str();
}

// Simple bounding box hit test for text
bool TextObj::isHit(const QPoint& p) {
    // Estimate width/height since we don't have QFontMetrics without a GUI context easily.
    // Average char width is often ~0.6 * fontSize.
    int estimatedWidth = _text.length() * _fontSize * 0.6;
    int estimatedHeight = _fontSize;

    // SVG Text anchor (x,y) is usually the Bottom-Left (Baseline)
    // So the "box" goes UP from y.
    
    // Check X
    bool xHit = (p.x() >= _x) && (p.x() <= _x + estimatedWidth);
    
    // Check Y (Point must be between baseline and top of text)
    // y - height <= p.y <= y
    bool yHit = (p.y() >= (_y - estimatedHeight)) && (p.y() <= _y);

    return xHit && yHit;
}


void TextObj::resize(int dx, int dy) {
    // Dragging right/down increases font size
    int change = (std::abs(dx) > std::abs(dy)) ? dx : dy;
    
    // Scale down the speed (it grows too fast otherwise)
    _fontSize += (change / 2); 
    
    if (_fontSize < 5) _fontSize = 5;
}

QRect TextObj::getBounds() const {
    // Estimate dimensions
    int estimatedWidth = _text.length() * _fontSize * 0.6;
    int estimatedHeight = _fontSize;

    // Text draws UP from baseline, so y is the bottom
    return QRect(_x, _y - estimatedHeight, estimatedWidth, estimatedHeight);
}
std::unique_ptr<Object> TextObj::clone() const {
    return std::make_unique<TextObj>(*this);
}