/************************************************* 
Author: Yuting Xie
Date:2020-10-23
Description: 253P QUIZ2 : find the longest concatenated palindrome string
**************************************************/ 

#include <iostream>
#include <vector>
#include <unordered_set>
#include <string>

using std::vector;
using std::string;
using std::unordered_set;

//Judge if 2 strings are mirroring in O(n) time.
bool isMirror(string& s1, string& s2, int L){
    for (auto i=0; i<L; i++)
        if(s1[i]!=s2[L-i-1])
            return false;
    return true;
}

//Judge if a string is palindrome in O(n) time.
bool isPal(string& s, int L){
    for (auto i=0; i<L/2; i++)
        if(s[i]!=s[L-i-1])
            return false;
    return true;
}

//First find a palindrome, then find mirroring string pairs, lastly concatenate them.
string longestPal(vector<string>& strs, int L){
    string res("");

    for(auto s : strs)
        if (isPal(s,L)){
            res = s;
            break;
        }

    // used stores those strings' indexes that have been paired
    unordered_set<int> used;
    for(int i=0 ; i<strs.size() && used.find(i)==used.end() ; i++){

        for (int j=i+1; j<strs.size();j++){

            if(used.find(j)==used.end() && isMirror(strs[i],strs[j],L)){
                res = strs[i]+res+strs[j];
                used.insert(i);
                used.insert(j);
            }
        }
    }
    
    return res;

}

int main(){
    
    vector<string> strs;

    strs.push_back("aaa");
    strs.push_back("xyz");
    strs.push_back("zyx");
    strs.push_back("bbb");
    strs.push_back("abc");
    strs.push_back("def");

    std::cout<<"result: "<<longestPal(strs,3)<<std::endl;

    return 0;
}

