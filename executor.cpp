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

bool isNumeric (char a) {
    return (a >= '0' && a <= '9');
}

struct componenetsRegisters {
    long long val;
    bool isInt = false;

    char car;
    bool isChar = false;

    int index;
    int whichString;
    bool isPointer = false;
};

std::map<int, std::string> codeRegisters;
std::map<std::string, componenetsRegisters> registers;

std::vector<componenetsRegisters> Stack;

std::vector<std::string> strings;

std::vector<std::vector<int>> arrayInt;
std::vector<std::vector<long long>> arrayLong;

int main () {
    while (!fin.eof ()) {
        std::string currentRegister; fin >> currentRegister;

        std::string dataType; fin >> dataType;

        std::string content; std::getline (fin, content);

        while (content[0] == ' ')
            content.erase (0, 1);

        if (dataType == "string") {
            content.resize (1000, '@');
            strings.emplace_back (content);

            registers[currentRegister].index = 0;
            registers[currentRegister].whichString = strings.size () - 1;
            registers[currentRegister].isPointer = true;
        } else if (dataType == "int") {
            registers[currentRegister].val = std::stoi (content);
            registers[currentRegister].isInt = true;
        } else if (dataType == "arrayInt") {
            std::vector<int> help;
            std::string::iterator it = content.begin ();
            while (it != content.end ()) {
                bool Minus = false;
                if (*it == '-')
                    Minus = true, ++it;

                int elementActual = 0;
                //std::cout << isNumeric (*it);
                while (it != content.end () && isNumeric (*it))
                    elementActual = elementActual * 10 + (int) (*it - '0'), ++it;

                if (Minus == true) elementActual = -elementActual;

                help.emplace_back (elementActual);
                help.emplace_back (0), help.emplace_back (0), help.emplace_back (0);

                if (it == content.end ())
                    break;
                ++it;
            }

            arrayInt.emplace_back (help);

            registers[currentRegister].index = 0;
            registers[currentRegister].whichString = arrayInt.size () - 1;
            registers[currentRegister].isPointer = true;
        } else if (dataType == "arrayLong") {
            std::vector<long long> help;
            std::string::iterator it = content.begin ();
            while (it != content.end ()) {
                bool Minus = false;
                if (*it == '-')
                    Minus = true, ++it;

                int elementActual = 0;
                while (it != content.end () && isNumeric (*it))
                    elementActual = elementActual * 10 + (int) (*it - '0'), ++it;

                if (Minus == true) elementActual = -elementActual;

                help.emplace_back (elementActual);
                help.emplace_back (0), help.emplace_back (0), help.emplace_back (0);
                help.emplace_back (0), help.emplace_back (0), help.emplace_back (0);
                help.emplace_back (0);

                if (it == content.end ())
                    break;
                ++it;
            }

            arrayLong.emplace_back (help);

            registers[currentRegister].index = 0;
            registers[currentRegister].whichString = arrayLong.size () - 1;
            registers[currentRegister].isPointer = true;
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

    int test = 0;

    executabil.read (&code, 1);
    while (code != 66) {
        test += 1;
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

            //std::cout << registers[codeRegisters[j]].val << ' ' << registers[codeRegisters[k]].val << '\n';

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
            } else if (registers[codeRegisters[j]].isInt == true && registers[codeRegisters[k]].isInt == true) {
                registers[codeRegisters[i]].isInt = true;
                registers[codeRegisters[i]].isChar = registers[codeRegisters[i]].isPointer = false;

                registers[codeRegisters[i]].val = registers[codeRegisters[j]].val + registers[codeRegisters[k]].val;
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
                if (constant < 0) {
                    constant = std::abs (constant) / 8; //std::cout << constant << '\n';
                    Stack.resize (constant);
                } else {
                    constant /= 8;
                    while (constant > 0)
                        Stack.pop_back (), constant -= 1;
                }
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

            //std::cout << registers[codeRegisters[currentRegister1]].val << ' ' << registers[codeRegisters[currentRegister2]].val << '\n';

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

            //std::cout << "bge: " << registers[codeRegisters[currentRegister1]].val << ' ' << registers[codeRegisters[currentRegister2]].val << '\n';

            if (registers[codeRegisters[currentRegister1]].val >= registers[codeRegisters[currentRegister2]].val)
                executabil.seekp (constanta, std::ios_base::beg);
        } else if (code == 33) { /// sd
            //std::cout << "got here\n";

            char currentRegister1;
            executabil.read (&currentRegister1, 1);

            int constanta;
            executabil.read (reinterpret_cast<char *> (&constanta), 4);

            char currentRegister2;
            executabil.read (&currentRegister2, 1);

            if (codeRegisters[currentRegister2] == "sp") {
                constanta /= 8;
                componenetsRegisters help = registers[codeRegisters[currentRegister1]];
                Stack[constanta] = help;
            } else if (registers[codeRegisters[currentRegister1]].isInt == true) {
                registers[codeRegisters[currentRegister2]].isPointer = true;
                registers[codeRegisters[currentRegister2]].isChar = registers[codeRegisters[currentRegister2]].isInt = false;

                int index = registers[codeRegisters[currentRegister2]].index;
                int whichString = registers[codeRegisters[currentRegister2]].whichString;
                int valoare = registers[codeRegisters[currentRegister1]].val;

                arrayLong[whichString][constanta + index] = valoare;
            }
            //std::cout << codeRegisters[currentRegister1] << ' ' << constanta << ' ' << codeRegisters[currentRegister2] << '\n';
        } else if (code == 65) { /// call
            char Function;
            executabil.read (&Function, 1);

            if (Function == 112) { /// strlen
                registers["a0"].isInt = true;
                registers["a0"].isChar = registers["a0"].isPointer = false;

                int index = 0;
                while (strings[registers["a0"].index][index] != '@')
                    index += 1;
                strings[registers["a0"].index].erase (index, strings[registers["a0"].index].size ());
                //std::cout <<
                //std::cout << "Sir: " << strings[registers["a0"].index] << '\n';
                registers["a0"].val = strings[registers["a0"].whichString].size ();
            }
        } else if (code == 49) { /// srai
            char currentRegister1;
            executabil.read (&currentRegister1, 1);

            char currentRegister2;
            executabil.read (&currentRegister2, 1);

            int constanta;
            executabil.read (reinterpret_cast<char *> (&constanta), 4);

            registers[codeRegisters[currentRegister1]].isInt = true;
            registers[codeRegisters[currentRegister1]].isChar = registers[codeRegisters[currentRegister1]].isPointer = false;

            registers[codeRegisters[currentRegister1]].val = (registers[codeRegisters[currentRegister2]].val >> constanta);
            //std::cout << registers[codeRegisters[currentRegister1]].val << '\n';
        } else if (code == 18) { /// sub
            char currentRegister1;
            executabil.read (&currentRegister1, 1);
            int i = currentRegister1;

            char currentRegister2;
            executabil.read (&currentRegister2, 1);
            int j = currentRegister2;

            char currentRegister3;
            executabil.read (&currentRegister3, 1);
            int k = currentRegister3;

            if (registers[codeRegisters[j]].isInt == true) {
                registers[codeRegisters[i]].isInt = true;
                registers[codeRegisters[i]].isChar = registers[codeRegisters[i]].isPointer = false;

                registers[codeRegisters[i]].val = registers[codeRegisters[j]].val - registers[codeRegisters[k]].val;
            }
        } else if (code == 3) { /// ld
            char currentRegister1;
            executabil.read (&currentRegister1, 1);

            int constanta;
            executabil.read (reinterpret_cast<char *> (&constanta), 4);

            char currentRegister2;
            executabil.read (&currentRegister2, 1);

            if (codeRegisters[currentRegister2] == "sp") {
                constanta /= 8;
                componenetsRegisters help = Stack[constanta];
                registers[codeRegisters[currentRegister1]] = help;
            } else if (registers[codeRegisters[currentRegister2]].isPointer == true) {
                registers[codeRegisters[currentRegister1]].isInt = true;
                registers[codeRegisters[currentRegister1]].isChar = registers[codeRegisters[currentRegister1]].isPointer = false;

                int index = registers[codeRegisters[currentRegister2]].index;
                int whichString = registers[codeRegisters[currentRegister2]].whichString;

                int indexArray = index + constanta;
                registers[codeRegisters[currentRegister1]].val = arrayLong[whichString][indexArray];

                //std::cout << "ld: " << registers[codeRegisters[currentRegister1]].val << '\n';
            }
        } else if (code == 48)  { /// slli
            char currentRegister1;
            executabil.read (&currentRegister1, 1);

            char currentRegister2;
            executabil.read (&currentRegister2, 1);

            int constant;
            executabil.read (reinterpret_cast<char *> (&constant), 4);

            registers[codeRegisters[currentRegister1]].isInt = true;
            registers[codeRegisters[currentRegister1]].isChar = registers[codeRegisters[currentRegister1]].isPointer = false;


            registers[codeRegisters[currentRegister1]].val = (registers[codeRegisters[currentRegister2]].val << constant);
            //std::cout << registers[codeRegisters[currentRegister1]].val << '\n';
        } else if (code == 2) { /// lw
            char currentRegister1;
            executabil.read (&currentRegister1, 1);

            int constant;
            executabil.read (reinterpret_cast<char *> (&constant), 4);

            char currentRegister2;
            executabil.read (&currentRegister2, 1);

            if (registers[codeRegisters[currentRegister2]].isPointer == true) {
                registers[codeRegisters[currentRegister1]].isInt = true;
                registers[codeRegisters[currentRegister1]].isChar = registers[codeRegisters[currentRegister1]].isPointer = false;

                int index = registers[codeRegisters[currentRegister2]].index;
                int whichString = registers[codeRegisters[currentRegister2]].whichString;

                int indexArray = index + constant;
                registers[codeRegisters[currentRegister1]].val = arrayInt[whichString][indexArray];

                //std::cout << codeRegisters[currentRegister1] << ' ' << registers[codeRegisters[currentRegister1]].val << '\n';
            }
        } else if (code == 84) { /// ble
            char currentRegister1;
            executabil.read (&currentRegister1, 1);

            char currentRegister2;
            executabil.read (&currentRegister2, 1);

            short constanta;
            executabil.read (reinterpret_cast<char *> (&constanta), 2);

            //std::cout << "ble: " << registers[codeRegisters[currentRegister1]].val << ' ' << registers[codeRegisters[currentRegister2]].val << '\n';
            //std::cout << (registers[codeRegisters[currentRegister1]].val <= registers[codeRegisters[currentRegister2]].val) << '\n';

            if (registers[codeRegisters[currentRegister1]].val <= registers[codeRegisters[currentRegister2]].val)
                executabil.seekp (constanta, std::ios_base::beg);
        } else if (code == 81) { /// bgt
            char currentRegister1;
            executabil.read (&currentRegister1, 1);

            short constanta;
            executabil.read (reinterpret_cast<char *> (&constanta), 2);

            if (registers[codeRegisters[currentRegister1]].val != 0)
                executabil.seekg (constanta, std::ios_base::beg);
        } else if (code == 83) {
            char currentRegister1;
            executabil.read (&currentRegister1, 1);

            char currentRegister2;
            executabil.read (&currentRegister2, 1);

            short constanta;
            executabil.read (reinterpret_cast<char *> (&constanta), 2);

            if (registers[codeRegisters[currentRegister1]].val > registers[codeRegisters[currentRegister2]].val)
                executabil.seekg (constanta, std::ios_base::beg);
        }

        //if (test == 34) {exit(0);}

        executabil.read (&code, 1);
    }

    //std::cout << "Rezultat: " registers["a0"].val << '\n';
    //std::cout << "Rezultat: " << strings[registers["a0"].whichString];
    //std::cout << "Rezultat: " << strings[re   gisters["a0"].whichString];
    //std::cout << "Rezultat: " << strings[registers["a0"].whichString];
    //std::cout << "Rezultat: " << registers["t0"].val;
    //std::cout << "Rezultat: ";
//    for (int i = 0; i < arrayLong[0].size (); i += 1)
//        std::cout << arrayLong[0][i] << ' ';

    std::cout << "Rezultat: " << registers["a0"].val << '\n';
    return 0;
}
