#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>

using namespace std;

#define CODE "code2.txt"
#define RESULT "result.txt"

//token数组用来接收关键字，变量，运算符和界符
//这里限制变量名的长度最多为14
//prog数组存储的是源代码字符串长度
char *prog, token[15];
char ch;
//syn是各个单词符号对应的数字
int syn, p, m = 0, n, line, sum = 0;
//keyword数组存储的是关键字
char *keyword1[10] = { "begin","if","then","else","while","do","end","repeat","until" };
char *keyword2[4] = { "const","var","procedure","call" };
char *keyword3[2] = { "write","read" };
void arry()
{
    if(ch=='(')
    {
        //p++;
        token[m++] = ch;
        ch=prog[p++];
        while((ch>='0'&&ch<='9')||(ch>='a'&&ch<='z')||(ch>='A'&&ch<='Z')||(ch=='+'||ch=='-'||ch=='*'||ch=='/'))
        {
            if(ch>='A'&& ch<='Z') ch+=32;
            token[m++] = ch;
            ch = prog[p++];
        }
        arry();
        if(ch==':') {
            token[m++] = ch;
            ch = prog[p++];
            while (ch >= '0' && ch <= '9' || ch >= 'a' && ch <= 'z' || ch >= 'A' && ch <= 'Z'||ch=='+'||ch=='-'||ch=='*'||ch=='/') {
                if (ch >= 'A' && ch <= 'Z') ch += 32;
                token[m++] = ch;
                ch = prog[p++];
            }
            arry();
            if (ch == ')') {
                token[m++] = ch;
                ch = prog[p++];
            }
        }
        if (ch == ')') {
            token[m++] = ch;
            ch = prog[p++];
        }
    }
}
void scaner()//状态分析模块
{
	//将全部置空
	for (n = 0; n<10; n++)
		//token为已捕获的字符数
		token[n] = NULL;
	ch = prog[p++];

	//这样处理，可以跳过空格
	while (ch == ' ')
	{
		ch = prog[p];
		p++;
	}

	//在这个if判断中，范围是a-z或者A-Z，因为规定变量只能以字母开头
	if ((ch >= 'a'&&ch <= 'z') || (ch >= 'A'&&ch <= 'Z'))  //可能是标示符或者变量名
	{
		m = 0;
		//这里，是变量的第一个字符以后，可以是字母，数字
		while ((ch >= '0'&&ch <= '9') || (ch >= 'a'&&ch <= 'z') || (ch >= 'A'&&ch <= 'Z'))
		{
            if(ch>='A'&& ch<='Z') ch+=32;
			token[m++] = ch;
			ch = prog[p++];
		}

		//变量的字符串结束标志
		token[m] = '\0';
		syn = 20;

		//将识别出来的字符和已定义的标示符作比较， 判断是否是关键字，所有关键字都是小写
		for (n = 0; n<9; n++)
			if (strcmp(token, keyword1[n]) == 0)
			{
				syn = n + 1;
				break;
			}
		for (n = 0; n < 4;n++)
		{
			if (strcmp(token, keyword2[n]) == 0)
			{
				syn = n + 11;
				break;
			}
		}
        for (n = 0; n < 2;n++)
        {
            if (strcmp(token, keyword3[n]) == 0)
            {
                syn = n + 16;
                break;
            }
        }
        //识别数组
        if(ch=='(')
        {
            syn = 39;
            arry();
            //p--;
            //token[m]='\0';
        }
        p--;
	}
	else if ((ch >= '0'&&ch <= '9'))  //数字 ，如果是数字，就用sum来保存这个数字
	{
		sum = 0;
		while ((ch >= '0'&&ch <= '9'))
		{
			//这里*10是只考虑十进制数
			sum = sum * 10 + ch - '0';
			ch = prog[p++];
		}
		p--;
		syn = 21;
		//可接收的数字的最大值为32767，如果更大，会报错
		if (sum>32767)
			syn = -1;
	}
	else switch (ch)   //如果是其他的字符
	{
	case '<':
		m = 0;
		token[m++] = ch;
		ch = prog[p++];
		if (ch == '>')
		{
			//说明是不等号
			syn = 29;
			token[m++] = ch;
		}
		else if (ch == '=')
		{
			//说明是<=
			syn = 30;
			token[m++] = ch;
		}
		else
		{
			//否则，就只是一个<符号
			syn = 31;
			//此时p回退一个
			p--;
		}
		break;
	case '>':
		m = 0;
		token[m++] = ch;
		ch = prog[p++];
		if (ch == '=')
		{
			syn = 32;
			token[m++] = ch;
		}
		else
		{
			syn = 33;
			p--;
		}
		break;
	case ':':
		m = 0;
		token[m++] = ch;
		ch = prog[p++];
		if (ch == '=')
		{
			//说明是赋值运算符
			syn = 27;
			token[m++] = ch;
		}
		else
		{
			//否则就只是个:
			syn = 28;
			p--;
		}
		break;
	case '*':
		syn = 23;
		token[0] = ch;
		break;
	case '/':
		syn = 24;
		token[0] = ch;
		break;
	case '+':
		syn = 25;
		token[0] = ch;
		break;
	case '-':
		syn = 26;
		token[0] = ch;
		break;
	case '=':
		syn = 34;
		token[0] = ch;
		break;
	case ';':
		syn = 35;
		token[0] = ch;
		break;
	case '(':
		syn = 36;
		token[0] = ch;
		break;
	case ')':
		syn = 37;
		token[0] = ch;
		break;
	case ',':
		syn = 38;
		token[0] = ch;
		break;
    case '{':
        while(1)
        {
            ch = prog[p++];
            if(ch=='\n')
            {
                syn = -1;
                break;
            }
            if(ch=='}')
            {
                syn = 100;
                break;
            }

        }
        break;
	case '.':
		//如果接收到的是.，说明到了源代码的结尾，置syn=0，函数结束
		syn = 0;
		token[0] = ch;
		break;
	case '\n':
		//如果接收到的是换行符，则syn=-2，行+1
		syn = -2;
		break;
	default:
		//如果接收到的是其他未定义的字符，置syn=-1，会报错。
		syn = -1;
		break;
	}
}

void read()//读取源代码文件(.txt)
{
	FILE *fp;
	fp = fopen(CODE, "r");
	fseek(fp, 0, SEEK_END);//将文件指针挪到文件结尾来计算文件大小
	int file_size;
	file_size = ftell(fp);
	fseek(fp, 0, SEEK_SET);//将文件指针挪到文件开头来读取文件
	prog = (char *)malloc(file_size * sizeof(char));
	fread(prog, file_size, sizeof(char), fp);

	//关闭文件流
	fclose(fp);
	cout<<"close success";
}

int main()
{
	line = 1;
	ofstream outfile(RESULT);

	cout<< "加载代码文件中......" << endl;
	read();

	p = 0;
	outfile << "词法分析的结果为：" << endl;
	do//标记识别模块
	{
		scaner();
		switch (syn)
		{
		case 21:
			outfile << "(" << syn << "," << sum << ")" << endl;
			break;
		case -1:
			outfile << "Error in line" << line << "!" << endl;
			break;
		case -2:
			line++;
			break;
        case 100:
            break;
		default:
			outfile << "(" << syn << "," << token << ")" << endl;
			break;
		}
	} while (syn != 0);
	outfile.close();
	cout << "词法分析完毕，请在result.txt中查看" << endl;
	return 0;
}