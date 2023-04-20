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

/* stack 내에서 우선순위는 내림차순, lparen = 0 가장 낮음 */
typedef enum {
	lparen = 0,  /* ( 왼쪽 괄호 */
	rparen = 9,  /* ) 오른쪽 괄호*/
	times = 7,   /* * 곱셈 */
	divide = 6,  /* / 나눗셈 */
	plus = 5,    /* + 덧셈 */
	minus = 4,   /* - 뺄셈 */
	operand = 1  /* 피연산자 */
} precedence;

//전역변수
char infixExp[MAX_EXPRESSION_SIZE];		//중위표기법
char postfixExp[MAX_EXPRESSION_SIZE];	//후위표기법 
char postfixStack[MAX_STACK_SIZE];		//후위표기로 전환시 연산자를 보관해 둘 스택
int evalStack[MAX_STACK_SIZE];			//계산 스택
int postfixStackTop = -1;				//후위표기 연산자 스택의 top
int evalStackTop = -1;					//연산시 결과 값 저장 스택의 top
int evalResult = 0;						//연산결과 

//함수
void postfixPush(char x);           //postfixStack에 push
char postfixPop();                  //postfixStack에 pop
void evalPush(int x);               //evalStack에 push
int evalPop();                      //evalStack에 pop
void getInfix();                    //중위표기법으로 수식 입력받는 함수

precedence getToken(char symbol);   //문자를 받아서
precedence getPriority(char x);     //연산우선순위 결정

void charCat(char* c);              //postfixExp에 붙여줌
void toPostfix();                   //후위표기법으로 변경
void debug();                       //실행후의 결과 출력
void reset();                       //모든 변수 및 스택 초기화
void evaluation();                  //후위표기 수식 연산함수

int main()
{
	printf("[-----[강은혜][2022041047]-----]");
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
		case 'i': case 'I':	//중위표기 수식 입력
			getInfix();
			break;

		case 'p': case 'P': //중위->후위표기 수식으로 변환
			toPostfix();
			break;

		case 'e': case 'E': //후위표기 수식 계산
			evaluation();
			break;

		case 'd': case 'D': //현재 진행 상태 보기
			debug();
			break;

		case 'r': case 'R': //모든 수식,스택 및 변수 초기화
			reset();
			break;

		case 'q':case 'Q': //나가기
			break;
		default:
			printf("\n       >>>>>   Concentration!!   <<<<<     \n");//
			break;
		}
	} while (command != 'q' && command != 'Q');
	return 1;
}

void postfixPush(char x)//postfixStack에 푸쉬
{
	postfixStack[++postfixStackTop] = x;//postfixStack에 x를 푸쉬 후 top 1 증가
}

char postfixPop()//postfixStack의 가장 위의 원소 반환
{
	char x;

	//postfixStack에
	if (postfixStackTop == -1)
		return '\0';
	else 
	//연산자가 남아있으면
	{
		x = postfixStack[postfixStackTop--];	//가장 위에있는 연산자 반환
	}
	return x;
}

void evalPush(int x)//후위표기 수식 계산 결과를 보관하는 스택
{
	evalStack[++evalStackTop] = x;			//수식계산 결과 보관
}

int evalPop() //evalStack에서 pop
{
	if (evalStackTop == -1)					//stack이 비어있으면 -1리턴
		return -1;
	else
		return evalStack[evalStackTop--];
}

/** * infix expression을 입력받는다.
* infixExp에는 입력된 값을 저장한다.
*/

void getInfix()//중위표기법으로 수식 입력받는 함수
{
	printf("Type the expression >>> ");
	scanf("%s", infixExp);//수식 입력
}

precedence getToken(char symbol) 			//매개변수를 열거형을 통해 숫자로 반환
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
precedence getPriority(char x)//연산자의 연산순위 책정(문자->)
{
	return getToken(x);			//연산순위 반환
}

