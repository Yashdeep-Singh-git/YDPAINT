#include "include/models/Object.h"

Object::Object(int x , int y, const std::string &stroke_color , const std::string &fill_color , int stroke_width){
    _x = x; _y = y; _stroke_color = stroke_color; _fill_color = fill_color; _stroke_width = stroke_width; // Initialize properties
}
void Object::setpos(int x , int y){
    _x = x; _y = y;                                      // Set absolute position
}
void Object::setStroke_color(const std::string &stroke_color){
    _stroke_color = stroke_color;                        // Update stroke color
    
}
void Object::setFill_color(const std::string &fill_color){
    _fill_color = fill_color;                            // Update fill color
    
}
void Object::setStroke_width(int stroke_width){
    _stroke_width = stroke_width;                        // Update line thickness
}
void Object::move(int dx , int dy){
    _x += dx; _y += dy;                                  // Translate by delta
}

void Object::setStrokeColor(const std::string& color) { _stroke_color = color; } // CamelCase setter
void Object::setFillColor(const std::string& color) { _fill_color = color; }     // CamelCase setter
void Object::setStrokeWidth(int w) { _stroke_width = w; }                        // CamelCase setter
void Object::setCornerRadius(int r){}                                            // Base class no-op