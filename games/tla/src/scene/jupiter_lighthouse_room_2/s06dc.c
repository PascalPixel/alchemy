#include "types.h"
#include "main_routine_work.h"
#include "event_inline.h"


void Func_020006dc(void) {
  struct MainRoutineWork *party = &gMainRoutineWork;


  if (party->renderMode == 10)
    Func_080dbdd4(0x1500000, 0x2500000, 0, 255);
}
