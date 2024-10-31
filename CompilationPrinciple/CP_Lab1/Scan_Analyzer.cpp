# include "Token.h"

unordered_map<string, string> macros;  // 存储宏定义的映射

// 创建一个映射，将每个 TokenCode 映射到对应的符号字符串
unordered_map<TokenCode, string> tokenSymbols = {
        {TK_UNDEF, "Undefined"},
        // 关键字
        {KW_VOID, "void"},
        {KW_MAIN, "main"},
        {KW_INT, "int"},
        {KW_DOUBLE, "double"},
        {KW_FOR, "for"},
        {KW_WHILE, "while"},
        {KW_SWITCH, "switch"},
        {KW_CASE, "case"},
        {KW_IF, "if"},
        {KW_ELSE, "else"},
        {KW_RETURN, "return"},
        // 运算符
        {TK_PLUS, "+"},
        {TK_MINUS, "-"},
        {TK_STAR, "*"},
        {TK_DIVIDE, "/"},
        {TK_ASSIGN, "="},
        {TK_EQ, "=="},
        {TK_LT, "<"},
        {TK_LEQ, "<="},
        {TK_GT, ">"},
        {TK_GEQ, ">="},
        {TK_NOT, "!"},
        {TK_AND, "&&"},
        {TK_NEQ, "!="},
        {TK_MOD, "%"},
        {TK_DECREMENT, "--"},
        {TK_DOT, "."},
        {TK_COLON, ":"},
        {TK_REF, "&"},
        // 分隔符
        {TK_OPENPA, "("},
        {TK_CLOSEPA, ")"},
        {TK_OPENBR, "["},
        {TK_CLOSEBR, "]"},
        {TK_BEGIN, "{"},
        {TK_END, "}"},
        {TK_COMMA, ","},
        {TK_SEMOCOLOM, ";"},
        {TK_ADD_ASSIGN, "+="},
        {TK_SUB_ASSIGN, "-="},
        {TK_MUL_ASSIGN, "*="},
        {TK_DIV_ASSIGN, "/="},
        {TK_MOD_ASSIGN, "%="},
        {TK_OR, "||"},
        {TK_SHL, "<<"},
        {TK_SHR, ">>"},
        {TK_CHAR_LITERAL, "Char"},
        // 常量
        {TK_INT, "INT"},
        {TK_DOUBLE, "Float"},
        // 标识符
        {TK_IDENT, "Identifier"}
};

// 打印所有单词符号及其类别编码的表格（按 TokenCode 逆序输出）
void printTokenCodes() {
    vector<pair<TokenCode, string>> sortedTokens(tokenSymbols.begin(), tokenSymbols.end());

    // 按 TokenCode 从小到大排序
    sort(sortedTokens.begin(), sortedTokens.end(), [](const auto& a, const auto& b) {
        return a.first < b.first;
    });

    // 输出表头
    cout << left << setw(20) << "单词符号" << setw(20) << "种类" << setw(20) <<"种别码" << endl;
    cout << "--------------------------------------------------------" << endl;

    // 按顺序输出每个符号的单词符号、种类和种别码
    for (const auto& [code, symbol] : sortedTokens) {
        string type;
        if (code >= KW_VOID && code <= KW_RETURN) type = "关键字";
        else if (code >= TK_PLUS && code <= TK_REF) type = "运算符";
        else if (code >= TK_OPENPA && code <= TK_SHR) type = "分隔符";
        else if (code == TK_INT || code == TK_DOUBLE) type = "常量";
        else if (code == TK_IDENT) type = "标识符";
        else type = "未定义";

        cout << left << setw(20) << symbol << setw(20) << type << setw(20) << code << endl;
    }

    cout << "--------------------------------------------------------" << endl;
}

