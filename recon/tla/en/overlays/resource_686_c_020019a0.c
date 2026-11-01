// Draft, differ 265 of 410, skeleton gap 67 (tla-en, 2026-11-01)
// Control flow, calls, arguments and fields are the native's. The body kept here
// is the best device-free form; the row loop walks the table as bytes through a
// pointer, and biv elimination folds t + row where the native keeps row * 8 (r7)
// and row * 8 + 4 (r11). Native: leader r6, work r7, level r8, &motionFlags r10,
// map r9, row r7, m r8, n r4, row pointer r6, 14 - i r5, table r9, n << 16 r10.
// Why the native keeps things in its loops, found with the loop dump:
// - Wait loop: the address load is moved at 10 real insns and stays at 18, with
//   two unused s16 counters stepped in the for (one is not enough). The whole
//   loop then matches. Plain C for the extra insns is not found.
// - Cell loop: 14 - i and the row pointer stay when each is a local set twice in
//   the loop, s32 a = 0 then a = 14 - i, and likewise p: the set count stops
//   scan_loop and flow drops the dead default. This gives the native's
//   movs r5, #14 / subs r5, r5, r2. S2's Default note covers it, but alone it
//   measures worse (differ 291, gap 93).
// - Pass loop: the guard's table + row is forced out with the table load. A copy
//   b = row set twice non-consecutively stops it, b = 0; n = 0; b = row inside a
//   one-pass block so cse1 keeps b; the loads at both tails then appear.
// - The m counter and level are one s16: the native's r8 for both, and this puts
//   work in r7. With the devices above m and level shared took differ 245 to 240.
// - z = row + 4 declared before the continue: differ 240 to 193, gap 74 to 47.
// - A row-level z is not reduced to r11 (benefit 2); gData_02002ff0[b + 4] twice
//   in the body is, and so is the n << 16 counter, but then the table is a fresh
//   ldr at the cell guard, not mov r9, r1.
// Best with those devices: differ 168 (gap 64) or 193 (gap 47), so they came back
// out, as differ 0 was not reached. Left in that form: the table + row sum is
// merged by PRE with the guard's (the native's guard uses r1, its body r9, and
// cprop here makes them one), the copy spilled to sp+12 is propagated away at
// the pass tail, and row and n fall a register down.
// Natural rows[row].field with the same defaults: 288, gap 84; with b, 255, 93.
// Measured without gain: the symbol as u8[] with no pointer (gap 114), the
// pointer reset per row or per m pass (95, 111), a struct pointer (113), z or x
// through the array (120, 106), the new level as its own variable (87), one-pass
// blocks round the two cell calls (102 and 86, the latter with shared a and p), a
// goto wait loop written as the native's strength-reduced counter (68, differ
// 242; not a form to keep), m and level shared in this body (277), table set in
// the for tests through t1, t2, t3 (differ 278, gap 116).
// Sixteen steps adding gData_02003788 to Data_02004030's level,
// gData_02003780 frames apart, carrying the leader when it stands on cell 232,
// then the cells for the new level from the gData_02002ff0 rows, as
// Func_0200160c does with other cells. The step counter and the new level
// share one variable, as the native keeps both in the same stack slot.
#include "../../../../games/tla/src/scene/ankohl_ruins/ankohl_ruins.h"
#include "map.h"
#include "call.h"

struct AnkohlRuinsFloatWork {
  s16 unk00;
  s16 tile;
  s32 level;
  s32 unk08;
  s32 unk0c;
};

struct AnkohlRuinsWaterRow {
  u8 level;
  u8 width;
  u8 height;
  u8 x;
  u8 z;
  u8 unk05[3];
};

extern struct AnkohlRuinsFloatWork Data_02004030;
extern struct AnkohlRuinsWaterRow gData_02002ff0[];

void Func_020015ac(void);
void Func_02002e04(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);

