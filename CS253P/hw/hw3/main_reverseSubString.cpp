/*************************************************
Author: Yuting Xie
Date:2020-10-23
Description: for 253P HW3: Leetcode 1190. Reverse Substrings Between Each Pair of Parentheses    
**************************************************/

#include <cstdio>
#include <stack>
#include <string>
#include <algorithm> //for std::reverse

using std::stack;
using std::string;

string reverseParentheses(string s) {
    //use a stack to store indexes for '(' s
    stack<int> index;
    for(int i=0; i<s.length(); i++){
        if('(' == s[i])
            index.push(i);
        else if(')'==s[i]){
            int l = index.top();
            index.pop();
            std::reverse(s.begin()+l, s.begin()+i);
        }
    }
    
    string ans;
    for(auto& ch : s)
        if(ch!='('&&ch!=')')
            ans+=ch;
    
    return ans;
}

int main(){
    string s("(u(love)i)");
    printf("%s\n", reverseParentheses(s).c_str());

    return 0;
}