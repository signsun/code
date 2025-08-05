#include <iostream>
#include <cmath>

using namespace std;

template <class K, size_t M>
struct BTreeNode
{
    // K _keys[M - 1];
    // BTreeNode<K, M>* _subs[M];

    // M��������һ���ڵ������M-1���ؼ���,M������
    // Ϊ�˷�������Ժ��ٷ��ѣ����һ���ռ�
    K _keys[M];                    // �洢�ؼ��ֵ�����
    BTreeNode<K, M> *_subs[M + 1]; // �洢����ָ�������
    size_t _n;                     // ��¼ʵ�ʴ洢�Ĺؼ��ֵĸ���
    BTreeNode<K, M> *_parent;      // ��ָ��

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

// �����Ǵ��ڴ��̣�K�Ǵ��̵�ַ
template <class K, size_t M>
class myBtree
{
    typedef BTreeNode<K, M> Node;

public:
    // ����ֵ: Node�����ҵ��Ľڵ�,intΪ��Ԫ���ڸýڵ��е�λ��
    pair<Node *, int> Find(const K &key)
    {
        // �Ӹ��ڵ㿪ʼ��
        Node *cur = _root;
        Node *parent = nullptr;

        while (cur) // �ڵ����
        {
            // ��һ���ڵ����
            size_t i = 0;
            while (i < cur->_n)
            {
                if (key < cur->_keys[i]) // ������������
                {
                    break;
                }
                else if (key > cur->_keys[i]) // ����Ѱ��
                {
                    ++i;
                }
                else // �ҵ�����
                {
                    return make_pair(cur, i);
                }
            }
            // �������в���
            parent = cur;
            cur = cur->_subs[i];
        }
        // û�ҵ�����Ҷ�ӽڵ㷵��ȥ
        return make_pair(parent, -1);
    }

