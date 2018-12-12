#ifndef OPERO_MATRIX_HPP_
#define OPERO_MATRIX_HPP_

#include <stdint.h>
#include <array>
#include <initializer_list>
#include <iomanip>
#include <iterator>
#include <sstream>

namespace opero {
template <typename _Tp, uint64_t _Nr, uint64_t _Nc,
          typename _Al = std::allocator<_Tp>>
class matrix {
 public:
  typedef _Al allocator_type;
  typedef typename _Al::value_type value_type;
  typedef typename _Al::reference reference;
  typedef typename _Al::const_reference const_reference;
  typedef typename _Al::difference_type difference_type;
  typedef typename _Al::size_type size_type;
  typedef typename _Al::pointer pointer;
  typedef typename _Al::const_pointer const_pointer;
  typedef typename _Al::pointer iterator;
  typedef typename _Al::const_pointer const_iterator;
  typedef std::reverse_iterator<iterator> reverse_iterator;
  typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

  matrix() { fill(_Tp()); }
  matrix(_Tp val) {
    fill(_Tp());
    for (uint64_t i = 0; i < _Nr && i < _Nc; ++i) {
      __data[i][i] = val;
    }
  }

  matrix(const std::initializer_list<_Tp>& mat) {
    fill(_Tp());
    if (_Nr * _Nc != mat.size()) {
      std::__throw_out_of_range_fmt(
          __N("initializer_list.size() (which is %zu) > __Ne (which is %zu)"),
          mat.size(), _Nr, _Nc);
    }
    std::copy(mat.begin(), mat.end(), __data.data()->data());
  }
  matrix(const std::initializer_list<std::initializer_list<_Tp>>& mat) {
    fill(_Tp());
    if (mat.size() != _Nr) {
      std::__throw_out_of_range_fmt(
          __N("initializer_list.size() (which is %zu) != _Nr (which is %zu)"),
          mat.size(), _Nr);
    }
    typename std::array<std::array<_Tp, _Nc>, _Nr>::iterator index =
        __data.begin();
    for (typename std::initializer_list<
             std::initializer_list<_Tp>>::const_iterator it = mat.begin();
         it != mat.end(); ++it) {
      if (it->size() != _Nc) {
        std::__throw_out_of_range_fmt(__N("initializer_list.size() (which is "
                                          "%zu) != _Nc (which is %zu)"),
                                      it->size(), _Nc);
      }
      std::copy(it->begin(), it->end(), index->data());
      ++index;
    }
  }

  template <uint64_t _N>
  matrix(const std::array<_Tp, _N>& vec) {
    fill(_Tp());
    if (_N == _Nr) {
      for (uint64_t i = 0; i < _Nr; ++i) {
        __data[0][i] = vec[i];
      }
    } else if (_N == _Nc) {
      __data[0] = vec;
    } else {
      std::__throw_out_of_range_fmt(
          __N("_N (%zu) must be either _Nr (%zu) or _Nc(%zu)"), _N, _Nc, _Nr);
    }
  }
  matrix(const std::array<std::array<_Tp, _Nc>, _Nr>& mat) : __data(mat) {}

  template <class _Iter>
  matrix(_Iter first, _Iter last) {
    fill(_Tp());
    std::copy(first, last, __data.data()->data());
  }

  matrix(const matrix& mat) : __data(mat.__data) {}

  ~matrix() {}

  matrix& operator=(const matrix& mat) {
    __data = mat.__data;
    return *this;
  }
  matrix& operator=(const std::initializer_list<_Tp>& mat) {
    if (_Nr * _Nc != mat.size()) {
      std::__throw_out_of_range_fmt(
          __N("initializer_list.size() (which is %zu) > __Ne (which is %zu)"),
          mat.size(), _Nr, _Nc);
    }
    std::copy(mat.begin(), mat.end(), __data.data()->data());
    return *this;
  }

  matrix& operator=(
      const std::initializer_list<std::initializer_list<_Tp>>& mat) {
    if (mat.size() != _Nr) {
      std::__throw_out_of_range_fmt(
          __N("initializer_list.size() (which is %zu) != _Nr (which is %zu)"),
          mat.size(), _Nr);
    }
    typename std::array<std::array<_Tp, _Nc>, _Nr>::iterator index =
        __data.begin();
    for (typename std::initializer_list<
             std::initializer_list<_Tp>>::const_iterator it = mat.begin();
         it != mat.end(); ++it) {
      if (it->size() != _Nc) {
        std::__throw_out_of_range_fmt(__N("initializer_list.size() (which is "
                                          "%zu) != _Nc (which is %zu)"),
                                      it->size(), _Nc);
      }
      std::copy(it->begin(), it->end(), index->data());
      ++index;
    }
    return *this;
  }

