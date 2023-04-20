/**
* postfix.c
*
* School of Computer Science,
* Chungbuk National University
*/



#include<stdio.h> 
#include<stdlib.h> 
#include<ctype.h> 
#include<string.h> 

#define _CRT_SECURE_NO_WARNINGS
#define MAX_STACK_SIZE 10 
#define MAX_EXPRESSION_SIZE 20 

/* stack ������ �켱������ ��������, lparen = 0 ���� ���� */
typedef enum {
	lparen = 0,  /* ( ���� ��ȣ */
	rparen = 9,  /* ) ������ ��ȣ*/
	times = 7,   /* * ���� */
	divide = 6,  /* / ������ */
	plus = 5,    /* + ���� */
	minus = 4,   /* - ���� */
	operand = 1  /* �ǿ����� */
} precedence;

//��������
char infixExp[MAX_EXPRESSION_SIZE];		//����ǥ���
char postfixExp[MAX_EXPRESSION_SIZE];	//����ǥ��� 
char postfixStack[MAX_STACK_SIZE];		//����ǥ��� ��ȯ�� �����ڸ� ������ �� ����
int evalStack[MAX_STACK_SIZE];			//��� ����
int postfixStackTop = -1;				//����ǥ�� ������ ������ top
int evalStackTop = -1;					//����� ��� �� ���� ������ top
int evalResult = 0;						//������ 

//�Լ�
void postfixPush(char x);           //postfixStack�� push
char postfixPop();                  //postfixStack�� pop
void evalPush(int x);               //evalStack�� push
int evalPop();                      //evalStack�� pop
void getInfix();                    //����ǥ������� ���� �Է¹޴� �Լ�

precedence getToken(char symbol);   //���ڸ� �޾Ƽ�
precedence getPriority(char x);     //����켱���� ����

void charCat(char* c);              //postfixExp�� �ٿ���
void toPostfix();                   //����ǥ������� ����
void debug();                       //�������� ��� ���
void reset();                       //��� ���� �� ���� �ʱ�ȭ
void evaluation();                  //����ǥ�� ���� �����Լ�

int main()
{
	printf("[-----[������][2022041047]-----]");
	char command;
	do {
		printf("---------------------------------------------------------------\n");
		printf("               Infix to Postfix, then Evaluation               \n");
		printf("---------------------------------------------------------------\n");
		printf(" Infix=i,   Postfix=p,  Eval=e,   Debug=d,   Reset=r,   Quit=q \n");
		printf("---------------------------------------------------------------\n");
		printf("Command = ");
		scanf(" %c", &command);
		switch (command) {
		case 'i': case 'I':	//����ǥ�� ���� �Է�
			getInfix();
			break;

		case 'p': case 'P': //����->����ǥ�� �������� ��ȯ
			toPostfix();
			break;

		case 'e': case 'E': //����ǥ�� ���� ���
			evaluation();
			break;

		case 'd': case 'D': //���� ���� ���� ����
			debug();
			break;

		case 'r': case 'R': //��� ����,���� �� ���� �ʱ�ȭ
			reset();
			break;

		case 'q':case 'Q': //������
			break;
		default:
			printf("\n       >>>>>   Concentration!!   <<<<<     \n");//
			break;
		}
	} while (command != 'q' && command != 'Q');
	return 1;
}

void postfixPush(char x)//postfixStack�� Ǫ��
{
	postfixStack[++postfixStackTop] = x;//postfixStack�� x�� Ǫ�� �� top 1 ����
}

char postfixPop()//postfixStack�� ���� ���� ���� ��ȯ
{
	char x;

	//postfixStack��
	if (postfixStackTop == -1)
		return '\0';
	else 
	//�����ڰ� ����������
	{
		x = postfixStack[postfixStackTop--];	//���� �����ִ� ������ ��ȯ
	}
	return x;
}

void evalPush(int x)//����ǥ�� ���� ��� ����� �����ϴ� ����
{
	evalStack[++evalStackTop] = x;			//���İ�� ��� ����
}

int evalPop() //evalStack���� pop
{
	if (evalStackTop == -1)					//stack�� ��������� -1����
		return -1;
	else
		return evalStack[evalStackTop--];
}

/** * infix expression�� �Է¹޴´�.
* infixExp���� �Էµ� ���� �����Ѵ�.
*/

void getInfix()//����ǥ������� ���� �Է¹޴� �Լ�
{
	printf("Type the expression >>> ");
	scanf("%s", infixExp);//���� �Է�
}

precedence getToken(char symbol) 			//�Ű������� �������� ���� ���ڷ� ��ȯ
{
	switch (symbol) {
	case '(':
		return lparen;
	case ')':
		return rparen;
	case '+':
		return plus;
	case '-':
		return minus;
	case '/':
		return divide;
	case '*':
		return times;
	default:
		return operand;
	}
} 
precedence getPriority(char x)//�������� ������� å��(����->)
{
	return getToken(x);			//������� ��ȯ
}

