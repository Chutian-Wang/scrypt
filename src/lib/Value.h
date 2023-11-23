#ifndef VALUE_H
#define VALUE_H
#include <functional>
#include <iostream>
#include <memory>
#include <variant>
#include <vector>

class Function;

enum struct ValueType { BOOL, DOUBLE, NONE, FUNCTION, null };

struct Value {
 public:
  ValueType type;
  std::variant<double, bool, std::nullptr_t, std::shared_ptr<Function> > _value;

  Value();
  Value(double num);
  Value(bool boolean);
  Value(std::nullptr_t n);
  Value(std::shared_ptr<Function> funct);

  Value& operator+=(const Value& rhs);
  Value& operator-=(const Value& rhs);
  Value& operator*=(const Value& rhs);
  Value& operator/=(const Value& rhs);
  Value& operator%=(const Value& rhs);
};

using FnPtr = std::shared_ptr<Function>;

// Value operator()(std::vector<Value>& args);

Value operator+(const Value& lhs, const Value& rhs);

Value operator-(const Value& lhs, const Value& rhs);

Value operator*(const Value& lhs, const Value& rhs);

Value operator/(const Value& lhs, const Value& rhs);

Value operator%(const Value& lhs, const Value& rhs);

Value operator==(const Value& lhs, const Value& rhs);

Value operator!=(const Value& lhs, const Value& rhs);

Value operator<(const Value& lhs, const Value& rhs);

Value operator<=(const Value& lhs, const Value& rhs);

Value operator>(const Value& lhs, const Value& rhs);

Value operator>=(const Value& lhs, const Value& rhs);

Value operator&(const Value& lhs, const Value& rhs);

Value operator^(const Value& lhs, const Value& rhs);

Value operator|(const Value& lhs, const Value& rhs);

std::ostream& operator<<(std::ostream& os, const Value& value);

#endif