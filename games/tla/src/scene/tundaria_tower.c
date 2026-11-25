/// @file
/// Tundaria Tower, the frozen tower that holds the Center Prong, in seven maps
/// that share this overlay (SceneId_TundariaTowerEntrance to
/// SceneId_TundariaTowerBurstBrooch).
///
/// In the first hall the middle way leads to an ice crystal with something
/// frozen inside, and Parch dries the pools left and right of it, uncovering a
/// door in each. The right door leads to fields of black ice to slide across
/// and up to a hall with three ways: Pound flattens the ice columns on the left
/// way for its chests, a black ice slide on the lower way drops to the sliding
/// puzzle where the Mars Djinni Reflux appears, and the right way leads to a
/// room where Pound and Move set two pillars against a ledge to climb. Beyond
/// it Reveal opens a way through an icy wall, and outside Felix finds the Burst
/// Brooch; Burst on the brown part of the outer wall brings its rocks down for
/// a shortcut. With Burst the crystal breaks to free the Center Prong, and the
/// icy wall at the end of the left door's way breaks to reach four more chests.
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
#include "field/field_psynergy_state.h"
#include "field/field_event.h"
#include "field/field_sequence.h"
#include "field/field_sequence_event.h"
#include "field/field_sprite.h"
#include "field/map.h"
#include "field/map_grid.h"
#include "field/object.h"
#include "field/particle.h"
#include "field/scene_tables.h"
#include "game/game.h"
#include "gamepak/far_call/system.h"
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
#include "system/main_routine_state.h"
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

/// Actor_StartScript, through the far-call veneer this overlay's import table
/// names for it.
void TundariaTower_StartEffectScript(struct FieldEffect *effect,
                                     const s32 *script);

/// The Center Prong frozen in the crystal (SceneId_TundariaTowerCenterProng),
/// an item icon (ITEM_CENTER_PRONG) until the crystal breaks.
#define ACTOR_FROZEN_CENTER_PRONG ACTOR_FIRST_PLACED
/// The Center Prong where it falls when the crystal breaks, glinting
/// (TundariaTower_SparkleCenterProng).
#define ACTOR_FALLEN_CENTER_PRONG (FIELD_TREASURE_ACTOR + 4)
/// Reflux, the Mars Djinni (SceneId_TundariaTowerReflux).
#define ACTOR_REFLUX ACTOR_FIRST_PLACED
/// The two stone pillars on the ledge in the room of pillars
/// (SceneId_TundariaTowerPillarPuzzle), which Move pulls down and a push sets
/// against the ledge as steps; the second is first in the table. The two that
/// slide over the ice there, and the ice columns before them.
#define ACTOR_SECOND_LEDGE_PILLAR (ACTOR_FIRST_PLACED + 4)
#define ACTOR_FIRST_LEDGE_PILLAR (ACTOR_FIRST_PLACED + 5)
#define ACTOR_FIRST_SLIDING_PILLAR (ACTOR_FIRST_PLACED + 3)
#define ACTOR_SECOND_SLIDING_PILLAR (ACTOR_FIRST_PLACED + 6)
#define ACTOR_FIRST_ICE_COLUMN ACTOR_FIRST_PLACED
/// The stone pillar in the Burst Brooch's room.
#define ACTOR_BROOCH_ROOM_PILLAR ACTOR_FIRST_PLACED

/// The ways between two parts of one map: each leads to its partner's
/// entrance and back.
enum TundariaTowerWay {
  /// The Center Prong's first hall: its north doorway to the crystal's
  /// chamber, and the doors behind its left and right pools
  /// (TundariaTower_OpenDoor), to the ends of the west and east rooms.
  CENTER_PRONG_HALL_NORTH_DOORWAY = 2,
  CENTER_PRONG_LEFT_POOL_DOOR = 3,
  CENTER_PRONG_RIGHT_POOL_DOOR = 4,
  CENTER_PRONG_WEST_ROOM_NORTH_DOORWAY = 6,
  CENTER_PRONG_WEST_ROOM_EAST_END = 7,
  CENTER_PRONG_EAST_ROOM_WEST_END = 8,
  CENTER_PRONG_EAST_ROOM_NORTH_DOORWAY = 9,
  CENTER_PRONG_CRYSTAL_CHAMBER = 11,
  /// The north corridor's two passages south, into the back of the two chest
  /// rooms, whose front halves the west and east rooms' doorways reach.
  CENTER_PRONG_CORRIDOR_MIDDLE_PASSAGE = 13,
  CENTER_PRONG_CORRIDOR_EAST_PASSAGE = 14,
  CENTER_PRONG_WEST_CHESTS_BACK = 15,
  CENTER_PRONG_WEST_CHESTS_FRONT = 16,
  CENTER_PRONG_EAST_CHESTS_BACK = 17,
  CENTER_PRONG_EAST_CHESTS_FRONT = 18,
  /// The Ice Slide: the feet of the icy hall west and east, to the north
  /// doorways of the rooms below, whose feet lead up the three ladders of the
  /// ice floor at the bottom.
  ICE_SLIDE_HALL_WEST_FOOT = 1,
  ICE_SLIDE_HALL_EAST_FOOT = 2,
  ICE_SLIDE_WEST_ROOM_DOORWAY = 3,
  ICE_SLIDE_WEST_ROOM_FOOT = 4,
  ICE_SLIDE_STAIR_ROOM_FOOT = 6,
  ICE_SLIDE_EAST_ROOM_DOORWAY = 7,
  ICE_SLIDE_EAST_ROOM_FOOT = 8,
  ICE_SLIDE_ICE_FLOOR_WEST_LADDER = 10,
  ICE_SLIDE_ICE_FLOOR_MIDDLE_LADDER = 11,
  ICE_SLIDE_ICE_FLOOR_EAST_LADDER = 12,
  /// Reflux's floor: the south-west foot of the hall of stairs, to the north
  /// doorway of the sliding puzzle's room, where Reflux waits.
  REFLUX_HALL_SOUTHWEST_FOOT = 5,
  REFLUX_PUZZLE_ROOM_DOORWAY = 6,
  /// Where the party lands in the sliding puzzle's room from the slide.
  REFLUX_FROM_SLIDE = 7,
  /// The ice columns' floor: the foot of the room of chests and ice columns,
  /// to the north room's east doorway; the great hall's west doorway, to the
  /// corridor's south one.
  ICE_COLUMNS_CHEST_ROOM_FOOT = 1,
  ICE_COLUMNS_NORTH_ROOM_EAST_DOORWAY = 3,
  ICE_COLUMNS_HALL_WEST_DOORWAY = 4,
  ICE_COLUMNS_CORRIDOR_SOUTH_DOORWAY = 8,
  /// The room of pillars' floor: the foot of the room whose ice floor
  /// reflects Felix, to the
  /// lower corridor's middle doorway; and the hole in the floor of the room
  /// by the Burst Brooch's south door, cell (29, 7), down a black ice slide
  /// to the lower corridor's west end.
  PILLAR_PUZZLE_MIRROR_ROOM_FOOT = 7,
  PILLAR_PUZZLE_CORRIDOR_DOORWAY = 8,
  PILLAR_PUZZLE_FROM_SLIDE = 10,
  PILLAR_PUZZLE_SLIDE_HOLE = 30
};

#include "../field/field_effect_move.c"
#define EFFECT_SET_MODE Actor_SelectAnimation
#define EFFECT_SET_SCRIPT TundariaTower_StartEffectScript
#include "../field/field_effect_spawn.c"

#undef EFFECT_SET_SCRIPT

#include "shared/tundaria_tower.h"
#include "shared/tables.h"
#include "shared/event_check_spheres_overlap.h"

/// Parch dried the pool left of the middle way.
#define FLAG_LEFT_WATER_PARCHED 0x942
/// Parch dried the pool right of the middle way.
#define FLAG_RIGHT_WATER_PARCHED 0x943
#define FLAG_CRYSTAL_BROKEN 0x944 ///< Burst broke the crystal round the prong
/// The second ledge pillar dropped beside the first: the steps are made.
#define FLAG_SECOND_LEDGE_PILLAR_SET 0x946
/// A ledge pillar dropped to the ledge's foot.
#define FLAG_FIRST_LEDGE_PILLAR_SET 0x947
/// Burst opened the brown wall outside, in the Burst Brooch's room.
#define FLAG_BROWN_WALL_BURST 0x948
/// Burst broke the icy wall at the end of the left door's way, in the room
/// whose ice floor reflects Felix.
#define FLAG_MIRROR_WALL_BURST 0x94b
#define FLAG_REFLUX_APPEARED FLAG_SCENE_TEMP_00
#define FLAG_FIGHTING_REFLUX FLAG_SCENE_TEMP_01
/// The first of four: Pound flattened ice column n.
#define FLAG_ICE_COLUMN_POUNDED FLAG_SCENE_TEMP_00
/// Reveal shows the hidden way through the icy wall at the dead end beyond the
/// room of pillars.
#define FLAG_HIDDEN_WAY_REVEALED FLAG_SCENE_TEMP_12

/// How far a standing ice column's top rises over the ground under it, in
/// height steps of 8 pixels.
#define ICE_COLUMN_HEIGHT_STEPS 4

/// A pillar's starting cell, kept in its timer while it slides: its column
/// over its row's byte.
#define START_CELL_ROW_MASK 0xff

/// The tower's own cell behaviors, the cells its step and Psynergy triggers
/// name; a cell's attributes hold its behavior in their high byte
/// (MapCell_SetAttributes).
enum TundariaTowerCellBehavior {
  /// Where the leader's slide over the ice ends
  /// (TundariaTower_StopSliding).
  CELL_BEHAVIOR_TUNDARIA_TOWER_SLIDE_END = 31,
  /// Black ice, slid across (TundariaTower_SlideOnIce).
  CELL_BEHAVIOR_TUNDARIA_TOWER_ICE = 50,
  /// In the Burst Brooch's room, the cell where Burst opens the brown wall
  /// (TundariaTower_BurstBrownWall).
  CELL_BEHAVIOR_TUNDARIA_TOWER_BROWN_WALL = 50,
  /// The step on which Reflux appears (TundariaTower_RefluxAppears).
  CELL_BEHAVIOR_TUNDARIA_TOWER_REFLUX_APPEARS = 51,
  /// The first hall's doors behind the left and right pools, each the number
  /// of the exit it leads through (TundariaTower_OpenDoor).
  CELL_BEHAVIOR_TUNDARIA_TOWER_LEFT_DOOR = CENTER_PRONG_LEFT_POOL_DOOR,
  CELL_BEHAVIOR_TUNDARIA_TOWER_RIGHT_DOOR = CENTER_PRONG_RIGHT_POOL_DOOR,
  /// The first hall's left and right pools, which Parch dries.
  CELL_BEHAVIOR_TUNDARIA_TOWER_LEFT_POOL = 50,
  CELL_BEHAVIOR_TUNDARIA_TOWER_RIGHT_POOL = 51,
  /// The crystal holding the Center Prong, and the icy wall Burst breaks at
  /// the end of the left door's way.
  CELL_BEHAVIOR_TUNDARIA_TOWER_CRYSTAL = 52,
  CELL_BEHAVIOR_TUNDARIA_TOWER_ICY_WALL = 52,
  /// The top of the black ice slide down to Reflux's sliding puzzle
  /// (TundariaTower_JumpDownSlide).
  CELL_BEHAVIOR_TUNDARIA_TOWER_SLIDE_TOP = 11
};

void TundariaTower_AdvanceIceChip(union FieldObject *object);
void TundariaTower_ScatterIceChips(struct Vector3 *position, s32 count,
                                   FIXED scale);
void Particle_UpdateFalling(void);
void TundariaTower_StartFallingRocks(void);
void TundariaTower_EndFallingRocks(void);
void TundariaTower_UpdateBrownWallCell(void);
void TundariaTower_SparkleCenterProng(struct Actor *actor);
extern s16 gTundariaTowerPillarPuzzlePillars[];
extern s16 gTundariaTowerBurstBroochPillar[];
extern struct Trigger gTundariaTowerEntranceTriggers[];
extern struct Trigger gTundariaTowerCenterProngTriggers[];
extern struct Trigger gTundariaTowerIceSlideTriggers[];
extern struct Trigger gTundariaTowerRefluxTriggers[];
extern struct Trigger gTundariaTowerIceColumnsTriggers[];
extern struct Trigger gTundariaTowerPillarPuzzleTriggers[];
extern struct Trigger gTundariaTowerBurstBroochTriggers[];

/// Talking to Reflux, once he has hopped down before the party, starts his
/// battle.
void TundariaTower_FightReflux(void) {
  GameFlag_Set(FLAG_FIGHTING_REFLUX);
  Event_RequestDjinnBattle(ACTOR_REFLUX, DJINNI_REFLUX);
}

#include "../field/actor_wait_until_landed.c"

#include "../field/map_cell_get.c"

#include "shared/map_cell_set.c"

void MapGrid_SetCellHeight(s32 x, s32 z, s32 terrain, s32 height);

/// Makes the ground under an ice column match it, by its Pound flag (kept in
/// its timer): a standing column blocks the way and Felix can jump onto its
/// top, ICE_COLUMN_HEIGHT_STEPS up; one pounded flat is walked over and drawn
/// beneath him.
void TundariaTower_SetColumnCell(struct Actor *actor) {
  s32 x = FIELD_CELL(actor->x.fixed);
  s32 z = FIELD_CELL(actor->z.fixed);
  s32 attr0 = MapCell_GetAttributes(MAPGRID_LAYER_GROUND, x, z);
  s32 attr2 = MapCell_GetAttributes(MAPGRID_LAYER_OVERHEAD, x, z);
  s32 height = CELL_HEIGHT_STEPS(
      Map_GetTerrainHeight(actor->layer, actor->x.fixed, actor->z.fixed));

  if (GameFlag_Get(actor->timer)) {
    attr2 &= ~CELL_ATTRIBUTES(CELL_BEHAVIOR_WALL, 0);
    attr0 &= ~MAP_CELL_JUMP;
    if (CELL_HEIGHT_STEPS(actor->groundHeight) != height)
      height -= ICE_COLUMN_HEIGHT_STEPS;
    Scene_SetActorSpritePriority(actor, 3);
  } else {
    attr2 = CELL_ATTRIBUTES(CELL_BEHAVIOR_WALL, attr2 & CELL_ATTRIBUTE_BITS);
    attr0 |= MAP_CELL_JUMP;
    height += ICE_COLUMN_HEIGHT_STEPS;
    Scene_SetActorSpritePriority(actor, 2);
  }
  MapCell_SetAttributes(MAPGRID_LAYER_GROUND, x, z, attr0);
  MapCell_SetAttributes(MAPGRID_LAYER_OVERHEAD, x, z, attr2);
  MapGrid_SetCellHeight(FIELD_CELL_ORIGIN(x), FIELD_CELL_ORIGIN(z),
                        actor->layer, height);
}

