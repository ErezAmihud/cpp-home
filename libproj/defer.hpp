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
  // TODO rule of 5
  // TODO maybe this should be a Clearable?
  public:
    FailStack();
     /*rule_of_five(const rule_of_five& other) // copy constructor
        : rule_of_five(other.cstring) {}

    rule_of_five(rule_of_five&& other) noexcept // move constructor
        : cstring(std::exchange(other.cstring, nullptr)) {}

    rule_of_five& operator=(const rule_of_five& other) // copy assignment
    {
        return *this = rule_of_five(other);
    }

    rule_of_five& operator=(rule_of_five&& other) noexcept // move assignment
    {
        std::swap(cstring, other.cstring);
        return *this;
    }*/
    ~FailStack();
    void add(std::unique_ptr<Clearable> clearable);
  private:
    std::vector<std::unique_ptr<Clearable>> stack;
};
