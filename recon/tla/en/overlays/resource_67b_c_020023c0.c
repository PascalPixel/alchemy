// Draft, differ 158 of 171: Func_02002250's twin for the other bubbles of
// gData_020034f0: each starts within 12 pixels of the source object, falls
// to the ground's height and then drifts forward 2 pixels a frame, ending at
// height -4 or row 50. Left: as Func_02002250's draft.
#include "types.h"
#include "object_runtime.h"
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
  struct ObjectRuntime *source;
  u16 block;
  u16 next;
  u8 unk08[4];
  struct Bubble bubbles[64];
};

extern struct BubbleWork gData_020034f0;
u16 Random16Far(void);
s32 Map_GetTerrainHeight(s32 terrain, s32 x, s32 z);
void Func_020028d0(u32 *oam, s32 depth);

void Func_020023c0(void) {
  u8 *map = (u8 *)Ram_HeapSlots->mapWork;
  s32 scrollX = *(s32 *)(map + 0xe4) & 0xffff0000;
  s32 scrollZ = *(s32 *)(map + 0xe8) & 0xffff0000;
  s32 lift = (*(s32 **)map)[1];
  struct BubbleWork *work = &gData_020034f0;
  u32 tile = gResourceTableEntries[work->block].offset >> 5;
  struct ObjectRuntime *source;
  struct Bubble *bubble;
  s32 i, x, y, z, screenX, screenY, depth;

  if (Random16Far() < 0x1000 && (source = work->source) != 0) {
    bubble = &work->bubbles[work->next];
    bubble->x = source->x + Random16Far() * 3 * 8 - 0xc0000;
    bubble->y = source->y;
    bubble->age = 0;
    bubble->z = source->z;
    if (++work->next > 63)
      work->next = 0;
  }
  bubble = work->bubbles;
  for (i = 63; i >= 0; i--, bubble++) {
    if (bubble->x == 0)
      continue;
    if (Map_GetTerrainHeight(0, bubble->x, bubble->z) < bubble->y)
      bubble->y += -0x20000;
    else
      bubble->z += 0x20000;
    if (bubble->y >> 20 == -4)
      bubble->x = 0;
    if (bubble->z >> 20 == 50)
      bubble->x = 0;
    bubble->age++;
    x = bubble->x - scrollX;
    z = bubble->z - scrollZ - lift;
    y = bubble->y - lift;
    screenX = x >> 16;
    screenY = (z - y) >> 16;
    depth = ((y + z) >> 16) + 58;
    if ((u32)(screenX + 11) > 334)
      continue;
    screenY -= 4;
    if (screenY <= -16 || screenY > 239)
      continue;
    bubble->oam[0] = 0;
    bubble->oam[1] = (screenY & 255) | (((screenX - 4) & 0x1ff) << 16) | 0x2000;
    bubble->oam[2] = (tile + (bubble->age & 3) * 2) | 0x400;
    Func_020028d0(bubble->oam, depth);
  }
}
