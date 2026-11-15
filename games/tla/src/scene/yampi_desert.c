/// @file
/// The Yampi Desert, in twelve rooms that share this overlay
/// (SceneId_YampiDesertGuardianRingChest to SceneId_YampiDesertBlowMaceChest):
/// its travelers, the scorpion in its way, the stakes Pound sinks and the sand
/// slides Sand slips under, and its tables of entrances, placements and
/// triggers by room.
#include "djinn_ids.h"
#include "fixed_math.h"
#include "flags.h"
#include "item_ids.h"
#include "macros.h"
#include "scene_ids.h"
#include "sprite_ids.h"
#include "types.h"
#include "version.h"
#include "battle/battle.h"
#include "field/actor.h"
#include "field/actor_script.h"
#include "field/call.h"
#include "field/event.h"
#include "field/event_commands.h"
#include "field/field.h"
#include "field/field_effect.h"
#include "field/field_event.h"
#include "field/field_sequence_event.h"
#include "field/flag_call.h"
#include "field/map_grid.h"
#include "field/scene_tables.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "graphics/resource.h"
#include "menu/menu.h"
#include "scene/scene.h"
#include "sound/sound.h"
#include "system/field_effect.h"
#include "system/frame.h"
#include "system/heap_release.h"
#include "system/input.h"
#include "system/io_reg.h"
#include "system/iwram_call.h"
#include "system/main_routine_work.h"
#include "system/math_table.h"
#include "system/ram_buffer.h"
#include "system/system.h"
#include "system/task.h"
#include "window/item_icon.h"
#include "text/msg_ids.h"

#include "../field/actor_set_sprite_priority.c"

#include "../field/field_effect_move.c"

#define EFFECT_SPAWN FieldEffect_Spawn
#define EFFECT_CREATE Import_ActorCreate
#define EFFECT_SET_MODE Actor_SelectAnimation
#define EFFECT_SET_SCRIPT Actor_StartScript
#define EFFECT_SCRIPTS gFieldEffect_DurationScripts
#include "../field/field_effect_spawn.c"

/// Set as Pound on the first screen's wooden column stirs the King Scorpion
/// out of the sand, and it flees to the next screen.
#define FLAG_YAMPI_DESERT_SCORPION_STIRRED 0x889
/// As FLAG_YAMPI_DESERT_SCORPION_STIRRED, on the second screen.
#define FLAG_YAMPI_DESERT_SCORPION_STIRRED_AGAIN 0x88a
/// Set once the King Scorpion is cornered in the screen with the trap.
#define FLAG_YAMPI_DESERT_SCORPION_CORNERED 0x88b
/// Set once the King Scorpion is beaten; it leaves the Scoop Gem.
#define FLAG_KING_SCORPION_BEATEN 0x88c
/// Set once Scoop has opened the sand pit that hides the ladder.
#define FLAG_YAMPI_DESERT_LADDER_DUG_UP 0x88d
/// Set once the travelers' elder has said he will not turn back.
#define FLAG_YAMPI_DESERT_ELDER_INSISTED 0x88f
/// Set once Scoop has opened the sand pit between the four rocks.
#define FLAG_YAMPI_DESERT_ROCK_PIT_DUG_UP 0x8ed
/// Set once the party has seen the travelers from the ledge above them; they
/// stand about the oasis from then on.
#define FLAG_YAMPI_DESERT_TRAVELERS_MET 0x8f0
/// Set while the leader stands on the tops of the stakes Pound sank.
#define FLAG_YAMPI_DESERT_ON_STAKE_TOPS FLAG_SCENE_TEMP_0B
/// Set while the sand slide's actors are shown.
#define FLAG_YAMPI_DESERT_SLIDE_ACTORS_SHOWN FLAG_SCENE_TEMP_0D
/// Set once Lash has tied the Trainer's Whip chest's room's rope.
#define FLAG_YAMPI_DESERT_ROPE_TIED FLAG_SCENE_TEMP_0C
/// Set once Scoop has dug up the treasure pit's room's treasure pit.
#define FLAG_YAMPI_DESERT_TREASURE_PIT_DUG_UP FLAG_AREA_TEMP_02

extern u16 gYampiDesertGuardianRingChestStakes[];
extern u16 gYampiDesertBlitzStakes[];
extern u16 gYampiDesertPsynergyStoneStakes[];
extern u16 gYampiDesertKingScorpionStakes[];
extern u16 gYampiDesertBlowMaceChestStakes[];
extern s32 gYampiDesert_RaisedEvent;

/// The triggers the scene raises through gYampiDesert_RaisedEvent while the
/// King Scorpion is chased through the sand.
enum YampiDesertRaisedTrigger {
  RAISED_STIR_SCORPION = 200,
  RAISED_STIR_SCORPION_AGAIN = 201,
  RAISED_CORNER_SCORPION = 202,
  RAISED_WATCH_SCORPION_STOP = 210
};

/// The map cells' collision codes the desert tests: sand that slides the
/// party north, south, west or east, the ramp to the upper level, and a wall.
enum YampiDesertCellCode {
  CELL_SLIDE_NORTH = 10,
  CELL_SLIDE_SOUTH = 11,
  CELL_SLIDE_WEST = 12,
  CELL_SLIDE_EAST = 13,
  CELL_RAMP = 20,
  CELL_WALL = 255
};

void Actor_SetWideSprite(s32 actor, s32 value);

void YampiDesert_StepOntoStakeTops(void);
void YampiDesert_OpenLadderPit(void);
void YampiDesert_SetSlideActorsShown(s32 show);
void YampiDesert_HideSlideActors(void);
void YampiDesert_SetActorsHidden(s32 show, s32 first, u32 count);
void YampiDesert_UpdateSandSlide(void);
void YampiDesert_TrackUpperLevel(void);
void YampiDesert_OpenScoopHole(void);
void Actor_Hide(s32 id);
void Scene_RunUnlessFlag(void);
void YampiDesert_SetUpBlowMaceChestSlideActors(void);
void YampiDesert_ClimbOut(struct Actor *object, s32 x, s32 z, s32 mode);
void YampiDesert_MatchStakePriority(void);
void YampiDesert_PlaceStakes(const void *script);
void Pound_SinkTarget(u16 *list, s32 mode, s32 count);
void Actor_DisableById(s32 actor);
void FieldEffect_StartBlankOverlay(s32 value);
void Rope_Initialize(void);
void Rope_Place(s32 a, s32 b, s32 c, s32 d);
void YampiDesert_ElderRefusesToQuit(void);
void YampiDesert_BlockPoundedStakes(u16 *list);
void YampiDesert_UnblockStakes(u16 *list);
void FieldEvent_Wait(s32 frames);
struct Actor *YampiDesert_CreateScoopGem(void);
void Actor_Release(struct Actor *object);
extern struct Actor *gYampiDesert_SandRipple;
void YampiDesert_ClearRaisedEvent(void);
void YampiDesert_PlayScorpionRumble(void);
void YampiDesert_SetUpPsynergyStoneScorpion(void);
void YampiDesert_ReadyStake(s32 id);
void YampiDesert_TrackUpperLevelFromBelow(void);
void Actor_SetCellOpen(s32 id, u32 kind, s32 flag, s32 layer);

/// Adds a task.
static inline void Scene_TaskAdd(void (*func)(void), s32 priority) {
  Task_Add(func, priority);
}

static inline s32 Scene_FlagTest(s32 flag) { return GameFlag_Get(flag); }

/// Turns an actor to an angle over the given frames.
static inline void Scene_ArmCallback(s32 id, s32 angle, s32 flags) {
  Actor_FaceDirectionById(id, angle, flags);
}

/// Places an actor at (x, z) on the ground, facing a direction.
static inline void YampiDesert_SetPositionAndFacing(s32 actorId, s32 x, s32 z,
                                                    s32 facing) {
  Actor_SetHorizontalPositionAndAngleWithTerrain(actorId, x, z, facing);
}

/// Places a rope between two poles.
static inline void YampiDesert_PlaceRope(s32 rope, s32 a, s32 b, s32 c) {
  Rope_Place(rope, a, b, c);
}

/// Scoop's dig at a treasure pit: the hidden actor comes up and the pit's flag
/// is set.
static inline void YampiDesert_ReturnHome(s32 mode, s32 actorId, s32 flag) {
  Actor_ReturnHome(mode, actorId, flag);
}

/// The stakes Pound sinks, the first rows of the rooms that have them (the
/// Guardian Ring chest's, Blitz's, the Psynergy Stone's, the King Scorpion's
/// and the Blow Mace chest's): stake n is the placed actor
/// STAKE(n), and FLAG_STAKE_POUNDED(n) is set once Pound has sunk it.
#define STAKE(n) (ACTOR_FIRST_PLACED + (n))
#define FLAG_STAKE_POUNDED(n) (FLAG_SCENE_TEMP_00 + (n))
/// A row of a room's stake list, as Pound_SinkTarget and Actor_SetCellOpen read
/// it: the stake, its kind of poses (0, a stake's) and its flag. A list ends at
/// STAKE_LIST_END.
#define STAKE_ROW(n) STAKE(n), 0, FLAG_STAKE_POUNDED(n)
#define STAKE_LIST_END 0xffff

/// The travelers from Madra resting by the oasis (SceneId_YampiDesertOasis), by
/// the rows of gYampiDesertOasisActorPlacements and
/// gYampiDesertOasisActorPlacementsAfterMeeting, chasing Briggs to Alhafra.
enum YampiDesertTravelersActorId {
  /// "We followed the pirates here from Madra... Are you going to Alhafra?"
  ACTOR_TRAVELER_FOLLOWING_THE_PIRATES = ACTOR_FIRST_PLACED,
  /// Who is to find evidence that Piers is a Champa.
  ACTOR_TRAVELER_SEEKING_EVIDENCE,
  /// "Ooooh... This desert... It's so hot out here! I feel dizzy!"
  ACTOR_DIZZY_TRAVELER,
  /// "Water... My throat is so parched!"
  ACTOR_PARCHED_TRAVELER,
  /// Who would forget about Briggs: "Can't we just forget about Briggs?"
  ACTOR_TRAVELER_WHO_WOULD_TURN_BACK,
  /// Madra's elder, who will not stop looking for Briggs.
  ACTOR_ELDER,
  /// Madra's mayor, the elder's son, who must get them through the desert.
  ACTOR_MAYOR
};

/// Blitz, the Jupiter Djinni, after Blitz's room's stakes.
#define ACTOR_BLITZ STAKE(11)

/// The King Scorpion and the ripple of sand that follows it while it is
/// buried: in the Psynergy Stone's room after its eight stakes, in the King
/// Scorpion's after its thirteen.
enum YampiDesertScorpionActorId {
  ACTOR_PSYNERGY_STONE_ROOM_KING_SCORPION = STAKE(8),
  ACTOR_PSYNERGY_STONE_ROOM_SAND_RIPPLE,
  ACTOR_KING_SCORPION_ROOM_KING_SCORPION = STAKE(13),
  ACTOR_KING_SCORPION_ROOM_SAND_RIPPLE,
  /// XXX: what the King Scorpion's room's last row (sprite 354) shows; a render
  /// of the room with the King Scorpion trapped would settle it.
  ACTOR_KING_SCORPION_ROOM_LAST_ROW
};

/// The Trainer's Whip chest's room's actors
/// (SceneId_YampiDesertTrainersWhipChest).
enum YampiDesertTrainersWhipChestActorId {
  /// The poles of the rope Lash ties.
  ACTOR_ROPE_POLE = ACTOR_FIRST_PLACED,
  ACTOR_ROPE_OTHER_POLE,
  ACTOR_SIGNPOST,
  /// The two actors the sand slide shows and hides
  /// (YampiDesert_SetSlideActorsShown).
  ACTOR_WHIP_CHEST_ROOM_FIRST_SLIDE,
  ACTOR_WHIP_CHEST_ROOM_SECOND_SLIDE
};

/// The scorpion pit's actors (SceneId_YampiDesertScorpionPit), where the King
/// Scorpion is fought.
enum YampiDesertScorpionPitActorId {
  ACTOR_KING_SCORPION = ACTOR_FIRST_PLACED,
  /// Moved to the middle of the four rocks once Scoop has dug the pit there.
  /// XXX: what it shows (sprite 489); a render of the room after the pit is
  /// dug would settle it.
  ACTOR_ROCK_PIT_MARKER
};

/// The room's first treasure, numbered from 64 by its item table: the hint in
/// the rocks, the medal's spot, the pit's find.
enum { ACTOR_HIDDEN_TREASURE = 64 };

/// The Blow Mace chest's room's actors after its stake
/// (SceneId_YampiDesertBlowMaceChest): the seven its sand slide shows and
/// hides.
enum YampiDesertBlowMaceChestActorId {
  ACTOR_MACE_CHEST_ROOM_FIRST_SLIDE = STAKE(1),
  ACTOR_MACE_CHEST_ROOM_LAST_SLIDE = ACTOR_MACE_CHEST_ROOM_FIRST_SLIDE + 6
};

void YampiDesert_RippleFollowsScorpion(void);

extern u32 gYampiDesert_SandCloudCells[];

extern u32 gYampiDesert_SandCloudDrifts[];

void YampiDesert_ScorpionUsesScoop(void);
/// Const: the cell helper only computes from its argument.
s32 YampiDesert_CellCenter(s32 cell) __attribute__((const));
extern u32 gYampiDesert_SlideDust[];
void YampiDesert_SetUpGuardianRingChest(void);
void YampiDesert_SetUpOasis(void);
void YampiDesert_SetUpBlitz(void);
void YampiDesert_SetUpPsynergyStone(void);
void YampiDesert_SetUpKingScorpion(void);

void YampiDesert_SetUpTreasurePit(void);

void YampiDesert_SetUpTrainersWhipChest(void);
void YampiDesert_SetUpMedalSpot(void);
void YampiDesert_SetUpScorpionPit(void);
void YampiDesert_SetUpOasisCave(void);
void YampiDesert_SetUpBlowMaceChest(void);

void YampiDesert_DropScoopGem(void);

void YampiDesert_TalkToElder(void);

void YampiDesert_ReadSignpost(void);

void YampiDesert_StepOffStakeTops(void);

void YampiDesert_CastSand(void);

void YampiDesert_EndSand(void);

void YampiDesert_WatchScorpionStop(void);

void YampiDesert_StirScorpion(void);

void YampiDesert_StirScorpionAgain(void);

void YampiDesert_CornerScorpion(void);

void YampiDesert_TrapKingScorpion(void);

void YampiDesert_RevealRockHint(void);

void YampiDesert_HideRockHint(void);

void YampiDesert_RevealMedalSpot(void);

void YampiDesert_ShowSlideActors(void);

void YampiDesert_ClimbRevealedLadder(void);
void YampiDesert_ClimbOutOfOasisCave(void);

extern u32 gYampiDesert_TravelerLooksAround[];

extern u32 gYampiDesert_OtherTravelerLooksAround[];

s32 MapGrid_GetTerrainKind(s32 layer, s32 x, s32 z);

void YampiDesert_StartRisingSand(s32 unused, s32 value);

extern struct SceneEntrance gYampiDesert_Entrances[];
extern struct SceneEntrance gYampiDesertOasisCaveEntrances[];
extern u32 gYampiDesertScorpionPitExitRegions[];
extern s16 gYampiDesertOasisCaveExitRegions[];

void YampiDesert_FightBlitz(void) {
  Field_RequestDjinnBattle(ACTOR_BLITZ, DJINNI_BLITZ);
}

/// Where the party can enter: the cave under the oasis has its own entrances.
const u8 *Scene_GetEntrances(void) {
  if (gMainRoutineWork.next == SCENE_YAMPI_DESERT_OASIS_CAVE)
    return gYampiDesertOasisCaveEntrances;
  return gYampiDesert_Entrances;
}

/// The regions of the rooms whose exits are rectangles: the tenth and eleventh.
const u8 *Scene_GetExitRegions(void) {
  s16 scene = gMainRoutineWork.next;

  if (scene == SCENE_YAMPI_DESERT_SCORPION_PIT)
    return gYampiDesertScorpionPitExitRegions;
  if (scene == SCENE_YAMPI_DESERT_OASIS_CAVE)
    return gYampiDesertOasisCaveExitRegions;
  return 0;
}

const u32 *Scene_GetExits(void) { return gSceneExits; }

extern struct ActorPlacement gYampiDesert_Placements[];
extern struct ActorPlacement gYampiDesertGuardianRingChestActorPlacements[];
extern struct ActorPlacement gYampiDesertOasisActorPlacements[];
extern struct ActorPlacement gYampiDesertOasisActorPlacementsAfterMeeting[];
extern struct ActorPlacement gYampiDesertBlitzActorPlacements[];
extern struct ActorPlacement gYampiDesertPsynergyStoneActorPlacements[];
extern struct ActorPlacement gYampiDesertKingScorpionActorPlacements[];
extern struct ActorPlacement gYampiDesertTrainersWhipChestActorPlacements[];
extern struct ActorPlacement gYampiDesertScorpionPitActorPlacements[];
extern struct ActorPlacement gYampiDesertBlowMaceChestActorPlacements[];

/// The current room's actors; in the oasis, the travelers stand about the
/// oasis once the party has seen them.
const struct ActorPlacement *Scene_GetActorPlacements(void) {
  s16 scene = gMainRoutineWork.next;

  if (scene == SCENE_YAMPI_DESERT_GUARDIAN_RING_CHEST)
    return gYampiDesertGuardianRingChestActorPlacements;
  if (scene == SCENE_YAMPI_DESERT_OASIS) {
    if (GameFlag_Get(FLAG_YAMPI_DESERT_TRAVELERS_MET))
      return gYampiDesertOasisActorPlacementsAfterMeeting;
    return gYampiDesertOasisActorPlacements;
  }
  if (scene == SCENE_YAMPI_DESERT_BLITZ)
    return gYampiDesertBlitzActorPlacements;
  if (scene == SCENE_YAMPI_DESERT_PSYNERGY_STONE)
    return gYampiDesertPsynergyStoneActorPlacements;
  if (scene == SCENE_YAMPI_DESERT_KING_SCORPION)
    return gYampiDesertKingScorpionActorPlacements;
  if (scene == SCENE_YAMPI_DESERT_TRAINERS_WHIP_CHEST)
    return gYampiDesertTrainersWhipChestActorPlacements;
  if (scene == SCENE_YAMPI_DESERT_SCORPION_PIT)
    return gYampiDesertScorpionPitActorPlacements;
  if (scene == SCENE_YAMPI_DESERT_BLOW_MACE_CHEST)
    return gYampiDesertBlowMaceChestActorPlacements;
  return gYampiDesert_Placements;
}

/// Scoop on the treasure pit's room's treasure pit digs it up.
void YampiDesert_ScoopTreasurePit(s32 a) {
  YampiDesert_ReturnHome(a, ACTOR_HIDDEN_TREASURE,
                         FLAG_YAMPI_DESERT_TREASURE_PIT_DUG_UP);
}

extern struct Trigger gYampiDesert_Triggers[];
extern struct Trigger gYampiDesertGuardianRingChestTriggers[];
extern struct Trigger gYampiDesertOasisTriggers[];
extern struct Trigger gYampiDesertBlitzTriggers[];
extern struct Trigger gYampiDesertPsynergyStoneTriggers[];
extern struct Trigger gYampiDesertKingScorpionTriggers[];
extern struct Trigger gYampiDesertTreasurePitTriggers[];
extern struct Trigger gYampiDesertSandPassTriggers[];
extern struct Trigger gYampiDesertTrainersWhipChestTriggers[];
extern struct Trigger gYampiDesertMedalSpotTriggers[];
extern struct Trigger gYampiDesertScorpionPitTriggers[];
extern struct Trigger gYampiDesertOasisCaveTriggers[];
extern struct Trigger gYampiDesertBlowMaceChestTriggers[];

