/* Crossbone Isle, the island of puzzles off the coast that the party can reach
   from the World Map, and the cave beneath it that winds down nine floors to
   Deadbeard's Pirate Ship. Its area names are Crossbone Isle, Crossbone Isle
   Cave and the Pirate Ship. No one here speaks.

   In the order the party meets them:

   - Crossbone Isle (SceneId_CrossboneIsle), the island. The party lands at
	 the south (WORLD_MAP_TO_CROSSBONE_ISLE) and walks west to the foot of a
	 cliff, where Growth on three sprouts, one on each ledge, grows the vines
	 that climb it to the top. They stay grown only until the party leaves
	 the island. Catch takes a Nut from the tree by the waterfall. From the
	 top, slides lead back down: one to the chest with a coin, one to a log
	 that has to be pushed aside to reach the chest with the Mint, and one to
	 a clump of ivy at the foot of the cliff. Whirlwind blows that ivy away for
	 good and uncovers the mouth of the cave
	 (CROSSBONE_ISLE_TO_CROSSBONE_ISLE_CAVE_ENTRANCE). A second clump, on the
	 top ledge beside the third vine, hides nothing.

   - Crossbone Isle Cave (SceneId_CrossboneIsleCaveEntrance), the winding
	 path lit by torches that leads from the cave's mouth down to the first
	 floor.

   - Crossbone Isle Cave (SceneId_CrossboneIsleFirstFloor to
	 SceneId_CrossboneIsleNinthFloor), the nine floors, each a hall like the
	 last. The party comes in at the east end (FLOOR_FROM_PREVIOUS_ROOM) and
	 leaves down the stairs at the west end (FLOOR_TO_NEXT_ROOM). The middle
	 of the hall is two raised ledges with a gap between them, each with a
	 door in the north wall into the floor's side room, and the side room is
	 the way across. The first time the party walks into the east door
	 (scCrossboneIsleOpenEastDoor), the monsters behind it attack, a group of
	 ENCOUNTER_ZONE_CROSSBONE_ISLE_FIRST_GUARDS on the first four floors and
	 of ENCOUNTER_ZONE_CROSSBONE_ISLE_LAST_GUARDS on the next five. Back from
	 the battle the door opens and the party walks in, and from then on it
	 simply opens. On the west ledge, at the edge of the gap, stands a stone
	 pillar: pushed east, it drops into the gap and makes a step across it
	 for good, so the way back east no longer needs the side room. On the
	 first floor the chest in front of the west door is a Mimic.

	 scene_exits.h names each floor's doors floor by floor
	 (CROSSBONE_ISLE_FIRST_FLOOR_FROM_CROSSBONE_ISLE_CAVE_ENTRANCE and
	 CROSSBONE_ISLE_FIRST_FLOOR_TO_CROSSBONE_ISLE_SECOND_FLOOR on the first),
	 and the side rooms of the first eight floors are in
	 crossbone_isle_stepping_stones.c and crossbone_isle_keys.c.

   - Crossbone Isle Cave (SceneId_CrossboneIsleFloatingLogs), the ninth
	 floor's side room: two pools of green water with twelve logs floating on
	 them. Walking onto a log and on along it rolls the log across the pool
	 with Isaac on top, until the far shore or another log stops it, and he
   hops from log to log or ashore over a metatile of water. The logs lead to
   four chests: 999 coins on the island in the east pool, a Sleep Bomb in the
   alcove north of the east pool, a Water of Life on the ledge between the
   pools, and the Muramasa in the alcove north of the west pool.

   - Crossbone Isle Cave (SceneId_CrossboneIslePier), the ledge past the ninth
	 floor where a wooden pier leads out to the Pirate Ship.

   - Pirate Ship (SceneId_PirateShip), at the bottom. The first chest, at the
	 foot of the ladder in the middle of the deck, is a Mimic. Climbing the
	 west stairs onto the raised foredeck sets off the battle with Deadbeard
	 (scCrossboneIsleFightDeadbeard). Once he is beaten, a chest with the Demon
	 Mail stands on the foredeck behind where the battle began.

   scInit restores each scene as its flags say: the ivy and the vines on the
   island, each floor's pillar in its gap, the Mimics until they are beaten and
   the chest Deadbeard leaves. The floating logs start in their places on every
   visit, their metatiles laid under them (scCrossboneIslePlaceLogs). */

#include "ability_ids.h"
#include "flags.h"
#include "item_ids.h"
#include "battle/battle.h"
#include "debug/debug.h"
#include "field/script_inline.h"
#include "graphics/graphics.h"
#include "scene/floating_nut.h"
#include "scene/catch_nut.h"

extern Uint16		  CrossboneIsle_DoorOpens[];
extern SCENE_ENTRANCE CrossboneIsle_IsleEntrances[];
extern SCENE_ENTRANCE CrossboneIsle_CaveEntranceEntrances[];
extern SCENE_ENTRANCE CrossboneIsle_FloatingLogsEntrances[];
extern SCENE_ENTRANCE CrossboneIsle_PierEntrances[];
extern SCENE_ENTRANCE CrossboneIsle_PirateShipEntrances[];
extern Uint32		  CrossboneIsle_Exits[];
extern ACTOR_PLACE	  CrossboneIsle_NoActorPlacements[];
extern ACTOR_PLACE	  CrossboneIsle_IsleActorPlacements[];
extern ACTOR_PLACE	  CrossboneIsle_FloatingLogsActorPlacements[];
extern ACTOR_PLACE	  CrossboneIsle_FloorActorPlacements[];
extern CUE			  CrossboneIsle_FloorCues[];
extern CUE			  CrossboneIsle_CaveEntranceCues[];
extern CUE			  CrossboneIsle_IsleCues[];
extern CUE			  CrossboneIsle_FloatingLogsCues[];
extern CUE			  CrossboneIsle_PierCues[];
extern CUE			  CrossboneIsle_PirateShipCues[];

/* Crossbone Isle's placed actors, numbered in the order of
   CrossboneIsle_IsleActorPlacements. */
typedef enum CROSSBONE_ISLE_ACTOR_ID {
	/* The Nut up in the tree by the waterfall, until Catch takes it
	   (FLAG_CROSSBONE_ISLE_NUT_CAUGHT). */
	ACTOR_ISLE_NUT = ACTOR_FIRST_PLACED,
	/* The two logs lying on the grass below the cliffs, which Isaac pushes by
	   walking into them (scPushedActorPushIfClear): the west one at the foot of
	   the cliff the vines climb, the east one in front of the chest with the
	   Mint. */
	ACTOR_FIRST_ISLE_LOG,
	/* The four sprouts Growth grows into vines, one flag each from
	   FLAG_CROSSBONE_ISLE_FIRST_PLANT_GROWN. The second, third and fourth stand
	   one above the other on the west cliff: at its foot, on its first ledge
	   and on the ledge above. The first is placed at the map's origin, off
	   stage (scCrossboneIsleUnusedGrowFirstPlant). */
	ACTOR_FIRST_PLANT = ACTOR_FIRST_ISLE_LOG + 2,
	/* The clumps of ivy Whirlwind blows away, one flag each from
	   FLAG_CROSSBONE_ISLE_FIRST_IVY_BLOWN_AWAY: the first over the cave's mouth
	   at the foot of the cliff, the third on the top ledge beside the highest
	   vine. The second is placed at the map's origin, off stage
	   (scCrossboneIsleUnusedBlowAwaySecondIvy), and the fourth, which its cue
	   and scCrossboneIsleRestoreIvy name, is not placed at all. */
	ACTOR_FIRST_IVY = ACTOR_FIRST_PLANT + 4
} CROSSBONE_ISLE_ACTOR_ID;

/* Growth has grown Crossbone Isle's first sprout, until the party leaves the
   island. Four scene temp flags run from this one, one a sprout. */
enum { FLAG_CROSSBONE_ISLE_FIRST_PLANT_GROWN = FLAG_SCENE_TEMP_40 };

/* Crossbone Isle's metatiles beside its two logs, where walking into a log
   pushes it (scPushedActorPushIfClear). */
enum {
	/* Beside the east log, in front of the chest with the Mint: metatiles (23,
	   22), (20, 26), (21, 26) and (23, 26). */
	METATILE_BEHAVIOR_CROSSBONE_ISLE_BESIDE_EAST_LOG = 10,
	/* Beside the west log at the foot of the cliff: metatiles (8, 23) to (10,
	   23), (7, 26), (8, 26) and (10, 26). */
	METATILE_BEHAVIOR_CROSSBONE_ISLE_BESIDE_WEST_LOG = 20
};

/* The twelve floating logs of the ninth floor's side room
   (SceneId_CrossboneIsleFloatingLogs), lettered A to L in the order of
   CrossboneIsle_FloatingLogsActorPlacements. Each is given by the place it
   starts in on every visit, with its middle metatile.

   A log lying west-east rolls north and south, and one lying north-south rolls
   east and west. Where the ways of two logs cross, one rolled into the other's
   way stops it, so the order the logs are rolled in decides where each can
   go. */
typedef enum CROSSBONE_ISLE_FLOATING_LOGS_ACTOR_ID {
	/* The east pool's south-west log, lying west-east at (25, 19). It rolls
	   north up the pool's west side, below the alcove with the Sleep Bomb. */
	ACTOR_LOG_A = ACTOR_FIRST_PLACED,
	/* The east one of the west pool's three south logs, lying west-east at (10,
	   19). */
	ACTOR_LOG_B,
	/* The east pool's log south-east of the island, lying west-east at (32,
	   18). It rolls north up the middle of the pool's east half. */
	ACTOR_LOG_C,
	/* The east pool's south-east log, lying north-south along the east shore at
	   (36, 17). It rolls west to the island. */
	ACTOR_LOG_D,
	/* The east pool's middle log, lying north-south east of the island at (34,
	   12). It rolls west to the pool's west side. */
	ACTOR_LOG_E,
	/* The east pool's north-east log, lying north-south at (36, 8). It rolls
	   west along the pool's north edge. */
	ACTOR_LOG_F,
	/* The middle one of the west pool's three side logs, lying north-south
	   along its east shore at (13, 13). */
	ACTOR_LOG_G,
	/* The east pool's north-west log, lying north-south at (24, 8). It rolls
	   east along the pool's north edge. */
	ACTOR_LOG_H,
	/* The north one of the west pool's three side logs, lying north-south along
	   its east shore at (13, 10). */
	ACTOR_LOG_I,
	/* The west one of the west pool's three south logs, lying west-east at (4,
	   19), in the bay at the pool's south-west. */
	ACTOR_LOG_J,
	/* The middle one of the west pool's three south logs, lying west-east at
	   (7, 19). */
	ACTOR_LOG_K,
	/* The south one of the west pool's three side logs, lying north-south along
	   its east shore at (13, 16). */
	ACTOR_LOG_L
} CROSSBONE_ISLE_FLOATING_LOGS_ACTOR_ID;

/* The line each floating log rides along that another can lie across: the
   column of a west-east log's middle metatile, which rides north and south, and
   the row of a north-south log's, which rides east and west. */
enum {
	LOG_B_COLUMN = 10,
	LOG_C_COLUMN = 32,
	LOG_J_COLUMN = 4,
	LOG_K_COLUMN = 7,
	LOG_G_ROW = 13,
	LOG_I_ROW = 10
};

/* True when a log whose middle metatile is at `position` lies across `line`, in
   the way of the three-metatile log riding along it. */
#define LIES_ACROSS(position, line) \
	((position) >= (line) - 1 && (position) <= (line) + 1)

/* The metatile behaviour under each floating log: stepping on it sets the log
   bobbing, walking on along it rolls it, and walking off its side hops Isaac
   over a metatile of water (CrossboneIsle_FloatingLogsCues). The metatiles are
   stored in rows 30 to 32 of the room's map.txt and laid under the logs on
   every visit (scCrossboneIslePlaceLogs). */
enum {
	METATILE_BEHAVIOR_CROSSBONE_ISLE_LOG_A = 21,
	METATILE_BEHAVIOR_CROSSBONE_ISLE_LOG_C = 22,
	METATILE_BEHAVIOR_CROSSBONE_ISLE_LOG_D = 23,
	METATILE_BEHAVIOR_CROSSBONE_ISLE_LOG_E = 24,
	METATILE_BEHAVIOR_CROSSBONE_ISLE_LOG_F = 25,
	METATILE_BEHAVIOR_CROSSBONE_ISLE_LOG_H = 26,
	METATILE_BEHAVIOR_CROSSBONE_ISLE_LOG_J = 31,
	METATILE_BEHAVIOR_CROSSBONE_ISLE_LOG_K = 32,
	METATILE_BEHAVIOR_CROSSBONE_ISLE_LOG_B = 33,
	METATILE_BEHAVIOR_CROSSBONE_ISLE_LOG_L = 34,
	METATILE_BEHAVIOR_CROSSBONE_ISLE_LOG_G = 35,
	METATILE_BEHAVIOR_CROSSBONE_ISLE_LOG_I = 36
};

/* The floating-logs room's shore metatiles where walking off the edge hops
   Isaac over a metatile of water onto a log beyond
   (CrossboneIsle_FloatingLogsCues). */
enum {
	/* The east shores of both pools: metatiles (14, 8) to (14, 19) beside the
	   west pool's side logs, and (37, 7) to (37, 18) beside the east pool's. */
	METATILE_BEHAVIOR_CROSSBONE_ISLE_SHORE_HOP_WEST = 40,
	/* The west pool's west shore: the ledge at (5, 9) to (5, 14) and the bay's
	   shore at (2, 15) to (2, 19). */
	METATILE_BEHAVIOR_CROSSBONE_ISLE_SHORE_HOP_EAST = 41,
	/* The west pool's north shore, (6, 8) to (8, 8). */
	METATILE_BEHAVIOR_CROSSBONE_ISLE_SHORE_HOP_SOUTH = 42,
	/* The west pool's south shore, (3, 20) to (13, 20). */
	METATILE_BEHAVIOR_CROSSBONE_ISLE_SHORE_HOP_NORTH = 43
};

/* The metatile on a floor's west ledge just west of the stone pillar, (38, 10),
   from which walking east pushes it (scCrossboneIslePushPillar). */
enum { METATILE_BEHAVIOR_CROSSBONE_ISLE_WEST_OF_PILLAR = 20 };

/* The nine floors' one placed actor (CrossboneIsle_FloorActorPlacements). */
typedef enum CROSSBONE_ISLE_FLOOR_ACTOR_ID {
	/* The stone pillar on the west ledge, at the edge of the gap between the
	   ledges, that drops into the gap when pushed east. */
	ACTOR_PILLAR = ACTOR_FIRST_PLACED
} CROSSBONE_ISLE_FLOOR_ACTOR_ID;

/* The nine floors' entrances, as the code the floors share names them.

   Every floor numbers them alike. scene_exits.h names each floor's own
   (CROSSBONE_ISLE_FIRST_FLOOR_FROM_CROSSBONE_ISLE_CAVE_ENTRANCE to
   CROSSBONE_ISLE_FIRST_FLOOR_FROM_CROSSBONE_ISLE_STEPPING_STONES_WEST in the
   first). */
enum {
	/* At the east end, from the room before (the cave's mouth,
	   CrossboneIsleCaveEntrance, for the first). */
	FLOOR_FROM_PREVIOUS_ROOM = 1,
	/* At the west end, from the room after (CrossboneIslePier for the last). */
	FLOOR_FROM_NEXT_ROOM = 2,
	FLOOR_FROM_SIDE_ROOM_EAST = 3, /* out of the side room by the east door */
	FLOOR_FROM_SIDE_ROOM_WEST = 4  /* out of the side room by the west door */
};

/* The nine floors' exits, numbered as their entrances. */
enum {
	FLOOR_TO_PREVIOUS_ROOM = 1,
	FLOOR_TO_NEXT_ROOM = 2,
	/* the east door (scCrossboneIsleOpenEastDoor) */
	FLOOR_TO_SIDE_ROOM_EAST = 3,
	/* the west door (scCrossboneIsleOpenWestDoor) */
	FLOOR_TO_SIDE_ROOM_WEST = 4
};

/* The entrance a battle returns the party to.

   On a floor that is after the monsters behind its east door
   (scCrossboneIsleOpenEastDoor). On the Pirate Ship it is after Deadbeard
   (scCrossboneIsleFightDeadbeard). */
enum { ENTRANCE_AFTER_BATTLE = 5 };

/* The Pirate Ship's step metatiles. */
enum {
	/* The top of the west stairs up to the raised foredeck, metatile (11, 34),
	   where the battle with Deadbeard starts
	   (scCrossboneIsleFightDeadbeard). */
	METATILE_BEHAVIOR_PIRATE_SHIP_TOP_OF_WEST_STAIRS = 10,
	/* Metatile (15, 40), beside the Mimic's chest at the foot of the ladder,
	   from which walking south hops Isaac across the hole in the deck
	   (scCrossboneIsleHopSouth). scInit lays it from the stored metatile at (0,
	   2) only while the Mimic is unbeaten. */
	METATILE_BEHAVIOR_PIRATE_SHIP_BESIDE_MIMIC = 20
};

/* The treasure spots of the chests that come and go, which their examine cues
   answer to. */
enum {
	/* The first floor's chest, a Mimic until it is beaten. */
	TREASURE_SPOT_CROSSBONE_ISLE_FIRST_FLOOR_MIMIC = TREASURE_SPOT_FIRST_ID,
	/* The Pirate Ship's chest with the Demon Mail, there once Deadbeard is
	   beaten. */
	TREASURE_SPOT_PIRATE_SHIP_DEMON_MAIL = TREASURE_SPOT_FIRST_ID,
	/* The Pirate Ship's other chest, a Mimic until it is beaten. */
	TREASURE_SPOT_PIRATE_SHIP_MIMIC = TREASURE_SPOT_FIRST_ID + 1
};

/* The treasure spots of the island and the floating-logs room that stay, named
   for what each holds. */
enum {
	TREASURE_SPOT_CROSSBONE_ISLE_COIN = TREASURE_SPOT_FIRST_ID,
	TREASURE_SPOT_CROSSBONE_ISLE_MINT,
	TREASURE_SPOT_CROSSBONE_ISLE_FLOATING_LOGS_999_COINS =
		TREASURE_SPOT_FIRST_ID,
	TREASURE_SPOT_CROSSBONE_ISLE_FLOATING_LOGS_WATER_OF_LIFE,
	TREASURE_SPOT_CROSSBONE_ISLE_FLOATING_LOGS_MURAMASA,
	TREASURE_SPOT_CROSSBONE_ISLE_FLOATING_LOGS_SLEEP_BOMB
};

static void scCrossboneIsleRestorePillar(void);
static void scCrossboneIsleOpenEastDoorAfterBattle(void);
static void scCrossboneIsleRestoreIvy(void);
static void scCrossboneIslePlaceLogs(void);

#include "shared/pushed_actor.c"

#include "shared/field_effect_create.c"
#include "shared/field_effect_spawn.c"

/* Unused: an empty function nothing refers to. */
UNUSED static void scCrossboneIsleUnusedNoOperation(void)
{
}

const SCENE_ENTRANCE *scGetEntrances(void)
{
	Sint16 sceneId = MainRoutineStateWork.next;

	if(sceneId == SCENE_CROSSBONE_ISLE) {
		return CrossboneIsle_IsleEntrances;
	}
	if(sceneId == SCENE_CROSSBONE_ISLE_CAVE_ENTRANCE) {
		return CrossboneIsle_CaveEntranceEntrances;
	}
	if(sceneId == SCENE_CROSSBONE_ISLE_FLOATING_LOGS) {
		return CrossboneIsle_FloatingLogsEntrances;
	}
	if(sceneId == SCENE_CROSSBONE_ISLE_PIER) {
		return CrossboneIsle_PierEntrances;
	}
	if(sceneId == SCENE_PIRATE_SHIP) {
		return CrossboneIsle_PirateShipEntrances;
	}
	/* The nine floors share the tail of the Pirate Ship's table. */
	return &CrossboneIsle_PirateShipEntrances[2];
}

