/// @file
/// The Shrine of the Sea God east of Daila, in eleven rooms that share this
/// overlay (SceneId_ShrineOfTheSeaGodRope to
/// SceneId_ShrineOfTheSeaGodRightProng).
///
/// On the first visit Tavi is stuck on a ledge in the entrance (the rope room)
/// and Riki cannot throw him the rope; Riki goes off to the side room by the
/// plank bridge, and Lash ties the rope so Tavi climbs down. Past the
/// crossroads, where the Djinni Breath is first seen, its footprints cross the
/// room below and it is chased round a ledge (Breath's room) until the torch
/// pushed against the north wall traps it. On the second visit, with Frost on
/// the puddles, Move on the pillar over the hole, Lash, and the broken bridges
/// whose planks drop the party into the stream, the party hops across the
/// stones in the water, Reveal showing the seventh in the middle of the hexagon
/// of rocks, to the island. The Sea God's Tear used on its crystal statue
/// raises the water to the statue room, and three ladders more lead up to the
/// Right Prong.
#include "djinn_ids.h"
#include "fixed_math.h"
#include "flags.h"
#include "item_ids.h"
#include "macros.h"
#include "scene_ids.h"
#include "sounds.h"
#include "types.h"
#include "battle/battle.h"
#include "field/actor_script.h"
#include "field/call.h"
#include "field/emote.h"
#include "field/event_inline.h"
#include "field/field.h"
#include "field/field_effect.h"
#include "field/field_sequence.h"
#include "field/field_sequence_event.h"
#include "field/map_grid.h"
#include "field/scene_tables.h"
#include "field/speaker.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "menu/menu.h"
#include "scene/ground_effects.h"
#include "scene/scene.h"
#include "scene/shrine_of_the_sea_god.h"
#include "sound/sound.h"
#include "system/field_effect.h"
#include "system/frame.h"
#include "system/io_reg.h"
#include "system/io_write.h"
#include "system/main_routine_work.h"
#include "system/math_table.h"
#include "system/ram_buffer.h"
#include "system/system.h"
#include "text/msg_ids.h"

void Rope_Initialize(void);
void FieldEffect_StartBlankOverlay(s32 object);

#include "../field/actor_set_sprite_priority.c"

#include "../field/field_effect_move.c"
#define EFFECT_SET_MODE Actor_SelectAnimation
#define EFFECT_SET_SCRIPT Actor_StartScript
#include "../field/field_effect_spawn.c"

#define FLAG_CROSSROADS_PILLAR_MOVED                                           \
  0x84c                              ///< Move pushed the crossroads' pillar
#define FLAG_RIKI_GAVE_UP 0x872      ///< Riki missed with the rope, went off
#define FLAG_DJINNI_FLED_EAST 0x874  ///< the Djinni ran east in the crossroads
#define FLAG_DJINNI_FLED_SOUTH 0x875 ///< and ran on, out of it
#define FLAG_DJINNI_RAN_PAST 0x876   ///< its footprints are gone
#define FLAG_DJINNI_BY_LEDGE 0x877   ///< it waits by the ledge
#define FLAG_DJINNI_ON_LEDGE 0x87c   ///< it hopped up onto the ledge
#define FLAG_SHRINE_FIRST_PUDDLE_FROZEN FLAG_SCENE_TEMP_00
#define FLAG_SHRINE_SECOND_PUDDLE_FROZEN FLAG_SCENE_TEMP_01
#define FLAG_SHRINE_THIRD_PUDDLE_FROZEN FLAG_SCENE_TEMP_02
#define FLAG_SHRINE_ROPE_TIED FLAG_SCENE_TEMP_03
#define FLAG_SHRINE_SECOND_ROPE_TIED FLAG_SCENE_TEMP_04
#define FLAG_DJINNI_CHASE_OVER FLAG_SCENE_TEMP_05
#define FLAG_DJINNI_TRAPPED FLAG_SCENE_TEMP_06
#define FLAG_TORCH_AT_NORTH_WALL FLAG_SCENE_TEMP_07
#define FLAG_BLOCKS_SHOWN FLAG_SCENE_TEMP_08
#define FLAG_LEADER_IN_FRONT FLAG_SCENE_TEMP_0A
/// A broken bridge's middle has fallen into the stream below.
#define FLAG_CROSSROADS_BRIDGE_BROKEN FLAG_SCENE_TEMP_30
#define FLAG_FIRST_BRIDGE_BROKEN FLAG_SCENE_TEMP_31
#define FLAG_SECOND_BRIDGE_BROKEN FLAG_SCENE_TEMP_32
#define FLAG_PILLAR_IN_HOLE FLAG_AREA_TEMP_01
/// Set as the battle with Breath starts, with where it was trapped, so the
/// scene puts it back there once it is beaten.
#define FLAG_FIGHTING_BREATH FLAG_AREA_TEMP_40
#define FLAG_BREATH_TRAPPED_EAST FLAG_AREA_TEMP_41
#define FLAG_BREATH_TRAPPED_WEST FLAG_AREA_TEMP_42

void Actor_DisableById(s32 actor);
void Actor_LiftTargetBlock(const void *script);
void Actor_PlaceMovedBlock(void);
void ShrineOfTheSeaGod_SetBlocksShown(s32 show);
void ShrineOfTheSeaGod_KeepWithLeader(struct Actor *actor);
#include "shared/tables.h"

/// The rope room's actors (SceneId_ShrineOfTheSeaGodRope), numbering the rows
/// of gShrineOfTheSeaGodRopeActorPlacements.
enum ShrineOfTheSeaGodRopeRoomActorId {
  /// Two logs floating on the room's water.
  ACTOR_ROPE_ROOM_FIRST_LOG = ACTOR_FIRST_PLACED,
  ACTOR_ROPE_ROOM_SECOND_LOG,
  /// The posts of the rope Riki cannot throw up to Tavi: his own, the rope
  /// wound round it, which Lash ties, and the one on Tavi's ledge.
  ACTOR_ROPE_ROOM_EAST_POST,
  ACTOR_ROPE_ROOM_WEST_POST,
  ACTOR_RIKI,
  /// Tavi, stuck on the ledge.
  ACTOR_TAVI,
  ACTOR_ROPE_ROOM_KRADEN
};

/// The torch Move pushes, the first row of the side room
/// (SceneId_ShrineOfTheSeaGodPlankBridge) and of the ledge room
/// (SceneId_ShrineOfTheSeaGodBreath): a pedestal whose flame is its own
/// animation. The second row is an unseen actor kept where the flame burns
/// (ShrineOfTheSeaGod_KeepFlameOnTorch).
enum ShrineOfTheSeaGodTorchActorId {
  ACTOR_TORCH = ACTOR_FIRST_PLACED,
  ACTOR_TORCH_FLAME_MARKER
};

/// The rest of the side room's actors.
enum ShrineOfTheSeaGodSideRoomActorId {
  /// The plank bridge over the stream.
  ACTOR_SIDE_ROOM_BRIDGE = ACTOR_TORCH_FLAME_MARKER + 1,
  /// Riki, who has come here looking for something to help Tavi with.
  ACTOR_SIDE_ROOM_RIKI,
  /// The actor after Riki, which the room disables though no row places it.
  ACTOR_SIDE_ROOM_DISABLED
};

/// The crossroads' actors (SceneId_ShrineOfTheSeaGodCrossroads), numbering the
/// rows of gShrineOfTheSeaGodCrossroadsActorPlacements.
enum ShrineOfTheSeaGodCrossroadsActorId {
  /// The stone pillar Move pushes aside.
  ACTOR_CROSSROADS_PILLAR = ACTOR_FIRST_PLACED,
  /// The room's bridges, the first two logs side by side, drawn in front of or
  /// behind the leader by the level it stands on
  /// (ShrineOfTheSeaGod_LayerCrossroadsBridges).
  ACTOR_CROSSROADS_LOG_BRIDGE,
  ACTOR_CROSSROADS_FIRST_BRIDGE,
  ACTOR_CROSSROADS_SECOND_BRIDGE,
  ACTOR_CROSSROADS_THIRD_BRIDGE,
  /// A broken bridge: its two ends on their legs, and its middle, which drops
  /// into the water below under the leader.
  ACTOR_CROSSROADS_BROKEN_BRIDGE_ENDS,
  ACTOR_CROSSROADS_BROKEN_BRIDGE_MIDDLE,
  /// Breath, the Jupiter Djinni, first seen here.
  ACTOR_CROSSROADS_BREATH
};

/// The room below's actors (SceneId_ShrineOfTheSeaGodFootprints): Breath's
/// footprints, sixteen of them, left and right feet in turn.
enum ShrineOfTheSeaGodFootprintsActorId {
  ACTOR_FIRST_FOOTPRINT = ACTOR_FIRST_PLACED,
  ACTOR_LAST_FOOTPRINT = ACTOR_FIRST_FOOTPRINT + 15
};

/// The rest of the ledge room's actors (SceneId_ShrineOfTheSeaGodBreath).
enum ShrineOfTheSeaGodLedgeRoomActorId {
  /// The puddles Frost freezes into pillars of ice.
  ACTOR_FIRST_PUDDLE = ACTOR_TORCH_FLAME_MARKER + 1,
  ACTOR_SECOND_PUDDLE,
  ACTOR_THIRD_PUDDLE,
  /// The plank bridge over the gap south of the ledge.
  ACTOR_LEDGE_ROOM_BRIDGE,
  /// Breath, chased round the ledge until it is trapped.
  ACTOR_BREATH
};

/// The hole room's actors (SceneId_ShrineOfTheSeaGodPillarHole).
enum ShrineOfTheSeaGodHoleRoomActorId {
  /// The pillar Move pushes into the hole.
  ACTOR_HOLE_ROOM_PILLAR = ACTOR_FIRST_PLACED,
  /// Three unseen blocks the room draws or not together
  /// (ShrineOfTheSeaGod_SetBlocksShown).
  ACTOR_HOLE_ROOM_FIRST_BLOCK,
  ACTOR_HOLE_ROOM_SECOND_BLOCK,
  ACTOR_HOLE_ROOM_THIRD_BLOCK
};

/// The pillar room's actors (SceneId_ShrineOfTheSeaGodPillars), numbering the
/// rows of gShrineOfTheSeaGodPillarsActorPlacements.
enum ShrineOfTheSeaGodPillarRoomActorId {
  /// The plank bridges between the rock pillars, and a bridge of two logs.
  ACTOR_PILLAR_ROOM_FIRST_BRIDGE = ACTOR_FIRST_PLACED,
  ACTOR_PILLAR_ROOM_SECOND_BRIDGE,
  ACTOR_PILLAR_ROOM_THIRD_BRIDGE,
  ACTOR_PILLAR_ROOM_FOURTH_BRIDGE,
  ACTOR_PILLAR_ROOM_LOG_BRIDGE,
  /// The two broken bridges' ends, then their middles, which drop into the
  /// stream under the leader and carry him down it.
  ACTOR_FIRST_BROKEN_BRIDGE_ENDS,
  ACTOR_SECOND_BROKEN_BRIDGE_ENDS,
  ACTOR_FIRST_BROKEN_BRIDGE_MIDDLE,
  ACTOR_SECOND_BROKEN_BRIDGE_MIDDLE,
  /// The posts of the room's two ropes; Lash ties each by the post its rope is
  /// wound round, the south rope's east one and the north rope's west one.
  ACTOR_SOUTH_ROPE_EAST_POST,
  ACTOR_SOUTH_ROPE_WEST_POST,
  ACTOR_NORTH_ROPE_WEST_POST,
  ACTOR_NORTH_ROPE_EAST_POST,
  /// The splashes each bridge's middle throws up as it lands, two apiece.
  ACTOR_FIRST_BRIDGE_SPLASH,
  ACTOR_FIRST_BRIDGE_OTHER_SPLASH,
  ACTOR_SECOND_BRIDGE_SPLASH,
  ACTOR_SECOND_BRIDGE_OTHER_SPLASH,
  // In a render of the room with the leader on its ladder, under the chest,
  // taking each marker away changes nothing on screen.
  /// Unseen markers behind the chest at the top of the room's ladder; talking
  /// to one opens the field menu, as talking to nothing does.
  ACTOR_CHEST_NORTHWEST_MARKER,
  ACTOR_CHEST_NORTHEAST_MARKER,
  ACTOR_CHEST_NORTH_MARKER
};

/// The island room's actors (SceneId_ShrineOfTheSeaGodSeaGodsTear): three logs
/// floating on the water the Sea God's Tear raises.
enum ShrineOfTheSeaGodIslandActorId {
  ACTOR_ISLAND_FIRST_LOG = ACTOR_FIRST_PLACED,
  ACTOR_ISLAND_SECOND_LOG,
  ACTOR_ISLAND_THIRD_LOG
};

/// Where Breath stands on the ledge, kept in its Actor.riseCounter: at a
/// corner, or stopped beside the torch on the north side, facing it.
enum ShrineOfTheSeaGodBreathPlace {
  BREATH_AT_SOUTHEAST_CORNER,
  BREATH_AT_NORTHEAST_CORNER,
  BREATH_AT_NORTHWEST_CORNER,
  BREATH_AT_SOUTHWEST_CORNER,
  BREATH_STOPPED_EAST_OF_TORCH,
  BREATH_STOPPED_WEST_OF_TORCH
};

/// The ledge room's step triggers round the ledge, each sending Breath on
/// (ShrineOfTheSeaGod_ChaseDjinni): the ways up at its south-east and
/// south-west corners, and two places on each of its other sides.
enum ShrineOfTheSeaGodLedgeStep {
  STEP_LEDGE_SOUTHEAST_WAY_UP = 13,
  STEP_LEDGE_EAST_SIDE_SOUTH,
  STEP_LEDGE_EAST_SIDE_NORTH,
  STEP_LEDGE_NORTH_SIDE_EAST,
  STEP_LEDGE_NORTH_SIDE_WEST,
  STEP_LEDGE_WEST_SIDE_NORTH,
  STEP_LEDGE_WEST_SIDE_SOUTH,
  STEP_LEDGE_SOUTHWEST_WAY_UP
};

/// The cells that run the other rooms' events, each room numbering its own
/// after the numbers of its exits.
enum ShrineOfTheSeaGodCell {
  /// The rope room's cells (14, 20) to (18, 20) and (18, 21), where Riki tries
  /// the rope again.
  RIKI_THROWS_ROPE_CELL = 10,
  /// The crossroads' cells at the ends of its bridges and by its north
  /// doorways: (15, 12), (14, 16) to (16, 16), (23, 13), (23, 20),
  /// (34, 13) to (34, 15) and (35, 20) to (35, 22).
  CROSSROADS_BRIDGE_END_CELL = 10,
  /// The foot of the crossroads' ladder below the bridges, cell (29, 22).
  CROSSROADS_LADDER_FOOT_CELL = 11,
  /// Where Breath, seen again east of the bridges, flees south.
  CROSSROADS_BREATH_FLEES_SOUTH_CELL = 12,
  /// The crossroads' broken bridge.
  CROSSROADS_BROKEN_BRIDGE_CELL = 15,
  /// The ledge room's stairs up from the south, where Breath runs north under
  /// the bridge.
  LEDGE_ROOM_SOUTH_STAIRS_CELL = 10,
  /// South of the ledge, cells (19, 34) to (21, 34), where Breath runs west
  /// past the leader.
  LEDGE_ROOM_SOUTH_PATH_CELL = 11,
  /// The path up toward the ledge's south-west corner, cells (15, 20) to
  /// (18, 23), where Breath hops onto the ledge.
  LEDGE_ROOM_WAY_UP_CELL = 12,
  /// The pillar room's first broken bridge, cell (20, 13).
  FIRST_BROKEN_BRIDGE_CELL = 5,
  /// Its second broken bridge, cell (25, 22).
  SECOND_BROKEN_BRIDGE_CELL = 6,
  /// Before the crystal statue on the island, cell (15, 9).
  ISLAND_STATUE_CELL = 10,
  /// Before the crystal statue in the room above the island, cell (21, 15).
  STATUE_ROOM_STATUE_CELL = 1
};

/// Talking to Breath from up on the ledge, once the torch has stopped it,
/// starts its battle.
void ShrineOfTheSeaGod_FightBreath(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  u8 *place = &Actor_GetById(ACTOR_BREATH)->riseCounter;
  FIXED y;

  if ((u8)(*place - BREATH_STOPPED_EAST_OF_TORCH) >
      BREATH_STOPPED_WEST_OF_TORCH - BREATH_STOPPED_EAST_OF_TORCH)
    return;
  y = leader->y.fixed;
  if (y > toFIXED(12)) {
    GameFlag_Set(FLAG_FIGHTING_BREATH);
    if (*place == BREATH_STOPPED_EAST_OF_TORCH)
      GameFlag_Set(FLAG_BREATH_TRAPPED_EAST);
    else if (*place == BREATH_STOPPED_WEST_OF_TORCH)
      GameFlag_Set(FLAG_BREATH_TRAPPED_WEST);
    Field_RequestDjinnBattle(ACTOR_BREATH, DJINNI_BREATH);
  }
}

extern struct SceneRegion gShrineOfTheSeaGodStatueExitRegions[];
extern struct SceneRegion gShrineOfTheSeaGodLaddersExitRegions[];
extern struct SceneRegion gShrineOfTheSeaGodRightProngExitRegions[];

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

/// The regions of the rooms whose exits are rectangles: the statue room, the
/// ladders and the Right Prong's room.
const struct SceneRegion *Scene_GetExitRegions(void) {
  s16 scene = gMainRoutineWork.next;

  if (scene == SCENE_SHRINE_OF_THE_SEA_GOD_STATUE)
    return gShrineOfTheSeaGodStatueExitRegions;
  if (scene == SCENE_SHRINE_OF_THE_SEA_GOD_LADDERS)
    return gShrineOfTheSeaGodLaddersExitRegions;
  if (scene == SCENE_SHRINE_OF_THE_SEA_GOD_RIGHT_PRONG)
    return gShrineOfTheSeaGodRightProngExitRegions;
  return 0;
}

extern struct ActorPlacement gShrineOfTheSeaGod_Placements[];
extern struct ActorPlacement gShrineOfTheSeaGodRopeActorPlacements[];
extern struct ActorPlacement gShrineOfTheSeaGodPlankBridgeActorPlacements[];
extern struct ActorPlacement gShrineOfTheSeaGodCrossroadsActorPlacements[];
extern struct ActorPlacement gShrineOfTheSeaGodFootprintsActorPlacements[];
extern struct ActorPlacement gShrineOfTheSeaGodBreathActorPlacements[];
extern struct ActorPlacement gShrineOfTheSeaGodPillarHoleActorPlacements[];
extern struct ActorPlacement gShrineOfTheSeaGodPillarsActorPlacements[];
extern struct ActorPlacement gShrineOfTheSeaGodSeaGodsTearActorPlacements[];
extern struct Trigger gShrineOfTheSeaGod_Triggers[];
extern struct Trigger gShrineOfTheSeaGodRopeTriggers[];
extern struct Trigger gShrineOfTheSeaGodPlankBridgeTriggers[];
extern struct Trigger gShrineOfTheSeaGodCrossroadsTriggers[];
extern struct Trigger gShrineOfTheSeaGodFootprintsTriggers[];
extern struct Trigger gShrineOfTheSeaGodBreathTriggers[];
extern struct Trigger gShrineOfTheSeaGodPillarHoleTriggers[];
extern struct Trigger gShrineOfTheSeaGodPillarsTriggers[];
extern struct Trigger gShrineOfTheSeaGodSeaGodsTearTriggers[];
extern struct Trigger gShrineOfTheSeaGodStatueTriggers[];

const u32 *Scene_GetExits(void) { return gSceneExits; }

/// The current room's actors.
const struct ActorPlacement *Scene_GetActorPlacements(void) {
  s16 scene = gMainRoutineWork.next;

  if (scene == SCENE_SHRINE_OF_THE_SEA_GOD_ROPE)
    return (const struct ActorPlacement *)gShrineOfTheSeaGodRopeActorPlacements;
  if (scene == SCENE_SHRINE_OF_THE_SEA_GOD_PLANK_BRIDGE)
    return (const struct ActorPlacement *)
        gShrineOfTheSeaGodPlankBridgeActorPlacements;
  if (scene == SCENE_SHRINE_OF_THE_SEA_GOD_CROSSROADS)
    return (const struct ActorPlacement *)
        gShrineOfTheSeaGodCrossroadsActorPlacements;
  if (scene == SCENE_SHRINE_OF_THE_SEA_GOD_FOOTPRINTS)
    return (const struct ActorPlacement *)
        gShrineOfTheSeaGodFootprintsActorPlacements;
  if (scene == SCENE_SHRINE_OF_THE_SEA_GOD_BREATH)
    return (
        const struct ActorPlacement *)gShrineOfTheSeaGodBreathActorPlacements;
  if (scene == SCENE_SHRINE_OF_THE_SEA_GOD_PILLAR_HOLE)
    return (const struct ActorPlacement *)
        gShrineOfTheSeaGodPillarHoleActorPlacements;
  if (scene == SCENE_SHRINE_OF_THE_SEA_GOD_PILLARS)
    return (
        const struct ActorPlacement *)gShrineOfTheSeaGodPillarsActorPlacements;
  if (scene == SCENE_SHRINE_OF_THE_SEA_GOD_SEA_GODS_TEAR)
    return (const struct ActorPlacement *)
        gShrineOfTheSeaGodSeaGodsTearActorPlacements;
  return (const struct ActorPlacement *)gShrineOfTheSeaGod_Placements;
}

