#include "types.h"
#include "call.h"
#include "text/msg_ids.h"
#include "version.h"
#include "main_routine_work.h"
#include "edition.h"

#include "input.h"
// The Japanese catalog does not name these yet.
extern u8 MsgWhatWillYou[], MsgCoins2[], MsgToss2[], MsgYouWonCoin[];
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
// right; Japanese draws them 16 pixels further left, each followed by its
// counter word.
#if LANGUAGE == LANGUAGE_FRENCH
#define COUNT_WINDOW_WIDTH 19
#define COUNT_NUMBER_X 88
#elif LANGUAGE == LANGUAGE_JAPANESE
#define COUNT_WINDOW_WIDTH 17
#define COUNT_NUMBER_X 56
#else
#define COUNT_WINDOW_WIDTH 17
#define COUNT_NUMBER_X 72
#endif

// The toss prompt's window: two tiles narrower in Japanese, against the right
// edge.
#if LANGUAGE == LANGUAGE_JAPANESE
#define TOSS_WINDOW_X 22
#define TOSS_WINDOW_WIDTH 7
#else
#define TOSS_WINDOW_X 20
#define TOSS_WINDOW_WIDTH 9
#endif

#define WEIGHT(i) ((s8)gMainRoutineWork.unk13d[i])

s32 Func_020001e4(void) {
  s32 choice;
#if !TLA_PATCH_1
  s32 text;
#endif
  s32 message;
  s32 coins;
  s32 medals;
  struct Window *window;
  s32 result;
  s32 prizeRoll;
#if TLA_PATCH_1
  s32 notice;
#endif
  s32 total;
  s32 prize;
  /* FAKEMATCH(temporary): minusOne, a 64-bit -1 for these calls, so CSE does
   * not share a compare's -1 with them; as -1, 57 of 286 instructions differ in
   * tla-en and 68 of 300 in tla-ja. */
  s64 minusOne = -1;

#if TLA_PATCH_1
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
  window = Window_Create(TOSS_WINDOW_X, 15, TOSS_WINDOW_WIDTH, 4, 2);
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
#else
  // The Japanese release keeps The Broken Seal's Japanese layout: a real loop
  // around the prompt and the toss, a counter word after each count, and the
  // coin or medal taken as soon as it is chosen, so cancelling the toss loses
  // it. From patch 1 it is taken once the toss is confirmed.
  choice = 0;
  Func_02001380();
  Func_020013f8(0);
  for (;;) {
    coins = gMainRoutineWork.coins;
    medals = PartyInventory_CountItem(229);
    message = (s32)MsgWhatWillYou;
    Func_020013c8(message);
    /* FAKEMATCH(wrapper): forwarded, so the -1 is built after movs r1, #0; as
     * a direct call, 38 of 300 instructions differ in tla-ja. */
    Value2(FieldMessage_ShowAtActor, -1, 0);
    window = Window_Create(0, 0, COUNT_WINDOW_WIDTH, 4, 2);
    text = (s32)MsgCoins2;
    Text_DrawCharacterAtOffset(text, window, 0, 0);
    Text_DrawNumberInWindow(coins, 6, window, COUNT_NUMBER_X, 0);
    Text_DrawCharacterAtOffset(text + 2, window, 104, 0);
    Text_DrawCharacterAtOffset(text + 1, window, 0, 8);
    Text_DrawNumberInWindow(medals, 6, window, COUNT_NUMBER_X, 8);
    Text_DrawCharacterAtOffset(text + 2, window, 104, 8);
    choice = Menu_SelectCoinsOrMedals(choice);
    Window_Close(window, 2);
    Message_CloseFinishedWindows();
    if (choice == -1)
      goto end;
    if (choice == 0) {
      if (coins == 0) {
        Func_020013c8(message + 1);
        Func_020013d8(-1, 0);
        wait(1);
        continue;
      }
      Party_AdjustCoins(-1);
    } else if (choice == 1) {
      if (medals == 0) {
        Func_020013c8(message + 2);
        Func_020013d8(-1, 0);
        wait(1);
        continue;
      }
      PartyInventory_Remove(229);
      if (PartyInventory_CountFreeSlots() == 0) {
        Func_020013c8(message + 4);
        FieldMessage_ShowAtActor(-1, 0);
        if (Event_AskYesNoWithActorAnimation(0, 0) != 0)
          goto end;
      }
    }
    window = Window_Create(TOSS_WINDOW_X, 15, TOSS_WINDOW_WIDTH, 4, 2);
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
    break;
  }
#endif
  if (result != -1) {
#if TLA_PATCH_1
    if (choice == 0) {
      Party_AdjustCoins(minusOne);
    } else if (choice == 1) {
      PartyInventory_Remove(229);
    }
#endif
    result = Func_02001184(choice);
    if (choice == 0) {
      if (result != 4) {
        Party_AdjustCoins(gData_0200173c[result]);
        Func_02001408(91);
        Func_02001328(gData_0200173c[result], 5);
        Func_020013c8((s32)MsgYouWonCoin);
        Func_020013d8((s32)minusOne, 0);
      } else {
        Func_02001408(113);
        FieldEvent_Wait(10);
      }
    } else {
      total = 0;
      /* FAKEMATCH(wrapper): forwarded, so the flag number is built for each
       * call; as a direct call, 43 of 286 instructions differ in tla-en and 40
       * of 300 in tla-ja. */
      if (result == 0 && Value1(GameFlag_Test, 0x90b) == 0) {
        GameFlag_SetBit(0x90b);
        Func_0200049c((s32)minusOne);
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
