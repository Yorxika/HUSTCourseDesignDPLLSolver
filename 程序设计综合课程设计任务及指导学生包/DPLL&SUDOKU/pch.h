#ifndef PCH_H
#define PCH_H
#ifndef HEAD_H_INCLUDED
#define HEAD_H_INCLUDED


#define FALSE 0
#define TRUE 1
#define ERROR 0
#define SUCCESS 1
typedef struct Clause {  //采用邻接表作为子句结构
	int name;
	struct Clause *next;
}Clause;

typedef struct Line {  //存储行子句
   //int linename;
	struct Clause *firstvariable;
	struct Line *nextline;
}Line;

/*typedef struct CNF
{
	int *vectors;  //记录变元值，0为假，1为真
	int *variabletimes;  //记录变元出现次数
	int variablenumbers, Clausenumbers;  //记录变量数和子句数
	struct Line *firstline;
}CNF;*/

typedef struct Sudoku {
	int s[9][9];
	struct Sudoku *next;
}Sudoku;

extern double t; //计时
extern int lag;//标记当前cnf范式求解状态
extern int * value_;  //记录变元值
extern int *occurtimes;  //记录变元出现的次数
//extern int *inittimes; //保存最开始的出现次数
extern int *maxtimes;  //按变元最大出现次数排序
extern int *backtrack; //记录变元的选取情况
extern int *positivetimes;  //记录变元出现的正状态次数
extern int *negativetimes;  //记录变元出现的负状态次数
extern int * multiplytimes;  //记录变元正负状态乘积次数
extern int variablenumbers, Clausenumbers;  //记录变元数和行数

void printfarray(int *array); //打印数组
Line * copyClause(Line *L);  //复制当前子句
Line * creatClause(char * cnfpath);  //读取CNF范例并创建对应的数据结构
void printCNF(Line * S);  //打印CNF范例
int isunitClause(Line *S); //判断是否有单子句
int find(Line *S);//找到纯文字
//int  unittransmit(Line *S) ;
int  unittransmit(Line *S, int *newbacktrack); //单子句规则
int deleteclause(Line *S, int *newbacktrack);//纯文字规则
int Allunit(Line *S); //检查是否所有子句都是单子句
int isEmptyClause(Line *S); //检查是否有空子句
int check(Line *S);  //检查当前范例情况是否可解
int choose(Line *S);  //选择变元策略
Line *split(Line *S, int lname);  //分裂策略
void removevariable(Clause* p);  //删除当前行子句
void removeClause(Line* S);  //删除cnf范式
int DPLL(Line *S, int *back);  //DPLL算法
void printValuation(Line *S);  //打印值
void writeSolution(Line  * S, int dpll, char *cnfpath);  //保存运行结果
int boole(int variable);  //用来判断变元正负
int checkchangevariable(int *value_); //检查是否所有变元都赋值了
int checksoulution(int *value, Line * S); //求解结果验证
int occurtimesmax(Line *S, int * variabletimes_, int *backtrack_);  //选择正负变元乘积数最大的
void sorttimes(int *variabletimes_, int *truevaluefirst_);  //将乘积数最大的变元按从大到小的顺序排列
void copyarray(int *variabletime, int *newtime);  //复制数组
char *st(char *str);  //更改后缀名用来输出
void formulaanlysis(Line *S); //公式解析
void  freearray();  //释放掉数组内存

int addLiteral(Line *L, int literal);  //增加变元
Line * addClause(Line *L, int n, int *clause);  //增加子句
Line * createCNF(Line *L, int variablenumber, int Clausenumber);  //创建空的cnf范式（用于数独中）
int LOC(int x, int y, int z);  //（x,y)=z  的点对应的一维数组位置
void printSudoku(Sudoku *s);  //打印数独
Sudoku* createSudoku(Sudoku *s); //挖洞生成数独
Sudoku *inputsolution(); //输入数独的解
Sudoku* createSudoku(Sudoku *s);//产生数独
int checkSudoku(Sudoku *s);  //检查生成的数独是否合法
int checkkeyword(Sudoku *s, int x, int y); //检验填入的数字是否正确
int startsolveSudoku(Sudoku *s, int *count); //解数独
int solveSudoku(Sudoku *s, int x, int y, int *solvecount); //回溯法求解数独的解
Line * changeintoCNF(Sudoku *s);  //转换成cnf范式
void cnfparser(Line *L);  //对cnf范式的解析
void writeresult(Line *L); //保存运行结果
Sudoku * changeintoSudoku();  //对解出的cnf范式转换成数独
int  checkinput(Sudoku *s, Sudoku *ss); //检查输入的数独
int createfinalsudoku(int i, int j); //生成数独终盘
void starts();  //开始生成数独终盘
extern int finalsudoku[10][10];  //数独终盘
#endif // HEAD_H_INCLUDED


// TODO: 添加要在此处预编译的标头

#endif //PCH_H
