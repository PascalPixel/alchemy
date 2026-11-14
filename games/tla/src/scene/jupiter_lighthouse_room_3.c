/// @file
/// The room of gaps and logs in Jupiter Lighthouse, between rooms 2, 4 and m.
/// The party jumps the gaps between its ledges (JupiterLighthouse_JumpForward),
/// pushes a blue log off the ledge to the floor below
/// (JupiterLighthouse_PushLogOffTheLedge), and Moves two pillars into the gaps
/// in the floor (JupiterLighthouse_FinishMovingPillar). A Cyclone teleporter
/// leads on (JupiterLighthouse_LeaveByCyclone).
///
/// The cracked tiles' code is in
/// jupiter_lighthouse_room_3/, in two parts the shared field effect objects
/// link between, and the tables the scene's getters return are in
/// data/jupiter_lighthouse_room_3.c, which the overlay links after the field's
/// cached pictures.

#include "system/system.h"
#include "system/io_write.h"
#include "flags.h"
#include "shared/tables.h"
#include "types.h"
#include "system/ram_buffer.h"
#include "actor.h"
#include "system/main_routine_work.h"
#include "scene_tables.h"
#include "system/iwram_call.h"
#include "system/dma.h"
#include "field/event_inline.h"
#include "map.h"
#include "system/irq.h"
#include "field_sprite.h"
#include "call.h"
#include "system/field_effect.h"
#include "field/field_coordinate.h"
#include "module/field/field.h"
#include "module/scene/scene.h"
#include "graphics/vram_tab.h"
#include "system/task.h"
#include "system/heap_release.h"
#include "item_icon.h"
#include "field/cell_target.h"
#include "field/field_sequence_event.h"
#include "field/field.h"
#include "system/io_reg.h"
#include "field_event.h"
#include "system/frame.h"
#include "module/field/field_effect.h"
#include "macros.h"
#include "objcmd.h"

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
    Event_PlaySound(SE_HEAVY_IMPACT);
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
    Event_PlaySound(SE_HEAVY_IMPACT);
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
  gFieldWork->transition = 0x204;
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
    object->terrainHeight = px(-32);
    object->y = px(-32);
    Func_020023b0_(14, px(328), px(584));
    MapGrid_SetCellHeight(object->x, object->z, 0, 0);
  }
  if (Event_GetFlag(FLAG_RIGHT_PILLAR_IN_GAP)) {
    object = Actor_Get(15);
    object->motionFlags = 0;
    object->terrainHeight = px(-32);
    object->y = px(-32);
    Func_020023b0_(15, px(520), px(584));
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

#include "shared/scene_set_arrival_flags.c"

#include "shared/palette_cycle_set_pauses_in_events.h"

#include "shared/palette_cycle_step_at.h"

#include "shared/jupiter_lighthouse_load_palettes_zero.h"

#include "shared/actor_raise_from_cell.h"

#include "shared/whirlwind_carry_leader_lower.h"

s32 Position_FlatDistance(s32 *a, s32 *b) {
  s32 dx = (a[0] - b[0]) >> 16;
  s32 dz = (a[2] - b[2]) >> 16;
  s32 dxsq = dx * dx;
  s32 dzsq = dz * dz;

  return Iwram_Sqrt(dxsq + dzsq);
}

extern s32 gWhirlwindEndScript[];

s32 MapGrid_GetCellBehavior(s32 x, s32 z, s32 layer);
void Actor_StartScript(struct Actor *object, const s32 *script);

#include "shared/whirlwind_update_lower.h"

void FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                       s32 velocityZ, u32 flags,
                       const struct EffectOptions *options);

#include "shared/whirlwind_spawn.h"

#include "shared/actor_emit_rising_sparks.h"

#include "shared/jupiter_lighthouse_place_raised_actors.h"

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

static inline s32 PushTarget_CheckCollision(struct Actor *object,
                                            s32 *position) {
  return MapGrid_CheckCollision(object, position);
}

/// Finds the object the leader is pushing and steps its footprint, one cell
/// at a time in the leader's direction, until a cell of it is blocked
/// (MapGrid_CheckCollision returns 2). Leaves the object's resting place in
/// target and returns 1, or 0 when nothing is pushed or it cannot move at all.
#include "shared/push_target_find_slide.h"

/// The whole-pixel half of a 16.16 coordinate.
#define PIXEL_PART(v) (((s16 *)&(v))[1])

