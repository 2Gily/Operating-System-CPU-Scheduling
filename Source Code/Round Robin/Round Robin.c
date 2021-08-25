#include<stdio.h>
#include<stdlib.h>
#define SIZE 100

int now_time = 0; // ��ü ���μ��� �ð��� �������� time���� ���� �� 0���� �ʱ�ȭ
int process_count = 0; // �Էµ� ���μ��� ����
int QSIZE = 0; //�ð��Ҵ緮�� ���� �����ϴ� ����
double normalized_turn_around_time_average = 0; // ����ȭ �� ��ȯ �ð��� ���

typedef struct Process //����� ������ ���� ����ü
{
	int type, id, com_time, arr_time, turn_time, remain_time;
	// �������Ῡ��, ���μ���ID , ����ð�, ��ȯ�ð�, ���� ����ð�
	double nomrtn_time;
	// ����ȭ �� ��ȯ �ð�
}process;

typedef struct Node //���Ḯ��Ʈ ��� ����
{
	process data; //����� ������
	struct Node *next; // ���� ����� �ּҸ� ����Ű�� ��
}Node;

void RR(process* ps, int cnt); // ���� �κ� �����ٸ� �Լ�

int inputArr(process* ps);	// input.txt�� �ҷ��� ����ü �迭�� �Է��ϴ� �Լ�

void printArr(process* ps, int cnt); //input.txt���� �ҷ��� �Է°��� ����ϴ� �Լ�

Node* makeNode(process data); // ��带 �����ϴ� �Լ�

void putNode(Node*head, Node* node); // ��带 ����Ʈ�� �߰��ϴ� �Լ�

Node* outNode(Node* head); // ����Ʈ���� ��带 �����ϴ� �Լ�

void delNode(Node *address); // �Է¹��� �ּҸ� ���� ��带 ���� �޸� �Ҵ��� �����ϴ� �Լ�

void print_ps(process* ps); // �����층�� �Ϸ�Ǹ� ����� �����͸� ����ϴ� �Լ�



int main(void)
{
	printf("\n  Input the time quantum : ");

	scanf("%d", &QSIZE);

	int ps_cnt = 0;

	process p1[SIZE];

	ps_cnt = inputArr(p1);

	printf("\n  |=================================|\n");
	printf("  |======  input.txt �Է� ��  ======|");
	printf("\n  |=================================|\n");
	printf("  | Type|  Proces_ID| Computing_Time|\n");
	printf("  |=================================|\n");
	printArr(p1, ps_cnt);
	printf("  |=================================|\n");
	printf("\n\n");
	printf("\n  |================================================================================|\n");
	printf("  |=============================== �����ٸ� ó�� �� ===============================|");
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

void RR(process* ps, int cnt) // ���� �κ� �����ٸ� �˰��� �Լ�
{
	int i = 0;

	Node* head = (Node*)malloc(sizeof(Node)); // �� ���� ����� �ּҸ� ����Ű�� head ��带 ����
	head->next = NULL; // head.next �� ���� NULL�� �ʱ�ȭ

	for (i = 0; i < cnt; i++)
	{
		if (ps[i].type == 0) //type == 0 �̸� ��带 �����ϰ� ����Ʈ�� �߰��Ѵ�
		{
			ps[i].arr_time = now_time; // �����ð��� ���� ���μ��� ���� �ð����� �����Ѵ�.
			process_count++; // ���μ����� ������ 1�� �����Ѵ�.
			putNode(head, makeNode(ps[i]));
		}
		else if (ps[i].type == 1) //type == 1 �̸� Round Robin �Լ��� 1ȸ ���� ��Ų��.
		{
			Node * deleting_one = outNode(head); // ����Ʈ���� ���� ����� �ּҸ� ��Ƶδ� ����

			if (deleting_one != NULL) // ����Ʈ���� �� ����� �ּҰ� NULL �ƴҰ���� ����
			{
				if (deleting_one->data.remain_time <= 0) // ����Ʈ���� �� ����� �����ִ� ����ð��� �Ϸ����� ���
				{
					print_ps(&(deleting_one->data)); // �Ϸ�� ���μ����� �����Ͱ��� ����ϴ� �Լ�
				}
				else
				{
					putNode(head, deleting_one); // ���μ����� �Ϸ���� �ʰ� ����ð��� ���� �ִٸ� ������ ����Ʈ�� �����Ѵ�
				}
			}
		}
		else if (ps[i].type == -1) //type == -1 �̸� �������� ����Ʈ �߰��� �Ϸ��ϰ� ����Ʈ�� ��尡 ���������� Round Robin�� �����Ѵ�.
		{
			while (1)
			{
				if (head->next == NULL) // ����Ʈ�� ��尡 ������ while �ݺ������� ������.
				{
					break;
				}
				Node * deleting_one = outNode(head);

				if (deleting_one != NULL) // ����Ʈ�� �����ִ� ��尡 ������ ����Ʈ���� ��尡 ���� ���������� Round Robin�� �����Ѵ�
				{
					if (deleting_one->data.remain_time <= 0) // �����층 �� ���μ����� ����ð��� ��� �Ϸ��� ��� ����Ѵ�
					{
						print_ps(&(deleting_one->data)); // �Ϸ�� ���μ����� �����Ͱ��� ����ϴ� �Լ�
						delNode(deleting_one); // ����� �������� ��� ���� �޸� �Ҵ� ����
					}
					else
					{
						putNode(head, deleting_one); // ���μ����� �Ϸ���� �ʰ� ����ð��� ���� �ִٸ� ������ ����Ʈ�� �����Ѵ�
					}
				}

			}
		}
	}
}

int inputArr(process* ps) //input.txt�� �ҷ��� ����ü �迭�� �Է��ϴ� �Լ�
{
	int i = 0, count = 0;

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

			ps[i].remain_time = ps[i].com_time; // �����ִ� ���� �ð� = �Էµ� ���� �ð����� �ʱ�ȭ

			if (ps[i].type == -1) // type = -1 �̶�� �迭�� 1�� �߰��ϰ� �Է��� �����Ѵ�.
			{
				count++;
				break;
			}
		count++;
		}
	}
	fclose(fp);

	return count; // �Էµ� �迭�� ������ ��ȯ
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
			printf("  |%5d|           |               | \n", ps[i].type);
		}
	}
}

