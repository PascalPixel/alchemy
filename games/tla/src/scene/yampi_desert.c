/// @file
/// The Yampi Desert, between southern Osenia and Alhafra: twelve rooms on one
/// overlay (SceneId_YampiDesertGuardianRingChest to
/// SceneId_YampiDesertBlowMaceChest). In the first room Pound sinks the wooden
/// stake before the Guardian Ring's chest. In the second, travelers from Madra
/// chasing Briggs rest by the spring, beside a sand puddle that hides a ladder.
/// In Blitz's room the stakes are Pounded into the sand or walked across on
/// their tops, to the Antidote's chest and up to Blitz. Past the Psynergy
/// Stone the King Scorpion moves under the sand: Pound on a stake makes it
/// burst out and flee north, twice, until the stakes of the trap corner it in
/// the ring of rocks at the north of its room. Walking up to it, Felix is
/// dragged down with it by its Scoop into its pit and fights it for the Scoop
/// Gem; Scoop between the pit's four rocks then opens a geyser out. Back at the
/// spring, Scoop opens the ladder, whose cave leads up to a ledge over the
/// travelers, who then find their way north; the next room hides 315 coins in
/// a sand puddle, and the cave beyond it, all slides of flowing sand, holds the
/// Hard Nut and the Blow Mace. In the long room after it Reveal changes two
/// rings of rocks, one to a hint and one to the Lucky Medal's chest, and Lash
/// ties the rope to the Trainer's Whip's chest; Reveal also brings out a
/// Psynergy Stone in the room that leads on from there to the World Map.
/// Sand lets Felix travel under the sand, past rocks and standing stakes, in
/// most of the rooms.
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
#include "field/field_sequence.h"
#include "field/field_sequence_event.h"
#include "field/field_sprite.h"
#include "field/flag_call.h"
#include "field/map_grid.h"
#include "field/object.h"
#include "field/scene_tables.h"
#include "game/game.h"
#include "graphics/glyph.h"
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
#include "system/main_routine_state.h"
#include "system/math_table.h"
#include "system/ram_buffer.h"
#include "system/system.h"
#include "system/system_call.h"
#include "system/task.h"
#include "window/item_icon.h"
#include "text/msg_ids.h"

#include "../field/actor_set_sprite_priority.c"

#include "../field/field_effect_move.c"

#define EFFECT_SET_MODE Actor_SelectAnimation
#define EFFECT_SET_SCRIPT Actor_StartScript
#include "../field/field_effect_spawn.c"

/// Set as Pound on a stake of the Psynergy Stone's room stirs the King Scorpion
/// out of the sand, and it flees into the King Scorpion's room.
#define FLAG_YAMPI_DESERT_SCORPION_STIRRED 0x889
/// As FLAG_YAMPI_DESERT_SCORPION_STIRRED, in the King Scorpion's room, from
/// whose south part it flees north.
#define FLAG_YAMPI_DESERT_SCORPION_STIRRED_AGAIN 0x88a
/// Set once the King Scorpion is cornered at the ring of rocks at the north end
/// of its room.
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
/// stand about the spring from then on.
#define FLAG_YAMPI_DESERT_TRAVELERS_MET 0x8f0
/// Set while the leader walks on the tops of the stakes Pound has not sunk.
#define FLAG_YAMPI_DESERT_ON_STAKE_TOPS FLAG_SCENE_TEMP_0B
/// Set while the unseen blocks by the sand's flow are shown.
#define FLAG_YAMPI_DESERT_BLOCKS_SHOWN FLAG_SCENE_TEMP_0D
/// Set once Lash has tied the Trainer's Whip chest's room's rope.
#define FLAG_YAMPI_DESERT_ROPE_TIED FLAG_SCENE_TEMP_0C
/// Set once Scoop has dug up the 315 coins in the room north of the spring.
#define FLAG_YAMPI_DESERT_COINS_PUDDLE_DUG_UP FLAG_AREA_TEMP_02

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

/// The cell behaviors the desert's code reads: sand that flows north, south,
/// west or east and carries the leader, and a rise to the upper level.
enum YampiDesertCellBehavior {
  CELL_BEHAVIOR_YAMPI_DESERT_FLOW_NORTH = 10,
  CELL_BEHAVIOR_YAMPI_DESERT_FLOW_SOUTH = 11,
  CELL_BEHAVIOR_YAMPI_DESERT_FLOW_WEST = 12,
  CELL_BEHAVIOR_YAMPI_DESERT_FLOW_EAST = 13,
  CELL_BEHAVIOR_YAMPI_DESERT_RISE = 20
};

/// The ways between the two parts of the Psynergy Stone's room and of the King
/// Scorpion's, each part seen through its own camera box: the exit from the
/// south part into the north and back, and the entrances they arrive by.
enum YampiDesertRoomPart {
  EXIT_TO_NORTH_PART = 2,
  EXIT_TO_SOUTH_PART = 3,
  ENTRANCE_FROM_NORTH_PART = 2, ///< arriving in the south part
  ENTRANCE_FROM_SOUTH_PART = 3  ///< arriving in the north part
};

/// The ways between the small chamber of the cave under the spring, where the
/// ladder from the spring comes down, and the cave below it.
enum YampiDesertSpringCaveLevel {
  SPRING_CAVE_TO_LOWER_CAVE = 2, ///< the chamber's ladder down
  SPRING_CAVE_TO_CHAMBER = 4,
  SPRING_CAVE_FROM_CHAMBER = 4,
  SPRING_CAVE_FROM_LOWER_CAVE = 2
};

/// The cells the rooms' trigger tables answer, by the behaviour each room's
/// map gives them.
enum YampiDesertCell {
  /// The rooms with stakes: stepping from the high ground beside the stakes,
  /// and back down off them.
  CELL_ONTO_STAKE_TOPS = 11,
  CELL_OFF_STAKE_TOPS = 10,
  /// The spring's room: the ring of cells round the sand puddle, the puddle
  /// that hides the ladder, and the ledge over the spring.
  CELL_AROUND_LADDER_PUDDLE = 5,
  CELL_LADDER_PUDDLE = 10,
  CELL_LEDGE_OVER_SPRING = 15,
  /// The room north of the spring: the sand puddle over the 315 coins.
  CELL_COINS_PUDDLE = 12,
  /// The King Scorpion's room: before the ring of rocks of the trap.
  CELL_BEFORE_TRAP = 10,
  /// The King Scorpion's pit: the sand between its four rocks.
  CELL_BETWEEN_FOUR_ROCKS = 10,
  /// The Trainer's Whip's chest's room: the inner and outer corner of the
  /// flowing sand by the invisible blocks.
  CELL_FLOW_CORNER_INSIDE = 7,
  CELL_FLOW_CORNER_OUTSIDE = 8
};

void Actor_SetWideSprite(s32 actor, s32 value);

void YampiDesert_StepOntoStakeTops(void);
void YampiDesert_OpenLadderPit(void);
void YampiDesert_SetBlocksShown(s32 show);
void YampiDesert_HideBlocks(void);
void Actors_SetDrawn(s32 show, s32 first, u32 count);
void YampiDesert_UpdateSandFlow(void);
void YampiDesert_CheckUpperLevel(void);
void YampiDesert_OpenScoopHole(void);
void Actor_Hide(s32 id);
void YampiDesert_RestoreBlocks(void);
void YampiDesert_SetUpBlowMaceChestBlocks(void);
void YampiDesert_ClimbDownLadder(struct Actor *actor, s32 x, s32 z, s32 exit);
void YampiDesert_LayerStakesWithLeader(void);
void YampiDesert_PlaceStakes(const void *script);
void Pound_SinkTarget(u16 *list, s32 mode, s32 count);
void Actor_DisableById(s32 actor);
void FieldEffect_StartBlankOverlay(s32 value);
void YampiDesert_ElderRefusesToQuit(void);
void YampiDesert_BlockPoundedStakes(u16 *list);
void YampiDesert_OpenStakeCells(u16 *list);
void FieldEvent_Wait(s32 frames);
struct Actor *YampiDesert_CreateScoopGem(void);
void Actor_Release(struct Actor *actor);
extern struct Actor *gYampiDesert_SandRipple;
void YampiDesert_ClearRaisedEvent(void);
void YampiDesert_PlayScorpionRumble(void);
void YampiDesert_SetUpPsynergyStoneScorpion(void);
void YampiDesert_SetStakeSunk(s32 id);
void YampiDesert_CheckUpperLevelFromLower(void);
void Actor_SetCellOpen(s32 id, u32 kind, s32 flag, s32 layer);

/// Places an actor at (x, z) on the ground, facing a direction.
static inline void YampiDesert_SetPositionAndFacing(s32 actorId, s32 x, s32 z,
                                                    u16 facing) {
  Actor_SetHorizontalPositionAndAngleWithTerrain(actorId, x, z, facing);
}

/// Places a rope between two posts.
static inline void YampiDesert_PlaceRope(s32 index, s32 flag, s32 post,
                                         s32 otherPost) {
  Rope_Place(index, flag, post, otherPost);
}

/// Scoop digs up a buried treasure: with `mode` 2, the hidden item comes up at
/// its home and its flag is set (Actor_ReturnHome).
static inline void YampiDesert_DigUpTreasure(s32 mode, s32 actorId, s32 flag) {
  Actor_ReturnHome(mode, actorId, flag);
}

/// The stakes Pound sinks, the first rows of the rooms that have them (the
/// Guardian Ring chest's, Blitz's, the Psynergy Stone's, the King Scorpion's
/// and the Blow Mace chest's): stake n is the placed actor STAKE(n), and
/// FLAG_YAMPI_DESERT_STAKE_POUNDED(n) is set once Pound has sunk it.
#define STAKE(n) (ACTOR_FIRST_PLACED + (n))
#define FLAG_YAMPI_DESERT_STAKE_POUNDED(n) (FLAG_SCENE_TEMP_00 + (n))
/// A row of a room's stake list, as Pound_SinkTarget and Actor_SetCellOpen read
/// it: the stake, its look (0, wood) and its flag. A list ends at
/// ACTOR_LIST_END.
#define STAKE_ROW(n) STAKE(n), 0, FLAG_YAMPI_DESERT_STAKE_POUNDED(n)

/// The travelers from Madra resting by the spring (SceneId_YampiDesertSpring),
/// by the rows of gYampiDesertSpringActorPlacements and
/// gYampiDesertSpringActorPlacementsAfterMeeting, chasing Briggs to Alhafra.
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

/// Blitz's room's last stake, standing at the foot of the cliff before a
/// climbable wall, which shows once Pound has sunk it.
#define ACTOR_CLIMBABLE_WALL_STAKE STAKE(10)
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
  /// Unused: a brown boulder placed at the origin, which nothing moves, so it
  /// is never on stage.
  ACTOR_KING_SCORPION_ROOM_BOULDER
};

/// The Trainer's Whip chest's room's actors
/// (SceneId_YampiDesertTrainersWhipChest).
enum YampiDesertTrainersWhipChestActorId {
  /// The posts of the rope Lash ties.
  ACTOR_ROPE_POST = ACTOR_FIRST_PLACED,
  ACTOR_ROPE_OTHER_POST,
  ACTOR_SIGN,
  /// Two unseen blocks by the sand's flow, shown and hidden by the step
  /// triggers beside it (YampiDesert_SetBlocksShown).
  ACTOR_WHIP_CHEST_ROOM_FIRST_BLOCK,
  ACTOR_WHIP_CHEST_ROOM_SECOND_BLOCK
};

/// The scorpion pit's actors (SceneId_YampiDesertScorpionPit), where the King
/// Scorpion is fought.
enum YampiDesertScorpionPitActorId {
  ACTOR_KING_SCORPION = ACTOR_FIRST_PLACED,
  /// The unseen point the geyser rises from, moved to the middle of the four
  /// rocks once Scoop has dug the pit there; Geyser_Add takes its shadow off.
  ACTOR_GEYSER
};

/// The room's hidden item: the Lucky Medal's chest in the Trainer's Whip's
/// chest's room, the Psynergy Stone of the room after it, and the 315 coins
/// under the sand puddle north of the spring.
enum { ACTOR_HIDDEN_TREASURE = FIELD_TREASURE_ACTOR };

/// The Blow Mace chest's room's actors after its stake
/// (SceneId_YampiDesertBlowMaceChest): seven unseen blocks by the sand's flow,
/// hidden as the room is set up.
enum YampiDesertBlowMaceChestActorId {
  ACTOR_BLOW_MACE_CHEST_ROOM_FIRST_BLOCK = STAKE(1),
  ACTOR_BLOW_MACE_CHEST_ROOM_LAST_BLOCK =
          ACTOR_BLOW_MACE_CHEST_ROOM_FIRST_BLOCK +
      6
};

void YampiDesert_RippleFollowsScorpion(void);

/// The points of gYampiDesert_SandCloudPoints.
#define SAND_CLOUD_POINTS 15

extern u16 gYampiDesert_SandCloudPoints[SAND_CLOUD_POINTS * 2];

extern u32 gYampiDesert_SandCloudDrifts[];

void YampiDesert_ScorpionUsesScoop(void);
/// The centre of a cell's column or row, in pixels.
s32 YampiDesert_CellCenter(s32 cell) __attribute__((const));
extern u32 gYampiDesert_FlowDust[];
void YampiDesert_SetUpGuardianRingChest(void);
void YampiDesert_SetUpSpring(void);
void YampiDesert_SetUpBlitz(void);
void YampiDesert_SetUpPsynergyStone(void);
void YampiDesert_SetUpKingScorpion(void);

void YampiDesert_SetUpCoinsPuddle(void);

void YampiDesert_SetUpTrainersWhipChest(void);
void YampiDesert_SetUpHiddenPsynergyStone(void);
void YampiDesert_SetUpScorpionPit(void);
void YampiDesert_SetUpSpringCave(void);
void YampiDesert_SetUpBlowMaceChest(void);

void YampiDesert_DropScoopGem(void);

void YampiDesert_TalkToElder(void);

void YampiDesert_ReadSign(void);

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

void YampiDesert_RevealPsynergyStone(void);

void YampiDesert_ShowBlocks(void);

void YampiDesert_ClimbRevealedLadder(void);
void YampiDesert_ClimbDownSpringCaveLadder(void);

extern u32 gYampiDesert_TravelerLooksAround[];

extern u32 gYampiDesert_OtherTravelerLooksAround[];

s32 MapGrid_GetTerrainKind(s32 layer, s32 x, s32 z);

extern struct SceneEntrance gSceneEntrances[];
extern struct SceneEntrance gYampiDesertSpringCaveEntrances[];
extern s16 gYampiDesertScorpionPitExitRegions[];
extern s16 gYampiDesertSpringCaveExitRegions[];

/// Talking to Blitz, the Jupiter Djinni at the top of the cliff in its room,
/// starts its battle.
void YampiDesert_FightBlitz(void) {
  Field_RequestDjinnBattle(ACTOR_BLITZ, DJINNI_BLITZ);
}

/// Where the party can enter: the cave under the spring has its own entrances.
const u8 *Scene_GetEntrances(void) {
  if (gMainRoutineState.next == SCENE_YAMPI_DESERT_SPRING_CAVE)
    return gYampiDesertSpringCaveEntrances;
  return gSceneEntrances;
}

/// The regions of the rooms whose exits are boxes: the scorpion pit and the
/// cave under the spring.
const u8 *Scene_GetExitRegions(void) {
  s16 scene = gMainRoutineState.next;

  if (scene == SCENE_YAMPI_DESERT_SCORPION_PIT)
    return gYampiDesertScorpionPitExitRegions;
  if (scene == SCENE_YAMPI_DESERT_SPRING_CAVE)
    return gYampiDesertSpringCaveExitRegions;
  return 0;
}

const u32 *Scene_GetExits(void) { return gSceneExits; }

extern struct ActorPlacement gYampiDesert_Placements[];
extern struct ActorPlacement gYampiDesertGuardianRingChestActorPlacements[];
extern struct ActorPlacement gYampiDesertSpringActorPlacements[];
extern struct ActorPlacement gYampiDesertSpringActorPlacementsAfterMeeting[];
extern struct ActorPlacement gYampiDesertBlitzActorPlacements[];
extern struct ActorPlacement gYampiDesertPsynergyStoneActorPlacements[];
extern struct ActorPlacement gYampiDesertKingScorpionActorPlacements[];
extern struct ActorPlacement gYampiDesertTrainersWhipChestActorPlacements[];
extern struct ActorPlacement gYampiDesertScorpionPitActorPlacements[];
extern struct ActorPlacement gYampiDesertBlowMaceChestActorPlacements[];