// 将词法分析结果写入输出文件
void print(TokenCode code, ofstream &outputFile) {
    // 根据传入的TokenCode，将对应的词法单元和值写入输出文件
    switch (code) {
            // 处理未识别的符号
        case TK_UNDEF:
            outputFile << '(' << code << ',' << token << ") 未识别的符号在第 " << row << " 行。" << endl;
            return;

            // 处理关键字
        case KW_VOID:
        case KW_MAIN:
        case KW_INT:
        case KW_DOUBLE:
        case KW_FOR:
        case KW_WHILE:
        case KW_SWITCH:
        case KW_CASE:
        case KW_IF:
        case KW_ELSE:
        case KW_RETURN:
            outputFile << '(' << code << ',' << token << ") " << endl;
            break;

            // 处理运算符
        case TK_PLUS:
        case TK_MINUS:
        case TK_STAR:
        case TK_DIVIDE:
        case TK_ASSIGN:
        case TK_EQ:
        case TK_LT:
        case TK_LEQ:
        case TK_GT:
        case TK_GEQ:
        case TK_NOT:
        case TK_AND:
        case TK_NEQ:
        case TK_MOD:
        case TK_DECREMENT:
        case TK_DOT:
        case TK_REF:
        case TK_ADD_ASSIGN:   // +=
        case TK_SUB_ASSIGN:   // -=
        case TK_MUL_ASSIGN:   // *=
        case TK_DIV_ASSIGN:   // /=
        case TK_MOD_ASSIGN:   // %=
        case TK_OR:           // ||
        case TK_SHL:          // <<
        case TK_SHR:          // >>
            outputFile << '(' << code << ',' << token << ") " << endl;
            break;

            // 处理分隔符
        case TK_OPENPA:
        case TK_CLOSEPA:
        case TK_OPENBR:
        case TK_CLOSEBR:
        case TK_BEGIN:
        case TK_END:
        case TK_COMMA:
        case TK_SEMOCOLOM:
        case TK_COLON: // 处理冒号
        case TK_CHAR_LITERAL:
            outputFile << '(' << code << ',' << token << ") " << endl;
            break;

            // 处理常量
        case TK_INT:
            outputFile << '(' << code << ',' << atoi(token.c_str()) << ")" << endl; // 整型
            return;
        case TK_DOUBLE:
            outputFile << '(' << code << ',' << atof(token.c_str()) << ") " << endl; // 浮点型
            return;

            // 处理标识符
        case TK_IDENT:
            outputFile << '(' << code << ',' << token << ") " << endl;
            break;

        default:
            outputFile << "未识别的TokenCode: " << code << endl; // 处理未识别的TokenCode
            break;
    }
}

bool isKey(string token)
{
    for (int i = 0; i < MAX; i++)
    {
        if (token.compare(keyWord[i]) == 0)
            return true;
    }
    return false;
}

int  getKeyID(string token)
{
    for (int i = 0; i < MAX; i++)
    {	//关键字的内码值为keyWord数组中对应的下标加1
        if (token.compare(keyWord[i]) == 0)
            return i+1;
    }
    return -1;
}

// 检查字符是否为字母
bool isLetter(char letter) {
    return (letter >= 'a' && letter <= 'z') || (letter >= 'A' && letter <= 'Z');
}

// 检查字符是否为数字
bool isDigit(char digit) {
    return (digit >= '0' && digit <= '9');
}

void preprocess(string &inputFilePath, string &outputFilePath) {
    ifstream inputFile(inputFilePath);
    ofstream outputFile(outputFilePath);

    if (!inputFile.is_open()) {
        cerr << "无法打开输入文件: " << inputFilePath << endl;
        return;
    }

    if (!outputFile.is_open()) {
        cerr << "无法创建输出文件: " << outputFilePath << endl;
        return;
    }

    string line;
    bool inComment = false; // 标记是否在多行注释中

    while (getline(inputFile, line)) {
        bool lineHasContent = false; // 标记这一行是否有有效内容

        // 处理头文件
        if (line.find("#include") != string::npos) {
            size_t start = line.find('"') + 1;
            size_t end = line.find('"', start);
            if (start != string::npos && end != string::npos) {
                string headerFile = line.substr(start, end - start);
                ifstream header(headerFile);
                if (header.is_open()) {
                    string headerLine;
                    while (getline(header, headerLine)) {
                        outputFile << headerLine << endl; // 将头文件内容写入输出
                    }
                    header.close();
                } else {
                    cerr << "无法打开头文件: " << headerFile << endl;
                }
            }
            continue; // 跳过头文件行
        }

        // 处理宏定义
        if (line.find("#define") != string::npos) {
            size_t start = line.find(" ") + 1; // 找到第一个空格后的起始位置
            size_t end = line.find(" ", start); // 找到第二个空格后的结束位置
            if (start != string::npos && end != string::npos) {
                string macroName = line.substr(start, end - start);
                string macroValue = line.substr(end + 1); // 剩余的部分为宏值
                macros[macroName] = macroValue; // 存储宏定义
            }
            continue; // 跳过宏定义行
        }

        // 处理条件编译
        if (line.find("#ifdef") != string::npos) {
            // 处理条件编译，例如检查宏是否定义
            // 这里可以根据具体需求实现
            continue;
        }

        // 处理注释
        size_t i = 0; // 当前字符索引

        while (i < line.length()) {
            char ch = line[i];
            // 检测多行注释
            if (inComment) {
                if (i + 1 < line.length() && ch == '*' && line[i + 1] == '/') {
                    inComment = false; // 结束多行注释
                    i++; // 跳过 '/'
                }
                i++; // 跳过多行注释内容
            } else {
                if (i + 1 < line.length() && ch == '/' && line[i + 1] == '*') {
                    inComment = true; // 进入多行注释
                    i++; // 跳过 '*'
                }
                    // 检测单行注释开始
                else if (i + 1 < line.length() && ch == '/' && line[i + 1] == '/') {
                    break; // 跳过到行尾，结束当前行的处理
                }
                else {
                    outputFile << ch;
                }
                i++;
                lineHasContent = true;
            }
        }

        // 如果这一行有有效内容，写入换行符
        if (lineHasContent && !inComment) {
            outputFile << endl;
        }
    }

    inputFile.close();
    outputFile.close();
}

