// Draft, differ 227 of 260, skeleton gap 93 (tla-en, 2026-11-01)
// Overlay 6b1's Func_02000858, the animated, flippable form of overlay 698's
// Func_02000aac (resource_698_c_02000aac.c): queues a sprite per actor in
// gData_02001ce0, its picture stepping every 4 frames through 5 steps.
// Settled: gcse's PRE no longer carries the pool load of &gData_02001cde round
// the loop (the tail's ldr now sits where the listing has it). PRE needs that
// load available at the loop head; actorCount, set above the __umodsi3 call and
// read only by the entry test, kills it there, and the tail reads the symbol.
// Reading *actorCount at both tests moves PRE onto the count load itself, and
// an unused local changes nothing.
// What is left starts at the priority stores, and it is a compiler question,
// not source. The listing's copy after -13 (movs #13, negs, adds) proves the
// mask stays in the loop: reload loads a QImode -13 into a low register and
// copies it. Here loop.c's second run lifts the 3 and the -13: four sites merge
// into savings 4, life 4 (life 8 for -13, after cse2), and threshold 15 passes
// against 151 real insns. The first run spares them only because the -1 compare
// moves three insns first (threshold 6). Stopping the second run from source
// needs four moves ahead of the first site, or a group split, and nothing plain
// gives either. Their hoisted pseudos take r9 and fp (priority 0.106), so viewX,
// viewZ and depth spill (frame 24 against 12).
// Measured with -fno-rerun-loop-opt in a direct compile, as an experiment only:
// all four sites come out as the listing's instructions, copy included. If the
// native's loop.c hoists less (the landing-burst notes in the facts), the
// question belongs to Pascal (K3).
// Allocation is open either way: the listing wants oam over mode (mode in ip),
// viewZ and viewX over i, depth over viewY; by the lreg numbers oam 0.37 against
// mode 0.67 (oam's constant doubles its live length) and viewX 0.06 against
// i 0.11.
// Tried, no gain: a plain for (differ 226, gap 96; its tail's pool load hoists);
// i < *actorCount in a for (234); do { } while (0) around the stores (229, the
// inner loop is phony); byte masks on the second arm (229, gap 83, wrong form);
// a goto loop (231, no hoist, no copy); mode read at the top of the body (262),
// no mode local (259), an s16 mode (no change); separate mask statements for
// viewX and viewZ (235 to 242); no timer pointer (231); the priority read as s16
// (240).
// Kept because they closed gaps: a loop with notes, not a goto loop (combine
// turns <= -16 into < -15); the range tests on ox and oy written twin-style;
// the priority read as a u16 and tested as an s16, in each arm. Declaring
// picture, tile, viewY in the listing's slot order changes nothing.
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
  s16 *actorCount = &gData_02001cde;
  s32 picture = gData_02001de4 * ((gFrameCount >> 2) % 5 * 32 + 32);
  s32 i;

  i = 0;
  if (i < *actorCount) do {
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
  } while (++i < gData_02001cde);
}
