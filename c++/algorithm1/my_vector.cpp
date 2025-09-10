#include <iostream>

using namespace std;

template <class T>
class myvector
{
public:
    myvector() // 构造函数
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

    // 在末尾插入
    void insert(T num)
    {
        if (size == cap)
        {
            resize(2 * cap);
        }
        arr[size] = num;
        size++;
    }

    // 在中间插入
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

    // 删除
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

    // 删除索引值
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

    // 改
    void mymodify(int index, T num)
    {
        arr[index] = num;
    }

    // 查
    T myfind(int index)
    {
        return arr[index];
    }

    // 查看数据个数
    int getSize()
    {
        return this->size;
    }

    // 查看全部数据
    void display()
    {
        for (int i = 0; i < size; i++)
        {
            cout << arr[i] << " ";
        }
        cout << endl;
    }

    // 重新分配大小
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
    int cap;                       // 最大容量
    int size;                      // 当前数据个数
    T *arr;                        // 底层数组
    static const int INIT_CAP = 1; // 初始容量
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