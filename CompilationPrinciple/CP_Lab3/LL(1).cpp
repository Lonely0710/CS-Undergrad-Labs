#ifndef LL_1_CPP
#define LL_1_CPP
#include "LL(1).h"

string ptr;
string start; // 起始符号，用于标识文法的开始符号
string table[100][100]; // 解析表，用于存储LL(1)文法的解析信息，二维数组形式
string look_like;
set<string> VtPlus; // 终结符集合加上结束符号'#'

// 存储文法规则的相关集合和映射
set<string> prod; // 存储产生式的集合
map<string, set<string>> splProd; // 存储分裂产生式的映射，键是非终结符，值是对应的产生式集合
set<string> Vt; // 终结符集合
set<string> Vn; // 非终结符集合
map<string, set<string>> first; // 存储First集的映射，键是非终结符，值是对应的First集合
map<string, set<string>> follow; // 存储Follow集的映射，键是非终结符，值是对应的Follow集合

bool isVn(string s) {
    return Vn.find(s) != Vn.end();
}

bool isVt(string s) {
    return Vt.find(s) != Vt.end();
}

int getVn(string s) {
    int cntVn = 0;
    for (set<string>::iterator it = Vn.begin(); it != Vn.end(); it++, cntVn++) {
        if (*it == s)
            return cntVn;
    }
    return -1;
}

int getVtPlus(string s) {
    int cntVt = 0;
    for (set<string>::iterator it = VtPlus.begin(); it != VtPlus.end(); it++, cntVt++) {
        if (*it == s)
            return cntVt;
    }
    return -1;
}

// 读取文法规则及分析方法
void readGrammar() {
    string line;
    ifstream in("../LL1.txt");

    // 初始化
    if (in) {
        getline(in, line);
        prod.insert(line);
        start = line.substr(0, 1);
        follow[line.substr(0, 1)].insert("#");//#表示初始符号的FOLLOW集
        while (getline(in, line)) prod.insert(line);
    }
    int position = 0;
    for (set<string>::iterator it = prod.begin(); it != prod.end(); it++) {
        string temp = *it;
        for (int i = 0; i < temp.length(); i++) {
            position = temp.find("->");
            string s = temp.substr(0, position);
            string ss = temp.substr(position + 2);
            set<string>sss;
            string t;
            for (int j = 0; j < ss.length(); j++) {
                if (ss[j] == '|') {
                    sss.insert(t);
                    t = "";
                }
                else  t.append(ss.substr(j, 1));
            }
            sss.insert(t);
            splProd.insert(pair<string, set<string> >(s, sss));
        }
    }

    // 求非终结&终结符号
    for (set<string>::iterator it = prod.begin(); it != prod.end(); it++) {
        string temp = *it;
        for (int i = 0; i < temp.length(); i++) {
            if ((temp[i] == '-' && temp[i + 1] == '>') || temp[i] == '|' || temp[i] == '>')  continue;
            if (temp[i] >= 'A' && temp[i] <= 'Z') {//大写字母为非终结符
                if (temp[i + 1] == '\'') {//带'的为非终结符
                    Vn.insert(temp.substr(i, 2));
                    i++;
                }
                else  Vn.insert(temp.substr(i, 1));
            }
            else  Vt.insert(temp.substr(i, 1));//小写字母为终结符
        }
    }

    cout << endl << "非终结符集合：" << endl;
    cout << "    ";
    for (set<string>::iterator it = Vn.begin(); it != Vn.end(); it++)  cout << *it << " ";
    cout << endl;

    cout << endl << "终结符集合：" << endl;
    cout << "    ";
    for (set<string>::iterator it = Vt.begin(); it != Vt.end(); it++)  cout << *it << " ";
    cout << endl;

    VtPlus = Vt;
    VtPlus.insert("#");
}

