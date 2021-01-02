/*************************************************
Author: Yuting Xie
Date:2020-11-23
Description: 253P HW8.3 DNA Sequence Alignment
**************************************************/


#include<string>
#include<vector>
#include<algorithm>
#include<cassert>
#include<iostream>

using std::pair;
using std::make_pair;
using std::vector;
using std::string;

bool isMatch(char a, char b){
    if(a=='A')
        return b=='T';

    if(a=='T')
        return b=='A';

    if(a=='C')
        return b=='G';
    
    if(a=='G')
        return b=='C';
    return false;
}

void dna_align(string s1, string s2){
    int m = s1.length(), n = s2.length();

    vector<vector<int>> dp(m+1, vector<int>(n+1, 0));
    
    for(int i=1; i<=m; i++)
        dp[i][0] = i;
    for(int i=1; i<=n; i++)
        dp[0][i] = i;

    for(int i=1; i<=m; i++){
        for(int j=1; j<=n; j++){
            if(isMatch(s1[i-1], s2[j-1])){
                dp[i][j] = dp[i-1][j-1];
                continue;
            }

            int a = dp[i-1][j]+1; //add "_" in s1[i]
            int b = dp[i][j-1]+1; //add "_" in s2[j]
            dp[i][j] = std::min(a,b);
        }
    }

    // for(auto each : dp){
    //     for(auto n : each){
    //         printf("%d ", n);
    //     }

    //     printf("\n");
    // }

    string res1, res2("_");
    int i=m, j=n;
    while(i>0&&j>0){
        if(dp[i][j]==dp[i-1][j-1]){
            res1.push_back(s1[i-1]);
            res2.push_back(s2[j-1]);
            i--, j--;
        }
        else if(dp[i][j]==dp[i-1][j]+1){
            res1.push_back(s1[i-1]);
            res1.push_back('_');
            i--;
        }
        else if(dp[i][j]==dp[i][j-1]+1){
            res2.push_back(s2[j-1]);
            res2.push_back('_');
            j--;
        }
        else
            assert(false);
    }
    while(i>0)
        res1.push_back(s1[--i]);

    while(j>0)
        res2.push_back(s2[--j]);
    

    std::reverse(res1.begin(), res1.end());
    std::reverse(res2.begin(), res2.end());

    printf("%s\n", res1.c_str());
    printf("%s\n", res2.c_str());

}

int main(){
    string s1,s2;
    std::cin>>s1>>s2;
    dna_align(s1, s2);
}