#include <libproj/install.hpp>
#include <libproj/directory.hpp>
#include <libproj/file.hpp>
#include <libproj/defer.hpp>



auto install(std::vector<std::wstring> paths, std::wstring destination_directory) -> void{
  try{
    create_directory(destination_directory);

  } catch (AlreadyExistsError& e){}

}
