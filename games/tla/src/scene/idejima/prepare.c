#include "types.h"
#include "scene_tables.h"
#include "task.h"
#include "main_routine_work.h"
#include "scene_ids.h"

s32 GameFlag_Test(s32 flag);
void Func_0200153c(void);
void Func_02001558(void);
void Func_020015a0(void);
void Func_020016d8(void);
void Func_02001ee0(void);
void Func_02002304(void);
void Func_02002fd4(void);
void Func_020031a8(s32 a);

/// The scene's start entry: the two scene callbacks, then by entrance the
/// scene that runs (2 and 4 also set the saved scene and entrance).
s32 Func_02002ec8(void) {
  s32 entrance;

  Func_0200153c();
  Task_Add(Func_02001558, TASK_PASS_FRAME);
  Task_Add(Func_020015a0, TASK_PASS_VBLANK);
  {
    /* FAKEMATCH: the party address pinned to r3 for the switch's read, apart
     * from case 4's; unpinned, gcse merges the two into r1 and 8 instructions
     * differ in tla-en. */
    register struct MainRoutineWork *party asm("r3") = &gMainRoutineWork;

    entrance = party->entrance;
  }
  switch (entrance) {
  case 2:
    if (GameFlag_Test(0x109) == 0)
      Func_020031a8(1);
  case 4: {
    struct MainRoutineWork *party = &gMainRoutineWork;

    /* FAKEMATCH: a one-pass block after the party address loads the scene id
     * before the saved scene's offset; without it, 2 instructions differ in
     * tla-en. */
    do {
    } while (0);
    party->savedScene = (s32)&SceneId_DailaInteriors2;
    party->savedEntrance = 1;
  }
    Func_02002fd4();
    break;
  case 11:
    Func_020016d8();
    break;
  case 12:
    Func_02001ee0();
    break;
  case 13:
    Func_02002304();
    break;
  }
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }
