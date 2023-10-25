#pragma once
#include <cstring>
#include <iostream>
#include <string>
#include <vector>

class BigInt {
  std::vector<int> big_int_;
  int sign_;

 public:
  BigInt();
  BigInt(int64_t number);
  BigInt(std::string str);
  BigInt(const BigInt& number);
  ~BigInt();
  [[nodiscard]] uint64_t Length() const;
  [[nodiscard]] bool Sign() const;
  [[nodiscard]] std::vector<int> OurBigInt() const;
  void PushBack(int num);
  void Resize(uint64_t size);
  void Normilize();
  void NormilizeNegative();
  BigInt Addition(const BigInt& other);
  BigInt Substruction(const BigInt& other);
  BigInt MultiplyNTimes(int n) const;
  void MultiplyTwoTimes();
  bool CompareVectors(const std::vector<int>& arr);
  friend std::ostream& operator<<(std::ostream& output,
                                  const BigInt& string_out);
  friend std::istream& operator>>(std::istream& input, BigInt& big_int);
  BigInt& operator=(const BigInt& other);
  BigInt& operator+=(const BigInt& other);
  BigInt& operator-=(const BigInt& other);
  BigInt& operator*=(const BigInt& other);
  BigInt& operator/=(const BigInt& other);
  BigInt& operator%=(const BigInt& other);

  BigInt& operator++();
  BigInt operator++(int);
  BigInt& operator--();
  BigInt operator--(int);
  BigInt operator-() const;
};

bool operator<(const BigInt& first, const BigInt& second);
bool operator>(const BigInt& first, const BigInt& second);
bool operator<=(const BigInt& first, const BigInt& second);
bool operator>=(const BigInt& first, const BigInt& second);
bool operator==(const BigInt& first, const BigInt& second);
bool operator!=(const BigInt& first, const BigInt& second);

bool CompareLessPositive(const BigInt& first, const BigInt& second);
bool CompareLessNegative(const BigInt& first, const BigInt& second);

BigInt operator+(const BigInt& left, const BigInt& right);
BigInt operator-(const BigInt& left, const BigInt& right);
BigInt operator*(const BigInt& left, const BigInt& right);
BigInt operator/(const BigInt& left, const BigInt& right);
BigInt operator%(const BigInt& left, const BigInt& right);

BigInt Quotient(const BigInt& cur, const BigInt& other);
std::vector<int> AddEqualLenthes(const BigInt& first, const BigInt& second);
std::vector<int> AddLessLenthes(const BigInt& first, const BigInt& second);
std::vector<int> AddGreaterLenthes(const BigInt& first, const BigInt& second);
std::vector<int> SubEqualLenthes(const BigInt& first, const BigInt& second);
std::vector<int> SubLessLenthes(const BigInt& first, const BigInt& second);
std::vector<int> SubGreaterLenthes(const BigInt& first, const BigInt& second);

const int kModuleOfTen = 10;
const int kAsciiShift = 48;