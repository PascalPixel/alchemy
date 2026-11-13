/// @file
/// The first room of Jupiter Lighthouse's left tower, behind its archer statue.
/// The leader pushes five blue logs about (JupiterLighthouse_PushBlock), Move
/// drops the pillar into a gap in the floor
/// (JupiterLighthouse_DropBlockIntoHole), and cracked tiles give way under a
/// second step (JupiterLighthouse_StepOnCrackedTile) to the floor below. Stairs
/// lead on to room m and back to the hall below the aerie.
///
/// The cracked tiles' code is in jupiter_lighthouse_room_8/, in two parts the
/// shared field effect objects link between, and the tables the scene's getters
/// return are in data/jupiter_lighthouse_room_8.c, which the overlay links
/// after the field's cached pictures.

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
#include "actor.h"
#include "system/ram_buffer.h"
#include "system/main_routine_work.h"
#include "field_event.h"
#include "event_work.h"
#include "module/field/field.h"
#include "module/scene/scene.h"
#include "module/tla.h"
#include "map.h"
#include "call.h"
#include "system/field_effect.h"
#include "field/field_coordinate.h"
#include "shared/push.h"
#include "graphics/vram_tab.h"
#include "system/task.h"
#include "system/heap_release.h"
#include "item_icon.h"
#include "field/cell_target.h"
#include "system/iwram_call.h"
#include "system/dma.h"
#include "system/io_reg.h"
#include "system/frame.h"
#include "field_sprite.h"
#include "shared/cracked_tile.h"
#include "system/io_write.h"
#include "module/field/field_effect.h"
#include "macros.h"
#include "objcmd.h"

/// What the files of Jupiter Lighthouse's left-tower room share: its flag and
/// its actors.

// Set once Move has dropped the pillar into the gap in the floor; cleared as
// the party comes back unless the party comes back from a battle
// (FLAG_KEEP_PARTY_POSITION).
#define FLAG_PILLAR_DROPPED 0xa18

/// The room's statue, pillar and logs, numbering the rows of gActorPlacements.
enum JupiterLighthouseLeftTowerActorId {
  ACTOR_ARCHER_STATUE = ACTOR_FIRST_PLACED,
  ACTOR_PILLAR = ACTOR_FIRST_PLACED + 2,
  /// The first of five blue logs, two standing and three lying, that the
  /// leader pushes.
  ACTOR_FIRST_LOG
};

struct Actor;
struct IsletsVector {
  s32 x;
  s32 y;
  s32 z;
};
s32 CrackedTiles_Step(void);
void MapGrid_SetAttributeBAt(s32 x, s32 z, s32 a, s32 b);
void Actor_DisableById(s32 id);
void CrackedTile_PlaceLanding(s32 x, s32 z, s32 effect);
void JupiterLighthouse_PushBlock(void);
struct Actor *PushTarget_FindAhead(s32 *direction, s32 *slot, s32 *kind);
extern u16 gPushKinds[];
extern s16 gLightSpriteCount;
extern s16 gLightSpritesLarge;
extern s16 gLightSpritePriority;
void Import_ActorSelectAnimation(struct FieldEffect *effect, s32 mode);
void MapGrid_SetCellHeight(s32 x, s32 z, s32 a, s32 b);
extern u16 gPaletteCycleColors[];
extern u16 gPaletteCycleUnlitColors[];
extern s32 gWhirlwindRiding;

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetPlacements(void) {
  return gActorPlacements;
}

/// Where a pushed object comes to rest, read by the push that follows.
struct PushTarget {
  s32 x; ///< 16.16 fixed-point pixels
  s32 y;
  s32 z;
  s32 footprint; ///< index into the footprint table
  s32 id;
  void (*callback)(void);
};

s32 PushTarget_FindSlide(struct PushTarget *target);
void PushTarget_PushFill(struct PushTarget target);

void JupiterLighthouse_StepOnCrackedTile(void) {
  if (CrackedTiles_Step())
    FieldEvent_SetExitRequest(13);
}

void JupiterLighthouse_PushBlock(void) {
  struct PushTarget target;

  if (PushTarget_FindSlide(&target))
    PushTarget_PushFill(target);
}

void JupiterLighthouse_StartMovingBlock(void) {}

