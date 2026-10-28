#include "types.h"

extern u8 gAdventure[];
void Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 priority);
void FieldEffect_CheckCloakTile(void);

void FieldEffect_ResumeCloakCheck(void) {
  u8 *state;
  s8 *flag;

  /* FAKEMATCH: the one-pass block's end is a scheduling barrier: the native
   * loads the adventure's address before it builds the offset 0x26d (one
   * reordered row, differ 2 without). */
  do {
    state = gAdventure;
  } while (0);
  flag = (s8 *)state + 0x26d;
  if (*flag != 0)
    Scheduler_AddOrUpdateCallback(FieldEffect_CheckCloakTile, 0x480);
}
