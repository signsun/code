#include <iostream>

using namespace std;

#define red 1
#define black 2
template <class DataType>
class RBTree
{
private:
    struct Node
    {
        DataType Data;      // ����
        int Color;          // �ڵ���ɫ
        int Key;            // �ؼ���
        Node *Left, *Right; // ���Һ���
        Node *Parent;       // ���ڵ�ָ��

        Node()
        {
            Data = DataType();
            Color = red;
            Key = 0;
            Left = nullptr;
            Right = nullptr;
            Parent = nullptr;
        }
    };

public:
    RBTree()
    {
        root = nullptr;
        nil = nullptr;
    }

    void insertKey(Node *par, Node *newNode)
    {
        if (newNode->Key < par->Key)
        {
            par->Left = newNode;
            newNode->Parent = par;
        }
        else
        {
            par->Right = newNode;
            newNode->Parent = par;
        }
    }

    bool insert(const int &key, const DataType &data)
    {
        // ��Ϊ��ֱ�Ӳ���
        if (root == nullptr)
        {
            Node *newNode = new Node;
            newNode->Color = black;
            newNode->Data = data;
            newNode->Key = key;
            root = newNode;
            return true;
        }

        // �жϷ���ֵ
        auto ret = find(key);
        Node *cur = ret.first; // ���صĽڵ�
        // �������key������������
        if (ret.second == 1)
        {
            cur->Data = data;
            return true;
        }

        Node *newNode = new Node;
        newNode->Data = data;
        newNode->Key = key;
        // ���ڵ��Ǻ�ɫ��ֱ�Ӳ���
        if (cur->Color == black)
        {

            insertKey(cur, newNode);
            return true;
        }
        // ���ڵ��Ǻ�ɫ
        else
        {
            Node *grandParent = cur->Parent;
            // ���ڵ����游�������ӽڵ�
            if (cur == grandParent->Left)
            {
                // ���ڵ�û���ֵܽڵ�����ֵܽڵ��Ǻ�ɫ
                if (grandParent->Right == nullptr || grandParent->Right->Color == black)
                {
                    insertKey(cur, newNode);
                    if (newNode == cur->Left)
                    {
                        cur->Color = black;
                        grandParent->Color = red;
                        rightRotate(grandParent);
                    }
                    else
                    {
                        leftRotate(cur);
                        newNode->Color = black;
                        grandParent->Color = red;
                        rightRotate(grandParent);
                    }
                }
                // ����ڵ���ɫ�Ǻ�ɫ
                // �ø��ڵ������ڵ���ɫ��Ϊ��ɫ���游�ڵ���ɫ��Ϊ��ɫ
                else if (grandParent->Right->Color == red)
                {
                    insertKey(cur, newNode);
                    cur->Color = black;
                    grandParent->Right->Color = black;
                    grandParent->Color = red;
                }
            }
            else
            {

                if (grandParent->Left == nullptr || grandParent->Left->Color == black)
                {
                    insertKey(cur, newNode);
                    if (newNode == cur->Left)
                    {
                        rightRotate(cur);
                        newNode->Color = black;
                        grandParent->Color = red;
                        leftRotate(grandParent);
                    }
                    else
                    {
                        cur->Color = black;
                        grandParent->Color = red;
                        leftRotate(grandParent);
                    }
                }
                // ����ڵ���ɫ�Ǻ�ɫ
                // �ø��ڵ������ڵ���ɫ��Ϊ��ɫ���游�ڵ���ɫ��Ϊ��ɫ
                else if (grandParent->Left->Color == red)
                {
                    insertKey(cur, newNode);
                    cur->Color = black;
                    grandParent->Left->Color = black;
                    grandParent->Color = red;
                }
            }
            root->Color = black;
            return true;
        }
    }

