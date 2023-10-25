#include <iostream>
#include <iterator>
#include <type_traits>
#include <typeinfo>
#include <vector>

template <typename T>
class Deque {
  static const size_t kSize = 10;

  class Index {
   public:
    Index() : x_cord_(0), y_cord_(0) {}
    Index(const size_t& index)
        : x_cord_(index / kSize), y_cord_(index % kSize) {}
    Index(const Index& other)
        : x_cord_(other.x_cord_), y_cord_(other.y_cord_) {}

    Index& operator=(const size_t& index) {
      x_cord_ = index / kSize;
      y_cord_ = index % kSize;
      return *this;
    }
    Index& operator=(const Index& other) {
      x_cord_ = other.x_cord_;
      y_cord_ = other.y_cord_;
      return *this;
    }

    ~Index() {}

    Index operator+(const Index& other) const {
      size_t ind = get_index() + other.get_index();
      return Index(ind);
    }
    Index& operator+=(const Index& other) {
      size_t ind = get_index() + other.get_index();
      *this = Index(ind);
      return *this;
    }
    Index operator-(const Index& other) const {
      size_t ind = get_index() - other.get_index();
      return Index(ind);
    }
    Index& operator-=(const Index& other) {
      size_t ind = get_index() - other.get_index();
      *this = Index(ind);
      return *this;
    }

    Index& operator++() {
      size_t ind = get_index() + 1;
      *this = Index(ind);
      return *this;
    }
    Index operator++(int) const { return Index(get_index() + 1); }
    Index& operator--() {
      size_t ind = get_index() - 1;
      *this = Index(ind);
      return *this;
    }
    Index operator--(int) const { return Index(get_index() - 1); }

    bool operator<(const Index& other) const {
      return (x_cord_ * kSize + y_cord_ <
              other.x_cord_ * kSize + other.y_cord_);
    }
    bool operator==(const Index& other) const {
      return (x_cord_ == other.x_cord_ && y_cord_ == other.y_cord_);
    }
    bool operator<=(const Index& other) const {
      return (*this < other || *this == other);
    }
    bool operator>(const Index& other) const { return !(*this <= other); }
    bool operator>=(const Index& other) const { return !(*this < other); }
    bool operator!=(const Index& other) const { return !(*this == other); }

    size_t get_x_cord() const { return x_cord_; }
    size_t get_y_cord() const { return y_cord_; }
    size_t get_index() const { return x_cord_ * kSize + y_cord_; }

   private:
    size_t x_cord_;
    size_t y_cord_;
  };

  template <bool IsConst>
  class BaseIterator {
   public:
    using difference_type = std::ptrdiff_t;
    using value_type = std::conditional_t<IsConst, const T, T>;
    using pointer = std::conditional_t<IsConst, const T*, T*>;
    using reference = std::conditional_t<IsConst, const T&, T&>;
    using iterator_category = std::random_access_iterator_tag;

    using data_ptr = typename std::conditional<IsConst, const T**, T**>::type;

    BaseIterator(data_ptr data, const Index& ind) : data_(data), ind_(ind){};

    BaseIterator& operator++() {
      ++ind_;
      return *this;
    }

    BaseIterator& operator--() {
      --ind_;
      return *this;
    }

    BaseIterator operator++(int) {
      BaseIterator tmp = *this;
      ++ind_;
      return tmp;
    }

    BaseIterator operator--(int) {
      BaseIterator tmp = *this;
      --ind_;
      return tmp;
    }

    BaseIterator operator+(const Index& val) const {
      BaseIterator copy = *this;
      copy.ind_ += val;
      return copy;
    }

    difference_type operator-(const BaseIterator& other) const {
      Index ind = ind_ - other.ind_;
      difference_type val = static_cast<std::ptrdiff_t>(ind.get_index());
      return val;
    }

    BaseIterator operator-(const Index& val) const {
      BaseIterator copy = *this;
      copy.ind_ -= val;
      return copy;
    }

