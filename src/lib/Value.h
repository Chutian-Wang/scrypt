#ifndef VALUE_H
#define VALUE_H

enum struct ValueType { BOOL, DOUBLE };

struct Value {
 public:
  ValueType type;
  union {
    double _double;
    bool _bool;
  };

  Value();
  Value(double num) {
    type = ValueType::DOUBLE;
    _double = num;
  }

  Value(bool boolean) {
    type = ValueType::BOOL;
    _bool = boolean;
  }
};

#endif