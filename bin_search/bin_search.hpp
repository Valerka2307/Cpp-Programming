#pragma once

int* LowerBound(int* first, int* last, int value) {
  while (last > first) {
    int* middle = first + (last - first) / 2;
    if (*middle == value) {
      return middle;
    }
    if (*middle > value) {
      last = middle;
    } else {
      first = middle + 1;
    }
  }
  return last;
}
