// Draft, differ 190 of 252, skeleton gap 26 (tla-en, 2026-11-01)
// Draws the swinging actors' ropes and heads (the 16 records of
// gData_020023c4 + 0x188 kept by Func_02001068): for each raised actor, a rope
// segment every 16 pixels of its height and the bobbing head on top, each an
// OAM entry at the table's OAM cursor (four frames of animation over a
// 12-frame cycle), at most 80 sprites, queued by Func_0200119c. The same
// listing as overlay 661's Func_02003b80 and 697's Func_02003f68.
// Left: GCSE merges the reads of swing->height across the inner loop: the load
// after the sprite's call becomes a zero-extended ldrh, and the loop test and
// the head's read use that register, where the native rereads with ldrsh at
// all of them. Every other block is close; the rest of the gap follows from
// that register: lift stays in r11 and the constant -0x20000 gets a stack slot
// (the native keeps lift on the stack, and loads the constant from the pool
// twice), the actor r8, h r10 (native: h r11, actor r10, y r8, x r7, z r5).
// Tried: volatile height does not help (it splits the load, gap 33): the
// native's reads are plain loads. A bitfield, for, while and do loops, the
// constant in a local, x in a second variable, inline masks and other
// statement orders in the sprite are all no better. Any full redundancy
// between the loop's tests and the head's read has no store or call on its
// path, so only a change in the address or in the pass can leave them apart;
// neither reached in plain C.
#include "types.h"
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
  s32 h, x, y, z, screenY, depth;
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
    // hoists the sprite's third word (skeleton gap 36).
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
    count++;
    oam += 3;
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
    screenY = ((screenY + Iwram_MulQ16(-0x20000, Math_Sine(swing->bob)) +
                -0x20000) >>
               16) -
              4;
    depth = ((y + z) >> 16) + 58;
    x = (x >> 16) - 8;
    if ((u32)(x + 15) > 334 || screenY <= -16 || screenY > 239)
      continue;
    x &= 511;
    screenY &= 255;
    p = oam;
    *p++ = 0;
    *p++ = screenY | x << 16 | 0x40002000;
    *p = (tile + frame - 32) | 0x800;
    Func_0200119c(oam, depth);
    count++;
    oam += 3;
  }
}
