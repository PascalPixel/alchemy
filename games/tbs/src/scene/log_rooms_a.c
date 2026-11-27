// Log rooms A (SceneIdLogRoomsA), a rooms map drawn in logs, holds only Vault's rooms while
// Hammet's Shaman's Rod is missing and just after it is found: the insides of the town's houses,
// shops and inn, the inn's attic and its loft.
//
// Every room is on one map, each an entrance with a camera box of its own. Vault's doors (vault.c)
// lead to the entrances named for them:
// - the gem owner's house, nearest the town's lower gate, where a big pale
//       blue gem stands on the round table;
// - the house of a burly grey-haired man and a small boy, in the middle of
//       town;
// - the inn, with the guests' room and its two beds upstairs, where the
//       three strangers stay;
// - the item shop, which is also the home of Grandpa and his wife;
// - the weapon and armor shop;
// - the mayor's house, where Ivan is.
//
// The ladder outside up to the inn's broken roof (LOG_ROOMS_A_FROM_VAULT_WEST) comes into the
// attic, a small room with a big crate and a dark hole in the floor. Behind the crate is the way
// into the loft, where the thieves hid the stolen goods.
//
// The loft has actor placements and scripts of its own (scGetActorPlacements and scGetCues).
// FLAG_VAULT_THIEVES_JAILED chooses the other rooms' tables.
//
// The story flags drive the rooms in order:
// - At the mayor's, talking to Ivan plays scLogRoomsAMeetIvan. He reads
//       Isaac's mind (FLAG_VAULT_MET_IVAN) and asks for help with the rod. On a
//       yes he joins the party (scLogRoomsAIvanJoinsRodSearch,
//       FLAG_VAULT_IVAN_JOINED_ROD_SEARCH).
// - In the guests' room upstairs at the inn, the two suspects see Ivan with
//       the party and run from him round the beds
//       (scLogRoomsAApproachSuspects). Stepping back out of the room, the party
//       plans to surround one of them (scLogRoomsADiscussSuspiciousGuests).
//       Going back in starts the chase (scLogRoomsAStartCorneringSuspects):
// Garet spreads his arms and Ivan follows, and the suspects dodge between spots
//       round the beds (LogRoomsAStops). Once one of them has been boxed in
//       for half a second, Ivan reads both minds
//       (scLogRoomsAReadCorneredSuspectsMinds, FLAG_VAULT_SUSPECTS_MIND_READ).
//       During the chase the menu only gets Garet's "We don't have time for
//       that" (scLogRoomsAGaretHasNoTimeForMenu).
// - In the attic, Move pushes the crate aside (scLogRoomsACheckCrateMoved,
//       FLAG_VAULT_CRATE_MOVED), and the way behind it leads into the loft
//       (scLogRoomsAEnterLoft). Cutting free the young man tied up there brings
//       out the three thieves, who attack
// (scLogRoomsAFreeVillagerAndFightThieves, FLAG_VAULT_THIEVES_ATTACKED). After a loss the thieves
// gloat over the fallen party, which comes to in the Vault Sanctum (scLogRoomsALoseToThieves).
// Coming back up replays the challenge
//       (scLogRoomsAFightThievesAgain). After a win (scLogRoomsADefeatThieves,
//       scLogRoomsAReturnStolenGoods) the mayor comes up and has the thieves
//       taken to jail (FLAG_VAULT_THIEVES_JAILED), the chests give back the
//       stolen goods and the Shaman's Rod, and Ivan leaves the party to rescue
//       Hammet from Lunpa.
// - Once the thieves are jailed, the mayor gives the promised Water of Life
//       (FLAG_VAULT_WATER_OF_LIFE_RECEIVED) and the girl at the inn offers
//       Bones (FLAG_VAULT_BONE_RECEIVED), with more on the barrel beside her
//       (scVaultFindBone) for the dog in town.
//
// The shops and the inn open across their counters when Isaac faces north. Spoken to from anywhere
// else, their keepers only talk.
//
// A jar, two barrels and a wooden box hold coins, a Sleep Bomb and a Mint, each found once, and two
// bookcases can be read.
//
// The metatiles the scripts answer are LOG_ROOMS_A_METATILE_BEHAVIOR (log_rooms_a.h).
//
// The three metatiles of the guests' room doorway, (14, 44) to (16, 44), change what they do as the
// story goes on. At first, stepping on them sets off scLogRoomsAApproachSuspects. After it they are
// rewritten from copies kept lower on the map, so that stepping on them on the way out plays
// scLogRoomsADiscussSuspiciousGuests, and after that, on the way back in,
// scLogRoomsAStartCorneringSuspects. Every later arrival with FLAG_VAULT_SUSPECTS_APPROACHED set
// rewrites them again.

#include "scene/log_rooms_a.h"
#include "ability_ids.h"
#include "flags.h"
#include "item_ids.h"
#include "macros.h"
#include "debug/debug.h"
#include "field/actor_script.h"
#include "field/script_inline.h"
#include "field/map_grid.h"
#include "graphics/graphics.h"
#include "scene/vault_item_icon.h"
#include "sound/sound_driver_symbols.h"
#include "text/msg_ids.h"

// The lines of Ivan's joining (scLogRoomsAIvanJoinsRodSearch), counted from
// MsgVaultThankHelpBelieve, where the script moves on to a new one.
typedef enum IVAN_JOINING_MESSAGE
{
	IVAN_JOINING_ISAAC_CLASPS_HANDS    = 5,
	IVAN_JOINING_GARET_FEELS_MIND_READ = 6,
	IVAN_JOINING_GARET_CLASPS_HANDS    = 13,
	IVAN_JOINING_GARET_READS_MIND      = 14,
	IVAN_JOINING_IVAN_WILL_READ_TOWN   = 22
} IVAN_JOINING_MESSAGE;

// A link's stopIndex where a stop has fewer than three links.
enum
{
	LOG_STOP_NONE = 255
};

// The chase's stops (LogRoomsAStops), LOG_STOP_COUNT of them, are placed in 8-pixel half metatiles
// counted from (LOG_STOPS_ORIGIN_X, LOG_STOPS_ORIGIN_Z); each covers two by two of them, around its
// middle.
#define LOG_STOP_COUNT     37
#define LOG_STOPS_ORIGIN_X 64
#define LOG_STOPS_ORIGIN_Z 624
// The 16.16 middle of a stop.
#define LOG_STOP_X(stop)                                                                           \
	(FIELD_HALF_METATILE_ORIGIN((stop)->gridX) + toFIXED(LOG_STOPS_ORIGIN_X + 8))
#define LOG_STOP_Z(stop)                                                                           \
	(FIELD_HALF_METATILE_ORIGIN((stop)->gridZ) + toFIXED(LOG_STOPS_ORIGIN_Z + 8))

// Where Move leaves the crate aside, its map column, which opens the way up to the inn's loft.
#define CRATE_MOVED_COLUMN 42

// One of a stop's links: the stop it leads to and the heading toward it.
typedef struct LOG_ROOMS_A_STOP_CONNECTION
{
	Uint8 stopIndex; // the stop it leads to, or LOG_STOP_NONE
	Uint8 facing;    // the heading toward it, a facing's high byte
	Uint8 padding[2];
} LOG_ROOMS_A_STOP_CONNECTION;

// A stop on the route network, two half metatiles square, with its links to others.
typedef struct LOG_ROOMS_A_STOP
{
	Uint8                       gridX; // half metatiles from LOG_STOPS_ORIGIN_X
	Uint8                       gridZ; // half metatiles from LOG_STOPS_ORIGIN_Z
	Uint8                       padding[2];
	LOG_ROOMS_A_STOP_CONNECTION connections[3];
} LOG_ROOMS_A_STOP;

STATIC_ASSERT(sizeof(LOG_ROOMS_A_STOP_CONNECTION) == 4);
STATIC_ASSERT(sizeof(LOG_ROOMS_A_STOP) == 16);

extern LOG_ROOMS_A_STOP  LogRoomsAStops[LOG_STOP_COUNT];
extern Sint32            LogRoomsABeatenThiefGasps[];
extern Sint32            LogRoomsAFirstThiefEntryScript[];
extern Sint32            LogRoomsASecondThiefEntryScript[];
extern Sint32            LogRoomsAVillagerLeavesLoft[];
extern Sint32            LogRoomsATiedThiefLeavesLoft[];
extern Sint32            LogRoomsAToSecondChest[];
extern Sint32            LogRoomsAToFirstChest[];
extern Sint32            LogRoomsABackFromChests[];
extern Sint32            LogRoomsASuspectToWestBedHead[];
extern Sint32            LogRoomsASuspectRoundToWestBedHead[];
extern Sint32            LogRoomsASuspectToMiddleBedHead[];
extern Sint32            LogRoomsASuspectToEastBedHead[];
extern Sint32            LogRoomsASuspectRoundToEastBedHead[];
extern Sint32            LogRoomsASuspectToWestBedFoot[];
extern Sint32            LogRoomsASuspectRoundToWestBedFoot[];
extern Sint32            LogRoomsASuspectToMiddleBedFoot[];
extern Sint32            LogRoomsASuspectToEastBedFoot[];
extern Sint32            LogRoomsASuspectRoundToEastBedFoot[];
extern Sint32            LogRoomsASuspectAcrossToWestOfBeds[];
extern Sint32            LogRoomsASuspectToWestOfBeds[];
extern Sint32            LogRoomsASuspectAcrossToEastOfBeds[];
extern Sint32            LogRoomsASuspectToEastOfBeds[];
extern Sint32            LogRoomsASuspectToSouthWestCorner[];
extern Sint32            LogRoomsASuspectAlongToSouthWestCorner[];
extern Sint32            LogRoomsASuspectToSouthEastCorner[];
extern Sint32            LogRoomsASuspectAlongToSouthEastCorner[];
extern SCENE_ENTRANCE    LogRoomsAEntrances[];
extern SCENE_EXIT_REGION LogRoomsARegions[];
extern Uint32            LogRoomsAExits[];
extern ACTOR_PLACE       LogRoomsAActorPlacements[];
extern ACTOR_PLACE       LogRoomsALoftActorPlacements[];
extern CUE               LogRoomsACues[];
extern CUE               LogRoomsALoftCues[];
extern CUE               LogRoomsACuesAfterThieves[];
extern Sint32           *LogRoomsALeaderRunsOn[];
extern Sint32           *LogRoomsALeaderRunsBack[];
extern Sint32           *LogRoomsAThiefRuns[];

// Where the mayor stands once the thieves are jailed: a step south of his place by the east wall,
// where the French, Spanish and Italian editions leave him.
#define MAYOR_AFTER_THIEVES_Z WHEN(VAULT_MAYOR_STAYS_IN_PLACE_AFTER_THIEVES, 451, 464)

// The spots the suspected leader runs between in the guests' room, kept in his timer: the three
// beds' heads, west to east, then their feet, east to west (LogRoomsALeaderRunsOn runs him from
// each to the next, and on round).
typedef enum LOG_ROOMS_A_LEADER_SPOT
{
	LEADER_SPOT_WEST_BED_HEAD,
	LEADER_SPOT_MIDDLE_BED_HEAD,
	LEADER_SPOT_EAST_BED_HEAD,
	LEADER_SPOT_EAST_BED_FOOT,
	LEADER_SPOT_MIDDLE_BED_FOOT,
	LEADER_SPOT_WEST_BED_FOOT
} LOG_ROOMS_A_LEADER_SPOT;

// The spots the suspected thief runs between, kept in his timer: west and east of the beds and the
// south corners, which LogRoomsAThiefRuns runs him between, and a spot on the south wall, (120,
// 696), from which he runs to either corner.
typedef enum LOG_ROOMS_A_THIEF_SPOT
{
	THIEF_SPOT_WEST_OF_BEDS,
	THIEF_SPOT_EAST_OF_BEDS,
	THIEF_SPOT_SOUTH_EAST_CORNER,
	THIEF_SPOT_SOUTH_WEST_CORNER,
	THIEF_SPOT_SOUTH_WALL
} LOG_ROOMS_A_THIEF_SPOT;

// A cornering lasts half a second before the chase raises a script.
#define LOG_ROOMS_A_CORNERED_TICKS 30

// The chase adds the blocked-suspect mask (1 the leader, 2 the thief) to 200.
typedef enum LOG_ROOMS_A_RAISED_CUE
{
	LOG_ROOMS_A_CORNERED_SCRIPT_BASE = 200,
	LOG_ROOMS_A_LEADER_CORNERED,
	LOG_ROOMS_A_THIEF_CORNERED,
	LOG_ROOMS_A_BOTH_CORNERED
} LOG_ROOMS_A_RAISED_CUE;

// The townspeople, named by what they say, numbering LogRoomsAActorPlacements' rows from
// ACTOR_FIRST_PLACED. Garet's and Ivan's rows take their own slots.
enum
{
	// A young man, to whose house a sparkling rock was blown from the volcano.
	ACTOR_GEM_OWNER                        = 8,
	// A woman with fair hair.
	ACTOR_GEM_OWNERS_WIFE                  = 9,
	// A burly grey-haired man who distrusts the man next door. "My neighbor creeps me out."
	ACTOR_VILLAGER_SUSPECTING_THE_NEIGHBOR = 10,
	// A small boy who asks about travel.
	ACTOR_BOY_ASKING_ABOUT_TRAVEL          = 11,
	// An old man with a cane who wonders what was stolen.
	ACTOR_GRANDPA                          = 12,
	// An old woman, who says Grandpa keeps wondering if anything was stolen.
	ACTOR_GRANDPAS_WIFE                    = 13,
	// A small boy. "I could help Ivan if I were just a little older."
	ACTOR_BOY_WISHING_TO_BE_OLDER          = 14,
	// A little girl who tells the party Ivan's secret once it helps him.
	ACTOR_GIRL_TELLING_A_SECRET            = 15,
	// Ivan's host, an old man with white hair.
	ACTOR_MAYOR                            = 16,
	// A woman in a green dress.
	ACTOR_MAYORS_WIFE                      = 17,
	// The girl at the inn, as the Bone's description calls her, a young woman with fair hair. "I
	// have a lot of leftover bones from dinner."
	ACTOR_GIRL_AT_THE_INN                  = 18,
	ACTOR_WEAPON_MERCHANT                  = 19,
	ACTOR_ARMOR_MERCHANT                   = 20,
	ACTOR_ITEM_MERCHANT                    = 21,
	// A burly grey-haired man who works at the inn.
	ACTOR_INN_STAFF                        = 22,
	// A woman in an apron behind the inn's counter.
	ACTOR_INNKEEPER                        = 23,
	// The two guests at the inn the town suspects: the thieves' leader and one of his men.
	ACTOR_SUSPECTED_LEADER                 = 24,
	ACTOR_SUSPECTED_THIEF                  = 25,
	// Pushed aside to open a passage.
	ACTOR_CRATE                            = 26
};

// Area flags of Vault's area (0x300 to 0x3ff, kept while the party stays in Vault and its
// buildings).
enum
{
	// Set while the two suspects run from the party, until the party steps aside to talk it over.
	FLAG_SUSPECTS_FLEEING = FLAG_AREA_TEMP_00
};

// The scene flag the inn keeps as FLAG_VAULT_LEADER_BESIDE_BONE
// (shared/vault_leader_beside_bone_flag.c) once the thieves are jailed means something else in the
// guests' room: The party is cornering the two suspects in the guests' room, so talking to Ivan or
// the suspects does nothing.
#define FLAG_VAULT_CORNERING_SUSPECTS FLAG_SCENE_TEMP_50

// The inn's loft (LOG_ROOMS_A_LOFT_FROM_CRATE_ROOM and the two after the battle), where the thieves
// hid the stolen goods, numbering LogRoomsALoftActorPlacements' rows from ACTOR_FIRST_PLACED.
enum
{
	// vault.c's ACTOR_YOUNG_MAN_FIXING_THE_ROOF, tied up while fixing the roof.
	ACTOR_TIED_UP_VILLAGER             = 8,
	ACTOR_LOFT_MAYOR                   = 9,
	// Speaks first and bargains for all three ("See, we're not so bad. How about it? Will you let
	// us go?").
	ACTOR_THIEF_LEADER                 = 10,
	ACTOR_FIRST_THIEF                  = 11,
	ACTOR_SECOND_THIEF                 = 12,
	// A strongman who comes up with the mayor and helps tie the thieves up.
	ACTOR_LOFT_ESCORT                  = 13,
	// A strongman who comes up with the mayor and opens the chests.
	ACTOR_STRONGMAN_OPENING_THE_CHESTS = 14,
	ACTOR_FIRST_CHEST                  = 15,
	ACTOR_SECOND_CHEST                 = 16,
	ACTOR_THIRD_CHEST                  = 17,
	// Rises from the first chest.
	ACTOR_FOUND_URN                    = 18,
	// Rises from the second chest, stolen from the Sanctum.
	ACTOR_FOUND_STATUE                 = 19
};

// Animations of the loft's sprites beyond ACTOR_ANIMATION, as their animation tables and ticks
// under graphics/sprite draw them.
typedef enum LOG_ROOMS_A_ANIMATION
{
	// Garet seen from behind, his arms spread wide (garet.png).
	ANIM_GARET_ARMS_SPREAD = 12,
	// A thief standing bound, a rope wound round his arms (man_with_long_dark_hair.png,
	// person_in_purple.png).
	ANIM_THIEF_TIED_UP     = 5,
	ANIM_THIEF_TIED_UP_WALK =
		6, // walking with the rope round his arms
	       // lying on the floor (man_with_long_dark_hair.png, person_in_purple.png)
	ANIM_THIEF_BEATEN = 9
} LOG_ROOMS_A_ANIMATION;

static void scLogRoomsAIvanJoinsRodSearch(void);
static void scLogRoomsADiscussMindReading(void);
static void scLogRoomsAFaceSuspectsTowardLeader(void);
static void
scLogRoomsAAdvanceSuspectRoute(Sint32 actorId, Sint32 expected, Sint32 next, const Sint32 *actions);
static void scLogRoomsALoseToThieves(void);
static void scLogRoomsAFightThievesAgain(void);
static void scLogRoomsADefeatThieves(void);
static void scLogRoomsAReturnStolenGoods(void);
static void scLogRoomsAOpenWindowTransition(void);
static void scLogRoomsAShowMessageAndWait(Sint32 actorId, Sint32 waitTicks);
static void scLogRoomsAFaceActorAndWait(Sint32 actorId, Sint32 targetId, Sint32 waitTicks);
static void scLogRoomsAFaceEachOtherAndWait(Sint32 firstId, Sint32 secondId, Sint32 waitTicks);
static void scLogRoomsAPlayAnimationAndWait(Sint32 actorId, Sint32 animation, Sint32 waitTicks);
static void scStartMindRead(Sint32 casterId, Sint32 targetId);
static void scEndMindRead(void);
static void scLogRoomsAUpdateCorneringChase(void);
static void scLogRoomsASpreadOutToCornerSuspects(void);

const SCENE_ENTRANCE *scGetEntrances(void)
{
	return LogRoomsAEntrances;
}

// Returns the one exit region: the box around (648, 416) in the room with the crate, which takes
// LOG_ROOMS_A_TO_VAULT_WEST back out to Vault.
const SCENE_EXIT_REGION *scGetExitRegions(void)
{
	return LogRoomsARegions;
}

const Uint32 *scGetExits(void)
{
	return LogRoomsAExits;
}

// Places the townspeople, or, for the inn's loft (LOG_ROOMS_A_LOFT_FROM_CRATE_ROOM and the two
// after the battle), the tied-up villager, the mayor, the thieves, their chests and the found
// treasures.
const ACTOR_PLACE *scGetActorPlacements(void)
{
	ACTOR_PLACE *table;

	switch (MainRoutineStateWork.entrance)
	{
		case LOG_ROOMS_A_LOFT_FROM_CRATE_ROOM:
		case LOG_ROOMS_A_LOFT_AFTER_LOSING_TO_THIEVES:
		case LOG_ROOMS_A_LOFT_AFTER_THIEVES_DEFEATED : table = LogRoomsALoftActorPlacements; break;
		default                                      : table = LogRoomsAActorPlacements; break;
	}
	fdAssignViewFlags(table);
	return table;
}

#if !JUMP_LANDS_ON_CELL_CENTER
// Jumps the leader to the requested destination when nothing blocks it there. Returns 1 when it
// jumped.
static Bool scLogRoomsATryJumpTo(FIELD_POS *pos)
{
	ACTOR *leader;
	Uint8  savedMotionFlags;

	leader           = fdActorGet(ACTOR_PARTY_LEADER);
	savedMotionFlags = leader->motionFlags;
	if (syFieldActorCheckMovementCollision(leader, pos) == 0)
	{
		fdScriptStart();
		fdActorSelectAnimation(leader, ANIM_LEADER_SPRING);
		sySynch(6);
		sySoundPlay(SE_JUMP);
		fdActorSelectAnimation(leader, ANIM_LEADER_LEAP);
		leader->speedLimit    = toFIXED(3);
		leader->acceleration  = toFIXED(2);
		leader->velocityY     = toFIXED(4);
		leader->motionFlags  &= ACTOR_JUMP_KEEPS_MOTION;
		fdActorSetSpriteFlags(leader, 0);
		fdActorMoveToAndWait(ACTOR_PARTY_LEADER,
		                     ((FIELD_COORDINATE *)&pos->x)->part[0].integer,
		                     ((FIELD_COORDINATE *)&pos->z)->part[0].integer);
		fdActorSelectAnimation(leader, ANIM_LEADER_SPRING);
		fdActorSetSpriteFlags(leader, SPRITE_FLAG_SHADOW);
		sySynch(6);
		leader->motionFlags = savedMotionFlags;
		fdScriptEnd();
		return TRUE;
	}
	return FALSE;
}
#endif

// In the inn's attic, walking north or south at the dark hole in the floor (metatiles (41, 25) and
// (41, 27), either side of it), Isaac jumps across.
//
// The overhead layer's metatile over the hole's north edge, (42, 25) (the map's row 85, the layer
// beginning at row 60), then takes the attributes of the one west of it, floor 32 pixels up, as he
// lands north, or of the one east of it, none, as he lands south (render probe scene LogRoomsA
// --entrance 13 --metatiles 40,84,44,87). XXX: what the overhead metatile's floor changes for the
// leader there; a probe of metatile (42, 85) and of his height after each jump would settle it.
static void scLogRoomsAJumpAcrossHoleInAtticFloor(void)
{
	Sint32 x;
	Sint32 z;
#if !JUMP_LANDS_ON_CELL_CENTER
	FIELD_POS target;
#endif

	if (fdActorGet(ACTOR_PARTY_LEADER)->facing >= FACING_NORTHWEST
	    && fdActorGet(ACTOR_PARTY_LEADER)->facing <= FACING_NORTHEAST)
	{
#if JUMP_LANDS_ON_CELL_CENTER
		fdTryJumpForward();
#else
		SET_POSITION(&target,
		             fdActorGet(ACTOR_PARTY_LEADER)->x.fixed,
		             fdActorGet(ACTOR_PARTY_LEADER)->y.fixed,
		             fdActorGet(ACTOR_PARTY_LEADER)->z.fixed - toFIXED(30));
		scLogRoomsATryJumpTo(&target);
#endif
		x = 42;
		z = 85;
		fdMapGridCopyMetatileAttributes(41, 85, 1, 1, x, z);
	}
	else if (fdActorGet(ACTOR_PARTY_LEADER)->facing >= FACING_SOUTHEAST
	         && fdActorGet(ACTOR_PARTY_LEADER)->facing <= FACING_SOUTHWEST)
	{
#if JUMP_LANDS_ON_CELL_CENTER
		fdTryJumpForward();
#else
		SET_POSITION(&target,
		             fdActorGet(ACTOR_PARTY_LEADER)->x.fixed,
		             fdActorGet(ACTOR_PARTY_LEADER)->y.fixed,
		             fdActorGet(ACTOR_PARTY_LEADER)->z.fixed + toFIXED(30));
		scLogRoomsATryJumpTo(&target);
#endif
		x = 42;
		z = 85;
		fdMapGridCopyMetatileAttributes(43, 85, 1, 1, x, z);
	}
}

// Move on the big crate in the inn's attic: once it has been pushed a metatile west, to map column
// 42, the way behind it into the loft opens.
static void scLogRoomsACheckCrateMoved(void)
{
	fdScriptStart();
	if (FIELD_METATILE(fdActorGet(ACTOR_CRATE)->x.fixed) == CRATE_MOVED_COLUMN)
	{
		fdMapGridCopyMetatileAttributes(101, 24, 3, 4, 41, 24);
		gmFlagSet(FLAG_VAULT_CRATE_MOVED);
	}
	fdScriptEnd();
}

// Spoken to across the counter of Vault's weapon and armor shop, the weapon merchant opens his
// shop. From anywhere else he says Hammet wasn't the only one robbed, or, once the thieves are
// jailed, that their stolen weapons were found rusted in the cemetery.
static void scLogRoomsATalkToWeaponMerchant(void)
{
	Sint32 leaderFacing = fdActorGet(ACTOR_PARTY_LEADER)->facing;

	fdScriptStart();
	if (leaderFacing > FACING_NORTHWEST && leaderFacing < FACING_NORTHEAST)
	{
		mnShopRun(VAULT_WEAPON_STOCK, ACTOR_WEAPON_MERCHANT);
	}
	else
	{
		if (gmFlagGet(FLAG_VAULT_THIEVES_JAILED) == 0)
		{
			fdScriptSetMessageId(MsgVaultMasterHammetIsntOnlyOne);
		}
		else
		{
			fdScriptSetMessageId(MsgVaultWeFoundOurStolenWeapons);
		}
		fdScriptShowMessage(ACTOR_WEAPON_MERCHANT, 0);
	}
	fdScriptEnd();
}

// Spoken to across the counter of Vault's weapon and armor shop, the armor merchant opens his shop.
// From anywhere else he is angry at the thieves, or, once they are jailed, warns the party to be
// prepared for the cave in Goma Range.
static void scLogRoomsATalkToArmorMerchant(void)
{
	Sint32 leaderFacing = fdActorGet(ACTOR_PARTY_LEADER)->facing;

	fdScriptStart();
	if (leaderFacing > FACING_NORTHWEST && leaderFacing < FACING_NORTHEAST)
	{
		mnShopRun(VAULT_ARMOR_STOCK, ACTOR_ARMOR_MERCHANT);
	}
	else
	{
		if (gmFlagGet(FLAG_VAULT_THIEVES_JAILED) == 0)
		{
			fdScriptSetMessageId(MsgVaultThievesDidntHitOurHouse);
		}
		else
		{
			fdScriptSetMessageId(MsgVaultIfYoureGonnaHeadInto);
		}
		fdScriptShowMessage(ACTOR_ARMOR_MERCHANT, 0);
	}
	fdScriptEnd();
}

// Spoken to across the counter of Vault's inn, the woman behind it offers a room. From anywhere
// else she thinks it unfair to accuse the polite guests upstairs, or, once they are jailed, is
// shocked they hid the stolen goods in the loft.
static void scLogRoomsATalkToInnkeeper(void)
{
	Sint32 leaderFacing = fdActorGet(ACTOR_PARTY_LEADER)->facing;

	fdScriptStart();
	if (leaderFacing > FACING_NORTHWEST && leaderFacing < FACING_NORTHEAST)
	{
		mnInnCheckIn(VAULT_INN, ACTOR_INNKEEPER);
	}
	else
	{
		if (gmFlagGet(FLAG_VAULT_THIEVES_JAILED) == 0)
		{
			fdScriptSetMessageId(MsgVaultEveryoneThinksOurGuestsThieves);
		}
		else
		{
			fdScriptSetMessageId(MsgVaultTheyHidThoseStolenGoods);
		}
		fdScriptShowMessage(ACTOR_INNKEEPER, 0);
	}
	fdScriptEnd();
}

#include "shared/vault_bone.c"

// Opening the menu while the party is cornering the suspects in the guests' room at Vault's inn
// only has Garet tell Isaac they don't have time for that.
static void scLogRoomsAGaretHasNoTimeForMenu(void)
{
#if VAULT_CHASE_MENU_STOPS_LEADER
	fdLeaderStopMoving();
#endif
	fdScriptSetMessageId(MsgVaultWeDontHaveTimeFor);
	// "We don't have time for that, Isaac!"
	fdScriptShowMessage(ACTOR_GARET, 0);
}

// Searching the barrel beside the girl at the inn before the thieves are caught, Isaac doesn't find
// anything. Later the Bones are there.
static void scLogRoomsASearchEmptyBarrel(void)
{
	fdScriptStart();
	mnFieldMessageShow(MsgVaultIsaacCheckedBarrel, 1);
	mnFieldMessageShow(MsgVaultButDidntFindAnything, 1);
	fdScriptEnd();
}

// Searching one of the three chests in the inn's loft, Isaac finds it empty.
static void scLogRoomsASearchEmptyChest(void)
{
	fdScriptStart();
	mnFieldMessageShow(MsgVaultIsaacCheckedChest, 1);
	mnFieldMessageShow(MsgVaultButChestWasEmpty, 1);
	fdScriptEnd();
}

// Returns the loft's scripts for the loft's three entrances, else the houses' scripts, which change
// once FLAG_VAULT_THIEVES_JAILED is set.
const CUE *scGetCues(void)
{
	switch (MainRoutineStateWork.entrance)
	{
		case LOG_ROOMS_A_LOFT_FROM_CRATE_ROOM:
		case LOG_ROOMS_A_LOFT_AFTER_LOSING_TO_THIEVES:
		case LOG_ROOMS_A_LOFT_AFTER_THIEVES_DEFEATED : return LogRoomsALoftCues;
	}
	if (gmFlagGet(FLAG_VAULT_THIEVES_JAILED) != 0)
	{
		return LogRoomsACuesAfterThieves;
	}
	return LogRoomsACues;
}

// Stands the speaker, faces the leader, and shows the selected message.
static void scLogRoomsAStandAndShowMessage(Sint32 speakerId)
{
	fdActorSetAnimation(speakerId, ANIM_STAND);
	scLogRoomsAFaceEachOtherAndWait(speakerId, ACTOR_PARTY_LEADER, 2);
	fdScriptShowMessage(speakerId, 0);
}

#include "shared/vault_talk.c"

// The speaker and the leader face each other, the speaker asks the selected question, and answers a
// yes with the next message and a no with the one after it.
static void scLogRoomsAAskYesNoAndAnswer(Sint32 speakerId)
{
	scLogRoomsAFaceEachOtherAndWait(speakerId, ACTOR_PARTY_LEADER, 2);
	fdScriptOpenMessage(speakerId, 0);
	if (fdScriptAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) != 0)
	{
		FieldStateWork->msg++;
	}
	fdScriptShowMessage(speakerId, 0);
}

