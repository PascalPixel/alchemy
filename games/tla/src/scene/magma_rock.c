/// @file
/// Magma Rock's outside: the climb up the mountain across the river from
/// Naribwe, past yellow statues that answer Burst, to the great fire on its
/// top that hides the way down into it. Felix comes here for a Magma Ball to
/// load Loho's cannon.
///
/// At the foot (the slide, `SceneId_MagmaRockSlide`) Lift moves a boulder off
/// the path, and the cave mouth above it, between two statues throwing a
/// fireball back and forth, leads a little way inside. Felix then climbs the
/// wall by the cave mouth. Burst on the statue on the ledge above makes it
/// spit a fireball that breaks a cracked pillar; pushing two stone pillars
/// opens the climb, and Burst on the statue further west makes its cap rise
/// on a column of flame, carrying Felix up to the next ledge.
///
/// The cliff above (`SceneId_MagmaRockSalamanderTailChest`, with the Salamander
/// Tail's chest) is four screens on one map, which the code calls levels and
/// tells apart by the entrance Felix came in by: the low east and low west
/// halves of the cliff, then the high west and high east halves above them.
/// Growth grows vines up its walls, Lash ties a rope, two cracked pillars
/// crumble under Felix's feet, more statues spit fire or lift him, and on the
/// high east wall three pairs of statues throw fireballs that knock him off it.
/// At the top (`SceneId_MagmaRockStatue`) Burst on the statue east of the great
/// fire makes it spit a fireball into the fire, and the blast opens the way
/// into Magma Rock.
#include "resource_ids.h"
#include "../field/actor_set_sprite_priority.c"

#include "../field/actor_prepare.c"

#include "types.h"
#include "flags.h"
#include "system/ram_buffer.h"
#include "system/iwram_call.h"
#include "field/field_sequence.h"
#include "field/actor.h"
#include "field/event_inline.h"
#include "scene/scene.h"
#include "system/main_routine_work.h"
#include "field/field_coordinate.h"
#include "system/scene.h"
#include "field/map_grid.h"
#include "field/field.h"
#include "field/event_commands.h"
#include "scene/magma_rock.h"
#include "battle/battle.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "system/math_table.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "field/scene_tables.h"
#include "scene_ids.h"
#include "system/frame.h"
#include "field/event.h"
#include "field/field_sequence_event.h"
#include "field/flag_call.h"
#include "system/field_effect.h"
#include "macros.h"
#include "field/call.h"
#include "fixed_math.h"
#include "scene/falling_rocks.h"
#include "field/sprite.h"
#include "field/object.h"
#include "scene/mars_lighthouse_star_room.h"
#include "field/field_sprite.h"
#include "graphics/resource.h"
#include "system/heap_release.h"
#include "field/field_effect.h"
#include "field/field_psynergy_work.h"
#include "field/particle.h"
#include "system/task.h"
#include "field/actor_script.h"
#include "text/msg_ids.h"

/// Each pushed pillar's flag keeps it where it was pushed: two on the slide,
/// one on each of the cliff's high west, high east and low west levels and two
/// on its low east level.
#define FLAG_SLIDE_PILLAR_A_PUSHED 0x8d5
#define FLAG_SLIDE_PILLAR_B_PUSHED 0x8d6
#define FLAG_HIGH_WEST_PILLAR_B_PUSHED 0x8d7
#define FLAG_HIGH_EAST_PILLAR_PUSHED 0x8d8
#define FLAG_LOW_WEST_PILLAR_PUSHED 0x8d9
#define FLAG_LOW_EAST_PILLAR_A_PUSHED 0x8da
#define FLAG_LOW_EAST_PILLAR_B_PUSHED 0x8db
/// The top's great fire is put out and the rocks round it scattered,
/// which opens the way into Magma Rock (MagmaRock_ScatterEmbers).
#define FLAG_WAY_IN_BLASTED 0x8dc
/// Set as Burst breaks each cracked pillar, until the scene is left.
#define FLAG_CRACKED_PILLAR_A_BURST FLAG_SCENE_TEMP_00
#define FLAG_CRACKED_PILLAR_B_BURST FLAG_SCENE_TEMP_01
#define FLAG_CRACKED_PILLAR_C_BURST FLAG_SCENE_TEMP_02
#define FLAG_CRACKED_PILLAR_D_BURST FLAG_SCENE_TEMP_03
#define FLAG_CRACKED_PILLAR_E_BURST FLAG_SCENE_TEMP_04
/// The high west level's crumbling pillars: each one's flag is set once it has
/// cracked under the leader, and the flag after it once it has fallen. The
/// first's is FLAG_CRACKED_PILLAR_A_BURST's too.
#define FLAG_CRUMBLING_PILLAR_A_CRACKED FLAG_SCENE_TEMP_00
#define FLAG_CRUMBLING_PILLAR_B_CRACKED FLAG_SCENE_TEMP_02
/// Set once Felix has been on the cave mouth's cell (MagmaRock_EnterCave), so
/// that when the game moves him onto it he is not sent straight in; his next
/// step there by himself clears it (MagmaRock_ResetCaveDoor).
#define FLAG_CAVE_DOOR_USED FLAG_SCENE_TEMP_F8
/// Set while Felix stands just behind the high west level's statue, the one
/// place Burst can wake it from (MagmaRock_ReadyHighWestStatue).
#define FLAG_HIGH_WEST_STATUE_READY FLAG_SCENE_TEMP_12
/// Set while a fireball knocks Felix off the high east wall
/// (MagmaRock_KnockLeaderOffWall); the fireballs stop whooshing meanwhile.
#define FLAG_LEADER_KNOCKED_DOWN FLAG_SCENE_TEMP_60
/// The cliff's low east level: each vine Growth has grown there.
#define FLAG_VINE_A_GROWN FLAG_SCENE_TEMP_50
#define FLAG_VINE_B_GROWN FLAG_SCENE_TEMP_51
#define FLAG_VINE_C_GROWN FLAG_SCENE_TEMP_52
/// Its low west level: the rope Lash ties between the two posts.
#define FLAG_LOW_WEST_ROPE_LASHED FLAG_SCENE_TEMP_50

/// The cliff's four levels, by the last entrance of each: its high west and
/// high east halves, then the low west and low east halves below them. They
/// share one map, but to the player each is a screen of its own, with its own
/// placed actors and triggers, left by climbing a wall off its top or foot.
/// From the slide the party climbs onto the low east level, and the high east
/// level's wall leads up to the top.
enum MagmaRockSalamanderTailLevel {
  SALAMANDER_TAIL_HIGH_WEST_LAST_ENTRANCE = 3,
  SALAMANDER_TAIL_HIGH_EAST_LAST_ENTRANCE = 8,
  SALAMANDER_TAIL_LOW_WEST_LAST_ENTRANCE = 12
};

/// The cliff's entrances from within the scene, by the level each opens onto
/// and the level it leads from; scene_exits.h names those from the slide and
/// the top. Two walls lead up from the low west level to the high west level, A
/// the western, and one from the low east level to the high east level; a path
/// leads from the high west level to the high east, and two from the low west
/// level to the low east.
enum MagmaRockSalamanderTailEntrance {
  ENTRANCE_HIGH_WEST_FROM_BELOW_A = 1,
  ENTRANCE_HIGH_WEST_FROM_BELOW_B,
  ENTRANCE_HIGH_WEST_FROM_HIGH_EAST,
  ENTRANCE_HIGH_EAST_FROM_HIGH_WEST,
  ENTRANCE_HIGH_EAST_FROM_BELOW = 7,
  ENTRANCE_LOW_WEST_FROM_ABOVE_A = 9,
  ENTRANCE_LOW_WEST_FROM_ABOVE_B,
  ENTRANCE_LOW_WEST_FROM_LOW_EAST_A,
  ENTRANCE_LOW_WEST_FROM_LOW_EAST_B,
  ENTRANCE_LOW_EAST_FROM_LOW_WEST_A,
  ENTRANCE_LOW_EAST_FROM_ABOVE,
  ENTRANCE_LOW_EAST_FROM_LOW_WEST_B
};

/// The cliff's exits from one of its levels to another, each leading to the
/// entrance of the same letter: the high west level's two walls down to the
/// low west level and the high east level's down to the low east level, the
/// walls back up, and the paths between the levels side by side.
enum MagmaRockSalamanderTailExit {
  EXIT_HIGH_WEST_DOWN_A = 1,
  EXIT_HIGH_WEST_DOWN_B,
  EXIT_HIGH_WEST_TO_HIGH_EAST,
  EXIT_HIGH_EAST_TO_HIGH_WEST,
  EXIT_HIGH_EAST_DOWN = 7,
  EXIT_LOW_WEST_UP_A = 9,
  EXIT_LOW_WEST_UP_B,
  EXIT_LOW_WEST_TO_LOW_EAST_A,
  EXIT_LOW_WEST_TO_LOW_EAST_B,
  EXIT_LOW_EAST_TO_LOW_WEST_A,
  EXIT_LOW_EAST_UP,
  EXIT_LOW_EAST_TO_LOW_WEST_B
};

/// The cell behaviours the scene's triggers answer. The three maps and the
/// cliff's four levels each read them their own way, so one number can stand
/// for two places. Cells on raised ground hold their behaviour some rows south
/// of where they show on screen.
enum MagmaRockCellBehavior {
  /// The cave mouth above the slide's four cracked pillars, cell (16, 28).
  CELL_BEHAVIOR_MAGMA_ROCK_CAVE_DOOR = 2,
  /// The tops of the high west level's two crumbling pillars, cells (15, 29)
  /// and (17, 29).
  CELL_BEHAVIOR_MAGMA_ROCK_CRUMBLING_PILLAR = 20,
  /// A statue that spits a fireball once Burst wakes it
  /// (MagmaRock_FireStatue): the slide's, at cell (25, 25), the high west
  /// level's while Felix stands behind it, and the low east level's upper one,
  /// at cell (48, 39).
  CELL_BEHAVIOR_MAGMA_ROCK_FIRING_STATUE = 30,
  /// The low east level's lower statue that spits fire, cell (52, 40).
  CELL_BEHAVIOR_MAGMA_ROCK_FIRING_STATUE_B = 31,
  /// The cracked pillars Burst breaks, lettered as the events that break
  /// them: on the slide A to D in front of the cave mouth, cells (15, 31),
  /// (17, 31), (14, 34) and (18, 34), and E on the ledge above, cell (20, 25);
  /// on the cliff A on the high west level, cell (22, 34), and B on the low
  /// east level, cell (42, 39).
  CELL_BEHAVIOR_MAGMA_ROCK_CRACKED_PILLAR_A = 31,
  CELL_BEHAVIOR_MAGMA_ROCK_CRACKED_PILLAR_B,
  CELL_BEHAVIOR_MAGMA_ROCK_CRACKED_PILLAR_C,
  CELL_BEHAVIOR_MAGMA_ROCK_CRACKED_PILLAR_D,
  CELL_BEHAVIOR_MAGMA_ROCK_CRACKED_PILLAR_E,
  /// Where walking into a stone pillar pushes it: west of the low west level's
  /// pillar, cell (14, 36), and east of the low east level's, cells (37, 38)
  /// to (40, 38).
  CELL_BEHAVIOR_MAGMA_ROCK_PILLAR_PUSH = 40,
  /// A statue whose grey cap rides up on a column of flame a while after
  /// Burst wakes it (MagmaRock_IgniteStatue), a level's first and second: the
  /// slide's at cell (9, 25), the high east level's at cells (54, 33) and
  /// (58, 34), the low west level's at (6, 38) and (26, 41) and the low east
  /// level's at (44, 39).
  CELL_BEHAVIOR_MAGMA_ROCK_COLUMN_STATUE_A = 50,
  CELL_BEHAVIOR_MAGMA_ROCK_COLUMN_STATUE_B = 51,
  /// On the high west level, the cell just behind its statue, cell (15, 34),
  /// and the one behind that, cell (14, 34).
  CELL_BEHAVIOR_MAGMA_ROCK_BESIDE_HIGH_WEST_STATUE = 50,
  CELL_BEHAVIOR_MAGMA_ROCK_PAST_HIGH_WEST_STATUE = 51,
  /// The statue east of the great fire on the top, cell (50, 22).
  CELL_BEHAVIOR_MAGMA_ROCK_STATUE_ROOM_STATUE = 50
};

/// The trigger a fireball raises as it runs into the leader
/// (FieldWork.raisedTrigger), which the high east level answers by knocking him
/// off the wall.
enum { TRIGGER_FIREBALL_HITS_LEADER = 200 };

/// A burning statue's flame column (MagmaRock_BurnStatue): from frame 35 of its
/// burning each part rises 8 pixels a frame to its level, 16 pixels above the
/// one below, and from frame 332 sinks as fast; the cap on top rises a frame
/// ahead of the column.
#define FLAME_RISE_FRAME 35
#define FLAME_SINK_FRAME 332
#define FLAME_SPEED toFIXED(8)
#define FLAME_LEVEL_HEIGHT toFIXED(16)

/// The frame of Burst's explosions at which a cracked pillar they shake starts
/// to crumble to a stump (its animation 3), before the last blast takes it
/// away.
#define PILLAR_CRUMBLE_FRAME 178

/// The slide: its two stone pillars, the statue on the west of the upper
/// ledge whose cap rides up on a column of flame, the statue on the east of it
/// that spits fire, the pair of statues on either side of the cave mouth with
/// a fireball flying between them, and its cracked pillars, lettered as the
/// events that break them: E on the upper ledge, A to D in front of the cave
/// mouth.
enum MagmaRockSlideActorId {
  ACTOR_SLIDE_PILLAR_A = ACTOR_FIRST_PLACED,
  ACTOR_SLIDE_COLUMN_STATUE,
  ACTOR_SLIDE_PILLAR_B,
  ACTOR_CRACKED_PILLAR_E,
  ACTOR_SLIDE_FIRING_STATUE,
  ACTOR_SLIDE_PAIR_WEST_STATUE,
  ACTOR_SLIDE_PAIR_EAST_STATUE,
  ACTOR_CRACKED_PILLAR_A,
  ACTOR_CRACKED_PILLAR_B,
  ACTOR_CRACKED_PILLAR_C,
  ACTOR_CRACKED_PILLAR_D,
  ACTOR_SLIDE_BOULDER, ///< Lift raises it
  ACTOR_SLIDE_STATUE_CAP,
  ACTOR_SLIDE_FIREBALL ///< drawn as SPRITE_FLAME_PUFF, flying between the pair
};

/// The cliff's high west level: two stone pillars to push, the two crumbling
/// pillars, the tightrope's posts, and the statue that spits fire at the
/// cracked pillar east of it.
enum MagmaRockSalamanderTailHighWestActorId {
  ACTOR_HIGH_WEST_PILLAR_A = ACTOR_FIRST_PLACED,
  ACTOR_CRUMBLING_PILLAR_A,
  ACTOR_CRUMBLING_PILLAR_B,
  ACTOR_HIGH_WEST_TIGHTROPE_WEST_POST,
  ACTOR_HIGH_WEST_TIGHTROPE_EAST_POST,
  ACTOR_HIGH_WEST_FIRING_STATUE,
  ACTOR_HIGH_WEST_CRACKED_PILLAR_A,
  ACTOR_HIGH_WEST_PILLAR_B
};

/// Its high east level: three pairs of statues facing each other across the
/// wall Felix climbs, one above another, with a fireball flying between each
/// pair, two statues whose caps ride up on columns of flame, and a stone
/// pillar to push.
enum MagmaRockSalamanderTailHighEastActorId {
  ACTOR_STATUE_PAIR_A_WEST = ACTOR_FIRST_PLACED,
  ACTOR_STATUE_PAIR_A_EAST,
  ACTOR_STATUE_PAIR_B_WEST,
  ACTOR_STATUE_PAIR_B_EAST,
  ACTOR_STATUE_PAIR_C_WEST,
  ACTOR_STATUE_PAIR_C_EAST,
  ACTOR_HIGH_EAST_COLUMN_STATUE_A,
  ACTOR_HIGH_EAST_STATUE_CAP_A,
  ACTOR_HIGH_EAST_PILLAR,
  ACTOR_HIGH_EAST_COLUMN_STATUE_B,
  ACTOR_HIGH_EAST_STATUE_CAP_B,
  ACTOR_FIREBALL_A,
  ACTOR_FIREBALL_B,
  ACTOR_FIREBALL_C
};

/// Its low west level: a stone pillar to push, two statues whose caps ride up
/// on columns of flame, and the posts Lash ties the rope between.
enum MagmaRockSalamanderTailLowWestActorId {
  ACTOR_LOW_WEST_PILLAR = ACTOR_FIRST_PLACED,
  ACTOR_LOW_WEST_COLUMN_STATUE_A,
  ACTOR_LOW_WEST_STATUE_CAP_A,
  ACTOR_LOW_WEST_COLUMN_STATUE_B,
  ACTOR_LOW_WEST_STATUE_CAP_B,
  ACTOR_LOW_WEST_ROPE_WEST_POST,
  ACTOR_LOW_WEST_ROPE_EAST_POST
};

/// Its low east level: two stone pillars to push, the cracked pillar, two
/// statues that spit fire, one whose cap rides up on a column of flame, and
/// three sprouts.
enum MagmaRockSalamanderTailLowEastActorId {
  ACTOR_LOW_EAST_PILLAR_A = ACTOR_FIRST_PLACED,
  ACTOR_LOW_EAST_PILLAR_B,
  ACTOR_LOW_EAST_CRACKED_PILLAR,
  ACTOR_LOW_EAST_FIRING_STATUE_A,
  ACTOR_LOW_EAST_COLUMN_STATUE,
  ACTOR_LOW_EAST_STATUE_CAP,
  ACTOR_LOW_EAST_FIRING_STATUE_B = ACTOR_FIRST_PLACED + 7,
  ACTOR_LOW_EAST_SPROUT_A, ///< Growth grows a vine from each
  ACTOR_LOW_EAST_SPROUT_B,
  ACTOR_LOW_EAST_SPROUT_C
};

/// The top: the great fire before the way in, and the statue east of it that
/// spits fire at it.
enum MagmaRockStatueActorId {
  ACTOR_GREAT_FIRE = ACTOR_FIRST_PLACED,
  ACTOR_STATUE_ROOM_STATUE
};
#include "shared/tables.h"

/// Unused: creates a hidden effect actor at a place: no motion, never
/// targeted, drawn under the actors.
UNUSED struct Actor *MagmaRock_CreateEffectActor(s32 x, s32 y, s32 z,
                                                 s32 kind) {
  struct Actor *actor = (FieldActor_Create(kind, x, y, z));

  if (actor != NULL) {
    Scene_SetActorSpritePriority(actor, 1);
    actor->motionFlags = 0;
    actor->collisionFlags = ACTOR_NOT_TARGETED;
    Actor_SetSpriteFlags(actor, 0);
    FieldActor_SetSpriteColor(actor, SPRITE_COLOR_HIDDEN);
    actor->priorityFlags |=
        ACTOR_PRIORITY_IGNORES_RISES | ACTOR_PRIORITY_UNDERFOOT;
    return actor;
  }
  return NULL;
}

#include "../field/field_effect_move.c"

#define EFFECT_SET_MODE Actor_SelectAnimation
#define EFFECT_SET_SCRIPT Actor_StartScript
#define EFFECT_SPRITE_PRIORITY
#include "../field/field_effect_spawn.c"

#include "shared/position_distance.h"

#include "../field/actor_find_in_cell.c"

extern s32 gPushLeaderSteps[];

#include "shared/block_push_onto.h"

