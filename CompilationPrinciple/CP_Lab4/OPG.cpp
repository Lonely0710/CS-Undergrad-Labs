#include "OPG.h"

char V[N][N];       // 存储拆分后的产生式
char VN[N];         // 储存非终结符号
char VT[N];         // 储存终结符号
char firstvt[N][N]; // 存储firstvt集合
char lastvt[N][N];  // 存储lastvt集合
char table[N][N];   // 优先分析表

// 从文件读取文法并存入 V 数组，返回文法数量
int readGrammar(const string &filename)
{
    ifstream file(filename);
    if (!file.is_open())
    {
        cout << "无法打开文件：" << filename << endl;
        return 0; // 返回 0 表示文件无法打开
    }

    int i = 0;
    char ch;
    while (file.get(ch)) // 逐字符读取文件
    {
        // 读取每一行
        string line = "";
        while (ch != '\n' && file.good()) // 读取直到换行符
        {
            line += ch;
            file.get(ch); // 继续读取下一个字符
        }

        // 如果读取到的内容是 "#", 说明结束
        if (line[0] == '#')
        {
            break;
        }

        // 将当前行存入 V[i]
        if (i < N)
        {
            line.copy(V[i], line.length());
            V[i][line.length()] = '\0'; // 确保字符串结束
            i++;
        }

        if (i >= N) // 检查文法数量是否超出限制
        {
            cout << "文法过多，超出数组限制！" << endl;
            break;
        }
    }

    file.close();

    // 验证文法是否正确
    int n = i;
    if (getVsets(n) == false)
    {
        cout << "文法左部可能有误，请检查后再重新输入。" << endl;
        return 0; // 返回 0 表示文法无效
    }
    else
    {
        cout << "文法加载完成。" << endl;
    }

    return n; // 返回文法数量
}

// 获取文法中的非终结符和终结符
int getVsets(int n)
{
    int flag = true; // 标记文法格式是否正确

    // 遍历每个文法产生式
    for (int i = 0; i < n; i++)
    {
        // 检查文法产生式是否符合规则：非终结符 -> 产生式
        if ((V[i][0] >= 'A' && V[i][0] <= 'Z') && V[i][1] == '-' && V[i][2] == '>')
        {
            bool sign = true;

            // 检查该非终结符是否已经存在于 VN 数组中
            for (int j = 0; j < strlen(VN); j++)
            {
                if (VN[j] == V[i][0])
                {
                    sign = false; // 已存在，则不再添加
                    break;
                }
            }

            // 如果 VN 中没有该非终结符，添加到 VN 数组
            if (sign == true)
            {
                VN[strlen(VN)] = V[i][0];
            }
        }
        else
        {
            // 如果文法格式不正确，设置 flag 为 false 并退出
            flag = false;
            break;
        }
    }

    // 如果文法格式不正确，直接返回 false
    if (!flag)
    {
        return false;
    }
    else
    {
        int k = 0; // 用于管理终结符 VT 数组的索引
        int l = 0; // 临时变量，用于检查终结符是否已存在

        // 遍历每个文法产生式的右部
        for (int i = 0; i < n; i++)
        {
            for (int j = 3; V[i][j] != '\0'; j++)
            {
                // 如果是终结符且不是 '|'
                if ((V[i][j] < 'A' || V[i][j] > 'Z') && V[i][j] != '|')
                {
                    // 检查该终结符是否已存在于 VT 数组中
                    for (l = 0; l < k; l++)
                    {
                        if (VT[l] == V[i][j])
                        {
                            break; // 已存在，不再添加
                        }
                    }

                    // 如果终结符不存在，添加到 VT 数组
                    if (l == k)
                    {
                        VT[k] = V[i][j];
                        k++; // 增加终结符数量
                    }
                }
            }
        }

        // 返回文法格式是否正确
        return true;
    }
}

