// Draft, differ 50 of 169, skeleton gap 3 (tla-en, 2026-11-01)
// Aqua Rock's rising bubbles (gData_020034f0, 64 entries of 28 bytes after a
// 12-byte head): now and then a bubble starts near the source object; each
// live bubble rises until it reaches the ground's height, then drifts back 2
// pixels a frame, ends at height 23 or row 48, and is drawn as one of four
// animation pictures where it is on screen (Func_020028d0).
// Left: allocation only. viewX, viewZ and lift rank by live length, giving
// lift r8, viewZ r10, viewX r9 where the native has viewZ r8, viewX r10, lift
// r9; all three carry 5 refs, so lift would have to be set before the masks,
// and writing it so breaks the prologue. Hard-register dependences then move
// the ldr of y, one asrs and the str of oam[1] in sched2 (the whole gap), and
// screenX and screenY take r2 and r0 the wrong way round.
// Forms that are load-bearing: `word` set twice keeps the third zero of the
// loop from combining with the two `bubble->x = 0` (three would be hoisted,
// take a saved register and push tile to the stack, and the -16 compare would
// fold differently); the store through `oam` makes CSE reload age; the
// for loop is needed for that same -16.
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

void Func_02002250(void) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  s32 *view = &map->viewX;
  s32 viewX = view[0] & 0xffff0000;
  s32 viewZ = view[1] & 0xffff0000;
  s32 lift = map->origin[1];
  struct BubbleWork *work = &gData_020034f0;
  u32 tile = gResourceTableEntries[work->block].offset >> 5;
  struct Actor *source;
  struct Bubble *bubble;
  s32 i, x, y, z, screenX, screenY;
  u32 word;
  u32 *oam;

  if (Random16Far() < 0x1000 && (source = work->source) != 0) {
    bubble = &work->bubbles[work->next];
    bubble->x = source->x + Random16Far() * 3 * 16 - 0x180000;
    bubble->y = source->y;
    bubble->z = source->z;
    bubble->age = 0;
    if (++work->next > 63)
      work->next = 0;
  }
  bubble = work->bubbles;
  for (i = 63; i >= 0; i--, bubble++) {
    if (bubble->x != 0) {
      if (Map_GetTerrainHeight(0, bubble->x, bubble->z) > bubble->y)
        bubble->y += 0x20000;
      else
        bubble->z += -0x20000;
      if (bubble->y >> 20 == 23)
        bubble->x = 0;
      if (bubble->z >> 20 == 48)
        bubble->x = 0;
      bubble->age++;
      x = bubble->x - viewX;
      z = bubble->z - viewZ - lift;
      y = bubble->y - lift;
      screenY = z - y;
      y += z;
      y >>= 16;
      screenX = x >> 16;
      x = screenX - 4;
      screenY >>= 16;
      screenY -= 4;
      z = y + 58;
      if ((u32)(screenX + 11) <= 334 && screenY > -16 && screenY <= 239) {
        x &= 0x1ff;
        screenY &= 255;
        word = 0;
        bubble->oam[0] = word;
        word = screenY | (x << 16) | 0x2000;
        oam = &bubble->oam[1];
        *oam = word;
        bubble->oam[2] = (tile + (bubble->age & 3) * 2) | 0x400;
        Func_020028d0(bubble->oam, z);
      }
    }
  }
}
