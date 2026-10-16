// Draft, differ 43 of 79: the body of Func_02001630 (s1630.c) with the target
// as parameters. Left are the registers: the native keeps x in r6 and shifts it
// there (lsls r6, r6, #16), z in r8, the leader in r10 and the event work in
// r9; this form gives the leader r6 and x and z the high registers. Shifting x
// and z in place and a party pointer taken first change nothing. Overlay 670's
// Func_02003a08 is the same routine and is 4 off at its entry.
#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"
#include "party_state.h"

struct EventWork154 {
  u8 unk000[0x154];
  s16 exit;
};

struct ObjectRuntime *Object_GetById(s32 id);
void Func_020056b4(struct ObjectRuntime *object);
void Func_0200580c(s32 value);
void ObjectMotion_SetSpeedParameters(s32 id, s32 speed, s32 acceleration);
void ObjectMotion_SetPositionAndReset(s32 id, s32 x, s32 z);
void ObjectMotion_ArmCallback(s32 id, s32 angle, s32 frames);
void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object,
                                        s32 flags);
void Func_0200565c(struct ObjectRuntime *object, s32 x, s32 y, s32 z);
void Func_02005664(struct ObjectRuntime *object);
void Func_02005854(s32 sound);
void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_020057c4(s32 exit);

static __inline__ void Actor_SetSpeed(s32 id, s32 speed, s32 acceleration) {
  /* FAKEMATCH: forwarding through the inline keeps each call's own constants,
   * as ☀️'s scene layer does. */
  ObjectMotion_SetSpeedParameters(id, speed, acceleration);
}

static __inline__ void Actor_Walk(s32 id, s32 x, s32 z) {
  /* FAKEMATCH: forwarding through the inline keeps each call's own constants,
   * as ☀️'s scene layer does. */
  ObjectMotion_SetPositionAndReset(id, x, z);
}

static __inline__ void Actor_Turn(s32 id, s32 angle, s32 frames) {
  /* FAKEMATCH: forwarding through the inline keeps each call's own constants,
   * as ☀️'s scene layer does. */
  ObjectMotion_ArmCallback(id, angle, frames);
}

static __inline__ void Actor_Place(struct ObjectRuntime *object, s32 x, s32 y,
                                   s32 z) {
  /* FAKEMATCH: forwarding through the inline keeps each call's own constants,
   * as ☀️'s scene layer does. */
  Func_0200565c(object, x, y, z);
}

void Func_020011bc(s32 x, s32 z) {
  struct PartyState *party = &gPartyState;
  struct EventWork154 *work = Ram_HeapSlots->eventWork;
  struct ObjectRuntime *leader = Object_GetById(party->currentOwner);

  Func_020056b4(leader);
  Func_0200580c(0);
  leader->motionFlags = 0;
  Actor_SetSpeed(gPartyState.currentOwner, 0x10000, 0x8000);
  Actor_Walk(gPartyState.currentOwner, x, z);
  Actor_Turn(gPartyState.currentOwner, 0xc000, 0);
  Object_RefreshSelectorById(gPartyState.currentOwner);
  ObjectDispatch_SetSingleChildFlags(Object_GetById(gPartyState.currentOwner),
                                     0);
  Object_SetModeById(gPartyState.currentOwner, 13);
  Actor_Place(leader, x << 16, 0xfff40000, z << 16);
  Func_02005664(leader);
  Object_SetModeById(gPartyState.currentOwner, 10);
  Func_02005854(123);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Func_020057c4(work->exit);
}
