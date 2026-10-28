#include "types.h"
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
  s32 message = (s32)MsgAlhafra92;

  if (Value1(GameFlag_Test, 0x8b9) != 0) {
    Call1(Func_02001c74, message + 4);
    Call2(FieldMessage_ShowAtActor, actor, 0);
  } else {
    Call1(Func_02001c74, message);
    Call2(FieldMessage_ShowAtActor, actor, 0);
    if (Value2(Event_AskYesNoWithActorAnimation, Func_02001d34(), 0) != 0) {
      Call1(FieldEvent_Wait, 20);
      Call1(Func_02001c74, message + 1);
      Call2(Func_02001c8c, actor, 0);
    } else {
      Call1(GameFlag_SetBit, 0x8b9);
      Call1(FieldEvent_Wait, 20);
      Call1(Func_02001c74, message + 2);
      Call2(Func_02001c8c, actor, 0);
      Call3(Actor_SetPositionAndReset, gMainRoutineWork.currentOwner, 72,
            344);
      Call3(Actor_ArmCallback, gMainRoutineWork.currentOwner, 0xc000, 0);
      Call1(FieldEvent_Wait, 10);
      Call3(Actor_SetSpeedParameters, actor, 0x10000, 0x8000);
      Call3(Actor_SetPositionAndReset, actor, 56, 328);
      Call3(Actor_ArmCallback, gMainRoutineWork.currentOwner, 0xa000, 0);
      Call3(Actor_ArmCallback, actor, 0x2000, 0);
      Call1(FieldEvent_Wait, 20);
      Call2(Motion_SetModeAndWaitAnimation, actor, 3);
      Call1(FieldEvent_Wait, 10);
      Call2(Func_02001c8c, actor, 0);
    }
  }
}
