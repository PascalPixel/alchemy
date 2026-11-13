/// @file
/// The Game Ticket counter and every shop's stock.
// Part of shop.c.

#include "../shop_internal.h"
#include "gamepak/far_call/game.h"
#include "module/shop/shop.h"

void Shop_GiveGameTicket(void *window, void *itemWindow) {
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
  Shop_ShowKeeperMessageHidingCursor(message);
  MessageArgument_PushFar(shop->selectedItem, 2);
  Shop_ShowKeeperMessageHidingCursor(message + 1);
  redraw = 2;
  while (1) {
    if (redraw != 0) {
      unitId = shop->partyMemberIds[memberIndex];
      MenuCursor_Place(window, (memberIndex % 4) * 24 - 12, 0);
      shop->mode = 3;
      if (redraw == 2) {
        Func_080f8058(memberIndex / 4 * 4);
        wait(1);
      }
      Shop_DrawParty(window, memberIndex, shop->selectedItem);
      Shop_DrawCarriedItem(itemWindow, unitId, shop->selectedItem);
      redraw = 0;
    }
    if (gInput.pressed & 1) {
      slot = Inventory_AddItemFar(unitId, shop->selectedItem);
      if (slot < 0) {
        Sound_PlayFar(113);
        MessageArgument_PushFar(unitId, 1);
        MessageArgument_PushFar(shop->selectedItem, 2);
        if (Inventory_CountFar(unitId) == 15)
          Shop_ShowKeeperMessage((s32)MsgPackFull);
        else
          Shop_ShowKeeperMessage((s32)MsgCannotCarryItem);
        continue;
      }
      Inventory_RemoveFar(unitId, slot);
      Sound_PlayFar(101);
      Shop_ShowKeeperMessageHidingCursor((s32)MsgHereYouGo);
      Inventory_AddItemFar(unitId, shop->selectedItem);
      Party_AdjustShopCreditFar(-creditCost);
      Party_AdjustShopGiftsFar(1);
      break;
    } else if (gInput.pressed & 2) {
      Shop_ShowKeeperMessageHidingCursor((s32)MsgDontWantIt);
      Sound_PlayFar(113);
      break;
    } else {
      redraw = MenuGrid_MoveCursor(&memberIndex, shop->partyMemberCount, 4);
      wait(1);
    }
  }
  shop->selectedItem = savedSelectedItem;
}

s32 ShopStock_GetRowCount(void) { return 32; }

void ShopStock_AddArtifacts(s32 rowNo) {
  s32 flag = rowNo + 0x400;
  s32 i;

  if (GameFlag_Test(flag) == 0) {
    GameFlag_SetBit(flag);
#if TLA_PATCH_1
    // From patch 1 an empty entry is skipped; the Japanese release stops at the
    // row's first one.
    for (i = 0; i <= 7; i++) {
      s16 value = gData_0810c3f4[rowNo][24 + i];

      if (value != 0)
        Item_AdjustCounterFar(value, 1);
    }
#else
    for (i = 0; i <= 7; i++) {
      s16 value = gData_0810c3f4[rowNo][24 + i];

      if (value == 0)
        break;
      Item_AdjustCounterFar(value, 1);
    }
#endif
  }
}

/// Copies the shop's ordinary stock, up to 24 items, to `output` with a 0
/// after, and returns how many. Called by Shop_Run for Buy.
s32 ShopStock_CopyItems(s32 stockRow, s16 *output) {
  const s16 *src;
  s16 *dst;
  s32 count;

  count = 0;
  if (gData_0810c3f4[stockRow][0] != 0) {
    dst = output;
    src = gData_0810c3f4[stockRow];
    do {
      // The row's halfword read through a volatile pointer, a load apart from
      // the end test's; as *src, 41 of 28 instructions differ in tla-en.
      *dst = *(const volatile s16 *)src;
      count++;
      src++;
      dst++;
      if (count > 23)
        break;
    } while (*src != 0);
  }
  output[count] = 0;
  return count;
}

s32 ShopStock_GetType(s32 index) {
  s32 offset = index << 5;
  u8 *table = (u8 *)gData_0810c3f4;

  offset += index;
  offset <<= 1;
  offset += 64;
  return *(s16 *)(table + offset);
}
