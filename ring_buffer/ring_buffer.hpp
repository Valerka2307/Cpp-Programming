#pragma once
#include <cstddef>

class RingBuffer {
  int front_;
  int last_elem_;
  size_t size_;
  size_t* arr_;
  size_t cap_;

 public:
  explicit RingBuffer(size_t capacity) {
    arr_ = new size_t[capacity];
    size_ = 0;
    cap_ = capacity;
    this->front_ = 0;
    this->last_elem_ = 0;
  }
  ~RingBuffer() { delete[] arr_; };
  size_t Size() const { return size_; }

  bool Empty() const { return size_ == 0; }

  bool TryPush(int element) {
    if (Size() >= cap_) {
      return false;
    }
    arr_[front_++ % cap_] = element;
    ++size_;
    return true;
  }

  bool TryPop(int* element) {
    if (Empty()) {
      return false;
    }
    *element = (int)arr_[last_elem_++ % cap_];
    --size_;
    return true;
  }
};
