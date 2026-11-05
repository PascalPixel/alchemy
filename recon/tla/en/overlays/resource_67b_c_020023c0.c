// Measured under -Os, 5 November 2026: differ 32 (tla-en); notes below may predate it.
// Draft, differ 46 of 171, skeleton gap 8 (tla-en, 2026-11-01)
// Func_02002250's twin for the other bubbles of gData_020034f0: each starts
// within 12 pixels of the source object, falls to the ground's height and then
// drifts forward 2 pixels a frame, ending at height -4 or row 50.
// Left, three things:
// 1. The clip compares screenY with -16 by ble; this label loop gets blt -15.
// Only a real loop keeps -16: loop.c puts the single-use constant into the jump
// before combine, and here combine sees the pseudo and canonicalizes it. But a
// real for loop hoists the three zeros (two bubble->x = 0, oam[0] = 0), which
// the native rebuilds each time: loop.c moves them when 14 * savings 3 * life 3
// = 126 >= real insns, and this loop has 81 (measured with filler: 121 hoists,
// 141 does not). A for loop with `word` set twice as in Func_02002250 stops the
// match and reaches differ 48, gap 3, but that is a reused local (a device), so
// it is not kept. Whether the native's threshold was lower is a compiler
// question, out of scope for a draft.
// 2. Allocation: native viewZ r8, viewX r10, lift r9; here lift r8, viewZ r10,
// viewX r9. All carry 5 refs, so live length ranks them (lift 117, viewZ 121,
// viewX 124); lift would need the longest, but setting it before the masks
// breaks the prologue (differ 74 to 86). The reload and z load order around
// the projection follow from this.
// 3. screenX r2 and screenY r0 are the listing's the other way round: local
// allocation gives screenX r2 before global takes screenY. Declaration order
// changes nothing; eight shapes of the screenY and depth sums lost ground.
// `oam` holds the one address for its store, which makes CSE reload age as the
// native does; every use through it (oam[0] to oam[2]) gives differ 193.
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
  s32 i, x, y, z, screenX, screenY;
  u32 *oam;

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
do {
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
    z = bubble->z;
    y = bubble->y - lift;
    z -= viewZ + lift;
    screenY = (z - y) >> 16;
    y += z;
    y >>= 16;
    screenX = x >> 16;
    x = screenX - 4;
    screenY -= 4;
    z = y + 58;
    if ((u32)(screenX + 11) <= 334 && screenY > -16 && screenY <= 239) {
      x &= 0x1ff;
      screenY &= 255;
      bubble->oam[0] = 0;
      oam = &bubble->oam[1];
      *oam = screenY | (x << 16) | 0x2000;
      bubble->oam[2] = (tile + (bubble->age & 3) * 2) | 0x400;
      Func_020028d0(bubble->oam, z);
    }
  }
  i--;
  bubble++;
} while (i >= 0);
}
