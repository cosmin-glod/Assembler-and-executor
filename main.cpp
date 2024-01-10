#include <ios>
#include <iostream>
#include <fstream>
#include <string>
#include <map>

std::ifstream in("input.in");
std::fstream executabil("executabil.bin", std::ios::out | std::ios::binary | std::ios::trunc);

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
    instructions["mv"] = 32;
    instructions["ret"] = 66;
    instructions["eticheta"] = 69;

    Registers["t0"] = 16;
    Registers["t1"] = 17;
    Registers["a0"] = 48;

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
            executabil.write (reinterpret_cast<const char *> (&instructions["eticheta"]), 1);

        }


        if (currentInstruction == "li") {
            while (line[i] == ' ') i += 1;

            std::string currentRegister = "";

            while (isLetter (line[i]) || isNumeric (line[i]))
                currentRegister += line[i], i += 1;

            while (!(isLetter (line[i]) || isNumeric (line[i]))) i += 1;

            int nr = 0;
            while (isNumeric (line[i]))
                nr = nr * 10 + (int) (line[i] - '0'), i += 1;

            executabil.write (reinterpret_cast<const char *> (&instructions[currentInstruction]), 1);
            executabil.write (reinterpret_cast<const char *> (&Registers[currentRegister]), 1);
            executabil.write (reinterpret_cast <const char *> (&nr), 4);
        } else if (currentInstruction == "add") {
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
        } else if (currentInstruction == "lb") {
            while (line[i] == ' ') i += 1;

            std::string currentRegister1 = "";

            while (isLetter (line[i]) || isNumeric (line[i]))
                currentRegister1 += line[i], i += 1;

            while (!(isLetter (line[i]) || isNumeric (line[i]))) i += 1;

            int nr = 0;
            while (isNumeric (line[i]))
                nr = nr * 10 + (int) (line[i] - '0'), i += 1;

            i += 1;

            std::string currentRegister2 = "";

            while (isLetter (line[i]) || isNumeric (line[i]))
                currentRegister2 += line[i], i += 1;

            executabil.write (reinterpret_cast<const char *> (&instructions[currentInstruction]), 1);
            executabil.write (reinterpret_cast<const char *> (&Registers[currentRegister1]), 1);
            executabil.write (reinterpret_cast<const char *> (&nr), 4);
            executabil.write (reinterpret_cast<const char *> (&Registers[currentRegister1]), 1);
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


            int nr = 0;

            while (isNumeric (line[i]))
                nr = nr * 10 + (int) (line[i] - '0'), i += 1;

            executabil.write (reinterpret_cast<const char *> (&instructions[currentInstruction]), 1);
            executabil.write (reinterpret_cast<const char *> (&Registers[currentRegister1]), 1);
            executabil.write (reinterpret_cast<const char *> (&Registers[currentRegister2]), 1);
            executabil.write (reinterpret_cast<const char *> (&nr), 4);
        } else if (currentInstruction == "mv") {
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
        } else if (currentInstruction == "ret")
            executabil.write (reinterpret_cast<const char *> (&instructions[currentInstruction]), 1);


    }
    return 0;
}
