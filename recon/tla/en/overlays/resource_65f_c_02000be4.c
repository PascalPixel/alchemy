// Draft, differ 10 of 137: Garoh's first room setup, restoring the
// transformed villagers or their palette. Left: register choice.
#include "../../../../games/tla/src/scene/garoh/garoh.h"
#include "party_state.h"
#include "event_work.h"
void Event_StartScreenTransition(void);
void Func_02000a68(void);
void Func_0200538c(void);

void Func_02000be4(void) {
  Scheduler_AddOrUpdateCallback(Func_02000a68, 0x480);
  ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0;
  if (GameFlag_Test(0x91b) == 0 || GameFlag_Test(0x310) != 0) {
    GameFlag_SetBit(0x310);
    Func_020052cc(0x10007, 1);
    if (GameFlag_Test(0x311))
      Func_020052cc(0x10003, 2);
    Func_020052d4(1);
    Event_WaitTransitionFrames();
    Func_0200538c();
    {
      u8 *palette = *(u8 **)((u8 *)Ram_HeapSlots + 0x80);

      *(volatile u16 *)0x0500003c = 0x7fff;
      *(u16 *)(palette + 60) = *(volatile u16 *)0x0500003c;
    }
    Func_020052cc(0x10000, 1);
  } else {
    Func_02005184(10, 32, 10, 10, 1, 1);
    Func_02005184(12, 32, 25, 22, 1, 1);
    Func_02005184(11, 33, 8, 22, 1, 1);
    Func_02005184(74, 32, 74, 10, 1, 1);
    Func_02005184(76, 32, 89, 22, 1, 1);
    Func_02005184(75, 33, 72, 22, 1, 1);
  }
  if (gPartyState.entrance == 12) {
    struct EventWork *work = Ram_HeapSlots->eventWork;

    work->screenEffect = 0x209;
    GameFlag_ClearBit(0x120);
    GameFlag_ClearBit(0x121);
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0;
  }
}
