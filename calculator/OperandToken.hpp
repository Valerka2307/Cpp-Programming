#pragma once
#include "BracketToken.hpp"

template <typename T>
class OperandToken : public BracketToken {
  T val_;

 public:
  explicit OperandToken(const std::string& str) : BracketToken(str) {
    std::stringstream strstream;
    strstream << GetStringToken();
    strstream >> val_;
  }
  explicit OperandToken(const T& value) {
    std::stringstream strstream;
    strstream << value;
    strstream >> val_;
  }
  const T& GetValue() { return val_; }
  virtual void Compute(std::vector<T>& stack) { stack.push_back(val_); };
  virtual ~OperandToken() = default;
};