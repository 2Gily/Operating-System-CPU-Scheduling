#include<stdio.h> 
#include<stdlib.h> 
#define SIZE 100

int time = 0; // 전체 프로세스 진행시간
int process_count = 0; // 입력된 프로세스 개수
double normalized_turn_around_time_average = 0; // 정규화된 반환 시간의 평균

typedef struct process { //노드에 들어갈 데이터 구조체 정의
	int type, id, com_time, arr_time, turn_time;
	double nta_time;
}process;

typedef struct Node { //노드의 구조체 정의
	process data;
	struct Node *next;
}Node;

void FCFS(process* ps, int cnt); // 선입선출 스케쥴링을 실행하는 함수

int inputArr(process* ps); // input.txt 로부터 데이터를 불러와 노드의 데이터에 넣는 함수

void printArr(process* ps, int cnt); // input.txt에서 불러와 노드 구조체에 입력한 데이터를 출력하는 함수

void print_ps(process *ending_ps); // 완료된 프로세스를 출력하는 함수

Node* makeNode(process data); // 노드를 생성하는 함수

void putNode(Node*head, Node* node); // 리스트에 노드를 추가하는 함수

Node* outNode(Node* head); // 노드를 리스트에서 빼는 함수

void delNode(Node *address); // 노드에 동적할당된 메모리를 해제하는 함수

int main(void)
{
	process p1[SIZE];

	int ps_cnt = 0;

	ps_cnt = inputArr(p1);

	printf("\n  |=================================|\n");
	printf("  |======  input.txt 입력 값  ======|\n");
	printf("  |=================================|\n");
	printf("  | Type|  Proces_ID| Computing_Time|\n");
	printf("  |=================================|\n");
	printArr(p1, ps_cnt);
	printf("  |=================================|\n");

	printf("\n\n\n");

	printf("  |===============================================================================|\n");
	printf("  |=============================== 스케줄링 처리 후 ==============================|\n");
	printf("  |===============================================================================|\n");
	printf("  |  Proces_ID| Computing_Time| Arrival_time| Turn_Around_time| Normalized TA time|\n");
	printf("  |===============================================================================|\n");
	FCFS(p1, ps_cnt);
	printf("  |===============================================================================|\n");
	printf("   Normalized_turn_around_time_average : %.2lf\n", normalized_turn_around_time_average / process_count);

	getchar();

	return 0;
}

void FCFS(process* ps, int cnt) // 선입선출 스케쥴링을 실행하는 함수
{
	int i = 0, ps_cnt = 0;
	int flag = 1;

	Node* head = (Node*)malloc(sizeof(Node)); // 맨 앞의 노드의 주소를 가리키는 head 노드를 생성
	head->next = NULL; // head.next 의 값을 NULL로 초기화

	process *p1;

	ps_cnt = inputArr(ps);

	p1 = &(head->data);

	while (flag)
	{
		if (ps[i].type == 0)
		{
			process_count++;
			ps[i].arr_time = time;
			putNode(head, makeNode(ps[i]));
		}
		else if (ps[i].type == 1) //type == 1 이면 FCFS 스케줄러를 1회 실행 시킨다.
		{

			Node * deleting_one = outNode(head); // 리스트에서 빼줄 노드의 주소를 담아두는 변수

			time += deleting_one->data.com_time;

			if (deleting_one != NULL) // 리스트에서 뺄 노드의 주소가 NULL 아닐경우의 조건
			{
				print_ps(&(deleting_one->data)); // 완료된 프로세스의 데이터값을 출력하는 함수
			}
		}
		else if (ps[i].type == -1) //type == -1 이면 노드생성가 리스트 추가를 완료하고 리스트에 노드가 없을때까지 선입선출 스케줄링을 실행한다.
		{
			while (1)
			{

				if (head->next == NULL) // 리스트에 노드가 없으면 while 반복문에서 나간다.
				{
					flag = 0;
					break;
				}

				Node * deleting_one = outNode(head);

				time += deleting_one->data.com_time;

				if (deleting_one != NULL) // 리스트에 남아있는 노드가 있으면 리스트에서 노드가 남지 않을때까지 우선순위 스케줄러를 실행한다
				{

					print_ps(&(deleting_one->data)); // 완료된 프로세스의 데이터값을 출력하는 함수
					delNode(deleting_one); // 출력한 데이터의 노드의 동적 메모리 할당 해제
				}
			}

		}

		i++;
	}
}

void printArr(process* ps, int cnt) //input.txt에서 불러온 입력값을 출력하는 함수
{
	int i = 0;

	for (i = 0; i <= cnt; i++)
	{
		if (ps[i].type == 0 || ps[i].type == 1)
		{
			printf("  |%5d|%11d|%15d|\n", ps[i].type, ps[i].id, ps[i].com_time);
		}
		else if (ps[i].type == -1)
		{
			printf("  |%5d|           |               |\n", ps[i].type);
		}
	}
}

int inputArr(process* ps) // input.txt에서 불러와 노드 구조체에 입력한 데이터를 출력하는 함수
{
	int i = 0, cnt = 0;

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
			fscanf(fp, "%d,%d,%d,%d", &ps[i].type, &ps[i].id, &ps[i].com_time); // input.txt 파일을 불러와 노드의 데이터 값에 넣어준다.

			if (ps[i].type == -1) // type = -1 이면 입력을 중단한다
			{
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
	ending_ps->turn_time = time - ending_ps->arr_time; // 반환시간 = 종료시간 - 도착시간
	ending_ps->nta_time = (double)ending_ps->turn_time / (double)ending_ps->com_time; // 정규화된 반환시간 = 반환시간 / 실행시간
	normalized_turn_around_time_average += ending_ps->nta_time;
	printf("  |%11d|%15d|%13d|%17d|%19.2lf|\n", ending_ps->id,  ending_ps->com_time, ending_ps->arr_time, ending_ps->turn_time, ending_ps->nta_time);
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
	if ((head->next) == NULL) // head->next 가 NULL 을 가지고 있으면  head에 node를 연결한다
	{
		head->next = node;
	}
	else
	{
		Node* finding_lastone = head->next;  // 마지막 노드의 주소를 담아둘 포인터 변수

		while (finding_lastone->next != NULL) // head->next 가 NULL 이 아니면 NULL이 될때까지 반복한다
		{
			finding_lastone = finding_lastone->next; // NULL이 아니면 포인터 변수 finding_lastone에 finding_lastone->next 값 을 넣는다
		}
		finding_lastone->next = node; // finding_lastone->next는 마지막 노드의 주소를 가지고 있다.
	}

}

Node* outNode(Node* head)  // 리스트에서 노드를 제거하는 함수
{
	Node* address; // 리스트에서 뺄 노드의 주소를 담아둘 포인터 변수
	address = head->next;

	if (head->next == NULL) // head->next 가 NULL 이면 아무것도 안하고 NULL을 반환한다
	{
		return NULL;
	}
	else
	{
		head->next = head->next->next; //head->next 가 NULL이 아니면 head->next를 연결되있는 노드의 next 값을 받아서 연결한다
	}
	address->next = NULL; // 리스트에서 빼낸 노드의 next 값에 NULL을 넣는다
	return address;
}

void delNode(Node *address) // 노드에 동적할당된 메모리를 해제하는 함수
{
	free(address);
}