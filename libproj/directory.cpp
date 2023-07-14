#include <libproj/directory.hpp>
#include <stdexcept>
#include <windows.h>

auto is_directory_exists(wchar_t *path) -> bool {
  DWORD ftyp = GetFileAttributesW(path);
  if (ftyp == INVALID_FILE_ATTRIBUTES)
    return false;

  if (ftyp & FILE_ATTRIBUTE_DIRECTORY)
    return true; // this is a directory!

  return false; // this is not a directory!
}

auto create_directory(const wchar_t* path) -> void{
  // TODO add support for more then MAX_PATH with `\?`
  if (0 != CreateDirectoryW(path, NULL)) {
    return;
  }
  DWORD error = GetLastError();
  if (error == ERROR_ALREADY_EXISTS) {
    throw AlreadyExistsError();
  } else if (ERROR_PATH_NOT_FOUND) {
    throw PathNotFoundError();
  } else {
    throw std::runtime_error(std::string("Directory creation error ") +
                             std::to_string(error));
    // shold be unreachable
  }
}

auto create_directory(std::wstring &path) -> void {
  return create_directory(path.c_str());
}

// TODO allow infinate paths with `\\?`
// TODO maybe make a switch case for some last error errors?
auto delete_directory(const wchar_t* path) -> void{
  BOOL res = RemoveDirectoryW(path);
  if(0!=res){
    return;
  }
  throw std::runtime_error(std::to_string(GetLastError()));
}
auto delete_directory(std::wstring &path) -> void{delete_directory(path.c_str());}
