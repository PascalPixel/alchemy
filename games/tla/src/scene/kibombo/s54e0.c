#include "kibombo.h"

void Func_0200538c(void);

void Func_020054e0(void) {
  s16 entrance;

  Func_0200538c();
  if (GameFlag_Test(0x8fb)) {
    Func_02003640(1, 0);
    Func_020036e0(1, 5);
  }
  wait(1);
  entrance = gAdventure.entrance;
  if (entrance == 99) {
    Func_02003640(0, 0);
    Func_020036e0(0, 0);
    Actor_SetPositionAndResetMotion(gData_02007950, 0, 0, 0);
  } else if (entrance == 98) {
    Func_02003640(1, 0);
    Func_020036e0(1, 0);
    Actor_SetPositionAndResetMotion(gData_02007950, 0, 0, 0);
  } else if (entrance == 97) {
    Func_02003640(1, 0);
    Func_020036e0(1, 2);
    Actor_SetPositionAndResetMotion(gData_02007950, 0, 0, 0);
  } else if (entrance == 96) {
    Func_02003640(1, 1);
    Func_020036e0(1, 4);
    Actor_SetPositionAndResetMotion(gData_02007950, 0, 0, 0);
    Actor_SetModeById(14, 4);
  }
}
