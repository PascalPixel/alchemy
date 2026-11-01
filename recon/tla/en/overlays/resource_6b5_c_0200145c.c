// Draft, differ 2 of 438, skeleton gap 1 (tla-en, 2026-11-01)
// Twin of ☀️'s BattleArena_RunRoundResult (overlay resource_3cb, scene/
// battle_arena.c): overlay 6b5's lobby set-up by entrance, same order of
// statements, entrances 8 to 11 and the fall-through arrival, same tallies,
// slot writes, party flags and "TALK" name stores. ⚓️ adds Flag_Set(0x17e) and
// two more cleared halfwords at the top.
// Left: one sched2 tie in entrance 8: linkTallies[3]'s offset, split after
// reload, puts its shift before the first tally's store; the native has it
// after. Seven spellings of the two increments (+= 1, = x + 1, ++x, pointer
// forms) leave the pair as it is, one shared pointer is worse (74); it stays
// a near miss.
// Tried for the flag loop, which needs the counter in r5 and the flag number
// in r6, the reverse of the twin's r6 and r5: the counter (priority 33/28)
// loses to the flag number (12/8) in global allocation, and sched2 then
// hoists the counter's clear past the calls. Direct calls, a named flag
// local in the loop and inline wrappers change nothing (differ 18). The
// counter holding the entrance (differ 12) gave it the priority but moved
// the entrance from r3 to r5; the counter as score is worse (76), and earlier
// notes measured it as wins, msg or v at 18 to 55.
// Sharing the variable that later holds the wins lengthens the flag number's
// live range enough, so that reuse replaces the entrance one.
// Devices in place: one scalar holds the score row's address, then its score,
// in entrances 8 and 9, and entrance 10's best streak reuses that slot.
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

static inline s32 Flag_GetByte(s32 flag) { return GameFlag_GetByte(flag); }

static inline void Flag_SetByte(s32 flag, s32 value) {
  GameFlag_SetByte(flag, value);
}

static inline void Map_CopyCells(s32 x, s32 y, s32 width, s32 height, s32 destX,
                                 s32 destY) {
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
     * value, 18 of 438 instructions differ in tla-en. */
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
    wins = (s8)Flag_GetByte(0x3f8);
    msg = wins * 2 + 2;
    if (msg > 14)
      msg = 14;
    v = Flag_GetByte(1000);
    if (v == 2) {
      Flag_SetByte(1000, 0);
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
      Flag_SetByte(0x3f8, wins);
    } else {
      Flag_Clear(0x173);
      Flag_SetByte(0x3f8, -1);
      /* FAKEMATCH: one scalar holds the row address, then its score, as in ☀️'s
       * entrance 9 */
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
    /* FAKEMATCH: one scalar holds the row address, then its score, as in ☀️'s
     * entrance 9 */
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
    Flag_SetByte(0x3f8, -1);
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
      /* FAKEMATCH: reuse the other score branches' address slot, as ☀️ does */
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
    Flag_SetByte(0x3f8, -1);
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
