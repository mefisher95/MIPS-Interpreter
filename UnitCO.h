#ifndef UNITCO_H
#define UNITCO_H

#include <string>
#include <iomanip>

class UnitConversionException
{
    public:
    UnitConversionException(int error_code=0)
    : error_code_(error_code)
    {}

    int error_code_;
};

std::string ui_to_hex(unsigned int x, int size=8)
{
    std::stringstream stream;
    stream << std::setfill('0') << std::setw(size)
           << std::hex << x;
    return stream.str();
}

unsigned int hex_to_ui(std::string &hex)
{
    return std::stoul(hex, 0, 16);
}

std::string ui_to_word(unsigned int x)
{
    std::string hex = ui_to_hex(x);

    std::string byte0 = hex.substr(0,2);
    std::string byte1 = hex.substr(2,2);
    std::string byte2 = hex.substr(4,2);
    std::string byte3 = hex.substr(6,2);
    
    char char_byte0 = hex_to_ui(byte0);
    char char_byte1 = hex_to_ui(byte1);
    char char_byte2 = hex_to_ui(byte2);
    char char_byte3 = hex_to_ui(byte3);

    char str[4] = { char_byte0, char_byte1, char_byte2, char_byte3 };
    
    return std::string(str,4);
}

unsigned int word_to_ui(const std::string &word)
{
    std::string zero = ui_to_hex(word[0], 2);
    std::string one  = ui_to_hex(word[1], 2);
    std::string two  = ui_to_hex(word[2], 2);
    std::string three= ui_to_hex(word[3], 2);

    std::string ret = zero + one + two + three;
    return (hex_to_ui(ret));
}

unsigned int word_to_ui(char w, char o, char r, char d)
{
    std::string zero = ui_to_hex(w, 2);
    std::string one  = ui_to_hex(o, 2);
    std::string two  = ui_to_hex(r, 2);
    std::string three= ui_to_hex(d, 2);

    std::string ret = zero + one + two + three;
    return (hex_to_ui(ret));
}

std::vector< std::string > string_to_word(std::string string)
{
    std::vector< std::string > list;
    while(string.size() > 0)
    {
        if (string.size() < 4)
        {
            int make_up_size = 4 - string.size();
            char str[4];
            for (int i = 0; i < string.size(); ++i)
            {
                str[i] = string[i];
            }
            for (int i = 4 - make_up_size ; i < 4; ++i)
            {
                str[i] = 0;
            }
            
            list.push_back(str);
            list[list.size()- 1].resize(4);

            break;
        }
        else
        {
            list.push_back(string.substr(0, 4));
            string = string.substr(4, string.size() - 4);
        }
    }

    list.shrink_to_fit();
    return list;
}

#endif