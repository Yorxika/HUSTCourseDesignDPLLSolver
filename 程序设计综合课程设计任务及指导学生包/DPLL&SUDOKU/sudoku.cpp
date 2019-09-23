#include "pch.h"

#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include "time.h"
#include <cstring>
#include<algorithm>
using namespace std;

int finalsudoku[10][10];

int LOC(int x, int y, int z)  //三维数组寻址公式对应着9*9*9个CNF变元
{
	int k = x * 81 + y * 9 + z;
	return k;
}

Line * createCNF(Line *L, int variablenumber, int Clausenumber)//先创建一个什么都没有的CNF范例
{
	int *v = &variablenumbers;//修改变元数目以及子句数目
	*v = variablenumber;
	v = &Clausenumbers;
	*v = Clausenumber;
	L = (Line *)malloc(sizeof(Line));
	L->firstvariable = NULL;
	L->nextline = NULL;
	return L;
}

Line * changeintoCNF(Sudoku *s)//转换成CNF范式
{
	int i, j, k, l, c, m;
	int *v = &Clausenumbers;
	*v = 0;
	Line *L = (Line *)malloc(sizeof(Line));
	L->firstvariable = NULL;
	L->nextline = NULL;
	int b[9], b2[9], b3[9], b4[9];
	if ((L = createCNF(L, 9 * 9 * 9, 0)) == NULL)
	{
		return NULL;
	}
	for (i = 0; i < 9; i++)//检查每个格子的情况
	{
		for (j = 0; j < 9; j++)
		{
			if (s->s[i][j] != 0)//如果这个格子有数字
			{
				for (k = 1; k < 10; k++) //生成一个单子句
				{
					if (k == s->s[i][j])
					{
						b[0] = LOC(i, j, k);
					}
					else
					{
						b[0] = 0-LOC(i, j, k);
					}
					L = addClause(L, 1, b);
					(*v)++;
				}
				for (k = j + 1; k < 9; k++)//同一行填的数字不能相等
				{
					if (s->s[i][k] == 0)
					{
						b[0] = 0-LOC(i, j, s->s[i][j]);
						b[1] = 0-LOC(i, k, s->s[i][j]);
						L = addClause(L, 2, b);
						(*v)++;
						//*L = *(addClause(L, 2, b));
					}
				}
				for (k = i + 1; k < 9; k++)//同一列的数字也不能相等
				{
					if (s->s[k][j] == 0) {
						b[0] = 0-LOC(i, j, s->s[i][j]);
						b[1] = 0-LOC(k, j, s->s[i][j]);
						L = addClause(L, 2, b);
						(*v)++;
						//L=addClause(L, 2, b);
					}
				}
				for (k = i + 1; k < i / 3 * 3 + 3; k++) //3*3九宫格也不能相等
				{
					for (l = j / 3 * 3; l < j / 3 * 3 + 3; l++)
					{
						if (l == j) //同一竖列已经比较过了
						{
							continue;
						}
						if (s->s[k][l] == 0)
						{
							b[0] = 0-LOC(i, j, s->s[i][j]);
							b[1] = 0-LOC(k, l, s->s[i][j]);
							L = addClause(L, 2, b);
							(*v)++;
							//L=addClause(L, 2, b);
						}
					}
				}
			}
			else //此时这个格子待输入数字
			{
				for (k = 1, c = 0; k <= 9; k++)
				{
					s->s[i][j] = k;
					if (checkkeyword(s, i, j))//找到可能的取值和可能的位置
					{
						b[c] = k;
						b2[c] = LOC(i, j, k);
						c++;
					}
				}
				s->s[i][j] = 0;
				L = addClause(L, c, b2);//对于这条子句，有且只能有1个变元取真
				for (k = 0; k < c - 1; k++)//则其他变元只能取假
				{
					for (l = k + 1; l < c; l++)
					{
						b2[0] = 0-LOC(i, j, b[k]);
						b2[1] = 0-LOC(i, j, b[l]);
						L = (addClause(L, 2, b2));
						(*v)++;
					}
				}
				for (k = j + 1; k < 9; k++)
				{
					if (s->s[i][k] != 0)//同一行其他位置有数字，则不能相等
					{
						b2[0] =0-LOC(i, j, s->s[i][k]);
						b2[1] =0-LOC(i, k, s->s[i][k]);
						L = (addClause(L, 2, b2));
						(*v)++;
					}
					else//为空则不能填一样
					{
						for (l = 0; l < c; l++)
						{
							b2[0] = 0 - LOC(i, j, b[l]);
							b2[1] = 0 - LOC(i, k, b[l]);
							L = (addClause(L, 2, b2));
							(*v)++;
						}
					}
				}
				for (k = i + 1; k < 9; k++)//竖列检查
				{
					if (s->s[k][j] != 0)//空格处与已填写的数字不能相等
					{
						b2[0] = 0 - LOC(i, j, s->s[k][j]);
						b2[1] = 0 - LOC(k, j, s->s[k][j]);
						L = (addClause(L, 2, b2));
						(*v)++;
					}

					else //若都是空格处数字不能相同
					{
						for (l = 0; l < c; l++)
						{
							b2[0] = 0 - LOC(i, j, b[l]);
							b2[1] = 0 - LOC(k, j, b[l]);
							L = (addClause(L, 2, b2));
							(*v)++;
						}
					}
				}

				for (k = i + 1; k < i / 3 * 3 + 3; k++)//3*3九宫格检查,由于当前行列都检查过了，可以跳过当前行当前列
				{
					for (l = j / 3 * 3; l < j / 3 * 3 + 3; l++)
					{
						if (l == j)
						{
							continue;
						}
						if (s->s[k][l] != 0)//空格处与已填写的数字不能相等
						{
							b2[0] = 0 - LOC(i, j, s->s[k][l]);
							b2[1] = 0 - LOC(k, j, s->s[k][l]);
							L = (addClause(L, 2, b2));
							(*v)++;
						}

						else//若都是空格处数字不能相同
						{
							for (m = 0; m < c; m++) {
								b2[0] = 0 - LOC(i, j, b[m]);
								b2[1] = 0 - LOC(k, l, b[m]);
								L = (addClause(L, 2, b2));
								(*v)++;
							}
						}
					}
				}
			}
		}
	}
	for (i = 0; i < 9; i++)//每行必须填1-9
	{
		memset(b, 0, sizeof(int) * 9);
		for (j = 0, l = 0; j < 9; j++)//找到未填写的数字以及位置
		{
			if (s->s[i][j] != 0)
			{
				b[s->s[i][j] - 1] = 1;
			}
			else
			{
				b2[l] = j;
				l++;
			}
		}
		//printf("l=%d\n", l);
		for (j = 0, k = 0; j < 9; j++)
		{
			if (b[j] == 0)  //未填写的数字
			{
				b[k] = j + 1;
				k++;
			}
		}
		for (j = 0; j < k; j++)  //将未填写的数字填充到空位
		{
			for (c = 0; c < l; c++)
			{
				b3[c] = LOC(i, b2[c], b[j]);
			}
			L = addClause(L, l, b3);
			(*v)++;
		}
	}
	for (i = 0; i < 9; i++)//每列必须填1-9
	{
		memset(b, 0, sizeof(int) * 9);
		for (j = 0, l = 0; j < 9; j++)
		{
			if (s->s[j][i] != 0)//找到未填写的数字以及位置
			{
				b[s->s[j][i] - 1] = 1;
			}
			else
			{
				b2[l] = i;
				l++;
			}
		}
		for (j = 0, k = 0; j < 9; j++)
		{
			if (b[j] == 0)
			{
				b[k] = j + 1;
				//k++;
			}
		}
		for (j = 0; j < k; j++)
		{
			for (c = 0; c < l; c++)
			{
				b3[c] = LOC(i, b2[c], b[j]);
			}
			L = addClause(L, l, b3);
			(*v)++;
		}
	}
	for (i = 0; i < 9; i += 3) //九宫格必须填1~9
	{
		for (m = 0; m < 9; m += 3)
		{
			memset(b, 0, sizeof(int) * 9);
			for (j = 0, l = 0; j < 9; j++)
			{
				if (s->s[i + j / 3][m + j % 3] != 0) //横行除列取余
				{
					b[s->s[i + j / 3][m + j % 3] - 1] = 1;//存放未填的数字
				}
				else
				{
					b2[l] = (m + j % 3);//存放未填的位置
					b4[l] = (i + j / 3);
					l++;
				}
			}
			for (j = 0, k = 0; j < 9; j++)
			{
				if (b[j] == 0)
				{
					b[k] = j + 1;
					k++;
				}
			}
			for (j = 0; j < k; j++)
			{
				for (c = 0; c < l; c++)
				{
					b3[c] = LOC(b4[c], b2[c], b[j]);
				}
				L = addClause(L, l, b3);
				(*v)++;
			}
		}
	}
	//printCNF(L);
	return L;
}

