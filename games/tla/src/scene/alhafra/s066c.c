#include "types.h"
#include "scene_tables.h"
#include "main_routine_work.h"
#include "call.h"

s32 GameFlag_Test();
s32 Func_02001c74();
s32 FieldMessage_ShowAtActor();
s32 Func_02001d34();
s32 Event_AskYesNoWithActorAnimation();
s32 FieldEvent_Wait();
s32 Func_02001c8c();
s32 GameFlag_SetBit();
s32 Actor_SetPositionAndReset();
s32 Actor_ArmCallback();
s32 Actor_SetSpeedParameters();
s32 Motion_SetModeAndWaitAnimation();
extern u8 MsgAlhafra92[];

/// Talking to the actor: once flag 0x8b9 is set it says its last line;
/// otherwise it asks a yes-or-no question. A nonzero answer ends with another
/// line; the other sets the flag, and the actor and the leader step aside.
void Func_0200066c(s32 actor) {
  /* FAKEMATCH(barrier): SPLIT_CONSTANT builds each constant as the game does, a
   * shifted byte plus an addend; as plain constants, 12 of 92 instructions
   * differ in tla-en. */
  s32 message = (s32)MsgAlhafra92;

  if (Value1(GameFlag_Test, SPLIT_CONSTANT(0x800, 185)) != 0) {
    Func_02001c74(message + 4);
    FieldMessage_ShowAtActor(actor, 0);
  } else {
    Func_02001c74(message);
    FieldMessage_ShowAtActor(actor, 0);
    if (Event_AskYesNoWithActorAnimation(Func_02001d34(), 0) != 0) {
      FieldEvent_Wait(20);
      Func_02001c74(message + 1);
      Func_02001c8c(actor, 0);
    } else {
      GameFlag_SetBit(SPLIT_CONSTANT(0x800, 185));
      FieldEvent_Wait(20);
      Func_02001c74(message + 2);
      Call2(Func_02001c8c, actor, 0);
      Call3(Actor_SetPositionAndReset, gMainRoutineWork.currentOwner, 72, 344);
      Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_NORTH, 0);
      FieldEvent_Wait(10);
      Call3(Actor_SetSpeedParameters, actor, 0x10000, 0x8000);
      Call3(Actor_SetPositionAndReset, actor, 56, 328);
      Call3(Actor_ArmCallback, gMainRoutineWork.currentOwner, FACING_NORTHWEST,
            0);
      Actor_ArmCallback(actor, FACING_SOUTHEAST, 0);
      FieldEvent_Wait(20);
      Motion_SetModeAndWaitAnimation(actor, 3);
      FieldEvent_Wait(10);
      Func_02001c8c(actor, 0);
    }
  }
}
