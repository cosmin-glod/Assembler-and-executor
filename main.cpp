#include <ios>
#include <istream>
#include <utility>
#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <map>

std::ifstream in("input.in");
std::fstream executabil("executabil.bin", std::ios::out | std::ios::binary | std::ios::trunc);

std::list<std::pair<std::string, int>> waitingLabels;

std::map<std::string, int> posEtichete;

std::map<std::string, int> CFunctions;
std::map<std::string, int> instructions;
std::map<std::string, int> Registers;

bool isNumeric (const char a) {
    return (a >= '0' && a <= '9');
}

bool isLetter (const char a) {
    return (a >= 'a' && a <= 'z') || (a >= 'A' && a <= 'Z');
}

int main () {
    instructions["li"] = 4;
    instructions["add"] = 16;
    instructions["addi"] = 17;
    instructions["lb"] = 1;
    instructions["mv"] = 34;
    instructions["mv"] = 34;
    instructions["ret"] = 66;
    instructions["eticheta"] = 69;
    instructions["beqz"] = 80;
    instructions["bneqz"] = 81;
    instructions["j"] = 64;
    instructions["sb"] = 32;
    instructions["bge"] = 82;
    instructions["call"] = 65;
    instructions["srai"] = 49;
    instructions["slli"] = 48;
    instructions["sub"] = 18;
    instructions["bgt"] = 83;
    instructions["ble"] = 84;
    instructions["sd"] = 21;


    Registers["zero"] = 0;
    Registers["ra"] = 1;
    Registers["sp"] = 2;
    Registers["s1"] = 33;
    Registers["t0"] = 16;
    Registers["t1"] = 17;
    Registers["t2"] = 18;
    Registers["t3"] = 19;
    Registers["t4"] = 20;
    Registers["t5"] = 21;
    Registers["a0"] = 48;
    Registers["a1"] = 49;
    Registers["a2"] = 50;

    CFunctions["strlen"] = 112;
    CFunctions["printf"] = 96;
    CFunctions["scanf"] = 97;
    CFunctions["cfunc"] = 113;

    while (!in.eof ()) {
        std::string line;
        std::getline (in, line);

        int i  = 0;
        while (line[i] != NULL && line[i] == ' ') i += 1;

        if (line[i] == '#')
            continue;

        std::string currentInstruction = "";

        while (line[i] != NULL && line[i] != ' ')
            currentInstruction += line[i], i += 1;

        if (currentInstruction[currentInstruction.size () - 1] == ':') {

            int currentSize = executabil.tellg ();
            currentInstruction.pop_back ();
            posEtichete[currentInstruction] = currentSize;

            std::list<std::pair<std::string, int>>::iterator it = waitingLabels.begin ();

            while (it != waitingLabels.end ()) {
                if ((*it).first == currentInstruction) {
                    executabil.seekg ((*it).second, std::ios_base::beg);
                    executabil.write (reinterpret_cast<const char *> (&posEtichete[currentInstruction]), 2);
                    executabil.seekg (0, std::ios_base::end);

                    it = waitingLabels.erase (it);
                }

                else
                    ++it;
            }
        }


        if (currentInstruction == "li") {
            while (line[i] == ' ') i += 1;

            std::string currentRegister = "";

            while (isLetter (line[i]) || isNumeric (line[i]))
                currentRegister += line[i], i += 1;

            while (!(isLetter (line[i]) || isNumeric (line[i]))) i += 1;

            i -= 1;

            bool Minus = 0;
            if (line[i] == '-') Minus = 1;

            i += 1;

            int nr = 0;
            while (isNumeric (line[i]))
                nr = nr * 10 + (int) (line[i] - '0'), i += 1;

            if (Minus == 1) nr = -nr;

            executabil.write (reinterpret_cast<const char *> (&instructions[currentInstruction]), 1);
            executabil.write (reinterpret_cast<const char *> (&Registers[currentRegister]), 1);
            executabil.write (reinterpret_cast <const char *> (&nr), 4);
        } else if (currentInstruction == "add" || currentInstruction == "sub" || currentInstruction == "fsub.d"
                   || currentInstruction == "fmul.d" || currentInstruction == "fadd.d"
                   || currentInstruction == "fgt.s" || currentInstruction == "flt.s") {
            while (line[i] == ' ') i += 1;

            std::string currentRegister1 = "";

            while (isLetter (line[i]) || isNumeric (line[i]))
                currentRegister1 += line[i], i += 1;

            while (!(isLetter (line[i]) || isNumeric (line[i]))) i += 1;


            std::string currentRegister2 = "";

            while (isLetter (line[i]) || isNumeric (line[i]))
                currentRegister2 += line[i], i += 1;


            while (!(isLetter (line[i]) || isNumeric (line[i]))) i += 1;

            std::string currentRegister3 = "";

            while (isLetter (line[i]) || isNumeric (line[i]))
                currentRegister3 += line[i], i += 1;

            executabil.write (reinterpret_cast<const char *> (&instructions[currentInstruction]), 1);
            executabil.write (reinterpret_cast<const char *> (&Registers[currentRegister1]), 1);
            executabil.write (reinterpret_cast<const char *> (&Registers[currentRegister2]), 1);
            executabil.write (reinterpret_cast<const char *> (&Registers[currentRegister3]), 1);
        } else if (currentInstruction == "sb" || currentInstruction == "sd" || currentInstruction == "lb"
                   || currentInstruction == "ld" || currentInstruction == "lw" || currentInstruction == "fld"
                   || currentInstruction == "fsw") {
            while (line[i] == ' ') i += 1;

            std::string currentRegister1 = "";

            while (isLetter (line[i]) || isNumeric (line[i]))
                currentRegister1 += line[i], i += 1;

            while (!(isLetter (line[i]) || isNumeric (line[i]))) i += 1;

            i -= 1;
            bool Minus = 0;
            if (line[i] == '-') Minus = 1;

            i += 1;

            int nr = 0;
            while (isNumeric (line[i]))
                nr = nr * 10 + (int) (line[i] - '0'), i += 1;

            if (Minus == 1) nr = -nr;

            i += 1;

            std::string currentRegister2 = "";

            while (isLetter (line[i]) || isNumeric (line[i]))
                currentRegister2 += line[i], i += 1;

            executabil.write (reinterpret_cast<const char *> (&instructions[currentInstruction]), 1);
            executabil.write (reinterpret_cast<const char *> (&Registers[currentRegister1]), 1);
            executabil.write (reinterpret_cast<const char *> (&nr), 4);
            executabil.write (reinterpret_cast<const char *> (&Registers[currentRegister2]), 1);
        } else if (currentInstruction == "addi") {
            while (line[i] == ' ') i += 1;

            std::string currentRegister1 = "";

            while (isLetter (line[i]) || isNumeric (line[i]))
                currentRegister1 += line[i], i += 1;

            while (!(isLetter (line[i]) || isNumeric (line[i]))) i += 1;


            std::string currentRegister2 = "";

            while (isLetter (line[i]) || isNumeric (line[i]))
                currentRegister2 += line[i], i += 1;

            while (!(isLetter (line[i]) || isNumeric (line[i]))) i += 1;

            i -= 1;

            bool Minus = 0;
            if (line[i] == '-') Minus = 1;
            i += 1;

            int nr = 0;

            while (isNumeric (line[i]))
                nr = nr * 10 + (int) (line[i] - '0'), i += 1;

            if (Minus == 1) nr = -nr;

            executabil.write (reinterpret_cast<const char *> (&instructions[currentInstruction]), 1);
            executabil.write (reinterpret_cast<const char *> (&Registers[currentRegister1]), 1);
            executabil.write (reinterpret_cast<const char *> (&Registers[currentRegister2]), 1);
            executabil.write (reinterpret_cast<const char *> (&nr), 4);
        } else if (currentInstruction == "mv" || currentInstruction == "fsqrt.d" || currentInstruction == "fmv.s") {
            while (line[i] == ' ') i += 1;

            std::string currentRegister1 = "";

            while (isLetter (line[i]) || isNumeric (line[i]))
                currentRegister1 += line[i], i += 1;

            while (!(isLetter (line[i]) || isNumeric (line[i]))) i += 1;

            std::string currentRegister2 = "";

            while (isLetter (line[i]) || isNumeric (line[i]))
                currentRegister2 += line[i], i += 1;

            executabil.write (reinterpret_cast<const char *> (&instructions[currentInstruction]), 1);
            executabil.write (reinterpret_cast<const char *> (&Registers[currentRegister1]), 1);
            executabil.write (reinterpret_cast<const char *> (&Registers[currentRegister2]), 1);
        } else if (currentInstruction == "ret") {
            executabil.write (reinterpret_cast<const char *> (&instructions[currentInstruction]), 1);
        } else if (currentInstruction == "beqz" || currentInstruction == "bneqz") {
            while (line[i] == ' ') i += 1;

            std::string currentRegister = "";

            while (isLetter (line[i]) || isNumeric (line[i]))
                currentRegister += line[i], i += 1;

            while (line[i] == ' ' ) i += 1;

            i += 1;

            while (line[i] == ' ') i += 1;

            std::string label = "";

            while (line[i] != ' ')
                label += line[i], i += 1;

            executabil.write (reinterpret_cast<const char *> (&instructions[currentInstruction]), 1);
            executabil.write (reinterpret_cast<const char *> (&Registers[currentRegister]), 1);

            if (label[label.size () - 1] == 'f') {
                int currentSize = executabil.tellg ();
                label.pop_back ();

                waitingLabels.push_back (std::make_pair (label, currentSize));

                int zero = 0;
                executabil.write (reinterpret_cast<const char *> (&zero), 2);

            } else if (label[label.size () - 1] == 'b') {
                label.pop_back ();
                executabil.write (reinterpret_cast<const char *> (&posEtichete[label]), 2);
            }
        } else if (currentInstruction == "j") {

            std::string label = "";

            while (line[i] == ' ') i += 1;

            while (line[i] != ' ')
                label += line[i], i += 1;

            executabil.write (reinterpret_cast<const char *> (&instructions[currentInstruction]), 1);

            if (label[label.size () - 1] == 'f') {
                int currentSize = executabil.tellg ();
                label.pop_back ();

                waitingLabels.push_back (std::make_pair (label, currentSize));

                int zero = 0;
                executabil.write (reinterpret_cast<const char *> (&zero), 2);

            } else if (label[label.size () - 1] == 'b') {
                label.pop_back ();
                executabil.write (reinterpret_cast<const char *> (&posEtichete[label]), 2);
            }
        } else if (currentInstruction == "bge" || currentInstruction == "bgt" || currentInstruction == "ble") {
            while (line[i] == ' ') i += 1;

            std::string currentRegister1 = "";

            while (isLetter (line[i]) || isNumeric (line[i]))
                currentRegister1 += line[i], i += 1;

            while (!(isLetter (line[i]) || isNumeric (line[i]))) i += 1;

            std::string currentRegister2 = "";

            while (isLetter (line[i]) || isNumeric (line[i]))
                currentRegister2 += line[i], i += 1;

            while (!(isLetter (line[i]) || isNumeric (line[i]))) i += 1;

            std::string label = "";

            while (line[i] != ' ')
                label += line[i], i += 1;

            executabil.write (reinterpret_cast<const char *> (&instructions[currentInstruction]), 1);
            executabil.write (reinterpret_cast<const char *> (&Registers[currentRegister1]), 1);
            executabil.write (reinterpret_cast<const char *> (&Registers[currentRegister2]), 1);

            if (label[label.size () - 1] == 'f') {
                int currentSize = executabil.tellg ();
                label.pop_back ();

                waitingLabels.push_back (std::make_pair (label, currentSize));

                int zero = 0;
                executabil.write (reinterpret_cast<const char *> (&zero), 2);
            } else if (label[label.size () - 1] == 'b') {
                label.pop_back ();
                executabil.write (reinterpret_cast<const char *> (&posEtichete[label]), 2);
            }
        } else if (currentInstruction == "call") {
            while (line[i] == ' ') i += 1;

            std::string funct = "";

            while (isLetter (line[i]))
                funct += line[i], i += 1;

            executabil.write (reinterpret_cast<const char *> (&instructions[currentInstruction]), 1);
            executabil.write (reinterpret_cast<const char *> (&CFunctions[funct]), 1);
        } else if (currentInstruction == "srai" || currentInstruction == "slli") {
            while (line[i] == ' ') i += 1;

            std::string currentRegister1 = "";

            while (isLetter (line[i]) || isNumeric (line[i]))
                currentRegister1 += line[i], i += 1;

            while (!(isLetter (line[i]) || isNumeric (line[i]))) i += 1;

            std::string currentRegister2 = "";

            while (isLetter (line[i]) || isNumeric (line[i]))
                currentRegister2 += line[i], i += 1;

            while (!(isLetter (line[i]) || isNumeric (line[i]))) i += 1;

            int nr = 0;
            while (isNumeric (line[i]))
                nr = nr * 10 + (int) (line[i] - '0'), i += 1;

            executabil.write (reinterpret_cast<const char *> (&instructions[currentInstruction]), 1);
            executabil.write (reinterpret_cast<const char *> (&Registers[currentRegister1]), 1);
            executabil.write (reinterpret_cast<const char *> (&Registers[currentRegister2]), 1);
            executabil.write (reinterpret_cast<const char *> (&nr), 4);
        } else if (currentInstruction == "fsub.d") {

        }
    }
    return 0;
}

