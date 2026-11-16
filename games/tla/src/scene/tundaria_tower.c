/// @file
/// Tundaria Tower, the frozen tower that holds the Center Prong, in seven rooms
/// that share this overlay (SceneId_TundariaTowerEntrance to
/// SceneId_TundariaTowerBurstBrooch).
///
/// In the first hall (room 2) Parch dries the water left and right of the
/// doors, and the Center Prong lies frozen in a crystal that only Burst
/// breaks. The floors above are crossed by sliding on black ice; Pound
/// flattens the ice columns and Move slides the pillars, and the Mars Djinni
/// Reflux waits at the end of one sliding puzzle (room 4). Beyond the room of
/// pillars (room 6), Reveal opens an icy wall and Burst a brown one outside;
/// Burst on the dead end by the chests (room 7) brings its rocks down.
#include "djinn_ids.h"
#include "fixed_math.h"
#include "flags.h"
#include "item_ids.h"
#include "scene_ids.h"
#include "sprite_ids.h"
#include "types.h"
#include "macros.h"
#include "version.h"
#include "battle/battle.h"
#include "field/actor.h"
#include "field/actor_script.h"
#include "field/call.h"
#include "field/effect_sprite.h"
#include "field/event.h"
#include "field/event_inline.h"
#include "field/field.h"
#include "field/field_effect.h"
#include "field/field_event.h"
#include "field/field_sequence_event.h"
#include "field/field_sprite.h"
#include "field/flag_call.h"
#include "field/map.h"
#include "field/map_grid.h"
#include "field/object.h"
#include "field/particle.h"
#include "field/scene_tables.h"
#include "game/game.h"
#include "gamepak/far_call/system.h"
#include "graphics/graphics.h"
#include "menu/menu.h"
#include "scene/falling_rocks.h"
#include "scene/magma_rock.h"
#include "scene/scene.h"
#include "sound/sound.h"
#include "system/field_effect.h"
#include "system/frame.h"
#include "system/input.h"
#include "system/io_reg.h"
#include "system/iwram_call.h"
#include "system/main_routine_work.h"
#include "system/math_table.h"
#include "system/ram_buffer.h"
#include "system/system.h"
#include "system/task.h"
#include "text/msg_ids.h"

void Leader_SlideOnIce(void);
void Actor_LiftTargetBlock(u16 *actors);
void Actor_PlaceMovedBlock(void);
void Actor_DisableById(u32 id);
void Actor_SetCellShape(s32 level, s32 layer, s32 owner);

#include "../field/actor_set_sprite_priority.c"

void TundariaTower_StartEffectScript(struct FieldEffect *effect,
                                     const s32 *script);

/// The Center Prong frozen in the crystal (SceneId_TundariaTowerCenterProng),
/// an item icon (ITEM_CENTER_PRONG) until the crystal breaks.
#define ACTOR_FROZEN_CENTER_PRONG ACTOR_FIRST_PLACED
/// The Center Prong where it falls when the crystal breaks, glinting
/// (TundariaTower_SparkleCenterProng).
#define ACTOR_FALLEN_CENTER_PRONG 68
/// Reflux, the Mercury Djinni (SceneId_TundariaTowerReflux).
#define ACTOR_REFLUX ACTOR_FIRST_PLACED
/// The two pillars Lift sets on the ledge (SceneId_TundariaTowerLiftPillars),
/// the second first in the table.
#define ACTOR_SECOND_LEDGE_PILLAR (ACTOR_FIRST_PLACED + 4)
#define ACTOR_FIRST_LEDGE_PILLAR (ACTOR_FIRST_PLACED + 5)

#include "../field/field_effect_move.c"
#define EFFECT_SPAWN FieldEffect_Spawn
#define EFFECT_CREATE Import_ActorCreate
#define EFFECT_SET_MODE Actor_SelectAnimation
#define EFFECT_SET_SCRIPT TundariaTower_StartEffectScript
#define EFFECT_SCRIPTS gFieldEffect_DurationScripts
#include "../field/field_effect_spawn.c"

#undef EFFECT_SET_SCRIPT

#include "shared/tundaria_tower.h"
#include "shared/tables.h"
#include "shared/event_check_spheres_overlap.h"

#define FLAG_LEFT_WATER_PARCHED 0x942
#define FLAG_RIGHT_WATER_PARCHED 0x943
#define FLAG_CRYSTAL_BROKEN 0x944 ///< Burst broke the crystal round the prong
#define FLAG_CENTER_PRONG_TAKEN 0x945
#define FLAG_SECOND_LEDGE_PILLAR_SET 0x946
#define FLAG_FIRST_LEDGE_PILLAR_SET 0x947
#define FLAG_ICY_WALL_BURST 0x948   ///< Burst opened the dead end by the chests
#define FLAG_BROWN_WALL_BURST 0x94b ///< Burst opened the outside wall
#define FLAG_REFLUX_APPEARED FLAG_SCENE_TEMP_00
#define FLAG_FIGHTING_REFLUX FLAG_SCENE_TEMP_01
/// The first of four: Pound flattened ice column n.
#define FLAG_ICE_COLUMN_POUNDED FLAG_SCENE_TEMP_00
#define FLAG_ICY_WALL_REVEALED FLAG_SCENE_TEMP_12

void TundariaTower_AdvanceIceShard(union FieldObject *object);
void TundariaTower_ScatterIceShards(struct Vector3 *position, s32 count,
                                    FIXED scale);
void TundariaTower_UpdateFallingRocks(void);
void TundariaTower_StartFallingRocks(void);
void TundariaTower_EndFallingRocks(void);
void TundariaTower_UpdateLedgeCell(void);
void TundariaTower_SparkleCenterProng(struct Actor *object);
extern s16 gTundariaTowerLiftPillarsPillars[];
extern s16 gTundariaTowerBurstBroochPillar[];
extern struct Trigger gTundariaTowerEntranceTriggers[];
extern struct Trigger gTundariaTowerCenterProngTriggers[];
extern struct Trigger gTundariaTowerIceSlideTriggers[];
extern struct Trigger gTundariaTowerRefluxTriggers[];
extern struct Trigger gTundariaTowerIceColumnsTriggers[];
extern struct Trigger gTundariaTowerLiftPillarsTriggers[];
extern struct Trigger gTundariaTowerBurstBroochTriggers[];

/// Talking to Reflux starts its battle.
void TundariaTower_FightReflux(void) {
  GameFlag_Set(FLAG_FIGHTING_REFLUX);
  Event_RequestDjinnBattle(ACTOR_REFLUX, DJINNI_REFLUX);
}

#include "../field/actor_wait_until_idle.c"

#include "../field/map_cell_get.c"

extern u8 gMapGridCellBuffer[];

/// Replaces a cell's collision code and attributes, returning the old value.
s32 MapCell_SetAttributes(u32 layer, s32 x, s32 z, s32 value) {
  struct MapWork *map = Ram_HeapSlots->mapGrid;
  u8 *cell;
  s32 old;

  if (map == NULL)
    return 0;
  if (layer <= 2)
    cell = (u8 *)map->layers[layer].cells;
  else
    cell = gMapGridCellBuffer;
  cell += (x + (z << 7)) << 2;
  old = (cell[2] << 8) | cell[3];
  cell[2] = value >> 8;
  cell[3] = value;
  return old;
}

void MapGrid_SetCellHeight(s32 x, s32 z, s32 terrain, s32 height);

/// Sets the cell under the object by the flag its action names: with the flag
/// set, bit 7 of the layer 0 attribute and the high byte of the layer 2
/// attribute are cleared and the sprite goes to priority 3; without it they are
/// set and the sprite goes to priority 2. MapGrid_SetCellHeight then gets the
/// cell, the object's terrain and the terrain height in half cells, 4 lower or
/// higher.
void TundariaTower_SetColumnCell(struct Actor *object) {
  s32 x = FIELD_CELL(object->x.fixed);
  s32 z = FIELD_CELL(object->z.fixed);
  s32 attr0 = MapCell_GetAttributes(0, x, z);
  s32 attr2 = MapCell_GetAttributes(2, x, z);
  s32 height =
      Map_GetTerrainHeight(object->layer, object->x.fixed, object->z.fixed) >>
      19;

  if (GameFlag_Get(object->timer)) {
    attr2 &= 0xffff00ff;
    attr0 &= ~128;
    if ((object->groundHeight >> 19) != height)
      height -= 4;
    Scene_SetActorSpritePriority(object, 3);
  } else {
    attr2 = (attr2 & 0xff) | 0xff00;
    attr0 |= 128;
    height += 4;
    Scene_SetActorSpritePriority(object, 2);
  }
  MapCell_SetAttributes(0, x, z, attr0);
  MapCell_SetAttributes(2, x, z, attr2);
  MapGrid_SetCellHeight(x << 20, z << 20, object->layer, height);
}

void TundariaTower_SetUpIceColumns(u16 *table) {
  u16 *row;
  struct FieldActor *object;
  s16 flag;
  s16 animation;

  if (table[0] == 0xffff)
    return;
  row = table;
loop:
  object = ((struct FieldActor *)Actor_GetById(row[0]));
  animation = row[1];
  flag = row[2];

  Actor_SetSpriteFlags(object, 0);
  Actor_SelectAnimation(object, (u16)animation + GameFlag_Get((u16)flag));
  object->motionFlags = 0;
  object->collisionFlags &= ~ACTOR_SOLID;
  object->timer = flag;
  TundariaTower_SetColumnCell(object);
  row += 3;
  if (row[0] != 0xffff)
    goto loop;
}

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

extern u32 gTundariaTowerLiftPillarsExitRegions[];
extern u32 gTundariaTowerBurstBroochExitRegions[];

const struct ActorPlacement *Scene_GetExitRegions(void) {
  if (gMainRoutineWork.next == SCENE_TUNDARIA_TOWER_LIFT_PILLARS)
    return (const struct ActorPlacement *)gTundariaTowerLiftPillarsExitRegions;
  if (gMainRoutineWork.next == SCENE_TUNDARIA_TOWER_BURST_BROOCH)
    return (const struct ActorPlacement *)gTundariaTowerBurstBroochExitRegions;
  return 0;
}

