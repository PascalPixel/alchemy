/// @file
/// The hall of hover tiles at the top of Jupiter Lighthouse's right tower,
/// below the aerie. Hover here lasts as long as the party stays on the hall's
/// floor (JupiterLighthouse_StartHovering), and stone faces blow whirlwinds
/// across the floor and the upper ledges as the party passes their cells
/// (JupiterLighthouse_RaiseWhirlwind, JupiterLighthouse_RaiseUpperWhirlwind).
/// Cracked tiles give way under a second step
/// (JupiterLighthouse_StepOnLowerCrackedTile,
/// JupiterLighthouse_StepOnUpperCrackedTile); the way on leads out to the
/// aerie.
///
/// The cracked tiles' code is in
/// jupiter_lighthouse_room_9/, in two parts the shared field effect objects
/// link between.

#include "shared/whirlwind.h"
#include "shared/tables.h"
#include "flags.h"
#include "shared/jupiter_lighthouse.h"
#include "shared/actors.h"
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
#include "main_routine_work.h"
#include "event_inline.h"
#include "ram_buffer.h"
#include "actor.h"
#include "event_work.h"
#include "module/field/field.h"

/// Where a stone face's whirlwind starts: an unseen actor moved to the face
/// that blows, which the whirlwind takes its place and heading from.
enum { ACTOR_WHIRLWIND_SOURCE = ACTOR_FIRST_PLACED };
#include "module/scene/scene.h"
#include "module/tla.h"
#include "call.h"
#include "field_effect.h"
#include "field_coordinate.h"
#include "map.h"
#include "vram_tab.h"
#include "task.h"
#include "item_icon.h"

#include "field/cell_target.h"
#include "objcmd.h"
struct Actor;
struct LightOam;
void CrackedTile_PlaceLanding(s32 x, s32 z, s32 effect);
void JupiterLighthouse_LoadPalettes(s32 mode);
extern s16 gLightSpriteTile;
extern struct Actor *gLightSpriteActors[];
s32 BattleEffect_GetObjectPriority(struct Actor *actor);
/* VIEW(first): routine Oam_AddSpriteAtDepthClamped as this part declared it;
 * the joined file has another. */
void Oam_AddSpriteAtDepthClamped__first(
    struct LightOam *oam, s32 depth) __asm__("Oam_AddSpriteAtDepthClamped");
#define Oam_AddSpriteAtDepthClamped Oam_AddSpriteAtDepthClamped__first
void JupiterLighthouse_StopHovering(void);
void Whirlwind_CarryLeader(void);
extern s32 gWhirlwindRiding;
extern u16 gPaletteCycleColors[];
extern u16 gPaletteCycleUnlitPalette[];
extern u16 gPaletteCycleUnlitColors[];
void MapGrid_SetCellHeight();
void MapGrid_SetAttributeBAt();

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetPlacements(void) {
  return gActorPlacements;
}

s32 CrackedTiles_Step();
struct GustObject *Whirlwind_SpawnUpper();

void JupiterLighthouse_StartHovering(void) {
  if (gMainRoutineWork.renderMode == 10)
    FieldMapGrid_SetCellBehavior(0x2200000, 0x1100000, 0, 255);
}

void JupiterLighthouse_StopHovering(void) {
  MapGrid_SetCellBehavior(0x2200000, 0x1100000, 0, 6);
}

void JupiterLighthouse_RaiseWhirlwind(s32 id) {
  struct Actor *object;

  if (gWhirlwindRiding == 0) {
    object = Actor_Get(ACTOR_WHIRLWIND_SOURCE);
    if (id == 30) {
      Actor_SetPosition(ACTOR_WHIRLWIND_SOURCE, PIXELS(296), PIXELS(600));
      object->y = 0;
      object->facing = FACING_WEST;
    } else if (id == 31) {
      Actor_SetPosition(ACTOR_WHIRLWIND_SOURCE, PIXELS(136), PIXELS(680));
      object->y = 0;
      object->facing = 0;
    }
    Whirlwind_SpawnUpper(object);
    gWhirlwindRiding = 1;
  }
}

