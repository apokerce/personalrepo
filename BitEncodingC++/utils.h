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
