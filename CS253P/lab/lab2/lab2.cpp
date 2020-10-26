#include <iostream>
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
#include <unordered_set>
#include <algorithm>

static std::vector<std::vector<int>> nexts;

void getAllNext(const std::vector<std::string>& strs) {
    nexts.resize(strs.size());

    auto getNext = [&](const std::string& s, int index) {
        if (0 == s.length())
            return;

        nexts[index].resize(s.length());
        nexts[index][0] = -1;           //while the first char not match, i++,j++
        int i = 0, j = -1;
        while (i < s.length() - 1){
            if (j == -1 || s[i] == s[j]){
                i++;
                j++;
                nexts[index][i] = j;
            }
            else
                j = nexts[index][j];
        }
    };

    for (int i = 0; i < strs.size(); i++) {
        getNext(strs[i], i);
    }

}

//index is for s
int strstr(const std::string& s, const std::string& t, int index) {
    if (!s.length() || !t.length())
        return -1;

    int i = 0, j = 0;
    while (i < (int)s.length() && j < (int)t.length()) {
        if (j == -1 || s[i] == t[j]){
            i++;
            j++;
        }
        else{
            j = nexts[index][j];
        }
    }
    if (j == (int)t.length()) {
        return i - j;
    }
    return -1;

}

std::vector<std::unordered_set<int>> func(std::vector<std::string> &strs) {

    std::vector<std::unordered_set<int>> res;
    res.resize(strs.size());

    //1. check edge cases
    if (1 >= strs.size())
        return res;

    //2. sort string list by length in descending order (self-defines functor)
    struct CmpByLength {
        bool operator()(const std::string& s1, const std::string& s2) const {
            return s1.length() > s2.length();
        }
    };
    std::sort(strs.begin(), strs.end(), CmpByLength());

    //3. get the next array in KMP algorithm for each string.
    getAllNext(strs);

    //4. for each string, compare it with those longer than it, if matched, use their result
    auto copyRes = [](std::unordered_set<int>& dst, std::unordered_set<int>& src) {
        for (auto each : src)
            if(dst.find(each)==dst.end())
                dst.insert(each);
    };
    for (int shortIndex = 1; shortIndex < strs.size(); shortIndex++) {
        for (int longIndex = 0; longIndex < shortIndex; longIndex++) {

            //already in unordered_set
            if (res[shortIndex].find(longIndex) != res[shortIndex].end())
                continue;

            if (-1 != strstr(strs[longIndex], strs[shortIndex], shortIndex)) {
                //matched
                copyRes(res[shortIndex], res[longIndex]);
                res[shortIndex].insert(longIndex);
            }
        }
    }

    //5. deal with the 1st longest string in the end
    for (int i = 1; i < strs.size() && strs[i].length() == strs[0].length(); i++) {
        if (-1 != strstr(strs[0], strs[i], 0))
            res[0].insert(i);
    }
  
    return res;

}

int main() {
    std::vector<std::string> data;
    char buffer[1024];
    auto split = [](char* s){
        for (int i = 0; i<strlen(s); i++){
            if('\t' == s[i] || '\n' == s[i]){
                s[i] = '\0';
                return;
            }
        }
    };

    std::cout<< "input:\n";
    while(1){
        printf("\t");

        memset(buffer,0,1024);
        fgets(buffer, 1024, stdin);
        split(buffer);
        if (0==strlen(buffer))
            break;
        
        data.push_back(std::string(buffer));
        
    }

    std::vector<std::unordered_set<int>>&& res = func(data);

    std::cout<<"output:\n";
    for (int i = 0; i < res.size(); i++) {
        std::cout<<"\t";
        std::cout << data[i] << ": ";
        for (auto each : res[i])
            std::cout << data[each] << " , ";
        std::cout << std::endl;
    }

    return 0;

}