/// Returns the current room's table.
// As ☀️'s Scene_GetEntrances picks one.
const struct Trigger *Scene_GetTriggers(void) {
  s16 scene = gMainRoutineWork.next;

  if (scene == SCENE_YAMPI_DESERT_GUARDIAN_RING_CHEST)
    return gYampiDesertGuardianRingChestTriggers;
  if (scene == SCENE_YAMPI_DESERT_OASIS)
    return gYampiDesertOasisTriggers;
  if (scene == SCENE_YAMPI_DESERT_BLITZ)
    return gYampiDesertBlitzTriggers;
  if (scene == SCENE_YAMPI_DESERT_PSYNERGY_STONE)
    return gYampiDesertPsynergyStoneTriggers;
  if (scene == SCENE_YAMPI_DESERT_KING_SCORPION)
    return gYampiDesertKingScorpionTriggers;
  if (scene == SCENE_YAMPI_DESERT_TREASURE_PIT)
    return gYampiDesertTreasurePitTriggers;
  if (scene == SCENE_YAMPI_DESERT_SAND_PASS)
    return gYampiDesertSandPassTriggers;
  if (scene == SCENE_YAMPI_DESERT_TRAINERS_WHIP_CHEST)
    return gYampiDesertTrainersWhipChestTriggers;
  if (scene == SCENE_YAMPI_DESERT_MEDAL_SPOT)
    return gYampiDesertMedalSpotTriggers;
  if (scene == SCENE_YAMPI_DESERT_SCORPION_PIT)
    return gYampiDesertScorpionPitTriggers;
  if (scene == SCENE_YAMPI_DESERT_OASIS_CAVE)
    return gYampiDesertOasisCaveTriggers;
  if (scene == SCENE_YAMPI_DESERT_BLOW_MACE_CHEST)
    return gYampiDesertBlowMaceChestTriggers;
  return gYampiDesert_Triggers;
}

/// The traveler who followed the pirates from Madra asks whether the party is
/// going to Alhafra too.
void YampiDesert_TalkToTraveler(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgWeFollowedThe);
  // "We followed the pirates here from Madra... Are you going to Alhafra..."
  Event_AskYesNoAndShowResponse(ACTOR_TRAVELER_FOLLOWING_THE_PIRATES, 0);
  Event_End();
}

/// One traveler asks to forget about Briggs, and the elder refuses: there must
/// be a way through the desert.
void YampiDesert_ElderRefusesToQuit(void) {
  Event_SetMessageId((s32)MsgCanTWe);
  Actor_SetAnimationById(ACTOR_TRAVELER_WHO_WOULD_TURN_BACK, 4);
  // "Can't we just forget about Briggs?"
  Event_ShowMessageAndWait(ACTOR_TRAVELER_WHO_WOULD_TURN_BACK, 0);
  Scene_ArmCallback(ACTOR_ELDER, FACING_WEST, 20);
  Actor_SetAnimationById(ACTOR_ELDER, 4);
  // "No. I'm not about to quit looking for him."
  Event_ShowMessageAndWait(ACTOR_ELDER, 0);
  Actor_FaceDirectionById(ACTOR_ELDER, FACING_EAST, 20);
  Scene_ArmCallback(ACTOR_ELDER, FACING_SOUTH_SOUTHEAST, 20);
  Actor_FaceDirectionById(ACTOR_ELDER, FACING_EAST, 20);
  Scene_ArmCallback(ACTOR_ELDER, FACING_SOUTH_SOUTHEAST, 0);
  // "There must be some way through this desert! We must press onward!"
  Event_ShowMessageAndWait(ACTOR_ELDER, 0);
}

/// A traveler tells how insistent the elder is on finding a way through, and
/// the elder refuses to quit.
void YampiDesert_TalkToElderAboutWay(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  if (!Scene_FlagTest(FLAG_YAMPI_DESERT_ELDER_INSISTED))
    YampiDesert_ElderRefusesToQuit();
  Actor_FaceActorById(ACTOR_TRAVELER_WHO_WOULD_TURN_BACK,
                      gMainRoutineWork.leaderActorId, 0);
  Event_SetMessageId((s32)MsgAsYouCan);
  // "As you can see, the elder is quite insistent on finding a way..."
  Event_ShowMessageAndWait(ACTOR_TRAVELER_WHO_WOULD_TURN_BACK, 0);
  GameFlag_Set(FLAG_YAMPI_DESERT_ELDER_INSISTED);
  Event_End();
}

/// The elder says this is where his son must prove himself as mayor.
void YampiDesert_TalkToElder(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  if (!Scene_FlagTest(FLAG_YAMPI_DESERT_ELDER_INSISTED))
    YampiDesert_ElderRefusesToQuit();
  Actor_FaceActorById(ACTOR_ELDER, gMainRoutineWork.leaderActorId, 0);
  Event_SetMessageId((s32)MsgThisIsWhere);
  // "This is where my son must prove himself as mayor... So, Son. How are..."
  Event_ShowMessageAndWait(ACTOR_ELDER, 0);
  GameFlag_Set(FLAG_YAMPI_DESERT_ELDER_INSISTED);
  Event_End();
}

/// Reads the signpost: north to Alhafra, south to Air's Rock.
void YampiDesert_ReadSignpost(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  // "North: Alhafra South: Air's Rock Warning: Beyond this point, there..."
  FieldMessage_Show((s32)MsgNorthAlhafraSouth, 1);
  Event_End();
}

/// Pound sinks one of the Guardian Ring chest's room's stakes.
void YampiDesert_PoundGuardianRingChestStake(s32 a, s32 b) {
  Pound_SinkTarget(gYampiDesertGuardianRingChestStakes, a, b);
}

/// Pound sinks one of Blitz's room's stakes.
void YampiDesert_PoundBlitzStake(s32 a, s32 b) {
  Pound_SinkTarget(gYampiDesertBlitzStakes, a, b);
}

#define DEFINE_POUND_STAKE_STOPPING_SCORPION(name, stakes)                     \
  void name(s32 a, s32 b) {                                                    \
    Pound_SinkTarget(stakes, a, b);                                            \
    if (a == 1) {                                                              \
      if (gYampiDesert_RaisedEvent == 0)                                       \
        gYampiDesert_RaisedEvent = RAISED_WATCH_SCORPION_STOP;                 \
    }                                                                          \
  }

DEFINE_POUND_STAKE_STOPPING_SCORPION(YampiDesert_PoundPsynergyStoneStake,
                                     gYampiDesertPsynergyStoneStakes)

/// Pound sinks the Blow Mace chest's room's stake.
void YampiDesert_PoundBlowMaceChestStake(s32 a, s32 b) {
  Pound_SinkTarget(gYampiDesertBlowMaceChestStakes, a, b);
}

DEFINE_POUND_STAKE_STOPPING_SCORPION(YampiDesert_PoundKingScorpionStake,
                                     gYampiDesertKingScorpionStakes)

/// Stepping up onto the tops of the sunken stakes: the stakes' cells take the
/// leader's height.
void YampiDesert_StepOntoStakeTops(void) {
  struct MapWork *map = Ram_HeapSlots->mapGrid;
  u16 *list = NULL;
  u8 *grid = gMapShapeGrid;

  if (gMainRoutineWork.next == SCENE_YAMPI_DESERT_BLITZ)
    list = gYampiDesertBlitzStakes;
  else if (gMainRoutineWork.next == SCENE_YAMPI_DESERT_PSYNERGY_STONE)
    list = gYampiDesertPsynergyStoneStakes;
  else if (gMainRoutineWork.next == SCENE_YAMPI_DESERT_KING_SCORPION)
    list = gYampiDesertKingScorpionStakes;
  if (list == NULL)
    return;
  while (*list != STAKE_LIST_END) {
    struct Actor *object = Actor_GetById(*list);

    list += 2;
    if (!GameFlag_Get(*list)) {
      s32 layer = object->layer;
      struct MapCell *cells = map->layers[layer].cells;
      u8 *shapes =
          (u8 *)((((s32)cells - (s32)Ram_MapCellBuffer) >> 2) + (s32)grid);
      s32 index = Actor_GetCellIndex(object);
      s32 shape =
          MapGrid_GetTerrainKind(layer, object->x.fixed, object->z.fixed);
      s32 height =
          Map_GetTerrainHeight(layer, object->x.fixed, object->z.fixed);

      cells += index;
      shapes += index;
      object->motionFlags = 0;
      cells->collisionCode = 0;
      cells->attributeB |= 128;
      *shapes = MapCollision_FindOrAddTag(layer, shape, (height >> 19) + 4);
    }
    list++;
  }
  GameFlag_Set(FLAG_YAMPI_DESERT_ON_STAKE_TOPS);
}

/// Stepping down off the tops of the sunken stakes.
void YampiDesert_StepOffStakeTops(void) {
  struct MapWork *map = Ram_HeapSlots->mapGrid;
  u16 *list = NULL;
  u8 *grid = gMapShapeGrid;

  if (gMainRoutineWork.next == SCENE_YAMPI_DESERT_BLITZ)
    list = gYampiDesertBlitzStakes;
  else if (gMainRoutineWork.next == SCENE_YAMPI_DESERT_PSYNERGY_STONE)
    list = gYampiDesertPsynergyStoneStakes;
  else if (gMainRoutineWork.next == SCENE_YAMPI_DESERT_KING_SCORPION)
    list = gYampiDesertKingScorpionStakes;
  if (list == NULL)
    return;
  if (*list == STAKE_LIST_END)
    goto done;
next:
  {
    struct Actor *object = Actor_GetById(*list);
    s32 set;

    list += 2;
    set = GameFlag_Get(*list);
    if (!set) {
      s32 layer = object->layer;
      struct MapCell *cells = map->layers[layer].cells;
      u8 *shapes =
          (u8 *)((((s32)cells - (s32)Ram_MapCellBuffer) >> 2) + (s32)grid);
      /* FAKEMATCH(pin): the index in r10. */
      register s32 index asm("r10") = Actor_GetCellIndex(object);
      s32 shape =
          MapGrid_GetTerrainKind(layer, object->x.fixed, object->z.fixed);
      s32 height =
          Map_GetTerrainHeight(layer, object->x.fixed, object->z.fixed);
      struct MapCell *cell = cells + index;

      object->motionFlags = set;
      cell->collisionCode = CELL_WALL;
      cell->attributeB &= 127;
      shapes[index] =
          MapCollision_FindOrAddTag(layer, shape, (height >> 19) - 4);
    }
  }
  list++;
  if (*list != STAKE_LIST_END)
    goto next;
done:
  GameFlag_Clear(FLAG_YAMPI_DESERT_ON_STAKE_TOPS);
}

/// For each six-byte record (actor, unused, flag) up to STAKE_LIST_END:
/// when the record's flag is set, the cell under the actor in map layer 2 takes
/// collision code 255.
void YampiDesert_BlockPoundedStakes(u16 *list) {
  struct MapWork *map = Ram_HeapSlots->mapGrid;

  for (; *list != STAKE_LIST_END; list += 3) {
    struct Actor *object = Actor_GetById(list[0]);

    if (GameFlag_Get(list[2])) {
      struct MapCell *cell = &map->layers[2].cells[Actor_GetCellIndex(object)];

      cell->collisionCode = CELL_WALL;
    }
  }
}

/// For each six-byte record up to STAKE_LIST_END, the cell under the actor
/// in map layer 2 takes collision code 232.
void YampiDesert_UnblockStakes(u16 *list) {
  struct MapWork *map = Ram_HeapSlots->mapGrid;
  struct MapCell *cell;
  s32 collisionCode;

  if (*list != STAKE_LIST_END) {
  loop:
    cell = &map->layers[2].cells[Actor_GetCellIndex(Actor_GetById(*list))];
    collisionCode = 232;
    /* FAKEMATCH(dead): the advance in a once-only block. */
    do {
      list += 3;
    } while (0);
    cell->collisionCode = collisionCode;
    if (*list != STAKE_LIST_END)
      goto loop;
  }
}

/// Sand sinks the leader into the sand's layer.
void YampiDesert_CastSand(void) {
  s16 scene;

  Actor_GetById(gMainRoutineWork.leaderActorId)->layer = 2;
  scene = gMainRoutineWork.next;
  if (scene == SCENE_YAMPI_DESERT_GUARDIAN_RING_CHEST)
    YampiDesert_BlockPoundedStakes(gYampiDesertGuardianRingChestStakes);
  else if (scene == SCENE_YAMPI_DESERT_BLITZ)
    YampiDesert_BlockPoundedStakes(gYampiDesertBlitzStakes);
  else if (scene == SCENE_YAMPI_DESERT_PSYNERGY_STONE)
    YampiDesert_BlockPoundedStakes(gYampiDesertPsynergyStoneStakes);
  else if (scene == SCENE_YAMPI_DESERT_KING_SCORPION)
    YampiDesert_BlockPoundedStakes(gYampiDesertKingScorpionStakes);
}

/// The leader rises out of the sand.
void YampiDesert_EndSand(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  s16 scene;

  leader->layer = 0;
  scene = gMainRoutineWork.next;
  if (scene == SCENE_YAMPI_DESERT_GUARDIAN_RING_CHEST)
    YampiDesert_UnblockStakes(gYampiDesertGuardianRingChestStakes);
  else if (scene == SCENE_YAMPI_DESERT_BLITZ)
    YampiDesert_UnblockStakes(gYampiDesertBlitzStakes);
  else if (scene == SCENE_YAMPI_DESERT_PSYNERGY_STONE)
    YampiDesert_UnblockStakes(gYampiDesertPsynergyStoneStakes);
  else if (scene == SCENE_YAMPI_DESERT_KING_SCORPION)
    YampiDesert_UnblockStakes(gYampiDesertKingScorpionStakes);
}

/// The camera follows the buried King Scorpion until it stops roaming.
void YampiDesert_WatchScorpionStop(void) {
  s16 scene;

  Event_Start();
  Event_ResolvePendingActions(0);
  scene = gMainRoutineWork.next;
  if (scene == SCENE_YAMPI_DESERT_PSYNERGY_STONE) {
    FieldCamera_FollowActor(ACTOR_PSYNERGY_STONE_ROOM_KING_SCORPION, 1);
    Actor_WatchAndSetScript(gMainRoutineWork.leaderActorId,
                            ACTOR_PSYNERGY_STONE_ROOM_KING_SCORPION);
  } else if (scene == SCENE_YAMPI_DESERT_KING_SCORPION) {
    FieldCamera_FollowActor(ACTOR_KING_SCORPION_ROOM_KING_SCORPION, 1);
    Actor_WatchAndSetScript(gMainRoutineWork.leaderActorId,
                            ACTOR_KING_SCORPION_ROOM_KING_SCORPION);
  }
  while (gYampiDesert_RaisedEvent == RAISED_WATCH_SCORPION_STOP)
    wait(1);
  Event_End();
}

#include "../field/field_effect_advance_motion_priority.c"

#undef EFFECT_SPAWN
#undef EFFECT_CREATE
#undef EFFECT_SET_MODE
#undef EFFECT_SET_SCRIPT
#undef EFFECT_SCRIPTS
#undef DEFINE_POUND_STAKE_STOPPING_SCORPION
// The Yampi Desert, between Osenia's south and Alhafra: twelve rooms of sand.
// Pound sinks its wooden stakes and Sand lets the party slip under them; a
// group of Madra's travelers hunting Briggs is stuck halfway (the oasis); the
// King Scorpion moves under the sand through the Psynergy Stone's room and the
// King Scorpion's room, stirred out by Pound and trapped between stakes, until
// it Scoops the leader down into its pit (the scorpion pit), where its battle
// is fought, the Scoop Gem is won, and a column of rising sand carries the
// party out. The Trainer's Whip chest's room and the Blow Mace chest's room
// have the sand slides, and Reveal shows the rocks' hints and a hidden Lucky
// Medal.

void FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                       s32 velocityZ, u32 flags,
                       const struct EffectOptions *options);

/// A burst of sand at (x, y, z): seventeen grains thrown east, south, west and
/// north in turn, each a little off at random.
void YampiDesert_BurstSand(s32 x, s32 y, s32 z) {
  struct EffectOptions options;
  u32 i;

  options.update = Effect_AdvanceMotionAtLeaderPriority;
  options.startScaleX = FIXED_ONE * 8 / 10;
  options.startScaleY = FIXED_ONE * 8 / 10;
  for (i = 0; i <= 16; i++) {
    s32 velocity[3];

    velocity[0] = Trig_Cos(i << 14);
    velocity[1] = 0;
    velocity[2] = Trig_Sin(i << 14);
    velocity[0] += velocity[0] >> 1;
    velocity[0] += ((u32)(Random16() << 17) >> 16) - toFIXED(1);
    velocity[2] += (u16)Random16() - toFIXED(1) / 2;
    FieldEffect_Spawn(x, y, z, velocity[0], velocity[1], velocity[2],
                      EFFECT_USE_UPDATE | 1, &options);
  }
}

/// Pound on the first screen's wooden column: the scorpion moving under the
/// sand bursts out and flees to the next screen.
void YampiDesert_StirScorpion(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  YampiDesert_ClearRaisedEvent();
  Actor_WatchAndSetScript(gMainRoutineWork.leaderActorId,
                          ACTOR_PSYNERGY_STONE_ROOM_KING_SCORPION);
  FieldCamera_SetSpeed(2.4);
  FieldCamera_MoveTo(300, CAMERA_KEEP, 248, 1);
  Actor_MoveToAndWaitById(ACTOR_PSYNERGY_STONE_ROOM_KING_SCORPION, 254, 248);
  Actor_MoveToAndWaitById(ACTOR_PSYNERGY_STONE_ROOM_KING_SCORPION, 352, 240);
  Task_Remove(YampiDesert_PlayScorpionRumble);
  FieldEvent_Wait(10);
  Task_Remove(YampiDesert_RippleFollowsScorpion);
  wait(1);
  Actor_SetPosition(ACTOR_PSYNERGY_STONE_ROOM_SAND_RIPPLE, 0, 0);
  FieldEvent_Wait(40);
  YampiDesert_BurstSand(toFIXED(352), 0, toFIXED(248));
  Sound_Play(SE_KOLIMA_BRIDGE_HOP);
  Actor_GetById(ACTOR_PSYNERGY_STONE_ROOM_KING_SCORPION)->velocityY =
      toFIXED(6);
  Actor_SetSpriteColor(Actor_GetById(ACTOR_PSYNERGY_STONE_ROOM_KING_SCORPION),
                       0);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_PSYNERGY_STONE_ROOM_KING_SCORPION),
                       1);
  Actor_MoveToAndWaitById(ACTOR_PSYNERGY_STONE_ROOM_KING_SCORPION, 380, 248);
  Actor_Stop(gMainRoutineWork.leaderActorId);
  Sound_Play(SE_WATER_MOVE);
  Actor_SetSpeed(ACTOR_PSYNERGY_STONE_ROOM_KING_SCORPION, 0.8);
  Actor_WalkToAndWaitById(ACTOR_PSYNERGY_STONE_ROOM_KING_SCORPION, 428, 248);
  Sound_Play(SOUND_FADE_OUT_ACTION_SE);
  Actor_SetPosition(ACTOR_PSYNERGY_STONE_ROOM_KING_SCORPION, 0, 0);
  GameFlag_Set(FLAG_YAMPI_DESERT_SCORPION_STIRRED);
  Event_End();
}

/// As YampiDesert_StirScorpion, on the second screen.
void YampiDesert_StirScorpionAgain(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  YampiDesert_ClearRaisedEvent();
  Actor_WatchAndSetScript(gMainRoutineWork.leaderActorId,
                          ACTOR_KING_SCORPION_ROOM_KING_SCORPION);
  FieldCamera_SetSpeed(2.4);
  FieldCamera_MoveTo(246, CAMERA_KEEP, 616, 1);
  Actor_MoveToAndWait(ACTOR_KING_SCORPION_ROOM_KING_SCORPION, 248, 648);
  Actor_MoveToAndWait(ACTOR_KING_SCORPION_ROOM_KING_SCORPION, 248, 576);
  Task_Remove(YampiDesert_PlayScorpionRumble);
  FieldEvent_Wait(10);
  Task_Remove(YampiDesert_RippleFollowsScorpion);
  wait(1);
  Actor_SetPosition(ACTOR_KING_SCORPION_ROOM_SAND_RIPPLE, 0, 0);
  FieldEvent_Wait(20);
  YampiDesert_BurstSand(0xf80000, 0, 0x2400000);
  Sound_Play(SE_KOLIMA_BRIDGE_HOP);
  Actor_GetById(ACTOR_KING_SCORPION_ROOM_KING_SCORPION)->velocityY = toFIXED(6);
  Actor_SetSpriteColor(Actor_GetById(ACTOR_KING_SCORPION_ROOM_KING_SCORPION),
                       0);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_KING_SCORPION_ROOM_KING_SCORPION),
                       1);
  Actor_MoveToAndWaitById(ACTOR_KING_SCORPION_ROOM_KING_SCORPION, 256, 560);
  FieldEvent_Wait(20);
  Actor_Stop(gMainRoutineWork.leaderActorId);
  Sound_Play(SE_WATER_MOVE);
  Actor_SetSpeed(ACTOR_KING_SCORPION_ROOM_KING_SCORPION, 0.8);
  Actor_WalkToAndWait(ACTOR_KING_SCORPION_ROOM_KING_SCORPION, 248, 496);
  Sound_Play(SOUND_FADE_OUT_ACTION_SE);
  Actor_SetPosition(ACTOR_KING_SCORPION_ROOM_KING_SCORPION, 0, 0);
  GameFlag_Set(FLAG_YAMPI_DESERT_SCORPION_STIRRED_AGAIN);
  Event_End();
}

