pushd libbacktrace
./configure
make CFLAGS=-fpic
popd
clang++ -shared -o cpp_exception_interposer.so -D_GNU_SOURCE cpp_exception_interposer.cpp -ldl -Ilibbacktrace -Llibbacktrace/.libs -lbacktrace -fPIC
