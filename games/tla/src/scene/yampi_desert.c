/* @file */
/* The Yampi Desert, between southern Osenia and Alhafra: twelve rooms on one */
/* overlay (SceneId_YampiDesertGuardianRingChest to */
/* SceneId_YampiDesertBlowMaceChest). In the first room Pound sinks the wooden
 */
/* stake before the Guardian Ring's chest. In the second, travelers from Madra
 */
/* chasing Briggs rest by the spring, beside a sand puddle that hides a ladder.
 */
/* In Blitz's room the stakes are Pounded into the sand or walked across on */
/* their tops, to the Antidote's chest and up to Blitz. Past the Psynergy */
/* Stone the King Scorpion moves under the sand: Pound on a stake makes it */
/* burst out and flee north, twice, until the stakes of the trap corner it in */
/* the ring of rocks at the north of its room. Walking up to it, Felix is */
/* dragged down with it by its Scoop into its pit and fights it for the Scoop */
/* Gem; Scoop between the pit's four rocks then opens a geyser out. Back at the
 */
/* spring, Scoop opens the ladder, whose cave leads up to a ledge over the */
/* travelers, who then find their way north; the next room hides 315 coins in */
/* a sand puddle, and the cave beyond it, all slides of flowing sand, holds the
 */
/* Hard Nut and the Blow Mace. In the long room after it Reveal changes two */
/* rings of rocks, one to a hint and one to the Lucky Medal's chest, and Lash */
/* ties the rope to the Trainer's Whip's chest; Reveal also brings out a */
/* Psynergy Stone in the room that leads on from there to the World Map. */
/* Sand lets Felix travel under the sand, past rocks and standing stakes, in */
/* most of the rooms. */
#include "item_ids.h"
#include "macros.h"
#include "version.h"
#include "field/actor_script.h"
#include "field/call.h"
#include "field/field_sequence.h"
#include "game/game.h"
#include "graphics/glyph.h"
#include "scene/scene.h"
#include "game/system_call.h"
#include "text/msg_ids.h"

#include "../field/actor_set_sprite_priority.c"

#include "../field/field_effect_move.c"

#define EFFECT_SET_MODE	  fdActorSelectAnimation
#define EFFECT_SET_SCRIPT fdActorStartScript
#include "../field/field_effect_spawn.c"

/* Set as Pound on a stake of the Psynergy Stone's room stirs the King Scorpion
 */
/* out of the sand, and it flees into the King Scorpion's room. */
#define FLAG_YAMPI_DESERT_SCORPION_STIRRED 0x889
/* As FLAG_YAMPI_DESERT_SCORPION_STIRRED, in the King Scorpion's room, from */
/* whose south part it flees north. */
#define FLAG_YAMPI_DESERT_SCORPION_STIRRED_AGAIN 0x88a
/* Set once the King Scorpion is cornered at the ring of rocks at the north end
 */
/* of its room. */
#define FLAG_YAMPI_DESERT_SCORPION_CORNERED 0x88b
/* Set once the King Scorpion is beaten; it leaves the Scoop Gem. */
#define FLAG_KING_SCORPION_BEATEN 0x88c
/* Set once Scoop has opened the sand pit that hides the ladder. */
#define FLAG_YAMPI_DESERT_LADDER_DUG_UP 0x88d
/* Set once the travelers' elder has said he will not turn back. */
#define FLAG_YAMPI_DESERT_ELDER_INSISTED 0x88f
/* Set once Scoop has opened the sand pit between the four rocks. */
#define FLAG_YAMPI_DESERT_ROCK_PIT_DUG_UP 0x8ed
/* Set once the party has seen the travelers from the ledge above them; they */
/* stand about the spring from then on. */
#define FLAG_YAMPI_DESERT_TRAVELERS_MET 0x8f0
/* Set while the leader walks on the tops of the stakes Pound has not sunk. */
#define FLAG_YAMPI_DESERT_ON_STAKE_TOPS FLAG_SCENE_TEMP_0B
/* Set while the unseen blocks by the sand's flow are shown. */
#define FLAG_YAMPI_DESERT_BLOCKS_SHOWN FLAG_SCENE_TEMP_0D
/* Set once Lash has tied the Trainer's Whip chest's room's rope. */
#define FLAG_YAMPI_DESERT_ROPE_TIED FLAG_SCENE_TEMP_0C
/* Set once Scoop has dug up the 315 coins in the room north of the spring. */
#define FLAG_YAMPI_DESERT_COINS_PUDDLE_DUG_UP FLAG_AREA_TEMP_02

extern u16 YampiDesertGuardianRingChestStakes[];
extern u16 YampiDesertBlitzStakes[];
extern u16 YampiDesertPsynergyStoneStakes[];
extern u16 YampiDesertKingScorpionStakes[];
extern u16 YampiDesertBlowMaceChestStakes[];
extern s32 YampiDesert_RaisedEvent;

/* The triggers the scene raises through YampiDesert_RaisedEvent while the */
/* King Scorpion is chased through the sand. */
typedef enum YAMPI_DESERT_RAISED_TRIGGER {
	RAISED_STIR_SCORPION = 200,
	RAISED_STIR_SCORPION_AGAIN = 201,
	RAISED_CORNER_SCORPION = 202,
	RAISED_WATCH_SCORPION_STOP = 210
} YAMPI_DESERT_RAISED_TRIGGER;

/* The cell behaviors the desert's code reads: sand that flows north, south, */
/* west or east and carries the leader, the rim round each stream, which */
/* carries nobody, and a rise to the upper level. */
typedef enum YAMPI_DESERT_CELL_BEHAVIOR {
	CELL_BEHAVIOR_YAMPI_DESERT_FLOW_NORTH = 10,
	CELL_BEHAVIOR_YAMPI_DESERT_FLOW_SOUTH = 11,
	CELL_BEHAVIOR_YAMPI_DESERT_FLOW_WEST = 12,
	CELL_BEHAVIOR_YAMPI_DESERT_FLOW_EAST = 13,
	CELL_BEHAVIOR_YAMPI_DESERT_FLOW_RIM = 14,
	CELL_BEHAVIOR_YAMPI_DESERT_RISE = 20
} YAMPI_DESERT_CELL_BEHAVIOR;

/* The ways between the two parts of the Psynergy Stone's room and of the King
 */
/* Scorpion's, each part seen through its own camera box: the exit from the */
/* south part into the north and back, and the entrances they arrive by. */
typedef enum YAMPI_DESERT_ROOM_PART {
	EXIT_TO_NORTH_PART = 2,
	EXIT_TO_SOUTH_PART = 3,
	ENTRANCE_FROM_NORTH_PART = 2, /* arriving in the south part */
	ENTRANCE_FROM_SOUTH_PART = 3  /* arriving in the north part */
} YAMPI_DESERT_ROOM_PART;

/* The ways between the small chamber of the cave under the spring, where the */
/* ladder from the spring comes down, and the cave below it. */
typedef enum YAMPI_DESERT_SPRING_CAVE_LEVEL {
	SPRING_CAVE_TO_LOWER_CAVE = 2, /* the chamber's ladder down */
	SPRING_CAVE_TO_CHAMBER = 4,
	SPRING_CAVE_FROM_CHAMBER = 4,
	SPRING_CAVE_FROM_LOWER_CAVE = 2
} YAMPI_DESERT_SPRING_CAVE_LEVEL;

/* The cells the rooms' trigger tables answer, by the behaviour each room's */
/* map gives them. */
typedef enum YAMPI_DESERT_CELL {
	/* The rooms with stakes: stepping from the high ground beside the stakes,
	 */
	/* and back down off them. */
	CELL_ONTO_STAKE_TOPS = 11,
	CELL_OFF_STAKE_TOPS = 10,
	/* The spring's room: the ring of cells round the sand puddle, the puddle */
	/* that hides the ladder, and the ledge over the spring. */
	CELL_AROUND_LADDER_PUDDLE = 5,
	CELL_LADDER_PUDDLE = 10,
	CELL_LEDGE_OVER_SPRING = 15,
	/* The room north of the spring: the sand puddle over the 315 coins. */
	CELL_COINS_PUDDLE = 12,
	/* The King Scorpion's room: before the ring of rocks of the trap. */
	CELL_BEFORE_TRAP = 10,
	/* The King Scorpion's pit: the sand between its four rocks. */
	CELL_BETWEEN_FOUR_ROCKS = 10,
	/* The Trainer's Whip's chest's room: the inner and outer corner of the */
	/* flowing sand by the invisible blocks. */
	CELL_FLOW_CORNER_INSIDE = 7,
	CELL_FLOW_CORNER_OUTSIDE = 8
} YAMPI_DESERT_CELL;

void scYampiDesertStepOntoStakeTops(void);
void scYampiDesertOpenLadderPit(void);
void scYampiDesertSetBlocksShown(s32 show);
void scYampiDesertHideBlocks(void);
void scActorsSetDrawn(s32 show, s32 first, u32 count);
void scYampiDesertUpdateSandFlow(void);
void scYampiDesertCheckUpperLevel(void);
void scYampiDesertOpenScoopHole(void);
void fdActorHide(s32 id);
void scYampiDesertRestoreBlocks(void);
void scYampiDesertSetUpBlowMaceChestBlocks(void);
void scYampiDesertClimbDownLadder(ACTOR *actor, s32 x, s32 z, s32 exit);
void scYampiDesertLayerStakesWithLeader(void);
void scYampiDesertPlaceStakes(const void *script);
void fdPoundSinkTarget(u16 *list, s32 mode, s32 count);
void scYampiDesertElderRefusesToQuit(void);
void scYampiDesertBlockPoundedStakes(u16 *list);
void scYampiDesertOpenStakeCells(u16 *list);
void syFieldEventWait(s32 frames);
ACTOR *scYampiDesertCreateScoopGem(void);
extern ACTOR *YampiDesert_SandRipple;
void scYampiDesertClearRaisedEvent(void);
void scYampiDesertPlayScorpionRumble(void);
void scYampiDesertSetUpPsynergyStoneScorpion(void);
void scYampiDesertSetStakeSunk(s32 id);
void scYampiDesertCheckUpperLevelFromLower(void);
void fdActorSetCellOpen(s32 id, u32 kind, s32 flag, s32 layer);

/* Places an actor at (x, z) on the ground, facing a direction. */
static inline void scYampiDesertSetPosAndFacing(s32 actorId, s32 x, s32 z,
												u16 facing) {
	fdActorSetHorizontalPosAndAngWithTerrain(actorId, x, z, facing);
}

/* Places a rope between two posts. */
static inline void scYampiDesertPlaceRope(s32 index, s32 flag, s32 post,
										  s32 otherPost) {
	fdRopePlace(index, flag, post, otherPost);
}

/* Scoop digs up a buried treasure: with `mode` RETURN_HOME_DUG_UP, the hidden
 */
/* item comes up at its home and its flag is set (fdActorReturnHome). */
static inline void scYampiDesertDigUpTreasure(s32 mode, s32 actorId, s32 flag) {
	fdActorReturnHome(mode, actorId, flag);
}

/* The stakes Pound sinks, the first rows of the rooms that have them (the */
/* Guardian Ring chest's, Blitz's, the Psynergy Stone's, the King Scorpion's */
/* and the Blow Mace chest's): stake n is the placed actor STAKE(n), and */
/* FLAG_YAMPI_DESERT_STAKE_POUNDED(n) is set once Pound has sunk it. */
#define STAKE(n)						   (ACTOR_FIRST_PLACED + (n))
#define FLAG_YAMPI_DESERT_STAKE_POUNDED(n) (FLAG_SCENE_TEMP_00 + (n))
/* A row of a room's stake list, as fdPoundSinkTarget and fdActorSetCellOpen */
/* read it: the stake, its look (0, wood) and its flag. A list ends at */
/* ACTOR_LIST_END. */
#define STAKE_ROW(n) STAKE(n), 0, FLAG_YAMPI_DESERT_STAKE_POUNDED(n)

/* The travelers from Madra resting by the spring (SceneId_YampiDesertSpring),
 */
/* by the rows of YampiDesertSpringActorPlacements and */
/* YampiDesertSpringActorPlacementsAfterMeeting, chasing Briggs to Alhafra. */
typedef enum YAMPI_DESERT_TRAVELERS_ACTOR_ID {
	/* "We followed the pirates here from Madra... Are you going to Alhafra?" */
	ACTOR_TRAVELER_FOLLOWING_THE_PIRATES = ACTOR_FIRST_PLACED,
	/* Who is to find evidence that Piers is a Champa. */
	ACTOR_TRAVELER_SEEKING_EVIDENCE,
	/* "Ooooh... This desert... It's so hot out here! I feel dizzy!" */
	ACTOR_DIZZY_TRAVELER,
	/* "Water... My throat is so parched!" */
	ACTOR_PARCHED_TRAVELER,
	/* Who would forget about Briggs: "Can't we just forget about Briggs?" */
	ACTOR_TRAVELER_WHO_WOULD_TURN_BACK,
	/* Madra's elder, who will not stop looking for Briggs. */
	ACTOR_ELDER,
	/* Madra's mayor, the elder's son, who must get them through the desert. */
	ACTOR_MAYOR
} YAMPI_DESERT_TRAVELERS_ACTOR_ID;

/* Blitz's room's last stake, standing at the foot of the cliff before a */
/* climbable wall, which shows once Pound has sunk it. */
#define ACTOR_CLIMBABLE_WALL_STAKE STAKE(10)
/* Blitz, the Jupiter Djinni, after Blitz's room's stakes. */
#define ACTOR_BLITZ STAKE(11)

/* The King Scorpion and the ripple of sand that follows it while it is */
/* buried: in the Psynergy Stone's room after its eight stakes, in the King */
/* Scorpion's after its thirteen. */
typedef enum YAMPI_DESERT_SCORPION_ACTOR_ID {
	ACTOR_PSYNERGY_STONE_ROOM_KING_SCORPION = STAKE(8),
	ACTOR_PSYNERGY_STONE_ROOM_SAND_RIPPLE,
	ACTOR_KING_SCORPION_ROOM_KING_SCORPION = STAKE(13),
	ACTOR_KING_SCORPION_ROOM_SAND_RIPPLE,
	/* Unused: a brown boulder placed at the origin, which nothing moves, so it
	 */
	/* is never on stage. */
	ACTOR_KING_SCORPION_ROOM_BOULDER
} YAMPI_DESERT_SCORPION_ACTOR_ID;

/* The Trainer's Whip chest's room's actors */
/* (SceneId_YampiDesertTrainersWhipChest). */
typedef enum YAMPI_DESERT_TRAINERS_WHIP_CHEST_ACTOR_ID {
	/* The posts of the rope Lash ties. */
	ACTOR_ROPE_POST = ACTOR_FIRST_PLACED,
	ACTOR_ROPE_OTHER_POST,
	ACTOR_SIGN,
	/* Two unseen blocks by the sand's flow, shown and hidden by the step */
	/* triggers beside it (scYampiDesertSetBlocksShown). */
	ACTOR_WHIP_CHEST_ROOM_FIRST_BLOCK,
	ACTOR_WHIP_CHEST_ROOM_SECOND_BLOCK
} YAMPI_DESERT_TRAINERS_WHIP_CHEST_ACTOR_ID;

/* The scorpion pit's actors (SceneId_YampiDesertScorpionPit), where the King */
/* Scorpion is fought. */
typedef enum YAMPI_DESERT_SCORPION_PIT_ACTOR_ID {
	ACTOR_KING_SCORPION = ACTOR_FIRST_PLACED,
	/* The unseen point the geyser rises from, moved to the middle of the four
	 */
	/* rocks once Scoop has dug the pit there; scGeyserAdd takes its shadow off.
	 */
	ACTOR_GEYSER
} YAMPI_DESERT_SCORPION_PIT_ACTOR_ID;

/* The room's hidden item: the Lucky Medal's chest in the Trainer's Whip's */
/* chest's room, the Psynergy Stone of the room after it, and the 315 coins */
/* under the sand puddle north of the spring. */
enum { ACTOR_HIDDEN_TREASURE = FIELD_TREASURE_ACTOR };

/* The Blow Mace chest's room's actors after its stake */
/* (SceneId_YampiDesertBlowMaceChest): seven unseen blocks by the sand's flow,
 */
/* hidden as the room is set up. */
typedef enum YAMPI_DESERT_BLOW_MACE_CHEST_ACTOR_ID {
	ACTOR_BLOW_MACE_CHEST_ROOM_FIRST_BLOCK = STAKE(1),
	ACTOR_BLOW_MACE_CHEST_ROOM_LAST_BLOCK =
		ACTOR_BLOW_MACE_CHEST_ROOM_FIRST_BLOCK + 6
} YAMPI_DESERT_BLOW_MACE_CHEST_ACTOR_ID;

void scYampiDesertRippleFollowsScorpion(void);

/* The points of YampiDesert_SandCloudPoints. */
#define SAND_CLOUD_POINTS 15

extern u16 YampiDesert_SandCloudPoints[SAND_CLOUD_POINTS * 2];

extern u32 YampiDesert_SandCloudDrifts[];

void scYampiDesertScorpionUsesScoop(void);
/* The centre of a cell's column or row, in pixels. */
s32 scYampiDesertCellCenter(s32 cell) __attribute__((const));
extern u32 YampiDesert_FlowDust[];
void scYampiDesertSetUpGuardianRingChest(void);
void scYampiDesertSetUpSpring(void);
void scYampiDesertSetUpBlitz(void);
void scYampiDesertSetUpPsynergyStone(void);
void scYampiDesertSetUpKingScorpion(void);

void scYampiDesertSetUpCoinsPuddle(void);

void scYampiDesertSetUpTrainersWhipChest(void);
void scYampiDesertSetUpHiddenPsynergyStone(void);
void scYampiDesertSetUpScorpionPit(void);
void scYampiDesertSetUpSpringCave(void);
void scYampiDesertSetUpBlowMaceChest(void);

void scYampiDesertDropScoopGem(void);

void scYampiDesertTalkToElder(void);

void scYampiDesertReadSign(void);

void scYampiDesertStepOffStakeTops(void);

void scYampiDesertCastSand(void);

void scYampiDesertEndSand(void);

void scYampiDesertWatchScorpionStop(void);

void scYampiDesertStirScorpion(void);

void scYampiDesertStirScorpionAgain(void);

void scYampiDesertCornerScorpion(void);

void scYampiDesertTrapKingScorpion(void);

void scYampiDesertRevealRockHint(void);

void scYampiDesertHideRockHint(void);

void scYampiDesertRevealPsynergyStone(void);

void scYampiDesertShowBlocks(void);

void scYampiDesertClimbRevealedLadder(void);
void scYampiDesertClimbDownSpringCaveLadder(void);

extern u32 YampiDesert_TravelerLooksAround[];

extern u32 YampiDesert_OtherTravelerLooksAround[];

extern SCENE_ENTRANCE SceneEntrances[];
extern SCENE_ENTRANCE YampiDesertSpringCaveEntrances[];
extern s16 YampiDesertScorpionPitExitRegions[];
extern s16 YampiDesertSpringCaveExitRegions[];

/* Talking to Blitz, the Jupiter Djinni at the top of the cliff in its room, */
/* starts its battle. */
void scYampiDesertFightBlitz(void) {
	fdRequestDjinnBattle(ACTOR_BLITZ, DJINNI_BLITZ);
}

/* Where the party can enter: the cave under the spring has its own entrances.
 */
const u8 *scGetEntrances(void) {
	if(MainRoutineStateWork.next == SCENE_YAMPI_DESERT_SPRING_CAVE)
		return YampiDesertSpringCaveEntrances;
	return SceneEntrances;
}

/* The regions of the rooms whose exits are boxes: the scorpion pit and the */
/* cave under the spring. */
const u8 *scGetExitRegions(void) {
	s16 scene = MainRoutineStateWork.next;

	if(scene == SCENE_YAMPI_DESERT_SCORPION_PIT)
		return YampiDesertScorpionPitExitRegions;
	if(scene == SCENE_YAMPI_DESERT_SPRING_CAVE)
		return YampiDesertSpringCaveExitRegions;
	return 0;
}

const u32 *scGetExits(void) { return SceneExits; }

extern ACTOR_PLACE YampiDesert_Placements[];
extern ACTOR_PLACE YampiDesertGuardianRingChestActorPlacements[];
extern ACTOR_PLACE YampiDesertSpringActorPlacements[];
extern ACTOR_PLACE YampiDesertSpringActorPlacementsAfterMeeting[];
extern ACTOR_PLACE YampiDesertBlitzActorPlacements[];
extern ACTOR_PLACE YampiDesertPsynergyStoneActorPlacements[];
extern ACTOR_PLACE YampiDesertKingScorpionActorPlacements[];
extern ACTOR_PLACE YampiDesertTrainersWhipChestActorPlacements[];
extern ACTOR_PLACE YampiDesertScorpionPitActorPlacements[];
extern ACTOR_PLACE YampiDesertBlowMaceChestActorPlacements[];

