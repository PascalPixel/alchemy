/// @file
/// Scene code of overlay 0x652, in ROM order, from item's offset up to the next object the overlay links.

#include "types.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "text/msg_ids.h"
#include "daila.h"
#include "item.h"

struct DailaActor {
  u8 unk00[0xc];
  s32 y;
  u8 unk10[8];
  s32 speed18;
  s32 speed1c;
  u8 unk20[8];
  s32 unk28;
  u8 unk2c[0x1c];
  s32 unk48;
  u8 unk4c[9];
  u8 kind55;
  u8 unk56[3];
  u8 flags59;
  u8 unk5a[0x12];
  s32 unk6c;
};

struct DailaActor *Actor_GetById(s32 id);
s32 PartyInventory_Add(s32 item);
s32 Func_02000d14(s32 item);
void Func_02000da4(struct DailaActor *object, s32 mode);
void Func_02000dfc(s32 cue);
void Func_02000ccc(s32 value, s32 slot);
void Func_02000c9c(struct DailaActor *object);
s32 Func_02000878(s32 item, s32 mode);

void Func_02000730(void) {
  s32 saved48;
  s32 saved28;
  s32 savedY;
  s32 item;

  saved48 = Actor_GetById(9)->unk48;
  item = 458;
  saved28 = Actor_GetById(9)->unk28;
  savedY = Actor_GetById(9)->y;
  Import_EventStart();
  Import_EventResolvePendingActions(0);
  if (Func_02000d14(item) >= 0) {
    Func_02000da4(Actor_GetById(9), 3);
    Func_02000dfc(83);
    Func_02000ccc(item, 2);
    FieldMessage_Show((s32)MsgItemReceived, 1);
    PartyInventory_Add(item);
    Func_02000d4c(9, 0, 0);
    Func_02000d4c(13, 232 << 16, 194 << 16);
    GameFlag_SetBit(0xa20);
  } else {
    Func_02000da4(Actor_GetById(9), 3);
    Func_02000dfc(83);
    if (Func_02000878(item, 0) != -1) {
      Func_02000d4c(9, 0, 0);
      Func_02000d4c(13, 232 << 16, 196 << 16);
      GameFlag_SetBit(0xa20);
    } else {
      Func_02000d4c(9, 0, 0);
      FieldEvent_Wait(1);
      Actor_GetById(9)->unk28 = saved28;
      Actor_GetById(9)->y = savedY;
      Actor_GetById(9)->unk48 = saved48;
      Actor_GetById(9)->unk6c = 0;
      Func_02000c9c(Actor_GetById(9));
      Import_ActorSetSpritePriority(9, 1);
      Actor_GetById(9)->kind55 = 2;
      Actor_GetById(9)->speed1c = 0x13333;
      Actor_GetById(9)->speed18 = 0x13333;
      Actor_GetById(9)->flags59 |= 4;
      Daila_ObjPlace(9, 232 << 16, 196 << 16);
    }
  }
  Import_EventEnd();
}

struct DailaEventWork {
  u8 unk000[0x1c4];
  s16 message;
};

struct ItemDefinition *Func_02000ce4(s32 item);
s32 Func_02000df4(s32 *member, s32 *slot);
s32 Func_02000dbc(s32 mode);
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
