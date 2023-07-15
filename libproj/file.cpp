#include <libproj/file.hpp>
#include <stdexcept>
#include <windows.h>

// TODO use /? to not constrain to MAX_PATH
// TODO symbolic link behavior is - to override the target of the link
auto copy_file (std::wstring& src, std::wstring& dest) -> void{
  BOOL res = CopyFileW(src.c_str(), dest.c_str(),true);
  if (0==res)
    throw std::runtime_error("Copy file failed " + std::to_string(GetLastError()));

}

// TODO use /? to not constrain to MAX_PATH
auto delete_file(std::wstring& file_name)->void{
  BOOL res = DeleteFileW(file_name.c_str());
  if (0==res)
    throw std::runtime_error("Could not delete file " + std::to_string(GetLastError()));
}

// TODO use /? for MAX_PATH
auto is_file(std::wstring& file_name)->bool{
  DWORD ftyp = GetFileAttributesW(file_name.c_str());
  if (ftyp == INVALID_FILE_ATTRIBUTES)
    return false;
  if (!(ftyp & FILE_ATTRIBUTE_DIRECTORY))
    return true;
  return false;
}