/* The current room's actors; in the spring's room, the travelers stand about */
/* the spring once the party has seen them. */
const ACTOR_PLACE *scGetActorPlacements(void) {
	s16 scene = MainRoutineStateWork.next;

	if(scene == SCENE_YAMPI_DESERT_GUARDIAN_RING_CHEST)
		return YampiDesertGuardianRingChestActorPlacements;
	if(scene == SCENE_YAMPI_DESERT_SPRING) {
		if(syGameFlagGet(FLAG_YAMPI_DESERT_TRAVELERS_MET))
			return YampiDesertSpringActorPlacementsAfterMeeting;
		return YampiDesertSpringActorPlacements;
	}
	if(scene == SCENE_YAMPI_DESERT_BLITZ)
		return YampiDesertBlitzActorPlacements;
	if(scene == SCENE_YAMPI_DESERT_PSYNERGY_STONE)
		return YampiDesertPsynergyStoneActorPlacements;
	if(scene == SCENE_YAMPI_DESERT_KING_SCORPION)
		return YampiDesertKingScorpionActorPlacements;
	if(scene == SCENE_YAMPI_DESERT_TRAINERS_WHIP_CHEST)
		return YampiDesertTrainersWhipChestActorPlacements;
	if(scene == SCENE_YAMPI_DESERT_SCORPION_PIT)
		return YampiDesertScorpionPitActorPlacements;
	if(scene == SCENE_YAMPI_DESERT_BLOW_MACE_CHEST)
		return YampiDesertBlowMaceChestActorPlacements;
	return YampiDesert_Placements;
}

/* Scoop on the sand puddle on the east side of the room north of the spring */
/* (cell (24, 23)) digs up 315 coins: "Felix got 315 coins." */
void scYampiDesertDigUpCoins(s32 mode) {
	scYampiDesertDigUpTreasure(mode, ACTOR_HIDDEN_TREASURE,
							   FLAG_YAMPI_DESERT_COINS_PUDDLE_DUG_UP);
}

extern TRIGGER YampiDesert_Triggers[];
extern TRIGGER YampiDesertGuardianRingChestTriggers[];
extern TRIGGER YampiDesertSpringTriggers[];
extern TRIGGER YampiDesertBlitzTriggers[];
extern TRIGGER YampiDesertPsynergyStoneTriggers[];
extern TRIGGER YampiDesertKingScorpionTriggers[];
extern TRIGGER YampiDesertCoinsPuddleTriggers[];
extern TRIGGER YampiDesertSandPassTriggers[];
extern TRIGGER YampiDesertTrainersWhipChestTriggers[];
extern TRIGGER YampiDesertHiddenPsynergyStoneTriggers[];
extern TRIGGER YampiDesertScorpionPitTriggers[];
extern TRIGGER YampiDesertSpringCaveTriggers[];
extern TRIGGER YampiDesertBlowMaceChestTriggers[];

/* The current room's triggers. */
const TRIGGER *scGetTriggers(void) {
	s16 scene = MainRoutineStateWork.next;

	if(scene == SCENE_YAMPI_DESERT_GUARDIAN_RING_CHEST)
		return YampiDesertGuardianRingChestTriggers;
	if(scene == SCENE_YAMPI_DESERT_SPRING)
		return YampiDesertSpringTriggers;
	if(scene == SCENE_YAMPI_DESERT_BLITZ)
		return YampiDesertBlitzTriggers;
	if(scene == SCENE_YAMPI_DESERT_PSYNERGY_STONE)
		return YampiDesertPsynergyStoneTriggers;
	if(scene == SCENE_YAMPI_DESERT_KING_SCORPION)
		return YampiDesertKingScorpionTriggers;
	if(scene == SCENE_YAMPI_DESERT_COINS_PUDDLE)
		return YampiDesertCoinsPuddleTriggers;
	if(scene == SCENE_YAMPI_DESERT_SAND_PASS)
		return YampiDesertSandPassTriggers;
	if(scene == SCENE_YAMPI_DESERT_TRAINERS_WHIP_CHEST)
		return YampiDesertTrainersWhipChestTriggers;
	if(scene == SCENE_YAMPI_DESERT_HIDDEN_PSYNERGY_STONE)
		return YampiDesertHiddenPsynergyStoneTriggers;
	if(scene == SCENE_YAMPI_DESERT_SCORPION_PIT)
		return YampiDesertScorpionPitTriggers;
	if(scene == SCENE_YAMPI_DESERT_SPRING_CAVE)
		return YampiDesertSpringCaveTriggers;
	if(scene == SCENE_YAMPI_DESERT_BLOW_MACE_CHEST)
		return YampiDesertBlowMaceChestTriggers;
	return YampiDesert_Triggers;
}

/* The traveler who followed the pirates from Madra asks whether the party is */
/* going to Alhafra too. */
void scYampiDesertTalkToTraveler(void) {
	fdEventStart();
	fdEventEndLastingPsynergy(0);
	fdEventSetMessageId((s32)MsgWeFollowedThe);
	/* "We followed the pirates here from Madra... Are you going to Alhafra, */
	/* too?" Yes: "I don't think you can get there through the desert..." No: */
	/* "You guys are lucky. We've gotta find some way through this desert." */
	fdEventAskYesNo(ACTOR_TRAVELER_FOLLOWING_THE_PIRATES, 0);
	fdEventEnd();
}

/* One traveler asks to forget about Briggs, and the elder refuses: there must
 */
/* be a way through the desert. */
void scYampiDesertElderRefusesToQuit(void) {
	fdEventSetMessageId((s32)MsgCanTWe);
	fdActorSetAnimationById(ACTOR_TRAVELER_WHO_WOULD_TURN_BACK,
							ANIM_SHAKE_HEAD);
	/* "Can't we just forget about Briggs?" */
	fdEventShowMessageAndWait(ACTOR_TRAVELER_WHO_WOULD_TURN_BACK, 0);
	fdActorFaceDirection(ACTOR_ELDER, FACING_WEST, 20);
	fdActorSetAnimationById(ACTOR_ELDER, ANIM_SHAKE_HEAD);
	/* "No. I'm not about to quit looking for him." */
	fdEventShowMessageAndWait(ACTOR_ELDER, 0);
	fdActorFaceDirectionById(ACTOR_ELDER, FACING_EAST, 20);
	fdActorFaceDirection(ACTOR_ELDER, FACING_SOUTH_SOUTHEAST, 20);
	fdActorFaceDirectionById(ACTOR_ELDER, FACING_EAST, 20);
	fdActorFaceDirection(ACTOR_ELDER, FACING_SOUTH_SOUTHEAST, 0);
	/* "There must be some way through this desert! We must press onward!" */
	fdEventShowMessageAndWait(ACTOR_ELDER, 0);
}

/* A traveler tells how insistent the elder is on finding a way through, and */
/* the elder refuses to quit. */
void scYampiDesertTalkToElderAboutWay(void) {
	fdEventStart();
	fdEventEndLastingPsynergy(0);
	if(!fdEventGetFlag(FLAG_YAMPI_DESERT_ELDER_INSISTED))
		scYampiDesertElderRefusesToQuit();
	fdActorFaceActorById(ACTOR_TRAVELER_WHO_WOULD_TURN_BACK,
						 MainRoutineStateWork.leaderActorId, 0);
	fdEventSetMessageId((s32)MsgAsYouCan);
	/* "As you can see, the elder is quite insistent on finding a way..." */
	fdEventShowMessageAndWait(ACTOR_TRAVELER_WHO_WOULD_TURN_BACK, 0);
	syGameFlagSet(FLAG_YAMPI_DESERT_ELDER_INSISTED);
	fdEventEnd();
}

/* The elder says this is where his son must prove himself as mayor. */
void scYampiDesertTalkToElder(void) {
	fdEventStart();
	fdEventEndLastingPsynergy(0);
	if(!fdEventGetFlag(FLAG_YAMPI_DESERT_ELDER_INSISTED))
		scYampiDesertElderRefusesToQuit();
	fdActorFaceActorById(ACTOR_ELDER, MainRoutineStateWork.leaderActorId, 0);
	fdEventSetMessageId((s32)MsgThisIsWhere);
	/* "This is where my son must prove himself as mayor... So, Son. How are..."
	 */
	fdEventShowMessageAndWait(ACTOR_ELDER, 0);
	syGameFlagSet(FLAG_YAMPI_DESERT_ELDER_INSISTED);
	fdEventEnd();
}

/* The sign in the Trainer's Whip chest's room reads: north to Alhafra, south */
/* to Air's Rock, with a warning about what lies beyond. */
void scYampiDesertReadSign(void) {
	fdEventStart();
	fdEventEndLastingPsynergy(0);
	/* "North: Alhafra South: Air's Rock Warning: Beyond this point, there..."
	 */
	mnFieldMessageShow((s32)MsgNorthAlhafraSouth, 1);
	fdEventEnd();
}

/* Pound on one of the wooden stakes of the desert's first room hammers it down
 */
/* into the sand; the one before the Guardian Ring's chest in the south-east */
/* corner opens the way to it. */
void scYampiDesertPoundGuardianRingChestStake(s32 mode, s32 count) {
	fdPoundSinkTarget(YampiDesertGuardianRingChestStakes, mode, count);
}

/* Pound on one of the wooden stakes in Blitz's room hammers it down into the */
/* sand. The last one stands before the climbable wall that leads up to Blitz;
 */
/* the one below the Antidote's chest is best left standing, as its top is the
 */
/* way across. */
void scYampiDesertPoundBlitzStake(s32 mode, s32 count) {
	fdPoundSinkTarget(YampiDesertBlitzStakes, mode, count);
}

/* Defines what Pound on one of a room's stakes does: it sinks */
/* (fdPoundSinkTarget), and once down it raises the watch for whether the King
 */
/* Scorpion is stopped. */
#define DEFINE_POUND_STAKE_STOPPING_SCORPION(name, stakes)                     \
	void name(s32 mode, s32 count) {                                           \
		fdPoundSinkTarget(stakes, mode, count);                                \
		if(mode == POUND_STAGE_LANDED) {                                       \
			if(YampiDesert_RaisedEvent == 0)                                   \
				YampiDesert_RaisedEvent = RAISED_WATCH_SCORPION_STOP;          \
		}                                                                      \
	}

/* Pound on one of the wooden stakes in the Psynergy Stone's room hammers it */
/* down into the sand. While the King Scorpion still roams under the sand, */
/* the camera then follows the ripple of sand over it until it stops. */
DEFINE_POUND_STAKE_STOPPING_SCORPION(YampiDesert_PoundPsynergyStoneStake,
									 YampiDesertPsynergyStoneStakes)

/* Pound on the wooden stake that blocks the way to the Blow Mace's chest, in */
/* the cave of sand slides, hammers it down into the sand. */
void scYampiDesertPoundBlowMaceChestStake(s32 mode, s32 count) {
	fdPoundSinkTarget(YampiDesertBlowMaceChestStakes, mode, count);
}

/* Pound on one of the wooden stakes in the King Scorpion's room hammers it */
/* down into the sand. Until the King Scorpion is cornered, the camera then */
/* follows the ripple of sand over it until it stops. */
DEFINE_POUND_STAKE_STOPPING_SCORPION(YampiDesert_PoundKingScorpionStake,
									 YampiDesertKingScorpionStakes)

/* Nothing visible happens when Felix steps from the high ground onto the */
/* cells beside the stakes in Blitz's room, the Psynergy Stone's or the King */
/* Scorpion's; the game makes the tops of the stakes still standing into */
/* squares he hops across at his height, so that a row of standing stakes is a
 */
/* way over the sand. */
/* Each stake's cell gives up its wall and takes the hop flag, and its height */
/* rises by four steps. */
void scYampiDesertStepOntoStakeTops(void) {
	MAP_STATE *map = Ram_HeapSlots->mapGrid;
	u16 *list = NULL;
	u8 *grid = MapShapeGrid;

	if(MainRoutineStateWork.next == SCENE_YAMPI_DESERT_BLITZ)
		list = YampiDesertBlitzStakes;
	else if(MainRoutineStateWork.next == SCENE_YAMPI_DESERT_PSYNERGY_STONE)
		list = YampiDesertPsynergyStoneStakes;
	else if(MainRoutineStateWork.next == SCENE_YAMPI_DESERT_KING_SCORPION)
		list = YampiDesertKingScorpionStakes;
	if(list == NULL)
		return;
	while(*list != ACTOR_LIST_END) {
		ACTOR *actor = fdActorGetById(*list);

		list += 2;
		if(!syGameFlagGet(*list)) {
			s32 layer = actor->layer;
			MAP_CELL *cells = map->layers[layer].cells;
			u8 *shapes = (u8 *)((((s32)cells - (s32)Ram_MapCellBuffer) >> 2) +
								(s32)grid);
			s32 index = fdActorGetCellIndex(actor);
			s32 shape =
				fdMapGridGetTerrainKind(layer, actor->x.fixed, actor->z.fixed);
			s32 height = CELL_HEIGHT_STEPS(
				fdMapGetTerrainHeight(layer, actor->x.fixed, actor->z.fixed));

			cells += index;
			shapes += index;
			actor->motionFlg = 0;
			cells->collisionCode = CELL_BEHAVIOR_NORMAL;
			cells->attributeB |= MAP_CELL_JUMP;
			*shapes = fdMapShapeFindOrAdd(layer, shape, height + 4);
		}
		list++;
	}
	syGameFlagSet(FLAG_YAMPI_DESERT_ON_STAKE_TOPS);
}

/* Nothing visible happens when Felix steps back down off the stakes' tops; the
 */
/* game makes the standing stakes walls again at ground level. */
void scYampiDesertStepOffStakeTops(void) {
	MAP_STATE *map = Ram_HeapSlots->mapGrid;
	u16 *list = NULL;
	u8 *grid = MapShapeGrid;

	if(MainRoutineStateWork.next == SCENE_YAMPI_DESERT_BLITZ)
		list = YampiDesertBlitzStakes;
	else if(MainRoutineStateWork.next == SCENE_YAMPI_DESERT_PSYNERGY_STONE)
		list = YampiDesertPsynergyStoneStakes;
	else if(MainRoutineStateWork.next == SCENE_YAMPI_DESERT_KING_SCORPION)
		list = YampiDesertKingScorpionStakes;
	if(list == NULL)
		return;
	for(; *list != ACTOR_LIST_END; list++) {
		ACTOR *actor = fdActorGetById(*list);

		list += 2;
		if(!syGameFlagGet(*list)) {
			s32 layer = actor->layer;
			MAP_CELL *cells = map->layers[layer].cells;
			u8 *shapes = (u8 *)((((s32)cells - (s32)Ram_MapCellBuffer) >> 2) +
								(s32)grid);
			s32 index = fdActorGetCellIndex(actor);
			s32 shape =
				fdMapGridGetTerrainKind(layer, actor->x.fixed, actor->z.fixed);
			s32 height = CELL_HEIGHT_STEPS(
				fdMapGetTerrainHeight(layer, actor->x.fixed, actor->z.fixed));

			cells += index;
			shapes += index;
			actor->motionFlg = 0;
			cells->collisionCode = CELL_BEHAVIOR_WALL;
			cells->attributeB &= ~MAP_CELL_JUMP;
			*shapes = fdMapShapeFindOrAdd(layer, shape, height - 4);
		}
	}
	syGameFlagClear(FLAG_YAMPI_DESERT_ON_STAKE_TOPS);
}

/* As Sand is cast, walls off the cell of each stake Pound has sunk, in the */
/* overhead layer Sand moves on. */
void scYampiDesertBlockPoundedStakes(u16 *list) {
	MAP_STATE *map = Ram_HeapSlots->mapGrid;

	for(; *list != ACTOR_LIST_END; list += 3) {
		ACTOR *actor = fdActorGetById(list[0]);

		if(syGameFlagGet(list[2])) {
			MAP_CELL *cell = &map->layers[MAPGRID_LAYER_OVERHEAD]
								  .cells[fdActorGetCellIndex(actor)];

			cell->collisionCode = CELL_BEHAVIOR_WALL;
		}
	}
}

/* As Sand ends, makes each stake's cell in the overhead layer sand again. */
void scYampiDesertOpenStakeCells(u16 *list) {
	MAP_STATE *map = Ram_HeapSlots->mapGrid;

	for(; *list != ACTOR_LIST_END; list += 3) {
		MAP_CELL *cell =
			&map->layers[MAPGRID_LAYER_OVERHEAD]
				 .cells[fdActorGetCellIndex(fdActorGetById(list[0]))];

		cell->collisionCode = CELL_BEHAVIOR_SAND;
	}
}

/* As Sand melts Felix into the sand, so that he can travel under it past */
/* rocks and standing stakes, the stakes Pound has hammered down still bar his
 */
/* way underground. */
/* He moves on the map's top layer while sunk, and each sunk stake's cell */
/* there is made a wall. */
void scYampiDesertCastSand(void) {
	s16 scene;

	fdActorGetById(MainRoutineStateWork.leaderActorId)->layer =
		MAPGRID_LAYER_OVERHEAD;
	scene = MainRoutineStateWork.next;
	if(scene == SCENE_YAMPI_DESERT_GUARDIAN_RING_CHEST)
		scYampiDesertBlockPoundedStakes(YampiDesertGuardianRingChestStakes);
	else if(scene == SCENE_YAMPI_DESERT_BLITZ)
		scYampiDesertBlockPoundedStakes(YampiDesertBlitzStakes);
	else if(scene == SCENE_YAMPI_DESERT_PSYNERGY_STONE)
		scYampiDesertBlockPoundedStakes(YampiDesertPsynergyStoneStakes);
	else if(scene == SCENE_YAMPI_DESERT_KING_SCORPION)
		scYampiDesertBlockPoundedStakes(YampiDesertKingScorpionStakes);
}

/* As Sand ends and Felix rises out of the sand, the sunk stakes' cells are */
/* sand again under him. */
void scYampiDesertEndSand(void) {
	ACTOR *leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
	s16 scene;

	leader->layer = MAPGRID_LAYER_GROUND;
	scene = MainRoutineStateWork.next;
	if(scene == SCENE_YAMPI_DESERT_GUARDIAN_RING_CHEST)
		scYampiDesertOpenStakeCells(YampiDesertGuardianRingChestStakes);
	else if(scene == SCENE_YAMPI_DESERT_BLITZ)
		scYampiDesertOpenStakeCells(YampiDesertBlitzStakes);
	else if(scene == SCENE_YAMPI_DESERT_PSYNERGY_STONE)
		scYampiDesertOpenStakeCells(YampiDesertPsynergyStoneStakes);
	else if(scene == SCENE_YAMPI_DESERT_KING_SCORPION)
		scYampiDesertOpenStakeCells(YampiDesertKingScorpionStakes);
}

/* After a stake is Pounded while the King Scorpion roams under the sand, */
/* the camera follows the ripple of sand over it until it stops, then */
/* returns to Felix. */
void scYampiDesertWatchScorpionStop(void) {
	s16 scene;

	fdEventStart();
	fdEventEndLastingPsynergy(0);
	scene = MainRoutineStateWork.next;
	if(scene == SCENE_YAMPI_DESERT_PSYNERGY_STONE) {
		fdCameraFollowActor(ACTOR_PSYNERGY_STONE_ROOM_KING_SCORPION, 1);
		fdActorWatchAndSetScript(MainRoutineStateWork.leaderActorId,
								 ACTOR_PSYNERGY_STONE_ROOM_KING_SCORPION);
	}
	else if(scene == SCENE_YAMPI_DESERT_KING_SCORPION) {
		fdCameraFollowActor(ACTOR_KING_SCORPION_ROOM_KING_SCORPION, 1);
		fdActorWatchAndSetScript(MainRoutineStateWork.leaderActorId,
								 ACTOR_KING_SCORPION_ROOM_KING_SCORPION);
	}
	while(YampiDesert_RaisedEvent == RAISED_WATCH_SCORPION_STOP)
		sySynch(1);
	fdEventEnd();
}

#include "../field/field_effect_advance_motion_priority.c"

#undef EFFECT_SET_MODE
#undef EFFECT_SET_SCRIPT
#undef DEFINE_POUND_STAKE_STOPPING_SCORPION

void scFieldEffectSpawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
						s32 velocityZ, u32 flags,
						const EFFECT_OPTIONS *options);

/* The grains of sand in a burst. */
#define SAND_BURST_GRAINS 17

/* A burst of sand at (x, y, z): its grains thrown east, south, west and north
 */
