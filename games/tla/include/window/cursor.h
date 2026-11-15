#ifndef ALCHEMY_CURSOR_H
#define ALCHEMY_CURSOR_H

/// @file
/// The menu cursor: a sprite that slides to its target beside a window.

#include "types.h"

struct WindowSprite;

struct MenuCursor {
  struct WindowSprite *anchor;
  u16 x;
  u16 y;
  s16 targetX;
  s16 targetY;
  s8 active;
  s8 kind;
  u8 padding[2]; ///< to the struct's word size
};

#endif