/// Pounded out of the sand, the King Scorpion runs north into the trap and is
/// cornered there.
void YampiDesert_CornerScorpion(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  YampiDesert_ClearRaisedEvent();
  Actor_WatchAndSetScript(gMainRoutineWork.leaderActorId,
                          ACTOR_KING_SCORPION_ROOM_KING_SCORPION);
  FieldCamera_SetSpeed(2.4);
  FieldCamera_MoveTo(232, CAMERA_KEEP, 328, 1);
  Actor_MoveToAndWait(ACTOR_KING_SCORPION_ROOM_KING_SCORPION, 232, 328);
  FieldCamera_MoveTo(232, CAMERA_KEEP, 200, 1);
  Actor_MoveToAndWait(ACTOR_KING_SCORPION_ROOM_KING_SCORPION, 232, 280);
  Actor_MoveToAndWaitById(ACTOR_KING_SCORPION_ROOM_KING_SCORPION, 232, 168);
  Task_Remove(YampiDesert_PlayScorpionRumble);
  Actor_FaceDirectionById(ACTOR_KING_SCORPION_ROOM_KING_SCORPION, FACING_SOUTH,
                          0);
  Actor_Stop(gMainRoutineWork.leaderActorId);
  FieldEvent_Wait(20);
  GameFlag_Set(FLAG_YAMPI_DESERT_SCORPION_CORNERED);
  Event_End();
}

void Actor_StartScript(struct Actor *object, const void *script);

/// Spawns an object of kind 285 at each of the fifteen cells of
/// gYampiDesert_SandCloudCells, twice, each moved by -2 to +1 pixels at random.
void YampiDesert_RaiseSandCloud(void) {
  u32 pass;
  u32 i;

  for (pass = 0; pass <= 1; pass++) {
    u16 *cell;

    cell = (u16 *)gYampiDesert_SandCloudCells;
    for (i = 0; i <= 14; i++) {
      s32 x = *cell++ + ((Random16() << 2) >> 16);
      s32 z = *cell++ + ((Random16() << 2) >> 16);
      struct Actor *object = Import_ActorCreate(
          SPRITE_DUST_CLOUD, INTtoFIXED(x - 2), 0, INTtoFIXED(z - 2));

      if (object != NULL) {
        object->speedLimit = toFIXED(9) / 8;
        object->acceleration = toFIXED(1) / 2;
        Actor_SetSpriteFlags((struct Actor *)object, 0);
        Actor_SelectAnimation((struct Actor *)object, 1);
        Actor_StartScript(object, gYampiDesert_SandCloudDrifts);
      }
    }
  }
  MapGrid_CopyCellPictureIndicesRect(74, 71, 77, 9, 3, 3);
}

#define OBJ(p) ((struct Actor *)(p))

/// The trapped scorpion uses Scoop, the leader falls with it, and the King
/// Scorpion's battle begins.
void YampiDesert_TrapKingScorpion(void) {
  struct Actor *leader = OBJ(Actor_GetById(gMainRoutineWork.leaderActorId));
  struct Actor *partner =
      OBJ(Actor_GetById(ACTOR_KING_SCORPION_ROOM_KING_SCORPION));
  u32 i;
  s32 angle;

  if (!GameFlag_Get(FLAG_YAMPI_DESERT_SCORPION_CORNERED))
    return;
  Event_Start();
  Event_ResolvePendingActions(0);
rippleShrinks:
  OBJ(gYampiDesert_SandRipple)->scaleX -= FIXED_ONE * 8 / 100;
  OBJ(gYampiDesert_SandRipple)->scaleY -= FIXED_ONE * 8 / 100;
  wait(1);
  if (OBJ(gYampiDesert_SandRipple)->scaleX >= FIXED_ONE * 8 / 10)
    goto rippleShrinks;
rippleGrows:
  OBJ(gYampiDesert_SandRipple)->scaleX += FIXED_ONE / 10;
  OBJ(gYampiDesert_SandRipple)->scaleY += FIXED_ONE / 10;
  wait(1);
  if (OBJ(gYampiDesert_SandRipple)->scaleX <= FIXED_ONE * 18 / 10)
    goto rippleGrows;
rippleFades:
  OBJ(gYampiDesert_SandRipple)->scaleX -= FIXED_ONE * 4 / 100;
  OBJ(gYampiDesert_SandRipple)->scaleY -= FIXED_ONE * 4 / 100;
  wait(1);
  if (OBJ(gYampiDesert_SandRipple)->scaleX >= FIXED_ONE * 2 / 10)
    goto rippleFades;
  Actor_SetPosition(ACTOR_KING_SCORPION_ROOM_SAND_RIPPLE, 0, 0);
  FieldEvent_Wait(20);
  FieldCamera_MoveTo(232, CAMERA_KEEP, 196, 1);
  Actor_ShowEmoteById(gMainRoutineWork.leaderActorId,
                      EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_SetSpeed(gMainRoutineWork.leaderActorId, 0.8);
  Actor_WalkToAndWaitById(gMainRoutineWork.leaderActorId, 232, 164);
  FieldEvent_Wait(20);
  angle = FACING_SOUTH;
  YampiDesert_SetPositionAndFacing(ACTOR_KING_SCORPION_ROOM_KING_SCORPION,
                                   toFIXED(232), toFIXED(184), angle);
  wait(1);
  YampiDesert_BurstSand(toFIXED(232), 0, toFIXED(184));
  Sound_Play(SE_KOLIMA_BRIDGE_HOP);
  partner->velocityY = toFIXED(6);
  partner->priorityFlags &= ~ACTOR_PRIORITY_UNDERFOOT;
  Actor_SetSpritePriority(ACTOR_KING_SCORPION_ROOM_KING_SCORPION, 2);
  Actor_SetSpriteColor(Actor_GetById(ACTOR_KING_SCORPION_ROOM_KING_SCORPION),
                       0);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_KING_SCORPION_ROOM_KING_SCORPION),
                       1);
  Actor_SetSpeed(ACTOR_KING_SCORPION_ROOM_KING_SCORPION, 1.6);
  Actor_MoveToAndWaitById(ACTOR_KING_SCORPION_ROOM_KING_SCORPION, 216, 184);
  Actor_FaceDirectionById(gMainRoutineWork.leaderActorId, angle, 0);
  Actor_FaceDirectionAndRefreshById(ACTOR_KING_SCORPION_ROOM_KING_SCORPION,
                                    FACING_EAST);
  FieldEvent_Wait(20);
  YampiDesert_ScorpionUsesScoop();
  Sound_Play(SE_WHOOSH);
  YampiDesert_RaiseSandCloud();
  Actor_SetSpriteFlags(Actor_GetById(gMainRoutineWork.leaderActorId), 0);
  FieldEvent_Wait(20);
  FieldEffect_SetAttached(gMainRoutineWork.leaderActorId,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAnimationById(gMainRoutineWork.leaderActorId, 27);
  FieldEvent_Wait(30);
  Actor_SetAnimationById(gMainRoutineWork.leaderActorId, 28);
  FieldEvent_Wait(10);
  Sound_Play(SE_DUST_PUFF);
  i = 0;
leaderSinks:
  leader->scaleX -= FIXED_ONE / 64;
  leader->scaleY -= FIXED_ONE / 64;
  leader->y.fixed += -toFIXED(2);
  wait(1);
  i++;
  if (i <= 13)
    goto leaderSinks;
  Actor_SetSpriteColor(Actor_GetById(gMainRoutineWork.leaderActorId), 15);
  Sound_Play(SE_HIGH_JUMP);
  partner->velocityY = toFIXED(8);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_KING_SCORPION_ROOM_KING_SCORPION),
                       0);
  Actor_MoveToAndWaitById(ACTOR_KING_SCORPION_ROOM_KING_SCORPION, 232, 168);
  FieldEvent_Wait(3);
  Sound_Play(SE_DUST_PUFF);
  i = 0;
scorpionSinks:
  partner->scaleX -= FIXED_ONE / 32;
  partner->scaleY -= FIXED_ONE / 32;
  partner->y.fixed += -toFIXED(7);
  wait(1);
  i++;
  if (i <= 5)
    goto scorpionSinks;
  Actor_SetPosition(ACTOR_KING_SCORPION_ROOM_KING_SCORPION, 0, 0);
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Import_EventSetExitRequest(4);
}

/// Scoop on the sand pit between the four rocks.
void YampiDesert_ScoopCenterPit(s32 mode) {
  struct Actor *object;
  s32 height;

  if (mode == 1) {
    object = Actor_GetById(ACTOR_ROCK_PIT_MARKER);
    MapGrid_CopyCellPictureIndicesRect(35, 39, 13, 11, 3, 3);
    Actor_SetPosition(ACTOR_ROCK_PIT_MARKER, 232, 200);
    height = Map_GetTerrainHeight(0, object->x.fixed, object->z.fixed);
    object->y.fixed = height;
    object->groundHeight = height;
    GameFlag_Set(FLAG_YAMPI_DESERT_ROCK_PIT_DUG_UP);
  }
}

s32 YampiDesert_CellCenter(s32 cell) { return cell * 16 + 8; }

/// Moves the leader to the centre of a cell, in pixels.
#define MOVE_LEADER_TO(column, row)                                            \
  {                                                                            \
    s32 x_ = YampiDesert_CellCenter(column);                                   \
    s32 z_ = YampiDesert_CellCenter(row);                                      \
    Actor_SetHorizontalPositionWithTerrain(gMainRoutineWork.leaderActorId,     \
                                           INTtoFIXED(x_), INTtoFIXED(z_));    \
  }

/// Reveal shows the hint in the rocks of the Trainer's Whip chest's room.
void YampiDesert_RevealRockHint(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  struct Actor *object = Actor_GetById(ACTOR_HIDDEN_TREASURE);
  s32 column = FIELD_CELL(leader->x.fixed);
  s32 row = FIELD_CELL(leader->z.fixed);

  if (object != NULL)
    Actor_SetPosition(ACTOR_HIDDEN_TREASURE, 616, 444);
  MapGrid_CopyCellPictureIndicesRect(82, 82, 15, 40, 5, 6);
  MapGrid_CopyCellAttributes(82, 82, 5, 6, 15, 40);
  MapGrid_CopyCellPictureIndicesRect(88, 82, 36, 25, 5, 5);
  MapGrid_CopyCellAttributes(88, 82, 5, 5, 36, 25);
  if (column == 15) {
    if (row == 41)
      MOVE_LEADER_TO(14, 41)
    else if (row == 43)
      MOVE_LEADER_TO(16, 43)
  } else if (column == 16) {
    if (row == 40)
      MOVE_LEADER_TO(16, 39)
    else if (row == 41)
      MOVE_LEADER_TO(16, 42)
  } else if (column == 18) {
    if (row == 40)
      MOVE_LEADER_TO(18, 41)
    else if (row == 43)
      MOVE_LEADER_TO(17, 43)
  }
}

/// Moves the leader to the centre of a cell, in pixels.
#define MOVE_LEADER_TO(column, row)                                            \
  {                                                                            \
    struct MainRoutineWork *work = &gMainRoutineWork;                          \
    s32 x = YampiDesert_CellCenter(column);                                    \
    s32 z = YampiDesert_CellCenter(row);                                       \
    Actor_SetHorizontalPositionWithTerrain(work->leaderActorId, INTtoFIXED(x), \
                                           INTtoFIXED(z));                     \
  }

/// As Reveal fades, the Trainer's Whip chest's room's hint is hidden again.
void YampiDesert_HideRockHint(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  s32 column = FIELD_CELL(leader->x.fixed);
  s32 row = FIELD_CELL(leader->z.fixed);

  if (Actor_GetById(ACTOR_HIDDEN_TREASURE) != NULL)
    Actor_SetPosition(ACTOR_HIDDEN_TREASURE, 0, 0);
  MapGrid_CopyCellPictureIndicesRect(82, 89, 15, 40, 5, 6);
  MapGrid_CopyCellAttributes(82, 89, 5, 6, 15, 40);
  MapGrid_CopyCellPictureIndicesRect(88, 89, 36, 25, 5, 5);
  MapGrid_CopyCellAttributes(88, 89, 5, 5, 36, 25);
  if (column == 15 && row == 44)
    MOVE_LEADER_TO(15, 43)
  else if (column == 17 && row == 44)
    MOVE_LEADER_TO(17, 43)
  else if (column == 19) {
    if (row == 40)
      MOVE_LEADER_TO(19, 41)
    else if (row == 42)
      MOVE_LEADER_TO(18, 42)
  }
}

/// Reveal shows the hidden spot of the medal spot's room.
void YampiDesert_RevealMedalSpot(void) {
  struct Actor *object = Actor_GetById(ACTOR_HIDDEN_TREASURE);
  struct Actor *leader;

  if (object != 0) {
    leader = Actor_GetById(gMainRoutineWork.leaderActorId);
    Actor_SetPosition(ACTOR_HIDDEN_TREASURE, 288, 240);
    if (FIELD_CELL(leader->x.fixed + toFIXED(8)) ==
            FIELD_CELL(object->x.fixed + toFIXED(8)) &&
        FIELD_CELL(leader->z.fixed + toFIXED(8)) ==
            FIELD_CELL(object->z.fixed + toFIXED(8)))
      Actor_SetPosition(gMainRoutineWork.leaderActorId, 288, 256);
  }
}

/// As Reveal fades, the medal spot's room's hidden spot is covered again.
void YampiDesert_HideMedalSpot(void) {
  if (Actor_GetById(ACTOR_HIDDEN_TREASURE))
    Actor_SetPosition(ACTOR_HIDDEN_TREASURE, 0, 0);
}

/// Opens the sand pit over the ladder in the oasis.
void YampiDesert_OpenLadderPit(void) {
  MapGrid_CopyCellPictureIndicesRect(64, 64, 72, 32, 1, 1);
  Map_CopyCellAttributeRect(64, 64, 1, 1, 8, 32);
  Map_CopyCellAttributeRect(64, 64, 1, 1, 8, 96);
}

/// Scoop on the sand pit that hides the ladder.
void YampiDesert_ScoopLadderPit(s32 mode) {
  if (mode == 1) {
    YampiDesert_OpenLadderPit();
    GameFlag_Set(FLAG_YAMPI_DESERT_LADDER_DUG_UP);
  }
}

/// Shows or hides the sand slide's actors of the eighth or twelfth room.
void YampiDesert_SetSlideActorsShown(s32 show) {
  s16 scene = gMainRoutineWork.next;

  if (scene == SCENE_YAMPI_DESERT_TRAINERS_WHIP_CHEST)
    YampiDesert_SetActorsHidden(show, ACTOR_WHIP_CHEST_ROOM_FIRST_SLIDE, 2);
  else if (scene == SCENE_YAMPI_DESERT_BLOW_MACE_CHEST)
    YampiDesert_SetActorsHidden(show, ACTOR_MACE_CHEST_ROOM_FIRST_SLIDE,
                                ACTOR_MACE_CHEST_ROOM_LAST_SLIDE -
                                    ACTOR_MACE_CHEST_ROOM_FIRST_SLIDE + 1);
}

/// Shows the sand slide's actors.
void YampiDesert_ShowSlideActors(void) {
  YampiDesert_SetSlideActorsShown(1);
  GameFlag_Set(FLAG_YAMPI_DESERT_SLIDE_ACTORS_SHOWN);
}

/// Hides the sand slide's actors.
void YampiDesert_HideSlideActors(void) {
  YampiDesert_SetSlideActorsShown(0);
  GameFlag_Clear(FLAG_YAMPI_DESERT_SLIDE_ACTORS_SHOWN);
}

/// Shows or hides `count` actors from `first`.
void YampiDesert_SetActorsHidden(s32 show, s32 first, u32 count) {
  u32 i;

  for (i = 0; i < count; i++) {
    struct Actor *actor = Actor_GetById(first + i);

    if (show)
      actor->priorityFlags &= ~ACTOR_PRIORITY_NOT_DRAWN;
    else
      actor->priorityFlags |= ACTOR_PRIORITY_NOT_DRAWN;
  }
}

extern u32 gYampiDesert_FinderRunsAhead[];
extern u32 gYampiDesert_TravelersFollow[];
extern u32 gYampiDesert_LastTravelersFollow[];

