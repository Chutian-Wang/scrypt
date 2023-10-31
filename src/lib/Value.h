#ifndef VALUE_H
#define VALUE_H
#include <iostream>

enum struct ValueType { BOOL, DOUBLE, NONE };

struct Value {
 public:
  ValueType type;
  union {
    double _double;
    bool _bool;
  } _value;

  Value();
  Value(double num);
  Value(bool boolean);

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