const SCENE_EXIT_REGION *scGetExitRegions(void)
{
	return NULL;
}

const Uint32 *scGetExits(void)
{
	return CrossboneIsle_Exits;
}

const ACTOR_PLACE *scGetActorPlacements(void)
{
	Sint16 sceneId = MainRoutineStateWork.next;

	if(sceneId == SCENE_CROSSBONE_ISLE) {
		return CrossboneIsle_IsleActorPlacements;
	}
	if(sceneId == SCENE_CROSSBONE_ISLE_FLOATING_LOGS) {
		return CrossboneIsle_FloatingLogsActorPlacements;
	}
	if(sceneId <= SCENE_CROSSBONE_ISLE_NINTH_FLOOR &&
		sceneId >= SCENE_CROSSBONE_ISLE_FIRST_FLOOR) {
		return CrossboneIsle_FloorActorPlacements;
	}
	return CrossboneIsle_NoActorPlacements;
}

#include "shared/field_effect_advance.c"

/* Puffs a ring of dust out from a floor's pillar as it lands in the gap. */
static void scCrossboneIsleSpawnPillarMovedBurst(Sint32 actorId)
{
	ACTOR		  *actor;
	VECTOR3		   velocity;
	EFFECT_OPTIONS options;
	Uint32		   angleStep;
	Sint32		   angle;
	Sint32		   velocityX;
	FIXED		   velocityZ;

	actor = fdActorGet(actorId);
	options.update = scFieldEffectAdvanceMotion;
	for(angleStep = 0; angleStep <= TRIG_TURN / FACING_STEP; angleStep += 2) {
		angle = angleStep * FACING_STEP;
		velocity.x = syCos(angle);
		velocity.y = 0;
		velocityZ = sySin(angle);
		velocityX = velocity.x;
		velocity.z = velocityZ;
		velocityX += syMathDivide(velocityX, 3);
		velocity.x = velocityX;
		fdEffectSpawn(actor->x.fixed, actor->y.fixed, actor->z.fixed, velocityX,
			velocity.y, velocityZ, EFFECT_USE_UPDATE | EFFECT_SCRIPT_MEDIUM,
			&options);
	}
}

/* Climbing the west stairs onto the Pirate Ship's raised foredeck, to metatile
   (11, 34), sets off the battle with Deadbeard.

   He is the sixth group of ENCOUNTER_ZONE_CROSSBONE_ISLE_LAST_GUARDS, and the
   battle brings the party back to ENTRANCE_AFTER_BATTLE on the foredeck, where
   scInit sets FLAG_CROSSBONE_ISLE_DEADBEARD_DEFEATED and puts the chest with
   the Demon Mail before them. The cue row stops answering once that flag is
   set. */
static void scCrossboneIsleFightDeadbeard(void)
{
	fdScriptStart();
	fdScriptSetAfterBattleDestination(
		MainRoutineStateWork.next, ENTRANCE_AFTER_BATTLE);
	SET_BATTLE_TYPE(BATTLE_TYPE_SCRIPTED);
	fdRequestBattleFromEncounterTable(
		ENCOUNTER_ZONE_CROSSBONE_ISLE_LAST_GUARDS, 5);
	fdScriptEnd();
}

const CUE *scGetCues(void)
{
	Sint16 sceneId = MainRoutineStateWork.next;

	if(sceneId == SCENE_CROSSBONE_ISLE) {
		return CrossboneIsle_IsleCues;
	}
	if(sceneId == SCENE_CROSSBONE_ISLE_CAVE_ENTRANCE) {
		return CrossboneIsle_CaveEntranceCues;
	}
	if(sceneId == SCENE_CROSSBONE_ISLE_FLOATING_LOGS) {
		return CrossboneIsle_FloatingLogsCues;
	}
	if(sceneId == SCENE_CROSSBONE_ISLE_PIER) {
		return CrossboneIsle_PierCues;
	}
	if(sceneId == SCENE_PIRATE_SHIP) {
		return CrossboneIsle_PirateShipCues;
	}
	return CrossboneIsle_FloorCues;
}

/* Catch on the Nut up in the tree by the waterfall on Crossbone Isle: the hand
   of Catch pulls it down to Isaac and "Isaac got a Nut."

   scCatchNut gives the Nut and sets FLAG_CROSSBONE_ISLE_NUT_CAUGHT, which keeps
   it from being placed again. */
static void scCrossboneIsleCatchNut(void)
{
	scCatchNut(ACTOR_ISLE_NUT, FLAG_CROSSBONE_ISLE_NUT_CAUGHT);
}

/* Restores each scene's puzzles from its flags. */
Sint32 scInit(void)
{
	ACTOR *actor;
	Sint32 sceneId;
	Sint32 current;
	Sint32 first;

	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
	/* The floating logs' room only places its logs. */
	sceneId = MainRoutineStateWork.next;
	if(sceneId == SCENE_CROSSBONE_ISLE_FLOATING_LOGS) {
		scCrossboneIslePlaceLogs();
		return 0;
	}
	if(sceneId == SCENE_PIRATE_SHIP) {
		if(gmFlagGet(FLAG_PIRATE_SHIP_MIMIC_BEATEN) == 0) {
			fdMapGridCopyMetatileAttributes(0, 3, 1, 1, 13, 40);
			fdMapGridCopyMetatileAttributes(0, 2, 1, 1, 15, 40);
			fdTreasureSpotSetPos(
				TREASURE_SPOT_PIRATE_SHIP_MIMIC, toFIXED(216), toFIXED(648));
		}
		if(MainRoutineStateWork.next == sceneId) {
			if(MainRoutineStateWork.entrance != ENTRANCE_AFTER_BATTLE) {
				if(gmFlagGet(FLAG_CROSSBONE_ISLE_DEADBEARD_DEFEATED) == 0) {
					return 0;
				}
			}
			gmFlagSet(FLAG_CROSSBONE_ISLE_DEADBEARD_DEFEATED);
			fdMapGridCopyMetatileAttributes(0, 1, 1, 1, 13, 30);
			fdTreasureSpotSetPos(TREASURE_SPOT_PIRATE_SHIP_DEMON_MAIL,
				toFIXED(216), toFIXED(488));
			return 0;
		}
	}
	current = MainRoutineStateWork.next;
	if(current == SCENE_CROSSBONE_ISLE) {
		scCrossboneIsleRestoreIvy();
		fdActorGet(ACTOR_ISLE_NUT)->tgtX = toFIXED(129);
		scPushedActorBlockFootprint(ACTOR_FIRST_ISLE_LOG);
		scPushedActorBlockFootprint(ACTOR_FIRST_ISLE_LOG + 1);
		if(gmFlagGet(FLAG_CROSSBONE_ISLE_FIRST_PLANT_GROWN) != 0) {
			actor = fdActorGet(ACTOR_FIRST_PLANT);
			if(actor != NULL) {
				actor->collisionFlg = 0;
				fdActorSelectAnimation(actor, SPROUT_ANIMATION_GROWN);
				fdActorSetSprFlags(actor, 0);
			}
			fdSetMetatileBehaviorAt(0, 304, 368, METATILE_BEHAVIOR_CLIMB);
		}
		if(gmFlagGet(FLAG_CROSSBONE_ISLE_FIRST_PLANT_GROWN + 1) != 0) {
			actor = fdActorGet(ACTOR_FIRST_PLANT + 1);
			if(actor != NULL) {
				actor->collisionFlg = 0;
				fdActorSelectAnimation(actor, SPROUT_ANIMATION_GROWN);
				fdActorSetSprFlags(actor, 0);
			}
			fdSetMetatileBehaviorAt(0, 80, 368, METATILE_BEHAVIOR_CLIMB);
		}
		if(gmFlagGet(FLAG_CROSSBONE_ISLE_FIRST_PLANT_GROWN + 2) != 0) {
			actor = fdActorGet(ACTOR_FIRST_PLANT + 2);
			if(actor != NULL) {
				actor->collisionFlg = 0;
				fdActorSelectAnimation(actor, SPROUT_ANIMATION_GROWN);
				fdActorSetSprFlags(actor, 0);
			}
			fdSetMetatileBehaviorAt(0, 96, 336, METATILE_BEHAVIOR_CLIMB);
		}
		if(gmFlagGet(FLAG_CROSSBONE_ISLE_FIRST_PLANT_GROWN + 3) != 0) {
			actor = fdActorGet(ACTOR_FIRST_PLANT + 3);
			if(actor != NULL) {
				actor->collisionFlg = 0;
				fdActorSelectAnimation(actor, SPROUT_ANIMATION_GROWN);
				fdActorSetSprFlags(actor, 0);
			}
			fdSetMetatileBehaviorAt(0, 144, 320, METATILE_BEHAVIOR_CLIMB);
			fdSetMetatileBehaviorAt(0, 752, 320, METATILE_BEHAVIOR_CLIMB);
		}
		if(gmFlagGet(FLAG_CROSSBONE_ISLE_NUT_CAUGHT) != 0) {
			return 0;
		}
		scFloatNut(ACTOR_ISLE_NUT);
		return 0;
	}
	/* The nine floors share their door battle, pillar and return entrance. */
	first = SCENE_CROSSBONE_ISLE_FIRST_FLOOR;
	if(current == first) {
		if(gmFlagGet(FLAG_CROSSBONE_ISLE_FIRST_FLOOR_MIMIC_BEATEN) == 0) {
			fdMapGridCopyMetatileAttributes(0, 0, 1, 1, 37, 10);
			fdTreasureSpotSetPos(TREASURE_SPOT_CROSSBONE_ISLE_FIRST_FLOOR_MIMIC,
				toFIXED(600), toFIXED(168));
		}
	}
	current = MainRoutineStateWork.next;
	if(current >= first) {
		if(current <= SCENE_CROSSBONE_ISLE_NINTH_FLOOR) {
			scCrossboneIsleRestorePillar();
			if(MainRoutineStateWork.entrance == ENTRANCE_AFTER_BATTLE) {
				scCrossboneIsleOpenEastDoorAfterBattle();
			}
		}
	}
	return 0;
}

/* On a floor of Crossbone Isle Cave whose pillar has already been pushed into
   the gap between the ledges, it stands there again, its top a step across
   (metatile (40, 10)). */
static void scCrossboneIsleRestorePillar(void)
{
	ACTOR *actor;
	Sint32 cleared;

	actor = fdActorGet(ACTOR_PILLAR);
	cleared = gmFlagGet((MainRoutineStateWork.next +
		(FLAG_CROSSBONE_ISLE_FIRST_PASSAGE_OPENED -
			SCENE_CROSSBONE_ISLE_FIRST_FLOOR)));
	if(cleared != 0) {
		fdActorSetPos(ACTOR_PILLAR, 650, 168);
		actor->y.fixed = -toFIXED(32);
		fdActorSetSprFlags(fdActorGet(ACTOR_PILLAR), 0);
		fdActorSetSprPriority(ACTOR_PILLAR, 3);
		actor->motionFlg = 0;
		actor->priFlg |= ACTOR_PRIORITY_UNDERFOOT;
		fdMapGridCopyMetatileAttributes(42, 10, 1, 1, 40, 10);
	} else {
		fdActorGet(ACTOR_PILLAR)->motionFlg = 0;
	}
}

/* Move on the stone pillar on a floor's west ledge, and every push of it: once
   it reaches the gap between the ledges (metatile (40, 10)), it drops into the
   gap in a ring of dust, and its top stays there as a step Isaac hops across.

   Anywhere else nothing more happens. The pillar's metatile takes the
   jump-across metatile at (42, 10) (METATILE_BEHAVIOR_JUMP_FORWARD), and one
   flag per floor from FLAG_CROSSBONE_ISLE_FIRST_PASSAGE_OPENED keeps it there.
   In every edition but the Japanese, a pillar already in the gap does not drop
   again. */
static void scCrossboneIsleMovePillar(void)
{
	ACTOR *pillar;
	Sint32 column;

	pillar = fdActorGet(ACTOR_PILLAR);
	column = FIELD_METATILE(pillar->x.fixed);
	if(column != 40) {
		return;
	}

#if CROSSBONE_ISLE_PILLAR_DROPS_ONCE
	if(gmFlagGet(MainRoutineStateWork.next +
		   (FLAG_CROSSBONE_ISLE_FIRST_PASSAGE_OPENED -
			   SCENE_CROSSBONE_ISLE_FIRST_FLOOR)) != 0) {
		return;
	}
#endif

	pillar->motionFlg = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;

	fdScriptWait(8);
	scCrossboneIsleSpawnPillarMovedBurst(ACTOR_PILLAR);
	sySndPlay(SE_WHOOSH);
	fdScriptWait(40);

	fdActorSetSprFlags(fdActorGet(ACTOR_PILLAR), 0);
	fdActorSetSprPriority(ACTOR_PILLAR, 3);

	pillar->motionFlg = 0;
	pillar->priFlg |= ACTOR_PRIORITY_UNDERFOOT;

	fdMapGridCopyMetatileAttributes(42, 10, 1, 1, column, 10);

	gmFlagSet(MainRoutineStateWork.next +
		(FLAG_CROSSBONE_ISLE_FIRST_PASSAGE_OPENED -
			SCENE_CROSSBONE_ISLE_FIRST_FLOOR));
}

/* Walking east into the stone pillar on a floor's west ledge, from metatile
   (38, 10), pushes it one metatile east, over the edge of the gap.

   scCrossboneIsleMovePillar then drops it into the gap. */
static void scCrossboneIslePushPillar(void)
{
	fdScriptStart();
	scPushedActorAdvancePair();
	fdScriptEnd();
	scCrossboneIsleMovePillar();
}

/* Walking north into the door on a floor's west ledge (metatile (37, 9)) opens
   it with a rumble, and the party goes through into the floor's side room. */
static void scCrossboneIsleOpenWestDoor(void)
{
	fdMapGridPlayMapEdit(CrossboneIsle_DoorOpens, 37, 7);
	sySndPlay(SE_STONE_RUMBLE);
	syImportScriptSetExitRequest(FLOOR_TO_SIDE_ROOM_WEST);
}

/* Walking north into the door on a floor's east ledge (metatile (44, 9)): the
   first time, the monsters behind it attack; after that it opens with a rumble
   and the party goes through into the floor's side room.

   The battle brings the party back to ENTRANCE_AFTER_BATTLE, in front of the
   door (scCrossboneIsleOpenEastDoorAfterBattle). */
static void scCrossboneIsleOpenEastDoor(void)
{
	if(gmFlagGet(MainRoutineStateWork.next +
		   (FLAG_CROSSBONE_ISLE_FIRST_EAST_DOOR_MONSTERS_DEFEATED -
			   SCENE_CROSSBONE_ISLE_FIRST_FLOOR)) == 0) {
		fdScriptStart();
		fdScriptSetAfterBattleDestination(
			MainRoutineStateWork.next, ENTRANCE_AFTER_BATTLE);
		SET_BATTLE_TYPE(BATTLE_TYPE_SCRIPTED);
		/* The first four floors use ENCOUNTER_ZONE_CROSSBONE_ISLE_FIRST_GUARDS;
		   the next five use ENCOUNTER_ZONE_CROSSBONE_ISLE_LAST_GUARDS. */
		switch(MainRoutineStateWork.next - SCENE_CROSSBONE_ISLE_FIRST_FLOOR) {
		case 0:
			fdRequestBattleFromEncounterTable(
				ENCOUNTER_ZONE_CROSSBONE_ISLE_FIRST_GUARDS, 0);
			break;
		case 1:
			fdRequestBattleFromEncounterTable(
				ENCOUNTER_ZONE_CROSSBONE_ISLE_FIRST_GUARDS, 1);
			break;
		case 2:
			fdRequestBattleFromEncounterTable(
				ENCOUNTER_ZONE_CROSSBONE_ISLE_FIRST_GUARDS, 2);
			break;
		case 3:
			fdRequestBattleFromEncounterTable(
				ENCOUNTER_ZONE_CROSSBONE_ISLE_FIRST_GUARDS, 3);
			break;
		case 4:
			fdRequestBattleFromEncounterTable(
				ENCOUNTER_ZONE_CROSSBONE_ISLE_LAST_GUARDS, 0);
			break;
		case 5:
			fdRequestBattleFromEncounterTable(
				ENCOUNTER_ZONE_CROSSBONE_ISLE_LAST_GUARDS, 1);
			break;
		case 6:
			fdRequestBattleFromEncounterTable(
				ENCOUNTER_ZONE_CROSSBONE_ISLE_LAST_GUARDS, 2);
			break;
		case 7:
			fdRequestBattleFromEncounterTable(
				ENCOUNTER_ZONE_CROSSBONE_ISLE_LAST_GUARDS, 3);
			break;
		case 8:
			fdRequestBattleFromEncounterTable(
				ENCOUNTER_ZONE_CROSSBONE_ISLE_LAST_GUARDS, 4);
			break;
		}
		fdScriptEnd();
	} else {
		fdMapGridPlayMapEdit(CrossboneIsle_DoorOpens, 44, 7);
		sySndPlay(SE_STONE_RUMBLE);
		syImportScriptSetExitRequest(FLOOR_TO_SIDE_ROOM_EAST);
	}
}

/* Back from the battle at a floor's east door, the door opens and Isaac walks
   in through it to the side room.

   It sets the floor's flag from
   FLAG_CROSSBONE_ISLE_FIRST_EAST_DOOR_MONSTERS_DEFEATED, so the monsters do not
   come again. */
static void scCrossboneIsleOpenEastDoorAfterBattle(void)
{
	fdScriptStart();
	fdActorSetSpeed(ACTOR_PARTY_LEADER, 0.4);
	fdScriptStartScreenTransition();
	fdScriptWaitTransitionFrames();
	gmFlagSet((MainRoutineStateWork.next +
		(FLAG_CROSSBONE_ISLE_FIRST_EAST_DOOR_MONSTERS_DEFEATED -
			SCENE_CROSSBONE_ISLE_FIRST_FLOOR)));
	fdScriptWait(30);
	fdMapGridAnimateMetatiles(CrossboneIsle_DoorOpens, 44, 7);
	fdActorCenterAndWalk(ACTOR_PARTY_LEADER, 3, -16);
	syImportScriptSetExitRequest(FLOOR_TO_SIDE_ROOM_EAST);
	fdScriptEnd();
}

/* Growth on one of Crossbone Isle's sprouts: it no longer stops the party, its
   vine stands grown, and the metatile at pixel (x, z) becomes the vine's foot,
   climbed with Up or Down held (METATILE_BEHAVIOR_CLIMB). */
static inline void scCrossboneIsleGrowPlant(
	Sint32 actorId, Sint32 plant, Sint32 x, Sint32 z)
{
	ACTOR *actor = fdActorGet(ACTOR_FIRST_PLANT + plant);

	if(actor != NULL) {
		actor->collisionFlg = 0;
	}
	fdActorSetSprFlags(fdActorGet(actorId), 0);
	fdMapGridSetMetatileBehaviorAt(
		0, toFIXED(x), toFIXED(z), METATILE_BEHAVIOR_CLIMB);
}