// 求FIRSTVT
void getFirstVT(int n)
{
    // int point=0;//用于指向每一个产生式的前两个符号
    for(int i=0;i<n;i++)//首先是找到每个产生式的前两个符号是否是终结符，是就加入到对应的firstvt集中。
    {
        int flag=true;
        for(int j=3;;)
        {
            for(int k=0;k<strlen(VT);k++)
            {
                if(V[i][j]==VT[k])
                {
                    int mark=true;//用于检查firstvt集中是否已经存在该终结符。
                    for(int l=0;l<strlen(firstvt[i]);l++)
                    {
                        if(V[i][j]==firstvt[i][l])
                        {
                            mark=false;
                            break;
                        }
                    }
                    if(mark==true)//若不存在，则加入到firstvt集中
                    {
                        int length=strlen(firstvt[i]);
                        firstvt[i][length]=V[i][j];
                    }
                }
                if(V[i][j+1]==VT[k])
                {
                    int mark=true;
                    for(int l=0;l<strlen(firstvt[i]);l++)
                    {
                        if(V[i][j+1]==firstvt[i][l])
                        {
                            mark=false;
                            break;
                        }
                    }
                    if(mark==true)
                    {
                        int length=strlen(firstvt[i]);
                        firstvt[i][length]=V[i][j+1];
                    }
                }
            }
            while(V[i][j]!='|')
            {
                if(V[i][j]=='\0')
                {
                    flag=false;
                    break;
                }
                j++;
            }
            j++;
            if(flag==false)
            {
                break;
            }
        }
    }
    //下面的代码是循环查看哪些非终结符的firstvt集可以加入到另一些非终结符的firstvt集中，一直循环添加，直到每个非终结符的长度不再变化为止。
    int *origin=new int[n];//用于记录遍历之前的数组长度，看是否有变化。
    while(1)
    {
        int sign=true;//用于标识遍历前后firstvt是否有变化。mark，symbol。
        for(int i=0;i<n;i++)
        {
            if(origin[i]!=strlen(firstvt[i]))
            {
                sign=false;//长度有变化
                origin[i]=strlen(firstvt[i]);
            }
        }
        if(sign==true)
        {
            break;
        }
        for(int i=0;i<n;i++)
        {
            for(int j=3;;)
            {
                for(int k=0;k<n;k++)
                {
                    if(k==i)
                    {
                        continue;
                    }
                    else if(V[i][j]==V[k][0])
                    {
                        for(int l=0;l<strlen(firstvt[k]);l++)
                        {
                            int flag=true;//用于标识一个终结符是否已经在firstvt集中，true为不在其中的意思。
                            for(int m=0;m<strlen(firstvt[i]);m++)
                            {
                                if(firstvt[k][l]==firstvt[i][m])
                                {
                                    flag=false;
                                    break;
                                }
                            }
                            if(flag==false)
                            {
                                continue;
                            }
                            else
                            {
                                int length=strlen(firstvt[i]);
                                firstvt[i][length]=firstvt[k][l];
                            }
                        }
                    }
                }
                int flag=true;
                while(V[i][j]!='|')
                {
                    if(V[i][j]=='\0')
                    {
                        flag=false;
                        break;
                    }
                    j++;
                }
                if(flag==false)
                {
                    break;
                }
                j++;
            }
        }
    }
}

