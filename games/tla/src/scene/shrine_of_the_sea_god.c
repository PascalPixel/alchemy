/// @file
/// The Shrine of the Sea God east of Daila, in eleven rooms that share this
/// overlay (SceneId_ShrineOfTheSeaGodRope to
/// SceneId_ShrineOfTheSeaGodRightProng).
///
/// On the first visit Tavi is stuck on a ledge in the entrance hall (the rope
/// room) and Riki cannot throw him the rope; Riki goes off by the plank bridge
/// and Lash ties the rope so Tavi climbs down. Past the crossroads, where the
/// Djinni Breath is first seen, its footprints cross the room below and it is
/// chased around a ledge (Breath's room) until the torch pushed against the
/// north wall traps it. On the second visit, with Frost on the puddles, Lash
/// and the fallen pillars (the pillar hole and the pillars), the party reaches
/// the island, whose Sea God's Tear raises the water to the statue room, and
/// climbs the ladders to the Right Prong.
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
#include "field/field_sequence_event.h"
#include "field/map_grid.h"
#include "field/scene_tables.h"
#include "field/speaker.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "menu/menu.h"
#include "scene/scene.h"
#include "scene/shrine_of_the_sea_god.h"
#include "sound/sound.h"
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
#define EFFECT_SPAWN FieldEffect_Spawn
#define EFFECT_CREATE Import_ActorCreate
#define EFFECT_SET_MODE Actor_SelectAnimation
#define EFFECT_SET_SCRIPT Actor_StartScript
#define EFFECT_SCRIPTS gFieldEffect_DurationScripts
#include "../field/field_effect_spawn.c"

#define FLAG_CROSSROADS_PILLAR_MOVED                                           \
  0x84c                              ///< Move pushed the great hall's pillar
#define FLAG_RIKI_GAVE_UP 0x872      ///< Riki missed with the rope, went off
#define FLAG_DJINNI_FLED_EAST 0x874  ///< the Djinni ran from the great hall
#define FLAG_DJINNI_FLED_SOUTH 0x875 ///< and ran on, out of it
#define FLAG_DJINNI_RAN_PAST 0x876   ///< its footprints are gone
#define FLAG_DJINNI_BY_LEDGE 0x877   ///< it waits by the ledge
#define FLAG_DJINNI_ON_LEDGE 0x87c   ///< it hopped up onto the ledge
#define FLAG_FIRST_PUDDLE_FROZEN FLAG_SCENE_TEMP_00
#define FLAG_SECOND_PUDDLE_FROZEN FLAG_SCENE_TEMP_01
#define FLAG_THIRD_PUDDLE_FROZEN FLAG_SCENE_TEMP_02
#define FLAG_ROPE_TIED FLAG_SCENE_TEMP_03
#define FLAG_SECOND_ROPE_TIED FLAG_SCENE_TEMP_04
#define FLAG_DJINNI_CHASE_OVER FLAG_SCENE_TEMP_05
#define FLAG_DJINNI_TRAPPED FLAG_SCENE_TEMP_06
#define FLAG_TORCH_AT_NORTH_WALL FLAG_SCENE_TEMP_07
#define FLAG_SLIDES_SHOWN FLAG_SCENE_TEMP_08
#define FLAG_LEADER_IN_FRONT FLAG_SCENE_TEMP_0A
#define FLAG_CROSSROADS_PILLAR_TOPPLED FLAG_SCENE_TEMP_30
#define FLAG_FIRST_PILLAR_TOPPLED FLAG_SCENE_TEMP_31
#define FLAG_SECOND_PILLAR_TOPPLED FLAG_SCENE_TEMP_32
#define FLAG_PILLAR_IN_HOLE FLAG_AREA_TEMP_01
/// Set as the battle with Breath starts, with where it was trapped, so the
/// scene puts it back there if the party runs.
#define FLAG_FIGHTING_BREATH FLAG_AREA_TEMP_40
#define FLAG_BREATH_TRAPPED_EAST FLAG_AREA_TEMP_41
#define FLAG_BREATH_TRAPPED_WEST FLAG_AREA_TEMP_42

enum { MAP_WADING_UPPER_LEVEL = 1 };

/// MapWork as far as groundEffects, read through its bits, the footprints and
/// the kind of wading; map_grid.h keeps the byte whole. A room with a second
/// level sets wading to MAP_WADING_UPPER_LEVEL.
struct MapWorkGroundEffects {
  u8 before[offsetof(struct MapWork, groundEffects)];
  u8 footprints : 2;
  u8 wading : 2;
  u8 unused : 4;
};

/// Sets the map's kind of wading to that of a room with a second level.
#define Map_SetUpperLevelWading(map)                                           \
  (((struct MapWorkGroundEffects *)(map))->wading = MAP_WADING_UPPER_LEVEL)

void Actor_DisableById(s32 actor);
void Actor_LiftTargetBlock(const void *script);
void Actor_PlaceMovedBlock(void);
void ShrineOfTheSeaGod_SetSlideActorsShown(s32 show);
void ShrineOfTheSeaGod_KeepWithLeader(struct Actor *object);

static inline void ShrineOfTheSeaGod_ObjSetPos(struct Actor *object, s32 x,
                                               s32 y, s32 z) {
  Actor_SetPositionAndResetMotion(object, x, y, z);
}

/// Places an actor at (x, z) on the ground, facing angle.
static inline void ShrineOfTheSeaGod_ObjPlaceTurn(s32 actor, s32 x, s32 z,
                                                  u16 angle) {
  Actor_SetHorizontalPositionAndAngleWithTerrain(actor, x, z, angle);
}
#include "shared/tables.h"

/// The entrance hall's actors (SceneId_ShrineOfTheSeaGodRope), numbering the
/// rows of gShrineOfTheSeaGodRopeActorPlacements.
enum ShrineOfTheSeaGodHallActorId {
  /// Two logs floating on the hall's water.
  ACTOR_HALL_FIRST_LOG = ACTOR_FIRST_PLACED,
  ACTOR_HALL_SECOND_LOG,
  /// The poles of the rope Riki cannot throw up to Tavi, which Lash ties.
  ACTOR_HALL_ROPE_POLE,
  ACTOR_HALL_ROPE_OTHER_POLE,
  ACTOR_RIKI,
  /// Tavi, stuck on the ledge.
  ACTOR_TAVI,
  ACTOR_HALL_KRADEN
};

/// The torch Move pushes and its flame, the first two rows of the side room
/// (SceneId_ShrineOfTheSeaGodPlankBridge) and of the ledge room
/// (SceneId_ShrineOfTheSeaGodBreath).
enum ShrineOfTheSeaGodTorchActorId {
  ACTOR_TORCH = ACTOR_FIRST_PLACED,
  ACTOR_TORCH_FLAME
};

/// The rest of the side room's actors.
enum ShrineOfTheSeaGodSideRoomActorId {
  /// A pillar sunk into the floor.
  ACTOR_SIDE_ROOM_PILLAR = ACTOR_TORCH_FLAME + 1,
  /// Riki, who has come here looking for something to help Tavi with.
  ACTOR_SIDE_ROOM_RIKI
};

/// The great hall's actors (SceneId_ShrineOfTheSeaGodCrossroads), numbering the
/// rows of gShrineOfTheSeaGodCrossroadsActorPlacements.
enum ShrineOfTheSeaGodGreatHallActorId {
  /// The block Move pushes aside.
  ACTOR_GREAT_HALL_PUSHED_PILLAR = ACTOR_FIRST_PLACED,
  /// The hall's pillars, the first a broader one, drawn in front of or behind
  /// the leader by the level it stands on
  /// (ShrineOfTheSeaGod_LayerCrossroadsProps).
  ACTOR_GREAT_HALL_FIRST_PILLAR,
  ACTOR_GREAT_HALL_SECOND_PILLAR,
  ACTOR_GREAT_HALL_THIRD_PILLAR,
  ACTOR_GREAT_HALL_FOURTH_PILLAR,
  /// The stump left standing when the pillar above it topples into the water.
  ACTOR_GREAT_HALL_PILLAR_STUMP,
  ACTOR_GREAT_HALL_TOPPLING_PILLAR,
  /// Breath, the Jupiter Djinni, first seen here.
  ACTOR_GREAT_HALL_BREATH
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
  ACTOR_FIRST_PUDDLE = ACTOR_TORCH_FLAME + 1,
  ACTOR_SECOND_PUDDLE,
  ACTOR_THIRD_PUDDLE,
  ACTOR_LEDGE_ROOM_PILLAR,
  /// Breath, chased around the ledge until it is trapped.
  ACTOR_BREATH
};

/// The hole room's actors (SceneId_ShrineOfTheSeaGodPillarHole).
enum ShrineOfTheSeaGodHoleRoomActorId {
  /// The pillar Move pushes into the hole.
  ACTOR_HOLE_ROOM_PILLAR = ACTOR_FIRST_PLACED,
  /// Three actors the room shows or hides together
  /// (ShrineOfTheSeaGod_SetSlideActorsShown).
  ACTOR_FIRST_SLIDE,
  ACTOR_SECOND_SLIDE,
  ACTOR_THIRD_SLIDE
};

/// The pillar room's actors (SceneId_ShrineOfTheSeaGodPillars), numbering the
/// rows of gShrineOfTheSeaGodPillarsActorPlacements.
enum ShrineOfTheSeaGodPillarRoomActorId {
  /// The room's standing pillars, the fifth a broader one.
  ACTOR_PILLAR_ROOM_FIRST_PILLAR = ACTOR_FIRST_PLACED,
  ACTOR_PILLAR_ROOM_SECOND_PILLAR,
  ACTOR_PILLAR_ROOM_THIRD_PILLAR,
  ACTOR_PILLAR_ROOM_FOURTH_PILLAR,
  ACTOR_PILLAR_ROOM_FIFTH_PILLAR,
  /// The stumps the two toppling pillars leave, and the pillars themselves.
  ACTOR_FIRST_PILLAR_STUMP,
  ACTOR_SECOND_PILLAR_STUMP,
  ACTOR_FIRST_TOPPLING_PILLAR,
  ACTOR_SECOND_TOPPLING_PILLAR,
  /// The poles of the room's two ropes, which Lash ties.
  ACTOR_FIRST_ROPE_POLE,
  ACTOR_FIRST_ROPE_OTHER_POLE,
  ACTOR_SECOND_ROPE_POLE,
  ACTOR_SECOND_ROPE_OTHER_POLE,
  /// The logs each toppled pillar lands as, two to a pillar.
  ACTOR_FIRST_PILLAR_LOG,
  ACTOR_FIRST_PILLAR_OTHER_LOG,
  ACTOR_SECOND_PILLAR_LOG,
  ACTOR_SECOND_PILLAR_OTHER_LOG,
  /// Three markers near (456, 248); talking to one sets FieldWork.fieldMenu.
  /// XXX: what the player sees there; a render of the room from its east
  /// side, by (456, 248), would settle it.
  ACTOR_FIRST_MENU_MARKER,
  ACTOR_SECOND_MENU_MARKER,
  ACTOR_THIRD_MENU_MARKER
};

/// The island room's actors (SceneId_ShrineOfTheSeaGodSeaGodsTear): three logs
/// floating on the water the Sea God's Tear raises.
enum ShrineOfTheSeaGodIslandActorId {
  ACTOR_ISLAND_FIRST_LOG = ACTOR_FIRST_PLACED,
  ACTOR_ISLAND_SECOND_LOG,
  ACTOR_ISLAND_THIRD_LOG
};

/// Talking to Breath once the torch has it trapped starts its battle.
void ShrineOfTheSeaGod_FightBreath(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  u8 *state = &Actor_GetById(ACTOR_BREATH)->riseCounter;
  s32 y;

  if ((u8)(*state - 4) > 1)
    return;
  y = leader->y.fixed;
  if (y > px(12)) {
    GameFlag_Set(FLAG_FIGHTING_BREATH);
    if (*state == 4)
      GameFlag_Set(FLAG_BREATH_TRAPPED_EAST);
    else if (*state == 5)
      GameFlag_Set(FLAG_BREATH_TRAPPED_WEST);
    Field_RequestDjinnBattle(ACTOR_BREATH, DJINNI_BREATH);
  }
}

struct Actor;

extern u32 gShrineOfTheSeaGodStatueExitRegions[];
extern s16 gShrineOfTheSeaGodLaddersExitRegions[];
extern u32 gShrineOfTheSeaGodRightProngExitRegions[];

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

/// The regions of the rooms whose exits are rectangles: the three rooms the
/// water rises through.
const struct SceneRegion *Scene_GetExitRegions(void) {
  s16 scene = gMainRoutineWork.next;

  if (scene == SCENE_SHRINE_OF_THE_SEA_GOD_STATUE)
    return (const struct SceneRegion *)gShrineOfTheSeaGodStatueExitRegions;
  if (scene == SCENE_SHRINE_OF_THE_SEA_GOD_LADDERS)
    return (const struct SceneRegion *)gShrineOfTheSeaGodLaddersExitRegions;
  if (scene == SCENE_SHRINE_OF_THE_SEA_GOD_RIGHT_PRONG)
    return (const struct SceneRegion *)gShrineOfTheSeaGodRightProngExitRegions;
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

/// Examining the statue on the island: it looks like the one in Daila's
/// sanctum.
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

/// Talking to actors 25 to 27 opens the field menu, as talking to nothing does.
void ShrineOfTheSeaGod_OpenFieldMenu(void) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;

  work->fieldMenu = 1;
}