const u32 *Scene_GetExits(void) { return gSceneExits; }

extern struct ActorPlacement gTundariaTowerCenterProngActorPlacements[];
extern struct ActorPlacement gTundariaTowerIceSlideActorPlacements[];
extern struct ActorPlacement gTundariaTowerRefluxActorPlacements[];
extern struct ActorPlacement gTundariaTowerIceColumnsActorPlacements[];
extern struct ActorPlacement gTundariaTowerLiftPillarsActorPlacements[];
extern struct ActorPlacement gTundariaTowerBurstBroochActorPlacements[];

const struct Trigger *Scene_GetActorPlacements(void) {
  if (gMainRoutineWork.next == SCENE_TUNDARIA_TOWER_CENTER_PRONG)
    return (const struct Trigger *)gTundariaTowerCenterProngActorPlacements;
  if (gMainRoutineWork.next == SCENE_TUNDARIA_TOWER_ICE_SLIDE)
    return (const struct Trigger *)gTundariaTowerIceSlideActorPlacements;
  if (gMainRoutineWork.next == SCENE_TUNDARIA_TOWER_REFLUX)
    return (const struct Trigger *)gTundariaTowerRefluxActorPlacements;
  if (gMainRoutineWork.next == SCENE_TUNDARIA_TOWER_ICE_COLUMNS)
    return (const struct Trigger *)gTundariaTowerIceColumnsActorPlacements;
  if (gMainRoutineWork.next == SCENE_TUNDARIA_TOWER_LIFT_PILLARS)
    return (const struct Trigger *)gTundariaTowerLiftPillarsActorPlacements;
  if (gMainRoutineWork.next == SCENE_TUNDARIA_TOWER_BURST_BROOCH)
    return (const struct Trigger *)gTundariaTowerBurstBroochActorPlacements;
  return (const struct Trigger *)gTundariaTowerCenterProngActorPlacements;
}

/// Parch dries the water left of the first hall's doors.
void TundariaTower_ParchLeftWater(void) {
  Event_Start();
  MapGrid_CopyCellAttributes(19, 55, 7, 8, 24, 46);
  MapGrid_CopyCellsTo(24, 102, 24, 110, 7, 8);
  GameFlag_Set(FLAG_LEFT_WATER_PARCHED);
  Event_End();
}

/// Parch dries the water right of the doors.
void TundariaTower_ParchRightWater(void) {
  Event_Start();
  MapGrid_CopyCellAttributes(37, 55, 7, 8, 32, 46);
  MapGrid_CopyCellsTo(32, 102, 32, 110, 7, 8);
  GameFlag_Set(FLAG_RIGHT_WATER_PARCHED);
  Event_End();
}

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

/// A door opens and the leader walks through it.
void TundariaTower_OpenDoor(void) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  s16 *choice;
  s32 x = 37;
  struct MainRoutineWork *state;
  s32 one;
  s32 cell;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  choice = &work->stepTrigger;
  if (*choice == 3)
    x = 25;
  Event_PlaySound(SE_DOOR_OPEN);
  one = 1;
  MapGrid_CopyCellsTo(30, 38, x, 49, one, 2);
  FieldEvent_Wait(10);
  MapGrid_CopyCellsTo(32, 38, x, 49, one, 2);
  FieldEvent_Wait(10);
  state = &gMainRoutineWork;
  Actor_SetSpeed(state->leaderActorId, 0.5);
  Actor_GetById(state->leaderActorId)->motionFlags = 0;
  cell = (Actor_GetById(state->leaderActorId)->x.fixed >> 19) | one;
  Actor_WalkToAndWait(state->leaderActorId, cell << 3,
                      Actor_GetById(state->leaderActorId)->z.part[0].integer |
                          one);
  Event_PlaySound(SE_MAP_EXIT);
  cell = Actor_GetById(state->leaderActorId)->x.fixed;
  Actor_WalkTo(state->leaderActorId, (cell >> 19) << 3,
               (Actor_GetById(state->leaderActorId)->z.part[0].integer | one) -
                   16);
  FieldEvent_Wait(5);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Actor_WaitForMoveById(state->leaderActorId);
  Import_EventSetExitRequest(*choice);
  Event_End();
}

void TundariaTower_AdvanceIceShard(union FieldObject *object) {
  object->effect.x += object->effect.velocity.x;
  object->effect.y += object->effect.velocity.y;
  object->effect.z += object->effect.velocity.z;
  object->effect.scaleX += object->effect.scaleRateX;
  object->effect.scaleY += object->effect.scaleRateY;
  object->effect.velocity.y -= toFIXED(3) / 10;
}

/// Scatters shards of ice (effect 279) around a position, each turned at
/// random and thrown off to fall under TundariaTower_AdvanceIceShard.
void TundariaTower_ScatterIceShards(struct Vector3 *position, s32 count,
                                    FIXED scale) {
  struct EffectOptions options;
  s32 i;
  s32 velocityX;
  s32 velocityY;
  FIXED x;

  options.priority = 1;
  options.type = SPRITE_BLUE_CRYSTAL;
  options.targetScaleY = FIXED_ONE * 8 / 10;
  options.targetScaleX = FIXED_ONE * 8 / 10;
  options.startScaleY = scale;
  options.startScaleX = scale;
  options.update = TundariaTower_AdvanceIceShard;
  for (i = 0; i != count; i++) {
    s32 mask;

    options.rotation = (Random16() & 15) << 12;
    velocityX = Random16();
    mask = 15;
    velocityX &= mask;
    velocityX -= 8;
    velocityX <<= 14;
    velocityY = Random16() & mask;
    velocityY += 8;
    velocityY <<= 14;
    x = position->x - ((Random16() & 31) << 16) + 0xf0000;
    FieldEffect_Spawn(x, position->y + ((Random16() & mask) << 16), position->z,
                      velocityX, velocityY, 0,
                      EFFECT_USE_UPDATE | EFFECT_USE_ROTATION |
                          EFFECT_USE_TYPE | EFFECT_USE_START_SCALE |
                          EFFECT_SCALE_TO_TARGET | EFFECT_USE_PRIORITY,
                      &options);
  }
}

/// Burst on the crystal: it cracks and throws off shards of ice.
void TundariaTower_BurstCrystal(s32 step) {
  struct Vector3 position;

  switch (step) {
  case 1:
    break;
  case 2:
    position.x = toFIXED(347);
    position.y = 0;
    position.z = toFIXED(376);
    TundariaTower_ScatterIceShards(&position, 1, FIXED_ONE * 8 / 10);
    break;
  case 3:
    MapGrid_CopyCellPictureIndicesRect(31, 85, 20, 85, 3, 4);
    position.x = toFIXED(347);
    position.y = 0;
    position.z = toFIXED(376);
    TundariaTower_ScatterIceShards(&position, 8, FIXED_ONE * 3 / 2);
    break;
  }
}

void TundariaTower_SparkleCenterProng(struct Actor *object) {
  struct EffectOptions params;
  u32 phase = gFrameCount & 7;
  s32 dx;
  s32 dy;

  if (phase == 0) {
    {
      // The mask, named where it is declared, is built before the draw.
      s32 mask = 15;

      dx = mask & Random16();
    }
    dy = Random16() & 7;
    params.type = SPRITE_SPARKLE;
    FieldEffect_Spawn(object->x.fixed + (INTtoFIXED(dx - 8)),
                      object->y.fixed + (INTtoFIXED(dy - 8)), object->z.fixed,
                      0, 0x8000, phase, 0x100000, &params);
  }
}

/// The crystal breaks and the Center Prong drops to the floor, glinting.
void TundariaTower_FreeCenterProng(void) {
  struct Actor *object = Actor_GetById(ACTOR_FROZEN_CENTER_PRONG);

  Event_Start();
  Event_ResolvePendingActions(0);
  GameFlag_Set(FLAG_CRYSTAL_BROKEN);
  MapGrid_CopyCellAttributes(11, 22, 3, 2, 20, 22);
  object->gravity = toFIXED(1) / 2;
  object->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  OverlayObject_WaitUntilIdle(object);
  Actor_SetPosition(ACTOR_FROZEN_CENTER_PRONG, 0, 0);
  Actor_SetPosition(ACTOR_FALLEN_CENTER_PRONG, 344, 372);
  Actor_GetById(ACTOR_FALLEN_CENTER_PRONG)->update =
      (void (*)(void))TundariaTower_SparkleCenterProng;
  Event_End();
}

/// "There's something..." frozen in the crystal.
void TundariaTower_ExamineCrystal(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  FieldMessage_Show(MsgThereSSomething, 1);
  if (GameFlag_Get(0x949)) {
    struct FieldWork *work = Ram_HeapSlots->fieldWork;

    work->fieldMenu = 1;
  }
  Event_End();
}

void TundariaTower_SlideOnIce(void) { Leader_SlideOnIce(); }

extern u32 gTundariaTower_RefluxHops[];

/// Reflux appears in a flash and a splash, hops down before the party and
/// waits.
void TundariaTower_RefluxAppears(void) {
  struct Actor *object = Actor_GetById(ACTOR_REFLUX);

  Event_Start();
  Event_ResolvePendingActions(0);
  TundariaTower_FadeFrom(FIXED_ONE, 0);
  PaletteFade_SetTarget(RGB_WHITE, 0);
  PaletteFade_StartInterpolation(2);
  Sound_Play(SE_WATER);
  MapGrid_SetShake(5, 7, 1);
  MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
  Event_Wait(2);
  PaletteFade_SetTarget(FIXED_ONE, 0);
  PaletteFade_StartInterpolation(2);
  object->steeringFlags = 0;
  object->update = 0;
  Actor_SetDestination(ACTOR_REFLUX, 424, 536);
  TundariaTower_StartEffectScript(object, gTundariaTower_RefluxHops);
  Actor_WaitForMoveById(ACTOR_REFLUX);
  GameFlag_Set(FLAG_REFLUX_APPEARED);
  Actor_WatchAndSetScript(ACTOR_REFLUX, gMainRoutineWork.leaderActorId);
  MapCell_SetAttributes(0, 21, 33, 0x3200);
  MapCell_SetAttributes(0, 22, 33, 0x3200);
  MapCell_SetAttributes(0, 26, 33, 0xff00);
  Event_End();
}

