#include "types.h"
#include "irq.h"

void BattleBackground_SetPaletteLevel(s32 unused, s32 level);

/// Puts the background's palette back undarkened, at level 0.
/// Called by Func_0811df70 and Func_0811f088.
void BattleBackground_RestorePalette(void) {
  BattleBackground_SetPaletteLevel(0, 0);
}

/// Removes the VCount interrupt's handler; nothing calls it.
void Irq_RemoveVCountHandler(void) { Irq_SetHandler(IRQ_VCOUNT, 0, 0); }
