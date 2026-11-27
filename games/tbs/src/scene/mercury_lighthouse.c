/* Mercury Lighthouse, the lighthouse of water north of Imil that Mia and the
   Mercury Clan guard: the ground outside its door
   (SceneId_MercuryLighthouseExterior) and three of its upper scenes
   (SceneId_MercuryLighthouseLower, SceneId_MercuryLighthouseMiddle and
   SceneId_MercuryLighthouseUpper). The ground floor inside the door, with the
   Lizard Man and the goddess's pool, is mercury_lighthouse_entrance.c, and the
   beacon at the top, the Lighthouse Aerie, is mercury_lighthouse_aerie.c. The
   four scenes here share one exit table, and every arrival sets
   FLAG_IN_MERCURY_LIGHTHOUSE, under which Mercury Adepts regain PP faster.
   Retreat leads out to the foot of the causeway
   (MERCURY_LIGHTHOUSE_EXTERIOR_FROM_WORLD_MAP).

   In the order the party meets them:

   - Outside. The party comes from the World Map up a causeway between two
	 blue statues standing in the water, past the dry Fountain of Healing and
	 its sign, to the lighthouse door. Until the party has met Mia in Imil
	 (FLAG_IMIL_MET_MIA) the door glows and searching it shows "Strange forces
	 are at work...". Then Mia kneels before it, and a stone statue stands in
	 her way. Spoken to, she senses someone inside; Move pushes the statue
	 aside, she asks whether Isaac did that, learns the word Psynergy from
	 Garet and Ivan, and opens the door with Ply before going in alone
	 (FLAG_MERCURY_LIGHTHOUSE_OPENED_BY_MIA).
   - SceneId_MercuryLighthouseLower: the floor above the goddess's pool. Its
	 great pool room is set with stepping squares, some marked with a cross.
	 Stairs from the platform in its middle lead to a ledge where Mia is stuck
	 again, before a blue statue that was not there before; Move drops it into
	 the gap at her feet and she joins the party with Fizz, a Mercury Djinni
	 (FLAG_MIA_JOINED_PARTY). In the pipe room three pipe sections pushed
	 across their grated pools join the channels (FLAG_PIPE_1_JOINED to
	 FLAG_PIPE_3_JOINED), and the water runs on into the rooms below
	 (FLAG_MERCURY_LIGHTHOUSE_CHANNELS_OPEN): before and after are
	 places/mercury-lighthouse-lower/map-4.png and
	 places/mercury-lighthouse-lower_mercury-lighthouse-channels-open/map-4.png.
	 In a small room a hole behind a blue crystal drops to the goddess's pool.
	 Once Ply has woken that pool (FLAG_MERCURY_LIGHTHOUSE_HEART_HONORED), the
	 cross-marked squares here give Isaac golden rings of light, each of which
	 holds him up on the water for one hop
	 (shared/mercury_lighthouse_pillar.c). That is the way across the great
	 pool room, whose chest holds a Sleep Bomb.
   - SceneId_MercuryLighthouseMiddle. In the big pipe room one pipe section,
	 joined, sets water spouting from four pipe ends (FLAG_SPOUTS_RUNNING); a
	 chest there holds a Psy Crystal. In the gate room a blue statue moved
	 onto the floor switch holds the sealed arch on the dais open for good
	 (FLAG_MERCURY_LIGHTHOUSE_GATE_HELD_OPEN), where Isaac standing on the
	 switch holds it only while he stays; past it is the crystal room.
	 In the room with the Nut chest a pipe joined across its pool sends a jet
	 along the north wall and opens a doorway up to the room of six
	 waterfalls (FLAG_NORTH_JET_RUNNING). Sleet, a Mercury Djinni, waits in a
	 room of its own and fights when spoken to.
   - SceneId_MercuryLighthouseUpper. Walking into the third waterfall from the
	 west of the first room of six waterfalls leads down to Sleet. In the room
	 of jets a pipe pushed along its pool turns the water from one pipe end to
	 another, and each jet pushes a blue statue a step, until it slides into a
	 square hole and falls to the gate room below
	 (FLAG_MERCURY_LIGHTHOUSE_STATUE_FELL). There Move sets it on the floor
	 switch, which holds the middle arch open
	 (FLAG_MERCURY_LIGHTHOUSE_UPPER_GATE_HELD_OPEN). Past it, five stone
	 statues stand along the wall; the third and the fifth hide doorways down
	 to the room with the Nut chest. In the second room of six waterfalls the
	 easternmost leads to a room of vines, where Move pushes a stone statue
	 off the doorway to the paths outside.
   - Outside again, high up. The paths end at a waterfall under a rainbow,
	 whose riddle appears on arrival: "He who honors the goddess of rainbows
	 shall be guided to the heavens upon wings of fluid grace." Ply on the
	 blue statue beside it reverses the waterfall, and Mia, Isaac, Garet and
	 Ivan rise up the water one by one to the Lighthouse Aerie, where Saturos
	 waits (mercury_lighthouse_aerie.c). Once he is beaten
	 (FLAG_SATUROS_BEATEN_AT_MERCURY) the lift brings the party down from the
	 aerie to the fountain, which flows again. People come to drink from it,
	 and an Empty Bottle used there becomes Hermes' Water, once a visit
	 (FLAG_HERMES_WATER_TAKEN). The waterfall still carries the leader up,
	 alone. */

#include "scene/mercury_lighthouse.h"
#include "ability_ids.h"
#include "flags.h"
#include "item_ids.h"
#include "battle/battle.h"
#include "field/facing.h"
#include "field/field_psynergy_state.h"
#include "field/ground_mask.h"
#include "field/map_grid_scroll.h"
#include "graphics/graphics.h"
#include "scene/mercury_lighthouse_lights.h"
#include "scene/mercury_lighthouse_pillar.h"
#include "scene/walk_into_leader.h"
#include "game/system_call.h"
#include "text/msg_ids.h"

extern Sint32		  MercuryLighthouse_RippleScript[];
extern Sint32		  Glint_TwinkleScript[];
extern Sint32		  MercuryLighthouse_StatueSlideWest[];
extern Sint32		  MercuryLighthouse_StatueSlideSouth[];
extern Sint32		  MercuryLighthouse_StatueSlideIntoHole[];
extern Sint32		  MercuryLighthouse_StatueStaysPut[];
extern Uint16		  MercuryLighthouse_GateMetatiles[];
extern Uint16		  MercuryLighthouse_GateCloseMetatiles[];
extern Uint16		  MercuryLighthouse_UpperGateMetatiles[];
extern Uint16		  MercuryLighthouse_UpperGateCloseMetatiles[];
extern Uint16		  MercuryLighthouse_ColumnMetatiles[];
extern SCENE_ENTRANCE MercuryLighthouseLowerEntrances[];
extern SCENE_ENTRANCE MercuryLighthouseMiddleEntrances[];
extern SCENE_ENTRANCE MercuryLighthouseUpperEntrances[];
extern SCENE_ENTRANCE MercuryLighthouseExteriorEntrances[];
extern Uint32		  MercuryLighthouse_Exits[];
extern ACTOR_PLACE	  MercuryLighthouseNoActorPlacements[];
extern ACTOR_PLACE	  MercuryLighthouseLowerActorPlacements[];
extern ACTOR_PLACE	  MercuryLighthouseMiddleActorPlacements[];
extern ACTOR_PLACE	  MercuryLighthouseUpperActorPlacements[];
extern ACTOR_PLACE	  MercuryLighthouseExteriorActorPlacements[];
extern CUE			  MercuryLighthouseLowerCues[];
extern CUE			  MercuryLighthouseMiddleCues[];
extern CUE			  MercuryLighthouseUpperCues[];
extern CUE			  MercuryLighthouseExteriorCues[];

/* How a spout carries Isaac back: in every edition but the Japanese the script
   waits until he is there. */
#if NUT_ROOM_JET_CARRIES_FURTHER
#define MERCURY_LIGHTHOUSE_CARRY_LEADER_BACK(actorId, x, z) \
	fdActorMoveToAndWait(actorId, x, z)
#else
#define MERCURY_LIGHTHOUSE_CARRY_LEADER_BACK(actorId, x, z) \
	fdActorSetDestination(actorId, x, z)
#endif

/* UB: this calls a void function through an Sint32 function type, the call the
   bytes show; UBFIX calls it as declared. */
#if FIX_FUNCTION_SIGNATURES
#define MERCURY_LIGHTHOUSE_ANIMATE_ACTOR_AND_WAIT fdActorSetAnimationAndWait
#else
#define MERCURY_LIGHTHOUSE_ANIMATE_ACTOR_AND_WAIT \
	((Sint32 (*)())fdActorSetAnimationAndWait)
#endif

static void scMercuryLighthouseOpenGateWithMovedStatue(void);
static void scMercuryLighthouseOpenUpperGateWithMovedStatue(void);
static void scMercuryLighthouseRecordRowStatues(void);
static void scMercuryLighthouseStartSpoutsWithSparkles(void);
static void scMercuryLighthouseStopSpoutsWithSparkles(void);
static void scMercuryLighthouseSparkleAroundLeader(void);
static void scMercuryLighthouseSparkleAt(Sint32 x, Sint32 y, Sint32 z);
static void scMercuryLighthouseSparkleNear(Sint32 x, Sint32 y, Sint32 z);
static void scMercuryLighthouseSparkleBurst(
	Sint32 x, Sint32 y, Sint32 z, Sint32 angle);
static void scMercuryLighthouseRideLift(void);
static void scMercuryLighthouseMiaCastsPlyOnEntrance(void);
static void scInventoryReplaceItem(Sint32 oldItem, Sint32 newItem);

/* Mia kneeling with her hands together, as she senses someone inside the
   lighthouse (mia.png's frame 76). */
enum { ANIM_MIA_KNEEL = 16 };

/* The metatiles the four scenes' cues answer, named for where they are. Exit
   metatiles are their exits' numbers past EXIT_METATILE_BEHAVIOR_OFFSET. */
enum {
	/* The great pool room: two metatiles of the floor beside the Sleep Bomb
	   chest, (7, 15) and (9, 15), which put out Isaac's lights like the pool's
	   plain squares but are no place to hop from. */
	METATILE_BEHAVIOR_SLEEP_BOMB_LEDGE = 32,
	/* The crystal room's hole behind its blue crystal, (29, 37), and the edge
	   of the gate room's terrace above another blue crystal, (13, 19). */
	METATILE_BEHAVIOR_BY_CRYSTAL = 15,
	/* Beside the three pipe sections of the pipe room. */
	METATILE_BEHAVIOR_CHANNEL_PIPE_PUSH = 25,
	/* Beside the spout pipe of the big pipe room. */
	METATILE_BEHAVIOR_SPOUT_PIPE_PUSH = 1,
	/* The three spouts of the big pipe room once they run: the one shooting
	   east from (10, 38), the one shooting south from (22, 41) and the one
	   shooting west at (16, 49) to (18, 49). */
	METATILE_BEHAVIOR_EAST_SPOUT = 2,
	METATILE_BEHAVIOR_SOUTH_SPOUT = 3,
	METATILE_BEHAVIOR_WEST_SPOUT = 4,
	/* Beside the pipe across the pool in the room with the Nut chest. */
	METATILE_BEHAVIOR_NORTH_JET_PIPE_PUSH = 45,
	/* The jet along that room's north wall once it runs, (43, 36) to (45, 36),
	   copied there from (43, 55) to (45, 55). */
	METATILE_BEHAVIOR_NORTH_JET = 46,
	/* Both gate rooms: the floor round the floor switch, which is also where
	   the gate statue is pushed from. */
	METATILE_BEHAVIOR_GATE_ROOM_FLOOR = 10,
	/* The floor switch of the gate room in the middle scene, (5, 13), and of
	   the one in the upper scene, (10, 23). */
	METATILE_BEHAVIOR_GATE_FLOOR_SWITCH = 12,
	/* The three metatiles round the middle scene's floor switch, (5, 12), (4,
	   13) and (6, 13). */
	METATILE_BEHAVIOR_BESIDE_GATE_FLOOR_SWITCH = 13,
	/* The sealed arch of either gate room, searched from in front. */
	METATILE_BEHAVIOR_SEALED_GATE = 20,
	/* Beside the two pipes of the room of jets. */
	METATILE_BEHAVIOR_JET_ROOM_PIPE_PUSH = 40,
	/* Beside the stone statues along the wall, (39, 5) to (53, 5), (57, 5) and
	   (59, 5). */
	METATILE_BEHAVIOR_ROW_STATUE_PUSH = 47,
	/* Outside: the lighthouse door while it is sealed, (52, 36). */
	METATILE_BEHAVIOR_SEALED_DOOR = 10,
	/* Outside: the open door, (52, 36) once Mia has opened it, and the way back
	   to the World Map at the foot of the causeway, (51, 54) to (53, 54). */
	METATILE_BEHAVIOR_LIGHTHOUSE_DOOR = 44,
	METATILE_BEHAVIOR_CAUSEWAY_FOOT = 43,
	/* Outside: the Fountain of Healing, (52, 47), and its sign, (52, 50). */
	METATILE_BEHAVIOR_HEALING_FOUNTAIN = 5,
	METATILE_BEHAVIOR_FOUNTAIN_SIGN = 42
};

/* MercuryLighthouseLower: the statue in Mia's way, and the pipe room's pipe
   sections and pipe end. */
typedef enum MERCURY_LIGHTHOUSE_LOWER_ACTOR_ID {
	ACTOR_STATUE_BLOCKING_MIA = ACTOR_FIRST_PLACED,
	/* Isaac pushes these pipe sections, by walking into them, across their
	   grated pools until each joins the pipes on either side
	   (SPRITE_PIPE_WEST_EAST for the first two, SPRITE_PIPE_NORTH_SOUTH for the
	   third). */
	ACTOR_FIRST_PIPE,
	ACTOR_SECOND_PIPE,
	ACTOR_THIRD_PIPE,
	/* The rim of the fixed pipe the third section joins. It is SPRITE_PIPE_END,
	   one still frame, 59 pixels south of where ACTOR_THIRD_PIPE comes to rest.
	   Pushing it does nothing, and nothing else moves it. */
	ACTOR_PIPE_END
} MERCURY_LIGHTHOUSE_LOWER_ACTOR_ID;

/* MercuryLighthouseMiddle: the spout pipe of the big pipe room, the pipe in the
   room with the Nut chest, the statue for the gate's floor switch and Sleet, a
   Mercury Djinni. */
typedef enum MERCURY_LIGHTHOUSE_MIDDLE_ACTOR_ID {
	ACTOR_SPOUT_PIPE = ACTOR_FIRST_PLACED,
	/* It stands 59 pixels south of where ACTOR_SPOUT_PIPE rests once it is
	   joined. Nothing moves it. */
	ACTOR_SPOUT_PIPE_END,
	ACTOR_NORTH_JET_PIPE,
	ACTOR_GATE_STATUE,
	ACTOR_SLEET
} MERCURY_LIGHTHOUSE_MIDDLE_ACTOR_ID;

/* MercuryLighthouseUpper: the blue statue of the room of jets and where it
   lands below, that room's two pipes and pipe ends, the five stone statues
   along the wall and the stone statue of the room of vines. */
typedef enum MERCURY_LIGHTHOUSE_UPPER_ACTOR_ID {
	/* The statue of ACTOR_SLIDING_STATUE once it has fallen to the gate room
	   below. */
	ACTOR_FALLEN_STATUE = ACTOR_FIRST_PLACED,
	/* At rest beside the jet that falls from the pipe end north of it. Each jet
	   that hits it pushes it a step, and the last slides it east over the floor
	   to the square hole at (840, 744), whose ground lies below the floor's,
	   and it falls through. */
	ACTOR_SLIDING_STATUE,
	/* The pipe section lying west to east, which only stands in the way. */
	ACTOR_WEST_EAST_PIPE,
	/* The pipe section lying north to south that turns the water from one jet
	   to another. */
	ACTOR_JET_SWITCH_PIPE,
	/* Three in a row south of the pool the switch pipe slides along. Nothing
	   moves them. */
	ACTOR_FIRST_JET_ROOM_PIPE_END,
	/* Five in a row along the north wall. */
	ACTOR_FIRST_ROW_STATUE = ACTOR_FIRST_JET_ROOM_PIPE_END + 3,
	ACTOR_LAST_ROW_STATUE = ACTOR_FIRST_ROW_STATUE + 4,
	ACTOR_GAP_STATUE
} MERCURY_LIGHTHOUSE_UPPER_ACTOR_ID;

/* MercuryLighthouseExterior: Garet, Ivan and Mia in their own party slots, the
   statues by the way in, the statue at the door, the Ply target, the statue by
   the waterfall, the statue on the lift and the people at the fountain. */
typedef enum MERCURY_LIGHTHOUSE_EXTERIOR_ACTOR_ID {
	/* A pair of statues either side of the way in from the World Map
	   (MERCURY_LIGHTHOUSE_EXTERIOR_FROM_WORLD_MAP). The east one is drawn
	   mirrored. */
	ACTOR_WEST_APPROACH_STATUE = ACTOR_FIRST_PLACED,
	ACTOR_EAST_APPROACH_STATUE,
	ACTOR_DOOR_STATUE,
	ACTOR_PLY_TARGET,
	ACTOR_WATERFALL_STATUE,
	ACTOR_LIFT_STATUE,
	/* The first of four people at the Fountain of Healing once Saturos is
	   beaten. In order they are a young man, a woman with fair hair, an old man
	   with a cane and an old woman. */
	ACTOR_FIRST_VISITOR
} MERCURY_LIGHTHOUSE_EXTERIOR_ACTOR_ID;

/* Puzzle states kept until the party leaves the lighthouse. */
enum {
	/* The big pipe room's spout pipe is joined and its spouts run. */
	FLAG_SPOUTS_RUNNING = FLAG_AREA_TEMP_02,
	/* The pipe across the pool in the room with the Nut chest is joined, and
	   the jet along the room's north wall runs. */
	FLAG_NORTH_JET_RUNNING = FLAG_AREA_TEMP_06,
	/* the camera has shown it start once */
	FLAG_NORTH_JET_SHOWN = FLAG_AREA_TEMP_07,
	/* The pipe room's first pipe section has been pushed into place, joining
	   the water pipes. With all three joined the water runs on below. */
	FLAG_PIPE_1_JOINED = FLAG_AREA_TEMP_10,
	FLAG_PIPE_2_JOINED = FLAG_AREA_TEMP_11,
	FLAG_PIPE_3_JOINED = FLAG_AREA_TEMP_12,
	/* Unused: cleared with the three, but nothing sets or tests it. */
	FLAG_UNUSED_PIPE_4_JOINED = FLAG_AREA_TEMP_13,
	FLAG_WEST_EAST_PIPE_PUSHED = FLAG_AREA_TEMP_18,
	/* Where the switch pipe rests in the room of jets, and so which jet runs
	   (scMercuryLighthousePushJetRoomPipes). At column 47 it is JET_WESTWARD,
	   along the sliding statue's row. */
	FLAG_SWITCH_PIPE_STARTS_WESTWARD_JET = FLAG_AREA_TEMP_19,
	/* At column 48 it is JET_EASTWARD, which slides the statue into the
	   hole. */
	FLAG_SWITCH_PIPE_STARTS_EASTWARD_JET = FLAG_AREA_TEMP_1A,
	/* At column 53 no jet runs. With none of the three set JET_SOUTHWARD
	   runs. */
	FLAG_SWITCH_PIPE_STOPS_JETS = FLAG_AREA_TEMP_1B,
	/* Two flags per stone statue of the row along the north wall, one for each
	   column it can be pushed to, then the last statue's. */
	FLAG_FIRST_ROW_STATUE = FLAG_AREA_TEMP_30,
	FLAG_LAST_ROW_STATUE_WEST = FLAG_AREA_TEMP_38,
	FLAG_LAST_ROW_STATUE_EAST = FLAG_AREA_TEMP_39,
	/* The stone statue of the room of vines has been moved west off the doorway
	   out to the paths, for good. */
	FLAG_PASSAGE_STATUE_MOVED = FLAG_AREA_TEMP_48
};

/* Scene flags, kept while the party stays in the scene. */
enum {
	/* The Empty Bottle has been filled at the Fountain of Healing this
	   visit. */
	FLAG_HERMES_WATER_TAKEN = FLAG_SCENE_TEMP_00,
	FLAG_LEADER_ON_FLOOR_SWITCH = FLAG_SCENE_TEMP_56
};

/* The chests: a Sleep Bomb, a Psy Crystal and a Nut. */
enum {
	FLAG_CHEST_SLEEP_BOMB = 0xf64,
	FLAG_CHEST_PSY_CRYSTAL = 0xf66,
	FLAG_CHEST_NUT = 0xf67
};

/* UB: the fountain rows include negative pixel offsets. Their range is -23..0,
   so multiplication is defined and fits the fixed-point coordinate. */
#if FIX_UNDEFINED_SHIFT
#define MERCURY_LIGHTHOUSE_OFFSET_FIXED(value) ((value) * toFIXED(1))
#else
#define MERCURY_LIGHTHOUSE_OFFSET_FIXED(value) toFIXED(value)
#endif

#include "shared/field_effect_create.c"
#include "shared/field_effect_spawn.c"
#include "shared/pushed_actor.c"

/* Talking to Sleet, the Mercury Djinni in the small room that the third
   waterfall from the west of the first room of six waterfalls leads to, starts
   a battle with it. */
static void scMercuryLighthouseStartDjinnBattle(void)
{
	fdRequestDjinnBattle(ACTOR_SLEET, DJINNI_SLEET);
}

#if MERCURY_LIGHTHOUSE_GATE_TERRACE_UNMASKED
/* The rows of MercuryLighthouseMiddle's gate room where the ground mask lets go
   of the leader walking on the ground layer. */
#define UNMASKED_FIRST_ROW 13
#define UNMASKED_LAST_ROW  18

/* Event: the ground mask stops following the leader while he crosses the rows
   from UNMASKED_FIRST_ROW to UNMASKED_LAST_ROW on MAPGRID_LAYER_GROUND. */
static void scMercuryLighthouseUpdateLeaderEffectTarget(void)
{
	ACTOR			  *leader = fdActorGet(ACTOR_PARTY_LEADER);
	GROUND_MASK_STATE *effectState = GroundMaskStateWork;

	if(leader->layer == 0 &&
		FIELD_METATILE(leader->z.fixed) >= UNMASKED_FIRST_ROW &&
		FIELD_METATILE(leader->z.fixed) <= UNMASKED_LAST_ROW)
		effectState->actor = NULL;
	else
		effectState->actor = leader;
}
#endif

#include "shared/mercury_lighthouse_sparks.c"
#include "shared/mercury_lighthouse_palette_cycle.c"

const SCENE_ENTRANCE *scGetEntrances(void)
{
	Sint16 scene = MainRoutineStateWork.next;

	if(scene == SCENE_MERCURY_LIGHTHOUSE_EXTERIOR) {
		return MercuryLighthouseExteriorEntrances;
	}
	if(scene == SCENE_MERCURY_LIGHTHOUSE_UPPER) {
		return MercuryLighthouseUpperEntrances;
	}
	if(scene == SCENE_MERCURY_LIGHTHOUSE_MIDDLE) {
		return MercuryLighthouseMiddleEntrances;
	}
	return MercuryLighthouseLowerEntrances;
}

const SCENE_EXIT_REGION *scGetExitRegions(void)
{
	return NULL;
}

/* The four scenes share one exit table. */
const Uint32 *scGetExits(void)
{
	return MercuryLighthouse_Exits;
}

/* Each scene's people, statues and pipes; the empty table is for no scene. */
const ACTOR_PLACE *scGetActorPlacements(void)
{
	Sint16 scene = MainRoutineStateWork.next;

	if(scene == SCENE_MERCURY_LIGHTHOUSE_LOWER) {
		return MercuryLighthouseLowerActorPlacements;
	}
	if(scene == SCENE_MERCURY_LIGHTHOUSE_MIDDLE) {
		return MercuryLighthouseMiddleActorPlacements;
	}
	if(scene == SCENE_MERCURY_LIGHTHOUSE_UPPER) {
		return MercuryLighthouseUpperActorPlacements;
	}
	if(scene == SCENE_MERCURY_LIGHTHOUSE_EXTERIOR) {
		return MercuryLighthouseExteriorActorPlacements;
	}
	return MercuryLighthouseNoActorPlacements;
}

/* Ripples spread round Isaac while he stands out on the open water of the great
   pool room, held up by one of his lights.

   The open water's metatiles are
   METATILE_BEHAVIOR_MERCURY_LIGHTHOUSE_POOL_WATER only once Ply has woken the
   goddess's pool below (FLAG_MERCURY_LIGHTHOUSE_HEART_HONORED), when `scInit`
   copies the pool's metatiles in. */
static void scMercuryLighthouseRippleWhileInPool(void)
{
	scMercuryLighthouseRippleAroundLeader();
}

/* Stepping onto a plain stepping square or the floor at the edge of the great
   pool room puts out the golden rings of light over Isaac's head, which swell
   and fade away.

   The squares and the floor's edge are
   METATILE_BEHAVIOR_MERCURY_LIGHTHOUSE_POOL_STEP. Two metatiles beside the
   Sleep Bomb chest, METATILE_BEHAVIOR_SLEEP_BOMB_LEDGE, do the same but are no
   place to hop from. */
static void scMercuryLighthouseLoseLightsOnLower(void)
{
	scMercuryLighthouseLoseLights();
}

/* In the great pool room, pushing against the water from a stepping square, the
   floor's edge or the open water, Isaac hops two metatiles the way he faces, to
   the next square or out onto the water.

   A hop onto open water spends one of the golden rings over his head, which
   flies ahead to hold him up where he lands; with none left he does not hop out
   onto it (shared/mercury_lighthouse_pillar.c). */
static void scMercuryLighthouseHopAcrossPoolOnLower(void)
{
	scMercuryLighthouseHopAcrossPool();
}

/* Stepping onto one of the great pool room's stepping squares marked with a
   cross puts golden rings of light over Isaac's head, one more every half
   second up to three. Each lets him hop once onto the open water.

   The cross-marked squares are METATILE_BEHAVIOR_MERCURY_LIGHTHOUSE_POOL_HEART
   once Ply has woken the goddess's pool below. */
static void scMercuryLighthouseChargeLightsOnLower(void)
{
	scMercuryLighthouseChargeLights();
}

/* Searching the lighthouse door before Mia has opened it, metatile (52, 36),
   shows "Strange forces are at work... It seems impossible to proceed."

   The door glows until then (scMercuryLighthouseCycleEntrancePalette). Once Mia
   opens it, the metatile becomes the door's exit,
   METATILE_BEHAVIOR_LIGHTHOUSE_DOOR. */
static void scMercuryLighthouseInspectSealedDoor(void)
{
	fdScriptStart();
	mnFieldMessageShow(MsgMercuryLighthouseStrangeForcesBlockWay, 1);
	fdScriptEnd();
}

/* Each scene's cues, the exterior's last. */
const CUE *scGetCues(void)
{
	Sint16 scene = MainRoutineStateWork.next;

	if(scene == SCENE_MERCURY_LIGHTHOUSE_LOWER) {
		return MercuryLighthouseLowerCues;
	}
	if(scene == SCENE_MERCURY_LIGHTHOUSE_MIDDLE) {
		return MercuryLighthouseMiddleCues;
	}
	if(scene == SCENE_MERCURY_LIGHTHOUSE_UPPER) {
		return MercuryLighthouseUpperCues;
	}
	return MercuryLighthouseExteriorCues;
}

#include "shared/mercury_lighthouse_waterfall.c"

/* Walking north into the third of the six waterfalls from the west, in the
   first room of waterfalls (metatile (16, 7)), Isaac vanishes in a trail of
   sparkles and comes out in Sleet's small room.

   Its metatile's behaviour is its exit's number,
   MERCURY_LIGHTHOUSE_UPPER_TO_MERCURY_LIGHTHOUSE_MIDDLE_NORTHWEST, past
   EXIT_METATILE_BEHAVIOR_OFFSET. */
static void scMercuryLighthouseWaterfallToSleetsRoom(void)
{
	scMercuryLighthouseWalkIntoWaterfall(
		FieldStateWork->stepCue - EXIT_METATILE_BEHAVIOR_OFFSET);
}

/* Walking north into the easternmost of the six waterfalls of the second room
   of waterfalls (metatile (28, 37)), Isaac vanishes in a trail of sparkles and
   comes out at the foot of the room of vines, through
   MERCURY_LIGHTHOUSE_UPPER_WATERFALLS_TO_GAP. */
