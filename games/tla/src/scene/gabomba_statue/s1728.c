#include "types.h"
#include "actor.h"
#include "adventure.h"

struct Actor *Actor_GetById(s32 id);
void Func_020056b4(struct Actor *object);
void Func_020056bc(void);
void Func_0200580c(s32 value);
void Func_02005854(s32 sound);
void Actor_SetSpeedParameters(s32 id, s32 speed, s32 acceleration);
void Actor_SetPositionAndCommit(s32 id, s32 x, s32 z);
void FieldEvent_WaitFrames(s32 frames);

static __inline__ void Actor_SetSpeed(s32 id, s32 speed, s32 acceleration) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Actor_SetSpeedParameters(id, speed, acceleration);
}

static __inline__ void Actor_SetPosition(s32 id, s32 x, s32 z) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Actor_SetPositionAndCommit(id, x, z);
}

void Func_02001728(struct Actor *actor) {
  Func_020056b4(actor);
  Func_0200580c(0);
  Func_02005854(581);
  Actor_SetSpeed(gAdventure.currentOwner, 0x19999, 0x19999);
  Actor_SetPosition(gAdventure.currentOwner, 152, 424);
  Func_020056bc();
}

void Func_0200176c(void) {
  struct Actor *actor = Actor_GetById(gAdventure.currentOwner);
  s32 speed;

  Func_020056b4(actor);
  Func_0200580c(0);
  Func_02005854(581);
  actor->motionFlags = 0;
  speed = 0x19999;
  /* FAKEMATCH: the native loads the speed before the owner, whose base is
   * already in a register here; the plain call loads the owner first (one
   * reordered pair, differ 2) */
  asm volatile("" : "+l"(speed));
  Actor_SetSpeed(gAdventure.currentOwner, speed, 0x19999);
  Actor_SetPosition(gAdventure.currentOwner, 152, 488);
  actor->motionFlags = 3;
  FieldEvent_WaitFrames(8);
  Func_02005854(161);
  Actor_SetModeById(gAdventure.currentOwner, 22);
  FieldEvent_WaitFrames(20);
  Func_020056bc();
}
