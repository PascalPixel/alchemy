#include "types.h"
#include "adventure.h"

extern u32 gData_020007e4;

struct Actor;
void Actor_SetPartAttribute(struct Actor *object, s32 value);
void Func_020001b0(void);
void Func_02000568(void);
void Func_020005c8(s32 a);
void Func_020005d0(s32 a, s32 b);
void Func_020005d8(s32 a);
void Func_020005f8(s32 skip);
void Func_02000610(s32 cue);
void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);
void WaitFrames(s32 frames);

s32 Func_02000038(struct Actor *object) {
  if ((gData_020007e4 >> 1) & 1)
    Actor_SetPartAttribute(object, 15);
  else
    Actor_SetPartAttribute(object, 7);
  return 0;
}
