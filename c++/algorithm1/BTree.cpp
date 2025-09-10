#include <iostream>
#include <cmath>

using namespace std;

template <class K, size_t M>
struct BTreeNode
{
    // K _keys[M - 1];
    // BTreeNode<K, M>* _subs[M];

    // M叉树：即一个节点最多有M-1个关键字,M个孩子
    // 为了方便插入以后再分裂，多给一个空间
    K _keys[M];                    // 存储关键字的数组
    BTreeNode<K, M> *_subs[M + 1]; // 存储孩子指针的数组
    size_t _n;                     // 记录实际存储的关键字的个数
    BTreeNode<K, M> *_parent;      // 父指针

    BTreeNode()
    {
        for (size_t i = 0; i < M; i++)
        {
            _keys[i] = K();
            _subs[i] = nullptr;
        }
        _n = 0;
        _subs[M] = nullptr;
        _parent = nullptr;
    }
};

// 数据是存在磁盘，K是磁盘地址
template <class K, size_t M>
class myBtree
{
    typedef BTreeNode<K, M> Node;

public:
    // 返回值: Node代表找到的节点,int为该元素在该节点中的位置
    pair<Node *, int> Find(const K &key)
    {
        // 从根节点开始找
        Node *cur = _root;
        Node *parent = nullptr;

        while (cur) // 节点存在
        {
            // 在一个节点查找
            size_t i = 0;
            while (i < cur->_n)
            {
                if (key < cur->_keys[i]) // 可能在左子树
                {
                    break;
                }
                else if (key > cur->_keys[i]) // 向右寻找
                {
                    ++i;
                }
                else // 找到返回
                {
                    return make_pair(cur, i);
                }
            }
            // 在子树中查找
            parent = cur;
            cur = cur->_subs[i];
        }
        // 没找到，把叶子节点返回去
        return make_pair(parent, -1);
    }

    void InsertKey(Node *node, const K &key, Node *child)
    {
        int end = node->_n - 1;
        while (end >= 0)
        {
            if (key < node->_keys[end])
            {
                // 插入 挪动key和它的右孩子
                node->_keys[end + 1] = node->_keys[end];
                node->_subs[end + 2] = node->_subs[end + 1];
                --end;
            }
            else
            {
                break;
            }
        }
        node->_keys[end + 1] = key;
        node->_subs[end + 2] = child;
        node->_n++;
        if (child)
        {
            child->_parent = node;
        }
    }

    bool Insert(const K &key)
    {
        // 如果树为空，直接插入
        if (_root == nullptr)
        {
            _root = new Node;
            _root->_keys[0] = key;
            _root->_n = 1;
            num++;
            return true;
        }

        // 找插入位置，如果key已经存在，则不插入
        auto ret = Find(key);
        if (ret.second >= 0)
        {
            return false;
        }

        // 如果没有找到，Find顺便带回了要插入的那个叶子节点

        // 循环每次往cur插入 newkey和child
        Node *cur = ret.first;
        Node *child = nullptr;
        K newkey = key;
        while (1)
        {
            InsertKey(cur, newkey, child);
            // 满了就要分裂
            // 没有满，插入就结束
            if (cur->_n < M)
            {
                num++;
                return true;
            }
            else
            {
                size_t mid = M / 2;
                // 申请兄弟节点,分裂一半给兄弟 [mid + 1, M - 1]
                Node *brother = new Node;
                size_t j = 0;
                size_t i = mid + 1;
                for (; i < M; i++)
                {
                    // 分裂拷贝key和key的右孩子给兄弟 (分裂的是叶子节点没有孩子,分支节点和根节点都有孩子)
                    brother->_keys[j] = cur->_keys[i];
                    brother->_subs[j] = cur->_subs[i];
                    // 如果孩子存在,分裂孩子给兄弟之后,孩子的父指针指向兄弟
                    if (cur->_subs[i])
                    {
                        cur->_subs[i]->_parent = brother;
                    }
                    ++j;

                    // 拷走重置一下方便观察
                    cur->_keys[i] = K();
                    cur->_subs[i] = nullptr;
                }

                // 最后一个右孩子也要拷贝给兄弟
                brother->_subs[j] = cur->_subs[i];
                // 孩子的父指针指向兄弟
                if (cur->_subs[i])
                    cur->_subs[i]->_parent = brother;
                cur->_subs[i] = nullptr;

                // 更新节点关键字个数
                brother->_n = j;
                cur->_n -= (brother->_n + 1); // 1表示中位数要提取给父亲

                // 提取中位数给父亲
                K newmid = cur->_keys[mid];
                cur->_keys[mid] = K();

                // 分裂的是根节点
                if (cur->_parent == nullptr)
                {
                    _root = new Node;
                    _root->_keys[0] = newmid;
                    _root->_subs[0] = cur;
                    _root->_subs[1] = brother;
                    _root->_n = 1;
                    cur->_parent = _root;
                    brother->_parent = _root;
                    num++;
                    return true;
                }
                else // 分裂的是叶子节点或者分支节点
                {
                    // 转换成往cur->parent 去插入cur->[mid] 和 brother
                    cur = cur->_parent;
                    newkey = newmid;
                    child = brother;
                }
            }
        }
    }