void JupiterLighthouse_RaiseUpperWhirlwind(s32 id) {
  struct Actor *object;
  struct GustObject *motion;

  if (gWhirlwindRiding == 0 &&
      Actor_Get(gMainRoutineWork.currentOwner)->y > 0) {
    object = Actor_Get(ACTOR_WHIRLWIND_SOURCE);
    if (id == 50) {
      Actor_SetPosition(ACTOR_WHIRLWIND_SOURCE, PIXELS(488), PIXELS(280));
      object->y = PIXELS(32);
      object->facing = FACING_WEST;
    } else if (id == 51) {
      Actor_SetPosition(ACTOR_WHIRLWIND_SOURCE, PIXELS(472), PIXELS(184));
      object->y = PIXELS(32);
      object->facing = FACING_WEST;
    } else if (id == 52) {
      Actor_SetPosition(ACTOR_WHIRLWIND_SOURCE, PIXELS(616), PIXELS(280));
      object->y = PIXELS(32);
      object->facing = 0;
    } else if (id == 53) {
      Actor_SetPosition(ACTOR_WHIRLWIND_SOURCE, PIXELS(632), PIXELS(184));
      object->y = PIXELS(32);
      object->facing = 0;
    }
    motion = Whirlwind_SpawnUpper(object);
    if (motion != NULL)
      motion->speedLimit = 0x10000;
    gWhirlwindRiding = 1;
  }
}

void JupiterLighthouse_StepOnLowerCrackedTile(s32 arg) {
  if (Actor_Get(gMainRoutineWork.currentOwner)->y == 0 &&
      CrackedTiles_Step(arg) != 0)
    FieldEvent_SetExitRequest(13);
}

void JupiterLighthouse_StepOnUpperCrackedTile(s32 arg) {
  struct Actor *leader;
  s32 x;
  s32 z;

  if (Actor_Get(gMainRoutineWork.currentOwner)->y == 0) {
    leader = Actor_Get(gMainRoutineWork.currentOwner);
    x = leader->x >> 20;
    z = leader->z >> 20;
    if (x == 18 && z == 34)
      FieldActor_SetSpritePriority(gMainRoutineWork.currentOwner, 3);
    if (x == 8 && z == 39)
      FieldActor_SetSpritePriority(gMainRoutineWork.currentOwner, 3);
    if (CrackedTiles_Step(arg) != 0)
      FieldEvent_SetExitRequest(14);
  }
}

void JupiterLighthouse_StepOnCrackedTileOrWhirlwind(s32 arg) {
  struct Actor *leader;
  s32 x;
  s32 z;

  leader = Actor_Get(gMainRoutineWork.currentOwner);
  if (leader->y <= 0) {
    JupiterLighthouse_StepOnLowerCrackedTile(arg);
  } else {
    x = leader->x >> 20;
    z = leader->z >> 20;
    if (x == 25) {
      if (z == 17)
        JupiterLighthouse_RaiseUpperWhirlwind(50);
      if (z == 11)
        JupiterLighthouse_RaiseUpperWhirlwind(51);
    }
    if (x == 43) {
      if (z == 17)
        JupiterLighthouse_RaiseUpperWhirlwind(52);
      if (z == 11)
        JupiterLighthouse_RaiseUpperWhirlwind(53);
    }
  }
}

const struct Trigger *Scene_GetEvents(void) { return gSceneEvents; }

/* VIEW(first): routine CrackedTiles_Find as this part declared it; the joined
 * file has another. */
void CrackedTiles_Find__first(s32 kind, s32 x, s32 z, s32 a,
                              s32 b) __asm__("CrackedTiles_Find");
#define CrackedTiles_Find CrackedTiles_Find__first