/* Unused: Growth on Crossbone Isle's first sprout would grow its vine and make
   metatile (19, 23) climbable.

   The sprout is placed at the map's origin, out of anyone's reach, so its cue
   never runs. scInit still restores its vine if its flag were set. */
static void scCrossboneIsleUnusedGrowFirstPlant(Sint32 actorId)
{
	scCrossboneIsleGrowPlant(actorId, 0, 304, 368);
	gmFlagSet(FLAG_CROSSBONE_ISLE_FIRST_PLANT_GROWN);
}

/* Growth on the sprout at the foot of Crossbone Isle's west cliff, metatile (5,
   23), grows a vine up the cliff face to its first ledge.

   The vine lasts until the party leaves the island. */
static void scCrossboneIsleGrowSecondPlant(Sint32 actorId)
{
	scCrossboneIsleGrowPlant(actorId, 1, 80, 368);
	gmFlagSet(FLAG_CROSSBONE_ISLE_FIRST_PLANT_GROWN + 1);
}

/* Growth on the sprout on the first ledge of Crossbone Isle's west cliff,
   metatile (6, 21), grows a vine from there up to the next ledge.

   The vine lasts until the party leaves the island. */
static void scCrossboneIsleGrowThirdPlant(Sint32 actorId)
{
	scCrossboneIsleGrowPlant(actorId, 2, 96, 336);
	gmFlagSet(FLAG_CROSSBONE_ISLE_FIRST_PLANT_GROWN + 2);
}

/* Growth on the sprout on the upper ledge of Crossbone Isle's west cliff,
   metatile (9, 20), grows the last vine, up to the top of the cliff.

   The vine's foot is made climbable twice: at (9, 20), and at (47, 20), the
   same place in the copy of the cliff's top drawn over it, 38 columns over in
   map.txt. The vine lasts until the party leaves the island. */
static void scCrossboneIsleGrowFourthPlant(Sint32 actorId)
{
	FIXED eastX = toFIXED(752);
	FIXED eastZ = toFIXED(320);

	scCrossboneIsleGrowPlant(actorId, 3, 144, 320);
	fdMapGridSetMetatileBehaviorAt(0, eastX, eastZ, METATILE_BEHAVIOR_CLIMB);
	gmFlagSet(FLAG_CROSSBONE_ISLE_FIRST_PLANT_GROWN + 3);
}

/* Whirlwind on the clump of ivy at the foot of Crossbone Isle's west cliff
   blows it away for good, uncovering the mouth of the cave behind it at
   metatile (8, 21).

   The mouth's metatile takes the stored metatile at (0, 0), the exit into
   CrossboneIsleCaveEntrance, and FLAG_CROSSBONE_ISLE_FIRST_IVY_BLOWN_AWAY keeps
   it open (scCrossboneIsleRestoreIvy). */
static void scCrossboneIsleBlowAwayFirstIvy(void)
{
	gmFlagSet(FLAG_CROSSBONE_ISLE_FIRST_IVY_BLOWN_AWAY);
	fdMapGridCopyMetatileAttributes(0, 0, 1, 1, 8, 21);
}

/* Unused: Whirlwind on Crossbone Isle's second clump of ivy would blow it away
   for good.

   The ivy is placed at the map's origin, out of anyone's reach, so its cue
   never runs. */
static void scCrossboneIsleUnusedBlowAwaySecondIvy(void)
{
	gmFlagSet(FLAG_CROSSBONE_ISLE_FIRST_IVY_BLOWN_AWAY + 1);
}

/* Whirlwind on the clump of ivy on the top ledge of Crossbone Isle's west
   cliff, beside the last vine, blows it away for good; nothing is hidden behind
   it. */
static void scCrossboneIsleBlowAwayThirdIvy(void)
{
	gmFlagSet(FLAG_CROSSBONE_ISLE_FIRST_IVY_BLOWN_AWAY + 2);
}

/* Unused: Whirlwind on a fourth clump of ivy on Crossbone Isle would blow it
   away for good.

   No fourth clump is placed, so its cue never runs. */
static void scCrossboneIsleUnusedBlowAwayFourthIvy(void)
{
	gmFlagSet(FLAG_CROSSBONE_ISLE_FIRST_IVY_BLOWN_AWAY + 3);
}

/* On Crossbone Isle, ivy Whirlwind has blown away stays gone, and the cave
   mouth behind the first stays open; the others are stretched to stand tall. */
static void scCrossboneIsleRestoreIvy(void)
{
	ACTOR *actor;

	/* The cave mouth scCrossboneIsleBlowAwayFirstIvy opens, opened again on
	   arrival. */
	if(gmFlagGet(FLAG_CROSSBONE_ISLE_FIRST_IVY_BLOWN_AWAY) != 0) {
		fdMapGridCopyMetatileAttributes(0, 0, 1, 1, 8, 21);
		fdActorSetPos(ACTOR_FIRST_IVY, 968, 968);
	} else {
		fdActorGet(ACTOR_FIRST_IVY)->scaleY = toFIXED(8) / 5;
	}
	if(gmFlagGet(FLAG_CROSSBONE_ISLE_FIRST_IVY_BLOWN_AWAY + 1) != 0) {
		fdActorSetPos(ACTOR_FIRST_IVY + 1, 968, 968);
	} else {
		fdActorGet(ACTOR_FIRST_IVY + 1)->scaleY = toFIXED(8) / 5;
	}
	if(gmFlagGet(FLAG_CROSSBONE_ISLE_FIRST_IVY_BLOWN_AWAY + 2) != 0) {
		fdActorSetPos(ACTOR_FIRST_IVY + 2, 968, 968);
	} else {
		fdActorGet(ACTOR_FIRST_IVY + 2)->scaleY = toFIXED(8) / 5;
	}
	if(gmFlagGet(FLAG_CROSSBONE_ISLE_FIRST_IVY_BLOWN_AWAY + 3) != 0) {
		fdActorSetPos(ACTOR_FIRST_IVY + 3, 968, 968);
	} else {
		actor = fdActorGet(ACTOR_FIRST_IVY + 3);
		/* BUG: no fourth ivy is placed, so its slot is empty (fdActorClearAll)
		   and this writes to address 0x18, in the BIOS, where the write is
		   lost. */
#if FIX_MISSING_IVY_SCALE
		if(actor != NULL)
#endif
			actor->scaleY = toFIXED(8) / 5;
	}
}

#include "shared/pushed_actor_push.c"

/* Rolls log actorId one leg, dx and dz pixels, with Isaac walking backwards on
   top of it to the metatile it stops at. The log rolls with a sound and stops
   with another. */
static void scCrossboneIsleRideLog(Sint32 actorId, Sint32 dx, Sint32 dz)
{
	ACTOR *leader;
	ACTOR *log;
	FIXED  x;
	FIXED  z;

	leader = fdActorGet(MainRoutineStateWork.leaderActorId);
	log = fdActorGet(actorId);
	fdScriptStart();
	x = ((leader->x.fixed + toFIXED(dx)) & FIELD_METATILE_MASK) + toFIXED(8);
	z = ((leader->z.fixed + toFIXED(dz)) & FIELD_METATILE_MASK) + toFIXED(8);

	leader->speedLimit = toFIXED(1);
	leader->acceleration = toFIXED(1) / 2;
	syFieldActorSetMoveTargetFX(leader, x, leader->y.fixed, z);
	fdActorSelectAnimation(leader, ANIM_LEADER_WALK_BACKWARDS);
	x = ((log->x.fixed + toFIXED(dx)) & FIELD_METATILE_MASK) + toFIXED(8);
	z = ((log->z.fixed + toFIXED(dz)) & FIELD_METATILE_MASK) + toFIXED(8);

	log->speedLimit = toFIXED(1);
	log->acceleration = toFIXED(1) / 2;
	syFieldActorSetMoveTargetFX(log, x, log->y.fixed, z);
	if(dx < 0 || dz < 0) {
		fdActorSelectAnimation(log, FLOATING_LOG_ANIMATION_ROLL_WEST_OR_NORTH);
	} else {
		fdActorSelectAnimation(log, FLOATING_LOG_ANIMATION_ROLL_EAST_OR_SOUTH);
	}
	sySndPlay(SE_LOG_PUSH);
	fdActorWaitForMove(leader);
	sySndPlay(SE_STOP_ACTION_SE);
	fdScriptEnd();
}

/* Lays the metatiles of a log lying west-east under it, so that it can be
   walked on: the three metatiles it covers take the stored metatiles at
   (sourceColumn, sourceRow). Its metatile behaviour is passed and not used. */
static void scCrossboneIslePlaceWestEastLog(Sint32 actorId, Sint32 sourceColumn,
	Sint32 sourceRow, Sint32 metatileBehavior)
{
	ACTOR *actor = fdActorGet(actorId);

	if(actor != NULL) {

		fdActorSetSprPriority(actorId, 3);
		actor->priFlg |= ACTOR_PRIORITY_UNDERFOOT;
		{
			Sint32 row = FIELD_METATILE(actor->z.fixed);
			Sint32 column = FIELD_METATILE(actor->x.fixed) - 1;

			fdMapGridCopyMetatileAttributes(
				sourceColumn, sourceRow, 3, 1, column, row);
		}
	}
}

/* As scCrossboneIslePlaceWestEastLog, for a log lying north-south. */
static void scCrossboneIslePlaceNorthSouthLog(Sint32 actorId,
	Sint32 sourceColumn, Sint32 sourceRow, Sint32 metatileBehavior)
{
	ACTOR *actor = fdActorGet(actorId);

	if(actor != NULL) {

		fdActorSetSprPriority(actorId, 3);
		actor->priFlg |= ACTOR_PRIORITY_UNDERFOOT;
		{
			Sint32 column = FIELD_METATILE(actor->x.fixed);
			Sint32 row = FIELD_METATILE(actor->z.fixed) - 1;

			fdMapGridCopyMetatileAttributes(
				sourceColumn, sourceRow, 1, 3, column, row);
		}
	}
}

/* Starts a floating log bobbing under Isaac's step, from the beginning. */
static void scCrossboneIsleSettleLog(int actorId)
{
	fdActorSetAnimation(actorId, FLOATING_LOG_ANIMATION_FLOAT);
	fdActorSetAnimation(actorId, FLOATING_LOG_ANIMATION_SETTLE);
}

/* Lays the metatiles of the floating-logs room's twelve logs under them, in
   their starting places. */
static void scCrossboneIslePlaceLogs(void)
{
	scCrossboneIslePlaceWestEastLog(
		ACTOR_LOG_A, 17, 30, METATILE_BEHAVIOR_CROSSBONE_ISLE_LOG_A);
	scCrossboneIslePlaceWestEastLog(
		ACTOR_LOG_C, 17, 31, METATILE_BEHAVIOR_CROSSBONE_ISLE_LOG_C);
	scCrossboneIslePlaceNorthSouthLog(
		ACTOR_LOG_D, 20, 30, METATILE_BEHAVIOR_CROSSBONE_ISLE_LOG_D);
	scCrossboneIslePlaceNorthSouthLog(
		ACTOR_LOG_E, 21, 30, METATILE_BEHAVIOR_CROSSBONE_ISLE_LOG_E);
	scCrossboneIslePlaceNorthSouthLog(
		ACTOR_LOG_F, 22, 30, METATILE_BEHAVIOR_CROSSBONE_ISLE_LOG_F);
	scCrossboneIslePlaceNorthSouthLog(
		ACTOR_LOG_H, 23, 30, METATILE_BEHAVIOR_CROSSBONE_ISLE_LOG_H);
	scCrossboneIslePlaceWestEastLog(
		ACTOR_LOG_J, 0, 30, METATILE_BEHAVIOR_CROSSBONE_ISLE_LOG_J);
	scCrossboneIslePlaceWestEastLog(
		ACTOR_LOG_K, 0, 31, METATILE_BEHAVIOR_CROSSBONE_ISLE_LOG_K);
	scCrossboneIslePlaceWestEastLog(
		ACTOR_LOG_B, 0, 32, METATILE_BEHAVIOR_CROSSBONE_ISLE_LOG_B);
	scCrossboneIslePlaceNorthSouthLog(
		ACTOR_LOG_L, 4, 30, METATILE_BEHAVIOR_CROSSBONE_ISLE_LOG_L);
	scCrossboneIslePlaceNorthSouthLog(
		ACTOR_LOG_G, 5, 30, METATILE_BEHAVIOR_CROSSBONE_ISLE_LOG_G);
	scCrossboneIslePlaceNorthSouthLog(
		ACTOR_LOG_I, 6, 30, METATILE_BEHAVIOR_CROSSBONE_ISLE_LOG_I);
}

/* Stepping onto the east pool's south-west log sets it bobbing in the water
   under Isaac. */
static void scCrossboneIsleStepOnLogA(void)
{
	scCrossboneIsleSettleLog(ACTOR_LOG_A);
}

/* Stepping onto the east pool's log south-east of the island sets it bobbing in
   the water under Isaac. */
static void scCrossboneIsleStepOnLogC(void)
{
	scCrossboneIsleSettleLog(ACTOR_LOG_C);
}

/* Stepping onto the east pool's south-east log, by the east shore, sets it
   bobbing in the water under Isaac. */
static void scCrossboneIsleStepOnLogD(void)
{
	scCrossboneIsleSettleLog(ACTOR_LOG_D);
}

/* Stepping onto the east pool's middle log sets it bobbing in the water under
   Isaac. */
static void scCrossboneIsleStepOnLogE(void)
{
	scCrossboneIsleSettleLog(ACTOR_LOG_E);
}

/* Stepping onto the east pool's north-east log sets it bobbing in the water
   under Isaac. */
static void scCrossboneIsleStepOnLogF(void)
{
	scCrossboneIsleSettleLog(ACTOR_LOG_F);
}

/* Stepping onto the east pool's north-west log sets it bobbing in the water
   under Isaac. */
static void scCrossboneIsleStepOnLogH(void)
{
	scCrossboneIsleSettleLog(ACTOR_LOG_H);
}

/* Stepping onto the west one of the west pool's three south logs sets it
   bobbing in the water under Isaac. */
static void scCrossboneIsleStepOnLogJ(void)
{
	scCrossboneIsleSettleLog(ACTOR_LOG_J);
}

/* Stepping onto the middle one of the west pool's three south logs sets it
   bobbing in the water under Isaac. */
static void scCrossboneIsleStepOnLogK(void)
{
	scCrossboneIsleSettleLog(ACTOR_LOG_K);
}

/* Stepping onto the east one of the west pool's three south logs sets it
   bobbing in the water under Isaac. */
static void scCrossboneIsleStepOnLogB(void)
{
	scCrossboneIsleSettleLog(ACTOR_LOG_B);
}

/* Stepping onto the south one of the west pool's three side logs sets it
   bobbing in the water under Isaac. */
static void scCrossboneIsleStepOnLogL(void)
{
	scCrossboneIsleSettleLog(ACTOR_LOG_L);
}

/* Stepping onto the middle one of the west pool's three side logs sets it
   bobbing in the water under Isaac. */
static void scCrossboneIsleStepOnLogG(void)
{
	scCrossboneIsleSettleLog(ACTOR_LOG_G);
}

/* Stepping onto the north one of the west pool's three side logs sets it
   bobbing in the water under Isaac. */
static void scCrossboneIsleStepOnLogI(void)
{
	scCrossboneIsleSettleLog(ACTOR_LOG_I);
}

/* Isaac springs and leaps two metatiles the way he faces, over a metatile of
   water from log to log or between a log and the shore, if he can land there,
   and it returns 1 if he hopped. The Japanese edition aims at `requested`, the
   others at the middle of the metatile 32 pixels ahead. */
