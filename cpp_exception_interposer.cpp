#include <dlfcn.h>
#include <stdio.h>
#include <backtrace.h>
#include <typeinfo>

// Function signature of cxa_throw - returns nothing, accepts three arguments (pointer to void, pointer to type info, function pointer)
typedef void (*cxa_throw_fn_t)(void*, std::type_info*, void(void*));

extern "C" void __cxa_throw(void* thrown_exception, std::type_info * tinfo, void (*dest)(void*))
{
  struct backtrace_state * state = backtrace_create_state (NULL, 0, NULL, NULL);

  fprintf(stderr, "*** C++ exception (%s) thrown ***\n", tinfo->name());
  backtrace_print(state, 0, stderr);
  fprintf(stderr, "*** Proceeding with C++ exception handling ***\n");

  // Get a pointer to the real __cxa_throw function
  cxa_throw_fn_t real_function = reinterpret_cast<cxa_throw_fn_t>(dlsym(RTLD_NEXT, "__cxa_throw"));
  // Call the real __cxa_throw
  real_function(thrown_exception, tinfo, dest);
}