/// The scene's set-up: screen effect 0x204, actor 64's action variant 3 and
/// actor 8 held still, then CrackedTiles_Find and CrackedTile_PlaceLanding by
/// the entrance.
s32 Scene_Initialize(void) {
  struct EventWork *work = Ram_HeapSlots->eventWork;

  work->screenEffect = 0x204;
  FieldActor_SetSpritePriority(64, 3);
  gWhirlwindRiding = 0;
  Actor_Get(ACTOR_WHIRLWIND_SOURCE)->motionFlags = 0;
  if ((u16)(gMainRoutineWork.entrance - 6) <= 1) {
    CrackedTiles_Find(33, 0x280, 0x380, 48, 1);
    CrackedTile_PlaceLanding(0, 0, 0x204);
  } else if (gMainRoutineWork.entrance == 3 || gMainRoutineWork.entrance == 5 ||
             gMainRoutineWork.entrance == 13) {
    CrackedTiles_Find(43, 0x280, 0x380, 49, 1);
    CrackedTile_PlaceLanding(-21, 32, 0x204);
  } else {
    CrackedTiles_Find(43, 0x280, 0x380, 0, 0);
    CrackedTile_PlaceLanding(-2, -27, 0x204);
  }
  return 0;
}

s32 Scene_PrepareMap(void) {
  Scene_SetArrivalFlags(0);
  JupiterLighthouse_LoadPalettes(0);
  return 0;
}

#include "shared/scene_set_arrival_flags.h"

void GameFlag_SetByte(s32 flag, s32 value);
void Map_CenterOnFollowedPoint(void);
#include "iwram_call.h"
#include "dma.h"
#include "io_reg.h"
#include "field_event.h"

extern s32 gPaletteCyclePausesInEvents;

void PaletteCycle_SetPausesInEvents(s32 value) {
  gPaletteCyclePausesInEvents = value;
}

extern s32 gPaletteCycleFrame;

/// Sets the counter, its value built as a word.
static inline void Counter_Set(s32 value) { gPaletteCycleFrame = value; }

DEFINE_PALETTE_CYCLE_STEP(PaletteCycle_Step, gPaletteCycleFrame,
                          gPaletteCyclePausesInEvents, gPaletteCycleColors)

DEFINE_JUPITER_LIGHTHOUSE_LOAD_PALETTES(JupiterLighthouse_LoadPalettes,
                                        gPaletteCyclePausesInEvents,
                                        gPaletteCycleColors,
                                        gPaletteCycleUnlitPalette,
                                        PaletteCycle_Step,
                                        gPaletteCycleUnlitColors)

DEFINE_ACTOR_RAISE_FROM_CELL(Actor_RaiseFromCell, MapGrid_SetCellHeight,
                             MapGrid_SetAttributeBAt)

#include "frame.h"
#include "field_sprite.h"

DEFINE_WHIRLWIND_CARRY_LEADER(Whirlwind_CarryLeader)

DEFINE_POSITION_FLAT_DISTANCE(Position_FlatDistance)

extern s32 gWhirlwindEndScript[];

s32 MapGrid_GetCellBehavior(s32 x, s32 z, s32 layer);
void Actor_StartScript(struct Actor *object, const s32 *script);

DEFINE_WHIRLWIND_UPDATE(Whirlwind_Update, MapGrid_GetCellBehavior,
                        Position_FlatDistance, gWhirlwindEndScript,
                        gWhirlwindRiding)

extern u8 gLightSpriteCount[];

/* VIEW(first): routine FieldEffect_Spawn as this part declared it; the joined
 * file has another. */
void FieldEffect_Spawn__first(
    s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY, s32 velocityZ, u32 flags,
    struct EffectOptions *options) __asm__("FieldEffect_Spawn");
#define FieldEffect_Spawn FieldEffect_Spawn__first

