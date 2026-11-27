/* Three of the side rooms of Crossbone Isle Cave, the cave beneath Crossbone
   Isle, with four chests each.

   Each opens off one of the nine floors the cave descends through
   (crossbone_isle.c), as the other five do (crossbone_isle_stepping_stones.c).
   The party comes in by the floor's east door and the room is the way across to
   its west door. All three take the area name Crossbone Isle Cave. In the order
   the party meets them:

   - The room of keys (SceneIdCrossboneIsleKeys), off the third floor: a
	 pit full of square stone posts, with a walkway along its north wall.
	 Four blue doors in a row along the walkway lead to four small rooms with
	 a chest each: 333 coins, the Fairy Ring, a Cookie and a Smoke Bomb, west
	 to east. A red door on a ledge west of the pit is the way on to the
	 floor's west door.

	 Four Blue Keys lie on top of posts out of reach, and Catch pulls each
	 one in. Walking up to a blue door with a Blue Key opens it and uses the
	 key up. From the walkway in front of the fourth door Isaac can hop down
	 onto the posts, and Reveal there shows a hidden post with the Red Key
	 on it, which Catch takes too. The Red Key opens the red door the same
	 way. The keys taken and the doors opened stay so on later visits.

   - The statue room (SceneIdCrossboneIsleStatues), off the fourth floor: a
	 dark hall where five statues of a robed woman stand in circles of light
	 round braziers.

	 The statue in the north-east, the one on the narrow walk in the west
	 and the one in the middle each guard the few tiles in front of them.
	 Stepping onto those tiles makes the statue glide onto Isaac with a thud
	 and shove him back south, then glide home, unless Cloak hides him. The
	 two statues in the alcoves on the south walkway near the east door slide
	 sideways to stay in front of Isaac as he walks past, so that he cannot
	 get by them. Halt stops a statue where it is, and while Reveal lasts the
	 darkness lifts from the whole hall.

   - Bane's room (SceneIdCrossboneIsleBane), off the sixth floor, where
	 Bane, a Venus Djinni, waits at the west end of the ledge along the back
	 wall. Five stone pillars to push or shift with Move, the ladders and a
	 puddle that Frost freezes into a pillar of ice make the way across the
	 room. A chest on one pillar holds the Demon Axe.

	 The first two times Isaac comes near, Bane runs off along the room. The
	 third time it flees to a ledge in the south-east with three spikes on
	 it, and there it keeps running round the spikes away from him until
	 Halt holds it still. Talked to while it is held, Bane joins the party. */

#include "ability_ids.h"
#include "flags.h"
#include "item_ids.h"
#include "battle/battle.h"
#include "debug/debug.h"
#include "field/script_inline.h"
#include "field/field_effect.h"
#include "graphics/graphics.h"

extern Sint32		  CrossboneIsleKeysBaneSquashScript[];
extern Sint32		  CrossboneIsleKeysBaneWalkTargets[][2];
extern SCENE_ENTRANCE CrossboneIsleKeysRoomEntrances[];
extern SCENE_ENTRANCE CrossboneIsleStatuesEntrances[];
extern SCENE_ENTRANCE CrossboneIsleBaneEntrances[];
extern Uint32		  CrossboneIsleKeysExits[];
extern ACTOR_PLACE	  CrossboneIsleKeysActorPlacementsOther[];
extern ACTOR_PLACE	  CrossboneIsleKeysRoomActorPlacements[];
extern ACTOR_PLACE	  CrossboneIsleStatuesActorPlacements[];
extern ACTOR_PLACE	  CrossboneIsleBaneActorPlacements[];
extern CUE			  CrossboneIsleKeysCuesOther[];
extern CUE			  CrossboneIsleKeysRoomCues[];
extern CUE			  CrossboneIsleStatuesCues[];
extern CUE			  CrossboneIsleBaneCues[];
extern Uint32		  CrossboneIsleKeysStatueColumns[];
extern Sint32		  CrossboneIsleKeysIconTimer;

/* Bane's room: the pillar on the raised platform in the south-west has been
   pushed off it and stands lower down, until the party leaves the cave's area
   (an area flag, which other areas use for their own). */
#define FLAG_CROSSBONE_ISLE_THIRD_PILLAR_LOWERED FLAG_AREA_TEMP_00

/* How far, in pixels, the first statue shoves Isaac back, the same in every
   edition. */
#define FIRST_STATUE_SHOVE_PIXELS 24

/* How far, in pixels, the first statue follows Isaac south as it shoves him
   back, and how far each of the others shoves him; the two are swapped outside
   the Japanese edition. */
#define FIRST_STATUE_FOLLOW_PIXELS \
	WHEN(CROSSBONE_ISLE_STATUES_SHOVE_ALIKE, 16, 24)
#define STATUE_SHOVE_PIXELS WHEN(CROSSBONE_ISLE_STATUES_SHOVE_ALIKE, 24, 16)

static void scCrossboneIsleKeysPushFirstPillar(void);
static void scCrossboneIsleKeysFollowSecondPillar(void);
static void scCrossboneIsleKeysPosSecondPillarMarker(void);
static void scCrossboneIsleKeysInitKeys(void);
static void scCrossboneIsleKeysInitStatues(void);
static void scCrossboneIsleKeysInitBane(void);

/* The room of keys' placed actors, numbered in the order of
   CrossboneIsleKeysRoomActorPlacements.

   They are the keys, each drawn as its item's icon on top of a stone post
   (scColossoShowItemIcon) until Catch takes it. */
typedef enum CROSSBONE_ISLE_KEYS_ACTOR_ID {
	ACTOR_FIRST_BLUE_KEY = ACTOR_FIRST_PLACED,
	ACTOR_SECOND_BLUE_KEY,
	ACTOR_THIRD_BLUE_KEY,
	ACTOR_FOURTH_BLUE_KEY,
	/* Shown only while Reveal lasts. */
	ACTOR_RED_KEY
} CROSSBONE_ISLE_KEYS_ACTOR_ID;

/* The statue room's placed actors, numbered in the order of
   CrossboneIsleStatuesActorPlacements.

   They are the five statues of a robed woman, all of one sprite, each waiting
   at its home in a circle of light. */
typedef enum CROSSBONE_ISLE_STATUES_ACTOR_ID {
	/* In the north-east, beside the chest with the Storm Gear. */
	ACTOR_FIRST_STATUE = ACTOR_FIRST_PLACED,
	/* On the narrow walk between two dark pits in the west. */
	ACTOR_SECOND_STATUE,
	/* In the west alcove of the south walkway, barring the way north to the
	   chest with the Sleep Bomb. */
	ACTOR_THIRD_STATUE,
	/* In the east alcove of the south walkway. */
	ACTOR_FOURTH_STATUE,
	/* In the middle of the hall, south of the chest with 444 coins. */
	ACTOR_FIFTH_STATUE
} CROSSBONE_ISLE_STATUES_ACTOR_ID;

/* The scripts the statue room's per-tick checks raise when Isaac steps onto
   the tiles in front of the first, second or fifth statue.

   The third and fourth statues answer metatiles of their own instead
   (CROSSBONE_ISLE_STATUES_METATILE_BEHAVIOR). */
typedef enum CROSSBONE_ISLE_STATUES_RAISED_CUE {
	RAISED_FIRST_CHASE = 91,
	RAISED_SECOND_CHASE = 92,
	RAISED_FIFTH_CHASE = 96
} CROSSBONE_ISLE_STATUES_RAISED_CUE;

/* Bane's room's placed actors, numbered in the order of
   CrossboneIsleBaneActorPlacements. */
typedef enum CROSSBONE_ISLE_BANE_ACTOR_ID {
	/* The five stone pillars, in Goma Cave's pillar sprite, pushed by walking
	   into them or with Move. The first stands on the low ledge in the
	   north-west, below where Bane first waits, at the top of a short
	   ladder. */
	ACTOR_FIRST_PILLAR = ACTOR_FIRST_PLACED,
	/* In the channel between two rows of spikes in the middle of the room, with
	   the Demon Axe's chest on top. */
	ACTOR_SECOND_PILLAR,
	/* On the raised platform in the south-west. */
	ACTOR_THIRD_PILLAR,
	/* The west one of the two side by side in the south-east, by the east
	   doorway. */
	ACTOR_FOURTH_PILLAR,
	/* The east one of the two. */
	ACTOR_FIFTH_PILLAR,
	/* The puddle between two spikes in the south-west, frozen by Frost into a
	   pillar of ice. */
	ACTOR_PUDDLE,
	/* The chest on top of the second pillar, holding the Demon Axe.

	   It is drawn in the sprite of McCoy's Palace's chests. It rides on the
	   pillar until the pillar reaches the top of its channel, by the ladders,
	   where the chest's examine cue, TREASURE_SPOT_DEMON_AXE, takes its place.
	   It is drawn open once FLAG_CROSSBONE_ISLE_DEMON_AXE is set. */
	ACTOR_SECOND_PILLAR_CHEST,
	/* Bane, a Venus Djinni, until it is caught. */
	ACTOR_BANE
} CROSSBONE_ISLE_BANE_ACTOR_ID;

/* The room of keys' metatiles that run its scripts, from
   crossbone_isle_keys_and_bane's map.txt. */
typedef enum CROSSBONE_ISLE_KEYS_METATILE_BEHAVIOR {
	/* In front of the red door, metatile (6, 52). */
	METATILE_BEHAVIOR_KEYS_RED_DOOR = 50,
	/* In front of each blue door on the walkway, west to east: metatiles (13,
	   43), (17, 43), (21, 43) and (25, 43). */
	METATILE_BEHAVIOR_KEYS_FIRST_BLUE_DOOR = 51,
	METATILE_BEHAVIOR_KEYS_SECOND_BLUE_DOOR = 52,
	METATILE_BEHAVIOR_KEYS_THIRD_BLUE_DOOR = 53,
	METATILE_BEHAVIOR_KEYS_FOURTH_BLUE_DOOR = 54
} CROSSBONE_ISLE_KEYS_METATILE_BEHAVIOR;

/* The statue room's metatiles that run the third and fourth statues' scripts,
   from crossbone_isle_statues's map.txt. */
typedef enum CROSSBONE_ISLE_STATUES_METATILE_BEHAVIOR {
	/* The rug in front of the third statue's alcove, metatiles (15..17, 23). */
	METATILE_BEHAVIOR_STATUES_BEFORE_THIRD_STATUE = 53,
	/* The rug in front of the fourth statue's alcove, metatiles (20..22,
	   23). */
	METATILE_BEHAVIOR_STATUES_BEFORE_FOURTH_STATUE = 54,
	/* The third statue's alcove, metatiles (15..17, 22). */
	METATILE_BEHAVIOR_STATUES_THIRD_ALCOVE = 61,
	/* The fourth statue's alcove, metatiles (20..22, 22). */
	METATILE_BEHAVIOR_STATUES_FOURTH_ALCOVE = 62
} CROSSBONE_ISLE_STATUES_METATILE_BEHAVIOR;

/* Bane's room's metatiles that run its scripts, from
   crossbone_isle_keys_and_bane's map.txt (the room is the east half of the map
   the room of keys shares). */
typedef enum CROSSBONE_ISLE_BANE_METATILE_BEHAVIOR {
	/* On the back ledge, metatile (40, 41). */
	METATILE_BEHAVIOR_BANE_FIRST_ESCAPE = 20,
	/* In the passage down the west side, metatile (36, 46). */
	METATILE_BEHAVIOR_BANE_SECOND_ESCAPE = 21,
	/* On the back ledge towards its east end, metatile (51, 41). */
	METATILE_BEHAVIOR_BANE_LAST_ESCAPE = 22,
	/* The first of the twelve metatiles round the three spikes on the
	   south-east ledge, (50..55, 51..54), one for each row of
	   CrossboneIsleKeysBaneWalkTargets. */
	METATILE_BEHAVIOR_BANE_CORNER_FIRST = 30,
	/* The south-west platform and the floor below it, where the third pillar is
	   pushed. */
	METATILE_BEHAVIOR_BANE_THIRD_PILLAR = 50,
	/* The first pillar's metatile and those it is pushed through: the low ledge
	   in the north-west, (36..41, 42), and the short ladder at (38, 41..43). */
	METATILE_BEHAVIOR_BANE_FIRST_PILLAR = 51,
	/* The spiked channel the second pillar is pushed up, (44..46, 43..49). */
	METATILE_BEHAVIOR_BANE_SECOND_PILLAR = 52,
	/* The row the fourth and fifth pillars are pushed along, (48..52, 55). */
	METATILE_BEHAVIOR_BANE_FOURTH_PILLAR = 53,
	/* The south edge of the south-west platform, metatile (38, 54). */
	METATILE_BEHAVIOR_BANE_THIRD_PILLAR_EDGE = 54,
	/* The foot and top of the two ladders down from the back ledge, either side
	   of the spiked channel: (42, 41..42) and (48, 41..42). */
	METATILE_BEHAVIOR_BANE_LADDER = 60,
	/* West of the first pillar's home, metatile (37, 42). */
	METATILE_BEHAVIOR_BANE_WEST_OF_FIRST_PILLAR = 61,
	/* East of the first pillar's home, metatile (39, 42). */
	METATILE_BEHAVIOR_BANE_EAST_OF_FIRST_PILLAR = 70
} CROSSBONE_ISLE_BANE_METATILE_BEHAVIOR;

/* Spare metatiles beside Bane's room, (57..64, 36..42), whose attributes the
   pillars' and the Demon Axe chest's moves copy into the room. */
typedef enum CROSSBONE_ISLE_BANE_SPARE_METATILE {
	/* The first pillar's top, level with the back ledge. */
	RAISED_PILLAR_TOP_COLUMN = 61,
	RAISED_PILLAR_TOP_ROW = 36,
	/* The low ledge's three metatiles round the first pillar's home, (37..39,
	   42), as the map lays them. */
	LOW_LEDGE_COLUMN = 62,
	LOW_LEDGE_ROW = 37,
	/* The gap's metatiles with no pillar in them: its floor, and the west
	   ladder's foot, (42, 42). */
	EMPTY_GAP_COLUMN = 57,
	EMPTY_LADDER_FOOT_COLUMN = 58,
	EMPTY_GAP_ROW = 42,
	/* The ladder's foot raised by the first pillar standing in it. */
	RAISED_LADDER_FOOT_COLUMN = 58,
	RAISED_LADDER_FOOT_ROW = 41,
	/* The Demon Axe chest's metatile, searched as TREASURE_SPOT_DEMON_AXE. */
	DEMON_AXE_CHEST_COLUMN = 58,
	DEMON_AXE_CHEST_ROW = 36
} CROSSBONE_ISLE_BANE_SPARE_METATILE;

