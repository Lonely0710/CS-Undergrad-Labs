#include "SLR1.h"

// 初始化全局变量
char terminals[100] = {};       // 终结符数组
int no_t = 0;                   // 终结符数量
char non_terminals[100] = {};   // 非终结符数组
int no_nt = 0;                  // 非终结符数量
char goto_table[100][100] = {}; // GOTO表
char reduce[20][20] = {};       // 规约表
char follow[20][20] = {};       // FOLLOW集合
char fo_co[20][20] = {};        // FOLLOW和FIRST集合的交集
char first[20][20] = {};        // FIRST集合
int temp_var_count = 0;
vector<string> all_productions = {};
vector<Quadruple> quads = {};

int main()
{
    cout << "---------------------------------------------------" << endl;
    cout << "                 2228XXXX XXX" << endl;
    cout << "              SLR(1)语法分析器程序" << endl;
    cout << "---------------------------------------------------" << endl;
    struct state init;
    struct state temp;
    struct state temp1;

    int state_count = 1;
    get_prods(&init);
    init_productions(&init); // 保存所有产生式
    temp = init;
    temp1 = temp;
    add_dots(&init);

    for (int i = 0; i < 100; i++)
        for (int j = 0; j < 100; j++)
            goto_table[i][j] = '~';

    struct state I[50];
    augument(&I[0], &init);
    closure(&init, &I[0]);
    cout << "\nI0:\n";
    print_prods(&I[0]);

    for (int i = 0; i < state_count; i++)
    {
        char characters[20] = {};
        for (int z = 0; z < I[i].prod_count; z++)
            if (!in_array(characters, char_after_dot(I[i].prod[z])))
                characters[strlen(characters)] = char_after_dot(I[i].prod[z]);

        for (int j = 0; j < strlen(characters); j++)
        {
            goto_state(&I[i], &I[state_count], characters[j]);
            closure(&init, &I[state_count]);
            int flag = 0;
            for (int k = 0; k < state_count - 1; k++)
            {
                if (same_state(&I[k], &I[state_count]))
                {
                    cleanup_prods(&I[state_count]);
                    flag = 1;
                    cout << "I" << i << " 读取符号 " << characters[j] << " 转移到 I" << k << "。\n";
                    goto_table[i][k] = characters[j];
                    ;
                    break;
                }
            }
            if (flag == 0)
            {
                state_count++;
                cout << "I" << i << " 读取符号 " << characters[j] << " 转移到 I" << state_count - 1 << ":\n";
                goto_table[i][state_count - 1] = characters[j];
                print_prods(&I[state_count - 1]);
            }
        }
    }

    std::vector<int> no_re(temp.prod_count, -1);
    terminals[no_t] = '#';
    no_t++;

    add_dot_at_end(&temp1);
    for (int i = 0; i < state_count; i++)
    {
        for (int j = 0; j < I[i].prod_count; j++)
            for (int k = 0; k < temp1.prod_count; k++)
                if (in_state(&I[i], temp1.prod[k]))
                    no_re[k] = i;
    }

    find_first(&temp);
    for (int l = 0; l < no_nt; l++)
    {
        for (int i = 0; i < temp.prod_count; i++)
        {
            if (is_non_terminal(temp.prod[i][3]))
            {
                add_to_first(return_index(temp.prod[i][0]) - no_t, return_index(temp.prod[i][3]) - no_t);
            }
        }
    }

    find_follow(&temp);
    add_to_follow(0, '#');
    for (int l = 0; l < no_nt; l++)
    {
        for (int i = 0; i < temp.prod_count; i++)
        {
            for (int k = 3; k < strlen(temp.prod[i]); k++)
            {
                if (temp.prod[i][k] == non_terminals[l])
                {
                    if (is_non_terminal(temp.prod[i][k + 1]))
                    {
                        add_to_follow_first(l, return_index(temp.prod[i][k + 1]) - no_t);
                    }
                    if (temp.prod[i][k + 1] == '\0')
                        add_to_follow(l, return_index(temp.prod[i][0]) - no_t);
                }
            }
        }
    }
    cout << endl;
    cout << "---------------------------------------------------" << endl;
    cout << "********** 移动表 **********" << endl;
    print_shift_table(state_count);
    cout << endl
         << endl;
    cout << "---------------------------------------------------" << endl;
    cout << "********** 规约表 **********" << endl;
    print_reduce_table(state_count, &no_re[0], &temp1);
    cout << "---------------------------------------------------" << endl;
    cout << "                 SLR(1)语法分析过程" << endl;

    string input = "a=(b+c)*(d/e)";

    // 分析输入串
    SLR_Analysis(input, state_count);
    print_quadruples(); // 打印四元式
}