// Under -mthumb-slow-loads (2026-10-26): differ 4 of 63; left: the native reads
// position[1] at (index * 8 + 4) + base + 2, reusing position[0]'s offset; this
// form adds 6 to the scaled index. A position pointer, struct and s16 types
// were worse.
// Draft, differ 8 of 63 (10 before). The (owner, 0, -4) call and the table
// call now go through call.h, which fixes the -4 halves. Left: the native
// shifts the index after Func_02001344 returns and reads the second halfword
// through the table plus the first's offset (adds r3, r3, r0; ldrh r2, [r3,
// #2]); here the shift rises above the call and the second halfword is read as
// [table + offset + 6]. Tried 2026-10-15: a position pointer (12), the address
// of position[0] indexed (8), a one-pass block before the shift (10); permute
// 4 minutes found nothing below 16.
#include "types.h"
#include "ram_buffer.h"
#include "sequence_parent_work.h"
#include "actor.h"
#include "call.h"

struct IsletCaveEntry {
  s32 value;
  u16 position[2];
};

extern const struct IsletCaveEntry gData_020019ec[];

struct Actor *Actor_GetById(s32 id);
void Func_02001254(void);
void Func_0200125c(void);
void Func_02001324(s32 a);
void Func_02001344(s32 a);
void Func_0200122c(s32 value, s32 x, s32 z);
void Func_020012cc(s32 a);
void Actor_SetSpeedParameters(s32 id, s32 speed, s32 acceleration);
s32 Actor_OffsetPositionAndResetMotion(s32 id, s32 x, s32 z);
void FieldEvent_Wait(s32 frames);
void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);

void Func_020005fc(void) {
  u8 *work = Ram_HeapSlots->eventWork;
  u32 i;
  u32 index;

  Func_02001254();
  Func_02001324(0);
  /* FAKEMATCH: the loop is written with goto so that it carries no loop notes:
   * the bytes rebuild the zero on every pass, and a for loop hoists it. */
  i = 8;
loop:
  {
    struct Actor *object = Actor_GetById(i);

    if (object != 0)
      object->motionFlags = 0;
  }
  i++;
  if (i <= 63)
    goto loop;
  index = *(s16 *)(work + 0x154) - 1;
  Func_02001344(158);
  index >>= 1;
  Call3((void (*)())Func_0200122c, gData_020019ec[index].value,
        gData_020019ec[index].position[0], gData_020019ec[index].position[1]);
  Actor_SetModeById(gSequenceParentWork.currentOwner, 2);
  Actor_SetSpeedParameters(gSequenceParentWork.currentOwner, 0x8000, 0x4000);
  Call3((void (*)())Actor_OffsetPositionAndResetMotion,
        gSequenceParentWork.currentOwner, 0, -4);
  FieldEvent_Wait(6);
  Func_020012cc(*(s16 *)(work + 0x154));
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Func_0200125c();
}