// In the house in the middle of Vault, a small boy asks Isaac whether it is fun to see strange new
// places. A Yes has him dream of being an adventurer and sleeping in a tent; a No, wonder why
// bother then.
static void scLogRoomsABoyAsksAboutTravel(void)
{
	fdScriptStart();
	fdScriptSetMessageId(MsgVaultMisterFunSeeStrangeNew);
	fdActorSetAnimation(ACTOR_BOY_ASKING_ABOUT_TRAVEL, ANIM_STAND);
	scLogRoomsAAskYesNoAndAnswer(ACTOR_BOY_ASKING_ABOUT_TRAVEL);
	fdScriptEnd();
}

// Before Ivan has joined the party, a little girl at the mayor's house in Vault teases that Ivan
// has great powers and asks whether Isaac would like to know them. A Yes gets no way, it's their
// secret; a No, a warning not to say what he doesn't mean, as Ivan can tell in an instant.
static void scLogRoomsAOfferIvansSecret(void)
{
	fdScriptStart();
	fdScriptSetMessageId(MsgVaultIvanHasGreatPowersWouldnt);
	scLogRoomsAAskYesNoAndAnswer(ACTOR_GIRL_TELLING_A_SECRET);
	fdActorFaceDirection(ACTOR_GIRL_TELLING_A_SECRET, FACING_WEST, 0);
	fdScriptEnd();
}

// Spoken to across the counter of Vault's item shop, the item merchant opens her shop. From
// anywhere else she asks whether the party arrived in town today, and finds them fishy on a No;
// once the thieves are jailed she says it will be a while before anyone can go to Kalay with the
// bridge out.
static void scLogRoomsATalkToItemMerchant(void)
{
	Sint32 leaderFacing = fdActorGet(ACTOR_PARTY_LEADER)->facing;

	fdScriptStart();
	if (leaderFacing > FACING_NORTHWEST && leaderFacing < FACING_NORTHEAST)
	{
		mnShopRun(VAULT_ITEM_STOCK, ACTOR_ITEM_MERCHANT);
	}
	else if (gmFlagGet(FLAG_VAULT_THIEVES_JAILED) == 0)
	{
		fdScriptSetMessageId(MsgVaultDidJustArriveInTown);
		scLogRoomsAAskYesNoAndAnswer(ACTOR_ITEM_MERCHANT);
	}
	else
	{
		fdScriptSetMessageId(MsgVaultWithBridgeOutWillQuite);
		// "With the bridge out, it will be quite a while 'til we can go..."
		fdScriptShowMessage(ACTOR_ITEM_MERCHANT, 0);
	}
	fdScriptEnd();
}

// In the gem owner's house, his wife wishes their rocks were gems, or, once the thieves are jailed,
// is thankful they were caught before they stole the gem.
static void scLogRoomsATalkAboutRocks(void)
{
	fdScriptStart();
	if (gmFlagGet(FLAG_VAULT_THIEVES_JAILED) == 0)
	{
		fdScriptSetMessageId(MsgVaultIfOnlyTheseRocksWere);
	}
	else
	{
		fdScriptSetMessageId(MsgVaultThankGoodnessThoseThievesWere);
	}
	scLogRoomsAStandAndShowMessage(ACTOR_GEM_OWNERS_WIFE);
	fdScriptEnd();
}

// In Vault's item shop, Grandpa wonders whether he is missing something, or, once the thieves are
// jailed, says they hid the stolen treasure in the hotel's loft and more in the cemetery.
static void scLogRoomsATalkAboutMissingThings(void)
{
	fdScriptStart();
	if (gmFlagGet(FLAG_VAULT_THIEVES_JAILED) != 0)
	{
		fdScriptSetMessageId(MsgVaultThievesHidStolenTreasureIn);
	}
	else
	{
		fdScriptSetMessageId(MsgVaultJustMeOrAmMissing);
	}
	scLogRoomsAStandAndShowMessage(ACTOR_GRANDPA);
	fdScriptEnd();
}

// In his house, Vault's mayor asks whether someone could please help Ivan. Once Ivan has joined the
// party, he thanks Isaac the first time, and after that asks him to look after Ivan.
static void scLogRoomsAMayorAsksHelpForIvan(void)
{
	Sint32 thankedForHelpingFlag = FLAG_VAULT_MAYOR_THANKED_PARTY;
	fdScriptStart();
	if (gmFlagGet(FLAG_VAULT_IVAN_JOINED_ROD_SEARCH) != 0)
	{
		if (gmFlagGet(thankedForHelpingFlag) == 0)
		{
			fdScriptSetMessageId(MsgVaultYoureGoingHelpIvan);
			scLogRoomsAStandAndShowMessage(ACTOR_MAYOR);
			fdScriptWait(10);
			scLogRoomsAPlayAnimationAndWait(ACTOR_MAYOR, ANIM_NOD, 20);
			gmFlagSet(thankedForHelpingFlag);
		}
		else
		{
			fdScriptSetMessageId(MsgVaultPleaseLookAfterIvan);
		}
	}
	else
	{
		fdScriptSetMessageId(MsgVaultCouldSomeonePleaseHelpIvan);
	}
	scLogRoomsAStandAndShowMessage(ACTOR_MAYOR);
	fdScriptEnd();
}

// Before the thieves are caught, the girl at Vault's inn says the three strangers have fooled the
// innkeeper and his wife.
static void scLogRoomsAWarnAboutThreeStrangers(void)
{
	fdScriptStart();
	fdScriptSetMessageId(MsgVaultThoseThreeStrangersSureHave);
	scLogRoomsAStandAndShowMessage(ACTOR_GIRL_AT_THE_INN);
	fdScriptEnd();
}

// Talking to, or reading the mind of, the suspect with long dark hair in the guests' room at
// Vault's inn makes him cry out to stop it and run round the beds to his next spot, on or back the
// way he was going by which side he is pushed from.
static void scLogRoomsASuspectedLeaderDodges(void)
{
	ACTOR *suspect;
	Uint32 facing;

	suspect = fdActorGet(ACTOR_SUSPECTED_LEADER);
	fdScriptStart();
	fdActorRunRepeatedMotion(ACTOR_SUSPECTED_LEADER, 2);
	fdScriptSetMessageId(MsgVaultOwStop);
	// "Ow! Stop it!"
	fdScriptShowMessage(ACTOR_SUSPECTED_LEADER, 0);
	fdActorSetSpeed(ACTOR_SUSPECTED_LEADER, 4);
	// Pushed from the north he runs on along his route, from the south back.
	facing = FACING_SIXTEENTH(suspect->facing);
	if (facing >= FACING_SOUTH_SOUTHWEST && facing <= FACING_NORTH_NORTHWEST)
	{
		if (suspect->timer <= LEADER_SPOT_EAST_BED_HEAD)
		{
			fdActorSetScript(ACTOR_SUSPECTED_LEADER, LogRoomsALeaderRunsOn[suspect->timer]);
			suspect->timer++;
		}
		else
		{
			fdActorSetScript(ACTOR_SUSPECTED_LEADER, LogRoomsALeaderRunsBack[suspect->timer]);
			suspect->timer--;
		}
	}
	else
	{
		if (suspect->timer > LEADER_SPOT_EAST_BED_HEAD)
		{
			fdActorSetScript(ACTOR_SUSPECTED_LEADER, LogRoomsALeaderRunsOn[suspect->timer]);
			suspect->timer++;
		}
		else
		{
			fdActorSetScript(ACTOR_SUSPECTED_LEADER, LogRoomsALeaderRunsBack[suspect->timer]);
			suspect->timer--;
		}
	}
	if (suspect->timer > LEADER_SPOT_WEST_BED_FOOT)
	{
		suspect->timer = LEADER_SPOT_WEST_BED_HEAD;
	}
	if (suspect->timer < LEADER_SPOT_WEST_BED_HEAD)
	{
		suspect->timer = LEADER_SPOT_WEST_BED_FOOT;
	}
	fdActorWaitForScriptEndById(ACTOR_SUSPECTED_LEADER);
	fdScriptEnd();
}

// Starts one of the suspected thief's runs (LogRoomsAThiefRuns) and moves his timer on to the spot
// it ends at.
static inline void
scLogRoomsAStartThiefRun(ACTOR *actor, Sint32 scriptRow, Sint32 routePair, Uint16 routeDelta)
{
	fdActorSetScript(ACTOR_SUSPECTED_THIEF, LogRoomsAThiefRuns[scriptRow * 4 + actor->timer]);
	actor->timer = actor->timer - routePair * 2 + routeDelta;
}

// Talking to, or reading the mind of, the suspect in purple in the guests' room at Vault's inn
// makes him cry out to be left alone and run to his next spot by the beds or in a corner, picked by
// the way he is pushed.
static void scLogRoomsASuspectedThiefDodges(void)
{
	ACTOR *actor;
	Uint32 facing;
	Sint32 routePair;

	actor     = fdActorGet(ACTOR_SUSPECTED_THIEF);
	facing    = FACING_SIXTEENTH(actor->facing);
	routePair = actor->timer >> 1;
	fdScriptStart();
	fdActorRunRepeatedMotion(ACTOR_SUSPECTED_THIEF, 2);
	fdScriptSetMessageId(MsgVaultNoLeaveAlone);
	// "No! Leave me alone!"
	fdScriptShowMessage(ACTOR_SUSPECTED_THIEF, 0);
	fdActorSetSpeed(ACTOR_SUSPECTED_THIEF, 3.5);
	switch (actor->timer)
	{
		case THIEF_SPOT_SOUTH_WALL:
			if (facing > FACING_SOUTHEAST && facing < FACING_NORTHWEST)
			{
				fdActorSetScript(ACTOR_SUSPECTED_THIEF, LogRoomsASuspectAlongToSouthEastCorner);
				actor->timer = THIEF_SPOT_SOUTH_EAST_CORNER;
			}
			else
			{
				fdActorSetScript(ACTOR_SUSPECTED_THIEF, LogRoomsASuspectAlongToSouthWestCorner);
				actor->timer = THIEF_SPOT_SOUTH_WEST_CORNER;
			}
			break;
		case THIEF_SPOT_WEST_OF_BEDS:
		case THIEF_SPOT_SOUTH_EAST_CORNER:
			if (facing > FACING_SOUTHEAST && facing < FACING_NORTHWEST)
			{
				scLogRoomsAStartThiefRun(actor, routePair, routePair, 1);
			}
			else
			{
				scLogRoomsAStartThiefRun(actor, routePair ^ 1, routePair, -1);
			}
			break;
		case THIEF_SPOT_EAST_OF_BEDS:
		case THIEF_SPOT_SOUTH_WEST_CORNER:
			if (facing > FACING_SOUTHWEST && facing < FACING_NORTHEAST)
			{
				scLogRoomsAStartThiefRun(actor, routePair, routePair, 1);
			}
			else
			{
				scLogRoomsAStartThiefRun(actor, routePair ^ 1, routePair, -1);
			}
			break;
	}
	actor->timer &= 3;
	fdActorWaitForScriptEndById(ACTOR_SUSPECTED_THIEF);
	fdScriptEnd();
}

// Once the thieves are jailed, the burly grey-haired man in the house in the middle of Vault asks
// whether they came from Lunpa. A Yes has him say Lunpa's master Dodonpa is an awful man and
// catching his thieves means trouble; a No, that the Lunpans were noble thieves until Dodonpa.
static void scLogRoomsAAskWhetherThievesCameFromLunpa(void)
{
	fdScriptStart();
	fdScriptSetMessageId(MsgVaultDidThoseThievesComeFrom);
	scLogRoomsAAskYesNoAndAnswer(ACTOR_VILLAGER_SUSPECTING_THE_NEIGHBOR);
	fdScriptEnd();
}

// Once the thieves are jailed, the small boy in the house in the middle of Vault says his father is
// worried about the prisoners from Lunpa, and asks the party to protect them if Dodonpa's men
// attack.
static void scLogRoomsABoyWorriesAboutFather(void)
{
	fdScriptStart();
	fdScriptSetMessageId(MsgVaultMyFatherWorriedAboutThose);
	scLogRoomsAStandAndShowMessage(ACTOR_BOY_ASKING_ABOUT_TRAVEL);
	fdScriptEnd();
}

// Once the thieves are jailed, a small boy at the mayor's house in Vault has heard the party
// defeated them. A Yes has him say you can't judge people by their looks; a No, that his father
// said it was them.
static void scLogRoomsAAskAboutDefeatingThieves(void)
{
	fdScriptStart();
	fdScriptSetMessageId(MsgVaultHeardDefeatedThoseThieves);
	scLogRoomsAAskYesNoAndAnswer(ACTOR_BOY_WISHING_TO_BE_OLDER);
	fdScriptEnd();
}

// Once the thieves are jailed, Vault's mayor thanks Isaac in his house and gives the promised Water
// of Life, or asks him to come back with fewer things if his bags are full. Then he asks whether
// the party is heading beyond Goma Range: a Yes gets directions to the cave in its mountains
// north-east of Vault, past which lies Bilibin; a No, that there is no other way out.
static void scLogRoomsAMayorGivesWaterOfLife(void)
{
	fdScriptStart();
	if (fdScriptGetFlag(FLAG_VAULT_WATER_OF_LIFE_RECEIVED) == 0)
	{
		fdScriptSetMessageId(MsgVaultYouWereSuchGreatHelp);
		scLogRoomsAShowMessageAndWait(ACTOR_MAYOR, 20);
		scLogRoomsAPlayAnimationAndWait(ACTOR_MAYOR, ANIM_NOD, 20);
		scLogRoomsAShowMessageAndWait(ACTOR_MAYOR, 30);
		fdActorFaceDirection(ACTOR_MAYOR, FACING_EAST, 0);
		fdScriptWait(30);
		fdActorRunRepeatedMotion(ACTOR_MAYOR, 2);
		fdScriptWait(30);
		scLogRoomsAFaceEachOtherAndWait(ACTOR_PARTY_LEADER, ACTOR_MAYOR, 20);
		scLogRoomsAPlayAnimationAndWait(ACTOR_MAYOR, ANIM_NOD, 20);
		FieldStateWork->msg++;
		if (gmPartyInventoryHasSpace() == 0)
		{
			// "Oh no, Isaac and his friends have too many items!"
			fdScriptSetMessageId(MsgVaultYouWereSuchGreatHelp + 3);
			scLogRoomsAShowMessageAndWait(ACTOR_MAYOR, 20);
			fdScriptEnd();
			return;
		}
		gmFlagSet(FLAG_VAULT_WATER_OF_LIFE_RECEIVED);
		gmPartyGiveItem(ITEM_WATER_OF_LIFE, 0);
	}
	fdScriptSetMessageId(MsgVaultHeadingOutBeyondGomaRange);
	// "Are you heading out beyond Goma Range, by any chance?"
	fdScriptOpenMessage(ACTOR_MAYOR, 0);
	fdScriptWait(20);
	if (fdScriptAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) != 0)
	{
		FieldStateWork->msg++;
	}
	fdScriptShowMessage(ACTOR_MAYOR, 0);
	fdScriptEnd();
}

// Once the thieves are jailed, the girl at Vault's inn offers Isaac leftover bones from dinner, or
// asks whether he wants more. Accepting gives a Bone, unless his bags are full and she calls him a
// greedy lad.
static void scLogRoomsAOfferBones(void)
{
	fdScriptStart();
	scLogRoomsAFaceEachOtherAndWait(ACTOR_GIRL_AT_THE_INN, ACTOR_PARTY_LEADER, 2);
	if (gmFlagGet(FLAG_VAULT_BONE_RECEIVED) == 0)
	{
		fdScriptSetMessageId(MsgVaultHaveLotLeftoverBonesFrom);
		// "I have a lot of leftover bones from dinner. Here, do you..."
		fdScriptOpenMessage(ACTOR_GIRL_AT_THE_INN, 0);
	}
	else
	{
		fdScriptSetMessageId(MsgVaultWantMoreBones);
		// "What? You want more bones?"
		fdScriptOpenMessage(ACTOR_GIRL_AT_THE_INN, 0);
	}
	if (fdScriptAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0)
	{
		fdScriptWait(20);
		fdScriptShowMessage(ACTOR_GIRL_AT_THE_INN, 0);
		fdScriptWait(20);
		fdActorRunRepeatedMotion(ACTOR_GIRL_AT_THE_INN, 2);
		fdScriptWait(20);
		if (gmPartyInventoryHasSpace() == 0)
		{
			fdActorSetAnimationAndWait(ACTOR_GIRL_AT_THE_INN, ANIM_SHAKE_HEAD);
			fdScriptWait(20);
			fdScriptSetMessageId(MsgVaultWowHaveManyThingsArent);
			// "Wow, you have so many things. Aren't you a greedy lad!"
			fdScriptShowMessage(ACTOR_GIRL_AT_THE_INN, 0);
		}
		else
		{
			gmItemShowFound(ITEM_BONE, 3);
			gmPartyGiveItem(ITEM_BONE, 0);
			gmFlagSet(FLAG_VAULT_BONE_RECEIVED);
		}
	}
	else
	{
		FieldStateWork->msg++;
		fdScriptWait(20);
		fdActorSetAnimationAndWait(ACTOR_GIRL_AT_THE_INN, ANIM_NOD);
		fdScriptWait(20);
		fdScriptShowMessage(ACTOR_GIRL_AT_THE_INN, 0);
	}
	fdActorFaceDirection(ACTOR_GIRL_AT_THE_INN, FACING_SOUTH, 0);
	fdScriptEnd();
}

// Mind Read on the gem owner's wife in their house: she wants a look at that rock, or, once the
// thieves are jailed, wonders whether it is worthless and that is why they didn't steal it.
static void scLogRoomsAReadGemOwnersWifesMind(void)
{
	fdScriptStart();
	if (gmFlagGet(FLAG_VAULT_THIEVES_JAILED) == 0)
	{
		fdScriptSetMessageId(MsgVaultOkMisterLetMeSee);
	}
	else
	{
		fdScriptSetMessageId(MsgVaultIfRockWorthlessMaybeThats);
	}
	scVaultShowThought(ACTOR_GEM_OWNERS_WIFE);
	fdScriptEnd();
}

// Mind Read on the small boy in the house in the middle of Vault: he wonders what the outside world
// is like, or, once the thieves are jailed, thinks his father looks sad with worry.
static void scLogRoomsAReadBoysMind(void)
{
	fdScriptStart();
	if (gmFlagGet(FLAG_VAULT_THIEVES_JAILED) == 0)
	{
		fdScriptSetMessageId(MsgVaultWonderOutsideWorldLike);
	}
	else
	{
		fdScriptSetMessageId(MsgVaultFatherLooksSadWorryingLike);
	}
	scVaultShowThought(ACTOR_BOY_ASKING_ABOUT_TRAVEL);
	fdScriptEnd();
}

// Mind Read on Grandpa in Vault's item shop: he wonders whether the party could be the thieves and
// doesn't know who to trust anymore. Once the thieves are jailed, he thinks nothing in their house
// was valuable enough to steal.
static void scLogRoomsAReadGrandpasMind(void)
{
	fdScriptStart();
	if (gmFlagGet(FLAG_VAULT_THIEVES_JAILED) == 0)
	{
		fdScriptSetMessageId(MsgVaultCouldTheyThievesGoodDont);
	}
	else
	{
		fdScriptSetMessageId(MsgVaultGuessNothingInOurHouse);
	}
	scVaultShowThought(ACTOR_GRANDPA);
	fdScriptEnd();
}

// Mind Read on Vault's mayor in his house: he laughs that it tickles, being tickled by a boy, or,
// once the thieves are jailed, thinks the cave in the Goma Range is dangerous, but the party will
// be okay.
static void scLogRoomsAReadMayorMind(void)
{
	fdScriptStart();
	if (gmFlagGet(FLAG_VAULT_THIEVES_JAILED) == 0)
	{
		fdScriptSetMessageId(MsgVaultTicklesBeingTickledByBoy);
	}
	else
	{
		fdScriptSetMessageId(MsgVaultCaveInGomaRangeDangerous);
	}
	scVaultShowThought(ACTOR_MAYOR);
	fdScriptEnd();
}

// Mind Read on the girl at Vault's inn: before the thieves are caught she thinks the master and his
// wife are blinded by money. After, she is tempted to taste the cooking, or, once Isaac has a Bone,
// wonders whether he has a calcium deficiency.
static void scLogRoomsAReadInnGirlsMind(void)
{
	fdScriptStart();
	if (gmFlagGet(FLAG_VAULT_THIEVES_JAILED) == 0)
	{
		fdScriptSetMessageId(MsgVaultMasterHisWifeBlindedBy);
	}
	else if (gmFlagGet(FLAG_VAULT_BONE_RECEIVED) == 0)
	{
		fdScriptSetMessageId(MsgVaultGeeAlwaysGetHungryWhen);
	}
	else
	{
		fdScriptSetMessageId(MsgVaultHeReallyLikesBonesWonder);
	}
	scVaultShowThought(ACTOR_GIRL_AT_THE_INN);
	fdScriptEnd();
}

// Talking to Ivan at the mayor's house in Vault: the first time, he reads Isaac's mind, admits his
// strange powers, learns they are called Psynergy, and asks for help getting Master Hammet's rod
// back. Later he only asks again. A Yes has him join the party.
static void scLogRoomsAMeetIvan(void)
{
	ACTOR *actor;

	fdScriptStart();
	fdActorSetSpeed(ACTOR_PARTY_LEADER, 0.8);
	fdActorSetSpeed(ACTOR_GARET, 0.8);
	fdActorSetSpeed(ACTOR_IVAN, 0.8);
	// Isaac steps up in front of Ivan and Garet comes out beside him.
	sySoundPlay(MUS_SILENCE);
	fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 384, 408);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
	fdActorSetPos(ACTOR_GARET, 384, 408);
	fdActorWalkToAndWait(ACTOR_GARET, 368, 408);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTH_NORTHEAST, 20);
	// Introduce Ivan's ability only on the first meeting.
	if (fdScriptGetFlag(FLAG_VAULT_MET_IVAN) == 0)
	{
		gmFlagSet(FLAG_VAULT_MET_IVAN);
		// Ivan reads Isaac's mind and admits his strange powers.
		scStartMindRead(ACTOR_IVAN, ACTOR_PARTY_LEADER);
		fdScriptWait(40);
		scEndMindRead();
		fdScriptSetMessageId(MsgVaultDoPossessStrangePowers);
		sySoundPlay(MUS_WIND_ADEPTS);
		fdScriptWait(30);
		scLogRoomsAPlayAnimationAndWait(ACTOR_IVAN, ANIM_NOD, 30);
		scLogRoomsAShowMessageAndWait(ACTOR_IVAN, 30);
		// The two start; he reads again: "You have them, too, don't you?"
		fdActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
		fdActorRunRepeatedMotion(ACTOR_GARET, 1);
		fdScriptWait(20);
		scStartMindRead(ACTOR_IVAN, ACTOR_PARTY_LEADER);
		fdScriptWait(40);
		scEndMindRead();
		scLogRoomsAShowMessageAndWait(ACTOR_IVAN, 30);
		scLogRoomsAFaceEachOtherAndWait(ACTOR_PARTY_LEADER, ACTOR_GARET, 50);
		fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
		fdActorFaceDirection(ACTOR_GARET, FACING_NORTH_NORTHEAST, 0);
		fdScriptWait(20);
		// A third reading: his power is called Psynergy; then they have so much in common he can
		// trust them.
		scStartMindRead(ACTOR_IVAN, ACTOR_PARTY_LEADER);
		fdScriptWait(40);
		scEndMindRead();
		fdActorRunRepeatedMotion(ACTOR_IVAN, 1);
		scLogRoomsAShowMessageAndWait(ACTOR_IVAN, 50);
		fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
		scLogRoomsAPlayAnimationAndWait(ACTOR_GARET, ANIM_NOD, 20);
		scLogRoomsAPlayAnimationAndWait(ACTOR_IVAN, ANIM_NOD, 20);
		scLogRoomsAShowMessageAndWait(ACTOR_IVAN, 40);
		// He walks off a step, turns back and tells of Hammet's stolen rod.
		fdActorRunRepeatedMotion(ACTOR_IVAN, 1);
		fdScriptWait(30);
		fdActorFaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
		fdScriptWait(30);
		fdActorWalkToAndWait(ACTOR_IVAN, 376, 376);
		fdScriptWait(40);
		scLogRoomsAFaceEachOtherAndWait(ACTOR_PARTY_LEADER, ACTOR_GARET, 50);
		fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
		fdActorFaceDirection(ACTOR_GARET, FACING_NORTH_NORTHEAST, 0);
		fdActorRunRepeatedMotion(ACTOR_IVAN, 1);
		fdScriptWait(50);
		scLogRoomsAPlayAnimationAndWait(ACTOR_IVAN, ANIM_NOD, 30);
		fdActorFaceDirection(ACTOR_IVAN, FACING_SOUTH, 0);
		fdScriptWait(10);
		fdScriptShowMessage(ACTOR_IVAN, 0);
		// "Would you please help me get my master's rod back?"
		fdScriptOpenMessage(ACTOR_IVAN, 0);
	}
	else
	{
		sySoundPlay(MUS_WIND_ADEPTS);
		fdScriptSetMessageId(MsgVaultWouldReallyWouldHelpMe);
		// "Would you really? Would you help me find the rod?"
		fdScriptOpenMessage(ACTOR_IVAN, 0);
	}
	// On a Yes Ivan joins and walks into Isaac, as a party member does; a No gets his apology for
	// asking.
	if (fdScriptAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0)
	{
		scLogRoomsAIvanJoinsRodSearch();
		gmFlagSet(FLAG_VAULT_IVAN_JOINED_ROD_SEARCH);
		fdActorSetAnimation(ACTOR_IVAN, ANIM_WALK);
		actor = fdActorGet(ACTOR_PARTY_LEADER);
		if (actor != NULL)
		{
			fdActorSetDestination(ACTOR_IVAN, actor->x.part[0].integer, actor->z.part[0].integer);
		}
		fdActorWaitForMoveById(ACTOR_IVAN);
		fdActorSetPos(ACTOR_IVAN, 0, 0);
	}
	else
	{
		fdScriptShowMessage(ACTOR_IVAN, 0);
	}
	// Garet walks back into the leader and the earlier music returns.
	fdActorSetAnimation(ACTOR_GARET, ANIM_WALK);
	actor = fdActorGet(ACTOR_PARTY_LEADER);
	if (actor != NULL)
	{
		fdActorSetDestination(ACTOR_GARET, actor->x.part[0].integer, actor->z.part[0].integer);
	}
	fdActorWaitForMoveById(ACTOR_GARET);
	fdActorSetPos(ACTOR_GARET, 0, 0);
	gmPlayMusic();
	fdScriptEnd();
}