DEFINE_WHIRLWIND_SPAWN_UPPER(Whirlwind_SpawnUpper, Actor_SelectAnimation,
                             Whirlwind_Update, FieldEffect_Spawn)

DEFINE_ACTOR_EMIT_RISING_SPARKS(Actor_EmitRisingSparks, FieldEffect_Spawn)

DEFINE_JUPITER_LIGHTHOUSE_PLACE_RAISED_ACTORS(
    JupiterLighthouse_PlaceRaisedActors)

/* VIEW(s0910): variable gLightSpriteCount as s16 gLightSpriteCount;
 * module/scene/scene.h has u8 gLightSpriteCount[]; tried as the rest of the
 * object declares it, it does not compile in tla-ja:
 * games/tla/src/scene/jupiter_lighthouse_room_9.c:302: `gLightSpriteCount' has
 * an incomplete type. */
extern s16 gLightSpriteCount__s0910 __asm__("gLightSpriteCount");
extern struct LightOam gLightSpriteOams[];
extern s16 gLightSpritesLarge;
extern s16 gLightSpritePriority;

/// Queues the lit actors' sprites: for each listed actor with a position,
/// projects it from the map's view, skips it off screen, and writes a 16 by 16
/// sprite, or a 32 by 64 one when the big switch is set, at its animation
/// picture and depth.
DEFINE_LIGHT_SPRITES_DRAW(LightSprites_Draw, gLightSpriteTile, gLightSpriteOams,
                          gLightSpriteCount__s0910, gLightSpriteActors,
                          gLightSpritesLarge, gLightSpritePriority,
                          BattleEffect_GetObjectPriority,
                          Oam_AddSpriteAtDepthClamped)

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static inline void *BumpAlloc(s32 size) { return Heap_Alloc(size); }

DEFINE_LIGHT_SPRITES_INIT_A(LightSprites_InitA, gLightSpriteActors,
                            gLightSpriteTile, LightSprites_Draw,
                            gLightSpriteCount__s0910, gLightSpritesLarge,
                            gLightSpritePriority)

DEFINE_LIGHT_SPRITES_INIT_B(LightSprites_InitB, gLightSpriteActors,
                            gLightSpriteTile, LightSprites_Draw,
                            gLightSpriteCount__s0910, gLightSpritesLarge,
                            gLightSpritePriority)

DEFINE_LIGHT_SPRITES_INIT_C(LightSprites_InitC, gLightSpriteActors,
                            gLightSpriteTile, LightSprites_Draw,
                            gLightSpriteCount__s0910, gLightSpritesLarge,
                            gLightSpritePriority)

DEFINE_LIGHT_SPRITES_ADD(LightSprites_Add, gLightSpriteCount__s0910,
                         gLightSpriteActors)

void LightSprites_SetPriority(s32 value) { gLightSpritePriority = value; }

#include "../field/object/set_sprite_priority.c"

extern u32 gFieldEffect_LongScript[];

// clang-format off
DEFINE_FIELD_EFFECT_DURATION_SCRIPT(gFieldEffect_ShortScript, 22)

DEFINE_FIELD_EFFECT_DURATION_SCRIPT(gFieldEffect_MediumScript, 44)

DEFINE_FIELD_EFFECT_DURATION_SCRIPT(gFieldEffect_LongScript, 126)

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(262),
    SCENE_EXIT(1, 260, 1),
    SCENE_EXIT(2, 262, 3),
    SCENE_EXIT(3, 262, 2),
    SCENE_EXIT(4, 264, 4),
    SCENE_EXIT(5, 262, 6),
    SCENE_EXIT(6, 262, 5),
    SCENE_EXIT(7, 262, 8),
    SCENE_EXIT(8, 262, 7),
    SCENE_EXIT(13, 262, 13),
    SCENE_EXIT(14, 262, 14),
    SCENE_EXITS_END,
};

