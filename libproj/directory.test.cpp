#include <gtest/gtest.h>
#include <windows.h>
#include <string>
#include <stdexcept>
#include <libproj/directory.hpp>
#include <libproj/defer.hpp>

/***
 * @brief get the system temp directory path.
 * @param wchar_t path [out]: Pointer to a buffer in the len of MAX_PATH+1.
 * return: the size of the temp path
 *
 * @NOTE this assumes that the temp path exists
 */
auto get_temp_dir(wchar_t* path) -> DWORD{
  // TODO remove usage of wchar_t and return std::string. Write a note about stack based string
  DWORD path_size= GetTempPathW(MAX_PATH, path);
  if (0==path_size)
    throw std::runtime_error(std::string("Get temp path failed with ")+ std::to_string(GetLastError()));
  return path_size;
}

auto path_exists(wchar_t* path) -> bool{
  DWORD ftyp = GetFileAttributesW(path);
  if (ftyp == INVALID_FILE_ATTRIBUTES)
    return false;
  return true;    // this is not a directory!
}

TEST(DirectoryExistsTest, Exists){
  WCHAR lpTempPathBuffer[MAX_PATH];
  get_temp_dir(lpTempPathBuffer);
  ASSERT_TRUE(is_directory_exists(lpTempPathBuffer));
}

TEST(DirectoryExistsTest, DoesNotExists){
  WCHAR lpTempPathBuffer[MAX_PATH];
  DWORD size = get_temp_dir(lpTempPathBuffer);
  // Stop the string sooner then the directory to check on non existant
  lpTempPathBuffer[size-2] = 0;
  ASSERT_FALSE(is_directory_exists(lpTempPathBuffer));
}

class DirectoryFile: public ::testing::Test {
 protected:
  void SetUp() override {
    DWORD size = get_temp_dir(lpTempPathBuffer);
    lpTempPathBuffer[size] = L'\\';
    lpTempPathBuffer[size+1] = L'a';
    lpTempPathBuffer[size+2] = 0;
    HANDLE file = CreateFileW(lpTempPathBuffer, GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    // TODO handle raii
    // TODO raise
    CloseHandle(file);
  }

  void TearDown() override {
    // TODO can disable path limitation with `\\?\` at the start of the function.
    DeleteFileW(lpTempPathBuffer); // TODO proper delete file with error handling
  }


  WCHAR lpTempPathBuffer[MAX_PATH];

};

TEST_F(DirectoryFile, NotADiretroy){
  // Stop the string sooner then the directory to check on non existant
  ASSERT_FALSE(is_directory_exists(lpTempPathBuffer));
}

TEST(CreateDirectoryTests, InGoodPath){
  WCHAR lpTempPathBuffer[MAX_PATH+10];
  DWORD size=get_temp_dir(lpTempPathBuffer);
  lpTempPathBuffer[size]=L'\\';
  lpTempPathBuffer[size+1]=L'd';
  lpTempPathBuffer[size+2]=0;
  ASSERT_FALSE(is_directory_exists(lpTempPathBuffer));
  create_directory(lpTempPathBuffer);
  // TODO prepend `\\?\`
  auto _defered_delete = defer_func([lpTempPathBuffer](){RemoveDirectoryW(lpTempPathBuffer);});
  ASSERT_TRUE(is_directory_exists(lpTempPathBuffer));
}

TEST(CreateDirectoryTests, Exists){
  WCHAR lpTempPathBuffer[MAX_PATH+10];
  DWORD size=get_temp_dir(lpTempPathBuffer);
  ASSERT_TRUE(is_directory_exists(lpTempPathBuffer));
  EXPECT_THROW(create_directory(lpTempPathBuffer), AlreadyExistsError);
}
TEST(CreateDirectoryTests, MultipleInner){
  WCHAR lpTempPathBuffer[MAX_PATH+10];
  DWORD size=get_temp_dir(lpTempPathBuffer);
  lpTempPathBuffer[size]=L'\\';
  lpTempPathBuffer[size+1]=L'e';
  lpTempPathBuffer[size+2]=0;
  ASSERT_FALSE(is_directory_exists(lpTempPathBuffer));
  lpTempPathBuffer[size+2]=L'\\';
  lpTempPathBuffer[size+3]=L'd';
  lpTempPathBuffer[size+4]=0;
  ASSERT_FALSE(is_directory_exists(lpTempPathBuffer));
  EXPECT_THROW(create_directory(lpTempPathBuffer),PathNotFoundError );
}


TEST(DeleteDirectoryTests, existing_one){
  WCHAR lpTempPathBuffer[MAX_PATH+10];
  DWORD size=get_temp_dir(lpTempPathBuffer);
  lpTempPathBuffer[size]=L'\\';
  lpTempPathBuffer[size+1]=L'f';
  lpTempPathBuffer[size+2]=0;
  try{
    create_directory(lpTempPathBuffer);
  } catch(AlreadyExistsError& e){}
  delete_directory(lpTempPathBuffer);
  ASSERT_FALSE(is_directory_exists(lpTempPathBuffer));
}

TEST(DeleteDirectoryTests, NotExists){
  WCHAR lpTempPathBuffer[MAX_PATH+10];
  DWORD size=get_temp_dir(lpTempPathBuffer);
  lpTempPathBuffer[size]=L'\\';
  lpTempPathBuffer[size+1]=L'g';
  lpTempPathBuffer[size+2]=0;
  ASSERT_FALSE(is_directory_exists(lpTempPathBuffer));
  EXPECT_THROW(delete_directory(lpTempPathBuffer), std::runtime_error);
}

TEST(DeleteDirectoryTests, recursive){
  WCHAR lpTempPathBuffer[MAX_PATH+10];
  DWORD size=get_temp_dir(lpTempPathBuffer);
  lpTempPathBuffer[size]=L'\\';
  lpTempPathBuffer[size+1]=L'f';
  lpTempPathBuffer[size+2]=0;
  create_directory(lpTempPathBuffer);
  lpTempPathBuffer[size+2]=L'\\';
  lpTempPathBuffer[size+3]=L'f';
  lpTempPathBuffer[size+4]=0;
  create_directory(lpTempPathBuffer);
  lpTempPathBuffer[size+2]=0;
  EXPECT_THROW(delete_directory(lpTempPathBuffer), std::runtime_error);
}
