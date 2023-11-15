#include "Value.h"

#include <cmath>
#include <cstring>
#include <iostream>
#include <variant>

#include "Errors.h"

Value::Value() : type(ValueType::NONE), _value() {}

Value::Value(double num) : type(ValueType::DOUBLE), _value(num) {}

Value::Value(bool boolean) : type(ValueType::BOOL), _value(boolean) {}

Value::Value(std::nullptr_t n) : type(ValueType::null), _value(n) {}

Value::Value(std::function<Value(const std::vector<Value>&)> function)
  : type(ValueType::FUNCTION), 
  _value(function) {}

Value& Value::operator+=(const Value& rhs) {
  if (this->type != ValueType::DOUBLE || rhs.type != ValueType::DOUBLE) {
    throw InvalidOperand();
  }
  _value = std::get<double>(_value) + std::get<double>(rhs._value);
  return *this;
}

Value& Value::operator-=(const Value& rhs) {
  if (this->type != ValueType::DOUBLE || rhs.type != ValueType::DOUBLE) {
    throw InvalidOperand();
  }
  _value = std::get<double>(_value) - std::get<double>(rhs._value);
  return *this;
}

Value& Value::operator*=(const Value& rhs) {
  if (this->type != ValueType::DOUBLE || rhs.type != ValueType::DOUBLE) {
    throw InvalidOperand();
  }
  _value = std::get<double>(_value) * std::get<double>(rhs._value);
  return *this;
}

Value& Value::operator/=(const Value& rhs) {
  if (this->type != ValueType::DOUBLE || rhs.type != ValueType::DOUBLE) {
    throw InvalidOperand();
  }
  if (std::get<double>(rhs._value) == 0.) {
    throw DivByZero();
  }
  _value = std::get<double>(_value) / std::get<double>(rhs._value);
  return *this;
}

Value& Value::operator%=(const Value& rhs) {
  if (this->type != ValueType::DOUBLE || rhs.type != ValueType::DOUBLE) {
    throw InvalidOperand();
  }
  if (std::get<double>(rhs._value) == 0.) {
    throw DivByZero();
  }
  _value = std::fmod(std::get<double>(_value), std::get<double>(rhs._value));
  return *this;
}

Value operator+(const Value& lhs, const Value& rhs) {
  if (lhs.type != ValueType::DOUBLE || rhs.type != ValueType::DOUBLE) {
    throw InvalidOperand();
  }
  return Value(std::get<double>(lhs._value) + std::get<double>(rhs._value));
}

Value operator-(const Value& lhs, const Value& rhs) {
  if (lhs.type != ValueType::DOUBLE || rhs.type != ValueType::DOUBLE) {
    throw InvalidOperand();
  }
  return Value(std::get<double>(lhs._value) - std::get<double>(rhs._value));
}

Value operator*(const Value& lhs, const Value& rhs) {
  if (lhs.type != ValueType::DOUBLE || rhs.type != ValueType::DOUBLE) {
    throw InvalidOperand();
  }
  return Value(std::get<double>(lhs._value) * std::get<double>(rhs._value));
}

Value operator/(const Value& lhs, const Value& rhs) {
  if (lhs.type != ValueType::DOUBLE || rhs.type != ValueType::DOUBLE) {
    throw InvalidOperand();
  }
  if (std::get<double>(rhs._value) == 0.) {
    throw DivByZero();
  }
  return Value(std::get<double>(lhs._value) / std::get<double>(rhs._value));
}

Value operator%(const Value& lhs, const Value& rhs) {
  if (lhs.type != ValueType::DOUBLE || rhs.type != ValueType::DOUBLE) {
    throw InvalidOperand();
  }
  if (std::get<double>(rhs._value) == 0.) {
    throw DivByZero();
  }
  return Value(std::fmod(std::get<double>(lhs._value), std::get<double>(rhs._value)));
}

Value operator==(const Value& lhs, const Value& rhs) {
  if (lhs.type != rhs.type) {
    return Value(false);
  }
  return Value(lhs.type == ValueType::BOOL 
              ? std::get<bool>(lhs._value) == std::get<bool>(rhs._value)
              : std::get<double>(lhs._value) == std::get<double>(rhs._value));
}

Value operator!=(const Value& lhs, const Value& rhs) {
  if (lhs.type != rhs.type) {
    return Value(true);
  }
  return Value(lhs.type == ValueType::BOOL 
              ? std::get<bool>(lhs._value) != std::get<bool>(rhs._value)
              : std::get<double>(lhs._value) != std::get<double>(rhs._value));
}

Value operator<(const Value& lhs, const Value& rhs) {
  if (lhs.type != ValueType::DOUBLE || rhs.type != ValueType::DOUBLE) {
    throw InvalidOperand();
  }
  return Value(std::get<double>(lhs._value) < std::get<double>(rhs._value));
}

Value operator<=(const Value& lhs, const Value& rhs) {
  if (lhs.type != ValueType::DOUBLE || rhs.type != ValueType::DOUBLE) {
    throw InvalidOperand();
  }
  return Value(std::get<double>(lhs._value) <= std::get<double>(rhs._value));
}

Value operator>(const Value& lhs, const Value& rhs) {
  if (lhs.type != ValueType::DOUBLE || rhs.type != ValueType::DOUBLE) {
    throw InvalidOperand();
  }
  return Value(std::get<double>(lhs._value) > std::get<double>(rhs._value));
}

Value operator>=(const Value& lhs, const Value& rhs) {
  if (lhs.type != ValueType::DOUBLE || rhs.type != ValueType::DOUBLE) {
    throw InvalidOperand();
  }
  return Value(std::get<double>(lhs._value) >= std::get<double>(rhs._value));
}

Value operator&(const Value& lhs, const Value& rhs) {
  if (lhs.type != ValueType::BOOL || rhs.type != ValueType::BOOL) {
    throw InvalidOperand();
  }
  return Value(std::get<bool>(lhs._value) && std::get<bool>(rhs._value));
}

Value operator^(const Value& lhs, const Value& rhs) {
  if (lhs.type != ValueType::BOOL || rhs.type != ValueType::BOOL) {
    throw InvalidOperand();
  }
  return Value(std::get<bool>(lhs._value) != std::get<bool>(rhs._value));
}

Value operator|(const Value& lhs, const Value& rhs) {
  if (lhs.type != ValueType::BOOL || rhs.type != ValueType::BOOL) {
    throw InvalidOperand();
  }
  return Value(std::get<bool>(lhs._value) || std::get<bool>(rhs._value));
}

std::ostream& operator<<(std::ostream& os, const Value& value) {
  if (value.type == ValueType::BOOL) {
    return os << (std::get<bool>(value._value) ? "true" : "false");
  }
  if (value.type == ValueType::DOUBLE) {
    return os << (std::get<double>(value._value));
  }
  return os;
}