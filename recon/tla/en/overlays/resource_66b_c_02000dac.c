// Draft, differ 190 of 252, skeleton gap 42 (tla-en, 2026-11-01): 23 listing
// instructions missing, 19 extra, no device.
// Draws the swinging actors' ropes and heads (the 16 records of
// gData_020023c4 + 0x188 kept by Func_02001068): for each raised actor, a rope
// segment every 16 pixels of its height and the bobbing head on top, each an
// OAM entry at the table's OAM cursor (four frames of animation over a
// 12-frame cycle), at most 80 sprites, queued by Func_0200119c. The same
// listing as overlay 661's Func_02003b80 and 697's Func_02003f68.
// Left, cause proved: PRE (.07.gcse: "redundant insn" for the latch's and the
// head's read of swing->height) merges them with the read before the loop, so
// the native's ldrsh at each becomes one ldrh and the allocation follows. The
// native keeps them apart because each read's address is a different pseudo.
// Probe, not kept: `(void)&swing;` after the declarations. A taken address
// makes swing an ADDRESSOF, which cse1 leaves as a MEM and purge_addressof
// turns into a copy per site, too late for CSE; PRE then has nothing to merge.
// Every height read matches, and the prologue with it: gap 31 (17 and 14).
// S2 counts a taken address as `dead`, so it stays out in a no-new-device
// round; whether it is the source's form is for Pascal to decide, and it
// reaches 661 and 697. No real statement that takes swing's address is known.
// With the probe, moving `x = (x >> 16) - 8;` straight after the head's
// `x = actor->x - scrollX;` and giving the head's first `z - y` its own
// variable (delta) reaches gap 20 (10 and 10). The native's r10 there is
// x >> 16 crossing the two calls, which sched2 then slides below them, so that
// move is probably real source; alone it gives gap 41 but differ 271, so it is
// not applied here. Still open under the probe: y takes r4 by caller-save
// (15 refs, 2 calls crossed), where the native has y r8, actor r10, h r11 and
// lift on the stack; and -0x20000 is held across the call by caller-save where
// the native reloads it from the pool (sp+0 may be that unused save slot).
// sp+32 above k says swing is a declared variable, not a loop-made giv.
// Tried, no better: indexing work->actors[k], or a local actors[k], everywhere:
// all reads are ldrsh, but the giv is a late pseudo, set after the preheader,
// and y still goes to r4 (gap 78); `swing = &work->actors[k]` in the body
// (gap 72, PRE still merges); y split by region (loop y goes low, gap 91 on
// the indexed form); y, z and delta all split in the head (gap 70); a real
// for loop (gap 64). Declaration order of actor, h, x, y, z and screenY changes
// the pseudos but not the code, in all 24 orders of each of two groups tried.
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
