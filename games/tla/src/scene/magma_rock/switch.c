#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "sequence_parent_work.h"
#include "magma_rock.h"

struct Actor *Actor_GetById(s32 id);
void GameFlag_SetBit(s32 flag);
void GameFlag_ClearBit(s32 flag);
void FieldEvent_Wait(s32 frames);
void Actor_ResetAndSetPositionInMode2(s32 id, s32 x, s32 z);
void Func_020014bc(s32 mode, s32 cue);
void Func_02003aec(void);
void Func_02003af4(void);
void Func_02003b7c(s32 a);
void Func_02003bcc(s32 value);
void Func_02003c54(s32 a);

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
  struct SequenceParentWork *party = &gSequenceParentWork;

  /* FAKEMATCH: a one-pass block loads gSequenceParentWork before its offset is
   * built (one reordered row). */
  do {
  } while (0);
  if (party->renderMode != 2)
    GameFlag_ClearBit(0x2f8);
}

void Func_02001b44(void) {
  struct SequenceParentWork *party = &gSequenceParentWork;

  /* FAKEMATCH: a one-pass block loads gSequenceParentWork before its offset is
   * built (one reordered row). */
  do {
  } while (0);
  if (party->renderMode != 2) {
    Func_02003aec();
    Func_02003bcc(0);
    Actor_GetById(party->currentOwner)->motionFlags = 0;
    Actor_ResetAndSetPositionInMode2(party->currentOwner, 264, 440);
    FieldEvent_Wait(5);
    Func_02003c54(123);
    Func_02003b7c(2);
    Func_02003af4();
  }
  GameFlag_SetBit(0x2f8);
}
