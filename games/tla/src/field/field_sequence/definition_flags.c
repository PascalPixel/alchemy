#include "types.h"
#include "main_routine_work.h"

struct BattleFxDefinition {
  u16 id;
  u8 value;
  u8 flags;
};

struct BattleFxDefinition *BattleEffect_FindDefinition(s32 id);

s32 BattleEffect_GetAnimationValue(void);

/// Returns the voice sound of sprite id's messages, its definition's value
/// plus 0x100, or 0 for none (0xff) or with option 3
/// (gMainRoutineWork.battleFxEnabled) off. Message_OpenWindowFar takes it in
/// the high half of its last argument.
s32 Speaker_GetVoice(s32 id) {
  struct MainRoutineWork *party = &gMainRoutineWork;
  u8 value;

  
  if (party->battleFxEnabled == 0 || (value = BattleEffect_FindDefinition(id)->value) == 0xff)
    return 0;
  return value + 0x100;
}

u8 BattleEffect_GetFlags(void) {
  return BattleEffect_FindDefinition(BattleEffect_GetAnimationValue())->flags;
}