static void scMercuryLighthouseWaterfallToGapRoom(void)
{
	scMercuryLighthouseWalkIntoWaterfall(
		FieldStateWork->stepCue - EXIT_METATILE_BEHAVIOR_OFFSET);
}

/* Walking north into the waterfall under the rainbow (metatile (49, 13)), Isaac
   vanishes in a trail of sparkles and comes out again on the path that led
   there, through MERCURY_LIGHTHOUSE_EXTERIOR_WATERFALL_TO_UPPER_PATH. */
static void scMercuryLighthouseWaterfallToUpperPath(void)
{
	scMercuryLighthouseWalkIntoWaterfall(
		FieldStateWork->stepCue - EXIT_METATILE_BEHAVIOR_OFFSET);
}

/* Once all three pipe sections of the pipe room are joined, the floor rumbles
   and shakes and the screen fades out. It shows the ground floor's room that
   the channels feed for two seconds
   (MERCURY_LIGHTHOUSE_LOWER_TO_MERCURY_LIGHTHOUSE_ENTRANCE_PIPE), with Isaac
   hidden, and then the party is back in the pipe room.

   From then on FLAG_MERCURY_LIGHTHOUSE_CHANNELS_OPEN chooses the running
   channels' entrances in mercury_lighthouse_entrance.c. */
static void scMercuryLighthouseExitWhenChannelsOpen(void)
{
	if(gmFlagGet(FLAG_PIPE_1_JOINED) != 0 &&
		gmFlagGet(FLAG_PIPE_2_JOINED) != 0 &&
		gmFlagGet(FLAG_PIPE_3_JOINED) != 0) {
		gmFlagSet(FLAG_MERCURY_LIGHTHOUSE_CHANNELS_OPEN);
		fdScriptWait(30);
		fdMapGridSetShake(1, 1, 1);
		sySndPlay(SE_RUMBLE);
		fdScriptWait(60);
		FieldStateWork->transition =
			SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
		fdScriptFinishScreenTransition();
		fdScriptWaitTransitionFrames();
		sySndPlay(SOUND_FADE_OUT_ACTION_SE);
		fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
		fdMapGridWaitForShakeToSettle();
		syImportScriptSetExitRequest(
			MERCURY_LIGHTHOUSE_LOWER_TO_MERCURY_LIGHTHOUSE_ENTRANCE_PIPE);
	} else {
		gmFlagClear(FLAG_MERCURY_LIGHTHOUSE_CHANNELS_OPEN);
	}
}

/* Where each channel pipe joins its channel: the first two along their rows,
   the third along its column. */
#define FIRST_PIPE_JOINED_ROW	 8
#define SECOND_PIPE_JOINED_ROW	 12
#define THIRD_PIPE_JOINED_COLUMN 40

/* Walking into one of the pipe room's three pipe sections, Isaac pushes it
   across its grated pool. Where it joins the pipes on either side, water bursts
   from the next pipe end in a spray of sparkles; pushed away again, it breaks
   the channel. With all three joined the water runs on below
   (scMercuryLighthouseExitWhenChannelsOpen).

   The metatiles copied redraw the water in the joined pipes. The second pipe's
   spray shows only once the first is joined, as the water reaches it through
   the first. */
static void scMercuryLighthousePushChannelPipes(void)
{
	PUSHED_ACTOR_PROBE probe;
	Sint32			   column;

	fdScriptStart();
	if(scPushedActorFindClearPos(&probe) != 0) {
		/* Each pipe controls one channel; restore the connected water tiles
		   too. */
		switch((Uint32)probe.actorSlot) {
		case ACTOR_FIRST_PIPE:
			if(FIELD_METATILE(probe.pos.z) == FIRST_PIPE_JOINED_ROW) {
				scPushedActorRoll(probe);
				fdScriptWait(20);
				fdMapGridCopyMetatilesTo(119, 9, 109, 11, 1, 1);
				scMercuryLighthouseSparkleBurst(
					toFIXED(726), 0, toFIXED(180), FACING_WEST);
				gmFlagSet(FLAG_PIPE_1_JOINED);
			} else {
				fdMapGridCopyMetatilesTo(117, 9, 104, 7, 1, 1);
				fdMapGridCopyMetatilesTo(119, 8, 109, 11, 1, 1);
				fdMapGridCopyMetatilesTo(118, 8, 104, 13, 1, 1);
				scPushedActorRoll(probe);
				gmFlagClear(FLAG_PIPE_1_JOINED);
			}
			break;
		case ACTOR_SECOND_PIPE:
			if(FIELD_METATILE(probe.pos.z) == SECOND_PIPE_JOINED_ROW) {
				scPushedActorRoll(probe);
				fdScriptWait(10);
				if(fdScriptGetFlag(FLAG_PIPE_1_JOINED)) {
					fdMapGridCopyMetatilesTo(118, 9, 104, 13, 1, 1);
					scMercuryLighthouseSparkleBurst(
						toFIXED(644), 0, toFIXED(210), FACING_SOUTH);
				}
				gmFlagSet(FLAG_PIPE_2_JOINED);
			} else {
				fdMapGridCopyMetatilesTo(119, 8, 109, 11, 1, 1);
				if(gmFlagGet(FLAG_PIPE_1_JOINED)) {
					fdMapGridCopyMetatilesTo(119, 9, 109, 11, 1, 1);
					fdMapGridCopyMetatilesTo(118, 8, 104, 13, 1, 1);
				}
				scPushedActorRoll(probe);
				fdScriptClearFlag(FLAG_PIPE_2_JOINED);
			}
			break;
		case ACTOR_THIRD_PIPE:
			column = probe.pos.x;
			if(FIELD_METATILE(column) == THIRD_PIPE_JOINED_COLUMN) {
				scPushedActorRoll(probe);
				fdScriptSetFlag(FLAG_PIPE_3_JOINED);
			} else {
				scPushedActorRoll(probe);
				fdScriptClearFlag(FLAG_PIPE_3_JOINED);
			}
			break;
		}
		scMercuryLighthouseExitWhenChannelsOpen();
	}
	fdScriptEnd();
}

/* Walking into one of the spouts of water that shoot from the pipe ends of the
   big pipe room while its spout pipe is joined, Isaac is carried back out to
   the spout's tip, standing, in a trail of sparkles: the spouts bar the way.
   The jet along the north wall of the room with the Nut chest does the same
   while it runs.

   Each spout's metatile behaviour picks the tip he is carried to. Every edition
   but the Japanese carries him a step further back from the jet in the Nut
   chest's room and pauses half a second (MERCURY_LIGHTHOUSE_CARRY_LEADER_BACK
   and NUT_ROOM_JET_CARRIES_FURTHER). */
static void scMercuryLighthousePushLeaderOutOfSpout(void)
{
	FIELD_STATE *state;

	state = FieldStateWork;
	fdScriptStart();
	sySetFrameEvent(scMercuryLighthouseSparkleAroundLeader, EVENT_PASS_FRAME);
	fdActorSetSpeed(ACTOR_PARTY_LEADER, 2.5);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
	fdActorGet(ACTOR_PARTY_LEADER)->steeringFlg &= ~ACTOR_FACES_MOTION;
	sySndPlay(SE_SPARKLING_PASSAGE);
	if(state->stepCue == METATILE_BEHAVIOR_EAST_SPOUT) {
		fdActorSetDestination(ACTOR_PARTY_LEADER, 232, 616);
	} else if(state->stepCue == METATILE_BEHAVIOR_SOUTH_SPOUT) {
		fdActorSetDestination(ACTOR_PARTY_LEADER, 360, 728);
	} else if(state->stepCue == METATILE_BEHAVIOR_WEST_SPOUT) {
		fdActorSetDestination(ACTOR_PARTY_LEADER, 248, 792);
	} else {
		MERCURY_LIGHTHOUSE_CARRY_LEADER_BACK(ACTOR_PARTY_LEADER, 696, 592);
#if NUT_ROOM_JET_CARRIES_FURTHER
		fdActorSetDestination(ACTOR_PARTY_LEADER, 696, 600);
		fdScriptWait(30);
#endif
	}
	fdActorWaitForMoveById(ACTOR_PARTY_LEADER);
	fdActorGet(ACTOR_PARTY_LEADER)->steeringFlg |= ACTOR_FACES_MOTION;
	syCloseEvent(scMercuryLighthouseSparkleAroundLeader);
	fdScriptEnd();
}

/* In the gate room, walking into the blue statue on the terrace pushes it a
   step while the sealed arch on the dais is shut, until it rests on the floor
   switch at the terrace's west end
   (scMercuryLighthouseOpenGateWithMovedStatue). */
static void scMercuryLighthousePushGateSwitchStatue(void)
{
	fdScriptStart();
	scPushedActorAdvancePair();
	fdScriptEnd();
	scMercuryLighthouseOpenGateWithMovedStatue();
}

/* Once the gate room's blue statue stands on the floor switch at the west end
   of the terrace (metatile (5, 13)), whether moved by Move or pushed, the
   switch sinks under it and the sealed arch at the top of the ladder on the
   dais opens for good.

   The statue is lowered 2 pixels onto the sunken switch, and the arch's
   metatile (9, 10) takes the open doorway's behaviour from (9, 5). */
static void scMercuryLighthouseOpenGateWithMovedStatue(void)
{
	Sint32 column;
	Sint32 row;

	column = fdActorGet(ACTOR_GATE_STATUE)->x.fixed / toFIXED(16);
	row = fdActorGet(ACTOR_GATE_STATUE)->z.fixed / toFIXED(16);
	fdScriptStart();
	if(column == 5 && row == 13) {
		fdActorGet(ACTOR_GATE_STATUE)->y.fixed -= toFIXED(2);
		fdActorGet(ACTOR_GATE_STATUE)->tgtY =
			fdActorGet(ACTOR_GATE_STATUE)->y.fixed;
		fdMapGridCopyMetatilesTo(5, 2, 5, 11, 1, 1);
		sySndPlay(SE_FLOOR_SWITCH);
		fdMapGridPlayMapEdit(MercuryLighthouse_GateMetatiles, 9, 7);
		fdMapGridCopyMetatileAttributes(9, 5, 1, 1, 9, 10);
		gmFlagSet(FLAG_MERCURY_LIGHTHOUSE_GATE_HELD_OPEN);
	}
	fdScriptEnd();
}

/* Standing on the gate room's floor switch, at the west end of the terrace
   (metatile (5, 13)), sinks it under Isaac and opens the sealed arch at the top
   of the ladder on the dais, for as long as he stays on it.

   The switch answers only once Isaac stands near its middle, the x and z range
   checked here. */
static void scMercuryLighthousePressGateFloorSwitch(void)
{
	Sint32 x;
	Sint32 z;

	if(fdScriptGetFlag(FLAG_LEADER_ON_FLOOR_SWITCH) == 0) {
		x = fdActorGet(ACTOR_PARTY_LEADER)->x.part[0].integer;
		z = fdActorGet(ACTOR_PARTY_LEADER)->z.part[0].integer;
		if(x >= 84 && x <= 91 && z > 211 && z <= 219) {
			fdScriptStart();
			gmFlagSet(FLAG_LEADER_ON_FLOOR_SWITCH);
			fdScriptWait(5);
			fdActorGet(ACTOR_PARTY_LEADER)->y.fixed -= toFIXED(2);
			fdActorGet(ACTOR_PARTY_LEADER)->tgtY =
				fdActorGet(ACTOR_PARTY_LEADER)->y.fixed;
			fdMapGridCopyMetatilesTo(5, 2, 5, 11, 1, 1);
			sySndPlay(SE_FLOOR_SWITCH);
			fdMapGridPlayMapEdit(MercuryLighthouse_GateMetatiles, 9, 7);
			fdScriptEnd();
		}
	}
}

/* Stepping off the gate room's floor switch lets it rise again, and the arch at
   the top of the ladder seals itself. */
static void scMercuryLighthouseReleaseGateFloorSwitch(void)
{
	if(fdScriptGetFlag(FLAG_LEADER_ON_FLOOR_SWITCH) != 0) {
		fdScriptStart();
		gmFlagClear(FLAG_LEADER_ON_FLOOR_SWITCH);
		fdActorGet(ACTOR_PARTY_LEADER)->y.fixed += toFIXED(2);
		fdActorGet(ACTOR_PARTY_LEADER)->tgtY =
			fdActorGet(ACTOR_PARTY_LEADER)->y.fixed;
		fdScriptWait(5);
		fdMapGridCopyMetatilesTo(7, 2, 5, 11, 1, 1);
		sySndPlay(SE_FLOOR_SWITCH);
		fdMapGridAnimateMetatiles(MercuryLighthouse_GateCloseMetatiles, 9, 7);
		fdScriptEnd();
	}
}

/* Draws the jet along the north wall of the room with the Nut chest a step at a
   time, the floor shaking at each step when delayFrames is not 0, and copies in
   the doorway that opens in the wall above it,
   MERCURY_LIGHTHOUSE_MIDDLE_TO_MERCURY_LIGHTHOUSE_UPPER_NORTH_NORTHEAST, with
   its metatiles. */
static void scMercuryLighthouseDrawNorthJetTiles(
	Sint32 delayFrames, Sint32 firstStep, Sint32 endStep)
{
	Uint32 step;
	Sint32 sourceColumn;

	if(delayFrames != 0) {
		sySndPlay(SE_GUSH);
	}
	for(step = firstStep; step < endStep; step++) {
		fdMapGridCopyMetatilesTo(
			45 - (step << 1), 32, 44 - (step << 1), 32, (step + 1), 6);
		fdMapGridCopyMetatilesTo(45 - step, 51, 45 - step, 32, 1, 6);
		sourceColumn = 109 - step;
		fdMapGridCopyMetatilesTo(sourceColumn, 32, 108 - step, 32, 1, 4);
		fdMapGridCopyMetatilesTo(sourceColumn, 51, sourceColumn, 32, 1, 4);
		if(delayFrames != 0) {
			fdMapGridSetShake(5, 5, 1);
			fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
			fdScriptWait(delayFrames);
		}
	}
	fdMapGridCopyMetatileAttributes(42, 52, 4, 5, 42, 33);
}

/* Takes the jet and its doorway away again a step at a time, the floor shaking,
   as the pipe is pushed back. */
static void scMercuryLighthouseRestoreNorthJetTiles(Sint32 delayFrames)
{
	Uint32 step;
	Sint32 destinationColumn;
	Sint32 sourceColumn;
	Sint32 column;

	sySndPlay(SE_GUSH);
	{
		/* The jet's strips are six metatiles tall. */
		Sint32 height = 6;

		step = 0;
		destinationColumn = 41;
		sourceColumn = 40;
		do {
			fdMapGridCopyMetatilesTo(
				sourceColumn, 32, destinationColumn, 32, 3 - step, height);
			{
				Sint32 width = 2;

				fdMapGridCopyMetatilesTo(39, 51, sourceColumn, 32, 1, height);
				column = step + 106;
				{
					Sint32 stripHeight = 4;

					fdMapGridCopyMetatileTilesIndicesRect(
						105, 51, column, 32, width, stripHeight);
					if(delayFrames != 0) {
						fdMapGridSetShake(5, 5, 1);
						fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
						fdScriptWait(delayFrames);
					}
					step++;
					destinationColumn += 2;
					sourceColumn += 2;
				}
			}
		} while(step <= 2);
	}
	sySndPlay(SE_STOP_ACTION_SE);
	fdMapGridCopyMetatileAttributes(106, 33, 4, 5, 42, 33);
	fdMapGridWaitForShakeToSettle();
}

/* Event: sparkles on the jet while it runs. */
static void scMercuryLighthouseSparkleOnNorthJet(void)
{
	scMercuryLighthouseSparkleAt(toFIXED(690), 0, toFIXED(584));
}

/* Starts the jet along the north wall of the room with the Nut chest, its
   leading edge throwing up spray and sparkles, and keeps it sparkling. */
static void scMercuryLighthouseStartNorthJet(Sint32 delayFrames)
{
	Uint32		   row;
	Sint32		   rowOffset;
	FIXED		   particleX;
	Uint32		   particleIndex;
	Sint32		   sparkleOffset;
	Sint32		   rowShift;
	FIXED		   velocityX;
	FIXED		   velocityZ;
	Uint32		   nextRow;
	EFFECT_OPTIONS options;

	fdMapGridCopyMetatilesTo(78, 59, 110, 36, 1, 1);
	fdMapGridCopyMetatilesTo(76, 59, 109, 36, 1, 1);
	options.pal = SPRITE_COLOR_WHITE;
	options.startScaleX = toFIXED(1) / 2;
	options.startScaleY = toFIXED(1) / 2;
	row = 0;
	/* Each row's edge sparkles and throws up spray as it opens. */
	do {
		rowOffset = row << 4;
		particleIndex = 0;
		rowShift = -(row * toFIXED(16));
		particleX = toFIXED(728) + rowShift;
		do {
			if((particleIndex & 1) != 0) {
				velocityX =
					syRandomBelow(8) * (toFIXED(1) / 5) - toFIXED(4) / 5;
				sparkleOffset = -particleIndex;
				velocityZ =
					syRandomBelow(8) * (toFIXED(1) / 5) - toFIXED(4) / 5;
				fdEffectSpawn(particleX, 0, toFIXED(584), velocityX, 0,
					velocityZ, EFFECT_USE_START_SCALE | EFFECT_USE_PALETTE,
					&options);
				fdScriptWait(1);
			} else {
				sparkleOffset = -particleIndex;
			}
			scMercuryLighthouseSparkleAt(
				MERCURY_LIGHTHOUSE_OFFSET_FIXED(sparkleOffset - rowOffset) +
					toFIXED(728),
				0, toFIXED(584));
			particleIndex++;
			particleX -= toFIXED(1);
		} while(particleIndex <= 7);
		fdMapGridCopyMetatilesTo(76, 59, 108 - row, 36, 2, 1);
		nextRow = row + 1;
		scMercuryLighthouseDrawNorthJetTiles(delayFrames, row, nextRow);
		row = nextRow;
	} while(nextRow <= 1);
	fdScriptWait(delayFrames);
	scMercuryLighthouseDrawNorthJetTiles(0, nextRow, nextRow + 1);
	sySndPlay(SE_MERCURY_LIGHTHOUSE_MECHANISM);
	sySetFrameEvent(scMercuryLighthouseSparkleOnNorthJet, EVENT_PASS_FRAME);
	fdMapGridWaitForShakeToSettle();
}

/* As the pipe moves away (the push's callback): spray drifts off the jet and
   its sparkles stop. */
static void scMercuryLighthouseStopNorthJet(void)
{
	Uint32		   row;
	Uint32		   particleIndex;
	FIXED		   offsetX;
	FIXED		   particleX;
	FIXED		   particleZ;
	EFFECT_OPTIONS options;

	fdMapGridCopyMetatilesTo(78, 58, 110, 36, 1, 1);
	options.pal = SPRITE_COLOR_BRG;
	options.startScaleX = toFIXED(1) / 2;
	options.startScaleY = toFIXED(1) / 2;
	for(row = 0; row <= 2; row++) {
		offsetX = -toFIXED(2);
		for(particleIndex = 1; particleIndex <= 7; particleIndex++) {
			if((particleIndex & 1) != 0) {
				particleZ = ((584 - syRandomBelow(5)) & 0xffff) << 16;
				particleX = (offsetX - row * toFIXED(8)) + toFIXED(728);
				fdEffectSpawn(particleX, 0, particleZ, -toFIXED(1) / 4, 0, 0,
					EFFECT_USE_START_SCALE | EFFECT_USE_PALETTE, &options);
				fdScriptWait(1);
			}
			offsetX -= toFIXED(2);
		}
		fdMapGridCopyMetatilesTo(111, 35, (109 - row), 36, 1, 1);
	}
	syCloseEvent(scMercuryLighthouseSparkleOnNorthJet);
}

/* The spout pipe joins the pipes above and below its pool at this column, and
   the spouts run; anywhere else they stop. */
#define SPOUT_PIPE_JOINED_COLUMN   11
/* The pipe in the room with the Nut chest joins the pipes either side of its
   pool at the first row, and the jet runs; at the second it stops. */
#define NORTH_JET_PIPE_RUNNING_ROW 40
#define NORTH_JET_PIPE_STOPPED_ROW 42

/* Walking into a pipe section, Isaac pushes it across its grated pool. In the
   big pipe room the spout pipe, joined to the pipes above and below it, sets
   water spouting from four pipe ends, one of them a tall spout in the room's
   west; in the room with the Nut chest the pipe joined across its pool sends a
   jet along the north wall and opens a doorway in the wall above it, the camera
   turning to show it the first time. Pushed away again, each stops its water.

   Three of the spouts bar the way (scMercuryLighthousePushLeaderOutOfSpout),
   and so does the jet. */
static void scMercuryLighthousePushSpoutAndNorthJetPipes(void)
{
	PUSHED_ACTOR_PROBE probe;
	Sint32			   pillarId;
	Sint32			   column;

	fdScriptStart();
	if(scPushedActorFindClearPos(&probe) != 0) {
		pillarId = probe.actorSlot;
		/* The spout pipe starts or stops the spouts; the other starts or stops
		   the jet. */
		if(pillarId == ACTOR_SPOUT_PIPE) {
			column = probe.pos.x;
			if(FIELD_METATILE(column) == SPOUT_PIPE_JOINED_COLUMN) {
				scPushedActorRoll(probe);
				fdScriptWait(30);
				sySndPlay(SE_MERCURY_LIGHTHOUSE_MECHANISM);
				scMercuryLighthouseStartSpoutsWithSparkles();
				fdMapGridCopyMetatilesTo(76, 60, 74, 38, 3, 1);
				fdMapGridCopyMetatilesTo(77, 60, 76, 38, 2, 1);
				fdMapGridCopyMetatilesTo(75, 58, 86, 41, 1, 3);
				fdMapGridCopyMetatilesTo(75, 59, 86, 43, 1, 2);
				fdMapGridCopyMetatilesTo(76, 59, 80, 49, 2, 1);
				fdMapGridCopyMetatilesTo(77, 59, 82, 49, 2, 1);
				gmFlagSet(FLAG_SPOUTS_RUNNING);
			} else {
				probe.func = scMercuryLighthouseStopSpoutsWithSparkles;
				fdMapGridCopyMetatilesTo(75, 57, 86, 41, 1, 1);
				fdMapGridCopyMetatilesTo(71, 59, 86, 42, 1, 1);
				fdMapGridCopyMetatilesTo(71, 59, 86, 43, 1, 1);
				fdMapGridCopyMetatilesTo(71, 59, 86, 44, 1, 1);
				fdMapGridCopyMetatilesTo(71, 59, 80, 49, 1, 1);
				fdMapGridCopyMetatilesTo(71, 59, 81, 49, 1, 1);
				fdMapGridCopyMetatilesTo(71, 59, 82, 49, 1, 1);
				fdMapGridCopyMetatilesTo(78, 58, 83, 49, 1, 1);
				scPushedActorRoll(probe);
				gmFlagClear(FLAG_SPOUTS_RUNNING);
			}
		} else if(pillarId == ACTOR_NORTH_JET_PIPE) {
			if(FIELD_METATILE(probe.pos.z) == NORTH_JET_PIPE_RUNNING_ROW) {
				scPushedActorRoll(probe);
				if(fdScriptGetFlag(FLAG_NORTH_JET_SHOWN) == 0) {
					fdCameraSetSpeed(1.5);
					fdCameraMoveTo(714, CAMERA_KEEP, 592, 1);
					fdCameraWaitForArrival();
					gmFlagSet(FLAG_NORTH_JET_SHOWN);
					scMercuryLighthouseStartNorthJet(5);
					fdScriptWait(50);
				} else {
					scMercuryLighthouseStartNorthJet(5);
				}
				gmFlagSet(FLAG_NORTH_JET_RUNNING);
			} else if(FIELD_METATILE(probe.pos.z) ==
				NORTH_JET_PIPE_STOPPED_ROW) {
				probe.func = scMercuryLighthouseStopNorthJet;
				scPushedActorRoll(probe);
				scMercuryLighthouseRestoreNorthJetTiles(5);
				gmFlagClear(FLAG_NORTH_JET_RUNNING);
			}
		}
	}
	fdScriptEnd();
}

/* In the upper gate room, walking into the blue statue that fell from the room
   of jets pushes it a step while the middle arch is shut, until it rests on the
   floor switch in the middle of the room
   (scMercuryLighthouseOpenUpperGateWithMovedStatue). */
static void scMercuryLighthousePushUpperGateSwitchStatue(void)
{
	fdScriptStart();
	scPushedActorAdvancePair();
	fdScriptEnd();
	scMercuryLighthouseOpenUpperGateWithMovedStatue();
}

/* Once the fallen blue statue stands on the floor switch in the middle of the
   upper gate room (metatile (10, 23)), whether moved by Move or pushed, the
   switch sinks under it and the sealed middle arch of the three in the north
   wall opens for good.

   The statue is lowered 2 pixels onto the sunken switch, and the arch's
   metatile (10, 19) takes the open doorway's behaviour from (10, 16). */
static void scMercuryLighthouseOpenUpperGateWithMovedStatue(void)
{
	Sint32 column = fdActorGet(ACTOR_FALLEN_STATUE)->x.fixed / toFIXED(16);
	Sint32 row = fdActorGet(ACTOR_FALLEN_STATUE)->z.fixed / toFIXED(16);

	fdScriptStart();
	if(column == 10 && row == 23) {
		fdActorGet(ACTOR_FALLEN_STATUE)->y.fixed -= toFIXED(2);
		fdActorGet(ACTOR_FALLEN_STATUE)->tgtY =
			fdActorGet(ACTOR_FALLEN_STATUE)->y.fixed;
		fdMapGridCopyMetatilesTo(6, 29, 10, 23, 1, 1);
		sySndPlay(SE_FLOOR_SWITCH);
		fdMapGridPlayMapEdit(MercuryLighthouse_UpperGateMetatiles, 10, 18);
		fdMapGridCopyMetatileAttributes(10, 16, 1, 1, column, 19);
		gmFlagSet(FLAG_MERCURY_LIGHTHOUSE_UPPER_GATE_HELD_OPEN);
	}
	fdScriptEnd();
}

/* Standing on the floor switch in the middle of the upper gate room, metatile
   (10, 23), sinks it under Isaac and opens the sealed middle arch of the three
   in the north wall, for as long as he stays on it. */
static void scMercuryLighthousePressUpperGateFloorSwitch(void)
{
	Sint32 x;
	Sint32 z;

	if(fdScriptGetFlag(FLAG_LEADER_ON_FLOOR_SWITCH) != 0)
		return;
	x = fdActorGet(ACTOR_PARTY_LEADER)->x.part[0].integer;
	z = fdActorGet(ACTOR_PARTY_LEADER)->z.part[0].integer;
	if(x < 164 || x > 171)
		return;
	if(z < 372)
		return;
	if(z >= 380)
		return;
	fdScriptStart();
	gmFlagSet(FLAG_LEADER_ON_FLOOR_SWITCH);
	fdScriptWait(5);
	fdActorGet(ACTOR_PARTY_LEADER)->y.fixed -= toFIXED(2);
	fdActorGet(ACTOR_PARTY_LEADER)->tgtY =
		fdActorGet(ACTOR_PARTY_LEADER)->y.fixed;
	fdMapGridCopyMetatilesTo(6, 29, 10, 23, 1, 1);
	sySndPlay(SE_FLOOR_SWITCH);
	fdMapGridPlayMapEdit(MercuryLighthouse_UpperGateMetatiles, 10, 18);
	fdScriptEnd();
}

/* Stepping off the upper gate room's floor switch lets it rise again, and the
   middle arch seals itself. */
static void scMercuryLighthouseReleaseUpperGateFloorSwitch(void)
{
	if(fdScriptGetFlag(FLAG_LEADER_ON_FLOOR_SWITCH) != 0) {
		fdScriptStart();
		gmFlagClear(FLAG_LEADER_ON_FLOOR_SWITCH);
		fdActorGet(ACTOR_PARTY_LEADER)->y.fixed += toFIXED(2);
		fdActorGet(ACTOR_PARTY_LEADER)->tgtY =
			fdActorGet(ACTOR_PARTY_LEADER)->y.fixed;
		fdScriptWait(5);
		fdMapGridCopyMetatilesTo(8, 29, 10, 23, 1, 1);
		sySndPlay(SE_FLOOR_SWITCH);
		fdMapGridAnimateMetatiles(
			MercuryLighthouse_UpperGateCloseMetatiles, 10, 18);
		fdScriptEnd();
	}
}

