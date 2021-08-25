#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define SIZE 100
#define QSIZE 20

int now_time = 0; // 프로세스 시간을 전역변수 time으로 정의 후 0으로 초기화

typedef struct Process
{
	int type, id, com_time,pri, arr_time, turn_time, remain_time; // 실행종료여부, 프로세스ID, 우선순위, 서비스 시간, 반환시간, 남은 서비스 시간
	double nomrtn_t; // 정규반환시간
}process;


typedef struct Node //연결리스트 노드 정의
{
	process data; //노드의 데이터
	Node *next; // 다음 노드의 주소를 가르키는 값
	
	int index; // 노드의 정보값 인덱스
}Node;

int inputArr(process* ps);	// input.txt를 불러와 구조체 배열에 입력하는 함수

void printArr(process* ps, int cnt); //input.txt에서 불러온 입력값을 출력하는 함수

Node* makeNode(process data); // 노드를 생성하는 함수

void putNode(Node*head, Node* node); // 노드를 리스트에 추가하는 함수

Node* outNode(Node* head); // 리스트에서 노드를 제거하는 함수

void print_ps(process* ps); // 스케쥴링이 완료되면 출력하는 함수

void delNode(Node *address); // 맨 앞의 노드를 삭제하고 삭제한 노드의 주소를 반환하는 함수

void RR(process* ps, int cnt); // 라운드 로빈 스케줄링 알고리즘 함수

int main(void)
{
	process p1[SIZE];

	int ps_cnt = 0;

	ps_cnt = inputArr(p1);

	printf("\n|===============================================|\n");
	printf("|=============  input.txt 입력 값  =============|");
	printf("\n|===============================================|\n");
	printf("| Type|  Proces_ID| Computing_Time| Arrival_time|\n");
	printf("|===============================================|\n");
	printArr(p1, ps_cnt);
	printf("|===============================================|\n");
	printf("\n\n\n");
	printf("\n  |=============================================|\n");
	printf("  |============= 스케줄링 처리 후 ==============|");
	printf("\n  |=============================================|\n");
	printf("  |  Proces_ID| Computing_Time| Turn_Around_time|\n");
	printf("  |=============================================|\n");
	RR(p1, ps_cnt);
	printf("  |=============================================|\n");


	return 0;
}

void RR(process* ps, int cnt) // 라운드 로빈 스케줄링 알고리즘 함수
{
	int i, total_time = 0, ps_cnt = 0;

	int min_num = SIZE;

	Node* head = (Node*)malloc(sizeof(Node)); // 맨 앞의 노드의 주소를 가르키는 head 노드를 생성
	head->next = NULL; // head.next 의 값을 NULL로 초기화

	process *p1;

	ps_cnt = inputArr(ps);

	p1 = &head->data;

	for (i = 0; i <= cnt; i++)
	{
		if (ps[i].type == 0)
		{
			putNode(head, makeNode(ps[i]));

			if (ps[i].pri < min_num)
			{
				min_num = ps[i].pri < min_num;
			}
		}
		else if (ps[i].type == 1)
		{
			Node * deleting_one = outNode(head);

			if (deleting_one != NULL)
			{
				if (deleting_one->data.remain_time <= 0)
				{
					print_ps(&(deleting_one->data));

				}
				else
				{
					putNode(head, deleting_one);
				}
			}
		}
		else if (ps[i].type == -1)
		{
			while (1)
			{
				if (head->next == NULL)
				{
					break;
				}
				Node * deleting_one = outNode(head);

				if (deleting_one != NULL)
				{
					if (deleting_one->data.remain_time <= 0)
					{
						print_ps(&(deleting_one->data));
					}
					else
					{
						putNode(head, deleting_one);
					}
				}

			}
		}
	}



}

int inputArr(process* ps) //input.txt를 불러와 구조체 배열에 입력하는 함수
{
	int i = 0, cnt = 0;
		
	int temp_time = 0; //종료시간을 임시로 담아두는 변수

	FILE *fp = fopen("input.txt", "r"); // input.txt 파일을 읽기모드로 불러오기

	if (fp == NULL)
	{
		printf("파일에 아무것도 없습니다.");
		return -1;
	}
	else
	{
		for (i = 0; i < SIZE; i++)
		{
			fscanf(fp, "%d,%d,%d", &ps[i].type, &ps[i].id, &ps[i].com_time); // input.txt 파일을 불러와 노드의 데이터 값에 넣어준다.

			if (ps[i].type == 0)
			{
				ps[i].arr_time = temp_time; // type이 0이면 종료시간을 증가시키지 않는다
			}
			else if (ps[i].type == 1)
			{
				temp_time += QSIZE; // type이 1이면 종료시간을 20만큼 증가시킨다
				ps[i].arr_time = temp_time;
				ps[i].arr_time = 0;
			}
			else if (ps[i].type == -1)
			{
				temp_time += QSIZE; // type이 -1이면 종료시간을 20만큼 증가시킨다
				ps[i].arr_time = temp_time;
				ps[i].arr_time = 0;
				break;
			}


			cnt++;
		}
	}
	fclose(fp);

	return cnt;
}

void print_ps(process *ending_ps) // 스케쥴링이 완료되면 출력하는 함수
{
	//printf("process id = %d , computing_time = %d\n", ending_ps->id, ending_ps->com_time);

	/*printf("|%5d|%11d|%15d|%13d|\n", ending_ps->type, ending_ps->id, ending_ps->com_time, ending_ps->arr_time);*/


	ending_ps->turn_time = now_time - (ending_ps->arr_time);
	ending_ps->nomrtn_t = (double)ending_ps->turn_time / (double)ending_ps->com_time;
	printf("  |%11d|%15d|%17d|\n", ending_ps->id, ending_ps->com_time, ending_ps->turn_time);
}


Node* makeNode(process data) // 노드를 생성하는 함수
{
	Node *newNode = (Node*)malloc(sizeof(Node));
	newNode->next = NULL;
	newNode->data = data;

	return newNode;
}

void putNode(Node*head, Node* node) // 노드를 리스트에 추가하는 함수
{
	if ((head->next) == NULL)
	{
		head->next = node;
	}
	else
	{
		Node* finding_lastone = head->next;

		while (finding_lastone->next != NULL)
		{
			finding_lastone = finding_lastone->next;
		}

		finding_lastone->next = node;
	}

}

Node* outNode(Node* head)  // 리스트에서 노드를 제거하는 함수
{
	Node* address;
	address = head->next;
	if (head->next->data.remain_time <= 20)
	{
		now_time += head->next->data.remain_time;
	}
	else  now_time += QSIZE;
	//printf("%d\n", now_time);

	if (head->next == NULL)
	{
		return NULL;
	}
	else
	{
		head->next->data.remain_time = head->next->data.remain_time - QSIZE;

		head->next = head->next->next;

	}
	address->next = NULL;
	return address;
}

void delNode(Node *address) // 맨 앞의 노드를 삭제하고 삭제한 노드의 주소를 반환하는 함수
{
	free(address);
}