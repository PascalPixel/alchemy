#include "types.h"
#include "scene_tables.h"
#include "text/msg_ids.h"
#include "version.h"
#include "main_routine_work.h"
#include "edition.h"
#include "input.h"
#include "event_inline.h"
#include "actor.h"

s32 Scene_PrepareMap(void) { return 0; }

// The Japanese catalog does not name these yet.
extern u8 MsgWhatWillYou[], MsgCoins2[], MsgToss2[], MsgYouWonCoin[];
extern u16 gData_0200173c[];
extern s32 gData_02001700[];
void Func_02001380(void);
s32 PartyInventory_CountItem();
struct Window *Window_Create();
void Text_DrawCharacterAtOffset();
void Text_DrawNumberInWindow();
s32 Menu_SelectCoinsOrMedals();
void Window_Close();
void Message_CloseFinishedWindows();
s32 PartyInventory_CountFreeSlots();
s32 Party_AdjustCoins(s32 amount);
s32 PartyInventory_Remove();
s32 Func_02001184();
void Func_02001328();
void Func_0200049c();
s32 Random16Far();
void Func_02001388(void);

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
  Event_ResolvePendingActions(0);
prompt:
  coins = gMainRoutineWork.coins;
  medals = PartyInventory_CountItem(229);
  message = (s32)MsgWhatWillYou;
  Event_SetMessageId(message);
  /* FAKEMATCH(wrapper): forwarded, so the -1 is built after movs r1, #0; as a
   * direct call, 144 of 286 instructions differ in tla-en. */
  Event_OpenMessage(-1, 0);
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
  Event_SetMessageId(notice);
  Event_ShowMessageAndWait(-1, 0);
  wait(1);
  goto prompt;
