#include "OPG.h"

int main()
{
    int n = readGrammar("../OPG.txt");

    // 检查文法是否有效
    if (n <= 0)
    {
        cerr << "文法加载失败，请检查文件内容。" << endl;
        return -1;
    }

    cout << "共读取到 " << n << " 条文法。" << endl;

    // 计算 FIRSTVT 和 LASTVT
    getFirstVT(n);
    getLastVT(n);
    generateTable(n);

    // 打印非终结符和终结符
    cout << "---------------------------------------------------" << endl;
    printSet("非终结符有：VN", VN);
    printSet("终结符有：VT", VT);
    cout << "---------------------------------------------------" << endl;

    // 打印 FIRSTVT 和 LASTVT
    printFirstLastVT("FIRSTVT", VN, firstvt);
    cout << endl;
    printFirstLastVT("LASTVT", VN, lastvt);

    // 打印算符优先分析表
    cout << "---------------------------------------------------" << endl
         << "算符优先分析表如下：" << endl;
    printTable(VT, table);

    cout << "---------------------------------------------------" << endl;
    char sentence[N];
    strcpy(sentence, "1+2*3");
    cout << "待识别的句子：" << sentence << endl;
    cout << "---------------------------------------------------" << endl;
    cout << "步骤" << '\t' << "栈" << '\t' << "输入串" << '\t' << "动作" << endl;
    analyze(sentence);

    return 0;
}
