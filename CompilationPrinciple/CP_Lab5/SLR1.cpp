#include "SLR1.h"

string new_temp()
{
    return "t" + to_string(++temp_var_count);
}

void add_dots(struct state *I)
{
    for (int i = 0; i < I->prod_count; i++)
    {
        for (int j = 99; j > 3; j--)
            I->prod[i][j] = I->prod[i][j - 1];
        I->prod[i][3] = '.';
    }
}

void augument(struct state *S, struct state *I)
{
    if (I->prod[0][0] == 'S')
        strcpy(S->prod[0], "Z->.S");
    else
    {
        strcpy(S->prod[0], "S->.");
        S->prod[0][4] = I->prod[0][0];
    }
    S->prod_count++;
}

void get_prods(struct state *I)
{
    ifstream infile("SLR1.txt");
    if (!infile)
    {
        cerr << "无法打开文件 SLR1.txt，请检查文件路径或文件是否存在！" << endl;
        exit(1);
    }

    // 初始化数据
    I->prod_count = 0;
    no_nt = 0;
    no_t = 0;

    string line;
    while (getline(infile, line))
    {
        if (line.empty())
            continue; // 跳过空行

        // 读取产生式并保存
        strcpy(I->prod[I->prod_count], line.c_str());
        I->prod_count++;

        // 提取产生式左部作为非终结符
        char lhs = line[0];
        if (strchr(non_terminals, lhs) == nullptr)
        {
            non_terminals[no_nt++] = lhs;
        }

        // 提取右部符号，区分终结符和非终结符
        for (size_t i = 3; i < line.size(); i++)
        {
            char symbol = line[i];
            if (isalpha(symbol))
            {
                if (isupper(symbol))
                { // 非终结符
                    if (strchr(non_terminals, symbol) == nullptr)
                    {
                        non_terminals[no_nt++] = symbol;
                    }
                }
                else
                { // 终结符
                    if (strchr(terminals, symbol) == nullptr)
                    {
                        terminals[no_t++] = symbol;
                    }
                }
            }
        }
    }

    infile.close();

    // 显示读取的内容
    cout << "从文件中读取的产生式数量：" << I->prod_count << endl;
    for (int i = 0; i < I->prod_count; i++)
    {
        cout << I->prod[i] << endl;
    }

    cout << "非终结符数量：" << no_nt << endl;
    for (int i = 0; i < no_nt; i++)
    {
        cout << non_terminals[i] << " ";
    }
    cout << endl;

    cout << "终结符数量：" << no_t << endl;
    for (int i = 0; i < no_t; i++)
    {
        cout << terminals[i] << " ";
    }
    cout << endl;
}

bool is_non_terminal(char a)
{
    if (a >= 'A' && a <= 'Z')
        return true;
    else
        return false;
}

bool in_state(struct state *I, char *a)
{
    for (int i = 0; i < I->prod_count; i++)
    {
        if (!strcmp(I->prod[i], a))
            return true;
    }
    return false;
}

char char_after_dot(char a[100])
{
    char b;
    for (int i = 0; i < strlen(a); i++)
        if (a[i] == '.')
        {
            b = a[i + 1];
            return b;
        }
}

char *move_dot(char b[100], int len)
{
    char a[100] = {};
    strcpy(a, b);
    for (int i = 0; i < len; i++)
    {
        if (a[i] == '.')
        {
            swap(a[i], a[i + 1]);
            break;
        }
    }
    return &a[0];
}

bool same_state(struct state *I0, struct state *I)
{

    if (I0->prod_count != I->prod_count)
        return false;

    for (int i = 0; i < I0->prod_count; i++)
    {
        int flag = 0;
        for (int j = 0; j < I->prod_count; j++)
            if (strcmp(I0->prod[i], I->prod[j]) == 0)
                flag = 1;
        if (flag == 0)
            return false;
    }
    return true;
}

void closure(struct state *I, struct state *I0)
{
    char a;
    for (int i = 0; i < I0->prod_count; i++)
    {
        a = char_after_dot(I0->prod[i]);
        if (is_non_terminal(a))
        {
            for (int j = 0; j < I->prod_count; j++)
            {
                if (I->prod[j][0] == a)
                {
                    if (!in_state(I0, I->prod[j]))
                    {
                        strcpy(I0->prod[I0->prod_count], I->prod[j]);
                        I0->prod_count++;
                    }
                }
            }
        }
    }
}

