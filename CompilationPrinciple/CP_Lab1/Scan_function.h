#ifndef SCAN_FUNCTION_H
#define SCAN_FUNCTION_H

#include "Token.h"

using namespace std;

/* 符号表打印 */
void printTokenCodes();

void print(TokenCode codes, ofstream &outputFile);

/* 词法分析函数 */
bool isKey(const string &token);

int getKeyID(const string &token);

bool isLetter(char letter);

bool isDigit(char digit);

void lexicalAnalysis(ifstream &file, ofstream &outputFile);

/* 文件前置处理函数 */
void preprocess(string &inputFilePath, string &outputFilePath);

void processFile(string &inputFilePath, string &outputFilePath);

void processAllFiles(string& inputDir, string& outputDir);

void processSingleFile(string inputFilePath);

void choice(string &inputDir,string &outputDir);

#endif //SCAN_FUNCTION_H