#include "../field/map_cell_fill_byte.c"

/// Unused: talking to Fury starts its battle, as in Magma Rock's inside; no
/// Fury stands outside.
UNUSED void MagmaRock_FightFury(void) {
  Field_RequestDjinnBattle(ACTOR_FIRST_PLACED, DJINNI_FURY);
}

#include "shared/position_flat_distance.h"

extern u8 gMapShapeGrid[];

s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
s32 MapGrid_GetTerrainKind(s32 layer, s32 x, s32 z);
s32 MapCollision_FindOrAddTag(s32 layer, s32 shape, s32 height);

#include "shared/map_cell_set_shape.h"

#include "../field/map_cell_get.c"

#include "shared/actors_raise_cells.h"

#include "shared/blocks_mark_cells.c"

#include "shared/block_clear_cells.h"

/// For each pillar of a list ended by ACTOR_LIST_END: raises its cell to its
/// top, keeps the cell's attributes, and where it stands on a climbable cell
/// (MAP_CELL_CLIMBABLE) blocks the cells ahead of and behind it.
void MagmaRock_SettleBlocks(u16 *ids) {
  for (; *ids != ACTOR_LIST_END; ids++) {
    struct Actor *actor = Actor_Get(*ids);
    u8 *terrainId = &actor->layer;
    s32 height =
        Map_GetTerrainHeight(*terrainId, actor->x.fixed, actor->z.fixed);
    u8 *blocked;
    struct MapWork *map;
    struct MapCell **cells;
    struct MapCell *cell;
    s32 x, z;

    actor->riseBaseY =
        CELL_ATTRIBUTE_BITS & MapCell_GetAttributes(MAPGRID_LAYER_GROUND,
                                                    FIELD_CELL(actor->x.fixed),
                                                    FIELD_CELL(actor->z.fixed));
    actor->motionFlags = 0;
    actor->collisionFlags = 0;
    Actor_SetSpriteFlags(actor, 0);
    MagmaRockInside_SetCellShape(*terrainId, FIELD_CELL(actor->x.fixed),
                                 FIELD_CELL(actor->z.fixed),
                                 CELL_HEIGHT_STEPS(height) + 4);
    blocked = &actor->riseEnabled;
    *blocked = 0;
    if (MapCell_GetAttributes(MAPGRID_LAYER_GROUND, FIELD_CELL(actor->x.fixed),
                              FIELD_CELL(actor->z.fixed)) &
        MAP_CELL_CLIMBABLE) {
      map = Ram_HeapSlots->mapGrid;
      if (map == NULL)
        return;
      cells = &map->layers[MAPGRID_LAYER_OVERHEAD].cells;
      z = FIELD_CELL(actor->z.fixed - actor->y.fixed);
      x = FIELD_CELL(actor->x.fixed);
      cell = *cells;
      cell += x + ((z + 1) << 7);
      cell->collisionCode = CELL_BEHAVIOR_WALL;
      cell = *cells;
      cell += x + ((z - 1) << 7);
      cell->collisionCode = CELL_BEHAVIOR_WALL;
      *blocked = 1;
    }
  }
}

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

extern struct SceneRegion gMagmaRock_Regions[];
extern struct SceneRegion gMagmaRock_SalamanderTailRegions[];
extern struct SceneRegion gMagmaRock_StatueRegions[];

const struct SceneRegion *Scene_GetExitRegions(void) {
  if (gMainRoutineWork.next == SCENE_MAGMA_ROCK_SALAMANDER_TAIL_CHEST)
    return gMagmaRock_SalamanderTailRegions;
  if (gMainRoutineWork.next == SCENE_MAGMA_ROCK_STATUE)
    return gMagmaRock_StatueRegions;
  return gMagmaRock_Regions;
}

const u32 *Scene_GetExits(void) { return gSceneExits; }

extern struct ActorPlacement gMagmaRock_Placements[];
extern struct ActorPlacement gMagmaRock_SalamanderTailHighWestPlacements[];
extern struct ActorPlacement gMagmaRock_SalamanderTailHighEastPlacements[];
extern struct ActorPlacement gMagmaRock_SalamanderTailLowWestPlacements[];
extern struct ActorPlacement gMagmaRock_SalamanderTailLowEastPlacements[];
extern struct ActorPlacement gMagmaRock_StatuePlacements[];

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  if (gMainRoutineWork.next == SCENE_MAGMA_ROCK_SALAMANDER_TAIL_CHEST) {
    if (gMainRoutineWork.entrance <= SALAMANDER_TAIL_HIGH_WEST_LAST_ENTRANCE)
      return gMagmaRock_SalamanderTailHighWestPlacements;
    if (gMainRoutineWork.entrance <= SALAMANDER_TAIL_HIGH_EAST_LAST_ENTRANCE)
      return gMagmaRock_SalamanderTailHighEastPlacements;
    if (gMainRoutineWork.entrance <= SALAMANDER_TAIL_LOW_WEST_LAST_ENTRANCE)
      return gMagmaRock_SalamanderTailLowWestPlacements;
    return gMagmaRock_SalamanderTailLowEastPlacements;
  }
  if (gMainRoutineWork.next == SCENE_MAGMA_ROCK_STATUE)
    return gMagmaRock_StatuePlacements;
  return gMagmaRock_Placements;
}

void Actor_LiftTargetBlock(s16 *table);

/// Nothing visible happens as Felix starts to push one of the striped stone
/// pillars, with Move or by walking into it; the push itself is the field's.
/// The game first takes the pillar out of the map, so that its cell is plain
/// ground under it as it slides. On the slide and on the low west and low east
/// levels it also clears the walls the map's upper layer keeps along the
/// pillars' paths (copied from spare cells off the screen), so that the pillar
/// can slide along them; MagmaRock_FinishPushingPillar puts them back.
void MagmaRock_StartPushingPillar(void) {
  // On the slide the overhead layer's closed cells there, as probed, are
  // columns 15 and 17 of rows 20 to 23, the upper pillar's cell (15, 22)
  // among them, and columns 14, 16 and 18 of rows 23 to 26, the lower
  // pillar's (16, 25) among them; the copies rewrite map.txt's (78, 21) to
  // (81, 26). While no pillar moves, those of columns 15 and 17 stop a climb
  // down those columns from the ledge on row 27, as Field_ClimbDown tests the
  // wall's face below him: standing on cell (15, 27) Felix holds Down and
  // stays at the ledge's edge, while from cell (16, 27) he climbs down the
  // pegs to the cave mouth above the four cracked pillars.
  struct FieldWork *event = Ram_HeapSlots->fieldWork;
  struct FieldEffectWork *work = Ram_HeapSlots->fieldEffectWork;

  if (gMainRoutineWork.next == SCENE_MAGMA_ROCK_SLIDE) {
    MapGrid_CopyCellAttributes(98, 18, 4, 3, 78, 21);
    MapGrid_CopyCellAttributes(98, 24, 4, 3, 78, 24);
    Actor_LiftTargetBlock(gMagmaRock_SlidePillars);
  } else if (gMainRoutineWork.next == SCENE_MAGMA_ROCK_SALAMANDER_TAIL_CHEST) {
    if (gMainRoutineWork.entrance <= SALAMANDER_TAIL_HIGH_WEST_LAST_ENTRANCE) {
      Actor_LiftTargetBlock(gMagmaRock_SalamanderTailHighWestPillars);
      // Unlike the rest of the scene, these tests give the high east level's
      // last entrance to the low west level's pillars, and the low west
      // level's last to the low east level's.
    } else if (gMainRoutineWork.entrance <
               SALAMANDER_TAIL_HIGH_EAST_LAST_ENTRANCE) {
      Actor_LiftTargetBlock(gMagmaRock_SalamanderTailHighEastPillars);
    } else if (gMainRoutineWork.entrance <
               SALAMANDER_TAIL_LOW_WEST_LAST_ENTRANCE) {
      MapGrid_CopyCellAttributes(79, 46, 2, 1, 79, 36);
      Actor_LiftTargetBlock(gMagmaRock_SalamanderTailLowWestPillars);
    } else {
      if (work->child->z.part[0].integer == 600)
        MapGrid_CopyCellAttributes(104, 38, 1, 1, 103, 38);
      else
        MapGrid_CopyCellAttributes(94, 44, 5, 1, 99, 38);
      Actor_LiftTargetBlock(gMagmaRock_SalamanderTailLowEastPillars);
    }
  }
  MagmaRockInside_ClearBlockCells(event->movedBlock);
}

void Event_Start();
void Actor_PlaceMovedBlock();
/// The overlay's veneer to Event_End.
s32 MagmaRock_EndEvent();

/// A striped stone pillar comes to rest after a push, and stands in the way
/// again where it stopped. One that has reached the place the climb needs
/// stays there when the party comes back: on the slide the pillar on the
/// statues' ledge pushed west out of the way, to cell (15, 25), and the one on
/// the ledge above pushed east, to cell (16, 22), before the climb to the
/// cliff; on the cliff the high west level's second pillar at cell (27, 33),
/// the high east level's at (55, 35), the low west level's at (16, 36), and
/// the low east level's two at (40, 37) and (35, 38). The game stands the
/// pillar in the map again, puts back the upper layer's walls
/// MagmaRock_StartPushingPillar cleared, and sets the pillar's flag.
s32 MagmaRock_FinishPushingPillar(void) {
  struct Actor *pillar = gFieldWork->movedBlock;
  s32 x, z;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Actor_PlaceMovedBlock();
  if (gMainRoutineWork.next == SCENE_MAGMA_ROCK_SLIDE) {
    MapGrid_CopyCellAttributes(98, 21, 4, 3, 78, 21);
    MapGrid_CopyCellAttributes(98, 27, 4, 3, 78, 24);
    Blocks_MarkCells(gMagmaRock_SlidePillars);
  } else if (gMainRoutineWork.next == SCENE_MAGMA_ROCK_SALAMANDER_TAIL_CHEST) {
    if (gMainRoutineWork.entrance <= SALAMANDER_TAIL_HIGH_WEST_LAST_ENTRANCE) {
      Blocks_MarkCells(gMagmaRock_SalamanderTailHighWestPillars);
    } else if (gMainRoutineWork.entrance <=
               SALAMANDER_TAIL_HIGH_EAST_LAST_ENTRANCE) {
      Blocks_MarkCells(gMagmaRock_SalamanderTailHighEastPillars);
    } else if (gMainRoutineWork.entrance <=
               SALAMANDER_TAIL_LOW_WEST_LAST_ENTRANCE) {
      MapGrid_CopyCellAttributes(79, 37, 2, 1, 79, 36);
      Blocks_MarkCells(gMagmaRock_SalamanderTailLowWestPillars);
    } else {
      MapGrid_CopyCellAttributes(103, 39, 1, 1, 103, 38);
      MapGrid_CopyCellAttributes(99, 42, 5, 1, 99, 38);
      Blocks_MarkCells(gMagmaRock_SalamanderTailLowEastPillars);
    }
  }
  x = FIELD_CELL(pillar->x.fixed);
  z = FIELD_CELL(pillar->z.fixed);
  if (gMainRoutineWork.next == SCENE_MAGMA_ROCK_SLIDE) {
    if (x == 15 && z == 25)
      Event_SetFlag(FLAG_SLIDE_PILLAR_B_PUSHED);
    else if (x == 16 && z == 22)
      Event_SetFlag(FLAG_SLIDE_PILLAR_A_PUSHED);
  } else if (gMainRoutineWork.next == SCENE_MAGMA_ROCK_SALAMANDER_TAIL_CHEST) {
    if (gMainRoutineWork.entrance <= SALAMANDER_TAIL_HIGH_WEST_LAST_ENTRANCE) {
      if (x == 27 && z == 33)
        Event_SetFlag(FLAG_HIGH_WEST_PILLAR_B_PUSHED);
    } else if (gMainRoutineWork.entrance <=
               SALAMANDER_TAIL_HIGH_EAST_LAST_ENTRANCE) {
      if (x == 55 && z == 35)
        Event_SetFlag(FLAG_HIGH_EAST_PILLAR_PUSHED);
    } else if (gMainRoutineWork.entrance <=
               SALAMANDER_TAIL_LOW_WEST_LAST_ENTRANCE) {
      if (x == 16 && z == 36)
        Event_SetFlag(FLAG_LOW_WEST_PILLAR_PUSHED);
    } else if (x == 40 && z == 37) {
      Event_SetFlag(FLAG_LOW_EAST_PILLAR_A_PUSHED);
    } else if (x == 35 && z == 38) {
      Event_SetFlag(FLAG_LOW_EAST_PILLAR_B_PUSHED);
    }
  }
  return MagmaRock_EndEvent();
}

/// The overlay's import veneer that sets up a field Psynergy's caster and
/// target.
void Import_FieldPsynergySetupActorPair(s32 owner, u16 value);

/// Plays Burst from the leader at an actor, outside the trigger table: the
/// explosion a statue's fireball makes where it lands.
void MagmaRock_PlayBurst(u16 value) {
  struct MainRoutineWork *party;

  FieldPsynergy_LoadResources(ABILITY_BURST, 1);
  party = &gMainRoutineWork;
  Import_FieldPsynergySetupActorPair(party->leaderActorId, value);
  FieldPsynergy_RunHandler(1);
  BattleEffect_StopMainActor();
  FieldPsynergy_End();
}

extern s32 gMagmaRock_FlameEmberScript[];

/// A statue's update when the fireball of its pair reaches it: it throws
/// embers and blinks red, faster and faster, flashes white, and after 144
/// frames spits the fireball back.
void MagmaRock_ThrowEmbers(struct Actor *actor) {
  struct Actor *linked = actor->watchedActor;
  s16 *timer = &actor->timer;

  if (*timer < 50) {
    u32 phase = gFrameCount & 3;

    if (phase == 0) {
      struct EffectOptions params;
      s32 dx;
      s32 dy;

      params.type = SPRITE_FLAME_PUFF;
      params.priority = 1;
      params.script = gMagmaRock_FlameEmberScript;
      {
        s32 mask = 15;

        dx = mask & Random16();
      }
      dy = Random16() & 31;
      FieldEffect_Spawn(actor->x.fixed + INTtoFIXED(dx - 8),
                        actor->y.fixed + INTtoFIXED(dy) - toFIXED(8),
                        actor->z.fixed, 0, phase, phase,
                        EFFECT_USE_SCRIPT | EFFECT_USE_TYPE |
                            EFFECT_USE_PRIORITY | 1,
                        &params);
    }
    if (gFrameCount & 4)
      FieldActor_SetSpriteColor(actor, SPRITE_COLOR_RED_TINT);
    else
      FieldActor_SetSpriteColor(actor, SPRITE_COLOR_NORMAL);
  } else if (*timer < 100) {
    if (gFrameCount & 4)
      FieldActor_SetSpriteColor(actor, SPRITE_COLOR_RED_TINT);
    else
      FieldActor_SetSpriteColor(actor, SPRITE_COLOR_NORMAL);
  } else if (*timer < 140) {
    if (gFrameCount & 2)
      FieldActor_SetSpriteColor(actor, SPRITE_COLOR_RED_TINT);
    else
      FieldActor_SetSpriteColor(actor, SPRITE_COLOR_NORMAL);
  } else if (*timer < 144) {
    FieldActor_SetSpriteColor(actor, SPRITE_COLOR_WHITE);
  } else {
    FieldActor_SetSpriteColor(actor, SPRITE_COLOR_NORMAL);
    actor->update = NULL;
    linked->x.fixed = actor->x.fixed;
    linked->y.fixed = actor->y.fixed;
    linked->z.fixed = actor->z.fixed;
    linked->update = MagmaRock_MoveFireball;
  }
  (*timer)++;
}

void Actor_PlaySoundInView(struct Actor *actor, s32 value);

/// The update of a fireball or a statue's fire: every fourth frame it leaves
/// an ember near it, drifting a little north or south, and every sixteenth it
/// whooshes, unless one has just knocked Felix down.
void MagmaRock_LeaveEmbers(struct Actor *actor) {
  u32 frame = gFrameCount;

  if ((frame & 3) == 0) {
    struct EffectOptions config;
    s32 dz;
    s32 velocityZ;
    FIXED z;

    config.type = SPRITE_FLAME_PUFF;
    config.priority = 2;
    config.script = gMagmaRock_EmberScript;
    if ((frame & 15) == 0 && !Event_GetFlag(FLAG_LEADER_KNOCKED_DOWN))
      Actor_PlaySoundInView(actor, SE_WHOOSH);
    dz = ((Random16() & 15) - 8) * (toFIXED(1) / 2);
    velocityZ = ((Random16() & 31) - 16) * (toFIXED(1) / 16);
    z = actor->z.fixed;
    FieldEffect_Spawn(
        actor->x.fixed, actor->y.fixed, z + dz, 0, 0, velocityZ,
        EFFECT_USE_SCRIPT | EFFECT_USE_TYPE | EFFECT_USE_PRIORITY | 1, &config);
  }
}

FIXED Trig_Cos(s32 angle);

/// A fireball's update: it flies toward the other statue of its pair leaving
/// embers; running into Felix raises TRIGGER_FIREBALL_HITS_LEADER and takes it
/// away, and reaching the statue hands it over (MagmaRock_ThrowEmbers).
void MagmaRock_MoveFireball(struct Actor *actor) {
  FIXED step;
  struct Actor *partner;
  struct Actor *leader;

  step = Trig_Cos(actor->facing);
  partner = Actor_Get(actor->userValue + actor->rollSide);
  leader = Actor_Get(gMainRoutineWork.leaderActorId);
  actor->x.fixed += step * 2;
  MagmaRock_LeaveEmbers(actor);
  if (leader->x.fixed - toFIXED(8) < actor->x.fixed + toFIXED(6) &&
      actor->x.fixed - toFIXED(6) < leader->x.fixed + toFIXED(8) &&
      leader->z.fixed - toFIXED(8) < actor->z.fixed + toFIXED(6) &&
      actor->z.fixed - toFIXED(6) < leader->z.fixed + toFIXED(8) &&
      leader->y.fixed < actor->y.fixed + toFIXED(8) &&
      actor->y.fixed - toFIXED(8) < leader->y.fixed + toFIXED(15)) {
    gFieldWork->raisedTrigger = TRIGGER_FIREBALL_HITS_LEADER;
    actor->update = NULL;
    actor->x.fixed = 0;
    actor->y.fixed = 0;
    actor->z.fixed = 0;
  }
  if (Position_FlatDistance(&actor->x.fixed, &partner->x.fixed) <= 8 &&
      FIELD_CELL(actor->y.fixed) == FIELD_CELL(partner->y.fixed)) {
    actor->x.fixed = 0;
    actor->update = NULL;
    // It turns back, every other time a sixteenth of a turn off the line, so
    // it rolls a little slower one way than the other.
    actor->facing ^= FACING_WEST | TRIG_TURN / 16;
    actor->y.fixed = 0;
    actor->z.fixed = 0;
    actor->rollSide ^= 1;
    partner->timer = 0;
    partner->update = (void *)MagmaRock_ThrowEmbers;
    partner->watchedActor = actor;
  }
}

