/// @file
/// The ground floor of Jupiter Lighthouse, inside its main gates. The party
/// first comes in from below, by the basement's Cyclone teleporter (entrance
/// 6). Standing on the button by the gates holds them open
/// (JupiterLighthouse_StepOnButton, JupiterLighthouse_StepOffButton); the
/// pillar pushed onto it opens them for good
/// (JupiterLighthouse_CheckBlockOnCell), so the way out to the entrance stays
/// open. A Cyclone on the teleporter here leads back down
/// (JupiterLighthouse_LeaveByCyclone).
///
/// The cracked tiles' code is in
/// jupiter_lighthouse_room_1/, in two parts the shared field effect objects
/// link between.

#include "shared/whirlwind.h"
#include "shared/tables.h"
#include "flags.h"
#include "shared/jupiter_lighthouse.h"
#include "shared/actors.h"
#include "shared/effects.h"
#include "battle/battle.h"
#include "field/field.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "lib.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "types.h"
#include "scene_tables.h"
#include "field/event_inline.h"
#include "field_event.h"
#include "system/ram_buffer.h"
#include "system/main_routine_work.h"
#include "debug.h"
#include "event_work.h"
#include "module/scene/scene.h"
#include "module/tla.h"
#include "system/irq.h"
#include "field_sprite.h"
#include "call.h"
#include "system/field_effect.h"
#include "field/field_coordinate.h"
#include "map.h"
#include "graphics/vram_tab.h"
#include "system/task.h"
#include "item_icon.h"
#include "module/field/field.h"
#include "objcmd.h"
#include "field/cell_target.h"
#include "system/iwram_call.h"
#include "system/dma.h"
#include "system/io_reg.h"
#include "actor.h"
#include "system/frame.h"
#include "shared/cracked_tile.h"
#include "system/heap_release.h"
#include "system/io_write.h"
#include "module/field/field_effect.h"

// Set while the leader stands on the button by the gates, which holds them
// open.
#define FLAG_GATES_BUTTON_HELD 0x200

/// The pillar the party pushes onto the button by the gates, after the two
/// angel statues in gActorPlacements.
/// ★ Named from what the player sees: a grey stone pillar (spr_312.png)
enum { ACTOR_GATES_PILLAR = ACTOR_FIRST_PLACED + 2 };

struct Actor;
struct LightOam;
void Actor_ArriveInRing(s32 owner);
extern s32 gWhirlwindRiding;
s32 MapGrid_GetCellBehavior(s32 x, s32 z, s32 layer);
extern s16 gLightSpriteTile;
extern s16 gLightSpriteCount;
extern struct Actor *gLightSpriteActors[];
extern s16 gLightSpritesLarge;
extern s16 gLightSpritePriority;
/* VIEW(first): routine Oam_AddSpriteAtDepthClamped as this part declared it;
 * the joined file has another. */
void Oam_AddSpriteAtDepthClamped__first(
    struct LightOam *oam, s32 depth) __asm__("Oam_AddSpriteAtDepthClamped");
#define Oam_AddSpriteAtDepthClamped Oam_AddSpriteAtDepthClamped__first
void JupiterLighthouse_StepOnButton(void);
void JupiterLighthouse_StepOffButton(void);
extern u16 gPaletteCycleColors[];
extern u16 gPaletteCycleUnlitPalette[];
extern u16 gPaletteCycleUnlitColors[];

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetPlacements(void) {
  return gActorPlacements;
}

void JupiterLighthouse_LeaveByCyclone(void) { Import_EventSetExitRequest(6); }

void JupiterLighthouse_OpenDoor(s32 animate) {
  if (animate == 1) {
    Event_PlaySound(133);
    MapGrid_CopyCellsLayered(81, 20, 2, 3, 79, 29);
    MapGrid_CopyCellsLayered(81, 20, 2, 3, 79, 37);
    System_Wait(6);
  }
  MapGrid_CopyCellsLayered(84, 20, 2, 3, 79, 29);
  MapGrid_CopyCellsLayered(84, 20, 2, 3, 79, 37);
  MapGrid_CopyCellAttributes(20, 22, 2, 1, 15, 31);
  MapGrid_CopyCellAttributes(20, 22, 2, 1, 15, 39);
}

