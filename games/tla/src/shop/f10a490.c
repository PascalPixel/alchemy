#include "types.h"
#include "input.h"
#include "ram_buffer.h"
#include "main_routine_work.h"

struct ShopRuntime {
  u8 unk000[0x138];
  s32 partyMemberScale[4];
  u8 unk148[0x382];
  s16 partyMemberIds[25];
  u16 selectedItem;
  u8 unk4fe[6];
  s8 partyMemberCount;
  u8 mode;
};

extern const s16 Data_0810c38e[];
// Unnamed in the catalog yet: 4724, 4687 and 4695 take placeholders here.
extern u8 MsgLetMeGiveYouThis[];
extern u8 MsgPackFull[];
extern u8 MsgCannotCarryItem[];
extern u8 MsgHereYouGo[];
extern u8 MsgDontWantIt[];

void MessageArgument_PushFar(s32 value, s32 slot);
void Func_0810857c(s32 message);
void Func_081084f4(s32 message);
void Shop_PlaceCursor(void *window, s32 x, s32 y);
void Func_080f8058(s32 first);
void wait(s32 frames);
void Func_0810928c(void *window, s32 memberIndex, s32 item);
void Func_081095b0(void *window, s32 unitId, s32 item);
s32 Inventory_AddItemFar(s32 unitId, s32 item);
void Inventory_RemoveFar(s32 unitId, s32 slot);
s32 Inventory_CountFar(s32 unitId);
void Sound_PlayFar(s32 cue);
void Party_AdjustShopCreditFar(s32 amount);
void Party_AdjustShopGiftsFar(s32 amount);
s32 Func_08108690(s32 *cursor, s32 count, s32 columns);

void Func_0810a490(void *window, void *itemWindow) {
  struct ShopRuntime *shop = Ram_HeapSlots->menuRuntime;
  u32 savedSelectedItem;
  s32 memberIndex;
  s32 unitId;
  s32 creditCost;
  s32 redraw;
  s32 slot;
  s32 message;

  memberIndex = 0;
  savedSelectedItem = shop->selectedItem;
  unitId = 0;
  creditCost = Data_0810c38e[gMainRoutineWork.counterC];

  if (creditCost > gMainRoutineWork.counterB)
    return;
  shop->selectedItem = 228;
  MessageArgument_PushFar(228, 2);
  message = (s32)MsgLetMeGiveYouThis;
  Func_0810857c(message);
  MessageArgument_PushFar(shop->selectedItem, 2);
  Func_0810857c(message + 1);
  redraw = 2;
  while (1) {
    if (redraw != 0) {
      unitId = shop->partyMemberIds[memberIndex];
      Shop_PlaceCursor(window, (memberIndex % 4) * 24 - 12, 0);
      shop->mode = 3;
      if (redraw == 2) {
        Func_080f8058(memberIndex / 4 * 4);
        wait(1);
      }
      Func_0810928c(window, memberIndex, shop->selectedItem);
      Func_081095b0(itemWindow, unitId, shop->selectedItem);
      redraw = 0;
    }
    if (gInput.pressed & 1) {
      slot = Inventory_AddItemFar(unitId, shop->selectedItem);
      if (slot < 0) {
        Sound_PlayFar(113);
        MessageArgument_PushFar(unitId, 1);
        MessageArgument_PushFar(shop->selectedItem, 2);
        if (Inventory_CountFar(unitId) == 15)
          Func_081084f4((s32)MsgPackFull);
        else
          Func_081084f4((s32)MsgCannotCarryItem);
        continue;
      }
      Inventory_RemoveFar(unitId, slot);
      Sound_PlayFar(101);
      Func_0810857c((s32)MsgHereYouGo);
      Inventory_AddItemFar(unitId, shop->selectedItem);
      Party_AdjustShopCreditFar(-creditCost);
      Party_AdjustShopGiftsFar(1);
      break;
    } else if (gInput.pressed & 2) {
      Func_0810857c((s32)MsgDontWantIt);
      Sound_PlayFar(113);
      break;
    } else {
      redraw = Func_08108690(&memberIndex, shop->partyMemberCount, 4);
      wait(1);
    }
  }
  shop->selectedItem = savedSelectedItem;
}
