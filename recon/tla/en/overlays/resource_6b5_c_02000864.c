// Draft of overlay 6b5's Func_02000864 (0x02008864), ported from ☀️'s
// EV10_TOUCH: differ 4 of 329. Devices in place, each measured: the 0x173
// test's result kept while the scene symbol is loaded once for both
// branches; a one-pass block loading gPartyState before the battle rule's
// offset; the copy loop as a goto loop with its bound in a variable and the
// byte read ahead of the increments (as ☀️'s do/while or a for loop, loop.c
// hoists and strength-reduces the destination, differ 18-20). Left: after
// the payload's first store the native loads 0x43 and stores it before the
// bound's movs r4, #217; sched2 here ties them at priority 3 and prefers the
// bound, which does not depend on the store just scheduled. One-pass blocks
// between the stores, store orders and five minutes of alchemy permute did
// not find it.
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
#include "party_state.h"
#include "ram_buffer.h"
#include "serial_runtime.h"
#include "callback_scheduler.h"
#include "text/msg_ids.h"

struct LobbyEventWork {
  u8 unk000[0x16a];
  s16 raisedTrigger;
};

struct UiWindow;

s32 Func_02000134(void);
s32 Func_0200006c(s32 slot);
s32 Func_020007b8(void);
void LinkLobby_WriteSlotValue(s32 slot);
void Func_02001944(void *callback, s32 mask);
void Func_0200198c(s32 layer);
void Func_02001994(s32 layer);
void Func_02001a14(void);
void Func_02001a1c(void);
void Func_02001a64(s32 message);
void Func_02001a8c(s32 a, s32 b);
void Func_02001ab4(s32 skip);
void Party_SetFields1eeAnd1f0(s32 scene, s32 entrance);
void Party_SetFields1f2And1f4(s32 scene, s32 entrance);
void Scheduler_RemoveCallbackFar(void *callback);
struct UiWindow *UiText_OpenMessageWindow(const u8 *message, s32 x, s32 y,
                                          s32 flags);
void ObjectMotion_SetAngleToward(s32 actor, s32 target, s32 frames);
void ObjectMotion_SetSpeedParameters(s32 actor, s32 speed, s32 acceleration);
void ObjectMotion_ResetAndSetPositionInMode2(s32 actor, s32 x, s32 z);
void ObjectMotion_CommitCurrentPositionAndActivate(s32 actor);

extern char SceneId_LinkLobby;

static inline void Actor_SetSpeed(s32 actor, s32 speed, s32 acceleration) {
  /* FAKEMATCH: as in ☀️'s scene layer, the inline parameters build each speed
   * again at each call; direct calls share registers */
  ObjectMotion_SetSpeedParameters(actor, speed, acceleration);
}