    void InOrder(Node *root)
    {
        if (root == nullptr)
            return;

        // 左 根 左 根 ... 右
        size_t i = 0;
        for (; i < root->_n; ++i)
        {
            InOrder(root->_subs[i]);       // 左子树
            cout << root->_keys[i] << " "; // 根
        }
        InOrder(root->_subs[i]); // 最后的那个右子树
    }

    bool Remove(const K &key)
    {
        auto ret = Find(key);
        // 没找到
        if (ret.second == -1)
            return false;

        // 找到删除结点以及删除关键字对应的下标
        Node *del = ret.first;
        size_t deli = ret.second;

        // 1.非叶子结点 ----> 找左子树最右关键字或者右子树最左关键字,(叶子节点的关键字)替换删除
        if (del->_subs[deli])
        {
            // 找右子树最左关键字
            Node *cur = del->_subs[deli + 1];
            while (cur->_subs[0])
            {
                cur = cur->_subs[0];
            }
            del->_keys[deli] = cur->_keys[0];
            del = cur;
            compress(del, 1); // 用后面关键字和孩子将前面覆盖
        }
        else // 叶子节点
        {
            compress(del, deli + 1);
        }

        // 删除关键字之后,看是否符合关键字个数最少是cile[M/2] - 1,如果不满足就去调整或者合并.
        int minkeycnt = ceil(M / 2.0) - 1;
        while (1)
        {
            if (del->_n < minkeycnt) // 关键字比最少还少,调整或者合并
            {
                // 不管调整还是合并都和父节点对应关键字有关,先找父亲对应关键字下标
                size_t j = 0;
                Node *parent = del->_parent;
                while (parent->_subs[j] != del)
                    j++;
                // 如果del有右兄弟 左调整或者左合并
                if (j < parent->_n)
                {
                    LeftAdjust(parent, del, minkeycnt, j);
                }
                else // del是父亲其他孩子,都可以进行 右调整或者右合并
                {
                    RightAdjust(parent, del, minkeycnt, j);
                }
                del = parent;    // 继续向上调整
                if (del = _root) // 调整到根结束
                    break;
            }
            else
            {
                break;
            }
        }
        if (_root->_n == 0)
        {
            _root = _root->_subs[0]; // 合并的是新根
            delete del;              // 删原本的根
            _root->_parent = nullptr;
        }
        num--;
        return true;
    }

    void LeftAdjust(Node *parent, Node *del, size_t minkeycnt, size_t j)
    {
        Node *Rbrother = parent->_subs[j + 1];
        if (Rbrother->_n > minkeycnt) // 调整
        {
            del->_keys[del->_n] = parent->_keys[j];       // 父节点对应关键字下移
            parent->_keys[j] = Rbrother->_keys[0];        // 右兄弟最小关键字上移父节点
            del->_subs[del->_n + 1] = Rbrother->_subs[0]; // 右兄弟最左指针左移
            if (Rbrother->_subs[0])                       // 修改右兄弟最左孩子的父指针
            {
                Rbrother->_subs[0]->_parent = del;
            }
            del->_n++;             // del关键字+1
            compress(Rbrother, 1); // 右兄弟中剩余关键性和孩子前移
            // size_t i = 1;
            // for(; i < Rbrother->_n; ++i)//右兄弟中剩余关键性和孩子前移
            // {
            //     Rbrother->_key[i - 1] = Rbrother->_key[i];
            //     Rbrother->_sub[i - 1] = Rbrother->_sub[i];
            // }
            // Rbrother->_sub[i - 1] = Rbrother->_sub[i];//最后一个孩子
            // Rbrother->_n--;//右兄弟关键字个数减一
        }
        else // 合并
        {
            merge(parent, del, Rbrother, j);
        }
    }