// Ivan jumps for joy, thanks Isaac and Garet, reads their names from Isaac's mind, shows them Mind
// Read on each other, and joins the party.
static void scLogRoomsAIvanJoinsRodSearch(void)
{
	// Ivan jumps for joy: "Thank you. With your help, I believe we can recover the rod." Then he
	// asks their names.
	fdScriptWait(20);
	fdActorGet(ACTOR_IVAN)->paused = 0;
	fdActorJump(ACTOR_IVAN, 4, 0);
	fdScriptWait(40);
	fdScriptSetMessageId(MsgVaultThankHelpBelieve);
	fdScriptWait(20);
	scLogRoomsAShowMessageAndWait(ACTOR_IVAN, 20);
	fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdScriptWait(50);
	scLogRoomsAShowMessageAndWait(ACTOR_IVAN, 30);
	// He reads their names from Isaac's mind; Isaac backs away from him.
	fdActorWalkToAndWait(ACTOR_IVAN, 376, 392);
	scStartMindRead(ACTOR_IVAN, ACTOR_PARTY_LEADER);
	fdScriptWait(40);
	scEndMindRead();
	fdActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 1);
	fdActorGet(ACTOR_PARTY_LEADER)->steeringFlags &= ~ACTOR_FACES_MOTION;
	fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 384, 424);
	fdScriptWait(1);
	fdActorGet(ACTOR_PARTY_LEADER)->steeringFlags |= ACTOR_FACES_MOTION;
	fdScriptWait(30);
	fdScriptShowMessage(ACTOR_IVAN, 0);
	// Demonstrate Mind Read and let Garet and Isaac react to Ivan's explanation.
	fdActorSetAttachedEffect(ACTOR_PARTY_LEADER, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdActorSetAttachedEffect(ACTOR_GARET, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdScriptWait(60);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
	scLogRoomsAPlayAnimationAndWait(ACTOR_GARET, ANIM_NOD, 30);
	fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdScriptWait(60);
	fdActorRunRepeatedMotion(ACTOR_IVAN, 1);
	fdScriptWait(10);
	scLogRoomsAPlayAnimationAndWait(ACTOR_IVAN, ANIM_NOD, 20);
	fdScriptShowMessage(ACTOR_IVAN, 0);
	fdActorRunRepeatedMotion(ACTOR_GARET, 2);
	fdScriptWait(10);
	scLogRoomsAFaceActorAndWait(ACTOR_GARET, ACTOR_PARTY_LEADER, 30);
	fdActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
	fdScriptWait(10);
	scLogRoomsAFaceActorAndWait(ACTOR_PARTY_LEADER, ACTOR_GARET, 40);
	fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
	fdScriptWait(60);
	// "Is my Mind Read so frightening?": either answer goes on the same way.
	fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
	fdScriptWait(60);
	fdScriptOpenMessage(ACTOR_IVAN, 0);
	fdActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_IVAN, 0);
	fdActorFaceActor(ACTOR_GARET, ACTOR_IVAN, 0);
	fdScriptAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0);
	fdScriptWait(30);
	// Ivan and Isaac clasp hands, and Garet cries out that his mind is read.
	scLogRoomsAPlayAnimationAndWait(ACTOR_IVAN, ANIM_NOD, 10);
	fdActorWalkToAndWait(ACTOR_IVAN, 384, 408);
	fdScriptWait(10);
	mnFieldMessageShow(MsgVaultThankHelpBelieve + IVAN_JOINING_ISAAC_CLASPS_HANDS, 1);
	fdScriptSetMessageId(MsgVaultThankHelpBelieve + IVAN_JOINING_GARET_FEELS_MIND_READ);
	fdActorFaceEachOther(ACTOR_IVAN, ACTOR_GARET, 0);
	scLogRoomsAFaceActorAndWait(ACTOR_PARTY_LEADER, ACTOR_GARET, 20);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
	fdScriptWait(60);
	fdActorRunRepeatedMotion(ACTOR_GARET, 1);
	fdActorGet(ACTOR_GARET)->steeringFlags &= ~ACTOR_FACES_MOTION;
	fdActorWalkToAndWait(ACTOR_GARET, 352, 408);
	fdScriptWait(1);
	fdActorGet(ACTOR_GARET)->steeringFlags |= ACTOR_FACES_MOTION;
	fdScriptWait(10);
	// Garet backs off; Ivan asks whether Isaac could read Garet's mind, and the two read it
	// together. "Just as I thought."
	fdActorWalkTo(ACTOR_IVAN, 368, 408);
	fdScriptWait(20);
	fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 368, 424);
	fdActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_GARET, 0);
	fdActorWaitForMoveById(ACTOR_IVAN);
	scStartMindRead(ACTOR_IVAN, ACTOR_GARET);
	scLogRoomsAPlayAnimationAndWait(ACTOR_GARET, ANIM_SHAKE_HEAD, 10);
	fdScriptShowMessage(ACTOR_GARET, 0);
	scEndMindRead();
	scLogRoomsAFaceEachOtherAndWait(ACTOR_IVAN, ACTOR_PARTY_LEADER, 30);
	fdScriptShowMessage(ACTOR_IVAN, 0);
	fdActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 1);
	fdScriptWait(30);
	scLogRoomsAPlayAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD, 30);
	scLogRoomsAPlayAnimationAndWait(ACTOR_IVAN, ANIM_NOD, 10);
	fdScriptShowMessage(ACTOR_IVAN, 0);
	fdActorRunRepeatedMotion(ACTOR_GARET, 2);
	fdActorFaceActor(ACTOR_GARET, ACTOR_PARTY_LEADER, 0);
	fdScriptShowMessage(ACTOR_GARET, 0);
	fdActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_GARET, 0);
	scLogRoomsAPlayAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD, 40);
	fdActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_IVAN, 0);
	scLogRoomsAFaceActorAndWait(ACTOR_GARET, ACTOR_IVAN, 30);
	fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdScriptWait(60);
	// Sharing their Psynergy let Isaac read minds too; Garet protests that it isn't fair, and Ivan
	// agrees it is one-way.
	scLogRoomsAPlayAnimationAndWait(ACTOR_IVAN, ANIM_NOD, 20);
	fdScriptShowMessage(ACTOR_IVAN, 0);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
	fdScriptWait(60);
	scLogRoomsAFaceActorAndWait(ACTOR_GARET, ACTOR_PARTY_LEADER, 10);
	fdScriptShowMessage(ACTOR_GARET, 0);
	fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdScriptWait(60);
	fdScriptShowMessage(ACTOR_IVAN, 0);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
	fdScriptWait(60);
	// Ivan and Garet clasp hands, and Garet reads Isaac's mind.
	mnFieldMessageShow(MsgVaultThankHelpBelieve + IVAN_JOINING_GARET_CLASPS_HANDS, 1);
	fdScriptSetMessageId(MsgVaultThankHelpBelieve + IVAN_JOINING_GARET_READS_MIND);
	fdActorFaceEachOther(ACTOR_IVAN, ACTOR_PARTY_LEADER, 0);
	fdActorFaceActor(ACTOR_GARET, ACTOR_PARTY_LEADER, 0);
	fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
	fdScriptWait(60);
	scStartMindRead(ACTOR_IVAN, ACTOR_PARTY_LEADER);
	fdPsynergyRestoreMindReadPalette();
	scLogRoomsAShowMessageAndWait(ACTOR_GARET, 30);
	scEndMindRead();
	fdActorFaceEachOther(ACTOR_IVAN, ACTOR_GARET, 0);
	scLogRoomsAPlayAnimationAndWait(ACTOR_IVAN, ANIM_NOD, 10);
	scLogRoomsAShowMessageAndWait(ACTOR_IVAN, 20);
	fdActorFaceEachOther(ACTOR_IVAN, ACTOR_PARTY_LEADER, 0);
	scLogRoomsAPlayAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD, 20);
	fdActorRunRepeatedMotion(ACTOR_GARET, 1);
	fdScriptWait(20);
	// "Let's use Mind Read to find the thieves!" Garet worries people will notice.
	scLogRoomsAPlayAnimationAndWait(ACTOR_GARET, ANIM_SHAKE_HEAD, 10);
	scLogRoomsAShowMessageAndWait(ACTOR_GARET, 30);
	fdActorFaceActor(ACTOR_IVAN, ACTOR_GARET, 0);
	fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdScriptWait(30);
	scLogRoomsAFaceEachOtherAndWait(ACTOR_PARTY_LEADER, ACTOR_GARET, 10);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
	scLogRoomsAPlayAnimationAndWait(ACTOR_GARET, ANIM_NOD, 30);
	fdActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_IVAN, 0);
	fdActorFaceActor(ACTOR_GARET, ACTOR_IVAN, 0);
	fdActorFaceActor(ACTOR_IVAN, ACTOR_PARTY_LEADER, 0);
	// Only Adepts can tell when Ivan uses Psynergy; ordinary people see nothing, and he promises
	// not to read their minds again.
	fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 0);
	fdScriptWait(60);
	scLogRoomsAShowMessageAndWait(ACTOR_IVAN, 10);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
	scLogRoomsAPlayAnimationAndWait(ACTOR_GARET, ANIM_NOD, 20);
	scLogRoomsAPlayAnimationAndWait(ACTOR_IVAN, ANIM_SHAKE_HEAD, 20);
	scLogRoomsAShowMessageAndWait(ACTOR_IVAN, 20);
	fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdScriptWait(60);
	scLogRoomsAPlayAnimationAndWait(ACTOR_IVAN, ANIM_NOD, 10);
	scLogRoomsAShowMessageAndWait(ACTOR_IVAN, 30);
	fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
	fdScriptWait(60);
	scLogRoomsAPlayAnimationAndWait(ACTOR_IVAN, ANIM_SHAKE_HEAD, 10);
	scLogRoomsAShowMessageAndWait(ACTOR_IVAN, 30);
	scLogRoomsAPlayAnimationAndWait(ACTOR_IVAN, ANIM_NOD, 10);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
	scLogRoomsAPlayAnimationAndWait(ACTOR_GARET, ANIM_NOD, 20);
	// "Ivan joined your party!", then he will read the townspeople's minds.
	fdScriptAddPartyMember(IVAN, 1);
	fdScriptSetMessageId(MsgVaultThankHelpBelieve + IVAN_JOINING_IVAN_WILL_READ_TOWN);
	fdActorRunRepeatedMotion(ACTOR_IVAN, 1);
	// "Let me read the minds of the townspeople to find the rod."
	fdScriptShowMessage(ACTOR_IVAN, 0);
}

// In the inn's loft, Isaac cuts free the young man tied up on the floor. He tells how he was
// whacked from behind while fixing the roof, and notices the chests; then the three thieves come
// in, tell of Lunpa and Dodonpa, ask to be let go, and attack.
static void scLogRoomsAFreeVillagerAndFightThieves(void)
{
	Sint32 base;

	fdScriptStart();
	fdActorSetSpeed(ACTOR_PARTY_LEADER, 0.8);
	fdActorSetSpeed(ACTOR_GARET, 0.8);
	fdActorSetSpeed(ACTOR_IVAN, 0.8);
	// Isaac cuts the tied-up villager free from the ropes; he gets up and nods.
	fdActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 3);
	fdScriptWait(20);
	base = MsgVaultVaultCutFree;
	mnFieldMessageShow(base, 1);
	base++;
	fdScriptSetMessageId(base);
	fdScriptWait(30);
	fdActorSetAnimation(ACTOR_TIED_UP_VILLAGER, ANIM_STAND);
	fdScriptWait(60);
	scLogRoomsAPlayAnimationAndWait(ACTOR_TIED_UP_VILLAGER, ANIM_NOD, 40);
	// Garet and Ivan come out of the leader around the tied-up villager.
	fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 792, 440);
	scLogRoomsAFaceActorAndWait(ACTOR_PARTY_LEADER, ACTOR_TIED_UP_VILLAGER, 20);
	fdActorSetPos(ACTOR_GARET, 792, 440);
	fdActorSetPos(ACTOR_IVAN, 792, 440);
	fdActorWalkTo(ACTOR_GARET, 808, 432);
	fdActorWalkTo(ACTOR_IVAN, 792, 456);
	fdActorWaitForMoveById(ACTOR_GARET);
	fdActorFaceActor(ACTOR_GARET, ACTOR_TIED_UP_VILLAGER, 0);
	fdActorWaitForMoveById(ACTOR_IVAN);
	scLogRoomsAFaceActorAndWait(ACTOR_IVAN, ACTOR_TIED_UP_VILLAGER, 60);
	// The tied-up villager tells how he came up to fix the roof, was whacked from behind and woke
	// up tied.
	fdActorShowEmote(ACTOR_TIED_UP_VILLAGER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
	fdScriptWait(60);
	scLogRoomsAShowMessageAndWait(ACTOR_TIED_UP_VILLAGER, 20);
	fdActorSetAnimation(ACTOR_IVAN, ANIM_NOD);
	scLogRoomsAPlayAnimationAndWait(ACTOR_GARET, ANIM_NOD, 30);
	fdActorRunRepeatedMotion(ACTOR_TIED_UP_VILLAGER, 2);
	fdScriptWait(20);
	scLogRoomsAShowMessageAndWait(ACTOR_TIED_UP_VILLAGER, 20);
	fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
	fdScriptWait(60);
	fdActorFaceDirection(ACTOR_TIED_UP_VILLAGER, FACING_SOUTH_SOUTHEAST, 0);
	fdScriptWait(10);
	scLogRoomsAShowMessageAndWait(ACTOR_TIED_UP_VILLAGER, 30);
	// He looks around at chests he doesn't remember; Ivan guesses the stolen goods are in them.
	fdActorFaceDirection(ACTOR_TIED_UP_VILLAGER, FACING_NORTH_NORTHEAST, 0);
	fdScriptWait(30);
	fdActorShowEmote(ACTOR_TIED_UP_VILLAGER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdScriptWait(60);
	fdActorFaceDirection(ACTOR_TIED_UP_VILLAGER, FACING_NORTH_NORTHWEST, 0);
	fdScriptWait(40);
	fdActorFaceDirection(ACTOR_TIED_UP_VILLAGER, FACING_NORTH_NORTHEAST, 0);
	fdScriptWait(40);
	scLogRoomsAFaceActorAndWait(ACTOR_TIED_UP_VILLAGER, ACTOR_PARTY_LEADER, 20);
	scLogRoomsAPlayAnimationAndWait(ACTOR_TIED_UP_VILLAGER, ANIM_SHAKE_HEAD, 30);
	scLogRoomsAShowMessageAndWait(ACTOR_TIED_UP_VILLAGER, 20);
	fdActorRunRepeatedMotion(ACTOR_IVAN, 1);
	scLogRoomsAShowMessageAndWait(ACTOR_IVAN, 40);
	// The ominous music starts as the thieves' leader appears at the ladder and says "Looks like
	// we've been found out!"; everyone turns to him.
	fdActorSetPos(ACTOR_THIEF_LEADER, 744, 408);
	sySoundPlay(MUS_CHANCE_MEETING_WITH_SWORN_ENEMIES);
	scLogRoomsAShowMessageAndWait(ACTOR_THIEF_LEADER, 20);
	fdActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
	fdActorStartRepeatedMotion(ACTOR_GARET, 1);
	fdActorStartRepeatedMotion(ACTOR_IVAN, 1);
	fdActorRunRepeatedMotion(ACTOR_TIED_UP_VILLAGER, 1);
	fdScriptWait(30);
	fdActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_THIEF_LEADER, 0);
	fdActorFaceActor(ACTOR_GARET, ACTOR_THIEF_LEADER, 0);
	fdActorFaceActor(ACTOR_IVAN, ACTOR_THIEF_LEADER, 0);
	scLogRoomsAFaceActorAndWait(ACTOR_TIED_UP_VILLAGER, ACTOR_THIEF_LEADER, 40);
	// He walks up and his two men run in behind him.
	fdActorSetSpeed(ACTOR_THIEF_LEADER, 0.8);
	fdActorSetSpeed(ACTOR_FIRST_THIEF, 1.5);
	fdActorSetSpeed(ACTOR_SECOND_THIEF, 1.5);
	fdActorSetPos(ACTOR_FIRST_THIEF, 744, 408);
	fdActorSetPos(ACTOR_SECOND_THIEF, 744, 408);
	fdActorWalkToAndWait(ACTOR_THIEF_LEADER, 792, 416);
	fdActorFaceDirection(ACTOR_THIEF_LEADER, FACING_SOUTH_SOUTHEAST, 0);
	fdScriptWait(70);
	fdActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_THIEF_LEADER, 0);
	fdActorFaceActor(ACTOR_GARET, ACTOR_THIEF_LEADER, 0);
	fdActorFaceActor(ACTOR_IVAN, ACTOR_THIEF_LEADER, 0);
	fdActorFaceActor(ACTOR_TIED_UP_VILLAGER, ACTOR_THIEF_LEADER, 0);
	fdActorSetScript(ACTOR_FIRST_THIEF, LogRoomsAFirstThiefEntryScript);
	fdScriptWait(40);
	fdActorSetScript(ACTOR_SECOND_THIEF, LogRoomsASecondThiefEntryScript);
	fdActorWaitForScriptEndById(ACTOR_SECOND_THIEF);
	fdActorFaceDirection(ACTOR_FIRST_THIEF, FACING_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_SECOND_THIEF, FACING_SOUTHEAST, 0);
	fdScriptWait(40);
	// The three taunt Hammet's whelp and say Hammet was caught by worse thieves after fleeing to
	// Lunpa.
	scLogRoomsAPlayAnimationAndWait(ACTOR_THIEF_LEADER, ANIM_SHAKE_HEAD, 20);
	scLogRoomsAShowMessageAndWait(ACTOR_THIEF_LEADER, 20);
	fdActorRunRepeatedMotion(ACTOR_FIRST_THIEF, 1);
	fdScriptWait(10);
	scLogRoomsAShowMessageAndWait(ACTOR_FIRST_THIEF, 20);
	fdActorRunRepeatedMotion(ACTOR_SECOND_THIEF, 1);
	fdScriptWait(10);
	scLogRoomsAShowMessageAndWait(ACTOR_SECOND_THIEF, 40);
	fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdScriptWait(60);
	scLogRoomsAPlayAnimationAndWait(ACTOR_THIEF_LEADER, ANIM_SHAKE_HEAD, 20);
	scLogRoomsAShowMessageAndWait(ACTOR_THIEF_LEADER, 20);
	fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdScriptWait(60);
	// The tied-up villager is alarmed at the name Lunpa; Garet asks what kind of place it is, and
	// the tied-up villager and the thieves tell of Lunpa, Donpa and Dodonpa.
	fdActorShowEmote(ACTOR_TIED_UP_VILLAGER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdScriptWait(40);
	fdActorRunRepeatedMotion(ACTOR_TIED_UP_VILLAGER, 2);
	fdScriptWait(20);
	scLogRoomsAShowMessageAndWait(ACTOR_TIED_UP_VILLAGER, 20);
	fdActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_TIED_UP_VILLAGER, 0);
	fdActorFaceActor(ACTOR_GARET, ACTOR_TIED_UP_VILLAGER, 0);
	scLogRoomsAFaceActorAndWait(ACTOR_IVAN, ACTOR_TIED_UP_VILLAGER, 20);
	scLogRoomsAPlayAnimationAndWait(ACTOR_TIED_UP_VILLAGER, ANIM_SHAKE_HEAD, 30);
	scLogRoomsAShowMessageAndWait(ACTOR_TIED_UP_VILLAGER, 20);
	fdActorRunRepeatedMotion(ACTOR_IVAN, 2);
	fdScriptWait(20);
	fdActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_IVAN, 0);
	scLogRoomsAFaceActorAndWait(ACTOR_GARET, ACTOR_IVAN, 30);
	scLogRoomsAShowMessageAndWait(ACTOR_GARET, 20);
	fdActorShowEmote(ACTOR_TIED_UP_VILLAGER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
	fdScriptWait(60);
	scLogRoomsAShowMessageAndWait(ACTOR_TIED_UP_VILLAGER, 20);
	fdActorRunRepeatedMotion(ACTOR_THIEF_LEADER, 1);
	fdScriptWait(10);
	scLogRoomsAShowMessageAndWait(ACTOR_THIEF_LEADER, 20);
	fdActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_THIEF_LEADER, 0);
	fdActorFaceActor(ACTOR_GARET, ACTOR_THIEF_LEADER, 0);
	scLogRoomsAFaceActorAndWait(ACTOR_IVAN, ACTOR_THIEF_LEADER, 20);
	scLogRoomsAPlayAnimationAndWait(ACTOR_FIRST_THIEF, ANIM_NOD, 20);
	scLogRoomsAShowMessageAndWait(ACTOR_FIRST_THIEF, 20);
	scLogRoomsAPlayAnimationAndWait(ACTOR_SECOND_THIEF, ANIM_SHAKE_HEAD, 20);
	scLogRoomsAShowMessageAndWait(ACTOR_SECOND_THIEF, 20);
	fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
	fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
	fdScriptWait(60);
	// "See, we're not so bad. How about it? Will you let us go?" Yes: if they return the goods and
	// repent? No: even returning everything is a crime?
	scLogRoomsAPlayAnimationAndWait(ACTOR_THIEF_LEADER, ANIM_NOD, 20);
	fdScriptOpenMessage(ACTOR_THIEF_LEADER, 0);
	fdScriptWait(50);
	fdActorFaceActor(ACTOR_IVAN, ACTOR_PARTY_LEADER, 0);
	scLogRoomsAFaceActorAndWait(ACTOR_GARET, ACTOR_PARTY_LEADER, 30);
	if (fdScriptAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0)
	{
		fdScriptWait(40);
		fdActorFaceActor(ACTOR_GARET, ACTOR_THIEF_LEADER, 0);
		fdActorFaceActor(ACTOR_IVAN, ACTOR_THIEF_LEADER, 0);
		fdActorRunRepeatedMotion(ACTOR_THIEF_LEADER, 2);
		fdScriptWait(20);
		fdScriptShowMessage(ACTOR_THIEF_LEADER, 0);
	}
	else
	{
		fdScriptWait(40);
		fdActorFaceActor(ACTOR_GARET, ACTOR_THIEF_LEADER, 0);
		fdActorFaceActor(ACTOR_IVAN, ACTOR_THIEF_LEADER, 0);
		base = MsgVaultVaultThievesCaught;
		fdScriptSetMessageId(base);
		fdActorRunRepeatedMotion(ACTOR_THIEF_LEADER, 2);
		fdScriptWait(20);
		// "What? You mean, even if we return everything we stole and..."
		fdScriptShowMessage(ACTOR_THIEF_LEADER, 0);
		// Both answers go on with the same refusal.
		base -= 3;
		fdScriptSetMessageId(base);
	}
	// "We stole this stuff fair and square!" They agree to keep the party quiet, and the leader
	// cries "Get 'em!!!"
	fdActorShowEmote(ACTOR_FIRST_THIEF, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
	fdScriptWait(60);
	scLogRoomsAShowMessageAndWait(ACTOR_FIRST_THIEF, 20);
	scLogRoomsAPlayAnimationAndWait(ACTOR_SECOND_THIEF, ANIM_SHAKE_HEAD, 20);
	scLogRoomsAShowMessageAndWait(ACTOR_SECOND_THIEF, 30);
	scLogRoomsAFaceEachOtherAndWait(ACTOR_THIEF_LEADER, ACTOR_FIRST_THIEF, 30);
	fdActorSetAnimation(ACTOR_THIEF_LEADER, ANIM_NOD);
	scLogRoomsAPlayAnimationAndWait(ACTOR_FIRST_THIEF, ANIM_NOD, 30);
	scLogRoomsAFaceEachOtherAndWait(ACTOR_THIEF_LEADER, ACTOR_SECOND_THIEF, 30);
	fdActorSetAnimation(ACTOR_THIEF_LEADER, ANIM_NOD);
	scLogRoomsAPlayAnimationAndWait(ACTOR_SECOND_THIEF, ANIM_NOD, 40);
	fdActorFaceDirection(ACTOR_THIEF_LEADER, FACING_SOUTH_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_FIRST_THIEF, FACING_SOUTH_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_SECOND_THIEF, FACING_SOUTH_SOUTHEAST, 0);
	fdScriptWait(20);
	scLogRoomsAPlayAnimationAndWait(ACTOR_THIEF_LEADER, ANIM_SHAKE_HEAD, 20);
	fdScriptShowMessage(ACTOR_THIEF_LEADER, 0);
	gmFlagSet(FLAG_VAULT_THIEVES_ATTACKED);
	FieldStateWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_BARS);
	base                       = SCENE_LOG_ROOMS_A;
	fdScriptSetAfterBattleDestination(base, LOG_ROOMS_A_LOFT_AFTER_THIEVES_DEFEATED);
	fdScriptSetDefeatDestination(base, LOG_ROOMS_A_LOFT_AFTER_LOSING_TO_THIEVES);
	SET_BATTLE_TYPE(BATTLE_TYPE_SCRIPTED);
	fdRequestBattleFromEncounterTable(ENCOUNTER_ZONE_SCRIPTED_VALE_VAULT, 5);
	fdScriptEnd();
}

// Going down the stairs from the inn's upstairs in Vault takes the party to the floor below.
static void scLogRoomsALeaveGuestsRoom(void)
{
	sySoundPlay(SE_MAP_EXIT);
	syImportScriptSetExitRequest(LOG_ROOMS_A_INN_UPSTAIRS_TO_INN);
}

// Once the crate is pushed aside, walking east from the inn's attic through the way behind it
// (metatile (45, 25)) takes the party into the loft.
static void scLogRoomsAEnterLoft(void)
{
	fdActorSetSpeed(ACTOR_PARTY_LEADER, 0.5);
	fdActorWalkTo(ACTOR_PARTY_LEADER, 728, 408);
	FieldStateWork->transitionFrame = MAP_TRANSITION_TICKS;
	sySoundPlay(SE_MAP_EXIT);
	syImportScriptSetExitRequest(LOG_ROOMS_A_CRATE_ROOM_TO_LOFT);
}

// Walking west out of the inn's loft (metatile (46, 25)) back into the attic; before the thieves
// have attacked, the tied-up young man groans as the party goes.
static void scLogRoomsALeaveLoft(void)
{
	fdActorSetSpeed(ACTOR_PARTY_LEADER, 0.5);
	fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 744, 408);
	if (gmFlagGet(FLAG_VAULT_THIEVES_ATTACKED) == 0)
	{
		fdScriptStart();
		fdScriptSetMessageId(MsgVaultUhnnUhnn);
		// "Uhnn... Uhnn..."
		fdScriptShowMessage(ACTOR_TIED_UP_VILLAGER, 0);
		fdScriptEnd();
	}
	FieldStateWork->transitionFrame = MAP_TRANSITION_TICKS;
	sySoundPlay(SE_MAP_EXIT);
	syImportScriptSetExitRequest(LOG_ROOMS_A_LOFT_TO_CRATE_ROOM);
}

// In the guests' room upstairs at Vault's inn, once Ivan is with the party, the two suspects at the
// west end of the room start, see that weird kid is with them, and run off round the beds.
static void scLogRoomsAApproachSuspects(void)
{
	ACTOR *suspectedLeader;
	ACTOR *suspectedThief;

	suspectedLeader = fdActorGet(ACTOR_SUSPECTED_LEADER);
	suspectedThief  = fdActorGet(ACTOR_SUSPECTED_THIEF);
	fdScriptStart();
	fdActorSetSpeed(ACTOR_PARTY_LEADER, 0.8);
	fdActorSetSpeed(ACTOR_GARET, 0.8);
	fdActorSetSpeed(ACTOR_IVAN, 0.8);
	// Isaac walks in; the two start and spot Ivan with him: "It looks like that weird kid is with
	// them!"
	fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 232, 696);
	fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 200, 696);
	fdScriptWait(10);
	fdActorShowEmote(ACTOR_SUSPECTED_THIEF, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdActorShowEmote(ACTOR_SUSPECTED_LEADER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdScriptWait(60);
	scLogRoomsAFaceActorAndWait(ACTOR_SUSPECTED_THIEF, ACTOR_PARTY_LEADER, 10);
	fdActorRunRepeatedMotion(ACTOR_SUSPECTED_LEADER, 2);
	fdScriptWait(20);
	fdScriptSetMessageId(MsgVaultWant);
	scLogRoomsAShowMessageAndWait(ACTOR_SUSPECTED_LEADER, 20);
	fdActorSetAttachedEffect(ACTOR_SUSPECTED_THIEF,
	                         ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdScriptWait(60);
	scLogRoomsAShowMessageAndWait(ACTOR_SUSPECTED_THIEF, 20);
	fdActorRunRepeatedMotion(ACTOR_SUSPECTED_LEADER, 1);
	scLogRoomsAShowMessageAndWait(ACTOR_SUSPECTED_LEADER, 30);
	// "Don't let him near me!" They run off to their routes' first spots.
	fdActorSetSpeed(ACTOR_SUSPECTED_LEADER, 4);
	fdActorSetSpeed(ACTOR_SUSPECTED_THIEF, 3.5);
	fdActorSetScript(ACTOR_SUSPECTED_THIEF, LogRoomsASuspectToSouthWestCorner);
	fdActorSetScript(ACTOR_SUSPECTED_LEADER, LogRoomsASuspectRoundToWestBedHead);
	fdActorWaitForScriptEndById(ACTOR_SUSPECTED_LEADER);
	fdMapGridCopyMetatileAttributes(14, 45, 3, 1, 14, 44);
	gmFlagSet(FLAG_VAULT_SUSPECTS_APPROACHED);
	gmFlagSet(FLAG_SUSPECTS_FLEEING);
	sySetTickEvent(scLogRoomsAFaceSuspectsTowardLeader, EVENT_PASS_TICK);
	// The leader stands at the west bed's head but takes up his route as from the middle one's: his
	// first run on goes straight to the east bed's head.
	suspectedLeader->timer = LEADER_SPOT_MIDDLE_BED_HEAD;
	suspectedThief->timer  = THIEF_SPOT_SOUTH_WEST_CORNER;
	fdScriptEnd();
}

// Stepping back out of the guests' room at Vault's inn after the suspects have run, the party talks
// it over: Mind Read is no use while they keep running, so Garet plans for the three to surround
// one of them, with Isaac in the lead.
static void scLogRoomsADiscussSuspiciousGuests(void)
{
	ACTOR *suspectedLeader;
	ACTOR *suspectedThief;
	Sint32 base;

	suspectedLeader = fdActorGet(ACTOR_SUSPECTED_LEADER);
	suspectedThief  = fdActorGet(ACTOR_SUSPECTED_THIEF);
	fdScriptStart();
	syCloseEvent(scLogRoomsAFaceSuspectsTowardLeader);
	gmFlagClear(FLAG_SUSPECTS_FLEEING);
	if (suspectedLeader->timer <= 3)
	{
		fdActorSetScript(ACTOR_SUSPECTED_LEADER, LogRoomsASuspectRoundToWestBedFoot);
	}
	else
	{
		fdActorSetScript(ACTOR_SUSPECTED_LEADER, LogRoomsASuspectToWestBedFoot);
	}
	if (suspectedThief->timer <= 2)
	{
		fdActorSetScript(ACTOR_SUSPECTED_THIEF, LogRoomsASuspectAcrossToWestOfBeds);
	}
	else
	{
		fdActorSetScript(ACTOR_SUSPECTED_THIEF, LogRoomsASuspectToWestBedFoot);
	}
	fdActorSetSpeed(ACTOR_PARTY_LEADER, 0.8);
	fdActorSetSpeed(ACTOR_GARET, 0.8);
	fdActorSetSpeed(ACTOR_IVAN, 0.8);
	// Outside the room, Garet and Ivan step out of the leader.
	fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 248, 728);
	fdActorSetPos(ACTOR_IVAN, 248, 728);
	fdActorSetPos(ACTOR_GARET, 248, 728);
	fdActorWalkTo(ACTOR_IVAN, 264, 744);
	fdActorWalkToAndWait(ACTOR_GARET, 232, 744);
	fdActorWaitForMoveById(ACTOR_IVAN);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
	fdActorFaceActor(ACTOR_GARET, ACTOR_PARTY_LEADER, 0);
	scLogRoomsAFaceActorAndWait(ACTOR_IVAN, ACTOR_PARTY_LEADER, 30);
	// Ivan: they're acting suspicious.
	fdActorRunRepeatedMotion(ACTOR_IVAN, 1);
	base = MsgVaultTheyreActingSuspiciousSomethingsNot;
	fdScriptSetMessageId(base);
	// "They're acting suspicious. Something's not right..."
	fdScriptShowMessage(ACTOR_IVAN, 0);
	fdActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_IVAN, 0);
	scLogRoomsAFaceActorAndWait(ACTOR_GARET, ACTOR_IVAN, 20);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
	scLogRoomsAPlayAnimationAndWait(ACTOR_GARET, ANIM_NOD, 20);
	scLogRoomsAFaceEachOtherAndWait(ACTOR_PARTY_LEADER, ACTOR_GARET, 10);
	// Garet: "But you can't use Mind Read if they keep running away, can you?" Yes: Ivan asks how
	// he can catch them; no: there must be a way.
	fdScriptOpenMessage(ACTOR_GARET, 0);
	if (fdScriptAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) != 0)
	{
		FieldStateWork->msg++;
	}
	scLogRoomsAShowMessageAndWait(ACTOR_GARET, 30);
	// Garet's plan: the three of them surround one of the two.
	fdScriptSetMessageId(base + 4);
	fdActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_IVAN, 0);
	scLogRoomsAFaceActorAndWait(ACTOR_GARET, ACTOR_IVAN, 50);
	fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdScriptWait(60);
	scLogRoomsAFaceEachOtherAndWait(ACTOR_PARTY_LEADER, ACTOR_GARET, 50);
	fdActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_IVAN, 0);
	scLogRoomsAFaceActorAndWait(ACTOR_GARET, ACTOR_IVAN, 30);
	scLogRoomsAPlayAnimationAndWait(ACTOR_IVAN, ANIM_NOD, 10);
	scLogRoomsAShowMessageAndWait(ACTOR_IVAN, 20);
	fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
	fdScriptWait(60);
	scLogRoomsAPlayAnimationAndWait(ACTOR_IVAN, ANIM_NOD, 20);
	scLogRoomsAShowMessageAndWait(ACTOR_IVAN, 30);
	// "Isaac, you take the lead." The two walk back into him, and the suspects are put back at
	// their table.
	fdScriptSetMessageId(MsgVaultIsaacTakeLead);
	// "Isaac, you take the lead."
	fdScriptShowMessage(ACTOR_GARET, 0);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
	fdScriptWait(40);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
	fdActorSetAnimation(ACTOR_GARET, ANIM_NOD);
	scLogRoomsAPlayAnimationAndWait(ACTOR_IVAN, ANIM_NOD, 50);
	fdActorWalkTo(ACTOR_IVAN, 248, 728);
	fdActorWalkToAndWait(ACTOR_GARET, 248, 728);
	fdActorSetPos(ACTOR_GARET, 0, 0);
	fdActorSetPos(ACTOR_IVAN, 0, 0);
	fdActorSetPos(ACTOR_SUSPECTED_LEADER, 104, 696);
	fdActorSetPos(ACTOR_SUSPECTED_THIEF, 120, 696);
	fdActorFaceDirection(ACTOR_SUSPECTED_LEADER, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_SUSPECTED_THIEF, FACING_WEST, 0);
	fdMapGridCopyMetatileAttributes(14, 50, 3, 1, 14, 44);
	fdScriptEnd();
}

