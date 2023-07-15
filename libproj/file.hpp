#include <string>

auto copy_file(const std::wstring &src, const std::wstring &dest) -> void;
auto delete_file(const std::wstring &file_name) -> void;
auto is_file(const std::wstring &file_name) -> bool;