// 求某个非终结符的 First 集
set<string> OneFirst(string s) {
    if (splProd.count(s) > 0) { // 如果该非终结符有产生式
        set<string> temp = splProd[s]; // 获取该非终结符的所有产生式
        for (set<string>::iterator it = temp.begin(); it != temp.end(); it++) {
            string stemp = *it;
            if (stemp == "^")  first[s].insert("^"); // 如果产生式是空串，直接将空串加入 First 集
            else {
                int flagAll = 0; // 标记所有符号的 First 集是否包含空串
                for (int i = 0; i < stemp.length(); i++) {
                    int flag = 0; // 标记当前符号的 First 集是否包含空串
                    if (stemp[i + 1] == '\'') { // 处理带单引号的非终结符
                        set<string> t1 = OneFirst(stemp.substr(i, 2)); // 递归求当前符号的 First 集
                        for (set<string>::iterator ii = t1.begin(); ii != t1.end(); ii++) {
                            if (*ii == "^")  flag = 1; // 如果包含空串，设置标记
                            else  first[s].insert(*ii); // 否则将其加入 First 集
                        }
                        i++; // 跳过单引号后的字符
                    }
                        // 如果是普通非终结符
                    else if (isVn(stemp.substr(i, 1))) {
                        set<string> t2 = OneFirst(stemp.substr(i, 1)); // 递归求当前非终结符的 First 集
                        for (set<string>::iterator ii = t2.begin(); ii != t2.end(); ii++) {
                            if (*ii == "^")  flag = 1; // 如果包含空串，设置标记
                            else  first[s].insert(*ii); // 否则将其加入 First 集
                        }
                    }
                        // 如果是终结符
                    else  first[s].insert(stemp.substr(i, 1)); // 直接加入 First 集
                    if (flag == 1 && i == stemp.length() - 1)  flagAll = 1; // 如果所有符号的 First 集包含空串，设置标记
                    if (flag == 0)  break; // 遇到不包含空串的符号时停止
                }
                if (flagAll == 1)  first[s].insert("^"); // 如果所有符号的 First 集包含空串，加入空串
            }
        }
    }
    return first[s];
}

// 求所有非终结符的 First 集
void findFirstAssemble() {
    for (map<string, set<string> >::iterator it = splProd.begin(); it != splProd.end(); it++) {
        OneFirst(it->first); // 逐个非终结符调用 OneFirst 函数
    }
    cout << endl << "对应的 First 集：" << endl;

    for (map<string, set<string> >::iterator it = first.begin(); it != first.end(); it++) {
        cout << "    " << it->first << ":";
        for (set<string>::iterator ii = it->second.begin(); ii != it->second.end(); ii++) {
            cout << "    " << *ii << "  "; // 打印 First 集
        }
        cout << endl;
    }
}

// FOLLOW 集计算的部分功能（处理右侧符号 FOLLOW 集）
void repeat() {
    for (const auto& [left, rightSet] : splProd) { // 遍历每个产生式
        for (const string& right : rightSet) {
            for (int j = right.length() - 1; j >= 0; j--) { // 从右向左遍历产生式
                string now;
                if (j > 0 && right[j] == '\'') { // 处理带单引号的符号
                    now = right.substr(j - 1, 2);
                    j--;
                } else {
                    now = right.substr(j, 1);
                }

                if (isVt(now)) break; // 如果是终结符，停止处理
                else { // 如果是非终结符
                    // 将左部 FOLLOW 集传播到当前符号
                    follow[now].insert(follow[left].begin(), follow[left].end());
                }

                // 如果当前符号的 FIRST 集不包含空串，停止处理
                if (first[now].find("^") == first[now].end()) break;
            }
        }
    }
}

