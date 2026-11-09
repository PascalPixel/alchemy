#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "magma_rock.h"
#include "event_inline.h"

void Func_020014bc(s32 mode, s32 cue);
void Func_02003aec(void);
void Func_02003af4(void);

void Func_02001a8c(s32 mode, s32 cue) {
  struct MagmaRockWork *work = Ram_HeapSlots->effectWork;

  work->target = Actor_GetById(17);
  Func_020014bc(mode, cue);
  GameFlag_SetBit(0x202);
}

void Func_02001ac0(s32 mode, s32 cue) {
  struct MagmaRockWork *work = Ram_HeapSlots->effectWork;

  work->target = Actor_GetById(18);
  Func_020014bc(mode, cue);
  GameFlag_SetBit(0x203);
}

void Func_02001af4(s32 mode, s32 cue) {
  struct MagmaRockWork *work = Ram_HeapSlots->effectWork;

  work->target = Actor_GetById(11);
  Func_020014bc(mode, cue);
  GameFlag_SetBit(0x204);
}

void Func_02001b24(void) {
  struct MainRoutineWork *party = &gMainRoutineWork;


  if (party->renderMode != 2)
    GameFlag_ClearBit(0x2f8);
}

void Func_02001b44(void) {
  struct MainRoutineWork *party = &gMainRoutineWork;


  if (party->renderMode != 2) {
    Func_02003aec();
    Event_ResolvePendingActions(0);
    Actor_GetById(party->currentOwner)->motionFlags = 0;
    Actor_ResetAndSetPositionInMode2(party->currentOwner, 264, 440);
    Event_Wait(5);
    Sound_Play(123);
    Import_EventSetExitRequest(2);
    Func_02003af4();
  }
  GameFlag_SetBit(0x2f8);
}
