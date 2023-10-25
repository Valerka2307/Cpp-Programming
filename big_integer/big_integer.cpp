#include "big_integer.hpp"

BigInt::BigInt() {
  big_int_ = {};
  sign_ = 0;
}

BigInt::BigInt(int64_t number) {
  std::string str = std::to_string(number);
  if (str == "-0" || str == "0") {
    big_int_.push_back(0);
    sign_ = 0;
    return;
  }
  if (str[0] == '-') {
    big_int_.resize(str.size() - 1);
    for (uint64_t i = 0; i < str.size() - 1; ++i) {
      big_int_[i] = str[i + 1] - kAsciiShift;
    }
    sign_ = 1;
  } else {
    big_int_.resize(str.size());
    for (uint64_t i = 0; i < str.size(); ++i) {
      big_int_[i] = str[i] - kAsciiShift;
    }
    sign_ = 0;
  }
}

BigInt::BigInt(std::string str) {
  if (str == "-0" || str == "0") {
    big_int_.push_back(0);
    sign_ = 0;
    return;
  }
  if (str[0] == '-') {
    big_int_.resize(str.size() - 1);
    for (uint64_t i = 0; i < str.size() - 1; ++i) {
      big_int_[i] = str[i + 1] - kAsciiShift;
    }
    sign_ = 1;
  } else {
    big_int_.resize(str.size());
    for (uint64_t i = 0; i < str.size(); ++i) {
      big_int_[i] = str[i] - kAsciiShift;
    }
    sign_ = 0;
  }
}

BigInt::BigInt(const BigInt& number) {
  sign_ = number.sign_;
  big_int_.clear();
  big_int_.resize(number.Length());
  uint64_t num = number.big_int_.size();
  for (uint64_t i = 0; i < num; ++i) {
    big_int_[i] = number.big_int_[i];
  }
}

BigInt::~BigInt() { big_int_.clear(); }

std::ostream& operator<<(std::ostream& output, const BigInt& string_out) {
  if (string_out.sign_ == 1) {
    output << "-";
  }
  for (auto i : string_out.big_int_) {
    output << i;
  }
  return output;
}

std::istream& operator>>(std::istream& input, BigInt& big_int) {
  std::string num;
  input >> num;
  big_int.big_int_.clear();
  big_int.sign_ = 0;
  int st = 0;
  if (num[0] == '-') {
    st = 1;
    big_int.sign_ = 1;
  }
  for (uint64_t i = st; i < num.size(); ++i) {
    big_int.big_int_.push_back(num[i] - '0');
  }
  while (big_int.big_int_.size() > 1 && big_int.big_int_.back() == 0) {
    big_int.big_int_.pop_back();
  }
  return input;
}

std::vector<int> BigInt::OurBigInt() const {
  std::vector<int> return_vec;
  return_vec.resize(big_int_.size());
  for (uint64_t i = 0; i < big_int_.size(); ++i) {
    return_vec[i] = big_int_[i];
  }
  return return_vec;
}

uint64_t BigInt::Length() const { return big_int_.size(); }

bool BigInt::Sign() const { return sign_ == 0; }

bool CompareLessPositive(const BigInt& first, const BigInt& second) {
  if (first.Length() < second.Length()) {
    return true;
  }
  if (second.Length() < first.Length()) {
    return false;
  }
  return !(first.OurBigInt() >= second.OurBigInt());
}

bool CompareLessNegative(const BigInt& first, const BigInt& second) {
  if (first.Length() > second.Length()) {
    return true;
  }
  if (second.Length() < first.Length()) {
    return false;
  }
  return !(first.OurBigInt() < second.OurBigInt());
}

bool operator<(const BigInt& first, const BigInt& second) {
  if (first == second) {
    return false;
  }
  if (!first.Sign() && second.Sign()) {
    return true;
  }
  if (first.Sign() && !second.Sign()) {
    return false;
  }
  if (first.Sign() && second.Sign()) {
    return CompareLessPositive(first, second);
  }
  if (!first.Sign() && !second.Sign()) {
    return CompareLessNegative(first, second);
  }
  return true;
}