    bool operator<(const BaseIterator& other) const {
      return ind_ < other.ind_;
    }

    bool operator!=(const BaseIterator& other) const {
      return (data_ != other.data_ || ind_ != other.ind_);
    }

    bool operator>(const BaseIterator& other) const {
      return (*this != other && !(*this < other));
    }

    bool operator>=(const BaseIterator& other) const {
      return !(*this < other);
    }

    bool operator<=(const BaseIterator& other) const {
      return !(*this > other);
    }

    bool operator==(const BaseIterator& other) const {
      return (!(*this != other));
    }

    BaseIterator& operator+=(const Index& val) {
      ind_ += val;
      return *this;
    }

    BaseIterator& operator-=(const Index& val) {
      ind_ -= val;
      return *this;
    }
    reference operator*() const {
      return data_[ind_.get_x_cord()][ind_.get_y_cord()];
    }

    pointer operator->() const {
      return data_[ind_.get_x_cord()] + ind_.get_y_cord();
    }

   private:
    data_ptr data_;
    Index ind_;
  };

 public:
  using iterator = BaseIterator<false>;
  using const_iterator = BaseIterator<true>;
  using reverse_iterator = std::reverse_iterator<iterator>;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;
  using value_type = T;

  Deque()
      : front_(0),
        back_(0),
        size_(0),
        ratio_(0),
        capacity_(0),
        data_(nullptr){};

  Deque(const Deque& other)
      : capacity_(other.capacity_),
        front_(other.front_),
        back_(other.back_),
        ratio_(other.ratio_) {
    T** temp_buf = new T*[ratio_];

    for (size_t i = 0; i < other.ratio_; ++i) {
      temp_buf[i] = reinterpret_cast<T*>(new uint8_t[sizeof(T) * kSize]);
    }

    for (Index i = front_; i < back_; ++i) {
      try {
        temp_buf[i.get_x_cord()][i.get_y_cord()] =
            other.data_[i.get_x_cord()][i.get_y_cord()];
      } catch (...) {
        for (Index j = i; j.get_index() != 0; --j) {
          temp_buf[j.get_x_cord()][j.get_y_cord()].~T();
          for (size_t ind = 0; ind < other.ratio_; ++ind) {
            delete[] temp_buf[ind];
          }
          delete[] temp_buf;
        }
      }
    }
    size_ = other.size_;
    std::swap(temp_buf, data_);
  }

  Deque(size_t count) : Deque() {
    for (size_t i = 0; i < count / 2; ++i) {
      push_back(T());
      push_front(T());
    }
    if (count % 2 != 0) {
      push_back(T());
    }
  }

  Deque(size_t count, const T& value) : Deque() {
    for (size_t i = 0; i < count / 2; ++i) {
      push_back(value);
      push_front(value);
    }
    if (count % 2 != 0) {
      push_back(value);
    }
  }

  ~Deque() {
    for (Index i = front_; i < back_; ++i) {
      data_[i.get_x_cord()][i.get_y_cord()].~T();
    }
    for (size_t arr = 0; arr < ratio_; ++arr) {
      delete[] reinterpret_cast<uint8_t*>(data_[arr]);
    }

    delete[] data_;
  }

  Deque& operator=(const Deque& other) {
    Deque copy(other);
    std::swap(data_, copy.data_);
    std::swap(size_, copy.size_);
    std::swap(capacity_, copy.capacity_);
    std::swap(back_, copy.back_);
    std::swap(front_, copy.front_);
    std::swap(ratio_, copy.ratio_);
    return *this;
  }

  size_t size() const { return size_; }

  bool empty() { return (size_ == 0); }

  T& operator[](size_t ind) {
    Index index = Index(ind);
    index += front_;
    return data_[index.get_x_cord()][index.get_y_cord()];
  }

  const T& operator[](size_t ind) const {
    Index index = Index(ind);
    index += front_;
    return data_[index.get_x_cord()][index.get_y_cord()];
  }

