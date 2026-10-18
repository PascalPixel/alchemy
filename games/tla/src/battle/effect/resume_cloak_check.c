#include "types.h"

extern u8 gPartyState[];
void Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 priority);
void FieldFx_CheckCloakTile(void);

void FieldFx_ResumeCloakCheck(void) {
  u8 *state;
  s8 *flag;

  /* FAKEMATCH: the one-pass block's end is a scheduling barrier: the native
   * loads the party state's address before it builds the offset 0x26d (one
   * reordered row, differ 2 without). */
  do {
    state = gPartyState;
  } while (0);
  flag = (s8 *)state + 0x26d;
  if (*flag != 0)
    Scheduler_AddOrUpdateCallback(FieldFx_CheckCloakTile, 0x480);
}
