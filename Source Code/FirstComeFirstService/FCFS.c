#include<stdio.h> 
#include<stdlib.h> 
#define SIZE 100

int time = 0; // ��ü ���μ��� ����ð�
int process_count = 0; // �Էµ� ���μ��� ����
double normalized_turn_around_time_average = 0; // ����ȭ�� ��ȯ �ð��� ���

typedef struct process { //��忡 �� ������ ����ü ����
	int type, id, com_time, arr_time, turn_time;
	double nta_time;
}process;

typedef struct Node { //����� ����ü ����
	process data;
	struct Node *next;
}Node;

void FCFS(process* ps, int cnt); // ���Լ��� �����층�� �����ϴ� �Լ�

int inputArr(process* ps); // input.txt �κ��� �����͸� �ҷ��� ����� �����Ϳ� �ִ� �Լ�

void printArr(process* ps, int cnt); // input.txt���� �ҷ��� ��� ����ü�� �Է��� �����͸� ����ϴ� �Լ�

void print_ps(process *ending_ps); // �Ϸ�� ���μ����� ����ϴ� �Լ�

Node* makeNode(process data); // ��带 �����ϴ� �Լ�

void putNode(Node*head, Node* node); // ����Ʈ�� ��带 �߰��ϴ� �Լ�

Node* outNode(Node* head); // ��带 ����Ʈ���� ���� �Լ�

void delNode(Node *address); // ��忡 �����Ҵ�� �޸𸮸� �����ϴ� �Լ�

int main(void)
{
	process p1[SIZE];

	int ps_cnt = 0;

	ps_cnt = inputArr(p1);

	printf("\n  |=================================|\n");
	printf("  |======  input.txt �Է� ��  ======|\n");
	printf("  |=================================|\n");
	printf("  | Type|  Proces_ID| Computing_Time|\n");
	printf("  |=================================|\n");
	printArr(p1, ps_cnt);
	printf("  |=================================|\n");

	printf("\n\n\n");

	printf("  |===============================================================================|\n");
	printf("  |=============================== �����ٸ� ó�� �� ==============================|\n");
	printf("  |===============================================================================|\n");
	printf("  |  Proces_ID| Computing_Time| Arrival_time| Turn_Around_time| Normalized TA time|\n");
	printf("  |===============================================================================|\n");
	FCFS(p1, ps_cnt);
	printf("  |===============================================================================|\n");
	printf("   Normalized_turn_around_time_average : %.2lf\n", normalized_turn_around_time_average / process_count);

	getchar();

	return 0;
}

void FCFS(process* ps, int cnt) // ���Լ��� �����층�� �����ϴ� �Լ�
{
	int i = 0, ps_cnt = 0;
	int flag = 1;

	Node* head = (Node*)malloc(sizeof(Node)); // �� ���� ����� �ּҸ� ����Ű�� head ��带 ����
	head->next = NULL; // head.next �� ���� NULL�� �ʱ�ȭ

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
		else if (ps[i].type == 1) //type == 1 �̸� FCFS �����ٷ��� 1ȸ ���� ��Ų��.
		{

			Node * deleting_one = outNode(head); // ����Ʈ���� ���� ����� �ּҸ� ��Ƶδ� ����

			time += deleting_one->data.com_time;

			if (deleting_one != NULL) // ����Ʈ���� �� ����� �ּҰ� NULL �ƴҰ���� ����
			{
				print_ps(&(deleting_one->data)); // �Ϸ�� ���μ����� �����Ͱ��� ����ϴ� �Լ�
			}
		}
		else if (ps[i].type == -1) //type == -1 �̸� �������� ����Ʈ �߰��� �Ϸ��ϰ� ����Ʈ�� ��尡 ���������� ���Լ��� �����ٸ��� �����Ѵ�.
		{
			while (1)
			{

				if (head->next == NULL) // ����Ʈ�� ��尡 ������ while �ݺ������� ������.
				{
					flag = 0;
					break;
				}

				Node * deleting_one = outNode(head);

				time += deleting_one->data.com_time;

				if (deleting_one != NULL) // ����Ʈ�� �����ִ� ��尡 ������ ����Ʈ���� ��尡 ���� ���������� �켱���� �����ٷ��� �����Ѵ�
				{

					print_ps(&(deleting_one->data)); // �Ϸ�� ���μ����� �����Ͱ��� ����ϴ� �Լ�
					delNode(deleting_one); // ����� �������� ����� ���� �޸� �Ҵ� ����
				}
			}

		}

		i++;
	}
}

