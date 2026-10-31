#include "types.h"
#include "ram_buffer.h"
#include "event_work.h"
#include "main_routine_work.h"
#include "edition.h"

#if LANGUAGE == LANGUAGE_GERMAN
#define ITEM_GIVEN_MODE 1
#else
#define ITEM_GIVEN_MODE 3
#endif

struct ItemHead {
  u8 unk00[3];
  u8 flags;
};

extern u8 MsgItemReceived;
extern u8 MsgItemGiven;
extern u8 MsgItemCannotCarry;
extern u8 MsgItemDropPrompt;

s32 PartyInventory_AddFar(s32 item);
void MessageArgument_PushFar(s32 value, s32 slot);
void FieldMessage_ShowFar(s32 message, s32 mode);
s32 Func_08108068(s32 *member, s32 *slot);
struct ItemHead *Item_Get(s32 item);
s32 Func_080d295c(s32 mode);
void Message_CloseFinishedWindowsFar(void);
s32 Item_AdjustCounterFar(s32 item, s32 delta);
void *Unit_Get(s32 owner);
s32 Inventory_GetQuantityFar(s32 member, s32 slot);
s32 Inventory_CountItemFar(s32 member, s32 item);
void Inventory_DiscardFar(s32 member, s32 slot);
void Unit_RefreshPsynergyFar(s32 member);
void Unit_CalculateFar(s32 member);
void Sound_PlayFar(s32 cue);

s32 PartyInventory_GiveItem(s32 item) {
  struct EventWork *work = Ram_HeapSlots->eventWork;
  s16 saved = work->messageId;
  s32 owner;
  s32 result;
  s32 count;
  s32 i;
  s32 member;
  s32 slot;

  owner = PartyInventory_AddFar(item);
  if (owner == -1) {
    MessageArgument_PushFar(item, 2);
    FieldMessage_ShowFar((s32)&MsgItemReceived, 1);
    FieldMessage_ShowFar((s32)&MsgItemCannotCarry, 1);
  retry:
    {
      FieldMessage_ShowFar((s32)&MsgItemDropPrompt, 1);
      result = Func_08108068(&member, &slot);
      if (result == -1) {
        if (Item_Get(item)->flags & 8) {
          MessageArgument_PushFar(item, 2);
          FieldMessage_ShowFar((s32)&MsgItemDropPrompt + 4, 1);
          goto retry;
        }
        MessageArgument_PushFar(item, 2);
        FieldMessage_ShowFar((s32)&MsgItemDropPrompt + 1, 5);
        result = Func_080d295c(1);
        Message_CloseFinishedWindowsFar();
        if (result != 0)
          goto retry;
        Item_AdjustCounterFar(item, 1);
        MessageArgument_PushFar(item, 2);
        FieldMessage_ShowFar((s32)&MsgItemDropPrompt + 2, 1);
        goto done;
      }
      Unit_Get(member);
      count = Inventory_GetQuantityFar(member, slot);
      if (Inventory_CountItemFar(member, item) > 29) {
        MessageArgument_PushFar(member, 1);
        MessageArgument_PushFar(item, 2);
        FieldMessage_ShowFar((s32)&MsgItemDropPrompt + 7, 1);
        goto retry;
      }
      for (i = 0; i < count; i++)
        Inventory_DiscardFar(member, slot);
      Unit_RefreshPsynergyFar(member);
      Unit_CalculateFar(member);
      owner = PartyInventory_AddFar(item);
      Sound_PlayFar(83);
      if (owner == gMainRoutineWork.currentOwner) {
        MessageArgument_PushFar(item, 2);
        FieldMessage_ShowFar((s32)&MsgItemReceived, 3);
      } else {
        MessageArgument_PushFar(item, 2);
        MessageArgument_PushFar(owner, 1);
#if (LANGUAGE != LANGUAGE_SPANISH) && (LANGUAGE != LANGUAGE_FRENCH) &&         \
    (LANGUAGE != LANGUAGE_ITALIAN)
        FieldMessage_ShowFar((s32)&MsgItemGiven, ITEM_GIVEN_MODE);
#endif
      }
    }
  done:
    work->messageId = saved;
    return owner;
  }
  Sound_PlayFar(83);
  MessageArgument_PushFar(item, 2);
  FieldMessage_ShowFar((s32)&MsgItemReceived, 3);
  if (owner != gMainRoutineWork.currentOwner) {
    MessageArgument_PushFar(item, 2);
    MessageArgument_PushFar(owner, 1);
    FieldMessage_ShowFar((s32)&MsgItemReceived + 1, 1);
  }
  work->messageId = saved;
  return owner;
}
