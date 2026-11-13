/// @file
/// The Gondowan Cliffs, west of Alhafra, in two scenes that share this overlay:
/// the cliffs (SceneId_GondowanCliffs) and the pass a rock blocks until Grind
/// breaks it (SceneId_GondowanCliffsPass). Kindle, a Mars Djinni, waits at the
/// end of the cliffs (GondowanCliffs_FightKindle); Frost, Whirlwind, Move and
/// Scoop each open a way (GondowanCliffs_FreezeFirstPuddle,
/// GondowanCliffs_BlowAwayFoliage, GondowanCliffs_PushPillarIntoWater,
/// GondowanCliffs_ScoopUpGeyser); and if the party heads west before helping
/// Piers recover his Black Orb, Kraden turns them back
/// (GondowanCliffs_TurnBackToHelpPiers).
///
/// The effect spawner and the cliffs' events are in gondowan_cliffs/, which a
/// shared field effect object links between, and the tables are in
/// data/gondowan_cliffs.c, which the overlay links after the field's tiles.

#include "shared/tables.h"
#include "shared/actors.h"
#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "scene_ids.h"
#include "event_inline.h"
#include "module/scene/scene.h"

extern const u8 gGondowanCliffs_Entrances[];
extern const u8 gGondowanCliffs_PassEntrances[];

DEFINE_TILE_SPRITE_RESET(TileSprite_Reset)

void GondowanCliffs_FightKindle(void) { Field_RequestDjinnBattle(21, 49); }

const u8 *Scene_GetEntrances(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_GondowanCliffsPass)
    return gGondowanCliffs_PassEntrances;
  return gGondowanCliffs_Entrances;
}

#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "scene_tables.h"

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

extern const u8 gGondowanCliffs_ActorPlacements[];
extern const u8 gGondowanCliffs_PassActorPlacements[];

const u8 *Scene_GetPlacements(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_GondowanCliffsPass)
    return gGondowanCliffs_PassActorPlacements;
  return gGondowanCliffs_ActorPlacements;
}

#include "objcmd.h"

// clang-format off
DEFINE_FIELD_EFFECT_LIFE_SCRIPT(gFieldEffect_ShortLifeScript, 22)

DEFINE_FIELD_EFFECT_LIFE_SCRIPT(gFieldEffect_MediumLifeScript, 44)

DEFINE_FIELD_EFFECT_LIFE_SCRIPT(gFieldEffect_LongLifeScript, 126)
// clang-format on

#include "../field/object/set_sprite_priority.c"

#include "../field/object/prepare_object.c"

DEFINE_SCENE_CREATE_ACTOR(Scene_CreateActor)