/// The current room's triggers.
const struct Trigger *Scene_GetTriggers(void) {
  s16 scene = gMainRoutineWork.next;

  if (scene == SCENE_SHRINE_OF_THE_SEA_GOD_ROPE)
    return gShrineOfTheSeaGodRopeTriggers;
  if (scene == SCENE_SHRINE_OF_THE_SEA_GOD_PLANK_BRIDGE)
    return gShrineOfTheSeaGodPlankBridgeTriggers;
  if (scene == SCENE_SHRINE_OF_THE_SEA_GOD_CROSSROADS)
    return gShrineOfTheSeaGodCrossroadsTriggers;
  if (scene == SCENE_SHRINE_OF_THE_SEA_GOD_FOOTPRINTS)
    return gShrineOfTheSeaGodFootprintsTriggers;
  if (scene == SCENE_SHRINE_OF_THE_SEA_GOD_BREATH)
    return gShrineOfTheSeaGodBreathTriggers;
  if (scene == SCENE_SHRINE_OF_THE_SEA_GOD_PILLAR_HOLE)
    return gShrineOfTheSeaGodPillarHoleTriggers;
  if (scene == SCENE_SHRINE_OF_THE_SEA_GOD_PILLARS)
    return gShrineOfTheSeaGodPillarsTriggers;
  if (scene == SCENE_SHRINE_OF_THE_SEA_GOD_SEA_GODS_TEAR)
    return gShrineOfTheSeaGodSeaGodsTearTriggers;
  if (scene == SCENE_SHRINE_OF_THE_SEA_GOD_STATUE)
    return gShrineOfTheSeaGodStatueTriggers;
  return gShrineOfTheSeaGod_Triggers;
}

extern u32 gShrineOfTheSeaGod_RikiLooksAround[];

/// Examining the crystal statue, on the island or in the room above it: it
/// looks like the one in Daila's Sanctum.
void ShrineOfTheSeaGod_ExamineStatue(void) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;

  Event_Start();
  Event_ResolvePendingActions(0);
  // "This statue looks a lot like the one in Daila's Sanctum."
  FieldMessage_Show((s32)MsgFieldStatueLikeDailaShrine, 1);
  work->fieldMenu = 1;
  Event_End();
}

/// Talking to Riki from behind in the side room startles him; he is busy trying
/// to help Tavi.
void ShrineOfTheSeaGod_StartleRiki(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_Stop(ACTOR_SIDE_ROOM_RIKI);
  FieldActor_ShowEmote(ACTOR_SIDE_ROOM_RIKI, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                       20);
  Actor_FaceActorById(ACTOR_SIDE_ROOM_RIKI, gMainRoutineWork.leaderActorId, 0);
  Event_SetMessageId((s32)MsgFieldStartledCompanion);
  // "Wah! You scared me!"
  Event_ShowMessageAndWait(ACTOR_SIDE_ROOM_RIKI, 0);
  // "I thought you were one of the grown-ups from Daila... So, who are you?"
  Event_ShowMessageAndWait(ACTOR_SIDE_ROOM_RIKI, 0);
  Actor_SetAnimationById(ACTOR_SIDE_ROOM_RIKI, 4);
  // "Look, I'm kinda busy trying to help my pal Tavi right now, OK?"
  Event_ShowMessageThenWait(ACTOR_SIDE_ROOM_RIKI, 0, 20);
  Actor_SetMovementTypeById(ACTOR_SIDE_ROOM_RIKI,
                            gShrineOfTheSeaGod_RikiLooksAround);
  Event_End();
}

/// Talking to an unseen marker behind the pillar room's chest opens the field
/// menu, as talking to nothing does.
void ShrineOfTheSeaGod_OpenFieldMenu(void) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;

  work->fieldMenu = 1;
}

/// The island room's water rises a step, lifting Felix, the camera and the
/// floating logs with it.
void ShrineOfTheSeaGod_LiftWithWater(void) {
  struct MapWork *map = Ram_HeapSlots->mapGrid;
  struct MapLayer *water = &map->layers[0];
  struct MapLayer *ground = &map->layers[1];
  struct MapLayer *groundTop = &map->layers[2];
  struct Actor *leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  struct Actor *camera = FieldCamera_GetActor();
  struct Actor *firstLog = Actor_GetById(ACTOR_ISLAND_FIRST_LOG);
  struct Actor *secondLog = Actor_GetById(ACTOR_ISLAND_SECOND_LOG);
  struct Actor *thirdLog = Actor_GetById(ACTOR_ISLAND_THIRD_LOG);

  water->offsetY -= toFIXED(1) / 2;
  ground->offsetY += toFIXED(1) / 2;
  groundTop->offsetY += toFIXED(1) / 2;
  leader->z.fixed -= toFIXED(1) / 2;
  camera->y.fixed -= toFIXED(1) / 2;
  firstLog->z.fixed -= toFIXED(1) / 2;
  secondLog->z.fixed -= toFIXED(1) / 2;
  thirdLog->z.fixed -= toFIXED(1) / 2;
  wait(4);
}

/// The water's layer moves a pixel as the water rises, then waits four frames.
void ShrineOfTheSeaGod_RaiseWaterLayer(void) {
  struct MapWork *map = Ram_HeapSlots->mapGrid;
  struct MapLayer *layer = &map->layers[0];

  layer->offsetY += -toFIXED(1);
  wait(4);
}

/// Copies the next band of the risen water's cells, 6 by 56 from (x, y) to
/// (18, 6), then moves the water and ground layers two pixels over 16 frames.
void ShrineOfTheSeaGod_RaiseWaterBand(s32 x, s32 y) {
  struct MapWork *map = Ram_HeapSlots->mapGrid;
  struct MapLayer *water = &map->layers[0];
  struct MapLayer *ground = &map->layers[1];
  u8 i;

  MapGrid_CopyCellsTo(x, y, 6, 56, 18, 6);
  for (i = 0; i < 16; i++) {
    water->offsetY -= toFIXED(1) / 8;
    ground->offsetY -= toFIXED(1) / 8;
    wait(1);
  }
}

void BlendScript_Suspend(void);

/// After its bands, the island room's water rises a pixel a step for this many
/// steps, then lifts the party half a pixel a step for as many again.
#define WATER_RISE_STEPS 24

/// Using the Sea God's Tear on the crystal statue on the island: the room
/// rumbles, the water rises round the island over the stepping stones, and it
/// carries Felix up to the room above (ShrineOfTheSeaGod_RiseOnWater).
void ShrineOfTheSeaGod_UseSeaGodsTear(void) {
  u32 i;

  Event_Start();
  Event_ResolvePendingActions(0);
  BlendScript_Suspend();
  Actor_FaceDirectionById(gMainRoutineWork.leaderActorId, FACING_SOUTH, 0);
  FieldCamera_MoveWithinMapBounds(toFIXED(248), -1, toFIXED(168), 1);
  FieldCamera_WaitForArrival();
  Sound_Play(SE_SANDSTORM);
  Io_Write16(BLEND_ALPHA(1, 16), &REG_BLDALPHA);
  wait(10);
  Io_Write16(BLEND_ALPHA(2, 16), &REG_BLDALPHA);
  wait(10);
  Io_Write16(BLEND_ALPHA(3, 16), &REG_BLDALPHA);
  wait(10);
  Io_Write16(BLEND_ALPHA(4, 16), &REG_BLDALPHA);
  wait(10);
  MapGrid_SetShakeFX(0, toFIXED(1), FIXED_ONE);
  MapGrid_CopyCellsTo(59, 124, 9, 12, 12, 3);
  Actor_GetById(gMainRoutineWork.leaderActorId)->motionFlags = 0;
  Actor_SetSpritePriority(gMainRoutineWork.leaderActorId, 1);
  Actor_SetSpritePriority(ACTOR_ISLAND_FIRST_LOG, 1);
  Actor_SetSpritePriority(ACTOR_ISLAND_SECOND_LOG, 1);
  Actor_SetSpritePriority(ACTOR_ISLAND_THIRD_LOG, 1);
  MapGrid_CopyCellsTo(46, 51, 6, 56, 18, 25);
  wait(10);
  MapGrid_SetShakeFX(toFIXED(1), toFIXED(1), FIXED_ONE);
  ShrineOfTheSeaGod_RaiseWaterBand(65, 51);
  ShrineOfTheSeaGod_RaiseWaterBand(65, 58);
  Io_Write16(BLEND_ALPHA(5, 15), &REG_BLDALPHA);
  ShrineOfTheSeaGod_RaiseWaterBand(65, 65);
  ShrineOfTheSeaGod_RaiseWaterBand(65, 72);
  Io_Write16(BLEND_ALPHA(6, 14), &REG_BLDALPHA);
  ShrineOfTheSeaGod_RaiseWaterBand(65, 79);
  ShrineOfTheSeaGod_RaiseWaterBand(65, 86);
  Io_Write16(BLEND_ALPHA(7, 13), &REG_BLDALPHA);
  ShrineOfTheSeaGod_RaiseWaterBand(65, 93);
  ShrineOfTheSeaGod_RaiseWaterBand(65, 100);
  Io_Write16(BLEND_ALPHA(8, 12), &REG_BLDALPHA);
  ShrineOfTheSeaGod_RaiseWaterBand(84, 51);
  ShrineOfTheSeaGod_RaiseWaterBand(84, 58);
  Io_Write16(BLEND_ALPHA(9, 11), &REG_BLDALPHA);
  ShrineOfTheSeaGod_RaiseWaterBand(84, 65);
  ShrineOfTheSeaGod_RaiseWaterBand(84, 72);
  Io_Write16(BLEND_ALPHA(10, 10), &REG_BLDALPHA);
  ShrineOfTheSeaGod_RaiseWaterBand(84, 79);
  ShrineOfTheSeaGod_RaiseWaterBand(84, 86);
  Io_Write16(BLEND_ALPHA(11, 9), &REG_BLDALPHA);
  ShrineOfTheSeaGod_RaiseWaterBand(84, 93);
  ShrineOfTheSeaGod_RaiseWaterBand(84, 100);
  Io_Write16(BLEND_ALPHA(12, 8), &REG_BLDALPHA);
  for (i = 0; i < WATER_RISE_STEPS; i++)
    ShrineOfTheSeaGod_RaiseWaterLayer();
  for (i = 0; i < WATER_RISE_STEPS; i++)
    ShrineOfTheSeaGod_LiftWithWater();
  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Event_SetExitRequest(
      SHRINE_OF_THE_SEA_GOD_SEA_GODS_TEAR_TO_SHRINE_OF_THE_SEA_GOD_STATUE);
}

/// The island room's water falls a step, lowering Felix, the camera and the
/// floating logs with it.
void ShrineOfTheSeaGod_LowerWithWater(void) {
  struct MapWork *map = Ram_HeapSlots->mapGrid;
  struct MapLayer *water = &map->layers[0];
  struct MapLayer *ground = &map->layers[1];
  struct MapLayer *groundTop = &map->layers[2];
  struct Actor *leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  struct Actor *camera = FieldCamera_GetActor();
  struct Actor *firstLog = Actor_GetById(ACTOR_ISLAND_FIRST_LOG);
  struct Actor *secondLog = Actor_GetById(ACTOR_ISLAND_SECOND_LOG);
  struct Actor *thirdLog = Actor_GetById(ACTOR_ISLAND_THIRD_LOG);

  water->offsetY += toFIXED(1) / 2;
  ground->offsetY -= toFIXED(1) / 2;
  groundTop->offsetY -= toFIXED(1) / 2;
  leader->z.fixed += toFIXED(1) / 2;
  camera->y.fixed += toFIXED(1) / 2;
  firstLog->z.fixed += toFIXED(1) / 2;
  secondLog->z.fixed += toFIXED(1) / 2;
  thirdLog->z.fixed += toFIXED(1) / 2;
  wait(4);
}

/// The water's layer moves a pixel as the water falls, then waits four frames.
void ShrineOfTheSeaGod_LowerWaterLayer(void) {
  struct MapWork *map = Ram_HeapSlots->mapGrid;
  struct MapLayer *layer = &map->layers[0];

  layer->offsetY += toFIXED(1);
  wait(4);
}

/// Copies the next band of the falling water's cells, 6 by 56 from (x, y) to
/// (18, 6), then moves the water and ground layers back two pixels over 16
/// frames.
void ShrineOfTheSeaGod_LowerWaterBand(s32 x, s32 y) {
  struct MapWork *map = Ram_HeapSlots->mapGrid;
  struct MapLayer *water = &map->layers[0];
  struct MapLayer *ground = &map->layers[1];
  u8 i;

  MapGrid_CopyCellsTo(x, y, 6, 56, 18, 6);
  for (i = 0; i < 16; i++) {
    water->offsetY += toFIXED(1) / 8;
    ground->offsetY += toFIXED(1) / 8;
    wait(1);
  }
}

/// Back on the island after using the Tear in the room above, the water drains
/// away round Felix until the stepping stones show again.
void ShrineOfTheSeaGod_LowerWater(void) {
  struct MapWork *map = Ram_HeapSlots->mapGrid;
  struct MapLayer *water = &map->layers[0];
  struct MapLayer *ground = &map->layers[1];
  struct MapLayer *groundTop = &map->layers[2];
  struct Actor *leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  struct Actor *camera = FieldCamera_GetActor();
  struct Actor *firstLog = Actor_GetById(ACTOR_ISLAND_FIRST_LOG);
  struct Actor *secondLog = Actor_GetById(ACTOR_ISLAND_SECOND_LOG);
  struct Actor *thirdLog = Actor_GetById(ACTOR_ISLAND_THIRD_LOG);
  u32 i;

  Event_Start();
  Event_ResolvePendingActions(0);
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  Actor_GetById(gMainRoutineWork.leaderActorId)->motionFlags = 0;
  Actor_SetSpritePriority(gMainRoutineWork.leaderActorId, 1);
  Actor_SetSpritePriority(ACTOR_ISLAND_FIRST_LOG, 1);
  Actor_SetSpritePriority(ACTOR_ISLAND_SECOND_LOG, 1);
  Actor_SetSpritePriority(ACTOR_ISLAND_THIRD_LOG, 1);
  BlendScript_Suspend();
  wait(1);
  MapGrid_CopyCellsTo(46, 51, 6, 56, 18, 25);
  MapGrid_CopyCellsTo(84, 100, 6, 56, 18, 6);
  MapGrid_CopyCellsTo(59, 124, 9, 12, 12, 3);
  Io_Write16(BLEND_ALPHA(12, 8), &REG_BLDALPHA);
  wait(1);
  water->offsetY += -toFIXED(68);
  ground->offsetY += -toFIXED(20);
  groundTop->offsetY += toFIXED(12);
  leader->z.fixed += -toFIXED(12);
  camera->y.fixed += -toFIXED(12);
  firstLog->z.fixed += -toFIXED(12);
  secondLog->z.fixed += -toFIXED(12);
  thirdLog->z.fixed += -toFIXED(12);
  MapGrid_CenterOnFollowedPoint();
  wait(4);
  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  MapGrid_SetShakeFX(toFIXED(1), toFIXED(1), FIXED_ONE);
  Sound_Play(SE_AMBIENT_STREAM);
  // One step more of each than the rise took: the water's layer ends a pixel
  // and a half from where it began, the ground's half a pixel.
  i = 0;
  do {
    i++;
    ShrineOfTheSeaGod_LowerWithWater();
  } while (i <= WATER_RISE_STEPS);
  i = 0;
  do {
    i++;
    ShrineOfTheSeaGod_LowerWaterLayer();
  } while (i <= WATER_RISE_STEPS);
  ShrineOfTheSeaGod_LowerWaterBand(84, 93);
  ShrineOfTheSeaGod_LowerWaterBand(84, 86);
  Io_Write16(BLEND_ALPHA(11, 9), &REG_BLDALPHA);
  ShrineOfTheSeaGod_LowerWaterBand(84, 79);
  ShrineOfTheSeaGod_LowerWaterBand(84, 72);
  Io_Write16(BLEND_ALPHA(10, 10), &REG_BLDALPHA);
  ShrineOfTheSeaGod_LowerWaterBand(84, 65);
  ShrineOfTheSeaGod_LowerWaterBand(84, 58);
  Io_Write16(BLEND_ALPHA(9, 11), &REG_BLDALPHA);
  ShrineOfTheSeaGod_LowerWaterBand(84, 51);
  ShrineOfTheSeaGod_LowerWaterBand(65, 100);
  Io_Write16(BLEND_ALPHA(8, 12), &REG_BLDALPHA);
  ShrineOfTheSeaGod_LowerWaterBand(65, 93);
  ShrineOfTheSeaGod_LowerWaterBand(65, 86);
  Io_Write16(BLEND_ALPHA(7, 13), &REG_BLDALPHA);
  ShrineOfTheSeaGod_LowerWaterBand(65, 79);
  ShrineOfTheSeaGod_LowerWaterBand(65, 72);
  Io_Write16(BLEND_ALPHA(6, 14), &REG_BLDALPHA);
  ShrineOfTheSeaGod_LowerWaterBand(65, 65);
  ShrineOfTheSeaGod_LowerWaterBand(65, 58);
  Io_Write16(BLEND_ALPHA(5, 15), &REG_BLDALPHA);
  ShrineOfTheSeaGod_LowerWaterBand(65, 51);
  MapGrid_SetShakeFX(0, toFIXED(1), FIXED_ONE);
  Io_Write16(BLEND_ALPHA(4, 16), &REG_BLDALPHA);
  wait(10);
  Sound_Play(SOUND_FADE_OUT_ACTION_SE);
  MapGrid_SetShakeFX(-1, -1, 9 * FIXED_ONE / 10);
  Io_Write16(BLEND_ALPHA(3, 16), &REG_BLDALPHA);
  wait(10);
  Io_Write16(BLEND_ALPHA(2, 16), &REG_BLDALPHA);
  wait(10);
  Io_Write16(BLEND_ALPHA(1, 16), &REG_BLDALPHA);
  wait(10);
  Io_Write16(BLEND_ALPHA(0, 16), &REG_BLDALPHA);
  MapGrid_CopyCellsTo(103, 51, 6, 56, 18, 25);
  MapGrid_CopyCellsTo(46, 108, 9, 12, 12, 3);
  wait(10);
  MapGrid_WaitForShakeToSettle();
  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
  ShrineOfTheSeaGod_SetUpFloatingLog(ACTOR_ISLAND_FIRST_LOG);
  ShrineOfTheSeaGod_SetUpFloatingLog(ACTOR_ISLAND_SECOND_LOG);
  ShrineOfTheSeaGod_SetUpFloatingLog(ACTOR_ISLAND_THIRD_LOG);
  Actor_GetById(gMainRoutineWork.leaderActorId)->priorityFlags |=
      ACTOR_PRIORITY_AUTOMATIC;
  leader->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  leader->y.fixed = 0;
  leader->groundHeight = 0;
  Event_End();
}

extern s16 gShrineOfTheSeaGod_PushedActors[];

/// How far the water carries the leader between the island and the statue
/// room, in pixels, half a pixel every two frames.
#define STATUE_ROOM_RISE 50
/// Sinking back to the island, the steps of half a pixel left when the screen
/// starts to close.
#define STATUE_ROOM_CLOSE_STEPS 10

/// Arriving in the room above the island, Felix rises up through the floor on
/// the crystal statue's platform as the screen opens. The platform is the
/// ground layer, which starts STATUE_ROOM_RISE pixels down with him.
void ShrineOfTheSeaGod_RiseOnWater(void) {
  struct MapWork *map = Ram_HeapSlots->mapGrid;
  struct MapLayer *layer;
  struct Actor *leader;
  u32 i;

  layer = &map->layers[1];
  leader = Actor_GetById(gMainRoutineWork.leaderActorId);

  Event_Start();
  Event_ResolvePendingActions(0);
  Sound_Play(SE_SANDSTORM);
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  FieldCamera_MoveWithinMapBounds(toFIXED(344), -1, toFIXED(248), 0);
  MapGrid_CopyCellsTo(62, 77, 62, 18, 19, 6);
  wait(1);
  Actor_SetSpritePriority(gMainRoutineWork.leaderActorId, 2);
  leader->motionFlags = 0;
  leader->y.fixed = -toFIXED(STATUE_ROOM_RISE);
  leader->groundHeight = -toFIXED(STATUE_ROOM_RISE);
  layer->offsetY += -toFIXED(STATUE_ROOM_RISE);
  MapGrid_CenterOnFollowedPoint();
  wait(1);
  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_CIRCLE);
  Event_StartScreenTransition();
  MapGrid_SetShakeFX(toFIXED(1), toFIXED(1), FIXED_ONE);
  for (i = 0; i < STATUE_ROOM_RISE * 2; i++) {
    leader->y.fixed += toFIXED(1) / 2;
    leader->groundHeight += toFIXED(1) / 2;
    layer->offsetY += toFIXED(1) / 2;
    wait(2);
  }
  Sound_Play(SE_STOP_ACTION_SE);
  Sound_Play(SOUND_FADE_OUT_ACTION_SE);
  MapGrid_SetScreenShake(0, 0, 0);
  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
  Actor_GetById(gMainRoutineWork.leaderActorId)->priorityFlags |=
      ACTOR_PRIORITY_AUTOMATIC;
  MapGrid_CopyCellsTo(62, 69, 62, 18, 19, 6);
  wait(1);
  Event_End();
}