static Bool scCrossboneIsleTryHop(ACTOR *actor, FIELD_POS *requested)
{
	Sint32 savedMotionFlags = actor->motionFlg;
#if JUMP_LANDS_ON_CELL_CENTER
	FIELD_POS destination;

	destination.x = (actor->x.fixed & FIELD_METATILE_MASK) + toFIXED(8);
	destination.y = actor->y.fixed;
	destination.z = (actor->z.fixed & FIELD_METATILE_MASK) + toFIXED(8);
	{
		Sint32 direction = FACING_NEAREST_CARDINAL(actor->facing);

		syVectorAddPolarOffset(toFIXED(32), direction, (FIXED *)&destination);
	}
	if(syFieldActorCheckMovementCollision(actor, &destination) == 0) {
#else
	if(syFieldActorCheckMovementCollision(actor, requested) == 0) {
#endif
		fdScriptStart();
		fdActorSelectAnimation(actor, ANIM_LEADER_SPRING);
		sySynch(6);
		sySndPlay(SE_JUMP);
		fdActorSelectAnimation(actor, ANIM_LEADER_LEAP);
		actor->speedLimit = toFIXED(3);
		actor->acceleration = toFIXED(2);
		actor->velY = toFIXED(4);
		actor->motionFlg &= ACTOR_JUMP_KEEPS_MOTION;
		fdActorSetSprFlags(actor, 0);
#if JUMP_LANDS_ON_CELL_CENTER
		fdActorMoveToAndWait(ACTOR_PARTY_LEADER, FIXEDtoINT(destination.x),
			FIXEDtoINT(destination.z));
#else
		{
			Sint16 *pixelCoordinates = (Sint16 *)requested;

			fdActorMoveToAndWait(
				ACTOR_PARTY_LEADER, pixelCoordinates[1], pixelCoordinates[5]);
		}
#endif
		fdActorSelectAnimation(actor, ANIM_LEADER_SPRING);
		fdActorSetSprFlags(actor, SPRITE_FLAG_SHADOW);
		actor->motionFlg = savedMotionFlags;
		fdScriptEnd();
		return TRUE;
	}
	return FALSE;
}

/* Walking north off the side of a floating log, or off the west pool's south
   shore, with a metatile of water ahead, hops Isaac over it onto the log or
   shore beyond, if there is one to land on. */
static Sint32 scCrossboneIsleHopNorth(void)
{
	ACTOR	 *actor = fdActorGet(ACTOR_PARTY_LEADER);
	FIELD_POS destination;

	destination.x = actor->x.fixed;
	destination.y = actor->y.fixed;
	destination.z = actor->z.fixed + -toFIXED(32);
	return scCrossboneIsleTryHop(actor, &destination);
}

/* Walking south off the side of a floating log, or off the west pool's north
   shore, with a metatile of water ahead, hops Isaac over it onto the log or
   shore beyond; on the Pirate Ship, walking south from beside the Mimic's chest
   hops him across the hole in the deck. */
static Sint32 scCrossboneIsleHopSouth(void)
{
	ACTOR	 *actor = fdActorGet(ACTOR_PARTY_LEADER);
	FIELD_POS destination;

	destination.x = actor->x.fixed;
	destination.y = actor->y.fixed;
	destination.z = actor->z.fixed + toFIXED(32);
	return scCrossboneIsleTryHop(actor, &destination);
}

/* Walking west off the side of a floating log, or off the east shore of either
   pool, with a metatile of water ahead, hops Isaac over it onto the log or
   shore beyond, if there is one to land on. */
static Sint32 scCrossboneIsleHopWest(void)
{
	ACTOR	 *actor = fdActorGet(ACTOR_PARTY_LEADER);
	FIELD_POS destination;

	destination.x = actor->x.fixed + -toFIXED(32);
	destination.y = actor->y.fixed;
	destination.z = actor->z.fixed;
	return scCrossboneIsleTryHop(actor, &destination);
}

/* Walking east off the side of a floating log, or off the west pool's west
   shore, with a metatile of water ahead, hops Isaac over it onto the log or
   shore beyond, if there is one to land on. */
static Sint32 scCrossboneIsleHopEast(void)
{
	ACTOR	 *actor = fdActorGet(ACTOR_PARTY_LEADER);
	FIELD_POS destination;

	destination.x = actor->x.fixed + toFIXED(32);
	destination.y = actor->y.fixed;
	destination.z = actor->z.fixed;
	return scCrossboneIsleTryHop(actor, &destination);
}

/* Moves the metatiles of a log lying west-east from row `row` to the row it now
   lies on, and turns the ones it left back into water that cannot be walked on
   (the stored wall metatiles at (0, 0)). */
static inline void scCrossboneIsleMoveWestEastLogMetatiles(
	Sint32 actorId, Sint32 column, Sint32 row)
{
	sySynch(2);
	{
		Sint32 destinationRow = FIELD_METATILE(fdActorGet(actorId)->z.fixed);
		Sint32 rectangleColumn = column - 1;

		fdMapGridCopyMetatileAttributes(
			rectangleColumn, row, 3, 1, rectangleColumn, destinationRow);
	}
	fdMapGridCopyMetatileAttributes(0, 0, 3, 1, column - 1, row);
}

/* As scCrossboneIsleMoveWestEastLogMetatiles, for a log lying north-south moved
   from column `column`. */
static inline void scCrossboneIsleMoveNorthSouthLogMetatiles(
	Sint32 actorId, Sint32 column, Sint32 row)
{
	sySynch(2);
	{
		Sint32 destinationColumn = FIELD_METATILE(fdActorGet(actorId)->x.fixed);
		Sint32 rectangleRow = row - 1;

		fdMapGridCopyMetatileAttributes(
			column, rectangleRow, 1, 3, destinationColumn, rectangleRow);
	}
	fdMapGridCopyMetatileAttributes(0, 0, 1, 3, column, row - 1);
}

/* Walking north along the east pool's south-west log rolls it north up the
   pool's west side, with Isaac on top, to the north edge below the alcove with
   the Sleep Bomb; there, walking on north hops him ashore.

   The log stops short of the north-west log while that one lies across its way,
   as it does at first, and short of the middle log if that has been rolled west
   across it. */
static void scCrossboneIsleRideLogANorth(void)
{
	Sint32 column = FIELD_METATILE(fdActorGet(ACTOR_LOG_A)->x.fixed);
	Sint32 row = FIELD_METATILE(fdActorGet(ACTOR_LOG_A)->z.fixed);
	Sint32 logEColumn = FIELD_METATILE(fdActorGet(ACTOR_LOG_E)->x.fixed);
	Sint32 logHColumn = FIELD_METATILE(fdActorGet(ACTOR_LOG_H)->x.fixed);

	if(row == 19) {
		if(logEColumn == 24) {
			scCrossboneIsleRideLog(ACTOR_LOG_A, 0, -80);
		} else if(logHColumn == 24) {
			scCrossboneIsleRideLog(ACTOR_LOG_A, 0, -112);
			scCrossboneIsleRideLog(ACTOR_LOG_A, 0, -32);
		} else {
			scCrossboneIsleRideLog(ACTOR_LOG_A, 0, -80);
			scCrossboneIsleRideLog(ACTOR_LOG_A, 0, -112);
		}
	} else if(row == 14) {
		if(logEColumn == 24) {
			return;
		}
		if(logHColumn == 24) {
			scCrossboneIsleRideLog(ACTOR_LOG_A, 0, -64);
		} else {
			scCrossboneIsleRideLog(ACTOR_LOG_A, 0, -112);
		}
	} else if(row == 10) {
		if(logHColumn == 24) {
			return;
		}
		scCrossboneIsleRideLog(ACTOR_LOG_A, 0, -48);
	} else {
		scCrossboneIsleHopNorth();
		return;
	}
	scCrossboneIsleMoveWestEastLogMetatiles(ACTOR_LOG_A, column, row);
}

/* Walking south along the east pool's south-west log rolls it back down the
   pool's west side, with Isaac on top, to the south shore.

   It stops short of the middle log if that has been rolled west across its
   way. */
static void scCrossboneIsleRideLogASouth(void)
{
	Sint32 column = FIELD_METATILE(fdActorGet(ACTOR_LOG_A)->x.fixed);
	Sint32 row = FIELD_METATILE(fdActorGet(ACTOR_LOG_A)->z.fixed);
	Sint32 logEColumn = FIELD_METATILE(fdActorGet(ACTOR_LOG_E)->x.fixed);

	if(row == 7) {
		if(logEColumn == 24) {
			scCrossboneIsleRideLog(ACTOR_LOG_A, 0, 48);
		} else {
			scCrossboneIsleRideLog(ACTOR_LOG_A, 0, 80);
			scCrossboneIsleRideLog(ACTOR_LOG_A, 0, 112);
		}
	} else if(row == 10) {
		if(logEColumn == 24) {
			return;
		}
		scCrossboneIsleRideLog(ACTOR_LOG_A, 0, 144);
	} else if(row == 14) {
		scCrossboneIsleRideLog(ACTOR_LOG_A, 0, 80);
	} else {
		return;
	}
	scCrossboneIsleMoveWestEastLogMetatiles(ACTOR_LOG_A, column, row);
}

/* Walking north along the east pool's log south-east of the island rolls it
   north up the pool, with Isaac on top, to the north edge.

   While the north-west or the north-east log lies across its way at the north
   edge, it stops short of it. */
static void scCrossboneIsleRideLogCNorth(void)
{
	Sint32 column = FIELD_METATILE(fdActorGet(ACTOR_LOG_C)->x.fixed);
	Sint32 row = FIELD_METATILE(fdActorGet(ACTOR_LOG_C)->z.fixed);
	Sint32 logFColumn = FIELD_METATILE(fdActorGet(ACTOR_LOG_F)->x.fixed);
	Sint32 logHColumn = FIELD_METATILE(fdActorGet(ACTOR_LOG_H)->x.fixed);

	if(row == 18) {
		if(LIES_ACROSS(logHColumn, LOG_C_COLUMN)) {
			scCrossboneIsleRideLog(ACTOR_LOG_C, 0, -128);
		} else if(LIES_ACROSS(logFColumn, LOG_C_COLUMN)) {
			scCrossboneIsleRideLog(ACTOR_LOG_C, 0, -128);
		} else {
			scCrossboneIsleRideLog(ACTOR_LOG_C, 0, -112);
			scCrossboneIsleRideLog(ACTOR_LOG_C, 0, -64);
		}
	} else if(row == 10) {
		if(LIES_ACROSS(logHColumn, LOG_C_COLUMN)) {
			return;
		}
		if(LIES_ACROSS(logFColumn, LOG_C_COLUMN)) {
			return;
		}
		scCrossboneIsleRideLog(ACTOR_LOG_C, 0, -48);
	} else if(row == 7) {
		return;
	}
	scCrossboneIsleMoveWestEastLogMetatiles(ACTOR_LOG_C, column, row);
}

/* Walking south along the east pool's log south-east of the island rolls it
   back south, with Isaac on top, to where it started. */
static void scCrossboneIsleRideLogCSouth(void)
{
	Sint32 column = FIELD_METATILE(fdActorGet(ACTOR_LOG_C)->x.fixed);
	Sint32 row = FIELD_METATILE(fdActorGet(ACTOR_LOG_C)->z.fixed);

	if(row != 18) {
		if(row == 10) {
			scCrossboneIsleRideLog(ACTOR_LOG_C, 0, 128);
		} else {
			scCrossboneIsleRideLog(ACTOR_LOG_C, 0, 112);
			scCrossboneIsleRideLog(ACTOR_LOG_C, 0, 64);
		}
		scCrossboneIsleMoveWestEastLogMetatiles(ACTOR_LOG_C, column, row);
	}
}

/* Walking west along the east pool's south-east log rolls it west from the east
   shore, with Isaac on top, to the island.

   While the log south-east of the island is in its starting place, it lies
   across the way and stops this one short of it. */
static void scCrossboneIsleRideLogDWest(void)
{
	Sint32 column = FIELD_METATILE(fdActorGet(ACTOR_LOG_D)->x.fixed);
	Sint32 row = FIELD_METATILE(fdActorGet(ACTOR_LOG_D)->z.fixed);

	if(column != 30) {
		if(column == 34) {
			if(FIELD_METATILE(fdActorGet(ACTOR_LOG_C)->z.fixed) == 18) {
				return;
			}
			scCrossboneIsleRideLog(ACTOR_LOG_D, -64, 0);
		} else if(column == 36) {
			if(FIELD_METATILE(fdActorGet(ACTOR_LOG_C)->z.fixed) == 18) {
				scCrossboneIsleRideLog(ACTOR_LOG_D, -32, 0);
			} else {
				scCrossboneIsleRideLog(ACTOR_LOG_D, -96, 0);
			}
		}
		scCrossboneIsleMoveNorthSouthLogMetatiles(ACTOR_LOG_D, column, row);
	}
}

/* Walking east along the east pool's south-east log rolls it back east, with
   Isaac on top, to the east shore.

   From the island it cannot move while the log south-east of the island is back
   in its starting place. */
static void scCrossboneIsleRideLogDEast(void)
{
	Sint32 column = FIELD_METATILE(fdActorGet(ACTOR_LOG_D)->x.fixed);
	Sint32 row = FIELD_METATILE(fdActorGet(ACTOR_LOG_D)->z.fixed);

	if(column != 36) {
		if(column == 30) {
			if(FIELD_METATILE(fdActorGet(ACTOR_LOG_C)->z.fixed) == 18) {
				return;
			}
			scCrossboneIsleRideLog(ACTOR_LOG_D, 96, 0);
		} else if(column == 34) {
			scCrossboneIsleRideLog(ACTOR_LOG_D, 32, 0);
		}
		scCrossboneIsleMoveNorthSouthLogMetatiles(ACTOR_LOG_D, column, row);
	}
}

/* Walking west along the east pool's middle log rolls it west past the north
   side of the island, with Isaac on top, to the pool's west side. */
static void scCrossboneIsleRideLogEWest(void)
{
	Sint32 column = FIELD_METATILE(fdActorGet(ACTOR_LOG_E)->x.fixed);
	Sint32 row = FIELD_METATILE(fdActorGet(ACTOR_LOG_E)->z.fixed);

	if(column == 36) {
		scCrossboneIsleRideLog(ACTOR_LOG_E, -96, 0);
		scCrossboneIsleRideLog(ACTOR_LOG_E, -96, 0);
	} else if(column == 34) {
		scCrossboneIsleRideLog(ACTOR_LOG_E, -96, 0);
		scCrossboneIsleRideLog(ACTOR_LOG_E, -64, 0);
	} else if(column == 24) {
		return;
	}
	scCrossboneIsleMoveNorthSouthLogMetatiles(ACTOR_LOG_E, column, row);
}

/* Walking east along the east pool's middle log rolls it east, with Isaac on
   top, to the pool's east shore. */
static void scCrossboneIsleRideLogEEast(void)
{
	Sint32 column = FIELD_METATILE(fdActorGet(ACTOR_LOG_E)->x.fixed);
	Sint32 row = FIELD_METATILE(fdActorGet(ACTOR_LOG_E)->z.fixed);

	if(column == 24) {
		scCrossboneIsleRideLog(ACTOR_LOG_E, 96, 0);
		scCrossboneIsleRideLog(ACTOR_LOG_E, 96, 0);
	} else if(column == 34) {
		scCrossboneIsleRideLog(ACTOR_LOG_E, 32, 0);
	} else if(column == 36) {
		return;
	}
	scCrossboneIsleMoveNorthSouthLogMetatiles(ACTOR_LOG_E, column, row);
}

/* Walking west along the east pool's north-east log rolls it west along the
   pool's north edge, with Isaac on top, to the north-west log.

   It stops sooner where the north-west log has been rolled east, or the log
   from south-east of the island has been rolled up to the north edge. */
static void scCrossboneIsleRideLogFWest(void)
{
	Sint32 column = FIELD_METATILE(fdActorGet(ACTOR_LOG_F)->x.fixed);
	Sint32 row = FIELD_METATILE(fdActorGet(ACTOR_LOG_F)->z.fixed);
	Sint32 logCRow = FIELD_METATILE(fdActorGet(ACTOR_LOG_C)->z.fixed);
	Sint32 logHColumn = FIELD_METATILE(fdActorGet(ACTOR_LOG_H)->x.fixed);

	if(column == 36) {
		if(logHColumn == 34) {
			scCrossboneIsleRideLog(ACTOR_LOG_F, -16, 0);
		} else if(logCRow == 7) {
			scCrossboneIsleRideLog(ACTOR_LOG_F, -32, 0);
		} else if(logHColumn == 30) {
			scCrossboneIsleRideLog(ACTOR_LOG_F, -80, 0);
		} else {
			scCrossboneIsleRideLog(ACTOR_LOG_F, -96, 0);
			scCrossboneIsleRideLog(ACTOR_LOG_F, -80, 0);
		}
	} else if(column == 35) {
		if(logHColumn == 34) {
			return;
		}
		if(logCRow == 7) {
			scCrossboneIsleRideLog(ACTOR_LOG_F, -16, 0);
		} else if(logHColumn == 30) {
			scCrossboneIsleRideLog(ACTOR_LOG_F, -64, 0);
		} else {
			scCrossboneIsleRideLog(ACTOR_LOG_F, -80, 0);
			scCrossboneIsleRideLog(ACTOR_LOG_F, -80, 0);
		}
	} else if(column == 34) {
		if(logCRow == 7) {
			return;
		}
		if(logHColumn == 30) {
			scCrossboneIsleRideLog(ACTOR_LOG_F, -48, 0);
		} else {
			scCrossboneIsleRideLog(ACTOR_LOG_F, -144, 0);
		}
	} else if(column == 31) {
		if(logHColumn == 30) {
			return;
		}
		scCrossboneIsleRideLog(ACTOR_LOG_F, -96, 0);
	} else if(column == 25) {
		return;
	}
	scCrossboneIsleMoveNorthSouthLogMetatiles(ACTOR_LOG_F, column, row);
}

/* Walking east along the east pool's north-east log rolls it back east along
   the north edge, with Isaac on top, to the north-east corner. */
static void scCrossboneIsleRideLogFEast(void)
{
	Sint32 column = FIELD_METATILE(fdActorGet(ACTOR_LOG_F)->x.fixed);
	Sint32 row = FIELD_METATILE(fdActorGet(ACTOR_LOG_F)->z.fixed);

	/* DEAD: log H looked up and dropped; the ROM keeps its call. */
	fdActorGet(ACTOR_LOG_H);
	if(column == 25) {
		scCrossboneIsleRideLog(ACTOR_LOG_F, 96, 0);
		scCrossboneIsleRideLog(ACTOR_LOG_F, 80, 0);
	} else if(column == 31) {
		scCrossboneIsleRideLog(ACTOR_LOG_F, 80, 0);
	} else if(column == 34) {
		scCrossboneIsleRideLog(ACTOR_LOG_F, 32, 0);
	} else if(column == 35) {
		scCrossboneIsleRideLog(ACTOR_LOG_F, 16, 0);
	} else if(column == 36) {
		return;
	}
	scCrossboneIsleMoveNorthSouthLogMetatiles(ACTOR_LOG_F, column, row);
}

/* Walking west along the east pool's north-west log rolls it back west along
   the north edge, with Isaac on top, to the north-west corner.

   It stops short of the log from south-east of the island or the south-west log
   if either has been rolled up to the north edge. */
static void scCrossboneIsleRideLogHWest(void)
{
	Sint32 column = FIELD_METATILE(fdActorGet(ACTOR_LOG_H)->x.fixed);
	Sint32 row = FIELD_METATILE(fdActorGet(ACTOR_LOG_H)->z.fixed);
	Sint32 logARow = FIELD_METATILE(fdActorGet(ACTOR_LOG_A)->z.fixed);
	Sint32 logCRow = FIELD_METATILE(fdActorGet(ACTOR_LOG_C)->z.fixed);

	if(column == 35) {
		if(logCRow == 7) {
			scCrossboneIsleRideLog(ACTOR_LOG_H, -16, 0);
		} else if(logARow == 7) {
			scCrossboneIsleRideLog(ACTOR_LOG_H, -112, 0);
		} else {
			scCrossboneIsleRideLog(ACTOR_LOG_H, -96, 0);
			scCrossboneIsleRideLog(ACTOR_LOG_H, -80, 0);
		}
	} else if(column == 34) {
		if(logCRow == 7) {
			return;
		}
		scCrossboneIsleRideLog(ACTOR_LOG_H, -96, 0);
		scCrossboneIsleRideLog(ACTOR_LOG_H, -64, 0);
	} else if(column == 33) {
		scCrossboneIsleRideLog(ACTOR_LOG_H, -144, 0);
	} else if(column == 31) {
		scCrossboneIsleRideLog(ACTOR_LOG_H, -80, 0);
	} else if(column == 30) {
		scCrossboneIsleRideLog(ACTOR_LOG_H, -96, 0);
	} else if(column == 24) {
		return;
	}
	scCrossboneIsleMoveNorthSouthLogMetatiles(ACTOR_LOG_H, column, row);
}

/* Walking east along the east pool's north-west log rolls it east along the
   north edge, with Isaac on top, toward the north-east corner.

   It stops short of the north-east log, wherever that has been rolled, or of
   the log from south-east of the island if that has been rolled up to the north
   edge. */
static void scCrossboneIsleRideLogHEast(void)
{
	Sint32 column = FIELD_METATILE(fdActorGet(ACTOR_LOG_H)->x.fixed);
	Sint32 row = FIELD_METATILE(fdActorGet(ACTOR_LOG_H)->z.fixed);
	Sint32 logCRow = FIELD_METATILE(fdActorGet(ACTOR_LOG_C)->z.fixed);
	Sint32 logFColumn = FIELD_METATILE(fdActorGet(ACTOR_LOG_F)->x.fixed);

	if(column == 24) {
		if(logCRow == 7 || logFColumn == 31) {
			scCrossboneIsleRideLog(ACTOR_LOG_H, 96, 0);
		} else if(logFColumn == 34) {
			scCrossboneIsleRideLog(ACTOR_LOG_H, 64, 0);
			scCrossboneIsleRideLog(ACTOR_LOG_H, 80, 0);
		} else if(logFColumn == 35) {
			scCrossboneIsleRideLog(ACTOR_LOG_H, 80, 0);
			scCrossboneIsleRideLog(ACTOR_LOG_H, 80, 0);
		} else {
			scCrossboneIsleRideLog(ACTOR_LOG_H, 80, 0);
			scCrossboneIsleRideLog(ACTOR_LOG_H, 96, 0);
		}
	} else if(column == 30 || logFColumn == 31) {
		if(logCRow == 7) {
			return;
		}
		if(logFColumn == 34) {
			scCrossboneIsleRideLog(ACTOR_LOG_H, 48, 0);
		} else if(logFColumn == 35) {
			scCrossboneIsleRideLog(ACTOR_LOG_H, 64, 0);
		} else {
			scCrossboneIsleRideLog(ACTOR_LOG_H, 80, 0);
		}
	} else if(column == 33) {
		if(logFColumn == 34) {
			return;
		}
		if(logFColumn == 35) {
			scCrossboneIsleRideLog(ACTOR_LOG_H, 16, 0);
		} else {
			scCrossboneIsleRideLog(ACTOR_LOG_H, 32, 0);
		}
	} else if(column == 34) {
		scCrossboneIsleRideLog(ACTOR_LOG_H, 16, 0);
	} else if(column == 35) {
		return;
	}
	scCrossboneIsleMoveNorthSouthLogMetatiles(ACTOR_LOG_H, column, row);
}

/* Walking north along the west one of the west pool's three south logs rolls it
   north up the bay at the pool's south-west, with Isaac on top.

   It stops short of the south side log if that has been rolled west into the
   bay. */
static void scCrossboneIsleRideLogJNorth(void)
{
	Sint32 column = FIELD_METATILE(fdActorGet(ACTOR_LOG_J)->x.fixed);
	Sint32 row = FIELD_METATILE(fdActorGet(ACTOR_LOG_J)->z.fixed);
	Sint32 logLColumn = FIELD_METATILE(fdActorGet(ACTOR_LOG_L)->x.fixed);

	if(row == 19) {
		if(LIES_ACROSS(logLColumn, LOG_J_COLUMN)) {
			scCrossboneIsleRideLog(ACTOR_LOG_J, 0, -16);
		} else {
			scCrossboneIsleRideLog(ACTOR_LOG_J, 0, -64);
		}
	} else if(row == 18) {
		if(LIES_ACROSS(logLColumn, LOG_J_COLUMN)) {
			return;
		}
		scCrossboneIsleRideLog(ACTOR_LOG_J, 0, -48);
	} else if(row == 15) {
		return;
	}
	scCrossboneIsleMoveWestEastLogMetatiles(ACTOR_LOG_J, column, row);
}

/* Walking south along the west one of the west pool's three south logs rolls it
   back south, with Isaac on top, to the south shore. */
static void scCrossboneIsleRideLogJSouth(void)
{
	Sint32 column = FIELD_METATILE(fdActorGet(ACTOR_LOG_J)->x.fixed);
	Sint32 row = FIELD_METATILE(fdActorGet(ACTOR_LOG_J)->z.fixed);

	if(row == 15) {
		scCrossboneIsleRideLog(ACTOR_LOG_J, 0, 64);
	} else if(row == 18) {
		scCrossboneIsleRideLog(ACTOR_LOG_J, 0, 16);
	} else if(row == 19) {
		return;
	}
	scCrossboneIsleMoveWestEastLogMetatiles(ACTOR_LOG_J, column, row);
}

/* Walking north along the middle one of the west pool's three south logs rolls
   it north up the pool, with Isaac on top, to the north shore.

   It stops short of the first of the pool's side logs that has been rolled west
   across its way. */
static void scCrossboneIsleRideLogKNorth(void)
{
	Sint32 column;
	Sint32 row;
	Sint32 logLColumn;
	Sint32 logGColumn;
	Sint32 logIColumn;

	column = FIELD_METATILE(fdActorGet(ACTOR_LOG_K)->x.fixed);
	row = FIELD_METATILE(fdActorGet(ACTOR_LOG_K)->z.fixed);
	logLColumn = FIELD_METATILE(fdActorGet(ACTOR_LOG_L)->x.fixed);
	logGColumn = FIELD_METATILE(fdActorGet(ACTOR_LOG_G)->x.fixed);
	logIColumn = FIELD_METATILE(fdActorGet(ACTOR_LOG_I)->x.fixed);
	if(row == 19) {
		if(LIES_ACROSS(logLColumn, LOG_K_COLUMN)) {
			scCrossboneIsleRideLog(ACTOR_LOG_K, 0, -16);
		} else if(LIES_ACROSS(logGColumn, LOG_K_COLUMN)) {
			scCrossboneIsleRideLog(ACTOR_LOG_K, 0, -64);
		} else if(LIES_ACROSS(logIColumn, LOG_K_COLUMN)) {
			scCrossboneIsleRideLog(ACTOR_LOG_K, 0, -112);
		} else {
			scCrossboneIsleRideLog(ACTOR_LOG_K, 0, -64);
			scCrossboneIsleRideLog(ACTOR_LOG_K, 0, -96);
		}
	} else if(row == 18) {
		if(LIES_ACROSS(logLColumn, LOG_K_COLUMN)) {
			return;
		}
		if(LIES_ACROSS(logGColumn, LOG_K_COLUMN)) {
			scCrossboneIsleRideLog(ACTOR_LOG_K, 0, -48);
		} else if(LIES_ACROSS(logIColumn, LOG_K_COLUMN)) {
			scCrossboneIsleRideLog(ACTOR_LOG_K, 0, -96);
		} else {
			scCrossboneIsleRideLog(ACTOR_LOG_K, 0, -144);
		}
	} else if(row == 15) {
		if(LIES_ACROSS(logGColumn, LOG_K_COLUMN)) {
			return;
		}
		if(LIES_ACROSS(logIColumn, LOG_K_COLUMN)) {
			scCrossboneIsleRideLog(ACTOR_LOG_K, 0, -48);
		} else {
			scCrossboneIsleRideLog(ACTOR_LOG_K, 0, -96);
		}
	} else if(row == 14) {
		if(LIES_ACROSS(logGColumn, LOG_K_COLUMN)) {
			return;
		}
		if(LIES_ACROSS(logIColumn, LOG_K_COLUMN)) {
			scCrossboneIsleRideLog(ACTOR_LOG_K, 0, -32);
		} else {
			scCrossboneIsleRideLog(ACTOR_LOG_K, 0, -80);
		}
	} else if(row == 12) {
		if(LIES_ACROSS(logIColumn, LOG_K_COLUMN)) {
			return;
		}
		scCrossboneIsleRideLog(ACTOR_LOG_K, 0, -48);
	} else if(row == 11) {
		if(LIES_ACROSS(logIColumn, LOG_K_COLUMN)) {
			return;
		}
		scCrossboneIsleRideLog(ACTOR_LOG_K, 0, -32);
	} else if(row == 9) {
		return;
	}
	sySynch(2);
	fdMapGridCopyMetatileAttributeRect(column - 1, row, 3, 1, column - 1,
		FIELD_METATILE(fdActorGet(ACTOR_LOG_K)->z.fixed));
	fdMapGridCopyMetatileAttributes(0, 0, 3, 1, column - 1, row);
}

/* Walking south along the middle one of the west pool's three south logs rolls
   it back south, with Isaac on top, to the south shore.

   It stops short of the middle or south side log if either has been rolled west
   across its way. */
static void scCrossboneIsleRideLogKSouth(void)
{
	Sint32 column;
	Sint32 row;
	Sint32 logLColumn;
	Sint32 logGColumn;

	column = FIELD_METATILE(fdActorGet(ACTOR_LOG_K)->x.fixed);
	row = FIELD_METATILE(fdActorGet(ACTOR_LOG_K)->z.fixed);
	logLColumn = FIELD_METATILE(fdActorGet(ACTOR_LOG_L)->x.fixed);
	logGColumn = FIELD_METATILE(fdActorGet(ACTOR_LOG_G)->x.fixed);

	if(row == 9) {
		if(LIES_ACROSS(logGColumn, LOG_K_COLUMN)) {
			scCrossboneIsleRideLog(ACTOR_LOG_K, 0, 32);
		} else if(LIES_ACROSS(logLColumn, LOG_K_COLUMN)) {
			scCrossboneIsleRideLog(ACTOR_LOG_K, 0, 80);
		} else {
			scCrossboneIsleRideLog(ACTOR_LOG_K, 0, 64);
			scCrossboneIsleRideLog(ACTOR_LOG_K, 0, 96);
		}
	} else if(row == 11) {
		if(LIES_ACROSS(logGColumn, LOG_K_COLUMN)) {
			return;
		}
		if(LIES_ACROSS(logLColumn, LOG_K_COLUMN)) {
			scCrossboneIsleRideLog(ACTOR_LOG_K, 0, 48);
		} else {
			scCrossboneIsleRideLog(ACTOR_LOG_K, 0, 128);
		}
	} else if(row == 12) {
		if(LIES_ACROSS(logLColumn, LOG_K_COLUMN)) {
			scCrossboneIsleRideLog(ACTOR_LOG_K, 0, 32);
		} else {
			scCrossboneIsleRideLog(ACTOR_LOG_K, 0, 112);
		}
	} else if(row == 14) {
		if(LIES_ACROSS(logLColumn, LOG_K_COLUMN)) {
			return;
		}
		scCrossboneIsleRideLog(ACTOR_LOG_K, 0, 80);
	} else if(row == 15) {
		scCrossboneIsleRideLog(ACTOR_LOG_K, 0, 64);
	} else if(row == 18) {
		scCrossboneIsleRideLog(ACTOR_LOG_K, 0, 16);
	} else if(row == 19) {
		return;
	}

	sySynch(2);

	fdMapGridCopyMetatileAttributeRect(column - 1, row, 3, 1, column - 1,
		FIELD_METATILE(fdActorGet(ACTOR_LOG_K)->z.fixed));
	fdMapGridCopyMetatileAttributes(0, 0, 3, 1, column - 1, row);
}

/* Walking north along the east one of the west pool's three south logs rolls it
   north up the pool, with Isaac on top, to the north shore.

   It stops short of the first of the pool's side logs that has been rolled west
   across its way. */
static void scCrossboneIsleRideLogBNorth(void)
{
	Sint32 column;
	Sint32 row;
	Sint32 logLColumn;
	Sint32 logGColumn;
	Sint32 logIColumn;

	column = FIELD_METATILE(fdActorGet(ACTOR_LOG_B)->x.fixed);
	row = FIELD_METATILE(fdActorGet(ACTOR_LOG_B)->z.fixed);
	logLColumn = FIELD_METATILE(fdActorGet(ACTOR_LOG_L)->x.fixed);
	logGColumn = FIELD_METATILE(fdActorGet(ACTOR_LOG_G)->x.fixed);
	logIColumn = FIELD_METATILE(fdActorGet(ACTOR_LOG_I)->x.fixed);
	if(row == 19) {
		if(LIES_ACROSS(logLColumn, LOG_B_COLUMN)) {
			scCrossboneIsleRideLog(ACTOR_LOG_B, 0, -16);
		} else if(LIES_ACROSS(logGColumn, LOG_B_COLUMN)) {
			scCrossboneIsleRideLog(ACTOR_LOG_B, 0, -64);
		} else if(LIES_ACROSS(logIColumn, LOG_B_COLUMN)) {
			scCrossboneIsleRideLog(ACTOR_LOG_B, 0, -112);
		} else {
			scCrossboneIsleRideLog(ACTOR_LOG_B, 0, -80);
			scCrossboneIsleRideLog(ACTOR_LOG_B, 0, -96);
		}
	} else if(row == 18) {
		if(LIES_ACROSS(logLColumn, LOG_B_COLUMN)) {
			return;
		}
		if(LIES_ACROSS(logGColumn, LOG_B_COLUMN)) {
			scCrossboneIsleRideLog(ACTOR_LOG_B, 0, -48);
		} else if(LIES_ACROSS(logIColumn, LOG_B_COLUMN)) {
			scCrossboneIsleRideLog(ACTOR_LOG_B, 0, -96);
		} else {
			scCrossboneIsleRideLog(ACTOR_LOG_B, 0, -96);
			scCrossboneIsleRideLog(ACTOR_LOG_B, 0, -64);
		}
	} else if(row == 15) {
		if(LIES_ACROSS(logGColumn, LOG_B_COLUMN)) {
			return;
		}
		if(LIES_ACROSS(logIColumn, LOG_B_COLUMN)) {
			scCrossboneIsleRideLog(ACTOR_LOG_B, 0, -48);
		} else {
			scCrossboneIsleRideLog(ACTOR_LOG_B, 0, -112);
		}
	} else if(row == 14) {
		if(LIES_ACROSS(logGColumn, LOG_B_COLUMN)) {
			return;
		}
		if(LIES_ACROSS(logIColumn, LOG_B_COLUMN)) {
			scCrossboneIsleRideLog(ACTOR_LOG_B, 0, -32);
		} else {
			scCrossboneIsleRideLog(ACTOR_LOG_B, 0, -96);
		}
	} else if(row == 12) {
		if(LIES_ACROSS(logIColumn, LOG_B_COLUMN)) {
			return;
		}
		scCrossboneIsleRideLog(ACTOR_LOG_B, 0, -64);
	} else if(row == 11) {
		if(LIES_ACROSS(logIColumn, LOG_B_COLUMN)) {
			return;
		}
		scCrossboneIsleRideLog(ACTOR_LOG_B, 0, -48);
	} else if(row >= 0 && row <= 9) {
		return;
	}
	sySynch(2);
	fdMapGridCopyMetatileAttributeRect(column - 1, row, 3, 1, column - 1,
		FIELD_METATILE(fdActorGet(ACTOR_LOG_B)->z.fixed));
	fdMapGridCopyMetatileAttributes(0, 0, 3, 1, column - 1, row);
}

/* Walking south along the east one of the west pool's three south logs rolls it
   back south, with Isaac on top, to the south shore.

   It stops short of the first of the pool's side logs that has been rolled west
   across its way. */
static void scCrossboneIsleRideLogBSouth(void)
{
	Sint32 column;
	Sint32 row;
	Sint32 logLColumn;
	Sint32 logGColumn;
	Sint32 logIColumn;

	column = FIELD_METATILE(fdActorGet(ACTOR_LOG_B)->x.fixed);
	row = FIELD_METATILE(fdActorGet(ACTOR_LOG_B)->z.fixed);
	logLColumn = FIELD_METATILE(fdActorGet(ACTOR_LOG_L)->x.fixed);
	logGColumn = FIELD_METATILE(fdActorGet(ACTOR_LOG_G)->x.fixed);
	logIColumn = FIELD_METATILE(fdActorGet(ACTOR_LOG_I)->x.fixed);

	if(row == 8) {
		if(LIES_ACROSS(logIColumn, LOG_B_COLUMN)) {
			return;
		}
		if(LIES_ACROSS(logGColumn, LOG_B_COLUMN)) {
			scCrossboneIsleRideLog(ACTOR_LOG_B, 0, 48);
		} else {
			if(!LIES_ACROSS(logLColumn, LOG_B_COLUMN)) {
				scCrossboneIsleRideLog(ACTOR_LOG_B, 0, 80);
			}
			scCrossboneIsleRideLog(ACTOR_LOG_B, 0, 96);
		}
	} else if(row == 11) {
		if(LIES_ACROSS(logGColumn, LOG_B_COLUMN)) {
			return;
		}
		if(LIES_ACROSS(logLColumn, LOG_B_COLUMN)) {
			scCrossboneIsleRideLog(ACTOR_LOG_B, 0, 48);
		} else {
			scCrossboneIsleRideLog(ACTOR_LOG_B, 0, 128);
		}
	} else if(row == 12) {
		if(LIES_ACROSS(logGColumn, LOG_B_COLUMN)) {
			return;
		}
		if(LIES_ACROSS(logLColumn, LOG_B_COLUMN)) {
			scCrossboneIsleRideLog(ACTOR_LOG_B, 0, 32);
		} else {
			scCrossboneIsleRideLog(ACTOR_LOG_B, 0, 112);
		}
	} else if(row == 14) {
		if(LIES_ACROSS(logLColumn, LOG_B_COLUMN)) {
			return;
		}
		scCrossboneIsleRideLog(ACTOR_LOG_B, 0, 80);
	} else if(row == 15) {
		scCrossboneIsleRideLog(ACTOR_LOG_B, 0, 64);
	} else if(row == 18) {
		scCrossboneIsleRideLog(ACTOR_LOG_B, 0, 16);
	}

	sySynch(2);

	fdMapGridCopyMetatileAttributeRect(column - 1, row, 3, 1, column - 1,
		FIELD_METATILE(fdActorGet(ACTOR_LOG_B)->z.fixed));
	fdMapGridCopyMetatileAttributes(0, 0, 3, 1, column - 1, row);
}

/* Walking west along the south one of the west pool's three side logs rolls it
   west across the pool, with Isaac on top, into the bay at its south-west.

   It stops short of the first of the pool's south logs that has been rolled
   north across its way. */
static void scCrossboneIsleRideLogLWest(void)
{
	Sint32 column;
	Sint32 row;
	Sint32 logJRow;
	Sint32 logKRow;
	Sint32 logBRow;

	column = FIELD_METATILE(fdActorGet(ACTOR_LOG_L)->x.fixed);
	row = FIELD_METATILE(fdActorGet(ACTOR_LOG_L)->z.fixed);
	logJRow = FIELD_METATILE(fdActorGet(ACTOR_LOG_J)->z.fixed);
	logKRow = FIELD_METATILE(fdActorGet(ACTOR_LOG_K)->z.fixed);
	logBRow = FIELD_METATILE(fdActorGet(ACTOR_LOG_B)->z.fixed);
	if(column == 3) {
		return;
	}
	if(column == 13) {
		if(logBRow == 15) {
			scCrossboneIsleRideLog(ACTOR_LOG_L, -16, 0);
		} else if(logKRow == 15) {
			scCrossboneIsleRideLog(ACTOR_LOG_L, -64, 0);
		} else if(logJRow == 15) {
			scCrossboneIsleRideLog(ACTOR_LOG_L, -112, 0);
		} else {
			scCrossboneIsleRideLog(ACTOR_LOG_L, -112, 0);
			scCrossboneIsleRideLog(ACTOR_LOG_L, -48, 0);
		}
	} else if(column == 6) {
		if(logJRow == 15) {
			return;
		}
		scCrossboneIsleRideLog(ACTOR_LOG_L, -48, 0);
	} else if(column == 5) {
		scCrossboneIsleRideLog(ACTOR_LOG_L, -32, 0);
	} else if(column == 8) {
		if(logKRow == 15) {
			return;
		}
		if(logJRow == 15) {
			scCrossboneIsleRideLog(ACTOR_LOG_L, -32, 0);
		} else {
			scCrossboneIsleRideLog(ACTOR_LOG_L, -80, 0);
		}
	} else if(column == 9) {
		if(logKRow == 15) {
			return;
		}
		if(logJRow == 15) {
			scCrossboneIsleRideLog(ACTOR_LOG_L, -48, 0);
		} else {
			scCrossboneIsleRideLog(ACTOR_LOG_L, -96, 0);
		}
	} else if(column == 12) {
		if(logBRow == 15) {
			return;
		}
		if(logKRow == 15) {
			scCrossboneIsleRideLog(ACTOR_LOG_L, -48, 0);
		} else if(logJRow == 15) {
			scCrossboneIsleRideLog(ACTOR_LOG_L, -96, 0);
		} else {
			scCrossboneIsleRideLog(ACTOR_LOG_L, -144, 0);
		}
	}
	sySynch(2);
	fdMapGridCopyMetatileAttributeRect(column, row - 1, 1, 3,
		FIELD_METATILE(fdActorGet(ACTOR_LOG_L)->x.fixed), row - 1);
	fdMapGridCopyMetatileAttributes(0, 0, 1, 3, column, row - 1);
}

/* Walking east along the south one of the west pool's three side logs rolls it
   back east, with Isaac on top, to the pool's east shore.

   It stops short of the middle or east south log if either has been rolled
   north across its way. */
static void scCrossboneIsleRideLogLEast(void)
{
	Sint32 column;
	Sint32 row;
	Sint32 logKRow;
	Sint32 logBRow;

	column = FIELD_METATILE(fdActorGet(ACTOR_LOG_L)->x.fixed);
	row = FIELD_METATILE(fdActorGet(ACTOR_LOG_L)->z.fixed);
	logKRow = FIELD_METATILE(fdActorGet(ACTOR_LOG_K)->z.fixed);
	logBRow = FIELD_METATILE(fdActorGet(ACTOR_LOG_B)->z.fixed);
	if(column == 3) {
		if(logKRow == 15) {
			scCrossboneIsleRideLog(ACTOR_LOG_L, 32, 0);
		} else if(logBRow == 15) {
			scCrossboneIsleRideLog(ACTOR_LOG_L, 80, 0);
		} else {
			scCrossboneIsleRideLog(ACTOR_LOG_L, 112, 0);
			scCrossboneIsleRideLog(ACTOR_LOG_L, 48, 0);
		}
	} else if(column == 5) {
		if(logKRow == 15) {
			return;
		}
		if(logBRow == 15) {
			scCrossboneIsleRideLog(ACTOR_LOG_L, 48, 0);
		} else {
			scCrossboneIsleRideLog(ACTOR_LOG_L, 128, 0);
		}
	} else if(column == 6) {
		if(logBRow == 15) {
			scCrossboneIsleRideLog(ACTOR_LOG_L, 32, 0);
		} else {
			scCrossboneIsleRideLog(ACTOR_LOG_L, 112, 0);
		}
	} else if(column == 8) {
		if(logBRow == 15) {
			return;
		}
		scCrossboneIsleRideLog(ACTOR_LOG_L, 80, 0);
	} else if(column == 9) {
		scCrossboneIsleRideLog(ACTOR_LOG_L, 64, 0);
	} else if(column == 12) {
		scCrossboneIsleRideLog(ACTOR_LOG_L, 16, 0);
	}
	sySynch(2);
	fdMapGridCopyMetatileAttributeRect(column, row - 1, 1, 3,
		FIELD_METATILE(fdActorGet(ACTOR_LOG_L)->x.fixed), row - 1);
	fdMapGridCopyMetatileAttributes(0, 0, 1, 3, column, row - 1);
}

/* Walking west along the middle one of the west pool's three side logs rolls it
   west across the pool, with Isaac on top, to its west side.

   It stops short of the east or middle south log if either has been rolled
   north across its way. */
static void scCrossboneIsleRideLogGWest(void)
{
	Sint32 column;
	Sint32 row;
	Sint32 logKRow;
	Sint32 logBRow;
	Sint32 rectangleRow;

	column = FIELD_METATILE(fdActorGet(ACTOR_LOG_G)->x.fixed);
	row = FIELD_METATILE(fdActorGet(ACTOR_LOG_G)->z.fixed);
	logKRow = FIELD_METATILE(fdActorGet(ACTOR_LOG_K)->z.fixed);
	logBRow = FIELD_METATILE(fdActorGet(ACTOR_LOG_B)->z.fixed);

	if(column == 13) {
		if(LIES_ACROSS(logBRow, LOG_G_ROW)) {
			scCrossboneIsleRideLog(ACTOR_LOG_G, -16, 0);
		} else if(LIES_ACROSS(logKRow, LOG_G_ROW)) {
			scCrossboneIsleRideLog(ACTOR_LOG_G, -64, 0);
		} else {
			scCrossboneIsleRideLog(ACTOR_LOG_G, -112, 0);
		}
	} else if(column == 12) {
		if(LIES_ACROSS(logBRow, LOG_G_ROW)) {
			return;
		}
		if(LIES_ACROSS(logKRow, LOG_G_ROW)) {
			scCrossboneIsleRideLog(ACTOR_LOG_G, -48, 0);
		} else {
			scCrossboneIsleRideLog(ACTOR_LOG_G, -96, 0);
		}
	} else if(column == 9) {
		if(LIES_ACROSS(logKRow, LOG_G_ROW)) {
			return;
		}
		scCrossboneIsleRideLog(ACTOR_LOG_G, -48, 0);
	} else if(column == 8) {
		if(LIES_ACROSS(logKRow, LOG_G_ROW)) {
			return;
		}
		scCrossboneIsleRideLog(ACTOR_LOG_G, -32, 0);
	} else if(column == 6) {
		return;
	}

	sySynch(2);

	rectangleRow = row - 1;
	fdMapGridCopyMetatileAttributeRect(column, rectangleRow, 1, 3,
		FIELD_METATILE(fdActorGet(ACTOR_LOG_G)->x.fixed), rectangleRow);
	fdMapGridCopyMetatileAttributes(0, 0, 1, 3, column, rectangleRow);
}

/* Walking east along the middle one of the west pool's three side logs rolls it
   back east, with Isaac on top, to the pool's east shore.

   It stops short of the east or middle south log if either has been rolled
   north across its way. */
static void scCrossboneIsleRideLogGEast(void)
{
	Sint32 column;
	Sint32 row;
	Sint32 logKRow;
	Sint32 logBRow;
	Sint32 rectangleRow;

	column = FIELD_METATILE(fdActorGet(ACTOR_LOG_G)->x.fixed);
	row = FIELD_METATILE(fdActorGet(ACTOR_LOG_G)->z.fixed);
	logKRow = FIELD_METATILE(fdActorGet(ACTOR_LOG_K)->z.fixed);
	logBRow = FIELD_METATILE(fdActorGet(ACTOR_LOG_B)->z.fixed);

	if(column == 6) {
		if(LIES_ACROSS(logBRow, LOG_G_ROW)) {
			scCrossboneIsleRideLog(ACTOR_LOG_G, 32, 0);
		} else if(LIES_ACROSS(logKRow, LOG_G_ROW)) {
			scCrossboneIsleRideLog(ACTOR_LOG_G, 64, 0);
		} else {
			scCrossboneIsleRideLog(ACTOR_LOG_G, 112, 0);
		}
	} else if(column == 8) {
		if(LIES_ACROSS(logBRow, LOG_G_ROW)) {
			return;
		}
		scCrossboneIsleRideLog(ACTOR_LOG_G, 80, 0);
	} else if(column == 9) {
		if(LIES_ACROSS(logBRow, LOG_G_ROW)) {
			return;
		}
		scCrossboneIsleRideLog(ACTOR_LOG_G, 64, 0);
	} else if(column == 12) {
		scCrossboneIsleRideLog(ACTOR_LOG_G, 16, 0);
	} else if(column == 13) {
		return;
	}

	sySynch(2);

	rectangleRow = row - 1;
	fdMapGridCopyMetatileAttributeRect(column, rectangleRow, 1, 3,
		FIELD_METATILE(fdActorGet(ACTOR_LOG_G)->x.fixed), rectangleRow);
	fdMapGridCopyMetatileAttributes(0, 0, 1, 3, column, rectangleRow);
}

/* Walking west along the north one of the west pool's three side logs rolls it
   west across the north of the pool, with Isaac on top, to its west side.

   It stops short of the east or middle south log if either has been rolled
   north across its way. */
static void scCrossboneIsleRideLogIWest(void)
{
	Sint32 column;
	Sint32 row;
	Sint32 logKRow;
	Sint32 logBRow;
	Sint32 rectangleRow;

	column = FIELD_METATILE(fdActorGet(ACTOR_LOG_I)->x.fixed);
	row = FIELD_METATILE(fdActorGet(ACTOR_LOG_I)->z.fixed);
	logKRow = FIELD_METATILE(fdActorGet(ACTOR_LOG_K)->z.fixed);
	logBRow = FIELD_METATILE(fdActorGet(ACTOR_LOG_B)->z.fixed);

	if(column == 13) {
		if(LIES_ACROSS(logBRow, LOG_I_ROW)) {
			scCrossboneIsleRideLog(ACTOR_LOG_I, -16, 0);
		} else if(LIES_ACROSS(logKRow, LOG_I_ROW)) {
			scCrossboneIsleRideLog(ACTOR_LOG_I, -64, 0);
		} else {
			scCrossboneIsleRideLog(ACTOR_LOG_I, -112, 0);
		}
	} else if(column == 12) {
		if(LIES_ACROSS(logBRow, LOG_I_ROW)) {
			return;
		}
		if(LIES_ACROSS(logKRow, LOG_I_ROW)) {
			scCrossboneIsleRideLog(ACTOR_LOG_I, -48, 0);
		} else {
			scCrossboneIsleRideLog(ACTOR_LOG_I, -96, 0);
		}
	} else if(column == 9) {
		if(LIES_ACROSS(logKRow, LOG_I_ROW)) {
			return;
		}
		scCrossboneIsleRideLog(ACTOR_LOG_I, -48, 0);
	} else if(column == 8) {
		scCrossboneIsleRideLog(ACTOR_LOG_I, -32, 0);
	} else if(column == 6) {
		return;
	}

	sySynch(2);

	rectangleRow = row - 1;
	fdMapGridCopyMetatileAttributeRect(column, rectangleRow, 1, 3,
		FIELD_METATILE(fdActorGet(ACTOR_LOG_I)->x.fixed), rectangleRow);
	fdMapGridCopyMetatileAttributes(0, 0, 1, 3, column, rectangleRow);
}

/* Walking east along the north one of the west pool's three side logs rolls it
   back east, with Isaac on top, to the pool's east shore.

   It stops short of the east south log if that has been rolled north across its
   way. */
static void scCrossboneIsleRideLogIEast(void)
{
	Sint32 column;
	Sint32 row;
	Sint32 logBRow;

	column = FIELD_METATILE(fdActorGet(ACTOR_LOG_I)->x.fixed);
	row = FIELD_METATILE(fdActorGet(ACTOR_LOG_I)->z.fixed);
	logBRow = FIELD_METATILE(fdActorGet(ACTOR_LOG_B)->z.fixed);

	if(column == 6) {
		if(LIES_ACROSS(logBRow, LOG_I_ROW)) {
			scCrossboneIsleRideLog(ACTOR_LOG_I, 32, 0);
		} else {
			scCrossboneIsleRideLog(ACTOR_LOG_I, 112, 0);
		}
	} else if(column == 8) {
		if(LIES_ACROSS(logBRow, LOG_I_ROW)) {
			return;
		}
		scCrossboneIsleRideLog(ACTOR_LOG_I, 80, 0);
	} else if(column == 9) {
		scCrossboneIsleRideLog(ACTOR_LOG_I, 64, 0);
	} else if(column == 12) {
		scCrossboneIsleRideLog(ACTOR_LOG_I, 16, 0);
	} else if(column == 13) {
		return;
	}

	sySynch(2);

	fdMapGridCopyMetatileAttributeRect(column, row - 1, 1, 3,
		FIELD_METATILE(fdActorGet(ACTOR_LOG_I)->x.fixed), row - 1);
	fdMapGridCopyMetatileAttributes(0, 0, 1, 3, column, row - 1);
}

#include "shared/floating_nut.c"

#include "shared/pushed_actor_tables.c"
/* clang-format off */

#include "shared/field_effect_duration_scripts.c"

/* A floor's side-room door opening with a rumble, a column of two metatiles */
/* at a time. */
Uint16 CrossboneIsle_DoorOpens[] = {
	MAPGRID_ANIMATION_STEP(58, 23, 1, 2, 5), MAPGRID_ANIMATION_STEP(59, 23, 1, 2, 5), MAPGRID_ANIMATION_STEPS_END,
};

const Sint32 *FieldEffect_DurationScripts[] = {
	FieldEffect_ShortScript,
	FieldEffect_MediumScript,
	FieldEffect_LongScript,
};

#include "shared/fallback_entrances.c"

SCENE_ENTRANCE CrossboneIsle_IsleEntrances[] = {
	{ CROSSBONE_ISLE_FROM_WORLD_MAP, CONDITION_ALWAYS, 280, 0, 544, FACING_NORTH, 0, 0, 0, 512, 560, 0 },
	{ CROSSBONE_ISLE_FROM_CROSSBONE_ISLE_CAVE_ENTRANCE, CONDITION_ALWAYS, 136, 0, 360, FACING_SOUTH, 0, 0, 0, 512, 560, 0 },
	{ SCENE_TABLE_END },
};

SCENE_ENTRANCE CrossboneIsle_CaveEntranceEntrances[] = {
	{ CROSSBONE_ISLE_CAVE_ENTRANCE_FROM_CROSSBONE_ISLE, CONDITION_ALWAYS, 312, 0, 520, FACING_NORTH, 0, 16, 64, 416, 544, 0 },
	{ CROSSBONE_ISLE_CAVE_ENTRANCE_FROM_CROSSBONE_ISLE_FIRST_FLOOR, CONDITION_ALWAYS, 280, 0, 144, FACING_EAST, 0, 16, 64, 416, 544, 0 },
	{ SCENE_TABLE_END },
};

SCENE_ENTRANCE CrossboneIsle_FloatingLogsEntrances[] = {
	{ CROSSBONE_ISLE_FLOATING_LOGS_FROM_CROSSBONE_ISLE_NINTH_FLOOR_EAST, CONDITION_ALWAYS, 312, 0, 408, FACING_NORTH, 0, 0, 16, 640, 464, 0 },
	{ CROSSBONE_ISLE_FLOATING_LOGS_FROM_CROSSBONE_ISLE_NINTH_FLOOR_WEST, CONDITION_ALWAYS, 200, 0, 408, FACING_NORTH, 0, 0, 16, 640, 464, 0 },
	{ CROSSBONE_ISLE_FLOATING_LOGS_MIDDLE, CONDITION_ALWAYS, 272, 0, 232, FACING_SOUTH, 0, 0, 16, 640, 464, 0 },
	{ SCENE_TABLE_END },
};

SCENE_ENTRANCE CrossboneIsle_PierEntrances[] = {
	{ CROSSBONE_ISLE_PIER_FROM_CROSSBONE_ISLE_NINTH_FLOOR, CONDITION_ALWAYS, 184, 0, 136, FACING_WEST, 0, -1, -1, -1, -1, 0 },
	{ CROSSBONE_ISLE_PIER_FROM_PIRATE_SHIP, CONDITION_ALWAYS, 40, 0, 184, FACING_EAST, 0, -1, -1, -1, -1, 0 },
	{ SCENE_TABLE_END },
};

SCENE_ENTRANCE CrossboneIsle_PirateShipEntrances[] = {
	{ PIRATE_SHIP_FROM_CROSSBONE_ISLE_PIER, CONDITION_ALWAYS, 312, 0, 656, FACING_WEST, 0, -1, -1, -1, -1, 0 },
	{ ENTRANCE_AFTER_BATTLE, CONDITION_ALWAYS, 216, 0, 544, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
	/* The nine floors (CrossboneIsleFirstFloor to CrossboneIsleNinthFloor) */
	/* enter through this shared tail, since every floor numbers its */
	/* entrances alike. */

	{ FLOOR_FROM_PREVIOUS_ROOM, CONDITION_ALWAYS, 840, 0, 176, FACING_WEST, 0, 432, 32, 880, 352, 0 },
	{ FLOOR_FROM_NEXT_ROOM, CONDITION_ALWAYS, 488, 0, 176, FACING_EAST, 0, 432, 32, 880, 352, 0 },
	{ FLOOR_FROM_SIDE_ROOM_EAST, CONDITION_ALWAYS, 712, 0, 152, FACING_SOUTH, 0, 432, 32, 880, 352, 0 },
	{ FLOOR_FROM_SIDE_ROOM_WEST, CONDITION_ALWAYS, 600, 0, 152, FACING_SOUTH, 0, 432, 32, 880, 352, 0 },
	{ ENTRANCE_AFTER_BATTLE, CONDITION_ALWAYS, 712, 0, 152, FACING_NORTH, 0, 432, 32, 880, 352, 0 },
	{ SCENE_TABLE_END },
};

Uint32 CrossboneIsle_Exits[] = {
	SCENE_EXITS(CrossboneIsle),
	SCENE_EXIT(CROSSBONE_ISLE_TO_WORLD_MAP, WorldMap, WORLD_MAP_FROM_CROSSBONE_ISLE),
	SCENE_EXIT(CROSSBONE_ISLE_TO_CROSSBONE_ISLE_CAVE_ENTRANCE, CrossboneIsleCaveEntrance, CROSSBONE_ISLE_CAVE_ENTRANCE_FROM_CROSSBONE_ISLE),
	SCENE_EXITS(CrossboneIsleCaveEntrance),
	SCENE_EXIT(CROSSBONE_ISLE_CAVE_ENTRANCE_TO_CROSSBONE_ISLE, CrossboneIsle, CROSSBONE_ISLE_FROM_CROSSBONE_ISLE_CAVE_ENTRANCE),
	SCENE_EXIT(CROSSBONE_ISLE_CAVE_ENTRANCE_TO_CROSSBONE_ISLE_FIRST_FLOOR, CrossboneIsleFirstFloor, CROSSBONE_ISLE_FIRST_FLOOR_FROM_CROSSBONE_ISLE_CAVE_ENTRANCE),
	SCENE_EXITS(CrossboneIsleFloatingLogs),
	SCENE_EXIT(CROSSBONE_ISLE_FLOATING_LOGS_TO_CROSSBONE_ISLE_NINTH_FLOOR_EAST, CrossboneIsleNinthFloor, CROSSBONE_ISLE_NINTH_FLOOR_FROM_CROSSBONE_ISLE_FLOATING_LOGS_EAST),
	SCENE_EXIT(CROSSBONE_ISLE_FLOATING_LOGS_TO_CROSSBONE_ISLE_NINTH_FLOOR_WEST, CrossboneIsleNinthFloor, CROSSBONE_ISLE_NINTH_FLOOR_FROM_CROSSBONE_ISLE_FLOATING_LOGS_WEST),
	SCENE_EXITS(CrossboneIslePier),
	SCENE_EXIT(CROSSBONE_ISLE_PIER_TO_CROSSBONE_ISLE_NINTH_FLOOR, CrossboneIsleNinthFloor, CROSSBONE_ISLE_NINTH_FLOOR_FROM_CROSSBONE_ISLE_PIER),
	SCENE_EXIT(CROSSBONE_ISLE_PIER_TO_PIRATE_SHIP, PirateShip, PIRATE_SHIP_FROM_CROSSBONE_ISLE_PIER),
	SCENE_EXITS(PirateShip),
	SCENE_EXIT(PIRATE_SHIP_TO_CROSSBONE_ISLE_PIER, CrossboneIslePier, CROSSBONE_ISLE_PIER_FROM_PIRATE_SHIP),
	SCENE_EXITS(CrossboneIsleFirstFloor),
	SCENE_EXIT(CROSSBONE_ISLE_FIRST_FLOOR_TO_CROSSBONE_ISLE_CAVE_ENTRANCE, CrossboneIsleCaveEntrance, CROSSBONE_ISLE_CAVE_ENTRANCE_FROM_CROSSBONE_ISLE_FIRST_FLOOR),
	SCENE_EXIT(CROSSBONE_ISLE_FIRST_FLOOR_TO_CROSSBONE_ISLE_SECOND_FLOOR, CrossboneIsleSecondFloor, CROSSBONE_ISLE_SECOND_FLOOR_FROM_CROSSBONE_ISLE_FIRST_FLOOR),
	SCENE_EXIT(CROSSBONE_ISLE_FIRST_FLOOR_TO_CROSSBONE_ISLE_STEPPING_STONES_EAST, CrossboneIsleSteppingStones, CROSSBONE_ISLE_STEPPING_STONES_FROM_CROSSBONE_ISLE_FIRST_FLOOR_EAST),
	SCENE_EXIT(CROSSBONE_ISLE_FIRST_FLOOR_TO_CROSSBONE_ISLE_STEPPING_STONES_WEST, CrossboneIsleSteppingStones, CROSSBONE_ISLE_STEPPING_STONES_FROM_CROSSBONE_ISLE_FIRST_FLOOR_WEST),
	SCENE_EXITS(CrossboneIsleSecondFloor),
	SCENE_EXIT(CROSSBONE_ISLE_SECOND_FLOOR_TO_CROSSBONE_ISLE_FIRST_FLOOR, CrossboneIsleFirstFloor, CROSSBONE_ISLE_FIRST_FLOOR_FROM_CROSSBONE_ISLE_SECOND_FLOOR),
	SCENE_EXIT(CROSSBONE_ISLE_SECOND_FLOOR_TO_CROSSBONE_ISLE_THIRD_FLOOR, CrossboneIsleThirdFloor, CROSSBONE_ISLE_THIRD_FLOOR_FROM_CROSSBONE_ISLE_SECOND_FLOOR),
	SCENE_EXIT(CROSSBONE_ISLE_SECOND_FLOOR_TO_CROSSBONE_ISLE_ICE_PILLARS_EAST, CrossboneIsleIcePillars, CROSSBONE_ISLE_ICE_PILLARS_FROM_CROSSBONE_ISLE_SECOND_FLOOR_EAST),
	SCENE_EXIT(CROSSBONE_ISLE_SECOND_FLOOR_TO_CROSSBONE_ISLE_ICE_PILLARS_WEST, CrossboneIsleIcePillars, CROSSBONE_ISLE_ICE_PILLARS_FROM_CROSSBONE_ISLE_SECOND_FLOOR_WEST),
	SCENE_EXITS(CrossboneIsleThirdFloor),
	SCENE_EXIT(CROSSBONE_ISLE_THIRD_FLOOR_TO_CROSSBONE_ISLE_SECOND_FLOOR, CrossboneIsleSecondFloor, CROSSBONE_ISLE_SECOND_FLOOR_FROM_CROSSBONE_ISLE_THIRD_FLOOR),
	SCENE_EXIT(CROSSBONE_ISLE_THIRD_FLOOR_TO_CROSSBONE_ISLE_FOURTH_FLOOR, CrossboneIsleFourthFloor, CROSSBONE_ISLE_FOURTH_FLOOR_FROM_CROSSBONE_ISLE_THIRD_FLOOR),
	SCENE_EXIT(CROSSBONE_ISLE_THIRD_FLOOR_TO_CROSSBONE_ISLE_KEYS_EAST, CrossboneIsleKeys, CROSSBONE_ISLE_KEYS_FROM_CROSSBONE_ISLE_THIRD_FLOOR_EAST),
	SCENE_EXIT(CROSSBONE_ISLE_THIRD_FLOOR_TO_CROSSBONE_ISLE_KEYS_WEST, CrossboneIsleKeys, CROSSBONE_ISLE_KEYS_FROM_CROSSBONE_ISLE_THIRD_FLOOR_WEST),
	SCENE_EXITS(CrossboneIsleFourthFloor),
	SCENE_EXIT(CROSSBONE_ISLE_FOURTH_FLOOR_TO_CROSSBONE_ISLE_THIRD_FLOOR, CrossboneIsleThirdFloor, CROSSBONE_ISLE_THIRD_FLOOR_FROM_CROSSBONE_ISLE_FOURTH_FLOOR),
	SCENE_EXIT(CROSSBONE_ISLE_FOURTH_FLOOR_TO_CROSSBONE_ISLE_FIFTH_FLOOR, CrossboneIsleFifthFloor, CROSSBONE_ISLE_FIFTH_FLOOR_FROM_CROSSBONE_ISLE_FOURTH_FLOOR),
	SCENE_EXIT(CROSSBONE_ISLE_FOURTH_FLOOR_TO_CROSSBONE_ISLE_STATUES_EAST, CrossboneIsleStatues, CROSSBONE_ISLE_STATUES_FROM_CROSSBONE_ISLE_FOURTH_FLOOR_EAST),
	SCENE_EXIT(CROSSBONE_ISLE_FOURTH_FLOOR_TO_CROSSBONE_ISLE_STATUES_WEST, CrossboneIsleStatues, CROSSBONE_ISLE_STATUES_FROM_CROSSBONE_ISLE_FOURTH_FLOOR_WEST),
	SCENE_EXITS(CrossboneIsleFifthFloor),
	SCENE_EXIT(CROSSBONE_ISLE_FIFTH_FLOOR_TO_CROSSBONE_ISLE_FOURTH_FLOOR, CrossboneIsleFourthFloor, CROSSBONE_ISLE_FOURTH_FLOOR_FROM_CROSSBONE_ISLE_FIFTH_FLOOR),
	SCENE_EXIT(CROSSBONE_ISLE_FIFTH_FLOOR_TO_CROSSBONE_ISLE_SIXTH_FLOOR, CrossboneIsleSixthFloor, CROSSBONE_ISLE_SIXTH_FLOOR_FROM_CROSSBONE_ISLE_FIFTH_FLOOR),
	SCENE_EXIT(CROSSBONE_ISLE_FIFTH_FLOOR_TO_CROSSBONE_ISLE_FLOODGATE_EAST, CrossboneIsleFloodgate, CROSSBONE_ISLE_FLOODGATE_FROM_CROSSBONE_ISLE_FIFTH_FLOOR_EAST),
	SCENE_EXIT(CROSSBONE_ISLE_FIFTH_FLOOR_TO_CROSSBONE_ISLE_FLOODGATE_WEST, CrossboneIsleFloodgate, CROSSBONE_ISLE_FLOODGATE_FROM_CROSSBONE_ISLE_FIFTH_FLOOR_WEST),
	SCENE_EXITS(CrossboneIsleSixthFloor),
	SCENE_EXIT(CROSSBONE_ISLE_SIXTH_FLOOR_TO_CROSSBONE_ISLE_FIFTH_FLOOR, CrossboneIsleFifthFloor, CROSSBONE_ISLE_FIFTH_FLOOR_FROM_CROSSBONE_ISLE_SIXTH_FLOOR),
	SCENE_EXIT(CROSSBONE_ISLE_SIXTH_FLOOR_TO_CROSSBONE_ISLE_SEVENTH_FLOOR, CrossboneIsleSeventhFloor, CROSSBONE_ISLE_SEVENTH_FLOOR_FROM_CROSSBONE_ISLE_SIXTH_FLOOR),
	SCENE_EXIT(CROSSBONE_ISLE_SIXTH_FLOOR_TO_CROSSBONE_ISLE_BANE_EAST, CrossboneIsleBane, CROSSBONE_ISLE_BANE_FROM_CROSSBONE_ISLE_SIXTH_FLOOR_EAST),
	SCENE_EXIT(CROSSBONE_ISLE_SIXTH_FLOOR_TO_CROSSBONE_ISLE_BANE_WEST, CrossboneIsleBane, CROSSBONE_ISLE_BANE_FROM_CROSSBONE_ISLE_SIXTH_FLOOR_WEST),
	SCENE_EXITS(CrossboneIsleSeventhFloor),
	SCENE_EXIT(CROSSBONE_ISLE_SEVENTH_FLOOR_TO_CROSSBONE_ISLE_SIXTH_FLOOR, CrossboneIsleSixthFloor, CROSSBONE_ISLE_SIXTH_FLOOR_FROM_CROSSBONE_ISLE_SEVENTH_FLOOR),
	SCENE_EXIT(CROSSBONE_ISLE_SEVENTH_FLOOR_TO_CROSSBONE_ISLE_EIGHTH_FLOOR, CrossboneIsleEighthFloor, CROSSBONE_ISLE_EIGHTH_FLOOR_FROM_CROSSBONE_ISLE_SEVENTH_FLOOR),
	SCENE_EXIT(CROSSBONE_ISLE_SEVENTH_FLOOR_TO_CROSSBONE_ISLE_STACKED_PILLARS_EAST, CrossboneIsleStackedPillars, CROSSBONE_ISLE_STACKED_PILLARS_FROM_CROSSBONE_ISLE_SEVENTH_FLOOR_EAST),
	SCENE_EXIT(CROSSBONE_ISLE_SEVENTH_FLOOR_TO_CROSSBONE_ISLE_STACKED_PILLARS_WEST, CrossboneIsleStackedPillars, CROSSBONE_ISLE_STACKED_PILLARS_FROM_CROSSBONE_ISLE_SEVENTH_FLOOR_WEST),
	SCENE_EXITS(CrossboneIsleEighthFloor),
	SCENE_EXIT(CROSSBONE_ISLE_EIGHTH_FLOOR_TO_CROSSBONE_ISLE_SEVENTH_FLOOR, CrossboneIsleSeventhFloor, CROSSBONE_ISLE_SEVENTH_FLOOR_FROM_CROSSBONE_ISLE_EIGHTH_FLOOR),
	SCENE_EXIT(CROSSBONE_ISLE_EIGHTH_FLOOR_TO_CROSSBONE_ISLE_NINTH_FLOOR, CrossboneIsleNinthFloor, CROSSBONE_ISLE_NINTH_FLOOR_FROM_CROSSBONE_ISLE_EIGHTH_FLOOR),
	SCENE_EXIT(CROSSBONE_ISLE_EIGHTH_FLOOR_TO_CROSSBONE_ISLE_DRAGON_STATUE_EAST, CrossboneIsleDragonStatue, CROSSBONE_ISLE_DRAGON_STATUE_FROM_CROSSBONE_ISLE_EIGHTH_FLOOR_EAST),
	SCENE_EXIT(CROSSBONE_ISLE_EIGHTH_FLOOR_TO_CROSSBONE_ISLE_DRAGON_STATUE_WEST, CrossboneIsleDragonStatue, CROSSBONE_ISLE_DRAGON_STATUE_FROM_CROSSBONE_ISLE_EIGHTH_FLOOR_WEST),
	SCENE_EXITS(CrossboneIsleNinthFloor),
	SCENE_EXIT(CROSSBONE_ISLE_NINTH_FLOOR_TO_CROSSBONE_ISLE_EIGHTH_FLOOR, CrossboneIsleEighthFloor, CROSSBONE_ISLE_EIGHTH_FLOOR_FROM_CROSSBONE_ISLE_NINTH_FLOOR),
	SCENE_EXIT(CROSSBONE_ISLE_NINTH_FLOOR_TO_CROSSBONE_ISLE_PIER, CrossboneIslePier, CROSSBONE_ISLE_PIER_FROM_CROSSBONE_ISLE_NINTH_FLOOR),
	SCENE_EXIT(CROSSBONE_ISLE_NINTH_FLOOR_TO_CROSSBONE_ISLE_FLOATING_LOGS_EAST, CrossboneIsleFloatingLogs, CROSSBONE_ISLE_FLOATING_LOGS_FROM_CROSSBONE_ISLE_NINTH_FLOOR_EAST),
	SCENE_EXIT(CROSSBONE_ISLE_NINTH_FLOOR_TO_CROSSBONE_ISLE_FLOATING_LOGS_WEST, CrossboneIsleFloatingLogs, CROSSBONE_ISLE_FLOATING_LOGS_FROM_CROSSBONE_ISLE_NINTH_FLOOR_WEST),
	SCENE_EXITS_END,
};

ACTOR_PLACE CrossboneIsle_NoActorPlacements[] = {
	{ SCENE_TABLE_END },
};

ACTOR_PLACE CrossboneIsle_IsleActorPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_BLANK, FLAG_CROSSBONE_ISLE_NUT_CAUGHT, MOVEMENT_TYPE_RESET_ACTION,
		344, 0, 280, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_LOG_WEST_EAST, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
		144, 0, 392, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_LOG_WEST_EAST, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
		352, 0, 376, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_SPROUT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_SPROUT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		88, 0, 368, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_SPROUT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		104, 0, 336, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_SPROUT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		152, 0, 320, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_IVY, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		136, 0, 352, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_IVY, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_IVY, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		88, 0, 312, FACING_SOUTH, TALK_FACING_KEEP, 0),
	{ SCENE_TABLE_END },
};

