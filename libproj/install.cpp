#include <libproj/defer.hpp>
#include <libproj/directory.hpp>
#include <libproj/file.hpp>
#include <libproj/install.hpp>
#include <memory>
#include <utility>

class DirectoryCleaner : public Clearable {
public:
  explicit DirectoryCleaner(std::wstring name) : directory_name(std::move(name)) {}
  void clear() noexcept override {
    try {
      delete_directory(directory_name);
    } catch (...) {
    }
  }

private:
  std::wstring directory_name;
};

class FileCleaner : public Clearable {
public:
  explicit FileCleaner(std::wstring name) : file_name(std::move(name)) {}
  void clear() noexcept override {
    try {
      delete_file(file_name);
    } catch (...) {
    }
  }

private:
  std::wstring file_name;
};

auto install(std::vector<std::wstring> paths,
             std::wstring destination_directory) -> void {
  FailStack fail_stack;
  size_t pos = destination_directory.find(L'\\',3); // search for path seperator without the first part of `drive`
  while(std::wstring::npos != pos){
    try {
      std::wstring path(destination_directory, pos);
      create_directory(path);
      fail_stack.add(std::make_unique<DirectoryCleaner>(std::move(path)));
    } catch (AlreadyExistsError &e) {
    }
    pos = destination_directory.find(L'\\', pos);
  }
  int a = 0;
  for (const auto &path : paths) {
    a++;
    std::wstring b(destination_directory + std::to_wstring(a));
    copy_file(path, b);
    fail_stack.add(std::make_unique<DirectoryCleaner>(destination_directory));
  }
}
