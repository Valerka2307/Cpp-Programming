#pragma once
#include <algorithm>
#include <vector>

template <size_t N, size_t M, typename T = int64_t>
class Matrix {
 public:
  std::vector<std::vector<T>> matrix;
  Matrix() {
    matrix.resize(N);
    for (size_t inc = 0; inc < N; ++inc) {
      matrix[inc].resize(M);
    }
  }
  explicit Matrix(std::vector<std::vector<T>>& vec) {
    matrix.resize(vec.size());
    for (size_t inc = 0; inc < vec.size(); ++inc) {
      matrix[inc].resize(vec[inc].size());
    }
    for (size_t inc = 0; inc < vec.size(); ++inc) {
      for (size_t jinc = 0; jinc < vec[inc].size(); ++jinc) {
        matrix[inc][jinc] = vec[inc][jinc];
      }
    }
  }
  explicit Matrix(T elem) {
    matrix.resize(N);
    for (size_t inc = 0; inc < N; ++inc) {
      matrix[inc].resize(M);
    }
    for (size_t inc = 0; inc < N; ++inc) {
      for (size_t jinc = 0; jinc < M; ++jinc) {
        matrix[inc][jinc] = elem;
      }
    }
  }
  Matrix(const Matrix<N, M, T>& matr) { matrix = matr.matrix; }
  ~Matrix() {
    for (size_t inc = 0; inc < N; ++inc) {
      matrix[inc].clear();
    }
    matrix.clear();
  }
  const T& operator()(size_t inc, size_t jinc) const {
    return matrix[inc][jinc];
  }
  T& operator()(size_t inc, size_t jinc) { return matrix[inc][jinc]; };
  Matrix<N, M, T>& operator+=(const Matrix<N, M, T>& r_matrix) {
    for (size_t inc = 0; inc < N; ++inc) {
      for (size_t jinc = 0; jinc < M; ++jinc) {
        this->matrix[inc][jinc] += r_matrix.matrix[inc][jinc];
      }
    }
    return *this;
  }
  Matrix<N, M, T>& operator-=(const Matrix<N, M, T>& r_matrix) {
    for (size_t inc = 0; inc < N; ++inc) {
      for (size_t jinc = 0; jinc < M; ++jinc) {
        this->matrix[inc][jinc] -= r_matrix.matrix[inc][jinc];
      }
    }
    return *this;
  }
  template <size_t K, size_t L>
  Matrix<N, L, T>& operator*=(const Matrix<K, L, T>& r_matrix) {
    Matrix<N, L, T> result;
    for (size_t inc = 0; inc < N; ++inc) {
      for (size_t jinc = 0; jinc < L; ++jinc) {
        result.matrix[inc][jinc] = this->matrix[inc][jinc];
        this->matrix[inc][jinc] = 0;
      }
    }
    for (size_t inc = 0; inc < N; ++inc) {
      for (size_t jinc = 0; jinc < L; ++jinc) {
        for (size_t pinc = 0; pinc < L; ++pinc) {
          this->matrix[inc][jinc] +=
              result.matrix[inc][pinc] * r_matrix.matrix[pinc][jinc];
        }
      }
    }
    return *this;
  }
  Matrix<N, M, T>& operator*=(T value) {
    for (size_t inc = 0; inc < N; ++inc) {
      for (size_t jinc = 0; jinc < M; ++jinc) {
        this->matrix[inc][jinc] *= value;
      }
    }
    return *this;
  }

  template <size_t K>
  friend Matrix<N, K, T> operator*(const Matrix<N, M, T>& l_value,
                                   const Matrix<M, K, T>& r_value) {
    Matrix<N, K, T> res;
    for (size_t inc = 0; inc < N; ++inc) {
      for (size_t jinc = 0; jinc < K; ++jinc) {
        for (size_t pinc = 0; pinc < M; ++pinc) {
          res.matrix[inc][jinc] +=
              l_value.matrix[inc][pinc] * r_value.matrix[pinc][jinc];
        }
      }
    }
    return res;
  }
  T Trace() {
    T value = 0;
    for (size_t inc = 0; inc < N; ++inc) {
      value += this->matrix[inc][inc];
    }
    return value;
  }
  Matrix<M, N, T> Transposed() {
    Matrix<M, N, T> res;
    for (size_t inc = 0; inc < M; ++inc) {
      for (size_t jinc = 0; jinc < N; ++jinc) {
        res.matrix[inc][jinc] = this->matrix[jinc][inc];
      }
    }
    return res;
  }
};

template <size_t N, size_t M, typename T>
Matrix<N, M, T> operator+(const Matrix<N, M, T>& l_matrix,
                          const Matrix<N, M, T>& r_matrix) {
  Matrix<N, M, T> res = l_matrix;
  res += r_matrix;
  return res;
}

template <size_t N, size_t M, typename T>
Matrix<N, M, T> operator-(const Matrix<N, M, T>& l_matrix,
                          const Matrix<N, M, T>& r_matrix) {
  Matrix<N, M, T> res = l_matrix;
  res -= r_matrix;
  return res;
}

template <size_t N, size_t M, typename T>
Matrix<N, M, T> operator*(const Matrix<N, M, T>& l_value, T r_value) {
  Matrix<N, M, T> res = l_value;
  res *= r_value;
  return res;
}

template <size_t N, size_t M, typename T>
Matrix<N, M, T> operator*(T l_value, const Matrix<N, M, T>& r_value) {
  Matrix<N, M, T> res = r_value;
  res *= l_value;
  return res;
}
