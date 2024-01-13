#include <ios>
#include <iostream>
#include <fstream>
#include <bitset>
#include <string>
#include <map>

std::ifstream fin("nume_fisier.in");
std::fstream executabil("executabil.bin", std::ios::in | std::ios::binary);

struct componenetsRegisters {
    int val;
    bool isInt = false;

    std::string String;
    bool isString = false;
};

std::map<int, std::string> codeRegisters;
std::map<std::string, componenetsRegisters> registers;

int main () {
    while (!fin.eof ()) {
        std::string line;
        std::getline (fin, line);

        int i = 0;
        while (!isalpha (line[i]) && line[i] != NULL) i += 1;

        std::string currentRegister = "";
        while (line[i] != NULL && line[i] != ' ')
            currentRegister += line[i], i += 1;

        i += 1;

        line.erase (0, i);
        registers[currentRegister].String = line;
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
        if (code == 4) {
            char currentRegister;
            executabil.read (&currentRegister, 1);

            int constant;
            executabil.read (reinterpret_cast<char *> (&constant), 4);

            registers[codeRegisters[(int) currentRegister]].val = constant;
            registers[codeRegisters[(int) currentRegister]].isInt = true;
        }
        executabil.read (&code, 1);
    }

    std::cout << registers["t0"].val;
    return 0;
}
