// Draft, differ 48 of 162: Jupiter Lighthouse room 4's arrival script: sets the
// event work's word 0x1ac to 516, runs the map's setup and the parts flags 2567
// and 2568 enable; arriving by entrance 11 (flag 265 clear) the leader rises
// from 120 pixels below, spinning, for up to 90 steps of 2 frames, lands facing
// a quarter turn and falls into place; by entrance 12 Func_02001f00 runs.
// Returns 0.
#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"
#include "party_state.h"

struct ObjectRuntime *Object_GetById(s32 id);
s32 GameFlag_Test(s32 flag);
void WaitFrames(s32 frames);
void FieldEvent_WaitFrames(s32 frames);
void Func_020000c0(void);
void Func_0200017c(void);
void Func_020001ac(void);
void Func_02000300(void);
void Func_02001e48(void);
void Func_02001e50(void);
void Func_02001ed0(s32 value);
void Func_02001f00(s32 id);
void ObjectMotion_SetActionVariant(u32 id, s32 variant);
void Object_SetModeById(u32 id, s32 mode);
void ObjectDispatch_SetAnimationSpeed(struct ObjectRuntime *object, s32 value);
void Event_StartScreenTransition(void);

s32 Func_02000384(void) {
  struct ObjectRuntime *leader;
  s32 flag;
  s32 i;

  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 516;
  Func_020000c0();
  if (GameFlag_Test(2567))
    Func_0200017c();
  if (GameFlag_Test(2568))
    Func_020001ac();
  Func_02000300();
  flag = GameFlag_Test(265);
  if (flag == 0 && gPartyState.entrance == 11) {
    leader = Object_GetById(gPartyState.currentOwner);
    leader->priorityFlags &= ~64;
    Func_02001e48();
    Func_02001ed0(0);
    ObjectMotion_SetActionVariant(gPartyState.currentOwner, 1);
    Object_SetModeById(gPartyState.currentOwner, 2);
    ObjectDispatch_SetAnimationSpeed(leader, 64);
    leader->motionFlags = flag;
    leader->y = leader->terrainHeight = -0x780000;
    Event_StartScreenTransition();
    FieldEvent_WaitFrames(10);
    i = 0;
    while (leader->y < 0) {
      leader->y = leader->terrainHeight = leader->y + 0x30000;
      leader->angle += 0x1800;
      i++;
      WaitFrames(2);
      if (i > 89)
        break;
    }
    leader->y = leader->terrainHeight = 0;
    leader->angle = 0x4000;
    leader->motionFlags = 3;
    leader->velocityY = 0x30000;
    leader->gravity = 0x8000;
    Object_SetModeById(gPartyState.currentOwner, 1);
    FieldEvent_WaitFrames(40);
    leader->gravity = 0x10000;
    leader->priorityFlags |= 64;
    Object_GetById(gPartyState.currentOwner)->priorityFlags |= 1;
    Func_02001e50();
  }
  if (!GameFlag_Test(265) && gPartyState.entrance == 12)
    Func_02001f00(gPartyState.currentOwner);
  return 0;
}
