#pragma once
#include <iterator>
#include <memory>

template <typename T, typename Allocator = std::allocator<T>>
class List {
 private:
  struct BaseNode {
    BaseNode* prev = nullptr;
    BaseNode* next = nullptr;

    BaseNode() {}
    BaseNode(BaseNode* previous_node, BaseNode* next_node)
        : prev(previous_node), next(next_node) {}
  };

  struct Node : public BaseNode {
    T value;

    Node() = default;
    explicit Node(const T& value) : BaseNode(), value(value){};
    Node& operator=(const Node& another) { value = another.value; };
  };

  template <bool IsConst>
  class BaseIterator {
    std::conditional_t<IsConst, const Node*, Node*> ptr_;

   public:
    using difference_type = std::ptrdiff_t;
    using value_type = std::conditional_t<IsConst, const T, T>;
    using pointer = std::conditional_t<IsConst, const T*, T*>;
    using reference = std::conditional_t<IsConst, const T&, T&>;
    using iterator_category = std::bidirectional_iterator_tag;

    BaseIterator(Node* node) : ptr_(node){};

    BaseIterator& operator++() {
      ptr_ = static_cast<Node*>(ptr_->next);
      return *this;
    }

    BaseIterator& operator--() {
      ptr_ = static_cast<Node*>(ptr_->prev);
      return *this;
    }

    bool operator==(const BaseIterator& iter) const {
      return (ptr_ == iter.ptr_);
    }

    bool operator!=(const BaseIterator& iter) const {
      return (ptr_ != iter.ptr_);
    }

    reference operator*() { return ptr_->value; }

    pointer operator->() { return &(ptr_->value); }

    BaseIterator operator++(int) {
      BaseIterator tmp = *this;
      ++(*this);
      return tmp;
    }

    BaseIterator operator--(int) {
      BaseIterator tmp = *this;
      --(*this);
      return tmp;
    }
  };

  using node_alloc =
      typename std::allocator_traits<Allocator>::template rebind_alloc<Node>;
  using allocate_traits = typename std::allocator_traits<node_alloc>;

 public:
  using iterator = BaseIterator<false>;
  using const_iterator = BaseIterator<true>;
  using reverse_iterator = std::reverse_iterator<iterator>;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;
  using value_type = T;
  using allocator_type = Allocator;

  List()
      : node_allocator_(Allocator()),
        fake_head_(BaseNode()),
        head_(static_cast<Node*>(&fake_head_)),
        size_(0) {
    head_->next = head_;
    head_->prev = head_;
  }

  List(size_t count, const T& value, const Allocator& alloc = Allocator())
      : node_allocator_(alloc),
        fake_head_(BaseNode()),
        head_(static_cast<Node*>(&fake_head_)),
        size_(0) {
    head_->next = head_;
    head_->prev = head_;
    try {
      for (size_t i = 0; i < count; ++i) {
        push_back(value);
      }
    } catch (...) {
      clear();
      throw;
    }
  }

  explicit List(size_t count, const Allocator& alloc = Allocator())
      : node_allocator_(alloc),
        fake_head_(BaseNode()),
        head_(static_cast<Node*>(&fake_head_)),
        size_(0) {
    head_->next = head_;
    head_->prev = head_;
    try {
      for (size_t i = 0; i < count; ++i) {
        push_back();
      }
    } catch (...) {
      clear();
      throw;
    }
  }

  List(const List& other)
      : node_allocator_(std::allocator_traits<allocator_type>::
                            select_on_container_copy_construction(
                                other.get_node_allocator())),
        fake_head_(BaseNode()),
        head_(static_cast<Node*>(&fake_head_)),
        size_(0) {
    head_->next = head_;
    head_->prev = head_;
    try {
      for (const T& element : other) {
        push_back(element);
      }
    } catch (...) {
      clear();
      throw;
    }
  }

  List(std::initializer_list<T> init, const Allocator& alloc = Allocator())
      : node_allocator_(alloc),
        fake_head_(BaseNode()),
        head_(static_cast<Node*>(&fake_head_)),
        size_(0) {
    head_->next = static_cast<Node*>(head_);
    head_->prev = static_cast<Node*>(head_);
    try {
      for (const T& element : init) {
        push_back(element);
      }
    } catch (...) {
      clear();
      throw;
    }
  }

  ~List() { clear(); }

  void clear() {
    while (!empty()) {
      pop_back();
    }
  }

  Allocator get_allocator() const noexcept { return node_allocator_; }
  inline size_t size() const { return size_; }
  inline bool empty() const { return (size_ == 0); }

  template <typename... Args>
  void emplace_back(Args&&... args) {
    Node* tmp = nullptr;
    try {
      tmp = allocate_traits::allocate(node_allocator_, 1);
    } catch (...) {
      throw;
    }
    try {
      allocate_traits::construct(node_allocator_, tmp,
                                 std::forward<Args>(args)...);
    } catch (...) {
      allocate_traits::deallocate(node_allocator_, tmp, 1);
      throw;
    }
    insert_node_after_node(static_cast<Node*>(head_->prev), tmp);
    ++size_;
  }