    void RightAdjust(Node *parent, Node *del, size_t minkeycnt, size_t j)
    {
        Node *Lbrother = parent->_subs[j - 1];
        if (Lbrother->_n > minkeycnt) // 调整
        {
            // 将删除结点的关键字和孩子往后移一位
            del->_subs[del->_n + 1] = del->_subs[del->_n];
            for (size_t i = del->_n - 1; i >= 0; --i)
            {
                del->_keys[i + 1] = del->_keys[i];
                del->_subs[i + 1] = del->_subs[i];
            }
            // 父亲结点对应关键字下移
            del->_keys[0] = parent->_keys[j - 1];
            // 左兄弟最大关键字上移
            parent->_keys[j - 1] = Lbrother->_keys[Lbrother->_n - 1];
            // 左兄弟最右指针右移
            del->_subs[0] = Lbrother->_subs[Lbrother->_n];
            // 修改左兄弟最右孩子的父指针
            if (Lbrother->_subs[Lbrother->_n])
            {
                Lbrother->_subs[Lbrother->_n]->_parent = del;
            }
            // 为了方便观察
            Lbrother->_keys[Lbrother->_n - 1] = K();
            Lbrother->_subs[Lbrother->_n] = nullptr;
            // del关键字+1
            del->_n++;
            // 左兄弟关键字减一即可,不用在移动
            Lbrother->_n--;
        }
        else
        {
            merge(parent, del, Lbrother, j);
        }
    }

    void merge(Node *parent, Node *del, Node *brother, size_t j)
    {
        if (brother == parent->_subs[j - 1])
        {
            // 右合并,保留左边结点,因此将del和左兄弟指针交换一下,转化成左合并
            swap(del, brother);
            j--;
        }

        // 左合并,保留del节点,删除兄弟节点(保留左边结点)
        del->_keys[del->_n] = parent->_keys[j]; // 将父节点对应关键字下移
        // del->_sub[del->_n + 1] = brother->_sub[0]; // 从右兄弟节点左移第一个指针
        // for (size_t i = 0; i < brother->_n; ++i)   // 将右兄弟剩余关键字和指针前移
        // {
        //     del->_key[del->_n + i + 1] = brother->_key[i];
        //     del->_sub[del->_n + i + 2] = brother->_sub[i + 1];
        // }
        // 将右兄弟关键字和指针前移
        size_t i = 0;
        for (; i < brother->_n; ++i)
        {
            del->_keys[del->_n + i + 1] = brother->_keys[i];
            del->_subs[del->_n + i + 1] = brother->_subs[i];
            // 别忘记修改孩子父指针的指向
            if (brother->_subs[i])
            {
                brother->_subs[i]->_parent = del;
            }
        }
        del->_subs[del->_n + i + 1] = brother->_subs[i];
        del->_n += brother->_n + 1;              // 更新保存节点的关键字个数
        delete brother;                          // 删除兄弟
        parent->_keys[j] = parent->_keys[j + 1]; // 先将父节点对应关键字后面一位关键字前移,否则compree会出错
        compress(parent, j + 2);                 // 将父节点对应关键字后面的关键字和孩子往前移
    }

    void compress(Node *node, size_t pos)
    {
        size_t i = pos;
        for (; i < node->_n; ++i)
        {
            node->_keys[i - 1] = node->_keys[i];
            node->_subs[i - 1] = node->_subs[i];
        }
        node->_subs[i - 1] = node->_subs[i]; // 最后一个孩子节点
        // 方便观察
        node->_keys[node->_n - 1] = K();
        node->_subs[node->_n] = nullptr;

        node->_n--;
    }

    Node *getRoot()
    {
        return _root;
    }

    size_t getNum()
    {
        return num;
    }

private:
    Node *_root = nullptr;
    size_t num = 0;
};

int main()
{
    myBtree<int, 5> B;
    B.Insert(39);
    B.Insert(22);
    B.Insert(97);
    B.Insert(41);
    B.Insert(53);
    B.Insert(13);
    B.Insert(21);
    B.Insert(40);
    B.Insert(30);
    B.Insert(27);
    B.Insert(33);
    B.Insert(36);
    B.Insert(35);
    B.Insert(34);
    B.Insert(24);
    B.Insert(29);
    B.Insert(26);
    B.Insert(17);
    B.Insert(28);
    B.Insert(31);
    B.Insert(32);
    cout << B.getNum() << endl;
    B.InOrder(B.getRoot());

    cout << endl;

    B.Remove(21);
    B.Remove(27);
    B.Remove(33);
    B.Remove(40);
    cout << B.getNum() << endl;
    B.InOrder(B.getRoot());
}