/// Sets each listed ice column, by its row of (actor, animation, flag),
/// standing or pounded flat as its flag says (TundariaTower_SetColumnCell).
void TundariaTower_SetUpIceColumns(u16 *table) {
  u16 *row;
  struct FieldActor *object;
  s16 flag;
  s16 animation;

  if (table[0] == ACTOR_LIST_END)
    return;
  for (row = table; row[0] != ACTOR_LIST_END; row += 3) {
    object = ((struct FieldActor *)Actor_GetById(row[0]));
    animation = row[1];
    flag = row[2];

    Actor_SetSpriteFlags(object, 0);
    Actor_SelectAnimation(object, (u16)animation + GameFlag_Get((u16)flag));
    object->motionFlags = 0;
    object->collisionFlags &= ~ACTOR_SOLID;
    object->timer = flag;
    TundariaTower_SetColumnCell(object);
  }
}

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

extern struct SceneRegion gTundariaTowerPillarPuzzleExitRegions[];
extern struct SceneRegion gTundariaTowerBurstBroochExitRegions[];

const struct SceneRegion *Scene_GetExitRegions(void) {
  if (gMainRoutineState.next == SCENE_TUNDARIA_TOWER_PILLAR_PUZZLE)
    return gTundariaTowerPillarPuzzleExitRegions;
  if (gMainRoutineState.next == SCENE_TUNDARIA_TOWER_BURST_BROOCH)
    return gTundariaTowerBurstBroochExitRegions;
  return 0;
}

const u32 *Scene_GetExits(void) { return gSceneExits; }

extern struct ActorPlacement gTundariaTowerCenterProngActorPlacements[];
extern struct ActorPlacement gTundariaTowerIceSlideActorPlacements[];
extern struct ActorPlacement gTundariaTowerRefluxActorPlacements[];
extern struct ActorPlacement gTundariaTowerIceColumnsActorPlacements[];
extern struct ActorPlacement gTundariaTowerPillarPuzzleActorPlacements[];
extern struct ActorPlacement gTundariaTowerBurstBroochActorPlacements[];

const struct Trigger *Scene_GetActorPlacements(void) {
  if (gMainRoutineState.next == SCENE_TUNDARIA_TOWER_CENTER_PRONG)
    return (const struct Trigger *)gTundariaTowerCenterProngActorPlacements;
  if (gMainRoutineState.next == SCENE_TUNDARIA_TOWER_ICE_SLIDE)
    return (const struct Trigger *)gTundariaTowerIceSlideActorPlacements;
  if (gMainRoutineState.next == SCENE_TUNDARIA_TOWER_REFLUX)
    return (const struct Trigger *)gTundariaTowerRefluxActorPlacements;
  if (gMainRoutineState.next == SCENE_TUNDARIA_TOWER_ICE_COLUMNS)
    return (const struct Trigger *)gTundariaTowerIceColumnsActorPlacements;
  if (gMainRoutineState.next == SCENE_TUNDARIA_TOWER_PILLAR_PUZZLE)
    return (const struct Trigger *)gTundariaTowerPillarPuzzleActorPlacements;
  if (gMainRoutineState.next == SCENE_TUNDARIA_TOWER_BURST_BROOCH)
    return (const struct Trigger *)gTundariaTowerBurstBroochActorPlacements;
  return (const struct Trigger *)gTundariaTowerCenterProngActorPlacements;
}

/// Parch on the pool left of the first hall's middle way dries it, uncovering
/// a green door in the wall behind it.
void TundariaTower_ParchLeftWater(void) {
  Event_Start();
  MapGrid_CopyCellAttributes(19, 55, 7, 8, 24, 46);
  MapGrid_CopyCellsTo(24, 102, 24, 110, 7, 8);
  GameFlag_Set(FLAG_LEFT_WATER_PARCHED);
  Event_End();
}

/// Parch on the pool right of the first hall's middle way dries it,
/// uncovering a door in the wall behind it.
void TundariaTower_ParchRightWater(void) {
  Event_Start();
  MapGrid_CopyCellAttributes(37, 55, 7, 8, 32, 46);
  MapGrid_CopyCellsTo(32, 102, 32, 110, 7, 8);
  GameFlag_Set(FLAG_RIGHT_WATER_PARCHED);
  Event_End();
}

/// Walking north into the door behind one of the first hall's dried pools
/// opens it, and Felix walks through: the left door leads to the way that
/// ends at an icy wall, the right one to the black ice fields.
void TundariaTower_OpenDoor(void) {
  struct FieldState *fieldState = Ram_HeapSlots->fieldState;
  s16 *choice;
  s32 x = 37;
  struct MainRoutineState *state;
  s32 cell;

  Event_Start();
  FieldEvent_EndLastingPsynergy(0);
  choice = &fieldState->stepTrigger;
  if (*choice == CELL_BEHAVIOR_TUNDARIA_TOWER_LEFT_DOOR)
    x = 25;
  Event_PlaySound(SE_DOOR_OPEN);
  MapGrid_CopyCellsTo(30, 38, x, 49, 1, 2);
  FieldEvent_Wait(10);
  MapGrid_CopyCellsTo(32, 38, x, 49, 1, 2);
  FieldEvent_Wait(10);
  state = &gMainRoutineState;
  Actor_SetSpeed(state->leaderActorId, 0.5);
  Actor_GetById(state->leaderActorId)->motionFlags = 0;
  cell = FIELD_HALF_CELL(Actor_GetById(state->leaderActorId)->x.fixed) | 1;
  Actor_WalkToAndWait(state->leaderActorId, cell << 3,
                      FIXEDtoINT(Actor_GetById(state->leaderActorId)->z.fixed) |
                          1);
  Event_PlaySound(SE_MAP_EXIT);
  cell = Actor_GetById(state->leaderActorId)->x.fixed;
  Actor_WalkTo(state->leaderActorId, FIELD_HALF_CELL(cell) << 3,
               (FIXEDtoINT(Actor_GetById(state->leaderActorId)->z.fixed) | 1) -
                   16);
  FieldEvent_Wait(5);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Actor_WaitForMoveById(state->leaderActorId);
  Import_EventSetExitRequest(*choice);
  Event_End();
}

/// Each frame of a chip of ice: it moves and scales by its rates and falls.
void TundariaTower_AdvanceIceChip(union FieldObject *object) {
  object->effect.x += object->effect.velocity.x;
  object->effect.y += object->effect.velocity.y;
  object->effect.z += object->effect.velocity.z;
  object->effect.scaleX += object->effect.scaleRateX;
  object->effect.scaleY += object->effect.scaleRateY;
  object->effect.velocity.y -= toFIXED(3) / 10;
}

/// Throws chips of ice (SPRITE_BLUE_CRYSTAL) off a point, each turned at
/// random, to fall under TundariaTower_AdvanceIceChip.
void TundariaTower_ScatterIceChips(struct Vector3 *position, s32 count,
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
  options.update = TundariaTower_AdvanceIceChip;
  for (i = 0; i != count; i++) {
    options.rotation = (Random16() & 15) << 12;
    velocityX = Random16() & 15;
    velocityX -= 8;
    velocityX *= toFIXED(1) / 4;
    velocityY = Random16() & 15;
    velocityY += 8;
    velocityY *= toFIXED(1) / 4;
    x = position->x - INTtoFIXED(Random16() & 31) + toFIXED(15);
    FieldEffect_Spawn(x, position->y + INTtoFIXED(Random16() & 15), position->z,
                      velocityX, velocityY, 0,
                      EFFECT_USE_UPDATE | EFFECT_USE_ROTATION |
                          EFFECT_USE_TYPE | EFFECT_USE_START_SCALE |
                          EFFECT_SCALE_TO_TARGET | EFFECT_USE_PRIORITY,
                      &options);
  }
}

/// Burst on the ice crystal in the Center Prong's chamber, cell (21, 23): it
/// cracks and throws off chips of ice.
void TundariaTower_BurstCrystal(s32 step) {
  struct Vector3 position;

  switch (step) {
  case 1:
    break;
  case 2:
    position.x = toFIXED(347);
    position.y = 0;
    position.z = toFIXED(376);
    TundariaTower_ScatterIceChips(&position, 1, FIXED_ONE * 8 / 10);
    break;
  case 3:
    MapGrid_CopyCellTilesIndicesRect(31, 85, 20, 85, 3, 4);
    position.x = toFIXED(347);
    position.y = 0;
    position.z = toFIXED(376);
    TundariaTower_ScatterIceChips(&position, 8, FIXED_ONE * 3 / 2);
    break;
  }
}

/// The fallen Center Prong's update: a sparkle near it every eighth frame.
void TundariaTower_SparkleCenterProng(struct Actor *actor) {
  struct EffectOptions params;
  u32 phase = gFrameCount & 7;
  s32 dx;
  s32 dy;

  if (phase == 0) {
    dx = (Random16() & 15) - 8;
    dy = Random16() & 7;
    params.type = SPRITE_SPARKLE;
    FieldEffect_Spawn(actor->x.fixed + (INTtoFIXED(dx)),
                      actor->y.fixed + (INTtoFIXED(dy - 8)), actor->z.fixed, 0,
                      toFIXED(1) / 2, phase, EFFECT_USE_TYPE, &params);
  }
}

/// As Burst ends, the crystal is broken and the Center Prong drops from it to
/// the floor, where it glints until Felix picks it up.
void TundariaTower_FreeCenterProng(void) {
  struct Actor *actor = Actor_GetById(ACTOR_FROZEN_CENTER_PRONG);

  Event_Start();
  Event_EndLastingPsynergy(0);
  GameFlag_Set(FLAG_CRYSTAL_BROKEN);
  MapGrid_CopyCellAttributes(11, 22, 3, 2, 20, 22);
  actor->gravity = toFIXED(1) / 2;
  actor->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  Actor_WaitUntilLanded(actor);
  Actor_SetPosition(ACTOR_FROZEN_CENTER_PRONG, 0, 0);
  Actor_SetPosition(ACTOR_FALLEN_CENTER_PRONG, 344, 372);
  Actor_GetById(ACTOR_FALLEN_CENTER_PRONG)->update =
      (ObjectUpdate)TundariaTower_SparkleCenterProng;
  Event_End();
}

/// Searching the ice crystal in the Center Prong's chamber, cell (21, 23),
/// says "There's something inside the cracked ice...". Once
/// FLAG_TUNDARIA_TOWER_BURST_BROOCH is set, the ordinary field menu opens by
/// itself as the message closes (`fieldMenu`): the party's HP and PP at the
/// top right and the command icons at the bottom, Psynergy picked, waiting for
/// the player.
void TundariaTower_ExamineCrystal(void) {
  Event_Start();
  Event_EndLastingPsynergy(0);
  FieldMessage_Show(MsgThereSSomething, 1);
  if (GameFlag_Get(FLAG_TUNDARIA_TOWER_BURST_BROOCH)) {
    struct FieldState *state = Ram_HeapSlots->fieldState;

    state->fieldMenu = 1;
  }
  Event_End();
}

/// Stepping onto black ice sets Felix sliding straight on until something
/// stops him.
void TundariaTower_SlideOnIce(void) { Leader_SlideOnIce(); }

extern s32 gTundariaTower_RefluxSpinScript[];

/// Sliding onto cell (21, 33) in Reflux's sliding puzzle: the screen flashes
/// white with a splash and a shake, and Reflux hops down before the party and
/// waits to be talked to.
void TundariaTower_RefluxAppears(void) {
  struct Actor *actor = Actor_GetById(ACTOR_REFLUX);

  Event_Start();
  Event_EndLastingPsynergy(0);
  TundariaTower_FadeFrom(FIXED_ONE, 0);
  PaletteFade_SetTarget(RGB_WHITE, 0);
  PaletteFade_StartInterpolation(2);
  Sound_Play(SE_WATER);
  MapGrid_SetShake(5, 7, 1);
  MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
  Event_Wait(2);
  PaletteFade_SetTarget(FIXED_ONE, 0);
  PaletteFade_StartInterpolation(2);
  actor->steeringFlags = 0;
  actor->update = 0;
  Actor_SetDestination(ACTOR_REFLUX, 424, 536);
  TundariaTower_StartEffectScript(actor, gTundariaTower_RefluxSpinScript);
  Actor_WaitForMoveById(ACTOR_REFLUX);
  GameFlag_Set(FLAG_REFLUX_APPEARED);
  Actor_WatchAndSetScript(ACTOR_REFLUX, gMainRoutineState.leaderActorId);
  MapCell_SetAttributes(MAPGRID_LAYER_GROUND, 21, 33,
                        CELL_ATTRIBUTES(CELL_BEHAVIOR_TUNDARIA_TOWER_ICE, 0));
  MapCell_SetAttributes(MAPGRID_LAYER_GROUND, 22, 33,
                        CELL_ATTRIBUTES(CELL_BEHAVIOR_TUNDARIA_TOWER_ICE, 0));
  MapCell_SetAttributes(MAPGRID_LAYER_GROUND, 26, 33,
                        CELL_ATTRIBUTES(CELL_BEHAVIOR_WALL, 0));
  Event_End();
}

