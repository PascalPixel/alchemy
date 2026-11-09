#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "serial.h"
#include "task.h"
#include "text/msg_ids.h"
#include "event_inline.h"

struct LobbyEventWork {
  u8 unk000[0x16a];
  s16 raisedTrigger;
};

struct Window;

s32 Func_02000134(void);
s32 Func_0200006c(s32 slot);
s32 Func_020007b8(void);
void LinkLobby_WriteSlotValue(s32 slot);
void Func_02001944(void *callback, s32 mask);
void Func_0200198c(s32 layer);
void Func_02001994(s32 layer);
void Func_02001a14(void);
void Func_02001a1c(void);
void Func_02001a8c(s32 a, s32 b);
struct Window *Message_OpenWindow(const u8 *message, s32 x, s32 y, s32 flags);

extern char SceneId_Ov6b5;

// Touching the lobby counter: waits for the opponent, walks the lead to the
// arena door, swaps party records and starts the link battle.
s32 Func_02000864(void) {
  struct LobbyEventWork *work;
  s32 failed;
  struct Window *window;
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
  if (Event_GetFlag(0x173)) {
    Func_02001a14();
    Event_ResolvePendingActions(0);
  } else {
    if (!Event_GetFlag(0x200))
      return;
    if (Event_GetFlag(0x205))
      return;
    Func_02001a14();
    Event_ResolvePendingActions(0);
    Event_SetFlag(0x203);
    LinkLobby_WriteSlotValue(2);
    if (!Func_0200006c(2))
      window = Message_OpenWindow(MsgAwaitingYourOpponent, 5, 4, 1);
    while (!Func_0200006c(2)) {
      wait(1);
      stop = 0;
      if (!Event_GetFlag(0x201))
        stop = 1;
      if (Event_GetFlag(0x205))
        stop = 1;
      if (!Func_0200006c(2) && !Func_0200006c(1)) {
        if (++cnt > 25)
          stop = 1;
      } else {
        cnt = 0;
      }
      if (stop) {
        work->raisedTrigger = 2;
        Event_SetFlag(0x205);
        Event_ClearFlag(0x201);
        Event_ClearFlag(0x202);
        LinkLobby_WriteSlotValue(4);
        failed = 1;
        Event_ClearFlag(0x200);
        break;
      }
    }
    if (window)
      Window_Close(window, 1);
    wait(5);
  }
  if (!failed) {
    buf = HeapSlot_AllocEwram(216, 0x7c8);
    Task_RemoveFar(Func_02000134);
    Func_0200198c(5);
    wait(8);
    Func_02001994(5);
    if (Event_GetFlag(0x173)) {
      Actor_SetAngleToward(8, gMainRoutineWork.currentOwner, 0);
      Event_SetMessageId((s32)MsgGoodLuck2);
      FieldMessage_ShowAtActor(8, 0);
      wait(45);
      Actor_SetSpeed(4, 0x10000, 0x8000);
      Actor_ResetAndSetPositionInMode2(4, 216, 184);
      Actor_WaitForMoveById(4);
      Actor_ResetAndSetPositionInMode2(4, 216, 168);
      Actor_WaitForMoveById(4);
    } else {
      Actor_SetSpeed(4, 0x10000, 0x8000);
      Actor_ResetAndSetPositionInMode2(4, 216, 200);
      Actor_WaitForMoveById(4);
      Actor_SetSpeed(4, 0x1999, 0xccc);
      Actor_ResetAndSetPositionInMode2(4, 216, 168);
      if (Func_020007b8() < 0) {
        Actor_SetSpeed(4, 0x10000, 0x8000);
        Actor_ResetAndSetPositionInMode2(4, 216, 200);
        Func_0200198c(5);
        wait(8);
        Func_02001994(5);
        Actor_WaitForMoveById(4);
        HeapSlot_Release(216);
        LinkLobby_WriteSlotValue(0);
        LinkLobby_WriteSlotValue(4);
        Task_Add((TaskFunc)Func_02000134, TASK_PASS_VBLANK);
        Func_02001944(Func_02000134, 1);
        Event_ClearFlag(0x201);
        Event_ClearFlag(0x202);
        Event_ClearFlag(0x303);
        Event_ClearFlag(0x203);
        Event_ClearFlag(0x200);
        work->raisedTrigger = 2;
        Func_02001a1c();
        return;
      }
      Actor_SetSpeedParameters(4, 0x8000, 0x4000);
      Actor_WaitForMoveById(4);
    }
    /* FAKEMATCH(temporary): the flag test's result kept while the scene
     * symbol is loaded once for both branches; as if (Event_GetFlag(0x173)), 2 of
     * 329 instructions differ in tla-en. */
    set = Event_GetFlag(0x173);
    scene = (s32)&SceneId_Ov6b5;
    if (set) {
      Party_SetFields1eeAnd1f0(scene, 8);
      Party_SetFields1f2And1f4(scene, 9);
    } else {
      Party_SetFields1eeAnd1f0(scene, 10);
      Party_SetFields1f2And1f4(scene, 11);
    }
    gMainRoutineWork.battleRule24b = 4;
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
    HeapSlot_Release(216);
  }
  Func_02001a1c();
}