void cnfparser(Line *L)
{
	Line *S = L;
	int *v = &variablenumbers;
	*v = 729;
	value_ = (int*)calloc((variablenumbers)+1, sizeof(int));
	occurtimes = (int *)calloc(variablenumbers + 1, sizeof(int));
	positivetimes = (int *)calloc(variablenumbers + 1, sizeof(int));
	negativetimes = (int *)calloc(variablenumbers + 1, sizeof(int));
	backtrack = (int *)calloc(variablenumbers + 1, sizeof(int));
	//inittimes = (int *)calloc(variablenumbers + 1, sizeof(int));
	maxtimes = (int *)calloc(variablenumbers + 1, sizeof(int));
	multiplytimes = (int *)calloc(variablenumbers + 1, sizeof(int));
	while (S != NULL)
	{
		Clause *c = S->firstvariable;
		while (c != NULL)
		{
			if (c->name > 0)
			{
				positivetimes[abs(c->name)]++;
				//inittimes[abs(c->name)] ++;
				occurtimes[abs(c->name)]++;
			}
			else
			{
				//inittimes[abs(c->name) ]++;
				occurtimes[abs(c->name)]++;
				negativetimes[abs(c->name)]++;
			}
			//printf("%d||", c->name);
			c = c->next;
		}
		//printf("\n");
		S = S->nextline;
	}
	int i;
	for (i = 1; i < variablenumbers + 1; i++)
		multiplytimes[i] = positivetimes[i] * negativetimes[i];
	/*for (i = 1; i < variablenumbers + 1; i++)
   {
	   printf("vtimes[%d]=%d  true[%d]=%d \n", i, variabletimes[i], i, truevaluefirst[i]);
   }*/
	sorttimes(multiplytimes, maxtimes);
	//sorttimes(occurtimes, maxtimes);
	/*for (i = 1; i < variablenumbers + 1; i++)
	{
		printf("vtimes[%d]=%d  true[%d]=%d \n", i, variabletimes[i], i, truevaluefirst[i]);
	}*/
	free(occurtimes);// 之前free掉会出现问题现在不会了
	free(multiplytimes);
	free(positivetimes);
	free(negativetimes);
}