/// Pound on an ice column: chips of ice fly off as it is squashed, and it
/// ends flat in the floor, to be walked over.
void TundariaTower_ShrinkIceColumn(s32 phase, s32 step) {
  struct EffectOptions options;
  struct FieldEffectState *effectState = Ram_HeapSlots->fieldEffectState;
  struct Actor *actor = Actor_GetById(effectState->targetId);
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
    FieldEffect_Spawn(actor->x.fixed,
                      actor->y.fixed - INTtoFIXED(step * 3) + toFIXED(16),
                      actor->z.fixed, velocityX, 0, 0,
                      EFFECT_USE_ROTATION | EFFECT_USE_TYPE |
                          EFFECT_USE_START_SCALE | EFFECT_USE_PRIORITY,
                      &options);
    actor->scaleY -= FIXED_ONE * 31 / 256;
  }
  if (phase == 1) {
    actor->scaleY = FIXED_ONE;
    GameFlag_Set(actor->timer);
    TundariaTower_SetColumnCell(Actor_GetById(effectState->targetId));
    MapGrid_CopyCellTilesIndicesRect(67, 23, (FIELD_CELL(actor->x.fixed)) + 64,
                                     FIELD_CELL(actor->z.fixed), 1, 1);
    actor->z.fixed = 0;
    actor->y.fixed = 0;
    actor->x.fixed = 0;
  }
}

/// Pound on one of the ice columns, on the hall's left way or in the room of
/// pillars, flattens it into the floor.
void TundariaTower_PoundIceColumn(s32 phase, s32 step) {
  TundariaTower_ShrinkIceColumn(phase, step);
}

/// Stepping onto the top of the black ice slide on the lower way from the
/// hall of three ways, cell (19, 57): Felix leans back, then runs and goes
/// down the slide to Reflux's sliding puzzle.
void TundariaTower_JumpDownSlide(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineState.leaderActorId);

  Event_Start();
  Event_EndLastingPsynergy(0);
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  Actor_SetSpriteFlags(Actor_GetById(gMainRoutineState.leaderActorId), 0);
  leader->facing = FACING_SOUTH;
  Event_Wait(10);
  Actor_SetAnimationById(gMainRoutineState.leaderActorId,
                         ANIM_LEADER_LEAN_BACK);
  Event_Wait(30);
  FieldEffect_SetAttached(gMainRoutineState.leaderActorId,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_RunRepeatedMotion(gMainRoutineState.leaderActorId, 2);
  Event_Wait(20);
  leader->facing = FACING_NORTH;
  Actor_SetAnimationById(gMainRoutineState.leaderActorId, ANIM_LEADER_RUN);
  Actor_SetAnimationSpeedById(gMainRoutineState.leaderActorId,
                              ANIM_STEP_NORMAL * 3 / 2);
  Event_Wait(40);
  MapGrid_SetCellHeight(FIELD_CELL_ORIGIN(19), FIELD_CELL_ORIGIN(57), 0, -12);
  Actor_GetById(gMainRoutineState.leaderActorId)->bounce = 0;
  Import_EventSetExitRequest(
      TUNDARIA_TOWER_ICE_COLUMNS_TO_TUNDARIA_TOWER_REFLUX_SOUTH);
  Event_End();
}

/// As Felix starts to push or Move a stone pillar in the room of pillars or the
/// Burst Brooch's room, it is taken off the map grid; in the room of pillars
/// the cell it started from is kept, so that Move can send it on sliding the
/// same way.
void TundariaTower_StartPillarMove(void) {
  struct FieldState *state = Ram_HeapSlots->fieldState;
  struct MainRoutineState *party = &gMainRoutineState;

  if (party->next == SCENE_TUNDARIA_TOWER_PILLAR_PUZZLE) {
    struct Actor *actor;

    Actor_LiftTargetBlock(gTundariaTowerPillarPuzzlePillars);
    actor = state->movedBlock;
    actor->timer = ((FIELD_CELL(actor->x.fixed)) << 8) |
                   ((FIELD_CELL(actor->z.fixed)) & START_CELL_ROW_MASK);
  } else if (party->next == SCENE_TUNDARIA_TOWER_BURST_BROOCH) {
    Actor_LiftTargetBlock(gTundariaTowerBurstBroochPillar);
  }
}

/// Move on one of the two stone pillars on the ice in the room of pillars
/// sends it sliding on, a cell at a time, until the ice ends or a wall stops
/// it, the camera following.
void TundariaTower_SlidePillar(s32 unused, s32 actorArg) {
  struct FieldState *state = Ram_HeapSlots->fieldState;
  struct Actor *actor;
  u16 actorId;
  s32 dx;
  s32 dz;

  actorId = actorArg;
  actor = state->movedBlock;
  Event_Start();
  Event_EndLastingPsynergy(0);
  dx = (FIELD_CELL(actor->x.fixed)) - (actor->timer >> 8);
  dz = (FIELD_CELL(actor->z.fixed)) - (actor->timer & START_CELL_ROW_MASK);
  actor->speedLimit = toFIXED(1) / 2;
  while ((MapCell_GetAttributes(MAPGRID_LAYER_MIDDLE,
                                (FIELD_CELL(actor->x.fixed)) + dx,
                                (FIELD_CELL(actor->z.fixed)) + dz) >>
          8) == CELL_BEHAVIOR_TUNDARIA_TOWER_ICE &&
         (MapCell_GetAttributes(MAPGRID_LAYER_OVERHEAD,
                                (FIELD_CELL(actor->x.fixed)) + dx,
                                (FIELD_CELL(actor->z.fixed)) + dz) >>
          8) != CELL_BEHAVIOR_WALL) {
    Actor_SetMoveTarget(actor, INTtoFIXED(dx * 16) + actor->x.fixed,
                        actor->y.fixed, INTtoFIXED(dz * 16) + actor->z.fixed);
    FieldCamera_FollowActor(actorId, 1);
    Actor_WaitForMove(actor);
    wait(1);
  }
  actor->x.fixed = FIELD_HALF_CELL_ORIGIN(FIELD_HALF_CELL(actor->x.fixed) | 1);
  actor->z.fixed = FIELD_HALF_CELL_ORIGIN(FIELD_HALF_CELL(actor->z.fixed) | 1);
  Actor_PlaceMovedBlock();
  Event_End();
}

/// A pillar pushed or Moved off the edge of a ledge drops to the floor below
/// with a thump. In the room of pillars the two pillars from the ledge, set
/// one beside the other at its foot, make the steps up it.
void TundariaTower_SetDownPillar(void) {
  struct FieldState *event = Ram_HeapSlots->fieldState;
  struct Actor *actor;

  actor = event->movedBlock;

  Event_Start();
  Event_EndLastingPsynergy(0);
  if (Map_GetTerrainHeight(2, actor->x.fixed, actor->z.fixed) !=
      actor->y.fixed) {
    actor->layer = 2;
    actor->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
    Actor_WaitUntilLanded(actor);
    Sound_Play(SE_THUMP);
    Actor_WaitUntilLanded(actor);
    actor->motionFlags = 0;
    actor->layer = 0;
  }
  Actor_PlaceMovedBlock();
  if (gMainRoutineState.next == SCENE_TUNDARIA_TOWER_PILLAR_PUZZLE) {
    if (FIELD_HALF_CELL(actor->x.fixed) == 27)
      GameFlag_Set(FLAG_FIRST_LEDGE_PILLAR_SET);
    if (FIELD_HALF_CELL(actor->x.fixed) == 25 &&
        GameFlag_Get(FLAG_FIRST_LEDGE_PILLAR_SET))
      GameFlag_Set(FLAG_SECOND_LEDGE_PILLAR_SET);
  }
  Event_End();
}

/// When Reveal wears off, the way through the icy wall at the dead end beyond
/// the room of pillars closes again; Felix, if he stands in it, is put out on
/// the side he faces.
void TundariaTower_EndRevealHiddenWay(void) {
  BlendScript_Resume();
  GameFlag_Clear(FLAG_HIDDEN_WAY_REVEALED);
  // The icy wall's part of the map is entered by 6, from the Burst Brooch's
  // room, or 7.
  if ((u16)(gMainRoutineState.entrance - 6) <= 1) {
    MapGrid_CopyCellTilesIndicesRect(39, 100, 53, 100, 3, 3);
    MapGrid_CopyCellAttributes(43, 41, 3, 3, 53, 37);
    if (FIELD_CELL(Actor_GetById(gMainRoutineState.leaderActorId)->z.fixed) ==
        38) {
      s16 angle = Actor_GetById(gMainRoutineState.leaderActorId)->facing;
      s32 owner = gMainRoutineState.leaderActorId;

      if (angle < 0)
        Actor_SetPosition(owner, 872, 600);
      else
        Actor_SetPosition(owner, 872, 632);
    }
  }
}

/// Reveal at the icy wall at the dead end beyond the room of pillars shows a
/// way through it, cells (53, 37) to (55, 39).
void TundariaTower_RevealHiddenWay(void) {
  BlendScript_Suspend();
  GameFlag_Set(FLAG_HIDDEN_WAY_REVEALED);
  if ((u16)(gMainRoutineState.entrance - 6) <= 1) {
    MapGrid_CopyCellTilesIndicesRect(43, 100, 53, 100, 3, 3);
    MapGrid_CopyCellAttributes(43, 37, 3, 3, 53, 37);
    if (FIELD_CELL(Actor_GetById(gMainRoutineState.leaderActorId)->z.fixed) >
        38)
      MapCell_SetAttributes(
          0, 54, 37,
          CELL_ATTRIBUTES(CELL_BEHAVIOR_TUNDARIA_TOWER_SLIDE_END, 0));
    else
      MapCell_SetAttributes(
          0, 54, 39,
          CELL_ATTRIBUTES(CELL_BEHAVIOR_TUNDARIA_TOWER_SLIDE_END, 0));
  }
}

/// The cells where the ice ends in the room of pillars stop Felix sliding.
void TundariaTower_StopSliding(void) { Event_EndLastingPsynergy(0); }

/// Burst on the icy wall at the end of the left door's way, cell (7, 35), in
/// the room whose ice floor reflects Felix: chips of ice fly and it breaks
/// open onto the rooms of chests beyond.
void TundariaTower_BurstMirrorWall(s32 step) {
  struct Vector3 position;

  switch (step) {
  case 1:
    MapGrid_CopyCellTilesIndicesRect(18, 97, 6, 97, 3, 4);
    MapGrid_CopyCellTilesIndicesRect(82, 34, 70, 34, 3, 2);
    MapGrid_CopyCellAttributes(7, 36, 1, 1, 7, 35);
    GameFlag_Set(FLAG_MIRROR_WALL_BURST);
    break;
  case 2:
    position.x = toFIXED(120);
    position.y = 0;
    position.z = toFIXED(568);
    TundariaTower_ScatterIceChips(&position, 1, FIXED_ONE * 8 / 10);
    break;
  case 3:
    position.x = toFIXED(120);
    position.y = 0;
    position.z = toFIXED(568);
    TundariaTower_ScatterIceChips(&position, 8, FIXED_ONE * 3 / 2);
    break;
  }
}

/// As Felix starts to cast Move on a sliding pillar or Burst on the icy wall,
/// whatever he was doing, such as sliding, stops first.
void TundariaTower_ResolveBeforePsynergy(void) { Event_EndLastingPsynergy(0); }

/// Burst on the brown part of the outer wall in the Burst Brooch's room, from
/// the ground below it: rocks fall as it breaks open into a shortcut.
void TundariaTower_BurstBrownWall(s32 shake, s32 frame) {
  if (frame == 0) {
    TundariaTower_StartFallingRocks();
    Task_Remove(TundariaTower_UpdateBrownWallCell);
  }
  if (shake == 1) {
    MapGrid_CopyCellTilesIndicesRect(13, 26, 13, 16, 3, 3);
    MapGrid_CopyCellAttributes(13, 26, 3, 3, 13, 16);
    GameFlag_Set(FLAG_BROWN_WALL_BURST);
  }
  if (shake == 2)
    Rock_Spawn(toFIXED(232), toFIXED(16), toFIXED(280), 2);
  if (shake == 3)
    Rock_Spawn(toFIXED(232), toFIXED(16), toFIXED(280), 30);
  if (frame == FIELD_PSYNERGY_FINISHED)
    TundariaTower_EndFallingRocks();
}

const struct Trigger *Scene_GetTriggers(void) {
  if (gMainRoutineState.next == SCENE_TUNDARIA_TOWER_ENTRANCE)
    return gTundariaTowerEntranceTriggers;
  if (gMainRoutineState.next == SCENE_TUNDARIA_TOWER_CENTER_PRONG)
    return gTundariaTowerCenterProngTriggers;
  if (gMainRoutineState.next == SCENE_TUNDARIA_TOWER_ICE_SLIDE)
    return gTundariaTowerIceSlideTriggers;
  if (gMainRoutineState.next == SCENE_TUNDARIA_TOWER_REFLUX)
    return gTundariaTowerRefluxTriggers;
  if (gMainRoutineState.next == SCENE_TUNDARIA_TOWER_ICE_COLUMNS)
    return gTundariaTowerIceColumnsTriggers;
  if (gMainRoutineState.next == SCENE_TUNDARIA_TOWER_PILLAR_PUZZLE)
    return gTundariaTowerPillarPuzzleTriggers;
  if (gMainRoutineState.next == SCENE_TUNDARIA_TOWER_BURST_BROOCH)
    return gTundariaTowerBurstBroochTriggers;
  return gTundariaTowerCenterProngTriggers;
}

/// Felix's reflection in the ice floor of two rooms, the one before the icy
/// wall Burst breaks and the one before the icy wall Reveal opens, mirrored
/// across the floor's edge.
void TundariaTower_ReflectLeader(struct Actor *actor) {
  struct Actor *leader = Actor_GetById(gMainRoutineState.leaderActorId);
  struct Sprite *leaderSprite = leader->sprite;
  struct Sprite *reflection = actor->sprite;

  if (leader->z.fixed <= actor->sceneWord) {
    actor->z.fixed = 0;
    actor->x.fixed = 0;
    REG_BG2CNT &= (u16)~BGCNT_PRIORITY(3);
    REG_BG2CNT |= BGCNT_PRIORITY(1);
  } else {
    REG_BG2CNT &= (u16)~BGCNT_PRIORITY(3);
    REG_BG2CNT |= BGCNT_PRIORITY(2);
    if (GameFlag_Get(FLAG_HIDDEN_WAY_REVEALED)) {
      actor->z.fixed = 0;
      actor->x.fixed = 0;
    } else {
      struct SpriteAnimation *animation = reflection->entries[0];
      struct SpriteAnimation *source;

      actor->x.fixed = leader->x.fixed;
      actor->y.fixed = leader->y.fixed;
      actor->z.fixed = actor->sceneWord - (leader->z.fixed - actor->sceneWord);
      actor->facing = ~leader->facing;
      source = leaderSprite->entries[0];
      animation->script = source->script;
      animation->timer = source->timer;
      animation->scriptOffset = source->scriptOffset;
    }
  }
}

