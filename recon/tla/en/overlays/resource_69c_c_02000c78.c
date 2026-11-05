// Measured under -Os, 5 November 2026: differ 50 (tla-en); notes below may predate it.
// Draft, differ 75 of 126, skeleton gap 7 (tla-en, 2026-11-01)
// Finds an object's footprint by its sprite's shape in a six-entry table and
// copies the cells under it (Func_02002330), then fills byte 2 of those cells
// with 255 on layers 0 and 2; returns 0 for a shape not in the table. Shared
// with overlay 6a1 and others.
//
// Settled from the listing: u16 shape and i (promoted signed, hence ldrsh and
// lsl/lsr at each use), a plain for loop (the same output as a goto one, so
// the old goto and s16 i with (u16) casts are gone), the table indexed directly
// rather than through a rect pointer, and toX and toZ as locals, which keeps
// both stack stores after both loads.
//
// Left: the listing never rereads i << 16 after i is set in the loop. Here the
// bound test and the index do, GCSE's PRE merges that with the loop top's
// i << 16, and i ends up held shifted in r4 (adds r4, #0x10000) with no asrs.
// Most of the differ follows from that: i and shape << 16 swap r1 and r4, the
// post-loop (u16)i lands in r3 instead of r2, r1 and r2 swap through the ABS
// block, and in the tail the first load goes to r2 instead of r3 with its asrs
// into r1.
//
// Tried: `j = i + 1; i = j;` with u16 j, testing and indexing through j, stops
// PRE and gives the listing's whole loop shape (lsl, add 0x10000, asr, lsr),
// differ 66 and gap 4. It was left out as a forced temporary; shape still
// outranks i in global.c there (4 refs over 13 against 7 over 30), so shape
// takes r1. Inside a real loop the same j gets hoisted (gap 9). No gain from
// ++i in the test, the assignment's value, s32 or s16 i or shape, a while or
// do loop, u16 or s16 view locals, a struct or flat table with a k = i * 4
// index, named left/top locals, or an if/else or result variable at the end.
#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "field_sprite.h"

extern const u16 gData_02002478[];
extern const s8 gData_02002484[][4];

struct Actor *Actor_GetById(s32 id);
void Func_02002330(s32 x, s32 z, s32 width, s32 height, s32 toX, s32 toZ);
void Map_FillCellRectByte2(s32 layer, s32 x, s32 z, s32 width, s32 height,
                           s32 value);

#define ABS(x) ((x) < 0 ? -(x) : (x))

s32 Func_02000c78(s32 id) {
  u8 *map = (u8 *)Ram_HeapSlots->mapWork;
  struct Actor *object = Actor_GetById(id);
  u16 shape = *(s16 *)((struct FieldSprite *)object->animation)->record;
  u16 i;
  s32 width;
  s32 height;
  s32 x;
  s32 z;
  s32 toX;
  s32 toZ;

  for (i = 0; i < 6; i++) {
    if (shape == gData_02002478[i])
      break;
  }
  if (i == 6)
    return 0;
  width = (ABS(gData_02002484[i][0]) + ABS(gData_02002484[i][2])) >> 4;
  height = (ABS(gData_02002484[i][1]) + ABS(gData_02002484[i][3])) >> 4;
  x = (object->x + (gData_02002484[i][0] << 16)) >> 20;
  z = (object->z + (gData_02002484[i][1] << 16)) >> 20;
  toX = x + (u16)(*(s32 *)(map + 328) >> 20);
  toZ = z + (u16)(*(s32 *)(map + 332) >> 20);
  Func_02002330(x, z, width, height, toX, toZ);
  Map_FillCellRectByte2(0, x, z, width, height, 255);
  Map_FillCellRectByte2(2, x, z, width, height, 255);
  return 1;
}