void printArr(process* ps, int cnt) //input.txt���� �ҷ��� �Է°��� ����ϴ� �Լ�
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

int inputArr(process* ps) // input.txt���� �ҷ��� ��� ����ü�� �Է��� �����͸� ����ϴ� �Լ�
{
	int i = 0, cnt = 0;

	FILE *fp = fopen("input.txt", "r"); // input.txt ������ �б���� �ҷ�����

	if (fp == NULL)
	{
		printf("���Ͽ� �ƹ��͵� �����ϴ�.");
		return -1;
	}
	else
	{
		for (i = 0; i < SIZE; i++)
		{
			fscanf(fp, "%d,%d,%d,%d", &ps[i].type, &ps[i].id, &ps[i].com_time); // input.txt ������ �ҷ��� ����� ������ ���� �־��ش�.

			if (ps[i].type == -1) // type = -1 �̸� �Է��� �ߴ��Ѵ�
			{
				break;
			}
		cnt++;
		}
	}

	fclose(fp);

	return cnt;
}

void print_ps(process *ending_ps) // �����층�� �Ϸ�Ǹ� ����ϴ� �Լ�
{
	ending_ps->turn_time = time - ending_ps->arr_time; // ��ȯ�ð� = ����ð� - �����ð�
	ending_ps->nta_time = (double)ending_ps->turn_time / (double)ending_ps->com_time; // ����ȭ�� ��ȯ�ð� = ��ȯ�ð� / ����ð�
	normalized_turn_around_time_average += ending_ps->nta_time;
	printf("  |%11d|%15d|%13d|%17d|%19.2lf|\n", ending_ps->id,  ending_ps->com_time, ending_ps->arr_time, ending_ps->turn_time, ending_ps->nta_time);
}

Node* makeNode(process data) // ��带 �����ϴ� �Լ�
{
	Node *newNode = (Node*)malloc(sizeof(Node));
	newNode->next = NULL;
	newNode->data = data;

	return newNode;
}

void putNode(Node*head, Node* node) // ��带 ����Ʈ�� �߰��ϴ� �Լ�
{
	if ((head->next) == NULL) // head->next �� NULL �� ������ ������  head�� node�� �����Ѵ�
	{
		head->next = node;
	}
	else
	{
		Node* finding_lastone = head->next;  // ������ ����� �ּҸ� ��Ƶ� ������ ����

		while (finding_lastone->next != NULL) // head->next �� NULL �� �ƴϸ� NULL�� �ɶ����� �ݺ��Ѵ�
		{
			finding_lastone = finding_lastone->next; // NULL�� �ƴϸ� ������ ���� finding_lastone�� finding_lastone->next �� �� �ִ´�
		}
		finding_lastone->next = node; // finding_lastone->next�� ������ ����� �ּҸ� ������ �ִ�.
	}

}

Node* outNode(Node* head)  // ����Ʈ���� ��带 �����ϴ� �Լ�
{
	Node* address; // ����Ʈ���� �� ����� �ּҸ� ��Ƶ� ������ ����
	address = head->next;

	if (head->next == NULL) // head->next �� NULL �̸� �ƹ��͵� ���ϰ� NULL�� ��ȯ�Ѵ�
	{
		return NULL;
	}
	else
	{
		head->next = head->next->next; //head->next �� NULL�� �ƴϸ� head->next�� ������ִ� ����� next ���� �޾Ƽ� �����Ѵ�
	}
	address->next = NULL; // ����Ʈ���� ���� ����� next ���� NULL�� �ִ´�
	return address;
}

void delNode(Node *address) // ��忡 �����Ҵ�� �޸𸮸� �����ϴ� �Լ�
{
	free(address);
}