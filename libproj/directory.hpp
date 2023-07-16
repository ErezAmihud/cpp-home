#include <string>

/***
 * @brief: check if a directory exists
 * @param path: null terminated string specifying the path
 */
auto is_directory(const std::wstring &path) -> bool;

/**
 * @brief: create a directory
 */
auto create_directory(const std::wstring &path) -> void;

/**
 * @brief: delete a directory
 */
auto delete_directory(const std::wstring &path) -> void;
