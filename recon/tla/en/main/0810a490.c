// Measured 2026-10-26 under the Makefile's options (-mthumb-slow-loads), differ
// 10 of 196 in tla-en. First difference: the native issues `sub sp, #8` earlier
// (row 12) than this does (row 22).
// Draft, differ 35 of 196: the shop's Game Ticket gift, as ☀️'s
// Func_0810a490 (games/tbs/src/shop/shop.c), with ⚓️'s party cursor
// helper (Func_08108690) in place of the left and right tests. Left: the
// prologue's order (shop load, the zero store to memberIndex and the saved
// item's store sit two to six slots apart), the B test reusing the pressed
// word already in a register where the native keeps gInput's address in r1
// and reloads (ands into r2 kills it), and one constant's halves at the end;
// instruction order and that one CSE choice. Tried: the locals in other
// orders (35).
#include "types.h"
#include "input.h"
#include "ram_buffer.h"
#include "adventure.h"

struct ShopRuntime {
  u8 unk000[0x4ca];
  s16 partyMemberIds[25];
  u16 selectedItem;
  u8 unk4fe[6];
  s8 partyMemberCount;
  u8 mode;
};

extern const s16 Data_0810c38e[];
// Unnamed in the catalog yet: 4724, 4687 and 4695 take placeholders here.
extern u8 Msg04724[];
extern u8 Msg04687[];
extern u8 Msg04695[];
extern u8 MsgHereYouGo[];
extern u8 MsgDontWantIt[];

void UiText_DrawQuantity(s32 value, s32 slot);
void Func_0810857c(s32 message);
void Func_081084f4(s32 message);
void Shop_PlaceCursor(void *window, s32 x, s32 y);
void Func_080f8058(s32 first);
void WaitFrames(s32 frames);
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
  u32 savedSelectedItem = shop->selectedItem;
  s32 memberIndex = 0;
  s32 unitId = 0;
  s32 creditCost = Data_0810c38e[gAdventure.counterC];
  s32 redraw;
  s32 slot;
  s32 message;

  if (creditCost > gAdventure.counterB)
    return;
  shop->selectedItem = 228;
  UiText_DrawQuantity(228, 2);
  message = (s32)Msg04724;
  Func_0810857c(message);
  UiText_DrawQuantity(shop->selectedItem, 2);
  Func_0810857c(message + 1);
  redraw = 2;
  while (1) {
    if (redraw != 0) {
      unitId = shop->partyMemberIds[memberIndex];
      Shop_PlaceCursor(window, (memberIndex % 4) * 24 - 12, 0);
      shop->mode = 3;
      if (redraw == 2) {
        Func_080f8058(memberIndex / 4 * 4);
        WaitFrames(1);
      }
      Func_0810928c(window, memberIndex, shop->selectedItem);
      Func_081095b0(itemWindow, unitId, shop->selectedItem);
      redraw = 0;
    }
    if (gInput.pressed & 1) {
      slot = Inventory_AddItemFar(unitId, shop->selectedItem);
      if (slot < 0) {
        Sound_PlayFar(113);
        UiText_DrawQuantity(unitId, 1);
        UiText_DrawQuantity(shop->selectedItem, 2);
        if (Inventory_CountFar(unitId) == 15)
          Func_081084f4((s32)Msg04687);
        else
          Func_081084f4((s32)Msg04695);
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
      WaitFrames(1);
    }
  }
  shop->selectedItem = savedSelectedItem;
}
