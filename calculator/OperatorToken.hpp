#pragma once
#include "OperandToken.hpp"

template <typename T, bool IsBin, typename Operand>
class OperatorToken : public OperandToken<T> {
 public:
  explicit OperatorToken(const std::string& str) : OperandToken<T>(str) {}
  bool IsBinary() { return IsBin; }
  virtual Operand* Calculate(Operand* lhs, Operand* rhs) = 0;
  virtual Operand* Calculate(Operand* operand) = 0;
  virtual ~OperatorToken() = default;
};