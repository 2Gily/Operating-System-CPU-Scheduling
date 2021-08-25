#include<stdio.h>
#include<stdlib.h>
#define SIZE 100

int now_time = 0; // 전체 프로세스 시간을 전역변수 time으로 정의 후 0으로 초기화
int process_count = 0; // 입력된 프로세스 개수
int QSIZE = 0; //시간할당량의 값을 저장하는 변수
double normalized_turn_around_time_average = 0; // 정규화 된 반환 시간의 평균

typedef struct Process //노드의 데이터 값의 구조체
{
	int type, id, com_time, arr_time, turn_time, remain_time;
	// 실행종료여부, 프로세스ID , 실행시간, 반환시간, 남은 실행시간
	double nomrtn_time;
	// 정규화 된 반환 시간
}process;

typedef struct Node //연결리스트 노드 정의
{
	process data; //노드의 데이터
	struct Node *next; // 다음 노드의 주소를 가르키는 값
}Node;

void RR(process* ps, int cnt); // 라운드 로빈 스케줄링 함수

int inputArr(process* ps);	// input.txt를 불러와 구조체 배열에 입력하는 함수

void printArr(process* ps, int cnt); //input.txt에서 불러온 입력값을 출력하는 함수

Node* makeNode(process data); // 노드를 생성하는 함수

void putNode(Node*head, Node* node); // 노드를 리스트에 추가하는 함수

Node* outNode(Node* head); // 리스트에서 노드를 제거하는 함수

void delNode(Node *address); // 입력받은 주소를 가진 노드를 동적 메모리 할당을 해제하는 함수

void print_ps(process* ps); // 스케쥴링이 완료되면 노드의 데이터를 출력하는 함수



int main(void)
{
	printf("\n  Input the time quantum : ");

	scanf("%d", &QSIZE);

	int ps_cnt = 0;

	process p1[SIZE];

	ps_cnt = inputArr(p1);

	printf("\n  |=================================|\n");
	printf("  |======  input.txt 입력 값  ======|");
	printf("\n  |=================================|\n");
	printf("  | Type|  Proces_ID| Computing_Time|\n");
	printf("  |=================================|\n");
	printArr(p1, ps_cnt);
	printf("  |=================================|\n");
	printf("\n\n");
	printf("\n  |================================================================================|\n");
	printf("  |=============================== 스케줄링 처리 후 ===============================|");
	printf("\n  |================================================================================|\n");
	printf("  |  Proces_ID| Computing_Time| Arrival_time| Turn_Around_time|  Normalized TA time|\n");
	printf("  |================================================================================|\n");
	RR(p1, ps_cnt);
	printf("  |================================================================================|\n");
	printf("   Normalized_turn_around_time_average : %.2lf\n", normalized_turn_around_time_average / process_count);
	printf("   Time quantum : %d \n\n", QSIZE);

	getchar();

	return 0;
}

void RR(process* ps, int cnt) // 라운드 로빈 스케줄링 알고리즘 함수
{
	int i = 0;

	Node* head = (Node*)malloc(sizeof(Node)); // 맨 앞의 노드의 주소를 가르키는 head 노드를 생성
	head->next = NULL; // head.next 의 값을 NULL로 초기화

	for (i = 0; i < cnt; i++)
	{
		if (ps[i].type == 0) //type == 0 이면 노드를 생성하고 리스트에 추가한다
		{
			ps[i].arr_time = now_time; // 도착시간을 현재 프로세스 진행 시간으로 설정한다.
			process_count++; // 프로세스의 개수가 1개 증가한다.
			putNode(head, makeNode(ps[i]));
		}
		else if (ps[i].type == 1) //type == 1 이면 Round Robin 함수를 1회 실행 시킨다.
		{
			Node * deleting_one = outNode(head); // 리스트에서 빼줄 노드의 주소를 담아두는 변수

			if (deleting_one != NULL) // 리스트에서 뺄 노드의 주소가 NULL 아닐경우의 조건
			{
				if (deleting_one->data.remain_time <= 0) // 리스트에서 뺄 노드의 남아있는 실행시간을 완료했을 경우
				{
					print_ps(&(deleting_one->data)); // 완료된 프로세스의 데이터값을 출력하는 함수
				}
				else
				{
					putNode(head, deleting_one); // 프로세스가 완료되지 않고 실행시간이 남아 있다면 마지막 리스트에 연결한다
				}
			}
		}
		else if (ps[i].type == -1) //type == -1 이면 노드생성가 리스트 추가를 완료하고 리스트에 노드가 없을때까지 Round Robin을 실행한다.
		{
			while (1)
			{
				if (head->next == NULL) // 리스트에 노드가 없으면 while 반복문에서 나간다.
				{
					break;
				}
				Node * deleting_one = outNode(head);

				if (deleting_one != NULL) // 리스트에 남아있는 노드가 있으면 리스트에서 노드가 남지 않을때까지 Round Robin을 실행한다
				{
					if (deleting_one->data.remain_time <= 0) // 스케쥴링 할 프로세스의 실행시간을 모두 완료한 경우 출력한다
					{
						print_ps(&(deleting_one->data)); // 완료된 프로세스의 데이터값을 출력하는 함수
						delNode(deleting_one); // 출력한 데이터의 노드 동적 메모리 할당 해제
					}
					else
					{
						putNode(head, deleting_one); // 프로세스가 완료되지 않고 실행시간이 남아 있다면 마지막 리스트에 연결한다
					}
				}

			}
		}
	}
}

