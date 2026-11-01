// Draft, differ 309 of 410, skeleton gap 99 (tla-en, 2026-11-01)
// Control flow, calls, arguments and fields are the native's; what remains is
// loop.c and allocation, the same gap as Func_020019a0. The native re-loads
// gData_02003780's address inside the wait loop; this compiler hoists it to a
// call-saved low register (r6), which pushes the leader to r7 and the work to a
// high register, so its reads gain a mov each. Native: leader r6, work r7,
// level r8, map r9, &motionFlags r10. In the innermost cell loop the native
// also recomputes 14 - i and the row's address each pass, and reads z through
// its own row * 8 + 4 offset (r11); here they are hoisted. Measured without
// gain: a real outer loop (hoists the constants 11 and 68, gap 121), z or x
// spelled gData_02002fa8[row].z (108 to 132), the row pointer at the m level
// (112), scope, type and declaration-order changes of the locals, a Call1 wait,
// and a goto wait loop with no hoist (103, and not a form to keep).
// Sixteen steps of rise (gData_02003788 each, gData_02003780 frames apart)
// carrying the leader when it stands on water (cell 232), then the cells for
// the new level from the gData_02002fa8 rows. The step counter and the new
// level share one variable, as the native keeps both in one stack slot.
// Func_020019a0 is the same with other cells.
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
extern struct AnkohlRuinsWaterRow gData_02002fa8[];

void Func_020015ac(void);
void Func_02002e04(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);

void Func_0200160c(void) {
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
  AnkohlRuins_Call2e24(0x40000, 0x40000, 0x10000);
  FieldEvent_Wait(30);
  Func_02002eec(0x22c);
  AnkohlRuins_Call2df4(22, 66, 3, 1, 7, 66);
  AnkohlRuins_Call2df4(22, 1, 3, 5, 7, 1);
  i = 0;
step:
  {
    struct MapCell *cells;

    for (j = 0; j < gData_02003780; j++)
      FieldEvent_Wait(1);
    work->level += gData_02003788;
    cells = map->layers[2].cells;
    if (cells[leader->x / 0x100000 + ((leader->z / 0x100000) << 7)]
            .collisionCode == 232) {
      leader->y += gData_02003788;
      leader->terrainHeight = leader->y;
    }
    FieldEvent_Wait(1);
    if (level == 2 && i == 0) {
      AnkohlRuins_Call2df4(11, 70, 1, 1, 11, 69);
      Call6((void (*)())Func_02002e04, 10, 68, 1, 1, 11, 68);
      AnkohlRuins_Call2dfc(10, 5, 1, 1, 11, 5);
    }
    if (level == 1 && i == 15)
      Call6((void (*)())Func_02002e04, 18, 73, 1, 1, 11, 68);
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
    Call6((void (*)())Func_02002e04, 69, 6, 1, 2, 68, 5);
    GameFlag_SetBit(0x204);
    GameFlag_ClearBit(0x17d);
  }
  AnkohlRuins_Call2df4(18, 66, 3, 1, 7, 66);
  AnkohlRuins_Call2df4(18, 1, 3, 5, 7, 1);
  i = work->level >> 19;
  for (row = 0; gData_02002fa8[row].level != 99; row++) {
    if (i < gData_02002fa8[row].level)
      continue;
    for (m = 0; m < gData_02002fa8[row].height; m++) {
      for (n = 0; n < gData_02002fa8[row].width; n++) {
        struct AnkohlRuinsWaterRow *p = &gData_02002fa8[row];
        AnkohlRuins_Call2dfc(21, 14 - i, 1, 1, p->x + n, p->z + m);
        AnkohlRuins_Call2dfc(25, 14 - i, 1, 1, p->x + n, p->z + m + 64);
      }
    }
  }
  AnkohlRuins_Call2dfc(19, 14 - i, 2, 1, 2, 7);
  AnkohlRuins_Call2dfc(22, 14 - i, 2, 1, 13, 7);
  AnkohlRuins_Call2dfc(18, 14 - i, 1, 1, 7, 5);
  AnkohlRuins_Call2dfc(24, 14 - i, 1, 1, 9, 5);
  AnkohlRuins_Call2dfc(19, 14 - i, 2, 1, 2, 71);
  AnkohlRuins_Call2dfc(22, 14 - i, 2, 1, 13, 71);
  AnkohlRuins_Call2dfc(26, 14 - i, 1, 1, 7, 69);
  AnkohlRuins_Call2dfc(27, 14 - i, 1, 1, 9, 69);
  if (i == 2)
    AnkohlRuins_Call2dfc(18, 8, 1, 1, 11, 5);
}