/// From the ledge, the party sees the travelers by the oasis below find their
/// way through.
void YampiDesert_SeeTravelersAbove(s32 p0) {
  struct MainRoutineWork *state;
  s32 angle;

  Call1((void (*)())Event_Start, p0);
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgHeyLookUp);
  Actor_Stop(ACTOR_TRAVELER_SEEKING_EVIDENCE);
  wait(1);
  Actor_ShowEmote(ACTOR_TRAVELER_SEEKING_EVIDENCE,
                  EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_FaceDirection(ACTOR_TRAVELER_SEEKING_EVIDENCE, FACING_EAST, 20);
  // "Hey! Look! Up there!"
  Event_ShowMessageAndWait(ACTOR_TRAVELER_SEEKING_EVIDENCE, 0);
  Actor_Stop(ACTOR_TRAVELER_FOLLOWING_THE_PIRATES);
  wait(1);
  Actor_FaceDirection(ACTOR_TRAVELER_FOLLOWING_THE_PIRATES, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_DIZZY_TRAVELER, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_PARCHED_TRAVELER, FACING_NORTH_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_TRAVELER_WHO_WOULD_TURN_BACK,
                      FACING_NORTH_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_ELDER, FACING_NORTH_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_MAYOR, FACING_NORTH_NORTHEAST, 0);
  state = &gMainRoutineWork;
  FieldActor_ShowEmote(state->leaderActorId, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                       0);
  Actor_FaceDirection(state->leaderActorId, FACING_WEST, 0);
  ((struct Actor *)FieldCamera_GetActor())->motionFlags = 0;
  FieldCamera_SetSpeed(1.6);
  FieldCamera_MoveTo(212, 0, 212, 1);
  FieldCamera_WaitForArrival();
  // "How did those guys get up there?"
  Event_ShowMessageAndWait(ACTOR_TRAVELER_WHO_WOULD_TURN_BACK, 0);
  Actor_FaceDirection(ACTOR_PARCHED_TRAVELER, FACING_EAST, 10);
  // "Maybe there's a path up ahead that we missed..."
  Event_ShowMessageAndWait(ACTOR_PARCHED_TRAVELER, 0);
  Actor_FaceDirection(ACTOR_ELDER, FACING_NORTH_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_MAYOR, FACING_SOUTH_SOUTHWEST, 20);
  Actor_Jump(ACTOR_MAYOR, 4, 40);
  Actor_SetAnimationAndWait(ACTOR_ELDER, 3);
  Actor_SetAnimationAndWait(ACTOR_MAYOR, 3);
  Actor_WatchAndSetScript(ACTOR_TRAVELER_FOLLOWING_THE_PIRATES, ACTOR_MAYOR);
  Actor_WatchAndSetScript(ACTOR_TRAVELER_SEEKING_EVIDENCE, ACTOR_MAYOR);
  Actor_WatchAndSetScript(ACTOR_DIZZY_TRAVELER, ACTOR_MAYOR);
  Actor_WatchAndSetScript(ACTOR_PARCHED_TRAVELER, ACTOR_MAYOR);
  Actor_WatchAndSetScript(ACTOR_TRAVELER_WHO_WOULD_TURN_BACK, ACTOR_MAYOR);
  Actor_WatchAndSetScript(ACTOR_ELDER, ACTOR_MAYOR);
  Actor_SetSpeed(ACTOR_ELDER, 1);
  Actor_SetMovementTypeById(ACTOR_MAYOR, (s32)gYampiDesert_FinderRunsAhead);
  FieldCamera_SetSpeed(0.8);
  FieldCamera_MoveTo(240, CAMERA_KEEP, 260, 1);
  FieldCamera_WaitForArrival();
  Actor_Stop(ACTOR_TRAVELER_FOLLOWING_THE_PIRATES);
  Actor_Stop(ACTOR_TRAVELER_SEEKING_EVIDENCE);
  Actor_Stop(ACTOR_DIZZY_TRAVELER);
  Actor_Stop(ACTOR_PARCHED_TRAVELER);
  Actor_Stop(ACTOR_TRAVELER_WHO_WOULD_TURN_BACK);
  Actor_Stop(ACTOR_ELDER);
  FieldCamera_SetSpeed(2.4);
  FieldCamera_MoveTo(212, CAMERA_KEEP, 212, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(20);
  Actor_FaceDirection(ACTOR_TRAVELER_FOLLOWING_THE_PIRATES,
                      FACING_NORTH_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_TRAVELER_SEEKING_EVIDENCE, FACING_SOUTH_SOUTHWEST,
                      20);
  // "Maybe... Maybe we can get through the desert after all!"
  Event_ShowMessageAndWait(ACTOR_TRAVELER_SEEKING_EVIDENCE, 0);
  Actor_SetAnimationAndWait(ACTOR_TRAVELER_FOLLOWING_THE_PIRATES, 3);
  // "We can find a way through to Alhafra!"
  Event_ShowMessageAndWait(ACTOR_TRAVELER_FOLLOWING_THE_PIRATES, 0);
  Actor_Jump(ACTOR_DIZZY_TRAVELER, 4, 0);
  Actor_FaceDirection(ACTOR_DIZZY_TRAVELER, FACING_EAST, 0);
  Actor_SetAnimationById(ACTOR_DIZZY_TRAVELER, 4);
  // "I don't care where we go, as long as it's cool."
  Event_ShowMessageAndWait(ACTOR_DIZZY_TRAVELER, 0);
  Actor_FaceDirection(ACTOR_TRAVELER_FOLLOWING_THE_PIRATES, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_TRAVELER_SEEKING_EVIDENCE, FACING_WEST, 0);
  Actor_ShowEmote(ACTOR_TRAVELER_FOLLOWING_THE_PIRATES,
                  EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Actor_ShowEmoteById(ACTOR_TRAVELER_SEEKING_EVIDENCE,
                      EMOTE_IN_FRONT | EMOTE_WINCE, 80);
  // "Hey! I found it! I found a way through!"
  Event_ShowMessageAndWait(ACTOR_MAYOR, 0);
  Actor_FaceDirection(ACTOR_TRAVELER_FOLLOWING_THE_PIRATES,
                      FACING_NORTH_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_DIZZY_TRAVELER, FACING_SOUTH_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_PARCHED_TRAVELER, FACING_NORTH_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_TRAVELER_WHO_WOULD_TURN_BACK, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_ELDER, FACING_WEST, 20);
  Actor_SetAnimationById(ACTOR_TRAVELER_FOLLOWING_THE_PIRATES, 3);
  Actor_SetAnimationById(ACTOR_TRAVELER_SEEKING_EVIDENCE, 3);
  Actor_SetAnimationById(ACTOR_DIZZY_TRAVELER, 3);
  Actor_SetAnimationById(ACTOR_PARCHED_TRAVELER, 3);
  Actor_SetAnimationById(ACTOR_TRAVELER_WHO_WOULD_TURN_BACK, 3);
  Actor_SetAnimationAndWait(ACTOR_ELDER, 3);
  Actor_SetSpeed(ACTOR_TRAVELER_FOLLOWING_THE_PIRATES, 1.6);
  Actor_SetSpeed(ACTOR_TRAVELER_SEEKING_EVIDENCE, 1.6);
  Actor_SetSpeed(ACTOR_DIZZY_TRAVELER, 1.6);
  Actor_SetSpeed(ACTOR_PARCHED_TRAVELER, 1.6);
  Actor_SetSpeed(ACTOR_TRAVELER_WHO_WOULD_TURN_BACK, 1.6);
  Actor_SetSpeed(ACTOR_ELDER, 1.6);
  Actor_WatchAndSetScript(state->leaderActorId, ACTOR_ELDER);
  {
    s32 path = (s32)gYampiDesert_TravelersFollow;

    Actor_SetMovementTypeById(ACTOR_ELDER, path);
    FieldEvent_Wait(10);
    Actor_SetMovementTypeById(ACTOR_TRAVELER_WHO_WOULD_TURN_BACK, path);
    FieldCamera_SetSpeed(0.4);
    FieldCamera_MoveTo(240, CAMERA_KEEP, 280, 1);
    Actor_SetMovementTypeById(ACTOR_TRAVELER_FOLLOWING_THE_PIRATES, path);
    FieldEvent_Wait(10);
    Actor_SetMovementTypeById(ACTOR_TRAVELER_SEEKING_EVIDENCE, path);
  }
  {
    s32 path = (s32)gYampiDesert_LastTravelersFollow;

    Actor_SetMovementTypeById(ACTOR_DIZZY_TRAVELER, path);
    FieldEvent_Wait(20);
    Actor_SetMovementTypeAndWaitById(ACTOR_PARCHED_TRAVELER, path);
  }
  Actor_Stop(state->leaderActorId);
  wait(1);
  Actor_SetPosition(ACTOR_TRAVELER_FOLLOWING_THE_PIRATES, 0, 0);
  Actor_SetPosition(ACTOR_TRAVELER_SEEKING_EVIDENCE, 0, 0);
  Actor_SetPosition(ACTOR_DIZZY_TRAVELER, 0, 0);
  Actor_SetPosition(ACTOR_PARCHED_TRAVELER, 0, 0);
  Actor_SetPosition(ACTOR_TRAVELER_WHO_WOULD_TURN_BACK, 0, 0);
  Actor_SetPosition(ACTOR_ELDER, 0, 0);
  Actor_SetPosition(ACTOR_MAYOR, 0, 0);
  angle = FACING_NORTH_NORTHWEST;
  YampiDesert_SetPositionAndFacing(ACTOR_TRAVELER_FOLLOWING_THE_PIRATES,
                                   toFIXED(520), toFIXED(242), angle);
  {
    s32 facing = FACING_WEST;

    YampiDesert_SetPositionAndFacing(ACTOR_TRAVELER_SEEKING_EVIDENCE,
                                     toFIXED(533), toFIXED(271), facing);
  }
  {
    s32 facing = FACING_SOUTH_SOUTHEAST;

    YampiDesert_SetPositionAndFacing(ACTOR_DIZZY_TRAVELER, toFIXED(435),
                                     toFIXED(232), facing);
  }
  {
    s32 facing = FACING_SOUTH_SOUTHWEST;

    YampiDesert_SetPositionAndFacing(ACTOR_PARCHED_TRAVELER, toFIXED(467),
                                     toFIXED(248), facing);
  }
  {
    s32 facing = FACING_NORTH_NORTHEAST;

    YampiDesert_SetPositionAndFacing(ACTOR_TRAVELER_WHO_WOULD_TURN_BACK,
                                     toFIXED(462), toFIXED(340), facing);
    YampiDesert_SetPositionAndFacing(ACTOR_ELDER, toFIXED(482), toFIXED(340),
                                     facing);
  }
  YampiDesert_SetPositionAndFacing(ACTOR_MAYOR, toFIXED(489), toFIXED(320),
                                   angle);
  Actor_SetAnimationById(ACTOR_TRAVELER_FOLLOWING_THE_PIRATES, 1);
  Actor_SetAnimationById(ACTOR_TRAVELER_SEEKING_EVIDENCE, 1);
  Actor_SetAnimationById(ACTOR_DIZZY_TRAVELER, 1);
  Actor_SetAnimationById(ACTOR_PARCHED_TRAVELER, 1);
  Actor_SetAnimationById(ACTOR_TRAVELER_WHO_WOULD_TURN_BACK, 1);
  Actor_SetAnimationById(ACTOR_ELDER, 1);
  Actor_SetAnimationById(ACTOR_MAYOR, 1);
  GameFlag_Set(FLAG_YAMPI_DESERT_TRAVELERS_MET);
  GameFlag_Set(FLAG_YAMPI_DESERT_ELDER_INSISTED);
  Event_End();
}

/// The Scoop Gem (item 209's icon) as an actor, after the King Scorpion's
/// battle.
struct Actor *YampiDesert_CreateScoopGem(void) {
  struct Actor *object =
      Import_ActorCreate(489, toFIXED(232), toFIXED(32), toFIXED(200));
  u8 *buffer = 0;

  struct YampiDesertVisual *visual;
  s32 mask;

  if (object == 0)
    return 0;
  {
    visual = object->sprite;

    {
      s32 keep = 33;

      keep = -keep;
      visual->flags05 &= keep;
    }
    mask = visual->flags09 & 0x0f;
    {
      s32 clear = 13;

      clear = -clear;
      mask &= clear;
    }
    mask |= 4;
    visual->flags09 = mask;
    object->motionFlags = 0;
    object->retainSpriteCache = 1;
    visual->value1a = 0;
    visual->value1b = 0;
    buffer = HeapSlot_Alloc(HEAP_SLOT_GLYPH_TRANSFER, 0x608);
    Icon_DrawItem(ITEM_SCOOP_GEM);
    VramCache_Load(visual->tileSlot, 128, buffer + 0x400);
    HeapSlot_Release(HEAP_SLOT_GLYPH_TRANSFER);
  }
  return object;
}

/// After the battle, the King Scorpion leaves the Scoop Gem behind.
void YampiDesert_DropScoopGem(void) {
  struct Actor *object;

  Event_Start();
  Event_ResolvePendingActions(0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_Wait(20);
  Actor_SetAnimationById(gMainRoutineWork.leaderActorId, 28);
  object = YampiDesert_CreateScoopGem();
  PartyInventory_GiveItem(ITEM_SCOOP_GEM, 0);
  if (object != 0) {
    Actor_SetPositionAndResetMotion((struct Actor *)object, 0, 0, 0);
    wait(1);
    Actor_Release(object);
  }
  Actor_SetAnimationById(gMainRoutineWork.leaderActorId, ANIM_STAND);
  GameFlag_Set(FLAG_KING_SCORPION_BEATEN);
  Event_End();
}

/// Keeps the ripple of sand over the buried King Scorpion.
void YampiDesert_RippleFollowsScorpion(void) {
  struct Actor *object;
  s16 scene;

  gFieldWork->raisedTrigger = gYampiDesert_RaisedEvent;
  if (gYampiDesert_SandRipple != 0) {
    scene = gMainRoutineWork.next;
    if (scene == SCENE_YAMPI_DESERT_PSYNERGY_STONE)
      object = Actor_GetById(ACTOR_PSYNERGY_STONE_ROOM_KING_SCORPION);
    else if (scene == SCENE_YAMPI_DESERT_KING_SCORPION)
      object = Actor_GetById(ACTOR_KING_SCORPION_ROOM_KING_SCORPION);
    else
      return;
    gYampiDesert_SandRipple->x.fixed = object->x.fixed;
    gYampiDesert_SandRipple->z.fixed = object->z.fixed;
    gYampiDesert_SandRipple->facing = object->facing;
  }
}
#undef OBJ
#undef MOVE_LEADER_TO

void Actor_StartScript(struct Actor *object, const void *script);

/// Slides the leader down a sand slide by (dx, dz), throwing up dust every
/// other frame.
void YampiDesert_SlideLeader(struct Actor *object, s32 dx, s32 dz) {
  Actor_SetSpriteFlags(Actor_GetById(gMainRoutineWork.leaderActorId), 0);
  if (gMainRoutineWork.movementMode != 5) {
    u32 odd = gFrameCount & 1;

    if (odd == 0) {
      struct Actor *dust = (struct Actor *)Value4(
          (s32 (*)())Import_ActorCreate, SPRITE_DUST_CLOUD, object->x.fixed,
          object->y.fixed, object->z.fixed);

      if (dust != NULL) {
        struct Sprite *sprite;

        dust->motionFlags = odd;
        dust->collisionFlags = odd;
        Actor_SetSpriteFlags(dust, 0);
        Actor_SelectAnimation(dust, 1);
        Actor_StartScript(dust, gYampiDesert_SlideDust);
        sprite = dust->sprite;
        sprite->image.priority = 1;
        if (gInput.held != 0) {
          dust->bounce = Trig_Cos(((Random16() << 3) >> 16) << 14) + dx;
          dust->riseBaseY = Trig_Sin(((Random16() << 3) >> 16) << 14) + dz;
          if (gInput.held & KEY_B) {
            dust->bounce += dx;
            dust->riseBaseY += dz;
          }
        } else {
          dust->bounce = (Trig_Cos(((Random16() << 3) >> 16) << 14) >> 1) + dx;
          dust->riseBaseY =
              (Trig_Sin(((Random16() << 3) >> 16) << 14) >> 1) + dz;
        }
        dust->gravity = 0;
        dust->timer = ((Random16() << 9) >> 16) - 256;
        dust->speedLimit = 0;
        dust->acceleration = 0;
        dust->update = Effect_AdvanceMotionAtLeaderPriority;
      }
    }
  }
  if (dx != 0) {
    object->x.fixed += dx;
    if (!(gInput.held & KEYS_DPAD)) {
      s32 z = FIXEDtoINT(object->z.fixed);

      object->z.fixed += INTtoFIXED(8 - z % 16) >> 3;
    }
  } else {
    object->z.fixed += dz;
    if (!(gInput.held & KEYS_DPAD)) {
      s32 x = FIXEDtoINT(object->x.fixed);

      object->x.fixed += INTtoFIXED(8 - x % 16) >> 3;
    }
  }
}

void TilesetAnim_SuspendAll(void);
void TilesetAnim_ResumeAll(void);

/// The task of the rooms with sand slides: a leader on a slide's cells slides
/// down it.
void YampiDesert_UpdateSandSlide(void) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  struct Actor *leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  s32 speed = 0x18000;
  struct MapWork *map;
  struct MapCell *here, *ahead, *under;
  s32 index, column, row;
  struct MapCell *cells;

  if (work->psynergy != 0 || work->fieldMenu != 0 || work->pauseMenu != 0) {
    Actor_SetAnimationSpeed(leader, 0);
    TilesetAnim_SuspendAll();
    return;
  }
  map = Ram_HeapSlots->mapGrid;
  index = Actor_GetCellIndex(leader);
  column = leader->x.fixed / toFIXED(16);
  row = (leader->z.fixed + toFIXED(4)) / toFIXED(16);
  cells = map->layers[0].cells;
  here = cells + index;
  ahead = cells + (column + row * MAPGRID_SIZE);
  under = map->layers[1].cells + index;
  Actor_SetAnimationSpeed(leader, ANIMATION_STEP_NORMAL);
  TilesetAnim_ResumeAll();
  if ((u8)(under->collisionCode - CELL_SLIDE_NORTH) <= 4) {
    if (gMainRoutineWork.movementMode == 5)
      speed = 0;
    else
      Actor_SetSpriteFlags(Actor_GetById(gMainRoutineWork.leaderActorId), 1);
  }
  if (ahead->collisionCode == CELL_WALL)
    speed = 0;
  switch (here->collisionCode) {
  case CELL_SLIDE_NORTH:
    if (gInput.held & KEY_UP) {
      YampiDesert_SlideLeader(leader, 0, -speed >> 2);
      return;
    }
    break;
  case CELL_SLIDE_SOUTH:
    if (gInput.held & KEY_DOWN) {
      YampiDesert_SlideLeader(leader, 0, speed >> 2);
      return;
    }
    break;
  case CELL_SLIDE_WEST:
    if (gInput.held & KEY_LEFT) {
      YampiDesert_SlideLeader(leader, -speed >> 2, 0);
      return;
    }
    break;
  case CELL_SLIDE_EAST:
    if (gInput.held & KEY_RIGHT) {
      YampiDesert_SlideLeader(leader, speed >> 2, 0);
      return;
    }
    break;
  }
  switch (under->collisionCode) {
  case CELL_SLIDE_NORTH:
    YampiDesert_SlideLeader(leader, 0, -speed);
    break;
  case CELL_SLIDE_SOUTH:
    YampiDesert_SlideLeader(leader, 0, speed);
    break;
  case CELL_SLIDE_WEST:
    YampiDesert_SlideLeader(leader, -speed, 0);
    break;
  case CELL_SLIDE_EAST:
    YampiDesert_SlideLeader(leader, speed, 0);
    break;
  }
}

/// Notes whether the leader stands on the upper level; in Blitz's room, keeps
/// its tall stake drawn as the leader is.
void YampiDesert_TrackUpperLevel(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  FIXED x;
  FIXED y;
  FIXED z;

  y = leader->y.fixed;
  if (y > toFIXED(16))
    gMainRoutineWork.onUpperLevel = 1;
  else
    gMainRoutineWork.onUpperLevel = 0;
  if (gMainRoutineWork.next == SCENE_YAMPI_DESERT_BLITZ) {
    Actor_SetSpritePriority(STAKE(10),
                            ((struct FieldSprite *)leader->sprite)->priority);
    x = leader->x.fixed;
    if (x > toFIXED(472)) {
      z = leader->z.fixed;
      if (z <= toFIXED(488))
        return;
      if (leader->y.fixed > -toFIXED(16))
        gMainRoutineWork.onUpperLevel = 1;
      else
        gMainRoutineWork.onUpperLevel = 0;
    }
  }
}

/// Notes whether the leader has climbed above the lower level.
void YampiDesert_TrackUpperLevelFromBelow(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.leaderActorId);

  if (leader->y.fixed > -toFIXED(16))
    gMainRoutineWork.onUpperLevel = 1;
  else
    gMainRoutineWork.onUpperLevel = 0;
}

/// Notes, on the ramp's cells, how high the leader stands.
void YampiDesert_TrackUpperLevelOnRamp(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  struct MapWork *map = Ram_HeapSlots->mapGrid;
  s32 index = Actor_GetCellIndex(leader);
  struct MapCell *cell = map->layers[0].cells + index;
  struct MainRoutineWork *party;
  FIXED y;

  if (cell->collisionCode == CELL_RAMP) {
    y = leader->y.fixed;
    if (y > toFIXED(16)) {
      gMainRoutineWork.onUpperLevel = 1;
      return;
    }
  }
  party = &gMainRoutineWork;
  party->onUpperLevel = 0;
}

/// Notes whether the leader stands by the pit, where opening a menu raises a
/// trigger.
void YampiDesert_TrackLeaderInPitArea(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  u32 x = FIELD_CELL(leader->x.fixed);
  u32 z = FIELD_CELL(leader->z.fixed);

  if (x > 12 && z > 10 && x <= 15 && z <= 13)
    GameFlag_Set(FLAG_MENUS_RAISE_TRIGGER);
  else
    GameFlag_Clear(FLAG_MENUS_RAISE_TRIGGER);
}

/// While the King Scorpion roams buried, a rumble every five frames, unless the
/// leader stands still.
void YampiDesert_PlayScorpionRumble(void) {
  if (gFieldWork->psynergy == 0 && gFrameCount % 5 == 0) {
    struct MainRoutineWork *party = &gMainRoutineWork;

    if (party->movementMode != 5 || gInput.held == 0)
      Sound_Play(SE_SAND_MOVE);
  }
}

/// Clears the trigger the scorpion has raised.
void YampiDesert_ClearRaisedEvent(void) {
  gFieldWork->raisedTrigger = 0;
  gYampiDesert_RaisedEvent = 0;
}

/// Whether an actor has no move target left.
s32 YampiDesert_HasArrived(struct Actor *object) {
  s32 x = object->targetX;

  if (x == TARGET_UNSET && object->targetZ == x)
    return 1;
  return 0;
}

/// The buried King Scorpion's round in the Psynergy Stone's room.
void YampiDesert_ScorpionRoamsPsynergyStone(struct Actor *object) {
  u8 *state = &object->riseCounter;

  switch (*state) {
  case 0:
    FieldActor_SetMoveTarget((s32)object, 280, 0, 200);
    *state = 1;
    break;
  case 1:
    if (YampiDesert_HasArrived(object))
      *state = 2;
    break;
  case 2:
    FieldActor_SetMoveTarget((s32)object, 280, 0, 264);
    *state = 3;
    break;
  case 3:
    if (YampiDesert_HasArrived(object))
      *state = 4;
    break;
  case 4:
    FieldActor_SetMoveTarget((s32)object, 184, 0, 264);
    *state = 5;
    break;
  case 5:
    if (YampiDesert_HasArrived(object))
      *state = 6;
    break;
  case 6:
    FieldActor_SetMoveTarget((s32)object, 184, 0, 248);
    *state = 7;
    break;
  case 7:
    if (YampiDesert_HasArrived(object)) {
      if (GameFlag_Get(FLAG_SCENE_TEMP_03)) {
        object->update = NULL;
        gYampiDesert_RaisedEvent = RAISED_STIR_SCORPION;
      } else {
        *state = 8;
      }
    }
    break;
  case 8:
    FieldActor_SetMoveTarget((s32)object, 184, 0, 200);
    *state = 9;
    break;
  case 9:
    if (YampiDesert_HasArrived(object))
      *state = 0;
    break;
  }
}

/// Readies actors 16 and 17 and the room's callbacks; actor 17 becomes the
/// followed actor (gYampiDesert_SandRipple).
void YampiDesert_SetUpPsynergyStoneScorpion(void) {
  struct Actor *leader = Actor_GetById(ACTOR_PSYNERGY_STONE_ROOM_KING_SCORPION);
  struct Actor *object;
  struct Actor **followed;

  Scene_TaskAdd(YampiDesert_PlayScorpionRumble, TASK_PASS_VBLANK);
  followed = &gYampiDesert_SandRipple;
  object = Actor_GetById(ACTOR_PSYNERGY_STONE_ROOM_SAND_RIPPLE);
  object->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
  *followed = (struct Actor *)object;
  object->scaleX = FIXED_ONE * 13 / 10;
  object->scaleY = FIXED_ONE * 13 / 10;
  Actor_DisableById(ACTOR_PSYNERGY_STONE_ROOM_SAND_RIPPLE);
  (*followed)->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  Actor_SetSpritePriority(ACTOR_PSYNERGY_STONE_ROOM_SAND_RIPPLE, 3);
  Actor_SetSpeed(ACTOR_PSYNERGY_STONE_ROOM_KING_SCORPION, 1.6);
  Actor_SetSpriteColor((struct Actor *)leader, 15);
  Actor_SetSpritePriority(ACTOR_PSYNERGY_STONE_ROOM_KING_SCORPION, 3);
  Actor_DisableById(ACTOR_PSYNERGY_STONE_ROOM_KING_SCORPION);
  Actor_GetById(ACTOR_PSYNERGY_STONE_ROOM_KING_SCORPION)->motionFlags =
      ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  if (!GameFlag_Get(FLAG_KEEP_PARTY_POSITION)) {
    leader->riseCounter = 0;
    leader->update = YampiDesert_ScorpionRoamsPsynergyStone;
  }
  YampiDesert_ClearRaisedEvent();
  Task_Add(YampiDesert_RippleFollowsScorpion, TASK_PASS_VBLANK);
}

/// Sends the object toward a point and moves to the next state.
#define WALK(state_, x, z)                                                     \
  case state_:                                                                 \
    FieldActor_SetMoveTargetFX((s32)object, x, 0, z);                          \
    *state = state_ + 1;                                                       \
    break;

/// Waits for the object to arrive before moving to the given state.
#define ARRIVE(state_, next)                                                   \
  case state_:                                                                 \
    if (YampiDesert_HasArrived(object))                                        \
      *state = next;                                                           \
    break;

/// The buried King Scorpion's round in the King Scorpion's room.
void YampiDesert_ScorpionRoamsKingScorpion(struct Actor *object) {
  u8 *state = &object->riseCounter;

  switch (*state) {
    WALK(0, toFIXED(312), toFIXED(664))
    ARRIVE(1, 2)
    WALK(2, toFIXED(312), toFIXED(728))
    ARRIVE(3, 4)
    WALK(4, toFIXED(246), toFIXED(728))
  case 5:
    if (YampiDesert_HasArrived(object)) {
      if (GameFlag_Get(FLAG_SCENE_TEMP_00)) {
        object->update = NULL;
        gYampiDesert_RaisedEvent = RAISED_STIR_SCORPION_AGAIN;
      } else {
        *state = 6;
      }
    }
    break;
    WALK(6, toFIXED(184), toFIXED(728))
    ARRIVE(7, 8)
    WALK(8, toFIXED(184), toFIXED(712))
    ARRIVE(9, 10)
    WALK(10, toFIXED(280), toFIXED(712))
    ARRIVE(11, 12)
    WALK(12, toFIXED(280), toFIXED(760))
    ARRIVE(13, 14)
    WALK(14, toFIXED(232), toFIXED(760))
    ARRIVE(15, 16)
    WALK(16, toFIXED(232), toFIXED(744))
  case 17:
    if (YampiDesert_HasArrived(object)) {
      if (!GameFlag_Get(FLAG_SCENE_TEMP_00))
        *state = 18;
      else
        *state = 32;
    }
    break;
    WALK(18, toFIXED(232), toFIXED(664))
    ARRIVE(19, 0)
  case 32:
    YampiDesert_ClearRaisedEvent();
    FieldActor_SetMoveTarget((s32)object, 296, 0, 744);
    *state = 33;
    break;
    ARRIVE(33, 34)
    WALK(34, toFIXED(296), toFIXED(760))
    ARRIVE(35, 36)
    WALK(36, toFIXED(232), toFIXED(760))
    ARRIVE(37, 38)
    WALK(38, toFIXED(232), toFIXED(744))
    ARRIVE(39, 32)
  }
}

/// YampiDesert_SetUpPsynergyStoneScorpion's counterpart for actors 21 and 22:
/// actor 22 becomes the followed actor, and until flag 0x109 actor 21 is placed
/// and given the update YampiDesert_ScorpionRoamsKingScorpion.
void YampiDesert_SetUpKingScorpionScorpion(void) {
  struct Actor *leader = Actor_GetById(ACTOR_KING_SCORPION_ROOM_KING_SCORPION);
  struct Actor *object;
  struct Actor **followed;

  Scene_TaskAdd(YampiDesert_PlayScorpionRumble, TASK_PASS_VBLANK);
  followed = &gYampiDesert_SandRipple;
  object = Actor_GetById(ACTOR_KING_SCORPION_ROOM_SAND_RIPPLE);
  object->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
  *followed = (struct Actor *)object;
  object->scaleX = FIXED_ONE * 13 / 10;
  object->scaleY = FIXED_ONE * 13 / 10;
  Actor_DisableById(ACTOR_KING_SCORPION_ROOM_SAND_RIPPLE);
  (*followed)->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  Actor_SetSpritePriority(ACTOR_KING_SCORPION_ROOM_SAND_RIPPLE, 3);
  Actor_SetSpeed(ACTOR_KING_SCORPION_ROOM_KING_SCORPION, 1.6);
  Actor_SetSpriteColor((struct Actor *)leader, 15);
  Actor_SetSpritePriority(ACTOR_KING_SCORPION_ROOM_KING_SCORPION, 3);
  Actor_DisableById(ACTOR_KING_SCORPION_ROOM_KING_SCORPION);
  Actor_GetById(ACTOR_KING_SCORPION_ROOM_KING_SCORPION)->motionFlags =
      ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  if (!GameFlag_Get(FLAG_KEEP_PARTY_POSITION)) {
    YampiDesert_SetPositionAndFacing(ACTOR_KING_SCORPION_ROOM_KING_SCORPION,
                                     toFIXED(232), toFIXED(664), 0);
    leader->riseCounter = 0;
    leader->update = YampiDesert_ScorpionRoamsKingScorpion;
  }
  YampiDesert_ClearRaisedEvent();
  Task_Add(YampiDesert_RippleFollowsScorpion, TASK_PASS_VBLANK);
}

// Sends the object toward a point and moves to the next state.

/// As WALK, after YampiDesert_ClearRaisedEvent.
#define WALK_ON(state_, x, z)                                                  \
  case state_:                                                                 \
    YampiDesert_ClearRaisedEvent();                                            \
    FieldActor_SetMoveTargetFX((s32)object, x, 0, z);                          \
    *state = state_ + 1;                                                       \
    break;

// Waits for the object to arrive before moving to the given state.

/// Waits for the object to arrive, then turns off by a flag.
#define ARRIVE_OR(state_, flag, turn, next)                                    \
  case state_:                                                                 \
    if (YampiDesert_HasArrived(object)) {                                      \
      if (GameFlag_Get(flag))                                                  \
        *state = turn;                                                         \
      else                                                                     \
        *state = next;                                                         \
    }                                                                          \
    break;

/// The buried King Scorpion's round in the King Scorpion's room once stirred
/// again, passing the trap.
void YampiDesert_ScorpionRoamsToTrap(struct Actor *object) {
  u8 *state = &object->riseCounter;

  switch (*state) {
    WALK(0, toFIXED(312), toFIXED(264))
    ARRIVE(1, 2)
    WALK(2, toFIXED(312), toFIXED(344))
    ARRIVE(3, 4)
    WALK(4, toFIXED(296), toFIXED(344))
    ARRIVE_OR(5, 0x202, 32, 6)
    WALK(6, toFIXED(168), toFIXED(344))
    ARRIVE(7, 8)
    WALK(8, toFIXED(168), toFIXED(296))
    ARRIVE(9, 10)
    WALK(10, toFIXED(184), toFIXED(296))
    ARRIVE_OR(11, 0x201, 48, 12)
    WALK(12, toFIXED(280), toFIXED(296))
    ARRIVE(13, 14)
    WALK(14, toFIXED(280), toFIXED(328))
  case 15:
    if (YampiDesert_HasArrived(object)) {
      if (GameFlag_Get(FLAG_SCENE_TEMP_02)) {
        object->update = NULL;
        gYampiDesert_RaisedEvent = RAISED_CORNER_SCORPION;
      } else {
        *state = 16;
      }
    }
    break;
    WALK(16, toFIXED(280), toFIXED(360))
    ARRIVE(17, 18)
    WALK(18, toFIXED(200), toFIXED(360))
    ARRIVE(19, 20)
    WALK(20, toFIXED(200), toFIXED(312))
    ARRIVE_OR(21, 0x201, 34, 22)
    WALK(22, toFIXED(200), toFIXED(264))
    ARRIVE(23, 0)
    WALK_ON(32, toFIXED(296), toFIXED(312))
    ARRIVE(33, 34)
    WALK_ON(34, toFIXED(328), toFIXED(312))
    ARRIVE(35, 36)
    WALK(36, toFIXED(328), toFIXED(376))
    ARRIVE(37, 38)
    WALK(38, toFIXED(296), toFIXED(376))
    ARRIVE(39, 32)
    WALK_ON(48, toFIXED(184), toFIXED(344))
    ARRIVE(49, 50)
    WALK(50, toFIXED(168), toFIXED(344))
    ARRIVE(51, 52)
    WALK(52, toFIXED(168), toFIXED(296))
    ARRIVE(53, 54)
    WALK(54, toFIXED(184), toFIXED(296))
    ARRIVE(55, 48)
  }
}

/// YampiDesert_SetUpPsynergyStoneScorpion's counterpart for actors 21 and 22 in
/// the King Scorpion's room: once the scorpion is cornered
/// (FLAG_YAMPI_DESERT_SCORPION_CORNERED) actor 21 is placed facing south;
/// otherwise, unless the party is back from a battle
/// (FLAG_KEEP_PARTY_POSITION), it is placed and given the update
/// YampiDesert_ScorpionRoamsToTrap.
void YampiDesert_SetUpTrappedScorpion(void) {
  struct Actor *leader = Actor_GetById(ACTOR_KING_SCORPION_ROOM_KING_SCORPION);
  struct Actor *object;
  struct Actor **followed;
  s32 angle;

  followed = &gYampiDesert_SandRipple;
  object = Actor_GetById(ACTOR_KING_SCORPION_ROOM_SAND_RIPPLE);
  object->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
  object->scaleX = FIXED_ONE * 13 / 10;
  object->scaleY = FIXED_ONE * 13 / 10;
  *followed = (struct Actor *)object;
  Actor_DisableById(ACTOR_KING_SCORPION_ROOM_SAND_RIPPLE);
  Actor_SetSpritePriority(ACTOR_KING_SCORPION_ROOM_SAND_RIPPLE, 3);
  (*followed)->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  Actor_SetSpeed(ACTOR_KING_SCORPION_ROOM_KING_SCORPION, 1.6);
  Actor_SetSpriteColor((struct Actor *)leader, 15);
  Actor_SetSpritePriority(ACTOR_KING_SCORPION_ROOM_KING_SCORPION, 3);
  Actor_DisableById(ACTOR_KING_SCORPION_ROOM_KING_SCORPION);
  Actor_GetById(ACTOR_KING_SCORPION_ROOM_KING_SCORPION)->motionFlags =
      ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  if (GameFlag_Get(FLAG_YAMPI_DESERT_SCORPION_CORNERED)) {
    angle = FACING_SOUTH;
    YampiDesert_SetPositionAndFacing(ACTOR_KING_SCORPION_ROOM_KING_SCORPION,
                                     toFIXED(232), toFIXED(168), angle);
  } else {
    Scene_TaskAdd(YampiDesert_PlayScorpionRumble, TASK_PASS_VBLANK);
    if (!GameFlag_Get(FLAG_KEEP_PARTY_POSITION)) {
      YampiDesert_SetPositionAndFacing(ACTOR_KING_SCORPION_ROOM_KING_SCORPION,
                                       toFIXED(200), toFIXED(264), 0);
      leader->riseCounter = 0;
      leader->update = YampiDesert_ScorpionRoamsToTrap;
    }
  }
  YampiDesert_ClearRaisedEvent();
  Task_Add(YampiDesert_RippleFollowsScorpion, TASK_PASS_VBLANK);
}

/// While the King Scorpion's Scoop runs, opens the hole it digs.
void YampiDesert_OpenScoopHole(void) {
  if (*(s16 *)((u8 *)Ram_HeapSlots->fieldEffectWork + 0x7c4) != 0)
    MapGrid_CopyCellPictureIndicesRect(73, 76, 78, 11, 1, 1);
}

/// The trapped King Scorpion digs its way out with Scoop.
void YampiDesert_ScorpionUsesScoop(void) {
  FieldPsynergy_LoadResources(ABILITY_SCOOP, 1);
  Scene_TaskAdd(YampiDesert_OpenScoopHole, TASK_PASS_VBLANK);
  FieldPsynergy_SetupActorPair(21, -1);
  FieldPsynergy_StartCast();
  FieldPsynergy_RunHandler(1);
  BattleEffect_StopMainActor();
  Task_Remove(YampiDesert_OpenScoopHole);
  FieldPsynergy_End();
}

#include "shared/actor_hide.h"

/// Runs YampiDesert_HideSlideActors unless FLAG_YAMPI_DESERT_SLIDE_ACTORS_SHOWN
/// is set, setting it when the party keeps its place.
void Scene_RunUnlessFlag(void) {
  if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION)) {
    if (GameFlag_Get(FLAG_YAMPI_DESERT_SLIDE_ACTORS_SHOWN) == 0)
      YampiDesert_HideSlideActors();
  } else {
    GameFlag_Set(FLAG_YAMPI_DESERT_SLIDE_ACTORS_SHOWN);
  }
}