  template <uint64_t _N>
  matrix& operator=(const std::array<_Tp, _N>& vec) {
    fill(_Tp());
    if (_N == _Nr) {
      for (uint64_t i = 0; i < _Nr; ++i) {
        __data[0][i] = vec[i];
      }
    } else if (_N == _Nc) {
      __data[0] = vec;
    } else {
      std::__throw_out_of_range_fmt(
          __N("_N (%zu) must be either _Nr (%zu) or _Nc(%zu)"), _N, _Nc, _Nr);
    }
    return *this;
  }
  matrix& operator=(const std::array<std::array<_Tp, _Nc>, _Nr>& mat) {
    __data = mat;
    return *this;
  }

  reference at(uint64_t n) {
    if (n >= _Nr * _Nc) {
      std::__throw_out_of_range_fmt(
          __N("array::at:n (which is %zu) >= __Ne (which is %zu)"), n,
          _Nr * _Nc);
    }
    return reference(__data[n / _Nc][n % _Nc]);
  }
  constexpr const_reference at(uint64_t n) const {
    if (n >= _Nr * _Nc) {
      std::__throw_out_of_range_fmt(
          __N("array::at:n (which is %zu) >= __Ne (which is %zu)"), n,
          _Nr * _Nc);
    }
    return const_reference(__data[n / _Nc][n % _Nc]);
  }

  reference at(uint64_t r, uint64_t c) {
    if (r > _Nr) {
      std::__throw_out_of_range_fmt(
          __N("std::at:__nr (which is %zu) >= __Nr (which is %zu)"), r, _Nr);
    } else if (c >= _Nc) {
      std::__throw_out_of_range_fmt(
          __N("std::at:__nc (which is %zu) >= __Nc (which is %zu)"), c, _Nc);
    } else {
      return reference(__data[r][c]);
    }
  }
  constexpr const_reference at(uint64_t r, uint64_t c) const {
    if (r > _Nr) {
      std::__throw_out_of_range_fmt(
          __N("std::at:__nr (which is %zu) >= __Nr (which is %zu)"), r, _Nr);
    } else if (c >= _Nc) {
      std::__throw_out_of_range_fmt(
          __N("std::at:__nc (which is %zu) >= __Nc (which is %zu)"), c, _Nc);
    } else {
      return const_reference(__data[r][c]);
    }
  }

  std::array<_Tp, _Nc>& operator[](uint64_t r) noexcept { return __data[r]; }
  const std::array<_Tp, _Nc>& operator[](uint64_t r) const noexcept {
    return __data[r];
  }

  reference operator()(uint64_t r, uint64_t c) noexcept {
    return reference(__data[r][c]);
  }
  constexpr const_reference operator()(uint64_t r, uint64_t c) const noexcept {
    return const_reference(__data[r][c]);
  }

  iterator begin() { return iterator(__data.front().begin()); }
  const_iterator begin() const {
    return const_iterator(__data.front().begin());
  }
  const_iterator cbegin() const {
    return const_iterator(__data.front().begin());
  }
  iterator end() { return iterator(__data.back().end()); }
  const_iterator end() const { return iterator(__data.back().end()); }
  const_iterator cend() const { return iterator(__data.back().end()); }

  constexpr inline bool empty() const noexcept { return _Nr * _Nc == 0; }
  constexpr inline uint64_t size() const noexcept { return _Nr * _Nc; }
  constexpr inline uint64_t rows() const noexcept { return _Nr; }
  constexpr inline uint64_t cols() const noexcept { return _Nc; }

  void fill(_Tp val) noexcept {
    for (uint64_t r = 0; r < _Nr; ++r) {
      __data[r].fill(val);
    }
  }

  void diag(_Tp val) noexcept {
    for (uint64_t i = 0; i < _Nr && i < _Nc; ++i) {
      __data[i][i] = val;
    }
  }

  inline void operator()(_Tp (*func)(_Tp)) {
    for (uint64_t r = 0; r < _Nr; ++r) {
      for (uint64_t c = 0; c < _Nc; ++c) {
        __data[r][c] = func(__data[r][c]);
      }
    }
  }
  inline void operator()(_Tp (*func)(_Tp, uint64_t, uint64_t)) {
    for (uint64_t r = 0; r < _Nr; ++r) {
      for (uint64_t c = 0; c < _Nc; ++c) {
        __data[r][c] = func(__data[r][c], r, c);
      }
    }
  }

