// Draft, differ 130 of 268: the Gabomba Catacombs' map setup: sets the screen
// effect (513 in the Japanese edition); entering Ov673Room1 by entrance 1
// sets Retreat to Ov671Room7's entrance 1 once flag 0xa27 is set, else to
// Ov672Room1's entrance 3 once flag 0xa28 is; before flag 265, entrances
// past 2 of Ov673Room2 run Func_0200126c for the leader; in Ov673Room3
// flags 0x200 to 518 open their cells, in Ov673Room5 flag 0x200 opens one,
// and in Ov673Room4, after flag 0x9ce and before flags 0xf93 and 265, the
// statue (object 0x40) stands raised 16 pixels.
// Left: register choice from the screen effect's store on (the native builds 428 in r0 and derives the entrance's offset from the scene's, adds r1 #2), which shifts the Retreat stores and the later tests.
#include "../../../../games/tla/src/scene/gabomba_catacombs/local.h"
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

void Func_02000ecc(void) {
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
}
