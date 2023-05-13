#pragma once
#include <exception>
#include <initializer_list>
#include <iterator>
#include <memory>
#include <vector>

namespace {
constexpr const size_t kArrayResizeK = 2;
}

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

    capacity_ = [&size]() {
      size_t res = 2;
      while (res < size) {
        res *= 2;
      }
      return res;
    }();

    data_ = new T[capacity_];
  }

  Vector(size_t size, const T &default_value) {
    size_ = size;

    capacity_ = [&size]() {
      size_t res = 2;
      while (res < size) {
        res *= 2;
      }
      return res;
    }();

    data_ = new T[capacity_];

    for (size_t i(0); i < size_; ++i) {
      data_[i] = default_value;
    }
  }

  Vector(std::initializer_list<const T> list) {
    size_ = list.size();

    capacity_ = [&list]() {
      size_t res = 2;
      while (res < list.size()) {
        res *= 2;
      }
      return res;
    }();

    data_ = new T[capacity_];
    size_t j(0);
    for (auto el = list.begin(); el != list.end(); ++el) {
      data_[j++] = std::move(*el);
    }
  }

  Vector(Vector &&v) {
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
      Reallocate();
    }
    data_[size_++] = elem;
  }

  void Reserve(size_t capacity) {

    capacity_ = [&capacity]() {
      size_t res = 2;
      while (res < capacity) {
        res *= 2;
      }
      return res;
    }();

    Reallocate();
  }

  void Resize(size_t size) {
    size_ = size;

    capacity_ = [&size]() {
      size_t res = 2;
      while (res < size) {
        res *= 2;
      }
      return res;
    }();

    Reallocate();
  }

  void Resize(size_t size, const T &default_value) {
    capacity_ = [&size]() {
      size_t res = 2;
      while (res < size) {
        res *= 2;
      }
      return res;
    }();

    Reallocate();

    for (size_t i(size_); i < size; ++i) {
      data_[i] = default_value;
    }
    size_ = size;
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

  T &At(size_t n) {
    if (n >= size_) {
      throw std::range_error("");
    }
    return data_[n];
  }

  [[nodiscard]] const T &At(size_t n) const {
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
  [[nodiscard]] size_t Capacity() const noexcept { return capacity_; };

private:
  void Reallocate() {
    capacity_ *= kArrayResizeK;
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

template <typename Tf, typename Ts>
std::ostream &operator<<(std::ostream &os, const std::pair<Tf, Ts> &p) {
  os << "{ " << p.first << ", " << p.second << " }";
  return os;
}

template <typename T>
std::ostream &operator<<(std::ostream &os, const Vector<T> &v) {
  os << "{ ";
  for (size_t i(0); i < v.Size(); ++i) {
    os << v[i] << (i + 1 == v.Size() ? "" : ", ");
  }
  os << " }";
  return os;
}

#ifdef DEBUG

class TestError : public std::runtime_error {
  using std::runtime_error::runtime_error;
};

template <typename T>
void AssertEqual(const Vector<T> &v, const std::vector<T> &expected,
                 const std::string &line) {

  if (v.Size() != expected.size())
    throw TestError("Wrong size of v vector" + std::string(__FILE__) + " : " +
                    std::to_string(__LINE__));

  for (size_t i(0); i < v.Size(); ++i) {
    if (v[i] != expected[i])
      throw TestError("Wrong element on position [" + std::to_string(i) +
                      "] : " + std::string(__FILE__) + " : " + line);
  }
}

#include "string.hpp"

namespace vector_test {

namespace {

constexpr const char *kRunning = "[RUNNING]";
constexpr const char *kOk = "[OK]";
constexpr const char *kFailed = "[FAILED]";
constexpr const char *kReason = "Reason: ";

} // namespace

void Test() {
  { /////////////////////////////////////////////////////////////////
    constexpr const char *kTestName = "test constructors for default types";
    std::cout << kRunning << ' ' << kTestName << std::endl;
    bool is_success(false);
    try {
      {
        const std::vector<int> expected = {1, 2, 2, 9, 2, 0, -4, 8};
        const Vector<int> v = {1, 2, 2, 9, 2, 0, -4, 8};
        AssertEqual(v, expected, std::to_string(__LINE__));
      }
      {
        const std::vector<int> expected(8, 0);
        const Vector<int> v(8, 0);
        AssertEqual(v, expected, std::to_string(__LINE__));
      }
      {
        const std::vector<int> expected(8);
        const Vector<int> v(8);
        if (v.Size() != expected.size())
          throw TestError(
              "Wrong size of v, expected: " + std::to_string(v.Size()) +
              ", got: " + std::to_string(expected.size()));
      }

      is_success = true;
    } catch (const TestError &te) {
      std::cout << kFailed << ' ' << kTestName << std::endl;
      std::cout << kReason << ' ' << te.what() << std::endl;
    }
    if (is_success)
      std::cout << kOk << ' ' << kTestName << std::endl;
  } /////////////////////////////////////////////////////////////////

  { /////////////////////////////////////////////////////////////////
    constexpr const char *kTestName = "test constructors for custom types";
    std::cout << kRunning << ' ' << kTestName << std::endl;
    bool is_success(false);
    try {
      {
        const std::vector<String> expected = {String("123"), String("456"),
                                              String("789"), String("012")};
        const Vector<String> v = {String("123"), String("456"), String("789"),
                                  String("012")};
        AssertEqual(v, expected, std::to_string(__LINE__));
      }
      is_success = true;
    } catch (const TestError &te) {
      std::cout << kFailed << ' ' << kTestName << std::endl;
      std::cout << kReason << ' ' << te.what() << std::endl;
    }
    if (is_success)
      std::cout << kOk << ' ' << kTestName << std::endl;
  } /////////////////////////////////////////////////////////////////

  { /////////////////////////////////////////////////////////////////
    constexpr const char *kTestName = "test capacity change";
    std::cout << kRunning << ' ' << kTestName << std::endl;
    bool is_success(false);
    try {
      {
        const std::vector expected = {1, 2, 3, 4};
        const Vector v = {1, 2, 3, 4};
        AssertEqual(v, expected, std::to_string(__LINE__));
        assert(v.Capacity() == 4);
      }
      {
        const std::vector expected = {1, 2, 3, 4, 5};
        Vector v = {1, 2, 3, 4};
        v.PushBack(5);
        AssertEqual(v, expected, std::to_string(__LINE__));
        assert(v.Capacity() == 8);
      }
      is_success = true;
    } catch (const TestError &te) {
      std::cout << kFailed << ' ' << kTestName << std::endl;
      std::cout << kReason << ' ' << te.what() << std::endl;
    }
    if (is_success)
      std::cout << kOk << ' ' << kTestName << std::endl;
  } /////////////////////////////////////////////////////////////////

  { /////////////////////////////////////////////////////////////////
    constexpr const char *kTestName = "test vector resize";
    std::cout << kRunning << ' ' << kTestName << std::endl;
    bool is_success(false);
    try {
      {
        Vector v = {1, 2, 3, 4};
        v.Resize(23);
        assert(v.Size() == 23);
      }
      {
        const std::vector expected = {1, 2, 3, 4, 0, 0, 0, 0};
        Vector v = {1, 2, 3, 4};
        v.Resize(8, 0);
        AssertEqual(v, expected, std::to_string(__LINE__));
      }
      is_success = true;
    } catch (const TestError &te) {
      std::cout << kFailed << ' ' << kTestName << std::endl;
      std::cout << kReason << ' ' << te.what() << std::endl;
    }
    if (is_success)
      std::cout << kOk << ' ' << kTestName << std::endl;
  } /////////////////////////////////////////////////////////////////
}

} // namespace vector_test

#endif
} // namespace tools::containers
