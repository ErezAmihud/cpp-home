#include <gtest/gtest.h>
#include <libproj/defer.hpp>
#include <libproj/directory.hpp>
#include <libproj/temp_path.hpp>
#include <stdexcept>
#include <string>
#include <utility>
#include <windows.h>

/***
 * @brief get the system temp directory path.
 * @param wchar_t path [out]: Pointer to a buffer in the len of MAX_PATH+1.
 * return: the size of the temp path
 *
 * @NOTE this assumes that the temp path exists
 */

auto path_exists(wchar_t *path) -> bool {
  DWORD ftyp = GetFileAttributesW(path);
  if (ftyp == INVALID_FILE_ATTRIBUTES)
    return false;
  return true; // this is not a directory!
}

TEST(DirectoryExistsTest, Exists) {
  auto path = get_temp_dir();
  ASSERT_TRUE(is_directory(path));
}

TEST(DirectoryExistsTest, DoesNotExists) {
  auto temp_dir = get_temp_dir();
  temp_dir += L"nonexistant";
  // Stop the string sooner then the directory to check on non existant
  ASSERT_FALSE(is_directory(temp_dir));
}

class DirectoryFile : public ::testing::Test {
protected:
  void SetUp() override {
    auto temp_dir = get_temp_dir();
    temp_dir += L"a";
    HANDLE file = CreateFileW(lpTempPathBuffer, GENERIC_READ | GENERIC_WRITE,
                              FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,
                              CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    // TODO handle raii
    // TODO raise
    CloseHandle(file);
  }

  void TearDown() override {
    // TODO can disable path limitation with `\\?\` at the start of the
    // function.
    DeleteFileW(
        lpTempPathBuffer); // TODO proper delete file with error handling
  }

  WCHAR lpTempPathBuffer[MAX_PATH];
};

TEST_F(DirectoryFile, NotADiretroy) {
  // Stop the string sooner then the directory to check on non existant
  ASSERT_FALSE(is_directory(lpTempPathBuffer));
}

TEST(CreateDirectoryTests, InGoodPath) {
  auto temp_dir = get_temp_dir();
  temp_dir += L"d";
  ASSERT_FALSE(is_directory(temp_dir));
  create_directory(temp_dir);
  // TODO prepend `\\?\`
  ASSERT_TRUE(is_directory(temp_dir));
  delete_directory(temp_dir);
}

TEST(CreateDirectoryTests, Exists) {
  auto temp_dir = get_temp_dir();
  ASSERT_TRUE(is_directory(temp_dir));
  EXPECT_THROW(create_directory(temp_dir), AlreadyExistsError);
}
TEST(CreateDirectoryTests, MultipleInner) {
  auto temp_dir = get_temp_dir();
  temp_dir += L"\\e";
  ASSERT_FALSE(is_directory(temp_dir));
  temp_dir += L"\\d";
  ASSERT_FALSE(is_directory(temp_dir));
  EXPECT_THROW(create_directory(temp_dir), PathNotFoundError);
}

TEST(DeleteDirectoryTests, existing_one) {
  auto temp_dir = get_temp_dir();
  temp_dir += L"q";
  try {
    create_directory(temp_dir);
  } catch (AlreadyExistsError &e) {
  }
  delete_directory(temp_dir);
  ASSERT_FALSE(is_directory(temp_dir));
}

TEST(DeleteDirectoryTests, NotExists) {
  auto temp_dir = get_temp_dir();
  temp_dir += L"g";
  ASSERT_FALSE(is_directory(temp_dir));
  EXPECT_THROW(delete_directory(temp_dir), std::runtime_error);
}

TEST(DeleteDirectoryTests, recursive) {
  auto temp_dir = get_temp_dir();
  temp_dir += L"j";
  try {
    create_directory(temp_dir);
  } catch (AlreadyExistsError &e) {
  }
  temp_dir += L"\\f";
  try {
    create_directory(temp_dir);
  } catch (AlreadyExistsError &e) {
  }
  temp_dir.resize(temp_dir.size() - 2);
  EXPECT_THROW(delete_directory(temp_dir), std::runtime_error);
  // TODO clear - delete dir
}