ACTOR_PLACE CrossboneIsle_FloatingLogsActorPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_FLOATING_LOG_WEST_EAST, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
		408, 0, 312, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_FLOATING_LOG_WEST_EAST, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
		168, 0, 312, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_FLOATING_LOG_WEST_EAST, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
		520, 0, 296, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_FLOATING_LOG_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
		584, 0, 280, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_FLOATING_LOG_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
		552, 0, 200, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_FLOATING_LOG_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
		584, 0, 136, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_FLOATING_LOG_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
		216, 0, 216, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_FLOATING_LOG_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
		392, 0, 136, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_FLOATING_LOG_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
		216, 0, 168, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_FLOATING_LOG_WEST_EAST, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
		72, 0, 312, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_FLOATING_LOG_WEST_EAST, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
		120, 0, 312, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_FLOATING_LOG_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
		216, 0, 264, FACING_SOUTH, TALK_FACING_KEEP, 0),
	{ SCENE_TABLE_END },
};

ACTOR_PLACE CrossboneIsle_FloorActorPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		632, 0, 168, FACING_SOUTH, TALK_FACING_KEEP, 0),
	{ SCENE_TABLE_END },
};

CUE CrossboneIsle_FloorCues[] = {
	{ CUE_EXIT, FLOOR_TO_PREVIOUS_ROOM, CONDITION_ALWAYS, FLOOR_TO_PREVIOUS_ROOM },
	{ CUE_EXIT, FLOOR_TO_NEXT_ROOM, CONDITION_ALWAYS, FLOOR_TO_NEXT_ROOM },
	{ STEP_CUE_WALKING_INTO(FACING_NORTH), FLOOR_TO_SIDE_ROOM_EAST, CONDITION_ALWAYS, SCRIPT_REF(scCrossboneIsleOpenEastDoor) },
	{ STEP_CUE_WALKING_INTO(FACING_NORTH), FLOOR_TO_SIDE_ROOM_WEST, CONDITION_ALWAYS, SCRIPT_REF(scCrossboneIsleOpenWestDoor) },
	{ CUE_STEP | STEP_CUE_FLAG_WALKING, METATILE_BEHAVIOR_CROSSBONE_ISLE_WEST_OF_PILLAR, CONDITION_ALWAYS, SCRIPT_REF(scCrossboneIslePushPillar) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_PILLAR, CONDITION_ALWAYS, SCRIPT_REF(scCrossboneIsleMovePillar) },
	{ EXAMINE_CUE(EXAMINE_CHEST), TREASURE_SPOT_CROSSBONE_ISLE_FIRST_FLOOR_MIMIC, FLAG_CROSSBONE_ISLE_FIRST_FLOOR_MIMIC_BEATEN, FIND_MIMIC | 4 },
	{ CUE_TABLE_END },
};

