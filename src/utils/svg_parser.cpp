#include "include/utils/svg_parser.h"
#include "include/models/Circle.h"
#include "include/models/Rectangle.h"
#include "include/models/Line.h"
#include "include/models/Hexagon.h"
#include "include/models/Text.h"
#include "include/models/Freehand.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>

std::vector<QPoint> parsePoints(const std::string& pointsStr) {
    std::vector<QPoint> points;
    std::string cleanStr = pointsStr;
    std::replace(cleanStr.begin(), cleanStr.end(), ',', ' ');
    std::stringstream ss(cleanStr);
    double x, y;
    while (ss >> x >> y) { 
        points.emplace_back((int)(x), (int)(y));
    }
    return points;
}

std::string SVGParser::getAttribute(const std::string& tag, const std::string& key) {
    std::string searchKey = key + "=\"";
    size_t startPos = tag.find(searchKey);
    if (startPos == std::string::npos) return "0"; 
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
        if (line.find("circle") != std::string::npos) {
            int cx = std::stoi(getAttribute(line, "cx"));
            int cy = std::stoi(getAttribute(line, "cy"));
            int r  = std::stoi(getAttribute(line, "r"));
            std::string fill = getAttribute(line, "fill");
            std::string stroke = getAttribute(line, "stroke");
            int sw = std::stoi(getAttribute(line, "stroke-width"));
            objects.push_back(std::make_unique<Circle>(cx, cy, r, stroke, fill, sw));
        }
        
        else if (line.find("rect") != std::string::npos) {
            int x = std::stoi(getAttribute(line, "x"));
            int y = std::stoi(getAttribute(line, "y"));
            int w = std::stoi(getAttribute(line, "width"));
            int h = std::stoi(getAttribute(line, "height"));
            int rx = std::stoi(getAttribute(line , "rx"));
            int ry = std::stoi(getAttribute(line , "ry"));
            std::string fill = getAttribute(line, "fill");
            std::string stroke = getAttribute(line, "stroke");
            int sw = std::stoi(getAttribute(line, "stroke-width"));
            objects.push_back(std::make_unique<Rectangle>(x, y, w, h, stroke, fill, sw , rx , ry));
        }
        else if (line.find("hexagon") != std::string::npos) {
            int cx = std::stoi(getAttribute(line, "cx"));
            int cy = std::stoi(getAttribute(line, "cy"));
            int r  = std::stoi(getAttribute(line, "r"));
            std::string fill = getAttribute(line, "fill");
            std::string stroke = getAttribute(line, "stroke");
            int sw = std::stoi(getAttribute(line, "stroke-width"));
            double rotation = std::stod(getAttribute(line , "rotation"));
            objects.push_back(std::make_unique<Hexagon>(cx, cy, r, rotation, stroke, fill, sw));
        }
        else if (line.find("polyline") != std::string::npos) {
            std::string pointsStr = getAttribute(line, "points");
            std::vector<QPoint> points = parsePoints(pointsStr);
            std::string stroke = getAttribute(line, "stroke");
            int sw = std::stoi(getAttribute(line, "stroke-width"));
            if (!points.empty()) {
                objects.push_back(std::make_unique<Freehand>(points, stroke, sw));
            }
        }
        else if (line.find("text") != std::string::npos) {
            int x = std::stoi(getAttribute(line, "x"));
            int y = std::stoi(getAttribute(line, "y"));
            int fontSize = std::stoi(getAttribute(line, "font-size"));
            std::string fill = getAttribute(line, "fill"); 
            size_t startPos = line.find(">");
            size_t endPos = line.find("</text>");
            std::string content = "";
            if (startPos != std::string::npos && endPos != std::string::npos) {
                content = line.substr(startPos + 1, endPos - startPos - 1);
            }
            objects.push_back(std::make_unique<TextObj>(x, y, content, fill, fontSize));
        }
        else if (line.find("line") != std::string::npos) {
            int x1 = std::stoi(getAttribute(line, "x1"));
            int y1 = std::stoi(getAttribute(line, "y1"));
            int x2 = std::stoi(getAttribute(line, "x2"));
            int y2 = std::stoi(getAttribute(line, "y2"));
            std::string stroke = getAttribute(line, "stroke");
            int sw = std::stoi(getAttribute(line, "stroke-width"));
            objects.push_back(std::make_unique<Line>(x1, y1, x2, y2, stroke, sw));
        }
    }
    return objects;
}
void SVGParser::saveFile(const std::string& filepath, const std::vector<std::unique_ptr<Object>>& shapes) {
    std::ofstream outFile(filepath);
    if (!outFile.is_open()) {
        std::cerr << "Error: Could not open file for writing: " << filepath << std::endl;
        return;
    }
    outFile << "<svg width=\"800\" height=\"600\" xmlns=\"http://www.w3.org/2000/svg\">\n";
    for (const auto& shape : shapes) {
        outFile << "  " << shape->toSVG() << "\n";
    }
    outFile << "</svg>";
    outFile.close();
    std::cout << "File saved successfully to: " << filepath << std::endl;
}