/* The three jets of the room of jets, named for the way each pushes the blue
   statue: JET_SOUTHWARD falls from the pipe end north of the statue's first
   place, JET_EASTWARD shoots along the row of the square hole from the pipe end
   at its west, and JET_WESTWARD shoots along the statue's first row from the
   pipe end at its east. */
enum { JET_SOUTHWARD = 0, JET_EASTWARD = 1, JET_WESTWARD = 2 };

/* Where the switch pipe stops along its pool, in metatiles: at the first column
   the water runs to JET_WESTWARD, at the second to JET_EASTWARD, at the third
   no jet runs, and anywhere else, as at the last, where it starts, the water
   falls as JET_SOUTHWARD. */
#define SWITCH_PIPE_WESTWARD_JET_COLUMN	 47
#define SWITCH_PIPE_EASTWARD_JET_COLUMN	 48
#define SWITCH_PIPE_NO_JET_COLUMN		 53
#define SWITCH_PIPE_SOUTHWARD_JET_COLUMN 54
/* The row of the pipe lying west to east once it has been pushed south into the
   pool (FLAG_WEST_EAST_PIPE_PUSHED). */
#define WEST_EAST_PIPE_PUSHED_ROW		 38

/* A random speed for the spray a jet throws up. */
static inline Sint32 scMercuryLighthouseRandomSpraySpeed(void)
{
	return syRandomBelow(8) * (toFIXED(1) / 5);
}

/* Starts one jet of the room of jets: the water reaches out from its pipe end a
   metatile at a time, throwing up spray. */
static void scMercuryLighthouseStartJet(Sint32 jet)
{
	EFFECT_OPTIONS options;
	Uint32		   particleIndex;
	Uint32		   row;

	sySndPlay(SE_MERCURY_LIGHTHOUSE_MECHANISM);
	if(jet == JET_SOUTHWARD) {
		fdMapGridCopyMetatilesTo(111, 57, 113, 42, 1, 1);
		fdMapGridCopyMetatilesTo(111, 59, 113, 43, 1, 1);
	} else if(jet == JET_EASTWARD) {
		fdMapGridCopyMetatilesTo(113, 58, 112, 46, jet, jet);
		fdMapGridCopyMetatilesTo(115, 58, 113, 46, jet, jet);
	} else {
		fdMapGridCopyMetatilesTo(115, 57, 116, 44, 1, 1);
		fdMapGridCopyMetatilesTo(113, 57, 115, 44, 1, 1);
	}
	options.pal = SPRITE_COLOR_WHITE;
	options.startScaleX = toFIXED(1) / 2;
	options.startScaleY = toFIXED(1) / 2;
	for(row = 0; row <= 1; row++) {
		for(particleIndex = 0; particleIndex <= 7; particleIndex++) {
			if(particleIndex & 1) {
				if(jet == JET_SOUTHWARD) {
					fdEffectSpawn(toFIXED(792), 0,
						toFIXED(704) + (row * 16 + particleIndex) * toFIXED(1),
						scMercuryLighthouseRandomSpraySpeed() - toFIXED(4) / 5,
						0,
						scMercuryLighthouseRandomSpraySpeed() - toFIXED(4) / 5,
						EFFECT_USE_START_SCALE | EFFECT_USE_PALETTE, &options);
				} else if(jet == JET_EASTWARD) {
					fdEffectSpawn(
						toFIXED(800) + (row * 16 + particleIndex) * toFIXED(1),
						0, toFIXED(746),
						scMercuryLighthouseRandomSpraySpeed() - toFIXED(4) / 5,
						0,
						scMercuryLighthouseRandomSpraySpeed() - toFIXED(4) / 5,
						EFFECT_USE_START_SCALE | EFFECT_USE_PALETTE, &options);
				} else {
					fdEffectSpawn(
						toFIXED(812) + (row * 16 + particleIndex) * -toFIXED(1),
						0, toFIXED(714),
						scMercuryLighthouseRandomSpraySpeed() - toFIXED(4) / 5,
						0,
						scMercuryLighthouseRandomSpraySpeed() - toFIXED(4) / 5,
						EFFECT_USE_START_SCALE | EFFECT_USE_PALETTE, &options);
				}
				fdScriptWait(1);
			}
		}
		if(jet == JET_SOUTHWARD) {
			fdMapGridCopyMetatilesTo(111, 58, 113, row + 43, 1, 1);
			fdMapGridCopyMetatilesTo(111, 59, 113, row + 44, 1, 1);
		} else if(jet == JET_EASTWARD) {
			fdMapGridCopyMetatilesTo(114, 58, row + 113, 46, jet, jet);
			fdMapGridCopyMetatilesTo(115, 58, row + 114, 46, jet, jet);
		} else {
			fdMapGridCopyMetatilesTo(114, 57, 115 - row, 44, 1, 1);
			fdMapGridCopyMetatilesTo(113, 57, 114 - row, 44, 1, 1);
		}
	}
}

/* Stops a jet: the water draws back into its pipe end a metatile at a time,
   spray drifting from it. The first metatile of all three jets is put back as
   it starts. */
static void scMercuryLighthouseStopJet(Sint32 jet)
{
	EFFECT_OPTIONS options;
	Uint32		   particleIndex, row;

	fdMapGridCopyMetatilesTo(112, 57, 113, 42, 1, 1);
	fdMapGridCopyMetatilesTo(117, 58, 112, 46, 1, 1);
	fdMapGridCopyMetatilesTo(117, 57, 116, 44, 1, 1);
	sySndPlay(SOUND_FADE_OUT_ACTION_SE);
	options.pal = SPRITE_COLOR_BRG;
	options.startScaleX = toFIXED(1) / 2;
	options.startScaleY = toFIXED(1) / 2;
	for(row = 0; row <= 2; row++) {
		for(particleIndex = 1; particleIndex <= 7; particleIndex++) {
			if((particleIndex & 1) != 0) {
				if(jet == JET_SOUTHWARD) {
					fdEffectSpawn(toFIXED(793 - syRandomBelow(5)), 0,
						(row * 4 + particleIndex) * toFIXED(2) + toFIXED(695),
						0, jet, toFIXED(1) / 4,
						EFFECT_USE_START_SCALE | EFFECT_USE_PALETTE, &options);
				} else if(jet == JET_EASTWARD) {
					fdEffectSpawn(
						(row * 4 + particleIndex) * toFIXED(2) + toFIXED(786),
						0, toFIXED(syRandomBelow(5)) + toFIXED(744),
						toFIXED(1) / 4, 0, 0,
						EFFECT_USE_START_SCALE | EFFECT_USE_PALETTE, &options);
				} else {
					fdEffectSpawn(toFIXED(824) - particleIndex * toFIXED(2) -
							row * toFIXED(8),
						0, toFIXED(syRandomBelow(5)) + toFIXED(712),
						toFIXED(1) / 4, 0, 0,
						EFFECT_USE_START_SCALE | EFFECT_USE_PALETTE, &options);
				}
				fdScriptWait(1);
			}
		}
		if(jet == JET_SOUTHWARD)
			fdMapGridCopyMetatilesTo(112, 58, 113, row + 43, 1, 1);
		else if(jet == JET_EASTWARD)
			fdMapGridCopyMetatilesTo(112, 58, row + 113, 46, jet, jet);
		else
			fdMapGridCopyMetatilesTo(112, 58, 115 - row, 44, 1, 1);
	}
}

/* The blue statue's first row, along which JET_WESTWARD pushes it under the
   pipe end of JET_SOUTHWARD. */
#define SLIDING_STATUE_UNDER_JET_ROW 44

/* The camera turns to the blue statue of the room of jets, and the jet just
   started pushes it a step if the statue stands where that jet hits it:
   JET_WESTWARD from its first place, then JET_SOUTHWARD from under the falling
   jet's pipe end, then JET_EASTWARD from the row of the hole, into the hole.
   Returns 1 when the slide is that last one.

   The flags read are the switch pipe's new place, set before this is called. */
static Bool scMercuryLighthouseStartStatueSlide(void)
{
	Sint32 metatileX;
	Sint32 metatileZ;

	metatileX = fdActorGet(ACTOR_SLIDING_STATUE)->x.fixed / toFIXED(16);
	metatileZ = fdActorGet(ACTOR_SLIDING_STATUE)->z.fixed / toFIXED(16);
	fdCameraSetSpeed(5);
	fdCameraMoveTo(816, CAMERA_KEEP, 712, 1);
	fdCameraWaitForArrival();
	/* A slide answers only the jet that hits the statue where it stands. */
	if(!gmFlagGet(FLAG_MERCURY_LIGHTHOUSE_STATUE_FELL)) {
		if(metatileX == 50 && gmFlagGet(FLAG_SWITCH_PIPE_STARTS_WESTWARD_JET)) {
			fdActorSetScript(
				ACTOR_SLIDING_STATUE, MercuryLighthouse_StatueSlideWest);
		} else if(metatileX == 49) {
			if(metatileZ == SLIDING_STATUE_UNDER_JET_ROW &&
				!gmFlagGet(FLAG_SWITCH_PIPE_STARTS_WESTWARD_JET) &&
				!gmFlagGet(FLAG_SWITCH_PIPE_STARTS_EASTWARD_JET) &&
				!gmFlagGet(FLAG_SWITCH_PIPE_STOPS_JETS)) {
				fdActorSetScript(
					ACTOR_SLIDING_STATUE, MercuryLighthouse_StatueSlideSouth);
			} else if(metatileZ == SLIDING_STATUE_UNDER_JET_ROW &&
				gmFlagGet(FLAG_SWITCH_PIPE_STARTS_WESTWARD_JET)) {
				fdActorSetScript(
					ACTOR_SLIDING_STATUE, MercuryLighthouse_StatueStaysPut);
			} else if(metatileZ == 46 &&
				gmFlagGet(FLAG_SWITCH_PIPE_STARTS_EASTWARD_JET)) {
				fdActorSetScript(ACTOR_SLIDING_STATUE,
					MercuryLighthouse_StatueSlideIntoHole);
				fdScriptWait(30);
				return TRUE;
			}
		}
	}
	fdScriptWait(30);
	return FALSE;
}

/* Sparkles rise from the sliding statue as it moves. */
static void scMercuryLighthouseSparkleAboveStatue(void)
{
	ACTOR *statue = fdActorGet(ACTOR_SLIDING_STATUE);
	FIXED  particleY = statue->y.fixed + toFIXED(syRandomBelow(4));
	FIXED  particleZ = statue->z.fixed;

	scMercuryLighthouseSparkleAt(statue->x.fixed, particleY, particleZ);
}

/* The slide scripts start the sparkles as the statue sets off. */
static Sint32 scMercuryLighthouseStartStatueSparkles(ACTOR *actor)
{
	sySetFrameEvent(scMercuryLighthouseSparkleAboveStatue, EVENT_PASS_FRAME);
	return 0;
}

/* The sparkles stop when the statue halts; slide scripts and code call it. An
   actor script passes its actor, which this one does not read. */
#if FIX_FUNCTION_SIGNATURES
static Sint32 scMercuryLighthouseStopStatueSparkles(ACTOR *actor)
{
#else
static Sint32 scMercuryLighthouseStopStatueSparkles(void)
{
#endif
	syCloseEvent(scMercuryLighthouseSparkleAboveStatue);
	return 0;
}

/* In the room of jets, walking into a pipe section, Isaac pushes it along its
   pool. The pipe lying north to south turns the water to another pipe end where
   it stops: the old jet stops, the new one starts, and the camera turns to the
   blue statue, which the new jet may push a step. When JET_EASTWARD slides the
   statue into the square hole, it falls through, and the view cuts to the gate
   room below as it lands (MERCURY_LIGHTHOUSE_UPPER_JETS_TO_GATE_STATUE_FALLS).
   The pipe lying west to east only moves. */
static void scMercuryLighthousePushJetRoomPipes(void)
{
	PUSHED_ACTOR_PROBE probe;
	Sint32			   destinationColumn;
	Sint32			   startColumn;

	fdScriptStart();
	if(scPushedActorFindClearPos(&probe) != 0) {
		switch(probe.actorSlot) {
		case ACTOR_WEST_EAST_PIPE:
			scPushedActorRoll(probe);
			if(FIELD_METATILE(probe.pos.z) == WEST_EAST_PIPE_PUSHED_ROW)
				gmFlagSet(FLAG_WEST_EAST_PIPE_PUSHED);
			else
				gmFlagClear(FLAG_WEST_EAST_PIPE_PUSHED);
			break;
		case ACTOR_JET_SWITCH_PIPE:
			startColumn =
				FIELD_METATILE(fdActorGet(ACTOR_JET_SWITCH_PIPE)->x.fixed);
			scPushedActorRoll(probe);
			destinationColumn = FIELD_METATILE(probe.pos.x);
			if(destinationColumn == SWITCH_PIPE_WESTWARD_JET_COLUMN) {
				gmFlagSet(FLAG_SWITCH_PIPE_STARTS_WESTWARD_JET);
				gmFlagClear(FLAG_SWITCH_PIPE_STARTS_EASTWARD_JET);
				gmFlagClear(FLAG_SWITCH_PIPE_STOPS_JETS);
				scMercuryLighthouseStartStatueSlide();
				if(startColumn == SWITCH_PIPE_SOUTHWARD_JET_COLUMN)
					scMercuryLighthouseStopJet(JET_SOUTHWARD);
				else if(startColumn == SWITCH_PIPE_EASTWARD_JET_COLUMN)
					scMercuryLighthouseStopJet(JET_EASTWARD);
				scMercuryLighthouseStartJet(JET_WESTWARD);
				fdScriptWait(60);
			} else if(destinationColumn == SWITCH_PIPE_EASTWARD_JET_COLUMN) {
				gmFlagSet(FLAG_SWITCH_PIPE_STARTS_EASTWARD_JET);
				gmFlagClear(FLAG_SWITCH_PIPE_STOPS_JETS);
				gmFlagClear(FLAG_SWITCH_PIPE_STARTS_WESTWARD_JET);
				if(scMercuryLighthouseStartStatueSlide() != 0) {
					FIXED x;

					scMercuryLighthouseStopJet(JET_WESTWARD);
					x = toFIXED(840);
#if FIX_FUNCTION_SIGNATURES
					scMercuryLighthouseStopStatueSparkles(NULL);
#else
					scMercuryLighthouseStopStatueSparkles();
#endif
					scMercuryLighthouseStartJet(JET_EASTWARD);
					fdActorWaitForScriptEndById(ACTOR_SLIDING_STATUE);
					scActorPrepareFX(x, 0, toFIXED(786), SPRITE_CRATE);
					scActorPrepareFX(x, 0, toFIXED(818), SPRITE_CRATE);
					fdActorSetDestination(ACTOR_SLIDING_STATUE, 840, 744);
					fdScriptWait(5);
					sySndPlay(SE_STONE_GRIND);
					fdActorWaitForMoveById(ACTOR_SLIDING_STATUE);
					fdScriptWait(40);
					gmFlagSet(FLAG_MERCURY_LIGHTHOUSE_STATUE_FELL);
					fdPaletteFadeApplySource(PALETTE_TINT_NONE, PALETTES_ALL);
					FieldStateWork->transition =
						SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
					fdScriptFinishScreenTransition();
					fdScriptWaitTransitionFrames();
					syImportScriptSetExitRequest(
						MERCURY_LIGHTHOUSE_UPPER_JETS_TO_GATE_STATUE_FALLS);
				} else {
					scMercuryLighthouseStopJet(JET_WESTWARD);
#if FIX_FUNCTION_SIGNATURES
					scMercuryLighthouseStopStatueSparkles(NULL);
#else
					scMercuryLighthouseStopStatueSparkles();
#endif
					scMercuryLighthouseStartJet(JET_EASTWARD);
					fdScriptWait(60);
				}
			} else if(destinationColumn == SWITCH_PIPE_NO_JET_COLUMN) {
				gmFlagSet(FLAG_SWITCH_PIPE_STOPS_JETS);
				gmFlagClear(FLAG_SWITCH_PIPE_STARTS_WESTWARD_JET);
				gmFlagClear(FLAG_SWITCH_PIPE_STARTS_EASTWARD_JET);
				scMercuryLighthouseStartStatueSlide();
				scMercuryLighthouseStopJet(JET_SOUTHWARD);
				fdScriptWait(60);
			} else {
				gmFlagClear(FLAG_SWITCH_PIPE_STARTS_WESTWARD_JET);
				gmFlagClear(FLAG_SWITCH_PIPE_STARTS_EASTWARD_JET);
				gmFlagClear(FLAG_SWITCH_PIPE_STOPS_JETS);
				scMercuryLighthouseStartStatueSlide();
				if(startColumn == SWITCH_PIPE_WESTWARD_JET_COLUMN)
					scMercuryLighthouseStopJet(JET_WESTWARD);
				else if(startColumn == SWITCH_PIPE_EASTWARD_JET_COLUMN)
					scMercuryLighthouseStopJet(JET_EASTWARD);
				scMercuryLighthouseStartJet(JET_SOUTHWARD);
				fdScriptWait(60);
			}
			break;
		}
	}
	fdScriptEnd();
}

/* Walking into one of the five stone statues along the north wall of the room
   of statues pushes it a step along the wall. The third from the west and the
   fifth, alone in its alcove to the east, stand in front of doorways to the
   room with the Nut chest, which open when they are pushed aside
   (scMercuryLighthouseRecordRowStatues). */
static void scMercuryLighthousePushRowStatue(void)
{
	fdScriptStart();
	scPushedActorAdvancePair();
	scMercuryLighthouseRecordRowStatues();
	fdScriptEnd();
}

/* Move on one of the stone statues along the north wall of the room of statues,
   or a push, moves it a step along the wall. Where each stands is kept until
   the party leaves the lighthouse, so the statues stay where they were pushed
   when Isaac comes back, and the fifth, pushed off its doorway, lets him jump
   the gap in front of its alcove. */
static void scMercuryLighthouseRecordRowStatues(void)
{
	Sint32 statueIndex;
	Sint32 positionFlag;
	Sint32 column;

	fdScriptStart();
	/* Four statues use paired flags for their two saved stopping columns. */
	for(statueIndex = 0, positionFlag = FLAG_FIRST_ROW_STATUE; statueIndex <= 3;
		statueIndex++, positionFlag += 2) {
		column = fdActorGet(statueIndex + ACTOR_FIRST_ROW_STATUE)->x.fixed /
			toFIXED(16);
		if(column == (statueIndex << 2) + 39) {
			gmFlagSet(positionFlag);
			gmFlagClear(positionFlag + 1);
		} else if(column == (statueIndex << 2) + 41) {
			gmFlagSet(positionFlag + 1);
			gmFlagClear(positionFlag);
		} else {
			gmFlagClear(positionFlag);
			gmFlagClear(positionFlag + 1);
		}
	}
	/* The last statue stands before the doorway to the room with the Nut chest.
	   Moved a column west or east it clears the doorway, and the metatile at
	   (58, 7) becomes a jump spot: pushing north from it, the leader jumps the
	   metatile between and walks up to the doorway. With the statue in place
	   the metatile is plain ground and the leader stays south of it. */
	column = fdActorGet(ACTOR_LAST_ROW_STATUE)->x.fixed / toFIXED(16);
	if(column == 57) {
		gmFlagSet(FLAG_LAST_ROW_STATUE_WEST);
		gmFlagClear(FLAG_LAST_ROW_STATUE_EAST);
		fdMapGridCopyMetatileAttributes(53, 10, 1, 1, 58, 7);
	} else if(column == 59) {
		gmFlagSet(FLAG_LAST_ROW_STATUE_EAST);
		gmFlagClear(FLAG_LAST_ROW_STATUE_WEST);
		fdMapGridCopyMetatileAttributes(53, 10, 1, 1, 58, 7);
	} else {
		gmFlagClear(FLAG_LAST_ROW_STATUE_WEST);
		gmFlagClear(FLAG_LAST_ROW_STATUE_EAST);
		fdMapGridCopyMetatileAttributes(53, 11, 1, 1, 58, 7);
	}
	fdScriptEnd();
}

/* Move on the stone statue standing in the doorway at the head of the room of
   vines: pushed one step west, to column 28, it stays there for good
   (FLAG_PASSAGE_STATUE_MOVED), and the doorway, which leads up to the paths
   outside, can be reached.

   The doorway's foot, metatile (29, 20), is a jump square on the strip of floor
   along the north wall, with a gap at (30, 20) east of it. Metatile (31, 20)
   takes the same behaviour, METATILE_BEHAVIOR_JUMP_FORWARD, so that Isaac,
   walking west along the strip from the room's east side, leaps the gap onto
   the doorway's foot. */
static void scMercuryLighthouseMoveGapStatueOffDoorway(void)
{
	fdScriptStart();
	if(fdActorGet(ACTOR_GAP_STATUE)->x.fixed / toFIXED(16) == 28) {
		gmFlagSet(FLAG_PASSAGE_STATUE_MOVED);
		fdMapGridCopyMetatileAttributes(29, 20, 1, 1, 31, 20);
	}
	fdScriptEnd();
}

/* Rotates the selected palette range once every eight frames. */
static inline void scMercuryLighthouseRotatePaletteRange(
	Uint16 *colors, Uint32 colorCount)
{
	Uint32	colorIndex;
	Uint16 *destination;
	Uint16 *source;
	Uint32	firstColor;

	if((FrameCount & 7) != 0) {
		return;
	}

	destination = colors;
	firstColor = *destination;
	colorIndex = 0;
	colors[colorCount - 1] = firstColor;

	source = colors + 1;
	while(colorIndex <= colorCount - 2) {
		*destination++ = *source++;
		colorIndex++;
	}
}

/* While the lighthouse door is sealed its light shimmers: background palette 2,
   colors 8 to 15, rotates every eight frames. */
static void scMercuryLighthouseCycleEntrancePalette(void)
{
	scMercuryLighthouseRotatePaletteRange((Uint16 *)&BG_PLTT_COLOR(2, 8), 8);
}

/* Talking to Mia, kneeling before the lighthouse door, or Mind Read on her, the
   first time: "Somebody...is here." She is sure it can only be Alex, the one
   other than her who can enter, and turns to the door to find the stone statue
   in her way: "But this statue is blocking my way..." */
static void scMercuryLighthouseMiaSensesSomeoneInside(void)
{
	fdScriptStart();
	fdActorSetSpeed(ACTOR_MIA, 0.8);
	fdActorSetSpeed(ACTOR_PARTY_LEADER, 0.8);
	fdScriptSetMessageId(MsgImilSomebodyHere);
	/* "Somebody...is here." */
	fdScriptShowMessageThenWait(ACTOR_MIA, 0, 20);
	fdActorWalkToAndWait(ACTOR_MIA, 840, 648);
	fdActorShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
	fdActorFaceDirection(ACTOR_MIA, FACING_WEST, 20);
	fdActorSetAnimation(ACTOR_MIA, ANIM_MIA_KNEEL);
	fdActorGet(ACTOR_MIA)->scaleX = -toFIXED(1);
	fdScriptWait(20);
	/* "Someone is here..." */
	fdScriptShowMessageThenWait(ACTOR_MIA, 0, 20);
	fdActorSetAnimation(ACTOR_MIA, ANIM_STAND);
	fdActorGet(ACTOR_MIA)->scaleX = toFIXED(1);
	fdScriptWait(20);
	fdActorFaceDirection(ACTOR_MIA, FACING_SOUTH, 20);
	/* "Can you hear it, too?" */
	fdScriptOpenMessage(ACTOR_MIA, 0);
	if(fdScriptAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
		fdScriptWait(20);
		fdActorSetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
		/* "Alex is the only one other than I who can enter Mercury..." */
		fdScriptShowMessageThenWait(ACTOR_MIA, 0, 20);
		FieldStateWork->msg += 1;
	} else {
		FieldStateWork->msg += 1;
		fdScriptWait(20);
		fdActorSetAnimationAndWait(ACTOR_MIA, ANIM_SHAKE_HEAD);
		/* "You can't hear it? Well, I can. Alex is the only one other..." */
		fdScriptShowMessageThenWait(ACTOR_MIA, 0, 20);
	}
	fdScriptWait(20);
	fdActorFaceDirection(ACTOR_MIA, FACING_NORTH, 20);
	fdCameraSetSpeed(0.8);
	fdCameraMoveTo(840, CAMERA_KEEP, 632, 1);
	fdActorWalkToAndWait(ACTOR_MIA, 840, 632);
	fdCameraWaitForArrival();
	fdScriptWait(20);
	fdActorRunRepeatedMotion(ACTOR_MIA, 2);
	fdScriptWait(10);
	fdActorSetAnimationAndWait(ACTOR_MIA, ANIM_SHAKE_HEAD);
	fdScriptWait(20);
	/* "But this statue is blocking my way..." */
	fdScriptShowMessageThenWait(ACTOR_MIA, 0, 20);
	gmFlagSet(FLAG_MIA_SENSED_SOMEONE_INSIDE);
	fdScriptEnd();
}

/* Talking to Mia again while the statue still blocks the door: she shakes her
   head. "This statue is blocking the entrance." */
static void scMercuryLighthouseMiaDiscussesBlockedEntrance(void)
{
	fdScriptStart();
	fdActorSetAnimationAndWait(ACTOR_MIA, ANIM_SHAKE_HEAD);
	fdScriptWait(20);
	fdScriptSetMessageId(MSGNO(MsgImilStatueBlocksEntrance));
	/* "This statue is blocking the entrance." */
	fdScriptShowMessageThenWait(ACTOR_MIA, 0, 20);
	fdScriptEnd();
}

/* The door's light brightens to white as it opens. */
static void scMercuryLighthouseFadePaletteToWhite(void)
{
	volatile Uint16 *palette;
	Uint32			 colorIndex;
	Uint32			 whiteColors;
	Sint32			 red;
	Sint32			 green;
	Sint32			 blue;

	do {
		palette = &BG_PLTT_COLOR(2, 8);
		whiteColors = 0;
		for(colorIndex = 0; colorIndex <= 7; colorIndex++) {
			red = RGB_RED(*palette);
			green = RGB_GREEN(*palette);
			blue = RGB_BLUE(*palette);
			if(red == RGB_LEVEL_MAX && green == RGB_LEVEL_MAX &&
				blue == RGB_LEVEL_MAX) {
				whiteColors++;
			} else {
				if(red < RGB_LEVEL_MAX)
					red++;
				if(green < RGB_LEVEL_MAX)
					green++;
				if(blue < RGB_LEVEL_MAX)
					blue++;
				*palette = RGB(red, green, blue);
			}
			palette++;
		}
		sySynch(2);
	} while(whiteColors <= 7);
}

/* Move on the stone statue in front of the lighthouse door while Mia waits
   there: once it has slid aside, to column 51, Mia turns and asks "Did you do
   that?" She saw the force that pushed it, Garet and Ivan tell her of Psynergy,
   and she opens the door with Ply in a flash of light, then jumps up and goes
   in alone.

   Garet and Ivan appear beside the leader for the talk and walk back into him
   at the end. */
static void scMercuryLighthouseMiaOpensEntranceWithPly(void)
{
	ACTOR *actor;
	Sint32 column;

	column = fdActorGet(ACTOR_DOOR_STATUE)->x.fixed / toFIXED(16);
	fdScriptStart();
	if(column == 51) {
		/* Mia turns: "Did you do that?" */
		fdActorSetSpeed(ACTOR_MIA, 0.8);
		fdScriptWait(20);
		fdActorRunRepeatedMotion(ACTOR_MIA, 2);
		fdScriptWait(20);
		fdActorFaceDirection(ACTOR_MIA, FACING_NORTH_NORTHEAST, 0);
		fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH_SOUTHWEST, 10);
		fdScriptSetMessageId(MsgMercuryDidThat);
		/* "Did you do that?" */
		fdScriptOpenMessage(ACTOR_MIA, 0);
		if(fdScriptAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
			FieldStateWork->msg += 1;
			fdScriptWait(20);
			fdActorSetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
			/* "I knew it... I saw a ghostly force pushing it!" */
			fdScriptShowMessageThenWait(ACTOR_MIA, 0, 20);
		} else {
			fdScriptWait(20);
			fdActorSetAnimationAndWait(ACTOR_MIA, ANIM_SHAKE_HEAD);
			/* "No, I saw it! I'm certain that flow of force came from you." */
			fdScriptShowMessageThenWait(ACTOR_MIA, 0, 20);
			FieldStateWork->msg += 1;
		}
		fdActorShowEmote(
			ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
		/* Garet and Ivan step out to explain Psynergy; Mia tells of Ply. */
		scSetActorSprPriority(fdActorGet(ACTOR_GARET), 1);
		scSetActorSprPriority(fdActorGet(ACTOR_IVAN), 1);
		fdActorSetSpeed(ACTOR_GARET, 0.8);
		fdActorSetSpeed(ACTOR_IVAN, 0.8);
		fdActorSetPos(ACTOR_GARET, 872, 600);
		fdActorSetPos(ACTOR_IVAN, 872, 600);
		fdActorWalkTo(ACTOR_IVAN, 888, 632);
		fdActorWalkToAndWait(ACTOR_GARET, 880, 616);
		fdActorFaceDirection(ACTOR_GARET, FACING_SOUTH_SOUTHWEST, 0);
		fdActorWaitForMoveById(ACTOR_IVAN);
		fdActorFaceDirection(ACTOR_IVAN, FACING_WEST, 0);
		fdActorRunRepeatedMotion(ACTOR_GARET, 1);
		fdScriptWait(20);
		/* "You... You could see our Psynergy?" */
		fdScriptShowMessageThenWait(ACTOR_GARET, 0, 20);
		fdActorShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
		/* "Psynergy... Is that what you call that power?" */
		fdScriptShowMessageThenWait(ACTOR_MIA, 0, 20);
		fdActorFaceDirection(ACTOR_GARET, FACING_SOUTH_SOUTHEAST, 0);
		fdActorFaceDirection(ACTOR_IVAN, FACING_NORTH_NORTHWEST, 20);
		fdActorSetAnimation(ACTOR_GARET, ANIM_NOD);
		fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
		fdScriptWait(30);
		fdActorFaceDirection(ACTOR_GARET, FACING_SOUTH_SOUTHWEST, 0);
		fdActorFaceDirection(ACTOR_IVAN, FACING_WEST, 20);
		/* "We had thought you were using Psynergy when you healed that
		   old..." */
		fdScriptShowMessageThenWait(ACTOR_IVAN, 0, 20);
		fdActorSetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
		fdScriptWait(20);
		/* "That's a special power, handed down to members of the Mercury
		   Clan..." */
		fdScriptShowMessageThenWait(ACTOR_MIA, 0, 20);
		fdActorWalkTo(ACTOR_MIA, 840, 656);
		fdScriptWait(5);
		fdActorFaceDirection(ACTOR_IVAN, FACING_SOUTH_SOUTHWEST, 0);
		fdScriptWait(10);
		fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
		fdActorWaitForMoveById(ACTOR_MIA);
		fdScriptWait(10);
		/* "It is called Ply, a power generated through meditation." */
		fdScriptShowMessageThenWait(ACTOR_MIA, 0, 20);
		fdActorFaceDirection(ACTOR_MIA, FACING_NORTH_NORTHEAST, 20);
		/* "It was once much stronger..." */
		fdScriptShowMessageThenWait(ACTOR_MIA, 0, 20);
		fdActorRunRepeatedMotion(ACTOR_GARET, 1);
		fdScriptWait(20);
		fdActorFaceDirection(ACTOR_GARET, FACING_NORTH, 20);
		/* "This Mercury Clan sounds a little like people of Vale, don't you
		   think?" */
		fdScriptOpenMessage(ACTOR_GARET, 0);
		if(fdScriptAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
			fdScriptWait(20);
			fdActorSetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
			/* "Sure... They're the sworn protectors of a special power,
			   just..." */
			fdScriptShowMessageThenWait(ACTOR_GARET, 0, 20);
			FieldStateWork->msg += 1;
		} else {
			FieldStateWork->msg += 1;
			fdScriptWait(20);
			fdActorSetAnimationAndWait(ACTOR_GARET, ANIM_SHAKE_HEAD);
			/* "Oh, come on! Both of them are protectors of special powers!
			   It's..." */
			fdScriptShowMessageThenWait(ACTOR_GARET, 0, 20);
		}
		fdActorSetAnimation(ACTOR_MIA, ANIM_MIA_KNEEL);
		fdScriptWait(30);
		fdActorRunRepeatedMotion(ACTOR_MIA, 1);
		fdScriptWait(20);
		/* "I have to go... I have to find whoever entered the lighthouse." */
		fdScriptShowMessageThenWait(ACTOR_MIA, 0, 20);
		fdActorWalkToAndWait(ACTOR_MIA, 840, 632);
		fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH_SOUTHWEST, 0);
		fdActorFaceDirection(ACTOR_GARET, FACING_SOUTH_SOUTHWEST, 0);
		fdActorFaceDirection(ACTOR_IVAN, FACING_WEST, 0);
		fdScriptWait(30);
		/* Mia casts Ply on the door: a flash, the light turns white and the
		   door opens. */
		scMercuryLighthouseMiaCastsPlyOnEntrance();
		fdScriptWait(50);
		sySndPlay(SE_PSYNERGY_SPARKLE);
		fdPaletteFadeApplySource(PALETTE_TINT_NONE, PALETTES_ALL);
		fdPaletteFadeApplyTarget(
			PALETTE_TINT_CUSTOM | RGB(31, 20, 31), PALETTES_ALL);
		fdPaletteFadeStartInterpolation(10);
		sySynch(1);
		sySndPlay(SE_SHIMMER);
		sySynch(40);
		fdPaletteFadeApplyTarget(PALETTE_TINT_NONE, PALETTES_ALL);
		fdPaletteFadeStartInterpolation(60);
		sySynch(60);
		sySndPlay(SE_MERCURY_LIGHTHOUSE_DOOR_OPEN);
		scMercuryLighthouseFadePaletteToWhite();
		fdMapGridCopyMetatilesTo(126, 35, 116, 35, 1, 2);
		fdMapGridPlayMapEdit(MercuryLighthouse_ColumnMetatiles, 116, 35);
		syCloseEvent(scMercuryLighthouseCycleEntrancePalette);
		fdScriptWait(20);
		/* She nods, jumps up and goes in. */
		fdActorSetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
		fdScriptWait(20);
		fdActorSetSpeed(ACTOR_MIA, 3);
		fdActorJump(ACTOR_MIA, 4, 0);
		fdActorWalkTo(ACTOR_MIA, 840, 600);
		fdActorWaitForMoveById(ACTOR_MIA);
		fdMapGridCopyMetatileAttributes(116, 36, 3, 4, 52, 36);
		fdActorPrepare(840, 56, 608, SPRITE_CRATE);
		fdActorSetSpeed(ACTOR_MIA, 0.8);
		fdActorWalkToAndWait(ACTOR_MIA, 840, 560);
		fdActorSetPos(ACTOR_MIA, 0, 0);
		fdScriptWait(20);
		fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
		fdActorFaceDirection(ACTOR_GARET, FACING_WEST, 0);
		fdActorFaceDirection(ACTOR_IVAN, FACING_NORTH_NORTHWEST, 0);
		fdScriptWait(10);
		fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
		fdActorSetAnimation(ACTOR_GARET, ANIM_NOD);
		fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
		fdScriptWait(20);
		/* Garet and Ivan rejoin the leader. */
		fdActorSetAnimation(ACTOR_GARET, ANIM_WALK);
		actor = fdActorGet(ACTOR_PARTY_LEADER);
		if(actor != NULL) {
			fdActorSetDestination(ACTOR_GARET, actor->x.part[0].integer,
				actor->z.part[0].integer);
		}
		fdActorSetAnimation(ACTOR_IVAN, ANIM_WALK);
		actor = fdActorGet(ACTOR_PARTY_LEADER);
		if(actor != NULL) {
			fdActorSetDestination(
				ACTOR_IVAN, actor->x.part[0].integer, actor->z.part[0].integer);
		}
		fdActorWaitForMoveById(ACTOR_GARET);
		fdActorSetPos(ACTOR_GARET, 0, 0);
		fdActorWaitForMoveById(ACTOR_IVAN);
		fdActorSetPos(ACTOR_IVAN, 0, 0);
		gmFlagSet(FLAG_MERCURY_LIGHTHOUSE_OPENED_BY_MIA);
	}
	fdScriptEnd();
}

