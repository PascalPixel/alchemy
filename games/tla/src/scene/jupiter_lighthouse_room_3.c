/// @file
/// The room of gaps and logs in Jupiter Lighthouse, between rooms 2, 4 and m.
/// The party jumps the gaps between its ledges (JupiterLighthouse_JumpForward),
/// pushes a blue log off the ledge to the floor below
/// (JupiterLighthouse_PushLogOffTheLedge), and Moves two pillars into the gaps
/// in the floor (JupiterLighthouse_FinishMovingPillar). A Cyclone teleporter
/// leads on (JupiterLighthouse_LeaveByCyclone).
///
/// The cracked tiles' code, the Teleporter routines, is in
/// jupiter_lighthouse_room_3/, in two parts the shared field effect objects
/// link between, and the tables the scene's getters return are in
/// data/jupiter_lighthouse_room_3.c, which the overlay links after the field's
/// cached pictures.

#include "flags.h"
#include "shared/whirlwind.h"
#include "shared/tables.h"
#include "shared/jupiter_lighthouse.h"
#include "shared/effects.h"
#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "scene_tables.h"
#include "iwram_call.h"
#include "dma.h"
#include "event_inline.h"
#include "map.h"
#include "irq.h"
#include "field_sprite.h"
#include "call.h"
#include "field_effect.h"
#include "field_coordinate.h"
#include "module/field/field.h"
#include "module/scene/scene.h"
#include "shared/push.h"
#include "vram_tab.h"
#include "task.h"
#include "heap_release.h"
#include "item_icon.h"

#include "field/cell_target.h"

/// Where a pushed object comes to rest, read by the push that follows.
struct PushTarget {
  s32 x; ///< 16.16 fixed-point pixels
  s32 y;
  s32 z;
  s32 footprint; ///< index into the footprint table
  s32 id;
  void (*callback)(void);
};

// Set once the log has been pushed off the ledge this visit.
#define FLAG_LOG_OFF_LEDGE 0x200
// Set as Move drops each pillar into its gap.
#define FLAG_LEFT_PILLAR_IN_GAP 0xa04
#define FLAG_RIGHT_PILLAR_IN_GAP 0xa05

/// The log the leader pushes off the ledge, one of four blue logs.
/// ★ Named from what the player sees: a blue log (spr_331.png)
enum { ACTOR_LEDGE_LOG = ACTOR_FIRST_PLACED + 2 };

void Scene_SetArrivalFlags(s32 a);
void Actor_SetSpritePriorityDirect(struct Actor *object, s32 a);
void MapGrid_SetCellHeight();
void Actor_DisableById(s32 id);
void Field_TryJumpForward(void);
void HangingVine_Place(s32 id, s32 a);
void MapGrid_SetAttributeBAt(s32 x, s32 z, s32 a, s32 b);
void Actor_DepartInRing(s32 id);
s32 PushTarget_FillFootprint(s32 id);
s32 PushTarget_FindSlide(struct PushTarget *target);
void PushTarget_PushFill(struct PushTarget target);

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static __inline__ void Lift_(s32 x, s32 z, s32 a, s32 b) {
  MapGrid_SetCellHeight(x, z, a, b);
}

static __inline__ void Func_020023b0_(s32 id, s32 x, s32 z) {
  Actor_SetHorizontalPositionWithTerrain(id, x, z);
}
void JupiterLighthouse_StartMovingPillar(void);
void JupiterLighthouse_LeaveByCyclone(void);
void JupiterLighthouse_JumpForward(void);
extern s32 gWhirlwindRiding;
struct Actor *PushTarget_FindAhead(s32 *direction, s32 *slot, s32 *kind);
s32 MapGrid_CheckCollision(struct Actor *object, s32 *position);
extern u16 gPushKinds[];
extern s16 gLightSpriteCount;
extern struct Actor *gLightSpriteActors[];
extern struct LightOam gLightSpriteOams[];
extern s16 gLightSpritesLarge;
extern s16 gLightSpritePriority;
s32 BattleEffect_GetObjectPriority(struct Actor *actor);
void PaletteCycle_StepAt(void);

