// Matches: differ 0 (tla-en, 2026-11-08); tla-ja, tla-de, tla-es, tla-fr and
// tla-it also differ 0 of 160, so every edition can adopt it.
// Yampi Desert Cave: finds a pushed object's kind by its sprite, copies its
// footprint between the map layers, then marks its cells.
// No device: every form below is ordinary C under S2.
// - The search is a goto loop stepped through u16 casts (kind = (u16)i;
//   kind = (u16)(kind + 1); i = (s16)kind): a real loop strength-reduces
//   i << 16, and a plain i++ gives adds, lsls, asrs.
// - i is reused twice, for the table's second byte and then for the first
//   layer's x offset. As a CSE temporary the second byte (5 refs over 15
//   insns) outranks kind * 4 (5 over 17) in global allocation and takes r2,
//   pushing kind * 4 to r1 (84 in this statement order). As its own local the
//   x offset ties to its load and leaves r1 (58 with only the first reuse).
// - Statement order x, z, x offset, z offset, then the second layer's raw
//   words, shifted after the first call: reload then builds 332 in r0 and
//   reaches 384 and 388 by adding to it, as the listing does.
// - Both calls' stack arguments are locals computed after the layer reads, so
//   each pair is live together; inline, the first call's pair stores one at a
//   time and its u16 views swap r2 and r3 (36). The first pair needs its own
//   locals: sharing toX and toZ with the second call sets them twice, which
//   makes them global (also 36).
// Before adoption: MapLayer.unk08's two words are read through s32 casts, and
// Func_02001848's parameter names come from this caller alone.
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
  s32 az;
  s32 bx, bz;
  s32 toX, toZ, fromX, fromZ;
  s32 layer1X, layer1Z;

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
  // i is reused: the footprint's second byte here, the first layer's x offset
  // in cells below.
  i = gData_02001974[kind][1];
  h = (ABS(i) + ABS(gData_02001974[kind][3])) >> 4;
  x = (object->x + (gData_02001974[kind][0] << 16)) >> 20;
  z = (object->z + (i << 16)) >> 20;
  i = *(s32 *)&map->layers[1].unk08[0] >> 20;
  az = *(s32 *)&map->layers[1].unk08[4] >> 20;
  bx = *(s32 *)&map->layers[2].unk08[0];
  bz = *(s32 *)&map->layers[2].unk08[4];
  layer1X = (u16)i + x;
  layer1Z = (u16)az + z;
  Func_02001848(x, z, w, h, layer1X, layer1Z);
  fromX = (u16)(bx >> 20) + x;
  fromZ = (u16)(bz >> 20) + z;
  toX = x + 64;
  toZ = z + 64;
  Func_02001848(fromX, fromZ, w, h, toX, toZ);
  Map_FillCellRectByte2(0, x, z, w, h, 255);
  Map_FillCellRectByte2(2, x, z, w, h, 255);
  return 1;
}