void Func_020019a0(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  struct MapWork *map = (struct MapWork *)Ram_HeapSlots->mapWork;
  struct AnkohlRuinsFloatWork *work = &Data_02004030;
  s32 level;
  s16 i;
  s16 j;
  s32 row;
  u8 *t;
  s16 m;
  s16 n;

  Actor_SetModeById(gMainRoutineWork.currentOwner, 0);
  level = work->level >> 19;
  leader->motionFlags = 0;
  AnkohlRuins_Call2e24(0x50000, 0x50000, 0x10000);
  FieldEvent_Wait(30);
  Func_02002eec(0x22c);
  AnkohlRuins_Call2df4(22, 66, 3, 1, 7, 84);
  AnkohlRuins_Call2df4(22, 1, 3, 5, 7, 19);
  /* FAKEMATCH(wrapper): the scene's forwarding inlines and Call6 kept; as direct
   * calls, 341 of 410 instructions differ in tla-en. */
  i = 0;
step:
  {
    struct MapCell *cells;
    s32 rise;

    for (j = 0; j < gData_02003780; j++)
      FieldEvent_Wait(1);
    rise = gData_02003788;
    work->level += rise;
    cells = map->layers[2].cells;
    if (cells[leader->x / 0x100000 + ((leader->z / 0x100000) << 7)]
            .collisionCode == 232) {
      leader->y += rise;
      leader->terrainHeight = leader->y;
    }
    FieldEvent_Wait(1);
    if (level == 2 && i == 0) {
      AnkohlRuins_Call2df4(11, 70, 1, 1, 11, 69);
      Call6((void (*)())Func_02002e04, 10, 68, 1, 1, 4, 86);
      AnkohlRuins_Call2dfc(4, 24, 1, 1, 4, 23);
    }
    if (level == 1 && i == 15)
      Call6((void (*)())Func_02002e04, 18, 73, 1, 1, 4, 86);
  }
  if (++i <= 15)
    goto step;
  AnkohlRuins_Call2e24(-1, -1, 0xe666);
  FieldEvent_Wait(5);
  leader->motionFlags = 3;
  if (work->level < 0x200000) {
    gData_02003782 = 0;
    Task_Add(Func_020015ac, 0x480);
  } else {
    Call6((void (*)())Func_02002e04, 69, 24, 1, 4, 68, 23);
    GameFlag_SetBit(0x204);
    GameFlag_ClearBit(0x17d);
  }
  AnkohlRuins_Call2df4(18, 66, 3, 1, 7, 84);
  AnkohlRuins_Call2df4(18, 1, 3, 5, 7, 19);
  i = work->level >> 19;
  t = (u8 *)gData_02002ff0;
  for (row = 0; t[row] != 99; row += 8) {
    if (i < t[row])
      continue;
    for (m = 0; m < t[row + 2]; m++) {
      for (n = 0; n < t[row + 1]; n++) {
        u8 *p = t + row;
        AnkohlRuins_Call2dfc(21, 14 - i, 1, 1, p[3] + n, t[row + 4] + m);
        AnkohlRuins_Call2dfc(25, 14 - i, 1, 1, p[3] + n, t[row + 4] + m + 64);
      }
    }
  }
  AnkohlRuins_Call2dfc(19, 14 - i, 2, 1, 2, 27);
  AnkohlRuins_Call2dfc(19, 14 - i, 2, 1, 13, 25);
  AnkohlRuins_Call2dfc(18, 14 - i, 1, 1, 7, 23);
  AnkohlRuins_Call2dfc(24, 14 - i, 1, 1, 9, 23);
  AnkohlRuins_Call2dfc(19, 14 - i, 2, 1, 2, 91);
  AnkohlRuins_Call2dfc(19, 14 - i, 2, 1, 13, 89);
  AnkohlRuins_Call2dfc(26, 14 - i, 1, 1, 7, 87);
  AnkohlRuins_Call2dfc(27, 14 - i, 1, 1, 9, 87);
  if (i == 2)
    AnkohlRuins_Call2dfc(18, 7, 1, 1, 4, 23);
}
