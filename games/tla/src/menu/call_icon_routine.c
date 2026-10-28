/// @file
/// Menu: reload the selected character's graphics.

#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "types.h"

/// Loads the character's graphics into slot 0.
/// menu: calling menu (unused).
/// unit: character ID.
void PsynergyMenu_CallIconRoutineWithValue(s32 menu, s32 unit) {
  Portrait_ReloadFar(0, unit, 0);
}
