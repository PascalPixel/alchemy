#include "types.h"
#include "main_routine_work.h"

void Party_SetFields1eeAnd1f0(u16 first, u16 second) {
  gMainRoutineWork.pair1ee[0] = first;
  gMainRoutineWork.pair1ee[1] = second;
}

void Party_SetFields1f2And1f4(u16 first, u16 second) {
  gMainRoutineWork.pair1f2[0] = first;
  gMainRoutineWork.pair1f2[1] = second;
}
