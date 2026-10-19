// Vault and its interiors: Ivan hands back Isaac's Djinni before he leaves
// the party ("Oh, wait, I don't want to take your Djinni.").
#include "types.h"
#include "field_obj.h"
#include "field_event.h"
#include "songs.h"

// Bring every Djinni back from recovery (twenty rounds of it), then, if Ivan
// holds the first Venus Djinni, give it back to Isaac and recalculate both.
static s32 OverlayObject_RunObject2WhenFlagged(void) {
  BattlePlacement_UpdateTimedEntriesTwentyTimes();
  if ((UnitGet(IVAN)->djinnAvailable[ELEMENT_VENUS] & 1) != 0) {
    Djinn_Transfer(IVAN, ELEMENT_VENUS, 0, ISAAC);
    Engine_AudioPlayCue(SE_RECOVERY);
    UnitCalc(ISAAC);
    UnitCalc(IVAN);
  }
}
