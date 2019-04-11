#include"JSON.hpp"
#include<string>
#include<iostream>
#include<cassert>
using namespace std;

void test_null()
{
    std::cerr << "entering test_null()\n";
    JSON a, b;
    b = JSON();
    JSON d = a;
    cout<<b<<endl;
    std::cerr << "leaving test_null()\n";
}

void test_string()
{
    JSON a("object");
    assert(a.type()==JSON::value_type::string);
    cout << a.toString() << endl;
}

void test_bool()
{
    
    bool True = true;
    bool False = false;
    JSON boolean(True);
    assert(boolean.type()==JSON::value_type::boolean);
    cout << boolean.toString() <<endl;
}
void test_number()
{
    std::cerr<<"entering test_number()\n";
    int a = 1;
    JSON J1(a);
    JSON J2 = J1;
    assert(J1.type()==JSON::value_type::number);
    std::cout << J2.toString()<<std::endl;
    std::cerr<<"leaving test_number()\n";
}

int main()
{
    test_null();
    test_string();
    test_bool();
    test_number();
    getchar();
}