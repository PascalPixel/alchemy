#include "types.h"
#include "party_state.h"

struct BattleFxDefinition {
  u16 id;
  u8 value;
  u8 flags;
};

struct BattleFxDefinition *BattleFx_FindDefinition(s32 id);

s32 BattleFx_GetAnimationValue(void);

s32 Func_080d1eac(s32 id) {
  struct PartyState *party = &gPartyState;
  u8 value;

  /* FAKEMATCH: a one-pass block loads the party state before its offset is
   * built (one reordered row). */
  do {
  } while (0);
  if (party->battleFxEnabled == 0 || (value = BattleFx_FindDefinition(id)->value) == 0xff)
    return 0;
  return value + 0x100;
}

u8 BattleFx_GetFlags(void) {
  return BattleFx_FindDefinition(BattleFx_GetAnimationValue())->flags;
}
