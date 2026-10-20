#ifndef ALCHEMY_B5_CONTEXT_H
#define ALCHEMY_B5_CONTEXT_H

/// @file
/// B5Context, an object context layout that no code in the tree reads.

#include "edition.h"
#include "types.h"
#include "static_assert.h"

/// An object context layout that many private headers and scenes include but
/// no code reads, so nothing in the tree supports its field names.
struct B5Context {
  void *object;
  u8 padding04[8];
  s32 word0c;
  s32 word10;
  u8 padding14[0x0c];
  void *excluded20;
  void *excluded24;
  u8 padding28[2];
  s16 suppressUpdates;
};

STATIC_ASSERT(offsetof(struct B5Context, object) == 0x00);
STATIC_ASSERT(offsetof(struct B5Context, word0c) == 0x0c);
STATIC_ASSERT(offsetof(struct B5Context, word10) == 0x10);
STATIC_ASSERT(offsetof(struct B5Context, suppressUpdates) == 0x2a);

#endif
