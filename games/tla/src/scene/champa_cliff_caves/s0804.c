#include "scene_tables.h"
#include "scene.h"

struct EventCounters {
  u8 unk000[0x1c4];
  u16 count;
};

void Motion_SetVarCbAndRefresh(s32 actor, s32 variant);
void Motion_SetModeAndWaitAnimation(s32 actor, s32 mode);
void Func_020005e8(s32 actor);
void Func_0200067c(s32 value);
s32 Func_080d47b4(s32 actor, s32 value, s32 frames);
void Event_ShowMessageThenWait(s32 actor, s32 value, s32 frames);

void Func_02000804(s32 actor) {
  Func_0200345c();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgThisLooksLike);
  Func_020005e8(actor);
  Func_02003534_(8, 0x3000);
  Actor_SetModeById(8, 7);
  Event_Wait(30);
  Func_080d47b4(8, 257, 50);
  Actor_SetModeById(8, 1);
  Event_Wait(15);
  Event_ShowMessageThenWait(8, 0, 5);
  Actor_SetModeById(8, 7);
  Event_Wait(30);
  Func_080d47b4(8, 261, 50);
  Actor_SetModeById(8, 1);
  Event_Wait(15);
  Event_ShowMessageThenWait(8, 0, 5);
  Actor_SetModeById(8, 7);
  Event_Wait(30);
  Func_080d47b4(8, 256, 50);
  Actor_SetModeById(8, 1);
  Event_Wait(15);
  Event_ShowMessageThenWait(8, 0, 5);
  Motion_SetVarCbAndRefresh(8, 2);
  Event_Wait(8);
  Event_ShowMessageThenWait(8, 0, 5);
  Func_0200067c(0);
  Actor_ArmCallbackAndRefresh(8, FACING_WEST);
  Event_Wait(10);
  Func_080d47b4(8, 258, 30);
  /* FAKEMATCH(signature): called through a type returning s32, which gives the
   * native's argument order (r1 before r0); as the void call scene_call.h
   * declares, which the other scenes match with, 2 of 144 instructions differ
   * in tla-en. */
  ((s32 (*)(s32, s32))FieldMessage_ShowAtActor)(8, 0);
  if (Event_AskYesNoWithActorAnimation(4, 0) == 0) {
    Event_Wait(20);
    Event_ShowMessageThenWait(8, 0, 5);
    ((struct EventCounters *)Ram_HeapSlots->eventWork)->count++;
  } else {
    Event_Wait(35);
    ((struct EventCounters *)Ram_HeapSlots->eventWork)->count++;
    Event_ShowMessageThenWait(8, 0, 5);
  }
  Motion_SetModeAndWaitAnimation(8, 3);
  Event_ShowMessageThenWait(8, 0, 5);
  Func_02003534_(8, 0x5000);
  Func_02003464();
}
