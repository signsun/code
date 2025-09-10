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
        DataType Data;      // 数据
        int Color;          // 节点颜色
        int Key;            // 关键字
        Node *Left, *Right; // 左右孩子
        Node *Parent;       // 父节点指针

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
        // 树为空直接插入
        if (root == nullptr)
        {
            Node *newNode = new Node;
            newNode->Color = black;
            newNode->Data = data;
            newNode->Key = key;
            root = newNode;
            return true;
        }

        // 判断返回值
        auto ret = find(key);
        Node *cur = ret.first; // 返回的节点
        // 如果存在key，更新其数据
        if (ret.second == 1)
        {
            cur->Data = data;
            return true;
        }

        Node *newNode = new Node;
        newNode->Data = data;
        newNode->Key = key;
        // 父节点是黑色，直接插入
        if (cur->Color == black)
        {

            insertKey(cur, newNode);
            return true;
        }
        // 父节点是红色
        else
        {
            Node *grandParent = cur->Parent;
            // 父节点是祖父结点的左子节点
            if (cur == grandParent->Left)
            {
                // 父节点没有兄弟节点或者兄弟节点是黑色
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
                // 叔叔节点颜色是红色
                // 让父节点和叔叔节点颜色变为黑色，祖父节点颜色变为红色
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
                // 叔叔节点颜色是红色
                // 让父节点和叔叔节点颜色变为黑色，祖父节点颜色变为红色
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
            cout << "数据不存在" << endl;
            return false;
        }

        // 要删除的节点
        Node *delNode = ret.first;
        ret.first = nullptr;
        Node *par = delNode->Parent;
        // 要删除的节点是叶子节点
        if (delNode->Left == nullptr && delNode->Right == nullptr)
        {
            // 要删除的叶子节点是红色，直接删除
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
            // 要删除节点D为黑色
            else
            {
                // 要删除的节点是其父节点的左子节点
                if (delNode == par->Left)
                {
                    Node *brother = par->Right;
                    // 兄弟节点没有左右孩子
                    if (brother->Left == nullptr && brother->Right == nullptr)
                    {
                        par->Left = nullptr;
                        delete delNode;
                        delNode = nullptr;
                        par->Color = black;
                        brother->Color = red;
                    }
                    // 兄弟节点有左孩子，右孩子为空
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
                    // 兄弟节点有右孩子，左孩子为空
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
                    // 兄弟节点左右孩子都存在，且为红色
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
                    // 兄弟节点为红色
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
                // 要删除的节点是其父节点的右子节点
                else
                {
                    Node *brother = par->Left;
                    // 兄弟节点没有左右孩子
                    if (brother->Left == nullptr && brother->Right == nullptr)
                    {
                        par->Right = nullptr;
                        delete delNode;
                        delNode = nullptr;
                        par->Color = black;
                        brother->Color = red;
                    }
                    // 兄弟节点有左孩子，右孩子为空
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
                    // 兄弟节点有右孩子，左孩子为空
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
                    // 兄弟节点左右孩子都存在，且为红色
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
                    // 兄弟节点为红色
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
        // 删除节点只有左孩子，没有右孩子
        else if (delNode->Left != nullptr && delNode->Right == nullptr)
        {
            Node *child = delNode->Left;
            delNode->Data = child->Data;
            delNode->Key = child->Key;
            delNode->Left = nullptr;
            delete child;
            child = nullptr;
        }
        // 删除节点只有右孩子，没有左孩子
        else if (delNode->Left == nullptr && delNode->Right != nullptr)
        {
            Node *child = delNode->Right;
            delNode->Data = child->Data;
            delNode->Key = child->Key;
            delNode->Right = nullptr;
            delete child;
            child = nullptr;
        }
        // 删除节点有左右子节点，且都为红色
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

    // 找到返回节点与1，空返回nullptr与0，没找到返回要插入位置与-1
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
    // 围绕节点x左旋
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

    // 围绕节点x右旋
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
    Node *root; // 根节点
    Node *nil;  // 叶子节点
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