extern s32 gPaletteCycleFrame;
extern s32 gPaletteCyclePausesInEvents;
extern const u16 gPaletteCycleColors[];
extern const u16 gPaletteCycleUnlitPalette[];
extern const u16 gPaletteCycleUnlitColors[];
#include "event_work.h"
#include "field.h"

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetPlacements(void) {
  return gActorPlacements;
}

void JupiterLighthouse_DrawLogBelowLedge(struct Actor *object) {
  if (object->x >> 20 == 14 && object->z >> 20 == 12)
    Actor_SetSpritePriorityDirect(object, 2);
}

void JupiterLighthouse_PushLogOffTheLedge(void) {
  struct Actor *object = Actor_Get(ACTOR_LEDGE_LOG);
  struct PushTarget target;
  s32 i;
  s32 x;

  object->update = (void (*)(void))JupiterLighthouse_DrawLogBelowLedge;
  if (PushTarget_FindSlide(&target))
    PushTarget_PushFill(target);
  if (object->x >> 20 == 16 && object->z >> 20 == 12 &&
      Event_GetFlag(FLAG_LOG_OFF_LEDGE) == 0) {
    Event_SetFlag(FLAG_LOG_OFF_LEDGE);
    FieldActor_SetSpritePriority(ACTOR_LEDGE_LOG, 3);
    object->motionFlags = 3;
    System_Wait(5);
    Event_PlaySound(134);
    object->motionFlags = 2;
    for (i = 0, x = 0xb00000; i <= 3; i++, x += 0x100000) {
      Lift_(0x1000000, x, 0, 0);
      MapGrid_SetCellBehavior(0x1000000, x, 0, 0);
      if (i <= 1)
        MapGrid_SetAttributeBAt(0x1000000, x, 0, 128);
    }
  }
  object->update = 0;
}

void JupiterLighthouse_StartMovingPillar(void) {}

/// Sets FLAG_LEFT_PILLAR_IN_GAP or FLAG_RIGHT_PILLAR_IN_GAP when the pillar
/// stands on cell (20, 36) or (32, 36), then waits up to 30 frames for it to
/// land and lifts it there.
void JupiterLighthouse_FinishMovingPillar(s32 unused, s32 id) {
  struct Actor *object = Actor_Get(id);
  s32 placed = 0;
  s32 i;
  if (object->x >> 20 == 20 && object->z >> 20 == 36) {
    Event_SetFlag(FLAG_LEFT_PILLAR_IN_GAP);
    placed = 1;
  }
  if (object->x >> 20 == 32 && object->z >> 20 == 36) {
    Event_SetFlag(FLAG_RIGHT_PILLAR_IN_GAP);
    placed = 1;
  }
  if (placed) {
    System_Wait(10);
    Event_PlaySound(134);
    for (i = 0; i < 30; i++) {
      if (object->y <= object->terrainHeight && object->velocityY == 0)
        break;
      System_Wait(1);
    }
    object->motionFlags = 0;
    MapGrid_SetCellHeight(object->x, object->z, 0, 0);
  }
}

void JupiterLighthouse_LeaveByCyclone(void) { Import_EventSetExitRequest(12); }

const struct Trigger *Scene_GetEvents(void) { return gSceneEvents; }

s32 Scene_Initialize(void) {
  ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0x204;
  HangingVine_Place(12, 1);
  HangingVine_Place(13, 1);
  if (Event_GetFlag(FLAG_KEEP_PARTY_POSITION) == 0 &&
      gMainRoutineWork.entrance == 12)
    Actor_DepartInRing(gMainRoutineWork.currentOwner);
  if (Event_GetFlag(FLAG_KEEP_PARTY_POSITION) == 0 &&
      gMainRoutineWork.entrance == 13)
    Actor_DepartInRing(gMainRoutineWork.currentOwner);
  return 0;
}