CUE CrossboneIsle_CaveEntranceCues[] = {
	{ CUE_EXIT, CROSSBONE_ISLE_CAVE_ENTRANCE_TO_CROSSBONE_ISLE, CONDITION_ALWAYS, CROSSBONE_ISLE_CAVE_ENTRANCE_TO_CROSSBONE_ISLE },
	{ CUE_EXIT, CROSSBONE_ISLE_CAVE_ENTRANCE_TO_CROSSBONE_ISLE_FIRST_FLOOR, CONDITION_ALWAYS, CROSSBONE_ISLE_CAVE_ENTRANCE_TO_CROSSBONE_ISLE_FIRST_FLOOR },
	{ CUE_TABLE_END },
};

CUE CrossboneIsle_IsleCues[] = {
	{ PSYNERGY_ON_ACTOR(ABILITY_CATCH), ACTOR_ISLE_NUT, FLAG_CROSSBONE_ISLE_NUT_CAUGHT, SCRIPT_REF(scCrossboneIsleCatchNut) },
	{ PSYNERGY_ON_ACTOR(ABILITY_GROWTH), ACTOR_FIRST_PLANT, FLAG_CROSSBONE_ISLE_FIRST_PLANT_GROWN, SCRIPT_REF(scCrossboneIsleUnusedGrowFirstPlant) },
	{ PSYNERGY_ON_ACTOR(ABILITY_GROWTH), ACTOR_FIRST_PLANT + 1, FLAG_CROSSBONE_ISLE_FIRST_PLANT_GROWN + 1, SCRIPT_REF(scCrossboneIsleGrowSecondPlant) },
	{ PSYNERGY_ON_ACTOR(ABILITY_GROWTH), ACTOR_FIRST_PLANT + 2, FLAG_CROSSBONE_ISLE_FIRST_PLANT_GROWN + 2, SCRIPT_REF(scCrossboneIsleGrowThirdPlant) },
	{ PSYNERGY_ON_ACTOR(ABILITY_GROWTH), ACTOR_FIRST_PLANT + 3, FLAG_CROSSBONE_ISLE_FIRST_PLANT_GROWN + 3, SCRIPT_REF(scCrossboneIsleGrowFourthPlant) },
	{ PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_FIRST_IVY, FLAG_CROSSBONE_ISLE_FIRST_IVY_BLOWN_AWAY, SCRIPT_REF(scCrossboneIsleBlowAwayFirstIvy) },
	{ PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_FIRST_IVY + 1, FLAG_CROSSBONE_ISLE_FIRST_IVY_BLOWN_AWAY + 1, SCRIPT_REF(scCrossboneIsleUnusedBlowAwaySecondIvy) },
	{ PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_FIRST_IVY + 2, FLAG_CROSSBONE_ISLE_FIRST_IVY_BLOWN_AWAY + 2, SCRIPT_REF(scCrossboneIsleBlowAwayThirdIvy) },
	{ PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_FIRST_IVY + 3, FLAG_CROSSBONE_ISLE_FIRST_IVY_BLOWN_AWAY + 3, SCRIPT_REF(scCrossboneIsleUnusedBlowAwayFourthIvy) },
	{ CUE_EXIT, CROSSBONE_ISLE_TO_WORLD_MAP, CONDITION_ALWAYS, CROSSBONE_ISLE_TO_WORLD_MAP },
	{ CUE_EXIT, CROSSBONE_ISLE_TO_CROSSBONE_ISLE_CAVE_ENTRANCE, CONDITION_ALWAYS, CROSSBONE_ISLE_TO_CROSSBONE_ISLE_CAVE_ENTRANCE },
	{ CUE_STEP | STEP_CUE_FLAG_WALKING, METATILE_BEHAVIOR_CROSSBONE_ISLE_BESIDE_WEST_LOG, CONDITION_ALWAYS, SCRIPT_REF(scPushedActorPushIfClear) },
	{ CUE_STEP | STEP_CUE_FLAG_WALKING, METATILE_BEHAVIOR_CROSSBONE_ISLE_BESIDE_EAST_LOG, CONDITION_ALWAYS, SCRIPT_REF(scPushedActorPushIfClear) },
	{ EXAMINE_CHEST_FACING(FACING_WEST), TREASURE_SPOT_CROSSBONE_ISLE_COIN, FLAG_CROSSBONE_ISLE_COIN, FIND_COINS(1) },
	{ EXAMINE_CHEST_FACING(FACING_NORTH), TREASURE_SPOT_CROSSBONE_ISLE_COIN, FLAG_CROSSBONE_ISLE_COIN, FIND_COINS(1) },
	{ EXAMINE_CHEST_FACING(FACING_NORTHWEST), TREASURE_SPOT_CROSSBONE_ISLE_COIN, FLAG_CROSSBONE_ISLE_COIN, FIND_COINS(1) },
	{ EXAMINE_CHEST_FACING(FACING_WEST), TREASURE_SPOT_CROSSBONE_ISLE_MINT, FLAG_CROSSBONE_ISLE_MINT, FIND_ITEM(ITEM_MINT) },
	{ EXAMINE_CHEST_FACING(FACING_NORTH), TREASURE_SPOT_CROSSBONE_ISLE_MINT, FLAG_CROSSBONE_ISLE_MINT, FIND_ITEM(ITEM_MINT) },
	{ EXAMINE_CHEST_FACING(FACING_NORTHWEST), TREASURE_SPOT_CROSSBONE_ISLE_MINT, FLAG_CROSSBONE_ISLE_MINT, FIND_ITEM(ITEM_MINT) },
	{ CUE_TABLE_END },
};

