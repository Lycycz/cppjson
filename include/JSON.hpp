#pragma once

#include <ciso646>
#include <initializer_list>
#include <iostream>
#include <map>
#include <string>
#include <vector>

//#define strcpy strcpy_s
//#define strncpy strncpy_s

class JSON {
public:
  class iterator;
  class const_iterator;
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
    value() = default;
    value(array_t*);
    value(object_t*);
    value(string_t*);
    value(boolean_t);
    value(number_t);
    value(number_float_t);
  };

public:
  JSON(const value_type) noexcept;
  JSON() = default;
  JSON(std::nullptr_t) noexcept;
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

  operator const std::string() const;
  operator int() const;
  operator double() const;
  operator bool() const;

  friend std::ostream &operator<<(std::ostream &o, const JSON &j) {
    o << j.toString();
    return o;
  }
  /// write to stream
  friend std::ostream &operator>>(const JSON &j, std::ostream &o) {
    o << j.toString();
    return o;
  }

  friend std::istream &operator>>(std::istream& i, JSON& j) {
    parser(i).parse(j);
    return i;
  }

  friend std::istream &operator<<(JSON& j, std::istream& i) {
    parser(i).parse(j);
    return i;
  }

  bool operator==(const JSON &) const;
  JSON &operator+=(const JSON &);
  JSON &operator+=(const std::string &);
  JSON &operator+=(const char *);

  JSON &operator[](int);
  const JSON &operator[](const int) const;
  JSON &operator[](const std::string &);
  JSON &operator[](const char *);
  const JSON &operator[](const std::string &) const;
  inline JSON& at(const std::string&);
  JSON& at(const char*);
  const JSON& at(const std::string&) const;
  value data() noexcept;
  const value data() const noexcept;
  size_t size() const;
  bool empty() const;

  void push_back(const JSON &);
  void push_back(JSON &&);

private:
  const std::string _typename() const noexcept;

private:
  value_type _type = value_type::null;
  value _value{};

public:
  class iterator {
    friend class JSON;
    friend class JSON::const_iterator;

  public:
    iterator();
    iterator(JSON *);
    iterator(const iterator &);
    ~iterator();

    iterator &operator=(const iterator &);
    bool operator==(const iterator &) const;
    bool operator!=(const iterator &) const;
    iterator &operator++();
    JSON &operator*() const;
    JSON *operator->() const;

  private:
    JSON *_object = nullptr;
    std::vector<JSON>::iterator *_vi = nullptr;
    std::map<std::string, JSON>::iterator *_oi = nullptr;
  };
  class const_iterator {
    friend class JSON;
  public:
    const_iterator();
    const_iterator(const JSON *);
    const_iterator(const const_iterator &);
    const_iterator(const iterator &);
    ~const_iterator();

    const_iterator &operator=(const const_iterator &);
    bool operator==(const const_iterator &) const;
    bool operator!=(const const_iterator &) const;
    const_iterator &operator++();
    const JSON &operator*() const;
    const JSON *operator->() const;

    /// getter for the key (in case of objects)
    std::string key() const;
    /// getter for the value
    const JSON &value() const;

  private:
    /// a JSON value
    const JSON *_object = nullptr;
    /// an iterator for JSON arrays
    std::vector<JSON>::const_iterator *_vi = nullptr;
    /// an iterator for JSON objects
    std::map<std::string, JSON>::const_iterator *_oi = nullptr;
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
    parser(char *);
    parser(std::string &);
    parser(std::istream &);
    ~parser();
    void parse(JSON &s);

  private:
    bool next();
    void error(std::string = " ");
    std::string parseString();
    void parseTrue();
    void parseFalse();
    void parseNull();
    void expect(char);

    char _current {};
    char* _buffer { nullptr };
    size_t _length {};
    size_t _pos = 0;
  };
};
