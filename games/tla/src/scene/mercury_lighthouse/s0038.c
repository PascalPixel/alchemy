#include "types.h"
#include "main_routine_work.h"
#include "event_inline.h"

extern u32 gData_020007e4;

struct Actor;
void Func_020001b0(void);
void Func_02000568(void);
void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);

s32 Func_02000038(struct Actor *object) {
  if ((gData_020007e4 >> 1) & 1)
    Actor_SetPartAttribute(object, 15);
  else
    Actor_SetPartAttribute(object, 7);
  return 0;
}
