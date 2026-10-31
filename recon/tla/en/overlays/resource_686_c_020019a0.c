// Near miss, differ 297 of 410 (tla-en, 2026-10-31): the control flow, calls,
// arguments and fields are the native's; what remains is loop.c and
// allocation. The native re-loads gData_02003780's address inside the wait
// loop, and inside the innermost cell loop recomputes 14 - i (from its stack
// slot) and the row's address; this compiler hoists all three (loop.c moves
// them), so the hoisted address takes r6 and the leader, the work and the
// level shift up a register (native: leader r6, work r7, level r8,
// &motionFlags r10, map r9). The work pointer's constant REG_EQUIV doubles its
// live length and puts it below the level. Spelling z as
// gData_02002ff0[row].z gives the native's separate row * 8 + 4 offset
// register (r11) but measures 316.
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
  for (row = 0; gData_02002ff0[row].level != 99; row++) {
    if (i < gData_02002ff0[row].level)
      continue;
    for (m = 0; m < gData_02002ff0[row].height; m++) {
      for (n = 0; n < gData_02002ff0[row].width; n++) {
        struct AnkohlRuinsWaterRow *p = &gData_02002ff0[row];
        AnkohlRuins_Call2dfc(21, 14 - i, 1, 1, p->x + n, p->z + m);
        AnkohlRuins_Call2dfc(25, 14 - i, 1, 1, p->x + n, p->z + m + 64);
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
