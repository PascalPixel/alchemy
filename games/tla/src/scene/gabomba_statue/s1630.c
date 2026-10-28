#include "types.h"
#include "ram_buffer.h"
#include "input.h"
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
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Actor_SetSpeedParameters(id, speed, acceleration);
}

static __inline__ void Actor_Walk(s32 id, s32 x, s32 z) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Actor_SetPositionAndReset(id, x, z);
}

static __inline__ void Actor_Turn(s32 id, s32 angle, s32 frames) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Actor_ArmCallback(id, angle, frames);
}

static __inline__ void Actor_Place(struct Actor *object, s32 x, s32 y,
                                   s32 z) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_0200565c(object, x, y, z);
}

/// Jumps down the hole.
void Func_02001630(void) {
  struct Actor *object = Actor_GetById(gAdventure.currentOwner);
  s32 x = object->x >> 20;
  s32 z = object->z >> 20;
  struct Actor *leader;
  struct EventWork154 *work;

  if (x == 28 && z == 9 && !(gInput.held & 0x10))
    return;
  if (x == 29 && z == 10 && !(gInput.held & 0x40))
    return;
  work = Ram_HeapSlots->eventWork;
  leader = Actor_GetById(gAdventure.currentOwner);
  Func_020056b4(leader);
  Func_0200580c(0);
  leader->motionFlags = 0;
  Actor_SetSpeed(gAdventure.currentOwner, 0x10000, 0x8000);
  Actor_Walk(gAdventure.currentOwner, 472, 144);
  Actor_Turn(gAdventure.currentOwner, 0xc000, 0);
  Actor_RefreshSelectorById(gAdventure.currentOwner);
  Actor_SetSingleChildFlags(Actor_GetById(gAdventure.currentOwner),
                                     0);
  Actor_SetModeById(gAdventure.currentOwner, 13);
  Actor_Place(leader, 472 << 16, 0, 156 << 16);
  Func_02005664(leader);
  Actor_SetModeById(gAdventure.currentOwner, 10);
  Func_02005854(123);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Func_020057c4(work->exit);
}
