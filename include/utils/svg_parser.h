#ifndef SVG_PARSER_HPP
#define SVG_PARSER_HPP

#include <string>
#include <vector>
#include <memory>
#include "include/models/Object.h" // Your base class

class SVGParser {
public:
    // Main function: Reads a file and returns the list of objects
    static std::vector<std::unique_ptr<Object>> parseFile(const std::string& filepath);

private:
    // Helper: Extracts value from a string (e.g., finds 'cx="100"' -> returns "100")
    static std::string getAttribute(const std::string& tag, const std::string& key);
};

#endif