// Near miss, differ 2 of 57 (tla-en, 2026-10-31): one swapped pair. The
// native spills the third argument (str r2, [sp, #16]) before building the
// heap slot address 0x030000a0; here that constant's first movs goes first.
// In sched2 the movs starts a chain into the work's load and the first call,
// priority 69 against the spill's 68, so no statement order moves it
// (scheduler order), as in its twin Func_080d9710. The argument setup of
// Func_080da6e0 now matches. u32 ids and declaring the locals before
// assigning them measured the same.
#include "worka0.h"
#include "actor.h"
#include "system.h"

extern s32 gData_080f394c;
s32 Trig_Sin(s32 angle);
void Func_080da6e0(s32 arg, s32 x1, s32 y1, s32 z1, s32 x2, s32 y2, s32 z2,
                   s32 wave);

void Func_080da660(s32 arg, s32 firstId, s32 secondId) {
  struct Actor **work = (struct Actor **)WORK_A0;
  struct Actor *first = ObjectTable_Get(firstId);
  struct Actor *second = ObjectTable_Get(secondId);

  if (first != NULL && second != NULL) {
    work[0] = first;
    work[1] = second;
    Func_080da6e0(arg, first->x, first->y + 0x80000, first->z, second->x,
                  second->y + 0x80000, second->z,
                  Trig_Sin(gData_080f394c) << 2);
    gData_080f394c += 0x1000;
  }
}
