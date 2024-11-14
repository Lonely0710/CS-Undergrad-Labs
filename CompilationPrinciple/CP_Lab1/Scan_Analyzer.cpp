# include "Token.h"

/* ————————常量赋值———————— */
TokenCode code = TK_UNDEF;
int row = 1;
string token;
string keyWord[MAX] = {
        // 数据类型&常用
        "void", "int", "double", "float", "char", "bool", "short", "long", "unsigned","cout", "cin", "endl",
        // 管理流程
        "if", "else", "switch", "case", "for", "while", "do", "break", "continue", "return",
        // 类访问
        "public", "protected", "private",
        // 类和结构体
        "class", "struct", "union", "enum", "namespace", "template", "typename", "friend",
        "virtual", "operator","using", "this", "new", "delete",
        // 异常处理
        "try", "catch", "throw",
        // 标准库函数
        "std", "sizeof","nullptr", "operator",
        // 特殊功能
        "inline", "static","extern", "asm", "main"
};

// 创建一个映射，将每个 TokenCode 映射到对应的符号字符串
unordered_map<TokenCode, string> tokenSymbols = {
        {TK_UNDEF, "Undefined"},
        {KW_VOID, "void"},          {KW_INT, "int"},           {KW_DOUBLE, "double"},   {KW_FLOAT, "float"},
        {KW_CHAR, "char"},           {KW_BOOL, "bool"},         {KW_SHORT, "short"},     {KW_LONG, "long"},
        {KW_UNSIGNED, "unsigned"},   {KW_IF, "if"},             {KW_ELSE, "else"},       {KW_SWITCH, "switch"},
        {KW_CASE, "case"},           {KW_FOR, "for"},           {KW_WHILE, "while"},     {KW_DO, "do"},
        {KW_BREAK, "break"},         {KW_CONTINUE, "continue"}, {KW_RETURN, "return"},   {KW_PUBLIC, "public"},
        {KW_PROTECTED, "protected"}, {KW_PRIVATE, "private"},   {KW_CLASS, "class"},     {KW_STRUCT, "struct"},
        {KW_UNION, "union"},         {KW_ENUM, "enum"},         {KW_NAMESPACE, "namespace"}, {KW_TEMPLATE, "template"},
        {KW_TYPENAME, "typename"},   {KW_FRIEND, "friend"},     {KW_VIRTUAL, "virtual"}, {KW_OPERATOR, "operator"},
        {KW_USING, "using"},         {KW_THIS, "this"},         {KW_NEW, "new"},         {KW_DELETE, "delete"},
        {KW_TRY, "try"},             {KW_CATCH, "catch"},       {KW_THROW, "throw"},     {KW_STD, "std"},
        {KW_SIZEOF, "sizeof"},       {KW_NULLPTR, "nullptr"},   {KW_OPERATOR_FUNC, "operator"}, {KW_INLINE, "inline"},
        {KW_STATIC,     "static"},       {KW_EXTERN, "extern"},     {KW_ASM, "asm"},          {KW_MAIN, "main"},
        {KW_COUT,       "cout"},           {KW_CIN, "cin"},           {KW_ENDL, "endl"},
        // 运算符
        {TK_PLUS,       "+"},{TK_MINUS, "-"},{TK_STAR, "*"},{TK_DIVIDE, "/"},{TK_ASSIGN, "="},{TK_EQ, "=="},{TK_LT, "<"},
        {TK_LEQ,        "<="},{TK_GT, ">"},{TK_GEQ, ">="},{TK_NOT, "!"},{TK_AND, "&&"},{TK_NEQ, "!="},
        {TK_MOD,        "%"},{TK_DECREMENT, "--"},{TK_DOT, "."},{TK_COLON, ":"},{TK_REF, "&"},
        // 分隔符
        {TK_OPENPA,     "("},{TK_CLOSEPA, ")"},
        {TK_OPENBR,     "["},{TK_CLOSEBR, "]"},
        {TK_BEGIN,      "{"},{TK_END, "}"},
        {TK_COMMA,      ","},{TK_SEMICOLON, ";"},
        {TK_ADD_ASSIGN, "+="},{TK_SUB_ASSIGN, "-="},{TK_MUL_ASSIGN, "*="},{TK_DIV_ASSIGN, "/="},{TK_MOD_ASSIGN, "%="},
        {TK_OR,         "||"},{TK_SHL, "<<"},{TK_SHR, ">>"},{TK_DELIMITER,"\""},
        // 常量"
        {TK_INT,        "INT"},{TK_FLOAT, "Float"}, {TK_DOUBLE, "Double"}, {TK_CHAR_LITERAL, "TXT/String"},

        // 标识符
        {TK_IDENT,      "Identifier"}
};