/* The doors' pictures, kept on map layer 1 (from the map's column 64): a blue
   door half open and open, and the red door half open and open, each one
   metatile by three. The blue doors are drawn at (77 + 4n, 40), that layer's
   (13 + 4n, 40), over their metatiles, and the red door at (70, 49). */
typedef enum CROSSBONE_ISLE_KEYS_DOOR_PICTURE {
	BLUE_DOOR_HALF_OPEN_COLUMN = 79,
	BLUE_DOOR_OPEN_COLUMN = 80,
	RED_DOOR_HALF_OPEN_COLUMN = 84,
	RED_DOOR_OPEN_COLUMN = 85,
	DOOR_PICTURE_ROW = 29
} CROSSBONE_ISLE_KEYS_DOOR_PICTURE;

/* What scCrossboneIsleKeysOpenBlueDoor is given: the door's number in the low
   byte, and PASSAGE_PLAY_OPENING to play its opening. */
enum { PASSAGE_INDEX_MASK = 0xff, PASSAGE_PLAY_OPENING = 0x100 };

/* The rooms' chests, named for what each holds; each room numbers its own. */
enum {
	TREASURE_SPOT_KEYS_333_COINS = TREASURE_SPOT_FIRST_ID,
	TREASURE_SPOT_KEYS_FAIRY_RING,
	TREASURE_SPOT_KEYS_COOKIE,
	TREASURE_SPOT_KEYS_SMOKE_BOMB,
	TREASURE_SPOT_STATUES_444_COINS = TREASURE_SPOT_FIRST_ID,
	TREASURE_SPOT_STATUES_SLEEP_BOMB,
	TREASURE_SPOT_STATUES_STORM_GEAR,
	TREASURE_SPOT_STATUES_PSY_CRYSTAL,
	TREASURE_SPOT_BANE_666_COINS = TREASURE_SPOT_FIRST_ID + 4,
	TREASURE_SPOT_BANE_WATER_OF_LIFE,
	TREASURE_SPOT_BANE_ELIXIR,
	/* The Demon Axe's chest, a treasure spot the code places where the second
	   pillar stands. */
	TREASURE_SPOT_DEMON_AXE
};

/* Bane's room: Frost has frozen the puddle into a pillar of ice. */
#define FLAG_CROSSBONE_ISLE_BANE_PUDDLE_FROZEN			 FLAG_SCENE_TEMP_00
/* Bane's room: the chest on the second pillar has reached the top of the spiked
   channel, by the ladders. */
#define FLAG_CROSSBONE_ISLE_BANE_CHEST_AT_LADDER		 FLAG_SCENE_TEMP_06
/* Bane's room: Isaac last left a ladder by hopping east off it, rather than
   climbing up it. While it is set, the chest's metatile at the top of the
   spiked channel, (45, 43), answers the Demon Axe's examine cue. */
#define FLAG_CROSSBONE_ISLE_BANE_JUMPED_EAST_FROM_LADDER FLAG_SCENE_TEMP_07
/* The Move scripts of Bane's room's pillars also test FLAG_SCENE_TEMP_01 and 03
   to 05, which nothing sets. The statue room: a statue is after Isaac. */
#define FLAG_CROSSBONE_ISLE_STATUE_AFTER_LEADER			 FLAG_SCENE_TEMP_20

/* Raises a script for the cue table to run on the next tick. */
static inline void scCrossboneIsleKeysRaiseScript(
	Sint16 *raisedCue, Sint32 event)
{
	*raisedCue = event;
}

/* Stands the first pillar on the low ledge in the north-west, in the given
   column, and makes its top ground level with the back ledge, so that Isaac can
   walk onto it from there. */
static inline void scCrossboneIsleKeysLiftFirstPillar(
	Sint32 column, Uint8 motionFlg)
{
	fdMapGridCopyMetatileAttributes(
		RAISED_PILLAR_TOP_COLUMN, RAISED_PILLAR_TOP_ROW, 1, 1, column, 42);
	fdActorGet(ACTOR_FIRST_PILLAR)->motionFlg = motionFlg;
	fdActorGet(ACTOR_FIRST_PILLAR)->y.fixed = toFIXED(32);
}

#include "shared/pushed_actor.c"

/* Talking to Bane on the spiked ledge in the south-east of Bane's room while
   Halt holds it still: Bane, a Venus Djinni, joins the party. */
static void scCrossboneIsleKeysBaneJoins(void)
{
	fdDjinnJoinParty(ACTOR_BANE, ELEMENT_VENUS, DJINNI_INDEX(DJINNI_BANE));
}

/* Unused: an empty function nothing refers to. */
UNUSED static void scCrossboneIsleKeysUnusedNoOperationA(void)
{
}

const SCENE_ENTRANCE *scGetEntrances(void)
{
	Sint16 selector = MainRoutineStateWork.next;

	if(selector == SCENE_CROSSBONE_ISLE_KEYS) {
		return CrossboneIsleKeysRoomEntrances;
	}
	if(selector == SCENE_CROSSBONE_ISLE_STATUES) {
		return CrossboneIsleStatuesEntrances;
	}
	if(selector == SCENE_CROSSBONE_ISLE_BANE) {
		return CrossboneIsleBaneEntrances;
	}
	return SceneFallbackEntrances;
}

const SCENE_EXIT_REGION *scGetExitRegions(void)
{
	return NULL;
}

const Uint32 *scGetExits(void)
{
	return CrossboneIsleKeysExits;
}

const ACTOR_PLACE *scGetActorPlacements(void)
{
	Sint16 selector = MainRoutineStateWork.next;

	if(selector == SCENE_CROSSBONE_ISLE_KEYS) {
		return CrossboneIsleKeysRoomActorPlacements;
	}
	if(selector == SCENE_CROSSBONE_ISLE_STATUES) {
		return CrossboneIsleStatuesActorPlacements;
	}
	if(selector == SCENE_CROSSBONE_ISLE_BANE) {
		return CrossboneIsleBaneActorPlacements;
	}
	return CrossboneIsleKeysActorPlacementsOther;
}

/* Halt on one of the five statues in the statue room: the statue stops where it
   is, wrapped in Halt's light, and while it lasts Isaac can walk past it.

   The script itself does nothing: Halt's own effect holds the statue, and each
   statue's chase checks MainRoutineStateWork.haltedActor before it moves. */
static void scCrossboneIsleKeysHaltStatue(void)
{
}

/* Keeps Bane turned toward Isaac on the spiked ledge, every tick. */
static UNREAD_RESULT scCrossboneIsleKeysFaceBaneTowardLeader(ACTOR *actor)
{
	ACTOR *leader = fdActorGet(ACTOR_PARTY_LEADER);

	actor->facing = syAtan(
		leader->z.fixed - actor->z.fixed, leader->x.fixed - actor->x.fixed);
	RETURN_UNREAD(0);
}

/* Bane runs off the first time Isaac comes along the back ledge towards it, at
   metatile (40, 41).

   The camera pans over to Bane at the ledge's west end. It turns with a start
   and hops down off the back ledge onto the low ledge below, beside the first
   pillar, then turns to face south. */