void TundariaTower_ShrinkIceColumn(s32 phase, s32 step) {
  struct EffectOptions options;
  s16 *effectWork = Ram_HeapSlots->fieldEffectWork;
  struct Actor *object = Actor_GetById(effectWork[13]);
  s32 velocityX;

  if (step < 8) {
    options.type = SPRITE_BLUE_CRYSTAL;
    options.startScaleY = FIXED_ONE;
    options.startScaleX = FIXED_ONE;
    options.priority = 0;
    options.targetScaleY = FIXED_ONE / 2;
    options.targetScaleX = FIXED_ONE / 2;
    options.rotation = (Random16() & 15) << 12;
    velocityX = ((Random16() & 15) - 8) << 13;
    FieldEffect_Spawn(object->x.fixed,
                      object->y.fixed - INTtoFIXED(step * 3) + toFIXED(16),
                      object->z.fixed, velocityX, 0, 0,
                      EFFECT_USE_ROTATION | EFFECT_USE_TYPE |
                          EFFECT_USE_START_SCALE | EFFECT_USE_PRIORITY,
                      &options);
    object->scaleY -= FIXED_ONE * 31 / 256;
  }
  if (phase == 1) {
    object->scaleY = FIXED_ONE;
    GameFlag_Set(object->timer);
    TundariaTower_SetColumnCell(Actor_GetById(effectWork[13]));
    MapGrid_CopyCellPictureIndicesRect(67, 23,
                                       (FIELD_CELL(object->x.fixed)) + 64,
                                       FIELD_CELL(object->z.fixed), 1, 1);
    object->z.fixed = 0;
    object->y.fixed = 0;
    object->x.fixed = 0;
  }
}

/// Pound flattens an ice column into the floor.
void TundariaTower_PoundIceColumn(s32 phase, s32 step) {
  TundariaTower_ShrinkIceColumn(phase, step);
}

/// The leader jumps onto the black ice slide and goes down it to the room
/// below.
void TundariaTower_JumpDownSlide(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.leaderActorId);

  Event_Start();
  Event_ResolvePendingActions(0);
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  Actor_SetSpriteFlags(Actor_GetById(gMainRoutineWork.leaderActorId), 0);
  leader->facing = FACING_SOUTH;
  Event_Wait(10);
  Actor_SetAnimationById(gMainRoutineWork.leaderActorId, 22);
  Event_Wait(30);
  FieldEffect_SetAttached(gMainRoutineWork.leaderActorId,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_RunRepeatedMotion(gMainRoutineWork.leaderActorId, 2);
  Event_Wait(20);
  leader->facing = FACING_NORTH;
  Actor_SetAnimationById(gMainRoutineWork.leaderActorId, 5);
  Actor_SetAnimationSpeedById(gMainRoutineWork.leaderActorId, 24);
  Event_Wait(40);
  MapGrid_SetCellHeight(0x1300000, 0x3900000, 0, -12);
  Actor_GetById(gMainRoutineWork.leaderActorId)->bounce = 0;
  Import_EventSetExitRequest(11);
  Event_End();
}

void TundariaTower_LiftPillar(void) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  struct MainRoutineWork *party = &gMainRoutineWork;

  if (party->next == SCENE_TUNDARIA_TOWER_LIFT_PILLARS) {
    struct Actor *object;

    Actor_LiftTargetBlock(gTundariaTowerLiftPillarsPillars);
    object = *(struct Actor **)((u8 *)work + 0xcbc);
    object->timer = ((FIELD_CELL(object->x.fixed)) << 8) |
                    ((FIELD_CELL(object->z.fixed)) & 0xff);
  } else if (party->next == SCENE_TUNDARIA_TOWER_BURST_BROOCH) {
    Actor_LiftTargetBlock(gTundariaTowerBurstBroochPillar);
  }
}

/// Move sends a pillar sliding on over the ice, a cell at a time, until a cell
/// that is not ice stops it.
void TundariaTower_SlidePillar(s32 unused, s32 actorArg) {
  u8 *work = Ram_HeapSlots->fieldWork;
  struct Actor *object;
  u16 actor;
  s32 dx;
  s32 dz;

  actor = actorArg;
  object = *(struct Actor **)(work + 0xcbc);
  Event_Start();
  Event_ResolvePendingActions(0);
  dx = (FIELD_CELL(object->x.fixed)) - (object->timer >> 8);
  dz = (FIELD_CELL(object->z.fixed)) - (object->timer & 0xff);
  object->speedLimit = toFIXED(1) / 2;
  while ((MapCell_GetAttributes(1, (FIELD_CELL(object->x.fixed)) + dx,
                                (FIELD_CELL(object->z.fixed)) + dz) >>
          8) == 50 &&
         (MapCell_GetAttributes(2, (FIELD_CELL(object->x.fixed)) + dx,
                                (FIELD_CELL(object->z.fixed)) + dz) >>
          8) != 255) {
    Actor_SetMoveTarget(object, INTtoFIXED(dx * 16) + object->x.fixed,
                        object->y.fixed, INTtoFIXED(dz * 16) + object->z.fixed);
    FieldCamera_FollowActor(actor, 1);
    Actor_WaitForMove(object);
    wait(1);
  }
  object->x.fixed = ((object->x.fixed >> 19) | 1) << 19;
  object->z.fixed = ((object->z.fixed >> 19) | 1) << 19;
  Actor_PlaceMovedBlock();
  Event_End();
}

/// A pushed pillar off the edge of its ledge drops to the floor below; in the
/// room of pillars the two set against the ledge open its climb.
void TundariaTower_SetDownPillar(void) {
  u8 *event = Ram_HeapSlots->fieldWork;
  struct Actor *object;

  object = *(struct Actor **)(event + 0xcbc);

  Event_Start();
  Event_ResolvePendingActions(0);
  if (Map_GetTerrainHeight(2, object->x.fixed, object->z.fixed) !=
      object->y.fixed) {
    object->layer = 2;
    object->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
    OverlayObject_WaitUntilIdle(object);
    Sound_Play(SE_THUMP);
    OverlayObject_WaitUntilIdle(object);
    object->motionFlags = 0;
    object->layer = 0;
  }
  Actor_PlaceMovedBlock();
  if (gMainRoutineWork.next == SCENE_TUNDARIA_TOWER_LIFT_PILLARS) {
    if (object->x.fixed >> 19 == 27)
      GameFlag_Set(FLAG_FIRST_LEDGE_PILLAR_SET);
    if (object->x.fixed >> 19 == 25 &&
        GameFlag_Get(FLAG_FIRST_LEDGE_PILLAR_SET))
      GameFlag_Set(FLAG_SECOND_LEDGE_PILLAR_SET);
  }
  Event_End();
}

void TundariaTower_EndRevealIcyWall(void) {
  BlendScript_Resume();
  GameFlag_Clear(FLAG_ICY_WALL_REVEALED);
  if ((u16)(gMainRoutineWork.entrance - 6) <= 1) {
    MapGrid_CopyCellPictureIndicesRect(39, 100, 53, 100, 3, 3);
    MapGrid_CopyCellAttributes(43, 41, 3, 3, 53, 37);
    if (FIELD_CELL(Actor_GetById(gMainRoutineWork.leaderActorId)->z.fixed) ==
        38) {
      s16 angle = Actor_GetById(gMainRoutineWork.leaderActorId)->facing;
      s32 owner = gMainRoutineWork.leaderActorId;

      if (angle < 0)
        Actor_SetPosition(owner, 872, 600);
      else
        Actor_SetPosition(owner, 872, 632);
    }
  }
}

/// Reveal shows the way through the icy wall at the dead end.
void TundariaTower_RevealIcyWall(void) {
  BlendScript_Suspend();
  GameFlag_Set(FLAG_ICY_WALL_REVEALED);
  if ((u16)(gMainRoutineWork.entrance - 6) <= 1) {
    MapGrid_CopyCellPictureIndicesRect(43, 100, 53, 100, 3, 3);
    MapGrid_CopyCellAttributes(43, 37, 3, 3, 53, 37);
    if (FIELD_CELL(Actor_GetById(gMainRoutineWork.leaderActorId)->z.fixed) > 38)
      MapCell_SetAttributes(0, 54, 37, 0x1f00);
    else
      MapCell_SetAttributes(0, 54, 39, 0x1f00);
  }
}

void TundariaTower_StopSliding(void) { Event_ResolvePendingActions(0); }

/// Burst on the brown part of the outside wall opens a short cut.
void TundariaTower_BurstBrownWall(s32 step) {
  struct Vector3 position;

  switch (step) {
  case 1:
    MapGrid_CopyCellPictureIndicesRect(18, 97, 6, 97, 3, 4);
    MapGrid_CopyCellPictureIndicesRect(82, 34, 70, 34, 3, 2);
    MapGrid_CopyCellAttributes(7, 36, 1, 1, 7, 35);
    GameFlag_Set(FLAG_BROWN_WALL_BURST);
    break;
  case 2:
    position.x = toFIXED(120);
    position.y = 0;
    position.z = toFIXED(568);
    TundariaTower_ScatterIceShards(&position, 1, FIXED_ONE * 8 / 10);
    break;
  case 3:
    position.x = toFIXED(120);
    position.y = 0;
    position.z = toFIXED(568);
    TundariaTower_ScatterIceShards(&position, 8, FIXED_ONE * 3 / 2);
    break;
  }
}

void TundariaTower_ResolveBeforePsynergy(void) {
  Event_ResolvePendingActions(0);
}