/// The current room's actors; in the spring's room, the travelers stand about
/// the spring once the party has seen them.
const struct ActorPlacement *Scene_GetActorPlacements(void) {
  s16 scene = gMainRoutineState.next;

  if (scene == SCENE_YAMPI_DESERT_GUARDIAN_RING_CHEST)
    return gYampiDesertGuardianRingChestActorPlacements;
  if (scene == SCENE_YAMPI_DESERT_SPRING) {
    if (GameFlag_Get(FLAG_YAMPI_DESERT_TRAVELERS_MET))
      return gYampiDesertSpringActorPlacementsAfterMeeting;
    return gYampiDesertSpringActorPlacements;
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

/// Scoop on the sand puddle on the east side of the room north of the spring
/// (cell (24, 23)) digs up 315 coins: "Felix got 315 coins."
void YampiDesert_DigUpCoins(s32 mode) {
  YampiDesert_DigUpTreasure(mode, ACTOR_HIDDEN_TREASURE,
                            FLAG_YAMPI_DESERT_COINS_PUDDLE_DUG_UP);
}

extern struct Trigger gYampiDesert_Triggers[];
extern struct Trigger gYampiDesertGuardianRingChestTriggers[];
extern struct Trigger gYampiDesertSpringTriggers[];
extern struct Trigger gYampiDesertBlitzTriggers[];
extern struct Trigger gYampiDesertPsynergyStoneTriggers[];
extern struct Trigger gYampiDesertKingScorpionTriggers[];
extern struct Trigger gYampiDesertCoinsPuddleTriggers[];
extern struct Trigger gYampiDesertSandPassTriggers[];
extern struct Trigger gYampiDesertTrainersWhipChestTriggers[];
extern struct Trigger gYampiDesertHiddenPsynergyStoneTriggers[];
extern struct Trigger gYampiDesertScorpionPitTriggers[];
extern struct Trigger gYampiDesertSpringCaveTriggers[];
extern struct Trigger gYampiDesertBlowMaceChestTriggers[];

/// The current room's triggers.
const struct Trigger *Scene_GetTriggers(void) {
  s16 scene = gMainRoutineState.next;

  if (scene == SCENE_YAMPI_DESERT_GUARDIAN_RING_CHEST)
    return gYampiDesertGuardianRingChestTriggers;
  if (scene == SCENE_YAMPI_DESERT_SPRING)
    return gYampiDesertSpringTriggers;
  if (scene == SCENE_YAMPI_DESERT_BLITZ)
    return gYampiDesertBlitzTriggers;
  if (scene == SCENE_YAMPI_DESERT_PSYNERGY_STONE)
    return gYampiDesertPsynergyStoneTriggers;
  if (scene == SCENE_YAMPI_DESERT_KING_SCORPION)
    return gYampiDesertKingScorpionTriggers;
  if (scene == SCENE_YAMPI_DESERT_COINS_PUDDLE)
    return gYampiDesertCoinsPuddleTriggers;
  if (scene == SCENE_YAMPI_DESERT_SAND_PASS)
    return gYampiDesertSandPassTriggers;
  if (scene == SCENE_YAMPI_DESERT_TRAINERS_WHIP_CHEST)
    return gYampiDesertTrainersWhipChestTriggers;
  if (scene == SCENE_YAMPI_DESERT_HIDDEN_PSYNERGY_STONE)
    return gYampiDesertHiddenPsynergyStoneTriggers;
  if (scene == SCENE_YAMPI_DESERT_SCORPION_PIT)
    return gYampiDesertScorpionPitTriggers;
  if (scene == SCENE_YAMPI_DESERT_SPRING_CAVE)
    return gYampiDesertSpringCaveTriggers;
  if (scene == SCENE_YAMPI_DESERT_BLOW_MACE_CHEST)
    return gYampiDesertBlowMaceChestTriggers;
  return gYampiDesert_Triggers;
}

/// The traveler who followed the pirates from Madra asks whether the party is
/// going to Alhafra too.
void YampiDesert_TalkToTraveler(void) {
  Event_Start();
  Event_EndLastingPsynergy(0);
  Event_SetMessageId((s32)MsgWeFollowedThe);
  // "We followed the pirates here from Madra... Are you going to Alhafra,
  // too?" Yes: "I don't think you can get there through the desert..." No:
  // "You guys are lucky. We've gotta find some way through this desert."
  Event_AskYesNo(ACTOR_TRAVELER_FOLLOWING_THE_PIRATES, 0);
  Event_End();
}

/// One traveler asks to forget about Briggs, and the elder refuses: there must
/// be a way through the desert.
void YampiDesert_ElderRefusesToQuit(void) {
  Event_SetMessageId((s32)MsgCanTWe);
  Actor_SetAnimationById(ACTOR_TRAVELER_WHO_WOULD_TURN_BACK, ANIM_SHAKE_HEAD);
  // "Can't we just forget about Briggs?"
  Event_ShowMessageAndWait(ACTOR_TRAVELER_WHO_WOULD_TURN_BACK, 0);
  Actor_FaceDirection(ACTOR_ELDER, FACING_WEST, 20);
  Actor_SetAnimationById(ACTOR_ELDER, ANIM_SHAKE_HEAD);
  // "No. I'm not about to quit looking for him."
  Event_ShowMessageAndWait(ACTOR_ELDER, 0);
  Actor_FaceDirectionById(ACTOR_ELDER, FACING_EAST, 20);
  Actor_FaceDirection(ACTOR_ELDER, FACING_SOUTH_SOUTHEAST, 20);
  Actor_FaceDirectionById(ACTOR_ELDER, FACING_EAST, 20);
  Actor_FaceDirection(ACTOR_ELDER, FACING_SOUTH_SOUTHEAST, 0);
  // "There must be some way through this desert! We must press onward!"
  Event_ShowMessageAndWait(ACTOR_ELDER, 0);
}

/// A traveler tells how insistent the elder is on finding a way through, and
/// the elder refuses to quit.
void YampiDesert_TalkToElderAboutWay(void) {
  Event_Start();
  Event_EndLastingPsynergy(0);
  if (!Event_GetFlag(FLAG_YAMPI_DESERT_ELDER_INSISTED))
    YampiDesert_ElderRefusesToQuit();
  Actor_FaceActorById(ACTOR_TRAVELER_WHO_WOULD_TURN_BACK,
                      gMainRoutineState.leaderActorId, 0);
  Event_SetMessageId((s32)MsgAsYouCan);
  // "As you can see, the elder is quite insistent on finding a way..."
  Event_ShowMessageAndWait(ACTOR_TRAVELER_WHO_WOULD_TURN_BACK, 0);
  GameFlag_Set(FLAG_YAMPI_DESERT_ELDER_INSISTED);
  Event_End();
}

/// The elder says this is where his son must prove himself as mayor.
void YampiDesert_TalkToElder(void) {
  Event_Start();
  Event_EndLastingPsynergy(0);
  if (!Event_GetFlag(FLAG_YAMPI_DESERT_ELDER_INSISTED))
    YampiDesert_ElderRefusesToQuit();
  Actor_FaceActorById(ACTOR_ELDER, gMainRoutineState.leaderActorId, 0);
  Event_SetMessageId((s32)MsgThisIsWhere);
  // "This is where my son must prove himself as mayor... So, Son. How are..."
  Event_ShowMessageAndWait(ACTOR_ELDER, 0);
  GameFlag_Set(FLAG_YAMPI_DESERT_ELDER_INSISTED);
  Event_End();
}

/// The sign in the Trainer's Whip chest's room reads: north to Alhafra, south
/// to Air's Rock, with a warning about what lies beyond.
void YampiDesert_ReadSign(void) {
  Event_Start();
  Event_EndLastingPsynergy(0);
  // "North: Alhafra South: Air's Rock Warning: Beyond this point, there..."
  FieldMessage_Show((s32)MsgNorthAlhafraSouth, 1);
  Event_End();
}

/// Pound on one of the wooden stakes of the desert's first room hammers it down
/// into the sand; the one before the Guardian Ring's chest in the south-east
/// corner opens the way to it.
void YampiDesert_PoundGuardianRingChestStake(s32 mode, s32 count) {
  Pound_SinkTarget(gYampiDesertGuardianRingChestStakes, mode, count);
}

/// Pound on one of the wooden stakes in Blitz's room hammers it down into the
/// sand. The last one stands before the climbable wall that leads up to Blitz;
/// the one below the Antidote's chest is best left standing, as its top is the
/// way across.
void YampiDesert_PoundBlitzStake(s32 mode, s32 count) {
  Pound_SinkTarget(gYampiDesertBlitzStakes, mode, count);
}

/// Defines what Pound on one of a room's stakes does: it sinks
/// (Pound_SinkTarget), and once down it raises the watch for whether the King
/// Scorpion is stopped.
#define DEFINE_POUND_STAKE_STOPPING_SCORPION(name, stakes)                     \
  void name(s32 mode, s32 count) {                                             \
    Pound_SinkTarget(stakes, mode, count);                                     \
    if (mode == 1) {                                                           \
      if (gYampiDesert_RaisedEvent == 0)                                       \
        gYampiDesert_RaisedEvent = RAISED_WATCH_SCORPION_STOP;                 \
    }                                                                          \
  }

/// Pound on one of the wooden stakes in the Psynergy Stone's room hammers it
/// down into the sand. While the King Scorpion still roams under the sand,
/// the camera then follows the ripple of sand over it until it stops.
DEFINE_POUND_STAKE_STOPPING_SCORPION(YampiDesert_PoundPsynergyStoneStake,
                                     gYampiDesertPsynergyStoneStakes)

/// Pound on the wooden stake that blocks the way to the Blow Mace's chest, in
/// the cave of sand slides, hammers it down into the sand.
void YampiDesert_PoundBlowMaceChestStake(s32 mode, s32 count) {
  Pound_SinkTarget(gYampiDesertBlowMaceChestStakes, mode, count);
}

/// Pound on one of the wooden stakes in the King Scorpion's room hammers it
/// down into the sand. Until the King Scorpion is cornered, the camera then
/// follows the ripple of sand over it until it stops.
DEFINE_POUND_STAKE_STOPPING_SCORPION(YampiDesert_PoundKingScorpionStake,
                                     gYampiDesertKingScorpionStakes)

/// Nothing visible happens when Felix steps from the high ground onto the
/// cells beside the stakes in Blitz's room, the Psynergy Stone's or the King
/// Scorpion's; the game makes the tops of the stakes still standing into
/// squares he hops across at his height, so that a row of standing stakes is a
/// way over the sand.
/// Each stake's cell gives up its wall and takes the hop flag, and its height
/// rises by four steps.
void YampiDesert_StepOntoStakeTops(void) {
  struct MapState *map = Ram_HeapSlots->mapGrid;
  u16 *list = NULL;
  u8 *grid = gMapShapeGrid;

  if (gMainRoutineState.next == SCENE_YAMPI_DESERT_BLITZ)
    list = gYampiDesertBlitzStakes;
  else if (gMainRoutineState.next == SCENE_YAMPI_DESERT_PSYNERGY_STONE)
    list = gYampiDesertPsynergyStoneStakes;
  else if (gMainRoutineState.next == SCENE_YAMPI_DESERT_KING_SCORPION)
    list = gYampiDesertKingScorpionStakes;
  if (list == NULL)
    return;
  while (*list != ACTOR_LIST_END) {
    struct Actor *actor = Actor_GetById(*list);

    list += 2;
    if (!GameFlag_Get(*list)) {
      s32 layer = actor->layer;
      struct MapCell *cells = map->layers[layer].cells;
      u8 *shapes =
          (u8 *)((((s32)cells - (s32)Ram_MapCellBuffer) >> 2) + (s32)grid);
      s32 index = Actor_GetCellIndex(actor);
      s32 shape = MapGrid_GetTerrainKind(layer, actor->x.fixed, actor->z.fixed);
      s32 height = CELL_HEIGHT_STEPS(
          Map_GetTerrainHeight(layer, actor->x.fixed, actor->z.fixed));

      cells += index;
      shapes += index;
      actor->motionFlags = 0;
      cells->collisionCode = CELL_BEHAVIOR_NORMAL;
      cells->attributeB |= MAP_CELL_JUMP;
      *shapes = MapCollision_FindOrAddTag(layer, shape, height + 4);
    }
    list++;
  }
  GameFlag_Set(FLAG_YAMPI_DESERT_ON_STAKE_TOPS);
}

/// Nothing visible happens when Felix steps back down off the stakes' tops; the
/// game makes the standing stakes walls again at ground level.
void YampiDesert_StepOffStakeTops(void) {
  struct MapState *map = Ram_HeapSlots->mapGrid;
  u16 *list = NULL;
  u8 *grid = gMapShapeGrid;

  if (gMainRoutineState.next == SCENE_YAMPI_DESERT_BLITZ)
    list = gYampiDesertBlitzStakes;
  else if (gMainRoutineState.next == SCENE_YAMPI_DESERT_PSYNERGY_STONE)
    list = gYampiDesertPsynergyStoneStakes;
  else if (gMainRoutineState.next == SCENE_YAMPI_DESERT_KING_SCORPION)
    list = gYampiDesertKingScorpionStakes;
  if (list == NULL)
    return;
  for (; *list != ACTOR_LIST_END; list++) {
    struct Actor *actor = Actor_GetById(*list);

    list += 2;
    if (!GameFlag_Get(*list)) {
      s32 layer = actor->layer;
      struct MapCell *cells = map->layers[layer].cells;
      u8 *shapes =
          (u8 *)((((s32)cells - (s32)Ram_MapCellBuffer) >> 2) + (s32)grid);
      s32 index = Actor_GetCellIndex(actor);
      s32 shape = MapGrid_GetTerrainKind(layer, actor->x.fixed, actor->z.fixed);
      s32 height = CELL_HEIGHT_STEPS(
          Map_GetTerrainHeight(layer, actor->x.fixed, actor->z.fixed));

      cells += index;
      shapes += index;
      actor->motionFlags = 0;
      cells->collisionCode = CELL_BEHAVIOR_WALL;
      cells->attributeB &= ~MAP_CELL_JUMP;
      *shapes = MapCollision_FindOrAddTag(layer, shape, height - 4);
    }
  }
  GameFlag_Clear(FLAG_YAMPI_DESERT_ON_STAKE_TOPS);
}

/// As Sand is cast, walls off the cell of each stake Pound has sunk, in the
/// overhead layer Sand moves on.
void YampiDesert_BlockPoundedStakes(u16 *list) {
  struct MapState *map = Ram_HeapSlots->mapGrid;

  for (; *list != ACTOR_LIST_END; list += 3) {
    struct Actor *actor = Actor_GetById(list[0]);

    if (GameFlag_Get(list[2])) {
      struct MapCell *cell =
          &map->layers[MAPGRID_LAYER_OVERHEAD].cells[Actor_GetCellIndex(actor)];

      cell->collisionCode = CELL_BEHAVIOR_WALL;
    }
  }
}

/// As Sand ends, makes each stake's cell in the overhead layer sand again.
void YampiDesert_OpenStakeCells(u16 *list) {
  struct MapState *map = Ram_HeapSlots->mapGrid;

  for (; *list != ACTOR_LIST_END; list += 3) {
    struct MapCell *cell =
        &map->layers[MAPGRID_LAYER_OVERHEAD]
             .cells[Actor_GetCellIndex(Actor_GetById(list[0]))];

    cell->collisionCode = CELL_BEHAVIOR_SAND;
  }
}

/// As Sand melts Felix into the sand, so that he can travel under it past
/// rocks and standing stakes, the stakes Pound has hammered down still bar his
/// way underground.
/// He moves on the map's top layer while sunk, and each sunk stake's cell
/// there is made a wall.
void YampiDesert_CastSand(void) {
  s16 scene;

  Actor_GetById(gMainRoutineState.leaderActorId)->layer =
      MAPGRID_LAYER_OVERHEAD;
  scene = gMainRoutineState.next;
  if (scene == SCENE_YAMPI_DESERT_GUARDIAN_RING_CHEST)
    YampiDesert_BlockPoundedStakes(gYampiDesertGuardianRingChestStakes);
  else if (scene == SCENE_YAMPI_DESERT_BLITZ)
    YampiDesert_BlockPoundedStakes(gYampiDesertBlitzStakes);
  else if (scene == SCENE_YAMPI_DESERT_PSYNERGY_STONE)
    YampiDesert_BlockPoundedStakes(gYampiDesertPsynergyStoneStakes);
  else if (scene == SCENE_YAMPI_DESERT_KING_SCORPION)
    YampiDesert_BlockPoundedStakes(gYampiDesertKingScorpionStakes);
}

/// As Sand ends and Felix rises out of the sand, the sunk stakes' cells are
/// sand again under him.
void YampiDesert_EndSand(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineState.leaderActorId);
  s16 scene;

  leader->layer = MAPGRID_LAYER_GROUND;
  scene = gMainRoutineState.next;
  if (scene == SCENE_YAMPI_DESERT_GUARDIAN_RING_CHEST)
    YampiDesert_OpenStakeCells(gYampiDesertGuardianRingChestStakes);
  else if (scene == SCENE_YAMPI_DESERT_BLITZ)
    YampiDesert_OpenStakeCells(gYampiDesertBlitzStakes);
  else if (scene == SCENE_YAMPI_DESERT_PSYNERGY_STONE)
    YampiDesert_OpenStakeCells(gYampiDesertPsynergyStoneStakes);
  else if (scene == SCENE_YAMPI_DESERT_KING_SCORPION)
    YampiDesert_OpenStakeCells(gYampiDesertKingScorpionStakes);
}

/// After a stake is Pounded while the King Scorpion roams under the sand,
/// the camera follows the ripple of sand over it until it stops, then
/// returns to Felix.
void YampiDesert_WatchScorpionStop(void) {
  s16 scene;

  Event_Start();
  Event_EndLastingPsynergy(0);
  scene = gMainRoutineState.next;
  if (scene == SCENE_YAMPI_DESERT_PSYNERGY_STONE) {
    FieldCamera_FollowActor(ACTOR_PSYNERGY_STONE_ROOM_KING_SCORPION, 1);
    Actor_WatchAndSetScript(gMainRoutineState.leaderActorId,
                            ACTOR_PSYNERGY_STONE_ROOM_KING_SCORPION);
  } else if (scene == SCENE_YAMPI_DESERT_KING_SCORPION) {
    FieldCamera_FollowActor(ACTOR_KING_SCORPION_ROOM_KING_SCORPION, 1);
    Actor_WatchAndSetScript(gMainRoutineState.leaderActorId,
                            ACTOR_KING_SCORPION_ROOM_KING_SCORPION);
  }
  while (gYampiDesert_RaisedEvent == RAISED_WATCH_SCORPION_STOP)
    wait(1);
  Event_End();
}

#include "../field/field_effect_advance_motion_priority.c"

#undef EFFECT_SET_MODE
#undef EFFECT_SET_SCRIPT
#undef DEFINE_POUND_STAKE_STOPPING_SCORPION

void FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                       s32 velocityZ, u32 flags,
                       const struct EffectOptions *options);

/// The grains of sand in a burst.
#define SAND_BURST_GRAINS 17

/// A burst of sand at (x, y, z): its grains thrown east, south, west and north
/// in turn, each a little off at random.
void YampiDesert_BurstSand(s32 x, s32 y, s32 z) {
  struct EffectOptions options;
  u32 i;

  options.update = FieldEffect_AdvanceMotionAtLeaderPriority;
  options.startScaleX = FIXED_ONE * 8 / 10;
  options.startScaleY = FIXED_ONE * 8 / 10;
  for (i = 0; i < SAND_BURST_GRAINS; i++) {
    s32 velocity[3];

    velocity[0] = Trig_Cos(i * DEGtoANG(90));
    velocity[1] = 0;
    velocity[2] = Trig_Sin(i * DEGtoANG(90));
    velocity[0] += velocity[0] >> 1;
    velocity[0] += (u16)(Random16() << 1) - toFIXED(1);
    velocity[2] += (u16)Random16() - toFIXED(1) / 2;
    FieldEffect_Spawn(x, y, z, velocity[0], velocity[1], velocity[2],
                      EFFECT_USE_UPDATE | EFFECT_SCRIPT_MEDIUM, &options);
  }
}

/// When the King Scorpion, roaming under the sand of the Psynergy Stone's room,
/// stops against the stake Pound has hammered down, it bursts out of the sand
/// with a spray, scuttles east and burrows away into the next room.
void YampiDesert_StirScorpion(void) {
  Event_Start();
  Event_EndLastingPsynergy(0);
  YampiDesert_ClearRaisedEvent();
  Actor_WatchAndSetScript(gMainRoutineState.leaderActorId,
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
                       SPRITE_COLOR_NORMAL);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_PSYNERGY_STONE_ROOM_KING_SCORPION),
                       SPRITE_FLAG_SHADOW);
  Actor_MoveToAndWaitById(ACTOR_PSYNERGY_STONE_ROOM_KING_SCORPION, 380, 248);
  Actor_Stop(gMainRoutineState.leaderActorId);
  Sound_Play(SE_WATER_MOVE);
  Actor_SetSpeed(ACTOR_PSYNERGY_STONE_ROOM_KING_SCORPION, 0.8);
  Actor_WalkToAndWaitById(ACTOR_PSYNERGY_STONE_ROOM_KING_SCORPION, 428, 248);
  Sound_Play(SOUND_FADE_OUT_ACTION_SE);
  Actor_SetPosition(ACTOR_PSYNERGY_STONE_ROOM_KING_SCORPION, 0, 0);
  GameFlag_Set(FLAG_YAMPI_DESERT_SCORPION_STIRRED);
  Event_End();
}