// Walking back into the guests' room at Vault's inn after the plan, the party sets out to corner
// the two suspects.
static void scLogRoomsAStartCorneringSuspects(void)
{
	gmFlagSet(FLAG_MENUS_RAISE_CUE);
	gmFlagSet(FLAG_VAULT_CORNERING_SUSPECTS);
	scLogRoomsASpreadOutToCornerSuspects();
}

// The suspects see the party back, cry that they're coming for them one-on-one, and run to their
// first spots again.
static void scLogRoomsAResetSuspectRoutes(void)
{
	fdScriptWait(30);
	fdActorRunRepeatedMotion(ACTOR_SUSPECTED_LEADER, 1);
	fdScriptWait(20);
	fdScriptSetMessageId(MsgVaultTheyreBack);
	scLogRoomsAShowMessageAndWait(ACTOR_SUSPECTED_LEADER, 20);
	fdActorFaceDirection(ACTOR_SUSPECTED_THIEF, FACING_EAST, 20);
	fdActorSetAttachedEffect(ACTOR_SUSPECTED_THIEF,
	                         ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdActorRunRepeatedMotion(ACTOR_SUSPECTED_THIEF, 2);
	scLogRoomsAShowMessageAndWait(ACTOR_SUSPECTED_THIEF, 20);
	fdActorSetAnimationAndWait(ACTOR_SUSPECTED_LEADER, ANIM_SHAKE_HEAD);
	fdScriptWait(20);
	scLogRoomsAShowMessageAndWait(ACTOR_SUSPECTED_LEADER, 20);
	fdActorSetSpeed(ACTOR_SUSPECTED_LEADER, 4);
	fdActorSetSpeed(ACTOR_SUSPECTED_THIEF, 3.5);
	fdActorSetScript(ACTOR_SUSPECTED_THIEF, LogRoomsASuspectToSouthWestCorner);
	fdActorSetScript(ACTOR_SUSPECTED_LEADER, LogRoomsASuspectRoundToWestBedHead);
	fdActorWaitForScriptEndById(ACTOR_SUSPECTED_LEADER);
	// As scLogRoomsAApproachSuspects leaves them.
	fdActorGet(ACTOR_SUSPECTED_LEADER)->timer = LEADER_SPOT_MIDDLE_BED_HEAD;
	fdActorGet(ACTOR_SUSPECTED_THIEF)->timer  = THIEF_SPOT_SOUTH_WEST_CORNER;
	fdMapGridCopyMetatileAttributes(14, 48, 4, 1, 14, 44);
}

// Draws Isaac, Garet and Ivan in front of the room's furniture for the talk after the Mind Read.
static inline void scLogRoomsASetPartyImagePriority(void)
{
	{
		SPR *sprite            = fdActorGet(ACTOR_PARTY_LEADER)->sprite;
		sprite->image.priority = 1;
	}
	{
		SPR *sprite            = fdActorGet(ACTOR_GARET)->sprite;
		sprite->image.priority = 1;
	}
	{
		SPR *sprite            = fdActorGet(ACTOR_IVAN)->sprite;
		sprite->image.priority = 1;
	}
}

// Once one of the suspects is boxed in for half a second in the guests' room at Vault's inn, he
// cries out, Garet calls Ivan, and Ivan reads their minds while they wonder what he is doing; the
// screen flashes green and purple.
static void scLogRoomsAReadCorneredSuspectsMinds(void)
{
	Sint32       actorId;
	FIELD_STATE *state;

	state = FieldStateWork;
	fdScriptStart();
	syCloseEvent(scLogRoomsAUpdateCorneringChase);
	gmFlagClear(FLAG_MENUS_RAISE_CUE);
	gmFlagClear(FLAG_VAULT_CORNERING_SUSPECTS);
	fdActorSetAnimation(ACTOR_SUSPECTED_LEADER, ANIM_STAND);
	fdActorSetAnimation(ACTOR_SUSPECTED_THIEF, ANIM_STAND);
	fdActorSetAnimation(ACTOR_IVAN, ANIM_STAND);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
	fdActorSetAnimation(ACTOR_GARET, ANIM_STAND);
	fdActorFaceActor(ACTOR_GARET, ACTOR_IVAN, 0);
	fdActorFaceEachOther(ACTOR_PARTY_LEADER, ACTOR_IVAN, 0);
	fdActorFaceActor(ACTOR_SUSPECTED_LEADER, ACTOR_IVAN, 0);
	fdActorFaceActor(ACTOR_SUSPECTED_THIEF, ACTOR_IVAN, 0);
	fdScriptWait(10);
	actorId = ACTOR_SUSPECTED_LEADER;
	// The script identifies the suspects blocked on the last tick of the wait.
	switch (state->raisedCue)
	{
		case LOG_ROOMS_A_THIEF_CORNERED:
		case LOG_ROOMS_A_BOTH_CORNERED:
			fdScriptSetMessageId(MsgVaultImSurrounded);
			fdActorSetAttachedEffect(ACTOR_SUSPECTED_THIEF,
			                         ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
			fdActorRunRepeatedMotion(ACTOR_SUSPECTED_THIEF, 2);
			scLogRoomsAShowMessageAndWait(ACTOR_SUSPECTED_THIEF, 20);
			if (state->raisedCue == LOG_ROOMS_A_THIEF_CORNERED)
			{
				actorId = ACTOR_SUSPECTED_THIEF;
				break;
			}
			// When both are cornered, the leader's reaction follows the thief's. fall through
		case LOG_ROOMS_A_LEADER_CORNERED:
			fdScriptSetMessageId(MsgVaultTheresNowhereRun);
			fdActorSetAttachedEffect(ACTOR_SUSPECTED_LEADER,
			                         ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
			fdActorRunRepeatedMotion(ACTOR_SUSPECTED_LEADER, 2);
			actorId = ACTOR_SUSPECTED_LEADER;
			scLogRoomsAShowMessageAndWait(ACTOR_SUSPECTED_LEADER, 20);
			break;
	}
	fdActorSetAnimation(ACTOR_IVAN, ANIM_STAND);
	fdActorFaceActor(ACTOR_IVAN, actorId, 0);
	fdActorFaceActor(ACTOR_GARET, ACTOR_IVAN, 0);
	fdActorRunRepeatedMotion(ACTOR_GARET, 2);
	fdScriptWait(20);
	// "Now, Ivan!" "A-All right!" Ivan reads the cornered one's mind while the two wonder what he
	// is doing.
	fdScriptSetMessageId(MsgVaultNowIvan);
	scLogRoomsAShowMessageAndWait(ACTOR_GARET, 20);
	fdActorRunRepeatedMotion(ACTOR_IVAN, 2);
	fdScriptWait(20);
	scLogRoomsAShowMessageAndWait(ACTOR_IVAN, 20);
	scStartMindRead(ACTOR_IVAN, actorId);
	fdPsynergyRestoreMindReadPalette();
	fdScriptWait(60);
	fdActorRunRepeatedMotion(ACTOR_SUSPECTED_LEADER, 2);
	scLogRoomsAShowMessageAndWait(ACTOR_SUSPECTED_LEADER, 20);
	fdActorRunRepeatedMotion(ACTOR_SUSPECTED_THIEF, 2);
	scLogRoomsAShowMessageAndWait(ACTOR_SUSPECTED_THIEF, 20);
	// "Are you done, Ivan?" He nods, and the window closes.
	fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
	scEndMindRead();
	fdActorStartRepeatedMotion(ACTOR_GARET, 2);
	fdActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
	fdScriptWait(20);
	scLogRoomsAShowMessageAndWait(ACTOR_GARET, 20);
	fdActorFaceActor(ACTOR_IVAN, ACTOR_GARET, 0);
	fdScriptWait(20);
	fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
	fdScriptWait(40);
	scLogRoomsASetPartyImagePriority();
	FieldStateWork->transitionFrame = 24;
	FieldStateWork->transition      = SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_SHAPE);
	fdScriptFinishScreenTransition();
	fdScriptWaitTransitionTicks();
	scLogRoomsADiscussMindReading();
	// The suspects stay at their table, turned toward the leader from now on.
	fdMapGridCopyMetatileAttributes(14, 45, 3, 1, 14, 44);
	gmFlagSet(FLAG_VAULT_SUSPECTS_MIND_READ);
	fdActorGet(ACTOR_SUSPECTED_LEADER)->timer = LEADER_SPOT_WEST_BED_FOOT;
	fdActorGet(ACTOR_SUSPECTED_THIEF)->timer  = THIEF_SPOT_SOUTH_WALL;
	sySetTickEvent(scLogRoomsAFaceSuspectsTowardLeader, EVENT_PASS_TICK);
	FieldStateWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
	fdScriptEnd();
}

// Outside the guests' room after the Mind Read, Ivan tells what he learned: it is them, and they
// hid the stolen goods somewhere in the inn. Garet says they have to search it. A Yes gets Garet's
// leave it to us; a No, another plea, from Garet and then Ivan, and after three Garet vouches for
// Isaac's sense of duty.
static void scLogRoomsADiscussMindReading(void)
{
	Sint32 persistedWithQuestions;

	gmPlayMusic();
	fdActorSetSpritePriority(ACTOR_PARTY_LEADER, 1);
	{
		ACTOR *leader         = fdActorGet(ACTOR_PARTY_LEADER);

		leader->priorityFlags = ACTOR_PRIORITY_AUTOMATIC | leader->priorityFlags;
	}
	fdSetCameraBounds(toFIXED(32), toFIXED(576), toFIXED(400), toFIXED(936));
	fdActorSetSpeed(ACTOR_PARTY_LEADER, 0.8);
	fdActorSetSpeed(ACTOR_GARET, 0.8);
	fdActorSetSpeed(ACTOR_IVAN, 0.8);
	fdActorSetPos(ACTOR_PARTY_LEADER, 248, 728);
	fdActorSetPos(ACTOR_IVAN, 264, 744);
	fdActorSetPos(ACTOR_GARET, 232, 744);
	scLogRoomsASetPartyImagePriority();
	fdActorFaceEachOther(ACTOR_PARTY_LEADER, ACTOR_IVAN, 0);
	scLogRoomsAFaceActorAndWait(ACTOR_GARET, ACTOR_IVAN, 30);
	fdActorSetPos(ACTOR_SUSPECTED_LEADER, 104, 696);
	fdActorSetPos(ACTOR_SUSPECTED_THIEF, 120, 696);
	fdActorFaceEachOther(ACTOR_SUSPECTED_LEADER, ACTOR_SUSPECTED_THIEF, 0);
	fdCameraFollowActor(ACTOR_PARTY_LEADER, 0);
	fdCameraWaitForMove();
	fdMapGridCenterOnFollowedPoint();
	fdScriptWait(30);
	{
		FIELD_STATE *state     = FieldStateWork;

		state->transitionFrame = 24;
		state->transition      = SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_SHAPE);
	}
	fdScriptStartScreenTransition();
	fdScriptWaitTransitionTicks();
	fdScriptWait(40);
	// "What did you learn, Ivan?" It is them: they hid the stolen goods somewhere in this inn, but
	// won't let him near again.
	fdActorRunRepeatedMotion(ACTOR_GARET, 1);
	fdScriptWait(10);
	fdScriptSetMessageId(MsgVaultLearn);
	scLogRoomsAShowMessageAndWait(ACTOR_GARET, 20);
	scLogRoomsAPlayAnimationAndWait(ACTOR_IVAN, ANIM_NOD, 20);
	scLogRoomsAShowMessageAndWait(ACTOR_IVAN, 20);
	scLogRoomsAFaceEachOtherAndWait(ACTOR_PARTY_LEADER, ACTOR_GARET, 50);
	fdActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_IVAN, 0);
	scLogRoomsAFaceActorAndWait(ACTOR_GARET, ACTOR_IVAN, 20);
	scLogRoomsAPlayAnimationAndWait(ACTOR_IVAN, ANIM_SHAKE_HEAD, 20);
	fdScriptShowMessage(ACTOR_IVAN, 0);
	fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
	fdScriptWait(60);
	scLogRoomsAPlayAnimationAndWait(ACTOR_IVAN, ANIM_NOD, 20);
	scLogRoomsAShowMessageAndWait(ACTOR_IVAN, 30);
	fdActorRunRepeatedMotion(ACTOR_GARET, 1);
	fdScriptWait(20);
	scLogRoomsAFaceEachOtherAndWait(ACTOR_PARTY_LEADER, ACTOR_GARET, 20);
	// Garet: they have to search the inn. Each no leads to another plea, from Garet and then Ivan;
	// three noes and Garet vouches for Isaac's sense of duty, while any yes gets "Leave it to us!"
	fdScriptOpenMessage(ACTOR_GARET, 0);
	persistedWithQuestions = 0;
	if (fdScriptAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) != 0)
	{
		fdScriptWait(20);
		fdActorStartRepeatedMotion(ACTOR_IVAN, 2);
		scLogRoomsAPlayAnimationAndWait(ACTOR_GARET, ANIM_SHAKE_HEAD, 30);
		fdScriptOpenMessage(ACTOR_GARET, 0);
		persistedWithQuestions = 0;
		if (fdScriptAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) != 0)
		{
			fdScriptWait(20);
			fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
			fdScriptWait(60);
			fdActorRunRepeatedMotion(ACTOR_IVAN, 2);
			scLogRoomsAFaceEachOtherAndWait(ACTOR_PARTY_LEADER, ACTOR_IVAN, 20);
			fdScriptOpenMessage(ACTOR_IVAN, 0);
			persistedWithQuestions = 0;
			if (fdScriptAskYesNoWithActorAnimation(ACTOR_IVAN, 0) != 0)
			{
				fdScriptWait(20);
				fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
				fdScriptWait(60);
				scLogRoomsAShowMessageAndWait(ACTOR_IVAN, 20);
				scLogRoomsAFaceActorAndWait(ACTOR_GARET, ACTOR_IVAN, 10);
				fdActorRunRepeatedMotion(ACTOR_GARET, 1);
				fdScriptWait(10);
				scLogRoomsAShowMessageAndWait(ACTOR_GARET, 10);
				scLogRoomsAFaceActorAndWait(ACTOR_IVAN, ACTOR_GARET, 20);
				fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
				fdScriptWait(60);
				scLogRoomsAPlayAnimationAndWait(ACTOR_IVAN, ANIM_SHAKE_HEAD, 20);
				scLogRoomsAShowMessageAndWait(ACTOR_GARET, 10);
				scLogRoomsAFaceEachOtherAndWait(ACTOR_GARET, ACTOR_PARTY_LEADER, 20);
				scLogRoomsAPlayAnimationAndWait(ACTOR_GARET, ANIM_NOD, 20);
				persistedWithQuestions = 1;
				scLogRoomsAShowMessageAndWait(ACTOR_GARET, 20);
			}
		}
	}
	if (persistedWithQuestions == 0)
	{
		fdScriptSetMessageId(MsgVaultLeave);
		scLogRoomsAShowMessageAndWait(ACTOR_GARET, 20);
		scLogRoomsAFaceEachOtherAndWait(ACTOR_GARET, ACTOR_PARTY_LEADER, 20);
		scLogRoomsAShowMessageAndWait(ACTOR_GARET, 20);
	}
	// "Let's go, Ivan." Garet and Ivan walk back into the leader.
	fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
	fdScriptWait(60);
	fdActorRunRepeatedMotion(ACTOR_GARET, 1);
	scLogRoomsAShowMessageAndWait(ACTOR_GARET, 10);
	scLogRoomsAPlayAnimationAndWait(ACTOR_GARET, ANIM_NOD, 10);
	fdActorFaceEachOther(ACTOR_GARET, ACTOR_IVAN, 0);
	scLogRoomsAFaceEachOtherAndWait(ACTOR_PARTY_LEADER, ACTOR_IVAN, 10);
	fdScriptShowMessage(ACTOR_GARET, 0);
	scLogRoomsAPlayAnimationAndWait(ACTOR_IVAN, ANIM_NOD, 10);
	fdActorWalkTo(ACTOR_IVAN, 248, 728);
	fdActorWalkToAndWait(ACTOR_GARET, 248, 728);
	fdActorSetPos(ACTOR_GARET, 0, 0);
	fdActorSetPos(ACTOR_IVAN, 0, 0);
}

// While the suspects are running from the party in the guests' room, Isaac stepping on one of the
// spots at the heads and feet of the beds (metatiles (4, 39) to (16, 43)) sends a suspect waiting
// at the matching spot on to the next.
static void scLogRoomsASuspectsRunFromParty(void)
{
	switch (FieldStateWork->stepCue)
	{
		case METATILE_BEHAVIOR_LOG_ROOMS_A_FIRST_SUSPECT_ROUTE:
			scLogRoomsAAdvanceSuspectRoute(
				ACTOR_SUSPECTED_LEADER, 1, 2, LogRoomsASuspectToMiddleBedHead);
			scLogRoomsAAdvanceSuspectRoute(
				ACTOR_SUSPECTED_THIEF, 3, 4, LogRoomsASuspectAlongToSouthEastCorner);
			break;
		case METATILE_BEHAVIOR_LOG_ROOMS_A_FIRST_SUSPECT_ROUTE + 1:
			scLogRoomsAAdvanceSuspectRoute(
				ACTOR_SUSPECTED_LEADER, 1, 4, LogRoomsASuspectRoundToWestBedFoot);
			scLogRoomsAAdvanceSuspectRoute(
				ACTOR_SUSPECTED_LEADER, 2, 3, LogRoomsASuspectToEastBedHead);
			scLogRoomsAAdvanceSuspectRoute(
				ACTOR_SUSPECTED_THIEF, 1, 3, LogRoomsASuspectToSouthWestCorner);
			break;
		case METATILE_BEHAVIOR_LOG_ROOMS_A_FIRST_SUSPECT_ROUTE + 2:
			scLogRoomsAAdvanceSuspectRoute(
				ACTOR_SUSPECTED_LEADER, 2, 1, LogRoomsASuspectToWestBedHead);
			scLogRoomsAAdvanceSuspectRoute(
				ACTOR_SUSPECTED_LEADER, 3, 6, LogRoomsASuspectRoundToEastBedFoot);
			scLogRoomsAAdvanceSuspectRoute(
				ACTOR_SUSPECTED_THIEF, 2, 4, LogRoomsASuspectToSouthEastCorner);
			break;
		case METATILE_BEHAVIOR_LOG_ROOMS_A_FIRST_SUSPECT_ROUTE + 3:
			scLogRoomsAAdvanceSuspectRoute(
				ACTOR_SUSPECTED_LEADER, 3, 2, LogRoomsASuspectToMiddleBedHead);
			scLogRoomsAAdvanceSuspectRoute(
				ACTOR_SUSPECTED_THIEF, 4, 3, LogRoomsASuspectAlongToSouthWestCorner);
			break;
		case METATILE_BEHAVIOR_LOG_ROOMS_A_FIRST_SUSPECT_ROUTE + 4:
			scLogRoomsAAdvanceSuspectRoute(
				ACTOR_SUSPECTED_LEADER, 4, 5, LogRoomsASuspectToMiddleBedFoot);
			scLogRoomsAAdvanceSuspectRoute(
				ACTOR_SUSPECTED_THIEF, 1, 2, LogRoomsASuspectAcrossToEastOfBeds);
			break;
		case METATILE_BEHAVIOR_LOG_ROOMS_A_FIRST_SUSPECT_ROUTE + 5:
			scLogRoomsAAdvanceSuspectRoute(
				ACTOR_SUSPECTED_LEADER, 4, 1, LogRoomsASuspectRoundToWestBedHead);
			scLogRoomsAAdvanceSuspectRoute(
				ACTOR_SUSPECTED_LEADER, 5, 6, LogRoomsASuspectToEastBedFoot);
			scLogRoomsAAdvanceSuspectRoute(
				ACTOR_SUSPECTED_THIEF, 3, 1, LogRoomsASuspectToWestOfBeds);
			break;
		case METATILE_BEHAVIOR_LOG_ROOMS_A_FIRST_SUSPECT_ROUTE + 6:
			scLogRoomsAAdvanceSuspectRoute(
				ACTOR_SUSPECTED_LEADER, 5, 4, LogRoomsASuspectToWestBedFoot);
			scLogRoomsAAdvanceSuspectRoute(
				ACTOR_SUSPECTED_LEADER, 6, 3, LogRoomsASuspectRoundToEastBedHead);
			scLogRoomsAAdvanceSuspectRoute(
				ACTOR_SUSPECTED_THIEF, 4, 2, LogRoomsASuspectToEastOfBeds);
			break;
		case METATILE_BEHAVIOR_LOG_ROOMS_A_LAST_SUSPECT_ROUTE:
			scLogRoomsAAdvanceSuspectRoute(
				ACTOR_SUSPECTED_LEADER, 6, 5, LogRoomsASuspectToMiddleBedFoot);
			scLogRoomsAAdvanceSuspectRoute(
				ACTOR_SUSPECTED_THIEF, 2, 1, LogRoomsASuspectAcrossToWestOfBeds);
			break;
	}
}

// Sends the suspect on a run when he waits at the expected spot, and numbers the spot it ends at.
static void
scLogRoomsAAdvanceSuspectRoute(Sint32 actorId, Sint32 expected, Sint32 next, const Sint32 *actions)
{
	ACTOR *suspect = fdActorGet(actorId);

	if (suspect->timer == expected)
	{
		fdActorSetScript(actorId, actions);
		suspect->timer = next;
	}
}

#include "shared/vault_leader_beside_bone_flag.c"

// Arriving in any room of Vault's houses, shops and inn sets the room up for the story so far, and
// in the loft plays its scripts.
Sint32 scInit(void)
{
	FieldStateWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
	switch (MainRoutineStateWork.entrance)
	{
		case LOG_ROOMS_A_FROM_VAULT_EAST_SOUTHEAST:
		{
			// The gem owner's house: he stands on the floor.
			fdActorGet(ACTOR_GEM_OWNER)->motionFlags  = 0;
			fdActorGet(ACTOR_GEM_OWNER)->y.fixed      = 0;
			fdActorGet(ACTOR_GEM_OWNER)->groundHeight = 0;
		}
		break;
		case LOG_ROOMS_A_FROM_VAULT_NORTHWEST:
			// The mayor's house: Ivan waits a step back once met and is gone once he has joined;
			// after the thieves are jailed, the mayor stands still, turned south-southwest.
			if (gmFlagGet(FLAG_VAULT_MET_IVAN) != 0)
			{
				fdActorSetPos(ACTOR_IVAN, 376, 376);
				fdActorFaceDirection(ACTOR_IVAN, FACING_SOUTH, 0);
			}
			if (gmFlagGet(FLAG_VAULT_IVAN_JOINED_ROD_SEARCH) != 0)
			{
				fdActorSetPos(ACTOR_IVAN, 0, 0);
			}
			if (gmFlagGet(FLAG_VAULT_THIEVES_JAILED) != 0)
			{
				fdActorSetPos(ACTOR_MAYOR, 536, MAYOR_AFTER_THIEVES_Z);
				fdActorSetMovementTypeById(ACTOR_MAYOR, MOVEMENT_TYPE_STAND);
				fdActorFaceDirection(ACTOR_MAYOR, FACING_SOUTH_SOUTHWEST, 0);
			}
			break;
		case LOG_ROOMS_A_FROM_VAULT_SOUTHWEST:
		case LOG_ROOMS_A_INN_FROM_INN_UPSTAIRS:
			// The inn: once the thieves are jailed, the Bone lies on the barrel beside the girl at
			// the inn; after the mind reading and before that, she is out.
			if (gmFlagGet(FLAG_VAULT_THIEVES_JAILED) != 0)
			{
				fdActorSetPos(ACTOR_GIRL_AT_THE_INN, 568, 648);
				fdActorSetMovementTypeById(ACTOR_GIRL_AT_THE_INN, MOVEMENT_TYPE_STAND);
				fdActorFaceDirection(ACTOR_GIRL_AT_THE_INN, FACING_SOUTH, 0);
				scVaultPlaceItemIcon(ITEM_BONE, toFIXED(568), toFIXED(16), toFIXED(672));
				sySetTickEvent(scVaultUpdateLeaderBesideBoneFlag, EVENT_PASS_TICK);
			}
			else if (gmFlagGet(FLAG_VAULT_SUSPECTS_MIND_READ) != 0)
			{
				fdActorSetPos(ACTOR_GIRL_AT_THE_INN, 0, 0);
			}
			break;
		case LOG_ROOMS_A_INN_UPSTAIRS_FROM_INN:
			// The guests' room upstairs at the inn: back from a battle mid-chase, the suspects keep
			// running.
			if (gmFlagGet(FLAG_KEEP_PARTY_POSITION) != 0
			    && gmFlagGet(FLAG_VAULT_SUSPECTS_APPROACHED) != 0
			    && gmFlagGet(FLAG_VAULT_SUSPECTS_MIND_READ) == 0
			    && gmFlagGet(FLAG_SUSPECTS_FLEEING) != 0)
			{
				fdMapGridCopyMetatileAttributes(14, 45, 3, 1, 14, 44);
				sySetTickEvent(scLogRoomsAFaceSuspectsTowardLeader, EVENT_PASS_TICK);
				break;
			}
			if (gmFlagGet(FLAG_VAULT_IVAN_JOINED_ROD_SEARCH) != 0)
			{
				fdActorSetPos(ACTOR_SUSPECTED_THIEF, 120, 696);
				fdActorFaceDirection(ACTOR_SUSPECTED_THIEF, FACING_WEST, 0);
			}
			// Once approached, the room's doorway metatile behavior changes; after the mind reading
			// the suspects sit turned toward the leader.
			if (gmFlagGet(FLAG_VAULT_SUSPECTS_APPROACHED) != 0)
			{
				fdMapGridCopyMetatileAttributes(14, 45, 3, 1, 14, 44);
				if (gmFlagGet(FLAG_VAULT_SUSPECTS_MIND_READ) == 0)
				{
					fdMapGridCopyMetatileAttributes(14, 50, 3, 1, 14, 44);
					break;
				}
				fdActorGet(ACTOR_SUSPECTED_LEADER)->timer = LEADER_SPOT_WEST_BED_FOOT;
				fdActorGet(ACTOR_SUSPECTED_THIEF)->timer  = THIEF_SPOT_SOUTH_WALL;
				sySetTickEvent(scLogRoomsAFaceSuspectsTowardLeader, EVENT_PASS_TICK);
			}
			break;
		case LOG_ROOMS_A_FROM_VAULT_WEST:
		case LOG_ROOMS_A_CRATE_ROOM_FROM_LOFT:
			// The room with the crate: the crate keeps its place once moved, and the passage behind
			// it stays open. Metatile (54, 2)'s blank picture covers what its camera box takes in
			// of the rooms beside it, (35, 20) to (36, 29) on the west and (46, 21) to (49, 28) on
			// the east, the loft's side, on the layers that draw them (the middle one from column
			// 60, the overhead one from row 60): they show black (render probe scene LogRoomsA
			// --entrance 13 --map).
			fdScriptWait(2);
			fdMapGridCopyMetatilesTo(54, 2, 35, 20, 2, 10);
			fdMapGridCopyMetatilesTo(54, 2, 95, 20, 2, 10);
			fdMapGridCopyMetatilesTo(54, 2, 35, 80, 2, 10);
			fdMapGridCopyMetatilesTo(54, 2, 46, 21, 4, 8);
			fdMapGridCopyMetatilesTo(54, 2, 46, 81, 4, 8);
			fdActorGet(ACTOR_CRATE)->collisionFlags |= ACTOR_NEAR_TALK;
			fdActorSetSpriteFlags(fdActorGet(ACTOR_CRATE), 0);
			// The metatiles scLogRoomsACheckCrateMoved copies as it sets the flag, redrawn on
			// arrival.
			if (gmFlagGet(FLAG_VAULT_CRATE_MOVED) != 0)
			{
				fdActorSetPos(ACTOR_CRATE, 676, 411);
				fdMapGridCopyMetatileAttributes(101, 24, 3, 4, 41, 24);
			}
			break;
		case LOG_ROOMS_A_LOFT_FROM_CRATE_ROOM:
			// The loft's camera box takes in the crate room's east side, (44, 21) to (45, 28): the
			// blank metatile covers it on the ground and overhead layers.
			fdScriptWait(2);
			fdMapGridCopyMetatilesTo(54, 2, 44, 21, 2, 8);
			fdMapGridCopyMetatilesTo(54, 2, 44, 81, 2, 8);
			fdScriptStart();
			// The loft: emptied chests after the thieves are jailed; else the tied-up villager lies
			// tied, groaning as the music fades out, or the thieves wait for a rematch.
			if (gmFlagGet(FLAG_VAULT_THIEVES_JAILED) != 0)
			{
				fdActorSetAnimation(ACTOR_FIRST_CHEST, CHEST_ANIMATION_OPEN_EMPTY);
				fdActorSetAnimation(ACTOR_SECOND_CHEST, CHEST_ANIMATION_OPEN_EMPTY);
				fdScriptWait(1);
				fdActorSetAnimation(ACTOR_THIRD_CHEST, CHEST_ANIMATION_OPEN_EMPTY);
				break;
			}
			fdActorSetPos(ACTOR_TIED_UP_VILLAGER, 824, 456);
			if (gmFlagGet(FLAG_VAULT_THIEVES_ATTACKED) != 0)
			{
				scLogRoomsAFightThievesAgain();
				fdScriptEnd();
				break;
			}
			fdActorSetAnimation(ACTOR_TIED_UP_VILLAGER,
			                    YOUNG_MAN_WITH_BROWN_HAIR_ANIMATION_TIED_UP);
			scLogRoomsAOpenWindowTransition();
			sySoundPlay(SOUND_FADE_OUT_BGM);
			fdScriptSetMessageId(MsgVaultUhnnUhnn);
			scLogRoomsAShowMessageAndWait(ACTOR_TIED_UP_VILLAGER, 10);
			fdScriptEnd();
			break;
		case LOG_ROOMS_A_LOFT_AFTER_LOSING_TO_THIEVES:
			// Lost to the thieves: they gloat, and the party wakes up elsewhere. The crate room's
			// east side is covered as on the way in.
			fdMapGridCopyMetatiles(54, 2, 2, 8, 44, 21);
			fdMapGridCopyMetatiles(54, 2, 2, 8, 44, 81);
			fdActorSetPos(ACTOR_TIED_UP_VILLAGER, 824, 456);
			scLogRoomsALoseToThieves();
			syImportScriptSetExitRequest(LOG_ROOMS_A_TO_SANCTUM);
			break;
		case LOG_ROOMS_A_LOFT_AFTER_THIEVES_DEFEATED:
			// Won: the thieves are caught and the stolen goods handed back. The crate room's east
			// side is covered as on the way in.
			fdMapGridCopyMetatiles(54, 2, 2, 8, 44, 21);
			fdMapGridCopyMetatiles(54, 2, 2, 8, 44, 81);
			if (gmFlagGet(FLAG_KEEP_PARTY_POSITION) == 0)
			{
				fdActorSetPos(ACTOR_TIED_UP_VILLAGER, 824, 456);
				scLogRoomsADefeatThieves();
				scLogRoomsAReturnStolenGoods();
				break;
			}
			fdActorSetAnimation(ACTOR_FIRST_CHEST, CHEST_ANIMATION_OPEN_EMPTY);
			fdActorSetAnimation(ACTOR_SECOND_CHEST, CHEST_ANIMATION_OPEN_EMPTY);
			fdScriptWait(1);
			fdActorSetAnimation(ACTOR_THIRD_CHEST, CHEST_ANIMATION_OPEN_EMPTY);
			break;
	}
	return 0;
}