/* in turn, each a little off at random. */
void scYampiDesertBurstSand(s32 x, s32 y, s32 z) {
	EFFECT_OPTIONS options;
	u32 i;

	options.update = fdEffectAdvanceMotionAtLeaderPriority;
	options.startScaleX = FIXED_ONE * 8 / 10;
	options.startScaleY = FIXED_ONE * 8 / 10;
	for(i = 0; i < SAND_BURST_GRAINS; i++) {
		s32 velocity[3];

		velocity[0] = syCos(i * DEGtoANG(90));
		velocity[1] = 0;
		velocity[2] = sySin(i * DEGtoANG(90));
		velocity[0] += velocity[0] >> 1;
		velocity[0] += (u16)(syRandom() << 1) - toFIXED(1);
		velocity[2] += (u16)syRandom() - toFIXED(1) / 2;
		scFieldEffectSpawn(x, y, z, velocity[0], velocity[1], velocity[2],
						   EFFECT_USE_UPDATE | EFFECT_SCRIPT_MEDIUM, &options);
	}
}

/* When the King Scorpion, roaming under the sand of the Psynergy Stone's room,
 */
/* stops against the stake Pound has hammered down, it bursts out of the sand */
/* with a spray, scuttles east and burrows away into the next room. */
void scYampiDesertStirScorpion(void) {
	fdEventStart();
	fdEventEndLastingPsynergy(0);
	scYampiDesertClearRaisedEvent();
	fdActorWatchAndSetScript(MainRoutineStateWork.leaderActorId,
							 ACTOR_PSYNERGY_STONE_ROOM_KING_SCORPION);
	fdCameraSetSpeed(2.4);
	fdCameraMoveTo(300, CAMERA_KEEP, 248, 1);
	fdActorMoveToAndWaitById(ACTOR_PSYNERGY_STONE_ROOM_KING_SCORPION, 254, 248);
	fdActorMoveToAndWaitById(ACTOR_PSYNERGY_STONE_ROOM_KING_SCORPION, 352, 240);
	syCloseEvent(scYampiDesertPlayScorpionRumble);
	syFieldEventWait(10);
	syCloseEvent(scYampiDesertRippleFollowsScorpion);
	sySynch(1);
	fdActorSetPos(ACTOR_PSYNERGY_STONE_ROOM_SAND_RIPPLE, 0, 0);
	syFieldEventWait(40);
	scYampiDesertBurstSand(toFIXED(352), 0, toFIXED(248));
	sySndPlay(SE_KOLIMA_BRIDGE_HOP);
	fdActorGetById(ACTOR_PSYNERGY_STONE_ROOM_KING_SCORPION)->velY = toFIXED(6);
	fdActorSetSprCol(fdActorGetById(ACTOR_PSYNERGY_STONE_ROOM_KING_SCORPION),
					 SPRITE_COLOR_NORMAL);
	fdActorSetSprFlags(fdActorGetById(ACTOR_PSYNERGY_STONE_ROOM_KING_SCORPION),
					   SPRITE_FLAG_SHADOW);
	fdActorMoveToAndWaitById(ACTOR_PSYNERGY_STONE_ROOM_KING_SCORPION, 380, 248);
	fdActorStop(MainRoutineStateWork.leaderActorId);
	sySndPlay(SE_WATER_MOVE);
	fdActorSetSpeed(ACTOR_PSYNERGY_STONE_ROOM_KING_SCORPION, 0.8);
	fdActorWalkToAndWaitById(ACTOR_PSYNERGY_STONE_ROOM_KING_SCORPION, 428, 248);
	sySndPlay(SOUND_FADE_OUT_ACTION_SE);
	fdActorSetPos(ACTOR_PSYNERGY_STONE_ROOM_KING_SCORPION, 0, 0);
	syGameFlagSet(FLAG_YAMPI_DESERT_SCORPION_STIRRED);
	fdEventEnd();
}

/* When the King Scorpion, roaming under the sand of the south part of its */
/* own room, stops against the stake Pound has hammered down, it bursts out of
 */
/* the sand with a spray and scuttles north, burrowing away towards the trap. */
void scYampiDesertStirScorpionAgain(void) {
	fdEventStart();
	fdEventEndLastingPsynergy(0);
	scYampiDesertClearRaisedEvent();
	fdActorWatchAndSetScript(MainRoutineStateWork.leaderActorId,
							 ACTOR_KING_SCORPION_ROOM_KING_SCORPION);
	fdCameraSetSpeed(2.4);
	fdCameraMoveTo(246, CAMERA_KEEP, 616, 1);
	fdActorMoveToAndWait(ACTOR_KING_SCORPION_ROOM_KING_SCORPION, 248, 648);
	fdActorMoveToAndWait(ACTOR_KING_SCORPION_ROOM_KING_SCORPION, 248, 576);
	syCloseEvent(scYampiDesertPlayScorpionRumble);
	syFieldEventWait(10);
	syCloseEvent(scYampiDesertRippleFollowsScorpion);
	sySynch(1);
	fdActorSetPos(ACTOR_KING_SCORPION_ROOM_SAND_RIPPLE, 0, 0);
	syFieldEventWait(20);
	scYampiDesertBurstSand(toFIXED(248), 0, toFIXED(576));
	sySndPlay(SE_KOLIMA_BRIDGE_HOP);
	fdActorGetById(ACTOR_KING_SCORPION_ROOM_KING_SCORPION)->velY = toFIXED(6);
	fdActorSetSprCol(fdActorGetById(ACTOR_KING_SCORPION_ROOM_KING_SCORPION),
					 SPRITE_COLOR_NORMAL);
	fdActorSetSprFlags(fdActorGetById(ACTOR_KING_SCORPION_ROOM_KING_SCORPION),
					   SPRITE_FLAG_SHADOW);
	fdActorMoveToAndWaitById(ACTOR_KING_SCORPION_ROOM_KING_SCORPION, 256, 560);
	syFieldEventWait(20);
	fdActorStop(MainRoutineStateWork.leaderActorId);
	sySndPlay(SE_WATER_MOVE);
	fdActorSetSpeed(ACTOR_KING_SCORPION_ROOM_KING_SCORPION, 0.8);
	fdActorWalkToAndWait(ACTOR_KING_SCORPION_ROOM_KING_SCORPION, 248, 496);
	sySndPlay(SOUND_FADE_OUT_ACTION_SE);
	fdActorSetPos(ACTOR_KING_SCORPION_ROOM_KING_SCORPION, 0, 0);
	syGameFlagSet(FLAG_YAMPI_DESERT_SCORPION_STIRRED_AGAIN);
	fdEventEnd();
}

/* When the stakes of the trap have shut the King Scorpion's way, the camera */
/* follows its ripple of sand north into the ring of rocks at the north end of
 */
/* its room, where it stops, cornered. */
void scYampiDesertCornerScorpion(void) {
	fdEventStart();
	fdEventEndLastingPsynergy(0);
	scYampiDesertClearRaisedEvent();
	fdActorWatchAndSetScript(MainRoutineStateWork.leaderActorId,
							 ACTOR_KING_SCORPION_ROOM_KING_SCORPION);
	fdCameraSetSpeed(2.4);
	fdCameraMoveTo(232, CAMERA_KEEP, 328, 1);
	fdActorMoveToAndWait(ACTOR_KING_SCORPION_ROOM_KING_SCORPION, 232, 328);
	fdCameraMoveTo(232, CAMERA_KEEP, 200, 1);
	fdActorMoveToAndWait(ACTOR_KING_SCORPION_ROOM_KING_SCORPION, 232, 280);
	fdActorMoveToAndWaitById(ACTOR_KING_SCORPION_ROOM_KING_SCORPION, 232, 168);
	syCloseEvent(scYampiDesertPlayScorpionRumble);
	fdActorFaceDirectionById(ACTOR_KING_SCORPION_ROOM_KING_SCORPION,
							 FACING_SOUTH, 0);
	fdActorStop(MainRoutineStateWork.leaderActorId);
	syFieldEventWait(20);
	syGameFlagSet(FLAG_YAMPI_DESERT_SCORPION_CORNERED);
	fdEventEnd();
}

/* Raises a cloud of dust twice at each point of */
/* YampiDesert_SandCloudPoints, each moved by -2 to +1 pixels at random. */
void scYampiDesertRaiseSandCloud(void) {
	u32 pass;
	u32 i;

	for(pass = 0; pass <= 1; pass++) {
		u16 *point;

		point = YampiDesert_SandCloudPoints;
		for(i = 0; i < SAND_CLOUD_POINTS; i++) {
			s32 x = *point++ + syRandomBelow(4);
			s32 z = *point++ + syRandomBelow(4);
			ACTOR *actor = syImportActorCreate(
				SPRITE_DUST_CLOUD, INTtoFIXED(x - 2), 0, INTtoFIXED(z - 2));

			if(actor != NULL) {
				actor->speedLimit = toFIXED(9) / 8;
				actor->acceleration = toFIXED(1) / 2;
				fdActorSetSprFlags((ACTOR *)actor, 0);
				fdActorSelectAnimation((ACTOR *)actor, 1);
				fdActorStartScript(actor, YampiDesert_SandCloudDrifts);
			}
		}
	}
	fdMapGridCopyCellTilesIndicesRect(74, 71, 77, 9, 3, 3);
}

/* Walking up to the cornered King Scorpion in the ring of rocks at the north */
/* of its room (cell (14, 13)): its ripple of sand pulses, Felix stops with a */
/* question mark, and the scorpion bursts out before him and uses Scoop. Felix
 */
