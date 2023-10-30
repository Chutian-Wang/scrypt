#ifndef VALUE_H
#define VALUE_H

enum struct ValueType { BOOL, DOUBLE, NONE };

struct Value {
 public:
  ValueType type;
  union {
    double _double;
    bool _bool;
  } _value;

  Value() {
    this->type = ValueType::NONE;
    memset(&(this->_value), 0, sizeof(this->_value));
  }
  Value(double num) {
    type = ValueType::DOUBLE;
    _value._double = num;
  }

  Value(bool boolean) {
    type = ValueType::BOOL;
    _value._bool = boolean;
  }
};

#endif