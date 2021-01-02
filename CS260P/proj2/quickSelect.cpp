#include"quickSelect.h"

int partition(vector<double>& nums, int lb, int rb, int k){
    if(k>rb-lb+1)
        return -1;
    
    if(lb==rb)
        return 1;

    //pick m
    double m = nums[lb];

    auto swap = [&](int a, int b){
        double temp = nums[b];
        nums[b] = nums[a];
        nums[a] = temp;
    };

    //partition elements into L E and G:
    int l = lb+1, r = rb;
    while(l<=r){
        while(nums[l]<=m && l<=r)
            l++;
        while(nums[r]>m && l<=r)
            r--;
        if(l<r)
            swap(l, r);
    }
    //when while loop ends, all the left of r <= m, right of r > m, so swap m to nums[r], this is garanteed!
    swap(lb, r);

    return r-lb+1;
}


double quickSelect(vector<double>& nums, int k){
    int mk = -1;
    int lb = 0, rb = nums.size()-1;

    while(-1 != (mk = partition(nums, lb, rb, k))){
        if(mk==k)
            return nums[mk+lb-1];
        else if(mk>k)
            rb = lb + mk + 2;
        else
            lb = lb + mk, k -= mk;        
    }

    return 0.f;
     
}