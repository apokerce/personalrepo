#include <iostream>
#include <vector>
#include <string>
using namespace std;

struct values{
    int a = 65;
    string b = "ahmet";
    long long c = 191246843;
};

template <typename T>
void binary_converter(T x, vector<int>& temp)
{
    int remain, binary_form[7] , product = 1, counter = 0;
    // Binary represantation is simply dividing by two and storing remainders, remainders stored in vector
    while(x!=0)
    {
        remain = x%2;
        x = x / 2;
        temp.push_back(remain);
    }
}
void string_to_binary(string temp_str, vector<int>&temp2)
{
    // Fetch a char and convert to binary and store in the vector
    for(int i=temp_str.length();i>=0;i--)
    {
        binary_converter(int(temp_str[i]), temp2);
    }
}
int main()
{
    values obj;
    vector <int> integer,str, longvec;
    
    binary_converter(obj.a, integer);
    string_to_binary(obj.b, str);
    binary_converter<int>(obj.c, longvec);
    
    cout << "0";
    int counter = 0;
    // No boundary check is made assumed binary represantation of numbers are 7 digit or its multiples
    for(int i=(integer.size()-1);i>=0;i--)
    {
        cout << integer[i];
    }
    cout <<"1";
    for(int i=(str.size()-1);i>=0;i--)
    {
        cout << str[i];
        counter++;
	//Add 0 to indicate message continues for every 7 digit
        if(counter %7 == 0 & i!=0)
            cout << "0";
    }
    cout <<"1";
    for(int i=(longvec.size()-1);i>=0;i--)
    {
        cout << longvec[i];
        counter++;
	//Add 0 to indicate message continues for every 7 digit
        if(counter %7 == 0 & i!=0)
            cout << "0";
    }

    return 0;
}