/// Burst on the dead end by the chests brings its rocks down and opens it.
void TundariaTower_BurstIcyWall(s32 mode, s32 cue) {
  if (cue == 0) {
    TundariaTower_StartFallingRocks();
    Task_Remove(TundariaTower_UpdateLedgeCell);
  }
  if (mode == 1) {
    MapGrid_CopyCellPictureIndicesRect(13, 26, 13, 16, 3, 3);
    MapGrid_CopyCellAttributes(13, 26, 3, 3, 13, 16);
    GameFlag_Set(FLAG_ICY_WALL_BURST);
  }
  if (mode == 2)
    Rocks_Spawn(0xe80000, 0x100000, 0x1180000, 2);
  if (mode == 3)
    Rocks_Spawn(0xe80000, 0x100000, 0x1180000, 30);
  if (cue == 0x3e7)
    TundariaTower_EndFallingRocks();
}

const struct Trigger *Scene_GetTriggers(void) {
  if (gMainRoutineWork.next == SCENE_TUNDARIA_TOWER_ENTRANCE)
    return gTundariaTowerEntranceTriggers;
  if (gMainRoutineWork.next == SCENE_TUNDARIA_TOWER_CENTER_PRONG)
    return gTundariaTowerCenterProngTriggers;
  if (gMainRoutineWork.next == SCENE_TUNDARIA_TOWER_ICE_SLIDE)
    return gTundariaTowerIceSlideTriggers;
  if (gMainRoutineWork.next == SCENE_TUNDARIA_TOWER_REFLUX)
    return gTundariaTowerRefluxTriggers;
  if (gMainRoutineWork.next == SCENE_TUNDARIA_TOWER_ICE_COLUMNS)
    return gTundariaTowerIceColumnsTriggers;
  if (gMainRoutineWork.next == SCENE_TUNDARIA_TOWER_LIFT_PILLARS)
    return gTundariaTowerLiftPillarsTriggers;
  if (gMainRoutineWork.next == SCENE_TUNDARIA_TOWER_BURST_BROOCH)
    return gTundariaTowerBurstBroochTriggers;
  return gTundariaTowerCenterProngTriggers;
}

/// The leader's reflection in the ice, mirrored across the room's line.
void TundariaTower_ReflectLeader(struct Actor *object) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  struct ReflectionAnimation *leaderAnimation = leader->sprite;
  struct ReflectionAnimation *animation = object->sprite;

  if (leader->z.fixed <= object->riseBaseY) {
    object->z.fixed = 0;
    object->x.fixed = 0;
    REG_BG2CNT &= 0xfffc;
    REG_BG2CNT |= 1;
  } else {
    REG_BG2CNT &= 0xfffc;
    REG_BG2CNT |= 2;
    if (GameFlag_Get(FLAG_ICY_WALL_REVEALED)) {
      object->z.fixed = 0;
      object->x.fixed = 0;
    } else {
      struct ReflectionSprite *sprite = animation->sprite;
      struct ReflectionSprite *source;

      object->x.fixed = leader->x.fixed;
      object->y.fixed = leader->y.fixed;
      object->z.fixed =
          object->riseBaseY - (leader->z.fixed - object->riseBaseY);
      object->facing = ~leader->facing;
      source = leaderAnimation->sprite;
      sprite->unk10 = source->unk10;
      sprite->unk02 = source->unk02;
      sprite->unk14 = source->unk14;
    }
  }
}
#undef FIELD_AT

#include "../field/field_effect_advance_motion.c"

struct Actor;
struct Particle;
struct ParticleSprite;
union FieldObject;

void Leader_StopIfPending(void);

void Actor_SetCellOpen(s32 id, u32 kind, s32 flag, s32 layer);

s32 MapGrid_GetTerrainKind(s32 layer, s32 x, s32 z);

void TundariaTower_SpawnRubble(s32 spread);
void TundariaTower_SetLeaderCellShape(s32 a, s32 b);

void TundariaTower_SpawnDustRing(void);

void Actor_MoveToObject(s32 id, s32 target);

union FieldObject *gTundariaTower_Reflection BSS_DATA;
volatile s16 gSlideOnIce_Stopped BSS_DATA;

void TundariaTower_ParchLeftWater(void);
void TundariaTower_ParchRightWater(void);

extern s16 gSlideOnIce_SteerAngles[];
extern s16 gSlideOnIce_AnglesByKey[];

extern u32 gTundariaTower_RubbleScript[];

extern u32 gTundariaTower_DustRingScript[];

void EffectSprite_DrawAtWorldPosition(struct ParticleSprite *sprite,
                                      struct Particle *particle);
void BattleEffect_StepVector3(struct Particle *particle, s32 damping,
                              s32 gravity);

void TundariaTower_ExamineCrystal(void);
void TundariaTower_SlideOnIce(void);
void TundariaTower_RefluxAppears(void);
void TundariaTower_PoundIceColumn(s32 phase, s32 step);
void TundariaTower_SetDownPillar(void);
void TundariaTower_EndRevealIcyWall(void);
void TundariaTower_RevealIcyWall(void);
void TundariaTower_StopSliding(void);
void TundariaTower_ResolveBeforePsynergy(void);

void FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                       s32 velocityZ, u32 flags,
                       const struct EffectOptions *options);

/// The party drops in from the slide above and lands in a puff of dust.
void TundariaTower_LandFromSlide(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  struct EffectOptions options;
  s32 angle;
  u32 i;

  Event_Start();
  Event_ResolvePendingActions(0);
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  MapGrid_CenterOnFollowedPoint();
  wait(1);
  leader->y.fixed = toFIXED(130);
  leader->gravity = toFIXED(1) / 2;
  leader->bounce = 0;
  leader->motionFlags = 0;
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Sound_Play(SE_DUST_PUFF);
  leader->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  Event_Wait(24);
  options.palette = 7;
  options.update = (void *)FieldEffect_AdvanceMotion;
  options.startScaleX = FIXED_ONE * 8 / 10;
  options.startScaleY = FIXED_ONE * 8 / 10;
  i = 0;
  do {
    FIXED velocity[3];

    angle = i << 12;
    velocity[0] = Trig_Cos(angle);
    velocity[1] = 0;
    velocity[2] = Trig_Sin(angle);
    velocity[0] += velocity[0] >> 1;
    velocity[0] += ((Random16() * 3) << 14 >> 16) - 0x6000;
    velocity[2] += ((Random16() * 3) << 13 >> 16) - 0x3000;
    FieldEffect_Spawn(leader->x.fixed, leader->y.fixed, leader->z.fixed,
                      velocity[0], velocity[1], velocity[2],
                      EFFECT_USE_UPDATE | EFFECT_USE_START_SCALE |
                          EFFECT_USE_PALETTE | 1,
                      &options);
    i++;
  } while (i <= 16);
  Sound_Play(SE_THUMP);
  FieldEffect_SetAttached(gMainRoutineWork.leaderActorId,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ATTACH);
  Actor_SetAnimationById(gMainRoutineWork.leaderActorId, 22);
  MapGrid_SetShake(5, 5, 1);
  MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
  MapGrid_WaitForShakeToSettle();
  FieldEffect_SetAttached(gMainRoutineWork.leaderActorId,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_REMOVE);
  leader->gravity = toFIXED(1);
  leader->bounce = FIXED_ONE / 4;
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
  Event_End();
}

void TundariaTower_UpdateLedgeCell(void) {
  if ((Actor_GetById(gMainRoutineWork.leaderActorId)->y.fixed >> 19) != 0)
    MapCell_SetAttributes(0, 14, 17, 0);
  else
    MapCell_SetAttributes(0, 14, 17, 0x3200);
}

extern s16 gTundariaTowerIceColumnsColumns[];
extern s16 gTundariaTowerLiftPillarsColumns[];
void Actor_PlaceBlocks(const s16 *data);

