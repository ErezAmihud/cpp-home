# cpp-base

This program was tested with clang-cl x86. To compile and run the tests use:

```
cmake --preset clang-cl
cmake --build build\clang-cl
ctest --test-dir .\build\clang-cl
```





This used a template for creating c++ cmake projects with the [canonical file structure](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2018/p1204r0.html).
This project uses the [clang-cmake](https://github.com/ErezAmihud/clang-cmake) extention, so it contains:
- [GTest](https://github.com/google/googletest) - google test
- [Include What You Use](https://github.com/include-what-you-use/include-what-you-use) - (not used in this project)
- [Clang Tidy](https://clang.llvm.org/extra/clang-tidy/) - (not used in this project)
- [Clang Format](https://clang.llvm.org/docs/ClangFormat.html)

The project is built by the guidlines specified in the [c++ canonical project structure](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2018/p1204r0.html).
That is why all of the includes are using `<` and not `"`, and the `.hpp`, `.test.cpp` endings.

