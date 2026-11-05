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
  u16 kind;
  s32 w, h, x, z;
  s32 ax, az;
  s32 bx, bz;
  s32 toX, toZ, fromX, fromZ;

  i = 0;
  if ((u16)sprite != gData_02001968[i]) {
  loop:
    kind = i;
    kind++;
    i = kind;
    if (kind <= 5 && (u16)sprite != gData_02001968[kind])
      goto loop;
  }
  kind = i;
  if (kind == 6)
    return 0;
  w = (ABS(gData_02001974[kind][0]) + ABS(gData_02001974[kind][2])) >> 4;
  h = (ABS(gData_02001974[kind][1]) + ABS(gData_02001974[kind][3])) >> 4;
  x = (object->x + (gData_02001974[kind][0] << 16)) >> 20;
  ax = *(s32 *)&map->layers[1].unk08[0] >> 20;
  z = (object->z + (gData_02001974[kind][1] << 16)) >> 20;
  az = *(s32 *)&map->layers[1].unk08[4] >> 20;
  bx = *(s32 *)&map->layers[2].unk08[0];
  bz = *(s32 *)&map->layers[2].unk08[4];
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
