// Draft, differ 197 of 171, skeleton gap 51 (tla-en, 2026-11-01)
// How far a pushed object slides: from the object Func_02004bf4 finds, steps
// its footprint one step at a time in the push direction until a cell of it is
// blocked (Func_020054f8 returns 2), and leaves the stopping point in move;
// returns 0 when it cannot move at all. Shared with overlay 6a1 and others.
// The frame now agrees slot for slot: position 28, step 24, direction 20, width
// 16, height 12, layer 8, table offset 4, the saved object 0. Three 8-byte
// locals carry that. size and cursor are DImode pseudos, which cannot take a
// high register, so they lose to dx, dz and count as the listing has it (as
// plain s32s, width and height outrank those three); the listing's str-then-ldr of
// layer may be that subreg reload, not reproduced yet. info is memory because &info.step escapes, which
// is why the step head reloads direction * 4 unshared with the width sums. A
// plain direction local lets PRE share it (gap 63). The row loop is goto (a
// for hoists the table address, gap 78 to 93); the column loop may be either, and a
// row-local pointer changes nothing, so there is none.
// Left, in order:
// 1. PRE. The listing has no shared frame address: y and z are direct sp stores
// (combine folds a single-use address), the call argument is a fresh add r1,
// the row end a fresh add r2. alchemy-gcc's PRE merges every (plus sfp -12) into
// one pseudo that takes r6. A diagnostic -fno-gcse compile (scratch only) did
// not reach the listing by itself, so this is a lead, not a proof. It looks like
// the frame-address-in-a-call-loop family; that side is a compiler question for
// Pascal (K3), not a source one. Stopping it from source failed: no spelling
// gives the base address a second expression.
// 2. &info.step stays in r6 across the call and serves the read, where the
// listing has add r0 and ldr r3, [sp, #24]; scalar step fixes that but loses
// direction's memory. (s32 *)&info + 1, step[1] and an array read change nothing.
// 3. Registers. Listing: object r4, p r5, move r6, col r7, row r8, dx r9, dz
// r10, count r11. Here: move r7, col r5, row r4, object r8, dz r9, count r10,
// dx r11. Hi registers go r8, r10, r9, r11, so the listing allocates row, dz,
// dx, count; here count (u8, l142) beats dz (u5, l73) and dx (u7, l138), and row
// (u13, l54) beats object (u13, l105) to r4.
// Tried, untagged and taken back out as they do not reach differ 0: writing
// direction as *(s32 *)(char *)&info with a plain int (differ 185, gap 45, but
// wrong slots: ADDRESSOF renumbers it after size), (void)&info, dx and dz as an
// 8-byte local (spills both), direction inside a 3-word size struct (gap 75+).
#include "types.h"
#include "actor.h"

struct PushMove {
  s32 x; ///< 16.16 fixed-point pixels
  s32 y;
  s32 z;
  s32 direction;
  s32 unk10;
  s32 unk14;
};

extern s8 gData_02005f64[][4];
extern s32 gData_02005f7c[];

struct Actor *Func_02004bf4(s32 *step, s32 *a, s32 *direction);
s32 Func_020054f8(struct Actor *object, s32 *position);

#define ABS(x) ((x) < 0 ? -(x) : (x))

s32 Func_02004a8c(struct PushMove *move) {
  struct Actor *object;
  struct {
    s32 direction;
    s32 step;
  } info;
  s32 position[3];
  struct {
    s32 height;
    s32 width;
  } size;
  s32 dx;
  s32 dz;
  s32 count;
  s32 row;
  s32 col;
  struct {
    s32 offset;
    u8 *layer;
  } cursor;
  s32 result;

  object = Func_02004bf4(&info.step, &move->unk10, &move->direction);
  if (object != 0) {
    info.direction = move->direction;
    size.width = (ABS(gData_02005f64[info.direction][0]) +
             ABS(gData_02005f64[info.direction][2])) >>
            4;
    size.height = (ABS(gData_02005f64[info.direction][1]) +
              ABS(gData_02005f64[info.direction][3])) >>
             4;
    dx = gData_02005f7c[info.step] & 0xffff0000;
    dz = gData_02005f7c[info.step] << 16;
    move->unk14 = 0;
    count = 0;
    cursor.layer = &object->terrainId;
    *cursor.layer = 2;
    move->x = object->x + dx;
    move->z = object->z + dz;
    position[1] = object->y;
  step:
    cursor.offset = info.direction * 4;
    position[2] = move->z + (gData_02005f64[0][cursor.offset + 1] << 16);
    row = 0;
    if (row >= size.height)
      goto advance;
  nextRow:
      position[0] = move->x + (gData_02005f64[0][cursor.offset] << 16);
      for (col = 0; col < size.width; col++) {
        if (Func_020054f8(object, position) == 2)
          goto blocked;
        position[0] += 0x100000;
      }
      position[2] += 0x100000;
      row++;
      if (row < size.height)
        goto nextRow;
  advance:
    move->x += dx;
    count++;
    move->z += dz;
    goto step;
  blocked:
    *cursor.layer = 0;
    result = 0;
    if (count != 0) {
      move->x = object->x + dx * count;
      result = 1;
      move->y = object->y;
      move->z = object->z + dz * count;
    }
  } else {
    result = 0;
  }
  return result;
}
