#pragma once
#include "OperatorToken.hpp"

template <typename T, bool IsBin, typename Operand>
class Subtraction : public virtual OperatorToken<T, IsBin, Operand> {
  Operand* sub_;

 public:
  explicit Subtraction(const std::string& str)
      : OperatorToken<T, IsBin, Operand>(str) {
    sub_ = new Operand;
  }
  Operand* Calculate(Operand* lhs, Operand* rhs) {
    *sub_ = *lhs - *rhs;
    return sub_;
  }
  Operand* Calculate(Operand* operand) {
    *operand *= -1;
    return operand;
  }
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
  virtual ~Subtraction() { delete sub_; }
};