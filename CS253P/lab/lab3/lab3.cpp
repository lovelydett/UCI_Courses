#include <cstdio>
#include <string>
#include <stack>
#include <vector>
#include <iostream>

using std::string;
using std::stack;
using std::vector;

bool isMatch(char a, char b){
    switch(a){
        case '(':
            return b==')';
        case '[':
            return b==']';
        case '{':
            return b=='}';
    }
    return false;
}

bool getNested(string& S, vector<string>& res){
    res.clear();
    res.push_back("");
    
    int layer = 0;
    stack<int> index;

    for(int i=0; i<S.length(); i++){
        switch(S[i]){
            case '(':
            case '[':
            case '{':
                index.push(i);
                layer++;
                res.push_back(""); //initialize an empty string for this layer
                break;

            case ')':
            case ']':
            case '}':
                if (isMatch(S[index.top()], S[i])){
                    layer--;
                    index.pop();
                }else{
                    return false;
                }
                break;

            default:
                res[layer]+=S[i];
                break;
            
        }
    }

    return true;

}

void printNested(vector<string>& res){
    for(int i=0; i<res.size(); i++){
        if (0==res[i].length())
            continue;
        for (int j=0; j<i; j++)
            printf("  ");
        printf("%s\n", res[i].c_str());
    }
}


int main(){
    string S;
    vector<string> res;
    
    std::cout<<"Input: ";
    std::cin>>S;

    if (!getNested(S, res))
        printf("mismatched groups!\n");
    else
        printNested(res);

    return 0;
    
}