s32 Scene_Initialize(void) {
  struct Actor *object;
  s32 scene;
  s32 flag;

  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
  scene = gMainRoutineWork.next;
  if (scene == SCENE_TUNDARIA_TOWER_ENTRANCE) {
  } else if (scene == SCENE_TUNDARIA_TOWER_CENTER_PRONG) {
    switch (gMainRoutineWork.entrance) {
    case 1:
    case 2:
    case 3:
    case 4:
      if (GameFlag_Get(FLAG_LEFT_WATER_PARCHED))
        TundariaTower_ParchLeftWater();
      if (GameFlag_Get(FLAG_RIGHT_WATER_PARCHED))
        TundariaTower_ParchRightWater();
      break;
    case 11:
      flag = GameFlag_Get(FLAG_CENTER_PRONG_TAKEN);
      if (flag == 0) {
        object = Actor_GetById(ACTOR_FROZEN_CENTER_PRONG);
        Actor_SetSpriteFlags(Actor_GetById(ACTOR_FROZEN_CENTER_PRONG), 0);
        object->collisionFlags = 0;
        object->motionFlags = 0;
        object->groundHeight = toFIXED(8);
        object->y.fixed = toFIXED(8);
        Actor_SetItemIcon(object, ITEM_CENTER_PRONG);
        if (GameFlag_Get(FLAG_CRYSTAL_BROKEN)) {
          MapGrid_CopyCellsTo(31, 85, 20, 85, 3, 4);
          MapGrid_CopyCellAttributes(11, 22, 3, 2, 20, 22);
          Actor_SetPosition(ACTOR_FALLEN_CENTER_PRONG, 344, 372);
          Actor_GetById(ACTOR_FALLEN_CENTER_PRONG)->update =
              (void (*)(void))TundariaTower_SparkleCenterProng;
          Actor_SetPosition(ACTOR_FROZEN_CENTER_PRONG, 0, 0);
        }
      } else if (GameFlag_Get(FLAG_CRYSTAL_BROKEN)) {
        MapGrid_CopyCellsTo(31, 85, 20, 85, 3, 4);
        MapGrid_CopyCellAttributes(11, 22, 3, 2, 20, 22);
        Actor_SetPosition(ACTOR_FROZEN_CENTER_PRONG, 0, 0);
      }
      break;
    }
  } else if (scene == SCENE_TUNDARIA_TOWER_REFLUX) {
    switch (gMainRoutineWork.entrance) {
    case 6:
    case 7:
      if (!GameFlag_Get(FLAG_DJINNI(DJINNI_REFLUX))) {
        if (!GameFlag_Get(FLAG_REFLUX_APPEARED)) {
          Actor_SetPosition(ACTOR_REFLUX, 344, 536);
          MapCell_SetAttributes(0, 21, 33, 0x3300);
          MapCell_SetAttributes(0, 22, 33, 0xff00);
        } else if (!GameFlag_Get(FLAG_FIGHTING_REFLUX)) {
          MapCell_SetAttributes(0, 26, 33, 0xff00);
        }
        Actor_WatchAndSetScript(ACTOR_REFLUX, gMainRoutineWork.leaderActorId);
      }
      if (!GameFlag_Get(FLAG_KEEP_PARTY_POSITION) &&
          gMainRoutineWork.entrance == 7)
        TundariaTower_LandFromSlide();
      break;
    }
  } else if (scene == SCENE_TUNDARIA_TOWER_ICE_COLUMNS) {
    TundariaTower_SetUpIceColumns(gTundariaTowerIceColumnsColumns);
  } else if (scene == SCENE_TUNDARIA_TOWER_LIFT_PILLARS) {
    switch (gMainRoutineWork.entrance) {
    case 3:
      object = FieldActor_Create(SPRITE_CRATE, toFIXED(472), 0, toFIXED(200));
      if (object != NULL) {
        Scene_SetActorSpritePriority((s32)object, 1);
        object->motionFlags = 0;
        Actor_SetSpriteFlags(object, 0);
        Actor_SetSpriteColor(object, 14);
        Actor_SetBlendMode(object, 1);
      }
      break;
    case 1:
    case 2:
      TundariaTower_SetUpIceColumns(gTundariaTowerLiftPillarsColumns);
      if (GameFlag_Get(FLAG_FIRST_LEDGE_PILLAR_SET)) {
        Actor_SetPosition(ACTOR_FIRST_LEDGE_PILLAR, 216, 136);
        if (GameFlag_Get(FLAG_SECOND_LEDGE_PILLAR_SET))
          Actor_SetPosition(ACTOR_SECOND_LEDGE_PILLAR, 200, 136);
      }
      Actor_PlaceBlocks(gTundariaTowerLiftPillarsPillars);
      Actor_SetSpritePriority(ACTOR_SECOND_LEDGE_PILLAR, 2);
      break;
    case 4:
    case 5:
      if (GameFlag_Get(FLAG_BROWN_WALL_BURST)) {
        MapGrid_CopyCellsTo(18, 97, 6, 97, 3, 4);
        MapGrid_CopyCellsTo(82, 34, 70, 34, 3, 2);
        MapGrid_CopyCellAttributes(7, 36, 1, 1, 7, 35);
      }
      {
        union FieldObject **slot = &gTundariaTower_Reflection;
        struct Actor *effect = ((struct Actor *)Import_ActorCreate(4, 0, 0, 0));

        effect->riseBaseY = toFIXED(576);
        effect->update = TundariaTower_ReflectLeader;
        *slot = (union FieldObject *)effect;
        effect->motionFlags = 0;
        Scene_SetActorSpritePriority(effect, 3);
      }
      break;
    case 6:
    case 7: {
      union FieldObject **slot = &gTundariaTower_Reflection;
      struct Actor *effect = ((struct Actor *)Import_ActorCreate(4, 0, 0, 0));

      effect->riseBaseY = toFIXED(624);
      effect->update = TundariaTower_ReflectLeader;
      *slot = (union FieldObject *)effect;
      effect->motionFlags = 0;
      Scene_SetActorSpritePriority(effect, 3);
    } break;
    case 10:
      if (!GameFlag_Get(FLAG_KEEP_PARTY_POSITION))
        TundariaTower_LandFromSlide();
      break;
    }
  } else if (scene == SCENE_TUNDARIA_TOWER_BURST_BROOCH) {
    GameFlag_Set(FLAG_RETREAT_BLOCKED);
    if (GameFlag_Get(FLAG_ICY_WALL_BURST)) {
      MapGrid_CopyCellsTo(13, 26, 13, 16, 3, 3);
      MapGrid_CopyCellAttributes(13, 26, 3, 3, 13, 16);
    } else {
      Task_Add(TundariaTower_UpdateLedgeCell, TASK_PASS_VBLANK);
    }
    Actor_PlaceBlocks(gTundariaTowerBurstBroochPillar);
  }
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

#include "shared/actor_steer_by_pad.h"

#include "shared/leader_stop_if_pending.h"

#include "shared/leader_slide_on_ice.h"

extern s16 gSlideOnIce_AnglesByKey[];
void Actor_SteerByPad(struct Actor *object);
void Leader_StopIfPending(void);
s32 Actor_HasArrived(struct Actor *object);

#include "shared/slide_on_ice_from_edge.c"

void Leader_SetSlideStopped(s32 value) { gSlideOnIce_Stopped = value; }

#include "shared/leader_slide_on_ice_ahead.h"

#define BYTE_AT(base, offset) (*(u8 *)((u8 *)(base) + (offset)))

UNUSED void TundariaTower_PlaceStakes(u16 *table) {
  u16 *row;
  s32 id;
  struct Actor *object;
  s16 x, z;

  if (table[0] == 0xffff)
    return;
  row = table;
loop:
  id = row[0];
  object = Actor_GetById(id);
  x = row[1];
  z = row[2];

  object->collisionFlags &= ~ACTOR_SOLID;
  Actor_SetSpriteFlags(object, 0);
  Actor_SetCellOpen(id, (u16)x, (u16)z, BYTE_AT(object, 34));
  row += 3;
  if (row[0] != 0xffff)
    goto loop;
}

/// Finds the current target's table row and shrinks it.
UNUSED void Pound_SinkTarget(u16 *list, s32 mode, s32 count) {
  struct FieldEffectWork *work = Ram_HeapSlots->fieldEffectWork;

  if (*list == 0xffff)
    return;
top:
  {
    u32 entry = *list;
    s32 id = work->secondActorId;

    if (id == entry) {
      struct Actor *object = Actor_GetById(id);
      s32 a;
      s32 flag;

      list++;
      a = list[0];
      flag = list[1];
      if (count < 8)
        object->scaleY -= FIXED_ONE * 31 / 256;
      if (mode == 1) {
        GameFlag_Set(flag);
        Actor_SetCellOpen(id, a, flag, object->layer);
        object->scaleY = FIXED_ONE;
      }
      return;
    }
  }
  list += 3;
  if (*list != 0xffff)
    goto top;
}

#include "shared/actor_set_cell_open.h"

/// On entering, sets each listed pillar from its two flags (pushed: moved to
/// its place; else its cell raised and its shape redrawn), then lifts the
/// leader onto the ground under it.
UNUSED void TundariaTower_RestoreCrumblingPillars(const u16 *table) {
  struct MapWork *map = Ram_HeapSlots->mapGrid;
  struct Actor *object;
  s32 id;
  s32 flag;
  s32 nextFlag;
  struct MapCell *cells;
  u8 *shapes;
  struct MapCell *cell;
  s32 index;
  struct Actor *leader;
  s32 ground;

  if (*table == 0xffff)
    goto done;
next:
  id = table[0];
  object = Actor_GetById(id);
  flag = table[1];
  nextFlag = flag + 1;
  cells = map->layers[object->layer].cells;
  shapes = Ram_MapShapeGrid + (cells - (struct MapCell *)Ram_MapCellBuffer);
  if (GameFlag_Get(nextFlag)) {
    Actor_SetPosition(id, 0, 0);
  } else {
    s32 shape;
    u8 *slot;
    s32 height;

    index = Actor_GetCellIndex(object);
    cell = &cells[index];
    slot = shapes + index;
    shape =
        MapGrid_GetTerrainKind(object->layer, object->x.fixed, object->z.fixed);
    height =
        Map_GetTerrainHeight(object->layer, object->x.fixed, object->z.fixed);
    Actor_DisableById(id);
    cell->attributeB = 128;
    *slot = MapCollision_FindOrAddTag(object->layer, shape, (height >> 19) + 4);
    if (GameFlag_Get(flag))
      Actor_SelectAnimation(object, 0);
  }
  table += 2;
  if (*table != 0xffff)
    goto next;
done:
  leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  ground =
      Map_GetTerrainHeight(leader->layer, leader->x.fixed, leader->z.fixed);
  if (leader->y.fixed < ground) {
    leader->groundHeight = ground;
    leader->y.fixed = ground;
  }
}

UNUSED void TundariaTower_CheckCrumblingStep(const u16 *table) {
  struct Actor *leader;
  struct Actor *trap;
  FIXED position[3];
  s32 id;
  s32 flag;
  s32 second;

  if (Field_TryJumpForward() != 0)
    return;
  leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  position[0] = leader->x.fixed;
  position[1] = leader->y.fixed;
  position[2] = leader->z.fixed;
  trap = Actor_FindAtCell(position, 0);
  if (trap != 0)
    goto test;
  return;
next:
  id = table[0];
  if (Actor_GetById(id) != trap) {
    table += 2;
    goto test;
  }
  {
    flag = table[1];
    Event_Start();
    if (!GameFlag_Get(flag)) {
      Sound_Play(SE_CHEST_CLOSE);
      Actor_SetSpriteColor(Actor_GetById(id), 7);
      wait(2);
      Actor_SelectAnimation(trap, 0);
      Actor_SetSpriteColor(Actor_GetById(id), 0);
      wait(2);
      Actor_SetSpriteColor(Actor_GetById(id), 7);
      wait(4);
      Actor_SetSpriteColor(Actor_GetById(id), 0);
      TundariaTower_SpawnRubble(0);
      GameFlag_Set(flag);
    } else {
      second = flag + 1;
      if (!GameFlag_Get(second)) {
        leader->motionFlags = 0;
        Sound_Play(SE_PUSH);
        MapGrid_SetShake(1, 4, 1);
        TundariaTower_SpawnRubble(0);
        wait(8);
        Actor_SelectAnimation(trap, 2);
        TundariaTower_SetLeaderCellShape(-2, trap->layer);
        TundariaTower_SpawnRubble(1);
        wait(16);
        TundariaTower_SetLeaderCellShape(-2, trap->layer);
        wait(4);
        MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
        wait(8);
        leader->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
        wait(5);
        Actor_SetPositionAndResetMotion(trap, 0, 0, 0);
        wait(2);
        Sound_Play(SE_THUMP);
        TundariaTower_SpawnDustRing();
        wait(20);
        GameFlag_Set(second);
      }
    }
    Event_End();
    return;
  }
test:
  if (*table != 0xffff)
    goto next;
}

UNUSED void TundariaTower_CrumblePillar(u16 *row, s32 id) {
  struct Actor *object = Actor_GetById(id);
  s32 flag;
  s32 nextFlag;

  if (*row == 0xffff)
    return;
loop:
  if (*row != id) {
    row += 2;
    goto test;
  }
  flag = row[1];
  Event_Start();
  nextFlag = flag + 1;
  if (!GameFlag_Get(nextFlag)) {
    Sound_Play(SE_PUSH);
    MapGrid_SetShake(1, 4, 1);
    TundariaTower_SpawnRubble(0);
    wait(8);
    Actor_SelectAnimation(object, 2);
    Actor_SetCellShape(-4, object->layer, id);
    TundariaTower_SpawnRubble(1);
    wait(16);
    MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
    wait(8);
    Actor_SetPositionAndResetMotion(object, 0, 0, 0);
    wait(2);
    Sound_Play(SE_THUMP);
    TundariaTower_SpawnDustRing();
    wait(20);
    GameFlag_Set(flag);
    GameFlag_Set(nextFlag);
  }
  Event_End();
  return;
test:
  if (*row != 0xffff)
    goto loop;
}

/// Keeps the pillar's sprite cached and stops it.
void TundariaTower_FreezePillar(s32 id) {
  Actor_GetById(id)->retainSpriteCache = 3;
  Actor_DisableById(id);
}

/// Places actors 8, 9 and 10, then walks a 0xffff-ended list of (actor, flag)
/// halfword pairs: flag + 1 set parks the actor at (0, 0); otherwise its cell
/// gets attributeB 128 and its shape-grid byte is rewritten, and flag set makes
/// it copy actor 9's visual attributes.
UNUSED void TundariaTower_RestorePillars(u16 *list) {
  struct MapWork *map = Ram_HeapSlots->mapGrid;
  struct Actor *object;
  s32 id;
  s32 flag;
  s32 nextFlag;
  struct MapCell *cells;
  u8 *shapes;
  struct MapCell *cell;
  s32 index;
  u8 *grid = gMapShapeGrid; // the shape grid, read once

  Actor_MoveToObject(8, gMainRoutineWork.leaderActorId);
  Actor_MoveToObject(9, gMainRoutineWork.leaderActorId);
  Actor_MoveToObject(10, gMainRoutineWork.leaderActorId);
  wait(1);
  TundariaTower_FreezePillar(8);
  TundariaTower_FreezePillar(9);
  TundariaTower_FreezePillar(10);
  Actor_SetAnimationById(9, 0);
  wait(1);
  Actor_SetPosition(8, 0, 0);
  Actor_SetPosition(9, 0, 0);
  Actor_SetPosition(10, 0, 0);
  wait(1);
  if (*list == 0xffff)
    goto done;
next:
  id = list[0];
  object = Actor_GetById(id);
  flag = list[1];
  nextFlag = flag + 1;
  cells = map->layers[object->layer].cells;
  shapes = grid + (cells - (struct MapCell *)Ram_MapCellBuffer);
  object->retainSpriteCache = 3;
  if (GameFlag_Get(nextFlag)) {
    Actor_SetPosition(id, 0, 0);
  } else {
    s32 shape;
    u8 *slot;
    s32 height;

    index = Actor_GetCellIndex(object);
    cell = &cells[index];
    slot = shapes + index;
    shape =
        MapGrid_GetTerrainKind(object->layer, object->x.fixed, object->z.fixed);
    height =
        Map_GetTerrainHeight(object->layer, object->x.fixed, object->z.fixed);
    Actor_DisableById(id);
    cell->attributeB = 128;
    *slot = MapCollision_FindOrAddTag(object->layer, shape, (height >> 19) + 4);
    if (GameFlag_Get(flag))
      Actor_CopyGraphics(id, 9);
  }
  list += 2;
  if (*list != 0xffff)
    goto next;
done:
  wait(10);
}

void TundariaTower_TakePlaceOf(struct Actor *object, struct Actor *target) {
  Actor_SetPositionAndResetMotion(object, target->x.fixed, target->y.fixed,
                                  target->z.fixed);
  Actor_SetPositionAndResetMotion(target, 0, 0, 0);
}

UNUSED void TundariaTower_StepOnTrap(const u16 *table) {
  struct Actor *leader;
  struct Actor *partner;
  struct Actor *trap;
  FIXED position[3];
  s32 id;
  s32 flag;
  s32 second;
  u8 *motion;

  if (Field_TryJumpForward() != 0)
    return;
  leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  partner = Actor_GetById(8);
  position[0] = leader->x.fixed;
  position[1] = leader->y.fixed;
  position[2] = leader->z.fixed;
  trap = Actor_FindAtCell(position, 0);
  if (trap != 0) {
    goto test;
  next:
    id = table[0];
    if (Actor_GetById(id) != trap) {
      table += 2;
      goto test;
    }
    flag = table[1];
    Event_Start();
    if (!GameFlag_Get(flag)) {
      Actor_SelectAnimation(partner, 1);
      TundariaTower_TakePlaceOf(partner, trap);
      wait(1);
      Sound_Play(SE_CHEST_CLOSE);
      Actor_SetSpriteColor(Actor_GetById(8), 7);
      wait(2);
      Actor_SelectAnimation(partner, 0);
      Actor_CopyGraphics(id, 9);
      Actor_SetSpriteColor(Actor_GetById(8), 0);
      wait(2);
      Actor_SetSpriteColor(Actor_GetById(8), 7);
      wait(4);
      Actor_SetSpriteColor(Actor_GetById(8), 0);
      TundariaTower_SpawnRubble(0);
      TundariaTower_TakePlaceOf(trap, partner);
      wait(1);
      GameFlag_Set(flag);
    } else {
      second = flag + 1;
      if (!GameFlag_Get(second)) {
        Actor_SelectAnimation(partner, 0);
        TundariaTower_TakePlaceOf(partner, trap);
        motion = &leader->motionFlags;
        wait(1);
        *motion = 0;
        Sound_Play(SE_PUSH);
        MapGrid_SetShake(1, 4, 1);
        TundariaTower_SpawnRubble(0);
        wait(8);
        Actor_SelectAnimation(partner, 2);
        TundariaTower_SetLeaderCellShape(-2, trap->layer);
        TundariaTower_SpawnRubble(1);
        wait(16);
        TundariaTower_SetLeaderCellShape(-2, trap->layer);
        wait(4);
        MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
        wait(8);
        *motion = 3;
        wait(5);
        Actor_SetPositionAndResetMotion(partner, 0, 0, 0);
        wait(2);
        Sound_Play(SE_THUMP);
        TundariaTower_SpawnDustRing();
        wait(20);
        GameFlag_Set(second);
      }
    }
    Event_End();
    return;
  test:
    if (*table != 0xffff)
      goto next;
  }
}

#include "../field/field_effect_advance_motion_priority.c"

void TundariaTower_SpawnRubble(s32 spread) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  struct FieldEffect *effect;
  u32 frame = 0;
  u32 i = 0;

  do {
    {
      // Each piece starts 0 to 13 pixels lower than the leader.
      FIXED y = leader->y.fixed - ((Random_Below(14)) << 16);

      effect = (struct FieldEffect *)Import_ActorCreate(
          SPRITE_DUST_CLOUD, (i << 17) + leader->x.fixed - toFIXED(8),
          y - toFIXED(2), leader->z.fixed);
    }
    if (effect != 0) {
      frame = Sprite_ShareVramCacheEntry(effect->sprite, frame);
      effect->motionFlags = 0;
      effect->collisionFlags = 0;
      Actor_SetSpriteFlags(effect, 0);
      Actor_SelectAnimation(effect, 2);
      TundariaTower_StartEffectScript(effect, gTundariaTower_RubbleScript);
      effect->scaleY = effect->scaleX = FIXED_ONE * 7 / 10;
      effect->sprite->priority = 1;
      if (spread != 0) {
        effect->velocity.x =
            Iwram_MulFX(Trig_Cos(i << 13), FIXED_ONE * 12 / 10);
        effect->velocity.z = Trig_Sin(i << 13);
      } else {
        effect->velocity.x = spread;
        effect->velocity.z = spread;
      }
      effect->velocity.y = -(((Random16() << 14) >> 16) + 0x6000);
      effect->spin = ((Random16() << 9) >> 16) - 0x100;
      effect->scaleRateX = -(FIXED_ONE / 32);
      effect->scaleRateY = -(FIXED_ONE * 3 / 128);
      effect->update = Effect_AdvanceMotionAtLeaderPriority;
    }
    i++;
  } while (i <= 7);
}

