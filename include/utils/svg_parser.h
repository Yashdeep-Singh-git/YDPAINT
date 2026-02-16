#ifndef SVG_PARSER_HPP
#define SVG_PARSER_HPP

#include <string>
#include <vector>
#include <memory>
#include "include/models/Object.h"

class SVGParser {
public:
    static std::vector<std::unique_ptr<Object>> parseFile(const std::string& filepath);
    static void saveFile(const std::string& filepath, const std::vector<std::unique_ptr<Object>>& shapes);

private:
    static std::string getAttribute(const std::string& tag, const std::string& key);
};

#endif