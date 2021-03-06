#include "JSON.hpp"
#include <iostream>

JSON::value::value(array_t* _array): array(_array) {}
JSON::value::value(object_t* _object): object(_object) {}
JSON::value::value(string_t* _string): string(_string) {}
JSON::value::value(boolean_t _boolean): boolean(_boolean) {}
JSON::value::value(number_t _number): number(_number) {}
JSON::value::value(number_float_t _number_float): number_float(_number_float) {}
JSON::JSON(const value_type t) noexcept 
  : _type(t) {
  switch (_type) {
    case (value_type::array):
    {
      _value.array = new array_t();
      break;
    }
    case (value_type::object):
    {
      _value.object = new object_t();
      break;
    }
    case (value_type::string):
    {
      _value.string = new string_t();
      break;
    }
    case (value_type::boolean):
    {
      _value.boolean = boolean_t();
      break;
    }
    case (value_type::number):
    {
      _value.number = number_t();
      break;
    }
    case (value_type::number_float):
    {
      _value.number_float = number_float_t();
      break;
    }
    case (value_type::null):
    {
      break;
    }
  }    
}

JSON::JSON(std::nullptr_t) noexcept : JSON()
{}

JSON::JSON(const std::string &s) noexcept 
  : _type(value_type::string), _value(new string_t(s)) {
}

JSON::JSON(std::string &&s) noexcept 
  : _type(value_type::string), _value(new string_t(std::move(s))) {
}

JSON::JSON(const char *s) noexcept 
  : _type(value_type::string), _value(new string_t(s)) {
}

JSON::JSON(const bool b) noexcept 
  : _type(value_type::boolean), _value(b) {
}

JSON::JSON(const int i) noexcept 
  : _type(value_type::number), _value(i) {
}

JSON::JSON(const double f) noexcept 
  : _type(value_type::number_float), _value(f) {
}

JSON::JSON(const array_t &a) noexcept 
  : _type(value_type::array), _value(new array_t(a)) {
}

JSON::JSON(array_t &&a) noexcept 
  : _type(value_type::array), _value(new array_t(std::move(a))) {
}

JSON::value_type JSON::type() const noexcept { return _type; }

const std::string JSON::toString() const noexcept {
  switch (_type) {
  case value_type::null: {
    return "null";
  }
  case value_type::string: {
    return "\"" + *_value.string + "\"";
  }
  case value_type::number: {
    return std::to_string(_value.number);
  }
  case value_type::number_float: {
    return std::to_string(_value.number_float);
  }
  case value_type::boolean: {
    return _value.boolean ? "true" : "false";
  }
  case value_type::array: {
    std::string result;
    for (array_t::const_iterator i = _value.array->begin();
         i != _value.array->end(); i++) {
      if (i != _value.array->begin())
        result += ", ";
      result += i->toString();
    }

    return "[" + result + "]";
  }
  case value_type::object: {
    std::string result;
    for (object_t::const_iterator i = _value.object->begin();
         i != _value.object->end(); i++) {
      if (i != _value.object->begin())
        result += ", ";
      result += "\"" + i->first + "\": " + i->second.toString();
    }
    return "{" + result + "}";
  }
  }
}

JSON &JSON::operator=(JSON o) noexcept {
  std::swap(_type, o._type);
  std::swap(_value, o._value);
  return *this;
}

JSON::operator const std::string() const {
  switch (_type) {
  case (value_type::string):
    return *static_cast<std::string *>(_value.string);
  default:
    throw std::runtime_error("cannot cast " + _typename() + " to JSON string ");
  }
}

JSON::operator int() const {
  switch (_type) {
  case value_type::number:
    return static_cast<int>(_value.number);
  case value_type::number_float:
    return static_cast<int>(_value.number_float);
  default:
    throw std::runtime_error("cannot cast " + _typename() + " to JSON number ");
  }
}

JSON::operator double() const {
  switch (_type) {
  case value_type::number:
    return static_cast<double>(_value.number);
  case value_type::number_float:
    return static_cast<double>(_value.number_float);
  default:
    throw std::runtime_error("cannot cast " + _typename() + " to JSON number ");
  }
}

