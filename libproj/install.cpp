#include <libproj/defer.hpp>
#include <libproj/directory.hpp>
#include <libproj/file.hpp>
#include <libproj/install.hpp>
#include <memory>
#include <utility>
#include <iostream>

class DirectoryCleaner : public Clearable {
public:
  explicit DirectoryCleaner(std::wstring name) : directory_name(std::move(name)) {}
  void clear() noexcept override {
    try {
      std::wcout << L"clean dir " << directory_name<< std::endl;
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
      std::wcout << L"clean file " << file_name << std::endl;
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
  try{
  size_t pos = destination_directory.find(L'\\',3); // search for path seperator without the first part of `drive`
  while(std::wstring::npos != pos){
    try {
      std::wstring path = destination_directory.substr(0, pos);
      create_directory(path);
      fail_stack.add(std::make_unique<DirectoryCleaner>(std::move(path)));
    } catch (AlreadyExistsError &e) {
    }
    pos = destination_directory.find(L'\\', pos+1);
  }
  try {
    create_directory(destination_directory);
    fail_stack.add(std::make_unique<DirectoryCleaner>(destination_directory));
  } catch (AlreadyExistsError &e) {
  }
  int a = 0;
  for (const auto &path : paths) {
    a++;
    std::wstring b(destination_directory+L"\\" + std::to_wstring(a));
    std::cout << "copy file 1";
    copy_file(path, b);
    fail_stack.add(std::make_unique<FileCleaner>(b));
  }
  std::cout << "done good";
  fail_stack.cancel();
  } catch(...){}
}
