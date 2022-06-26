#pragma once

#include <concepts>
#include <type_traits>

namespace c3lt {
  /// Indicates a normal pointer
  template<typename T>
  class ptr {
  private:
    T* p = nullptr;

  public:
    T* get() const { return p; }

  public:
//    constexpr operator ptr<std::remove_const_t<T>>() const { return p; }
    template<typename U>
    constexpr operator ptr<U>() const { return ptr<U>{static_cast<U*>(this->get())}; }
    constexpr operator bool() const { return p; }
    constexpr operator T*() const { return p; }
    constexpr T& operator*() const { return *p; }
    constexpr T* operator->() const { return p; }

  public:
    inline ptr() = default;
    inline ptr(T* p_) : p{p_} {}
  };

  /// Indicates that the pointed-to object should live for the entirety of the object's life
  template<typename T>
  class managed : public ptr<T> {
  public:
    template<typename U>
    constexpr operator managed<U>() const { return managed<U>{static_cast<U*>(this->get())}; }

  public:
    inline managed() = default;
    inline explicit managed(T* p) : ptr<T>{p} {}
  };

  namespace {
    struct foo {};
    struct bar : foo {};
    [[maybe_unused]]
    consteval void test() {
      managed<bar> x;
      managed<foo> y = x;
      managed<foo> z = y;
      z = y;
    }
  }
}