  T& at(size_t ind) {
    Index index = Index(ind);
    index += front_;
    if (ind >= size_) {
      throw std::out_of_range("WTF!?");
    }
    return data_[index.get_x_cord()][index.get_y_cord()];
  }

  const T& at(size_t ind) const {
    Index index = Index(ind);
    index += front_;
    if (ind >= size_) {
      throw std::out_of_range("WTF!?");
    }
    return data_[index.get_x_cord()][index.get_y_cord()];
  }

  void push_back(T value) {
    ++size_;

    if (back_ >= capacity_) {
      reallocate();
    }
    try {
      new (&data_[back_.get_x_cord()][back_.get_y_cord()]) T(value);
    } catch (...) {
      --size_;
      throw;
    }
    ++back_;
  }

  void pop_back() {
    data_[back_.get_x_cord()][back_.get_y_cord()].~T();
    --back_;
    --size_;
  }

  void push_front(T value) {
    ++size_;
    if (front_ == 0) {
      reallocate();
    }
    --front_;
    new (&data_[front_.get_x_cord()][front_.get_y_cord()]) T(value);
  }

  void pop_front() {
    --size_;
    data_[front_.get_x_cord()][front_.get_y_cord()].~T();
    ++front_;
  }

  void reallocate() {
    if (ratio_ == 0) {
      ratio_ = 2;
      data_ = new T*[ratio_];
      data_[0] = reinterpret_cast<T*>(new uint8_t[sizeof(T) * kSize]);
      data_[1] = reinterpret_cast<T*>(new uint8_t[sizeof(T) * kSize]);
      capacity_ = ratio_ * kSize;
      front_ = capacity_ / 2;
      back_ = capacity_ / 2;
    } else {
      T** temp_data = nullptr;
      try {
        ratio_ *= 2;
        temp_data = new T*[ratio_];
        for (size_t arr = ratio_ / 4; arr < (ratio_ / 2 + ratio_ / 4); ++arr) {
          temp_data[arr] = data_[arr - ratio_ / 4];
        }
        for (size_t arr = 0; arr < ratio_ / 4; ++arr) {
          temp_data[arr] = reinterpret_cast<T*>(new uint8_t[sizeof(T) * kSize]);
        }
        for (size_t arr = ratio_ / 2 + ratio_ / 4; arr < ratio_; ++arr) {
          temp_data[arr] = reinterpret_cast<T*>(new uint8_t[sizeof(T) * kSize]);
        }
        capacity_ = ratio_ * kSize;
        front_ += capacity_ / 4;
        back_ += capacity_ / 4;
        delete[] data_;
        data_ = temp_data;
      } catch (...) {
        ratio_ /= 2;
        delete[] temp_data;
        throw;
      }
    }
  }

  iterator begin() { return iterator(data_, front_); }
  iterator end() { return iterator(data_, back_); }
  const_iterator cbegin() const {
    return const_iterator(const_cast<const T**>(data_), front_);
  }
  const_iterator cend() const {
    return const_iterator(const_cast<const T**>(data_), back_);
  }
  reverse_iterator rbegin() { return reverse_iterator(end()); }
  reverse_iterator rend() { return reverse_iterator(begin()); }
  const_reverse_iterator crbegin() const {
    return const_reverse_iterator(end());
  }
  const_reverse_iterator crend() const {
    return const_reverse_iterator(begin());
  }

  void insert(iterator iter, const T& val) {
    size_t diff = iter - begin();
    push_back(val);
    size_t ind = size();
    for (ind--; ind > diff; ind--) {
      std::swap((*this)[ind - 1], (*this)[ind]);
    }
  }
  void erase(iterator iter) {
    size_t diff = iter - begin();
    size_t ind = diff + 1;
    for (; ind < size_; ind++) {
      std::swap((*this)[ind - 1], (*this)[ind]);
    }
    pop_back();
  }

 private:
  T** data_;
  size_t size_;
  size_t capacity_;
  size_t ratio_;
  Index front_;
  Index back_;
};