/// @file
/// The eight test scenes that share one room, whose talk events would give
/// Flint, Fizz, Forge or Gust; the room places no actors to talk to.

#include "battle/battle.h"
#include "djinn_ids.h"
#include "field/field.h"
#include "flags.h"
#include "game/game.h"
#include "system/math_table.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "types.h"
#include "field/scene_tables.h"
#include "field/event_inline.h"
#include "macros.h"

/// The actors the talk events answer: the first Djinni of each element. The
/// room's actor list is empty, so none of them is ever there.
enum TestActorId {
  ACTOR_FLINT = ACTOR_FIRST_PLACED,
  ACTOR_FIZZ,
  ACTOR_FORGE,
  ACTOR_GUST
};

void Test_ReceiveFizz(void);

void Test_ReceiveForge(void);

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetExitRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  return gActorPlacements;
}

/// Gives the first Djinni of an element, then clears its found flag again, so
/// talking to it can give it once more.
static __inline__ void Test_GiveDjinni(s32 actorId, s32 element) {
  Djinn_JoinParty(actorId, element, 0);
  GameFlag_Clear(FLAG_DJINN_FIRST + element * DJINN_PER_ELEMENT);
}

void Test_ReceiveFlint(void) { Test_GiveDjinni(ACTOR_FLINT, ELEMENT_VENUS); }

void Test_ReceiveFizz(void) { Test_GiveDjinni(ACTOR_FIZZ, ELEMENT_MERCURY); }

void Test_ReceiveForge(void) { Test_GiveDjinni(ACTOR_FORGE, ELEMENT_MARS); }

void Test_ReceiveGust(void) { Test_GiveDjinni(ACTOR_GUST, ELEMENT_JUPITER); }

const struct Trigger *Scene_GetTriggers(void) { return gSceneEvents; }

/// Nothing to set up.
s32 Scene_Initialize(void) { return 0; }

s32 Scene_PrepareMap(void) { return 0; }

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 192, 0, 192, FACING_WEST, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(Test0), SCENE_EXITS(Test1), SCENE_EXITS(Test2),
    SCENE_EXITS(Test3), SCENE_EXITS_END,
};

struct ActorPlacement gActorPlacements[] = {
    {SCENE_TABLE_END},
};

struct Trigger gSceneEvents[] = {
    {TRIGGER_TALK, ACTOR_FLINT, CONDITION_ALWAYS, EVENT(Test_ReceiveFlint)},
    {TRIGGER_TALK, ACTOR_FIZZ, CONDITION_ALWAYS, EVENT(Test_ReceiveFizz)},
    {TRIGGER_TALK, ACTOR_FORGE, CONDITION_ALWAYS, EVENT(Test_ReceiveForge)},
    {TRIGGER_TALK, ACTOR_GUST, CONDITION_ALWAYS, EVENT(Test_ReceiveGust)},
    {TRIGGER_TABLE_END},
};

/// Unused: four halfwords and an end of -1s after the room's tables; nothing
/// reads them.
s16 gTestScenes_UnusedRow[] ALIGNED(4) = {
    512, 512, 1024, 864, -1, -1, -1, -1,
};