/// On cell (6, 14), sets flag FLAG_PILLAR_DROPPED, waits until the object
/// finishes falling and stops (at most 30 frames), then releases it.
void JupiterLighthouse_DropBlockIntoHole(s32 unused, s32 id) {
  struct Actor *object = Actor_Get(id);
  s32 i;
  s32 y;
  s32 floor;

  if (object->x >> 20 == 6 && object->z >> 20 == 14) {
    Event_SetFlag(FLAG_PILLAR_DROPPED);
    System_Wait(10);
    Event_PlaySound(SE_HEAVY_IMPACT);
    y = object->y;
    floor = object->terrainHeight;
    /* FAKEMATCH(dead): the height and the floor are read before the counter is
     * cleared and again at the end of each pass. */
    do {
    } while (0);
    i = 0;
    goto test;
    do {
      System_Wait(1);
      if (++i > 29)
        break;
      y = object->y;
      floor = object->terrainHeight;
    test:;
    } while (y > floor || object->velocityY != 0);
    {
      s32 cleared = 0;

      object->motionFlags = cleared;
      MapGrid_SetCellHeight(object->x, object->z, 0, 0);
    }
    MapGrid_SetAttributeBAt(object->x, object->z, 0, 128);
  }
}

const struct Trigger *Scene_GetEvents(void) { return gSceneEvents; }

s32 PushTarget_FillFootprint(s32 id);

s32 Scene_Initialize(void) {
  ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0x204;
  Actor_Get(ACTOR_ARCHER_STATUE)->scaleX = 0xc000;
  Actor_Get(ACTOR_ARCHER_STATUE)->scaleY = 0xc000;
  FieldActor_SetSpritePriority(ACTOR_FIRST_LOG + 1, 3);
  FieldActor_SetSpritePriority(ACTOR_FIRST_LOG + 3, 3);
  Actor_DisableById(ACTOR_FIRST_LOG);
  Actor_DisableById(ACTOR_FIRST_LOG + 1);
  Actor_DisableById(ACTOR_FIRST_LOG + 2);
  Actor_DisableById(ACTOR_FIRST_LOG + 3);
  Actor_DisableById(ACTOR_FIRST_LOG + 4);
  PushTarget_FillFootprint(ACTOR_FIRST_LOG);
  PushTarget_FillFootprint(ACTOR_FIRST_LOG + 1);
  PushTarget_FillFootprint(ACTOR_FIRST_LOG + 2);
  PushTarget_FillFootprint(ACTOR_FIRST_LOG + 3);
  PushTarget_FillFootprint(ACTOR_FIRST_LOG + 4);
  CrackedTiles_Find(33, 0x280, 0x380, 0, 1);
  if (gMainRoutineWork.entrance == 13)
    CrackedTile_PlaceLanding(19, 0, 0x204);
  else if (gMainRoutineWork.entrance == 14)
    CrackedTile_PlaceLanding(-20, -22, 0x204);
  return 0;
}

s32 Scene_PrepareMap(void) {
  Scene_SetArrivalFlags(0);
  if (Event_GetFlag(FLAG_KEEP_PARTY_POSITION) == 0)
    Event_ClearFlag(FLAG_PILLAR_DROPPED);
  if (Event_GetFlag(FLAG_PILLAR_DROPPED)) {
    struct Actor *object = Actor_Get(ACTOR_PILLAR);

    object->motionFlags = 0;
    object->terrainHeight = -0x200000;
    object->y = -0x200000;
    Actor_SetPosition(ACTOR_PILLAR, 104, 232);
    MapGrid_SetCellHeight(object->x, object->z, 0, 0);
    MapGrid_SetAttributeBAt(object->x, object->z, 0, 128);
  }
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

DEFINE_PALETTE_CYCLE_STEP(PaletteCycle_Step, gPaletteCycleFrame,
                          gPaletteCyclePausesInEvents, gPaletteCycleColors)

extern u16 gLighthousePalette13[];

DEFINE_JUPITER_LIGHTHOUSE_LOAD_PALETTES(JupiterLighthouse_LoadPalettes,
                                        gPaletteCyclePausesInEvents,
                                        gPaletteCycleColors,
                                        gLighthousePalette13, PaletteCycle_Step,
                                        gPaletteCycleUnlitColors)

DEFINE_ACTOR_RAISE_FROM_CELL(Actor_RaiseFromCell, MapGrid_SetCellHeight,
                             MapGrid_SetAttributeBAt)

DEFINE_WHIRLWIND_CARRY_LEADER(Whirlwind_CarryLeader)

DEFINE_POSITION_FLAT_DISTANCE(Position_FlatDistance)

extern s32 gWhirlwindEndScript[];

s32 MapGrid_GetCellBehavior(s32 x, s32 z, s32 layer);
void Actor_StartScript(struct Actor *object, const s32 *script);

DEFINE_WHIRLWIND_UPDATE(Whirlwind_Update, MapGrid_GetCellBehavior,
                        Position_FlatDistance, gWhirlwindEndScript,
                        gWhirlwindRiding)

void FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                       s32 velocityZ, u32 flags,
                       const struct EffectOptions *options);