void goto_state(struct state *I, struct state *S, char a)
{
    int time = 1;
    for (int i = 0; i < I->prod_count; i++)
    {
        if (char_after_dot(I->prod[i]) == a)
        {
            if (time == 1)
            {
                time++;
            }
            strcpy(S->prod[S->prod_count], move_dot(I->prod[i], strlen(I->prod[i])));
            S->prod_count++;
        }
    }
}

void print_prods(struct state *I)
{
    for (int i = 0; i < I->prod_count; i++)
        printf("%s\n", I->prod[i]);
    cout << endl;
}

bool in_array(char a[20], char b)
{
    for (int i = 0; i < strlen(a); i++)
        if (a[i] == b)
            return true;
    return false;
}

char *chars_after_dots(struct state *I)
{
    char a[20] = {};
    for (int i = 0; i < I->prod_count; i++)
    {
        if (!in_array(a, char_after_dot(I->prod[i])))
        {
            a[strlen(a)] = char_after_dot(I->prod[i]);
        }
    }
    return &a[0];
}

void cleanup_prods(struct state *I)
{
    char a[100] = {};
    for (int i = 0; i < I->prod_count; i++)
        strcpy(I->prod[i], a);
    I->prod_count = 0;
}

int return_index(char a)
{
    for (int i = 0; i < no_t; i++)
        if (terminals[i] == a)
            return i;
    for (int i = 0; i < no_nt; i++)
        if (non_terminals[i] == a)
            return no_t + i;
}

void print_shift_table(int state_count)
{
    cout << "******** 移进动作表 *********" << endl
         << endl;
    cout << "\t";
    for (int i = 0; i < no_t; i++)
        cout << terminals[i] << "\t";
    for (int i = 0; i < no_nt; i++)
        cout << non_terminals[i] << "\t";
    cout << endl;
    for (int i = 0; i < state_count; i++)
    {
        std::vector<int> arr(no_nt + no_t, -1);
        for (int j = 0; j < state_count; j++)
        {
            if (goto_table[i][j] != '~')
            {
                arr[return_index(goto_table[i][j])] = j;
            }
        }
        cout << "I" << i << "\t";
        for (int j = 0; j < no_nt + no_t; j++)
        {
            if (i == 1 && j == no_t - 1)
                cout << "ACC" << "\t";
            if (arr[j] == -1 || arr[j] == 0)
                cout << "\t";
            else
            {
                if (j < no_t)
                    cout << "S" << arr[j] << "\t";
                else
                    cout << arr[j] << "\t";
            }
        }
        cout << "\n";
    }
}

int get_index(char c,char *a){
for(int i=0;i<strlen(a);i++)
    if(a[i]==c)
    return i;
}

void add_dot_at_end(struct state* I){
    for(int i=0;i<I->prod_count;i++){
        strcat(I->prod[i],".");
    }
}

void add_to_first(int n,char b){
   for(int i=0;i<strlen(first[n]);i++)
        if(first[n][i]==b)
            return;
    first[n][strlen(first[n])]=b;
}

void add_to_first(int m,int n){
    for(int i=0;i<strlen(first[n]);i++){
            int flag=0;
        for(int j=0;j<strlen(first[m]);j++){
            if(first[n][i]==first[m][j])
                flag=1;
        }
        if(flag==0)
            add_to_first(m,first[n][i]);
    }
}

void add_to_follow(int n,char b){
   for(int i=0;i<strlen(follow[n]);i++)
        if(follow[n][i]==b)
            return;
    follow[n][strlen(follow[n])]=b;
}

void add_to_follow(int m,int n){
    for(int i=0;i<strlen(follow[n]);i++){
            int flag=0;
        for(int j=0;j<strlen(follow[m]);j++){
            if(follow[n][i]==follow[m][j])
                flag=1;
        }
        if(flag==0)
            add_to_follow(m,follow[n][i]);
    }
}

void add_to_follow_first(int m,int n){
    for(int i=0;i<strlen(first[n]);i++){
            int flag=0;
        for(int j=0;j<strlen(follow[m]);j++){
            if(first[n][i]==follow[m][j])
                flag=1;
        }
        if(flag==0)
            add_to_follow(m,first[n][i]);
    }
}

void find_first(struct state *I){
    for(int i=0;i<no_nt;i++){
        for(int j=0;j<I->prod_count;j++){
            if(I->prod[j][0]==non_terminals[i]){
                if(!is_non_terminal(I->prod[j][3])){
                    add_to_first(i,I->prod[j][3]);
                    }

            }
        }
    }
}

