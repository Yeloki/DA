#include <algorithm>
#include <cctype>
#include <cstring>
#include <exception>
#include <functional>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <memory>
#include <string>

#ifdef LOCAL
#define _GLIBCXX_DEBUG
#else
#pragma GCC optimize(                                                          \
        "O3,unroll-loops,inline-small-functions,inline-functions-called-once")
#pragma GCC target("avx,avx2")
#endif

namespace {
constexpr const float kArrayResizeK = 1.5;
constexpr const size_t kArrayInitSize = 2;
} // namespace

namespace tools::containers {

template <typename T> class Vector {
public:
  Vector() {
    size_ = 0;
    capacity_ = 4;
    data_ = new T[capacity_];
  }

  explicit Vector(size_t size) {
    size_ = size;

    capacity_ = size;

    data_ = new T[capacity_];
  }

  Vector(size_t size, const T &default_value) {
    size_ = size;

    capacity_ = size;

    data_ = new T[capacity_];

    for (size_t i(0); i < size_; ++i) {
      data_[i] = default_value;
    }
  }

  Vector(Vector &&v) noexcept {
    capacity_ = v.capacity;
    size_ = v.Size();
    delete[] data_;
    data_ = v.data_;
    v.data_ = nullptr;
    v.capacity_ = 0;
    v.size_ = 0;
  }

  void PushBack(const T &elem) {
    if (size_ + 1 > capacity_) {
      capacity_ *= kArrayResizeK;
      Reallocate();
    }
    data_[size_++] = elem;
  }

  void Reserve(size_t capacity) {

    capacity_ = [&capacity]() {
      size_t res = 4;
      while (res < capacity) {
        res *= kArrayResizeK;
      }
      return res;
    }();
    Reallocate();
  }

  ~Vector() {
    delete[] data_;
    data_ = nullptr;
  }

  T &operator[](size_t n) {
    if (n >= size_) {
      throw std::range_error("");
    }
    return data_[n];
  }

  const T &operator[](size_t n) const {
    if (n >= size_) {
      throw std::range_error("");
    }
    return data_[n];
  }

  Vector &operator=(const Vector &x) {
    if (this == *x)
      return *this;

    Reserve(x.capacity_);
    size_ = x.size_;
    for (size_t i(0); i < size_; ++i) {
      data_[i] = x.data_[i];
    }

    return *this;
  }

  Vector &operator=(Vector &&x) noexcept {
    delete[] data_;
    data_ = x.data_;
    x.data_ = nullptr;
    capacity_ = x.capacity_;
    x.capacity_ = 0;
    size_ = x.size_;
    x.size_ = 0;
    return *this;
  }

  [[nodiscard]] size_t Size() const noexcept { return size_; }

private:
  void Reallocate() {
    auto new_data = new T[capacity_];
    for (size_t i(0); i < size_; ++i) {
      new_data[i] = std::move(data_[i]);
    }
    delete[] data_;
    data_ = new_data;
  }

  T *data_ = nullptr;
  size_t size_;
  size_t capacity_;
};

} // namespace tools::containers

namespace tools::containers::vector_tools {

template <typename T>
void LinearStableSort(Vector<T> &v, std::function<size_t(T)> GetVal) {
  if (v.Size() < 2) {
    return;
  }

  size_t max_val = 0;
  for (size_t i(0); i < v.Size(); ++i) {
    max_val = std::max(max_val, GetVal(v[i]));
  }

  Vector<int> count(max_val + 1, 0);
  for (size_t i(0); i < v.Size(); ++i) {
    ++count[GetVal(v[i])];
  }

  for (size_t i(1); i < count.Size(); ++i) {
    count[i] += count[i - 1];
  }

  Vector<T> res(v.Size());

  for (int64_t i = v.Size() - 1; i >= 0; --i) {
    res[--count[GetVal(v[i])]] = std::move(v[i]);
  }

  v = std::move(res);
}

} // namespace tools::containers::vector_tools

namespace tc = tools::containers;
namespace vt = tools::containers::vector_tools;

template <typename Tf, typename Ts>
std::istream &operator>>(std::istream &is, std::pair<Tf, Ts> &p) {
  is >> p.first >> p.second;
  return is;
}

using TV = std::pair<size_t, std::string>;

int main() {

#define QUICK_IO

#ifdef QUICK_IO
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
#endif

  tc::Vector<TV> v;
  v.Reserve(kArrayInitSize);
  TV tmp;
  while (std::cin >> tmp) {
    v.PushBack(tmp);
  }
  vt::LinearStableSort<TV>(v, [](const TV &p) { return p.first; });
  for (size_t i(0); i < v.Size(); ++i) {
    std::cout << std::setw(6) << std::setfill('0') << v[i].first << '\t'
              << v[i].second << '\n';
  }
  return 0;
}