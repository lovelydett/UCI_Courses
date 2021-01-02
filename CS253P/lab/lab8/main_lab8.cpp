/*************************************************
Author: Yuting Xie
Date:2020-11-27
Description: for 253P LAB8: Truck Delivery
**************************************************/

#include <cstdio>
#include <stack>
#include <vector>
#include <cmath>

using std::vector;
using std::stack;

int recurTest(vector<int>& positions, vector<int>& values, vector<int>& time, int curPos, int curTime, int task, stack<int>& stk, vector<int>& res, int &revenue){
    //check whether current task can be tested: 
    int timeNeeded = std::abs(positions[task]-curPos)/100;
    if(curTime+timeNeeded<=time[task]){
        //we can go there before due
        //try pick it:
        
    }
}