bool operator==(const BigInt& first, const BigInt& second) {
  if (first.Length() < second.Length() || first.Length() > second.Length()) {
    return false;
  }
  if (first.Sign() != second.Sign()) {
    return false;
  }
  for (uint64_t i = 0; i < first.Length(); ++i) {
    if (first.OurBigInt()[i] != second.OurBigInt()[i]) {
      return false;
    }
  }
  return true;
}

bool operator>(const BigInt& first, const BigInt& second) {
  return !(first < second || first == second);
}

bool operator<=(const BigInt& first, const BigInt& second) {
  return first == second || first < second;
}

bool operator>=(const BigInt& first, const BigInt& second) {
  return !(first < second);
}

bool operator!=(const BigInt& first, const BigInt& second) {
  return !(first == second);
}

bool BigInt::CompareVectors(const std::vector<int>& arr) {
  if (big_int_.size() < arr.size()) {
    return false;
  }
  if (big_int_.size() > arr.size()) {
    return true;
  }
  if (big_int_.size() == arr.size()) {
    return big_int_ > arr;
  }
  return true;
}

BigInt& BigInt::operator=(const BigInt& other) {
  big_int_.clear();
  big_int_.resize(other.big_int_.size());
  for (uint64_t i = 0; i < other.big_int_.size(); ++i) {
    big_int_[i] = other.big_int_[i];
  }
  return *this;
}

void BigInt::Normilize() {
  std::vector<int> buffer;
  for (uint64_t i = Length() - 1; i >= 1; --i) {
    if (big_int_[i] >= kModuleOfTen) {
      big_int_[i] = big_int_[i] % kModuleOfTen;
      big_int_[i - 1] += 1;
    }
  }
  if (big_int_[0] >= kModuleOfTen) {
    buffer.resize(Length() + 1);
    buffer[0] = 1;
    buffer[1] = big_int_[0] % kModuleOfTen;
    for (uint64_t i = 2; i <= Length(); ++i) {
      buffer[i] = big_int_[i - 1];
    }
    big_int_.clear();
    big_int_ = buffer;
  }
}

std::vector<int> AddEqualLenthes(const BigInt& first, const BigInt& second) {
  uint64_t min = std::min(first.OurBigInt().size(), second.OurBigInt().size());
  uint64_t max = std::max(first.OurBigInt().size(), second.OurBigInt().size());
  std::vector<int> max_vec = std::max(first.OurBigInt(), second.OurBigInt());
  std::vector<int> min_vec = std::min(first.OurBigInt(), second.OurBigInt());
  std::vector<int> buffer;
  uint64_t minimum = min;
  uint64_t maximum = max;
  uint64_t iterable = max;
  for (uint64_t i = 0; i < minimum; ++i) {
    buffer.push_back(max_vec[--max] + min_vec[--min]);
  }
  for (uint64_t i = minimum; i < maximum; ++i) {
    buffer.push_back(max_vec[--max]);
  }
  return buffer;
}

std::vector<int> AddLessLenthes(const BigInt& first, const BigInt& second) {
  uint64_t min = std::min(first.OurBigInt().size(), second.OurBigInt().size());
  uint64_t max = std::max(first.OurBigInt().size(), second.OurBigInt().size());
  std::vector<int> max_vec = second.OurBigInt();
  std::vector<int> min_vec = first.OurBigInt();
  std::vector<int> buffer;
  uint64_t minimum = min;
  uint64_t maximum = max;
  uint64_t iterable = max;
  for (uint64_t i = 0; i < minimum; ++i) {
    buffer.push_back(max_vec[--max] + min_vec[--min]);
  }
  for (uint64_t i = minimum; i < maximum; ++i) {
    buffer.push_back(max_vec[--max]);
  }
  return buffer;
}

