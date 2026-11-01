// Matches: differ 0 (tla-en, 2026-11-01)
// Twin of The Broken Seal's Kalay_OpenDoorAndExit: the same door-exit event
// (stop every actor from id 8, trigger number - 1 picks an 8-byte cell-animation
// entry, a sound, the door animates, the leader walks in, the exit is requested
// with the trigger number). It matched before the twin was compared, so no form
// was ported. The twin's two locals and Sound_Play order agree; here the
// FAKEMATCH(temporary) stays because the twin's plain reads differ by 4 (below).
// The native reads position[0]
// straight from the table, then reads position[1] through a pointer built from
// the table and the same scaled offset (adds r3, r3, r0; ldrh r2, [r3, #2]);
// the FAKEMATCH(temporary) below keeps that order. Read straight from the
// table, position[1] folds into [table + index * 8 + 6] (differ 4); a pointer
// from &gData_020019ec[index].position folds the 4 into the table's literal
// (12); the offset added inline also folds (4); the pointer built before
// position[0] is read takes both reads (2).
// Earlier: the (owner, 0, -4) call and the table call go through call.h,
// which fixes the -4 halves.
#include "types.h"
#include "stddef.h"
#include "ram_buffer.h"
#include "main_routine_work.h"
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
  // A goto loop carries no loop notes: the bytes rebuild the zero on every
  // pass; as a for loop, loop.c hoists it and 20 of 63 instructions differ.
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
  {
    s32 x = gData_020019ec[index].position[0];
    /* FAKEMATCH(temporary): position[0] read first and position[1] read
     * through a pointer from a separate byte offset; as
     * gData_020019ec[index].position[1] in the call, 4 of 63 instructions
     * differ in tla-en. */
    u32 offset = index * sizeof(struct IsletCaveEntry) +
                 offsetof(struct IsletCaveEntry, position);
    const u16 *position = (const u16 *)((const u8 *)gData_020019ec + offset);

    Call3((void (*)())Func_0200122c, gData_020019ec[index].value, x,
          position[1]);
  }
  Actor_SetModeById(gMainRoutineWork.currentOwner, 2);
  Actor_SetSpeedParameters(gMainRoutineWork.currentOwner, 0x8000, 0x4000);
  Call3((void (*)())Actor_OffsetPositionAndResetMotion,
        gMainRoutineWork.currentOwner, 0, -4);
  FieldEvent_Wait(6);
  Func_020012cc(*(s16 *)(work + 0x154));
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Func_0200125c();
}
