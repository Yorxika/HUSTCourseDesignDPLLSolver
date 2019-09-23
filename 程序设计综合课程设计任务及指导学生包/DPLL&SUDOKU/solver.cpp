#include "pch.h"

#include <stdio.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

int * value_;
int lag = 0;

void sorttimes(int *variabletimes_, int *truevaluefirst_) //对传入的正变元出现次数进行排序
{
	int i, j, max = -1, c = 0;
	for (j = 1; j < variablenumbers + 1; j++)
	{
		for (i = 1; i < variablenumbers + 1; i++)
		{
			if (variabletimes_[i] > max)
			{
				max = variabletimes_[i];
				c = i;
			}
		}
		truevaluefirst_[j] = c;
		variabletimes_[c] = 0;
		max = -1;
	}
}

void copyarray(int *variabletime, int *newtime)
{
	int i;
	for (i = 1; i < variablenumbers + 1; i++)
		newtime[i] = variabletime[i];
	//return newtime;
}

int isunitClause(Line *S)
{
	//判断是否为单子句,并且返回变元标号,没有就返回0
	Line * L = S;
	while (L != NULL)
	{
		if (L->firstvariable == NULL)    //空子句
		{
			continue;
		}
		if (L->firstvariable->next == NULL)
		{
			return L->firstvariable->name;
		}
		L = L->nextline;
	}
	return 0;
}

int  unittransmit(Line *S, int *newbacktrack)   //单子句传播规则
{
	int unitliteral = isunitClause(S);
	if (unitliteral == 0) //没有单子句
		return 0;
	//printf("单子句：%d \n", unitliteral);
	newbacktrack[abs(unitliteral)] = 1;
	value_[abs(unitliteral)] = unitliteral > 0 ? 1 : 0;//为这个变元赋值使其为真
//接下来通过循环遍历cnf范例，删除单子句以及删除其负文字
	Line *L = S, *q = NULL;
	while (L != NULL)
	{
		Clause *p1 = L->firstvariable;
		Clause *p2 = NULL;
		while (p1 != NULL)
		{
			if (p1->name == unitliteral)//找到单子句文字并且调整指针
			{
				if (L == S)//如果是第一行要改变第一行的结构
				{
					if (S->nextline == NULL)
					{
						//*S = *((Line *)NULL);
						removevariable(L->firstvariable);//释放单子句
						free(L);
						S = NULL;
						L = NULL;
					}
					else
					{
						removevariable(L->firstvariable);//释放单子句
						Line * q = L->nextline;
						*S = *(S->nextline);
						free(q);
						// S=S->nextline;
						//free(L);
						L = NULL;
					}
				}
				else
				{
					q->nextline = L->nextline;
					removevariable(L->firstvariable);//释放单子句
					free(L);
					L = q;
				}
				break;
			}
			else if (p1->name == -unitliteral)   //找到负文字，将其从子句中移除，其他文字保留
			{
				if (p1 == L->firstvariable)
				{
					L->firstvariable = p1->next;
					free(p1); //释放单子句内存
				}
				else
				{
					p2->next = p1->next;
					free(p1); //释放单子句内存
				}
				p1 = p2;
				continue;
			}
			p2 = p1;
			p1 = p1->next;
		}
		q = L;
		L = L == NULL ? S : L->nextline;
	}
	return 1;
}

int boole(int variable)  //用来判断变元正负
{
	if (variable > 0)
		return 1;
	else return -1;
}

int find(Line  *S)//找到纯文字
{
	//int *literal = (int*)alloca((variablenumbers + 1)* sizeof(int));//整形数组标志纯文字并且初始化为0
	int * literal = (int*)calloc(variablenumbers + 1, sizeof(int));
	/*	int i = 0;
	for (i = 0; i < variablenumbers + 1; i++)
		literal[i] = 0;*/
	if (literal == NULL)
	{
		return  -2;
	}
	Line *L = S;
	while (L != NULL)
	{
		Clause * c = L->firstvariable;
		while (c != NULL)
		{
			int f = literal[abs(c->name)];//用来记录找到的纯文字
			if (f == 0) 
				literal[abs(c->name)] = boole(c->name);
			else if (f == -1 && (c->name) > 0)//此时有多个文字且正负不一致
			{
				literal[abs(c->name)] = 2;
				//	printf("%d ", literal[abs(c->name)]);
			}
			else if (f == 1 && (c->name) < 0)
			{
				literal[abs(c->name)] = 2;
				//	printf("%d ", literal[abs(c->name)]);
			}
			c = c->next;
		}
		L = L->nextline;
	}
	int i;
	for (i = 1; i < variablenumbers + 1; i++) //遍历找到第一个纯文字
	{
		if (literal[i] == -1 || literal[i] == 1)
		{
			int j = literal[i];
				free(literal);
			return i * j;
		}
	}
	return 0;
}

