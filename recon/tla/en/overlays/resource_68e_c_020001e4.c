// Draft, differ 144 of 286, skeleton gap 32 (tla-en, 2026-11-01)
// The Lost Age's twin of The Broken Seal's TolbiSpring_RunSpringGame
// (games/tbs/src/scene/tolbi_games.c): choose coin or medal, toss, award.
// The prompt is a goto loop, as a real outer loop hoists its pool loads, and
// the shared notice tail sits at a join so its -1 is reloaded. One local per
// callee-saved register of the listing: choice also adds up the weights (r7),
// result also holds the text ids (r5), window also the prize index (r6);
// separate locals put choice in r9 and cost 15 of skeleton.
// Left: 1. the -1 of each cancel compare stays in r5 and is reused by the
// free-slots notice and Party_AdjustCoins, where the listing reloads it into
// r2 after the calls; Call and Value do not stop that, nor did one mask
// variable set before each compare (gap 49), a cancel local or a switch.
// 2. gInput and the 1 are hoisted out of the key loop. A goto into the loop
// stops the hoist but puts the confirm block after that goto; a goto loop
// pulls the block inline. 3. The weight sum shares result * 3 only by
// recomputing it: a count local gives gap 33, a do-while 42. 4. The first read
// of weight 0 and the pick loop share state + 1, which the listing recomputes.
#include "types.h"
#include "call.h"
#include "text/msg_ids.h"
#include "main_routine_work.h"

#include "input.h"
extern u16 gData_0200173c[];
extern s32 gData_02001700[];
void Func_02001380();
void Func_020013f8();
s32 PartyInventory_CountItem();
void Func_020013c8();
void FieldMessage_ShowAtActor();
struct Window *Window_Create();
void Text_DrawCharacterAtOffset();
void Text_DrawNumberInWindow();
s32 Menu_SelectCoinsOrMedals();
void Window_Close();
void Message_CloseFinishedWindows();
s32 Func_020013d8();
void wait();
void Func_02001408();
s32 PartyInventory_CountFreeSlots();
s32 Event_AskYesNoWithActorAnimation();
s32 Party_AdjustCoins();
s32 PartyInventory_Remove();
s32 Func_02001184();
void Func_02001328();
void FieldEvent_Wait();
s32 GameFlag_Test();
void GameFlag_SetBit();
s32 Func_0200049c();
s32 Random16Far();
void Func_02001388();

s32 Func_020001e4(void) {
  s32 choice;
  s32 message;
  s32 coins;
  s32 medals;
  s32 window;
  s32 result;
  s32 prizeRoll;
  s32 notice;
  struct MainRoutineWork *state;

  Func_02001380();
  choice = 0;
  Call1((void (*)())Func_020013f8, 0);
prompt:
  coins = gMainRoutineWork.coins;
  medals = PartyInventory_CountItem(229);
  message = (s32)MsgWhatWillYou;
  Func_020013c8(message);
  Call2((void (*)())FieldMessage_ShowAtActor, -1, 0);
  window = (s32)Window_Create(0, 0, 17, 4, 2);
  result = (s32)MsgCoins2;
  Call4((void (*)())Text_DrawCharacterAtOffset, result, window, 0, 0);
  Text_DrawNumberInWindow(coins, 6, window, 72, 0);
  Call4((void (*)())Text_DrawCharacterAtOffset, result + 1, window, 0, 8);
  Text_DrawNumberInWindow(medals, 6, window, 72, 8);
  choice = Menu_SelectCoinsOrMedals(choice);
  Call2((void (*)())Window_Close, window, 2);
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
  Call2((void (*)())Func_020013d8, -1, 0);
  Call1((void (*)())wait, 1);
  goto prompt;
full:
  if (PartyInventory_CountFreeSlots() == 0) {
    Func_020013c8(message + 4);
    Call2((void (*)())FieldMessage_ShowAtActor, -1, 0);
    if (Event_AskYesNoWithActorAnimation(0, 0) != 0)
      goto end;
  }
toss:
  window = (s32)Window_Create(20, 15, 9, 4, 2);
  result = (s32)MsgToss2;
  Call4((void (*)())Text_DrawCharacterAtOffset, result, window, 0, 0);
  Call4((void (*)())Text_DrawCharacterAtOffset, result + 1, window, 0, 8);
  Call1((void (*)())wait, 5);
  Call1((void (*)())Func_02001408, 116);
  for (;;) {
    if (gInput.pressed & 1) {
      Call1((void (*)())Func_02001408, 112);
      result = 0;
      break;
    }
    if (gInput.pressed & 2) {
      Call1((void (*)())Func_02001408, 113);
      result = -1;
      break;
    }
    Call1((void (*)())wait, 1);
  }
  Call2((void (*)())Window_Close, window, 2);
  if (result != -1) {
    if (choice == 0) {
      Call1((void (*)())Party_AdjustCoins, -1);
    } else if (choice == 1) {
      Call1((void (*)())PartyInventory_Remove, 229);
    }
    result = Func_02001184(choice);
    if (choice == 0) {
      if (result != 4) {
        Party_AdjustCoins(gData_0200173c[result]);
        Call1((void (*)())Func_02001408, 91);
        Func_02001328(gData_0200173c[result], 5);
        Func_020013c8((s32)MsgYouWonCoin);
        Call2((void (*)())Func_020013d8, -1, 0);
      } else {
        Call1((void (*)())Func_02001408, 113);
        Call1((void (*)())FieldEvent_Wait, 10);
      }
    } else {
      choice = 0;
      if (result == 0 && Value1((s32 (*)())GameFlag_Test, 0x90b) == 0) {
        Call1((void (*)())GameFlag_SetBit, 0x90b);
        Call1((void (*)())Func_0200049c, -1);
      } else {
        window = 0;
        if (window < result * 3 + 3) {
          state = &gMainRoutineWork;
        sum:
          choice += (s8)state->unk13d[window];
          window++;
          if (window < result * 3 + 3)
            goto sum;
        }
        prizeRoll = (u32)(Random16Far() * choice) >> 16;
        state = &gMainRoutineWork;
        {
          s32 slot = 316;
          s8 *weights = (s8 *)state + 1;

          prizeRoll -= weights[slot];
        }
        window = 0;
        if (prizeRoll >= 0) {
        pick:
          window++;
          if (window <= 14) {
            prizeRoll -= (s8)state->unk13d[window];
            if (prizeRoll >= 0)
              goto pick;
          }
        }
        if (window == 15)
          window = 14;
        Func_0200049c(gData_02001700[window]);
        {
          s32 slot = window + 316;
          s8 *weights;

          state = &gMainRoutineWork;
          weights = (s8 *)state + 1;
          if (weights[slot] > 1)
            weights[slot] /= 2;
        }
      }
    }
  }
end:
  Func_02001388();
  return 0;
}
