// #include "include/utils/parse_circle.h"

// std::unique_ptr<Object> in_parse_circle(const std::string &s){
//     std::int cx , cy , r , width; std::string stroke , fill; 
//     std::vector<std::string> v_out(6 , std::string "");
//     std::int ct = 0; std::bool on = 0;
//     std::int ti = -1; 
//     std::string ts ="";
//     std::int ind = 0; 
//     std::int n = s.size();
//     while (ind < n){
//         if (ct) {
//             if (s[ind]=="\""){
//                 if (on){
//                     on = 0;
//                     v_out[ct-1] = ts;
//                     ct = 0;
//                     ts = "";
//                 }
//                 else{
//                     on = 1;
//                 }
//                 ind++;
//                 continue;
//             }
//             else{
//                 if (on) ts.pb(s[ind]);
//                 ind++; continue;
//             }
//         }
//         if (s[ind] == 'c'){
//             if (s[ind+1]=='x'){
//                 ct = 1;
//             }
//             if (s[ind+1]=='y'){
//                 ct = 2;
//             }
//             ind += 2;
//             continue;
//         }
//         if (s[ind] == 'r'){
//             ct = 3; ind++;
//             continue;
//         }
//         if (s[ind]=='s'){
//             if (s[ind+6] == '-'){
//                 ct = 4;
//                 ind += 7;
//             }
//             else{
//                 ct = 5; ind += 6;
//             }
//             continue;
//         }
//         if (s[ind]=='f'){
//             ct = 6;
//             ind += 4;
//             continue;
//         }
//     }
//     cx = std::stoi(v_out[0]);
//     cy = std::stoi(v_out[1]);
//     r = std::stoi(v_out[2]);
//     width = std::stoi(v_out[3]);
//     stroke = v_out[4];
//     fill = v_out[5];

//     auto out = new Circle(cx , cy , r , width , stroke , fill);
//     return out;
// }