DEFINE_WHIRLWIND_SPAWN(Whirlwind_Spawn, Import_ActorSelectAnimation,
                       Whirlwind_Update, FieldEffect_Spawn)

DEFINE_ACTOR_EMIT_RISING_SPARKS(Actor_EmitRisingSparks, FieldEffect_Spawn)

DEFINE_JUPITER_LIGHTHOUSE_PLACE_RAISED_ACTORS(
    JupiterLighthouse_PlaceRaisedActors)

extern s8 gPushFootprints[][4];

#define ABS(x) ((x) < 0 ? -(x) : (x))

s32 PushTarget_FillFootprint(s32 id) {
  u8 *map = (u8 *)Ram_HeapSlots->mapWork;
  struct Actor *object = Actor_Get(id);
  s16 shape = *(s16 *)((struct FieldSprite *)object->sprite)->record;
  s16 i = 0;
  u32 j;
  s32 width;
  s32 height;
  s32 x;
  s32 z;
  s32 mapX;
  s32 mapZ;
  s32 toX;
  s32 toZ;

  if ((u16)shape != gPushKinds[(u16)i]) {
  next:
    j = (u16)((i << 16) + 0x10000 >> 16);
    i = j;
    if (j <= 5 && (u16)shape != gPushKinds[j])
      goto next;
  }
  j = (u16)i;
  if (j == 6)
    return 0;
  width = (ABS(gPushFootprints[j][0]) + ABS(gPushFootprints[j][2])) >> 4;
  height = (ABS(gPushFootprints[j][1]) + ABS(gPushFootprints[j][3])) >> 4;
  x = (object->x + (gPushFootprints[j][0] << 16)) >> 20;
  z = (object->z + (gPushFootprints[j][1] << 16)) >> 20;
  mapX = *(s32 *)(map + 328) >> 20;
  i = mapX;
  mapZ = *(s32 *)(map + 332) >> 20;
  toX = x + (u16)i;
  toZ = z + (u16)mapZ;
  MapGrid_CopyCellAttributes(x, z, width, height, toX, toZ);
  Map_FillCellRectByte2(0, x, z, width, height, 255);
  Map_FillCellRectByte2(2, x, z, width, height, 255);
  return 1;
}

#include "../field/map/cellfill.c"

extern s32 gPushLeaderSteps[];

#define ABS(x) ((x) < 0 ? -(x) : (x))

static inline s32 PushTarget_CheckCollision(struct Actor *object,
                                            s32 *position) {
  return MapGrid_CheckCollision(object, position);
}

/// Finds the object the leader is pushing and steps its footprint, one cell
/// at a time in the leader's direction, until a cell of it is blocked
/// (MapGrid_CheckCollision returns 2). Leaves the object's resting place in
/// target and returns 1, or 0 when nothing is pushed or it cannot move at all.
DEFINE_PUSH_TARGET_FIND_SLIDE(PushTarget_FindSlide, PushTarget_FindAhead,
                              gPushFootprints, gPushLeaderSteps,
                              PushTarget_CheckCollision)

/// The whole-pixel half of a 16.16 coordinate.
#define PIXELS(v) (((s16 *)&(v))[1])

DEFINE_PUSH_TARGET_FIND_AHEAD(PushTarget_FindAhead, gPushKinds,
                              gPushLeaderSteps, gPushFootprints)

void PushTarget_UpdatePusher(struct Actor *object);

DEFINE_PUSH_TARGET_PUSH_FILL(PushTarget_PushFill, gPushFootprints,
                             PushTarget_UpdatePusher,
                             Import_ActorSelectAnimation, gPushLeaderSteps)

