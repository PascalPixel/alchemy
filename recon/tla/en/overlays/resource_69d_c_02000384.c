// Draft, differ 27 of 162: Jupiter Lighthouse room 4's arrival script: sets the
// event work's word 0x1ac to 516, runs the map's setup and the parts flags 2567
// and 2568 enable; arriving by entrance 11 (flag 265 clear) the leader rises
// from 120 pixels below, spinning, for up to 90 steps of 2 frames, lands facing
// a quarter turn and falls into place; by entrance 12 Func_02001f00 runs.
// Returns 0. The rise is a goto loop (the native builds 0x1800 again
// each pass) behind its own entry test, storing y before the ground height,
// with the count after the wait. Left: the y read by the entry test is carried
// into the first pass in r2 where the native keeps it in r3, and the gravity
// store comes after the adventure's pool load.
#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "adventure.h"

struct Actor *Actor_GetById(s32 id);
s32 GameFlag_Test(s32 flag);
void wait(s32 frames);
void FieldEvent_Wait(s32 frames);
void Func_020000c0(void);
void Func_0200017c(void);
void Func_020001ac(void);
void Func_02000300(void);
void Func_02001e48(void);
void Func_02001e50(void);
void Func_02001ed0(s32 value);
void Func_02001f00(s32 id);
void Actor_SetSpritePriority(u32 id, s32 variant);
void Actor_SetModeById(u32 id, s32 mode);
void Actor_SetAnimationSpeed(struct Actor *object, s32 value);
void Event_StartScreenTransition(void);

s32 Func_02000384(void) {
  struct Actor *leader;
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
  if (flag == 0 && gAdventure.entrance == 11) {
    leader = Actor_GetById(gAdventure.currentOwner);
    leader->priorityFlags &= ~64;
    Func_02001e48();
    Func_02001ed0(0);
    Actor_SetSpritePriority(gAdventure.currentOwner, 1);
    Actor_SetModeById(gAdventure.currentOwner, 2);
    Actor_SetAnimationSpeed(leader, 64);
    leader->motionFlags = flag;
    leader->y = leader->terrainHeight = -0x780000;
    Event_StartScreenTransition();
    FieldEvent_Wait(10);
    i = 0;
    if (leader->y < 0) {
    rise:
      leader->terrainHeight = leader->y = leader->y + 0x30000;
      leader->angle += 0x1800;
      wait(2);
      i++;
      if (i <= 89 && leader->y < 0)
        goto rise;
    }
    leader->terrainHeight = leader->y = 0;
    leader->angle = 0x4000;
    leader->motionFlags = 3;
    leader->velocityY = 0x30000;
    leader->gravity = 0x8000;
    Actor_SetModeById(gAdventure.currentOwner, 1);
    FieldEvent_Wait(40);
    leader->gravity = 0x10000;
    leader->priorityFlags |= 64;
    Actor_GetById(gAdventure.currentOwner)->priorityFlags |= 1;
    Func_02001e50();
  }
  if (!GameFlag_Test(265) && gAdventure.entrance == 12)
    Func_02001f00(gAdventure.currentOwner);
  return 0;
}
