#include <ios>
#include <iostream>
#include <fstream>
#include <cstring>
#include <sstream>
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
    long long val = 1;
    bool isInt = false;

    char car = 1;
    bool isChar = false;

    int index;
    int whichString;
    bool isPointer = false;

    float fval;
    bool isFloat = false;

    double dval;
    bool isDouble = false;
};

std::map<int, std::string> codeRegisters;
std::map<std::string, componenetsRegisters> registers;

std::vector<componenetsRegisters> Stack;

std::vector<std::string> strings;
std::vector<std::vector<int>> arrayInt;
std::vector<std::vector<long long>> arrayLong;
std::vector<std::vector<float>> arrayFloat;
std::vector<std::vector<double>> arrayDouble;
std::vector<std::vector<short>> arrayShort;

int main () {
    while (!fin.eof ()) {
        std::string currentRegister; fin >> currentRegister;

        std::string dataType; fin >> dataType;

        std::string content; std::getline (fin, content);

        while (content[0] == ' ')
            content.erase (0, 1);

        if (dataType == "string") {
            content.resize (1000, '\0');
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
        } else if (dataType == "float") {
            float valoare = std::stof (content);

            registers[currentRegister].isFloat = 1;
            registers[currentRegister].fval = valoare;
        } else if (dataType == "float&" || dataType == "arrayFloat") {
            //std::cout << currentRegister << ' ' << dataType << ' ';

            std::istringstream citireFloat(content);
            std::vector<float> help;

            float valoare;
            while (citireFloat >> valoare) {
                help.emplace_back (valoare);
                help.emplace_back (0), help.emplace_back (0), help.emplace_back (0);
            }

            arrayFloat.emplace_back (help);

            registers[currentRegister].isPointer = 1;
            registers[currentRegister].whichString = arrayFloat.size () - 1;
            registers[currentRegister].index = 0;
        } else if (dataType == "Coordinate&") {
            std::istringstream citireFloat(content);
            std::vector<double> help;

            double valoare;
            while (citireFloat >> valoare) {
                help.emplace_back (valoare);
                help.emplace_back (0), help.emplace_back (0), help.emplace_back (0);
                help.emplace_back (0), help.emplace_back (0), help.emplace_back (0);
                help.emplace_back (0);
            }

            arrayDouble.emplace_back (help);

            registers[currentRegister].isPointer = 1;
            registers[currentRegister].whichString = arrayDouble.size () - 1;
            registers[currentRegister].index = 0;
        } else if (dataType == "LL*") {
            std::istringstream citireLL(content);
            std::vector<short> help;

            short valoare;
            while (citireLL >> valoare) {
                help.emplace_back (valoare);
                //std::cout << valoare << '\n';
                help.emplace_back (0), help.emplace_back (0), help.emplace_back (0);
                help.emplace_back (0), help.emplace_back (0), help.emplace_back (0);
                help.emplace_back (0);
            }

            arrayShort.emplace_back (help);

            registers[currentRegister].isPointer = 1;
            registers[currentRegister].whichString = arrayShort.size () - 1;
            registers[currentRegister].index = 0;
        }
    }

//    for (int i = 0; i < arrayFloat[registers["a1"].whichString].size (); i += 4)
//        std::cout << arrayFloat[registers["a1"].whichString][i] << ' ';
//    std::cout << '\n';
//    for (int i = 0; i < arrayFloat[registers["a2"].whichString].size (); i += 4)
//        std::cout << arrayFloat[registers["a2"].whichString][i] << ' ';
//    std::cout << '\n';

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

    registers["zero"].isInt = true;
    registers["zero"].val = 0;
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
        //std::cout << (int) code << " ft0: " << registers["ft0"].fval << '\n';
        //std::cout << std::hex << code << '\n';
        //std::cout << std::dec << registers["ft0"].fval << '\n';
        if (code == 4) { /// li
            char currentRegister;
            executabil.read (&currentRegister, 1);

            int constant;
            executabil.read (reinterpret_cast<char *> (&constant), 4);


            registers[codeRegisters[currentRegister]].isInt = true;
            registers[codeRegisters[currentRegister]].isChar = registers[codeRegisters[currentRegister]].isPointer = false;
            registers[codeRegisters[currentRegister]].isDouble = registers[codeRegisters[currentRegister]].isFloat = false;

            registers[codeRegisters[currentRegister]].val = constant;

            //std::cout << "li: " << codeRegisters[currentRegister] << ' ' << registers[codeRegisters[currentRegister]].val << '\n';
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
                registers[codeRegisters[i]].isFloat = registers[codeRegisters[i]].isDouble = false;

                registers[codeRegisters[i]].index = registers[codeRegisters[k]].val;
                registers[codeRegisters[i]].whichString = registers[codeRegisters[j]].whichString;
            } else if (registers[codeRegisters[k]].isPointer == true) {
                registers[codeRegisters[i]].isPointer = true;
                registers[codeRegisters[i]].isChar = registers[codeRegisters[i]].isInt = false;
                registers[codeRegisters[i]].isFloat = registers[codeRegisters[i]].isDouble = false;

                registers[codeRegisters[i]].index = registers[codeRegisters[j]].val;
                registers[codeRegisters[i]].whichString = registers[codeRegisters[k]].whichString;
            } else if (registers[codeRegisters[j]].isInt == true && registers[codeRegisters[k]].isInt == true) {
                registers[codeRegisters[i]].isInt = true;
                registers[codeRegisters[i]].isChar = registers[codeRegisters[i]].isPointer = false;
                registers[codeRegisters[i]].isFloat = registers[codeRegisters[i]].isDouble = false;

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
                //std::cout << "E char: " << codeRegisters[currentRegister1] << '\n';
                //std::cout << "lb: " << registers[codeRegisters[currentRegister2]].Pointer[] << '\n';

                registers[codeRegisters[currentRegister1]].isChar = true;

                //std::cout << registers[codeRegisters[currentRegister1]].isChar << '\n';

                registers[codeRegisters[currentRegister1]].isPointer = registers[codeRegisters[currentRegister1]].isInt = false;
                registers[codeRegisters[currentRegister1]].isFloat = registers[codeRegisters[currentRegister1]].isDouble = false;

                int index = registers[codeRegisters[currentRegister2]].index;
                int whichString = registers[codeRegisters[currentRegister2]].whichString;

                //std::cout << strings[whichString][constant + index] << '\n';

                //if (strings[whichString][constant])

                if (strings[whichString][constant + index] == '\0')
                    registers[codeRegisters[currentRegister1]].car = 0; //std::cout << registers[codeRegisters[currentRegister1]].isChar << ' ';
                else
                    registers[codeRegisters[currentRegister1]].car = strings[whichString][constant + index];

//
//                if (constant + index < strings[whichString].size ())
//                    registers[codeRegisters[currentRegister1]].car = strings[whichString][constant + index];
//                else
//                    registers[codeRegisters[currentRegister1]].car = 0;
                //std::cout << registers[codeRegisters[currentRegister1]].car << '\n' << '\n';
           }
        } else if (code == 80) { /// beqz
            char currentRegister;
            executabil.read (&currentRegister, 1);

            short constant;
            executabil.read (reinterpret_cast<char *> (&constant), 2);

            //std::cout << "beqz: " << codeRegisters[currentRegister] << ' ' << registers[codeRegisters[currentRegister]].val << ' ' << (registers[codeRegisters[currentRegister]].val == 0) << '\n';
           // std::cout << '\n';

           //std::cout << "Este char: " << registers[codeRegisters[currentRegister]].isChar << '\n';

            if (registers[codeRegisters[currentRegister]].car == 0) {
                //std::cout << "Sar de aici\n";
                executabil.seekp (constant, std::ios_base::beg);
            }
            if (registers[codeRegisters[currentRegister]].val == 0) {
                //std::cout << "sar de aici!\n";
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
                    registers[codeRegisters[currentRegister1]].isFloat = registers[codeRegisters[currentRegister1]].isDouble = false;

                    registers[codeRegisters[currentRegister1]].val = registers[codeRegisters[currentRegister2]].val + constant;
                } else if (registers[codeRegisters[currentRegister2]].isPointer == true) {
                    registers[codeRegisters[currentRegister1]].isPointer = true;
                    registers[codeRegisters[currentRegister1]].isChar = registers[codeRegisters[currentRegister1]].isInt = false;
                    registers[codeRegisters[currentRegister1]].isFloat = registers[codeRegisters[currentRegister1]].isDouble = false;

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

            if (registers[codeRegisters[currentRegister1]].isPointer == true && registers[codeRegisters[currentRegister2]].isPointer == true) {
                registers[codeRegisters[currentRegister1]].whichString = registers[codeRegisters[currentRegister2]].whichString;
            }

            if (registers[codeRegisters[currentRegister2]].isPointer == true) {
                registers[codeRegisters[currentRegister1]].isPointer = true;
                registers[codeRegisters[currentRegister1]].isChar = registers[codeRegisters[currentRegister1]].isInt = false;
                registers[codeRegisters[currentRegister1]].isDouble = registers[codeRegisters[currentRegister1]].isFloat = false;

                registers[codeRegisters[currentRegister1]].index = registers[codeRegisters[currentRegister2]].index;
                registers[codeRegisters[currentRegister1]].whichString = registers[codeRegisters[currentRegister2]].whichString;
            } else if (registers[codeRegisters[currentRegister2]].isInt == true) {
                registers[codeRegisters[currentRegister1]].isInt = true;
                registers[codeRegisters[currentRegister1]].isChar = registers[codeRegisters[currentRegister1]].isPointer = false;
                registers[codeRegisters[currentRegister1]].isFloat = registers[codeRegisters[currentRegister1]].isDouble = false;

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
                registers[codeRegisters[j]].isFloat = registers[codeRegisters[i]].isDouble = false;

                //std::cout << "sb: " << registers[codeRegisters[i]].car << '\n';

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
                    //std::cout << (int) mutare << '\n';

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

//            std::cout << "bge: \n";
//            std::cout << "    " << codeRegisters[currentRegister1] << ' ' << registers[codeRegisters[currentRegister1]].val << '\n';
//            std::cout << "    " << codeRegisters[currentRegister2] << ' ' << registers[codeRegisters[currentRegister2]].val << '\n';
//            std::cout << "    " << (registers[codeRegisters[currentRegister1]].val >= registers[codeRegisters[currentRegister2]].val) << '\n';

            if (registers[codeRegisters[currentRegister1]].val >= registers[codeRegisters[currentRegister2]].val)
                executabil.seekp (constanta, std::ios_base::beg);
//            if (registers[codeRegisters[currentRegister1]].fval >= registers[codeRegisters[currentRegister2]].fval)
//                executabil.seekp (constanta, std::ios_base::beg);
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
                registers[codeRegisters[currentRegister2]].isFloat = registers[codeRegisters[currentRegister2]].isDouble = false;

                int index = registers[codeRegisters[currentRegister2]].index;
                int whichString = registers[codeRegisters[currentRegister2]].whichString;
                int valoare = registers[codeRegisters[currentRegister1]].val;

                arrayLong[whichString][constanta + index] = valoare;
            } else if (registers[codeRegisters[currentRegister1]].isPointer == true) {
                int index1 = registers[codeRegisters[currentRegister1]].whichString;
                int index2 = registers[codeRegisters[currentRegister2]].whichString;

                arrayShort[index2].insert (arrayShort[index2].begin () + constanta, arrayShort[index1].begin (), arrayShort[index1].end ());

//                for (int i = 0; i < arrayShort[registers["a0"].whichString].size (); i += 8)
//                    std::cout << arrayShort[registers["a0"].whichString][i] << ' ';
//                std::cout << '\n';
//
//                for (int i = 0; i < arrayShort[registers["a1"].whichString].size (); i += 8)
//                    std::cout << arrayShort[registers["a1"].whichString][i] << ' ';
//                std::cout << '\n';
            }
            //std::cout << codeRegisters[currentRegister1] << ' ' << constanta << ' ' << codeRegisters[currentRegister2] << '\n';
        } else if (code == 65) { /// call
            char Function;
            executabil.read (&Function, 1);

            if (Function == 112) { /// strlen
                registers["a0"].isInt = true;
                registers["a0"].isChar = registers["a0"].isPointer = false;
                registers["a0"].isDouble = registers["a0"].isFloat = false;

                int index = 0;
                while (strings[registers["a0"].index][index] != '\0')
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
            registers[codeRegisters[currentRegister1]].isFloat = registers[codeRegisters[currentRegister1]].isDouble = false;

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
                registers[codeRegisters[i]].isFloat = registers[codeRegisters[i]].isDouble = false;

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
                registers[codeRegisters[currentRegister1]].isFloat = registers[codeRegisters[currentRegister1]].isDouble = false;

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
            registers[codeRegisters[currentRegister1]].isFloat = registers[codeRegisters[currentRegister1]].isDouble = false;

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
                registers[codeRegisters[currentRegister1]].isFloat = registers[codeRegisters[currentRegister1]].isDouble = false;

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
        } else if (code == 81) { /// bnez
            char currentRegister1;
            executabil.read (&currentRegister1, 1);

            short constanta;
            executabil.read (reinterpret_cast<char *> (&constanta), 2);

            if (registers[codeRegisters[currentRegister1]].val != 0)
                executabil.seekg (constanta, std::ios_base::beg);
        } else if (code == 83) { /// bgt
            char currentRegister1;
            executabil.read (&currentRegister1, 1);

            char currentRegister2;
            executabil.read (&currentRegister2, 1);

            short constanta;
            executabil.read (reinterpret_cast<char *> (&constanta), 2);

            if (registers[codeRegisters[currentRegister1]].val > registers[codeRegisters[currentRegister2]].val)
                executabil.seekg (constanta, std::ios_base::beg);
        } else if (code == -126) { /// fmv.s
            char currentRegister1;
            executabil.read (&currentRegister1, 1);

            char currentRegister2;
            executabil.read (&currentRegister2, 1);

            registers[codeRegisters[currentRegister1]].isFloat = true;
            registers[codeRegisters[currentRegister1]].isChar = registers[codeRegisters[currentRegister1]].isInt = false;
            registers[codeRegisters[currentRegister1]].isPointer = registers[codeRegisters[currentRegister1]].isDouble = false;
            registers[codeRegisters[currentRegister1]].fval = registers[codeRegisters[currentRegister2]].fval;

            //std::cout << "fmv.s: " << registers[codeRegisters[currentRegister1]].fval << '\n';
        } else if (code == -124) { /// fgt.s
            char currentRegister1;
            executabil.read (&currentRegister1, 1);

            char currentRegister2;
            executabil.read (&currentRegister2, 1);

            char currentRegister3;
            executabil.read (&currentRegister3, 1);

            float float1 = registers[codeRegisters[currentRegister2]].fval;
            float float2 = registers[codeRegisters[currentRegister3]].fval;

            registers[codeRegisters[currentRegister1]].isInt = true;
            registers[codeRegisters[currentRegister1]].isChar = registers[codeRegisters[currentRegister1]].isInt = false;
            registers[codeRegisters[currentRegister1]].isFloat = registers[codeRegisters[currentRegister1]].isDouble = false;

            registers[codeRegisters[currentRegister1]].isChar = registers[codeRegisters[currentRegister1]].isFloat = registers[codeRegisters[currentRegister1]].isPointer = false;

            registers[codeRegisters[currentRegister1]].val = (float1 > float2);

            //std::cout << "fgt.s: " << codeRegisters[currentRegister2] << ' ' << float1 << '\n';
            //std::cout << codeRegisters[currentRegister3] << ' ' << float2 << '\n';
            //std::cout << codeRegisters[currentRegister1] << ' ' << registers[codeRegisters[currentRegister1]].val << '\n';
        } else if (code == -123) { /// flt.s
            char currentRegister1;
            executabil.read (&currentRegister1, 1);

            char currentRegister2;
            executabil.read (&currentRegister2, 1);

            char currentRegister3;
            executabil.read (&currentRegister3, 1);

            float float1 = registers[codeRegisters[currentRegister2]].fval;
            float float2 = registers[codeRegisters[currentRegister3]].fval;

            registers[codeRegisters[currentRegister1]].isInt = true;
            registers[codeRegisters[currentRegister1]].isChar = registers[codeRegisters[currentRegister1]].isPointer = false;
            registers[codeRegisters[currentRegister1]].isFloat = registers[codeRegisters[currentRegister1]].isDouble = false;
            registers[codeRegisters[currentRegister1]].isChar = registers[codeRegisters[currentRegister1]].isFloat = registers[codeRegisters[currentRegister1]].isPointer = false;

            registers[codeRegisters[currentRegister1]].val = (float1 < float2);

            //std::cout << "flt.s: " << registers[codeRegisters[currentRegister1]].val << '\n';
        } else if (code == 98) { /// fsw
            char currentRegister1;
            executabil.read (&currentRegister1, 1);

            int constant;
            executabil.read (reinterpret_cast<char *> (&constant), 4);

            char currentRegister2;
            executabil.read (&currentRegister2, 1);

            //std::cout << "Got here!";

            //std::cout << "Got here: " << codeRegisters[currentRegister1] << ' ' << registers[codeRegisters[currentRegister1]].isFloat << '\n';

            if (registers[codeRegisters[currentRegister1]].isFloat == true) {
                registers[codeRegisters[currentRegister2]].isPointer = true;
                registers[codeRegisters[currentRegister2]].isChar = registers[codeRegisters[currentRegister2]].isFloat = registers[codeRegisters[currentRegister2]].isInt = false;
                registers[codeRegisters[currentRegister2]].isDouble = false;


                int index = registers[codeRegisters[currentRegister2]].index;
                int whichString = registers[codeRegisters[currentRegister2]].whichString;
                float valoare = registers[codeRegisters[currentRegister1]].fval;

                //std::cout << whichString << ' ' << index + constant << ' ' << valoare << '\n';


                if (index + constant < arrayFloat[whichString].size ()) {
                    arrayFloat[whichString][index + constant] = valoare;
                    if (index + constant + 1 < arrayFloat[whichString].size ())
                        arrayFloat[whichString][index + constant + 1] = 0;
                    else
                        arrayFloat[whichString].emplace_back (0);
                    if (index + constant + 2 < arrayFloat[whichString].size ())
                        arrayFloat[whichString][index + constant + 2] = 0;
                    else
                        arrayFloat[whichString].emplace_back (0);
                    if (index + constant + 3 < arrayFloat[whichString].size ())
                        arrayFloat[whichString][index + constant + 3] = 0;
                    else
                        arrayFloat[whichString].emplace_back (0);
                }
                else {
                    arrayFloat[whichString].emplace_back (valoare);
                    arrayFloat[whichString].emplace_back (0), arrayFloat[whichString].emplace_back (0), arrayFloat[whichString].emplace_back (0);
                }
            }
        } else if (code == 97) { /// fld
            char currentRegister1;
            executabil.read (&currentRegister1, 1);

            int constanta;
            executabil.read (reinterpret_cast<char *> (&constanta), 4);

            char currentRegister2;
            executabil.read (&currentRegister2, 1);

            int index = registers[codeRegisters[currentRegister2]].index;
            int whichString = registers[codeRegisters[currentRegister2]].whichString;

            registers[codeRegisters[currentRegister1]].isDouble = true;
            registers[codeRegisters[currentRegister1]].isChar = registers[codeRegisters[currentRegister1]].isInt = false;
            registers[codeRegisters[currentRegister1]].isFloat = registers[codeRegisters[currentRegister1]].isPointer = false;
            registers[codeRegisters[currentRegister1]].dval = arrayDouble[whichString][index + constanta];

            //std::cout << registers[codeRegisters[currentRegister1]].dval << '\n';
        } else if (code == 113) { /// fsub.d
            char currentRegister1;
            executabil.read (&currentRegister1, 1);

            char currentRegister2;
            executabil.read (&currentRegister2, 1);

            char currentRegister3;
            executabil.read (&currentRegister3, 1);

            registers[codeRegisters[currentRegister1]].isDouble = 1;
            registers[codeRegisters[currentRegister1]].isFloat = registers[codeRegisters[currentRegister1]].isPointer = false;
            registers[codeRegisters[currentRegister1]].isChar = registers[codeRegisters[currentRegister1]].isInt = false;

            registers[codeRegisters[currentRegister1]].dval = registers[codeRegisters[currentRegister2]].dval - registers[codeRegisters[currentRegister3]].dval;
        } else if (code == 112) { /// fadd.d
            char currentRegister1;
            executabil.read (&currentRegister1, 1);

            char currentRegister2;
            executabil.read (&currentRegister2, 1);

            char currentRegister3;
            executabil.read (&currentRegister3, 1);

            registers[codeRegisters[currentRegister1]].isDouble = 1;
            registers[codeRegisters[currentRegister1]].isFloat = registers[codeRegisters[currentRegister1]].isPointer = false;
            registers[codeRegisters[currentRegister1]].isChar = registers[codeRegisters[currentRegister1]].isInt = false;
            registers[codeRegisters[currentRegister1]].dval = registers[codeRegisters[currentRegister2]].dval + registers[codeRegisters[currentRegister3]].dval;
        } else if (code == 114) { /// fmul.d
            char currentRegister1;
            executabil.read (&currentRegister1, 1);

            char currentRegister2;
            executabil.read (&currentRegister2, 1);

            char currentRegister3;
            executabil.read (&currentRegister3, 1);

            registers[codeRegisters[currentRegister1]].isDouble = 1;
            registers[codeRegisters[currentRegister1]].isFloat = registers[codeRegisters[currentRegister1]].isPointer = false;
            registers[codeRegisters[currentRegister1]].isChar = registers[codeRegisters[currentRegister1]].isInt = false;
            registers[codeRegisters[currentRegister1]].dval = registers[codeRegisters[currentRegister2]].dval * registers[codeRegisters[currentRegister3]].dval;
        } else if (code == 115) { /// fsqrt.d
            char currentRegister1;
            executabil.read (&currentRegister1, 1);

            char currentRegister2;
            executabil.read (&currentRegister2, 1);

            registers[codeRegisters[currentRegister1]].isDouble = 1;
            registers[codeRegisters[currentRegister1]].isFloat = registers[codeRegisters[currentRegister1]].isPointer = false;
            registers[codeRegisters[currentRegister1]].isChar = registers[codeRegisters[currentRegister1]].isInt = false;
            registers[codeRegisters[currentRegister1]].dval = std::sqrt (registers[codeRegisters[currentRegister2]].dval);
        } else if (code == 96) { /// flw
            char currentRegister1;
            executabil.read (&currentRegister1, 1);

            int constanta;
            executabil.read (reinterpret_cast<char *> (&constanta), 4);

            char currentRegister2;
            executabil.read (&currentRegister2, 1);

            int index = registers[codeRegisters[currentRegister2]].index;
            int whichString = registers[codeRegisters[currentRegister2]].whichString;

            float valoare = arrayFloat[whichString][index + constanta];

            registers[codeRegisters[currentRegister1]].isFloat = 1;
            registers[codeRegisters[currentRegister1]].isDouble = registers[codeRegisters[currentRegister1]].isPointer = false;
            registers[codeRegisters[currentRegister1]].isChar = registers[codeRegisters[currentRegister1]].isInt = false;
            registers[codeRegisters[currentRegister1]].fval = valoare;

            //std::cout << "flw: " << codeRegisters[currentRegister1] << ' ' << registers[codeRegisters[currentRegister1]].fval << '\n';
        } else if (code == -128) { /// fadd.s
            char currentRegister1;
            executabil.read (&currentRegister1, 1);

            char currentRegister2;
            executabil.read (&currentRegister2, 1);

            char currentRegister3;
            executabil.read (&currentRegister3, 1);

            registers[codeRegisters[currentRegister1]].isFloat = 1;
            registers[codeRegisters[currentRegister1]].isDouble = registers[codeRegisters[currentRegister1]].isPointer = false;
            registers[codeRegisters[currentRegister1]].isChar = registers[codeRegisters[currentRegister1]].isInt = false;
            registers[codeRegisters[currentRegister1]].fval = registers[codeRegisters[currentRegister2]].fval + registers[codeRegisters[currentRegister3]].fval;

            //std::cout << "fadd.s " << registers["fa0"].fval << '\n';
        } else if (code == -127) { /// fmul.s
            //std::cout << "Test: " << registers["ft0"].fval << '\n';

            char currentRegister1;
            executabil.read (&currentRegister1, 1);

            char currentRegister2;
            executabil.read (&currentRegister2, 1);

            char currentRegister3;
            executabil.read (&currentRegister3, 1);

            //std::cout << "Test2: " << registers["ft0"].fval << '\n';


            registers[codeRegisters[currentRegister1]].isFloat = 1;
            registers[codeRegisters[currentRegister1]].isDouble = registers[codeRegisters[currentRegister1]].isPointer = false;
            registers[codeRegisters[currentRegister1]].isChar = registers[codeRegisters[currentRegister1]].isInt = false;

           // std::cout << codeRegisters[currentRegister2] << ' ' << registers[codeRegisters[currentRegister2]].fval << '\n';
           // std::cout << codeRegisters[currentRegister3] << ' ' << registers[codeRegisters[currentRegister3]].fval << '\n';

            registers[codeRegisters[currentRegister1]].fval = registers[codeRegisters[currentRegister2]].fval * registers[codeRegisters[currentRegister3]].fval;

           // std::cout << "Test3: " << registers["ft0"].fval << '\n';


            //std::cout << "fmul.s: " << registers["ft0"].fval << '\n';
//            std::cout << "       " << codeRegisters[currentRegister2] << ' ' << registers[codeRegisters[currentRegister2]].fval << '\n';
//            std::cout << "       " << codeRegisters[currentRegister3] << ' ' << registers[codeRegisters[currentRegister3]].fval << '\n';

//            std::cout << "fmul.s: ";
//            std::cout << codeRegisters[currentRegister1] << ' ' << registers[codeRegisters[currentRegister1]].fval << '\n';
//            std::cout << codeRegisters[currentRegister2] << ' ' << registers[codeRegisters[currentRegister2]].fval << '\n';
//
//            std::cout << '\n';
        } else if (code == -125) { /// fmv.s.x
            char currentRegister1;
            executabil.read (&currentRegister1, 1);

            char currentRegister2;
            executabil.read (&currentRegister2, 1);

            registers[codeRegisters[currentRegister1]].isFloat = 1;
            registers[codeRegisters[currentRegister1]].isDouble = registers[codeRegisters[currentRegister1]].isPointer = false;
            registers[codeRegisters[currentRegister1]].isChar = registers[codeRegisters[currentRegister1]].isInt = false;
            registers[codeRegisters[currentRegister1]].fval = float (registers[codeRegisters[currentRegister2]].val);

            //std::cout << "fmv.s.x: " << codeRegisters[currentRegister1] << ' ' << codeRegisters[currentRegister2] << '\n';
            //std::cout << "         " << registers[codeRegisters[currentRegister1]].fval << '\n';
        }
        //if (test == 34) {exit(0);}

        executabil.read (&code, 1);
    }

    for (auto it : registers) {
        std::cout << it.first << ": ";
        if (it.second.isInt)
            std::cout << it.second.val;
        else if (it.second.isChar)
            std::cout << it.second.car;
        else if (it.second.isFloat)
            std::cout << it.second.fval;
        else if (it.second.isDouble)
            std::cout << it.second.dval;
        else if (it.second.isPointer) {
            int index = it.second.index;
            int whichString = it.second.whichString;
            if (arrayDouble.size () > 0) {
                for (int i = 0; i < arrayDouble[whichString].size (); i += 8)
                    std::cout << arrayDouble[whichString][i] << ' ';
            } else if (arrayFloat.size () > 0) {
                for (int i = 0; i < arrayFloat[whichString].size (); i += 4)
                    std::cout << arrayFloat[whichString][i] << ' ';
            } else if (arrayInt.size () > 0) {
                for (int i = 0; i < arrayInt[whichString].size (); i += 4)
                    std::cout << arrayInt[whichString][i] << ' ';
            } else if (arrayLong.size () > 0) {
                for (int i = 0; i < arrayLong[whichString].size (); i += 8)
                    std::cout << arrayLong[whichString][i] << ' ';
            } else if (arrayShort.size () > 0) {
                for (int i = 0; i < arrayShort[whichString].size (); i += 8)
                    std::cout << arrayShort[whichString][i] << ' ';
            } else if (strings.size () > 0) {
                std::cout << strings[whichString];
            }
        }
        std::cout << '\n';
    }
////
////   // std::cout << "Rezultat: " << registers["a0"].val << '\n';
////    //std::cout << "Rezultat: " << strings[registers["a0"].whichString];
////    //std::cout << "Rezultat: " << strings[registers["a0"].whichString];
////    //std::cout << "Rezultat: " << strings[registers["a0"].whichString];
////    //std::cout << "Rezultat: " << registers["t0"].val;
////    //std::cout << "Rezultat: ";
//////    for (int i = 0; i < arrayLong[0].size (); i += 1)
//////        std::cout << arrayLong[0][i] << ' ';
////
////    //std::cout << "Rezultat: " << registers["a0"].val << '\n';
////
//////    int index = registers["a0"].index;
//////    int whichString = registers["a0"].whichString;
//////    for (int i = 0; i < arrayFloat[whichString].size (); i += 1) {
//////        std::cout << arrayFloat[whichString][i] << ' ';
//////    }
//////
//////    index = registers["a1"].index;
//////    whichString = registers["a1"].whichString;
//////    for (int i = 0; i < arrayFloat[whichString].size (); i += 1) {
//////        std::cout << arrayFloat[whichString][i] << ' ';
//////    }
////
////    //std::cout << registers["fa0"].dval;
////
//////    for (int i = 0; i < 3 * 4; i += 4)
//////        std::cout << arrayFloat[registers["a0"].whichString][i] << ' ';

    return 0;
}
