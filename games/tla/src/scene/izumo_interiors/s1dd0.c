#include "types.h"
#include "main_routine_work.h"
#include "scene_tables.h"
#include "event_inline.h"

void Func_020021c4();
void Func_020021cc();
void Func_020022e4();
void Func_020022ec();
void Func_020022f4();
void Func_020022fc();
void PartyInventory_Remove();
extern u8 MsgAgainYouHave[];
extern u8 MsgAhCouldIt[];

void Func_02001dd0(void) {
  s32 t9, t21, t31;

  Func_020021c4();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)&MsgAhCouldIt);
  Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 144, 536);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_NORTH, 0);
  Event_Wait(15);
  Actor_ShowEmote(15, 0x100, 60);
  Event_OpenMessage(15, 0);
  t9 = Event_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0);
  if (t9 != 0)
    goto L_02009e3a;
  Event_SetMessageId(((s32)&MsgAhCouldIt + 1));
  Event_ShowMessageAndWait(15, 0);
  goto L_02009e48;
L_02009e3a:
  Event_SetMessageId(((s32)&MsgAhCouldIt + 2));
  Event_ShowMessageAndWait(15, 0);
L_02009e48:
  Func_080d47b4(15, 0x109, 60);
  Event_SetMessageId((s32)&MsgAgainYouHave);
  Event_ShowMessageAndWait(15, 0);
  Actor_ShowEmote(15, 0x105, 120);
  Func_080d47b4(15, 0x106, 60);
  Event_ShowMessageAndWait(15, 0);
  Func_020022e4();
  t21 = ((s32)Actor_GetById(15));
  Func_020022fc(t21, 2);
  Sound_Play(201);
  Event_Wait(60);
  Actor_SetPosition(24, PIXELS(144), PIXELS(520));
  Actor_Jump(24, 6, 0);
  Actor_SetDestination(24, 160, 520);
  Actor_WaitForMoveById(24);
  Actor_ArmCallback(24, FACING_SOUTH, 0);
  Event_Wait(60);
  t31 = ((s32)Actor_GetById(15));
  Func_020022fc(t31, 0);
  Func_020022f4();
  Func_020022ec();
  Event_Wait(30);
  Event_Wait(30);
  Actor_SetAngleToward(15, 24, 0);
  Event_Wait(30);
  Event_ShowMessageAndWait(15, 0);
  Actor_SetAngleToward(15, gMainRoutineWork.currentOwner, 0);
  Event_Wait(60);
  Event_ShowMessageAndWait(15, 0);
  Event_Wait(30);
  Actor_SetModeById(15, 4);
  Event_Wait(60);
  Event_ShowMessageAndWait(15, 0);
  Actor_TurnToFaceEachOther(15, 24, 0);
  Event_ShowMessageAndWait(15, 0);
  Event_Wait(60);
  Actor_Jump(24, 4, 20);
  Event_Wait(60);
  Actor_SetAngleToward(15, gMainRoutineWork.currentOwner, 0);
  Actor_SetAngleToward(24, gMainRoutineWork.currentOwner, 0);
  Event_Wait(60);
  Event_ShowMessageAndWait(15, 0);
  Actor_Jump(24, 2, 20);
  Func_080d7788(24, 2, 11);
  Func_020021cc();
  Func_020021c4();
  Event_ResolvePendingActions(0);
  Actor_SetAngleToward(gMainRoutineWork.currentOwner, 15, 0);
  Event_Wait(60);
  Actor_StartRepeatedMotion(15, 2);
  Event_Wait(30);
  Event_ShowMessageAndWait(15, 0);
  Event_Wait(60);
  GameFlag_SetBit(0x92c);
  PartyInventory_Remove(0x1c3);
  Func_020021cc();
}