/// When the King Scorpion, roaming under the sand of the south part of its
/// own room, stops against the stake Pound has hammered down, it bursts out of
/// the sand with a spray and scuttles north, burrowing away towards the trap.
void YampiDesert_StirScorpionAgain(void) {
  Event_Start();
  Event_EndLastingPsynergy(0);
  YampiDesert_ClearRaisedEvent();
  Actor_WatchAndSetScript(gMainRoutineState.leaderActorId,
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
  YampiDesert_BurstSand(toFIXED(248), 0, toFIXED(576));
  Sound_Play(SE_KOLIMA_BRIDGE_HOP);
  Actor_GetById(ACTOR_KING_SCORPION_ROOM_KING_SCORPION)->velocityY = toFIXED(6);
  Actor_SetSpriteColor(Actor_GetById(ACTOR_KING_SCORPION_ROOM_KING_SCORPION),
                       SPRITE_COLOR_NORMAL);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_KING_SCORPION_ROOM_KING_SCORPION),
                       SPRITE_FLAG_SHADOW);
  Actor_MoveToAndWaitById(ACTOR_KING_SCORPION_ROOM_KING_SCORPION, 256, 560);
  FieldEvent_Wait(20);
  Actor_Stop(gMainRoutineState.leaderActorId);
  Sound_Play(SE_WATER_MOVE);
  Actor_SetSpeed(ACTOR_KING_SCORPION_ROOM_KING_SCORPION, 0.8);
  Actor_WalkToAndWait(ACTOR_KING_SCORPION_ROOM_KING_SCORPION, 248, 496);
  Sound_Play(SOUND_FADE_OUT_ACTION_SE);
  Actor_SetPosition(ACTOR_KING_SCORPION_ROOM_KING_SCORPION, 0, 0);
  GameFlag_Set(FLAG_YAMPI_DESERT_SCORPION_STIRRED_AGAIN);
  Event_End();
}

/// When the stakes of the trap have shut the King Scorpion's way, the camera
/// follows its ripple of sand north into the ring of rocks at the north end of
/// its room, where it stops, cornered.
void YampiDesert_CornerScorpion(void) {
  Event_Start();
  Event_EndLastingPsynergy(0);
  YampiDesert_ClearRaisedEvent();
  Actor_WatchAndSetScript(gMainRoutineState.leaderActorId,
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
  Actor_Stop(gMainRoutineState.leaderActorId);
  FieldEvent_Wait(20);
  GameFlag_Set(FLAG_YAMPI_DESERT_SCORPION_CORNERED);
  Event_End();
}

void Actor_StartScript(struct Actor *actor, const void *script);

/// Raises a cloud of dust twice at each point of
/// gYampiDesert_SandCloudPoints, each moved by -2 to +1 pixels at random.
void YampiDesert_RaiseSandCloud(void) {
  u32 pass;
  u32 i;

  for (pass = 0; pass <= 1; pass++) {
    u16 *point;

    point = gYampiDesert_SandCloudPoints;
    for (i = 0; i < SAND_CLOUD_POINTS; i++) {
      s32 x = *point++ + ((Random16() << 2) >> 16);
      s32 z = *point++ + ((Random16() << 2) >> 16);
      struct Actor *actor = Import_ActorCreate(
          SPRITE_DUST_CLOUD, INTtoFIXED(x - 2), 0, INTtoFIXED(z - 2));

      if (actor != NULL) {
        actor->speedLimit = toFIXED(9) / 8;
        actor->acceleration = toFIXED(1) / 2;
        Actor_SetSpriteFlags((struct Actor *)actor, 0);
        Actor_SelectAnimation((struct Actor *)actor, 1);
        Actor_StartScript(actor, gYampiDesert_SandCloudDrifts);
      }
    }
  }
  MapGrid_CopyCellTilesIndicesRect(74, 71, 77, 9, 3, 3);
}

/// Walking up to the cornered King Scorpion in the ring of rocks at the north
/// of its room (cell (14, 13)): its ripple of sand pulses, Felix stops with a
/// question mark, and the scorpion bursts out before him and uses Scoop. Felix
/// staggers back, raises his arms and sinks into the hole with it, and they
/// drop into its pit below, where its battle begins.
void YampiDesert_TrapKingScorpion(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineState.leaderActorId);
  struct Actor *scorpion =
      Actor_GetById(ACTOR_KING_SCORPION_ROOM_KING_SCORPION);
  u32 i;

  if (!GameFlag_Get(FLAG_YAMPI_DESERT_SCORPION_CORNERED))
    return;
  Event_Start();
  Event_EndLastingPsynergy(0);
  do {
    gYampiDesert_SandRipple->scaleX -= FIXED_ONE * 8 / 100;
    gYampiDesert_SandRipple->scaleY -= FIXED_ONE * 8 / 100;
    wait(1);
  } while (gYampiDesert_SandRipple->scaleX >= FIXED_ONE * 8 / 10);
  do {
    gYampiDesert_SandRipple->scaleX += FIXED_ONE / 10;
    gYampiDesert_SandRipple->scaleY += FIXED_ONE / 10;
    wait(1);
  } while (gYampiDesert_SandRipple->scaleX <= FIXED_ONE * 18 / 10);
  do {
    gYampiDesert_SandRipple->scaleX -= FIXED_ONE * 4 / 100;
    gYampiDesert_SandRipple->scaleY -= FIXED_ONE * 4 / 100;
    wait(1);
  } while (gYampiDesert_SandRipple->scaleX >= FIXED_ONE * 2 / 10);
  Actor_SetPosition(ACTOR_KING_SCORPION_ROOM_SAND_RIPPLE, 0, 0);
  FieldEvent_Wait(20);
  FieldCamera_MoveTo(232, CAMERA_KEEP, 196, 1);
  Actor_ShowEmoteById(gMainRoutineState.leaderActorId,
                      EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_SetSpeed(gMainRoutineState.leaderActorId, 0.8);
  Actor_WalkToAndWaitById(gMainRoutineState.leaderActorId, 232, 164);
  FieldEvent_Wait(20);
  YampiDesert_SetPositionAndFacing(ACTOR_KING_SCORPION_ROOM_KING_SCORPION,
                                   toFIXED(232), toFIXED(184), FACING_SOUTH);
  wait(1);
  YampiDesert_BurstSand(toFIXED(232), 0, toFIXED(184));
  Sound_Play(SE_KOLIMA_BRIDGE_HOP);
  scorpion->velocityY = toFIXED(6);
  scorpion->priorityFlags &= ~ACTOR_PRIORITY_UNDERFOOT;
  Actor_SetSpritePriority(ACTOR_KING_SCORPION_ROOM_KING_SCORPION, 2);
  Actor_SetSpriteColor(Actor_GetById(ACTOR_KING_SCORPION_ROOM_KING_SCORPION),
                       SPRITE_COLOR_NORMAL);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_KING_SCORPION_ROOM_KING_SCORPION),
                       SPRITE_FLAG_SHADOW);
  Actor_SetSpeed(ACTOR_KING_SCORPION_ROOM_KING_SCORPION, 1.6);
  Actor_MoveToAndWaitById(ACTOR_KING_SCORPION_ROOM_KING_SCORPION, 216, 184);
  Actor_FaceDirectionById(gMainRoutineState.leaderActorId, FACING_SOUTH, 0);
  Actor_FaceDirectionAndRefreshById(ACTOR_KING_SCORPION_ROOM_KING_SCORPION,
                                    FACING_EAST);
  FieldEvent_Wait(20);
  YampiDesert_ScorpionUsesScoop();
  Sound_Play(SE_WHOOSH);
  YampiDesert_RaiseSandCloud();
  Actor_SetSpriteFlags(Actor_GetById(gMainRoutineState.leaderActorId), 0);
  FieldEvent_Wait(20);
  FieldEffect_SetAttached(gMainRoutineState.leaderActorId,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAnimationById(gMainRoutineState.leaderActorId,
                         ANIM_LEADER_WALK_BACKWARDS);
  FieldEvent_Wait(30);
  Actor_SetAnimationById(gMainRoutineState.leaderActorId,
                         ANIM_LEADER_ARMS_RAISED);
  FieldEvent_Wait(10);
  Sound_Play(SE_DUST_PUFF);
  for (i = 0; i < 14; i++) {
    leader->scaleX -= FIXED_ONE / 64;
    leader->scaleY -= FIXED_ONE / 64;
    leader->y.fixed += -toFIXED(2);
    wait(1);
  }
  Actor_SetSpriteColor(Actor_GetById(gMainRoutineState.leaderActorId),
                       SPRITE_COLOR_HIDDEN);
  Sound_Play(SE_HIGH_JUMP);
  scorpion->velocityY = toFIXED(8);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_KING_SCORPION_ROOM_KING_SCORPION),
                       0);
  Actor_MoveToAndWaitById(ACTOR_KING_SCORPION_ROOM_KING_SCORPION, 232, 168);
  FieldEvent_Wait(3);
  Sound_Play(SE_DUST_PUFF);
  for (i = 0; i < 6; i++) {
    scorpion->scaleX -= FIXED_ONE / 32;
    scorpion->scaleY -= FIXED_ONE / 32;
    scorpion->y.fixed += -toFIXED(7);
    wait(1);
  }
  Actor_SetPosition(ACTOR_KING_SCORPION_ROOM_KING_SCORPION, 0, 0);
  gFieldState->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Import_EventSetExitRequest(
      YAMPI_DESERT_KING_SCORPION_TO_YAMPI_DESERT_SCORPION_PIT);
}

/// Scoop on the sand between the four rocks in the middle of the King
/// Scorpion's pit (cell (14, 12)) opens a geyser there, a column of water that
/// rises and falls and carries the party up and out of the pit.
void YampiDesert_OpenGeyser(s32 mode) {
  struct Actor *actor;
  s32 height;

  if (mode == 1) {
    actor = Actor_GetById(ACTOR_GEYSER);
    MapGrid_CopyCellTilesIndicesRect(35, 39, 13, 11, 3, 3);
    Actor_SetPosition(ACTOR_GEYSER, 232, 200);
    height = Map_GetTerrainHeight(MAPGRID_LAYER_GROUND, actor->x.fixed,
                                  actor->z.fixed);
    actor->y.fixed = height;
    actor->groundHeight = height;
    GameFlag_Set(FLAG_YAMPI_DESERT_ROCK_PIT_DUG_UP);
  }
}

s32 YampiDesert_CellCenter(s32 cell) {
  return cell * MAPGRID_CELL_PIXELS + MAPGRID_CELL_PIXELS / 2;
}

/// Moves the leader to the centre of a cell, in pixels.
#define MOVE_LEADER_TO(column, row)                                            \
  {                                                                            \
    s32 x_ = YampiDesert_CellCenter(column);                                   \
    s32 z_ = YampiDesert_CellCenter(row);                                      \
    Actor_SetHorizontalPositionWithTerrain(gMainRoutineState.leaderActorId,    \
                                           INTtoFIXED(x_), INTtoFIXED(z_));    \
  }

/// Reveal cast in the long room of the Trainer's Whip's chest changes two
/// rings of rocks while it lasts: the first (cells (15, 40) to (19, 45)), three
/// rows of three rocks, shifts into an arrow of rocks pointing north-west (a
/// row along the top, a short column down its west end and a line running
/// down to the south-east from their corner), and the second (cells (36, 25)
/// to (40, 29)) shows the Lucky Medal's chest in its middle. If Felix stands
/// where a rock now appears, he is moved to the nearest open cell.
void YampiDesert_RevealRockHint(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineState.leaderActorId);
  struct Actor *actor = Actor_GetById(ACTOR_HIDDEN_TREASURE);
  s32 column = FIELD_CELL(leader->x.fixed);
  s32 row = FIELD_CELL(leader->z.fixed);

  if (actor != NULL)
    Actor_SetPosition(ACTOR_HIDDEN_TREASURE, 616, 444);
  MapGrid_CopyCellTilesIndicesRect(82, 82, 15, 40, 5, 6);
  MapGrid_CopyCellAttributes(82, 82, 5, 6, 15, 40);
  MapGrid_CopyCellTilesIndicesRect(88, 82, 36, 25, 5, 5);
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

#undef MOVE_LEADER_TO

/// Moves the leader to the centre of a cell, in pixels.
#define MOVE_LEADER_TO(column, row)                                            \
  {                                                                            \
    struct MainRoutineState *state = &gMainRoutineState;                       \
    s32 x = YampiDesert_CellCenter(column);                                    \
    s32 z = YampiDesert_CellCenter(row);                                       \
    Actor_SetHorizontalPositionWithTerrain(state->leaderActorId,               \
                                           INTtoFIXED(x), INTtoFIXED(z));      \
  }

/// As Reveal fades in the Trainer's Whip's chest's room, both rings of rocks
/// go back as they were and the Lucky Medal's chest is hidden again; Felix is
/// moved off any cell a rock returns to.
void YampiDesert_HideRockHint(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineState.leaderActorId);
  s32 column = FIELD_CELL(leader->x.fixed);
  s32 row = FIELD_CELL(leader->z.fixed);

  if (Actor_GetById(ACTOR_HIDDEN_TREASURE) != NULL)
    Actor_SetPosition(ACTOR_HIDDEN_TREASURE, 0, 0);
  MapGrid_CopyCellTilesIndicesRect(82, 89, 15, 40, 5, 6);
  MapGrid_CopyCellAttributes(82, 89, 5, 6, 15, 40);
  MapGrid_CopyCellTilesIndicesRect(88, 89, 36, 25, 5, 5);
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

/// Reveal cast in the room that leads on from the Trainer's Whip's chest's room
/// to the World Map brings out a hidden Psynergy Stone at cell (18, 15),
/// moving Felix a cell south if he stands on its spot. Once Reveal fades the
/// stone is gone again (YampiDesert_HidePsynergyStone), so it has to be
/// touched while Reveal lasts.
void YampiDesert_RevealPsynergyStone(void) {
  struct Actor *actor = Actor_GetById(ACTOR_HIDDEN_TREASURE);
  struct Actor *leader;

  if (actor != 0) {
    leader = Actor_GetById(gMainRoutineState.leaderActorId);
    Actor_SetPosition(ACTOR_HIDDEN_TREASURE, 288, 240);
    if (FIELD_CELL(leader->x.fixed + toFIXED(8)) ==
            FIELD_CELL(actor->x.fixed + toFIXED(8)) &&
        FIELD_CELL(leader->z.fixed + toFIXED(8)) ==
            FIELD_CELL(actor->z.fixed + toFIXED(8)))
      Actor_SetPosition(gMainRoutineState.leaderActorId, 288, 256);
  }
}

/// As Reveal fades, the hidden Psynergy Stone of the room that leads on to the
/// World Map is put away again.
void YampiDesert_HidePsynergyStone(void) {
  if (Actor_GetById(ACTOR_HIDDEN_TREASURE))
    Actor_SetPosition(ACTOR_HIDDEN_TREASURE, 0, 0);
}

/// Opens the hole over the ladder in the spring's sand puddle.
void YampiDesert_OpenLadderPit(void) {
  MapGrid_CopyCellTilesIndicesRect(64, 64, 72, 32, 1, 1);
  Map_CopyCellAttributeRect(64, 64, 1, 1, 8, 32);
  Map_CopyCellAttributeRect(64, 64, 1, 1, 8, 96);
}

/// Scoop on the oddly shaped sand puddle at the south of the travelers'
/// spring's room (cell (8, 32)) digs it out to show a ladder going down.
void YampiDesert_ScoopLadderPit(s32 mode) {
  if (mode == 1) {
    YampiDesert_OpenLadderPit();
    GameFlag_Set(FLAG_YAMPI_DESERT_LADDER_DUG_UP);
  }
}

/// Shows or hides the unseen blocks of the Trainer's Whip chest's room or the
/// Blow Mace chest's.
void YampiDesert_SetBlocksShown(s32 show) {
  s16 scene = gMainRoutineState.next;

  if (scene == SCENE_YAMPI_DESERT_TRAINERS_WHIP_CHEST)
    Actors_SetDrawn(show, ACTOR_WHIP_CHEST_ROOM_FIRST_BLOCK, 2);
  else if (scene == SCENE_YAMPI_DESERT_BLOW_MACE_CHEST)
    Actors_SetDrawn(show, ACTOR_BLOW_MACE_CHEST_ROOM_FIRST_BLOCK,
                    ACTOR_BLOW_MACE_CHEST_ROOM_LAST_BLOCK -
                        ACTOR_BLOW_MACE_CHEST_ROOM_FIRST_BLOCK + 1);
}

/// Nothing visible happens when Felix steps onto the sand round the corner of
/// the flowing sand in the Trainer's Whip's chest's room (cells (48, 23) to
/// (51, 23) and (48, 24) to (48, 26)) after the inner corner has hidden the two
/// blocks at cells (51, 26) and (51, 27): the game marks them placed again.
/// The room's set-up hid both with Actor_Hide, see-through and without
/// collision, so placed or not they stop nothing and show nothing.
void YampiDesert_ShowBlocks(void) {
  YampiDesert_SetBlocksShown(1);
  GameFlag_Set(FLAG_YAMPI_DESERT_BLOCKS_SHOWN);
}

/// Nothing visible happens when Felix steps onto the inner corner of the
/// flowing sand in the Trainer's Whip's chest's room (cells (49, 24) to
/// (51, 24) and (49, 25)); the game takes the two invisible blocks at cells
/// (51, 26) and (51, 27) away.
void YampiDesert_HideBlocks(void) {
  YampiDesert_SetBlocksShown(0);
  GameFlag_Clear(FLAG_YAMPI_DESERT_BLOCKS_SHOWN);
}

#include "shared/actors_set_hidden.c"

extern u32 gYampiDesert_MayorRunsAhead[];
extern u32 gYampiDesert_TravelersFollow[];
extern u32 gYampiDesert_LastTravelersFollow[];

