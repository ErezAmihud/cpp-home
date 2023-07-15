#include <libproj/install.hpp>
#include <libproj/directory.hpp>
#include <libproj/file.hpp>
#include <libproj/defer.hpp>
#include <memory>


class DirectoryCleaner : public Clearable{
  public:
    DirectoryCleaner(std::wstring name):directory_name(name){}
    void clear() noexcept{
      try{
        delete_directory(directory_name);
      } catch(...){
      }
    }
  private:
    std::wstring directory_name;

};

class FileCleaner : public Clearable{
  public:
    FileCleaner(std::wstring name):file_name(name){}
    void clear() noexcept{
      try{
        delete_file(file_name);
      } catch(...){
      }
    }
  private:
    std::wstring file_name;

};

auto install(std::vector<std::wstring> paths, std::wstring destination_directory) -> void{
  FailStack fail_stack;
  try{
    create_directory(destination_directory);
    fail_stack.add(std::make_unique<DirectoryCleaner>(destination_directory));
  } catch (AlreadyExistsError& e){}
  int a = 0;
  for(const auto& path : paths){
    a++;
    std::wstring b (destination_directory+std::to_wstring(a));
    copy_file(path,b);
    fail_stack.add(std::make_unique<DirectoryCleaner>(destination_directory));
  }
}
