#ifndef svg_parser_hpp
#define svg_parser_hpp

#include <vector>
#include <memory>
#include <string>

class Parser{
    private:

    std::vector<std::string> _breakdown(const std::string &s);

    std::string _type(const std::string &s);

    public:

    // API 1: READ - Converts a file on disk into C++ objects
    static std::vector<std::unique_ptr<GraphicsObject>> parseFile(const std::string& filename);

    // API 2: WRITE - Converts C++ objects back into a file on disk
    static void saveFile(const std::string& filename, const std::vector<std::unique_ptr<GraphicsObject>>& shapes);
};


#endif