/// Stepping out onto the ledge above the travelers' spring (cells (15, 12)
/// and (16, 12)), the party is seen from below: "Hey! Look! Up there!" The
/// travelers wonder how they got up there, the mayor finds the way, and they
/// all run off along it.
void YampiDesert_TravelersLookUp(s32 p0) {
  struct MainRoutineState *state;

  Call1((void (*)())Event_Start, p0);
  Event_EndLastingPsynergy(0);
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
  state = &gMainRoutineState;
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
  Actor_SetAnimationAndWait(ACTOR_ELDER, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_MAYOR, ANIM_NOD);
  Actor_WatchAndSetScript(ACTOR_TRAVELER_FOLLOWING_THE_PIRATES, ACTOR_MAYOR);
  Actor_WatchAndSetScript(ACTOR_TRAVELER_SEEKING_EVIDENCE, ACTOR_MAYOR);
  Actor_WatchAndSetScript(ACTOR_DIZZY_TRAVELER, ACTOR_MAYOR);
  Actor_WatchAndSetScript(ACTOR_PARCHED_TRAVELER, ACTOR_MAYOR);
  Actor_WatchAndSetScript(ACTOR_TRAVELER_WHO_WOULD_TURN_BACK, ACTOR_MAYOR);
  Actor_WatchAndSetScript(ACTOR_ELDER, ACTOR_MAYOR);
  Actor_SetSpeed(ACTOR_ELDER, 1);
  Actor_SetMovementTypeById(ACTOR_MAYOR, (s32)gYampiDesert_MayorRunsAhead);
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
  Actor_SetAnimationAndWait(ACTOR_TRAVELER_FOLLOWING_THE_PIRATES, ANIM_NOD);
  // "We can find a way through to Alhafra!"
  Event_ShowMessageAndWait(ACTOR_TRAVELER_FOLLOWING_THE_PIRATES, 0);
  Actor_Jump(ACTOR_DIZZY_TRAVELER, 4, 0);
  Actor_FaceDirection(ACTOR_DIZZY_TRAVELER, FACING_EAST, 0);
  Actor_SetAnimationById(ACTOR_DIZZY_TRAVELER, ANIM_SHAKE_HEAD);
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
  Actor_SetAnimationById(ACTOR_TRAVELER_FOLLOWING_THE_PIRATES, ANIM_NOD);
  Actor_SetAnimationById(ACTOR_TRAVELER_SEEKING_EVIDENCE, ANIM_NOD);
  Actor_SetAnimationById(ACTOR_DIZZY_TRAVELER, ANIM_NOD);
  Actor_SetAnimationById(ACTOR_PARCHED_TRAVELER, ANIM_NOD);
  Actor_SetAnimationById(ACTOR_TRAVELER_WHO_WOULD_TURN_BACK, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_ELDER, ANIM_NOD);
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
  YampiDesert_SetPositionAndFacing(ACTOR_TRAVELER_FOLLOWING_THE_PIRATES,
                                   toFIXED(520), toFIXED(242),
                                   FACING_NORTH_NORTHWEST);
  YampiDesert_SetPositionAndFacing(ACTOR_TRAVELER_SEEKING_EVIDENCE,
                                   toFIXED(533), toFIXED(271), FACING_WEST);
  YampiDesert_SetPositionAndFacing(ACTOR_DIZZY_TRAVELER, toFIXED(435),
                                   toFIXED(232), FACING_SOUTH_SOUTHEAST);
  YampiDesert_SetPositionAndFacing(ACTOR_PARCHED_TRAVELER, toFIXED(467),
                                   toFIXED(248), FACING_SOUTH_SOUTHWEST);
  YampiDesert_SetPositionAndFacing(ACTOR_TRAVELER_WHO_WOULD_TURN_BACK,
                                   toFIXED(462), toFIXED(340),
                                   FACING_NORTH_NORTHEAST);
  YampiDesert_SetPositionAndFacing(ACTOR_ELDER, toFIXED(482), toFIXED(340),
                                   FACING_NORTH_NORTHEAST);
  YampiDesert_SetPositionAndFacing(ACTOR_MAYOR, toFIXED(489), toFIXED(320),
                                   FACING_NORTH_NORTHWEST);
  Actor_SetAnimationById(ACTOR_TRAVELER_FOLLOWING_THE_PIRATES, ANIM_STAND);
  Actor_SetAnimationById(ACTOR_TRAVELER_SEEKING_EVIDENCE, ANIM_STAND);
  Actor_SetAnimationById(ACTOR_DIZZY_TRAVELER, ANIM_STAND);
  Actor_SetAnimationById(ACTOR_PARCHED_TRAVELER, ANIM_STAND);
  Actor_SetAnimationById(ACTOR_TRAVELER_WHO_WOULD_TURN_BACK, ANIM_STAND);
  Actor_SetAnimationById(ACTOR_ELDER, ANIM_STAND);
  Actor_SetAnimationById(ACTOR_MAYOR, ANIM_STAND);
  GameFlag_Set(FLAG_YAMPI_DESERT_TRAVELERS_MET);
  GameFlag_Set(FLAG_YAMPI_DESERT_ELDER_INSISTED);
  Event_End();
}

/// The Scoop Gem's icon as an actor, after the King Scorpion's battle.
struct Actor *YampiDesert_CreateScoopGem(void) {
  struct Actor *actor = Import_ActorCreate(SPRITE_UNSEEN_MARKER, toFIXED(232),
                                           toFIXED(32), toFIXED(200));
  GlyphTransfer *icon = NULL;
  struct YampiDesertVisual *visual;
  s32 mask;

  if (actor == NULL)
    return NULL;
  {
    visual = actor->sprite;
    // Sixteen colours, palette 0, priority 1.
    {
      s32 sixteenColors = ~(OBJ_256_COLORS >> 8);

      visual->attr0High &= sixteenColors;
    }
    mask = visual->attr2High & 0x0f;
    mask &= ~(OBJ_PRIORITY(3) >> 8);
    mask |= OBJ_PRIORITY(1) >> 8;
    visual->attr2High = mask;
    actor->motionFlags = 0;
    actor->retainSpriteCache = 1;
    visual->flags = 0;
    visual->layerCount = 0;
    icon = HeapSlot_Alloc(HEAP_SLOT_GLYPH_TRANSFER, sizeof(GlyphTransfer));
    Icon_DrawItem(ITEM_SCOOP_GEM);
    VramCache_Load(visual->vramBlock, ICON_TILE_BYTES, icon->tiles);
    HeapSlot_Release(HEAP_SLOT_GLYPH_TRANSFER);
  }
  return actor;
}

/// After the battle, the King Scorpion leaves the Scoop Gem behind.
void YampiDesert_DropScoopGem(void) {
  struct Actor *actor;

  Event_Start();
  Event_EndLastingPsynergy(0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_Wait(20);
  Actor_SetAnimationById(gMainRoutineState.leaderActorId,
                         ANIM_LEADER_ARMS_RAISED);
  actor = YampiDesert_CreateScoopGem();
  PartyInventory_GiveItem(ITEM_SCOOP_GEM, 0);
  if (actor != 0) {
    Actor_SetPositionAndResetMotion((struct Actor *)actor, 0, 0, 0);
    wait(1);
    Actor_Release(actor);
  }
  Actor_SetAnimationById(gMainRoutineState.leaderActorId, ANIM_STAND);
  GameFlag_Set(FLAG_KING_SCORPION_BEATEN);
  Event_End();
}

/// Keeps the ripple of sand over the buried King Scorpion.
void YampiDesert_RippleFollowsScorpion(void) {
  struct Actor *actor;
  s16 scene;

  gFieldState->raisedTrigger = gYampiDesert_RaisedEvent;
  if (gYampiDesert_SandRipple != 0) {
    scene = gMainRoutineState.next;
    if (scene == SCENE_YAMPI_DESERT_PSYNERGY_STONE)
      actor = Actor_GetById(ACTOR_PSYNERGY_STONE_ROOM_KING_SCORPION);
    else if (scene == SCENE_YAMPI_DESERT_KING_SCORPION)
      actor = Actor_GetById(ACTOR_KING_SCORPION_ROOM_KING_SCORPION);
    else
      return;
    gYampiDesert_SandRipple->x.fixed = actor->x.fixed;
    gYampiDesert_SandRipple->z.fixed = actor->z.fixed;
    gYampiDesert_SandRipple->facing = actor->facing;
  }
}
#undef MOVE_LEADER_TO

void Actor_StartScript(struct Actor *actor, const void *script);

/// The flowing sand carries the leader by (dx, dz), throwing up dust every
/// other frame; with no direction held, it draws the leader to the middle of
/// its cells.
void YampiDesert_CarryLeader(struct Actor *actor, FIXED dx, FIXED dz) {
  Actor_SetSpriteFlags(Actor_GetById(gMainRoutineState.leaderActorId), 0);
  if (gMainRoutineState.movementMode != LEADER_MOVEMENT_SAND) {
    u32 odd = gFrameCount & 1;

    if (odd == 0) {
      struct Actor *dust = (struct Actor *)Value4(
          (s32 (*)())Import_ActorCreate, SPRITE_DUST_CLOUD, actor->x.fixed,
          actor->y.fixed, actor->z.fixed);

      if (dust != NULL) {
        struct Sprite *sprite;

        dust->motionFlags = odd;
        dust->collisionFlags = odd;
        Actor_SetSpriteFlags(dust, 0);
        Actor_SelectAnimation(dust, 1);
        Actor_StartScript(dust, gYampiDesert_FlowDust);
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
        dust->update = FieldEffect_AdvanceMotionAtLeaderPriority;
      }
    }
  }
  if (dx != 0) {
    actor->x.fixed += dx;
    if (!(gInput.held & KEYS_DPAD)) {
      s32 z = FIXEDtoINT(actor->z.fixed);

      actor->z.fixed += INTtoFIXED(8 - z % 16) >> 3;
    }
  } else {
    actor->z.fixed += dz;
    if (!(gInput.held & KEYS_DPAD)) {
      s32 x = FIXEDtoINT(actor->x.fixed);

      actor->x.fixed += INTtoFIXED(8 - x % 16) >> 3;
    }
  }
}

void TilesetAnim_SuspendAll(void);
void TilesetAnim_ResumeAll(void);

/// The task of the rooms with flowing sand: on the flow's cells the sand
/// carries the leader a pixel and a half a frame its way; on the cells the
/// ground marks, holding the pad that way moves the leader a quarter as fast.
void YampiDesert_UpdateSandFlow(void) {
  struct FieldState *state = Ram_HeapSlots->fieldState;
  struct Actor *leader = Actor_GetById(gMainRoutineState.leaderActorId);
  s32 speed = toFIXED(3) / 2;
  struct MapState *map;
  struct MapCell *here, *ahead, *under;
  s32 index, column, row;
  struct MapCell *cells;

  if (state->psynergy != 0 || state->fieldMenu != 0 || state->pauseMenu != 0) {
    Actor_SetAnimationSpeed(leader, 0);
    TilesetAnim_SuspendAll();
    return;
  }
  map = Ram_HeapSlots->mapGrid;
  index = Actor_GetCellIndex(leader);
  column = leader->x.fixed / toFIXED(16);
  row = (leader->z.fixed + toFIXED(4)) / toFIXED(16);
  cells = map->layers[MAPGRID_LAYER_GROUND].cells;
  here = cells + index;
  ahead = cells + (column + row * MAPGRID_SIZE);
  under = map->layers[MAPGRID_LAYER_MIDDLE].cells + index;
  Actor_SetAnimationSpeed(leader, ANIM_STEP_NORMAL);
  TilesetAnim_ResumeAll();
  if ((u8)(under->collisionCode - CELL_BEHAVIOR_YAMPI_DESERT_FLOW_NORTH) <= 4) {
    if (gMainRoutineState.movementMode == LEADER_MOVEMENT_SAND)
      speed = 0;
    else
      Actor_SetSpriteFlags(Actor_GetById(gMainRoutineState.leaderActorId),
                           SPRITE_FLAG_SHADOW);
  }
  if (ahead->collisionCode == CELL_BEHAVIOR_WALL)
    speed = 0;
  switch (here->collisionCode) {
  case CELL_BEHAVIOR_YAMPI_DESERT_FLOW_NORTH:
    if (gInput.held & KEY_UP) {
      YampiDesert_CarryLeader(leader, 0, -speed >> 2);
      return;
    }
    break;
  case CELL_BEHAVIOR_YAMPI_DESERT_FLOW_SOUTH:
    if (gInput.held & KEY_DOWN) {
      YampiDesert_CarryLeader(leader, 0, speed >> 2);
      return;
    }
    break;
  case CELL_BEHAVIOR_YAMPI_DESERT_FLOW_WEST:
    if (gInput.held & KEY_LEFT) {
      YampiDesert_CarryLeader(leader, -speed >> 2, 0);
      return;
    }
    break;
  case CELL_BEHAVIOR_YAMPI_DESERT_FLOW_EAST:
    if (gInput.held & KEY_RIGHT) {
      YampiDesert_CarryLeader(leader, speed >> 2, 0);
      return;
    }
    break;
  }
  switch (under->collisionCode) {
  case CELL_BEHAVIOR_YAMPI_DESERT_FLOW_NORTH:
    YampiDesert_CarryLeader(leader, 0, -speed);
    break;
  case CELL_BEHAVIOR_YAMPI_DESERT_FLOW_SOUTH:
    YampiDesert_CarryLeader(leader, 0, speed);
    break;
  case CELL_BEHAVIOR_YAMPI_DESERT_FLOW_WEST:
    YampiDesert_CarryLeader(leader, -speed, 0);
    break;
  case CELL_BEHAVIOR_YAMPI_DESERT_FLOW_EAST:
    YampiDesert_CarryLeader(leader, speed, 0);
    break;
  }
}

/// Notes whether the leader stands on the upper level; in Blitz's room, keeps
/// the stake before the climbable wall drawn as the leader is.
void YampiDesert_CheckUpperLevel(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineState.leaderActorId);
  FIXED x;
  FIXED y;
  FIXED z;

  y = leader->y.fixed;
  if (y > toFIXED(16))
    gMainRoutineState.onUpperLevel = 1;
  else
    gMainRoutineState.onUpperLevel = 0;
  if (gMainRoutineState.next == SCENE_YAMPI_DESERT_BLITZ) {
    Actor_SetSpritePriority(ACTOR_CLIMBABLE_WALL_STAKE,
                            ((struct FieldSprite *)leader->sprite)->priority);
    x = leader->x.fixed;
    if (x > toFIXED(472)) {
      z = leader->z.fixed;
      if (z <= toFIXED(488))
        return;
      if (leader->y.fixed > -toFIXED(16))
        gMainRoutineState.onUpperLevel = 1;
      else
        gMainRoutineState.onUpperLevel = 0;
    }
  }
}

/// Notes whether the leader has climbed above the lower level.
void YampiDesert_CheckUpperLevelFromLower(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineState.leaderActorId);

  if (leader->y.fixed > -toFIXED(16))
    gMainRoutineState.onUpperLevel = 1;
  else
    gMainRoutineState.onUpperLevel = 0;
}

/// Unused: notes whether the leader stands high on the cells of
/// CELL_BEHAVIOR_YAMPI_DESERT_RISE.
UNUSED void YampiDesert_CheckUpperLevelOnRise(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineState.leaderActorId);
  struct MapState *map = Ram_HeapSlots->mapGrid;
  s32 index = Actor_GetCellIndex(leader);
  struct MapCell *cell = map->layers[MAPGRID_LAYER_GROUND].cells + index;
  struct MainRoutineState *party;
  FIXED y;

  if (cell->collisionCode == CELL_BEHAVIOR_YAMPI_DESERT_RISE) {
    y = leader->y.fixed;
    if (y > toFIXED(16)) {
      gMainRoutineState.onUpperLevel = 1;
      return;
    }
  }
  party = &gMainRoutineState;
  party->onUpperLevel = 0;
}

/// Unused: notes whether the leader stands near the scorpion pit's middle,
/// where opening a menu raises a trigger.
UNUSED void YampiDesert_CheckLeaderNearPit(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineState.leaderActorId);
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
  if (gFieldState->psynergy == 0 && gFrameCount % 5 == 0) {
    struct MainRoutineState *party = &gMainRoutineState;

    if (party->movementMode != LEADER_MOVEMENT_SAND || gInput.held == 0)
      Sound_Play(SE_SAND_MOVE);
  }
}

/// Clears the trigger the scorpion has raised.
void YampiDesert_ClearRaisedEvent(void) {
  gFieldState->raisedTrigger = 0;
  gYampiDesert_RaisedEvent = 0;
}

/// Whether an actor has no move target left.
s32 YampiDesert_HasArrived(struct Actor *actor) {
  s32 x = actor->targetX;

  if (x == ACTOR_NO_TARGET && actor->targetZ == x)
    return 1;
  return 0;
}

/// A step of the buried King Scorpion's rounds, kept in its riseCounter: sends
/// it toward a point, in pixels, and goes on to the next step, which waits for
/// it there.
#define WALK(state_, x, z)                                                     \
  case state_:                                                                 \
    FieldActor_SetMoveTarget((s32)actor, x, 0, z);                             \
    *state = state_ + 1;                                                       \
    break;

/// As WALK, after YampiDesert_ClearRaisedEvent.
#define WALK_ON(state_, x, z)                                                  \
  case state_:                                                                 \
    YampiDesert_ClearRaisedEvent();                                            \
    FieldActor_SetMoveTarget((s32)actor, x, 0, z);                             \
    *state = state_ + 1;                                                       \
    break;

/// Waits for the actor to arrive, then goes on to step next.
#define ARRIVE(state_, next)                                                   \
  case state_:                                                                 \
    if (YampiDesert_HasArrived(actor))                                         \
      *state = next;                                                           \
    break;

/// Waits for the actor to arrive, then goes on to step turn once the flag is
/// set and to step next while it is not.
#define ARRIVE_OR(state_, flag, turn, next)                                    \
  case state_:                                                                 \
    if (YampiDesert_HasArrived(actor)) {                                       \
      if (GameFlag_Get(flag))                                                  \
        *state = turn;                                                         \
      else                                                                     \
        *state = next;                                                         \
    }                                                                          \
    break;

/// The buried King Scorpion's round in the Psynergy Stone's room, a loop of
/// steps, two to each point: an even step sends it toward the point, the odd
/// one after waits for it there. Once Pound has sunk stake 3, it stops at the
/// stake's south side and raises RAISED_STIR_SCORPION.
void YampiDesert_ScorpionRoamsPsynergyStone(struct Actor *actor) {
  u8 *state = &actor->riseCounter;

  switch (*state) {
    WALK(0, 280, 200)
    ARRIVE(1, 2)
    WALK(2, 280, 264)
    ARRIVE(3, 4)
    WALK(4, 184, 264)
    ARRIVE(5, 6)
    WALK(6, 184, 248)
  case 7:
    if (YampiDesert_HasArrived(actor)) {
      if (GameFlag_Get(FLAG_YAMPI_DESERT_STAKE_POUNDED(3))) {
        actor->update = NULL;
        gYampiDesert_RaisedEvent = RAISED_STIR_SCORPION;
      } else {
        *state = 8;
      }
    }
    break;
    WALK(8, 184, 200)
    ARRIVE(9, 0)
  }
}

/// Readies the Psynergy Stone room's King Scorpion and the sand ripple that
/// follows it under the sand, and sets the scorpion on its round unless the
/// party is back from a battle.
void YampiDesert_SetUpPsynergyStoneScorpion(void) {
  struct Actor *scorpion =
      Actor_GetById(ACTOR_PSYNERGY_STONE_ROOM_KING_SCORPION);
  struct Actor *actor;
  struct Actor **followed;

  FieldTask_AddFrame(YampiDesert_PlayScorpionRumble, TASK_PASS_FRAME);
  followed = &gYampiDesert_SandRipple;
  actor = Actor_GetById(ACTOR_PSYNERGY_STONE_ROOM_SAND_RIPPLE);
  actor->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
  *followed = (struct Actor *)actor;
  actor->scaleX = FIXED_ONE * 13 / 10;
  actor->scaleY = FIXED_ONE * 13 / 10;
  Actor_DisableById(ACTOR_PSYNERGY_STONE_ROOM_SAND_RIPPLE);
  (*followed)->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  Actor_SetSpritePriority(ACTOR_PSYNERGY_STONE_ROOM_SAND_RIPPLE, 3);
  Actor_SetSpeed(ACTOR_PSYNERGY_STONE_ROOM_KING_SCORPION, 1.6);
  Actor_SetSpriteColor((struct Actor *)scorpion, SPRITE_COLOR_HIDDEN);
  Actor_SetSpritePriority(ACTOR_PSYNERGY_STONE_ROOM_KING_SCORPION, 3);
  Actor_DisableById(ACTOR_PSYNERGY_STONE_ROOM_KING_SCORPION);
  Actor_GetById(ACTOR_PSYNERGY_STONE_ROOM_KING_SCORPION)->motionFlags =
      ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  if (!GameFlag_Get(FLAG_KEEP_PARTY_POSITION)) {
    scorpion->riseCounter = 0;
    scorpion->update = YampiDesert_ScorpionRoamsPsynergyStone;
  }
  YampiDesert_ClearRaisedEvent();
  Task_Add(YampiDesert_RippleFollowsScorpion, TASK_PASS_FRAME);
}

