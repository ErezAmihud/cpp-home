#include <libproj/defer.hpp>
#include <memory>
#include <string>

FailStack::FailStack() { stack = std::vector<std::unique_ptr<Clearable>>(); }
void FailStack::add(std::unique_ptr<Clearable> clearable) {
  stack.push_back(std::move(clearable));
}

FailStack::~FailStack() {
  for (size_t i = 0; i < stack.size(); i++) {
    stack[stack.size() - 1 - i]->clear();
  }
}
