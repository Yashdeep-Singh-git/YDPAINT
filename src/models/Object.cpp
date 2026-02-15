#include "include/models/Object.h"

Object::Object(int x , int y, const std::string &stroke_color , const std::string &fill_color , int stroke_width){
    _x = x; _y = y; _stroke_color = stroke_color; _fill_color = fill_color; _stroke_width = stroke_width;
}
void Object::setpos(int x , int y){
    _x = x; _y = y;
}
void Object::setStroke_color(const std::string &stroke_color){
    _stroke_color = stroke_color;
    
}
void Object::setFill_color(const std::string &fill_color){
    _fill_color = fill_color;
    
}
void Object::setStroke_width(int stroke_width){
    _stroke_width = stroke_width;
}
void Object::move(int dx , int dy){
    _x += dx; _y += dy;
}