/// The buried King Scorpion's round in the King Scorpion's room, about stake 0:
/// once Pound has sunk the stake, it stops at its east side and raises
/// RAISED_STIR_SCORPION_AGAIN, or, past that point, keeps from step 32 to a
/// small loop south of it.
void YampiDesert_ScorpionRoamsKingScorpion(struct Actor *actor) {
  u8 *state = &actor->riseCounter;

  switch (*state) {
    WALK(0, 312, 664)
    ARRIVE(1, 2)
    WALK(2, 312, 728)
    ARRIVE(3, 4)
    WALK(4, 246, 728)
  case 5:
    if (YampiDesert_HasArrived(actor)) {
      if (GameFlag_Get(FLAG_YAMPI_DESERT_STAKE_POUNDED(0))) {
        actor->update = NULL;
        gYampiDesert_RaisedEvent = RAISED_STIR_SCORPION_AGAIN;
      } else {
        *state = 6;
      }
    }
    break;
    WALK(6, 184, 728)
    ARRIVE(7, 8)
    WALK(8, 184, 712)
    ARRIVE(9, 10)
    WALK(10, 280, 712)
    ARRIVE(11, 12)
    WALK(12, 280, 760)
    ARRIVE(13, 14)
    WALK(14, 232, 760)
    ARRIVE(15, 16)
    WALK(16, 232, 744)
  case 17:
    if (YampiDesert_HasArrived(actor)) {
      if (!GameFlag_Get(FLAG_YAMPI_DESERT_STAKE_POUNDED(0)))
        *state = 18;
      else
        *state = 32;
    }
    break;
    WALK(18, 232, 664)
    ARRIVE(19, 0)
    WALK_ON(32, 296, 744)
    ARRIVE(33, 34)
    WALK(34, 296, 760)
    ARRIVE(35, 36)
    WALK(36, 232, 760)
    ARRIVE(37, 38)
    WALK(38, 232, 744)
    ARRIVE(39, 32)
  }
}

/// YampiDesert_SetUpPsynergyStoneScorpion's counterpart in the King Scorpion's
/// room, which also places the scorpion at the start of its round.
void YampiDesert_SetUpKingScorpionScorpion(void) {
  struct Actor *scorpion =
      Actor_GetById(ACTOR_KING_SCORPION_ROOM_KING_SCORPION);
  struct Actor *actor;
  struct Actor **followed;

  FieldTask_AddFrame(YampiDesert_PlayScorpionRumble, TASK_PASS_FRAME);
  followed = &gYampiDesert_SandRipple;
  actor = Actor_GetById(ACTOR_KING_SCORPION_ROOM_SAND_RIPPLE);
  actor->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
  *followed = (struct Actor *)actor;
  actor->scaleX = FIXED_ONE * 13 / 10;
  actor->scaleY = FIXED_ONE * 13 / 10;
  Actor_DisableById(ACTOR_KING_SCORPION_ROOM_SAND_RIPPLE);
  (*followed)->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  Actor_SetSpritePriority(ACTOR_KING_SCORPION_ROOM_SAND_RIPPLE, 3);
  Actor_SetSpeed(ACTOR_KING_SCORPION_ROOM_KING_SCORPION, 1.6);
  Actor_SetSpriteColor((struct Actor *)scorpion, SPRITE_COLOR_HIDDEN);
  Actor_SetSpritePriority(ACTOR_KING_SCORPION_ROOM_KING_SCORPION, 3);
  Actor_DisableById(ACTOR_KING_SCORPION_ROOM_KING_SCORPION);
  Actor_GetById(ACTOR_KING_SCORPION_ROOM_KING_SCORPION)->motionFlags =
      ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  if (!GameFlag_Get(FLAG_KEEP_PARTY_POSITION)) {
    YampiDesert_SetPositionAndFacing(ACTOR_KING_SCORPION_ROOM_KING_SCORPION,
                                     toFIXED(232), toFIXED(664), 0);
    scorpion->riseCounter = 0;
    scorpion->update = YampiDesert_ScorpionRoamsKingScorpion;
  }
  YampiDesert_ClearRaisedEvent();
  Task_Add(YampiDesert_RippleFollowsScorpion, TASK_PASS_FRAME);
}

/// The buried King Scorpion's round in the King Scorpion's room once stirred
/// again, between stakes 1 and 2 of the trap: stopped by stake 2 sunk to its
/// west, it loops east of that stake from step 32, by stake 1 sunk to its east,
/// west of that stake from step 48, and by stake 1 sunk to its north, east of
/// stake 2 from step 34; reaching stake 2's north side once that is sunk, it is
/// cornered and raises RAISED_CORNER_SCORPION.
void YampiDesert_ScorpionRoamsToTrap(struct Actor *actor) {
  u8 *state = &actor->riseCounter;

  switch (*state) {
    WALK(0, 312, 264)
    ARRIVE(1, 2)
    WALK(2, 312, 344)
    ARRIVE(3, 4)
    WALK(4, 296, 344)
    ARRIVE_OR(5, FLAG_YAMPI_DESERT_STAKE_POUNDED(2), 32, 6)
    WALK(6, 168, 344)
    ARRIVE(7, 8)
    WALK(8, 168, 296)
    ARRIVE(9, 10)
    WALK(10, 184, 296)
    ARRIVE_OR(11, FLAG_YAMPI_DESERT_STAKE_POUNDED(1), 48, 12)
    WALK(12, 280, 296)
    ARRIVE(13, 14)
    WALK(14, 280, 328)
  case 15:
    if (YampiDesert_HasArrived(actor)) {
      if (GameFlag_Get(FLAG_YAMPI_DESERT_STAKE_POUNDED(2))) {
        actor->update = NULL;
        gYampiDesert_RaisedEvent = RAISED_CORNER_SCORPION;
      } else {
        *state = 16;
      }
    }
    break;
    WALK(16, 280, 360)
    ARRIVE(17, 18)
    WALK(18, 200, 360)
    ARRIVE(19, 20)
    WALK(20, 200, 312)
    ARRIVE_OR(21, FLAG_YAMPI_DESERT_STAKE_POUNDED(1), 34, 22)
    WALK(22, 200, 264)
    ARRIVE(23, 0)
    WALK_ON(32, 296, 312)
    ARRIVE(33, 34)
    WALK_ON(34, 328, 312)
    ARRIVE(35, 36)
    WALK(36, 328, 376)
    ARRIVE(37, 38)
    WALK(38, 296, 376)
    ARRIVE(39, 32)
    WALK_ON(48, 184, 344)
    ARRIVE(49, 50)
    WALK(50, 168, 344)
    ARRIVE(51, 52)
    WALK(52, 168, 296)
    ARRIVE(53, 54)
    WALK(54, 184, 296)
    ARRIVE(55, 48)
  }
}

/// YampiDesert_SetUpPsynergyStoneScorpion's counterpart in the King Scorpion's
/// room once it is stirred again: once the scorpion is cornered
/// (FLAG_YAMPI_DESERT_SCORPION_CORNERED) it is placed facing south;
/// otherwise, unless the party is back from a battle
/// (FLAG_KEEP_PARTY_POSITION), it is placed and given the update
/// YampiDesert_ScorpionRoamsToTrap.
void YampiDesert_SetUpTrappedScorpion(void) {
  struct Actor *scorpion =
      Actor_GetById(ACTOR_KING_SCORPION_ROOM_KING_SCORPION);
  struct Actor *actor;
  struct Actor **followed;

  followed = &gYampiDesert_SandRipple;
  actor = Actor_GetById(ACTOR_KING_SCORPION_ROOM_SAND_RIPPLE);
  actor->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
  actor->scaleX = FIXED_ONE * 13 / 10;
  actor->scaleY = FIXED_ONE * 13 / 10;
  *followed = (struct Actor *)actor;
  Actor_DisableById(ACTOR_KING_SCORPION_ROOM_SAND_RIPPLE);
  Actor_SetSpritePriority(ACTOR_KING_SCORPION_ROOM_SAND_RIPPLE, 3);
  (*followed)->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  Actor_SetSpeed(ACTOR_KING_SCORPION_ROOM_KING_SCORPION, 1.6);
  Actor_SetSpriteColor((struct Actor *)scorpion, SPRITE_COLOR_HIDDEN);
  Actor_SetSpritePriority(ACTOR_KING_SCORPION_ROOM_KING_SCORPION, 3);
  Actor_DisableById(ACTOR_KING_SCORPION_ROOM_KING_SCORPION);
  Actor_GetById(ACTOR_KING_SCORPION_ROOM_KING_SCORPION)->motionFlags =
      ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  if (GameFlag_Get(FLAG_YAMPI_DESERT_SCORPION_CORNERED)) {
    YampiDesert_SetPositionAndFacing(ACTOR_KING_SCORPION_ROOM_KING_SCORPION,
                                     toFIXED(232), toFIXED(168), FACING_SOUTH);
  } else {
    FieldTask_AddFrame(YampiDesert_PlayScorpionRumble, TASK_PASS_FRAME);
    if (!GameFlag_Get(FLAG_KEEP_PARTY_POSITION)) {
      YampiDesert_SetPositionAndFacing(ACTOR_KING_SCORPION_ROOM_KING_SCORPION,
                                       toFIXED(200), toFIXED(264), 0);
      scorpion->riseCounter = 0;
      scorpion->update = YampiDesert_ScorpionRoamsToTrap;
    }
  }
  YampiDesert_ClearRaisedEvent();
  Task_Add(YampiDesert_RippleFollowsScorpion, TASK_PASS_FRAME);
}

/// While the King Scorpion's Scoop runs, opens the hole it digs.
void YampiDesert_OpenScoopHole(void) {
  if (((struct FieldEffectState *)Ram_HeapSlots->fieldEffectState)->stage != 0)
    MapGrid_CopyCellTilesIndicesRect(73, 76, 78, 11, 1, 1);
}

/// The trapped King Scorpion digs its way out with Scoop.
void YampiDesert_ScorpionUsesScoop(void) {
  FieldPsynergy_LoadResources(ABILITY_SCOOP, 1);
  FieldTask_AddFrame(YampiDesert_OpenScoopHole, TASK_PASS_FRAME);
  FieldPsynergy_SetupActorPair(ACTOR_KING_SCORPION_ROOM_KING_SCORPION, -1);
  FieldPsynergy_StartCast();
  FieldPsynergy_RunHandler(1);
  BattleEffect_StopMainActor();
  Task_Remove(YampiDesert_OpenScoopHole);
  FieldPsynergy_End();
}

#include "shared/actor_hide.h"

/// Back from a battle, hides the unseen blocks again unless they were shown;
/// otherwise marks them shown.
void YampiDesert_RestoreBlocks(void) {
  if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION)) {
    if (GameFlag_Get(FLAG_YAMPI_DESERT_BLOCKS_SHOWN) == 0)
      YampiDesert_HideBlocks();
  } else {
    GameFlag_Set(FLAG_YAMPI_DESERT_BLOCKS_SHOWN);
  }
}

/// Hides the Trainer's Whip chest's room's unseen blocks.
void YampiDesert_SetUpTrainersWhipChestBlocks(void) {
  Actor_Hide(ACTOR_WHIP_CHEST_ROOM_FIRST_BLOCK);
  Actor_Hide(ACTOR_WHIP_CHEST_ROOM_SECOND_BLOCK);
  YampiDesert_RestoreBlocks();
}

/// Hides the Blow Mace chest's room's unseen blocks.
void YampiDesert_SetUpBlowMaceChestBlocks(void) {
  u32 i;

  for (i = 0; i <= ACTOR_BLOW_MACE_CHEST_ROOM_LAST_BLOCK -
                       ACTOR_BLOW_MACE_CHEST_ROOM_FIRST_BLOCK;
       i++)
    Actor_Hide(ACTOR_BLOW_MACE_CHEST_ROOM_FIRST_BLOCK + i);
  YampiDesert_RestoreBlocks();
}

/// The leader climbs down the ladder at (x, z) and leaves by `exit`.
void YampiDesert_ClimbDownLadder(struct Actor *actor, s32 x, s32 z, s32 exit) {
  struct MainRoutineState *party;

  Event_Start();
  Event_EndLastingPsynergy(0);
  actor->motionFlags = 0;
  party = &gMainRoutineState;
  Actor_SetSpeed(party->leaderActorId, 1);
  Actor_WalkToAndWaitById(party->leaderActorId, x, z);
  Actor_FaceDirectionAndRefreshById(party->leaderActorId, FACING_NORTH);
  Actor_SetSpriteFlags(Actor_GetById(party->leaderActorId), 0);
  Actor_SetAnimationById(party->leaderActorId, ANIM_LEADER_CLIMB_DOWN_FROM_TOP);
  Actor_SetMoveTarget(actor, actor->x.fixed, actor->y.fixed - toFIXED(10),
                      actor->z.fixed);
  Actor_WaitForMove(actor);
  Actor_SetAnimationById(party->leaderActorId, ANIM_LEADER_CLIMB);
  Sound_Play(SE_MAP_EXIT);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Import_EventSetExitRequest(exit);
}

/// Walking into the hole Scoop has dug in the sand puddle at the south of the
/// spring's room (cell (8, 32)), holding the pad towards it, Felix climbs down
/// its ladder, facing north, into the cave under the spring.
void YampiDesert_ClimbRevealedLadder(void) {
  struct Actor *actor;
  s32 x;
  s32 z;

  if (GameFlag_Get(FLAG_YAMPI_DESERT_LADDER_DUG_UP)) {
    actor = Actor_GetById(gMainRoutineState.leaderActorId);
    x = FIELD_CELL(actor->x.fixed);
    z = FIELD_CELL(actor->z.fixed);
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
    YampiDesert_ClimbDownLadder(
        actor, 136, 516, YAMPI_DESERT_SPRING_TO_YAMPI_DESERT_SPRING_CAVE_SOUTH);
  }
}

/// Walking north into the ladder of the small chamber of the cave under the
/// spring, Felix climbs down it to the cave below.
void YampiDesert_ClimbDownSpringCaveLadder(void) {
  YampiDesert_ClimbDownLadder(Actor_GetById(gMainRoutineState.leaderActorId),
                              408, 432, SPRING_CAVE_TO_LOWER_CAVE);
}

void YampiDesert_BurstSand(s32 x, s32 y, s32 z);

/// Dropping into the scorpion pit, the leader lands before the King Scorpion,
/// and its battle starts.
void YampiDesert_LandAndFightKingScorpion(void) {
  struct MainRoutineState *party = &gMainRoutineState;
  struct Actor *leader = Actor_GetById(party->leaderActorId);
  struct Actor *scorpion = Actor_GetById(ACTOR_KING_SCORPION);

  Event_Start();
  Event_EndLastingPsynergy(0);
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
  YampiDesert_SetPositionAndFacing(ACTOR_KING_SCORPION, toFIXED(232),
                                   toFIXED(148), FACING_SOUTH);
  scorpion->motionFlags = 0;
  scorpion->y.fixed = toFIXED(130);
  scorpion->gravity = FIXED_ONE / 2;
  scorpion->bounce = 0;
  wait(1);
  gFieldState->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_CIRCLE);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Sound_Play(SE_DUST_PUFF);
  leader->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  Actor_SetSpriteFlags(Actor_GetById(party->leaderActorId), SPRITE_FLAG_SHADOW);
  FieldEvent_Wait(24);
  YampiDesert_BurstSand(leader->x.fixed, leader->y.fixed, leader->z.fixed);
  Sound_Play(SE_THUMP);
  FieldEffect_SetAttached(party->leaderActorId,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ATTACH);
  Actor_SetAnimationAndWait(party->leaderActorId, ANIM_LEADER_SIT);
  Sound_Play(SE_DUST_PUFF);
  scorpion->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_KING_SCORPION), SPRITE_FLAG_SHADOW);
  FieldEvent_Wait(24);
  YampiDesert_BurstSand(scorpion->x.fixed, scorpion->y.fixed,
                        scorpion->z.fixed);
  Sound_Play(SE_THUMP);
  Actor_FaceDirectionById(party->leaderActorId, FACING_NORTH, 0);
  FieldEvent_Wait(40);
  Sound_Play(SE_BEAST_CRY);
  Actor_SetAnimationById(ACTOR_KING_SCORPION, 3);
  FieldEvent_Wait(20);
  scorpion->velocityY = toFIXED(8);
  scorpion->gravity = toFIXED(9) / 10;
  Actor_SetSpeed(ACTOR_KING_SCORPION, 1.6);
  Actor_MoveToAndWaitById(ACTOR_KING_SCORPION, 232, 184);
  party->battleType = BATTLE_TYPE_NO_ESCAPE;
  Event_SetAfterBattleDestination(
      SCENE_YAMPI_DESERT_SCORPION_PIT,
      YAMPI_DESERT_SCORPION_PIT_AFTER_KING_SCORPION_BATTLE);
  Field_RequestBattleFromEncounterTable(
      ENCOUNTER_ZONE_SCRIPTED_YAMPI_DESERT_ALHAFRA, 0);
}

/// Clears the scorpion's sand ripple and runs the entered room's setup; the
/// sand pass has none.
s32 Scene_Initialize(void) {
  s16 scene;

  YampiDesert_ClearRaisedEvent();
  gYampiDesert_SandRipple = 0;
  scene = gMainRoutineState.next;
  if (scene == SCENE_YAMPI_DESERT_GUARDIAN_RING_CHEST)
    YampiDesert_SetUpGuardianRingChest();
  else if (scene == SCENE_YAMPI_DESERT_SPRING)
    YampiDesert_SetUpSpring();
  else if (scene == SCENE_YAMPI_DESERT_BLITZ)
    YampiDesert_SetUpBlitz();
  else if (scene == SCENE_YAMPI_DESERT_PSYNERGY_STONE)
    YampiDesert_SetUpPsynergyStone();
  else if (scene == SCENE_YAMPI_DESERT_KING_SCORPION)
    YampiDesert_SetUpKingScorpion();
  else if (scene == SCENE_YAMPI_DESERT_COINS_PUDDLE)
    YampiDesert_SetUpCoinsPuddle();
  else if (scene == SCENE_YAMPI_DESERT_TRAINERS_WHIP_CHEST)
    YampiDesert_SetUpTrainersWhipChest();
  else if (scene == SCENE_YAMPI_DESERT_HIDDEN_PSYNERGY_STONE)
    YampiDesert_SetUpHiddenPsynergyStone();
  else if (scene == SCENE_YAMPI_DESERT_SCORPION_PIT)
    YampiDesert_SetUpScorpionPit();
  else if (scene == SCENE_YAMPI_DESERT_SPRING_CAVE)
    YampiDesert_SetUpSpringCave();
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
  if (gMainRoutineState.entrance ==
      YAMPI_DESERT_GUARDIAN_RING_CHEST_FROM_WORLD_MAP) {
    GameFlag_Set(FLAG_YAMPI_DESERT_ENTERED_BY_GUARDIAN_RING_CHEST);
    GameFlag_Clear(FLAG_YAMPI_DESERT_ENTERED_BY_TRAINERS_WHIP_CHEST);
    GameFlag_Clear(FLAG_YAMPI_DESERT_ENTERED_BY_HIDDEN_PSYNERGY_STONE);
  }
}

/// The travelers are gone once the Madrans have reached Alhafra
/// (FLAG_ALHAFRA_MADRANS_ARRIVED); once met (FLAG_YAMPI_DESERT_TRAVELERS_MET)
/// they stand about the spring.
void YampiDesert_SetUpSpring(void) {
  s32 angleA;
  s32 angleB;

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
    YampiDesert_SetPositionAndFacing(ACTOR_TRAVELER_SEEKING_EVIDENCE,
                                     toFIXED(533), toFIXED(271), FACING_WEST);
    YampiDesert_SetPositionAndFacing(ACTOR_DIZZY_TRAVELER, toFIXED(435),
                                     toFIXED(232), FACING_SOUTH_SOUTHEAST);
    YampiDesert_SetPositionAndFacing(ACTOR_PARCHED_TRAVELER, toFIXED(467),
                                     toFIXED(248), FACING_SOUTH_SOUTHWEST);
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
  Task_Add(YampiDesert_CheckUpperLevel, TASK_PASS_FRAME);
}

