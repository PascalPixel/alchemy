#include "local.h"
#include "call.h"

void Func_02004da0(s32 id, s32 x, s32 z);

void Func_02002318(void) {
  /* FAKEMATCH(barrier): SPLIT_CONSTANT builds each constant as the game does, a
   * shifted byte plus an addend; as plain constants, 4 of 34 instructions
   * differ in tla-en. */
  if (*(s8 *)((u8 *)Ram_HeapSlots->eventWork + 0xd30) == -1) {
    if (!GameFlag_Test(0x304))
      Func_02004da0(22, 0, 0);
    if (!GameFlag_Test(SPLIT_CONSTANT(0x303)))
      Func_02004da0(21, 0, 0);
  }
}

extern const u8 MsgBlazeStoneEmber[];

void FieldMessage_Show(const u8 *message, s32 mode);
s32 FieldMessage_ShowAtActor(s32 id, s32 flags);
s32 Event_AskYesNoWithActorAnimation(s32 id, s32 flags);
s32 PartyInventory_GiveItem(s32 item, s32 count);
s32 PartyInventory_FindOwner(s32 item);
void Actor_SetModeById(u32 id, s32 mode);
void Func_02004d40(void);
void Func_02004d48(void);
void Func_02004dd0(const u8 *message);
void Func_02004e58(s32 item, s32 mode);

void Func_02002364(void) {
  /* FAKEMATCH(barrier): SPLIT_CONSTANT builds each constant as the game does, a
   * shifted byte plus an addend; as plain constants, 4 of 49 instructions
   * differ in tla-en. */
  const u8 *message;

  Func_02004d40();
  Func_02004e90(0);
  message = MsgBlazeStoneEmber;
  FieldMessage_Show(message, 1);
  Func_02004dd0(message + 1);
  FieldMessage_ShowAtActor(12, 0);
  if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) == 0) {
    Value2((s32 (*)())PartyInventory_GiveItem, 460, 0);
    if (Value1((s32 (*)())PartyInventory_FindOwner, 460) == -1) {
      Func_02004d48();
      return;
    }
    Func_02004e58(460, 3);
    Actor_SetModeById(12, 2);
    GameFlag_SetBit(SPLIT_CONSTANT(0x9f9));
  }
  Func_02004d48();
}