// Every tick while the suspects are running from the party, both keep facing Isaac.
static void scLogRoomsAFaceSuspectsTowardLeader(void)
{
	ACTOR *origin  = fdActorGet(ACTOR_PARTY_LEADER);
	ACTOR *first   = fdActorGet(ACTOR_SUSPECTED_LEADER);
	ACTOR *second  = fdActorGet(ACTOR_SUSPECTED_THIEF);

	first->facing  = syAtan(origin->z.fixed - first->z.fixed, origin->x.fixed - first->x.fixed);
	second->facing = syAtan(origin->z.fixed - second->z.fixed, origin->x.fixed - second->x.fixed);
}

// After a lost battle with the thieves in the inn's loft, the party lies collapsed while the
// thieves say these kids are nothing to worry about and keep the tied-up young man as their
// hostage.
static void scLogRoomsALoseToThieves(void)
{
	Sint32 priorityFlags = ACTOR_PRIORITY_UNDERFOOT;

	// The party lies collapsed on the floor before the three thieves.
	fdScriptStart();
	fdActorSetPos(ACTOR_THIEF_LEADER, 792, 416);
	fdActorSetPos(ACTOR_FIRST_THIEF, 800, 400);
	fdActorSetPos(ACTOR_SECOND_THIEF, 776, 416);
	fdActorFaceDirection(ACTOR_THIEF_LEADER, FACING_SOUTH_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_FIRST_THIEF, FACING_SOUTH_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_SECOND_THIEF, FACING_SOUTH_SOUTHEAST, 0);
	fdActorSetSpeed(ACTOR_FIRST_THIEF, 0.8);
	fdActorSetSpeed(ACTOR_SECOND_THIEF, 0.8);
	fdActorSetPos(ACTOR_PARTY_LEADER, 784, 448);
	fdActorSetPos(ACTOR_GARET, 808, 432);
	fdActorSetPos(ACTOR_IVAN, 776, 432);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_COLLAPSE);
	fdActorSetAnimation(ACTOR_GARET, ANIM_COLLAPSE);
	fdActorSetAnimation(ACTOR_IVAN, ANIM_COLLAPSE);
	fdActorGet(ACTOR_PARTY_LEADER)->priorityFlags = priorityFlags;
	fdActorGet(ACTOR_GARET)->priorityFlags        = priorityFlags;
	fdActorGet(ACTOR_IVAN)->priorityFlags         = priorityFlags;
	fdActorSetSpriteFlags(fdActorGet(ACTOR_PARTY_LEADER), 0);
	fdActorSetSpriteFlags(fdActorGet(ACTOR_IVAN), 0);
	fdActorSetSpriteFlags(fdActorGet(ACTOR_GARET), 0);
	fdActorFaceDirection(ACTOR_TIED_UP_VILLAGER, FACING_NORTH_NORTHWEST, 0);
	FieldStateWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
	fdCameraFollowActor(ACTOR_PARTY_LEADER, 0);
	fdCameraWaitForMove();
	fdMapGridCenterOnFollowedPoint();
	scLogRoomsAOpenWindowTransition();
	fdScriptWait(60);
	// "These kids are nothing to worry about." The tied-up villager groans.
	scLogRoomsAPlayAnimationAndWait(ACTOR_THIEF_LEADER, ANIM_NOD, 20);
	fdScriptSetMessageId(MsgVaultTheseKidsNothingWorryAbout);
	scLogRoomsAShowMessageAndWait(ACTOR_THIEF_LEADER, 30);
	scLogRoomsAShowMessageAndWait(ACTOR_TIED_UP_VILLAGER, 30);
	// The two men step to the tied-up villager: he stays as their hostage.
	fdActorWalkTo(ACTOR_FIRST_THIEF, 808, 456);
	fdActorWalkTo(ACTOR_SECOND_THIEF, 792, 456);
	fdActorWaitForMoveById(ACTOR_SECOND_THIEF);
	fdActorFaceDirection(ACTOR_SECOND_THIEF, FACING_EAST, 0);
	fdActorWaitForMoveById(ACTOR_FIRST_THIEF);
	fdActorFaceDirection(ACTOR_FIRST_THIEF, FACING_EAST, 0);
	fdScriptWait(30);
	scLogRoomsAPlayAnimationAndWait(ACTOR_FIRST_THIEF, ANIM_NOD, 20);
	scLogRoomsAShowMessageAndWait(ACTOR_FIRST_THIEF, 20);
	scLogRoomsAFaceActorAndWait(ACTOR_SECOND_THIEF, ACTOR_PARTY_LEADER, 30);
	scLogRoomsAShowMessageAndWait(ACTOR_SECOND_THIEF, 60);
	fdScriptEnd();
}

// Coming back up into the inn's loft after losing to the thieves, they are waiting: they complain
// the party won't go away, and attack again.
static void scLogRoomsAFightThievesAgain(void)
{
	// The thieves stand over the tied-up villager as the party comes up.
	fdActorSetPos(ACTOR_THIEF_LEADER, 792, 416);
	fdActorSetPos(ACTOR_FIRST_THIEF, 800, 400);
	fdActorSetPos(ACTOR_SECOND_THIEF, 776, 416);
	fdActorFaceDirection(ACTOR_THIEF_LEADER, FACING_SOUTH_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_FIRST_THIEF, FACING_SOUTH_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_SECOND_THIEF, FACING_SOUTH_SOUTHEAST, 0);
	fdActorSetPos(ACTOR_PARTY_LEADER, 792, 440);
	fdActorSetPos(ACTOR_GARET, 808, 432);
	fdActorSetPos(ACTOR_IVAN, 792, 456);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTH_NORTHWEST, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTH_NORTHWEST, 0);
	fdActorFaceActor(ACTOR_TIED_UP_VILLAGER, ACTOR_THIEF_LEADER, 0);
	FieldStateWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
	fdCameraFollowActor(ACTOR_PARTY_LEADER, 0);
	fdCameraWaitForMove();
	fdMapGridCenterOnFollowedPoint();
	sySynch(1);
	FieldStateWork->transitionFrame = 32;
	scLogRoomsAOpenWindowTransition();
	fdScriptWait(60);
	fdScriptSetMessageId(MsgVaultWontGoAway);
	fdActorRunRepeatedMotion(ACTOR_FIRST_THIEF, 1);
	fdScriptWait(20);
	scLogRoomsAShowMessageAndWait(ACTOR_FIRST_THIEF, 30);
	fdActorRunRepeatedMotion(ACTOR_SECOND_THIEF, 1);
	fdScriptWait(20);
	fdScriptShowMessage(ACTOR_SECOND_THIEF, 0);
	scLogRoomsAFaceEachOtherAndWait(ACTOR_THIEF_LEADER, ACTOR_FIRST_THIEF, 30);
	fdActorSetAnimation(ACTOR_THIEF_LEADER, ANIM_NOD);
	scLogRoomsAPlayAnimationAndWait(ACTOR_FIRST_THIEF, ANIM_NOD, 30);
	scLogRoomsAFaceEachOtherAndWait(ACTOR_THIEF_LEADER, ACTOR_SECOND_THIEF, 30);
	fdActorSetAnimation(ACTOR_THIEF_LEADER, ANIM_NOD);
	scLogRoomsAPlayAnimationAndWait(ACTOR_SECOND_THIEF, ANIM_NOD, 40);
	fdActorFaceDirection(ACTOR_THIEF_LEADER, FACING_SOUTH_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_FIRST_THIEF, FACING_SOUTH_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_SECOND_THIEF, FACING_SOUTH_SOUTHEAST, 0);
	fdScriptWait(20);
	// "We'll make you sorry you were ever born! Get 'em!!" and the battle.
	scLogRoomsAPlayAnimationAndWait(ACTOR_THIEF_LEADER, ANIM_SHAKE_HEAD, 20);
	fdScriptShowMessage(ACTOR_THIEF_LEADER, 0);
	FieldStateWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_BARS);
	fdScriptSetAfterBattleDestination(SCENE_LOG_ROOMS_A, LOG_ROOMS_A_LOFT_AFTER_THIEVES_DEFEATED);
	fdScriptSetDefeatDestination(SCENE_LOG_ROOMS_A, LOG_ROOMS_A_LOFT_AFTER_LOSING_TO_THIEVES);
	SET_BATTLE_TYPE(BATTLE_TYPE_SCRIPTED);
	fdRequestBattleFromEncounterTable(ENCOUNTER_ZONE_SCRIPTED_VALE_VAULT, 5);
}

// Counts the Venus Djinn Ivan holds, which he gives back before he leaves the party.
static Sint32 scLogRoomsAGetIvanVenusDjinnCount(void)
{
	return gmUnitGet(IVAN)->djinnHeldCounts[ELEMENT_VENUS];
}

#include "shared/vault_djinn.c"

// After the battle in the inn's loft, the beaten thieves lie gasping on the floor. The freed young
// man runs for the mayor, who comes up with two strongmen; the thieves are tied up.
static void scLogRoomsADefeatThieves(void)
{
	FIELD_STATE  *state;
	const Sint32 *actions;
	SPR          *secondThiefSprite;

	secondThiefSprite = fdActorGet(ACTOR_SECOND_THIEF)->sprite;
	// The beaten thieves lie gasping before the party.
	fdScriptStart();
	fdActorSetPos(ACTOR_THIEF_LEADER, 792, 416);
	fdActorSetPos(ACTOR_FIRST_THIEF, 800, 400);
	fdActorSetPos(ACTOR_SECOND_THIEF, 776, 392);
	fdActorSetSpriteFlags(fdActorGet(ACTOR_THIEF_LEADER), 0);
	fdActorSetSpriteFlags(fdActorGet(ACTOR_FIRST_THIEF), 0);
	fdActorSetSpriteFlags(fdActorGet(ACTOR_SECOND_THIEF), 0);
	fdActorSetAnimation(ACTOR_THIEF_LEADER, ANIM_THIEF_BEATEN);
	fdActorSetAnimation(ACTOR_FIRST_THIEF, ANIM_THIEF_BEATEN);
	fdActorSetAnimation(ACTOR_SECOND_THIEF, ANIM_THIEF_BEATEN);
	fdActorGet(ACTOR_SECOND_THIEF)->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
	secondThiefSprite->image.priority              = 3;
	actions                                        = LogRoomsABeatenThiefGasps;
	fdActorSetScript(ACTOR_THIEF_LEADER, actions);
	fdActorSetPos(ACTOR_PARTY_LEADER, 792, 440);
	fdActorSetPos(ACTOR_GARET, 808, 432);
	fdActorSetPos(ACTOR_IVAN, 776, 440);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTH_NORTHWEST, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTH_NORTHWEST, 0);
	fdActorSetSpeed(ACTOR_TIED_UP_VILLAGER, 0.8);
	fdActorFaceDirection(ACTOR_TIED_UP_VILLAGER, FACING_NORTH_NORTHWEST, 0);
	FieldStateWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
	fdCameraFollowActor(ACTOR_PARTY_LEADER, 0);
	fdCameraWaitForMove();
	fdMapGridCenterOnFollowedPoint();
	scLogRoomsAOpenWindowTransition();
	fdActorSetScript(ACTOR_FIRST_THIEF, actions);
	fdScriptWait(30);
	fdActorSetScript(ACTOR_SECOND_THIEF, actions);
	fdScriptWait(30);
	// "They... They got us..." The tied-up villager thought they were goners; he slips past Garet,
	// cheers the win and runs for the mayor.
	fdScriptSetMessageId(MsgVaultTheyTheyGotUs);
	scLogRoomsAShowMessageAndWait(ACTOR_THIEF_LEADER, 20);
	fdActorShowEmote(ACTOR_TIED_UP_VILLAGER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
	fdScriptWait(60);
	scLogRoomsAShowMessageAndWait(ACTOR_TIED_UP_VILLAGER, 20);
	fdActorWalkToAndWait(ACTOR_TIED_UP_VILLAGER, 808, 456);
	fdActorSetSpeed(ACTOR_GARET, 0.8);
	fdActorWalkToAndWait(ACTOR_GARET, 792, 432);
	fdActorFaceDirection(ACTOR_GARET, FACING_EAST, 0);
	fdActorWalkToAndWait(ACTOR_TIED_UP_VILLAGER, 808, 408);
	fdActorFaceDirection(ACTOR_TIED_UP_VILLAGER, FACING_WEST, 0);
	fdActorWalkToAndWait(ACTOR_GARET, 808, 432);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTH, 0);
	fdScriptWait(20);
	scLogRoomsAPlayAnimationAndWait(ACTOR_TIED_UP_VILLAGER, ANIM_NOD, 20);
	scLogRoomsAShowMessageAndWait(ACTOR_TIED_UP_VILLAGER, 20);
	fdActorWalkToAndWait(ACTOR_TIED_UP_VILLAGER, 768, 408);
	fdScriptWait(20);
	fdActorFaceActor(ACTOR_TIED_UP_VILLAGER, ACTOR_PARTY_LEADER, 0);
	fdActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_TIED_UP_VILLAGER, 0);
	fdActorFaceActor(ACTOR_GARET, ACTOR_TIED_UP_VILLAGER, 0);
	scLogRoomsAFaceActorAndWait(ACTOR_IVAN, ACTOR_TIED_UP_VILLAGER, 40);
	scLogRoomsAShowMessageAndWait(ACTOR_TIED_UP_VILLAGER, 30);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
	fdActorSetAnimation(ACTOR_GARET, ANIM_NOD);
	scLogRoomsAPlayAnimationAndWait(ACTOR_IVAN, ANIM_NOD, 20);
	fdActorWalkToAndWait(ACTOR_TIED_UP_VILLAGER, 744, 408);
	fdScriptWait(50);
	// The thieves grumble; Garet: stealing is wrong, evil never prevails.
	fdActorRunRepeatedMotion(ACTOR_FIRST_THIEF, 2);
	scLogRoomsAShowMessageAndWait(ACTOR_FIRST_THIEF, 20);
	fdActorSetScript(ACTOR_FIRST_THIEF, actions);
	fdActorRunRepeatedMotion(ACTOR_GARET, 1);
	fdScriptWait(20);
	fdActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_FIRST_THIEF, 0);
	fdActorFaceActor(ACTOR_GARET, ACTOR_FIRST_THIEF, 0);
	scLogRoomsAFaceActorAndWait(ACTOR_IVAN, ACTOR_FIRST_THIEF, 20);
	scLogRoomsAShowMessageAndWait(ACTOR_GARET, 20);
	fdActorRunRepeatedMotion(ACTOR_SECOND_THIEF, 2);
	scLogRoomsAShowMessageAndWait(ACTOR_SECOND_THIEF, 30);
	fdActorSetScript(ACTOR_SECOND_THIEF, actions);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
	fdScriptWait(60);
	scLogRoomsAShowMessageAndWait(ACTOR_GARET, 30);
	// Ivan will take the rod to Hammet in Lunpa, and asks what the two will do: go after Saturos
	// and the others.
	scLogRoomsAFaceActorAndWait(ACTOR_IVAN, ACTOR_PARTY_LEADER, 30);
	scLogRoomsAFaceActorAndWait(ACTOR_PARTY_LEADER, ACTOR_IVAN, 30);
	scLogRoomsAPlayAnimationAndWait(ACTOR_IVAN, ANIM_NOD, 20);
	scLogRoomsAShowMessageAndWait(ACTOR_IVAN, 20);
	scLogRoomsAFaceActorAndWait(ACTOR_GARET, ACTOR_IVAN, 30);
	scLogRoomsAPlayAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD, 30);
	scLogRoomsAShowMessageAndWait(ACTOR_IVAN, 20);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdScriptWait(60);
	scLogRoomsAShowMessageAndWait(ACTOR_GARET, 30);
	fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdScriptWait(60);
	scLogRoomsAFaceEachOtherAndWait(ACTOR_PARTY_LEADER, ACTOR_GARET, 20);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
	scLogRoomsAPlayAnimationAndWait(ACTOR_GARET, ANIM_NOD, 20);
	// Ivan reads Isaac's mind. Garet: "Shouldn't we stop him?" Yes: too late, he's done. No: "If
	// you say so."
	scStartMindRead(ACTOR_IVAN, ACTOR_PARTY_LEADER);
	fdPsynergyRestoreMindReadPalette();
	fdActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_IVAN, 0);
	scLogRoomsAFaceActorAndWait(ACTOR_GARET, ACTOR_IVAN, 10);
	fdActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
	fdActorRunRepeatedMotion(ACTOR_GARET, 1);
	fdScriptWait(20);
	fdActorSetAttachedEffect(ACTOR_GARET, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdScriptWait(60);
	scLogRoomsAFaceActorAndWait(ACTOR_PARTY_LEADER, ACTOR_GARET, 10);
	fdScriptOpenMessage(ACTOR_GARET, 0);
	if (fdScriptAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0)
	{
		fdScriptWait(20);
		fdActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_IVAN, 0);
		scLogRoomsAFaceActorAndWait(ACTOR_GARET, ACTOR_IVAN, 20);
		scEndMindRead();
		fdActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
		fdActorRunRepeatedMotion(ACTOR_GARET, 1);
		fdScriptWait(20);
		scLogRoomsAShowMessageAndWait(ACTOR_GARET, 20);
	}
	else
	{
		FieldStateWork->msg++;
		fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
		fdScriptWait(60);
		scLogRoomsAShowMessageAndWait(ACTOR_GARET, 20);
		fdActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_IVAN, 0);
		scLogRoomsAFaceActorAndWait(ACTOR_GARET, ACTOR_IVAN, 20);
		scEndMindRead();
		fdActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
		fdActorRunRepeatedMotion(ACTOR_GARET, 1);
		fdScriptWait(20);
	}
	// "I see... So that's what happened..." The tied-up villager comes back with the mayor and two
	// strongmen.
	fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
	fdScriptWait(60);
	scLogRoomsAPlayAnimationAndWait(ACTOR_IVAN, ANIM_SHAKE_HEAD, 20);
	fdScriptSetMessageId(MsgVaultSeeThatsHappened);
	scLogRoomsAShowMessageAndWait(ACTOR_IVAN, 20);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
	scLogRoomsAPlayAnimationAndWait(ACTOR_GARET, ANIM_NOD, 40);
	fdActorWalkToAndWait(ACTOR_TIED_UP_VILLAGER, 808, 408);
	fdActorFaceDirection(ACTOR_TIED_UP_VILLAGER, FACING_WEST, 0);
	fdScriptWait(30);
	fdActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_TIED_UP_VILLAGER, 0);
	fdActorFaceActor(ACTOR_GARET, ACTOR_TIED_UP_VILLAGER, 0);
	scLogRoomsAFaceActorAndWait(ACTOR_IVAN, ACTOR_TIED_UP_VILLAGER, 20);
	fdActorRunRepeatedMotion(ACTOR_TIED_UP_VILLAGER, 1);
	fdScriptWait(20);
	scLogRoomsAShowMessageAndWait(ACTOR_TIED_UP_VILLAGER, 20);
	fdActorSetSpeed(ACTOR_LOFT_MAYOR, 0.8);
	fdActorSetSpeed(ACTOR_LOFT_ESCORT, 0.8);
	fdActorSetSpeed(ACTOR_STRONGMAN_OPENING_THE_CHESTS, 0.8);
	fdActorSetPos(ACTOR_LOFT_MAYOR, 744, 408);
	fdActorWalkToAndWait(ACTOR_LOFT_MAYOR, 768, 408);
	scLogRoomsAFaceActorAndWait(ACTOR_LOFT_MAYOR, ACTOR_THIEF_LEADER, 30);
	fdActorSetPos(ACTOR_LOFT_ESCORT, 744, 408);
	fdActorWalkToAndWait(ACTOR_LOFT_ESCORT, 768, 408);
	fdActorSetPos(ACTOR_STRONGMAN_OPENING_THE_CHESTS, 744, 408);
	fdActorWalkTo(ACTOR_STRONGMAN_OPENING_THE_CHESTS, 784, 400);
	fdActorWalkToAndWait(ACTOR_LOFT_ESCORT, 776, 424);
	fdActorWaitForMoveById(ACTOR_STRONGMAN_OPENING_THE_CHESTS);
	fdActorFaceActor(ACTOR_LOFT_ESCORT, ACTOR_THIEF_LEADER, 0);
	scLogRoomsAFaceActorAndWait(ACTOR_STRONGMAN_OPENING_THE_CHESTS, ACTOR_THIEF_LEADER, 20);
	fdActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_LOFT_MAYOR, 0);
	fdActorFaceActor(ACTOR_GARET, ACTOR_LOFT_MAYOR, 0);
	scLogRoomsAFaceActorAndWait(ACTOR_IVAN, ACTOR_LOFT_MAYOR, 20);
	// The mayor scolds them for stealing in a disaster; the thieves talk back.
	scLogRoomsAPlayAnimationAndWait(ACTOR_LOFT_MAYOR, ANIM_SHAKE_HEAD, 20);
	scLogRoomsAShowMessageAndWait(ACTOR_LOFT_MAYOR, 20);
	scLogRoomsAShowMessageAndWait(ACTOR_FIRST_THIEF, 20);
	scLogRoomsAShowMessageAndWait(ACTOR_SECOND_THIEF, 30);
	// "What should we do with them?" Locked up for a long, long time; the two strongmen rouse the
	// thieves to tie them up.
	scLogRoomsAFaceEachOtherAndWait(ACTOR_LOFT_MAYOR, ACTOR_LOFT_ESCORT, 20);
	fdActorRunRepeatedMotion(ACTOR_LOFT_ESCORT, 1);
	fdScriptWait(20);
	scLogRoomsAShowMessageAndWait(ACTOR_LOFT_ESCORT, 20);
	scLogRoomsAPlayAnimationAndWait(ACTOR_LOFT_MAYOR, ANIM_NOD, 30);
	scLogRoomsAFaceEachOtherAndWait(ACTOR_LOFT_MAYOR, ACTOR_STRONGMAN_OPENING_THE_CHESTS, 20);
	scLogRoomsAPlayAnimationAndWait(ACTOR_LOFT_MAYOR, ANIM_NOD, 30);
	scLogRoomsAFaceActorAndWait(ACTOR_LOFT_MAYOR, ACTOR_THIEF_LEADER, 20);
	scLogRoomsAShowMessageAndWait(ACTOR_LOFT_MAYOR, 20);
	fdActorSetAnimation(ACTOR_LOFT_ESCORT, ANIM_NOD);
	scLogRoomsAPlayAnimationAndWait(ACTOR_STRONGMAN_OPENING_THE_CHESTS, ANIM_NOD, 20);
	scLogRoomsAFaceEachOtherAndWait(ACTOR_LOFT_ESCORT, ACTOR_STRONGMAN_OPENING_THE_CHESTS, 20);
	fdActorSetAnimation(ACTOR_LOFT_ESCORT, ANIM_NOD);
	scLogRoomsAPlayAnimationAndWait(ACTOR_STRONGMAN_OPENING_THE_CHESTS, ANIM_NOD, 20);
	fdActorWalkTo(ACTOR_STRONGMAN_OPENING_THE_CHESTS, 792, 392);
	fdActorWalkToAndWait(ACTOR_LOFT_ESCORT, 784, 400);
	fdActorFaceActor(ACTOR_LOFT_ESCORT, ACTOR_SECOND_THIEF, 0);
	fdActorWaitForMoveById(ACTOR_STRONGMAN_OPENING_THE_CHESTS);
	scLogRoomsAFaceActorAndWait(ACTOR_STRONGMAN_OPENING_THE_CHESTS, ACTOR_FIRST_THIEF, 20);
	fdActorRunRepeatedMotion(ACTOR_LOFT_ESCORT, 1);
	fdScriptWait(20);
	scLogRoomsAShowMessageAndWait(ACTOR_LOFT_ESCORT, 20);
	scLogRoomsAPlayAnimationAndWait(ACTOR_STRONGMAN_OPENING_THE_CHESTS, ANIM_SHAKE_HEAD, 20);
	scLogRoomsAShowMessageAndWait(ACTOR_STRONGMAN_OPENING_THE_CHESTS, 30);
	scLogRoomsAFaceEachOtherAndWait(ACTOR_LOFT_ESCORT, ACTOR_PARTY_LEADER, 20);
	scLogRoomsAShowMessageAndWait(ACTOR_LOFT_ESCORT, 20);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
	fdActorSetAnimation(ACTOR_GARET, ANIM_NOD);
	scLogRoomsAPlayAnimationAndWait(ACTOR_IVAN, ANIM_NOD, 50);
	// The window closes while the thieves are tied up.
	state                  = FieldStateWork;
	state->transitionFrame = 30;
	state->transition      = SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_SHAPE);
	fdScriptFinishScreenTransition();
	fdScriptWaitTransitionTicks();
	fdScriptWait(60);
	fdScriptEnd();
}