void print_ps(process *ending_ps) // �����층�� �Ϸ�Ǹ� ����ϴ� �Լ�
{
	ending_ps->turn_time = now_time - (ending_ps->arr_time); // ��ȯ�ð� = ����ð� - �����ð�
	ending_ps->nomrtn_time = (double)ending_ps->turn_time / (double)ending_ps->com_time; // ����ȭ�� ��ȯ�ð� = ��ȯ�ð� / ����ð�
	normalized_turn_around_time_average += ending_ps->nomrtn_time;
	printf("  |%11d|%15d|%13d|%17d|%20.2lf|\n", ending_ps->id, ending_ps->com_time, ending_ps->arr_time, ending_ps->turn_time, ending_ps->nomrtn_time); // Process ID, ����ð�, ��ȯ�ð��� ����ϴ� �Լ�
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
	if ((head->next) == NULL) //head->next �� NULL�̸� ����Ʈ�� ��尡 ���� �����̴�
	{
		head->next = node;
	}
	else
	{
		Node* finding_lastone = head->next; // ������ ����� �ּҸ� �޾Ƴ��� ����ü ������ ����

		while (finding_lastone->next != NULL)
		{
			finding_lastone = finding_lastone->next;
			//finding_lastone->next�� NULL�� ������ ������ ����̹Ƿ� finding_lastone�� NULL�� ã�� ������ �ݺ��Ѵ�.
		}

		finding_lastone->next = node;
	}
}

Node* outNode(Node* head)  // ����Ʈ���� ��带 �����ϴ� �Լ�
{
	Node* address;

	address = head->next;

	if (head->next->data.remain_time <= QSIZE) // �����ִ� ����ð��� time_quatum ���� ������ ���μ����� �Ϸ�ȴ�.
	{
		now_time += head->next->data.remain_time; // ���μ����� �Ϸ�Ǹ� ���������� ������ ����ð���ŭ ��ü ���μ��� �ð��� �����Ѵ�.
	}
	else  now_time += QSIZE; // ���μ����� �Ϸ���� �ʰ� Round Robin�� �ѹ� �� ����Ǹ� time_quatum ��ŭ ��ü ���μ��� �ð��� �����Ѵ�.

	if (head->next == NULL)
	{
		return NULL;
	}
	else
	{
		head->next->data.remain_time = head->next->data.remain_time - QSIZE; // �� ó���� ����� ���� ����ð��� time_quatum ��ŭ ����.

		head->next = head->next->next; // head->next �� �� �տ��ִ� ��尡 �ƴ� �� ���� ���� ����� �ּҸ� ����Ų��
	}
	address->next = NULL;
	return address;
}

void delNode(Node *address) // �Է¹��� �ּҸ� ���� ��带 ���� �޸� �Ҵ��� �����ϴ� �Լ�
{
	free(address);
}