#include "types.h"
#include "call.h"
#include "actor.h"
#include "map.h"
#include "ram_buffer.h"
#include "iwram_call.h"
#include "graphics/vram_tab.h"

struct SwingActor {
  s16 id;
  s16 phase;
  s16 height;
  s16 period;
  s16 riseAt;
  s16 sinkAt;
  s16 top;
  s16 bottom;
  s16 unk10;
  s16 speed;
  s16 unk14;
  s16 bob;
  struct Actor *shadow;
};

struct SwingWork {
  s16 entry;
  s16 value;
  struct SwingActor actors[16];
  u32 *oam;
};

extern u8 gData_020023c4[];
extern u32 gFrameCount;
struct Actor *Actor_GetById(u32 id);
s32 Math_Sine(s32 angle);
void Func_0200119c(u32 *oam, s32 depth);

// Draws the swinging actors' ropes and heads (the 16 records of gData_020023c4
// + 0x188 kept by Func_02001068): for each raised actor, a rope segment every
// 16 pixels of its height and the bobbing head on top, at most 80 sprites,
// queued by Func_0200119c.
void Func_02000dac(void) {
  struct SwingWork *work = (struct SwingWork *)(gData_020023c4 + 0x188);
  struct SwingActor *swing = work->actors;
  s32 k;
  u32 tile = gResourceTableEntries[work->entry].offset >> 5;
  s32 frame;
  u32 *oam = work->oam;
  s32 count = 0;
  struct MapWork *map = Ram_HeapSlots->mapWork;
  s32 *view = &map->viewX;
  s32 scrollX, scrollZ, lift;
  struct Actor *actor;
  s32 h, x, y, screenY, z, depth;
  u32 *p;

  scrollX = view[0];
  scrollX &= 0xffff0000;
  scrollZ = view[1] & 0xffff0000;
  lift = map->origin[1];
  for (k = 15; k >= 0; k--, swing++) {
    if (swing->id == 0 || swing->height == 0)
      continue;
    actor = Actor_GetById(swing->id);
    frame = (gFrameCount % 12) / 3 * 8 + 32;
    // A goto loop: the native moves nothing out of it; as a for loop, loop.c
    // hoists the sprite's third word (skeleton gap 64).
    h = 0;
    if (h >= swing->height)
      goto top;
  again:
    if (count > 79)
      goto next;
    x = actor->x - scrollX;
    y = actor->y + (h << 16) - lift;
    z = actor->z - scrollZ - lift;
    x = (x >> 16) - 8;
    screenY = z - y;
    screenY = (screenY >> 16) - 16;
    depth = ((y + z) >> 16) + 58;
    if ((u32)(x + 15) > 334 || screenY <= -16 || screenY > 239)
      goto next;
    x &= 511;
    screenY &= 255;
    p = oam;
    *p++ = 0;
    *p++ = screenY | x << 16 | 0x40002000;
    *p = (tile + frame) | 0x800;
    Func_0200119c(oam, depth);
    oam += 3;
    count++;
  next:
    h += 16;
    if (h < swing->height)
      goto again;
  top:
    if (count > 79)
      continue;
    x = actor->x - scrollX;
    y = actor->y + (swing->height << 16) - lift;
    z = actor->z - scrollZ - lift;
    screenY = z - y;
    x = (x >> 16) - 8;
    /* FAKEMATCH(wrapper): forwarded, so -0x20000 is built for the call and again
     * for the sum; as a direct call, 276 of 252 instructions differ in tla-en. */
    screenY = ((screenY + Value2((s32 (*)())Iwram_MulQ16, -0x20000, Math_Sine(swing->bob)) + -0x20000) >> 16) - 4;
    depth = ((y + z) >> 16) + 58;
    if ((u32)(x + 15) > 334 || screenY <= -16 || screenY > 239)
      continue;
    x &= 511;
    screenY &= 255;
    p = oam;
    *p++ = 0;
    *p++ = screenY | x << 16 | 0x40002000;
    *p = (tile + frame - 32) | 0x800;
    Func_0200119c(oam, depth);
    oam += 3;
    count++;
  }
}