int deleteclause(Line  *S, int *newbacktrack) //纯文字删除
{
	int pliteral = find(S);
	if (pliteral == 0)
		return 0;
	//printf("纯文字：%d \n", pliteral);
	newbacktrack[abs(pliteral)] = 1;
	value_[abs(pliteral)] = pliteral > 0 ? 1 : 0;
	Line * L = S;
	Line * q=NULL;
	while (L != NULL)  //删除包含纯文字的子句
	{
		Clause * c = L->firstvariable;
		while (c != NULL)
		{
			if (c->name == pliteral)
			{
				if (L == S)
				{
					if (S->nextline == NULL)  //这时候只有一行
					{
						//*S =*((Line *)NULL);
					    removevariable(L->firstvariable); //释放纯文字内存
						free(L);
						S = NULL;
						L = NULL;
					}
					else
					{
                        removevariable(L->firstvariable);//释放纯文字内存
						Line *q1 = L->nextline;
						*S = *(S->nextline);
						// S->firstline=L->nextline;
						free(q1);
						L = NULL;
					}
				}
				else
				{
					q->nextline = L->nextline;
					removevariable(L->firstvariable);//释放纯文字内存
					free(L);
					L = q;
				}
				break;
			}
			c = c->next;
		}
		q = L;
		L = L == NULL ? S : L->nextline;
	}
	return 1;
}

int Allunit(Line  *S) //检查是否所有都是单子句
{
	int * literal = (int*)calloc(variablenumbers + 1, sizeof(int));
	//int * literal = (int *)alloca((variablenumbers + 1)* sizeof(int));
		/*int i = 0;
		for (i = 0; i < variablenumbers + 1; i++)
			literal[i] = 0;*/
	if (literal == NULL)
		return -2;
	Line * L = S;
	while (L != NULL)
	{
		Clause * c = L->firstvariable;
		while (c != NULL)
		{
			int j = literal[abs(c->name)];
			if (j == 0) 
				literal[abs(c->name)] = boole(c->name);
			//如果之前找到过这个变元
			else if (j == -1 && (c->name) > 0)
			{
					free(literal);
				return 0;
			}
			else if (j == 1 && (c->name) < 0)
			{
							free(literal);
				return 0;
			}
			c = c->next;
		}
		L = L->nextline;
	}
	//此时没有冲突变元，我们可以决定各个变元的值
	L = S;
	while (L != NULL)
	{
		Clause *c = L->firstvariable;
		while (c != NULL)
		{
			value_[abs(c->name)] = c->name > 0 ? 1 : 0;
			c = c->next;
		}
		L = L->nextline;
	}
	return 1;
}

int isEmptyClause(Line  *S)  //是否有空子句
{
	Line * L = S;
	while (L != NULL)
	{
		if (L->firstvariable == NULL)
			return 1;
		L = L->nextline;
	}
	return 0;
}

int check(Line *S)  //检查是否可满足，不可满足返回-1待定返回0；
{
	if (isEmptyClause(S))//有空子句
		return -1;
	if (Allunit(S) && checkchangevariable(value_) != 0)
		return 1;
	return 0;
}

int checkchangevariable(int *value_)  //确保所有变元都设置了值
{
	int i = 1;
	for (i = 1; i < variablenumbers + 1; i++)
	{
		if (value_[i] == -1)
			return  0;
	}
	return 1;
}

int checksoulution(int *value, Line * S)
{
	int flag = 0;
	//int i = 0;
	Line  *L = S;
	while (L != NULL)
	{
		flag = 0;  //开始检验当前行
		Clause *c = L->firstvariable;
		while (c != NULL)
		{
			if (c->name == 0)  //在复制的时候增加了第一行为0的情况需要跳过
			{
				c=c->next;
				flag = 1;
				continue;
			}
			if (value[abs(c->name)] + boole(c->name) == 2 || value[abs(c->name)] + boole(c->name) == -1) //一行只要有一个结果为真即可
			{
              flag = 1;
			  //i++;
			 // printf("%d %d \n", c->name,i);
              break;
			}
			/*if (value[abs(c->name)] + boole(c->name) == 1 || value[abs(c->name)] + boole(c->name) == 0)  //结果错误
				flag = 0;*/
			c = c->next;
		}
		if (flag == 0)
			break;
		L = L->nextline;
	}
	if (flag == 1)
		return 1;
	else return 0;
}

int choose(Line * S)
{
	return S->firstvariable->name;
}

