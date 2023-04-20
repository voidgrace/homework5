/* circularQ.c
 *
 *  Data Structures, Homework #5
 *  School of Computer Science at Chungbuk National University
 */

#include <stdio.h>
#include <stdlib.h>

#define MAX_QUEUE_SIZE 4
typedef char element;				//����
typedef struct {					//ť ����ü
	element queue[MAX_QUEUE_SIZE];		//ť
	int front, rear;					//����Ʈ�� ����
}QueueType;


QueueType* createQueue();			//ť ���� �Ҵ�
int freeQueue(QueueType* cQ);		//ť ��ȯ
int isEmpty(QueueType* cQ);			//ť�� ������� Ȯ��
int isFull(QueueType* cQ);			//ť�� á���� Ȯ��
void enQueue(QueueType* cQ, element item);	//���� ����
void deQueue(QueueType* cQ, element* item);	//���� ����
void printQ(QueueType* cQ);			//ť ���
void debugQ(QueueType* cQ);			//����Ʈ�� ������ ��ġ ���
element getElement();				//���� �� �ޱ�

int main(void)
{
	QueueType* cQ = createQueue();	//ť �Ҵ� �� �ּ� �ޱ�
	element data;					//
	char command;					//������� ����� ���� ������ ����

	printf("[-----[������][2022041047]-----]");

	//����ڰ� ���� �Ҷ����� �ݺ�
	do {
		printf("\n-----------------------------------------------------\n");
		printf("                     Circular Q                   \n");
		printf("------------------------------------------------------\n");
		printf(" Insert=i,  Delete=d,   PrintQ=p,   Debug=b,   Quit=q \n");
		printf("------------------------------------------------------\n");

		printf("Command = ");	//����ڷ� ���� ��� �ޱ�
		scanf(" %c", &command);

		switch (command) {
		case 'i': case 'I':		//����
			if (isFull(cQ)) 	//���� �� ť�� á�ٸ� break
			break;
			data = getElement();
			enQueue(cQ, data);
			break;

		case 'd': case 'D':		//����
			if(isEmpty(cQ))		//���� �� ť�� ����ٸ� break
			break;				
			deQueue(cQ, &data);
			break;
		case 'p': case 'P':		//ť ���
			printQ(cQ);
			break;
		case 'b': case 'B':		//�����:����Ʈ�� ������ ��ġ ȭ�����
			debugQ(cQ);
			break;
		case 'q': case 'Q':		//������ ť ����� ��ȯ
			freeQueue(cQ);
			break;

		default:				//���� �� ��� �������
			printf("\n       >>>>>   Concentration!!   <<<<<     \n");
			break;
		}

	} while (command != 'q' && command != 'Q');


	return 1;
}

QueueType* createQueue()//ť �����
{
	QueueType* cQ;									//ť ������ ���� ����
	cQ = (QueueType*)calloc(1,sizeof(QueueType));	//ť ���� �Ҵ�
	cQ->front = 0;	
	cQ->rear = 0; 									//����Ʈ�� �� ���� 0���� �ʱ�ȭ
	return cQ;										//������� ť ����
}

int freeQueue(QueueType* cQ)//ť ��ȯ
{
	if (cQ == NULL) return 1;						//ť�� ����� ���� �ʾ����� ���� 1 ����
	free(cQ);										//ť ��ȯ
	return 1;
}

element getElement()//������ ���� �ޱ�
{
	element item;									//���� ���� ����
	printf("Input element = ");
	scanf(" %c", &item);
	return item;									//���� �ް� ����	
}


int isEmpty(QueueType* cQ)//ť�� ������� Ȯ��
{
	if (((cQ->rear) % MAX_QUEUE_SIZE) == ((cQ->front) % MAX_QUEUE_SIZE)) 		//����Ʈ�� ������ ��ĭ�տ� ��ġ�� �ִٸ� FULL
	{
		printf("       >>>>>   EMPTY!!   <<<<<     \n");
		return 1;
	}
	return 0;
}


int isFull(QueueType* cQ)//ť�� á���� Ȯ��
{
	if (((cQ->rear + 1) % MAX_QUEUE_SIZE) == ((cQ->front) % MAX_QUEUE_SIZE))	//��� ����Ʈ�� ��ĭ�տ� ��ġ�� �ִٸ� ENPTY
	{
		printf("       >>>>>   FULL!!   <<<<<     \n");
		return 1;
	}
	return 0;
}

void enQueue(QueueType* cQ, element item)//��ť
{
	
	cQ->rear++;											//rear��ĭ �ڷ�
	cQ->queue[(cQ->rear) % MAX_QUEUE_SIZE]=item;		//rear�� ��ġ�� ��ť
	return;
}

void deQueue(QueueType* cQ, element* item)//��ť
{
	cQ->front++;										//front��ĭ �ڷ�
	cQ->queue[(cQ->front) % MAX_QUEUE_SIZE] = 0;		//front�� ��ġ ��ť
	return;
}


void printQ(QueueType* cQ)//ť ���
{	
	int i, first, last;									//ť�� ó��,��, �ӽ� �ε��� ����

	first = (cQ->front + 1) % MAX_QUEUE_SIZE;			//ť ���� ���۵Ǵ� ��ġ
	last = (cQ->rear + 1) % MAX_QUEUE_SIZE;				//ť ���� ������ ��ġ

	printf("Circular Queue : [");

	i = first;											//�ε��� front������ �ʱ�ȭ
	while (i != last) 									//�ε����� ť ���� �ݱ���� �ݺ�
	{
		printf("%3c", cQ->queue[i]);
		i = (i + 1) % MAX_QUEUE_SIZE;					//���� ť�� �� ����ϱ� ��, i�� ť ũ�⸦ �Ѱܵ� �ٽ� 1�� ���ƿ��� ��.

	}
	printf(" ]\n");
}


void debugQ(QueueType* cQ)//ť �����
{

	printf("\n---DEBUG\n");
	for (int i = 0; i < MAX_QUEUE_SIZE; i++)			//ť �迭�� �� ���� �� �Ǵ� front���
	{
		if (i == cQ->front) 
		{
			printf("  [%d] = front\n", i);				
			continue;						
		}
		printf("  [%d] = %c\n", i, cQ->queue[i]);		

	}

	printf("front = %d, rear = %d\n", 
			cQ->front % MAX_QUEUE_SIZE, cQ->rear % MAX_QUEUE_SIZE);//front�� rear ���
}