/// Unless the party keeps its place: sets a fireball on one statue of the pair
/// whose first statue is `first` (`side` 0 that one, 1 the other), facing the
/// other, and starts it flying.
void MagmaRock_PlaceFireball(u16 id, s16 first, s16 side) {
  struct Actor *actor = Actor_Get(id);
  struct Actor *firstStatue;
  struct Actor *secondStatue;
  u16 firstId;
  u16 startSide;
  s32 startId;
  s32 secondId;
  u8 *flags;
  u8 rollSide;
  s16 *pairFirst;

  if (Event_GetFlag(FLAG_KEEP_PARTY_POSITION))
    return;
  firstId = first;
  startSide = side;
  startId = firstId + startSide;
  actor->x.fixed = Actor_Get(startId)->x.fixed;
  actor->y.fixed = Actor_Get(startId)->y.fixed;
  actor->z.fixed = Actor_Get(startId)->z.fixed;
  Scene_SetActorSpritePriority(actor, 2);
  pairFirst = &actor->userValue;
  *pairFirst = first;
  rollSide = side ^ 1;
  actor->rollSide = rollSide;
  actor->update = (void *)MagmaRock_MoveFireball;
  secondId = firstId + 1;
  flags = &actor->motionFlags;
  if (startSide != 0) {
    firstStatue = Actor_Get(firstId);
    secondStatue = Actor_Get(secondId);
    if (firstStatue->x.fixed < secondStatue->x.fixed)
      actor->facing = FACING_WEST;
    else
      actor->facing = FACING_EAST;
  } else {
    firstStatue = Actor_Get(firstId);
    secondStatue = Actor_Get(secondId);
    if (firstStatue->x.fixed > secondStatue->x.fixed)
      actor->facing = FACING_WEST;
    else
      actor->facing = FACING_EAST;
  }
  *flags = 0;
}

#include "shared/statue_flash_under_burst.h"

/// The update of a statue Burst has woken, counting down from 1,024 while no
/// menu, Psynergy or item is in use: it blinks red quickly, then slowly from
/// 528, and from 400 it burns (MagmaRock_BurnStatue).
void MagmaRock_WarnStatue(struct Actor *actor) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;

  if (actor->timer > 528) {
    if (actor->timer & 2)
      FieldActor_SetSpriteColor(actor, SPRITE_COLOR_RED_TINT);
    else
      FieldActor_SetSpriteColor(actor, SPRITE_COLOR_NORMAL);
  } else if (actor->timer > 400) {
    if (actor->timer & 16)
      FieldActor_SetSpriteColor(actor, SPRITE_COLOR_RED_TINT);
    else
      FieldActor_SetSpriteColor(actor, SPRITE_COLOR_NORMAL);
  } else {
    actor->update = MagmaRock_BurnStatue;
    actor->timer = 1;
  }
  if (work->psynergy == 0 && work->fieldMenu == 0 && work->pauseMenu == 0 &&
      work->item == 0 && work->transitionActive != 0)
    actor->timer--;
}

#include "shared/statue_find_under_burst.h"

/// Looks up the cell at (x, z) in an (actor, cell, value) table ended by
/// ACTOR_LIST_END and returns the value of its row.
s32 MagmaRock_LookUpCellStatue(s32 x, s32 z, u16 *table) {
  s32 result = 0;
  s32 code = MapCell_GetAttributes(MAPGRID_LAYER_GROUND, FIELD_CELL(x),
                                   FIELD_CELL(z)) >>
             8;

  while (*table != ACTOR_LIST_END) {
    table++;
    if (code == *table) {
      result = table[1];
      break;
    }
    table++;
    result = *table++;
  }
  return result;
}

/// Felix casts Burst at one of the yellow statues, and it glows red while the
/// explosions go off round it, blinking faster at the last blast. The game
/// finds the statue by its cell in the room's table (Statue_FindUnderBurst) and
/// blinks it (Statue_FlashUnderBurst), every fourth frame while the explosions
/// shake the ground (BURST_STAGE_SMALL_BLAST) and every other one at the last
/// blast (BURST_STAGE_GREAT_BLAST).
/// The statue then fires (MagmaRock_FireStatue) or lights
/// (MagmaRock_IgniteStatue).
void MagmaRock_TargetStatue(s32 stage) {
  struct FieldEffectWork *work = Ram_HeapSlots->fieldEffectWork;
  s16 *table = gMagmaRock_SlideStatues;
  struct MainRoutineWork *party = &gMainRoutineWork;

  if (party->next == SCENE_MAGMA_ROCK_SALAMANDER_TAIL_CHEST) {
    if (party->entrance <= SALAMANDER_TAIL_HIGH_WEST_LAST_ENTRANCE)
      table = gMagmaRock_SalamanderTailHighWestStatues;
    else if (party->entrance <= SALAMANDER_TAIL_HIGH_EAST_LAST_ENTRANCE)
      table = gMagmaRock_SalamanderTailHighEastStatues;
    else if (party->entrance <= SALAMANDER_TAIL_LOW_WEST_LAST_ENTRANCE)
      table = gMagmaRock_SalamanderTailLowWestStatues;
    else
      table = gMagmaRock_SalamanderTailLowEastStatues;
  } else if (party->next == SCENE_MAGMA_ROCK_STATUE) {
    table = gMagmaRock_StatueStatues;
  }
  if (stage == BURST_STAGE_SMALL_BLAST) {
    struct Actor *target;

    Statue_FindUnderBurst(table);
    target = work->child;
    target->flashMask = 4;
    target->update = (ObjectUpdate)Statue_FlashUnderBurst;
  }
  if (stage == BURST_STAGE_GREAT_BLAST) {
    struct Actor *target;

    Statue_FindUnderBurst(table);
    target = work->child;
    target->flashMask = 2;
    target->update = (ObjectUpdate)Statue_FlashUnderBurst;
  }
}

/// An ember's update: it moves by its velocity, kept in bounce, gravity and
/// riseBaseY, and slows by an eighth.
void MagmaRock_DriftAndSlow(struct Actor *actor) {
  actor->x.fixed += actor->bounce;
  actor->y.fixed += actor->gravity;
  actor->z.fixed += actor->riseBaseY;
  actor->bounce -= actor->bounce >> 3;
  actor->gravity -= actor->gravity >> 3;
  actor->riseBaseY -= actor->riseBaseY >> 3;
}

s32 Position_FlatDistance(FIXED *a, FIXED *b);
void Actor_Release(struct Actor *actor);
s32 MagmaRock_EndEvent(void);
void FieldCamera_WaitForArrival(void);

/// A statue's fireball flies straight ahead, the camera following it, until it
/// meets the edge of its ledge, a striped stone pillar or anything else
/// standing in its way: at the edge or a stone pillar it bursts into embers,
/// and on anything else it explodes as Burst does (MagmaRock_PlayBurst). Then
/// the camera goes back to Felix.
s32 MagmaRock_FireFromStatue(struct Actor *source) {
  FIXED position[3];
  struct EffectOptions options;
  struct Actor *actor;
  struct Actor *other;
  s32 step;
  s32 parity;
  s32 i;
  s32 attributes;

  {
    FIXED x = source->x.fixed;
    FIXED y = source->y.fixed;
    FIXED z = source->z.fixed;

    actor = FieldActor_Create(SPRITE_FLAME_PUFF, x, y, z);
  }
  step = Trig_Cos(source->facing) << 1;
  actor->motionFlags = 0;
  Actor_SelectAnimation(actor, 1);
  Actor_SetSpriteFlags(actor, 0);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  position[0] = actor->x.fixed;
  position[1] = actor->y.fixed;
  position[2] = actor->z.fixed;
  for (;;) {
    position[0] += step;
    attributes =
        MapCell_GetAttributes(MAPGRID_LAYER_OVERHEAD, FIELD_CELL(position[0]),
                              FIELD_CELL(position[2] - position[1])) >>
        8;
    if (attributes == CELL_BEHAVIOR_MAGMA_ROCK_FIRE_EDGE ||
        attributes == CELL_BEHAVIOR_MAGMA_ROCK_LAVA_FALL)
      goto stop;
    for (i = ACTOR_FIRST_PLACED; i != FIELD_TREASURE_ACTOR; i++) {
      other = gFieldWork->actors[i];
      if (other->script == NULL)
        continue;
      if (FIELD_CELL(source->x.fixed) == FIELD_CELL(other->x.fixed) &&
          FIELD_CELL(source->y.fixed) == FIELD_CELL(other->y.fixed) &&
          FIELD_CELL(source->z.fixed) == FIELD_CELL(other->z.fixed))
        continue;
      if (Position_FlatDistance(&other->x.fixed, position) > 8)
        continue;
      if (FIELD_CELL(other->y.fixed) != FIELD_CELL(position[1]))
        continue;
      if (((struct Sprite *)other->sprite)->entries[0]->animationId ==
          SPRITE_STONE_PILLAR)
        goto stop;
      goto hit;
    }
  }

stop:
  actor->acceleration = toFIXED(2);
  actor->speedLimit = toFIXED(2);
  actor->update = MagmaRock_LeaveEmbers;
  FieldActor_SetMoveTargetFX(actor, position[0], position[1], position[2]);
  FieldCamera_SetSpeed(2);
  FieldCamera_MoveToFX(position[0], position[1], position[2], 1);
  FieldActor_WaitForMove(actor);
  actor->update = NULL;
  Actor_Release(actor);
  for (i = 0; i != 16; i++) {
    parity = gFrameCount & 1;

    if (parity == 0) {
      s32 randomX;
      s32 randomY;

      // Each ember flies off at up to 2 pixels a frame across and up or down.
      options.type = SPRITE_FLAME_PUFF;
      options.priority = 1;
      options.update = (void (*)(union FieldObject *))MagmaRock_DriftAndSlow;
      options.script = gMagmaRock_EmberScript;
      randomX = Random16() & 31;
      randomY = Random16() & 31;
      FieldEffect_Spawn(position[0], position[1], position[2],
                        (randomX - 16) << 13, (randomY - 16) << 13, parity,
                        EFFECT_USE_UPDATE | EFFECT_USE_SCRIPT |
                            EFFECT_USE_TYPE | EFFECT_USE_PRIORITY,
                        &options);
    }
    System_Wait(1);
  }
  System_Wait(24);
  EventCamera_FollowActor(gMainRoutineWork.leaderActorId, 1);
  FieldCamera_WaitForArrival();
  System_Wait(10);
  return MagmaRock_EndEvent();

hit:
  actor->acceleration = toFIXED(2);
  actor->speedLimit = toFIXED(2);
  actor->update = MagmaRock_LeaveEmbers;
  FieldActor_SetMoveTargetFX(actor, position[0], position[1], position[2]);
  FieldCamera_SetSpeed(2);
  FieldCamera_MoveToFX(position[0], position[1], position[2], 1);
  FieldActor_WaitForMove(actor);
  actor->update = NULL;
  Actor_Release(actor);
  Event_SetFlag(FLAG_FIRE_BURST_PLAYING);
  MagmaRock_PlayBurst(i);
  Event_ClearFlag(FLAG_FIRE_BURST_PLAYING);
  EventCamera_FollowActor(gMainRoutineWork.leaderActorId, 1);
  FieldCamera_WaitForArrival();
  System_Wait(10);
  return MagmaRock_EndEvent();
}

/// After Felix's Burst, the yellow statue he cast it at flashes white and spits
/// a fireball straight ahead: on the slide's statues' ledge and the high west
/// level at the cracked pillar beyond it, which the fireball's blast breaks,
/// and on the top at the great fire. The fireball's flight is
/// MagmaRock_FireFromStatue's. On the low east level both statues face west:
/// the upper one's fireball meets the cracked pillar at cell (42, 39), and the
/// lower one's, from cell (52, 40), the level's second cracked pillar, at
/// (760, 648) on the same level (the placement table's seventh actor), and
/// explodes on it as Burst does.
void MagmaRock_FireStatue(void) {
  struct FieldEffectWork *work = Ram_HeapSlots->fieldEffectWork;
  struct Actor *actor;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_Wait(10);
  actor = work->child;
  actor->update = NULL;
  FieldActor_SetSpriteColor(actor, SPRITE_COLOR_WHITE);
  FieldEvent_Wait(3);
  FieldActor_SetSpriteColor(work->child, SPRITE_COLOR_NORMAL);
  FieldEvent_Wait(1);
  MagmaRock_FireFromStatue(work->child);
  MagmaRock_EndEvent();
}

/// After Felix's Burst, a yellow statue with a grey cap blinks red for about
/// ten seconds, faster at first; then fire bursts from it and its cap rides up
/// on a column of flame for about five seconds, carrying Felix up to the ledge
/// above if he is standing on it, before it sinks again. The game only starts
/// the statue's countdown (MagmaRock_WarnStatue, then MagmaRock_BurnStatue).
void MagmaRock_IgniteStatue(void) {
  struct FieldEffectWork *work = Ram_HeapSlots->fieldEffectWork;
  struct Actor *actor;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  actor = work->child;
  actor->timer = 1024;
  actor->update = MagmaRock_WarnStatue;
  MagmaRock_EndEvent();
}

/// Burst goes off at a cracked pillar: rocks fly up round it while the
/// explosions shake the ground, the pillar crumbles to a stump, and the last
/// blast takes it away. Felix's own Burst at a cracked pillar runs it through
/// the pillar's rows; a statue that spits fire runs it as its fireball
/// explodes on the cracked pillar in its way. On the cliff's high west level,
/// Felix's Burst at the statue on cell (16, 34) has it flash white and spit a
/// fireball east, and the fireball's blast breaks the cracked pillar on cell
/// (22, 34) this way. The statue's rows that run it come after the
/// MagmaRock_TargetStatue rows, which take Felix's own Burst and are passed
/// over while the fireball's blast plays (FLAG_FIRE_BURST_PLAYING); they set
/// no flag for the pillar. Rocks are thrown one a frame during the shaking
/// (BURST_STAGE_SMALL_BLAST) and eight at the last blast
/// (BURST_STAGE_GREAT_BLAST), which moves the target out of the scene; the
/// stump is the pillar's animation from PILLAR_CRUMBLE_FRAME.
void MagmaRock_BurstStatue(s32 stage, s32 frame) {
  struct FieldEffectWork *work = Ram_HeapSlots->fieldEffectWork;

  if (frame == 0)
    MagmaRock_StartFallingRocks();
  if (stage == BURST_STAGE_SMALL_BLAST) {
    struct Actor *actor = work->child;

    MagmaRock_DropRocks(actor->x.fixed, actor->y.fixed + toFIXED(15),
                        actor->z.fixed, 1);
  }
  if (stage == BURST_STAGE_GREAT_BLAST) {
    struct Actor *actor = work->child;
    struct Actor *target;

    MagmaRock_DropRocks(actor->x.fixed, actor->y.fixed + toFIXED(15),
                        actor->z.fixed, 8);
    target = work->child;
    SET_ACTOR_POSITION(target, 0, 0, 0);
  }
  if (frame == PSYNERGY_EVENT_LAST_FRAME)
    Rock_EndFalling();
  if (frame == PILLAR_CRUMBLE_FRAME)
    Actor_SelectAnimation(work->child, 3);
}

void Actor_Release(struct Actor *part);

/// The update of one of the four parts of a statue's flame column
/// (MagmaRock_BurnStatue): it rises and sinks with the statue's burning, and is
/// released once sunk.
void MagmaRock_RaiseFlamePart(struct Actor *part) {
  struct Actor *owner = part->watchedActor;

  if (owner->timer > FLAME_RISE_FRAME &&
      owner->timer < part->flameLevel * 2 + FLAME_RISE_FRAME + 1) {
    part->y.fixed = owner->y.fixed + toFIXED(32);
    part->y.fixed +=
        owner->timer * FLAME_SPEED - FLAME_RISE_FRAME * FLAME_SPEED;
  } else if (part->flameLevel * 2 + FLAME_RISE_FRAME + 1 < owner->timer &&
             owner->timer < FLAME_SINK_FRAME) {
    part->y.fixed =
        owner->y.fixed + part->flameLevel * FLAME_LEVEL_HEIGHT + toFIXED(32);
  } else if (owner->timer > FLAME_SINK_FRAME &&
             owner->timer < part->flameLevel * 2 + FLAME_SINK_FRAME + 1) {
    part->y.fixed =
        owner->y.fixed + part->flameLevel * FLAME_LEVEL_HEIGHT + toFIXED(32);
    part->y.fixed -=
        owner->timer * FLAME_SPEED - FLAME_SINK_FRAME * FLAME_SPEED;
  } else if (part->flameLevel * 2 + FLAME_SINK_FRAME + 1 < owner->timer) {
    Actor_Release(part);
  }
}

/// The update of a statue's cap on top of its flame column: it rises and sinks
/// with the column, a frame ahead, throws a flame puff every eighth frame, and
/// raises the statue's cell with it, so that the leader on the cap rides up.
void MagmaRock_RaiseFlameTop(union FieldObject *object) {
  struct Actor *watched = (struct Actor *)object->effect.countdown;
  struct EffectOptions config;
  s32 step;
  s32 mask;

  if (watched->timer >= FLAME_RISE_FRAME &&
      watched->timer <= FLAME_RISE_FRAME + 7) {
    object->effect.y = watched->y.fixed + toFIXED(24);
    object->effect.y = object->effect.y + watched->timer * FLAME_SPEED -
                       (FLAME_RISE_FRAME - 1) * FLAME_SPEED;
  } else {
    if (watched->timer >= 9 && watched->timer < FLAME_SINK_FRAME) {
      object->effect.y = watched->y.fixed +
                         object->effect.riseCounter * FLAME_LEVEL_HEIGHT +
                         toFIXED(24);
    } else if (watched->timer > FLAME_SINK_FRAME &&
               watched->timer <
                   object->effect.riseCounter * 2 + FLAME_SINK_FRAME + 1) {
      object->effect.y = watched->y.fixed +
                         object->effect.riseCounter * FLAME_LEVEL_HEIGHT +
                         toFIXED(24);
      object->effect.y -=
          watched->timer * FLAME_SPEED - FLAME_SINK_FRAME * FLAME_SPEED;
    } else if (object->effect.riseCounter * 2 + FLAME_SINK_FRAME + 1 <
               watched->timer) {
      object->effect.y = object->actor.groundHeight;
      object->effect.update = 0;
    }
  }

  mask = 7;
  step = gFrameCount & mask;
  if (step == 0) {
    config.type = SPRITE_FLAME_PUFF;
    config.palette = SPRITE_COLOR_RED_TINT;
    config.priority = 2;
    config.update = (void (*)(union FieldObject *))MagmaRock_DriftAndSlow;
    config.script = gMagmaRock_EmberScript;
    FieldEffect_Spawn(object->effect.x + toFIXED(1),
                      object->effect.y - toFIXED(4), object->effect.z,
                      ((Random16() & mask) - 3) << 14, -toFIXED(2), step,
                      EFFECT_USE_UPDATE | EFFECT_USE_SCRIPT | EFFECT_USE_TYPE |
                          EFFECT_USE_PRIORITY | EFFECT_USE_PALETTE,
                      &config);
  }
  MagmaRockInside_SetCellShape(watched->layer, FIELD_CELL(watched->x.fixed),
                               FIELD_CELL(watched->z.fixed),
                               CELL_HEIGHT_STEPS(object->effect.y) + 1);
}

void Actor_PlaySoundInView(struct Actor *actor, s32 a);

