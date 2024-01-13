#include <ios>
#include <iostream>
#include <fstream>
#include <bitset>
#include <vector>
#include <string>
#include <map>

std::ifstream fin("nume_fisier.in");
std::fstream executabil("executabil.bin", std::ios::in | std::ios::binary);

struct componenetsRegisters {
    int val;
    bool isInt = false;

    char car;
    bool isChar = false;

    std::vector<char> String;
    bool isString = false;
};

std::map<int, std::string> codeRegisters;
std::map<std::string, componenetsRegisters> registers;

int main () {
    while (!fin.eof ()) {
        std::string line;
        std::getline (fin, line);

        int i = 0;
        std::string currentRegister = "";
        while (line[i] != ' ')
            currentRegister += line[i], i += 1;

        i += 1;

        std::string dataType = "";
        while (line[i] != ' ')
            dataType += line[i], i += 1;

        i += 1;

        line.erase (0, i);

        std::vector<char> value;

        for (int i = 0; i < line.size (); i += 1)
            value.push_back (line[i]);

        value.push_back (0);

        if (dataType == "string") {
            registers[currentRegister].String = value;
            registers[currentRegister].isString = true;
        }
        //line.erase (0, i);
        //registers[currentRegister].String = line;
    }

    codeRegisters[0] = "zero";
    codeRegisters[1] = "ra";
    codeRegisters[2] = "sp";

    codeRegisters[16] = "t0";
    codeRegisters[17] = "t1";
    codeRegisters[18] = "t2";
    codeRegisters[19] = "t3";
    codeRegisters[20] = "t4";
    codeRegisters[21] = "t5";

    codeRegisters[33] = "s1";

    codeRegisters[48] = "a0";
    codeRegisters[49] = "a1";
    codeRegisters[50] = "a2";
    codeRegisters[51] = "a3";

    codeRegisters[64] = "ft0";
    codeRegisters[65] = "ft1";
    codeRegisters[66] = "ft2";
    codeRegisters[67] = "ft3";

    codeRegisters[80] = "fa0";
    codeRegisters[81] = "fa1";
    codeRegisters[82] = "fa2";


/*
    cod_functions[112] = "strlen";
    cod_functions[96] = "printf";
    cod_functions[97] = "scanf";
    cod_functions[113] = "cfunc";
*/

    char code;
    executabil.read (&code, 1);
    while (code != 66) {
        if (code == 4) { /// li
            char currentRegister;
            executabil.read (&currentRegister, 1);

            int constant;
            executabil.read (reinterpret_cast<char *> (&constant), 4);

            registers[codeRegisters[currentRegister]].isInt = true;
            registers[codeRegisters[currentRegister]].val = constant;
       } else if (code == 16) { /// add
            char currentRegister1;
            executabil.read (&currentRegister1, 1);
            int i = currentRegister1;

            char currentRegister2;
            executabil.read (&currentRegister2, 1);
            int j = currentRegister2;

            char currentRegister3;
            executabil.read (&currentRegister3, 1);
            int k = currentRegister3;


            if (registers[codeRegisters[j]].isString == true) {
                registers[codeRegisters[i]].isString = true;
                std::vector<char> v(registers[codeRegisters[j]].String);
                std::vector<char> res(v.begin () + registers[codeRegisters[k]].val, v.end ());
                registers[codeRegisters[i]].String = res;
            } else if (registers[codeRegisters[k]].isString == true) {
                registers[codeRegisters[i]].isString = true;
                std::vector<char> v(registers[codeRegisters[k]].String);
                std::vector<char> res(v.begin () + registers[codeRegisters[j]].val, v.end ());
                registers[codeRegisters[i]].String = res;
            }
        } else if (code == 1) { /// lb
            char currentRegister1;
            executabil.read (&currentRegister1, 1);

            int constant;
            executabil.read (reinterpret_cast<char *> (&constant), 4);

            char currentRegister2;
            executabil.read (&currentRegister2, 1);

            if (registers[codeRegisters[currentRegister2]].isString == true) {
                registers[codeRegisters[currentRegister1]].isChar = true;
                registers[codeRegisters[currentRegister1]].car = registers[codeRegisters[currentRegister2]].String[constant];
            }
        } else if (code == 80) { /// beqz
            char currentRegister;
            executabil.read (&currentRegister, 1);

            short constant;
            executabil.read (reinterpret_cast<char *> (&constant), 2);

            if ((int) registers[codeRegisters[currentRegister]].car == 0)
                executabil.seekp (constant, std::ios_base::beg);
        } else if (code == 17) { /// addi
            char currentRegister1;
            executabil.read (&currentRegister1, 1);

            char currentRegister2;
            executabil.read (&currentRegister2, 1);

            int constant;
            executabil.read (reinterpret_cast<char *> (&constant), 4);


            registers[codeRegisters[currentRegister1]].isInt = true;
            registers[codeRegisters[currentRegister1]].val = registers[codeRegisters[currentRegister2]].val + constant;
        } else if (code == 64) { /// j
            short constant;
            executabil.read (reinterpret_cast<char *> (&constant), 2);

            executabil.seekp (constant, std::ios_base::beg);
        } else if (code == 34) { /// mv
            char currentRegister1;
            executabil.read (&currentRegister1, 1);

            char currentRegister2;
            executabil.read (&currentRegister2, 1);


            if (registers[codeRegisters[currentRegister2]].isString == true) {
                registers[codeRegisters[currentRegister1]].isString = true;
                registers[codeRegisters[currentRegister1]].String = registers[codeRegisters[currentRegister2]].String;
            } else if (registers[codeRegisters[currentRegister2]].isInt == true) {
                registers[codeRegisters[currentRegister1]].isInt = true;
                registers[codeRegisters[currentRegister1]].val = registers[codeRegisters[currentRegister2]].val;
            }
        }
        executabil.read (&code, 1);
    }

    std::cout << registers["a0"].val;
    return 0;
}
