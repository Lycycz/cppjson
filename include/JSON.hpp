#pragma once

#include <ciso646>
#include <initializer_list>
#include <iostream>
#include <map>
#include <string>
#include <vector>

class JSON {
public:
  enum class value_type : uint8_t {
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
  union value {
    array_t *array;
    object_t *object;
    string_t *string;
    boolean_t boolean;
    number_t number;
    number_float_t number_float;
  };

public:
  JSON() = default;
  JSON(const std::string &) noexcept;
  JSON(std::string &&) noexcept;
  JSON(const char *) noexcept;
  JSON(const bool) noexcept;
  JSON(const int) noexcept;
  JSON(const double) noexcept;
  JSON(const array_t &) noexcept;
  JSON(array_t &&) noexcept;

  JSON &operator=(JSON) noexcept;
  value_type type() const noexcept;
  const std::string toString() const noexcept;

  friend std::ostream &operator<<(std::ostream &o, const JSON &j) {
    o << j.toString();
    return o;
  }
  /// write to stream
  friend std::ostream &operator>>(const JSON &j, std::ostream &o) {
    o << j.toString();
    return o;
  }

  JSON &operator+=(const JSON &);
  JSON &operator+=(const std::string &);
  JSON &operator+=(const char *);

  void push_back(const JSON &);
  void push_back(JSON &&);

private:
  const std::string _typename() const noexcept;

private:
  value_type _type = value_type::null;
  value _value{};

  void *_payload;

public:
  class const_iterator;

public:
  class iterator {
    friend class JSON::const_iterator;

  public:
    iterator();
    iterator(JSON *);
    iterator(const iterator &);
    ~iterator();

  private:
    JSON *_object;
    std::vector<JSON>::iterator *_vi;
    std::map<std::string, JSON>::iterator *_oi;
  };

public:
    iterator begin();
    iterator end();
    const_iterator begin() const;
    const_iterator end() const;
    const_iterator cbegin() const;
    const_iterator cend() const;

private:
    class parser {
    public:
        parser(char*);
        parser(std::string&);
        parser(std::istream&);
    private:
        bool next();

        char _current;
        char *_buffer;
        size_t _pos;
    };
};