struct ActorPlacement gActorPlacements[] = {
    {489, CONDITION_ALWAYS, 11, -0x80000, 0, -0x80000, FACING_WEST, 2, 0},
    {317, CONDITION_ALWAYS, 11, PIXELS(424), 0, PIXELS(168), FACING_SOUTH, 2, 0},
    {317, CONDITION_ALWAYS, 11, PIXELS(440), 0, PIXELS(168), FACING_SOUTH, 2, 1},
    {317, CONDITION_ALWAYS, 11, PIXELS(664), 0, PIXELS(200), FACING_SOUTH, 2, 1},
    {317, CONDITION_ALWAYS, 11, PIXELS(680), 0, PIXELS(200), FACING_SOUTH, 2, 1},
    {317, CONDITION_ALWAYS, 11, PIXELS(440), 0, PIXELS(296), FACING_SOUTH, 2, 1},
    {317, CONDITION_ALWAYS, 11, PIXELS(408), 0, PIXELS(344), FACING_SOUTH, 2, 1},
    {317, CONDITION_ALWAYS, 11, PIXELS(424), 0, PIXELS(360), FACING_SOUTH, 2, 1},
    {317, CONDITION_ALWAYS, 11, PIXELS(440), 0, PIXELS(360), FACING_SOUTH, 2, 1},
    {317, CONDITION_ALWAYS, 11, PIXELS(456), 0, PIXELS(360), FACING_SOUTH, 2, 1},
    {317, CONDITION_ALWAYS, 11, PIXELS(472), 0, PIXELS(360), FACING_SOUTH, 2, 1},
    {317, CONDITION_ALWAYS, 11, PIXELS(488), 0, PIXELS(360), FACING_SOUTH, 2, 1},
    {317, CONDITION_ALWAYS, 11, PIXELS(504), 0, PIXELS(360), FACING_SOUTH, 2, 1},
    {317, CONDITION_ALWAYS, 11, PIXELS(520), 0, PIXELS(360), FACING_SOUTH, 2, 1},
    {317, CONDITION_ALWAYS, 11, PIXELS(536), 0, PIXELS(360), FACING_SOUTH, 2, 1},
    {317, CONDITION_ALWAYS, 11, PIXELS(552), 0, PIXELS(360), FACING_SOUTH, 2, 1},
    {317, CONDITION_ALWAYS, 11, PIXELS(568), 0, PIXELS(360), FACING_SOUTH, 2, 1},
    {317, CONDITION_ALWAYS, 11, PIXELS(584), 0, PIXELS(360), FACING_SOUTH, 2, 1},
    {317, CONDITION_ALWAYS, 11, PIXELS(600), 0, PIXELS(360), FACING_SOUTH, 2, 1},
    {317, CONDITION_ALWAYS, 11, PIXELS(616), 0, PIXELS(360), FACING_SOUTH, 2, 1},
    {317, CONDITION_ALWAYS, 11, PIXELS(632), 0, PIXELS(360), FACING_SOUTH, 2, 1},
    {317, CONDITION_ALWAYS, 11, PIXELS(648), 0, PIXELS(360), FACING_SOUTH, 2, 1},
    {317, CONDITION_ALWAYS, 11, PIXELS(664), 0, PIXELS(360), FACING_SOUTH, 2, 1},
    {317, CONDITION_ALWAYS, 11, PIXELS(680), 0, PIXELS(360), FACING_SOUTH, 2, 1},
    {317, CONDITION_ALWAYS, 11, PIXELS(696), 0, PIXELS(344), FACING_SOUTH, 2, 1},
    {SCENE_TABLE_END},
};

