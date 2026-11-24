/// @file
/// Champa's Sanctum, whose healer serves the party across the counter and
/// otherwise talks of Briggs: the village's last hope until he sails from
/// Alhafra, and after it the jewels that made Champa's people lazy.

#include "battle/battle.h"
#include "field/field.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "system/math_table.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "types.h"
#include "flags.h"
#include "field/scene_tables.h"
#include "system/ram_buffer.h"
#include "field/actor.h"
#include "field/field_sequence.h"
#include "field/field_sequence_event.h"
#include "field/event_inline.h"

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetExitRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  return gActorPlacements;
}

/// The healer behind the counter, a bearded man in a long robe.
#define ACTOR_HEALER ACTOR_FIRST_PLACED

s32 Sanctum_Run(s32 keeper);

/// Talking to the healer from across the counter, facing north: the Sanctum's
/// services.
void ChampaSanctum_ServeAcrossTheCounter(s32 keeper) { Sanctum_Run(keeper); }

const struct Trigger *Scene_GetTriggers(void) { return gSceneEvents; }

s32 Scene_Initialize(void) {
  struct FieldState *state = Ram_HeapSlots->fieldState;

  state->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_SHAPE);
  Actor_SetSpritePriority(ACTOR_HEALER, 2);
  Actor_GetById(ACTOR_HEALER)->collisionFlags |= ACTOR_NEAR_TALK;
  Actor_GetById(ACTOR_HEALER)->collisionFlags |= ACTOR_NEAR_PSYNERGY;
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

#include "text/msg_ids.h"

#include "shared/scene_entrances_default.c"

u32 gSceneExits[] = {
    SCENE_EXITS(ChampaSanctum),
    SCENE_EXIT_UNLESS(CHAMPA_SANCTUM_TO_CHAMPA, Champa,
                      CHAMPA_FROM_CHAMPA_SANCTUM, EXIT_FLAG_NONE),
    SCENE_EXITS_END,
};

struct ActorPlacement gActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_BEARDED_MAN_IN_LONG_ROBE, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND, 720, 0, 147, FACING_SOUTH,
                    TALK_FACING_PARTY_AND_BACK, 0),
    {SCENE_TABLE_END},
};

struct Trigger gSceneEvents[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, CHAMPA_SANCTUM_TO_CHAMPA},
    {TALK_TRIGGER_FACING(FACING_NORTH), ACTOR_HEALER, CONDITION_ALWAYS,
     EVENT(ChampaSanctum_ServeAcrossTheCounter)},
    {TRIGGER_TALK, ACTOR_HEALER, IF_FLAG_SET(FLAG_BRIGGS_SAILED_FROM_ALHAFRA),
     MSGNO(MsgChampaTreasureEncouragesLaziness)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HEALER,
     IF_FLAG_SET(FLAG_BRIGGS_SAILED_FROM_ALHAFRA),
     MSGNO(MsgChampaVillagersStoppedWorking)},
    {TRIGGER_TALK, ACTOR_HEALER, CONDITION_ALWAYS,
     MSGNO(MsgChampaCaptainLastHope)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HEALER, CONDITION_ALWAYS,
     MSGNO(MsgChampaNeedsCaptainTreasure)},
    {TRIGGER_TABLE_END},
};
