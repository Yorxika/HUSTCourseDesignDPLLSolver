// pch.cpp: 与预编译标头对应的源文件；编译成功所必需的

#include "pch.h"

// 一般情况下，忽略此文件，但如果你使用的是预编译标头，请保留它。

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
int variablenumbers, Clausenumbers;
 int *occurtimes;
 int *backtrack;
//int *inittimes;
int *positivetimes;  //记录变元出现的正状态次数
 int *negativetimes;
 int *maxtimes;
 int * multiplytimes;

Line* creatClause(char * cnfpath)  //读取CNF范例并创建对应的数据结构
{
	FILE *fp;
	fp = fopen(cnfpath, "r");
	if (fp == NULL)
	{
		printf("打开路径出错!\n");
		system("pause");
		return NULL;
	}
	char x; //用于存储当前读取内容
	Clause *p, *p1;
	Line *q=NULL, *q1;
	Line * cnf = (Line *)malloc(sizeof(Line));  //将CNF头节点去掉改成首行指针
	cnf->firstvariable = NULL;
	cnf->nextline = NULL;
	while ((x = fgetc(fp)) == 'c')
		while ((x = fgetc(fp)) != '\n')
			;  //跳过注释部分
	int i = 0, j, k;
	for (i = 0; i < 5; i++) {  //读取CNF范例直到变元数以及子句数目
		x = fgetc(fp);
	}
	fscanf(fp, "%d", &variablenumbers);
	fscanf(fp, "%d", &Clausenumbers);
	//cnf=(Line *)malloc(sizeof(Line));
	//cnf->vectors=(int *)malloc(sizeof(int)*(cnf->variablenumbers+1));
	value_ = (int*)calloc((variablenumbers)+1, sizeof(int));
	occurtimes = (int *)calloc(variablenumbers + 1, sizeof(int));
	positivetimes = (int *)calloc(variablenumbers + 1, sizeof(int));
	negativetimes = (int *)calloc(variablenumbers + 1, sizeof(int));
	backtrack = (int *)calloc(variablenumbers + 1, sizeof(int));
	//inittimes=(int *)calloc(variablenumbers+1,sizeof(int));
	multiplytimes = (int *)calloc(variablenumbers + 1, sizeof(int));
	maxtimes = (int *)calloc(variablenumbers + 1, sizeof(int));
   // memset(cnf->variabletimes,0,sizeof(int)*(cnf->variablenumbers+1));
  // memset(value_,-1,sizeof(int)*(cnf->variablenumbers+1));
 //  value_=(int *)malloc(sizeof(int)*cnf->variablenumbers);
	for (i = 0; i < (variablenumbers + 1); i++)
	{
		value_[i] = -1;
		//backtrack[i]=-1;//用-1来标志当前变元未被选择，1标志着选择过，0标志着当前变元选择策略失败需要回溯
	}
	for (i = 0; i < Clausenumbers; i++)//记录子句数据
	{
		for (j = 0;; j++)
		{
			p = (Clause *)malloc(sizeof(Clause));
			p->next = NULL;
			fscanf(fp, "%d", &k);
			if (k == 0)  //结束读取当前行数据
				break;
			p->name = k;
			p->next = NULL;
			if (k > 0)
			{
				positivetimes[abs(k)]++;
				//inittimes[abs(k)]++;
				occurtimes[abs(k)]++;
			}
			else
			{
				//inittimes[abs(k)]++;
				occurtimes[abs(k) ]++;
				negativetimes[abs(k)]++;
			}
			if (i == 0) { //第一行
				if (cnf->firstvariable == NULL)
					cnf->firstvariable = p;
				else
				{
					p1 = cnf->firstvariable;
					while (p1->next != NULL)
						p1 = p1->next;
					p1->next = p;
				}
			}
			else if (q->firstvariable == NULL)
				q->firstvariable = p;
			else
			{
				p1 = q->firstvariable;
				while (p1->next != NULL)
					p1 = p1->next;
				p1->next = p;
			}
		}
		q = (Line *)malloc(sizeof(Line));
		q->firstvariable = NULL;
		q->nextline = NULL;
		q1 = cnf;
		while (q1->nextline != NULL)
			q1 = q1->nextline;
		q1->nextline = q;
		if (i == Clausenumbers - 1)
		{
			q1->nextline = NULL;
			free(q);
		}
	}
	for (i = 1; i < variablenumbers + 1; i++)
		multiplytimes[i] = positivetimes[i] * negativetimes[i];
	/*for (i = 1; i < variablenumbers + 1; i++)
	{
		//printf("vtimes[%d]=%d  max[%d]=%d \n", i, occurtimes[i], i, maxtimes[i]);
		printf("vtimes[%d]=%d  occurtimes[%d]=%d \n", i, multiplytimes[i], i, occurtimes[i]);
		printf("正次数=%d 负次数=%d\n", positivetimes[i], negativetimes[i]);
	}*/
	//sorttimes(occurtimes, maxtimes);
	sorttimes(multiplytimes, maxtimes);
	/*for (i = 1; i < variablenumbers + 1; i++)
	{
		printf("vtimes[%d]=%d  max[%d]=%d \n", i, occurtimes[i], i,maxtimes[i]);
	}*/

	free(occurtimes);// 之前free掉会出现问题现在不会了
	free(multiplytimes);
	free(positivetimes);
	free(negativetimes);
	fclose(fp);
	// printCNF(cnf);
	return cnf;
}