/// A statue's update once it burns: embers first, then at frame 33 its flame
/// column of four parts rises, and the cap on its top with it
/// (MagmaRock_RaiseFlameTop).
void MagmaRock_BurnStatue(struct Actor *actor) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  s32 type = SPRITE_STEAM_VENT;

  if ((gFrameCount & 15) == 0)
    Actor_PlaySoundInView(actor, SE_WHOOSH);
  FieldActor_SetSpriteColor(actor, SPRITE_COLOR_RED_TINT);
  if (actor->timer <= 31) {
    u32 phase = gFrameCount & 3;

    if (phase == 0) {
      struct EffectOptions config;
      s32 randomX;
      s32 randomZ;

      config.type = SPRITE_FLAME_PUFF;
      config.palette = SPRITE_COLOR_RED_TINT;
      config.priority = 2;
      config.update = (void (*)(union FieldObject *))MagmaRock_DriftAndSlow;
      config.script = gMagmaRock_EmberScript;
      // Each ember flies off at up to 2 pixels a frame across the ground.
      randomX = Random16() & 15;
      randomZ = Random16() & 15;
      FieldEffect_Spawn(
          actor->x.fixed + toFIXED(1), actor->y.fixed + toFIXED(24),
          actor->z.fixed, (randomX - 8) << 14, 0, (randomZ - 8) << 14,
          EFFECT_USE_UPDATE | EFFECT_USE_SCRIPT | EFFECT_USE_TYPE |
              EFFECT_USE_PRIORITY | EFFECT_USE_PALETTE,
          &config);
    }
  } else if (actor->timer == 33) {
    struct Actor *parts[4];
    struct Actor *fifthPart;
    s32 id;
    s32 i;

    Actor_PlaySoundInView(actor, SE_CLATTER);
    parts[0] =
        (FieldActor_Create(type, actor->x.fixed + toFIXED(1),
                           actor->y.fixed + toFIXED(32), actor->z.fixed));
    for (i = 1; i != 4; i++) {
      struct Sprite *first = parts[0]->sprite;
      struct Actor *part = (FieldActor_Create(type, 0, 0, 0));
      struct Sprite *sprite;

      parts[i] = part;
      sprite = part->sprite;
      sprite->slot = first->slot;
      sprite->retainCacheEntry = 1;
      part->x.fixed = parts[0]->x.fixed;
      part->y.fixed = parts[0]->y.fixed;
      part->z.fixed = parts[0]->z.fixed;
      part->motionFlags = 0;
      Actor_SetSpriteFlags(part, 0);
      Actor_SelectAnimation(parts[i], 3);
      {
        struct Actor *flame = parts[i];

        flame->flameLevel = 3 - i;
        flame->watchedActor = actor;
        flame->update = MagmaRock_RaiseFlamePart;
        FieldActor_SetSpriteColor(flame, SPRITE_COLOR_BGR);
      }
    }
    {
      struct Actor *part = parts[0];

      part->motionFlags = 0;
      Actor_SetSpriteFlags(part, 0);
      Actor_SelectAnimation(parts[0], 3);
      Scene_SetActorSpritePriority(parts[0], 3);
      {
        struct Actor *flame = parts[0];

        flame->flameLevel = 3;
        flame->watchedActor = actor;
        flame->update = MagmaRock_RaiseFlamePart;
        FieldActor_SetSpriteColor(flame, SPRITE_COLOR_BGR);
      }
    }
    id = 0;
    if (gMainRoutineWork.next == SCENE_MAGMA_ROCK_SLIDE) {
      id = MagmaRock_LookUpCellStatue(actor->x.fixed, actor->z.fixed,
                                      gMagmaRock_SlideStatues);
    } else if (gMainRoutineWork.next ==
               SCENE_MAGMA_ROCK_SALAMANDER_TAIL_CHEST) {
      s16 entrance = gMainRoutineWork.entrance;
      FIXED x = actor->x.fixed;
      FIXED z = actor->z.fixed;

      if (entrance <= SALAMANDER_TAIL_HIGH_WEST_LAST_ENTRANCE)
        id = MagmaRock_LookUpCellStatue(
            x, z, gMagmaRock_SalamanderTailHighWestStatues);
      else if (entrance <= SALAMANDER_TAIL_HIGH_EAST_LAST_ENTRANCE)
        id = MagmaRock_LookUpCellStatue(
            x, z, gMagmaRock_SalamanderTailHighEastStatues);
      else if (entrance <= SALAMANDER_TAIL_LOW_WEST_LAST_ENTRANCE)
        id = MagmaRock_LookUpCellStatue(
            x, z, gMagmaRock_SalamanderTailLowWestStatues);
      else
        id = MagmaRock_LookUpCellStatue(
            x, z, gMagmaRock_SalamanderTailLowEastStatues);
    }
    fifthPart = Actor_Get(id);
    fifthPart->flameLevel = 4;
    fifthPart->watchedActor = actor;
    fifthPart->update = MagmaRock_RaiseFlameTop;
    actor->timer++;
  } else if (actor->timer > 364) {
    FieldActor_SetSpriteColor(actor, SPRITE_COLOR_NORMAL);
    actor->update = NULL;
  }
  if (work->psynergy == 0 && work->fieldMenu == 0 && work->pauseMenu == 0 &&
      work->item == 0 && (u16)work->transitionActive != 0)
    actor->timer++;
}

/// Burst at a cracked pillar on the slide or on the cliff: the pillar breaks
/// (MagmaRock_BurstStatue) and its flag keeps it gone until the party leaves.
#define DEFINE_BURST_SLIDE_OR_CLIFF_PILLAR(name, slidePillar, cliffPillar,     \
                                           flag)                               \
  void name(s32 stage, s32 frame) {                                            \
    struct FieldEffectWork *work = Ram_HeapSlots->fieldEffectWork;             \
    struct MainRoutineWork *party = &gMainRoutineWork;                         \
                                                                               \
    {                                                                          \
      s32 scene = party->next;                                                 \
                                                                               \
      if (scene == SCENE_MAGMA_ROCK_SLIDE)                                     \
        work->child = Actor_Get(slidePillar);                                  \
      else if (scene == SCENE_MAGMA_ROCK_SALAMANDER_TAIL_CHEST)                \
        work->child = Actor_Get(cliffPillar);                                  \
      MagmaRock_BurstStatue(stage, frame);                                     \
    }                                                                          \
    Event_SetFlag(flag);                                                       \
  }

/// Burst breaks the north-west one of the four cracked pillars in front of the
/// slide's cave mouth (cell (15, 31)), or the cracked pillar on the cliff's
/// high west level (cell (22, 34)). Either sets `FLAG_CRACKED_PILLAR_A_BURST`.
/// The high west statue's fireball breaks that pillar through the statue's own
/// rows instead (MagmaRock_BurstStatue), not this.
DEFINE_MAGMA_ROCK_BURST_COLUMN(MagmaRock_BurstCrackedPillarA,
                               ACTOR_CRACKED_PILLAR_A,
                               ACTOR_HIGH_WEST_CRACKED_PILLAR_A,
                               FLAG_CRACKED_PILLAR_A_BURST)

/// Burst breaks the north-east one of the four cracked pillars in front of the
/// slide's cave mouth (cell (17, 31)), or the cracked pillar on the cliff's low
/// east level (cell (42, 39)) as its statue's fireball lands on it. Either sets
/// `FLAG_CRACKED_PILLAR_B_BURST`.
DEFINE_BURST_SLIDE_OR_CLIFF_PILLAR(MagmaRock_BurstCrackedPillarB,
                                   ACTOR_CRACKED_PILLAR_B,
                                   ACTOR_LOW_EAST_CRACKED_PILLAR,
                                   FLAG_CRACKED_PILLAR_B_BURST)

/// Defines what Burst does to a cracked pillar: it breaks the pillar actor
/// (MagmaRock_BurstStatue) and sets its flag.
#define DEFINE_BURST_CRACKED_PILLAR(name, pillar, flag)                        \
  void name(s32 stage, s32 frame) {                                            \
    struct FieldEffectWork *work = Ram_HeapSlots->fieldEffectWork;             \
                                                                               \
    work->child = Actor_Get(pillar);                                           \
    MagmaRock_BurstStatue(stage, frame);                                       \
    Event_SetFlag(flag);                                                       \
  }

/// Burst breaks the south-west one of the four cracked pillars in front of the
/// slide's cave mouth (cell (14, 34)). It sets `FLAG_CRACKED_PILLAR_C_BURST`.
DEFINE_BURST_CRACKED_PILLAR(MagmaRock_BurstCrackedPillarC,
                            ACTOR_CRACKED_PILLAR_C, FLAG_CRACKED_PILLAR_C_BURST)

/// Burst breaks the south-east one of the four cracked pillars in front of the
/// slide's cave mouth (cell (18, 34)). It sets `FLAG_CRACKED_PILLAR_D_BURST`.
DEFINE_BURST_CRACKED_PILLAR(MagmaRock_BurstCrackedPillarD,
                            ACTOR_CRACKED_PILLAR_D, FLAG_CRACKED_PILLAR_D_BURST)

/// The fireball of the statue at the east end of the slide's upper ledge
/// lands on the cracked pillar west of it (cell (20, 25)), and the blast breaks
/// the pillar, opening the climb up to the ledge where it stood. It sets
/// `FLAG_CRACKED_PILLAR_E_BURST`.
DEFINE_BURST_CRACKED_PILLAR(MagmaRock_BurstCrackedPillarE,
                            ACTOR_CRACKED_PILLAR_E, FLAG_CRACKED_PILLAR_E_BURST)

/// Nothing visible happens when Felix, come out of the Ember Stone's room
/// (MAGMA_ROCK_SLIDE_FROM_MAGMA_ROCK_INSIDE_EMBER_STONE), takes his first step
/// of his own in the cave mouth above the slide's cracked pillars (cell
/// (16, 28)). He arrives standing in the mouth facing out, and the game's
/// placing him there only sets `FLAG_CAVE_DOOR_USED` (MagmaRock_EnterCave), so
/// he is not sent straight back in; this step clears it, so that walking north
/// into the cave takes him back inside from then on.
void MagmaRock_ResetCaveDoor(void) {
  struct MainRoutineWork *party = &gMainRoutineWork;

  if (party->movementMode != LEADER_MOVEMENT_KEYS)
    Event_ClearFlag(FLAG_CAVE_DOOR_USED);
}

/// Felix walks north into the dark cave mouth above the slide's cracked
/// pillars (cell (16, 28)) and goes inside Magma Rock, to the room of the Ember
/// Stone. The game walks him a step further in, plays the exit sound and
/// leaves; when the game rather than the player moved him onto the cell, it
/// only sets `FLAG_CAVE_DOOR_USED`.
void MagmaRock_EnterCave(void) {
  struct MainRoutineWork *party = &gMainRoutineWork;

  if (party->movementMode != LEADER_MOVEMENT_KEYS) {
    Event_Start();
    FieldEvent_ResolvePendingActions(0);
    Actor_Get(party->leaderActorId)->motionFlags = 0;
    Actor_WalkTo(party->leaderActorId, 264, 440);
    FieldEvent_Wait(5);
    Event_PlaySound(SE_MAP_EXIT);
    FieldEvent_SetExitRequest(
        MAGMA_ROCK_SLIDE_TO_MAGMA_ROCK_INSIDE_EMBER_STONE);
    MagmaRock_EndEvent();
  }
  Event_SetFlag(FLAG_CAVE_DOOR_USED);
}

/// Nothing visible happens as Felix begins to cast Burst at one of the yellow
/// statues; the game settles whatever the field has pending before the
/// statue's own events run (MagmaRock_TargetStatue).
void MagmaRock_ResolveBeforeBurst(void) { Event_ResolvePendingActions(0); }

/// Felix comes down on one of the two dark pillars on the cliff's high west
/// level (cells (15, 29) and (17, 29)) without hopping straight on: the first
/// time it cracks with a flash and a puff of dust, the second time it crumbles
/// under him and he drops with it to the ground below
/// (MagmaRock_StepOnCrumblingPillar).
void MagmaRock_CheckCrumblingPillars(void) {
  MagmaRock_StepOnCrumblingPillar(gMagmaRock_CrumblingPillars);
}

/// Nothing visible happens when Felix steps onto the cell just behind the
/// yellow statue on the cliff's high west level (cell (15, 34)); from then on
/// Burst cast at the statue wakes it, so that it spits a fireball east at the
/// cracked pillar. The game makes the statue's cell answer Burst as a statue
/// that spits fire, and sets `FLAG_HIGH_WEST_STATUE_READY`.
void MagmaRock_ReadyHighWestStatue(void) {
  Field_SetCellBehaviorAt(MAPGRID_LAYER_GROUND, toFIXED(264), toFIXED(552),
                          CELL_BEHAVIOR_MAGMA_ROCK_FIRING_STATUE);
  Event_SetFlag(FLAG_HIGH_WEST_STATUE_READY);
}

/// Nothing visible happens when Felix steps a cell further west from behind
/// the high west level's yellow statue (cell (14, 34)); Burst no longer wakes
/// the statue until he steps back behind it. The game makes the statue's cell
/// plain ground again and clears `FLAG_HIGH_WEST_STATUE_READY`.
void MagmaRock_ResetHighWestStatue(void) {
  Field_SetCellBehaviorAt(MAPGRID_LAYER_GROUND, toFIXED(264), toFIXED(552),
                          CELL_BEHAVIOR_NORMAL);
  Event_ClearFlag(FLAG_HIGH_WEST_STATUE_READY);
}

void Actor_SetAnimationSpeedById(s32 id, s32 action);
void FieldCamera_WaitForArrival(void);

/// A fireball flying between a pair of yellow statues on the cliff's high east
/// wall hits Felix as he climbs: he turns grey among dark puffs, falls to the
/// foot of the wall with a thump that shakes the ground, and shakes his head.
void MagmaRock_KnockLeaderOffWall(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  struct EffectOptions options;
  s32 position[3];
  s32 i;
  u32 phase;

  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetFlag(FLAG_LEADER_KNOCKED_DOWN);
  Event_PlaySound(SE_IMPACT);
  FieldActor_SetSpriteColor(Actor_GetById(gMainRoutineWork.leaderActorId),
                            SPRITE_COLOR_GREY);
  Actor_SetAnimation(gMainRoutineWork.leaderActorId, ANIM_LEADER_CLIMB);
  Actor_SetAnimationSpeedById(gMainRoutineWork.leaderActorId,
                              ANIM_STEP_NORMAL * 2);
  options.priority = 1;
  options.palette = SPRITE_COLOR_BLACK;
  options.startScaleX = FIXED_ONE * 4 / 5;
  options.startScaleY = FIXED_ONE * 4 / 5;
  for (i = 0; i != 8; i++) {
    position[0] = toFIXED(Random16() & 15) - toFIXED(8);
    position[1] = (Random16() & 15) << 16;
    FieldEffect_Spawn(
        leader->x.fixed + position[0], leader->y.fixed + position[1],
        leader->z.fixed, 0, toFIXED(6) / 5, 0,
        EFFECT_USE_START_SCALE | EFFECT_USE_PRIORITY | EFFECT_USE_PALETTE,
        &options);
    wait(5);
  }
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  gMainRoutineWork.movementMode = LEADER_MOVEMENT_WALK;
  leader->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  leader->gravity = toFIXED(4) / 5;
  leader->bounce = 0;
  Event_Wait(1);
  leader->facing = FACING_NORTH;
  // He falls for up to 60 frames, until he lands, a dark puff rising round him
  // every other frame.
  i = 60;
  do {
    phase = gFrameCount & 1;
    if (phase == 0) {
      position[0] = toFIXED(Random16() & 15) - toFIXED(8);
      position[1] = (Random16() & 15) << 16;
      FieldEffect_Spawn(
          leader->x.fixed + position[0], leader->y.fixed + position[1],
          leader->z.fixed, 0, toFIXED(6) / 5, phase,
          EFFECT_USE_START_SCALE | EFFECT_USE_PRIORITY | EFFECT_USE_PALETTE,
          &options);
    }
    wait(1);
    if (leader->velocityY == 0)
      break;
    i--;
  } while (i != 0);
  Event_PlaySound(SE_THUMP);
  Actor_SetSpriteFlags(leader, 1);
  Actor_SetAnimation(gMainRoutineWork.leaderActorId, ANIM_LEADER_LEAN_BACK);
  MapGrid_SetShake(5, 8, 1);
  MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
  MapGrid_WaitForShakeToSettle();
  FieldCamera_MoveToActor(gMainRoutineWork.leaderActorId, 1);
  FieldCamera_WaitForArrival();
  Actor_SetAnimation(gMainRoutineWork.leaderActorId, ANIM_SHAKE_HEAD);
  FieldActor_SetSpriteColor(Actor_GetById(gMainRoutineWork.leaderActorId),
                            SPRITE_COLOR_NORMAL);
  Actor_WaitForAnimationChange(gMainRoutineWork.leaderActorId);
  Event_ClearFlag(FLAG_LEADER_KNOCKED_DOWN);
  leader->gravity = toFIXED(1);
  leader->bounce = FIXED_ONE / 4;
  MagmaRock_EndEvent();
}

void MagmaRockInside_ClearBlockCells(struct Actor *actor);
void MagmaRockInside_PushBlockOnto(void);

/// Felix walks east into the striped stone pillar on the cliff's low west level
/// from cell (14, 36) and pushes it a cell east, to cell (16, 36), where it
/// stays when the party comes back. The game takes the pillar out of the map,
/// clears the upper layer's walls along its path for the push
/// (MagmaRockInside_PushBlockOnto), puts them back and stands it in the map
/// again.
void MagmaRock_PushPillarEast(void) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.leaderActorId);
  s32 position[3];
  struct Actor *actor;
  u8 *terrainId;
  s32 x;
  s32 z;
  s32 height;

  position[0] = leader->x.fixed + toFIXED(16);
  position[1] = leader->y.fixed;
  position[2] = leader->z.fixed;
  actor = Actor_FindInCell(position, leader);
  MagmaRockInside_ClearBlockCells(actor);
  terrainId = &actor->layer;
  x = FIELD_CELL(actor->x.fixed);
  z = FIELD_CELL(actor->z.fixed);
  height = CELL_HEIGHT_STEPS(actor->groundHeight);
  MagmaRockInside_SetCellShape(*terrainId, x, z, height);
  MapGrid_CopyCellAttributes(79, 46, 2, 1, 79, 36);
  MagmaRockInside_PushBlockOnto();
  *terrainId = 0;
  MapGrid_CopyCellAttributes(79, 37, 2, 1, 79, 36);
  MagmaRockInside_SetCellShape(*terrainId, FIELD_CELL(actor->x.fixed),
                               FIELD_CELL(actor->z.fixed),
                               CELL_HEIGHT_STEPS(actor->groundHeight) + 4);
  Blocks_MarkCells(gMagmaRock_SalamanderTailLowWestPillars);
  position[0] = FIELD_CELL(actor->x.fixed);
  position[2] = FIELD_CELL(actor->z.fixed);
  if (position[0] == 16 && position[2] == 36)
    Event_SetFlag(FLAG_LOW_WEST_PILLAR_PUSHED);
}

/// Felix walks west into the striped stone pillar beside the cracked pillar on
/// the cliff's low east level, from any of cells (37, 38) to (40, 38), and
/// pushes it a cell west along a grey track; after four pushes it stands on the
/// grey square at the track's end, cell (35, 38), and stays there when the
/// party comes back. The game moves the pillar as MagmaRock_PushPillarEast
/// does.
void MagmaRock_PushPillarWest(void) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.leaderActorId);
  s32 position[3];
  struct Actor *actor;
  u8 *terrainId;
  s32 x;
  s32 z;
  s32 height;

  position[0] = leader->x.fixed - toFIXED(16);
  position[1] = leader->y.fixed;
  position[2] = leader->z.fixed;
  actor = Actor_FindInCell(position, leader);
  MagmaRockInside_ClearBlockCells(actor);
  terrainId = &actor->layer;
  x = FIELD_CELL(actor->x.fixed);
  z = FIELD_CELL(actor->z.fixed);
  height = CELL_HEIGHT_STEPS(actor->groundHeight);
  MagmaRockInside_SetCellShape(*terrainId, x, z, height);
  MapGrid_CopyCellAttributes(94, 44, 5, 1, 99, 38);
  MagmaRockInside_PushBlockOnto();
  *terrainId = 0;
  MapGrid_CopyCellAttributes(99, 42, 5, 1, 99, 38);
  MagmaRockInside_SetCellShape(*terrainId, FIELD_CELL(actor->x.fixed),
                               FIELD_CELL(actor->z.fixed),
                               CELL_HEIGHT_STEPS(actor->groundHeight) + 4);
  Blocks_MarkCells(gMagmaRock_SalamanderTailLowEastPillars);
  position[0] = FIELD_CELL(actor->x.fixed);
  position[2] = FIELD_CELL(actor->z.fixed);
  if (position[0] == 35 && position[2] == 38)
    Event_SetFlag(FLAG_LOW_EAST_PILLAR_B_PUSHED);
}