void TundariaTower_SpawnDustRing(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  s32 state;
  u32 i;
  s32 zero = 0;

  if (leader != NULL) {
    state = 0;
    for (i = 0; i < 17; i++) {
      struct FieldEffect *effect = ((struct FieldEffect *)Import_ActorCreate(
          SPRITE_DUST_CLOUD, leader->x.fixed, leader->y.fixed,
          leader->z.fixed));

      if (effect != NULL) {
        s32 angle;

        state = Sprite_ShareVramCacheEntry(effect->sprite, state);
        zero = 0;
        effect->motionFlags = zero;
        effect->collisionFlags = zero;
        Actor_SetSpriteFlags(effect, 0);
        Actor_SelectAnimation(effect, 2);
        TundariaTower_StartEffectScript(effect, gTundariaTower_DustRingScript);
        angle = i << 12;
        effect->velocity.x = Trig_Cos(angle);
        effect->velocity.y = zero;
        effect->velocity.z = Trig_Sin(angle);
        effect->velocity.x += effect->velocity.x >> 1;
        effect->velocity.x += ((Random16() * 3) << 14 >> 16) - 0x6000;
        effect->velocity.z += ((Random16() * 3) << 13 >> 16) - 0x3000;
        effect->spin = ((Random16() << 12) >> 16) - 0x800;
        effect->scaleRateX = zero;
        effect->scaleRateY = zero;
        effect->update = Effect_AdvanceMotionAtLeaderPriority;
        effect->sprite->priority =
            ((struct FieldSprite *)leader->sprite)->priority;
      }
    }
  }
}