/// The island room's water rises a step, lifting the leader, the camera and the
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

  water->offsetY -= px(1) / 2;
  ground->offsetY += px(1) / 2;
  groundTop->offsetY += px(1) / 2;
  leader->z.fixed -= px(1) / 2;
  camera->y.fixed -= px(1) / 2;
  firstLog->z.fixed -= px(1) / 2;
  secondLog->z.fixed -= px(1) / 2;
  thirdLog->z.fixed -= px(1) / 2;
  wait(4);
}

/// Scrolls the water's layer up a pixel, then waits four frames.
void ShrineOfTheSeaGod_ScrollWaterUp(void) {
  struct MapWork *map = Ram_HeapSlots->mapGrid;
  struct MapLayer *layer = &map->layers[0];

  layer->offsetY += -px(1);
  wait(4);
}

/// Copies a 6 by 56 rectangle of cells from (x, y) to (18, 6), then
/// moves the first two layers' y by -0x2000 a frame for 16 frames.
void ShrineOfTheSeaGod_RaiseWaterBand(s32 x, s32 y) {
  struct MapWork *map = Ram_HeapSlots->mapGrid;
  struct MapLayer *water = &map->layers[0];
  struct MapLayer *ground = &map->layers[1];
  u8 i;

  MapGrid_CopyCellsTo(x, y, 6, 56, 18, 6);
  for (i = 0; i < 16; i++) {
    water->offsetY += -0x2000;
    ground->offsetY += -0x2000;
    wait(1);
  }
}

void BlendScript_Suspend(void);

/// The Sea God's Tear (item 458) used on the statue on the island, actor 10,
/// makes the water rise.
void ShrineOfTheSeaGod_UseSeaGodsTear(void) {
  u32 i;

  Event_Start();
  Event_ResolvePendingActions(0);
  BlendScript_Suspend();
  Actor_FaceDirectionById(gMainRoutineWork.leaderActorId, FACING_SOUTH, 0);
  FieldCamera_MoveWithinMapBounds(px(248), -1, px(168), 1);
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
  MapGrid_SetShakeFixed(0, px(1), Q16_ONE);
  MapGrid_CopyCellsTo(59, 124, 9, 12, 12, 3);
  Actor_GetById(gMainRoutineWork.leaderActorId)->motionFlags = 0;
  Actor_SetSpritePriority(gMainRoutineWork.leaderActorId, 1);
  Actor_SetSpritePriority(ACTOR_ISLAND_FIRST_LOG, 1);
  Actor_SetSpritePriority(ACTOR_ISLAND_SECOND_LOG, 1);
  Actor_SetSpritePriority(ACTOR_ISLAND_THIRD_LOG, 1);
  MapGrid_CopyCellsTo(46, 51, 6, 56, 18, 25);
  wait(10);
  MapGrid_SetShakeFixed(px(1), px(1), Q16_ONE);
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
  for (i = 0; i < 24; i++)
    ShrineOfTheSeaGod_ScrollWaterUp();
  for (i = 0; i < 24; i++)
    ShrineOfTheSeaGod_LiftWithWater();
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Event_SetExitRequest(3);
}

/// The island room's water falls a step, lowering the leader, the camera and
/// the floating logs with it.
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

  water->offsetY += px(1) / 2;
  ground->offsetY -= px(1) / 2;
  groundTop->offsetY -= px(1) / 2;
  leader->z.fixed += px(1) / 2;
  camera->y.fixed += px(1) / 2;
  firstLog->z.fixed += px(1) / 2;
  secondLog->z.fixed += px(1) / 2;
  thirdLog->z.fixed += px(1) / 2;
  wait(4);
}

/// Scrolls the water's layer down a pixel, then waits four frames.
void ShrineOfTheSeaGod_ScrollWaterDown(void) {
  struct MapWork *map = Ram_HeapSlots->mapGrid;
  struct MapLayer *layer = &map->layers[0];

  layer->offsetY += px(1);
  wait(4);
}

/// Copies a rectangle of cells from (x, y), then raises map layers 0 and 1 by
/// 0x2000 a frame for 16 frames.
void ShrineOfTheSeaGod_LowerWaterBand(s32 x, s32 y) {
  struct MapWork *map = Ram_HeapSlots->mapGrid;
  struct MapLayer *water = &map->layers[0];
  struct MapLayer *ground = &map->layers[1];
  u8 i;

  MapGrid_CopyCellsTo(x, y, 6, 56, 18, 6);
  i = 0;
loop:
  water->offsetY += 0x2000;
  ground->offsetY += 0x2000;
  wait(1);
  i++;
  if (i <= 15)
    goto loop;
}

/// Back on the island after the second Tear, the water sinks to where it was.
void ShrineOfTheSeaGod_LowerWater(void) {
  struct MapWork *map = Ram_HeapSlots->mapGrid;
  struct MapLayer *water = &map->layers[0];
  struct MapLayer *ground = &map->layers[1];
  struct MapLayer *groundTop = &map->layers[2];
  struct Actor *leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  struct Actor *ride = FieldCamera_GetActor();
  struct Actor *a = Actor_GetById(ACTOR_ISLAND_FIRST_LOG);
  struct Actor *b = Actor_GetById(ACTOR_ISLAND_SECOND_LOG);
  struct Actor *c = Actor_GetById(ACTOR_ISLAND_THIRD_LOG);
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
  water->offsetY += -px(68);
  ground->offsetY += -px(20);
  groundTop->offsetY += px(12);
  leader->z.fixed += -px(12);
  ride->y.fixed += -px(12);
  a->z.fixed += -px(12);
  b->z.fixed += -px(12);
  c->z.fixed += -px(12);
  MapGrid_CenterOnFollowedPoint();
  wait(4);
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  MapGrid_SetShakeFixed(px(1), px(1), Q16_ONE);
  Sound_Play(SE_245);
  i = 0;
  do {
    i++;
    ShrineOfTheSeaGod_LowerWithWater();
  } while (i <= 24);
  i = 0;
  do {
    i++;
    ShrineOfTheSeaGod_ScrollWaterDown();
  } while (i <= 24);
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
  MapGrid_SetShakeFixed(0, px(1), Q16_ONE);
  Io_Write16(BLEND_ALPHA(4, 16), &REG_BLDALPHA);
  wait(10);
  Sound_Play(SOUND_FADE_OUT_ACTION_SE);
  MapGrid_SetShakeFixed(-1, -1, 9 * Q16_ONE / 10);
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
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
  ShrineOfTheSeaGod_SetUpFlatProp(ACTOR_ISLAND_FIRST_LOG);
  ShrineOfTheSeaGod_SetUpFlatProp(ACTOR_ISLAND_SECOND_LOG);
  ShrineOfTheSeaGod_SetUpFlatProp(ACTOR_ISLAND_THIRD_LOG);
  Actor_GetById(gMainRoutineWork.leaderActorId)->priorityFlags |= 1;
  leader->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  leader->y.fixed = 0;
  leader->groundHeight = 0;
  Event_End();
}

extern s16 gShrineOfTheSeaGod_MovableActors[];

/// The leader is placed 50 pixels down and map layer 1 with it; then both rise
/// half a pixel every two frames, 100 times, as a screen transition starts.
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
  FieldCamera_MoveWithinMapBounds(px(344), -1, px(248), 0);
  MapGrid_CopyCellsTo(62, 77, 62, 18, 19, 6);
  wait(1);
  Actor_SetSpritePriority(gMainRoutineWork.leaderActorId, 2);
  leader->motionFlags = 0;
  leader->y.fixed = px(-50);
  leader->groundHeight = px(-50);
  layer->offsetY += px(-50);
  MapGrid_CenterOnFollowedPoint();
  wait(1);
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 3);
  Event_StartScreenTransition();
  MapGrid_SetShakeFixed(px(1), px(1), Q16_ONE);
  for (i = 0; i < 100; i++) {
    leader->y.fixed += px(1) / 2;
    leader->groundHeight += px(1) / 2;
    layer->offsetY += px(1) / 2;
    wait(2);
  }
  Sound_Play(SE_STOP_ACTION_SE);
  Sound_Play(SOUND_FADE_OUT_ACTION_SE);
  MapGrid_SetScreenShake(0, 0, 0);
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
  Actor_GetById(gMainRoutineWork.leaderActorId)->priorityFlags |= 1;
  MapGrid_CopyCellsTo(62, 69, 62, 18, 19, 6);
  wait(1);
  Event_End();
}

/// The leader turns, then moves with map layer 1 by -0x8000 in y a frame for
/// 100 frames; at frame 90 the screen transition finishes.
void ShrineOfTheSeaGod_SinkWithWater(void) {
  struct MapWork *map = Ram_HeapSlots->mapGrid;
  struct MapLayer *layer;
  struct Actor *object;
  u32 i;

  layer = &map->layers[1];
  object = Actor_GetById(gMainRoutineWork.leaderActorId);

  Event_Start();
  Event_ResolvePendingActions(0);
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  FieldCamera_MoveWithinMapBounds(px(344), -1, px(248), 1);
  MapGrid_CopyCellsTo(62, 77, 62, 18, 19, 6);
  Actor_FaceDirectionById(gMainRoutineWork.leaderActorId, FACING_SOUTH, 20);
  Actor_SetSpritePriority(gMainRoutineWork.leaderActorId, 2);
  object->motionFlags = 0;
  Sound_Play(SE_245);
  MapGrid_SetShakeFixed(px(1), px(1), Q16_ONE);
  i = 0;
loop:
  object->y.fixed -= px(1) / 2;
  object->groundHeight -= px(1) / 2;
  layer->offsetY -= px(1) / 2;
  wait(2);
  if (i == 90) {
    gFieldWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 3);
    Event_FinishScreenTransition();
  }
  i++;
  if (i <= 99)
    goto loop;
  Event_SetExitRequest(1);
}

/// Sets up a puddle Frost can freeze, disabled until it does.
void ShrineOfTheSeaGod_SetUpPuddle(s32 actor) {
  Actor_DisableById(actor);
  Actor_GetById(actor)->priorityFlags |= ACTOR_PRIORITY_IGNORES_RISES;
}

/// Frost on the first puddle turns it to an ice pillar.
void ShrineOfTheSeaGod_FreezeFirstPuddle(void) {
  MapGrid_CopyCellAttributes(0, 0, 1, 1, 8, 18);
  GameFlag_Set(FLAG_FIRST_PUDDLE_FROZEN);
}

/// Frost on the second puddle turns it to an ice pillar.
void ShrineOfTheSeaGod_FreezeSecondPuddle(void) {
  MapGrid_CopyCellAttributes(0, 0, 1, 1, 10, 18);
  GameFlag_Set(FLAG_SECOND_PUDDLE_FROZEN);
}

/// Frost on the third puddle turns it to an ice pillar.
void ShrineOfTheSeaGod_FreezeThirdPuddle(void) {
  MapGrid_CopyCellAttributes(0, 0, 1, 1, 10, 17);
  GameFlag_Set(FLAG_THIRD_PUDDLE_FROZEN);
}

/// Move lifts the room's torch, or in the great hall its pushed pillar.
void ShrineOfTheSeaGod_LiftBlock(void) {
  Actor_LiftTargetBlock(gShrineOfTheSeaGod_MovableActors);
}

/// Sets the torch down, noting whether it stands against the north wall where
/// it traps Breath.
void ShrineOfTheSeaGod_SetDownTorch(void) {
  struct Actor *object;

  Actor_PlaceMovedBlock();
  object = Actor_GetById(ACTOR_TORCH);
  if (FIELD_CELL(object->x.fixed) == 21 && FIELD_CELL(object->z.fixed) == 13)
    GameFlag_Set(FLAG_TORCH_AT_NORTH_WALL);
  else
    GameFlag_Clear(FLAG_TORCH_AT_NORTH_WALL);
}

/// Sets the great hall's pushed pillar down; pushed all the way west, it stays
/// there.
void ShrineOfTheSeaGod_MovePillar(void) {
  struct Actor *object = Actor_GetById(ACTOR_GREAT_HALL_PUSHED_PILLAR);

  Actor_PlaceMovedBlock();
  if (FIELD_CELL(object->x.fixed) == 12)
    GameFlag_Set(FLAG_CROSSROADS_PILLAR_MOVED);
}

/// Lets a pillar fall to the ground with a thump.
void ShrineOfTheSeaGod_DropPillar(struct Actor *object) {
  object->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  do {
    wait(1);
  } while (object->velocityY != 0);
  Sound_Play(SE_THUMP);
  wait(10);
  object->motionFlags = 0;
}

/// Pushed onto the hole, the pillar drops in and fills it.
void ShrineOfTheSeaGod_PushPillarIntoHole(void) {
  struct Actor *object = Actor_GetById(ACTOR_HOLE_ROOM_PILLAR);
  s32 z = FIELD_CELL(object->z.fixed);

  if (z == 16) {
    Event_Start();
    Event_ResolvePendingActions(0);
    ShrineOfTheSeaGod_DropPillar(object);
    Map_CopyCellAttributeRect(0, 0, 1, 1, 15, z);
    Actor_SetSpritePriority(ACTOR_HOLE_ROOM_PILLAR, 3);
    Event_End();
    GameFlag_Set(FLAG_PILLAR_IN_HOLE);
  }
}

/// The great hall's task: its pillars are drawn behind the leader on the upper
/// level, in front of it below.
void ShrineOfTheSeaGod_LayerCrossroadsProps(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  u32 id;

  for (id = 9; id <= 13; id++) {
    struct Actor *object = Actor_GetById(id);

    if (leader->y.fixed > px(16)) {
      Actor_SetSpritePriority(id, 2);
      object->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
    } else {
      Actor_SetSpritePriority(id, 1);
      object->priorityFlags &= ~ACTOR_PRIORITY_UNDERFOOT;
    }
  }
}