// 词法分析函数，从输入文件流中读取代码，分析词法单元，并将结果写入输出文件流
void lexicalAnalysis(ifstream &file, ofstream &outputFile) {
    char ch;
    while (file.get(ch)) { // 从文件流中获取一个字符
        token.clear();      // 清空当前 token
        token.push_back(ch);

        // 跳过空格、制表符和换行符
        if (ch == ' ' || ch == '\t' || ch == '\n') {
            if (ch == '\n') row++; // 遇到换行符，行数加1
            continue;
        }

        // 识别关键字和标识符
        else if (isLetter(ch) || ch == '_') { // 允许下划线作为标识符的一部分
            token.clear(); // 清空 token，以便构建新的标识符或关键字
            token.push_back(ch); // 将第一个字符加入 token

            // 继续读取字符，直到遇到非法字符
            while (file.get(ch) && (isLetter(ch) || isDigit(ch) || ch == '_')) {
                token.push_back(ch); // 构建完整的标识符或关键字
            }
            file.unget();  // 退回一个字符，防止多读

            if (isKey(token)) // 判断是否为关键字
                code = TokenCode(getKeyID(token));
            else
                code = TK_IDENT;  // 非关键字则为标识符
        }

        // 识别数字和浮点常数
        else if (isDigit(ch)) {
            bool isDouble = false;
            while (file.get(ch) && (isDigit(ch) || (ch == '.' && !isDouble))) {
                if (ch == '.') isDouble = true;
                token.push_back(ch);
            }
            file.unget(); // 退回一个字符

            if (isDouble)
                code = TK_DOUBLE;
            else
                code = TK_INT;
        }

        // 识别运算符和分隔符
        else {
            switch (ch) {
                case '+':
                    if (file.peek() == '=') {
                        file.get();
                        code = TK_ADD_ASSIGN; // +=
                    } else {
                        code = TK_PLUS; // +
                    }
                    break;
                case '-':
                    if (file.peek() == '=') {
                        file.get();
                        code = TK_SUB_ASSIGN; // -=
                    } else if (file.peek() == '-') {
                        file.get();
                        code = TK_DECREMENT; // --
                    } else {
                        code = TK_MINUS; // -
                    }
                    break;
                case '*':
                    if (file.peek() == '=') {
                        file.get();
                        code = TK_MUL_ASSIGN; // *=
                    } else {
                        code = TK_STAR; // *
                    }
                    break;
                case '/':
                    if (file.peek() == '=') {
                        file.get();
                        code = TK_DIV_ASSIGN; // /=
                    } else {
                        code = TK_DIVIDE; // /
                    }
                    break;
                case '=':
                    if (file.peek() == '=') {
                        file.get();
                        code = TK_EQ; // ==
                    } else {
                        code = TK_ASSIGN; // =
                    }
                    break;
                case '<':
                    if (file.peek() == '=') {
                        file.get();
                        code = TK_LEQ; // <=
                    } else if (file.peek() == '<') {
                        file.get();
                        code = TK_SHL; // <<
                    } else {
                        code = TK_LT; // <
                    }
                    break;
                case '>':
                    if (file.peek() == '=') {
                        file.get();
                        code = TK_GEQ; // >=
                    } else if (file.peek() == '>') {
                        file.get();
                        code = TK_SHR; // >>
                    } else {
                        code = TK_GT; // >
                    }
                    break;
                case '.':
                    code = TK_DOT; break; // 识别点运算符
                case '(': code = TK_OPENPA; break;
                case ')': code = TK_CLOSEPA; break;
                case '[': code = TK_OPENBR; break;
                case ']': code = TK_CLOSEBR; break;
                case '{': code = TK_BEGIN; break;
                case '}': code = TK_END; break;
                case ',': code = TK_COMMA; break;
                case ';': code = TK_SEMOCOLOM; break;
                case ':': code = TK_COLON; break; // 识别冒号
                case '&':
                    if (file.peek() == '&') {
                        file.get();
                        code = TK_AND; // 识别逻辑与运算符
                    } else {
                        code = TK_REF; // 单独的 & 被视为引用运算符
                    }
                    break;
                case '\'': code = TK_CHAR_LITERAL; break; // 识别字符字面量
                case '!':
                    if (file.peek() == '=') {
                        file.get();
                        code = TK_NEQ; // !=
                    } else {
                        code = TK_NOT; // !
                    }
                    break;
                case '%':
                    if (file.peek() == '=') {
                        file.get();
                        code = TK_MOD_ASSIGN; // %=
                    } else {
                        code = TK_MOD; // %
                    }
                    break;
                case '|':
                    if (file.peek() == '|') {
                        file.get();
                        code = TK_OR; // ||
                    } else {
                        code = TK_UNDEF; // 单独的 | 被视为未定义
                    }
                    break;
                default:
                    code = TK_UNDEF;  // 未识别符号
                    break;
            }
        }

        // 将词法分析结果写入输出文件
        print(code, outputFile);
    }
}

