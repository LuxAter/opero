#ifndef OPERO_HPP_
#define OPERO_HPP_

#include <array>
#include <complex>
#include <iostream>
#include <map>
#include <vector>

namespace opero {

typedef double real;
typedef typename std::complex<real> complex;

template <typename _T>
struct pair {
  _T x, y;
};
template <typename _T>
struct triple {
  _T x, y, z;
};

template <typename _In, typename _Out>
class function {
 public:
  explicit function(const std::string& func) : function_(func) {
    ParseFunction();
  }
  _Out operator()(_In input) { return input / 2; }

 private:
  std::string token_;
  std::string function_;

  void ParseFunction() {
    std::size_t i = 0;
    std::map<std::string, uint8_t> operators_{
        {"^", 4},   {"*", 3},    {"/", 3},    {"+", 2},   {"-", 2},
        {"sin", 5}, {"cos", 5},  {"tan", 5},  {"exp", 5}, {"abs", 5},
        {"log", 5}, {"asin", 5}, {"acos", 5}, {"atan", 5}};
    std::vector<std::string> output_;
    std::vector<std::string> stack_;
    while (i < function_.size()) {
      ReadToken(i);
      if ((token_[0] >= 48 && token_[0] <= 57) || token_[0] == 46) {
        output_.push_back(token_);
      } if(operators_.find(token_) == operators_.end()){
        output_.push_back(token_);
      }
    }
  }

  void ReadToken(std::size_t& i) {
    std::string current_token;
    uint8_t state = 0;
    while (i < function_.size() && function_[i] == ' ') {
      ++i;
    }
    if (function_[i] >= 48 && function_[i] <= 57) {
      state = 1;
    } else if (function_[i] == 46) {
      state = 2;
    } else if (function_[i] == '+') {
      current_token = "+";
    } else if (function_[i] == '-') {
      current_token = "-";
    } else if (function_[i] == '*') {
      current_token = "*";
    } else if (function_[i] == '/') {
      current_token = "/";
    } else if (function_[i] == '^') {
      current_token = "^";
    } else if (function_[i] == '%') {
      current_token = "%";
    } else if (function_[i] == '!') {
      current_token = "!";
    } else if (function_[i] == '(') {
      current_token = "(";
    } else if (function_[i] == ')') {
      current_token = ")";
    } else {
      state = 3;
    }
    if (state != 0) {
      while (i < function_.size()) {
        if (state == 1) {
          if (function_[i] >= 48 && function_[i] <= 57) {
            current_token += function_[i];
          } else if (function_[i] == 46) {
            current_token += function_[i];
            state = 2;
          } else {
            i--;
            break;
          }
        } else if (state == 2) {
          if (function_[i] >= 48 && function_[i] <= 57) {
            current_token += function_[i];
          } else {
            i--;
            break;
          }
        } else if (state == 3) {
          if (function_[i] != '+' && function_[i] != '-' &&
              function_[i] != '*' && function_[i] != '/' &&
              function_[i] != '%' && function_[i] != '!' &&
              function_[i] != '(' && function_[i] != ')' &&
              function_[i] != '^') {
            current_token += function_[i];
          } else {
            i--;
            break;
          }
        }
        i++;
      }
    }
    i++;
    token_ = current_token;
  }
};

}  // namespace opero

#endif  // OPERO_HPP_
