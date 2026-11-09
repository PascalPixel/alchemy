#include "lemurian_ship.h"

void Func_02001a9c(void) {
  Func_02002c68();
  Event_ResolvePendingActions(0);
  Sound_Play(164);
  Event_Wait(120);
  Func_02002c70();
}
