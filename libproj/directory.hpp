#include <exception>
#include <stdexcept>
#include <string>

class AlreadyExistsError : public std::exception{
  public:
    AlreadyExistsError() = default;

};
class PathNotFoundError : public std::exception{
  public:
    PathNotFoundError() = default;
};


/***
 * @brief: check if a directory exists
 * @param path: null terminated string specifying the path
 */
auto is_directory(std::wstring& path)->bool;
auto is_directory(const wchar_t *path) -> bool;

/**
 * @brief: create a directory
 */
auto create_directory(const wchar_t* path) -> void;
auto create_directory(std::wstring &path) -> void;

/**
 * @brief: delete a directory
 */
auto delete_directory(const wchar_t* path) -> void;
auto delete_directory(std::wstring &path) -> void;
