/************************************************* 
Author: Yuting Xie
Date:2020-10-29
Description: 253P QUIZ3 : find the needed numbers for open parentheses and brackets.
**************************************************/ 

#include<cstdio>
#include<vector>
#include<string>
#include<stack>

using std::vector;
using std::string;
using std::stack;

bool isMatch(char left, char right){
    return left=='('&&right==')' || left=='['&&right==']' ;
}

vector<int> checkParens(string S){
    stack<char> stk;
    int needLP=0, needRP=0, needLB=0, needRB=0;
    for (auto ch:S){
        switch(ch){
            case '(':
            case '[':
                stk.push(ch);
                break;

            case ']':            
            case ')':
                if(stk.empty() || !isMatch(stk.top(),ch))
                    ch==')' ? needLP++ : needLB++;
                else
                    stk.pop();
                break; 

            default:
                break;
        }
    }

    while(!stk.empty()){
        stk.top()=='(' ? needRP++ : needRB++;
        stk.pop();
    }

    vector<int> res;
    res.push_back(needLP);
    res.push_back(needRP);
    res.push_back(needLB);
    res.push_back(needRB);

    return res;

}

int main(){
    // for (auto i : checkParens(string("( hello [ my [ (name is (Ray) ] ] ]"))){
    //     printf("%d ",i);
    // }

    for (auto i : checkParens(string(")))(((]]][[["))){
        printf("%d ",i);
    }
    printf("\n");
    return 0;
}