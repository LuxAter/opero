#ifndef OPERO_HPP_
#define OPERO_HPP_

#include <algorithm>
#include <array>
#include <complex>
#include <iostream>
#include <map>
#include <vector>

#define PI 3.14159265358979323846264338327950288
#define E 2.71828182845904523536028747135266249

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

inline std::string asString(real val) {
  char buff[256];
  std::sprintf(buff, "%lf", val);
  return std::string(buff);
}
inline std::string asString(complex val) {
  char buff[256];
  std::sprintf(buff, "%lf%+lfi", val.real(), val.imag());
  return std::string(buff);
}
template <typename _T>
inline std::string asString(pair<_T> val) {
  char buff[256];
  std::sprintf(buff, "(%s,%s)", asString(val.x).c_str(),
               asString(val.y).c_str());
  return std::string(buff);
}
template <typename _T>
inline std::string asString(triple<_T> val) {
  char buff[256];
  std::sprintf(buff, "(%lf,%lf,%lf)", asString(val.x).c_str(),
               asString(val.y).c_str(), asString(val.z).c_str());
  return std::string(buff);
}
inline real readReal(const std::string& src) { return std::stod(src); }
inline complex readComplex(const std::string& src) {
  real re(0), im(0);
  if (src.find("i") != std::string::npos) {
    if (src.find_last_of("+-") != 0 &&
        src.find_last_of("+-") != std::string::npos) {
      std::sscanf(src.c_str(), "%lf%lfi", &re, &im);
    } else {
      std::sscanf(src.c_str(), "%lfi", &im);
    }
  } else {
    std::sscanf(src.c_str(), "%lf", &re);
  }
  return complex(re, im);
  // std::stringstream ss(src);
  // real re, im;
  // char sign;
  // ss >> re >> sign >> im;
  // return complex(re, (sign == '-') ? -im : im);
}
inline pair<real> readRealPair(const std::string& src) {
  real x, y;
  std::sscanf(src.c_str(), "(%lf,%lf)", &x, &y);
  return pair<real>{x, y};
}
inline pair<complex> readComplexPair(const std::string& src) {
  char x[256], y[256];
  std::sscanf(src.c_str(), "(%255s,%255s)", x, y);
  return pair<complex>{readComplex(x), readComplex(y)};
}
inline triple<real> readRealTriple(const std::string& src) {
  real x, y, z;
  std::sscanf(src.c_str(), "(%lf,%lf,%lf)", &x, &y, &z);
  return triple<real>{x, y, z};
}
inline triple<complex> readComplexTriple(const std::string& src) {
  char x[256], y[256], z[256];
  std::sscanf(src.c_str(), "(%255s,%255s,%255s)", x, y, z);
  return triple<complex>{readComplex(x), readComplex(y), readComplex(z)};
}

template <typename _In, typename _Out>
class function {
 public:
  explicit function(const std::string& func) : function_(func) {
    ParseFunction();
  }
  _Out operator()(_In input) { return EvalFunction(input); }

 private:
  struct EvalObj {
    enum State {
      REAL,
      COMPLEX,
      REAL_PAIR,
      COMPLEX_PAIR,
      REAL_TRIPLE,
      COMPLEX_TRIPLE,
      FUNCTION,
      VARIABLE
    };
    State state_;
    real real_;
    complex complex_;
    pair<real> p_real_;
    pair<complex> p_complex_;
    triple<real> t_real_;
    triple<complex> t_complex_;
    std::string op;

    void read(const std::string& src){

    }
  };

  std::string token_;
  std::string function_;
  std::vector<std::string> operation_;

  _Out EvalFunction(_In input) {
    std::map<std::string, uint8_t> functions_{
        {"^", 2},    {"!", 1},   {"*", 2},    {"/", 2},    {"+", 2},
        {"-", 2},    {"sin", 1}, {"cos", 1},  {"tan", 1},  {"exp", 1},
        {"abs", 1},  {"log", 1}, {"logb", 2}, {"asin", 1}, {"acos", 1},
        {"atan", 1}, {"max", 2}, {"min", 2}};
    std::vector<EvalObj> eval;
    for (auto& it : operation_) {
      eval.push_back({it, readReal(it)});
    }
    for (std::size_t i = 0; i < eval.size(); ++i) {
      if (functions_.find(eval[i].op) != functions_.end()) {
        if (eval[i].op == "^") {
          eval[i - 2].real_ = std::pow(eval[i - 2].real_, eval[i - 1].real_);
          std::cout << "?>>" << eval[i - 2].real_ << "<<\n!";
        } else if (eval[i].op == "!") {
        } else if (eval[i].op == "*") {
        } else if (eval[i].op == "/") {
        } else if (eval[i].op == "+") {
        } else if (eval[i].op == "-") {
        }
        // TODO APPLY FUNCTION
      }
    }
  }

  void ParseFunction() {
    std::size_t i = 0;
    std::map<std::string, std::pair<uint8_t, bool>> operators_{
        {"^", {4, true}},  {"!", {4, true}},  {"*", {3, false}},
        {"/", {3, false}}, {"+", {2, false}}, {"-", {2, false}}};
    std::vector<std::string> functions_{"sin",  "cos",  "tan",  "exp",
                                        "abs",  "log",  "logb", "asin",
                                        "acos", "atan", "max",  "min"};
    std::vector<std::string> output_;
    std::vector<std::string> stack_;
    while (i < function_.size()) {
      ReadToken(i);
      if ((token_[0] >= 48 && token_[0] <= 57) || token_[0] == 46) {
        output_.push_back(token_);
      } else if (std::find(functions_.begin(), functions_.end(), token_) !=
                 functions_.end()) {
        stack_.push_back(token_);
      } else if (token_ == "(") {
        stack_.push_back(token_);
      } else if (token_ == ")") {
        while (stack_.size() != 0 && stack_.back() != "(") {
          output_.push_back(stack_.back());
          stack_.pop_back();
        }
        stack_.pop_back();
      } else if (operators_.find(token_) == operators_.end()) {
        output_.push_back(token_);
      } else {
        while (
            stack_.size() != 0 &&
            (std::find(functions_.begin(), functions_.end(), stack_.back()) !=
                 functions_.end() ||
             ((operators_[stack_.back()].first > operators_[token_].first ||
               ((!operators_[stack_.back()].second || token_ == "!" ||
                 stack_.back() == "!") &&
                operators_[stack_.back()].first >= operators_[token_].first)) &&
              stack_.back() != "("))) {
          output_.push_back(stack_.back());
          stack_.pop_back();
        }
        stack_.push_back(token_);
      }
    }
    while (stack_.size() != 0) {
      output_.push_back(stack_.back());
      stack_.pop_back();
    }
    operation_ = output_;
  }

  void ReadToken(std::size_t& i) {
    std::string current_token;
    uint8_t state = 0;
    while (i < function_.size() &&
           (function_[i] == ' ' || function_[i] == ',')) {
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
