/*************************************************
Author: Yuting Xie
Date:2020-12-11
Description: 253P QUIZ9 : Find the smallest n whose digits multiply to given number A
**************************************************/

#include<vector>
#include<algorithm>
#include<cmath>
#include<iostream>
#include<cassert>

using std::vector;
using std::cout;

//get all the factors in range [2,9] for A
void getFactors(int A, vector<int>& res) {
    if (A == 1)
        return;

    for (int i = 9; i >= 2; i--)
        if (A % i == 0) {
            res.push_back(i);
            getFactors(A / i, res);
            return;
        }
    
    res.clear();
}

//build the number for the given vector of single digits
int buildInteger(vector<int>& nums) {
    int sum = 0;
    for (int i = 0; i < nums.size(); i++)
        sum += nums[i] * pow(10, i);

    return sum;
}

//Find the smallest integer whose digits multiply to a given number A
int find_cool_number(int A) {

    vector<int> factors;
    getFactors(A, factors);

    if (factors.empty())
        return -1;

    return buildInteger(factors);
}

int main() {
    vector<int> nums{ 36, 49, 100, 255 };
    for (int n : nums)
        cout << "given number:" << n << "\n" << "result:" << find_cool_number(n) << "\n";

    return 0;
}