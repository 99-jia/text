#include<iostream>
#include<string>
using namespace std;
#include<windows.h>
//进程控制块结构体
typedef struct node
{
	char name[10];//进程标识符
	int prio;  //进程的优先数
	int round;   //进程每次轮转的时间片数
	int cputime; //进程累计占用CPU的时间片
	int needtime; //进程到完成还需要的时间
	int count; //记录执行的次数   
	char state;  // 进程的状态，W——就绪态，R——执行态，F——完成态
	struct node* next; //链表指针  
}PCB;
//进程就绪态和等待态均为链表结构，其所需指针
PCB* run,* ready, * tail,* finish;//当前运行队列进程指针、就绪队列头指针、就绪队列尾指针、完成队列头指针
int N;
void FIRSTIN(void);	 //取得第一个就绪队列节点			
void PRINT1(char a);	//输出队列title信息		
void PRINT2(char chose, PCB* p);	//输出队列content信息
void PTINT(char chose);
void insert_prio(PCB* q);//创建优先级队列，规定优先数越小，优先级越高			
void prior_init(char chose);
void priority(char chose);
void insert_rr(PCB* q);
void roundrun_init(char chose);//创建时间片轮转调度队列
void roundrun(char chose);		


void main()//主函数
{
	char chose = ' ';
	while ((chose != 'e') && (chose != 'E'))
	{
		fflush(stdin);
		cout << "请选择算法：\nA：进程优先级算法\nB：循环轮转算法\n退出请按E\n";
		//cout << "请输入你的选择：";
		cin >> chose;
		if ((chose != 'e') && (chose != 'E'))
		{
			system("cls");
			if ((chose == 'a') || (chose == 'A'))
			{

				prior_init(chose);
				priority(chose);
				system("cls");
			}
			else if ((chose == 'b') || (chose == 'B'))
			{

				roundrun_init(chose);
				roundrun(chose);
				system("cls");
			}
		}
	}
	cout << "谢谢使用！\n";
}

void FIRSTIN(void) //调度就绪队列的第一个进程投入运行
{
	run = ready;//头指针入
	if (ready != NULL)//判断就绪队列是否为空
	{
		ready = ready->next;//不为空则更改状态为R
		run->state = 'R';
		run->next = NULL;
	}
	else//就绪状态为空则结束
	{
		run = NULL;
	}
}


void PRINT1(char a) //输出队列TITLE信息
{
	if (toupper(a) == 'A')
	{
		cout << "name  cputime  needtime priority state \n";
	}
	else
	{
		cout << "name  cputime  needtime count round state \n";
	}
}


void PRINT2(char chose, PCB* p)//输出队列CONTENT信息 
{
	if (toupper(chose) == 'A')
	{
		cout << p->name << "         " << p->cputime << "       " << p->needtime << "       " << p->prio << "       " << p->state << endl;
	}
	else
	{
		cout << p->name << "         " << p->cputime << "       " << p->needtime << "       " << p->count << "       " << p->round << "       " << p->state << endl;
	}
}
void PRINT(char chose) //输出所有信息
{
	PCB* p;
	PRINT1(chose);//输出title
	if (run != NULL)//判断事件类型，事件如果不是运行状态
	{
		PRINT2(chose, run);//就输出结束队列
	}
	p = ready;//获取就绪队列
	while (p != NULL)//运行状态时，只要就绪队列不为空
	{
		PRINT2(chose, p);//输出就绪队列信息
		p = p->next;//指向下一指针
	}
	p = finish;//就绪队列为空时，即事件结束
	while (p != NULL)//输出结束队列
	{
		PRINT2(chose, p);
		p = p->next;
	}
}

