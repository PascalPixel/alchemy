// Matches: differ 0 (tla-en, 2026-11-08)
// Also differ 0 in tla-de, tla-es, tla-fr and tla-it (French through
// COUNT_WINDOW_WIDTH and COUNT_NUMBER_X). tla-ja stays on its scaffold: its
// message ids have other names, so the draft does not compile there, and its
// listing is an older layout (about 300 instructions, saves r11, holds choice
// in r8).
// The Lost Age's twin of The Broken Seal's TolbiSpring_RunSpringGame
// (games/tbs/src/scene/tolbi_games.c): choose coin or medal, toss, award.
// The prompt is a goto loop, as a real outer loop hoists its pool loads, and
// the shared notice tail sits at a join so its -1 is reloaded. result also
// holds the text ids (r5) and window the windows (r6); the weight total and
// prize index have their own locals, as in the twin.
// What closed it (52 to 0, 2026-11-08): FieldMessage_ShowAtActor returns s32
// (games/tla/src/field/event/f0d3c88.c); called as a value-returning function,
// movs r1, #0 falls between movs r0, #1 and negs r0, r0 as in the listing (48).
// Separate total and prize locals instead of reusing choice and window (26):
// window's argument-register preferences no longer pass to the halving's slot,
// so the weight takes r1, r1 joins reload's spill registers, and reload's round
// robin gives the listing's r1, r2, r3, r1, r2 at the top. Then the text id no
// longer needs to pass through result (0).
// Devices left, each measured at the match: minusOne (temporary, 57) and two
// call.h wrappers, the first FieldMessage_ShowAtActor call (144) and
// GameFlag_Test (43); the other ShowAtActor call and Party_AdjustCoins are
// direct calls through their prototypes (0). Tried and no better: minusOne
// as s32, u32, s16 or s8 (57; s64 and u64 both match), one s32 -1 local for the
// compares and calls (50), a separate text local (13 or 17), a call through a
// cast for the first ShowAtActor (144). Func_020013d8 and Func_0200049c are
// declared void and GameFlag_SetBit s32, as their definitions are (no change).

#include "types.h"
#include "call.h"
#include "text/msg_ids.h"
#include "main_routine_work.h"
#include "edition.h"

#include "input.h"
extern u16 gData_0200173c[];
extern s32 gData_02001700[];
void Func_02001380();
void Func_020013f8();
s32 PartyInventory_CountItem();
void Func_020013c8();
s32 FieldMessage_ShowAtActor(s32 localId, s32 unused);
struct Window *Window_Create();
void Text_DrawCharacterAtOffset();
void Text_DrawNumberInWindow();
s32 Menu_SelectCoinsOrMedals();
void Window_Close();
void Message_CloseFinishedWindows();
void Func_020013d8();
void wait();
void Func_02001408();
s32 PartyInventory_CountFreeSlots();
s32 Event_AskYesNoWithActorAnimation();
s32 Party_AdjustCoins(s32 amount);
s32 PartyInventory_Remove();
s32 Func_02001184();
void Func_02001328();
void FieldEvent_Wait();
s32 GameFlag_Test();
s32 GameFlag_SetBit();
void Func_0200049c();
s32 Random16Far();
void Func_02001388();

// French draws the count window two tiles wider, its numbers 16 pixels further
// right.
#if LANGUAGE == LANGUAGE_FRENCH
#define COUNT_WINDOW_WIDTH 19
#define COUNT_NUMBER_X 88
#else
#define COUNT_WINDOW_WIDTH 17
#define COUNT_NUMBER_X 72
#endif

#define WEIGHT(i) ((s8)gMainRoutineWork.unk13d[i])

