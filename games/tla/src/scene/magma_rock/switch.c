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

  work->target = Actor_Get(17);
  Func_020014bc(mode, cue);
  Event_SetFlag(0x202);
}

void Func_02001ac0(s32 mode, s32 cue) {
  struct MagmaRockWork *work = Ram_HeapSlots->effectWork;

  work->target = Actor_Get(18);
  Func_020014bc(mode, cue);
  Event_SetFlag(0x203);
}

void Func_02001af4(s32 mode, s32 cue) {
  struct MagmaRockWork *work = Ram_HeapSlots->effectWork;

  work->target = Actor_Get(11);
  Func_020014bc(mode, cue);
  Event_SetFlag(0x204);
}

void Func_02001b24(void) {
  struct MainRoutineWork *party = &gMainRoutineWork;


  if (party->renderMode != 2)
    Event_ClearFlag(0x2f8);
}

void Func_02001b44(void) {
  struct MainRoutineWork *party = &gMainRoutineWork;


  if (party->renderMode != 2) {
    Func_02003aec();
    FieldEvent_ResolvePendingActions(0);
    Actor_Get(party->currentOwner)->motionFlags = 0;
    Actor_WalkTo(party->currentOwner, 264, 440);
    FieldEvent_Wait(5);
    Event_PlaySound(123);
    FieldEvent_SetExitRequest(2);
    Func_02003af4();
  }
  Event_SetFlag(0x2f8);
}
