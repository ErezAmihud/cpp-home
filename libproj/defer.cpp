#include <libproj/defer.hpp>
#include <stddef.h>
#include <memory>
#include <type_traits>

FailStack::FailStack() { stack_ = std::vector<std::unique_ptr<Clearable>>(); }
void FailStack::add(std::unique_ptr<Clearable> clearable) {
  stack_.push_back(std::move(clearable));
}

FailStack::~FailStack() {
  for (size_t i = 0; i < stack_.size(); i++) {
    stack_[stack_.size() - 1 - i]->clear();
  }
}

void FailStack::cancel() { stack_.clear(); }