  inline void swap_row(uint64_t a, uint64_t b) {
    for (uint64_t r = 0; r < _Nr; ++r) {
      std::swap(__data[r][a], __data[r][b]);
    }
  }

  inline void swap_col(uint64_t a, uint64_t b) {
    for (uint64_t c = 0; c < _Nc; ++c) {
      std::swap(__data[a][c], __data[b][c]);
    }
  }

  std::string dump() const {
    std::stringstream ss;
    ss << '[';
    for (uint64_t r = 0; r < _Nr - 1; ++r) {
      ss << '[';
      for (uint64_t c = 0; c < _Nc - 1; ++c) {
        ss << __data[r][c] << ',';
      }
      ss << __data[r][_Nc - 1] << ']' << ',';
    }
    ss << '[';
    for (uint64_t c = 0; c < _Nc - 1; ++c) {
      ss << __data[_Nc - 1][c] << ',';
    }
    ss << __data[_Nr - 1][_Nc - 1] << ']' << ']';
    return ss.str();
  }
  std::string pretty() const {
    std::stringstream ss;
    for (uint64_t r = 0; r < _Nr - 1; ++r) {
      for (uint64_t c = 0; c < _Nc - 1; ++c) {
        ss << std::setw(10) << __data[r][c] << ' ';
      }
      ss << std::setw(10) << __data[r][_Nc - 1] << '\n';
    }
    for (uint64_t c = 0; c < _Nc - 1; ++c) {
      ss << std::setw(10) << __data[_Nc - 1][c] << ' ';
    }
    ss << std::setw(10) << __data[_Nr - 1][_Nc - 1] << '\n';
    return ss.str();
  }

  template <typename _T>
  inline matrix& operator+=(const _T& rhs) {
    *this = *this + rhs;
    return *this;
  }
  template <typename _T>
  inline matrix& operator-=(const _T& rhs) {
    *this = *this - rhs;
    return *this;
  }
  template <typename _T>
  inline matrix& operator*=(const _T& rhs) {
    *this = *this * rhs;
    return *this;
  }
  template <typename _T>
  inline matrix& operator/=(const _T& rhs) {
    *this = *this / rhs;
    return *this;
  }

 private:
  std::array<std::array<_Tp, _Nc>, _Nr> __data;
  // pointer __data = nullptr;
};

template <typename _Tp, uint64_t _Nr, uint64_t _Nc>
inline std::ostream& operator<<(std::ostream& out,
                                const matrix<_Tp, _Nr, _Nc>& rhs) {
  out << rhs.dump();
  return out;
}

template <typename _TpA, typename _TpB, uint64_t _NrA, uint64_t _NrB,
          uint64_t _NcA, uint64_t _NcB>
inline bool operator==(const matrix<_TpA, _NrA, _NcA>& lhs,
                       const matrix<_TpB, _NrB, _NcB>& rhs) {
  if (_NrA != _NrB || _NcA != _NcB) {
    return false;
  }
  return std::equal(lhs.begin(), lhs.end(), rhs.begin());
}
template <typename _TpA, typename _TpB, uint64_t _NrA, uint64_t _NrB,
          uint64_t _NcA, uint64_t _NcB>
inline bool operator!=(const matrix<_TpA, _NrA, _NcA>& lhs,
                       const matrix<_TpB, _NrB, _NcB>& rhs) {
  return !(lhs == rhs);
}
template <typename _TpA, typename _TpB, uint64_t _NrA, uint64_t _NrB,
          uint64_t _NcA, uint64_t _NcB>
inline bool operator<(const matrix<_TpA, _NrA, _NcA>& lhs,
                      const matrix<_TpB, _NrB, _NcB>& rhs) {
  return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(),
                                      rhs.end());
}
template <typename _TpA, typename _TpB, uint64_t _NrA, uint64_t _NrB,
          uint64_t _NcA, uint64_t _NcB>
inline bool operator>(const matrix<_TpA, _NrA, _NcA>& lhs,
                      const matrix<_TpB, _NrB, _NcB>& rhs) {
  return rhs < lhs;
}
template <typename _TpA, typename _TpB, uint64_t _NrA, uint64_t _NrB,
          uint64_t _NcA, uint64_t _NcB>
inline bool operator<=(const matrix<_TpA, _NrA, _NcA>& lhs,
                       const matrix<_TpB, _NrB, _NcB>& rhs) {
  return !(lhs > rhs);
}
template <typename _TpA, typename _TpB, uint64_t _NrA, uint64_t _NrB,
          uint64_t _NcA, uint64_t _NcB>
