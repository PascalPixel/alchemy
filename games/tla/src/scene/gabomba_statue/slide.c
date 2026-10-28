#include "types.h"
#include "actor.h"
#include "sequence_parent_work.h"

void Func_020056b4(struct Actor *object);
void Func_020056bc(void);
void Func_0200580c(s32 value);
void Func_02005854(s32 sound);
void Actor_SetSpeedParameters(s32 id, s32 speed, s32 acceleration);
void Actor_SetPositionAndCommit(s32 id, s32 x, s32 z);

void Func_020017dc(struct Actor *actor) {
  s32 acceleration;

  Func_020056b4(actor);
  Func_0200580c(0);
  Func_02005854(581);
  acceleration = 0x19999;
  /* FAKEMATCH: ⚓️ loads the speed and the acceleration from the pool separately
   */
  asm("" : "+l"(acceleration));
  Actor_SetSpeedParameters(gSequenceParentWork.currentOwner, 0x19999,
                                  acceleration);
  Actor_SetPositionAndCommit(gSequenceParentWork.currentOwner, 376, 200);
  Func_020056bc();
}

void Func_02001820(struct Actor *actor) {
  s32 acceleration;

  Func_020056b4(actor);
  Func_0200580c(0);
  Func_02005854(581);
  acceleration = 0x19999;
  /* FAKEMATCH: ⚓️ loads the speed and the acceleration from the pool separately
   */
  asm("" : "+l"(acceleration));
  Actor_SetSpeedParameters(gSequenceParentWork.currentOwner, 0x19999,
                                  acceleration);
  Actor_SetPositionAndCommit(gSequenceParentWork.currentOwner, 376, 136);
  Func_020056bc();
}

void Func_02001864(struct Actor *actor) {
  s32 acceleration;

  Func_020056b4(actor);
  Func_0200580c(0);
  Func_02005854(581);
  acceleration = 0x19999;
  /* FAKEMATCH: ⚓️ loads the speed and the acceleration from the pool separately
   */
  asm("" : "+l"(acceleration));
  Actor_SetSpeedParameters(gSequenceParentWork.currentOwner, 0x19999,
                                  acceleration);
  Actor_SetPositionAndCommit(gSequenceParentWork.currentOwner, 440, 248);
  Func_020056bc();
}

void Func_020018a8(struct Actor *actor) {
  s32 acceleration;

  Func_020056b4(actor);
  Func_0200580c(0);
  Func_02005854(581);
  acceleration = 0x19999;
  /* FAKEMATCH: ⚓️ loads the speed and the acceleration from the pool separately
   */
  asm("" : "+l"(acceleration));
  Actor_SetSpeedParameters(gSequenceParentWork.currentOwner, 0x19999,
                                  acceleration);
  Actor_SetPositionAndCommit(gSequenceParentWork.currentOwner, 440, 184);
  Func_020056bc();
}