struct Trigger gSceneEvents[] = {
    {49, 1, CONDITION_ALWAYS, 1},
    {33, 2, CONDITION_ALWAYS, 2},
    {49, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {33, 5, CONDITION_ALWAYS, 5},
    {49, 6, CONDITION_ALWAYS, 6},
    {33, 7, CONDITION_ALWAYS, 7},
    {49, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_EXIT, 12, CONDITION_ALWAYS, 12},
    {TRIGGER_STEP, 30, CONDITION_ALWAYS, EVENT(JupiterLighthouse_RaiseWhirlwind)},
    {TRIGGER_STEP, 31, CONDITION_ALWAYS, EVENT(JupiterLighthouse_RaiseWhirlwind)},
    {TRIGGER_STEP, 50, CONDITION_ALWAYS, EVENT(JupiterLighthouse_RaiseUpperWhirlwind)},
    {TRIGGER_STEP, 51, CONDITION_ALWAYS, EVENT(JupiterLighthouse_RaiseUpperWhirlwind)},
    {TRIGGER_STEP, 52, CONDITION_ALWAYS, EVENT(JupiterLighthouse_RaiseUpperWhirlwind)},
    {TRIGGER_STEP, 53, CONDITION_ALWAYS, EVENT(JupiterLighthouse_RaiseUpperWhirlwind)},
    {TRIGGER_RAISED, 200, CONDITION_ALWAYS, EVENT(Whirlwind_CarryLeader)},
    {TRIGGER_STEP, 33, CONDITION_ALWAYS, EVENT(JupiterLighthouse_StepOnLowerCrackedTile)},
    {TRIGGER_STEP, 34, CONDITION_ALWAYS, EVENT(JupiterLighthouse_StepOnCrackedTileOrWhirlwind)},
    {TRIGGER_STEP, 35, CONDITION_ALWAYS, EVENT(JupiterLighthouse_StepOnLowerCrackedTile)},
    {TRIGGER_STEP, 43, CONDITION_ALWAYS, EVENT(JupiterLighthouse_StepOnUpperCrackedTile)},
    {TRIGGER_STEP, 44, CONDITION_ALWAYS, EVENT(JupiterLighthouse_StepOnUpperCrackedTile)},
    {TRIGGER_STEP, 45, CONDITION_ALWAYS, EVENT(JupiterLighthouse_StepOnUpperCrackedTile)},
    {PSYNERGY_ANYWHERE(ABILITY_HOVER, PSYNERGY_PHASE_AFTER_CAST), 0, CONDITION_ALWAYS, EVENT(JupiterLighthouse_StartHovering)},
    {PSYNERGY_ANYWHERE(ABILITY_HOVER, PSYNERGY_PHASE_AFTER_END), 0, CONDITION_ALWAYS, EVENT(JupiterLighthouse_StopHovering)},
    {TRIGGER_TABLE_END},
};

s32 gPaletteCycleFrame = -1;

s32 gPaletteCyclePausesInEvents = 1;

s32 gWhirlwindEndScript[] = {
    39, 5, 0, 12, 38,
};
// clang-format on

const s32 *gFieldEffect_DurationScripts[] = {
    gFieldEffect_ShortScript,
    gFieldEffect_MediumScript,
    gFieldEffect_LongScript,
};

#include "../field/field_effect/move.c"

#undef Oam_AddSpriteAtDepthClamped
#undef CrackedTiles_Find
#undef FieldEffect_Spawn
// ---- from scene/jupiter_lighthouse_room_9/cracked_tiles.c ----
// @file
// The hover-tile hall of Jupiter Lighthouse's right tower: its copy of the
// effect spawner, and the cracked tiles, found, cracked and fallen through.

#include "shared/cracked_tile.h"
#include "types.h"
#include "field_effect.h"
#include "main_routine_work.h"
#include "event_inline.h"
#include "heap_release.h"
#include "item_icon.h"
#include "module/scene/scene.h"
#include "io_reg.h"
#include "field.h"
#include "event_work.h"

extern const s32 *gFieldEffect_DurationScripts[];

/* VIEW(cracked_tiles): routine Actor_SelectAnimation as this part declared it;
 * the joined file has another. */
void Actor_SelectAnimation__cracked_tiles(
    struct FieldEffect *effect, s32 mode) __asm__("Actor_SelectAnimation");
#define Actor_SelectAnimation Actor_SelectAnimation__cracked_tiles
/* VIEW(cracked_tiles): routine Actor_StartScript as this part declared it; the
 * joined file has another. */
void Actor_StartScript__cracked_tiles(
    struct FieldEffect *effect, const s32 *script) __asm__("Actor_StartScript");
#define Actor_StartScript Actor_StartScript__cracked_tiles

void FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                       s32 velocityZ, u32 flags,
                       const struct EffectOptions *options) {
  struct Actor *party = Actor_GetById(gMainRoutineWork.currentOwner);
  struct FieldEffect *effect;
  struct FieldSprite *sprite;
  const s32 *script;

  if ((flags & EFFECT_USE_TYPE) != 0 && options != 0)
    effect = ((struct FieldEffect *)Import_ActorCreate(options->type, x, y, z));
  else
    effect = ((struct FieldEffect *)Import_ActorCreate(285, x, y, z));
  if (effect == 0)
    return;

  sprite = effect->sprite;
  Actor_SelectAnimation(effect, (flags + 1) & EFFECT_SCRIPT_MASK);
  Actor_StartScript(effect,
                    gFieldEffect_DurationScripts[flags & EFFECT_SCRIPT_MASK]);
  effect->motionFlags = 0;
  Actor_SetSpriteFlags(effect, 0);
  effect->update = FieldEffect_UpdateMotionScaleAndSpin;
  effect->velocity.x = velocityX;
  effect->velocity.y = velocityY;
  effect->velocity.z = velocityZ;
  Scene_SetActorSpritePriority(effect,
                               ((struct FieldSprite *)party->sprite)->priority);
  effect->scaleRateX = 0;
  effect->scaleRateY = 0;
  effect->spin = 0;

  if ((flags & 0xffff0000) == 0 || options == 0)
    return;
  if ((flags & EFFECT_USE_PALETTE) != 0)
    FieldActor_SetSpriteColor(effect, options->palette);
  if ((flags & EFFECT_USE_PRIORITY) != 0) {
    effect->priorityFlags &= ~1;
    Scene_SetActorSpritePriority(effect, options->priority);
  }
  if ((flags & EFFECT_USE_START_SCALE) != 0) {
    effect->scaleX = options->startScaleX;
    effect->scaleY = options->startScaleY;
  }
  if ((flags & EFFECT_SCALE_TO_TARGET) != 0) {
    s32 target;
    s32 frames;

    script = gFieldEffect_DurationScripts[flags & EFFECT_SCRIPT_MASK];
    target = options->targetScaleX;
    frames = script[3];
    if ((flags & EFFECT_USE_START_SCALE) != 0) {
      effect->scaleRateX = (target - effect->scaleX) / frames;
      effect->scaleRateY = (options->targetScaleY - effect->scaleY) / script[3];
    } else {
      effect->scaleRateX = (target - 0x10000) / frames;
      effect->scaleRateY = (options->targetScaleY - 0x10000) / script[3];
    }
  }
  if ((flags & EFFECT_USE_SCRIPT) != 0) {
    Actor_SelectAnimation(effect, 1);
    Actor_StartScript(effect, options->script);
  }
  if ((flags & EFFECT_USE_ROTATION) != 0)
    sprite->rotation = options->rotation;
  if ((flags & EFFECT_USE_SPIN) != 0)
    effect->spin = options->spin;
  if ((flags & EFFECT_USE_UPDATE) != 0)
    effect->update = options->update;
}

