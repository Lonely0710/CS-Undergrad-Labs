#ifndef TOKEN_H
#define TOKEN_H

#include <iostream>
#include <fstream> // 用于文件流操作
#include <filesystem>
#include <cctype>  // 用于字符判断函数 isalpha, isdigit 等
#include <string>
#include <iomanip> // 用于格式化输出
#include <chrono>

using namespace std;

namespace fs = filesystem; // 引入文件系统库的命名空间

/* 单词编码 */
enum TokenCode {
    /*未定义*/       TK_UNDEF = 0,
    /* 关键字 */     KW_VOID,   // void关键字
                    KW_MAIN,   // main关键字
                    KW_INT,    // int关键字
                    KW_DOUBLE, // double关键字
                    KW_FOR,    // for关键字
                    KW_WHILE,  // while关键字
                    KW_SWITCH, // switch关键字
                    KW_CASE,   // case关键字
                    KW_IF,     // if关键字
                    KW_ELSE,   // else关键字
                    KW_RETURN, // return关键字
    /* 运算符 */     TK_PLUS,   // +加号
                    TK_MINUS,  // -减号
                    TK_STAR,   // *乘号
                    TK_DIVIDE, // /除号
                    TK_ASSIGN, // =赋值运算符
                    TK_EQ,     // ==等于号
                    TK_LT,     // <小于号
                    TK_LEQ,    // <=小于等于号
                    TK_GT,     // >大于号
                    TK_GEQ,    // >=大于等于号
                    TK_NOT,    // 表示 "!"
                    TK_AND,    // 表示 "&&"
                    TK_NEQ,    // 表示 "!="
                    TK_MOD,    // 表示 "%="
                    TK_DECREMENT, // 表示 "--"
                    TK_DOT,         // 点运算符
                    TK_COLON,       // 冒号
                    TK_REF,     // & 引用运算符
    /* 分隔符 */     TK_OPENPA,    // (左圆括号
                    TK_CLOSEPA,   // )右圆括号
                    TK_OPENBR,    // [左中括号
                    TK_CLOSEBR,   // ]右中括号
                    TK_BEGIN,     // {左大括号
                    TK_END,       // }右大括号
                    TK_COMMA,     // ,逗号
                    TK_SEMOCOLOM, // ;分号
                    TK_ADD_ASSIGN, // +=
                    TK_SUB_ASSIGN, // -=
                    TK_MUL_ASSIGN, // *=
                    TK_DIV_ASSIGN, // /=
                    TK_MOD_ASSIGN, // %=
                    TK_OR,       // ||
                    TK_SHL,       // <<
                    TK_SHR,       // >>
                    TK_CHAR_LITERAL, // 字符字面量
    /* 常量 */       TK_INT,    // 整型常量
                    TK_DOUBLE, // 浮点型常量
    /* 标识符 */     TK_IDENT
};

/* 全局变量 */
extern TokenCode code;      // 记录单词的种别码
extern unordered_map<TokenCode, string> tokenSymbols;


extern string token;        // 存储当前的单词
extern int row;             // 行数

const int MAX = 12;         // 假设最大关键字数量
extern string keyWord[MAX]; // 存储关键词

#endif //TOKEN_H