void JupiterLighthouse_CloseDoor(void) {
  Event_PlaySound(134);
  MapGrid_CopyCellsLayered(81, 20, 2, 3, 79, 29);
  MapGrid_CopyCellsLayered(81, 20, 2, 3, 79, 37);
  System_Wait(6);
  MapGrid_CopyCellsLayered(87, 20, 2, 3, 79, 29);
  MapGrid_CopyCellsLayered(87, 20, 2, 3, 79, 37);
  MapGrid_CopyCellAttributes(23, 22, 2, 1, 15, 31);
  MapGrid_CopyCellAttributes(23, 22, 2, 1, 15, 39);
}

DEFINE_JUPITER_LIGHTHOUSE_CHECK_BLOCK_ON_CELL(
    JupiterLighthouse_CheckBlockOnCell, ACTOR_GATES_PILLAR,
    FLAG_JUPITER_LIGHTHOUSE_GATES_PILLAR_PLACED, 17, 33,
    JupiterLighthouse_OpenDoor)

DEFINE_BUTTON_STEP_ON(JupiterLighthouse_StepOnButton, FLAG_GATES_BUTTON_HELD,
                      JupiterLighthouse_OpenDoor)

DEFINE_BUTTON_STEP_OFF(JupiterLighthouse_StepOffButton, FLAG_GATES_BUTTON_HELD,
                       JupiterLighthouse_CloseDoor,
                       JupiterLighthouse_CheckBlockOnCell)

void JupiterLighthouse_FinishMovingBlock(void) {
  System_Wait(10);
  JupiterLighthouse_CheckBlockOnCell();
}

const struct Trigger *Scene_GetEvents(void) { return gSceneEvents; }

s32 Scene_Initialize(void) {
  ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0x204;
  if (!Event_GetFlag(FLAG_KEEP_PARTY_POSITION) &&
      gMainRoutineWork.entrance == 6)
    Actor_ArriveInRing(gMainRoutineWork.currentOwner);
  if (Event_GetFlag(FLAG_JUPITER_LIGHTHOUSE_GATES_PILLAR_PLACED)) {
    Actor_SetPosition(ACTOR_GATES_PILLAR, PIXELS(280), PIXELS(536));
    JupiterLighthouse_OpenDoor(0);
  }
  if (Event_GetFlag(FLAG_GATES_BUTTON_HELD))
    JupiterLighthouse_OpenDoor(0);
  return 0;
}

s32 Scene_PrepareMap(void) {
  Scene_SetArrivalFlags(0);
  return 0;
}

#include "shared/scene_set_arrival_flags.c"

void GameFlag_SetByte(s32 flag, s32 value);
void Map_CenterOnFollowedPoint(void);

extern s32 gPaletteCyclePausesInEvents;

void PaletteCycle_SetPausesInEvents(s32 value) {
  gPaletteCyclePausesInEvents = value;
}

extern s32 gPaletteCycleFrame;

/// Sets the counter, its value built as a word.
static inline void Counter_Set(s32 value) { gPaletteCycleFrame = value; }

DEFINE_PALETTE_CYCLE_STEP_AT(PaletteCycle_StepAt, gPaletteCycleFrame,
                             gPaletteCyclePausesInEvents, gPaletteCycleColors,
                             0x05000184)

void MapGrid_SetCellHeight();
void MapGrid_SetAttributeBAt();

DEFINE_JUPITER_LIGHTHOUSE_LOAD_PALETTES(JupiterLighthouse_LoadPalettes,
                                        gPaletteCyclePausesInEvents,
                                        gPaletteCycleColors,
                                        gPaletteCycleUnlitPalette,
                                        PaletteCycle_StepAt,
                                        gPaletteCycleUnlitColors)

