#ifndef ALCHEMY_RESOURCE_MENU_H
#define ALCHEMY_RESOURCE_MENU_H

/// @file
/// The resource menu's entries and work.

#include "types.h"
#include "static_assert.h"
#include "window.h"

struct ResourceMenuEntry {
  u32 link;
  u32 attr01;
  u32 attr2;
  s16 x;
  s16 y;
  u16 unk10;
  u16 slot;
};

struct ResourceMenuWork {
  struct ResourceMenuEntry entries[6];
  struct UiWindow *window;
  struct UiWindow *lowerWindow;
  struct UiWindow *upperWindow;
  u8 resourceIds[8];
  s16 selection;
  s16 count;
  s16 width;
  s16 resourceBase;
  s16 row;
  s16 previousMode;
};

STATIC_ASSERT(sizeof(struct ResourceMenuEntry) == 0x14);
STATIC_ASSERT(offsetof(struct ResourceMenuEntry, slot) == 0x12);
STATIC_ASSERT(sizeof(struct ResourceMenuWork) == 0x98);
STATIC_ASSERT(offsetof(struct ResourceMenuWork, window) == 0x78);
STATIC_ASSERT(offsetof(struct ResourceMenuWork, selection) == 0x8c);
STATIC_ASSERT(offsetof(struct ResourceMenuWork, resourceBase) == 0x92);
STATIC_ASSERT(offsetof(struct ResourceMenuWork, previousMode) == 0x96);

extern struct ResourceMenuWork *gMenuSelectWork;

#endif