/* The rainbow over the waterfall shimmers: background palette 6, colors 1 to 7,
   rotates every eight frames. */
static void scMercuryLighthouseCycleWaterfallPalette(void)
{
	scMercuryLighthouseRotatePaletteRange((Uint16 *)&BG_PLTT_COLOR(6, 1), 7);
}

/* The rainbow's colours fade to black, so it vanishes, as Ply takes effect. */
static void scMercuryLighthouseFadePaletteToBlack(void)
{
	/* Palette RAM is hardware, read through a volatile pointer. */
	volatile Uint16 *palette;
	Uint32			 colorIndex;
	Sint32			 blackColors;
	Sint32			 red;
	Sint32			 green;
	Sint32			 blue;

	do {
		palette = &BG_PLTT_COLOR(6, 1);
		blackColors = 0;
		for(colorIndex = 0; colorIndex <= 6; colorIndex++) {
			red = RGB_RED(*palette);
			green = RGB_GREEN(*palette);
			blue = RGB_BLUE(*palette);
			if(red > 0)
				red--;
			if(green > 0)
				green--;
			if(blue > 0)
				blue--;
			*palette = RGB(red, green, blue);
			if(*palette == 0)
				blackColors++;
			palette++;
		}
		sySynch(5);
	} while(blackColors != 7);
}

/* Raises one of the party up the reversed waterfall in a shower of sparks,
   faster and faster, for 96 frames. */
static void scMercuryLighthouseRaiseActorWithSparks(Sint32 actorId)
{
	EFFECT_OPTIONS options;
	ACTOR		  *actor;
	Uint32		   frame;
	FIXED		   particleX;
	FIXED		   particleY;
	FIXED		   riseSpeed;

	actor = fdActorGet(actorId);
	actor->motionFlg = 0;
	fdActorSetSprFlags(actor, 0);
	fdActorSetSprColById(actorId, SPRITE_COLOR_BLINK);
	sySndPlay(SE_RISE_WITH_SPARKS);
	options.pri = 1;
	options.pal = SPRITE_COLOR_BRG;
	options.type = SPRITE_GLINT;
	options.script = Glint_TwinkleScript;
	for(frame = 0; frame <= 47; frame++) {
		if(frame <= 31)
			scMercuryLighthouseSparkleNear(
				actor->x.fixed, actor->y.fixed, actor->z.fixed);
		if(frame & 1) {
			sySndPlay(SE_GLITTER);
			particleX =
				actor->x.fixed + toFIXED(syRandomBelow(24)) - toFIXED(12);
			particleY =
				actor->y.fixed + toFIXED(syRandomBelow(32)) - toFIXED(16);
			riseSpeed = ((syRandomBelow(4)) << 15) + toFIXED(1) / 2;
			fdEffectSpawn(particleX, particleY, actor->z.fixed, 0, riseSpeed, 0,
				EFFECT_USE_SCRIPT | EFFECT_USE_TYPE | EFFECT_USE_PRIORITY |
					EFFECT_USE_PALETTE,
				&options);
		}
		actor->y.fixed += frame * (toFIXED(1) / 10);
		actor->tgtY = actor->y.fixed;
		sySynch(2);
	}
}

/* The sparks of each of the two bursts over the waterfall, and the spray thrown
   up from each row of it as it parts. */
#define WATERFALL_BURST_SPARKS 16
#define WATERFALL_ROW_SPRAY	   32

/* Ply on the blue statue beside the waterfall under the rainbow: the rainbow
   fades, sparks gather over the water, the floor rumbles and the waterfall
   turns to stream upward. The first time Mia steps in and rises with it, and
   Isaac, Garet and Ivan follow her one by one; the screen turns white and the
   party is shown the lighthouse's beacon from outside before landing at the
   Lighthouse Aerie. Once Saturos is beaten, Isaac rises alone.

   The first time the exit is
   MERCURY_LIGHTHOUSE_EXTERIOR_TO_WORLD_MAP_LIGHTHOUSE_LIT, whose World Map view
   ends at the aerie; later it is
   MERCURY_LIGHTHOUSE_EXTERIOR_TO_MERCURY_LIGHTHOUSE_AERIE. */
static void scMercuryLighthousePlyOpensWaterfallPassage(void)
{
	Sint32		   rowOffset;
	Uint32		   wave;
	Uint32		   particleIndex;
	FIXED		   particleX;
	FIXED		   particleVelocityX;
	EFFECT_OPTIONS options;

	fdScriptStart();
	fdScriptWait(20);
	scMercuryLighthouseFadePaletteToBlack();
	syCloseEvent(scMercuryLighthouseCycleWaterfallPalette);
	fdMapGridCopyMetatilesTo(45, 77, 45, 73, 9, 4);
	fdScriptWait(30);
	options.pri = 1;
	options.pal = SPRITE_COLOR_BRG;
	options.type = SPRITE_GLINT;
	options.script = Glint_TwinkleScript;
	/* Two bursts spread sparks across the water before it begins to part. */
	for(wave = 0; wave < WATERFALL_BURST_SPARKS; wave++) {
		Uint32 particleX, z;

		if((1 & wave) != 0) {
			sySndPlay(SE_GLITTER);
		}
		particleX = toFIXED(syRandomBelow(48)) + toFIXED(768);
		z = toFIXED(syRandomBelow(56)) + toFIXED(136);
		fdEffectSpawn(particleX, 0, z, 0, 0, 0,
			EFFECT_USE_SCRIPT | EFFECT_USE_TYPE | EFFECT_USE_PRIORITY |
				EFFECT_USE_PALETTE | EFFECT_SCRIPT_MEDIUM,
			&options);
		fdScriptWait(2);
	}
	fdScriptWait(40);
	for(wave = 0; wave < WATERFALL_BURST_SPARKS; wave++) {
		Uint32 particleX, z;
		Sint32 particleSpeed;

		if((1 & wave) != 0) {
			sySndPlay(SE_GLITTER);
		}
		particleX = toFIXED(syRandomBelow(48)) + toFIXED(768);
		z = toFIXED(syRandomBelow(56)) + toFIXED(152);
		particleSpeed =
			-(syRandomBelow(10) * (toFIXED(1) / 5)) - toFIXED(1) / 5;
		fdEffectSpawn(particleX, 0, z, 0, 0, particleSpeed,
			EFFECT_USE_SCRIPT | EFFECT_USE_TYPE | EFFECT_USE_PRIORITY |
				EFFECT_USE_PALETTE | EFFECT_SCRIPT_MEDIUM,
			&options);
		fdScriptWait(2);
	}
	fdScriptWait(60);
	sySndPlay(SE_RUMBLE);
	fdMapGridSetShake(5, 5, 1);
	fdScriptWait(60);
	options.pal = SPRITE_COLOR_WHITE;
	options.startScaleX = toFIXED(7) / 10;
	options.startScaleY = toFIXED(7) / 10;
	options.tgtScaleX = toFIXED(6) / 5;
	options.tgtScaleY = toFIXED(6) / 5;
	for(wave = 0; wave <= 3; wave++) {
		FIXED particleSpeed;

		fdMapGridCopyMetatilesTo(59, (12 - wave), 48, (12 - wave), 3, 1);
		particleIndex = 0;
		rowOffset = (wave << 4);
		do {
			particleX = toFIXED(syRandomBelow(48)) + toFIXED(768);
			particleVelocityX = ((toFIXED(1) / 10) * (syRandomBelow(8))) -
				4 * (toFIXED(1) / 10);
			particleSpeed = (toFIXED(1) / 10) * (syRandomBelow(8));
			fdEffectSpawn(particleX, 0,
				toFIXED(-(particleIndex >> 1) - rowOffset) + toFIXED(192),
				particleVelocityX, 0, particleSpeed,
				EFFECT_USE_START_SCALE | EFFECT_SCALE_TO_TARGET |
					EFFECT_USE_PALETTE | EFFECT_SCRIPT_MEDIUM,
				&options);
			particleIndex++;
			fdScriptWait(2);
		} while(particleIndex < WATERFALL_ROW_SPRAY);
	}
	sySndPlay(SOUND_FADE_OUT_ACTION_SE);
	fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
	fdMapGridWaitForShakeToSettle();
	fdPlayQueuedMusic();
	fdScriptWait(30);
	/* After Saturos the leader goes alone; the first time Mia leads the
	   party. */
	if(gmFlagGet(FLAG_SATUROS_BEATEN_AT_MERCURY) != 0) {
		fdActorSetSpeed(ACTOR_PARTY_LEADER, 0.8);
		fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 824, 232);
		fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 792, 232);
		fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 792, 208);
		fdActorSetSpeed(ACTOR_PARTY_LEADER, 2);
		fdActorJump(ACTOR_PARTY_LEADER, 4, 0);
		fdActorWalkTo(ACTOR_PARTY_LEADER, 792, 200);
		fdScriptWait(10);
		fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_LEADER_BATHE);
		scMercuryLighthouseRaiseActorWithSparks(ACTOR_PARTY_LEADER);
		fdScriptWait(60);
		syCloseEvent(scMercuryLighthouseCycleWaterfallPalette);
		fdPaletteFadeApplySource(PALETTE_TINT_NONE, PALETTES_ALL);
		fdPaletteFadeApplyTarget(PALETTE_TINT_PALE, PALETTES_ALL);
		fdPaletteFadeStartInterpolation(120);
		fdScriptWait(120);
		fdPaletteFadeApplyTarget(RGB_WHITE, PALETTES_ALL);
		fdPaletteFadeStartInterpolation(60);
		fdScriptWait(60);
		syImportScriptSetExitRequest(
			MERCURY_LIGHTHOUSE_EXTERIOR_TO_MERCURY_LIGHTHOUSE_AERIE);
		fdScriptEnd();
	} else {
		fdActorSetSpeed(ACTOR_PARTY_LEADER, 0.8);
		fdActorSetSpeed(ACTOR_GARET, 0.8);
		fdActorSetSpeed(ACTOR_IVAN, 0.8);
		fdActorSetSpeed(ACTOR_MIA, 0.8);
		fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 824, 240);
		fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 20);
		fdActorSetPos(ACTOR_MIA, 824, 240);
		fdActorWalkToAndWait(ACTOR_MIA, 792, 232);
		fdActorFaceDirection(ACTOR_MIA, FACING_NORTH, 0);
		fdScriptWait(60);
		fdActorFaceDirection(ACTOR_MIA, FACING_SOUTHEAST, 20);
		fdActorSetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
		fdScriptWait(40);
		fdActorFaceDirection(ACTOR_MIA, FACING_NORTH, 20);
		fdActorWalkToAndWait(ACTOR_MIA, 792, 200);
		scMercuryLighthouseRaiseActorWithSparks(ACTOR_MIA);
		fdScriptWait(20);
		fdActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
		fdScriptWait(30);
		fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 792, 232);
		fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
		fdActorSetPos(ACTOR_GARET, 792, 232);
		fdActorSetPos(ACTOR_IVAN, 792, 232);
		fdActorWalkTo(ACTOR_GARET, 816, 224);
		fdActorWalkToAndWait(ACTOR_IVAN, 768, 224);
		fdActorWaitForMoveById(ACTOR_GARET);
		fdActorFaceDirection(ACTOR_GARET, FACING_NORTHWEST, 0);
		fdActorFaceDirection(ACTOR_IVAN, FACING_NORTHEAST, 20);
		fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
		fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
		fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_WINCE, 80);
		fdActorFaceDirection(ACTOR_GARET, FACING_SOUTHWEST, 0);
		fdActorFaceDirection(ACTOR_IVAN, FACING_SOUTHEAST, 20);
		fdActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 1);
		fdScriptWait(60);
		fdActorSetSpeed(ACTOR_PARTY_LEADER, 0.5);
		fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 792, 224);
		fdActorFaceDirection(ACTOR_GARET, FACING_WEST, 0);
		fdActorFaceDirection(ACTOR_IVAN, FACING_EAST, 0);
		fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 792, 208);
		fdActorFaceDirection(ACTOR_GARET, FACING_NORTHWEST, 0);
		fdActorFaceDirection(ACTOR_IVAN, FACING_NORTHEAST, 20);
		fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
		fdScriptWait(20);
		fdActorSetSpeed(ACTOR_PARTY_LEADER, 2);
		fdActorJump(ACTOR_PARTY_LEADER, 4, 0);
		fdActorWalkTo(ACTOR_PARTY_LEADER, 792, 200);
		fdScriptWait(10);
		fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_LEADER_BATHE);
		fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
		fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
		fdActorStartRepeatedMotion(ACTOR_GARET, 2);
		fdActorStartRepeatedMotion(ACTOR_IVAN, 2);
		scMercuryLighthouseRaiseActorWithSparks(ACTOR_PARTY_LEADER);
		fdScriptWait(60);
		fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
		fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_WINCE, 80);
		fdActorTurnToFaceEachOther(ACTOR_GARET, ACTOR_IVAN, 20);
		fdActorSetAnimation(ACTOR_GARET, ANIM_NOD);
		fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
		fdScriptWait(40);
		fdActorWalkToAndWait(ACTOR_GARET, 792, 216);
		fdActorFaceDirection(ACTOR_IVAN, FACING_NORTHEAST, 0);
		fdActorWalkToAndWait(ACTOR_GARET, 792, 200);
		fdScriptWait(30);
		scMercuryLighthouseRaiseActorWithSparks(ACTOR_GARET);
		fdActorWalkToAndWait(ACTOR_IVAN, 792, 216);
		fdActorWalkToAndWait(ACTOR_IVAN, 792, 200);
		fdScriptWait(30);
		scMercuryLighthouseRaiseActorWithSparks(ACTOR_IVAN);
		fdPaletteFadeApplySource(PALETTE_TINT_NONE, PALETTES_ALL);
		syCloseEvent(scMercuryLighthouseCycleWaterfallPalette);
		fdPaletteFadeApplyTarget(PALETTE_TINT_PALE, PALETTES_ALL);
		fdPaletteFadeStartInterpolation(120);
		fdScriptWait(120);
		fdPaletteFadeApplyTarget(RGB_WHITE, PALETTES_ALL);
		fdPaletteFadeStartInterpolation(60);
		fdScriptWait(60);
		fdScriptEnd();
		syImportScriptSetExitRequest(
			MERCURY_LIGHTHOUSE_EXTERIOR_TO_WORLD_MAP_LIGHTHOUSE_LIT);
	}
}

/* Searching the sign in front of the Fountain of Healing, at the top of the
   causeway (metatile (52, 50)), shows "Fountain of Healing" and "Water of
   Hermes:
   It brings rejuvenation to all that lives." */
static void scMercuryLighthouseReadHermesFountainSign(void)
{
	fdScriptStart();
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
	/* "Fountain of Healing" "Water of Hermes: It brings rejuvenation to all
	   that lives." */
	mnFieldMessageShow(MSGNO(MsgImilHermesHealingWaterFountain), 1);
	fdScriptEnd();
}

/* Searching the sealed arch of either gate room while it is shut, the one at
   the top of the ladder on the dais or the middle one of three: "The door is
   tightly locked." */
static void scMercuryLighthouseInspectLockedDoor(void)
{
	fdScriptStart();
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
	mnFieldMessageShow(MSGNO(MsgFieldDoorTightlyLocked), 1);
	fdScriptEnd();
}

/* Searching the Fountain of Healing at the top of the causeway, metatile (52,
   47), until its water has been taken this visit: "The fountain seems dry."
   before Saturos is beaten, "The fountain is flowing with water." after.

   With an Empty Bottle in the party it then asks for the field menu
   (FieldStateWork->fieldMenu), so the bottle can be used there. */
static void scMercuryLighthouseInspectHealingFountain(void)
{
	fdScriptStart();
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
	if(gmFlagGet(FLAG_SATUROS_BEATEN_AT_MERCURY) == 0)
		mnFieldMessageShow(MSGNO(MsgMercuryLighthouseFountainSeemsDry), 1);
	else
		mnFieldMessageShow(
			MSGNO(MsgMercuryLighthouseFountainFlowsWithWater), 1);
	if(gmPartyInventoryFindMember(ITEM_EMPTY_BOTTLE) != -1) {
		/* The field mode handles this menu request after the inspection
		   ends. */
		FieldStateWork->fieldMenu = 1;
	}
	fdScriptEnd();
}

/* Using the Empty Bottle at the Fountain of Healing once Saturos is beaten
   fills it: the bottle becomes Hermes' Water, once a visit. */
static void scMercuryLighthouseFillHermesWaterBottle(void)
{
	fdScriptStart();
	sySndPlay(MUS_TREASURE_FOUND);
	fdItemDispShowItem(
		ITEM_HERMES_WATER, ITEM_DISPLAY_SPARKLES | ITEM_DISPLAY_RAISED);
	scInventoryReplaceItem(ITEM_EMPTY_BOTTLE, ITEM_HERMES_WATER);
	mnMessageArgumentPush(
		gmPartyInventoryFindMember(ITEM_HERMES_WATER), MESSAGE_ARGUMENT_ACTOR);
	mnMessageArgumentPush(ITEM_HERMES_WATER, MESSAGE_ARGUMENT_ITEM);
	mnFieldMessageShow(MSGNO(MsgMercuryLighthouseIsaacGot), 1);
	gmFlagSet(FLAG_HERMES_WATER_TAKEN);
	fdScriptEnd();
}

/* Walking south into the hole behind the blue crystal of the crystal room
   (metatile (29, 37)), Isaac drops through the floor and lands in the goddess's
   pool on the ground floor
   (MERCURY_LIGHTHOUSE_LOWER_TO_MERCURY_LIGHTHOUSE_ENTRANCE_HOLE).

   In the gate room the same metatile, at the terrace's edge above another blue
   crystal (metatile (13, 19)), only jumps him down to the floor below. */
static void scMercuryLighthouseJumpDownPastCrystal(void)
{
	fdScriptStart();
	if(MainRoutineStateWork.next == SCENE_MERCURY_LIGHTHOUSE_LOWER) {
		fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 472, 600);
		fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 10);
		fdCameraMoveTo(464, CAMERA_KEEP, 656, 1);
		fdActorSetSprFlags(fdActorGet(ACTOR_PARTY_LEADER), 0);
		scActorPrepareFX(fdActorGet(ACTOR_PARTY_LEADER)->x.fixed, 0,
			toFIXED(702), SPRITE_CRATE);
		fdMapGridCopyMetatilesTo(92, 46, 92, 40, 3, 2);
		fdActorGet(ACTOR_PARTY_LEADER)->gravity = toFIXED(1) / 2;
		fdActorSetSprPriority(ACTOR_PARTY_LEADER, 2);
		syFieldActorJumpDownLedge(ACTOR_PARTY_LEADER, 6, -1);
		FieldStateWork->transition =
			SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_CIRCLE);
		fdScriptWait(60);
		syImportScriptSetExitRequest(
			MERCURY_LIGHTHOUSE_LOWER_TO_MERCURY_LIGHTHOUSE_ENTRANCE_HOLE);
	} else {
		syFieldActorJumpDownLedge(ACTOR_PARTY_LEADER, 6, -1);
	}
	fdScriptEnd();
}

#include "shared/mercury_lighthouse_arrive.c"

/* Starts the tall spout in the west of the big pipe room: the water rises from
   its pointed pipe end a metatile at a time, two metatiles, sparkles rising
   along its edge. */
static void scMercuryLighthouseStartSpoutsWithSparkles(void)
{
	EFFECT_OPTIONS options;
	Uint32		   particleIndex, row;

	fdMapGridCopyMetatilesTo(74, 58, 70, 34, 1, 1);
	options.pal = SPRITE_COLOR_WHITE;
	options.startScaleX = toFIXED(1) / 2;
	options.startScaleY = toFIXED(1) / 2;
	for(row = 0; row <= 1; row++) {
		for(particleIndex = 0; particleIndex <= 7; particleIndex++) {
			if((particleIndex & 1) != 0) {
				FIXED velocityX =
					(syRandomBelow(8)) * (toFIXED(1) / 5) - toFIXED(4) / 5;
				FIXED velocityZ =
					(syRandomBelow(8)) * (toFIXED(1) / 5) - toFIXED(4) / 5;

				fdEffectSpawn(toFIXED(105), 0,
					toFIXED(-particleIndex - (row << 4)) + toFIXED(544),
					velocityX, 0, velocityZ,
					EFFECT_USE_START_SCALE | EFFECT_USE_PALETTE, &options);
				fdScriptWait(1);
			}
		}
		fdMapGridCopyMetatilesTo(74, 59, 70, 34 - row, 1, 1);
		fdMapGridCopyMetatilesTo(74, 58, 70, 33 - row, 1, 1);
	}
}

