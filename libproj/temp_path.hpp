#include <string>

/***
 * @brief get the path to the system's temporary dir
 * @NOTE the path is not gerenteed to exist
 * @NOTE it uses `GetTempPath`
 */
auto get_temp_dir() -> std::wstring;

/***
 * @brief get a temporary file path. The file is created.
 * @NOTE it uses `GetTempFileName`
 */
auto get_temp_file() -> std::wstring;
