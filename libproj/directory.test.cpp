#include <gtest/gtest.h>
#include <libproj/defer.hpp>
#include <libproj/directory.hpp>
#include <libproj/temp_path.hpp>
#include <libproj/file.hpp>
#include <libproj/exceptions.hpp>
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
  return ftyp != INVALID_FILE_ATTRIBUTES;
}

TEST(DirectoryExistsTest, Exists) {
  auto path = get_temp_dir();
  ASSERT_TRUE(is_directory(path));
}

TEST(DirectoryExistsTest, DoesNotExists) {
  auto temp_dir = get_temp_dir();
  temp_dir += L"nonexistant";
  ASSERT_FALSE(is_directory(temp_dir));
}

class DirectoryFile : public ::testing::Test {
protected:
  void SetUp() override {
    temp_path = get_temp_file();
  }

  void TearDown() override {
    delete_file(temp_path);
  }

  std::wstring temp_path;
};

TEST_F(DirectoryFile, NotADiretroy) {
  ASSERT_FALSE(is_directory(temp_path));
}

TEST(CreateDirectoryTests, InGoodPath) {
  auto temp_dir = get_temp_dir();
  temp_dir += L"d";
  ASSERT_FALSE(is_directory(temp_dir));
  create_directory(temp_dir);
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

TEST(DeleteDirectoryTests, ExistingOne) {
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
}
