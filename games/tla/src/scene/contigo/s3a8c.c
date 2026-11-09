#include "contigo.h"
#include "scene_tables.h"
#include "edition.h"
#include "event_inline.h"

// The lines the prize counter speaks: the Spanish text numbers them
// differently.
#if (LANGUAGE == LANGUAGE_SPANISH)
#define MSG_COINS_WON ((s32)MsgContigo03 + 1)
#define MSG_COINS_LOST ((s32)MsgContigo03 + 2)
#define MSG_READ_SIGN ((s32)MsgContigo04)
#define MSG_CONGRATULATIONS ((s32)MsgContigo05)
#define MSG_FIRST_PRIZE ((s32)MsgContigo11)
#define MSG_NEXT_PRIZE ((s32)MsgContigo14)
#define MSG_COME_AGAIN ((s32)MsgContigo17)
#else
#define MSG_COINS_WON ((s32)MsgContigo04)
#define MSG_COINS_LOST ((s32)MsgContigo05)
#define MSG_READ_SIGN ((s32)MsgContigo11)
#define MSG_CONGRATULATIONS ((s32)MsgContigo14)
#define MSG_FIRST_PRIZE ((s32)MsgContigo15)
#define MSG_NEXT_PRIZE ((s32)MsgContigo16)
#define MSG_COME_AGAIN ((s32)MsgContigo17)
#endif

struct ContigoPrizes {
  u8 unk000[0x14c];
  s8 list[8];
};

extern u8 gSceneState[];

void Func_02001c44(s32 id);
s32 Func_02001a60(s32 topic);
void Func_0200403c(s32 value, s32 digits);
void Func_020041fc(s32 item, s32 count);
void Sound_WaitForJingle(void);

void Func_02003a8c(void) {
  s32 diff;
  s8 *list;
  s8 *prize;
  s32 item;

  ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0x209;
  Func_02001c44(13);
  Func_02001c44(16);
  Func_02001c44(17);
  Func_02001c44(19);
  Func_02001c44(20);
  Actor_SetPartAttribute(((struct ContigoObject *)Actor_GetById(14)), 1);
  Actor_SetPartAttribute(((struct ContigoObject *)Actor_GetById(15)), 2);
  Actor_SetPartAttribute(((struct ContigoObject *)Actor_GetById(8)), 0);
  Actor_SetPartAttribute(((struct ContigoObject *)Actor_GetById(9)), 3);
  if (!Event_GetFlag(0x9e5))
    return;
  GameFlag_ClearBit(0x9e5);
  if (gMainRoutineWork.entrance == 18) {
    diff = gMainRoutineWork.coins - *(s32 *)gSceneState;
    Func_0200406c();
    Event_ResolvePendingActions(0);
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    if (diff > 0) {
      if (diff > 19999)
        Sound_Play(93);
      else if (diff > 4999)
        Sound_Play(92);
      else
        Sound_Play(91);
      Event_Wait(20);
      Event_SetMessageId(MSG_COINS_WON);
      Func_0200403c(diff, 5);
      Event_ShowMessageAndWait(14, 0);
      Sound_WaitForJingle();
    } else if (diff < 0) {
      Event_SetMessageId(MSG_COINS_LOST);
      Func_0200403c(-diff, 5);
      Event_ShowMessageAndWait(14, 0);
    }
    Func_02004074();
  } else if (gMainRoutineWork.entrance == 19) {
    diff = gMainRoutineWork.coins - *(s32 *)gSceneState;
    Func_0200406c();
    Event_ResolvePendingActions(0);
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    if (diff > 0) {
      if (diff > 19999)
        Sound_Play(93);
      else if (diff > 4999)
        Sound_Play(92);
      else
        Sound_Play(91);
      Event_Wait(20);
      Event_SetMessageId(MSG_COINS_WON);
      Func_0200403c(diff, 5);
      Event_ShowMessageAndWait(14, 0);
      Sound_WaitForJingle();
    } else if (diff < 0) {
      Event_SetMessageId(MSG_COINS_LOST);
      Func_0200403c(-diff, 5);
      Event_ShowMessageAndWait(14, 0);
    }
    Func_02004074();
  } else if (gMainRoutineWork.entrance == 20) {
    list = ((struct ContigoPrizes *)&gMainRoutineWork)->list;
    Func_0200406c();
    Event_ResolvePendingActions(0);
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    Event_Wait(20);
    if (list[0] == -1) {
      Func_02001938(1);
    } else if (list[0] == -2) {
      Event_SetMessageId(MSG_READ_SIGN);
      Event_ShowMessageAndWait(18, 0);
    } else {
      Event_SetMessageId(MSG_CONGRATULATIONS);
      Event_ShowMessageAndWait(18, 0);
      if (list[0] != -1) {
        prize = list;
      next:
        if (prize == list)
          Event_SetMessageId(MSG_FIRST_PRIZE);
        else
          Event_SetMessageId(MSG_NEXT_PRIZE);
        item = Func_02001a60(*prize);
        Func_0200403c(item, 2);
        Event_ShowMessageAndWait(18, 0);
        Func_020041fc(item, 3);
        PartyInventory_GiveItem(item, 0);
        Actor_FaceDirectionAndWait(0, FACING_NORTH);
        prize++;
        if (*prize != -1)
          goto next;
      }
      ((struct ContigoPrizes *)&gMainRoutineWork)->list[0] = -2;
      Event_SetMessageId(MSG_COME_AGAIN);
      Event_ShowMessageAndWait(18, 0);
    }
    Func_02004074();
  }
}
