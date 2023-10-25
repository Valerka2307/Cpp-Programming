#pragma once
#include <cstring>
#include <iostream>
#include <vector>

class String {
  char* str_;
  size_t size_;
  size_t capacity_;

 public:
  String();
  ~String();
  String(size_t size, char character);
  String(const char* character);
  String(const String& s);
  bool Empty() const;
  size_t Size() const;
  size_t Capacity() const;
  void Clear();
  const char* Data() const;
  char* Data();
  void PushBack(char character);
  void PopBack();
  void Resize(size_t new_size);
  void Resize(size_t new_size, char character);
  void Reserve(size_t new_cap);
  void ShrinkToFit();
  void Swap(String& other);
  char& Front();
  const char& Front() const;
  char& Back();
  const char& Back() const;

  String& operator*=(size_t number);
  String& operator=(const String& new_string);
  String& operator+=(const String& add_string);
  String& operator+=(const char* array_string);
  String& operator+=(char character);

  char& operator[](size_t ind);
  const char& operator[](size_t ind) const;

  String Join(const std::vector<String>& arr) const;
  std::vector<String> Split(const String& delim = " ");

  friend bool operator>(const String& left_string, const String& right_string);
  friend std::ostream& operator<<(std::ostream& output,
                                  const String& string_out);
};

String operator+(const String& left_string, const String& right_string);
String operator*(const String& left_string, size_t number);
String operator*(size_t number, const String& right_string);

std::istream& operator>>(std::istream& input, String& string_in);

bool operator<(const String& left_string, const String& right_string);
bool operator<=(const String& left_string, const String& right_string);
bool operator>=(const String& left_string, const String& right_string);
bool operator==(const String& left_string, const String& right_string);
bool operator!=(const String& left_string, const String& right_string);