/// Growth on the west one of the two sprouts on the north-east ledge of the
/// cliff's low east level (cell (59, 39)) grows a vine up the wall above it.
/// The game opens the three cells of wall above the sprout for climbing and
/// sets `FLAG_VINE_A_GROWN`.
void MagmaRock_GrowVineA(void) {
  MapGrid_CopyCellAttributes(121, 48, 1, 3, 123, 48);
  Event_SetFlag(FLAG_VINE_A_GROWN);
}

/// Growth on the east one of the two sprouts on the north-east ledge of the
/// cliff's low east level (cell (61, 39)) grows a vine up the wall above it.
/// The game opens the three cells of wall above the sprout for climbing and
/// sets `FLAG_VINE_B_GROWN`.
void MagmaRock_GrowVineB(void) {
  MapGrid_CopyCellAttributes(121, 48, 1, 3, 125, 48);
  Event_SetFlag(FLAG_VINE_B_GROWN);
}

/// Growth on the sprout on the path where Felix comes up from the slide, on
/// the cliff's low east level (cell (45, 41)), grows a vine up the wall above
/// it. The game opens the three cells of wall above the sprout for climbing and
/// sets `FLAG_VINE_C_GROWN`.
void MagmaRock_GrowVineC(void) {
  MapGrid_CopyCellAttributes(108, 56, 1, 3, 109, 56);
  Event_SetFlag(FLAG_VINE_C_GROWN);
}

/// Four large flame puffs scatter round (x, z) in the blast at the great fire,
/// and the way in stays open for good.
void MagmaRock_ScatterEmbers(s32 x, s32 z) {
  struct EffectOptions config;
  s32 position[3];
  s32 i;
  s32 kind;

  config.startScaleX = FIXED_ONE * 2;
  config.startScaleY = FIXED_ONE * 2;
  config.type = SPRITE_FLAME_PUFF;
  kind = 1;
  config.script = gMagmaRock_EmberScript;
  config.priority = kind;
  for (i = 0; i != 4; i++) {
    position[0] = x + toFIXED(Random16() & 63) - FIXED_ONE * 32;
    position[1] = (Random16() & 15) << 12;
    position[2] = z + toFIXED(Random16() & 63) - FIXED_ONE * 32;
    FieldEffect_Spawn(position[0], 0, position[2], 0, position[1], 0,
                      EFFECT_USE_SCRIPT | EFFECT_USE_TYPE |
                          EFFECT_USE_START_SCALE | EFFECT_USE_PRIORITY | 1,
                      &config);
  }
  Event_SetFlag(FLAG_WAY_IN_BLASTED);
}

/// The great fire's update as Burst plays on it: it blinks white on the frames
/// its mask picks.
void MagmaRock_FlashTarget(struct Actor *actor) {
  if (gFrameCount & actor->flashMask)
    FieldActor_SetSpriteColor(actor, SPRITE_COLOR_WHITE);
  else
    FieldActor_SetSpriteColor(actor, SPRITE_COLOR_NORMAL);
}

/// The statue's fireball explodes on the great fire on the top of Magma Rock:
/// the fire blinks white as flame puffs scatter and rocks fly while the
/// explosions shake the ground, and at the last blast the fire is gone and the
/// rocks round it lie scattered, opening the way down into Magma Rock (cells
/// (42, 17) to (46, 24)). The game copies the open ground and its pictures over
/// those cells from spare ones below the room.
void MagmaRock_BlastWayIn(s32 stage, s32 frame) {
  struct FieldEffectWork *work = Ram_HeapSlots->fieldEffectWork;

  if (frame == 0) {
    struct Actor *actor;

    MagmaRock_StartFallingRocks();
    actor = work->child;
    actor->flashMask = 4;
    actor->update = (void *)MagmaRock_FlashTarget;
  }
  if (stage == BURST_STAGE_SMALL_BLAST) {
    MagmaRock_ScatterEmbers(work->child->x.fixed, work->child->z.fixed);
    MagmaRock_DropRocks(work->child->x.fixed,
                        work->child->y.fixed + toFIXED(15),
                        work->child->z.fixed, 1);
  }
  if (stage == BURST_STAGE_GREAT_BLAST) {
    struct Actor *target;
    s32 last = 8;

    MagmaRock_ScatterEmbers(work->child->x.fixed, work->child->z.fixed);
    MagmaRock_ScatterEmbers(work->child->x.fixed, work->child->z.fixed);
    MagmaRock_ScatterEmbers(work->child->x.fixed,
                            work->child->z.fixed - toFIXED(32));
    MagmaRock_DropRocks(work->child->x.fixed,
                        work->child->y.fixed + toFIXED(15),
                        work->child->z.fixed, 16);
    target = work->child;
    SET_ACTOR_POSITION(target, 0, 0, 0);
    MapGrid_CopyCellAttributes(0, 32, 5, 8, 42, 17);
    MapGrid_CopyCellAttributes(0, 96, 5, 8, 42, 81);
    MapGrid_CopyCellsTo(0, 32, 42, 17, 5, last);
    MapGrid_CopyCellsTo(0, 96, 42, 81, 5, last);
  }
  if (frame == PSYNERGY_EVENT_LAST_FRAME)
    Rock_EndFalling();
}

extern struct Trigger gMagmaRock_Triggers[];
extern struct Trigger gMagmaRock_SalamanderTailHighWestTriggers[];
extern struct Trigger gMagmaRock_SalamanderTailHighEastTriggers[];
extern struct Trigger gMagmaRock_SalamanderTailLowWestTriggers[];
extern struct Trigger gMagmaRock_SalamanderTailLowEastTriggers[];
extern struct Trigger gMagmaRock_StatueTriggers[];

const struct Trigger *Scene_GetTriggers(void) {
  if (gMainRoutineWork.next == SCENE_MAGMA_ROCK_SALAMANDER_TAIL_CHEST) {
    if (gMainRoutineWork.entrance <= SALAMANDER_TAIL_HIGH_WEST_LAST_ENTRANCE)
      return gMagmaRock_SalamanderTailHighWestTriggers;
    if (gMainRoutineWork.entrance <= SALAMANDER_TAIL_HIGH_EAST_LAST_ENTRANCE)
      return gMagmaRock_SalamanderTailHighEastTriggers;
    if (gMainRoutineWork.entrance <= SALAMANDER_TAIL_LOW_WEST_LAST_ENTRANCE)
      return gMagmaRock_SalamanderTailLowWestTriggers;
    return gMagmaRock_SalamanderTailLowEastTriggers;
  }
  if (gMainRoutineWork.next == SCENE_MAGMA_ROCK_STATUE)
    return gMagmaRock_StatueTriggers;
  return gMagmaRock_Triggers;
}

#include "../field/field_effect_advance_motion.c"

#undef DEFINE_BURST_SLIDE_OR_CLIFF_PILLAR
#undef DEFINE_BURST_CRACKED_PILLAR
#undef EFFECT_SET_MODE
#undef EFFECT_SET_SCRIPT
#undef EFFECT_SPRITE_PRIORITY

FIXED Trig_Cos(s32 angle);
void FieldEffect_AdvanceMotion();
void MapGrid_CenterOnFollowedPoint(void);

/// Felix arrives on the cliff's high east level by dropping from the top
/// (MAGMA_ROCK_SALAMANDER_TAIL_CHEST_FROM_MAGMA_ROCK_STATUE_EAST_A): he falls
/// in from 130 pixels up, lands with a thump in a ring of dust and leans back.
void MagmaRock_LaunchLeader(void) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.leaderActorId);
  struct EffectOptions options;
  u32 i;
  s32 angle;
  FIXED velocity[3];

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  MapGrid_CenterOnFollowedPoint();
  System_Wait(1);
  leader->y.fixed += toFIXED(130);
  leader->gravity = toFIXED(1) / 2;
  leader->bounce = 0;
  leader->motionFlags = 0;
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_PlaySound(SE_DUST_PUFF);
  leader->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  FieldEvent_Wait(24);
  options.palette = SPRITE_COLOR_WHITE;
  options.update = FieldEffect_AdvanceMotion;
  options.startScaleX = FIXED_ONE * 4 / 5;
  options.startScaleY = FIXED_ONE * 4 / 5;
  for (i = 0; i <= DUST_RING_PUFFS; i++) {
    angle = i * (TRIG_TURN / DUST_RING_PUFFS);
    velocity[0] = Trig_Cos(angle);
    velocity[1] = 0;
    velocity[2] = Math_Sin(angle);
    velocity[0] += velocity[0] >> 1;
    velocity[0] += ((Random16() * 3) << 14 >> 16) - toFIXED(3) / 8;
    velocity[2] += ((Random16() * 3) << 13 >> 16) - toFIXED(3) / 16;
    FieldEffect_Spawn(leader->x.fixed, leader->y.fixed, leader->z.fixed,
                      velocity[0], velocity[1], velocity[2],
                      EFFECT_USE_UPDATE | EFFECT_USE_START_SCALE |
                          EFFECT_USE_PALETTE | 1,
                      &options);
  }
  Event_PlaySound(SE_THUMP);
  Actor_SetAttachedEffect(gMainRoutineWork.leaderActorId,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ATTACH);
  Actor_SetAnimation(gMainRoutineWork.leaderActorId, ANIM_LEADER_LEAN_BACK);
  MapGrid_SetShake(5, 5, 1);
  MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
  FieldMapGrid_WaitForShakeToSettle();
  Actor_SetAttachedEffect(gMainRoutineWork.leaderActorId,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_REMOVE);
  leader->gravity = toFIXED(1);
  leader->bounce = FIXED_ONE / 4;
  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
  MagmaRock_EndEvent();
}

#include "shared/leader_lift.h"

s32 Map_GetTerrainHeight(s32 terrain, s32 x, s32 z);

/// Arriving with the party's place kept: sets the leader on the ground under
/// it.
void MagmaRock_LandLeader(void) {
  struct Actor *actor = Actor_Get(gMainRoutineWork.leaderActorId);

  if (Event_GetFlag(FLAG_KEEP_PARTY_POSITION)) {
    s32 height;

    System_Wait(1);
    height = Map_GetTerrainHeight(actor->layer, actor->x.fixed, actor->z.fixed);
    actor->velocityY = 0;
    actor->groundHeight = height;
    actor->y.fixed = height;
    actor->targetY = ACTOR_NO_TARGET;
    EventCamera_FollowActor(gMainRoutineWork.leaderActorId, 0);
  }
}

/// Hangs a part of a flame column on its statue at the given level
/// (MagmaRock_RaiseFlamePart).
static inline void AttachPart(struct Actor *part, struct Actor *anchor,
                              u8 order) {
  part->flameLevel = order;
  part->watchedActor = anchor;
  part->update = MagmaRock_RaiseFlamePart;
  FieldActor_SetSpriteColor(part, SPRITE_COLOR_BGR);
}

/// Builds the four parts of a statue's flame column on re-entry, as
/// MagmaRock_BurnStatue does.
static inline void BuildParts(struct Actor *anchor) {
  struct Actor *parts[4];
  struct Actor *part;
  struct Sprite *first;
  struct Sprite *sprite;
  u32 i;

  parts[0] = ((struct Actor *)Import_ActorCreate(
      SPRITE_STEAM_VENT, anchor->x.fixed + toFIXED(1),
      anchor->y.fixed + toFIXED(32), anchor->z.fixed));
  for (i = 1; i != 4; i++) {
    first = parts[0]->sprite;
    part = ((struct Actor *)Import_ActorCreate(SPRITE_STEAM_VENT, 0, 0, 0));
    parts[i] = part;
    sprite = part->sprite;
    sprite->slot = first->slot;
    sprite->retainCacheEntry = 1;
    part->x.fixed = parts[0]->x.fixed;
    part->y.fixed = parts[0]->y.fixed;
    part->z.fixed = parts[0]->z.fixed;
    part->motionFlags = 0;
    Actor_SetSpriteFlags(part, 0);
    Actor_SelectAnimation(parts[i], 3);
    AttachPart(parts[i], anchor, i - 1);
  }
  part = parts[0];
  part->motionFlags = 0;
  Actor_SetSpriteFlags(part, 0);
  Actor_SelectAnimation(parts[0], 3);
  Scene_SetActorSpritePriority(parts[0], 3);
  AttachPart(parts[0], anchor, 3);
}

/// On entry: rebuilds the flame column under a statue's cap that was riding
/// one when Felix left.
void MagmaRock_RebuildFlameParts(u16 id) {
  if (Actor_Get(id)->update != MagmaRock_RaiseFlameTop)
    return;
  BuildParts(Actor_Get(id)->watchedActor);
}

void MagmaRock_SettleBlocks();
/// The overlay's import veneer to Rope_PlaceFar.
void MagmaRock_PlaceRope();

