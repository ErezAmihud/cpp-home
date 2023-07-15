#include <vector>
#include <memory>

template <typename F>
struct privDefer {
	F f;
	privDefer(F f) : f(f) {}
	~privDefer() { f(); }
};

template <typename F>
privDefer<F> defer_func(F f) {
	return privDefer<F>(f);
}



class Clearable {
   public:
     Clearable(){}
      virtual void clear() noexcept{};
};

class FailStack{
  // TODO rule of 5 - change the moveable operators to not deleted
  public:
    FailStack();
    FailStack(const FailStack& other)=delete;
    FailStack(const FailStack&& other) noexcept = delete;
    FailStack& operator=(const FailStack& other)=delete;
    FailStack& operator=(FailStack&& other) noexcept=delete;
    ~FailStack();
    void add(std::unique_ptr<Clearable> clearable);
    void cancel();
  private:
    std::vector<std::unique_ptr<Clearable>> stack;
};