/// Notes whether the leader stands on the upper level of the room.
void ShrineOfTheSeaGod_TrackUpperLevel(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.leaderActorId);

  Actor_GetCellIndex(leader);
  if (leader->y.fixed < px(16))
    gMainRoutineWork.onUpperLevel = 0;
  else
    gMainRoutineWork.onUpperLevel = 1;
}

/// The ledge room's task: the puddles are drawn behind or in front of the
/// leader by where it stands.
void ShrineOfTheSeaGod_LayerPuddles(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  s32 y;
  s32 z;

  y = leader->y.fixed;
  if (y < px(16)) {
    z = leader->z.fixed;
    if (z < px(296)) {
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

/// Keeps the torch's flame on top of the torch as Move carries it.
void ShrineOfTheSeaGod_KeepFlameOnTorch(void) {
  struct Actor *first = Actor_GetById(ACTOR_TORCH);
  struct Actor *second = Actor_GetById(ACTOR_TORCH_FLAME);

  SET_ACTOR_POSITION(second, first->x.fixed, px(20), first->z.fixed);
}

// XXX: what priority flag 0x20 does to the leader, which the slide room sets on
// arrival; a render of the slide room with and without it would settle it.
UNUSED void ShrineOfTheSeaGod_ClearLeaderPriorityBit(void) {
  Actor_GetById(gMainRoutineWork.leaderActorId)->priorityFlags &=
      ~ACTOR_PRIORITY_IGNORES_RISES;
}

/// Sets the leader's priority bit 0x20.
UNUSED void ShrineOfTheSeaGod_SetLeaderPriorityBit(void) {
  Actor_GetById(gMainRoutineWork.leaderActorId)->priorityFlags |=
      ACTOR_PRIORITY_IGNORES_RISES;
}

void ShrineOfTheSeaGod_SetActorsHidden(s32 show, s32 first, u32 count);

/// In the hole room, shows or hides its three slides.
void ShrineOfTheSeaGod_SetSlideActorsShown(s32 show) {
  if (gMainRoutineWork.next == SCENE_SHRINE_OF_THE_SEA_GOD_PILLAR_HOLE)
    ShrineOfTheSeaGod_SetActorsHidden(show, 9, 3);
}

/// Shows the hole room's slides.
UNUSED void ShrineOfTheSeaGod_ShowSlideActors(void) {
  ShrineOfTheSeaGod_SetSlideActorsShown(1);
  GameFlag_Set(FLAG_SLIDES_SHOWN);
}

/// Hides the hole room's slides.
void ShrineOfTheSeaGod_HideSlideActors(void) {
  ShrineOfTheSeaGod_SetSlideActorsShown(0);
  GameFlag_Clear(FLAG_SLIDES_SHOWN);
}

/// Shows or hides count actors from first.
void ShrineOfTheSeaGod_SetActorsHidden(s32 show, s32 first, u32 count) {
  u32 i;

  for (i = 0; i < count; i++) {
    struct Actor *actor = Actor_GetById(first + i);

    if (show)
      actor->priorityFlags &= ~ACTOR_PRIORITY_NOT_DRAWN;
    else
      actor->priorityFlags |= ACTOR_PRIORITY_NOT_DRAWN;
  }
}

/// Stepping out in front of the great hall's pillars, the leader is drawn in
/// front of them.
void ShrineOfTheSeaGod_DrawLeaderInFront(void) {
  Actor_SetSpritePriority(gMainRoutineWork.leaderActorId, 1);
  GameFlag_Set(FLAG_LEADER_IN_FRONT);
}

/// Stepping back behind the great hall's pillars, the leader is drawn behind
/// them.
void ShrineOfTheSeaGod_DrawLeaderBehind(void) {
  Actor_SetSpritePriority(gMainRoutineWork.leaderActorId, 2);
  Actor_GetById(gMainRoutineWork.leaderActorId)->priorityFlags |= 1;
  GameFlag_Clear(FLAG_LEADER_IN_FRONT);
}

#include "../field/field_effect_advance_motion.c"

#undef EFFECT_SPAWN
#undef EFFECT_CREATE
#undef EFFECT_SET_MODE
#undef EFFECT_SET_SCRIPT
#undef EFFECT_SCRIPTS

extern u32 gShrineOfTheSeaGod_PillarOffset[];

/// The leader rocks a pillar back and forth, then it is carried along with him.
void ShrineOfTheSeaGod_RockPillar(struct Actor *object, struct Actor *other) {
  struct FieldSprite *sprite = object->sprite;
  u32 n;
  u32 i;

  FieldEffect_SetAttached(gMainRoutineWork.leaderActorId,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Sound_Play(SE_HIDDEN_PASSAGE_OPEN);
  object->motionFlags = 0;
  for (n = 0; n <= 1; n++) {
    other->y.fixed -= px(1);
    for (i = 0; i <= 3; i++) {
      sprite->rotation += 128;
      wait(1);
    }
    other->y.fixed += px(1);
    for (i = 0; i <= 7; i++) {
      sprite->rotation -= 128;
      wait(1);
    }
    other->y.fixed -= px(1);
    for (i = 0; i <= 3; i++) {
      sprite->rotation += 128;
      wait(1);
    }
  }
  other->y.fixed += px(1);
  wait(20);
  gShrineOfTheSeaGod_PillarOffset[0] = other->x.fixed - object->x.fixed;
  gShrineOfTheSeaGod_PillarOffset[1] = other->y.fixed - object->y.fixed;
  gShrineOfTheSeaGod_PillarOffset[2] = other->z.fixed - object->z.fixed;
  Actor_SetSpriteFlags(Actor_GetById(gMainRoutineWork.leaderActorId), 0);
}

/// Keeps a toppling pillar where it was against the leader as the leader moves.
void ShrineOfTheSeaGod_KeepWithLeader(struct Actor *object) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  s32 *offset = gShrineOfTheSeaGod_PillarOffset;

  SET_ACTOR_POSITION(object, leader->x.fixed + offset[0],
                     leader->y.fixed + offset[1], leader->z.fixed + offset[2]);
}

/// The task that carries the great hall's toppling pillar along with the
/// leader.
void ShrineOfTheSeaGod_KeepCrossroadsPillarWithLeader(void) {
  ShrineOfTheSeaGod_KeepWithLeader(
      Actor_GetById(ACTOR_GREAT_HALL_TOPPLING_PILLAR));
}

void FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                       s32 velocityZ, u32 flags,
                       const struct EffectOptions *options);

/// The leader rocks the great hall's pillar until it topples into the water,
/// riding it down.
void ShrineOfTheSeaGod_TopplePillarIntoWater(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  struct Actor *stone = Actor_GetById(ACTOR_GREAT_HALL_TOPPLING_PILLAR);
  struct FieldSprite *sprite = stone->sprite;
  struct EffectOptions options;
  u32 i;

  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_WalkToAndWaitById(gMainRoutineWork.leaderActorId, 408, 328);
  ShrineOfTheSeaGod_RockPillar(leader, stone);
  MapGrid_CopyCellAttributes(0, 50, 3, 1, 24, 70);
  GameFlag_Set(FLAG_CROSSROADS_PILLAR_TOPPLED);
  wait(1);
  leader->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  Actor_SetSpritePriority(ACTOR_GREAT_HALL_TOPPLING_PILLAR, 3);
  stone->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
  FieldTask_AddFrame(ShrineOfTheSeaGod_KeepCrossroadsPillarWithLeader,
                     TASK_PASS_VBLANK);
  do
    wait(1);
  while (leader->velocityY != 0);
  Sound_Play(SE_TOPPLE);
  MapGrid_SetShakeFixed(px(1), px(2), Q16_ONE);
  options.palette = 7;
  options.update = FieldEffect_AdvanceMotion;
  options.startScaleX = 0xa3d7;
  options.startScaleY = 0xa3d7;
  for (i = 0; i <= 16;) {
    s32 velocity[3];

    velocity[0] = Trig_Cos(i << 12);
    velocity[1] = 0;
    velocity[2] = Trig_Sin(i << 12);
    velocity[0] += velocity[0] >> 1;
    velocity[0] += ((Random16() * 3) << 14) >> 16;
    i++;
    velocity[0] += -0x6000;
    velocity[2] += (((Random16() * 3) << 13) >> 16) + -0x3000;
    FieldEffect_Spawn(leader->x.fixed, leader->y.fixed + px(-4),
                      leader->z.fixed, velocity[0], velocity[1], velocity[2],
                      0x01090001, &options);
  }
  leader->velocityY = px(4);
  wait(2);
  MapGrid_SetShakeFixed(-1, -1, 9 * Q16_ONE / 10);
  Task_Remove(ShrineOfTheSeaGod_KeepCrossroadsPillarWithLeader);
  for (i = 0; i < 8; i++) {
    sprite->rotation += 0x200;
    stone->z.fixed += px(1);
    wait(1);
  }
  MapGrid_WaitForShakeToSettle();
  Actor_SetSpriteFlags(Actor_GetById(gMainRoutineWork.leaderActorId), 1);
  leader->layer = 0;
  wait(10);
  Event_End();
}

/// Waits for the toppling pillar to land in the water, then lays the pair of
/// logs it becomes from base.
void ShrineOfTheSeaGod_LandPillarAsLogs(struct Actor *object, s32 base) {
  do {
    wait(1);
  } while (object->velocityY != 0);
  Sound_Play(SE_LOG_FALL);
  Actor_SetSpritePriority(base + 21, 3);
  Actor_SetSpritePriority(base + 22, 3);
  Actor_SetPositionAndResetMotion(Actor_GetById(base + 21), object->x.fixed, 0,
                                  object->z.fixed);
  Actor_SetPositionAndResetMotion(Actor_GetById(base + 22), object->x.fixed, 0,
                                  object->z.fixed + px(-16));
  Actor_SetAnimationById(base + 21, 1);
  Actor_SetAnimationById(base + 22, 1);
  wait(20);
}

/// The task that carries the pillar room's first toppling pillar along with the
/// leader.
void ShrineOfTheSeaGod_KeepFirstPillarWithLeader(void) {
  ShrineOfTheSeaGod_KeepWithLeader(Actor_GetById(ACTOR_FIRST_TOPPLING_PILLAR));
}

/// The leader rocks the pillar room's first pillar until it topples into the
/// water, riding it down and across.
void ShrineOfTheSeaGod_ToppleFirstPillar(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  struct Actor *pillar = Actor_GetById(ACTOR_FIRST_TOPPLING_PILLAR);

  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_WalkToAndWaitById(gMainRoutineWork.leaderActorId, 328, 216);
  ShrineOfTheSeaGod_RockPillar(leader, pillar);
  Map_CopyCellAttributeRect(0, 0, 3, 1, 19, 13);
  Map_CopyCellAttributeRect(40, 0, 2, 1, 19, 33);
  wait(1);
  leader->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  Actor_SetSpritePriority(ACTOR_FIRST_TOPPLING_PILLAR, 3);
  pillar->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
  FieldTask_AddFrame(ShrineOfTheSeaGod_KeepFirstPillarWithLeader,
                     TASK_PASS_VBLANK);
  ShrineOfTheSeaGod_LandPillarAsLogs(leader, 0);
  Actor_SetSpritePriority(ACTOR_PILLAR_ROOM_SECOND_PILLAR, 1);
  Actor_GetById(ACTOR_PILLAR_ROOM_SECOND_PILLAR)->priorityFlags &=
      ~ACTOR_PRIORITY_UNDERFOOT;
  Actor_SetSpritePriority(ACTOR_PILLAR_ROOM_FOURTH_PILLAR, 1);
  Actor_GetById(ACTOR_PILLAR_ROOM_FOURTH_PILLAR)->priorityFlags &=
      ~ACTOR_PRIORITY_UNDERFOOT;
  leader->motionFlags = 0;
  Actor_SetSpriteFlags(Actor_GetById(gMainRoutineWork.leaderActorId), 1);
  Actor_SetSpeedAndAcceleration(gMainRoutineWork.leaderActorId, 0.6, 0.1);
  Actor_MoveToAndWaitById(gMainRoutineWork.leaderActorId, 328, 224);
  Actor_MoveToAndWait(gMainRoutineWork.leaderActorId, 321, 256);
  Actor_SetHorizontalPositionWithTerrain(ACTOR_FIRST_PILLAR_LOG, 0, 0);
  Actor_SetHorizontalPositionWithTerrain(ACTOR_FIRST_PILLAR_OTHER_LOG, 0, 0);
  Actor_MoveToAndWait(gMainRoutineWork.leaderActorId, 321, 320);
  Actor_MoveToAndWait(gMainRoutineWork.leaderActorId, 321, 400);
  Actor_MoveToAndWait(gMainRoutineWork.leaderActorId, 321, 480);
  Actor_MoveToAndWaitById(gMainRoutineWork.leaderActorId, 321, 536);
  Sound_Play(SE_RATTLE);
  Actor_SetSpritePriority(ACTOR_PILLAR_ROOM_SECOND_PILLAR, 2);
  Actor_GetById(ACTOR_PILLAR_ROOM_SECOND_PILLAR)->priorityFlags |=
      ACTOR_PRIORITY_UNDERFOOT;
  Actor_SetSpritePriority(ACTOR_PILLAR_ROOM_FOURTH_PILLAR, 2);
  Actor_GetById(ACTOR_PILLAR_ROOM_FOURTH_PILLAR)->priorityFlags |=
      ACTOR_PRIORITY_UNDERFOOT;
  leader->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  Task_Remove(ShrineOfTheSeaGod_KeepFirstPillarWithLeader);
  wait(1);
  Event_End();
  GameFlag_Set(FLAG_FIRST_PILLAR_TOPPLED);
}

/// The task that carries the pillar room's second toppling pillar along with
/// the leader.
void ShrineOfTheSeaGod_KeepSecondPillarWithLeader(void) {
  ShrineOfTheSeaGod_KeepWithLeader(Actor_GetById(ACTOR_SECOND_TOPPLING_PILLAR));
}

/// The leader rocks the pillar room's second pillar until it topples into the
/// water, riding it down and across.
void ShrineOfTheSeaGod_ToppleSecondPillar(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  struct Actor *pillar = Actor_GetById(ACTOR_SECOND_TOPPLING_PILLAR);

  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_WalkToAndWait(gMainRoutineWork.leaderActorId, 408, 360);
  ShrineOfTheSeaGod_RockPillar(leader, pillar);
  Map_CopyCellAttributeRect(0, 0, 3, 1, 24, 22);
  Map_CopyCellAttributeRect(40, 0, 2, 1, 24, 24);
  wait(1);
  leader->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  Actor_SetSpritePriority(ACTOR_SECOND_TOPPLING_PILLAR, 3);
  pillar->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
  FieldTask_AddFrame(ShrineOfTheSeaGod_KeepSecondPillarWithLeader,
                     TASK_PASS_VBLANK);
  ShrineOfTheSeaGod_LandPillarAsLogs(leader, 2);
  Actor_SetSpriteFlags(Actor_GetById(gMainRoutineWork.leaderActorId), 1);
  Actor_SetSpeedAndAcceleration(gMainRoutineWork.leaderActorId, 0.6, 0.1);
  Actor_MoveToAndWaitById(gMainRoutineWork.leaderActorId, 406, 360);
  Actor_MoveToAndWaitById(gMainRoutineWork.leaderActorId, 400, 392);
  Sound_Play(SE_RATTLE);
  Actor_SetHorizontalPositionWithTerrain(ACTOR_SECOND_PILLAR_LOG, 0, 0);
  Actor_SetHorizontalPositionWithTerrain(ACTOR_SECOND_PILLAR_OTHER_LOG, 0, 0);
  Task_Remove(ShrineOfTheSeaGod_KeepSecondPillarWithLeader);
  wait(1);
  Event_End();
  GameFlag_Set(FLAG_SECOND_PILLAR_TOPPLED);
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

/// Back in the entrance hall, Riki tries to throw the rope up to Tavi again.
void ShrineOfTheSeaGod_ThrowRopeAgain(void) {
  struct Actor *leader;

  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgFieldRopeThrownAgain);
  // "All right! I'm gonna throw the rope again!"
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_RIKI, 0);
  Actor_SetSpeed(gMainRoutineWork.leaderActorId, 0.8);
  Actor_SetSpeed(ACTOR_HALL_KRADEN, 0.8);
  Actor_WalkToAndWait(gMainRoutineWork.leaderActorId, 272, 336);
  Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_NORTH, 0);
  Actor_SetHorizontalPositionWithTerrain(ACTOR_HALL_KRADEN, px(272), px(336));
  wait(1);
  Actor_WalkToAndWait(ACTOR_HALL_KRADEN, 256, 336);
  Actor_FaceDirectionById(ACTOR_HALL_KRADEN, FACING_EAST, 0);
  Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_WEST, 40);
  Actor_FaceDirection(ACTOR_HALL_KRADEN, FACING_NORTH_NORTHEAST, 0);
  Actor_FaceDirectionById(gMainRoutineWork.leaderActorId, FACING_NORTH, 0);
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  FieldCamera_SetSpeed(4.8);
  FieldCamera_MoveTo(288, CAMERA_KEEP, 126, 1);
  FieldCamera_WaitForArrival();
  Actor_Jump(ACTOR_TAVI, 4, 20);
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_TAVI, 0);
  Actor_SetAnimationAndWait(ACTOR_RIKI, 3);
  Event_Wait(20);
  Actor_RunRepeatedMotion(ACTOR_RIKI, 2);
  Actor_Jump(ACTOR_RIKI, 6, 80);
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_TAVI, 0);
  Actor_SetAnimationById(ACTOR_RIKI, 4);
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_RIKI, 0);
  Actor_FaceDirection(ACTOR_TAVI, FACING_SOUTH_SOUTHEAST, 0);
  FieldActor_ShowEmote(ACTOR_TAVI, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 20);
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_TAVI, 0);
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_RIKI, 0);
  Actor_FaceDirectionById(ACTOR_TAVI, FACING_EAST, 0);
  FieldEffect_SetAttached(ACTOR_TAVI,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(20);
  Event_ShowMessageAndWait(SPEAKER_WINDOW_BELOW | ACTOR_TAVI, 0);
  Actor_SetAnimationAndWait(ACTOR_RIKI, 3);
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
  FieldCamera_MoveWithinMapBounds(px(264), -1, px(320), 1);
  FieldCamera_WaitForArrival();
  Actor_FaceDirectionById(gMainRoutineWork.leaderActorId, FACING_WEST, 0);
  Actor_FaceDirectionById(ACTOR_HALL_KRADEN, FACING_EAST, 20);
  Actor_SetAnimationById(gMainRoutineWork.leaderActorId, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_HALL_KRADEN, 3);
  Actor_SetAnimationById(ACTOR_HALL_KRADEN, 2);
  leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  if (leader != NULL)
    Actor_SetDestinationById(ACTOR_HALL_KRADEN, ((s16 *)&leader->x)[1],
                             ((s16 *)&leader->z)[1]);
  Actor_WaitForMoveById(ACTOR_HALL_KRADEN);
  Actor_SetHorizontalPositionWithTerrain(ACTOR_HALL_KRADEN, 0, 0);
  Actor_SetHorizontalPositionWithTerrain(ACTOR_TAVI, 0, 0);
  GameFlag_Set(FLAG_RIKI_GAVE_UP);
  Event_End();
}

