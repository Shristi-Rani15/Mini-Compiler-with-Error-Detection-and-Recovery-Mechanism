/*
#ifndef COMPILER_ENGINE_H
#define COMPILER_ENGINE_H

#include <string>
#include <vector>
#include <sstream>
#include <map>
#include <regex>

struct Node {
    std::string name;
    std::vector<Node*> children;
    Node(std::string n) : name(n) {}
};

struct ErrorReport {
    int line;
    std::string type, message, recovery;
};

class CompilerEngine {
public:
    std::map*/

#ifndef COMPILER_ENGINE_H
#define COMPILER_ENGINE_H

#include <string>
#include <vector>
#include <sstream>
#include <map>
#include <regex>

struct Node {
    std::string name;
    std::vector<Node*> children;
    Node(std::string n) : name(n) {}
};

struct ErrorReport {
    int line;
    std::string type, message, recovery;
};

class CompilerEngine {
public:
    std::map<std::string, std::string> symbolTable; 
    std::map<std::string, int> arraySizes; 
    std::vector<std::string> intermediateCode;
    Node* root;

    CompilerEngine() { root = new Node("Program_Root"); }

    std::vector<ErrorReport> analyze(std::string code) {
        std::vector<ErrorReport> reports;
        std::stringstream ss(code);
        std::string line;
        int lineNum = 1;
        int tempVarCount = 0;
        
        symbolTable.clear();
        arraySizes.clear();
        intermediateCode.clear();
        root = new Node("Program_Root");

        while (std::getline(ss, line)) {
            line.erase(0, line.find_first_not_of(" \t\r\n"));
            if (line.empty()) { lineNum++; continue; }
            
            Node* lineNode = new Node("Line " + std::to_string(lineNum));
            root->children.push_back(lineNode);

            // PHASE 1: PREPROCESSOR & LEXICAL
            if (line[0] == '#') {
                lineNode->children.push_back(new Node("Preprocessor: " + line));
               lineNum++; 
               continue;
            } else if (line.find("@") != std::string::npos || line.find("$") != std::string::npos) {
                reports.push_back({lineNum, "LEXICAL", "Illegal character detected", "RECOVERY: Ignored."});
            }

            // PHASE 2: SYNTAX
            if (line.back() != ';' && line.find("{") == std::string::npos && line.find("}") == std::string::npos) {
                reports.push_back({lineNum, "SYNTAX", "Missing semicolon ';'", "PHRASE-RECOVERY: Assumed."});
            }

            // PHASE 3: SEMANTIC
            std::regex declRegex(R"((int|float|string)\s+(\w+)(\[(\d+)\])?\s*(=?\s*(.*))?)");
            std::smatch match;
            if (std::regex_search(line, match, declRegex)) {
                std::string type = match[1], varName = match[2];
                symbolTable[varName] = type;
                if(match[4].matched) arraySizes[varName] = std::stoi(match[4]);
                lineNode->children.push_back(new Node("Decl: " + type + " " + varName));

                // PHASE 4 & 5: ICG & OPTIMIZATION (Generating TAC)
                if (match[6].matched && !match[6].str().empty()) {
                    std::string val = match[6].str();
                    val.erase(std::remove(val.begin(), val.end(), ';'), val.end());
                    intermediateCode.push_back("t" + std::to_string(tempVarCount++) + " = " + val);
                    intermediateCode.push_back(varName + " = t" + std::to_string(tempVarCount-1));
                    lineNode->children.push_back(new Node("ICG: TAC Generated"));
                }
            }

            // Array Bounds Check
            std::regex arrayAccess(R"((\w+)\[(\d+)\])");
            if (std::regex_search(line, match, arrayAccess)) {
                std::string vName = match[1];
                int idx = std::stoi(match[2]);
                if (arraySizes.count(vName) && idx >= arraySizes[vName]) {
                    reports.push_back({lineNum, "SEMANTIC", "Out-of-Bounds", "RECOVERY: Checked."});
                }
            }
            lineNum++;
        }
        return reports;
    }
};
#endif
