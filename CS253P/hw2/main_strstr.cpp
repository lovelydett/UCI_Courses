#include<iostream>
#include<string>

//BF for string compare
int strstr(std::string& needle, std::string& haystack) {
    if (0 == needle.length() || needle.length() > haystack.length())
        return -1;

    int i = 0, j = 0, matched = 0;
    while(i <= haystack.length()-needle.length() && j < needle.length()) {
        if (haystack[i+matched] == needle[j]) {
            j++, matched++;
        }
        else {
            i++;
            matched=0;
            j = 0;
        }
    }

    if (needle.length() == j)
        return i;

    return -1;
}

int main() {
    std::string needle, haystack;
    std::cout << "needle: ";
    std::cin >> needle;
    std::cout << "haystack: ";
    std::cin >> haystack;

    std::cout <<strstr(needle, haystack)<<"\n";

    return 0;

}