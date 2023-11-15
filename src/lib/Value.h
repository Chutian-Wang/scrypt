#ifndef VALUE_H
#define VALUE_H
#include <iostream>
#include <variant>
#include <vector>

enum struct ValueType { BOOL, DOUBLE, NONE, FUNCTION, null };

struct Value {
 public:
  ValueType type;
  std::variant<
    double, 
    bool, 
    std::nullptr_t,
    std::function<Value(const std::vector<Value>&)>
  > _value;

  Value();
  Value(double num);
  Value(bool boolean);
  Value(std::nullptr_t n);
  Value(std::function<Value(const std::vector<Value>&)> function);

  Value& operator+=(const Value& rhs);
  Value& operator-=(const Value& rhs);
  Value& operator*=(const Value& rhs);
  Value& operator/=(const Value& rhs);
  Value& operator%=(const Value& rhs);
};

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