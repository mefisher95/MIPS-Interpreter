#include <iostream>
#include <vector>
#include <string>
#include <map>
#include "UnitCO.h"
#include "Token.h"

#ifndef DATACELL_H
#define DATACELL_H

class DataCellExcepiton
{
    public:
    DataCellExcepiton(int err_code = 0)
    : error_code(err_code)
    {}

    const int error_code;

    void print_error()
    {
        switch(error_code)
        {
            case 0:
                std::cout << "DataCellException: INVALID COMMAND FOR DATA SEGMENT" 
                          << std::endl;
            break;

            case 1:
                std::cout << "DataCellException: INVALID DOT-CODE FOR DATA SEGMENT"
                          << std::endl;
            break;

            case 2:
                std::cout << "DataCellExcepiton: INVALID MEMORY ADDRESS FOR GET-WORD"
                          << std::endl;
            break;

            default:
                std::cout << "DataCellException: UNSPECIFIED DATA ERROR"
                          << std::endl;

        }
    }
};


class DataCell
{
    public:
    DataCell(std::string &word)
    {
        addr_int_ = last_data_addr;
        last_data_addr += 4;
        addr_hex_ = ui_to_hex(addr_int_);
        value_int_ = word_to_ui(word); // used in printing labels i think? 
        value_hex_ = ui_to_hex(value_int_); // these all may be depricated
        value_char_= ui_to_word(value_int_); // but we'll see
    }

    unsigned int addr_int() const { return addr_int_; }
    std::string addr_hex() const { return addr_hex_; }
    unsigned int value_int() const { return value_int_; }
    std::string value_hex() const { return value_hex_; }
    std::string value_char() const { return value_char_; }

    unsigned int &value_int() { return value_int_; }
    std::string &value_hex() { return value_hex_; }
    std::string &value_char() { return value_char_; }


    std::string get_word(const std::vector< DataCell > &data_stack, 
                         unsigned int addr_of_word) const
    {
        for (int i = 0; i < data_stack.size(); ++i)
        {
            if (data_stack[i].addr_int() == addr_of_word)
            return data_stack[i].value_char();
        }
        throw DataCellExcepiton(2);
    }

    void set_word(std::vector< DataCell > &data_stack,
                  unsigned int word_as_int)
    {
        value_int_ = word_as_int;
        value_hex_ = ui_to_hex(word_as_int);
        value_char_ = ui_to_word(word_as_int);
    }

    static unsigned int last_data_addr;
private:
    unsigned int addr_int_;
    std::string addr_hex_;
    unsigned int value_int_;
    std::string value_hex_;
    std::string value_char_;

    
};

unsigned int DataCell::last_data_addr = 536870912;


std::string get_word(std::vector< DataCell > data_stack, unsigned int addr_of_label, 
                     unsigned int addr_of_word)
{
    for (int i = 0; i < data_stack.size(); ++i)
    {
        if (data_stack[i].addr_int() == addr_of_label) return data_stack[i].get_word(data_stack, addr_of_word);
    }
    return std::string();
}

std::ostream &operator<<(std::ostream &cout, std::vector< DataCell > data_stack)
{
    std::cout << std::endl;
    const int n = 13;
    if (data_stack.size() < 1)
    {
        cout << "EMPTY SEGMENT" << std::endl;
        return cout;
    }
    cout << '|' << std::setfill('=') << std::setw(n * 5) << '|' << std::endl;
    cout << '|' << " DATA SEGMENT:                                                  |\n";

    cout << '|' << std::setfill('=') << std::setw(n * 5) << '|' << std::endl;
    cout << '|' << std::setfill(' ') << std::setw(n) << "addr (int)|"  
         << std::setw(n) << " addr (hex)|"
         << std::setw(n) << "value (int)|"
         << std::setw(n) << "value (hex)|"
         << std::setw(n) << "value (char)|\n"
         << '|' << std::setfill('=') << std::setw(n * 5) << "|"
         << std::setfill(' ') << std::endl;

    for (int i = 0; i < data_stack.size(); ++i)
    {
        cout << std::setfill(' ') 
             << '|'
             << std::setw(n - 1) << data_stack[i].addr_int() << '|'
             << std::setw(n - 1) << data_stack[i].addr_hex() << '|'
             << std::setw(n - 1) << data_stack[i].value_int() << '|'
             << std::setw(3) << data_stack[i].value_hex().substr(0, 2)
             << std::setw(3) << data_stack[i].value_hex().substr(2, 2)
             << std::setw(3) << data_stack[i].value_hex().substr(4, 2)
             << std::setw(3) << data_stack[i].value_hex().substr(6, 2)
             << "|"
             << std::setw(3) << data_stack[i].value_char()[0]
             << std::setw(3) << data_stack[i].value_char()[1]
             << std::setw(3) << data_stack[i].value_char()[2]
             << std::setw(3) << data_stack[i].value_char()[3]
             << "|\n";
    }

    cout << '|' << std::setfill('=') << std::setw(n * 5 + 1) << "|\n" << std::endl;

    return cout;
}

std::ostream &operator<<(std::ostream &cout, const DataCell &cell)
{
    const int n = 13;
    cout << '|' << std::setfill('=') << std::setw(n * 5 - 1) << '|' << std::endl;
    cout << '|' << std::setfill(' ') << std::setw(n) << "addr (int)|"  
         << std::setw(n) << " addr (hex)|"
         << std::setw(n) << "value (int)|"
         << std::setw(n) << "value (hex)|"
         << std::setw(n) << "value (char)|\n"
         << std::setfill('=') << std::setw(n * 5) << "="
         << std::setfill(' ') << std::endl;
    cout << std::setfill(' ') << std::setw(n) << cell.addr_int()
         << std::setw(n) << cell.addr_hex()
         << std::setw(n) << cell.value_int()
         << std::setw(n) << cell.value_hex()
         << std::setw(n) << cell.value_char()
         << std::endl;
    return cout;
}

void put_word_in_data_seg(std::vector< DataCell > &data_stack, unsigned int word_as_int, 
                          unsigned int base_addr_of_datacell, unsigned int offset)
{
    unsigned int addr_to_put_word = base_addr_of_datacell + offset;
    
    for (int i = 0; i < data_stack.size(); ++i)
    {
        if (data_stack[i].addr_int() == base_addr_of_datacell) 
        {
            data_stack[i].set_word(data_stack, word_as_int);
        }
    }
}

void Data_stack_evaluate(std::vector< DataCell > &data_stack,
                         std::vector< std::string > tokens)
{
    if (tokens[0].back() == ':')
    {
        for (int i = 0; i < tokens.size() - 1; ++i)
        {
            tokens[i] = tokens[i + 1];
        }
        tokens.resize(tokens.size() - 1);
    }

    if (tokens[0].front() == '.')
    {
        if (tokens[0] == ".asciiz")
        {
            std::vector< std::string > word_list = string_to_word(recomposite(tokens));

            for (int i = 0; i < word_list.size(); ++i)
            {
                data_stack.push_back(DataCell(word_list[i]));
            }
            return;
        }
        if (tokens[0] == ".word")
        {
            std::cout << "to be implemented" << std::endl;
            return;
        }
        throw DataCellExcepiton(1);
        
    }
    throw DataCellExcepiton(0);
}

#endif 