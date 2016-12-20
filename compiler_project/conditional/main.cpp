#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <vector>
#include <map>
#include <sstream> // stringstream
#include <climits> // CHAR_BIT

using namespace std;

int const NUM_REGS = 8;

struct instruction {
    bool (*const execute)(const string[]);
    string args[];
};

map<string, int> numArgs;

void setupArgCount() {
    numArgs["noop"] = 0;
    numArgs["rwtm"] = 2;
    numArgs["vwtm"] = 2;
    numArgs["rstv"] = 2;
    numArgs["rstr"] = 2;
    numArgs["radv"] = 2;
    numArgs["radr"] = 2;
    numArgs["rlng"] = 1;
    numArgs["rang"] = 1;
    numArgs["reqv"] = 1;
    numArgs["reqr"] = 2;
    numArgs["rgtv"] = 2;
    numArgs["rgtr"] = 2;
    numArgs["moup"] = 2;
    numArgs["minp"] = 2;
}

void writeFlagCheck(ostream &output, string const &flagCheckString, unsigned long long firstToCheck = 0, size_t num_flag_bytes) {
    // For each character in flagCheckString...
    // If the character is 1, set the corresponding bit in mustBeSet
    unsigned char mustBeSet[num_flag_bytes];
    // If the character is 0, set the corresponding bit in mustBeUnset
    unsigned char mustBeUnset[num_flag_bytes];
    for (unsigned long long i = 0; i < flagCheckString.size(); ++i) {
        int indexToSet = i + firstToCheck;
        if (flagCheckString[i] == '1') {
            // Set the proper bit in mustBeSet;
        } else if (flagCheckString[i] == '0') {
            mustBeUnset |= 1 << (firstToCheck + i);
        }
    }
    output << "if (";


}

void writeFlagSet(ostream &output, unsigned long long flagIndex) {
    output << "\t"<< "flags[" << flagIndex/CHAR_BIT << "] |= 1 << " << flagIndex%CHAR_BIT << ";";
}

/*
 * Turns a string into a chain of memory lookups based on how many times it's prefixed with '@'
 */
string dereference(string &param) {
    size_t depth = param.find_first_not_of('@');
    string result = param.substr(depth);
    for (;depth != 0; --depth) {
        result = "memory[" + result + "]";
    }
    return result;
}

void writeInstruction(ostream &output, string opcode, string args[], unsigned long long flagIndex) {
    // Because you did not write a program of over 18 quintillion lines in some weird esolang I came up with, I can be sure the flag index fits in a long long.
    if (opcode == "noop") {
        writeFlagSet(output, flagIndex);
    } else if (opcode == "rwtm") {
        output << '\t' << "memory[" << dereference(args[1]) << "] = " << args[0] << ";\n";
        writeFlagSet(output, flagIndex);
    } else if (opcode == "vwtm") {
        output << '\t' << "memory[" << dereference(args[1]) << "] = " << dereference(args[0]) << ";\n";
        writeFlagSet(output, flagIndex);
    } else if (opcode == "rstv") {
        output << '\t' << args[0] << " = " << dereference(args[1]) << "; ";
        writeFlagSet(output, flagIndex);
    } else if (opcode == "rstr") {
        output << '\t' << args[0] << " = " << args[1] << "; ";
        writeFlagSet(output, flagIndex);
    } else if (opcode == "radv") {
        output << '\t' << args[0] << " += " << dereference(args[1]) << ";";
        writeFlagSet(output, flagIndex);
    } else if (opcode == "radr") {
        output << '\t' << args[0] << " += " << args[1] << "; ";
        writeFlagSet(output, flagIndex);
    } else if (opcode == "rlng") {
        output << '\t' << args[0] << " = !" << args[0] << "; ";
        writeFlagSet(output, flagIndex);
    } else if (opcode == "rang") {
        output << '\t' << args[0] << " = -" << args[0] << "; ";
        writeFlagSet(output, flagIndex);
    } else if (opcode == "reqv") {
        output << '\t' << "if (" << args[0] << " == " << dereference(args[1]) << ") ";
        writeFlagSet(output, flagIndex);
    } else if (opcode == "reqr") {
        output << '\t' << "if (" << args[0] << " == " << args[1] << ") ";
        writeFlagSet(output, flagIndex);
    } else if (opcode == "rgtv") {
        output << '\t' << "if (" << args[0] << " > " << dereference(args[1]) << ") ";
        writeFlagSet(output, flagIndex);
    } else if (opcode == "rgtr") {
        output << '\t' << "if (" << args[0] << " > " << args[1] << ") ";
        writeFlagSet(output, flagIndex);
    } else if (opcode == "moup") {
        output << '\t' << "for (int i = 0; i < " << dereference(args[1]) << "; ++i) {";
        output << "std::cout.put(memory[" << dereference(args[0]) << " + i]);}\n";
        // TODO: Finish implementing this
        writeFlagSet(output, flagIndex);
    } else if (opcode == "minp") {
        // TODO: Implement this opcode.
    } else {
        cout << "Unknown opcode: " << opcode;
        throw 0;
    }
    output << '\n';
}

