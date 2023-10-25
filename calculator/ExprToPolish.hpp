#pragma once
#include "Addition.hpp"
#include "Division.hpp"
#include "Multiplication.hpp"
#include "Subtraction.hpp"

template <typename T>
class ExprInPolishNotation {
  std::vector<AbstractToken*> tokens_;
  std::map<char, int> priority_{{'(', 1}, {'+', 2}, {'-', 2}, {'*', 3},
                                {'/', 3}, {'^', 4}, {'~', 4}};

 public:
  explicit ExprInPolishNotation(const std::string& str) {
    std::string copy;
    for (auto symb : str) {
      if (symb != ' ') {
        copy.push_back(symb);
      }
    }
    for (size_t ind = 0; ind < copy.size(); ++ind) {
      std::string symbol;
      bool na_n = true;
      CheckNumber(copy, symbol, na_n, ind);
      AbstractToken* tok;
      if (!na_n) {
        tok = new OperandToken<T>(symbol);
      } else if (symbol == "+") {
        if (priority_[tokens_.back()->GetStringToken()[0]] > 0) {
          symbol = "^";
          tok = new Addition<T, false, T>(symbol);
        } else {
          tok = new Addition<T, true, T>(symbol);
        }
      } else if (symbol == "-") {
        if (priority_[tokens_.back()->GetStringToken()[0]] > 0) {
          symbol = "~";
          tok = new Subtraction<T, false, T>(symbol);
        } else {
          tok = new Subtraction<T, true, T>(symbol);
        }
      } else if (symbol == "*") {
        tok = new Multiplication<T, true, T>(symbol);
      } else if (symbol == "/") {
        tok = new Division<T, true, T>(symbol);
      } else if (symbol == "(" || symbol == ")") {
        tok = new OperandToken<T>(symbol);
      }
      tokens_.push_back(tok);
    }
    ToPostfix();
  }

  void ToPostfix() {
    std::vector<AbstractToken*> output_string;
    std::vector<AbstractToken*> stack;
    for (size_t i = 0; i < tokens_.size(); ++i) {
      if (tokens_[i]->GetStringToken() == "(") {
        stack.push_back(tokens_[i]);
      } else if (tokens_[i]->GetStringToken()[0] >= '0' &&
                 tokens_[i]->GetStringToken()[0] <= '9') {
        output_string.push_back(tokens_[i]);
      } else if (priority_[tokens_[i]->GetStringToken()[0]] > 0) {
        char cur = tokens_[i]->GetStringToken()[0];
        while (!stack.empty() &&
               priority_[stack.back()->GetStringToken()[0]] > priority_[cur]) {
          output_string.push_back(stack.back());
          stack.pop_back();
        }
        stack.push_back(tokens_[i]);
      } else if (tokens_[i]->GetStringToken() == ")") {
        delete tokens_[i];
        while (stack.back()->GetStringToken() != "(") {
          output_string.push_back(stack.back());
          stack.pop_back();
        }
        delete stack.back();
        stack.pop_back();
      }
    }
    while (!stack.empty()) {
      output_string.push_back(stack.back());
      stack.pop_back();
    }
    InvalidExpr exception;
    if (!stack.empty()) {
      while (!stack.empty()) {
        delete stack.back();
        stack.pop_back();
      }
      throw exception;
    }
    tokens_ = output_string;
  }

  void CheckNumber(std::string& copy, std::string& symbol, bool& na_n,
                   size_t& ind) {
    if (copy[ind] >= '0' && copy[ind] <= '9') {
      na_n = false;
      symbol.push_back(copy[ind]);
      if (copy[ind + 1] == '.') {
        ind++;
        symbol.push_back(copy[ind]);
        while (copy[ind + 1] >= '0' && copy[ind + 1] <= '9') {
          ind++;
          symbol.push_back(copy[ind]);
        }
      }
      if (copy[ind + 1] >= '0' && copy[ind + 1] <= '9') {
        ind++;
        symbol.push_back(copy[ind]);
        while (copy[ind + 1] >= '0' && copy[ind - 1] <= '9') {
          ind++;
          symbol.push_back(copy[ind]);
        }
        if (copy[ind + 1] == '.') {
          ind++;
          symbol.push_back(copy[ind]);
          while (copy[ind + 1] >= '0' && copy[ind + 1] <= '9') {
            ind++;
            symbol.push_back(copy[ind]);
          }
        }
      }
    } else {
      symbol.push_back(copy[ind]);
    }
  }
  const std::vector<AbstractToken*>& GetTokens() { return tokens_; }
};
