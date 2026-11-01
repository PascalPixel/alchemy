// Draft, differ 222 of 171, skeleton gap 82 (tla-en, 2026-11-01)
// How far a pushed object slides: from the object Func_02004bf4 finds, steps
// its footprint one step at a time in the push direction until a cell of it is
// blocked (Func_020054f8 returns 2), and leaves the stopping point in move;
// returns 0 when it cannot move at all. Shared with overlay 6a1 and others.
// The shape is settled: the failure return sits at the end, the tables are not
// const (a const table's byte load is hoisted out of the row loop, the listing
// reloads it per row), and the direction lives in memory: the step head reloads
// it from the slot just below the step index, so the listing's direction * 4 is
// not shared with the width and height sums. info[2] is the plain form of that;
// a local whose address is taken gives the same effect but is a device.
// What is left is allocation. Here global CSE's single stack address for
// position takes a low register (r6) and object is pushed to a high one; the
// listing has object in r4, saved around the call, move in r6, col in r7, row,
// dx, dz and count in r8 to r11, width, height, the layer pointer and
// direction * 4 on the stack, and position addressed by sp directly at the
// step head, by r5 in the row and by a fresh add at the row's end. That stream
// needs no address pseudo shared across the loops, and no C form tried here
// stops global CSE making one: struct or array, a goto or for at each level, a
// row-local pointer (p here saves 11 instructions on plain array indexing). A real
// row loop also hoists the table address, which accounts for the 4 extra
// instructions. Next: find what keeps width and height out of high registers
// (dx, dz and count outrank them in the listing, the reverse of the numbers
// here: 8 refs over 75 insns for width against 7 over 132 for dx).
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
  s32 info[2]; // [0] the direction, [1] the step index Func_02004bf4 writes
  s32 position[3];
  s32 width;
  s32 height;
  s32 dx;
  s32 dz;
  s32 count;
  s32 row;
  s32 col;
  s32 direction;
  s32 *p;
  u8 *layer;
  s32 result;

  object = Func_02004bf4(&info[1], &move->unk10, &move->direction);
  if (object != 0) {
    info[0] = move->direction;
    width = (ABS(gData_02005f64[info[0]][0]) +
             ABS(gData_02005f64[info[0]][2])) >>
            4;
    height = (ABS(gData_02005f64[info[0]][1]) +
              ABS(gData_02005f64[info[0]][3])) >>
             4;
    dx = gData_02005f7c[info[1]] & 0xffff0000;
    dz = gData_02005f7c[info[1]] << 16;
    move->unk14 = 0;
    count = 0;
    layer = &object->terrainId;
    *layer = 2;
    move->x = object->x + dx;
    move->z = object->z + dz;
    position[1] = object->y;
  step:
    direction = info[0];
    position[2] = move->z + (gData_02005f64[direction][1] << 16);
    for (row = 0; row < height; row++) {
      p = position;
      *p = move->x + (gData_02005f64[direction][0] << 16);
      for (col = 0; col < width; col++) {
        if (Func_020054f8(object, position) == 2)
          goto blocked;
        *p += 0x100000;
      }
      p[2] += 0x100000;
    }
    move->x += dx;
    count++;
    move->z += dz;
    goto step;
  blocked:
    *layer = 0;
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