s32 Scene_Initialize(void) {
  struct MainRoutineWork *work;
  s32 scene;
  s32 *owner, *leader;
  s16 *entrance;
  struct Actor *actor;
  // other serves in turn as a table, a step and a size.
  FIXED other, rise;
  FIXED offset;
  s32 priorityBit, destinationX, destinationColumn, width;

  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
  work = &gMainRoutineWork;
  work->retreatScene = SCENE_MAGMA_ROCK_SLIDE;
  work->retreatEntrance = MAGMA_ROCK_SLIDE_FROM_WORLD_MAP;
  scene = work->next;
  if (scene == SCENE_MAGMA_ROCK_SLIDE) {
    priorityBit = ACTOR_PRIORITY_IGNORES_RISES;
    Actor_GetById(work->leaderActorId)->priorityFlags |= priorityBit;
    Actor_GetById(ACTOR_SLIDE_PILLAR_A)->priorityFlags |= priorityBit;
    Actor_GetById(ACTOR_SLIDE_PILLAR_B)->priorityFlags |= priorityBit;
    MagmaRockInside_RaiseActorCells(gMagmaRock_SlideStatues);
    if (Event_GetFlag(FLAG_KEEP_PARTY_POSITION) == 0)
      MagmaRock_PlaceFireball(ACTOR_SLIDE_FIREBALL,
                              ACTOR_SLIDE_PAIR_WEST_STATUE, 0);
    if (Event_GetFlag(FLAG_SLIDE_PILLAR_A_PUSHED))
      Actor_SetPosition(ACTOR_SLIDE_PILLAR_A, 264, 360);
    if (Event_GetFlag(FLAG_SLIDE_PILLAR_B_PUSHED))
      Actor_SetPosition(ACTOR_SLIDE_PILLAR_B, 248, 408);
    other = (s32)gMagmaRock_SlidePillars;
    destinationX = 78;
    Actor_PlaceBlocks(other);
    MapGrid_CopyCellAttributes(98, 21, 4, 3, destinationX, 21);
    MapGrid_CopyCellAttributes(98, 27, 4, 3, destinationX, 24);
    Blocks_MarkCells(other);
    if (Event_GetFlag(FLAG_KEEP_PARTY_POSITION) == 0) {
      actor = Actor_GetById(ACTOR_SLIDE_STATUE_CAP);
      actor->x.fixed += toFIXED(1);
      actor->motionFlags = 0;
      actor->collisionFlags = 0;
      actor->groundHeight = actor->y.fixed += toFIXED(24);
      Actor_SetSpriteFlags(actor, 0);
    }
    MagmaRock_RebuildFlameParts(ACTOR_SLIDE_STATUE_CAP);
    switch (work->entrance) {
    case MAGMA_ROCK_SLIDE_FROM_MAGMA_ROCK_SALAMANDER_TAIL_CHEST_WEST:
    case MAGMA_ROCK_SLIDE_FROM_MAGMA_ROCK_SALAMANDER_TAIL_CHEST_EAST:
      offset = toFIXED(16);
      goto move;
    }
    goto settle;
  } else if (scene == SCENE_MAGMA_ROCK_SALAMANDER_TAIL_CHEST) {
    owner = &work->leaderActorId;
    Actor_GetById(*owner)->priorityFlags |= ACTOR_PRIORITY_IGNORES_RISES;
    entrance = &work->entrance;
    if (work->entrance <= SALAMANDER_TAIL_HIGH_WEST_LAST_ENTRANCE) {
      s32 id;

      MagmaRock_RestoreCrumblingPillars(gMagmaRock_CrumblingPillars);
      MagmaRockInside_RaiseActorCells(gMagmaRock_SalamanderTailHighWestStatues);
      Tightrope_Initialize();
      MagmaRock_PlaceTightrope(0, ACTOR_HIGH_WEST_TIGHTROPE_WEST_POST,
                               ACTOR_HIGH_WEST_TIGHTROPE_EAST_POST);
      if (Event_GetFlag(FLAG_HIGH_WEST_PILLAR_B_PUSHED))
        Actor_SetPosition(ACTOR_HIGH_WEST_PILLAR_B, 440, 536);
      Actor_PlaceBlocks(gMagmaRock_SalamanderTailHighWestPillars);
      id = *entrance;
      if (id == ENTRANCE_HIGH_WEST_FROM_BELOW_A) {
        if (Event_GetFlag(FLAG_KEEP_PARTY_POSITION) == 0) {
          actor = Actor_GetById(*owner);
          actor->z.fixed += toFIXED(16);
        }
        Leader_Lift(toFIXED(-16));
      } else if (id == ENTRANCE_HIGH_WEST_FROM_BELOW_B) {
        if (Event_GetFlag(FLAG_KEEP_PARTY_POSITION) == 0) {
          actor = Actor_GetById(*owner);
          actor->z.fixed += toFIXED(16);
        }
        Leader_Lift(toFIXED(-96));
      }
    } else {
      if (*entrance <= SALAMANDER_TAIL_HIGH_EAST_LAST_ENTRANCE) {
        s32 id;

        if (Event_GetFlag(FLAG_KEEP_PARTY_POSITION) == 0) {
          Actor_GetById(ACTOR_STATUE_PAIR_A_WEST)->motionFlags = 0;
          rise = toFIXED(80);
          Actor_GetById(ACTOR_STATUE_PAIR_A_WEST)->y.fixed += rise;
          Actor_GetById(ACTOR_STATUE_PAIR_A_EAST)->motionFlags = 0;
          Actor_GetById(ACTOR_STATUE_PAIR_A_EAST)->y.fixed += rise;
          MagmaRock_PlaceFireball(ACTOR_FIREBALL_A, ACTOR_STATUE_PAIR_A_WEST,
                                  0);
          Actor_GetById(ACTOR_STATUE_PAIR_B_WEST)->motionFlags = 0;
          rise = toFIXED(160);
          Actor_GetById(ACTOR_STATUE_PAIR_B_WEST)->y.fixed += rise;
          Actor_GetById(ACTOR_STATUE_PAIR_B_EAST)->motionFlags = 0;
          Actor_GetById(ACTOR_STATUE_PAIR_B_EAST)->y.fixed += rise;
          MagmaRock_PlaceFireball(ACTOR_FIREBALL_B, ACTOR_STATUE_PAIR_B_WEST,
                                  1);
          Actor_GetById(ACTOR_STATUE_PAIR_C_WEST)->motionFlags = 0;
          rise = toFIXED(240);
          Actor_GetById(ACTOR_STATUE_PAIR_C_WEST)->y.fixed += rise;
          Actor_GetById(ACTOR_STATUE_PAIR_C_EAST)->motionFlags = 0;
          Actor_GetById(ACTOR_STATUE_PAIR_C_EAST)->y.fixed += rise;
          MagmaRock_PlaceFireball(ACTOR_FIREBALL_C, ACTOR_STATUE_PAIR_C_WEST,
                                  0);
        }
        if (Event_GetFlag(FLAG_HIGH_EAST_PILLAR_PUSHED))
          Actor_SetPosition(ACTOR_HIGH_EAST_PILLAR, 888, 568);
        Actor_PlaceBlocks(gMagmaRock_SalamanderTailHighEastPillars);
        MagmaRockInside_RaiseActorCells(
            gMagmaRock_SalamanderTailHighEastStatues);
        if (Event_GetFlag(FLAG_KEEP_PARTY_POSITION) == 0) {
          actor = Actor_GetById(ACTOR_HIGH_EAST_STATUE_CAP_A);
          other = toFIXED(1);
          actor->x.fixed += other;
          actor->motionFlags = 0;
          actor->collisionFlags = 0;
          rise = toFIXED(24);
          actor->groundHeight = actor->y.fixed += rise;
          Actor_SetSpriteFlags(actor, 0);
          actor = Actor_GetById(ACTOR_HIGH_EAST_STATUE_CAP_B);
          actor->x.fixed += other;
          actor->motionFlags = 0;
          actor->collisionFlags = 0;
          actor->groundHeight = actor->y.fixed += rise;
          Actor_SetSpriteFlags(actor, 0);
        }
        MagmaRock_RebuildFlameParts(ACTOR_HIGH_EAST_STATUE_CAP_A);
        MagmaRock_RebuildFlameParts(ACTOR_HIGH_EAST_STATUE_CAP_B);
        id = *entrance;
        if (id ==
            MAGMA_ROCK_SALAMANDER_TAIL_CHEST_FROM_MAGMA_ROCK_STATUE_WEST) {
          Leader_Lift(toFIXED(288));
        } else if (
            id ==
            MAGMA_ROCK_SALAMANDER_TAIL_CHEST_FROM_MAGMA_ROCK_STATUE_EAST_A) {
          if (Event_GetFlag(FLAG_KEEP_PARTY_POSITION) == 0)
            MagmaRock_LaunchLeader();
        } else if (id == ENTRANCE_HIGH_EAST_FROM_BELOW) {
          if (Event_GetFlag(FLAG_KEEP_PARTY_POSITION) == 0) {
            actor = Actor_GetById(*owner);
            actor->z.fixed += toFIXED(16);
          }
          Leader_Lift(toFIXED(-48));
        } else if (
            id ==
            MAGMA_ROCK_SALAMANDER_TAIL_CHEST_FROM_MAGMA_ROCK_STATUE_EAST_B) {
          offset = toFIXED(104);
          goto move;
        }
      } else if (*entrance <= SALAMANDER_TAIL_LOW_WEST_LAST_ENTRANCE) {
        if (Event_GetFlag(FLAG_LOW_WEST_PILLAR_PUSHED))
          Actor_SetPosition(ACTOR_LOW_WEST_PILLAR, 264, 584);
        rise = (s32)gMagmaRock_SalamanderTailLowWestPillars;
        Actor_PlaceBlocks(rise);
        Blocks_MarkCells(rise);
        Rope_Initialize();
        Call4((void (*)())MagmaRock_PlaceRope, 0, FLAG_LOW_WEST_ROPE_LASHED,
              ACTOR_LOW_WEST_ROPE_EAST_POST, ACTOR_LOW_WEST_ROPE_WEST_POST);
        MagmaRockInside_RaiseActorCells(
            gMagmaRock_SalamanderTailLowWestStatues);
        if (Event_GetFlag(FLAG_KEEP_PARTY_POSITION) == 0) {
          actor = Actor_GetById(ACTOR_LOW_WEST_STATUE_CAP_A);
          other = toFIXED(1);
          actor->x.fixed += other;
          actor->motionFlags = 0;
          actor->collisionFlags = 0;
          rise = toFIXED(24);
          actor->groundHeight = actor->y.fixed += rise;
          Actor_SetSpriteFlags(actor, 0);
          actor = Actor_GetById(ACTOR_LOW_WEST_STATUE_CAP_B);
          actor->x.fixed += other;
          actor->motionFlags = 0;
          actor->collisionFlags = 0;
          actor->groundHeight = actor->y.fixed += rise;
          Actor_SetSpriteFlags(actor, 0);
        }
        MagmaRock_RebuildFlameParts(ACTOR_LOW_WEST_STATUE_CAP_A);
        MagmaRock_RebuildFlameParts(ACTOR_LOW_WEST_STATUE_CAP_B);
        if (*entrance == ENTRANCE_LOW_WEST_FROM_ABOVE_A ||
            *entrance == ENTRANCE_LOW_WEST_FROM_ABOVE_B) {
          offset = toFIXED(48);
          goto move;
        }
      } else {
        MagmaRockInside_RaiseActorCells(
            gMagmaRock_SalamanderTailLowEastStatues);
        if (Event_GetFlag(FLAG_KEEP_PARTY_POSITION) == 0) {
          actor = Actor_GetById(ACTOR_LOW_EAST_STATUE_CAP);
          actor->x.fixed += toFIXED(1);
          actor->motionFlags = 0;
          actor->collisionFlags = 0;
          actor->groundHeight = actor->y.fixed += toFIXED(24);
          Actor_SetSpriteFlags(actor, 0);
        }
        MagmaRock_RebuildFlameParts(ACTOR_LOW_EAST_STATUE_CAP);
        if (Event_GetFlag(FLAG_LOW_EAST_PILLAR_A_PUSHED))
          Actor_SetPosition(ACTOR_LOW_EAST_PILLAR_A, 648, 600);
        if (Event_GetFlag(FLAG_LOW_EAST_PILLAR_B_PUSHED))
          Actor_SetPosition(ACTOR_LOW_EAST_PILLAR_B, 568, 616);
        MagmaRock_SettleBlocks(gMagmaRock_SalamanderTailLowEastPillars);
        if (Event_GetFlag(FLAG_VINE_A_GROWN))
          MapGrid_CopyCellAttributes(121, 48, 1, 3, 123, 48);
        if (Event_GetFlag(FLAG_VINE_B_GROWN))
          MapGrid_CopyCellAttributes(121, 48, 1, 3, 125, 48);
        if (Event_GetFlag(FLAG_VINE_C_GROWN))
          MapGrid_CopyCellAttributes(108, 56, 1, 3, 109, 56);
        if (*entrance == ENTRANCE_LOW_EAST_FROM_ABOVE) {
          Leader_Lift(toFIXED(48));
        } else if (
            *entrance ==
                MAGMA_ROCK_SALAMANDER_TAIL_CHEST_FROM_MAGMA_ROCK_SLIDE_WEST ||
            *entrance ==
                MAGMA_ROCK_SALAMANDER_TAIL_CHEST_FROM_MAGMA_ROCK_SLIDE_EAST) {
          if (Event_GetFlag(FLAG_KEEP_PARTY_POSITION) == 0) {
            actor = Actor_GetById(work->leaderActorId);
            actor->z.fixed += toFIXED(16);
          }
          offset = -toFIXED(32);
        move:
          Leader_Lift(offset);
        }
      }
    settle:
      MagmaRock_LandLeader();
    }
  } else if (scene == SCENE_MAGMA_ROCK_STATUE) {
    if (Event_GetFlag(FLAG_WAY_IN_BLASTED)) {
      Actor_SetPosition(ACTOR_GREAT_FIRE, 0, 0);
      destinationColumn = 42;
      MapGrid_CopyCellAttributes(0, 32, 5, 8, destinationColumn, 17);
      MapGrid_CopyCellAttributes(0, 96, 5, 8, destinationColumn, 81);
      width = 5;
      other = 8;
      MapGrid_CopyCellsTo(0, 32, 42, 17, width, other);
      MapGrid_CopyCellsTo(0, 96, 42, 81, width, other);
    }
    leader = &work->leaderActorId;
    Actor_GetById(*leader)->priorityFlags |= ACTOR_PRIORITY_IGNORES_RISES;
    MagmaRockInside_RaiseActorCells(gMagmaRock_StatueStatues);
    if (work->entrance <=
        MAGMA_ROCK_STATUE_FROM_MAGMA_ROCK_SALAMANDER_TAIL_CHEST_EAST_B) {
      if (Event_GetFlag(FLAG_KEEP_PARTY_POSITION) == 0) {
        actor = Actor_GetById(*leader);
        actor->z.fixed += toFIXED(16);
      }
      Leader_Lift(toFIXED(-32));
    }
  }
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

extern u8 gMapShapeGrid[];

s32 MapGrid_GetTerrainKind(s32 layer, s32 x, s32 z);

/// On entry: sets each listed pillar from its two flags (cracked, then fallen),
/// then lifts the leader onto the ground under it.
void MagmaRock_RestoreCrumblingPillars(const u16 *table) {
  struct MapWork *map = Ram_HeapSlots->mapGrid;
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

  for (; *table != ACTOR_LIST_END; table += 2) {
    id = table[0];
    actor = Actor_Get(id);
    flag = table[1];
    nextFlag = flag + 1;
    cells = map->layers[actor->layer].cells;
    shapes = gMapShapeGrid + (cells - (struct MapCell *)Ram_MapCellBuffer);
    if (Event_GetFlag(nextFlag)) {
      Actor_SetPosition(id, 0, 0);
    } else {
      s32 shape;
      u8 *slot;
      s32 height;

      index = Actor_GetCellIndex(actor);
      cell = &cells[index];
      slot = shapes + index;
      shape =
          MapGrid_GetTerrainKind(actor->layer, actor->x.fixed, actor->z.fixed);
      height =
          Map_GetTerrainHeight(actor->layer, actor->x.fixed, actor->z.fixed);
      Actor_DisableById(id);
      cell->attributeB = MAP_CELL_JUMP;
      *slot = MapCollision_FindOrAddTag(actor->layer, shape,
                                        CELL_HEIGHT_STEPS(height) + 4);
      if (Event_GetFlag(flag))
        Actor_SelectAnimation(actor, 0);
    }
  }
  leader = Actor_Get(gMainRoutineWork.leaderActorId);
  ground =
      Map_GetTerrainHeight(leader->layer, leader->x.fixed, leader->z.fixed);
  if (leader->y.fixed < ground) {
    leader->groundHeight = ground;
    leader->y.fixed = ground;
  }
}

void MagmaRock_SpawnDustRing(void);

/// Felix on a crumbling pillar: the first time it cracks with a flash, the
/// second time it crumbles and he drops with it, landing with a thump in a ring
/// of dust.
void MagmaRock_StepOnCrumblingPillar(u16 *list) {
  struct Actor *leader;
  struct Actor *target;
  FIXED position[3];

  if (Field_TryJumpForward() != 0)
    return;
  leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  position[0] = leader->x.fixed;
  position[1] = leader->y.fixed;
  position[2] = leader->z.fixed;
  target = Actor_FindAtCell(position, 0);
  if (target == NULL)
    return;
  while (list[0] != ACTOR_LIST_END) {
    s32 id = list[0];

    if (Actor_GetById(id) != target) {
      list += 2;
      continue;
    }
    {
      s32 flag = list[1];

      Event_Start();
      if (!Event_GetFlag(flag)) {
        Event_PlaySound(SE_CHEST_CLOSE);
        FieldActor_SetSpriteColor(Actor_GetById(id), SPRITE_COLOR_WHITE);
        wait(2);
        Actor_SelectAnimation(target, 0);
        FieldActor_SetSpriteColor(Actor_GetById(id), SPRITE_COLOR_NORMAL);
        wait(2);
        FieldActor_SetSpriteColor(Actor_GetById(id), SPRITE_COLOR_WHITE);
        wait(4);
        FieldActor_SetSpriteColor(Actor_GetById(id), SPRITE_COLOR_NORMAL);
        Dust_Scatter(0);
        Event_SetFlag(flag);
      } else if (!Event_GetFlag(flag + 1)) {
        leader->motionFlags = 0;
        Event_PlaySound(SE_PUSH);
        MapGrid_SetShake(1, 4, 1);
        Dust_Scatter(0);
        wait(8);
        Actor_SelectAnimation(target, 2);
        Leader_SetCellShapeAt(-2, target->layer);
        Dust_Scatter(1);
        wait(16);
        Leader_SetCellShapeAt(-2, target->layer);
        wait(4);
        MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
        wait(8);
        leader->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
        wait(5);
        Actor_SetPositionAndResetMotion(target, 0, 0, 0);
        wait(2);
        Event_PlaySound(SE_THUMP);
        MagmaRock_SpawnDustRing();
        wait(20);
        Event_SetFlag(flag + 1);
      }
    }
    MagmaRock_EndEvent();
    return;
  }
}

/// Unused: a crumbling pillar named in the (actor, flag) table gives way with a
/// shake, dust and a thump.
UNUSED void MagmaRock_CrumblePillar(struct MagmaRockCrumblingPillar *entry,
                                    s32 id) {
  struct Actor *target;
  s32 flag;
  s32 nextFlag;

  target = Actor_Get(id);
  while (entry->id != ACTOR_LIST_END) {
    if (entry->id != id) {
      entry++;
      continue;
    }
    flag = entry->flag;
    Event_Start();
    nextFlag = flag + 1;
    if (Event_GetFlag(nextFlag) == 0) {
      Event_PlaySound(SE_PUSH);
      MapGrid_SetShake(1, 4, 1);
      Dust_Scatter(0);
      System_Wait(8);
      Actor_SelectAnimation(target, 2);
      Actor_SetCellShape(-4, target->layer, id);
      Dust_Scatter(1);
      System_Wait(16);
      MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
      System_Wait(8);
      FieldActor_SetPositionAndResetMotion(target, 0, 0, 0);
      System_Wait(2);
      Event_PlaySound(SE_THUMP);
      MagmaRock_SpawnDustRing();
      System_Wait(20);
      Event_SetFlag(flag);
      Event_SetFlag(nextFlag);
    }
    MagmaRock_EndEvent();
    return;
  }
}

#include "shared/pillar_take_out_of_play.c"

void Actor_PlaceOnActor(s32 id, s32 owner);

#include "shared/pillar_restore.h"

#include "shared/actor_take_place_of.h"

/// Unused: stepping onto a crumbling floor: the first time it shakes and
/// cracks, the second time it gives way with the leader.
UNUSED void MagmaRock_StepOnTrap(u16 *list) {
  struct Actor *leader;
  struct Actor *firstPlaced;
  struct Actor *target;
  FIXED position[3];

  if (Field_TryJumpForward() != 0)
    return;
  leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  firstPlaced = Actor_GetById(ACTOR_FIRST_PLACED);
  position[0] = leader->x.fixed;
  position[1] = leader->y.fixed;
  position[2] = leader->z.fixed;
  target = Actor_FindAtCell(position, 0);
  if (target == NULL)
    return;
  while (*list != ACTOR_LIST_END) {
    u32 id = list[0];

    if (Actor_GetById(id) != target) {
      list += 2;
      continue;
    }
    {
      s32 flag = list[1];

      Event_Start();
      if (!Event_GetFlag(flag)) {
        Actor_SelectAnimation(firstPlaced, 1);
        Actor_TakePlaceOf(firstPlaced, target);
        wait(1);
        Event_PlaySound(SE_CHEST_CLOSE);
        FieldActor_SetSpriteColor(Actor_GetById(ACTOR_FIRST_PLACED),
                                  SPRITE_COLOR_WHITE);
        wait(2);
        Actor_SelectAnimation(firstPlaced, 0);
        Actor_CopyGraphics(id, ACTOR_FIRST_PLACED + 1);
        FieldActor_SetSpriteColor(Actor_GetById(ACTOR_FIRST_PLACED),
                                  SPRITE_COLOR_NORMAL);
        wait(2);
        FieldActor_SetSpriteColor(Actor_GetById(ACTOR_FIRST_PLACED),
                                  SPRITE_COLOR_WHITE);
        wait(4);
        FieldActor_SetSpriteColor(Actor_GetById(ACTOR_FIRST_PLACED),
                                  SPRITE_COLOR_NORMAL);
        Dust_Scatter(0);
        Actor_TakePlaceOf(target, firstPlaced);
        wait(1);
        Event_SetFlag(flag);
      } else if (!Event_GetFlag(flag + 1)) {
        u8 *flags;

        Actor_SelectAnimation(firstPlaced, 0);
        Actor_TakePlaceOf(firstPlaced, target);
        wait(1);
        flags = &leader->motionFlags;
        *flags = 0;
        Event_PlaySound(SE_PUSH);
        MapGrid_SetShake(1, 4, 1);
        Dust_Scatter(0);
        wait(8);
        Actor_SelectAnimation(firstPlaced, 2);
        Leader_SetCellShapeAt(-2, target->layer);
        Dust_Scatter(1);
        wait(16);
        Leader_SetCellShapeAt(-2, target->layer);
        wait(4);
        MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
        wait(8);
        *flags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
        wait(5);
        Actor_SetPositionAndResetMotion(firstPlaced, 0, 0, 0);
        wait(2);
        Event_PlaySound(SE_THUMP);
        MagmaRock_SpawnDustRing();
        wait(20);
        Event_SetFlag(flag + 1);
      }
      MagmaRock_EndEvent();
      return;
    }
  }
}

#include "../field/field_effect_advance_motion_priority.c"

void Actor_StartScript(struct Actor *actor, const s32 *script);

#include "shared/dust_scatter.c"

/// Throws a ring of dust clouds out from Felix as he lands with a crumbled
/// pillar.
void MagmaRock_SpawnDustRing(void) {
  struct Actor *leader;
  struct FieldEffect *effect;
  s32 state;
  u32 i;
  s32 angle;

  leader = Actor_Get(gMainRoutineWork.leaderActorId);
  if (leader == NULL)
    return;
  state = 0;
  for (i = 0; i <= DUST_RING_PUFFS; i++) {
    effect = (struct FieldEffect *)FieldActor_Create(
        SPRITE_DUST_CLOUD, leader->x.fixed, leader->y.fixed, leader->z.fixed);
    if (effect != NULL) {
      state = Sprite_ShareVramCacheEntry(effect->sprite, state);
      effect->motionFlags = 0;
      effect->collisionFlags = 0;
      Actor_SetSpriteFlags((struct Actor *)effect, 0);
      Actor_SelectAnimation(effect, 2);
      Actor_StartScript(effect, gDust_BurstScript);
      angle = i * (TRIG_TURN / DUST_RING_PUFFS);
      effect->velocity.x = Trig_Cos(angle);
      effect->velocity.y = 0;
      effect->velocity.z = Math_Sin(angle);
      effect->velocity.x += effect->velocity.x >> 1;
      effect->velocity.x += (Random_Below(toFIXED(3) / 4)) - toFIXED(3) / 8;
      effect->velocity.z += (Random_Below(toFIXED(3) / 8)) - toFIXED(3) / 16;
      effect->spin = (Random_Below(TRIG_TURN / 16)) - TRIG_TURN / 32;
      effect->scaleRateX = 0;
      effect->scaleRateY = 0;
      effect->update = Effect_AdvanceMotionAtLeaderPriority;
      effect->sprite->priority =
          ((struct FieldSprite *)leader->sprite)->priority;
    }
  }
}

s32 MapGrid_GetTerrainKind(s32 terrainId, s32 x, s32 z);
s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);