/* staggers back, raises his arms and sinks into the hole with it, and they */
/* drop into its pit below, where its battle begins. */
void scYampiDesertTrapKingScorpion(void) {
	ACTOR *leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
	ACTOR *scorpion = fdActorGetById(ACTOR_KING_SCORPION_ROOM_KING_SCORPION);
	u32 i;

	if(!syGameFlagGet(FLAG_YAMPI_DESERT_SCORPION_CORNERED))
		return;
	fdEventStart();
	fdEventEndLastingPsynergy(0);
	do {
		YampiDesert_SandRipple->scaleX -= FIXED_ONE * 8 / 100;
		YampiDesert_SandRipple->scaleY -= FIXED_ONE * 8 / 100;
		sySynch(1);
	} while(YampiDesert_SandRipple->scaleX >= FIXED_ONE * 8 / 10);
	do {
		YampiDesert_SandRipple->scaleX += FIXED_ONE / 10;
		YampiDesert_SandRipple->scaleY += FIXED_ONE / 10;
		sySynch(1);
	} while(YampiDesert_SandRipple->scaleX <= FIXED_ONE * 18 / 10);
	do {
		YampiDesert_SandRipple->scaleX -= FIXED_ONE * 4 / 100;
		YampiDesert_SandRipple->scaleY -= FIXED_ONE * 4 / 100;
		sySynch(1);
	} while(YampiDesert_SandRipple->scaleX >= FIXED_ONE * 2 / 10);
	fdActorSetPos(ACTOR_KING_SCORPION_ROOM_SAND_RIPPLE, 0, 0);
	syFieldEventWait(20);
	fdCameraMoveTo(232, CAMERA_KEEP, 196, 1);
	fdActorShowEmoteById(MainRoutineStateWork.leaderActorId,
						 EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdActorSetSpeed(MainRoutineStateWork.leaderActorId, 0.8);
	fdActorWalkToAndWaitById(MainRoutineStateWork.leaderActorId, 232, 164);
	syFieldEventWait(20);
	scYampiDesertSetPosAndFacing(ACTOR_KING_SCORPION_ROOM_KING_SCORPION,
								 toFIXED(232), toFIXED(184), FACING_SOUTH);
	sySynch(1);
	scYampiDesertBurstSand(toFIXED(232), 0, toFIXED(184));
	sySndPlay(SE_KOLIMA_BRIDGE_HOP);
	scorpion->velY = toFIXED(6);
	scorpion->priFlg &= ~ACTOR_PRIORITY_UNDERFOOT;
	fdActorSetSprPriority(ACTOR_KING_SCORPION_ROOM_KING_SCORPION, 2);
	fdActorSetSprCol(fdActorGetById(ACTOR_KING_SCORPION_ROOM_KING_SCORPION),
					 SPRITE_COLOR_NORMAL);
	fdActorSetSprFlags(fdActorGetById(ACTOR_KING_SCORPION_ROOM_KING_SCORPION),
					   SPRITE_FLAG_SHADOW);
	fdActorSetSpeed(ACTOR_KING_SCORPION_ROOM_KING_SCORPION, 1.6);
	fdActorMoveToAndWaitById(ACTOR_KING_SCORPION_ROOM_KING_SCORPION, 216, 184);
	fdActorFaceDirectionById(MainRoutineStateWork.leaderActorId, FACING_SOUTH,
							 0);
	fdActorFaceDirectionAndRefreshById(ACTOR_KING_SCORPION_ROOM_KING_SCORPION,
									   FACING_EAST);
	syFieldEventWait(20);
	scYampiDesertScorpionUsesScoop();
	sySndPlay(SE_WHOOSH);
	scYampiDesertRaiseSandCloud();
	fdActorSetSprFlags(fdActorGetById(MainRoutineStateWork.leaderActorId), 0);
	syFieldEventWait(20);
	fdEffectSetAttached(MainRoutineStateWork.leaderActorId,
						ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdActorSetAnimationById(MainRoutineStateWork.leaderActorId,
							ANIM_LEADER_WALK_BACKWARDS);
	syFieldEventWait(30);
	fdActorSetAnimationById(MainRoutineStateWork.leaderActorId,
							ANIM_LEADER_ARMS_RAISED);
	syFieldEventWait(10);
	sySndPlay(SE_DUST_PUFF);
	for(i = 0; i < 14; i++) {
		leader->scaleX -= FIXED_ONE / 64;
		leader->scaleY -= FIXED_ONE / 64;
		leader->y.fixed += -toFIXED(2);
		sySynch(1);
	}
	fdActorSetSprCol(fdActorGetById(MainRoutineStateWork.leaderActorId),
					 SPRITE_COLOR_HIDDEN);
	sySndPlay(SE_HIGH_JUMP);
	scorpion->velY = toFIXED(8);
	fdActorSetSprFlags(fdActorGetById(ACTOR_KING_SCORPION_ROOM_KING_SCORPION),
					   0);
	fdActorMoveToAndWaitById(ACTOR_KING_SCORPION_ROOM_KING_SCORPION, 232, 168);
	syFieldEventWait(3);
	sySndPlay(SE_DUST_PUFF);
	for(i = 0; i < 6; i++) {
		scorpion->scaleX -= FIXED_ONE / 32;
		scorpion->scaleY -= FIXED_ONE / 32;
		scorpion->y.fixed += -toFIXED(7);
		sySynch(1);
	}
	fdActorSetPos(ACTOR_KING_SCORPION_ROOM_KING_SCORPION, 0, 0);
	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
	fdEventFinishScrTransition();
	fdEventWaitTransitionFrames();
	syImportEventSetExitRequest(
		YAMPI_DESERT_KING_SCORPION_TO_YAMPI_DESERT_SCORPION_PIT);
}

/* Scoop on the sand between the four rocks in the middle of the King */
/* Scorpion's pit (cell (14, 12)) opens a geyser there, a column of water that
 */
/* rises and falls and carries the party up and out of the pit. */
void scYampiDesertOpenGeyser(s32 mode) {
	ACTOR *actor;
	s32 height;

	if(mode == SCOOP_STAGE_DUG) {
		actor = fdActorGetById(ACTOR_GEYSER);
		fdMapGridCopyCellTilesIndicesRect(35, 39, 13, 11, 3, 3);
		fdActorSetPos(ACTOR_GEYSER, 232, 200);
		height = fdMapGetTerrainHeight(MAPGRID_LAYER_GROUND, actor->x.fixed,
									   actor->z.fixed);
		actor->y.fixed = height;
		actor->groundHeight = height;
		syGameFlagSet(FLAG_YAMPI_DESERT_ROCK_PIT_DUG_UP);
	}
}

s32 scYampiDesertCellCenter(s32 cell) {
	return cell * MAPGRID_CELL_PIXELS + MAPGRID_CELL_PIXELS / 2;
}

/* Moves the leader to the centre of a cell, in pixels. */
#define MOVE_LEADER_TO(column, row)                                            \
	{                                                                          \
		s32 x_ = scYampiDesertCellCenter(column);                              \
		s32 z_ = scYampiDesertCellCenter(row);                                 \
		fdActorSetHorizontalPosWithTerrain(MainRoutineStateWork.leaderActorId, \
										   INTtoFIXED(x_), INTtoFIXED(z_));    \
	}

/* Reveal cast in the long room of the Trainer's Whip's chest changes two */
/* rings of rocks while it lasts: the first (cells (15, 40) to (19, 45)), three
 */
/* rows of three rocks, shifts into an arrow of rocks pointing north-west (a */
/* row along the top, a short column down its west end and a line running */
/* down to the south-east from their corner), and the second (cells (36, 25) */
/* to (40, 29)) shows the Lucky Medal's chest in its middle. If Felix stands */
/* where a rock now appears, he is moved to the nearest open cell. */
void scYampiDesertRevealRockHint(void) {
	ACTOR *leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
	ACTOR *actor = fdActorGetById(ACTOR_HIDDEN_TREASURE);
	s32 column = FIELD_CELL(leader->x.fixed);
	s32 row = FIELD_CELL(leader->z.fixed);

	if(actor != NULL)
		fdActorSetPos(ACTOR_HIDDEN_TREASURE, 616, 444);
	fdMapGridCopyCellTilesIndicesRect(82, 82, 15, 40, 5, 6);
	fdMapGridCopyCellAttributes(82, 82, 5, 6, 15, 40);
	fdMapGridCopyCellTilesIndicesRect(88, 82, 36, 25, 5, 5);
	fdMapGridCopyCellAttributes(88, 82, 5, 5, 36, 25);
	if(column == 15) {
		if(row == 41)
			MOVE_LEADER_TO(14, 41)
		else if(row == 43)
			MOVE_LEADER_TO(16, 43)
	}
	else if(column == 16) {
		if(row == 40)
			MOVE_LEADER_TO(16, 39)
		else if(row == 41)
			MOVE_LEADER_TO(16, 42)
	}
	else if(column == 18) {
		if(row == 40)
			MOVE_LEADER_TO(18, 41)
		else if(row == 43)
			MOVE_LEADER_TO(17, 43)
	}
}

#undef MOVE_LEADER_TO

/* Moves the leader to the centre of a cell, in pixels. */
#define MOVE_LEADER_TO(column, row)                                            \
	{                                                                          \
		MAIN_ROUTINE_STATE *state = &MainRoutineStateWork;                     \
		s32 x = scYampiDesertCellCenter(column);                               \
		s32 z = scYampiDesertCellCenter(row);                                  \
		fdActorSetHorizontalPosWithTerrain(state->leaderActorId,               \
										   INTtoFIXED(x), INTtoFIXED(z));      \
	}

/* As Reveal fades in the Trainer's Whip's chest's room, both rings of rocks */
/* go back as they were and the Lucky Medal's chest is hidden again; Felix is */
/* moved off any cell a rock returns to. */
void scYampiDesertHideRockHint(void) {
	ACTOR *leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
	s32 column = FIELD_CELL(leader->x.fixed);
	s32 row = FIELD_CELL(leader->z.fixed);

	if(fdActorGetById(ACTOR_HIDDEN_TREASURE) != NULL)
		fdActorSetPos(ACTOR_HIDDEN_TREASURE, 0, 0);
	fdMapGridCopyCellTilesIndicesRect(82, 89, 15, 40, 5, 6);
	fdMapGridCopyCellAttributes(82, 89, 5, 6, 15, 40);
	fdMapGridCopyCellTilesIndicesRect(88, 89, 36, 25, 5, 5);
	fdMapGridCopyCellAttributes(88, 89, 5, 5, 36, 25);
	if(column == 15 && row == 44)
		MOVE_LEADER_TO(15, 43)
	else if(column == 17 && row == 44)
		MOVE_LEADER_TO(17, 43)
	else if(column == 19) {
		if(row == 40)
			MOVE_LEADER_TO(19, 41)
		else if(row == 42)
			MOVE_LEADER_TO(18, 42)
	}
}

/* Reveal cast in the room that leads on from the Trainer's Whip's chest's room
 */
/* to the World Map brings out a hidden Psynergy Stone at cell (18, 15), */
/* moving Felix a cell south if he stands on its spot. Once Reveal fades the */
/* stone is gone again (scYampiDesertHidePsynergyStone), so it has to be */
/* touched while Reveal lasts. */
void scYampiDesertRevealPsynergyStone(void) {
	ACTOR *actor = fdActorGetById(ACTOR_HIDDEN_TREASURE);
	ACTOR *leader;

	if(actor != 0) {
		leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
		fdActorSetPos(ACTOR_HIDDEN_TREASURE, 288, 240);
		if(FIELD_CELL(leader->x.fixed + toFIXED(8)) ==
			   FIELD_CELL(actor->x.fixed + toFIXED(8)) &&
		   FIELD_CELL(leader->z.fixed + toFIXED(8)) ==
			   FIELD_CELL(actor->z.fixed + toFIXED(8)))
			fdActorSetPos(MainRoutineStateWork.leaderActorId, 288, 256);
	}
}

/* As Reveal fades, the hidden Psynergy Stone of the room that leads on to the
 */
/* World Map is put away again. */
void scYampiDesertHidePsynergyStone(void) {
	if(fdActorGetById(ACTOR_HIDDEN_TREASURE))
		fdActorSetPos(ACTOR_HIDDEN_TREASURE, 0, 0);
}

/* Opens the hole over the ladder in the spring's sand puddle. */
void scYampiDesertOpenLadderPit(void) {
	fdMapGridCopyCellTilesIndicesRect(64, 64, 72, 32, 1, 1);
	fdMapCopyCellAttributeRect(64, 64, 1, 1, 8, 32);
	fdMapCopyCellAttributeRect(64, 64, 1, 1, 8, 96);
}

/* Scoop on the oddly shaped sand puddle at the south of the travelers' */
/* spring's room (cell (8, 32)) digs it out to show a ladder going down. */
void scYampiDesertScoopLadderPit(s32 mode) {
	if(mode == SCOOP_STAGE_DUG) {
		scYampiDesertOpenLadderPit();
		syGameFlagSet(FLAG_YAMPI_DESERT_LADDER_DUG_UP);
	}
}

/* Shows or hides the unseen blocks of the Trainer's Whip chest's room or the */
/* Blow Mace chest's. */
void scYampiDesertSetBlocksShown(s32 show) {
	s16 scene = MainRoutineStateWork.next;

	if(scene == SCENE_YAMPI_DESERT_TRAINERS_WHIP_CHEST)
		scActorsSetDrawn(show, ACTOR_WHIP_CHEST_ROOM_FIRST_BLOCK, 2);
	else if(scene == SCENE_YAMPI_DESERT_BLOW_MACE_CHEST)
		scActorsSetDrawn(show, ACTOR_BLOW_MACE_CHEST_ROOM_FIRST_BLOCK,
						 ACTOR_BLOW_MACE_CHEST_ROOM_LAST_BLOCK -
							 ACTOR_BLOW_MACE_CHEST_ROOM_FIRST_BLOCK + 1);
}

/* Nothing visible happens when Felix steps onto the sand round the corner of */
/* the flowing sand in the Trainer's Whip's chest's room (cells (48, 23) to */
/* (51, 23) and (48, 24) to (48, 26)) after the inner corner has hidden the two
 */
/* blocks at cells (51, 26) and (51, 27): the game marks them placed again. */
/* The room's set-up hid both with fdActorHide, see-through and without */
/* collision, so placed or not they stop nothing and show nothing. */
void scYampiDesertShowBlocks(void) {
	scYampiDesertSetBlocksShown(1);
	syGameFlagSet(FLAG_YAMPI_DESERT_BLOCKS_SHOWN);
}

/* Nothing visible happens when Felix steps onto the inner corner of the */
/* flowing sand in the Trainer's Whip's chest's room (cells (49, 24) to */
/* (51, 24) and (49, 25)); the game takes the two invisible blocks at cells */
/* (51, 26) and (51, 27) away. */
void scYampiDesertHideBlocks(void) {
	scYampiDesertSetBlocksShown(0);
	syGameFlagClear(FLAG_YAMPI_DESERT_BLOCKS_SHOWN);
}

#include "shared/actors_set_hidden.c"

extern u32 YampiDesert_MayorRunsAhead[];
extern u32 YampiDesert_TravelersFollow[];
extern u32 YampiDesert_LastTravelersFollow[];

/* Stepping out onto the ledge above the travelers' spring (cells (15, 12) */
/* and (16, 12)), the party is seen from below: "Hey! Look! Up there!" The */
/* travelers wonder how they got up there, the mayor finds the way, and they */
/* all run off along it. */
void scYampiDesertTravelersLookUp(s32 p0) {
	MAIN_ROUTINE_STATE *state;

	Call1((void (*)())fdEventStart, p0);
	fdEventEndLastingPsynergy(0);
	fdEventSetMessageId((s32)MsgHeyLookUp);
	fdActorStop(ACTOR_TRAVELER_SEEKING_EVIDENCE);
	sySynch(1);
	fdActorShowEmote(ACTOR_TRAVELER_SEEKING_EVIDENCE,
					 EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdActorFaceDirection(ACTOR_TRAVELER_SEEKING_EVIDENCE, FACING_EAST, 20);
	/* "Hey! Look! Up there!" */
	fdEventShowMessageAndWait(ACTOR_TRAVELER_SEEKING_EVIDENCE, 0);
	fdActorStop(ACTOR_TRAVELER_FOLLOWING_THE_PIRATES);
	sySynch(1);
	fdActorFaceDirection(ACTOR_TRAVELER_FOLLOWING_THE_PIRATES, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_DIZZY_TRAVELER, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_PARCHED_TRAVELER, FACING_NORTH_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_TRAVELER_WHO_WOULD_TURN_BACK,
						 FACING_NORTH_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_ELDER, FACING_NORTH_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_MAYOR, FACING_NORTH_NORTHEAST, 0);
	state = &MainRoutineStateWork;
	scFieldActorShowEmote(state->leaderActorId,
						  EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdActorFaceDirection(state->leaderActorId, FACING_WEST, 0);
	((ACTOR *)fdCameraGetActor())->motionFlg = 0;
	fdCameraSetSpeed(1.6);
	fdCameraMoveTo(212, 0, 212, 1);
	fdCameraWaitForArrival();
	/* "How did those guys get up there?" */
	fdEventShowMessageAndWait(ACTOR_TRAVELER_WHO_WOULD_TURN_BACK, 0);
	fdActorFaceDirection(ACTOR_PARCHED_TRAVELER, FACING_EAST, 10);
	/* "Maybe there's a path up ahead that we missed..." */
	fdEventShowMessageAndWait(ACTOR_PARCHED_TRAVELER, 0);
	fdActorFaceDirection(ACTOR_ELDER, FACING_NORTH_NORTHWEST, 0);
	fdActorFaceDirection(ACTOR_MAYOR, FACING_SOUTH_SOUTHWEST, 20);
	fdActorJump(ACTOR_MAYOR, 4, 40);
	fdActorSetAnimationAndWait(ACTOR_ELDER, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_MAYOR, ANIM_NOD);
	fdActorWatchAndSetScript(ACTOR_TRAVELER_FOLLOWING_THE_PIRATES, ACTOR_MAYOR);
	fdActorWatchAndSetScript(ACTOR_TRAVELER_SEEKING_EVIDENCE, ACTOR_MAYOR);
	fdActorWatchAndSetScript(ACTOR_DIZZY_TRAVELER, ACTOR_MAYOR);
	fdActorWatchAndSetScript(ACTOR_PARCHED_TRAVELER, ACTOR_MAYOR);
	fdActorWatchAndSetScript(ACTOR_TRAVELER_WHO_WOULD_TURN_BACK, ACTOR_MAYOR);
	fdActorWatchAndSetScript(ACTOR_ELDER, ACTOR_MAYOR);
	fdActorSetSpeed(ACTOR_ELDER, 1);
	fdActorSetMovementTypeById(ACTOR_MAYOR, (s32)YampiDesert_MayorRunsAhead);
	fdCameraSetSpeed(0.8);
	fdCameraMoveTo(240, CAMERA_KEEP, 260, 1);
	fdCameraWaitForArrival();
	fdActorStop(ACTOR_TRAVELER_FOLLOWING_THE_PIRATES);
	fdActorStop(ACTOR_TRAVELER_SEEKING_EVIDENCE);
	fdActorStop(ACTOR_DIZZY_TRAVELER);
	fdActorStop(ACTOR_PARCHED_TRAVELER);
	fdActorStop(ACTOR_TRAVELER_WHO_WOULD_TURN_BACK);
	fdActorStop(ACTOR_ELDER);
	fdCameraSetSpeed(2.4);
	fdCameraMoveTo(212, CAMERA_KEEP, 212, 1);
	fdCameraWaitForArrival();
	syFieldEventWait(20);
	fdActorFaceDirection(ACTOR_TRAVELER_FOLLOWING_THE_PIRATES,
						 FACING_NORTH_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_TRAVELER_SEEKING_EVIDENCE,
						 FACING_SOUTH_SOUTHWEST, 20);
	/* "Maybe... Maybe we can get through the desert after all!" */
	fdEventShowMessageAndWait(ACTOR_TRAVELER_SEEKING_EVIDENCE, 0);
	fdActorSetAnimationAndWait(ACTOR_TRAVELER_FOLLOWING_THE_PIRATES, ANIM_NOD);
	/* "We can find a way through to Alhafra!" */
	fdEventShowMessageAndWait(ACTOR_TRAVELER_FOLLOWING_THE_PIRATES, 0);
	fdActorJump(ACTOR_DIZZY_TRAVELER, 4, 0);
	fdActorFaceDirection(ACTOR_DIZZY_TRAVELER, FACING_EAST, 0);
	fdActorSetAnimationById(ACTOR_DIZZY_TRAVELER, ANIM_SHAKE_HEAD);
	/* "I don't care where we go, as long as it's cool." */
	fdEventShowMessageAndWait(ACTOR_DIZZY_TRAVELER, 0);
	fdActorFaceDirection(ACTOR_TRAVELER_FOLLOWING_THE_PIRATES, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_TRAVELER_SEEKING_EVIDENCE, FACING_WEST, 0);
	fdActorShowEmote(ACTOR_TRAVELER_FOLLOWING_THE_PIRATES,
					 EMOTE_IN_FRONT | EMOTE_WINCE, 0);
	fdActorShowEmoteById(ACTOR_TRAVELER_SEEKING_EVIDENCE,
						 EMOTE_IN_FRONT | EMOTE_WINCE, 80);
	/* "Hey! I found it! I found a way through!" */
	fdEventShowMessageAndWait(ACTOR_MAYOR, 0);
	fdActorFaceDirection(ACTOR_TRAVELER_FOLLOWING_THE_PIRATES,
						 FACING_NORTH_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_DIZZY_TRAVELER, FACING_SOUTH_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_PARCHED_TRAVELER, FACING_NORTH_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_TRAVELER_WHO_WOULD_TURN_BACK, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_ELDER, FACING_WEST, 20);
	fdActorSetAnimationById(ACTOR_TRAVELER_FOLLOWING_THE_PIRATES, ANIM_NOD);
	fdActorSetAnimationById(ACTOR_TRAVELER_SEEKING_EVIDENCE, ANIM_NOD);
	fdActorSetAnimationById(ACTOR_DIZZY_TRAVELER, ANIM_NOD);
	fdActorSetAnimationById(ACTOR_PARCHED_TRAVELER, ANIM_NOD);
	fdActorSetAnimationById(ACTOR_TRAVELER_WHO_WOULD_TURN_BACK, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_ELDER, ANIM_NOD);
	fdActorSetSpeed(ACTOR_TRAVELER_FOLLOWING_THE_PIRATES, 1.6);
	fdActorSetSpeed(ACTOR_TRAVELER_SEEKING_EVIDENCE, 1.6);
	fdActorSetSpeed(ACTOR_DIZZY_TRAVELER, 1.6);
	fdActorSetSpeed(ACTOR_PARCHED_TRAVELER, 1.6);
	fdActorSetSpeed(ACTOR_TRAVELER_WHO_WOULD_TURN_BACK, 1.6);
	fdActorSetSpeed(ACTOR_ELDER, 1.6);
	fdActorWatchAndSetScript(state->leaderActorId, ACTOR_ELDER);
	{
		s32 path = (s32)YampiDesert_TravelersFollow;

		fdActorSetMovementTypeById(ACTOR_ELDER, path);
		syFieldEventWait(10);
		fdActorSetMovementTypeById(ACTOR_TRAVELER_WHO_WOULD_TURN_BACK, path);
		fdCameraSetSpeed(0.4);
		fdCameraMoveTo(240, CAMERA_KEEP, 280, 1);
		fdActorSetMovementTypeById(ACTOR_TRAVELER_FOLLOWING_THE_PIRATES, path);
		syFieldEventWait(10);
		fdActorSetMovementTypeById(ACTOR_TRAVELER_SEEKING_EVIDENCE, path);
	}
	{
		s32 path = (s32)YampiDesert_LastTravelersFollow;

		fdActorSetMovementTypeById(ACTOR_DIZZY_TRAVELER, path);
		syFieldEventWait(20);
		fdActorSetMovementTypeAndWaitById(ACTOR_PARCHED_TRAVELER, path);
	}
	fdActorStop(state->leaderActorId);
	sySynch(1);
	fdActorSetPos(ACTOR_TRAVELER_FOLLOWING_THE_PIRATES, 0, 0);
	fdActorSetPos(ACTOR_TRAVELER_SEEKING_EVIDENCE, 0, 0);
	fdActorSetPos(ACTOR_DIZZY_TRAVELER, 0, 0);
	fdActorSetPos(ACTOR_PARCHED_TRAVELER, 0, 0);
	fdActorSetPos(ACTOR_TRAVELER_WHO_WOULD_TURN_BACK, 0, 0);
	fdActorSetPos(ACTOR_ELDER, 0, 0);
	fdActorSetPos(ACTOR_MAYOR, 0, 0);
	scYampiDesertSetPosAndFacing(ACTOR_TRAVELER_FOLLOWING_THE_PIRATES,
								 toFIXED(520), toFIXED(242),
								 FACING_NORTH_NORTHWEST);
	scYampiDesertSetPosAndFacing(ACTOR_TRAVELER_SEEKING_EVIDENCE, toFIXED(533),
								 toFIXED(271), FACING_WEST);
	scYampiDesertSetPosAndFacing(ACTOR_DIZZY_TRAVELER, toFIXED(435),
								 toFIXED(232), FACING_SOUTH_SOUTHEAST);
	scYampiDesertSetPosAndFacing(ACTOR_PARCHED_TRAVELER, toFIXED(467),
								 toFIXED(248), FACING_SOUTH_SOUTHWEST);
	scYampiDesertSetPosAndFacing(ACTOR_TRAVELER_WHO_WOULD_TURN_BACK,
								 toFIXED(462), toFIXED(340),
								 FACING_NORTH_NORTHEAST);
	scYampiDesertSetPosAndFacing(ACTOR_ELDER, toFIXED(482), toFIXED(340),
								 FACING_NORTH_NORTHEAST);
	scYampiDesertSetPosAndFacing(ACTOR_MAYOR, toFIXED(489), toFIXED(320),
								 FACING_NORTH_NORTHWEST);
	fdActorSetAnimationById(ACTOR_TRAVELER_FOLLOWING_THE_PIRATES, ANIM_STAND);
	fdActorSetAnimationById(ACTOR_TRAVELER_SEEKING_EVIDENCE, ANIM_STAND);
	fdActorSetAnimationById(ACTOR_DIZZY_TRAVELER, ANIM_STAND);
	fdActorSetAnimationById(ACTOR_PARCHED_TRAVELER, ANIM_STAND);
	fdActorSetAnimationById(ACTOR_TRAVELER_WHO_WOULD_TURN_BACK, ANIM_STAND);
	fdActorSetAnimationById(ACTOR_ELDER, ANIM_STAND);
	fdActorSetAnimationById(ACTOR_MAYOR, ANIM_STAND);
	syGameFlagSet(FLAG_YAMPI_DESERT_TRAVELERS_MET);
	syGameFlagSet(FLAG_YAMPI_DESERT_ELDER_INSISTED);
	fdEventEnd();
}

/* The Scoop Gem's icon as an actor, after the King Scorpion's battle. */
ACTOR *scYampiDesertCreateScoopGem(void) {
	ACTOR *actor = syImportActorCreate(SPRITE_UNSEEN_MARKER, toFIXED(232),
									   toFIXED(32), toFIXED(200));
	ICON_TRANSFER *icon = NULL;
	YAMPI_DESERT_VISUAL *visual;
	s32 mask;

	if(actor == NULL)
		return NULL;
	{
		visual = actor->spr;
		/* Sixteen colours, palette 0, priority 1. */
		{
			s32 sixteenColors = ~(OBJ_256_COLORS >> 8);

			visual->attr0High &= sixteenColors;
		}
		mask = visual->attr2High & 0x0f;
		mask &= ~(OBJ_PRIORITY(3) >> 8);
		mask |= OBJ_PRIORITY(1) >> 8;
		visual->attr2High = mask;
		actor->motionFlg = 0;
		actor->retainSprCache = ACTOR_KEEP_SPRITE_VRAM;
		visual->flg = 0;
		visual->layerCnt = 0;
		icon = syHeapSlotAlloc(HEAP_SLOT_ICON_TRANSFER, sizeof(ICON_TRANSFER));
		mnIconDrawItem(ITEM_SCOOP_GEM);
		syVramCacheLoad(visual->vramBlock, ICON_TILE_BYTES, icon->tiles);
		syHeapSlotRelease(HEAP_SLOT_ICON_TRANSFER);
	}
	return actor;
}

/* After the battle, the King Scorpion leaves the Scoop Gem behind. */
void scYampiDesertDropScoopGem(void) {
	ACTOR *actor;

	fdEventStart();
	fdEventEndLastingPsynergy(0);
	fdEventStartScrTransition();
	fdEventWaitTransitionFrames();
	syFieldEventWait(20);
	fdActorSetAnimationById(MainRoutineStateWork.leaderActorId,
							ANIM_LEADER_ARMS_RAISED);
	actor = scYampiDesertCreateScoopGem();
	fdPartyInventoryGiveItem(ITEM_SCOOP_GEM, 0);
	if(actor != 0) {
		fdActorSetPosAndResetMotion((ACTOR *)actor, 0, 0, 0);
		sySynch(1);
		fdActorRelease(actor);
	}
	fdActorSetAnimationById(MainRoutineStateWork.leaderActorId, ANIM_STAND);
	syGameFlagSet(FLAG_KING_SCORPION_BEATEN);
	fdEventEnd();
}

/* Keeps the ripple of sand over the buried King Scorpion. */
void scYampiDesertRippleFollowsScorpion(void) {
	ACTOR *actor;
	s16 scene;

	FieldStateWork->raisedTrigger = YampiDesert_RaisedEvent;
	if(YampiDesert_SandRipple != 0) {
		scene = MainRoutineStateWork.next;
		if(scene == SCENE_YAMPI_DESERT_PSYNERGY_STONE)
			actor = fdActorGetById(ACTOR_PSYNERGY_STONE_ROOM_KING_SCORPION);
		else if(scene == SCENE_YAMPI_DESERT_KING_SCORPION)
			actor = fdActorGetById(ACTOR_KING_SCORPION_ROOM_KING_SCORPION);
		else
			return;
		YampiDesert_SandRipple->x.fixed = actor->x.fixed;
		YampiDesert_SandRipple->z.fixed = actor->z.fixed;
		YampiDesert_SandRipple->facing = actor->facing;
	}
}
#undef MOVE_LEADER_TO

/* The flowing sand carries the leader by (dx, dz), throwing up dust every */
/* other frame; with no direction held, it draws the leader to the middle of */
/* its cells. */
void scYampiDesertCarryLeader(ACTOR *actor, FIXED dx, FIXED dz) {
	fdActorSetSprFlags(fdActorGetById(MainRoutineStateWork.leaderActorId), 0);
	if(MainRoutineStateWork.movementMode != LEADER_MOVEMENT_SAND) {
		u32 odd = FrameCount & 1;

		if(odd == 0) {
			ACTOR *dust = (ACTOR *)Value4((s32 (*)())syImportActorCreate,
										  SPRITE_DUST_CLOUD, actor->x.fixed,
										  actor->y.fixed, actor->z.fixed);

			if(dust != NULL) {
				SPR *sprite;

				dust->motionFlg = odd;
				dust->collisionFlg = odd;
				fdActorSetSprFlags(dust, 0);
				fdActorSelectAnimation(dust, 1);
				fdActorStartScript(dust, YampiDesert_FlowDust);
				sprite = dust->spr;
				sprite->image.pri = 1;
				if(InputWork.held != 0) {
					dust->bounce = syCos(syRandomBelow(8) << 14) + dx;
					dust->sceneWord = sySin(syRandomBelow(8) << 14) + dz;
					if(InputWork.held & KEY_B) {
						dust->bounce += dx;
						dust->sceneWord += dz;
					}
				}
				else {
					dust->bounce = (syCos(syRandomBelow(8) << 14) >> 1) + dx;
					dust->sceneWord = (sySin(syRandomBelow(8) << 14) >> 1) + dz;
				}
				dust->gravity = 0;
				dust->tmr = syRandomBelow(512) - 256;
				dust->speedLimit = 0;
				dust->acceleration = 0;
				dust->update = fdEffectAdvanceMotionAtLeaderPriority;
			}
		}
	}
	if(dx != 0) {
		actor->x.fixed += dx;
		if(!(InputWork.held & KEYS_DPAD)) {
			s32 z = FIXEDtoINT(actor->z.fixed);

			actor->z.fixed += INTtoFIXED(8 - z % 16) >> 3;
		}
	}
	else {
		actor->z.fixed += dz;
		if(!(InputWork.held & KEYS_DPAD)) {
			s32 x = FIXEDtoINT(actor->x.fixed);

			actor->x.fixed += INTtoFIXED(8 - x % 16) >> 3;
		}
	}
}

/* The task of the rooms with flowing sand: on the flow's cells the sand */
/* carries the leader a pixel and a half a frame its way; on the cells the */
/* ground marks, holding the pad that way moves the leader a quarter as fast. */
void scYampiDesertUpdateSandFlow(void) {
	FIELD_STATE *state = Ram_HeapSlots->fieldState;
	ACTOR *leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
	s32 speed = toFIXED(3) / 2;
	MAP_STATE *map;
	MAP_CELL *here, *ahead, *under;
	s32 index, column, row;
	MAP_CELL *cells;

	if(state->psynergy != 0 || state->fieldMenu != 0 || state->pauseMenu != 0) {
		fdActorSetAnimationSpeed(leader, 0);
		fdTilesetAnimSuspendAll();
		return;
	}
	map = Ram_HeapSlots->mapGrid;
	index = fdActorGetCellIndex(leader);
	column = leader->x.fixed / toFIXED(16);
	row = (leader->z.fixed + toFIXED(4)) / toFIXED(16);
	cells = map->layers[MAPGRID_LAYER_GROUND].cells;
	here = cells + index;
	ahead = cells + (column + row * MAPGRID_SIZE);
	under = map->layers[MAPGRID_LAYER_MIDDLE].cells + index;
	fdActorSetAnimationSpeed(leader, ANIM_STEP_NORMAL);
	fdTilesetAnimResumeAll();
	/* On a stream or its rim, a leader moving as Sand is not carried, and one
	 */
	/* on foot has his sprite flags set to show his shadow alone. */
	if((u8)(under->collisionCode - CELL_BEHAVIOR_YAMPI_DESERT_FLOW_NORTH) <=
	   CELL_BEHAVIOR_YAMPI_DESERT_FLOW_RIM -
		   CELL_BEHAVIOR_YAMPI_DESERT_FLOW_NORTH) {
		if(MainRoutineStateWork.movementMode == LEADER_MOVEMENT_SAND)
			speed = 0;
		else
			fdActorSetSprFlags(
				fdActorGetById(MainRoutineStateWork.leaderActorId),
				SPRITE_FLAG_SHADOW);
	}
	if(ahead->collisionCode == CELL_BEHAVIOR_WALL)
		speed = 0;
	switch(here->collisionCode) {
	case CELL_BEHAVIOR_YAMPI_DESERT_FLOW_NORTH:
		if(InputWork.held & KEY_UP) {
			scYampiDesertCarryLeader(leader, 0, -speed >> 2);
			return;
		}
		break;
	case CELL_BEHAVIOR_YAMPI_DESERT_FLOW_SOUTH:
		if(InputWork.held & KEY_DOWN) {
			scYampiDesertCarryLeader(leader, 0, speed >> 2);
			return;
		}
		break;
	case CELL_BEHAVIOR_YAMPI_DESERT_FLOW_WEST:
		if(InputWork.held & KEY_LEFT) {
			scYampiDesertCarryLeader(leader, -speed >> 2, 0);
			return;
		}
		break;
	case CELL_BEHAVIOR_YAMPI_DESERT_FLOW_EAST:
		if(InputWork.held & KEY_RIGHT) {
			scYampiDesertCarryLeader(leader, speed >> 2, 0);
			return;
		}
		break;
	}
	switch(under->collisionCode) {
	case CELL_BEHAVIOR_YAMPI_DESERT_FLOW_NORTH:
		scYampiDesertCarryLeader(leader, 0, -speed);
		break;
	case CELL_BEHAVIOR_YAMPI_DESERT_FLOW_SOUTH:
		scYampiDesertCarryLeader(leader, 0, speed);
		break;
	case CELL_BEHAVIOR_YAMPI_DESERT_FLOW_WEST:
		scYampiDesertCarryLeader(leader, -speed, 0);
		break;
	case CELL_BEHAVIOR_YAMPI_DESERT_FLOW_EAST:
		scYampiDesertCarryLeader(leader, speed, 0);
		break;
	}
}

/* Notes whether the leader stands on the upper level; in Blitz's room, keeps */
/* the stake before the climbable wall drawn as the leader is. */
void scYampiDesertCheckUpperLevel(void) {
	ACTOR *leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
	FIXED x;
	FIXED y;
	FIXED z;

	y = leader->y.fixed;
	if(y > toFIXED(16))
		MainRoutineStateWork.onUpperLevel = 1;
	else
		MainRoutineStateWork.onUpperLevel = 0;
	if(MainRoutineStateWork.next == SCENE_YAMPI_DESERT_BLITZ) {
		fdActorSetSprPriority(ACTOR_CLIMBABLE_WALL_STAKE,
							  ((FIELD_SPR *)leader->spr)->pri);
		x = leader->x.fixed;
		if(x > toFIXED(472)) {
			z = leader->z.fixed;
			if(z <= toFIXED(488))
				return;
			if(leader->y.fixed > -toFIXED(16))
				MainRoutineStateWork.onUpperLevel = 1;
			else
				MainRoutineStateWork.onUpperLevel = 0;
		}
	}
}

/* Notes whether the leader has climbed above the lower level. */
void scYampiDesertCheckUpperLevelFromLower(void) {
	ACTOR *leader = fdActorGetById(MainRoutineStateWork.leaderActorId);

	if(leader->y.fixed > -toFIXED(16))
		MainRoutineStateWork.onUpperLevel = 1;
	else
		MainRoutineStateWork.onUpperLevel = 0;
}

/* Unused: notes whether the leader stands high on the cells of */
/* CELL_BEHAVIOR_YAMPI_DESERT_RISE. */
UNUSED void scYampiDesertCheckUpperLevelOnRise(void) {
	ACTOR *leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
	MAP_STATE *map = Ram_HeapSlots->mapGrid;
	s32 index = fdActorGetCellIndex(leader);
	MAP_CELL *cell = map->layers[MAPGRID_LAYER_GROUND].cells + index;
	MAIN_ROUTINE_STATE *party;
	FIXED y;

	if(cell->collisionCode == CELL_BEHAVIOR_YAMPI_DESERT_RISE) {
		y = leader->y.fixed;
		if(y > toFIXED(16)) {
			MainRoutineStateWork.onUpperLevel = 1;
			return;
		}
	}
	party = &MainRoutineStateWork;
	party->onUpperLevel = 0;
}

/* Unused: notes whether the leader stands near the scorpion pit's middle, */
/* where opening a menu raises a trigger. */
UNUSED void scYampiDesertCheckLeaderNearPit(void) {
	ACTOR *leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
	u32 x = FIELD_CELL(leader->x.fixed);
	u32 z = FIELD_CELL(leader->z.fixed);

	if(x > 12 && z > 10 && x <= 15 && z <= 13)
		syGameFlagSet(FLAG_MENUS_RAISE_TRIGGER);
	else
		syGameFlagClear(FLAG_MENUS_RAISE_TRIGGER);
}

/* While the King Scorpion roams buried, a rumble every five frames, unless the
 */
/* leader stands still. */
void scYampiDesertPlayScorpionRumble(void) {
	if(FieldStateWork->psynergy == 0 && FrameCount % 5 == 0) {
		MAIN_ROUTINE_STATE *party = &MainRoutineStateWork;

		if(party->movementMode != LEADER_MOVEMENT_SAND || InputWork.held == 0)
			sySndPlay(SE_SAND_MOVE);
	}
}

/* Clears the trigger the scorpion has raised. */
void scYampiDesertClearRaisedEvent(void) {
	FieldStateWork->raisedTrigger = 0;
	YampiDesert_RaisedEvent = 0;
}

/* Whether an actor has no move target left. */
s32 scYampiDesertHasArrived(ACTOR *actor) {
	s32 x = actor->tgtX;

	if(x == ACTOR_NO_TARGET && actor->tgtZ == x)
		return 1;
	return 0;
}

/* A step of the buried King Scorpion's rounds, kept in its sceneCnt: sends
 */
/* it toward a point, in pixels, and goes on to the next step, which waits for
 */
/* it there. */
#define WALK(state_, x, z)                                                     \
	case state_:                                                               \
		scFieldActorSetMoveTarget((s32)actor, x, 0, z);                        \
		*state = state_ + 1;                                                   \
		break;

/* As WALK, after scYampiDesertClearRaisedEvent. */
#define WALK_ON(state_, x, z)                                                  \
	case state_:                                                               \
		scYampiDesertClearRaisedEvent();                                       \
		scFieldActorSetMoveTarget((s32)actor, x, 0, z);                        \
		*state = state_ + 1;                                                   \
		break;

/* Waits for the actor to arrive, then goes on to step next. */
#define ARRIVE(state_, next)                                                   \
	case state_:                                                               \
		if(scYampiDesertHasArrived(actor))                                     \
			*state = next;                                                     \
		break;

/* Waits for the actor to arrive, then goes on to step turn once the flag is */
/* set and to step next while it is not. */
#define ARRIVE_OR(state_, flag, turn, next)                                    \
	case state_:                                                               \
		if(scYampiDesertHasArrived(actor)) {                                   \
			if(syGameFlagGet(flag))                                            \
				*state = turn;                                                 \
			else                                                               \
				*state = next;                                                 \
		}                                                                      \
		break;

/* The buried King Scorpion's round in the Psynergy Stone's room, a loop of */
/* steps, two to each point: an even step sends it toward the point, the odd */
/* one after waits for it there. Once Pound has sunk stake 3, it stops at the */
/* stake's south side and raises RAISED_STIR_SCORPION. */
void scYampiDesertScorpionRoamsPsynergyStone(ACTOR *actor) {
	u8 *state = &actor->sceneCnt;

	switch(*state) {
		WALK(0, 280, 200)
		ARRIVE(1, 2)
		WALK(2, 280, 264)
		ARRIVE(3, 4)
		WALK(4, 184, 264)
		ARRIVE(5, 6)
		WALK(6, 184, 248)
	case 7:
		if(scYampiDesertHasArrived(actor)) {
			if(syGameFlagGet(FLAG_YAMPI_DESERT_STAKE_POUNDED(3))) {
				actor->update = NULL;
				YampiDesert_RaisedEvent = RAISED_STIR_SCORPION;
			}
			else {
				*state = 8;
			}
		}
		break;
		WALK(8, 184, 200)
		ARRIVE(9, 0)
	}
}

/* Readies the Psynergy Stone room's King Scorpion and the sand ripple that */
/* follows it under the sand, and sets the scorpion on its round unless the */
/* party is back from a battle. */
void scYampiDesertSetUpPsynergyStoneScorpion(void) {
	ACTOR *scorpion = fdActorGetById(ACTOR_PSYNERGY_STONE_ROOM_KING_SCORPION);
	ACTOR *actor;
	ACTOR **followed;

	syFieldTaskAddFrame(scYampiDesertPlayScorpionRumble, TASK_PASS_FRAME);
	followed = &YampiDesert_SandRipple;
	actor = fdActorGetById(ACTOR_PSYNERGY_STONE_ROOM_SAND_RIPPLE);
	actor->priFlg |= ACTOR_PRIORITY_UNDERFOOT;
	*followed = (ACTOR *)actor;
	actor->scaleX = FIXED_ONE * 13 / 10;
	actor->scaleY = FIXED_ONE * 13 / 10;
	fdActorDisableById(ACTOR_PSYNERGY_STONE_ROOM_SAND_RIPPLE);
	(*followed)->motionFlg = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
	fdActorSetSprPriority(ACTOR_PSYNERGY_STONE_ROOM_SAND_RIPPLE, 3);
	fdActorSetSpeed(ACTOR_PSYNERGY_STONE_ROOM_KING_SCORPION, 1.6);
	fdActorSetSprCol((ACTOR *)scorpion, SPRITE_COLOR_HIDDEN);
	fdActorSetSprPriority(ACTOR_PSYNERGY_STONE_ROOM_KING_SCORPION, 3);
	fdActorDisableById(ACTOR_PSYNERGY_STONE_ROOM_KING_SCORPION);
	fdActorGetById(ACTOR_PSYNERGY_STONE_ROOM_KING_SCORPION)->motionFlg =
		ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
	if(!syGameFlagGet(FLAG_KEEP_PARTY_POSITION)) {
		scorpion->sceneCnt = 0;
		scorpion->update = scYampiDesertScorpionRoamsPsynergyStone;
	}
	scYampiDesertClearRaisedEvent();
	sySetEvent(scYampiDesertRippleFollowsScorpion, TASK_PASS_FRAME);
}

/* The buried King Scorpion's round in the King Scorpion's room, about stake 0:
 */
/* once Pound has sunk the stake, it stops at its east side and raises */
/* RAISED_STIR_SCORPION_AGAIN, or, past that point, keeps from step 32 to a */
/* small loop south of it. */
void scYampiDesertScorpionRoamsKingScorpion(ACTOR *actor) {
	u8 *state = &actor->sceneCnt;

	switch(*state) {
		WALK(0, 312, 664)
		ARRIVE(1, 2)
		WALK(2, 312, 728)
		ARRIVE(3, 4)
		WALK(4, 246, 728)
	case 5:
		if(scYampiDesertHasArrived(actor)) {
			if(syGameFlagGet(FLAG_YAMPI_DESERT_STAKE_POUNDED(0))) {
				actor->update = NULL;
				YampiDesert_RaisedEvent = RAISED_STIR_SCORPION_AGAIN;
			}
			else {
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
		if(scYampiDesertHasArrived(actor)) {
			if(!syGameFlagGet(FLAG_YAMPI_DESERT_STAKE_POUNDED(0)))
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

/* scYampiDesertSetUpPsynergyStoneScorpion's counterpart in the King Scorpion's
 */
/* room, which also places the scorpion at the start of its round. */
void scYampiDesertSetUpKingScorpionScorpion(void) {
	ACTOR *scorpion = fdActorGetById(ACTOR_KING_SCORPION_ROOM_KING_SCORPION);
	ACTOR *actor;
	ACTOR **followed;

	syFieldTaskAddFrame(scYampiDesertPlayScorpionRumble, TASK_PASS_FRAME);
	followed = &YampiDesert_SandRipple;
	actor = fdActorGetById(ACTOR_KING_SCORPION_ROOM_SAND_RIPPLE);
	actor->priFlg |= ACTOR_PRIORITY_UNDERFOOT;
	*followed = (ACTOR *)actor;
	actor->scaleX = FIXED_ONE * 13 / 10;
	actor->scaleY = FIXED_ONE * 13 / 10;
	fdActorDisableById(ACTOR_KING_SCORPION_ROOM_SAND_RIPPLE);
	(*followed)->motionFlg = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
	fdActorSetSprPriority(ACTOR_KING_SCORPION_ROOM_SAND_RIPPLE, 3);
	fdActorSetSpeed(ACTOR_KING_SCORPION_ROOM_KING_SCORPION, 1.6);
	fdActorSetSprCol((ACTOR *)scorpion, SPRITE_COLOR_HIDDEN);
	fdActorSetSprPriority(ACTOR_KING_SCORPION_ROOM_KING_SCORPION, 3);
	fdActorDisableById(ACTOR_KING_SCORPION_ROOM_KING_SCORPION);
	fdActorGetById(ACTOR_KING_SCORPION_ROOM_KING_SCORPION)->motionFlg =
		ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
	if(!syGameFlagGet(FLAG_KEEP_PARTY_POSITION)) {
		scYampiDesertSetPosAndFacing(ACTOR_KING_SCORPION_ROOM_KING_SCORPION,
									 toFIXED(232), toFIXED(664), 0);
		scorpion->sceneCnt = 0;
		scorpion->update = scYampiDesertScorpionRoamsKingScorpion;
	}
	scYampiDesertClearRaisedEvent();
	sySetEvent(scYampiDesertRippleFollowsScorpion, TASK_PASS_FRAME);
}

/* The buried King Scorpion's round in the King Scorpion's room once stirred */
/* again, between stakes 1 and 2 of the trap: stopped by stake 2 sunk to its */
/* west, it loops east of that stake from step 32, by stake 1 sunk to its east,
 */
/* west of that stake from step 48, and by stake 1 sunk to its north, east of */
/* stake 2 from step 34; reaching stake 2's north side once that is sunk, it is
 */
/* cornered and raises RAISED_CORNER_SCORPION. */
void scYampiDesertScorpionRoamsToTrap(ACTOR *actor) {
	u8 *state = &actor->sceneCnt;

	switch(*state) {
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
		if(scYampiDesertHasArrived(actor)) {
			if(syGameFlagGet(FLAG_YAMPI_DESERT_STAKE_POUNDED(2))) {
				actor->update = NULL;
				YampiDesert_RaisedEvent = RAISED_CORNER_SCORPION;
			}
			else {
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

/* scYampiDesertSetUpPsynergyStoneScorpion's counterpart in the King Scorpion's
 */
/* room once it is stirred again: once the scorpion is cornered */
/* (FLAG_YAMPI_DESERT_SCORPION_CORNERED) it is placed facing south; */
/* otherwise, unless the party is back from a battle */
/* (FLAG_KEEP_PARTY_POSITION), it is placed and given the update */
/* scYampiDesertScorpionRoamsToTrap. */
void scYampiDesertSetUpTrappedScorpion(void) {
	ACTOR *scorpion = fdActorGetById(ACTOR_KING_SCORPION_ROOM_KING_SCORPION);
	ACTOR *actor;
	ACTOR **followed;

	followed = &YampiDesert_SandRipple;
	actor = fdActorGetById(ACTOR_KING_SCORPION_ROOM_SAND_RIPPLE);
	actor->priFlg |= ACTOR_PRIORITY_UNDERFOOT;
	actor->scaleX = FIXED_ONE * 13 / 10;
	actor->scaleY = FIXED_ONE * 13 / 10;
	*followed = (ACTOR *)actor;
	fdActorDisableById(ACTOR_KING_SCORPION_ROOM_SAND_RIPPLE);
	fdActorSetSprPriority(ACTOR_KING_SCORPION_ROOM_SAND_RIPPLE, 3);
	(*followed)->motionFlg = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
	fdActorSetSpeed(ACTOR_KING_SCORPION_ROOM_KING_SCORPION, 1.6);
	fdActorSetSprCol((ACTOR *)scorpion, SPRITE_COLOR_HIDDEN);
	fdActorSetSprPriority(ACTOR_KING_SCORPION_ROOM_KING_SCORPION, 3);
	fdActorDisableById(ACTOR_KING_SCORPION_ROOM_KING_SCORPION);
	fdActorGetById(ACTOR_KING_SCORPION_ROOM_KING_SCORPION)->motionFlg =
		ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
	if(syGameFlagGet(FLAG_YAMPI_DESERT_SCORPION_CORNERED)) {
		scYampiDesertSetPosAndFacing(ACTOR_KING_SCORPION_ROOM_KING_SCORPION,
									 toFIXED(232), toFIXED(168), FACING_SOUTH);
	}
	else {
		syFieldTaskAddFrame(scYampiDesertPlayScorpionRumble, TASK_PASS_FRAME);
		if(!syGameFlagGet(FLAG_KEEP_PARTY_POSITION)) {
			scYampiDesertSetPosAndFacing(ACTOR_KING_SCORPION_ROOM_KING_SCORPION,
										 toFIXED(200), toFIXED(264), 0);
			scorpion->sceneCnt = 0;
			scorpion->update = scYampiDesertScorpionRoamsToTrap;
		}
	}
	scYampiDesertClearRaisedEvent();
	sySetEvent(scYampiDesertRippleFollowsScorpion, TASK_PASS_FRAME);
}

/* While the King Scorpion's Scoop runs, opens the hole it digs. */
void scYampiDesertOpenScoopHole(void) {
	if(((FIELD_EFFECT_STATE *)Ram_HeapSlots->fieldEffState)->stage != 0)
		fdMapGridCopyCellTilesIndicesRect(73, 76, 78, 11, 1, 1);
}

/* The trapped King Scorpion digs its way out with Scoop. */
void scYampiDesertScorpionUsesScoop(void) {
	fdPsynergyLoadResources(ABILITY_SCOOP, 1);
	syFieldTaskAddFrame(scYampiDesertOpenScoopHole, TASK_PASS_FRAME);
	fdPsynergySetupActorPair(ACTOR_KING_SCORPION_ROOM_KING_SCORPION, -1);
	fdPsynergyStartCast();
	fdPsynergyRunHandler(1);
	fdBattleEffectStopMainActor();
	syCloseEvent(scYampiDesertOpenScoopHole);
	fdPsynergyEnd();
}

#include "shared/actor_hide.h"

/* Back from a battle, hides the unseen blocks again unless they were shown; */
/* otherwise marks them shown. */
void scYampiDesertRestoreBlocks(void) {
	if(syGameFlagGet(FLAG_KEEP_PARTY_POSITION)) {
		if(syGameFlagGet(FLAG_YAMPI_DESERT_BLOCKS_SHOWN) == 0)
			scYampiDesertHideBlocks();
	}
	else {
		syGameFlagSet(FLAG_YAMPI_DESERT_BLOCKS_SHOWN);
	}
}

/* Hides the Trainer's Whip chest's room's unseen blocks. */
void scYampiDesertSetUpTrainersWhipChestBlocks(void) {
	fdActorHide(ACTOR_WHIP_CHEST_ROOM_FIRST_BLOCK);
	fdActorHide(ACTOR_WHIP_CHEST_ROOM_SECOND_BLOCK);
	scYampiDesertRestoreBlocks();
}

/* Hides the Blow Mace chest's room's unseen blocks. */
void scYampiDesertSetUpBlowMaceChestBlocks(void) {
	u32 i;

	for(i = 0; i <= ACTOR_BLOW_MACE_CHEST_ROOM_LAST_BLOCK -
						ACTOR_BLOW_MACE_CHEST_ROOM_FIRST_BLOCK;
		i++)
		fdActorHide(ACTOR_BLOW_MACE_CHEST_ROOM_FIRST_BLOCK + i);
	scYampiDesertRestoreBlocks();
}

/* The leader climbs down the ladder at (x, z) and leaves by `exit`. */
void scYampiDesertClimbDownLadder(ACTOR *actor, s32 x, s32 z, s32 exit) {
	MAIN_ROUTINE_STATE *party;

	fdEventStart();
	fdEventEndLastingPsynergy(0);
	actor->motionFlg = 0;
	party = &MainRoutineStateWork;
	fdActorSetSpeed(party->leaderActorId, 1);
	fdActorWalkToAndWaitById(party->leaderActorId, x, z);
	fdActorFaceDirectionAndRefreshById(party->leaderActorId, FACING_NORTH);
	fdActorSetSprFlags(fdActorGetById(party->leaderActorId), 0);
	fdActorSetAnimationById(party->leaderActorId,
							ANIM_LEADER_CLIMB_DOWN_FROM_TOP);
	fdActorSetMoveTarget(actor, actor->x.fixed, actor->y.fixed - toFIXED(10),
						 actor->z.fixed);
	fdActorWaitForMove(actor);
	fdActorSetAnimationById(party->leaderActorId, ANIM_LEADER_CLIMB);
	sySndPlay(SE_MAP_EXIT);
	fdEventFinishScrTransition();
	fdEventWaitTransitionFrames();
	syImportEventSetExitRequest(exit);
}

/* Walking into the hole Scoop has dug in the sand puddle at the south of the */
/* spring's room (cell (8, 32)), holding the pad towards it, Felix climbs down
 */
/* its ladder, facing north, into the cave under the spring. */
void scYampiDesertClimbRevealedLadder(void) {
	ACTOR *actor;
	s32 x;
	s32 z;

	if(syGameFlagGet(FLAG_YAMPI_DESERT_LADDER_DUG_UP)) {
		actor = fdActorGetById(MainRoutineStateWork.leaderActorId);
		x = FIELD_CELL(actor->x.fixed);
		z = FIELD_CELL(actor->z.fixed);
		if(x == 8) {
			if(z == 31 && !(InputWork.held & KEY_DOWN))
				return;
			if(z == 33 && !(InputWork.held & KEY_UP))
				return;
		}
		else {
			if(z != 32)
				return;
			if(x == 7 && !(InputWork.held & KEY_RIGHT))
				return;
			if(x == 9 && !(InputWork.held & KEY_LEFT))
				return;
		}
		scYampiDesertClimbDownLadder(
			actor, 136, 516,
			YAMPI_DESERT_SPRING_TO_YAMPI_DESERT_SPRING_CAVE_SOUTH);
	}
}

/* Walking north into the ladder of the small chamber of the cave under the */
/* spring, Felix climbs down it to the cave below. */
void scYampiDesertClimbDownSpringCaveLadder(void) {
	scYampiDesertClimbDownLadder(
		fdActorGetById(MainRoutineStateWork.leaderActorId), 408, 432,
		SPRING_CAVE_TO_LOWER_CAVE);
}

void scYampiDesertBurstSand(s32 x, s32 y, s32 z);

/* Dropping into the scorpion pit, the leader lands before the King Scorpion, */
/* and its battle starts. */
void scYampiDesertLandAndFightKingScorpion(void) {
	MAIN_ROUTINE_STATE *party = &MainRoutineStateWork;
	ACTOR *leader = fdActorGetById(party->leaderActorId);
	ACTOR *scorpion = fdActorGetById(ACTOR_KING_SCORPION);

	fdEventStart();
	fdEventEndLastingPsynergy(0);
	sySndPlay(SOUND_FADE_OUT_BGM);
	fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
	fdCameraMoveTo(232, CAMERA_KEEP, 184, 0);
	fdMapGridCenterOnFollowedPoint();
	sySynch(1);
	fdActorSetSprFlags(fdActorGetById(party->leaderActorId), 0);
	leader->motionFlg = 0;
	leader->y.fixed = toFIXED(130);
	leader->gravity = FIXED_ONE / 2;
	leader->bounce = 0;
	fdActorSetSprFlags(fdActorGetById(ACTOR_KING_SCORPION), 0);
	scYampiDesertSetPosAndFacing(ACTOR_KING_SCORPION, toFIXED(232),
								 toFIXED(148), FACING_SOUTH);
	scorpion->motionFlg = 0;
	scorpion->y.fixed = toFIXED(130);
	scorpion->gravity = FIXED_ONE / 2;
	scorpion->bounce = 0;
	sySynch(1);
	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_CIRCLE);
	fdEventStartScrTransition();
	fdEventWaitTransitionFrames();
	sySndPlay(SE_DUST_PUFF);
	leader->motionFlg = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
	fdActorSetSprFlags(fdActorGetById(party->leaderActorId),
					   SPRITE_FLAG_SHADOW);
	syFieldEventWait(24);
	scYampiDesertBurstSand(leader->x.fixed, leader->y.fixed, leader->z.fixed);
	sySndPlay(SE_THUMP);
	fdEffectSetAttached(party->leaderActorId,
						ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ATTACH);
	fdActorSetAnimationAndWait(party->leaderActorId, ANIM_LEADER_SIT);
	sySndPlay(SE_DUST_PUFF);
	scorpion->motionFlg = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
	fdActorSetSprFlags(fdActorGetById(ACTOR_KING_SCORPION), SPRITE_FLAG_SHADOW);
	syFieldEventWait(24);
	scYampiDesertBurstSand(scorpion->x.fixed, scorpion->y.fixed,
						   scorpion->z.fixed);
	sySndPlay(SE_THUMP);
	fdActorFaceDirectionById(party->leaderActorId, FACING_NORTH, 0);
	syFieldEventWait(40);
	sySndPlay(SE_BEAST_CRY);
	fdActorSetAnimationById(ACTOR_KING_SCORPION, 3);
	syFieldEventWait(20);
	scorpion->velY = toFIXED(8);
	scorpion->gravity = toFIXED(9) / 10;
	fdActorSetSpeed(ACTOR_KING_SCORPION, 1.6);
	fdActorMoveToAndWaitById(ACTOR_KING_SCORPION, 232, 184);
	party->battleType = BATTLE_TYPE_NO_ESCAPE;
	fdEventSetAfterBattleDestination(
		SCENE_YAMPI_DESERT_SCORPION_PIT,
		YAMPI_DESERT_SCORPION_PIT_AFTER_KING_SCORPION_BATTLE);
	fdRequestBattleFromEncounterTable(
		ENCOUNTER_ZONE_SCRIPTED_YAMPI_DESERT_ALHAFRA, 0);
}

/* Clears the scorpion's sand ripple and runs the entered room's setup; the */
/* sand pass has none. */
s32 scInit(void) {
	s16 scene;

	scYampiDesertClearRaisedEvent();
	YampiDesert_SandRipple = 0;
	scene = MainRoutineStateWork.next;
	if(scene == SCENE_YAMPI_DESERT_GUARDIAN_RING_CHEST)
		scYampiDesertSetUpGuardianRingChest();
	else if(scene == SCENE_YAMPI_DESERT_SPRING)
		scYampiDesertSetUpSpring();
	else if(scene == SCENE_YAMPI_DESERT_BLITZ)
		scYampiDesertSetUpBlitz();
	else if(scene == SCENE_YAMPI_DESERT_PSYNERGY_STONE)
		scYampiDesertSetUpPsynergyStone();
	else if(scene == SCENE_YAMPI_DESERT_KING_SCORPION)
		scYampiDesertSetUpKingScorpion();
	else if(scene == SCENE_YAMPI_DESERT_COINS_PUDDLE)
		scYampiDesertSetUpCoinsPuddle();
	else if(scene == SCENE_YAMPI_DESERT_TRAINERS_WHIP_CHEST)
		scYampiDesertSetUpTrainersWhipChest();
	else if(scene == SCENE_YAMPI_DESERT_HIDDEN_PSYNERGY_STONE)
		scYampiDesertSetUpHiddenPsynergyStone();
	else if(scene == SCENE_YAMPI_DESERT_SCORPION_PIT)
		scYampiDesertSetUpScorpionPit();
	else if(scene == SCENE_YAMPI_DESERT_SPRING_CAVE)
		scYampiDesertSetUpSpringCave();
	else if(scene == SCENE_YAMPI_DESERT_BLOW_MACE_CHEST)
		scYampiDesertSetUpBlowMaceChest();
	return 0;
}

s32 scPrepareMap(void) { return 0; }

/* Sets up the Guardian Ring chest's room: its stakes, and the way in it was */
/* entered by. */
void scYampiDesertSetUpGuardianRingChest(void) {
	scYampiDesertPlaceStakes(YampiDesertGuardianRingChestStakes);
	fdEffectStartBlankOverlay(0);
	if(MainRoutineStateWork.entrance ==
	   YAMPI_DESERT_GUARDIAN_RING_CHEST_FROM_WORLD_MAP) {
		syGameFlagSet(FLAG_YAMPI_DESERT_ENTERED_BY_GUARDIAN_RING_CHEST);
		syGameFlagClear(FLAG_YAMPI_DESERT_ENTERED_BY_TRAINERS_WHIP_CHEST);
		syGameFlagClear(FLAG_YAMPI_DESERT_ENTERED_BY_HIDDEN_PSYNERGY_STONE);
	}
}

/* The travelers are gone once the Madrans have reached Alhafra */
/* (FLAG_ALHAFRA_MADRANS_ARRIVED); once met (FLAG_YAMPI_DESERT_TRAVELERS_MET) */
/* they stand about the spring. */
void scYampiDesertSetUpSpring(void) {
	s32 angleA;
	s32 angleB;

	if(syGameFlagGet(FLAG_ALHAFRA_MADRANS_ARRIVED)) {
		fdActorSetPos(ACTOR_TRAVELER_FOLLOWING_THE_PIRATES, 0, 0);
		fdActorSetPos(ACTOR_TRAVELER_SEEKING_EVIDENCE, 0, 0);
		fdActorSetPos(ACTOR_DIZZY_TRAVELER, 0, 0);
		fdActorSetPos(ACTOR_PARCHED_TRAVELER, 0, 0);
		fdActorSetPos(ACTOR_TRAVELER_WHO_WOULD_TURN_BACK, 0, 0);
		fdActorSetPos(ACTOR_ELDER, 0, 0);
		fdActorSetPos(ACTOR_MAYOR, 0, 0);
	}
	else if(syGameFlagGet(FLAG_YAMPI_DESERT_TRAVELERS_MET)) {
		angleA = FACING_NORTH_NORTHWEST;
		scYampiDesertSetPosAndFacing(ACTOR_TRAVELER_FOLLOWING_THE_PIRATES,
									 toFIXED(520), toFIXED(242), angleA);
		scYampiDesertSetPosAndFacing(ACTOR_TRAVELER_SEEKING_EVIDENCE,
									 toFIXED(533), toFIXED(271), FACING_WEST);
		scYampiDesertSetPosAndFacing(ACTOR_DIZZY_TRAVELER, toFIXED(435),
									 toFIXED(232), FACING_SOUTH_SOUTHEAST);
		scYampiDesertSetPosAndFacing(ACTOR_PARCHED_TRAVELER, toFIXED(467),
									 toFIXED(248), FACING_SOUTH_SOUTHWEST);
		angleB = FACING_NORTH_NORTHEAST;
		scYampiDesertSetPosAndFacing(ACTOR_TRAVELER_WHO_WOULD_TURN_BACK,
									 toFIXED(462), toFIXED(340), angleB);
		scYampiDesertSetPosAndFacing(ACTOR_ELDER, toFIXED(482), toFIXED(340),
									 angleB);
		scYampiDesertSetPosAndFacing(ACTOR_MAYOR, toFIXED(489), toFIXED(320),
									 angleA);
	}
	if(syGameFlagGet(FLAG_YAMPI_DESERT_LADDER_DUG_UP))
		scYampiDesertOpenLadderPit();
	sySetEvent(scYampiDesertCheckUpperLevel, TASK_PASS_FRAME);
}

/* Keeps two of Blitz's room's stakes drawn as the leader is. */
void scYampiDesertLayerStakesWithLeader(void) {
	s32 variant =
		((FIELD_SPR *)fdActorGetById(MainRoutineStateWork.leaderActorId)->spr)
			->pri;

	fdActorSetSprPriority(STAKE(5), variant);
	fdActorSetSprPriority(STAKE(6), variant);
}

/* Sets up Blitz's room: its stakes and the tasks that layer them. */
void scYampiDesertSetUpBlitz(void) {
	scYampiDesertPlaceStakes(YampiDesertBlitzStakes);
	if(syGameFlagGet(FLAG_YAMPI_DESERT_ON_STAKE_TOPS)) {
		scYampiDesertStepOntoStakeTops();
		sySynch(10);
	}
	syFieldTaskAddFrame(scYampiDesertCheckUpperLevel, TASK_PASS_FRAME);
	syFieldTaskAddFrame(scYampiDesertLayerStakesWithLeader, TASK_PASS_FRAME);
}

/* Sets up a stake that stands sunk to its top from the start: drawn underfoot
 */
/* and behind the party in its sunk wooden pose, with no shadow, and neither */
/* moving nor colliding. */
void scYampiDesertSetStakeSunk(s32 id) {
	fdActorGetById(id)->priFlg |= ACTOR_PRIORITY_UNDERFOOT;
	fdActorSetSprPriority(id, 3);
	fdActorDisableById(id);
	fdActorSetAnimationById(id, 2);
}

/* Sets up the Psynergy Stone's room: its stakes, and the King Scorpion until */
/* it has been stirred. */
void scYampiDesertSetUpPsynergyStone(void) {
	fdEffectStartBlankOverlay(0);
	if(MainRoutineStateWork.entrance == ENTRANCE_FROM_SOUTH_PART ||
	   MainRoutineStateWork.entrance ==
		   YAMPI_DESERT_PSYNERGY_STONE_FROM_YAMPI_DESERT_KING_SCORPION) {
		if(fdEventGetFlag(FLAG_YAMPI_DESERT_SCORPION_STIRRED))
			fdActorSetPos(ACTOR_PSYNERGY_STONE_ROOM_KING_SCORPION, 0, 0);
		else
			scYampiDesertSetUpPsynergyStoneScorpion();
	}
	scYampiDesertPlaceStakes(YampiDesertPsynergyStoneStakes);
	if(fdEventGetFlag(FLAG_YAMPI_DESERT_ON_STAKE_TOPS)) {
		scYampiDesertStepOntoStakeTops();
		sySynch(10);
	}
	scYampiDesertSetStakeSunk(STAKE(4));
	scYampiDesertSetStakeSunk(STAKE(5));
	scYampiDesertSetStakeSunk(STAKE(6));
	scYampiDesertSetStakeSunk(STAKE(7));
	if(MainRoutineStateWork.entrance ==
		   YAMPI_DESERT_PSYNERGY_STONE_FROM_YAMPI_DESERT_BLITZ ||
	   MainRoutineStateWork.entrance == ENTRANCE_FROM_NORTH_PART)
		sySetEvent(scYampiDesertCheckUpperLevelFromLower, TASK_PASS_FRAME);
}

/* Sets up the King Scorpion's room: its stakes; in the south part, the King */
/* Scorpion once stirred from the Psynergy Stone's room; in the north, the ten
 */
/* stakes of the trap, and the scorpion until it is beaten, or the hole it */
/* leaves in the ring of rocks, which shows sand once the geyser is dug. */
void scYampiDesertSetUpKingScorpion(void) {
	fdEffectStartBlankOverlay(0);
	scYampiDesertPlaceStakes(YampiDesertKingScorpionStakes);
	scYampiDesertSetStakeSunk(STAKE(3));
	scYampiDesertSetStakeSunk(STAKE(4));
	scYampiDesertSetStakeSunk(STAKE(5));
	scYampiDesertSetStakeSunk(STAKE(6));
	scYampiDesertSetStakeSunk(STAKE(7));
	scYampiDesertSetStakeSunk(STAKE(8));
	scYampiDesertSetStakeSunk(STAKE(9));
	scYampiDesertSetStakeSunk(STAKE(10));
	scYampiDesertSetStakeSunk(STAKE(11));
	scYampiDesertSetStakeSunk(STAKE(12));
	if(MainRoutineStateWork.entrance ==
		   YAMPI_DESERT_KING_SCORPION_FROM_YAMPI_DESERT_PSYNERGY_STONE ||
	   MainRoutineStateWork.entrance == ENTRANCE_FROM_NORTH_PART) {
		if(syGameFlagGet(FLAG_YAMPI_DESERT_SCORPION_STIRRED) &&
		   !syGameFlagGet(FLAG_YAMPI_DESERT_SCORPION_STIRRED_AGAIN))
			scYampiDesertSetUpKingScorpionScorpion();
	}
	else {
		fdActorSetPos(STAKE(3), 200, 248);
		fdActorSetPos(STAKE(4), 168, 280);
		fdActorSetPos(STAKE(5), 152, 344);
		fdActorSetPos(STAKE(6), 184, 360);
		fdActorSetPos(STAKE(7), 328, 264);
		fdActorSetPos(STAKE(8), 296, 296);
		fdActorSetPos(STAKE(9), 344, 312);
		fdActorSetPos(STAKE(10), 312, 360);
		fdActorSetPos(STAKE(11), 280, 376);
		fdActorSetPos(STAKE(12), 328, 392);
		if(syGameFlagGet(FLAG_KING_SCORPION_BEATEN)) {
			if(syGameFlagGet(FLAG_YAMPI_DESERT_ROCK_PIT_DUG_UP)) {
				fdMapGridCopyCellTilesIndicesRect(68, 77, 13, 9, 3, 3);
				fdMapGridCopyCellAttributes(68, 77, 3, 3, 13, 9);
			}
			else {
				fdMapGridCopyCellTilesIndicesRect(74, 71, 77, 9, 3, 3);
				fdMapGridCopyCellAttributes(74, 71, 3, 3, 13, 9);
			}
			fdMapGridCopyCellAttributes(63, 77, 3, 3, 13, 73);
		}
		else if(syGameFlagGet(FLAG_YAMPI_DESERT_SCORPION_STIRRED_AGAIN)) {
			scYampiDesertSetUpTrappedScorpion();
		}
	}
}

/* Sets up the room north of the spring: its 315 coins stay hidden until dug */
/* up. */
void scYampiDesertSetUpCoinsPuddle(void) {
	fdEffectStartBlankOverlay(0);
	if(syGameFlagGet(FLAG_YAMPI_DESERT_COINS_PUDDLE_DUG_UP) == 0)
		fdActorSetWideSpr(ACTOR_HIDDEN_TREASURE, TREASURE_SPOT_PUT_AWAY);
}

/* Sets up the Trainer's Whip chest's room: its rope, sign and flowing */
/* sand, and the way in it was entered by. Entered from the Yampi Desert Cave,
 */
/* it sends Retreat to the World Map door the party last came in by (not in the
 */
/* Japanese edition). */
void scYampiDesertSetUpTrainersWhipChest(void) {
	ACTOR *actor = fdActorGetById(ACTOR_HIDDEN_TREASURE);

	scQueueMusic(SE_KIRIN);
	fdRopeInit();
	scYampiDesertPlaceRope(0, FLAG_YAMPI_DESERT_ROPE_TIED, ACTOR_ROPE_POST,
						   ACTOR_ROPE_OTHER_POST);
	if(actor != NULL)
		fdActorSetPos(ACTOR_HIDDEN_TREASURE, 0, 0);
	actor = fdActorGetById(ACTOR_SIGN);
	actor->scaleX = FIXED_ONE * 7 / 10;
	actor->scaleY = FIXED_ONE * 7 / 10;
	actor->radius = 8;
	scYampiDesertSetUpTrainersWhipChestBlocks();
	sySetEvent(scYampiDesertUpdateSandFlow, TASK_PASS_FRAME);
	if(MainRoutineStateWork.entrance ==
	   YAMPI_DESERT_TRAINERS_WHIP_CHEST_FROM_WORLD_MAP) {
		syGameFlagClear(FLAG_YAMPI_DESERT_ENTERED_BY_GUARDIAN_RING_CHEST);
		syGameFlagSet(FLAG_YAMPI_DESERT_ENTERED_BY_TRAINERS_WHIP_CHEST);
		syGameFlagClear(FLAG_YAMPI_DESERT_ENTERED_BY_HIDDEN_PSYNERGY_STONE);
	}
#if TLA_PATCH_1
	else if(
		MainRoutineStateWork.entrance ==
		YAMPI_DESERT_TRAINERS_WHIP_CHEST_FROM_YAMPI_DESERT_CAVE_WATER_OF_LIFE_CHEST) {
		if(syGameFlagGet(FLAG_YAMPI_DESERT_ENTERED_BY_GUARDIAN_RING_CHEST)) {
			MainRoutineStateWork.retreatScene =
				SCENE_YAMPI_DESERT_GUARDIAN_RING_CHEST;
			MainRoutineStateWork.retreatEntrance =
				YAMPI_DESERT_GUARDIAN_RING_CHEST_FROM_WORLD_MAP;
		}
		else if(syGameFlagGet(
					FLAG_YAMPI_DESERT_ENTERED_BY_TRAINERS_WHIP_CHEST)) {
			MainRoutineStateWork.retreatScene =
				SCENE_YAMPI_DESERT_TRAINERS_WHIP_CHEST;
			MainRoutineStateWork.retreatEntrance =
				YAMPI_DESERT_TRAINERS_WHIP_CHEST_FROM_WORLD_MAP;
		}
		else if(syGameFlagGet(
					FLAG_YAMPI_DESERT_ENTERED_BY_HIDDEN_PSYNERGY_STONE)) {
			MainRoutineStateWork.retreatScene =
				SCENE_YAMPI_DESERT_HIDDEN_PSYNERGY_STONE;
			MainRoutineStateWork.retreatEntrance =
				YAMPI_DESERT_HIDDEN_PSYNERGY_STONE_FROM_WORLD_MAP;
		}
	}
#endif
}

/* Sets up the room of the hidden Psynergy Stone, and the way in it was entered
 */
/* by. */
void scYampiDesertSetUpHiddenPsynergyStone(void) {
	if(fdActorGetById(ACTOR_HIDDEN_TREASURE) != 0)
		fdActorSetPos(ACTOR_HIDDEN_TREASURE, 0, 0);
	if(MainRoutineStateWork.entrance ==
	   YAMPI_DESERT_HIDDEN_PSYNERGY_STONE_FROM_WORLD_MAP) {
		syGameFlagClear(FLAG_YAMPI_DESERT_ENTERED_BY_GUARDIAN_RING_CHEST);
		syGameFlagClear(FLAG_YAMPI_DESERT_ENTERED_BY_TRAINERS_WHIP_CHEST);
		syGameFlagSet(FLAG_YAMPI_DESERT_ENTERED_BY_HIDDEN_PSYNERGY_STONE);
	}
}

/* The geyser Scoop opens in the scorpion pit: it rises and falls on */
/* its timer, carrying the platform at its top, as Gondowan Cliffs' geysers do.
 */
typedef struct GEYSER {
	s16 id;				/* its actor; 0 for a free entry */
	s16 tmr;			/* frames into its period */
	s16 height;			/* pixels it has risen */
	s16 period;			/* frames in one rise and fall */
	s16 riseAt;			/* the timer it starts to rise at */
	s16 fallAt;			/* the timer it starts to fall at */
	s16 max;			/* pixels it rises to */
	s16 min;			/* pixels it falls to */
	s16 unreadSetToOne; /* set to 1 by scGeyserAdd and never read */
	s16 step;			/* pixels a frame: 4 rising, -8 falling, 0 at rest */
	s16 unreadCleared;	/* cleared by scGeyserAdd and never read */
	s16 phase;			/* the platform's bob, 1/65536 turns */
	ACTOR *rider;		/* the platform at its top, which the leader rides */
} GEYSER;

/* The geysers' state, kept in the inventory snapshot's spare room. */
typedef struct GEYSER_WORK {
	s16 entry;		  /* the VRAM cache entry holding the column's tiles */
	s16 firstTerrain; /* the terrain id and collision slot of the first entry */
	GEYSER entries[GEYSER_ENTRIES];
	/* The OAM words btGeyserDraw queues: GEYSER_SPRITES sprites of three words.
	 */
	void *block;
} GEYSER_WORK;

void btGeyserStart(s32 unused, s32 firstTerrain);
GEYSER *scGeyserAdd(s32 slot, s32 id, s32 timer, s32 minHeight, s32 maxHeight);

/* Sets up the scorpion pit: dropped in, the King Scorpion's battle; back from
 */
/* it, the Scoop Gem; and the geyser between the four rocks once dug. */
void scYampiDesertSetUpScorpionPit(void) {
	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
	syGameFlagSet(FLAG_RETREAT_BLOCKED);
	if(MainRoutineStateWork.entrance ==
		   YAMPI_DESERT_SCORPION_PIT_FROM_YAMPI_DESERT_KING_SCORPION &&
	   !syGameFlagGet(FLAG_KING_SCORPION_BEATEN)) {
		scYampiDesertLandAndFightKingScorpion();
	}
	else {
		if(MainRoutineStateWork.entrance ==
			   YAMPI_DESERT_SCORPION_PIT_AFTER_KING_SCORPION_BATTLE &&
		   !syGameFlagGet(FLAG_KING_SCORPION_BEATEN))
			scYampiDesertDropScoopGem();
	}
	if(syGameFlagGet(FLAG_YAMPI_DESERT_ROCK_PIT_DUG_UP)) {
		fdMapGridCopyCellTilesIndicesRect(35, 39, 13, 11, 3, 3);
		fdActorSetPos(ACTOR_GEYSER, 232, 200);
	}
	btGeyserStart(0, GEYSER_FIRST_TERRAIN);
	scGeyserAdd(0, ACTOR_GEYSER, 0, 0, GEYSER_MAX_HEIGHT);
	sySynch(10);
}

/* Sets up the cave under the spring: it opens through a window transition. */
void scYampiDesertSetUpSpringCave(void) {
	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
}

/* Sets up the Blow Mace chest's room: its stake and its flowing sand. */
void scYampiDesertSetUpBlowMaceChest(void) {
	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
	scQueueMusic(SE_KIRIN);
	scYampiDesertPlaceStakes(YampiDesertBlowMaceChestStakes);
	scYampiDesertSetUpBlowMaceChestBlocks();
	syFieldTaskAddFrame(scYampiDesertUpdateSandFlow, TASK_PASS_FRAME);
}

/* Places each stake of a room's list, sunk if Pound has sunk it. */
void scYampiDesertPlaceStakes(const void *script) {
	const u16 *entry;

	if(*(const u16 *)script == ACTOR_LIST_END)
		return;
	for(entry = script; entry[0] != ACTOR_LIST_END; entry += 3) {
		s32 id = entry[0];
		ACTOR *actor = fdActorGetById(id);
		s32 kind = (s16)entry[1];
		s32 flag = (s16)entry[2];

		actor->collisionFlg &= ~ACTOR_SOLID;
		fdActorSetSprFlags(actor, 0);
		fdActorSetCellOpen(id, (u16)kind, (u16)flag, actor->layer);
	}
}
#undef WALK
#undef ARRIVE
#undef WALK_ON
#undef ARRIVE_OR

/* Finds the current target's table row and shrinks it. */
#include "shared/pound_sink_target.h"

#include "shared/actor_set_cell_open.h"

#include "shared/geyser_update.h"

#include "shared/geyser_draw.h"

#include "shared/geyser_start.h"

/* Makes actor id the geyser of entry slot, rising from minHeight to maxHeight
 */
/* pixels every six seconds, starting timer frames into its round, with an */
/* unseen platform at its top for the leader. */
GEYSER *scGeyserAdd(s32 slot, s32 id, s32 timer, s32 minHeight, s32 maxHeight) {
	GEYSER_WORK *state = GEYSER_STATE;
	GEYSER *entry = &state->entries[slot];
	ACTOR *actor = fdActorGetById(id);
	ACTOR *rider;

	/* DEAD: the actor looked up again and dropped; the ROM keeps the call. */
	fdActorGetById(id);
	fdActorSetSprFlags(actor, 0);
	if(!syGameFlagGet(FLAG_KEEP_PARTY_POSITION)) {
		if(maxHeight < minHeight) {
			s32 swap = minHeight;

			minHeight = maxHeight;
			maxHeight = swap;
		}
		entry->id = id;
		entry->height = 0;
		entry->tmr = timer;
		entry->period = GEYSER_PERIOD_FRAMES;
		entry->max = maxHeight;
		entry->riseAt = GEYSER_RISE_FRAME;
		entry->fallAt = GEYSER_FALL_FRAME;
		entry->min = minHeight;
		entry->unreadSetToOne = 1;
		actor->motionFlg = 0;
		actor->collisionFlg = ACTOR_NOT_TARGETED;
		entry->step = 0;
		entry->phase = 0;
		entry->unreadCleared = 0;
		actor->priFlg |= ACTOR_PRIORITY_UNDERFOOT;
	}
	rider = syImportActorCreate(ACTOR_CREATE_KIND(ACTOR_CREATE_NO_SPRITE),
								actor->x.fixed, actor->y.fixed, actor->z.fixed);
	rider->collisionFlg = 0;
	rider->motionFlg = 0;
	rider->priFlg |= ACTOR_PRIORITY_PLATFORM;
	entry->rider = rider;
	return entry;
}

/* clang-format off */
/* A field effect's actor that lasts a short, a medium or a long while, then */
/* goes back to the origin and frees itself (FieldEffect_DurationScripts). */
#include "shared/field_effect_duration_scripts.c"

/* Where the King Scorpion's cloud of dust rises: x and z in pixels. */
u16 YampiDesert_SandCloudPoints[SAND_CLOUD_POINTS * 2] ALIGNED(4) = {
    224, 152, 232, 152, 240, 152,
    216, 160, 224, 160, 232, 160, 240, 160, 248, 160,
    216, 168, 224, 168, 232, 168, 240, 168, 248, 168,
    216, 176, 248, 176,
};

/* A puff of the flowing sand's dust, which frees itself after a while. */
u32 YampiDesert_FlowDust[] = {
    SC_WAIT(25),
    SC_SET(SW_HOOK, 0),
    SC_POSITION_ORIGIN,
    SC_WAIT(1),
    SC_RELEASE,
};
/* clang-format on */

#undef MOVE_LEADER_TO
#undef fdActorSetAnimationAndWait
#undef fdActorSetMovementTypeAndWaitById

void scYampiDesertFightBlitz(void);
void scYampiDesertTalkToTraveler(void);
void scYampiDesertTalkToElderAboutWay(void);
void scYampiDesertStepOntoStakeTops(void);
void scYampiDesertHidePsynergyStone(void);
void scYampiDesertHideBlocks(void);

LATE_DATA const s32 *FieldEffect_DurationScripts[] = {
	FieldEffect_ShortScript,
	FieldEffect_MediumScript,
	FieldEffect_LongScript,
};

/* clang-format off */
/* The two travelers resting at the spring look about them in turn. */
LATE_DATA u32 YampiDesert_TravelerLooksAround[] = {
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

/* As YampiDesert_TravelerLooksAround, for the other traveler, out of step */
/* with the first. */
LATE_DATA u32 YampiDesert_OtherTravelerLooksAround[] = {
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

/* The mayor runs ahead to the travelers, and the others follow. */
LATE_DATA u32 YampiDesert_MayorRunsAhead[] = {
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

/* As YampiDesert_MayorRunsAhead, for the travelers who follow. */
LATE_DATA u32 YampiDesert_TravelersFollow[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(240, 0, 280),
    SC_SYNC,
    SC_MOVE(264, 0, 400),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_END,
};

/* As YampiDesert_MayorRunsAhead, for the last travelers, from further back. */
LATE_DATA u32 YampiDesert_LastTravelersFollow[] = {
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

/* Each room's stakes, as STAKE_ROW lists them. */
LATE_DATA u16 YampiDesertGuardianRingChestStakes[] = {
    STAKE_ROW(0), STAKE_ROW(1), STAKE_ROW(2), STAKE_ROW(3),
    STAKE_ROW(4), STAKE_ROW(5), ACTOR_LIST_END,
};

/* Blitz's room's stakes. */
LATE_DATA u16 YampiDesertBlitzStakes[] = {
    STAKE_ROW(0), STAKE_ROW(1), STAKE_ROW(2), STAKE_ROW(3),
    STAKE_ROW(4), STAKE_ROW(5), STAKE_ROW(6), STAKE_ROW(7),
    STAKE_ROW(8), STAKE_ROW(9), STAKE_ROW(10), ACTOR_LIST_END,
};

/* The Psynergy Stone's room's stakes. */
LATE_DATA u16 YampiDesertPsynergyStoneStakes[] = {
    STAKE_ROW(0), STAKE_ROW(1), STAKE_ROW(2), STAKE_ROW(3), ACTOR_LIST_END,
};

/* The King Scorpion's room's stakes. */
LATE_DATA u16 YampiDesertKingScorpionStakes[] = {
    STAKE_ROW(0), STAKE_ROW(1), STAKE_ROW(2), ACTOR_LIST_END,
};

/* The Blow Mace chest's room's stake. */
LATE_DATA ALIGNED(4) u16 YampiDesertBlowMaceChestStakes[] = {
    STAKE_ROW(0), ACTOR_LIST_END,
};

/* The trigger the scorpions raise for the scene to run, or 0. */
LATE_DATA s32 YampiDesert_RaisedEvent = 0;

/* The sand ripple that follows the room's King Scorpion under the sand. */
LATE_DATA ACTOR *YampiDesert_SandRipple = NULL;

#define SCENE_ENTRANCES_SECTION LATE_DATA
#include "shared/scene_entrances_default.c"

/* Where the party arrives in the cave under the spring. */
LATE_DATA SCENE_ENTRANCE YampiDesertSpringCaveEntrances[] = {
    {1, CONDITION_ALWAYS, 408, 0, 480, FACING_NORTH, 0, 288, 376, 528, 536, 0},
    {2, CONDITION_ALWAYS, 408, 0, 456, FACING_SOUTH, 0, 288, 376, 528, 536, 0},
    {3, CONDITION_ALWAYS, 120, 0, 424, FACING_SOUTH, 0, 0, 16, 448, 496, 0},
    {4, CONDITION_ALWAYS, 376, 0, 88, FACING_SOUTH, 0, 0, 16, 448, 496, 0},
    {SCENE_TABLE_END},
};

/* The region that leaves the scorpion's pit, a row: the box's least x, y */
/* and z and its greatest, in pixels, its condition (-1 always) and its exit; */
/* -1 ends the list. */
LATE_DATA s16 YampiDesertScorpionPitExitRegions[] ALIGNED(4) = {
    216, 44, 184, 248, 76, 216, -1, 1,
    -1, 0, 0, 0, 0, 0, 0, 0,
};

/* The two regions that leave the cave under the spring, in rows as the pit's. */
LATE_DATA s16 YampiDesertSpringCaveExitRegions[] = {
    112, 24, 416, 128, 40, 432, -1, 3,
    368, 24, 80, 384, 40, 96, -1, 4,
    -1, 0, 0, 0, 0, 0, 0, 0,
};

LATE_DATA u32 SceneExits[] = {
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

/* The actors of the rooms that place none. */
LATE_DATA ACTOR_PLACE YampiDesert_Placements[] = {
    {SCENE_TABLE_END},
};

/* The Guardian Ring chest's room's actors. */
LATE_DATA ACTOR_PLACE YampiDesertGuardianRingChestActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_STAKE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 296, 0, 216, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STAKE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 424, 0, 168, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STAKE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 456, 0, 136, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STAKE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 616, 0, 168, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STAKE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 632, 0, 392, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STAKE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 680, 0, 424, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

/* The spring's room's actors before the party meets the travelers. */
LATE_DATA ACTOR_PLACE YampiDesertSpringActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_MADRAN_SOLDIER, CONDITION_ALWAYS, (s32)YampiDesert_TravelerLooksAround, 200, 0, 216, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_MADRAN_SOLDIER, CONDITION_ALWAYS, (s32)YampiDesert_OtherTravelerLooksAround, 216, 0, 200, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_WITH_CURLY_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 152, 0, 200, FACING_SOUTH_SOUTHEAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_RUFFIAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 162, 0, 232, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_IN_GREY_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 212, 0, 246, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_MADRAS_ELDER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 230, 0, 248, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_MADRAS_MAYOR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 234, 0, 230, FACING_EAST, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

/* The spring's room's actors once the party has met the travelers. */
LATE_DATA ACTOR_PLACE YampiDesertSpringActorPlacementsAfterMeeting[] = {
    ACTOR_PLACEMENT(SPRITE_MADRAN_SOLDIER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 520, 0, 242, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_MADRAN_SOLDIER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 533, 0, 271, FACING_WEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_WITH_CURLY_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 435, 0, 323, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_RUFFIAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 467, 0, 248, FACING_SOUTH_SOUTHWEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_IN_GREY_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 462, 0, 340, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_MADRAS_ELDER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 482, 0, 340, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_MADRAS_MAYOR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 489, 0, 320, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    {SCENE_TABLE_END},
};

/* Blitz's room's actors. */
LATE_DATA ACTOR_PLACE YampiDesertBlitzActorPlacements[] = {
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

/* The Psynergy Stone's room's actors. */
LATE_DATA ACTOR_PLACE YampiDesertPsynergyStoneActorPlacements[] = {
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

/* The King Scorpion's room's actors. */
LATE_DATA ACTOR_PLACE YampiDesertKingScorpionActorPlacements[] = {
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

/* The Trainer's Whip chest's room's actors. */
LATE_DATA ACTOR_PLACE YampiDesertTrainersWhipChestActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 408, 0, 360, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 360, 0, 360, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_SIGN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 768, 0, 616, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 824, 0, 440, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 824, 0, 424, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

/* The scorpion's pit's actors. */
LATE_DATA ACTOR_PLACE YampiDesertScorpionPitActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_KING_SCORPION, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

/* The Blow Mace chest's room's actors. */
LATE_DATA ACTOR_PLACE YampiDesertBlowMaceChestActorPlacements[] = {
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

/* The triggers of the rooms that have none. */
LATE_DATA TRIGGER YampiDesert_Triggers[] = {
    {TRIGGER_TABLE_END},
};

/* The Guardian Ring chest's room's triggers. */
LATE_DATA TRIGGER YampiDesertGuardianRingChestTriggers[] = {
    {TRIGGER_EXIT, YAMPI_DESERT_GUARDIAN_RING_CHEST_TO_WORLD_MAP, CONDITION_ALWAYS, YAMPI_DESERT_GUARDIAN_RING_CHEST_TO_WORLD_MAP},
    {TRIGGER_EXIT, YAMPI_DESERT_GUARDIAN_RING_CHEST_TO_YAMPI_DESERT_SPRING, CONDITION_ALWAYS, YAMPI_DESERT_GUARDIAN_RING_CHEST_TO_YAMPI_DESERT_SPRING},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), STAKE(0), FLAG_YAMPI_DESERT_STAKE_POUNDED(0), EVENT(scYampiDesertPoundGuardianRingChestStake)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), STAKE(1), FLAG_YAMPI_DESERT_STAKE_POUNDED(1), EVENT(scYampiDesertPoundGuardianRingChestStake)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), STAKE(2), FLAG_YAMPI_DESERT_STAKE_POUNDED(2), EVENT(scYampiDesertPoundGuardianRingChestStake)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), STAKE(3), FLAG_YAMPI_DESERT_STAKE_POUNDED(3), EVENT(scYampiDesertPoundGuardianRingChestStake)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), STAKE(4), FLAG_YAMPI_DESERT_STAKE_POUNDED(4), EVENT(scYampiDesertPoundGuardianRingChestStake)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), STAKE(5), FLAG_YAMPI_DESERT_STAKE_POUNDED(5), EVENT(scYampiDesertPoundGuardianRingChestStake)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_AFTER_CAST), 0, CONDITION_ALWAYS, EVENT(scYampiDesertCastSand)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(scYampiDesertEndSand)},
    {TRIGGER_TABLE_END},
};

/* The spring's room's triggers. */
LATE_DATA TRIGGER YampiDesertSpringTriggers[] = {
    {TRIGGER_EXIT, YAMPI_DESERT_SPRING_TO_YAMPI_DESERT_GUARDIAN_RING_CHEST, CONDITION_ALWAYS, YAMPI_DESERT_SPRING_TO_YAMPI_DESERT_GUARDIAN_RING_CHEST},
    {TRIGGER_EXIT, YAMPI_DESERT_SPRING_TO_YAMPI_DESERT_COINS_PUDDLE, CONDITION_ALWAYS, YAMPI_DESERT_SPRING_TO_YAMPI_DESERT_COINS_PUDDLE},
    {TRIGGER_EXIT, YAMPI_DESERT_SPRING_TO_YAMPI_DESERT_SPRING_CAVE_NORTH, CONDITION_ALWAYS, YAMPI_DESERT_SPRING_TO_YAMPI_DESERT_SPRING_CAVE_NORTH},
    {TRIGGER_EXIT, YAMPI_DESERT_SPRING_TO_YAMPI_DESERT_BLITZ, CONDITION_ALWAYS, YAMPI_DESERT_SPRING_TO_YAMPI_DESERT_BLITZ},
    {STEP_TRIGGER_WALKING, CELL_AROUND_LADDER_PUDDLE, CONDITION_ALWAYS, EVENT(scYampiDesertClimbRevealedLadder)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_AFTER_CAST), 0, CONDITION_ALWAYS, EVENT(scYampiDesertCastSand)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(scYampiDesertEndSand)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_SCOOP, PSYNERGY_PHASE_ACTS), CELL_LADDER_PUDDLE, FLAG_YAMPI_DESERT_LADDER_DUG_UP, EVENT(scYampiDesertScoopLadderPit)},
    {TRIGGER_TALK, ACTOR_TRAVELER_FOLLOWING_THE_PIRATES, FLAG_YAMPI_DESERT_TRAVELERS_MET, EVENT(scYampiDesertTalkToTraveler)},
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
    {TRIGGER_TALK, ACTOR_TRAVELER_WHO_WOULD_TURN_BACK, FLAG_YAMPI_DESERT_TRAVELERS_MET, EVENT(scYampiDesertTalkToElderAboutWay)},
    {TRIGGER_TALK, ACTOR_TRAVELER_WHO_WOULD_TURN_BACK, CONDITION_ALWAYS, MSGNO(MsgOseniaIsSo)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TRAVELER_WHO_WOULD_TURN_BACK | TRIGGER_SUBJECT_OWN_EFFECT, FLAG_YAMPI_DESERT_ELDER_INSISTED, EVENT(scYampiDesertTalkToElderAboutWay)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TRAVELER_WHO_WOULD_TURN_BACK, FLAG_YAMPI_DESERT_TRAVELERS_MET, MSGNO(MsgWeVeReached)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TRAVELER_WHO_WOULD_TURN_BACK, CONDITION_ALWAYS, MSGNO(MsgHowMuchLonger)},
    {TRIGGER_TALK, ACTOR_ELDER, FLAG_YAMPI_DESERT_TRAVELERS_MET, EVENT(scYampiDesertTalkToElder)},
    {TRIGGER_TALK, ACTOR_ELDER, CONDITION_ALWAYS, MSGNO(MsgMySonHas)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ELDER | TRIGGER_SUBJECT_OWN_EFFECT, FLAG_YAMPI_DESERT_ELDER_INSISTED, EVENT(scYampiDesertTalkToElder)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ELDER, FLAG_YAMPI_DESERT_TRAVELERS_MET, MSGNO(MsgMySonIs)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ELDER, CONDITION_ALWAYS, MSGNO(MsgRegardlessOfWhat)},
    {TRIGGER_TALK, ACTOR_MAYOR, FLAG_YAMPI_DESERT_TRAVELERS_MET, MSGNO(MsgFatherHowAm)},
    {TRIGGER_TALK, ACTOR_MAYOR, CONDITION_ALWAYS, MSGNO(MsgIfWeRemain)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAYOR, FLAG_YAMPI_DESERT_TRAVELERS_MET, MSGNO(MsgMyFatherS)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAYOR, CONDITION_ALWAYS, MSGNO(MsgHoooICan)},
    {TRIGGER_STEP, CELL_LEDGE_OVER_SPRING, FLAG_YAMPI_DESERT_TRAVELERS_MET, EVENT(scYampiDesertTravelersLookUp)},
    {TRIGGER_TABLE_END},
};

/* Blitz's room's triggers. */
LATE_DATA TRIGGER YampiDesertBlitzTriggers[] = {
    {TRIGGER_EXIT, YAMPI_DESERT_BLITZ_TO_YAMPI_DESERT_SPRING, CONDITION_ALWAYS, YAMPI_DESERT_BLITZ_TO_YAMPI_DESERT_SPRING},
    {TRIGGER_EXIT, YAMPI_DESERT_BLITZ_TO_YAMPI_DESERT_PSYNERGY_STONE, CONDITION_ALWAYS, YAMPI_DESERT_BLITZ_TO_YAMPI_DESERT_PSYNERGY_STONE},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), STAKE(0), FLAG_YAMPI_DESERT_STAKE_POUNDED(0), EVENT(scYampiDesertPoundBlitzStake)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), STAKE(1), FLAG_YAMPI_DESERT_STAKE_POUNDED(1), EVENT(scYampiDesertPoundBlitzStake)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), STAKE(2), FLAG_YAMPI_DESERT_STAKE_POUNDED(2), EVENT(scYampiDesertPoundBlitzStake)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), STAKE(3), FLAG_YAMPI_DESERT_STAKE_POUNDED(3), EVENT(scYampiDesertPoundBlitzStake)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), STAKE(4), FLAG_YAMPI_DESERT_STAKE_POUNDED(4), EVENT(scYampiDesertPoundBlitzStake)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), STAKE(5), FLAG_YAMPI_DESERT_STAKE_POUNDED(5), EVENT(scYampiDesertPoundBlitzStake)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), STAKE(6), FLAG_YAMPI_DESERT_STAKE_POUNDED(6), EVENT(scYampiDesertPoundBlitzStake)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), STAKE(7), FLAG_YAMPI_DESERT_STAKE_POUNDED(7), EVENT(scYampiDesertPoundBlitzStake)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), STAKE(8), FLAG_YAMPI_DESERT_STAKE_POUNDED(8), EVENT(scYampiDesertPoundBlitzStake)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), STAKE(9), FLAG_YAMPI_DESERT_STAKE_POUNDED(9), EVENT(scYampiDesertPoundBlitzStake)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), STAKE(10), FLAG_YAMPI_DESERT_STAKE_POUNDED(10), EVENT(scYampiDesertPoundBlitzStake)},
    {TRIGGER_STEP, CELL_ONTO_STAKE_TOPS, FLAG_YAMPI_DESERT_ON_STAKE_TOPS, EVENT(scYampiDesertStepOntoStakeTops)},
    {TRIGGER_STEP, CELL_OFF_STAKE_TOPS, IF_FLAG_SET(FLAG_YAMPI_DESERT_ON_STAKE_TOPS), EVENT(scYampiDesertStepOffStakeTops)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_AFTER_CAST), 0, CONDITION_ALWAYS, EVENT(scYampiDesertCastSand)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(scYampiDesertEndSand)},
    {TRIGGER_TALK, ACTOR_BLITZ, CONDITION_ALWAYS, EVENT(scYampiDesertFightBlitz)},
    {TRIGGER_TABLE_END},
};