extern u32 gShrineOfTheSeaGod_BoysGoHome[];

/// Lash ties the boys' rope, Tavi climbs down and both meet the party.
void ShrineOfTheSeaGod_LashRope(void) {
  struct Actor *object;
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
  Actor_SetSpeed(ACTOR_HALL_KRADEN, 0.8);
  Actor_WalkToAndWait(gMainRoutineWork.leaderActorId, 344, 184);
  Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_SOUTHEAST, 0);
  Actor_SetPosition(ACTOR_HALL_KRADEN, 344, 184);
  wait(1);
  Actor_WalkToAndWait(ACTOR_HALL_KRADEN, 356, 196);
  Actor_FaceDirectionAndWait(ACTOR_HALL_KRADEN, FACING_NORTH_NORTHWEST);
  Event_SetMessageId((s32)MsgFieldPraiseRopeWork);
  Event_ShowMessage(ACTOR_HALL_KRADEN, 0);
  Actor_SetAnimationAndWait(gMainRoutineWork.leaderActorId, ANIM_NOD);
  Event_ShowMessage(ACTOR_TAVI, 0);
  Actor_SetPosition(ACTOR_TAVI, 184, 168);
  Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_HALL_KRADEN, FACING_WEST, 0);
  FieldCamera_SetSpeed(4.8);
  FieldCamera_MoveTo(232, 64, 176, 1);
  Actor_SetSpeed(ACTOR_TAVI, 1.6);
  Actor_WalkToAndWait(ACTOR_TAVI, 232, 168);
  Event_Wait(20);
  Event_ShowMessage(ACTOR_TAVI, 0);
  FieldCamera_SetSpeed(1.2);
  FieldCamera_MoveTo(348, CAMERA_KEEP, 216, 1);
  object = Actor_GetById(ACTOR_TAVI);
  sprite = object->sprite;
  Actor_SetSpritePriority(ACTOR_TAVI, 1);
  Actor_SetSpeed(ACTOR_TAVI, 1.6);
  Actor_Jump(ACTOR_TAVI, 4, 0);
  Actor_MoveToAndWait(ACTOR_TAVI, 248, 168);
  Actor_SetAnimationById(ACTOR_TAVI, 6);
  object->scaleX = -Q16_ONE;
  sprite->rotation = 0xd000;
  object->motionFlags = 0;
  object->y.fixed += px(6);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_TAVI), 0);
  Actor_SetSpeed(ACTOR_TAVI, 0.4);
  FieldActor_SetMoveTarget(object, 328, 38, 168);
  do {
    Actor_WaitForMoveById(ACTOR_TAVI);
  } while (0);
  object->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  object->groundHeight = px(32);
  object->scaleX = Q16_ONE;
  sprite->rotation = 0;
  Actor_SetAnimationById(ACTOR_TAVI, 1);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_TAVI), 1);
  Actor_SetSpeed(ACTOR_TAVI, 1.6);
  Actor_Jump(ACTOR_TAVI, 4, 0);
  Actor_MoveToAndWait(ACTOR_TAVI, 328, 184);
  Actor_FaceDirection(ACTOR_TAVI, 0, 20);
  FieldActor_ShowEmote(ACTOR_TAVI, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  Actor_SetSpritePriority(ACTOR_TAVI, 2);
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_TAVI, 0);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_HALL_KRADEN, 0);
  FieldActor_ShowEmote(ACTOR_TAVI, EMOTE_IN_FRONT | EMOTE_QUESTION, 20);
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_TAVI, 0);
  Actor_FaceDirection(ACTOR_HALL_KRADEN, FACING_NORTH_NORTHWEST, 40);
  Actor_FaceDirection(ACTOR_HALL_KRADEN, FACING_WEST, 0);
  Event_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_HALL_KRADEN, 0, 60);
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_TAVI, 0);
  Actor_SetAnimationById(ACTOR_TAVI, 3);
  Actor_SetPosition(ACTOR_RIKI, 394, 286);
  wait(1);
  Actor_SetSpeed(ACTOR_RIKI, 1.6);
  Actor_WalkToAndWait(ACTOR_RIKI, 394, 248);
  Actor_FaceDirection(ACTOR_RIKI, FACING_NORTH_NORTHWEST, 0);
  Actor_Jump(ACTOR_RIKI, 4, 20);
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_RIKI, 0);
  FieldActor_ShowEmote(ACTOR_TAVI, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_FaceDirection(ACTOR_TAVI, FACING_SOUTH_SOUTHEAST, 0);
  Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_HALL_KRADEN, FACING_SOUTH_SOUTHEAST, 0);
  FieldCamera_SetSpeed(4.8);
  FieldCamera_MoveTo(376, CAMERA_KEEP, 264, 1);
  FieldCamera_WaitForArrival();
  Event_ShowMessage(ACTOR_RIKI, 0);
  FieldCamera_SetSpeed(2.4);
  FieldCamera_MoveTo(348, CAMERA_KEEP, 216, 1);
  Actor_WatchAndSetScript(gMainRoutineWork.leaderActorId, ACTOR_RIKI);
  Actor_WatchAndSetScript(ACTOR_HALL_KRADEN, ACTOR_RIKI);
  Actor_WatchAndSetScript(ACTOR_TAVI, ACTOR_RIKI);
  Actor_WalkToAndWait(ACTOR_RIKI, 394, 220);
  Actor_WalkToAndWait(ACTOR_RIKI, 370, 198);
  Actor_SetSpeed(ACTOR_HALL_KRADEN, 1.6);
  Actor_MoveToAndWait(ACTOR_HALL_KRADEN, 368, 180);
  Actor_WatchAndSetScript(ACTOR_HALL_KRADEN, ACTOR_RIKI);
  Actor_WalkToAndWait(ACTOR_RIKI, 336, 198);
  Actor_FaceDirection(ACTOR_RIKI, FACING_NORTH_NORTHWEST, 0);
  Actor_Stop(ACTOR_TAVI);
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_TAVI, 0);
  Actor_FaceDirectionById(ACTOR_RIKI, FACING_NORTH_NORTHEAST, 0);
  Event_ShowMessage(ACTOR_RIKI, 0);
  Actor_FaceDirectionAndWait(ACTOR_RIKI, FACING_NORTH_NORTHWEST);
  Actor_Jump(ACTOR_RIKI, 4, 20);
  Event_ShowMessageThenWait(ACTOR_RIKI, 0, 20);
  FieldActor_ShowEmote(ACTOR_TAVI, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
  FieldActor_ShowEmote(ACTOR_RIKI, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  Event_ShowMessage(ACTOR_RIKI, 0);
  Actor_SetAnimationById(ACTOR_TAVI, 4);
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_TAVI, 0);
  FieldActor_ShowEmote(ACTOR_RIKI, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  Event_ShowMessage(ACTOR_RIKI, 0);
  Actor_FaceDirection(ACTOR_TAVI, FACING_WEST, 20);
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_TAVI, 0);
  Actor_FaceDirection(ACTOR_RIKI, FACING_WEST, 0);
  Event_ShowMessage(ACTOR_RIKI, 0);
  Actor_SetAnimationAndWait(ACTOR_TAVI, 4);
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_TAVI, 0);
  FieldActor_ShowEmote(ACTOR_RIKI, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 40);
  Event_ShowMessage(ACTOR_RIKI, 0);
  FieldActor_ShowEmote(ACTOR_RIKI, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  FieldActor_ShowEmote(ACTOR_TAVI, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_FaceDirection(ACTOR_RIKI, FACING_NORTH_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_TAVI, FACING_SOUTH_SOUTHEAST, 40);
  Actor_Jump(ACTOR_TAVI, 4, 40);
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_TAVI, 0);
  Actor_SetAnimationById(ACTOR_RIKI, 3);
  Event_ShowMessage(ACTOR_RIKI, 0);
  Actor_SetAnimationAndWait(ACTOR_TAVI, 3);
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_TAVI, 0);
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
  Actor_Stop(ACTOR_HALL_KRADEN);
  wait(1);
  Event_ShowMessageThenWait(ACTOR_HALL_KRADEN, 0, 40);
  Actor_SetPositionFixed(ACTOR_TAVI, 0, 0);
  Actor_SetPositionFixed(ACTOR_RIKI, 0, 0);
  Event_ShowMessage(ACTOR_HALL_KRADEN, 0);
  Actor_FaceDirection(gMainRoutineWork.leaderActorId, 0, 0);
  Actor_FaceDirection(ACTOR_HALL_KRADEN, FACING_WEST, 20);
  Event_ShowMessage(ACTOR_HALL_KRADEN, 0);
  Actor_SetAnimationById(ACTOR_HALL_KRADEN, 2);
  leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  if (leader != NULL)
    Actor_SetDestinationById(ACTOR_HALL_KRADEN, FIELD_PIXEL(leader->x.fixed),
                             FIELD_PIXEL(leader->z.fixed));
  Actor_WaitForMoveById(ACTOR_HALL_KRADEN);
  Actor_SetPositionFixed(ACTOR_HALL_KRADEN, 0, 0);
  GameFlag_Set(FLAG_TAVI_RESCUED);
  Event_End();
}

