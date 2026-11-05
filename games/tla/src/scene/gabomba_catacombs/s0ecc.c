#include "local.h"
#include "edition.h"
#include "call.h"

#if (LANGUAGE == LANGUAGE_JAPANESE)
#define CATACOMBS_SCREEN_EFFECT 513
#else
#define CATACOMBS_SCREEN_EFFECT 516
#endif

extern char SceneId_Ov671Room7, SceneId_Ov672Room1;
void Func_02001254(s32 a);
void Func_0200126c(s32 id);

s32 Func_02000ecc(void) {
  s32 entrance;
  struct Actor *statue;

  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = CATACOMBS_SCREEN_EFFECT;
  Func_02001254(0);
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov673Room1 &&
      (entrance = gMainRoutineWork.entrance) == 1) {
    if (GameFlag_Test(0xa27)) {
      gMainRoutineWork.retreatScene = (s32)&SceneId_Ov671Room7;
      gMainRoutineWork.retreatEntrance = entrance;
    } else if (GameFlag_Test(0xa28)) {
      gMainRoutineWork.retreatScene = (s32)&SceneId_Ov672Room1;
      gMainRoutineWork.retreatEntrance = 3;
    }
  }
  if (!GameFlag_Test(265) && gMainRoutineWork.scene == (s32)&SceneId_Ov673Room2 &&
      gMainRoutineWork.entrance > 2)
    Func_0200126c(gMainRoutineWork.currentOwner);
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov673Room3) {
    if (GameFlag_Test(0x200))
      Call6(Func_02001164, 6, 23, 1, 1, 8, 23);
    if (GameFlag_Test(513))
      Call6(Func_02001164, 0, 4, 1, 1, 8, 15);
    if (GameFlag_Test(514))
      Call6(Func_02001164, 0, 4, 1, 1, 9, 0x10);
    if (GameFlag_Test(515))
      Call6(Func_02001164, 0, 4, 1, 1, 8, 17);
    if (GameFlag_Test(516))
      Call6(Func_02001164, 0, 4, 1, 1, 6, 17);
    if (GameFlag_Test(517))
      Call6(Func_02001164, 0, 4, 1, 1, 5, 0x10);
    if (GameFlag_Test(518))
      Call6(Func_02001164, 0, 4, 1, 1, 6, 15);
  }
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov673Room5 && GameFlag_Test(0x200))
    Call6(Func_02001164, 15, 18, 1, 1, 17, 18);
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov673Room4 && GameFlag_Test(0x9ce) &&
      !GameFlag_Test(0xf93) && !GameFlag_Test(265)) {
    statue = Actor_GetById(0x40);
    Call3(Func_020011cc, 0x40, 0xa80000, 0x980000);
    statue->motionFlags = 6;
    statue->terrainHeight += 0x100000;
  }
  return 0;
}