std::vector<int> AddGreaterLenthes(const BigInt& first, const BigInt& second) {
  uint64_t min = std::min(first.OurBigInt().size(), second.OurBigInt().size());
  uint64_t max = std::max(first.OurBigInt().size(), second.OurBigInt().size());
  std::vector<int> max_vec = first.OurBigInt();
  std::vector<int> min_vec = second.OurBigInt();
  std::vector<int> buffer;
  uint64_t minimum = min;
  uint64_t maximum = max;
  for (uint64_t i = 0; i < minimum; ++i) {
    buffer.push_back(max_vec[--max] + min_vec[--min]);
  }
  for (uint64_t i = minimum; i < maximum; ++i) {
    buffer.push_back(max_vec[--max]);
  }
  return buffer;
}

BigInt BigInt::Addition(const BigInt& other) {
  uint64_t max = std::max(this->big_int_.size(), other.big_int_.size());
  std::vector<int> buffer;
  if (this->Length() < other.Length()) {
    buffer = AddLessLenthes(*this, other);
  }
  if (this->Length() > other.Length()) {
    buffer = AddGreaterLenthes(*this, other);
  }
  if (this->Length() == other.Length()) {
    buffer = AddEqualLenthes(*this, other);
  }
  uint64_t maximum = max;
  uint64_t iterable = max;
  BigInt result;
  result.Resize(maximum);
  for (uint64_t i = 0; i < maximum; i++) {
    result.big_int_[i] = buffer[--iterable];
  }
  result.Normilize();
  return result;
}

BigInt& BigInt::operator+=(const BigInt& other) {
  BigInt num(0);
  std::vector<int> max_vec = std::max(this->big_int_, other.big_int_);
  if (!this->Sign() && other.Sign()) {
    sign_ = 0;
    if (max_vec == big_int_) {
      sign_ = 1;
    }
    *this = Substruction(other);
    return *this;
  }
  if (this->Sign() && !other.Sign()) {
    sign_ = 1;
    if (max_vec == big_int_) {
      sign_ = 0;
    }
    *this = Substruction(other);
    return *this;
  }
  *this = Addition(other);
  return *this;
}

void BigInt::Resize(uint64_t size) { big_int_.resize(size); }

BigInt operator+(const BigInt& left, const BigInt& right) {
  BigInt copy = left;
  copy += right;
  return copy;
}

void BigInt::NormilizeNegative() {
  std::vector<int> buffer;
  for (uint64_t i = Length() - 1; i >= 1; --i) {
    if (big_int_[i] < 0) {
      big_int_[i] = big_int_[i] + kModuleOfTen;
      big_int_[i - 1] -= 1;
    }
  }
  if (big_int_[0] == 0) {
    buffer.resize(Length() - 1);
    for (uint64_t i = 0; i < Length() - 1; ++i) {
      buffer[i] = big_int_[i + 1];
    }
    big_int_.clear();
    big_int_ = buffer;
  }
  if (Length() > 1 && big_int_[0] == 0) {
    uint64_t i = 0;
    while (big_int_[i] == 0 && i != Length() - 1) {
      big_int_.erase(big_int_.begin());
    }
  }
}

std::vector<int> SubEqualLenthes(const BigInt& first, const BigInt& second) {
  uint64_t min = std::min(first.OurBigInt().size(), second.OurBigInt().size());
  uint64_t max = std::max(first.OurBigInt().size(), second.OurBigInt().size());
  std::vector<int> max_vec = std::max(first.OurBigInt(), second.OurBigInt());
  std::vector<int> min_vec = std::min(first.OurBigInt(), second.OurBigInt());
  std::vector<int> buffer;
  uint64_t minimum = min;
  uint64_t maximum = max;
  uint64_t iterable = max;
  for (uint64_t i = 0; i < minimum; ++i) {
    buffer.push_back(max_vec[--max] - min_vec[--min]);
  }
  for (uint64_t i = minimum; i < maximum; ++i) {
    buffer.push_back(max_vec[--max]);
  }
  return buffer;
}

