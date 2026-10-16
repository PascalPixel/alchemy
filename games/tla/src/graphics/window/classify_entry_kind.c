//--------------------------------------------------------------------------
// classify_entry_kind.c
// Battle: ability target-selection categories.
//--------------------------------------------------------------------------

#include "types.h"
#include "battle_system.h"
#include "scene.h"

//--------------------------------------------------------------------------
// Classify an ability's effect for target selection.
// 0 other, 1 restore HP, 2 restore PP, 3 cure poison, 4 cure an ailment,
// 5 revive, 6 cure all ailments.
// act: ability data.
//--------------------------------------------------------------------------
s32 Battle_ClassifyEntryKind(const struct BattleAct *act) {
  s32 val;
  s32 type;
  s32 kind;
  u8 effect;

  kind = 0;
  type = act->flags & 15;
  if (type == 1) // Restore HP.
    kind = 1;
  if (type == 11) // Restore PP.
    kind = 2;
  val = (s8)act->effect;
  effect = val;
  if (effect == 3) // Cure poison.
    kind = 3;
  if (effect == 4) // Cure an ailment.
    kind = 4;
  if (effect == 64) // Cure all ailments.
    kind = 6;
  if (Battle_IsReviveEffectFar(act->effect) != 0) // Revive.
    kind = 5;
  return kind;
}