#include "../field/field_effect_advance_motion.c"

struct Actor;
struct Particle;
struct ParticleSprite;
union FieldObject;

void Leader_StopIfPending(void);

void Actor_SetCellOpen(s32 id, u32 kind, s32 flag, s32 layer);

s32 MapGrid_GetTerrainKind(s32 layer, s32 x, s32 z);

void TundariaTower_ScatterDust(s32 spread);
void Leader_SetCellShapeAt(s32 a, s32 b);

void TundariaTower_SpawnDustRing(void);

void Actor_PlaceOnActor(s32 id, s32 target);

struct Actor *gTundariaTower_Reflection BSS_DATA;
volatile s16 gSlideOnIce_Stopped BSS_DATA;

void TundariaTower_ParchLeftWater(void);
void TundariaTower_ParchRightWater(void);

extern s16 gSlideOnIce_SteerAngles[];
extern s16 gSlideOnIce_AnglesByKey[];

extern u32 gDust_ScatterScript[];

extern u32 gDust_BurstScript[];

void EffectSprite_DrawAtWorldPosition(struct ParticleSprite *sprite,
                                      struct Particle *particle);
void BattleEffect_StepVector3(struct Particle *particle, s32 damping,
                              s32 gravity);

void TundariaTower_ExamineCrystal(void);
void TundariaTower_SlideOnIce(void);
void TundariaTower_RefluxAppears(void);
void TundariaTower_PoundIceColumn(s32 phase, s32 step);
void TundariaTower_SetDownPillar(void);
void TundariaTower_EndRevealHiddenWay(void);
void TundariaTower_RevealHiddenWay(void);
void TundariaTower_StopSliding(void);
void TundariaTower_ResolveBeforePsynergy(void);

void FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                       s32 velocityZ, u32 flags,
                       const struct EffectOptions *options);

/// At the foot of a black ice slide, Felix drops in from above and lands in a
/// puff of dust with a thump that shakes the screen.
void TundariaTower_LandFromSlide(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineState.leaderActorId);
  struct EffectOptions options;
  s32 angle;
  u32 i;

  Event_Start();
  Event_EndLastingPsynergy(0);
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  MapGrid_CenterOnFollowedPoint();
  wait(1);
  leader->y.fixed = toFIXED(130);
  leader->gravity = toFIXED(1) / 2;
  leader->bounce = 0;
  leader->motionFlags = 0;
  gFieldState->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
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

    angle = i * (TRIG_TURN / DUST_RING_PUFFS);
    velocity[0] = Trig_Cos(angle);
    velocity[1] = 0;
    velocity[2] = Trig_Sin(angle);
    velocity[0] += velocity[0] >> 1;
    velocity[0] += ((Random16() * 3) << 14 >> 16) - toFIXED(3) / 8;
    velocity[2] += ((Random16() * 3) << 13 >> 16) - toFIXED(3) / 16;
    FieldEffect_Spawn(leader->x.fixed, leader->y.fixed, leader->z.fixed,
                      velocity[0], velocity[1], velocity[2],
                      EFFECT_USE_UPDATE | EFFECT_USE_START_SCALE |
                          EFFECT_USE_PALETTE | 1,
                      &options);
    i++;
  } while (i <= DUST_RING_PUFFS);
  Sound_Play(SE_THUMP);
  FieldEffect_SetAttached(gMainRoutineState.leaderActorId,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ATTACH);
  Actor_SetAnimationById(gMainRoutineState.leaderActorId,
                         ANIM_LEADER_LEAN_BACK);
  MapGrid_SetShake(5, 5, 1);
  MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
  MapGrid_WaitForShakeToSettle();
  FieldEffect_SetAttached(gMainRoutineState.leaderActorId,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_REMOVE);
  leader->gravity = toFIXED(1);
  leader->bounce = FIXED_ONE / 4;
  gFieldState->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
  Event_End();
}

/// Each frame in the Burst Brooch's room, the cell before the brown wall
/// answers Burst only while Felix stands on the ground below it, not on the
/// ladders above.
void TundariaTower_UpdateBrownWallCell(void) {
  if (FIELD_HALF_CELL(
          Actor_GetById(gMainRoutineState.leaderActorId)->y.fixed) != 0)
    MapCell_SetAttributes(MAPGRID_LAYER_GROUND, 14, 17, 0);
  else
    MapCell_SetAttributes(
        MAPGRID_LAYER_GROUND, 14, 17,
        CELL_ATTRIBUTES(CELL_BEHAVIOR_TUNDARIA_TOWER_BROWN_WALL, 0));
}

extern s16 gTundariaTowerIceColumnsColumns[];
extern s16 gTundariaTowerPillarPuzzleColumns[];
void Actor_PlaceBlocks(const s16 *data);

