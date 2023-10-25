#include "deque.hpp"
#include <tuple>

struct NotDefaultConstructible {
  NotDefaultConstructible() = delete;
  NotDefaultConstructible(int data) : data(data) {}
  int data;

  auto operator<=>(const NotDefaultConstructible&) const = default;
};

void f(int a){};

int main() {

Deque<NotDefaultConstructible> deq;
f(deq.rend());
deq.rbegin().data;
deq.rend()->data;
}
