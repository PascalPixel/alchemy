#include "types.h"
#include "field_event.h"

extern u8 gSceneState[];

void TrialRoad_Idle(void) {}

void TrialRoad_SetBalanceStateReady(void) {
  u16 *state = (u16 *)gSceneState;
  /* FAKEMATCH(temporary): a local holding 9 builds it with movs; as a plain
   * constant, which is a halfword pool load, 4 of 4 instructions differ in
   * tla-en. */
  u16 ready = 9;

  *state = ready;
}

void TrialRoad_WaitForBalanceState(void) {
  s16 *state = (s16 *)gSceneState;

  while (*state != 9) {
    wait(1);
  }
}
