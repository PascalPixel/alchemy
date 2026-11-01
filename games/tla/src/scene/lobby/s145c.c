#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "flag_call.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "text/msg_ids.h"

struct LobbyEventWork {
  u8 unk000[0x16a];
  s16 raisedTrigger;
  u8 unk16c[0x40];
  s32 startTransition;
};

extern s32 gData_020022b8;
extern s32 gData_020022bc;
extern u16 gData_02003a74[];
extern u8 gAutoSleepEnabled;

s32 Func_02000134(void);
void Func_02000044(void);
s32 Func_02000f6c(s32 owner);
s32 Func_020013ec(void);
s32 Func_02001400(void);
void Func_02001414(s32 value);
void Func_02001914(void);
void Func_02001944(void *func, s32 mask);
void Func_02001994(s32 entry);
void Func_0200199c(s32 x, s32 y, s32 width, s32 height, s32 destX, s32 destY);
void Func_020019e4(s32 value, s32 digits);
void Func_02001a04(s32 value);
void Func_02001a14(void);
void Func_02001a1c(void);
void Func_02001a64(s32 message);
void Func_02001a74(s32 actor, s32 flags);
void Func_02001ab4(s32 skip);
void Actor_SetAngleToward(s32 actor, s32 target, s32 frames);
void Sound_LoadPresetParameters(s32 preset);
void LinkLobby_WriteSlotValue(s32 value);
void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);
void wait(s32 frames);
void Task_Add(void *func, s32 priority);
s32 GameFlag_GetByte(s32 flag);
void GameFlag_SetByte(s32 flag, s32 value);

static inline s32 Flag_GetByte(s32 flag) {
  /* FAKEMATCH(wrapper): Flag_GetByte forwarded inline; as direct calls to
   * GameFlag_GetByte, 60 of 438 instructions differ in tla-en. */
  return GameFlag_GetByte(flag);
}

static inline void Flag_SetByte(s32 flag, s32 value) {
  /* FAKEMATCH(wrapper): Flag_SetByte forwarded inline; as direct calls to
   * GameFlag_SetByte, 17 of 438 instructions differ in tla-en. */
  GameFlag_SetByte(flag, value);
}

static inline void Map_CopyCells(s32 x, s32 y, s32 width, s32 height, s32 destX,
                                 s32 destY) {
  /* FAKEMATCH(wrapper): Map_CopyCells forwarded inline; as direct calls to
   * Func_0200199c, 6 of 438 instructions differ in tla-en. */
  Func_0200199c(x, y, width, height, destX, destY);
}