void find_follow(struct state *I){
    for(int i=0;i<no_nt;i++){
        for(int j=0;j<I->prod_count;j++){
            for(int k=3;k<strlen(I->prod[j]);k++){
                if(I->prod[j][k]==non_terminals[i]){
                    if(I->prod[j][k+1]!='\0'){
                        if(!is_non_terminal(I->prod[j][k+1])){
                            add_to_follow(i,I->prod[j][k+1]);
                        }
                    }
                }
            }
        }
    }
}

int get_index(int *arr,int n){
    for(int i=0;i<no_t;i++){
        if(arr[i]==n)
            return i;
    }
    return -1;
}

void print_reduce_table(int state_count,int *no_re,struct state *temp1){
    cout<<"********** 规约动作表 **********"<<endl<<endl;
    cout<<"\t";
    std::vector<std::vector<int> > arr(temp1->prod_count, std::vector<int>(no_t, -1));
    for(int i=0;i<no_t;i++){
        cout<<terminals[i]<<"\t";
    }
    cout<<endl;
    for(int i=0;i<temp1->prod_count;i++){
    int n=no_re[i];
    for(int j=0;j<strlen(follow[return_index(temp1->prod[i][0])-no_t]);j++){
        for(int k=0;k<no_t;k++){
            if(follow[return_index(temp1->prod[i][0])-no_t][j]==terminals[k])
                arr[i][k]=i+1;
        }
    }
    cout<<"I"<<n<<"\t";
    for(int j=0;j<no_t;j++){
        if(arr[i][j]!=-1&&arr[i][j]!=0&&arr[i][j]<state_count)
            cout<<"R"<<arr[i][j]<<"\t";
        else
            cout<<"\t";
    }
    cout<<endl;
    }
}

// 初始化所有产生式
void init_productions(struct state *init)
{
    for (int i = 0; i < init->prod_count; i++)
    {
        all_productions.push_back(string(init->prod[i]));
    }
}

// SLR(1) 分析函数
void SLR_Analysis(string input, int state_count)
{
    stack<int> state_stack;     // 状态栈
    stack<string> symbol_stack; // 符号栈
    int input_index = 0;        // 输入指针
    input += "#";               // 加入结束符号

    state_stack.push(0); // 初始状态
    cout << "分析栈 | 输入 | 动作\n";
    cout << "-----------------------------------\n";

    while (true)
    {
        int state = state_stack.top();
        char lookahead = input[input_index];
        char action = reduce[state][return_index(lookahead)];

        // 输出分析栈和输入串
        stack<int> temp_state = state_stack;
        vector<int> states;
        while (!temp_state.empty())
        {
            states.insert(states.begin(), temp_state.top());
            temp_state.pop();
        }

        cout << "[ ";
        for (int s : states)
            cout << s << " ";
        cout << "] | ";
        cout << input.substr(input_index) << " | ";

        if (action == 's') // 移入
        {
            int next_state = goto_table[state][return_index(lookahead)];
            cout << "移入 " << lookahead << endl;

            state_stack.push(next_state);
            symbol_stack.push(string(1, lookahead));
            input_index++;
        }
        else if (action == 'r') // 规约
        {
            int prod_index = goto_table[state][return_index(lookahead)] - '0';
            string prod = all_productions[prod_index];

            cout << "规约 " << prod[0] << " -> " << prod.substr(3) << endl;

            // 提取规约产生式的右部符号
            int prod_len = prod.length() - 3;
            vector<string> right_side;
            for (int i = 0; i < prod_len; i++)
            {
                right_side.insert(right_side.begin(), symbol_stack.top());
                symbol_stack.pop();
                state_stack.pop();
            }

            // 根据产生式构造四元式
            if (prod[3] == '+' || prod[3] == '-' || prod[3] == '*' || prod[3] == '/')
            {
                string arg2 = right_side[2];
                string op = right_side[1];
                string arg1 = right_side[0];
                string result = new_temp();
                quads.push_back({op, arg1, arg2, result});
                symbol_stack.push(result); // 将结果压入符号栈
            }
            else
            {
                symbol_stack.push(right_side[0]); // 直接规约
            }

            // 转移到下一个状态
            char head = prod[0];
            int next_state = goto_table[state_stack.top()][return_index(head)];
            state_stack.push(next_state);
        }
        else if (action == 'a') // 接受
        {
            cout << "接受" << endl;
            break;
        }
        else
        {
            cout << "ERROR: 无法识别输入" << endl;
            return;
        }
    }
}

void print_quadruples()
{
    cout << "\n生成的四元式：" << endl;
    cout << "Op\tArg1\tArg2\tResult" << endl;
    for (const auto &quad : quads)
    {
        cout << quad.op << "\t" << quad.arg1 << "\t" << quad.arg2 << "\t" << quad.result << endl;
    }
}