/// @file
/// Vale, its home, storm and Babi maps, Sol Sanctum's eruption and Venus
/// Lighthouse's aerie: Psynergy shown in an event. One is the villager's in
/// Vale: "Behold, the power of Psynergy!"
#include "types.h"
#include "script_layer.h"
#include "ability_ids.h"

/// Prepares the effect work and spark tiles of Move (ability 140) for the
/// event's Psynergy.
void Scene_StartEventPsynergy(void) {
  FieldEffect_LoadActionEffectResources(ABILITY_MOVE, 0);
}

/// Ends the event's Psynergy and releases its effect work.
void Scene_EndEventPsynergy(void) { FieldEffect_End(); }
