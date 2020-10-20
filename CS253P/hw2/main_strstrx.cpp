#include<iostream>
#include<string>
#include<vector>

//KMP for string compare
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

int strstrx(std::string& needle, std::string& haystack) {
    //use KMP algorithm

    //check edge case
    if (0 == needle.length() || haystack.length() < needle.length())
        return -1;

    //get next array
    std::vector<int> next = getNext(needle);

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


int main() {
    std::string needle, haystack;
    std::cout << "needle: ";
    std::cin >> needle;
    std::cout << "haystack: ";
    std::cin >> haystack;

    std::cout <<strstrx(needle, haystack)<<"\n";

    return 0;

}