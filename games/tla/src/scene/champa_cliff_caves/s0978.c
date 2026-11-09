#include "scene_tables.h"
#include "scene.h"

void Func_020005e8(s32 actor);
void Func_0200067c(s32 value);
void Func_080d47b4(s32 actor, s32 value, s32 frames);
void Event_ShowMessageThenWait(s32 actor, s32 value, s32 frames);

void Func_02000978(s32 actor) {
  Func_0200345c();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgAhHaveYou);
  Func_020005e8(actor);
  Func_02003534_(8, 0x3000);
  Actor_SetModeById(8, 7);
  Event_Wait(30);
  Func_080d47b4(8, 258, 50);
  Actor_SetModeById(8, 1);
  Event_Wait(15);
  Event_ShowMessageThenWait(8, 0, 5);
  Actor_SetModeById(8, 7);
  Event_Wait(30);
  Func_080d47b4(8, 264, 50);
  Actor_SetModeById(8, 1);
  Event_Wait(15);
  Event_ShowMessageThenWait(8, 0, 5);
  Func_0200067c(0);
  Actor_ArmCallbackAndRefresh(8, FACING_WEST);
  Event_Wait(10);
  Event_ShowMessageThenWait(8, 0, 5);
  Func_02003534_(8, 0x5000);
  Func_02003464();
}
