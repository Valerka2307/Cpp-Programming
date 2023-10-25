#pragma once
#include "ExprToPolish.hpp"

template <typename T>
class Calculator {
 public:
  Calculator() = default;
  static T CalculateExpr(const std::string& expr) {
    std::vector<AbstractToken*> expression;
    std::vector<T> stack;
    ExprInPolishNotation<T> polish(expr);
    expression = polish.GetTokens();
    for (auto* elem : expression) {
      auto ptr = static_cast<OperandToken<T>*>(elem);
      ptr->Compute(stack);
      delete ptr;
    }
    InvalidExpr exception;
    if (stack.size() > 1) {
      while (!stack.empty()) {
        stack.pop_back();
      }
      throw exception;
    }
    return stack.front();
  }
};