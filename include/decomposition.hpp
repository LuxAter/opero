#ifndef OPERO_DECOMPOSITION_HPP_
#define OPERO_DECOMPOSITION_HPP_

#include <cmath>

#include "matrix.hpp"

namespace opero {

namespace {
  template <typename _Tp, uint64_t _N>
  std::pair<matrix<_Tp, _N, _N>, uint64_t> pivotize(
      const matrix<_Tp, _N, _N>& A) {
    matrix<_Tp, _N, _N> id(_Tp(1));
    uint64_t n = 0;
    for (uint64_t j = 0; j < _N; ++j) {
      _Tp row = j;
      for (uint64_t k = j; k < _N; ++k) {
        if (A[k][j] > A[row][j]) {
          row = k;
        }
      }
      if (j != row) {
        id.swap_row(j, row);
        ++n;
      }
    }
    return std::make_pair(id, n);
  }
}  // namespace

template <typename _Tp, uint64_t _N>
std::pair<std::array<matrix<_Tp, _N, _N>, 3>, uint64_t> LUPF(
    const matrix<_Tp, _N, _N>& A) {
  matrix<_Tp, _N, _N> L;
  matrix<_Tp, _N, _N> U;
  std::pair<matrix<_Tp, _N, _N>, uint64_t> P = pivotize(A);
  matrix<_Tp, _N, _N> A2 = P.first * A;
  for (uint64_t j = 0; j < _N; ++j) {
    L[j][j] = _Tp(1);
    for (uint64_t i = 0; i < j + 1; ++i) {
      _Tp s1 = _Tp();
      for (uint64_t k = 0; k < i; ++k) {
        s1 += (U[k][j] * L[i][k]);
      }
      U[i][j] = A2[i][j] - s1;
    }
    for (uint64_t i = j; i < _N; ++i) {
      _Tp s2 = _Tp();
      for (uint64_t k = 0; k < j; ++k) {
        s2 += (U[k][j] * L[i][k]);
      }
      L[i][j] = (A2[i][j] - s2) / U[j][j];
    }
  }
  return std::make_pair(std::array<matrix<_Tp, _N, _N>, 3>{L, U, P.first},
                        P.second);
}
template <typename _Tp, uint64_t _N>
std::array<matrix<_Tp, _N, _N>, 3> LUP(const matrix<_Tp, _N, _N>& A) {
  matrix<_Tp, _N, _N> L;
  matrix<_Tp, _N, _N> U;
  matrix<_Tp, _N, _N> P = pivotize(A).first;
  matrix<_Tp, _N, _N> A2 = P * A;
  for (uint64_t j = 0; j < _N; ++j) {
    L[j][j] = _Tp(1);
    for (uint64_t i = 0; i < j + 1; ++i) {
      _Tp s1 = _Tp();
      for (uint64_t k = 0; k < i; ++k) {
        s1 += (U[k][j] * L[i][k]);
      }
      U[i][j] = A2[i][j] - s1;
    }
    for (uint64_t i = j; i < _N; ++i) {
      _Tp s2 = _Tp();
      for (uint64_t k = 0; k < j; ++k) {
        s2 += (U[k][j] * L[i][k]);
      }
      L[i][j] = (A2[i][j] - s2) / U[j][j];
    }
  }
  return std::array<matrix<_Tp, _N, _N>, 3>{L, U, P};
}
/* TODO: Add support for complex values! <11-12-18, Arden Rasmussen> */
template <typename _Tp, uint64_t _Nr, uint64_t _Nc>
matrix<_Tp, _Nr, _Nc> Cholesky(const matrix<_Tp, _Nr, _Nc>& A) {
  matrix<_Tp, _Nr, _Nc> L;
  for (uint64_t j = 0; j < _Nc; ++j) {
    _Tp sum = _Tp();
    for (uint64_t k = 0; k < j; ++k) {
      sum += L[j][k] * L[j][k];
    }
    L[j][j] = std::sqrt(A[j][j] - sum);
    for (uint64_t i = j; i < _Nr; ++i) {
      sum = _Tp();
      for (uint64_t k = 0; k < j; ++k) {
        sum += L[i][k] * L[j][k];
      }
      L[i][j] = (A[i][j] - sum) / L[j][j];
    }
  }
  return L;
}
template <typename _Tp, uint64_t _Nr, uint64_t _Nc>
std::array<matrix<_Tp, _Nr, _Nc>, 2> QR(const matrix<_Tp, _Nr, _Nc>& mat) {}

}  // namespace opero

#endif  // OPERO_DECOMPOSITION_HPP_
