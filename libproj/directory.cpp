#include <libproj/directory.hpp>
#include <windows.h>
#include <libproj/exceptions.hpp>
#include <string>
#include <stdexcept>

auto is_directory(const std::wstring &path) -> bool {
  DWORD ftyp = GetFileAttributesW(path.c_str());
  if (ftyp == INVALID_FILE_ATTRIBUTES) return false;

  if (ftyp & FILE_ATTRIBUTE_DIRECTORY) return true;

  return false;
}

auto create_directory(const std::wstring &path) -> void {
  if (0 != CreateDirectoryW(path.c_str(), nullptr)) {
    return;
  }
  DWORD error = GetLastError();
  if (error == ERROR_ALREADY_EXISTS) {
    throw AlreadyExistsError();
  }
  if (ERROR_PATH_NOT_FOUND) {
    throw PathNotFoundError();
  }
  throw std::runtime_error(std::string("Directory creation error ") +
                           std::to_string(error));
  // should be unreachable
}

auto delete_directory(const std::wstring &path) -> void {
  BOOL res = RemoveDirectoryW(path.c_str());
  if (0 != res) {
    return;
  }
  throw std::runtime_error(std::to_string(GetLastError()));
}
