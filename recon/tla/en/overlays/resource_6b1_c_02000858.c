// Draft, differ 226 of 260, skeleton gap 96 (tla-en, 2026-11-01)
// Overlay 6b1's Func_02000858, the animated, flippable form of overlay 698's
// Func_02000aac (resource_698_c_02000aac.c): queues a sprite per actor in
// gData_02001ce0, its picture stepping every 4 frames through 5 steps.
// What is left is allocation and the moves and spills it forces:
// - gcse's PRE carries the pool load of &gData_02001cde from the loop head to
//   the continue paths in one pseudo, which takes ip (a pool load is memory, so
//   a store or call kills it and PRE reinserts it after the call). mode then
//   takes r7 instead of ip, oam r8; 3 and -13, hoisted from the four priority
//   stores, outrank viewX, viewZ and depth, which spill (frame 24 against 12).
//   In the listing every hoisted constant lost its register and reload put it
//   back at its use: that is why -13 shows a copy there. Getting rid of that
//   PRE should settle the rest. A do/while and a while do not; the same
//   pseudo is in overlay 6ad's header.
// Kept because they closed gaps: a for loop, not a goto loop (no loop notes
// means no hoisting, so combine turns <= -16 into < -15 and the ranges differ);
// the range tests on ox and oy written twin-style; the priority read as a u16
// and tested as an s16, in each arm (the listing keeps both loads and 4 arms).
// Reordering or rescoping locals changes nothing; separate mask statements for
// viewX and viewZ, read first, cost 2 more.
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

extern s16 gData_02001cdc;
extern s16 gData_02001cde;
extern struct FieldActor *gData_02001ce0[];
extern struct LightOam gData_02001d20[];
extern s16 gData_02001de0;
extern s16 gData_02001de2;
extern s16 gData_02001de4;
extern u32 gFrameCount;

s32 Func_02000d5c(struct FieldActor *actor);
void Func_02000c3c(struct LightOam *oam, s32 depth);

void Func_02000858(void) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  s32 *view = &map->viewX;
  s32 viewX = view[0] & 0xffff0000;
  s32 viewZ = view[1] & 0xffff0000;
  s32 tile = gResourceTableEntries[gData_02001cdc].offset >> 5;
  s32 viewY = map->origin[1];
  struct LightOam *oam = gData_02001d20;
  s32 picture = gData_02001de4 * ((gFrameCount >> 2) % 5 * 32 + 32);
  s32 i;

  for (i = 0; i < gData_02001cde; i++) {
    struct FieldActor *actor = gData_02001ce0[i];
    s32 ox, y, z, oy, sx, sy, depth, frame, mode, priority;
    u16 *timer;
    u32 *word;

    if (actor == 0 || actor->x == 0)
      continue;
    ox = actor->x - viewX;
    y = actor->y - viewY + 0x80000;
    z = actor->z - viewZ - viewY;
    oy = z - y;
    depth = ((y + z) >> 16) + 58;
    mode = gData_02001de0;
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
      *word++ = oy | (ox << 16) | (frame & 0x80) << 21 | 0x40002000;
      *word = (tile + picture + (frame & 7) * 8) | 0x800;
      priority = *(u16 *)&gData_02001de2;
      if (gData_02001de2 == -1)
        oam->priority = Func_02000d5c(actor);
      else
        oam->priority = priority;
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
      *word = (tile + frame * 8) | 0x800;
      priority = *(u16 *)&gData_02001de2;
      if (gData_02001de2 == -1)
        oam->priority = Func_02000d5c(actor);
      else
        oam->priority = priority;
    }
    Func_02000c3c(oam, depth);
    oam++;
  }
}
