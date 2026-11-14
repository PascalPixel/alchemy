/// @file
/// Champa's Sanctum, whose healer serves the party across the counter and,
/// until Briggs sails from Alhafra, talks of the village's last hope.

#include "battle/battle.h"
#include "field/field.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "lib.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "types.h"
#include "flags.h"
#include "scene_tables.h"
#include "system/ram_buffer.h"
#include "actor.h"
#include "field/field_sequence_event.h"
#include "field/event_inline.h"

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetPlacements(void) {
  return gActorPlacements;
}

void Sanctum_Run(void);

/// Talking to the healer from across the counter, facing north.

void ChampaSanctum_ServeAcrossTheCounter(void) { Sanctum_Run(); }

const struct Trigger *Scene_GetEvents(void) { return gSceneEvents; }

s32 Scene_Initialize(void) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;

  work->transition = 513;
  Actor_SetSpritePriority(8, 2);
  Actor_GetById(8)->collisionFlags |= 4;
  Actor_GetById(8)->collisionFlags |= 16;
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

#include "text/msg_ids.h"

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(ChampaSanctum),
    SCENE_EXIT_UNLESS(1, Champa1, 7, EXIT_FLAG_NONE),
    SCENE_EXITS_END,
};

struct ActorPlacement gActorPlacements[] = {
    ACTOR_PLACEMENT(202, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 720, 0, 147,
                    FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    {SCENE_TABLE_END},
};

struct Trigger gSceneEvents[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TALK_TRIGGER_FACING(FACING_NORTH), 8, CONDITION_ALWAYS,
     EVENT(ChampaSanctum_ServeAcrossTheCounter)},
    {TRIGGER_TALK, 8, IF_FLAG_SET(FLAG_BRIGGS_SAILED_FROM_ALHAFRA),
     MSGNO(MsgChampaTreasureEncouragesLaziness)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8,
     IF_FLAG_SET(FLAG_BRIGGS_SAILED_FROM_ALHAFRA),
     MSGNO(MsgChampaVillagersStoppedWorking)},
    {TRIGGER_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgChampaCaptainLastHope)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS,
     MSGNO(MsgChampaNeedsCaptainTreasure)},
    {TRIGGER_TABLE_END},
};