s32 Func_020001e4(void) {
  s32 choice;
  s32 message;
  s32 coins;
  s32 medals;
  struct Window *window;
  s32 result;
  s32 prizeRoll;
  s32 notice;
  s32 total;
  s32 prize;
  /* FAKEMATCH(temporary): minusOne, a 64-bit -1 for two calls, so CSE does
   * not share the cancel compare's -1 with them; as -1, 57 of 286 instructions
   * differ in tla-en. */
  s64 minusOne = -1;

  Func_02001380();
  choice = 0;
  Func_020013f8(0);
prompt:
  coins = gMainRoutineWork.coins;
  medals = PartyInventory_CountItem(229);
  message = (s32)MsgWhatWillYou;
  Func_020013c8(message);
  /* FAKEMATCH(wrapper): forwarded, so the -1 is built after movs r1, #0; as a
   * direct call, 144 of 286 instructions differ in tla-en. */
  Value2(FieldMessage_ShowAtActor, -1, 0);
  window = Window_Create(0, 0, COUNT_WINDOW_WIDTH, 4, 2);
  result = (s32)MsgCoins2;
  Text_DrawCharacterAtOffset(result, window, 0, 0);
  Text_DrawNumberInWindow(coins, 6, window, COUNT_NUMBER_X, 0);
  Text_DrawCharacterAtOffset(result + 1, window, 0, 8);
  Text_DrawNumberInWindow(medals, 6, window, COUNT_NUMBER_X, 8);
  choice = Menu_SelectCoinsOrMedals(choice);
  Window_Close(window, 2);
  Message_CloseFinishedWindows();
  if (choice == -1)
    goto end;
  if (choice == 0) {
    if (coins != 0)
      goto toss;
    notice = message + 1;
  } else {
    if (choice != 1)
      goto toss;
    if (medals != 0)
      goto full;
    notice = message + 2;
  }
  Func_020013c8(notice);
  Func_020013d8(-1, 0);
  wait(1);
  goto prompt;
full:
  if (PartyInventory_CountFreeSlots() == 0) {
    Func_020013c8(message + 4);
    FieldMessage_ShowAtActor(minusOne, 0);
    if (Event_AskYesNoWithActorAnimation(0, 0) != 0)
      goto end;
  }
toss:
  window = Window_Create(20, 15, 9, 4, 2);
  result = (s32)MsgToss2;
  Text_DrawCharacterAtOffset(result, window, 0, 0);
  Text_DrawCharacterAtOffset(result + 1, window, 0, 8);
  wait(5);
  Func_02001408(116);
  for (;;) {
    if (gInput.pressed & 1) {
      Func_02001408(112);
      result = 0;
      break;
    }
    if (gInput.pressed & 2) {
      Func_02001408(113);
      result = -1;
      break;
    }
    wait(1);
  }
  Window_Close(window, 2);
  if (result != -1) {
    if (choice == 0) {
      Party_AdjustCoins(minusOne);
    } else if (choice == 1) {
      PartyInventory_Remove(229);
    }
    result = Func_02001184(choice);
    if (choice == 0) {
      if (result != 4) {
        Party_AdjustCoins(gData_0200173c[result]);
        Func_02001408(91);
        Func_02001328(gData_0200173c[result], 5);
        Func_020013c8((s32)MsgYouWonCoin);
        Func_020013d8(-1, 0);
      } else {
        Func_02001408(113);
        FieldEvent_Wait(10);
      }
    } else {
      total = 0;
      /* FAKEMATCH(wrapper): forwarded, so the flag number is built for each
       * call; as a direct call, 43 of 286 instructions differ in tla-en. */
      if (result == 0 && Value1(GameFlag_Test, 0x90b) == 0) {
        GameFlag_SetBit(0x90b);
        Func_0200049c(-1);
      } else {
        for (prize = 0; prize < result * 3 + 3; prize++)
          total += (s8)gMainRoutineWork.unk13d[prize];
        prizeRoll = (u32)(Random16Far() * total) >> 16;
        for (prize = 0; prize < 15; prize++) {
          prizeRoll -= WEIGHT(prize);
          if (prizeRoll < 0)
            break;
        }
        if (prize == 15)
          prize = 14;
        Func_0200049c(gData_02001700[prize]);
        if (WEIGHT(prize) > 1)
          gMainRoutineWork.unk13d[prize] = WEIGHT(prize) / 2;
      }
    }
  }
end:
  Func_02001388();
  return 0;
}
