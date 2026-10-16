#include "scene.h"

struct EventCounters {
  u8 unk000[0x1c4];
  u16 count;
};

void Motion_SetVarCbAndRefresh(s32 actor, s32 variant);
void Motion_SetModeAndWaitAnimation(s32 actor, s32 mode);
void Func_020005e8(s32 actor);
void Func_0200067c(s32 value);
s32 Func_02003544(s32 actor, s32 value, s32 frames);
void Func_0200351c(s32 actor, s32 value, s32 frames);

void Func_02000804(s32 actor) {
  Func_0200345c();
  Func_020035b4(0);
  Func_0200350c((s32)MsgThisLooksLike);
  Func_020005e8(actor);
  Func_02003534_(8, 0x3000);
  Object_SetModeById(8, 7);
  Battle_WaitMode0(30);
  Func_02003544(8, 257, 50);
  Object_SetModeById(8, 1);
  Battle_WaitMode0(15);
  Func_0200351c(8, 0, 5);
  Object_SetModeById(8, 7);
  Battle_WaitMode0(30);
  Func_02003544(8, 261, 50);
  Object_SetModeById(8, 1);
  Battle_WaitMode0(15);
  Func_0200351c(8, 0, 5);
  Object_SetModeById(8, 7);
  Battle_WaitMode0(30);
  Func_02003544(8, 256, 50);
  Object_SetModeById(8, 1);
  Battle_WaitMode0(15);
  Func_0200351c(8, 0, 5);
  Motion_SetVarCbAndRefresh(8, 2);
  Battle_WaitMode0(8);
  Func_0200351c(8, 0, 5);
  Func_0200067c(0);
  Func_02003534(8, 0x8000);
  Battle_WaitMode0(10);
  Func_02003544(8, 258, 30);
  /* FAKEMATCH: called as returning a value, which the native's argument order
   * shows (r1 before r0); scene_call.h declares it void for the scenes that
   * match so. */
  ((s32 (*)(s32, s32))UiText_OpenMessageAtObject)(8, 0);
  if (Event_AskYesNoWithActorAnimation(4, 0) == 0) {
    Battle_WaitMode0(20);
    Func_0200351c(8, 0, 5);
    ((struct EventCounters *)Ram_HeapSlots->eventWork)->count++;
  } else {
    Battle_WaitMode0(35);
    ((struct EventCounters *)Ram_HeapSlots->eventWork)->count++;
    Func_0200351c(8, 0, 5);
  }
  Motion_SetModeAndWaitAnimation(8, 3);
  Func_0200351c(8, 0, 5);
  Func_02003534_(8, 0x5000);
  Func_02003464();
}