// 求LASTVT
void getLastVT(int n)
{
    for(int a=0;a<n;a++)
    {
        for(int b=0;;)
        {
            int sign=true;  // 标识是否到末尾
            while(V[a][b]!='|')
            {
                if(V[a][b]=='\0')
                {
                    sign=false;
                    break;
                }
                b++;
            }
            for(int c=0;c<strlen(VT);c++)
            {
                if(V[a][b-1]==VT[c])
                {
                    int flag=true;
                    for(int e=0;e<strlen(lastvt[a]);e++)
                    {
                        if(V[a][b-1]==lastvt[a][e])
                        {
                            flag=false;
                            break;
                        }
                    }
                    if(flag==true)
                    {
                        int length=strlen(lastvt[a]);
                        lastvt[a][length]=VT[c];
                    }
                }
                if(V[a][b-2]==VT[c])
                {
                    int flag=true;
                    for(int e=0;e<strlen(lastvt[a]);e++)
                    {
                        if(V[a][b-2]==lastvt[a][e])
                        {
                            flag=false;
                            break;
                        }
                    }
                    if(flag==true)
                    {
                        int length=strlen(lastvt[a]);
                        lastvt[a][length]=VT[c];
                    }
                }
            }
            if(sign==false)
            {
                break;
            }
            b++;
            // 跳过 “｜” 符号进行扫描
        }
    }

    //  回溯，同FIRSTVT
    int *origin=new int[n];//用于记录遍历之前的数组长度，看是否有变化。
    while(1)
    {
        int sign=true;  //  标识遍历前后LastVT是否有变化，
        for(int i=0;i<n;i++)
        {
            if(origin[i]!=strlen(lastvt[i]))
            {
                sign=false; //  长度有变化
                origin[i]=strlen(lastvt[i]);
            }
        }
        if(sign==true)
        {
            break;
        }
        for(int i=0;i<n;i++)
        {
            for(int j=3;;)
            {
                for(int k=0;k<n;k++)
                {
                    if(k==i)
                    {
                        continue;
                    }
                    else if(V[i][j]==V[k][0])
                    {
                        for(int l=0;l<strlen(lastvt[k]);l++)
                        {
                            int flag=true;  //  用于标识一个终结符是否已经在LastVT集中，true为不在其中的意思。
                            for(int m=0;m<strlen(lastvt[i]);m++)
                            {
                                if(lastvt[k][l]==lastvt[i][m])
                                {
                                    flag=false;
                                    break;
                                }
                            }
                            if(flag==false)
                            {
                                continue;
                            }
                            else
                            {
                                int length=strlen(lastvt[i]);
                                lastvt[i][length]=lastvt[k][l];
                            }
                        }
                    }
                }
                int flag=true;
                while(V[i][j]!='|')
                {
                    if(V[i][j]=='\0')
                    {
                        flag=false;
                        break;
                    }
                    j++;
                }
                if(flag==false)
                {
                    break;
                }
                j++;
            }
        }
    }
}

// 辅助函数：打印集合
void printSet(const char *title, const char *set)
{
    cout << title << "={";
    for (int i = 0; i < strlen(set); i++)
    {
        cout << set[i];
        if (i != strlen(set) - 1)
        {
            cout << ',';
        }
    }
    cout << '}' << endl;
}

// 辅助函数：打印 FIRSTVT 或 LASTVT
void printFirstLastVT(const char *label, const char *vnSet, const char sets[][N])
{
    for (int i = 0; i < strlen(vnSet); i++)
    {
        cout << label << "(" << vnSet[i] << ")={";
        for (int j = 0; j < strlen(sets[i]); j++)
        {
            cout << sets[i][j];
            if (j != strlen(sets[i]) - 1)
            {
                cout << ',';
            }
        }
        cout << '}' << endl;
    }
}

