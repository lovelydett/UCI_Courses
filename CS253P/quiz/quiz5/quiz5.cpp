/************************************************* 
Author: Yuting Xie
Date:2020-11-13
Description: 253P QUIZ4 : The sum of deepest leaves.
**************************************************/ 

#include<cstdio>
#include <queue>
using std::queue;

struct Node { 
    int key; 
    int info; 
    Node* left; 
    Node* right;
    Node(int _key, Node* l, Node* r):key(_key),left(l), right(r){};
};

//get the sum of deepest leaves
int sum_deepest_leaves(Node* root){
    if(!root)
        return 0;

    queue<Node*> q;
    q.push(root);
    Node* cur;
    int sum = 0;
    while(!q.empty()){
        sum = 0;
        int n = q.size();
        for(int i=0; i<n; i++){
            cur = q.front();
            q.pop();
            sum+=cur->key;
            if(cur->left) q.push(cur->left);
            if(cur->right) q.push(cur->right);
        }
    }

    return sum;
}

int main(){
    Node* llc = new Node(3, nullptr, nullptr);
    Node* lc = new Node(6, llc, nullptr);
    Node* rrc = new Node(10, nullptr, nullptr);
    Node* rc = new Node(9, nullptr, rrc);

    Node* root = new Node(1, lc, rc);

    printf("%d\n", sum_deepest_leaves(root));

    return 0;
}