/* Stops the spouts as the spout pipe moves away (the push's callback): the tall
   spout and the one shooting east draw back a metatile at a time, each in a
   scatter of sparkles. */
static void scMercuryLighthouseStopSpoutsWithSparkles(void)
{
	EFFECT_OPTIONS options;
	Uint32		   row;
	Uint32		   particleIndex;
	FIXED		   offsetZ;

	fdMapGridCopyMetatilesTo(76, 61, 74, 38, 1, 1);
	options.pal = SPRITE_COLOR_BRG;
	options.startScaleX = toFIXED(1) / 2;
	options.startScaleY = toFIXED(1) / 2;
	for(row = 0; row <= 2; row++) {
		offsetZ = -toFIXED(2);
		for(particleIndex = 1; particleIndex <= 7; particleIndex++) {
			if(particleIndex & 1) {
				if(particleIndex & 2) {
					fdEffectSpawn(toFIXED(105 - syRandomBelow(5)), 0,
						offsetZ - row * toFIXED(8) + toFIXED(558), 0, 0,
						-toFIXED(1) / 4,
						EFFECT_USE_START_SCALE | EFFECT_USE_PALETTE, &options);
				} else {
					fdEffectSpawn(
						(row * 4 + particleIndex) * toFIXED(2) + toFIXED(183),
						0, toFIXED(620 - syRandomBelow(5)), toFIXED(1) / 4, 0,
						0, EFFECT_USE_START_SCALE | EFFECT_USE_PALETTE,
						&options);
				}
				fdScriptWait(1);
			}
			offsetZ -= toFIXED(2);
		}
		fdMapGridCopyMetatilesTo(71, 59, 70, 34 - row, 1, 1);
		fdMapGridCopyMetatilesTo(71, 59, row + 75, 38, 1, 1);
	}
}

/* Event: a sparkle from the leader every fourth frame, as a spout carries him
   back. */
static void scMercuryLighthouseSparkleAroundLeader(void)
{
	EFFECT_OPTIONS options;
	ACTOR		  *leader = fdActorGet(ACTOR_PARTY_LEADER);
	Sint32		   framePhase = FrameCount & 3;

	if(framePhase == 0) {
		options.pal = SPRITE_COLOR_WHITE;
		if(((syRandomBelow(2)) & 1) == 0)
			options.pal = SPRITE_COLOR_BRG;
		options.startScaleX = toFIXED(7) / 10;
		options.startScaleY = toFIXED(7) / 10;
		{
			FIXED y = leader->y.fixed + toFIXED(syRandomBelow(4));
			FIXED velocityX =
				(syRandomBelow(8)) * (toFIXED(1) / 5) - toFIXED(4) / 5;
			FIXED velocityY =
				(syRandomBelow(8)) * (toFIXED(1) / 5) - toFIXED(4) / 5;

			fdEffectSpawn(leader->x.fixed, y, leader->z.fixed, velocityX,
				velocityY, framePhase,
				EFFECT_USE_START_SCALE | EFFECT_USE_PALETTE, &options);
		}
	}
}

/* A sparkle at a point every fourth frame, from an event. */
static void scMercuryLighthouseSparkleAt(Sint32 x, Sint32 y, Sint32 z)
{
	EFFECT_OPTIONS options;
	Sint32		   framePhase = FrameCount & 3;

	if(framePhase == 0) {
		options.pal = SPRITE_COLOR_WHITE;
		if(((syRandomBelow(2)) & 1) == 0)
			options.pal = SPRITE_COLOR_BRG;
		options.startScaleX = toFIXED(7) / 10;
		options.startScaleY = toFIXED(7) / 10;
		{
			FIXED velocityX =
				(syRandomBelow(8)) * (toFIXED(1) / 5) - toFIXED(4) / 5;
			FIXED velocityY =
				(syRandomBelow(8)) * (toFIXED(1) / 5) - toFIXED(4) / 5;

			fdEffectSpawn(x, y, z, velocityX, velocityY, framePhase,
				EFFECT_USE_START_SCALE | EFFECT_USE_PALETTE, &options);
		}
	}
}

/* One still sparkle near a point, up to 8 pixels either side and 4 north or
   south. */
static void scMercuryLighthouseSparkleNear(Sint32 x, Sint32 y, Sint32 z)
{
	EFFECT_OPTIONS options;
	FIXED		   particleX;
	FIXED		   particleZ;

	options.pal = SPRITE_COLOR_WHITE;
	options.pri = 1;
	options.startScaleX = toFIXED(7) / 10;
	options.startScaleY = toFIXED(7) / 10;
	particleX = x + toFIXED(syRandomBelow(16)) - toFIXED(8);
	particleZ = z + toFIXED(syRandomBelow(8)) - toFIXED(4);

	fdEffectSpawn(particleX, y, particleZ, 0, 0, 0,
		EFFECT_USE_START_SCALE | EFFECT_USE_PRIORITY | EFFECT_USE_PALETTE,
		&options);
}

/* The sparkles of a burst, one every four frames. */
#define BURST_SPARKLES 6

/* BURST_SPARKLES sparkles, spraying the way angle points and slowing one by
   one, as the water runs through a joined pipe. */
static void scMercuryLighthouseSparkleBurst(
	Sint32 x, Sint32 y, Sint32 z, Sint32 angle)
{
	EFFECT_OPTIONS options;
	Sint32		   velocity[3];
	Uint32		   particleIndex;

	sySndPlay(SE_BURST);
	for(particleIndex = 0; particleIndex < BURST_SPARKLES * 2;
		particleIndex++) {
		if((particleIndex & 1) != 0) {
			options.pal = SPRITE_COLOR_WHITE;
			if((particleIndex & 2) != 0)
				options.pal = SPRITE_COLOR_BRG;
			options.startScaleX = toFIXED(3) / 5;
			options.startScaleY = toFIXED(3) / 5;
			SET_VECTOR(velocity, 0, 0, 0);
			syVectorAddPolarOffset(
				(BURST_SPARKLES - (particleIndex >> 1)) * (toFIXED(1) / 10),
				angle, velocity);
			{
				FIXED particleX = x + toFIXED(6 - syRandomBelow(6));
				FIXED particleZ = z + toFIXED(6 - syRandomBelow(6));

				fdEffectSpawn(particleX, y, particleZ, velocity[0], velocity[1],
					velocity[2], EFFECT_USE_START_SCALE | EFFECT_USE_PALETTE,
					&options);
			}
		}
		sySynch(2);
	}
}

/* Restores each lighthouse scene's puzzle state and plays its entrance's
   script. */
Sint32 scInit(void)
{
	Uint32 statueIndex;
	Sint32 positionFlag;
	FIXED  statueX;
	Sint32 keepPartyPosition;
	Sint32 alpha;

	gmFlagSet(FLAG_IN_MERCURY_LIGHTHOUSE);
	MainRoutineStateWork.retreatEntrance =
		MERCURY_LIGHTHOUSE_EXTERIOR_FROM_WORLD_MAP;
	MainRoutineStateWork.retreatScene = SCENE_MERCURY_LIGHTHOUSE_EXTERIOR;
	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
	IO_WRITE16(BLDCNT_TGT2_ALL | BLDCNT_EFFECT_BLEND, &REG_BLDCNT);
	alpha = BLEND_ALPHA(16, 16);
	REG_BLDALPHA = alpha;
	scMercuryLighthouseRestoreLights(
		METATILE_BEHAVIOR_MERCURY_LIGHTHOUSE_POOL_WATER,
		(MERCURY_LIGHTHOUSE_LIGHTS *)SceneLocalState);
	fdEffectStartGroundMask(0);
	if(gmFlagGet(FLAG_MERCURY_LIGHTHOUSE_HEART_HONORED))
		sySetFrameEvent(scMercuryLighthouseCyclePalette, EVENT_PASS_FRAME);
	else
		scMercuryLighthouseClearPalette();
	if(MainRoutineStateWork.next == SCENE_MERCURY_LIGHTHOUSE_LOWER) {
		/* The great pool room, the pipe room and Mia's ledge. */
		switch(MainRoutineStateWork.entrance) {
		case MERCURY_LIGHTHOUSE_LOWER_FROM_MERCURY_LIGHTHOUSE_ENTRANCE_WEST:
		case MERCURY_LIGHTHOUSE_LOWER_FROM_MERCURY_LIGHTHOUSE_ENTRANCE_WEST_NORTHWEST:
		case MERCURY_LIGHTHOUSE_LOWER_POOL_FROM_STATUE:
			if(gmFlagGet(FLAG_MERCURY_LIGHTHOUSE_HEART_HONORED))
				fdMapGridCopyMetatileAttributes(66, 5, 27, 23, 2, 5);
			break;
		case MERCURY_LIGHTHOUSE_LOWER_WATERFALLS_FROM_STATUE:
		case MERCURY_LIGHTHOUSE_LOWER_FROM_MERCURY_LIGHTHOUSE_MIDDLE_SOUTH:
			fdQueueMusic(SE_AMBIENT_WATER);
			break;
		case MERCURY_LIGHTHOUSE_LOWER_FROM_MERCURY_LIGHTHOUSE_ENTRANCE_PIPE:
			scSetActorSprPriority(fdActorGet(ACTOR_PARTY_LEADER), 1);
			/* Fall through. */
		case MERCURY_LIGHTHOUSE_LOWER_FROM_MERCURY_LIGHTHOUSE_ENTRANCE_EAST_NORTHEAST:
			if(gmFlagGet(FLAG_MERCURY_LIGHTHOUSE_CHANNELS_OPEN)) {
				sySynch(1);
				fdActorSetPos(ACTOR_FIRST_PIPE, 672, 128);
				fdActorSetPos(ACTOR_SECOND_PIPE, 704, 192);
				fdActorSetPos(ACTOR_THIRD_PIPE, 648, 240);
			} else if(!gmFlagGet(FLAG_KEEP_PARTY_POSITION)) {
				gmFlagClear(FLAG_PIPE_1_JOINED);
				gmFlagClear(FLAG_PIPE_2_JOINED);
				gmFlagClear(FLAG_PIPE_3_JOINED);
				gmFlagClear(FLAG_UNUSED_PIPE_4_JOINED);
			}
			scSetActorSprPriority(fdActorGet(ACTOR_FIRST_PIPE), 1);
			scSetActorSprPriority(fdActorGet(ACTOR_SECOND_PIPE), 1);
			scSetActorSprPriority(fdActorGet(ACTOR_THIRD_PIPE), 1);
			scPushedActorBlockFootprint(ACTOR_FIRST_PIPE);
			scPushedActorBlockFootprint(ACTOR_SECOND_PIPE);
			scPushedActorBlockFootprint(ACTOR_THIRD_PIPE);
			scSetActorSprPriority(fdActorGet(ACTOR_PIPE_END), 1);
			/* The metatiles scMercuryLighthousePushChannelPipes copies as it
			   sets the flag, redrawn on arrival. */
			if(gmFlagGet(FLAG_PIPE_1_JOINED)) {
				fdMapGridCopyMetatilesTo(119, 9, 109, 11, 1, 1);
				/* The metatiles scMercuryLighthousePushChannelPipes copies as
				   it sets the flag, redrawn on arrival. */
				if(gmFlagGet(FLAG_PIPE_2_JOINED))
					fdMapGridCopyMetatilesTo(118, 9, 104, 13, 1, 1);
				fdMapGridCenterOnFollowedPoint();
				sySynch(1);
			}
			break;
		case MERCURY_LIGHTHOUSE_LOWER_STATUE_FROM_POOL:
		case MERCURY_LIGHTHOUSE_LOWER_STATUE_FROM_WATERFALLS:
			if(!gmFlagGet(FLAG_MIA_JOINED_PARTY)) {
				fdActorSetPos(ACTOR_MIA, 696, 632);
				fdActorFaceDirection(ACTOR_MIA, FACING_EAST, 0);
			} else {
				fdActorSetPos(ACTOR_STATUE_BLOCKING_MIA, 776, 632);
				fdMapGridCopyMetatileAttributes(110, 39, 5, 1, 46, 39);
			}
			break;
		}
	}
	if(MainRoutineStateWork.next == SCENE_MERCURY_LIGHTHOUSE_MIDDLE) {
		switch(MainRoutineStateWork.entrance) {
		case MERCURY_LIGHTHOUSE_MIDDLE_FROM_MERCURY_LIGHTHOUSE_LOWER_SOUTH:
		case MERCURY_LIGHTHOUSE_MIDDLE_PIPES_FROM_GATE:
			scSetActorSprPriority(fdActorGet(ACTOR_SPOUT_PIPE), 1);
			scSetActorSprPriority(fdActorGet(ACTOR_SPOUT_PIPE_END), 1);
			/* With the spout pipe pushed into its pool, joining the pipe above
			   to the one below, water runs on through the room's pipes and
			   spouts from their open ends. The pipe is put back in the pool,
			   and the metatiles copied here are that state's. */
			if(gmFlagGet(FLAG_SPOUTS_RUNNING)) {
				sySynch(1);
				sySndPlay(SE_MERCURY_LIGHTHOUSE_MECHANISM);
				fdActorSetPos(ACTOR_SPOUT_PIPE, 184, 528);
				fdMapGridCopyMetatileAttributes(11, 31, 1, 4, 9, 31);
				fdMapGridCopyMetatileAttributes(7, 30, 1, 4, 11, 31);
				fdMapGridCopyMetatilesTo(74, 58, 70, 32, 1, 2);
				fdMapGridCopyMetatilesTo(74, 59, 70, 34, 1, 1);
				fdMapGridCopyMetatilesTo(76, 60, 74, 38, 3, 1);
				fdMapGridCopyMetatilesTo(77, 60, 76, 38, 2, 1);
				fdMapGridCopyMetatilesTo(75, 58, 86, 41, 1, 3);
				fdMapGridCopyMetatilesTo(75, 59, 86, 43, 1, 2);
				fdMapGridCopyMetatilesTo(76, 59, 80, 49, 2, 1);
				fdMapGridCopyMetatilesTo(77, 59, 82, 49, 2, 1);
			}
			break;
		case MERCURY_LIGHTHOUSE_MIDDLE_FROM_MERCURY_LIGHTHOUSE_LOWER_NORTH:
		case MERCURY_LIGHTHOUSE_MIDDLE_GATE_FROM_PIPES:
			fdMapGridCenterOnFollowedPoint();
			sySynch(1);
			if(gmFlagGet(FLAG_KEEP_PARTY_POSITION) &&
				gmFlagGet(FLAG_LEADER_ON_FLOOR_SWITCH)) {
				fdMapGridCopyMetatilesTo(5, 2, 5, 11, 1, 1);
				fdMapGridCopyMetatilesTo(9, 1, 9, 7, 1, 2);
			}
			/* The statue stands on the floor switch west of the dais, which
			   holds the gate open: the sealed arch north of the dais is drawn
			   as an open doorway. */
			if(gmFlagGet(FLAG_MERCURY_LIGHTHOUSE_GATE_HELD_OPEN)) {
				fdActorSetPos(ACTOR_GATE_STATUE, 88, 216);
				fdActorGet(ACTOR_GATE_STATUE)->y.fixed -= toFIXED(2);
				fdActorGet(ACTOR_GATE_STATUE)->tgtY =
					fdActorGet(ACTOR_GATE_STATUE)->y.fixed;
				fdMapGridCopyMetatilesTo(9, 1, 9, 7, 1, 2);
				fdMapGridCopyMetatilesTo(5, 2, 5, 11, 1, 1);
				fdMapGridCopyMetatileAttributes(9, 5, 1, 1, 9, 10);
			}
#if MERCURY_LIGHTHOUSE_GATE_TERRACE_UNMASKED
			sySetFrameEvent(
				scMercuryLighthouseUpdateLeaderEffectTarget, EVENT_PASS_FRAME);
#endif
			break;
		case MERCURY_LIGHTHOUSE_MIDDLE_FROM_MERCURY_LIGHTHOUSE_UPPER_NORTHEAST:
		case MERCURY_LIGHTHOUSE_MIDDLE_FROM_MERCURY_LIGHTHOUSE_UPPER_EAST_NORTHEAST:
		case MERCURY_LIGHTHOUSE_MIDDLE_FROM_MERCURY_LIGHTHOUSE_UPPER_NORTH_NORTHEAST:
			scSetActorSprPriority(fdActorGet(ACTOR_NORTH_JET_PIPE), 1);
			/* The north jet's pipe joined across the head of its pool, with a
			   jet of water crossing it. */
			if(gmFlagGet(FLAG_NORTH_JET_RUNNING)) {
				scMercuryLighthouseStartNorthJet(0);
				fdMapGridCopyMetatileAttributes(42, 41, 4, 1, 42, 39);
				fdMapGridCopyMetatileAttributes(42, 40, 4, 1, 42, 41);
				fdActorSetPos(ACTOR_NORTH_JET_PIPE, 704, 640);
			}
			break;
		}
	}
	if(MainRoutineStateWork.next == SCENE_MERCURY_LIGHTHOUSE_UPPER) {
		switch(MainRoutineStateWork.entrance) {
		case MERCURY_LIGHTHOUSE_UPPER_ROW_STATUES_FROM_GATE:
		case MERCURY_LIGHTHOUSE_UPPER_FROM_MERCURY_LIGHTHOUSE_MIDDLE_NORTHEAST:
		case MERCURY_LIGHTHOUSE_UPPER_FROM_MERCURY_LIGHTHOUSE_MIDDLE_EAST_NORTHEAST:
			scSetActorSprPriority(fdActorGet(ACTOR_FIRST_ROW_STATUE), 1);
			scSetActorSprPriority(fdActorGet(ACTOR_FIRST_ROW_STATUE + 1), 1);
			scSetActorSprPriority(fdActorGet(ACTOR_FIRST_ROW_STATUE + 2), 1);
			scSetActorSprPriority(fdActorGet(ACTOR_FIRST_ROW_STATUE + 3), 1);
			scSetActorSprPriority(fdActorGet(ACTOR_LAST_ROW_STATUE), 1);
			scSetActorSprPriority(fdActorGet(ACTOR_PARTY_LEADER), 1);
			statueIndex = 0;
			statueX = toFIXED(632);
			positionFlag = FLAG_FIRST_ROW_STATUE;
			for(; statueIndex <= 3; statueIndex++) {
				if(gmFlagGet(positionFlag))
					fdActorSetPosFX(statueIndex + ACTOR_FIRST_ROW_STATUE,
						statueX, toFIXED(88));
				else if(gmFlagGet(positionFlag + 1))
					fdActorSetPosFX(statueIndex + ACTOR_FIRST_ROW_STATUE,
						statueX + toFIXED(32), toFIXED(88));
				statueX += toFIXED(64);
				positionFlag += 2;
			}
			/* The metatiles scMercuryLighthouseRecordRowStatues copies as it
			   sets the flag, redrawn on arrival. */
			if(gmFlagGet(FLAG_LAST_ROW_STATUE_WEST)) {
				fdActorSetPos(ACTOR_LAST_ROW_STATUE, 920, 88);
				fdMapGridCopyMetatileAttributes(53, 10, 1, 1, 58, 7);
			} else if(gmFlagGet(FLAG_LAST_ROW_STATUE_EAST)) {
				fdActorSetPos(ACTOR_LAST_ROW_STATUE, 952, 88);
				fdMapGridCopyMetatileAttributes(53, 10, 1, 1, 58, 7);
			}
			break;
		case MERCURY_LIGHTHOUSE_UPPER_FROM_MERCURY_LIGHTHOUSE_EXTERIOR_EAST:
		case MERCURY_LIGHTHOUSE_UPPER_GAP_FROM_WATERFALLS:
			/* The gap statue one column west of the door it hid, and the jump
			   spot that reaches the door
			   (scMercuryLighthouseMoveGapStatueOffDoorway). */
			if(gmFlagGet(FLAG_PASSAGE_STATUE_MOVED)) {
				fdActorSetPos(ACTOR_GAP_STATUE, 456, 328);
				fdMapGridCopyMetatileAttributes(29, 20, 1, 1, 31, 20);
			}
			break;
		case MERCURY_LIGHTHOUSE_UPPER_FROM_MERCURY_LIGHTHOUSE_MIDDLE_NORTHWEST:
		case MERCURY_LIGHTHOUSE_UPPER_WATERFALLS_FROM_GAP:
			scMercuryLighthouseArriveWithSparks();
			/* Fall through. */
		case MERCURY_LIGHTHOUSE_UPPER_FROM_MERCURY_LIGHTHOUSE_MIDDLE_WEST_NORTHWEST:
		case MERCURY_LIGHTHOUSE_UPPER_FROM_MERCURY_LIGHTHOUSE_MIDDLE_NORTH_NORTHEAST:
		case MERCURY_LIGHTHOUSE_UPPER_FROM_MERCURY_LIGHTHOUSE_EXTERIOR_WEST:
			fdQueueMusic(SE_AMBIENT_WATER);
			break;
		case MERCURY_LIGHTHOUSE_UPPER_FROM_MERCURY_LIGHTHOUSE_LOWER:
		case MERCURY_LIGHTHOUSE_UPPER_GATE_FROM_ROW_STATUES:
		case MERCURY_LIGHTHOUSE_UPPER_GATE_FROM_JETS:
			if(gmFlagGet(FLAG_KEEP_PARTY_POSITION) &&
				gmFlagGet(FLAG_LEADER_ON_FLOOR_SWITCH)) {
				fdActorGet(ACTOR_PARTY_LEADER)->y.fixed = -toFIXED(2);
				fdActorGet(ACTOR_PARTY_LEADER)->tgtY =
					fdActorGet(ACTOR_PARTY_LEADER)->y.fixed;
				fdMapGridCopyMetatilesTo(6, 29, 10, 23, 1, 1);
				fdMapGridCopyMetatilesTo(10, 28, 10, 18, 1, 2);
			}
			/* The upper floors' gate held open the same way: the sealed middle
			   arch is drawn as an open doorway. */
			if(gmFlagGet(FLAG_MERCURY_LIGHTHOUSE_UPPER_GATE_HELD_OPEN)) {
				fdActorSetPos(ACTOR_FALLEN_STATUE, 168, 376);
				fdActorGet(ACTOR_FALLEN_STATUE)->y.fixed -= toFIXED(2);
				fdActorGet(ACTOR_FALLEN_STATUE)->tgtY =
					fdActorGet(ACTOR_FALLEN_STATUE)->y.fixed;
				fdMapGridCopyMetatilesTo(6, 29, 10, 23, 1, 1);
				fdMapGridCopyMetatilesTo(10, 28, 10, 18, 1, 2);
				fdMapGridCopyMetatileAttributes(10, 16, 1, 1, 10, 19);
				fdMapGridCenterOnFollowedPoint();
			}
			break;
		case MERCURY_LIGHTHOUSE_UPPER_JETS_FROM_GATE_STATUE_FALLS:
			sySynch(1);
#if JET_ROOM_RESTORED_AFTER_STATUE_FALL
			if(gmFlagGet(FLAG_WEST_EAST_PIPE_PUSHED))
				fdActorSetPos(ACTOR_WEST_EAST_PIPE, 816, 608);
			if(gmFlagGet(FLAG_SWITCH_PIPE_STARTS_EASTWARD_JET))
				fdActorSetPos(ACTOR_JET_SWITCH_PIPE, 776, 576);
			else if(gmFlagGet(FLAG_SWITCH_PIPE_STARTS_WESTWARD_JET))
				fdActorSetPos(ACTOR_JET_SWITCH_PIPE, 760, 576);
			else if(gmFlagGet(FLAG_SWITCH_PIPE_STOPS_JETS))
				fdActorSetPos(ACTOR_JET_SWITCH_PIPE, 856, 576);
#else
			fdActorSetPos(ACTOR_WEST_EAST_PIPE, 816, 608);
			fdActorSetPos(ACTOR_JET_SWITCH_PIPE, 776, 576);
#endif
			scSetActorSprPriority(fdActorGet(ACTOR_PARTY_LEADER), 1);
			scMercuryLighthouseStopJet(JET_SOUTHWARD);
#if JET_ROOM_RESTORED_AFTER_STATUE_FALL
			if(gmFlagGet(FLAG_SWITCH_PIPE_STARTS_WESTWARD_JET)) {
				scMercuryLighthouseStartJet(JET_WESTWARD);
				if(FIELD_METATILE(fdActorGet(ACTOR_SLIDING_STATUE)->z.fixed) ==
					SLIDING_STATUE_UNDER_JET_ROW)
					sySetFrameEvent(scMercuryLighthouseSparkleAboveStatue,
						EVENT_PASS_FRAME);
			} else if(gmFlagGet(FLAG_SWITCH_PIPE_STARTS_EASTWARD_JET)) {
				scMercuryLighthouseStartJet(JET_EASTWARD);
			} else if(!gmFlagGet(FLAG_SWITCH_PIPE_STOPS_JETS)) {
				scMercuryLighthouseStartJet(JET_SOUTHWARD);
			}
#else
			scMercuryLighthouseStartJet(JET_EASTWARD);
#endif
			/* Fall through. */
		case MERCURY_LIGHTHOUSE_UPPER_JETS_FROM_GATE:
			if(MainRoutineStateWork.entrance ==
				MERCURY_LIGHTHOUSE_UPPER_JETS_FROM_GATE)
				sySndPlay(SE_MERCURY_LIGHTHOUSE_MECHANISM);
			scSetActorSprPriority(fdActorGet(ACTOR_SLIDING_STATUE), 1);
			fdActorSetSprPriority(ACTOR_WEST_EAST_PIPE, 2);
			fdActorGet(ACTOR_WEST_EAST_PIPE)->layer = 2;
			scSetActorSprPriority(fdActorGet(ACTOR_JET_SWITCH_PIPE), 1);
			scSetActorSprPriority(fdActorGet(ACTOR_FIRST_JET_ROOM_PIPE_END), 1);
			scSetActorSprPriority(
				fdActorGet(ACTOR_FIRST_JET_ROOM_PIPE_END + 1), 1);
			scSetActorSprPriority(
				fdActorGet(ACTOR_FIRST_JET_ROOM_PIPE_END + 2), 1);
			scPushedActorBlockFootprint(ACTOR_WEST_EAST_PIPE);
			scPushedActorBlockFootprint(ACTOR_JET_SWITCH_PIPE);
			if(MainRoutineStateWork.entrance !=
				MERCURY_LIGHTHOUSE_UPPER_JETS_FROM_GATE)
				break;
			if(!gmFlagGet(FLAG_KEEP_PARTY_POSITION)) {
				gmFlagClear(FLAG_WEST_EAST_PIPE_PUSHED);
				gmFlagClear(FLAG_SWITCH_PIPE_STARTS_WESTWARD_JET);
				gmFlagClear(FLAG_SWITCH_PIPE_STARTS_EASTWARD_JET);
				gmFlagClear(FLAG_SWITCH_PIPE_STOPS_JETS);
				break;
			}
			scMercuryLighthouseStopJet(JET_SOUTHWARD);
			if(gmFlagGet(FLAG_SWITCH_PIPE_STARTS_WESTWARD_JET)) {
				scMercuryLighthouseStartJet(JET_WESTWARD);
				if(FIELD_METATILE(fdActorGet(ACTOR_SLIDING_STATUE)->z.fixed) ==
					SLIDING_STATUE_UNDER_JET_ROW)
					sySetFrameEvent(scMercuryLighthouseSparkleAboveStatue,
						EVENT_PASS_FRAME);
			} else if(gmFlagGet(FLAG_SWITCH_PIPE_STARTS_EASTWARD_JET)) {
				scMercuryLighthouseStartJet(JET_EASTWARD);
			} else if(!gmFlagGet(FLAG_SWITCH_PIPE_STOPS_JETS)) {
				scMercuryLighthouseStartJet(JET_SOUTHWARD);
			}
			break;
		case MERCURY_LIGHTHOUSE_UPPER_GATE_FROM_JETS_STATUE_FALLS:
			fdScriptStart();
			scSetActorSprPriority(fdActorGet(ACTOR_FALLEN_STATUE), 1);
			fdActorSetSprColById(ACTOR_PARTY_LEADER, SPRITE_COLOR_HIDDEN);
			fdActorSetSprFlags(fdActorGet(ACTOR_PARTY_LEADER), 0);
			fdActorGet(ACTOR_FALLEN_STATUE)->y.fixed = toFIXED(144);
			fdActorGet(ACTOR_FALLEN_STATUE)->motionFlg = 0;
			fdActorGet(ACTOR_FALLEN_STATUE)->bounce = 0;
			fdActorGet(ACTOR_FALLEN_STATUE)->gravity = toFIXED(3) / 10;
			FieldStateWork->transition =
				SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
			fdScriptStartScreenTransition();
			fdScriptWaitTransitionFrames();
			fdActorGet(ACTOR_FALLEN_STATUE)->motionFlg =
				ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
			sySndPlay(SE_STONE_GRIND);
			fdScriptWait(32);
			sySndPlay(SE_THUMP);
			scSetActorSprPriority(fdActorGet(ACTOR_FALLEN_STATUE), 2);
			fdMapGridSetShake(3, 3, 1);
			fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
			fdMapGridWaitForShakeToSettle();
			fdScriptWait(60);
			syImportScriptSetExitRequest(
				MERCURY_LIGHTHOUSE_UPPER_GATE_TO_JETS_STATUE_FALLS);
			fdScriptEnd();
			break;
		}
	}
	if(MainRoutineStateWork.next == SCENE_MERCURY_LIGHTHOUSE_EXTERIOR) {
		/* This scene contains Mia's entrance, the lift and the healing
		   fountain. */
		switch(MainRoutineStateWork.entrance) {
		case MERCURY_LIGHTHOUSE_EXTERIOR_WATERFALL_FROM_UPPER_PATH:
			sySetFrameEvent(
				scMercuryLighthouseCycleWaterfallPalette, EVENT_PASS_FRAME);
			if(gmFlagGet(FLAG_KEEP_PARTY_POSITION))
				break;
			scMercuryLighthouseArriveWithSparks();
			fdQueueMusic(SE_AMBIENT_WATER);
			fdPaletteFadeApplySource(PALETTE_TINT_NONE, PALETTES_ALL);
			fdPaletteFadeApplyTarget(PALETTE_TINT_DIM, PALETTES_BG);
			fdPaletteFadeStartInterpolation(30);
			fdScriptWaitTransitionFrames();
			fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
			fdScriptWait(30);
			mnFieldMessageShowCentered(MsgMercuryHonorsGoddessRainbows, 0, 0);
			fdPaletteFadeApplyTarget(PALETTE_TINT_NONE, PALETTES_ALL);
			fdPaletteFadeStartInterpolation(30);
			break;
		case MERCURY_LIGHTHOUSE_EXTERIOR_FROM_MERCURY_LIGHTHOUSE_AERIE:
			fdActorSetPos(ACTOR_MIA, 0, 0);
			fdBlendScriptSuspend();
			IO_WRITE16(0, &REG_BLDCNT);
			fdActorGet(ACTOR_EAST_APPROACH_STATUE)->scaleX = -toFIXED(1);
			fdActorSetSprPriority(ACTOR_FIRST_VISITOR, 1);
			fdActorSetSprPriority(ACTOR_FIRST_VISITOR + 1, 1);
			fdActorSetSprPriority(ACTOR_FIRST_VISITOR + 2, 1);
			keepPartyPosition = gmFlagGet(FLAG_KEEP_PARTY_POSITION);
			/* The lift's place with the lift gone back up, the two copies
			   scMercuryLighthouseRideLift ends with; the arrivals from the
			   entrance and the World Map below draw the same. */
			if(keepPartyPosition) {
				fdMapGridCopyMetatileAttributes(104, 34, 5, 4, 40, 34);
				fdMapGridCopyMetatilesTo(45, 91, 40, 91, 5, 4);
				break;
			}
			fdActorSetPos(ACTOR_DOOR_STATUE, 824, 600);
			fdMapGridCopyMetatileAttributes(116, 36, 3, 4, 52, 36);
			fdActorGet(ACTOR_DOOR_STATUE)->motionFlg = keepPartyPosition;
			scSetActorSprPriority(fdActorGet(ACTOR_DOOR_STATUE), 1);
			fdScriptStart();
			fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
			fdCameraGetActor()->motionFlg = keepPartyPosition;
			fdActorSetSprPriority(ACTOR_PARTY_LEADER, 1);
			fdActorSetSprPriority(ACTOR_LIFT_STATUE, 1);
			scMercuryLighthouseRideLift();
			fdBlendScriptResume();
			fdScriptEnd();
			break;
		case MERCURY_LIGHTHOUSE_EXTERIOR_FROM_MERCURY_LIGHTHOUSE_ENTRANCE:
			fdScriptWait(1);
			scSetActorSprPriority(fdActorGet(ACTOR_PARTY_LEADER), 1);
			/* Fall through. */
		case MERCURY_LIGHTHOUSE_EXTERIOR_FROM_WORLD_MAP:
			fdMapGridCopyMetatileAttributes(104, 34, 5, 4, 40, 34);
			fdMapGridCopyMetatilesTo(45, 91, 40, 91, 5, 4);
			if(!gmFlagGet(FLAG_SATUROS_BEATEN_AT_MERCURY)) {
				fdMapGridCopyMetatilesTo(30, 45, 50, 45, 5, 6);
				fdMapGridCopyMetatilesTo(50, 105, 50, 109, 5, 3);
				fdMapGridCenterOnFollowedPoint();
				sySynch(1);
			} else {
				fdActorSetSprPriority(ACTOR_FIRST_VISITOR, 1);
				fdActorSetSprPriority(ACTOR_FIRST_VISITOR + 1, 1);
				fdActorSetSprPriority(ACTOR_FIRST_VISITOR + 2, 1);
			}
			fdActorGet(ACTOR_EAST_APPROACH_STATUE)->scaleX = -toFIXED(1);
			/* Until Mia has hurried here from Imil (FLAG_IMIL_MET_MIA, set in
			   imil.c), she is not here and the door stays shut. */
			if(!gmFlagGet(FLAG_IMIL_MET_MIA)) {
				fdActorSetPos(ACTOR_MIA, 0, 0);
				fdActorSetPos(ACTOR_DOOR_STATUE, 824, 600);
				fdActorSetSprPriority(ACTOR_DOOR_STATUE, 1);
				fdMapGridCopyMetatileAttributes(116, 37, 3, 3, 52, 37);
				fdMapGridCopyMetatilesTo(126, 35, 116, 35, 1, 2);
				sySetFrameEvent(
					scMercuryLighthouseCycleEntrancePalette, EVENT_PASS_FRAME);
				break;
			}
			if(!gmFlagGet(FLAG_MERCURY_LIGHTHOUSE_OPENED_BY_MIA)) {
				if(!gmFlagGet(FLAG_MIA_SENSED_SOMEONE_INSIDE)) {
					fdActorFaceDirection(ACTOR_MIA, FACING_EAST, 0);
					fdActorSetAnimation(ACTOR_MIA, ANIM_MIA_KNEEL);
				} else {
					fdActorSetPos(ACTOR_MIA, 840, 632);
					fdActorFaceDirection(ACTOR_MIA, FACING_NORTH, 0);
				}
				scSetActorSprPriority(fdActorGet(ACTOR_MIA), 1);
				fdMapGridCopyMetatilesTo(126, 35, 116, 35, 1, 2);
				sySetFrameEvent(
					scMercuryLighthouseCycleEntrancePalette, EVENT_PASS_FRAME);
			} else {
				fdActorSetPos(ACTOR_MIA, 0, 0);
				fdActorSetPos(ACTOR_DOOR_STATUE, 824, 600);
				fdMapGridCopyMetatileAttributes(116, 36, 3, 4, 52, 36);
				fdActorGet(ACTOR_DOOR_STATUE)->collisionFlg =
					(Uint8)~ACTOR_SOLID;
				fdScriptWait(1);
			}
			fdActorGet(ACTOR_DOOR_STATUE)->motionFlg = 0;
			scSetActorSprPriority(fdActorGet(ACTOR_DOOR_STATUE), 1);
			break;
		}
	}
	return 0;
}

