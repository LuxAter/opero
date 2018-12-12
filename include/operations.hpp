#ifndef OPERO_OPERATIONS_HPP_
#define OPERO_OPERATIONS_HPP_

#include "matrix.hpp"

#include "decomposition.hpp"

namespace opero {
template <typename _Tp, uint64_t _Nr, uint64_t _Nc>
_Tp trace(const matrix<_Tp, _Nr, _Nc>& lhs) {
  _Tp sum = _Tp();
  for (uint64_t i = 0; i < _Nr && i < _Nc; ++i) {
    sum += lhs[i][i];
  }
  return sum;
}

template <typename _Tp, uint64_t _N>
_Tp det(const matrix<_Tp, _N, _N>& A) {
  std::pair<std::array<matrix<_Tp, _N, _N>, 3>, uint64_t> lupf = LUPF(A);
  _Tp D = _Tp(1);
  for (uint64_t k = 0; k < _N; ++k) {
    D *= (lupf.first[0][k][k] * lupf.first[1][k][k]);
  }
  return std::pow(_Tp(-1), lupf.second) * D;
}

template <typename _Tp, uint64_t _N>
_Tp inverse(const matrix<_Tp, _N, _N>& A) {
  std::array<matrix<_Tp, _N, _N>, 3> lup = LUP(A);
}
}  // namespace opero

#endif  // OPERO_OPERATIONS_HPP_
