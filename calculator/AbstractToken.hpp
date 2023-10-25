#pragma once
#include "Libraries.hpp"

class AbstractToken {
  std::string token_;

 public:
  AbstractToken() = default;
  explicit AbstractToken(const std::string& str) {
    for (auto symbol : str) {
      if (symbol != ' ') {
        token_.push_back(symbol);
      }
    }
  }
  [[nodiscard]] const std::string& GetStringToken() const { return token_; }
  virtual ~AbstractToken() = default;
};