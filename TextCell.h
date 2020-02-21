#ifndef Cell_H
#define Cell_H

#include <vector>
#include <map>
#include <string>
#include <iostream>
#include "RegisterFile.h"
#include "UnitCO.h"
#include "DataCell.h"

class CommandException
{
    public:
    CommandException(int err_cd = 0)
    : error_code(err_cd)
    {}

    void print_error() const
    {
        std::cout << "COMMAND_EXCEPTION: ";

        switch (error_code)
        {
            case 0:
            std::cout << "INVALID STACK SEGMENT" << std::endl;
            return;

            case 1:
            std::cout << "INVALID OP CODE FOR Cell" << std::endl;
            return;

            case 2:
            std::cout << "INVALID REGISTER FOR OPERATION" << std::endl;
            return;

            case 3:
            std::cout << "INVALID ARGUMENT LIST" << std::endl;
            break;  

            default:
            std::cout << "UNDEFINED ERROR" << std::endl;
        }
    }

    const int error_code;
};


class TextCell 
{
    public:

    TextCell(std::vector< std::string > tokens, std::string rawinput)
    : tokens_(tokens), Cell_str_(rawinput)
    {
        addr_int_ = last_text_addr;
        addr_hex_ = ui_to_hex(addr_int_);
        last_text_addr += 4;
    }

    //=========================================================================
    // get methods for data from private variables
    //=========================================================================
    unsigned int addr_int() const { return addr_int_; }
    std::string addr_hex() const { return addr_hex_; }

    std::string value_char()  {return Cell_str_; }
    std::string raw_input() const { return Cell_str_; }
    std::vector< std::string > tokens() const { return tokens_; }
    
    //=========================================================================
    // user methods
    //=========================================================================
    void evaluate(std::map<std::string, unsigned int> label_stack, std::vector< DataCell> &data_stack)
    {
        __cmd_eval__(tokens_, label_stack, data_stack);
    }

    static unsigned int last_text_addr;

    private:
    
    //=========================================================================
    // private member variables
    //=========================================================================
    std::vector < std::string > tokens_;
    std::string Cell_str_; //raw input

    unsigned int addr_int_;
    std::string addr_hex_;
    
    static unsigned int last_data_addr;
    const static unsigned int starting_address = 268435456;

    //=========================================================================
    // private methods
    //=========================================================================

    int __op_code__(std::vector< std::string > tokens)
    {
        if (tokens[0] == "add") return 1;
        else if (tokens[0] == "sub") return 2;
        else if (tokens[0] == "addi") return 3;
        else if (tokens[0] == "mul") return 4;
        else if (tokens[0] == "mult") return 5;
        else if (tokens[0] == "div") return 6;
        
        else if (tokens[0] == "lw") return 7;
        else if (tokens[0] == "sw") return 8;
        else if (tokens[0] == "la") return 9;
        else if (tokens[0] == "li") return 10;
        else if (tokens[0] == "mfhi") return 11;
        else if (tokens[0] == "mflo") return 12;
        else if (tokens[0] == "move") return 13;

        else if (tokens[0] == "beq") return 14;
        else if (tokens[0] == "bne") return 15;
        else if (tokens[0] == "bgt") return 16;
        else if (tokens[0] == "bge") return 17;
        else if (tokens[0] == "blt") return 18;
        else if (tokens[0] == "ble") return 19;
        else if (tokens[0] == "slt") return 20;
        else if (tokens[0] == "slti") return 21;

        else if (tokens[0] == "j") return 22;
        else if (tokens[0] == "jr") return 23;
        else if (tokens[0] == "jal") return 24;
        
        else if (tokens[0] == "syscall") return 99;
        else if (tokens[0].back() == ':') return 69; 
        else throw CommandException(1);
    }

