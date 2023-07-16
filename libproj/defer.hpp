#include <memory>
#include <vector>

class Clearable {
 public:
  Clearable() = default;
  virtual void clear() noexcept {};
};

class FailStack {
 public:
  FailStack();
  FailStack(const FailStack &other) = delete;
  FailStack(const FailStack &&other) noexcept = delete;
  FailStack &operator=(const FailStack &other) = delete;
  FailStack &operator=(FailStack &&other) noexcept = delete;
  ~FailStack();
  void add(std::unique_ptr<Clearable> clearable);
  void cancel();

 private:
  std::vector<std::unique_ptr<Clearable>> stack_;
};