/**
* 문자하나를 전달받아, postfixExp에 추가
*/
void charCat(char* c)//문자를 받아 후위표기로 작성
{
	if (postfixExp == '\0')			//첫 문자라면
		strncpy(postfixExp, c, 1);	//문자열카피함수
	else
		strncat(postfixExp, c, 1);	//아니라면 문자열연결함수로 postFixExp에 후위표기수식 작성
}
/**
* infixExp의 문자를 하나씩 읽어가면서 stack을 이용하여 postfix로 변경한다.
* 변경된 postfix는 postFixExp에 저장된다.
*/
void toPostfix()//후위표기법으로 변경 함수
{
	/* infixExp의 문자 하나씩을 읽기위한 포인터 */
	char* exp = infixExp;
	
	/* 문자하나를 임시로 저장하기 위한 변수 */
    char x;

	/* exp를 증가시켜가면서, 문자를 읽고 postfix로 변경 */
	while (*exp != '\0')
	{
		if (getPriority(*exp) == operand)	//피연산자일때
		{
			x = *exp;
			charCat(&x);						// postFixExp에 바로 입력
		}

		else if (getPriority(*exp) == lparen)//열린 괄호일떄
		{
			postfixPush(*exp);					//후위표기 스택에 push
		}
		else if (getPriority(*exp) == rparen)//닫힌 괄호일떄
		{
			while ((x = postfixPop()) != '(')	//후위표기 스택에 '('이후 쓰인 연산자 모두
			{
				charCat(&x);//postFixExp에 입력
			}
		}
		else//괄호 이외의 연산자 일 때
		{
			while (getPriority(postfixStack[postfixStackTop]) >= getPriority(*exp))//현재 연산자가 postfixStack의 top 연산자보다 우선순위가 높아질때까지,
			{
				x = postfixPop();		//postfixStack의 top연산자를 리턴해와서
				charCat(&x);			//문자를 받아 후위표기 스택에 붙임
			}
			postfixPush(*exp);//이후 현재 연산자를 후위표기 스택에 붙임
		}
		exp++;
	}
	while (postfixStackTop != -1)//postfixStack가 빌때까지,
	{
		x = postfixPop();		//postfixStack의 top을 
		charCat(&x);			//postfixExp에 입력
	}
}
void debug()//각 기능 실행 후 결과 출력
{
	printf("\n---DEBUG\n");
	printf("infixExp =  %s\n", infixExp);		//중위표기법으로 표기된 수식
	printf("postExp =  %s\n", postfixExp);		//후위표기법으로 표기된 수식
	printf("eval result = %d\n", evalResult);	//연산 결과 값
	printf("postfixStack : ");					//후위표기시 연산자들을 보관하는 스택
	for (int i = 0; i < MAX_STACK_SIZE; i++)
		printf("%c  ", postfixStack[i]);
	printf("\n");
}
void reset()//모든 스택, 스택 top, 수식, 이외의 변수 초기화
{
	infixExp[0] = '\0';			//중위표기 수식
	postfixExp[0] = '\0';		//후위표기 수식

	for (int i = 0; i < MAX_STACK_SIZE; i++)	//postfixStack 리셋
		postfixStack[i] = '\0';
	postfixStackTop = -1;		//이외의 변수 리셋.
	evalStackTop = -1;
	evalResult = 0;
}
void evaluation()//후위표기법으로 바뀐 수식 계산
{
	int opr1, opr2, i; 					//피연산자1, 피연산자2, 인덱스
	int length = strlen(postfixExp);	//후위표기 스택의 길이
	char symbol; evalStackTop = -1;		//수식의 토큰을 받을 변수, 계산스택top변수
	for (i = 0; i < length; i++)
	{
		symbol = postfixExp[i];				//postfixExp으로부터 차례차례 토큰 받아오기

		if (getToken(symbol) == operand) 	//symbol이 피연산자일때,
		{
			evalPush(symbol - '0');			//문자를 정수로 바꾸어 evalStack에 push
		}
		else								//symbol이 연산자일때, 
		{
			opr2 = evalPop();				//evalstack에서 pop하여 피연산자 지정
			opr1 = evalPop();
			switch (getToken(symbol))		//symbol에 따른 사칙연산 실행 후 몫 다시 evalstack에 push
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
	evalResult = evalPop();					//모든 연산 종료 후 남은 몫 evalResult에 저장
}