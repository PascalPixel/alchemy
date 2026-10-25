// Draft, differ 2 of 79: puts an item down on the Trial Road: centres the
// leader on its cell, idles, and when Func_020000fc finds a slot sets that
// slot's flag (stage field16[0] + 897); with sound 124 it shows object 9 (or 11
// when mirrored) in mode 6 with flag 645, object 10 (or 12) with flag 646 when
// a slot was found, then sets flag 644.
#include "../../../../games/tla/src/scene/trial_road/trial_road.h"

void Func_02005880(void);
void Func_02005888(void);
void Func_020059e8(s32 value);
void ObjectMotion_SetPositionAndReset(s32 id, s32 x, s32 z);
void TrialRoad_Idle(void);
s32 Func_020000fc(s32 value);

void Func_02000428(void) {
  struct ObjectRuntime *leader = Object_GetById(gPartyState.currentOwner);
  struct TrialRoadState *state = gTrialRoadState;
  s32 found;

  Func_02005880();
  Func_020059e8(0);
  ObjectMotion_SetPositionAndReset(gPartyState.currentOwner,
                                   ((leader->x >> 20) << 4) + 8,
                                   ((leader->z >> 20) << 4) + 8);
  TrialRoad_Idle();
  found = Func_020000fc(0);
  if (found != 0)
    GameFlag_SetBit(state->field16[0] + 897);
  Func_02005a68(124);
  if (state->mirrored == 0) {
    Object_SetModeById(9, 6);
    GameFlag_SetBit(645);
    if (found != 0) {
      Object_SetModeById(10, 6);
      GameFlag_SetBit(646);
    }
  } else {
    Object_SetModeById(11, 6);
    GameFlag_SetBit(645);
    if (found != 0) {
      Object_SetModeById(12, 6);
      GameFlag_SetBit(646);
    }
  }
  GameFlag_SetBit(644);
  Func_02005888();
}
