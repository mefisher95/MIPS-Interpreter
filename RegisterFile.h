#ifndef REGISTERFILE_H
#define REGISTERFILE_H

#include <iostream>
#include <iomanip>
#include <string>
#include <climits>

class Error{};


class RegisterFile
{
    public:

    unsigned int &operator[](int reg) { return r[reg]; }
    unsigned int operator[](int reg) const { return r[reg]; }
    unsigned int &HI() { return high; }
    unsigned int HI() const { return high; }
    unsigned int &LO() { return low; }
    unsigned int LO() const { return low; }
    unsigned int &PC() { return program_counter; }
    unsigned int PC() const { return program_counter; }
    void run()
    { 
        is_running_ = true; 
        for (int i = 0; i < 32; ++i)
        {
            r[i] = 0;
        }
        high = 0;
        low = 0;
        program_counter = 268435456;
    }
    void clear()
    {
        is_running_ = false; 
        for (int i = 0; i < 32; ++i)
        {
            r[i] = 0;
        }
        high = 0;
        low = 0;
        program_counter = 268435456;
    }

    void stop() { is_running_ = false; }
    void inc_pc() { program_counter += 4; } 
    bool is_running() { return is_running_; }
    int register_(std::string reg) const 
    {
        if (reg == "$2" || reg == "$v0") return r[2];
        else if (reg == "$4" || reg == "$a0") return r[4];
        else if (reg == "$5" || reg == "$a1") return r[5];
        else if (reg == "$6" || reg == "$a2") return r[6];
        else if (reg == "$7" || reg == "$a3") return r[7];
        else if (reg == "$8" || reg == "$t0") return r[8];
        else if (reg == "$9" || reg == "$t1") return r[9];
        else if (reg == "$10" || reg == "$t2") return r[10];
        else if (reg == "$11" || reg == "$t3") return r[11];
        else if (reg == "$12" || reg == "$t4") return r[12];
        else if (reg == "$13" || reg == "$t5") return r[13];
        else if (reg == "$14" || reg == "$t6") return r[14];
        else if (reg == "$15" || reg == "$t7") return r[15];
        else if (reg == "$16" || reg == "$s0") return r[16];
        else if (reg == "$17" || reg == "$s1") return r[17];
        else if (reg == "$18" || reg == "$s2") return r[18];
        else if (reg == "$19" || reg == "$s3") return r[19];
        else if (reg == "$20" || reg == "$s4") return r[20];
        else if (reg == "$21" || reg == "$s5") return r[21];
        else if (reg == "$22" || reg == "$s6") return r[22];
        else if (reg == "$23" || reg == "$s7") return r[23];
        else if (reg == "$24" || reg == "$t8") return r[24];
        else if (reg == "$25" || reg == "$t9") return r[25];
        else if (reg == "$28" || reg == "$gp") return r[28];
        else if (reg == "$29" || reg == "$sp") return r[29];
        else if (reg == "$30" || reg == "$fp") return r[30];
        else if (reg == "$31" || reg == "$ra") return r[31];
        else if (reg == "$hi") return HI();
        else if (reg == "$lo") return LO();
        else if (reg == "$pc") return PC(); 
        else throw Error();
    }


    void print() const
    {
        
        std::cout << "\n|======================================================================|\n"
                  << "| REGISTER FILE:                                                       |\n"
                  << "|======================================================================|\n"
                  << '|' << std::setfill(' ') << std::setw(7) << "$v0: " << std::setw(10) << r[2]
                  << std::setw(7) << "$v1: " << std::setw(10) << r[3]
                  << std::setw(7) << "$HI: " << std::setw(10) << HI()
                  << std::setw(7) << "$LO: " << std::setw(10) << LO() << "  |"
                  << "\n|                                                                      |\n"
                  << '|' << std::setw(7) << "$a0: " << std::setw(10) << r[4]
                  << std::setw(7) << "$a1: " << std::setw(10) << r[5]
                  << std::setw(7) << "$a2: " << std::setw(10) << r[6]
                  << std::setw(7) << "$a3: " << std::setw(10) << r[7] << "  |"
                  << "\n|                                                                      |\n"
                  << '|' << std::setw(7) << "$t0: " << std::setw(10) << r[8]
                  << std::setw(7) << "$t1: " << std::setw(10) << r[9]
                  << std::setw(7) << "$t2: " << std::setw(10) << r[10]
                  << std::setw(7) << "$t3: " << std::setw(10) << r[11] << "  |"
                  << '\n'
                  << '|' << std::setw(7) << "$t4: " << std::setw(10) << r[12]
                  << std::setw(7) << "$t5: " << std::setw(10) << r[13]
                  << std::setw(7) << "$t6: " << std::setw(10) << r[14]
                  << std::setw(7) << "$t7: " << std::setw(10) << r[15] << "  |"
                  << '\n'
                  << '|' << std::setw(7) << "$t8: " << std::setw(10) << r[24]
                  << std::setw(7) << "$t9: " << std::setw(10) << r[25] << "                                    |"
                  << "\n|                                                                      |\n"
                  << '|' << std::setw(7) << "$s0: " << std::setw(10) << r[16]
                  << std::setw(7) << "$s1: " << std::setw(10) << r[17]
                  << std::setw(7) << "$s2: " << std::setw(10) << r[18]
                  << std::setw(7) << "$s3: " << std::setw(10) << r[19] << "  |"
                  << '\n'
                  << '|' << std::setw(7) << "$s4: " << std::setw(10) << r[20]
                  << std::setw(7) << "$s5: " << std::setw(10) << r[21]
                  << std::setw(7) << "$s6: " << std::setw(10) << r[22]
                  << std::setw(7) << "$s7: " << std::setw(10) << r[23] << "  |"
                  << "\n|                                                                      |\n"
                  << '|' << std::setw(7) << "$gp: " << std::setw(10) << r[28]
                  << std::setw(7) << "$sp: " << std::setw(10) << r[29]
                  << std::setw(7) << "$fp: " << std::setw(10) << r[30]
                  << std::setw(7) << "$ra: " << std::setw(10) << r[31] << "  |"
                  << "\n|                                                                      |\n"
                  << '|' << std::setw(7) << "$pc: " << std::setw(10) << PC()
                  << "                                                     |\n"
                  << "|======================================================================|\n"
                  << std::endl;
    }

    private:
    unsigned int r[32] = { 0 };
    unsigned int high = 0;
    unsigned int low = 0;
    unsigned int program_counter = 268435456;
    bool is_running_ = false;
};


static RegisterFile reg;

std::ostream &operator<<(std::ostream &cout, const RegisterFile &r)
{
    r.print();
    return cout;
}

#endif