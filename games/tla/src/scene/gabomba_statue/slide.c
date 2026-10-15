#include "types.h"
#include "object_runtime.h"
#include "party_state.h"

void Func_020056b4(struct ObjectRuntime *object);
void Func_020056bc(void);
void Func_0200580c(s32 value);
void Func_02005854(s32 sound);
void ObjectMotion_SetSpeedParameters(s32 id, s32 speed, s32 acceleration);
void ObjectMotion_SetPositionAndCommit(s32 id, s32 x, s32 z);

void Func_020017dc(struct ObjectRuntime *actor) {
  s32 acceleration;

  Func_020056b4(actor);
  Func_0200580c(0);
  Func_02005854(581);
  acceleration = 0x19999;
  /* FAKEMATCH: ⚓️ loads the speed and the acceleration from the pool separately
   */
  asm("" : "+l"(acceleration));
  ObjectMotion_SetSpeedParameters(gPartyState.currentOwner, 0x19999,
                                  acceleration);
  ObjectMotion_SetPositionAndCommit(gPartyState.currentOwner, 376, 200);
  Func_020056bc();
}

void Func_02001820(struct ObjectRuntime *actor) {
  s32 acceleration;

  Func_020056b4(actor);
  Func_0200580c(0);
  Func_02005854(581);
  acceleration = 0x19999;
  /* FAKEMATCH: ⚓️ loads the speed and the acceleration from the pool separately
   */
  asm("" : "+l"(acceleration));
  ObjectMotion_SetSpeedParameters(gPartyState.currentOwner, 0x19999,
                                  acceleration);
  ObjectMotion_SetPositionAndCommit(gPartyState.currentOwner, 376, 136);
  Func_020056bc();
}

void Func_02001864(struct ObjectRuntime *actor) {
  s32 acceleration;

  Func_020056b4(actor);
  Func_0200580c(0);
  Func_02005854(581);
  acceleration = 0x19999;
  /* FAKEMATCH: ⚓️ loads the speed and the acceleration from the pool separately
   */
  asm("" : "+l"(acceleration));
  ObjectMotion_SetSpeedParameters(gPartyState.currentOwner, 0x19999,
                                  acceleration);
  ObjectMotion_SetPositionAndCommit(gPartyState.currentOwner, 440, 248);
  Func_020056bc();
}

void Func_020018a8(struct ObjectRuntime *actor) {
  s32 acceleration;

  Func_020056b4(actor);
  Func_0200580c(0);
  Func_02005854(581);
  acceleration = 0x19999;
  /* FAKEMATCH: ⚓️ loads the speed and the acceleration from the pool separately
   */
  asm("" : "+l"(acceleration));
  ObjectMotion_SetSpeedParameters(gPartyState.currentOwner, 0x19999,
                                  acceleration);
  ObjectMotion_SetPositionAndCommit(gPartyState.currentOwner, 440, 184);
  Func_020056bc();
}
