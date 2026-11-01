// Draft, differ 135 of 171, skeleton gap 39 (tla-en, 2026-11-01)
// Func_02002250's twin for the other bubbles of gData_020034f0: each starts
// within 12 pixels of the source object, falls to the ground's height and then
// drifts forward 2 pixels a frame, ending at height -4 or row 50.
// Left: the native reloads bubble->age for oam[2] after storing oam[1]; here
// CSE keeps age + 1 in a register, which takes r7, pushes i to r8 and spills
// tile to the stack, and the rest of the gap follows from that (i--, the 511
// built from 334 by adding 177). A pointer oam = bubble->oam gives the reload
// but breaks the direct stores; *(oam + n), casts and ++age do not. The clip
// compares screenY with -16 by ble; every spelling here comes out as blt -15.
// The loop is label and goto because a for loop hoists the 0 and other
// constants that the native loads inside the loop (skeleton 20/31 as for).
#include "types.h"
#include "actor.h"
#include "map.h"
#include "ram_buffer.h"
#include "graphics/vram_tab.h"

struct Bubble {
  s32 x;
  s32 y;
  s32 z;
  s32 age;
  u32 oam[3];
};

struct BubbleWork {
  struct Actor *source;
  u16 block;
  u16 next;
  u8 unk08[4];
  struct Bubble bubbles[64];
};

extern struct BubbleWork gData_020034f0;
u32 Random16Far(void);
s32 Map_GetTerrainHeight(s32 terrain, s32 x, s32 z);
void Func_020028d0(u32 *oam, s32 depth);

void Func_020023c0(void) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  s32 *view = &map->viewX;
  s32 viewX = view[0] & 0xffff0000;
  s32 viewZ = view[1] & 0xffff0000;
  s32 lift = map->origin[1];
  struct BubbleWork *work = &gData_020034f0;
  u32 tile = gResourceTableEntries[work->block].offset >> 5;
  struct Actor *source;
  struct Bubble *bubble;
  s32 i, x, y, z, screenX, screenY, left, depth;

  if (Random16Far() < 0x1000 && (source = work->source) != 0) {
    bubble = &work->bubbles[work->next];
    bubble->x = source->x + Random16Far() * 3 * 8 - 0xc0000;
    bubble->y = source->y;
    bubble->z = source->z;
    bubble->age = 0;
    if (++work->next > 63)
      work->next = 0;
  }
  bubble = work->bubbles;
  i = 63;
loop:
  {
    if (bubble->x != 0) {
      if (Map_GetTerrainHeight(0, bubble->x, bubble->z) < bubble->y)
        bubble->y += -0x20000;
      else
        bubble->z += 0x20000;
      if (bubble->y >> 20 == -4)
        bubble->x = 0;
      if (bubble->z >> 20 == 50)
        bubble->x = 0;
      bubble->age++;
      x = bubble->x - viewX;
      z = bubble->z - viewZ - lift;
      y = bubble->y - lift;
      screenX = x >> 16;
      left = screenX - 4;
      screenY = ((z - y) >> 16) - 4;
      depth = ((y + z) >> 16) + 58;
      if (screenX < -11 || screenX > 323 || screenY <= -16 || screenY > 239)
        goto next;
      left &= 0x1ff;
      screenY &= 255;
      bubble->oam[0] = 0;
      bubble->oam[1] = screenY | (left << 16) | 0x2000;
      bubble->oam[2] = (tile + (bubble->age & 3) * 2) | 0x400;
      Func_020028d0(bubble->oam, depth);
    }
  }
next:
  i--;
  bubble++;
  if (i >= 0)
    goto loop;
}
