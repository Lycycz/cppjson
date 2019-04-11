#pragma once

#include<iostream>
#include<initializer_list>
#include<map>
#include<string>
#include<vector>

class JSON
{
public:
    enum class value_type : uint8_t
    {
        array = 0,
        object,
        null,
        string,
        boolean,
        number,
        number_float
    };
    using object_t = std::map<std::string, JSON>;
    using array_t = std::vector<JSON>;
    using string_t = std::string;
    using boolean_t = bool;
    using number_t = int;
    using number_float_t = double;
    using list_init_t = std::initializer_list<JSON>;
    union value
    {
        array_t *array;
        object_t *object;
        string_t *string;
        boolean_t boolean;
        number_t number;
        number_float_t number_float;
    };
public:
    JSON() = default;
    JSON(const std::string&) noexcept;
    JSON(std::string &&) noexcept;
    JSON(const char *) noexcept;
    JSON(const int) noexcept;
    JSON(const bool) noexcept;

    JSON& operator=(JSON) noexcept;
    value_type type() const noexcept;
    const std::string toString() const noexcept;

    friend std::ostream& operator<<(std::ostream& o, const JSON& j)
    {
        o << j.toString();
        return o;
    }
    /// write to stream
    friend std::ostream& operator>>(const JSON& j, std::ostream& o)
    {
        o << j.toString();
        return o;
    }
private:
  value_type _type = value_type::null;
  value _value{};
};