/// Using the Sea God's Tear on the crystal statue in the room above the island
/// takes Felix back down: he turns to face south, the statue's platform sinks
/// into the floor with him, the screen closes round him, and he is back on the
/// island, where the water drains away (ShrineOfTheSeaGod_LowerWater). The
/// platform is the ground layer, lowered STATUE_ROOM_RISE pixels with him.
void ShrineOfTheSeaGod_SinkWithWater(void) {
  struct MapWork *map = Ram_HeapSlots->mapGrid;
  struct MapLayer *layer;
  struct Actor *actor;
  u32 i;

  layer = &map->layers[1];
  actor = Actor_GetById(gMainRoutineWork.leaderActorId);

  Event_Start();
  Event_ResolvePendingActions(0);
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  FieldCamera_MoveWithinMapBounds(toFIXED(344), -1, toFIXED(248), 1);
  MapGrid_CopyCellsTo(62, 77, 62, 18, 19, 6);
  Actor_FaceDirectionById(gMainRoutineWork.leaderActorId, FACING_SOUTH, 20);
  Actor_SetSpritePriority(gMainRoutineWork.leaderActorId, 2);
  actor->motionFlags = 0;
  Sound_Play(SE_AMBIENT_STREAM);
  MapGrid_SetShakeFX(toFIXED(1), toFIXED(1), FIXED_ONE);
  for (i = 0; i < STATUE_ROOM_RISE * 2; i++) {
    actor->y.fixed -= toFIXED(1) / 2;
    actor->groundHeight -= toFIXED(1) / 2;
    layer->offsetY -= toFIXED(1) / 2;
    wait(2);
    if (i == STATUE_ROOM_RISE * 2 - STATUE_ROOM_CLOSE_STEPS) {
      gFieldWork->transition =
          SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_CIRCLE);
      Event_FinishScreenTransition();
    }
  }
  Event_SetExitRequest(
      SHRINE_OF_THE_SEA_GOD_STATUE_TO_SHRINE_OF_THE_SEA_GOD_SEA_GODS_TEAR);
}

/// Sets up one of the ledge room's puddles, its ice pillar not shown until
/// Frost freezes it.
void ShrineOfTheSeaGod_SetUpPuddle(s32 actor) {
  Actor_DisableById(actor);
  Actor_GetById(actor)->priorityFlags |= ACTOR_PRIORITY_IGNORES_RISES;
}

/// Frost on the westernmost of the three puddles west of Breath's ledge, cell
/// (8, 18), freezes it into a pillar of ice. Felix hops along the three ice
/// pillars to the way out at the room's north-west, cell (7, 12), to the room
/// with the pillar over the hole; each frozen cell is given the hop of the
/// spare cell in the map's corner, (0, 0).
void ShrineOfTheSeaGod_FreezeFirstPuddle(void) {
  MapGrid_CopyCellAttributes(0, 0, 1, 1, 8, 18);
  GameFlag_Set(FLAG_SHRINE_FIRST_PUDDLE_FROZEN);
}

/// Frost on the puddle two cells east of the first, cell (10, 18), the south
/// one of the pair beside it, freezes it into a pillar of ice to hop along.
void ShrineOfTheSeaGod_FreezeSecondPuddle(void) {
  MapGrid_CopyCellAttributes(0, 0, 1, 1, 10, 18);
  GameFlag_Set(FLAG_SHRINE_SECOND_PUDDLE_FROZEN);
}

/// Frost on the north puddle of that pair, cell (10, 17), freezes it into a
/// pillar of ice to hop along.
void ShrineOfTheSeaGod_FreezeThirdPuddle(void) {
  MapGrid_CopyCellAttributes(0, 0, 1, 1, 10, 17);
  GameFlag_Set(FLAG_SHRINE_THIRD_PUDDLE_FROZEN);
}

/// Move lifts the room's torch, or in the crossroads its stone pillar.
void ShrineOfTheSeaGod_LiftBlock(void) {
  Actor_LiftTargetBlock(gShrineOfTheSeaGod_PushedActors);
}

/// Sets the torch down, noting whether it stands against the north wall where
/// it traps Breath.
void ShrineOfTheSeaGod_SetDownTorch(void) {
  struct Actor *actor;

  Actor_PlaceMovedBlock();
  actor = Actor_GetById(ACTOR_TORCH);
  if (FIELD_CELL(actor->x.fixed) == 21 && FIELD_CELL(actor->z.fixed) == 13)
    GameFlag_Set(FLAG_TORCH_AT_NORTH_WALL);
  else
    GameFlag_Clear(FLAG_TORCH_AT_NORTH_WALL);
}

/// Sets the crossroads' stone pillar down; pushed all the way west, it stays
/// there.
void ShrineOfTheSeaGod_MovePillar(void) {
  struct Actor *actor = Actor_GetById(ACTOR_CROSSROADS_PILLAR);

  Actor_PlaceMovedBlock();
  if (FIELD_CELL(actor->x.fixed) == 12)
    GameFlag_Set(FLAG_CROSSROADS_PILLAR_MOVED);
}

/// Lets a pillar fall to the ground with a thump.
void ShrineOfTheSeaGod_DropPillar(struct Actor *actor) {
  actor->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  do {
    wait(1);
  } while (actor->velocityY != 0);
  Sound_Play(SE_THUMP);
  wait(10);
  actor->motionFlags = 0;
}

/// Pushed onto the hole, the pillar drops in and fills it.
void ShrineOfTheSeaGod_PushPillarIntoHole(void) {
  struct Actor *actor = Actor_GetById(ACTOR_HOLE_ROOM_PILLAR);
  s32 z = FIELD_CELL(actor->z.fixed);

  if (z == 16) {
    Event_Start();
    Event_ResolvePendingActions(0);
    ShrineOfTheSeaGod_DropPillar(actor);
    Map_CopyCellAttributeRect(0, 0, 1, 1, 15, z);
    Actor_SetSpritePriority(ACTOR_HOLE_ROOM_PILLAR, 3);
    Event_End();
    GameFlag_Set(FLAG_PILLAR_IN_HOLE);
  }
}

/// The crossroads' task: its bridges are drawn under the leader when he is up
/// on them, in front of him below.
void ShrineOfTheSeaGod_LayerCrossroadsBridges(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  u32 id;

  for (id = ACTOR_CROSSROADS_LOG_BRIDGE;
       id <= ACTOR_CROSSROADS_BROKEN_BRIDGE_ENDS; id++) {
    struct Actor *actor = Actor_GetById(id);

    if (leader->y.fixed > toFIXED(16)) {
      Actor_SetSpritePriority(id, 2);
      actor->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
    } else {
      Actor_SetSpritePriority(id, 1);
      actor->priorityFlags &= ~ACTOR_PRIORITY_UNDERFOOT;
    }
  }
}

/// Notes whether the leader stands on the upper level of the room.
void ShrineOfTheSeaGod_TrackUpperLevel(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.leaderActorId);

  Actor_GetCellIndex(leader);
  if (leader->y.fixed < toFIXED(16))
    gMainRoutineWork.onUpperLevel = 0;
  else
    gMainRoutineWork.onUpperLevel = 1;
}

/// The ledge room's task: the puddles are drawn behind or in front of the
/// leader by where it stands.
void ShrineOfTheSeaGod_LayerPuddles(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  FIXED y;
  FIXED z;

  y = leader->y.fixed;
  if (y < toFIXED(16)) {
    z = leader->z.fixed;
    if (z < toFIXED(296)) {
      Actor_SetSpritePriority(ACTOR_FIRST_PUDDLE, 1);
      Actor_SetSpritePriority(ACTOR_SECOND_PUDDLE, 1);
      Actor_SetSpritePriority(ACTOR_THIRD_PUDDLE, 2);
    } else {
      Actor_SetSpritePriority(ACTOR_FIRST_PUDDLE, 2);
      Actor_SetSpritePriority(ACTOR_SECOND_PUDDLE, 2);
      Actor_SetSpritePriority(ACTOR_THIRD_PUDDLE, 3);
    }
  } else {
    Actor_SetSpritePriority(ACTOR_FIRST_PUDDLE, 2);
    Actor_SetSpritePriority(ACTOR_SECOND_PUDDLE, 2);
    Actor_SetSpritePriority(ACTOR_THIRD_PUDDLE, 3);
  }
}

/// Keeps the marker on the torch's flame, 20 pixels up, as Move carries the
/// torch.
void ShrineOfTheSeaGod_KeepFlameOnTorch(void) {
  struct Actor *torch = Actor_GetById(ACTOR_TORCH);
  struct Actor *marker = Actor_GetById(ACTOR_TORCH_FLAME_MARKER);

  SET_ACTOR_POSITION(marker, torch->x.fixed, toFIXED(20), torch->z.fixed);
}

// field_actors.c's priority update answers what ACTOR_PRIORITY_IGNORES_RISES
// does: with it, an actor takes only the priority of the cells under its feet,
// and ground rising a cell or two in front of it never puts it further back.
/// Unused: lets the leader be put behind rising ground again, undoing what the
/// hole room sets on arrival (ACTOR_PRIORITY_IGNORES_RISES).
UNUSED void ShrineOfTheSeaGod_ClearLeaderIgnoresRises(void) {
  Actor_GetById(gMainRoutineWork.leaderActorId)->priorityFlags &=
      ~ACTOR_PRIORITY_IGNORES_RISES;
}

/// Unused: keeps the leader in front of rising ground, as the hole room does.
UNUSED void ShrineOfTheSeaGod_SetLeaderIgnoresRises(void) {
  Actor_GetById(gMainRoutineWork.leaderActorId)->priorityFlags |=
      ACTOR_PRIORITY_IGNORES_RISES;
}

void Actors_SetDrawn(s32 show, s32 first, u32 count);

/// Draws (show 1) or stops drawing the hole room's three unseen blocks.
void ShrineOfTheSeaGod_SetBlocksShown(s32 show) {
  if (gMainRoutineWork.next == SCENE_SHRINE_OF_THE_SEA_GOD_PILLAR_HOLE)
    Actors_SetDrawn(show, ACTOR_HOLE_ROOM_FIRST_BLOCK,
                    ACTOR_HOLE_ROOM_THIRD_BLOCK - ACTOR_HOLE_ROOM_FIRST_BLOCK +
                        1);
}

/// Unused: draws the hole room's unseen blocks and notes them shown.
UNUSED void ShrineOfTheSeaGod_ShowBlocks(void) {
  ShrineOfTheSeaGod_SetBlocksShown(1);
  GameFlag_Set(FLAG_BLOCKS_SHOWN);
}

/// Stops drawing the hole room's unseen blocks.
void ShrineOfTheSeaGod_HideBlocks(void) {
  ShrineOfTheSeaGod_SetBlocksShown(0);
  GameFlag_Clear(FLAG_BLOCKS_SHOWN);
}

#include "shared/actors_set_hidden.c"

/// Nothing visible happens when Felix steps onto a cell at the end of one of
/// the crossroads' bridges or by its north doorways. The game sets him to be
/// drawn in front of the bridges from then on, so that he shows on top of the
/// one he walks across, until he comes down the ladder below them.
void ShrineOfTheSeaGod_DrawLeaderInFront(void) {
  Actor_SetSpritePriority(gMainRoutineWork.leaderActorId, 1);
  GameFlag_Set(FLAG_LEADER_IN_FRONT);
}

/// Nothing visible happens when Felix comes down to the foot of the
/// crossroads' ladder below the bridges, cell (29, 22). The game lets the
/// ground under him decide again how he is drawn, so that he passes behind the
/// bridges overhead.
void ShrineOfTheSeaGod_DrawLeaderBehind(void) {
  Actor_SetSpritePriority(gMainRoutineWork.leaderActorId, 2);
  Actor_GetById(gMainRoutineWork.leaderActorId)->priorityFlags |=
      ACTOR_PRIORITY_AUTOMATIC;
  GameFlag_Clear(FLAG_LEADER_IN_FRONT);
}

#include "../field/field_effect_advance_motion.c"

#undef EFFECT_SET_MODE
#undef EFFECT_SET_SCRIPT

FIXED gShrineOfTheSeaGod_BridgeOffset[3] BSS_DATA;

/// On a broken bridge's middle the leader sways twice, the middle dipping under
/// him, and loses his shadow; the middle's place against him is kept for the
/// fall (ShrineOfTheSeaGod_KeepWithLeader).
void ShrineOfTheSeaGod_RockOnBridge(struct Actor *leader,
                                    struct Actor *middle) {
  struct FieldSprite *sprite = leader->sprite;
  u32 n;
  u32 i;

  FieldEffect_SetAttached(gMainRoutineWork.leaderActorId,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Sound_Play(SE_HIDDEN_PASSAGE_OPEN);
  leader->motionFlags = 0;
  for (n = 0; n <= 1; n++) {
    middle->y.fixed -= toFIXED(1);
    for (i = 0; i <= 3; i++) {
      sprite->rotation += TRIG_TURN / 512;
      wait(1);
    }
    middle->y.fixed += toFIXED(1);
    for (i = 0; i <= 7; i++) {
      sprite->rotation -= TRIG_TURN / 512;
      wait(1);
    }
    middle->y.fixed -= toFIXED(1);
    for (i = 0; i <= 3; i++) {
      sprite->rotation += TRIG_TURN / 512;
      wait(1);
    }
  }
  middle->y.fixed += toFIXED(1);
  wait(20);
  gShrineOfTheSeaGod_BridgeOffset[0] = middle->x.fixed - leader->x.fixed;
  gShrineOfTheSeaGod_BridgeOffset[1] = middle->y.fixed - leader->y.fixed;
  gShrineOfTheSeaGod_BridgeOffset[2] = middle->z.fixed - leader->z.fixed;
  Actor_SetSpriteFlags(Actor_GetById(gMainRoutineWork.leaderActorId), 0);
}

/// Keeps a broken bridge's middle where it lies against the leader as he
/// falls.
void ShrineOfTheSeaGod_KeepWithLeader(struct Actor *actor) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  FIXED *offset = gShrineOfTheSeaGod_BridgeOffset;

  SET_ACTOR_POSITION(actor, leader->x.fixed + offset[0],
                     leader->y.fixed + offset[1], leader->z.fixed + offset[2]);
}

/// The task that carries the crossroads' broken bridge's middle along with the
/// leader.
void ShrineOfTheSeaGod_KeepCrossroadsBridgeWithLeader(void) {
  ShrineOfTheSeaGod_KeepWithLeader(
      Actor_GetById(ACTOR_CROSSROADS_BROKEN_BRIDGE_MIDDLE));
}

void FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                       s32 velocityZ, u32 flags,
                       const struct EffectOptions *options);

/// The spray the crossroads' bridge throws up as it hits the water: a drop
/// every sixteenth of a turn, the first direction thrown twice.
#define SPRAY_DIRECTIONS 16

/// On the crossroads' broken bridge the leader sways until its middle drops
/// with him into the water below, throwing up spray, and tips there.
void ShrineOfTheSeaGod_BreakCrossroadsBridge(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  struct Actor *middle = Actor_GetById(ACTOR_CROSSROADS_BROKEN_BRIDGE_MIDDLE);
  struct FieldSprite *sprite = middle->sprite;
  struct EffectOptions options;
  u32 i;

  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_WalkToAndWaitById(gMainRoutineWork.leaderActorId, 408, 328);
  ShrineOfTheSeaGod_RockOnBridge(leader, middle);
  MapGrid_CopyCellAttributes(0, 50, 3, 1, 24, 70);
  GameFlag_Set(FLAG_CROSSROADS_BRIDGE_BROKEN);
  wait(1);
  leader->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  Actor_SetSpritePriority(ACTOR_CROSSROADS_BROKEN_BRIDGE_MIDDLE, 3);
  middle->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
  FieldTask_AddFrame(ShrineOfTheSeaGod_KeepCrossroadsBridgeWithLeader,
                     TASK_PASS_FRAME);
  do
    wait(1);
  while (leader->velocityY != 0);
  Sound_Play(SE_TOPPLE);
  MapGrid_SetShakeFX(toFIXED(1), toFIXED(2), FIXED_ONE);
  options.palette = 7;
  options.update = FieldEffect_AdvanceMotion;
  options.startScaleX = FIXED_ONE * 64 / 100;
  options.startScaleY = FIXED_ONE * 64 / 100;
  for (i = 0; i <= SPRAY_DIRECTIONS;) {
    FIXED velocity[3];

    velocity[0] = Trig_Cos(i * (TRIG_TURN / SPRAY_DIRECTIONS));
    velocity[1] = 0;
    velocity[2] = Trig_Sin(i * (TRIG_TURN / SPRAY_DIRECTIONS));
    velocity[0] += velocity[0] >> 1;
    velocity[0] += ((Random16() * 3) << 14) >> 16;
    i++;
    velocity[0] += -(toFIXED(3) / 8);
    velocity[2] += (((Random16() * 3) << 13) >> 16) - toFIXED(3) / 16;
    FieldEffect_Spawn(leader->x.fixed, leader->y.fixed - toFIXED(4),
                      leader->z.fixed, velocity[0], velocity[1], velocity[2],
                      EFFECT_USE_UPDATE | EFFECT_USE_START_SCALE |
                          EFFECT_USE_PALETTE | 1,
                      &options);
  }
  leader->velocityY = toFIXED(4);
  wait(2);
  MapGrid_SetShakeFX(-1, -1, 9 * FIXED_ONE / 10);
  Task_Remove(ShrineOfTheSeaGod_KeepCrossroadsBridgeWithLeader);
  for (i = 0; i < 8; i++) {
    sprite->rotation += TRIG_TURN / 128;
    middle->z.fixed += toFIXED(1);
    wait(1);
  }
  MapGrid_WaitForShakeToSettle();
  Actor_SetSpriteFlags(Actor_GetById(gMainRoutineWork.leaderActorId),
                       SPRITE_FLAG_SHADOW);
  leader->layer = 0;
  wait(10);
  Event_End();
}

/// Waits for the leader, falling with a broken bridge's middle, to land in the
/// stream, then throws up the bridge's two splashes there; bridge counts the
/// splashes from ACTOR_FIRST_BRIDGE_SPLASH.
void ShrineOfTheSeaGod_LandBrokenBridge(struct Actor *actor, s32 bridge) {
  do {
    wait(1);
  } while (actor->velocityY != 0);
  Sound_Play(SE_LOG_FALL);
  Actor_SetSpritePriority(ACTOR_FIRST_BRIDGE_SPLASH + bridge, 3);
  Actor_SetSpritePriority(ACTOR_FIRST_BRIDGE_OTHER_SPLASH + bridge, 3);
  Actor_SetPositionAndResetMotion(
      Actor_GetById(ACTOR_FIRST_BRIDGE_SPLASH + bridge), actor->x.fixed, 0,
      actor->z.fixed);
  Actor_SetPositionAndResetMotion(
      Actor_GetById(ACTOR_FIRST_BRIDGE_OTHER_SPLASH + bridge), actor->x.fixed,
      0, actor->z.fixed - toFIXED(16));
  Actor_SetAnimationById(ACTOR_FIRST_BRIDGE_SPLASH + bridge, 1);
  Actor_SetAnimationById(ACTOR_FIRST_BRIDGE_OTHER_SPLASH + bridge, 1);
  wait(20);
}

/// The task that carries the pillar room's first broken bridge's middle along
/// with the leader.
void ShrineOfTheSeaGod_KeepFirstBridgeWithLeader(void) {
  ShrineOfTheSeaGod_KeepWithLeader(
      Actor_GetById(ACTOR_FIRST_BROKEN_BRIDGE_MIDDLE));
}

/// On the pillar room's first broken bridge the leader sways until its middle
/// drops with him into the stream, which carries him south on it.
void ShrineOfTheSeaGod_BreakFirstBridge(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  struct Actor *middle = Actor_GetById(ACTOR_FIRST_BROKEN_BRIDGE_MIDDLE);

  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_WalkToAndWaitById(gMainRoutineWork.leaderActorId, 328, 216);
  ShrineOfTheSeaGod_RockOnBridge(leader, middle);
  Map_CopyCellAttributeRect(0, 0, 3, 1, 19, 13);
  Map_CopyCellAttributeRect(40, 0, 2, 1, 19, 33);
  wait(1);
  leader->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  Actor_SetSpritePriority(ACTOR_FIRST_BROKEN_BRIDGE_MIDDLE, 3);
  middle->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
  FieldTask_AddFrame(ShrineOfTheSeaGod_KeepFirstBridgeWithLeader,
                     TASK_PASS_FRAME);
  ShrineOfTheSeaGod_LandBrokenBridge(leader, 0);
  // Down in the stream, the bridges it passes under are drawn over him.
  Actor_SetSpritePriority(ACTOR_PILLAR_ROOM_SECOND_BRIDGE, 1);
  Actor_GetById(ACTOR_PILLAR_ROOM_SECOND_BRIDGE)->priorityFlags &=
      ~ACTOR_PRIORITY_UNDERFOOT;
  Actor_SetSpritePriority(ACTOR_PILLAR_ROOM_FOURTH_BRIDGE, 1);
  Actor_GetById(ACTOR_PILLAR_ROOM_FOURTH_BRIDGE)->priorityFlags &=
      ~ACTOR_PRIORITY_UNDERFOOT;
  leader->motionFlags = 0;
  Actor_SetSpriteFlags(Actor_GetById(gMainRoutineWork.leaderActorId),
                       SPRITE_FLAG_SHADOW);
  Actor_SetSpeedAndAcceleration(gMainRoutineWork.leaderActorId, 0.6, 0.1);
  Actor_MoveToAndWaitById(gMainRoutineWork.leaderActorId, 328, 224);
  Actor_MoveToAndWait(gMainRoutineWork.leaderActorId, 321, 256);
  Actor_SetHorizontalPositionWithTerrain(ACTOR_FIRST_BRIDGE_SPLASH, 0, 0);
  Actor_SetHorizontalPositionWithTerrain(ACTOR_FIRST_BRIDGE_OTHER_SPLASH, 0, 0);
  Actor_MoveToAndWait(gMainRoutineWork.leaderActorId, 321, 320);
  Actor_MoveToAndWait(gMainRoutineWork.leaderActorId, 321, 400);
  Actor_MoveToAndWait(gMainRoutineWork.leaderActorId, 321, 480);
  Actor_MoveToAndWaitById(gMainRoutineWork.leaderActorId, 321, 536);
  Sound_Play(SE_RATTLE);
  Actor_SetSpritePriority(ACTOR_PILLAR_ROOM_SECOND_BRIDGE, 2);
  Actor_GetById(ACTOR_PILLAR_ROOM_SECOND_BRIDGE)->priorityFlags |=
      ACTOR_PRIORITY_UNDERFOOT;
  Actor_SetSpritePriority(ACTOR_PILLAR_ROOM_FOURTH_BRIDGE, 2);
  Actor_GetById(ACTOR_PILLAR_ROOM_FOURTH_BRIDGE)->priorityFlags |=
      ACTOR_PRIORITY_UNDERFOOT;
  leader->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  Task_Remove(ShrineOfTheSeaGod_KeepFirstBridgeWithLeader);
  wait(1);
  Event_End();
  GameFlag_Set(FLAG_FIRST_BRIDGE_BROKEN);
}

