// Draft, differ 197 of 171, skeleton gap 27 (tla-en, 2026-11-01)
// How far a pushed object slides: from the object Func_02004bf4 finds, steps
// its footprint one step at a time in the push direction until a cell of it is
// blocked (Func_020054f8 returns 2), and leaves the stopping point in move;
// returns 0 when it cannot move at all. Shared with overlay 6a1 and others.
// The frame agrees slot for slot: position 28, step 24, direction 20, width 16,
// height 12, layer 8, table offset 4, the saved object 0. Three 8-byte locals
// carry that, and they are the first thing to doubt: they look like steering,
// not source. size and cursor are DImode pseudos, which cannot take a high
// register; info is memory because &info.step escapes. The row loop is goto (a
// for hoists the table address); the column loop may be either.
// Left, in order:
// 1. The frame address, which drives everything else. Each array or struct
// access expands to its own pseudo = sfp - 12 (a struct spells it the same;
// differ unchanged). gcse then ties them to one register: PRE's deletions plus
// its COPY-PROP pass (.07.gcse: insns z store, row x store, call argument, x +=)
// all become the y store's pseudo, which takes r6. The listing has none of that:
// y and z are direct sp stores, the argument a fresh add r1, the row x store and
// x += share r5, the z += has its own add r2. With -fno-gcse (scratch only)
// plain scalars for everything except position, with the goto row loop, come
// nearest in shape and spill direction and the others unforced, and a scalar
// step with the size and cursor structs scores best of all (149 changed lines
// against 183 with gcse on, whole-line diff by hand). One reading that fits most
// of it: that pseudo got no register and is rematerialized from its REG_EQUIV,
// but that leaves r5 and r2 unexplained. Why gcse does not act here is the open
// question; gcse_main has no early exit that applies (no setjmp, few blocks).
// This is the frame-address-in-a-call-loop family. If no source form stops it,
// it is a compiler question for Pascal (K3), out of scope for a draft.
// 2. Registers. Listing: object r4, x pointer r5, move r6, col r7, row r8, dx r9,
// dz r10, count r11. Here move r7, col r5, row r4, object r8, dz r9, count r10,
// dx r11, with the shared frame address in r6. Expect this to move once 1 does.
// Tried without gain, all from this draft: s32 *p or *q set in the row (copy-prop
// removes them, differ unchanged); position as a struct; x += in the for
// increment; continue form; a named result for the call; column loop as goto;
// y store inside the step head (204); step as for (;;) (223), with a for or
// do-while row (229); swapping the pair members (201 to 207); declaring row, dz,
// dx first (no change). A scalar step without info gives differ 193, and 189
// with size's members swapped, but gap 33: direction takes a register and loses
// its slot, so it is not kept. All-plain scalars give 213, gap 41.
#include "types.h"
#include "call.h"
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
