#ifndef ALCHEMY_STATIC_ASSERT_H
#define ALCHEMY_STATIC_ASSERT_H

#include "types.h"
#include "stddef.h"

#ifdef MODERN
#define STATIC_ASSERT(condition) _Static_assert((condition), #condition)
#else
// C89 permits repeated extern declarations of the same array. A false
// condition gives it an invalid size; a true one emits no code or storage.
#define STATIC_ASSERT(condition)                                              \
  extern char alchemyStaticAssertion[(condition) ? 1 : -1]
#endif

#endif
