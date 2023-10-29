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
};

#endif