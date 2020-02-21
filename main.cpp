#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <map>

#include "Token.h"
#include "RegisterFile.h"
#include "UnitCO.h"
#include "TextCell.h"
#include "DataCell.h"
#include "LabelCell.h"


//=============================================================================
// OVERRIDE
//=============================================================================
// defined override functions that allows a user to interact with the emulator
// without affecting the data. None of these commands will be recorded in the 
// program log
//=============================================================================

bool override(std::vector< std::string > &tokens,
              std::vector< std::string > &Prog_log,
              std::vector< TextCell > &text_stack,
              std::vector< DataCell > &data_stack, 
              std::map< std::string, unsigned int > &label_stack, 
              int &current_stack,
              std::vector< std::string > &load_list)
{
    std::fstream file;

    if (tokens[0] == "\\quit") 
    {
        current_stack = -1;
        return true;
    }
    else if (tokens[0] == "\\print")
    {
        if (tokens[1] == ".text") std::cout << text_stack;
        if (tokens[1] == ".data") std::cout << data_stack;
        if (tokens[1] == "labels" || tokens[1] == "label") std::cout << label_stack;
        if (tokens[1] == "registers" || tokens[1] == "reg") std::cout << reg;
        if (tokens[1] == "program" || tokens[1] == "prog") std::cout << Prog_log << std::endl;
        return true;
    }
    else if (tokens[0] == "\\text")
    {
        std::cout << text_stack;
        
        if (current_stack != 0)
        {
            current_stack = 0;
            std::vector< std::string > text_temp;
            text_temp.push_back(".text\n");
            Prog_log = Prog_log + text_temp;
        }

        return true;
    }
    else if (tokens[0] == "\\data")
    {
        std::cout << data_stack;
        if (current_stack != 1)
        {
            current_stack = 1;
            std::vector< std::string > data_temp;
            data_temp.push_back(".data\n");
            Prog_log = Prog_log + data_temp;
        }
        return true;
    }
    else if (tokens[0] == "\\label")
    {
        std::cout << label_stack;
        return true;
    }
    else if (tokens[0] == "\\registers" || tokens[0] == "\\reg")
    { 

        if (tokens.size() > 2)
        if (tokens[1].front() == '$')
        {
            try { reg.register_(tokens[1]); }
            catch ( Error & e ) 
            { 
                std::cout << "INVALID REGISTER" << std::endl;
                return true;
            }

            std::cout << tokens[1] << "         OUTPUT >>> " << reg.register_(tokens[1]) << std::endl;
            return true;
        }
        std::cout << reg;
        return true;
    }
    else if (tokens[0] == "\\ls")
    {
        std::cout << text_stack;
        std::cout << data_stack;
        std::cout << label_stack;
        std::cout << reg;
        return true;
    }
    else if (tokens[0] == "\\run") 
    {
        reg.run();
        reg.PC() = label_stack["main"];
        return true;
    }
    else if (tokens[0] == "\\stop") 
    {
        reg.stop();
        return true;
    }
    else if (tokens[0] == "\\clear")
    {
        label_stack.clear();
        data_stack.clear();
        text_stack.clear();
        Prog_log.clear();
        DataCell::last_data_addr = 536870912;
        TextCell::last_text_addr = 268435456;
        reg.clear();
        reg.PC() = DataCell::last_data_addr;
        Prog_log.push_back(".text");
        Prog_log.push_back("\n.globl main");
        Prog_log.push_back("\nmain:");
        label_stack.insert(std::pair< std::string, unsigned int >("main", TextCell::last_text_addr));
        return true;
    }
    else if (tokens[0] == "\\load")
    {
        std::ifstream input_file;
        input_file.open(tokens[1]);
        
        std::string load_line;
        while (getline(input_file, load_line))
        {
            load_list.push_back(load_line);
        }
        std::cout << load_list << std::endl;
        return true;
    }
    else if (tokens[0] == "\\save")
    {
        if (file.is_open())
        {
            for (int i = 0; i < tokens.size(); ++i)
            {
                file << tokens[i] << " ";
            }
            file.close();
        }
        else return false;
        return true;
    }
    else if (tokens[0] == "\\save-as")
    {
        std::ofstream output_file;
        output_file.open(tokens[1]);
        for (int i = 0; i < Prog_log.size(); ++i)
        {
            output_file << Prog_log[i] << " ";
        }
        output_file.close();
        return true;
    }
    else if (tokens[0] == "\\new_line")
    { 
        Prog_log.push_back("\n");
        return true;
    }
    else if (tokens[0] == "\\help")
    {
        std::cout << "\n|====================================================|\n"
                  << "| COMMANDS:                                          |\n"
                  << "|====================================================|\n"
                  << "| .text               - switches to text segment     |\n"
                  << "| .data               - switches to data segment     |\n"
                  << "| \\print program      - prints current progam log    |\n"
                  << "| \\print .text        - prints text segment          |\n"
                  << "| \\print .data        - prints data segment          |\n"
                  << "| \\print lables       - prints label segment         |\n"
                  << "| \\print registers    - prints register file         |\n"
                  << "| \\registers          - prints register file         |\n"
                  << "| \\reg                - prints register file         |\n"
                  << "| \\ls                 - prints all segments          |\n"
                  << "| \\text               - prints and switches to text  |\n"
                  << "| \\data               - prints and switches to data  |\n"
                  << "| \\label              - prints labesl                |\n"
                  << "| \\run                - runs current program         |\n"
                  << "| \\stop               - terminates current program   |\n"
                  << "| \\clear              - clears stack and registers   |\n"
                  << "| \\load 'filename'    - opens indicated file         |\n"
                  << "| \\save               - saves open file              |\n"
                  << "| \\save-as 'filename' - saves file as specific name  |\n"
                  << "| \\new_line           - adds a newline to program log|\n"
                  << "| \\quit               - terminates system            |\n"
                  << "| \\help               - displays command menu        |\n"
                  << "|====================================================|\n"
                  << std::endl;

        return true;  
    }

    return false;
}