/// Keeps two of Blitz's room's stakes drawn as the leader is.
void YampiDesert_LayerStakesWithLeader(void) {
  s32 variant =
      ((struct FieldSprite *)Actor_GetById(gMainRoutineState.leaderActorId)
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
  FieldTask_AddFrame(YampiDesert_CheckUpperLevel, TASK_PASS_FRAME);
  FieldTask_AddFrame(YampiDesert_LayerStakesWithLeader, TASK_PASS_FRAME);
}

/// Sets up a stake that stands sunk to its top from the start: drawn underfoot
/// and behind the party in its sunk wooden pose, with no shadow, and neither
/// moving nor colliding.
void YampiDesert_SetStakeSunk(s32 id) {
  Actor_GetById(id)->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
  Actor_SetSpritePriority(id, 3);
  Actor_DisableById(id);
  Actor_SetAnimationById(id, 2);
}

/// Sets up the Psynergy Stone's room: its stakes, and the King Scorpion until
/// it has been stirred.
void YampiDesert_SetUpPsynergyStone(void) {
  FieldEffect_StartBlankOverlay(0);
  if (gMainRoutineState.entrance == ENTRANCE_FROM_SOUTH_PART ||
      gMainRoutineState.entrance ==
          YAMPI_DESERT_PSYNERGY_STONE_FROM_YAMPI_DESERT_KING_SCORPION) {
    if (Event_GetFlag(FLAG_YAMPI_DESERT_SCORPION_STIRRED))
      Actor_SetPosition(ACTOR_PSYNERGY_STONE_ROOM_KING_SCORPION, 0, 0);
    else
      YampiDesert_SetUpPsynergyStoneScorpion();
  }
  YampiDesert_PlaceStakes(gYampiDesertPsynergyStoneStakes);
  if (Event_GetFlag(FLAG_YAMPI_DESERT_ON_STAKE_TOPS)) {
    YampiDesert_StepOntoStakeTops();
    wait(10);
  }
  YampiDesert_SetStakeSunk(STAKE(4));
  YampiDesert_SetStakeSunk(STAKE(5));
  YampiDesert_SetStakeSunk(STAKE(6));
  YampiDesert_SetStakeSunk(STAKE(7));
  if (gMainRoutineState.entrance ==
          YAMPI_DESERT_PSYNERGY_STONE_FROM_YAMPI_DESERT_BLITZ ||
      gMainRoutineState.entrance == ENTRANCE_FROM_NORTH_PART)
    Task_Add(YampiDesert_CheckUpperLevelFromLower, TASK_PASS_FRAME);
}

/// Sets up the King Scorpion's room: its stakes; in the south part, the King
/// Scorpion once stirred from the Psynergy Stone's room; in the north, the ten
/// stakes of the trap, and the scorpion until it is beaten, or the hole it
/// leaves in the ring of rocks, which shows sand once the geyser is dug.
void YampiDesert_SetUpKingScorpion(void) {
  FieldEffect_StartBlankOverlay(0);
  YampiDesert_PlaceStakes(gYampiDesertKingScorpionStakes);
  YampiDesert_SetStakeSunk(STAKE(3));
  YampiDesert_SetStakeSunk(STAKE(4));
  YampiDesert_SetStakeSunk(STAKE(5));
  YampiDesert_SetStakeSunk(STAKE(6));
  YampiDesert_SetStakeSunk(STAKE(7));
  YampiDesert_SetStakeSunk(STAKE(8));
  YampiDesert_SetStakeSunk(STAKE(9));
  YampiDesert_SetStakeSunk(STAKE(10));
  YampiDesert_SetStakeSunk(STAKE(11));
  YampiDesert_SetStakeSunk(STAKE(12));
  if (gMainRoutineState.entrance ==
          YAMPI_DESERT_KING_SCORPION_FROM_YAMPI_DESERT_PSYNERGY_STONE ||
      gMainRoutineState.entrance == ENTRANCE_FROM_NORTH_PART) {
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
        MapGrid_CopyCellTilesIndicesRect(68, 77, 13, 9, 3, 3);
        MapGrid_CopyCellAttributes(68, 77, 3, 3, 13, 9);
      } else {
        MapGrid_CopyCellTilesIndicesRect(74, 71, 77, 9, 3, 3);
        MapGrid_CopyCellAttributes(74, 71, 3, 3, 13, 9);
      }
      MapGrid_CopyCellAttributes(63, 77, 3, 3, 13, 73);
    } else if (GameFlag_Get(FLAG_YAMPI_DESERT_SCORPION_STIRRED_AGAIN)) {
      YampiDesert_SetUpTrappedScorpion();
    }
  }
}

/// Sets up the room north of the spring: its 315 coins stay hidden until dug
/// up.
void YampiDesert_SetUpCoinsPuddle(void) {
  FieldEffect_StartBlankOverlay(0);
  if (GameFlag_Get(FLAG_YAMPI_DESERT_COINS_PUDDLE_DUG_UP) == 0)
    Actor_SetWideSprite(ACTOR_HIDDEN_TREASURE, 0);
}

/// Sets up the Trainer's Whip chest's room: its rope, sign and flowing
/// sand, and the way in it was entered by. Entered from the Yampi Desert Cave,
/// it sends Retreat to the World Map door the party last came in by (not in the
/// Japanese edition).
void YampiDesert_SetUpTrainersWhipChest(void) {
  struct Actor *actor = Actor_GetById(ACTOR_HIDDEN_TREASURE);

  Scene_QueueMusic(SE_KIRIN);
  Rope_Initialize();
  YampiDesert_PlaceRope(0, FLAG_YAMPI_DESERT_ROPE_TIED, ACTOR_ROPE_POST,
                        ACTOR_ROPE_OTHER_POST);
  if (actor != NULL)
    Actor_SetPosition(ACTOR_HIDDEN_TREASURE, 0, 0);
  actor = Actor_GetById(ACTOR_SIGN);
  actor->scaleX = FIXED_ONE * 7 / 10;
  actor->scaleY = FIXED_ONE * 7 / 10;
  actor->radius = 8;
  YampiDesert_SetUpTrainersWhipChestBlocks();
  Task_Add(YampiDesert_UpdateSandFlow, TASK_PASS_FRAME);
  if (gMainRoutineState.entrance ==
      YAMPI_DESERT_TRAINERS_WHIP_CHEST_FROM_WORLD_MAP) {
    GameFlag_Clear(FLAG_YAMPI_DESERT_ENTERED_BY_GUARDIAN_RING_CHEST);
    GameFlag_Set(FLAG_YAMPI_DESERT_ENTERED_BY_TRAINERS_WHIP_CHEST);
    GameFlag_Clear(FLAG_YAMPI_DESERT_ENTERED_BY_HIDDEN_PSYNERGY_STONE);
  }
#if TLA_PATCH_1
  else if (
      gMainRoutineState.entrance ==
      YAMPI_DESERT_TRAINERS_WHIP_CHEST_FROM_YAMPI_DESERT_CAVE_WATER_OF_LIFE_CHEST) {
    if (GameFlag_Get(FLAG_YAMPI_DESERT_ENTERED_BY_GUARDIAN_RING_CHEST)) {
      gMainRoutineState.retreatScene = SCENE_YAMPI_DESERT_GUARDIAN_RING_CHEST;
      gMainRoutineState.retreatEntrance =
          YAMPI_DESERT_GUARDIAN_RING_CHEST_FROM_WORLD_MAP;
    } else if (GameFlag_Get(FLAG_YAMPI_DESERT_ENTERED_BY_TRAINERS_WHIP_CHEST)) {
      gMainRoutineState.retreatScene = SCENE_YAMPI_DESERT_TRAINERS_WHIP_CHEST;
      gMainRoutineState.retreatEntrance =
          YAMPI_DESERT_TRAINERS_WHIP_CHEST_FROM_WORLD_MAP;
    } else if (GameFlag_Get(
                   FLAG_YAMPI_DESERT_ENTERED_BY_HIDDEN_PSYNERGY_STONE)) {
      gMainRoutineState.retreatScene = SCENE_YAMPI_DESERT_HIDDEN_PSYNERGY_STONE;
      gMainRoutineState.retreatEntrance =
          YAMPI_DESERT_HIDDEN_PSYNERGY_STONE_FROM_WORLD_MAP;
    }
  }
#endif
}

/// Sets up the room of the hidden Psynergy Stone, and the way in it was entered
/// by.
void YampiDesert_SetUpHiddenPsynergyStone(void) {
  if (Actor_GetById(ACTOR_HIDDEN_TREASURE) != 0)
    Actor_SetPosition(ACTOR_HIDDEN_TREASURE, 0, 0);
  if (gMainRoutineState.entrance ==
      YAMPI_DESERT_HIDDEN_PSYNERGY_STONE_FROM_WORLD_MAP) {
    GameFlag_Clear(FLAG_YAMPI_DESERT_ENTERED_BY_GUARDIAN_RING_CHEST);
    GameFlag_Clear(FLAG_YAMPI_DESERT_ENTERED_BY_TRAINERS_WHIP_CHEST);
    GameFlag_Set(FLAG_YAMPI_DESERT_ENTERED_BY_HIDDEN_PSYNERGY_STONE);
  }
}

/// The geyser Scoop opens in the scorpion pit: it rises and falls on
/// its timer, carrying the platform at its top, as Gondowan Cliffs' geysers do.
struct Geyser {
  s16 id;              ///< its actor; 0 for a free entry
  s16 timer;           ///< frames into its period
  s16 height;          ///< pixels it has risen
  s16 period;          ///< frames in one rise and fall
  s16 riseAt;          ///< the timer it starts to rise at
  s16 fallAt;          ///< the timer it starts to fall at
  s16 max;             ///< pixels it rises to
  s16 min;             ///< pixels it falls to
  s16 unreadSetToOne;  ///< set to 1 by Geyser_Add and never read
  s16 step;            ///< pixels a frame: 4 rising, -8 falling, 0 at rest
  s16 unreadCleared;   ///< cleared by Geyser_Add and never read
  s16 phase;           ///< the platform's bob, 1/65536 turns
  struct Actor *rider; ///< the platform at its top, which the leader rides
};

/// The geysers' state, kept in the inventory snapshot's spare room.
struct GeyserState {
  s16 entry;        ///< the VRAM cache entry holding the column's tiles
  s16 firstTerrain; ///< the terrain id and collision slot of the first entry
  struct Geyser entries[GEYSER_ENTRIES];
  /// The OAM words Geyser_Draw queues: GEYSER_SPRITES sprites of three words.
  void *block;
};

void Geyser_Start(s32 unused, s32 firstTerrain);
struct Geyser *Geyser_Add(s32 slot, s32 id, s32 timer, s32 minHeight,
                          s32 maxHeight);

/// Sets up the scorpion pit: dropped in, the King Scorpion's battle; back from
/// it, the Scoop Gem; and the geyser between the four rocks once dug.
void YampiDesert_SetUpScorpionPit(void) {
  gFieldState->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
  GameFlag_Set(FLAG_RETREAT_BLOCKED);
  if (gMainRoutineState.entrance ==
          YAMPI_DESERT_SCORPION_PIT_FROM_YAMPI_DESERT_KING_SCORPION &&
      !GameFlag_Get(FLAG_KING_SCORPION_BEATEN)) {
    YampiDesert_LandAndFightKingScorpion();
  } else {
    if (gMainRoutineState.entrance ==
            YAMPI_DESERT_SCORPION_PIT_AFTER_KING_SCORPION_BATTLE &&
        !GameFlag_Get(FLAG_KING_SCORPION_BEATEN))
      YampiDesert_DropScoopGem();
  }
  if (GameFlag_Get(FLAG_YAMPI_DESERT_ROCK_PIT_DUG_UP)) {
    MapGrid_CopyCellTilesIndicesRect(35, 39, 13, 11, 3, 3);
    Actor_SetPosition(ACTOR_GEYSER, 232, 200);
  }
  Geyser_Start(0, GEYSER_FIRST_TERRAIN);
  Geyser_Add(0, ACTOR_GEYSER, 0, 0, GEYSER_MAX_HEIGHT);
  wait(10);
}

/// Sets up the cave under the spring: it opens through a window transition.
void YampiDesert_SetUpSpringCave(void) {
  gFieldState->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
}

/// Sets up the Blow Mace chest's room: its stake and its flowing sand.
void YampiDesert_SetUpBlowMaceChest(void) {
  gFieldState->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
  Scene_QueueMusic(SE_KIRIN);
  YampiDesert_PlaceStakes(gYampiDesertBlowMaceChestStakes);
  YampiDesert_SetUpBlowMaceChestBlocks();
  FieldTask_AddFrame(YampiDesert_UpdateSandFlow, TASK_PASS_FRAME);
}

/// Places each stake of a room's list, sunk if Pound has sunk it.
void YampiDesert_PlaceStakes(const void *script) {
  const u16 *entry;

  if (*(const u16 *)script == ACTOR_LIST_END)
    return;
  for (entry = script; entry[0] != ACTOR_LIST_END; entry += 3) {
    s32 id = entry[0];
    struct Actor *actor = Actor_GetById(id);
    s32 kind = (s16)entry[1];
    s32 flag = (s16)entry[2];

    actor->collisionFlags &= ~ACTOR_SOLID;
    Actor_SetSpriteFlags(actor, 0);
    Actor_SetCellOpen(id, (u16)kind, (u16)flag, actor->layer);
  }
}
#undef WALK
#undef ARRIVE
#undef WALK_ON
#undef ARRIVE_OR

// Finds the current target's table row and shrinks it.
#include "shared/pound_sink_target.h"

#include "shared/actor_set_cell_open.h"

#include "shared/geyser_update.h"

#include "shared/geyser_draw.h"

#include "shared/geyser_start.h"

/// Makes actor id the geyser of entry slot, rising from minHeight to maxHeight
/// pixels every six seconds, starting timer frames into its round, with an
/// unseen platform at its top for the leader.
struct Geyser *Geyser_Add(s32 slot, s32 id, s32 timer, s32 minHeight,
                          s32 maxHeight) {
  struct GeyserState *state = GEYSER_STATE;
  struct Geyser *entry = &state->entries[slot];
  struct Actor *actor = Actor_GetById(id);
  struct Actor *rider;

  // DEAD: the actor looked up again and dropped; the ROM keeps the call.
  Actor_GetById(id);
  Actor_SetSpriteFlags(actor, 0);
  if (!GameFlag_Get(FLAG_KEEP_PARTY_POSITION)) {
    if (maxHeight < minHeight) {
      s32 swap = minHeight;

      minHeight = maxHeight;
      maxHeight = swap;
    }
    entry->id = id;
    entry->height = 0;
    entry->timer = timer;
    entry->period = GEYSER_PERIOD_FRAMES;
    entry->max = maxHeight;
    entry->riseAt = GEYSER_RISE_FRAME;
    entry->fallAt = GEYSER_FALL_FRAME;
    entry->min = minHeight;
    entry->unreadSetToOne = 1;
    actor->motionFlags = 0;
    actor->collisionFlags = ACTOR_NOT_TARGETED;
    entry->step = 0;
    entry->phase = 0;
    entry->unreadCleared = 0;
    actor->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
  }
  rider = Import_ActorCreate(ACTOR_CREATE_KIND(ACTOR_CREATE_NO_SPRITE),
                             actor->x.fixed, actor->y.fixed, actor->z.fixed);
  rider->collisionFlags = 0;
  rider->motionFlags = 0;
  rider->priorityFlags |= ACTOR_PRIORITY_PLATFORM;
  entry->rider = rider;
  return entry;
}

// clang-format off
/// A field effect's actor that lasts a short, a medium or a long while, then
/// goes back to the origin and frees itself (gFieldEffect_DurationScripts).
#include "shared/field_effect_duration_scripts.c"

/// Where the King Scorpion's cloud of dust rises: x and z in pixels.
u16 gYampiDesert_SandCloudPoints[SAND_CLOUD_POINTS * 2] ALIGNED(4) = {
    224, 152, 232, 152, 240, 152,
    216, 160, 224, 160, 232, 160, 240, 160, 248, 160,
    216, 168, 224, 168, 232, 168, 240, 168, 248, 168,
    216, 176, 248, 176,
};

/// A puff of the flowing sand's dust, which frees itself after a while.
u32 gYampiDesert_FlowDust[] = {
    SC_WAIT(25),
    SC_SET(SW_HOOK, 0),
    SC_POSITION_ORIGIN,
    SC_WAIT(1),
    SC_RELEASE,
};
// clang-format on

#undef MOVE_LEADER_TO
#undef Actor_SetAnimationAndWait
#undef Actor_SetMovementTypeAndWaitById

void YampiDesert_FightBlitz(void);
void YampiDesert_TalkToTraveler(void);
void YampiDesert_TalkToElderAboutWay(void);
void YampiDesert_StepOntoStakeTops(void);
void YampiDesert_HidePsynergyStone(void);
void YampiDesert_HideBlocks(void);

LATE_DATA const s32 *gFieldEffect_DurationScripts[] = {
    gFieldEffect_ShortScript,
    gFieldEffect_MediumScript,
    gFieldEffect_LongScript,
};

