#include "types.h"
#include "main_routine_work.h"

struct BattleFxDefinition {
  u16 id;
  u8 value;
  u8 flags;
};

struct BattleFxDefinition *BattleEffect_FindDefinition(s32 id);

s32 BattleEffect_GetAnimationValue(void);

s32 Func_080d1eac(s32 id) {
  struct MainRoutineWork *party = &gMainRoutineWork;
  u8 value;

  /* FAKEMATCH: a one-pass block loads gMainRoutineWork before its offset is
   * built (one reordered row). */
  do {
  } while (0);
  if (party->battleFxEnabled == 0 || (value = BattleEffect_FindDefinition(id)->value) == 0xff)
    return 0;
  return value + 0x100;
}

u8 BattleEffect_GetFlags(void) {
  return BattleEffect_FindDefinition(BattleEffect_GetAnimationValue())->flags;
}