/* The Psynergy Stone's room's triggers. */
LATE_DATA TRIGGER YampiDesertPsynergyStoneTriggers[] = {
    {TRIGGER_EXIT, YAMPI_DESERT_PSYNERGY_STONE_TO_YAMPI_DESERT_BLITZ, CONDITION_ALWAYS, YAMPI_DESERT_PSYNERGY_STONE_TO_YAMPI_DESERT_BLITZ},
    {TRIGGER_EXIT, EXIT_TO_NORTH_PART, CONDITION_ALWAYS, EXIT_TO_NORTH_PART},
    {TRIGGER_EXIT, EXIT_TO_SOUTH_PART, CONDITION_ALWAYS, EXIT_TO_SOUTH_PART},
    {TRIGGER_EXIT, YAMPI_DESERT_PSYNERGY_STONE_TO_YAMPI_DESERT_KING_SCORPION, CONDITION_ALWAYS, YAMPI_DESERT_PSYNERGY_STONE_TO_YAMPI_DESERT_KING_SCORPION},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), STAKE(0), FLAG_YAMPI_DESERT_STAKE_POUNDED(0), EVENT(YampiDesert_PoundPsynergyStoneStake)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), STAKE(1), FLAG_YAMPI_DESERT_STAKE_POUNDED(1), EVENT(YampiDesert_PoundPsynergyStoneStake)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), STAKE(2), FLAG_YAMPI_DESERT_STAKE_POUNDED(2), EVENT(YampiDesert_PoundPsynergyStoneStake)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), STAKE(3), FLAG_YAMPI_DESERT_STAKE_POUNDED(3), EVENT(YampiDesert_PoundPsynergyStoneStake)},
    {TRIGGER_STEP, CELL_ONTO_STAKE_TOPS, FLAG_YAMPI_DESERT_ON_STAKE_TOPS, EVENT(scYampiDesertStepOntoStakeTops)},
    {TRIGGER_STEP, CELL_OFF_STAKE_TOPS, IF_FLAG_SET(FLAG_YAMPI_DESERT_ON_STAKE_TOPS), EVENT(scYampiDesertStepOffStakeTops)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_AFTER_CAST), 0, CONDITION_ALWAYS, EVENT(scYampiDesertCastSand)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(scYampiDesertEndSand)},
    {TRIGGER_RAISED, RAISED_WATCH_SCORPION_STOP, CONDITION_ALWAYS, EVENT(scYampiDesertWatchScorpionStop)},
    {TRIGGER_RAISED, RAISED_STIR_SCORPION, CONDITION_ALWAYS, EVENT(scYampiDesertStirScorpion)},
    {TRIGGER_TABLE_END},
};