void processData(const char *data, char symbol)
{
    for (int k = 0; k < strlen(data); k++)
    {
        if (data[k] < 'A' || data[k] > 'Z') // 非终结符
        {
            int x, yk;
            if (symbol == '<' && data[k + 1] >= 'A' && data[k + 1] <= 'Z') // 处理 <
            {
                // 找出 data[k] 和 data[k+1] 的位置
                x = strchr(VT, data[k]) - VT;
                yk = strchr(VN, data[k + 1]) - VN;

                for (int l = 0; l < strlen(firstvt[yk]); l++)
                {
                    int m = strchr(VT, firstvt[yk][l]) - VT;
                    if (table[x][m] == '\0')
                    {
                        table[x][m] = symbol;
                    }
                }
            }
            else if (symbol == '>' && k >= 1 && data[k - 1] >= 'A' && data[k - 1] <= 'Z') // 处理 >
            {
                // 找出 data[k] 和 data[k-1] 的位置
                yk = strchr(VN, data[k - 1]) - VN;
                x = strchr(VT, data[k]) - VT;

                for (int l = 0; l < strlen(lastvt[yk]); l++)
                {
                    int m = strchr(VT, lastvt[yk][l]) - VT;
                    if (table[m][x] == '\0')
                    {
                        table[m][x] = symbol;
                    }
                }
            }
            else if (symbol == '=' && data[k + 1] < 'A' || data[k + 1] > 'Z') // 处理 =
            {
                x = strchr(VT, data[k]) - VT;
                yk = strchr(VT, data[k + 1]) - VT;
                if (table[x][yk] == '\0')
                {
                    table[x][yk] = symbol;
                }
            }
        }
    }
}

// 辅助函数：打印算符优先分析表
void printTable(const char *vtSet, const char table[][N])
{
    cout << '\t';
    for (int i = 0; i < strlen(vtSet); i++)
    {
        cout << vtSet[i] << '\t';
    }
    cout << '#' << endl;

    for (int i = 0; i < strlen(vtSet) + 1; i++)
    {
        cout << (i == strlen(vtSet) ? '#' : vtSet[i]) << '\t';
        for (int j = 0; j < strlen(vtSet) + 1; j++)
        {
            cout << table[i][j] << '\t';
        }
        cout << endl;
    }
}

void generateTable(int n)
{
    char data[N]; // 用于保存每个产生式
    for (int i = 0; i < n; i++)
    {
        for (int j = 3, dt = 0;; j++)
        {
            if (V[i][j] == '|' || V[i][j] == '\0')
            {
                data[dt] = '\0'; // 结束符
                if (strlen(data) != 1)
                {
                    processData(data, '<'); // 处理 <
                }
                if (V[i][j] == '\0')
                    break;
                dt = 0;
            }
            else
            {
                data[dt++] = V[i][j];
            }
        }
    }

    for (int i = 0; i < n; i++)
    {
        for (int j = 3, dt = 0;; j++)
        {
            if (V[i][j] == '|' || V[i][j] == '\0')
            {
                data[dt] = '\0'; // 结束符
                if (strlen(data) != 1)
                {
                    processData(data, '>'); // 处理 >
                }
                if (V[i][j] == '\0')
                    break;
                dt = 0;
            }
            else
            {
                data[dt++] = V[i][j];
            }
        }
    }

    for (int i = 0; i < n; i++)
    {
        for (int j = 3, dt = 0;; j++)
        {
            if (V[i][j] == '|' || V[i][j] == '\0')
            {
                data[dt] = '\0'; // 结束符
                if (strlen(data) != 1)
                {
                    processData(data, '='); // 处理 =
                }
                if (V[i][j] == '\0')
                    break;
                dt = 0;
            }
            else
            {
                data[dt++] = V[i][j];
            }
        }
    }

    int jinghao = strlen(VT);
    table[jinghao][jinghao] = '=';

    for (int i = 0; i < strlen(VT); i++)
    {
        for (int j = 0; j < strlen(firstvt[0]); j++)
        {
            if (VT[i] == firstvt[0][j])
            {
                table[jinghao][i] = '<';
                break;
            }
        }
    }

    for (int i = 0; i < strlen(VT); i++)
    {
        for (int j = 0; j < strlen(lastvt[0]); j++)
        {
            if (VT[i] == lastvt[0][j])
            {
                table[i][jinghao] = '>';
                break;
            }
        }
    }
}