// Touching the lobby counter: waits for the opponent, walks the lead to the
// arena door, swaps party records and starts the link battle.
s32 Func_02000864(void) {
  struct LobbyEventWork *work;
  s32 failed;
  struct UiWindow *window;
  s32 cnt;
  s32 stop;
  u8 *buf;
  u8 *src;
  u16 *table;
  u32 i;
  s32 scene;
  s32 set;

  failed = 0;
  window = 0;
  work = Ram_HeapSlots->eventWork;
  cnt = 0;
  if (Flag_IsSet(0x173)) {
    Func_02001a14();
    Func_02001ab4(0);
  } else {
    if (!Flag_IsSet(0x200))
      return;
    if (Flag_IsSet(0x205))
      return;
    Func_02001a14();
    Func_02001ab4(0);
    Flag_Set(0x203);
    LinkLobby_WriteSlotValue(2);
    if (!Func_0200006c(2))
      window = UiText_OpenMessageWindow(MsgAwaitingYourOpponent, 5, 4, 1);
    while (!Func_0200006c(2)) {
      WaitFrames(1);
      stop = 0;
      if (!Flag_IsSet(0x201))
        stop = 1;
      if (Flag_IsSet(0x205))
        stop = 1;
      if (!Func_0200006c(2) && !Func_0200006c(1)) {
        if (++cnt > 25)
          stop = 1;
      } else {
        cnt = 0;
      }
      if (stop) {
        work->raisedTrigger = 2;
        Flag_Set(0x205);
        Flag_Clear(0x201);
        Flag_Clear(0x202);
        LinkLobby_WriteSlotValue(4);
        failed = 1;
        Flag_Clear(0x200);
        break;
      }
    }
    if (window)
      UiWork_Finalize(window, 1);
    WaitFrames(5);
  }
  if (!failed) {
    buf = Runtime_AllocateBlock(216, 0x7c8);
    Scheduler_RemoveCallbackFar(Func_02000134);
    Func_0200198c(5);
    WaitFrames(8);
    Func_02001994(5);
    if (Flag_IsSet(0x173)) {
      ObjectMotion_SetAngleToward(8, gPartyState.currentOwner, 0);
      Func_02001a64((s32)MsgGoodLuck2);
      UiText_OpenMessageAtObject(8, 0);
      WaitFrames(45);
      Actor_SetSpeed(4, 0x10000, 0x8000);
      ObjectMotion_ResetAndSetPositionInMode2(4, 216, 184);
      ObjectMotion_CommitCurrentPositionAndActivate(4);
      ObjectMotion_ResetAndSetPositionInMode2(4, 216, 168);
      ObjectMotion_CommitCurrentPositionAndActivate(4);
    } else {
      Actor_SetSpeed(4, 0x10000, 0x8000);
      ObjectMotion_ResetAndSetPositionInMode2(4, 216, 200);
      ObjectMotion_CommitCurrentPositionAndActivate(4);
      Actor_SetSpeed(4, 0x1999, 0xccc);
      ObjectMotion_ResetAndSetPositionInMode2(4, 216, 168);
      if (Func_020007b8() < 0) {
        Actor_SetSpeed(4, 0x10000, 0x8000);
        ObjectMotion_ResetAndSetPositionInMode2(4, 216, 200);
        Func_0200198c(5);
        WaitFrames(8);
        Func_02001994(5);
        ObjectMotion_CommitCurrentPositionAndActivate(4);
        Runtime_ReleaseHeapBlock(216);
        LinkLobby_WriteSlotValue(0);
        LinkLobby_WriteSlotValue(4);
        Scheduler_AddOrUpdateCallback((SchedulerCallback)Func_02000134, 0x480);
        Func_02001944(Func_02000134, 1);
        Flag_Clear(0x201);
        Flag_Clear(0x202);
        Flag_Clear(0x303);
        Flag_Clear(0x203);
        Flag_Clear(0x200);
        work->raisedTrigger = 2;
        Func_02001a1c();
        return;
      }
      Actor_SetSpeed(4, 0x8000, 0x4000);
      ObjectMotion_CommitCurrentPositionAndActivate(4);
    }
    set = Flag_IsSet(0x173);
    scene = (s32)&SceneId_LinkLobby;
    if (set) {
      Party_SetFields1eeAnd1f0(scene, 8);
      Party_SetFields1f2And1f4(scene, 9);
    } else {
      Party_SetFields1eeAnd1f0(scene, 10);
      Party_SetFields1f2And1f4(scene, 11);
    }
    {
      struct PartyState *party = &gPartyState;

      do {
      } while (0);
      party->battleRule24b = 4;
    }
    Func_02001a8c(1, 1);
    table = (u16 *)gSerialTransfer.payload;
    table[0] = 0x45;
    table[1] = 0x58;
    table[2] = 0x45;
    table[3] = 0x43;
    {
      u32 last = 0x7c7;

      i = 0;
      src = buf;
    copy:
      {
        u8 *dst = Ram_MapCellBuffer + 0x8000 + i;
        u8 b = *src;

        i++;
        src++;
        *dst = b;
      }
      if (i <= last)
        goto copy;
    }
    Runtime_ReleaseHeapBlock(216);
  }
  Func_02001a1c();
}