/* The King Scorpion's room's triggers. */
LATE_DATA TRIGGER YampiDesertKingScorpionTriggers[] = {
    {TRIGGER_EXIT, YAMPI_DESERT_KING_SCORPION_TO_YAMPI_DESERT_PSYNERGY_STONE, CONDITION_ALWAYS, YAMPI_DESERT_KING_SCORPION_TO_YAMPI_DESERT_PSYNERGY_STONE},
    {TRIGGER_EXIT, EXIT_TO_NORTH_PART, CONDITION_ALWAYS, EXIT_TO_NORTH_PART},
    {TRIGGER_EXIT, EXIT_TO_SOUTH_PART, CONDITION_ALWAYS, EXIT_TO_SOUTH_PART},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), STAKE(0), FLAG_YAMPI_DESERT_STAKE_POUNDED(0), EVENT(YampiDesert_PoundKingScorpionStake)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), STAKE(1), FLAG_YAMPI_DESERT_STAKE_POUNDED(1), EVENT(YampiDesert_PoundKingScorpionStake)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), STAKE(2), FLAG_YAMPI_DESERT_STAKE_POUNDED(2), EVENT(YampiDesert_PoundKingScorpionStake)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_AFTER_CAST), 0, CONDITION_ALWAYS, EVENT(scYampiDesertCastSand)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(scYampiDesertEndSand)},
    {TRIGGER_RAISED, RAISED_WATCH_SCORPION_STOP, FLAG_YAMPI_DESERT_SCORPION_CORNERED, EVENT(scYampiDesertWatchScorpionStop)},
    {TRIGGER_RAISED, RAISED_STIR_SCORPION_AGAIN, CONDITION_ALWAYS, EVENT(scYampiDesertStirScorpionAgain)},
    {TRIGGER_RAISED, RAISED_CORNER_SCORPION, CONDITION_ALWAYS, EVENT(scYampiDesertCornerScorpion)},
    {TRIGGER_STEP, CELL_BEFORE_TRAP, FLAG_KING_SCORPION_BEATEN, EVENT(scYampiDesertTrapKingScorpion)},
    {TRIGGER_TABLE_END},
};

