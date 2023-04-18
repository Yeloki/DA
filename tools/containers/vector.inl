#include "vector.hpp"

namespace tools::containers {

  template<typename T>
  Vector<T>::Vector() {
    size_ = 0;
    capacity_ = 4;
    data_ = reinterpret_cast<T *>(malloc(sizeof(T) * capacity_));
  }

  template<typename T>
  Vector<T>::Vector(size_t size) {
    size_ = size;

    capacity_ = [&size]() {
      size_t res = 2;
      while (res < size) {
        res *= 2;
      }
      return res;
    }();

    data_ = malloc(sizeof(T) * capacity_);
  }

  template<typename T>
  Vector<T>::Vector(size_t size, const T &default_value) {
    size_ = size;

    capacity_ = [&size]() {
      size_t res = 2;
      while (res < size) {
        res *= 2;
      }
      return res;
    }();

    data_ = malloc(sizeof(T) * capacity_);

    for (size_t i(0); i < size_; ++i) {
      data_[i] = default_value;
    }
  }


  template<typename T>
  void Vector<T>::PushBack(const T &elem) {
    if (size_ + 1 > capacity_) {
      capacity_ *= 2;
      data_ = reinterpret_cast<T *>(realloc(data_, sizeof(T) * capacity_));
    }
    data_[size_++] = elem;
  }

  template<typename T>
  void Vector<T>::Reserve(size_t capacity) {

    capacity_ = [&capacity]() {
      size_t res = 2;
      while (res < capacity) {
        res *= 2;
      }
      return res;
    }();

    data_ = realloc(data_, sizeof(T) * capacity_);
  }

  template<typename T>
  void Vector<T>::Resize(size_t size) {
    size_ = size;

    capacity_ = [&size]() {
      size_t res = 2;
      while (res < size) {
        res *= 2;
      }
      return res;
    }();

    data_ = realloc(data_, sizeof(T) * capacity_);
  }

  template<typename T>
  void Vector<T>::Resize(size_t size, const T &default_value) {
    capacity_ = [&size]() {
      size_t res = 2;
      while (res < size) {
        res *= 2;
      }
      return res;
    }();

    data_ = reinterpret_cast<T *>(realloc(data_, sizeof(T) * capacity_));

    for (size_t i(size_); i < size; ++i) {
      data_[i] = default_value;
    }
    size_ = size;
  }

  template<typename T>
  Vector<T>::~Vector() {
    free(data_);
  }

  template<typename T>
  T &Vector<T>::operator[](size_t n) {
    if (n >= size_) {
      throw std::range_error("");
    }
    return data_[n];
  }

  template<typename T>
  const T &Vector<T>::operator[](size_t n) const {
    if (n >= size_) {
      throw std::range_error("");
    }
    return data_[n];
  }

  template<typename T>
  T &Vector<T>::at(size_t n) {
    if (n >= size_) {
      throw std::range_error("");
    }
    return data_[n];
  }

  template<typename T>
  const T &Vector<T>::at(size_t n) const {
    if (n >= size_) {
      throw std::range_error("");
    }
    return data_[n];
  }

  template<typename T>
  Vector<T> &Vector<T>::operator=(const Vector &x) {
    if (this == *x)
      return *this;

    Reserve(x.capacity_);
    size_ = x.size_;
    for (size_t i(0); i < size_; ++i) {
      data_[i] = x.data_[i];
    }

    return *this;
  }

  template<typename T>
  Vector<T> &Vector<T>::operator=(Vector &&x) noexcept {
    if (this == *x)
      return *this;
    free(data_);
    data_ = x.data_;
    capacity_ = x.capacity_;
    size_ = x.size_;
    return *this;
  }

  template<typename T>
  size_t Vector<T>::size() const noexcept {
    return size_;
  }


} // tools::containers