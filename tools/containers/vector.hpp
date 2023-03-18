#include <initializer_list>
#include <memory>
#include <vector>
#include <iterator>

#pragma once

namespace tools::containers {


  template<class T, class Allocator = std::allocator<T> >
  class vector {
  public:

    typedef T value_type;
    typedef Allocator allocator_type;
    typedef ptrdiff_t size_type;
    typedef typename allocator_type::difference_type difference_type;
    typedef typename allocator_type::pointer pointer;
    typedef typename allocator_type::reference reference;
    typedef typename allocator_type::const_pointer const_pointer;
    typedef typename allocator_type::const_reference const_reference;
    typedef std::iterator<std::random_access_iterator_tag, value_type, difference_type, pointer, reference> iterator;
    typedef std::iterator<std::random_access_iterator_tag, value_type, difference_type, pointer, reference> const_iterator;
    typedef std::reverse_iterator<iterator> reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

    vector();

    explicit vector(const Allocator &);

    explicit vector(size_t n);

    explicit vector(size_t n, const Allocator &); // C++14
    vector(size_t n, const T &value, const Allocator & = Allocator());

    template<class InputIterator>
    vector(InputIterator first, InputIterator last, const Allocator & = Allocator());

    vector(const vector &x);

    vector(vector &&x);

    vector(std::initializer_list<T> il);

    vector(std::initializer_list<T> il, const Allocator &a);

    ~vector();

    vector &operator=(const vector &x);

    vector &operator=(vector &&x);

    vector &operator=(std::initializer_list<T> il);

    template<class InputIterator>
    void assign(InputIterator first, InputIterator last);

    void assign(size_t n, const T &u);

    void assign(std::initializer_list<T> il);

    Allocator get_allocator() const noexcept;

    std::iterator<std::random_access_iterator_tag, T> begin() noexcept;

    const std::iterator<std::random_access_iterator_tag, T> begin() const noexcept;

    std::iterator<std::random_access_iterator_tag, T> end() noexcept;

    const std::iterator<std::random_access_iterator_tag, T> end() const noexcept;

    reverse_iterator rbegin() noexcept;

    const_reverse_iterator rbegin() const noexcept;

    reverse_iterator rend() noexcept;

    const_reverse_iterator rend() const noexcept;

    const_iterator cbegin() const noexcept;

    const_iterator cend() const noexcept;

    const_reverse_iterator crbegin() const noexcept;

    const_reverse_iterator crend() const noexcept;

    size_t size() const noexcept;

    size_t max_size() const noexcept;

    size_t capacity() const noexcept;

    bool empty() const noexcept;

    void reserve(size_t n);

    void shrink_to_fit() noexcept;

    T &operator[](size_t n);

    const T &operator[](size_t n) const;

    T &at(size_t n);

    const T &at(size_t n) const;

    T &front();

    const T &front() const;

    T &back();

    const T &back() const;

    T *data() noexcept;

    const T *data() const noexcept;

    void push_back(const value_type &x);

    void push_back(value_type &&x);

    template<class... Args>
    reference emplace_back(Args &&... args); // reference in C++17
    void pop_back();

    template<class... Args>
    iterator emplace(const_iterator position, Args &&... args);

    iterator insert(const_iterator position, const value_type &x);

    iterator insert(const_iterator position, value_type &&x);

    iterator insert(const_iterator position, size_type n, const value_type &x);

    template<class InputIterator>
    iterator insert(const_iterator position, InputIterator first, InputIterator last);

    iterator insert(const_iterator position, std::initializer_list <value_type> il);

    iterator erase(const_iterator position);

    iterator erase(const_iterator first, const_iterator last);

    void clear() noexcept;

    void resize(size_type sz);

    void resize(size_type sz, const value_type &c);

    void swap(vector &)
    noexcept(std::allocator_traits<allocator_type>::propagate_on_container_swap::value ||
             std::allocator_traits<allocator_type>::is_always_equal::value);  // C++17

    bool __invariants() const;
  };

  template<class InputIterator, class Allocator = std::allocator<typename std::iterator_traits<InputIterator>::value_type>>
  vector(InputIterator, InputIterator, Allocator = Allocator())
  -> vector<typename std::iterator_traits<InputIterator>::value_type, Allocator>;


  template<class T, class Allocator>
  bool operator==(const vector<T, Allocator> &x, const vector<T, Allocator> &y);

  template<class T, class Allocator>
  bool operator<(const vector<T, Allocator> &x, const vector<T, Allocator> &y);

  template<class T, class Allocator>
  bool operator!=(const vector<T, Allocator> &x, const vector<T, Allocator> &y);

  template<class T, class Allocator>
  bool operator>(const vector<T, Allocator> &x, const vector<T, Allocator> &y);

  template<class T, class Allocator>
  bool operator>=(const vector<T, Allocator> &x, const vector<T, Allocator> &y);

  template<class T, class Allocator>
  bool operator<=(const vector<T, Allocator> &x, const vector<T, Allocator> &y);

  template<class T, class Allocator>
  void swap(vector<T, Allocator> &x, vector<T, Allocator> &y)
  noexcept(noexcept(x.swap(y)));

  template<class T, class Allocator, class U>
  typename vector<T, Allocator>::size_type
  erase(vector<T, Allocator> &c, const U &value);       // C++20
  template<class T, class Allocator, class Predicate>
  typename vector<T, Allocator>::size_type
  erase_if(vector<T, Allocator> &c, Predicate pred);    // C++20

} // tools::containers


