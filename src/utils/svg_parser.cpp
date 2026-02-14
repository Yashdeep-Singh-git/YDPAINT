#include "include/utils/svg_parser.h"
#include "include/models/Circle.h"
#include "include/models/Rectangle.h"
#include <fstream>
#include <iostream>
#include <sstream>

// Helper to extract values like cx="50"
std::string SVGParser::getAttribute(const std::string& tag, const std::string& key) {
    std::string searchKey = key + "=\"";
    size_t startPos = tag.find(searchKey);
    
    if (startPos == std::string::npos) return "0"; // Default if not found

    startPos += searchKey.length();
    size_t endPos = tag.find("\"", startPos);
    
    return tag.substr(startPos, endPos - startPos);
}

std::vector<std::unique_ptr<Object>> SVGParser::parseFile(const std::string& filepath) {
    std::vector<std::unique_ptr<Object>> objects;
    std::ifstream file(filepath);

    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filepath << std::endl;
        return objects;
    }

    std::string line;
    while (std::getline(file, line)) {
        // 1. Check for Circle
        if (line.find("circle") != std::string::npos) {
            int cx = std::stoi(getAttribute(line, "cx"));
            int cy = std::stoi(getAttribute(line, "cy"));
            int r  = std::stoi(getAttribute(line, "r"));
            std::string fill = getAttribute(line, "fill");
            std::string stroke = getAttribute(line, "stroke");
            int sw = std::stoi(getAttribute(line, "stroke-width"));

            objects.push_back(std::make_unique<Circle>(cx, cy, r, stroke, fill, sw));
        }
        
        // 2. Check for Rectangle
        else if (line.find("rect") != std::string::npos) {
            int x = std::stoi(getAttribute(line, "x"));
            int y = std::stoi(getAttribute(line, "y"));
            int w = std::stoi(getAttribute(line, "width"));
            int h = std::stoi(getAttribute(line, "height"));
            std::string fill = getAttribute(line, "fill");
            std::string stroke = getAttribute(line, "stroke");
            int sw = std::stoi(getAttribute(line, "stroke-width"));

            objects.push_back(std::make_unique<Rectangle>(x, y, w, h, stroke, fill, sw));
        }
    }

    return objects;
}