#include "types.h"

extern u8 gMainRoutineWork[];
void Task_Add(void (*func)(void), s32 priority);
void FieldEffect_CheckCloakTile(void);

void FieldEffect_ResumeCloakCheck(void) {
  u8 *state;
  s8 *flag;

  /* FAKEMATCH: the one-pass block's end is a scheduling barrier: the native
   * loads gMainRoutineWork's address before it builds the offset 0x26d (one
   * reordered row, differ 2 without). */
  do {
    state = gMainRoutineWork;
  } while (0);
  flag = (s8 *)state + 0x26d;
  if (*flag != 0)
    Task_Add(FieldEffect_CheckCloakTile, 0x480);
}
