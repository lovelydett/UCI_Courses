/************************************************* 
Author: Yuting Xie
Date:2020-10-19 
Description: 260P PROJECT1 : find the 1st, 2nd, 3rd... kth largest numbers' indexes
**************************************************/ 

void swap(int* a, int* b);
void constructMinHeap(int heap[], int size);
void siftDown(int heap[], int size, int pos);
//Since I only delete the root, no need to siftUp
//void siftUp(int heap[], int size, int pos);

int doalg(int n, int k, int Best[]){

    //1. Init the Best[] with value 1-k, corresponding to index i-k in COMPARE api. 
    for(int i=0; i<k ; i+=1)
        Best[i] = i+1; //use i+1 since in COMPARE index starts from 1

    //2. Construct min heap
    constructMinHeap(Best, k);

    //3. Orderly replace the root of heap with the rest n-k elements' indexes and adjust the min heap
    for(int i=k+1; i<=n; i+=1){
        if(1 == COMPARE(i,Best[0])){
            Best[0] = i;
            siftDown(Best, k, 0);
        }
    }

    //4. Order the results descendingly: swap the top to the back, heap size minus one, and adjust
    for(int i=k-1; i>=1; i-=1){
        swap(Best, Best+i); 
        siftDown(Best, i, 0);
    }

    return 1;
}

void swap(int* a, int* b){
    (*a)+=(*b);
    (*b)=(*a)-(*b);
    (*a)-=(*b);
}

void constructMinHeap(int heap[], int size){
    //only need to siftDown, since we will deal with fathers later
    for (int i=(size-2)/2; i>=0; i-=1)
        siftDown(heap, size, i);
}

void siftDown(int heap[], int size, int pos){
    //pos starts from 0
    int left = 2*pos+1, right = 2*(pos+1);

    //get smaller one
    int smaller = (right<size && 1 == COMPARE(heap[left], heap[right])) ? right : left;

    //recursively siftDown
    if(smaller<size && 1 == COMPARE(heap[pos],heap[smaller])){
        swap(heap+pos, heap+smaller);
        siftDown(heap, size, smaller);
    }

    //done
    return;

}