std::vector<int> SubLessLenthes(const BigInt& first, const BigInt& second) {
  uint64_t min = std::min(first.OurBigInt().size(), second.OurBigInt().size());
  uint64_t max = std::max(first.OurBigInt().size(), second.OurBigInt().size());
  std::vector<int> max_vec = second.OurBigInt();
  std::vector<int> min_vec = first.OurBigInt();
  std::vector<int> buffer;
  uint64_t minimum = min;
  uint64_t maximum = max;
  uint64_t iterable = max;
  for (uint64_t i = 0; i < minimum; ++i) {
    buffer.push_back(max_vec[--max] - min_vec[--min]);
  }
  for (uint64_t i = minimum; i < maximum; ++i) {
    buffer.push_back(max_vec[--max]);
  }
  return buffer;
}

std::vector<int> SubGreaterLenthes(const BigInt& first, const BigInt& second) {
  uint64_t min = std::min(first.OurBigInt().size(), second.OurBigInt().size());
  uint64_t max = std::max(first.OurBigInt().size(), second.OurBigInt().size());
  std::vector<int> max_vec = first.OurBigInt();
  std::vector<int> min_vec = second.OurBigInt();
  std::vector<int> buffer;
  uint64_t minimum = min;
  uint64_t maximum = max;
  for (uint64_t i = 0; i < minimum; ++i) {
    buffer.push_back(max_vec[--max] - min_vec[--min]);
  }
  for (uint64_t i = minimum; i < maximum; ++i) {
    buffer.push_back(max_vec[--max]);
  }
  return buffer;
}

BigInt BigInt::Substruction(const BigInt& other) {
  if (this->OurBigInt() == other.OurBigInt()) {
    BigInt zero(0);
    return zero;
  }
  uint64_t max = std::max(this->big_int_.size(), other.big_int_.size());
  std::vector<int> buffer;
  if (this->Length() < other.Length()) {
    buffer = SubLessLenthes(*this, other);
  }
  if (this->Length() > other.Length()) {
    buffer = SubGreaterLenthes(*this, other);
  }
  if (this->Length() == other.Length()) {
    buffer = SubEqualLenthes(*this, other);
  }
  uint64_t maximum = max;
  uint64_t iterable = max;
  BigInt result;
  result.Resize(maximum);
  for (uint64_t i = 0; i < maximum; i++) {
    result.big_int_[i] = buffer[--iterable];
  }
  result.NormilizeNegative();
  return result;
}

BigInt& BigInt::operator-=(const BigInt& other) {
  if (!this->Sign() && other.Sign()) {
    *this = Addition(other);
    sign_ = 1;
    return *this;
  }
  if (this->Sign() && !other.Sign()) {
    *this = Addition(other);
    sign_ = 0;
    return *this;
  }
  if (this->Sign() && other.Sign()) {
    int sign = 1;
    if (CompareVectors(other.OurBigInt())) {
      sign = 0;
    }
    *this = Substruction(other);
    sign_ = sign;
  }
  if (!this->Sign() && !other.Sign()) {
    int sign = 0;
    if (CompareVectors(other.OurBigInt())) {
      sign = 1;
    }
    *this = Substruction(other);
    sign_ = sign;
  }
  return *this;
}

BigInt operator-(const BigInt& left, const BigInt& right) {
  BigInt copy = left;
  copy -= right;
  return copy;
}

void BigInt::PushBack(int num) { big_int_.push_back(num); }

BigInt BigInt::MultiplyNTimes(int n) const {
  BigInt result(0);
  for (int i = 0; i < n; ++i) {
    result += *this;
  }
  return result;
}

