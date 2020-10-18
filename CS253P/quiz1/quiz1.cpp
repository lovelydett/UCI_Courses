#include <iostream>
#include <string>

std::string foo(std::string s1, std::string s2){
    //empty string
    if (0==s1.length() || 0==s2.length())
        return std::string("");
    auto gcd = [](unsigned a, unsigned b)->unsigned {
        if (a < b) {
            a = a+b, b = a-b, a = a-b;
        }
        unsigned tmp;
        while (b != 0) {
            tmp = a % b, a = b, b = tmp;
        }
        return a;
    };
    auto isDevided = [](std::string s1, std::string s2)->bool {
        for(unsigned i = 0;i<s1.length()/s2.length();i++){
            for (unsigned j = 0; j<s2.length();j++)
                if(s1[i*s2.length()+j] != s2[j])
                    return false;
        }
        return true;
    };
    auto isValid = [&](unsigned i)->bool {
        if(s1.length()%i!=0 || s2.length()%i != 0 || s1.substr(0,i)!=s2.substr(0,i))
            return false;
        return isDevided(s1, s1.substr(0,i)) && isDevided(s2, s1.substr(0,i));
    };

    //start try from gcd
    for (unsigned i = gcd(s1.length(),s2.length()); i > 0;i--){
        if (isValid(i))
            return s1.substr(0,i);
    }
    return "";

}

int main(){
    std::string s1, s2;
    std::cin>>s1;
    std::cin>>s2;
    std::cout<<foo(s1,s2);
    return 0;

}
