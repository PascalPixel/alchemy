#include "local.h"


void Func_02002318(void) {
  if (*(s8 *)((u8 *)Ram_HeapSlots->eventWork + 0xd30) == -1) {
    if (!Event_GetFlag(0x304))
      Actor_SetPosition(22, 0, 0);
    if (!Event_GetFlag(0x303))
      Actor_SetPosition(21, 0, 0);
  }
}

extern const u8 MsgBlazeStoneEmber[];

void Func_02004d40(void);
void Func_02004d48(void);
void Func_02004e58(s32 item, s32 mode);

void Func_02002364(void) {
  const u8 *message;

  Func_02004d40();
  FieldEvent_ResolvePendingActions(0);
  message = MsgBlazeStoneEmber;
  Message_ShowCentered(message, 1);
  FieldEvent_SetMessageId(message + 1);
  Event_OpenMessage(12, 0);
  if (FieldEvent_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) == 0) {
    Party_GiveItem(460, 0);
    if (FieldPartyInventory_FindOwner(460) == -1) {
      Func_02004d48();
      return;
    }
    Func_02004e58(460, 3);
    Actor_SetAnimation(12, 2);
    Event_SetFlag(0x9f9);
  }
  Func_02004d48();
}
