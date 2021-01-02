/*************************************************
Author: Yuting Xie
Date:2020-12-8
Description: for 260P PROJ2: Select
**************************************************/

#include<cstdio>
#include<ctime>
#include<cstdlib>
#include<vector>
#include<algorithm>
#include "dSelect.h"
#include "quickSelect.h"

using std::vector;

double testQuickSelect(vector<double>& nums){

    //quick select:
    int N = nums.size();
    printf("QuickSelect:\t");

    //set up timer
    clock_t start = clock();

    double res = quickSelect(nums, N/2);
    if(nums.size()%2==0)
        res = (res + quickSelect(nums, N/2 + 1))/2;

    //end timer
    double runTime = double(clock()-start) / CLOCKS_PER_SEC;

    printf("midean: %.2f, process time: %.2f ms\n", res, runTime*1000);

    return runTime;
    
}

double testDSelect(vector<double>& nums, int groupSize){

    //quick select:
    int N = nums.size();
    printf("DSelect(%d):\t", groupSize);

    //set up timer
    clock_t start = clock();

    double res = dSelect(nums, N/2, groupSize);
    if(nums.size()%2==0)
        res = (res + dSelect(nums, N/2 + 1, groupSize))/2;

    //end timer
    double runTime = double(clock()-start) / CLOCKS_PER_SEC;

    printf("midean: %.2f, process time: %.2f ms\n", res, runTime*1000);

    return runTime;
    
}

void doTest(int dataSize){
    printf("doing test for %d numbers\n", dataSize);
    //generate data:
    vector<double> nums(dataSize, 0.);
    for(int i=0; i<nums.size(); i++){
        nums[i] = (double)rand()/(double)rand();
    }

    //test qs 
    testQuickSelect(nums);

    //test ds with size 5
    testDSelect(nums, 5);

    //test ds with size 3
    testDSelect(nums, 3);

    //test ds with size 7
    testDSelect(nums, 7);


    //if the numbers are sorted:
    printf("(Now we SORT the data)\n");
    std::sort(nums.begin(), nums.end());

    //test qs 
    testQuickSelect(nums);

    //test ds with size 5
    testDSelect(nums, 5);

    //test ds with size 3
    testDSelect(nums, 3);

    //test ds with size 7
    testDSelect(nums, 7);

    printf("-------------------------\n\n");
}

int main(){
    //set random seed
    srand(time(nullptr));

    //current data size
    for(int i=1000; i<=500000; i*=5)
        doTest(i);

    return 0;
}