int inputArr(process* ps) //input.txt를 불러와 구조체 배열에 입력하는 함수
{
	int i = 0, count = 0;

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

			ps[i].remain_time = ps[i].com_time; // 남아있는 서비스 시간 = 입력된 서비스 시간으로 초기화

			if (ps[i].type == -1) // type = -1 이라면 배열에 1을 추가하고 입력을 종료한다.
			{
				count++;
				break;
			}
		count++;
		}
	}
	fclose(fp);

	return count; // 입력된 배열의 개수를 반환
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
			printf("  |%5d|           |               | \n", ps[i].type);
		}
	}
}

void print_ps(process *ending_ps) // 스케쥴링이 완료되면 출력하는 함수
{
	ending_ps->turn_time = now_time - (ending_ps->arr_time); // 반환시간 = 종료시간 - 도착시간
	ending_ps->nomrtn_time = (double)ending_ps->turn_time / (double)ending_ps->com_time; // 정규화된 반환시간 = 반환시간 / 실행시간
	normalized_turn_around_time_average += ending_ps->nomrtn_time;
	printf("  |%11d|%15d|%13d|%17d|%20.2lf|\n", ending_ps->id, ending_ps->com_time, ending_ps->arr_time, ending_ps->turn_time, ending_ps->nomrtn_time); // Process ID, 실행시간, 반환시간을 출력하는 함수
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
	if ((head->next) == NULL) //head->next 가 NULL이면 리스트에 노드가 없는 상태이다
	{
		head->next = node;
	}
	else
	{
		Node* finding_lastone = head->next; // 마지막 노드의 주소를 받아놓을 구조체 포인터 변수

		while (finding_lastone->next != NULL)
		{
			finding_lastone = finding_lastone->next;
			//finding_lastone->next가 NULL을 가지면 마지막 노드이므로 finding_lastone이 NULL을 찾을 때까지 반복한다.
		}

		finding_lastone->next = node;
	}
}

Node* outNode(Node* head)  // 리스트에서 노드를 제거하는 함수
{
	Node* address;

	address = head->next;

	if (head->next->data.remain_time <= QSIZE) // 남아있는 실행시간이 time_quatum 보다 작으면 프로세스가 완료된다.
	{
		now_time += head->next->data.remain_time; // 프로세스가 완료되면 마지막으로 실행한 실행시간만큼 전체 프로세스 시간이 증가한다.
	}
	else  now_time += QSIZE; // 프로세스가 완료되지 않고 Round Robin이 한번 더 실행되면 time_quatum 만큼 전체 프로세스 시간이 증가한다.

	if (head->next == NULL)
	{
		return NULL;
	}
	else
	{
		head->next->data.remain_time = head->next->data.remain_time - QSIZE; // 맨 처음의 노드의 남은 실행시간을 time_quatum 만큼 뺀다.

		head->next = head->next->next; // head->next 를 맨 앞에있던 노드가 아닌 맨 앞의 다음 노드의 주소를 가르킨다
	}
	address->next = NULL;
	return address;
}

void delNode(Node *address) // 입력받은 주소를 가진 노드를 동적 메모리 할당을 해제하는 함수
{
	free(address);
}