/// Hides the Trainer's Whip chest's room's sand slide actors.
void YampiDesert_SetUpTrainersWhipChestSlideActors(void) {
  Actor_Hide(ACTOR_WHIP_CHEST_ROOM_FIRST_SLIDE);
  Actor_Hide(ACTOR_WHIP_CHEST_ROOM_SECOND_SLIDE);
  Scene_RunUnlessFlag();
}

/// Runs Actor_Hide for each id from 9 to `9 + 6`, then Scene_RunUnlessFlag.
void YampiDesert_SetUpBlowMaceChestSlideActors(void) {
  u32 i;

  for (i = 0; i <= 6; i++)
    Actor_Hide(i + 9);
  Scene_RunUnlessFlag();
}

/// The leader climbs out of a pit at (x, z).
void YampiDesert_ClimbOut(struct Actor *object, s32 x, s32 z, s32 mode) {
  struct MainRoutineWork *party;
  s32 zero;
  u8 *flags;

  Event_Start();
  Event_ResolvePendingActions(0);
  flags = &object->motionFlags;
  zero = 0;
  party = &gMainRoutineWork;
  *flags = zero;
  Actor_SetSpeed(party->leaderActorId, 1);
  Actor_WalkToAndWaitById(party->leaderActorId, x, z);
  Actor_FaceDirectionAndRefreshById(party->leaderActorId, FACING_NORTH);
  Actor_SetSpriteFlags(Actor_GetById(party->leaderActorId), 0);
  Actor_SetAnimationById(party->leaderActorId, 13);
  Actor_SetMoveTarget(object, object->x.fixed, object->y.fixed - toFIXED(10),
                      object->z.fixed);
  Actor_WaitForMove(object);
  Actor_SetAnimationById(party->leaderActorId, 10);
  Sound_Play(SE_MAP_EXIT);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Import_EventSetExitRequest(mode);
}

/// Climbing down the ladder Scoop has dug up.
void YampiDesert_ClimbRevealedLadder(void) {
  struct Actor *object;
  s32 x;
  s32 z;

  if (GameFlag_Get(FLAG_YAMPI_DESERT_LADDER_DUG_UP)) {
    object = Actor_GetById(gMainRoutineWork.leaderActorId);
    x = FIELD_CELL(object->x.fixed);
    z = FIELD_CELL(object->z.fixed);
    if (x == 8) {
      if (z == 31 && !(gInput.held & KEY_DOWN))
        return;
      if (z == 33 && !(gInput.held & KEY_UP))
        return;
    } else {
      if (z != 32)
        return;
      if (x == 7 && !(gInput.held & KEY_RIGHT))
        return;
      if (x == 9 && !(gInput.held & KEY_LEFT))
        return;
    }
    YampiDesert_ClimbOut(object, 136, 516, 5);
  }
}

/// Climbs out of the cave under the oasis.
void YampiDesert_ClimbOutOfOasisCave(void) {
  YampiDesert_ClimbOut(Actor_GetById(gMainRoutineWork.leaderActorId), 408, 432,
                       2);
}

void YampiDesert_BurstSand(s32 x, s32 y, s32 z);

/// Dropping into the scorpion pit, the leader lands before the King Scorpion,
/// and its battle starts.
void YampiDesert_LandAndFightKingScorpion(void) {
  struct MainRoutineWork *party = &gMainRoutineWork;
  struct Actor *leader = Actor_GetById(party->leaderActorId);
  struct Actor *actor8 = Actor_GetById(ACTOR_KING_SCORPION);

  Event_Start();
  Event_ResolvePendingActions(0);
  Sound_Play(SOUND_FADE_OUT_BGM);
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  FieldCamera_MoveTo(232, CAMERA_KEEP, 184, 0);
  MapGrid_CenterOnFollowedPoint();
  wait(1);
  Actor_SetSpriteFlags(Actor_GetById(party->leaderActorId), 0);
  leader->motionFlags = 0;
  leader->y.fixed = toFIXED(130);
  leader->gravity = FIXED_ONE / 2;
  leader->bounce = 0;
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_KING_SCORPION), 0);
  {
    s32 angle = FACING_SOUTH;

    YampiDesert_SetPositionAndFacing(ACTOR_KING_SCORPION, toFIXED(232),
                                     toFIXED(148), angle);
  }
  actor8->motionFlags = 0;
  actor8->y.fixed = toFIXED(130);
  actor8->gravity = FIXED_ONE / 2;
  actor8->bounce = 0;
  wait(1);
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 3);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Sound_Play(SE_DUST_PUFF);
  leader->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  Actor_SetSpriteFlags(Actor_GetById(party->leaderActorId), 1);
  FieldEvent_Wait(24);
  YampiDesert_BurstSand(leader->x.fixed, leader->y.fixed, leader->z.fixed);
  Sound_Play(SE_THUMP);
  FieldEffect_SetAttached(party->leaderActorId,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ATTACH);
  Actor_SetAnimationAndWait(party->leaderActorId, 49);
  Sound_Play(SE_DUST_PUFF);
  actor8->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_KING_SCORPION), 1);
  FieldEvent_Wait(24);
  YampiDesert_BurstSand(actor8->x.fixed, actor8->y.fixed, actor8->z.fixed);
  Sound_Play(SE_THUMP);
  Actor_FaceDirectionById(party->leaderActorId, TRIG_TURN * 3 / 4, 0);
  FieldEvent_Wait(40);
  Sound_Play(SE_BEAST_CRY);
  Actor_SetAnimationById(ACTOR_KING_SCORPION, 3);
  FieldEvent_Wait(20);
  actor8->velocityY = FIXED_ONE * 8;
  actor8->gravity = toFIXED(9) / 10;
  Actor_SetSpeed(ACTOR_KING_SCORPION, 1.6);
  Actor_MoveToAndWaitById(ACTOR_KING_SCORPION, 232, 184);
  party->battleType = BATTLE_TYPE_NO_ESCAPE;
  Event_SetAfterBattleDestination(SCENE_YAMPI_DESERT_SCORPION_PIT, 2);
  Field_RequestBattleFromEncounterTable(10, 0);
}

