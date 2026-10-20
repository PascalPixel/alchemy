/// @file
/// Vault and its interiors: Ivan hands back Isaac's Djinni before he leaves
/// the party. "Oh, wait, I don't want to take your Djinni."
#include "types.h"
#include "field_obj.h"
#include "field_event.h"
#include "songs.h"

/// Brings every Djinni back from recovery (twenty rounds of it), then, if Ivan
/// holds the first Venus Djinni, gives it back to Isaac and recalculates both.
///
/// Called before Ivan leaves the party, at Vault's gates and in the loft.
// UB: declared s32 with no return value, as the epilogue shows; UBFIX gives
// the void form.
#ifdef UBFIX
static void Vault_ReturnIsaacsDjinni(void) {
#else
static s32 Vault_ReturnIsaacsDjinni(void) {
#endif
  DjinnRecovery_TickTwenty();
  if ((UnitGet(IVAN)->djinnAvailable[ELEMENT_VENUS] & 1) != 0) {
    Djinn_Transfer(IVAN, ELEMENT_VENUS, 0, ISAAC);
    Engine_AudioPlayCue(SE_RECOVERY);
    UnitCalc(ISAAC);
    UnitCalc(IVAN);
  }
}
