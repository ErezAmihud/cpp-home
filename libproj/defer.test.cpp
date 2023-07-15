#include <gtest/gtest.h>

#include <libproj/defer.hpp>
#include <memory>

class BoolClearable : public Clearable {
 public:
  explicit BoolClearable(bool *b) : flag(b) {}
  void clear() noexcept override { *flag = true; }

 private:
  bool *flag;
};

TEST(DeferStack, OneItem) {
  bool flag = false;
  {
    FailStack stack;
    auto b = std::make_unique<BoolClearable>(&flag);
    stack.add(std::move(b));
  }
  ASSERT_TRUE(flag);
}
TEST(DeferStack, cancel) {
  bool flag = false;
  {
    FailStack stack;
    auto b = std::make_unique<BoolClearable>(&flag);
    stack.add(std::move(b));
    stack.cancel();
  }
  ASSERT_FALSE(flag);
}
class MultiClear : public Clearable {
 public:
  MultiClear(std::vector<int> &b, int num) : arr(b), num(num) {}
  void clear() noexcept override { arr.push_back(num); }

 private:
  std::vector<int> &arr;
  int num;
};

// tests the order of the cleanup
TEST(DeferStack, TwoItems) {
  std::vector<int> items;
  {
    FailStack stack;
    auto b = std::make_unique<MultiClear>(items, 1);
    stack.add(std::move(b));
    auto b2 = std::make_unique<MultiClear>(items, 2);
    stack.add(std::move(b2));
  }
  ASSERT_EQ(items.size(), 2);
  ASSERT_EQ(items[0], 2);
  ASSERT_EQ(items[1], 1);
}
