#include <libproj/file.hpp>
#include <stdexcept>
#include <windows.h>
#include <libproj/exceptions.hpp>

auto copy_file(const std::wstring &src, const std::wstring &dest) -> void {
  BOOL res = CopyFileW(src.c_str(), dest.c_str(), TRUE);
  if (0 == res){
    DWORD last_error = GetLastError();
    if (last_error == ERROR_ALREADY_EXISTS)
      throw AlreadyExistsError();

    throw std::runtime_error("Copy file failed " +
                             std::to_string(GetLastError()));
  }
}

auto delete_file(const std::wstring &file_name) -> void {
  BOOL res = DeleteFileW(file_name.c_str());
  if (0 == res)
    throw std::runtime_error("Could not delete file " +
                             std::to_string(GetLastError()));
}

auto is_file(const std::wstring &file_name) -> bool {
  DWORD ftyp = GetFileAttributesW(file_name.c_str());
  if (ftyp == INVALID_FILE_ATTRIBUTES)
    return false;
  if (!(ftyp & FILE_ATTRIBUTE_DIRECTORY))
    return true;
  return false;
}

