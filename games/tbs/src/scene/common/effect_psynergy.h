/// @file
/// Vale, its home, storm and Babi scenes, Sol Sanctum's eruption and Venus
/// Lighthouse's aerie: Psynergy shown in a cutscene. One is the villager's in
/// Vale: "Behold, the power of Psynergy!"
#include "types.h"
#include "field_event.h"
#include "ability_ids.h"

/// Prepares the effect work and spark tiles of Move (ability 140) for the
/// cutscene's Psynergy.
void Scene_BeginCutscenePsynergy(void) {
  Engine_PsynergyBegin(ABILITY_MOVE, 0);
}

/// Ends the cutscene's Psynergy and releases its effect work.
void Scene_EndCutscenePsynergy(void) { FieldFx_End(); }
