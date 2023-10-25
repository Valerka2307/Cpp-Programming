#pragma once
#include "AbstractToken.hpp"

class BracketToken : public AbstractToken {
 public:
  bool open = false;
  explicit BracketToken(const std::string& str) : AbstractToken(str) {}
  [[nodiscard]] bool IsOpening() const { return open; }
  virtual ~BracketToken() = default;
};