/*************************************************
Author: Yuting Xie
Date:2020-11-9
Description: 253P HW5.1 Implement AVL Tree
**************************************************/

#include <cstdio>
#include <iostream>
#include <queue>
#include <cassert>
#include <string>

using std::queue;
using std::string;

//balance factor
#define BF (1)

typedef struct TreeNode {
    TreeNode* left;
    TreeNode* right;
    TreeNode* parent;
    int val;
    int height;

    TreeNode(int _val, TreeNode* _parent)
        :val(_val),
        left(nullptr),
        right(nullptr),
        parent(_parent),
        height(0)
    {};
    bool isBalance() {
        int l = left ? left->height+1 : 0;
        int r = right ? right->height+1 : 0;
        return std::abs(l - r) <= BF;
    }
    void updateHeight() {
        if (left && right)
            height = std::max(left->height, right->height) + 1;

        else if (left)
            height = left->height + 1;

        else if (right)
            height = right->height + 1;

        else
            height = 0;
    }
} Node;

class AVL {
public:
    void readCmd();

private:
    Node* root = nullptr;

    void insert(int value);
    bool remove(int value);
    Node* search(int value);
    void printTree();

    void LL(Node* node, bool isLeftChildOfParent);
    void LR(Node* node, bool isLeftChildOfParent);
    void RR(Node* node, bool isLeftChildOfParent);
    void RL(Node* node, bool isLeftChildOfParent);
};

inline void myFlush() {
    char ch;
    while ((ch = getchar()) != '\n' && ch != EOF);
}

void AVL::readCmd() {
    string cmd;
    int value;
    while (true) {
        std::getline(std::cin, cmd, ' ');
        if (cmd == "print") {
            printTree();
        }
        else if (cmd == "insert") {
            std::cin >> value;
            insert(value);
            printf("(inserted)\n");
        }
        else if (cmd == "delete") {
            std::cin >> value;
            if (remove(value))
                printf("(deleted)\n");
            else
                printf("(not found)\n");
        }
        else if (cmd == "find") {
            std::cin >> value;
            if (nullptr != search(value))
                printf("(found)\n");
            else
                printf("(not found)\n");
        }
        else if (cmd == "exit")
            break;

        printf("\n");
        myFlush();
    }

}

//insert a value into the tree
void AVL::insert(int value) {
    if (!root) {
        root = new Node(value, nullptr);
        return;
    }

    Node* cur = root, * prev = cur;
    bool isLeft;
    while (cur) {
        printf("%d ", cur->val);
        prev = cur;
        if (value <= cur->val) {
            isLeft = true;
            cur = cur->left;
        }
        else {
            isLeft = false;
            cur = cur->right;
        }

    }

    //prev is the parent of the inserted node
    Node* newNode = new Node(value, prev);
    (isLeft ? prev->left : prev->right) = newNode;

    //update the height of the nodes that along the path in inverse order of searching:
    cur = newNode;
    while (cur) {
        cur->updateHeight();
        cur = cur->parent;
    }

    //adjust the tree back to balance
    cur = newNode;
    while (cur) {
        if (!cur->isBalance()) {
            //first determine is left sub-tree or right sub-tree
            int l = cur->left ? cur->left->height+1 : 0;
            int r = cur->right ? cur->right->height+1 : 0;
            bool isLeftTree = l > r;
            Node* child = isLeftTree ? cur->left : cur->right;

            //second determine is left child or right child
            l = child->left ? child->left->height+1 : 0;
            r = child->right ? child->right->height+1 : 0;
            bool isLeftChild = l > r;
            bool isLeftChildOfParent = cur->parent && cur->parent->left == cur;

            //according to the unbalance type, adjust the tree back to balanced
            if (isLeftTree && isLeftChild)
                LL(cur, isLeftChildOfParent);
            else if (isLeftTree && !isLeftChild)
                LR(cur, isLeftChildOfParent);
            else if (!isLeftTree && !isLeftChild)
                RR(cur, isLeftChildOfParent);
            else if (!isLeftTree && isLeftChild)
                RL(cur, isLeftChildOfParent);
        }

        cur = cur->parent;
    }


}

