#include "LL(1).h"

int main(int argc, const char* argv[]) {
    cout << "-----------------------------------" << endl;
    cout << "22231301 杨龙澧"<<endl;
    cout<<"LL1语法分析器程序" << endl;
    cout << "-----------------------------------" << endl;
    cout << "ͨ通过文件读入LL(1)文法" << endl;

    readGrammar();
    findFirstAssemble();
    findFollowAssemble();
    parseTable();
    analysis();
    cout << "---------------------------------------------------------------------------------------------" << endl;
    return 0;
}