s32 Scene_PrepareMap(void) {
  struct Actor *object;
  s32 i;
  s32 x;

  Scene_SetArrivalFlags(0);
  if (Event_GetFlag(FLAG_LEFT_PILLAR_IN_GAP)) {
    object = Actor_Get(14);
    object->motionFlags = 0;
    object->terrainHeight = PIXELS(-32);
    object->y = PIXELS(-32);
    Func_020023b0_(14, PIXELS(328), PIXELS(584));
    MapGrid_SetCellHeight(object->x, object->z, 0, 0);
  }
  if (Event_GetFlag(FLAG_RIGHT_PILLAR_IN_GAP)) {
    object = Actor_Get(15);
    object->motionFlags = 0;
    object->terrainHeight = PIXELS(-32);
    object->y = PIXELS(-32);
    Func_020023b0_(15, PIXELS(520), PIXELS(584));
    MapGrid_SetCellHeight(object->x, object->z, 0, 0);
  }
  Actor_Get(8)->priorityFlags |= 32;
  Actor_Get(9)->priorityFlags |= 32;
  Actor_Get(ACTOR_LEDGE_LOG)->priorityFlags |= 32;
  Actor_Get(11)->priorityFlags |= 32;
  FieldActor_SetSpritePriority(8, 3);
  FieldActor_SetSpritePriority(10, 3);
  FieldActor_SetSpritePriority(9, 3);
  FieldActor_SetSpritePriority(11, 3);
  Actor_Get(ACTOR_LEDGE_LOG)->priorityFlags |= 2;
  Actor_DisableById(8);
  Actor_DisableById(9);
  Actor_DisableById(10);
  Actor_DisableById(11);
  PushTarget_FillFootprint(8);
  PushTarget_FillFootprint(9);
  PushTarget_FillFootprint(10);
  PushTarget_FillFootprint(11);
  if (Event_GetFlag(FLAG_LOG_OFF_LEDGE)) {
    for (i = 0, x = 0xb00000; i <= 3; i++, x += 0x100000) {
      Lift_(0x1000000, x, 0, 0);
      MapGrid_SetCellBehavior(0x1000000, x, 0, 0);
      if (i <= 1)
        MapGrid_SetAttributeBAt(0x1000000, x, 0, 128);
    }
  }
  return 0;
}

void JupiterLighthouse_JumpForward(void) { Field_TryJumpForward(); }

#include "shared/scene_set_arrival_flags.h"

#include "io_reg.h"
#include "field_event.h"

void PaletteCycle_SetPausesInEvents(s32 value) {
  gPaletteCyclePausesInEvents = value;
}

DEFINE_PALETTE_CYCLE_STEP_AT(PaletteCycle_StepAt, gPaletteCycleFrame,
                             gPaletteCyclePausesInEvents, gPaletteCycleColors,
                             0x05000184)

DEFINE_JUPITER_LIGHTHOUSE_LOAD_PALETTES_ZERO(
    JupiterLighthouse_LoadPalettesZero, gPaletteCyclePausesInEvents,
    gPaletteCycleColors, gPaletteCycleUnlitPalette, gPaletteCycleFrame,
    PaletteCycle_StepAt, gPaletteCycleUnlitColors)

DEFINE_ACTOR_RAISE_FROM_CELL(Actor_RaiseFromCell, MapGrid_SetCellHeight, MapGrid_SetAttributeBAt)

#include "frame.h"

DEFINE_WHIRLWIND_CARRY_LEADER_LOWER(Whirlwind_CarryLeaderLower)

s32 Position_FlatDistance(s32 *a, s32 *b) {
  s32 dx = (a[0] - b[0]) >> 16;
  s32 dz = (a[2] - b[2]) >> 16;
  s32 dxsq = dx * dx;
  s32 dzsq = dz * dz;

  return Iwram_Sqrt(dxsq + dzsq);
}

extern const s32 gWhirlwindEndScript[];

s32 MapGrid_GetCellBehavior(s32 x, s32 z, s32 layer);
void Actor_StartScript(struct Actor *object, const s32 *script);

DEFINE_WHIRLWIND_UPDATE_LOWER(Whirlwind_UpdateLower, MapGrid_GetCellBehavior,
                              Position_FlatDistance, gWhirlwindEndScript,
                              gWhirlwindRiding)

void FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                       s32 velocityZ, u32 flags, struct EffectOptions *options);

DEFINE_WHIRLWIND_SPAWN(Whirlwind_Spawn, Actor_SelectAnimation,
                       Whirlwind_UpdateLower, FieldEffect_Spawn)

DEFINE_ACTOR_EMIT_RISING_SPARKS(Actor_EmitRisingSparks, FieldEffect_Spawn)

DEFINE_JUPITER_LIGHTHOUSE_PLACE_RAISED_ACTORS(
    JupiterLighthouse_PlaceRaisedActors)