/**
* �����ϳ��� ���޹޾�, postfixExp�� �߰�
*/
void charCat(char* c)//���ڸ� �޾� ����ǥ��� �ۼ�
{
	if (postfixExp == '\0')			//ù ���ڶ��
		strncpy(postfixExp, c, 1);	//���ڿ�ī���Լ�
	else
		strncat(postfixExp, c, 1);	//�ƴ϶�� ���ڿ������Լ��� postFixExp�� ����ǥ����� �ۼ�
}
/**
* infixExp�� ���ڸ� �ϳ��� �о�鼭 stack�� �̿��Ͽ� postfix�� �����Ѵ�.
* ����� postfix�� postFixExp�� ����ȴ�.
*/
void toPostfix()//����ǥ������� ���� �Լ�
{
	/* infixExp�� ���� �ϳ����� �б����� ������ */
	char* exp = infixExp;
	
	/* �����ϳ��� �ӽ÷� �����ϱ� ���� ���� */
    char x;

	/* exp�� �������Ѱ��鼭, ���ڸ� �а� postfix�� ���� */
	while (*exp != '\0')
	{
		if (getPriority(*exp) == operand)	//�ǿ������϶�
		{
			x = *exp;
			charCat(&x);						// postFixExp�� �ٷ� �Է�
		}

		else if (getPriority(*exp) == lparen)//���� ��ȣ�ϋ�
		{
			postfixPush(*exp);					//����ǥ�� ���ÿ� push
		}
		else if (getPriority(*exp) == rparen)//���� ��ȣ�ϋ�
		{
			while ((x = postfixPop()) != '(')	//����ǥ�� ���ÿ� '('���� ���� ������ ���
			{
				charCat(&x);//postFixExp�� �Է�
			}
		}
		else//��ȣ �̿��� ������ �� ��
		{
			while (getPriority(postfixStack[postfixStackTop]) >= getPriority(*exp))//���� �����ڰ� postfixStack�� top �����ں��� �켱������ ������������,
			{
				x = postfixPop();		//postfixStack�� top�����ڸ� �����ؿͼ�
				charCat(&x);			//���ڸ� �޾� ����ǥ�� ���ÿ� ����
			}
			postfixPush(*exp);//���� ���� �����ڸ� ����ǥ�� ���ÿ� ����
		}
		exp++;
	}
	while (postfixStackTop != -1)//postfixStack�� ��������,
	{
		x = postfixPop();		//postfixStack�� top�� 
		charCat(&x);			//postfixExp�� �Է�
	}
}
void debug()//�� ��� ���� �� ��� ���
{
	printf("\n---DEBUG\n");
	printf("infixExp =  %s\n", infixExp);		//����ǥ������� ǥ��� ����
	printf("postExp =  %s\n", postfixExp);		//����ǥ������� ǥ��� ����
	printf("eval result = %d\n", evalResult);	//���� ��� ��
	printf("postfixStack : ");					//����ǥ��� �����ڵ��� �����ϴ� ����
	for (int i = 0; i < MAX_STACK_SIZE; i++)
		printf("%c  ", postfixStack[i]);
	printf("\n");
}
void reset()//��� ����, ���� top, ����, �̿��� ���� �ʱ�ȭ
{
	infixExp[0] = '\0';			//����ǥ�� ����
	postfixExp[0] = '\0';		//����ǥ�� ����

	for (int i = 0; i < MAX_STACK_SIZE; i++)	//postfixStack ����
		postfixStack[i] = '\0';
	postfixStackTop = -1;		//�̿��� ���� ����.
	evalStackTop = -1;
	evalResult = 0;
}
void evaluation()//����ǥ������� �ٲ� ���� ���
{
	int opr1, opr2, i; 					//�ǿ�����1, �ǿ�����2, �ε���
	int length = strlen(postfixExp);	//����ǥ�� ������ ����
	char symbol; evalStackTop = -1;		//������ ��ū�� ���� ����, ��꽺��top����
	for (i = 0; i < length; i++)
	{
		symbol = postfixExp[i];				//postfixExp���κ��� �������� ��ū �޾ƿ���

		if (getToken(symbol) == operand) 	//symbol�� �ǿ������϶�,
		{
			evalPush(symbol - '0');			//���ڸ� ������ �ٲپ� evalStack�� push
		}
		else								//symbol�� �������϶�, 
		{
			opr2 = evalPop();				//evalstack���� pop�Ͽ� �ǿ����� ����
			opr1 = evalPop();
			switch (getToken(symbol))		//symbol�� ���� ��Ģ���� ���� �� �� �ٽ� evalstack�� push
			{
			case plus:
				evalPush(opr1 + opr2);
				break;
			case minus: evalPush(opr1 - opr2);
				break; 
			case times:
				evalPush(opr1 * opr2);
				break; 
			case divide:
				evalPush(opr1 / opr2);
				break;
			default:
				break;
			}
		}
	}
	evalResult = evalPop();					//��� ���� ���� �� ���� �� evalResult�� ����
}