void PushTarget_UpdatePusher(struct Actor *object) {
  struct IsletsVector position;
  struct Actor *hit;
  u32 i;
  s32 angle = object->facing >> 12;

  angle = ((angle + 2) & 12) << 12;
  position.x = object->x;
  position.y = object->y;
  position.z = object->z;
  FieldVector_AddPolarOffset(0x100000, angle, &position);
  hit = Actor_FindAtCell(&position, 1);
  if (hit != 0) {
    i = 0;
  next:
    if (*((struct ObjectAnimationState *)hit->sprite)->value28 == gPushKinds[i])
      return;
    i++;
    if (i <= 5)
      goto next;
    FieldActor_SetPositionAndResetMotion(object, object->x, object->y,
                                         object->z);
  }
  position.x = object->x;
  position.y = object->y;
  position.z = object->z;
  if (MapGrid_CheckCollision(object, &position) > 0)
    FieldActor_SetPositionAndResetMotion(object, object->x, object->y,
                                         object->z);
}

extern s16 gLightSpriteTile;
extern struct Actor *gLightSpriteActors[];
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
u16 gPushKinds[] = {
    258, 259, 293, 294, 332, 331,
};

s8 gPushFootprints[][4] = {
    {-32, -8, 32, 8}, {-8, -32, 8, 32}, {-32, -16, 32, 0},
    {-8, -32, 8, 32}, {-32, -8, 32, 8}, {-8, -32, 8, 32},
};

DEFINE_PUSH_LEADER_STEPS(gPushLeaderSteps)
// clang-format on

#include "../field/field_effect/move.c"

#undef ABS
#define ABS(v) ((v) < 0 ? -(v) : (v))
#undef PIXELS
#define PIXELS(value) ((value) << 16)
// @file
// The first room of Jupiter Lighthouse's left tower: its copy of the effect
// spawner, and the cracked tiles, found, cracked and fallen through.

struct Actor;
struct CellTarget;
struct FieldEffect;
s32 CrackedTiles_Step(void);
void Import_ActorSelectAnimation(struct FieldEffect *effect, s32 mode);
void Leader_CenterInCell(void);
extern struct CellTarget gCrackedTiles;
void CrackedTile_Crack(struct CellTarget *table, struct CellTargetEntry *entry);

extern const s32 *gFieldEffect_DurationScripts[];

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
  Import_ActorSelectAnimation(effect, (flags + 1) & EFFECT_SCRIPT_MASK);
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
    Import_ActorSelectAnimation(effect, 1);
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

DEFINE_CRACKED_TILES_FIND(CrackedTiles_Find, gCrackedTiles, CrackedTiles_Update)

DEFINE_LEADER_CENTER_IN_CELL(Leader_CenterInCell)

// Centers the leader on its cell, plays sound 161 and copies the cell below
// the table's source cell over the leader's, keeping the source column's and
// row's 64 bit; CrackedTiles_Step calls it the first time the leader steps on
// one of the table's cells.
DEFINE_CRACKED_TILE_CRACK(CrackedTile_Crack, Leader_CenterInCell)

/* VIEW(s1c30): tag struct CellTarget as struct CellTarget__s1c30; cell_target.h
 * has another definition; tried with the definition the rest of the object has,
 * CrackedTile_Fall moves: 2 of 131 instructions in tla-ja. */