int addLiteral(Line *L, int literal)//增加变元
{
	Clause *p = (Clause*)malloc(sizeof(Clause));
	if (p == NULL)
	{
		return -2;
	}
	p->name = literal;
	p->next = NULL;
	if (L->firstvariable == NULL)
		L->firstvariable = p;
	else
	{
		p->next = L->firstvariable;
		L->firstvariable = p;
	}
	return 1;
}

Line* addClause(Line *L, int n, int *clause)//增加子句
{
	int i = 0;
	Line *newClause = (Line *)malloc(sizeof(Line));
	if (newClause == NULL || n <= 0) {
		return NULL;
	}
	newClause->firstvariable = NULL;
	newClause->nextline = NULL;
	for (i = 0; i < n; i++)
	{
		addLiteral(newClause, clause[i]);
		/*if (==NULL)
		{
			printCNF(newClause);
			return NULL;
		}*/
	}
	if (L->firstvariable == NULL)  //对应第一次进来的情况
		return newClause;
	else
	{
		newClause->nextline = L;
		return newClause;
	}

	//*L = *newClause;
	//L = newClause;

}

/*int removeClause(Line *L,CNF *S)   //按照单子句规则删除子句
{
  int  unitclausename=L->firstvariable->name;
  Line *p=S->firstline,*q;
  Clause *clause;
  while(p!=NULL)
  {
	  clause=p->firstvariable;
	  while(clause!=NULL)
	  {
		  if(clause->name==unitclausename&&clause->name>0)
			{
				if(p->linename==0){
					  *S->firstline=*S->firstline->nextline;
					  break;
				}
				  else{
					q->nextline=p->nextline;
				  }
			}
			else clause=clause->next;
	  }
	  q=p;
	  p=p->nextline;
  }
  return SUCCESS;
}

 int removeClausevariable(CNF *S,Line *L)  //删除子句中的文字，前提是L为一个单子句
 {
	 int unitclausename=L->firstvariable->name;
	 int val=L->firstvariable->value;
	 if(val==0)
		val=1;
	 else val=0;
	 Line *p=S->firstline,*p1;
	 Clause *clause,*q;
	 while(p!=NULL)
	 {
		 clause=p->firstvariable;
		 while(clause!=NULL)
		 {
			 if(clause->name==unitclausename&&clause->value==val)
			 {
				 if(p->firstvariable==clause)
					p->firstvariable=clause->next;
				 else
				 q->next=clause->next;
			 }
			 q=clause;
			 clause=clause->next;
		 }
		 p1=p;
		 p=p->nextline;
	 }
 }*/




int haveemptyClause(Line *S)
{
	Line * L = S;
	while (L != NULL)
	{
		if (L->firstvariable == NULL)
			return TRUE;
		else L = L->nextline;
	}
	return FALSE;
}

/*void deleteClause(int vname,int val,Line *S)
{
  Line *p=S,*q;
  Clause *clause;
  while(p!=NULL)
  {
	  clause=p->firstvariable;
	  while(clause!=NULL)
	  {
		  if(clause->name==vname)
			{
				if(p->linename==0){
					  *S->firstline=*S->firstline->nextline;
					  break;
				}
				  else{
					q->nextline=p->nextline;
				  }
			}
			else clause=clause->next;
	  }
	  q=p;
	  p=p->nextline;
  }
}*/