JSON::operator bool() const {
  switch (_type) {
  case value_type::boolean:
    return _value.boolean;
  default:
    throw std::runtime_error("cannot cast " + _typename() + " to JSON bool ");
  }
}

JSON &JSON::operator+=(const JSON &o) {
  push_back(o);
  return *this;
}
JSON &JSON::operator+=(const std::string &s) {
  push_back(JSON(s));
  return *this;
}
JSON &JSON::operator+=(const char *s) {
  push_back(JSON(s));
  return *this;
}

JSON &JSON::operator[](int index) {
  std::vector<JSON> *array = static_cast<std::vector<JSON> *>(_value.array);
  return array->at(index);
}

const JSON &JSON::operator[](const int index) const {
  std::vector<JSON> *array = static_cast<std::vector<JSON> *>(_value.array);
  return array->at(index);
}

JSON &JSON::operator[](const std::string &key) {
  if (_type == value_type::null) {
    _type = value_type::object;
    _value.object = new std::map<std::string, JSON>;
  }

  if (_type != value_type::object) {
    throw std::runtime_error("cannot add entry with key " + std::string(key) +
                             " to " + _typename());
  }
  std::map<std::string, JSON> *object =
      static_cast<std::map<std::string, JSON> *>(_value.object);
  if (object->find(key) == object->end())
    (*object)[key] = JSON();

  return (*object)[key];
}

JSON &JSON::operator[](const char *key) {
  if (_type == value_type::null) {
    _type = value_type::object;
    _value.object = new std::map<std::string, JSON>;
  }

  if (_type != value_type::object) {
    throw std::runtime_error("cannot add entry with key " + std::string(key) +
                             " to " + _typename());
  }
  std::map<std::string, JSON> *object =
      static_cast<std::map<std::string, JSON> *>(_value.object);
  if (object->find(key) == object->end())
    (*object)[key] = JSON();

  return (*object)[key];
}

const JSON &JSON::operator[](const std::string &key) const {
  if (_type != value_type::object) {
    throw std::runtime_error("cannot add entry with key " + std::string(key) +
                             " to " + _typename());
  }
  std::map<std::string, JSON> *object =
      static_cast<std::map<std::string, JSON> *>(_value.object);
  if (object->find(key) == object->end()) {
    throw std::out_of_range("key " + key + "not found");
  } else {
    return object->find(key)->second;
  }
}

JSON &JSON::at(const std::string &key) {
  return at(key.c_str());
}

JSON &JSON::at(const char *key) {
  if (_type != value_type::object) {
    throw std::logic_error("cannot add entry with key " + std::string(key) +
                             " to " + _typename());
  }
  return _value.object->at(key);
}

const JSON &JSON::at(const std::string &key) const {
  if (_type != value_type::object) {
    throw std::logic_error("cannot get entry with key " + std::string(key) +
                             " to " + _typename());
  }
  return _value.object->at(key);
}

JSON::value JSON::data() noexcept {
  return _value;
}

const JSON::value JSON::data() const noexcept {
  return _value;
}

void JSON::push_back(const JSON &o) {
  if (not(_type == value_type::array or _type == value_type::null)) {
    throw std::runtime_error("cannot add element to" + _typename());
  }

  if (_type == value_type::null) {
    _type = value_type::array;
    _value.array = new array_t;
  }

  _value.array->push_back(o);
}

void JSON::push_back(JSON &&o) {
  if (not(_type == value_type::array or _type == value_type::null)) {
    throw std::runtime_error("cannot add element to" + _typename());
  }

  if (_type == value_type::null) {
    _type = value_type::array;
    _value.array = new array_t;
  }

  _value.array->emplace_back(std::move(o));
}

const std::string JSON::_typename() const noexcept {
  switch (_type) {
  case value_type::array:
    return "array";
  case value_type::object:
    return "object";
  case value_type::null:
    return "null";
  case value_type::string:
    return "string";
  case value_type::boolean:
    return "boolean";
  case value_type::number:
    return "number";
  case value_type::number_float:
    return "number";
  }
}

size_t JSON::size() const {
  switch (_type) {
  case value_type::array:
    return static_cast<std::vector<JSON> *>(_value.array)->size();
  case value_type::object:
    return static_cast<std::map<std::string, JSON> *>(_value.object)->size();
  case value_type::null:
    return 0;
  case value_type::string:
    return 1;
  case value_type::boolean:
    return 1;
  case value_type::number:
    return 1;
  case value_type::number_float:
    return 1;
  }
}