DEFINE_ACTOR_RAISE_FROM_CELL(Actor_RaiseFromCell, MapGrid_SetCellHeight,
                             MapGrid_SetAttributeBAt)

DEFINE_WHIRLWIND_CARRY_LEADER_LOWER(Whirlwind_CarryLeaderLower)

DEFINE_POSITION_FLAT_DISTANCE(Position_FlatDistance)

extern s32 gWhirlwindEndScript[];

s32 Position_FlatDistance(s32 *from, s32 *to);
void Actor_StartScript(struct Actor *object, const s32 *script);

DEFINE_WHIRLWIND_UPDATE_LOWER(Whirlwind_UpdateLower, MapGrid_GetCellBehavior,
                              Position_FlatDistance, gWhirlwindEndScript,
                              gWhirlwindRiding)

/* VIEW(first): routine FieldEffect_Spawn as this part declared it; the joined
 * file has another. */
void FieldEffect_Spawn__first(
    s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY, s32 velocityZ, u32 flags,
    struct EffectOptions *options) __asm__("FieldEffect_Spawn");
#define FieldEffect_Spawn FieldEffect_Spawn__first

DEFINE_WHIRLWIND_SPAWN(Whirlwind_Spawn, Actor_SelectAnimation,
                       Whirlwind_UpdateLower, FieldEffect_Spawn)

DEFINE_ACTOR_EMIT_RISING_SPARKS_B(Actor_EmitRisingSparksB, FieldEffect_Spawn)

DEFINE_JUPITER_LIGHTHOUSE_PLACE_RAISED_ACTORS(
    JupiterLighthouse_PlaceRaisedActors)

extern struct LightOam gLightSpriteOams[];

/// Queues the lit actors' sprites: for each listed actor with a position,
/// projects it from the map's view, skips it off screen, and writes a 16 by 16
/// sprite, or a 32 by 64 one when the big switch is set, at its animation
/// picture and depth.
DEFINE_LIGHT_SPRITES_DRAW(LightSprites_Draw, gLightSpriteTile, gLightSpriteOams,
                          gLightSpriteCount, gLightSpriteActors,
                          gLightSpritesLarge, gLightSpritePriority,
                          BattleEffect_GetObjectPriority,
                          Oam_AddSpriteAtDepthClamped)

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

void JupiterLighthouse_LeaveByCyclone(void);
void JupiterLighthouse_FinishMovingBlock(void);

// clang-format off
DEFINE_FIELD_EFFECT_DURATION_SCRIPT(gFieldEffect_ShortScript, 22)

DEFINE_FIELD_EFFECT_DURATION_SCRIPT(gFieldEffect_MediumScript, 44)

DEFINE_FIELD_EFFECT_DURATION_SCRIPT(gFieldEffect_LongScript, 126)

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(254),
    SCENE_EXIT(1, 255, 1),
    SCENE_EXIT(2, 251, 2),
    SCENE_EXIT(3, 251, 3),
    SCENE_EXIT(4, 254, 5),
    SCENE_EXIT(5, 254, 4),
    SCENE_EXIT(6, 252, 6),
    SCENE_EXITS_END,
};

struct ActorPlacement gActorPlacements[] = {
    {341, CONDITION_ALWAYS, 1, PIXELS(200), 0, PIXELS(744), FACING_SOUTHWEST, 2, 0},
    {341, CONDITION_ALWAYS, 1, PIXELS(296), 0, PIXELS(744), FACING_SOUTHEAST, 2, 1},
    {312, CONDITION_ALWAYS, 1, PIXELS(232), 0, PIXELS(536), FACING_EAST, 2, 0},
    {SCENE_TABLE_END},
};

