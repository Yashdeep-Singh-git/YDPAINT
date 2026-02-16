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
    std::replace(cleanStr.begin(), cleanStr.end(), ',', ' ');      // Replace commas for easier parsing
    std::stringstream ss(cleanStr);
    double x, y;
    while (ss >> x >> y) {                                         // Extract coordinate pairs
        points.emplace_back((int)(x), (int)(y));                   // Store as integer points
    }
    return points;
}

std::string SVGParser::getAttribute(const std::string& tag, const std::string& key) {
    std::string searchKey = key + "=\"";                           // Format: key="
    int startPos = tag.find(searchKey);
    if (startPos == std::string::npos) return "0";                 // Return default if key missing
    startPos += searchKey.length();                                // Move to value start
    int endPos = tag.find("\"", startPos);                         // Find closing quote
    return tag.substr(startPos, endPos - startPos);                // Extract substring value
}

std::vector<std::unique_ptr<Object>> SVGParser::parseFile(const std::string& filepath) {
    std::vector<std::unique_ptr<Object>> objects;
    std::ifstream file(filepath);
    if (!file.is_open()) {                                         // Check file existence
        std::cerr << "Error: Could not open file " << filepath << std::endl;
        return objects;
    }
    std::string line;
    while (std::getline(file, line)) {                             // Process file line by line
        if (line.find("circle") != std::string::npos) {            // Parse circle element
            int cx = std::stoi(getAttribute(line, "cx"));
            int cy = std::stoi(getAttribute(line, "cy"));
            int r  = std::stoi(getAttribute(line, "r"));
            std::string fill = getAttribute(line, "fill");
            std::string stroke = getAttribute(line, "stroke");
            int sw = std::stoi(getAttribute(line, "stroke-width"));
            objects.push_back(std::make_unique<Circle>(cx, cy, r, stroke, fill, sw));
        }
        
        else if (line.find("rect") != std::string::npos) {         // Parse rectangle element
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
        else if (line.find("hexagon") != std::string::npos) {      // Parse custom hexagon element
            int cx = std::stoi(getAttribute(line, "cx"));
            int cy = std::stoi(getAttribute(line, "cy"));
            int r  = std::stoi(getAttribute(line, "r"));
            std::string fill = getAttribute(line, "fill");
            std::string stroke = getAttribute(line, "stroke");
            int sw = std::stoi(getAttribute(line, "stroke-width"));
            double rotation = std::stod(getAttribute(line , "rotation"));
            objects.push_back(std::make_unique<Hexagon>(cx, cy, r, rotation, stroke, fill, sw));
        }
        else if (line.find("polyline") != std::string::npos) {     // Parse freehand polyline
            std::string pointsStr = getAttribute(line, "points");
            std::vector<QPoint> points = parsePoints(pointsStr);
            std::string stroke = getAttribute(line, "stroke");
            int sw = std::stoi(getAttribute(line, "stroke-width"));
            if (!points.empty()) {
                objects.push_back(std::make_unique<Freehand>(points, stroke, sw));
            }
        }
        else if (line.find("text") != std::string::npos) {         // Parse text element
            int x = std::stoi(getAttribute(line, "x"));
            int y = std::stoi(getAttribute(line, "y"));
            int fontSize = std::stoi(getAttribute(line, "font-size"));
            std::string fill = getAttribute(line, "fill"); 
            int startPos = line.find(">");
            int endPos = line.find("</text>");
            std::string content = "";
            if (startPos != std::string::npos && endPos != std::string::npos) {
                content = line.substr(startPos + 1, endPos - startPos - 1); // Extract inner text
            }
            objects.push_back(std::make_unique<TextObj>(x, y, content, fill, fontSize));
        }
        else if (line.find("line") != std::string::npos) {         // Parse line element
            int x1 = std::stoi(getAttribute(line, "x1"));
            int y1 = std::stoi(getAttribute(line, "y1"));
            int x2 = std::stoi(getAttribute(line, "x2"));
            int y2 = std::stoi(getAttribute(line, "y2"));
            std::string stroke = getAttribute(line, "stroke");
            int sw = std::stoi(getAttribute(line, "stroke-width"));
            objects.push_back(std::make_unique<Line>(x1, y1, x2, y2, stroke, sw));
        }
    }
    return objects;                                                // Return list of parsed shapes
}

void SVGParser::saveFile(const std::string& filepath, const std::vector<std::unique_ptr<Object>>& shapes) {
    std::ofstream outFile(filepath);
    if (!outFile.is_open()) {                                      // Check write permissions
        std::cerr << "Error: Could not open file for writing: " << filepath << std::endl;
        return;
    }
    outFile << "<svg width=\"800\" height=\"600\" xmlns=\"http://www.w3.org/2000/svg\">\n"; // Header
    for (const auto& shape : shapes) {
        outFile << "  " << shape->toSVG() << "\n";                 // Delegate SVG generation to objects
    }
    outFile << "</svg>";                                           // Footer
    outFile.close();
    std::cout << "File saved successfully to: " << filepath << std::endl;
}