    bool remove(const int &key)
    {
        if (root == nullptr)
            return false;
        auto ret = find(key);
        if (ret.second == -1)
        {
            cout << "���ݲ�����" << endl;
            return false;
        }

        // Ҫɾ���Ľڵ�
        Node *delNode = ret.first;
        ret.first = nullptr;
        Node *par = delNode->Parent;
        // Ҫɾ���Ľڵ���Ҷ�ӽڵ�
        if (delNode->Left == nullptr && delNode->Right == nullptr)
        {
            // Ҫɾ����Ҷ�ӽڵ��Ǻ�ɫ��ֱ��ɾ��
            if (delNode->Color == red)
            {
                if (delNode == par->Left)
                {
                    par->Left = nullptr;
                    delete delNode;
                    delNode = nullptr;
                    return true;
                }
                else
                {
                    par->Right = nullptr;
                    delete delNode;
                    delNode = nullptr;
                    return true;
                }
            }
            // Ҫɾ���ڵ�DΪ��ɫ
            else
            {
                // Ҫɾ���Ľڵ����丸�ڵ�����ӽڵ�
                if (delNode == par->Left)
                {
                    Node *brother = par->Right;
                    // �ֵܽڵ�û�����Һ���
                    if (brother->Left == nullptr && brother->Right == nullptr)
                    {
                        par->Left = nullptr;
                        delete delNode;
                        delNode = nullptr;
                        par->Color = black;
                        brother->Color = red;
                    }
                    // �ֵܽڵ������ӣ��Һ���Ϊ��
                    else if (brother->Left != nullptr && brother->Right == nullptr)
                    {
                        par->Left = nullptr;
                        delete delNode;
                        delNode = nullptr;
                        rightRotate(par->Right);
                        par->Right->Color = par->Color;
                        par->Color = black;
                        leftRotate(par);
                    }
                    // �ֵܽڵ����Һ��ӣ�����Ϊ��
                    else if (brother->Left == nullptr && brother->Right != nullptr)
                    {
                        par->Left = nullptr;
                        delete delNode;
                        delNode = nullptr;
                        brother->Color = par->Color;
                        par->Color = black;
                        brother->Right->Color = black;
                        leftRotate(par);
                    }
                    // �ֵܽڵ����Һ��Ӷ����ڣ���Ϊ��ɫ
                    else if (brother->Left != nullptr && brother->Right != nullptr)
                    {
                        par->Left = nullptr;
                        delete delNode;
                        delNode = nullptr;
                        leftRotate(par);
                        brother->Color = par->Color;
                        par->Color = black;
                        brother->Right->Color = black;
                    }
                    // �ֵܽڵ�Ϊ��ɫ
                    else if (brother->Color == red)
                    {
                        par->Left = nullptr;
                        delete delNode;
                        delNode = nullptr;
                        leftRotate(par);
                        brother->Color = black;
                        par->Right->Color = red;
                    }
                }
                // Ҫɾ���Ľڵ����丸�ڵ�����ӽڵ�
                else
                {
                    Node *brother = par->Left;
                    // �ֵܽڵ�û�����Һ���
                    if (brother->Left == nullptr && brother->Right == nullptr)
                    {
                        par->Right = nullptr;
                        delete delNode;
                        delNode = nullptr;
                        par->Color = black;
                        brother->Color = red;
                    }
                    // �ֵܽڵ������ӣ��Һ���Ϊ��
                    else if (brother->Left != nullptr && brother->Right == nullptr)
                    {
                        par->Right = nullptr;
                        delete delNode;
                        delNode = nullptr;
                        brother->Color = par->Color;
                        par->Color = black;
                        brother->Left->Color = black;
                        rightRotate(par);
                    }
                    // �ֵܽڵ����Һ��ӣ�����Ϊ��
                    else if (brother->Left == nullptr && brother->Right != nullptr)
                    {
                        par->Right = nullptr;
                        delete delNode;
                        delNode = nullptr;
                        leftRotate(brother);
                        par->Left->Color = par->Color;
                        par->Color = black;
                        rightRotate(par);
                    }
                    // �ֵܽڵ����Һ��Ӷ����ڣ���Ϊ��ɫ
                    else if (brother->Left != nullptr && brother->Right != nullptr)
                    {
                        par->Right = nullptr;
                        delete delNode;
                        delNode = nullptr;
                        rightRotate(par);
                        brother->Color = par->Color;
                        par->Color = black;
                        brother->Left->Color = black;
                    }
                    // �ֵܽڵ�Ϊ��ɫ
                    else if (brother->Color == red)
                    {
                        par->Right = nullptr;
                        delete delNode;
                        delNode = nullptr;
                        rightRotate(par);
                        brother->Color = black;
                        par->Left->Color = red;
                    }
                }
            }
        }
        // ɾ���ڵ�ֻ�����ӣ�û���Һ���
        else if (delNode->Left != nullptr && delNode->Right == nullptr)
        {
            Node *child = delNode->Left;
            delNode->Data = child->Data;
            delNode->Key = child->Key;
            delNode->Left = nullptr;
            delete child;
            child = nullptr;
        }
        // ɾ���ڵ�ֻ���Һ��ӣ�û������
        else if (delNode->Left == nullptr && delNode->Right != nullptr)
        {
            Node *child = delNode->Right;
            delNode->Data = child->Data;
            delNode->Key = child->Key;
            delNode->Right = nullptr;
            delete child;
            child = nullptr;
        }
        // ɾ���ڵ��������ӽڵ㣬�Ҷ�Ϊ��ɫ
        else if (delNode->Left != nullptr && delNode->Right != nullptr)
        {
            Node *sub = delNode->Right;
            while (sub->Left != nullptr)
            {
                sub = sub->Left;
            }
            DataType tempData = sub->Data;
            int tempkey = sub->Key;
            remove(sub->Key);
            delNode->Data = tempData;
            delNode->Key = tempkey;
        }
        root->Color = black;

        return true;
    }