static void scCrossboneIsleKeysBaneFirstEscape(void)
{
	if(fdScriptGetFlag(FLAG_CROSSBONE_ISLE_BANE_FIRST_ESCAPE) == 0) {
		gmFlagSet(FLAG_CROSSBONE_ISLE_BANE_FIRST_ESCAPE);
		fdScriptStart();
		fdCameraSetSpeed(2);
		fdCameraMoveToActorById(ACTOR_BANE, 1);
		fdCameraWaitForMove();
		fdActorFaceDirection(ACTOR_BANE, FACING_EAST, 20);
		fdActorSetAttachedEffect(
			ACTOR_BANE, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
		fdActorRunRepeatedMotion(ACTOR_BANE, 2);
		fdScriptWait(20);
		fdActorSetSpeed(ACTOR_BANE, 1);
		sySndPlay(SE_JUMP);
		fdActorGet(ACTOR_BANE)->velY = toFIXED(8);
		fdActorWalkToAndWait(ACTOR_BANE, 584, 680);
		fdActorFaceDirection(ACTOR_BANE, FACING_SOUTH, 20);
		fdScriptEnd();
	}
}

/* Bane runs off again when Isaac comes up the passage down the west side of the
   room towards it, at metatile (36, 46).

   The camera pans to Bane on the low ledge. It hops back up onto the back ledge
   and dashes east along it to the far end. It lands there with a thud that
   shakes the room (SE_BANE_ESCAPE) and is left tipped over, squashing and
   stretching over and over (CrossboneIsleKeysBaneSquashScript). */
static void scCrossboneIsleKeysBaneSecondEscape(void)
{
	ACTOR *spinningDjinni;

	if(gmFlagGet(FLAG_CROSSBONE_ISLE_BANE_FIRST_ESCAPE) != 0) {
		if(fdScriptGetFlag(FLAG_CROSSBONE_ISLE_BANE_SECOND_ESCAPE) == 0) {
			gmFlagSet(FLAG_CROSSBONE_ISLE_BANE_SECOND_ESCAPE);
			fdScriptStart();

			/* Focus on the Djinni before its next jump away. */
			fdCameraSetSpeed(2);
			fdCameraMoveToActorById(ACTOR_BANE, 1);
			fdCameraWaitForMove();
			fdActorFaceDirection(ACTOR_BANE, FACING_SOUTH, 20);
			fdActorSetAttachedEffect(
				ACTOR_BANE, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
			fdActorRunRepeatedMotion(ACTOR_BANE, 2);
			fdScriptWait(20);
			fdActorSetSpeed(ACTOR_BANE, 1);
			sySndPlay(SE_JUMP);
			fdActorGet(ACTOR_BANE)->velY = toFIXED(10);
			fdActorWalkToAndWait(ACTOR_BANE, 584, 664);
			fdActorFaceDirection(ACTOR_BANE, FACING_SOUTH, 20);
			fdActorSetAttachedEffect(
				ACTOR_BANE, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
			fdScriptWait(30);

			/* Dash across the platforms, then shake the room on landing. */
			fdActorSetSpeedAndAcceleration(ACTOR_BANE, 8, 0.25);
			fdActorWalkToAndWait(ACTOR_BANE, 664, 664);
			fdActorWalkToAndWait(ACTOR_BANE, 744, 664);
			fdActorWalkToAndWait(ACTOR_BANE, 824, 664);
			fdScriptWait(10);
			sySndPlay(SE_BANE_ESCAPE);
			fdMapGridSetShake(4, 2, 1);
			fdScriptWait(20);
			fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
			fdScriptWait(30);

			fdActorSetPos(ACTOR_BANE, 888, 664);
			spinningDjinni = fdActorGet(ACTOR_BANE);
			spinningDjinni->spr->rotation = TRIG_TURN * 31 / 32;
			spinningDjinni->facing = FACING_EAST;
			fdActorSetAnimationSpeed(spinningDjinni, 0);
			fdActorStartScript(
				spinningDjinni, CrossboneIsleKeysBaneSquashScript);
			fdScriptEnd();
		}
	}
}

/* Bane flees a third time when Isaac comes east along the back ledge to it, at
   metatile (51, 41), and this time it runs to the ledge with three spikes in
   the south-east.

   It rights itself, hops down off the east end of the back ledge and scurries
   south down the east side out of sight, wobbling from side to side, to its
   last spot north of the spikes. From then on it keeps turning to face Isaac
   (scCrossboneIsleKeysFaceBaneTowardLeader). */
static void scCrossboneIsleKeysBaneLastEscape(void)
{
	ACTOR *djinni;

	if(gmFlagGet(FLAG_CROSSBONE_ISLE_BANE_SECOND_ESCAPE) != 0 &&
		fdScriptGetFlag(FLAG_CROSSBONE_ISLE_BANE_LAST_ESCAPE) == 0) {
		gmFlagSet(FLAG_CROSSBONE_ISLE_BANE_LAST_ESCAPE);
		fdScriptStart();

		djinni = fdActorGet(ACTOR_BANE);
		djinni->spr->rotation = 0;
		fdActorSetAnimationSpeed(djinni, ANIM_STEP_NORMAL);
		sySndPlay(SE_JUMP);
		fdActorGet(ACTOR_BANE)->velY = toFIXED(8);
		fdActorFaceDirection(ACTOR_BANE, FACING_WEST, 30);
		fdActorSetAttachedEffect(
			ACTOR_BANE, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
		fdActorRunRepeatedMotion(ACTOR_BANE, 2);
		fdScriptWait(20);
		fdActorSetSpeed(ACTOR_BANE, 1);
		sySndPlay(SE_JUMP);
		fdActorGet(ACTOR_BANE)->velY = toFIXED(4);
		fdActorWalkToAndWait(ACTOR_BANE, 880, 680);
		fdScriptWait(10);
		fdActorSetAttachedEffect(
			ACTOR_BANE, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ATTACH);
		fdActorSetSpeed(ACTOR_BANE, 2);
		fdActorWalkToAndWait(ACTOR_BANE, 880, 696);
		fdActorWalkToAndWait(ACTOR_BANE, 882, 704);
		fdActorWalkToAndWait(ACTOR_BANE, 880, 712);
		fdActorWalkToAndWait(ACTOR_BANE, 878, 720);
		fdActorWalkToAndWait(ACTOR_BANE, 880, 728);
		fdActorWalkToAndWait(ACTOR_BANE, 882, 736);
		fdActorWalkToAndWait(ACTOR_BANE, 880, 744);
		fdActorWalkToAndWait(ACTOR_BANE, 878, 752);
		fdActorWalkToAndWait(ACTOR_BANE, 880, 760);
		fdActorSetPos(ACTOR_BANE, 856, 824);
		fdScriptWait(10);
		fdActorFaceDirection(ACTOR_BANE, FACING_NORTH, 20);
		fdActorSetAttachedEffect(
			ACTOR_BANE, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_REMOVE);
		fdActorGet(ACTOR_BANE)->update =
			(void (*)(ACTOR *))scCrossboneIsleKeysFaceBaneTowardLeader;
		fdScriptEnd();
	}
}

/* Bane keeps the three spikes on the south-east ledge between itself and Isaac:
   whenever he steps onto one of the twelve metatiles round them, metatiles
   (50..55, 51..54), it runs as fast as he walks to the spot across from him,
   unless Halt holds it.

   Each metatile's behaviour, less METATILE_BEHAVIOR_BANE_CORNER_FIRST, picks
   Bane's spot from CrossboneIsleKeysBaneWalkTargets. */
static void scCrossboneIsleKeysBaneKeepsAway(void)
{
	ACTOR		*djinni;
	Sint32		 tgt;
	FIELD_STATE *state;

	state = FieldStateWork;
	if(gmFlagGet(FLAG_CROSSBONE_ISLE_BANE_LAST_ESCAPE) != 0) {
		if(MainRoutineStateWork.haltedActor != ACTOR_BANE) {
			tgt = state->stepCue;
			djinni = fdActorGet(ACTOR_BANE);
			djinni->speedLimit = fdActorGet(ACTOR_PARTY_LEADER)->speedLimit;
			djinni = fdActorGet(ACTOR_BANE);
			djinni->acceleration = fdActorGet(ACTOR_PARTY_LEADER)->speedLimit;
			tgt -= METATILE_BEHAVIOR_BANE_CORNER_FIRST;
			fdActorWalkTo(ACTOR_BANE, CrossboneIsleKeysBaneWalkTargets[tgt][0],
				CrossboneIsleKeysBaneWalkTargets[tgt][1]);
		}
	}
}

/* Halt on Bane in Bane's room: it stops still where it is, and Isaac can walk
   up and talk to it. */
static void scCrossboneIsleKeysHaltBane(void)
{
	fdScriptStart();
	fdActorSetAnimation(ACTOR_BANE, ANIM_STILL);
	fdScriptEnd();
}

const CUE *scGetCues(void)
{
	Sint16 selector = MainRoutineStateWork.next;

	if(selector == SCENE_CROSSBONE_ISLE_KEYS) {
		return CrossboneIsleKeysRoomCues;
	}
	if(selector == SCENE_CROSSBONE_ISLE_STATUES) {
		return CrossboneIsleStatuesCues;
	}
	if(selector == SCENE_CROSSBONE_ISLE_BANE) {
		return CrossboneIsleBaneCues;
	}
	return CrossboneIsleKeysCuesOther;
}

/* Unused: pushing south on the walkway in front of the third or fourth blue
   door in the room of keys would hop Isaac down onto the stone post two
   metatiles south, at metatile (21, 45) or (25, 45).

   Its cue rows come after the plain step rows for the same metatiles, which
   answer first and make the same hop themselves
   (scCrossboneIsleKeysOpenThirdBlueDoor). */
static void scCrossboneIsleKeysUnusedJumpOntoPost(void)
{
	fdTryJumpForward();
}

/* Opens a blue door in the room of keys, numbered 0 to 3 from the west, so that
   the doorway leads through to its chest.

   With PASSAGE_PLAY_OPENING the door grinds open as the floor shakes, a
   half-open picture first; without it, as when the party comes back in, the
   door is simply drawn open. */
static void scCrossboneIsleKeysOpenBlueDoor(Sint32 passageInfo)
{
	Sint32 columnOfs = (passageInfo & PASSAGE_INDEX_MASK) * 4;
	Sint32 metatileColumn = columnOfs + 77;
	Sint32 attributeColumn = columnOfs + 13;

	if(passageInfo & PASSAGE_PLAY_OPENING) {
		sySndPlay(SE_GATE_MOVE);
		fdMapGridSetShake(2, 2, 1);
		fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
		fdMapGridCopyMetatiles(BLUE_DOOR_HALF_OPEN_COLUMN, DOOR_PICTURE_ROW, 1,
			3, metatileColumn, 40);
		sySynch(40);
	}
	fdMapGridCopyMetatiles(
		BLUE_DOOR_OPEN_COLUMN, DOOR_PICTURE_ROW, 1, 3, metatileColumn, 40);
	fdMapGridCopyMetatileAttributes(
		attributeColumn, 40, 1, 1, attributeColumn, 41);
	fdMapGridCopyMetatileAttributes(
		attributeColumn, 40, 1, 1, attributeColumn, 42);
}

/* Opens the red door on the ledge west of the pit, playing its grinding opening
   with PASSAGE_PLAY_OPENING as a blue door's. */
static void scCrossboneIsleKeysOpenRedDoor(Sint32 passageInfo)
{
	if((passageInfo & PASSAGE_PLAY_OPENING) != 0) {
		sySndPlay(SE_GATE_MOVE);
		fdMapGridSetShake(2, 2, 1);
		fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
		fdMapGridCopyMetatiles(
			RED_DOOR_HALF_OPEN_COLUMN, DOOR_PICTURE_ROW, 1, 3, 70, 49);
		sySynch(60);
	}
	fdMapGridCopyMetatiles(
		RED_DOOR_OPEN_COLUMN, DOOR_PICTURE_ROW, 1, 3, 70, 49);
	fdMapGridCopyMetatileAttributes(6, 49, 1, 1, 6, 50);
	fdMapGridCopyMetatileAttributes(6, 49, 1, 1, 6, 51);
}

/* Walking up to the red door on the ledge west of the pit with the Red Key, at
   metatile (6, 52), opens it: it grinds open as the floor shakes, and the key
   is used up.

   Without the key, or facing any other way, nothing happens. Through the door
   the way leads on to the floor's west door. */
static void scCrossboneIsleKeysUseRedKey(void)
{
	if(fdActorGet(ACTOR_PARTY_LEADER)->facing == FACING_NORTH) {
		if(fdScriptGetFlag(FLAG_CROSSBONE_ISLE_RED_DOOR_OPEN) == 0) {
			if(gmPartyInventoryFindMember(ITEM_RED_KEY) != -1) {
				gmFlagSet(FLAG_CROSSBONE_ISLE_RED_DOOR_OPEN);
				scCrossboneIsleKeysOpenRedDoor(PASSAGE_PLAY_OPENING);
				gmPartyInventoryRemove(ITEM_RED_KEY);
			}
		}
	}
}

/* Walking up to a blue door with a Blue Key, numbered 0 to 3 from the west,
   opens it: it grinds open as the floor shakes, and one Blue Key is used up.

   Without a key, facing any other way, or once the door is open, nothing
   happens. */
static void scCrossboneIsleKeysUseBlueKey(Sint32 doorIdx)
{
	ACTOR *leader = fdActorGet(ACTOR_PARTY_LEADER);
	Sint32 flg;

	if(leader->facing != FACING_NORTH) {
		return;
	}
	flg = doorIdx + FLAG_CROSSBONE_ISLE_FIRST_BLUE_DOOR_OPEN;
	if(gmFlagGet(flg) != 0) {
		return;
	}
	if(gmPartyInventoryFindMember(ITEM_BLUE_KEY) == -1) {
		return;
	}
	gmFlagSet(flg);
	scCrossboneIsleKeysOpenBlueDoor(PASSAGE_PLAY_OPENING | doorIdx);
	gmPartyInventoryRemove(ITEM_BLUE_KEY);
}

/* Walking up to the westmost blue door on the walkway along the pit's north
   wall, at metatile (13, 43), opens it with a Blue Key, showing the chest with
   333 coins behind it. */
static void scCrossboneIsleKeysOpenFirstBlueDoor(void)
{
	scCrossboneIsleKeysUseBlueKey(0);
}

/* Walking up to the second blue door from the west on the walkway, at metatile
   (17, 43), opens it with a Blue Key, showing the chest with the Fairy Ring
   behind it. */
static void scCrossboneIsleKeysOpenSecondBlueDoor(void)
{
	scCrossboneIsleKeysUseBlueKey(1);
}

/* Walking up to the third blue door from the west on the walkway, at metatile
   (21, 43), opens it with a Blue Key, showing the chest with the Cookie behind
   it.

   Pushing south from the same spot hops Isaac down off the walkway onto the
   stone post two metatiles south, at metatile (21, 45). */
static void scCrossboneIsleKeysOpenThirdBlueDoor(void)
{
	Sint16 *blockedSteps;
	Sint32	resetSteps = 0;

	scCrossboneIsleKeysUseBlueKey(2);
	if(fdActorGet(ACTOR_PARTY_LEADER)->facing == FACING_SOUTH) {
		blockedSteps = &FieldStateWork->blockedSteps;
		if(*blockedSteps > FIELD_PUSH_TICKS) {
			fdTryJumpForward();
			*blockedSteps = resetSteps;
		}
	}
}

/* Walking up to the eastmost blue door on the walkway, at metatile (25, 43),
   opens it with a Blue Key, showing the chest with the Smoke Bomb behind it.

   Pushing south from the same spot hops Isaac down off the walkway onto the
   stone post two metatiles south, at metatile (25, 45), the way to where the
   Red Key is hidden. */
static void scCrossboneIsleKeysOpenFourthBlueDoor(void)
{
	Sint16 *blockedSteps;
	Sint32	resetSteps = 0;

	scCrossboneIsleKeysUseBlueKey(3);
	if(fdActorGet(ACTOR_PARTY_LEADER)->facing == FACING_SOUTH) {
		blockedSteps = &FieldStateWork->blockedSteps;
		if(*blockedSteps > FIELD_PUSH_TICKS) {
			fdTryJumpForward();
			*blockedSteps = resetSteps;
		}
	}
}

/* Reveal in the room of keys shows a hidden stone post, with the Red Key on
   top, three metatiles south of the post below the fourth blue door, at
   metatile (25, 48).

   The post is copied there, picture and metatiles, from the one at (25, 45),
   and the key is put on it unless the party already has it. It appears wherever
   in the room Reveal is cast, but is only seen when Isaac is near it, as from
   the post below the fourth blue door. */
static void scCrossboneIsleKeysShowRedKey(void)
{
	fdMapGridCopyMetatiles(25, 45, 1, 2, 25, 48);
	if(gmFlagGet(FLAG_CROSSBONE_ISLE_RED_KEY) == 0)
		fdActorSetPos(ACTOR_RED_KEY, 408, 776);
	fdScriptWait(1);
}

/* When Reveal wears off in the room of keys, the hidden post south of the
   fourth blue door vanishes again, at metatile (25, 48), and so does the Red
   Key if it is still on it. */
static void scCrossboneIsleKeysHideRedKey(void)
{
	fdMapGridCopyMetatiles(24, 48, 1, 2, 25, 48);
	fdActorSetPos(ACTOR_RED_KEY, 8, 8);
}

/* Gives the party a key that Catch has pulled in: "Isaac got the Blue Key." (or
   the Red Key), and the key's icon is gone from its post for good. */
static inline void scCrossboneIsleKeysGetKey(
	Sint32 actorId, Sint32 itemId, Sint32 collectedFlg)
{
	gmItemShowFound(itemId, 3);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
	gmPartyGiveItem(itemId, 0);
	fdActorSetPos(actorId, 0, 0);
	gmFlagSet(collectedFlg);
}

/* Catch on the Blue Key on the post just north of where Isaac comes into the
   pit from the east door: a hand of light reaches out and pulls the key to
   him. */
static void scCrossboneIsleKeysCatchFirstBlueKey(void)
{
	scCrossboneIsleKeysGetKey(ACTOR_FIRST_BLUE_KEY, ITEM_BLUE_KEY,
		FLAG_CROSSBONE_ISLE_FIRST_BLUE_KEY);
}

/* Catch on the Blue Key on a post in the middle of the pit: a hand of light
   pulls it to Isaac. */
static void scCrossboneIsleKeysCatchSecondBlueKey(void)
{
	scCrossboneIsleKeysGetKey(ACTOR_SECOND_BLUE_KEY, ITEM_BLUE_KEY,
		FLAG_CROSSBONE_ISLE_FIRST_BLUE_KEY + 1);
}

/* Catch on the Blue Key on a post in the north of the pit, below the walkway: a
   hand of light pulls it to Isaac. */
static void scCrossboneIsleKeysCatchThirdBlueKey(void)
{
	scCrossboneIsleKeysGetKey(ACTOR_THIRD_BLUE_KEY, ITEM_BLUE_KEY,
		FLAG_CROSSBONE_ISLE_FIRST_BLUE_KEY + 2);
}

/* Catch on the Blue Key on a post in the south of the pit, east of the middle:
   a hand of light pulls it to Isaac. */
static void scCrossboneIsleKeysCatchFourthBlueKey(void)
{
	scCrossboneIsleKeysGetKey(ACTOR_FOURTH_BLUE_KEY, ITEM_BLUE_KEY,
		FLAG_CROSSBONE_ISLE_FIRST_BLUE_KEY + 3);
}

/* Catch on the Red Key, on the hidden post Reveal shows south of the fourth
   blue door: a hand of light pulls it to Isaac. */
static void scCrossboneIsleKeysCatchRedKey(void)
{
	scCrossboneIsleKeysGetKey(
		ACTOR_RED_KEY, ITEM_RED_KEY, FLAG_CROSSBONE_ISLE_RED_KEY);
}

/* Watches, every tick, for Isaac on the tiles just south of the statue in the
   north-east, metatiles (21..23, 10..11), and sets it on him unless Cloak hides
   him, Halt holds the statue or another statue is already after him. */
static void scCrossboneIsleKeysCheckFirstStatueCue(void)
{
	ACTOR		*leader = fdActorGet(ACTOR_PARTY_LEADER);
	Sint32		 column = leader->x.fixed / toFIXED(16);
	Sint32		 row = leader->z.fixed / toFIXED(16);
	FIELD_STATE *state = FieldStateWork;

	if(!fdScriptGetFlag(FLAG_CROSSBONE_ISLE_STATUE_AFTER_LEADER) &&
		MainRoutineStateWork.cloaked == 0 &&
		MainRoutineStateWork.haltedActor != ACTOR_FIRST_STATUE &&
		(column >= 21 && column <= 23) && row >= 10 && row <= 11) {
		gmFlagSet(FLAG_CROSSBONE_ISLE_STATUE_AFTER_LEADER);
		state->raisedCue = RAISED_FIRST_CHASE;
	}
}

/* The statue in the north-east of the statue room glides down onto Isaac when
   he steps onto the tiles in front of it, with a thud, shoves him back south
   and glides home again.

   scCrossboneIsleKeysCheckFirstStatueCue raises it. Isaac is pushed 24 pixels
   south, and the statue follows him FIRST_STATUE_FOLLOW_PIXELS pixels before it
   turns back. */
static void scCrossboneIsleKeysFirstStatuePushesLeaderBack(void)
{
	ACTOR *leader;

	fdScriptStart();
	fdActorSetSpeed(ACTOR_PARTY_LEADER, 1.9);
	fdActorSetSpeed(ACTOR_FIRST_STATUE, 1.9);
	sySndPlay(SE_THUMP);
	leader = fdActorGet(ACTOR_PARTY_LEADER);
	if(leader != NULL) {
		fdActorSetDestination(ACTOR_FIRST_STATUE, leader->x.part[0].integer,
			leader->z.part[0].integer);
	}
	fdActorWaitForMoveById(ACTOR_FIRST_STATUE);
	fdActorSetDestinationOffset(
		ACTOR_PARTY_LEADER, 0, FIRST_STATUE_SHOVE_PIXELS);
	fdScriptWait(4);
	sySndPlay(SE_THUMP);
	fdActorSetDestinationOffset(
		ACTOR_FIRST_STATUE, 0, FIRST_STATUE_FOLLOW_PIXELS);
	fdActorWaitForMoveById(ACTOR_PARTY_LEADER);
	fdActorSetDestination(ACTOR_FIRST_STATUE, 360, 152);
	fdActorWaitForMoveById(ACTOR_FIRST_STATUE);
	fdScriptEnd();
	gmFlagClear(FLAG_CROSSBONE_ISLE_STATUE_AFTER_LEADER);
}

/* Watches, every tick, for Isaac on the narrow walk below the statue in the
   west, metatiles (10, 16..18), and sets it on him unless Cloak hides him, Halt
   holds the statue or another statue is already after him. */
static void scCrossboneIsleKeysCheckSecondStatueCue(void)
{
	ACTOR		*leader = fdActorGet(ACTOR_PARTY_LEADER);
	Sint32		 column = leader->x.fixed / toFIXED(16);
	Sint32		 row = leader->z.fixed / toFIXED(16);
	FIELD_STATE *state = FieldStateWork;

	if(fdScriptGetFlag(FLAG_CROSSBONE_ISLE_STATUE_AFTER_LEADER) == 0 &&
		MainRoutineStateWork.cloaked == 0 &&
		MainRoutineStateWork.haltedActor != ACTOR_SECOND_STATUE &&
		column == 10 && (row >= 16 && row <= 18)) {
		fdScriptSetFlag(FLAG_CROSSBONE_ISLE_STATUE_AFTER_LEADER);
		scCrossboneIsleKeysRaiseScript(&state->raisedCue, RAISED_SECOND_CHASE);
	}
}

/* The statue on the narrow walk between the dark pits in the west of the statue
   room glides down onto Isaac as he comes north up the walk, with a thud,
   shoves him back south and glides home again.

   scCrossboneIsleKeysCheckSecondStatueCue raises it. */
static void scCrossboneIsleKeysSecondStatuePushesLeaderBack(void)
{
	ACTOR *leader;

	fdScriptStart();
	fdActorSetSpeed(ACTOR_PARTY_LEADER, 1.7);
	fdActorSetSpeed(ACTOR_SECOND_STATUE, 1.7);
	sySndPlay(SE_THUMP);
	leader = fdActorGet(ACTOR_PARTY_LEADER);
	if(leader != NULL) {
		fdActorSetDestination(ACTOR_SECOND_STATUE, leader->x.part[0].integer,
			leader->z.part[0].integer);
	}
	fdActorWaitForMoveById(ACTOR_SECOND_STATUE);
	fdActorSetDestinationOffset(ACTOR_PARTY_LEADER, 0, STATUE_SHOVE_PIXELS);
	sySndPlay(SE_THUMP);
	fdScriptWait(4);
	fdActorSetDestinationOffset(ACTOR_SECOND_STATUE, 0, 16);
	fdActorWaitForMoveById(ACTOR_PARTY_LEADER);
	fdActorSetDestination(ACTOR_SECOND_STATUE, 168, 264);
	fdActorWaitForMoveById(ACTOR_SECOND_STATUE);
	fdScriptEnd();
	gmFlagClear(FLAG_CROSSBONE_ISLE_STATUE_AFTER_LEADER);
}

/* Slides the third or fourth statue (0 or 1) along its alcove with a thud to
   stand right in front of Isaac's column, unless Halt holds it, and nudges him
   8 pixels south if he has got into the alcove's row.

   CrossboneIsleKeysStatueColumns keeps the column each statue stands in. */
static void scCrossboneIsleKeysTrackStatueColumn(Sint32 statueIdx)
{
	ACTOR *leader = fdActorGet(ACTOR_PARTY_LEADER);
	Sint32 column = leader->x.fixed / toFIXED(16);
	Sint32 row = leader->z.fixed / toFIXED(16);
	Sint32 statueId = statueIdx + ACTOR_THIRD_STATUE;

	if(MainRoutineStateWork.haltedActor == statueId)
		return;
	if(column == CrossboneIsleKeysStatueColumns[statueIdx])
		return;

	fdActorSetSpeed(statueId, 4.5);
	sySndPlay(SE_THUMP);
	fdActorSetDestination(statueId, (column << 4) + 8, 360);

	CrossboneIsleKeysStatueColumns[statueIdx] = column;

	if(row <= 22) {
		fdActorSetDestinationOffset(ACTOR_PARTY_LEADER, 0, 8);
	}
	fdActorWaitForMoveById(ACTOR_PARTY_LEADER);
}

/* As Isaac walks along the rug in front of the third statue, in the west alcove
   of the statue room's south walkway, metatiles (15..17, 23), the statue slides
   sideways to stay right in front of him, barring the way north to the chest
   with the Sleep Bomb.

   Halt on the statue while it stands to one side lets him walk past. */
static void scCrossboneIsleKeysThirdStatueShadowsLeader(void)
{
	scCrossboneIsleKeysTrackStatueColumn(0);
}

/* As Isaac walks along the rug in front of the fourth statue, in the east
   alcove of the statue room's south walkway, metatiles (20..22, 23), the statue
   slides sideways to stay right in front of him and bar the way north.

   Halt on the statue while it stands to one side lets him walk past. */
static void scCrossboneIsleKeysFourthStatueShadowsLeader(void)
{
	scCrossboneIsleKeysTrackStatueColumn(1);
}

/* Unused: would make the fifth statue slide to stay in front of Isaac as the
   third and fourth do; nothing calls it. */
UNUSED static void scCrossboneIsleKeysTrackFifthStatueColumn(void)
{
	scCrossboneIsleKeysTrackStatueColumn(2);
}

/* Stepping into the third statue's alcove on the south walkway, metatiles
   (15..17, 22), while Halt does not hold the statue, makes it glide onto Isaac
   with a thud, shove him back south and glide home again. */
static void scCrossboneIsleKeysThirdStatuePushesLeaderBack(void)
{
	ACTOR *leader;

	if(MainRoutineStateWork.haltedActor != ACTOR_THIRD_STATUE) {
		fdScriptStart();
		fdActorSetSpeed(ACTOR_PARTY_LEADER, 1.7);
		fdActorSetSpeed(ACTOR_THIRD_STATUE, 1.7);
		sySndPlay(SE_THUMP);
		leader = fdActorGet(ACTOR_PARTY_LEADER);
		if(leader != NULL) {
			fdActorSetDestination(ACTOR_THIRD_STATUE, leader->x.part[0].integer,
				leader->z.part[0].integer);
		}
		fdActorWaitForMoveById(ACTOR_THIRD_STATUE);
		fdActorSetDestinationOffset(ACTOR_PARTY_LEADER, 0, STATUE_SHOVE_PIXELS);
		fdScriptWait(4);
		sySndPlay(SE_THUMP);
		fdActorSetDestinationOffset(ACTOR_THIRD_STATUE, 0, 16);
		fdActorWaitForMoveById(ACTOR_PARTY_LEADER);
		fdActorSetDestination(ACTOR_THIRD_STATUE, 264, 360);
		fdActorWaitForMoveById(ACTOR_THIRD_STATUE);
		fdScriptWait(10);
		fdScriptEnd();
	}
}

/* Stepping into the fourth statue's alcove on the south walkway, metatiles
   (20..22, 22), while Halt does not hold the statue, makes it shuffle half a
   metatile towards Isaac, glide onto him with a thud, shove him back south and
   glide home again. */
static void scCrossboneIsleKeysFourthStatuePushesLeaderBack(void)
{
	ACTOR *leader;

	if(MainRoutineStateWork.haltedActor != ACTOR_FOURTH_STATUE) {
		fdScriptStart();
		fdActorSetSpeed(ACTOR_PARTY_LEADER, 1.7);
		fdActorSetSpeed(ACTOR_FOURTH_STATUE, 1.7);
		sySndPlay(SE_THUMP);
		if(fdActorGet(ACTOR_PARTY_LEADER)->x.fixed / toFIXED(16) >
			fdActorGet(ACTOR_FOURTH_STATUE)->x.fixed / toFIXED(16)) {
			fdActorSetDestinationOffset(ACTOR_FOURTH_STATUE, 8, 0);
		}
		if(fdActorGet(ACTOR_PARTY_LEADER)->x.fixed / toFIXED(16) <
			fdActorGet(ACTOR_FOURTH_STATUE)->x.fixed / toFIXED(16)) {
			fdActorSetDestinationOffset(ACTOR_FOURTH_STATUE, -8, 0);
		}
		fdActorWaitForMoveById(ACTOR_FOURTH_STATUE);
		leader = fdActorGet(ACTOR_PARTY_LEADER);
		if(leader != NULL) {
			fdActorSetDestination(ACTOR_FOURTH_STATUE,
				leader->x.part[0].integer, leader->z.part[0].integer);
		}
		fdActorWaitForMoveById(ACTOR_FOURTH_STATUE);
		fdActorSetDestinationOffset(ACTOR_PARTY_LEADER, 0, STATUE_SHOVE_PIXELS);
		fdScriptWait(4);
		sySndPlay(SE_THUMP);
		fdActorSetDestinationOffset(ACTOR_FOURTH_STATUE, 0, 16);
		fdActorWaitForMoveById(ACTOR_PARTY_LEADER);
		fdActorSetDestination(ACTOR_FOURTH_STATUE, 344, 360);
		fdActorWaitForMoveById(ACTOR_FOURTH_STATUE);
		fdScriptWait(10);
		fdScriptEnd();
	}
}

/* Watches, every tick, for Isaac on the two tiles just south of the statue in
   the middle of the hall, metatiles (19, 15..16), and sets it on him unless
   Cloak hides him, Halt holds the statue or another statue is already after
   him. */
static void scCrossboneIsleKeysCheckFifthStatueCue(void)
{
	ACTOR			   *leader = fdActorGet(ACTOR_PARTY_LEADER);
	Sint32				column = leader->x.fixed / toFIXED(16);
	Sint32				row = leader->z.fixed / toFIXED(16);
	FIELD_STATE		   *fieldState = FieldStateWork;
	MAIN_ROUTINE_STATE *state = &MainRoutineStateWork;

	if(state->haltedActor != ACTOR_FIFTH_STATUE &&
		fdScriptGetFlag(FLAG_CROSSBONE_ISLE_STATUE_AFTER_LEADER) == 0 &&
		state->cloaked == 0 && column == 19 && (row >= 15 && row <= 16)) {
		fdScriptSetFlag(FLAG_CROSSBONE_ISLE_STATUE_AFTER_LEADER);
		scCrossboneIsleKeysRaiseScript(
			&fieldState->raisedCue, RAISED_FIFTH_CHASE);
	}
}

/* The statue in the middle of the statue room glides down onto Isaac when he
   steps onto the tiles in front of it, with a thud, shoves him back south and
   glides home again.

   scCrossboneIsleKeysCheckFifthStatueCue raises it. */
static void scCrossboneIsleKeysFifthStatuePushesLeaderBack(void)
{
	ACTOR *leader;

	fdScriptStart();
	fdActorSetSpeed(ACTOR_PARTY_LEADER, 1.7);
	fdActorSetSpeed(ACTOR_FIFTH_STATUE, 1.7);
	sySndPlay(SE_THUMP);
	leader = fdActorGet(ACTOR_PARTY_LEADER);
	if(leader != NULL) {
		fdActorSetDestination(ACTOR_FIFTH_STATUE, leader->x.part[0].integer,
			leader->z.part[0].integer);
	}
	fdActorWaitForMoveById(ACTOR_FIFTH_STATUE);
	fdActorSetDestinationOffset(ACTOR_PARTY_LEADER, 0, STATUE_SHOVE_PIXELS);
	sySndPlay(SE_THUMP);
	fdActorSetDestinationOffset(ACTOR_FIFTH_STATUE, 0, 16);
	fdActorWaitForMoveById(ACTOR_PARTY_LEADER);
	fdActorSetDestination(ACTOR_FIFTH_STATUE, 312, 232);
	fdActorWaitForMoveById(ACTOR_FIFTH_STATUE);
	fdScriptEnd();
	gmFlagClear(FLAG_CROSSBONE_ISLE_STATUE_AFTER_LEADER);
}

/* Reveal in the statue room lifts the darkness: while it lasts the whole hall
   shows evenly in Reveal's grey, not only the circles of light round the
   braziers.

   The darkness is a background layer laid over the room, half see-through
   (scCrossboneIsleKeysInitStatues). Reveal's blend counts that layer for
   nothing and the room under it for 13 sixteenths, so the hall shows whole and
   a little dim. */
static void scCrossboneIsleKeysCastRevealInStatueRoom(void)
{
	Uint16 value = BLEND_ALPHA(0, 13);

	REG_BLDALPHA = value;
}

/* When Reveal wears off in the statue room, the darkness falls over the hall
   again outside the circles of light. */
static void scCrossboneIsleKeysRevealEndsInStatueRoom(void)
{
	Uint16 value = BLEND_ALPHA(7, 6);

	REG_BLDALPHA = value;
}

/* At either of the two ladders down from the back ledge of Bane's room, by the
   spiked channel in the middle, metatiles (42, 41..42) and (48, 41..42), Isaac
   climbs up with Up and down with Down, and with Right hops east off the
   ladder.

   Hopping east from the west ladder, over the wall beside it, lands him on the
   little ledge at the top of the spiked channel, by the Demon Axe's chest once
   the second pillar has brought it there. Pushing west from the foot of the
   west ladder pushes the first pillar back west if it has been pushed up to it;
   from higher up the ladder he hops west instead. */
static void scCrossboneIsleKeysUseLadder(void)
{
	ACTOR *leader = fdActorGet(ACTOR_PARTY_LEADER);
	Uint16 facing = leader->facing;

	if(facing == FACING_NORTH) {
		if(gmFlagGet(FLAG_CROSSBONE_ISLE_BANE_CHEST_AT_LADDER) != 0) {
			{
				Sint32 dstColumn = 45;
				Sint32 dstRow = 43;
				/* The plain ledge beside it, (46, 43). */
				fdMapGridCopyMetatileAttributes(
					46, 43, 1, 1, dstColumn, dstRow);
			}
		}
		gmFlagClear(FLAG_CROSSBONE_ISLE_BANE_JUMPED_EAST_FROM_LADDER);
		fdClimbUp();
	} else if(facing == FACING_SOUTH) {
		fdClimbDown();
	} else if(facing == FACING_EAST) {
		if(gmFlagGet(FLAG_CROSSBONE_ISLE_BANE_CHEST_AT_LADDER) != 0) {
			{
				Sint32 dstColumn = 45;
				Sint32 dstRow = 43;
				fdMapGridCopyMetatileAttributes(DEMON_AXE_CHEST_COLUMN,
					DEMON_AXE_CHEST_ROW, 1, 1, dstColumn, dstRow);
			}
		}
		gmFlagSet(FLAG_CROSSBONE_ISLE_BANE_JUMPED_EAST_FROM_LADDER);
		fdTryJumpForward();
	} else if(facing == FACING_WEST) {
		if(leader->y.fixed == 0) {
			scCrossboneIsleKeysPushFirstPillar();
		} else {
			fdTryJumpForward();
		}
	}
}

/* Unused: an empty function nothing refers to. */
UNUSED static void scCrossboneIsleKeysUnusedNoOperationB(void)
{
}

/* Frost on the puddle between the two spikes in the south-west of Bane's room
   freezes it into a pillar of ice, which Isaac can hop onto and across like a
   stepping stone, at metatile (40, 55). */
static void scCrossboneIsleKeysFreezePuddle(void)
{
	ACTOR *puddle = fdActorGet(ACTOR_PUDDLE);

	fdMapGridCopyMetatileAttributes(40, 54, 1, 1, 40, 55);
	if(puddle != NULL) {
		Uint8 *motionFlg = &fdActorGet(ACTOR_PUDDLE)->motionFlg;
		Uint8 *priFlg = &puddle->priFlg;

		*motionFlg = 0;
		*priFlg = ACTOR_PRIORITY_UNDERFOOT;
	}
	gmFlagSet(FLAG_CROSSBONE_ISLE_BANE_PUDDLE_FROZEN);
}

/* Just east of the first pillar's home on the low ledge in the north-west of
   Bane's room, at metatile (39, 42), pushing east hops Isaac east off the
   ledge; pushing west into the pillar pushes it. */
static void scCrossboneIsleKeysJumpEastOrPushFirstPillar(void)
{
	ACTOR *leader = fdActorGet(ACTOR_PARTY_LEADER);

	if(leader->facing == FACING_EAST) {
		fdTryJumpForward();
	} else {
		scCrossboneIsleKeysPushFirstPillar();
	}
}

/* Lays the low ledge in the north-west as it is with the first pillar gone,
   before the pillar's own metatile is laid where it stands. */
static void scCrossboneIsleKeysRestoreFirstPillarTerrain(void)
{
	fdMapGridCopyMetatileAttributes(
		EMPTY_GAP_COLUMN, EMPTY_GAP_ROW, 1, 1, 40, 42);
	fdMapGridCopyMetatileAttributes(
		EMPTY_GAP_COLUMN, EMPTY_GAP_ROW, 1, 1, 41, 42);
	fdMapGridCopyMetatileAttributes(
		EMPTY_LADDER_FOOT_COLUMN, EMPTY_GAP_ROW, 1, 1, 42, 42);
	fdMapGridCopyMetatileAttributes(
		LOW_LEDGE_COLUMN, LOW_LEDGE_ROW, 3, 1, 37, 42);

	fdActorGet(ACTOR_FIRST_PILLAR)->motionFlg = ACTOR_FOLLOWS_TERRAIN;
}

/* The first pillar stands in the gap's east metatile, at the foot of the west
   ladder, metatile (42, 42): its top raises the ladder's foot to the low
   ledge's height. */
static void scCrossboneIsleKeysRestoreEastUnderfootMetatile(void)
{
	fdMapGridCopyMetatileAttributes(
		RAISED_LADDER_FOOT_COLUMN, RAISED_LADDER_FOOT_ROW, 1, 1, 42, 42);
	fdActorGet(ACTOR_FIRST_PILLAR)->priFlg = ACTOR_PRIORITY_UNDERFOOT;
}

/* The first pillar stands in the middle of the gap, metatile (41, 42): its top
   is a square Isaac can hop across. */
static void scCrossboneIsleKeysRestoreMiddleUnderfootMetatile(void)
{
	/* The hop square beside the gap, (44, 42). */
	fdMapGridCopyMetatileAttributes(44, 42, 1, 1, 41, 42);
	fdActorGet(ACTOR_FIRST_PILLAR)->priFlg = ACTOR_PRIORITY_UNDERFOOT;
}

/* The first pillar stands in the gap's west metatile, metatile (40, 42), its
   top level with the low ledge, which it lengthens by a metatile. */
static void scCrossboneIsleKeysRestoreWestUnderfootMetatile(void)
{
	/* The ledge's metatile east of the pillar's home, (39, 42). */
	fdMapGridCopyMetatileAttributes(39, 42, 1, 1, 40, 42);
	fdActorGet(ACTOR_FIRST_PILLAR)->priFlg = ACTOR_PRIORITY_UNDERFOOT;
}

/* Move on the first pillar, on the low ledge in the north-west of Bane's room,
   slides it along the ledge; the metatiles under it follow it.

   Pushed east off the ledge, it drops into the gap between the ledge and the
   west ladder, and its top makes a step there. On the ledge, at the top of the
   short ladder at column 38, its top is level with the back ledge. The pushes
   and the room's setting up call this too, to lay its metatiles. */
static void scCrossboneIsleKeysMoveFirstPillar(void)
{
	ACTOR *actor = fdActorGet(ACTOR_FIRST_PILLAR);
	FIXED  x = actor->x.fixed;
	FIXED  height = actor->y.fixed;
	Sint32 column = x / toFIXED(16);

	if(height == 0) {
		fdActorGet(ACTOR_FIRST_PILLAR)->priFlg = ACTOR_PRIORITY_UNDERFOOT;
	}
	scCrossboneIsleKeysRestoreFirstPillarTerrain();
	fdActorGet(ACTOR_FIRST_PILLAR)->motionFlg =
		ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
	if(column == 40) {
		scCrossboneIsleKeysRestoreWestUnderfootMetatile();
	} else if(column == 42) {
		scCrossboneIsleKeysRestoreEastUnderfootMetatile();
	} else if(column == 41) {
		scCrossboneIsleKeysRestoreMiddleUnderfootMetatile();
	} else if(column == 39) {
		scCrossboneIsleKeysLiftFirstPillar(column, 0);
	} else if(column == 38) {
		scCrossboneIsleKeysLiftFirstPillar(column, 0);
	} else if(column == 37) {
		scCrossboneIsleKeysLiftFirstPillar(column, 0);
	}
}

/* Walking into the first pillar on the low ledge in the north-west of Bane's
   room pushes it a metatile along the ledge.

   Once the pillar has left the top of the short ladder at column 38, pushing
   north or south there climbs the ladder instead. */
static void scCrossboneIsleKeysPushFirstPillar(void)
{
	ACTOR *leader = fdActorGet(ACTOR_PARTY_LEADER);
	ACTOR *pillar = fdActorGet(ACTOR_FIRST_PILLAR);
	Sint32 leaderColumn = leader->x.fixed / toFIXED(16);
	Sint32 pillarColumn = pillar->x.fixed / toFIXED(16);

	if(leaderColumn == 38 && pillarColumn != 38) {
		Uint16 facing = leader->facing;

		if(facing == FACING_NORTH) {
			fdClimbUp();
			return;
		}
		if(facing == FACING_SOUTH) {
			fdClimbDown();
			return;
		}
	}

	scCrossboneIsleKeysRestoreFirstPillarTerrain();
	scPushedActorAdvancePair();
	scCrossboneIsleKeysMoveFirstPillar();
}

/* Leaves the Demon Axe's chest on the second pillar at the top of the spiked
   channel, where the chest's examine cue takes its place. */
static void scCrossboneIsleKeysReleaseSecondPillarChest(void)
{
	syCloseEvent(scCrossboneIsleKeysFollowSecondPillar);
	fdActorSetPos(ACTOR_SECOND_PILLAR_CHEST, 0, 0);
	if(gmFlagGet(FLAG_CROSSBONE_ISLE_BANE_JUMPED_EAST_FROM_LADDER) != 0) {
		fdMapGridCopyMetatileAttributes(
			DEMON_AXE_CHEST_COLUMN, DEMON_AXE_CHEST_ROW, 1, 1, 45, 43);
	} else {
		fdMapGridCopyMetatileAttributes(46, 43, 1, 1, 45, 43);
	}
	scCrossboneIsleKeysPosSecondPillarMarker();
	gmFlagSet(FLAG_CROSSBONE_ISLE_BANE_CHEST_AT_LADDER);
}

/* Move on the second pillar, in the spiked channel in the middle of Bane's
   room, slides it with the Demon Axe's chest riding on top; once it reaches the
   top of the channel by the ladders, metatile (45, 43), it stays there and the
   chest can be opened from beside it. */
static void scCrossboneIsleKeysMoveSecondPillar(void)
{
	ACTOR *pillar = fdActorGet(ACTOR_SECOND_PILLAR);
	Sint32 column = pillar->x.fixed / toFIXED(16);
	Sint32 row = pillar->z.fixed / toFIXED(16);

	if(column == 45 && row == 43) {
		scCrossboneIsleKeysReleaseSecondPillarChest();
	}
}

/* Walking into the second pillar, at the foot of the spiked channel in the
   middle of Bane's room, pushes it a metatile north up the channel towards the
   back of the room, carrying the Demon Axe's chest on top. */
static void scCrossboneIsleKeysPushSecondPillar(void)
{
	scPushedActorAdvancePair();
	scCrossboneIsleKeysMoveSecondPillar();
}

/* Lays the floor along the south of the south-west platform as it is with no
   pillar at either end, metatiles (38, 55) and (42, 55). */
static void scCrossboneIsleKeysRestoreThirdPillarTerrain(void)
{
	fdMapGridCopyMetatileAttributes(38, 56, 1, 1, 38, 55);
	fdMapGridCopyMetatileAttributes(42, 56, 1, 1, 42, 55);
}

/* The third pillar has come down at the west end of the floor below its
   platform, metatile (38, 55): its top becomes a square Isaac can hop
   across. */
static void scCrossboneIsleKeysRestoreThirdPillarWestMetatile(void)
{
	fdMapGridCopyMetatileAttributes(40, 54, 1, 1, 38, 55);
	fdActorGet(ACTOR_THIRD_PILLAR)->motionFlg = 0;
}

/* The third pillar has come down at the east end of the floor below its
   platform, metatile (42, 55): its top becomes a square Isaac can hop
   across. */
static void scCrossboneIsleKeysRestoreThirdPillarEastMetatile(void)
{
	fdMapGridCopyMetatileAttributes(40, 54, 1, 1, 42, 55);
	fdActorGet(ACTOR_THIRD_PILLAR)->motionFlg = 0;
}

/* Move on the third pillar, on the raised platform in the south-west of Bane's
   room, slides it; pushed off the platform's south edge it drops to the floor
   below, and its top becomes a stepping stone beside the puddle.

   The pushes and the room's setting up call this too, to lay its metatiles. */
static void scCrossboneIsleKeysMoveThirdPillar(void)
{
	ACTOR *pillar = fdActorGet(ACTOR_THIRD_PILLAR);
	Sint32 heightMetatiles = pillar->y.fixed / toFIXED(16);
	Sint32 column = pillar->x.fixed / toFIXED(16);
	Sint32 row = pillar->z.fixed / toFIXED(16);

	if(!fdScriptGetFlag(FLAG_CROSSBONE_ISLE_THIRD_PILLAR_LOWERED) &&
		heightMetatiles <= 2) {
		scCrossboneIsleKeysRestoreThirdPillarWestMetatile();
		gmFlagSet(FLAG_CROSSBONE_ISLE_THIRD_PILLAR_LOWERED);
	}
	if(row == 55) {
		if(column == 42) {
			scCrossboneIsleKeysRestoreThirdPillarEastMetatile();
		}
		if(column == 38) {
			scCrossboneIsleKeysRestoreThirdPillarWestMetatile();
		}
	} else {
		scCrossboneIsleKeysRestoreThirdPillarTerrain();
	}
}

/* Walking into the third pillar, on the raised platform in the south-west of
   Bane's room or on the floor below it, pushes it a metatile. */
static void scCrossboneIsleKeysPushThirdPillar(void)
{
	scCrossboneIsleKeysRestoreThirdPillarTerrain();
	scPushedActorAdvancePair();
	scCrossboneIsleKeysMoveThirdPillar();
}

/* At the south edge of the raised platform in the south-west of Bane's room,
   metatile (38, 54), pushing south jumps Isaac down off the platform to the
   floor below in a puff of dust; pushing any other way pushes the third
   pillar. */
static void scCrossboneIsleKeysTouchThirdPillar(void)
{
	ACTOR *leader = fdActorGet(ACTOR_PARTY_LEADER);

	if(leader->facing == FACING_SOUTH) {
		syFieldActorJumpDownLedge(ACTOR_PARTY_LEADER, 6, 0);
	} else {
		scCrossboneIsleKeysPushThirdPillar();
	}
}

/* Lays the row the fourth and fifth pillars are pushed along, metatiles
   (49..52, 55), as plain floor. */
static void scCrossboneIsleKeysRestoreFourthAndFifthPillarTerrain(void)
{
	fdMapGridCopyMetatileAttributes(48, 55, 1, 1, 49, 55);
	fdMapGridCopyMetatileAttributes(48, 55, 1, 1, 50, 55);
	fdMapGridCopyMetatileAttributes(48, 55, 1, 1, 51, 55);
	fdMapGridCopyMetatileAttributes(48, 55, 1, 1, 52, 55);
}

/* Makes the tops of the fourth and fifth pillars squares Isaac can hop across,
   wherever along their row they stand. */
static void scCrossboneIsleKeysRepaintFourthAndFifthPillarMetatiles(void)
{
	ACTOR *pillar;
	Sint32 column;

	scCrossboneIsleKeysRestoreFourthAndFifthPillarTerrain();

	pillar = fdActorGet(ACTOR_FOURTH_PILLAR);
	column = pillar->x.fixed / toFIXED(16);
	fdMapGridCopyMetatileAttributes(53, 55, 1, 1, column, 55);
	pillar = fdActorGet(ACTOR_FIFTH_PILLAR);
	column = pillar->x.fixed / toFIXED(16);
	fdMapGridCopyMetatileAttributes(53, 55, 1, 1, column, 55);
}

/* Move on the west one of the two pillars by the east doorway of Bane's room
   slides it along the south row; its top stays a stepping stone. */
static void scCrossboneIsleKeysMoveFourthPillar(void)
{
	scCrossboneIsleKeysRepaintFourthAndFifthPillarMetatiles();
}

/* Walking into either of the two pillars by the east doorway of Bane's room
   pushes it along the south row, metatiles (48..52, 55), and its top stays a
   stepping stone wherever it stops.

   The push moves whichever pillar Isaac walks into
   (scPushedActorAdvancePair). */
static void scCrossboneIsleKeysPushFourthOrFifthPillar(void)
{
	scCrossboneIsleKeysRestoreFourthAndFifthPillarTerrain();
	scPushedActorAdvancePair();
	scCrossboneIsleKeysMoveFourthPillar();
}

/* Move on the east one of the two pillars by the east doorway of Bane's room
   slides it along the south row; its top stays a stepping stone. */
static void scCrossboneIsleKeysMoveFifthPillar(void)
{
	scCrossboneIsleKeysRepaintFourthAndFifthPillarMetatiles();
}

/* Unused: the same push as scCrossboneIsleKeysPushFourthOrFifthPillar's,
   written for the fifth pillar; nothing refers to it. */
UNUSED static void scCrossboneIsleKeysPushFifthPillar(void)
{
	scCrossboneIsleKeysRestoreFourthAndFifthPillarTerrain();
	scPushedActorAdvancePair();
	scCrossboneIsleKeysMoveFifthPillar();
}

/* Keeps the Demon Axe's chest on top of the second pillar as it moves, every
   tick. */
static void scCrossboneIsleKeysFollowSecondPillar(void)
{
	ACTOR *tgt = fdActorGet(ACTOR_SECOND_PILLAR_CHEST);
	ACTOR *src = fdActorGet(ACTOR_SECOND_PILLAR);

	tgt->y.fixed = toFIXED(32);
	tgt->x.fixed = src->x.fixed;
	tgt->z.fixed = src->z.fixed + toFIXED(1);
}

/* Puts the Demon Axe chest's examine cue where the second pillar stands. */
static void scCrossboneIsleKeysPosSecondPillarMarker(void)
{
	ACTOR *frm = fdActorGet(ACTOR_SECOND_PILLAR);

	fdTreasureSpotSetActorPos(
		TREASURE_SPOT_DEMON_AXE, frm->x.fixed, frm->z.fixed + toFIXED(1));
}

/* Draws the pillars in front of Isaac or behind him, every tick, by how high
   he stands and, for the third, how far south he is. */
static void scCrossboneIsleKeysUpdatePillarPriorities(void)
{
	ACTOR *leader = fdActorGet(ACTOR_PARTY_LEADER);
	Uint8  pri;

	if(leader->y.fixed > toFIXED(16)) {
		pri = ACTOR_PRIORITY_UNDERFOOT;
		fdActorGet(ACTOR_FIRST_PILLAR)->priFlg = pri;
		if(fdActorGet(ACTOR_THIRD_PILLAR)->y.fixed == 0)
			fdActorGet(ACTOR_THIRD_PILLAR)->priFlg = pri;
		fdActorGet(ACTOR_FOURTH_PILLAR)->priFlg = pri;
	} else {
		if(fdActorGet(ACTOR_THIRD_PILLAR)->y.fixed == 0 &&
			fdActorGet(ACTOR_PARTY_LEADER)->z.fixed / toFIXED(16) > 56) {
			fdActorSetSprPriority(ACTOR_THIRD_PILLAR, 3);
		} else {
			fdActorSetSprPriority(ACTOR_THIRD_PILLAR, 1);
			fdActorGet(ACTOR_THIRD_PILLAR)->priFlg = ACTOR_PRIORITY_AUTOMATIC;
		}
		pri = 0;
		fdActorGet(ACTOR_FOURTH_PILLAR)->priFlg = pri;
	}
	fdActorGet(ACTOR_FIFTH_PILLAR)->priFlg = pri;
}

/* Sets up whichever of the three rooms the party enters: the keys and doors of
   the room of keys, the statues of the statue room, or the pillars and Bane of
   Bane's room. */
Sint32 scInit(void)
{
	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
	if(MainRoutineStateWork.next == SCENE_CROSSBONE_ISLE_KEYS) {
		scCrossboneIsleKeysInitKeys();
	}
	if(MainRoutineStateWork.next == SCENE_CROSSBONE_ISLE_STATUES) {
		scCrossboneIsleKeysInitStatues();
	}
	if(MainRoutineStateWork.next == SCENE_CROSSBONE_ISLE_BANE) {
		scCrossboneIsleKeysInitBane();
	}
	return 0;
}

/* Unused: nothing adds it as an event.

   Every 17 ticks it would put uncaught Blue Keys back on their metatiles and
   redraw the five key actors, one every 2 ticks from the Red Key to the first
   Blue Key. It would draw them with item codes 0 and 1 (a gem and the Long
   Sword) rather than ITEM_BLUE_KEY and ITEM_RED_KEY. */
UNUSED static void scCrossboneIsleKeysUnusedRedrawKeyActors(void)
{
	if(++CrossboneIsleKeysIconTimer > 16) {
		CrossboneIsleKeysIconTimer = 0;
	}
	switch(CrossboneIsleKeysIconTimer) {
	case 12:
		if(gmFlagGet(FLAG_CROSSBONE_ISLE_FIRST_BLUE_KEY) == 0) {
			fdActorSetPos(ACTOR_FIRST_BLUE_KEY, 232, 872);
		}
		if(gmFlagGet(FLAG_CROSSBONE_ISLE_FIRST_BLUE_KEY + 1) == 0) {
			fdActorSetPos(ACTOR_SECOND_BLUE_KEY, 296, 824);
		}
		if(gmFlagGet(FLAG_CROSSBONE_ISLE_FIRST_BLUE_KEY + 2) == 0) {
			fdActorSetPos(ACTOR_THIRD_BLUE_KEY, 328, 760);
		}
		if(gmFlagGet(FLAG_CROSSBONE_ISLE_FIRST_BLUE_KEY + 3) == 0) {
			fdActorSetPos(ACTOR_FOURTH_BLUE_KEY, 360, 872);
		}
		break;
	/* XXX (no evidence): why item codes 0 and 1, whose icons are a yellow gem
	   and a sword, where scCrossboneIsleKeysInitKeys draws the keys' own;
	   nothing calls this, and no other code or text says what it was for. */
	case 10:
		scColossoShowItemIcon(ACTOR_FIRST_BLUE_KEY, 0);
		break;
	case 8:
		scColossoShowItemIcon(ACTOR_SECOND_BLUE_KEY, 0);
		break;
	case 6:
		scColossoShowItemIcon(ACTOR_THIRD_BLUE_KEY, 0);
		break;
	case 4:
		scColossoShowItemIcon(ACTOR_FOURTH_BLUE_KEY, 0);
		break;
	case 2:
		scColossoShowItemIcon(ACTOR_RED_KEY, 1);
		break;
	}
}

#include "shared/colosso_item_icon.c"

/* Puts each key not yet taken on its post, and draws open the doors already
   opened. */
static void scCrossboneIsleKeysInitKeys(void)
{
	sySynch(1);
	scColossoShowItemIcon(ACTOR_RED_KEY, ITEM_RED_KEY);
	scColossoShowItemIcon(ACTOR_FOURTH_BLUE_KEY, ITEM_BLUE_KEY);
	scColossoShowItemIcon(ACTOR_THIRD_BLUE_KEY, ITEM_BLUE_KEY);
	scColossoShowItemIcon(ACTOR_SECOND_BLUE_KEY, ITEM_BLUE_KEY);
	scColossoShowItemIcon(ACTOR_FIRST_BLUE_KEY, ITEM_BLUE_KEY);
	if(gmFlagGet(FLAG_CROSSBONE_ISLE_FIRST_BLUE_KEY) == 0) {
		fdActorSetPos(ACTOR_FIRST_BLUE_KEY, 232, 872);
	}
	if(gmFlagGet(FLAG_CROSSBONE_ISLE_FIRST_BLUE_KEY + 1) == 0) {
		fdActorSetPos(ACTOR_SECOND_BLUE_KEY, 296, 824);
	}
	if(gmFlagGet(FLAG_CROSSBONE_ISLE_FIRST_BLUE_KEY + 2) == 0) {
		fdActorSetPos(ACTOR_THIRD_BLUE_KEY, 328, 760);
	}
	if(gmFlagGet(FLAG_CROSSBONE_ISLE_FIRST_BLUE_KEY + 3) == 0) {
		fdActorSetPos(ACTOR_FOURTH_BLUE_KEY, 360, 872);
	}
	if(gmFlagGet(FLAG_CROSSBONE_ISLE_FIRST_BLUE_DOOR_OPEN) != 0) {
		scCrossboneIsleKeysOpenBlueDoor(0);
	}
	if(gmFlagGet(FLAG_CROSSBONE_ISLE_FIRST_BLUE_DOOR_OPEN + 1) != 0) {
		scCrossboneIsleKeysOpenBlueDoor(1);
	}
	if(gmFlagGet(FLAG_CROSSBONE_ISLE_FIRST_BLUE_DOOR_OPEN + 2) != 0) {
		scCrossboneIsleKeysOpenBlueDoor(2);
	}
	if(gmFlagGet(FLAG_CROSSBONE_ISLE_FIRST_BLUE_DOOR_OPEN + 3) != 0) {
		scCrossboneIsleKeysOpenBlueDoor(3);
	}
	if(gmFlagGet(FLAG_CROSSBONE_ISLE_RED_DOOR_OPEN) != 0) {
		scCrossboneIsleKeysOpenRedDoor(0);
	}
}

/* Makes the first four statues solid and draws all five a little narrower,
   starts the watches for Isaac in front of the first, second and fifth, and
   lays the darkness over the hall, half see-through. */
static void scCrossboneIsleKeysInitStatues(void)
{
	fdActorGet(ACTOR_FIRST_STATUE)->collisionFlg = ACTOR_SOLID;
	fdActorGet(ACTOR_SECOND_STATUE)->collisionFlg = ACTOR_SOLID;
	fdActorGet(ACTOR_THIRD_STATUE)->collisionFlg = ACTOR_SOLID;
	fdActorGet(ACTOR_FOURTH_STATUE)->collisionFlg = ACTOR_SOLID;
	fdActorGet(ACTOR_FIRST_STATUE)->scaleX = toFIXED(7) / 10;
	fdActorGet(ACTOR_SECOND_STATUE)->scaleX = toFIXED(7) / 10;
	fdActorGet(ACTOR_THIRD_STATUE)->scaleX = toFIXED(7) / 10;
	fdActorGet(ACTOR_FOURTH_STATUE)->scaleX = toFIXED(7) / 10;
	fdActorGet(ACTOR_FIFTH_STATUE)->scaleX = toFIXED(7) / 10;
	sySetOrUpdateTickEvent(
		scCrossboneIsleKeysCheckFifthStatueCue, EVENT_PASS_TICK);
	sySetOrUpdateTickEvent(
		scCrossboneIsleKeysCheckSecondStatueCue, EVENT_PASS_TICK);
	sySetTickEvent(scCrossboneIsleKeysCheckFirstStatueCue, EVENT_PASS_TICK);
	IO_WRITE16(
		BLDCNT_TGT1_BG1 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL, &REG_BLDCNT);
	IO_WRITE16(BLEND_ALPHA(7, 6), &REG_BLDALPHA);
}

/* Lays the metatiles under each pillar where it stands, refreezes the puddle if
   Frost froze it since the party came in, and puts Bane where its last escape
   left it:
   at the west end of the back ledge, on the low ledge below it, at the east end
   of the back ledge, or on the spiked ledge in the south-east. */
static void scCrossboneIsleKeysInitBane(void)
{
	ACTOR *djinni;

	fdActorGet(ACTOR_SECOND_PILLAR_CHEST)->motionFlg = 0;
	sySetOrUpdateTickEvent(
		scCrossboneIsleKeysFollowSecondPillar, EVENT_PASS_TICK);
	sySetTickEvent(scCrossboneIsleKeysUpdatePillarPriorities, EVENT_PASS_TICK);
	fdTreasureSpotSetPos(TREASURE_SPOT_DEMON_AXE, 0, 0);
	if(gmFlagGet(FLAG_CROSSBONE_ISLE_DEMON_AXE) != 0) {
		fdActorSetAnimation(
			ACTOR_SECOND_PILLAR_CHEST, CHEST_ANIMATION_OPEN_EMPTY);
	}
	scCrossboneIsleKeysMoveFirstPillar();
	scCrossboneIsleKeysMoveSecondPillar();
	scCrossboneIsleKeysMoveThirdPillar();
	scCrossboneIsleKeysMoveFourthPillar();
	scCrossboneIsleKeysMoveFifthPillar();
#if BANE_FIRST_PILLAR_LOWEST_SPRITE_PRIORITY
	fdActorSetSprPriority(ACTOR_FIRST_PILLAR, 3);
#endif
	fdActorGet(ACTOR_FOURTH_PILLAR)->motionFlg = 0;
	fdActorGet(ACTOR_FIFTH_PILLAR)->motionFlg = 0;
	scCrossboneIsleKeysRepaintFourthAndFifthPillarMetatiles();
	if(gmFlagGet(FLAG_CROSSBONE_ISLE_BANE_PUDDLE_FROZEN) != 0) {
		scCrossboneIsleKeysFreezePuddle();
		fdActorSetAnimation(ACTOR_PUDDLE, PUDDLE_ANIMATION_PILLAR);
	}

	if(gmFlagGet(FLAG_KEEP_PARTY_POSITION) == 0) {
		if(gmFlagGet(FLAG_CROSSBONE_ISLE_BANE_LAST_ESCAPE) != 0) {
			fdActorSetPos(ACTOR_BANE, 856, 824);
			fdActorGet(ACTOR_BANE)->update =
				(void (*)(ACTOR *))scCrossboneIsleKeysFaceBaneTowardLeader;
		} else if(gmFlagGet(FLAG_CROSSBONE_ISLE_BANE_SECOND_ESCAPE) != 0) {
			fdActorSetPos(ACTOR_BANE, 888, 664);
			djinni = fdActorGet(ACTOR_BANE);
			djinni->spr->rotation = 0;
			fdActorSetAnimationSpeed(djinni, ANIM_STEP_NORMAL);
		} else if(gmFlagGet(FLAG_CROSSBONE_ISLE_BANE_FIRST_ESCAPE) != 0) {
			fdActorSetPos(ACTOR_BANE, 584, 680);
		} else {
			fdActorSetPos(ACTOR_BANE, 584, 664);
		}
	}
}

#include "shared/pushed_actor_tables.c"
/* clang-format off */

Sint32 CrossboneIsleKeysBaneSquashScript[] = {
	SC_ADD(SW_SCALE_X, -toFIXED(1) / 4),
	SC_ADD(SW_SCALE_Y, toFIXED(1) / 8),
	SC_WAIT(2),
	SC_ADD(SW_SCALE_X, toFIXED(1) / 8),
	SC_ADD(SW_SCALE_Y, -toFIXED(1) / 16),
	SC_WAIT(2),
	SC_ADD(SW_SCALE_X, toFIXED(1) / 16),
	SC_ADD(SW_SCALE_Y, -toFIXED(1) / 32),
	SC_WAIT(2),
	SC_ADD(SW_SCALE_X, toFIXED(1) / 16),
	SC_ADD(SW_SCALE_Y, -toFIXED(1) / 32),
	SC_WAIT(4),
	SC_ADD(SW_SCALE_X, -toFIXED(1) / 4),
	SC_ADD(SW_SCALE_Y, toFIXED(1) / 8),
	SC_WAIT(2),
	SC_ADD(SW_SCALE_X, toFIXED(1) / 8),
	SC_ADD(SW_SCALE_Y, -toFIXED(1) / 16),
	SC_WAIT(2),
	SC_ADD(SW_SCALE_X, toFIXED(1) / 16),
	SC_ADD(SW_SCALE_Y, -toFIXED(1) / 32),
	SC_WAIT(2),
	SC_ADD(SW_SCALE_X, toFIXED(1) / 16),
	SC_ADD(SW_SCALE_Y, -toFIXED(1) / 32),
	SC_WAIT(64),
	SC_LOOP(SC_FOREVER, SC_TOP),
};

/* Where Bane runs to, in pixels, when Isaac steps onto each of the twelve */
/* metatiles round the three spikes on the south-east ledge, from */
/* METATILE_BEHAVIOR_BANE_CORNER_FIRST on: always the spot across the spikes */
/* from him. */
Sint32 CrossboneIsleKeysBaneWalkTargets[][2] = {
	{ 888, 856 },
	{ 872, 856 },
	{ 856, 856 },
	{ 840, 856 },
	{ 824, 856 },
	{ 824, 840 },
	{ 824, 824 },
	{ 840, 824 },
	{ 856, 824 },
	{ 872, 824 },
	{ 888, 824 },
	{ 888, 840 },
};

#include "shared/fallback_entrances.c"

SCENE_ENTRANCE CrossboneIsleKeysRoomEntrances[] = {
	{ CROSSBONE_ISLE_KEYS_FROM_CROSSBONE_ISLE_THIRD_FLOOR_EAST, CONDITION_ALWAYS, 200, 0, 968, FACING_NORTH, 0, 24, 560, 504, 1008, 0 },
	{ CROSSBONE_ISLE_KEYS_FROM_CROSSBONE_ISLE_THIRD_FLOOR_WEST, CONDITION_ALWAYS, 88, 0, 968, FACING_NORTH, 0, 24, 560, 504, 1008, 0 },
	{ SCENE_TABLE_END },
};

SCENE_ENTRANCE CrossboneIsleStatuesEntrances[] = {
	{ CROSSBONE_ISLE_STATUES_FROM_CROSSBONE_ISLE_FOURTH_FLOOR_EAST, CONDITION_ALWAYS, 200, 0, 408, FACING_NORTH, 0, 40, 0, 464, 440, 0 },
	{ CROSSBONE_ISLE_STATUES_FROM_CROSSBONE_ISLE_FOURTH_FLOOR_WEST, CONDITION_ALWAYS, 88, 0, 392, FACING_NORTH, 0, 40, 0, 464, 440, 0 },
	{ SCENE_TABLE_END },
};

SCENE_ENTRANCE CrossboneIsleBaneEntrances[] = {
	{ CROSSBONE_ISLE_BANE_FROM_CROSSBONE_ISLE_SIXTH_FLOOR_EAST, CONDITION_ALWAYS, 776, 0, 952, FACING_NORTH, 0, 512, 544, 928, 992, 0 },
	{ CROSSBONE_ISLE_BANE_FROM_CROSSBONE_ISLE_SIXTH_FLOOR_WEST, CONDITION_ALWAYS, 664, 0, 952, FACING_NORTH, 0, 512, 544, 928, 992, 0 },
	{ SCENE_TABLE_END },
};

Uint32 CrossboneIsleKeysExits[] = {
	SCENE_EXITS(CrossboneIsleKeys),
	SCENE_EXIT(CROSSBONE_ISLE_KEYS_TO_CROSSBONE_ISLE_THIRD_FLOOR_EAST, CrossboneIsleThirdFloor, CROSSBONE_ISLE_THIRD_FLOOR_FROM_CROSSBONE_ISLE_KEYS_EAST),
	SCENE_EXIT(CROSSBONE_ISLE_KEYS_TO_CROSSBONE_ISLE_THIRD_FLOOR_WEST, CrossboneIsleThirdFloor, CROSSBONE_ISLE_THIRD_FLOOR_FROM_CROSSBONE_ISLE_KEYS_WEST),
	SCENE_EXITS(CrossboneIsleStatues),
	SCENE_EXIT(CROSSBONE_ISLE_STATUES_TO_CROSSBONE_ISLE_FOURTH_FLOOR_EAST, CrossboneIsleFourthFloor, CROSSBONE_ISLE_FOURTH_FLOOR_FROM_CROSSBONE_ISLE_STATUES_EAST),
	SCENE_EXIT(CROSSBONE_ISLE_STATUES_TO_CROSSBONE_ISLE_FOURTH_FLOOR_WEST, CrossboneIsleFourthFloor, CROSSBONE_ISLE_FOURTH_FLOOR_FROM_CROSSBONE_ISLE_STATUES_WEST),
	SCENE_EXITS(CrossboneIsleBane),
	SCENE_EXIT(CROSSBONE_ISLE_BANE_TO_CROSSBONE_ISLE_SIXTH_FLOOR_EAST, CrossboneIsleSixthFloor, CROSSBONE_ISLE_SIXTH_FLOOR_FROM_CROSSBONE_ISLE_BANE_EAST),
	SCENE_EXIT(CROSSBONE_ISLE_BANE_TO_CROSSBONE_ISLE_SIXTH_FLOOR_WEST, CrossboneIsleSixthFloor, CROSSBONE_ISLE_SIXTH_FLOOR_FROM_CROSSBONE_ISLE_BANE_WEST),
	SCENE_EXITS_END,
};

ACTOR_PLACE CrossboneIsleKeysActorPlacementsOther[] = {
	{ SCENE_TABLE_END },
};

ACTOR_PLACE CrossboneIsleKeysRoomActorPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_BLANK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		80, 0, 1016, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_BLANK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		80, 0, 1016, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_BLANK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		80, 0, 1016, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_BLANK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		80, 0, 1016, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_BLANK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		80, 0, 1016, FACING_SOUTH, TALK_FACING_KEEP, 0),
	{ SCENE_TABLE_END },
};

ACTOR_PLACE CrossboneIsleStatuesActorPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_LADY_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		360, 0, 152, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_LADY_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		168, 0, 264, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_LADY_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		264, 0, 360, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_LADY_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		344, 0, 360, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_LADY_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		312, 0, 232, FACING_SOUTH, TALK_FACING_KEEP, 0),
	{ SCENE_TABLE_END },
};

