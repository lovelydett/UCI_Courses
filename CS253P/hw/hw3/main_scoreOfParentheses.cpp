/*************************************************
Author: Yuting Xie
Date:2020-10-23
Description: for 253P HW3: Leetcode 856. Score of Parentheses    
**************************************************/

#include <cstdio>

#include <string>
#include <unordered_map>
using std::string;
using std::unordered_map;

int recur(string& S, int lb, int rb){
    //must be "()"
    if (lb+1 == rb)
        return 1;

    unordered_map<int, int> partitions;
    int l=lb, r, count=0;
    while(l<rb){
        r=l;
        while(r<=rb){
            count+= ( S[r]=='('?1:-1 ) ;
            if(0==count){
                partitions[l] = r;
                break;
            }
            r++;
        }
        l=r+1;
    }

    if (1 == partitions.size())
        return 2 * recur(S, lb + 1, rb - 1);

    int sum = 0;
    for (auto it : partitions) {
        sum += recur(S, it.first, it.second);
    }

    return sum;
}

int scoreOfParentheses(string S) {
    return recur(S, 0, S.length()-1);
}

int main(){
    string S("((())()(()()))");
    printf("%d\n", scoreOfParentheses(S));
    return 0;
}