/// The task that carries the pillar room's second broken bridge's middle along
/// with the leader.
void ShrineOfTheSeaGod_KeepSecondBridgeWithLeader(void) {
  ShrineOfTheSeaGod_KeepWithLeader(
      Actor_GetById(ACTOR_SECOND_BROKEN_BRIDGE_MIDDLE));
}

/// On the pillar room's second broken bridge the leader sways until its middle
/// drops with him into the stream, which carries him a little way south.
void ShrineOfTheSeaGod_BreakSecondBridge(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  struct Actor *middle = Actor_GetById(ACTOR_SECOND_BROKEN_BRIDGE_MIDDLE);

  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_WalkToAndWait(gMainRoutineWork.leaderActorId, 408, 360);
  ShrineOfTheSeaGod_RockOnBridge(leader, middle);
  Map_CopyCellAttributeRect(0, 0, 3, 1, 24, 22);
  Map_CopyCellAttributeRect(40, 0, 2, 1, 24, 24);
  wait(1);
  leader->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  Actor_SetSpritePriority(ACTOR_SECOND_BROKEN_BRIDGE_MIDDLE, 3);
  middle->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
  FieldTask_AddFrame(ShrineOfTheSeaGod_KeepSecondBridgeWithLeader,
                     TASK_PASS_FRAME);
  ShrineOfTheSeaGod_LandBrokenBridge(leader, ACTOR_SECOND_BRIDGE_SPLASH -
                                                 ACTOR_FIRST_BRIDGE_SPLASH);
  Actor_SetSpriteFlags(Actor_GetById(gMainRoutineWork.leaderActorId),
                       SPRITE_FLAG_SHADOW);
  Actor_SetSpeedAndAcceleration(gMainRoutineWork.leaderActorId, 0.6, 0.1);
  Actor_MoveToAndWaitById(gMainRoutineWork.leaderActorId, 406, 360);
  Actor_MoveToAndWaitById(gMainRoutineWork.leaderActorId, 400, 392);
  Sound_Play(SE_RATTLE);
  Actor_SetHorizontalPositionWithTerrain(ACTOR_SECOND_BRIDGE_SPLASH, 0, 0);
  Actor_SetHorizontalPositionWithTerrain(ACTOR_SECOND_BRIDGE_OTHER_SPLASH, 0,
                                         0);
  Task_Remove(ShrineOfTheSeaGod_KeepSecondBridgeWithLeader);
  wait(1);
  Event_End();
  GameFlag_Set(FLAG_SECOND_BRIDGE_BROKEN);
}

/// Reveal shows the seventh stepping stone in the hexagon of rocks.
void ShrineOfTheSeaGod_RevealStone(void) {
  MapGrid_CopyCellsTo(42, 1, 15, 14, 1, 2);
  MapGrid_CopyCellAttributes(0, 0, 1, 1, 15, 14);
}

/// As Reveal fades, the seventh stone is hidden again.
void ShrineOfTheSeaGod_HideStone(void) {
  MapGrid_CopyCellsTo(15, 9, 15, 14, 1, 2);
  MapGrid_CopyCellAttributes(14, 14, 1, 1, 15, 14);
}

/// Back in the rope room, Riki tries to throw the rope up to Tavi again, misses
/// and goes off; Kraden joins the leader.
void ShrineOfTheSeaGod_ThrowRopeAgain(void) {
  struct Actor *leader;

  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgFieldRopeThrownAgain);
  // "All right! I'm gonna throw the rope again!"
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_RIKI, 0);
  Actor_SetSpeed(gMainRoutineWork.leaderActorId, 0.8);
  Actor_SetSpeed(ACTOR_ROPE_ROOM_KRADEN, 0.8);
  Actor_WalkToAndWait(gMainRoutineWork.leaderActorId, 272, 336);
  Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_NORTH, 0);
  Actor_SetHorizontalPositionWithTerrain(ACTOR_ROPE_ROOM_KRADEN, toFIXED(272),
                                         toFIXED(336));
  wait(1);
  Actor_WalkToAndWait(ACTOR_ROPE_ROOM_KRADEN, 256, 336);
  Actor_FaceDirectionById(ACTOR_ROPE_ROOM_KRADEN, FACING_EAST, 0);
  Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_WEST, 40);
  Actor_FaceDirection(ACTOR_ROPE_ROOM_KRADEN, FACING_NORTH_NORTHEAST, 0);
  Actor_FaceDirectionById(gMainRoutineWork.leaderActorId, FACING_NORTH, 0);
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  FieldCamera_SetSpeed(4.8);
  FieldCamera_MoveTo(288, CAMERA_KEEP, 126, 1);
  FieldCamera_WaitForArrival();
  Actor_Jump(ACTOR_TAVI, 4, 20);
  // "Just don't miss this time!"
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_TAVI, 0);
  Actor_SetAnimationAndWait(ACTOR_RIKI, 3);
  Event_Wait(20);
  Actor_RunRepeatedMotion(ACTOR_RIKI, 2);
  Actor_Jump(ACTOR_RIKI, 6, 80);
  // "Aww, no! You missed again!"
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_TAVI, 0);
  Actor_SetAnimationById(ACTOR_RIKI, 4);
  // "Sorry, Tavi... I can't do it!"
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_RIKI, 0);
  Actor_FaceDirection(ACTOR_TAVI, FACING_SOUTH_SOUTHEAST, 0);
  FieldActor_ShowEmote(ACTOR_TAVI, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 20);
  // "I thought I got lucky when that wave carried me up here..."
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_TAVI, 0);
  // "What are you, crazy? It knocked you stupid for a good ten minutes!"
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_RIKI, 0);
  Actor_FaceDirectionById(ACTOR_TAVI, FACING_EAST, 0);
  FieldEffect_SetAttached(ACTOR_TAVI,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(20);
  // "Yeah, yeah! Just find some way to get me down from here!"
  Event_ShowMessageAndWait(SPEAKER_WINDOW_BELOW | ACTOR_TAVI, 0);
  Actor_SetAnimationAndWait(ACTOR_RIKI, 3);
  // "Fine! Just don't jump down or do anything dumb, OK?"
  Event_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | ACTOR_RIKI, 0);
  FieldCamera_SetSpeed(3.2);
  FieldCamera_MoveTo(420, CAMERA_KEEP, 200, 1);
  Actor_SetSpeed(ACTOR_RIKI, 1.6);
  Actor_WalkToAndWait(ACTOR_RIKI, 346, 186);
  Actor_WalkToAndWait(ACTOR_RIKI, 374, 192);
  Actor_WalkToAndWait(ACTOR_RIKI, 392, 224);
  Actor_WalkToAndWait(ACTOR_RIKI, 394, 246);
  Actor_WalkToAndWait(ACTOR_RIKI, 420, 268);
  Actor_WalkToAndWait(ACTOR_RIKI, 456, 268);
  Actor_WalkToAndWait(ACTOR_RIKI, 488, 240);
  Actor_WalkToAndWait(ACTOR_RIKI, 488, 226);
  Actor_SetHorizontalPositionWithTerrain(ACTOR_RIKI, 0, 0);
  FieldCamera_SetSpeed(4.8);
  FieldCamera_MoveWithinMapBounds(toFIXED(264), -1, toFIXED(320), 1);
  FieldCamera_WaitForArrival();
  Actor_FaceDirectionById(gMainRoutineWork.leaderActorId, FACING_WEST, 0);
  Actor_FaceDirectionById(ACTOR_ROPE_ROOM_KRADEN, FACING_EAST, 20);
  Actor_SetAnimationById(gMainRoutineWork.leaderActorId, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_ROPE_ROOM_KRADEN, 3);
  Actor_SetAnimationById(ACTOR_ROPE_ROOM_KRADEN, 2);
  leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  if (leader != NULL)
    Actor_SetDestinationById(ACTOR_ROPE_ROOM_KRADEN, leader->x.part[0].integer,
                             leader->z.part[0].integer);
  Actor_WaitForMoveById(ACTOR_ROPE_ROOM_KRADEN);
  Actor_SetHorizontalPositionWithTerrain(ACTOR_ROPE_ROOM_KRADEN, 0, 0);
  Actor_SetHorizontalPositionWithTerrain(ACTOR_TAVI, 0, 0);
  GameFlag_Set(FLAG_RIKI_GAVE_UP);
  Event_End();
}

extern u32 gShrineOfTheSeaGod_BoysGoHome[];

/// Lash ties the boys' rope, Tavi climbs down and both meet the party.
void ShrineOfTheSeaGod_LashRope(void) {
  struct Actor *tavi;
  struct Actor *leader;
  struct FieldSprite *sprite;
  const void *callback;

  if (Event_GetFlag(FLAG_TAVI_RESCUED))
    return;
  Event_Start();
  Event_ResolvePendingActions(0);
  FieldCamera_GetActor()->motionFlags = 0;
  FieldCamera_MoveTo(340, CAMERA_KEEP, 176, 1);
  Actor_SetSpeed(gMainRoutineWork.leaderActorId, 0.8);
  Actor_SetSpeed(ACTOR_ROPE_ROOM_KRADEN, 0.8);
  Actor_WalkToAndWait(gMainRoutineWork.leaderActorId, 344, 184);
  Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_SOUTHEAST, 0);
  Actor_SetPosition(ACTOR_ROPE_ROOM_KRADEN, 344, 184);
  wait(1);
  Actor_WalkToAndWait(ACTOR_ROPE_ROOM_KRADEN, 356, 196);
  Actor_FaceDirectionAndWait(ACTOR_ROPE_ROOM_KRADEN, FACING_NORTH_NORTHWEST);
  Event_SetMessageId((s32)MsgFieldPraiseRopeWork);
  // "Excellent work, Felix!"
  Event_ShowMessage(ACTOR_ROPE_ROOM_KRADEN, 0);
  Actor_SetAnimationAndWait(gMainRoutineWork.leaderActorId, ANIM_NOD);
  // "Hey, the rope!"
  Event_ShowMessage(ACTOR_TAVI, 0);
  Actor_SetPosition(ACTOR_TAVI, 184, 168);
  Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_ROPE_ROOM_KRADEN, FACING_WEST, 0);
  FieldCamera_SetSpeed(4.8);
  FieldCamera_MoveTo(232, 64, 176, 1);
  Actor_SetSpeed(ACTOR_TAVI, 1.6);
  Actor_WalkToAndWait(ACTOR_TAVI, 232, 168);
  Event_Wait(20);
  // "Awesome, Riki! Way to go!"
  Event_ShowMessage(ACTOR_TAVI, 0);
  FieldCamera_SetSpeed(1.2);
  FieldCamera_MoveTo(348, CAMERA_KEEP, 216, 1);
  tavi = Actor_GetById(ACTOR_TAVI);
  sprite = tavi->sprite;
  Actor_SetSpritePriority(ACTOR_TAVI, 1);
  Actor_SetSpeed(ACTOR_TAVI, 1.6);
  Actor_Jump(ACTOR_TAVI, 4, 0);
  Actor_MoveToAndWait(ACTOR_TAVI, 248, 168);
  Actor_SetAnimationById(ACTOR_TAVI, 6);
  tavi->scaleX = -FIXED_ONE;
  sprite->rotation = TRIG_TURN * 13 / 16;
  tavi->motionFlags = 0;
  tavi->y.fixed += toFIXED(6);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_TAVI), 0);
  Actor_SetSpeed(ACTOR_TAVI, 0.4);
  FieldActor_SetMoveTarget(tavi, 328, 38, 168);
  Actor_WaitForMoveById(ACTOR_TAVI);
  tavi->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  tavi->groundHeight = toFIXED(32);
  tavi->scaleX = FIXED_ONE;
  sprite->rotation = 0;
  Actor_SetAnimationById(ACTOR_TAVI, 1);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_TAVI), SPRITE_FLAG_SHADOW);
  Actor_SetSpeed(ACTOR_TAVI, 1.6);
  Actor_Jump(ACTOR_TAVI, 4, 0);
  Actor_MoveToAndWait(ACTOR_TAVI, 328, 184);
  Actor_FaceDirection(ACTOR_TAVI, FACING_EAST, 20);
  FieldActor_ShowEmote(ACTOR_TAVI, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  Actor_SetSpritePriority(ACTOR_TAVI, 2);
  // "Who are you?"
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_TAVI, 0);
  // "We're just passing through."
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_ROPE_ROOM_KRADEN, 0);
  FieldActor_ShowEmote(ACTOR_TAVI, EMOTE_IN_FRONT | EMOTE_QUESTION, 20);
  // "How come you threw me the rope?"
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_TAVI, 0);
  Actor_FaceDirection(ACTOR_ROPE_ROOM_KRADEN, FACING_NORTH_NORTHWEST, 40);
  Actor_FaceDirection(ACTOR_ROPE_ROOM_KRADEN, FACING_WEST, 0);
  // "We'd heard that you needed some assistance."
  Event_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_ROPE_ROOM_KRADEN, 0,
                            60);
  // "Well, whatever. Thanks anyway."
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_TAVI, 0);
  Actor_SetAnimationById(ACTOR_TAVI, 3);
  Actor_SetPosition(ACTOR_RIKI, 394, 286);
  wait(1);
  Actor_SetSpeed(ACTOR_RIKI, 1.6);
  Actor_WalkToAndWait(ACTOR_RIKI, 394, 248);
  Actor_FaceDirection(ACTOR_RIKI, FACING_NORTH_NORTHWEST, 0);
  Actor_Jump(ACTOR_RIKI, 4, 20);
  // "Hey, Tavi!"
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_RIKI, 0);
  FieldActor_ShowEmote(ACTOR_TAVI, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_FaceDirection(ACTOR_TAVI, FACING_SOUTH_SOUTHEAST, 0);
  Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_ROPE_ROOM_KRADEN, FACING_SOUTH_SOUTHEAST, 0);
  FieldCamera_SetSpeed(4.8);
  FieldCamera_MoveTo(376, CAMERA_KEEP, 264, 1);
  FieldCamera_WaitForArrival();
  // "How'd ya get down from there?"
  Event_ShowMessage(ACTOR_RIKI, 0);
  FieldCamera_SetSpeed(2.4);
  FieldCamera_MoveTo(348, CAMERA_KEEP, 216, 1);
  Actor_WatchAndSetScript(gMainRoutineWork.leaderActorId, ACTOR_RIKI);
  Actor_WatchAndSetScript(ACTOR_ROPE_ROOM_KRADEN, ACTOR_RIKI);
  Actor_WatchAndSetScript(ACTOR_TAVI, ACTOR_RIKI);
  Actor_WalkToAndWait(ACTOR_RIKI, 394, 220);
  Actor_WalkToAndWait(ACTOR_RIKI, 370, 198);
  Actor_SetSpeed(ACTOR_ROPE_ROOM_KRADEN, 1.6);
  Actor_MoveToAndWait(ACTOR_ROPE_ROOM_KRADEN, 368, 180);
  Actor_WatchAndSetScript(ACTOR_ROPE_ROOM_KRADEN, ACTOR_RIKI);
  Actor_WalkToAndWait(ACTOR_RIKI, 336, 198);
  Actor_FaceDirection(ACTOR_RIKI, FACING_NORTH_NORTHWEST, 0);
  Actor_Stop(ACTOR_TAVI);
  // "These guys threw me the rope..."
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_TAVI, 0);
  Actor_FaceDirectionById(ACTOR_RIKI, FACING_NORTH_NORTHEAST, 0);
  // "Really? These grown-ups?"
  Event_ShowMessage(ACTOR_RIKI, 0);
  Actor_FaceDirectionAndWait(ACTOR_RIKI, FACING_NORTH_NORTHWEST);
  Actor_Jump(ACTOR_RIKI, 4, 20);
  // "Well, whatever. Now we can go nab that critter we saw!"
  Event_ShowMessageThenWait(ACTOR_RIKI, 0, 20);
  FieldActor_ShowEmote(ACTOR_TAVI, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
  FieldActor_ShowEmote(ACTOR_RIKI, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  // "What? What is it?"
  Event_ShowMessage(ACTOR_RIKI, 0);
  Actor_SetAnimationById(ACTOR_TAVI, 4);
  // "That little guy's so fast, I can't catch him!"
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_TAVI, 0);
  FieldActor_ShowEmote(ACTOR_RIKI, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  // "Do you think we could catch him together?"
  Event_ShowMessage(ACTOR_RIKI, 0);
  Actor_FaceDirection(ACTOR_TAVI, FACING_WEST, 20);
  // "Well, yeah, except that every time I'm about to grab him, he flies off!"
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_TAVI, 0);
  Actor_FaceDirection(ACTOR_RIKI, FACING_WEST, 0);
  // "So I guess it wouldn't matter if we worked together or not."
  Event_ShowMessage(ACTOR_RIKI, 0);
  Actor_SetAnimationAndWait(ACTOR_TAVI, 4);
  // "Yeah, I guess not."
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_TAVI, 0);
  FieldActor_ShowEmote(ACTOR_RIKI, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 40);
  // "Well, that's too bad."
  Event_ShowMessage(ACTOR_RIKI, 0);
  FieldActor_ShowEmote(ACTOR_RIKI, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  FieldActor_ShowEmote(ACTOR_TAVI, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_FaceDirection(ACTOR_RIKI, FACING_NORTH_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_TAVI, FACING_SOUTH_SOUTHEAST, 40);
  Actor_Jump(ACTOR_TAVI, 4, 40);
  // "Hey, I'm really hungry! I haven't eaten since before I got stuck!"
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_TAVI, 0);
  Actor_SetAnimationById(ACTOR_RIKI, 3);
  // "Yeah, same here... My stomach's growling up a storm!"
  Event_ShowMessage(ACTOR_RIKI, 0);
  Actor_SetAnimationAndWait(ACTOR_TAVI, 3);
  // "Let's go home, Riki."
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_TAVI, 0);
  // "Sounds good, Tavi."
  Event_ShowMessage(ACTOR_RIKI, 0);
  Actor_SetAnimationById(ACTOR_RIKI, 3);
  Actor_SetAnimationAndWait(ACTOR_TAVI, 3);
  Actor_SetSpeed(ACTOR_RIKI, 1.2);
  Actor_SetSpeed(ACTOR_TAVI, 1.2);
  callback = gShrineOfTheSeaGod_BoysGoHome;
  Actor_SetMovementTypeById(ACTOR_RIKI, callback);
  Event_Wait(10);
  Actor_SetMovementTypeAndWaitById(ACTOR_TAVI, callback);
  Event_Wait(40);
  Actor_Stop(gMainRoutineWork.leaderActorId);
  Actor_Stop(ACTOR_ROPE_ROOM_KRADEN);
  wait(1);
  // "They're gone..."
  Event_ShowMessageThenWait(ACTOR_ROPE_ROOM_KRADEN, 0, 40);
  Actor_SetPositionFX(ACTOR_TAVI, 0, 0);
  Actor_SetPositionFX(ACTOR_RIKI, 0, 0);
  // "Well, it was good of you to help them."
  Event_ShowMessage(ACTOR_ROPE_ROOM_KRADEN, 0);
  Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_ROPE_ROOM_KRADEN, FACING_WEST, 20);
  // "We should be going, too."
  Event_ShowMessage(ACTOR_ROPE_ROOM_KRADEN, 0);
  Actor_SetAnimationById(ACTOR_ROPE_ROOM_KRADEN, 2);
  leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  if (leader != NULL)
    Actor_SetDestinationById(ACTOR_ROPE_ROOM_KRADEN,
                             FIXEDtoINT(leader->x.fixed),
                             FIXEDtoINT(leader->z.fixed));
  Actor_WaitForMoveById(ACTOR_ROPE_ROOM_KRADEN);
  Actor_SetPositionFX(ACTOR_ROPE_ROOM_KRADEN, 0, 0);
  GameFlag_Set(FLAG_TAVI_RESCUED);
  Event_End();
}

