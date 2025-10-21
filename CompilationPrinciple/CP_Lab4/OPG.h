#ifndef OPG_H
#define OPG_H

#include <iostream>
#include <string>
#include <fstream>
#include <set>
#include <map>
#include <cstdlib>
#include <string.h>

using namespace std;

#define N 100
#define true 1
#define false -1

extern char V[N][N]; //  存储拆分后的产生式
extern char VN[N],VT[N];  // 储存非终结、终结符号
extern char firstvt[N][N],lastvt[N][N],table[N][N];

int readGrammar(const string &filename);
int getVsets(int n); // 获取文法中的非终结符和终结符
void getFirstVT(int n);
void getLastVT(int n);

void printSet(const char *title, const char *set);
void printFirstLastVT(const char *label, const char *vnSet, const char sets[][N]);
void printTable(const char *vtSet, const char table[][N]);

void processData(const char *data, char symbol); // 处理三种算符优先顺序
void generateTable(int n);
void analyze(char *sentence);
#endif