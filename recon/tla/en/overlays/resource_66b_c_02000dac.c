// Draft, differ 342 of 252: draws the swinging actors' ropes and heads (the 16
// records of gData_020023c4 + 0x188 kept by Func_02001068): for each raised
// actor, a rope segment every 16 pixels of its height and the head at the
// top, bobbing, each an OAM entry at the table's OAM cursor (four frames of
// animation a 12-frame cycle), at most 80 sprites, drawn by
// Func_0200119c. The same listing as overlay 661's Func_02003b80 and 697's
// Func_02003f68. Left: a rough reading: the native keeps the record cursor, the counts, the tile and the scroll on the stack (r8 to r11 hold the OAM cursor, the actor and the segment height), and computes the positions in another order; the alignment is poor.
#include "types.h"
#include "actor.h"
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

extern u8 gData_020023c4[];
extern u32 gFrameCount;
struct Actor *Actor_GetById(u32 id);
s32 Math_Sine(s32 angle);
void Func_0200119c(u32 *oam, s32 depth);

void Func_02000dac(void) {
  u8 *table = gData_020023c4 + 0x188;
  struct SwingActor *swing = (struct SwingActor *)(table + 4);
  u32 tile = gResourceTableEntries[*(s16 *)table].offset >> 5;
  u32 *oam = *(u32 **)(table + 0x1c4);
  u8 *map = (u8 *)Ram_HeapSlots->mapWork;
  s32 scrollX = *(s32 *)(map + 0xe4) & 0xffff0000;
  s32 scrollZ = *(s32 *)(map + 0xe8) & 0xffff0000;
  s32 lift = (*(s32 **)map)[1];
  struct Actor *actor;
  s32 count = 0, k, h, frame, x, y, z, screenY, depth;

  for (k = 15; k >= 0; k--, swing++) {
    if (swing->id == 0 || swing->height == 0)
      continue;
    actor = Actor_GetById(swing->id);
    frame = (gFrameCount % 12 / 3) * 8 + 32;
    for (h = 0; h < swing->height; h += 16) {
      if (count > 79)
        continue;
      x = (actor->x - scrollX) >> 16;
      y = actor->y + (h << 16) - lift;
      z = actor->z - scrollZ - lift;
      screenY = ((z - y) >> 16) - 16;
      depth = ((y + z) >> 16) + 58;
      if ((u32)(x + 7) > 334 || screenY <= -16 || screenY > 239)
        continue;
      oam[0] = 0;
      oam[1] = (screenY & 255) | (((x - 8) & 0x1ff) << 16) | 0x40002000;
      oam[2] = (tile + frame) | 0x800;
      Func_0200119c(oam, depth);
      count++;
      oam += 3;
    }
    if (count > 79)
      continue;
    x = (actor->x - scrollX) >> 16;
    y = actor->y + (swing->height << 16) - lift;
    z = actor->z - scrollZ - lift;
    screenY = ((z - y + Iwram_MulQ16(-0x20000, Math_Sine(swing->bob)) +
                -0x20000) >>
               16) -
              4;
    depth = ((y + z) >> 16) + 58;
    if ((u32)(x + 7) > 334 || screenY <= -16 || screenY > 239)
      continue;
    oam[0] = 0;
    oam[1] = (screenY & 255) | (((x - 8) & 0x1ff) << 16) | 0x40002000;
    oam[2] = (tile + frame - 32) | 0x800;
    Func_0200119c(oam, depth);
    count++;
    oam += 3;
  }
}
