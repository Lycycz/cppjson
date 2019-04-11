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
JSON::JSON(const bool b) noexcept
    :_type(value_type::boolean)
{
    _value.boolean = b;
}
JSON::JSON(const int i) noexcept
    : _type(value_type::number)
{
    _value.number = i;
}
JSON::JSON(const double f) noexcept
    :_type(value_type::number_float)
{
    _value.number_float = f;
}
JSON::JSON(const array_t& a) noexcept
    : _type(value_type::array)
{
    _value.array = new array_t(a);
}
JSON::JSON(array_t&& a ) noexcept
    :_type(value_type::array)
{
    _value.array = new array_t(std::move(a));
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

JSON& JSON::operator+=(const JSON& o)
{
    push_back(o);
    return *this;
}
JSON& JSON::operator+=(const std::string& s)
{
    push_back(JSON(s));
    return *this;
}
JSON& JSON::operator+=(const char*s)
{
    push_back(JSON(s));
    return *this;
}
void JSON::push_back(const JSON& o)
{
    if(not(_type == value_type::array or _type == value_type::null))
    {
        throw std::runtime_error("cannot add element to" + _typename());
    }

    if (_type == value_type::null)
    {
        _type = value_type::array;
        _value.array = new array_t;
    }

    _value.array->push_back(o);
}
void JSON::push_back(JSON&& o)
{
    if(not(_type == value_type::array or _type == value_type::null))
    {
        throw std::runtime_error("cannot add element to" + _typename());
    }

    if (_type == value_type::null)
    {
        _type = value_type::array;
        _value.array = new array_t;
    }

    _value.array->emplace_back(std::move(o));
}
const std::string JSON::_typename() const noexcept
{
    switch (_type)
    {
    case value_type::array:
        return "array";
    case value_type::object:
        return "object";
    case value_type::null:
        return "null";
    case value_type::boolean:
        return "boolean";
    case value_type::number:
        return "number";
    case value_type::number_float:
        return "number";
    }   
}