// With the thieves tied up, the mayor has them taken down the ladder to jail. The chests give back
// his urn, the gold statue stolen from the Sanctum and Master Hammet's Shaman's Rod; the mayor
// tells that Hammet was kidnapped by Dodonpa of Lunpa, and Ivan takes the rod and leaves the party
// to rescue him.
static void scLogRoomsAReturnStolenGoods(void)
{
	ACTOR       *temporaryActor;
	ACTOR       *actor;
	Sint32       aftermath;
	Sint32       rod;
	FIELD_STATE *state;

	state = FieldStateWork;
	gmFlagSet(FLAG_VAULT_THIEVES_JAILED);
	// With the screen still closed, the chests go back along the wall and the thieves stand tied
	// before the mayor.
	fdScriptStart();
	{
		ACTOR *actor         = fdActorGet(ACTOR_SECOND_THIEF);

		actor->priorityFlags = ACTOR_PRIORITY_AUTOMATIC | actor->priorityFlags;
	}
	fdActorMoveToAndWait(ACTOR_FIRST_CHEST, 872, 425);
	fdActorMoveToAndWait(ACTOR_SECOND_CHEST, 872, 409);
	fdActorMoveToAndWait(ACTOR_THIRD_CHEST, 872, 377);
	fdActorSetPos(ACTOR_FIRST_THIEF, 776, 392);
	fdActorSetPos(ACTOR_THIEF_LEADER, 792, 392);
	fdActorSetPos(ACTOR_SECOND_THIEF, 808, 392);
	fdActorSetAnimation(ACTOR_THIEF_LEADER, ANIM_THIEF_TIED_UP);
	fdActorSetAnimation(ACTOR_FIRST_THIEF, ANIM_THIEF_TIED_UP);
	fdActorSetAnimation(ACTOR_SECOND_THIEF, ANIM_THIEF_TIED_UP);
	fdActorFaceActor(ACTOR_FIRST_THIEF, ACTOR_PARTY_LEADER, 0);
	fdActorFaceActor(ACTOR_THIEF_LEADER, ACTOR_PARTY_LEADER, 0);
	fdActorFaceActor(ACTOR_SECOND_THIEF, ACTOR_PARTY_LEADER, 0);
	fdActorSetSpriteFlags(fdActorGet(ACTOR_THIEF_LEADER), SPRITE_FLAG_SHADOW);
	fdActorSetSpriteFlags(fdActorGet(ACTOR_FIRST_THIEF), SPRITE_FLAG_SHADOW);
	fdActorSetSpriteFlags(fdActorGet(ACTOR_SECOND_THIEF), SPRITE_FLAG_SHADOW);
	fdActorSetPos(ACTOR_LOFT_ESCORT, 768, 408);
	fdActorSetPos(ACTOR_STRONGMAN_OPENING_THE_CHESTS, 768, 424);
	fdActorMoveToAndWait(ACTOR_LOFT_MAYOR, 784, 424);
	fdActorSetPos(ACTOR_TIED_UP_VILLAGER, 808, 408);
	fdActorFaceActor(ACTOR_LOFT_ESCORT, ACTOR_LOFT_MAYOR, 0);
	fdActorFaceActor(ACTOR_TIED_UP_VILLAGER, ACTOR_LOFT_MAYOR, 0);
	fdActorFaceActor(ACTOR_STRONGMAN_OPENING_THE_CHESTS, ACTOR_THIEF_LEADER, 0);
	fdActorFaceActor(ACTOR_LOFT_MAYOR, ACTOR_THIEF_LEADER, 0);
	fdActorSetPos(ACTOR_PARTY_LEADER, 792, 440);
	fdActorSetPos(ACTOR_GARET, 808, 440);
	fdActorSetPos(ACTOR_IVAN, 776, 440);
	fdActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_THIEF_LEADER, 0);
	fdActorFaceActor(ACTOR_GARET, ACTOR_THIEF_LEADER, 0);
	fdActorFaceActor(ACTOR_IVAN, ACTOR_THIEF_LEADER, 0);
	FieldStateWork->transitionFrame = 30;
	FieldStateWork->transition      = SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_SHAPE);
	fdScriptStartScreenTransition();
	fdScriptWaitTransitionTicks();
	fdScriptWait(40);
	// "Isaac, right? I won't forget this!" "Take them away!"
	fdActorRunRepeatedMotion(ACTOR_THIEF_LEADER, 2);
	aftermath = MsgVaultYouIsaacRightWontForget;
	fdScriptSetMessageId(aftermath);
	scLogRoomsAShowMessageAndWait(ACTOR_THIEF_LEADER, 20);
	fdActorRunRepeatedMotion(ACTOR_LOFT_MAYOR, 1);
	fdScriptWait(20);
	scLogRoomsAPlayAnimationAndWait(ACTOR_LOFT_MAYOR, ANIM_SHAKE_HEAD, 20);
	scLogRoomsAShowMessageAndWait(ACTOR_LOFT_MAYOR, 20);
	fdActorSetAnimation(ACTOR_LOFT_ESCORT, ANIM_NOD);
	scLogRoomsAPlayAnimationAndWait(ACTOR_TIED_UP_VILLAGER, ANIM_NOD, 20);
	fdActorStartRepeatedMotion(ACTOR_FIRST_THIEF, 2);
	fdActorStartRepeatedMotion(ACTOR_SECOND_THIEF, 2);
	fdScriptWait(60);
	// The escort leads the three thieves down the ladder, the tied-up villager after them.
	fdActorSetSpeed(ACTOR_LOFT_ESCORT, 0.8);
	fdActorWalkTo(ACTOR_LOFT_ESCORT, 746, 408);
	fdActorFaceDirection(ACTOR_LOFT_MAYOR, FACING_NORTH_NORTHWEST, 0);
	fdActorFaceDirection(ACTOR_STRONGMAN_OPENING_THE_CHESTS, FACING_NORTH_NORTHWEST, 0);
	fdActorSetScript(ACTOR_FIRST_THIEF, LogRoomsATiedThiefLeavesLoft);
	fdScriptWait(20);
	fdActorSetScript(ACTOR_THIEF_LEADER, LogRoomsATiedThiefLeavesLoft);
	fdScriptWait(15);
	fdActorSetScript(ACTOR_SECOND_THIEF, LogRoomsATiedThiefLeavesLoft);
	fdScriptWait(35);
	fdActorSetScript(ACTOR_TIED_UP_VILLAGER, LogRoomsAVillagerLeavesLoft);
	fdScriptWait(20);
	fdActorWaitForMoveById(ACTOR_LOFT_ESCORT);
	fdActorSetPos(ACTOR_LOFT_ESCORT, 0, 0);
	fdScriptWait(40);
	// The mayor and the strongman thank the party.
	fdActorSetSpeed(ACTOR_LOFT_MAYOR, 0.8);
	fdActorSetSpeed(ACTOR_STRONGMAN_OPENING_THE_CHESTS, 0.8);
	fdActorWalkToAndWait(ACTOR_LOFT_MAYOR, 784, 408);
	fdActorFaceActor(ACTOR_LOFT_MAYOR, ACTOR_PARTY_LEADER, 0);
	fdActorWalkToAndWait(ACTOR_STRONGMAN_OPENING_THE_CHESTS, 768, 408);
	scLogRoomsAFaceActorAndWait(ACTOR_STRONGMAN_OPENING_THE_CHESTS, ACTOR_PARTY_LEADER, 20);
	scLogRoomsAPlayAnimationAndWait(ACTOR_LOFT_MAYOR, ANIM_NOD, 20);
	scLogRoomsAShowMessageAndWait(ACTOR_LOFT_MAYOR, 20);
	scLogRoomsAPlayAnimationAndWait(ACTOR_STRONGMAN_OPENING_THE_CHESTS, ANIM_NOD, 20);
	fdActorFaceDirection(ACTOR_STRONGMAN_OPENING_THE_CHESTS, FACING_SOUTHEAST, 10);
	scLogRoomsAShowMessageAndWait(ACTOR_STRONGMAN_OPENING_THE_CHESTS, 20);
	scLogRoomsAFaceEachOtherAndWait(ACTOR_PARTY_LEADER, ACTOR_GARET, 50);
	scLogRoomsAFaceEachOtherAndWait(ACTOR_PARTY_LEADER, ACTOR_IVAN, 50);
	fdActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_LOFT_MAYOR, 0);
	fdActorFaceActor(ACTOR_GARET, ACTOR_LOFT_MAYOR, 0);
	scLogRoomsAFaceActorAndWait(ACTOR_IVAN, ACTOR_LOFT_MAYOR, 20);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
	fdActorSetAnimation(ACTOR_GARET, ANIM_NOD);
	scLogRoomsAPlayAnimationAndWait(ACTOR_IVAN, ANIM_NOD, 50);
	// "Let's take a look at what they stole." The two go round to the chests.
	fdActorFaceEachOther(ACTOR_LOFT_MAYOR, ACTOR_STRONGMAN_OPENING_THE_CHESTS, 0);
	scLogRoomsAPlayAnimationAndWait(ACTOR_LOFT_MAYOR, ANIM_NOD, 20);
	scLogRoomsAShowMessageAndWait(ACTOR_LOFT_MAYOR, 20);
	fdActorSetScript(ACTOR_STRONGMAN_OPENING_THE_CHESTS, LogRoomsAToSecondChest);
	fdScriptWait(50);
	fdActorSetScript(ACTOR_LOFT_MAYOR, LogRoomsAToFirstChest);
	fdActorSetSpeed(ACTOR_GARET, 0.8);
	fdActorSetSpeed(ACTOR_IVAN, 0.8);
	fdActorWalkToAndWait(ACTOR_GARET, 792, 456);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTH_NORTHEAST, 0);
	fdScriptWait(30);
	fdActorWalkToAndWait(ACTOR_IVAN, 792, 408);
	fdActorFaceDirection(ACTOR_IVAN, FACING_EAST, 0);
	fdActorWalkToAndWait(ACTOR_GARET, 808, 456);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTH_NORTHEAST, 0);
	fdScriptWait(100);
	scLogRoomsAFaceActorAndWait(ACTOR_STRONGMAN_OPENING_THE_CHESTS, ACTOR_LOFT_MAYOR, 60);
	scLogRoomsAFaceActorAndWait(ACTOR_LOFT_MAYOR, ACTOR_STRONGMAN_OPENING_THE_CHESTS, 40);
	scLogRoomsAPlayAnimationAndWait(ACTOR_LOFT_MAYOR, ANIM_NOD, 40);
	fdActorFaceDirection(ACTOR_LOFT_MAYOR, FACING_EAST, 0);
	fdScriptWait(20);
	// The mayor opens the first chest and his urn rises out of it.
	fdActorRunRepeatedMotion(ACTOR_LOFT_MAYOR, 2);
	sySoundPlay(SE_CHEST_OPEN);
	fdActorSetAnimation(ACTOR_FIRST_CHEST, CHEST_ANIMATION_OPEN_ON_TREASURE);
	fdActorSetPos(ACTOR_FOUND_URN, 872, 424);
	fdActorSetSpritePriority(ACTOR_FOUND_URN, 1);
	fdActorSetSpeed(ACTOR_FOUND_URN, 0.8);
	fdActorSetDestinationOffset(ACTOR_FOUND_URN, 0, -8);
	fdActorWaitForMoveById(ACTOR_FOUND_URN);
	fdActorRunRepeatedMotion(ACTOR_FOUND_URN, 2);
	fdScriptWait(60);
	// "Found the mayor's precious urn."
	mnFieldMessageShow((aftermath + 5), 1);
	fdActorSetAnimation(ACTOR_FIRST_CHEST, CHEST_ANIMATION_OPEN_EMPTY);
	fdActorSetPos(ACTOR_FOUND_URN, 0, 0);
	FieldStateWork->msg++;
	fdActorRunRepeatedMotion(ACTOR_STRONGMAN_OPENING_THE_CHESTS, 1);
	scLogRoomsAShowMessageAndWait(ACTOR_STRONGMAN_OPENING_THE_CHESTS, 20);
	scLogRoomsAFaceEachOtherAndWait(ACTOR_PARTY_LEADER, ACTOR_GARET, 40);
	scLogRoomsAFaceActorAndWait(ACTOR_LOFT_MAYOR, ACTOR_STRONGMAN_OPENING_THE_CHESTS, 20);
	scLogRoomsAPlayAnimationAndWait(ACTOR_LOFT_MAYOR, ANIM_NOD, 20);
	scLogRoomsAShowMessageAndWait(ACTOR_LOFT_MAYOR, 30);
	fdActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_STRONGMAN_OPENING_THE_CHESTS, 0);
	scLogRoomsAFaceActorAndWait(ACTOR_GARET, ACTOR_STRONGMAN_OPENING_THE_CHESTS, 40);
	fdActorFaceDirection(ACTOR_STRONGMAN_OPENING_THE_CHESTS, FACING_EAST, 0);
	fdScriptWait(40);
	// The strongman opens the second: the gold statue stolen from the Sanctum.
	fdActorRunRepeatedMotion(ACTOR_STRONGMAN_OPENING_THE_CHESTS, 2);
	sySoundPlay(SE_CHEST_OPEN);
	fdActorSetAnimation(ACTOR_SECOND_CHEST, CHEST_ANIMATION_OPEN_ON_TREASURE);
	fdActorGet(ACTOR_FOUND_STATUE)->motionFlags = 0;
	fdActorSetSpritePriority(ACTOR_FOUND_STATUE, 1);
	fdActorSetPos(ACTOR_FOUND_STATUE, 872, 408);
	fdActorSetSpeed(ACTOR_FOUND_STATUE, 0.8);
	fdActorSetDestinationOffset(ACTOR_FOUND_STATUE, 0, -8);
	fdActorWaitForMoveById(ACTOR_FOUND_STATUE);
	fdActorRunRepeatedMotion(ACTOR_FOUND_STATUE, 2);
	fdScriptWait(60);
	// "Found a gold statue."
	mnFieldMessageShow((aftermath + 8), 1);
	fdActorSetAnimation(ACTOR_SECOND_CHEST, CHEST_ANIMATION_OPEN_EMPTY);
	fdActorSetPos(ACTOR_FOUND_STATUE, 0, 0);
	FieldStateWork->msg++;
	fdActorShowEmote(ACTOR_LOFT_MAYOR, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
	fdScriptWait(60);
	scLogRoomsAShowMessageAndWait(ACTOR_LOFT_MAYOR, 20);
	fdActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
	fdActorStartRepeatedMotion(ACTOR_GARET, 2);
	fdActorRunRepeatedMotion(ACTOR_IVAN, 2);
	fdScriptWait(20);
	scLogRoomsAPlayAnimationAndWait(ACTOR_STRONGMAN_OPENING_THE_CHESTS, ANIM_NOD, 50);
	scLogRoomsAFaceActorAndWait(ACTOR_LOFT_MAYOR, ACTOR_PARTY_LEADER, 20);
	scLogRoomsAShowMessageAndWait(ACTOR_LOFT_MAYOR, 30);
	// The strongman finds another chest; the mayor asks whether it is what Ivan was looking for,
	// and he can return to Master Hammet.
	fdActorFaceDirection(ACTOR_STRONGMAN_OPENING_THE_CHESTS, FACING_NORTH_NORTHEAST, 0);
	fdScriptWait(30);
	fdActorShowEmote(ACTOR_STRONGMAN_OPENING_THE_CHESTS, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdScriptWait(60);
	fdActorWalkToAndWait(ACTOR_STRONGMAN_OPENING_THE_CHESTS, 856, 376);
	fdScriptWait(20);
	scLogRoomsAFaceActorAndWait(ACTOR_STRONGMAN_OPENING_THE_CHESTS, ACTOR_LOFT_MAYOR, 20);
	scLogRoomsAShowMessageAndWait(ACTOR_STRONGMAN_OPENING_THE_CHESTS, 20);
	fdActorFaceActor(ACTOR_LOFT_MAYOR, ACTOR_STRONGMAN_OPENING_THE_CHESTS, 0);
	fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdScriptWait(60);
	scLogRoomsAFaceActorAndWait(ACTOR_IVAN, ACTOR_STRONGMAN_OPENING_THE_CHESTS, 30);
	scLogRoomsAFaceActorAndWait(ACTOR_LOFT_MAYOR, ACTOR_IVAN, 20);
	scLogRoomsAPlayAnimationAndWait(ACTOR_LOFT_MAYOR, ANIM_NOD, 20);
	fdScriptShowMessage(ACTOR_LOFT_MAYOR, 0);
	fdActorFaceDirection(ACTOR_STRONGMAN_OPENING_THE_CHESTS, FACING_SOUTH_SOUTHWEST, 0);
	fdScriptWait(30);
	scLogRoomsAFaceActorAndWait(ACTOR_IVAN, ACTOR_LOFT_MAYOR, 20);
	scLogRoomsAPlayAnimationAndWait(ACTOR_IVAN, ANIM_NOD, 20);
	scLogRoomsAShowMessageAndWait(ACTOR_LOFT_MAYOR, 20);
	fdActorFaceEachOther(ACTOR_PARTY_LEADER, ACTOR_IVAN, 0);
	scLogRoomsAFaceActorAndWait(ACTOR_GARET, ACTOR_IVAN, 20);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
	scLogRoomsAPlayAnimationAndWait(ACTOR_GARET, ANIM_NOD, 40);
	scLogRoomsAPlayAnimationAndWait(ACTOR_IVAN, ANIM_NOD, 30);
	// Hammet was seen heading toward Lunpa: Dodonpa must have kidnapped him.
	fdActorRunRepeatedMotion(ACTOR_LOFT_MAYOR, 1);
	fdScriptWait(20);
	scLogRoomsAPlayAnimationAndWait(ACTOR_LOFT_MAYOR, ANIM_SHAKE_HEAD, 20);
	scLogRoomsAShowMessageAndWait(ACTOR_LOFT_MAYOR, 20);
	fdActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_LOFT_MAYOR, 0);
	fdActorFaceActor(ACTOR_GARET, ACTOR_LOFT_MAYOR, 0);
	scLogRoomsAFaceActorAndWait(ACTOR_IVAN, ACTOR_LOFT_MAYOR, 20);
	fdActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
	fdActorStartRepeatedMotion(ACTOR_GARET, 1);
	fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
	fdScriptWait(60);
	scLogRoomsAShowMessageAndWait(ACTOR_IVAN, 40);
	scLogRoomsAPlayAnimationAndWait(ACTOR_LOFT_MAYOR, ANIM_NOD, 20);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_SHAKE_HEAD);
	fdActorSetAnimationAndWait(ACTOR_GARET, ANIM_SHAKE_HEAD);
	fdActorRunRepeatedMotion(ACTOR_IVAN, 3);
	fdScriptWait(30);
	scLogRoomsAPlayAnimationAndWait(ACTOR_LOFT_MAYOR, ANIM_SHAKE_HEAD, 20);
	scLogRoomsAShowMessageAndWait(ACTOR_LOFT_MAYOR, 20);
	scLogRoomsAFaceEachOtherAndWait(ACTOR_PARTY_LEADER, ACTOR_GARET, 20);
	// Ivan rushes forward: what will happen to Hammet? The mayor calms him: Dodonpa wants a ransom,
	// so Hammet is safe for now.
	fdActorRunRepeatedMotion(ACTOR_IVAN, 2);
	scLogRoomsAPlayAnimationAndWait(ACTOR_IVAN, ANIM_SHAKE_HEAD, 30);
	fdActorSetSpeed(ACTOR_IVAN, 1.5);
	fdActorWalkToAndWait(ACTOR_IVAN, 800, 408);
	fdActorRunRepeatedMotion(ACTOR_IVAN, 2);
	fdScriptShowMessage(ACTOR_IVAN, 0);
	fdActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_LOFT_MAYOR, 0);
	scLogRoomsAFaceActorAndWait(ACTOR_GARET, ACTOR_LOFT_MAYOR, 30);
	scLogRoomsAPlayAnimationAndWait(ACTOR_LOFT_MAYOR, ANIM_SHAKE_HEAD, 20);
	scLogRoomsAShowMessageAndWait(ACTOR_LOFT_MAYOR, 20);
	fdActorSetAttachedEffect(ACTOR_PARTY_LEADER, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdActorSetAttachedEffect(ACTOR_GARET, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdActorSetAttachedEffect(ACTOR_IVAN, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdScriptWait(60);
	scLogRoomsAShowMessageAndWait(ACTOR_IVAN, 20);
	fdActorRunRepeatedMotion(ACTOR_LOFT_MAYOR, 1);
	fdScriptWait(20);
	scLogRoomsAShowMessageAndWait(ACTOR_LOFT_MAYOR, 20);
	fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdScriptWait(60);
	scLogRoomsAPlayAnimationAndWait(ACTOR_LOFT_MAYOR, ANIM_SHAKE_HEAD, 20);
	scLogRoomsAShowMessageAndWait(ACTOR_LOFT_MAYOR, 20);
	fdActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
	fdActorStartRepeatedMotion(ACTOR_GARET, 1);
	fdActorRunRepeatedMotion(ACTOR_IVAN, 1);
	fdScriptWait(20);
	scLogRoomsAPlayAnimationAndWait(ACTOR_LOFT_MAYOR, ANIM_NOD, 20);
	scLogRoomsAShowMessageAndWait(ACTOR_LOFT_MAYOR, 40);
	// The mayor asks the two their names, thanks them, and asks them to come see him before they
	// leave town.
	fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
	fdScriptWait(60);
	fdActorWalkToAndWait(ACTOR_LOFT_MAYOR, 840, 424);
	scLogRoomsAFaceEachOtherAndWait(ACTOR_LOFT_MAYOR, ACTOR_PARTY_LEADER, 20);
	scLogRoomsAShowMessageAndWait(ACTOR_LOFT_MAYOR, 20);
	scLogRoomsAPlayAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD, 20);
	fdActorFaceDirection(ACTOR_LOFT_MAYOR, FACING_SOUTH_SOUTHWEST, 0);
	fdScriptWait(20);
	scLogRoomsAShowMessageAndWait(ACTOR_LOFT_MAYOR, 20);
	fdActorRunRepeatedMotion(ACTOR_GARET, 1);
	fdScriptWait(20);
	scLogRoomsAShowMessageAndWait(ACTOR_GARET, 20);
	scLogRoomsAPlayAnimationAndWait(ACTOR_LOFT_MAYOR, ANIM_NOD, 20);
	scLogRoomsAShowMessageAndWait(ACTOR_LOFT_MAYOR, 30);
	scLogRoomsAFaceEachOtherAndWait(ACTOR_LOFT_MAYOR, ACTOR_STRONGMAN_OPENING_THE_CHESTS, 20);
	scLogRoomsAShowMessageAndWait(ACTOR_LOFT_MAYOR, 20);
	// The mayor and the strongman leave down the ladder.
	fdActorWalkToAndWait(ACTOR_STRONGMAN_OPENING_THE_CHESTS, 856, 408);
	fdActorSetScript(ACTOR_LOFT_MAYOR, LogRoomsABackFromChests);
	fdActorSetScript(ACTOR_STRONGMAN_OPENING_THE_CHESTS, LogRoomsABackFromChests);
	fdActorWalkTo(ACTOR_GARET, 792, 456);
	fdActorSetSpeed(ACTOR_IVAN, 0.8);
	fdActorWalkTo(ACTOR_IVAN, 776, 432);
	fdActorWaitForMoveById(ACTOR_GARET);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTH_NORTHEAST, 0);
	fdActorWaitForMoveById(ACTOR_IVAN);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTH_NORTHEAST, 0);
	fdActorWaitForScriptEndById(ACTOR_LOFT_MAYOR);
	fdActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_LOFT_MAYOR, 0);
	temporaryActor = fdActorGet(ACTOR_STRONGMAN_OPENING_THE_CHESTS);
	{
		Uint8 *pauseFlag = &temporaryActor->paused;
		Uint8  paused    = 1;

		*pauseFlag       = paused;
	}
	temporaryActor->targetX = ACTOR_NO_TARGET;
	temporaryActor->targetY = ACTOR_NO_TARGET;
	temporaryActor->targetZ = ACTOR_NO_TARGET;
	fdActorShowEmote(ACTOR_LOFT_MAYOR, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdActorSetAnimation(ACTOR_STRONGMAN_OPENING_THE_CHESTS, ANIM_STAND);
	fdScriptWait(50);
	fdActorFaceActor(ACTOR_LOFT_MAYOR, ACTOR_PARTY_LEADER, 0);
	fdActorWalkToAndWait(ACTOR_GARET, 808, 440);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTH_NORTHWEST, 0);
	fdScriptWait(20);
	scLogRoomsAShowMessageAndWait(ACTOR_LOFT_MAYOR, 20);
	scLogRoomsAFaceEachOtherAndWait(ACTOR_PARTY_LEADER, ACTOR_GARET, 50);
	fdActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_LOFT_MAYOR, 0);
	scLogRoomsAFaceActorAndWait(ACTOR_GARET, ACTOR_LOFT_MAYOR, 20);
	scLogRoomsAPlayAnimationAndWait(ACTOR_LOFT_MAYOR, ANIM_NOD, 20);
	fdActorWalkTo(ACTOR_LOFT_MAYOR, 744, 408);
	fdActorWalkTo(ACTOR_STRONGMAN_OPENING_THE_CHESTS, 744, 408);
	fdActorWaitForMoveById(ACTOR_LOFT_MAYOR);
	fdActorWaitForMoveById(ACTOR_STRONGMAN_OPENING_THE_CHESTS);
	fdScriptWait(30);
	// Alone, Garet tries to cheer Ivan up; Ivan only wants to rescue Hammet, but has seen the two's
	// mission in their minds and won't ask their help.
	fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 792, 408);
	scLogRoomsAFaceEachOtherAndWait(ACTOR_PARTY_LEADER, ACTOR_GARET, 30);
	fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
	fdScriptWait(50);
	fdActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_IVAN, 0);
	scLogRoomsAFaceActorAndWait(ACTOR_GARET, ACTOR_IVAN, 20);
	fdActorRunRepeatedMotion(ACTOR_GARET, 1);
	fdScriptWait(20);
	scLogRoomsAShowMessageAndWait(ACTOR_GARET, 40);
	scLogRoomsAPlayAnimationAndWait(ACTOR_IVAN, ANIM_NOD, 30);
	scLogRoomsAFaceEachOtherAndWait(ACTOR_PARTY_LEADER, ACTOR_GARET, 40);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_SHAKE_HEAD);
	scLogRoomsAPlayAnimationAndWait(ACTOR_GARET, ANIM_SHAKE_HEAD, 30);
	fdActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_IVAN, 0);
	scLogRoomsAFaceActorAndWait(ACTOR_GARET, ACTOR_IVAN, 20);
	fdActorRunRepeatedMotion(ACTOR_GARET, 1);
	fdScriptWait(20);
	scLogRoomsAShowMessageAndWait(ACTOR_GARET, 40);
	fdActorRunRepeatedMotion(ACTOR_IVAN, 2);
	fdScriptWait(20);
	scLogRoomsAShowMessageAndWait(ACTOR_IVAN, 20);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
	scLogRoomsAPlayAnimationAndWait(ACTOR_GARET, ANIM_NOD, 20);
	scLogRoomsAPlayAnimationAndWait(ACTOR_IVAN, ANIM_SHAKE_HEAD, 20);
	scLogRoomsAShowMessageAndWait(ACTOR_IVAN, 20);
	fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdScriptWait(60);
	scLogRoomsAPlayAnimationAndWait(ACTOR_IVAN, ANIM_NOD, 20);
	scLogRoomsAShowMessageAndWait(ACTOR_IVAN, 20);
	fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdScriptWait(60);
	scLogRoomsAPlayAnimationAndWait(ACTOR_IVAN, ANIM_NOD, 20);
	scLogRoomsAShowMessageAndWait(ACTOR_IVAN, 20);
	fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
	fdScriptWait(60);
	scLogRoomsAPlayAnimationAndWait(ACTOR_IVAN, ANIM_SHAKE_HEAD, 20);
	scLogRoomsAShowMessageAndWait(ACTOR_IVAN, 20);
	scLogRoomsAPlayAnimationAndWait(ACTOR_GARET, ANIM_NOD, 20);
	scLogRoomsAShowMessageAndWait(ACTOR_GARET, 30);
	// "Then, I suppose this is good-bye." A no has Garet say Isaac would help, and Ivan decline.
	fdScriptOpenMessage(ACTOR_IVAN, 0);
	if (fdScriptAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) != 0)
	{
		fdScriptWait(20);
		fdActorRunRepeatedMotion(ACTOR_GARET, 1);
		fdScriptWait(20);
		scLogRoomsAShowMessageAndWait(ACTOR_GARET, 20);
		scLogRoomsAPlayAnimationAndWait(ACTOR_IVAN, ANIM_SHAKE_HEAD, 20);
		scLogRoomsAShowMessageAndWait(ACTOR_IVAN, 20);
	}
	// Ivan walks round to the third chest and takes the Shaman's Rod from it.
	scLogRoomsAPlayAnimationAndWait(ACTOR_IVAN, ANIM_NOD, 30);
	fdActorWalkToAndWait(ACTOR_IVAN, 800, 456);
	fdActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_IVAN, 0);
	fdActorFaceActor(ACTOR_GARET, ACTOR_IVAN, 0);
	fdActorWalkToAndWait(ACTOR_IVAN, 856, 456);
	fdActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_IVAN, 0);
	fdActorFaceActor(ACTOR_GARET, ACTOR_IVAN, 0);
	fdActorWalkToAndWait(ACTOR_IVAN, 856, 376);
	fdActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_IVAN, 0);
	fdActorFaceActor(ACTOR_GARET, ACTOR_IVAN, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_EAST, 0);
	fdScriptWait(20);
	fdActorRunRepeatedMotion(ACTOR_IVAN, 2);
	temporaryActor =
		(ACTOR *)fdChestOpenAndShowItem(ACTOR_IVAN, ACTOR_THIRD_CHEST, ITEM_SHAMANS_ROD);
	fdScriptWait(60);
	rod = MsgVaultIvanGotShamansRod;
	mnFieldMessageShow(rod, 1);
	fdActorRelease(temporaryActor);
	fdActorSetAnimation(ACTOR_THIRD_CHEST, CHEST_ANIMATION_OPEN_EMPTY);
	fdScriptWait(20);
	scLogRoomsAPlayAnimationAndWait(ACTOR_IVAN, ANIM_NOD, 20);
	fdActorWalkToAndWait(ACTOR_IVAN, 856, 456);
	fdActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_IVAN, 0);
	fdActorFaceActor(ACTOR_GARET, ACTOR_IVAN, 0);
	fdActorWalkToAndWait(ACTOR_IVAN, 800, 456);
	fdActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_IVAN, 0);
	fdActorFaceActor(ACTOR_GARET, ACTOR_IVAN, 0);
	fdActorWalkToAndWait(ACTOR_IVAN, 776, 424);
	fdActorFaceEachOther(ACTOR_PARTY_LEADER, ACTOR_IVAN, 0);
	scLogRoomsAFaceActorAndWait(ACTOR_GARET, ACTOR_IVAN, 30);
	// He thanks the town, hands back any Venus Djinni and leaves the party.
	scLogRoomsAPlayAnimationAndWait(ACTOR_IVAN, ANIM_NOD, 20);
	fdScriptSetMessageId(rod + 1);
	scLogRoomsAShowMessageAndWait(ACTOR_IVAN, 20);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
	{
		Sint32 saved = (Sint16)state->msg;

		if (scLogRoomsAGetIvanVenusDjinnCount() != 0)
		{
			fdScriptSetMessageId(MsgVaultWaitDontWantTakeYour);
			// "Oh, wait, I don't want to take your Djinni. I'll just leave..."
			fdScriptShowMessage(ACTOR_IVAN, 0);
			scVaultReturnIsaacsDjinni();
		}
		fdPartyRemoveMemberRestored(IVAN);
		state->msg = saved;
	}
	scLogRoomsAPlayAnimationAndWait(ACTOR_GARET, ANIM_NOD, 50);
	fdActorWalkToAndWait(ACTOR_IVAN, 776, 408);
	fdActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_IVAN, 0);
	fdActorFaceActor(ACTOR_GARET, ACTOR_IVAN, 0);
	// Ivan goes down the ladder; Garet: they'd better get moving too.
	fdActorWalkToAndWait(ACTOR_IVAN, 744, 408);
	fdScriptWait(40);
	scLogRoomsAFaceEachOtherAndWait(ACTOR_PARTY_LEADER, ACTOR_GARET, 20);
	fdActorRunRepeatedMotion(ACTOR_GARET, 1);
	fdScriptWait(20);
	scLogRoomsAShowMessageAndWait(ACTOR_GARET, 20);
	scLogRoomsAPlayAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD, 20);
	fdActorSetAnimation(ACTOR_GARET, ANIM_WALK);
	actor = fdActorGet(ACTOR_PARTY_LEADER);
	if (actor != NULL)
	{
		fdActorSetDestination(ACTOR_GARET, actor->x.part[0].integer, actor->z.part[0].integer);
	}
	fdActorWaitForMoveById(ACTOR_GARET);
	fdActorSetPos(ACTOR_GARET, 0, 0);
	fdScriptWait(30);
	fdActorSetPos(ACTOR_TIED_UP_VILLAGER, 0, 0);
	fdActorSetPos(ACTOR_LOFT_MAYOR, 0, 0);
	fdActorSetPos(ACTOR_LOFT_ESCORT, 0, 0);
	fdActorSetPos(ACTOR_STRONGMAN_OPENING_THE_CHESTS, 0, 0);
	fdActorSetPos(ACTOR_THIEF_LEADER, 0, 0);
	fdActorSetPos(ACTOR_FIRST_THIEF, 0, 0);
	fdActorSetPos(ACTOR_SECOND_THIEF, 0, 0);
	fdActorSetPos(ACTOR_IVAN, 0, 0);
	FieldStateWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
	fdScriptEnd();
}

// Opens the window transition and waits one tick after it finishes.
static void scLogRoomsAOpenWindowTransition(void)
{
	FieldStateWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
	fdScriptStartScreenTransition();
	fdScriptWaitTransitionTicks();
	fdScriptWait(1);
}

static void scLogRoomsAShowMessageAndWait(Sint32 actorId, Sint32 waitTicks)
{
	fdScriptShowMessage(actorId, 0);
	fdScriptWait(waitTicks);
}

static void scLogRoomsAFaceActorAndWait(Sint32 actorId, Sint32 targetId, Sint32 waitTicks)
{
	fdActorFaceActor(actorId, targetId, 0);
	fdScriptWait(waitTicks);
}

static void scLogRoomsAFaceEachOtherAndWait(Sint32 firstId, Sint32 secondId, Sint32 waitTicks)
{
	fdActorFaceEachOther(firstId, secondId, 0);
	fdScriptWait(waitTicks);
}

// Plays the animation, waits until it has taken over, then waits waitTicks.
static void scLogRoomsAPlayAnimationAndWait(Sint32 actorId, Sint32 animation, Sint32 waitTicks)
{
	syImportActorSetAnimation(actorId, animation, waitTicks);
	fdActorWaitForAnimationChange(actorId);
	fdScriptWait(waitTicks);
}

#include "shared/mind_read.c"

#include "shared/vault_item_icon.c"