#include "shared/actor_set_cell_shape.h"

void TundariaTower_SetLeaderCellShape(s32 a, s32 b) {
  Actor_SetCellShape(a, b, gMainRoutineWork.leaderActorId);
}

void TundariaTower_UpdateFallingRocks(void) {
  struct ParticleWork *work = Ram_HeapSlots->menuWork;
  struct Particle *particle = work->particles;
  struct ParticleSprite *sprite = work->sprites;
  s32 i;
  FIXED step = -(toFIXED(1) / 2);

  i = 63;
next:
  if ((u32)particle->variant <= 19) {
    u16 *base = &work->tile;
    s32 mask = 7;
    s32 tile = (u16)((*base + ((particle->variant >> 2) & mask) * 8) & 0x3ff);
    s32 attr = sprite->attr2;

    sprite->attr2 = (attr & ~0x3ff) | tile;
    EffectSprite_DrawAtWorldPosition(sprite, particle);
    BattleEffect_StepVector3(particle, 63, step);
    particle->variant++;
  }
  i--;
  sprite++;
  particle++;
  if (i >= 0)
    goto next;
}

/// The falling rocks' work: their tiles, 64 rocks and their sprites, in a
/// ring that next walks round.
struct RockWork {
  u8 tiles[0x500];
  struct Rock rocks[64];
  struct EffectSprite sprites[64];
  s16 next;
  u16 tileBase;
  s16 resource;
};

// Spawns count rocks around (x, y, z), each pushed out at a random angle and
// thrown up and out along it.
#include "shared/rocks_spawn.h"

/// The falling rocks' setup: loads their picture, sets up 64 small sprites,
/// marks each rock idle and starts the task that spawns them.
void TundariaTower_StartFallingRocks(void) {
  struct RockWork *work =
      HeapSlot_Alloc(HEAP_SLOT_MENU, sizeof(struct RockWork));
  s32 resource;
  s32 tile;
  struct Rock *rock;
  struct EffectSprite *sprite;
  s32 i;

  Decompress_TaggedLz(Resource_GetTableEntry((s32)&ResourceId_FallingRock),
                      work);
  resource = VramCache_FindFreeEntry();
  tile = Vram_Load(resource, sizeof(work->tiles), work);
  work->tileBase = tile;
  work->resource = resource;
  rock = work->rocks;
  sprite = work->sprites;
  for (i = 63; i >= 0; i--) {
    EffectSprite_Init(sprite, 8, 8, 0x40000000, tile);
    sprite->oam.fullColor = 1;
    sprite->oam.palette = 0;
    sprite->oam.priority = 0;
    sprite->depth = 240;
    rock->timer = -1;
    sprite++;
    rock++;
  }
  {
    s32 zero = 0;

    work->next = zero;
  }
  Task_AddFrame(TundariaTower_UpdateFallingRocks, TASK_PASS_VBLANK);
}

void TundariaTower_EndFallingRocks(void) {
  struct RockWork *work = Ram_HeapSlots->menuWork;

  Task_Remove(TundariaTower_UpdateFallingRocks);
  VramCache_FreeEntry(work->resource);
  HeapSlot_Release(HEAP_SLOT_MENU);
}

void TundariaTower_FightReflux(void);
void TundariaTower_OpenDoor(void);
void TundariaTower_FreeCenterProng(void);
void TundariaTower_JumpDownSlide(void);
void TundariaTower_LiftPillar(void);

// clang-format off
u32 gFieldEffect_ShortScript[] = {
    SC_WAIT(1),
    SC_LOOP(22, SC_TOP),
    SC_WAIT(1),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_RELEASE,
    SC_END,
};

u32 gFieldEffect_MediumScript[] = {
    SC_WAIT(1),
    SC_LOOP(44, SC_TOP),
    SC_WAIT(1),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_RELEASE,
    SC_END,
};

u32 gFieldEffect_LongScript[] = {
    SC_WAIT(1),
    SC_LOOP(126, SC_TOP),
    SC_WAIT(1),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_RELEASE,
    SC_END,
};

s16 gTundariaTowerLiftPillarsPillars[] = {
    11, 12, 13, 14, -1,
};

s16 gTundariaTowerBurstBroochPillar[] = {
    8, -1,
};

s16 gSlideOnIce_SteerAngles[] = {                                                               
      -1,     0,      0x8000, -1,     0xc000, 0xe000, 0xa000, 0xc000,          
      0x4000, 0x2000, 0x6000, 0x4000, -1,     0,      0x8000, -1,              
      -1,     0,      0x8000, -1,     0xc000, -1,     -1,     0xc000,          
      0x4000, -1,     -1,     0x4000, -1,     0,      0x8000, -1,              
  };

s16 gSlideOnIce_AnglesByKey[] = {
    -1, 0, 0x8000, -1, 0xc000, 0, 0x8000, 0xc000,
    0x4000, 0, 0x8000, 0x4000, -1, 0, 0x8000, -1,
    0,
};

u32 gTundariaTower_RubbleScript[] = {
    SC_WAIT(33),
    SC_SET(SW_HOOK, 0),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_RELEASE,
};

u32 gTundariaTower_DustRingScript[] = {
    SC_WAIT(44),
    SC_SET(SW_HOOK, 0),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_RELEASE,
};

const s32 *gFieldEffect_DurationScripts[] = {
    gFieldEffect_ShortScript,
    gFieldEffect_MediumScript,
    gFieldEffect_LongScript,
};

s16 gTundariaTowerIceColumnsColumns[] = {
    8, 7, FLAG_ICE_COLUMN_POUNDED, 9, 7, FLAG_ICE_COLUMN_POUNDED + 1, 10, 7,
    FLAG_ICE_COLUMN_POUNDED + 2, 11, 7, FLAG_ICE_COLUMN_POUNDED + 3, -1,
};

s16 gTundariaTowerLiftPillarsColumns[] = {
    8, 7, FLAG_ICE_COLUMN_POUNDED, 9, 7, FLAG_ICE_COLUMN_POUNDED + 1, 10, 7,
    FLAG_ICE_COLUMN_POUNDED + 2, -1, 0,
};

u32 gTundariaTower_RefluxHops[] = {
    22, 3, 0, 0x80010000, 0, 1,
    23, 3, 0x1000, 13, 0xffff, 0x80010000,
};

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gTundariaTowerLiftPillarsExitRegions[] = {
    0x500330, 0x3400210, 0x2200060, 0x6ffff, 0xffff, 0,
    0, 0,
};

u32 gTundariaTowerBurstBroochExitRegions[] = {
    0xf00b0, 0xc000c0, 0xd00021, 0x1ffff, 0xffff, 0,
    0, 0,
};

