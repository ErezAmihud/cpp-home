#include <iostream>
#include <libproj/defer.hpp>
#include <libproj/directory.hpp>
#include <libproj/exceptions.hpp>
#include <libproj/file.hpp>
#include <libproj/install.hpp>
#include <memory>
#include <utility>

class DirectoryCleaner : public Clearable {
 public:
  explicit DirectoryCleaner(std::wstring name)
      : directory_name(std::move(name)) {}
  void clear() noexcept override {
    try {
      delete_directory(directory_name);
    } catch (...) {
    }
  }

 private:
  const std::wstring directory_name;
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
  const std::wstring file_name;
};

auto get_file_name(const std::wstring& path) -> std::wstring {
  return path.substr(path.rfind(L'\\') + 1, path.size());
}

auto install(const std::vector<std::wstring>& paths,
             const std::wstring& original_destination_directory) -> void {
  const std::wstring destination_directory =
      L"\\\\?\\" + original_destination_directory;
  FailStack fail_stack;
  try {
    size_t pos = destination_directory.find(
        L'\\', 7);  // search for path seperator without the first part of
                    // `drive` (`\\?\c:\`)
    while (std::wstring::npos != pos) {
      try {
        std::wstring path = destination_directory.substr(0, pos);
        std::wcout << L"dir " << path << std::endl;
        create_directory(path);
        fail_stack.add(std::make_unique<DirectoryCleaner>(std::move(path)));
      } catch (AlreadyExistsError& e) {
      }
      pos = destination_directory.find(L'\\', pos + 1);
    }
    try {
      create_directory(destination_directory);
      fail_stack.add(std::make_unique<DirectoryCleaner>(destination_directory));
    } catch (AlreadyExistsError& e) {
    }

    for (const auto& o_path : paths) {
      const std::wstring path = L"\\\\?\\" + o_path;
      std::wstring destination = destination_directory;
      auto file_name = get_file_name(o_path);
      destination.append(L"\\").append(file_name);
      copy_file(path, destination);
      fail_stack.add(std::make_unique<FileCleaner>(destination));
    }
    fail_stack.cancel();
  } catch (...) {
  }  // NOTE this catch block exists here because when an exception is not
     // caught then not all destructors are called.
}
