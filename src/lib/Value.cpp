#include "Value.h"

#include <cmath>
#include <cstring>
#include <iostream>
#include <variant>

#include "Errors.h"

Value::Value() : type(ValueType::null), _value() {}

Value::Value(double num) : type(ValueType::DOUBLE), _value(num) {}

Value::Value(bool boolean) : type(ValueType::BOOL), _value(boolean) {}

Value::Value(std::nullptr_t n) : type(ValueType::null), _value(n) {}

Value::Value(std::shared_ptr<std::vector<Value>> arr_val) : type(ValueType::ARRAY), _value(arr_val) {}

Value::Value(std::shared_ptr<Function> funct)
    : type(ValueType::FUNCTION), _value(funct) {}

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
  return Value(
      std::fmod(std::get<double>(lhs._value), std::get<double>(rhs._value)));
}

Value operator==(const Value& lhs, const Value& rhs) {
  if (lhs.type != rhs.type) {
    return Value(false);
  }
  if (lhs.type == ValueType::BOOL) {
    return Value(std::get<bool>(lhs._value) == std::get<bool>(rhs._value));
  } else if (lhs.type == ValueType::DOUBLE) {
    return Value(std::get<double>(lhs._value) == std::get<double>(rhs._value));
  } else if (lhs.type == ValueType::null) {
    return Value(std::get<std::nullptr_t>(lhs._value) ==
                 std::get<std::nullptr_t>(rhs._value));
  } else if (lhs.type == ValueType::FUNCTION) {
    return Value(std::get<std::shared_ptr<Function>>(lhs._value) ==
                 std::get<std::shared_ptr<Function>>(rhs._value));
  } else if (lhs.type == rhs.type && lhs.type == ValueType::ARRAY) {
    if (std::get<std::shared_ptr<std::vector<Value>>>(lhs._value)->size() !=
        std::get<std::shared_ptr<std::vector<Value>>>(rhs._value)->size()) {
      return Value(false);
    }
    int size = std::get<std::shared_ptr<std::vector<Value>>>(rhs._value)->size();
    for (int i = 0; i < size; i++) {
      if (std::get<bool> (
        (std::get<std::shared_ptr<std::vector<Value>>>(lhs._value)->at(i) !=
         std::get<std::shared_ptr<std::vector<Value>>>(rhs._value)->at(i))._value
        )) {
        return Value(false);
      }
    }
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
  } else if (lhs.type == ValueType::DOUBLE) {
    return Value(std::get<double>(lhs._value) != std::get<double>(rhs._value));
  } else if (lhs.type == ValueType::null) {
    return Value(std::get<std::nullptr_t>(lhs._value) !=
                 std::get<std::nullptr_t>(rhs._value));
  } else if (lhs.type == ValueType::FUNCTION) {
    return Value(std::get<std::shared_ptr<Function>>(lhs._value) !=
                 std::get<std::shared_ptr<Function>>(rhs._value));
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
  if (value.type == ValueType::null) {
    return os << "null";
  }
  if (value.type == ValueType::ARRAY) {
    os << "[";
    const auto& vec = *(std::get<std::shared_ptr<std::vector<Value>>>(value._value));
    for (auto element = vec.begin(); element < vec.end(); element++) {
      os << (*element);
      if (element != vec.end() - 1) os << ", ";
    }
    return os << ']';
  }
  return os;
}