CUE CrossboneIsle_FloatingLogsCues[] = {
	{ CUE_EXIT, CROSSBONE_ISLE_FLOATING_LOGS_TO_CROSSBONE_ISLE_NINTH_FLOOR_EAST, CONDITION_ALWAYS, CROSSBONE_ISLE_FLOATING_LOGS_TO_CROSSBONE_ISLE_NINTH_FLOOR_EAST },
	{ CUE_EXIT, CROSSBONE_ISLE_FLOATING_LOGS_TO_CROSSBONE_ISLE_NINTH_FLOOR_WEST, CONDITION_ALWAYS, CROSSBONE_ISLE_FLOATING_LOGS_TO_CROSSBONE_ISLE_NINTH_FLOOR_WEST },
	{ STEP_CUE_WALKING_INTO(FACING_WEST), METATILE_BEHAVIOR_CROSSBONE_ISLE_SHORE_HOP_WEST, CONDITION_ALWAYS, SCRIPT_REF(scCrossboneIsleHopWest) },
	{ STEP_CUE_WALKING_INTO(FACING_EAST), METATILE_BEHAVIOR_CROSSBONE_ISLE_SHORE_HOP_EAST, CONDITION_ALWAYS, SCRIPT_REF(scCrossboneIsleHopEast) },
	{ STEP_CUE_WALKING_INTO(FACING_SOUTH), METATILE_BEHAVIOR_CROSSBONE_ISLE_SHORE_HOP_SOUTH, CONDITION_ALWAYS, SCRIPT_REF(scCrossboneIsleHopSouth) },
	{ STEP_CUE_WALKING_INTO(FACING_NORTH), METATILE_BEHAVIOR_CROSSBONE_ISLE_SHORE_HOP_NORTH, CONDITION_ALWAYS, SCRIPT_REF(scCrossboneIsleHopNorth) },
	{ STEP_CUE_WALKING_INTO(FACING_NORTH), METATILE_BEHAVIOR_CROSSBONE_ISLE_LOG_A, CONDITION_ALWAYS, SCRIPT_REF(scCrossboneIsleRideLogANorth) },
	{ STEP_CUE_WALKING_INTO(FACING_SOUTH), METATILE_BEHAVIOR_CROSSBONE_ISLE_LOG_A, CONDITION_ALWAYS, SCRIPT_REF(scCrossboneIsleRideLogASouth) },
	{ STEP_CUE_WALKING_INTO(FACING_EAST), METATILE_BEHAVIOR_CROSSBONE_ISLE_LOG_A, CONDITION_ALWAYS, SCRIPT_REF(scCrossboneIsleHopEast) },
	{ STEP_CUE_WALKING_INTO(FACING_WEST), METATILE_BEHAVIOR_CROSSBONE_ISLE_LOG_A, CONDITION_ALWAYS, SCRIPT_REF(scCrossboneIsleHopWest) },
	{ STEP_CUE_WALKING_INTO(FACING_NORTH), METATILE_BEHAVIOR_CROSSBONE_ISLE_LOG_C, CONDITION_ALWAYS, SCRIPT_REF(scCrossboneIsleRideLogCNorth) },
	{ STEP_CUE_WALKING_INTO(FACING_SOUTH), METATILE_BEHAVIOR_CROSSBONE_ISLE_LOG_C, CONDITION_ALWAYS, SCRIPT_REF(scCrossboneIsleRideLogCSouth) },
	{ STEP_CUE_WALKING_INTO(FACING_EAST), METATILE_BEHAVIOR_CROSSBONE_ISLE_LOG_C, CONDITION_ALWAYS, SCRIPT_REF(scCrossboneIsleHopEast) },
	{ STEP_CUE_WALKING_INTO(FACING_WEST), METATILE_BEHAVIOR_CROSSBONE_ISLE_LOG_C, CONDITION_ALWAYS, SCRIPT_REF(scCrossboneIsleHopWest) },
	{ STEP_CUE_WALKING_INTO(FACING_WEST), METATILE_BEHAVIOR_CROSSBONE_ISLE_LOG_D, CONDITION_ALWAYS, SCRIPT_REF(scCrossboneIsleRideLogDWest) },
	{ STEP_CUE_WALKING_INTO(FACING_EAST), METATILE_BEHAVIOR_CROSSBONE_ISLE_LOG_D, CONDITION_ALWAYS, SCRIPT_REF(scCrossboneIsleRideLogDEast) },
	{ STEP_CUE_WALKING_INTO(FACING_NORTH), METATILE_BEHAVIOR_CROSSBONE_ISLE_LOG_D, CONDITION_ALWAYS, SCRIPT_REF(scCrossboneIsleHopNorth) },
	{ STEP_CUE_WALKING_INTO(FACING_SOUTH), METATILE_BEHAVIOR_CROSSBONE_ISLE_LOG_D, CONDITION_ALWAYS, SCRIPT_REF(scCrossboneIsleHopSouth) },
	{ STEP_CUE_WALKING_INTO(FACING_WEST), METATILE_BEHAVIOR_CROSSBONE_ISLE_LOG_E, CONDITION_ALWAYS, SCRIPT_REF(scCrossboneIsleRideLogEWest) },
	{ STEP_CUE_WALKING_INTO(FACING_EAST), METATILE_BEHAVIOR_CROSSBONE_ISLE_LOG_E, CONDITION_ALWAYS, SCRIPT_REF(scCrossboneIsleRideLogEEast) },
	{ STEP_CUE_WALKING_INTO(FACING_NORTH), METATILE_BEHAVIOR_CROSSBONE_ISLE_LOG_E, CONDITION_ALWAYS, SCRIPT_REF(scCrossboneIsleHopNorth) },
	{ STEP_CUE_WALKING_INTO(FACING_SOUTH), METATILE_BEHAVIOR_CROSSBONE_ISLE_LOG_E, CONDITION_ALWAYS, SCRIPT_REF(scCrossboneIsleHopSouth) },
	{ STEP_CUE_WALKING_INTO(FACING_WEST), METATILE_BEHAVIOR_CROSSBONE_ISLE_LOG_F, CONDITION_ALWAYS, SCRIPT_REF(scCrossboneIsleRideLogFWest) },
	{ STEP_CUE_WALKING_INTO(FACING_EAST), METATILE_BEHAVIOR_CROSSBONE_ISLE_LOG_F, CONDITION_ALWAYS, SCRIPT_REF(scCrossboneIsleRideLogFEast) },
	{ STEP_CUE_WALKING_INTO(FACING_NORTH), METATILE_BEHAVIOR_CROSSBONE_ISLE_LOG_F, CONDITION_ALWAYS, SCRIPT_REF(scCrossboneIsleHopNorth) },
	{ STEP_CUE_WALKING_INTO(FACING_SOUTH), METATILE_BEHAVIOR_CROSSBONE_ISLE_LOG_F, CONDITION_ALWAYS, SCRIPT_REF(scCrossboneIsleHopSouth) },
	{ STEP_CUE_WALKING_INTO(FACING_WEST), METATILE_BEHAVIOR_CROSSBONE_ISLE_LOG_H, CONDITION_ALWAYS, SCRIPT_REF(scCrossboneIsleRideLogHWest) },
	{ STEP_CUE_WALKING_INTO(FACING_EAST), METATILE_BEHAVIOR_CROSSBONE_ISLE_LOG_H, CONDITION_ALWAYS, SCRIPT_REF(scCrossboneIsleRideLogHEast) },
	{ STEP_CUE_WALKING_INTO(FACING_NORTH), METATILE_BEHAVIOR_CROSSBONE_ISLE_LOG_H, CONDITION_ALWAYS, SCRIPT_REF(scCrossboneIsleHopNorth) },
	{ STEP_CUE_WALKING_INTO(FACING_SOUTH), METATILE_BEHAVIOR_CROSSBONE_ISLE_LOG_H, CONDITION_ALWAYS, SCRIPT_REF(scCrossboneIsleHopSouth) },
	{ STEP_CUE_WALKING_INTO(FACING_NORTH), METATILE_BEHAVIOR_CROSSBONE_ISLE_LOG_J, CONDITION_ALWAYS, SCRIPT_REF(scCrossboneIsleRideLogJNorth) },
	{ STEP_CUE_WALKING_INTO(FACING_SOUTH), METATILE_BEHAVIOR_CROSSBONE_ISLE_LOG_J, CONDITION_ALWAYS, SCRIPT_REF(scCrossboneIsleRideLogJSouth) },
	{ STEP_CUE_WALKING_INTO(FACING_EAST), METATILE_BEHAVIOR_CROSSBONE_ISLE_LOG_J, CONDITION_ALWAYS, SCRIPT_REF(scCrossboneIsleHopEast) },
	{ STEP_CUE_WALKING_INTO(FACING_WEST), METATILE_BEHAVIOR_CROSSBONE_ISLE_LOG_J, CONDITION_ALWAYS, SCRIPT_REF(scCrossboneIsleHopWest) },
	{ STEP_CUE_WALKING_INTO(FACING_NORTH), METATILE_BEHAVIOR_CROSSBONE_ISLE_LOG_K, CONDITION_ALWAYS, SCRIPT_REF(scCrossboneIsleRideLogKNorth) },
	{ STEP_CUE_WALKING_INTO(FACING_SOUTH), METATILE_BEHAVIOR_CROSSBONE_ISLE_LOG_K, CONDITION_ALWAYS, SCRIPT_REF(scCrossboneIsleRideLogKSouth) },
	{ STEP_CUE_WALKING_INTO(FACING_EAST), METATILE_BEHAVIOR_CROSSBONE_ISLE_LOG_K, CONDITION_ALWAYS, SCRIPT_REF(scCrossboneIsleHopEast) },
	{ STEP_CUE_WALKING_INTO(FACING_WEST), METATILE_BEHAVIOR_CROSSBONE_ISLE_LOG_K, CONDITION_ALWAYS, SCRIPT_REF(scCrossboneIsleHopWest) },
	{ STEP_CUE_WALKING_INTO(FACING_NORTH), METATILE_BEHAVIOR_CROSSBONE_ISLE_LOG_B, CONDITION_ALWAYS, SCRIPT_REF(scCrossboneIsleRideLogBNorth) },
	{ STEP_CUE_WALKING_INTO(FACING_SOUTH), METATILE_BEHAVIOR_CROSSBONE_ISLE_LOG_B, CONDITION_ALWAYS, SCRIPT_REF(scCrossboneIsleRideLogBSouth) },
	{ STEP_CUE_WALKING_INTO(FACING_EAST), METATILE_BEHAVIOR_CROSSBONE_ISLE_LOG_B, CONDITION_ALWAYS, SCRIPT_REF(scCrossboneIsleHopEast) },
	{ STEP_CUE_WALKING_INTO(FACING_WEST), METATILE_BEHAVIOR_CROSSBONE_ISLE_LOG_B, CONDITION_ALWAYS, SCRIPT_REF(scCrossboneIsleHopWest) },
	{ STEP_CUE_WALKING_INTO(FACING_WEST), METATILE_BEHAVIOR_CROSSBONE_ISLE_LOG_L, CONDITION_ALWAYS, SCRIPT_REF(scCrossboneIsleRideLogLWest) },
	{ STEP_CUE_WALKING_INTO(FACING_EAST), METATILE_BEHAVIOR_CROSSBONE_ISLE_LOG_L, CONDITION_ALWAYS, SCRIPT_REF(scCrossboneIsleRideLogLEast) },
	{ STEP_CUE_WALKING_INTO(FACING_NORTH), METATILE_BEHAVIOR_CROSSBONE_ISLE_LOG_L, CONDITION_ALWAYS, SCRIPT_REF(scCrossboneIsleHopNorth) },
	{ STEP_CUE_WALKING_INTO(FACING_SOUTH), METATILE_BEHAVIOR_CROSSBONE_ISLE_LOG_L, CONDITION_ALWAYS, SCRIPT_REF(scCrossboneIsleHopSouth) },
	{ STEP_CUE_WALKING_INTO(FACING_WEST), METATILE_BEHAVIOR_CROSSBONE_ISLE_LOG_G, CONDITION_ALWAYS, SCRIPT_REF(scCrossboneIsleRideLogGWest) },
	{ STEP_CUE_WALKING_INTO(FACING_EAST), METATILE_BEHAVIOR_CROSSBONE_ISLE_LOG_G, CONDITION_ALWAYS, SCRIPT_REF(scCrossboneIsleRideLogGEast) },
	{ STEP_CUE_WALKING_INTO(FACING_NORTH), METATILE_BEHAVIOR_CROSSBONE_ISLE_LOG_G, CONDITION_ALWAYS, SCRIPT_REF(scCrossboneIsleHopNorth) },
	{ STEP_CUE_WALKING_INTO(FACING_SOUTH), METATILE_BEHAVIOR_CROSSBONE_ISLE_LOG_G, CONDITION_ALWAYS, SCRIPT_REF(scCrossboneIsleHopSouth) },
	{ STEP_CUE_WALKING_INTO(FACING_WEST), METATILE_BEHAVIOR_CROSSBONE_ISLE_LOG_I, CONDITION_ALWAYS, SCRIPT_REF(scCrossboneIsleRideLogIWest) },
	{ STEP_CUE_WALKING_INTO(FACING_EAST), METATILE_BEHAVIOR_CROSSBONE_ISLE_LOG_I, CONDITION_ALWAYS, SCRIPT_REF(scCrossboneIsleRideLogIEast) },
	{ STEP_CUE_WALKING_INTO(FACING_NORTH), METATILE_BEHAVIOR_CROSSBONE_ISLE_LOG_I, CONDITION_ALWAYS, SCRIPT_REF(scCrossboneIsleHopNorth) },
	{ STEP_CUE_WALKING_INTO(FACING_SOUTH), METATILE_BEHAVIOR_CROSSBONE_ISLE_LOG_I, CONDITION_ALWAYS, SCRIPT_REF(scCrossboneIsleHopSouth) },
	{ CUE_STEP, METATILE_BEHAVIOR_CROSSBONE_ISLE_LOG_A, CONDITION_ALWAYS, SCRIPT_REF(scCrossboneIsleStepOnLogA) },
	{ CUE_STEP, METATILE_BEHAVIOR_CROSSBONE_ISLE_LOG_C, CONDITION_ALWAYS, SCRIPT_REF(scCrossboneIsleStepOnLogC) },
	{ CUE_STEP, METATILE_BEHAVIOR_CROSSBONE_ISLE_LOG_D, CONDITION_ALWAYS, SCRIPT_REF(scCrossboneIsleStepOnLogD) },
	{ CUE_STEP, METATILE_BEHAVIOR_CROSSBONE_ISLE_LOG_E, CONDITION_ALWAYS, SCRIPT_REF(scCrossboneIsleStepOnLogE) },
	{ CUE_STEP, METATILE_BEHAVIOR_CROSSBONE_ISLE_LOG_F, CONDITION_ALWAYS, SCRIPT_REF(scCrossboneIsleStepOnLogF) },
	{ CUE_STEP, METATILE_BEHAVIOR_CROSSBONE_ISLE_LOG_H, CONDITION_ALWAYS, SCRIPT_REF(scCrossboneIsleStepOnLogH) },
	{ CUE_STEP, METATILE_BEHAVIOR_CROSSBONE_ISLE_LOG_J, CONDITION_ALWAYS, SCRIPT_REF(scCrossboneIsleStepOnLogJ) },
	{ CUE_STEP, METATILE_BEHAVIOR_CROSSBONE_ISLE_LOG_K, CONDITION_ALWAYS, SCRIPT_REF(scCrossboneIsleStepOnLogK) },
	{ CUE_STEP, METATILE_BEHAVIOR_CROSSBONE_ISLE_LOG_B, CONDITION_ALWAYS, SCRIPT_REF(scCrossboneIsleStepOnLogB) },
	{ CUE_STEP, METATILE_BEHAVIOR_CROSSBONE_ISLE_LOG_L, CONDITION_ALWAYS, SCRIPT_REF(scCrossboneIsleStepOnLogL) },
	{ CUE_STEP, METATILE_BEHAVIOR_CROSSBONE_ISLE_LOG_G, CONDITION_ALWAYS, SCRIPT_REF(scCrossboneIsleStepOnLogG) },
	{ CUE_STEP, METATILE_BEHAVIOR_CROSSBONE_ISLE_LOG_I, CONDITION_ALWAYS, SCRIPT_REF(scCrossboneIsleStepOnLogI) },
	{ EXAMINE_CUE(EXAMINE_CHEST), TREASURE_SPOT_CROSSBONE_ISLE_FLOATING_LOGS_999_COINS, FLAG_CROSSBONE_ISLE_FLOATING_LOGS_999_COINS, FIND_COINS(999) },
	{ EXAMINE_CUE(EXAMINE_CHEST), TREASURE_SPOT_CROSSBONE_ISLE_FLOATING_LOGS_WATER_OF_LIFE, FLAG_CROSSBONE_ISLE_FLOATING_LOGS_WATER_OF_LIFE, FIND_ITEM(ITEM_WATER_OF_LIFE) },
	{ EXAMINE_CUE(EXAMINE_CHEST), TREASURE_SPOT_CROSSBONE_ISLE_FLOATING_LOGS_MURAMASA, FLAG_CROSSBONE_ISLE_FLOATING_LOGS_MURAMASA, FIND_ITEM(ITEM_MURAMASA) },
	{ EXAMINE_CUE(EXAMINE_CHEST), TREASURE_SPOT_CROSSBONE_ISLE_FLOATING_LOGS_SLEEP_BOMB, FLAG_CROSSBONE_ISLE_FLOATING_LOGS_SLEEP_BOMB, FIND_ITEM(ITEM_SLEEP_BOMB) },
	{ CUE_TABLE_END },
};
/* clang-format on */