#include "shared/actor_set_cell_shape.h"

#include "shared/leader_set_cell_shape.h"

void BattleEffect_StepVector3(struct Particle *particle, s32 damping,
                              s32 gravity);

#include "shared/particle_update_falling.h"

/// Drops count rocks from (x, y, z), each flung out at a random angle.
void MagmaRock_DropRocks(s32 x, s32 y, s32 z, s32 count) {
  struct RockWork *work;
  s32 i;

  work = Ram_HeapSlots->menuWork;
  if (count > 0) {
    i = count;
    do {
      struct Rock *rock;
      s32 angle = Random16();

      rock = &work->rocks[work->next];
      rock->timer = 0;
      rock->position[0] = x;
      rock->position[1] = y;
      rock->position[2] = z;
      FieldVector_AddPolarOffsetFX(Random16() * 8 + toFIXED(8), angle,
                                   rock->position);
      rock->velocity[0] = 0;
      rock->velocity[1] = toFIXED(5);
      rock->velocity[2] = 0;
      FieldVector_AddPolarOffsetFX(Random16() * 2 + toFIXED(2), angle,
                                   rock->velocity);
      {
        s32 mask = ROCK_COUNT - 1;

        work->next = (work->next + 1) & mask;
      }
      i--;
    } while (i != 0);
  }
}

/// The overlay's import veneer to the tagged LZ decompressor.
void Import_DecompressTaggedLz(void *entry, void *destination);

/// Starts the falling rocks: loads their picture, prepares a small sprite for
/// each rock and adds the task that draws them.
void MagmaRock_StartFallingRocks(void) {
  struct RockWork *work =
      HeapSlot_Alloc(HEAP_SLOT_MENU, sizeof(struct RockWork));
  s32 entry;
  s32 tile;
  s32 i;
  struct Rock *rock;
  struct EffectSprite *sprite;

  Import_DecompressTaggedLz(Resource_GetTableEntry(RESOURCE_FALLING_ROCK),
                            work);
  entry = VramCache_FindFreeEntry();
  tile = Vram_Load(entry, sizeof(work->tiles), work);
  work->tileBase = tile;
  work->resource = entry;
  rock = work->rocks;
  for (i = 0; i < ROCK_COUNT; i++) {
    sprite = &work->sprites[i];
    EffectSprite_Init(sprite, 8, 8, OBJ_SIZE_16X16, tile);
    sprite->oam.fullColor = 1;
    sprite->oam.palette = SPRITE_COLOR_NORMAL;
    sprite->oam.priority = 0;
    sprite->depth = OAM_DEPTH_WINDOW;
    rock->timer = -1;
    rock++;
  }
  work->next = 0;
  Task_AddFrame(Particle_UpdateFalling, TASK_PASS_FRAME);
}

#include "shared/rock_end_falling.h"

// clang-format off
#include "shared/field_effect_duration_scripts.c"

#include "shared/push_leader_steps.c"

#include "shared/magma_rock_ember_scripts.c"

/// The pillars Move pushes on the slide, ended by -1.
s16 gMagmaRock_SlidePillars[] = {
    ACTOR_SLIDE_PILLAR_A, ACTOR_SLIDE_PILLAR_B, -1,
};

/// The slide's statues (Statue_FindUnderBurst): each row a statue, the cell
/// Burst finds it by and the cap its flame column raises (-1 for none), ended
/// by -1.
s16 gMagmaRock_SlideStatues[] = {
    ACTOR_SLIDE_COLUMN_STATUE, CELL_BEHAVIOR_MAGMA_ROCK_COLUMN_STATUE_A, ACTOR_SLIDE_STATUE_CAP,
    ACTOR_SLIDE_FIRING_STATUE, CELL_BEHAVIOR_MAGMA_ROCK_FIRING_STATUE, -1,
    -1,
};

/// The pillars Move pushes on the cliff's high west level, ended by -1.
s16 gMagmaRock_SalamanderTailHighWestPillars[] = {
    ACTOR_HIGH_WEST_PILLAR_A, ACTOR_HIGH_WEST_PILLAR_B, -1,
};

/// The high west level's statues, as gMagmaRock_SlideStatues.
s16 gMagmaRock_SalamanderTailHighWestStatues[] = {
    ACTOR_HIGH_WEST_FIRING_STATUE, CELL_BEHAVIOR_MAGMA_ROCK_FIRING_STATUE, -1,
    -1,
};

/// The high west level's crumbling pillars: (actor, flag) rows ended by -1, the
/// flag set once the pillar has cracked and the next once it has fallen.
s16 gMagmaRock_CrumblingPillars[] = {
    ACTOR_CRUMBLING_PILLAR_A, FLAG_CRUMBLING_PILLAR_A_CRACKED,
    ACTOR_CRUMBLING_PILLAR_B, FLAG_CRUMBLING_PILLAR_B_CRACKED,
    -1,
};

/// The high east level's statues, as gMagmaRock_SlideStatues.
s16 gMagmaRock_SalamanderTailHighEastStatues[] = {
    ACTOR_HIGH_EAST_COLUMN_STATUE_A, CELL_BEHAVIOR_MAGMA_ROCK_COLUMN_STATUE_A, ACTOR_HIGH_EAST_STATUE_CAP_A,
    ACTOR_HIGH_EAST_COLUMN_STATUE_B, CELL_BEHAVIOR_MAGMA_ROCK_COLUMN_STATUE_B, ACTOR_HIGH_EAST_STATUE_CAP_B,
    -1,
};

/// The pillars Move pushes on the high east level, ended by -1.
s16 gMagmaRock_SalamanderTailHighEastPillars[] = {
    ACTOR_HIGH_EAST_PILLAR, -1,
};

/// The low west level's statues, as gMagmaRock_SlideStatues.
s16 gMagmaRock_SalamanderTailLowWestStatues[] = {
    ACTOR_LOW_WEST_COLUMN_STATUE_A, CELL_BEHAVIOR_MAGMA_ROCK_COLUMN_STATUE_A, ACTOR_LOW_WEST_STATUE_CAP_A,
    ACTOR_LOW_WEST_COLUMN_STATUE_B, CELL_BEHAVIOR_MAGMA_ROCK_COLUMN_STATUE_B, ACTOR_LOW_WEST_STATUE_CAP_B,
    -1,
};

/// The pillars Move pushes on the low west level, ended by -1.
s16 gMagmaRock_SalamanderTailLowWestPillars[] = {
    ACTOR_LOW_WEST_PILLAR, -1,
};

/// The pillars Move pushes on the low east level, ended by -1.
s16 gMagmaRock_SalamanderTailLowEastPillars[] = {
    ACTOR_LOW_EAST_PILLAR_A, ACTOR_LOW_EAST_PILLAR_B, -1,
};

/// The low east level's statues, as gMagmaRock_SlideStatues.
s16 gMagmaRock_SalamanderTailLowEastStatues[] = {
    ACTOR_LOW_EAST_FIRING_STATUE_A, CELL_BEHAVIOR_MAGMA_ROCK_FIRING_STATUE, -1,
    ACTOR_LOW_EAST_COLUMN_STATUE, CELL_BEHAVIOR_MAGMA_ROCK_COLUMN_STATUE_A, ACTOR_LOW_EAST_STATUE_CAP,
    ACTOR_LOW_EAST_FIRING_STATUE_B, CELL_BEHAVIOR_MAGMA_ROCK_FIRING_STATUE_B, -1,
    -1,
};

/// The statue on the top, as gMagmaRock_SlideStatues.
s16 gMagmaRock_StatueStatues[] = {
    ACTOR_STATUE_ROOM_STATUE, CELL_BEHAVIOR_MAGMA_ROCK_STATUE_ROOM_STATUE, -1,
    -1, 0,
};

#include "shared/dust_scatter_script.c"

#include "shared/dust_burst_script.c"

const s32 *gFieldEffect_DurationScripts[] = {
    gFieldEffect_ShortScript,
    gFieldEffect_MediumScript,
    gFieldEffect_LongScript,
};

#include "shared/scene_entrances_default.c"

/// The exit regions of the slide.
struct SceneRegion gMagmaRock_Regions[] = {
    {240, 328, 352, 256, 344, 368, CONDITION_ALWAYS, MAGMA_ROCK_SLIDE_TO_MAGMA_ROCK_SALAMANDER_TAIL_CHEST_WEST},
    {448, 328, 352, 464, 344, 368, CONDITION_ALWAYS, MAGMA_ROCK_SLIDE_TO_MAGMA_ROCK_SALAMANDER_TAIL_CHEST_EAST},
    {SCENE_TABLE_END},
};

/// The exit regions of the cliff.
struct SceneRegion gMagmaRock_SalamanderTailRegions[] = {
    {272, 56, 576, 288, 72, 592, CONDITION_ALWAYS, EXIT_HIGH_WEST_DOWN_A},
    {352, 40, 560, 368, 56, 576, CONDITION_ALWAYS, EXIT_HIGH_WEST_DOWN_B},
    {752, 512, 536, 768, 528, 552, CONDITION_ALWAYS, MAGMA_ROCK_SALAMANDER_TAIL_CHEST_TO_MAGMA_ROCK_STATUE_WEST},
    {784, 80, 592, 800, 96, 608, CONDITION_ALWAYS, EXIT_HIGH_EAST_DOWN},
    {896, 512, 528, 912, 528, 544, CONDITION_ALWAYS, MAGMA_ROCK_SALAMANDER_TAIL_CHEST_TO_MAGMA_ROCK_STATUE_EAST_B},
    {272, 48, 576, 288, 64, 592, CONDITION_ALWAYS, EXIT_LOW_WEST_UP_A},
    {352, 32, 560, 368, 48, 576, CONDITION_ALWAYS, EXIT_LOW_WEST_UP_B},
    {784, 56, 592, 800, 72, 608, CONDITION_ALWAYS, EXIT_LOW_EAST_UP},
    {752, -344, 672, 768, -328, 688, CONDITION_ALWAYS, MAGMA_ROCK_SALAMANDER_TAIL_CHEST_TO_MAGMA_ROCK_SLIDE_WEST},
    {976, -344, 672, 992, -328, 688, CONDITION_ALWAYS, MAGMA_ROCK_SALAMANDER_TAIL_CHEST_TO_MAGMA_ROCK_SLIDE_EAST},
    {SCENE_TABLE_END},
};

/// The exit regions of the top.
struct SceneRegion gMagmaRock_StatueRegions[] = {
    {240, -56, 448, 256, -40, 464, CONDITION_ALWAYS, MAGMA_ROCK_STATUE_TO_MAGMA_ROCK_SALAMANDER_TAIL_CHEST_WEST},
    {384, -56, 448, 400, -40, 464, CONDITION_ALWAYS, MAGMA_ROCK_STATUE_TO_MAGMA_ROCK_SALAMANDER_TAIL_CHEST_EAST_B},
    {416, -128, 448, 432, -112, 464, CONDITION_ALWAYS, MAGMA_ROCK_STATUE_TO_MAGMA_ROCK_SALAMANDER_TAIL_CHEST_EAST_A},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(MagmaRockSlide),
    SCENE_EXIT(MAGMA_ROCK_SLIDE_TO_WORLD_MAP, WorldMap, WORLD_MAP_FROM_MAGMA_ROCK_SLIDE),
    SCENE_EXIT(MAGMA_ROCK_SLIDE_TO_MAGMA_ROCK_INSIDE_EMBER_STONE, MagmaRockInsideEmberStone, MAGMA_ROCK_INSIDE_EMBER_STONE_FROM_MAGMA_ROCK_SLIDE),
    SCENE_EXIT(MAGMA_ROCK_SLIDE_TO_MAGMA_ROCK_SALAMANDER_TAIL_CHEST_WEST, MagmaRockSalamanderTailChest, MAGMA_ROCK_SALAMANDER_TAIL_CHEST_FROM_MAGMA_ROCK_SLIDE_WEST),
    SCENE_EXIT(MAGMA_ROCK_SLIDE_TO_MAGMA_ROCK_SALAMANDER_TAIL_CHEST_EAST, MagmaRockSalamanderTailChest, MAGMA_ROCK_SALAMANDER_TAIL_CHEST_FROM_MAGMA_ROCK_SLIDE_EAST),
    SCENE_EXITS(MagmaRockSalamanderTailChest),
    SCENE_EXIT(EXIT_HIGH_WEST_DOWN_A, MagmaRockSalamanderTailChest, ENTRANCE_LOW_WEST_FROM_ABOVE_A),
    SCENE_EXIT(EXIT_HIGH_WEST_DOWN_B, MagmaRockSalamanderTailChest, ENTRANCE_LOW_WEST_FROM_ABOVE_B),
    SCENE_EXIT(EXIT_HIGH_WEST_TO_HIGH_EAST, MagmaRockSalamanderTailChest, ENTRANCE_HIGH_EAST_FROM_HIGH_WEST),
    SCENE_EXIT(EXIT_HIGH_EAST_TO_HIGH_WEST, MagmaRockSalamanderTailChest, ENTRANCE_HIGH_WEST_FROM_HIGH_EAST),
    SCENE_EXIT(MAGMA_ROCK_SALAMANDER_TAIL_CHEST_TO_MAGMA_ROCK_STATUE_WEST, MagmaRockStatue, MAGMA_ROCK_STATUE_FROM_MAGMA_ROCK_SALAMANDER_TAIL_CHEST_WEST),
    SCENE_EXIT(MAGMA_ROCK_SALAMANDER_TAIL_CHEST_TO_MAGMA_ROCK_STATUE_EAST_A, MagmaRockStatue, MAGMA_ROCK_STATUE_FROM_MAGMA_ROCK_SALAMANDER_TAIL_CHEST_EAST_B),
    SCENE_EXIT(EXIT_HIGH_EAST_DOWN, MagmaRockSalamanderTailChest, ENTRANCE_LOW_EAST_FROM_ABOVE),
    SCENE_EXIT(MAGMA_ROCK_SALAMANDER_TAIL_CHEST_TO_MAGMA_ROCK_STATUE_EAST_B, MagmaRockStatue, MAGMA_ROCK_STATUE_FROM_MAGMA_ROCK_SALAMANDER_TAIL_CHEST_EAST_B),
    SCENE_EXIT(EXIT_LOW_WEST_UP_A, MagmaRockSalamanderTailChest, ENTRANCE_HIGH_WEST_FROM_BELOW_A),
    SCENE_EXIT(EXIT_LOW_WEST_UP_B, MagmaRockSalamanderTailChest, ENTRANCE_HIGH_WEST_FROM_BELOW_B),
    SCENE_EXIT(EXIT_LOW_WEST_TO_LOW_EAST_A, MagmaRockSalamanderTailChest, ENTRANCE_LOW_EAST_FROM_LOW_WEST_A),
    SCENE_EXIT(EXIT_LOW_WEST_TO_LOW_EAST_B, MagmaRockSalamanderTailChest, ENTRANCE_LOW_EAST_FROM_LOW_WEST_B),
    SCENE_EXIT(EXIT_LOW_EAST_TO_LOW_WEST_A, MagmaRockSalamanderTailChest, ENTRANCE_LOW_WEST_FROM_LOW_EAST_A),
    SCENE_EXIT(EXIT_LOW_EAST_UP, MagmaRockSalamanderTailChest, ENTRANCE_HIGH_EAST_FROM_BELOW),
    SCENE_EXIT(EXIT_LOW_EAST_TO_LOW_WEST_B, MagmaRockSalamanderTailChest, ENTRANCE_LOW_WEST_FROM_LOW_EAST_B),
    SCENE_EXIT(MAGMA_ROCK_SALAMANDER_TAIL_CHEST_TO_MAGMA_ROCK_SLIDE_WEST, MagmaRockSlide, MAGMA_ROCK_SLIDE_FROM_MAGMA_ROCK_SALAMANDER_TAIL_CHEST_WEST),
    SCENE_EXIT(MAGMA_ROCK_SALAMANDER_TAIL_CHEST_TO_MAGMA_ROCK_SLIDE_EAST, MagmaRockSlide, MAGMA_ROCK_SLIDE_FROM_MAGMA_ROCK_SALAMANDER_TAIL_CHEST_EAST),
    SCENE_EXITS(MagmaRockStatue),
    SCENE_EXIT(MAGMA_ROCK_STATUE_TO_MAGMA_ROCK_SALAMANDER_TAIL_CHEST_WEST, MagmaRockSalamanderTailChest, MAGMA_ROCK_SALAMANDER_TAIL_CHEST_FROM_MAGMA_ROCK_STATUE_WEST),
    SCENE_EXIT(MAGMA_ROCK_STATUE_TO_MAGMA_ROCK_SALAMANDER_TAIL_CHEST_EAST_B, MagmaRockSalamanderTailChest, MAGMA_ROCK_SALAMANDER_TAIL_CHEST_FROM_MAGMA_ROCK_STATUE_EAST_B),
    SCENE_EXIT(MAGMA_ROCK_STATUE_TO_MAGMA_ROCK_INSIDE_ENTRANCE, MagmaRockInsideEntrance, MAGMA_ROCK_INSIDE_ENTRANCE_FROM_MAGMA_ROCK_STATUE),
    SCENE_EXIT(MAGMA_ROCK_STATUE_TO_MAGMA_ROCK_SALAMANDER_TAIL_CHEST_EAST_A, MagmaRockSalamanderTailChest, MAGMA_ROCK_SALAMANDER_TAIL_CHEST_FROM_MAGMA_ROCK_STATUE_EAST_A),
    SCENE_EXITS_END,
};

