// Differ 45 of 297 (tla-en, 2026-11-07).
// Editions, 2026-11-07: ja, de, es and fr take the English form. Italian
// (0810abf4, 282 instructions, one function) creates no price window (both
// Shop_DrawSelectMsg and the closing Window_CloseFar take 0), keeps
// messageBase and chooseAgain together in r7 with no pin, and sets
// chooseAgain = 1 at each retry's end: differ 134 to 46 of 282, left only
// scheduling and allocation at the top (unitId and memberIndex share a zero in
// r5 natively) and one argument order before Func_08108690.
// 2026-11-07, second round: messageBase pinned to r6 (FAKEMATCH pin), 77 to 45;
// left are only the native's three chooseAgain = 1 stores at each retry's
// end (moving them there with the pin: 134).
// 2026-11-07: the outer loop kept as a label with goto for continue and break,
// the 77 form below; left are shop and messageBase swapped between r6 and r7
// and the native's three chooseAgain = 1 stores. Before, differ 81
// (2026-11-06): allocation only (48 of 81 rows are registers). shop takes r6
// and messageBase r7, the native the reverse. In the lreg dump shop is used 36
// times across 208 insns and messageBase 16 across 82, so global.c ranks shop
// first; the native must rank messageBase first. The native's r6 = 0 at the top
// is then free for chooseAgain's store and inherited by unitId = 0 and a second
// memberIndex = 0 before the first search (the native stores memberIndex
// twice). Tried 2026-11-06: that second memberIndex = 0 (differ 96: it reloads
// chooseAgain's slot instead), messageBase set at the head of the A block
// (180), messageBase = 0 first with chooseAgain, unitId and memberIndex copied
// from it (160, CSE folds it), volatile chooseAgain (197). Also tried
// 2026-11-06: messageBase as u8 * (no change); 5 minutes of permute reached 75
// to 76 only by a forced temporary (a 1 local, or coins read into chooseAgain),
// not kept. Earlier, 2026-11-05: 57 with slots and pool offsets normalized, 15
// with registers blind too. chooseAgain = 1 now sits after messageBase's set,
// once for all three retries: that makes the two conflict, so chooseAgain
// spills to sp + 4 as natively and the frame is 16. The native sets it at each
// retry's end (three rows off), and what makes it conflict there is not found.
// Also tried 2026-11-06: the outer loop as a label with goto for continue and
// break (differ 77: the two chooseAgain = 1 tails stop merging, and what is
// left is only shop and messageBase swapped between r6 and r7, and the native's
// three chooseAgain = 1 stores); with chooseAgain = 1 at each retry's end there
// (158), or messageBase set at the top (232), or a priceWindow = 0 default
// (179); the search loops written with goto (no change: the counts are by CFG
// loop depth, not loop notes); Msg04824 + n at each site in place of
// messageBase (no change). global.c ranks by floor_log2(refs) * refs / length:
// price 14/45 (.93), shop 36/214 (.84), messageBase 16/84 (.76); the native
// needs messageBase between price and shop. 10 minutes of permute on the goto
// form reached 72 only by reading coins into chooseAgain again. Draft, differ
// 134 of 297, skeleton gap 18/8 (tla-en, 2026-11-01) Twin of ☀️'s
// Sanctum_RunPartyService (games/tbs/src/shop/shop/sanctum.c): the sanctum's
// service for one party member at a time, four a page. It asks who to treat,
// shows the donation, and on yes and enough coins applies the service
// (BattleUnit_ResetStateByMode), plays the icon burst (Func_0810b1b4) and takes
// the coins, until nobody needs it or B. ⚓️ adds the page: the member's slot is
// memberIndex % 4, Func_080f8058 turns to a new page, and Func_08108690 moves
// the cursor where ☀️ reads left and right itself. Its calls: Func_0810a9ac and
// Func_0810a9fc show a message, Func_08108660 the choice, Shop_DrawServiceParty
// the party page, Func_08109188 the coins. Ported from the twin's declaration
// order and loops, which puts shop in r7, messageBase in r6, price in r5 and
// every other register where the native has it. Left: the native spills
// chooseAgain to sp + 4 (frame 16 bytes); here it shares r6 with messageBase
// (frame 12), since it is dead on every path where messageBase lives. That one
// difference shifts the stack offsets, and with no stores to the stack
// cross-jumping merges two chooseAgain = 1 tails. The native's r6 at the top is
// not a variable: it is the reload register for chooseAgain's spill, which
// later zeros inherit. Tried: chooseAgain and memberIndex set at the top
// (allocation falls back to shop in r6, differ 158 to 161); a local holding 0
// (CSE folds it); messageBase = 0 (dead, deleted); memberIndex = chooseAgain at
// the second loop and price and messageBase in the inner block (no change);
// volatile chooseAgain, as a diagnostic only (differ 194: it is no memory
// variable). Nothing plain found yet makes chooseAgain conflict with
// messageBase.
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
  /* FAKEMATCH(pin): messageBase held in r6; as a plain local, 77 of 297 instructions differ in tla-en. */
#else
  s32 messageBase;
#endif

#if SERVICE_PRICE_WINDOW
  chooseAgain = 0;
#endif
  memberIndex = 0;
  redraw = 1;
  serviceKind = shop->partyAction;
  Func_0810a9ac((s32)Msg04823);

  listWindow = Window_CreateFar(1, 12, 13, 3, 2);
  shop->cursorAnchor->active = 4;
  shop->mode = redraw;
  Func_080f8060(listWindow, 2, 0);
#if SERVICE_PRICE_WINDOW
  priceWindow = Window_CreateFar(1, 16, 23, 3, 2);
#else
  priceWindow = NULL;
#endif

#if !SERVICE_PRICE_WINDOW
  memberIndex = 0;
#endif
  unitId = 0;
#if !SERVICE_PRICE_WINDOW
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
#if SERVICE_PRICE_WINDOW
    chooseAgain = 1;
#endif
    Func_0810a9ac(messageBase);
    if (Func_08108660(0) != 0) {
      Func_0810a9fc(messageBase + 2);
#if !SERVICE_PRICE_WINDOW
      chooseAgain = 1;
#endif
      goto loop;
    }
    if ((u32)price > gMainRoutineWork.coins) {
      Sound_PlayFar(113);
      Func_0810a9fc(messageBase + 1);
#if !SERVICE_PRICE_WINDOW
      chooseAgain = 1;
#endif
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
  redraw = Func_08108690(&memberIndex, shop->partyMemberCount, 4);
  wait(1);
  goto loop;

done:
  Menu_ReleaseEntryObjectsFar();
  Window_CloseFar(priceWindow, 2);
  Window_CloseFar(listWindow, 2);
  wait(1);
  return 0;
}
