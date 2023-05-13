#include <algorithm>
#include <cctype>
#include <exception>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <memory>

namespace {
constexpr const size_t kMaxStringSize = 1'048'576;
constexpr const size_t kArrayResizeK = 2;
} // namespace

namespace tools::containers {

class String {
public:
  String() {
    data_ = new char[4];
    capacity_ = 4;
    size_ = 0;
  }

  String(const String &string) {
    data_ = new char[string.capacity_];
    capacity_ = string.capacity_;
    size_ = string.size_;
    for (size_t i(0); i < size_; ++i) {
      data_[i] = string.data_[i];
    }
  }

  explicit String(const char string[]) {
    data_ = new char[strlen(string)];
    capacity_ = strlen(string);
    size_ = strlen(string);
    for (size_t i(0); i < size_; ++i) {
      data_[i] = string[i];
    }
  }

  String(String &&sr) noexcept {
    this->data_ = sr.data_;
    sr.data_ = nullptr;
    this->size_ = sr.size_;
    this->capacity_ = sr.capacity_;
  }

  [[nodiscard]] size_t Size() const { return size_; }

  ~String() {
    delete[] data_;
    data_ = nullptr;
  }

  char &operator[](size_t index) {
    if (index >= size_)
      throw std::runtime_error("Bad index");
    return data_[index];
  }

  const char &operator[](size_t index) const {
    if (index >= size_)
      throw std::runtime_error("Bad index");
    return data_[index];
  }

  void Insert(char v) {
    if (size_ + 1 > capacity_) {
      Reallocate();
    }
    data_[size_++] = v;
  }

  String &operator=(const String &sr) {
    if (this == &sr)
      return *this;
    this->data_ = new char[sr.capacity_];
    this->capacity_ = sr.capacity_;
    this->size_ = 0;
    for (; this->size_ < sr.size_; ++this->size_) {
      this->data_[this->size_] = sr.data_[this->size_];
    }
    return *this;
  }

  String &operator=(String &&sr) noexcept {
    data_ = sr.data_;
    sr.data_ = nullptr;
    capacity_ = sr.capacity_;
    sr.capacity_ = 0;
    size_ = sr.size_;
    sr.size_ = 0;
    return *this;
  }

  friend std::istream &operator>>(std::istream &is, String &s);

private:
  char *data_ = nullptr;
  size_t size_;
  size_t capacity_;

  void Reallocate() {
    capacity_ *= 2;
    char *n_data = new char[capacity_];
    for (size_t i(0); i < size_; ++i) {
      n_data[i] = data_[i];
    }
    delete[] data_;
    data_ = n_data;
  }
};

std::ostream &operator<<(std::ostream &os, const String &s) {
  for (size_t i(0); i < s.Size(); ++i) {
    os << s[i];
  }
  return os;
}

std::istream &operator>>(std::istream &is, String &s) {
  delete[] s.data_;
  s.capacity_ = 4;
  s.size_ = 0;
  s.data_ = new char[s.capacity_];
  is.setstate(std::istream::goodbit);
  std::basic_istream<char>::sentry sentry(is);
  if (sentry) {
    while (s.size_ < kMaxStringSize) {
      int c = is.rdbuf()->sgetc();
      if (c == EOF) {
        is.setstate(std::istream::eofbit);
        break;
      }
      if (isspace(c)) {
        break;
      }
      s.Insert(static_cast<char>(c));
      is.rdbuf()->sbumpc();
    }
    if (s.size_ == 0) {
      is.setstate(std::istream::failbit);
    }
  }
  return is;
}

bool operator==(const String &sl, const String &sr) {
  if (sl.Size() != sr.Size())
    return false;
  for (size_t i(0); i < sl.Size(); ++i) {
    if (sl[i] != sr[i])
      return false;
  }
  return true;
}

} // namespace tools::containers

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
    res[--count[GetVal(v[i])]] = v[i];
  }

  v = std::move(res);
}

} // namespace tools::containers::vector_tools

namespace tc = tools::containers;
namespace vt = tools::containers::vector_tools;

template <typename Tf, typename Ts> struct Pair {
  Tf first{};
  Ts second{};
};

template <typename Tf, typename Ts>
std::istream &operator>>(std::istream &is, Pair<Tf, Ts> &p) {
  is >> p.first >> p.second;
  return is;
}

using TV = Pair<size_t, tc::String>;

int main() {
  tc::Vector<TV> v;
  v.Reserve(65536);
  TV tmp;
  while (std::cin >> tmp) {
    v.PushBack(tmp);
  }
  vt::LinearStableSort<TV>(v, [](const TV &p) { return p.first; });
  for (size_t i(0); i < v.Size(); ++i) {
    std::cout << std::setw(6) << std::setfill('0') << v[i].first << ' '
              << v[i].second << '\n';
  }
  return 0;
}