ACTOR_PLACE CrossboneIsleBaneActorPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		616, 0, 680, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		728, 0, 744, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		600, 0, 856, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		824, 0, 888, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		840, 0, 888, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		648, 0, 888, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_CHEST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		728, 0, 760, FACING_SOUTH, TALK_FACING_KEEP, 0),
	/* Bane, until it is caught */
	ACTOR_PLACEMENT(SPRITE_VENUS_DJINNI, FLAG_DJINNI(DJINNI_BANE), MOVEMENT_TYPE_STAND,
		0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
	{ SCENE_TABLE_END },
};

CUE CrossboneIsleKeysCuesOther[] = {
	{ CUE_TABLE_END },
};

CUE CrossboneIsleKeysRoomCues[] = {
	{ CUE_EXIT, 1, CONDITION_ALWAYS, CROSSBONE_ISLE_KEYS_TO_CROSSBONE_ISLE_THIRD_FLOOR_EAST },
	{ CUE_EXIT, 2, CONDITION_ALWAYS, CROSSBONE_ISLE_KEYS_TO_CROSSBONE_ISLE_THIRD_FLOOR_WEST },
	{ PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0, CONDITION_ALWAYS, SCRIPT_REF(scCrossboneIsleKeysShowRedKey) },
	{ PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, SCRIPT_REF(scCrossboneIsleKeysHideRedKey) },
	{ PSYNERGY_ON_ACTOR(ABILITY_CATCH), ACTOR_FIRST_BLUE_KEY, CONDITION_ALWAYS, SCRIPT_REF(scCrossboneIsleKeysCatchFirstBlueKey) },
	{ PSYNERGY_ON_ACTOR(ABILITY_CATCH), ACTOR_SECOND_BLUE_KEY, CONDITION_ALWAYS, SCRIPT_REF(scCrossboneIsleKeysCatchSecondBlueKey) },
	{ PSYNERGY_ON_ACTOR(ABILITY_CATCH), ACTOR_THIRD_BLUE_KEY, CONDITION_ALWAYS, SCRIPT_REF(scCrossboneIsleKeysCatchThirdBlueKey) },
	{ PSYNERGY_ON_ACTOR(ABILITY_CATCH), ACTOR_FOURTH_BLUE_KEY, CONDITION_ALWAYS, SCRIPT_REF(scCrossboneIsleKeysCatchFourthBlueKey) },
	{ PSYNERGY_ON_ACTOR(ABILITY_CATCH), ACTOR_RED_KEY, CONDITION_ALWAYS, SCRIPT_REF(scCrossboneIsleKeysCatchRedKey) },
	{ CUE_STEP, METATILE_BEHAVIOR_KEYS_RED_DOOR, CONDITION_ALWAYS, SCRIPT_REF(scCrossboneIsleKeysUseRedKey) },
	{ CUE_STEP, METATILE_BEHAVIOR_KEYS_FIRST_BLUE_DOOR, CONDITION_ALWAYS, SCRIPT_REF(scCrossboneIsleKeysOpenFirstBlueDoor) },
	{ CUE_STEP, METATILE_BEHAVIOR_KEYS_SECOND_BLUE_DOOR, CONDITION_ALWAYS, SCRIPT_REF(scCrossboneIsleKeysOpenSecondBlueDoor) },
	{ CUE_STEP, METATILE_BEHAVIOR_KEYS_THIRD_BLUE_DOOR, CONDITION_ALWAYS, SCRIPT_REF(scCrossboneIsleKeysOpenThirdBlueDoor) },
	{ CUE_STEP, METATILE_BEHAVIOR_KEYS_FOURTH_BLUE_DOOR, CONDITION_ALWAYS, SCRIPT_REF(scCrossboneIsleKeysOpenFourthBlueDoor) },
	/* Unused: the plain step rows above answer the same metatiles first. */
	{ STEP_CUE_WALKING_INTO(FACING_SOUTH), METATILE_BEHAVIOR_KEYS_THIRD_BLUE_DOOR, CONDITION_ALWAYS, SCRIPT_REF(scCrossboneIsleKeysUnusedJumpOntoPost) },
	{ STEP_CUE_WALKING_INTO(FACING_SOUTH), METATILE_BEHAVIOR_KEYS_FOURTH_BLUE_DOOR, CONDITION_ALWAYS, SCRIPT_REF(scCrossboneIsleKeysUnusedJumpOntoPost) },
	{ EXAMINE_CUE(EXAMINE_CHEST), TREASURE_SPOT_KEYS_333_COINS, FLAG_CROSSBONE_ISLE_KEYS_333_COINS, FIND_COINS(333) },
	{ EXAMINE_CUE(EXAMINE_CHEST), TREASURE_SPOT_KEYS_FAIRY_RING, FLAG_CROSSBONE_ISLE_KEYS_FAIRY_RING, FIND_ITEM(ITEM_FAIRY_RING) },
	{ EXAMINE_CUE(EXAMINE_CHEST), TREASURE_SPOT_KEYS_COOKIE, FLAG_CROSSBONE_ISLE_KEYS_COOKIE, FIND_ITEM(ITEM_COOKIE) },
	{ EXAMINE_CUE(EXAMINE_CHEST), TREASURE_SPOT_KEYS_SMOKE_BOMB, FLAG_CROSSBONE_ISLE_KEYS_SMOKE_BOMB, FIND_ITEM(ITEM_SMOKE_BOMB) },
	{ CUE_TABLE_END },
};

