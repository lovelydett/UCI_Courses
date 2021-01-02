/*************************************************
Author: Yuting Xie
Date:2020-11-23
Description: 253P HW8.1 Paying in coins
**************************************************/

#include<vector>
#include<climits>
#include<cstdio>
#include<iostream>
using std::vector;

vector<int> paying_in_coins(vector<int> nums, int amount) {
    vector<int> coins{ 1, 5, 10, 25 };

    vector<int> dp(amount + 1, -1);
    vector<vector<int>> lefts(amount + 1, vector<int>(coins.size(), 0));
    dp[0] = 0;
    lefts[0] = nums;

    printf("  ");
    for (int n : nums)
        printf("%d ", n);
    printf("\n");

    for (int i = 1; i <= amount; i++) {
        int m = -1;
        vector<int> left(coins.size(), 0);
        for (int j = 0; j < coins.size(); j++) {
            int c = coins[j];

            if (i<c || dp[i - c] == -1 || lefts[i - c][j] == 0)
                continue;

            if (dp[i - c] > m) {
                left = lefts[i - c];
                left[j]--;
                m = dp[i - c] + 1;
            }

        }
        dp[i] = m;
        lefts[i] = left;
    }

    printf("- ");
    for (int i = 0; i < nums.size(); i++)
        printf("%d ", dp[amount]==-1?0:nums[i] - lefts[amount][i]);

    if (dp[amount] == -1)
        printf("(cannot pay %d)", amount);

    printf("\n");
    printf("-------------------\n");

    return dp[amount] == -1 ? nums : lefts[amount];

}

int main() {
    vector<int> nums(4,0);
    for (int i = 0; i < 4; i++)
        std::cin >> nums[i];

    for (int i = 0; i < 4; i++) {
        int amount;
        std::cin >> amount;
        nums = paying_in_coins(nums, amount);

    }
        
    return 0;
}