// clang-format off
/// The two travelers resting at the spring look about them in turn.
LATE_DATA u32 gYampiDesert_TravelerLooksAround[] = {
    SC_SET(SW_FACING, FACING_NORTH_NORTHWEST),
    SC_WAIT(80),
    SC_SET(SW_FACING, FACING_EAST),
    SC_WAIT(80),
    SC_SET(SW_FACING, FACING_NORTH_NORTHEAST),
    SC_WAIT(20),
    SC_SET(SW_FACING, FACING_EAST),
    SC_WAIT(40),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

/// As gYampiDesert_TravelerLooksAround, for the other traveler, out of step
/// with the first.
LATE_DATA u32 gYampiDesert_OtherTravelerLooksAround[] = {
    SC_SET(SW_FACING, FACING_NORTH_NORTHEAST),
    SC_WAIT(20),
    SC_SET(SW_FACING, FACING_EAST),
    SC_WAIT(40),
    SC_SET(SW_FACING, FACING_NORTH_NORTHWEST),
    SC_WAIT(80),
    SC_SET(SW_FACING, FACING_EAST),
    SC_WAIT(80),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

/// The mayor runs ahead to the travelers, and the others follow.
LATE_DATA u32 gYampiDesert_MayorRunsAhead[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(248, 0, 248),
    SC_SYNC,
    SC_MOVE(248, 0, 280),
    SC_SYNC,
    SC_MOVE(264, 0, 400),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_END,
};

/// As gYampiDesert_MayorRunsAhead, for the travelers who follow.
LATE_DATA u32 gYampiDesert_TravelersFollow[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(240, 0, 280),
    SC_SYNC,
    SC_MOVE(264, 0, 400),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_END,
};

/// As gYampiDesert_MayorRunsAhead, for the last travelers, from further back.
LATE_DATA u32 gYampiDesert_LastTravelersFollow[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(216, 0, 232),
    SC_SYNC,
    SC_MOVE(240, 0, 280),
    SC_SYNC,
    SC_MOVE(264, 0, 400),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_END,
};

/// Each room's stakes, as STAKE_ROW lists them.
LATE_DATA u16 gYampiDesertGuardianRingChestStakes[] = {
    STAKE_ROW(0), STAKE_ROW(1), STAKE_ROW(2), STAKE_ROW(3),
    STAKE_ROW(4), STAKE_ROW(5), ACTOR_LIST_END,
};

/// Blitz's room's stakes.
LATE_DATA u16 gYampiDesertBlitzStakes[] = {
    STAKE_ROW(0), STAKE_ROW(1), STAKE_ROW(2), STAKE_ROW(3),
    STAKE_ROW(4), STAKE_ROW(5), STAKE_ROW(6), STAKE_ROW(7),
    STAKE_ROW(8), STAKE_ROW(9), STAKE_ROW(10), ACTOR_LIST_END,
};

/// The Psynergy Stone's room's stakes.
LATE_DATA u16 gYampiDesertPsynergyStoneStakes[] = {
    STAKE_ROW(0), STAKE_ROW(1), STAKE_ROW(2), STAKE_ROW(3), ACTOR_LIST_END,
};

/// The King Scorpion's room's stakes.
LATE_DATA u16 gYampiDesertKingScorpionStakes[] = {
    STAKE_ROW(0), STAKE_ROW(1), STAKE_ROW(2), ACTOR_LIST_END,
};

/// The Blow Mace chest's room's stake.
LATE_DATA ALIGNED(4) u16 gYampiDesertBlowMaceChestStakes[] = {
    STAKE_ROW(0), ACTOR_LIST_END,
};

/// The trigger the scorpions raise for the scene to run, or 0.
LATE_DATA s32 gYampiDesert_RaisedEvent = 0;

/// The sand ripple that follows the room's King Scorpion under the sand.
LATE_DATA struct Actor *gYampiDesert_SandRipple = NULL;

#define SCENE_ENTRANCES_SECTION LATE_DATA
#include "shared/scene_entrances_default.c"

/// Where the party arrives in the cave under the spring.
LATE_DATA struct SceneEntrance gYampiDesertSpringCaveEntrances[] = {
    {1, CONDITION_ALWAYS, 408, 0, 480, FACING_NORTH, 0, 288, 376, 528, 536, 0},
    {2, CONDITION_ALWAYS, 408, 0, 456, FACING_SOUTH, 0, 288, 376, 528, 536, 0},
    {3, CONDITION_ALWAYS, 120, 0, 424, FACING_SOUTH, 0, 0, 16, 448, 496, 0},
    {4, CONDITION_ALWAYS, 376, 0, 88, FACING_SOUTH, 0, 0, 16, 448, 496, 0},
    {SCENE_TABLE_END},
};

/// The region that leaves the scorpion's pit, a row: the box's least x, y
/// and z and its greatest, in pixels, its condition (-1 always) and its exit;
/// -1 ends the list.
LATE_DATA s16 gYampiDesertScorpionPitExitRegions[] ALIGNED(4) = {
    216, 44, 184, 248, 76, 216, -1, 1,
    -1, 0, 0, 0, 0, 0, 0, 0,
};

/// The two regions that leave the cave under the spring, in rows as the pit's.
LATE_DATA s16 gYampiDesertSpringCaveExitRegions[] = {
    112, 24, 416, 128, 40, 432, -1, 3,
    368, 24, 80, 384, 40, 96, -1, 4,
    -1, 0, 0, 0, 0, 0, 0, 0,
};

LATE_DATA u32 gSceneExits[] = {
    SCENE_EXITS(YampiDesertGuardianRingChest),
    SCENE_EXIT(YAMPI_DESERT_GUARDIAN_RING_CHEST_TO_WORLD_MAP, WorldMap, WORLD_MAP_FROM_YAMPI_DESERT_GUARDIAN_RING_CHEST),
    SCENE_EXIT(YAMPI_DESERT_GUARDIAN_RING_CHEST_TO_YAMPI_DESERT_SPRING, YampiDesertSpring, YAMPI_DESERT_SPRING_FROM_YAMPI_DESERT_GUARDIAN_RING_CHEST),
    SCENE_EXITS(YampiDesertSpring),
    SCENE_EXIT(YAMPI_DESERT_SPRING_TO_YAMPI_DESERT_GUARDIAN_RING_CHEST, YampiDesertGuardianRingChest, YAMPI_DESERT_GUARDIAN_RING_CHEST_FROM_YAMPI_DESERT_SPRING),
    SCENE_EXIT(YAMPI_DESERT_SPRING_TO_YAMPI_DESERT_COINS_PUDDLE, YampiDesertCoinsPuddle, YAMPI_DESERT_COINS_PUDDLE_FROM_YAMPI_DESERT_SPRING),
    SCENE_EXIT(YAMPI_DESERT_SPRING_TO_YAMPI_DESERT_SPRING_CAVE_NORTH, YampiDesertSpringCave, YAMPI_DESERT_SPRING_CAVE_FROM_YAMPI_DESERT_SPRING_NORTH),
    SCENE_EXIT(YAMPI_DESERT_SPRING_TO_YAMPI_DESERT_BLITZ, YampiDesertBlitz, YAMPI_DESERT_BLITZ_FROM_YAMPI_DESERT_SPRING),
    SCENE_EXIT(YAMPI_DESERT_SPRING_TO_YAMPI_DESERT_SPRING_CAVE_SOUTH, YampiDesertSpringCave, YAMPI_DESERT_SPRING_CAVE_FROM_YAMPI_DESERT_SPRING_SOUTH),
    SCENE_EXITS(YampiDesertBlitz),
    SCENE_EXIT(YAMPI_DESERT_BLITZ_TO_YAMPI_DESERT_SPRING, YampiDesertSpring, YAMPI_DESERT_SPRING_FROM_YAMPI_DESERT_BLITZ),
    SCENE_EXIT(YAMPI_DESERT_BLITZ_TO_YAMPI_DESERT_PSYNERGY_STONE, YampiDesertPsynergyStone, YAMPI_DESERT_PSYNERGY_STONE_FROM_YAMPI_DESERT_BLITZ),
    SCENE_EXITS(YampiDesertPsynergyStone),
    SCENE_EXIT(YAMPI_DESERT_PSYNERGY_STONE_TO_YAMPI_DESERT_BLITZ, YampiDesertBlitz, YAMPI_DESERT_BLITZ_FROM_YAMPI_DESERT_PSYNERGY_STONE),
    SCENE_EXIT(EXIT_TO_NORTH_PART, YampiDesertPsynergyStone, ENTRANCE_FROM_SOUTH_PART),
    SCENE_EXIT(EXIT_TO_SOUTH_PART, YampiDesertPsynergyStone, ENTRANCE_FROM_NORTH_PART),
    SCENE_EXIT(YAMPI_DESERT_PSYNERGY_STONE_TO_YAMPI_DESERT_KING_SCORPION, YampiDesertKingScorpion, YAMPI_DESERT_KING_SCORPION_FROM_YAMPI_DESERT_PSYNERGY_STONE),
    SCENE_EXITS(YampiDesertKingScorpion),
    SCENE_EXIT(YAMPI_DESERT_KING_SCORPION_TO_YAMPI_DESERT_PSYNERGY_STONE, YampiDesertPsynergyStone, YAMPI_DESERT_PSYNERGY_STONE_FROM_YAMPI_DESERT_KING_SCORPION),
    SCENE_EXIT(EXIT_TO_NORTH_PART, YampiDesertKingScorpion, ENTRANCE_FROM_SOUTH_PART),
    SCENE_EXIT(EXIT_TO_SOUTH_PART, YampiDesertKingScorpion, ENTRANCE_FROM_NORTH_PART),
    SCENE_EXIT(YAMPI_DESERT_KING_SCORPION_TO_YAMPI_DESERT_SCORPION_PIT, YampiDesertScorpionPit, YAMPI_DESERT_SCORPION_PIT_FROM_YAMPI_DESERT_KING_SCORPION),
    SCENE_EXITS(YampiDesertCoinsPuddle),
    SCENE_EXIT(YAMPI_DESERT_COINS_PUDDLE_TO_YAMPI_DESERT_SPRING, YampiDesertSpring, YAMPI_DESERT_SPRING_FROM_YAMPI_DESERT_COINS_PUDDLE),
    SCENE_EXIT(YAMPI_DESERT_COINS_PUDDLE_TO_YAMPI_DESERT_BLOW_MACE_CHEST, YampiDesertBlowMaceChest, YAMPI_DESERT_BLOW_MACE_CHEST_FROM_YAMPI_DESERT_COINS_PUDDLE),
    SCENE_EXITS(YampiDesertSandPass),
    SCENE_EXIT(YAMPI_DESERT_SAND_PASS_TO_YAMPI_DESERT_BLOW_MACE_CHEST, YampiDesertBlowMaceChest, YAMPI_DESERT_BLOW_MACE_CHEST_FROM_YAMPI_DESERT_SAND_PASS),
    SCENE_EXIT(YAMPI_DESERT_SAND_PASS_TO_YAMPI_DESERT_TRAINERS_WHIP_CHEST, YampiDesertTrainersWhipChest, YAMPI_DESERT_TRAINERS_WHIP_CHEST_FROM_YAMPI_DESERT_SAND_PASS),
    SCENE_EXITS(YampiDesertTrainersWhipChest),
    SCENE_EXIT(YAMPI_DESERT_TRAINERS_WHIP_CHEST_TO_YAMPI_DESERT_SAND_PASS, YampiDesertSandPass, YAMPI_DESERT_SAND_PASS_FROM_YAMPI_DESERT_TRAINERS_WHIP_CHEST),
    SCENE_EXIT(YAMPI_DESERT_TRAINERS_WHIP_CHEST_TO_WORLD_MAP, WorldMap, WORLD_MAP_FROM_YAMPI_DESERT_TRAINERS_WHIP_CHEST),
    SCENE_EXIT(YAMPI_DESERT_TRAINERS_WHIP_CHEST_TO_YAMPI_DESERT_HIDDEN_PSYNERGY_STONE, YampiDesertHiddenPsynergyStone, YAMPI_DESERT_HIDDEN_PSYNERGY_STONE_FROM_YAMPI_DESERT_TRAINERS_WHIP_CHEST),
    SCENE_EXIT(YAMPI_DESERT_TRAINERS_WHIP_CHEST_TO_YAMPI_DESERT_CAVE_WATER_OF_LIFE_CHEST, YampiDesertCaveWaterOfLifeChest, YAMPI_DESERT_CAVE_WATER_OF_LIFE_CHEST_FROM_YAMPI_DESERT_TRAINERS_WHIP_CHEST),
    SCENE_EXITS(YampiDesertHiddenPsynergyStone),
    SCENE_EXIT(YAMPI_DESERT_HIDDEN_PSYNERGY_STONE_TO_YAMPI_DESERT_TRAINERS_WHIP_CHEST, YampiDesertTrainersWhipChest, YAMPI_DESERT_TRAINERS_WHIP_CHEST_FROM_YAMPI_DESERT_HIDDEN_PSYNERGY_STONE),
    SCENE_EXIT(YAMPI_DESERT_HIDDEN_PSYNERGY_STONE_TO_WORLD_MAP, WorldMap, WORLD_MAP_FROM_YAMPI_DESERT_HIDDEN_PSYNERGY_STONE),
    SCENE_EXITS(YampiDesertScorpionPit),
    SCENE_EXIT(YAMPI_DESERT_SCORPION_PIT_TO_YAMPI_DESERT_KING_SCORPION, YampiDesertKingScorpion, YAMPI_DESERT_KING_SCORPION_FROM_YAMPI_DESERT_SCORPION_PIT),
    SCENE_EXITS(YampiDesertSpringCave),
    SCENE_EXIT(YAMPI_DESERT_SPRING_CAVE_TO_YAMPI_DESERT_SPRING_NORTH, YampiDesertSpring, YAMPI_DESERT_SPRING_FROM_YAMPI_DESERT_SPRING_CAVE_NORTH),
    SCENE_EXIT(SPRING_CAVE_TO_LOWER_CAVE, YampiDesertSpringCave, SPRING_CAVE_FROM_CHAMBER),
    SCENE_EXIT(YAMPI_DESERT_SPRING_CAVE_TO_YAMPI_DESERT_SPRING_SOUTH, YampiDesertSpring, YAMPI_DESERT_SPRING_FROM_YAMPI_DESERT_SPRING_CAVE_SOUTH),
    SCENE_EXIT(SPRING_CAVE_TO_CHAMBER, YampiDesertSpringCave, SPRING_CAVE_FROM_LOWER_CAVE),
    SCENE_EXITS(YampiDesertBlowMaceChest),
    SCENE_EXIT(YAMPI_DESERT_BLOW_MACE_CHEST_TO_YAMPI_DESERT_COINS_PUDDLE, YampiDesertCoinsPuddle, YAMPI_DESERT_COINS_PUDDLE_FROM_YAMPI_DESERT_BLOW_MACE_CHEST),
    SCENE_EXIT(YAMPI_DESERT_BLOW_MACE_CHEST_TO_YAMPI_DESERT_SAND_PASS, YampiDesertSandPass, YAMPI_DESERT_SAND_PASS_FROM_YAMPI_DESERT_BLOW_MACE_CHEST),
    SCENE_EXITS_END,
};

/// The actors of the rooms that place none.
LATE_DATA struct ActorPlacement gYampiDesert_Placements[] = {
    {SCENE_TABLE_END},
};

/// The Guardian Ring chest's room's actors.
LATE_DATA struct ActorPlacement gYampiDesertGuardianRingChestActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_STAKE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 296, 0, 216, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STAKE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 424, 0, 168, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STAKE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 456, 0, 136, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STAKE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 616, 0, 168, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STAKE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 632, 0, 392, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STAKE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 680, 0, 424, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

/// The spring's room's actors before the party meets the travelers.
LATE_DATA struct ActorPlacement gYampiDesertSpringActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_MADRAN_SOLDIER, CONDITION_ALWAYS, (s32)gYampiDesert_TravelerLooksAround, 200, 0, 216, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_MADRAN_SOLDIER, CONDITION_ALWAYS, (s32)gYampiDesert_OtherTravelerLooksAround, 216, 0, 200, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_WITH_CURLY_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 152, 0, 200, FACING_SOUTH_SOUTHEAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_RUFFIAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 162, 0, 232, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_IN_GREY_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 212, 0, 246, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_MADRAS_ELDER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 230, 0, 248, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_MADRAS_MAYOR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 234, 0, 230, FACING_EAST, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

/// The spring's room's actors once the party has met the travelers.
LATE_DATA struct ActorPlacement gYampiDesertSpringActorPlacementsAfterMeeting[] = {
    ACTOR_PLACEMENT(SPRITE_MADRAN_SOLDIER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 520, 0, 242, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_MADRAN_SOLDIER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 533, 0, 271, FACING_WEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_WITH_CURLY_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 435, 0, 323, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_RUFFIAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 467, 0, 248, FACING_SOUTH_SOUTHWEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_IN_GREY_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 462, 0, 340, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_MADRAS_ELDER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 482, 0, 340, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_MADRAS_MAYOR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 489, 0, 320, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    {SCENE_TABLE_END},
};

/// Blitz's room's actors.
LATE_DATA struct ActorPlacement gYampiDesertBlitzActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_STAKE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 216, 0, 184, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STAKE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 376, 0, 216, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STAKE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 216, 0, 344, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STAKE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 152, 0, 376, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STAKE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 168, 0, 520, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STAKE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 296, 0, 456, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STAKE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 328, 0, 456, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STAKE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 344, 0, 552, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STAKE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 296, 0, 616, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STAKE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 504, 0, 376, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STAKE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 536, 0, 728, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_JUPITER_DJINNI, FLAG_DJINNI(DJINNI_BLITZ), MOVEMENT_TYPE_STAND, 504, 0, 568, FACING_SOUTH, TALK_FACING_PARTY, 0),
    {SCENE_TABLE_END},
};

/// The Psynergy Stone's room's actors.
LATE_DATA struct ActorPlacement gYampiDesertPsynergyStoneActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_STAKE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 136, 0, 568, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STAKE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 232, 0, 600, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STAKE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 296, 0, 568, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STAKE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 184, 0, 232, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STAKE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 184, 0, 184, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STAKE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 296, 0, 200, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_STAKE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 168, 0, 264, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_STAKE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 280, 0, 280, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_KING_SCORPION, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 232, 0, 200, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SAND_RIPPLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

/// The King Scorpion's room's actors.
LATE_DATA struct ActorPlacement gYampiDesertKingScorpionActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_STAKE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 232, 0, 728, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STAKE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 200, 0, 296, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STAKE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 280, 0, 344, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STAKE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 232, 0, 648, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STAKE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 184, 0, 696, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_STAKE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 168, 0, 728, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_STAKE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 216, 0, 760, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_STAKE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 328, 0, 664, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_STAKE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 296, 0, 712, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_STAKE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 312, 0, 744, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_STAKE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 296, 0, 776, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_STAKE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 280, 0, 776, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_STAKE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_KING_SCORPION, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SAND_RIPPLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_BROWN_BOULDER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

/// The Trainer's Whip chest's room's actors.
LATE_DATA struct ActorPlacement gYampiDesertTrainersWhipChestActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 408, 0, 360, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 360, 0, 360, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_SIGN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 768, 0, 616, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 824, 0, 440, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 824, 0, 424, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

/// The scorpion's pit's actors.
LATE_DATA struct ActorPlacement gYampiDesertScorpionPitActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_KING_SCORPION, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

/// The Blow Mace chest's room's actors.
LATE_DATA struct ActorPlacement gYampiDesertBlowMaceChestActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_STAKE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 520, 0, 152, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 488, 0, 184, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 504, 0, 184, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 520, 0, 184, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 520, 0, 200, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 536, 0, 216, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 584, 0, 264, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 776, 0, 264, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    {SCENE_TABLE_END},
};

/// The triggers of the rooms that have none.
LATE_DATA struct Trigger gYampiDesert_Triggers[] = {
    {TRIGGER_TABLE_END},
};

/// The Guardian Ring chest's room's triggers.
LATE_DATA struct Trigger gYampiDesertGuardianRingChestTriggers[] = {
    {TRIGGER_EXIT, YAMPI_DESERT_GUARDIAN_RING_CHEST_TO_WORLD_MAP, CONDITION_ALWAYS, YAMPI_DESERT_GUARDIAN_RING_CHEST_TO_WORLD_MAP},
    {TRIGGER_EXIT, YAMPI_DESERT_GUARDIAN_RING_CHEST_TO_YAMPI_DESERT_SPRING, CONDITION_ALWAYS, YAMPI_DESERT_GUARDIAN_RING_CHEST_TO_YAMPI_DESERT_SPRING},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), STAKE(0), FLAG_YAMPI_DESERT_STAKE_POUNDED(0), EVENT(YampiDesert_PoundGuardianRingChestStake)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), STAKE(1), FLAG_YAMPI_DESERT_STAKE_POUNDED(1), EVENT(YampiDesert_PoundGuardianRingChestStake)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), STAKE(2), FLAG_YAMPI_DESERT_STAKE_POUNDED(2), EVENT(YampiDesert_PoundGuardianRingChestStake)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), STAKE(3), FLAG_YAMPI_DESERT_STAKE_POUNDED(3), EVENT(YampiDesert_PoundGuardianRingChestStake)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), STAKE(4), FLAG_YAMPI_DESERT_STAKE_POUNDED(4), EVENT(YampiDesert_PoundGuardianRingChestStake)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), STAKE(5), FLAG_YAMPI_DESERT_STAKE_POUNDED(5), EVENT(YampiDesert_PoundGuardianRingChestStake)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_AFTER_CAST), 0, CONDITION_ALWAYS, EVENT(YampiDesert_CastSand)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(YampiDesert_EndSand)},
    {TRIGGER_TABLE_END},
};

