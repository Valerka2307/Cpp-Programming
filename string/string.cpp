#include "string.hpp"

String::String() {
  capacity_ = 0;
  size_ = 0;
  str_ = nullptr;
}

String::String(size_t size, char character) {
  size_ = size;
  capacity_ = 0;
  str_ = new char[size_ + 1];
  memset(str_, character, size_);
  *(str_ + size_) = '\0';
  if (size_ > 0) {
    capacity_ = 1;
  }
  while (capacity_ < size_) {
    capacity_ *= 2;
  }
}

String::String(const char* character) {
  capacity_ = 0;
  size_ = strlen(character);
  str_ = new char[size_ + 1];
  for (size_t i = 0; i < size_; ++i) {
    *(str_ + i) = *(character + i);
  }
  *(str_ + size_) = '\0';
  if (size_ > 0) {
    capacity_ = 1;
  }
  while (capacity_ < size_) {
    capacity_ *= 2;
  }
}

String::String(const String& s) {
  size_ = s.size_;
  capacity_ = s.capacity_;
  str_ = new char[size_ + 1];
  memcpy(str_, s.str_, size_);
  *(str_ + size_) = '\0';
}

char& String::operator[](size_t ind) { return str_[ind]; }

const char& String::operator[](size_t ind) const { return str_[ind]; }

String::~String() { delete[] str_; }

size_t String::Size() const { return size_; }

size_t String::Capacity() const { return capacity_; }

bool String::Empty() const { return size_ == 0; }

void String::Clear() { size_ = 0; }

const char* String::Data() const { return str_; }

char* String::Data() { return str_; }

void String::PushBack(char character) {
  size_ += 1;
  char* new_memory = new char[size_];
  for (size_t i = 0; i < size_ - 1; ++i) {
    new_memory[i] = str_[i];
  }
  delete[] str_;
  str_ = new char[size_ + 1];
  for (size_t i = 0; i < size_ - 1; ++i) {
    *(str_ + i) = *(new_memory + i);
  }
  delete[] new_memory;
  *(str_ + size_ - 1) = character;
  *(str_ + size_) = '\0';
  if (size_ - 1 == 0 && capacity_ - 1 == 0) {
    capacity_ = 1;
  } else if (size_ > 0 && capacity_ == size_) {
    capacity_ *= 2;
  }
}

void String::PopBack() {
  if (size_ == 0) {
    return;
  }
  str_[size_ - 1] = '\0';
  --size_;
}

void String::Resize(size_t new_size) {
  if (capacity_ >= new_size) {
    size_ = new_size;
    return;
  }
  char* new_buffer = new char[new_size];
  memcpy(new_buffer, str_, size_);
  delete[] str_;
  str_ = new_buffer;
  size_ = new_size;
  if (size_ > 0) {
    capacity_ = 1;
  }
  while (capacity_ < size_) {
    capacity_ *= 2;
  }
}

void String::Resize(size_t new_size, char character) {
  size_t number_of_bytes = new_size - size_;
  size_t previous_size = size_;
  (*this).Resize(new_size);
  if (previous_size < new_size) {
    memset(str_ + previous_size, character, number_of_bytes);
  }
}

void String::Reserve(size_t new_cap) {
  if (new_cap > capacity_) {
    capacity_ = new_cap;
  }
}

void String::ShrinkToFit() { capacity_ = size_; }

void String::Swap(String& other) {
  std::swap(str_, other.str_);
  std::swap(size_, other.size_);
  std::swap(capacity_, other.capacity_);
}

char& String::Front() { return *(str_ + 0); }

const char& String::Front() const { return *(str_ + 0); }

char& String::Back() { return *(str_ + size_ - 1); }

const char& String::Back() const { return *(str_ + size_ - 1); }

bool operator>(const String& left_string, const String& right_string) {
  for (size_t i = 0; i < left_string.size_; ++i) {
    if (*(left_string.str_ + i) > *(right_string.str_ + i)) {
      return true;
    }
  }
  return left_string.size_ > right_string.size_;
}

bool operator>=(const String& left_string, const String& right_string) {
  return left_string > right_string || left_string == right_string;
}