    void InsertKey(Node *node, const K &key, Node *child)
    {
        int end = node->_n - 1;
        while (end >= 0)
        {
            if (key < node->_keys[end])
            {
                // ���� Ų��key�������Һ���
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
        // �����Ϊ�գ�ֱ�Ӳ���
        if (_root == nullptr)
        {
            _root = new Node;
            _root->_keys[0] = key;
            _root->_n = 1;
            num++;
            return true;
        }

        // �Ҳ���λ�ã����key�Ѿ����ڣ��򲻲���
        auto ret = Find(key);
        if (ret.second >= 0)
        {
            return false;
        }

        // ���û���ҵ���Find˳�������Ҫ������Ǹ�Ҷ�ӽڵ�

        // ѭ��ÿ����cur���� newkey��child
        Node *cur = ret.first;
        Node *child = nullptr;
        K newkey = key;
        while (1)
        {
            InsertKey(cur, newkey, child);
            // ���˾�Ҫ����
            // û����������ͽ���
            if (cur->_n < M)
            {
                num++;
                return true;
            }
            else
            {
                size_t mid = M / 2;
                // �����ֵܽڵ�,����һ����ֵ� [mid + 1, M - 1]
                Node *brother = new Node;
                size_t j = 0;
                size_t i = mid + 1;
                for (; i < M; i++)
                {
                    // ���ѿ���key��key���Һ��Ӹ��ֵ� (���ѵ���Ҷ�ӽڵ�û�к���,��֧�ڵ�͸��ڵ㶼�к���)
                    brother->_keys[j] = cur->_keys[i];
                    brother->_subs[j] = cur->_subs[i];
                    // ������Ӵ���,���Ѻ��Ӹ��ֵ�֮��,���ӵĸ�ָ��ָ���ֵ�
                    if (cur->_subs[i])
                    {
                        cur->_subs[i]->_parent = brother;
                    }
                    ++j;

                    // ��������һ�·���۲�
                    cur->_keys[i] = K();
                    cur->_subs[i] = nullptr;
                }

                // ���һ���Һ���ҲҪ�������ֵ�
                brother->_subs[j] = cur->_subs[i];
                // ���ӵĸ�ָ��ָ���ֵ�
                if (cur->_subs[i])
                    cur->_subs[i]->_parent = brother;
                cur->_subs[i] = nullptr;

                // ���½ڵ�ؼ��ָ���
                brother->_n = j;
                cur->_n -= (brother->_n + 1); // 1��ʾ��λ��Ҫ��ȡ������

                // ��ȡ��λ��������
                K newmid = cur->_keys[mid];
                cur->_keys[mid] = K();

                // ���ѵ��Ǹ��ڵ�
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
                else // ���ѵ���Ҷ�ӽڵ���߷�֧�ڵ�
                {
                    // ת������cur->parent ȥ����cur->[mid] �� brother
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

        // �� �� �� �� ... ��
        size_t i = 0;
        for (; i < root->_n; ++i)
        {
            InOrder(root->_subs[i]);       // ������
            cout << root->_keys[i] << " "; // ��
        }
        InOrder(root->_subs[i]); // �����Ǹ�������
    }

    bool Remove(const K &key)
    {
        auto ret = Find(key);
        // û�ҵ�
        if (ret.second == -1)
            return false;

        // �ҵ�ɾ������Լ�ɾ���ؼ��ֶ�Ӧ���±�
        Node *del = ret.first;
        size_t deli = ret.second;

        // 1.��Ҷ�ӽ�� ----> �����������ҹؼ��ֻ�������������ؼ���,(Ҷ�ӽڵ�Ĺؼ���)�滻ɾ��
        if (del->_subs[deli])
        {
            // ������������ؼ���
            Node *cur = del->_subs[deli + 1];
            while (cur->_subs[0])
            {
                cur = cur->_subs[0];
            }
            del->_keys[deli] = cur->_keys[0];
            del = cur;
            compress(del, 1); // �ú���ؼ��ֺͺ��ӽ�ǰ�渲��
        }
        else // Ҷ�ӽڵ�
        {
            compress(del, deli + 1);
        }

        // ɾ���ؼ���֮��,���Ƿ���Ϲؼ��ָ���������cile[M/2] - 1,����������ȥ�������ߺϲ�.
        int minkeycnt = ceil(M / 2.0) - 1;
        while (1)
        {
            if (del->_n < minkeycnt) // �ؼ��ֱ����ٻ���,�������ߺϲ�
            {
                // ���ܵ������Ǻϲ����͸��ڵ��Ӧ�ؼ����й�,���Ҹ��׶�Ӧ�ؼ����±�
                size_t j = 0;
                Node *parent = del->_parent;
                while (parent->_subs[j] != del)
                    j++;
                // ���del�����ֵ� �����������ϲ�
                if (j < parent->_n)
                {
                    LeftAdjust(parent, del, minkeycnt, j);
                }
                else // del�Ǹ�����������,�����Խ��� �ҵ��������Һϲ�
                {
                    RightAdjust(parent, del, minkeycnt, j);
                }
                del = parent;    // �������ϵ���
                if (del = _root) // ������������
                    break;
            }
            else
            {
                break;
            }
        }
        if (_root->_n == 0)
        {
            _root = _root->_subs[0]; // �ϲ������¸�
            delete del;              // ɾԭ���ĸ�
            _root->_parent = nullptr;
        }
        num--;
        return true;
    }

    void LeftAdjust(Node *parent, Node *del, size_t minkeycnt, size_t j)
    {
        Node *Rbrother = parent->_subs[j + 1];
        if (Rbrother->_n > minkeycnt) // ����
        {
            del->_keys[del->_n] = parent->_keys[j];       // ���ڵ��Ӧ�ؼ�������
            parent->_keys[j] = Rbrother->_keys[0];        // ���ֵ���С�ؼ������Ƹ��ڵ�
            del->_subs[del->_n + 1] = Rbrother->_subs[0]; // ���ֵ�����ָ������
            if (Rbrother->_subs[0])                       // �޸����ֵ������ӵĸ�ָ��
            {
                Rbrother->_subs[0]->_parent = del;
            }
            del->_n++;             // del�ؼ���+1
            compress(Rbrother, 1); // ���ֵ���ʣ��ؼ��Ժͺ���ǰ��
            // size_t i = 1;
            // for(; i < Rbrother->_n; ++i)//���ֵ���ʣ��ؼ��Ժͺ���ǰ��
            // {
            //     Rbrother->_key[i - 1] = Rbrother->_key[i];
            //     Rbrother->_sub[i - 1] = Rbrother->_sub[i];
            // }
            // Rbrother->_sub[i - 1] = Rbrother->_sub[i];//���һ������
            // Rbrother->_n--;//���ֵܹؼ��ָ�����һ
        }
        else // �ϲ�
        {
            merge(parent, del, Rbrother, j);
        }
    }

    void RightAdjust(Node *parent, Node *del, size_t minkeycnt, size_t j)
    {
        Node *Lbrother = parent->_subs[j - 1];
        if (Lbrother->_n > minkeycnt) // ����
        {
            // ��ɾ�����Ĺؼ��ֺͺ���������һλ
            del->_subs[del->_n + 1] = del->_subs[del->_n];
            for (size_t i = del->_n - 1; i >= 0; --i)
            {
                del->_keys[i + 1] = del->_keys[i];
                del->_subs[i + 1] = del->_subs[i];
            }
            // ���׽���Ӧ�ؼ�������
            del->_keys[0] = parent->_keys[j - 1];
            // ���ֵ����ؼ�������
            parent->_keys[j - 1] = Lbrother->_keys[Lbrother->_n - 1];
            // ���ֵ�����ָ������
            del->_subs[0] = Lbrother->_subs[Lbrother->_n];
            // �޸����ֵ����Һ��ӵĸ�ָ��
            if (Lbrother->_subs[Lbrother->_n])
            {
                Lbrother->_subs[Lbrother->_n]->_parent = del;
            }
            // Ϊ�˷���۲�
            Lbrother->_keys[Lbrother->_n - 1] = K();
            Lbrother->_subs[Lbrother->_n] = nullptr;
            // del�ؼ���+1
            del->_n++;
            // ���ֵܹؼ��ּ�һ����,�������ƶ�
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
            // �Һϲ�,������߽��,��˽�del�����ֵ�ָ�뽻��һ��,ת������ϲ�
            swap(del, brother);
            j--;
        }

        // ��ϲ�,����del�ڵ�,ɾ���ֵܽڵ�(������߽��)
        del->_keys[del->_n] = parent->_keys[j]; // �����ڵ��Ӧ�ؼ�������
        // del->_sub[del->_n + 1] = brother->_sub[0]; // �����ֵܽڵ����Ƶ�һ��ָ��
        // for (size_t i = 0; i < brother->_n; ++i)   // �����ֵ�ʣ��ؼ��ֺ�ָ��ǰ��
        // {
        //     del->_key[del->_n + i + 1] = brother->_key[i];
        //     del->_sub[del->_n + i + 2] = brother->_sub[i + 1];
        // }
        // �����ֵܹؼ��ֺ�ָ��ǰ��
        size_t i = 0;
        for (; i < brother->_n; ++i)
        {
            del->_keys[del->_n + i + 1] = brother->_keys[i];
            del->_subs[del->_n + i + 1] = brother->_subs[i];
            // �������޸ĺ��Ӹ�ָ���ָ��
            if (brother->_subs[i])
            {
                brother->_subs[i]->_parent = del;
            }
        }
        del->_subs[del->_n + i + 1] = brother->_subs[i];
        del->_n += brother->_n + 1;              // ���±���ڵ�Ĺؼ��ָ���
        delete brother;                          // ɾ���ֵ�
        parent->_keys[j] = parent->_keys[j + 1]; // �Ƚ����ڵ��Ӧ�ؼ��ֺ���һλ�ؼ���ǰ��,����compree�����
        compress(parent, j + 2);                 // �����ڵ��Ӧ�ؼ��ֺ���Ĺؼ��ֺͺ�����ǰ��
    }

    void compress(Node *node, size_t pos)
    {
        size_t i = pos;
        for (; i < node->_n; ++i)
        {
            node->_keys[i - 1] = node->_keys[i];
            node->_subs[i - 1] = node->_subs[i];
        }
        node->_subs[i - 1] = node->_subs[i]; // ���һ�����ӽڵ�
        // ����۲�
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