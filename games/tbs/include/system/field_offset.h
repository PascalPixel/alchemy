#ifndef ALCHEMY_FIELD_OFFSET_H
#define ALCHEMY_FIELD_OFFSET_H

/// @file
/// FIELD_AT_OFFSET and two placeholder views, Ent and Cam; no code in the tree
/// uses any of them.

#include "types.h"
#include "edition.h"
#include "farcall.h"

/// The object of pointer type `type` at byte `offset` from `base`.
#define FIELD_AT_OFFSET(base, type, offset) (*(type)((u8 *)(base) + (offset)))

/// A placeholder layout no code in the tree uses.
struct Ent {
  s32 f00;
  u8 f04;
  u8 f05;
  u16 f06 : 9;
  u16 f07 : 7;
  s32 f08;
};

/// A placeholder layout no code in the tree uses.
struct Cam {
  u8 pad[228];
  s32 x;
  s32 y;
};

#endif
