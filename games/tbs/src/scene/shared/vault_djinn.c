/// @file
/// Vault and its rooms: Ivan hands back Isaac's Djinni before he leaves
/// the party. "Oh, wait, I don't want to take your Djinni."

#include "djinn_ids.h"

/// Brings every Djinni back from recovery (twenty turns of it), then, if Ivan
/// holds DJINNI_FLINT, gives it back to Isaac and recalculates both.
static UNREAD_RESULT Vault_ReturnIsaacsDjinni(void) {
  DjinnRecovery_CountDownTwenty();
  if ((Unit_Get(IVAN)->djinnHeld[ELEMENT_VENUS] & 1) != 0) {
    Djinn_Transfer(IVAN, ELEMENT_VENUS, DJINNI_INDEX(DJINNI_FLINT), ISAAC);
    Sound_Play(SE_RECOVERY);
    Unit_Calculate(ISAAC);
    Unit_Calculate(IVAN);
  }
}
