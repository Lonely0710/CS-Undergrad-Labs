#ifndef LL_1_H
#define LL_1_H

#include <iostream>
#include <cstring>
#include <set>
#include <map>
#include <fstream>
#include <iomanip>
#include <string>
#include <cstdio>
#include <vector>

using namespace std;

// 读取文法规则及分析方法
void readGrammar();

// 求First集
set<string> OneFirst(string s);
void findFirstAssemble();

// 求Follow集
void repeat();
void findFollowAssemble();

// 构造分析表
void parseTable();
bool nxtPos(FILE** fp);

// LL1文法分析主函数
void analysis();

#endif //LL_1_H
