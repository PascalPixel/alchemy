// Matches: differ 0 (tla-en, 2026-11-08); tla-ja, tla-de, tla-es, tla-fr and
// tla-it measure differ 0 too. Overlay 674's copy of overlay 668's
// Func_02004240 (the same listing). What closed it from 20: each pass loads the
// table into a local (table) before it sets index = kind * 4. The table's
// register is then live at the shift, so reload's round robin gives kind's
// reload r3 rather than r2; index + 1 shares r3 and the table keeps r2, and j's
// 0 and height's reload then come out r1 and r2, as in the listing. RowByte
// keeps index + 1 a value of its own: table[index + 1] folds to (table + index)
// + 1 (27). The row reads gData_0200407c[0][index] as it stands. Left: two
// tagged devices, each measured at this state.
// - (void)&kind (dead) makes kind addressable before width and height are
//   declared, so its slot (+20) comes ahead of their spill slots (+16, +12).
//   Without it 127 differ; taking the address in width's initializer instead
//   (BoxSpan(*&kind, 0)) gives 10. The old box pointer to kind (s32 *box =
//   &kind above width) also matches, but it is a temporary, a worse device.
// - The call through CallFunc_02003e2c (wrapper): inlining puts &cell straight
//   into r1, as the listing rebuilds it from sp. A direct call, a block
//   temporary for &cell, a pointer local set before the call, a result local
//   and cell as an array of one all take gcse's shared sfp - 12 from r5 (2).
// The shape still matters: index declared before layer gives 10, width, height,
// layer and index declared at the top with the block's statements kept 56.
// Retired 8 November: the box pointer (an untagged temporary) and the BoxByte
// inline; BoxSpan now takes kind by value.
#include "types.h"
#include "call.h"
#include "actor.h"
#include "system/vector3.h"

struct PushState {
  s32 x;
  s32 y;
  s32 z;
  s32 kind;
  s32 number;
  s32 unk14;
};

extern s8 gData_0200407c[][4];
extern s32 gData_02004094[];
struct Actor *Func_02003940(s32 *direction, s32 *number, s32 *kind);
s32 Func_02003e2c(struct Actor *object, struct Vector3 *cell);

static __inline__ s32 Abs(s32 value) { return value < 0 ? -value : value; }

static __inline__ s32 RowByte(s8 *row, s32 index) { return row[index]; }

static __inline__ s32 BoxSpan(s32 kind, s32 start) {
  return (Abs(gData_0200407c[kind][start]) +
          Abs(gData_0200407c[kind][start + 2])) >>
         4;
}

static __inline__ s32 CallFunc_02003e2c(struct Actor *object,
                                        struct Vector3 *cell) {
  return Func_02003e2c(object, cell);
}

s32 Func_020037d8(struct PushState *push) {
  struct Actor *object;
  struct Vector3 cell;
  s32 direction, kind;
  s32 dx, dz, steps, i, j, offset, result;

  object = Func_02003940(&direction, &push->number, &push->kind);
  if (object != 0) {
    kind = push->kind;
    /* FAKEMATCH(dead): the address of kind taken and dropped; as kind =
     * push->kind alone, 127 of 171 instructions differ in tla-en. */
    (void)&kind;
    {
      s32 width = BoxSpan(kind, 0);
      s32 height = BoxSpan(kind, 1);
      u8 *layer;
      s32 index;

      offset = gData_02004094[direction];
      dx = offset & 0xffff0000;
      dz = offset << 16;
      push->unk14 = 0;
      steps = 0;
      layer = &object->terrainId;
      *layer = 2;
      push->x = object->x + dx;
      push->z = object->z + dz;
      cell.y = object->y;
      for (;;) {
        s8 *table = gData_0200407c[0];

        index = kind * 4;
        cell.z = push->z + (RowByte(table, index + 1) << 16);
        for (j = 0; j < height; j++) {
          cell.x = push->x + (gData_0200407c[0][index] << 16);
          for (i = 0; i < width; i++) {
            /* FAKEMATCH(wrapper): the call through CallFunc_02003e2c; as a
             * direct call, 2 of 171 instructions differ in tla-en. */
            if (CallFunc_02003e2c(object, &cell) == 2)
              goto blocked;
            cell.x += 0x100000;
          }
          cell.z += 0x100000;
        }
        push->x += dx;
        steps++;
        push->z += dz;
      }
    blocked:
      *layer = 0;
    }
    result = 0;
    if (steps != 0) {
      push->x = object->x + dx * steps;
      result = 1;
      push->y = object->y;
      push->z = object->z + dz * steps;
    }
  } else
    result = 0;
  return result;
}
