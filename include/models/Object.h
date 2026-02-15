#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <string>
#include <QPainter>
#include <QPoint>

class Object{
    protected:
    int _x,_y;
    std::string _stroke_color, _fill_color;
    int _stroke_width;

    public:
    Object(int x , int y, const std::string &stroke_color , const std::string &fill_color , int stroke_width) ;
    virtual ~Object() = default;
    virtual void draw(QPainter &qpainter) = 0;
    virtual std::string toSVG() = 0;
    virtual bool isHit(const QPoint& qpoint) = 0;

    virtual void resize(int dx , int dy) = 0;
    virtual QRect getBounds() const = 0;
    virtual void move(int dx , int dy);

    void setpos(int x , int y);
    void setStroke_color(const std::string &stroke_color);
    void setFill_color(const std::string &fill_color);
    void setStroke_width(const int stroke_width);
};


#endif