/* The triggers of the room north of the spring, with the 315 coins. */
LATE_DATA TRIGGER YampiDesertCoinsPuddleTriggers[] = {
    {TRIGGER_EXIT, YAMPI_DESERT_COINS_PUDDLE_TO_YAMPI_DESERT_SPRING, CONDITION_ALWAYS, YAMPI_DESERT_COINS_PUDDLE_TO_YAMPI_DESERT_SPRING},
    {TRIGGER_EXIT, YAMPI_DESERT_COINS_PUDDLE_TO_YAMPI_DESERT_BLOW_MACE_CHEST, CONDITION_ALWAYS, YAMPI_DESERT_COINS_PUDDLE_TO_YAMPI_DESERT_BLOW_MACE_CHEST},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_AFTER_CAST), 0, CONDITION_ALWAYS, EVENT(scYampiDesertCastSand)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(scYampiDesertEndSand)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_SCOOP, PSYNERGY_PHASE_ACTS), CELL_COINS_PUDDLE, FLAG_YAMPI_DESERT_COINS_PUDDLE_DUG_UP, EVENT(scYampiDesertDigUpCoins)},
    {TRIGGER_TABLE_END},
};

/* The sand pass's triggers. */
LATE_DATA TRIGGER YampiDesertSandPassTriggers[] = {
    {TRIGGER_EXIT, YAMPI_DESERT_SAND_PASS_TO_YAMPI_DESERT_BLOW_MACE_CHEST, CONDITION_ALWAYS, YAMPI_DESERT_SAND_PASS_TO_YAMPI_DESERT_BLOW_MACE_CHEST},
    {TRIGGER_EXIT, YAMPI_DESERT_SAND_PASS_TO_YAMPI_DESERT_TRAINERS_WHIP_CHEST, CONDITION_ALWAYS, YAMPI_DESERT_SAND_PASS_TO_YAMPI_DESERT_TRAINERS_WHIP_CHEST},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_AFTER_CAST), 0, CONDITION_ALWAYS, EVENT(scYampiDesertCastSand)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(scYampiDesertEndSand)},
    {TRIGGER_TABLE_END},
};

