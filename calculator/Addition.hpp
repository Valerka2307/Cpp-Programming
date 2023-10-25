#pragma once
#include "OperatorToken.hpp"

template <typename T, bool IsBin, typename Operand>
class Addition : public virtual OperatorToken<T, IsBin, Operand> {
  Operand* sum_;

 public:
  explicit Addition(const std::string& str)
      : OperatorToken<T, IsBin, Operand>(str) {
    sum_ = new Operand;
  }
  Operand* Calculate(Operand* lhs, Operand* rhs) {
    *sum_ = *lhs + *rhs;
    return sum_;
  }
  Operand* Calculate(Operand* operand) { return operand; }
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
    if (!IsBin) {
      T last = stack.back();
      stack.pop_back();
      T cur = *(Calculate(&last));
      stack.push_back(cur);
      return;
    }
  };
  virtual ~Addition() { delete sum_; }
};