/* Move on the blue statue in Mia's way, on the ledge up the stairs from the
   great pool room: once it has dropped into the gap at her feet, at column 48,
   Mia thanks the party ("You've saved me again, haven't you!"), asks their
   business in the lighthouse until they own to it, and joins them, bringing
   Fizz, a Mercury Djinni, already set.

   The gap's row (46, 39) to (50, 39) takes the walkable metatiles of (110, 39)
   to (114, 39). */
static void scMercuryLighthouseMiaJoinsAfterRescue(void)
{
	Sint32 firstMessage;
	Sint32 questionMessage;

	if(fdActorGet(ACTOR_STATUE_BLOCKING_MIA)->x.fixed / toFIXED(16) != 48) {
		return;
	}
	fdScriptStart();
	firstMessage = MsgMercurySavedMeAgain;
	fdScriptSetMessageId(firstMessage);
	fdScriptWait(20);
	fdActorRunRepeatedMotion(ACTOR_MIA, 1);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 20);
	/* "You've saved me again, haven't you!" */
	fdScriptShowMessageThenWait(ACTOR_MIA, 0, 20);
	fdActorSetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
	fdScriptWait(20);
	fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
	fdScriptWait(20);
	fdScriptWait(60);
	fdActorSetAnimation(ACTOR_MIA, ANIM_MIA_KNEEL);
	fdScriptWait(50);
	fdActorSetAnimation(ACTOR_MIA, ANIM_STAND);
	/* "Do you have business in the lighthouse, too?" */
	fdScriptOpenMessage(ACTOR_MIA, 0);
	if(fdScriptAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 1) {
		fdScriptWait(20);
		fdActorRunRepeatedMotion(ACTOR_MIA, 2);
		fdScriptWait(20);
		/* "Please tell me the truth!" */
		fdScriptShowMessageThenWait(ACTOR_MIA, 0, 20);
		fdActorSetAnimationAndWait(ACTOR_MIA, ANIM_SHAKE_HEAD);
		fdScriptWait(20);
		/* "I can't believe you'd go so far to help someone you just met." */
		fdScriptShowMessageThenWait(ACTOR_MIA, 0, 20);
		fdActorSetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
		fdScriptWait(20);
		/* "You have some errand in this lighthouse, don't you?" */
		fdScriptOpenMessage(ACTOR_MIA, 0);
		if(fdScriptAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 1) {
			fdScriptWait(20);
			MERCURY_LIGHTHOUSE_ANIMATE_ACTOR_AND_WAIT(
				ACTOR_MIA, ANIM_SHAKE_HEAD);
			fdScriptWait(20);
			/* She asks again until the party says yes: "Please tell me the
			   truth!" the first time, then "Please, don't hide anything. Just
			   tell me the truth!" */
			questionMessage = firstMessage + 5;
			for(;;) {
				fdScriptSetMessageId(questionMessage);
				fdScriptOpenMessage(ACTOR_MIA, 0);
				if(fdScriptAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) !=
					1) {
					break;
				}
				fdScriptWait(20);
				MERCURY_LIGHTHOUSE_ANIMATE_ACTOR_AND_WAIT(
					ACTOR_MIA, ANIM_SHAKE_HEAD);
				fdScriptWait(20);
				questionMessage = MsgMercuryDontHideTruth;
			}
		}
	}
	fdScriptSetMessageId(MsgMercuryThoughtSo);
	fdActorSetSpeed(ACTOR_MIA, 0.8);
	fdActorWalkToAndWait(ACTOR_MIA, 728, 632);
	fdScriptWait(20);
	/* "I thought so..." */
	fdScriptShowMessageThenWait(ACTOR_MIA, 0, 20);
	fdActorSetAnimation(ACTOR_MIA, ANIM_MIA_KNEEL);
	/* "I am sure there will be more places ahead that I cannot pass..." */
	fdScriptShowMessageThenWait(ACTOR_MIA, 0, 20);
	fdActorSetAnimation(ACTOR_MIA, ANIM_STAND);
	fdActorFaceActor(ACTOR_MIA, ACTOR_PARTY_LEADER, 20);
	fdActorSetAnimationAndWait(ACTOR_MIA, ANIM_SHAKE_HEAD);
	fdScriptWait(20);
	/* "Only those of the Mercury Clan are permitted to enter this..." */
	fdScriptShowMessageThenWait(ACTOR_MIA, 0, 20);
	fdActorShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 90);
	fdActorSetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
	fdScriptWait(20);
	/* "However, I sense something special about you. I feel that I..." */
	fdScriptShowMessageThenWait(ACTOR_MIA, 0, 20);
	fdScriptAddPartyMember(MIA, 1);
	/* "The Adept Mia joined your party!" Fizz, a Mercury Djinni, comes with her
	   and is set; her stats are worked out again. */
	gmFlagSet(FLAG_DJINNI(DJINNI_FIZZ));
	gmDjinnGive(MIA, ELEMENT_MERCURY, DJINNI_INDEX(DJINNI_FIZZ));
	gmDjinnSet(MIA, ELEMENT_MERCURY, DJINNI_INDEX(DJINNI_FIZZ));
	gmUnitCalc(MIA);
	scWalkIntoLeader(ACTOR_MIA);
	fdMapGridCopyMetatileAttributes(110, 39, 5, 1, 46, 39);
	gmFlagSet(FLAG_MIA_JOINED_PARTY);
	fdScriptEnd();
}

/* Coming down from the Lighthouse Aerie
   (MERCURY_LIGHTHOUSE_EXTERIOR_FROM_MERCURY_LIGHTHOUSE_AERIE), Isaac rides the
   lift down beside its blue statue, slowing as it nears the bottom, hops off,
   and the lift goes back up without him.

   The lift is drawn on the overhead layer, scrolled with both riders over 137
   pixels. */
static void scMercuryLighthouseRideLift(void)
{
	MAP_GRID_LAYER_SCROLL *liftLayer;
	FIXED				   liftSpeed;

	liftLayer = &((MAP_GRID_SCROLL_STATE *)MapGridState[0])
					 ->layers[MAPGRID_LAYER_OVERHEAD];
	liftSpeed = toFIXED(0.61);
	liftLayer->ofsY = toFIXED(1161);
	liftLayer->speedY = 0;
	fdActorGet(ACTOR_PARTY_LEADER)->motionFlg = 0;
	fdActorGet(ACTOR_PARTY_LEADER)->z.fixed -= toFIXED(137);
	fdActorGet(ACTOR_PARTY_LEADER)->tgtZ =
		fdActorGet(ACTOR_PARTY_LEADER)->z.fixed;
	fdActorGet(ACTOR_LIFT_STATUE)->motionFlg = 0;
	fdActorSetPos(ACTOR_LIFT_STATUE, 680, 440);
	fdActorGet(ACTOR_LIFT_STATUE)->z.fixed -= toFIXED(137);
	fdActorGet(ACTOR_LIFT_STATUE)->tgtZ =
		fdActorGet(ACTOR_LIFT_STATUE)->z.fixed;
	fdMapGridCenterOnFollowedPoint();
	sySynch(1);
	fdScriptStartScreenTransition();
	fdScriptWaitTransitionFrames();
	fdScriptWait(60);
	fdBlendScriptSuspend();
	sySndPlay(SE_LIFT);
	/* The lift layer scrolls down with both riders, 137 pixels, slowing as it
	   nears the landing. */
	for(;;) {
		liftLayer->ofsY -= liftSpeed;
		fdActorGet(ACTOR_PARTY_LEADER)->z.fixed += liftSpeed;
		fdActorGet(ACTOR_PARTY_LEADER)->tgtZ =
			fdActorGet(ACTOR_PARTY_LEADER)->z.fixed;
		fdActorGet(ACTOR_LIFT_STATUE)->z.fixed += liftSpeed;
		fdActorGet(ACTOR_LIFT_STATUE)->tgtZ =
			fdActorGet(ACTOR_LIFT_STATUE)->z.fixed;
		if(liftLayer->ofsY > toFIXED(1024)) {
			if((FrameCount & 15) == 0 && liftSpeed >= toFIXED(0.05))
				liftSpeed -= toFIXED(0.021);
			sySynch(1);
			continue;
		}
		break;
	}
	liftLayer->ofsY = toFIXED(1024);
	fdMapGridCenterOnFollowedPoint();
	sySynch(2);
	fdActorGet(ACTOR_PARTY_LEADER)->motionFlg =
		ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
	fdActorGet(ACTOR_LIFT_STATUE)->z.fixed = toFIXED(440);
	fdActorGet(ACTOR_LIFT_STATUE)->tgtZ =
		fdActorGet(ACTOR_LIFT_STATUE)->z.fixed;
	fdScriptWait(30);
	fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 704, 584);
	fdActorGet(ACTOR_PARTY_LEADER)->bounce = 0;
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_LEADER_SPRING);
	fdScriptWait(6);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_LEADER_LEAP);
	fdActorGet(ACTOR_PARTY_LEADER)->speedLimit = toFIXED(3);
	fdActorGet(ACTOR_PARTY_LEADER)->acceleration = toFIXED(2);
	sySndPlay(SE_JUMP);
	fdActorGet(ACTOR_PARTY_LEADER)->velY = toFIXED(4);
	fdActorSetSprFlags(fdActorGet(ACTOR_PARTY_LEADER), 0);
	fdActorSetDestination(ACTOR_PARTY_LEADER, 736, 584);
	fdActorWaitForMoveById(ACTOR_PARTY_LEADER);
	fdActorSetSprFlags(fdActorGet(ACTOR_PARTY_LEADER), SPRITE_FLAG_SHADOW);
	fdActorGet(ACTOR_PARTY_LEADER)->bounce = toFIXED(1) / 4;
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_LEADER_SPRING);
	fdScriptWait(6);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 30);
	sySndPlay(SE_LIFT);
	/* The lift and its statue go back up without the leader. */
	for(;;) {
		liftLayer->ofsY += liftSpeed;
		fdActorGet(ACTOR_LIFT_STATUE)->z.fixed -= liftSpeed;
		fdActorGet(ACTOR_LIFT_STATUE)->tgtZ =
			fdActorGet(ACTOR_LIFT_STATUE)->z.fixed;
		if(liftLayer->ofsY >= toFIXED(1161))
			break;
		if((FrameCount & 7) == 0 && liftSpeed <= toFIXED(4) / 5)
			liftSpeed += toFIXED(1) / 10;
		sySynch(1);
	}
	liftLayer->ofsY = toFIXED(1024);
	fdMapGridCopyMetatilesTo(45, 91, 40, 91, 5, 4);
	fdMapGridCopyMetatileAttributes(104, 34, 5, 4, 40, 34);
	fdMapGridCenterOnFollowedPoint();
	sySynch(2);
	fdActorSetPos(ACTOR_LIFT_STATUE, 0, 0);
	fdScriptWait(30);
	fdActorGet(ACTOR_PARTY_LEADER)->priFlg |= ACTOR_PRIORITY_AUTOMATIC;
}

/* Mia casts Ply on the Ply target, moved onto the door. */
static void scMercuryLighthouseMiaCastsPlyOnEntrance(void)
{
	FIELD_EFFECT_STATE *effectState;
	Uint8				flags;

	effectState = FieldEffectStateWork;
	fdActorSetPos(ACTOR_PLY_TARGET, 840, 600);
	fdPsynergyLoadResources(ABILITY_PLY, 1);
	fdPsynergySetupActorPair(ACTOR_MIA, ACTOR_PLY_TARGET);
	flags = (Uint8)effectState->msgMode | FIELD_MESSAGE_LOWER;
	effectState->msgMode = flags;
	fdPsynergyStartCast();
	fdPsynergyRunHandler(1);
	fdPsynergyStopCasterUpdate();
}

#include "shared/hermes_water_bottle.c"

#include "shared/mercury_lighthouse_lights.c"

/* Spreads one blue ripple (SPRITE_RIPPLE) from an actor, for 30 frames. */
static void scMercuryLighthouseSpawnRipple(ACTOR *source)
{
	ACTOR *effect;
	SPR	  *sprite;

	effect = syFieldActorCreate(
		SPRITE_RIPPLE, source->x.fixed, source->y.fixed, source->z.fixed);
	if(effect == NULL) {
		return;
	}

	sprite = effect->spr;
	fdActorStartScript(effect, MercuryLighthouse_RippleScript);

	effect->motionFlg = 0;
	effect->layer = 1;
	effect->priFlg = ACTOR_PRIORITY_UNDERFOOT;

	if(sprite != NULL) {
		fdSprStartAnimation(sprite, 2);
		sprite->flg = 0;
		sprite->image.pri = 3;
	}
}

#include "shared/mercury_lighthouse_pillar.c"

#include "shared/field_effect_duration_scripts.c"

#include "shared/pushed_actor_tables.c"
/* clang-format off */

Sint32 MercuryLighthouse_RippleScript[] = {
	SC_WAIT(30),
	SC_RELEASE,
};

Sint32 MercuryLighthouse_SparkScript[] = {
	SC_CALL(scMercuryLighthouseRiseSpark),
	SC_RELEASE,
};

Sint32 MercuryLighthouse_GrowLightScript[] = {
	SC_CALL(scMercuryLighthouseGrowLight),
	SC_END,
};

Sint32 MercuryLighthouse_LightOutScript[] = {
	SC_CALL(scMercuryLighthouseSwellLightOut),
	SC_RELEASE,
};

#include "shared/mercury_lighthouse_pillar_tables.c"

const Sint32 *FieldEffect_DurationScripts[] = {
	FieldEffect_ShortScript,
	FieldEffect_MediumScript,
	FieldEffect_LongScript,
};

#include "shared/glint_twinkle_script.c"

/* The blue statue's slides in the room of jets, each three quarters of a */
/* second or a second after the jet starts: JET_WESTWARD pushes it west under */
/* the falling jet's pipe end, JET_SOUTHWARD south to the row of the hole, */
/* and JET_EASTWARD east into the hole. */
Sint32 MercuryLighthouse_StatueSlideWest[] = {
	SC_WAIT(45),
	SC_SPEED(0.8),
	SC_ACCELERATION(0.2),
	SC_CALL(scMercuryLighthouseStartStatueSparkles),
	SC_MOVE(792, 0, 712),
	SC_SYNC,
	SC_END,
};

Sint32 MercuryLighthouse_StatueSlideSouth[] = {
	SC_WAIT(60),
	SC_SPEED(0.8),
	SC_ACCELERATION(0.2),
	SC_CALL(scMercuryLighthouseStartStatueSparkles),
	SC_MOVE(792, 0, 744),
	SC_SYNC,
	SC_CALL(scMercuryLighthouseStopStatueSparkles),
	SC_END,
};

Sint32 MercuryLighthouse_StatueSlideIntoHole[] = {
	SC_WAIT(60),
	SC_SPEED(0.8),
	SC_ACCELERATION(0.2),
	SC_CALL(scMercuryLighthouseStartStatueSparkles),
	SC_MOVE(824, 0, 744),
	SC_SYNC,
	SC_CALL(scMercuryLighthouseStopStatueSparkles),
	SC_END,
};

/* JET_WESTWARD again once the statue is under the falling jet's pipe end: */
/* the sparkles start, but the statue stays where it is. */
Sint32 MercuryLighthouse_StatueStaysPut[] = {
	SC_WAIT(45),
	SC_CALL(scMercuryLighthouseStartStatueSparkles),
	SC_END,
};

/* Unused, with PaletteCycleGreen and PaletteCycleBlue: nothing reads them or */
/* changes them. */
Sint32 PaletteCycleRed = 0;
Sint32 PaletteCycleGreen = 0;
Sint32 PaletteCycleBlue = 0;

/* The gate room's sealed arch on the dais opening a column at a time, and */
/* closing; then the upper gate room's middle arch, the same. */
Uint16 MercuryLighthouse_GateMetatiles[] = {
	MAPGRID_ANIMATION_STEP(15, 1, 1, 2, 10), MAPGRID_ANIMATION_STEP(13, 1, 1, 2, 6), MAPGRID_ANIMATION_STEP(11, 1, 1, 2, 6), MAPGRID_ANIMATION_STEP(9, 1, 1, 2, 6), MAPGRID_ANIMATION_STEPS_END,
};
Uint16 MercuryLighthouse_GateCloseMetatiles[] = {
	MAPGRID_ANIMATION_STEP(9, 1, 1, 2, 10), MAPGRID_ANIMATION_STEP(11, 1, 1, 2, 6), MAPGRID_ANIMATION_STEP(13, 1, 1, 2, 6), MAPGRID_ANIMATION_STEP(15, 1, 1, 2, 6), MAPGRID_ANIMATION_STEPS_END,
};
Uint16 MercuryLighthouse_UpperGateMetatiles[] = {
	MAPGRID_ANIMATION_STEP(16, 28, 1, 2, 10), MAPGRID_ANIMATION_STEP(14, 28, 1, 2, 6), MAPGRID_ANIMATION_STEP(12, 28, 1, 2, 6), MAPGRID_ANIMATION_STEP(10, 28, 1, 2, 6), MAPGRID_ANIMATION_STEPS_END,
};
Uint16 MercuryLighthouse_UpperGateCloseMetatiles[] = {
	MAPGRID_ANIMATION_STEP(10, 28, 1, 2, 10), MAPGRID_ANIMATION_STEP(12, 28, 1, 2, 6), MAPGRID_ANIMATION_STEP(14, 28, 1, 2, 6), MAPGRID_ANIMATION_STEP(16, 28, 1, 2, 6), MAPGRID_ANIMATION_STEPS_END,
};

/* The lighthouse door opening in five steps as Mia casts Ply on it. */
Uint16 MercuryLighthouse_ColumnMetatiles[] = {
	MAPGRID_ANIMATION_STEP(126, 35, 1, 2, 2), MAPGRID_ANIMATION_STEP(126, 38, 1, 2, 2), MAPGRID_ANIMATION_STEP(126, 41, 1, 2, 2), MAPGRID_ANIMATION_STEP(126, 44, 1, 2, 2), MAPGRID_ANIMATION_STEP(126, 47, 1, 2, 2), MAPGRID_ANIMATION_STEPS_END,
};

SCENE_ENTRANCE MercuryLighthouseLowerEntrances[] = {
	{ ENTRANCE_DEFAULT, CONDITION_ALWAYS, 248, 0, 344, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ MERCURY_LIGHTHOUSE_LOWER_FROM_MERCURY_LIGHTHOUSE_ENTRANCE_WEST, CONDITION_ALWAYS, 56, 0, 440, FACING_NORTH, 0, 16, 16, 496, 448, 0 },
	{ MERCURY_LIGHTHOUSE_LOWER_FROM_MERCURY_LIGHTHOUSE_ENTRANCE_WEST_NORTHWEST, CONDITION_ALWAYS, 248, 0, 440, FACING_NORTH, 0, 16, 16, 496, 448, 0 },
	{ MERCURY_LIGHTHOUSE_LOWER_POOL_FROM_STATUE, CONDITION_ALWAYS, 248, 0, 344, FACING_SOUTH, 0, 16, 16, 496, 448, 0 },
	{ MERCURY_LIGHTHOUSE_LOWER_FROM_MERCURY_LIGHTHOUSE_ENTRANCE_EAST_NORTHEAST, CONDITION_ALWAYS, 760, 0, 344, FACING_NORTH, 0, 544, 16, 816, 352, 0 },
	{ MERCURY_LIGHTHOUSE_LOWER_WATERFALLS_FROM_STATUE, CONDITION_ALWAYS, 120, 0, 936, FACING_NORTH, 0, 64, 784, 384, 944, 0 },
	{ MERCURY_LIGHTHOUSE_LOWER_FROM_MERCURY_LIGHTHOUSE_MIDDLE_SOUTH, CONDITION_ALWAYS, 344, 0, 936, FACING_NORTH, 0, 64, 784, 384, 944, 0 },
	{ MERCURY_LIGHTHOUSE_LOWER_FROM_MERCURY_LIGHTHOUSE_ENTRANCE_SOUTHWEST, CONDITION_ALWAYS, 120, 0, 584, FACING_SOUTH, 0, 80, 528, 320, 688, 0 },
	{ MERCURY_LIGHTHOUSE_LOWER_FROM_MERCURY_LIGHTHOUSE_UPPER, CONDITION_ALWAYS, 280, 0, 584, FACING_SOUTH, 0, 80, 528, 320, 688, 0 },
	{ MERCURY_LIGHTHOUSE_LOWER_STATUE_FROM_POOL, CONDITION_ALWAYS, 776, 0, 472, FACING_SOUTH, 0, 640, 416, 896, 704, 0 },
	{ MERCURY_LIGHTHOUSE_LOWER_STATUE_FROM_WATERFALLS, CONDITION_ALWAYS, 856, 0, 520, FACING_SOUTH, 0, 640, 416, 896, 704, 0 },
	{ MERCURY_LIGHTHOUSE_LOWER_FROM_MERCURY_LIGHTHOUSE_MIDDLE_NORTH, CONDITION_ALWAYS, 472, 0, 696, FACING_NORTH, 0, 352, 512, 592, 720, 0 },
	{ MERCURY_LIGHTHOUSE_LOWER_FROM_MERCURY_LIGHTHOUSE_ENTRANCE_PIPE, CONDITION_ALWAYS, 712, 0, 264, FACING_SOUTH, 0, 544, 16, 816, 352, 0 },
	{ SCENE_TABLE_END },
};

