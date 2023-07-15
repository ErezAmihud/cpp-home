#include <gtest/gtest.h>
#include <libproj/defer.hpp>
#include <memory>

class BoolClearable: public Clearable{
  public:
  BoolClearable(bool* b):flag(b),Clearable(){}
  void clear() noexcept override{
    *flag=true;
  }
  private:
  bool* flag;
};

TEST(DeferStack,one_item){
  bool flag = false;
  {
    FailStack stack;
    auto b = std::make_unique<BoolClearable>(&flag);
    stack.add(std::move(b));
  }
  ASSERT_TRUE(flag);
}
class MultiClear: public Clearable{
  public:
    MultiClear(std::vector<int> &b, int num):arr(b),num(num),Clearable(){}
    void clear() noexcept override{
      arr.push_back(num);
    }
  private:
  std::vector<int>& arr;
  int num;
    };

// tests that the stack cleans from the end to the first added clear
TEST(DeferStack,two_items){
  std::vector<int> items;
  {
    FailStack stack;
    auto b = std::make_unique<MultiClear>(items,1);
    stack.add(std::move(b));
    auto b2 = std::make_unique<MultiClear>(items,2);
    stack.add(std::move(b2));
  }
  ASSERT_EQ(items.size(), 2);
  ASSERT_EQ(items[0], 2);
  ASSERT_EQ(items[1], 1);

}
