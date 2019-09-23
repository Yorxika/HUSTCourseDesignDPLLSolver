// ConsoleApplication1.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

double t;
int main()
{
	int option=1;
	char ch;
	while (option)
	{
		system("cls");
		printf("\n\n");
		printf("-------------基于SAT的数独游戏求解程序------------\n");
		printf("                      1.SAT\n");
		printf("                      2.SUDOKU\n");
		printf("    	              0. Exit\n");
		printf("-------------------------------------------------\n");
		printf("请选择你的操作:");
		scanf("%d", &option);
		switch (option)
		{
		case 1:
		{
			printf("请输入读取的CNF范例名称：");
			fflush(stdin);
			char cnfpath[300];   //用于存储cnf路径或名称
			scanf("%s", cnfpath);
			Line *S = creatClause(cnfpath);
			
			if (S == NULL)
			{
				printf("读取cnf范例失败请检查!\n");
				break;
			}
			else
			{
                Line *L = split(S, 0);//备份最原始状态的CNF范例，用来日后检查
				//fflush(stdin);
				getchar();
				printf("读取成功，是否打印cnf范例：Y/N\n");
				scanf("%c", &ch);
				if (ch == 'Y' || ch == 'y')
					printCNF(S);
				getchar();
				printf("是否解析公式：Y/N\n");
				scanf("%c", &ch);
				if (ch == 'Y' || ch == 'y')
					formulaanlysis(S);
				clock_t start, finish;
				start = clock();
				int dpll = DPLL(S, backtrack);
				finish = clock();
				t = (double)(finish - start) / CLOCKS_PER_SEC;
				//int check = checksoulution(value_, creatClause(cnfpath));
				if (dpll == 1/*&&check==1*/)
				{
					printf("该范例可满足,用时：%lf s\n", t);
					//fflush(stdin);
					getchar();
					printf("是否打印变元值：Y/N");
					scanf("%c", &ch);
					if (ch == 'Y' || ch == 'y')
						printValuation(S);
					getchar();
					printf("是否验证结果：Y/N");
					scanf("%c", &ch);
					if (ch == 'Y' || ch == 'y')
					{
						//printValuation(S);
						//printCNF(L);
						if (checksoulution(value_, L) == 1)
							printf("求解结果正确!\n");
						else
							printf("求解结果错误!\n");
					}
				}
				else if (dpll == -1)
				{
					printf("该范例不可满足，用时：%lf s\n", t);
				}
				else if (dpll == -2)
				{
					printf("该范例满足情况待定，用时：%lf s\n", t);
				}
				//fflush(stdin);
				getchar();
				printf("是否保存运行结果：Y/N");
				scanf("%c", &ch);
				if (ch == 'Y' || ch == 'y')
				{
					st(cnfpath);
					writeSolution(S, dpll, cnfpath);
				}

				free(S);
				free(L);  //还有几个数组没free
				freearray;
			}
			break;
		}
		case 2:
		{
			Sudoku* s = (Sudoku *)malloc(sizeof(Sudoku));
			//s = NULL;
			Line * L = NULL;
			starts();
			int i, j;
			for (i = 1; i < 10; i++)
				for (j = 1; j < 10; j++)
					s->s[i - 1][j - 1] = finalsudoku[i][j];
			//printSudoku(s);
			s = createSudoku(s);
			printf("数独生成成功!\n");
			printSudoku(s);
			// system("pause");
			clock_t start, finish;
			L = changeintoCNF(s);
			cnfparser(L);
			Line *S = split(L, 0);//备份最原始状态的CNF范例，用来日后检查
			if (L!=NULL)
			{
				getchar();
				printf("转化成功，是否打印cnf范例：Y/N\n");
				//fflush(stdin);
				scanf("%c", &ch);
				if (ch == 'Y' || ch == 'y')
					printCNF(L);
				getchar();
				printf("转化成功，是否保存cnf范例：Y/N\n");
				scanf("%c", &ch);
				if (ch == 'Y' || ch == 'y')
					writeresult(L);
				start = clock();
				int dpll = DPLL(L, backtrack);
				finish = clock();
				t = (double)(finish - start) / CLOCKS_PER_SEC;
				if (dpll == 1)
				{
					Sudoku* ss = (Sudoku *)malloc(sizeof(Sudoku));
					ss->next = NULL;
					ss = changeintoSudoku();
					printf("该数独成功解出!\n");
					getchar();
					printf("是否打印变元值：Y/N");
					// fflush(stdin);
					scanf("%c", &ch);
					if (ch == 'Y' || ch == 'y')
						printValuation(L);
					printf("用时: %lf s\n", t);
					getchar();
					printf("是否验证结果：Y/N");
					scanf("%c", &ch);
					if (ch == 'Y' || ch == 'y')
					{
						//printValuation(S);
						//printCNF(L);
						if (checksoulution(value_, S) == 1)
							printf("求解结果正确!\n");
						else
							printf("求解结果错误!\n");
					}
					getchar();
					printf("是否输入你求解出的数独结果：Y/N");
					// fflush(stdin);
					scanf("%c", &ch);
					if (ch == 'Y' || ch == 'y')
					{
						while (1)
						{
							printf("请输入你求解的数独(每个数字之间用空格隔开，行与行之间用换行隔开):\n");
							Sudoku* input = (Sudoku *)malloc(sizeof(Sudoku));
							input->next = NULL;
							input = inputsolution();
							//printSudoku(input);
							if (checkinput(ss, s) == 1)
							{
								printf("求解正确！");
									break;
							}
							else
							{
								getchar();
								printf("求解错误!是否继续求解：Y/N");
								scanf("%c", &ch);
								if (ch == 'N' || ch == 'n')
									break;
							}
							free(ss);
						}
					}
					getchar();
					 printf("是否打印结果：Y/N");
					 // fflush(stdin);
					 scanf("%c", &ch);
					 if (ch == 'Y' || ch == 'y')
					 {
						/* Sudoku* ss = (Sudoku *)malloc(sizeof(Sudoku));
						 ss->next = NULL;
						 ss= changeintoSudoku();**/
						 printSudoku(ss);
						 free(ss);
					 }
				}
				else if (dpll == -1)
				{
					printf("数独求解失败，用时：%lf s\n", t);
				}
				else if (dpll = -2)
				{
					printf("数独求解情况待定，用时：%lf s\n", t);
				}
				else
				{
					printf("转化CNF失败!\n");
				}
				//fflush(stdin);
				getchar();
				printf("是否保存运行结果：Y/N");
				scanf("%c", &ch);
				if (ch == 'Y' || ch == 'y')
				{
					char cnfpath[20] = "Sudoku.res";
					writeSolution(S, dpll, cnfpath);
				}
			}
			//system("pause");
			free(s);
			//free(L);  //为什么free L会触发断点 
			//free(ss); //还有数组没free
			freearray;
			break;
		}
		case 0:
			break;
		}
		system("pause");
	}
	return 0;
}
