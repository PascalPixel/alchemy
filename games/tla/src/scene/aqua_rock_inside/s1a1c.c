#include "types.h"
#include "scene_tables.h"
#include "main_routine_work.h"
#include "event_inline.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

void Func_0200124c();
void Func_02001620();
void Func_02003d7c();
void Func_02003d84();
void Func_02003de4();
void Party_LearnAbility();
extern u8 MsgCheckedTheTabletIt[];
extern u8 MsgWhatWentWrong[];

void Func_02001a1c(void) {
  s32 t4, t59;

  Func_02003d7c();
  Event_ResolvePendingActions(0);
  FieldMessage_Show((s32)&MsgCheckedTheTabletIt, 1);
  t4 = Event_GetFlag(0x9ae);
  if (t4 == 0)
    goto L_02009a46;
  Func_02003d84();
  goto L_02009c56;
L_02009a46:
  Event_SetMessageId((s32)&MsgCheckedTheTabletIt);
  FIELD_AT((FIELD_AT(0x3000000, s32, 108) + 0x1c4), s16, 0) =
      (FIELD_AT((FIELD_AT(0x3000000, s32, 108) + 0x1c4), u16, 0) + 1);
  Func_02003de4(7, gMainRoutineWork.currentOwner);
  Event_Wait(1);
  Actor_WalkTo(gMainRoutineWork.currentOwner, 256, 184);
  Actor_ResetAndSetPositionInMode2(7, 274, 184);
  Actor_WaitForMoveById(gMainRoutineWork.currentOwner);
  Actor_WaitForMoveById(7);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_EAST, 0);
  Actor_FaceDirection(7, FACING_WEST, 0);
  Event_Wait(5);
  Event_ShowMessageAndWait(7, 0);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 3);
  Event_Wait(30);
  Actor_SetPositionAndReset(gMainRoutineWork.currentOwner, 248, 184);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_NORTHEAST, 0);
  Event_Wait(20);
  Actor_FaceDirection(7, FACING_WEST, 0);
  Event_Wait(20);
  Actor_SetModeById(7, 3);
  Event_Wait(30);
  Actor_FaceDirection(7, FACING_NORTH, 0);
  Event_Wait(20);
  Actor_SetSpeed(7, 0x10000, 0x8000);
  Actor_WalkToAndWait(7, 264, 168);
  Actor_ArmCallback(7, FACING_NORTH, 0);
  Event_Wait(20);
  FieldMessage_Show(((s32)&MsgCheckedTheTabletIt + 2), 1);
  Func_02001620(14, 7);
  Party_LearnAbility(7, 138);
  GameFlag_SetBit(0x9ae);
  Sound_Play(107);
  Event_Wait(30);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_SOUTH, 0);
  Actor_ArmCallback(7, FACING_SOUTH, 0);
  Event_Wait(10);
  Actor_ShowEmote(gMainRoutineWork.currentOwner, 0x100, 0);
  Actor_ShowEmote(7, 0x100, 0);
  Func_0200124c();
  Sound_Play(0x186);
  Event_Wait(30);
  Event_SetMessageId((s32)&MsgWhatWentWrong);
  Actor_ShowEmote(7, 0x105, 20);
  Actor_TurnToFaceEachOther(gMainRoutineWork.currentOwner, 7, 0);
  Event_Wait(10);
  Event_ShowMessageAndWait(7, 0);
  Func_080d47b4(7, 0x100, 20);
  Event_ShowMessageAndWait(7, 0);
  Event_Wait(10);
  Actor_SetModeById(7, 3);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 3);
  Event_Wait(20);
  Actor_SetSpeed(7, 0x13333, 0x9999);
  Actor_SetModeById(7, 2);
  t59 = ((s32)Actor_GetById(gMainRoutineWork.currentOwner));
  if (t59 == 0)
    goto L_02009c42;
  Actor_ResetAndSetPosition(7, FIELD_AT(t59, s16, 10), FIELD_AT(t59, s16, 18));
L_02009c42:
  Actor_WaitForMoveById(7);
  Actor_SetHorizontalPositionWithTerrain(7, 0, 0);
  Func_02003d84();
L_02009c56:
}
