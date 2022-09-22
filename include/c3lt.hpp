#pragma once

#include <concepts>
#include <memory>
#include <type_traits>
#include <variant>

namespace c3lt {
  /// Indicates a normal pointer
  template<typename T>
  class ptr {
  private:
    T* p = nullptr;

  public:
    T* get() const { return p; }

  public:
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

  template<typename T>
  class safe_ptr;

  /// Indicates that the pointed-to object will outlive the returned value
  template<typename T>
  constexpr safe_ptr<T> managed(T* ptr) {
    return safe_ptr<T>{ptr};
  }

  template<typename T>
  constexpr safe_ptr<T> shared(std::shared_ptr<T> ptr) {
    return safe_ptr<T>{std::move(ptr)};
  }

  template<typename T, typename... Args>
  constexpr safe_ptr<T> make_shared(Args&&... args) {
    return safe_ptr<T>{std::make_shared<T>(std::forward<Args>(args)...)};
  }

  template<typename T>
  class safe_ptr {
  private:
    template<typename U>
    friend constexpr safe_ptr<U> managed(U*);

    template<typename U>
    friend class safe_ptr;

    template<typename U>
    using variant_t = std::variant<std::nullptr_t, U*, std::shared_ptr<U>>;

  private:
    variant_t<T> p;

  public:
    constexpr T* get() const {
      return std::visit([](auto& p) -> T* {
        using P_t = std::remove_cvref_t<decltype(p)>;
        if constexpr (std::same_as<P_t, std::shared_ptr<T>>)
          return p.get();
        else
          return p;
      }, p);
    }

  public:
    template<typename U>
    constexpr operator safe_ptr<U>() const {
      return safe_ptr<U>{std::visit([](auto& p) -> variant_t<U> {
        using P_t = std::remove_cvref_t<decltype(p)>;
        if constexpr (std::same_as<P_t, std::shared_ptr<T>>)
          return static_cast<std::shared_ptr<U>>(p);
        else
          return static_cast<U*>(p);
      }, p)};
    }
    constexpr operator bool() const { return p; }
    constexpr operator T*() const { return get(); }
    constexpr T& operator*() const { return *get(); }
    constexpr T* operator->() const { return get(); }

  public:
    constexpr safe_ptr() = default;
    constexpr safe_ptr(std::nullptr_t) {}
  private:
    inline safe_ptr(decltype(p)&& p_) : p{p_} {}
  };



//  /// Indicates that the pointed-to object should live for the entirety of the object's life
//  template<typename T>
//  class managed : public ptr<T> {
//  public:
//    template<typename U>
//    constexpr operator managed<U>() const { return managed<U>{static_cast<U*>(this->get())}; }

//  public:
//    inline managed() = default;
//    inline explicit managed(T* p) : ptr<T>{p} {}
//  };

  namespace {
    struct foo {};
    struct bar : foo {};
    [[maybe_unused]]
    static void test() {
      safe_ptr<bar> x;
      safe_ptr<foo> y = x;
      safe_ptr<foo> z = y;
      z = y;
    }
  }
}