/* ————————打印函数———————— */
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
        if (code >= KW_VOID && code <= KW_MAIN) type = "关键字";
        else if (code >= TK_PLUS && code <= TK_REF) type = "运算符";
        else if (code >= TK_OPENPA && code <= TK_DELIMITER) type = "分隔符";
        else if (code >= TK_INT && code <= TK_CHAR_LITERAL) type = "常量";
        else if (code == TK_IDENT) type = "标识符";
        else type = "未定义";

        cout << left << setw(20) << symbol << setw(20) << type << setw(20) << code << endl;
    }

    cout << "--------------------------------------------------------" << endl;
}

// 将词法分析结果写入输出文件
void print(TokenCode code, ofstream &outputFile) {
    // 根据传入的 TokenCode，将对应的词法单元和值写入输出文件
    if (code == TK_UNDEF) {
        outputFile << '(' << code << ',' << token << ") 未识别的符号在第 " << row << " 行。" << endl;
        return;
    }

    // 根据 TokenCode 的范围判断类别
    if (code >= KW_VOID && code <= KW_MAIN) {  // 关键字范围
        outputFile << '(' << code << ',' << token << ") 关键字" << endl;
    }
    else if (code >= TK_PLUS && code <= TK_REF) {  // 运算符范围
        outputFile << '(' << code << ',' << token << ") 运算符" << endl;
    }
    else if (code >= TK_OPENPA && code <= TK_DELIMITER) {  // 分隔符范围
        outputFile << '(' << code << ',' << token << ") 分隔符" << endl;
    }
    else if (code >= TK_INT || code <= TK_CHAR_LITERAL) {  // 常量范围
        if (code == TK_INT) {
            outputFile << '(' << code << ',' << atoi(token.c_str()) << ") 整型常量" << endl;
        }
        else if(code == TK_FLOAT){
            outputFile << '(' << code << ',' << atof(token.c_str()) << ") 浮点型常量float" << endl;
        }
        else if(code == TK_DOUBLE){
            outputFile << '(' << code << ',' << atof(token.c_str()) << ") 浮点型常量double" << endl;
        } else{
            outputFile << '(' << code << ',' << token << ") 字符字面量" << endl;
        }
    }
    else if (code == TK_IDENT) {  // 标识符
        outputFile << '(' << code << ',' << token << ") 标识符" << endl;
    }
    else {
        outputFile << "未识别的TokenCode: " << code << endl;  // 未识别的 TokenCode
    }
}

/* ————————辅助检测函数———————— */
// 1. 检索关键字
bool isKey(string token)
{
    for (int i = 0; i < MAX; i++)
    {
        if (token.compare(keyWord[i]) == 0)
            return true;
    }
    return false;
}

// keyWord数组查找
int  getKeyID(string token)
{
    for (int i = 0; i < MAX; i++)
    {	//关键字的内码值为keyWord数组中对应的下标加1
        if (token.compare(keyWord[i]) == 0)
            return i+1;
    }
    return -1;
}

// 2. 检查字符是否为字母
bool isLetter(char letter) {
    return (letter >= 'a' && letter <= 'z') || (letter >= 'A' && letter <= 'Z');
}

// 3. 检查字符是否为数字
bool isDigit(char digit) {
    return (digit >= '0' && digit <= '9');
}