int occurtimesmax(Line *S, int * variabletimes_, int *backtrack_)
{
	int i = 1;
	for (i = 1; i < variablenumbers + 1; i++)
	{
		//printf("%d true[%d]=%d back[%d]=%d \n", i, i,variabletimes_[i], variabletimes_[i], backtrack_[variabletimes_[i]]);
		if (backtrack_[variabletimes_[i]] != 1) //找到出现次数最大的并且没选择过的变元
		{
			backtrack_[variabletimes_[i]] = 1;
			if (negativetimes[variabletimes_[i]] > positivetimes[variabletimes_[i]]) //即出现负次数较多
				return 0 - variabletimes_[i];
			else
				return variabletimes_[i];
		}
	}
	return S->firstvariable->name;
}

Line * split(Line * S, int lname)  //执行分裂策略

{
	value_[abs(lname)] = lname > 0 ? 1 : 0;
	//printf("此次选取的变元是：%d\n", lname);
	Line * newLine = NULL;
	Line * p1 = S;
	Line  * p2 = NULL;
	while (p1 != NULL)
	{
		Line *copyCL = copyClause(p1);
		if (copyCL == NULL)
		{
			lag = -2;
			return NULL;
		}
		if (newLine == NULL)
		{
			newLine = copyCL;
		}
		if (p2 != NULL)
		{
			p2->nextline = copyCL;
		}
		p2 = copyCL;
		p1 = p1->nextline;
	}
	Line *q = (Line *)malloc(sizeof(Line));
	if (q == NULL)
	{
		lag = -2;
		return NULL;
	}
	q->firstvariable = NULL;
	q->nextline = NULL;
	Clause *c = (Clause *)malloc(sizeof(Clause));
	if (c == NULL)
	{
		lag = -2;
		return NULL;
	}
	c->name = lname;
	c->next = NULL;
	q->firstvariable = c;
	q->nextline = newLine;
	//S->firstline=q;
		// *(S)=*(q);
	return q;
}

void removevariable(Clause * c)  //删除当前行
{
	while (c != NULL)
	{
		Clause * next = c->next;
		free(c);
		c = next;
	}
}

void removeClause(Line * S)  //删除整个cnf范式
{
	//    Line *L=S;
	while (S != NULL)
	{
		Line * nextline = S->nextline;
		if (S->firstvariable != NULL) 
			removevariable(S->firstvariable);
		free(S);
		S = nextline;
	}
}

int DPLL(Line *S, int *back)
{
	if (S == NULL)
		return -2;
	if (lag == -2)
		return -2;
	int k = check(S);
	if (k != 0)//检查是否已经处于可满足状态
	{
		removeClause(S);
		return k;
	}
	while (1)//单子句传播
	{
		k = check(S);
		if (k != 0)
		{
			removeClause(S);
			return k;
		}
		k = unittransmit(S, back);
		if (k == 0) //没有单子句
			break;
		if (k == -2)  //此时内存分配失败
			return -2;
	}
	while (1) //纯文字消除
	{
		int k = check(S);
		if (k != 0)
		{
			removeClause(S);
			return k;
		}
		k = deleteclause(S, back);
		if (k == 0)
			break;
		if (k == -2)
			return -2;
	}
	// int key = choose(S);  //选取变元策略此时为了方便先行选取第一个变元

	 //此为策略二

	int key = occurtimesmax(S, maxtimes, back);  //选取出现次数最多的变元
	//printf("选取的变元为%d\n",key);
	back[abs(key)] = 1;
	int * newbacktrack = (int *)calloc((variablenumbers + 1), sizeof(int));
	if (newbacktrack == NULL)
		return -2;
	copyarray(back, newbacktrack);
	//printfarray(back);
	//printfarray(newbacktrack);
	int status = DPLL(split(S, key), newbacktrack);
	if (status == 1)
	{
		free(newbacktrack);
		return 1;
	}
	else if (status == -2)
		return -2;
	else
		/*{
			printfarray(newbacktrack);
			return DPLL(split(S, 0 - key), newbacktrack);//=失败回溯之后没有对当前选取的变元赋0故而回溯层次增多
		}*/
	{
		int k = DPLL(split(S, 0 - key), newbacktrack);
		if (k != 1)
		{
			back[abs(key)] = 0;
			free(newbacktrack);
			return k;
		}
		else
		{
			free(newbacktrack);
            return 1;
		}
			
	}

}

void printValuation(Line *S)
{
	int i;
	for (i = 1; i < variablenumbers + 1; i++)
	{
		printf("%d=%d ", i, value_[i]);
		if (i % 10 == 0)
			printf("\n");
	}
	printf("\n");
}
