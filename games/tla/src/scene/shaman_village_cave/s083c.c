#include "local.h"

extern char SceneId_Ov695;

void Func_0200083c(void) {
  Func_02001080();
  FieldEvent_ResolvePendingActions(0);
  {
    struct MainRoutineWork *party = &gMainRoutineWork;


    party->battleRule24b = 2;
    /* FAKEMATCH(dead): the empty once-only block's loop note ends the
     * scheduling block after the store; without it the scene id is loaded
     * before the store (two reordered rows). */
    do {
    } while (0);
  }
  Event_SetAfterBattleDestination((s32)&SceneId_Ov695, 20);
  Event_SetDefeatDestination((s32)&SceneId_Ov695, 21);
  Func_020011a0(102, 2);
}
