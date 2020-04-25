#include <dlfcn.h>
#include <stdio.h>
#include <backtrace.h>
#include <typeinfo>

extern "C" void __cxa_throw(void* thrown_exception, std::type_info * tinfo, void (*dest)(void*))
{
  struct backtrace_state * state = backtrace_create_state (NULL, 0, NULL, NULL);

  fprintf(stderr, "*** C++ exception (%s) thrown ***\n", tinfo->name());
  backtrace_print(state, 0, stderr);
  fprintf(stderr, "*** Proceeding with C++ exception handling ***\n");

  // Call the real __cxa_throw
  void* (*real_function)(void*, void*, void(void*)) = reinterpret_cast<void *(*)(void *, void *, void (*)(void *))>(dlsym(RTLD_NEXT, "__cxa_throw"));
  real_function(thrown_exception, tinfo, dest);
}


