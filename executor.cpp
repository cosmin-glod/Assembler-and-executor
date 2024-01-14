#include <ios>
#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
#include <string>
#include <cmath>
#include <map>

const int nMax = 1e6 + 1;

std::ifstream fin("nume_fisier.in");
std::fstream executabil("executabil.bin", std::ios::in | std::ios::binary);

struct componenetsRegisters {
    int val;
    bool isInt = false;

    char car;
    bool isChar = false;

    int index;
    int whichString;
    bool isPointer = false;
    bool isNull = false;
};

std::map<int, std::string> codeRegisters;
std::map<std::string, componenetsRegisters> registers;

std::vector<componenetsRegisters> Stack;
std::vector<std::string> strings;

int main () {
    while (!fin.eof ()) {
        std::string currentRegister; fin >> currentRegister;

        std::string dataType; fin >> dataType;

        std::string content; std::getline (fin, content);

        while (content[0] == ' ')
            content.erase (0, 1);

        if (dataType == "string") {
            content.resize (1000);
            strings.emplace_back (content);

            registers[currentRegister].index = 0;
            registers[currentRegister].whichString = strings.size () - 1;
            registers[currentRegister].isPointer = true;
        } else if (dataType == "int") {
            registers[currentRegister].val = std::stoi (content);
            registers[currentRegister].isInt = true;
        }
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
    codeFunctions[112] = "strlen";
    codeFunctions[96] = "printf";
    codeFunctions[97] = "scanf";
    codeFunctions[113] = "cfunc";
*/

    char code;
    executabil.read (&code, 1);
    while (code != 66) {
        //std::cout << (int) code << ' ';
        if (code == 4) { /// li
            char currentRegister;
            executabil.read (&currentRegister, 1);

            int constant;
            executabil.read (reinterpret_cast<char *> (&constant), 4);


            registers[codeRegisters[currentRegister]].isInt = true;
            registers[codeRegisters[currentRegister]].isChar = registers[codeRegisters[currentRegister]].isPointer = false;

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

            if (registers[codeRegisters[j]].isPointer == true) {
                registers[codeRegisters[i]].isPointer = true;
                registers[codeRegisters[i]].isChar = registers[codeRegisters[i]].isInt = false;

                registers[codeRegisters[i]].index = registers[codeRegisters[k]].val;
                registers[codeRegisters[i]].whichString = registers[codeRegisters[j]].whichString;
            } else if (registers[codeRegisters[k]].isPointer == true) {
                registers[codeRegisters[i]].isPointer = true;
                registers[codeRegisters[i]].isChar = registers[codeRegisters[i]].isInt = false;

                registers[codeRegisters[i]].index = registers[codeRegisters[j]].val;
                registers[codeRegisters[i]].whichString = registers[codeRegisters[k]].whichString;
            }
        } else if (code == 1) { /// lb
            char currentRegister1;
            executabil.read (&currentRegister1, 1);

            int constant;
            executabil.read (reinterpret_cast<char *> (&constant), 4);

            char currentRegister2;
            executabil.read (&currentRegister2, 1);

            //std::cout << "lb: " << codeRegisters[currentRegister1] << ' ' << constant << ' ' << codeRegisters[currentRegister2] << '\n';
            //std::cout << '\n';

            if (registers[codeRegisters[currentRegister2]].isPointer == true) {
                //std::cout << "sb:\n" << registers[codeRegisters[currentRegister2]].Pointer[constant] << '\n';

                registers[codeRegisters[currentRegister1]].isChar = true;
                registers[codeRegisters[currentRegister1]].isPointer = registers[codeRegisters[currentRegister1]].isInt = false;

                int index = registers[codeRegisters[currentRegister2]].index;
                int whichString = registers[codeRegisters[currentRegister2]].whichString;

                if (constant + index < strings[whichString].size ())
                    registers[codeRegisters[currentRegister1]].car = strings[whichString][constant + index];
                else
                    registers[codeRegisters[currentRegister1]].car = 0;
                //std::cout << registers[codeRegisters[currentRegister1]].car << '\n' << '\n';
           }
        } else if (code == 80) { /// beqz
            char currentRegister;
            executabil.read (&currentRegister, 1);

            short constant;
            executabil.read (reinterpret_cast<char *> (&constant), 2);

           // std::cout << "beqz: " << registers[codeRegisters[currentRegister]].car << '\n';
           // std::cout << '\n';

            if (registers[codeRegisters[currentRegister]].car == 0) {
                executabil.seekp (constant, std::ios_base::beg);
            }
        } else if (code == 17) { /// addi
            char currentRegister1;
            executabil.read (&currentRegister1, 1);

            char currentRegister2;
            executabil.read (&currentRegister2, 1);

            int constant;
            executabil.read (reinterpret_cast<char *> (&constant), 4);

            if (currentRegister1 == 2) { /// sp
                //std::cout << codeRegisters[currentRegister1] << ' ' << codeRegisters[currentRegister2] << ' ' << constant << '\n';
                constant = std::abs (constant) / 8; //std::cout << constant << '\n';
                Stack.resize (constant);
            } else {
                if (registers[codeRegisters[currentRegister2]].isInt == true) {
                    registers[codeRegisters[currentRegister1]].isInt = true;
                    registers[codeRegisters[currentRegister1]].isChar = registers[codeRegisters[currentRegister1]].isPointer = false;

                    registers[codeRegisters[currentRegister1]].val = registers[codeRegisters[currentRegister2]].val + constant;
                } else if (registers[codeRegisters[currentRegister2]].isPointer == true) {
                    registers[codeRegisters[currentRegister1]].isPointer = true;
                    registers[codeRegisters[currentRegister1]].isChar = registers[codeRegisters[currentRegister1]].isInt = false;

                    registers[codeRegisters[currentRegister1]].index = registers[codeRegisters[currentRegister2]].index + constant;

                    //std::cout << "addi: " << registers[codeRegisters[currentRegister1]].index << '\n';
                    //std::cout << '\n';
                    //std::string help = registers[codeRegisters[currentRegister2]].Pointer;
                    //registers[codeRegisters[currentRegister1]].Pointer = help.erase (0, constant);
                }
            }
        } else if (code == 64) { /// j
            short constant;
            executabil.read (reinterpret_cast<char *> (&constant), 2);

            executabil.seekp (constant, std::ios_base::beg);
        } else if (code == 34) { /// mv
            char currentRegister1;
            executabil.read (&currentRegister1, 1);

            char currentRegister2;
            executabil.read (&currentRegister2, 1);


            if (registers[codeRegisters[currentRegister2]].isPointer == true) {
                registers[codeRegisters[currentRegister1]].isPointer = true;
                registers[codeRegisters[currentRegister1]].isChar = registers[codeRegisters[currentRegister1]].isInt = false;

                registers[codeRegisters[currentRegister1]].index = registers[codeRegisters[currentRegister2]].index;
                registers[codeRegisters[currentRegister1]].whichString = registers[codeRegisters[currentRegister2]].whichString;
            } else if (registers[codeRegisters[currentRegister2]].isInt == true) {
                registers[codeRegisters[currentRegister1]].isInt = true;
                registers[codeRegisters[currentRegister1]].isChar = registers[codeRegisters[currentRegister1]].isPointer = false;

                registers[codeRegisters[currentRegister1]].val = registers[codeRegisters[currentRegister2]].val;
            }
        }else if (code == 32) { /// sb
            char currentRegister1;
            executabil.read (&currentRegister1, 1);
            int i = currentRegister1;

            int constanta;
            executabil.read (reinterpret_cast<char *> (&constanta), 4);

            char currentRegister2;
            executabil.read (&currentRegister2, 1);
            int j = currentRegister2;

            //std::cout << "sb: " << codeRegisters[i] << ' ' << constanta << ' ' << codeRegisters[j] << '\n';

            //std::cout << "Caracter: " << registers[codeRegisters[i]].car << '\n';

            //std::cout << '\n';

            if (registers[codeRegisters[i]].isChar == true) {
                registers[codeRegisters[j]].isPointer = true;
                registers[codeRegisters[j]].isInt = registers[codeRegisters[i]].isChar = false;

                int index = registers[codeRegisters[j]].index;
                int whichString = registers[codeRegisters[j]].whichString;
                char mutare = registers[codeRegisters[i]].car;

               // std::cout << whichString << ' ' << constanta + index << ' ' << (int) mutare << '\n';

                if (mutare != 0) {
                    strings[whichString][constanta + index] = mutare;
                    //std::cout << strings[whichString] << '\n';
//                    for (int h = 0; h < strings[whichString].size (); h += 1)
//                        std::cout << strings[whichString][h] << '\n';
//                    std::cout << strings[whichString][constanta + index] << '\n';
                } else {
                   // std::cout << (int) mutare << '\n';
                   // std::cout << constanta + index << '\n';

                    int sfarsit = strings[whichString].size ();
                    strings[whichString].erase (constanta + index, sfarsit);
                }
            }
            //std::cout << '\n';
        } else if (code == 82) { /// bge
            char currentRegister1;
            executabil.read (&currentRegister1, 1);
            int i = currentRegister1;

            char currentRegister2;
            executabil.read (&currentRegister2, 1);
            int j = currentRegister2;

            short constanta;
            executabil.read (reinterpret_cast<char *> (&constanta), 2);

            if (registers[codeRegisters[currentRegister1]].val >= registers[codeRegisters[currentRegister2]].val)
                executabil.seekp (constanta, std::ios_base::beg);
        } else if (code == 33) {
            //std::cout << "got here\n";

            char currentRegister1;
            executabil.read (&currentRegister1, 1);

            int constanta;
            executabil.read (reinterpret_cast<char *> (&constanta), 4);

            char currentRegister2;
            executabil.read (&currentRegister2, 1);

            if (codeRegisters[currentRegister2] == "sp") {
                constanta /= 8;
                componenetsRegisters help = registers[currentRegister1];
                Stack[constanta] = componenetsRegisters;
            }
            //std::cout << codeRegisters[currentRegister1] << ' ' << constanta << ' ' << codeRegisters[currentRegister2] << '\n';
        }
        executabil.read (&code, 1);
    }

    //std::cout << "Rezultat: " registers["a0"].val << '\n';
    //std::cout << "Rezultat: " << strings[registers["a0"].whichString];
    //std::cout << "Rezultat: " << strings[registers["a0"].whichString];
    return 0;
}