// 预处理和词法分析文件的处理函数
void processFile(string &inputFilePath, string &outputDir, string &fileName) {
    string filteredDir = "../filtered";
    string outputFilePath = outputDir + "/output_" + fileName;
    string filteredFilePath = filteredDir + "/filtered_" + fileName;

    // 创建 filtered 目录
    if (!fs::exists(filteredDir)) {
        fs::create_directory(filteredDir);
    }

    ifstream inputFile(inputFilePath);
    ofstream outputFile(outputFilePath);
    ofstream filteredFile(filteredFilePath);

    if (!inputFile.is_open()) {
        cerr << "无法打开输入文件: " << inputFilePath << endl;
        return;
    }
    if (!outputFile.is_open()) {
        cerr << "无法创建输出文件: " << outputFilePath << endl;
        inputFile.close();
        return;
    }
    if (!filteredFile.is_open()) {
        cerr << "无法创建过滤文件: " << filteredFilePath << endl;
        inputFile.close();
        outputFile.close();
        return;
    }

    // 获取当前时间
    auto now = chrono::system_clock::now();
    time_t currentTime = chrono::system_clock::to_time_t(now);
    tm *localTime = localtime(&currentTime);

    // 在词法分析文件开头写入文件信息
    outputFile << "词法分析结果：" << endl;
    outputFile << "处理文件：" << inputFilePath << endl;
    outputFile << "处理日期：" << put_time(localTime, "%Y-%m-%d %H:%M:%S") << endl;

    // 记录开始时间
    auto start = chrono::high_resolution_clock::now();

    // 第一步：预处理，将结果写入 filtered 文件
    preprocess(inputFilePath, filteredFilePath);

    // 关闭输入文件与过滤文件
    inputFile.close();
    filteredFile.close();

    // 第二步：词法分析，将 filtered 文件内容写入 output 文件
    ifstream filteredInput(filteredFilePath);
    if (filteredInput.is_open()) {
        lexicalAnalysis(filteredInput, outputFile);
        filteredInput.close();
    } else {
        cerr << "无法打开过滤文件进行词法分析: " << filteredFilePath << endl;
    }

    // 记录结束时间
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end - start;

    // 输出文件处理时间
    cout << "文件 " << fileName << " 处理时间: " << duration.count() << " 秒" << endl;

    outputFile.close();
}

// 处理整个文件夹的所有文件
void processAllFiles(string &inputDir, string &outputDir) {
    try {
        if (!fs::exists(outputDir)) {
            fs::create_directory(outputDir);
        }

        // 遍历 inputDir 中的所有文件
        for (const auto& entry : fs::directory_iterator(inputDir)) {
            string inputFilePath = entry.path().string();
            string fileName = entry.path().filename().string();

            cout << "处理文件: " << fileName << endl;
            processFile(inputFilePath, outputDir, fileName);
        }

        cout << "所有文件处理完成。" << endl;

    } catch (const fs::filesystem_error& e) {
        cerr << "文件系统错误: " << e.what() << endl;
    }
}