int main(int argc, char **argv)
{
    std::string inputFile;
    std::string outputFile;
    int maxMemory = 0x1000;

    // Reading command line arguments.
    bool inputFileSet = false;
    bool outputFileSet = false;
    for (int i = 1; i < argc; ++i) {
        if (argv[i][0] == '-') {
            // it's a flag
            switch (argv[i][1]) {
            case 'i':
                maxMemory = stoi(argv[i+1]);
                break;
            default:
                cout << "Unknown flag: " << argv[i][1];
                return EXIT_FAILURE;
            }
        } else if (!inputFileSet) {
            inputFileSet = true;
            inputFile = argv[i];
        } else if (!outputFileSet) {
            outputFileSet = true;
            outputFile = argv[i];
        }
    }

    // Opening required streams
    if (!inputFileSet) {
        cout << "No input file provided" << endl;
        return 1;
    }
    ifstream input;
    input.open(inputFile);
    if (!input.is_open()) {
        cout << "Could not read input file" << endl;
        return 1;
    }
    if (!outputFileSet) {
        cout << "No output file specified" << endl;
        return 1;
    }
    ofstream output;
    output.open(outputFile);
    if (!output.is_open()) {
        cout << "Could not open output file" << endl;
        return 1;
    }
    int lineCount = 0;

    output << "#include <iostream>\n";
    output << "int main() {\n";
    //setup "memory"
    output << "\t" << "unsigned char memory[" << maxMemory << "];\n";
    // setup registers
    for (int i = 0; i < NUM_REGS; ++i) {
        output << "\t" << "unsigned char r" << i << ";\n";
    }

    setupArgCount();

    std::string line;
    vector<std::string> text; // No idea how many we need.

    while (!input.eof() && !input.fail()) {
        cerr << "Reading a line..." << endl;
        getline(input, line);
        line = line.substr(line.find_first_not_of(" \n\t"));
        cerr << "Got a line: " << line << endl;
        if (line.length() > 0 && line[0] != '#') {
            cerr << "It wasn't a comment!" << endl;
            text.push_back(line);
            ++lineCount;
            cerr << "Inserting line..." << endl;
            cerr << "We've counted " << lineCount << " lines and stored " << text.size() << '\n';
            cerr << "Most recent line was: " << text.back() << '\n';
        }
    }

    // Setting up flags
    int numFlags = (lineCount)/CHAR_BIT +1;
    output << "\t" << "unsigned char flags[" << numFlags << "];\n";

    // And loops. Because loops.
    output << "do {";

    // And now we start the actual compiling
    for (int i = 0; i < lineCount; ++i) {
        cerr << "Parsing line: " << text[i] << '\n';
        // Break it into parts.
        stringstream lineStream(text[i]);
        // We've stripped off leading whitespace, the first part is the opcode
        string opcode;
        std::getline(lineStream, opcode, ' ');
        cerr << "Opcode found: " << opcode << "\n";
        cerr << "With length: " << opcode.size() << " (4 expected)\n";
        cerr << "First character had value: " << (int) opcode[0] << endl;

        // Next, some known number of arguments
        if (!numArgs.count(opcode)) {
            cout << "COMPILE ERROR: Unknown opcode " << opcode << endl;
            return EXIT_FAILURE;
        }
        string args[numArgs.at(opcode)];
        for (int a = 0; a < numArgs.at(opcode); ++a) {
            lineStream >> args[a];
        }

        // Next, maybe a flag specifier
        string flagSpecifier;
        lineStream >> flagSpecifier;
        if (flagSpecifier != "" && flagSpecifier[0] != '#') {
            // There is a flag specifier.
            // Two possible formats - prefixed by a number and a colon, or not.
            size_t colonIndex = flagSpecifier.find(':');
            int firstFlagToCheck = 0;
            if (colonIndex != string::npos && colonIndex < flagSpecifier.find('#')) {
                // Prefixed flag specifier - we change the offset.
                int offset = stoi(flagSpecifier.substr(0, colonIndex));
                flagSpecifier = flagSpecifier.substr(colonIndex+1);
                firstFlagToCheck = i - offset;
            }
            writeFlagCheck(output, flagSpecifier, firstFlagToCheck, numFlags);
            writeInstruction(output, opcode, args, i);
            output << "}\n";
        } else {
            writeInstruction(output, opcode, args, i);
        }
    }






    output << "}while (flags[" << lineCount/CHAR_BIT << "] & 1 << " << lineCount%CHAR_BIT << ");\n}";
}