// 计算每个非终结符的 FOLLOW 集
void findFollowAssemble() {
    // 初始化 FOLLOW 集，将 # 加入起始符号的 FOLLOW 集
    follow[start].insert("#");

    // 遍历产生式，计算 FOLLOW 集
    for (const auto& [left, rightSet] : splProd) {
        for (const string& right : rightSet) {
            for (int i = 0; i < right.length(); i++) {
                string current;
                if (i + 1 < right.length() && right[i + 1] == '\'') {
                    current = right.substr(i, 2); // 处理带引号的符号
                    i++;
                } else {
                    current = right.substr(i, 1); // 普通符号
                }

                if (!isVt(current)) { // 如果是非终结符
                    string next;
                    if (i + 1 < right.length()) {
                        if (i + 2 < right.length() && right[i + 2] == '\'') {
                            next = right.substr(i + 1, 2); // 处理带引号的后续符号
                            i++;
                        } else {
                            next = right.substr(i + 1, 1);
                        }
                    }

                    // 将后续符号的 FIRST 集（去除空串）添加到当前符号的 FOLLOW 集
                    if (!next.empty()) {
                        for (const string& f : first[next]) {
                            if (f != "^") {
                                follow[current].insert(f);
                            }
                        }

                        // 如果后续符号的 FIRST 集包含空串，添加左部的 FOLLOW 集
                        if (first[next].count("^")) {
                            follow[current].insert(follow[left].begin(), follow[left].end());
                        }
                    } else {
                        // 如果当前符号是最后一个，直接继承左部的 FOLLOW 集
                        follow[current].insert(follow[left].begin(), follow[left].end());
                    }
                }
            }
        }
    }

    // 迭代调用 repeat，确保 FOLLOW 集完全传播
    for (int k = 0; k < 5; ++k) { // 根据需要调整迭代次数
        repeat();
    }

    // 输出 FOLLOW 集
    cout << "对应的 FOLLOW 集：" << endl;
    for (const auto& [nonTerminal, followSet] : follow) {
        if (!followSet.empty() && isVn(nonTerminal)) { // 仅输出非空集合且为合法非终结符
            cout << "    " << nonTerminal << ":   ";
            for (const string& f : followSet) {
                cout << f << "    ";
            }
            cout << endl;
        }
    }
}

// 生成LL(1)解析表
void parseTable() {
    int cntNon = 0;
    for (map<string, set<string> >::iterator it = splProd.begin(); it != splProd.end(); it++, cntNon++) {
        for (set<string>::iterator tmp = it->second.begin(); tmp != it->second.end(); tmp++) {
            if (isVt((*tmp).substr(0, 1))) {
                // 如果是终结符，直接添加到表中
                if (*tmp != "^") {
                    table[cntNon][getVtPlus((*tmp).substr(0, 1))] = *tmp;
                }
                if (*tmp == "^") {
                    // 如果是结束符，将对应非终结符的Follow集合中的元素添加到表中
                    for (map<string, set<string> >::iterator in = follow.begin(); in != follow.end(); in++) {
                        if (in->first == it->first) {
                            for (set<string>::iterator ii = in->second.begin(); ii != in->second.end(); ii++) {
                                if (isVt(*ii) || (*ii) == "#") table[cntNon][getVtPlus(*ii)] = "^";
                            }
                        }
                    }
                }
            }
            else { // 如果是非终结符，根据First集合添加到表中
                for (map<string, set<string> >::iterator in = first.begin(); in != first.end(); in++) {
                    if (in->first == (*tmp).substr(0, 1)) {
                        for (set<string>::iterator ii = in->second.begin(); ii != in->second.end(); ii++) {
                            if (*ii == "^") {
                                // 如果First集合中是结束符，将对应非终结符的Follow集合中的元素添加到表中
                                for (map<string, set<string> >::iterator in = follow.begin(); in != follow.end(); in++) {
                                    if (in->first == it->first) {
                                        for (set<string>::iterator ii = in->second.begin(); ii != in->second.end(); ii++) {
                                            if (isVt(*ii) || (*ii) == "#") { table[cntNon][getVtPlus(*ii)] = "^"; }
                                        }
                                    }
                                }
                            }
                            else table[cntNon][getVtPlus(*ii)] = *tmp;
                        }
                    }
                }
            }
        }
    }

    // 打印LL(1)解析表

    cout << "---------------------------------LL(1)解析表------------------------------" << endl;
    cout << "-----------------------------------------------------------------------------------------" << endl;
    cout << "\t|";
    for (set<string>::iterator in = VtPlus.begin(); in != VtPlus.end(); in++)
        cout << *in << "\t|";
    cout << endl;
    cout << "-----------------------------------------------------------------------------------------" << endl;
    set<string>::iterator it = Vn.begin();
    for (int i = 0; i < Vn.size(); i++, it++) {
        cout << *it << "\t|";
        for (int j = 0; j < VtPlus.size(); j++) {
            cout << table[i][j] << "\t|";
        }
        cout << endl;
        cout << "-----------------------------------------------------------------------------------------";
        cout << endl;
    }
    // cout << "******************************************************************************************" << endl;
}

