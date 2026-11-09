#include "shrine_of_the_sea_god.h"
#include "text/msg_ids.h"

extern const u8 gData_02003c10[];

void Func_020004b0(void) {
  struct ShrineOfTheSeaGodEventWork *work = Ram_HeapSlots->eventWork;

  Func_020039d8();
  Event_ResolvePendingActions(0);
  FieldMessage_Show((s32)MsgFieldStatueLikeDailaShrine, 1);
  work->unk15a = 1;
  Func_020039e0();
}

void Func_020004e0(void) {
  Func_020039d8();
  Event_ResolvePendingActions(0);
  Actor_Stop(11);
  ShrineOfTheSeaGod_Jump(11, 0x100, 20);
  Actor_SetAngleToward(11, gMainRoutineWork.currentOwner, 0);
  Event_SetMessageId((s32)MsgFieldStartledCompanion);
  Event_ShowMessageAndWait(11, 0);
  Event_ShowMessageAndWait(11, 0);
  Actor_SetModeById(11, 4);
  Event_ShowMessageThenWait(11, 0, 20);
  Actor_SetMovementTypeById(11, gData_02003c10);
  Func_020039e0();
}

void Func_02000554(void) {
  struct ShrineOfTheSeaGodEventWork *work = Ram_HeapSlots->eventWork;

  work->unk15a = 1;
}
