#include "types.h"
#include "motion_object.h"

s32 Func_080b6e30();
s32 ActivateBattleObjectSlot(s32 object_id)
{
  struct BattleObjectSlot *slot;
  Func_080b6e30();
  slot = GetBattleObjectSlot(object_id);
  slot->active = 1;
  return 0;
}
