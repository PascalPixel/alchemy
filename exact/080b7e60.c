#include "types.h"
#include "motion_object.h"

s32 Func_080b6e30();
struct BattleObjectSlot *Func_080b7dd0(s32);
s32 Func_080b7e60(s32 object_id)
{
  struct BattleObjectSlot *slot;
  Func_080b6e30();
  slot = Func_080b7dd0(object_id);
  slot->active = 1;
  return 0;
}
