#include "types.h"

struct SummonOwnerState {
  u8 unk000[0x14a];
  u16 summon;
};

struct SummonOwnerState *Unit_Get(s32 owner);
u8 BattleClass_GetEffectHeight(s32 index);
s32 BattleClass_IsSmall(s32 index);

s32 Func_0811c37c(s32 owner) {
  s32 value = BattleClass_GetEffectHeight(Unit_Get(owner)->summon) << 16;

  if (value == 0) {
    if (BattleClass_IsSmall(Unit_Get(owner)->summon) != 0)
      value = 0x180000;
    else
      value = 0x300000;
  }
  return value;
}
