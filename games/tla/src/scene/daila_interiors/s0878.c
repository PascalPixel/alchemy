#include "types.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "item.h"
#include "text/msg_ids.h"

struct DailaEventWork {
  u8 unk000[0x1c4];
  s16 message;
};

s32 PartyInventory_Add(s32 item);
void Func_02000ccc(s32 value, s32 slot);
struct ItemDefinition *Func_02000ce4(s32 item);
s32 Func_02000df4(s32 *member, s32 *slot);
s32 Func_02000dbc(s32 mode);
void Func_02000dfc(s32 cue);
void FieldMessage_Show(s32 msg, s32 mode);
void Message_CloseFinishedWindows(void);
u8 *Unit_Get(s32 owner);
void Unit_Calculate(s32 owner);
s32 Inventory_GetQuantity(s32 member, s32 slot);
s32 Inventory_CountItem(s32 member, s32 item);
void Inventory_Discard(s32 member, s32 slot);
void Unit_RefreshPsynergy(s32 member);

s32 Func_02000878(s32 item, s32 unused) {
  struct DailaEventWork *work;
  s32 saved;
  s32 owner;
  s32 result;
  s32 count;
  s32 i;
  s32 member;
  s32 slot;

  work = Ram_HeapSlots->eventWork;
  saved = work->message;
  owner = PartyInventory_Add(item);
  if (owner == -1) {
    Func_02000ccc(item, 2);
    FieldMessage_Show((s32)MsgItemReceived, 1);
    FieldMessage_Show((s32)MsgItemCannotCarry, 1);
  retry:
    FieldMessage_Show((s32)MsgItemDropPrompt, 1);
    result = Func_02000df4(&member, &slot);
    if (result == -1) {
      if (Func_02000ce4(item)->flags & 8) {
        Func_02000ccc(item, 2);
        FieldMessage_Show((s32)MsgItemDropPrompt + 4, 1);
        goto retry;
      }
      Func_02000ccc(item, 2);
      FieldMessage_Show((s32)MsgItemDropPrompt + 1, 5);
      result = Func_02000dbc(1);
      Message_CloseFinishedWindows();
      if (result != 0)
        goto retry;
      Func_02000ccc(item, 2);
      FieldMessage_Show((s32)MsgItemDropPrompt + 2, 1);
      work->message = saved;
      return owner;
    } else {
      Unit_Get(member);
      count = Inventory_GetQuantity(member, slot);
      if (Inventory_CountItem(member, item) > 29) {
        Func_02000ccc(member, 1);
        Func_02000ccc(item, 2);
        FieldMessage_Show((s32)MsgItemDropPrompt + 7, 1);
        goto retry;
      }
      for (i = 0; i < count; i++)
        Inventory_Discard(member, slot);
      Unit_RefreshPsynergy(member);
      Unit_Calculate(member);
      owner = PartyInventory_Add(item);
      Func_02000dfc(83);
      if (owner == gMainRoutineWork.currentOwner) {
        Func_02000ccc(item, 2);
        FieldMessage_Show((s32)MsgItemReceived, 3);
      } else {
        Func_02000ccc(item, 2);
        Func_02000ccc(owner, 1);
        FieldMessage_Show((s32)MsgItemGiven, 3);
      }
      work->message = saved;
      return owner;
    }
  } else {
    Func_02000dfc(83);
    Func_02000ccc(item, 2);
    FieldMessage_Show((s32)MsgItemReceived, 3);
    if (owner != gMainRoutineWork.currentOwner) {
      Func_02000ccc(item, 2);
      Func_02000ccc(owner, 1);
      FieldMessage_Show((s32)MsgItemReceived + 1, 1);
    }
    work->message = saved;
  }
  return owner;
}