inline bool operator>=(const matrix<_TpA, _NrA, _NcA>& lhs,
                       const matrix<_TpB, _NrB, _NcB>& rhs) {
  return !(lhs < rhs);
}
template <typename _TpA, typename _TpB, uint64_t _Nr, uint64_t _Nc>
inline matrix<_TpA, _Nr, _Nc> operator+(const matrix<_TpA, _Nr, _Nc>& lhs,
                                        const _TpB& rhs) {
  matrix<_TpA, _Nr, _Nc> mat(lhs);
  for (typename matrix<_TpA, _Nr, _Nc>::iterator it = mat.begin();
       it != mat.end(); ++it) {
    *it += *rhs;
  }
}
template <typename _TpA, typename _TpB, uint64_t _NrA, uint64_t _NrB,
          uint64_t _NcA, uint64_t _NcB>
inline matrix<_TpA, std::min(_NrA, _NrB), std::min(_NcA, _NcB)> operator+(
    const matrix<_TpA, _NrA, _NcA>& lhs, const matrix<_TpB, _NrB, _NcB>& rhs) {
  matrix<_TpA, std::min(_NrA, _NrB), std::min(_NcA, _NcB)> mat;
  for (uint64_t r = 0; r < mat.rows(); ++r) {
    for (uint64_t c = 0; c < mat.cols(); ++c) {
      mat[r][c] = lhs[r][c] + rhs[r][c];
    }
  }
  return mat;
}
template <typename _TpA, typename _TpB, uint64_t _Nr, uint64_t _Nc>
inline matrix<_TpA, _Nr, _Nc> operator-(const matrix<_TpA, _Nr, _Nc>& lhs,
                                        const _TpB& rhs) {
  matrix<_TpA, _Nr, _Nc> mat(lhs);
  for (typename matrix<_TpA, _Nr, _Nc>::iterator it = mat.begin();
       it != mat.end(); ++it) {
    *it += *rhs;
  }
}
template <typename _TpA, typename _TpB, uint64_t _NrA, uint64_t _NrB,
          uint64_t _NcA, uint64_t _NcB>
inline matrix<_TpA, std::min(_NrA, _NrB), std::min(_NcA, _NcB)> operator-(
    const matrix<_TpA, _NrA, _NcA>& lhs, const matrix<_TpB, _NrB, _NcB>& rhs) {
  matrix<_TpA, std::min(_NrA, _NrB), std::min(_NcA, _NcB)> mat;
  for (uint64_t r = 0; r < mat.rows(); ++r) {
    for (uint64_t c = 0; c < mat.cols(); ++c) {
      mat[r][c] = lhs[r][c] - rhs[r][c];
    }
  }
  return mat;
}
template <typename _TpA, typename _TpB, uint64_t _Nr, uint64_t _Nc>
inline matrix<_TpA, _Nr, _Nc> operator*(const matrix<_TpA, _Nr, _Nc>& lhs,
                                        const _TpB& rhs) {
  matrix<_TpA, _Nr, _Nc> mat(lhs);
  for (typename matrix<_TpA, _Nr, _Nc>::iterator it = mat.begin();
       it != mat.end(); ++it) {
    *it += *rhs;
  }
}
template <typename _TpA, typename _TpB, uint64_t _NrA, uint64_t _NrB,
          uint64_t _NcA, uint64_t _NcB>
inline matrix<_TpA, _NrA, _NcB> operator*(const matrix<_TpA, _NrA, _NcA>& lhs,
                                          const matrix<_TpB, _NrB, _NcB>& rhs) {
  matrix<_TpA, _NrA, _NcB> mat;
  for (uint64_t r = 0; r < mat.rows(); ++r) {
    for (uint64_t c = 0; c < mat.cols(); ++c) {
      for (uint64_t k = 0; k < _NcA; ++k) {
        mat[r][c] += (lhs[r][k] * rhs[k][c]);
      }
    }
  }
  return mat;
}

template <typename _TpA, uint64_t _Nr, uint64_t _Nc>
inline matrix<_TpA, _Nr, _Nc> operator^(const matrix<_TpA, _Nr, _Nc>& lhs,
                                        const uint64_t& rhs) {
  if (rhs == 0) {
    return matrix<_TpA, _Nr, _Nc>(_TpA(1));
  }
  matrix<_TpA, _Nr, _Nc> mat(lhs);
  for (uint64_t i = 1; i < rhs; ++i) {
    mat *= mat;
  }
  return mat;
}

}  // namespace opero

#endif  // OPERO_MATRIX_HPP_
