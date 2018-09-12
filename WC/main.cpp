#include <bits/stdc++.h>
#include <unistd.h>
#include <dirent.h>
#include <stdlib.h>
#include <sys/stat.h>
using namespace std;

struct data
{
    int Empty = 0;  //空行
    int Code = 0;   //代码行
    int Comment = 0;//注释行

    string str = "";//文件内容保存进字符串

    bool Null = false;//空指针标记

    void Calc()
    {
        bool line_comment = false;  //行注释
        bool block_comment = false; //块注释标记
        bool comment_exist = false; //注释出现过标记

        string s_comment = "";
        string s_code = "";
        //注释里面的字符串和代码里面的字符串

        if(str[0] == '\n')
            Empty ++;
        else  if(str[0] != ' ' && str[0] != '\t')
            s_code = s_code + str[0];

        for(int i = 1; i < str.length(); i ++)
        {
            if(str[i] == ' ' || str[i] == '\t')
                continue ;

            if(str[i] == '\n')
            {
                if(comment_exist || line_comment || block_comment)
                    Comment ++;
                //注释判断

                if(s_comment.length() == 0)
                {
                    if(s_code.length() == 0)
                        Empty ++;

                    if(s_code.length() == 1 && (s_code[0] == '{' || s_code[0] == '}' || s_code[0] == ';'))
                        Empty ++;
                }
                //空行判断

                if(s_code.length() > 1 || (s_code[0] != '{' && s_code[0] != '}' && s_code[0] != ';'))
                    Code ++;
                //代码行判断

                s_code = "";
                s_comment = "";
                line_comment = false;
                comment_exist = false;
                //换行之后初始化
            }

            else if(line_comment)
            //行注释中
            {
                s_comment = s_comment + str[i];
                comment_exist = true;
            }

            else if(block_comment)
            //块注释中
            {
                s_comment = s_comment + str[i];
                comment_exist = true;

                if(str[i - 1] == '*' && str[i] == '/')
                    block_comment = false;
                //块注释终止
            }

            else if(str[i - 1] == '/' && str[i] == '/')
            //将代码字符串最后一个 / 保存到注释字符串之中，并行注释标记为真
            {
                s_code[s_code.length() - 1] = '\0';
                s_comment = s_comment + str[i - 1] + str[i];

                line_comment = true;
                comment_exist = true;
            }

            else if(str[i - 1] == '/' && str[i] == '*')
            //将代码字符串最后一个 / 保存到注释字符串之中，并块注释标记为真
            {
                s_code[s_code.length() - 1] = '\0';
                s_comment = s_comment + str[i - 1] + str[i];
                block_comment = true;
                comment_exist = true;
            }

            else if(str[i] == '\"')
            //读到双引号时，直接循环到下一个双引号, 并保存至代码字符串之中
            {
                s_code = s_code + str[i];
                while(str[++ i] != '\"')
                {
                    s_code = s_code + str[i];
                }
                s_code = s_code + str[i];
            }

            else if(str[i] == '\'')
            //读到单引号时，直接循环到下一个单引号, 并保存至代码字符串之中
            {
                s_code = s_code + str[i];
                while(str[++ i] != '\'')
                {
                    s_code = s_code + str[i];
                }
                s_code = s_code + str[i];
            }

            else
            //普通字符
                s_code = s_code + str[i];
        }

        if(!Null)
        {
            printf("注释行数 : %d\n", Comment);
            printf("代码行数 : %d\n", Code);
            printf("空行数   : %d\n", Empty);
        }
    }

    bool input(FILE * fp)
    //读入文本
    {
        str = "";
        char s;
        while((s = fgetc(fp)) != EOF)
            str = str + s;
        str = str + '\n';
        if(str.length() == 1)
        {
            Null = true;
        }
        return Null;
    }

    void Line()
    //统计行数
    {
        int num = 0;
        for(int i = 0; i < str.length(); i ++)
        {
            if(str[i] == '\n')   num ++;
        }
        if(!Null)
            printf("行数    ：%d\n", num);
    }

    void Char()
    //统计字符
    {
        int num = 0;
        for(int i = 0; i < str.length(); i ++)
        {
            if(str[i] != ' ' && str[i] != '\n' && str[i] != '\t')
                num ++;
        }
        if(!Null)
            printf("字符个数 ：%d\n", num);
    }

    void Words()
    //统计单词
    {
        int num = 0;
        bool flag = 0;
        for(int i = 0; i < str.length(); i ++)
        {
            if(str[i] <= 'z' && str[i] >= 'a' || str[i] <= 'Z' && str[i] >= 'A' || str[i] == '_')
            {
                if(!flag)   num ++;
                flag = true;
            }
            else flag = false;
        }
        if(!Null)
            printf("单词个数 ：%d\n", num);
    }

};

int main(int argc, char *argv[])
{

    bool operation[5] = {false, false, false, false, false};
    //下表从0开始对应的操作为 -s, -a, -c, -w, -l

    int file_start = 0;
    //第几个参数开始是文件名

    for(int i = 1; i < argc; i ++)
    {
        if(argv[i][0] == '-')
        {
            switch(argv[i][1])
            {
                case 's' : operation[0] = true;
                    break;
                case 'a' : operation[1] = true;
                    break;
                case 'c' : operation[2] = true;
                    break;
                case 'w' : operation[3] = true;
                    break;
                case 'l' : operation[4] = true;
                    break;
                default :
                    break;
            }
        }
        else
        {
            file_start = i;
            break;
        }
    }
    //读入main函数参数，判断出操作参数类型，并保存文件名


    int num = 0;

    if(operation[0])
    {
        const char directory[] = "../Debug";
        const char type[] = ".c";

        DIR * dir = opendir(directory);

        struct dirent * filename;

        while( (filename = readdir(dir)) != NULL)
        {
            int len_filename = strlen(filename->d_name);

            if(strcmp(type, filename->d_name + len_filename - 2) == 0)
            {
                puts("");
                num ++;
                FILE * fp = fopen(filename->d_name, "r");
                data test;
                if(test.input(fp))
                    printf("%s 是一个空文件！\n", filename->d_name);
                else
                {
                    printf("%s 读取成功，文件信息如下：\n", filename->d_name);

                    if(operation[1])
                        test.Calc();

                    if(operation[2])
                        test.Char();

                    if(operation[3])
                        test.Words();

                    if(operation[4])
                        test.Line();

                }
            }
        }
    }
    else
    {
        for(int i = file_start; i < argc; i ++)
        {
            puts("");
            num ++;
            FILE * fp = fopen(argv[i], "r");
            data test;
            if(test.input(fp))
                printf("%s 是一个空文件！\n", argv[i]);
            else
            {
                printf("%s 读取成功，文件信息如下：\n", argv[i]);

                if(operation[1])
                    test.Calc();

                if(operation[2])
                    test.Char();

                if(operation[3])
                    test.Words();

                if(operation[4])
                    test.Line();

            }
        }
    }

    if(num == 0)
    {
        puts("并没有找到匹配的文件！");
    }

    return 0;
}


//