void analyze(char *sentence)
{
    char in[N];
    in[0]='#';
    char out[N];
    for(int i=0;i<strlen(sentence);i++)
    {
        out[i]=sentence[i];
    }
    out[strlen(sentence)]='#';
    int step=0;
    int lin=1;
    int lout=strlen(sentence)+1;
    cout<<step<<'\t';
    for(int i=0;i<lin;i++)
    {
        cout<<in[i];
    }
    cout<<'\t';
    for(int i=0;i<lout;i++)
    {
        cout<<out[i];
    }
    cout<<'\t'<<"预备"<<endl;
    int flag=true;
    while(true)
    {
        int i=lin-1;
        while(in[i]=='i')
        {
            i--;
        }
        int j=0;
        int x,y;
        for(int k=0;k<strlen(VT);k++)
        {
            if(in[i]==VT[k])
            {
                x=k;
            }
            if(out[0]==VT[k])
            {
                y=k;
            }
        }
        if((in[i]>='a'&&in[i]<='z')||(in[i]>='0'&&in[i]<='9'))
        {
            for(int k=0;k<strlen(VT);k++)
            {
                if('i'==VT[k])
                {
                    x=k;
                }
            }
        }
        if((out[0]>='a'&&out[0]<='z')||(out[0]>='0'&&out[0]<='9'))
        {
            for(int k=0;k<strlen(VT);k++)
            {
                if('i'==VT[k])
                {
                    y=k;
                }
            }
        }
        if(in[i]=='#')
        {
            x=strlen(VT);
        }
        if(out[0]=='#')
        {
            y=strlen(VT);
        }
        if(x==y&&y==strlen(VT))
        {
            break;
        }
        if(table[x][y]=='<')
        {
            in[lin]=out[0];
            lin++;
            lout--;
            for(int l=0;l<lout;l++)
            {
                out[l]=out[l+1];
            }
            step++;
            cout<<step<<'\t';
            for(int l=0;l<lin;l++)
            {
                cout<<in[l];
            }
            cout<<'\t';
            for(int l=0;l<lout;l++)
            {
                cout<<out[l];
            }
            cout<<'\t'<<"移进"<<endl;
        }
        else if(table[x][y]=='=')
        {
            step++;
            cout<<step<<'\t';
            for(int l=0;l<lin;l++)
            {
                cout<<in[l];
            }
            cout<<out[0]<<'\t';
            for(int l=1;l<lout;l++)
            {
                cout<<out[l];
            }
            cout<<'\t'<<"移进"<<endl;
            in[i]='i';
            int lin1=lin;
            for(int k=i+1;k<lin1;k++)
            {
                in[k]='\0';
                lin--;
            }
            lout--;
            for(int k=0;k<lout;k++)
            {
                out[k]=out[k+1];
            }
            step++;
            cout<<step<<'\t';
            for(int l=0;l<lin;l++)
            {
                cout<<in[l];
            }
            cout<<'\t';
            if(out[0]=='#')
            {
                cout<<'#';
            }
            else
            {
                for(int l=1;l<lout;l++)
                {
                    cout<<out[l];
                }
            }
            cout<<'\t'<<"规约"<<endl;
        }
        else if(table[x][y]=='>')
        {
            while(true)
            {
                if(in[i-1]=='i')
                {
                    i--;
                }
                else
                {
                    break;
                }
            }
            in[i]='i';
            int lin1=lin;
            for(int k=i+1;k<lin1;k++)
            {
                in[k]='\0';
                lin--;
            }
            step++;
            cout<<step<<'\t';
            for(int k=0;k<lin;k++)
            {
                cout<<in[k];
            }
            cout<<'\t';
            if(out[0]=='#')
            {
                cout<<'#';
            }
            else
            {
                for(int k=0;k<lout;k++)
                {
                    cout<<out[k];
                }
            }
            cout<<'\t'<<"规约"<<endl;
        }
        else
        {
            flag=false;
            break;
        }
    }
    if (flag == true)
    {
        cout << "^_^ => 输入的句子成功解析！<= ^_^" << endl;
    }
    else if (flag == false)
    {
        cout << ">_< => 优先关系错误，解析失败！<= >_<" << endl;
    }
}