/// Breath, first seen in the crossroads, startles and runs east.
void ShrineOfTheSeaGod_DjinniFleesEast(void) {
  u8 off = 0;

  Event_Start();
  Event_ResolvePendingActions(0);
  FieldCamera_GetActor()->motionFlags = off;
  FieldCamera_MoveWithinMapBounds(toFIXED(480), toFIXED(32), toFIXED(340), 1);
  FieldCamera_WaitForArrival();
  Actor_FaceActorById(ACTOR_CROSSROADS_BREATH, gMainRoutineWork.leaderActorId,
                      0);
  Actor_ShowEmoteById(ACTOR_CROSSROADS_BREATH,
                      EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  Actor_SetSpritePriority(ACTOR_CROSSROADS_BREATH, 1);
  Actor_GetById(ACTOR_CROSSROADS_BREATH)->motionFlags = off;
  Actor_SetSpeed(ACTOR_CROSSROADS_BREATH, 1.6);
  Actor_WalkToAndWait(ACTOR_CROSSROADS_BREATH, 500, 344);
  Actor_WalkToAndWait(ACTOR_CROSSROADS_BREATH, 580, 344);
  Actor_WatchAndSetScript(ACTOR_CROSSROADS_BREATH,
                          gMainRoutineWork.leaderActorId);
  Event_Wait(20);
  GameFlag_Set(FLAG_DJINNI_FLED_EAST);
  Event_End();
}

/// In the crossroads, Breath flees south out of the room.
void ShrineOfTheSeaGod_DjinniFleesSouth(void) {
  u8 off = 0;

  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_Stop(ACTOR_CROSSROADS_BREATH);
  FieldCamera_GetActor()->motionFlags = off;
  FieldCamera_MoveWithinMapBounds(toFIXED(564), toFIXED(32), toFIXED(344), 1);
  FieldCamera_WaitForArrival();
  Actor_FaceActorById(ACTOR_CROSSROADS_BREATH, gMainRoutineWork.leaderActorId,
                      0);
  Actor_ShowEmoteById(ACTOR_CROSSROADS_BREATH,
                      EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  Actor_SetSpritePriority(ACTOR_CROSSROADS_BREATH, 1);
  Actor_GetById(ACTOR_CROSSROADS_BREATH)->motionFlags = off;
  Actor_SetSpeed(ACTOR_CROSSROADS_BREATH, 1.6);
  Actor_WalkToAndWait(ACTOR_CROSSROADS_BREATH, 616, 376);
  Actor_WalkToAndWait(ACTOR_CROSSROADS_BREATH, 616, 460);
  Actor_SetHorizontalPositionWithTerrain(ACTOR_CROSSROADS_BREATH, 0, 0);
  Event_Wait(20);
  GameFlag_Set(FLAG_DJINNI_FLED_SOUTH);
  Event_End();
}

/// The Djinni runs up from the south stairs, under the bridge, and waits by the
/// ledge.
void ShrineOfTheSeaGod_DjinniRunsNorth(void) {
  struct Actor *bridge = Actor_GetById(ACTOR_LEDGE_ROOM_BRIDGE);
  s32 angle;

  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_ShowEmoteById(gMainRoutineWork.leaderActorId,
                      EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  FieldCamera_GetActor()->motionFlags = 0;
  FieldCamera_MoveTo(328, 32, 552, 1);
  FieldCamera_WaitForArrival();
  Actor_SetPositionAndFacing(ACTOR_BREATH, toFIXED(328), toFIXED(624),
                             FACING_NORTH);
  wait(1);
  Actor_WatchAndSetScript(gMainRoutineWork.leaderActorId, ACTOR_BREATH);
  bridge->priorityFlags &= ~ACTOR_PRIORITY_UNDERFOOT;
  Actor_SetSpritePriority(ACTOR_BREATH, 3);
  Actor_SetSpeed(ACTOR_BREATH, 1.6);
  Actor_WalkToAndWait(ACTOR_BREATH, 328, 568);
  Actor_WalkToAndWait(ACTOR_BREATH, 328, 496);
  Actor_WalkToAndWaitById(ACTOR_BREATH, 280, 432);
  Actor_Stop(gMainRoutineWork.leaderActorId);
  wait(1);
  angle = FACING_SOUTH;
  Actor_SetPositionAndFacing(ACTOR_BREATH, toFIXED(280), toFIXED(328), angle);
  Actor_SetSpritePriority(ACTOR_BREATH, 2);
  Actor_GetById(ACTOR_BREATH)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
  bridge->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
  wait(1);
  GameFlag_Set(FLAG_DJINNI_RAN_PAST);
  GameFlag_Set(FLAG_DJINNI_BY_LEDGE);
  Event_End();
}

/// In the ledge room, Breath runs west past the leader.
void ShrineOfTheSeaGod_DjinniRunsWest(void) {
  s32 done = Event_GetFlag(FLAG_DJINNI_RAN_PAST);

  if (done == 0) {
    Event_Start();
    Event_ResolvePendingActions(0);
    Actor_ShowEmoteById(gMainRoutineWork.leaderActorId,
                        EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
    FieldCamera_GetActor()->motionFlags = done;
    FieldCamera_MoveWithinMapBounds(toFIXED(328), toFIXED(48), toFIXED(552), 1);
    FieldCamera_WaitForArrival();
    done = FACING_SOUTH;
    Actor_SetPositionAndFacing(ACTOR_BREATH, toFIXED(424), toFIXED(520), done);
    wait(1);
    Actor_WatchAndSetScript(gMainRoutineWork.leaderActorId, ACTOR_BREATH);
    Actor_SetSpritePriority(ACTOR_BREATH, 1);
    Actor_SetSpeed(ACTOR_BREATH, 1.6);
    Actor_WalkToAndWait(ACTOR_BREATH, 380, 552);
    Actor_WalkToAndWait(ACTOR_BREATH, 264, 552);
    Actor_WalkToAndWaitById(ACTOR_BREATH, 264, 540);
    Actor_Stop(gMainRoutineWork.leaderActorId);
    wait(1);
    Actor_SetHorizontalPositionWithTerrain(ACTOR_BREATH, 0, 0);
    Actor_SetSpritePriority(ACTOR_BREATH, 2);
    Actor_GetById(ACTOR_BREATH)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
    wait(1);
    GameFlag_Set(FLAG_DJINNI_RAN_PAST);
    Event_End();
  }
}

/// Breath hops up onto the ledge, where the chase round it begins.
void ShrineOfTheSeaGod_DjinniHopsOntoLedge(void) {
  u8 off;

  if (GameFlag_Get(FLAG_DJINNI_BY_LEDGE)) {
    Event_Start();
    Event_ResolvePendingActions(0);
    off = 0;
    FieldCamera_GetActor()->motionFlags = off;
    FieldCamera_MoveWithinMapBounds(toFIXED(320), -1, toFIXED(300), 1);
    FieldActor_ShowEmote(ACTOR_BREATH, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
    Actor_SetSpeed(ACTOR_BREATH, 1.2);
    Actor_WalkToAndWait(ACTOR_BREATH, 345, 296);
    Actor_WalkToAndWait(ACTOR_BREATH, 392, 296);
    Actor_WatchAndSetScript(ACTOR_BREATH, gMainRoutineWork.leaderActorId);
    Actor_GetById(ACTOR_BREATH)->riseCounter = off;
    wait(10);
    GameFlag_Set(FLAG_DJINNI_ON_LEDGE);
    Event_End();
  }
}

/// Starts one of Breath's dashes: the camera and Breath stop, and Breath picks
/// up speed.
void ShrineOfTheSeaGod_StartDjinniDash(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  FieldCamera_GetActor()->motionFlags = 0;
  Actor_Stop(ACTOR_BREATH);
  wait(1);
  Actor_SetSpeed(ACTOR_BREATH, 2.4);
}

/// Ends one of Breath's dashes: Breath watches the leader again.
void ShrineOfTheSeaGod_EndDjinniDash(void) {
  Actor_WatchAndSetScript(ACTOR_BREATH, gMainRoutineWork.leaderActorId);
  Event_Wait(5);
  Event_End();
}

/// A dash of Breath's along the ledge to (toX, toZ), where it stands at place.
#define DEFINE_DJINNI_DASH(name, toX, toZ, place)                              \
  void name(void) {                                                            \
    ShrineOfTheSeaGod_StartDjinniDash();                                       \
    FieldCamera_MoveWithinMapBounds(toFIXED(toX), toFIXED(16), toFIXED(toZ),   \
                                    1);                                        \
    Actor_WalkToAndWaitById(ACTOR_BREATH, toX, toZ);                           \
    Actor_GetById(ACTOR_BREATH)->riseCounter = place;                          \
    ShrineOfTheSeaGod_EndDjinniDash();                                         \
  }

DEFINE_DJINNI_DASH(ShrineOfTheSeaGod_DjinniSouthEastToNorthEast, 392, 216,
                   BREATH_AT_NORTHEAST_CORNER)

/// Breath runs along the ledge's north side to stopX, beside the torch's place
/// at the north wall. With the torch there, it turns to face it and stops
/// (stoppedPlace), trapped; otherwise it leaps over to landX and runs on to the
/// corner at cornerX (cornerPlace).
#define DEFINE_DJINNI_WALK_AND_JUMP(name, stopX, turnTo, stoppedPlace,         \
                                    cornerX, landX, cornerPlace)               \
  void name(void) {                                                            \
    struct Actor *djinni = Actor_GetById(ACTOR_BREATH);                        \
    u8 *place;                                                                 \
    s32 flag;                                                                  \
                                                                               \
    ShrineOfTheSeaGod_StartDjinniDash();                                       \
    FieldCamera_MoveToFX(toFIXED(stopX), toFIXED(16), toFIXED(216), 1);        \
    Actor_WalkToAndWait(ACTOR_BREATH, stopX, 216);                             \
    flag = GameFlag_Get(FLAG_TORCH_AT_NORTH_WALL);                             \
    place = &djinni->riseCounter;                                              \
    if (flag) {                                                                \
      djinni->facing = turnTo;                                                 \
      Actor_SetAttachedEffect(ACTOR_BREATH, ATTACHED_EFFECT_IN_FRONT |         \
                                                ATTACHED_EFFECT_ONCE);         \
      Event_Wait(20);                                                          \
      *place = stoppedPlace;                                                   \
      GameFlag_Set(FLAG_DJINNI_TRAPPED);                                       \
    } else {                                                                   \
      Event_Wait(5);                                                           \
      FieldCamera_MoveWithinMapBounds(toFIXED(cornerX), toFIXED(16),           \
                                      toFIXED(216), 1);                        \
      Sound_Play(SE_HIGH_JUMP);                                                \
      Actor_SetSpriteFlags(Actor_GetById(ACTOR_BREATH), 0);                    \
      Actor_SetSpeed(ACTOR_BREATH, 4.8);                                       \
      djinni->velocityY = toFIXED(8);                                          \
      Actor_MoveToAndWaitById(ACTOR_BREATH, landX, 216);                       \
      Actor_SetSpriteFlags(Actor_GetById(ACTOR_BREATH), SPRITE_FLAG_SHADOW);   \
      Event_Wait(10);                                                          \
      Actor_SetSpeed(ACTOR_BREATH, 1.6);                                       \
      Actor_WalkToAndWait(ACTOR_BREATH, cornerX, 216);                         \
      *place = cornerPlace;                                                    \
    }                                                                          \
    ShrineOfTheSeaGod_EndDjinniDash();                                         \
  }

DEFINE_DJINNI_WALK_AND_JUMP(ShrineOfTheSeaGod_DjinniNorthEastJumpsWest, 360,
                            FACING_WEST, BREATH_STOPPED_EAST_OF_TORCH, 296, 328,
                            BREATH_AT_NORTHWEST_CORNER)

/// A dash of Breath's from the corner at fromX round the ledge's side, by its
/// middle row, to (toX, toZ), where it stands at place.
#define DEFINE_DJINNI_DASH_AROUND(name, toX, toZ, fromX, place)                \
  void name(void) {                                                            \
    ShrineOfTheSeaGod_StartDjinniDash();                                       \
    FieldCamera_MoveWithinMapBounds(toFIXED(toX), toFIXED(16), toFIXED(toZ),   \
                                    1);                                        \
    Actor_WalkToAndWait(ACTOR_BREATH, fromX, 248);                             \
    Actor_WalkToAndWait(ACTOR_BREATH, toX, 248);                               \
    Actor_WalkToAndWaitById(ACTOR_BREATH, toX, toZ);                           \
    Actor_GetById(ACTOR_BREATH)->riseCounter = place;                          \
    ShrineOfTheSeaGod_EndDjinniDash();                                         \
  }

DEFINE_DJINNI_DASH_AROUND(ShrineOfTheSeaGod_DjinniNorthWestToSouthWest, 280,
                          296, 296, BREATH_AT_SOUTHWEST_CORNER)

DEFINE_DJINNI_DASH(ShrineOfTheSeaGod_DjinniSouthWestToSouthEast, 392, 296,
                   BREATH_AT_SOUTHEAST_CORNER)

DEFINE_DJINNI_DASH(ShrineOfTheSeaGod_DjinniSouthEastToSouthWest, 280, 296,
                   BREATH_AT_SOUTHWEST_CORNER)

DEFINE_DJINNI_DASH_AROUND(ShrineOfTheSeaGod_DjinniSouthWestToNorthWest, 296,
                          216, 280, BREATH_AT_NORTHWEST_CORNER)

DEFINE_DJINNI_WALK_AND_JUMP(ShrineOfTheSeaGod_DjinniNorthWestJumpsEast, 328,
                            FACING_EAST, BREATH_STOPPED_WEST_OF_TORCH, 392, 360,
                            BREATH_AT_NORTHEAST_CORNER)

DEFINE_DJINNI_DASH(ShrineOfTheSeaGod_DjinniNorthEastToSouthEast, 392, 296,
                   BREATH_AT_SOUTHEAST_CORNER)

/// As DEFINE_DJINNI_WALK_AND_JUMP for Breath already beside the torch's place.
#define DEFINE_DJINNI_JUMP(name, turnTo, stoppedPlace, cornerX, landX,         \
                           cornerPlace)                                        \
  void name(void) {                                                            \
    struct Actor *djinni = Actor_GetById(ACTOR_BREATH);                        \
    u8 *place;                                                                 \
    s32 flag;                                                                  \
                                                                               \
    ShrineOfTheSeaGod_StartDjinniDash();                                       \
    flag = GameFlag_Get(FLAG_TORCH_AT_NORTH_WALL);                             \
    place = &djinni->riseCounter;                                              \
    if (flag) {                                                                \
      djinni->facing = turnTo;                                                 \
      Actor_SetAttachedEffect(ACTOR_BREATH, ATTACHED_EFFECT_IN_FRONT |         \
                                                ATTACHED_EFFECT_ONCE);         \
      Event_Wait(20);                                                          \
      *place = stoppedPlace;                                                   \
      GameFlag_Set(FLAG_DJINNI_TRAPPED);                                       \
    } else {                                                                   \
      FieldCamera_MoveWithinMapBounds(toFIXED(cornerX), toFIXED(16),           \
                                      toFIXED(216), 1);                        \
      Sound_Play(SE_HIGH_JUMP);                                                \
      Actor_SetSpriteFlags(Actor_GetById(ACTOR_BREATH), 0);                    \
      Actor_SetSpeed(ACTOR_BREATH, 4.8);                                       \
      djinni->velocityY = toFIXED(8);                                          \
      Actor_MoveToAndWaitById(ACTOR_BREATH, landX, 216);                       \
      Actor_SetSpriteFlags(Actor_GetById(ACTOR_BREATH), SPRITE_FLAG_SHADOW);   \
      Event_Wait(10);                                                          \
      Actor_SetSpeed(ACTOR_BREATH, 1.6);                                       \
      Actor_WalkToAndWait(ACTOR_BREATH, cornerX, 216);                         \
      *place = cornerPlace;                                                    \
    }                                                                          \
    ShrineOfTheSeaGod_EndDjinniDash();                                         \
  }

DEFINE_DJINNI_JUMP(ShrineOfTheSeaGod_TrappedDjinniJumpsWest, FACING_WEST,
                   BREATH_STOPPED_EAST_OF_TORCH, 296, 328,
                   BREATH_AT_NORTHWEST_CORNER)

DEFINE_DJINNI_DASH(ShrineOfTheSeaGod_TrappedDjinniToNorthEast, 392, 216,
                   BREATH_AT_NORTHEAST_CORNER)

DEFINE_DJINNI_DASH(ShrineOfTheSeaGod_TrappedDjinniToNorthWest, 296, 216,
                   BREATH_AT_NORTHWEST_CORNER)

DEFINE_DJINNI_JUMP(ShrineOfTheSeaGod_TrappedDjinniJumpsEast, FACING_EAST,
                   BREATH_STOPPED_WEST_OF_TORCH, 392, 360,
                   BREATH_AT_NORTHEAST_CORNER)

/// Each step onto or round the ledge sends Breath away from the leader to
/// another corner, leaping the north side where the torch's place is, unless
/// the torch stands there and stops it.
void ShrineOfTheSeaGod_ChaseDjinni(void) {
  struct FieldWork *work;
  struct Actor *actor;

  if (!GameFlag_Get(FLAG_DJINNI_ON_LEDGE))
    return;
  if (GameFlag_Get(FLAG_DJINNI(DJINNI_BREATH)))
    return;
  work = Ram_HeapSlots->fieldWork;
  actor = Actor_GetById(ACTOR_BREATH);
  switch (work->stepTrigger) {
  case STEP_LEDGE_SOUTHEAST_WAY_UP:
    if (actor->riseCounter == BREATH_AT_SOUTHEAST_CORNER)
      ShrineOfTheSeaGod_DjinniSouthEastToNorthEast();
    GameFlag_Clear(FLAG_DJINNI_TRAPPED);
    break;
  case STEP_LEDGE_EAST_SIDE_SOUTH:
    if (actor->riseCounter == BREATH_AT_SOUTHEAST_CORNER)
      ShrineOfTheSeaGod_DjinniSouthEastToSouthWest();
    break;
  case STEP_LEDGE_EAST_SIDE_NORTH: {
    u8 current = actor->riseCounter;

    if (current == BREATH_AT_NORTHEAST_CORNER)
      ShrineOfTheSeaGod_DjinniNorthEastJumpsWest();
    else if (current == BREATH_STOPPED_EAST_OF_TORCH &&
             !GameFlag_Get(FLAG_DJINNI_TRAPPED))
      ShrineOfTheSeaGod_TrappedDjinniJumpsWest();
  } break;
  case STEP_LEDGE_NORTH_SIDE_EAST: {
    u8 current = actor->riseCounter;

    if (current == BREATH_AT_NORTHEAST_CORNER)
      ShrineOfTheSeaGod_DjinniNorthEastToSouthEast();
    else if (current == BREATH_STOPPED_WEST_OF_TORCH)
      ShrineOfTheSeaGod_TrappedDjinniToNorthWest();
  } break;
  case STEP_LEDGE_NORTH_SIDE_WEST: {
    u8 current = actor->riseCounter;

    if (current == BREATH_AT_NORTHWEST_CORNER)
      ShrineOfTheSeaGod_DjinniNorthWestToSouthWest();
    else if (current == BREATH_STOPPED_EAST_OF_TORCH)
      ShrineOfTheSeaGod_TrappedDjinniToNorthEast();
  } break;
  case STEP_LEDGE_WEST_SIDE_NORTH: {
    u8 current = actor->riseCounter;

    if (current == BREATH_AT_NORTHWEST_CORNER)
      ShrineOfTheSeaGod_DjinniNorthWestJumpsEast();
    else if (current == BREATH_STOPPED_WEST_OF_TORCH &&
             !GameFlag_Get(FLAG_DJINNI_TRAPPED))
      ShrineOfTheSeaGod_TrappedDjinniJumpsEast();
  } break;
  case STEP_LEDGE_WEST_SIDE_SOUTH:
    if (actor->riseCounter == BREATH_AT_SOUTHWEST_CORNER)
      ShrineOfTheSeaGod_DjinniSouthWestToSouthEast();
    break;
  case STEP_LEDGE_SOUTHWEST_WAY_UP:
    if (actor->riseCounter == BREATH_AT_SOUTHWEST_CORNER)
      ShrineOfTheSeaGod_DjinniSouthWestToNorthWest();
    GameFlag_Clear(FLAG_DJINNI_TRAPPED);
    break;
  }
}

extern struct ShrineOfTheSeaGodSpot gShrineOfTheSeaGod_Footprints[];

void Rope_Place(s32 rope, s32 flag, s32 post, s32 farPost);
void Actor_PlaceBlocks(const void *script);

/// Lights the torch: the pedestal in its burning animation, and the marker on
/// its flame in its empty picture, disabled.
void ShrineOfTheSeaGod_LightTorch(void) {
  Actor_SetAnimationById(ACTOR_TORCH, 2);
  Actor_SetAnimationById(ACTOR_TORCH_FLAME_MARKER, 0);
  Actor_DisableById(ACTOR_TORCH_FLAME_MARKER);
}

/// Casting Sand in the rope room turns Felix to sand as anywhere, and here also
/// moves him onto the map's overhead layer. That layer's cells here (behaviour
/// 232, ringed by cells 16 pixels down) mark out a winding way under the ridge
/// of rock round the south ladder, cells (13, 21) to (25, 23) and (21, 18) to
/// (20, 27), reaching the floor west of the ridge, the corridor south of it
/// and the ledge east of it. Cast on the floor west of the ridge, at cell
/// (17, 21), Felix sinks into a mound of sand; steered east, then north, then
/// east, the mound slides in under the ridge's rocky rim, comes out on its
/// east side and goes on east along the floor to the room's east wall below
/// the east ladder, still sand. Steered north off the way onto the floor by
/// the water, he rises out of the sand. Cast off the way, as by the south
/// ladder, he sinks and rises again in place a second later, whatever key is
/// held (FieldPsynergy_Sand).
void ShrineOfTheSeaGod_CastSand(void) {
  Actor_GetById(gMainRoutineWork.leaderActorId)->layer = 2;
}

/// As Sand wears off, Felix rises out of the sand and walks on the ground layer
/// again.
void ShrineOfTheSeaGod_EndSand(void) {
  Actor_GetById(gMainRoutineWork.leaderActorId)->layer = 0;
}

/// Sets up the room being entered.
s32 Scene_Initialize(void) {
  s16 scene;

  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
  scene = gMainRoutineWork.next;
  if (scene == SCENE_SHRINE_OF_THE_SEA_GOD_ROPE)
    ShrineOfTheSeaGod_InitializeRope();
  else if (scene == SCENE_SHRINE_OF_THE_SEA_GOD_PLANK_BRIDGE)
    ShrineOfTheSeaGod_InitializePlankBridge();
  else if (scene == SCENE_SHRINE_OF_THE_SEA_GOD_CROSSROADS)
    ShrineOfTheSeaGod_InitializeCrossroads();
  else if (scene == SCENE_SHRINE_OF_THE_SEA_GOD_FOOTPRINTS)
    ShrineOfTheSeaGod_InitializeFootprints();
  else if (scene == SCENE_SHRINE_OF_THE_SEA_GOD_BREATH)
    ShrineOfTheSeaGod_InitializeBreath();
  else if (scene == SCENE_SHRINE_OF_THE_SEA_GOD_PILLAR_HOLE)
    ShrineOfTheSeaGod_InitializePillarHole();
  else if (scene == SCENE_SHRINE_OF_THE_SEA_GOD_PILLARS)
    ShrineOfTheSeaGod_InitializePillars();
  else if (scene == SCENE_SHRINE_OF_THE_SEA_GOD_SEA_GODS_TEAR)
    ShrineOfTheSeaGod_InitializeSeaGodsTear();
  else if (scene == SCENE_SHRINE_OF_THE_SEA_GOD_STATUE)
    ShrineOfTheSeaGod_InitializeStatue();
  else if (scene == SCENE_SHRINE_OF_THE_SEA_GOD_RIGHT_PRONG)
    ShrineOfTheSeaGod_InitializeRightProng();
  return 0;
}

/// Sets up a bridge: twelve pixels up, drawn under the leader, disabled.
struct Actor *ShrineOfTheSeaGod_SetUpBridge(s32 id) {
  struct Actor *actor = Actor_GetById(id);

  actor->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
  actor->y.fixed = toFIXED(12);
  Actor_DisableById(id);
  return actor;
}

#include "shared/actor_hide.h"

/// Back in the hole room with the party kept where it was, stops drawing its
/// unseen blocks unless they were drawn; on a fresh arrival, notes them drawn.
void Scene_RunUnlessFlag(void) {
  if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION)) {
    if (GameFlag_Get(FLAG_BLOCKS_SHOWN) == 0)
      ShrineOfTheSeaGod_HideBlocks();
  } else {
    GameFlag_Set(FLAG_BLOCKS_SHOWN);
  }
}

/// Hides the hole room's three unseen blocks behind the room.
void ShrineOfTheSeaGod_SetUpBlocks(void) {
  u32 i;

  for (i = 0; i <= ACTOR_HOLE_ROOM_THIRD_BLOCK - ACTOR_HOLE_ROOM_FIRST_BLOCK;
       i++)
    Actor_Hide(ACTOR_HOLE_ROOM_FIRST_BLOCK + i);
  Scene_RunUnlessFlag();
}

/// Lays the Djinni's footprints across the room below the crossroads, drawn
/// dark on the sand.
void ShrineOfTheSeaGod_PlaceFootprints(void) {
  const struct ShrineOfTheSeaGodSpot *spot = gShrineOfTheSeaGod_Footprints;
  u32 i;

  for (i = 0; i <= ACTOR_LAST_FOOTPRINT - ACTOR_FIRST_FOOTPRINT; i++) {
    s32 id = i + ACTOR_FIRST_FOOTPRINT;
    struct Actor *actor = Actor_GetById(id);
    struct FieldSprite *sprite;

    Actor_DisableById(id);
    Actor_SetAnimationById(id, 0);
    Actor_SetSpritePriority(id, 3);
    actor->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
    Actor_SetSpriteColor(Actor_GetById(id), SPRITE_COLOR_DARK);
    sprite = actor->sprite;
    actor->x.fixed = spot->x;
    actor->z.fixed = spot->z;
    actor->scaleX = spot->scaleX;
    sprite->rotation = spot->rotation;
    spot++;
  }
}

/// Sets up the rope room: its floating logs, the rope, and Riki and Tavi gone
/// once Riki has given up.
void ShrineOfTheSeaGod_InitializeRope(void) {
  struct MapWork *map = Ram_HeapSlots->mapGrid;

  ShrineOfTheSeaGod_SetUpFloatingLog(ACTOR_ROPE_ROOM_FIRST_LOG);
  ShrineOfTheSeaGod_SetUpFloatingLog(ACTOR_ROPE_ROOM_SECOND_LOG);
  Map_SetUpperLevelWading(map);
  FieldTask_AddFrame(ShrineOfTheSeaGod_TrackUpperLevel, TASK_PASS_FRAME);
  Rope_Initialize();
  Rope_Place(0, FLAG_SHRINE_ROPE_TIED, ACTOR_ROPE_ROOM_EAST_POST,
             ACTOR_ROPE_ROOM_WEST_POST);
  if (GameFlag_Get(FLAG_RIKI_GAVE_UP)) {
    Actor_SetHorizontalPositionWithTerrain(ACTOR_RIKI, 0, 0);
    Actor_SetHorizontalPositionWithTerrain(ACTOR_TAVI, 0, 0);
  }
}

// In renders of the room once Riki has given up on the rope, A pressed beside
// him from the west opens the field menu, and from the east, behind him, he
// says "Wah! You scared me!".
/// The side room's task: Riki, looking west, can be talked to and read only
/// from behind; while he stands east of the leader he is no target.
void ShrineOfTheSeaGod_UpdateRikiTarget(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  struct Actor *riki = Actor_GetById(ACTOR_SIDE_ROOM_RIKI);

  if (riki->x.fixed > leader->x.fixed)
    riki->collisionFlags |= ACTOR_NOT_TARGETED;
  else
    riki->collisionFlags &= ~ACTOR_NOT_TARGETED;
}

/// Sets up the side room: the plank bridge down over the stream, the torch, and
/// Riki looking around once he has given up on the rope.
void ShrineOfTheSeaGod_InitializePlankBridge(void) {
  ShrineOfTheSeaGod_SetUpBridge(ACTOR_SIDE_ROOM_BRIDGE)->y.fixed = -toFIXED(16);
  Actor_SetSpritePriority(ACTOR_SIDE_ROOM_BRIDGE, 3);
  Actor_PlaceBlocks(gShrineOfTheSeaGod_PushedActors);
  ShrineOfTheSeaGod_LightTorch();
  FieldTask_AddFrame(ShrineOfTheSeaGod_KeepFlameOnTorch, TASK_PASS_FRAME);
  Actor_DisableById(ACTOR_SIDE_ROOM_RIKI);
  // UB: the room places no ACTOR_SIDE_ROOM_DISABLED, so Actor_GetById returns
  // NULL and these stores go through it.
#ifndef UBFIX
  Actor_DisableById(ACTOR_SIDE_ROOM_DISABLED);
  Actor_GetById(ACTOR_SIDE_ROOM_DISABLED)->motionFlags = 0;
#endif
  MapGrid_CopyCellAttributes(3, 15, 1, 1, 6, 15);
  if (GameFlag_Get(FLAG_TAVI_RESCUED)) {
    Actor_SetHorizontalPositionWithTerrain(ACTOR_SIDE_ROOM_RIKI, 0, 0);
  } else if (GameFlag_Get(FLAG_RIKI_GAVE_UP)) {
    Actor_SetPositionAndFacing(ACTOR_SIDE_ROOM_RIKI, toFIXED(328), toFIXED(200),
                               FACING_WEST);
    wait(1);
    Actor_SetMovementTypeById(ACTOR_SIDE_ROOM_RIKI,
                              gShrineOfTheSeaGod_RikiLooksAround);
    Actor_SetSpriteFlags(Actor_GetById(ACTOR_SIDE_ROOM_RIKI),
                         SPRITE_FLAG_SHADOW);
    Actor_GetById(ACTOR_SIDE_ROOM_RIKI)->collisionFlags = ACTOR_SOLID;
  }
  FieldTask_AddFrame(ShrineOfTheSeaGod_UpdateRikiTarget, TASK_PASS_FRAME);
}

/// Sets up the crossroads: its bridges, the broken one's middle down in the
/// water once it has fallen, the stone pillar where Move left it, and Breath
/// where it fled.
void ShrineOfTheSeaGod_InitializeCrossroads(void) {
  struct Actor *middle;

  ShrineOfTheSeaGod_SetUpBridge(ACTOR_CROSSROADS_LOG_BRIDGE);
  ShrineOfTheSeaGod_SetUpBridge(ACTOR_CROSSROADS_FIRST_BRIDGE);
  ShrineOfTheSeaGod_SetUpBridge(ACTOR_CROSSROADS_SECOND_BRIDGE);
  ShrineOfTheSeaGod_SetUpBridge(ACTOR_CROSSROADS_THIRD_BRIDGE);
  ShrineOfTheSeaGod_SetUpBridge(ACTOR_CROSSROADS_BROKEN_BRIDGE_ENDS);
  middle = ShrineOfTheSeaGod_SetUpBridge(ACTOR_CROSSROADS_BROKEN_BRIDGE_MIDDLE);
  Actor_SetAnimationById(ACTOR_CROSSROADS_LOG_BRIDGE, 0);
  Actor_SetAnimationById(ACTOR_CROSSROADS_BROKEN_BRIDGE_ENDS, 0);
  Actor_SetAnimationById(ACTOR_CROSSROADS_BROKEN_BRIDGE_MIDDLE, 2);
  Actor_SetSpritePriority(ACTOR_CROSSROADS_LOG_BRIDGE, 1);
  Actor_SetSpritePriority(ACTOR_CROSSROADS_FIRST_BRIDGE, 1);
  Actor_SetSpritePriority(ACTOR_CROSSROADS_SECOND_BRIDGE, 1);
  Actor_SetSpritePriority(ACTOR_CROSSROADS_THIRD_BRIDGE, 1);
  Actor_SetSpritePriority(ACTOR_CROSSROADS_BROKEN_BRIDGE_ENDS, 1);
  Actor_SetSpritePriority(ACTOR_CROSSROADS_BROKEN_BRIDGE_MIDDLE, 1);
  FieldTask_AddFrame(ShrineOfTheSeaGod_LayerCrossroadsBridges, TASK_PASS_FRAME);
  if (GameFlag_Get(FLAG_CROSSROADS_BRIDGE_BROKEN)) {
    FieldActor_SetPositionAndResetMotion(middle, toFIXED(408), -toFIXED(13),
                                         toFIXED(336));
    ((struct FieldSprite *)middle->sprite)->rotation = TRIG_TURN / 16;
    Actor_SetSpritePriority(ACTOR_CROSSROADS_BROKEN_BRIDGE_MIDDLE, 3);
    middle->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
    MapGrid_CopyCellAttributes(0, 50, 3, 1, 24, 70);
  }
  if (GameFlag_Get(FLAG_CROSSROADS_PILLAR_MOVED))
    FieldActor_SetPositionAndResetMotion(Actor_GetById(ACTOR_CROSSROADS_PILLAR),
                                         toFIXED(200), 0, toFIXED(296));
  Actor_PlaceBlocks(gShrineOfTheSeaGod_PushedActors);
  if (GameFlag_Get(FLAG_DJINNI_FLED_SOUTH))
    Actor_SetHorizontalPositionWithTerrain(ACTOR_CROSSROADS_BREATH, 0, 0);
  else if (GameFlag_Get(FLAG_DJINNI_FLED_EAST)) {
    Actor_SetPositionAndFacing(ACTOR_CROSSROADS_BREATH, toFIXED(580),
                               toFIXED(344), FACING_SOUTH);
  }
  FieldEffect_StartBlankOverlay(0);
  wait(1);
}

/// Sets up the room below: Breath's footprints until it has run past.
void ShrineOfTheSeaGod_InitializeFootprints(void) {
  struct MapWork *map = Ram_HeapSlots->mapGrid;

  if (GameFlag_Get(FLAG_DJINNI_RAN_PAST) == 0)
    ShrineOfTheSeaGod_PlaceFootprints();
  Map_SetUpperLevelWading(map);
  FieldTask_AddFrame(ShrineOfTheSeaGod_TrackUpperLevel, TASK_PASS_FRAME);
}

/// Sets up the ledge room: the bridge, under the leader when he comes up the
/// north-east or west stairs and over him otherwise, the puddles, frozen or
/// not, the torch, and Breath where the chase left it.
void ShrineOfTheSeaGod_InitializeBreath(void) {
  struct Actor *bridge = ShrineOfTheSeaGod_SetUpBridge(ACTOR_LEDGE_ROOM_BRIDGE);

  if ((u16)(gMainRoutineWork.entrance -
            SHRINE_OF_THE_SEA_GOD_BREATH_FROM_SHRINE_OF_THE_SEA_GOD_FOOTPRINTS_NORTHEAST) <=
      SHRINE_OF_THE_SEA_GOD_BREATH_FROM_SHRINE_OF_THE_SEA_GOD_FOOTPRINTS_WEST -
          SHRINE_OF_THE_SEA_GOD_BREATH_FROM_SHRINE_OF_THE_SEA_GOD_FOOTPRINTS_NORTHEAST) {
    Actor_SetSpritePriority(ACTOR_LEDGE_ROOM_BRIDGE, 3);
    bridge->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
  } else {
    Actor_SetSpritePriority(ACTOR_LEDGE_ROOM_BRIDGE, 1);
    bridge->priorityFlags &= ~ACTOR_PRIORITY_UNDERFOOT;
  }
  ShrineOfTheSeaGod_SetUpPuddle(ACTOR_FIRST_PUDDLE);
  ShrineOfTheSeaGod_SetUpPuddle(ACTOR_SECOND_PUDDLE);
  ShrineOfTheSeaGod_SetUpPuddle(ACTOR_THIRD_PUDDLE);
  if (GameFlag_Get(FLAG_SHRINE_FIRST_PUDDLE_FROZEN))
    MapGrid_CopyCellAttributes(0, 0, 1, 1, 8, 18);
  if (GameFlag_Get(FLAG_SHRINE_SECOND_PUDDLE_FROZEN))
    MapGrid_CopyCellAttributes(0, 0, 1, 1, 10, 18);
  if (GameFlag_Get(FLAG_SHRINE_THIRD_PUDDLE_FROZEN))
    MapGrid_CopyCellAttributes(0, 0, 1, 1, 10, 17);
  FieldTask_AddFrame(ShrineOfTheSeaGod_LayerPuddles, TASK_PASS_FRAME);
  Actor_PlaceBlocks(gShrineOfTheSeaGod_PushedActors);
  ShrineOfTheSeaGod_LightTorch();
  FieldTask_AddFrame(ShrineOfTheSeaGod_KeepFlameOnTorch, TASK_PASS_FRAME);
  if (GameFlag_Get(FLAG_DJINNI(DJINNI_BREATH)) ||
      GameFlag_Get(FLAG_FIGHTING_BREATH)) {
    GameFlag_Set(FLAG_DJINNI_CHASE_OVER);
  } else if (GameFlag_Get(FLAG_DJINNI_ON_LEDGE)) {
    struct Actor *djinni = Actor_GetById(ACTOR_BREATH);
    s32 moved = GameFlag_Get(FLAG_KEEP_PARTY_POSITION);

    // A fresh arrival finds Breath back at the south-east corner.
    if (moved == 0) {
      djinni->riseCounter = moved;
      Actor_SetPosition(ACTOR_BREATH, 392, 296);
    }
    Actor_WatchAndSetScript(ACTOR_BREATH, gMainRoutineWork.leaderActorId);
  } else if (GameFlag_Get(FLAG_DJINNI_BY_LEDGE)) {
    Actor_SetPositionAndFacing(ACTOR_BREATH, toFIXED(280), toFIXED(328),
                               FACING_SOUTH);
  }
}

/// Sets up the hole room: the pillar, in its hole once pushed there, and the
/// unseen blocks hidden; the leader stays in front of rising ground.
void ShrineOfTheSeaGod_InitializePillarHole(void) {
  s32 open;

  Actor_GetById(gMainRoutineWork.leaderActorId)->priorityFlags |=
      ACTOR_PRIORITY_IGNORES_RISES;
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_HOLE_ROOM_PILLAR), 0);
  open = GameFlag_Get(FLAG_PILLAR_IN_HOLE);
  if (open) {
    Actor_SetPosition(ACTOR_HOLE_ROOM_PILLAR, 248, 264);
    Actor_SetSpritePriority(ACTOR_HOLE_ROOM_PILLAR, 3);
    Actor_GetById(ACTOR_HOLE_ROOM_PILLAR)->motionFlags = 0;
    MapGrid_CopyCellAttributes(0, 0, 1, 1, 15, 16);
    Actor_DisableById(ACTOR_HOLE_ROOM_PILLAR);
  } else {
    Actor_SetSpritePriority(ACTOR_HOLE_ROOM_PILLAR, 1);
    Actor_GetById(ACTOR_HOLE_ROOM_PILLAR)->motionFlags = open;
  }
  ShrineOfTheSeaGod_SetUpBlocks();
}

