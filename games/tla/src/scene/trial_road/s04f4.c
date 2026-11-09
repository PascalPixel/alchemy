#include "trial_road.h"
#include "event_inline.h"

extern char MsgTrialRoadStoredItem;

void Func_02005880(void);
void Func_02005888(void);
void Func_02005858(s32 value, s32 slot);
void Func_02000218(s32 a, s32 b);
/// Func_080af0e4 through its far call: empties an inventory slot and returns 2.
s32 Func_02005870(s32 owner, s32 slot);

/// Puts an item in the chest: "{protagonist} put {actor}'s {item} in the
/// chest." (MsgTrialRoadStoredItem) with sound 125 while the chest object
/// Func_020059b8 picks plays motion 5; chests 9 and 11 clear flag 645, the
/// others 646, and once both are clear it calls Func_02000218 with the stage
/// state's mirrored and field16[0], plays sound 138 and sets flag 642. Then the
/// item leaves the owner's slot.
void Func_020004f4(s32 item, s32 owner, s32 slot) {
  s32 kind = Func_020059b8(Party_GetLeaderActor());
  struct TrialRoadState *state = gTrialRoadState;

  Func_02005880();
  Event_ResolvePendingActions(0);
  Func_02005858(owner, 1);
  Func_02005858(item, 2);
  FieldMessage_Show((s32)&MsgTrialRoadStoredItem, 1);
  Sound_Play(125);
  Motion_SetModeAndWaitAnimation(kind, 5);
  if (kind == 9 || kind == 11)
    GameFlag_ClearBit(645);
  else
    GameFlag_ClearBit(646);
  if (!GameFlag_Test(645) && !GameFlag_Test(646)) {
    Func_02000218(state->mirrored, state->field16[0]);
    Sound_Play(138);
    GameFlag_SetBit(642);
  }
  Func_02005870(owner, slot);
  Func_02005888();
}
