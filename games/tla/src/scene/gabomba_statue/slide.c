#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "event_inline.h"

void Func_020056b4(struct Actor *object);
void Func_020056bc(void);

void Func_020017dc(struct Actor *actor) {
  s32 acceleration;

  Func_020056b4(actor);
  Event_ResolvePendingActions(0);
  Sound_Play(581);
  acceleration = 0x19999;
  /* FAKEMATCH: ⚓️ loads the speed and the acceleration from the pool separately
   */
  asm("" : "+l"(acceleration));
  Actor_SetSpeedParameters(gMainRoutineWork.currentOwner, 0x19999,
                                  acceleration);
  Actor_SetPositionAndCommit(gMainRoutineWork.currentOwner, 376, 200);
  Func_020056bc();
}

void Func_02001820(struct Actor *actor) {
  s32 acceleration;

  Func_020056b4(actor);
  Event_ResolvePendingActions(0);
  Sound_Play(581);
  acceleration = 0x19999;
  /* FAKEMATCH: ⚓️ loads the speed and the acceleration from the pool separately
   */
  asm("" : "+l"(acceleration));
  Actor_SetSpeedParameters(gMainRoutineWork.currentOwner, 0x19999,
                                  acceleration);
  Actor_SetPositionAndCommit(gMainRoutineWork.currentOwner, 376, 136);
  Func_020056bc();
}

void Func_02001864(struct Actor *actor) {
  s32 acceleration;

  Func_020056b4(actor);
  Event_ResolvePendingActions(0);
  Sound_Play(581);
  acceleration = 0x19999;
  /* FAKEMATCH: ⚓️ loads the speed and the acceleration from the pool separately
   */
  asm("" : "+l"(acceleration));
  Actor_SetSpeedParameters(gMainRoutineWork.currentOwner, 0x19999,
                                  acceleration);
  Actor_SetPositionAndCommit(gMainRoutineWork.currentOwner, 440, 248);
  Func_020056bc();
}

void Func_020018a8(struct Actor *actor) {
  s32 acceleration;

  Func_020056b4(actor);
  Event_ResolvePendingActions(0);
  Sound_Play(581);
  acceleration = 0x19999;
  /* FAKEMATCH: ⚓️ loads the speed and the acceleration from the pool separately
   */
  asm("" : "+l"(acceleration));
  Actor_SetSpeedParameters(gMainRoutineWork.currentOwner, 0x19999,
                                  acceleration);
  Actor_SetPositionAndCommit(gMainRoutineWork.currentOwner, 440, 184);
  Func_020056bc();
}