  template <typename... Args>
  void emplace_front(Args&&... args) {
    Node* tmp = nullptr;
    try {
      tmp = allocate_traits::allocate(node_allocator_, 1);
    } catch (...) {
      throw;
    }
    try {
      allocate_traits::construct(node_allocator_, tmp,
                                 std::forward<Args>(args)...);
    } catch (...) {
      allocate_traits::deallocate(node_allocator_, tmp, 1);
      throw;
    }
    insert_node_after_node(static_cast<Node*>(head_), tmp);
    ++size_;
  }

  T& front() { return *(static_cast<Node*>(head_->next)); }
  const T& front() const { return *(static_cast<Node*>(head_->next)); }
  T& back() { return *(static_cast<Node*>(head_->prev)); }
  const T& back() const { return *(static_cast<Node*>(head_->prev)); }

  void delete_node(Node* node) {
    if (node->prev != nullptr) {
      node->prev->next = static_cast<Node*>(node->next);
    }
    if (node->next != nullptr) {
      node->next->prev = static_cast<Node*>(node->prev);
    }
    allocate_traits::destroy(node_allocator_, static_cast<Node*>(node));
    allocate_traits::deallocate(node_allocator_, static_cast<Node*>(node), 1);
    --size_;
  }

  void push_back() { emplace_back(); }
  void push_back(const T& val) { emplace_back(val); }
  void push_back(T&& val) { emplace_back(std::move(val)); }
  void pop_back() { delete_node(static_cast<Node*>(head_->prev)); }
  void push_front(const T& val) { emplace_front(val); }
  void push_front(T&& val) { emplace_front(std::move(val)); }
  void pop_front() { delete_node(static_cast<Node*>(head_->next)); }

  List& operator=(const List& other) {
    if (this != &other) {
      if (allocate_traits::propagate_on_container_copy_assignment::value) {
        node_allocator_ = other.get_node_allocator();
      }
      size_t min_size = std::min(size(), other.size());
      auto cur = begin();
      auto cur_other = other.begin();
      for (size_t i = 0; i < min_size; ++i) {
        *cur = *cur_other;
        ++cur;
        ++cur_other;
      }
      size_t cur_size = size();
      size_t cur_other_size = other.size();
      while (size() != other.size()) {
        if (cur_size >= cur_other_size) {
          pop_back();
        } else {
          push_back(*cur_other);
          ++cur_other;
        }
      }
    }
    return *this;
  }

  iterator begin() const { return iterator(static_cast<Node*>(head_->next)); }
  iterator end() const { return iterator(static_cast<Node*>(head_)); }
  const_iterator cbegin() const {
    return const_iterator(static_cast<Node*>(head_->next));
  }
  const_iterator cend() const {
    return const_iterator(static_cast<Node*>(head_));
  }
  iterator rbegin() const { return reverse_iterator(end()); }
  iterator rend() const { return reverse_iterator(begin()); }
  const_iterator crbegin() const { return const_reverse_iterator(cend()); }
  const_iterator crend() const { return const_reverse_iterator(cbegin()); }

 private:
  node_alloc get_node_allocator() const noexcept { return node_allocator_; }

  void connect_nodes(Node* left, Node* right) {
    left->next = static_cast<Node*>(right);
    right->prev = static_cast<Node*>(left);
  }

  void append(Node* node, Node* end) {
    Node* last = static_cast<Node*>(end->prev);
    connect_nodes(static_cast<Node*>(last), static_cast<Node*>(node));
    connect_nodes(static_cast<Node*>(node), static_cast<Node*>(end));
  }

  void remove(Node* node) {
    Node* prev_node = static_cast<Node*>(node->prev);
    Node* next_node = static_cast<Node*>(node->next);
    connect_nodes(static_cast<Node*>(prev_node), static_cast<Node*>(next_node));

    allocate_traits::destroy(node_allocator_, static_cast<Node*>(node));
    allocate_traits::deallocate(node_allocator_, static_cast<Node*>(node), 1);
    size_--;
  }

  void insert_node_after_node(Node* node, Node* current) {
    current->prev = node;
    if (node != nullptr) {
      current->next = node->next;
      node->next = current;
    }
    if (current->next != nullptr) {
      current->next->prev = current;
    }
  }

  void insert_val_after_node(Node* node, const T& value) {
    Node* tmp = allocate_traits::allocate(node_allocator_, 1);
    try {
      allocate_traits::construct(node_allocator_, static_cast<Node*>(tmp),
                                 value);
    } catch (...) {
      allocate_traits::deallocate(node_allocator_, static_cast<Node*>(tmp), 1);
      throw;
    }
    insert_node_after_node(static_cast<Node*>(node), static_cast<Node*>(tmp));
  }

  BaseNode fake_head_;
  Node* head_;
  node_alloc node_allocator_;
  size_t size_;
};
