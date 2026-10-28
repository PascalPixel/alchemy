#include "local.h"
#include "input.h"

void Func_02002998(void);
void Func_020029a0(void);
void Func_02002a48(s32 a);
void Func_02002a70(s32 cue);
void Func_02001d30(struct Actor *object);
void Actor_SetPositionAndCommit(s32 id, s32 x, s32 z);

void Func_02001460(void) {
  struct Actor *object = Actor_GetById(gSequenceParentWork.currentOwner);
  s32 saved = object->motionFlags;
  s32 i;
  s32 angle;
  s32 up;
  struct SequenceParentWork *state;

  Func_02002a70(612);
  Func_02002998();
  Func_02002a48(0);
  Event_StartScreenTransition();
  Actor_SetSingleChildFlags(object, 0);
  object->motionFlags = 0;
  object->y = -0x100000;
  Actor_SetSpeedParameters(gSequenceParentWork.currentOwner, 0x20000, 0x10000);
  for (i = 15; i >= 0; i--) {
    object->y += 0x10000;
    angle = object->angle;
    /* FAKEMATCH: keeps the angle load before its step is built (differ 2
     * without) */
    __asm__("" : : "r"(angle));
    angle += 0x2000;
    object->angle = angle;
    wait(1);
  }
  Func_02001d30(object);
  object->motionFlags = saved;
  object->velocityY = 0x80000;
  Actor_SetSingleChildFlags(object, 1);
  up = gInput.held & 0x40;
  state = &gSequenceParentWork;
  if (up)
    Actor_SetPositionAndCommit(state->currentOwner, 264, 760);
  else
    Actor_SetPositionAndCommit(state->currentOwner, 264, 824);
  Func_020029a0();
}