/* ————————预处理 & 词法分析———————— */
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
    int lineNumber = 0; // 行号，用于错误报告

    while (getline(inputFile, line)) {
        lineNumber++;
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
                // 检测多行注释开始
                if (i + 1 < line.length() && ch == '/' && line[i + 1] == '*') {
                    inComment = true; // 进入多行注释
                    i++; // 跳过 '*'
                }
                // 检测单行注释开始
                else if (i + 1 < line.length() && ch == '/' && line[i + 1] == '/') {
                    // 检查是否后面有内容，没有则报错
                    if (line.length() == i + 2) {
                        cerr << "警告: 第 " << lineNumber << " 行的单行注释没有跟随内容" << endl;
                    }
                    break; // 跳过到行尾，结束当前行的处理
                }
                else {
                    outputFile << ch;
                    lineHasContent = true;
                }
                i++;
            }
        }

        // 如果这一行有有效内容，写入换行符
        if (lineHasContent && !inComment) {
            outputFile << endl;
        }
    }

    // 如果文件结束但仍然处于多行注释状态，报错
    if (inComment) {
        cerr << "错误: 多行注释在文件末尾未闭合" << endl;
    }

    inputFile.close();
    outputFile.close();
}

void lexicalAnalysis(ifstream &file, ofstream &outputFile) {
    char ch;
    std::stack<std::pair<char, int>> bracketStack; // 栈记录括号及其行号

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

        // 识别字符串字面量
        else if (ch == '"') {
            print(TK_DELIMITER, outputFile);
            token.clear();
            bool isClosed = false;

            // 继续读取字符直到找到配对的引号
            while (file.get(ch)) {
                // 检测到转义字符，忽略下一个字符的特殊含义
                if (ch == '\\' && file.peek() == '"') {
                    token.push_back(ch); // 加入转义字符
                    file.get(ch);        // 获取并加入被转义的引号
                    token.push_back(ch);
                    continue;
                }

                // 找到配对的引号，字符串字面量闭合
                if (ch == '"') {
                    isClosed = true;
                    break;
                }
                token.push_back(ch);  // 添加字符串中的字符
            }

            if (isClosed) {
                print(TK_CHAR_LITERAL, outputFile);
                token.clear();
                token.push_back('"');
                code = TK_DELIMITER;  // 标记为字符串字面量
            } else {
                // 抛出异常，未闭合的字符串
                std::ostringstream errorMsg;
                errorMsg << "\n>_< Error at line " << row << ": unmatched '\"'";
                throw std::runtime_error(errorMsg.str());
            }
        }

        // 识别运算符和分隔符代码
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
                        token += "=";
                        code = TK_LEQ; // <=
                    } else if (file.peek() == '<') {
                        file.get();
                        token += "<";
                        code = TK_SHL; // <<
                    } else {
                        code = TK_LT; // <
                    }
                    break;
                case '>':
                    if (file.peek() == '=') {
                        file.get();
                        token += "=";
                        code = TK_GEQ; // >=
                    } else if (file.peek() == '>') {
                        file.get();
                        token += ">";
                        code = TK_SHR; // >>
                    } else {
                        code = TK_GT; // >
                    }
                    break;
                case '.':
                    code = TK_DOT; break; // 点运算符
                case '(':
                    bracketStack.push({'(', row});
                    code = TK_OPENPA;
                    break;
                case ')':
                    if (bracketStack.empty() || bracketStack.top().first != '(') {
                        std::ostringstream errorMsg;
                        errorMsg << "\n>_< Error at line " << row
                                 << ": unmatched ')'";
                        throw std::runtime_error(errorMsg.str());
                    } else {
                        bracketStack.pop();
                    }
                    code = TK_CLOSEPA;
                    break;
                case '[':
                    bracketStack.push({'[',row});
                    code = TK_OPENBR;
                    break;
                case ']':
                    if (bracketStack.empty() || bracketStack.top().first != '[') {
                        std::ostringstream errorMsg;
                        errorMsg << "\n>_< Error at line " << row
                                 << ": unmatched ']'";
                        throw std::runtime_error(errorMsg.str());
                    } else {
                        bracketStack.pop();
                    }
                    code = TK_CLOSEBR;
                    break;
                case '{':
                    bracketStack.push({'{',row});
                    code = TK_BEGIN;
                    break;
                case '}':
                    if (bracketStack.empty() || bracketStack.top().first != '{') {
                        std::ostringstream errorMsg;
                        errorMsg << "\n>_< Error at line " << row
                                 << ": unmatched '}'";
                        throw std::runtime_error(errorMsg.str());
                    } else {
                        bracketStack.pop();
                    }
                    code = TK_END;
                    break;
                case ',': code = TK_COMMA; break;
                case ';': code = TK_SEMICOLON; break;
                case ':': code = TK_COLON; break; // 冒号
                case '&':
                    if (file.peek() == '&') {
                        file.get();
                        code = TK_AND; // &&
                    } else {
                        code = TK_REF; // &
                    }
                    break;
                case '\'':
                    code = TK_CHAR_LITERAL; break; // 字符字面量
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
                        code = TK_UNDEF; // 单独的 | 未定义
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

    // 检查是否有未闭合的左括号
    while (!bracketStack.empty()) {
        char unclosedBracket = bracketStack.top().first;
        int unclosedLine = bracketStack.top().second;
        bracketStack.pop();

        std::ostringstream errorMsg;
        errorMsg << "\n Error at line " << unclosedLine
                 << ": unmatched '" << unclosedBracket << "'";
        throw std::runtime_error(errorMsg.str());
    }
}

