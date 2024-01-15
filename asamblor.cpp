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
std::map<std::string, std::pair<std::string, char>> variables;

bool isNumeric (const char a) {
    return (a >= '0' && a <= '9');
}

bool isLetter (const char a) {
    return (a >= 'a' && a <= 'z') || (a >= 'A' && a <= 'Z');
}

int main () {
    instructions["la"] = 0;
    instructions["lb"] = 1;
    instructions["lw"] = 2;
    instructions["ld"] = 3;
    instructions["li"] = 4;
    instructions["add"] = 16;
    instructions["addi"] = 17;
    instructions["sub"] = 18;
    instructions["sb"] = 32;
    instructions["sd"] = 33;
    instructions["mv"] = 34;
    instructions["slli"] = 48;
    instructions["srai"] = 49;
    instructions["j"] = 64;
    instructions["call"] = 65;
    instructions["ret"] = 66;
    instructions["beqz"] = 80;
    instructions["bnez"] = 81;
    instructions["bge"] = 82;
    instructions["bgt"] = 83;
    instructions["ble"] = 84;

    instructions["flw"] = 96;
    instructions["fld"] = 97;
    instructions["fsw"] = 98;

    instructions["fadd.d"] = 112;
    instructions["fsub.d"] = 113;
    instructions["fmul.d"] = 114;
    instructions["fsqrt.d"] = 115;


    instructions["fadd.s"] = -128;
    instructions["fmul.s"] = -127;
    instructions["fmv.s"] = -126;
    instructions["fmv.s.x"] = -125;
    instructions["fgt.s"] = -124;
    instructions["flt.s"] = -123;


    Registers["zero"] = 0;
    Registers["ra"] = 1;
    Registers["sp"] = 2;

    Registers["t0"] = 16;
    Registers["t1"] = 17;
    Registers["t2"] = 18;
    Registers["t3"] = 19;
    Registers["t4"] = 20;
    Registers["t5"] = 21;

    Registers["s1"] = 33;

    Registers["a0"] = 48;
    Registers["a1"] = 49;
    Registers["a2"] = 50;
    Registers["a3"] = 51;

    Registers["ft0"] = 64;
    Registers["ft1"] = 65;
    Registers["ft2"] = 66;
    Registers["ft3"] = 67;

    Registers["fa0"] = 80;
    Registers["fa1"] = 81;
    Registers["fa2"] = 82;

    CFunctions["strlen"] = 112;
    CFunctions["printf"] = 96;
    CFunctions["scanf"] = 97;
    CFunctions["cfunc"] = 113;

    std::string line;

    std:getline (in, line);

    if (line == ".section .rodata")
    {
        std::getline(in, line);
        int nr_variables = 0;
        while (line != ".section .text")
        {
            if (line == "")
            {
                std::getline(in, line);
                continue;
            }

            //std::cout << line << '\n';
            int i = 0;
            while (line[i] == ' ') ++i;

            if (line[i] == '#')
                continue;

            std::string currentVariable = "";
            while (line[i] != ':')
                currentVariable += line[i], i++;

            i++; // trec peste :
            while (line[i] == ' ') ++i;
            while (line[i] != ' ') ++i; // trec peste .asciz
            while (line[i] == ' ') ++i;
            ++i;
            std::string valoare = "";
            while (line[i] != '"')
                valoare += line[i], i++;

            //std::cout << currentVariable << ' ' << valoare << ' ' << nr_variables << '\n';
            variables[currentVariable] = {valoare, nr_variables++};
            //std::cout << variables[currentVariable].first << ' ' << variables[currentVariable].second << '\n';

            char codificare = -1;
            executabil.write(reinterpret_cast<const char*>(&codificare), 1);

            for (i = 0; valoare[i] != '\0'; ++i)
                if (valoare[i] != '\\')
                    executabil.write(reinterpret_cast<const char*>(&(valoare[i])), 1);
                else
                {
                    char v;
                    i++;
                    if (valoare[i] == 'n')
                        v = '\n';
                    else if (valoare[i] == 't')
                        v = '\t';
                    else if (valoare[i] == '\\')
                        v = '\\';
                    else if (valoare[i] == '\"')
                        v = '\"';
                    else if (valoare[i] == '\'')
                        v = '\'';
                    executabil.write(reinterpret_cast<const char*>(&v), 1);
                }


            executabil.write(reinterpret_cast<const char*>(&valoare[i]), 1);

            std::getline(in, line);
        }
    }
    while (line != "main:")
        std::getline(in,line);


    while (!in.eof ()) {
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

            //std::cout << currentSize << '\n';

            currentInstruction.pop_back ();
            posEtichete[currentInstruction] = currentSize;

            //std::cout << posEtichete["1"] << '\n';

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
                   || currentInstruction == "fmul.d" || currentInstruction == "fadd.d" || currentInstruction == "fmul.s"
                   || currentInstruction == "fgt.s" || currentInstruction == "flt.s" || currentInstruction == "fadd.s") {
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
                   || currentInstruction == "fsw" || currentInstruction == "flw") {
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
        } else if (currentInstruction == "mv" || currentInstruction == "fsqrt.d" || currentInstruction == "fmv.s"
                   || currentInstruction == "fmv.s.x" || currentInstruction == "la") {
            while (line[i] == ' ') i += 1;

            std::string currentRegister1 = "";

            while (isLetter (line[i]) || isNumeric (line[i]))
                currentRegister1 += line[i], i += 1;

            while (!(isLetter (line[i]) || isNumeric (line[i]))) i += 1;

            std::string currentRegister2 = "";

            while (isLetter (line[i]) || isNumeric (line[i]) || line[i] == '_')
                currentRegister2 += line[i], i += 1;

            executabil.write (reinterpret_cast<const char *> (&instructions[currentInstruction]), 1);
            executabil.write (reinterpret_cast<const char *> (&Registers[currentRegister1]), 1);
            if (variables.find(currentRegister2) == variables.end())
                executabil.write (reinterpret_cast<const char *> (&Registers[currentRegister2]), 1);
            else
                executabil.write (reinterpret_cast<const char *> (&variables[currentRegister2].second), 1);
        } else if (currentInstruction == "ret") {
            executabil.write (reinterpret_cast<const char *> (&instructions[currentInstruction]), 1);
        } else if (currentInstruction == "beqz" || currentInstruction == "bnez") {
            while (line[i] == ' ') i += 1;

            std::string currentRegister = "";

            while (isLetter (line[i]) || isNumeric (line[i]))
                currentRegister += line[i], i += 1;

            while (line[i] == ' ' ) i += 1;

            i += 1;

            while (line[i] == ' ') i += 1;

            std::string label = "";

            while (line[i] != 'b' && line[i] != 'f')
                label += line[i], i += 1;

            executabil.write (reinterpret_cast<const char *> (&instructions[currentInstruction]), 1);
            executabil.write (reinterpret_cast<const char *> (&Registers[currentRegister]), 1);

            if (line[i] == 'f') {
                int currentSize = executabil.tellg ();

                waitingLabels.push_back (std::make_pair (label, currentSize));

                int zero = 0;
                executabil.write (reinterpret_cast<const char *> (&zero), 2);

            } else if (line[i] == 'b') {
                executabil.write (reinterpret_cast<const char *> (&posEtichete[label]), 2);
            }
        } else if (currentInstruction == "j") {

            std::string label = "";

            while (line[i] == ' ') i += 1;

            while (line[i] != 'b' && line[i] != 'f')
                label += line[i], i += 1;

            executabil.write (reinterpret_cast<const char *> (&instructions[currentInstruction]), 1);

            if (line[i] == 'f') {
                int currentSize = executabil.tellg ();

                waitingLabels.push_back (std::make_pair (label, currentSize));

                int zero = 0;
                executabil.write (reinterpret_cast<const char *> (&zero), 2);

            } else if (line[i] == 'b')
                executabil.write (reinterpret_cast<const char *> (&posEtichete[label]), 2);

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

            while (line[i] != 'b' && line[i] != 'f')
                label += line[i], i += 1;

            //while (line[i] != ' ' || line[i] != '\n')
                //std::cout << line[i], i += 1;
                //label += line[i], i += 1;

            executabil.write (reinterpret_cast<const char *> (&instructions[currentInstruction]), 1);
            executabil.write (reinterpret_cast<const char *> (&Registers[currentRegister1]), 1);
            executabil.write (reinterpret_cast<const char *> (&Registers[currentRegister2]), 1);

            //std::cout << label.size () << '\n';

            if (line[i] == 'f') {
                int currentSize = executabil.tellg ();

                //std::cout << label.siz << '\n';

                waitingLabels.push_back (std::make_pair (label, currentSize));

                int zero = 0;
                executabil.write (reinterpret_cast<const char *> (&zero), 2);
            } else if (line[i] == 'b') {
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
        }
    }
    return 0;
}
