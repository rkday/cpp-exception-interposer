# cpp-exception-interposer

This library intercepts any thrown C++ exception, and prints a stack trace of the point it was thrown from - even if the exception is caught and ignored.

It's useful for debugging exceptions in cases where using `gdb` and `catch throw` is difficult, or for profiling applications that may be inefficiently using C++ exceptions for mainline control flow.

It's licensed under the 3-clause BSD license (<https://github.com/rkday/cpp-exception-interposer/releases>), as is libbacktrace (<https://github.com/ianlancetaylor/libbacktrace/blob/ca0de0517f3be44fedf5a2c01cfaf6437d4cae68/LICENSE>), which is statically linked into the .so.

## Downloading

The .so is available from <https://github.com/rkday/cpp-exception-interposer/releases>.

## Compiling

To compile from source:

- check out this repository (and the libbacktrace submodule) with `git clone --recursive`
- run `./build.sh`

## Running

You can use this by setting `LD_PRELOAD=cpp_exception_interposer.so`.

For example, this program (compiled with `g++ -g -std=c++17 -o exceptions exceptions.cpp`):

```cpp
#include <stdexcept>
#include <stdio.h>
#include <optional>

void foo()
{
  std::optional<int> x;
  x.value();
}

void bar()
{
  try {
    foo();
  } catch (std::exception& e) {
    printf("Exception caught and ignored\n");
  }
}

int main()
{
  bar();
}
```

normally provides this output:

```
$ ./exceptions 
Exception caught and ignored
```

but running it with the interposer provides much better diagnostics:

```
$ LD_PRELOAD=cpp-exception-interposer/cpp_exception_interposer.so ./exceptions 
*** C++ exception (St19bad_optional_access) thrown ***
0x7fa4646ee451 ???
  ???:0
0x55958f3f12ea _ZSt27__throw_bad_optional_accessv
  /usr/include/c++/9.3.0/optional:99
0x55958f3f132c _ZNRSt8optionalIiE5valueEv
  /usr/include/c++/9.3.0/optional:931
0x55958f3f11e3 _Z3foov
  /home/rkd/src/exceptions.cpp:8
0x55958f3f1208 _Z3barv
  /home/rkd/src/exceptions.cpp:14
0x55958f3f125a main
  /home/rkd/src/exceptions.cpp:22
0x7fa4641ca022 ???
  ???:0
0x55958f3f10ed ???
  ???:0
0xffffffffffffffff ???
  ???:0
*** Proceeding with C++ exception handling ***
Exception caught and ignored
```
