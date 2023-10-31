#include "Value.h"

#include <cmath>
#include <cstring>
#include <iostream>

#include "Errors.h"

Value::Value() {
  this->type = ValueType::NONE;
  memset(&(this->_value), 0, sizeof(this->_value));
}

Value::Value(double num) {
  type = ValueType::DOUBLE;
  _value._double = num;
}

Value::Value(bool boolean) {
  type = ValueType::BOOL;
  _value._bool = boolean;
}

Value& Value::operator+=(const Value& rhs) {
  if (this->type != ValueType::DOUBLE || rhs.type != ValueType::DOUBLE) {
    throw InvalidOperand();
  }
  this->_value._double = this->_value._double + rhs._value._double;
  return *this;
}

Value& Value::operator-=(const Value& rhs) {
  if (this->type != ValueType::DOUBLE || rhs.type != ValueType::DOUBLE) {
    throw InvalidOperand();
  }
  this->_value._double = this->_value._double - rhs._value._double;
  return *this;
}

Value& Value::operator*=(const Value& rhs) {
  if (this->type != ValueType::DOUBLE || rhs.type != ValueType::DOUBLE) {
    throw InvalidOperand();
  }
  this->_value._double = this->_value._double * rhs._value._double;
  return *this;
}

Value& Value::operator/=(const Value& rhs) {
  if (this->type != ValueType::DOUBLE || rhs.type != ValueType::DOUBLE) {
    throw InvalidOperand();
  }
  if (rhs._value._double == 0.) {
    throw DivByZero();
  }
  this->_value._double = this->_value._double / rhs._value._double;
  return *this;
}

Value& Value::operator%=(const Value& rhs) {
  if (this->type != ValueType::DOUBLE || rhs.type != ValueType::DOUBLE) {
    throw InvalidOperand();
  }
  if (rhs._value._double == 0.) {
    throw DivByZero();
  }
  std::cout << (this->type == ValueType::DOUBLE) << std::endl;
  this->_value._double = std::fmod(this->_value._double, rhs._value._double);
  return *this;
}

Value operator+(const Value& lhs, const Value& rhs) {
  if (lhs.type != ValueType::DOUBLE || rhs.type != ValueType::DOUBLE) {
    throw InvalidOperand();
  }
  return Value(lhs._value._double + rhs._value._double);
}

Value operator-(const Value& lhs, const Value& rhs) {
  if (lhs.type != ValueType::DOUBLE || rhs.type != ValueType::DOUBLE) {
    throw InvalidOperand();
  }
  return Value(lhs._value._double - rhs._value._double);
}

Value operator*(const Value& lhs, const Value& rhs) {
  if (lhs.type != ValueType::DOUBLE || rhs.type != ValueType::DOUBLE) {
    throw InvalidOperand();
  }
  return Value(lhs._value._double * rhs._value._double);
}

Value operator/(const Value& lhs, const Value& rhs) {
  if (lhs.type != ValueType::DOUBLE || rhs.type != ValueType::DOUBLE) {
    throw InvalidOperand();
  }
  if (rhs._value._double == 0.) {
    throw DivByZero();
  }
  return Value(lhs._value._double / rhs._value._double);
}

Value operator%(const Value& lhs, const Value& rhs) {
  if (lhs.type != ValueType::DOUBLE || rhs.type != ValueType::DOUBLE) {
    throw InvalidOperand();
  }
  if (rhs._value._double == 0.) {
    throw DivByZero();
  }
  return Value(std::fmod(lhs._value._double, rhs._value._double));
}

Value operator==(const Value& lhs, const Value& rhs) {
  if (lhs.type != rhs.type) {
    throw InvalidOperand();
  }
  return Value(lhs.type == ValueType::BOOL
                   ? lhs._value._bool == rhs._value._bool
                   : lhs._value._double == rhs._value._double);
}

Value operator!=(const Value& lhs, const Value& rhs) {
  if (lhs.type != rhs.type) {
    throw InvalidOperand();
  }
  return Value(lhs.type == ValueType::BOOL
                   ? lhs._value._bool != rhs._value._bool
                   : lhs._value._double != rhs._value._double);
}

Value operator<(const Value& lhs, const Value& rhs) {
  if (lhs.type != ValueType::DOUBLE || rhs.type != ValueType::DOUBLE) {
    throw InvalidOperand();
  }
  return Value(lhs._value._double < rhs._value._double);
}

Value operator<=(const Value& lhs, const Value& rhs) {
  if (lhs.type != ValueType::DOUBLE || rhs.type != ValueType::DOUBLE) {
    throw InvalidOperand();
  }
  return Value(lhs._value._double <= rhs._value._double);
}

Value operator>(const Value& lhs, const Value& rhs) {
  if (lhs.type != ValueType::DOUBLE || rhs.type != ValueType::DOUBLE) {
    throw InvalidOperand();
  }
  return Value(lhs._value._double > rhs._value._double);
}

Value operator>=(const Value& lhs, const Value& rhs) {
  if (lhs.type != ValueType::DOUBLE || rhs.type != ValueType::DOUBLE) {
    throw InvalidOperand();
  }
  return Value(lhs._value._double >= rhs._value._double);
}

Value operator&(const Value& lhs, const Value& rhs) {
  if (lhs.type != ValueType::BOOL || rhs.type != ValueType::BOOL) {
    throw InvalidOperand();
  }
  return Value(lhs._value._bool && rhs._value._bool);
}

Value operator^(const Value& lhs, const Value& rhs) {
  if (lhs.type != ValueType::BOOL || rhs.type != ValueType::BOOL) {
    throw InvalidOperand();
  }
  return Value(lhs._value._bool != rhs._value._bool);
}

Value operator|(const Value& lhs, const Value& rhs) {
  if (lhs.type != ValueType::BOOL || rhs.type != ValueType::BOOL) {
    throw InvalidOperand();
  }
  return Value(lhs._value._bool || rhs._value._bool);
}

std::ostream& operator<<(std::ostream& os, const Value& value) {
  if (value.type == ValueType::BOOL) {
    return os << (value._value._bool ? "true" : "false");
  }
  if (value.type == ValueType::DOUBLE) {
    return os << (value._value._double);
  }
  return os;
}