struct CellTarget__s1c30 {
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

DEFINE_CRACKED_TILE_FALL(CrackedTile_Fall, CellTarget__s1c30,
                         Leader_CenterInCell)

#include "../field/field_effect/advance_motion.c"

#undef CELL_TABLE
#undef CELL_SPRITE_DRAW
#undef CELL_PUSH_SLOT_ENTRY
// @file
// The first room of Jupiter Lighthouse's left tower: the cracked tiles' steps
// and the fall to the floor below.

struct CellTarget;
struct CellTargetEntry;
s32 CrackedTiles_Step(void);
void CrackedTile_PlaceLanding(s32 x, s32 z, s32 effect);
extern u16 gPushKinds[];
extern struct CellTarget gCrackedTiles;
void CrackedTile_Crack(struct CellTarget *table, struct CellTargetEntry *entry);

DEFINE_CRACKED_TILE_DROP_IN(CrackedTile_DropIn, FieldEffect_Spawn)

void GameFlag_SetByte(s32 flag, s32 value);

DEFINE_CRACKED_TILES_STEP(CrackedTiles_Step, gCrackedTiles, CrackedTile_Crack,
                          CrackedTile_Fall)

DEFINE_CRACKED_TILE_PLACE_LANDING(CrackedTile_PlaceLanding, gCrackedTiles,
                                  CrackedTile_DropIn)

// @file
// The tables of Jupiter Lighthouse's left-tower room that the scene's getters
// return.

void JupiterLighthouse_PushBlock(void);
extern u32 gFieldEffect_ShortScript[];
extern u32 gFieldEffect_MediumScript[];
extern u32 gFieldEffect_LongScript[];

void JupiterLighthouse_StepOnCrackedTile(void);

void JupiterLighthouse_StartMovingBlock(void);

// clang-format off
LATE_DATA DEFINE_FIELD_EFFECT_DURATION_SCRIPT(gFieldEffect_ShortScript, 22)

LATE_DATA DEFINE_FIELD_EFFECT_DURATION_SCRIPT(gFieldEffect_MediumScript, 44)

LATE_DATA DEFINE_FIELD_EFFECT_DURATION_SCRIPT(gFieldEffect_LongScript, 126)

LATE_DATA struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

LATE_DATA u32 gSceneExits[] = {
    SCENE_EXITS(JupiterLighthouseRoom8),
    SCENE_EXIT(1, JupiterLighthouseRoom8, 2),
    SCENE_EXIT(2, JupiterLighthouseRoom8, 1),
    SCENE_EXIT(3, JupiterLighthouseRoom8, 4),
    SCENE_EXIT(4, JupiterLighthouseRoom8, 3),
    SCENE_EXIT(5, JupiterLighthouseRoom8, 6),
    SCENE_EXIT(6, JupiterLighthouseRoom8, 5),
    SCENE_EXIT(7, JupiterLighthouseRoomM, 7),
    SCENE_EXIT(8, JupiterLighthouseRoom6, 8),
    SCENE_EXIT(9, JupiterLighthouseRoom8, 10),
    SCENE_EXIT(10, JupiterLighthouseRoom8, 9),
    SCENE_EXIT(11, JupiterLighthouseRoom8, 12),
    SCENE_EXIT(12, JupiterLighthouseRoom8, 11),
    SCENE_EXIT(13, JupiterLighthouseRoom8, 13),
    SCENE_EXITS_END,
};

LATE_DATA struct ActorPlacement gActorPlacements[] = {
    ACTOR_PLACEMENT(340, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 203, 0, 904, FACING_SOUTHEAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(341, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 184, 0, 296, FACING_SOUTHWEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(312, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 120, 0, 232, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(331, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 408, 0, 160, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(331, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 584, 0, 160, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(332, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 400, 0, 216, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(332, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 480, 0, 200, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(332, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 560, 0, 216, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

LATE_DATA struct Trigger gSceneEvents[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_EXIT, 12, CONDITION_ALWAYS, 12},
    {STEP_TRIGGER_WALKING, 30, CONDITION_ALWAYS, EVENT(JupiterLighthouse_PushBlock)},
    {TRIGGER_STEP, 33, CONDITION_ALWAYS, EVENT(JupiterLighthouse_StepOnCrackedTile)},
    {TRIGGER_STEP, 34, CONDITION_ALWAYS, EVENT(JupiterLighthouse_StepOnCrackedTile)},
    {TRIGGER_STEP, 35, CONDITION_ALWAYS, EVENT(JupiterLighthouse_StepOnCrackedTile)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_PILLAR, CONDITION_ALWAYS, EVENT(JupiterLighthouse_StartMovingBlock)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_PILLAR, CONDITION_ALWAYS, EVENT(JupiterLighthouse_DropBlockIntoHole)},
    {TRIGGER_TABLE_END},
};

LATE_DATA s32 gPaletteCycleFrame = -1;

LATE_DATA s32 gPaletteCyclePausesInEvents = 1;

LATE_DATA s32 gWhirlwindEndScript[] = {
    39, 5, 0, 12, 38,
};
// clang-format on

LATE_DATA const s32 *gFieldEffect_DurationScripts[] = {
    gFieldEffect_ShortScript,
    gFieldEffect_MediumScript,
    gFieldEffect_LongScript,
};
#undef JupiterLighthouse_DropBlockIntoHole
