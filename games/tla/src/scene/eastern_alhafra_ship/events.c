#include "types.h"
#include "scene_tables.h"
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "event_work.h"
#include "text/msg_ids.h"
#include "event_inline.h"

void Func_02003148();

void Func_0200008c(void) {
  s32 v5;
  struct Actor *object0;
  s32 v6;
  struct Actor *object1;

  GameFlag_SetBit(0x901);
  v5 = &gMainRoutineWork;
  v5 = (v5 + 0x214);
  object0 = Actor_GetById(*(s32 *)((u8 *)v5 + 0));
  v6 = 192;
  v6 = (v6 << 11);
  *(s32 *)((u8 *)object0 + 40) = v6;
  Actor_SetSpeedParameters(*(s32 *)((u8 *)v5 + 0), 0x20000, 0x10000);
  Actor_WalkTo(*(s32 *)((u8 *)v5 + 0), 136, 488);
  Event_Wait(10);
  Actor_SetDestination(15, 120, 472);
  Actor_SetPositionAndCommit(14, 136, 475);
  object1 = Actor_GetById(15);
  *(s32 *)((u8 *)object1 + 40) = v6;
  Sound_Play(0x237);
  Actor_WaitForMoveById(*(s32 *)((u8 *)v5 + 0));
  Actor_SetModeById(*(s32 *)((u8 *)v5 + 0), 1);
  Actor_ArmCallback(*(s32 *)((u8 *)v5 + 0), FACING_NORTH, 0);
  Event_Wait(20);
  Actor_WaitForMoveById(15);
  Sound_Play(159);
  return;
}

void Func_02000128(s32 arg0) {
  Func_02003148(arg0, 0x1c9, 0x902);
  return;
}

void Field_StartPaletteTransition();
void Func_02003130();
void Func_02003138();
void Func_02003268();
void Func_02003278();
void Func_02003280();
void Func_02003288();
void Func_02003290();

void Func_0200013c(s32 id) {
  struct Actor *object = Actor_GetById(12);
  u8 *flags;
  s32 i;

  Func_02003130();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgGooGooGa);
  Event_ShowMessageAndWait(11, 0);
  Func_02003268(8, 0x1210);
  Func_02003278();
  Func_02003290((s32)Actor_GetById(id), 2);
  Sound_Play(201);
  Event_Wait(30);
  flags = &object->motionFlags;
  *flags = 4;
  for (i = 39; i >= 0; i--) {
    object->y += 0x3333;
    Event_Wait(1);
  }
  Event_Wait(70);
  Func_02003290((s32)Actor_GetById(id), 0);
  Func_02003288();
  Func_02003280();
  Field_StartPaletteTransition(8);
  *flags = 3;
  object->gravity = 0x8000;
  Event_Wait(5);
  Sound_Play(132);
  if (Event_GetFlag(0x8ae) == 0) {
    GameFlag_SetBit(0x8ae);
    Event_Wait(30);
    Actor_ArmCallback(10, 0x3000, 0);
    Event_Wait(30);
    Motion_SetVarCbAndRefresh(10, 2);
    Event_Wait(10);
    Event_ShowMessageThenWait(10, 0, 10);
    Actor_FaceDirection(10, 0xafff, 0);
  }
  Func_02003138();
}
