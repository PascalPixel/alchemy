// Draft, differ 165 of 223: the Trial Road stage's frame counter (a scheduled
// callback while fewer than four stages are done): at the start of a stage
// (Func_020056e8) it loads the stage's pattern (Func_020056c4 from
// gData_02005b84) and resets object 8, or after the fourth hides object 8;
// object 8 shows while the stage is the current one; then, unless flag 260 is
// set, it plays the stage's timed beats: frames 1 and 601 open the chests
// (objects 9 to 12 in mode 6, sound 124), 655, 50 and 710 play motion 5 on one
// of them with sound 125, 653 and 708 turn object 8, and 70 and 730 end
// the stage (Func_020002a4, sound 138). Object 8's flag is set in an if and
// else on the object, as the native stores 1 or 0 after a branch (193 with the
// comparison stored). Left: after the best score the native reads the stage
// again for the increment, its switch builds each case value afresh where CSE
// here derives 653 and 728 from 708, and the timer's halfword for the increment
// is read early in each case.
#include "../../../../games/tla/src/scene/trial_road/trial_road.h"

struct TrialRoadStage {
  s16 order[8];
  s16 pattern;
  s16 mirrored;
  s16 timer;
  s16 current;
  s16 stage;
  s16 unk1a;
  s16 best;
};

extern const s32 *const gData_02005b84[];

s32 Func_020056e8(void);
s32 Func_020000fc(s32 value);
void Func_020056c4(const s32 *pattern);
void Func_020002a4(s32 mirrored, s32 stage);

void Func_0200223c(void) {
  struct TrialRoadStage *state = (struct TrialRoadStage *)gTrialRoadState;
  s32 side;

  if (state->stage > 3)
    return;
  if (Func_020056e8() != 0) {
    if (state->timer == 0) {
      side = Func_020000fc(1) ^ 1;
      Func_020056c4(gData_02005b84[side + 8]);
      state->timer = side * 5 * 120 + 1;
    } else {
      if (state->stage >= state->best)
        state->best = state->stage + 1;
      if (++state->stage <= 3) {
        Func_020056c4(gData_02005b84[state->order[state->stage * 2 +
                                                  (s16)(state->mirrored ^ 1)] &
                                     7]);
        Object_GetById(8)->terrainHeight = 0;
        Object_GetById(8)->y = 0;
      } else {
        Func_020058e0(8, 0, 0);
      }
      state->timer = 0;
    }
  }
  {
    struct ObjectRuntime *marker = Object_GetById(8);

    if (state->current == state->stage)
      marker->animationKind = 1;
    else
      marker->animationKind = 0;
  }
  if (state->timer == 0)
    return;
  if (GameFlag_Test(260))
    return;
  if (state->current == state->stage) {
    switch (state->timer) {
    case 1:
    case 601:
      Func_02005a68(124);
      if (state->mirrored != 0) {
        Object_SetModeById(9, 6);
        if (state->timer > 600)
          Object_SetModeById(10, 6);
      } else {
        Object_SetModeById(11, 6);
        if (state->timer > 600)
          Object_SetModeById(12, 6);
      }
      break;
    case 655:
      Func_02005a68(125);
      Object_SetModeById(state->mirrored != 0 ? 10 : 12, 5);
      break;
    case 653:
    case 708:
      Object_GetById(8)->angle = 0xc000;
      break;
    case 50:
    case 710:
      Func_02005a68(125);
      Object_SetModeById(state->mirrored != 0 ? 9 : 11, 5);
      break;
    case 70:
    case 730:
      Func_020002a4(state->mirrored, state->stage);
      Func_02005a68(138);
      break;
    }
  }
  state->timer++;
}
