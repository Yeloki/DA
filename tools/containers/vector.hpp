#pragma once

#include <initializer_list>
#include <memory>
#include <vector>
#include <iterator>


//namespace tools::containers {


  template<typename T>
  class Vector {
  public:
    Vector();

    explicit Vector(size_t size);

    Vector(size_t size, const T &default_value);

    void PushBack(const T &elem);

    void Reserve(size_t capacity);

    void Resize(size_t size);

    void Resize(size_t size, const T &default_value);

    ~Vector();

    T &operator[](size_t n);

    const T &operator[](size_t n) const;

    T &at(size_t n);

    const T &at(size_t n) const;

    Vector &operator=(const Vector &x);

    Vector &operator=(Vector &&x) noexcept;

    size_t size() const noexcept;

  private:
    T *data_;
    size_t size_;
    size_t capacity_;
  };

#include "vector.inl"
//} // tools::containers