// 获取下一位置并解析
bool nxtPos(FILE** fp) {
    char buf[32];
    memset(buf, 0, sizeof(buf)); // 初始化缓冲区为 0
    if (!feof(*fp))  // 如果文件未到结尾
        fgets(buf, 32, *fp); // 读取一行
    else
        return false; // 文件结束，返回 false

    string word = buf; // 转换为字符串
    if (!word.compare(""))  return false; // 如果为空行，返回 false

    string typevalue;
    typevalue = word.substr(word.find('(') + 1, word.find(',') - word.find('(') - 1); // 提取类型值
    string value;
    value = word.substr(word.find(',') + 2, word.find_last_of(')') - word.find(',') - 3); // 提取具体值

    cout << value;
    ptr = value; // 更新全局变量 `ptr`
    if (typevalue == "1")
        ptr = "i"; // 如果类型为 "1"，将值替换为 "i"
    look_like += ptr;  // 累积符号流
    return true; // 返回 true 表示成功解析一行
}

// 语法分析主函数
void analysis() {
    vector<string> stack; // 分析栈
    vector<string> left;  // 剩余输入符号
    vector<string> temp;  // 临时存储
    FILE* fp;
    fp = fopen("../text.txt", "r+"); // 打开输入文件
    left.push_back("#"); // 在剩余符号中加入结束符
    cout << endl;
    cout << "输入表达式:";

    // 读取文件中所有符号到临时存储
    while (nxtPos(&fp)) {
        temp.push_back(ptr);
    }

    cout << endl;
    cout << "待分析串为:" << look_like << endl; // 输出符号流
    cout << endl;

    // 将临时存储逆序加入到剩余输入符号中
    for (vector<string>::reverse_iterator it = temp.rbegin(); it != temp.rend(); it++)
        left.push_back(*it);

    // 表头
    cout << "---------------------------------------------------------------------------------------------" << endl;
    cout << "|" << setw(20) << "分析栈" << "|" << setw(20) << "剩余输入符号" << "|" << setw(25) << "对应产生式 |" << endl;
    cout << "---------------------------------------------------------------------------------------------" << endl;

    stack.push_back("#"); // 初始化栈，加入结束符
    stack.push_back(start); // 加入文法起始符号

    // 主分析循环
    while (left.size() > 0) {
        string outputs = ""; // 用于打印分析栈内容
        for (int i = 0; i < stack.size(); i++)
            outputs += stack[i]; // 拼接栈内容
        cout << "|" << setw(20) << outputs;

        outputs = ""; // 用于打印剩余符号内容
        for (int i = left.size() - 1; i >= 0; i--)
            outputs += left[i]; // 拼接剩余输入符号
        cout << "|" << setw(20) << outputs;

        // 获取栈顶符号和剩余符号的末尾
        vector<string>::iterator it = stack.end() - 1;
        vector<string>::iterator is = left.end() - 1;
        string f1 = *it;
        string f2 = *is;

        if (f1 == f2 && f1 == "#") { // 如果栈和输入都为结束符，成功匹配
            cout << "|" << setw(25) << "全部匹配成功!" << endl;
            return;
        }

        if (f1 == f2) { // 如果栈顶和输入符号相同，匹配成功
            stack.pop_back();
            left.pop_back();
            cout << "|" << setw(15) << "'" << f1 << "'" << "匹配成功" << endl;
        }
        else if (table[getVn(f1.substr(0, 1))][getVtPlus(f2)] != "") { // 检查预测分析表
            stack.pop_back(); // 弹出栈顶
            for (map<string, set<string>>::iterator it = splProd.begin(); it != splProd.end(); it++) {
                if (it->first == f1.substr(0, 1) || it->first == f1.substr(0, 2)) {
                    string production = table[getVn(f1.substr(0, 1))][getVtPlus(f2)];
                    if (production != "^") { // 如果产生式不是空串
                        for (int i = production.length(); i > 0; i--) {
                            string rec = production.substr(i - 1, i); // 从后向前处理产生式右侧
                            if (rec == "'") { // 特殊处理带单引号符号
                                rec = production.substr(i - 2, 2);
                                i--;
                            }
                            stack.push_back(rec); // 推入栈
                        }
                    }
                    cout << "|" << setw(20) << it->first << "->" << production << endl;
                }
            }
        }
        else { // 出错情况
            cout << "|" << setw(20) << "匹配失败" << endl;
            return;
        }
    }
}

#endif