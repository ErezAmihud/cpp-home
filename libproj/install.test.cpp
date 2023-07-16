#include <gtest/gtest.h>
#include <libproj/file.hpp>
#include <libproj/directory.hpp>
#include <libproj/install.hpp>
#include <libproj/temp_path.hpp>


// TODO make work multiple times - delete directory before run
TEST(InstallTest, RecursiveDirectory) {
  auto temp_file1 = get_temp_file();
  auto temp_file2 = get_temp_file();
  auto temp_path = get_temp_dir() + L"L\\abc\\";
  std::vector<std::wstring> paths{temp_file1, temp_file2};
  install(paths, temp_path);

  ASSERT_TRUE(is_directory(temp_path));
  auto a = temp_path + L"1";
  ASSERT_TRUE(is_file(a));
  a = temp_path + L"2";
  ASSERT_TRUE(is_file(a));
  std::cout << "file 3" << std::endl;
}
/*
TEST(BTest, c) {
  auto temp_file1 = get_temp_file();
  auto temp_file2 = get_temp_file();
  auto temp_path = get_temp_dir();
  std::vector<std::wstring> paths{temp_file1, temp_file2};
  install(paths, temp_path);

  auto a = temp_path + L"1";
  ASSERT_TRUE(is_file(a));
  a = temp_path + L"2";
  ASSERT_TRUE(is_file(a));
}*/
