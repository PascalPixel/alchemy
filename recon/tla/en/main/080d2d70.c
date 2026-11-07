// Matches: differ 0 of 8 (tla-it, 2026-11-07).
// Only the Spanish and Italian editions have it here, as their own scaffold
// function (es 0x080dbd64, it 0x080d2d64); event_no_op.c builds the other
// editions' copy. Its body is the same: it counts up the main routine's
// counter274.
#include "types.h"
#include "main_routine_work.h"

void Func_080d2d70(void) { gMainRoutineWork.counter274++; }
