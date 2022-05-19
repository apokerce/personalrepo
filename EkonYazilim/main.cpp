#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include "utils.h"
using namespace std;

struct values{
    int a;
    string b;
    long long c;
};

int main()
{
    values obj;
    vector <int> integer,str, longvec;
    ifstream input("input.txt");
    ofstream output("output.txt");

    try
    {    if(!output) throw 98;    }
    catch(int i)
    {    cout << "Cannot create file" << endl;  }
    try
    {
        if(!(input.is_open()))  throw 99;
        input >> obj.a;
        input >> obj.b;
        input >> obj.c;
    }
    catch(int i)
    {   cout << "Cannot read file" << endl;   }
    // Close the read file
    input.close();

    // Convert the variables to encoded binary
    binary_converter(obj.a, integer);
    string_to_binary(obj.b, str);
    binary_converter<int>(obj.c, longvec);

    // No boundary check is made assumed binary represantation of numbers are 7 digit or its multiples
    output_encoded_integer(output, integer);
    output_the_encoded_message(output, str);
    output_the_encoded_message(output, longvec);

    // Close output file
    output.close();
    return 0;
}
