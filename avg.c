#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "math.h"

#define MAXLEN	1024

typedef struct Table{
	double	id;
	int	num;
	struct Table *next;
} Table;
typedef Table	*TablePtr;

Table *div_list(Table *head)
{
	if(NULL == head)
	{
		return head;
	}

	Table *tmp = head;
	Table *slow = head, *fast = head;
	while(fast)
	{
		fast = fast->next;
		if(fast)
		{
			fast = fast->next;
		}

		if(NULL == fast)
		{
			break;
		}
		slow = slow->next;
	}

	tmp = slow;
	slow = slow->next;
	tmp->next = NULL;

	return slow;
}

Table* merge(Table *head1, Table *head2)
{
	if(NULL == head1) return head2;
	if(NULL == head2) return head1;

	Table head;
	Table *tail = &head;

	while(head1 && head2)
	{
		if(head1->num < head2->num)
		{
			tail->next = head1;
			head1 = head1->next;
		}
		else
		{
			tail->next = head2;
			head2 = head2->next;
		}
		tail = tail->next;
	}

	if(head1)
	{
		tail->next = head1;
	}
	if(head2)
	{
		tail->next = head2;
	}

	return head.next;
}

Table *
merge_sort(Table *head)
{
	if(NULL == head || NULL == head->next)
	{
		return head;
	}

	Table *head1 = head;
	Table *head2 = div_list(head);

	head1 = merge_sort(head1);
	head2 = merge_sort(head2);
	return merge(head1, head2);
}

void main()
{
	char	*filename = "input.csv";
	char	buffer[MAXLEN] = {'\0'};
	double	result = 0;
	FILE	*fp = NULL;
	int		arg1 = 0;
	int		arg2 = 0;
	int		i = 1;
	int		ncount = 0;
	int		dcount = 0;
	int		*count;
	double	*avg;
	TablePtr	tphead = NULL;
	TablePtr	tptail = NULL;
	TablePtr	tppre = NULL;

	fp = fopen(filename, "r");
	if(!fp)
		perror("error open file");
	
	tphead = (TablePtr)malloc(sizeof(Table));
	tphead->next = NULL;
	tptail = tphead;

	/* read */
	while(fgets(buffer, MAXLEN, fp))
	{
		TablePtr	tmpcell = NULL;
		char	*tmp1 = strtok(buffer, ",");
		char	*tmp2 = buffer+strlen(tmp1)+1;

		arg1 = (double)atoi(tmp1);
		arg2 = atoi(tmp2);

		tmpcell = (TablePtr)malloc(sizeof(Table));
		tmpcell->next = NULL;
		tptail->id = arg1;
		tptail->num = arg2;
		tptail->next = tmpcell;
		tppre = tptail;
		tptail = tmpcell;
		ncount++;
	}
	tptail = tppre;
	tptail->next = NULL;

	/* sort */
	tphead = merge_sort(tphead);

	tptail = tphead;
	count = (int *)malloc(ncount * sizeof(int));
	avg = (double *)malloc(ncount * sizeof(double));
	memset(count, 0 , ncount * sizeof(int));
	memset(avg, 0 , ncount * sizeof(double));

	/* compute */
	while(tptail)
	{
		count[dcount] = tptail->num;
		avg[dcount] += (tptail->id - avg[dcount]) / i;
		if(tptail->next &&
			(tptail->num != tptail->next->num))
		{
			dcount++;
			i = 1;
		}
		else
			i++;

		tptail = tptail->next;
	}

	/* printf */
	for (i = 0; i <= dcount; i++)
		printf("avg:%lf, num:%d\n", avg[i], count[i]);
}
