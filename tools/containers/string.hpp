#pragma once

#include <cassert>
#include <cctype>
#include <iostream>
#include <string>
#include <strstream>
namespace {
constexpr const size_t kMaxStringSize = 1'048'576;
}
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

  void Erase(size_t i, size_t cnt) {
    if (i + cnt > size_)
      throw std::runtime_error("Bad index");
    for (size_t i_v(i + cnt); i_v < size_; ++i_v) {
      data_[i_v - cnt] = data_[i_v];
    }
    size_ -= cnt;
  }

  String &operator=(const String &sr) {
    if (this == &sr)
      return *this;
    delete[] data_;
    this->data_ = new char[sr.capacity_];
    this->capacity_ = sr.capacity_;
    this->size_ = 0;
    for (; this->size_ < sr.size_; ++this->size_) {
      this->data_[this->size_] = sr.data_[this->size_];
    }
    return *this;
  }

  String &operator=(String &&sr) noexcept {
    delete[] data_;
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

#ifdef DEBUG

namespace string_test {
namespace {

void AssertEq(const String &str, const std::string &expected) {
  assert(str.Size() == expected.size());
  for (int i(0); i < expected.size(); ++i) {
    assert(str[i] == expected[i]);
  }
}
} // namespace

void Test() {
  {
    String s1;
    { String s2 = s1; }
    { String s3(s1); }
  }

  String s1;
  s1.Insert('a');

  std::string expected_result_1 = "a";
  assert(s1[0] == expected_result_1[0]);
  s1.Insert('b');
  s1.Insert('c');
  std::string expected_result_2 = "abc";
  AssertEq(s1, expected_result_2);

  s1.Insert('d');
  s1.Insert('e');
  s1.Insert('f');
  std::string expected_result_3 = "abcdef";
  AssertEq(s1, expected_result_3);

  s1.Erase(1, 3);
  std::string expected_result_4 = "aef";
  AssertEq(s1, expected_result_4);

  s1.Erase(1, 2);
  std::string expected_result_5 = "a";
  AssertEq(s1, expected_result_5);
}

} // namespace string_test
#endif

} // namespace tools::containers