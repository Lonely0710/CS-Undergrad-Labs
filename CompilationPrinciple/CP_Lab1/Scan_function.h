#ifndef SCAN_FUNCTION_H
#define SCAN_FUNCTION_H

#include "Token.h"

using namespace std;

void printTokenCodes();

void print(TokenCode codes, ofstream &outputFile);

void preprocess(string &inputFilePath, string &outputFilePath);

void pre_processAllFiles(const string& inputDir, const string& outputDir);

bool isKey(const string &token);

int getKeyID(const string &token);

bool isLetter(char letter);

bool isDigit(char digit);

void lexicalAnalysis(ifstream &file, ofstream &outputFile);

void processFile(string &inputFilePath, string &outputFilePath);

void processAllFiles(string& inputDir, string& outputDir);

#endif //SCAN_FUNCTION_H
