/// @file
/// Vale, its home, storm and Babi scenes, Sol Sanctum's eruption and Venus
/// Lighthouse's aerie: Psynergy shown in a cutscene. One is the villager's in
/// Vale: "Behold, the power of Psynergy!"
#include "types.h"
#include "script_layer.h"
#include "ability_ids.h"

/// Prepares the effect work and spark tiles of Move (ability 140) for the
/// cutscene's Psynergy.
void Map_StartCutscenePsynergy(void) { Engine_PsynergyStart(ABILITY_MOVE, 0); }

/// Ends the cutscene's Psynergy and releases its effect work.
void Map_EndCutscenePsynergy(void) { FieldEffect_End(); }
