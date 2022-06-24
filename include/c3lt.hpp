#pragma once

namespace c3lt {
  /// Indicates a normal pointer
  template<typename T>
  class ptr {
  private:
    T* p = nullptr;

  public:
    inline operator bool() const { return p; }
    inline operator T*() const { return p; }
    inline T& operator*() const { return *p; }
    inline T* operator->() const { return p; }

  public:
    inline ptr() = default;
    inline ptr(ptr const&) = default;
    inline ptr(ptr&&) = default;
    inline ptr(T* p_) : p{p_} {}
  };

  /// Indicates that the pointed-to object should live for the entirety of the object's life
  template<typename T>
  class managed : public ptr<T> {
  public:
    inline managed() = default;
    inline explicit managed(T* p) : ptr<T>{p} {}
  };
}
