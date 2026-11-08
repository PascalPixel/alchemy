// Matches: differ 0 (tla-en, 2026-11-08)
// All six editions measure 0 (tla-en, ja, de, es, fr, it), no fake device.
// Aqua Rock's rising bubbles (gData_020034f0, 64 entries of 28 bytes after a
// 12-byte head): now and then a bubble starts near the source object; each
// live bubble rises until it reaches the ground's height, then drifts back 2
// pixels a frame, ends at height 23 or row 48, and is drawn as one of four
// animation pictures where it is on screen (Func_020028d0).
// Forms that are load-bearing, all ordinary C under S2:
// - screenX first holds the ground's height (a local reused for a second
//   value). That makes it live in two blocks, so global-alloc rather than
//   local-alloc places it, and by priority (screenY 14 refs over 28 insns,
//   z 9 over 35) screenY, z and screenX take the listing's r2, r1 and r0.
//   Without the reuse screenX was local, took r2 first and left screenY r0,
//   which moved the str of oam[1] (differ 30).
// - screenX += 11 in place before the range test: with screenX global, a
//   separate screenX + 11 took its own register and 334 another (41).
// - the volatile age: the listing reads age again after the OAM stores
//   (37 without it).
// - the for loop, for the -16 compare.
// Before (differ 30): oam temporaries, unsigned screenX, the range test as
// two compares, screenX computed late or from x >>= 16, temporaries for
// z - y, once-only blocks and permutes of up to 14,000 candidates all left
// the r2/r0 swap.
#include "types.h"
#include "call.h"
#include "actor.h"
#include "map.h"
#include "ram_buffer.h"
#include "graphics/vram_tab.h"

struct Bubble {
  s32 x;
  s32 y;
  s32 z;
  // volatile: the listing reads age again after the OAM stores.
  volatile s32 age;
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
      // screenX holds the ground's height here, before its own value.
      screenX = Map_GetTerrainHeight(0, bubble->x, bubble->z);
      if (screenX > bubble->y)
        bubble->y += 0x20000;
      else
        bubble->z += -0x20000;
      if (bubble->y >> 20 == 23)
        bubble->x = 0;
      if (bubble->z >> 20 == 48)
        bubble->x = 0;
      bubble->age++;
      x = bubble->x - viewX;
      y = bubble->y;
      z = bubble->z;
      y -= lift;
      z -= viewZ;
      z -= lift;
      screenY = z - y;
      y += z;
      y >>= 16;
      screenX = x >> 16;
      x = screenX - 4;
      screenY >>= 16;
      screenY -= 4;
      z = y + 58;
      // screenX now holds its offset into the range test (on screen from
      // -11 to 323).
      screenX += 11;
      if ((u32)screenX <= 334 && screenY > -16 && screenY <= 239) {
        x &= 0x1ff;
        screenY &= 255;
        bubble->oam[0] = 0;
        bubble->oam[1] = screenY | (x << 16) | 0x2000;
        bubble->oam[2] = (tile + (bubble->age & 3) * 2) | 0x400;
        Func_020028d0(bubble->oam, z);
      }
    }
  }
}
