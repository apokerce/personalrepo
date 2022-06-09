#include <iostream>
#include <vector>
#include <string>
#include <fstream>
using namespace std;

template <typename T>
void binary_converter(T x, std::vector<int>& temp)
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




void string_to_binary(std::string temp_str, std::vector<int>&temp2)
{
    // Fetch a char and convert to binary and store in the vector
    for(int i=temp_str.length();i>=0;i--)
    {
        binary_converter(int(temp_str[i]), temp2);
    }
}




// Adds zero to indicate message is not done
void add_zero(std::ofstream& out)
{
    out << "0";
}




// Adds one to indicate message is done
void add_one(std::ofstream& out)
{
    out << "1";
}




void output_the_encoded_message(std::ofstream& out, std::vector<int> encoded)
{
    int counter = 0;
    for(int i=(encoded.size()-1);i>=0;i--)
    {
        out << encoded[i];
        counter++;
	//Add 0 to indicate message continues for every 7 digit
        if(counter %7 == 0 & i!=0)
         {
             add_zero(out);
         }
    }
    // Indicates message end
    add_one(out);
}




void output_encoded_integer(std::ofstream& out, std::vector<int> encoded)
{
    add_zero(out);
    for(int i=(encoded.size()-1);i>=0;i--)
        out << encoded[i];
}

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