    int __register__(std::string reg)
    {
        if (reg == "$2" || reg == "$v0") return 2;
        else if (reg == "$4" || reg == "$a0") return 4;
        else if (reg == "$5" || reg == "$a1") return 5;
        else if (reg == "$6" || reg == "$a2") return 6;
        else if (reg == "$7" || reg == "$a3") return 7;
        else if (reg == "$8" || reg == "$t0") return 8;
        else if (reg == "$9" || reg == "$t1") return 9;
        else if (reg == "$10" || reg == "$t2") return 10;
        else if (reg == "$11" || reg == "$t3") return 11;
        else if (reg == "$12" || reg == "$t4") return 12;
        else if (reg == "$13" || reg == "$t5") return 13;
        else if (reg == "$14" || reg == "$t6") return 14;
        else if (reg == "$15" || reg == "$t7") return 15;
        else if (reg == "$16" || reg == "$s0") return 16;
        else if (reg == "$17" || reg == "$s1") return 17;
        else if (reg == "$18" || reg == "$s2") return 18;
        else if (reg == "$19" || reg == "$s3") return 19;
        else if (reg == "$20" || reg == "$s4") return 20;
        else if (reg == "$21" || reg == "$s5") return 21;
        else if (reg == "$22" || reg == "$s6") return 22;
        else if (reg == "$23" || reg == "$s7") return 23;
        else if (reg == "$24" || reg == "$t8") return 24;
        else if (reg == "$25" || reg == "$t9") return 25;
        else if (reg == "$28" || reg == "$gp") return 28;
        else if (reg == "$29" || reg == "$sp") return 29;
        else if (reg == "$30" || reg == "$fp") return 30;
        else if (reg == "$31" || reg == "$ra") return 31;
        else throw CommandException(2);
    }

