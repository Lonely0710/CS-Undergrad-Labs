#include "Scan_function.h"

TokenCode code = TK_UNDEF;
int row = 1;
string token;
string keyWord[MAX] = {"void", "main", "int", "double", "for", "while", "switch", "case", "if", "else", "return"}; // 存储关键词

int main()
{
    string inputDir = "../input";   // 输入文件夹路径
    string outputDir = "../output"; // 输出文件夹路径

//    printTokenCodes();
    cout << "------------------------------------------------------" << endl;
    processAllFiles(inputDir, outputDir);

    return 0;
}