full:
  if (PartyInventory_CountFreeSlots() == 0) {
    Event_SetMessageId(message + 4);
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
  Sound_Play(116);
  for (;;) {
    if (gInput.pressed & 1) {
      Sound_Play(112);
      result = 0;
      break;
    }
    if (gInput.pressed & 2) {
      Sound_Play(113);
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
  Event_ResolvePendingActions(0);
  for (;;) {
    coins = gMainRoutineWork.coins;
    medals = PartyInventory_CountItem(229);
    message = (s32)MsgWhatWillYou;
    Event_SetMessageId(message);
    /* FAKEMATCH(wrapper): forwarded, so the -1 is built after movs r1, #0; as
     * a direct call, 38 of 300 instructions differ in tla-ja. */
    Event_OpenMessage(-1, 0);
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
        Event_SetMessageId(message + 1);
        Event_ShowMessageAndWait(-1, 0);
        wait(1);
        continue;
      }
      Party_AdjustCoins(-1);
    } else if (choice == 1) {
      if (medals == 0) {
        Event_SetMessageId(message + 2);
        Event_ShowMessageAndWait(-1, 0);
        wait(1);
        continue;
      }
      PartyInventory_Remove(229);
      if (PartyInventory_CountFreeSlots() == 0) {
        Event_SetMessageId(message + 4);
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
    Sound_Play(116);
    for (;;) {
      if (gInput.pressed & 1) {
        Sound_Play(112);
        result = 0;
        break;
      }
      if (gInput.pressed & 2) {
        Sound_Play(113);
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
        Sound_Play(91);
        Func_02001328(gData_0200173c[result], 5);
        Event_SetMessageId((s32)MsgYouWonCoin);
        Event_ShowMessageAndWait((s32)minusOne, 0);
      } else {
        Sound_Play(113);
        FieldEvent_Wait(10);
      }
    } else {
      total = 0;
      /* FAKEMATCH(wrapper): forwarded, so the flag number is built for each
       * call; as a direct call, 43 of 286 instructions differ in tla-en and 40
       * of 300 in tla-ja. */
      if (result == 0 && Event_GetFlag(0x90b) == 0) {
        Event_SetFlag(0x90b);
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

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

#include "call.h"
s32 Func_020013b8();
s32 Func_02001400();
s32 Func_02001338();
s32 Func_02001370();
s32 Func_020013f0();

void Func_0200049c(s32 a0) {
  s32 t52;

  Func_02001380();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_Wait(30);
  Event_PlaySound(0x94);
  FieldEvent_Wait(100);
  Actor_FaceDirection(0, FACING_NORTH, 0);
  FieldEvent_Wait(40);
  MapGrid_CopyCellsTo(0x52, 0x14, 0x46, 0, 3, 8);
  FieldEvent_Wait(3);
  MapGrid_CopyCellsTo(0x55, 0x14, 0x46, 0, 3, 8);
  Event_PlaySound(0x9a);
  FieldEvent_Wait(8);
  MapGrid_CopyCellsTo(0x58, 0x14, 0x46, 0, 3, 8);
  Event_PlaySound(0x9a);
  FieldEvent_Wait(8);
  MapGrid_CopyCellsTo(0x5b, 0x14, 0x46, 0, 3, 8);
  Event_PlaySound(0x9a);
  FieldEvent_Wait(8);
  MapGrid_CopyCellsTo(0x5e, 0x14, 0x46, 0, 3, 8);
  Event_PlaySound(0x9a);
  FieldEvent_Wait(8);
  MapGrid_CopyCellsTo(0x61, 0x14, 0x46, 0, 3, 8);
  Event_PlaySound(0x9a);
  FieldEvent_Wait(8);
  MapGrid_CopyCellsTo(0x64, 0x14, 0x46, 0, 3, 8);
  Event_PlaySound(0x9a);
  FieldEvent_Wait(8);
  MapGrid_CopyCellsTo(0x4f, 0x1d, 0x46, 0, 3, 8);
  Event_PlaySound(0x9a);
  FieldEvent_Wait(8);
  MapGrid_CopyCellsTo(0x52, 0x1d, 0x46, 0, 3, 8);
  Event_PlaySound(0x9a);
  FieldEvent_Wait(8);
  MapGrid_CopyCellsTo(0x55, 0x1d, 0x46, 0, 3, 8);
  Event_PlaySound(0x9a);
  FieldEvent_Wait(8);
  MapGrid_CopyCellsTo(0x58, 0x1d, 0x46, 0, 3, 8);
  Event_PlaySound(0x9a);
  FieldEvent_Wait(8);
  MapGrid_CopyCellsTo(0x5b, 0x1d, 0x46, 0, 3, 8);
  Event_PlaySound(0x9a);
  FieldEvent_Wait(8);
  MapGrid_CopyCellsTo(0x5e, 0x1d, 0x46, 0, 3, 8);
  Event_PlaySound(0x9a);
  FieldEvent_Wait(8);
  MapGrid_CopyCellsTo(0x61, 0x1d, 0x46, 0, 3, 8);
  Event_PlaySound(0x9a);
  FieldEvent_Wait(8);
  MapGrid_CopyCellsTo(0x64, 0x1d, 0x46, 0, 3, 8);
  Event_PlaySound(0x9a);
  FieldEvent_Wait(70);
  Event_PlaySound(0x7e);
  if (a0 == -1) {
    t52 = Actor_Get(0x1d);
    Func_020013b8(0x1d, FIELD_AT(&gMainRoutineWork, s32, 532));
    FIELD_AT((t52 + 0x55), u8, 0) = 0;
    FIELD_AT(t52, s32, 12) = 0x200000;
    FieldActor_SetAnimation(FIELD_AT(&gMainRoutineWork, s32, 532), 0x1c);
    Func_02001400(0x1d);
    Func_02001338(0x16);
    Func_02001370(0x16);
    Actor_SetPosition(0x1d, 0, 0);
    FieldActor_SetAnimation(FIELD_AT(&gMainRoutineWork, s32, 532), 1);
  } else {
    Call2(Func_020013f0, a0, 3);
    Party_GiveItem(a0, 0);
    FieldEvent_Wait(20);
  }
  MapGrid_CopyCellsTo(0x61, 0x1d, 0x46, 0, 3, 8);
  Event_PlaySound(0x9a);
  FieldEvent_Wait(8);
  MapGrid_CopyCellsTo(0x5e, 0x1d, 0x46, 0, 3, 8);
  Event_PlaySound(0x9a);
  FieldEvent_Wait(8);
  MapGrid_CopyCellsTo(0x5b, 0x1d, 0x46, 0, 3, 8);
  Event_PlaySound(0x9a);
  FieldEvent_Wait(8);
  MapGrid_CopyCellsTo(0x58, 0x1d, 0x46, 0, 3, 8);
  Event_PlaySound(0x9a);
  FieldEvent_Wait(8);
  MapGrid_CopyCellsTo(0x55, 0x1d, 0x46, 0, 3, 8);
  Event_PlaySound(0x9a);
  FieldEvent_Wait(8);
  MapGrid_CopyCellsTo(0x52, 0x1d, 0x46, 0, 3, 8);
  Event_PlaySound(0x9a);
  FieldEvent_Wait(8);
  MapGrid_CopyCellsTo(0x64, 0x14, 0x46, 0, 3, 8);
  Event_PlaySound(0x9a);
  FieldEvent_Wait(8);
  MapGrid_CopyCellsTo(0x61, 0x14, 0x46, 0, 3, 8);
  Event_PlaySound(0x9a);
  FieldEvent_Wait(8);
  MapGrid_CopyCellsTo(0x5e, 0x14, 0x46, 0, 3, 8);
  Event_PlaySound(0x9a);
  FieldEvent_Wait(8);
  MapGrid_CopyCellsTo(0x5b, 0x14, 0x46, 0, 3, 8);
  Event_PlaySound(0x9a);
  FieldEvent_Wait(8);
  MapGrid_CopyCellsTo(0x58, 0x14, 0x46, 0, 3, 8);
  Event_PlaySound(0x9a);
  FieldEvent_Wait(8);
  MapGrid_CopyCellsTo(0x55, 0x14, 0x46, 0, 3, 8);
  Event_PlaySound(0x9a);
  FieldEvent_Wait(8);
  MapGrid_CopyCellsTo(0x52, 0x14, 0x46, 0, 3, 8);
  Event_PlaySound(0x9a);
  FieldEvent_Wait(8);
  MapGrid_CopyCellsTo(0x4f, 0x14, 0x46, 0, 3, 8);
  Event_PlaySound(0x9a);
  FieldEvent_Wait(8);
  Func_02001388();
}

void Actor_SetAnimationSpeed(struct Actor *object, s32 value);

struct Half {
  u16 v;
};

void Func_02000884(s32 id, s32 *pos, s32 dir, s32 palette, s32 value) {
  struct Actor *object = Actor_Get(id);
  u8 *sprite;
  u8 **list;
  s32 i;

  if (object != NULL) {
    struct Half zero;

    object->x = *pos++;
    object->y = *pos++;
    object->z = *pos;
    object->angle = dir;
    zero.v = 0;
    object->motionFlags = zero.v;
    ((u8 *)object->animation)[26] = zero.v;
    Actor_SetAnimationSpeed(object, value);
  }
  sprite = object->animation;
  i = 0;
  if (i >= sprite[27])
    return;
  list = (u8 **)(sprite + 40);
next:
  {
    u8 *cell = *list++;

    if (cell[5] != palette) {
      cell[5] = palette;
      cell[22] = 255;
    }
  }
  i++;
  if (i < sprite[27])
    goto next;
}
