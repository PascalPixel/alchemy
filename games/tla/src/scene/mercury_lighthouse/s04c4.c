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
void wait(s32 frames);

s32 Func_020004c4(void) {
  if (gAdventure.entrance == 12) {
    Func_02000568();
    Func_020005f8(0);
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    Func_02000610(201);
    Func_020005d0(0x7fff, 0);
    Func_020005d8(20);
    wait(20);
    Func_020005c8(9);
  } else {
    Func_020001b0();
  }
  return 0;
}
