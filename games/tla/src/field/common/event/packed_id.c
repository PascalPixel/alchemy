#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "owner_state.h"

s32 GameFlag_Test(s32 group);

void Event_PrepareObjectAndApplyValue(s32 event_id, s32 value) {
  UnitGet(event_id);
  Party_AddActiveOwnerFar(event_id);
  if (value != 0) {
    Party_ShowJoinedMessageFar(event_id, value);
  }
}

void Event_PrepareTwoObjectsAndApply(s32 first, s32 second) {
  Party_AddActiveOwnerFar(first);
  Party_AddActiveOwnerFar(second);
  Party_ShowPairJoinedMessageFar(first, second);
}

s32 Event_ValidatePackedId(u32 packed_id) {
  s32 group = (packed_id >> 10) & 0xf;
  s32 entry = packed_id & 0x3ff;

  if (group > 7)
    return -1;
  if (GameFlag_Test(group) == 0)
    return -2;
  if (Unit_KnowsActionFar(group, entry) == 0)
    return -3;
  return 0;
}
