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

//First find mirroring string pairs, then find a palindrome in the middle, last concatenate them.
string longestPal(vector<string>& strs, int L){
    string res("");
    unordered_set<int> used;//stores strings' indexes been paired

    for(int i=0 ; i<strs.size() ; i++){
        if (used.find(i)!=used.end())
            continue;
        for (int j=i+1; j<strs.size();j++){
            if(used.find(j)==used.end() && isMirror(strs[i],strs[j],L)){
                res = strs[i]+res+strs[j];
                used.insert(i);
                used.insert(j);
            }
        }
    }

    string mid;
    for(int i=0 ; i<strs.size(); i++)
        if (used.find(i)==used.end() && isPal(strs[i],L)){
            mid = strs[i]; //find a palindrome as middle
            break;
        }

    //concanate them
    res = res.substr(0,(res.length())/2)+mid+res.substr(res.length()/2,res.length()/2); 
    return res;
}

int main(){
    
    vector<string> strs;

    // strs.push_back("aaa");
    // strs.push_back("aaa");
    strs.push_back("bbb");
    strs.push_back("xyz");
    //strs.push_back("zyx");
    strs.push_back("def");

    std::cout<<"result: "<<longestPal(strs,3)<<std::endl;

    return 0;
}

