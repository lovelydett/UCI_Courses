/************************************************* 
Author: tt
Date:2020-10-19 
Description: for problem3 in 253P HW2 
**************************************************/ 

#include<iostream>
#include<string>
#include<vector>
#include<cstring> //for memset

std::vector<int> anagram(std::string& needle, std::string& haystack) {
    std::vector<int> res;
    if (0 == needle.length() || needle.length() > haystack.length())
        return res;

    // for 26 non-cap letters
    // comparation of these 2 array is O(26)
    int needleMap[26], haystackMap[26];
    
    //init 2 map arrays
    auto initMap = [&]() {
        memset(needleMap, 0, sizeof(needleMap));
        memset(haystackMap, 0, sizeof(haystackMap));
        for (auto ch : needle)
            needleMap[ch - 97] += 1;
        std::string tmp;
        tmp.assign(haystack, 0, needle.length());
        for (auto ch : tmp)
            haystackMap[ch - 97] += 1;
    };

    //refresh haystack map array during traverse
    auto refreshMap = [&](char prev, char next) {
        haystackMap[prev - 97]--;
        haystackMap[next - 97]++;
    };

    //compare 2 map arrays
    auto isSame = [&]() ->bool {
        for (int i = 0; i < 26; i++)
            if (needleMap[i] != haystackMap[i])
                return false;
        return true;
    };
    
    //main steps:
    initMap();
    int i = 0, j = needle.length() - 1;
    while (1) {

        //first judge whether find one anagram
        if (isSame())
            res.push_back(i);

        //second check whether we reach the haystack end
        if (j == haystack.length() - 1)
            break;
        
        //if not, refresh map, minus one i++ and add one ++j
        refreshMap(haystack[i++], haystack[++j]);
    }
    
    return res;
}

int main() {
    std::string needle, haystack;
    std::cout << "needle: ";
    std::cin >> needle;
    std::cout << "haystack: ";
    std::cin >> haystack;

    auto res = anagram(needle, haystack);
    if (0 == res.size())
       std::cout << 0<<"\n";
    for (auto num : res)
       std::cout << num << " ";

    std::cout<<"\n";
    return 0;

}