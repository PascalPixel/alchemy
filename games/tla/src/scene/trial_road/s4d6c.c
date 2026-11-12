#include "trial_road.h"

void Func_02004ce0(s32 a, s32 b, s32 owner);

void Func_02004d6c(s32 a, s32 b) {
  Func_02004ce0(a, b, gMainRoutineWork.currentOwner);
}

extern const u8 gData_020063ac[];

/// Writes value as eight hexadecimal digits and a terminating null.
UNUSED void String_WriteU32AsHex(u8 *buf, u32 value) {
  s32 i;
  const u8 *digits = gData_020063ac;

  buf += 8;
  *buf = 0;
  buf--;
  i = 7;
  // A goto loop, which builds the 15 on every pass: as a for loop, the 15 is
  // moved out of it (differ 13).
next:
  *buf = digits[value & 15];
  value >>= 4;
  buf--;
  i--;
  if (i >= 0)
    goto next;
}

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
