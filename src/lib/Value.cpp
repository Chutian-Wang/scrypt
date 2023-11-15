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
  std::visit([this, &rhs](auto&& op) {
    if constexpr (std::is_same_v<decltype(op), double>) {
      this->_value = op + std::get<double>(rhs._value);
    }
    // else {
    //   throw InvalidOperand();
    // }
  }, _value);
  return *this;
}

Value& Value::operator-=(const Value& rhs) {
  if (this->type != ValueType::DOUBLE || rhs.type != ValueType::DOUBLE) {
    throw InvalidOperand();
  }
  std::visit([this, &rhs](auto&& op) {
    if constexpr (std::is_same_v<decltype(op), double>) {
      this->_value = op - std::get<double>(rhs._value);
    }
    // else {
    //   throw InvalidOperand();
    // }
  }, _value);
  return *this;
}

Value& Value::operator*=(const Value& rhs) {
  if (this->type != ValueType::DOUBLE || rhs.type != ValueType::DOUBLE) {
    throw InvalidOperand();
  }
  std::visit([this, &rhs](auto&& op) {
    if constexpr (std::is_same_v<decltype(op), double>) {
      this->_value = op * std::get<double>(rhs._value);
    }
    // else {
    //   throw InvalidOperand();
    // }
  }, _value);
  return *this;
}

Value& Value::operator/=(const Value& rhs) {
  if (this->type != ValueType::DOUBLE || rhs.type != ValueType::DOUBLE) {
    throw InvalidOperand();
  }
  if (std::get<double>(rhs._value) == 0.) {
    throw DivByZero();
  }
  std::visit([this, &rhs](auto&& op) {
    if constexpr (std::is_same_v<decltype(op), double>) {
      this->_value = op / std::get<double>(rhs._value);
    }
    // else {
    //   throw InvalidOperand();
    // }
  }, _value);
  return *this;
}

Value& Value::operator%=(const Value& rhs) {
  if (this->type != ValueType::DOUBLE || rhs.type != ValueType::DOUBLE) {
    throw InvalidOperand();
  }
  if (std::get<double>(rhs._value) == 0.) {
    throw DivByZero();
  }
  std::visit([this, &rhs](auto&& op) {
    if constexpr (std::is_same_v<decltype(op), double>) {
      this->_value = std::fmod(op, std::get<double>(rhs._value));
    }
    // else {
    //   throw InvalidOperand();
    // }
  }, _value);

  return *this;
}

Value operator+(const Value& lhs, const Value& rhs) {
  if (lhs.type != ValueType::DOUBLE || rhs.type != ValueType::DOUBLE) {
    throw InvalidOperand();
  }
  std::cout << std::get<double>(lhs._value) << std::endl;
  std::cout << std::get<double>(rhs._value) << std::endl;
  return Value(std::get<double>(lhs._value) + std::get<double>(rhs._value));
}

Value operator-(const Value& lhs, const Value& rhs) {
  if (lhs.type != ValueType::DOUBLE || rhs.type != ValueType::DOUBLE) {
    throw InvalidOperand();
  }
  std::cout << std::get<double>(lhs._value) << std::endl;
  std::cout << std::get<double>(rhs._value) << std::endl;
  return Value(std::get<double>(lhs._value) - std::get<double>(rhs._value));
}

Value operator*(const Value& lhs, const Value& rhs) {
  if (lhs.type != ValueType::DOUBLE || rhs.type != ValueType::DOUBLE) {
    throw InvalidOperand();
  }
  std::cout << std::get<double>(lhs._value) << std::endl;
  std::cout << std::get<double>(rhs._value) << std::endl;
  return Value(std::get<double>(lhs._value) * std::get<double>(rhs._value));
}

Value operator/(const Value& lhs, const Value& rhs) {
  if (lhs.type != ValueType::DOUBLE || rhs.type != ValueType::DOUBLE) {
    throw InvalidOperand();
  }
  if (std::get<double>(rhs._value) == 0.) {
    throw DivByZero();
  }
  std::cout << std::get<double>(lhs._value) << std::endl;
  std::cout << std::get<double>(rhs._value) << std::endl;
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
  if (lhs.type == ValueType::BOOL) {
    return Value(std::get<bool>(lhs._value) == std::get<bool>(rhs._value));
  }
  else if (lhs.type == ValueType::DOUBLE) {
    return Value(std::get<double>(lhs._value) == std::get<double>(rhs._value));
  }
  else if (lhs.type == ValueType::null) {
    return Value(true);
  }

  return Value(false);
}

Value operator!=(const Value& lhs, const Value& rhs) {
  if (lhs.type != rhs.type) {
    return Value(true);
  }

  if (lhs.type == ValueType::BOOL) {
    return Value(std::get<bool>(lhs._value) != std::get<bool>(rhs._value));
  }
  else if (lhs.type == ValueType::DOUBLE) {
    return Value(std::get<double>(lhs._value) != std::get<double>(rhs._value));
  }
  else if (lhs.type == ValueType::null) {
    return Value(false);
  }

  return Value(true);
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