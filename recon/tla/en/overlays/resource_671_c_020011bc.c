// Draft, differ 43 of 79: the body of Func_02001630 (s1630.c) with the target
// as parameters. Left are the registers: the native keeps x in r6 and shifts it
// there (lsls r6, r6, #16), z in r8, the leader in r10 and the event work in
// r9; this form gives the leader r6 and x and z the high registers. Shifting x
// and z in place and a party pointer taken first change nothing. Overlay 670's
// Func_02003a08 is the same routine and is 4 off at its entry.
#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "adventure.h"

struct EventWork154 {
  u8 unk000[0x154];
  s16 exit;
};

struct Actor *Actor_GetById(s32 id);
void Func_020056b4(struct Actor *object);
void Func_0200580c(s32 value);
void Actor_SetSpeedParameters(s32 id, s32 speed, s32 acceleration);
void Actor_SetPositionAndReset(s32 id, s32 x, s32 z);
void Actor_ArmCallback(s32 id, s32 angle, s32 frames);
void Actor_SetSingleChildFlags(struct Actor *object,
                                        s32 flags);
void Func_0200565c(struct Actor *object, s32 x, s32 y, s32 z);
void Func_02005664(struct Actor *object);
void Func_02005854(s32 sound);
void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_020057c4(s32 exit);

static __inline__ void Actor_SetSpeed(s32 id, s32 speed, s32 acceleration) {
  /* FAKEMATCH: forwarding through the inline keeps each call's own constants,
   * as ☀️'s scene layer does. */
  Actor_SetSpeedParameters(id, speed, acceleration);
}

static __inline__ void Actor_Walk(s32 id, s32 x, s32 z) {
  /* FAKEMATCH: forwarding through the inline keeps each call's own constants,
   * as ☀️'s scene layer does. */
  Actor_SetPositionAndReset(id, x, z);
}

static __inline__ void Actor_Turn(s32 id, s32 angle, s32 frames) {
  /* FAKEMATCH: forwarding through the inline keeps each call's own constants,
   * as ☀️'s scene layer does. */
  Actor_ArmCallback(id, angle, frames);
}

static __inline__ void Actor_Place(struct Actor *object, s32 x, s32 y,
                                   s32 z) {
  /* FAKEMATCH: forwarding through the inline keeps each call's own constants,
   * as ☀️'s scene layer does. */
  Func_0200565c(object, x, y, z);
}

void Func_020011bc(s32 x, s32 z) {
  struct Adventure *party = &gAdventure;
  struct EventWork154 *work = Ram_HeapSlots->eventWork;
  struct Actor *leader = Actor_GetById(party->currentOwner);

  Func_020056b4(leader);
  Func_0200580c(0);
  leader->motionFlags = 0;
  Actor_SetSpeed(gAdventure.currentOwner, 0x10000, 0x8000);
  Actor_Walk(gAdventure.currentOwner, x, z);
  Actor_Turn(gAdventure.currentOwner, 0xc000, 0);
  Actor_RefreshSelectorById(gAdventure.currentOwner);
  Actor_SetSingleChildFlags(Actor_GetById(gAdventure.currentOwner),
                                     0);
  Actor_SetModeById(gAdventure.currentOwner, 13);
  Actor_Place(leader, x << 16, 0xfff40000, z << 16);
  Func_02005664(leader);
  Actor_SetModeById(gAdventure.currentOwner, 10);
  Func_02005854(123);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Func_020057c4(work->exit);
}
