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
        Actor_SetHorizontalPositionWithTerrain(8, 0, 0);
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
      Sound_Play(124);
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
      Sound_Play(125);
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
      Sound_Play(125);
      if (state->mirrored != 0)
        Actor_SetModeById(9, 5);
      else
        Actor_SetModeById(11, 5);
      break;
    case 70:
    case 730:
      Func_020002a4(state->mirrored, state->stage);
      Sound_Play(138);
      break;
    }
  }
  road->field14++;
}
