#include "ankohl_ruins.h"
#include "task.h"

void Func_020020fc(void) {
  s32 z = Actor_GetById(9)->z >> 20;

  if (AnkohlRuins_FlagCheck(0x9dd) != 0 && z == 24) {
    s32 found = Func_02002dcc(Func_02001f7c);

    if (found == -1) {
      Func_02002e44();
      Func_02002edc(0);
      Func_02002eec(207);
      AnkohlRuins_Call2e24(0x20000, 0x20000, 0x10000);
      FieldEvent_Wait(10);
      Camera_MoveWithinMapBounds(0xe80000, -1, 0x1880000, 1);
      Func_02002eb4();
      Task_Add(Func_02001f7c, TASK_PASS_VBLANK);
      FieldEvent_Wait(40);
      Camera_FollowActor(gAdventure.currentOwner, 1);
      Func_02002eb4();
      AnkohlRuins_Call2dfc(13, 30, 3, 2, 13, z);
      AnkohlRuins_Call2dfc(14, 87, 1, 1, 14, 88);
      AnkohlRuins_FlagSet(381);
      AnkohlRuins_FlagSet(353);
      Func_02002e4c();
    }
  }
}

void Func_020021d4(void) {
  s32 z = Actor_GetById(9)->z >> 20;

  if (AnkohlRuins_FlagCheck(0x9dd) != 0 && z == 24) {
    /* FAKEMATCH: the task function pinned to r5 leaves the z cell in r6; plain, the
     * two swap (differ 12). */
    register void (*func)(void) asm("r5") = Func_02001f7c;

    if (Func_02002dcc(func) == -1) {
      AnkohlRuins_Call2e24(0x20000, 0x20000, 0x10000);
      Task_Add(func, TASK_PASS_VBLANK);
      AnkohlRuins_Call2dfc(13, 30, 3, 2, 13, z);
      AnkohlRuins_Call2dfc(14, 87, 1, 1, 14, 88);
      AnkohlRuins_FlagSet(381);
      AnkohlRuins_FlagSet(353);
      Func_02002e4c();
    }
  }
}
