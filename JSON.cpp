#include "JSON.hpp"
#include <iostream>

JSON::JSON(const std::string& s) noexcept
    : _type(value_type::string)
{
    _value.string = new string_t(s);
}
JSON::JSON(std::string&& s) noexcept
    : _type(value_type::string)
{
    _value.string = new string_t(std::move(s));
}
JSON::JSON(const char* s) noexcept
    : _type(value_type::string)
{
    _value.string = new string_t(s);
}
JSON::JSON(const int i) noexcept
    : _type(value_type::number)
{
    _value.number = i;
}
JSON::JSON(const bool b) noexcept
    :_type(value_type::boolean)
{
    _value.boolean = b;
}
JSON::value_type JSON::type() const noexcept
{
    return _type;
}
const std::string JSON::toString() const noexcept
{
    switch (_type)
    {
        case value_type::null:
        {
            return "null";
        }
        case value_type::string:
        {
            return "\"" + *_value.string + "\"";
        }
        case value_type::number:
        {
            return std::to_string(_value.number);
        }
        case value_type::number_float:
        {
            return std::to_string(_value.number_float);
        }
        case value_type::boolean:
        {
            return _value.boolean?"true":"false";
        }
    }
}

JSON& JSON::operator=(JSON o) noexcept
{
    std::swap(_type,o._type);
    std::swap(_value,o._value);
    return *this;
}
