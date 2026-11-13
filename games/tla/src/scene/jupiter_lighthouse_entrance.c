/// @file
/// The grounds before Jupiter Lighthouse's main gates, reached from the world
/// map. Cyclone blows away five patches of bushes
/// (JupiterLighthouse_CycloneFirstPatch to JupiterLighthouse_CycloneFifthPatch,
/// JupiterLighthouse_CycloneUncoversTheSpot), and at the dead end Cyclone on
/// the teleporter carries the party down to the basement
/// (JupiterLighthouse_LeaveByCyclone). The gates lead into the ground floor,
/// once they are opened from within.
///
/// The cracked tiles' code, the Teleporter routines, is in
/// jupiter_lighthouse_entrance/, in two parts the shared field effect objects
/// link between.

#include "shared/whirlwind.h"
#include "shared/tables.h"
#include "flags.h"
#include "shared/jupiter_lighthouse.h"
#include "shared/effects.h"
#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "scene_tables.h"
#include "event_work.h"
#include "module/scene/scene.h"
#include "module/tla.h"
#include "shared/actors.h"
#include "irq.h"
#include "field_sprite.h"
#include "field_effect.h"
#include "field_coordinate.h"
#include "task.h"
#include "item_icon.h"
#include "module/field/field.h"

#include "field/cell_target.h"
#include "objcmd.h"
extern u16 gLighthouseUnlitColors[];
void FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                       s32 velocityZ, u32 flags,
                       const struct EffectOptions *options);
extern s16 gLightSpriteCount;
extern struct LightOam gLightSpriteOams[];
extern s16 gLightSpritesLarge;
extern s16 gLightSpritePriority;
void Import_ActorSelectAnimation(struct FieldEffect *effect, s32 mode);
void JupiterLighthouse_CycloneSecondPatch(void);
void JupiterLighthouse_CycloneFourthPatch(void);
void JupiterLighthouse_CycloneFifthPatch(void);
void JupiterLighthouse_CycloneUncoversTheSpot(void);
void JupiterLighthouse_LeaveByCyclone(void);
extern u16 gPaletteCycleColors[];
extern u16 gPaletteCycleUnlitPalette[];

void Func_02001b34();

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetPlacements(void) {
  return gActorPlacements;
}

#include "call.h"
#include "event_inline.h"
s32 Func_02001b1c();

void JupiterLighthouse_CycloneFirstPatch(void) {
  Call7(Func_02001b1c, 0x38, 0, 4, 2, 0x46, 0x13, 1);
}

void JupiterLighthouse_CycloneSecondPatch(void) {
  Call7(Func_02001b1c, 0x38, 0, 4, 2, 0x45, 0x16, 1);
}

void JupiterLighthouse_CycloneThirdPatch(void) {
  Call7(Func_02001b1c, 0x38, 0, 3, 3, 0x4f, 0x19, 1);
}

void JupiterLighthouse_CycloneFourthPatch(void) {
  Call7(Func_02001b1c, 0x38, 0, 5, 3, 0x55, 0x1a, 1);
}

void JupiterLighthouse_CycloneFifthPatch(void) {
  Call7(Func_02001b1c, 0x38, 0, 5, 3, 0x5e, 0x1a, 1);
}

#include "main_routine_work.h"

void JupiterLighthouse_CycloneUncoversTheSpot(void) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);

  Call7(Func_02001b1c, 0x38, 0, 3, 3, 0x4f, 0x19, 1);
  if (!Event_GetFlag(0xfe2)) {
    Actor_SetPosition(64, PIXELS(280), PIXELS(440));
    if (leader->x >> 20 == 17 && leader->z >> 20 == 27) {
      Event_Start();
      FieldEvent_ResolvePendingActions(0);
      Actor_SetAttachedEffect(gMainRoutineWork.currentOwner, 0x102);
      Actor_SetDestinationOffset(gMainRoutineWork.currentOwner, -16, 0);
      leader->velocityY = 0x60000;
      FieldActor_WaitForMoveById(gMainRoutineWork.currentOwner);
      Event_End();
    }
  }
}

#include "ram_buffer.h"
#include "scene_ids.h"
#include "debug.h"

