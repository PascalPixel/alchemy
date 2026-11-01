// Draft, differ 175 of 200, skeleton gap 49 (tla-en, 2026-11-01)
// Overlay 6ad's Func_02005048, the twin of overlay 69d's Func_02000e58.
// What is left is allocation and the moves it forces, no structural gap:
// - mode must end in ip and frame in r7; mode's priority here beats actor,
//   oam and frame, so mode takes r5, actor r6, oam r7, frame ip. Moving its
//   load earlier in the block did not help.
// - gcse's PRE carries &gData_020074ce from the loop head to the continue
//   paths in one pseudo (in lr: 3 extra moves). The listing reloads it locally
//   at both places. A do/while, nested ifs and a volatile count all failed.
// - Listing: camZ r8, camX r10, i r9, depth r11, originY [sp], tile [sp].
//   Here i, depth and originY outrank both camera values.
// Kept because it closed gaps: ox and oy hold the unshifted values first; the
// masks are separate statements, ox first; timer is taken before the branch.
// map->viewX and viewZ (offset 228) in place of camera[] measure worse.
#include "types.h"
#include "ram_buffer.h"
#include "map.h"
#include "field_event.h"
#include "vram_tab.h"

struct LightOam {
  u32 link;
  u32 attr01;
  u16 tile : 10;
  u16 priority : 2;
  u16 palette : 4;
  u16 unk0a;
};

extern s16 gData_020074cc;
extern s16 gData_020074ce;
extern struct FieldActor *gData_020074d0[];
extern struct LightOam Data_0200f510[];
extern s16 gData_020075d0;
extern s16 gData_020075d2;

s32 Func_02005698(struct FieldActor *actor);
void Func_02005440(struct LightOam *oam, s32 depth);

#define MAP_WORD(map, offset) (*(s32 *)((u8 *)(map) + (offset)))

void Func_02005048(void) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  s32 *camera = (s32 *)((u8 *)map + 228);
  s32 cameraX = camera[0] & 0xffff0000;
  s32 cameraZ = camera[1] & 0xffff0000;
  s32 tile = gResourceTableEntries[gData_020074cc].offset >> 5;
  struct LightOam *oam = Data_0200f510;
  s32 originY = map->origin[1];
  s32 i;

  for (i = 0; i < gData_020074ce; i++) {
    struct FieldActor *actor = gData_020074d0[i];
    s32 y, z, depth, frame, sx, sy, ox, oy, mode;
    u16 *timer;
    u32 *word;

    if (actor == 0 || actor->x == 0)
      continue;
    ox = actor->x - cameraX;
    y = actor->y - originY + 0x80000;
    z = actor->z - cameraZ - originY;
    oy = z - y;
    depth = ((y + z) >> 16) + 58;
    mode = gData_020075d0;
    timer = &actor->timer;
    sx = ox >> 16;
    sy = oy >> 16;
    if (mode == 0) {
      frame = (s16)*timer;
      ox = sx - 8;
      oy = sy - 16;
      if (ox <= -16 || ox >= 320 || oy <= -16 || oy >= 240)
        continue;
      ox &= 0x1ff;
      word = (u32 *)oam;
      oy &= 0xff;
      *word++ = 0;
      *word++ = oy | (ox << 16) | 0x40002000;
    } else {
      frame = (s16)*timer;
      ox = sx - 8;
      oy = sy - 64;
      if (ox <= -32 || ox >= 320 || oy <= -64 || oy >= 176)
        continue;
      ox &= 0x1ff;
      word = (u32 *)oam;
      oy &= 0xff;
      *word++ = 0;
      *word++ = oy | (ox << 16) | 0xc000a000;
    }
    *word = (tile + frame * 8) | 0x800;
    if (gData_020075d2 == -1)
      oam->priority = Func_02005698(actor);
    else
      oam->priority = gData_020075d2;
    Func_02005440(oam, depth);
    oam++;
  }
}