// The cells CrackedTiles_Step watches, and the cell it copies over the one the
// leader steps on.
#define CELL_TABLE gCrackedTiles
#define CELL_SPRITE_DRAW CrackedTiles_Update
#define CELL_PUSH_SLOT_ENTRY Oam_AddSpriteAtDepthClamped
#include "../field/map/cell_sprite.c"

#include "ram_buffer.h"
#include "actor.h"
#include "iwram_call.h"
#include "scene_tables.h"
#include "cell_target.h"

DEFINE_CRACKED_TILES_FIND(CrackedTiles_Find, gCrackedTiles, CrackedTiles_Update)

DEFINE_LEADER_CENTER_IN_CELL(Leader_CenterInCell)

// Centers the leader on its cell, plays sound 161 and copies the cell below
// the table's source cell over the leader's, keeping the source column's and
// row's 64 bit; CrackedTiles_Step calls it the first time the leader steps on
// one of the table's cells.
DEFINE_CRACKED_TILE_CRACK(CrackedTile_Crack, Leader_CenterInCell)

/* VIEW(s1648): tag struct CellTarget as struct CellTarget__s1648; cell_target.h
 * has another definition; tried with the definition the rest of the object has,
 * CrackedTile_Fall moves: 2 of 131 instructions in tla-ja. */
