#include <libproj/install.hpp>
#include <string>
#include <vector>

int main() {
  std::vector<std::wstring> paths = {L"C:\\temp\\a.txt", L"C:\\temp\\b.txt"};
  std::wstring destination_dir = L"C:\\Users\\ErezAmihud\\AppData\\Local\\Temp";
  install(paths, destination_dir);
  return 0;
}
