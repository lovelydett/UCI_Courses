#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

static std::vector<std::vector<int>> nexts;

void getAllNext(const std::vector<std::string>& strs) {
    nexts.resize(strs.size());

    auto getNext = [&](const std::string& s, unsigned index) {
        if (0 == s.size())
            return;
        nexts[index].resize(s.size());
        nexts[index][0] = -1;           //while the first char not match, i++,j++
        unsigned j = 0;
        unsigned k = -1;
        while (j < s.length() - 1){
            if (k == -1 || s[j] == s[k]){
                j++;
                k++;
                nexts[index][j] = k;
            }
            else
                k = nexts[index][k];
        }
    };

    for (unsigned i = 0; i < strs.size(); i++) {
        getNext(strs[i], i);
    }

}

//index is for s
int strstr(const std::string& s, const std::string& t, unsigned index) {
    if (!s.length() || !t.length())
        return -1;

    unsigned i = 0, j = 0;
    while (i < s.length() && j < t.length()) {
        if (j == -1 || s[i] == t[j])
        {
            i++;
            j++;
        }
        else
        {
            j = nexts[index][j];
        }
    }
    if (j == (int)t.length()) {
        return i - j;
    }
    return -1;

}

std::vector<std::vector<unsigned>> func(std::vector<std::string> &strs) {

    std::vector<std::vector<unsigned>> res;
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
    auto copyRes = [](std::vector<unsigned>& dst, std::vector<unsigned>& src) {
        for (auto each : src)
            dst.push_back(each);
    };
    for (unsigned shortIndex = 1; shortIndex < strs.size(); shortIndex++) {
        for (unsigned longIndex = 0; longIndex < shortIndex; longIndex++) {
            if (-1 != strstr(strs[longIndex], strs[shortIndex], longIndex)) {
                //matched
                copyRes(res[shortIndex], res[longIndex]);
                res[shortIndex].push_back(longIndex);
            }
        }
    }

    //5. deal with the 1st longest string in the end
    for (unsigned i = 1; i < strs.size() && strs[i].length() == strs[0].length(); i++) {
        if (-1 != strstr(strs[0], strs[i], 0))
            res[0].push_back(i);
    }
  
    return res;

}

int main() {
    std::vector<std::string> data;
    data.push_back("abcdefg");
    data.push_back("bcdef");
    data.push_back("cdefg");
    data.push_back("acdeg");
    data.push_back("ab");
    data.push_back("de");
    data.push_back("cd");
    data.push_back("a");

    data.push_back("");
    data.push_back("abcdefg");
    data.push_back("bcdef");
    //data.push_back("xxxx");

    std::vector<std::vector<unsigned>>&& res = func(data);

    for (unsigned i = 0; i < res.size(); i++) {
        std::cout << data[i] << ": ";
        for (auto each : res[i])
            std::cout << data[each] << " , ";
        std::cout << std::endl;
    }

    return 0;

}