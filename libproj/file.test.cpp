#include <gtest/gtest.h>

#include <libproj/defer.hpp>
#include <libproj/directory.hpp>
#include <libproj/file.hpp>
#include <libproj/temp_path.hpp>
#include <stdexcept>

class TempFile : public ::testing::Test {
 protected:
  void SetUp() override { temp_file = get_temp_file(); }

  void TearDown() override { delete_file(temp_file); }

  std::wstring temp_file;
};
TEST_F(TempFile, exists) {
  auto temp_path = get_temp_dir();
  temp_path += L"ueue";
  ASSERT_TRUE(is_file(temp_file));
  copy_file(temp_file, temp_path);
  ASSERT_TRUE(is_file(temp_path));
  delete_file(temp_path);
}

TEST_F(TempFile, SrcNotExists) {
  auto temp_path = get_temp_dir() + L"ueue";
  std::wstring temp_file = L"C:\\ueouoeuo\\ueocnh.txt";
  ASSERT_FALSE(is_file(temp_file));
  EXPECT_THROW(copy_file(temp_file, temp_path), std::runtime_error);
}

TEST_F(TempFile, DestNotInExistDirectory) {
  auto temp_path = get_temp_dir() + L"ueue";
  ASSERT_FALSE(is_directory(temp_path));
  temp_path += L"\\te.txt";
  EXPECT_THROW(copy_file(temp_file, temp_path), std::runtime_error);
}
