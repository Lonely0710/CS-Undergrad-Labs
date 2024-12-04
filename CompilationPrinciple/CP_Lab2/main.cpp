#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string.h>
#include <cstdio>
#define MAX_SIZE 128

using namespace std;

char str[MAX_SIZE];
int current = 0;
int flag = 0;
string result;

void S();
void E();
void E_();
void T();
void T_();
void F();
void A();
void M();
void V();

void If(); // 解析 if 语句
void While(); // 解析 while 语句
void Begin(); // 解析 begin...end 语句块
void L(); // 解析语句列表
void A_(); // 解析赋值语句

int main() {
    FILE* fp = NULL;
    char target[MAX_SIZE];
    int length;
    if ((fp = fopen("../test1.txt", "r")) == NULL) {
        printf("文件打开失败\n");
        exit(-1);
    }

    printf("开始解析文本内容：\n");
    while ((fgets(target, MAX_SIZE, fp) != NULL)) {
        length = strlen(target);
        target[length] = '\0';
        cout << target << endl;
        int mark = 1; // 用来标记是否遇到字符 ')'
        for (int i = 0; i < length; i++) {
            if (mark == 1 && i > 1 && target[i - 1] == '\'') {
                result += target[i];
                if (target[i - 3] == '1') {
                    str[current] = 'i';
                    current++;
                } else if (target[i - 3] == '3') {
                    str[current] = target[i];
                    current++;
                }
                mark = 0;
            }
            if (target[i] == ')' && target[i + 1] != '\'') {
                mark = 1;
            }
        }
    }

    str[current] = '#';
    current++;
    str[current] = '\0';
    current = 0;

    cout <<"输入语句翻译为：" << endl << result << endl << endl;
    printf("解析结果为：\n%s\n\n", str);
    printf("开始解析...\n");
    S();
    if (flag == 0) {
        printf("\n解析成功！\n");
    } else {
        printf("\n解析失败！\n");
    }
    fclose(fp);
    return 0;
}

void S() {
    // printf("S ");
    if (str[current] == 'i' && str[current + 1] == 'f') { // 解析 if 语句
        If();
    } else if (str[current] == 'i') {
        V();
        if (str[current] == '=') {
            current++;
            E();
        }
    } else if (str[current] == 'w' && str[current + 1] == 'h') {
        While();
    } else if (str[current] == 'b' && str[current + 1] == 'e' && str[current + 2] == 'g' && str[current + 3] == 'i' && str[current + 4] == 'n') {
        Begin();
    } else if (str[current] == 'A') {
        A_();
    } else {
        printf("\nS 语法错误，解析失败\n");
        flag = 1;
        return;
    }
}

void If() {
    // printf("If ");
    // 如果当前字符是 'i' 下一个字符是 'f'
    if (str[current] == 'i' && str[current + 1] == 'f') {
        current += 2; // 跳过 "if"

        E(); // 解析条件表达式
        S(); // 解析 then 部分
        if (str[current] == 'e' && str[current + 1] == 'l' && str[current + 2] == 's' && str[current + 3] == 'e') {
            current += 4; // 跳过 "else"
            S(); // 解析 else 部分
        }
    } else {
        printf("\nIf语句解析失败\n");
        flag = 1;
    }
}

void While() {
    //printf("While ");
    // 如果当前字符是 'w' 下一个字符是 'h'
    if (str[current] == 'w' && str[current + 1] == 'h') {
        current += 2; // 跳过 "while"
        E(); // 解析条件表达式
        S(); // 解析 do 部分
    } else {
        printf("\nWhile语句解析失败\n");
        flag = 1;
    }
}

void Begin() {
    //printf("Begin ");
    if (str[current] == 'b' && str[current + 1] == 'e' && str[current + 2] == 'g' && str[current + 3] == 'i' && str[current + 4] == 'n') {
        current += 5; // 跳过 "begin"
        L(); // 解析语句块
        if (str[current] == 'e' && str[current + 1] == 'n' && str[current + 2] == 'd') {
            current += 3; // 跳过 "end"
        } else {
            printf("\nEnd keyword expected after statement block!\n");
            flag = 1;
        }
    } else {
        printf("\nBegin语句解析失败\n");
        flag = 1;
    }
}

void L() {
    //printf("L ");
    // 循环解析 S 直到遇到 ';' 或者 '#'
    while (str[current] != ';' && str[current] != '#') {
        S(); // 解析单个语句
        if (str[current] == ';') {
            current++; // 跳过 ';'
        }
    }
}

void A_() {
    // printf("A' ");
    V(); // 解析变量
    if (str[current] == '=') {
        current++; // 跳过 '='
        E(); // 解析表达式
    } else {
        printf("\nAssignment operator '=' expected!\n");
        flag = 1;
    }
}

void E() {
    //printf("E ");
    if (str[current] == 'i' || str[current] == '(') {
        T();
        E_();
    } else {
        printf("\nE语句解析失败\n");
        flag = 1;
        return;
    }
}

void E_() {
    //printf("E' ");
    while (str[current] == '+' || str[current] == '-') {
        A();
        T();
    }
    if (str[current] == '#' || str[current] == ')') {
        return; // 符合FOLLOW集
    } else {
        printf("\nE'语句解析失败\n");
        flag = 1;
        return;
    }
}

void T() {
    //printf("T ");
    if (str[current] == 'i' || str[current] == '(') {
        F();
        T_();
    } else {
        printf("\nT语句解析失败\n");
        flag = 1;
        return;
    }
}

void T_() {
    //printf("T' ");
    while (str[current] == '*' || str[current] == '/') {
        M();
        F();
    }
    if (str[current] == '+' || str[current] == '-' || str[current] == '#' || str[current] == ')') {
        return; // 符合FOLLOW集
    } else {
        printf("\nT'语句解析失败\n");
        flag = 1;
        return;
    }
}

void F() {
    //printf("F ");
    if (str[current] == '(') {
        current++;
        E();
        if (str[current] == ')') {
            current++;
        } else {
            printf("\nF语句解析失败\n");
            flag = 1;
            return;
        }
    } else if (str[current] == 'i') {
        current++;
    } else {
        printf("\nF语句解析失败\n");
        flag = 1;
        return;
    }
}

void A() {
    //printf("A ");
    if (str[current] == '+' || str[current] == '-') {
        current++;
    } else {
        printf("\nA语句解析失败\n");
        flag = 1;
        return;
    }
}

void M() {
    // printf("M ");
    if (str[current] == '*' || str[current] == '/') {
        current++;
    } else {
        printf("\nM语句解析失败\n");
        flag = 1;
        return;
    }
}

void V() {
    // printf("V ");
    if (str[current] == 'i') {
        current++;
    } else {
        printf("\nV语句解析失败\n");
        flag = 1;
        return;
    }
}