/// Finds the object the leader is pushing: among objects 8 to 63 whose sprite
/// shape is in the six-entry footprint table, the one whose footprint holds the
/// cell one step ahead of the leader, when the leader is not in line with its
/// near edge. Returns the object, with its id and the footprint's index.
#include "shared/push_target_find_ahead.h"

void PushTarget_UpdatePusher(struct Actor *object);

#include "shared/push_target_push_fill.h"

struct Actor *Actor_FindAtCell(s32 *position, s32 mode);

/// Stops the object where it stands (Actor_SetPositionAndResetMotion) when
/// the point a cell ahead along its facing holds another object
/// (Actor_FindAtCell) whose sprite shape is not in the footprint table, and
/// again when its own cell test (MapGrid_CheckCollision) is positive.
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
  FieldVector_AddPolarOffset(px(16), angle, position);
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

/// Queues the lit actors' sprites: for each listed actor with a position,
/// projects it from the map's view, skips it off screen, and writes a 16 by 16
/// sprite, or a 32 by 64 one when the big switch is set, at its animation
/// picture and depth.
#include "shared/light_sprites_draw.h"

static inline void *BumpAlloc(s32 size) { return Heap_Alloc(size); }

#include "shared/light_sprites_init_a.h"

#include "shared/light_sprites_init_b.h"

#include "shared/light_sprites_init_c.h"

#include "shared/light_sprites_add.h"

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

s32 gPushLeaderSteps[] = {                                                               
      PUSH_STEP(16, 0),  PUSH_STEP(16, 0),  PUSH_STEP(16, 0),                  
      PUSH_STEP(0, 16),  PUSH_STEP(0, 16),  PUSH_STEP(0, 16),                  
      PUSH_STEP(0, 16),  PUSH_STEP(-16, 0), PUSH_STEP(-16, 0),                 
      PUSH_STEP(-16, 0), PUSH_STEP(-16, 0), PUSH_STEP(0, -16),                 
      PUSH_STEP(0, -16), PUSH_STEP(0, -16), PUSH_STEP(0, -16),                 
      PUSH_STEP(16, 0),                                                        
  };
// clang-format on

#include "../field/field_effect/move.c"

#undef FLAG_LOG_OFF_LEDGE
#undef FLAG_LEFT_PILLAR_IN_GAP
#undef FLAG_RIGHT_PILLAR_IN_GAP
#undef ABS
#define ABS(v) ((v) < 0 ? -(v) : (v))
#undef PIXEL_PART
// @file
// The room of gaps and logs in Jupiter Lighthouse: its copy of the effect
// spawner, and the cracked tiles, found, cracked and fallen through.

// The cells CrackedTiles_Step watches, and the cell it copies over the one the
// leader steps on.
extern struct CellTarget gCrackedTiles;

void CrackedTiles_Update(void);

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

#define CELL_TABLE gCrackedTiles
#define CELL_SPRITE_DRAW CrackedTiles_Update
#define CELL_PUSH_SLOT_ENTRY Oam_AddSpriteAtDepthClamped
#include "../field/map/cell_sprite.c"

#include "shared/cracked_tiles_find_lower.h"

#include "shared/leader_center_in_cell.h"

// Centers the leader on its cell, plays sound 161 and copies the cell below
// the table's source cell over the leader's, keeping the source column's and
// row's 64 bit; CrackedTiles_Step calls it the first time the leader steps on
// one of the table's cells.
#include "shared/cracked_tile_crack.h"

#include "shared/cracked_tile_fall.h"

#include "../field/field_effect/advance_motion.c"

#undef CELL_TABLE
#undef CELL_SPRITE_DRAW
#undef CELL_PUSH_SLOT_ENTRY
// @file
// The room of gaps and logs in Jupiter Lighthouse: the cracked tiles' steps
// and the fall to the floor below.

#include "system/system.h"
void GameFlag_SetByte(s32 flag, s32 value);
void Map_CenterOnFollowedPoint(void);

extern u16 gPushKinds[];

// The cells CrackedTiles_Step watches, and the cell it copies over the one the
// leader steps on.
extern struct CellTarget gCrackedTiles;

static __inline__ void CamBounds(s32 x, s32 y, s32 z, s32 mode) {
  FieldCamera_MoveWithinMapBounds(x, y, z, mode);
}

static __inline__ void SetWorkTriple(s32 a, s32 b, s32 c) {
  MapGrid_SetScreenShake(a, b, c);
}

#include "shared/cracked_tile_drop_in.h"

void CrackedTile_Crack();
#include "shared/cracked_tiles_step.h"

#include "shared/cracked_tile_place_landing.h"