/*int occurtimesmax(Line *S)
{
	int max=0,i,j;
	for(i=0;i<=(S->variablenumbers);i++)
	{
		if(max<S->variabletimes[i])
		{
			 max=S->variabletimes[i];
			 j=i;
		}
	}
	S->variabletimes[j]=0;//
	return j;
}

int addClause(CNF *S,int addvariable,int value)
{
	int i;
	Line *p=S->firstline;
	Line *newClause=(Line *)malloc(sizeof(Line));
	newClause->firstvariable=(Clause *)malloc(sizeof(Clause));
	newClause->nextline=p;
	S->firstline=newClause;
	newClause->firstvariable->name=addvariable;
	newClause->firstvariable->next=NULL;
	//newClause->firstvariable->result=value;
	//newClause->firstvariable->value=value;
	return 1;
}

CNF* copycnf(CNF *S)
{
	CNF *newcnf=(CNF *)malloc(sizeof(CNF));
	newcnf->Clausenumbers=S->Clausenumbers;
	newcnf->variablenumbers=S->variablenumbers;
	newcnf->vectors=(int *)malloc(sizeof(int)*(newcnf->variablenumbers+1));
	Line *q=(Line *)malloc(sizeof(Line)),*q1=S->firstline,*q2;
	q->firstvariable=NULL;
	Clause *p,*p1,*p2;
	newcnf->firstline=q;
	while(q1!=NULL)
	{
		p=q1->firstvariable;
		p1=(Clause *)malloc(sizeof(Clause));
		q->firstvariable=p1;
		q2=q;
		q=(Line *)malloc(sizeof(Line));
		while(p!=NULL)
		{
			p2=p1;
			p1->name=p->name;
			p1->next=NULL;
			p=p->next;
			if(p->next==NULL)
				break;
			p1=(Clause *)malloc(sizeof(Clause));
			p2->next=p1;
		}
		q1=q1->nextline;
		q2->nextline=q;
	}
	q2->nextline=NULL;
	int i;
	for(i=1;i<newcnf->variablenumbers+1;i++)
		newcnf->vectors[i]=S->vectors[i];
	return newcnf;
}

*/

Line * copyClause(Line *L)
{
	Line * newClause;
	newClause = (Line *)malloc(sizeof(Line));
	if (newClause == NULL)
	{
		lag = -2;
return NULL;
	}
	newClause->firstvariable = NULL;
	newClause->nextline = NULL;
	Clause *p1 = L->firstvariable;
	Clause *p2 = NULL;
	while (p1 != NULL)
	{
		Clause *copyCl = (Clause *)malloc(sizeof(Clause));
		if (copyCl == NULL)
		{
			lag = -2;
			return NULL;
		}
		copyCl->name = p1->name;
		copyCl->next = NULL;
		if (newClause->firstvariable == NULL)
			newClause->firstvariable = copyCl;
		if (p2 != NULL)
			p2->next = copyCl;
		p2 = copyCl;
		p1 = p1->next;
	}
	return newClause;
}


void printCNF(Line * S) 
{
	Line* L = S;
	while (L != NULL) 
	{
		Clause *p = L->firstvariable;
		while (p != NULL)
		{
			printf("%d ", p->name);
			p = p->next;
		}
		printf("\n");
		L = L->nextline;
	}
}



void formulaanlysis(Line *S)  //解析公式
{
	Line *L = S;
	while (L != NULL)
	{
		Clause *c = L->firstvariable;
		while (c != NULL)
		{
			if ((c->name) > 0)
				printf("%d∨", c->name);
			if ((c->name) < 0)
				printf("!%d∨",abs(c->name));
			if (c->next == NULL)
				printf("\b \n");
			c = c->next;
		}
		L = L->nextline;
	}
}

void printfarray(int *array)
{
	int i = 0;
	for (int i = 1; i < variablenumbers + 1;i++)
		printf("array[%d]=%d \n", i, array[i]);
}

void  freearray()
{
	free(value_);
	free(backtrack);
	free(maxtimes);
}

void writeSolution(Line  * S, int dpll, char *cnfpath)
{
	FILE *fp;
	fp = fopen(cnfpath, "w+");
	if (fp == NULL)
	{
		printf("打开文件失败\n");
		exit(1);
	}
	//	int i;
	if (dpll == FALSE)
	{
		fprintf(fp, "s 0 \n");
		fprintf(fp, "t %lf ms", t * 1000);
	}
	else if (dpll == TRUE)
	{
		fprintf(fp, "s 1 \n");
		int i = 0;
		fprintf(fp, "v ");
		for (i = 1; i < variablenumbers + 1; i++)
		{
			if (value_[i] == 0)
				fprintf(fp, "%d ", 0 - i);
			else
				fprintf(fp, "%d ", i);
		}

		fprintf(fp, "\nt %lf ms", t * 1000);
	}
	else
	{
		fprintf(fp, "s -1\n");
		fprintf(fp, "t %lf ms", t * 1000);
	}
	printf("文件保存成功！\n");
	fclose(fp);
}

char *st(char *str)
{
	int i = 0;
	for (i = 0; str[i] != '.'; i++);
	str[i++] = '.';
	str[i++] = 'r';
	str[i++] = 'e';
	str[i++] = 's';
	return str;
}