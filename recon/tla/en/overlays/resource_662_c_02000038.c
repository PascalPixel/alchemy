// Matches: differ 0 (tla-en, 2026-11-08)
// tla-ja, tla-de, tla-es, tla-fr and tla-it also differ 0 of 160, so every
// edition can adopt it.
// Yampi Desert Cave: finds a pushed object's kind by its sprite, copies its
// footprint between the map layers, then marks its cells.
// Three tagged temporaries: the search's step, and the two calls' argument
// locals.
// - The search is a goto loop; a real loop strength-reduces i << 16.
// - i is reused twice, for the table's second byte and then for layers[1]'s x
//   offset. With no reuse 74 of 160 differ; reusing i only for the second byte,
//   48.
// - Statement order x, z, x offset, z offset, then layers[2]'s raw words,
//   shifted after the first call: reload then builds 332 in r0 and reaches 384
//   and 388 by adding to it, as the listing does.
// Before adoption: MapLayer.unk08's two words are read through s32 casts
// (typing them as s32 in map.h also matches), and Func_02001848's parameter
// names come from this caller alone.
#include "types.h"
#include "call.h"
#include "ram_buffer.h"
#include "map.h"
#include "actor.h"
#include "macros.h"

extern u16 gData_02001968[];
extern s8 gData_02001974[][4];

struct Actor *Actor_GetById(s32 id);
void Func_02001848(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Map_FillCellRectByte2(s32 layer, s32 x, s32 z, u32 width, u32 height,
                           s32 value);

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
    /* FAKEMATCH(temporary): kind set to i, then stepped; as kind = (u16)(i +
     * 1), 12 of 160 instructions differ in tla-en. */
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
  // i is reused: the footprint's second byte here, layers[1]'s x offset in
  // cells below.
  i = gData_02001974[kind][1];
  h = (ABS(i) + ABS(gData_02001974[kind][3])) >> 4;
  x = (object->x + (gData_02001974[kind][0] << 16)) >> 20;
  z = (object->z + (i << 16)) >> 20;
  i = *(s32 *)&map->layers[1].unk08[0] >> 20;
  az = *(s32 *)&map->layers[1].unk08[4] >> 20;
  bx = *(s32 *)&map->layers[2].unk08[0];
  bz = *(s32 *)&map->layers[2].unk08[4];
  /* FAKEMATCH(temporary): the first call's last two arguments held in locals;
   * as direct arguments, 36 of 160 instructions differ in tla-en. */
  layer1X = (u16)i + x;
  layer1Z = (u16)az + z;
  Func_02001848(x, z, w, h, layer1X, layer1Z);
  /* FAKEMATCH(temporary): the second call's arguments held in locals; as direct
   * arguments, 10 of 160 instructions differ in tla-en. */
  fromX = (u16)(bx >> 20) + x;
  fromZ = (u16)(bz >> 20) + z;
  toX = x + 64;
  toZ = z + 64;
  Func_02001848(fromX, fromZ, w, h, toX, toZ);
  Map_FillCellRectByte2(0, x, z, w, h, 255);
  Map_FillCellRectByte2(2, x, z, w, h, 255);
  return 1;
}
