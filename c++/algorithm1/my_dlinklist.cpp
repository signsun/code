#include <iostream>

using namespace std;

template <typename T>
class myDLinkList
{
private:
    // 虚拟头尾节点
    struct Node
    {
        T val;
        Node *next;
        Node *prev;
        Node(T value)
        {
            this->val = value;
            this->next = nullptr;
            this->prev = nullptr;
        }
    };
    // 找到对应节点
    Node *findNode(int index)
    {
        Node *temp = head;
        for (int i = 0; i < index; i++)
        {
            temp = temp->next;
        }
        return temp;
    }

    int size;
    Node *head;
    Node *tail;

public:
    myDLinkList()
    {
        head = new Node(T());
        tail = new Node(T());
        head->next = tail;
        tail->prev = head;
        size = 0;
    }

    ~myDLinkList()
    {
        while (size > 0)
        {
            removeHead();
        }
        delete head;
        delete tail;
    }
    // 尾增
    void addLast(T value)
    {
        Node *x = new Node(value);
        Node *temp = tail->prev;

        temp->next = x;
        x->prev = temp;

        x->next = tail;
        tail->prev = x;

        size++;
    }

    void addHead(T value)
    {
        Node *x = new Node(value);
        Node *temp = head->next;

        x->next = temp;
        temp->prev = x;

        x->prev = head;
        head->next = x;

        size++;
    }

    void add(int index, T value)
    {
        if (index == size)
        {
            addLast(value);
            return;
        }

        Node *temp = findNode(index);
        Node *pretemp = temp->prev;

        Node *x = new Node(value);

        x->next = temp;
        temp->prev = x;

        pretemp->next = x;
        x->prev = pretemp;

        size++;
    }

    // 尾删
    void removeLast()
    {
        Node *old = tail->prev;
        Node *last = old->prev;

        last->next = tail;
        tail->prev = last;

        old->next = nullptr;
        old->prev = nullptr;

        size--;
    }

    // 头删
    void removeHead()
    {
        Node *old = head->next;
        Node *first = old->next;

        head->next = first;
        first->prev = head;

        old->next = nullptr;
        old->prev = nullptr;

        size--;
    }

    // 删除
    T remove(int index)
    {
        Node *qtemp = findNode(index);
        Node *p = qtemp->prev;
        Node *n = qtemp->next;

        p->next = n;
        n->prev = p;

        qtemp->next = nullptr;
        qtemp->prev = nullptr;

        size--;

        return qtemp->val;
    }

    // 改
    void modify(int index, T value)
    {
        Node *p = findNode(index);
        p->val = value;
    }

    // 查找
    T myfind(int index)
    {
        Node *p = findNode(index);
        return p->val;
    }

    // 获取链表大小
    int getSize()
    {
        return size;
    }

    void display()
    {

        for (Node *p = head->next; p != tail; p = p->next)
        {
            cout << p->val << " ";
        }
        cout << endl;
    }
};

int main()
{
    myDLinkList<int> list;

    list.addHead(1);
    list.addLast(5);
    list.addHead(2);
    list.addLast(3);
    list.display();

    list.add(2, 4);
    list.display();

    list.modify(4, 2);
    list.display();

    cout << list.myfind(2) << endl;
    cout << list.getSize() << endl;

    list.removeHead();
    list.display();

    list.removeLast();
    list.display();

    list.remove(2);
    list.display();
}