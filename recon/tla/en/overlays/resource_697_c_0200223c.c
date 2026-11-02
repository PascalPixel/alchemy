// Matches: differ 0 (tla-en, 2026-11-01)
// The Trial Road stage's frame counter, a scheduled callback while fewer than
// four stages are done: at the start of a stage (Func_020056e8) it loads the
// stage's pattern (Func_020056c4 from gData_02005b84) and resets object 8, or
// after the fourth hides object 8; object 8 shows while the stage is the
// current one; then, unless flag 260 is set, it plays the stage's timed beats:
// frames 1 and 601 open the chests (objects 9 to 12 in mode 6, sound 124), 655,
// 50 and 710 play motion 5 on one of them with sound 125, 653 and 708 turn
// object 8, and 70 and 730 end the stage (Func_020002a4, sound 138).
// Three forms to check on adoption, each what closed the differ, none tagged:
// - road: the two increments read through struct TrialRoadState and the rest
//   through this draft's view, the same two-struct habit s242c.c documents.
//   Through one struct GCSE merges the increments' reads with the compares'
//   (differ 148: the timer read in each case, no reread of the stage). No cast
//   is added, but the only effect is the alias set: review it as a disguise.
// - pattern: without the local the read narrows to ldrh under `& 7` and two
//   lines swap (differ 14). Read as S2's `temporary` it would need a tag.
// - kind: the byte's address is built before the compare; through the actor in
//   each arm it is built twice (differ 13), and a stored 0 or 1 becomes a
//   store-flag sequence.
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
  struct TrialRoadState *road = gTrialRoadState;
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
      if (++road->field16[1] <= 3) {
        s32 pattern =
            state->order[state->stage * 2 + (s16)(state->mirrored ^ 1)];

        Func_020056c4(gData_02005b84[pattern & 7]);
        Actor_GetById(8)->terrainHeight = 0;
        Actor_GetById(8)->y = 0;
      } else {
        Func_020058e0(8, 0, 0);
      }
      state->timer = 0;
    }
  }
  {
    u8 *kind = &Actor_GetById(8)->animationKind;

    if (state->current == state->stage)
      *kind = 1;
    else
      *kind = 0;
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
        Actor_SetModeById(9, 6);
        if (state->timer > 600)
          Actor_SetModeById(10, 6);
      } else {
        Actor_SetModeById(11, 6);
        if (state->timer > 600)
          Actor_SetModeById(12, 6);
      }
      break;
    case 655:
      Func_02005a68(125);
      if (state->mirrored != 0)
        Actor_SetModeById(10, 5);
      else
        Actor_SetModeById(12, 5);
      break;
    case 653:
    case 708:
      Actor_GetById(8)->angle = 0xc000;
      break;
    case 50:
    case 710:
      Func_02005a68(125);
      if (state->mirrored != 0)
        Actor_SetModeById(9, 5);
      else
        Actor_SetModeById(11, 5);
      break;
    case 70:
    case 730:
      Func_020002a4(state->mirrored, state->stage);
      Func_02005a68(138);
      break;
    }
  }
  road->field14++;
}