bool JSON::empty() const {
  switch (_type) {
  case value_type::array:
    return static_cast<std::vector<JSON> *>(_value.array)->empty();
  case value_type::object:
    return static_cast<std::map<std::string, JSON> *>(_value.object)->empty();
  case value_type::null:
    return true;
  case value_type::string:
    return false;
  case value_type::boolean:
    return false;
  case value_type::number:
    return false;
  case value_type::number_float:
    return false;
  }
}

bool JSON::operator==(const JSON &o) const {
  switch (_type) {
  case (value_type::array): {
    if (o._type == value_type::array) {
      std::vector<JSON> *a = static_cast<std::vector<JSON> *>(_value.array);
      std::vector<JSON> *b = static_cast<std::vector<JSON> *>(o._value.array);
      return *a == *b;
    }
  }
  case (value_type::object): {
    if (o._type == value_type::object) {
      std::map<std::string, JSON> *a =
          static_cast<std::map<std::string, JSON> *>(_value.object);
      std::map<std::string, JSON> *b =
          static_cast<std::map<std::string, JSON> *>(o._value.object);
      return *a == *b;
    }
  }
  case (value_type::null): {
    if (o._type == value_type::null) {
      return true;
    }
  }
  case (value_type::string): {
    if (o._type == value_type::string) {
      const std::string a = *this->_value.string;
      const std::string b = *o._value.string;
      return a == b;
    }
  }
  case (value_type::boolean): {
    if (o._type == value_type::boolean) {
      bool a = this->_value.boolean;
      bool b = o._value.boolean;
      return a == b;
    }
  }
  case (value_type::number): {
    if (o._type == value_type::number or o._type == value_type::number_float) {
      int a = this->_value.number;
      int b = o._value.number;
      return a == b;
    }
  }
  case (value_type::number_float): {
    if (o._type == value_type::number or o._type == value_type::number_float) {
      double a = this->_value.number_float;
      double b = o._value.number_float;
      return a == b;
    }
  }
  }
}

JSON::iterator::iterator() : _object(nullptr), _vi(nullptr), _oi(nullptr) {}

JSON::iterator::iterator(JSON *j) : _object(j), _vi(nullptr), _oi(nullptr) {
  switch (_object->_type) {
  case (value_type::array): {
    _vi = new std::vector<JSON>::iterator(
        static_cast<std::vector<JSON> *>(_object->_value.array)->begin());
  }
  case (value_type::object): {
    _oi = new std::map<std::string, JSON>::iterator(
        static_cast<std::map<std::string, JSON> *>(_object->_value.object)
            ->begin());
  }
  default:
    break;
  }
}

JSON::iterator::iterator(const iterator &o)
    : _object(o._object), _vi(nullptr), _oi(nullptr) {
  switch (_object->_type) {
  case (value_type::array): {
    _vi = new std::vector<JSON>::iterator(
        static_cast<std::vector<JSON> *>(_object->_value.array)->begin());
  }
  case (value_type::object): {
    _oi = new std::map<std::string, JSON>::iterator(
        static_cast<std::map<std::string, JSON> *>(_object->_value.object)
            ->begin());
  }
  default:
    break;
  }
}

JSON::iterator::~iterator() {
  delete _oi;
  delete _vi;
}

JSON::iterator &JSON::iterator::operator=(const iterator &o) {
  _object = o._object;
  return *this;
}

bool JSON::iterator::operator==(const iterator &o) const {
  return _object == o._object;
}

bool JSON::iterator::operator!=(const iterator &o) const {
  return _object != o._object;
}

JSON::iterator &JSON::iterator::operator++() {
  if (_object == nullptr) {
    return *this;
  }

  switch (_object->_type) {
  case JSON::value_type::array: {
    if (++(*_vi) ==
        static_cast<std::vector<JSON> *>(_object->_value.array)->end())
      _object = nullptr;
    break;
  }
  case JSON::value_type::object:
    if (++(*_oi) ==
        static_cast<std::map<std::string, JSON> *>(_object->_value.object)
            ->end())
      _object = nullptr;
    break;
  default: {
    _object = nullptr;
  }
  }
  return *this;
}

