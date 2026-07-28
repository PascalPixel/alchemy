#include "types.h"
#include "motion_object.h"

s32 Func_080b6e30();
struct BattleObjectSlot *Func_080b7dd0(s32);
s32 Func_080b7e60(s32 object_id)
{
  int active;
  u8 *slot_state;
  Func_080b6e30();
  slot_state = ((u8 *) Func_080b7dd0(object_id)) + 0x28;
  active = 1;
  *((s16 *) slot_state) = active;
  return 0;
}
