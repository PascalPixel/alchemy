/// @file
/// Vale, its home, storm and Babi scenes, Sol Sanctum's eruption and Venus
/// Lighthouse's aerie: Psynergy shown in an event. One is the villager's in
/// Vale: "Behold, the power of Psynergy!"
#include "types.h"
#include "event.h"
#include "ability_ids.h"

/// Prepares the effect work and spark tiles of Move (ABILITY_MOVE) for the
/// event's Psynergy.
void Scene_StartEventPsynergy(void) {
  FieldPsynergy_LoadResources(ABILITY_MOVE, 0);
}

/// Ends the event's Psynergy and releases its effect work.
void Scene_EndEventPsynergy(void) { FieldPsynergy_End(); }
