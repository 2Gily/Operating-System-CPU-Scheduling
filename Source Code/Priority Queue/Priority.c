#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define SIZE 100
#define QSIZE 20

int now_time = 0; // ���μ��� �ð��� �������� time���� ���� �� 0���� �ʱ�ȭ

typedef struct Process
{
	int type, id, com_time,pri, arr_time, turn_time, remain_time; // �������Ῡ��, ���μ���ID, �켱����, ���� �ð�, ��ȯ�ð�, ���� ���� �ð�
	double nomrtn_t; // ���Թ�ȯ�ð�
}process;


typedef struct Node //���Ḯ��Ʈ ��� ����
{
	process data; //����� ������
	Node *next; // ���� ����� �ּҸ� ����Ű�� ��
	
	int index; // ����� ������ �ε���
}Node;

int inputArr(process* ps);	// input.txt�� �ҷ��� ����ü �迭�� �Է��ϴ� �Լ�

void printArr(process* ps, int cnt); //input.txt���� �ҷ��� �Է°��� ����ϴ� �Լ�

Node* makeNode(process data); // ��带 �����ϴ� �Լ�

void putNode(Node*head, Node* node); // ��带 ����Ʈ�� �߰��ϴ� �Լ�

Node* outNode(Node* head); // ����Ʈ���� ��带 �����ϴ� �Լ�

void print_ps(process* ps); // �����층�� �Ϸ�Ǹ� ����ϴ� �Լ�

void delNode(Node *address); // �� ���� ��带 �����ϰ� ������ ����� �ּҸ� ��ȯ�ϴ� �Լ�

void RR(process* ps, int cnt); // ���� �κ� �����ٸ� �˰��� �Լ�

int main(void)
{
	process p1[SIZE];

	int ps_cnt = 0;

	ps_cnt = inputArr(p1);

	printf("\n|===============================================|\n");
	printf("|=============  input.txt �Է� ��  =============|");
	printf("\n|===============================================|\n");
	printf("| Type|  Proces_ID| Computing_Time| Arrival_time|\n");
	printf("|===============================================|\n");
	printArr(p1, ps_cnt);
	printf("|===============================================|\n");
	printf("\n\n\n");
	printf("\n  |=============================================|\n");
	printf("  |============= �����ٸ� ó�� �� ==============|");
	printf("\n  |=============================================|\n");
	printf("  |  Proces_ID| Computing_Time| Turn_Around_time|\n");
	printf("  |=============================================|\n");
	RR(p1, ps_cnt);
	printf("  |=============================================|\n");


	return 0;
}

void RR(process* ps, int cnt) // ���� �κ� �����ٸ� �˰��� �Լ�
{
	int i, total_time = 0, ps_cnt = 0;

	int min_num = SIZE;

	Node* head = (Node*)malloc(sizeof(Node)); // �� ���� ����� �ּҸ� ����Ű�� head ��带 ����
	head->next = NULL; // head.next �� ���� NULL�� �ʱ�ȭ

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

int inputArr(process* ps) //input.txt�� �ҷ��� ����ü �迭�� �Է��ϴ� �Լ�
{
	int i = 0, cnt = 0;
		
	int temp_time = 0; //����ð��� �ӽ÷� ��Ƶδ� ����

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
			fscanf(fp, "%d,%d,%d", &ps[i].type, &ps[i].id, &ps[i].com_time); // input.txt ������ �ҷ��� ����� ������ ���� �־��ش�.

			if (ps[i].type == 0)
			{
				ps[i].arr_time = temp_time; // type�� 0�̸� ����ð��� ������Ű�� �ʴ´�
			}
			else if (ps[i].type == 1)
			{
				temp_time += QSIZE; // type�� 1�̸� ����ð��� 20��ŭ ������Ų��
				ps[i].arr_time = temp_time;
				ps[i].arr_time = 0;
			}
			else if (ps[i].type == -1)
			{
				temp_time += QSIZE; // type�� -1�̸� ����ð��� 20��ŭ ������Ų��
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

void print_ps(process *ending_ps) // �����층�� �Ϸ�Ǹ� ����ϴ� �Լ�
{
	//printf("process id = %d , computing_time = %d\n", ending_ps->id, ending_ps->com_time);

	/*printf("|%5d|%11d|%15d|%13d|\n", ending_ps->type, ending_ps->id, ending_ps->com_time, ending_ps->arr_time);*/


	ending_ps->turn_time = now_time - (ending_ps->arr_time);
	ending_ps->nomrtn_t = (double)ending_ps->turn_time / (double)ending_ps->com_time;
	printf("  |%11d|%15d|%17d|\n", ending_ps->id, ending_ps->com_time, ending_ps->turn_time);
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

Node* outNode(Node* head)  // ����Ʈ���� ��带 �����ϴ� �Լ�
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

void delNode(Node *address) // �� ���� ��带 �����ϰ� ������ ����� �ּҸ� ��ȯ�ϴ� �Լ�
{
	free(address);
}