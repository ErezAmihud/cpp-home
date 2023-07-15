#include <libproj/directory.hpp>
#include <stdexcept>
#include <string>
#include <utility>
#include <windows.h>

auto is_directory(const std::wstring &path) -> bool {
  DWORD ftyp = GetFileAttributesW(path.c_str());
  if (ftyp == INVALID_FILE_ATTRIBUTES)
    return false;

  if (ftyp & FILE_ATTRIBUTE_DIRECTORY)
    return true; // this is a directory!

  return false; // this is not a directory!
}

auto create_directory(const wchar_t *path) -> void {
  // TODO add support for more then MAX_PATH with `\?`
  if (0 != CreateDirectoryW(path, nullptr)) {
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
    // shold be unreachable

}

auto create_directory(const std::wstring &path) -> void {
  return create_directory(path.c_str());
}

// TODO allow infinate paths with `\\?`
// TODO maybe make a switch case for some last error errors?
auto delete_directory(const wchar_t *path) -> void {
  BOOL res = RemoveDirectoryW(path);
  if (0 != res) {
    return;
  }
  throw std::runtime_error(std::to_string(GetLastError()));
}
auto delete_directory(const std::wstring &path) -> void {
  delete_directory(path.c_str());
}
