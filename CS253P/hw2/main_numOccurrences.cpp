/************************************************* 
Author: tt
Date:2020-10-19 
Description: for problem2 in 253P HW2 
**************************************************/ 

#include<iostream>
#include<vector>
#include<string>

std::vector<int> getNext(std::string& str) {
    std::vector<int> next;
    if (0 == str.length())
        return next;

    next.resize(str.length());
    next[0] = -1;

    int i = 0, j = -1;
    while (i < str.length() - 1) {
        if (-1 == j || str[i] == str[j]) {
            i++, j++;
            next[i] = j;
        }
        else {
            j = next[j];
        }
    }

    return next;
}

int KMP(std::string& needle, std::string& haystack, std::vector<int>& next) {

    if (0 == needle.length() || haystack.length() < needle.length())
        return -1;

    int i = 0, j = 0;
    while (i < (int)haystack.length() && j < (int)needle.length()) {
        if (-1 == j || haystack[i] == needle[j])
            i++, j++;
        else 
            j = next[j];
    }
    if (j ==(int)needle.length())
        return i-j;

    return -1;
}

int numOccurrences(std::string& needle, std::string& haystack) {

    if (0==needle.length() || needle.length() > haystack.length())
        return 0;

    //pre-calculate the NEXT array which will be repeatedly used in later comparisons
    std::vector<int>&& next = getNext(needle);

    //find and count ocurrances one-by-one
    int count = 0, index = 0;
    while (1) {
        //find the next index where haystack[index] == needle[0]
        while(index<haystack.length() && haystack[index]!=needle[0])
            index++;

        if(index==haystack.length()) 
            break;
        
        haystack.assign(haystack, index, haystack.length() - index);
        if(-1 == (index = KMP(needle, haystack, next)))
            break;
        else{
            count++, index++;
        }
    }

    return count;

}


int main() {
    std::string needle, haystack;
    std::cout << "needle: ";
    std::cin >> needle;
    std::cout << "haystack: ";
    std::cin >> haystack;

    std::cout <<numOccurrences(needle, haystack)<<"\n";

    return 0;

}