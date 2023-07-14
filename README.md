# cpp-base
A template for c++ projects


This is a template for creating c++ cmake projects with the [canonical file structure](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2018/p1204r0.html).
This project uses the [clang-cmake](https://github.com/ErezAmihud/clang-cmake) extention, so it contains:
- [GTest](https://github.com/google/googletest) - google test
- [Include What You Use](https://github.com/include-what-you-use/include-what-you-use)
- [Clang Tidy](https://clang.llvm.org/extra/clang-tidy/)
- [Clang Format](https://clang.llvm.org/docs/ClangFormat.html)

The project is built by the guidlines specified in the [c++ canonical project structure](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2018/p1204r0.html).

To run ctest with check for memory leaks use:
`ctest --overwrite MemoryCheckCommandOptions="--leak-check=full --error-exitcode=100" -T memcheck`


NOTE that for some reason, in mingw gtest compiles only with posix mingw, not the win32 one. The flag gtest_disable_pthreads=ON should fix it, but for me it did not 