void starts()
{
	srand(unsigned(time(NULL)));  /* 产生random_shuffle的随机数种子 */
	for (int i = 1; i <= 9; ++i)
		for (int j = 1; j <= 9; ++j)
			finalsudoku[i][j] = 0;

	for (int i = 1; i <= 9; ++i)
		finalsudoku[1][i] = i;

	random_shuffle(&(finalsudoku[1][1]), &(finalsudoku[1][10]));  /* 第一行随机排列产生 */

	createfinalsudoku(2, 1);  /* 从第二行开始搜索 */
}

int createfinalsudoku(int i, int j)  //搜索第（ i , j ）位置处可以存储的数字,找到解则返回true，否则返回false
{
	if (i > 9 || j > 9)
		return TRUE;

	for (int k = 1; k <= 9; ++k)
	{
		int can = TRUE;                // can 变量用于记录数字k能否放在 ( i , j ) 处
		for (int m = 1; m < i; ++m)
			if (finalsudoku[m][j] == k)  // 检查同一列是否出现过数字k
			{
				can = FALSE;
				break;
			}
		if (can)
			for (int n = 1; n < j; ++n)
				if (finalsudoku[i][n] == k)  // 检查同一行是否出现过数字k
				{
					can = FALSE;
					break;
				}
		if (can)
		{
			int up1 = (i / 3) * 3 + 3; // (i,j)方格所在的3×3小方格i坐标的上限
			int up2 = (j / 3) * 3 + 3;   // (i,j)方格所在的3×3小方格在j坐标的上限

			if (i % 3 == 0)    //这是针对特殊情况的处理
				up1 = i;
			if (j % 3 == 0)
				up2 = j;

			for (int p = up1 - 2; p <= up1; ++p)  //检查在3×3的小方格中是否出现了同一个数字 
			{
				if (can == FALSE)   
					break;
				for (int q = up2 - 2; q <= up2; ++q)
					if (finalsudoku[p][q] == k)
					{
						can = FALSE;
						break;
					}
			}
		}
		if (can)
		{
			finalsudoku[i][j] = k;
			if (j < 9)
			{
				if (createfinalsudoku(i, j + 1)) //到同一行的下一位置开始搜索 
					return TRUE;
			}
			else
			{
				if (i < 9)
				{
					if (createfinalsudoku(i + 1, 1))    //到下一行的第一个空格开始搜索 
						return TRUE;
				}
				else
					return TRUE;  //i >= 9  && j >= 9  , 搜索结束

			}
			finalsudoku[i][j] = 0;   //找不到解就要回复原状，否则会对下面的搜索造成影响 
		}
	}
	return FALSE;  //1到9都尝试过都不行，则返回递归的上一步 
}