/// Sets up the pillar room: its bridges, the broken ones' middles down in the
/// stream once fallen, the room's two ropes, and the splashes, out of sight.
void ShrineOfTheSeaGod_InitializePillars(void) {
  struct Actor *firstSplash;
  struct Actor *secondSplash;

  Map_SetUpperLevelWading((struct MapWork *)Ram_HeapSlots->mapGrid);
  ShrineOfTheSeaGod_SetUpBridge(ACTOR_PILLAR_ROOM_FIRST_BRIDGE);
  ShrineOfTheSeaGod_SetUpBridge(ACTOR_PILLAR_ROOM_SECOND_BRIDGE);
  ShrineOfTheSeaGod_SetUpBridge(ACTOR_PILLAR_ROOM_THIRD_BRIDGE);
  ShrineOfTheSeaGod_SetUpBridge(ACTOR_PILLAR_ROOM_FOURTH_BRIDGE);
  ShrineOfTheSeaGod_SetUpBridge(ACTOR_PILLAR_ROOM_LOG_BRIDGE);
  ShrineOfTheSeaGod_SetUpBridge(ACTOR_FIRST_BROKEN_BRIDGE_ENDS);
  ShrineOfTheSeaGod_SetUpBridge(ACTOR_SECOND_BROKEN_BRIDGE_ENDS);
  ShrineOfTheSeaGod_SetUpBridge(ACTOR_FIRST_BROKEN_BRIDGE_MIDDLE);
  ShrineOfTheSeaGod_SetUpBridge(ACTOR_SECOND_BROKEN_BRIDGE_MIDDLE);
  Actor_SetAnimationById(ACTOR_FIRST_BROKEN_BRIDGE_ENDS, 0);
  Actor_SetAnimationById(ACTOR_SECOND_BROKEN_BRIDGE_ENDS, 0);
  Actor_SetAnimationById(ACTOR_FIRST_BROKEN_BRIDGE_MIDDLE, 2);
  Actor_SetAnimationById(ACTOR_SECOND_BROKEN_BRIDGE_MIDDLE, 2);
  if (GameFlag_Get(FLAG_FIRST_BRIDGE_BROKEN)) {
    Map_CopyCellAttributeRect(0, 0, 3, 1, 19, 13);
    Map_CopyCellAttributeRect(40, 0, 2, 1, 19, 33);
    Actor_SetPositionAndResetMotion(
        Actor_GetById(ACTOR_FIRST_BROKEN_BRIDGE_MIDDLE), toFIXED(320),
        -toFIXED(21), toFIXED(534));
    Actor_SetSpritePriority(ACTOR_FIRST_BROKEN_BRIDGE_MIDDLE, 3);
  }
  if (GameFlag_Get(FLAG_SECOND_BRIDGE_BROKEN)) {
    Map_CopyCellAttributeRect(0, 0, 3, 1, 24, 22);
    Map_CopyCellAttributeRect(40, 0, 2, 1, 24, 24);
    Actor_SetPositionAndResetMotion(
        Actor_GetById(ACTOR_SECOND_BROKEN_BRIDGE_MIDDLE), toFIXED(400),
        -toFIXED(21), toFIXED(390));
    Actor_SetSpritePriority(ACTOR_SECOND_BROKEN_BRIDGE_MIDDLE, 3);
  }
  Rope_Initialize();
  Call4((void (*)())Rope_Place, 0, FLAG_SHRINE_ROPE_TIED,
        ACTOR_SOUTH_ROPE_EAST_POST, ACTOR_SOUTH_ROPE_WEST_POST);
  Rope_Place(1, FLAG_SHRINE_SECOND_ROPE_TIED, ACTOR_NORTH_ROPE_WEST_POST,
             ACTOR_NORTH_ROPE_EAST_POST);
  Actor_DisableById(ACTOR_FIRST_BRIDGE_SPLASH);
  Actor_GetById(ACTOR_FIRST_BRIDGE_SPLASH)->motionFlags = 0;
  firstSplash = Actor_GetById(ACTOR_FIRST_BRIDGE_OTHER_SPLASH);
  Actor_DisableById(ACTOR_FIRST_BRIDGE_OTHER_SPLASH);
  firstSplash->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
  firstSplash->motionFlags = 0;
  Actor_DisableById(ACTOR_SECOND_BRIDGE_SPLASH);
  Actor_GetById(ACTOR_SECOND_BRIDGE_SPLASH)->motionFlags = 0;
  secondSplash = Actor_GetById(ACTOR_SECOND_BRIDGE_OTHER_SPLASH);
  Actor_DisableById(ACTOR_SECOND_BRIDGE_OTHER_SPLASH);
  secondSplash->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
  secondSplash->motionFlags = 0;
}