void JupiterLighthouse_LeaveByCyclone(void) { Import_EventSetExitRequest(4); }

const struct Trigger *Scene_GetEvents(void) { return gSceneEvents; }

s32 Scene_Initialize(void) {
  s32 clear;

  ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0x100;
  clear = Event_GetFlag(FLAG_KEEP_PARTY_POSITION);
  if (clear == 0) {
    if (gMainRoutineWork.entrance == 4)
      Func_02001b3c(gMainRoutineWork.currentOwner);
  }
  gMainRoutineWork.savedScene = (s32)&SceneId_Ov694Room2;
  gMainRoutineWork.savedEntrance = 6;
  return 0;
}

s32 Scene_PrepareMap(void) {
  Scene_SetArrivalFlags(0);
  return 0;
}

#include "shared/scene_set_arrival_flags.h"

extern s32 gPaletteCyclePausesInEvents;

void PaletteCycle_SetPausesInEvents(s32 a0) {
  gPaletteCyclePausesInEvents = a0;
}

void GameFlag_SetByte(s32 flag, s32 value);
void Map_CenterOnFollowedPoint(void);
#include "iwram_call.h"
#include "dma.h"
#include "io_reg.h"

extern s32 gPaletteCycleFrame;

/// Sets the counter, its value built as a word.
static inline void Counter_Set(s32 value) { gPaletteCycleFrame = value; }

DEFINE_PALETTE_CYCLE_STEP_AT(PaletteCycle_StepAt, gPaletteCycleFrame,
                             gPaletteCyclePausesInEvents, gPaletteCycleColors,
                             0x05000184)

#include "field_event.h"

void Func_02001a6c();

DEFINE_JUPITER_LIGHTHOUSE_LOAD_PALETTES(JupiterLighthouse_LoadPalettes,
                                        gPaletteCyclePausesInEvents,
                                        gPaletteCycleColors,
                                        gPaletteCycleUnlitPalette,
                                        PaletteCycle_StepAt,
                                        gLighthouseUnlitColors)

DEFINE_ACTOR_RAISE_FROM_CELL(Actor_RaiseFromCell, Func_02001a6c, Func_02001b34)

#include "actor.h"
#include "frame.h"

DEFINE_WHIRLWIND_CARRY_LEADER_LOWER(Whirlwind_CarryLeaderLower)

DEFINE_POSITION_FLAT_DISTANCE(Position_FlatDistance)

extern s32 gWhirlwindEndScript[];
extern s32 gWhirlwindRiding;

s32 Func_02001b2c(s32 x, s32 z, s32 layer);
void Actor_StartScript(struct Actor *object, const s32 *script);

DEFINE_WHIRLWIND_UPDATE_LOWER(Whirlwind_UpdateLower, Func_02001b2c,
                              Position_FlatDistance, gWhirlwindEndScript,
                              gWhirlwindRiding)

DEFINE_WHIRLWIND_SPAWN_UPPER(Whirlwind_SpawnUpper, Import_ActorSelectAnimation,
                             Whirlwind_UpdateLower, FieldEffect_Spawn)

s32 JupiterLighthouse_EmitRisingMotes(struct SceneEffectSource *object) {
  struct SceneEffectParams params;
  s32 phase, x, y, speed;

  phase = gFrameCount & 7;
  if (phase == 0) {
    params.priority = 3;
    params.startScaleX = 0xb333;
    params.startScaleY = 0xb333;
    params.palette = 14;
    x = object->x + (((s32)((u32)(Random16Far() * 9) >> 16) - 4) << 16);
    y = object->y + ((32 - (s32)((u32)(Random16Far() * 20) >> 16)) << 16);
    speed = ((((s32)((u32)(Random16Far() * 5) >> 16)) << 16) + 0x50000) / 10;
    FieldEffect_Spawn(x, y, object->z, 0, speed, phase, 0xb0000, &params);
  }
  return 0;
}

DEFINE_JUPITER_LIGHTHOUSE_PLACE_RAISED_ACTORS(
    JupiterLighthouse_PlaceRaisedActors)

