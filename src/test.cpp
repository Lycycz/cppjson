#include "JSON.hpp"
#include <cassert>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

using namespace std;

void test_null()
{
    std::cerr << "entering test_null()\n";
    JSON a, b;
    b = JSON();
    JSON c(a);
    JSON d = a;

    assert(a == b);

    assert(a.type() == JSON::value_type::null);

    assert(a.size() == 0);
    assert(a.empty() == true);

    cout << b << endl;

    assert(a.toString() == std::string("null"));

    try {
        int i = 0;
        i = a;
        assert(false);
    } catch (const std::exception& ex) {
        assert(ex.what() == std::string("cannot cast null to JSON number "));
    }


    std::cerr << "leaving test_null()\n";
}

void test_string()
{
    JSON a("object");
    assert(a.type() == JSON::value_type::string);
    cout << a.toString() << endl;
}

void test_bool()
{

    bool True = true;
    bool False = false;
    JSON boolean(True);
    assert(boolean.type() == JSON::value_type::boolean);
    cout << boolean.toString() << endl;
}
void test_number()
{
    std::cerr << "entering test_number()\n";
    int a = 1;
    JSON J1(a);
    JSON J2 = J1;
    assert(J1.type() == JSON::value_type::number);
    std::cout << J2.toString() << std::endl;
    std::cerr << "leaving test_number()\n";
}

void test_array()
{
    std::cerr << "entering test_array()\n";

    JSON a;
    a += JSON();
    a += 1;
    a += 1.0;
    a += true;
    a += "string";

    assert(a.type() == JSON::value_type::array);

    assert(a.size() == 5);
    assert(a.empty() == false);

    assert(a[1] == JSON(1));
    assert(a[2] == JSON(1.0));
    assert(a[3] == JSON(true));
    assert(a[4] == JSON("string"));

    for (JSON::iterator i = a.begin(); i != a.end(); ++i)
    {
        std::cerr << *i << '\n';
    }

    for (JSON::const_iterator i = a.cbegin(); i != a.cend(); ++i)
    {
        std::cerr << *i << '\n';
    }

    std::cerr << "leaving test_array()\n";
}

void test_streaming() {
    std::stringstream i;

    i << "{ \"foo\": false, \"baz\": [1,2,3,4] }";

    {
        JSON j,k;
        i >> j;
        k << i;
        assert(j.toString() == k.toString());
    }

    {
        std::stringstream number_stream;
        number_stream << "[0, -1, 1, 1.0, -1.0, 1.0e+1, 1.0e-1, 1.0E+1, 1.0E-1, -1.2345678e-12]";
        JSON j;
        j << number_stream;
        std::cout << j.toString() << std::endl;
    }

    {
        std::stringstream unicode_stream;
        unicode_stream << "[\"öäüÖÄÜß\", \"ÀÁÂÃĀĂȦ\", \"★☆→➠♥︎♦︎☁︎\"]";
        JSON j;
        j << unicode_stream;
        std::cout << j.toString() << std::endl;
    }
}
int main()
{
    // test_null();
    // test_string();
    // test_bool();
    // test_number();
    // test_array();
    test_streaming();
    getchar();
}