Sudoku* createSudoku(Sudoku *s)//挖洞产生数独
{
	Sudoku *ss = NULL;
	s->next = NULL;
	int i, j, k, c, flag = 0, x, y;
	x = rand() % 9;
	y = rand() % 9;
	s->s[x][y] = 0;//先挖掉1个洞
	while (1)  //挖洞过程
	{
		startsolveSudoku(s, &c);//求解该数独，得到解的个数c
		if (c > 1)//此时解不唯一
		{
			s->s[x][y] = k;
			flag++; //将出口设置在这里，意味着挖洞的数目远大于17，提示数至少有17个，即有唯一解
			if (flag >= 17)
			{
				break;
			}
			for (ss = s->next; ss != NULL; ss = s->next)// 后续生成的都没什么用了
			{
				s->next = ss->next;
				free(ss);
			}
		}
		else
		{
			do {
				x = rand() % 9;
				y = rand() % 9;
			} while (s->s[x][y] == 0);
			k = s->s[x][y]; //此时(x,y)不为0，记录下这个值
			s->s[x][y] = 0;
			/*flag++;
			if (flag >=50)  //此时挖了60个洞，可以退出了
			{
				break;
			}*/
		}
	}
	return s;
}

int startsolveSudoku(Sudoku *s, int *count)  //开始求解数独
{
	*count = 0;
	solveSudoku(s, 0, 0, count);
	if (count > 0)
		return TRUE;
	else
		return FALSE;
}

int solveSudoku(Sudoku *s, int x, int y, int *count) //求解数独
{
	int i, j, k;
	Sudoku *ss;
	while (s->s[x][y] != 0 && x < 9)//从上往下找找到第一个为0的空
	{
		y++;
		if (y == 9) //搜索完一行
		{
			x++;
			y = 0;
		}
	}
	if (x > 8)  //这时数独棋盘没有空的
	{
		ss = (Sudoku*)malloc(sizeof(Sudoku));
		for (j = 0; j < 9; j++) {
			for (k = 0; k < 9; k++) {
				ss->s[j][k] = s->s[j][k];
			}
		}
		ss->next = s->next;
		s->next = ss;
		(*count)++;
		return TRUE;
	}
	for (i = 1; i <= 9; i++)
	{
		s->s[x][y] = i;
		if ((k = checkkeyword(s, x, y)) == 1)
		{
			if (x == 8 && y == 8)//达到最后一个格子
			{
				ss = (Sudoku*)malloc(sizeof(Sudoku));
				for (j = 0; j < 9; j++)
				{
					for (k = 0; k < 9; k++)
					{
						ss->s[j][k] = s->s[j][k];
					}
				}
				ss->next = s->next;
				s->next = ss;
				s->s[x][y] = 0;
				(*count)++;
				return TRUE;
			}
			else
			{
				if (y == 8)  //求解下一行
					solveSudoku(s, x + 1, 0, count);
				else  //求解下一列
					solveSudoku(s, x, y + 1, count);
			}
		}
	}
	s->s[x][y] = 0;
	return FALSE;
}

void printSudoku(Sudoku *s) //打印数独
{
	int i, j;
	//printf("此为生成的数独:\n");
	printf("-------------------\n");
	for (i = 0; i < 9; i++) {
		printf("|");
		for (j = 0; j < 9; j++) {
			printf("%d%c", s->s[i][j], ((j + 1) % 3 == 0) ? '|' : ' ');
		}
		printf("\n");
		if ((i + 1) % 3 == 0) {
			printf("|");
			printf("------------------\n");
		}
	}
}