/// Clears the scorpion's sand ripple and runs the entered room's setup; the
/// sand pass has none.
s32 Scene_Initialize(void) {
  s16 scene;

  YampiDesert_ClearRaisedEvent();
  gYampiDesert_SandRipple = 0;
  scene = gMainRoutineWork.next;
  if (scene == SCENE_YAMPI_DESERT_GUARDIAN_RING_CHEST)
    YampiDesert_SetUpGuardianRingChest();
  else if (scene == SCENE_YAMPI_DESERT_OASIS)
    YampiDesert_SetUpOasis();
  else if (scene == SCENE_YAMPI_DESERT_BLITZ)
    YampiDesert_SetUpBlitz();
  else if (scene == SCENE_YAMPI_DESERT_PSYNERGY_STONE)
    YampiDesert_SetUpPsynergyStone();
  else if (scene == SCENE_YAMPI_DESERT_KING_SCORPION)
    YampiDesert_SetUpKingScorpion();
  else if (scene == SCENE_YAMPI_DESERT_TREASURE_PIT)
    YampiDesert_SetUpTreasurePit();
  else if (scene == SCENE_YAMPI_DESERT_TRAINERS_WHIP_CHEST)
    YampiDesert_SetUpTrainersWhipChest();
  else if (scene == SCENE_YAMPI_DESERT_MEDAL_SPOT)
    YampiDesert_SetUpMedalSpot();
  else if (scene == SCENE_YAMPI_DESERT_SCORPION_PIT)
    YampiDesert_SetUpScorpionPit();
  else if (scene == SCENE_YAMPI_DESERT_OASIS_CAVE)
    YampiDesert_SetUpOasisCave();
  else if (scene == SCENE_YAMPI_DESERT_BLOW_MACE_CHEST)
    YampiDesert_SetUpBlowMaceChest();
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

/// Sets up the Guardian Ring chest's room: its stakes, and the way in it was
/// entered by.
void YampiDesert_SetUpGuardianRingChest(void) {
  YampiDesert_PlaceStakes(gYampiDesertGuardianRingChestStakes);
  FieldEffect_StartBlankOverlay(0);
  if (gMainRoutineWork.entrance == 1) {
    GameFlag_Set(FLAG_YAMPI_DESERT_ENTERED_BY_GUARDIAN_RING_CHEST);
    GameFlag_Clear(FLAG_YAMPI_DESERT_ENTERED_BY_TRAINERS_WHIP_CHEST);
    GameFlag_Clear(FLAG_YAMPI_DESERT_ENTERED_BY_MEDAL_SPOT);
  }
}

/// The travelers are gone once the Madrans have reached Alhafra
/// (FLAG_ALHAFRA_MADRANS_ARRIVED); once met (FLAG_YAMPI_DESERT_TRAVELERS_MET)
/// they stand about the oasis.
void YampiDesert_SetUpOasis(void) {
  s32 angleA;
  s32 angleB;
  s32 angle;

  if (GameFlag_Get(FLAG_ALHAFRA_MADRANS_ARRIVED)) {
    Actor_SetPosition(ACTOR_TRAVELER_FOLLOWING_THE_PIRATES, 0, 0);
    Actor_SetPosition(ACTOR_TRAVELER_SEEKING_EVIDENCE, 0, 0);
    Actor_SetPosition(ACTOR_DIZZY_TRAVELER, 0, 0);
    Actor_SetPosition(ACTOR_PARCHED_TRAVELER, 0, 0);
    Actor_SetPosition(ACTOR_TRAVELER_WHO_WOULD_TURN_BACK, 0, 0);
    Actor_SetPosition(ACTOR_ELDER, 0, 0);
    Actor_SetPosition(ACTOR_MAYOR, 0, 0);
  } else if (GameFlag_Get(FLAG_YAMPI_DESERT_TRAVELERS_MET)) {
    angleA = FACING_NORTH_NORTHWEST;
    YampiDesert_SetPositionAndFacing(ACTOR_TRAVELER_FOLLOWING_THE_PIRATES,
                                     toFIXED(520), toFIXED(242), angleA);
    angle = FACING_WEST;
    YampiDesert_SetPositionAndFacing(ACTOR_TRAVELER_SEEKING_EVIDENCE,
                                     toFIXED(533), toFIXED(271), angle);
    angle = FACING_SOUTH_SOUTHEAST;
    YampiDesert_SetPositionAndFacing(ACTOR_DIZZY_TRAVELER, toFIXED(435),
                                     toFIXED(232), angle);
    angle = FACING_SOUTH_SOUTHWEST;
    YampiDesert_SetPositionAndFacing(ACTOR_PARCHED_TRAVELER, toFIXED(467),
                                     toFIXED(248), angle);
    angleB = FACING_NORTH_NORTHEAST;
    YampiDesert_SetPositionAndFacing(ACTOR_TRAVELER_WHO_WOULD_TURN_BACK,
                                     toFIXED(462), toFIXED(340), angleB);
    YampiDesert_SetPositionAndFacing(ACTOR_ELDER, toFIXED(482), toFIXED(340),
                                     angleB);
    YampiDesert_SetPositionAndFacing(ACTOR_MAYOR, toFIXED(489), toFIXED(320),
                                     angleA);
  }
  if (GameFlag_Get(FLAG_YAMPI_DESERT_LADDER_DUG_UP))
    YampiDesert_OpenLadderPit();
  Task_Add(YampiDesert_TrackUpperLevel, TASK_PASS_VBLANK);
}

/// Keeps two of Blitz's room's stakes drawn as the leader is.
void YampiDesert_MatchStakePriority(void) {
  s32 variant =
      ((struct FieldSprite *)Actor_GetById(gMainRoutineWork.leaderActorId)
           ->sprite)
          ->priority;

  Actor_SetSpritePriority(STAKE(5), variant);
  Actor_SetSpritePriority(STAKE(6), variant);
}

/// Sets up Blitz's room: its stakes and the tasks that layer them.
void YampiDesert_SetUpBlitz(void) {
  YampiDesert_PlaceStakes(gYampiDesertBlitzStakes);
  if (GameFlag_Get(FLAG_YAMPI_DESERT_ON_STAKE_TOPS)) {
    YampiDesert_StepOntoStakeTops();
    wait(10);
  }
  Scene_TaskAdd(YampiDesert_TrackUpperLevel, TASK_PASS_VBLANK);
  Scene_TaskAdd(YampiDesert_MatchStakePriority, TASK_PASS_VBLANK);
}

/// Sets up a stake: behind the leader, disabled, in its standing animation.
void YampiDesert_ReadyStake(s32 id) {
  Actor_GetById(id)->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
  Actor_SetSpritePriority(id, 3);
  Actor_DisableById(id);
  Actor_SetAnimationById(id, 2);
}

/// Sets up the Psynergy Stone's room: its stakes, and the King Scorpion until
/// it has been stirred.
void YampiDesert_SetUpPsynergyStone(void) {
  FieldEffect_StartBlankOverlay(0);
  if (gMainRoutineWork.entrance == 3 || gMainRoutineWork.entrance == 4) {
    if (Scene_FlagTest(FLAG_YAMPI_DESERT_SCORPION_STIRRED))
      Actor_SetPosition(ACTOR_PSYNERGY_STONE_ROOM_KING_SCORPION, 0, 0);
    else
      YampiDesert_SetUpPsynergyStoneScorpion();
  }
  YampiDesert_PlaceStakes(gYampiDesertPsynergyStoneStakes);
  if (Scene_FlagTest(FLAG_YAMPI_DESERT_ON_STAKE_TOPS)) {
    YampiDesert_StepOntoStakeTops();
    wait(10);
  }
  YampiDesert_ReadyStake(12);
  YampiDesert_ReadyStake(13);
  YampiDesert_ReadyStake(14);
  YampiDesert_ReadyStake(15);
  if (gMainRoutineWork.entrance == 1 || gMainRoutineWork.entrance == 2)
    Task_Add(YampiDesert_TrackUpperLevelFromBelow, TASK_PASS_VBLANK);
}

/// The setup of the King Scorpion's room: from entrances other than 1 and 2 it
/// places actors 11 to 20 and copies the map cells flags 0x88c and 0x8ed
/// select.
void YampiDesert_SetUpKingScorpion(void) {
  FieldEffect_StartBlankOverlay(0);
  YampiDesert_PlaceStakes(gYampiDesertKingScorpionStakes);
  YampiDesert_ReadyStake(STAKE(3));
  YampiDesert_ReadyStake(STAKE(4));
  YampiDesert_ReadyStake(STAKE(5));
  YampiDesert_ReadyStake(STAKE(6));
  YampiDesert_ReadyStake(STAKE(7));
  YampiDesert_ReadyStake(STAKE(8));
  YampiDesert_ReadyStake(STAKE(9));
  YampiDesert_ReadyStake(STAKE(10));
  YampiDesert_ReadyStake(STAKE(11));
  YampiDesert_ReadyStake(STAKE(12));
  if (gMainRoutineWork.entrance == 1 || gMainRoutineWork.entrance == 2) {
    if (GameFlag_Get(FLAG_YAMPI_DESERT_SCORPION_STIRRED) &&
        !GameFlag_Get(FLAG_YAMPI_DESERT_SCORPION_STIRRED_AGAIN))
      YampiDesert_SetUpKingScorpionScorpion();
  } else {
    Actor_SetPosition(STAKE(3), 200, 248);
    Actor_SetPosition(STAKE(4), 168, 280);
    Actor_SetPosition(STAKE(5), 152, 344);
    Actor_SetPosition(STAKE(6), 184, 360);
    Actor_SetPosition(STAKE(7), 328, 264);
    Actor_SetPosition(STAKE(8), 296, 296);
    Actor_SetPosition(STAKE(9), 344, 312);
    Actor_SetPosition(STAKE(10), 312, 360);
    Actor_SetPosition(STAKE(11), 280, 376);
    Actor_SetPosition(STAKE(12), 328, 392);
    if (GameFlag_Get(FLAG_KING_SCORPION_BEATEN)) {
      if (GameFlag_Get(FLAG_YAMPI_DESERT_ROCK_PIT_DUG_UP)) {
        MapGrid_CopyCellPictureIndicesRect(68, 77, 13, 9, 3, 3);
        MapGrid_CopyCellAttributes(68, 77, 3, 3, 13, 9);
      } else {
        MapGrid_CopyCellPictureIndicesRect(74, 71, 77, 9, 3, 3);
        MapGrid_CopyCellAttributes(74, 71, 3, 3, 13, 9);
      }
      MapGrid_CopyCellAttributes(63, 77, 3, 3, 13, 73);
    } else if (GameFlag_Get(FLAG_YAMPI_DESERT_SCORPION_STIRRED_AGAIN)) {
      YampiDesert_SetUpTrappedScorpion();
    }
  }
}

/// A halfword of gMainRoutineWork that has no field name yet.
#define MAIN_ROUTINE_WORK_U16(offset)                                          \
  (*(u16 *)((u8 *)&gMainRoutineWork + (offset)))

/// Sets up the treasure pit's room: its treasure stays hidden until it is dug
/// up.
void YampiDesert_SetUpTreasurePit(void) {
  FieldEffect_StartBlankOverlay(0);
  if (GameFlag_Get(FLAG_YAMPI_DESERT_TREASURE_PIT_DUG_UP) == 0)
    Actor_SetWideSprite(ACTOR_HIDDEN_TREASURE, 0);
}

/// The setup of the Trainer's Whip chest's room. Entered at entrance 4 (not in
/// the Japanese edition), it sets where Retreat returns the party to the room
/// whose flag is set.
void YampiDesert_SetUpTrainersWhipChest(void) {
  struct Actor *object = Actor_GetById(ACTOR_HIDDEN_TREASURE);

  Scene_QueueMusic(SE_KIRIN);
  Rope_Initialize();
  YampiDesert_PlaceRope(0, FLAG_YAMPI_DESERT_ROPE_TIED, ACTOR_ROPE_POLE,
                        ACTOR_ROPE_OTHER_POLE);
  if (object != NULL)
    Actor_SetPosition(ACTOR_HIDDEN_TREASURE, 0, 0);
  object = Actor_GetById(ACTOR_SIGNPOST);
  object->scaleX = FIXED_ONE * 7 / 10;
  object->scaleY = FIXED_ONE * 7 / 10;
  object->radius = 8;
  YampiDesert_SetUpTrainersWhipChestSlideActors();
  Task_Add(YampiDesert_UpdateSandSlide, TASK_PASS_VBLANK);
  if (gMainRoutineWork.entrance == 2) {
    GameFlag_Clear(FLAG_YAMPI_DESERT_ENTERED_BY_GUARDIAN_RING_CHEST);
    GameFlag_Set(FLAG_YAMPI_DESERT_ENTERED_BY_TRAINERS_WHIP_CHEST);
    GameFlag_Clear(FLAG_YAMPI_DESERT_ENTERED_BY_MEDAL_SPOT);
  }
#if TLA_PATCH_1
  else if (gMainRoutineWork.entrance == 4) {
    if (GameFlag_Get(FLAG_YAMPI_DESERT_ENTERED_BY_GUARDIAN_RING_CHEST)) {
      gMainRoutineWork.retreatScene = SCENE_YAMPI_DESERT_GUARDIAN_RING_CHEST;
      gMainRoutineWork.retreatEntrance = 1;
    } else if (GameFlag_Get(FLAG_YAMPI_DESERT_ENTERED_BY_TRAINERS_WHIP_CHEST)) {
      gMainRoutineWork.retreatScene = SCENE_YAMPI_DESERT_TRAINERS_WHIP_CHEST;
      gMainRoutineWork.retreatEntrance = 2;
    } else if (GameFlag_Get(FLAG_YAMPI_DESERT_ENTERED_BY_MEDAL_SPOT)) {
      gMainRoutineWork.retreatScene = SCENE_YAMPI_DESERT_MEDAL_SPOT;
      gMainRoutineWork.retreatEntrance = 2;
    }
  }
#endif
}

/// Sets up the medal spot's room, and the way in it was entered by.
void YampiDesert_SetUpMedalSpot(void) {
  if (Actor_GetById(ACTOR_HIDDEN_TREASURE) != 0)
    Actor_SetPosition(ACTOR_HIDDEN_TREASURE, 0, 0);
  if (gMainRoutineWork.entrance == 2) {
    GameFlag_Clear(FLAG_YAMPI_DESERT_ENTERED_BY_GUARDIAN_RING_CHEST);
    GameFlag_Clear(FLAG_YAMPI_DESERT_ENTERED_BY_TRAINERS_WHIP_CHEST);
    GameFlag_Set(FLAG_YAMPI_DESERT_ENTERED_BY_MEDAL_SPOT);
  }
}

struct RideEntry *YampiDesert_AddRisingSand(s32 slot, s32 id, s32 timer,
                                            s32 low, s32 high);

/// The setup of the scorpion pit.
void YampiDesert_SetUpScorpionPit(void) {
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
  GameFlag_Set(FLAG_RETREAT_BLOCKED);
  if (gMainRoutineWork.entrance == 1 &&
      !GameFlag_Get(FLAG_KING_SCORPION_BEATEN)) {
    YampiDesert_LandAndFightKingScorpion();
  } else {
    if (gMainRoutineWork.entrance == 2 &&
        !GameFlag_Get(FLAG_KING_SCORPION_BEATEN))
      YampiDesert_DropScoopGem();
  }
  if (GameFlag_Get(FLAG_YAMPI_DESERT_ROCK_PIT_DUG_UP)) {
    MapGrid_CopyCellPictureIndicesRect(35, 39, 13, 11, 3, 3);
    Actor_SetPosition(ACTOR_ROCK_PIT_MARKER, 232, 200);
  }
  YampiDesert_StartRisingSand(0, 240);
  YampiDesert_AddRisingSand(0, 9, 0, 0, 64);
  wait(10);
}

void YampiDesert_SetUpOasisCave(void) {
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
}

/// Sets up the Blow Mace chest's room: its stake and its sand slide.
void YampiDesert_SetUpBlowMaceChest(void) {
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
  Scene_QueueMusic(SE_KIRIN);
  YampiDesert_PlaceStakes(gYampiDesertBlowMaceChestStakes);
  YampiDesert_SetUpBlowMaceChestSlideActors();
  Scene_TaskAdd(YampiDesert_UpdateSandSlide, TASK_PASS_VBLANK);
}

/// Places each stake of a room's list, sunk if Pound has sunk it.
void YampiDesert_PlaceStakes(const void *script) {
  const u16 *entry;

  if (*(const u16 *)script == STAKE_LIST_END)
    return;
  entry = script;
loop:
  {
    s32 id = entry[0];
    struct Actor *object = Actor_GetById(id);
    s32 kind = (s16)entry[1];
    s32 flag = (s16)entry[2];

    object->collisionFlags &= ~ACTOR_SOLID;
    Actor_SetSpriteFlags(object, 0);
    Actor_SetCellOpen(id, (u16)kind, (u16)flag, object->layer);
    entry += 3;
  }
  if (entry[0] != STAKE_LIST_END)
    goto loop;
}
#undef WALK
#undef ARRIVE
#undef WALK_ON
#undef ARRIVE_OR
#undef MAIN_ROUTINE_WORK_U16

// Finds the current target's table row and shrinks it.
#include "shared/pound_sink_target.h"

#include "shared/actor_set_cell_open.h"

/// One rising platform and what rides it.
struct RideEntry {
  s16 id;
  s16 timer;
  s16 height;
  s16 period;
  s16 riseAt;
  s16 fallAt;
  s16 max;
  s16 min;
  s16 unk10;
  s16 step;
  s16 unk14;
  s16 phase;
  struct Actor *rider;
};

struct RideWork {
  s16 entry;
  s16 value;
  struct RideEntry entries[16];
  void *block;
};

/// The rising platforms callback: for each of the 16 entries it raises or
/// lowers the platform on its timer, writes its height to the collision and
/// cell maps, and carries its rider with a bob.
void YampiDesert_UpdateRisingSand(void) {
  struct RideWork *work = (struct RideWork *)(gInventorySnapshot + 0x188);
  struct MapWork *map = Ram_HeapSlots->mapGrid;
  struct RideEntry *entry = work->entries;
  s32 value = work->value;
  u8 *terrain = map->layers[0].terrainIds;
  struct MapCell *cells = map->layers[0].cells;
  s32 i = 0;
  s32 *collision = (s32 *)Ram_MapCollision + value;

  for (; i <= 15; collision++, entry++, i++) {
    struct Actor *object;
    struct Actor *rider;
    struct MapCell *cell;
    s32 index;

    if (entry->id == 0)
      continue;
    object = Actor_GetById(entry->id);
    if (object->x.fixed == 0 && object->z.fixed == 0)
      continue;
    if (entry->timer == entry->riseAt) {
      Sound_Play(SE_CURSE_FALL);
      entry->step = 4;
    }
    if (entry->timer == entry->fallAt) {
      Sound_Play(SOUND_FADE_OUT_EXIT_SE);
      entry->step = -8;
    }
    if (entry->step != 0) {
      entry->height += entry->step;
      if (entry->height >= entry->max) {
        entry->height = entry->max;
        entry->step = 0;
        entry->phase = 0;
      }
      if (entry->height <= entry->min) {
        entry->height = entry->min;
        entry->step = 0;
        entry->phase = 0;
      }
    }
    if (!GameFlag_Get(FLAG_FIELD_HANDLING_REQUESTS)) {
      if (++entry->timer >= entry->period)
        entry->timer = 0;
    }
    index = (FIELD_CELL(object->x.fixed)) + (FIELD_CELL(object->z.fixed)) * 128;
    terrain[index] = (u8)work->value + i;
    *collision = ((object->y.part[0].integer + entry->height) / 8) << 8;
    cell = &cells[index];
    if (entry->step != 0)
      cell->attributeB = 0;
    else
      cell->attributeB |= 128;
    if (entry->height != 0)
      cell->attributeB |= 16;
    else
      cell->attributeB = 0;
    rider = entry->rider;
    if (rider == NULL)
      continue;
    if (entry->height == 0) {
      rider->y.fixed = object->y.fixed + -toFIXED(16);
    } else {
      FIXED wave;

      entry->phase += 0x1000;
      wave = Iwram_MulFX(toFIXED(2), Trig_Sin(entry->phase)) - toFIXED(14);
      rider->y.fixed = object->y.fixed + INTtoFIXED(entry->height) + wave;
    }
    rider->x.fixed = object->x.fixed;
    rider->z.fixed = object->z.fixed;
  }
}

/// Draws the rising platforms' columns as sprites, one 16-pixel piece per step
/// of height and a bobbing top, up to 80 of them, each an OAM entry queued by
/// Oam_AddSpriteAtDepthClamped.
void YampiDesert_DrawRisingSand(void) {
  struct RideWork *work = (struct RideWork *)(gInventorySnapshot + 0x188);
  struct RideEntry *entry = work->entries;
  s32 i;
  u32 tile = gVramCacheEntries[work->entry].offset >> 5;
  s32 frame;
  u32 *oam = work->block;
  s32 count = 0;
  struct MapWork *map = Ram_HeapSlots->mapGrid;
  s32 *view = &map->viewX;
  s32 viewX, viewZ, originY;
  struct Actor *object;
  s32 h, x;
  FIXED y;
  s32 sy;
  FIXED z;
  s32 depth;
  u32 *p;

  viewX = view[0];
  viewX &= FIELD_PIXEL_MASK;
  viewZ = view[1] & FIELD_PIXEL_MASK;
  originY = map->origin[1];
  for (i = 15; i >= 0; i--, entry++) {
    if (entry->id == 0 || entry->height == 0)
      continue;
    object = Actor_GetById(entry->id);
    frame = (gFrameCount % 12) / 3 * 8 + 32;
    h = 0;
    if (h >= entry->height)
      goto top;
  again:
    if (count > 79)
      goto next;
    x = object->x.fixed - viewX;
    y = object->y.fixed + INTtoFIXED(h) - originY;
    z = object->z.fixed - viewZ - originY;
    sy = z - y;
    x = FIXEDtoINT(x) - 8;
    sy = (sy >> 16) - 16;
    depth = ((y + z) >> 16) + 58;
    if ((u32)(x + 15) > 334 || sy <= -16 || sy > 239)
      goto next;
    x &= 511;
    sy &= 255;
    p = oam;
    *p++ = 0;
    *p++ = sy | OBJ_X(x) | OBJ_SIZE_16X16 | OBJ_256_COLORS;
    *p = (tile + frame) | OBJ_PRIORITY(2);
    Oam_AddSpriteAtDepthClamped(oam, depth);
    oam += 3;
    count++;
  next:
    h += 16;
    if (h < entry->height)
      goto again;
  top:
    if (count > 79)
      continue;
    x = object->x.fixed - viewX;
    y = object->y.fixed + INTtoFIXED(entry->height) - originY;
    z = object->z.fixed - viewZ - originY;
    sy = z - y;
    x = FIXEDtoINT(x) - 8;
    sy = ((sy + Math_MulFX(-toFIXED(2), Trig_Sin(entry->phase)) - toFIXED(2)) >>
          16) -
         4;
    depth = ((y + z) >> 16) + 58;
    if ((u32)(x + 15) > 334 || sy <= -16 || sy > 239)
      continue;
    x &= 511;
    sy &= 255;
    p = oam;
    *p++ = 0;
    *p++ = sy | OBJ_X(x) | OBJ_SIZE_16X16 | OBJ_256_COLORS;
    *p = (tile + frame - 32) | OBJ_PRIORITY(2);
    Oam_AddSpriteAtDepthClamped(oam, depth);
    oam += 3;
    count++;
  }
}

/// Starts the scorpion pit's rising sand.
void YampiDesert_StartRisingSand(s32 unused, s32 value) {
  struct YampiDesertRideWork *work =
      (struct YampiDesertRideWork *)(gInventorySnapshot + 0x188);
  void *buffer;

  if (!GameFlag_Get(FLAG_KEEP_PARTY_POSITION))
    Iwram_ClearWords((s32)work, 456);
  buffer = (void *)Value1((s32 (*)())Heap_AllocEwram, 0x800);
  Decompress_TaggedLz(gField_GeyserTiles, buffer);
  work->entry = VramCache_FindFreeEntry();
  VramCache_Load(work->entry, 0x800, buffer);
  Heap_Release(buffer);
  work->block = Heap_AllocEwram(0x3c0);
  work->value = value;
  Task_Add((s32)YampiDesert_UpdateRisingSand, TASK_PASS_VBLANK + 13);
  Task_Add((s32)YampiDesert_DrawRisingSand, TASK_PASS_VBLANK);
  Actor_GetById(Party_GetLeaderActor())->priorityFlags |= 0x40;
}

/// Adds an actor to the rising sand, riding between low and high.
struct RideEntry *YampiDesert_AddRisingSand(s32 slot, s32 id, s32 timer,
                                            s32 low, s32 high) {
  struct RideWork *work = (struct RideWork *)(gInventorySnapshot + 0x188);
  struct RideEntry *entry = &work->entries[slot];
  struct Actor *object = Actor_GetById(id);
  struct Actor *rider;

  Actor_GetById(id);
  Actor_SetSpriteFlags(object, 0);
  if (!GameFlag_Get(FLAG_KEEP_PARTY_POSITION)) {
    if (high < low) {
      s32 swap = low;

      low = high;
      high = swap;
    }
    entry->id = id;
    entry->height = 0;
    entry->timer = timer;
    entry->period = 360;
    entry->max = high;
    entry->riseAt = 60;
    entry->fallAt = 240;
    entry->min = low;
    entry->unk10 = 1;
    object->motionFlags = 0;
    object->collisionFlags = ACTOR_NOT_TARGETED;
    entry->step = 0;
    entry->phase = 0;
    entry->unk14 = 0;
    object->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
  }
  rider = Import_ActorCreate(0x8000, object->x.fixed, object->y.fixed,
                             object->z.fixed);
  rider->collisionFlags = 0;
  rider->motionFlags = 0;
  rider->priorityFlags |= 128;
  entry->rider = rider;
  return entry;
}

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

u32 gYampiDesert_SandCloudCells[] = {
    0x9800e0, 0x9800e8, 0x9800f0, 0xa000d8, 0xa000e0, 0xa000e8,
    0xa000f0, 0xa000f8, 0xa800d8, 0xa800e0, 0xa800e8, 0xa800f0,
    0xa800f8, 0xb000d8, 0xb000f8,
};

u32 gYampiDesert_SlideDust[] = {
    0, 25, 22, 38, 0, 3,
    0, 1, 38,
};
// clang-format on

#undef OBJ
#undef MOVE_LEADER_TO
#undef WALK
#undef ARRIVE
#undef WALK_ON
#undef ARRIVE_OR
#undef MAIN_ROUTINE_WORK_U16
#undef Actor_SetAnimationAndWait
#undef Actor_SetMovementTypeAndWaitById

// The Yampi Desert's data: its effect and travelers' scripts, stake lists,
// rooms' tables and triggers.

void YampiDesert_FightBlitz(void);
void YampiDesert_TalkToTraveler(void);
void YampiDesert_TalkToElderAboutWay(void);
void YampiDesert_StepOntoStakeTops(void);
void YampiDesert_HideMedalSpot(void);
void YampiDesert_HideSlideActors(void);

LATE_DATA const s32 *gFieldEffect_DurationScripts[] = {
    gFieldEffect_ShortScript,
    gFieldEffect_MediumScript,
    gFieldEffect_LongScript,
};

// clang-format off
LATE_DATA u32 gYampiDesert_TravelerLooksAround[] = {
    22, 3, 0xb000, 0, 80, 22,
    3, 0, 0, 80, 22, 3,
    0xd000, 0, 20, 22, 3, 0,
    0, 40, 13, 0xffff, 0,
};

LATE_DATA u32 gYampiDesert_OtherTravelerLooksAround[] = {
    22, 3, 0xd000, 0, 20, 22,
    3, 0, 0, 40, 22, 3,
    0xb000, 0, 80, 22, 3, 0,
    0, 80, 13, 0xffff, 0,
};

LATE_DATA u32 gYampiDesert_FinderRunsAhead[] = {
    39, 2, 4, 0xf80000, 0, 0xf80000,
    1, 4, 0xf80000, 0, 0x1180000, 1,
    4, 0x1080000, 0, 0x1900000, 1, 39,
    1, 17,
};

LATE_DATA u32 gYampiDesert_TravelersFollow[] = {
    39, 2, 4, 0xf00000, 0, 0x1180000,
    1, 4, 0x1080000, 0, 0x1900000, 1,
    39, 1, 17,
};

LATE_DATA u32 gYampiDesert_LastTravelersFollow[] = {
    39, 2, 4, 0xd80000, 0, 0xe80000,
    1, 4, 0xf00000, 0, 0x1180000, 1,
    4, 0x1080000, 0, 0x1900000, 1, 39,
    1, 17,
};

LATE_DATA u16 gYampiDesertGuardianRingChestStakes[] = {
    STAKE_ROW(0), STAKE_ROW(1), STAKE_ROW(2), STAKE_ROW(3),
    STAKE_ROW(4), STAKE_ROW(5), STAKE_LIST_END,
};

LATE_DATA u16 gYampiDesertBlitzStakes[] = {
    STAKE_ROW(0), STAKE_ROW(1), STAKE_ROW(2), STAKE_ROW(3),
    STAKE_ROW(4), STAKE_ROW(5), STAKE_ROW(6), STAKE_ROW(7),
    STAKE_ROW(8), STAKE_ROW(9), STAKE_ROW(10), STAKE_LIST_END,
};

LATE_DATA u16 gYampiDesertPsynergyStoneStakes[] = {
    STAKE_ROW(0), STAKE_ROW(1), STAKE_ROW(2), STAKE_ROW(3), STAKE_LIST_END,
};

LATE_DATA u16 gYampiDesertKingScorpionStakes[] = {
    STAKE_ROW(0), STAKE_ROW(1), STAKE_ROW(2), STAKE_LIST_END,
};

LATE_DATA ALIGNED(4) u16 gYampiDesertBlowMaceChestStakes[] = {
    STAKE_ROW(0), STAKE_LIST_END,
};

LATE_DATA s32 gYampiDesert_RaisedEvent = 0;

LATE_DATA struct Actor *gYampiDesert_SandRipple = NULL;

LATE_DATA struct SceneEntrance gYampiDesert_Entrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

LATE_DATA struct SceneEntrance gYampiDesertOasisCaveEntrances[] = {
    {1, CONDITION_ALWAYS, 408, 0, 480, FACING_NORTH, 0, 288, 376, 528, 536, 0},
    {2, CONDITION_ALWAYS, 408, 0, 456, FACING_SOUTH, 0, 288, 376, 528, 536, 0},
    {3, CONDITION_ALWAYS, 120, 0, 424, FACING_SOUTH, 0, 0, 16, 448, 496, 0},
    {4, CONDITION_ALWAYS, 376, 0, 88, FACING_SOUTH, 0, 0, 16, 448, 496, 0},
    {SCENE_TABLE_END},
};

LATE_DATA u32 gYampiDesertScorpionPitExitRegions[] = {
    0x2c00d8, 0xf800b8, 0xd8004c, 0x1ffff, 0xffff, 0,
    0, 0,
};

LATE_DATA s16 gYampiDesertOasisCaveExitRegions[] = {
    112, 24, 416, 128, 40, 432, -1, 3,
    368, 24, 80, 384, 40, 96, -1, 4,
    -1, 0, 0, 0, 0, 0, 0, 0,
};

LATE_DATA u32 gSceneExits[] = {
    SCENE_EXITS(YampiDesertGuardianRingChest),
    SCENE_EXIT(YAMPI_DESERT_GUARDIAN_RING_CHEST_TO_WORLD_MAP, WorldMap, WORLD_MAP_FROM_YAMPI_DESERT_GUARDIAN_RING_CHEST),
    SCENE_EXIT(YAMPI_DESERT_GUARDIAN_RING_CHEST_TO_YAMPI_DESERT_OASIS, YampiDesertOasis, YAMPI_DESERT_OASIS_FROM_YAMPI_DESERT_GUARDIAN_RING_CHEST),
    SCENE_EXITS(YampiDesertOasis),
    SCENE_EXIT(YAMPI_DESERT_OASIS_TO_YAMPI_DESERT_GUARDIAN_RING_CHEST, YampiDesertGuardianRingChest, YAMPI_DESERT_GUARDIAN_RING_CHEST_FROM_YAMPI_DESERT_OASIS),
    SCENE_EXIT(YAMPI_DESERT_OASIS_TO_YAMPI_DESERT_TREASURE_PIT, YampiDesertTreasurePit, YAMPI_DESERT_TREASURE_PIT_FROM_YAMPI_DESERT_OASIS),
    SCENE_EXIT(YAMPI_DESERT_OASIS_TO_YAMPI_DESERT_OASIS_CAVE_NORTH, YampiDesertOasisCave, YAMPI_DESERT_OASIS_CAVE_FROM_YAMPI_DESERT_OASIS_NORTH),
    SCENE_EXIT(YAMPI_DESERT_OASIS_TO_YAMPI_DESERT_BLITZ, YampiDesertBlitz, YAMPI_DESERT_BLITZ_FROM_YAMPI_DESERT_OASIS),
    SCENE_EXIT(YAMPI_DESERT_OASIS_TO_YAMPI_DESERT_OASIS_CAVE_SOUTH, YampiDesertOasisCave, YAMPI_DESERT_OASIS_CAVE_FROM_YAMPI_DESERT_OASIS_SOUTH),
    SCENE_EXITS(YampiDesertBlitz),
    SCENE_EXIT(YAMPI_DESERT_BLITZ_TO_YAMPI_DESERT_OASIS, YampiDesertOasis, YAMPI_DESERT_OASIS_FROM_YAMPI_DESERT_BLITZ),
    SCENE_EXIT(YAMPI_DESERT_BLITZ_TO_YAMPI_DESERT_PSYNERGY_STONE, YampiDesertPsynergyStone, YAMPI_DESERT_PSYNERGY_STONE_FROM_YAMPI_DESERT_BLITZ),
    SCENE_EXITS(YampiDesertPsynergyStone),
    SCENE_EXIT(YAMPI_DESERT_PSYNERGY_STONE_TO_YAMPI_DESERT_BLITZ, YampiDesertBlitz, YAMPI_DESERT_BLITZ_FROM_YAMPI_DESERT_PSYNERGY_STONE),
    SCENE_EXIT(2, YampiDesertPsynergyStone, 3),
    SCENE_EXIT(3, YampiDesertPsynergyStone, 2),
    SCENE_EXIT(YAMPI_DESERT_PSYNERGY_STONE_TO_YAMPI_DESERT_KING_SCORPION, YampiDesertKingScorpion, YAMPI_DESERT_KING_SCORPION_FROM_YAMPI_DESERT_PSYNERGY_STONE),
    SCENE_EXITS(YampiDesertKingScorpion),
    SCENE_EXIT(YAMPI_DESERT_KING_SCORPION_TO_YAMPI_DESERT_PSYNERGY_STONE, YampiDesertPsynergyStone, YAMPI_DESERT_PSYNERGY_STONE_FROM_YAMPI_DESERT_KING_SCORPION),
    SCENE_EXIT(2, YampiDesertKingScorpion, 3),
    SCENE_EXIT(3, YampiDesertKingScorpion, 2),
    SCENE_EXIT(YAMPI_DESERT_KING_SCORPION_TO_YAMPI_DESERT_SCORPION_PIT, YampiDesertScorpionPit, YAMPI_DESERT_SCORPION_PIT_FROM_YAMPI_DESERT_KING_SCORPION),
    SCENE_EXITS(YampiDesertTreasurePit),
    SCENE_EXIT(YAMPI_DESERT_TREASURE_PIT_TO_YAMPI_DESERT_OASIS, YampiDesertOasis, YAMPI_DESERT_OASIS_FROM_YAMPI_DESERT_TREASURE_PIT),
    SCENE_EXIT(YAMPI_DESERT_TREASURE_PIT_TO_YAMPI_DESERT_BLOW_MACE_CHEST, YampiDesertBlowMaceChest, YAMPI_DESERT_BLOW_MACE_CHEST_FROM_YAMPI_DESERT_TREASURE_PIT),
    SCENE_EXITS(YampiDesertSandPass),
    SCENE_EXIT(YAMPI_DESERT_SAND_PASS_TO_YAMPI_DESERT_BLOW_MACE_CHEST, YampiDesertBlowMaceChest, YAMPI_DESERT_BLOW_MACE_CHEST_FROM_YAMPI_DESERT_SAND_PASS),
    SCENE_EXIT(YAMPI_DESERT_SAND_PASS_TO_YAMPI_DESERT_TRAINERS_WHIP_CHEST, YampiDesertTrainersWhipChest, YAMPI_DESERT_TRAINERS_WHIP_CHEST_FROM_YAMPI_DESERT_SAND_PASS),
    SCENE_EXITS(YampiDesertTrainersWhipChest),
    SCENE_EXIT(YAMPI_DESERT_TRAINERS_WHIP_CHEST_TO_YAMPI_DESERT_SAND_PASS, YampiDesertSandPass, YAMPI_DESERT_SAND_PASS_FROM_YAMPI_DESERT_TRAINERS_WHIP_CHEST),
    SCENE_EXIT(YAMPI_DESERT_TRAINERS_WHIP_CHEST_TO_WORLD_MAP, WorldMap, WORLD_MAP_FROM_YAMPI_DESERT_TRAINERS_WHIP_CHEST),
    SCENE_EXIT(YAMPI_DESERT_TRAINERS_WHIP_CHEST_TO_YAMPI_DESERT_MEDAL_SPOT, YampiDesertMedalSpot, YAMPI_DESERT_MEDAL_SPOT_FROM_YAMPI_DESERT_TRAINERS_WHIP_CHEST),
    SCENE_EXIT(YAMPI_DESERT_TRAINERS_WHIP_CHEST_TO_YAMPI_DESERT_CAVE_WATER_OF_LIFE_CHEST, YampiDesertCaveWaterOfLifeChest, YAMPI_DESERT_CAVE_WATER_OF_LIFE_CHEST_FROM_YAMPI_DESERT_TRAINERS_WHIP_CHEST),
    SCENE_EXITS(YampiDesertMedalSpot),
    SCENE_EXIT(YAMPI_DESERT_MEDAL_SPOT_TO_YAMPI_DESERT_TRAINERS_WHIP_CHEST, YampiDesertTrainersWhipChest, YAMPI_DESERT_TRAINERS_WHIP_CHEST_FROM_YAMPI_DESERT_MEDAL_SPOT),
    SCENE_EXIT(YAMPI_DESERT_MEDAL_SPOT_TO_WORLD_MAP, WorldMap, WORLD_MAP_FROM_YAMPI_DESERT_MEDAL_SPOT),
    SCENE_EXITS(YampiDesertScorpionPit),
    SCENE_EXIT(YAMPI_DESERT_SCORPION_PIT_TO_YAMPI_DESERT_KING_SCORPION, YampiDesertKingScorpion, YAMPI_DESERT_KING_SCORPION_FROM_YAMPI_DESERT_SCORPION_PIT),
    SCENE_EXITS(YampiDesertOasisCave),
    SCENE_EXIT(YAMPI_DESERT_OASIS_CAVE_TO_YAMPI_DESERT_OASIS_NORTH, YampiDesertOasis, YAMPI_DESERT_OASIS_FROM_YAMPI_DESERT_OASIS_CAVE_NORTH),
    SCENE_EXIT(2, YampiDesertOasisCave, 4),
    SCENE_EXIT(YAMPI_DESERT_OASIS_CAVE_TO_YAMPI_DESERT_OASIS_SOUTH, YampiDesertOasis, YAMPI_DESERT_OASIS_FROM_YAMPI_DESERT_OASIS_CAVE_SOUTH),
    SCENE_EXIT(4, YampiDesertOasisCave, 2),
    SCENE_EXITS(YampiDesertBlowMaceChest),
    SCENE_EXIT(YAMPI_DESERT_BLOW_MACE_CHEST_TO_YAMPI_DESERT_TREASURE_PIT, YampiDesertTreasurePit, YAMPI_DESERT_TREASURE_PIT_FROM_YAMPI_DESERT_BLOW_MACE_CHEST),
    SCENE_EXIT(YAMPI_DESERT_BLOW_MACE_CHEST_TO_YAMPI_DESERT_SAND_PASS, YampiDesertSandPass, YAMPI_DESERT_SAND_PASS_FROM_YAMPI_DESERT_BLOW_MACE_CHEST),
    SCENE_EXITS_END,
};

LATE_DATA struct ActorPlacement gYampiDesert_Placements[] = {
    {SCENE_TABLE_END},
};

LATE_DATA struct ActorPlacement gYampiDesertGuardianRingChestActorPlacements[] = {
    ACTOR_PLACEMENT(406, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 296, 0, 216, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(406, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 424, 0, 168, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(406, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 456, 0, 136, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(406, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 616, 0, 168, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(406, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 632, 0, 392, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(406, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 680, 0, 424, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

LATE_DATA struct ActorPlacement gYampiDesertOasisActorPlacements[] = {
    ACTOR_PLACEMENT(88, CONDITION_ALWAYS, (s32)gYampiDesert_TravelerLooksAround, 200, 0, 216, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(88, CONDITION_ALWAYS, (s32)gYampiDesert_OtherTravelerLooksAround, 216, 0, 200, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(114, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 152, 0, 200, FACING_SOUTH_SOUTHEAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(93, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 162, 0, 232, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(112, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 212, 0, 246, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(19, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 230, 0, 248, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(20, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 234, 0, 230, FACING_EAST, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

LATE_DATA struct ActorPlacement gYampiDesertOasisActorPlacementsAfterMeeting[] = {
    ACTOR_PLACEMENT(88, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 520, 0, 242, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(88, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 533, 0, 271, FACING_WEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(114, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 435, 0, 323, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(93, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 467, 0, 248, FACING_SOUTH_SOUTHWEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(112, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 462, 0, 340, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(19, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 482, 0, 340, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(20, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 489, 0, 320, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    {SCENE_TABLE_END},
};

LATE_DATA struct ActorPlacement gYampiDesertBlitzActorPlacements[] = {
    ACTOR_PLACEMENT(406, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 216, 0, 184, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(406, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 376, 0, 216, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(406, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 216, 0, 344, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(406, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 152, 0, 376, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(406, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 168, 0, 520, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(406, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 296, 0, 456, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(406, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 328, 0, 456, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(406, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 344, 0, 552, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(406, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 296, 0, 616, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(406, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 504, 0, 376, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(406, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 536, 0, 728, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_JUPITER_DJINNI, FLAG_DJINNI(DJINNI_BLITZ), MOVEMENT_TYPE_STAND, 504, 0, 568, FACING_SOUTH, TALK_FACING_PARTY, 0),
    {SCENE_TABLE_END},
};

LATE_DATA struct ActorPlacement gYampiDesertPsynergyStoneActorPlacements[] = {
    ACTOR_PLACEMENT(406, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 136, 0, 568, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(406, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 232, 0, 600, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(406, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 296, 0, 568, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(406, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 184, 0, 232, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(406, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 184, 0, 184, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(406, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 296, 0, 200, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(406, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 168, 0, 264, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(406, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 280, 0, 280, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(103, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 232, 0, 200, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(251, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

LATE_DATA struct ActorPlacement gYampiDesertKingScorpionActorPlacements[] = {
    ACTOR_PLACEMENT(406, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 232, 0, 728, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(406, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 200, 0, 296, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(406, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 280, 0, 344, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(406, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 232, 0, 648, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(406, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 184, 0, 696, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(406, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 168, 0, 728, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(406, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 216, 0, 760, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(406, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 328, 0, 664, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(406, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 296, 0, 712, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(406, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 312, 0, 744, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(406, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 296, 0, 776, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(406, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 280, 0, 776, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(406, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(103, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(251, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(354, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

LATE_DATA struct ActorPlacement gYampiDesertTrainersWhipChestActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 408, 0, 360, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 360, 0, 360, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(450, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 768, 0, 616, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(337, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 824, 0, 440, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(337, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 824, 0, 424, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

LATE_DATA struct ActorPlacement gYampiDesertScorpionPitActorPlacements[] = {
    ACTOR_PLACEMENT(103, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(489, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

LATE_DATA struct ActorPlacement gYampiDesertBlowMaceChestActorPlacements[] = {
    ACTOR_PLACEMENT(406, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 520, 0, 152, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(337, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 488, 0, 184, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(337, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 504, 0, 184, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(337, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 520, 0, 184, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(337, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 520, 0, 200, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(337, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 536, 0, 216, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(337, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 584, 0, 264, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(337, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 776, 0, 264, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    {SCENE_TABLE_END},
};

LATE_DATA struct Trigger gYampiDesert_Triggers[] = {
    {TRIGGER_TABLE_END},
};

LATE_DATA struct Trigger gYampiDesertGuardianRingChestTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), STAKE(0), FLAG_STAKE_POUNDED(0), EVENT(YampiDesert_PoundGuardianRingChestStake)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), STAKE(1), FLAG_STAKE_POUNDED(1), EVENT(YampiDesert_PoundGuardianRingChestStake)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), STAKE(2), FLAG_STAKE_POUNDED(2), EVENT(YampiDesert_PoundGuardianRingChestStake)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), STAKE(3), FLAG_STAKE_POUNDED(3), EVENT(YampiDesert_PoundGuardianRingChestStake)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), STAKE(4), FLAG_STAKE_POUNDED(4), EVENT(YampiDesert_PoundGuardianRingChestStake)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), STAKE(5), FLAG_STAKE_POUNDED(5), EVENT(YampiDesert_PoundGuardianRingChestStake)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_AFTER_CAST), 0, CONDITION_ALWAYS, EVENT(YampiDesert_CastSand)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(YampiDesert_EndSand)},
    {TRIGGER_TABLE_END},
};

LATE_DATA struct Trigger gYampiDesertOasisTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {STEP_TRIGGER_WALKING, 5, CONDITION_ALWAYS, EVENT(YampiDesert_ClimbRevealedLadder)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_AFTER_CAST), 0, CONDITION_ALWAYS, EVENT(YampiDesert_CastSand)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(YampiDesert_EndSand)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_SCOOP, PSYNERGY_PHASE_ACTS), 10, FLAG_YAMPI_DESERT_LADDER_DUG_UP, EVENT(YampiDesert_ScoopLadderPit)},
    {TRIGGER_TALK, ACTOR_TRAVELER_FOLLOWING_THE_PIRATES, FLAG_YAMPI_DESERT_TRAVELERS_MET, EVENT(YampiDesert_TalkToTraveler)},
    {TRIGGER_TALK, ACTOR_TRAVELER_FOLLOWING_THE_PIRATES, CONDITION_ALWAYS, MSGNO(MsgAtLastWe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TRAVELER_FOLLOWING_THE_PIRATES, FLAG_YAMPI_DESERT_TRAVELERS_MET, MSGNO(MsgBriggsAndThe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TRAVELER_FOLLOWING_THE_PIRATES, CONDITION_ALWAYS, MSGNO(MsgOhNotAgain)},
    {TRIGGER_TALK, ACTOR_TRAVELER_SEEKING_EVIDENCE, FLAG_YAMPI_DESERT_TRAVELERS_MET, MSGNO(MsgIfThatAccursed)},
    {TRIGGER_TALK, ACTOR_TRAVELER_SEEKING_EVIDENCE, CONDITION_ALWAYS, MSGNO(MsgIWasCertain)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TRAVELER_SEEKING_EVIDENCE, FLAG_YAMPI_DESERT_TRAVELERS_MET, MSGNO(MsgThatIsSo)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TRAVELER_SEEKING_EVIDENCE, CONDITION_ALWAYS, MSGNO(MsgMyJobIs)},
    {TRIGGER_TALK, ACTOR_DIZZY_TRAVELER, FLAG_YAMPI_DESERT_TRAVELERS_MET, MSGNO(MsgOooohThisDesert)},
    {TRIGGER_TALK, ACTOR_DIZZY_TRAVELER, CONDITION_ALWAYS, MSGNO(MsgIMJust)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_DIZZY_TRAVELER, FLAG_YAMPI_DESERT_TRAVELERS_MET, MSGNO(MsgSureItS)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_DIZZY_TRAVELER, CONDITION_ALWAYS, MSGNO(MsgMaybeWeShould)},
    {TRIGGER_TALK, ACTOR_PARCHED_TRAVELER, FLAG_YAMPI_DESERT_TRAVELERS_MET, MSGNO(MsgWaterMyThroat)},
    {TRIGGER_TALK, ACTOR_PARCHED_TRAVELER, CONDITION_ALWAYS, MSGNO(MsgOhManThis)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_PARCHED_TRAVELER, FLAG_YAMPI_DESERT_TRAVELERS_MET, MSGNO(MsgThereSA)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_PARCHED_TRAVELER, CONDITION_ALWAYS, MSGNO(MsgIDonTWant)},
    {TRIGGER_TALK, ACTOR_TRAVELER_WHO_WOULD_TURN_BACK, FLAG_YAMPI_DESERT_TRAVELERS_MET, EVENT(YampiDesert_TalkToElderAboutWay)},
    {TRIGGER_TALK, ACTOR_TRAVELER_WHO_WOULD_TURN_BACK, CONDITION_ALWAYS, MSGNO(MsgOseniaIsSo)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12 | TRIGGER_SUBJECT_OWN_EFFECT, FLAG_YAMPI_DESERT_ELDER_INSISTED, EVENT(YampiDesert_TalkToElderAboutWay)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TRAVELER_WHO_WOULD_TURN_BACK, FLAG_YAMPI_DESERT_TRAVELERS_MET, MSGNO(MsgWeVeReached)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TRAVELER_WHO_WOULD_TURN_BACK, CONDITION_ALWAYS, MSGNO(MsgHowMuchLonger)},
    {TRIGGER_TALK, ACTOR_ELDER, FLAG_YAMPI_DESERT_TRAVELERS_MET, EVENT(YampiDesert_TalkToElder)},
    {TRIGGER_TALK, ACTOR_ELDER, CONDITION_ALWAYS, MSGNO(MsgMySonHas)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13 | TRIGGER_SUBJECT_OWN_EFFECT, FLAG_YAMPI_DESERT_ELDER_INSISTED, EVENT(YampiDesert_TalkToElder)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ELDER, FLAG_YAMPI_DESERT_TRAVELERS_MET, MSGNO(MsgMySonIs)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ELDER, CONDITION_ALWAYS, MSGNO(MsgRegardlessOfWhat)},
    {TRIGGER_TALK, ACTOR_MAYOR, FLAG_YAMPI_DESERT_TRAVELERS_MET, MSGNO(MsgFatherHowAm)},
    {TRIGGER_TALK, ACTOR_MAYOR, CONDITION_ALWAYS, MSGNO(MsgIfWeRemain)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAYOR, FLAG_YAMPI_DESERT_TRAVELERS_MET, MSGNO(MsgMyFatherS)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAYOR, CONDITION_ALWAYS, MSGNO(MsgHoooICan)},
    {TRIGGER_STEP, 15, FLAG_YAMPI_DESERT_TRAVELERS_MET, EVENT(YampiDesert_SeeTravelersAbove)},
    {TRIGGER_TABLE_END},
};

LATE_DATA struct Trigger gYampiDesertBlitzTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), STAKE(0), FLAG_STAKE_POUNDED(0), EVENT(YampiDesert_PoundBlitzStake)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), STAKE(1), FLAG_STAKE_POUNDED(1), EVENT(YampiDesert_PoundBlitzStake)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), STAKE(2), FLAG_STAKE_POUNDED(2), EVENT(YampiDesert_PoundBlitzStake)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), STAKE(3), FLAG_STAKE_POUNDED(3), EVENT(YampiDesert_PoundBlitzStake)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), STAKE(4), FLAG_STAKE_POUNDED(4), EVENT(YampiDesert_PoundBlitzStake)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), STAKE(5), FLAG_STAKE_POUNDED(5), EVENT(YampiDesert_PoundBlitzStake)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), STAKE(6), FLAG_STAKE_POUNDED(6), EVENT(YampiDesert_PoundBlitzStake)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), STAKE(7), FLAG_STAKE_POUNDED(7), EVENT(YampiDesert_PoundBlitzStake)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), STAKE(8), FLAG_STAKE_POUNDED(8), EVENT(YampiDesert_PoundBlitzStake)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), STAKE(9), FLAG_STAKE_POUNDED(9), EVENT(YampiDesert_PoundBlitzStake)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), STAKE(10), FLAG_STAKE_POUNDED(10), EVENT(YampiDesert_PoundBlitzStake)},
    {TRIGGER_STEP, 11, FLAG_YAMPI_DESERT_ON_STAKE_TOPS, EVENT(YampiDesert_StepOntoStakeTops)},
    {TRIGGER_STEP, 10, IF_FLAG_SET(FLAG_YAMPI_DESERT_ON_STAKE_TOPS), EVENT(YampiDesert_StepOffStakeTops)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_AFTER_CAST), 0, CONDITION_ALWAYS, EVENT(YampiDesert_CastSand)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(YampiDesert_EndSand)},
    {TRIGGER_TALK, ACTOR_BLITZ, CONDITION_ALWAYS, EVENT(YampiDesert_FightBlitz)},
    {TRIGGER_TABLE_END},
};

LATE_DATA struct Trigger gYampiDesertPsynergyStoneTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), STAKE(0), FLAG_STAKE_POUNDED(0), EVENT(YampiDesert_PoundPsynergyStoneStake)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), STAKE(1), FLAG_STAKE_POUNDED(1), EVENT(YampiDesert_PoundPsynergyStoneStake)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), STAKE(2), FLAG_STAKE_POUNDED(2), EVENT(YampiDesert_PoundPsynergyStoneStake)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), STAKE(3), FLAG_STAKE_POUNDED(3), EVENT(YampiDesert_PoundPsynergyStoneStake)},
    {TRIGGER_STEP, 11, FLAG_YAMPI_DESERT_ON_STAKE_TOPS, EVENT(YampiDesert_StepOntoStakeTops)},
    {TRIGGER_STEP, 10, IF_FLAG_SET(FLAG_YAMPI_DESERT_ON_STAKE_TOPS), EVENT(YampiDesert_StepOffStakeTops)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_AFTER_CAST), 0, CONDITION_ALWAYS, EVENT(YampiDesert_CastSand)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(YampiDesert_EndSand)},
    {TRIGGER_RAISED, RAISED_WATCH_SCORPION_STOP, CONDITION_ALWAYS, EVENT(YampiDesert_WatchScorpionStop)},
    {TRIGGER_RAISED, RAISED_STIR_SCORPION, CONDITION_ALWAYS, EVENT(YampiDesert_StirScorpion)},
    {TRIGGER_TABLE_END},
};

LATE_DATA struct Trigger gYampiDesertKingScorpionTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), STAKE(0), FLAG_STAKE_POUNDED(0), EVENT(YampiDesert_PoundKingScorpionStake)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), STAKE(1), FLAG_STAKE_POUNDED(1), EVENT(YampiDesert_PoundKingScorpionStake)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), STAKE(2), FLAG_STAKE_POUNDED(2), EVENT(YampiDesert_PoundKingScorpionStake)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_AFTER_CAST), 0, CONDITION_ALWAYS, EVENT(YampiDesert_CastSand)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(YampiDesert_EndSand)},
    {TRIGGER_RAISED, RAISED_WATCH_SCORPION_STOP, FLAG_YAMPI_DESERT_SCORPION_CORNERED, EVENT(YampiDesert_WatchScorpionStop)},
    {TRIGGER_RAISED, RAISED_STIR_SCORPION_AGAIN, CONDITION_ALWAYS, EVENT(YampiDesert_StirScorpionAgain)},
    {TRIGGER_RAISED, RAISED_CORNER_SCORPION, CONDITION_ALWAYS, EVENT(YampiDesert_CornerScorpion)},
    {TRIGGER_STEP, 10, FLAG_KING_SCORPION_BEATEN, EVENT(YampiDesert_TrapKingScorpion)},
    {TRIGGER_TABLE_END},
};

LATE_DATA struct Trigger gYampiDesertTreasurePitTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_AFTER_CAST), 0, CONDITION_ALWAYS, EVENT(YampiDesert_CastSand)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(YampiDesert_EndSand)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_SCOOP, PSYNERGY_PHASE_ACTS), 12, FLAG_YAMPI_DESERT_TREASURE_PIT_DUG_UP, EVENT(YampiDesert_ScoopTreasurePit)},
    {TRIGGER_TABLE_END},
};

LATE_DATA struct Trigger gYampiDesertSandPassTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_AFTER_CAST), 0, CONDITION_ALWAYS, EVENT(YampiDesert_CastSand)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(YampiDesert_EndSand)},
    {TRIGGER_TABLE_END},
};

LATE_DATA struct Trigger gYampiDesertTrainersWhipChestTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {PSYNERGY_ON_ACTOR(ABILITY_LASH), ACTOR_ROPE_POLE, FLAG_YAMPI_DESERT_ROPE_TIED, 0},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_AFTER_CAST), 0, CONDITION_ALWAYS, EVENT(YampiDesert_CastSand)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(YampiDesert_EndSand)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0, CONDITION_ALWAYS, EVENT(YampiDesert_RevealRockHint)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(YampiDesert_HideRockHint)},
    {TRIGGER_TALK, ACTOR_SIGNPOST, CONDITION_ALWAYS, EVENT(YampiDesert_ReadSignpost)},
    {TRIGGER_STEP, 7, IF_FLAG_SET(FLAG_YAMPI_DESERT_SLIDE_ACTORS_SHOWN), EVENT(YampiDesert_HideSlideActors)},
    {TRIGGER_STEP, 8, FLAG_YAMPI_DESERT_SLIDE_ACTORS_SHOWN, EVENT(YampiDesert_ShowSlideActors)},
    {TRIGGER_TABLE_END},
};

LATE_DATA struct Trigger gYampiDesertMedalSpotTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0, CONDITION_ALWAYS, EVENT(YampiDesert_RevealMedalSpot)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(YampiDesert_HideMedalSpot)},
    {TRIGGER_TABLE_END},
};

LATE_DATA struct Trigger gYampiDesertScorpionPitTriggers[] = {
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_AFTER_CAST), 0, CONDITION_ALWAYS, EVENT(YampiDesert_CastSand)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(YampiDesert_EndSand)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_SCOOP, PSYNERGY_PHASE_ACTS), 10, FLAG_YAMPI_DESERT_ROCK_PIT_DUG_UP, EVENT(YampiDesert_ScoopCenterPit)},
    {TRIGGER_TABLE_END},
};

LATE_DATA struct Trigger gYampiDesertOasisCaveTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 2, CONDITION_ALWAYS, EVENT(YampiDesert_ClimbOutOfOasisCave)},
    {TRIGGER_TABLE_END},
};

LATE_DATA struct Trigger gYampiDesertBlowMaceChestTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), STAKE(0), FLAG_STAKE_POUNDED(0), EVENT(YampiDesert_PoundBlowMaceChestStake)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_AFTER_CAST), 0, CONDITION_ALWAYS, EVENT(YampiDesert_CastSand)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(YampiDesert_EndSand)},
    {TRIGGER_TABLE_END},
};

LATE_DATA u32 gYampiDesert_SandCloudDrifts[] = {
    0, 1, 4, 0xe80000, 0, 0xba0000,
    1, 38,
};
// clang-format on