    void __cmd_eval__(std::vector< std::string > tok, std::map< std::string, unsigned int > label_stack, 
                    std::vector< DataCell > &data_stack)
    {
        int cmd = __op_code__(tok);
        int dest_reg= -1;
        int reg0 = -1;
        int reg1 = -1;
        int intgr;

        switch (cmd)
        {

            case 0: // invalid op_code
            {
                throw CommandException(1);
                return;
            }

            case 1: // add
            {
                if (tok.size() < 4 || tok.size() > 4) throw CommandException(3);
                dest_reg = __register__(tok[1]);
                reg0 = __register__(tok[2]);
                reg1 = __register__(tok[3]);
                if (reg0 == -1 || reg1 == -1) throw CommandException(2);
                reg[dest_reg] = reg[reg0] + reg[reg1];
                return;
            }

            case 2: // sub
            {
                if (tok.size() < 4 || tok.size() > 4) throw CommandException(3);
                dest_reg = __register__(tok[1]);
                reg0 = __register__(tok[2]);
                reg1 = __register__(tok[3]);
                reg[dest_reg] = reg[reg0] - reg[reg1];
                return;
            }
            
            case 3: // addi
            {
                if (tok.size() < 4 || tok.size() > 4) throw CommandException(3);
                dest_reg = __register__(tok[1]);
                reg0 = __register__(tok[2]);
                intgr = std::stoi(tok[3]);
                reg[dest_reg] = reg[reg0] + intgr;
                return;
            }

            case 4: // mul
            {
                if (tok.size() < 4 || tok.size() > 4) throw CommandException(3);
                dest_reg = __register__(tok[1]);
                reg0 = __register__(tok[2]);
                reg1 = __register__(tok[3]);
                unsigned long long t0 = reg[reg0];
                unsigned long long t1 = reg[reg1];
                unsigned int temp = t0 * t1;
                reg[dest_reg] = temp;

                return;
            }

            case 5: // mult
            {
                if (tok.size() < 3 || tok.size() > 3) throw CommandException(3);
                reg0 = __register__(tok[1]);
                reg1 = __register__(tok[2]);
                unsigned long long t0 = reg[reg0];
                unsigned long long t1 = reg[reg1];
                unsigned long long temp = t0 * t1;
                reg.HI() = temp >> 32;
                reg.LO() = temp;

                return;
            }

            case 6: //div
            {
                if (tok.size() < 3 || tok.size() > 3) throw CommandException(3);
                reg0 = __register__(tok[1]);
                reg1 = __register__(tok[2]);
                reg.HI() = reg[reg0] % reg[reg1];
                reg.LO() = reg[reg0] / reg[reg1];

                return;
            }
 
            case 7: // lw
            {
                if (tok.size() < 4 || tok.size() > 4) throw CommandException(3);
                dest_reg = __register__(tok[1]);
                
                unsigned int offset = stoi(tok[2]);
                
                unsigned int from_reg = __register__(tok[3]);
                unsigned int label_addr = reg[from_reg];
                unsigned int addr_of_word = label_addr + offset;

                std::string word = get_word(data_stack, label_addr, addr_of_word);

                reg[dest_reg] = word_to_ui(word);
                return;
            }

            case 8:// sw
            {
                if (tok.size() < 4 || tok.size() > 4) throw CommandException(3);
                dest_reg = __register__(tok[3]);
                unsigned int base_addr_of_datacell = reg[dest_reg];
                unsigned int offset = stoi(tok[2]);
                unsigned int from_reg = __register__(tok[1]);
                unsigned int word_addr = reg[from_reg];
                std::string word = get_word(data_stack, word_addr, word_addr + offset);
                unsigned int word_as_int = word_to_ui(word);

                put_word_in_data_seg(data_stack, word_as_int, base_addr_of_datacell, offset);
                return;
            }

            case 9: // la
            {
                if (tok.size() < 3 || tok.size() > 3) throw CommandException(3);
                // get label name
                std::string label_name = tok[2];
                dest_reg = __register__(tok[1]);

                // look up label name in Label stack and get address of cell
                for (auto element : label_stack)
                {
                    if (label_name == element.first) reg[dest_reg] = element.second;                        
                }
                return;
            }

            case 10: // li 
            {
                if (tok.size() < 3 || tok.size() > 3) throw CommandException(3);
                dest_reg = __register__(tok[1]);
                intgr = std::stoi(tok[2]);
                reg[dest_reg] = intgr;
                return;
            }

            case 11: //mfhi
            {
                if (tok.size() < 2 || tok.size() > 2) throw CommandException(3);
                dest_reg = __register__(tok[1]);
                reg[dest_reg] = reg.HI();
                return;
            }
            
            case 12: //mflo
            {
                if (tok.size() < 2 || tok.size() > 2) throw CommandException(3);
                dest_reg = __register__(tok[1]);
                reg[dest_reg] = reg.LO();
                return;
            }

            case 13:// move
            {
                if (tok.size() < 3 || tok.size() > 3) throw CommandException(3);
                dest_reg = __register__(tok[1]);
                reg0 = __register__(tok[2]);
                reg[dest_reg] = reg[reg0];
                return;
            }
      
            case 14: // branch on equal
            {
                if (tok.size() < 4 || tok.size() > 4) throw CommandException(3);
                reg0 = __register__(tok[1]);
                reg1 = __register__(tok[2]);
                if (reg[reg0] == reg[reg1]) reg.PC() = label_stack[tok[3]];
                return;
            }

            case 15: // branch on not equal
            {
                if (tok.size() < 4 || tok.size() > 4) throw CommandException(3);
                reg0 = __register__(tok[1]);
                reg1 = __register__(tok[2]);
                if (reg[reg0] != reg[reg1]) reg.PC() = label_stack[tok[3]];
                return;
            }

            case 16: // branch on greater than
            {
                if (tok.size() < 4 || tok.size() > 4) throw CommandException(3);
                reg0 = __register__(tok[1]);
                reg1 = __register__(tok[2]);
                if (reg[reg0] > reg[reg1]) reg.PC() = label_stack[tok[3]];
                return;
            }

            case 17: // branch on greater than or equal
            {
                if (tok.size() < 4 || tok.size() > 4) throw CommandException(3);
                reg0 = __register__(tok[1]);
                reg1 = __register__(tok[2]);
                if (reg[reg0] >= reg[reg1]) reg.PC() = label_stack[tok[3]];
                return;
            }

            case 18: // branch on less than
            {
                if (tok.size() < 4 || tok.size() > 4) throw CommandException(3);
                reg0 = __register__(tok[1]);
                reg1 = __register__(tok[2]);
                if (reg[reg0] < reg[reg1]) reg.PC() = label_stack[tok[3]];
                return;
            }

            case 19: // branch on less than or equal
            {
                if (tok.size() < 4 || tok.size() > 4) throw CommandException(3);
                reg0 = __register__(tok[1]);
                reg1 = __register__(tok[2]);
                if (reg[reg0] <= reg[reg1]) reg.PC() = label_stack[tok[3]];
                return;
            }

            case 20: // set on less than
            {
                if (tok.size() < 4 || tok.size() > 4) throw CommandException(3);
                dest_reg = __register__(tok[1]);
                reg0 = __register__(tok[2]);
                reg1 = __register__(tok[3]);

                if (reg[reg0] < reg[reg1]) reg[dest_reg] = 1;
                else reg[dest_reg] = 0;

                return;
            }

            case 21: // set on less than immediate
            {
                if (tok.size() < 4 || tok.size() > 4) throw CommandException(3);
                dest_reg = __register__(tok[1]);
                reg0 = __register__(tok[2]);
                intgr = std::stoi(tok[3]);
                
                if (reg[reg0] < intgr) reg[dest_reg] = 1;
                else reg[dest_reg] = 0;

                return;
            }

            case 22: // jump
            {
                if (tok.size() < 2 || tok.size() > 2) throw CommandException(3);
                reg.PC() = label_stack[tok[1]];
                return;
            }

            case 23: // jump register
            {
                if (tok.size() < 2 || tok.size() > 2) throw CommandException(3);
                reg0 = __register__(tok[1]);
                reg.PC() = reg[reg0];
                return;
            }

            case 24: // jump and link
            {
                if (tok.size() < 2 || tok.size() > 2) throw CommandException(3);
                reg[31] = reg.PC();
                reg.PC() = label_stack[tok[1]];
                return;
            }
            
            // if first token is a label, execute remainder of command
            case 69: // labels, noice ;)
            {
                std::vector< std::string >::const_iterator first = tok.begin() + 1;
                std::vector< std::string >::const_iterator last = tok.end();

                if (tok.size() < 2) return;
                std::vector< std::string > new_tokens(first, last);
                __cmd_eval__(new_tokens, label_stack, data_stack);
                return;
            }

            case 99: // syscall
            {
                // store values in reg[2]
                int v0_value = reg[2];
                int a0_value = reg[4];
                int a1_value = reg[5];
                std::string temp = "";
                switch(v0_value)
                {
                    case 1: // print integer
                        //$a0 = value
                        std::cout << "syscall 1   OUTPUT >>> " << a0_value << std::endl;
                    return;

                    case 4: // print string
                    // $a0 = address of string
                    std::cout << "syscall 4   OUTPUT >>> " << a0_value << std::endl;
                    for (int i = 0; i < data_stack.size(); ++i)
                    {
                        for (int k = 0; k < data_stack[i].value_char().size(); ++k)
                        {
                            if (data_stack[i].value_char()[k] == 0) break;
                            temp += data_stack[i].value_char()[k];
                        }
                    }
                    std::cout << temp << std::endl;
                    return;

                    case 5: // read interger
                    // $v0 = value read
                    std::cout << "syscall 5    INPUT >>> ";
                    std::cin >> reg[2];
                    std::cout << '\n';
                    return;

                    case 8: // read string
                    {
                        // $a0 = address where string to be stored
                        std::string string;
                        std::cout << "syscall 8    INPUT >>> ";
                        std::getline(std::cin, string);
                        std::vector< std::string > word_list = string_to_word(string);

                        for (int i = 0; i < word_list.size(); ++i)
                        {
                            if (a0_value < DataCell::last_data_addr)
                            {
                                put_word_in_data_seg(data_stack, word_to_ui(word_list[i]),a0_value, 0);
                            }
                            else 
                            {
                                data_stack.push_back(DataCell(word_list[i]));
                            }
                            a0_value += 4;
                        }

                        return;
                    }

                    case 9: // memory allocation
                    {
                        //$a0 = number of bytes of storage desired
                        //$v0 = address of block
                        reg[2] = DataCell::last_data_addr; 
                        for (int i = 0; i < a0_value / 4; ++i)
                        {
                                std::string word("\0\0\0\0");
                                data_stack.push_back(DataCell(word));
                        }

                        std::cout << "syscall 9 ALLOCATE >>> MEMORY ALLOCATED" << std::endl;
                    }

                    return;

                    case 10: // exit
                        std::cout << "syscall 10    EXIT >>> TERMINATING MIPS PROGRAM" << std::endl;
                        reg.stop();
                    return;

                    case 11: // print character
                    // $a0 = integer	
                    std::cout << "syscall 11 >>> " << ui_to_word(a0_value) << std::endl;

                    return;

                    case 12: // read character
                    // char in $v0
                    std::cout << "syscall 12 >>> ";
                    std::cin >> reg[2];

                    return;
                }
            }
        } 
    }
};