/* The Trainer's Whip chest's room's triggers. */
LATE_DATA TRIGGER YampiDesertTrainersWhipChestTriggers[] = {
    {TRIGGER_EXIT, YAMPI_DESERT_TRAINERS_WHIP_CHEST_TO_YAMPI_DESERT_SAND_PASS, CONDITION_ALWAYS, YAMPI_DESERT_TRAINERS_WHIP_CHEST_TO_YAMPI_DESERT_SAND_PASS},
    {TRIGGER_EXIT, YAMPI_DESERT_TRAINERS_WHIP_CHEST_TO_WORLD_MAP, CONDITION_ALWAYS, YAMPI_DESERT_TRAINERS_WHIP_CHEST_TO_WORLD_MAP},
    {TRIGGER_EXIT, YAMPI_DESERT_TRAINERS_WHIP_CHEST_TO_YAMPI_DESERT_HIDDEN_PSYNERGY_STONE, CONDITION_ALWAYS, YAMPI_DESERT_TRAINERS_WHIP_CHEST_TO_YAMPI_DESERT_HIDDEN_PSYNERGY_STONE},
    {TRIGGER_EXIT, YAMPI_DESERT_TRAINERS_WHIP_CHEST_TO_YAMPI_DESERT_CAVE_WATER_OF_LIFE_CHEST, CONDITION_ALWAYS, YAMPI_DESERT_TRAINERS_WHIP_CHEST_TO_YAMPI_DESERT_CAVE_WATER_OF_LIFE_CHEST},
    {PSYNERGY_ON_ACTOR(ABILITY_LASH), ACTOR_ROPE_POST, FLAG_YAMPI_DESERT_ROPE_TIED, 0},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_AFTER_CAST), 0, CONDITION_ALWAYS, EVENT(scYampiDesertCastSand)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(scYampiDesertEndSand)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0, CONDITION_ALWAYS, EVENT(scYampiDesertRevealRockHint)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(scYampiDesertHideRockHint)},
    {TRIGGER_TALK, ACTOR_SIGN, CONDITION_ALWAYS, EVENT(scYampiDesertReadSign)},
    {TRIGGER_STEP, CELL_FLOW_CORNER_INSIDE, IF_FLAG_SET(FLAG_YAMPI_DESERT_BLOCKS_SHOWN), EVENT(scYampiDesertHideBlocks)},
    {TRIGGER_STEP, CELL_FLOW_CORNER_OUTSIDE, FLAG_YAMPI_DESERT_BLOCKS_SHOWN, EVENT(scYampiDesertShowBlocks)},
    {TRIGGER_TABLE_END},
};

/* The triggers of the room of the hidden Psynergy Stone. */
LATE_DATA TRIGGER YampiDesertHiddenPsynergyStoneTriggers[] = {
    {TRIGGER_EXIT, YAMPI_DESERT_HIDDEN_PSYNERGY_STONE_TO_YAMPI_DESERT_TRAINERS_WHIP_CHEST, CONDITION_ALWAYS, YAMPI_DESERT_HIDDEN_PSYNERGY_STONE_TO_YAMPI_DESERT_TRAINERS_WHIP_CHEST},
    {TRIGGER_EXIT, YAMPI_DESERT_HIDDEN_PSYNERGY_STONE_TO_WORLD_MAP, CONDITION_ALWAYS, YAMPI_DESERT_HIDDEN_PSYNERGY_STONE_TO_WORLD_MAP},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0, CONDITION_ALWAYS, EVENT(scYampiDesertRevealPsynergyStone)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(scYampiDesertHidePsynergyStone)},
    {TRIGGER_TABLE_END},
};

/* The scorpion's pit's triggers. */
LATE_DATA TRIGGER YampiDesertScorpionPitTriggers[] = {
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_AFTER_CAST), 0, CONDITION_ALWAYS, EVENT(scYampiDesertCastSand)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(scYampiDesertEndSand)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_SCOOP, PSYNERGY_PHASE_ACTS), CELL_BETWEEN_FOUR_ROCKS, FLAG_YAMPI_DESERT_ROCK_PIT_DUG_UP, EVENT(scYampiDesertOpenGeyser)},
    {TRIGGER_TABLE_END},
};

/* The triggers of the cave under the spring. */
LATE_DATA TRIGGER YampiDesertSpringCaveTriggers[] = {
    {TRIGGER_EXIT, YAMPI_DESERT_SPRING_CAVE_TO_YAMPI_DESERT_SPRING_NORTH, CONDITION_ALWAYS, YAMPI_DESERT_SPRING_CAVE_TO_YAMPI_DESERT_SPRING_NORTH},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 2, CONDITION_ALWAYS, EVENT(scYampiDesertClimbDownSpringCaveLadder)},
    {TRIGGER_TABLE_END},
};

LATE_DATA TRIGGER YampiDesertBlowMaceChestTriggers[] = {
    {TRIGGER_EXIT, YAMPI_DESERT_BLOW_MACE_CHEST_TO_YAMPI_DESERT_COINS_PUDDLE, CONDITION_ALWAYS, YAMPI_DESERT_BLOW_MACE_CHEST_TO_YAMPI_DESERT_COINS_PUDDLE},
    {TRIGGER_EXIT, YAMPI_DESERT_BLOW_MACE_CHEST_TO_YAMPI_DESERT_SAND_PASS, CONDITION_ALWAYS, YAMPI_DESERT_BLOW_MACE_CHEST_TO_YAMPI_DESERT_SAND_PASS},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), STAKE(0), FLAG_YAMPI_DESERT_STAKE_POUNDED(0), EVENT(scYampiDesertPoundBlowMaceChestStake)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_AFTER_CAST), 0, CONDITION_ALWAYS, EVENT(scYampiDesertCastSand)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(scYampiDesertEndSand)},
    {TRIGGER_TABLE_END},
};

/* A puff of the King Scorpion's dust drifts aside and frees itself. */
LATE_DATA u32 YampiDesert_SandCloudDrifts[] = {
    SC_WAIT(1),
    SC_MOVE(232, 0, 186),
    SC_SYNC,
    SC_RELEASE,
};
/* clang-format on */
