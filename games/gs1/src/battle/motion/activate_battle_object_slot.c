#include "types.h"
#include "scene.h"
#include "abi/battle/motion/activate_battle_object_slot.h"
#include "motion_object.h"

s32 ActivateBattleObjectSlot(s32 object_id)
{
  struct BattleObjectSlot *slot;
  Battle_Check();
  slot = GetBattleObjectSlot(object_id);
  slot->active = 1;
  return 0;
}
