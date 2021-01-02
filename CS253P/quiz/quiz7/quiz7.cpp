/************************************************* 
Author: Yuting Xie
Date:2020-12-3
Description: 253P QUIZ7 : The first missing number within n.
**************************************************/ 
#include<vector>
#include<cstdio>

using std::vector;

//find the missing item in 1-N
int find_missing_item(vector<int> num_list){
    int N = num_list.size()+1;
    bool isNMissing = true;

    //use num_list as a bit-map
    for(int num : num_list)
        if(num==N)
            isNMissing = false;
        else
            num_list[num-1] *= -1;
    
    if(isNMissing)
        return N;

    for(int i=0; i<num_list.size(); i++)
        if(num_list[i]>0)
            return i+1;

    return 0;
}

int main(){
    vector<int> nums = {4,2,3};
    printf("%d\n", find_missing_item(nums));
    return 0;
}