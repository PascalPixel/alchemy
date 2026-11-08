// Matches: differ 0 (tla-en, 2026-11-08)
// Editions, 2026-11-08: tla-en, tla-ja, tla-de, tla-es and tla-fr differ 0 of
// 297 (ja at 0810ac00, de at 08112bf4, es at 0810ebf4, fr at 0810abf4); German
// and French differ from English only in the price window's column and width
// (SERVICE_PRICE_WINDOW_X and _WIDTH: 0 and 30 in German, 1 and 25 in French).
// Italian (0810abf4, 282 instructions) differs 6 of 282 and stays on its
// scaffold: it zeroes unitId and memberIndex at the top and chooseAgain after
// the list window, where the native zeroes unitId after the list window
// (reload register r5, mov r8, r5) and stores memberIndex a second time from
// r5. Italian orders tried with the devices below in place: every placement
// of memberIndex = 0 (once or twice), unitId = 0 and chooseAgain = 0 at the
// top and after the list window, with and without messageBase = 0 and
// memberIndex = messageBase or memberIndex = unitId: 6 at best (unitId and
// memberIndex at the top, 6; memberIndex at the top and the rest after, 42 to
// 44).
// How the English form matches (2026-11-08): chooseAgain = 1 sits at each
// retry's end, as natively. chooseAgain goes to the stack (sp + 4) because it
// conflicts with r6 from the top: messageBase, held in r6, is set to 0 first
// and copied into memberIndex after the price window, so r6 is live across
// both window calls; reload then finds that 0 in r6 for chooseAgain's and
// unitId's stores, and shop is pushed to r7. As a plain local, CSE replaces
// the copy with chooseAgain (the zero that lives longest) and messageBase's 0
// is deleted (133). The partyAction read through a pointer makes the load
// alias memberIndex's top store, which keeps that store ahead of it as
// natively (4 without). The count copied before Func_08108690 puts
// add r0, sp, #12 after the ldrsb (2 without).
// Tried 2026-11-08 without success: every placement of the zero assignments
// across six points of the opening without the pin (56 at best: chooseAgain =
// 0 between Func_080f8060 and the price window spills it, but a local zero
// takes r5 instead of r6); (void)&chooseAgain (no change: cse2 folds the copy
// back); a zero local pinned to r6 instead of messageBase (122, messageBase
// then needs another register); empty asm barriers on messageBase in place of
// the pin (188); Func_08108690's parameters as s8, s16, u8 or void * (no
// change). Earlier rounds (2026-11-01 to 11-07) reached 45 with the pin and
// chooseAgain = 1 set once before the first donation message; superseded.
// Twin of ☀️'s Sanctum_RunPartyService (games/tbs/src/shop/shop/sanctum.c): the
// sanctum's service for one party member at a time, four a page. It asks who
// to treat, shows the donation, and on yes and enough coins applies the
// service (BattleUnit_ResetStateByMode), plays the icon burst (Func_0810b1b4)
// and takes the coins, until nobody needs it or B. ⚓️ adds the page: the
// member's slot is memberIndex % 4, Func_080f8058 turns to a new page, and
// Func_08108690 moves the cursor where ☀️ reads left and right itself. Its
// calls: Func_0810a9ac and Func_0810a9fc show a message, Func_08108660 the
// choice, Shop_DrawServiceParty the party page, Func_08109188 the coins.
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
#if SERVICE_PRICE_WINDOW
  register s32 messageBase asm("r6");
  /* FAKEMATCH(pin): messageBase held in r6; as a plain local, 133 of 297 instructions differ in tla-en. */
#else
  s32 messageBase;
#endif

#if SERVICE_PRICE_WINDOW
  // messageBase holds the 0 the first search starts from until the donation
  // message needs it.
  messageBase = 0;
  chooseAgain = 0;
  unitId = 0;
  memberIndex = 0;
#else
  unitId = 0;
  memberIndex = 0;
#endif
  redraw = 1;
  {
    s8 *partyAction = &shop->partyAction;
    /* FAKEMATCH(temporary): partyAction read through a pointer to it; as serviceKind = shop->partyAction, 4 of 297 instructions differ in tla-en. */
    serviceKind = *partyAction;
  }
  Func_0810a9ac((s32)Msg04823);

  listWindow = Window_CreateFar(1, 12, 13, 3, 2);
  shop->cursorAnchor->active = 4;
  shop->mode = redraw;
  Func_080f8060(listWindow, 2, 0);
#if SERVICE_PRICE_WINDOW
  priceWindow = Window_CreateFar(SERVICE_PRICE_WINDOW_X, 16,
                                 SERVICE_PRICE_WINDOW_WIDTH, 3, 2);
  memberIndex = messageBase;
#else
  priceWindow = NULL;
  chooseAgain = 0;
#endif
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
    Func_0810a9ac((s32)Msg04823);
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
    messageBase = (s32)Msg04824;
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
    /* FAKEMATCH(temporary): the count copied into count before the call; as Func_08108690(&memberIndex, shop->partyMemberCount, 4), 2 of 297 instructions differ in tla-en. */
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