BigInt& BigInt::operator*=(const BigInt& other) {
  BigInt res(0);
  if (*this == res || other == res) {
    *this = res;
    return *this;
  }
  int sign = 0;
  if (this->Sign() != other.Sign()) {
    sign = 1;
  }
  uint64_t times = other.Length();
  BigInt current = this->MultiplyNTimes(other.big_int_[0]);
  res.Resize(current.Length() + other.Length() - 1);
  for (uint64_t i = 0; i < other.Length(); ++i) {
    current = this->MultiplyNTimes(other.big_int_[i]);
    uint64_t j = times;
    while (j != 1) {
      current.PushBack(0);
      --j;
    }
    --times;
    res += current;
  }
  *this = res;
  sign_ = sign;
  return *this;
}

BigInt operator*(const BigInt& left, const BigInt& right) {
  BigInt copy = left;
  copy *= right;
  return copy;
}

void BigInt::MultiplyTwoTimes() {
  BigInt res = *this;
  BigInt two(2);
  res *= two;
  *this = res;
}

BigInt Quotient(const BigInt& current, const BigInt& other) {
  BigInt two(2);
  BigInt iterable(0);
  BigInt copy(0);
  BigInt one(1);
  while (copy <= current - other || copy >= current + other) {
    if (copy <= current - other) {
      BigInt cur = other;
      BigInt deg(1);
      while (copy <= current - other) {
        copy += cur;
        cur.MultiplyTwoTimes();
        deg *= two;
        iterable += deg;
      }
      iterable -= deg;
    }
    if (copy >= current + other) {
      BigInt cur = other;
      BigInt deg(1);
      while (copy >= current + other) {
        copy -= cur;
        cur.MultiplyTwoTimes();
        deg *= two;
        iterable -= deg;
      }
      iterable += deg;
    }
  }
  return iterable;
}

BigInt& BigInt::operator/=(const BigInt& other) {
  BigInt two(2);
  BigInt iterable(0);
  BigInt copy(0);
  BigInt one(1);
  int sign = 0;
  if (this->Sign() != other.Sign()) {
    sign = 1;
  }
  sign_ = other.sign_;
  if (Sign() && *this < other || !Sign() && *this > other) {
    *this = copy;
    sign_ = 0;
    return *this;
  }
  if (other == one || other == -one) {
    sign_ = sign;
    return *this;
  }
  if (*this < other + other) {
    *this = one;
    sign_ = sign;
    return *this;
  }
  iterable = Quotient(*this, other);
  *this = iterable;
  sign_ = sign;
  return *this;
}

BigInt operator/(const BigInt& left, const BigInt& right) {
  BigInt copy = left;
  copy /= right;
  return copy;
}

BigInt& BigInt::operator%=(const BigInt& other) {
  BigInt copy = *this;
  BigInt res_of_div = *this / other;
  res_of_div *= other;
  *this = copy - res_of_div;
  return *this;
}

BigInt operator%(const BigInt& left, const BigInt& right) {
  BigInt copy = left;
  copy %= right;
  return copy;
}

BigInt& BigInt::operator++() {
  BigInt one(1);
  BigInt zero(0);
  *this += one;
  if (big_int_ == zero.big_int_) {
    sign_ = 0;
  }
  return *this;
}

BigInt BigInt::operator++(int) {
  BigInt one(1);
  BigInt copy = *this;
  ++(*this);
  return copy;
}

BigInt& BigInt::operator--() {
  BigInt one(1);
  BigInt zero(0);
  *this -= one;
  if (big_int_ == zero.big_int_) {
    sign_ = 0;
  }
  return *this;
}

BigInt BigInt::operator--(int) {
  BigInt one(1);
  BigInt copy = *this;
  --(*this);
  return copy;
}

BigInt BigInt::operator-() const {
  BigInt copy = *this;
  BigInt zero(0);
  if (*this == zero) {
    return zero;
  }
  if (this->sign_ == 1) {
    copy.sign_ = 0;
  } else {
    copy.sign_ = 1;
  }
  return copy;
}
