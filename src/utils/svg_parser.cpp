#include "include/utils/svg_parser.h"
#include "include/utils/parse_circle.h"

std::vector<std::string> Parser::_breakdown(string &s){
    std::vector<std::string> out;
    std::string temp = ""; std::bool on = 0;
    for (auto x:s){
        if (x=='<') {
            on = 1;
            temp = "";
            continue;
        }
        if (x=='>'){
            on = 0;
            out.pb(temp); temp = "";
            continue;
        }
        if (on) {
            temp.pb(x);
        }
    }
    return out;
}

std::string Parser::_type(const std::string &s){
    std::string out = ""; std::bool on = 0;
    for (auto x:s){
        if (on){
            if (x == " ") {
                on = 0;
                break;
            }
            out.pb(x)
        }
        else{
            if (x != " "){
                on = 1;
                out.pb(x);
            }
        }
    }
    return out;
}

static std::vector<std::unique_ptr<GraphicsObject>> parseFile(const std::string& filename){
    std::string complete = filename.open();
    std::vector<std::string> cmds = _breakdown(complete);
    std::vector<std::unique_ptr<GraphicsObject>> out;
    for (auto x:cmds){
        std::string tag = _type(x);
        if (x == "circle") out.pb(in_parse_circle(x));
    }
}