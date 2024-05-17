#include<iostream>
#include<string>
using namespace std;
#define MAX_SIZE 32767
typedef struct node {
    int id;
    int adr;//分区首地址
    int size;//分区大小
    struct node* next;//指向下一个分区的指针
}Node;//定义分区描述器
//定义指向node结构的指针变量，空闲队列的首指针，指向释放区的
Node* head1, * head2, * back1, * back2, * assign;
int request;
int check(int add, int siz, char c)//用于检查指定的释放块（由用户键入）的合法性
{
    Node* p, * head;
    int check = 1;
    if (add < 0 || siz < 0)
        check = 0;/*地址和大小不能为负*/
    if (c == 'a' || c == 'A')
        head = head1;
    else
        head = head2;
    p = head->next;
    while ((p != NULL) && check)
        if (((add < p->adr) && (add + siz > p->adr)) || ((add >= p->adr) && (add < p->adr + p->size)))
            check = 0;
        else
            p = p->next;
    if (check == 0)
        cout << "\t输入释放区地址或大小有错误！！！";
    return check;
}
void init()
{
    Node* p;
    head1 = (Node*)malloc(sizeof(Node));// 初始化运行进程链表头结点
    head2 = (Node*)malloc(sizeof(Node));
    p = (Node*)malloc(sizeof(Node));
    head1->next = p;
    head2->next = p;
    p->size = MAX_SIZE;
    p->adr = 0;
    p->next = NULL;
    p->id = 0;
}
Node* assignment1(int num, int req)//实现First Fit Algorithm
{
    Node* before, * after, * ass;
    ass = (Node*)malloc(sizeof(Node));
    before = head1;
    after = head1->next;
    ass->id = num;
    ass->size = req;
    while (after->size < req)
    {
        before = before->next;
        after = after->next;
    }
    if (after == NULL)
    {
        ass->adr = -1;
    }
    else
    {
        if (after->size == req)
        {
            before->next = after->next;
            ass->adr = after->adr;
        }
        else
        {
            after->size -= req;
            ass->adr = after->adr;
            after->adr += req;
        }
    }
    return ass;
}
void acceptment1(int address, int siz, int rd)//实现First Fit Algorithm的回收算法
{
    Node* before, * after;
    int insert = 0;
    back1 = (Node*)malloc(sizeof(Node));
    before = head1;
    after = head1->next;
    back1->adr = address;
    back1->size = siz;
    back1->id = rd;
    back1->next = NULL;
    while (!insert && after)
    {//将要被回收的分区插入空闲区（按首址大小从小到大插入）
        if ((after == NULL) || ((back1->adr <= after->adr) && (back1->adr >= before->adr))) {
            before->next = back1;
            back1->next = after;
            insert = 1;
        }
        else
        {
            before = before->next;
            after = after->next;
        }
    }
    if (insert)
    {
        if (back1->adr == before->adr + before->size)
        { //和前边分区合并
            before->size += back1->size;
            before->next = back1->next;
            free(back1);
        }
        else if (after && back1->adr + back1->size == after->adr)
        {//和后边分区合并
            back1->size += after->size;
            back1->next = after->next;
            back1->id = after->id;
            free(after);
            after = back1;
        }
        cout << "\t最先适应算法回收内存成功！";
    }
    else
        cout << "\t最先适应算法回收内存失败！";
}
Node* assignment2(int num, int req)//实现Best Fit Algorithm
{
    Node* before, * after, * ass, * q;
    ass = (Node*)malloc(sizeof(Node));
    q = (Node*)malloc(sizeof(Node));
    before = head2;
    after = head2->next;
    ass->id = num;
    ass->size = req;
    while (after->size < req)
    {
        before = before->next;
        after = after->next;
    }
    if (after == NULL)
    {
        ass->adr = -1;
    }
    else
    {
        if (after->size == req)
        {
            before->next = after->next;
            ass->adr = after->adr;
        }
        else
        {
            q = after;
            before->next = after->next;
            ass->adr = q->adr;
            q->size -= req;
            q->adr += req;
            before = head2;
            after = head2->next;
            if (after == NULL)
            {
                before->next = q;
                q->next = NULL;
            }
            else
            {
                while ((after->size) < (q->size))
                {
                    before = before->next;
                    after = after->next;
                }
                before->next = q;
                q->next = after;
            }
        }
    }
    return (ass);
}
void acceptment2(int address, int siz, int rd) //实现Best Fit Algorithm的回收算法
{
    Node* before, * after;
    int insert = 0;
    back2 = (Node*)malloc(sizeof(Node));
    before = head2;
    after = head2->next;
    back2->adr = address;
    back2->size = siz;
    back2->id = rd;
    back2->next = NULL;
    if (head2->next == NULL)
    {//空闲队列为空
        head2->next = back2;
        head2->size = back2->size;
    }
    else
    {//空闲队列不为空
        while (after)
        {
            if (back2->adr == after->adr + after->size)
            {//和前边空闲分区合并
                before->next = after->next;
                after->size += back2->size;
                back2 = after;
            }
            else
            {
                before = before->next;
                after = after->next;
            }
        }
        before = head2;
        after = head2->next;
        while (after)
        {
            if (after->adr == back2->adr + back2->size)
            {//和后边空闲区合并
                before->next = after->next;
                back2->size += after->size;
            }
            else
            {
                before = before->next;
                after = after->next;
            }
        }
        before = head2;
        after = head2->next;
        while (!insert)
        {//将被回收的块插入到恰当的位置（按分区大小从小到大）
            if (after == NULL || ((after->size > back2->size) && (before->size < back2->size))) {
                before->next = back2;
                back2->next = after;
                insert = 1;
                break;
            }
            else
            {
                before = before->next;
                after = after->next;
            }
        }
    }
    if (insert)
        cout << "\t最佳适应算法回收内存成功！";
    else
        cout << "\t最佳适应算法回收内存失败！！";
}
void print(char choice)//输出空闲区队列信息,打印空闲区队列
{
    Node* p;
    if (choice == 'a' || choice == 'A')
        p = head1->next;
    else
        p = head2->next;
    if (p)
    {
        cout << "\n空闲区队列的情况为：" << endl;
        cout << "\t编号\t首址\t终址\t大小" << endl;
        while (p)
        {
            cout << "\t" << p->id << "      " << p->adr << "      " << p->adr + p->size - 1 << "      " << p->size << endl;
            p = p->next;
        }
    }
}
void menu()//菜单及主要过程
{
    char chose;
    int ch, r, add, rd;
    int num = 0;
    while (1)
    {
        system("cls");
        cout << "选择最先适应算法请输入A，选择最佳适应算法请输入B，退出程序请输入E\n\n";
        cout << "请输入序号:";
        cin >> chose;
        if (chose == 'e' || chose == 'E')
            exit(0);
        else
        {
            system("cls");
            while (1)
            {
                if (chose == 'a' || chose == 'A')
                    cout << "最先适应算法(First-Fit)模拟:\n";
                if (chose == 'b' || chose == 'B')
                    cout << "最佳适应算法(Best-Fit)模拟:\n";
                cout << "1.分配内存,2.回收内存,3.查看内存,4.返回\n\n";
                cout << "请输入序号:";
                cin >> ch;
                fflush(stdin);
                switch (ch)
                {
                case 1:
                    cout << "输入申请分区的大小：" << endl;
                    cin >> r;
                    if (chose == 'a' || chose == 'A')
                        assign = assignment1(num, r);
                    else
                        assign = assignment2(num, r);
                    if (assign->adr == -1)
                    {
                        cout << "空间不足，分配内存失败！\n";
                    }
                    else
                        cout << "分配成功！分配的内存的首址为:" << assign->adr << endl;
                    break;
                case 2:
                    cout << "输入释放的内存的首址：" << endl;
                    cin >> add;
                    cout << "输入释放的内存的大小：" << endl;
                    cin >> r;
                    cout << "输入释放的内存的编号：" << endl;
                    cin >> rd;
                    if (check(add, r, chose))
                    {
                        if (chose == 'a' || chose == 'A')
                            acceptment1(add, r, rd);
                        else
                            acceptment2(add, r, rd);
                    }
                    break;
                case 3:
                    print(chose);
                    break;
                case 4:
                    menu();
                    break;
                }
            }
        }
    }
}
void main()//主函数 
{
    init();
    menu();
}