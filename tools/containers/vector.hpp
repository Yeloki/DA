#include <initializer_list>

#pragma once

namespace tools::containers {

  template<typename T_v>
  class vector {
  public:

    // constructors
    vector();

    vector(size_t size);

    vector(size_t size, const T_v& default_value);

    vector(const std::initializer_list <T_v> &initializer_list);

    // methods
    void push_back(const T_v& val);

    void resize(size_t size) {

    }
  private:
    T_v* data_;
    size_t size_;
    size_t capacity_;
  };



} // tools::containers