//first search for, then remove a element with value in the tree
bool AVL::remove(int value) {
    Node* node = search(value);
    if (!node)
        return false;

    Node* parent = node->parent;
    bool isLeftChildOfParent = true;
    if (parent)
        isLeftChildOfParent = parent->left == node;

    //deleting leaf node
    if (!node->left && !node->right) {
        if (parent)
            (isLeftChildOfParent ? parent->left : parent->right) = nullptr;
        else
            root = nullptr;
    }

    //node has no left child
    else if (!node->left) {
        if (parent)
            (isLeftChildOfParent ? parent->left : parent->right) = node->right;
        else
            root = node->right;

        node->right->parent = parent;

    }

    //node has no right child
    else if (!node->right) {
        if (parent)
            (isLeftChildOfParent ? parent->left : parent->right) = node->left;
        else
            root = node->left;

        node->left->parent = parent;
    }

    //has both left and right children, replace with predecessor
    else {
        Node* lChild = node->left, * predecessor = lChild;
        while (predecessor->right) {
            predecessor = predecessor->right;
        }

        if (predecessor == lChild) {
            if (parent)
                (isLeftChildOfParent ? parent->left : parent->right) = lChild;
            else
                root = lChild;

            lChild->parent = parent;
            lChild->right = node->right;
            node->right->parent = lChild;
        }
        else {
            predecessor->parent->right = nullptr;
            node->val = predecessor->val;
            node = predecessor;
            parent = predecessor->parent;
        }
    }

    //delete the node
    delete(node);

    //update heights on the path
    Node* cur = parent;
    while (cur) {
        cur->updateHeight();
        cur = cur->parent;
    }

    //adjust:
    cur = parent;
    while (cur) {
        if (!cur->isBalance()) {
            //first determine is left sub-tree or right sub-tree
            int l = cur->left ? cur->left->height+1 : 0;
            int r = cur->right ? cur->right->height+1 : 0;
            bool isLeftTree = l > r;
            Node* child = isLeftTree ? cur->left : cur->right;

            //second determine is left child or right child
            l = child->left ? child->left->height+1 : 0;
            r = child->right ? child->right->height+1 : 0;
            bool isLeftChild = l > r;
            bool isLeftChildOfParent = cur->parent && cur->parent->left == cur;

            //according to the unbalance type, adjust the tree back to balanced
            if (isLeftTree && isLeftChild)
                LL(cur, isLeftChildOfParent);
            else if (isLeftTree && !isLeftChild)
                LR(cur, isLeftChildOfParent);
            else if (!isLeftTree && !isLeftChild)
                RR(cur, isLeftChildOfParent);
            else if (!isLeftTree && isLeftChild)
                RL(cur, isLeftChildOfParent);
        }

        cur = cur->parent;
    }

    return true;

}

//search a value in the tree and show the searching-path
Node* AVL::search(int value) {
    Node* cur = root;
    while (cur) {
        printf("%d ", cur->val);
        if (value == cur->val) {
            return cur;
        }
        else
            cur = value < cur->val ? cur->left : cur->right;
    }

    return nullptr;
}

//print the tree in level order
void AVL::printTree() {
    printf("Tree:\n");
    if (!root) {
        printf("null\n");
        return;
    }


    queue<Node*> q;
    q.push(root);
    Node* tmp;
    while (!q.empty()) {
        int n = q.size();
        for (int i = 0; i < n; i++) {
            tmp = q.front();
            q.pop();
            printf("%d(%d) ", tmp->val, tmp->height);
            if (tmp->left) q.push(tmp->left);
            if (tmp->right)q.push(tmp->right);
        }
        printf("\n");
    }
}

void AVL::LL(Node* node, bool isLeftChildOfParent) {
    Node* A = node, * B = A->left, * BR = B->right;
    Node* parent = A->parent;

    assert(B != nullptr);

    A->left = BR;
    B->right = A;
    A->parent = B;
    B->parent = parent;

    if (BR)
        BR->parent = A;

    if (parent)
        (isLeftChildOfParent ? parent->left : parent->right) = B;

    //update heights of affected nodes;
    A->updateHeight();
    B->updateHeight();
    if (parent)
        parent->updateHeight();

    if (A == root)
        root = B;

}

void AVL::RR(Node* node, bool isLeftChildOfParent) {
    Node* A = node, * B = A->right, * BL = B->left;
    Node* parent = A->parent;

    assert(B != nullptr);

    A->right = BL;
    B->left = A;
    A->parent = B;
    B->parent = parent;

    if (BL)
        BL->parent = A;

    if (parent)
        (isLeftChildOfParent ? parent->left : parent->right) = B;

    //update heights of affected nodes;
    A->updateHeight();
    B->updateHeight();
    if (parent)
        parent->updateHeight();

    if (A == root)
        root = B;

}

void AVL::LR(Node* node, bool isLeftChildOfParent) {
    Node* A = node, * B = A->left, * C = B->right;

    assert(A != nullptr);
    assert(B != nullptr);
    assert(C != nullptr);

    //first do RR on B C
    RR(B, true);

    //second do LL on A C
    LL(A, isLeftChildOfParent);
}

void AVL::RL(Node* node, bool isLeftChildOfParent) {
    Node* A = node, * B = A->right, * C = B->left;

    assert(A != nullptr);
    assert(B != nullptr);
    assert(C != nullptr);

    //first do LL on B C
    LL(B, false); //false cuz B is the right child of A

    //second do RR on A C
    RR(A, isLeftChildOfParent);

}

int main() {
    AVL tree;
    tree.readCmd();

    return 0;
}

/*
insert 50
insert 25
insert 10
insert 5
insert 7
insert 3
insert 30
insert 20
insert 8
insert 15
find 10
find 12
delete 4
delete 20
find 22
delete 50
find 30
delete 10
find 7
*/