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
#include "event_inline.h"
#include "actor.h"
#include "ram_buffer.h"
#include "main_routine_work.h"
#include "field_event.h"
#include "event_work.h"
#include "module/field/field.h"
#include "module/scene/scene.h"
#include "module/tla.h"
#include "map.h"
#include "call.h"
#include "field_effect.h"
#include "field_coordinate.h"
#include "shared/push.h"
#include "vram_tab.h"
#include "task.h"
#include "heap_release.h"
#include "item_icon.h"

#include "field/cell_target.h"

#include "jupiter_lighthouse_room_8.h"
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
    Event_PlaySound(134);
    y = object->y;
    floor = object->terrainHeight;
    /* FAKEMATCH(dead): the height and the floor are read before the counter
     * is cleared and again at the end of each pass; as plain C, 8 of 50
     * instructions differ in tla-en. */
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
void CrackedTiles_Find(s32 a, s32 b, s32 c, s32 d, s32 e);

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
    Actor_SetPosition(ACTOR_PILLAR, PIXELS(104), PIXELS(232));
    MapGrid_SetCellHeight(object->x, object->z, 0, 0);
    MapGrid_SetAttributeBAt(object->x, object->z, 0, 128);
  }
  return 0;
}

#include "shared/scene_set_arrival_flags.h"

void GameFlag_SetByte(s32 flag, s32 value);
void Map_CenterOnFollowedPoint(void);
#include "iwram_call.h"
#include "dma.h"
#include "io_reg.h"

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

DEFINE_ACTOR_RAISE_FROM_CELL(Actor_RaiseFromCell, MapGrid_SetCellHeight, MapGrid_SetAttributeBAt)

#include "frame.h"
#include "field_sprite.h"

DEFINE_WHIRLWIND_CARRY_LEADER(Whirlwind_CarryLeader)

DEFINE_POSITION_FLAT_DISTANCE(Position_FlatDistance)

extern s32 gWhirlwindEndScript[];

s32 MapGrid_GetCellBehavior(s32 x, s32 z, s32 layer);
void Actor_StartScript(struct Actor *object, const s32 *script);

DEFINE_WHIRLWIND_UPDATE(Whirlwind_Update, MapGrid_GetCellBehavior, Position_FlatDistance,
                        gWhirlwindEndScript, gWhirlwindRiding)

void FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                       s32 velocityZ, u32 flags, struct EffectOptions *options);

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

void Import_OamAddSpriteAtDepthClamped(struct LightOam *oam, s32 depth);

/// Queues the lit actors' sprites: for each listed actor with a position,
/// projects it from the map's view, skips it off screen, and writes a 16 by 16
/// sprite, or a 32 by 64 one when the big switch is set, at its animation
/// picture and depth.
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
u16 gPushKinds[] = {
    258, 259, 293, 294, 332, 331,
};

s8 gPushFootprints[][4] = {
    {-32, -8, 32, 8}, {-8, -32, 8, 32}, {-32, -16, 32, 0},
    {-8, -32, 8, 32}, {-32, -8, 32, 8}, {-8, -32, 8, 32},
};

DEFINE_PUSH_LEADER_STEPS(gPushLeaderSteps)
// clang-format on
