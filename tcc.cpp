#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>

using namespace std;

#define CODE "code2.txt"
#define RESULT "result.txt"

//token�����������չؼ��֣�������������ͽ��
//�������Ʊ������ĳ������Ϊ14
//prog����洢����Դ�����ַ�������
char *prog, token[15];
char ch;
//syn�Ǹ������ʷ��Ŷ�Ӧ������
int syn, p, m = 0, n, line, sum = 0;
//keyword����洢���ǹؼ���
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
void scaner()//״̬����ģ��
{
	//��ȫ���ÿ�
	for (n = 0; n<10; n++)
		//tokenΪ�Ѳ�����ַ���
		token[n] = NULL;
	ch = prog[p++];

	//�����������������ո�
	while (ch == ' ')
	{
		ch = prog[p];
		p++;
	}

	//�����if�ж��У���Χ��a-z����A-Z����Ϊ�涨����ֻ������ĸ��ͷ
	if ((ch >= 'a'&&ch <= 'z') || (ch >= 'A'&&ch <= 'Z'))  //�����Ǳ�ʾ�����߱�����
	{
		m = 0;
		//����Ǳ����ĵ�һ���ַ��Ժ󣬿�������ĸ������
		while ((ch >= '0'&&ch <= '9') || (ch >= 'a'&&ch <= 'z') || (ch >= 'A'&&ch <= 'Z'))
		{
            if(ch>='A'&& ch<='Z') ch+=32;
			token[m++] = ch;
			ch = prog[p++];
		}

		//�������ַ���������־
		token[m] = '\0';
		syn = 20;

		//��ʶ��������ַ����Ѷ���ı�ʾ�����Ƚϣ� �ж��Ƿ��ǹؼ��֣����йؼ��ֶ���Сд
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
        //ʶ������
        if(ch=='(')
        {
            syn = 39;
            arry();
            //p--;
            //token[m]='\0';
        }
        p--;
	}
	else if ((ch >= '0'&&ch <= '9'))  //���� ����������֣�����sum�������������
	{
		sum = 0;
		while ((ch >= '0'&&ch <= '9'))
		{
			//����*10��ֻ����ʮ������
			sum = sum * 10 + ch - '0';
			ch = prog[p++];
		}
		p--;
		syn = 21;
		//�ɽ��յ����ֵ����ֵΪ32767��������󣬻ᱨ��
		if (sum>32767)
			syn = -1;
	}
	else switch (ch)   //������������ַ�
	{
	case '<':
		m = 0;
		token[m++] = ch;
		ch = prog[p++];
		if (ch == '>')
		{
			//˵���ǲ��Ⱥ�
			syn = 29;
			token[m++] = ch;
		}
		else if (ch == '=')
		{
			//˵����<=
			syn = 30;
			token[m++] = ch;
		}
		else
		{
			//���򣬾�ֻ��һ��<����
			syn = 31;
			//��ʱp����һ��
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
			//˵���Ǹ�ֵ�����
			syn = 27;
			token[m++] = ch;
		}
		else
		{
			//�����ֻ�Ǹ�:
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
		//������յ�����.��˵������Դ����Ľ�β����syn=0����������
		syn = 0;
		token[0] = ch;
		break;
	case '\n':
		//������յ����ǻ��з�����syn=-2����+1
		syn = -2;
		break;
	default:
		//������յ���������δ������ַ�����syn=-1���ᱨ��
		syn = -1;
		break;
	}
}

void read()//��ȡԴ�����ļ�(.txt)
{
	FILE *fp;
	fp = fopen(CODE, "r");
	fseek(fp, 0, SEEK_END);//���ļ�ָ��Ų���ļ���β�������ļ���С
	int file_size;
	file_size = ftell(fp);
	fseek(fp, 0, SEEK_SET);//���ļ�ָ��Ų���ļ���ͷ����ȡ�ļ�
	prog = (char *)malloc(file_size * sizeof(char));
	fread(prog, file_size, sizeof(char), fp);

	//�ر��ļ���
	fclose(fp);
	cout<<"close success";
}

int main()
{
	line = 1;
	ofstream outfile(RESULT);

	cout<< "���ش����ļ���......" << endl;
	read();

	p = 0;
	outfile << "�ʷ������Ľ��Ϊ��" << endl;
	do//���ʶ��ģ��
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
	cout << "�ʷ�������ϣ�����result.txt�в鿴" << endl;
	return 0;
}