s32 Func_0200145c(void) {
  s32 i;
  s32 wins;
  u32 score;
  struct LobbyEventWork *work;
  void *func;

  Flag_Set(0x17e);
  gMainRoutineWork.packedEvents[0] = 0;
  gMainRoutineWork.packedEvents[1] = 0;
  gData_020022bc = 0;
  gData_020022b8 = 0;
  ((struct LobbyEventWork *)Ram_HeapSlots->eventWork)->startTransition = 0x201;
  Sound_LoadPresetParameters(2);
  Func_02001414(gMainRoutineWork.linkTallies[6]);
  Map_CopyCells(11, 11, 1, 1, 13, 10);
  LinkLobby_WriteSlotValue(4);
  wait(1);
  Func_02001994(5);
  {
    u16 *name = gData_02003a74;

    name[4] = 'T';
    name[5] = 'A';
    name[6] = 'L';
    name[7] = 'K';
  }
  for (i = 0; i < 8; i++) {
    /* FAKEMATCH(temporary): wins, reused, holds the flag number; as a separate
     * value, 16 of 438 instructions differ in tla-en. */
    wins = 0x2f0 + i;
    Flag_Clear(wins);
    if (Func_02000f6c(i))
      Flag_Set(wins);
  }
  if (gMainRoutineWork.entrance == 8) {
    s32 msg;
    s32 v;

    Func_02001a14();
    Func_02001ab4(0);
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    LinkLobby_WriteSlotValue(5);
    gMainRoutineWork.linkTallies[0]++;
    gMainRoutineWork.linkTallies[3]++;
    wins = (s8)GameFlag_GetByte(0x3f8);
    msg = wins * 2 + 2;
    if (msg > 14)
      msg = 14;
    v = Flag_GetByte(1000);
    if (v == 2) {
      GameFlag_SetByte(1000, 0);
      wins++;
      msg++;
    } else {
      Flag_SetByte(1000, v + 1);
    }
    Actor_SetAngleToward(8, gMainRoutineWork.currentOwner, 0);
    Func_02001a64((s32)MsgNotBadWould + msg);
    FieldMessage_ShowAtActor(8, 0);
    if (Event_AskYesNoWithActorAnimation(4, 0) == 0) {
      if (wins > 90)
        wins = 90;
      GameFlag_SetByte(0x3f8, wins);
    } else {
      Flag_Clear(0x173);
      GameFlag_SetByte(0x3f8, -1);
      /* FAKEMATCH(temporary): one scalar holds the row address, then its score;
       * as the row read directly in entrances 8 and 9, 36 of 438 instructions
       * differ in tla-en. */
      score = (u32)&gMainRoutineWork.linkTallies[3];
      Func_020019e4(*(u16 *)score, 5);
      score = *(u16 *)score;
      if (gMainRoutineWork.linkTallies[2] < score) {
        gMainRoutineWork.linkTallies[2] = score;
        Func_02001a64((s32)MsgYouWonBattle);
        FieldMessage_ShowAtActor(8, 0);
        Func_02001400();
      } else {
        Func_02001a64((s32)MsgYouWonBattlePlease);
        FieldMessage_ShowAtActor(8, 0);
      }
      LinkLobby_WriteSlotValue(0);
    }
    Func_02001a1c();
  } else if (gMainRoutineWork.entrance == 9) {
    gMainRoutineWork.linkTallies[1]++;
    Func_02001a14();
    Func_02001ab4(0);
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    LinkLobby_WriteSlotValue(5);
    Actor_SetAngleToward(8, gMainRoutineWork.currentOwner, 0);
    /* FAKEMATCH(temporary): one scalar holds the row address, then its score;
     * as the row read directly in entrances 8 and 9, 36 of 438 instructions
     * differ in tla-en. */
    score = (u32)&gMainRoutineWork.linkTallies[3];
    Func_020019e4(*(u16 *)score, 5);
    score = *(u16 *)score;
    if (gMainRoutineWork.linkTallies[2] < score) {
      gMainRoutineWork.linkTallies[2] = score;
      Func_02001a64((s32)MsgYouWonBattle);
      FieldMessage_ShowAtActor(8, 0);
      Func_02001400();
    } else {
      Func_02001a64((s32)MsgYouWonBattleI);
      FieldMessage_ShowAtActor(8, 0);
    }
    gMainRoutineWork.linkTallies[3] = 0;
    Flag_Clear(0x173);
    GameFlag_SetByte(0x3f8, -1);
    LinkLobby_WriteSlotValue(0);
    Func_02001a1c();
  } else if (gMainRoutineWork.entrance == 10) {
    s32 v;

    Func_02001a14();
    Func_02001ab4(0);
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    LinkLobby_WriteSlotValue(0);
    LinkLobby_WriteSlotValue(4);
    if (Flag_IsSet(1000)) {
      work = Ram_HeapSlots->eventWork;
      Flag_Clear(1000);
      work->raisedTrigger = 2;
      Flag_Clear(0x304);
      wait(20);
      Func_02000044();
      LinkLobby_WriteSlotValue(0);
      LinkLobby_WriteSlotValue(4);
    } else {
      gMainRoutineWork.linkTallies[4]++;
      v = gMainRoutineWork.linkTallies[7] + 1;
      gMainRoutineWork.linkTallies[7] = v;
      /* FAKEMATCH(temporary): the other score branches' address slot reused; as
       * the row read directly, 8 of 438 instructions differ in tla-en. */
      score = (u32)&gMainRoutineWork.linkTallies[6];
      if (*(u16 *)score < (u16)v)
        *(u16 *)score = v;
      Func_02001414(*(u16 *)score);
      Func_020013ec();
      Flag_Set(0x304);
      Flag_Set(0x305);
    }
    Func_02001a1c();
  } else if (gMainRoutineWork.entrance == 11) {
    s32 v;

    Func_02001a14();
    Func_02001ab4(0);
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    LinkLobby_WriteSlotValue(0);
    LinkLobby_WriteSlotValue(4);
    if ((v = Flag_IsSet(0x173)) == 0) {
      gMainRoutineWork.linkTallies[5]++;
      gMainRoutineWork.linkTallies[7] = v;
      Func_020013ec();
    }
    Flag_Set(0x304);
    Flag_Clear(0x305);
    Func_02001a1c();
  } else {
    Func_02001914();
    Flag_Clear(0x172);
    GameFlag_SetByte(0x3f8, -1);
    if (gMainRoutineWork.unk244[6] != 0) {
      Func_02001a14();
      Func_02001ab4(0);
      Event_StartScreenTransition();
      Event_WaitTransitionFrames();
      Actor_SetAngleToward(8, gMainRoutineWork.currentOwner, 0);
      Func_02001a64((s32)MsgNoteYouCan);
      Func_02001a74(8, 0);
      Func_02001a1c();
    }
    gMainRoutineWork.unk244[6] = 0;
    gAutoSleepEnabled = 0;
    LinkLobby_WriteSlotValue(0);
    LinkLobby_WriteSlotValue(4);
  }
  func = Func_02000134;
  Task_Add(func, 0x480);
  Func_02001944(func, 1);
  if (gMainRoutineWork.entrance != 8 || !Flag_IsSet(0x173)) {
    Func_02001a04(1);
    DjinnRecovery_CountDownTwenty();
  }
  return 0;
}