int  checkSudoku(Sudoku *s)  //检查生成的数独是否合法
{
	int i, j, k, l;
	int flag = 0;
	char b[9];
	for (i = 0; i < 9; i++)  //横行检查
	{
		memset(b, 0, sizeof(int) * 9);
		for (j = 0; j < 10; j++)
		{
			if (s->s[i][j] == 0)  //如果有为0的情况
			{
				flag = 1;
			}
			else if (b[s->s[i][j] - 1] == 0)  //如果当前空格填的数字只出现了一次无冲突
			{
				b[s->s[i][j] - 1] = 1;
			}
			else
			{
				return FALSE;
			}
		}
	}
	for (i = 0; i < 9; i++)// 列检查
	{
		memset(b, 0, sizeof(int) * 9);
		for (j = 0; j < 9; j++)
		{
			if (s->s[j][i] == 0) {
				flag = 1;
			}
			else if (b[s->s[j][i] - 1] == 0)
			{
				b[s->s[j][i] - 1] = 1;
			}
			else {
				return FALSE;
			}
		}
	}
	for (i = 0; i < 9; i += 3) //3*3检查,从第一个九宫格开始往右检查
	{
		for (j = 0; j < 9; j += 3)
		{
			memset(b, 0, sizeof(int) * 9);
			for (k = i; k < i + 3; k++)
			{
				for (l = j; l < j + 3; l++)
				{
					if (s->s[k][l] == 0)
					{
						flag = 1;
					}
					else if (b[s->s[k][l]] == 0)
					{
						b[s->s[k][l]] = 1;
					}
					else
					{
						return FALSE;
					}
				}
			}
		}
	}
	if (flag == 1)
		return 0;
	else return 1;
}

Sudoku *inputsolution() //输入数独的解
{
	int i, j;
	char ch;
	Sudoku *s = (Sudoku*)malloc(sizeof(Sudoku));
	if (s == NULL)
		return NULL;
	s->next = NULL;
	for (i = 0; i < 9; i++)
	{
		for (j = 0; j < 9; j++)
		{
			scanf("%d", &s->s[i][j]);
		}
	}
	return s;
}

Sudoku * changeintoSudoku()
{
	int i,j,k;
	Sudoku *s =(Sudoku *) malloc(sizeof(Sudoku));
	s->next = NULL;
	for(i=0;i<9;i++)
	for (j=0;j<9;j++)
	{
		for (k = 1; k < 10; k++)
		{
			/*printf("[%d][%d][%d]=%d ", i, j, k, LOC(i, j, k));
			if (k == 9)
				printf("\n");*/
			if (value_[LOC(i, j, k)] == 1)
				s->s[i][j] = k;
		}
	}
	return s;
}

int checkkeyword(Sudoku *s, int x, int y) //检验填入的数字是否正确
{
	int i, j;
	for (i = 0; i < 9; i++)  //检查横行是否冲突
	{
		if (s->s[x][y] == s->s[i][y] && i != x)
		{
			return FALSE;
		}

	}
	for (i = 0; i < 9; i++)  //检查竖列是否冲突
	{
		if (s->s[x][y] == s->s[x][i] && i != y)
		{
			return FALSE;
		}

	}
	for (i = x / 3 * 3; i < x / 3 * 3 + 3; i++)//检查3*3九宫格是否冲突
	{
		for (j = y / 3 * 3; j < y / 3 * 3 + 3; j++)
		{
			if (s->s[x][y] == s->s[i][j] && (i != x || j != y))
			{
				return FALSE;
			}

		}
	}
	return TRUE;
}

void writeresult(Line *L)
{
	FILE *fp;
	fp = fopen("Sudoku.cnf", "w+");
	if (fp == NULL)
	{
		printf("打开文件失败\n");
		exit(1);
	}
	fprintf(fp, "p cnf %d %d\n", variablenumbers, Clausenumbers);
	Line *S = L;
	while(S!=NULL)
	{ 
		Clause *c = S->firstvariable;
		while (c != NULL)
		{
			fprintf(fp,"%d ", c->name);
			c = c->next;
		}
		fprintf(fp,"0\n");
		S = S->nextline;
	}
	fclose(fp);
}

int  checkinput(Sudoku *s, Sudoku *ss)
{
	int flag = 1,i,j;
	for(i=0;i<9;i++)
		for (j = 0; j < 9; j++)
		{
			if (s->s[i][j] == ss->s[i][j])
				continue;
			if (s->s[i][j] != ss->s[i][j])
			{
				flag = 0;
				break;
			}
		}
	if (flag == 0)
		return 0;
	else
		return 1;
}