JSON &JSON::iterator::operator*() const {
  if (_object == nullptr) {
    throw std::runtime_error("cannot get value");
  }

  switch (_object->_type) {
  case value_type::array:
    return *_vi[0];
  case value_type::object:
    return (*_oi)->second;
  default:
    return *_object;
  }
}

JSON::iterator JSON::begin() { return JSON::iterator(this); }

JSON::iterator JSON::end() { return JSON::iterator(); }

JSON::const_iterator JSON::begin() const { return JSON::const_iterator(this); }

JSON::const_iterator JSON::end() const { return JSON::const_iterator(); }

JSON::const_iterator JSON::cbegin() const { return JSON::const_iterator(this); }

JSON::const_iterator JSON::cend() const { return JSON::const_iterator(); }

JSON::const_iterator::const_iterator()
    : _object(nullptr), _vi(nullptr), _oi(nullptr) {}

JSON::const_iterator::const_iterator(const JSON *j)
    : _object(j), _vi(nullptr), _oi(nullptr) {
  switch (_object->_type) {
  case (value_type::array): {
    _vi = new std::vector<JSON>::const_iterator(
        static_cast<std::vector<JSON> *>(_object->_value.array)->begin());
    break;
  }
  case (value_type::object): {
    _oi = new std::map<std::string, JSON>::const_iterator(
        static_cast<std::map<std::string, JSON> *>(_object->_value.object)
            ->begin());
    break;
  }
  default:
    break;
  }
}

JSON::const_iterator::const_iterator(const const_iterator &o)
    : _object(o._object), _vi(nullptr), _oi(nullptr) {
  switch (_object->_type) {
  case (value_type::array): {
    _vi = new std::vector<JSON>::const_iterator(
        static_cast<std::vector<JSON> *>(_object->_value.array)->begin());
    break;
  }
  case (value_type::object): {
    _oi = new std::map<std::string, JSON>::const_iterator(
        static_cast<std::map<std::string, JSON> *>(_object->_value.object)
            ->begin());
    break;
  }
  default:
    break;
  }
}

JSON::const_iterator::const_iterator(const iterator &o)
    : _object(o._object), _vi(nullptr), _oi(nullptr) {
  switch (_object->_type) {
  case (value_type::array): {
    _vi = new std::vector<JSON>::const_iterator(
        static_cast<std::vector<JSON> *>(_object->_value.array)->begin());
    break;
  }
  case (value_type::object): {
    _oi = new std::map<std::string, JSON>::const_iterator(
        static_cast<std::map<std::string, JSON> *>(_object->_value.object)
            ->begin());
    break;
  }
  default:
    break;
  }
}

JSON::const_iterator::~const_iterator() {
  delete _vi;
  delete _oi;
}

bool JSON::const_iterator::operator==(const const_iterator &o) const {
  return _object == o._object;
}

bool JSON::const_iterator::operator!=(const const_iterator &o) const {
  return _object != o._object;
}

const JSON &JSON::const_iterator::operator*() const {
  if (_object == nullptr) {
    throw std::runtime_error("cannot get value");
  }

  switch (_object->_type) {
  case value_type::array:
    return **_vi;
  case value_type::object:
    return (*_oi)->second;
  default:
    return *_object;
  }
}

JSON::const_iterator &JSON::const_iterator::operator++() {
  if (_object == nullptr) {
    return *this;
  }
  switch (_object->_type) {
  case value_type::array: {
    if (++(*_vi) ==
        static_cast<std::vector<JSON> *>(_object->_value.array)->cend())
      _object = nullptr;
    break;
  }
  case value_type::object: {
    if (++(*_oi) ==
        static_cast<std::map<std::string, JSON> *>(_object->_value.object)
            ->cend())
      _object = nullptr;
    break;
  }
  default:
    _object = nullptr;
  }
  return *this;
}

JSON::parser::parser(char *s) : _pos(0) {
  _buffer = new char[strlen(s) + 1];
  _length = std::strlen(s);
  //strcpy_s(_buffer, _length + 1, s);
  strcpy(_buffer, s);

  next();
}

