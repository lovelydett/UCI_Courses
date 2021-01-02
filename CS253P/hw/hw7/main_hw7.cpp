/*************************************************
Author: Yuting Xie
Date:2020-11-23
Description: 253P HW7.1 Roman Numeral Converter
**************************************************/

#include<iostream>
#include<string>
#include<sstream>

using std::string;
using std::cout;
using std::cin;
using std::stringstream;

int romanToInt(string s) {
    int sum=0;
    for(int i=0; i<s.length(); i++){
        switch(s[i]){
            case 'I':
                if(i<s.length()-1 && s[i+1]=='V')
                    sum+=4, i++;
                else if(i<s.length()-1 && s[i+1]=='X')
                    sum+=9, i++;
                else
                    sum+=1;
                break;
            case 'V':
                sum+=5;
                break;
            case 'X':
                if(i<s.length()-1 && s[i+1]=='L')
                    sum+=40, i++;
                else if(i<s.length()-1 && s[i+1]=='C')
                    sum+=90, i++;
                else
                    sum+=10;
                break;
            case 'L':
                sum+=50;
                break;
            case 'C':
                if(i<s.length()-1 && s[i+1]=='D')
                    sum+=400, i++;
                else if(i<s.length()-1 && s[i+1]=='M')
                    sum+=900, i++;
                else
                    sum+=100;
                break;
            case 'D':
                sum+=500;
                break;
            case 'M':
                sum+=1000;
                break;
            default:
                sum+=0;
        }
    }

    return sum;
}

string intToRoman(int num) 
    {
        int values[]={1000,900,500,400,100,90,50,40,10,9,5,4,1};
        string reps[]={"M","CM","D","CD","C","XC","L","XL","X","IX","V","IV","I"};  
        string res;
        for(int i=0; i<13; i++)
        {
            while(num>=values[i])
            {
                num -= values[i];
                res += reps[i];
            }
        }
        return res;
}

int main(){
    string input;
    stringstream str2digit;
    int num;
    while(true){
        cin>>input;
        if(input=="exit")
            return 0;
        if(input.size()<1)
            continue;
        if(input[0]>='0' && input[0]<='9'){
            str2digit<<input;
            str2digit>>num;
            cout<<intToRoman(num)<<"\n";
            str2digit.clear();
        }
        else
            cout<<romanToInt(input)<<"\n";
        input.clear();
    }
    return 0;
}