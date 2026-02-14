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

std::vector<QPoint> parsePoints(const std::string& pointsStr) {
    std::vector<QPoint> points;
    std::stringstream ss(pointsStr);
    std::string segment;

    // Split by space first: "100,100" "200,200"
    while (std::getline(ss, segment, ' ')) {
        if (segment.empty()) continue;
        
        // Now find the comma
        size_t commaPos = segment.find(',');
        if (commaPos != std::string::npos) {
            int x = std::stoi(segment.substr(0, commaPos));
            int y = std::stoi(segment.substr(commaPos + 1));
            points.emplace_back(x, y);
        }
    }
    return points;
}

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
            int rx = std::stoi(getAttribute(line , "rx"));
            int ry = std::stoi(getAttribute(line , "ry"));
            std::string fill = getAttribute(line, "fill");
            std::string stroke = getAttribute(line, "stroke");
            int sw = std::stoi(getAttribute(line, "stroke-width"));

            objects.push_back(std::make_unique<Rectangle>(x, y, w, h, stroke, fill, sw , rx , ry));
        }
        else if (line.find("line") != std::string::npos) {
            int x1 = std::stoi(getAttribute(line, "x1"));
            int y1 = std::stoi(getAttribute(line, "y1"));
            int x2 = std::stoi(getAttribute(line, "x2"));
            int y2 = std::stoi(getAttribute(line, "y2"));
            
            std::string stroke = getAttribute(line, "stroke");
            int sw = std::stoi(getAttribute(line, "stroke-width"));

            // Create Line Object
            objects.push_back(std::make_unique<Line>(x1, y1, x2, y2, stroke, sw));
        }

        else if (line.find("hexagon") != std::string::npos) {
            int cx = std::stoi(getAttribute(line, "cx"));
            int cy = std::stoi(getAttribute(line, "cy"));
            int r  = std::stoi(getAttribute(line, "r"));
            
            std::string fill = getAttribute(line, "fill");
            std::string stroke = getAttribute(line, "stroke");
            int sw = std::stoi(getAttribute(line, "stroke-width"));
            double rotation = std::stod(getAttribute(line , "rotation"));

            // Create Hexagon Object
            objects.push_back(std::make_unique<Hexagon>(cx, cy, r, rotation, stroke, fill, sw));
        }

        // 5. Check for Freehand (Polyline)
        else if (line.find("polyline") != std::string::npos) {
            std::string pointsStr = getAttribute(line, "points");
            std::vector<QPoint> points = parsePoints(pointsStr);
            
            std::string stroke = getAttribute(line, "stroke");
            int sw = std::stoi(getAttribute(line, "stroke-width"));
            
            // Create Freehand Object
            if (!points.empty()) {
                objects.push_back(std::make_unique<Freehand>(points, stroke, sw));
            }
        }

        // 6. Check for Text
        else if (line.find("text") != std::string::npos) {
            int x = std::stoi(getAttribute(line, "x"));
            int y = std::stoi(getAttribute(line, "y"));
            int fontSize = std::stoi(getAttribute(line, "font-size"));
            std::string fill = getAttribute(line, "fill"); // Text color is usually 'fill' in SVG
            
            // EXTRACT CONTENT: The text is between ">" and "</text>"
            // Example: <text ...>Hello World</text>
            size_t startPos = line.find(">");
            size_t endPos = line.find("</text>");
            
            std::string content = "";
            if (startPos != std::string::npos && endPos != std::string::npos) {
                // +1 to skip the '>'
                content = line.substr(startPos + 1, endPos - startPos - 1);
            }
            
            // Create Text Object
            objects.push_back(std::make_unique<TextObj>(x, y, content, fill, fontSize));
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
    // 1. Write Header
    outFile << "<svg width=\"800\" height=\"600\" xmlns=\"http://www.w3.org/2000/svg\">\n";
    // 2. Loop through all shapes and write their SVG string
    for (const auto& shape : shapes) {
        outFile << "  " << shape->toSVG() << "\n";
    }
    // 3. Write Footer
    outFile << "</svg>";
    
    outFile.close();
    std::cout << "File saved successfully to: " << filepath << std::endl;
}