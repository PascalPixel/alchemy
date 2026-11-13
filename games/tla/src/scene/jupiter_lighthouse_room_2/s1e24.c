/// @file
/// Scene code of overlay 0x69b, in ROM order.

#include "../shared/tables.h"
#include "../shared/cracked_tile.h"
#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "field_effect.h"
#include "event_inline.h"
#include "../shared/jupiter_lighthouse_aerie.h"
#include "event_work.h"
#include "module/field/field_effect.h"
#include "module/scene/jupiter_lighthouse_room_2.h"
#include "module/scene/scene.h"

void FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                       s32 velocityZ, u32 flags, struct EffectOptions *options);

static __inline__ void CamBounds(s32 x, s32 y, s32 z, s32 mode) {
  FieldCamera_MoveWithinMapBounds(x, y, z, mode);
}

static __inline__ void SetWorkTriple(s32 a, s32 b, s32 c) {
  MapGrid_SetScreenShake(a, b, c);
}

DEFINE_CRACKED_TILE_DROP_IN(CrackedTile_DropIn, FieldEffect_Spawn)

void CrackedTile_Crack();
void CrackedTile_Fall();

DEFINE_CRACKED_TILES_STEP(CrackedTiles_Step, gCrackedTiles, CrackedTile_Crack,
                          CrackedTile_Fall)

#include "cell_target.h"

DEFINE_CRACKED_TILE_PLACE_LANDING(CrackedTile_PlaceLanding, gCrackedTiles,
                                  CrackedTile_DropIn)

#include "scene_tables.h"
#include "objcmd.h"

void Func_020000d0(void);
void Func_02000230(void);
void Func_0200042c(void);
void Func_020004d0(void);
void Func_02000500(void);
void Func_02000674(void);
void Func_02000680(void);

// clang-format off
DEFINE_FIELD_EFFECT_DURATION_SCRIPT(gFieldEffect_ShortScript, 22)

DEFINE_FIELD_EFFECT_DURATION_SCRIPT(gFieldEffect_MediumScript, 44)

DEFINE_FIELD_EFFECT_DURATION_SCRIPT(gFieldEffect_LongScript, 126)

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

DEFINE_G_SCENE_EXITS_3(255, 254, 256, 6, 7, 256, 9, 10, 11, 257)

struct ActorPlacement gActorPlacements[] = {
    {316, CONDITION_ALWAYS, 1, PIXELS(168), 0, PIXELS(104), FACING_EAST, 2, 0},
    {316, CONDITION_ALWAYS, 1, PIXELS(200), 0, PIXELS(104), FACING_EAST, 2, 1},
    {316, CONDITION_ALWAYS, 1, PIXELS(232), 0, PIXELS(104), FACING_EAST, 2, 1},
    {316, CONDITION_ALWAYS, 1, PIXELS(264), 0, PIXELS(104), FACING_EAST, 2, 1},
    {316, CONDITION_ALWAYS, 1, PIXELS(312), 0, PIXELS(104), FACING_EAST, 2, 1},
    {341, CONDITION_ALWAYS, 1, PIXELS(184), 0, PIXELS(600), FACING_SOUTHWEST, 2, 0},
    {341, CONDITION_ALWAYS, 1, PIXELS(248), 0, PIXELS(600), FACING_SOUTHEAST, 2, 1},
    {341, CONDITION_ALWAYS, 1, PIXELS(184), 0, PIXELS(728), FACING_SOUTHWEST, 2, 1},
    {341, CONDITION_ALWAYS, 1, PIXELS(248), 0, PIXELS(728), FACING_SOUTHEAST, 2, 1},
    {428, CONDITION_ALWAYS, 1, PIXELS(376), 0, PIXELS(648), FACING_SOUTH, 2, 0},
    {SCENE_TABLE_END},
};

struct Trigger gSceneEvents[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {33, 6, CONDITION_ALWAYS, 6},
    {33, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_EXIT, 12, CONDITION_ALWAYS, 12},
    {TRIGGER_STEP, 20, CONDITION_ALWAYS, EVENT(Func_02000454)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 30, CONDITION_ALWAYS, EVENT(Func_020003f4)},
    {TRIGGER_RAISED, 30, CONDITION_ALWAYS, EVENT(Func_02000230)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 31, CONDITION_ALWAYS, EVENT(Func_02000410)},
    {TRIGGER_RAISED, 31, CONDITION_ALWAYS, EVENT(Func_02000230)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 32, CONDITION_ALWAYS, EVENT(Func_0200042c)},
    {TRIGGER_RAISED, 32, CONDITION_ALWAYS, EVENT(Func_020000d0)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 33, CONDITION_ALWAYS, EVENT(Func_02000440)},
    {TRIGGER_RAISED, 33, CONDITION_ALWAYS, EVENT(Func_020000d0)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 17, CONDITION_ALWAYS, EVENT(Func_020004d0)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 17, CONDITION_ALWAYS, EVENT(Func_02000500)},
    {8, 35, CONDITION_ALWAYS, EVENT(Func_020004d0)},
    {9, 35, CONDITION_ALWAYS, EVENT(Func_02000500)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 12, CONDITION_ALWAYS, EVENT(Func_02000674)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 12, CONDITION_ALWAYS, EVENT(Func_02000680)},
    {PSYNERGY_ANYWHERE(ABILITY_HOVER, PSYNERGY_PHASE_AFTER_CAST), 0, CONDITION_ALWAYS, EVENT(Func_020006dc)},
    {PSYNERGY_ANYWHERE(ABILITY_HOVER, PSYNERGY_PHASE_AFTER_END), 0, CONDITION_ALWAYS, EVENT(Func_02000704)},
    {TRIGGER_TABLE_END},
};

s32 gPaletteCycleFrame[] = {
    -1,
};

u32 gPaletteCyclePausesInEvents[] = {
    1,
};

u32 gWhirlwindEndScript[] = {
    39, 5, 0, 12, 38,
};
// clang-format on

const s32 *gFieldEffect_DurationScripts[] = {
    gFieldEffect_ShortScript,
    gFieldEffect_MediumScript,
    gFieldEffect_LongScript,
};
