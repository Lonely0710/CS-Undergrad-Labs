#ifndef SLR1_H
#define SLR1_H

#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <vector>
#include <stack>

using namespace std;

extern char terminals[100]; // 终结符数组
extern int no_t; // 终结符数量
extern char non_terminals[100]; // 非终结符数组
extern int no_nt; // 非终结符数量
extern char goto_table[100][100]; // GOTO表
extern char reduce[20][20]; // 规约表
extern char follow[20][20]; // FOLLOW集合
extern char fo_co[20][20]; // FOLLOW和FIRST集合的交集
extern char first[20][20]; // FIRST集合

extern vector<string> all_productions; // 定义一个全局变量，用于存储所有产生式
extern int temp_var_count;             // 临时变量计数器

struct state
{
    int prod_count;
    char prod[100][100] = {{}};
};

// 四元式数据结构
struct Quadruple
{
    string op, arg1, arg2, result;
};

extern vector<Quadruple> quads; // 四元式列表

/* ==================== 状态处理函数 ==================== */
void add_dots(struct state *I);                            // 在状态 I 的产生式中添加初始的点符号('.')
void augument(struct state *S, struct state *I);           // 扩展状态 I 的产生式到状态 S
void get_prods(struct state *I);                           // 获取状态 I 的产生式集合
bool is_non_terminal(char a);                              // 判断字符 a 是否是非终结符
bool in_state(struct state *I, char *a);                   // 判断产生式 a 是否在状态 I 中
char char_after_dot(char a[100]);                          // 获取字符串 a 中点符号('.') 之后的字符
char *move_dot(char b[100], int len);                      // 将字符串 b 中的点符号向右移动一位
bool same_state(struct state *I0, struct state *I);        // 判断两个状态 I0 和 I 是否相同
void closure(struct state *I, struct state *I0);           // 计算状态 I 的闭包，将结果存入状态 I0
void goto_state(struct state *I, struct state *S, char a); // 通过字符 a 从状态 I 转移到状态 S
void print_prods(struct state *I);                         // 打印状态 I 的产生式集合
bool in_array(char a[20], char b);                         // 判断字符 b 是否在数组 a 中
char *chars_after_dots(struct state *I);                   // 获取状态 I 中所有点符号后面的字符集合
void cleanup_prods(struct state *I);                       // 清理状态 I 中的产生式（例如删除重复项）
void add_dot_at_end(struct state *I);                      // 在状态 I 的所有产生式末尾添加点符号('.')

/* ==================== 符号与字符处理函数 ==================== */
bool is_non_terminal(char a);            // 判断字符 a 是否是非终结符
char char_after_dot(char a[100]);        // 获取字符串 a 中点符号('.') 之后的字符
char *move_dot(char b[100], int len);    // 将字符串 b 中的点符号向右移动一位
char *chars_after_dots(struct state *I); // 获取状态 I 中所有点符号后面的字符集合
bool in_array(char a[20], char b);       // 判断字符 b 是否在数组 a 中

/* ==================== 打印与调试函数 ==================== */
void print_prods(struct state *I);                                         // 打印状态 I 的产生式集合
void print_shift_table(int state_count);                                   // 打印移入表，显示 state_count 个状态的信息
void print_reduce_table(int state_count, int *no_re, struct state *temp1); // 打印规约表，显示 state_count 个状态的信息，基于参数 no_re 和临时状态 temp1

/* ==================== 索引与数组操作函数 ==================== */
int return_index(char a);       // 返回字符 a 在某个表或数组中的索引
int get_index(char c, char *a); // 返回字符 c 在字符串 a 中的索引
int get_index(int *arr, int n); // 返回整数 n 在数组 arr 中的索引

/* ==================== First 和 Follow 集相关函数 ==================== */
void add_to_first(int n, char b);       // 将字符 b 添加到第 n 个非终结符的 First 集中
void add_to_first(int m, int n);        // 将第 n 个非终结符的 First 集合并到第 m 个非终结符的 First 集中
void add_to_follow(int n, char b);      // 将字符 b 添加到第 n 个非终结符的 Follow 集中
void add_to_follow(int m, int n);       // 将第 n 个非终结符的 Follow 集合并到第 m 个非终结符的 Follow 集中
void add_to_follow_first(int m, int n); // 将第 n 个非终结符的 First 集合并到第 m 个非终结符的 Follow 集中
void find_first(struct state *I);       // 计算状态 I 中所有非终结符的 First 集
void find_follow(struct state *I);      // 计算状态 I 中所有非终结符的 Follow 集

/* ==================== SLR(1)分析主函数 ==================== */
void SLR_Analysis(string input, int state_count); // SLR(1) 分析函数
void init_productions(struct state *init);        // 初始化所有产生式
void print_quadruples(); // 打印四元式列表

#endif // SLR1_H