unsigned int TextCell::last_text_addr = 268435456;



std::ostream &operator<<(std::ostream & cout, std::vector< TextCell > map)
{
    std::cout << std::endl;
    const int n = 13;
    if (map.size() < 1 ) 
    {
        cout << "EMPTY SEGMENT" << std::endl;
        return cout;
    }

    cout << '|' << std::setfill('=') << std::setw(n * 5) << '|' << std::endl;
    cout << '|' << " TEXT SEGMENT:                                                  |\n";

    cout << '|' << std::setfill('=') << std::setw(n * 5) << '|' << std::endl;
    cout << '|' << std::setfill(' ') << std::setw(n) << "addr (int)|"    
        << std::setw(n) << " addr (hex)|" 
        << std::setw(3*n + 1) << "value (char) |\n"
        << '|' << std::setfill('=') << std::setw(n * 5) << "|" 
        << std::setfill(' ' ) << std::endl;

    for (int i = 0; i < map.size(); ++i)
    {
        cout << '|' << std::setw(n-1) << map[i].addr_int() << '|'
                << std::setw(n-1) << map[i].addr_hex() << '|'
                << std::setw(3*n-1) << map[i].value_char() << '|'
                << std::endl;
    }

    cout << '|' << std::setfill('=') << std::setw(n * 5 + 1) << "|\n" << std::endl;
    return cout;
}
   
#endif 