/// The actors placed in the slide.
struct ActorPlacement gMagmaRock_Placements[] = {
    {SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 1, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_WIND_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 2, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 3, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_BURST_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 4, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_WIND_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 5, 0, 0, FACING_WEST, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_WIND_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 6, 0, 0, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_WIND_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 7, 0, 0, FACING_WEST, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_BURST_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_BURST_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 9, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_BURST_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 10, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_BURST_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 11, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_ROUND_BOULDER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 12, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_STATUE_CAP, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 2, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    ACTOR_PLACEMENT(SPRITE_FLAME_PUFF, CONDITION_ALWAYS, 7, 8, 0, 8, FACING_WEST, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

/// The actors placed in the cliff's high west level.
struct ActorPlacement gMagmaRock_SalamanderTailHighWestPlacements[] = {
    {SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 1, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_BURST_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 2, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_BURST_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 3, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 4, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 5, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT | ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE},
    {SPRITE_WIND_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 6, 0, 0, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_BURST_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 7, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SCENE_TABLE_END},
};

/// The actors placed in the cliff's high east level.
struct ActorPlacement gMagmaRock_SalamanderTailHighEastPlacements[] = {
    {SPRITE_WIND_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 9, 0, 0, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_WIND_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 10, 0, 0, FACING_WEST, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_WIND_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 9, 0, 0, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_WIND_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 10, 0, 0, FACING_WEST, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_WIND_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 9, 0, 0, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_WIND_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 10, 0, 0, FACING_WEST, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_WIND_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 11, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_STATUE_CAP, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 11, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 12, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_WIND_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 13, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_STATUE_CAP, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 13, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    ACTOR_PLACEMENT(SPRITE_FLAME_PUFF, CONDITION_ALWAYS, 7, 8, 0, 8, FACING_WEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FLAME_PUFF, CONDITION_ALWAYS, 7, 8, 0, 8, FACING_WEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FLAME_PUFF, CONDITION_ALWAYS, 7, 8, 0, 8, FACING_WEST, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

/// The actors placed in the cliff's low west level.
struct ActorPlacement gMagmaRock_SalamanderTailLowWestPlacements[] = {
    {SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 14, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_WIND_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 15, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_STATUE_CAP, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 15, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_WIND_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 16, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_STATUE_CAP, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 16, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 17, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 18, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT | ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE},
    {SCENE_TABLE_END},
};

/// The actors placed in the cliff's low east level.
struct ActorPlacement gMagmaRock_SalamanderTailLowEastPlacements[] = {
    {SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 19, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 20, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_BURST_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 21, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_WIND_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 22, 0, 0, FACING_WEST, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_WIND_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 23, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_STATUE_CAP, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 23, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_BURST_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 24, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_WIND_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 25, 0, 0, FACING_WEST, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_SPROUT, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 26, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_SPROUT, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 27, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_SPROUT, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 28, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SCENE_TABLE_END},
};

/// The actors placed on the top.
struct ActorPlacement gMagmaRock_StatuePlacements[] = {
    {SPRITE_GREAT_FIRE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 1, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_WIND_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 2, 0, 0, FACING_WEST, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SCENE_TABLE_END},
};
// clang-format on

/// The triggers of the slide.
struct Trigger gMagmaRock_Triggers[] = {
    {TRIGGER_EXIT, MAGMA_ROCK_SLIDE_TO_WORLD_MAP, CONDITION_ALWAYS,
     MAGMA_ROCK_SLIDE_TO_WORLD_MAP},
    {STEP_TRIGGER_WALKING | STEP_TRIGGER_FLAG_QUICK | FACING_NORTH,
     CELL_BEHAVIOR_MAGMA_ROCK_CAVE_DOOR, FLAG_CAVE_DOOR_USED,
     EVENT(MagmaRock_EnterCave)},
    {TRIGGER_STEP, CELL_BEHAVIOR_MAGMA_ROCK_CAVE_DOOR, CONDITION_ALWAYS,
     EVENT(MagmaRock_ResetCaveDoor)},
    {PSYNERGY_ON_ACTOR(ABILITY_LIFT), ACTOR_SLIDE_BOULDER, CONDITION_ALWAYS, 0},
    {TRIGGER_PUSH, 0, CONDITION_ALWAYS, EVENT(MagmaRock_StartPushingPillar)},
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS, EVENT(MagmaRock_FinishPushingPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST),
     ACTOR_SLIDE_PILLAR_A, FLAG_SLIDE_PILLAR_A_PUSHED,
     EVENT(MagmaRock_StartPushingPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_SLIDE_PILLAR_A,
     FLAG_SLIDE_PILLAR_A_PUSHED, EVENT(MagmaRock_FinishPushingPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST),
     ACTOR_SLIDE_PILLAR_B, FLAG_SLIDE_PILLAR_B_PUSHED,
     EVENT(MagmaRock_StartPushingPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_SLIDE_PILLAR_B,
     FLAG_SLIDE_PILLAR_B_PUSHED, EVENT(MagmaRock_FinishPushingPillar)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS),
     CELL_BEHAVIOR_MAGMA_ROCK_CRACKED_PILLAR_A, FLAG_CRACKED_PILLAR_A_BURST,
     EVENT(MagmaRock_BurstCrackedPillarA)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS),
     CELL_BEHAVIOR_MAGMA_ROCK_CRACKED_PILLAR_B, FLAG_CRACKED_PILLAR_B_BURST,
     EVENT(MagmaRock_BurstCrackedPillarB)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS),
     CELL_BEHAVIOR_MAGMA_ROCK_CRACKED_PILLAR_C, FLAG_CRACKED_PILLAR_C_BURST,
     EVENT(MagmaRock_BurstCrackedPillarC)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS),
     CELL_BEHAVIOR_MAGMA_ROCK_CRACKED_PILLAR_D, FLAG_CRACKED_PILLAR_D_BURST,
     EVENT(MagmaRock_BurstCrackedPillarD)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS),
     CELL_BEHAVIOR_MAGMA_ROCK_CRACKED_PILLAR_E, FLAG_CRACKED_PILLAR_E_BURST,
     EVENT(MagmaRock_BurstCrackedPillarE)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_FIRST),
     CELL_BEHAVIOR_MAGMA_ROCK_FIRING_STATUE, CONDITION_ALWAYS,
     EVENT(MagmaRock_ResolveBeforeBurst)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS),
     CELL_BEHAVIOR_MAGMA_ROCK_FIRING_STATUE, FLAG_FIRE_BURST_PLAYING,
     EVENT(MagmaRock_TargetStatue)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS),
     CELL_BEHAVIOR_MAGMA_ROCK_FIRING_STATUE, CONDITION_ALWAYS,
     EVENT(MagmaRock_BurstStatue)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_AFTER_CAST),
     CELL_BEHAVIOR_MAGMA_ROCK_FIRING_STATUE, CONDITION_ALWAYS,
     EVENT(MagmaRock_FireStatue)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_FIRST),
     CELL_BEHAVIOR_MAGMA_ROCK_COLUMN_STATUE_A, CONDITION_ALWAYS,
     EVENT(MagmaRock_ResolveBeforeBurst)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS),
     CELL_BEHAVIOR_MAGMA_ROCK_COLUMN_STATUE_A, CONDITION_ALWAYS,
     EVENT(MagmaRock_TargetStatue)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_AFTER_CAST),
     CELL_BEHAVIOR_MAGMA_ROCK_COLUMN_STATUE_A, CONDITION_ALWAYS,
     EVENT(MagmaRock_IgniteStatue)},
    {TRIGGER_TABLE_END},
};

/// The triggers of the cliff's high west level.
struct Trigger gMagmaRock_SalamanderTailHighWestTriggers[] = {
    {TRIGGER_EXIT, EXIT_HIGH_WEST_TO_HIGH_EAST, CONDITION_ALWAYS,
     EXIT_HIGH_WEST_TO_HIGH_EAST},
    {STEP_TRIGGER_WALKING, CELL_BEHAVIOR_MAGMA_ROCK_CRUMBLING_PILLAR,
     CONDITION_ALWAYS, EVENT(MagmaRock_CheckCrumblingPillars)},
    {TRIGGER_PUSH, 0, CONDITION_ALWAYS, EVENT(MagmaRock_StartPushingPillar)},
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS, EVENT(MagmaRock_FinishPushingPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST),
     ACTOR_HIGH_WEST_PILLAR_A, CONDITION_ALWAYS,
     EVENT(MagmaRock_StartPushingPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_HIGH_WEST_PILLAR_A,
     CONDITION_ALWAYS, EVENT(MagmaRock_FinishPushingPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST),
     ACTOR_HIGH_WEST_PILLAR_B, FLAG_HIGH_WEST_PILLAR_B_PUSHED,
     EVENT(MagmaRock_StartPushingPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_HIGH_WEST_PILLAR_B,
     FLAG_HIGH_WEST_PILLAR_B_PUSHED, EVENT(MagmaRock_FinishPushingPillar)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS),
     CELL_BEHAVIOR_MAGMA_ROCK_CRACKED_PILLAR_A, FLAG_CRACKED_PILLAR_A_BURST,
     EVENT(MagmaRock_BurstCrackedPillarA)},
    {TRIGGER_STEP, CELL_BEHAVIOR_MAGMA_ROCK_BESIDE_HIGH_WEST_STATUE,
     FLAG_HIGH_WEST_STATUE_READY, EVENT(MagmaRock_ReadyHighWestStatue)},
    {TRIGGER_STEP, CELL_BEHAVIOR_MAGMA_ROCK_PAST_HIGH_WEST_STATUE,
     CONDITION_ALWAYS, EVENT(MagmaRock_ResetHighWestStatue)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_FIRST),
     CELL_BEHAVIOR_MAGMA_ROCK_FIRING_STATUE, CONDITION_ALWAYS,
     EVENT(MagmaRock_ResolveBeforeBurst)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS),
     CELL_BEHAVIOR_MAGMA_ROCK_FIRING_STATUE, FLAG_FIRE_BURST_PLAYING,
     EVENT(MagmaRock_TargetStatue)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS),
     CELL_BEHAVIOR_MAGMA_ROCK_FIRING_STATUE, CONDITION_ALWAYS,
     EVENT(MagmaRock_BurstStatue)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_AFTER_CAST),
     CELL_BEHAVIOR_MAGMA_ROCK_FIRING_STATUE, CONDITION_ALWAYS,
     EVENT(MagmaRock_FireStatue)},
    {TRIGGER_TABLE_END},
};

/// The triggers of the cliff's high east level.
struct Trigger gMagmaRock_SalamanderTailHighEastTriggers[] = {
    {TRIGGER_EXIT, EXIT_HIGH_EAST_TO_HIGH_WEST, CONDITION_ALWAYS,
     EXIT_HIGH_EAST_TO_HIGH_WEST},
    {TRIGGER_PUSH, 0, CONDITION_ALWAYS, EVENT(MagmaRock_StartPushingPillar)},
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS, EVENT(MagmaRock_FinishPushingPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST),
     ACTOR_HIGH_EAST_PILLAR, FLAG_HIGH_EAST_PILLAR_PUSHED,
     EVENT(MagmaRock_StartPushingPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_HIGH_EAST_PILLAR,
     FLAG_HIGH_EAST_PILLAR_PUSHED, EVENT(MagmaRock_FinishPushingPillar)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_FIRST),
     CELL_BEHAVIOR_MAGMA_ROCK_COLUMN_STATUE_A, CONDITION_ALWAYS,
     EVENT(MagmaRock_ResolveBeforeBurst)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS),
     CELL_BEHAVIOR_MAGMA_ROCK_COLUMN_STATUE_A, CONDITION_ALWAYS,
     EVENT(MagmaRock_TargetStatue)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_AFTER_CAST),
     CELL_BEHAVIOR_MAGMA_ROCK_COLUMN_STATUE_A, CONDITION_ALWAYS,
     EVENT(MagmaRock_IgniteStatue)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_FIRST),
     CELL_BEHAVIOR_MAGMA_ROCK_COLUMN_STATUE_B, CONDITION_ALWAYS,
     EVENT(MagmaRock_ResolveBeforeBurst)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS),
     CELL_BEHAVIOR_MAGMA_ROCK_COLUMN_STATUE_B, CONDITION_ALWAYS,
     EVENT(MagmaRock_TargetStatue)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_AFTER_CAST),
     CELL_BEHAVIOR_MAGMA_ROCK_COLUMN_STATUE_B, CONDITION_ALWAYS,
     EVENT(MagmaRock_IgniteStatue)},
    {TRIGGER_RAISED, TRIGGER_FIREBALL_HITS_LEADER, CONDITION_ALWAYS,
     EVENT(MagmaRock_KnockLeaderOffWall)},
    {TRIGGER_TABLE_END},
};

/// The triggers of the cliff's low west level.
struct Trigger gMagmaRock_SalamanderTailLowWestTriggers[] = {
    {TRIGGER_EXIT, EXIT_LOW_WEST_TO_LOW_EAST_A, CONDITION_ALWAYS,
     EXIT_LOW_WEST_TO_LOW_EAST_A},
    {TRIGGER_EXIT, EXIT_LOW_WEST_TO_LOW_EAST_B, CONDITION_ALWAYS,
     EXIT_LOW_WEST_TO_LOW_EAST_B},
    // Flag 8 comes after the party members' flags and no code sets it, so the
    // push always answers, as the low east level's does.
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST),
     CELL_BEHAVIOR_MAGMA_ROCK_PILLAR_PUSH, 0x8,
     EVENT(MagmaRock_PushPillarEast)},
    {TRIGGER_PUSH, 0, CONDITION_ALWAYS, EVENT(MagmaRock_StartPushingPillar)},
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS, EVENT(MagmaRock_FinishPushingPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST),
     ACTOR_LOW_WEST_PILLAR, FLAG_LOW_WEST_PILLAR_PUSHED,
     EVENT(MagmaRock_StartPushingPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_LOW_WEST_PILLAR,
     FLAG_LOW_WEST_PILLAR_PUSHED, EVENT(MagmaRock_FinishPushingPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_LASH), ACTOR_LOW_WEST_ROPE_EAST_POST,
     FLAG_LOW_WEST_ROPE_LASHED, 0},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_FIRST),
     CELL_BEHAVIOR_MAGMA_ROCK_COLUMN_STATUE_A, CONDITION_ALWAYS,
     EVENT(MagmaRock_ResolveBeforeBurst)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS),
     CELL_BEHAVIOR_MAGMA_ROCK_COLUMN_STATUE_A, CONDITION_ALWAYS,
     EVENT(MagmaRock_TargetStatue)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_AFTER_CAST),
     CELL_BEHAVIOR_MAGMA_ROCK_COLUMN_STATUE_A, CONDITION_ALWAYS,
     EVENT(MagmaRock_IgniteStatue)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_FIRST),
     CELL_BEHAVIOR_MAGMA_ROCK_COLUMN_STATUE_B, CONDITION_ALWAYS,
     EVENT(MagmaRock_ResolveBeforeBurst)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS),
     CELL_BEHAVIOR_MAGMA_ROCK_COLUMN_STATUE_B, CONDITION_ALWAYS,
     EVENT(MagmaRock_TargetStatue)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_AFTER_CAST),
     CELL_BEHAVIOR_MAGMA_ROCK_COLUMN_STATUE_B, CONDITION_ALWAYS,
     EVENT(MagmaRock_IgniteStatue)},
    {TRIGGER_TABLE_END},
};

/// The triggers of the cliff's low east level.
struct Trigger gMagmaRock_SalamanderTailLowEastTriggers[] = {
    {TRIGGER_EXIT, EXIT_LOW_EAST_TO_LOW_WEST_A, CONDITION_ALWAYS,
     EXIT_LOW_EAST_TO_LOW_WEST_A},
    {TRIGGER_EXIT, EXIT_LOW_EAST_TO_LOW_WEST_B, CONDITION_ALWAYS,
     EXIT_LOW_EAST_TO_LOW_WEST_B},
    {STEP_TRIGGER_WALKING_INTO(FACING_WEST),
     CELL_BEHAVIOR_MAGMA_ROCK_PILLAR_PUSH, CONDITION_ALWAYS,
     EVENT(MagmaRock_PushPillarWest)},
    {TRIGGER_PUSH, 0, CONDITION_ALWAYS, EVENT(MagmaRock_StartPushingPillar)},
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS, EVENT(MagmaRock_FinishPushingPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST),
     ACTOR_LOW_EAST_PILLAR_A, FLAG_LOW_EAST_PILLAR_A_PUSHED,
     EVENT(MagmaRock_StartPushingPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_LOW_EAST_PILLAR_A,
     FLAG_LOW_EAST_PILLAR_A_PUSHED, EVENT(MagmaRock_FinishPushingPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST),
     ACTOR_LOW_EAST_PILLAR_B, FLAG_LOW_EAST_PILLAR_B_PUSHED,
     EVENT(MagmaRock_StartPushingPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_LOW_EAST_PILLAR_B,
     FLAG_LOW_EAST_PILLAR_B_PUSHED, EVENT(MagmaRock_FinishPushingPillar)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_FIRST),
     CELL_BEHAVIOR_MAGMA_ROCK_FIRING_STATUE, CONDITION_ALWAYS,
     EVENT(MagmaRock_ResolveBeforeBurst)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS),
     CELL_BEHAVIOR_MAGMA_ROCK_FIRING_STATUE, FLAG_FIRE_BURST_PLAYING,
     EVENT(MagmaRock_TargetStatue)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS),
     CELL_BEHAVIOR_MAGMA_ROCK_FIRING_STATUE, CONDITION_ALWAYS,
     EVENT(MagmaRock_BurstStatue)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_AFTER_CAST),
     CELL_BEHAVIOR_MAGMA_ROCK_FIRING_STATUE, CONDITION_ALWAYS,
     EVENT(MagmaRock_FireStatue)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_FIRST),
     CELL_BEHAVIOR_MAGMA_ROCK_FIRING_STATUE_B, CONDITION_ALWAYS,
     EVENT(MagmaRock_ResolveBeforeBurst)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS),
     CELL_BEHAVIOR_MAGMA_ROCK_FIRING_STATUE_B, FLAG_FIRE_BURST_PLAYING,
     EVENT(MagmaRock_TargetStatue)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS),
     CELL_BEHAVIOR_MAGMA_ROCK_FIRING_STATUE_B, CONDITION_ALWAYS,
     EVENT(MagmaRock_BurstStatue)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_AFTER_CAST),
     CELL_BEHAVIOR_MAGMA_ROCK_FIRING_STATUE_B, CONDITION_ALWAYS,
     EVENT(MagmaRock_FireStatue)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS),
     CELL_BEHAVIOR_MAGMA_ROCK_CRACKED_PILLAR_B, FLAG_CRACKED_PILLAR_B_BURST,
     EVENT(MagmaRock_BurstCrackedPillarB)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_FIRST),
     CELL_BEHAVIOR_MAGMA_ROCK_COLUMN_STATUE_A, CONDITION_ALWAYS,
     EVENT(MagmaRock_ResolveBeforeBurst)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS),
     CELL_BEHAVIOR_MAGMA_ROCK_COLUMN_STATUE_A, CONDITION_ALWAYS,
     EVENT(MagmaRock_TargetStatue)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_AFTER_CAST),
     CELL_BEHAVIOR_MAGMA_ROCK_COLUMN_STATUE_A, CONDITION_ALWAYS,
     EVENT(MagmaRock_IgniteStatue)},
    {PSYNERGY_ON_ACTOR(ABILITY_GROWTH), ACTOR_LOW_EAST_SPROUT_A,
     FLAG_VINE_A_GROWN, EVENT(MagmaRock_GrowVineA)},
    {PSYNERGY_ON_ACTOR(ABILITY_GROWTH), ACTOR_LOW_EAST_SPROUT_B,
     FLAG_VINE_B_GROWN, EVENT(MagmaRock_GrowVineB)},
    {PSYNERGY_ON_ACTOR(ABILITY_GROWTH), ACTOR_LOW_EAST_SPROUT_C,
     FLAG_VINE_C_GROWN, EVENT(MagmaRock_GrowVineC)},
    {TRIGGER_TABLE_END},
};

/// The triggers of the top.
struct Trigger gMagmaRock_StatueTriggers[] = {
    {TRIGGER_EXIT, MAGMA_ROCK_STATUE_TO_MAGMA_ROCK_INSIDE_ENTRANCE,
     CONDITION_ALWAYS, MAGMA_ROCK_STATUE_TO_MAGMA_ROCK_INSIDE_ENTRANCE},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_FIRST),
     CELL_BEHAVIOR_MAGMA_ROCK_STATUE_ROOM_STATUE, CONDITION_ALWAYS,
     EVENT(MagmaRock_ResolveBeforeBurst)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS),
     CELL_BEHAVIOR_MAGMA_ROCK_STATUE_ROOM_STATUE, FLAG_FIRE_BURST_PLAYING,
     EVENT(MagmaRock_TargetStatue)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS),
     CELL_BEHAVIOR_MAGMA_ROCK_STATUE_ROOM_STATUE, CONDITION_ALWAYS,
     EVENT(MagmaRock_BlastWayIn)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_AFTER_CAST),
     CELL_BEHAVIOR_MAGMA_ROCK_STATUE_ROOM_STATUE, CONDITION_ALWAYS,
     EVENT(MagmaRock_FireStatue)},
    {TRIGGER_TABLE_END},
};