extern s8 gPushFootprints[][4];

/// Rewrites the map cells under a pushable actor: its sprite shape picks one of
/// the six footprints in gPushFootprints (four signed bytes each, in sixteenths
/// of a cell); the cells under it are copied from the map's spare area
/// (Map_CopyCellAttributeRect) and byte 2 of both collision layers there is
/// filled with 255. Returns 1, or 0 for a shape with no footprint.

#define ABS(x) ((x) < 0 ? -(x) : (x))

/// Rewrites the map cells under a pushable actor: its sprite shape picks one of
/// the six footprints in gPushFootprints (four signed bytes each, in sixteenths
/// of a cell); the cells under it are copied from the map's spare area
/// (Map_CopyCellAttributeRect) and byte 2 of both collision layers there is
/// filled with 255. Returns 1, or 0 for a shape with no footprint.
s32 PushTarget_FillFootprint(s32 id) {
  u8 *map = (u8 *)Ram_HeapSlots->mapWork;
  struct Actor *actor = Actor_Get(id);
  s16 shape = *(s16 *)((struct FieldSprite *)actor->sprite)->record;
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
  x = (actor->x + (gPushFootprints[j][0] << 16)) >> 20;
  z = (actor->z + (gPushFootprints[j][1] << 16)) >> 20;
  mapX = *(s32 *)(map + 328) >> 20;
  i = mapX; // i is reused to hold mapX.
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

static inline s32 Func_02002338_(struct Actor *object, s32 *position) {
  return MapGrid_CheckCollision(object, position);
}

/// Finds the object the leader is pushing and steps its footprint, one cell
/// at a time in the leader's direction, until a cell of it is blocked
/// (MapGrid_CheckCollision returns 2). Leaves the object's resting place in
/// target and returns 1, or 0 when nothing is pushed or it cannot move at all.
DEFINE_PUSH_TARGET_FIND_SLIDE(PushTarget_FindSlide, PushTarget_FindAhead,
                              gPushFootprints, gPushLeaderSteps, Func_02002338_)

/// The whole-pixel half of a 16.16 coordinate.
#define PIXELS(v) (((s16 *)&(v))[1])

/// Finds the object the leader is pushing: among objects 8 to 63 whose sprite
/// shape is in the six-entry footprint table, the one whose footprint holds the
/// cell one step ahead of the leader, when the leader is not in line with its
/// near edge. Returns the object, with its id and the footprint's index.
DEFINE_PUSH_TARGET_FIND_AHEAD(PushTarget_FindAhead, gPushKinds,
                              gPushLeaderSteps, gPushFootprints)

void PushTarget_UpdatePusher(struct Actor *object);

DEFINE_PUSH_TARGET_PUSH_FILL(PushTarget_PushFill, gPushFootprints,
                             PushTarget_UpdatePusher, Actor_SelectAnimation,
                             gPushLeaderSteps)

struct Actor *Actor_FindAtCell(s32 *position, s32 mode);

/// Stops the object where it stands (Actor_SetPositionAndResetMotion) when
/// the point a cell ahead along its facing holds another object
/// (Actor_FindAtCell) whose sprite shape is not in the footprint table, and again
/// when its own cell test (MapGrid_CheckCollision) is positive.
void PushTarget_UpdatePusher(struct Actor *object) {
  s32 position[3];
  struct Actor *other;
  u32 i;
  s32 angle;

  angle = object->facing >> 12;
  angle = ((angle + 2) & 12) << 12;
  position[0] = object->x;
  position[1] = object->y;
  position[2] = object->z;
  FieldVector_AddPolarOffset(0x100000, angle, position);
  other = Actor_FindAtCell(position, 1);
  if (other != NULL) {
    i = 0;
  next:
    if (*((struct ObjectAnimationState *)other->sprite)->value28 ==
        gPushKinds[i])
      return;
    i++;
    if (i < 6)
      goto next;
    FieldActor_SetPositionAndResetMotion(object, object->x, object->y,
                                         object->z);
  }
  position[0] = object->x;
  position[1] = object->y;
  position[2] = object->z;
  if (MapGrid_CheckCollision(object, position) > 0)
    FieldActor_SetPositionAndResetMotion(object, object->x, object->y,
                                         object->z);
}

extern s16 gLightSpriteTile;

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
