#include <stdio.h>


int isValid(int heap[], int size){
    for(int i=0;i<(size-2)/2;i+=1){
        if(2*i+1<size && heap[2*i+1]<heap[i])
            return 0;
        if(2*i+2<size && heap[2*i+2]<heap[i])
            return 0;
    }
    return 1;
}

void printNums(int nums[], int size){
    printf("\n");
    for(int i=0; i<size; i+=1){
        printf("%d\t", nums[i]);
    }
    printf("\n valid = %d\n",isValid(nums, size));
}

void swap(int* a, int* b){
    (*a)+=(*b);
    (*b)=(*a)-(*b);
    (*a)-=(*b);
}

void siftDown(int heap[], int size, int pos){
    //pos starts from 0
    int left = 2*pos+1, right = 2*(pos+1);

    //get smaller one
    int smaller = (right<size && heap[right]<heap[left]) ? right : left;

    //recursively siftDown
    if(smaller<size && heap[pos]>heap[smaller]){
        swap(heap+pos, heap+smaller);
        siftDown(heap, size, smaller);
    }

    //done
    return;

}

void constructMinHeap(int heap[], int size){
    //only need to siftDown, since we will deal with fathers later
    for (int i=(size-2)/2; i>=0; i-=1)
        siftDown(heap, size, i);
}


int main(){
    int heap[10]={3,6,12,5,9,8,2,0,33,7};
    constructMinHeap(heap, 10);

    //4. Order the results descendingly
    for(int i=9; i>=1; i-=1){
        swap(heap, heap+i);
        siftDown(heap, i, 0);
    }

    printNums(heap, 10);
    return 0;

}