#ifndef parse_circle_hpp
#define parse_circle_hpp

#include "include/models/circle.h"
#include <memory>
#include <string>

std::unique_ptr<Object> in_parse_circle(const std::string &s);

#endif