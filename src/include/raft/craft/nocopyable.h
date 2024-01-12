#pragma once
class noncopyable {
 protected:
  noncopyable() = default;
  ~noncopyable() = default;

 public:
  noncopyable(const noncopyable &) = delete;
  noncopyable &operator=(const noncopyable &) = delete;
};