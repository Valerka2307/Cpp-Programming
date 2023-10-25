#pragma once
#include "OperatorToken.hpp"

template <typename T, bool IsBin, typename Operand>
class Multiplication : public virtual OperatorToken<T, IsBin, Operand> {
  Operand* mul_;

 public:
  explicit Multiplication(const std::string& str)
      : OperatorToken<T, IsBin, Operand>(str) {
    mul_ = new Operand;
  }
  Operand* Calculate(Operand* lhs, Operand* rhs) {
    *mul_ = (*lhs) * (*rhs);
    return mul_;
  }
  Operand* Calculate(Operand* operand) {
    mul_ = operand;
    return mul_;
  };
  void Compute(std::vector<T>& stack) {
    if (IsBin) {
      T second = stack.back();
      stack.pop_back();
      T first = stack.back();
      stack.pop_back();
      T res = *(Calculate(&first, &second));
      stack.push_back(res);
      return;
    }
  };
  virtual ~Multiplication() { delete mul_; }
};