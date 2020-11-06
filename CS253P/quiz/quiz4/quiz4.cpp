/************************************************* 
Author: Yuting Xie
Date:2020-10-29
Description: 253P QUIZ4 : delete the ith node from back of a linked single list.
**************************************************/ 
#include<cstdio>


struct  linked_list{
    int val;
    linked_list* next;
    linked_list(int _val, linked_list* _next):val(_val), next(_next){};
};

//remove ith node from back, assume list has >= i nodes
linked_list* remove_ith_element(linked_list* head,int i){
    if(!head || !(head->next))
        return head;

    linked_list* fast = head;
    linked_list* dummy = new linked_list(-1, head), *slow = dummy;

    //fast pointer go i-1 steps first
    for(int n = 1; n<=i-1; n++)
        fast=fast->next;
    
    //fast and slow move simultaneously until fast reaches end
    while(fast->next)
        fast = fast->next, slow = slow->next;

    //now slow->next is which to remove
    linked_list* tmp = slow->next;
    slow->next = slow->next->next;

    //if remove origin head:
    if(tmp == head)
        head = head->next;

    delete(tmp);
    delete(dummy);

    return head;

}


int main(){
    linked_list *n1 = new linked_list(2, nullptr);
    linked_list *n2 = new linked_list(5, n1);
    linked_list *n3 = new linked_list(8, n2);
    linked_list *n4 = new linked_list(1, n3);
    linked_list *n5 = new linked_list(0, n4);
    linked_list *head = new linked_list(2, n5);

    auto printList = [](linked_list *head){
        linked_list* cur = head;
        while(cur){
            printf("%d->", cur->val);
            cur = cur->next;
        }
        printf("\n");
    };

    printList(head);
    printList(remove_ith_element(head, 6));

    return 0;



}