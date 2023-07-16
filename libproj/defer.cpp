#include <iostream>
#include <libproj/defer.hpp>
#include <memory>
#include <string>

FailStack::FailStack() { stack_ = std::vector<std::unique_ptr<Clearable>>(); }
void FailStack::add(std::unique_ptr<Clearable> clearable) {
  stack_.push_back(std::move(clearable));
}

FailStack::~FailStack() {
  std::cout << "clean" << std::endl;
  for (size_t i = 0; i < stack_.size(); i++) {
  std::cout << "something" << std::endl;
    stack_[stack_.size() - 1 - i]->clear();
  }
}

void FailStack::cancel() { stack_.clear(); }
