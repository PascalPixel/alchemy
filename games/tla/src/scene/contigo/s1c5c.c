#include "types.h"
#include "party_state.h"

extern const u8 gData_0200466c[];

void WaitTicks(s32 ticks);
void FieldEvent_WaitTicks(s32 ticks);
void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);
void ObjectMotion_SetSpeedParameters(s32 actor, s32 speed, s32 acceleration);
void ObjectMotion_EnableActionAndSetCallback(s32 actor, const void *script);
void Func_02004004(s32 srcX, s32 srcY, s32 destX, s32 destY, s32 width,
                   s32 height);
void Func_0200406c(void);
void Func_02004204(s32 skip);
void Func_020042b4(s32 cue);

void Func_02001c5c(s32 a, s32 b, s32 c, s32 d) {
  Func_0200406c();
  Func_02004204(0);
  Func_020042b4(188);
  {
    s32 width = 1;
    s32 height = 2;

    Func_02004004(a, b, c, d, width, height);
    Func_02004004(a, b + 2, c + 1, d, width, height);
  }
  FieldEvent_WaitTicks(20);
  ObjectMotion_SetSpeedParameters(gPartyState.currentOwner, 0x8000, 0x4000);
  ObjectMotion_EnableActionAndSetCallback(gPartyState.currentOwner,
                                          gData_0200466c);
  WaitTicks(12);
  Func_020042b4(123);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
}
