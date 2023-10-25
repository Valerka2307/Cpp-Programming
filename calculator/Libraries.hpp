#pragma once
#include <cmath>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

struct InvalidExpr : std::exception {
  [[nodiscard]] const char* what() const noexcept override {
    return "Invalid expression!";
  }
};