/// Breath, first seen in the great hall, startles and runs off.
void ShrineOfTheSeaGod_DjinniFleesEast(void) {
  u8 off = 0;

  Event_Start();
  Event_ResolvePendingActions(0);
  FieldCamera_GetActor()->motionFlags = off;
  FieldCamera_MoveWithinMapBounds(px(480), px(32), px(340), 1);
  FieldCamera_WaitForArrival();
  Actor_FaceActorById(ACTOR_GREAT_HALL_BREATH, gMainRoutineWork.leaderActorId,
                      0);
  Actor_ShowEmoteById(ACTOR_GREAT_HALL_BREATH,
                      EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  Actor_SetSpritePriority(ACTOR_GREAT_HALL_BREATH, 1);
  Actor_GetById(ACTOR_GREAT_HALL_BREATH)->motionFlags = off;
  Actor_SetSpeed(ACTOR_GREAT_HALL_BREATH, 1.6);
  Actor_WalkToAndWait(ACTOR_GREAT_HALL_BREATH, 500, 344);
  Actor_WalkToAndWait(ACTOR_GREAT_HALL_BREATH, 580, 344);
  Actor_WatchAndSetScript(ACTOR_GREAT_HALL_BREATH,
                          gMainRoutineWork.leaderActorId);
  Event_Wait(20);
  GameFlag_Set(FLAG_DJINNI_FLED_EAST);
  Event_End();
}

/// In the great hall, Breath flees south out of the room.
void ShrineOfTheSeaGod_DjinniFleesSouth(void) {
  u8 off = 0;

  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_Stop(ACTOR_GREAT_HALL_BREATH);
  FieldCamera_GetActor()->motionFlags = off;
  FieldCamera_MoveWithinMapBounds(px(564), px(32), px(344), 1);
  FieldCamera_WaitForArrival();
  Actor_FaceActorById(ACTOR_GREAT_HALL_BREATH, gMainRoutineWork.leaderActorId,
                      0);
  Actor_ShowEmoteById(ACTOR_GREAT_HALL_BREATH,
                      EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  Actor_SetSpritePriority(ACTOR_GREAT_HALL_BREATH, 1);
  Actor_GetById(ACTOR_GREAT_HALL_BREATH)->motionFlags = off;
  Actor_SetSpeed(ACTOR_GREAT_HALL_BREATH, 1.6);
  Actor_WalkToAndWait(ACTOR_GREAT_HALL_BREATH, 616, 376);
  Actor_WalkToAndWait(ACTOR_GREAT_HALL_BREATH, 616, 460);
  Actor_SetHorizontalPositionWithTerrain(ACTOR_GREAT_HALL_BREATH, 0, 0);
  Event_Wait(20);
  GameFlag_Set(FLAG_DJINNI_FLED_SOUTH);
  Event_End();
}

/// The Djinni runs up from the south stairs and waits by the ledge.
void ShrineOfTheSeaGod_DjinniRunsNorth(void) {
  struct Actor *object = Actor_GetById(ACTOR_LEDGE_ROOM_PILLAR);
  s32 angle;

  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_ShowEmoteById(gMainRoutineWork.leaderActorId,
                      EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  FieldCamera_GetActor()->motionFlags = 0;
  FieldCamera_MoveTo(328, 32, 552, 1);
  FieldCamera_WaitForArrival();
  ShrineOfTheSeaGod_ObjPlaceTurn(ACTOR_BREATH, px(328), px(624), FACING_NORTH);
  wait(1);
  Actor_WatchAndSetScript(gMainRoutineWork.leaderActorId, ACTOR_BREATH);
  object->priorityFlags &= ~ACTOR_PRIORITY_UNDERFOOT;
  Actor_SetSpritePriority(ACTOR_BREATH, 3);
  Actor_SetSpeed(ACTOR_BREATH, 1.6);
  Actor_WalkToAndWait(ACTOR_BREATH, 328, 568);
  Actor_WalkToAndWait(ACTOR_BREATH, 328, 496);
  Actor_WalkToAndWaitById(ACTOR_BREATH, 280, 432);
  Actor_Stop(gMainRoutineWork.leaderActorId);
  wait(1);
  angle = FACING_SOUTH;
  ShrineOfTheSeaGod_ObjPlaceTurn(ACTOR_BREATH, px(280), px(328), angle);
  Actor_SetSpritePriority(ACTOR_BREATH, 2);
  Actor_GetById(ACTOR_BREATH)->priorityFlags |= 1;
  object->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
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
    FieldCamera_MoveWithinMapBounds(px(328), px(48), px(552), 1);
    FieldCamera_WaitForArrival();
    done = 0x4000;
    ShrineOfTheSeaGod_ObjPlaceTurn(ACTOR_BREATH, px(424), px(520), done);
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
    Actor_GetById(ACTOR_BREATH)->priorityFlags |= 1;
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
    FieldCamera_MoveWithinMapBounds(px(320), -1, px(300), 1);
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

#define DEFINE_DJINNI_DASH(name, n1, n2, n3)                                   \
  void name(void) {                                                            \
    ShrineOfTheSeaGod_StartDjinniDash();                                       \
    FieldCamera_MoveWithinMapBounds(px(n1), px(16), px(n2), 1);                \
    Actor_WalkToAndWaitById(ACTOR_BREATH, n1, n2);                             \
    Actor_GetById(ACTOR_BREATH)->riseCounter = n3;                             \
    ShrineOfTheSeaGod_EndDjinniDash();                                         \
  }

DEFINE_DJINNI_DASH(ShrineOfTheSeaGod_DjinniSouthEastToNorthEast, 392, 216, 1)

#define DEFINE_DJINNI_WALK_AND_JUMP(name, n1, n2, n3, n4, n5, n6)              \
  void name(void) {                                                            \
    struct Actor *block = Actor_GetById(ACTOR_BREATH);                         \
    u8 *state;                                                                 \
    s32 flag;                                                                  \
                                                                               \
    ShrineOfTheSeaGod_StartDjinniDash();                                       \
    FieldCamera_MoveToFixed(px(n1), px(16), px(216), 1);                       \
    Actor_WalkToAndWait(ACTOR_BREATH, n1, 216);                                \
    flag = GameFlag_Get(FLAG_TORCH_AT_NORTH_WALL);                             \
    state = &block->riseCounter;                                               \
    if (flag) {                                                                \
      block->facing = n2;                                                      \
      Actor_SetAttachedEffect(ACTOR_BREATH, 258);                              \
      Event_Wait(20);                                                          \
      *state = n3;                                                             \
      GameFlag_Set(FLAG_DJINNI_TRAPPED);                                       \
    } else {                                                                   \
      Event_Wait(5);                                                           \
      FieldCamera_MoveWithinMapBounds(px(n4), px(16), px(216), 1);             \
      Sound_Play(SE_HIGH_JUMP);                                                \
      Actor_SetSpriteFlags(Actor_GetById(ACTOR_BREATH), 0);                    \
      Actor_SetSpeed(ACTOR_BREATH, 4.8);                                       \
      block->velocityY = px(8);                                                \
      Actor_MoveToAndWaitById(ACTOR_BREATH, n5, 216);                          \
      Actor_SetSpriteFlags(Actor_GetById(ACTOR_BREATH), 1);                    \
      Event_Wait(10);                                                          \
      Actor_SetSpeed(ACTOR_BREATH, 1.6);                                       \
      Actor_WalkToAndWait(ACTOR_BREATH, n4, 216);                              \
      *state = n6;                                                             \
    }                                                                          \
    ShrineOfTheSeaGod_EndDjinniDash();                                         \
  }

DEFINE_DJINNI_WALK_AND_JUMP(ShrineOfTheSeaGod_DjinniNorthEastJumpsWest, 360,
                            0x8000, 4, 296, 328, 2)

#define DEFINE_DJINNI_DASH_AROUND(name, n1, n2, n3, n4)                        \
  void name(void) {                                                            \
    ShrineOfTheSeaGod_StartDjinniDash();                                       \
    FieldCamera_MoveWithinMapBounds(px(n1), px(16), px(n2), 1);                \
    Actor_WalkToAndWait(ACTOR_BREATH, n3, 248);                                \
    Actor_WalkToAndWait(ACTOR_BREATH, n1, 248);                                \
    Actor_WalkToAndWaitById(ACTOR_BREATH, n1, n2);                             \
    Actor_GetById(ACTOR_BREATH)->riseCounter = n4;                             \
    ShrineOfTheSeaGod_EndDjinniDash();                                         \
  }

DEFINE_DJINNI_DASH_AROUND(ShrineOfTheSeaGod_DjinniNorthWestToSouthWest, 280,
                          296, 296, 3)

DEFINE_DJINNI_DASH(ShrineOfTheSeaGod_DjinniSouthWestToSouthEast, 392, 296, 0)

DEFINE_DJINNI_DASH(ShrineOfTheSeaGod_DjinniSouthEastToSouthWest, 280, 296, 3)

DEFINE_DJINNI_DASH_AROUND(ShrineOfTheSeaGod_DjinniSouthWestToNorthWest, 296,
                          216, 280, 2)

DEFINE_DJINNI_WALK_AND_JUMP(ShrineOfTheSeaGod_DjinniNorthWestJumpsEast, 328, 0,
                            5, 392, 360, 1)

DEFINE_DJINNI_DASH(ShrineOfTheSeaGod_DjinniNorthEastToSouthEast, 392, 296, 0)

#define DEFINE_DJINNI_JUMP(name, n1, n2, n3, n4, n5)                           \
  void name(void) {                                                            \
    struct Actor *block = Actor_GetById(ACTOR_BREATH);                         \
    u8 *state;                                                                 \
    s32 flag;                                                                  \
                                                                               \
    ShrineOfTheSeaGod_StartDjinniDash();                                       \
    flag = GameFlag_Get(FLAG_TORCH_AT_NORTH_WALL);                             \
    state = &block->riseCounter;                                               \
    if (flag) {                                                                \
      block->facing = n1;                                                      \
      Actor_SetAttachedEffect(ACTOR_BREATH, 258);                              \
      Event_Wait(20);                                                          \
      *state = n2;                                                             \
      GameFlag_Set(FLAG_DJINNI_TRAPPED);                                       \
    } else {                                                                   \
      FieldCamera_MoveWithinMapBounds(px(n3), px(16), px(216), 1);             \
      Sound_Play(SE_HIGH_JUMP);                                                \
      Actor_SetSpriteFlags(Actor_GetById(ACTOR_BREATH), 0);                    \
      Actor_SetSpeed(ACTOR_BREATH, 4.8);                                       \
      block->velocityY = px(8);                                                \
      Actor_MoveToAndWaitById(ACTOR_BREATH, n4, 216);                          \
      Actor_SetSpriteFlags(Actor_GetById(ACTOR_BREATH), 1);                    \
      Event_Wait(10);                                                          \
      Actor_SetSpeed(ACTOR_BREATH, 1.6);                                       \
      Actor_WalkToAndWait(ACTOR_BREATH, n3, 216);                              \
      *state = n5;                                                             \
    }                                                                          \
    ShrineOfTheSeaGod_EndDjinniDash();                                         \
  }

DEFINE_DJINNI_JUMP(ShrineOfTheSeaGod_TrappedDjinniJumpsWest, 0x8000, 4, 296,
                   328, 2)

DEFINE_DJINNI_DASH(ShrineOfTheSeaGod_TrappedDjinniToNorthEast, 392, 216, 1)

DEFINE_DJINNI_DASH(ShrineOfTheSeaGod_TrappedDjinniToNorthWest, 296, 216, 2)

DEFINE_DJINNI_JUMP(ShrineOfTheSeaGod_TrappedDjinniJumpsEast, 0, 5, 392, 360, 1)

/// Each step onto the ledge sends Breath on round it, a jump at a time, unless
/// the torch at the north wall stops the jump and traps it.
void ShrineOfTheSeaGod_ChaseDjinni(void) {
  struct FieldWork *work;
  struct Actor *object;

  if (!GameFlag_Get(FLAG_DJINNI_ON_LEDGE))
    return;
  if (GameFlag_Get(FLAG_DJINNI(DJINNI_BREATH)))
    return;
  work = Ram_HeapSlots->fieldWork;
  object = Actor_GetById(ACTOR_BREATH);
  switch (work->stepTrigger) {
  case 13:
    if (object->riseCounter == 0)
      ShrineOfTheSeaGod_DjinniSouthEastToNorthEast();
    GameFlag_Clear(FLAG_DJINNI_TRAPPED);
    break;
  case 14:
    if (object->riseCounter == 0)
      ShrineOfTheSeaGod_DjinniSouthEastToSouthWest();
    break;
  case 15: {
    u8 current = object->riseCounter;

    if (current == 1)
      ShrineOfTheSeaGod_DjinniNorthEastJumpsWest();
    else if (current == 4 && !GameFlag_Get(FLAG_DJINNI_TRAPPED))
      ShrineOfTheSeaGod_TrappedDjinniJumpsWest();
  } break;
  case 16: {
    u8 current = object->riseCounter;

    if (current == 1)
      ShrineOfTheSeaGod_DjinniNorthEastToSouthEast();
    else if (current == 5)
      ShrineOfTheSeaGod_TrappedDjinniToNorthWest();
  } break;
  case 17: {
    u8 current = object->riseCounter;

    if (current == 2)
      ShrineOfTheSeaGod_DjinniNorthWestToSouthWest();
    else if (current == 4)
      ShrineOfTheSeaGod_TrappedDjinniToNorthEast();
  } break;
  case 18: {
    u8 current = object->riseCounter;

    if (current == 2)
      ShrineOfTheSeaGod_DjinniNorthWestJumpsEast();
    else if (current == 5 && !GameFlag_Get(FLAG_DJINNI_TRAPPED))
      ShrineOfTheSeaGod_TrappedDjinniJumpsEast();
  } break;
  case 19:
    if (object->riseCounter == 3)
      ShrineOfTheSeaGod_DjinniSouthWestToSouthEast();
    break;
  case 20:
    if (object->riseCounter == 3)
      ShrineOfTheSeaGod_DjinniSouthWestToNorthWest();
    GameFlag_Clear(FLAG_DJINNI_TRAPPED);
    break;
  }
}

extern s16 gShrineOfTheSeaGod_Footprints[];
extern s16 gShrineOfTheSeaGod_MovableActors[];
extern u32 gShrineOfTheSeaGod_RikiLooksAround[];

void Rope_Place(s32 a, s32 b, s32 c, s32 d);
void Actor_PlaceBlocks(const void *script);
void ShrineOfTheSeaGod_LayerPuddles(void);

/// Lights the torch: the torch in its burning animation, the flame in its own.
void ShrineOfTheSeaGod_LightTorch(void) {
  Actor_SetAnimationById(ACTOR_TORCH, 2);
  Actor_SetAnimationById(ACTOR_TORCH_FLAME, 0);
  Actor_DisableById(ACTOR_TORCH_FLAME);
}

/// Sand sinks the leader into the sand's layer.
void ShrineOfTheSeaGod_CastSand(void) {
  Actor_GetById(gMainRoutineWork.leaderActorId)->layer = 2;
}

/// The leader rises out of the sand.
void ShrineOfTheSeaGod_EndSand(void) {
  Actor_GetById(gMainRoutineWork.leaderActorId)->layer = 0;
}

/// Sets up the room being entered.
s32 Scene_Initialize(void) {
  s16 scene;

  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
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

/// Sets up a pillar: twelve pixels up, behind the leader, disabled.
struct Actor *ShrineOfTheSeaGod_SetUpUnderfoot(s32 id) {
  struct Actor *object = Actor_GetById(id);

  object->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
  object->y.fixed = px(12);
  Actor_DisableById(id);
  return object;
}

#include "shared/actor_hide.h"

/// Hides the hole room's slides when returning to it with them hidden, and
/// otherwise marks them shown.
void Scene_RunUnlessFlag(void) {
  if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION)) {
    if (GameFlag_Get(FLAG_SLIDES_SHOWN) == 0)
      ShrineOfTheSeaGod_HideSlideActors();
  } else {
    GameFlag_Set(FLAG_SLIDES_SHOWN);
  }
}

/// Hides the hole room's three slides behind the room.
void ShrineOfTheSeaGod_HideSlides(void) {
  u32 i;

  for (i = 0; i <= 2; i++)
    Actor_Hide(ACTOR_FIRST_SLIDE + i);
  Scene_RunUnlessFlag();
}

/// Lays the Djinni's footprints across the room below the great hall.
void ShrineOfTheSeaGod_PlaceFootprints(void) {
  const struct ShrineOfTheSeaGodSpot *spot = gShrineOfTheSeaGod_Footprints;
  u32 i;

  for (i = 0; i <= 15; i++) {
    s32 id = i + 8;
    struct Actor *object = Actor_GetById(id);
    struct FieldSprite *sprite;

    Actor_DisableById(id);
    Actor_SetAnimationById(id, 0);
    Actor_SetSpritePriority(id, 3);
    object->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
    Actor_SetSpriteColor(Actor_GetById(id), 12);
    sprite = object->sprite;
    object->x.fixed = spot->x;
    object->z.fixed = spot->z;
    object->scaleX = spot->scaleX;
    sprite->rotation = spot->rotation;
    spot++;
  }
}

/// Sets up the entrance hall: its floating logs, the rope, and Riki and Tavi
/// gone once Riki has given up.
void ShrineOfTheSeaGod_InitializeRope(void) {
  struct MapWork *map = Ram_HeapSlots->mapGrid;

  ShrineOfTheSeaGod_SetUpFlatProp(ACTOR_HALL_FIRST_LOG);
  ShrineOfTheSeaGod_SetUpFlatProp(ACTOR_HALL_SECOND_LOG);
  Map_SetUpperLevelWading(map);
  FieldTask_AddFrame(ShrineOfTheSeaGod_TrackUpperLevel, TASK_PASS_VBLANK);
  Rope_Initialize();
  Rope_Place(0, FLAG_ROPE_TIED, ACTOR_HALL_ROPE_POLE,
             ACTOR_HALL_ROPE_OTHER_POLE);
  if (GameFlag_Get(FLAG_RIKI_GAVE_UP)) {
    Actor_SetHorizontalPositionWithTerrain(ACTOR_RIKI, 0, 0);
    Actor_SetHorizontalPositionWithTerrain(ACTOR_TAVI, 0, 0);
  }
}

/// The side room's task: sets Riki's collision bit 8 while he stands east of
/// the leader.
/// XXX: what the bit changes; walking into Riki from each side in a render
/// would settle it.
void ShrineOfTheSeaGod_TrackRikiSide(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  struct Actor *object = Actor_GetById(ACTOR_SIDE_ROOM_RIKI);

  if (object->x.fixed > leader->x.fixed)
    object->collisionFlags |= 8;
  else
    object->collisionFlags &= ~8;
}

/// Sets up the side room: the sunken pillar, the torch and its flame, and Riki
/// looking around once he has given up on the rope.
void ShrineOfTheSeaGod_InitializePlankBridge(void) {

  ShrineOfTheSeaGod_SetUpUnderfoot(ACTOR_SIDE_ROOM_PILLAR)->y.fixed = -px(16);
  Actor_SetSpritePriority(ACTOR_SIDE_ROOM_PILLAR, 3);
  Actor_PlaceBlocks(gShrineOfTheSeaGod_MovableActors);
  ShrineOfTheSeaGod_LightTorch();
  FieldTask_AddFrame(ShrineOfTheSeaGod_KeepFlameOnTorch, TASK_PASS_VBLANK);
  Actor_DisableById(ACTOR_SIDE_ROOM_RIKI);
  Actor_DisableById(12);
  Actor_GetById(12)->motionFlags = 0;
  MapGrid_CopyCellAttributes(3, 15, 1, 1, 6, 15);
  if (GameFlag_Get(FLAG_TAVI_RESCUED)) {
    Actor_SetHorizontalPositionWithTerrain(ACTOR_SIDE_ROOM_RIKI, 0, 0);
  } else if (GameFlag_Get(FLAG_RIKI_GAVE_UP)) {
    ShrineOfTheSeaGod_ObjPlaceTurn(ACTOR_SIDE_ROOM_RIKI, px(328), px(200),
                                   0x8000);
    wait(1);
    Actor_SetMovementTypeById(ACTOR_SIDE_ROOM_RIKI,
                              gShrineOfTheSeaGod_RikiLooksAround);
    Actor_SetSpriteFlags(Actor_GetById(ACTOR_SIDE_ROOM_RIKI), 1);
    Actor_GetById(ACTOR_SIDE_ROOM_RIKI)->collisionFlags = 1;
  }
  FieldTask_AddFrame(ShrineOfTheSeaGod_TrackRikiSide, TASK_PASS_VBLANK);
}

/// Sets up the great hall: its pillars, the toppled one in the water, the
/// pushed pillar, and Breath where it fled.
void ShrineOfTheSeaGod_InitializeCrossroads(void) {
  struct Actor *block;

  ShrineOfTheSeaGod_SetUpUnderfoot(ACTOR_GREAT_HALL_FIRST_PILLAR);
  ShrineOfTheSeaGod_SetUpUnderfoot(ACTOR_GREAT_HALL_SECOND_PILLAR);
  ShrineOfTheSeaGod_SetUpUnderfoot(ACTOR_GREAT_HALL_THIRD_PILLAR);
  ShrineOfTheSeaGod_SetUpUnderfoot(ACTOR_GREAT_HALL_FOURTH_PILLAR);
  ShrineOfTheSeaGod_SetUpUnderfoot(ACTOR_GREAT_HALL_PILLAR_STUMP);
  block = ShrineOfTheSeaGod_SetUpUnderfoot(ACTOR_GREAT_HALL_TOPPLING_PILLAR);
  Actor_SetAnimationById(ACTOR_GREAT_HALL_FIRST_PILLAR, 0);
  Actor_SetAnimationById(ACTOR_GREAT_HALL_PILLAR_STUMP, 0);
  Actor_SetAnimationById(ACTOR_GREAT_HALL_TOPPLING_PILLAR, 2);
  Actor_SetSpritePriority(ACTOR_GREAT_HALL_FIRST_PILLAR, 1);
  Actor_SetSpritePriority(ACTOR_GREAT_HALL_SECOND_PILLAR, 1);
  Actor_SetSpritePriority(ACTOR_GREAT_HALL_THIRD_PILLAR, 1);
  Actor_SetSpritePriority(ACTOR_GREAT_HALL_FOURTH_PILLAR, 1);
  Actor_SetSpritePriority(ACTOR_GREAT_HALL_PILLAR_STUMP, 1);
  Actor_SetSpritePriority(ACTOR_GREAT_HALL_TOPPLING_PILLAR, 1);
  FieldTask_AddFrame(ShrineOfTheSeaGod_LayerCrossroadsProps, TASK_PASS_VBLANK);
  if (GameFlag_Get(FLAG_CROSSROADS_PILLAR_TOPPLED)) {
    ShrineOfTheSeaGod_ObjSetPos(block, px(408), -(px(13)), px(336));
    ((struct FieldSprite *)block->sprite)->rotation = 0x1000;
    Actor_SetSpritePriority(ACTOR_GREAT_HALL_TOPPLING_PILLAR, 3);
    block->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
    MapGrid_CopyCellAttributes(0, 50, 3, 1, 24, 70);
  }
  if (GameFlag_Get(FLAG_CROSSROADS_PILLAR_MOVED))
    ShrineOfTheSeaGod_ObjSetPos(Actor_GetById(ACTOR_GREAT_HALL_PUSHED_PILLAR),
                                px(200), 0, px(296));
  Actor_PlaceBlocks(gShrineOfTheSeaGod_MovableActors);
  if (GameFlag_Get(FLAG_DJINNI_FLED_SOUTH))
    Actor_SetHorizontalPositionWithTerrain(ACTOR_GREAT_HALL_BREATH, 0, 0);
  else if (GameFlag_Get(FLAG_DJINNI_FLED_EAST)) {
    ShrineOfTheSeaGod_ObjPlaceTurn(ACTOR_GREAT_HALL_BREATH, px(580), px(344),
                                   0x4000);
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
  FieldTask_AddFrame(ShrineOfTheSeaGod_TrackUpperLevel, TASK_PASS_VBLANK);
}

/// Sets up the ledge room: the pillar, the puddles, frozen or not, the torch,
/// and Breath where the chase left it.
void ShrineOfTheSeaGod_InitializeBreath(void) {
  struct Actor *object =
      ShrineOfTheSeaGod_SetUpUnderfoot(ACTOR_LEDGE_ROOM_PILLAR);

  if ((u16)(gMainRoutineWork.entrance - 2) <= 1) {
    Actor_SetSpritePriority(ACTOR_LEDGE_ROOM_PILLAR, 3);
    object->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
  } else {
    Actor_SetSpritePriority(ACTOR_LEDGE_ROOM_PILLAR, 1);
    object->priorityFlags &= ~ACTOR_PRIORITY_UNDERFOOT;
  }
  ShrineOfTheSeaGod_SetUpPuddle(ACTOR_FIRST_PUDDLE);
  ShrineOfTheSeaGod_SetUpPuddle(ACTOR_SECOND_PUDDLE);
  ShrineOfTheSeaGod_SetUpPuddle(ACTOR_THIRD_PUDDLE);
  if (GameFlag_Get(FLAG_FIRST_PUDDLE_FROZEN))
    MapGrid_CopyCellAttributes(0, 0, 1, 1, 8, 18);
  if (GameFlag_Get(FLAG_SECOND_PUDDLE_FROZEN))
    MapGrid_CopyCellAttributes(0, 0, 1, 1, 10, 18);
  if (GameFlag_Get(FLAG_THIRD_PUDDLE_FROZEN))
    MapGrid_CopyCellAttributes(0, 0, 1, 1, 10, 17);
  FieldTask_AddFrame(ShrineOfTheSeaGod_LayerPuddles, TASK_PASS_VBLANK);
  Actor_PlaceBlocks(gShrineOfTheSeaGod_MovableActors);
  ShrineOfTheSeaGod_LightTorch();
  FieldTask_AddFrame(ShrineOfTheSeaGod_KeepFlameOnTorch, TASK_PASS_VBLANK);
  if (GameFlag_Get(FLAG_DJINNI(DJINNI_BREATH)) ||
      GameFlag_Get(FLAG_FIGHTING_BREATH)) {
    GameFlag_Set(FLAG_DJINNI_CHASE_OVER);
  } else if (GameFlag_Get(FLAG_DJINNI_ON_LEDGE)) {
    struct Actor *block = Actor_GetById(ACTOR_BREATH);
    s32 moved = GameFlag_Get(FLAG_KEEP_PARTY_POSITION);

    if (moved == 0) {
      block->riseCounter = moved;
      Actor_SetPosition(ACTOR_BREATH, 392, 296);
    }
    Actor_WatchAndSetScript(ACTOR_BREATH, gMainRoutineWork.leaderActorId);
  } else if (GameFlag_Get(FLAG_DJINNI_BY_LEDGE)) {
    ShrineOfTheSeaGod_ObjPlaceTurn(ACTOR_BREATH, px(280), px(328),
                                   FACING_SOUTH);
  }
}

/// Sets up the hole room: the pillar, in its hole once pushed there, and the
/// slides hidden.
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
  ShrineOfTheSeaGod_HideSlides();
}

/// Lays out the two pillars, fallen or standing, and the room's two ropes.
void ShrineOfTheSeaGod_InitializePillars(void) {
  struct Actor *firstLog;
  struct Actor *secondLog;

  Map_SetUpperLevelWading((struct MapWork *)Ram_HeapSlots->mapGrid);
  ShrineOfTheSeaGod_SetUpUnderfoot(ACTOR_PILLAR_ROOM_FIRST_PILLAR);
  ShrineOfTheSeaGod_SetUpUnderfoot(ACTOR_PILLAR_ROOM_SECOND_PILLAR);
  ShrineOfTheSeaGod_SetUpUnderfoot(ACTOR_PILLAR_ROOM_THIRD_PILLAR);
  ShrineOfTheSeaGod_SetUpUnderfoot(ACTOR_PILLAR_ROOM_FOURTH_PILLAR);
  ShrineOfTheSeaGod_SetUpUnderfoot(ACTOR_PILLAR_ROOM_FIFTH_PILLAR);
  ShrineOfTheSeaGod_SetUpUnderfoot(ACTOR_FIRST_PILLAR_STUMP);
  ShrineOfTheSeaGod_SetUpUnderfoot(ACTOR_SECOND_PILLAR_STUMP);
  ShrineOfTheSeaGod_SetUpUnderfoot(ACTOR_FIRST_TOPPLING_PILLAR);
  ShrineOfTheSeaGod_SetUpUnderfoot(ACTOR_SECOND_TOPPLING_PILLAR);
  Actor_SetAnimationById(ACTOR_FIRST_PILLAR_STUMP, 0);
  Actor_SetAnimationById(ACTOR_SECOND_PILLAR_STUMP, 0);
  Actor_SetAnimationById(ACTOR_FIRST_TOPPLING_PILLAR, 2);
  Actor_SetAnimationById(ACTOR_SECOND_TOPPLING_PILLAR, 2);
  if (GameFlag_Get(FLAG_FIRST_PILLAR_TOPPLED)) {
    Map_CopyCellAttributeRect(0, 0, 3, 1, 19, 13);
    Map_CopyCellAttributeRect(40, 0, 2, 1, 19, 33);
    Actor_SetPositionAndResetMotion(Actor_GetById(ACTOR_FIRST_TOPPLING_PILLAR),
                                    px(320), -px(21), px(534));
    Actor_SetSpritePriority(ACTOR_FIRST_TOPPLING_PILLAR, 3);
  }
  if (GameFlag_Get(FLAG_SECOND_PILLAR_TOPPLED)) {
    Map_CopyCellAttributeRect(0, 0, 3, 1, 24, 22);
    Map_CopyCellAttributeRect(40, 0, 2, 1, 24, 24);
    Actor_SetPositionAndResetMotion(Actor_GetById(ACTOR_SECOND_TOPPLING_PILLAR),
                                    px(400), -px(21), px(390));
    Actor_SetSpritePriority(ACTOR_SECOND_TOPPLING_PILLAR, 3);
  }
  Rope_Initialize();
  Call4((void (*)())Rope_Place, 0, FLAG_ROPE_TIED, 17, 18);
  Rope_Place(1, FLAG_SECOND_ROPE_TIED, 19, 20);
  Actor_DisableById(ACTOR_FIRST_PILLAR_LOG);
  Actor_GetById(ACTOR_FIRST_PILLAR_LOG)->motionFlags = 0;
  firstLog = Actor_GetById(ACTOR_FIRST_PILLAR_OTHER_LOG);
  Actor_DisableById(ACTOR_FIRST_PILLAR_OTHER_LOG);
  firstLog->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
  firstLog->motionFlags = 0;
  Actor_DisableById(ACTOR_SECOND_PILLAR_LOG);
  Actor_GetById(ACTOR_SECOND_PILLAR_LOG)->motionFlags = 0;
  secondLog = Actor_GetById(ACTOR_SECOND_PILLAR_OTHER_LOG);
  Actor_DisableById(ACTOR_SECOND_PILLAR_OTHER_LOG);
  secondLog->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
  secondLog->motionFlags = 0;
}

/// Sets up the island room: its floating logs, and the water falling when the
/// party comes down from above.
void ShrineOfTheSeaGod_InitializeSeaGodsTear(void) {
  ShrineOfTheSeaGod_SetUpFlatProp(8);
  ShrineOfTheSeaGod_SetUpFlatProp(9);
  ShrineOfTheSeaGod_SetUpFlatProp(10);
  if (gMainRoutineWork.entrance == 3 &&
      GameFlag_Get(FLAG_KEEP_PARTY_POSITION) == 0)
    ShrineOfTheSeaGod_LowerWater();
}

/// Sets up the ninth room: the party rises on the water when it comes up from
/// the island.
void ShrineOfTheSeaGod_InitializeStatue(void) {
  if (gMainRoutineWork.entrance == 1 &&
      GameFlag_Get(FLAG_KEEP_PARTY_POSITION) == 0)
    ShrineOfTheSeaGod_RiseOnWater();
}

/// The Right Prong floats above the last room, shedding sparkles.
void ShrineOfTheSeaGod_SparkleRightProng(struct Actor *object) {
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
    FieldEffect_Spawn(object->x.fixed + px(dx - 8),
                      object->y.fixed + px(dy - 8), object->z.fixed, 0,
                      px(1) / 2, phase, EFFECT_USE_TYPE, &options);
  }
}

/// Sets up the top room: the Right Prong sparkles above its place.
void ShrineOfTheSeaGod_InitializeRightProng(void) {
  struct Actor *object = Actor_GetById(64);
  struct MapWork *map = Ram_HeapSlots->mapGrid;
  struct MapLayer *layer = &map->layers[2];

  layer->offsetX += px(128);
  MapGrid_CenterOnFollowedPoint();
  wait(1);
  if (object != NULL) {
    object->motionFlags = 0;
    object->y.fixed = px(66);
    object->update = ShrineOfTheSeaGod_SparkleRightProng;
  }
}

/// Sets up a floating log: flat on the water, behind the leader, disabled.
void ShrineOfTheSeaGod_SetUpFlatProp(s32 id) {
  struct Actor *object = Actor_GetById(id);

  Actor_SetSpritePriority(id, 3);
  Actor_DisableById(id);
  object->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
  object->motionFlags = 0;
  object->scaleX = Q16_ONE * 8 / 10;
  object->scaleY = 0xe666;
}

/// Nothing to prepare.
s32 Scene_PrepareMap(void) {

  if (GameFlag_Get(FLAG_FIGHTING_BREATH)) {
    if (gMainRoutineWork.sequenceResult == 1) {
      if (GameFlag_Get(FLAG_BREATH_TRAPPED_EAST))
        Actor_SetPositionAndFacing(ACTOR_BREATH, px(360), px(216), 0);
      else if (GameFlag_Get(FLAG_BREATH_TRAPPED_WEST)) {
        ShrineOfTheSeaGod_ObjPlaceTurn(ACTOR_BREATH, px(328), px(216),
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

void ShrineOfTheSeaGod_FightBreath(void);
void ShrineOfTheSeaGod_ExamineStatue(void);
void ShrineOfTheSeaGod_PushPillarIntoHole(void);
void ShrineOfTheSeaGod_ToppleSecondPillar(void);
void ShrineOfTheSeaGod_RevealStone(void);

extern u32 gFieldEffect_ShortScript[];
extern u32 gFieldEffect_LongScript[];

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

const s32 *gFieldEffect_DurationScripts[] = {
    gFieldEffect_ShortScript,
    gFieldEffect_MediumScript,
    gFieldEffect_LongScript,
};

u32 gShrineOfTheSeaGod_RikiLooksAround[] = {
    22, 3, 0xb000, 0, 80, 22,
    3, 0x5000, 0, 80, 22, 3,
    0x8000, 0, 120, 13, 0xffff, 0,
};

u32 gShrineOfTheSeaGod_BoysGoHome[] = {
    39, 2, 4, 0x1580000, 0, 0xc60000,
    1, 4, 0x17a0000, 0, 0xc60000, 1,
    4, 0x1880000, 0, 0xdc0000, 1, 4,
    0x1880000, 0, 0x11c0000, 1, 39, 1,
    17,
};

s16 gShrineOfTheSeaGod_MovableActors[] = {
    ACTOR_TORCH, -1,
};

s16 gShrineOfTheSeaGod_Footprints[] = {
    0, 316, 0, 136, 0, -1, 0x8000, 0,
    0, 318, 0, 142, 0, 1, 0x7000, 0,
    0, 324, 0, 154, 0, -1, 0x6000, 0,
    0, 330, 0, 158, 0, 1, 0x5000, 0,
    0, 338, 0, 166, 0, -1, 0x4800, 0,
    0, 344, 0, 166, 0, 1, 0x4800, 0,
    0, 288, 0, 240, 0, 1, 0xd800, 0,
    0, 284, 0, 236, 0, -1, 0xd800, 0,
    0, 272, 0, 232, 0, 1, 0xc800, 0,
    0, 266, 0, 230, 0, -1, 0xc800, 0,
    0, 256, 0, 238, 0, 1, 0xb000, 0,
    0, 248, 0, 238, 0, -1, 0xb800, 0,
    0, 240, 0, 250, 0, 1, 0xa800, 0,
    0, 234, 0, 252, 0, -1, 0xb000, 0,
    0, 224, 0, 262, 0, 1, 0xa800, 0,
    0, 220, 0, 262, 0, -1, 0xa800, 0,
    -1, 0,
};

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gShrineOfTheSeaGodStatueExitRegions[] = {
    0x200100, 0x11000c0, 0xd00030, 0x2ffff, 0xffff, 0,
    0, 0,
};

s16 gShrineOfTheSeaGodLaddersExitRegions[] = {
    720, -38, 238, 736, -22, 254, -1, 1,
    864, 32, 208, 880, 48, 224, -1, 2,
    -1, 0, 0, 0, 0, 0, 0, 0,
};

u32 gShrineOfTheSeaGodRightProngExitRegions[] = {
    0xffda0310, 0x32000a0, 0xb0ffea, 0x1ffff, 0xffff, 0,
    0, 0,
};

u32 gSceneExits[] = {
    SCENE_EXITS(ShrineOfTheSeaGodRope),
    SCENE_EXIT(1, ShrineOfTheSeaGodCrossroads, 5),
    SCENE_EXIT(2, ShrineOfTheSeaGodPlankBridge, 1),
    SCENE_EXIT(3, WorldMap, 5),
    SCENE_EXITS(ShrineOfTheSeaGodPlankBridge),
    SCENE_EXIT(1, ShrineOfTheSeaGodRope, 2),
    SCENE_EXITS(ShrineOfTheSeaGodCrossroads),
    SCENE_EXIT(1, ShrineOfTheSeaGodPillars, 2),
    SCENE_EXIT(2, ShrineOfTheSeaGodSeaGodsTear, 1),
    SCENE_EXIT(3, ShrineOfTheSeaGodSeaGodsTear, 2),
    SCENE_EXIT(4, ShrineOfTheSeaGodFootprints, 1),
    SCENE_EXIT(5, ShrineOfTheSeaGodRope, 1),
    SCENE_EXITS(ShrineOfTheSeaGodFootprints),
    SCENE_EXIT(1, ShrineOfTheSeaGodCrossroads, 4),
    SCENE_EXIT(2, ShrineOfTheSeaGodBreath, 2),
    SCENE_EXIT(3, ShrineOfTheSeaGodBreath, 4),
    SCENE_EXIT(4, ShrineOfTheSeaGodBreath, 3),
    SCENE_EXITS(ShrineOfTheSeaGodBreath),
    SCENE_EXIT(1, ShrineOfTheSeaGodPillarHole, 1),
    SCENE_EXIT(2, ShrineOfTheSeaGodFootprints, 2),
    SCENE_EXIT(3, ShrineOfTheSeaGodFootprints, 4),
    SCENE_EXIT(4, ShrineOfTheSeaGodFootprints, 3),
    SCENE_EXITS(ShrineOfTheSeaGodPillarHole),
    SCENE_EXIT(1, ShrineOfTheSeaGodBreath, 1),
    SCENE_EXIT(2, ShrineOfTheSeaGodPillars, 1),
    SCENE_EXITS(ShrineOfTheSeaGodPillars),
    SCENE_EXIT(1, ShrineOfTheSeaGodPillarHole, 2),
    SCENE_EXIT(2, ShrineOfTheSeaGodCrossroads, 1),
    SCENE_EXITS(ShrineOfTheSeaGodSeaGodsTear),
    SCENE_EXIT(1, ShrineOfTheSeaGodCrossroads, 2),
    SCENE_EXIT(2, ShrineOfTheSeaGodCrossroads, 3),
    SCENE_EXIT(3, ShrineOfTheSeaGodStatue, 1),
    SCENE_EXITS(ShrineOfTheSeaGodStatue),
    SCENE_EXIT(1, ShrineOfTheSeaGodSeaGodsTear, 3),
    SCENE_EXIT(2, ShrineOfTheSeaGodLadders, 40),
    SCENE_EXITS(ShrineOfTheSeaGodLadders),
    SCENE_EXIT(1, ShrineOfTheSeaGodStatue, 2),
    SCENE_EXIT(2, ShrineOfTheSeaGodRightProng, 40),
    SCENE_EXITS(ShrineOfTheSeaGodRightProng),
    SCENE_EXIT(1, ShrineOfTheSeaGodLadders, 41),
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
    ACTOR_PLACEMENT(410, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(369, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 216, 0, 272, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_RIKI, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gShrineOfTheSeaGodCrossroadsActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 216, 0, 296, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(370, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 248, 0, 216, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(369, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 408, 0, 216, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(369, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 520, 0, 232, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(369, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 536, 0, 344, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(369, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 408, 0, 328, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(369, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 408, 0, 328, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_JUPITER_DJINNI, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 472, 0, 328, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gShrineOfTheSeaGodFootprintsActorPlacements[] = {
    ACTOR_PLACEMENT(270, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(270, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(270, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(270, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(270, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(270, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(270, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(270, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(270, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(270, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(270, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(270, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(270, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(270, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(270, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(270, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    {SCENE_TABLE_END},
};

struct ActorPlacement gShrineOfTheSeaGodBreathActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_FLAME_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 344, 0, 248, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(410, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 136, 0, 296, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 168, 0, 296, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 168, 0, 280, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(369, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 328, 0, 552, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_JUPITER_DJINNI, FLAG_DJINNI(DJINNI_BREATH), MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gShrineOfTheSeaGodPillarHoleActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 248, 0, 248, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(337, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 232, 0, 424, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(337, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 232, 0, 408, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(337, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 232, 0, 392, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    {SCENE_TABLE_END},
};

struct ActorPlacement gShrineOfTheSeaGodPillarsActorPlacements[] = {
    ACTOR_PLACEMENT(369, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 216, 0, 248, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(369, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 312, 0, 264, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(369, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 408, 0, 232, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(369, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 312, 0, 376, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(370, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 360, 0, 424, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(369, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 328, 0, 216, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(369, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 408, 0, 360, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(369, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 328, 0, 216, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(369, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 408, 0, 360, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 344, 0, 616, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 296, 0, 616, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 456, 0, 392, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 504, 0, 392, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_SPLASH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SPLASH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_SPLASH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SPLASH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(489, CONDITION_ALWAYS, 7, 448, 0, 248, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(489, CONDITION_ALWAYS, 7, 464, 0, 248, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(489, CONDITION_ALWAYS, 7, 456, 0, 240, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gShrineOfTheSeaGodSeaGodsTearActorPlacements[] = {                                             
      {297, CONDITION_ALWAYS, 1, px(160), 0, px(202), FACING_SOUTH, 2,    
       0},                                                                     
      {297, CONDITION_ALWAYS, 1, px(176), 0, px(298), FACING_SOUTH, 2,    
       1},                                                                    
      {297, CONDITION_ALWAYS, 1, px(320), 0, px(218), FACING_SOUTH, 2,    
       1},                                                                    
      {SCENE_TABLE_END},                                                       
  };

struct Trigger gShrineOfTheSeaGod_Triggers[] = {
    {TRIGGER_TABLE_END},
};

struct Trigger gShrineOfTheSeaGodRopeTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {PSYNERGY_ON_ACTOR(ABILITY_LASH), ACTOR_HALL_ROPE_POLE, FLAG_ROPE_TIED, EVENT(ShrineOfTheSeaGod_LashRope)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_AFTER_CAST), 0, CONDITION_ALWAYS, EVENT(ShrineOfTheSeaGod_CastSand)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(ShrineOfTheSeaGod_EndSand)},
    {TRIGGER_STEP, 10, FLAG_RIKI_GAVE_UP, EVENT(ShrineOfTheSeaGod_ThrowRopeAgain)},
    {TRIGGER_TABLE_END},
};

struct Trigger gShrineOfTheSeaGodPlankBridgeTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TALK_TRIGGER_FACING(FACING_WEST), ACTOR_SIDE_ROOM_RIKI, CONDITION_ALWAYS, EVENT(ShrineOfTheSeaGod_StartleRiki)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SIDE_ROOM_RIKI, CONDITION_ALWAYS, MSGNO(MsgFieldICantFindAnythingAroundHere)},
    {TRIGGER_TABLE_END},
};

struct Trigger gShrineOfTheSeaGodCrossroadsTriggers[] = {
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_GREAT_HALL_PUSHED_PILLAR, FLAG_CROSSROADS_PILLAR_MOVED, EVENT(ShrineOfTheSeaGod_LiftBlock)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_GREAT_HALL_PUSHED_PILLAR, FLAG_CROSSROADS_PILLAR_MOVED, EVENT(ShrineOfTheSeaGod_MovePillar)},
    {TRIGGER_STEP, 10, FLAG_LEADER_IN_FRONT, EVENT(ShrineOfTheSeaGod_DrawLeaderInFront)},
    {TRIGGER_STEP, 11, IF_FLAG_SET(FLAG_LEADER_IN_FRONT), EVENT(ShrineOfTheSeaGod_DrawLeaderBehind)},
    {TRIGGER_STEP, 15, FLAG_CROSSROADS_PILLAR_TOPPLED, EVENT(ShrineOfTheSeaGod_TopplePillarIntoWater)},
    {TRIGGER_STEP, 11, FLAG_DJINNI_FLED_EAST, EVENT(ShrineOfTheSeaGod_DjinniFleesEast)},
    {TRIGGER_STEP, 12, FLAG_DJINNI_FLED_SOUTH, EVENT(ShrineOfTheSeaGod_DjinniFleesSouth)},
    {TRIGGER_TABLE_END},
};

struct Trigger gShrineOfTheSeaGodFootprintsTriggers[] = {
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_TABLE_END},
};
// clang-format on

struct Trigger gShrineOfTheSeaGodBreathTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 4, CONDITION_ALWAYS, 4},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_FIRST_PUDDLE,
     FLAG_FIRST_PUDDLE_FROZEN, EVENT(ShrineOfTheSeaGod_FreezeFirstPuddle)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_SECOND_PUDDLE,
     FLAG_SECOND_PUDDLE_FROZEN, EVENT(ShrineOfTheSeaGod_FreezeSecondPuddle)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_THIRD_PUDDLE,
     FLAG_THIRD_PUDDLE_FROZEN, EVENT(ShrineOfTheSeaGod_FreezeThirdPuddle)},
    {TRIGGER_PUSH, 0, CONDITION_ALWAYS, EVENT(ShrineOfTheSeaGod_LiftBlock)},
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS,
     EVENT(ShrineOfTheSeaGod_SetDownTorch)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST),
     ACTOR_TORCH, CONDITION_ALWAYS, EVENT(ShrineOfTheSeaGod_LiftBlock)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_TORCH, CONDITION_ALWAYS,
     EVENT(ShrineOfTheSeaGod_SetDownTorch)},
    {TRIGGER_STEP, 10, FLAG_DJINNI_BY_LEDGE,
     EVENT(ShrineOfTheSeaGod_DjinniRunsNorth)},
    {TRIGGER_STEP, 11, FLAG_DJINNI_BY_LEDGE,
     EVENT(ShrineOfTheSeaGod_DjinniRunsWest)},
    {TRIGGER_STEP, 12, FLAG_DJINNI_ON_LEDGE,
     EVENT(ShrineOfTheSeaGod_DjinniHopsOntoLedge)},
    {TRIGGER_STEP, 13, FLAG_DJINNI_CHASE_OVER,
     EVENT(ShrineOfTheSeaGod_ChaseDjinni)},
    {TRIGGER_STEP, 14, FLAG_DJINNI_CHASE_OVER,
     EVENT(ShrineOfTheSeaGod_ChaseDjinni)},
    {TRIGGER_STEP, 15, FLAG_DJINNI_CHASE_OVER,
     EVENT(ShrineOfTheSeaGod_ChaseDjinni)},
    {TRIGGER_STEP, 16, FLAG_DJINNI_CHASE_OVER,
     EVENT(ShrineOfTheSeaGod_ChaseDjinni)},
    {TRIGGER_STEP, 17, FLAG_DJINNI_CHASE_OVER,
     EVENT(ShrineOfTheSeaGod_ChaseDjinni)},
    {TRIGGER_STEP, 18, FLAG_DJINNI_CHASE_OVER,
     EVENT(ShrineOfTheSeaGod_ChaseDjinni)},
    {TRIGGER_STEP, 19, FLAG_DJINNI_CHASE_OVER,
     EVENT(ShrineOfTheSeaGod_ChaseDjinni)},
    {TRIGGER_STEP, 20, FLAG_DJINNI_CHASE_OVER,
     EVENT(ShrineOfTheSeaGod_ChaseDjinni)},
    {TRIGGER_TALK, ACTOR_BREATH, CONDITION_ALWAYS,
     EVENT(ShrineOfTheSeaGod_FightBreath)},
    {TRIGGER_TABLE_END},
};

struct Trigger gShrineOfTheSeaGodPillarHoleTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_PUSHED, 0, FLAG_PILLAR_IN_HOLE,
     EVENT(ShrineOfTheSeaGod_PushPillarIntoHole)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_HOLE_ROOM_PILLAR,
     FLAG_PILLAR_IN_HOLE, EVENT(ShrineOfTheSeaGod_PushPillarIntoHole)},
    {TRIGGER_TABLE_END},
};

struct Trigger gShrineOfTheSeaGodPillarsTriggers[] = {
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 2, CONDITION_ALWAYS, 2},
    {PSYNERGY_ON_ACTOR(ABILITY_LASH), ACTOR_FIRST_ROPE_POLE, FLAG_ROPE_TIED, 0},
    {PSYNERGY_ON_ACTOR(ABILITY_LASH), ACTOR_SECOND_ROPE_POLE,
     FLAG_SECOND_ROPE_TIED, 0},
    {TRIGGER_STEP, 5, FLAG_FIRST_PILLAR_TOPPLED,
     EVENT(ShrineOfTheSeaGod_ToppleFirstPillar)},
    {TRIGGER_STEP, 6, FLAG_SECOND_PILLAR_TOPPLED,
     EVENT(ShrineOfTheSeaGod_ToppleSecondPillar)},
    {TRIGGER_TALK, ACTOR_FIRST_MENU_MARKER, CONDITION_ALWAYS,
     EVENT(ShrineOfTheSeaGod_OpenFieldMenu)},
    {TRIGGER_TALK, ACTOR_SECOND_MENU_MARKER, CONDITION_ALWAYS,
     EVENT(ShrineOfTheSeaGod_OpenFieldMenu)},
    {TRIGGER_TALK, ACTOR_THIRD_MENU_MARKER, CONDITION_ALWAYS,
     EVENT(ShrineOfTheSeaGod_OpenFieldMenu)},
    {TRIGGER_TABLE_END},
};

struct Trigger gShrineOfTheSeaGodSeaGodsTearTriggers[] = {
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXAMINE, 10, CONDITION_ALWAYS,
     EVENT(ShrineOfTheSeaGod_ExamineStatue)},
    {USE_ITEM(ITEM_SEA_GODS_TEAR), 10, CONDITION_ALWAYS,
     EVENT(ShrineOfTheSeaGod_UseSeaGodsTear)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0,
     CONDITION_ALWAYS, EVENT(ShrineOfTheSeaGod_RevealStone)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0,
     CONDITION_ALWAYS, EVENT(ShrineOfTheSeaGod_HideStone)},
    {TRIGGER_TABLE_END},
};

struct Trigger gShrineOfTheSeaGodStatueTriggers[] = {
    {TRIGGER_EXAMINE, 1, CONDITION_ALWAYS,
     EVENT(ShrineOfTheSeaGod_ExamineStatue)},
    {USE_ITEM(ITEM_SEA_GODS_TEAR), 1, CONDITION_ALWAYS,
     EVENT(ShrineOfTheSeaGod_SinkWithWater)},
    {TRIGGER_TABLE_END},
};