    // �ҵ����ؽڵ���1���շ���nullptr��0��û�ҵ�����Ҫ����λ����-1
    pair<Node *, int> find(const int &key)
    {
        if (root == nullptr)
            return make_pair(nullptr, 0);
        Node *cur = root;
        while (cur)
        {
            if (cur->Key == key)
                return make_pair(cur, 1);
            else if (cur->Key < key)
            {
                if (cur->Right == nullptr)
                    break;
                cur = cur->Right;
            }
            else
            {
                if (cur->Left == nullptr)
                    break;
                cur = cur->Left;
            }
        }
        return make_pair(cur, -1);
    }

    void inorder_travel(Node *node)
    {
        if (node == nil)
            return;
        inorder_travel(node->Left);
        cout << node->Key << ":" << node->Color << endl;
        inorder_travel(node->Right);
    }

    Node *getroot()
    {
        return root;
    }

private:
    // Χ�ƽڵ�x����
    void leftRotate(Node *x)
    {
        Node *y = x->Right;
        x->Right = y->Left;
        if (x->Right != nil)
            x->Right->Parent = x;
        y->Parent = x->Parent;
        if (x->Parent == nil)
            root = y;
        else
        {
            if (x == x->Parent->Left)
                x->Parent->Left = y;
            if (x == x->Parent->Right)
                x->Parent->Right = y;
        }
        y->Left = x;
        x->Parent = y;
    }

    // Χ�ƽڵ�x����
    void rightRotate(Node *x)
    {
        Node *y = x->Left;
        x->Left = y->Right;
        if (y->Right != nil)
            y->Right->Parent = x;
        y->Parent = x->Parent;
        if (x->Parent == nil)
            root = y;
        else
        {
            if (x == x->Parent->Left)
                x->Parent->Left = y;
            if (x == x->Parent->Right)
                x->Parent->Right = y;
        }
        y->Right = x;
        x->Parent = y;
    }

private:
    Node *root; // ���ڵ�
    Node *nil;  // Ҷ�ӽڵ�
};

int main()
{
    RBTree<int> rbtree;
    rbtree.insert(20, 1);
    rbtree.insert(15, 1);
    rbtree.insert(30, 1);
    rbtree.insert(43, 1);
    rbtree.insert(22, 1);
    rbtree.insert(18, 1);
    rbtree.insert(8, 1);
    rbtree.insert(25, 1);
    rbtree.insert(6, 1);
    rbtree.insert(16, 1);
    rbtree.insert(7, 1);
    rbtree.insert(19, 1);
    rbtree.insert(17, 1);
    rbtree.insert(21, 1);
    rbtree.insert(24, 1);
    rbtree.insert(23, 1);
    rbtree.insert(34, 1);
    rbtree.insert(35, 1);
    rbtree.insert(45, 1);
    rbtree.inorder_travel(rbtree.getroot());
    // rbtree.remove(43);
    // rbtree.remove(18);
    // rbtree.inorder_travel(rbtree.getroot());
    return 0;
}