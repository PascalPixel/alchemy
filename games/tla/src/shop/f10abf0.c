#include "types.h"
#include "edition.h"
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
extern u8 MsgWhoShallIRevive[], MsgReviving[];

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

// Italian creates no price window and passes 0 where the others pass it, a
// layout difference of that language's edition.
// XXX: what Italian shows in its place; the Italian render of the sanctum
// would settle it.
#define SERVICE_PRICE_WINDOW (LANGUAGE != LANGUAGE_ITALIAN)

// The price window's left column and width in tiles, sized to each
// language's text.
#if LANGUAGE == LANGUAGE_GERMAN
#define SERVICE_PRICE_WINDOW_X 0
#define SERVICE_PRICE_WINDOW_WIDTH 30
#elif LANGUAGE == LANGUAGE_FRENCH
#define SERVICE_PRICE_WINDOW_X 1
#define SERVICE_PRICE_WINDOW_WIDTH 25
#else
#define SERVICE_PRICE_WINDOW_X 1
#define SERVICE_PRICE_WINDOW_WIDTH 23
#endif

// Matches: differ 0 in tla-it and in the five editions that already link
// games/tla/src/shop/f10abf0.c (11 November 2026). One source for all six,
// and it needs neither that file's pin and messageBase temporary nor the
// Italian draft's firstMessage: every local takes its default at the top, in
// this order, and memberIndex is zeroed again after the windows.
// How the listing's zeros come out of the defaults (rounds 1 to 9 held
// tla-it at 8 of 282 by zeroing after the windows instead):
// - listWindow's NULL comes first, so cse stores memberIndex's first 0 from
//   listWindow's register, r11. A store from a high register is reloaded
//   through a low one, the function's first reload, so r0: reload registers
//   go round r0 to r3 from one instruction to the next, which is why redraw's
//   1 then takes r1 and the 0x507 offset r2. listWindow is set again before
//   any read, so its own mov from r0 is deleted after reload; only
//   movs r0, #0 and the store remain.
// - In Italian priceWindow's NULL is the second zero. cse1 stores it for the
//   second memberIndex = 0 while its uses in the loop still exist; gcse then
//   turns those uses into constants, which leaves it local to the first
//   block and alive across the three calls: local-alloc gives it r5, ahead
//   of global's choice for shop. unitId's 0 beside it is reloaded as a copy
//   of r5, and both, with chooseAgain's 0, sink below the last call in
//   sched2.
// - With a price window the second store's zero is a temporary of its own,
//   local to the first block across its four calls: local-alloc gives it r6
//   (r5 holds the windows' style, 2), which leaves global one register short,
//   so chooseAgain and priceWindow go to the stack and chooseAgain's 0 is
//   stored from r6.
s32 Func_0810abf0(void) {
  struct ShopRuntime *shop = Ram_HeapSlots->menuRuntime;
  void *priceWindow;
  s32 redraw;
  s32 serviceKind;
  void *listWindow;
  s32 memberIndex;
  s32 unitId;
  s32 chooseAgain;
  s32 price;
  s32 messageBase;

  // Default: neither window exists yet; Italian's price window never does.
  listWindow = NULL;
  priceWindow = NULL;
  // DEAD: memberIndex is set again after the windows before any read.
  memberIndex = 0;
  unitId = 0;
  chooseAgain = 0;
  redraw = 1;
  {
    s8 *partyAction = &shop->partyAction;
    /* FAKEMATCH(temporary): partyAction read through a pointer to it, so
     * memberIndex's 0 is stored before it is read; as serviceKind =
     * shop->partyAction, 4 of 297 instructions differ in tla-en. */
    serviceKind = *partyAction;
  }
  Func_0810a9ac((s32)MsgWhoShallIRevive);

  listWindow = Window_CreateFar(1, 12, 13, 3, 2);
  shop->cursorAnchor->active = 4;
  shop->mode = redraw;
  Func_080f8060(listWindow, 2, 0);
#if SERVICE_PRICE_WINDOW
  priceWindow = Window_CreateFar(SERVICE_PRICE_WINDOW_X, 16,
                                 SERVICE_PRICE_WINDOW_WIDTH, 3, 2);
#endif
  memberIndex = 0;
  while (memberIndex < shop->partyMemberCount) {
    unitId = shop->partyMemberIds[memberIndex];
    if (Shop_CanServe(unitId, serviceKind) != 0)
      break;
    memberIndex++;
  }

  redraw = 2;
loop:
  if (chooseAgain != 0) {
    chooseAgain = 0;
    Func_0810a9ac((s32)MsgWhoShallIRevive);
    redraw = 2;
    memberIndex = 0;
    while (memberIndex < shop->partyMemberCount) {
      unitId = shop->partyMemberIds[memberIndex];
      if (Shop_CanServe(unitId, serviceKind) != 0)
        break;
      memberIndex++;
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

  if ((gInput.pressed & 1) != 0) {
    wait(1);
    price = Shop_ServicePrice(unitId, serviceKind);
    if (Shop_CanServe(unitId, serviceKind) == 0) {
      Sound_PlayFar(113);
      goto loop;
    }
    WindowSprite_PrepareForRedrawFar(listWindow);
    MessageArgument_PushFar(unitId, 1);
    MessageArgument_PushFar(price, 5);
    messageBase = (s32)MsgReviving;
    Func_0810a9ac(messageBase);
    if (Func_08108660(0) != 0) {
      Func_0810a9fc(messageBase + 2);
      chooseAgain = 1;
      goto loop;
    }
    if ((u32)price > gMainRoutineWork.coins) {
      Sound_PlayFar(113);
      Func_0810a9fc(messageBase + 1);
      chooseAgain = 1;
      goto loop;
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
      goto loop;
    }
    goto done;
  }
  if ((gInput.pressed & 2) != 0) {
    Sound_PlayFar(113);
    goto done;
  }
  {
    s32 count = shop->partyMemberCount;
    /* FAKEMATCH(temporary): the member count read into a local of its own, so
     * it loads before memberIndex's address is set; as
     * Func_08108690(&memberIndex, shop->partyMemberCount, 4), 2 of 297
     * instructions differ in tla-en. */
    redraw = Func_08108690(&memberIndex, count, 4);
  }
  wait(1);
  goto loop;

done:
  Menu_ReleaseEntryObjectsFar();
  Window_CloseFar(priceWindow, 2);
  Window_CloseFar(listWindow, 2);
  wait(1);
  return 0;
}