// @file
// The tables of Jupiter Lighthouse's room of gaps and logs that the scene's
// getters return.

void JupiterLighthouse_StartMovingPillar(void);
void JupiterLighthouse_LeaveByCyclone(void);
void JupiterLighthouse_JumpForward(void);
extern u32 gFieldEffect_ShortScript[];
extern u32 gFieldEffect_MediumScript[];
extern u32 gFieldEffect_LongScript[];

void JupiterLighthouse_PushLogOffTheLedge(void);

// clang-format off
LATE_DATA u32 gFieldEffect_ShortScript[] = {                                                               
      SC_WAIT(1), SC_LOOP(22, SC_TOP),                                     
      SC_WAIT(1), SC_POSITION(0, 0, 0),                                        
      SC_WAIT(1), SC_FREE,                                                     
      SC_END,                                                                  
  };

LATE_DATA u32 gFieldEffect_MediumScript[] = {                                                               
      SC_WAIT(1), SC_LOOP(44, SC_TOP),                                     
      SC_WAIT(1), SC_POSITION(0, 0, 0),                                        
      SC_WAIT(1), SC_FREE,                                                     
      SC_END,                                                                  
  };

LATE_DATA u32 gFieldEffect_LongScript[] = {                                                               
      SC_WAIT(1), SC_LOOP(126, SC_TOP),                                     
      SC_WAIT(1), SC_POSITION(0, 0, 0),                                        
      SC_WAIT(1), SC_FREE,                                                     
      SC_END,                                                                  
  };

LATE_DATA struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

LATE_DATA u32 gSceneExits[] = {
    SCENE_EXITS(JupiterLighthouseRoom3),
    SCENE_EXIT(1, JupiterLighthouseRoomM, 1),
    SCENE_EXIT(2, JupiterLighthouseRoom4, 2),
    SCENE_EXIT(3, JupiterLighthouseRoomM, 3),
    SCENE_EXIT(4, JupiterLighthouseRoomM, 4),
    SCENE_EXIT(6, JupiterLighthouseRoom2, 6),
    SCENE_EXIT(7, JupiterLighthouseRoom2, 7),
    SCENE_EXIT(8, JupiterLighthouseRoom2, 8),
    SCENE_EXIT(9, JupiterLighthouseRoom2, 9),
    SCENE_EXIT(10, JupiterLighthouseRoom2, 10),
    SCENE_EXIT(11, JupiterLighthouseRoom2, 11),
    SCENE_EXIT(12, JupiterLighthouseRoom4, 12),
    SCENE_EXITS_END,
};

LATE_DATA struct ActorPlacement gActorPlacements[] = {
    ACTOR_PLACEMENT(332, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 144, 0, 120, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(332, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 128, 0, 200, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(331, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 168, 0, 192, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(332, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 208, 0, 216, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(373, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 360, 0, 568, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(373, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 488, 0, 568, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(312, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 312, 0, 584, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(312, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 536, 0, 584, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(341, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 248, 0, 680, FACING_SOUTHWEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(341, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 600, 0, 680, FACING_SOUTHEAST, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(341, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 120, 0, 840, FACING_SOUTHWEST, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(341, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 184, 0, 840, FACING_SOUTHEAST, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(341, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 728, 0, 840, FACING_SOUTHEAST, TALK_FACING_KEEP, 1),
    {SCENE_TABLE_END},
};

LATE_DATA struct Trigger gSceneEvents[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_EXIT, 12, CONDITION_ALWAYS, 12},
    {STEP_TRIGGER_WALKING, 30, CONDITION_ALWAYS, EVENT(JupiterLighthouse_PushLogOffTheLedge)},
    {STEP_TRIGGER_WALKING_INTO(FACING_WEST), 31, CONDITION_ALWAYS, EVENT(JupiterLighthouse_JumpForward)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 12, CONDITION_ALWAYS, 0},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 13, CONDITION_ALWAYS, 0},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 14, CONDITION_ALWAYS, (u32)((u8 *)JupiterLighthouse_StartMovingPillar + 1)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 14, CONDITION_ALWAYS, EVENT(JupiterLighthouse_FinishMovingPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 15, CONDITION_ALWAYS, (u32)((u8 *)JupiterLighthouse_StartMovingPillar + 1)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 15, CONDITION_ALWAYS, EVENT(JupiterLighthouse_FinishMovingPillar)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 40, CONDITION_ALWAYS, EVENT(JupiterLighthouse_LeaveByCyclone)},
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
#undef JupiterLighthouse_FinishMovingPillar
