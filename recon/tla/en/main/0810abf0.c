// Draft: the sanctum's service for one party member at a time, four a page:
// asks who to treat, shows the donation, and on yes and enough coins applies
// the service (BattleUnit_ResetStateByMode), plays the icon burst
// (Func_0810b1b4) and takes the coins, until nobody needs it or B. As ☀️'s
// Shop_ServeParty loop in Shop_ConfirmAct's family (games/tbs/src/shop/shop.c,
// after Shop_ConfirmAct), with ⚓️'s calls: Func_0810a9ac and Func_0810a9fc to
// show a message, Func_08108660 for the choice, Shop_DrawServiceParty for the party
// page, Func_08108690 for the page cursor and Func_08109188 for the coins.
// Draft, differ 167 of 297 (2026-10-19), the shape following the listing:
// the native keeps chooseAgain on the stack (sp + 4, its frame 16 bytes) and
// the work in r7, here chooseAgain takes r7 and the work r6.
#include "types.h"
#include "input.h"
#include "main_routine_work.h"
#include "ram_buffer.h"

struct ShopCursorAnchor {
  u8 unk00[5];
  u8 active;
};

struct ShopRuntime {
  u8 unk000[0x4ca];
  s16 partyMemberIds[8];
  u8 unk4da[2];
  struct ShopCursorAnchor *cursorAnchor;
  u8 unk4e0[0x24];
  s8 partyMemberCount;
  u8 mode;
  u8 unk506;
  s8 partyAction;
};

// Unnamed in the catalog yet: 4823 ("who to treat") and 4824 to 4828 (the
// donation and its answers) take placeholders here.
extern u8 Msg04823[], Msg04824[];

void Func_0810a9ac(s32 message);
void Func_0810a9fc(s32 message);
void *Window_CreateFar(s32 x, s32 y, s32 width, s32 height, s32 style);
void Func_080f8060(void *window, s32 a, s32 b);
s32 Shop_CanServe(s32 unitId, s32 serviceKind);
s32 Shop_ServicePrice(s32 unitId, s32 serviceKind);
void Shop_PlaceCursor(void *window, s32 x, s32 y);
void Func_080f8058(s32 first);
void wait(s32 frames);
void Shop_DrawServiceParty(void *window, s32 memberIndex);
void Shop_DrawSelectMsg(void *window, s32 unitId);
void Sound_PlayFar(s32 cue);
void WindowSprite_PrepareForRedrawFar(void *window);
void MessageArgument_PushFar(s32 value, s32 slot);
s32 Func_08108660(s32 value);
void Message_CloseFinishedWindowsFar(void);
void BattleUnit_ResetStateByMode(s32 unitId, s32 serviceKind);
void Func_0810b1b4(s32 member);
void Party_AdjustCoinsFar(s32 amount);
void Func_08109188(void);
s32 Shop_CountUnits(void);
s32 Func_08108690(s32 *cursor, s32 count, s32 columns);
void Menu_ReleaseEntryObjectsFar(void);
void Window_CloseFar(void *window, s32 style);

s32 Func_0810abf0(void) {
  struct ShopRuntime *shop = Ram_HeapSlots->menuRuntime;
  s32 memberIndex = 0;
  s32 chooseAgain = 0;
  s32 serviceKind = shop->partyAction;
  s32 redraw = 1;
  void *listWindow;
  void *priceWindow;
  s32 unitId;
  s32 price;
  s32 messageBase;

  Func_0810a9ac((s32)Msg04823);
  listWindow = Window_CreateFar(1, 12, 13, 3, 2);
  shop->cursorAnchor->active = 4;
  shop->mode = redraw;
  Func_080f8060(listWindow, 2, 0);
  priceWindow = Window_CreateFar(1, 16, 23, 3, 2);
  unitId = 0;
  for (memberIndex = 0; memberIndex < shop->partyMemberCount; memberIndex++) {
    unitId = shop->partyMemberIds[memberIndex];
    if (Shop_CanServe(unitId, serviceKind) != 0)
      break;
  }
  redraw = 2;
  while (1) {
    if (chooseAgain != 0) {
      chooseAgain = 0;
      Func_0810a9ac((s32)Msg04823);
      redraw = 2;
      for (memberIndex = chooseAgain; memberIndex < shop->partyMemberCount;
           memberIndex++) {
        unitId = shop->partyMemberIds[memberIndex];
        if (Shop_CanServe(unitId, serviceKind) != 0)
          break;
      }
    }
    if (redraw != 0) {
      unitId = shop->partyMemberIds[memberIndex];
      Shop_PlaceCursor(listWindow, (memberIndex % 4) * 24 - 12, 0);
      shop->mode = 3;
      if (redraw == 2) {
        Func_080f8058(memberIndex / 4 * 4);
        wait(1);
      }
      Shop_DrawServiceParty(listWindow, memberIndex);
      Shop_DrawSelectMsg(priceWindow, unitId);
      redraw = 0;
    }
    if (gInput.pressed & 1) {
      wait(1);
      price = Shop_ServicePrice(unitId, serviceKind);
      if (Shop_CanServe(unitId, serviceKind) == 0) {
        Sound_PlayFar(113);
        continue;
      }
      WindowSprite_PrepareForRedrawFar(listWindow);
      MessageArgument_PushFar(unitId, 1);
      MessageArgument_PushFar(price, 5);
      messageBase = (s32)Msg04824;
      Func_0810a9ac(messageBase);
      if (Func_08108660(0) != 0) {
        Func_0810a9fc(messageBase + 2);
        chooseAgain = 1;
        continue;
      }
      if ((u32)price > gMainRoutineWork.coins) {
        Sound_PlayFar(113);
        Func_0810a9fc(messageBase + 1);
        chooseAgain = 1;
        continue;
      }
      MessageArgument_PushFar(unitId, 1);
      Func_0810a9ac(messageBase + 3);
      Message_CloseFinishedWindowsFar();
      BattleUnit_ResetStateByMode(unitId, serviceKind);
      Func_0810b1b4(memberIndex % 4);
      Party_AdjustCoinsFar(-price);
      Func_08109188();
      MessageArgument_PushFar(unitId, 1);
      Func_0810a9ac(messageBase + 4);
      if (Shop_CountUnits() != 0) {
        chooseAgain = 1;
        continue;
      }
      break;
    } else if (gInput.pressed & 2) {
      Sound_PlayFar(113);
      break;
    } else {
      redraw = Func_08108690(&memberIndex, shop->partyMemberCount, 4);
      wait(1);
    }
  }
  Menu_ReleaseEntryObjectsFar();
  Window_CloseFar(priceWindow, 2);
  Window_CloseFar(listWindow, 2);
  wait(1);
  return 0;
}