/// The spring's room's triggers.
LATE_DATA struct Trigger gYampiDesertSpringTriggers[] = {
    {TRIGGER_EXIT, YAMPI_DESERT_SPRING_TO_YAMPI_DESERT_GUARDIAN_RING_CHEST, CONDITION_ALWAYS, YAMPI_DESERT_SPRING_TO_YAMPI_DESERT_GUARDIAN_RING_CHEST},
    {TRIGGER_EXIT, YAMPI_DESERT_SPRING_TO_YAMPI_DESERT_COINS_PUDDLE, CONDITION_ALWAYS, YAMPI_DESERT_SPRING_TO_YAMPI_DESERT_COINS_PUDDLE},
    {TRIGGER_EXIT, YAMPI_DESERT_SPRING_TO_YAMPI_DESERT_SPRING_CAVE_NORTH, CONDITION_ALWAYS, YAMPI_DESERT_SPRING_TO_YAMPI_DESERT_SPRING_CAVE_NORTH},
    {TRIGGER_EXIT, YAMPI_DESERT_SPRING_TO_YAMPI_DESERT_BLITZ, CONDITION_ALWAYS, YAMPI_DESERT_SPRING_TO_YAMPI_DESERT_BLITZ},
    {STEP_TRIGGER_WALKING, CELL_AROUND_LADDER_PUDDLE, CONDITION_ALWAYS, EVENT(YampiDesert_ClimbRevealedLadder)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_AFTER_CAST), 0, CONDITION_ALWAYS, EVENT(YampiDesert_CastSand)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(YampiDesert_EndSand)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_SCOOP, PSYNERGY_PHASE_ACTS), CELL_LADDER_PUDDLE, FLAG_YAMPI_DESERT_LADDER_DUG_UP, EVENT(YampiDesert_ScoopLadderPit)},
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
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TRAVELER_WHO_WOULD_TURN_BACK | TRIGGER_SUBJECT_OWN_EFFECT, FLAG_YAMPI_DESERT_ELDER_INSISTED, EVENT(YampiDesert_TalkToElderAboutWay)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TRAVELER_WHO_WOULD_TURN_BACK, FLAG_YAMPI_DESERT_TRAVELERS_MET, MSGNO(MsgWeVeReached)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TRAVELER_WHO_WOULD_TURN_BACK, CONDITION_ALWAYS, MSGNO(MsgHowMuchLonger)},
    {TRIGGER_TALK, ACTOR_ELDER, FLAG_YAMPI_DESERT_TRAVELERS_MET, EVENT(YampiDesert_TalkToElder)},
    {TRIGGER_TALK, ACTOR_ELDER, CONDITION_ALWAYS, MSGNO(MsgMySonHas)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ELDER | TRIGGER_SUBJECT_OWN_EFFECT, FLAG_YAMPI_DESERT_ELDER_INSISTED, EVENT(YampiDesert_TalkToElder)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ELDER, FLAG_YAMPI_DESERT_TRAVELERS_MET, MSGNO(MsgMySonIs)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ELDER, CONDITION_ALWAYS, MSGNO(MsgRegardlessOfWhat)},
    {TRIGGER_TALK, ACTOR_MAYOR, FLAG_YAMPI_DESERT_TRAVELERS_MET, MSGNO(MsgFatherHowAm)},
    {TRIGGER_TALK, ACTOR_MAYOR, CONDITION_ALWAYS, MSGNO(MsgIfWeRemain)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAYOR, FLAG_YAMPI_DESERT_TRAVELERS_MET, MSGNO(MsgMyFatherS)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAYOR, CONDITION_ALWAYS, MSGNO(MsgHoooICan)},
    {TRIGGER_STEP, CELL_LEDGE_OVER_SPRING, FLAG_YAMPI_DESERT_TRAVELERS_MET, EVENT(YampiDesert_TravelersLookUp)},
    {TRIGGER_TABLE_END},
};

/// Blitz's room's triggers.
LATE_DATA struct Trigger gYampiDesertBlitzTriggers[] = {
    {TRIGGER_EXIT, YAMPI_DESERT_BLITZ_TO_YAMPI_DESERT_SPRING, CONDITION_ALWAYS, YAMPI_DESERT_BLITZ_TO_YAMPI_DESERT_SPRING},
    {TRIGGER_EXIT, YAMPI_DESERT_BLITZ_TO_YAMPI_DESERT_PSYNERGY_STONE, CONDITION_ALWAYS, YAMPI_DESERT_BLITZ_TO_YAMPI_DESERT_PSYNERGY_STONE},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), STAKE(0), FLAG_YAMPI_DESERT_STAKE_POUNDED(0), EVENT(YampiDesert_PoundBlitzStake)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), STAKE(1), FLAG_YAMPI_DESERT_STAKE_POUNDED(1), EVENT(YampiDesert_PoundBlitzStake)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), STAKE(2), FLAG_YAMPI_DESERT_STAKE_POUNDED(2), EVENT(YampiDesert_PoundBlitzStake)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), STAKE(3), FLAG_YAMPI_DESERT_STAKE_POUNDED(3), EVENT(YampiDesert_PoundBlitzStake)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), STAKE(4), FLAG_YAMPI_DESERT_STAKE_POUNDED(4), EVENT(YampiDesert_PoundBlitzStake)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), STAKE(5), FLAG_YAMPI_DESERT_STAKE_POUNDED(5), EVENT(YampiDesert_PoundBlitzStake)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), STAKE(6), FLAG_YAMPI_DESERT_STAKE_POUNDED(6), EVENT(YampiDesert_PoundBlitzStake)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), STAKE(7), FLAG_YAMPI_DESERT_STAKE_POUNDED(7), EVENT(YampiDesert_PoundBlitzStake)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), STAKE(8), FLAG_YAMPI_DESERT_STAKE_POUNDED(8), EVENT(YampiDesert_PoundBlitzStake)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), STAKE(9), FLAG_YAMPI_DESERT_STAKE_POUNDED(9), EVENT(YampiDesert_PoundBlitzStake)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), STAKE(10), FLAG_YAMPI_DESERT_STAKE_POUNDED(10), EVENT(YampiDesert_PoundBlitzStake)},
    {TRIGGER_STEP, CELL_ONTO_STAKE_TOPS, FLAG_YAMPI_DESERT_ON_STAKE_TOPS, EVENT(YampiDesert_StepOntoStakeTops)},
    {TRIGGER_STEP, CELL_OFF_STAKE_TOPS, IF_FLAG_SET(FLAG_YAMPI_DESERT_ON_STAKE_TOPS), EVENT(YampiDesert_StepOffStakeTops)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_AFTER_CAST), 0, CONDITION_ALWAYS, EVENT(YampiDesert_CastSand)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(YampiDesert_EndSand)},
    {TRIGGER_TALK, ACTOR_BLITZ, CONDITION_ALWAYS, EVENT(YampiDesert_FightBlitz)},
    {TRIGGER_TABLE_END},
};

/// The Psynergy Stone's room's triggers.
LATE_DATA struct Trigger gYampiDesertPsynergyStoneTriggers[] = {
    {TRIGGER_EXIT, YAMPI_DESERT_PSYNERGY_STONE_TO_YAMPI_DESERT_BLITZ, CONDITION_ALWAYS, YAMPI_DESERT_PSYNERGY_STONE_TO_YAMPI_DESERT_BLITZ},
    {TRIGGER_EXIT, EXIT_TO_NORTH_PART, CONDITION_ALWAYS, EXIT_TO_NORTH_PART},
    {TRIGGER_EXIT, EXIT_TO_SOUTH_PART, CONDITION_ALWAYS, EXIT_TO_SOUTH_PART},
    {TRIGGER_EXIT, YAMPI_DESERT_PSYNERGY_STONE_TO_YAMPI_DESERT_KING_SCORPION, CONDITION_ALWAYS, YAMPI_DESERT_PSYNERGY_STONE_TO_YAMPI_DESERT_KING_SCORPION},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), STAKE(0), FLAG_YAMPI_DESERT_STAKE_POUNDED(0), EVENT(YampiDesert_PoundPsynergyStoneStake)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), STAKE(1), FLAG_YAMPI_DESERT_STAKE_POUNDED(1), EVENT(YampiDesert_PoundPsynergyStoneStake)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), STAKE(2), FLAG_YAMPI_DESERT_STAKE_POUNDED(2), EVENT(YampiDesert_PoundPsynergyStoneStake)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), STAKE(3), FLAG_YAMPI_DESERT_STAKE_POUNDED(3), EVENT(YampiDesert_PoundPsynergyStoneStake)},
    {TRIGGER_STEP, CELL_ONTO_STAKE_TOPS, FLAG_YAMPI_DESERT_ON_STAKE_TOPS, EVENT(YampiDesert_StepOntoStakeTops)},
    {TRIGGER_STEP, CELL_OFF_STAKE_TOPS, IF_FLAG_SET(FLAG_YAMPI_DESERT_ON_STAKE_TOPS), EVENT(YampiDesert_StepOffStakeTops)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_AFTER_CAST), 0, CONDITION_ALWAYS, EVENT(YampiDesert_CastSand)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(YampiDesert_EndSand)},
    {TRIGGER_RAISED, RAISED_WATCH_SCORPION_STOP, CONDITION_ALWAYS, EVENT(YampiDesert_WatchScorpionStop)},
    {TRIGGER_RAISED, RAISED_STIR_SCORPION, CONDITION_ALWAYS, EVENT(YampiDesert_StirScorpion)},
    {TRIGGER_TABLE_END},
};

/// The King Scorpion's room's triggers.
LATE_DATA struct Trigger gYampiDesertKingScorpionTriggers[] = {
    {TRIGGER_EXIT, YAMPI_DESERT_KING_SCORPION_TO_YAMPI_DESERT_PSYNERGY_STONE, CONDITION_ALWAYS, YAMPI_DESERT_KING_SCORPION_TO_YAMPI_DESERT_PSYNERGY_STONE},
    {TRIGGER_EXIT, EXIT_TO_NORTH_PART, CONDITION_ALWAYS, EXIT_TO_NORTH_PART},
    {TRIGGER_EXIT, EXIT_TO_SOUTH_PART, CONDITION_ALWAYS, EXIT_TO_SOUTH_PART},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), STAKE(0), FLAG_YAMPI_DESERT_STAKE_POUNDED(0), EVENT(YampiDesert_PoundKingScorpionStake)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), STAKE(1), FLAG_YAMPI_DESERT_STAKE_POUNDED(1), EVENT(YampiDesert_PoundKingScorpionStake)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), STAKE(2), FLAG_YAMPI_DESERT_STAKE_POUNDED(2), EVENT(YampiDesert_PoundKingScorpionStake)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_AFTER_CAST), 0, CONDITION_ALWAYS, EVENT(YampiDesert_CastSand)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(YampiDesert_EndSand)},
    {TRIGGER_RAISED, RAISED_WATCH_SCORPION_STOP, FLAG_YAMPI_DESERT_SCORPION_CORNERED, EVENT(YampiDesert_WatchScorpionStop)},
    {TRIGGER_RAISED, RAISED_STIR_SCORPION_AGAIN, CONDITION_ALWAYS, EVENT(YampiDesert_StirScorpionAgain)},
    {TRIGGER_RAISED, RAISED_CORNER_SCORPION, CONDITION_ALWAYS, EVENT(YampiDesert_CornerScorpion)},
    {TRIGGER_STEP, CELL_BEFORE_TRAP, FLAG_KING_SCORPION_BEATEN, EVENT(YampiDesert_TrapKingScorpion)},
    {TRIGGER_TABLE_END},
};

/// The triggers of the room north of the spring, with the 315 coins.
LATE_DATA struct Trigger gYampiDesertCoinsPuddleTriggers[] = {
    {TRIGGER_EXIT, YAMPI_DESERT_COINS_PUDDLE_TO_YAMPI_DESERT_SPRING, CONDITION_ALWAYS, YAMPI_DESERT_COINS_PUDDLE_TO_YAMPI_DESERT_SPRING},
    {TRIGGER_EXIT, YAMPI_DESERT_COINS_PUDDLE_TO_YAMPI_DESERT_BLOW_MACE_CHEST, CONDITION_ALWAYS, YAMPI_DESERT_COINS_PUDDLE_TO_YAMPI_DESERT_BLOW_MACE_CHEST},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_AFTER_CAST), 0, CONDITION_ALWAYS, EVENT(YampiDesert_CastSand)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(YampiDesert_EndSand)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_SCOOP, PSYNERGY_PHASE_ACTS), CELL_COINS_PUDDLE, FLAG_YAMPI_DESERT_COINS_PUDDLE_DUG_UP, EVENT(YampiDesert_DigUpCoins)},
    {TRIGGER_TABLE_END},
};

/// The sand pass's triggers.
LATE_DATA struct Trigger gYampiDesertSandPassTriggers[] = {
    {TRIGGER_EXIT, YAMPI_DESERT_SAND_PASS_TO_YAMPI_DESERT_BLOW_MACE_CHEST, CONDITION_ALWAYS, YAMPI_DESERT_SAND_PASS_TO_YAMPI_DESERT_BLOW_MACE_CHEST},
    {TRIGGER_EXIT, YAMPI_DESERT_SAND_PASS_TO_YAMPI_DESERT_TRAINERS_WHIP_CHEST, CONDITION_ALWAYS, YAMPI_DESERT_SAND_PASS_TO_YAMPI_DESERT_TRAINERS_WHIP_CHEST},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_AFTER_CAST), 0, CONDITION_ALWAYS, EVENT(YampiDesert_CastSand)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(YampiDesert_EndSand)},
    {TRIGGER_TABLE_END},
};

/// The Trainer's Whip chest's room's triggers.
LATE_DATA struct Trigger gYampiDesertTrainersWhipChestTriggers[] = {
    {TRIGGER_EXIT, YAMPI_DESERT_TRAINERS_WHIP_CHEST_TO_YAMPI_DESERT_SAND_PASS, CONDITION_ALWAYS, YAMPI_DESERT_TRAINERS_WHIP_CHEST_TO_YAMPI_DESERT_SAND_PASS},
    {TRIGGER_EXIT, YAMPI_DESERT_TRAINERS_WHIP_CHEST_TO_WORLD_MAP, CONDITION_ALWAYS, YAMPI_DESERT_TRAINERS_WHIP_CHEST_TO_WORLD_MAP},
    {TRIGGER_EXIT, YAMPI_DESERT_TRAINERS_WHIP_CHEST_TO_YAMPI_DESERT_HIDDEN_PSYNERGY_STONE, CONDITION_ALWAYS, YAMPI_DESERT_TRAINERS_WHIP_CHEST_TO_YAMPI_DESERT_HIDDEN_PSYNERGY_STONE},
    {TRIGGER_EXIT, YAMPI_DESERT_TRAINERS_WHIP_CHEST_TO_YAMPI_DESERT_CAVE_WATER_OF_LIFE_CHEST, CONDITION_ALWAYS, YAMPI_DESERT_TRAINERS_WHIP_CHEST_TO_YAMPI_DESERT_CAVE_WATER_OF_LIFE_CHEST},
    {PSYNERGY_ON_ACTOR(ABILITY_LASH), ACTOR_ROPE_POST, FLAG_YAMPI_DESERT_ROPE_TIED, 0},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_AFTER_CAST), 0, CONDITION_ALWAYS, EVENT(YampiDesert_CastSand)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(YampiDesert_EndSand)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0, CONDITION_ALWAYS, EVENT(YampiDesert_RevealRockHint)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(YampiDesert_HideRockHint)},
    {TRIGGER_TALK, ACTOR_SIGN, CONDITION_ALWAYS, EVENT(YampiDesert_ReadSign)},
    {TRIGGER_STEP, CELL_FLOW_CORNER_INSIDE, IF_FLAG_SET(FLAG_YAMPI_DESERT_BLOCKS_SHOWN), EVENT(YampiDesert_HideBlocks)},
    {TRIGGER_STEP, CELL_FLOW_CORNER_OUTSIDE, FLAG_YAMPI_DESERT_BLOCKS_SHOWN, EVENT(YampiDesert_ShowBlocks)},
    {TRIGGER_TABLE_END},
};

/// The triggers of the room of the hidden Psynergy Stone.
LATE_DATA struct Trigger gYampiDesertHiddenPsynergyStoneTriggers[] = {
    {TRIGGER_EXIT, YAMPI_DESERT_HIDDEN_PSYNERGY_STONE_TO_YAMPI_DESERT_TRAINERS_WHIP_CHEST, CONDITION_ALWAYS, YAMPI_DESERT_HIDDEN_PSYNERGY_STONE_TO_YAMPI_DESERT_TRAINERS_WHIP_CHEST},
    {TRIGGER_EXIT, YAMPI_DESERT_HIDDEN_PSYNERGY_STONE_TO_WORLD_MAP, CONDITION_ALWAYS, YAMPI_DESERT_HIDDEN_PSYNERGY_STONE_TO_WORLD_MAP},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0, CONDITION_ALWAYS, EVENT(YampiDesert_RevealPsynergyStone)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(YampiDesert_HidePsynergyStone)},
    {TRIGGER_TABLE_END},
};

/// The scorpion's pit's triggers.
LATE_DATA struct Trigger gYampiDesertScorpionPitTriggers[] = {
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_AFTER_CAST), 0, CONDITION_ALWAYS, EVENT(YampiDesert_CastSand)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(YampiDesert_EndSand)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_SCOOP, PSYNERGY_PHASE_ACTS), CELL_BETWEEN_FOUR_ROCKS, FLAG_YAMPI_DESERT_ROCK_PIT_DUG_UP, EVENT(YampiDesert_OpenGeyser)},
    {TRIGGER_TABLE_END},
};

/// The triggers of the cave under the spring.
LATE_DATA struct Trigger gYampiDesertSpringCaveTriggers[] = {
    {TRIGGER_EXIT, YAMPI_DESERT_SPRING_CAVE_TO_YAMPI_DESERT_SPRING_NORTH, CONDITION_ALWAYS, YAMPI_DESERT_SPRING_CAVE_TO_YAMPI_DESERT_SPRING_NORTH},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 2, CONDITION_ALWAYS, EVENT(YampiDesert_ClimbDownSpringCaveLadder)},
    {TRIGGER_TABLE_END},
};

LATE_DATA struct Trigger gYampiDesertBlowMaceChestTriggers[] = {
    {TRIGGER_EXIT, YAMPI_DESERT_BLOW_MACE_CHEST_TO_YAMPI_DESERT_COINS_PUDDLE, CONDITION_ALWAYS, YAMPI_DESERT_BLOW_MACE_CHEST_TO_YAMPI_DESERT_COINS_PUDDLE},
    {TRIGGER_EXIT, YAMPI_DESERT_BLOW_MACE_CHEST_TO_YAMPI_DESERT_SAND_PASS, CONDITION_ALWAYS, YAMPI_DESERT_BLOW_MACE_CHEST_TO_YAMPI_DESERT_SAND_PASS},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), STAKE(0), FLAG_YAMPI_DESERT_STAKE_POUNDED(0), EVENT(YampiDesert_PoundBlowMaceChestStake)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_AFTER_CAST), 0, CONDITION_ALWAYS, EVENT(YampiDesert_CastSand)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(YampiDesert_EndSand)},
    {TRIGGER_TABLE_END},
};

/// A puff of the King Scorpion's dust drifts aside and frees itself.
LATE_DATA u32 gYampiDesert_SandCloudDrifts[] = {
    SC_WAIT(1),
    SC_MOVE(232, 0, 186),
    SC_SYNC,
    SC_RELEASE,
};
// clang-format on
