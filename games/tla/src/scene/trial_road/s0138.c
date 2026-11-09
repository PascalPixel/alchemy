#include "trial_road.h"
#include "event_inline.h"

s32 Func_02000330(s32 value);
s32 Func_020000fc(s32 value);
void ObjectEffect_StartContextEffect26(void);

void Func_02000138(s32 direction) {
  struct TrialRoadState *state = gTrialRoadState;
  s16 *order = (s16 *)state;

  if ((u32)(direction - 1) <= 1) {
    if (state->field16[0] <= 0) {
      if (GameFlag_Test(2559)) {
        if (Func_02000330(0) == 0)
          /* FAKEMATCH(wrapper): Call3 kept; as a direct call, 2 of 93
           * instructions differ in tla-en. */
          Actor_WalkByAndWait(gMainRoutineWork.currentOwner, 0, -16);
        return;
      }
      Import_EventSetExitRequest(direction + 4);
    } else {
      state->field16[0]--;
      Import_EventSetExitRequest(direction);
    }
    Sound_Play(123);
  }
  if ((u32)(direction - 3) <= 1) {
    state->field16[0]++;
    if (state->field16[0] > 3) {
      if (GameFlag_Test(2559) && Func_020000fc(0) == 0)
        GameFlag_SetBit(901);
      Sound_Play(128);
      ObjectEffect_StartContextEffect26();
      Import_EventSetExitRequest(direction + 4);
    } else {
      Import_EventSetExitRequest(direction);
      Sound_Play(123);
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