// The chase's stop under a position (its x, y and z), or NULL between stops.
static LOG_ROOMS_A_STOP *scLogRoomsAFindStopAtPos(Sint32 *pos)
{
	Sint32            gridX  = FIELD_HALF_METATILE(pos[0] - toFIXED(LOG_STOPS_ORIGIN_X));
	Sint32            gridZ  = FIELD_HALF_METATILE(pos[2] - toFIXED(LOG_STOPS_ORIGIN_Z));
	LOG_ROOMS_A_STOP *stop   = LogRoomsAStops;
	LOG_ROOMS_A_STOP *result = NULL;
	Uint32            i;

	for (i = 0; i < LOG_STOP_COUNT; i++, stop++)
	{
		Sint32 stopX = stop->gridX;

		if (stopX == gridX || stopX + 1 == gridX)
		{
			Sint32 stopZ = stop->gridZ;

			if (stopZ == gridZ || stopZ + 1 == gridZ)
			{
				result = stop;
				break;
			}
		}
	}
	return result;
}

// The stop linked to this one in the direction nearest requestedFacing, which is turned to that
// link's heading; NULL when it has no link.
static LOG_ROOMS_A_STOP *scLogRoomsAChooseStopTowardFacing(LOG_ROOMS_A_STOP *node,
                                                           Sint16           *requestedFacing)
{
	LOG_ROOMS_A_STOP_CONNECTION *connection;
	Sint32                       stopIndex;
	Sint32                       nearestDifference;
	Sint32                       chosenFacing;
	Sint32                       angleDifference;
	Uint32                       connectionIndex;
	Uint16                       connectionFacing;

	stopIndex         = -1;
	chosenFacing      = *requestedFacing;
	connection        = node->connections;
	nearestDifference = DEGtoANG(180);
	for (connectionIndex = 0; connectionIndex <= 2; connectionIndex++, connection++)
	{
		connectionFacing = connection->facing << 8;
		angleDifference  = (Sint16)(connectionFacing - *requestedFacing);
		if (angleDifference < 0)
			angleDifference = -angleDifference;
		if (connection->stopIndex != LOG_STOP_NONE && angleDifference < nearestDifference)
		{
			nearestDifference = angleDifference;
			// FAKEMATCH(dead): updates of chosenFacing that cancel out, under a
			// * test of it against the difference.
			if (chosenFacing != angleDifference)
			{
				chosenFacing++;
				chosenFacing--;
			}
			stopIndex    = connection->stopIndex;
			chosenFacing = (Sint16)connectionFacing;
		}
	}
	if (stopIndex == -1)
		return 0;
	*requestedFacing = chosenFacing;
	return &LogRoomsAStops[stopIndex];
}

// Whether the actor's destination, or where it stands when it has none, is within half a metatile
// (8 pixels) of the stop at (x, z).
static Sint32 scLogRoomsAIsWithinHalfMetatileOfStop(Sint32 x, Sint32 z, Sint32 id)
{
	ACTOR *actor   = fdActorLookup(id);
	Sint32 targetX = actor->targetX;
	Sint32 targetZ;

	if (targetX == ACTOR_NO_TARGET)
	{
		targetX = actor->x.fixed;
	}
	targetZ = actor->targetZ;
	if (targetZ == ACTOR_NO_TARGET)
	{
		targetZ = actor->z.fixed;
	}

	targetX = FIXEDtoINT(targetX - x);
	targetZ = FIXEDtoINT(targetZ - z);
	if (targetX * targetX + targetZ * targetZ
	    <= (MAPGRID_METATILE_PIXELS / 2) * (MAPGRID_METATILE_PIXELS / 2))
		return 1;
	return 0;
}

// Whether the actor's destination, or where it stands when it has none, is within a metatile (16
// pixels) of the stop at (x, z).
static Bool scLogRoomsAIsWithinMetatileOfStop(Sint32 x, Sint32 z, Sint32 id)
{
	ACTOR *actor = fdActorLookup(id);
	Sint32 ax;
	Sint32 az;

	ax = actor->targetX;
	if (ax == ACTOR_NO_TARGET)
		ax = actor->x.fixed;
	az = actor->targetZ;
	if (az == ACTOR_NO_TARGET)
		az = actor->z.fixed;
	ax = FIXEDtoINT(ax - x);
	az = FIXEDtoINT(az - z);
	if (ax * ax + az * az <= MAPGRID_METATILE_PIXELS * MAPGRID_METATILE_PIXELS)
		return TRUE;
	return FALSE;
}

// Whether a stop is taken: -1 when Isaac stands within a metatile of it, or Ivan or a suspect
// within half a metatile; 1 when there is no stop; else 0.
static Sint32 scLogRoomsACheckStopBlocked(LOG_ROOMS_A_STOP *stop)
{
	FIXED x;
	FIXED z;

	if (stop == NULL)
	{
		return 1;
	}
	x = LOG_STOP_X(stop);
	z = LOG_STOP_Z(stop);
	if (scLogRoomsAIsWithinMetatileOfStop(x, z, ACTOR_PARTY_LEADER) != 0
	    || scLogRoomsAIsWithinHalfMetatileOfStop(x, z, ACTOR_IVAN) != 0
	    || scLogRoomsAIsWithinHalfMetatileOfStop(x, z, ACTOR_SUSPECTED_LEADER) != 0
	    || scLogRoomsAIsWithinHalfMetatileOfStop(x, z, ACTOR_SUSPECTED_THIEF) != 0)
	{
		return -1;
	}
	return 0;
}

// Sends the actor walking to the middle of the stop.
static void scLogRoomsAMoveActorToStop(ACTOR *actor, LOG_ROOMS_A_STOP *stop)
{
	syFieldActorSetMoveTargetFX(actor, LOG_STOP_X(stop), 0, LOG_STOP_Z(stop));
}

// Every tick of the chase in the guests' room, moves Ivan and the two suspects on from stop to stop
// round the beds, and raises the cornering script once a suspect has had nowhere to run for half a
// second.
static void scLogRoomsAUpdateCorneringChase(void)
{
	ACTOR            *leader;
	ACTOR            *actor;
	FIELD_STATE      *state;
	LOG_ROOMS_A_STOP *entry;
	LOG_ROOMS_A_STOP *dest;
	Sint32            dx;
	Sint32            dz;
	Sint32            blocked;
	Uint16            angle;
	Sint16            facing;
	Sint16            facingDifference;

	leader  = fdActorLookup(ACTOR_PARTY_LEADER);
	blocked = 0;
	state   = FieldStateWork;

	// Ivan runs ahead of the leader when the leader pushes toward him from close by, and otherwise
	// walks on the way he faces.
	actor   = fdActorLookup(ACTOR_IVAN);
	entry   = scLogRoomsAFindStopAtPos(&actor->x.fixed);
	if (entry != NULL && actor->targetX == ACTOR_NO_TARGET)
	{
		dx     = actor->x.fixed - leader->x.fixed;
		dz     = actor->z.fixed - leader->z.fixed;
		facing = leader->facing;
		angle  = syAtan(dz, dx);
		dx     = FIXEDtoINT(dx);
		dz     = FIXEDtoINT(dz);
		if (state->blockedSteps > 0 && dx * dx + dz * dz <= 20 * 20
		    && (facingDifference = facing - angle) > -FACING_STEP && facingDifference < FACING_STEP)
		{
			// Pushed toward from close by, he takes the leader's way. FAKEMATCH(dead): a dead
			// store.
			facingDifference = 0;
#if VAULT_CHASE_IVAN_TAKES_LEADER_HEADING_UP_CLOSE
		}
		else if (dx * dx + dz * dz > 8 * 8)
		{
#else
		}
		else
		{
#endif
			facing = actor->facing;
		}
		dest = scLogRoomsAChooseStopTowardFacing(entry, &facing);
		if (scLogRoomsACheckStopBlocked(dest) == 0)
		{
			scLogRoomsAMoveActorToStop(actor, dest);
			fdActorSelectAnimation(actor, ANIM_WALK);
		}
		else
		{
			fdActorSelectAnimation(actor, ANIM_STAND);
		}
	}

	// Each suspect tries a random turn of three sixteenths, the leader to either side and the thief
	// to either side or straight on, reckoned in 16.16 so that it wraps round. Where someone stands
	// he turns back, and with both ways taken he shakes his head.
	actor = fdActorLookup(ACTOR_SUSPECTED_LEADER);
	entry = scLogRoomsAFindStopAtPos(&actor->x.fixed);
	if (entry != NULL && actor->targetX == ACTOR_NO_TARGET)
	{
		Uint32 turn = syRandomBelow(2);
		Sint32 currentFacing;

		turn          *= toFIXED(FACING_STEP * 6);
		currentFacing  = actor->facing;
		facing         = FIXEDtoINT(turn - toFIXED(FACING_STEP * 3)) + currentFacing;
		dest           = scLogRoomsAChooseStopTowardFacing(entry, &facing);
		if (scLogRoomsACheckStopBlocked(dest) != 0)
		{
			facing = actor->facing + DEGtoANG(180);
			dest   = scLogRoomsAChooseStopTowardFacing(entry, &facing);
			if (scLogRoomsACheckStopBlocked(dest) == 0)
			{
				fdActorSetAttachedEffect(ACTOR_SUSPECTED_LEADER, ATTACHED_EFFECT_ONCE);
				goto leaderMoves;
			}
			fdActorSelectAnimation(actor, ANIM_SHAKE_HEAD);
			blocked = 1;
		}
		else
		{
		leaderMoves:
			scLogRoomsAMoveActorToStop(actor, dest);
			fdActorSelectAnimation(actor, ANIM_WALK);
		}
	}
	actor = fdActorLookup(ACTOR_SUSPECTED_THIEF);
	entry = scLogRoomsAFindStopAtPos(&actor->x.fixed);
	if (entry != NULL && actor->targetX == ACTOR_NO_TARGET)
	{
		Uint32 turn = syRandomBelow(3);
		Sint32 currentFacing;

		turn          *= toFIXED(FACING_STEP * 3);
		currentFacing  = actor->facing;
		facing         = FIXEDtoINT(turn - toFIXED(FACING_STEP * 3)) + currentFacing;
		dest           = scLogRoomsAChooseStopTowardFacing(entry, &facing);
		if (scLogRoomsACheckStopBlocked(dest) != 0)
		{
			facing = actor->facing + DEGtoANG(180);
			dest   = scLogRoomsAChooseStopTowardFacing(entry, &facing);
			if (scLogRoomsACheckStopBlocked(dest) == 0)
			{
				fdActorSetAttachedEffect(ACTOR_SUSPECTED_THIEF, ATTACHED_EFFECT_ONCE);
				goto thiefMoves;
			}
			fdActorSelectAnimation(actor, ANIM_SHAKE_HEAD);
			blocked += 2;
		}
		else
		{
		thiefMoves:
			scLogRoomsAMoveActorToStop(actor, dest);
			fdActorSelectAnimation(actor, ANIM_WALK);
		}
	}

	// Only half a second with nowhere to run counts as cornered, naming who.
	if (blocked != 0)
	{
		if (++LogRoomsACorneredTicks >= LOG_ROOMS_A_CORNERED_TICKS)
			state->raisedCue = LOG_ROOMS_A_CORNERED_SCRIPT_BASE + blocked;
	}
	else
	{
		LogRoomsACorneredTicks = 0;
	}
}

// Sets the chase in the guests' room going: Isaac walks in, Garet and Ivan step out beside him,
// Garet spreads his arms, and the camera is held on the room.
static void scLogRoomsASpreadOutToCornerSuspects(void)
{
	fdScriptStart();
	fdCameraMoveTo(168, CAMERA_KEEP, 656, 1);
	fdActorSetSpeed(ACTOR_PARTY_LEADER, 0.8);
	fdActorSetSpeed(ACTOR_GARET, 0.8);
	fdActorSetSpeed(ACTOR_IVAN, 0.8);
	fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 248, 696);
	fdActorSetPos(ACTOR_GARET, 248, 696);
	fdActorSetPos(ACTOR_IVAN, 248, 696);
	fdActorWalkTo(ACTOR_PARTY_LEADER, 200, 696);
	fdActorWalkTo(ACTOR_GARET, 248, 712);
	fdActorWalkToAndWait(ACTOR_IVAN, 232, 696);
	fdActorWaitForMoveById(ACTOR_GARET);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_WEST, 0);
	fdActorWaitForMoveById(ACTOR_PARTY_LEADER);
	fdActorSetAnimation(ACTOR_GARET, ANIM_GARET_ARMS_SPREAD);
	scLogRoomsAResetSuspectRoutes();
	fdSetCameraBounds(toFIXED(48), toFIXED(576), toFIXED(288), toFIXED(736));
	fdActorSetSpeed(ACTOR_GARET, 1);
	fdActorSetSpeed(ACTOR_IVAN, 0.75);
	fdActorSetSpeedAndAcceleration(ACTOR_SUSPECTED_LEADER, 1, 1.2);
	fdActorSetSpeedAndAcceleration(ACTOR_SUSPECTED_THIEF, 1.5, 1.5);
	LogRoomsACorneredTicks = 0;
	sySetOrUpdateTickEvent(scLogRoomsAUpdateCorneringChase, EVENT_PASS_TICK + 20);
	// DEAD: the last system flag holds nothing, since no table, script or function sets or reads
	// it; the ROM keeps the call that clears it.
	fdScriptClearFlag(FLAG_SYSTEM_LAST);
	fdScriptEnd();
	sySoundPlay(MUS_TRY_YOUR_LUCK);
}

// clang-format off
// A link: the stop it leads to and the heading toward it, kept as the facing's high byte. An unused link is LOG_STOP_NONE with heading 255.
#define NO_CONNECTION {LOG_STOP_NONE, 255, {0, 0}}
#define CONNECTION(stop, direction) {stop, (direction) >> 8, {0, 0}}
// The route network's stops, each with its links to the others.
LOG_ROOMS_A_STOP LogRoomsAStops[LOG_STOP_COUNT] = {
	{4, 0, {0},
	 {CONNECTION(1, FACING_EAST), CONNECTION(30, FACING_WEST), NO_CONNECTION}},
	{6, 0, {0},
	 {CONNECTION(2, FACING_EAST), CONNECTION(0, FACING_WEST), NO_CONNECTION}},
	{8, 0, {0},
	 {CONNECTION(3, FACING_EAST), CONNECTION(1, FACING_WEST), CONNECTION(31, FACING_SOUTH)}},
	{10, 0, {0},
	 {CONNECTION(4, FACING_EAST), CONNECTION(2, FACING_WEST), NO_CONNECTION}},
	{12, 0, {0},
	 {CONNECTION(5, FACING_EAST), CONNECTION(3, FACING_WEST), NO_CONNECTION}},
	{14, 0, {0},
	 {CONNECTION(6, FACING_EAST), CONNECTION(4, FACING_WEST), NO_CONNECTION}},
	{16, 0, {0},
	 {CONNECTION(7, FACING_EAST), CONNECTION(5, FACING_WEST), CONNECTION(34, FACING_SOUTH)}},
	{18, 0, {0},
	 {CONNECTION(8, FACING_EAST), CONNECTION(6, FACING_WEST), NO_CONNECTION}},
	{20, 0, {0},
	 {CONNECTION(9, FACING_EAST), CONNECTION(7, FACING_WEST), NO_CONNECTION}},
	{22, 0, {0},
	 {CONNECTION(10, FACING_EAST), CONNECTION(8, FACING_WEST), NO_CONNECTION}},
	{24, 0, {0},
	 {CONNECTION(11, FACING_SOUTH), CONNECTION(9, FACING_WEST), NO_CONNECTION}},
	{24, 2, {0},
	 {CONNECTION(12, FACING_SOUTH), CONNECTION(10, FACING_NORTH), NO_CONNECTION}},
	{24, 4, {0},
	 {CONNECTION(13, FACING_SOUTH), CONNECTION(11, FACING_NORTH), NO_CONNECTION}},
	{24, 6, {0},
	 {CONNECTION(14, FACING_SOUTH), CONNECTION(12, FACING_NORTH), NO_CONNECTION}},
	{24, 8, {0},
	 {CONNECTION(15, FACING_WEST), CONNECTION(13, FACING_NORTH), NO_CONNECTION}},
	{22, 8, {0},
	 {CONNECTION(16, FACING_WEST), CONNECTION(14, FACING_EAST), NO_CONNECTION}},
	{20, 8, {0},
	 {CONNECTION(17, FACING_WEST), CONNECTION(15, FACING_EAST), NO_CONNECTION}},
	{18, 8, {0},
	 {CONNECTION(18, FACING_WEST), CONNECTION(16, FACING_EAST), NO_CONNECTION}},
	{16, 8, {0},
	 {CONNECTION(19, FACING_WEST), CONNECTION(17, FACING_EAST), CONNECTION(36, FACING_NORTH)}},
	{14, 8, {0},
	 {CONNECTION(20, FACING_WEST), CONNECTION(18, FACING_EAST), NO_CONNECTION}},
	{12, 8, {0},
	 {CONNECTION(21, FACING_WEST), CONNECTION(19, FACING_EAST), NO_CONNECTION}},
	{10, 8, {0},
	 {CONNECTION(22, FACING_WEST), CONNECTION(20, FACING_EAST), NO_CONNECTION}},
	{8, 8, {0},
	 {CONNECTION(23, FACING_WEST), CONNECTION(21, FACING_EAST), CONNECTION(33, FACING_NORTH)}},
	{6, 8, {0},
	 {CONNECTION(24, FACING_WEST), CONNECTION(22, FACING_EAST), NO_CONNECTION}},
	{4, 8, {0},
	 {CONNECTION(25, FACING_WEST), CONNECTION(23, FACING_EAST), NO_CONNECTION}},
	{2, 8, {0},
	 {CONNECTION(26, FACING_WEST), CONNECTION(24, FACING_EAST), NO_CONNECTION}},
	{0, 8, {0},
	 {CONNECTION(27, FACING_NORTH), CONNECTION(25, FACING_EAST), NO_CONNECTION}},
	{0, 6, {0},
	 {CONNECTION(28, FACING_NORTH), CONNECTION(26, FACING_SOUTH), NO_CONNECTION}},
	{0, 4, {0},
	 {CONNECTION(29, FACING_NORTH), CONNECTION(27, FACING_SOUTH), NO_CONNECTION}},
	{0, 2, {0},
	 {CONNECTION(30, FACING_EAST), CONNECTION(28, FACING_SOUTH), NO_CONNECTION}},
	{2, 1, {0},
	 {CONNECTION(0, FACING_EAST), CONNECTION(29, FACING_WEST), NO_CONNECTION}},
	{8, 2, {0},
	 {CONNECTION(32, FACING_SOUTH), CONNECTION(2, FACING_NORTH), NO_CONNECTION}},
	{8, 4, {0},
	 {CONNECTION(33, FACING_SOUTH), CONNECTION(31, FACING_NORTH), NO_CONNECTION}},
	{8, 6, {0},
	 {CONNECTION(22, FACING_SOUTH), CONNECTION(32, FACING_NORTH), NO_CONNECTION}},
	{16, 2, {0},
	 {CONNECTION(35, FACING_SOUTH), CONNECTION(6, FACING_NORTH), NO_CONNECTION}},
	{16, 4, {0},
	 {CONNECTION(36, FACING_SOUTH), CONNECTION(34, FACING_NORTH), NO_CONNECTION}},
	{16, 6, {0},
	 {CONNECTION(18, FACING_SOUTH), CONNECTION(35, FACING_NORTH), NO_CONNECTION}},
};

#undef CONNECTION
#undef NO_CONNECTION

// A beaten thief lying on the floor gasps twice, his sprite squashed and let go, then lies still for three seconds, over and over.
Sint32 LogRoomsABeatenThiefGasps[] = {
	SC_ADD(SW_SCALE_Y, (-toFIXED(1) / 16)),
	SC_ADD(SW_SCALE_X, (-toFIXED(1) / 256)),
	SC_WAIT(2),
	SC_ADD(SW_SCALE_Y, (-toFIXED(1) / 16)),
	SC_ADD(SW_SCALE_X, (toFIXED(1) / 256)),
	SC_WAIT(2),
	SC_SET(SW_SCALE_Y, toFIXED(1)),
	SC_SET(SW_SCALE_X, toFIXED(1)),
	SC_WAIT(4),
	SC_ADD(SW_SCALE_Y, (-toFIXED(1) / 16)),
	SC_ADD(SW_SCALE_X, (-toFIXED(1) / 256)),
	SC_WAIT(2),
	SC_ADD(SW_SCALE_Y, (-toFIXED(1) / 16)),
	SC_ADD(SW_SCALE_X, (toFIXED(1) / 256)),
	SC_WAIT(2),
	SC_SET(SW_SCALE_Y, toFIXED(1)),
	SC_SET(SW_SCALE_X, toFIXED(1)),
	SC_WAIT(180),
	SC_LOOP(SC_FOREVER, SC_TOP),
};

Sint32 LogRoomsAFirstThiefEntryScript[] = {
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE(776, 0, 408),
	SC_SYNC,
	SC_MOVE(776, 0, 400),
	SC_SYNC,
	SC_MOVE(800, 0, 400),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_SET(SW_FACING, FACING_SOUTH_SOUTHEAST),
	SC_WAIT(20),
	SC_END,
};

Sint32 LogRoomsASecondThiefEntryScript[] = {
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE(776, 0, 408),
	SC_SYNC,
	SC_MOVE(776, 0, 416),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_SET(SW_FACING, FACING_SOUTH_SOUTHEAST),
	SC_WAIT(20),
	SC_END,
};

// The loft's walks once the thieves are beaten: the freed villager, and the thieves bound, go down the way to the room with the crate and are gone; the strongman and the mayor go round to the second and the first chest and come back from them.
Sint32 LogRoomsAVillagerLeavesLoft[] = {
	SC_SPEED(0.8),
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE(776, 0, 408),
	SC_SYNC,
	SC_MOVE(744, 0, 408),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_POSITION(0, 0, 0),
	SC_END,
};

Sint32 LogRoomsATiedThiefLeavesLoft[] = {
	SC_SPEED(0.8),
	SC_ANIMATION(ANIM_THIEF_TIED_UP_WALK),
	SC_MOVE(776, 0, 408),
	SC_SYNC,
	SC_MOVE(744, 0, 408),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_POSITION(0, 0, 0),
	SC_END,
};

Sint32 LogRoomsAToSecondChest[] = {
	SC_SPEED(0.8),
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE(792, 0, 408),
	SC_SYNC,
	SC_MOVE(808, 0, 440),
	SC_SYNC,
	SC_MOVE(808, 0, 456),
	SC_SYNC,
	SC_MOVE(840, 0, 456),
	SC_SYNC,
	SC_MOVE(856, 0, 408),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_SET(SW_FACING, FACING_EAST),
	SC_WAIT(10),
	SC_END,
};

Sint32 LogRoomsAToFirstChest[] = {
	SC_SPEED(0.8),
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE(808, 0, 408),
	SC_SYNC,
	SC_MOVE(808, 0, 456),
	SC_SYNC,
	SC_MOVE(840, 0, 456),
	SC_SYNC,
	SC_MOVE(856, 0, 424),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_SET(SW_FACING, FACING_EAST),
	SC_WAIT(10),
	SC_END,
};

Sint32 LogRoomsABackFromChests[] = {
	SC_SPEED(0.8),
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE(840, 0, 456),
	SC_SYNC,
	SC_MOVE(808, 0, 456),
	SC_SYNC,
	SC_MOVE(808, 0, 408),
	SC_SYNC,
	SC_MOVE(776, 0, 408),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_END,
};

// The suspects' runs about the guests' room, named by the spot each ends at among the room's three beds: the head of each bed (z 632), its foot (z 696), beside the outer beds (x 72 and 264, z 648) and the room's two south corners. A Round run goes about the end of the outer bed, an Across run over the beds' row, an Along run past the feet of the beds. The suspects' timers number the spots (scLogRoomsASuspectsRunFromParty): the leader's 1 to 3 the heads and 4 to 6 the feet, west to east; the thief's 1 and 2 beside the west and east beds, 3 and 4 the south-west and south-east corners.
Sint32 LogRoomsASuspectToWestBedHead[] = {
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE(104, 0, 632),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_END,
};

Sint32 LogRoomsASuspectRoundToWestBedHead[] = {
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE(72, 0, 696),
	SC_SYNC,
	SC_MOVE(80, 0, 640),
	SC_SYNC,
	SC_MOVE(104, 0, 632),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_END,
};

Sint32 LogRoomsASuspectToMiddleBedHead[] = {
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE(168, 0, 632),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_END,
};

Sint32 LogRoomsASuspectToEastBedHead[] = {
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE(232, 0, 632),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_END,
};

Sint32 LogRoomsASuspectRoundToEastBedHead[] = {
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE(264, 0, 696),
	SC_SYNC,
	SC_MOVE(264, 0, 632),
	SC_SYNC,
	SC_MOVE(232, 0, 632),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_END,
};

Sint32 LogRoomsASuspectToWestBedFoot[] = {
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE(104, 0, 696),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_END,
};

Sint32 LogRoomsASuspectRoundToWestBedFoot[] = {
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE(80, 0, 640),
	SC_SYNC,
	SC_MOVE(72, 0, 696),
	SC_SYNC,
	SC_MOVE(104, 0, 696),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_END,
};

Sint32 LogRoomsASuspectToMiddleBedFoot[] = {
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE(168, 0, 696),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_END,
};

Sint32 LogRoomsASuspectToEastBedFoot[] = {
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE(232, 0, 696),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_END,
};

Sint32 LogRoomsASuspectRoundToEastBedFoot[] = {
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE(264, 0, 632),
	SC_SYNC,
	SC_MOVE(264, 0, 696),
	SC_SYNC,
	SC_MOVE(232, 0, 696),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_END,
};

Sint32 LogRoomsASuspectAcrossToWestOfBeds[] = {
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE(168, 0, 648),
	SC_SYNC,
	SC_MOVE(72, 0, 648),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_END,
};

Sint32 LogRoomsASuspectToWestOfBeds[] = {
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE(72, 0, 648),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_END,
};

Sint32 LogRoomsASuspectAcrossToEastOfBeds[] = {
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE(168, 0, 648),
	SC_SYNC,
	SC_MOVE(264, 0, 648),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_END,
};

Sint32 LogRoomsASuspectToEastOfBeds[] = {
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE(264, 0, 648),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_END,
};

Sint32 LogRoomsASuspectToSouthWestCorner[] = {
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE(72, 0, 696),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_END,
};

Sint32 LogRoomsASuspectAlongToSouthWestCorner[] = {
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE(120, 0, 696),
	SC_SYNC,
	SC_MOVE(72, 0, 696),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_END,
};

Sint32 LogRoomsASuspectToSouthEastCorner[] = {
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE(264, 0, 696),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_END,
};

Sint32 LogRoomsASuspectAlongToSouthEastCorner[] = {
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE(168, 0, 696),
	SC_SYNC,
	SC_MOVE(264, 0, 696),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_END,
};

SCENE_ENTRANCE LogRoomsAEntrances[] = {
	{ ENTRANCE_DEFAULT, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ LOG_ROOMS_A_FROM_VAULT_EAST_SOUTHEAST, CONDITION_ALWAYS, 192, 0, 220, FACING_NORTH, 0, 32, 0, 272, 240, 0 },
	{ LOG_ROOMS_A_FROM_VAULT_SOUTHEAST, CONDITION_ALWAYS, 432, 0, 254, FACING_NORTH, 0, 288, 0, 528, 272, 0 },
	{ LOG_ROOMS_A_FROM_VAULT_SOUTHWEST, CONDITION_ALWAYS, 720, 0, 782, FACING_NORTH, 0, 432, 576, 800, 800, 0 },
	{ LOG_ROOMS_A_FROM_VAULT_EAST, CONDITION_ALWAYS, 624, 0, 236, FACING_NORTH, 0, 560, 32, 848, 256, 0 },
	{ LOG_ROOMS_A_FROM_VAULT_NORTH_NORTHEAST, CONDITION_ALWAYS, 144, 0, 492, FACING_NORTH, 0, 16, 288, 256, 512, 0 },
	{ LOG_ROOMS_A_FROM_VAULT_NORTHWEST, CONDITION_ALWAYS, 416, 0, 526, FACING_NORTH, 0, 320, 288, 592, 544, 0 },
	{ LOG_ROOMS_A_INN_FROM_INN_UPSTAIRS, CONDITION_ALWAYS, 744, 0, 640, FACING_SOUTH, 0, 432, 576, 800, 800, 0 },
	{ LOG_ROOMS_A_INN_UPSTAIRS_FROM_INN, CONDITION_ALWAYS, 358, 0, 632, FACING_SOUTH, 0, 32, 576, 400, 776, 0 },
	{ LOG_ROOMS_A_FROM_VAULT_WEST, CONDITION_ALWAYS, 648, 0, 424, FACING_SOUTH, 0, 560, 320, 800, 480, 0 },
	{ LOG_ROOMS_A_CRATE_ROOM_FROM_LOFT, CONDITION_ALWAYS, 704, 0, 408, FACING_WEST, 0, 560, 320, 800, 480, 0 },
	{ LOG_ROOMS_A_LOFT_FROM_CRATE_ROOM, CONDITION_ALWAYS, 768, 0, 408, FACING_EAST, 0, 712, 288, 952, 496, 0 },
	{ LOG_ROOMS_A_LOFT_AFTER_LOSING_TO_THIEVES, CONDITION_ALWAYS, 792, 0, 432, FACING_NORTH, 0, 712, 288, 952, 496, 0 },
	{ LOG_ROOMS_A_LOFT_AFTER_THIEVES_DEFEATED, CONDITION_ALWAYS, 792, 0, 432, FACING_NORTH, 0, 712, 288, 952, 496, 0 },
	{ SCENE_TABLE_END },
};

SCENE_EXIT_REGION LogRoomsARegions[] = {
	{639, 33, 407, 657, 51, 425, CONDITION_ALWAYS, LOG_ROOMS_A_TO_VAULT_WEST},
	{SCENE_TABLE_END},
};

