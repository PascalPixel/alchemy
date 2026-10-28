#ifndef TLA_ITEM_ICON_H
#define TLA_ITEM_ICON_H

#include "types.h"
#include "ram_buffer.h"
#include "task.h"
#include "vram_tab.h"

#define ICON_COUNT 15 ///< Number of carried items.
#define ICON_NONE 96  ///< No resource.

struct IconAttr {
  u8 y;
  u8 attr0High;
  u16 x : 9;
  u16 attr1High : 7;
  u16 tile : 10;
  u16 attr2High : 6;
  u16 unk06;
};

struct IconSprite {
  u32 link;
  struct IconAttr attr;
};

struct IconWork {
  u8 unk000[0xc14];
  struct IconSprite sprites[ICON_COUNT];
  u16 resources[ICON_COUNT];
  s8 active;
};

s32 Resource_ResetEntry(u32 entry);
s32 Resource_FindFreeEntry(void);
void Icon_CopyResourceToSlotFar(s32 item, s32 mode, s32 resource);
void *Unit_Get(s32 owner);

void Func_0810bdb0(void);
void Func_0810be70(s32 index, s32 x, s32 y);
void Func_0810bea8(s32 active);
void Func_0810bebc(u16 *items, s32 count, s32 mode);

#endif