bool operator==(const String& left_string, const String& right_string) {
  return !(left_string > right_string || right_string > left_string);
}

bool operator!=(const String& left_string, const String& right_string) {
  return !(left_string == right_string);
}

bool operator<(const String& left_string, const String& right_string) {
  return !(left_string >= right_string);
}

bool operator<=(const String& left_string, const String& right_string) {
  return !(left_string > right_string);
}

String& String::operator=(const String& new_string) {
  delete[] str_;
  size_ = new_string.size_;
  str_ = new char[size_ + 1];
  memcpy(str_, new_string.str_, size_);
  *(str_ + size_) = '\0';
  return *this;
}

String& String::operator+=(const String& add_string) {
  size_t old_size = size_;
  size_ += add_string.size_;
  char* new_memory = new char[size_ + 1];
  memcpy(new_memory, str_, old_size);
  delete[] str_;
  memcpy(new_memory + old_size, add_string.str_, add_string.size_);
  str_ = new_memory;
  *(str_ + size_) = '\0';
  if (size_ > 0) {
    capacity_ = 1;
  }
  while (capacity_ < size_) {
    capacity_ *= 2;
  }
  return *this;
}

String& String::operator+=(const char* array_string) {
  size_t add_size = strlen(array_string);
  size_t old_size = size_;
  size_ += add_size;
  char* new_memory = new char[size_ + 1];
  memcpy(new_memory, str_, old_size);
  delete[] str_;
  memcpy(new_memory + old_size, array_string, add_size);
  str_ = new_memory;
  *(str_ + size_) = '\0';
  if (size_ > 0) {
    capacity_ = 1;
  }
  while (capacity_ < size_) {
    capacity_ *= 2;
  }
  return *this;
}

String& String::operator+=(char character) {
  this->PushBack(character);
  return *this;
}

String& String::operator*=(size_t number) {
  size_t b = number % 2;
  String s = "";
  if (number == 0) {
    *this = "\0";
    return *this;
  }
  if (number == 1) {
    return *this;
  }
  if (number == 2) {
    s += *this;
    s += *this;
    return *this = s;
  }
  for (size_t i = 0; i < number / 2; ++i) {
    s += *this;
  }
  s *= 2;
  if (b == 1) {
    s += *this;
  }
  *this = s;
  return *this;
}

String operator+(const String& left_string, const String& right_string) {
  String copy = left_string;
  copy += right_string;
  return copy;
}

String operator*(const String& left_string, size_t number) {
  String copy = left_string;
  copy *= number;
  return copy;
}

String operator*(size_t number, const String& right_string) {
  String copy = right_string;
  copy *= number;
  return copy;
}

std::ostream& operator<<(std::ostream& output, const String& string_out) {
  for (size_t i = 0; i < string_out.size_; ++i) {
    output << string_out[i];
  }
  return output;
}

std::istream& operator>>(std::istream& input, String& str) {
  str.Clear();
  char t;
  while (input.get(t) and t != '\n' and t != ' ') {
    str += t;
  }
  return input;
}

String String::Join(const std::vector<String>& arr) const {
  String str("");
  size_t arr_size = arr.size();
  if (arr_size == 0) {
    return str;
  }
  for (size_t i = 0; i < arr_size; ++i) {
    str += arr[i];
    if (i != arr_size - 1) {
      str += (*this);
    }
  }
  return str;
}

std::vector<String> String::Split(const String& delim) {
  std::vector<String> string_list;
  char* delim_ptr = str_;
  char* end;
  for (char* ptr = str_; delim_ptr != nullptr; ptr = delim_ptr + delim.size_) {
    String supported_string;
    delim_ptr = strstr(ptr, delim.str_);
    end = (delim_ptr == nullptr) ? (str_ + size_) : delim_ptr;
    char* support_ptr = ptr;
    while (support_ptr < end) {
      supported_string.PushBack(*support_ptr);
      ++support_ptr;
    }
    if (supported_string.size_ == 0) {
      supported_string = "";
    }
    string_list.push_back(supported_string);
  }
  return string_list;
}