struct CellTarget__s1648 {
  s16 firstFlag;
  s16 flagByte;
  s16 kind;
  s16 x;
  s16 z;
  s16 count;
  s16 current;
  s16 shown;
  s16 tile;
};

#include "io_write.h"

DEFINE_CRACKED_TILE_FALL(CrackedTile_Fall, CellTarget__s1648,
                         Leader_CenterInCell)

#include "../field/field_effect/advance_motion.c"

#undef CELL_TABLE
#undef CELL_SPRITE_DRAW
#undef CELL_PUSH_SLOT_ENTRY
#undef Actor_SelectAnimation
#undef Actor_StartScript
// ---- from scene/jupiter_lighthouse_room_9/cracked_tile_steps.c ----
// @file
// The hover-tile hall of Jupiter Lighthouse's right tower: the cracked tiles'
// steps and landings.

#include "shared/tables.h"
#include "shared/cracked_tile.h"
#include "types.h"
#include "cell_target.h"
#include "system.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "field_effect.h"
#include "event_inline.h"
#include "event_work.h"
#include "module/field/field_effect.h"
#include "module/scene/scene.h"

struct CellTarget;
struct CellTargetEntry;
void CrackedTile_PlaceLanding(s32 x, s32 z, s32 effect);
extern struct CellTarget gCrackedTiles;
void CrackedTile_Crack(struct CellTarget *table, struct CellTargetEntry *entry);
void JupiterLighthouse_StopHovering(void);
void Whirlwind_CarryLeader(void);
extern u32 gFieldEffect_ShortScript[];
extern u32 gFieldEffect_MediumScript[];

/* VIEW(cracked_tile_steps): routine FieldEffect_Spawn as this part declared it;
 * the joined file has another. */
void FieldEffect_Spawn__cracked_tile_steps(
    s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY, s32 velocityZ, u32 flags,
    struct EffectOptions *options) __asm__("FieldEffect_Spawn");
#define FieldEffect_Spawn FieldEffect_Spawn__cracked_tile_steps

DEFINE_CRACKED_TILE_DROP_IN(CrackedTile_DropIn, FieldEffect_Spawn)

void GameFlag_SetByte(s32 flag, s32 value);
/* VIEW(cracked_tile_steps): routine CrackedTile_Fall as this part declared it;
 * the joined file has another. */
void CrackedTile_Fall__cracked_tile_steps(
    struct CellTarget *work,
    struct CellTargetEntry *entry) __asm__("CrackedTile_Fall");
#define CrackedTile_Fall CrackedTile_Fall__cracked_tile_steps

DEFINE_CRACKED_TILES_STEP(CrackedTiles_Step, gCrackedTiles, CrackedTile_Crack,
                          CrackedTile_Fall)

DEFINE_CRACKED_TILE_PLACE_LANDING(CrackedTile_PlaceLanding, gCrackedTiles,
                                  CrackedTile_DropIn)
#undef FieldEffect_Spawn
#undef CrackedTile_Fall
