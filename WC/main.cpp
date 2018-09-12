#include <bits/stdc++.h>
#include <unistd.h>
#include <dirent.h>
#include <stdlib.h>
#include <sys/stat.h>
using namespace std;

struct data
{
    int Empty = 0;  //����
    int Code = 0;   //������
    int Comment = 0;//ע����

    string str = "";//�ļ����ݱ�����ַ���

    bool Null = false;//��ָ����

    void Calc()
    {
        bool line_comment = false;  //��ע��
        bool block_comment = false; //��ע�ͱ��
        bool comment_exist = false; //ע�ͳ��ֹ����

        string s_comment = "";
        string s_code = "";
        //ע��������ַ����ʹ���������ַ���

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
                //ע���ж�

                if(s_comment.length() == 0)
                {
                    if(s_code.length() == 0)
                        Empty ++;

                    if(s_code.length() == 1 && (s_code[0] == '{' || s_code[0] == '}' || s_code[0] == ';'))
                        Empty ++;
                }
                //�����ж�

                if(s_code.length() > 1 || (s_code[0] != '{' && s_code[0] != '}' && s_code[0] != ';'))
                    Code ++;
                //�������ж�

                s_code = "";
                s_comment = "";
                line_comment = false;
                comment_exist = false;
                //����֮���ʼ��
            }

            else if(line_comment)
            //��ע����
            {
                s_comment = s_comment + str[i];
                comment_exist = true;
            }

            else if(block_comment)
            //��ע����
            {
                s_comment = s_comment + str[i];
                comment_exist = true;

                if(str[i - 1] == '*' && str[i] == '/')
                    block_comment = false;
                //��ע����ֹ
            }

            else if(str[i - 1] == '/' && str[i] == '/')
            //�������ַ������һ�� / ���浽ע���ַ���֮�У�����ע�ͱ��Ϊ��
            {
                s_code[s_code.length() - 1] = '\0';
                s_comment = s_comment + str[i - 1] + str[i];

                line_comment = true;
                comment_exist = true;
            }

            else if(str[i - 1] == '/' && str[i] == '*')
            //�������ַ������һ�� / ���浽ע���ַ���֮�У�����ע�ͱ��Ϊ��
            {
                s_code[s_code.length() - 1] = '\0';
                s_comment = s_comment + str[i - 1] + str[i];
                block_comment = true;
                comment_exist = true;
            }

            else if(str[i] == '\"')
            //����˫����ʱ��ֱ��ѭ������һ��˫����, �������������ַ���֮��
            {
                s_code = s_code + str[i];
                while(str[++ i] != '\"')
                {
                    s_code = s_code + str[i];
                }
                s_code = s_code + str[i];
            }

            else if(str[i] == '\'')
            //����������ʱ��ֱ��ѭ������һ��������, �������������ַ���֮��
            {
                s_code = s_code + str[i];
                while(str[++ i] != '\'')
                {
                    s_code = s_code + str[i];
                }
                s_code = s_code + str[i];
            }

            else
            //��ͨ�ַ�
                s_code = s_code + str[i];
        }

        if(!Null)
        {
            printf("ע������ : %d\n", Comment);
            printf("�������� : %d\n", Code);
            printf("������   : %d\n", Empty);
        }
    }

    bool input(FILE * fp)
    //�����ı�
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
    //ͳ������
    {
        int num = 0;
        for(int i = 0; i < str.length(); i ++)
        {
            if(str[i] == '\n')   num ++;
        }
        if(!Null)
            printf("����    ��%d\n", num);
    }

    void Char()
    //ͳ���ַ�
    {
        int num = 0;
        for(int i = 0; i < str.length(); i ++)
        {
            if(str[i] != ' ' && str[i] != '\n' && str[i] != '\t')
                num ++;
        }
        if(!Null)
            printf("�ַ����� ��%d\n", num);
    }

    void Words()
    //ͳ�Ƶ���
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
            printf("���ʸ��� ��%d\n", num);
    }

};

int main(int argc, char *argv[])
{

    bool operation[5] = {false, false, false, false, false};
    //�±��0��ʼ��Ӧ�Ĳ���Ϊ -s, -a, -c, -w, -l

    int file_start = 0;
    //�ڼ���������ʼ���ļ���

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
    //����main�����������жϳ������������ͣ��������ļ���


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
                    printf("%s ��һ�����ļ���\n", filename->d_name);
                else
                {
                    printf("%s ��ȡ�ɹ����ļ���Ϣ���£�\n", filename->d_name);

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
                printf("%s ��һ�����ļ���\n", argv[i]);
            else
            {
                printf("%s ��ȡ�ɹ����ļ���Ϣ���£�\n", argv[i]);

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
        puts("��û���ҵ�ƥ����ļ���");
    }

    return 0;
}


//