/// Sets up the island room: its floating logs, and the water sinking back when
/// the party comes down from the statue room.
void ShrineOfTheSeaGod_InitializeSeaGodsTear(void) {
  ShrineOfTheSeaGod_SetUpFloatingLog(ACTOR_ISLAND_FIRST_LOG);
  ShrineOfTheSeaGod_SetUpFloatingLog(ACTOR_ISLAND_SECOND_LOG);
  ShrineOfTheSeaGod_SetUpFloatingLog(ACTOR_ISLAND_THIRD_LOG);
  if (gMainRoutineWork.entrance ==
          SHRINE_OF_THE_SEA_GOD_SEA_GODS_TEAR_FROM_SHRINE_OF_THE_SEA_GOD_STATUE &&
      GameFlag_Get(FLAG_KEEP_PARTY_POSITION) == 0)
    ShrineOfTheSeaGod_LowerWater();
}

/// Sets up the statue room: the party rises on the water when it comes up from
/// the island.
void ShrineOfTheSeaGod_InitializeStatue(void) {
  if (gMainRoutineWork.entrance ==
          SHRINE_OF_THE_SEA_GOD_STATUE_FROM_SHRINE_OF_THE_SEA_GOD_SEA_GODS_TEAR &&
      GameFlag_Get(FLAG_KEEP_PARTY_POSITION) == 0)
    ShrineOfTheSeaGod_RiseOnWater();
}

/// The Right Prong's update: a sparkle every eight frames, somewhere about it.
void ShrineOfTheSeaGod_SparkleRightProng(struct Actor *actor) {
  struct EffectOptions options;
  u32 phase = gFrameCount & 7;
  s32 dx;
  s32 dy;

  if (phase == 0) {
    {
      s32 mask = 15;

      dx = mask & Random16();
    }
    dy = Random16() & 7;
    options.type = SPRITE_SPARKLE;
    FieldEffect_Spawn(actor->x.fixed + INTtoFIXED(dx - 8),
                      actor->y.fixed + INTtoFIXED(dy - 8), actor->z.fixed, 0,
                      toFIXED(1) / 2, phase, EFFECT_USE_TYPE, &options);
  }
}

/// Sets up the Right Prong's room: the Prong floats above its place, shedding
/// sparkles.
void ShrineOfTheSeaGod_InitializeRightProng(void) {
  struct Actor *actor = Actor_GetById(FIELD_TREASURE_ACTOR);
  struct MapWork *map = Ram_HeapSlots->mapGrid;
  struct MapLayer *layer = &map->layers[2];

  layer->offsetX += toFIXED(128);
  MapGrid_CenterOnFollowedPoint();
  wait(1);
  if (actor != NULL) {
    actor->motionFlags = 0;
    actor->y.fixed = toFIXED(66);
    actor->update = ShrineOfTheSeaGod_SparkleRightProng;
  }
}

/// Sets up a floating log: flat on the water, behind the leader, disabled.
void ShrineOfTheSeaGod_SetUpFloatingLog(s32 id) {
  struct Actor *actor = Actor_GetById(id);

  Actor_SetSpritePriority(id, 3);
  Actor_DisableById(id);
  actor->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
  actor->motionFlags = 0;
  actor->scaleX = FIXED_ONE * 8 / 10;
  actor->scaleY = FIXED_ONE * 9 / 10;
}

/// Back from the battle with Breath: once it is beaten, the one enemy
/// defeated, it is put back where the torch trapped it, facing away.
s32 Scene_PrepareMap(void) {
  if (GameFlag_Get(FLAG_FIGHTING_BREATH)) {
    if (gMainRoutineWork.sequenceResult == 1) {
      if (GameFlag_Get(FLAG_BREATH_TRAPPED_EAST))
        Actor_SetPositionAndFacing(ACTOR_BREATH, toFIXED(360), toFIXED(216),
                                   FACING_EAST);
      else if (GameFlag_Get(FLAG_BREATH_TRAPPED_WEST)) {
        Actor_SetPositionAndFacing(ACTOR_BREATH, toFIXED(328), toFIXED(216),
                                   FACING_WEST);
      }
    }
    GameFlag_Clear(FLAG_FIGHTING_BREATH);
    GameFlag_Clear(FLAG_BREATH_TRAPPED_EAST);
    GameFlag_Clear(FLAG_BREATH_TRAPPED_WEST);
  }
  return 0;
}

#undef DEFINE_DJINNI_DASH
#undef DEFINE_DJINNI_WALK_AND_JUMP
#undef DEFINE_DJINNI_DASH_AROUND
#undef DEFINE_DJINNI_JUMP

extern u32 gFieldEffect_ShortScript[];
extern u32 gFieldEffect_LongScript[];

// clang-format off
#include "shared/field_effect_duration_scripts.c"

const s32 *gFieldEffect_DurationScripts[] = {
    gFieldEffect_ShortScript,
    gFieldEffect_MediumScript,
    gFieldEffect_LongScript,
};

/// Riki looks about the room, north-north-west, south-south-west and west, for
/// ever.
u32 gShrineOfTheSeaGod_RikiLooksAround[] = {
    SC_SET(SW_FACING, FACING_NORTH_NORTHWEST),
    SC_WAIT(80),
    SC_SET(SW_FACING, FACING_SOUTH_SOUTHWEST),
    SC_WAIT(80),
    SC_SET(SW_FACING, FACING_WEST),
    SC_WAIT(120),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

/// The boys walk home, out by the room's south way.
u32 gShrineOfTheSeaGod_BoysGoHome[] = {
    SC_ANIMATION(2),
    SC_MOVE(344, 0, 198),
    SC_SYNC,
    SC_MOVE(378, 0, 198),
    SC_SYNC,
    SC_MOVE(392, 0, 220),
    SC_SYNC,
    SC_MOVE(392, 0, 284),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_END,
};

/// What a push or Move lifts and sets down: each room's first placed actor, the
/// torch, or in the crossroads its stone pillar.
s16 gShrineOfTheSeaGod_PushedActors[] = {
    ACTOR_TORCH, -1,
};

/// The Djinni's footprints, laid by ShrineOfTheSeaGod_PlaceFootprints: where
/// each lies, mirrored (a scale of -1) or not, and its turn.
struct ShrineOfTheSeaGodSpot gShrineOfTheSeaGod_Footprints[] = {
    {toFIXED(316), toFIXED(136), -FIXED_ONE, TRIG_TURN * 16 / 32},
    {toFIXED(318), toFIXED(142), FIXED_ONE, TRIG_TURN * 14 / 32},
    {toFIXED(324), toFIXED(154), -FIXED_ONE, TRIG_TURN * 12 / 32},
    {toFIXED(330), toFIXED(158), FIXED_ONE, TRIG_TURN * 10 / 32},
    {toFIXED(338), toFIXED(166), -FIXED_ONE, TRIG_TURN * 9 / 32},
    {toFIXED(344), toFIXED(166), FIXED_ONE, TRIG_TURN * 9 / 32},
    {toFIXED(288), toFIXED(240), FIXED_ONE, TRIG_TURN * 27 / 32},
    {toFIXED(284), toFIXED(236), -FIXED_ONE, TRIG_TURN * 27 / 32},
    {toFIXED(272), toFIXED(232), FIXED_ONE, TRIG_TURN * 25 / 32},
    {toFIXED(266), toFIXED(230), -FIXED_ONE, TRIG_TURN * 25 / 32},
    {toFIXED(256), toFIXED(238), FIXED_ONE, TRIG_TURN * 22 / 32},
    {toFIXED(248), toFIXED(238), -FIXED_ONE, TRIG_TURN * 23 / 32},
    {toFIXED(240), toFIXED(250), FIXED_ONE, TRIG_TURN * 21 / 32},
    {toFIXED(234), toFIXED(252), -FIXED_ONE, TRIG_TURN * 22 / 32},
    {toFIXED(224), toFIXED(262), FIXED_ONE, TRIG_TURN * 21 / 32},
    {toFIXED(220), toFIXED(262), -FIXED_ONE, TRIG_TURN * 21 / 32},
};

/// Unused: the halfwords after the last footprint.
s16 gShrineOfTheSeaGod_FootprintsEnd[] = {-1, 0};

#include "shared/scene_entrances_default.c"

struct SceneRegion gShrineOfTheSeaGodStatueExitRegions[] = {
    {256, 32, 192, 272, 48, 208, CONDITION_ALWAYS,
     SHRINE_OF_THE_SEA_GOD_STATUE_TO_SHRINE_OF_THE_SEA_GOD_LADDERS_SOUTH},
    {-1},
};

struct SceneRegion gShrineOfTheSeaGodLaddersExitRegions[] = {
    {720, -38, 238, 736, -22, 254, CONDITION_ALWAYS,
     SHRINE_OF_THE_SEA_GOD_LADDERS_TO_SHRINE_OF_THE_SEA_GOD_STATUE_SOUTH},
    {864, 32, 208, 880, 48, 224, CONDITION_ALWAYS,
     SHRINE_OF_THE_SEA_GOD_LADDERS_TO_SHRINE_OF_THE_SEA_GOD_RIGHT_PRONG_WEST},
    {-1},
};

struct SceneRegion gShrineOfTheSeaGodRightProngExitRegions[] = {
    {784, -38, 160, 800, -22, 176, CONDITION_ALWAYS,
     SHRINE_OF_THE_SEA_GOD_RIGHT_PRONG_TO_SHRINE_OF_THE_SEA_GOD_LADDERS_EAST},
    {-1},
};

u32 gSceneExits[] = {
    SCENE_EXITS(ShrineOfTheSeaGodRope),
    SCENE_EXIT(SHRINE_OF_THE_SEA_GOD_ROPE_TO_SHRINE_OF_THE_SEA_GOD_CROSSROADS, ShrineOfTheSeaGodCrossroads, SHRINE_OF_THE_SEA_GOD_CROSSROADS_FROM_SHRINE_OF_THE_SEA_GOD_ROPE),
    SCENE_EXIT(SHRINE_OF_THE_SEA_GOD_ROPE_TO_SHRINE_OF_THE_SEA_GOD_PLANK_BRIDGE, ShrineOfTheSeaGodPlankBridge, SHRINE_OF_THE_SEA_GOD_PLANK_BRIDGE_FROM_SHRINE_OF_THE_SEA_GOD_ROPE),
    SCENE_EXIT(SHRINE_OF_THE_SEA_GOD_ROPE_TO_WORLD_MAP, WorldMap, WORLD_MAP_FROM_SHRINE_OF_THE_SEA_GOD_ROPE),
    SCENE_EXITS(ShrineOfTheSeaGodPlankBridge),
    SCENE_EXIT(SHRINE_OF_THE_SEA_GOD_PLANK_BRIDGE_TO_SHRINE_OF_THE_SEA_GOD_ROPE, ShrineOfTheSeaGodRope, SHRINE_OF_THE_SEA_GOD_ROPE_FROM_SHRINE_OF_THE_SEA_GOD_PLANK_BRIDGE),
    SCENE_EXITS(ShrineOfTheSeaGodCrossroads),
    SCENE_EXIT(SHRINE_OF_THE_SEA_GOD_CROSSROADS_TO_SHRINE_OF_THE_SEA_GOD_PILLARS, ShrineOfTheSeaGodPillars, SHRINE_OF_THE_SEA_GOD_PILLARS_FROM_SHRINE_OF_THE_SEA_GOD_CROSSROADS),
    SCENE_EXIT(SHRINE_OF_THE_SEA_GOD_CROSSROADS_TO_SHRINE_OF_THE_SEA_GOD_SEA_GODS_TEAR_WEST, ShrineOfTheSeaGodSeaGodsTear, SHRINE_OF_THE_SEA_GOD_SEA_GODS_TEAR_FROM_SHRINE_OF_THE_SEA_GOD_CROSSROADS_WEST),
    SCENE_EXIT(SHRINE_OF_THE_SEA_GOD_CROSSROADS_TO_SHRINE_OF_THE_SEA_GOD_SEA_GODS_TEAR_EAST, ShrineOfTheSeaGodSeaGodsTear, SHRINE_OF_THE_SEA_GOD_SEA_GODS_TEAR_FROM_SHRINE_OF_THE_SEA_GOD_CROSSROADS_EAST),
    SCENE_EXIT(SHRINE_OF_THE_SEA_GOD_CROSSROADS_TO_SHRINE_OF_THE_SEA_GOD_FOOTPRINTS, ShrineOfTheSeaGodFootprints, SHRINE_OF_THE_SEA_GOD_FOOTPRINTS_FROM_SHRINE_OF_THE_SEA_GOD_CROSSROADS),
    SCENE_EXIT(SHRINE_OF_THE_SEA_GOD_CROSSROADS_TO_SHRINE_OF_THE_SEA_GOD_ROPE, ShrineOfTheSeaGodRope, SHRINE_OF_THE_SEA_GOD_ROPE_FROM_SHRINE_OF_THE_SEA_GOD_CROSSROADS),
    SCENE_EXITS(ShrineOfTheSeaGodFootprints),
    SCENE_EXIT(SHRINE_OF_THE_SEA_GOD_FOOTPRINTS_TO_SHRINE_OF_THE_SEA_GOD_CROSSROADS, ShrineOfTheSeaGodCrossroads, SHRINE_OF_THE_SEA_GOD_CROSSROADS_FROM_SHRINE_OF_THE_SEA_GOD_FOOTPRINTS),
    SCENE_EXIT(SHRINE_OF_THE_SEA_GOD_FOOTPRINTS_TO_SHRINE_OF_THE_SEA_GOD_BREATH_NORTHEAST, ShrineOfTheSeaGodBreath, SHRINE_OF_THE_SEA_GOD_BREATH_FROM_SHRINE_OF_THE_SEA_GOD_FOOTPRINTS_NORTHEAST),
    SCENE_EXIT(SHRINE_OF_THE_SEA_GOD_FOOTPRINTS_TO_SHRINE_OF_THE_SEA_GOD_BREATH_SOUTHEAST, ShrineOfTheSeaGodBreath, SHRINE_OF_THE_SEA_GOD_BREATH_FROM_SHRINE_OF_THE_SEA_GOD_FOOTPRINTS_SOUTHEAST),
    SCENE_EXIT(SHRINE_OF_THE_SEA_GOD_FOOTPRINTS_TO_SHRINE_OF_THE_SEA_GOD_BREATH_WEST, ShrineOfTheSeaGodBreath, SHRINE_OF_THE_SEA_GOD_BREATH_FROM_SHRINE_OF_THE_SEA_GOD_FOOTPRINTS_WEST),
    SCENE_EXITS(ShrineOfTheSeaGodBreath),
    SCENE_EXIT(SHRINE_OF_THE_SEA_GOD_BREATH_TO_SHRINE_OF_THE_SEA_GOD_PILLAR_HOLE, ShrineOfTheSeaGodPillarHole, SHRINE_OF_THE_SEA_GOD_PILLAR_HOLE_FROM_SHRINE_OF_THE_SEA_GOD_BREATH),
    SCENE_EXIT(SHRINE_OF_THE_SEA_GOD_BREATH_TO_SHRINE_OF_THE_SEA_GOD_FOOTPRINTS_NORTHEAST, ShrineOfTheSeaGodFootprints, SHRINE_OF_THE_SEA_GOD_FOOTPRINTS_FROM_SHRINE_OF_THE_SEA_GOD_BREATH_NORTHEAST),
    SCENE_EXIT(SHRINE_OF_THE_SEA_GOD_BREATH_TO_SHRINE_OF_THE_SEA_GOD_FOOTPRINTS_WEST, ShrineOfTheSeaGodFootprints, SHRINE_OF_THE_SEA_GOD_FOOTPRINTS_FROM_SHRINE_OF_THE_SEA_GOD_BREATH_WEST),
    SCENE_EXIT(SHRINE_OF_THE_SEA_GOD_BREATH_TO_SHRINE_OF_THE_SEA_GOD_FOOTPRINTS_SOUTHEAST, ShrineOfTheSeaGodFootprints, SHRINE_OF_THE_SEA_GOD_FOOTPRINTS_FROM_SHRINE_OF_THE_SEA_GOD_BREATH_SOUTHEAST),
    SCENE_EXITS(ShrineOfTheSeaGodPillarHole),
    SCENE_EXIT(SHRINE_OF_THE_SEA_GOD_PILLAR_HOLE_TO_SHRINE_OF_THE_SEA_GOD_BREATH, ShrineOfTheSeaGodBreath, SHRINE_OF_THE_SEA_GOD_BREATH_FROM_SHRINE_OF_THE_SEA_GOD_PILLAR_HOLE),
    SCENE_EXIT(SHRINE_OF_THE_SEA_GOD_PILLAR_HOLE_TO_SHRINE_OF_THE_SEA_GOD_PILLARS, ShrineOfTheSeaGodPillars, SHRINE_OF_THE_SEA_GOD_PILLARS_FROM_SHRINE_OF_THE_SEA_GOD_PILLAR_HOLE),
    SCENE_EXITS(ShrineOfTheSeaGodPillars),
    SCENE_EXIT(SHRINE_OF_THE_SEA_GOD_PILLARS_TO_SHRINE_OF_THE_SEA_GOD_PILLAR_HOLE, ShrineOfTheSeaGodPillarHole, SHRINE_OF_THE_SEA_GOD_PILLAR_HOLE_FROM_SHRINE_OF_THE_SEA_GOD_PILLARS),
    SCENE_EXIT(SHRINE_OF_THE_SEA_GOD_PILLARS_TO_SHRINE_OF_THE_SEA_GOD_CROSSROADS, ShrineOfTheSeaGodCrossroads, SHRINE_OF_THE_SEA_GOD_CROSSROADS_FROM_SHRINE_OF_THE_SEA_GOD_PILLARS),
    SCENE_EXITS(ShrineOfTheSeaGodSeaGodsTear),
    SCENE_EXIT(SHRINE_OF_THE_SEA_GOD_SEA_GODS_TEAR_TO_SHRINE_OF_THE_SEA_GOD_CROSSROADS_WEST, ShrineOfTheSeaGodCrossroads, SHRINE_OF_THE_SEA_GOD_CROSSROADS_FROM_SHRINE_OF_THE_SEA_GOD_SEA_GODS_TEAR_WEST),
    SCENE_EXIT(SHRINE_OF_THE_SEA_GOD_SEA_GODS_TEAR_TO_SHRINE_OF_THE_SEA_GOD_CROSSROADS_EAST, ShrineOfTheSeaGodCrossroads, SHRINE_OF_THE_SEA_GOD_CROSSROADS_FROM_SHRINE_OF_THE_SEA_GOD_SEA_GODS_TEAR_EAST),
    SCENE_EXIT(SHRINE_OF_THE_SEA_GOD_SEA_GODS_TEAR_TO_SHRINE_OF_THE_SEA_GOD_STATUE, ShrineOfTheSeaGodStatue, SHRINE_OF_THE_SEA_GOD_STATUE_FROM_SHRINE_OF_THE_SEA_GOD_SEA_GODS_TEAR),
    SCENE_EXITS(ShrineOfTheSeaGodStatue),
    SCENE_EXIT(SHRINE_OF_THE_SEA_GOD_STATUE_TO_SHRINE_OF_THE_SEA_GOD_SEA_GODS_TEAR, ShrineOfTheSeaGodSeaGodsTear, SHRINE_OF_THE_SEA_GOD_SEA_GODS_TEAR_FROM_SHRINE_OF_THE_SEA_GOD_STATUE),
    SCENE_EXIT(SHRINE_OF_THE_SEA_GOD_STATUE_TO_SHRINE_OF_THE_SEA_GOD_LADDERS_SOUTH, ShrineOfTheSeaGodLadders, SHRINE_OF_THE_SEA_GOD_LADDERS_FROM_SHRINE_OF_THE_SEA_GOD_STATUE_NORTH),
    SCENE_EXITS(ShrineOfTheSeaGodLadders),
    SCENE_EXIT(SHRINE_OF_THE_SEA_GOD_LADDERS_TO_SHRINE_OF_THE_SEA_GOD_STATUE_SOUTH, ShrineOfTheSeaGodStatue, SHRINE_OF_THE_SEA_GOD_STATUE_FROM_SHRINE_OF_THE_SEA_GOD_LADDERS_SOUTH),
    SCENE_EXIT(SHRINE_OF_THE_SEA_GOD_LADDERS_TO_SHRINE_OF_THE_SEA_GOD_RIGHT_PRONG_WEST, ShrineOfTheSeaGodRightProng, SHRINE_OF_THE_SEA_GOD_RIGHT_PRONG_FROM_SHRINE_OF_THE_SEA_GOD_LADDERS_WEST),
    SCENE_EXITS(ShrineOfTheSeaGodRightProng),
    SCENE_EXIT(SHRINE_OF_THE_SEA_GOD_RIGHT_PRONG_TO_SHRINE_OF_THE_SEA_GOD_LADDERS_EAST, ShrineOfTheSeaGodLadders, SHRINE_OF_THE_SEA_GOD_LADDERS_FROM_SHRINE_OF_THE_SEA_GOD_RIGHT_PRONG_EAST),
    SCENE_EXITS_END,
};

struct ActorPlacement gShrineOfTheSeaGod_Placements[] = {
    {SCENE_TABLE_END},
};

struct ActorPlacement gShrineOfTheSeaGodRopeActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_FLOATING_LOG_WEST_EAST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 224, 0, 216, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FLOATING_LOG_WEST_EAST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 256, 0, 280, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 328, 0, 168, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 248, 0, 168, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_RIKI, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 340, 0, 168, FACING_WEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TAVI, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 232, 0, 168, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gShrineOfTheSeaGodPlankBridgeActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_FLAME_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 328, 0, 184, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_EXCLAMATION_BUBBLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PLANK_BRIDGE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 216, 0, 272, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_RIKI, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gShrineOfTheSeaGodCrossroadsActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 216, 0, 296, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_LOG_PAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 248, 0, 216, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PLANK_BRIDGE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 408, 0, 216, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PLANK_BRIDGE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 520, 0, 232, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_PLANK_BRIDGE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 536, 0, 344, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_PLANK_BRIDGE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 408, 0, 328, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PLANK_BRIDGE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 408, 0, 328, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_JUPITER_DJINNI, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 472, 0, 328, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gShrineOfTheSeaGodFootprintsActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_FOOTPRINTS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FOOTPRINTS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_FOOTPRINTS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_FOOTPRINTS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_FOOTPRINTS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_FOOTPRINTS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_FOOTPRINTS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_FOOTPRINTS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_FOOTPRINTS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_FOOTPRINTS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_FOOTPRINTS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_FOOTPRINTS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_FOOTPRINTS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_FOOTPRINTS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_FOOTPRINTS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_FOOTPRINTS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    {SCENE_TABLE_END},
};

