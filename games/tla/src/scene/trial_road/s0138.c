#include "trial_road.h"
#include "call.h"

void Func_02005990(s32 direction);
s32 Func_02000330(s32 value);
s32 Func_020000fc(s32 value);
void Actor_CommitPositionAndActivate(s32 id, s32 dx, s32 dz);
void ObjectEffect_StartContextEffect26(void);

void Func_02000138(s32 direction) {
  /* FAKEMATCH(barrier): SPLIT_CONSTANT builds each constant as the game does, a
   * shifted byte plus an addend; as plain constants, 14 of 93 instructions
   * differ in tla-en. */
  struct TrialRoadState *state = gTrialRoadState;
  s16 *order = (s16 *)state;

  if ((u32)(direction - 1) <= 1) {
    if (state->field16[0] <= 0) {
      if (GameFlag_Test(SPLIT_CONSTANT(0x900, 255))) {
        if (Func_02000330(0) == 0)
          /* FAKEMATCH(wrapper): Call3 kept; as a direct call, 2 of 93
           * instructions differ in tla-en. */
          Call3(Actor_CommitPositionAndActivate, gMainRoutineWork.currentOwner,
                0, -16);
        return;
      }
      Func_02005990(direction + 4);
    } else {
      state->field16[0]--;
      Func_02005990(direction);
    }
    Func_02005a68(123);
  }
  if ((u32)(direction - 3) <= 1) {
    state->field16[0]++;
    if (state->field16[0] > 3) {
      if (GameFlag_Test(SPLIT_CONSTANT(0x900, 255)) && Func_020000fc(0) == 0)
        GameFlag_SetBit(SPLIT_CONSTANT(0x300, 133));
      Func_02005a68(128);
      ObjectEffect_StartContextEffect26();
      Func_02005990(direction + 4);
    } else {
      Func_02005990(direction);
      Func_02005a68(123);
    }
    if (state->field16[0] > state->field1a)
      state->field1a = state->field16[0];
  }
  {
    /* FAKEMATCH(temporary): the byte offset kept in a local; as order[index], 2
     * of 93 instructions differ in tla-en. */
    s32 offset = (state->field16[0] * 2 + state->mirrored) * 2;

    state->pattern = *(s16 *)((u8 *)order + offset);
  }
}
