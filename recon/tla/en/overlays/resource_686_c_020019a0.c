// Draft, differ 265 of 410, skeleton gap 67 (tla-en, 2026-11-01)
// Control flow, calls, arguments and fields are the native's. The row loop
// walks the table as bytes (row += 8, t[row + n]) through a pointer: that is
// what took the gap from 101 to 67, but biv elimination then folds t + row into
// one pointer, where the native keeps row * 8 (r7) and row * 8 + 4 (r11) as
// offsets from a base it re-loads from the pool at each loop's tail. What is
// left is loop.c: the native keeps gData_02003780's address inside the wait
// loop (10 real insns, life 1: threshold * savings * lifetime passes, so this
// compiler always moves it), and keeps 14 - i and the row's address inside the
// cell loop; here they sit a level out and push the work pointer to a high
// register, so its reads gain a mov each. Native: leader r6, work r7, level r8,
// &motionFlags r10, map r9.
// Measured without gain: the symbol as u8[] with no pointer (gap 114), the
// pointer reset per row or per m pass (95, 111), a struct pointer (113), z or x
// through the array (120, 106), the new level as its own variable (87), one-pass
// blocks round the two cell calls (102; they do keep cse1 from merging 14 - i,
// but the matched constants are still moved), and a goto wait loop written as
// the native's strength-reduced counter (68, differ 242; not a form to keep).
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