CUE CrossboneIsle_PierCues[] = {
	{CUE_EXIT, CROSSBONE_ISLE_PIER_TO_CROSSBONE_ISLE_NINTH_FLOOR,
		CONDITION_ALWAYS, CROSSBONE_ISLE_PIER_TO_CROSSBONE_ISLE_NINTH_FLOOR},
	{CUE_EXIT, CROSSBONE_ISLE_PIER_TO_PIRATE_SHIP, CONDITION_ALWAYS,
		CROSSBONE_ISLE_PIER_TO_PIRATE_SHIP},
	{CUE_TABLE_END},
};

CUE CrossboneIsle_PirateShipCues[] = {
	{CUE_EXIT, PIRATE_SHIP_TO_CROSSBONE_ISLE_PIER, CONDITION_ALWAYS,
		PIRATE_SHIP_TO_CROSSBONE_ISLE_PIER},
	{CUE_STEP, METATILE_BEHAVIOR_PIRATE_SHIP_TOP_OF_WEST_STAIRS,
		FLAG_CROSSBONE_ISLE_DEADBEARD_DEFEATED,
		SCRIPT_REF(scCrossboneIsleFightDeadbeard)},
	{STEP_CUE_WALKING_INTO(FACING_SOUTH),
		METATILE_BEHAVIOR_PIRATE_SHIP_BESIDE_MIMIC, CONDITION_ALWAYS,
		SCRIPT_REF(scCrossboneIsleHopSouth)},
	{EXAMINE_CUE(EXAMINE_CHEST), TREASURE_SPOT_PIRATE_SHIP_DEMON_MAIL,
		FLAG_PIRATE_SHIP_DEMON_MAIL, FIND_ITEM(ITEM_DEMON_MAIL)},
	{EXAMINE_CUE(EXAMINE_CHEST), TREASURE_SPOT_PIRATE_SHIP_MIMIC,
		FLAG_PIRATE_SHIP_MIMIC_BEATEN, FIND_MIMIC | 7},
	{CUE_TABLE_END},
};
