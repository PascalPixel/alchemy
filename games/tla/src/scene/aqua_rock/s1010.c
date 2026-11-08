#include "local.h"

void Func_02002968(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_02001f60(s32 id);

void Func_02001010(void) {
  Func_02002960_(62, 41, 1, 2, 33, 41);
  Func_02002968(125, 32, 3, 13, 96, 32);
  Func_02002968(61, 96, 3, 13, 32, 96);
  Actor_SetPartAttribute(Actor_GetById(12), 5);
  Func_02001f60(12);
  GameFlag_SetBit(0xa6b);
}

void Func_02002998(void);
void Func_020029a0(void);
void Func_02002a48(s32 a);
void Func_020029f8(s32 id, s32 value);

void Func_0200106c(void) {
  struct Actor *object = Actor_GetById(gMainRoutineWork.currentOwner);
  s32 saved = object->gravity;
  s32 i;
  struct MainRoutineWork *state;

  Func_02002998();
  Func_02002a48(0);
  Func_020029f8(gMainRoutineWork.currentOwner, 257);
  Func_02002910(object, 40);
  object->motionFlags = 3;
  for (i = 0; i < 180; i++) {
    wait(1);
    if (object->y <= object->terrainHeight)
      break;
  }
  state = &gMainRoutineWork;
  object->gravity = saved;
  /* FAKEMATCH: keeps the gravity store before the owner offset is built (differ
   * 2 without) */
  __asm__("");
  Func_020029f8(state->currentOwner, 0);
  Actor_SetSingleChildFlags(object, 1);
  Func_02002910(object, 1);
  state->renderMode = 0;
  Func_020029a0();
}