s32 Scene_Initialize(void) {
  struct Actor *actor;
  s32 scene;
  s32 flag;

  gFieldState->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
  scene = gMainRoutineState.next;
  if (scene == SCENE_TUNDARIA_TOWER_ENTRANCE) {
  } else if (scene == SCENE_TUNDARIA_TOWER_CENTER_PRONG) {
    switch (gMainRoutineState.entrance) {
    // The first hall, by its four ways in.
    case TUNDARIA_TOWER_CENTER_PRONG_FROM_TUNDARIA_TOWER_ENTRANCE:
    case CENTER_PRONG_HALL_NORTH_DOORWAY:
    case CENTER_PRONG_LEFT_POOL_DOOR:
    case CENTER_PRONG_RIGHT_POOL_DOOR:
      if (GameFlag_Get(FLAG_LEFT_WATER_PARCHED))
        TundariaTower_ParchLeftWater();
      if (GameFlag_Get(FLAG_RIGHT_WATER_PARCHED))
        TundariaTower_ParchRightWater();
      break;
    case CENTER_PRONG_CRYSTAL_CHAMBER:
      flag = GameFlag_Get(FLAG_CENTER_PRONG_TAKEN);
      if (flag == 0) {
        actor = Actor_GetById(ACTOR_FROZEN_CENTER_PRONG);
        Actor_SetSpriteFlags(Actor_GetById(ACTOR_FROZEN_CENTER_PRONG), 0);
        actor->collisionFlags = 0;
        actor->motionFlags = 0;
        actor->groundHeight = toFIXED(8);
        actor->y.fixed = toFIXED(8);
        Actor_SetItemIcon(actor, ITEM_CENTER_PRONG);
        if (GameFlag_Get(FLAG_CRYSTAL_BROKEN)) {
          MapGrid_CopyCellsTo(31, 85, 20, 85, 3, 4);
          MapGrid_CopyCellAttributes(11, 22, 3, 2, 20, 22);
          Actor_SetPosition(ACTOR_FALLEN_CENTER_PRONG, 344, 372);
          Actor_GetById(ACTOR_FALLEN_CENTER_PRONG)->update =
              (ObjectUpdate)TundariaTower_SparkleCenterProng;
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
    switch (gMainRoutineState.entrance) {
    // The sliding puzzle's part of the map.
    case REFLUX_PUZZLE_ROOM_DOORWAY:
    case REFLUX_FROM_SLIDE:
      if (!GameFlag_Get(FLAG_DJINNI(DJINNI_REFLUX))) {
        if (!GameFlag_Get(FLAG_REFLUX_APPEARED)) {
          Actor_SetPosition(ACTOR_REFLUX, 344, 536);
          MapCell_SetAttributes(
              MAPGRID_LAYER_GROUND, 21, 33,
              CELL_ATTRIBUTES(CELL_BEHAVIOR_TUNDARIA_TOWER_REFLUX_APPEARS, 0));
          MapCell_SetAttributes(MAPGRID_LAYER_GROUND, 22, 33,
                                CELL_ATTRIBUTES(CELL_BEHAVIOR_WALL, 0));
        } else if (!GameFlag_Get(FLAG_FIGHTING_REFLUX)) {
          MapCell_SetAttributes(MAPGRID_LAYER_GROUND, 26, 33,
                                CELL_ATTRIBUTES(CELL_BEHAVIOR_WALL, 0));
        }
        Actor_WatchAndSetScript(ACTOR_REFLUX, gMainRoutineState.leaderActorId);
      }
      if (!GameFlag_Get(FLAG_KEEP_PARTY_POSITION) &&
          gMainRoutineState.entrance == REFLUX_FROM_SLIDE)
        TundariaTower_LandFromSlide();
      break;
    }
  } else if (scene == SCENE_TUNDARIA_TOWER_ICE_COLUMNS) {
    TundariaTower_SetUpIceColumns(gTundariaTowerIceColumnsColumns);
  } else if (scene == SCENE_TUNDARIA_TOWER_PILLAR_PUZZLE) {
    switch (gMainRoutineState.entrance) {
    case TUNDARIA_TOWER_PILLAR_PUZZLE_FROM_TUNDARIA_TOWER_BURST_BROOCH_SOUTH:
      actor = FieldActor_Create(SPRITE_CRATE, toFIXED(472), 0, toFIXED(200));
      if (actor != NULL) {
        Scene_SetActorSpritePriority((s32)actor, 1);
        actor->motionFlags = 0;
        Actor_SetSpriteFlags(actor, 0);
        Actor_SetSpriteColor(actor, SPRITE_COLOR_BLACK);
        Actor_SetBlendMode(actor, 1);
      }
      break;
    case TUNDARIA_TOWER_PILLAR_PUZZLE_FROM_TUNDARIA_TOWER_ICE_COLUMNS_NORTH:
    case TUNDARIA_TOWER_PILLAR_PUZZLE_FROM_TUNDARIA_TOWER_ICE_COLUMNS_SOUTHWEST:
      TundariaTower_SetUpIceColumns(gTundariaTowerPillarPuzzleColumns);
      if (GameFlag_Get(FLAG_FIRST_LEDGE_PILLAR_SET)) {
        Actor_SetPosition(ACTOR_FIRST_LEDGE_PILLAR, 216, 136);
        if (GameFlag_Get(FLAG_SECOND_LEDGE_PILLAR_SET))
          Actor_SetPosition(ACTOR_SECOND_LEDGE_PILLAR, 200, 136);
      }
      Actor_PlaceBlocks(gTundariaTowerPillarPuzzlePillars);
      Actor_SetSpritePriority(ACTOR_SECOND_LEDGE_PILLAR, 2);
      break;
    case TUNDARIA_TOWER_PILLAR_PUZZLE_FROM_TUNDARIA_TOWER_CENTER_PRONG_NORTH:
    case TUNDARIA_TOWER_PILLAR_PUZZLE_FROM_TUNDARIA_TOWER_CENTER_PRONG_SOUTH:
      if (GameFlag_Get(FLAG_MIRROR_WALL_BURST)) {
        MapGrid_CopyCellsTo(18, 97, 6, 97, 3, 4);
        MapGrid_CopyCellsTo(82, 34, 70, 34, 3, 2);
        MapGrid_CopyCellAttributes(7, 36, 1, 1, 7, 35);
      }
      {
        /* FAKEMATCH(temporary): the reflection's global reached through a local
           pointer; as gTundariaTower_Reflection = effect. */
        struct Actor **slot = &gTundariaTower_Reflection;
        struct Actor *effect =
            ((struct Actor *)Import_ActorCreate(SPRITE_FELIX, 0, 0, 0));

        effect->sceneWord = toFIXED(576);
        effect->update = TundariaTower_ReflectLeader;
        *slot = effect;
        effect->motionFlags = 0;
        Scene_SetActorSpritePriority(effect, 3);
      }
      break;
    case TUNDARIA_TOWER_PILLAR_PUZZLE_FROM_TUNDARIA_TOWER_BURST_BROOCH_NORTH:
    case PILLAR_PUZZLE_MIRROR_ROOM_FOOT: {
      /* FAKEMATCH(temporary): the reflection's global reached through a local
         pointer; as gTundariaTower_Reflection = effect. */
      struct Actor **slot = &gTundariaTower_Reflection;
      struct Actor *effect =
          ((struct Actor *)Import_ActorCreate(SPRITE_FELIX, 0, 0, 0));

      effect->sceneWord = toFIXED(624);
      effect->update = TundariaTower_ReflectLeader;
      *slot = effect;
      effect->motionFlags = 0;
      Scene_SetActorSpritePriority(effect, 3);
    } break;
    case PILLAR_PUZZLE_FROM_SLIDE:
      if (!GameFlag_Get(FLAG_KEEP_PARTY_POSITION))
        TundariaTower_LandFromSlide();
      break;
    }
  } else if (scene == SCENE_TUNDARIA_TOWER_BURST_BROOCH) {
    GameFlag_Set(FLAG_RETREAT_BLOCKED);
    if (GameFlag_Get(FLAG_BROWN_WALL_BURST)) {
      MapGrid_CopyCellsTo(13, 26, 13, 16, 3, 3);
      MapGrid_CopyCellAttributes(13, 26, 3, 3, 13, 16);
    } else {
      Task_Add(TundariaTower_UpdateBrownWallCell, TASK_PASS_FRAME);
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
void Actor_SteerByPad(struct Actor *actor);
void Leader_StopIfPending(void);
s32 Actor_HasArrived(struct Actor *actor);

#include "shared/slide_on_ice_from_edge.c"

/// Stops the leader's slide on the ice where it is (nonzero), or lets it run.
void Leader_SetSlideStopped(s32 value) { gSlideOnIce_Stopped = value; }

#include "shared/leader_slide_on_ice_ahead.h"

/// Unused: places each listed stake at its cell, as Yampi Desert's do.
UNUSED void TundariaTower_PlaceStakes(u16 *table) {
  u16 *row;
  s32 id;
  struct Actor *actor;
  s16 x, z;

  if (table[0] == ACTOR_LIST_END)
    return;
  for (row = table; row[0] != ACTOR_LIST_END; row += 3) {
    id = row[0];
    actor = Actor_GetById(id);
    x = row[1];
    z = row[2];

    actor->collisionFlags &= ~ACTOR_SOLID;
    Actor_SetSpriteFlags(actor, 0);
    Actor_SetCellOpen(id, (u16)x, (u16)z, actor->layer);
  }
}

#include "shared/pound_sink_target.h"

#include "shared/actor_set_cell_open.h"

/// Unused: Magma Rock's crumbling pillars on entering: each listed pillar
/// from its two flags (fallen: moved away; else its cell raised and its shape
/// redrawn), then the leader lifted onto the ground under him.
UNUSED void TundariaTower_RestoreCrumblingPillars(const u16 *table) {
  struct MapState *map = Ram_HeapSlots->mapGrid;
  struct Actor *actor;
  s32 id;
  s32 flag;
  s32 nextFlag;
  struct MapCell *cells;
  u8 *shapes;
  struct MapCell *cell;
  s32 index;
  struct Actor *leader;
  s32 ground;

  if (*table != ACTOR_LIST_END) {
    for (; *table != ACTOR_LIST_END; table += 2) {
      id = table[0];
      actor = Actor_GetById(id);
      flag = table[1];
      nextFlag = flag + 1;
      cells = map->layers[actor->layer].cells;
      shapes = Ram_MapShapeGrid + (cells - (struct MapCell *)Ram_MapCellBuffer);
      if (GameFlag_Get(nextFlag)) {
        Actor_SetPosition(id, 0, 0);
      } else {
        s32 shape;
        u8 *slot;
        s32 height;

        index = Actor_GetCellIndex(actor);
        cell = &cells[index];
        slot = shapes + index;
        shape = MapGrid_GetTerrainKind(actor->layer, actor->x.fixed,
                                       actor->z.fixed);
        height =
            Map_GetTerrainHeight(actor->layer, actor->x.fixed, actor->z.fixed);
        Actor_DisableById(id);
        cell->attributeB = MAP_CELL_JUMP;
        *slot = MapShape_FindOrAdd(actor->layer, shape,
                                   FIELD_HALF_CELL(height) + 4);
        if (GameFlag_Get(flag))
          Actor_SelectAnimation(actor, 0);
      }
    }
  }
  leader = Actor_GetById(gMainRoutineState.leaderActorId);
  ground =
      Map_GetTerrainHeight(leader->layer, leader->x.fixed, leader->z.fixed);
  if (leader->y.fixed < ground) {
    leader->groundHeight = ground;
    leader->y.fixed = ground;
  }
}

/// Unused: Magma Rock's crumbling floor: a listed pillar stepped on flashes
/// the first time and crumbles under the leader the second.
UNUSED void TundariaTower_CheckCrumblingStep(const u16 *table) {
  struct Actor *leader;
  struct Actor *trap;
  FIXED position[3];
  s32 id;
  s32 flag;
  s32 second;

  if (Field_TryJumpForward() != 0)
    return;
  leader = Actor_GetById(gMainRoutineState.leaderActorId);
  position[0] = leader->x.fixed;
  position[1] = leader->y.fixed;
  position[2] = leader->z.fixed;
  trap = Actor_FindAtCell(position, 0);
  if (trap == 0)
    return;
  while (*table != ACTOR_LIST_END) {
    id = table[0];
    if (Actor_GetById(id) != trap) {
      table += 2;
      continue;
    }
    flag = table[1];
    Event_Start();
    if (!GameFlag_Get(flag)) {
      Sound_Play(SE_CHEST_CLOSE);
      Actor_SetSpriteColor(Actor_GetById(id), SPRITE_COLOR_WHITE);
      wait(2);
      Actor_SelectAnimation(trap, 0);
      Actor_SetSpriteColor(Actor_GetById(id), SPRITE_COLOR_NORMAL);
      wait(2);
      Actor_SetSpriteColor(Actor_GetById(id), SPRITE_COLOR_WHITE);
      wait(4);
      Actor_SetSpriteColor(Actor_GetById(id), SPRITE_COLOR_NORMAL);
      TundariaTower_ScatterDust(0);
      GameFlag_Set(flag);
    } else {
      second = flag + 1;
      if (!GameFlag_Get(second)) {
        leader->motionFlags = 0;
        Sound_Play(SE_PUSH);
        MapGrid_SetShake(1, 4, 1);
        TundariaTower_ScatterDust(0);
        wait(8);
        Actor_SelectAnimation(trap, 2);
        Leader_SetCellShapeAt(-2, trap->layer);
        TundariaTower_ScatterDust(1);
        wait(16);
        Leader_SetCellShapeAt(-2, trap->layer);
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
}

/// Unused: Magma Rock's crumbling pillar: the listed pillar shakes, crumbles
/// and falls away.
UNUSED void TundariaTower_CrumblePillar(u16 *row, s32 id) {
  struct Actor *actor = Actor_GetById(id);
  s32 flag;
  s32 nextFlag;

  if (*row == ACTOR_LIST_END)
    return;
  do {
    if (*row != id) {
      row += 2;
      continue;
    }
    flag = row[1];
    Event_Start();
    nextFlag = flag + 1;
    if (!GameFlag_Get(nextFlag)) {
      Sound_Play(SE_PUSH);
      MapGrid_SetShake(1, 4, 1);
      TundariaTower_ScatterDust(0);
      wait(8);
      Actor_SelectAnimation(actor, 2);
      Actor_SetCellShape(-4, actor->layer, id);
      TundariaTower_ScatterDust(1);
      wait(16);
      MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
      wait(8);
      Actor_SetPositionAndResetMotion(actor, 0, 0, 0);
      wait(2);
      Sound_Play(SE_THUMP);
      TundariaTower_SpawnDustRing();
      wait(20);
      GameFlag_Set(flag);
      GameFlag_Set(nextFlag);
    }
    Event_End();
    return;
  } while (*row != ACTOR_LIST_END);
}

/// Keeps the pillar's sprite cached and stops it.
void TundariaTower_FreezePillar(s32 id) {
  Actor_GetById(id)->retainSpriteCache =
      ACTOR_KEEP_SPRITE_VRAM | ACTOR_COMPOSE_OFF_SCREEN;
  Actor_DisableById(id);
}

/// Unused: Magma Rock's trap floor on entering: the first three placed actors
/// gather at the leader and are put away, then each listed (actor, flag) pair
/// is set from its two flags, as TundariaTower_RestoreCrumblingPillars does,
/// a flashed trap taking the second actor's pictures.
UNUSED void TundariaTower_RestorePillars(u16 *list) {
  struct MapState *map = Ram_HeapSlots->mapGrid;
  struct Actor *actor;
  s32 id;
  s32 flag;
  s32 nextFlag;
  struct MapCell *cells;
  u8 *shapes;
  struct MapCell *cell;
  s32 index;
  u8 *grid = gMapShapeGrid;

  Actor_PlaceOnActor(ACTOR_FIRST_PLACED, gMainRoutineState.leaderActorId);
  Actor_PlaceOnActor(ACTOR_FIRST_PLACED + 1, gMainRoutineState.leaderActorId);
  Actor_PlaceOnActor(ACTOR_FIRST_PLACED + 2, gMainRoutineState.leaderActorId);
  wait(1);
  TundariaTower_FreezePillar(ACTOR_FIRST_PLACED);
  TundariaTower_FreezePillar(ACTOR_FIRST_PLACED + 1);
  TundariaTower_FreezePillar(ACTOR_FIRST_PLACED + 2);
  Actor_SetAnimationById(ACTOR_FIRST_PLACED + 1, 0);
  wait(1);
  Actor_SetPosition(ACTOR_FIRST_PLACED, 0, 0);
  Actor_SetPosition(ACTOR_FIRST_PLACED + 1, 0, 0);
  Actor_SetPosition(ACTOR_FIRST_PLACED + 2, 0, 0);
  wait(1);
  if (*list != ACTOR_LIST_END) {
    for (; *list != ACTOR_LIST_END; list += 2) {
      id = list[0];
      actor = Actor_GetById(id);
      flag = list[1];
      nextFlag = flag + 1;
      cells = map->layers[actor->layer].cells;
      shapes = grid + (cells - (struct MapCell *)Ram_MapCellBuffer);
      actor->retainSpriteCache =
          ACTOR_KEEP_SPRITE_VRAM | ACTOR_COMPOSE_OFF_SCREEN;
      if (GameFlag_Get(nextFlag)) {
        Actor_SetPosition(id, 0, 0);
      } else {
        s32 shape;
        u8 *slot;
        s32 height;

        index = Actor_GetCellIndex(actor);
        cell = &cells[index];
        slot = shapes + index;
        shape = MapGrid_GetTerrainKind(actor->layer, actor->x.fixed,
                                       actor->z.fixed);
        height =
            Map_GetTerrainHeight(actor->layer, actor->x.fixed, actor->z.fixed);
        Actor_DisableById(id);
        cell->attributeB = MAP_CELL_JUMP;
        *slot = MapShape_FindOrAdd(actor->layer, shape,
                                   FIELD_HALF_CELL(height) + 4);
        if (GameFlag_Get(flag))
          Actor_CopyGraphics(id, ACTOR_FIRST_PLACED + 1);
      }
    }
  }
  wait(10);
}

#include "shared/actor_take_place_of.h"

/// Unused: Magma Rock's trap floor: a listed pillar stepped on flashes the
/// first time and gives way under the leader the second.
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
  leader = Actor_GetById(gMainRoutineState.leaderActorId);
  partner = Actor_GetById(ACTOR_FIRST_PLACED);
  position[0] = leader->x.fixed;
  position[1] = leader->y.fixed;
  position[2] = leader->z.fixed;
  trap = Actor_FindAtCell(position, 0);
  if (trap != 0) {
    while (*table != ACTOR_LIST_END) {
      id = table[0];
      if (Actor_GetById(id) != trap) {
        table += 2;
        continue;
      }
      flag = table[1];
      Event_Start();
      if (!GameFlag_Get(flag)) {
        Actor_SelectAnimation(partner, 1);
        Actor_TakePlaceOf(partner, trap);
        wait(1);
        Sound_Play(SE_CHEST_CLOSE);
        Actor_SetSpriteColor(Actor_GetById(ACTOR_FIRST_PLACED),
                             SPRITE_COLOR_WHITE);
        wait(2);
        Actor_SelectAnimation(partner, 0);
        Actor_CopyGraphics(id, ACTOR_FIRST_PLACED + 1);
        Actor_SetSpriteColor(Actor_GetById(ACTOR_FIRST_PLACED),
                             SPRITE_COLOR_NORMAL);
        wait(2);
        Actor_SetSpriteColor(Actor_GetById(ACTOR_FIRST_PLACED),
                             SPRITE_COLOR_WHITE);
        wait(4);
        Actor_SetSpriteColor(Actor_GetById(ACTOR_FIRST_PLACED),
                             SPRITE_COLOR_NORMAL);
        TundariaTower_ScatterDust(0);
        Actor_TakePlaceOf(trap, partner);
        wait(1);
        GameFlag_Set(flag);
      } else {
        second = flag + 1;
        if (!GameFlag_Get(second)) {
          Actor_SelectAnimation(partner, 0);
          Actor_TakePlaceOf(partner, trap);
          motion = &leader->motionFlags;
          wait(1);
          *motion = 0;
          Sound_Play(SE_PUSH);
          MapGrid_SetShake(1, 4, 1);
          TundariaTower_ScatterDust(0);
          wait(8);
          Actor_SelectAnimation(partner, 2);
          Leader_SetCellShapeAt(-2, trap->layer);
          TundariaTower_ScatterDust(1);
          wait(16);
          Leader_SetCellShapeAt(-2, trap->layer);
          wait(4);
          MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
          wait(8);
          *motion = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
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
    }
  }
}

#include "../field/field_effect_advance_motion_priority.c"

/// Scatters eight clouds of dust round the leader's feet, outwards when spread
/// is set.
void TundariaTower_ScatterDust(s32 spread) {
  struct Actor *leader = Actor_GetById(gMainRoutineState.leaderActorId);
  struct FieldEffect *effect;
  u32 frame = 0;
  u32 i;

  for (i = 0; i <= 7; i++) {
    {
      // Each piece starts 0 to 13 pixels lower than the leader.
      FIXED y = leader->y.fixed - ((Random_Below(14)) << 16);

      effect = (struct FieldEffect *)Import_ActorCreate(
          SPRITE_DUST_CLOUD, i * toFIXED(2) + leader->x.fixed - toFIXED(8),
          y - toFIXED(2), leader->z.fixed);
    }
    if (effect != 0) {
      frame = Sprite_ShareVramCacheEntry(effect->sprite, frame);
      effect->motionFlags = 0;
      effect->collisionFlags = 0;
      Actor_SetSpriteFlags(effect, 0);
      Actor_SelectAnimation(effect, 2);
      TundariaTower_StartEffectScript(effect, gDust_ScatterScript);
      effect->scaleY = effect->scaleX = FIXED_ONE * 7 / 10;
      effect->sprite->priority = 1;
      if (spread != 0) {
        effect->velocity.x =
            Iwram_MulFX(Trig_Cos(i * DEGtoANG(45)), FIXED_ONE * 6 / 5);
        effect->velocity.z = Trig_Sin(i * DEGtoANG(45));
      } else {
        effect->velocity.x = spread;
        effect->velocity.z = spread;
      }
      effect->velocity.y = -(((Random16() << 14) >> 16) + (toFIXED(3) / 8));
      effect->spin = ((Random16() << 9) >> 16) - (TRIG_TURN / 256);
      effect->scaleRateX = -(FIXED_ONE / 32);
      effect->scaleRateY = -(FIXED_ONE * 3 / 128);
      effect->update = FieldEffect_AdvanceMotionAtLeaderPriority;
    }
  }
}

/// Throws a ring of dust clouds out from the leader as something lands.
void TundariaTower_SpawnDustRing(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineState.leaderActorId);
  s32 state;
  u32 i;

  if (leader != NULL) {
    state = 0;
    for (i = 0; i <= DUST_RING_PUFFS; i++) {
      struct FieldEffect *effect = ((struct FieldEffect *)Import_ActorCreate(
          SPRITE_DUST_CLOUD, leader->x.fixed, leader->y.fixed,
          leader->z.fixed));

      if (effect != NULL) {
        s32 angle;

        state = Sprite_ShareVramCacheEntry(effect->sprite, state);
        effect->motionFlags = 0;
        effect->collisionFlags = 0;
        Actor_SetSpriteFlags(effect, 0);
        Actor_SelectAnimation(effect, 2);
        TundariaTower_StartEffectScript(effect, gDust_BurstScript);
        angle = i * (TRIG_TURN / DUST_RING_PUFFS);
        effect->velocity.x = Trig_Cos(angle);
        effect->velocity.y = 0;
        effect->velocity.z = Trig_Sin(angle);
        effect->velocity.x += effect->velocity.x >> 1;
        effect->velocity.x += ((Random16() * 3) << 14 >> 16) - toFIXED(3) / 8;
        effect->velocity.z += ((Random16() * 3) << 13 >> 16) - toFIXED(3) / 16;
        effect->spin = ((Random16() << 12) >> 16) - (TRIG_TURN / 32);
        effect->scaleRateX = 0;
        effect->scaleRateY = 0;
        effect->update = FieldEffect_AdvanceMotionAtLeaderPriority;
        effect->sprite->priority =
            ((struct FieldSprite *)leader->sprite)->priority;
      }
    }
  }
}

#include "shared/actor_set_cell_shape.h"

#include "shared/leader_set_cell_shape.h"

#include "shared/particle_update_falling.h"

// Spawns count rocks around (x, y, z), each pushed out at a random angle and
// thrown up and out along it.
#include "shared/rock_spawn.h"

/// The falling rocks' setup: loads their picture, sets up ROCK_COUNT small
/// sprites, marks each rock idle and starts the task that spawns them.
void TundariaTower_StartFallingRocks(void) {
  struct RockState *state =
      HeapSlot_Alloc(HEAP_SLOT_MENU, sizeof(struct RockState));
  s32 resource;
  s32 tile;
  struct Rock *rock;
  struct EffectSprite *sprite;
  s32 i;

  Decompress_TaggedLz(Resource_GetTableEntry(RESOURCE_FALLING_ROCK), state);
  resource = VramCache_FindFreeEntry();
  tile = Vram_Load(resource, sizeof(state->tiles), state);
  state->tileBase = tile;
  state->resource = resource;
  rock = state->rocks;
  for (i = 0; i < ROCK_COUNT; i++) {
    sprite = &state->sprites[i];
    EffectSprite_Init(sprite, 8, 8, OBJ_SIZE_16X16, tile);
    sprite->oam.fullColor = 1;
    sprite->oam.palette = 0;
    sprite->oam.priority = 0;
    sprite->depth = OAM_DEPTH_WINDOW;
    rock->timer = -1;
    rock++;
  }
  state->next = 0;
  Task_AddFrame(Particle_UpdateFalling, TASK_PASS_FRAME);
}

/// Stops the falling rocks and frees their pictures and state.
void TundariaTower_EndFallingRocks(void) {
  struct RockState *state = Ram_HeapSlots->menuState;

  Task_Remove(Particle_UpdateFalling);
  VramCache_FreeEntry(state->resource);
  HeapSlot_Release(HEAP_SLOT_MENU);
}

void TundariaTower_FightReflux(void);
void TundariaTower_OpenDoor(void);
void TundariaTower_FreeCenterProng(void);
void TundariaTower_JumpDownSlide(void);
void TundariaTower_StartPillarMove(void);

// clang-format off
#include "shared/field_effect_duration_scripts.c"

/// The blocks Move pushes, ended by -1: the pillar puzzle's four, and the one
/// in the Burst Brooch's room.
s16 gTundariaTowerPillarPuzzlePillars[] = {
    ACTOR_FIRST_SLIDING_PILLAR, ACTOR_SECOND_LEDGE_PILLAR,
    ACTOR_FIRST_LEDGE_PILLAR, ACTOR_SECOND_SLIDING_PILLAR, -1,
};
s16 gTundariaTowerBurstBroochPillar[] = {
    ACTOR_BROOCH_ROOM_PILLAR, -1,
};

#include "shared/slide_on_ice_angles.c"

#include "shared/dust_scatter_script.c"

#include "shared/dust_burst_script.c"

const s32 *gFieldEffect_DurationScripts[] = {
    gFieldEffect_ShortScript,
    gFieldEffect_MediumScript,
    gFieldEffect_LongScript,
};

/// The ice columns of the room of ice columns: each one's actor, its standing
/// column's animation (the flattened one follows it) and its Pound flag,
/// ending -1.
s16 gTundariaTowerIceColumnsColumns[] = {
    ACTOR_FIRST_ICE_COLUMN, 7, FLAG_ICE_COLUMN_POUNDED,
    ACTOR_FIRST_ICE_COLUMN + 1, 7, FLAG_ICE_COLUMN_POUNDED + 1,
    ACTOR_FIRST_ICE_COLUMN + 2, 7, FLAG_ICE_COLUMN_POUNDED + 2,
    ACTOR_FIRST_ICE_COLUMN + 3, 7, FLAG_ICE_COLUMN_POUNDED + 3,
    -1,
};

/// The ice columns of the room of pillars, as gTundariaTowerIceColumnsColumns.
s16 gTundariaTowerPillarPuzzleColumns[] = {
    ACTOR_FIRST_ICE_COLUMN, 7, FLAG_ICE_COLUMN_POUNDED,
    ACTOR_FIRST_ICE_COLUMN + 1, 7, FLAG_ICE_COLUMN_POUNDED + 1,
    ACTOR_FIRST_ICE_COLUMN + 2, 7, FLAG_ICE_COLUMN_POUNDED + 2,
    -1, 0,
};

/// Reflux turning round a sixteenth of a turn each frame as he comes down.
s32 gTundariaTower_RefluxSpinScript[] = {
    SC_SET(SW_FACING, 0),
    SC_LABEL(1),
    SC_WAIT(1),
    SC_ADD(SW_FACING, TRIG_TURN / 16),
    SC_LOOP(SC_FOREVER, SC_LABEL(1)),
};

#include "shared/scene_entrances_default.c"

struct SceneRegion gTundariaTowerPillarPuzzleExitRegions[] = {
    {816, 80, 528, 832, 96, 544, CONDITION_ALWAYS,
     TUNDARIA_TOWER_PILLAR_PUZZLE_TO_TUNDARIA_TOWER_BURST_BROOCH_NORTH},
    {SCENE_TABLE_END},
};

struct SceneRegion gTundariaTowerBurstBroochExitRegions[] = {
    {176, 15, 192, 192, 33, 208, CONDITION_ALWAYS,
     TUNDARIA_TOWER_BURST_BROOCH_TO_TUNDARIA_TOWER_PILLAR_PUZZLE_NORTH},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(TundariaTowerEntrance),
    SCENE_EXIT_UNLESS(TUNDARIA_TOWER_ENTRANCE_TO_WORLD_MAP, WorldMap, WORLD_MAP_FROM_TUNDARIA_TOWER_ENTRANCE, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(TUNDARIA_TOWER_ENTRANCE_TO_TUNDARIA_TOWER_CENTER_PRONG, TundariaTowerCenterProng, TUNDARIA_TOWER_CENTER_PRONG_FROM_TUNDARIA_TOWER_ENTRANCE, EXIT_FLAG_NONE),
    SCENE_EXITS(TundariaTowerCenterProng),
    SCENE_EXIT_UNLESS(TUNDARIA_TOWER_CENTER_PRONG_TO_TUNDARIA_TOWER_ENTRANCE, TundariaTowerEntrance, TUNDARIA_TOWER_ENTRANCE_FROM_TUNDARIA_TOWER_CENTER_PRONG, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(CENTER_PRONG_HALL_NORTH_DOORWAY, TundariaTowerCenterProng, CENTER_PRONG_CRYSTAL_CHAMBER, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(CENTER_PRONG_LEFT_POOL_DOOR, TundariaTowerCenterProng, CENTER_PRONG_WEST_ROOM_EAST_END, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(CENTER_PRONG_RIGHT_POOL_DOOR, TundariaTowerCenterProng, CENTER_PRONG_EAST_ROOM_WEST_END, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(TUNDARIA_TOWER_CENTER_PRONG_TO_TUNDARIA_TOWER_PILLAR_PUZZLE_SOUTH, TundariaTowerPillarPuzzle, TUNDARIA_TOWER_PILLAR_PUZZLE_FROM_TUNDARIA_TOWER_CENTER_PRONG_SOUTH, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(CENTER_PRONG_WEST_ROOM_NORTH_DOORWAY, TundariaTowerCenterProng, CENTER_PRONG_WEST_CHESTS_FRONT, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(CENTER_PRONG_WEST_ROOM_EAST_END, TundariaTowerCenterProng, CENTER_PRONG_LEFT_POOL_DOOR, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(CENTER_PRONG_EAST_ROOM_WEST_END, TundariaTowerCenterProng, CENTER_PRONG_RIGHT_POOL_DOOR, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(CENTER_PRONG_EAST_ROOM_NORTH_DOORWAY, TundariaTowerCenterProng, CENTER_PRONG_EAST_CHESTS_FRONT, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(TUNDARIA_TOWER_CENTER_PRONG_TO_TUNDARIA_TOWER_ICE_SLIDE, TundariaTowerIceSlide, TUNDARIA_TOWER_ICE_SLIDE_FROM_TUNDARIA_TOWER_CENTER_PRONG, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(CENTER_PRONG_CRYSTAL_CHAMBER, TundariaTowerCenterProng, CENTER_PRONG_HALL_NORTH_DOORWAY, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(TUNDARIA_TOWER_CENTER_PRONG_TO_TUNDARIA_TOWER_PILLAR_PUZZLE_NORTH, TundariaTowerPillarPuzzle, TUNDARIA_TOWER_PILLAR_PUZZLE_FROM_TUNDARIA_TOWER_CENTER_PRONG_NORTH, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(CENTER_PRONG_CORRIDOR_MIDDLE_PASSAGE, TundariaTowerCenterProng, CENTER_PRONG_WEST_CHESTS_BACK, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(CENTER_PRONG_CORRIDOR_EAST_PASSAGE, TundariaTowerCenterProng, CENTER_PRONG_EAST_CHESTS_BACK, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(CENTER_PRONG_WEST_CHESTS_BACK, TundariaTowerCenterProng, CENTER_PRONG_CORRIDOR_MIDDLE_PASSAGE, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(CENTER_PRONG_WEST_CHESTS_FRONT, TundariaTowerCenterProng, CENTER_PRONG_WEST_ROOM_NORTH_DOORWAY, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(CENTER_PRONG_EAST_CHESTS_BACK, TundariaTowerCenterProng, CENTER_PRONG_CORRIDOR_EAST_PASSAGE, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(CENTER_PRONG_EAST_CHESTS_FRONT, TundariaTowerCenterProng, CENTER_PRONG_EAST_ROOM_NORTH_DOORWAY, EXIT_FLAG_NONE),
    SCENE_EXITS(TundariaTowerIceSlide),
    SCENE_EXIT_UNLESS(ICE_SLIDE_HALL_WEST_FOOT, TundariaTowerIceSlide, ICE_SLIDE_WEST_ROOM_DOORWAY, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(ICE_SLIDE_HALL_EAST_FOOT, TundariaTowerIceSlide, ICE_SLIDE_EAST_ROOM_DOORWAY, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(ICE_SLIDE_WEST_ROOM_DOORWAY, TundariaTowerIceSlide, ICE_SLIDE_HALL_WEST_FOOT, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(ICE_SLIDE_WEST_ROOM_FOOT, TundariaTowerIceSlide, ICE_SLIDE_ICE_FLOOR_WEST_LADDER, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(TUNDARIA_TOWER_ICE_SLIDE_TO_TUNDARIA_TOWER_REFLUX, TundariaTowerReflux, TUNDARIA_TOWER_REFLUX_FROM_TUNDARIA_TOWER_ICE_SLIDE, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(ICE_SLIDE_STAIR_ROOM_FOOT, TundariaTowerIceSlide, ICE_SLIDE_ICE_FLOOR_MIDDLE_LADDER, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(ICE_SLIDE_EAST_ROOM_DOORWAY, TundariaTowerIceSlide, ICE_SLIDE_HALL_EAST_FOOT, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(ICE_SLIDE_EAST_ROOM_FOOT, TundariaTowerIceSlide, ICE_SLIDE_ICE_FLOOR_EAST_LADDER, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(TUNDARIA_TOWER_ICE_SLIDE_TO_TUNDARIA_TOWER_CENTER_PRONG, TundariaTowerCenterProng, TUNDARIA_TOWER_CENTER_PRONG_FROM_TUNDARIA_TOWER_ICE_SLIDE, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(ICE_SLIDE_ICE_FLOOR_WEST_LADDER, TundariaTowerIceSlide, ICE_SLIDE_WEST_ROOM_FOOT, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(ICE_SLIDE_ICE_FLOOR_MIDDLE_LADDER, TundariaTowerIceSlide, ICE_SLIDE_STAIR_ROOM_FOOT, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(ICE_SLIDE_ICE_FLOOR_EAST_LADDER, TundariaTowerIceSlide, ICE_SLIDE_EAST_ROOM_FOOT, EXIT_FLAG_NONE),
    SCENE_EXITS(TundariaTowerReflux),
    SCENE_EXIT_UNLESS(TUNDARIA_TOWER_REFLUX_TO_TUNDARIA_TOWER_ICE_COLUMNS_NORTHWEST, TundariaTowerIceColumns, TUNDARIA_TOWER_ICE_COLUMNS_FROM_TUNDARIA_TOWER_REFLUX_NORTHWEST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(TUNDARIA_TOWER_REFLUX_TO_TUNDARIA_TOWER_ICE_SLIDE, TundariaTowerIceSlide, TUNDARIA_TOWER_ICE_SLIDE_FROM_TUNDARIA_TOWER_REFLUX, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(TUNDARIA_TOWER_REFLUX_TO_TUNDARIA_TOWER_ICE_COLUMNS_NORTHEAST, TundariaTowerIceColumns, TUNDARIA_TOWER_ICE_COLUMNS_FROM_TUNDARIA_TOWER_REFLUX_NORTHEAST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(TUNDARIA_TOWER_REFLUX_TO_TUNDARIA_TOWER_ICE_COLUMNS_NORTH, TundariaTowerIceColumns, TUNDARIA_TOWER_ICE_COLUMNS_FROM_TUNDARIA_TOWER_REFLUX_NORTH, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(REFLUX_HALL_SOUTHWEST_FOOT, TundariaTowerReflux, REFLUX_PUZZLE_ROOM_DOORWAY, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(REFLUX_PUZZLE_ROOM_DOORWAY, TundariaTowerReflux, REFLUX_HALL_SOUTHWEST_FOOT, EXIT_FLAG_NONE),
    SCENE_EXITS(TundariaTowerIceColumns),
    SCENE_EXIT_UNLESS(ICE_COLUMNS_CHEST_ROOM_FOOT, TundariaTowerIceColumns, ICE_COLUMNS_NORTH_ROOM_EAST_DOORWAY, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(TUNDARIA_TOWER_ICE_COLUMNS_TO_TUNDARIA_TOWER_REFLUX_NORTHWEST, TundariaTowerReflux, TUNDARIA_TOWER_REFLUX_FROM_TUNDARIA_TOWER_ICE_COLUMNS_NORTHWEST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(ICE_COLUMNS_NORTH_ROOM_EAST_DOORWAY, TundariaTowerIceColumns, ICE_COLUMNS_CHEST_ROOM_FOOT, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(ICE_COLUMNS_HALL_WEST_DOORWAY, TundariaTowerIceColumns, ICE_COLUMNS_CORRIDOR_SOUTH_DOORWAY, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(TUNDARIA_TOWER_ICE_COLUMNS_TO_TUNDARIA_TOWER_REFLUX_NORTH, TundariaTowerReflux, TUNDARIA_TOWER_REFLUX_FROM_TUNDARIA_TOWER_ICE_COLUMNS_NORTH, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(TUNDARIA_TOWER_ICE_COLUMNS_TO_TUNDARIA_TOWER_REFLUX_NORTHEAST, TundariaTowerReflux, TUNDARIA_TOWER_REFLUX_FROM_TUNDARIA_TOWER_ICE_COLUMNS_NORTHEAST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(TUNDARIA_TOWER_ICE_COLUMNS_TO_TUNDARIA_TOWER_PILLAR_PUZZLE_NORTH, TundariaTowerPillarPuzzle, TUNDARIA_TOWER_PILLAR_PUZZLE_FROM_TUNDARIA_TOWER_ICE_COLUMNS_NORTH, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(ICE_COLUMNS_CORRIDOR_SOUTH_DOORWAY, TundariaTowerIceColumns, ICE_COLUMNS_HALL_WEST_DOORWAY, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(TUNDARIA_TOWER_ICE_COLUMNS_TO_TUNDARIA_TOWER_PILLAR_PUZZLE_SOUTHWEST, TundariaTowerPillarPuzzle, TUNDARIA_TOWER_PILLAR_PUZZLE_FROM_TUNDARIA_TOWER_ICE_COLUMNS_SOUTHWEST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(TUNDARIA_TOWER_ICE_COLUMNS_TO_TUNDARIA_TOWER_PILLAR_PUZZLE_SOUTH, TundariaTowerPillarPuzzle, TUNDARIA_TOWER_PILLAR_PUZZLE_FROM_TUNDARIA_TOWER_ICE_COLUMNS_SOUTH, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(TUNDARIA_TOWER_ICE_COLUMNS_TO_TUNDARIA_TOWER_REFLUX_SOUTH, TundariaTowerReflux, TUNDARIA_TOWER_REFLUX_FROM_TUNDARIA_TOWER_ICE_COLUMNS_SOUTH, EXIT_FLAG_NONE),
    SCENE_EXITS(TundariaTowerPillarPuzzle),
    SCENE_EXIT_UNLESS(TUNDARIA_TOWER_PILLAR_PUZZLE_TO_TUNDARIA_TOWER_ICE_COLUMNS_NORTH, TundariaTowerIceColumns, TUNDARIA_TOWER_ICE_COLUMNS_FROM_TUNDARIA_TOWER_PILLAR_PUZZLE_NORTH, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(TUNDARIA_TOWER_PILLAR_PUZZLE_TO_TUNDARIA_TOWER_ICE_COLUMNS_SOUTHWEST, TundariaTowerIceColumns, TUNDARIA_TOWER_ICE_COLUMNS_FROM_TUNDARIA_TOWER_PILLAR_PUZZLE_SOUTHWEST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(TUNDARIA_TOWER_PILLAR_PUZZLE_TO_TUNDARIA_TOWER_BURST_BROOCH_SOUTH, TundariaTowerBurstBrooch, TUNDARIA_TOWER_BURST_BROOCH_FROM_TUNDARIA_TOWER_PILLAR_PUZZLE_SOUTH, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(TUNDARIA_TOWER_PILLAR_PUZZLE_TO_TUNDARIA_TOWER_CENTER_PRONG_NORTH, TundariaTowerCenterProng, TUNDARIA_TOWER_CENTER_PRONG_FROM_TUNDARIA_TOWER_PILLAR_PUZZLE_NORTH, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(TUNDARIA_TOWER_PILLAR_PUZZLE_TO_TUNDARIA_TOWER_CENTER_PRONG_SOUTH, TundariaTowerCenterProng, TUNDARIA_TOWER_CENTER_PRONG_FROM_TUNDARIA_TOWER_PILLAR_PUZZLE_SOUTH, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(TUNDARIA_TOWER_PILLAR_PUZZLE_TO_TUNDARIA_TOWER_BURST_BROOCH_NORTH, TundariaTowerBurstBrooch, TUNDARIA_TOWER_BURST_BROOCH_FROM_TUNDARIA_TOWER_PILLAR_PUZZLE_NORTH, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(PILLAR_PUZZLE_MIRROR_ROOM_FOOT, TundariaTowerPillarPuzzle, PILLAR_PUZZLE_CORRIDOR_DOORWAY, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(PILLAR_PUZZLE_CORRIDOR_DOORWAY, TundariaTowerPillarPuzzle, PILLAR_PUZZLE_MIRROR_ROOM_FOOT, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(TUNDARIA_TOWER_PILLAR_PUZZLE_TO_TUNDARIA_TOWER_ICE_COLUMNS_SOUTH, TundariaTowerIceColumns, TUNDARIA_TOWER_ICE_COLUMNS_FROM_TUNDARIA_TOWER_PILLAR_PUZZLE_SOUTH, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(PILLAR_PUZZLE_FROM_SLIDE, TundariaTowerPillarPuzzle, PILLAR_PUZZLE_FROM_SLIDE, EXIT_FLAG_NONE),
    SCENE_EXITS(TundariaTowerBurstBrooch),
    SCENE_EXIT_UNLESS(TUNDARIA_TOWER_BURST_BROOCH_TO_TUNDARIA_TOWER_PILLAR_PUZZLE_NORTH, TundariaTowerPillarPuzzle, TUNDARIA_TOWER_PILLAR_PUZZLE_FROM_TUNDARIA_TOWER_BURST_BROOCH_NORTH, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(TUNDARIA_TOWER_BURST_BROOCH_TO_TUNDARIA_TOWER_PILLAR_PUZZLE_SOUTH, TundariaTowerPillarPuzzle, TUNDARIA_TOWER_PILLAR_PUZZLE_FROM_TUNDARIA_TOWER_BURST_BROOCH_SOUTH, EXIT_FLAG_NONE),
    SCENE_EXITS_END,
};

struct ActorPlacement gTundariaTowerCenterProngActorPlacements[] = {
    {SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, 1, 1, 0, 0, FACING_EAST, 2, 2},
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
      {SPRITE_STAKE, CONDITION_ALWAYS, 1, 1, 0, 0, FACING_SOUTH, 2, 2},                
      {SPRITE_STAKE, CONDITION_ALWAYS, 1, 2, 0, 0, FACING_SOUTH, 2, 2},                
      {SPRITE_STAKE, CONDITION_ALWAYS, 1, 3, 0, 0, FACING_SOUTH, 2, 2},                
      {SPRITE_STAKE, CONDITION_ALWAYS, 1, 4, 0, 0, FACING_SOUTH, 2, 2},                
      {SCENE_TABLE_END},                                                       
  };

struct ActorPlacement gTundariaTowerPillarPuzzleActorPlacements[] = {                                             
      {SPRITE_STAKE, CONDITION_ALWAYS, 1, 1, 0, 0, FACING_SOUTH, 2, 2},                
      {SPRITE_STAKE, CONDITION_ALWAYS, 1, 2, 0, 0, FACING_SOUTH, 2, 2},                
      {SPRITE_STAKE, CONDITION_ALWAYS, 1, 3, 0, 0, FACING_SOUTH, 2, 2},                
      {SPRITE_STONE_PILLAR, CONDITION_ALWAYS, 1, 4, 0, 0, FACING_SOUTH, 2, 2},                
      {SPRITE_STONE_PILLAR, CONDITION_ALWAYS, 1, 5, 0, 0, FACING_SOUTH, 2, 2},                
      {SPRITE_STONE_PILLAR, CONDITION_ALWAYS, 1, 6, 0, 0, FACING_SOUTH, 2, 2},                
      {SPRITE_STONE_PILLAR, CONDITION_ALWAYS, 1, 7, 0, 0, FACING_SOUTH, 2, 2},                
      {SCENE_TABLE_END},                                                       
  };

struct ActorPlacement gTundariaTowerBurstBroochActorPlacements[] = {
    {SPRITE_STONE_PILLAR, CONDITION_ALWAYS, 1, 2, 0, 0, FACING_SOUTH, 2, 2},
    {SCENE_TABLE_END},
};

struct Trigger gTundariaTowerEntranceTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, TUNDARIA_TOWER_ENTRANCE_TO_WORLD_MAP},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, TUNDARIA_TOWER_ENTRANCE_TO_TUNDARIA_TOWER_CENTER_PRONG},
    {TRIGGER_TABLE_END},
};
// clang-format on

struct Trigger gTundariaTowerCenterProngTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS,
     TUNDARIA_TOWER_CENTER_PRONG_TO_TUNDARIA_TOWER_ENTRANCE},
    {TRIGGER_EXIT, CENTER_PRONG_HALL_NORTH_DOORWAY, CONDITION_ALWAYS,
     CENTER_PRONG_HALL_NORTH_DOORWAY},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH),
     CELL_BEHAVIOR_TUNDARIA_TOWER_LEFT_DOOR, CONDITION_ALWAYS,
     EVENT(TundariaTower_OpenDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH),
     CELL_BEHAVIOR_TUNDARIA_TOWER_RIGHT_DOOR, CONDITION_ALWAYS,
     EVENT(TundariaTower_OpenDoor)},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS,
     TUNDARIA_TOWER_CENTER_PRONG_TO_TUNDARIA_TOWER_PILLAR_PUZZLE_SOUTH},
    {TRIGGER_EXIT, CENTER_PRONG_WEST_ROOM_NORTH_DOORWAY, CONDITION_ALWAYS,
     CENTER_PRONG_WEST_ROOM_NORTH_DOORWAY},
    {TRIGGER_EXIT, CENTER_PRONG_WEST_ROOM_EAST_END, CONDITION_ALWAYS,
     CENTER_PRONG_WEST_ROOM_EAST_END},
    {TRIGGER_EXIT, CENTER_PRONG_EAST_ROOM_WEST_END, CONDITION_ALWAYS,
     CENTER_PRONG_EAST_ROOM_WEST_END},
    {TRIGGER_EXIT, CENTER_PRONG_EAST_ROOM_NORTH_DOORWAY, CONDITION_ALWAYS,
     CENTER_PRONG_EAST_ROOM_NORTH_DOORWAY},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 10, CONDITION_ALWAYS,
     TUNDARIA_TOWER_CENTER_PRONG_TO_TUNDARIA_TOWER_ICE_SLIDE},
    {TRIGGER_EXIT, CENTER_PRONG_CRYSTAL_CHAMBER, CONDITION_ALWAYS,
     CENTER_PRONG_CRYSTAL_CHAMBER},
    {TRIGGER_EXIT, 12, CONDITION_ALWAYS,
     TUNDARIA_TOWER_CENTER_PRONG_TO_TUNDARIA_TOWER_PILLAR_PUZZLE_NORTH},
    {TRIGGER_EXIT, CENTER_PRONG_CORRIDOR_MIDDLE_PASSAGE, CONDITION_ALWAYS,
     CENTER_PRONG_CORRIDOR_MIDDLE_PASSAGE},
    {TRIGGER_EXIT, CENTER_PRONG_CORRIDOR_EAST_PASSAGE, CONDITION_ALWAYS,
     CENTER_PRONG_CORRIDOR_EAST_PASSAGE},
    {TRIGGER_EXIT, CENTER_PRONG_WEST_CHESTS_BACK, CONDITION_ALWAYS,
     CENTER_PRONG_WEST_CHESTS_BACK},
    {TRIGGER_EXIT, CENTER_PRONG_WEST_CHESTS_FRONT, CONDITION_ALWAYS,
     CENTER_PRONG_WEST_CHESTS_FRONT},
    {TRIGGER_EXIT, CENTER_PRONG_EAST_CHESTS_BACK, CONDITION_ALWAYS,
     CENTER_PRONG_EAST_CHESTS_BACK},
    {TRIGGER_EXIT, CENTER_PRONG_EAST_CHESTS_FRONT, CONDITION_ALWAYS,
     CENTER_PRONG_EAST_CHESTS_FRONT},
    {TRIGGER_EXAMINE, CELL_BEHAVIOR_TUNDARIA_TOWER_CRYSTAL, CONDITION_ALWAYS,
     EVENT(TundariaTower_ExamineCrystal)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_PARCH, PSYNERGY_PHASE_ACTS),
     CELL_BEHAVIOR_TUNDARIA_TOWER_LEFT_POOL, FLAG_LEFT_WATER_PARCHED,
     EVENT(TundariaTower_ParchLeftWater)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_PARCH, PSYNERGY_PHASE_ACTS),
     CELL_BEHAVIOR_TUNDARIA_TOWER_RIGHT_POOL, FLAG_RIGHT_WATER_PARCHED,
     EVENT(TundariaTower_ParchRightWater)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS),
     CELL_BEHAVIOR_TUNDARIA_TOWER_CRYSTAL, FLAG_CRYSTAL_BROKEN,
     EVENT(TundariaTower_BurstCrystal)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_AFTER_CAST),
     CELL_BEHAVIOR_TUNDARIA_TOWER_CRYSTAL, FLAG_CRYSTAL_BROKEN,
     EVENT(TundariaTower_FreeCenterProng)},
    {TRIGGER_TABLE_END},
};

struct Trigger gTundariaTowerIceSlideTriggers[] = {
    {TRIGGER_EXIT, ICE_SLIDE_HALL_WEST_FOOT, CONDITION_ALWAYS,
     ICE_SLIDE_HALL_WEST_FOOT},
    {TRIGGER_EXIT, ICE_SLIDE_HALL_EAST_FOOT, CONDITION_ALWAYS,
     ICE_SLIDE_HALL_EAST_FOOT},
    {TRIGGER_EXIT, ICE_SLIDE_WEST_ROOM_DOORWAY, CONDITION_ALWAYS,
     ICE_SLIDE_WEST_ROOM_DOORWAY},
    {TRIGGER_EXIT, ICE_SLIDE_WEST_ROOM_FOOT, CONDITION_ALWAYS,
     ICE_SLIDE_WEST_ROOM_FOOT},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 5, CONDITION_ALWAYS,
     TUNDARIA_TOWER_ICE_SLIDE_TO_TUNDARIA_TOWER_REFLUX},
    {TRIGGER_EXIT, ICE_SLIDE_STAIR_ROOM_FOOT, CONDITION_ALWAYS,
     ICE_SLIDE_STAIR_ROOM_FOOT},
    {TRIGGER_EXIT, ICE_SLIDE_EAST_ROOM_DOORWAY, CONDITION_ALWAYS,
     ICE_SLIDE_EAST_ROOM_DOORWAY},
    {TRIGGER_EXIT, ICE_SLIDE_EAST_ROOM_FOOT, CONDITION_ALWAYS,
     ICE_SLIDE_EAST_ROOM_FOOT},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 9, CONDITION_ALWAYS,
     TUNDARIA_TOWER_ICE_SLIDE_TO_TUNDARIA_TOWER_CENTER_PRONG},
    {TRIGGER_EXIT, ICE_SLIDE_ICE_FLOOR_WEST_LADDER, CONDITION_ALWAYS,
     ICE_SLIDE_ICE_FLOOR_WEST_LADDER},
    {TRIGGER_EXIT, ICE_SLIDE_ICE_FLOOR_MIDDLE_LADDER, CONDITION_ALWAYS,
     ICE_SLIDE_ICE_FLOOR_MIDDLE_LADDER},
    {TRIGGER_EXIT, ICE_SLIDE_ICE_FLOOR_EAST_LADDER, CONDITION_ALWAYS,
     ICE_SLIDE_ICE_FLOOR_EAST_LADDER},
    {TRIGGER_STEP, CELL_BEHAVIOR_TUNDARIA_TOWER_ICE, CONDITION_ALWAYS,
     EVENT(TundariaTower_SlideOnIce)},
    {TRIGGER_TABLE_END},
};

struct Trigger gTundariaTowerRefluxTriggers[] = {
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 1, CONDITION_ALWAYS,
     TUNDARIA_TOWER_REFLUX_TO_TUNDARIA_TOWER_ICE_COLUMNS_NORTHWEST},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 2, CONDITION_ALWAYS,
     TUNDARIA_TOWER_REFLUX_TO_TUNDARIA_TOWER_ICE_SLIDE},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 3, CONDITION_ALWAYS,
     TUNDARIA_TOWER_REFLUX_TO_TUNDARIA_TOWER_ICE_COLUMNS_NORTHEAST},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 4, CONDITION_ALWAYS,
     TUNDARIA_TOWER_REFLUX_TO_TUNDARIA_TOWER_ICE_COLUMNS_NORTH},
    {TRIGGER_EXIT, REFLUX_HALL_SOUTHWEST_FOOT, CONDITION_ALWAYS,
     REFLUX_HALL_SOUTHWEST_FOOT},
    {TRIGGER_EXIT, REFLUX_PUZZLE_ROOM_DOORWAY, CONDITION_ALWAYS,
     REFLUX_PUZZLE_ROOM_DOORWAY},
    {TRIGGER_STEP, CELL_BEHAVIOR_TUNDARIA_TOWER_ICE, CONDITION_ALWAYS,
     EVENT(TundariaTower_SlideOnIce)},
    {TRIGGER_STEP, CELL_BEHAVIOR_TUNDARIA_TOWER_REFLUX_APPEARS,
     FLAG_REFLUX_APPEARED, EVENT(TundariaTower_RefluxAppears)},
    {TRIGGER_TALK, ACTOR_REFLUX, CONDITION_ALWAYS,
     EVENT(TundariaTower_FightReflux)},
    {TRIGGER_TABLE_END},
};

struct Trigger gTundariaTowerIceColumnsTriggers[] = {
    {TRIGGER_EXIT, ICE_COLUMNS_CHEST_ROOM_FOOT, CONDITION_ALWAYS,
     ICE_COLUMNS_CHEST_ROOM_FOOT},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 2, CONDITION_ALWAYS,
     TUNDARIA_TOWER_ICE_COLUMNS_TO_TUNDARIA_TOWER_REFLUX_NORTHWEST},
    {TRIGGER_EXIT, ICE_COLUMNS_NORTH_ROOM_EAST_DOORWAY, CONDITION_ALWAYS,
     ICE_COLUMNS_NORTH_ROOM_EAST_DOORWAY},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, ICE_COLUMNS_HALL_WEST_DOORWAY,
     CONDITION_ALWAYS, ICE_COLUMNS_HALL_WEST_DOORWAY},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 5, CONDITION_ALWAYS,
     TUNDARIA_TOWER_ICE_COLUMNS_TO_TUNDARIA_TOWER_REFLUX_NORTH},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 6, CONDITION_ALWAYS,
     TUNDARIA_TOWER_ICE_COLUMNS_TO_TUNDARIA_TOWER_REFLUX_NORTHEAST},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS,
     TUNDARIA_TOWER_ICE_COLUMNS_TO_TUNDARIA_TOWER_PILLAR_PUZZLE_NORTH},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, ICE_COLUMNS_CORRIDOR_SOUTH_DOORWAY,
     CONDITION_ALWAYS, ICE_COLUMNS_CORRIDOR_SOUTH_DOORWAY},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS,
     TUNDARIA_TOWER_ICE_COLUMNS_TO_TUNDARIA_TOWER_PILLAR_PUZZLE_SOUTHWEST},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 10, CONDITION_ALWAYS,
     TUNDARIA_TOWER_ICE_COLUMNS_TO_TUNDARIA_TOWER_PILLAR_PUZZLE_SOUTH},
    {TRIGGER_STEP, CELL_BEHAVIOR_TUNDARIA_TOWER_SLIDE_TOP, CONDITION_ALWAYS,
     EVENT(TundariaTower_JumpDownSlide)},
    {TRIGGER_STEP, CELL_BEHAVIOR_TUNDARIA_TOWER_ICE, CONDITION_ALWAYS,
     EVENT(TundariaTower_SlideOnIce)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS),
     ACTOR_FIRST_ICE_COLUMN, FLAG_ICE_COLUMN_POUNDED,
     EVENT(TundariaTower_PoundIceColumn)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS),
     ACTOR_FIRST_ICE_COLUMN + 1, FLAG_ICE_COLUMN_POUNDED + 1,
     EVENT(TundariaTower_PoundIceColumn)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS),
     ACTOR_FIRST_ICE_COLUMN + 2, FLAG_ICE_COLUMN_POUNDED + 2,
     EVENT(TundariaTower_PoundIceColumn)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS),
     ACTOR_FIRST_ICE_COLUMN + 3, FLAG_ICE_COLUMN_POUNDED + 3,
     EVENT(TundariaTower_PoundIceColumn)},
    {TRIGGER_TABLE_END},
};

struct Trigger gTundariaTowerPillarPuzzleTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS,
     TUNDARIA_TOWER_PILLAR_PUZZLE_TO_TUNDARIA_TOWER_ICE_COLUMNS_NORTH},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS,
     TUNDARIA_TOWER_PILLAR_PUZZLE_TO_TUNDARIA_TOWER_ICE_COLUMNS_SOUTHWEST},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS,
     TUNDARIA_TOWER_PILLAR_PUZZLE_TO_TUNDARIA_TOWER_BURST_BROOCH_SOUTH},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS,
     TUNDARIA_TOWER_PILLAR_PUZZLE_TO_TUNDARIA_TOWER_CENTER_PRONG_NORTH},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS,
     TUNDARIA_TOWER_PILLAR_PUZZLE_TO_TUNDARIA_TOWER_CENTER_PRONG_SOUTH},
    {TRIGGER_EXIT, PILLAR_PUZZLE_MIRROR_ROOM_FOOT, CONDITION_ALWAYS,
     PILLAR_PUZZLE_MIRROR_ROOM_FOOT},
    {TRIGGER_EXIT, PILLAR_PUZZLE_CORRIDOR_DOORWAY, CONDITION_ALWAYS,
     PILLAR_PUZZLE_CORRIDOR_DOORWAY},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 9, CONDITION_ALWAYS,
     TUNDARIA_TOWER_PILLAR_PUZZLE_TO_TUNDARIA_TOWER_ICE_COLUMNS_SOUTH},
    {TRIGGER_EXIT, PILLAR_PUZZLE_SLIDE_HOLE, CONDITION_ALWAYS,
     PILLAR_PUZZLE_FROM_SLIDE},
    {TRIGGER_STEP, CELL_BEHAVIOR_TUNDARIA_TOWER_SLIDE_END, CONDITION_ALWAYS,
     EVENT(TundariaTower_StopSliding)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS),
     ACTOR_FIRST_ICE_COLUMN, FLAG_ICE_COLUMN_POUNDED,
     EVENT(TundariaTower_PoundIceColumn)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS),
     ACTOR_FIRST_ICE_COLUMN + 1, FLAG_ICE_COLUMN_POUNDED + 1,
     EVENT(TundariaTower_PoundIceColumn)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS),
     ACTOR_FIRST_ICE_COLUMN + 2, FLAG_ICE_COLUMN_POUNDED + 2,
     EVENT(TundariaTower_PoundIceColumn)},
    {TRIGGER_PUSH, 0, CONDITION_ALWAYS, EVENT(TundariaTower_StartPillarMove)},
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS, EVENT(TundariaTower_SetDownPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_FIRST),
     ACTOR_FIRST_SLIDING_PILLAR, CONDITION_ALWAYS,
     EVENT(TundariaTower_ResolveBeforePsynergy)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST),
     ACTOR_FIRST_SLIDING_PILLAR, CONDITION_ALWAYS,
     EVENT(TundariaTower_StartPillarMove)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_ACTS),
     ACTOR_FIRST_SLIDING_PILLAR, CONDITION_ALWAYS,
     EVENT(TundariaTower_SlidePillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST),
     ACTOR_SECOND_LEDGE_PILLAR, FLAG_SECOND_LEDGE_PILLAR_SET,
     EVENT(TundariaTower_StartPillarMove)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_SECOND_LEDGE_PILLAR,
     FLAG_SECOND_LEDGE_PILLAR_SET, EVENT(TundariaTower_SetDownPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST),
     ACTOR_FIRST_LEDGE_PILLAR, FLAG_FIRST_LEDGE_PILLAR_SET,
     EVENT(TundariaTower_StartPillarMove)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_LEDGE_PILLAR,
     FLAG_FIRST_LEDGE_PILLAR_SET, EVENT(TundariaTower_SetDownPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_FIRST),
     ACTOR_SECOND_SLIDING_PILLAR, CONDITION_ALWAYS,
     EVENT(TundariaTower_ResolveBeforePsynergy)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST),
     ACTOR_SECOND_SLIDING_PILLAR, CONDITION_ALWAYS,
     EVENT(TundariaTower_StartPillarMove)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_ACTS),
     ACTOR_SECOND_SLIDING_PILLAR, CONDITION_ALWAYS,
     EVENT(TundariaTower_SlidePillar)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0,
     CONDITION_ALWAYS, EVENT(TundariaTower_RevealHiddenWay)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0,
     CONDITION_ALWAYS, EVENT(TundariaTower_EndRevealHiddenWay)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_FIRST),
     CELL_BEHAVIOR_TUNDARIA_TOWER_ICY_WALL, CONDITION_ALWAYS,
     EVENT(TundariaTower_ResolveBeforePsynergy)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS),
     CELL_BEHAVIOR_TUNDARIA_TOWER_ICY_WALL, FLAG_MIRROR_WALL_BURST,
     EVENT(TundariaTower_BurstMirrorWall)},
    {TRIGGER_TABLE_END},
};

struct Trigger gTundariaTowerBurstBroochTriggers[] = {
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS,
     TUNDARIA_TOWER_BURST_BROOCH_TO_TUNDARIA_TOWER_PILLAR_PUZZLE_SOUTH},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST),
     ACTOR_BROOCH_ROOM_PILLAR, CONDITION_ALWAYS,
     EVENT(TundariaTower_StartPillarMove)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_BROOCH_ROOM_PILLAR,
     CONDITION_ALWAYS, EVENT(TundariaTower_SetDownPillar)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS),
     CELL_BEHAVIOR_TUNDARIA_TOWER_BROWN_WALL, FLAG_BROWN_WALL_BURST,
     EVENT(TundariaTower_BurstBrownWall)},
    {TRIGGER_TABLE_END},
};
#undef EFFECT_SET_MODE
