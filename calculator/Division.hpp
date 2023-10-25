#pragma once
#include "OperatorToken.hpp"

template <typename T, bool IsBin, typename Operand>
class Division : public virtual OperatorToken<T, IsBin, Operand> {
  Operand* div_;

 public:
  explicit Division(const std::string& str)
      : OperatorToken<T, IsBin, Operand>(str) {
    div_ = new Operand;
  }
  Operand* Calculate(Operand* lhs, Operand* rhs) {
    *div_ = (*lhs) / (*rhs);
    return div_;
  }
  Operand* Calculate(Operand* operand) {
    div_ = operand;
    return div_;
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
  virtual ~Division() { delete div_; }
};