u32 gSceneExits[] = {
    SCENE_EXITS(TundariaTowerEntrance),
    SCENE_EXIT_UNLESS(1, WorldMap, 46, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, TundariaTowerCenterProng, 1, EXIT_FLAG_NONE),
    SCENE_EXITS(TundariaTowerCenterProng),
    SCENE_EXIT_UNLESS(1, TundariaTowerEntrance, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, TundariaTowerCenterProng, 11, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(3, TundariaTowerCenterProng, 7, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(4, TundariaTowerCenterProng, 8, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(5, TundariaTowerLiftPillars, 5, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(6, TundariaTowerCenterProng, 16, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(7, TundariaTowerCenterProng, 3, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(8, TundariaTowerCenterProng, 4, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(9, TundariaTowerCenterProng, 18, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(10, TundariaTowerIceSlide, 9, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(11, TundariaTowerCenterProng, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(12, TundariaTowerLiftPillars, 4, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(13, TundariaTowerCenterProng, 15, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(14, TundariaTowerCenterProng, 17, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(15, TundariaTowerCenterProng, 13, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(16, TundariaTowerCenterProng, 6, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(17, TundariaTowerCenterProng, 14, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(18, TundariaTowerCenterProng, 9, EXIT_FLAG_NONE),
    SCENE_EXITS(TundariaTowerIceSlide),
    SCENE_EXIT_UNLESS(1, TundariaTowerIceSlide, 3, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, TundariaTowerIceSlide, 7, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(3, TundariaTowerIceSlide, 1, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(4, TundariaTowerIceSlide, 10, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(5, TundariaTowerReflux, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(6, TundariaTowerIceSlide, 11, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(7, TundariaTowerIceSlide, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(8, TundariaTowerIceSlide, 12, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(9, TundariaTowerCenterProng, 10, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(10, TundariaTowerIceSlide, 4, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(11, TundariaTowerIceSlide, 6, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(12, TundariaTowerIceSlide, 8, EXIT_FLAG_NONE),
    SCENE_EXITS(TundariaTowerReflux),
    SCENE_EXIT_UNLESS(1, TundariaTowerIceColumns, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, TundariaTowerIceSlide, 5, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(3, TundariaTowerIceColumns, 6, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(4, TundariaTowerIceColumns, 5, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(5, TundariaTowerReflux, 6, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(6, TundariaTowerReflux, 5, EXIT_FLAG_NONE),
    SCENE_EXITS(TundariaTowerIceColumns),
    SCENE_EXIT_UNLESS(1, TundariaTowerIceColumns, 3, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, TundariaTowerReflux, 1, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(3, TundariaTowerIceColumns, 1, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(4, TundariaTowerIceColumns, 8, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(5, TundariaTowerReflux, 4, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(6, TundariaTowerReflux, 3, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(7, TundariaTowerLiftPillars, 1, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(8, TundariaTowerIceColumns, 4, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(9, TundariaTowerLiftPillars, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(10, TundariaTowerLiftPillars, 9, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(11, TundariaTowerReflux, 7, EXIT_FLAG_NONE),
    SCENE_EXITS(TundariaTowerLiftPillars),
    SCENE_EXIT_UNLESS(1, TundariaTowerIceColumns, 7, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, TundariaTowerIceColumns, 9, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(3, TundariaTowerBurstBrooch, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(4, TundariaTowerCenterProng, 12, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(5, TundariaTowerCenterProng, 5, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(6, TundariaTowerBurstBrooch, 1, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(7, TundariaTowerLiftPillars, 8, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(8, TundariaTowerLiftPillars, 7, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(9, TundariaTowerIceColumns, 10, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(10, TundariaTowerLiftPillars, 10, EXIT_FLAG_NONE),
    SCENE_EXITS(TundariaTowerBurstBrooch),
    SCENE_EXIT_UNLESS(1, TundariaTowerLiftPillars, 6, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, TundariaTowerLiftPillars, 3, EXIT_FLAG_NONE),
    SCENE_EXITS_END,
};

struct ActorPlacement gTundariaTowerCenterProngActorPlacements[] = {
    {489, CONDITION_ALWAYS, 1, 1, 0, 0, FACING_EAST, 2, 2},
    {SCENE_TABLE_END},
};

struct ActorPlacement gTundariaTowerIceSlideActorPlacements[] = {
    {SCENE_TABLE_END},
};

struct ActorPlacement gTundariaTowerRefluxActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_MARS_DJINNI, FLAG_DJINNI(DJINNI_REFLUX), 7, 424, 0, 536, FACING_WEST, TALK_FACING_PARTY, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gTundariaTowerIceColumnsActorPlacements[] = {                                             
      {406, CONDITION_ALWAYS, 1, 1, 0, 0, FACING_SOUTH, 2, 2},                
      {406, CONDITION_ALWAYS, 1, 2, 0, 0, FACING_SOUTH, 2, 2},                
      {406, CONDITION_ALWAYS, 1, 3, 0, 0, FACING_SOUTH, 2, 2},                
      {406, CONDITION_ALWAYS, 1, 4, 0, 0, FACING_SOUTH, 2, 2},                
      {SCENE_TABLE_END},                                                       
  };

struct ActorPlacement gTundariaTowerLiftPillarsActorPlacements[] = {                                             
      {406, CONDITION_ALWAYS, 1, 1, 0, 0, FACING_SOUTH, 2, 2},                
      {406, CONDITION_ALWAYS, 1, 2, 0, 0, FACING_SOUTH, 2, 2},                
      {406, CONDITION_ALWAYS, 1, 3, 0, 0, FACING_SOUTH, 2, 2},                
      {272, CONDITION_ALWAYS, 1, 4, 0, 0, FACING_SOUTH, 2, 2},                
      {272, CONDITION_ALWAYS, 1, 5, 0, 0, FACING_SOUTH, 2, 2},                
      {272, CONDITION_ALWAYS, 1, 6, 0, 0, FACING_SOUTH, 2, 2},                
      {272, CONDITION_ALWAYS, 1, 7, 0, 0, FACING_SOUTH, 2, 2},                
      {SCENE_TABLE_END},                                                       
  };

struct ActorPlacement gTundariaTowerBurstBroochActorPlacements[] = {
    {272, CONDITION_ALWAYS, 1, 2, 0, 0, FACING_SOUTH, 2, 2},
    {SCENE_TABLE_END},
};

struct Trigger gTundariaTowerEntranceTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_TABLE_END},
};
// clang-format on

struct Trigger gTundariaTowerCenterProngTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 3, CONDITION_ALWAYS,
     EVENT(TundariaTower_OpenDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 4, CONDITION_ALWAYS,
     EVENT(TundariaTower_OpenDoor)},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_EXIT, 12, CONDITION_ALWAYS, 12},
    {TRIGGER_EXIT, 13, CONDITION_ALWAYS, 13},
    {TRIGGER_EXIT, 14, CONDITION_ALWAYS, 14},
    {TRIGGER_EXIT, 15, CONDITION_ALWAYS, 15},
    {TRIGGER_EXIT, 16, CONDITION_ALWAYS, 16},
    {TRIGGER_EXIT, 17, CONDITION_ALWAYS, 17},
    {TRIGGER_EXIT, 18, CONDITION_ALWAYS, 18},
    {TRIGGER_EXAMINE, 52, CONDITION_ALWAYS,
     EVENT(TundariaTower_ExamineCrystal)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_PARCH, PSYNERGY_PHASE_ACTS), 50,
     FLAG_LEFT_WATER_PARCHED, EVENT(TundariaTower_ParchLeftWater)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_PARCH, PSYNERGY_PHASE_ACTS), 51,
     FLAG_RIGHT_WATER_PARCHED, EVENT(TundariaTower_ParchRightWater)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS), 52,
     FLAG_CRYSTAL_BROKEN, EVENT(TundariaTower_BurstCrystal)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_AFTER_CAST), 52,
     FLAG_CRYSTAL_BROKEN, EVENT(TundariaTower_FreeCenterProng)},
    {TRIGGER_TABLE_END},
};

struct Trigger gTundariaTowerIceSlideTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_EXIT, 12, CONDITION_ALWAYS, 12},
    {TRIGGER_STEP, 50, CONDITION_ALWAYS, EVENT(TundariaTower_SlideOnIce)},
    {TRIGGER_TABLE_END},
};

struct Trigger gTundariaTowerRefluxTriggers[] = {
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_STEP, 50, CONDITION_ALWAYS, EVENT(TundariaTower_SlideOnIce)},
    {TRIGGER_STEP, 51, FLAG_REFLUX_APPEARED,
     EVENT(TundariaTower_RefluxAppears)},
    {TRIGGER_TALK, 8, CONDITION_ALWAYS, EVENT(TundariaTower_FightReflux)},
    {TRIGGER_TABLE_END},
};

struct Trigger gTundariaTowerIceColumnsTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_STEP, 11, CONDITION_ALWAYS, EVENT(TundariaTower_JumpDownSlide)},
    {TRIGGER_STEP, 50, CONDITION_ALWAYS, EVENT(TundariaTower_SlideOnIce)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), 8,
     FLAG_ICE_COLUMN_POUNDED, EVENT(TundariaTower_PoundIceColumn)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), 9,
     FLAG_ICE_COLUMN_POUNDED + 1, EVENT(TundariaTower_PoundIceColumn)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), 10,
     FLAG_ICE_COLUMN_POUNDED + 2, EVENT(TundariaTower_PoundIceColumn)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), 11,
     FLAG_ICE_COLUMN_POUNDED + 3, EVENT(TundariaTower_PoundIceColumn)},
    {TRIGGER_TABLE_END},
};

struct Trigger gTundariaTowerLiftPillarsTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 30, CONDITION_ALWAYS, 10},
    {TRIGGER_STEP, 31, CONDITION_ALWAYS, EVENT(TundariaTower_StopSliding)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), 8,
     FLAG_ICE_COLUMN_POUNDED, EVENT(TundariaTower_PoundIceColumn)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), 9,
     FLAG_ICE_COLUMN_POUNDED + 1, EVENT(TundariaTower_PoundIceColumn)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), 10,
     FLAG_ICE_COLUMN_POUNDED + 2, EVENT(TundariaTower_PoundIceColumn)},
    {TRIGGER_PUSH, 0, CONDITION_ALWAYS, EVENT(TundariaTower_LiftPillar)},
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS, EVENT(TundariaTower_SetDownPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_FIRST), 11,
     CONDITION_ALWAYS, EVENT(TundariaTower_ResolveBeforePsynergy)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 11,
     CONDITION_ALWAYS, EVENT(TundariaTower_LiftPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_ACTS), 11,
     CONDITION_ALWAYS, EVENT(TundariaTower_SlidePillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 12,
     FLAG_SECOND_LEDGE_PILLAR_SET, EVENT(TundariaTower_LiftPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 12, FLAG_SECOND_LEDGE_PILLAR_SET,
     EVENT(TundariaTower_SetDownPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 13,
     FLAG_FIRST_LEDGE_PILLAR_SET, EVENT(TundariaTower_LiftPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 13, FLAG_FIRST_LEDGE_PILLAR_SET,
     EVENT(TundariaTower_SetDownPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_FIRST), 14,
     CONDITION_ALWAYS, EVENT(TundariaTower_ResolveBeforePsynergy)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 14,
     CONDITION_ALWAYS, EVENT(TundariaTower_LiftPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_ACTS), 14,
     CONDITION_ALWAYS, EVENT(TundariaTower_SlidePillar)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0,
     CONDITION_ALWAYS, EVENT(TundariaTower_RevealIcyWall)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0,
     CONDITION_ALWAYS, EVENT(TundariaTower_EndRevealIcyWall)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_FIRST), 52,
     CONDITION_ALWAYS, EVENT(TundariaTower_ResolveBeforePsynergy)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS), 52,
     FLAG_BROWN_WALL_BURST, EVENT(TundariaTower_BurstBrownWall)},
    {TRIGGER_TABLE_END},
};

struct Trigger gTundariaTowerBurstBroochTriggers[] = {
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 8,
     CONDITION_ALWAYS, EVENT(TundariaTower_LiftPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 8, CONDITION_ALWAYS,
     EVENT(TundariaTower_SetDownPillar)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS), 50,
     FLAG_ICY_WALL_BURST, EVENT(TundariaTower_BurstIcyWall)},
    {TRIGGER_TABLE_END},
};
#undef EFFECT_SPAWN
#undef EFFECT_CREATE
#undef EFFECT_SET_MODE
#undef EFFECT_SET_SCRIPT
#undef EFFECT_SCRIPTS
#undef FIELD_AT
#undef BYTE_AT