void preamble()
{
    std::cout << "\n|====================================================|\n"
              << "| MIPS Interpreter v0.1                              |\n"
              << "| Michael Fisher, 2019                               |\n"
              << "|====================================================|\n"
              << "| Thank you for using this MIPS Interpreter          |\n"
              << "| To see a list of user commands, type \\help.        |\n"
              << "| To use the interpreter, simply begin typing.       |\n"
              << "|                                                    |\n"
              << "|====================================================|\n"
              << std::endl;
    
    return;
}


int main() 
{   
    preamble();
    // define program log
   std::vector< std::string > load_list;

    // define the memory stacks
    static std::vector< TextCell > text_stack;
    static std::vector< DataCell > data_stack;
    static std::map<std::string, unsigned int> label_stack; 

    std::vector< std::string > Prog_log;
    Prog_log.push_back(".text");
    Prog_log.push_back("\n.globl main");
    Prog_log.push_back("\nmain:");
    label_stack.insert(std::pair< std::string, unsigned int >("main", TextCell::last_text_addr));

    


    //current stack
    int current_stack = 0;

    while (1)
    {
        // displays the current working stack
        if (!reg.is_running())
        {
            switch (current_stack)
            {
                case -1:
                std::cout << "\nEXITING\n" << std::endl;
                return 0;

                case 0:
                std::cout << "CURRENT SEG: " << ".text ";
                break;

                case 1:
                std::cout << "CURRENT SEG: " << ".data ";
                break;

                case 2:
                std::cout << "CURRENT SEG: " << ".lable ";
                break;

                default:
                break;
            }
        }

        if (reg.is_running()  && (reg.PC() < TextCell::last_text_addr))
        {
            for (int i = 0; i < text_stack.size(); ++i)
            {
                if (text_stack[i].addr_int() == reg.PC())
                {
                    text_stack[i].evaluate(label_stack, data_stack);
                    break;
                }
            }
            reg.inc_pc();
            continue;
        }
        //define the input string and token vectors, take in user input
        std::string input_str;
        std::vector< std::string > tokens = input_tokens(input_str);
        //std::cout << input_str << std::endl;
        
        // if the user gives no input, ask for input again
        if (!tokens.size()) continue; 

        // non-recording commands, denoted by a '\' 
        if (override(tokens, Prog_log, text_stack, data_stack, label_stack, current_stack, load_list)) continue;
       
        // allow user to change working stack
        if (tokens[0] == ".text") 
        { 
            current_stack = 0; 
            Prog_log = Prog_log + tokens;
            continue; 
        }
        else if (tokens[0] == ".data") 
        { 
            current_stack = 1; 
            Prog_log = Prog_log + tokens;
            //Prog_log.push_back(input_str);
            continue; 
        }

        // if token is a label, add the label to the lable stack
        if (tokens[0].back() == ':')
        {
            std::string label = tokens[0].substr(0, tokens[0].size() - 1);
            unsigned int addr_int = (current_stack == 0 ? 
                                    TextCell::last_text_addr : DataCell::last_data_addr);

            label_stack.insert( std::pair< std::string, unsigned int >(label, addr_int));
        }

        // instruction handling in the text stack
        if (current_stack == 0) 
        {
            TextCell cell(tokens, input_str);
            try { cell.evaluate(label_stack, data_stack); }
            catch(CommandException &e) { e.print_error(); continue; }
            text_stack.push_back(cell);
        }
        // handling of data in the data stack
        else
        {
            try { Data_stack_evaluate(data_stack, tokens); }
            catch ( DataCellExcepiton &e ) { e.print_error(); continue; }
        }

        // append the user input to the program log
        Prog_log = Prog_log + tokens;

    }
    
   //write_file(global_record, input_file_name);
   return 0;
} 