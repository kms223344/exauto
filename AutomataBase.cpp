#include "AutomataBase.hpp"

std::string Auto::examine_bytestring(std::string s)
{
    std::string ret;
    for(auto c : s)
    {
        if(32 <= c && c <= 126 && c != 92)
            ret += c;
        else if(c == 92) ret += "\\\\";
        else ret += "\\"+std::format("{:02X}", c); 
        // else ret += "{"+std::to_string(uint8_t(c))+"}";
    }
    return ret;
}
