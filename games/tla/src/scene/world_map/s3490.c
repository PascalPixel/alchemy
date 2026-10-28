#include "types.h"
#include "actor.h"
#include "adventure.h"

struct Actor *Actor_GetById(s32 id);
void WaitFrames(s32 frames);
void Actor_SetPartAttribute(struct Actor *object, s32 value);
void Actor_SetSingleChildFlags(struct Actor *object,
                                        s32 flags);
void Actor_SetPositionAndCommit(s32 actor, s32 x, s32 z);
void Event_StartScreenTransition(void);
void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_0200343c(void);
void Func_02004c20(void);
void Func_02004c28(void);
void Func_02004d40(s32 value);
void Func_02004db8(s32 value);
void Func_02004df8(s32 value, s32 mode);

static __inline__ void Actor_SetPositionAndCommit_(s32 actor, s32 x,
                                                          s32 z) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Actor_SetPositionAndCommit(actor, x, z);
}

static __inline__ void Func_02004df8_(s32 value, s32 mode) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_02004df8(value, mode);
}

void Func_02003490(void) {
  Func_02004c20();
  Func_02004db8(0);
  Actor_SetPartAttribute(Actor_GetById(gAdventure.currentOwner), 15);
  Actor_SetSingleChildFlags(Actor_GetById(gAdventure.currentOwner),
                                     0);
  Actor_GetById(gAdventure.currentOwner)->update = Func_0200343c;
  Actor_GetById(gAdventure.currentOwner)->acceleration = 0x2000;
  Actor_GetById(gAdventure.currentOwner)->speedLimit = 0x20000;
  WaitFrames(5);
  Func_02004df8_(0xc000, 60);
  Event_StartScreenTransition();
  Actor_SetPositionAndCommit_(gAdventure.currentOwner, 14537, 8577);
  Actor_SetPositionAndCommit_(gAdventure.currentOwner, 14465, 8662);
  Actor_SetPositionAndCommit_(gAdventure.currentOwner, 14452, 8766);
  Actor_SetPositionAndCommit_(gAdventure.currentOwner, 14387, 8828);
  Actor_SetPositionAndCommit_(gAdventure.currentOwner, 14331, 8888);
  Actor_SetPositionAndCommit_(gAdventure.currentOwner, 14284, 8904);
  Actor_SetPositionAndCommit_(gAdventure.currentOwner, 14255, 8870);
  Actor_SetPositionAndCommit_(gAdventure.currentOwner, 14255, 8831);
  Actor_SetPositionAndCommit_(gAdventure.currentOwner, 14255, 8772);
  Func_02004df8_(0x14000, 120);
  Actor_SetPositionAndCommit_(gAdventure.currentOwner, 14275, 8736);
  Actor_SetPositionAndCommit_(gAdventure.currentOwner, 14257, 8720);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Func_02004c28();
  Func_02004d40(78);
}
