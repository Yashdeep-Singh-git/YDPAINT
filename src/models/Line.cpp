#include "include/models/Line.h"
#include <sstream>
#include <cmath>
#include <algorithm> // for std::max/min

// Constructor
// Note: We pass "none" for fill because lines generally don't have fill
Line::Line(int x1, int y1, int x2, int y2, std::string stroke, int width)
    : Object(x1, y1, stroke, "none", width), _x2(x2), _y2(y2) 
{
}

void Line::draw(QPainter &painter) {
    // 1. Setup Pen (Stroke)
    if (_stroke_color == "none") {
        painter.setPen(Qt::NoPen);
    } else {
        QColor sColor(_stroke_color.c_str());
        QPen pen(sColor, _stroke_width);
        // CapStyle::RoundCap makes the ends look nicer
        pen.setCapStyle(Qt::RoundCap); 
        painter.setPen(pen);
    }

    // Lines don't use Brush (Fill), so we ignore _fill_color

    // 2. Draw
    painter.drawLine(_x, _y, _x2, _y2);
}

std::string Line::toSVG() {
    std::stringstream out;
    out << "<line x1=\"" << _x << "\" y1=\"" << _y 
        << "\" x2=\"" << _x2 << "\" y2=\"" << _y2 
        << "\" stroke=\"" << _stroke_color 
        << "\" stroke-width=\"" << _stroke_width << "\" />";
    return out.str();
}

bool Line::isHit(const QPoint& p) {
    // We treat the line as a "Capsule" or a rotated Rectangle.
    // Distance from Point P to Line Segment AB must be <= (Stroke Width / 2 + Buffer)
    
    double px = p.x();
    double py = p.y();
    double x1 = _x;
    double y1 = _y;
    double x2 = _x2;
    double y2 = _y2;

    // 1. Vector AB (Line Segment)
    double dx = x2 - x1;
    double dy = y2 - y1;
    
    if (dx == 0 && dy == 0) return false; // Line is a single point

    // 2. Project Point P onto Line AB to find the closest t
    // t = Dot(AP, AB) / Dot(AB, AB)
    double t = ((px - x1) * dx + (py - y1) * dy) / (dx * dx + dy * dy);

    // 3. Clamp t to the segment [0, 1]
    // If t < 0, closest point is A. If t > 1, closest point is B.
    t = std::max(0.0, std::min(1.0, t));

    // 4. Find closest point on the segment
    double closestX = x1 + t * dx;
    double closestY = y1 + t * dy;

    // 5. Calculate distance from P to Closest Point
    double distSq = (px - closestX) * (px - closestX) + (py - closestY) * (py - closestY);

    // 6. Check if distance is within stroke width (plus a 5px buffer for easy clicking)
    double tolerance = (_stroke_width / 2.0) + 5.0; 
    
    return distSq <= (tolerance * tolerance);
}


void Line::resize(int dx, int dy) {
    _x2 += dx;
    _y2 += dy;
}

QRect Line::getBounds() const {
    // QRect::normalized() handles the case where x2 < x1 or y2 < y1
    return QRect(_x, _y, _x2 - _x, _y2 - _y).normalized();
}