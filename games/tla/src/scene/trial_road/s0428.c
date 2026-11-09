#include "trial_road.h"
#include "event_inline.h"

void Func_02005880(void);
void Func_02005888(void);
void TrialRoad_Idle(void);
s32 Func_020000fc(s32 value);

void Func_02000428(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  struct TrialRoadState *state = gTrialRoadState;
  s32 found;

  Func_02005880();
  Event_ResolvePendingActions(0);
  Actor_SetPositionAndReset(gMainRoutineWork.currentOwner,
                                   ((leader->x >> 20) << 4) + 8,
                                   ((leader->z >> 20) << 4) + 8);
  TrialRoad_Idle();
  found = Func_020000fc(0);
  if (found != 0)
    GameFlag_SetBit(state->field16[0] + 897);
  Sound_Play(124);
  if (state->mirrored == 0) {
    Actor_SetModeById(9, 6);
    GameFlag_SetBit(645);
    if (found != 0) {
      Actor_SetModeById(10, 6);
      GameFlag_SetBit(646);
    }
  } else {
    Actor_SetModeById(11, 6);
    GameFlag_SetBit(645);
    if (found != 0) {
      Actor_SetModeById(12, 6);
      GameFlag_SetBit(646);
    }
  }
  GameFlag_SetBit(644);
  Func_02005888();
}