SCENE_ENTRANCE MercuryLighthouseMiddleEntrances[] = {
	{ ENTRANCE_DEFAULT, CONDITION_ALWAYS, 248, 0, 344, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ MERCURY_LIGHTHOUSE_MIDDLE_FROM_MERCURY_LIGHTHOUSE_LOWER_SOUTH, CONDITION_ALWAYS, 120, 0, 472, FACING_SOUTH, 0, 32, 400, 480, 848, 0 },
	{ MERCURY_LIGHTHOUSE_MIDDLE_PIPES_FROM_GATE, CONDITION_ALWAYS, 152, 0, 824, FACING_NORTH, 0, 32, 400, 480, 848, 0 },
	{ MERCURY_LIGHTHOUSE_MIDDLE_FROM_MERCURY_LIGHTHOUSE_LOWER_NORTH, CONDITION_ALWAYS, 152, 0, 184, FACING_SOUTH, 0, 48, 16, 592, 416, 0 },
	{ MERCURY_LIGHTHOUSE_MIDDLE_GATE_FROM_PIPES, CONDITION_ALWAYS, 488, 0, 136, FACING_SOUTH, 0, 48, 16, 592, 416, 0 },
	{ MERCURY_LIGHTHOUSE_MIDDLE_FROM_MERCURY_LIGHTHOUSE_UPPER_WEST_NORTHWEST, CONDITION_ALWAYS, 728, 0, 88, FACING_SOUTH, 0, 656, 16, 896, 272, 0 },
	{ MERCURY_LIGHTHOUSE_MIDDLE_FROM_MERCURY_LIGHTHOUSE_EXTERIOR, CONDITION_ALWAYS, 728, 0, 216, FACING_SOUTH, 0, 656, 16, 896, 272, 0 },
	{ MERCURY_LIGHTHOUSE_MIDDLE_FROM_MERCURY_LIGHTHOUSE_UPPER_NORTHEAST, CONDITION_ALWAYS, 632, 0, 712, FACING_NORTH, 0, 576, 480, 832, 736, 0 },
	{ MERCURY_LIGHTHOUSE_MIDDLE_FROM_MERCURY_LIGHTHOUSE_UPPER_EAST_NORTHEAST, CONDITION_ALWAYS, 792, 0, 712, FACING_NORTH, 0, 576, 480, 832, 736, 0 },
	{ MERCURY_LIGHTHOUSE_MIDDLE_FROM_MERCURY_LIGHTHOUSE_UPPER_NORTH_NORTHEAST, CONDITION_ALWAYS, 712, 0, 568, FACING_SOUTH, 0, 576, 480, 832, 736, 0 },
	{ MERCURY_LIGHTHOUSE_MIDDLE_FROM_MERCURY_LIGHTHOUSE_UPPER_NORTHWEST, CONDITION_ALWAYS, 888, 0, 440, FACING_NORTH, 0, 760, 304, 1040, 464, 0 },
	{ SCENE_TABLE_END },
};

SCENE_ENTRANCE MercuryLighthouseUpperEntrances[] = {
	{ ENTRANCE_DEFAULT, CONDITION_ALWAYS, 248, 0, 344, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ MERCURY_LIGHTHOUSE_UPPER_FROM_MERCURY_LIGHTHOUSE_MIDDLE_WEST_NORTHWEST, CONDITION_ALWAYS, 88, 0, 184, FACING_NORTH, 0, 32, 32, 544, 208, 0 },
	{ MERCURY_LIGHTHOUSE_UPPER_FROM_MERCURY_LIGHTHOUSE_MIDDLE_NORTH_NORTHEAST, CONDITION_ALWAYS, 504, 0, 184, FACING_NORTH, 0, 32, 32, 544, 208, 0 },
	{ MERCURY_LIGHTHOUSE_UPPER_FROM_MERCURY_LIGHTHOUSE_MIDDLE_NORTHWEST, CONDITION_ALWAYS, 264, 0, 120, FACING_SOUTH, 0, 32, 32, 544, 208, 0 },
	{ MERCURY_LIGHTHOUSE_UPPER_ROW_STATUES_FROM_GATE, CONDITION_ALWAYS, 648, 0, 168, FACING_NORTH, 0, 592, 16, 992, 320, 0 },
	{ MERCURY_LIGHTHOUSE_UPPER_FROM_MERCURY_LIGHTHOUSE_MIDDLE_NORTHEAST, CONDITION_ALWAYS, 776, 0, 88, FACING_SOUTH, 0, 592, 16, 992, 320, 0 },
	{ MERCURY_LIGHTHOUSE_UPPER_FROM_MERCURY_LIGHTHOUSE_MIDDLE_EAST_NORTHEAST, CONDITION_ALWAYS, 936, 0, 88, FACING_SOUTH, 0, 592, 16, 992, 320, 0 },
	{ MERCURY_LIGHTHOUSE_UPPER_FROM_MERCURY_LIGHTHOUSE_LOWER, CONDITION_ALWAYS, 88, 0, 328, FACING_SOUTH, 0, 48, 272, 288, 432, 0 },
	{ MERCURY_LIGHTHOUSE_UPPER_GATE_FROM_ROW_STATUES, CONDITION_ALWAYS, 168, 0, 328, FACING_SOUTH, 0, 48, 272, 288, 432, 0 },
	{ MERCURY_LIGHTHOUSE_UPPER_GATE_FROM_JETS, CONDITION_ALWAYS, 248, 0, 328, FACING_SOUTH, 0, 48, 272, 288, 432, 0 },
	{ MERCURY_LIGHTHOUSE_UPPER_FROM_MERCURY_LIGHTHOUSE_EXTERIOR_EAST, CONDITION_ALWAYS, 472, 0, 328, FACING_SOUTH, 0, 352, 272, 592, 432, 0 },
	{ MERCURY_LIGHTHOUSE_UPPER_GAP_FROM_WATERFALLS, CONDITION_ALWAYS, 424, 0, 408, FACING_NORTH, 0, 352, 272, 592, 432, 0 },
	{ MERCURY_LIGHTHOUSE_UPPER_FROM_MERCURY_LIGHTHOUSE_EXTERIOR_WEST, CONDITION_ALWAYS, 88, 0, 600, FACING_SOUTH, 0, 48, 512, 544, 672, 0 },
	{ MERCURY_LIGHTHOUSE_UPPER_WATERFALLS_FROM_GAP, CONDITION_ALWAYS, 456, 0, 600, FACING_SOUTH, 0, 48, 512, 544, 672, 0 },
	{ MERCURY_LIGHTHOUSE_UPPER_JETS_FROM_GATE, CONDITION_ALWAYS, 888, 0, 712, FACING_SOUTH, 0, 688, 384, 928, 816, 0 },
	{ MERCURY_LIGHTHOUSE_UPPER_GATE_FROM_JETS_STATUE_FALLS, CONDITION_ALWAYS, 168, 0, 368, FACING_SOUTH, 0, 40, 256, 296, 448, 0 },
	{ MERCURY_LIGHTHOUSE_UPPER_JETS_FROM_GATE_STATUE_FALLS, CONDITION_ALWAYS, 760, 0, 584, FACING_SOUTH, 0, 688, 384, 928, 816, 0 },
	{ SCENE_TABLE_END },
};

SCENE_ENTRANCE MercuryLighthouseExteriorEntrances[] = {
	{ ENTRANCE_DEFAULT, CONDITION_ALWAYS, 248, 0, 344, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ MERCURY_LIGHTHOUSE_EXTERIOR_FROM_MERCURY_LIGHTHOUSE_MIDDLE, CONDITION_ALWAYS, 232, 0, 104, FACING_SOUTH, 0, 64, 32, 288, 504, 0 },
	{ MERCURY_LIGHTHOUSE_EXTERIOR_FROM_MERCURY_LIGHTHOUSE_UPPER_WEST, CONDITION_ALWAYS, 232, 0, 488, FACING_NORTH, 0, 64, 32, 288, 504, 0 },
	{ MERCURY_LIGHTHOUSE_EXTERIOR_FROM_MERCURY_LIGHTHOUSE_UPPER_EAST, CONDITION_ALWAYS, 408, 0, 104, FACING_SOUTH, 0, 352, 32, 592, 328, 0 },
	{ MERCURY_LIGHTHOUSE_EXTERIOR_UPPER_PATH_FROM_WATERFALL, CONDITION_ALWAYS, 408, 0, 312, FACING_NORTH, 0, 352, 32, 592, 328, 0 },
	{ MERCURY_LIGHTHOUSE_EXTERIOR_WATERFALL_FROM_UPPER_PATH, CONDITION_ALWAYS, 792, 0, 216, FACING_SOUTH, 0, 664, 124, 920, 292, 0 },
	{ MERCURY_LIGHTHOUSE_EXTERIOR_FROM_WORLD_MAP, CONDITION_ALWAYS, 840, 0, 856, FACING_NORTH, 0, 576, 328, 960, 912, 0 },
	{ MERCURY_LIGHTHOUSE_EXTERIOR_FROM_MERCURY_LIGHTHOUSE_ENTRANCE, CONDITION_ALWAYS, 840, 0, 600, FACING_SOUTH, 0, 576, 328, 960, 912, 0 },
	{ MERCURY_LIGHTHOUSE_EXTERIOR_UNUSED, CONDITION_ALWAYS, 872, 0, 600, FACING_SOUTH, 0, 720, 544, 960, 912, 0 },
	{ MERCURY_LIGHTHOUSE_EXTERIOR_FROM_MERCURY_LIGHTHOUSE_AERIE, CONDITION_ALWAYS, 680, 0, 584, FACING_SOUTH, 0, 576, 328, 960, 912, 0 },
	{ SCENE_TABLE_END },
};

Uint32 MercuryLighthouse_Exits[] = {
	SCENE_EXITS(MercuryLighthouseLower),
	SCENE_EXIT(MERCURY_LIGHTHOUSE_LOWER_TO_MERCURY_LIGHTHOUSE_ENTRANCE_WEST, MercuryLighthouseEntrance, MERCURY_LIGHTHOUSE_ENTRANCE_FROM_MERCURY_LIGHTHOUSE_LOWER_WEST),
	SCENE_EXIT(MERCURY_LIGHTHOUSE_LOWER_TO_MERCURY_LIGHTHOUSE_ENTRANCE_WEST_NORTHWEST, MercuryLighthouseEntrance, MERCURY_LIGHTHOUSE_ENTRANCE_FROM_MERCURY_LIGHTHOUSE_LOWER_WEST_NORTHWEST),
	SCENE_EXIT(MERCURY_LIGHTHOUSE_LOWER_POOL_TO_STATUE, MercuryLighthouseLower, MERCURY_LIGHTHOUSE_LOWER_STATUE_FROM_POOL),
	SCENE_EXIT(MERCURY_LIGHTHOUSE_LOWER_TO_MERCURY_LIGHTHOUSE_ENTRANCE_EAST_NORTHEAST, MercuryLighthouseEntrance, MERCURY_LIGHTHOUSE_ENTRANCE_FROM_MERCURY_LIGHTHOUSE_LOWER_EAST_NORTHEAST),
	SCENE_EXIT(MERCURY_LIGHTHOUSE_LOWER_TO_MERCURY_LIGHTHOUSE_ENTRANCE_NORTHEAST, MercuryLighthouseEntrance, MERCURY_LIGHTHOUSE_ENTRANCE_FROM_MERCURY_LIGHTHOUSE_LOWER_NORTHEAST),
	SCENE_EXIT(MERCURY_LIGHTHOUSE_LOWER_TO_MERCURY_LIGHTHOUSE_UPPER, MercuryLighthouseUpper, MERCURY_LIGHTHOUSE_UPPER_FROM_MERCURY_LIGHTHOUSE_LOWER),
	SCENE_EXIT(MERCURY_LIGHTHOUSE_LOWER_TO_MERCURY_LIGHTHOUSE_MIDDLE_NORTH, MercuryLighthouseMiddle, MERCURY_LIGHTHOUSE_MIDDLE_FROM_MERCURY_LIGHTHOUSE_LOWER_NORTH),
	SCENE_EXIT(MERCURY_LIGHTHOUSE_LOWER_TO_MERCURY_LIGHTHOUSE_ENTRANCE_HOLE, MercuryLighthouseEntrance, MERCURY_LIGHTHOUSE_ENTRANCE_FROM_MERCURY_LIGHTHOUSE_LOWER_HOLE),
	SCENE_EXIT(MERCURY_LIGHTHOUSE_LOWER_STATUE_TO_POOL, MercuryLighthouseLower, MERCURY_LIGHTHOUSE_LOWER_POOL_FROM_STATUE),
	SCENE_EXIT(MERCURY_LIGHTHOUSE_LOWER_STATUE_TO_WATERFALLS, MercuryLighthouseLower, MERCURY_LIGHTHOUSE_LOWER_WATERFALLS_FROM_STATUE),
	SCENE_EXIT(MERCURY_LIGHTHOUSE_LOWER_WATERFALLS_TO_STATUE, MercuryLighthouseLower, MERCURY_LIGHTHOUSE_LOWER_STATUE_FROM_WATERFALLS),
	SCENE_EXIT(MERCURY_LIGHTHOUSE_LOWER_TO_MERCURY_LIGHTHOUSE_MIDDLE_SOUTH, MercuryLighthouseMiddle, MERCURY_LIGHTHOUSE_MIDDLE_FROM_MERCURY_LIGHTHOUSE_LOWER_SOUTH),
	SCENE_EXIT(MERCURY_LIGHTHOUSE_LOWER_TO_MERCURY_LIGHTHOUSE_ENTRANCE_PIPE, MercuryLighthouseEntrance, MERCURY_LIGHTHOUSE_ENTRANCE_FROM_MERCURY_LIGHTHOUSE_LOWER_PIPE),
	SCENE_EXITS(MercuryLighthouseMiddle),
	SCENE_EXIT(MERCURY_LIGHTHOUSE_MIDDLE_TO_MERCURY_LIGHTHOUSE_LOWER_NORTH, MercuryLighthouseLower, MERCURY_LIGHTHOUSE_LOWER_FROM_MERCURY_LIGHTHOUSE_MIDDLE_NORTH),
	SCENE_EXIT(MERCURY_LIGHTHOUSE_MIDDLE_GATE_TO_PIPES, MercuryLighthouseMiddle, MERCURY_LIGHTHOUSE_MIDDLE_PIPES_FROM_GATE),
	SCENE_EXIT(MERCURY_LIGHTHOUSE_MIDDLE_TO_MERCURY_LIGHTHOUSE_UPPER_WEST_NORTHWEST, MercuryLighthouseUpper, MERCURY_LIGHTHOUSE_UPPER_FROM_MERCURY_LIGHTHOUSE_MIDDLE_WEST_NORTHWEST),
	SCENE_EXIT(MERCURY_LIGHTHOUSE_MIDDLE_TO_MERCURY_LIGHTHOUSE_EXTERIOR, MercuryLighthouseExterior, MERCURY_LIGHTHOUSE_EXTERIOR_FROM_MERCURY_LIGHTHOUSE_MIDDLE),
	SCENE_EXIT(MERCURY_LIGHTHOUSE_MIDDLE_TO_MERCURY_LIGHTHOUSE_UPPER_NORTHWEST, MercuryLighthouseUpper, MERCURY_LIGHTHOUSE_UPPER_FROM_MERCURY_LIGHTHOUSE_MIDDLE_NORTHWEST),
	SCENE_EXIT(MERCURY_LIGHTHOUSE_MIDDLE_TO_MERCURY_LIGHTHOUSE_LOWER_SOUTH, MercuryLighthouseLower, MERCURY_LIGHTHOUSE_LOWER_FROM_MERCURY_LIGHTHOUSE_MIDDLE_SOUTH),
	SCENE_EXIT(MERCURY_LIGHTHOUSE_MIDDLE_PIPES_TO_GATE, MercuryLighthouseMiddle, MERCURY_LIGHTHOUSE_MIDDLE_GATE_FROM_PIPES),
	SCENE_EXIT(MERCURY_LIGHTHOUSE_MIDDLE_TO_MERCURY_LIGHTHOUSE_UPPER_NORTHEAST, MercuryLighthouseUpper, MERCURY_LIGHTHOUSE_UPPER_FROM_MERCURY_LIGHTHOUSE_MIDDLE_NORTHEAST),
	SCENE_EXIT(MERCURY_LIGHTHOUSE_MIDDLE_TO_MERCURY_LIGHTHOUSE_UPPER_EAST_NORTHEAST, MercuryLighthouseUpper, MERCURY_LIGHTHOUSE_UPPER_FROM_MERCURY_LIGHTHOUSE_MIDDLE_EAST_NORTHEAST),
	SCENE_EXIT(MERCURY_LIGHTHOUSE_MIDDLE_TO_MERCURY_LIGHTHOUSE_UPPER_NORTH_NORTHEAST, MercuryLighthouseUpper, MERCURY_LIGHTHOUSE_UPPER_FROM_MERCURY_LIGHTHOUSE_MIDDLE_NORTH_NORTHEAST),
	SCENE_EXITS(MercuryLighthouseUpper),
	SCENE_EXIT(MERCURY_LIGHTHOUSE_UPPER_TO_MERCURY_LIGHTHOUSE_MIDDLE_WEST_NORTHWEST, MercuryLighthouseMiddle, MERCURY_LIGHTHOUSE_MIDDLE_FROM_MERCURY_LIGHTHOUSE_UPPER_WEST_NORTHWEST),
	SCENE_EXIT(MERCURY_LIGHTHOUSE_UPPER_TO_MERCURY_LIGHTHOUSE_MIDDLE_NORTH_NORTHEAST, MercuryLighthouseMiddle, MERCURY_LIGHTHOUSE_MIDDLE_FROM_MERCURY_LIGHTHOUSE_UPPER_NORTH_NORTHEAST),
	SCENE_EXIT(MERCURY_LIGHTHOUSE_UPPER_TO_MERCURY_LIGHTHOUSE_MIDDLE_NORTHWEST, MercuryLighthouseMiddle, MERCURY_LIGHTHOUSE_MIDDLE_FROM_MERCURY_LIGHTHOUSE_UPPER_NORTHWEST),
	SCENE_EXIT(MERCURY_LIGHTHOUSE_UPPER_ROW_STATUES_TO_GATE, MercuryLighthouseUpper, MERCURY_LIGHTHOUSE_UPPER_GATE_FROM_ROW_STATUES),
	SCENE_EXIT(MERCURY_LIGHTHOUSE_UPPER_TO_MERCURY_LIGHTHOUSE_MIDDLE_NORTHEAST, MercuryLighthouseMiddle, MERCURY_LIGHTHOUSE_MIDDLE_FROM_MERCURY_LIGHTHOUSE_UPPER_NORTHEAST),
	SCENE_EXIT(MERCURY_LIGHTHOUSE_UPPER_TO_MERCURY_LIGHTHOUSE_MIDDLE_EAST_NORTHEAST, MercuryLighthouseMiddle, MERCURY_LIGHTHOUSE_MIDDLE_FROM_MERCURY_LIGHTHOUSE_UPPER_EAST_NORTHEAST),
	SCENE_EXIT(MERCURY_LIGHTHOUSE_UPPER_TO_MERCURY_LIGHTHOUSE_LOWER, MercuryLighthouseLower, MERCURY_LIGHTHOUSE_LOWER_FROM_MERCURY_LIGHTHOUSE_UPPER),
	SCENE_EXIT(MERCURY_LIGHTHOUSE_UPPER_GATE_TO_JETS, MercuryLighthouseUpper, MERCURY_LIGHTHOUSE_UPPER_JETS_FROM_GATE),
	SCENE_EXIT(MERCURY_LIGHTHOUSE_UPPER_GATE_TO_ROW_STATUES, MercuryLighthouseUpper, MERCURY_LIGHTHOUSE_UPPER_ROW_STATUES_FROM_GATE),
	SCENE_EXIT(MERCURY_LIGHTHOUSE_UPPER_TO_MERCURY_LIGHTHOUSE_EXTERIOR_EAST, MercuryLighthouseExterior, MERCURY_LIGHTHOUSE_EXTERIOR_FROM_MERCURY_LIGHTHOUSE_UPPER_EAST),
	SCENE_EXIT(MERCURY_LIGHTHOUSE_UPPER_GAP_TO_WATERFALLS, MercuryLighthouseUpper, MERCURY_LIGHTHOUSE_UPPER_WATERFALLS_FROM_GAP),
	SCENE_EXIT(MERCURY_LIGHTHOUSE_UPPER_TO_MERCURY_LIGHTHOUSE_EXTERIOR_WEST, MercuryLighthouseExterior, MERCURY_LIGHTHOUSE_EXTERIOR_FROM_MERCURY_LIGHTHOUSE_UPPER_WEST),
	SCENE_EXIT(MERCURY_LIGHTHOUSE_UPPER_WATERFALLS_TO_GAP, MercuryLighthouseUpper, MERCURY_LIGHTHOUSE_UPPER_GAP_FROM_WATERFALLS),
	SCENE_EXIT(MERCURY_LIGHTHOUSE_UPPER_JETS_TO_GATE, MercuryLighthouseUpper, MERCURY_LIGHTHOUSE_UPPER_GATE_FROM_JETS),
	SCENE_EXIT(MERCURY_LIGHTHOUSE_UPPER_JETS_TO_GATE_STATUE_FALLS, MercuryLighthouseUpper, MERCURY_LIGHTHOUSE_UPPER_GATE_FROM_JETS_STATUE_FALLS),
	SCENE_EXIT(MERCURY_LIGHTHOUSE_UPPER_GATE_TO_JETS_STATUE_FALLS, MercuryLighthouseUpper, MERCURY_LIGHTHOUSE_UPPER_JETS_FROM_GATE_STATUE_FALLS),
	SCENE_EXITS(MercuryLighthouseExterior),
	SCENE_EXIT(MERCURY_LIGHTHOUSE_EXTERIOR_TO_MERCURY_LIGHTHOUSE_MIDDLE, MercuryLighthouseMiddle, MERCURY_LIGHTHOUSE_MIDDLE_FROM_MERCURY_LIGHTHOUSE_EXTERIOR),
	SCENE_EXIT(MERCURY_LIGHTHOUSE_EXTERIOR_TO_MERCURY_LIGHTHOUSE_UPPER_WEST, MercuryLighthouseUpper, MERCURY_LIGHTHOUSE_UPPER_FROM_MERCURY_LIGHTHOUSE_EXTERIOR_WEST),
	SCENE_EXIT(MERCURY_LIGHTHOUSE_EXTERIOR_TO_MERCURY_LIGHTHOUSE_UPPER_EAST, MercuryLighthouseUpper, MERCURY_LIGHTHOUSE_UPPER_FROM_MERCURY_LIGHTHOUSE_EXTERIOR_EAST),
	SCENE_EXIT(MERCURY_LIGHTHOUSE_EXTERIOR_UPPER_PATH_TO_WATERFALL, MercuryLighthouseExterior, MERCURY_LIGHTHOUSE_EXTERIOR_WATERFALL_FROM_UPPER_PATH),
	SCENE_EXIT(MERCURY_LIGHTHOUSE_EXTERIOR_WATERFALL_TO_UPPER_PATH, MercuryLighthouseExterior, MERCURY_LIGHTHOUSE_EXTERIOR_UPPER_PATH_FROM_WATERFALL),
	SCENE_EXIT(MERCURY_LIGHTHOUSE_EXTERIOR_TO_WORLD_MAP_EAST, WorldMap, WORLD_MAP_FROM_MERCURY_LIGHTHOUSE_EXTERIOR_EAST),
	SCENE_EXIT(MERCURY_LIGHTHOUSE_EXTERIOR_TO_MERCURY_LIGHTHOUSE_ENTRANCE, MercuryLighthouseEntrance, MERCURY_LIGHTHOUSE_ENTRANCE_FROM_MERCURY_LIGHTHOUSE_EXTERIOR),
	SCENE_EXIT(MERCURY_LIGHTHOUSE_EXTERIOR_TO_WORLD_MAP_LIGHTHOUSE_LIT, WorldMap, WORLD_MAP_AFTER_MERCURY_LIGHTHOUSE_LIT),
	SCENE_EXIT(MERCURY_LIGHTHOUSE_EXTERIOR_TO_MERCURY_LIGHTHOUSE_AERIE, MercuryLighthouseAerie, MERCURY_LIGHTHOUSE_AERIE_FROM_MERCURY_LIGHTHOUSE_EXTERIOR),
	SCENE_EXITS_END,
};

ACTOR_PLACE MercuryLighthouseNoActorPlacements[] = {
	{ SCENE_TABLE_END },
};

ACTOR_PLACE MercuryLighthouseLowerActorPlacements[] = {
	ACTOR_PLACEMENT(ACTOR_MIA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		8, 0, 8, FACING_NORTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_BLUE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		792, 0, 632, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_PIPE_WEST_EAST, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
		672, 0, 176, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_PIPE_WEST_EAST, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
		704, 0, 272, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_PIPE_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
		744, 0, 240, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_PIPE_END, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
		648, 0, 299, FACING_SOUTH, TALK_FACING_KEEP, 0),
	{ SCENE_TABLE_END },
};

ACTOR_PLACE MercuryLighthouseMiddleActorPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_PIPE_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
		152, 0, 528, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_PIPE_END, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
		184, 0, 587, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_PIPE_WEST_EAST, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
		704, 0, 672, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_BLUE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		216, 0, 216, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_MERCURY_DJINNI, FLAG_DJINNI(DJINNI_SLEET), MOVEMENT_TYPE_STAND,
		888, 0, 392, FACING_SOUTH, TALK_FACING_PARTY, 0),
	{ SCENE_TABLE_END },
};

ACTOR_PLACE MercuryLighthouseUpperActorPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_BLUE_STATUE, IF_FLAG_SET(FLAG_MERCURY_LIGHTHOUSE_STATUE_FELL), MOVEMENT_TYPE_STAND,
		200, 0, 360, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_BLUE_STATUE, FLAG_MERCURY_LIGHTHOUSE_STATUE_FELL, MOVEMENT_TYPE_STAND,
		808, 0, 712, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_PIPE_WEST_EAST, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
		816, 0, 544, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_PIPE_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
		872, 0, 576, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_PIPE_END, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
		760, 0, 635, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_PIPE_END, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
		776, 0, 635, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_PIPE_END, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
		872, 0, 635, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_STONE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		648, 0, 88, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_STONE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		712, 0, 88, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_STONE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		776, 0, 88, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_STONE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		840, 0, 88, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_STONE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		936, 0, 88, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_STONE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		472, 0, 328, FACING_SOUTH, TALK_FACING_KEEP, 0),
	{ SCENE_TABLE_END },
};

