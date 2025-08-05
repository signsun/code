#include <iostream>

using namespace std;

template <class T>
class myvector
{
public:
    myvector() // ���캯��
    {
        this->arr = new T[INIT_CAP];
        this->cap = INIT_CAP;
        this->size = 0;
    }

    myvector(int init_cap)
    {
        this->arr = new T[init_cap];
        this->cap = 0;
        this->size = init_cap;
    }

    ~myvector()
    {
        delete[] arr;
    }

    // ��ĩβ����
    void insert(T num)
    {
        if (size == cap)
        {
            resize(2 * cap);
        }
        arr[size] = num;
        size++;
    }

    // ���м����
    void insert(int index, T num)
    {
        if (size == cap)
        {
            resize(2 * cap);
        }

        for (int i = size - 1; i >= index; i--)
        {
            arr[i + 1] = arr[i];
        }
        arr[index] = num;
        size++;
    }

    // ɾ��
    T mydelete()
    {
        T temp = arr[size - 1];
        arr[size - 1] = T();
        size--;
        if (size == cap / 4)
        {
            resize(cap / 2);
        }

        return temp;
    }

    // ɾ������ֵ
    T mydelete(int index)
    {
        T temp = arr[index];
        for (int i = index; i < size - 1; i++)
        {
            arr[i] = arr[i + 1];
        }
        arr[size] = T();
        size--;
        if (size == cap / 4)
        {
            resize(cap / 2);
        }
        return temp;
    }

    // ��
    void mymodify(int index, T num)
    {
        arr[index] = num;
    }

    // ��
    T myfind(int index)
    {
        return arr[index];
    }

    // �鿴���ݸ���
    int getSize()
    {
        return this->size;
    }

    // �鿴ȫ������
    void display()
    {
        for (int i = 0; i < size; i++)
        {
            cout << arr[i] << " ";
        }
        cout << endl;
    }

    // ���·����С
    void resize(int newcap)
    {
        T *newarr = new T[newcap];
        for (int i = 0; i < this->size; i++)
        {
            newarr[i] = arr[i];
        }
        delete[] arr;

        cap = newcap;
        arr = newarr;
    }

private:
    int cap;                       // �������
    int size;                      // ��ǰ���ݸ���
    T *arr;                        // �ײ�����
    static const int INIT_CAP = 1; // ��ʼ����
};

int main()
{
    myvector<int> my;
    my.insert(1);
    my.insert(2);
    my.insert(3);
    my.insert(4);
    my.insert(5);
    my.insert(6);
    my.display();
    my.insert(2, 4);
    my.display();
    my.mydelete();
    my.display();
    my.mydelete(3);
    my.display();
    cout << my.myfind(3) << endl;
    cout << my.getSize() << endl;
    my.mymodify(1, 7);
    my.display();

    return 0;
}