CUE CrossboneIsleStatuesCues[] = {
	{ CUE_EXIT, 1, CONDITION_ALWAYS, CROSSBONE_ISLE_STATUES_TO_CROSSBONE_ISLE_FOURTH_FLOOR_EAST },
	{ CUE_EXIT, 2, CONDITION_ALWAYS, CROSSBONE_ISLE_STATUES_TO_CROSSBONE_ISLE_FOURTH_FLOOR_WEST },
	{ CUE_STEP, METATILE_BEHAVIOR_STATUES_BEFORE_THIRD_STATUE, CONDITION_ALWAYS, SCRIPT_REF(scCrossboneIsleKeysThirdStatueShadowsLeader) },
	{ CUE_STEP, METATILE_BEHAVIOR_STATUES_BEFORE_FOURTH_STATUE, CONDITION_ALWAYS, SCRIPT_REF(scCrossboneIsleKeysFourthStatueShadowsLeader) },
	{ CUE_STEP, METATILE_BEHAVIOR_STATUES_THIRD_ALCOVE, CONDITION_ALWAYS, SCRIPT_REF(scCrossboneIsleKeysThirdStatuePushesLeaderBack) },
	{ CUE_STEP, METATILE_BEHAVIOR_STATUES_FOURTH_ALCOVE, CONDITION_ALWAYS, SCRIPT_REF(scCrossboneIsleKeysFourthStatuePushesLeaderBack) },
	{ CUE_RAISED, RAISED_FIFTH_CHASE, CONDITION_ALWAYS, SCRIPT_REF(scCrossboneIsleKeysFifthStatuePushesLeaderBack) },
	{ CUE_RAISED, RAISED_SECOND_CHASE, CONDITION_ALWAYS, SCRIPT_REF(scCrossboneIsleKeysSecondStatuePushesLeaderBack) },
	{ CUE_RAISED, RAISED_FIRST_CHASE, CONDITION_ALWAYS, SCRIPT_REF(scCrossboneIsleKeysFirstStatuePushesLeaderBack) },
	{ PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0, CONDITION_ALWAYS, SCRIPT_REF(scCrossboneIsleKeysCastRevealInStatueRoom) },
	{ PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, SCRIPT_REF(scCrossboneIsleKeysRevealEndsInStatueRoom) },
	{ PSYNERGY_ON_ACTOR(ABILITY_HALT), ACTOR_FIRST_STATUE, CONDITION_ALWAYS, SCRIPT_REF(scCrossboneIsleKeysHaltStatue) },
	{ PSYNERGY_ON_ACTOR(ABILITY_HALT), ACTOR_SECOND_STATUE, CONDITION_ALWAYS, SCRIPT_REF(scCrossboneIsleKeysHaltStatue) },
	{ PSYNERGY_ON_ACTOR(ABILITY_HALT), ACTOR_THIRD_STATUE, CONDITION_ALWAYS, SCRIPT_REF(scCrossboneIsleKeysHaltStatue) },
	{ PSYNERGY_ON_ACTOR(ABILITY_HALT), ACTOR_FOURTH_STATUE, CONDITION_ALWAYS, SCRIPT_REF(scCrossboneIsleKeysHaltStatue) },
	{ PSYNERGY_ON_ACTOR(ABILITY_HALT), ACTOR_FIFTH_STATUE, CONDITION_ALWAYS, SCRIPT_REF(scCrossboneIsleKeysHaltStatue) },
	{ EXAMINE_CUE(EXAMINE_CHEST), TREASURE_SPOT_STATUES_444_COINS, FLAG_CROSSBONE_ISLE_STATUES_444_COINS, FIND_COINS(444) },
	{ EXAMINE_CUE(EXAMINE_CHEST), TREASURE_SPOT_STATUES_SLEEP_BOMB, FLAG_CROSSBONE_ISLE_STATUES_SLEEP_BOMB, FIND_ITEM(ITEM_SLEEP_BOMB) },
	{ EXAMINE_CUE(EXAMINE_CHEST), TREASURE_SPOT_STATUES_STORM_GEAR, FLAG_CROSSBONE_ISLE_STATUES_STORM_GEAR, FIND_ITEM(ITEM_STORM_GEAR) },
	{ EXAMINE_CUE(EXAMINE_CHEST), TREASURE_SPOT_STATUES_PSY_CRYSTAL, FLAG_CROSSBONE_ISLE_STATUES_PSY_CRYSTAL, FIND_ITEM(ITEM_PSY_CRYSTAL) },
	{ CUE_TABLE_END },
};
/* clang-format on */