struct ActorPlacement gShrineOfTheSeaGodBreathActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_FLAME_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 344, 0, 248, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_EXCLAMATION_BUBBLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 136, 0, 296, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 168, 0, 296, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 168, 0, 280, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PLANK_BRIDGE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 328, 0, 552, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_JUPITER_DJINNI, FLAG_DJINNI(DJINNI_BREATH), MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gShrineOfTheSeaGodPillarHoleActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 248, 0, 248, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 232, 0, 424, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 232, 0, 408, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 232, 0, 392, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    {SCENE_TABLE_END},
};

struct ActorPlacement gShrineOfTheSeaGodPillarsActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_PLANK_BRIDGE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 216, 0, 248, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PLANK_BRIDGE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 312, 0, 264, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_PLANK_BRIDGE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 408, 0, 232, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_PLANK_BRIDGE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 312, 0, 376, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_LOG_PAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 360, 0, 424, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PLANK_BRIDGE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 328, 0, 216, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PLANK_BRIDGE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 408, 0, 360, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_PLANK_BRIDGE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 328, 0, 216, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PLANK_BRIDGE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 408, 0, 360, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 344, 0, 616, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 296, 0, 616, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 456, 0, 392, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 504, 0, 392, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_SPLASH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SPLASH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_SPLASH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SPLASH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 448, 0, 248, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 464, 0, 248, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 456, 0, 240, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gShrineOfTheSeaGodSeaGodsTearActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_FLOATING_LOG_WEST_EAST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 160, 0, 202, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FLOATING_LOG_WEST_EAST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 176, 0, 298, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_FLOATING_LOG_WEST_EAST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 320, 0, 218, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    {SCENE_TABLE_END},
};

struct Trigger gShrineOfTheSeaGod_Triggers[] = {
    {TRIGGER_TABLE_END},
};

struct Trigger gShrineOfTheSeaGodRopeTriggers[] = {
    {TRIGGER_EXIT, SHRINE_OF_THE_SEA_GOD_ROPE_TO_SHRINE_OF_THE_SEA_GOD_CROSSROADS, CONDITION_ALWAYS, SHRINE_OF_THE_SEA_GOD_ROPE_TO_SHRINE_OF_THE_SEA_GOD_CROSSROADS},
    {TRIGGER_EXIT, SHRINE_OF_THE_SEA_GOD_ROPE_TO_SHRINE_OF_THE_SEA_GOD_PLANK_BRIDGE, CONDITION_ALWAYS, SHRINE_OF_THE_SEA_GOD_ROPE_TO_SHRINE_OF_THE_SEA_GOD_PLANK_BRIDGE},
    {TRIGGER_EXIT, SHRINE_OF_THE_SEA_GOD_ROPE_TO_WORLD_MAP, CONDITION_ALWAYS, SHRINE_OF_THE_SEA_GOD_ROPE_TO_WORLD_MAP},
    {PSYNERGY_ON_ACTOR(ABILITY_LASH), ACTOR_ROPE_ROOM_EAST_POST, FLAG_SHRINE_ROPE_TIED, EVENT(ShrineOfTheSeaGod_LashRope)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_AFTER_CAST), 0, CONDITION_ALWAYS, EVENT(ShrineOfTheSeaGod_CastSand)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(ShrineOfTheSeaGod_EndSand)},
    {TRIGGER_STEP, RIKI_THROWS_ROPE_CELL, FLAG_RIKI_GAVE_UP, EVENT(ShrineOfTheSeaGod_ThrowRopeAgain)},
    {TRIGGER_TABLE_END},
};

struct Trigger gShrineOfTheSeaGodPlankBridgeTriggers[] = {
    {TRIGGER_EXIT, SHRINE_OF_THE_SEA_GOD_PLANK_BRIDGE_TO_SHRINE_OF_THE_SEA_GOD_ROPE, CONDITION_ALWAYS, SHRINE_OF_THE_SEA_GOD_PLANK_BRIDGE_TO_SHRINE_OF_THE_SEA_GOD_ROPE},
    {TALK_TRIGGER_FACING(FACING_WEST), ACTOR_SIDE_ROOM_RIKI, CONDITION_ALWAYS, EVENT(ShrineOfTheSeaGod_StartleRiki)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SIDE_ROOM_RIKI, CONDITION_ALWAYS, MSGNO(MsgFieldICantFindAnythingAroundHere)},
    {TRIGGER_TABLE_END},
};

struct Trigger gShrineOfTheSeaGodCrossroadsTriggers[] = {
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, SHRINE_OF_THE_SEA_GOD_CROSSROADS_TO_SHRINE_OF_THE_SEA_GOD_PILLARS, CONDITION_ALWAYS, SHRINE_OF_THE_SEA_GOD_CROSSROADS_TO_SHRINE_OF_THE_SEA_GOD_PILLARS},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, SHRINE_OF_THE_SEA_GOD_CROSSROADS_TO_SHRINE_OF_THE_SEA_GOD_SEA_GODS_TEAR_WEST, CONDITION_ALWAYS, SHRINE_OF_THE_SEA_GOD_CROSSROADS_TO_SHRINE_OF_THE_SEA_GOD_SEA_GODS_TEAR_WEST},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, SHRINE_OF_THE_SEA_GOD_CROSSROADS_TO_SHRINE_OF_THE_SEA_GOD_SEA_GODS_TEAR_EAST, CONDITION_ALWAYS, SHRINE_OF_THE_SEA_GOD_CROSSROADS_TO_SHRINE_OF_THE_SEA_GOD_SEA_GODS_TEAR_EAST},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, SHRINE_OF_THE_SEA_GOD_CROSSROADS_TO_SHRINE_OF_THE_SEA_GOD_FOOTPRINTS, CONDITION_ALWAYS, SHRINE_OF_THE_SEA_GOD_CROSSROADS_TO_SHRINE_OF_THE_SEA_GOD_FOOTPRINTS},
    {TRIGGER_EXIT, SHRINE_OF_THE_SEA_GOD_CROSSROADS_TO_SHRINE_OF_THE_SEA_GOD_ROPE, CONDITION_ALWAYS, SHRINE_OF_THE_SEA_GOD_CROSSROADS_TO_SHRINE_OF_THE_SEA_GOD_ROPE},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_CROSSROADS_PILLAR, FLAG_CROSSROADS_PILLAR_MOVED, EVENT(ShrineOfTheSeaGod_LiftBlock)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_CROSSROADS_PILLAR, FLAG_CROSSROADS_PILLAR_MOVED, EVENT(ShrineOfTheSeaGod_MovePillar)},
    {TRIGGER_STEP, CROSSROADS_BRIDGE_END_CELL, FLAG_LEADER_IN_FRONT, EVENT(ShrineOfTheSeaGod_DrawLeaderInFront)},
    {TRIGGER_STEP, CROSSROADS_LADDER_FOOT_CELL, IF_FLAG_SET(FLAG_LEADER_IN_FRONT), EVENT(ShrineOfTheSeaGod_DrawLeaderBehind)},
    {TRIGGER_STEP, CROSSROADS_BROKEN_BRIDGE_CELL, FLAG_CROSSROADS_BRIDGE_BROKEN, EVENT(ShrineOfTheSeaGod_BreakCrossroadsBridge)},
    {TRIGGER_STEP, CROSSROADS_LADDER_FOOT_CELL, FLAG_DJINNI_FLED_EAST, EVENT(ShrineOfTheSeaGod_DjinniFleesEast)},
    {TRIGGER_STEP, CROSSROADS_BREATH_FLEES_SOUTH_CELL, FLAG_DJINNI_FLED_SOUTH, EVENT(ShrineOfTheSeaGod_DjinniFleesSouth)},
    {TRIGGER_TABLE_END},
};

struct Trigger gShrineOfTheSeaGodFootprintsTriggers[] = {
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, SHRINE_OF_THE_SEA_GOD_FOOTPRINTS_TO_SHRINE_OF_THE_SEA_GOD_CROSSROADS, CONDITION_ALWAYS, SHRINE_OF_THE_SEA_GOD_FOOTPRINTS_TO_SHRINE_OF_THE_SEA_GOD_CROSSROADS},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, SHRINE_OF_THE_SEA_GOD_FOOTPRINTS_TO_SHRINE_OF_THE_SEA_GOD_BREATH_NORTHEAST, CONDITION_ALWAYS, SHRINE_OF_THE_SEA_GOD_FOOTPRINTS_TO_SHRINE_OF_THE_SEA_GOD_BREATH_NORTHEAST},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, SHRINE_OF_THE_SEA_GOD_FOOTPRINTS_TO_SHRINE_OF_THE_SEA_GOD_BREATH_SOUTHEAST, CONDITION_ALWAYS, SHRINE_OF_THE_SEA_GOD_FOOTPRINTS_TO_SHRINE_OF_THE_SEA_GOD_BREATH_SOUTHEAST},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, SHRINE_OF_THE_SEA_GOD_FOOTPRINTS_TO_SHRINE_OF_THE_SEA_GOD_BREATH_WEST, CONDITION_ALWAYS, SHRINE_OF_THE_SEA_GOD_FOOTPRINTS_TO_SHRINE_OF_THE_SEA_GOD_BREATH_WEST},
    {TRIGGER_TABLE_END},
};
// clang-format on

struct Trigger gShrineOfTheSeaGodBreathTriggers[] = {
    {TRIGGER_EXIT,
     SHRINE_OF_THE_SEA_GOD_BREATH_TO_SHRINE_OF_THE_SEA_GOD_PILLAR_HOLE,
     CONDITION_ALWAYS,
     SHRINE_OF_THE_SEA_GOD_BREATH_TO_SHRINE_OF_THE_SEA_GOD_PILLAR_HOLE},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN,
     SHRINE_OF_THE_SEA_GOD_BREATH_TO_SHRINE_OF_THE_SEA_GOD_FOOTPRINTS_NORTHEAST,
     CONDITION_ALWAYS,
     SHRINE_OF_THE_SEA_GOD_BREATH_TO_SHRINE_OF_THE_SEA_GOD_FOOTPRINTS_NORTHEAST},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN,
     SHRINE_OF_THE_SEA_GOD_BREATH_TO_SHRINE_OF_THE_SEA_GOD_FOOTPRINTS_WEST,
     CONDITION_ALWAYS,
     SHRINE_OF_THE_SEA_GOD_BREATH_TO_SHRINE_OF_THE_SEA_GOD_FOOTPRINTS_WEST},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN,
     SHRINE_OF_THE_SEA_GOD_BREATH_TO_SHRINE_OF_THE_SEA_GOD_FOOTPRINTS_SOUTHEAST,
     CONDITION_ALWAYS,
     SHRINE_OF_THE_SEA_GOD_BREATH_TO_SHRINE_OF_THE_SEA_GOD_FOOTPRINTS_SOUTHEAST},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_FIRST_PUDDLE,
     FLAG_SHRINE_FIRST_PUDDLE_FROZEN,
     EVENT(ShrineOfTheSeaGod_FreezeFirstPuddle)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_SECOND_PUDDLE,
     FLAG_SHRINE_SECOND_PUDDLE_FROZEN,
     EVENT(ShrineOfTheSeaGod_FreezeSecondPuddle)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_THIRD_PUDDLE,
     FLAG_SHRINE_THIRD_PUDDLE_FROZEN,
     EVENT(ShrineOfTheSeaGod_FreezeThirdPuddle)},
    {TRIGGER_PUSH, 0, CONDITION_ALWAYS, EVENT(ShrineOfTheSeaGod_LiftBlock)},
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS,
     EVENT(ShrineOfTheSeaGod_SetDownTorch)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST),
     ACTOR_TORCH, CONDITION_ALWAYS, EVENT(ShrineOfTheSeaGod_LiftBlock)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_TORCH, CONDITION_ALWAYS,
     EVENT(ShrineOfTheSeaGod_SetDownTorch)},
    {TRIGGER_STEP, LEDGE_ROOM_SOUTH_STAIRS_CELL, FLAG_DJINNI_BY_LEDGE,
     EVENT(ShrineOfTheSeaGod_DjinniRunsNorth)},
    {TRIGGER_STEP, LEDGE_ROOM_SOUTH_PATH_CELL, FLAG_DJINNI_BY_LEDGE,
     EVENT(ShrineOfTheSeaGod_DjinniRunsWest)},
    {TRIGGER_STEP, LEDGE_ROOM_WAY_UP_CELL, FLAG_DJINNI_ON_LEDGE,
     EVENT(ShrineOfTheSeaGod_DjinniHopsOntoLedge)},
    {TRIGGER_STEP, STEP_LEDGE_SOUTHEAST_WAY_UP, FLAG_DJINNI_CHASE_OVER,
     EVENT(ShrineOfTheSeaGod_ChaseDjinni)},
    {TRIGGER_STEP, STEP_LEDGE_EAST_SIDE_SOUTH, FLAG_DJINNI_CHASE_OVER,
     EVENT(ShrineOfTheSeaGod_ChaseDjinni)},
    {TRIGGER_STEP, STEP_LEDGE_EAST_SIDE_NORTH, FLAG_DJINNI_CHASE_OVER,
     EVENT(ShrineOfTheSeaGod_ChaseDjinni)},
    {TRIGGER_STEP, STEP_LEDGE_NORTH_SIDE_EAST, FLAG_DJINNI_CHASE_OVER,
     EVENT(ShrineOfTheSeaGod_ChaseDjinni)},
    {TRIGGER_STEP, STEP_LEDGE_NORTH_SIDE_WEST, FLAG_DJINNI_CHASE_OVER,
     EVENT(ShrineOfTheSeaGod_ChaseDjinni)},
    {TRIGGER_STEP, STEP_LEDGE_WEST_SIDE_NORTH, FLAG_DJINNI_CHASE_OVER,
     EVENT(ShrineOfTheSeaGod_ChaseDjinni)},
    {TRIGGER_STEP, STEP_LEDGE_WEST_SIDE_SOUTH, FLAG_DJINNI_CHASE_OVER,
     EVENT(ShrineOfTheSeaGod_ChaseDjinni)},
    {TRIGGER_STEP, STEP_LEDGE_SOUTHWEST_WAY_UP, FLAG_DJINNI_CHASE_OVER,
     EVENT(ShrineOfTheSeaGod_ChaseDjinni)},
    {TRIGGER_TALK, ACTOR_BREATH, CONDITION_ALWAYS,
     EVENT(ShrineOfTheSeaGod_FightBreath)},
    {TRIGGER_TABLE_END},
};

struct Trigger gShrineOfTheSeaGodPillarHoleTriggers[] = {
    {TRIGGER_EXIT,
     SHRINE_OF_THE_SEA_GOD_PILLAR_HOLE_TO_SHRINE_OF_THE_SEA_GOD_BREATH,
     CONDITION_ALWAYS,
     SHRINE_OF_THE_SEA_GOD_PILLAR_HOLE_TO_SHRINE_OF_THE_SEA_GOD_BREATH},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN,
     SHRINE_OF_THE_SEA_GOD_PILLAR_HOLE_TO_SHRINE_OF_THE_SEA_GOD_PILLARS,
     CONDITION_ALWAYS,
     SHRINE_OF_THE_SEA_GOD_PILLAR_HOLE_TO_SHRINE_OF_THE_SEA_GOD_PILLARS},
    {TRIGGER_PUSHED, 0, FLAG_PILLAR_IN_HOLE,
     EVENT(ShrineOfTheSeaGod_PushPillarIntoHole)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_HOLE_ROOM_PILLAR,
     FLAG_PILLAR_IN_HOLE, EVENT(ShrineOfTheSeaGod_PushPillarIntoHole)},
    {TRIGGER_TABLE_END},
};

struct Trigger gShrineOfTheSeaGodPillarsTriggers[] = {
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP,
     SHRINE_OF_THE_SEA_GOD_PILLARS_TO_SHRINE_OF_THE_SEA_GOD_PILLAR_HOLE,
     CONDITION_ALWAYS,
     SHRINE_OF_THE_SEA_GOD_PILLARS_TO_SHRINE_OF_THE_SEA_GOD_PILLAR_HOLE},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP,
     SHRINE_OF_THE_SEA_GOD_PILLARS_TO_SHRINE_OF_THE_SEA_GOD_CROSSROADS,
     CONDITION_ALWAYS,
     SHRINE_OF_THE_SEA_GOD_PILLARS_TO_SHRINE_OF_THE_SEA_GOD_CROSSROADS},
    {PSYNERGY_ON_ACTOR(ABILITY_LASH), ACTOR_SOUTH_ROPE_EAST_POST,
     FLAG_SHRINE_ROPE_TIED, 0},
    {PSYNERGY_ON_ACTOR(ABILITY_LASH), ACTOR_NORTH_ROPE_WEST_POST,
     FLAG_SHRINE_SECOND_ROPE_TIED, 0},
    {TRIGGER_STEP, FIRST_BROKEN_BRIDGE_CELL, FLAG_FIRST_BRIDGE_BROKEN,
     EVENT(ShrineOfTheSeaGod_BreakFirstBridge)},
    {TRIGGER_STEP, SECOND_BROKEN_BRIDGE_CELL, FLAG_SECOND_BRIDGE_BROKEN,
     EVENT(ShrineOfTheSeaGod_BreakSecondBridge)},
    {TRIGGER_TALK, ACTOR_CHEST_NORTHWEST_MARKER, CONDITION_ALWAYS,
     EVENT(ShrineOfTheSeaGod_OpenFieldMenu)},
    {TRIGGER_TALK, ACTOR_CHEST_NORTHEAST_MARKER, CONDITION_ALWAYS,
     EVENT(ShrineOfTheSeaGod_OpenFieldMenu)},
    {TRIGGER_TALK, ACTOR_CHEST_NORTH_MARKER, CONDITION_ALWAYS,
     EVENT(ShrineOfTheSeaGod_OpenFieldMenu)},
    {TRIGGER_TABLE_END},
};

struct Trigger gShrineOfTheSeaGodSeaGodsTearTriggers[] = {
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP,
     SHRINE_OF_THE_SEA_GOD_SEA_GODS_TEAR_TO_SHRINE_OF_THE_SEA_GOD_CROSSROADS_WEST,
     CONDITION_ALWAYS,
     SHRINE_OF_THE_SEA_GOD_SEA_GODS_TEAR_TO_SHRINE_OF_THE_SEA_GOD_CROSSROADS_WEST},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP,
     SHRINE_OF_THE_SEA_GOD_SEA_GODS_TEAR_TO_SHRINE_OF_THE_SEA_GOD_CROSSROADS_EAST,
     CONDITION_ALWAYS,
     SHRINE_OF_THE_SEA_GOD_SEA_GODS_TEAR_TO_SHRINE_OF_THE_SEA_GOD_CROSSROADS_EAST},
    {TRIGGER_EXAMINE, ISLAND_STATUE_CELL, CONDITION_ALWAYS,
     EVENT(ShrineOfTheSeaGod_ExamineStatue)},
    {USE_ITEM(ITEM_SEA_GODS_TEAR), ISLAND_STATUE_CELL, CONDITION_ALWAYS,
     EVENT(ShrineOfTheSeaGod_UseSeaGodsTear)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0,
     CONDITION_ALWAYS, EVENT(ShrineOfTheSeaGod_RevealStone)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0,
     CONDITION_ALWAYS, EVENT(ShrineOfTheSeaGod_HideStone)},
    {TRIGGER_TABLE_END},
};

struct Trigger gShrineOfTheSeaGodStatueTriggers[] = {
    {TRIGGER_EXAMINE, STATUE_ROOM_STATUE_CELL, CONDITION_ALWAYS,
     EVENT(ShrineOfTheSeaGod_ExamineStatue)},
    {USE_ITEM(ITEM_SEA_GODS_TEAR), STATUE_ROOM_STATUE_CELL, CONDITION_ALWAYS,
     EVENT(ShrineOfTheSeaGod_SinkWithWater)},
    {TRIGGER_TABLE_END},
};
