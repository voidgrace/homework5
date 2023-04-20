/* circularQ.c
 *
 *  Data Structures, Homework #5
 *  School of Computer Science at Chungbuk National University
 */

#include <stdio.h>
#include <stdlib.h>

#define MAX_QUEUE_SIZE 4
typedef char element;				//원소
typedef struct {					//큐 구조체
	element queue[MAX_QUEUE_SIZE];		//큐
	int front, rear;					//프론트와 리어
}QueueType;


QueueType* createQueue();			//큐 공간 할당
int freeQueue(QueueType* cQ);		//큐 반환
int isEmpty(QueueType* cQ);			//큐가 비었는지 확인
int isFull(QueueType* cQ);			//큐가 찼는지 확인
void enQueue(QueueType* cQ, element item);	//원소 삽입
void deQueue(QueueType* cQ, element* item);	//원소 제거
void printQ(QueueType* cQ);			//큐 출력
void debugQ(QueueType* cQ);			//프론트와 리어의 위치 출력
element getElement();				//원소 값 받기

int main(void)
{
	QueueType* cQ = createQueue();	//큐 할당 후 주소 받기
	element data;					//
	char command;					//사용자의 명령을 받을 문자형 변수

	printf("[-----[강은혜][2022041047]-----]");

	//사용자가 종료 할때까지 반복
	do {
		printf("\n-----------------------------------------------------\n");
		printf("                     Circular Q                   \n");
		printf("------------------------------------------------------\n");
		printf(" Insert=i,  Delete=d,   PrintQ=p,   Debug=b,   Quit=q \n");
		printf("------------------------------------------------------\n");

		printf("Command = ");	//사용자로 부터 명령 받기
		scanf(" %c", &command);

		switch (command) {
		case 'i': case 'I':		//삽입
			if (isFull(cQ)) 	//삽입 시 큐가 찼다면 break
			break;
			data = getElement();
			enQueue(cQ, data);
			break;

		case 'd': case 'D':		//삭제
			if(isEmpty(cQ))		//삭제 시 큐가 비었다면 break
			break;				
			deQueue(cQ, &data);
			break;
		case 'p': case 'P':		//큐 출력
			printQ(cQ);
			break;
		case 'b': case 'B':		//디버그:프론트와 리어의 위치 화면출력
			debugQ(cQ);
			break;
		case 'q': case 'Q':		//끝내고 큐 저장소 반환
			freeQueue(cQ);
			break;

		default:				//예외 시 경고 문자출력
			printf("\n       >>>>>   Concentration!!   <<<<<     \n");
			break;
		}

	} while (command != 'q' && command != 'Q');


	return 1;
}

QueueType* createQueue()//큐 만들기
{
	QueueType* cQ;									//큐 포인터 변수 선언
	cQ = (QueueType*)calloc(1,sizeof(QueueType));	//큐 공간 할당
	cQ->front = 0;	
	cQ->rear = 0; 									//프론트값 및 리어 0으로 초기화
	return cQ;										//만들어진 큐 리턴
}

int freeQueue(QueueType* cQ)//큐 반환
{
	if (cQ == NULL) return 1;						//큐가 만들어 지지 않았으면 오류 1 리턴
	free(cQ);										//큐 반환
	return 1;
}

element getElement()//삽입할 문자 받기
{
	element item;									//문자 받을 변수
	printf("Input element = ");
	scanf(" %c", &item);
	return item;									//문자 받고 리턴	
}


int isEmpty(QueueType* cQ)//큐가 비었는지 확인
{
	if (((cQ->rear) % MAX_QUEUE_SIZE) == ((cQ->front) % MAX_QUEUE_SIZE)) 		//프론트가 리어의 한칸앞에 위치해 있다면 FULL
	{
		printf("       >>>>>   EMPTY!!   <<<<<     \n");
		return 1;
	}
	return 0;
}


int isFull(QueueType* cQ)//큐가 찼는지 확인
{
	if (((cQ->rear + 1) % MAX_QUEUE_SIZE) == ((cQ->front) % MAX_QUEUE_SIZE))	//리어가 프론트의 한칸앞에 위치해 있다면 ENPTY
	{
		printf("       >>>>>   FULL!!   <<<<<     \n");
		return 1;
	}
	return 0;
}

void enQueue(QueueType* cQ, element item)//인큐
{
	
	cQ->rear++;											//rear한칸 뒤로
	cQ->queue[(cQ->rear) % MAX_QUEUE_SIZE]=item;		//rear의 위치에 인큐
	return;
}

void deQueue(QueueType* cQ, element* item)//디큐
{
	cQ->front++;										//front한칸 뒤로
	cQ->queue[(cQ->front) % MAX_QUEUE_SIZE] = 0;		//front의 위치 디큐
	return;
}


void printQ(QueueType* cQ)//큐 출력
{	
	int i, first, last;									//큐의 처음,끝, 임시 인덱스 변수

	first = (cQ->front + 1) % MAX_QUEUE_SIZE;			//큐 값이 시작되는 위치
	last = (cQ->rear + 1) % MAX_QUEUE_SIZE;				//큐 값이 끝나는 위치

	printf("Circular Queue : [");

	i = first;											//인덱스 front값으로 초기화
	while (i != last) 									//인덱스가 큐 끝에 닫기까지 반복
	{
		printf("%3c", cQ->queue[i]);
		i = (i + 1) % MAX_QUEUE_SIZE;					//만일 큐를 다 출력하기 전, i가 큐 크기를 넘겨도 다시 1로 돌아오게 함.

	}
	printf(" ]\n");
}


void debugQ(QueueType* cQ)//큐 디버그
{

	printf("\n---DEBUG\n");
	for (int i = 0; i < MAX_QUEUE_SIZE; i++)			//큐 배열의 각 원소 값 또는 front출력
	{
		if (i == cQ->front) 
		{
			printf("  [%d] = front\n", i);				
			continue;						
		}
		printf("  [%d] = %c\n", i, cQ->queue[i]);		

	}

	printf("front = %d, rear = %d\n", 
			cQ->front % MAX_QUEUE_SIZE, cQ->rear % MAX_QUEUE_SIZE);//front와 rear 출력
}