CUE CrossboneIsleBaneCues[] = {
	{CUE_EXIT, 1, CONDITION_ALWAYS,
		CROSSBONE_ISLE_BANE_TO_CROSSBONE_ISLE_SIXTH_FLOOR_EAST},
	{CUE_EXIT, 2, CONDITION_ALWAYS,
		CROSSBONE_ISLE_BANE_TO_CROSSBONE_ISLE_SIXTH_FLOOR_WEST},
	{CUE_STEP | STEP_CUE_FLAG_WALKING, METATILE_BEHAVIOR_BANE_THIRD_PILLAR,
		CONDITION_ALWAYS, SCRIPT_REF(scCrossboneIsleKeysPushThirdPillar)},
	{CUE_STEP | STEP_CUE_FLAG_WALKING, METATILE_BEHAVIOR_BANE_FIRST_PILLAR,
		CONDITION_ALWAYS, SCRIPT_REF(scCrossboneIsleKeysPushFirstPillar)},
	{CUE_STEP | STEP_CUE_FLAG_WALKING, METATILE_BEHAVIOR_BANE_SECOND_PILLAR,
		CONDITION_ALWAYS, SCRIPT_REF(scCrossboneIsleKeysPushSecondPillar)},
	{CUE_STEP | STEP_CUE_FLAG_WALKING, METATILE_BEHAVIOR_BANE_FOURTH_PILLAR,
		CONDITION_ALWAYS,
		SCRIPT_REF(scCrossboneIsleKeysPushFourthOrFifthPillar)},
#if BANE_WEST_OF_FIRST_PILLAR_OWN_CELL_BEHAVIOR
	{CUE_STEP | STEP_CUE_FLAG_WALKING,
		METATILE_BEHAVIOR_BANE_WEST_OF_FIRST_PILLAR, CONDITION_ALWAYS,
		SCRIPT_REF(scCrossboneIsleKeysPushFirstPillar)},
#endif
	{CUE_STEP | STEP_CUE_FLAG_WALKING, METATILE_BEHAVIOR_BANE_THIRD_PILLAR_EDGE,
		CONDITION_ALWAYS, SCRIPT_REF(scCrossboneIsleKeysTouchThirdPillar)},
	{CUE_STEP | STEP_CUE_FLAG_WALKING, METATILE_BEHAVIOR_BANE_LADDER,
		CONDITION_ALWAYS, SCRIPT_REF(scCrossboneIsleKeysUseLadder)},
	{CUE_STEP | STEP_CUE_FLAG_WALKING,
		METATILE_BEHAVIOR_BANE_EAST_OF_FIRST_PILLAR, CONDITION_ALWAYS,
		SCRIPT_REF(scCrossboneIsleKeysJumpEastOrPushFirstPillar)},
	{CUE_STEP, METATILE_BEHAVIOR_BANE_FIRST_ESCAPE, FLAG_DJINNI(DJINNI_BANE),
		SCRIPT_REF(scCrossboneIsleKeysBaneFirstEscape)},
	{CUE_STEP, METATILE_BEHAVIOR_BANE_SECOND_ESCAPE, FLAG_DJINNI(DJINNI_BANE),
		SCRIPT_REF(scCrossboneIsleKeysBaneSecondEscape)},
	{CUE_STEP, METATILE_BEHAVIOR_BANE_LAST_ESCAPE, FLAG_DJINNI(DJINNI_BANE),
		SCRIPT_REF(scCrossboneIsleKeysBaneLastEscape)},
	{CUE_STEP, METATILE_BEHAVIOR_BANE_CORNER_FIRST, FLAG_DJINNI(DJINNI_BANE),
		SCRIPT_REF(scCrossboneIsleKeysBaneKeepsAway)},
	{CUE_STEP, METATILE_BEHAVIOR_BANE_CORNER_FIRST + 1,
		FLAG_DJINNI(DJINNI_BANE), SCRIPT_REF(scCrossboneIsleKeysBaneKeepsAway)},
	{CUE_STEP, METATILE_BEHAVIOR_BANE_CORNER_FIRST + 2,
		FLAG_DJINNI(DJINNI_BANE), SCRIPT_REF(scCrossboneIsleKeysBaneKeepsAway)},
	{CUE_STEP, METATILE_BEHAVIOR_BANE_CORNER_FIRST + 3,
		FLAG_DJINNI(DJINNI_BANE), SCRIPT_REF(scCrossboneIsleKeysBaneKeepsAway)},
	{CUE_STEP, METATILE_BEHAVIOR_BANE_CORNER_FIRST + 4,
		FLAG_DJINNI(DJINNI_BANE), SCRIPT_REF(scCrossboneIsleKeysBaneKeepsAway)},
	{CUE_STEP, METATILE_BEHAVIOR_BANE_CORNER_FIRST + 5,
		FLAG_DJINNI(DJINNI_BANE), SCRIPT_REF(scCrossboneIsleKeysBaneKeepsAway)},
	{CUE_STEP, METATILE_BEHAVIOR_BANE_CORNER_FIRST + 6,
		FLAG_DJINNI(DJINNI_BANE), SCRIPT_REF(scCrossboneIsleKeysBaneKeepsAway)},
	{CUE_STEP, METATILE_BEHAVIOR_BANE_CORNER_FIRST + 7,
		FLAG_DJINNI(DJINNI_BANE), SCRIPT_REF(scCrossboneIsleKeysBaneKeepsAway)},
	{CUE_STEP, METATILE_BEHAVIOR_BANE_CORNER_FIRST + 8,
		FLAG_DJINNI(DJINNI_BANE), SCRIPT_REF(scCrossboneIsleKeysBaneKeepsAway)},
	{CUE_STEP, METATILE_BEHAVIOR_BANE_CORNER_FIRST + 9,
		FLAG_DJINNI(DJINNI_BANE), SCRIPT_REF(scCrossboneIsleKeysBaneKeepsAway)},
	{CUE_STEP, METATILE_BEHAVIOR_BANE_CORNER_FIRST + 10,
		FLAG_DJINNI(DJINNI_BANE), SCRIPT_REF(scCrossboneIsleKeysBaneKeepsAway)},
	{CUE_STEP, METATILE_BEHAVIOR_BANE_CORNER_FIRST + 11,
		FLAG_DJINNI(DJINNI_BANE), SCRIPT_REF(scCrossboneIsleKeysBaneKeepsAway)},
	{CUE_TALK_HALTED, ACTOR_BANE, CONDITION_ALWAYS,
		SCRIPT_REF(scCrossboneIsleKeysBaneJoins)},
	{PSYNERGY_ON_ACTOR(ABILITY_HALT), ACTOR_BANE, CONDITION_ALWAYS,
		SCRIPT_REF(scCrossboneIsleKeysHaltBane)},
	{PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_PUDDLE,
		FLAG_CROSSBONE_ISLE_BANE_PUDDLE_FROZEN,
		SCRIPT_REF(scCrossboneIsleKeysFreezePuddle)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_PILLAR, FLAG_SCENE_TEMP_01,
		SCRIPT_REF(scCrossboneIsleKeysMoveFirstPillar)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_SECOND_PILLAR,
		FLAG_CROSSBONE_ISLE_BANE_CHEST_AT_LADDER,
		SCRIPT_REF(scCrossboneIsleKeysMoveSecondPillar)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_THIRD_PILLAR, FLAG_SCENE_TEMP_03,
		SCRIPT_REF(scCrossboneIsleKeysMoveThirdPillar)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FOURTH_PILLAR, FLAG_SCENE_TEMP_04,
		SCRIPT_REF(scCrossboneIsleKeysMoveFourthPillar)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIFTH_PILLAR, FLAG_SCENE_TEMP_05,
		SCRIPT_REF(scCrossboneIsleKeysMoveFifthPillar)},
	{EXAMINE_CUE(EXAMINE_CHEST), TREASURE_SPOT_BANE_666_COINS,
		FLAG_CROSSBONE_ISLE_BANE_666_COINS, FIND_COINS(666)},
	{EXAMINE_CUE(EXAMINE_CHEST), TREASURE_SPOT_BANE_WATER_OF_LIFE,
		FLAG_CROSSBONE_ISLE_BANE_WATER_OF_LIFE, FIND_ITEM(ITEM_WATER_OF_LIFE)},
	{EXAMINE_CUE(EXAMINE_CHEST), TREASURE_SPOT_BANE_ELIXIR,
		FLAG_CROSSBONE_ISLE_BANE_ELIXIR, FIND_ITEM(ITEM_ELIXIR)},
	{EXAMINE_CUE(EXAMINE_CHEST), TREASURE_SPOT_DEMON_AXE,
		FLAG_CROSSBONE_ISLE_DEMON_AXE, FIND_ITEM(ITEM_DEMON_AXE)},
	{CUE_TABLE_END},
};

/* The column the third and fourth statues stand in as they slide to stay in
   front of Isaac; the third entry is the fifth statue's, which nothing
   slides. */
Uint32 CrossboneIsleKeysStatueColumns[] = {
	16,
	21,
	0,
};

/* Unused: the tick counter of scCrossboneIsleKeysUnusedRedrawKeyActors. */
Sint32 CrossboneIsleKeysIconTimer = 0;
