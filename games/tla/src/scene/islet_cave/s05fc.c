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
