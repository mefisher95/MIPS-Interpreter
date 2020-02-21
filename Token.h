#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <limits>


//=============================================================================
// __CONVERT_LOWER__
//=============================================================================
// converts all characters in a string to their lowercase variants
//=============================================================================
void __convert_lower__(std::string &string)
{
    for (int i = 0; i < string.size(); ++i)
    {
        if (string[i] >= 'A' &&  string[i] <= 'Z') string[i] += 32;
    }
}

//=============================================================================
// __IS_DELIM__
//=============================================================================
// checks to see if the inputed character is found within the set of inputed
// delimiters. 
//=============================================================================
bool __is_delim__(const char c, const std::string &delim)
{
    for (int i = 0; i < delim.size(); ++i)
    {
        if (delim[i] == c) return true;
    }  
    return false;
}

//=============================================================================
// STRING_STREAM_INPUT
//=============================================================================
// takes input from the stream of max character size of 1024. Returns this
// stream input as a std::string to the user
//=============================================================================
std::string string_stream_input()
{
    const int MAX = 1024;
    char string[MAX];

    std::cout << ">>> ";
    std::cin.getline(string, MAX);

    if (std::cin.eof()) return string;
    if (std::cin.fail() || std::cin.bad())
    {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
    }

    return string;
}

//==============================================================================
// INPUT_TOKENS
//==============================================================================
// Takes in input paramaters of enforce_lower(bool) and deliminator(std::String)
// enforce_lower has a default value of true, and controls the auto conversion
// of user inputs into common lowercase. deliminiator has a default
// value of " ," and controls the points at which a token is created
//==============================================================================
std::vector< std::string > input_tokens(std::string & rawinput,
                                        const bool enforce_lower=true, 
                                        const std::string &deliminator = " ,()")
{
    // vector of tokens to be returned to the user
    std::vector< std::string> tokens; 

    // create a new working string, and recieve input from the stream.
    // If enforce_lower is true, then convert the entire string to lowercase
    // characters
    rawinput = string_stream_input();
    
    if (enforce_lower) __convert_lower__(rawinput);
    
    std::string string = rawinput;

    //=========================================================================
    // tokenize the string using the deliminators
    //=========================================================================

    // run through the string using string_pos, searching for any instances of
    // defined delimiters
    for (int string_pos = 0; string_pos < string.size(); ++string_pos)
    {
        // create a new token if a delimiter is found, and append it to the 
        // token vector. Adjust string to have a new starting position after
        // the sequence of delimiters
        if (__is_delim__(string[string_pos], deliminator))
        {
            std::string token = string.substr(0, string_pos);

            tokens.push_back(token);

            int space = 0;
            for (int k = string_pos; k < string.size(); ++k)
            {
                if (__is_delim__(string[k], deliminator)) ++space;
                else break;
            }

            string = string.substr(string_pos + space, string.size()
                                   - token.size());
            string_pos = 0;
        }
    
        // if the last element of the string and no delimiters have been found,
        // append the entire remaining string to the token vector
        if (string_pos == string.size() - 1) tokens.push_back(string);
    }

    return tokens;
}




//=============================================================================
// DEBUG PRINT FOR TOKEN VECTORS
//=============================================================================
std::ostream &operator<<(std::ostream &cout, const std::vector< std::string > &string)
{
    bool debug = false;

        for (int i = 0; i < string.size(); ++i)
        {
            if (string[i] == "\n") cout << string[i];
            else if (debug) cout << '[' << string[i] << ']';
            else cout << string[i] << ' ';
        }

    return cout;
}

std::vector< std::string > operator+(const std::vector< std::string> &str1,
                                     const std::vector< std::string> &str2)
{
    std::vector< std::string > ret;
    for(int i = 0; i < str1.size(); ++i)
    {
        ret.push_back(str1[i]);
    }
    
    if (ret.size() > 0) ret.push_back("\n");
    
    for(int i = 0; i < str2.size(); ++i)
    {
        ret.push_back(str2[i]);
    }
    return ret;
}

std::vector< std::string > operator+=(std::vector< std::string > &str1,
                                      std::vector< std::string > &str2)
{
    return (str1 + str2);
}

std::string recomposite(std::vector< std::string > tokens,
                 std::string delim = "\"")
{
    std::string new_string = "";
    int start = 0;
    for (int i = 0; i < tokens.size(); ++i)
    {
        if (__is_delim__(tokens[i][0], delim))
        {
            start = i;
            break;
        }
    }

    for (int i = start; i < tokens.size(); ++i)
    {
        if (tokens[i].front() == '\"')
        {
            if (tokens[i].back() == '\"')
            {
                new_string += tokens[i].substr(1, tokens[i].size() - 2);
            }
            else
            {
                new_string += tokens[i].substr(1, tokens[i].size() - 1) + ' ';
            }
        }
        else  
        {
            if (tokens[i].back() == '\"')
            {
                new_string += tokens[i].substr(0, tokens[i].size() - 1);
            }
            else
            {
                new_string += tokens[i] + ' ';
            }
        }
    }
    return new_string;
}

#endif