// Draft, differ 23 of 93: steps the Trial Road's stage by direction: 1 and 2 go
// back a stage (at the first, with flag 2559 set, the party is moved back 16
// pixels unless Func_02000330 objects), 3 and 4 go on (past the fourth they
// start effect 26, sound 128 and, with flag 2559 set and no slot from
// Func_020000fc, flag 901), each with its walk (Func_02005990, direction + 4 at
// either end) and sound 123; then the furthest stage reached and the stage's
// pattern (from the dealt order and mirroring) are updated.
#include "../../../../games/tla/src/scene/trial_road/trial_road.h"

void Func_02005990(s32 direction);
s32 Func_02000330(s32 value);
s32 Func_020000fc(s32 value);
void Actor_CommitPositionAndActivate(s32 id, s32 dx, s32 dz);
void ObjectEffect_StartContextEffect26(void);

void Func_02000138(s32 direction) {
  struct TrialRoadState *state = gTrialRoadState;
  s16 *order = (s16 *)state;

  if ((u32)(direction - 1) <= 1) {
    if (state->field16[0] <= 0) {
      if (GameFlag_Test(2559)) {
        if (Func_02000330(0) == 0)
          Actor_CommitPositionAndActivate(gMainRoutineWork.currentOwner, 0,
                                                 -16);
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
      if (GameFlag_Test(2559) && Func_020000fc(0) == 0)
        GameFlag_SetBit(901);
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
  state->pattern = order[state->field16[0] * 2 + state->mirrored];
}
