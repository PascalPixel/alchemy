// Draft, differ 40 of 126 (tla-en, 2026-11-05). Older notes below may predate this figure.
// Overlay 6ad's copy of resource_69c_c_02000c78.c, shared with overlay 6a1 and
// others: finds an object's footprint by its sprite's shape in a six-entry
// table and copies the cells under it (Func_020054e8), then fills byte 2 of
// those cells with 255 on layers 0 and 2; returns 0 for a shape not in the
// table.
// Left: allocation only. The listing keeps i in r1 and the shifted shape in r4
// (here swapped), and the top corner in r1 with the byte offset in r2 (also
// swapped); the gap of 3 is the second map offset's load scheduled ahead of the
// shifts, which the listing's r3 reuse orders. Moving the z statement after the
// map offsets gives differ 38 but gap 5. Tried without effect: declaration
// orders, inner-block scopes, locals for the corners, an explicit offset
// variable, splitting the coordinate sums from their shifts.
// The increment's lsl plus 0x10000 comes only from spelling it shifted: a real
// for loop gets strength-reduced, and plain i + 1 keeps adds #1.
#include "types.h"
#include "call.h"
#include "ram_buffer.h"
#include "actor.h"
#include "field_sprite.h"

extern const u16 gData_02005f58[];
extern const s8 gData_02005f64[][4];

struct Actor *Actor_GetById(s32 id);
void Func_020054e8(s32 x, s32 z, s32 width, s32 height, s32 toX, s32 toZ);
void Func_02004a44(s32 layer, s32 x, s32 z, s32 width, s32 height, s32 value);

#define ABS(x) ((x) < 0 ? -(x) : (x))

s32 Func_02004938(s32 id) {
  u8 *map = (u8 *)Ram_HeapSlots->mapWork;
  struct Actor *object = Actor_GetById(id);
  s16 shape = *(s16 *)((struct FieldSprite *)object->animation)->record;
  s16 i = 0;
  u32 j;
  s32 width;
  s32 height;
  s32 x;
  s32 z;
  s32 mapX;
  s32 mapZ;
  s32 toX;
  s32 toZ;

  if ((u16)shape != gData_02005f58[(u16)i]) {
  next:
    j = (u16)((i << 16) + 0x10000 >> 16);
    i = j;
    if (j <= 5 && (u16)shape != gData_02005f58[j])
      goto next;
  }
  j = (u16)i;
  if (j == 6)
    return 0;
  width = (ABS(gData_02005f64[j][0]) + ABS(gData_02005f64[j][2])) >> 4;
  height = (ABS(gData_02005f64[j][1]) + ABS(gData_02005f64[j][3])) >> 4;
  x = (object->x + (gData_02005f64[j][0] << 16)) >> 20;
  z = (object->z + (gData_02005f64[j][1] << 16)) >> 20;
  mapX = *(s32 *)(map + 328) >> 20;
  mapZ = *(s32 *)(map + 332) >> 20;
  toX = x + (u16)mapX;
  toZ = z + (u16)mapZ;
  Func_020054e8(x, z, width, height, toX, toZ);
  Func_02004a44(0, x, z, width, height, 255);
  Func_02004a44(2, x, z, width, height, 255);
  return 1;
}