struct Trigger gSceneEvents[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_STEP, 20, FLAG_JUPITER_LIGHTHOUSE_GATES_PILLAR_PLACED, EVENT(JupiterLighthouse_StepOnButton)},
    {TRIGGER_STEP, 21, FLAG_JUPITER_LIGHTHOUSE_GATES_PILLAR_PLACED, EVENT(JupiterLighthouse_StepOffButton)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 30, CONDITION_ALWAYS, EVENT(JupiterLighthouse_LeaveByCyclone)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_GATES_PILLAR, FLAG_JUPITER_LIGHTHOUSE_GATES_PILLAR_PLACED, EVENT(JupiterLighthouse_FinishMovingBlock)},
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

#undef FLAG_GATES_BUTTON_HELD
#undef Oam_AddSpriteAtDepthClamped
#undef FieldEffect_Spawn
// @file
// Jupiter Lighthouse's ground floor: its copy of the effect spawner, and the
// cracked tiles, found, cracked and fallen through.

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

DEFINE_CRACKED_TILES_FIND_LOWER(CrackedTiles_FindLower, gCrackedTiles,
                                CrackedTiles_Update)

DEFINE_LEADER_CENTER_IN_CELL(Leader_CenterInCell)

// Centers the leader on its cell, plays sound 161 and copies the cell below
// the table's source cell over the leader's, keeping the source column's and
// row's 64 bit; CrackedTiles_Step calls it the first time the leader steps on
// one of the table's cells.
DEFINE_CRACKED_TILE_CRACK(CrackedTile_Crack, Leader_CenterInCell)

struct CellTargetEntry;

/* VIEW(s156c): tag struct CellTarget as struct CellTarget__s156c; cell_target.h
 * has another definition; tried with the definition the rest of the object has,
 * CrackedTile_Fall moves: 2 of 131 instructions in tla-ja. */
struct CellTarget__s156c {
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

DEFINE_CRACKED_TILE_FALL(CrackedTile_Fall, CellTarget__s156c,
                         Leader_CenterInCell)

#include "../field/field_effect/advance_motion.c"

#undef CELL_TABLE
#undef CELL_SPRITE_DRAW
#undef CELL_PUSH_SLOT_ENTRY
#undef Actor_SelectAnimation
#undef Actor_StartScript
// @file
// Jupiter Lighthouse's ground floor: the cracked tiles' steps and landings.

void GameFlag_SetByte(s32 flag, s32 value);
void Map_CenterOnFollowedPoint(void);

void JupiterLighthouse_StepOnButton(void);
void JupiterLighthouse_StepOffButton(void);
extern u32 gFieldEffect_ShortScript[];
extern u32 gFieldEffect_MediumScript[];
extern u32 gFieldEffect_LongScript[];

/* VIEW(cracked_tile_steps): routine FieldEffect_Spawn as this part declared it;
 * the joined file has another. */
void FieldEffect_Spawn__cracked_tile_steps(
    s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY, s32 velocityZ, u32 flags,
    struct EffectOptions *options) __asm__("FieldEffect_Spawn");
#define FieldEffect_Spawn FieldEffect_Spawn__cracked_tile_steps

static __inline__ void CamBounds(s32 x, s32 y, s32 z, s32 mode) {
  FieldCamera_MoveWithinMapBounds(x, y, z, mode);
}

static __inline__ void SetWorkTriple(s32 a, s32 b, s32 c) {
  MapGrid_SetScreenShake(a, b, c);
}

DEFINE_CRACKED_TILE_DROP_IN_LOWER(CrackedTile_DropInLower, FieldEffect_Spawn)

extern struct CellTarget gCrackedTiles;
void CrackedTile_Crack();
void CrackedTile_Fall();

DEFINE_CRACKED_TILES_STEP(CrackedTiles_Step, gCrackedTiles, CrackedTile_Crack,
                          CrackedTile_Fall)

DEFINE_CRACKED_TILE_PLACE_LANDING(CrackedTile_PlaceLanding, gCrackedTiles,
                                  CrackedTile_DropInLower)
#undef FieldEffect_Spawn
