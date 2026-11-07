// Differ 49 of 160 (tla-en, 2026-11-07).
// 2026-11-07: kind as u32 stepped through u16 casts (kind = (u16)i; kind = (u16)(kind + 1); i = (s16)kind) puts
// the search's u16 view in r2 before i's asrs, as native; the loop now matches. Left: i = 0 before sprite fixes the
// prologue but moves the 0x10000 reload to r1 (55); the tail's native order x, z, ax, az, bx, bz gets the constants
// right (328, 332, 384, 388) but kind * 4 (5 refs over 17 insns) loses to the second offset (5 over 15) and takes
// r1 (84); on this head all 720 orders with w and h first and 16,000 of the rest were measured: none beats this.
// Earlier (61 of 160, 2026-11-06):
// 2026-11-06 (later): s16 i = 0 declared before sprite gives the listing's prologue (i's zero in r1 set first,
// the value28 read on a zero of its own in r2), but 67 overall: in the search the step still shifts i (asrs r1)
// before the u16 view (lsrs), which then ties to the step's register r3 where the listing has the view first in r2.
// On that base kind = i + 1; i = kind and i++; kind = i give 69; i = kind = i + 1 gives 73 on this draft; a
// once-only block around i = 0 gives 63.
// Native order with az and bx, bz read raw: differ 94; permuter 8 min: no gain.
// Near miss: the search's u16 view lands in
// r3, not r2, so kind and kind * 4 are two registers; the second layer's first
// word is read ahead of the object's x, and the first layer's words are read
// after z, not inside it. Reading the second layer's x first is what puts the
// table's first offset in r4 and its absolute value in r1, as native has them;
// in the native order (x, ax, z, az, bx, bz) they swap, differ 80. Measured
// every order of the eight statements with w, x, ax, bx before h, z, az, bz:
// w and h first is always best. Draft is 161 instructions to native 160.
// Under -Os, 5 November: differ 80 of 160 (tla-en); older figures below predate it.
// Draft, differ 108 of 160, skeleton gap 13 (tla-en, 2026-11-01)
// Yampi Desert Cave: finds a pushed object's kind by its sprite, copies its
// footprint between the map layers, then marks its cells.
// Proven: the search is a goto loop (a real loop strength-reduces i << 16). The
// step's pool 0x10000 needs a u16 view stepped apart from i (kind = i; kind++;
// i = kind) so combine splits it; a plain i++ gives adds, lsls, asrs. The second
// call's arguments must be locals, computed from, then to, so both stack
// arguments are live together; inline they store one at a time.
// Left, all allocation. i has 7 refs but i = 0's REG_EQUAL doubles its live
// length (30), so (u16)sprite's shift, 4 refs over 13, ranks first and takes r1;
// native has i in r1 and the shift in r4. kind and kind * 4 are one pseudo in
// native (r2 across the branch): s32 kind; kind *= 4 makes one, but it still
// lands in r1, and it reuses a local for a second value, so it is not kept. The
// loop's u16 view wants r2: native sets it before i, while the shift is live.
// kind * 4 in r1 instead of r2 is what rotates the ABS registers, moves the ldr
// past mov r9, and turns the 328 and 332 reloads into the other order; the 13
// skeleton lines are that block and the lsrs, asrs pair, so they should follow.
// Reading the first layer's words inline in the call gives differ 102 at
// skeleton gap 15. Native keeps ax's shift in r1 across az's load.
// Tried, no gain: while, for and break forms of the search (gap 23 to 37), i = 0
// inside the index, reversed compare, a s32 sprite << 16 (drops ldrsh), offset,
// dx and dz locals, declaration orders, toX and toZ for the first call, u16 ax.
// The compiler change did not close this one.
#include "types.h"
#include "call.h"
#include "ram_buffer.h"
#include "map.h"
#include "actor.h"

#define ABS(v) ((v) < 0 ? -(v) : (v))

extern u16 gData_02001968[];
extern s8 gData_02001974[][4];

struct Actor *Actor_GetById(s32 id);
void Func_02001848(s32 x, s32 z, s32 w, s32 h, s32 toX, s32 toZ);
void Map_FillCellRectByte2(s32 layer, s32 x, s32 z, s32 w, s32 h, s32 value);

s32 Func_02000038(s32 id) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  struct Actor *object = Actor_GetById(id);
  s16 sprite = ((struct ObjectAnimationState *)object->animation)->value28[0];
  s16 i;
  u32 kind;
  s32 w, h, x, z;
  s32 ax, az;
  s32 bx, bz;
  s32 toX, toZ, fromX, fromZ;

  i = 0;
  if ((u16)sprite != gData_02001968[i]) {
  loop:
    kind = (u16)i;
    kind = (u16)(kind + 1);
    i = (s16)kind;
    if (kind <= 5 && (u16)sprite != gData_02001968[kind])
      goto loop;
  }
  kind = (u16)i;
  if (kind == 6)
    return 0;
  w = (ABS(gData_02001974[kind][0]) + ABS(gData_02001974[kind][2])) >> 4;
  h = (ABS(gData_02001974[kind][1]) + ABS(gData_02001974[kind][3])) >> 4;
  bx = *(s32 *)&map->layers[2].unk08[0];
  x = (object->x + (gData_02001974[kind][0] << 16)) >> 20;
  z = (object->z + (gData_02001974[kind][1] << 16)) >> 20;
  ax = *(s32 *)&map->layers[1].unk08[0] >> 20;
  bz = *(s32 *)&map->layers[2].unk08[4];
  az = *(s32 *)&map->layers[1].unk08[4] >> 20;
  Func_02001848(x, z, w, h, (u16)ax + x, (u16)az + z);
  fromX = (u16)(bx >> 20) + x;
  fromZ = (u16)(bz >> 20) + z;
  toX = x + 64;
  toZ = z + 64;
  Func_02001848(fromX, fromZ, w, h, toX, toZ);
  Map_FillCellRectByte2(0, x, z, w, h, 255);
  Map_FillCellRectByte2(2, x, z, w, h, 255);
  return 1;
}
