#include <windows.h>

#include <libproj/temp_path.hpp>
#include <stdexcept>

auto get_temp_dir() -> std::wstring {
  wchar_t path[MAX_PATH + 1];

  DWORD path_size = GetTempPathW(MAX_PATH + 1, path);
  if (0 == path_size)
    throw std::runtime_error(std::string("Get temp path size failed with ") +
                             std::to_string(GetLastError()));
  return std::wstring(path);
}

auto get_temp_file() -> std::wstring {
  wchar_t temp_file[MAX_PATH + 1];
  auto path = get_temp_dir();

  DWORD res = GetTempFileNameW(path.c_str(), L"test_file", 0, temp_file);
  if (0 == res)
    throw std::runtime_error("Temp file creation problem " +
                             std::to_string(GetLastError()));
  return std::wstring(temp_file);
}