// Queues the lit actors' sprites: for each listed actor with a position,
// projects it from the map's view, skips it off screen, and writes a 16 by 16
// sprite, or a 32 by 64 one when the big switch is set, at its animation
// picture and depth.
#include "map.h"
#include "vram_tab.h"

extern s16 gLightSpriteTile;
extern u32 gLightSpriteActors[];

void Import_OamAddSpriteAtDepthClamped(struct LightOam *oam, s32 depth);

DEFINE_LIGHT_SPRITES_DRAW(LightSprites_Draw, gLightSpriteTile, gLightSpriteOams,
                          gLightSpriteCount, gLightSpriteActors,
                          gLightSpritesLarge, gLightSpritePriority,
                          BattleEffect_GetObjectPriority,
                          Import_OamAddSpriteAtDepthClamped)

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static inline void *BumpAlloc(s32 size) { return Heap_Alloc(size); }

DEFINE_LIGHT_SPRITES_INIT_A(LightSprites_InitA, gLightSpriteActors,
                            gLightSpriteTile, LightSprites_Draw,
                            gLightSpriteCount, gLightSpritesLarge,
                            gLightSpritePriority)

DEFINE_LIGHT_SPRITES_INIT_B(LightSprites_InitB, gLightSpriteActors,
                            gLightSpriteTile, LightSprites_Draw,
                            gLightSpriteCount, gLightSpritesLarge,
                            gLightSpritePriority)

DEFINE_LIGHT_SPRITES_INIT_C(LightSprites_InitC, gLightSpriteActors,
                            gLightSpriteTile, LightSprites_Draw,
                            gLightSpriteCount, gLightSpritesLarge,
                            gLightSpritePriority)

DEFINE_LIGHT_SPRITES_ADD(LightSprites_Add, gLightSpriteCount,
                         gLightSpriteActors)

void LightSprites_SetPriority(s32 value) { gLightSpritePriority = value; }

#include "../field/object/set_sprite_priority.c"

// clang-format off
DEFINE_FIELD_EFFECT_LIFE_SCRIPT(gFieldEffect_ShortLifeScript, 22)

DEFINE_FIELD_EFFECT_LIFE_SCRIPT(gFieldEffect_MediumLifeScript, 44)

DEFINE_FIELD_EFFECT_LIFE_SCRIPT(gFieldEffect_LongLifeScript, 126)

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(251),
    SCENE_EXIT(1, 2, 55),
    SCENE_EXIT(2, 254, 2),
    SCENE_EXIT(3, 254, 3),
    SCENE_EXIT(4, 252, 4),
    SCENE_EXITS_END,
};

struct ActorPlacement gActorPlacements[] = {
    {341, CONDITION_ALWAYS, 1, PIXELS(408), 0, PIXELS(360), FACING_SOUTHWEST, 2, 0},
    {341, CONDITION_ALWAYS, 1, PIXELS(472), 0, PIXELS(360), FACING_SOUTHEAST, 2, 1},
    {SCENE_TABLE_END},
};

struct Trigger gSceneEvents[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 30, CONDITION_ALWAYS, EVENT(JupiterLighthouse_CycloneFirstPatch)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 31, CONDITION_ALWAYS, EVENT(JupiterLighthouse_CycloneSecondPatch)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 32, CONDITION_ALWAYS, EVENT(JupiterLighthouse_CycloneThirdPatch)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 33, CONDITION_ALWAYS, EVENT(JupiterLighthouse_CycloneFourthPatch)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 34, CONDITION_ALWAYS, EVENT(JupiterLighthouse_CycloneFifthPatch)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 35, CONDITION_ALWAYS, EVENT(JupiterLighthouse_LeaveByCyclone)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 36, CONDITION_ALWAYS, EVENT(JupiterLighthouse_CycloneUncoversTheSpot)},
    {TRIGGER_TABLE_END},
};

s32 gPaletteCycleFrame = -1;

s32 gPaletteCyclePausesInEvents = 1;

s32 gWhirlwindEndScript[] = {
    39, 5, 0, 12, 38,
};
// clang-format on

s32 gFieldEffect_LifeScripts[] = {
    (s32)gFieldEffect_ShortLifeScript,
    (s32)gFieldEffect_MediumLifeScript,
    (s32)gFieldEffect_LongLifeScript,
};