/* ————————文件处理函数———————— */
// 文件处理函数
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

// 处理单个文件
void processSingleFile(string inputFilePath) {

    // 确保输入文件存在
    if (!fs::exists(inputFilePath)) {
        cerr << "输入文件不存在: " << inputFilePath << endl;
        return;
    }

    // 设置文件名和目录路径
    string fileName = fs::path(inputFilePath).filename().string();
    string outputDir = "../output";
    string filteredDir = "../filtered";

    // 创建输出目录和过滤目录
    if (!fs::exists(outputDir)) {
        fs::create_directory(outputDir);
    }
    if (!fs::exists(filteredDir)) {
        fs::create_directory(filteredDir);
    }

    // 设置输出文件路径
    string outputFilePath = outputDir + "/output_" + fileName;
    string filteredFilePath = filteredDir + "/filtered_" + fileName;

    // 打开文件流
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

    // 获取当前时间并写入词法分析文件开头
    auto now = chrono::system_clock::now();
    time_t currentTime = chrono::system_clock::to_time_t(now);
    tm *localTime = localtime(&currentTime);
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

// 选择交互函数
void choice(string &inputDir,string &outputDir){
    int choice;
    cout << "请选择要执行的操作：" << endl;
    cout << "1. 处理单个文件" << endl;
    cout << "2. 批量处理文件" << endl;
    cout << "3. 打印符号对照表" << endl;
    cout << "请输入选择 : ";
    cin >> choice;

    // 根据选择调用不同的处理函数
    if (choice == 1) {
        string inputFilePath;

        // 输入文件路径并检查其有效性
        while (true) {
            cout << "请输入要处理的文件路径: ";
            cin >> inputFilePath;

            // 确保输入文件存在
            if (!fs::exists(inputFilePath)) {
                cerr << "输入文件不存在，请重新输入文件路径。" << endl;
            } else {
                processSingleFile(inputFilePath);
                cout << "^_^ 单个文件处理完成 -> 结果保存在输出目录: " << outputDir << endl;
                break;
            }
        }
    }  else if (choice == 2) {
        cout << "请将待处理的文件放入 " << inputDir << " 文件夹中。" << endl;
        cout << "开始批量处理文件..." << endl;
        processAllFiles(inputDir, outputDir);
        cout << "^_^ 批量文件处理完成 -> 所有结果已保存在输出目录: " << outputDir << endl;
    }  else if(choice == 3) {
        printTokenCodes();
    }else {
        cout << "无效选择，请输入 1 或 2。" << endl;
    }
}