Uint32 LogRoomsAExits[] = {
	SCENE_EXITS(LogRoomsA),
	SCENE_EXIT(LOG_ROOMS_A_TO_VAULT_EAST_SOUTHEAST, Vault, VAULT_FROM_LOG_ROOMS_A_EAST_SOUTHEAST),
	SCENE_EXIT(LOG_ROOMS_A_TO_VAULT_SOUTHEAST, Vault, VAULT_FROM_LOG_ROOMS_A_SOUTHEAST),
	SCENE_EXIT(LOG_ROOMS_A_TO_VAULT_SOUTHWEST, Vault, VAULT_FROM_LOG_ROOMS_A_SOUTHWEST),
	SCENE_EXIT(LOG_ROOMS_A_TO_VAULT_EAST, Vault, VAULT_FROM_LOG_ROOMS_A_EAST),
	SCENE_EXIT(LOG_ROOMS_A_TO_VAULT_NORTH_NORTHEAST, Vault, VAULT_FROM_LOG_ROOMS_A_NORTH_NORTHEAST),
	SCENE_EXIT(LOG_ROOMS_A_TO_VAULT_NORTHWEST, Vault, VAULT_FROM_LOG_ROOMS_A_NORTHWEST),
	SCENE_EXIT(LOG_ROOMS_A_INN_UPSTAIRS_TO_INN, LogRoomsA, LOG_ROOMS_A_INN_FROM_INN_UPSTAIRS),
	SCENE_EXIT(LOG_ROOMS_A_INN_TO_INN_UPSTAIRS, LogRoomsA, LOG_ROOMS_A_INN_UPSTAIRS_FROM_INN),
	SCENE_EXIT(LOG_ROOMS_A_TO_VAULT_WEST, Vault, VAULT_FROM_LOG_ROOMS_A_WEST),
	SCENE_EXIT(LOG_ROOMS_A_LOFT_TO_CRATE_ROOM, LogRoomsA, LOG_ROOMS_A_CRATE_ROOM_FROM_LOFT),
	SCENE_EXIT(LOG_ROOMS_A_CRATE_ROOM_TO_LOFT, LogRoomsA, LOG_ROOMS_A_LOFT_FROM_CRATE_ROOM),
	SCENE_EXIT(LOG_ROOMS_A_TO_SANCTUM, Sanctum, SANCTUM_FROM_LOG_ROOMS_A),
	SCENE_EXITS_END,
};

ACTOR_PLACE LogRoomsAActorPlacements[] = {
	ACTOR_PLACEMENT(ACTOR_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		8, 0, 8, FACING_NORTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(ACTOR_IVAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		376, 0, 392, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
		160, 0, 136, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_IN_VIEW, MOVEMENT_TYPE_WANDER_NEAR,
		152, 0, 72, FACING_WEST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_BURLY_MAN_WITH_GREY_HAIR, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
		456, 0, 184, FACING_EAST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_BOY_WITH_FAIR_HAIR, CONDITION_IN_VIEW, MOVEMENT_TYPE_WANDER,
		368, 0, 120, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_OLD_MAN_WITH_CANE, CONDITION_IN_VIEW, MOVEMENT_TYPE_WANDER,
		744, 0, 200, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_OLD_WOMAN, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
		752, 0, 128, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_BOY_WITH_FAIR_HAIR, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
		496, 0, 416, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_GIRL_WITH_PIGTAILS, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
		512, 0, 416, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_OLD_MAN_WITH_WHITE_HAIR, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
		536, 0, 451, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_IN_GREEN_DRESS, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
		416, 0, 464, FACING_NORTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_IN_VIEW, MOVEMENT_TYPE_WANDER,
		536, 0, 672, FACING_NORTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_BALD_MAN_IN_RED, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
		200, 0, 392, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_IN_GREEN, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
		88, 0, 392, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_PONYTAIL, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
		632, 0, 136, FACING_SOUTH, TALK_FACING_PARTY_THEN_WATCH, 0),
	ACTOR_PLACEMENT(SPRITE_BURLY_MAN_WITH_GREY_HAIR, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
		664, 0, 632, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PINK_APRON, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
		696, 0, 696, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_MAN_WITH_LONG_DARK_HAIR, FLAG_VAULT_THIEVES_ATTACKED, MOVEMENT_TYPE_STAND,
		104, 0, 696, FACING_EAST, TALK_FACING_PARTY_THEN_WATCH, 0),
	ACTOR_PLACEMENT(SPRITE_PERSON_IN_PURPLE, FLAG_VAULT_THIEVES_ATTACKED, MOVEMENT_TYPE_STAND,
		200, 0, 680, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_CRATE, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
		692, 0, 411, FACING_SOUTH, TALK_FACING_KEEP, 0),
	{ SCENE_TABLE_END },
};

ACTOR_PLACE LogRoomsALoftActorPlacements[] = {
	ACTOR_PLACEMENT(ACTOR_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		8, 0, 8, FACING_NORTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(ACTOR_IVAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		376, 0, 392, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		160, 0, 136, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_OLD_MAN_WITH_WHITE_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		504, 0, 456, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_MAN_WITH_LONG_DARK_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		104, 0, 696, FACING_EAST, TALK_FACING_PARTY_THEN_WATCH, 0),
	ACTOR_PLACEMENT(SPRITE_PERSON_IN_PURPLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		200, 0, 680, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_PERSON_IN_PURPLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		8, 0, 8, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_STRONGMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		8, 0, 8, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_STRONGMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		8, 0, 8, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_CHEST, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
		872, 0, 424, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_CHEST, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
		872, 0, 408, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_CHEST, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
		872, 0, 376, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_URN, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
		8, 0, 8, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_SANCTUM_GOLD_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
		8, 0, 8, FACING_SOUTH, TALK_FACING_PARTY, 0),
	{ SCENE_TABLE_END },
};

CUE LogRoomsACues[] = {
	{ CUE_EXIT, 1, CONDITION_ALWAYS, LOG_ROOMS_A_TO_VAULT_EAST_SOUTHEAST },
	{ CUE_EXIT, 2, CONDITION_ALWAYS, LOG_ROOMS_A_TO_VAULT_SOUTHEAST },
	{ CUE_EXIT, 3, CONDITION_ALWAYS, LOG_ROOMS_A_TO_VAULT_SOUTHWEST },
	{ CUE_EXIT, 4, CONDITION_ALWAYS, LOG_ROOMS_A_TO_VAULT_EAST },
	{ CUE_EXIT, 5, CONDITION_ALWAYS, LOG_ROOMS_A_TO_VAULT_NORTH_NORTHEAST },
	{ CUE_EXIT, 6, CONDITION_ALWAYS, LOG_ROOMS_A_TO_VAULT_NORTHWEST },
	{ CUE_STEP, METATILE_BEHAVIOR_LOG_ROOMS_A_GUESTS_ROOM_STAIRS, CONDITION_ALWAYS, SCRIPT_REF(scLogRoomsALeaveGuestsRoom) },
	{ CUE_EXIT, 8, CONDITION_ALWAYS, LOG_ROOMS_A_INN_TO_INN_UPSTAIRS },
	{ CUE_STEP, METATILE_BEHAVIOR_LOG_ROOMS_A_GUESTS_ROOM, IF_FLAG_SET(FLAG_VAULT_IVAN_JOINED_ROD_SEARCH), SCRIPT_REF(scLogRoomsAApproachSuspects) },
	{ CUE_STEP, METATILE_BEHAVIOR_LOG_ROOMS_A_FIRST_SUSPECT_ROUTE, IF_FLAG_SET(FLAG_SUSPECTS_FLEEING), SCRIPT_REF(scLogRoomsASuspectsRunFromParty) },
	{ CUE_STEP, METATILE_BEHAVIOR_LOG_ROOMS_A_FIRST_SUSPECT_ROUTE + 1, IF_FLAG_SET(FLAG_SUSPECTS_FLEEING), SCRIPT_REF(scLogRoomsASuspectsRunFromParty) },
	{ CUE_STEP, METATILE_BEHAVIOR_LOG_ROOMS_A_FIRST_SUSPECT_ROUTE + 2, IF_FLAG_SET(FLAG_SUSPECTS_FLEEING), SCRIPT_REF(scLogRoomsASuspectsRunFromParty) },
	{ CUE_STEP, METATILE_BEHAVIOR_LOG_ROOMS_A_FIRST_SUSPECT_ROUTE + 3, IF_FLAG_SET(FLAG_SUSPECTS_FLEEING), SCRIPT_REF(scLogRoomsASuspectsRunFromParty) },
	{ CUE_STEP, METATILE_BEHAVIOR_LOG_ROOMS_A_FIRST_SUSPECT_ROUTE + 4, IF_FLAG_SET(FLAG_SUSPECTS_FLEEING), SCRIPT_REF(scLogRoomsASuspectsRunFromParty) },
	{ CUE_STEP, METATILE_BEHAVIOR_LOG_ROOMS_A_FIRST_SUSPECT_ROUTE + 5, IF_FLAG_SET(FLAG_SUSPECTS_FLEEING), SCRIPT_REF(scLogRoomsASuspectsRunFromParty) },
	{ CUE_STEP, METATILE_BEHAVIOR_LOG_ROOMS_A_FIRST_SUSPECT_ROUTE + 6, IF_FLAG_SET(FLAG_SUSPECTS_FLEEING), SCRIPT_REF(scLogRoomsASuspectsRunFromParty) },
	{ CUE_STEP, METATILE_BEHAVIOR_LOG_ROOMS_A_LAST_SUSPECT_ROUTE, IF_FLAG_SET(FLAG_SUSPECTS_FLEEING), SCRIPT_REF(scLogRoomsASuspectsRunFromParty) },
	{ CUE_STEP, METATILE_BEHAVIOR_LOG_ROOMS_A_GUESTS_ROOM_DOOR, IF_FLAG_SET(FLAG_SUSPECTS_FLEEING), SCRIPT_REF(scLogRoomsADiscussSuspiciousGuests) },
	{ CUE_STEP, METATILE_BEHAVIOR_LOG_ROOMS_A_CORNERING_START, IF_FLAG_SET(FLAG_VAULT_SUSPECTS_APPROACHED), SCRIPT_REF(scLogRoomsAStartCorneringSuspects) },
	{ CUE_RAISED, LOG_ROOMS_A_LEADER_CORNERED, CONDITION_ALWAYS, SCRIPT_REF(scLogRoomsAReadCorneredSuspectsMinds) },
	{ CUE_RAISED, LOG_ROOMS_A_THIEF_CORNERED, CONDITION_ALWAYS, SCRIPT_REF(scLogRoomsAReadCorneredSuspectsMinds) },
	{ CUE_RAISED, LOG_ROOMS_A_BOTH_CORNERED, CONDITION_ALWAYS, SCRIPT_REF(scLogRoomsAReadCorneredSuspectsMinds) },
	{ CUE_RAISED, FIELD_RAISED_CUE_MENUS, CONDITION_ALWAYS, SCRIPT_REF(scLogRoomsAGaretHasNoTimeForMenu) },
	{ CUE_STEP, METATILE_BEHAVIOR_LOG_ROOMS_A_LOFT_WAY_IN, CONDITION_ALWAYS, SCRIPT_REF(scLogRoomsAEnterLoft) },
	{ STEP_CUE_WALKING, METATILE_BEHAVIOR_LOG_ROOMS_A_ATTIC_FLOOR_HOLE, CONDITION_ALWAYS, SCRIPT_REF(scLogRoomsAJumpAcrossHoleInAtticFloor) },
	{ CUE_TALK, ACTOR_IVAN, IF_FLAG_SET(FLAG_VAULT_CORNERING_SUSPECTS), 0 },
	{ CUE_TALK, ACTOR_IVAN, CONDITION_ALWAYS, SCRIPT_REF(scLogRoomsAMeetIvan) },
	{ CUE_TALK, ACTOR_GEM_OWNER, CONDITION_ALWAYS, MSGNO(MsgVaultThisRockWasBlownHereAll) },
	{ CUE_TALK, ACTOR_GEM_OWNERS_WIFE, CONDITION_ALWAYS, SCRIPT_REF(scLogRoomsATalkAboutRocks) },
	{ CUE_TALK, ACTOR_VILLAGER_SUSPECTING_THE_NEIGHBOR, CONDITION_ALWAYS, MSGNO(MsgVaultMyNeighborCreepsMeOutHe) },
	{ CUE_TALK, ACTOR_BOY_ASKING_ABOUT_TRAVEL, CONDITION_ALWAYS, SCRIPT_REF(scLogRoomsABoyAsksAboutTravel) },
	{ CUE_TALK, ACTOR_GRANDPA, CONDITION_ALWAYS, SCRIPT_REF(scLogRoomsATalkAboutMissingThings) },
	{ CUE_TALK, ACTOR_GRANDPAS_WIFE, CONDITION_ALWAYS, MSGNO(MsgVaultEverSinceTheEruptionGrandpaKeeps) },
	{ CUE_TALK, ACTOR_BOY_WISHING_TO_BE_OLDER, FLAG_VAULT_IVAN_JOINED_ROD_SEARCH, MSGNO(MsgVaultICouldHelpIvanIfI) },
	{ CUE_TALK, ACTOR_BOY_WISHING_TO_BE_OLDER, CONDITION_ALWAYS, MSGNO(MsgVaultYoureHelpingIvanYouDontLook) },
	{ CUE_TALK, ACTOR_GIRL_TELLING_A_SECRET, FLAG_VAULT_IVAN_JOINED_ROD_SEARCH, SCRIPT_REF(scLogRoomsAOfferIvansSecret) },
	{ CUE_TALK, ACTOR_GIRL_TELLING_A_SECRET, CONDITION_ALWAYS, MSGNO(MsgVaultSinceYoureHelpingIvanIllTell) },
	{ CUE_TALK, ACTOR_MAYOR, CONDITION_ALWAYS, SCRIPT_REF(scLogRoomsAMayorAsksHelpForIvan) },
	{ CUE_TALK, ACTOR_MAYORS_WIFE, FLAG_VAULT_IVAN_JOINED_ROD_SEARCH, MSGNO(MsgVaultMyHusbandTheMayorCantBelieve) },
	{ CUE_TALK, ACTOR_MAYORS_WIFE, CONDITION_ALWAYS, MSGNO(MsgVaultReallyYoureGoingToHelpIvan) },
	{ CUE_TALK, ACTOR_GIRL_AT_THE_INN, CONDITION_ALWAYS, SCRIPT_REF(scLogRoomsAWarnAboutThreeStrangers) },
	{ CUE_TALK, ACTOR_WEAPON_MERCHANT, CONDITION_ALWAYS, SCRIPT_REF(scLogRoomsATalkToWeaponMerchant) },
	{ CUE_TALK, ACTOR_ARMOR_MERCHANT, CONDITION_ALWAYS, SCRIPT_REF(scLogRoomsATalkToArmorMerchant) },
	{ CUE_TALK, ACTOR_ITEM_MERCHANT, CONDITION_ALWAYS, SCRIPT_REF(scLogRoomsATalkToItemMerchant) },
	{ CUE_TALK, ACTOR_INN_STAFF, CONDITION_ALWAYS, MSGNO(MsgVaultTheGuestsUpstairsWerentInTheir) },
	{ CUE_TALK, ACTOR_INNKEEPER, CONDITION_ALWAYS, SCRIPT_REF(scLogRoomsATalkToInnkeeper) },
	{ CUE_TALK, ACTOR_SUSPECTED_LEADER, IF_FLAG_SET(FLAG_VAULT_CORNERING_SUSPECTS), 0 },
	{ CUE_TALK, ACTOR_SUSPECTED_THIEF, IF_FLAG_SET(FLAG_VAULT_CORNERING_SUSPECTS), 0 },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SUSPECTED_LEADER, IF_FLAG_SET(FLAG_VAULT_CORNERING_SUSPECTS), 0 },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SUSPECTED_THIEF, IF_FLAG_SET(FLAG_VAULT_CORNERING_SUSPECTS), 0 },
	{ CUE_TALK, ACTOR_SUSPECTED_LEADER, FLAG_VAULT_SUSPECTS_MIND_READ, MSGNO(MsgVaultThatKidIvanHeCreepsMe) },
	{ CUE_TALK, ACTOR_SUSPECTED_THIEF, FLAG_VAULT_SUSPECTS_MIND_READ, MSGNO(MsgVaultWeHaventDoneAnythingYoureA) },
	{ CUE_TALK, ACTOR_SUSPECTED_LEADER, CONDITION_ALWAYS, SCRIPT_REF(scLogRoomsASuspectedLeaderDodges) },
	{ CUE_TALK, ACTOR_SUSPECTED_THIEF, CONDITION_ALWAYS, SCRIPT_REF(scLogRoomsASuspectedThiefDodges) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GEM_OWNER, CONDITION_ALWAYS, MSGNO(MsgVaultTheOtherSparklingRocksAllCrumbled) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GEM_OWNERS_WIFE, CONDITION_ALWAYS, SCRIPT_REF(scLogRoomsAReadGemOwnersWifesMind) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_SUSPECTING_THE_NEIGHBOR, CONDITION_ALWAYS, MSGNO(MsgVaultHeyMaybeMyNeighborIsThe) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BOY_ASKING_ABOUT_TRAVEL, CONDITION_ALWAYS, SCRIPT_REF(scLogRoomsAReadBoysMind) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GRANDPA, CONDITION_ALWAYS, SCRIPT_REF(scLogRoomsAReadGrandpasMind) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GRANDPAS_WIFE, CONDITION_ALWAYS, MSGNO(MsgVaultYouNeverBoughtMeAnythingWorth) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BOY_WISHING_TO_BE_OLDER, CONDITION_ALWAYS, MSGNO(MsgVaultTheseGuysLookReliableIGuess) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GIRL_TELLING_A_SECRET, CONDITION_ALWAYS, MSGNO(MsgVaultIWonderWhatSortOfThing) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAYOR, CONDITION_ALWAYS, SCRIPT_REF(scLogRoomsAReadMayorMind) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAYORS_WIFE, CONDITION_ALWAYS, MSGNO(MsgVaultMaybeTheyllFindOurStolenFamily) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GIRL_AT_THE_INN, CONDITION_ALWAYS, SCRIPT_REF(scLogRoomsAReadInnGirlsMind) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WEAPON_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgVaultAllOurBestWeaponsWereStolen) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ARMOR_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgVaultIWonderIfTheyLeftUs) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ITEM_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgVaultHahhWereSureToSeeA) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INN_STAFF, CONDITION_ALWAYS, MSGNO(MsgVaultItWouldBeAwfulIfOur) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INNKEEPER, CONDITION_ALWAYS, MSGNO(MsgVaultThatVolcanoCostUsALot) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), CUE_SUBJECT_OWN_EFFECT | ACTOR_SUSPECTED_LEADER, CONDITION_ALWAYS, SCRIPT_REF(scLogRoomsASuspectedLeaderDodges) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), CUE_SUBJECT_OWN_EFFECT | ACTOR_SUSPECTED_THIEF, CONDITION_ALWAYS, SCRIPT_REF(scLogRoomsASuspectedThiefDodges) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_CRATE, FLAG_VAULT_CRATE_MOVED, SCRIPT_REF(scLogRoomsACheckCrateMoved) },
	{ EXAMINE_CUE(EXAMINE_SILENT), METATILE_BEHAVIOR_LOG_ROOMS_A_INN_BARREL, CONDITION_ALWAYS, SCRIPT_REF(scLogRoomsASearchEmptyBarrel) },
	{ EXAMINE_CUE(EXAMINE_JAR), METATILE_BEHAVIOR_LOG_ROOMS_A_JAR, FLAG_LOG_ROOMS_A_JAR_COINS, FIND_COINS(7) },
	{ EXAMINE_CUE(EXAMINE_BARREL), METATILE_BEHAVIOR_LOG_ROOMS_A_FIRST_BARREL, FLAG_LOG_ROOMS_A_BARREL_COINS, FIND_COINS(4) },
	{ EXAMINE_CUE(EXAMINE_BARREL), METATILE_BEHAVIOR_LOG_ROOMS_A_SECOND_BARREL, FLAG_LOG_ROOMS_A_BARREL_SLEEP_BOMB, FIND_ITEM(ITEM_SLEEP_BOMB) },
	{ EXAMINE_CUE(EXAMINE_WOODEN_BOX), METATILE_BEHAVIOR_LOG_ROOMS_A_WOODEN_BOX, FLAG_LOG_ROOMS_A_BOX_MINT, FIND_ITEM(ITEM_MINT) },
	{ EXAMINE_CUE(EXAMINE_BOOKCASE) | CUE_FLAG_FACING | FACING_NORTH, METATILE_BEHAVIOR_LOG_ROOMS_A_HISTORY_BOOKCASE, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgVaultTheresABookEntitledHistoryOf)) },
	{ EXAMINE_CUE(EXAMINE_BOOKCASE), METATILE_BEHAVIOR_LOG_ROOMS_A_GEMS_BOOKCASE, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgVaultTheresABookEntitledGemsOf)) },
	{ CUE_TABLE_END },
};

CUE LogRoomsALoftCues[] = {
	{ CUE_STEP, METATILE_BEHAVIOR_LOG_ROOMS_A_LOFT_WAY_OUT, CONDITION_ALWAYS, SCRIPT_REF(scLogRoomsALeaveLoft) },
	{ CUE_TALK, ACTOR_TIED_UP_VILLAGER, CONDITION_ALWAYS, SCRIPT_REF(scLogRoomsAFreeVillagerAndFightThieves) },
	{ CUE_TALK, ACTOR_FIRST_CHEST, CONDITION_ALWAYS, SCRIPT_REF(scLogRoomsASearchEmptyChest) },
	{ CUE_TALK, ACTOR_SECOND_CHEST, CONDITION_ALWAYS, SCRIPT_REF(scLogRoomsASearchEmptyChest) },
	{ CUE_TALK, ACTOR_THIRD_CHEST, CONDITION_ALWAYS, SCRIPT_REF(scLogRoomsASearchEmptyChest) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TIED_UP_VILLAGER, CONDITION_ALWAYS, MSGNO(MsgVaultAhhHelpMe) },
	{ CUE_TABLE_END },
};

CUE LogRoomsACuesAfterThieves[] = {
	{ EXAMINE_CUE(EXAMINE_SILENT), METATILE_BEHAVIOR_LOG_ROOMS_A_INN_BARREL, CONDITION_ALWAYS, SCRIPT_REF(scVaultFindBone) },
	{ CUE_EXIT, 1, CONDITION_ALWAYS, LOG_ROOMS_A_TO_VAULT_EAST_SOUTHEAST },
	{ CUE_EXIT, 2, CONDITION_ALWAYS, LOG_ROOMS_A_TO_VAULT_SOUTHEAST },
	{ CUE_EXIT, 3, CONDITION_ALWAYS, LOG_ROOMS_A_TO_VAULT_SOUTHWEST },
	{ CUE_EXIT, 4, CONDITION_ALWAYS, LOG_ROOMS_A_TO_VAULT_EAST },
	{ CUE_EXIT, 5, CONDITION_ALWAYS, LOG_ROOMS_A_TO_VAULT_NORTH_NORTHEAST },
	{ CUE_EXIT, 6, CONDITION_ALWAYS, LOG_ROOMS_A_TO_VAULT_NORTHWEST },
	{ CUE_EXIT, 7, CONDITION_ALWAYS, LOG_ROOMS_A_INN_UPSTAIRS_TO_INN },
	{ CUE_EXIT, 8, CONDITION_ALWAYS, LOG_ROOMS_A_INN_TO_INN_UPSTAIRS },
	{ CUE_STEP, METATILE_BEHAVIOR_LOG_ROOMS_A_LOFT_WAY_IN, CONDITION_ALWAYS, SCRIPT_REF(scLogRoomsAEnterLoft) },
	{ CUE_STEP, METATILE_BEHAVIOR_LOG_ROOMS_A_LOFT_WAY_OUT, CONDITION_ALWAYS, SCRIPT_REF(scLogRoomsALeaveLoft) },
	{ CUE_TALK, ACTOR_GEM_OWNER, CONDITION_ALWAYS, MSGNO(MsgVaultICanSleepWellNowThat) },
	{ CUE_TALK, ACTOR_GEM_OWNERS_WIFE, CONDITION_ALWAYS, SCRIPT_REF(scLogRoomsATalkAboutRocks) },
	{ CUE_TALK, ACTOR_VILLAGER_SUSPECTING_THE_NEIGHBOR, CONDITION_ALWAYS, SCRIPT_REF(scLogRoomsAAskWhetherThievesCameFromLunpa) },
	{ CUE_TALK, ACTOR_BOY_ASKING_ABOUT_TRAVEL, CONDITION_ALWAYS, SCRIPT_REF(scLogRoomsABoyWorriesAboutFather) },
	{ CUE_TALK, ACTOR_GRANDPA, CONDITION_ALWAYS, SCRIPT_REF(scLogRoomsATalkAboutMissingThings) },
	{ CUE_TALK, ACTOR_GRANDPAS_WIFE, CONDITION_ALWAYS, MSGNO(MsgVaultJustAsIThoughtNothingWas) },
	{ CUE_TALK, ACTOR_BOY_WISHING_TO_BE_OLDER, CONDITION_ALWAYS, SCRIPT_REF(scLogRoomsAAskAboutDefeatingThieves) },
	{ CUE_TALK, ACTOR_GIRL_TELLING_A_SECRET, CONDITION_ALWAYS, MSGNO(MsgVaultIvanLearnedALotByTalking) },
	{ CUE_TALK, ACTOR_MAYOR, CONDITION_ALWAYS, SCRIPT_REF(scLogRoomsAMayorGivesWaterOfLife) },
	{ CUE_TALK, ACTOR_MAYORS_WIFE, CONDITION_ALWAYS, MSGNO(MsgVaultYoureThePersonWhoGotOur) },
	{ CUE_TALK, ACTOR_GIRL_AT_THE_INN, FLAG_VAULT_LEADER_BESIDE_BONE, SCRIPT_REF(scLogRoomsAOfferBones) },
	{ CUE_TALK, ACTOR_WEAPON_MERCHANT, CONDITION_ALWAYS, SCRIPT_REF(scLogRoomsATalkToWeaponMerchant) },
	{ CUE_TALK, ACTOR_ARMOR_MERCHANT, CONDITION_ALWAYS, SCRIPT_REF(scLogRoomsATalkToArmorMerchant) },
	{ CUE_TALK, ACTOR_ITEM_MERCHANT, CONDITION_ALWAYS, SCRIPT_REF(scLogRoomsATalkToItemMerchant) },
	{ CUE_TALK, ACTOR_INN_STAFF, CONDITION_ALWAYS, MSGNO(MsgVaultImShockedThoseGuestsOfOurs) },
	{ CUE_TALK, ACTOR_INNKEEPER, CONDITION_ALWAYS, SCRIPT_REF(scLogRoomsATalkToInnkeeper) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GEM_OWNER, CONDITION_ALWAYS, MSGNO(MsgVaultEverybodysAfterMyRockTheyWant) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GEM_OWNERS_WIFE, CONDITION_ALWAYS, SCRIPT_REF(scLogRoomsAReadGemOwnersWifesMind) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_SUSPECTING_THE_NEIGHBOR, CONDITION_ALWAYS, MSGNO(MsgVaultTheReasonLunpaTurnedBadIs) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BOY_ASKING_ABOUT_TRAVEL, CONDITION_ALWAYS, SCRIPT_REF(scLogRoomsAReadBoysMind) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GRANDPA, CONDITION_ALWAYS, SCRIPT_REF(scLogRoomsAReadGrandpasMind) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GRANDPAS_WIFE, CONDITION_ALWAYS, MSGNO(MsgVaultIWishIHadSomethingWorth) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BOY_WISHING_TO_BE_OLDER, CONDITION_ALWAYS, MSGNO(MsgVaultWhenIGrowUpIWant) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GIRL_TELLING_A_SECRET, CONDITION_ALWAYS, MSGNO(MsgVaultIWishIHadWingsLike) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAYOR, CONDITION_ALWAYS, SCRIPT_REF(scLogRoomsAReadMayorMind) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAYORS_WIFE, CONDITION_ALWAYS, MSGNO(MsgVaultImSoRelievedNowThatWe) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GIRL_AT_THE_INN, CONDITION_ALWAYS, SCRIPT_REF(scLogRoomsAReadInnGirlsMind) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WEAPON_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgVaultMyWeaponsWereFoundInThe) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ARMOR_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgVaultItMustBeSnowingUpIn) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ITEM_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgVaultWhyAreThesePeopleSoAnxious) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INN_STAFF, CONDITION_ALWAYS, MSGNO(MsgVaultWhoCouldHaveKnownTheyWere) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INNKEEPER, CONDITION_ALWAYS, MSGNO(MsgVaultOfCourseIAlwaysSuspectedThey) },
	{ EXAMINE_CUE(EXAMINE_JAR), METATILE_BEHAVIOR_LOG_ROOMS_A_JAR, FLAG_LOG_ROOMS_A_JAR_COINS, FIND_COINS(7) },
	{ EXAMINE_CUE(EXAMINE_BARREL), METATILE_BEHAVIOR_LOG_ROOMS_A_FIRST_BARREL, FLAG_LOG_ROOMS_A_BARREL_COINS, FIND_COINS(4) },
	{ EXAMINE_CUE(EXAMINE_BARREL), METATILE_BEHAVIOR_LOG_ROOMS_A_SECOND_BARREL, FLAG_LOG_ROOMS_A_BARREL_SLEEP_BOMB, FIND_ITEM(ITEM_SLEEP_BOMB) },
	{ EXAMINE_CUE(EXAMINE_WOODEN_BOX), METATILE_BEHAVIOR_LOG_ROOMS_A_WOODEN_BOX, FLAG_LOG_ROOMS_A_BOX_MINT, FIND_ITEM(ITEM_MINT) },
	{ EXAMINE_CUE(EXAMINE_BOOKCASE) | CUE_FLAG_FACING | FACING_NORTH, METATILE_BEHAVIOR_LOG_ROOMS_A_HISTORY_BOOKCASE, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgVaultTheresABookEntitledHistoryOf)) },
	{ EXAMINE_CUE(EXAMINE_BOOKCASE), METATILE_BEHAVIOR_LOG_ROOMS_A_GEMS_BOOKCASE, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgVaultTheresABookEntitledGemsOf)) },
	{ CUE_TABLE_END },
};
// clang-format on

// The suspected leader's runs when the party talks to him, by his timer: on round the beds, from
// the heads' row eastward, or back the other way.
Sint32 *LogRoomsALeaderRunsOn[] = {
	LogRoomsASuspectToMiddleBedHead,
	LogRoomsASuspectToEastBedHead,
	LogRoomsASuspectRoundToEastBedFoot,
	LogRoomsASuspectToMiddleBedFoot,
	LogRoomsASuspectToWestBedFoot,
	LogRoomsASuspectRoundToWestBedHead,
};
Sint32 *LogRoomsALeaderRunsBack[] = {
	LogRoomsASuspectRoundToWestBedFoot,
	LogRoomsASuspectToWestBedHead,
	LogRoomsASuspectToMiddleBedHead,
	LogRoomsASuspectRoundToEastBedHead,
	LogRoomsASuspectToEastBedFoot,
	LogRoomsASuspectToMiddleBedFoot,
};

// The suspected thief's runs when the party talks to him, four to a row:
// scLogRoomsASuspectedThiefDodges picks the row by which way he is pushed and the run by his timer.
Sint32 *LogRoomsAThiefRuns[] = {
	LogRoomsASuspectAcrossToEastOfBeds,
	LogRoomsASuspectToSouthEastCorner,
	LogRoomsASuspectAlongToSouthWestCorner,
	LogRoomsASuspectToWestOfBeds,
	LogRoomsASuspectToSouthWestCorner,
	LogRoomsASuspectAcrossToWestOfBeds,
	LogRoomsASuspectToEastOfBeds,
	LogRoomsASuspectAlongToSouthEastCorner,
};

Uint16 LogRoomsACorneredTicks;