JSON::parser::parser(std::string &s) : _pos(0) {
  _buffer = new char[s.length() + 1];
  _length = s.length();
  //strcpy_s(_buffer, _length + 1, s.c_str());
  strcpy(_buffer, s.c_str());

  next();
}

JSON::parser::parser(std::istream &_is) : _pos(0) {
  _is.seekg(0, std::ios::end);
  _length = _is.tellg();
  _is.seekg(0, std::ios::beg);

  _buffer = new char[_length];
  _is.read(_buffer, _length);
  next();
}

JSON::parser::~parser() { delete[] _buffer; }

void JSON::parser::error(std::string msg) {
  throw std::runtime_error("parse error at position " + std::to_string(_pos) +
                           ":" + msg + ",last read: '" + _current + "'");
}

bool JSON::parser::next() {
  if (_pos == _length) {
    return false;
  }
  _current = _buffer[_pos++];

  while (std::isspace(_current)) {
    if (_pos == _length) {
      return false;
    }
    _current = _buffer[_pos++];
  }

  return true;
}

std::string JSON::parser::parseString() {
  char *p = strchr(_buffer + _pos, '\"');

  while (p != nullptr and *(p - 1) == '\\') {
    // length of the string so far
    const size_t length = p - _buffer - _pos;
    // continue checking after escaped quote
    p = strchr(_buffer + _pos + length + 1, '\"');
  }

  if (p == nullptr) {
    error("expect '\"'");
  }

  const size_t length = p - _buffer - _pos;
  std::string result(_buffer + _pos, length);

  _pos += length + 1;

  next();

  return result;
}

void JSON::parser::parseTrue() {
  if (strncmp(_buffer + _pos, "rue", 3)) {
    error("expect true");
  }

  _pos += 3;

  next();
}

void JSON::parser::parseFalse() {
  if (strncmp(_buffer + _pos, "alse", 4)) {
    error("expect false");
  }

  _pos += 4;

  next();
}

void JSON::parser::parseNull() {
  if (strncmp(_buffer + _pos, "ull", 3)) {
    error("expect null");
  }

  _pos += 3;

  next();
}

void JSON::parser::expect(char c) {
  if (_current != c) {
    std::string msg = "expected '";
    msg.append(1, c);
    msg += "'";
    error(msg.c_str());
  } else {
    next();
  }
}

void JSON::parser::parse(JSON &result) {
  if (!_buffer) {
    error("unexpected end of file");
  }

  switch (_current) {
  case ('{'): {
    result._type = value_type::object;
    result._value.object = new std::map<std::string, JSON>;

    next();

    if (_current != '}') {
      do {
        const std::string key = parseString();

        expect(':');
        parse(result[key]);
      } while (_current == ',' && next());
    }

    expect('}');

    break;
  }
  case ('['): {
    result._type = value_type::array;
    result._value.array = new std::vector<JSON>;

    next();

    if (_current != '[') {
      size_t element_count = 0;
      do {
        result += JSON();
        parse(result[element_count++]);
      } while (_current == ',' && next());
    }
    expect(']');
    break;
  }
  case ('\"'): {
    result._type = value_type::string;
    result._value.string = new string_t(std::move(parseString()));
    break;
  }
  case ('t'): {
    parseTrue();
    result._type = value_type::boolean;
    result._value.boolean = true;
    break;
  }
  case ('f'): {
    parseFalse();
    result._type = value_type::boolean;
    result._value.boolean = false;
    break;
  }
  case ('n'): {
    parseNull();
    // result._type = value_type::null;
    break;
  }
  default: {
    if (std::isdigit(_current) || _current == '-') {
      std::string tmp;
      do {
        tmp += _current;
      } while (next() &&
               (std::isdigit(_current) || _current == '.' || _current == 'e' ||
                _current == 'E' || _current == '+' || _current == '-'));
      try {
        const auto float_val = std::stod(tmp);
        const auto int_val = static_cast<int>(float_val);

        if (float_val == int_val) {
          result._type = value_type::number;
          result._value.number = int_val;
        } else {
          result._type = value_type::number_float;
          result._value.number_float = float_val;
        }
      } catch (...) {
        error("error while translating " + tmp + " to number");
      }

      break;

    } else {
      error("unexpected character");
    }
  }
  }
}