void insert_prio(PCB* s)//创建优先级队列，规定优先数越小，优先级越低
{
	PCB* p, * r;
	p = ready;
	r = p;
	if (s->prio > ready->prio)//将当前指针和就绪队列头指针比较优先级
	{
		s->next = ready;//如果当前指针优先级大就插到最前头
		ready = s;
	}
	else//如果当前指针优先级没头指针大
	{
		while (p)
		{
			if (p->prio >= s->prio)//就找一个合适的位置给他插进去
			{
				r = p;
				p = p->next;
			}
			else
				break;
		} 
		s->next = p;
		r->next = s;//没找到的话指针递给给下一个
	}
}
void prior_init(char chose)//优先级输入函数
{
	PCB* p;
	int i, time;
	char na[10];
	ready = NULL;
	finish = NULL;
	run = NULL;
	cout << "输入进程的个数 N:\n";
	cin >> N;
	for (i = 0; i < N; i++)
	{
		p = (PCB*)malloc(sizeof(PCB));//返回指向被分配内存的指针
		cout << "输入第" << i + 1 << "个进程名\n";
		cin >> na;
		cout << "完成进程需要的时间片数\n";
		cin >> time;
		strcpy_s(p->name, na);//复制名字给指针
		//对输出数据进行输入赋值
		p->cputime = 0;//CPU的执行时间
		p->needtime = time;//进程执行所需要的时间
		p->state = 'W';
		p->prio = 50 - time; //设置其优先级，需要的时间越多，优先级越低 
		if (ready == NULL)//判断队列是否为空，如果队列为空，当前指针为头指针
		{
			ready = p;
			ready->next = NULL;
		}
		else
		{
			insert_prio(p);//队列不为空，就按优先级合理插入
		}
		cout << "当前就绪队列的进程的信息\n";
		//PRINT1( chose);	//输出队列title信息		
		//PRINT2( chose,run);
		PRINT(chose);//打印信息
	}
	cout << i + 1 << "个进程已按优先级从高到低进到就绪队列中\n\n";
	cout << "开始模拟优先级算法.....\n";
	fflush(stdin);
	getchar();
	FIRSTIN();
}
void priority(char chose)//按照优先级调度，每次执行一个时间片
{
	int i = 1;
	while (run != NULL)//取到头指针时进入执行队列
	{
		run->cputime++;//每执行一次，CPU时间片加1
		run->needtime--;//进程执行完成的剩余时间减1
		run->prio -= 1;//每执行一次,优先级减1
		if (run->needtime == 0)//所需执行时间为0时，则执行队列结束，转换状态为F
		{
			run->next = finish;
			finish = run;
			run->state = 'F';
			run->prio = 0;
			run = NULL;
			FIRSTIN();//继续取就绪队列队头进程进入执行队列
		}
		else//将进程状态置为W，入就绪队列
		{
			if ((ready != NULL) && (run->prio < ready->prio))
			{
				run->state = 'W';
				insert_prio(run);//准备就绪队列
				run = NULL;
				FIRSTIN();//继续取就绪队列队头进程进入执行队列
			}
		}
		PRINT(chose);//输出结果
	}
	getchar();
}


//循环轮转调度算法
void insert_rr(PCB* q)//时间片轮转插入函数
{
	tail->next = q;
	tail = q;
	q->next = NULL;
}
void roundrun_init(char chose)//时间片输入函数
{
	PCB* p;
	int i, time;
	char na[10];
	ready = NULL;
	finish = NULL;
	run = NULL;
	cout << "\t\t循环轮转算法模拟全过程\n\n";
	cout << "输入进程的个数N:\n";
	cin >> N;
	for (i = 0; i < N; i++)
	{
		p = (PCB*)malloc(sizeof(PCB));//返回指向被分配内存的指针
		cout << "输入第" << i + 1 << "个进程名\n";
		cin >> na;
		cout << "完成进程需要的时间片数\n";
		cin >> time;
		strcpy_s(p->name, na);
		//对输出结果进行初始设置
		p->cputime = 0;
		p->needtime = time;
		p->count = 0;
		p->state = 'W';
		p->round = 2;
		if (ready != NULL)//如果头指针不为空，就插入到队尾
		{
			insert_rr(p);
		}
		else//如果头指针为空，就当前指针为第一个
		{
			p->next = ready;
			ready = p;
			tail = p;
		}
		cout << "当前就绪队列的进程的信息\n";
		PRINT(chose);
	}
	cout << i + 1 << "个进程已按FIFO进到就绪队列中\n\n";
	cout << "开始模循环轮转算法.....\n";
	fflush(stdin);
	getchar();
	run = ready;//取就绪队列头指针
	ready = ready->next;
	run->state = 'R';//状态为R

}
void roundrun(char chose)//时间片轮转调度算法
{
	int i = 1;
	while (run != NULL)//取到头指针进入执行队列
	{//处理输出结果
		run->cputime++;
		run->needtime--;
		run->count++;
		if (run->needtime == 0)//进程执行完毕
		{
			run->next = finish;
			finish = run;
			run->state = 'F';//转换状态为F
			run->prio = 0;//优先级一样
			run = NULL;
			if (ready != NULL)//如果就绪状态不为空，获取头指针
			{
				FIRSTIN();
			}
		}
		else
		{
			if (run->count == run->round)//时间片用完时
			{
				run->count = 0; //计数器清零，为下次做准备
				if (ready != NULL)//如果就绪队列还有数值，将状态设置为W，等待下次调度
				{
					run->state = 'W';
					insert_rr(run);//按位置插入
					FIRSTIN();
				}
			}
		}
		PRINT(chose);//打印结果
	}
	getchar();
}