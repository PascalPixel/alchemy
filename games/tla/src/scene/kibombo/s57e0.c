#include "kibombo.h"

void Func_02003c14(void);
void Func_02003c84(void);

#define PALETTE_WORD(index) (*(u16 *)(0x05000000 + (index) * 2))

void Func_020057e0(void) {
  Func_02005a9c();
  Func_02005c3c(0);
  GameFlag_SetBit(514);
  gData_02007948[0] = PALETTE_WORD(169);
  gData_02007948[1] = PALETTE_WORD(167);
  gData_02007948[2] = PALETTE_WORD(166);
  if (gAdventure.entrance == 10) {
    Actor_SetSingleChildFlags(Actor_GetById(gAdventure.currentOwner),
                                       0);
    Actor_SetPartAttribute(Actor_GetById(gAdventure.currentOwner), 15);
    Func_02003c84();
  } else if (gAdventure.entrance == 11) {
    Actor_SetSingleChildFlags(Actor_GetById(gAdventure.currentOwner),
                                       0);
    Actor_SetPartAttribute(Actor_GetById(gAdventure.currentOwner), 15);
    Func_02003c14();
  } else {
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    KIBOMBO_TASK_ADD(Func_02002d64, 1152);
    Func_02005aa4();
  }
}
