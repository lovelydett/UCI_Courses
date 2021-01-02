#include "dSelect.h"
#include<algorithm>

int partition(vector<double>& nums, int lb, int rb, int k, int groupSize){
    if(k>rb-lb+1)
        return -1;
    
    //base case: sub-array has less than 50 elements
    if(rb-lb+1 <= 50){
        std::sort(nums.begin()+lb, nums.begin()+rb+1);
        return k;
    }
        
    //other cases we have to CAREFULLY PICK m:
    //1. divide into n groups by size:
    int n = (rb-lb+1)/groupSize;

    //2. get their mideans respectively:
    std::vector<double> mideans(n, 0.f);
    for(int i=0; i<n; i++){
        std::sort(nums.begin()+lb+i*groupSize, nums.begin()+lb+(i+1)*groupSize);
        mideans[i] = nums[lb+i*groupSize+groupSize/2];
    }

    //3. get the midean of mideans by dSelect recursively
    double m = dSelect(mideans, n/2, groupSize);

    //4. swap the first number with m:
    int mIndex = lb;
    while(true){
        if(nums[mIndex]==m)
            break;
        mIndex++;
    }

    auto swap = [&](int a, int b){
        double temp = nums[b];
        nums[b] = nums[a];
        nums[a] = temp;
    };

    swap(lb, mIndex);

    //5. use m to partition elements into L E and G:
    int l = lb+1, r = rb;
    while(l<=r){
        while(nums[l]<=m && l<=r)
            l++;
        while(nums[r]>m && l<=r)
            r--;
        if(l<r)
            swap(l, r);
    }

    //6. when while loop ends, all the left of r <= m, right of r > m, so swap m to nums[r], this is garanteed!
    swap(lb, r);

    return r-lb+1;
}

double dSelect(vector<double>& nums, int k, int groupSize){
    int mk = -1;
    int lb = 0, rb = nums.size()-1;

    while(-1 != (mk = partition(nums, lb, rb, k, groupSize))){
        if(mk==k)
            return nums[mk+lb-1];
        else if(mk>k)
            rb = lb + mk + 2;
        else
            lb = lb + mk, k -= mk;        
    }

    return 0.f;
}