ACTOR_PLACE MercuryLighthouseExteriorActorPlacements[] = {
	ACTOR_PLACEMENT(ACTOR_MIA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		840, 0, 664, FACING_NORTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(ACTOR_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		8, 0, 8, FACING_NORTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(ACTOR_IVAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		8, 0, 8, FACING_NORTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_BLUE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		792, 0, 872, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_BLUE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		888, 0, 872, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_STONE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		840, 0, 600, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_BLANK, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
		8, 0, 8, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_BLUE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		856, 0, 216, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_BLUE_STATUE, IF_FLAG_SET(FLAG_SATUROS_BEATEN_AT_MERCURY), MOVEMENT_TYPE_STAND,
		8, 0, 8, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, IF_FLAG_SET(FLAG_SATUROS_BEATEN_AT_MERCURY), MOVEMENT_TYPE_STAND,
		824, 0, 696, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_FAIR_HAIR, IF_FLAG_SET(FLAG_SATUROS_BEATEN_AT_MERCURY), MOVEMENT_TYPE_WANDER,
		760, 0, 744, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_OLD_MAN_WITH_CANE, IF_FLAG_SET(FLAG_SATUROS_BEATEN_AT_MERCURY), MOVEMENT_TYPE_WANDER,
		920, 0, 744, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_OLD_WOMAN, IF_FLAG_SET(FLAG_SATUROS_BEATEN_AT_MERCURY), MOVEMENT_TYPE_STAND,
		856, 0, 792, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY, 0),
	{ SCENE_TABLE_END },
};

CUE MercuryLighthouseLowerCues[] = {
	{ CUE_EXIT, EXIT_METATILE_BEHAVIOR_OFFSET + MERCURY_LIGHTHOUSE_LOWER_TO_MERCURY_LIGHTHOUSE_ENTRANCE_WEST_NORTHWEST, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_LOWER_TO_MERCURY_LIGHTHOUSE_ENTRANCE_WEST_NORTHWEST },
	{ CUE_EXIT, EXIT_METATILE_BEHAVIOR_OFFSET + MERCURY_LIGHTHOUSE_LOWER_TO_MERCURY_LIGHTHOUSE_ENTRANCE_WEST, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_LOWER_TO_MERCURY_LIGHTHOUSE_ENTRANCE_WEST },
	{ CUE_EXIT | EXIT_STYLE_STAIRS_UP, EXIT_METATILE_BEHAVIOR_OFFSET + MERCURY_LIGHTHOUSE_LOWER_POOL_TO_STATUE, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_LOWER_POOL_TO_STATUE },
	{ CUE_EXIT | EXIT_STYLE_STAIRS_DOWN, EXIT_METATILE_BEHAVIOR_OFFSET + MERCURY_LIGHTHOUSE_LOWER_STATUE_TO_POOL, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_LOWER_STATUE_TO_POOL },
	{ CUE_EXIT, EXIT_METATILE_BEHAVIOR_OFFSET + MERCURY_LIGHTHOUSE_LOWER_STATUE_TO_WATERFALLS, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_LOWER_STATUE_TO_WATERFALLS },
	{ CUE_EXIT, EXIT_METATILE_BEHAVIOR_OFFSET + MERCURY_LIGHTHOUSE_LOWER_TO_MERCURY_LIGHTHOUSE_ENTRANCE_EAST_NORTHEAST, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_LOWER_TO_MERCURY_LIGHTHOUSE_ENTRANCE_EAST_NORTHEAST },
	{ CUE_EXIT, EXIT_METATILE_BEHAVIOR_OFFSET + MERCURY_LIGHTHOUSE_LOWER_WATERFALLS_TO_STATUE, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_LOWER_WATERFALLS_TO_STATUE },
	{ CUE_EXIT, EXIT_METATILE_BEHAVIOR_OFFSET + MERCURY_LIGHTHOUSE_LOWER_TO_MERCURY_LIGHTHOUSE_MIDDLE_SOUTH, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_LOWER_TO_MERCURY_LIGHTHOUSE_MIDDLE_SOUTH },
	{ CUE_EXIT, EXIT_METATILE_BEHAVIOR_OFFSET + MERCURY_LIGHTHOUSE_LOWER_TO_MERCURY_LIGHTHOUSE_MIDDLE_NORTH, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_LOWER_TO_MERCURY_LIGHTHOUSE_MIDDLE_NORTH },
	{ CUE_EXIT | EXIT_STYLE_STAIRS_DOWN, EXIT_METATILE_BEHAVIOR_OFFSET + MERCURY_LIGHTHOUSE_LOWER_TO_MERCURY_LIGHTHOUSE_ENTRANCE_NORTHEAST, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_LOWER_TO_MERCURY_LIGHTHOUSE_ENTRANCE_NORTHEAST },
	{ CUE_EXIT | EXIT_STYLE_STAIRS_UP, EXIT_METATILE_BEHAVIOR_OFFSET + MERCURY_LIGHTHOUSE_LOWER_TO_MERCURY_LIGHTHOUSE_UPPER, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_LOWER_TO_MERCURY_LIGHTHOUSE_UPPER },
	{ CUE_STEP, METATILE_BEHAVIOR_SLEEP_BOMB_LEDGE, IF_FLAG_SET(FLAG_MERCURY_LIGHTHOUSE_HEART_HONORED), SCRIPT_REF(scMercuryLighthouseLoseLightsOnLower) },
	{ STEP_CUE_WALKING, METATILE_BEHAVIOR_MERCURY_LIGHTHOUSE_POOL_STEP, IF_FLAG_SET(FLAG_MERCURY_LIGHTHOUSE_HEART_HONORED), SCRIPT_REF(scMercuryLighthouseHopAcrossPoolOnLower) },
	{ CUE_STEP, METATILE_BEHAVIOR_MERCURY_LIGHTHOUSE_POOL_STEP, IF_FLAG_SET(FLAG_MERCURY_LIGHTHOUSE_HEART_HONORED), SCRIPT_REF(scMercuryLighthouseLoseLightsOnLower) },
	{ STEP_CUE_WALKING | STEP_CUE_FLAG_QUICK, METATILE_BEHAVIOR_MERCURY_LIGHTHOUSE_POOL_WATER, IF_FLAG_SET(FLAG_MERCURY_LIGHTHOUSE_HEART_HONORED), SCRIPT_REF(scMercuryLighthouseHopAcrossPoolOnLower) },
	{ CUE_STEP, METATILE_BEHAVIOR_MERCURY_LIGHTHOUSE_POOL_WATER, IF_FLAG_SET(FLAG_MERCURY_LIGHTHOUSE_HEART_HONORED), SCRIPT_REF(scMercuryLighthouseRippleWhileInPool) },
	{ STEP_CUE_WALKING, METATILE_BEHAVIOR_MERCURY_LIGHTHOUSE_POOL_HEART, IF_FLAG_SET(FLAG_MERCURY_LIGHTHOUSE_HEART_HONORED), SCRIPT_REF(scMercuryLighthouseHopAcrossPoolOnLower) },
	{ CUE_STEP, METATILE_BEHAVIOR_MERCURY_LIGHTHOUSE_POOL_HEART, IF_FLAG_SET(FLAG_MERCURY_LIGHTHOUSE_HEART_HONORED), SCRIPT_REF(scMercuryLighthouseChargeLightsOnLower) },
	{ STEP_CUE_WALKING_INTO(FACING_SOUTH), METATILE_BEHAVIOR_BY_CRYSTAL, CONDITION_ALWAYS, SCRIPT_REF(scMercuryLighthouseJumpDownPastCrystal) },
	{ STEP_CUE_WALKING, METATILE_BEHAVIOR_CHANNEL_PIPE_PUSH, CONDITION_ALWAYS, SCRIPT_REF(scMercuryLighthousePushChannelPipes) },
	{ CUE_TALK, ACTOR_MIA, FLAG_MIA_JOINED_PARTY, MSGNO(MsgMercuryLighthouseThePathIsBlockedAgainWhat) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MIA, FLAG_MIA_JOINED_PARTY, MSGNO(MsgMercuryLighthouseThisStatueWasntHereBeforeI) },
	{ PSYNERGY_ANYWHERE(ABILITY_RETREAT, PSYNERGY_PHASE_BEFORE_CAST), 0, CONDITION_ALWAYS, SCRIPT_REF(scMercuryLighthouseRemoveColumnOnRetreat) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_STATUE_BLOCKING_MIA, FLAG_MIA_JOINED_PARTY, SCRIPT_REF(scMercuryLighthouseMiaJoinsAfterRescue) },
	{ EXAMINE_CUE(EXAMINE_CHEST), 100, FLAG_CHEST_SLEEP_BOMB, FIND_ITEM(ITEM_SLEEP_BOMB) },
	{ CUE_TABLE_END },
};

CUE MercuryLighthouseMiddleCues[] = {
	{ CUE_EXIT, EXIT_METATILE_BEHAVIOR_OFFSET + MERCURY_LIGHTHOUSE_MIDDLE_GATE_TO_PIPES, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_MIDDLE_GATE_TO_PIPES },
	{ CUE_EXIT, EXIT_METATILE_BEHAVIOR_OFFSET + MERCURY_LIGHTHOUSE_MIDDLE_TO_MERCURY_LIGHTHOUSE_LOWER_NORTH, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_MIDDLE_TO_MERCURY_LIGHTHOUSE_LOWER_NORTH },
	{ CUE_EXIT, EXIT_METATILE_BEHAVIOR_OFFSET + MERCURY_LIGHTHOUSE_MIDDLE_TO_MERCURY_LIGHTHOUSE_LOWER_SOUTH, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_MIDDLE_TO_MERCURY_LIGHTHOUSE_LOWER_SOUTH },
	{ CUE_EXIT, EXIT_METATILE_BEHAVIOR_OFFSET + MERCURY_LIGHTHOUSE_MIDDLE_PIPES_TO_GATE, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_MIDDLE_PIPES_TO_GATE },
	{ CUE_EXIT, EXIT_METATILE_BEHAVIOR_OFFSET + MERCURY_LIGHTHOUSE_MIDDLE_TO_MERCURY_LIGHTHOUSE_UPPER_WEST_NORTHWEST, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_MIDDLE_TO_MERCURY_LIGHTHOUSE_UPPER_WEST_NORTHWEST },
	{ CUE_EXIT | EXIT_STYLE_STAIRS_UP, EXIT_METATILE_BEHAVIOR_OFFSET + MERCURY_LIGHTHOUSE_MIDDLE_TO_MERCURY_LIGHTHOUSE_EXTERIOR, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_MIDDLE_TO_MERCURY_LIGHTHOUSE_EXTERIOR },
	{ CUE_EXIT, EXIT_METATILE_BEHAVIOR_OFFSET + MERCURY_LIGHTHOUSE_MIDDLE_TO_MERCURY_LIGHTHOUSE_UPPER_NORTHWEST, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_MIDDLE_TO_MERCURY_LIGHTHOUSE_UPPER_NORTHWEST },
	{ CUE_EXIT, EXIT_METATILE_BEHAVIOR_OFFSET + MERCURY_LIGHTHOUSE_MIDDLE_TO_MERCURY_LIGHTHOUSE_UPPER_NORTHEAST, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_MIDDLE_TO_MERCURY_LIGHTHOUSE_UPPER_NORTHEAST },
	{ CUE_EXIT, EXIT_METATILE_BEHAVIOR_OFFSET + MERCURY_LIGHTHOUSE_MIDDLE_TO_MERCURY_LIGHTHOUSE_UPPER_EAST_NORTHEAST, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_MIDDLE_TO_MERCURY_LIGHTHOUSE_UPPER_EAST_NORTHEAST },
	{ CUE_EXIT, EXIT_METATILE_BEHAVIOR_OFFSET + MERCURY_LIGHTHOUSE_MIDDLE_TO_MERCURY_LIGHTHOUSE_UPPER_NORTH_NORTHEAST, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_MIDDLE_TO_MERCURY_LIGHTHOUSE_UPPER_NORTH_NORTHEAST },
	{ STEP_CUE_WALKING_INTO(FACING_SOUTH), METATILE_BEHAVIOR_BY_CRYSTAL, CONDITION_ALWAYS, SCRIPT_REF(scMercuryLighthouseJumpDownPastCrystal) },
	{ STEP_CUE_WALKING, METATILE_BEHAVIOR_SPOUT_PIPE_PUSH, CONDITION_ALWAYS, SCRIPT_REF(scMercuryLighthousePushSpoutAndNorthJetPipes) },
	{ CUE_STEP, METATILE_BEHAVIOR_EAST_SPOUT, IF_FLAG_SET(FLAG_SPOUTS_RUNNING), SCRIPT_REF(scMercuryLighthousePushLeaderOutOfSpout) },
	{ CUE_STEP, METATILE_BEHAVIOR_SOUTH_SPOUT, IF_FLAG_SET(FLAG_SPOUTS_RUNNING), SCRIPT_REF(scMercuryLighthousePushLeaderOutOfSpout) },
	{ CUE_STEP, METATILE_BEHAVIOR_WEST_SPOUT, IF_FLAG_SET(FLAG_SPOUTS_RUNNING), SCRIPT_REF(scMercuryLighthousePushLeaderOutOfSpout) },
	{ STEP_CUE_WALKING, METATILE_BEHAVIOR_GATE_ROOM_FLOOR, FLAG_MERCURY_LIGHTHOUSE_GATE_HELD_OPEN, SCRIPT_REF(scMercuryLighthousePushGateSwitchStatue) },
	{ CUE_STEP, METATILE_BEHAVIOR_GATE_FLOOR_SWITCH, FLAG_MERCURY_LIGHTHOUSE_GATE_HELD_OPEN, SCRIPT_REF(scMercuryLighthousePressGateFloorSwitch) },
	{ CUE_STEP, METATILE_BEHAVIOR_GATE_ROOM_FLOOR, FLAG_MERCURY_LIGHTHOUSE_GATE_HELD_OPEN, SCRIPT_REF(scMercuryLighthouseReleaseGateFloorSwitch) },
	{ CUE_STEP, METATILE_BEHAVIOR_BESIDE_GATE_FLOOR_SWITCH, FLAG_MERCURY_LIGHTHOUSE_GATE_HELD_OPEN, SCRIPT_REF(scMercuryLighthouseReleaseGateFloorSwitch) },
	{ STEP_CUE_WALKING, METATILE_BEHAVIOR_NORTH_JET_PIPE_PUSH, CONDITION_ALWAYS, SCRIPT_REF(scMercuryLighthousePushSpoutAndNorthJetPipes) },
	{ CUE_STEP, METATILE_BEHAVIOR_NORTH_JET, CONDITION_ALWAYS, SCRIPT_REF(scMercuryLighthousePushLeaderOutOfSpout) },
	{ CUE_TALK, ACTOR_SLEET, CONDITION_ALWAYS, SCRIPT_REF(scMercuryLighthouseStartDjinnBattle) },
	{ EXAMINE_CUE(EXAMINE_SILENT), METATILE_BEHAVIOR_SEALED_GATE, CONDITION_ALWAYS, SCRIPT_REF(scMercuryLighthouseInspectLockedDoor) },
	{ PSYNERGY_ANYWHERE(ABILITY_RETREAT, PSYNERGY_PHASE_BEFORE_CAST), 0, CONDITION_ALWAYS, SCRIPT_REF(scMercuryLighthouseRemoveColumnOnRetreat) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_GATE_STATUE, FLAG_MERCURY_LIGHTHOUSE_GATE_HELD_OPEN, SCRIPT_REF(scMercuryLighthouseOpenGateWithMovedStatue) },
	{ EXAMINE_CUE(EXAMINE_CHEST), 101, FLAG_CHEST_PSY_CRYSTAL, FIND_ITEM(ITEM_PSY_CRYSTAL) },
	{ EXAMINE_CUE(EXAMINE_CHEST), 102, FLAG_CHEST_NUT, FIND_ITEM(ITEM_NUT) },
	{ CUE_TABLE_END },
};

CUE MercuryLighthouseUpperCues[] = {
	{ CUE_EXIT, EXIT_METATILE_BEHAVIOR_OFFSET + MERCURY_LIGHTHOUSE_UPPER_TO_MERCURY_LIGHTHOUSE_MIDDLE_WEST_NORTHWEST, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_UPPER_TO_MERCURY_LIGHTHOUSE_MIDDLE_WEST_NORTHWEST },
	{ CUE_EXIT, EXIT_METATILE_BEHAVIOR_OFFSET + MERCURY_LIGHTHOUSE_UPPER_TO_MERCURY_LIGHTHOUSE_MIDDLE_NORTH_NORTHEAST, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_UPPER_TO_MERCURY_LIGHTHOUSE_MIDDLE_NORTH_NORTHEAST },
	{ STEP_CUE_WALKING_INTO(FACING_NORTH), EXIT_METATILE_BEHAVIOR_OFFSET + MERCURY_LIGHTHOUSE_UPPER_TO_MERCURY_LIGHTHOUSE_MIDDLE_NORTHWEST, CONDITION_ALWAYS, SCRIPT_REF(scMercuryLighthouseWaterfallToSleetsRoom) },
	{ CUE_EXIT | EXIT_STYLE_STAIRS_DOWN, EXIT_METATILE_BEHAVIOR_OFFSET + MERCURY_LIGHTHOUSE_UPPER_TO_MERCURY_LIGHTHOUSE_LOWER, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_UPPER_TO_MERCURY_LIGHTHOUSE_LOWER },
	{ CUE_EXIT, EXIT_METATILE_BEHAVIOR_OFFSET + MERCURY_LIGHTHOUSE_UPPER_GATE_TO_ROW_STATUES, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_UPPER_GATE_TO_ROW_STATUES },
	{ CUE_EXIT | EXIT_STYLE_STAIRS_UP, EXIT_METATILE_BEHAVIOR_OFFSET + MERCURY_LIGHTHOUSE_UPPER_GATE_TO_JETS, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_UPPER_GATE_TO_JETS },
	{ CUE_EXIT, EXIT_METATILE_BEHAVIOR_OFFSET + MERCURY_LIGHTHOUSE_UPPER_GAP_TO_WATERFALLS, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_UPPER_GAP_TO_WATERFALLS },
	{ CUE_EXIT | EXIT_STYLE_STAIRS_UP, EXIT_METATILE_BEHAVIOR_OFFSET + MERCURY_LIGHTHOUSE_UPPER_TO_MERCURY_LIGHTHOUSE_EXTERIOR_EAST, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_UPPER_TO_MERCURY_LIGHTHOUSE_EXTERIOR_EAST },
	{ CUE_EXIT, EXIT_METATILE_BEHAVIOR_OFFSET + MERCURY_LIGHTHOUSE_UPPER_TO_MERCURY_LIGHTHOUSE_EXTERIOR_WEST, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_UPPER_TO_MERCURY_LIGHTHOUSE_EXTERIOR_WEST },
	{ STEP_CUE_WALKING_INTO(FACING_NORTH), EXIT_METATILE_BEHAVIOR_OFFSET + MERCURY_LIGHTHOUSE_UPPER_WATERFALLS_TO_GAP, CONDITION_ALWAYS, SCRIPT_REF(scMercuryLighthouseWaterfallToGapRoom) },
	{ CUE_EXIT, EXIT_METATILE_BEHAVIOR_OFFSET + MERCURY_LIGHTHOUSE_UPPER_ROW_STATUES_TO_GATE, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_UPPER_ROW_STATUES_TO_GATE },
	{ CUE_EXIT, EXIT_METATILE_BEHAVIOR_OFFSET + MERCURY_LIGHTHOUSE_UPPER_TO_MERCURY_LIGHTHOUSE_MIDDLE_NORTHEAST, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_UPPER_TO_MERCURY_LIGHTHOUSE_MIDDLE_NORTHEAST },
	{ CUE_EXIT, EXIT_METATILE_BEHAVIOR_OFFSET + MERCURY_LIGHTHOUSE_UPPER_TO_MERCURY_LIGHTHOUSE_MIDDLE_EAST_NORTHEAST, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_UPPER_TO_MERCURY_LIGHTHOUSE_MIDDLE_EAST_NORTHEAST },
	{ CUE_EXIT | EXIT_STYLE_STAIRS_DOWN, EXIT_METATILE_BEHAVIOR_OFFSET + MERCURY_LIGHTHOUSE_UPPER_JETS_TO_GATE, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_UPPER_JETS_TO_GATE },
	{ STEP_CUE_WALKING, METATILE_BEHAVIOR_GATE_ROOM_FLOOR, FLAG_MERCURY_LIGHTHOUSE_UPPER_GATE_HELD_OPEN, SCRIPT_REF(scMercuryLighthousePushUpperGateSwitchStatue) },
	{ STEP_CUE_WALKING, METATILE_BEHAVIOR_GATE_FLOOR_SWITCH, FLAG_MERCURY_LIGHTHOUSE_UPPER_GATE_HELD_OPEN, SCRIPT_REF(scMercuryLighthousePushUpperGateSwitchStatue) },
	{ CUE_STEP, METATILE_BEHAVIOR_GATE_FLOOR_SWITCH, FLAG_MERCURY_LIGHTHOUSE_UPPER_GATE_HELD_OPEN, SCRIPT_REF(scMercuryLighthousePressUpperGateFloorSwitch) },
	{ CUE_STEP, METATILE_BEHAVIOR_GATE_ROOM_FLOOR, FLAG_MERCURY_LIGHTHOUSE_UPPER_GATE_HELD_OPEN, SCRIPT_REF(scMercuryLighthouseReleaseUpperGateFloorSwitch) },
	{ STEP_CUE_WALKING, METATILE_BEHAVIOR_JET_ROOM_PIPE_PUSH, CONDITION_ALWAYS, SCRIPT_REF(scMercuryLighthousePushJetRoomPipes) },
	{ STEP_CUE_WALKING, METATILE_BEHAVIOR_ROW_STATUE_PUSH, CONDITION_ALWAYS, SCRIPT_REF(scMercuryLighthousePushRowStatue) },
	{ EXAMINE_CUE(EXAMINE_SILENT), METATILE_BEHAVIOR_SEALED_GATE, CONDITION_ALWAYS, SCRIPT_REF(scMercuryLighthouseInspectLockedDoor) },
	{ PSYNERGY_ANYWHERE(ABILITY_RETREAT, PSYNERGY_PHASE_BEFORE_CAST), 0, CONDITION_ALWAYS, SCRIPT_REF(scMercuryLighthouseRemoveColumnOnRetreat) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FALLEN_STATUE, FLAG_MERCURY_LIGHTHOUSE_UPPER_GATE_HELD_OPEN, SCRIPT_REF(scMercuryLighthouseOpenUpperGateWithMovedStatue) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_ROW_STATUE, CONDITION_ALWAYS, SCRIPT_REF(scMercuryLighthouseRecordRowStatues) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_ROW_STATUE + 1, CONDITION_ALWAYS, SCRIPT_REF(scMercuryLighthouseRecordRowStatues) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_ROW_STATUE + 2, CONDITION_ALWAYS, SCRIPT_REF(scMercuryLighthouseRecordRowStatues) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_ROW_STATUE + 3, CONDITION_ALWAYS, SCRIPT_REF(scMercuryLighthouseRecordRowStatues) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_LAST_ROW_STATUE, CONDITION_ALWAYS, SCRIPT_REF(scMercuryLighthouseRecordRowStatues) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_GAP_STATUE, CONDITION_ALWAYS, SCRIPT_REF(scMercuryLighthouseMoveGapStatueOffDoorway) },
	{ CUE_TABLE_END },
};

CUE MercuryLighthouseExteriorCues[] = {
	{ CUE_EXIT, METATILE_BEHAVIOR_LIGHTHOUSE_DOOR, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_EXTERIOR_TO_MERCURY_LIGHTHOUSE_ENTRANCE },
	{ CUE_EXIT, METATILE_BEHAVIOR_CAUSEWAY_FOOT, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_EXTERIOR_TO_WORLD_MAP_EAST },
	{ CUE_EXIT | EXIT_STYLE_STAIRS_DOWN, EXIT_METATILE_BEHAVIOR_OFFSET + MERCURY_LIGHTHOUSE_EXTERIOR_TO_MERCURY_LIGHTHOUSE_MIDDLE, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_EXTERIOR_TO_MERCURY_LIGHTHOUSE_MIDDLE },
	{ CUE_EXIT, EXIT_METATILE_BEHAVIOR_OFFSET + MERCURY_LIGHTHOUSE_EXTERIOR_TO_MERCURY_LIGHTHOUSE_UPPER_WEST, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_EXTERIOR_TO_MERCURY_LIGHTHOUSE_UPPER_WEST },
	{ CUE_EXIT | EXIT_STYLE_STAIRS_DOWN, EXIT_METATILE_BEHAVIOR_OFFSET + MERCURY_LIGHTHOUSE_EXTERIOR_TO_MERCURY_LIGHTHOUSE_UPPER_EAST, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_EXTERIOR_TO_MERCURY_LIGHTHOUSE_UPPER_EAST },
	{ CUE_EXIT, EXIT_METATILE_BEHAVIOR_OFFSET + MERCURY_LIGHTHOUSE_EXTERIOR_UPPER_PATH_TO_WATERFALL, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_EXTERIOR_UPPER_PATH_TO_WATERFALL },
	{ STEP_CUE_WALKING_INTO(FACING_NORTH), EXIT_METATILE_BEHAVIOR_OFFSET + MERCURY_LIGHTHOUSE_EXTERIOR_WATERFALL_TO_UPPER_PATH, CONDITION_ALWAYS, SCRIPT_REF(scMercuryLighthouseWaterfallToUpperPath) },
	{ CUE_TALK, ACTOR_MIA, FLAG_MIA_SENSED_SOMEONE_INSIDE, SCRIPT_REF(scMercuryLighthouseMiaSensesSomeoneInside) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), CUE_SUBJECT_OWN_EFFECT | ACTOR_MIA, FLAG_MIA_SENSED_SOMEONE_INSIDE, SCRIPT_REF(scMercuryLighthouseMiaSensesSomeoneInside) },
	{ CUE_TALK, ACTOR_MIA, FLAG_MERCURY_LIGHTHOUSE_OPENED_BY_MIA, SCRIPT_REF(scMercuryLighthouseMiaDiscussesBlockedEntrance) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MIA, FLAG_MERCURY_LIGHTHOUSE_OPENED_BY_MIA, MSGNO(MsgMercuryLighthouseSomeoneDoesntWantMeToGet) },
	{ CUE_TALK, ACTOR_FIRST_VISITOR, IF_FLAG_SET(FLAG_SATUROS_BEATEN_AT_MERCURY), MSGNO(MsgMercuryLighthouseTheLegendsSaidThatIfThe) },
	{ CUE_TALK, ACTOR_FIRST_VISITOR + 1, IF_FLAG_SET(FLAG_SATUROS_BEATEN_AT_MERCURY), MSGNO(MsgMercuryLighthouseOurFountainIsBackTheySay) },
	{ CUE_TALK, ACTOR_FIRST_VISITOR + 2, IF_FLAG_SET(FLAG_SATUROS_BEATEN_AT_MERCURY), MSGNO(MsgMercuryLighthouseIDrankTheHealingWaterAnd) },
	{ CUE_TALK, ACTOR_FIRST_VISITOR + 3, IF_FLAG_SET(FLAG_SATUROS_BEATEN_AT_MERCURY), MSGNO(MsgMercuryLighthouseWeDontHaveToFearAny) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FIRST_VISITOR, IF_FLAG_SET(FLAG_SATUROS_BEATEN_AT_MERCURY), MSGNO(MsgMercuryLighthouseHowDidTheFountainKnowThat) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FIRST_VISITOR + 1, IF_FLAG_SET(FLAG_SATUROS_BEATEN_AT_MERCURY), MSGNO(MsgMercuryLighthouseAllThatLivesThatMeansIt) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FIRST_VISITOR + 2, IF_FLAG_SET(FLAG_SATUROS_BEATEN_AT_MERCURY), MSGNO(MsgMercuryLighthouseIKnewItWouldWorkBut) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FIRST_VISITOR + 3, IF_FLAG_SET(FLAG_SATUROS_BEATEN_AT_MERCURY), MSGNO(MsgMercuryLighthouseINeverBelievedThoseStoriesBut) },
	{ EXAMINE_CUE(EXAMINE_SILENT), METATILE_BEHAVIOR_SEALED_DOOR, CONDITION_ALWAYS, SCRIPT_REF(scMercuryLighthouseInspectSealedDoor) },
	{ EXAMINE_CUE(EXAMINE_SILENT), METATILE_BEHAVIOR_FOUNTAIN_SIGN, CONDITION_ALWAYS, SCRIPT_REF(scMercuryLighthouseReadHermesFountainSign) },
	{ EXAMINE_CUE(EXAMINE_SILENT), METATILE_BEHAVIOR_HEALING_FOUNTAIN, FLAG_HERMES_WATER_TAKEN, SCRIPT_REF(scMercuryLighthouseInspectHealingFountain) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_DOOR_STATUE, FLAG_IMIL_MET_MIA, 0 },
	{ PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_DOOR_STATUE, FLAG_MERCURY_LIGHTHOUSE_OPENED_BY_MIA, SCRIPT_REF(scMercuryLighthouseMiaOpensEntranceWithPly) },
	{ PSYNERGY_ON_ACTOR(ABILITY_PLY), ACTOR_WATERFALL_STATUE, CONDITION_ALWAYS, SCRIPT_REF(scMercuryLighthousePlyOpensWaterfallPassage) },
	{ PSYNERGY_ANYWHERE(ABILITY_RETREAT, PSYNERGY_PHASE_BEFORE_CAST), 0, CONDITION_ALWAYS, SCRIPT_REF(scMercuryLighthouseRemoveColumnOnRetreat) },
	{ USE_ITEM(ITEM_EMPTY_BOTTLE), METATILE_BEHAVIOR_HEALING_FOUNTAIN, IF_FLAG_SET(FLAG_SATUROS_BEATEN_AT_MERCURY), SCRIPT_REF(scMercuryLighthouseFillHermesWaterBottle) },
	{ CUE_TABLE_END },
};
/* clang-format on */
