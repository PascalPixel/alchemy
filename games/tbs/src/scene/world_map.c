/* The World Map (SceneIdWorldMap), the one perspective map (map/world) that
   joins the areas, and the only scene of this overlay.

   Its code does three kinds of work.

   It lets the party into the areas. Each town, cave, dock and passage stands on
   the map as a small picture of its kind (a village, a cave mouth, a pier and
   the rest), turned as the map turns, and walking into the picture enters the
   place. A few ways in are stretches of ground instead, such as the deserts'
   edges and the paths up to Venus Lighthouse.

   scWorldMapUpdateAreaContacts runs every tick. It shows only the pictures
   near the view and, when the leader touches one, raises that area's
   STEP_CUE_AREA, which the cue table turns into the area's exit.

   Five areas have two ways in, and the party comes in by the side it walks up
   from: the Bilibin Barricade from the west or the east, and Kolima Bridge,
   Kalay Docks, Gondowan Passage and Lalivero from the north or the south.

   Kolima Bridge's picture shows the drawbridge let down once it has been
   lowered (FLAG_KOLIMA_BRIDGE_LOWERED).

   It holds what the party meets on the map itself. Flint, the first Djinni, is
   a small spark circling a spot just outside Vale. Three more wait unseen: Vine
   on a small island north-west of the Lamakan Desert's way out, Corona on a
   lone island far north of Xian, and Hail in the fields far west of Tolbi.
   Walking over their ground once a random battle is nearly due may bring the
   Djinni's battle instead.

   A whirlwind spins near Crossbone Isle (ACTOR_STORM), where the red storm in
   Suhalla Desert drops a party it carries off. It is put away while the
   Tolbi-bound Ship lies at anchor off the island. Walked into, it lifts the
   party into the air for three seconds, as Suhalla Desert's storms do. Douse on
   it brings the party down to fight the Tempest Lizard inside. If the time runs
   out it carries the party off and drops them outside Suhalla.

   The Turtle Boots are buried past the Bilibin Barricade, in the forest at the
   end of a long, narrow island, and are found by checking the ground there.

   And it plays the story's moments on the map, each run by scInit for the
   entrance another scene sends the party to. In story order:
   - Leaving Vale (WORLD_MAP_FROM_VALE_PLAZA) sets FLAG_VALE_PARTY_LEFT the
	 first time.
   - Flint, the first Djinni, circles his spot until the party walks up to him
	 (scWorldMapMeetFlint). He joins and teaches how Djinn are set and put on
	 standby, and a lesson battle follows.
   - Hammet's caravan. It plays the first time the party touches Vault
	 (WORLD_MAP_HAMMETS_CARAVAN_LEAVES). The caravan, cut off from Kalay by the
	 destroyed bridge, turns north for Lunpa as the ground shakes and a rock
	 falls beside it. Then the party goes on into Vault. It sets
	 FLAG_LUNPA_GUARDS_SUSPECT_KALAY.
   - Mercury Lighthouse lit (WORLD_MAP_AFTER_MERCURY_LIGHTHOUSE_LIT). The
	 lighthouse transition plays (scWorldMapPlayLighthouseTransition), then
	 Mercury Lighthouse Aerie follows
	 (MERCURY_LIGHTHOUSE_AERIE_AFTER_BEACON_LIT).
   - Bunza's wagon. Out of West Lunpa Cave (WORLD_MAP_FROM_WEST_LUNPA_CAVE)
	 Bunza, Hammet and the leader board it and it rolls off
	 (FLAG_BUNZAS_WAGON_LEFT). At the Kalay Tunnel Entrance
	 (WORLD_MAP_FROM_KALAY_TUNNEL_ENTRANCE_SOUTH) it stops and the leader steps
	 down.
   - The Tolbi-bound Ship's crossing of the Karagol Sea. Between the ship's
	 scenes (WORLD_MAP_FROM_SHIP_ROOMS_SAIL_TO_FIRST_ATTACK to
	 _SAIL_TO_LAND) the map shows the ship sailing west and sends the party
	 back aboard (FLAG_TOLBI_SHIP_ROWING). A rowing off course takes it north
	 to anchor off the island (WORLD_MAP_FROM_SHIP_ROOMS,
	 FLAG_TOLBI_SHIP_AT_ISLAND). There
	 METATILE_BEHAVIOR_WORLD_MAP_CROSSBONE_ISLE leads ashore and
	 METATILE_BEHAVIOR_WORLD_MAP_TOLBI_SHIP back aboard.
	 WORLD_MAP_FROM_TOLBI_SHIP_DECK_REACH_TOLBI ends the crossing at Tolbi
	 (FLAG_TOLBI_SHIP_REACHED_TOLBI).
   - The Black Orb. Out of Gateway Cave (WORLD_MAP_FROM_GATEWAY_CAVE) Iodem
	 sees Babi's ship scuttled off the coast and gives Isaac the orb to use on
	 it (FLAG_WORLD_MAP_BLACK_ORB_HANDED_OVER, FLAG_SAW_SHIP_AT_IDEJIMA). He
	 calls him back if he walks into the cave again. Used on the wreck, the
	 orb sparkles but nothing happens: Iodem decides it is not yet time,
	 takes it back and sends the party on to Babi Lighthouse
	 (FLAG_BLACK_ORB_RETURNED). A defeat meanwhile returns to
	 WORLD_MAP_AFTER_BLACK_ORB_DEFEAT, which discards the orb and leads on by
	 WORLD_MAP_TO_ROCK_ROOMS.
   - Venus Lighthouse lit (WORLD_MAP_AFTER_VENUS_LIGHTHOUSE_LIT). The
	 lighthouse transition plays, then the party is back at the Lighthouse
	 Aerie (VENUS_LIGHTHOUSE_AERIE_AFTER_BEACON_LIT). After the look back
	 (WORLD_MAP_BEFORE_SHEBAS_FALL) the lighthouse shakes in clouds of dust
	 before Sheba's fall (WORLD_MAP_TO_VENUS_LIGHTHOUSE_AERIE).
   - The ending. Once the cleared game is saved (scene/file_screen.c,
	 WORLD_MAP_FROM_FILE_SCREEN), the island Jenna and Kraden are adrift on
	 drifts east. WORLD_MAP_JENNA_AND_KRADEN_ADRIFT then shows Alex leading
	 them to Felix and Sheba before the Title returns.

   WORLD_MAP_FROM_DEBUG, the debug menus' entrance, turns encounters off. */

#include "ability_ids.h"
#include "item_ids.h"
#include "battle/battle.h"
#include "debug/debug.h"
#include "field/script_inline.h"
#include "field/field_effect.h"
#include "field/field_psynergy_state.h"
#include "field/perspective.h"
#include "graphics/graphics.h"
#include "scene/tolbi_ship.h"
#include "scene/walk_into_leader.h"
#include "scene/golden_ring.h"
#include "text/msg_ids.h"

Sint32				  scInit(void);
extern Sint32		  IodemActor;
extern Sint32		  CaravanFirstWagonArrives[];
extern Sint32		  CaravanFirstWagonDeparts[];
extern Sint32		  CaravanFirstWagonReturns[];
extern Sint32		  CaravanWagonHeadsNorth[];
extern Sint32		  CaravanSecondWagonArrives[];
extern Sint32		  CaravanSecondWagonDeparts[];
extern Sint32		  CaravanSecondWagonReturns[];
extern Sint32		  CaravanThirdWagonArrives[];
extern Sint32		  CaravanThirdWagonDeparts[];
extern Sint32		  CaravanThirdWagonReturns[];
extern Sint32		  CaravanFourthWagonArrives[];
extern Sint32		  CaravanFourthWagonDeparts[];
extern Sint32		  CaravanFourthWagonReturns[];
extern Sint32		  CaravanRockGuideScript[];
extern Sint32		  BlackOrbLeaderScript[];
extern Sint32		  EndingAlexWalk[];
extern Sint32		  EndingKradenWalk[];
extern Sint32		  EndingJennaWalk[];
extern Sint32		  QuakeCloudScript[];
extern Sint32		  CrossingShipSailsWest[];
extern Sint32		  CrossingShipDriftsNorth[];
extern Sint32		  IslandDriftsEast[];
extern SCENE_ENTRANCE WorldSceneEntrances[];
extern Uint32		  WorldMapExits[];
extern ACTOR_PLACE	  WorldMapActorPlacements[];
extern ACTOR_PLACE	  WorldMapActorPlacementsHammetsCaravan[];
extern ACTOR_PLACE	  WorldMapActorPlacementsFromWestLunpaCave[];
extern ACTOR_PLACE	  WorldMapActorPlacementsCrossingEnds[];
extern ACTOR_PLACE	  WorldMapActorPlacementsCrossing[];
extern ACTOR_PLACE	  WorldMapActorPlacementsFromKalayTunnelEntranceSouth[];
extern ACTOR_PLACE	  WorldMapActorPlacementsJennaAndKradenAdrift[];
extern ACTOR_PLACE	  WorldMapActorPlacementsFromClear[];
extern ACTOR_PLACE	  WorldMapActorPlacementsBeforeShebasFall[];
extern CUE			  WorldCues[];
extern Sint32		  IslandWakeScript[];

/* The falling rock of Hammet's caravan script has landed and shattered. */
#define FLAG_WORLD_MAP_FALLING_ROCK_LANDED	 FLAG_SCENE_TEMP_00
/* Iodem has handed Isaac the Black Orb and waits for it to be used on the wreck
   off Gateway Cave. */
#define FLAG_WORLD_MAP_BLACK_ORB_HANDED_OVER FLAG_SCENE_TEMP_34
/* The scene's ordinary actor placements (WorldMapActorPlacements) are in use
   rather than a script's. */
#define FLAG_WORLD_MAP_ORDINARY_ACTORS		 FLAG_SCENE_TEMP_35
/* The storm holds the party up in the air, where Douse can bring it down. */
#define FLAG_WORLD_MAP_CAUGHT_IN_STORM		 FLAG_SCENE_TEMP_F0
/* The storm is done with, or put away while the Tolbi-bound Ship lies off the
   island, and catches no one. */
#define FLAG_WORLD_MAP_STORM_GONE			 FLAG_SCENE_TEMP_F1
/* The first flag of a byte counting the ticks the storm keeps the party in the
   air. The byte is read and written with syFieldGameFlagGetByte and
   syFieldGameFlagSetByte. */
#define FLAG_WORLD_MAP_STORM_TICKS_LEFT		 FLAG_SCENE_TEMP_F8
/* The ticks the storm holds the party in the air before it drops them outside
   Suhalla. */
#define STORM_TICKS_IN_AIR					 180

static Sint32 scWorldMapAlignToMap(ACTOR *actor);
static void	  scWorldMapBoardBunzasWagon(void);
static void	  scWorldMapShowHammetsCaravan(void);
static void	  scWorldMapShakeCameraOverCaravan(void);
static void	  scWorldMapShakeCameraWhereRockFalls(void);
static void	  scWorldMapUpdateFallingRock(void);
static void	  scWorldMapSailToFirstMonsterAttack(void);
static void	  scWorldMapSailToSecondMonsterAttack(void);
static void	  scWorldMapSailToThirdMonsterAttack(void);
static void	  scWorldMapSailToKraken(void);
static void	  scWorldMapSailUntilLandSighted(void);
static void	  scWorldMapSailIntoTolbi(void);
static void	  scWorldMapStepDownFromBunzasWagon(void);
static void	  scWorldMapRunBlackOrbScript(void);
static void	  scWorldMapBlockGatewayCaveExit(Sint32 actorId);
static void	  scWorldMapIodemCallsIsaacBack(void);
static void	  scWorldMapShowEndingScript(void);
static void	  scWorldMapShakeVenusLighthouse(void);
static void	  scWorldMapUpdateLighthouseQuake(void);
static void	  scWorldMapSailTolbiShipToIsland(void);
static Sint32 scWorldMapGetRowingBalance(void);
static Sint32 scWorldMapGetSeatRowValue(Uint32 seat);
static void	  scWorldMapShowIslandDrifting(void);
static void	  scWorldMapUpdateStormTimer(void);
static void scWorldMapPlayLighthouseTransition(WORLD_MAP_LIGHTHOUSE lighthouse);

/* The actors scWorldMapUpdateAreaContacts looks at, the placed ones to the last
   slot, and the base of the metatile behaviors a touch raises
   (STEP_CUE_AREA). */
enum { CONTACT_LAST_ACTOR = 65, STEP_CUE_AREA_BASE = 100 };

/* The World Map's own actors, numbered in the order of WorldMapActorPlacements
   from ACTOR_FIRST_PLACED.

   First come the Djinn met on the map, then the areas, then the Tolbi-bound
   Ship, the storm and Iodem.

   Each area is named by the scene its exit leads into (WorldMapExits). Touching
   it raises its STEP_CUE_AREA (scWorldMapUpdateAreaContacts). */
typedef enum WORLD_MAP_ACTOR_ID {
	/* The Venus Djinni: Flint, circling his spot until the party meets him, and
	   later Vine. */
	ACTOR_VENUS_DJINNI = ACTOR_FIRST_PLACED,
	/* The Mars Djinni Corona. */
	ACTOR_CORONA,
	/* The Mercury Djinni Hail. */
	ACTOR_HAIL,
	/* Vale, whose exit leads to the town gate (SceneIdValePlaza). */
	ACTOR_TO_VALE_PLAZA,
	ACTOR_TO_VAULT,
	ACTOR_TO_ALTIN,
	ACTOR_TO_SUHALLA,
	ACTOR_TO_LUNPA,
	ACTOR_TO_KALAY_DOCKS,
	ACTOR_TO_TOLBI_DOCKS,
	ACTOR_TO_IMIL,
	ACTOR_TO_KOLIMA,
	ACTOR_TO_XIAN,
	ACTOR_TO_LAMA_TEMPLE,
	ACTOR_TO_FUCHIN_TEMPLE,
	ACTOR_TO_BILIBIN,
	ACTOR_TO_KALAY,
	ACTOR_TO_LALIVERO,
	ACTOR_TO_TOLBI,
	ACTOR_TO_MERCURY_LIGHTHOUSE,
	/* After two pictures no script answers: a mountain north of Vale
	   (SPRITE_WORLD_MAP_MOUNTAIN) and Venus Lighthouse, which is entered by the
	   metatiles around it. */
	ACTOR_TO_BILIBIN_BARRICADE = ACTOR_TO_MERCURY_LIGHTHOUSE + 3,
	ACTOR_TO_ROAD_TO_BABI_LIGHTHOUSE,
	ACTOR_TO_GOMA_CAVE_ENTRANCE,
	/* Goma Cave's way out, where a wooden bridge crosses the river
	   (SceneIdGomaCaveBridge). */
	ACTOR_TO_GOMA_CAVE_BRIDGE,
	ACTOR_TO_ALTMILLER_CAVE,
	ACTOR_TO_GONDOWAN_CAVE_NORTH,
	ACTOR_TO_GONDOWAN_CAVE_SOUTH,
	/* The way into Mogall Forest where the Killer Ape waits
	   (SceneIdMogallForestKillerApe). */
	ACTOR_TO_MOGALL_FOREST_KILLER_APE,
	ACTOR_TO_GATEWAY_CAVE,
	ACTOR_TO_BILIBIN_CAVE_ENTRANCE,
	ACTOR_TO_BILIBIN_CAVE,
	ACTOR_TO_WEST_LUNPA_CAVE,
	ACTOR_TO_ALTIN_PEAK,
	ACTOR_TO_LAMAKAN_DESERT_CAVE,
	ACTOR_TO_KOLIMA_FOREST,
	/* The way into Mogall Forest by its broken sign
	   (SceneIdMogallForestSign). */
	ACTOR_TO_MOGALL_FOREST_SIGN,
	ACTOR_TO_KALAY_TUNNEL_ENTRANCE,
	/* After three signposts (SPRITE_SIGNPOST). Unused: they stand at the map's
	   origin, out of the player's reach, and no code moves them. */
	ACTOR_TO_KOLIMA_BRIDGE = ACTOR_TO_KALAY_TUNNEL_ENTRANCE + 4,
	ACTOR_TO_GONDOWAN_PASSAGE,
	/* The Tolbi-bound Ship at anchor off Crossbone Isle, after the wreck of
	   Babi's ship (SPRITE_SHIPWRECK), which stands off Gateway Cave. */
	ACTOR_TOLBI_SHIP = ACTOR_TO_GONDOWAN_PASSAGE + 2,
	/* The storm that lifts the party, as the Suhalla Desert's do. */
	ACTOR_STORM,
	/* Placed by the coast once he has handed over the Black Orb. */
	ACTOR_IODEM
} WORLD_MAP_ACTOR_ID;

/* The metatile behavior raised by touching an area's actor. */
#define STEP_CUE_AREA(actorId) (STEP_CUE_AREA_BASE + (actorId))

/* The World Map's own metatile behaviors, which the map's metatiles carry.

   Each exit metatile carries the number of the exit it leads to, so the exit
   rows of WorldCues name their metatile behavior by that exit. */
typedef enum WORLD_MAP_METATILE_BEHAVIOR {
	/* the shore beside the anchored ship */
	METATILE_BEHAVIOR_WORLD_MAP_TOLBI_SHIP = 74,
	/* the island's shore, walked onto northward */
	METATILE_BEHAVIOR_WORLD_MAP_CROSSBONE_ISLE = 75,
	METATILE_BEHAVIOR_WORLD_MAP_FLINT = 90,	 /* the ground around Flint */
	METATILE_BEHAVIOR_WORLD_MAP_CORONA = 91, /* the ground around Corona */
	METATILE_BEHAVIOR_WORLD_MAP_VINE = 92,	 /* the ground around Vine */
	METATILE_BEHAVIOR_WORLD_MAP_HAIL = 93,	 /* the ground around Hail */
	METATILE_BEHAVIOR_WORLD_MAP_STORM = 95,	 /* the ground under the storm */
	/* the scuttled ship off Gateway Cave */
	METATILE_BEHAVIOR_WORLD_MAP_SHIPWRECK = 96,
	/* the ground where the Turtle Boots are buried */
	METATILE_BEHAVIOR_WORLD_MAP_TURTLE_BOOTS = 100
} WORLD_MAP_METATILE_BEHAVIOR;

/* Raised by scWorldMapUpdateStormTimer when the party's time in the air runs
   out. */
typedef enum WORLD_MAP_RAISED_CUE {
	RAISED_STORM_TIME_UP = 99
} WORLD_MAP_RAISED_CUE;

/* SPRITE_WORLD_MAP_BRIDGE's animation once Kolima Bridge's drawbridge is
   down. */
enum { WORLD_MAP_BRIDGE_ANIMATION_LOWERED = 2 };

/* The falling rock's animations (blue_crystal.png). It tumbles as it falls,
   then shatters where it lands. */
enum { FALLING_ROCK_ANIMATION_TUMBLE = 2, FALLING_ROCK_ANIMATION_SHATTER = 3 };

/* The actors of Hammet's caravan leaving Vault
   (WorldMapActorPlacementsHammetsCaravan, WORLD_MAP_HAMMETS_CARAVAN_LEAVES),
   numbered from ACTOR_FIRST_PLACED. Garet's row takes his own actor id. */
typedef enum WORLD_MAP_CARAVAN_ACTOR_ID {
	/* Hammet, in his sprite, as ACTOR_HAMMET is out of West Lunpa Cave. */
	ACTOR_CARAVAN_HAMMET = ACTOR_FIRST_PLACED,
	/* One of Hammet's men, bald and bare-chested. "Master Hammet! This is
	   terrible! The bridge has been destroyed..." */
	ACTOR_CARAVAN_MAN,
	/* The caravan's four wagons, in the sprite of Bunza's wagon. */
	ACTOR_CARAVAN_FIRST_WAGON,
	/* A blank actor (SPRITE_BLANK) that the falling rock is held above as it
	   drops (scWorldMapUpdateFallingRock). It grows from a tenth of its size as
	   it moves (CaravanRockGuideScript). */
	ACTOR_INVISIBLE_ROCK_GUIDE = ACTOR_CARAVAN_FIRST_WAGON + 4,
	/* Drawn as the rocks in sol_sanctum_eruption.c. It lands by the caravan and
	   shatters. */
	ACTOR_FALLING_ROCK
} WORLD_MAP_CARAVAN_ACTOR_ID;

/* The Tolbi-bound Ship, shown on its way across the Karagol Sea.

   It is the first actor of WorldMapActorPlacementsCrossingEnds and
   WorldMapActorPlacementsCrossing
   (WORLD_MAP_FROM_SHIP_ROOMS_SAIL_TO_FIRST_ATTACK to
   WORLD_MAP_FROM_TOLBI_SHIP_DECK_REACH_TOLBI and WORLD_MAP_FROM_SHIP_ROOMS). */
enum { ACTOR_CROSSING_SHIP = ACTOR_FIRST_PLACED };

/* Bunza's wagon at Kalay (WorldMapActorPlacementsFromKalayTunnelEntranceSouth),
   after Kalay's and the Kalay Tunnel Entrance's area actors. */
enum { ACTOR_KALAY_WAGON = ACTOR_FIRST_PLACED + 2 };

/* The one actor of WorldMapActorPlacementsFromClear, before the ending.

   It is the island Jenna and Kraden are adrift on ("How long will this island
   continue to drift?"). It drifts east trailing a wake
   (scWorldMapSpawnIslandWake) until the map leads into their script. */
enum { ACTOR_DRIFTING_ISLAND = ACTOR_FIRST_PLACED };

/* The ending's actors (WorldMapActorPlacementsJennaAndKradenAdrift), numbered
   from ACTOR_FIRST_PLACED after the rows of Jenna and Sheba, which take their
   own actor ids. */
typedef enum WORLD_MAP_ENDING_ACTOR_ID {
	/* Kraden, adrift with Jenna. */
	ACTOR_KRADEN = ACTOR_FIRST_PLACED,
	/* Alex, who comes to show them that Felix and Sheba are alive. "There is
	   something I wish to show you." */
	ACTOR_ALEX,
	/* Felix, in his sprite as at Mercury Lighthouse (SPRITE_FELIX,
	   mercury_lighthouse_aerie.c), shown with Sheba as Jenna and Kraden call
	   their names. */
	ACTOR_FELIX
} WORLD_MAP_ENDING_ACTOR_ID;

/* Animations the map plays by number beyond ACTOR_ANIMATION, from each sprite's
   animation table (graphics/sprite/<name>.tsv). */
enum {
	ANIM_JENNA_LIE = 19, /* lying on her back (jenna.tsv's animation list) */
	ANIM_KRADEN_LIE = 5	 /* lying on his back (kraden.tsv) */
};

/* The battles the map starts from the scripted encounter tables. */
enum {
	/* Flint's lesson battle (ENCOUNTER_ZONE_SCRIPTED_VALE_VAULT). */
	ENCOUNTER_SLOT_DJINN_LESSON = 6,
	/* The Tornado Lizard of Suhalla Desert's sandstorms
	   (ENCOUNTER_ZONE_SCRIPTED). */
	ENCOUNTER_SLOT_TORNADO_LIZARD = 5,
	/* The Tempest Lizard of its great red storm (ENCOUNTER_ZONE_SCRIPTED). */
	ENCOUNTER_SLOT_TEMPEST_LIZARD = 7
};

/* Walking over the small island north-west of the Lamakan Desert's way out,
   once a random battle is nearly due, may start a battle with the Venus Djinni
   Vine; nothing is seen of him beforehand.

   From nine tenths of the steps to the next battle on, each step there has an
   even chance of bringing Vine's battle; otherwise the next random battle is
   made due at once. The row for talking to him is never reached, as his actor
   stays at the map's corner once it has played Flint. */
static void scWorldMapMeetVine(void)
{
	FIELD_STATE *state = FieldStateWork;

	if(MainRoutineStateWork.encounterSteps >=
		state->encounterThreshold * 9 / 10) {
		if((Uint32)syRandomNext() < 32768) {
			fdRequestDjinnBattle(
				DJINN_NO_HOP | ACTOR_VENUS_DJINNI, DJINNI_VINE);
			state->encounterBias = 0;
		} else {
			MainRoutineStateWork.encounterSteps = state->encounterThreshold;
		}
	}
}

/* Walking over the lone island far north of Xian, once a random battle is
   nearly due, starts a battle with the Mars Djinni Corona; nothing is seen of
   him beforehand.

   His actor is placed at the map's corner, out of reach, so the row for talking
   to him is never reached. */
static void scWorldMapMeetCorona(void)
{
	MAIN_ROUTINE_STATE *routine = &MainRoutineStateWork;
	Sint32			   *steps = &routine->encounterSteps;
	FIELD_STATE		   *state = FieldStateWork;

	if(*steps >= state->encounterThreshold * 9 / 10) {
		fdRequestDjinnBattle(DJINN_NO_HOP | ACTOR_CORONA, DJINNI_CORONA);
		state->encounterBias = 0;
	}
}

/* Walking over the fields far west of Tolbi, once a random battle is nearly
   due, starts a battle with the Mercury Djinni Hail; nothing is seen of him
   beforehand.

   His actor is placed at the map's corner, out of reach, so the row for talking
   to him is never reached. */
static void scWorldMapMeetHail(void)
{
	MAIN_ROUTINE_STATE *routine = &MainRoutineStateWork;
	Sint32			   *steps = &routine->encounterSteps;
	FIELD_STATE		   *state = FieldStateWork;

	if(*steps >= state->encounterThreshold * 9 / 10) {
		fdRequestDjinnBattle(DJINN_NO_HOP | ACTOR_HAIL, DJINNI_HAIL);
		state->encounterBias = 0;
	}
}

/* The update function of a sparkle around the Black Orb. It releases the
   sparkle on its second tick. */
static void scWorldMapReleaseSparkAfterDelay(ACTOR *actor)
{
	if(actor->tmr <= 0) {
		actor->tmr++;
	} else {
		fdActorRelease(actor);
	}
}

/* The update function of the Black Orb held up over the wreck. It swells and
   shrinks every four ticks and gives off a sparkle every other tick. */
static void scWorldMapPulseOrbAndSpawnSparks(ACTOR *actor)
{
	ACTOR *spark;
	FIXED  x;
	FIXED  y;
	FIXED  z;

	if(TickCount & 4) {
		actor->scaleX = toFIXED(13) / 10;
		actor->scaleY = toFIXED(13) / 10;
	} else {
		actor->scaleX = toFIXED(1);
		actor->scaleY = toFIXED(1);
	}
	if(!(TickCount & 2))
		return;
	x = actor->x.fixed;
	y = actor->y.fixed;
	z = actor->z.fixed;
	spark = syFieldActorCreate(SPRITE_SPARKLE, x, y, z);
	sySndPlay(SE_GLITTER);
	if(spark == NULL)
		return;
	spark->motionFlg = 0;
	spark->spr->image.pri = 1;
	fdActorSetSprFlags(spark, 0);
	fdActorSelectAnimation(spark, 1);
	spark->tmr = 0;
	spark->update = scWorldMapReleaseSparkAfterDelay;
}

/* Flint's update function until the party meets him (scWorldMapSetUpFlint). */
static void scWorldMapCircleFlint(ACTOR *actor)
{
	if(TickCount & 2)
		fdActorSetColMode(actor, SPRITE_COLOR_RED_TINT);
	else
		fdActorSetColMode(actor, SPRITE_COLOR_WHITE);
	if(actor->userValue == 0) {
		actor->x.fixed = toFIXED(5584);
		actor->y.fixed =
			syMulFX(sySin(actor->tmr << 3), toFIXED(4)) + toFIXED(16);
		actor->z.fixed = toFIXED(1328);
		syVectorAddPolarOffset(toFIXED(16), actor->tmr, (FIXED *)&actor->x);
		actor->facing = actor->tmr + FACING_SOUTH;
		actor->tmr += TRIG_TURN / 64;
	}
}

/* Flint's actor script (WorldMapFlintScript). Until he is caught or his lesson
   has begun (FLAG_DJINN_LESSON), he circles his spot at half size. */
static Sint32 scWorldMapSetUpFlint(ACTOR *actor)
{
	if(gmFlagGet(FLAG_DJINNI(DJINNI_FLINT)) != 0) {
		return 0;
	}
	if(gmFlagGet(FLAG_DJINN_LESSON) != 0) {
		return 0;
	}
	actor->update = scWorldMapCircleFlint;
	actor->motionFlg = 0;
	actor->tmr = 0;
	actor->userValue = 0;
	actor->scaleX = toFIXED(1) / 2;
	actor->scaleY = toFIXED(1) / 2;
	return 0;
}

const SCENE_ENTRANCE *scGetEntrances(void)
{
	return WorldSceneEntrances;
}

const SCENE_EXIT_REGION *scGetExitRegions(void)
{
	return NULL;
}

const Uint32 *scGetExits(void)
{
	return WorldMapExits;
}

/* Kolima Bridge's actor script (WorldMapKolimaBridgeScript). */
static Sint32 scWorldMapSetUpKolimaBridge(ACTOR *actor)
{
	scWorldMapAlignToMap(actor);
	if(gmFlagGet(FLAG_KOLIMA_BRIDGE_LOWERED) != 0) {
		fdActorSelectAnimation(actor, WORLD_MAP_BRIDGE_ANIMATION_LOWERED);
	}
	return 1;
}

/* The actor script of every picture on the map (WorldMapAlignToMapScript). It
   turns the sprite with the map's rotation. */
static Sint32 scWorldMapAlignToMap(ACTOR *actor)
{
	WORLD_MAP_VIEW *state = MapGridState[0];
	SPR			   *spr = actor->spr;

	spr->rotation = state->yaw;
	spr->flg = 0;
	return 1;
}

/* Iodem's actor script (WorldMapIodemScript). He turns with the map and blocks
   the way, so the leader cannot walk through him. */
static Sint32 scWorldMapSetUpIodem(ACTOR *actor)
{
	WORLD_MAP_VIEW *state = MapGridState[0];
	SPR			   *spr = actor->spr;

	actor->collisionFlg |= ACTOR_SOLID;
	spr->rotation = state->yaw;
	return 1;
}

/* Sets up the storm (ACTOR_STORM) as it is placed. */
static Sint32 scWorldMapSetUpStorm(ACTOR *actor)
{
	fdActorSetSprFlags(actor, 0);
	fdActorSetColMode(actor, SPRITE_COLOR_RED_TINT);
	actor->collisionFlg = 0;
	if(gmFlagGet(FLAG_TOLBI_SHIP_AT_ISLAND) != 0) {
		gmFlagSet(FLAG_WORLD_MAP_STORM_GONE);
		actor->x.fixed = 0;
		actor->y.fixed = 0;
	}
	return 0;
}

/* Called each tick by the storm's actor script (WorldMapStormScript): it hides
   the storm on odd ticks, but scWorldMapUpdateAreaContacts shows it again in
   the same tick, and it is drawn in every tick. */
static Sint32 scWorldMapHideStormOnOddTicks(ACTOR *actor)
{
	if((actor->sprStorageKind & ACTOR_SPRITE_ONE) && (TickCount & 1))
		actor->sprStorageKind ^= ACTOR_SPRITE_ONE;
	return 1;
}

/* Chooses the actors for the entrance. */
const ACTOR_PLACE *scGetActorPlacements(void)
{
	switch(MainRoutineStateWork.entrance) {
	case WORLD_MAP_FROM_WEST_LUNPA_CAVE:
		if(gmFlagGet(FLAG_BUNZAS_WAGON_LEFT) == 0 &&
			gmFlagGet(FLAG_DODONPA_LOCKED_UP) != 0) {
			return WorldMapActorPlacementsFromWestLunpaCave;
		}
		break;
	case WORLD_MAP_HAMMETS_CARAVAN_LEAVES:
		if(gmFlagGet(FLAG_LUNPA_GUARDS_SUSPECT_KALAY) == 0) {
			return WorldMapActorPlacementsHammetsCaravan;
		}
		break;
	case WORLD_MAP_FROM_SHIP_ROOMS_SAIL_TO_FIRST_ATTACK:
	case WORLD_MAP_FROM_TOLBI_SHIP_DECK_REACH_TOLBI:
		return WorldMapActorPlacementsCrossingEnds;
	case WORLD_MAP_FROM_KALAY_TUNNEL_ENTRANCE_SOUTH:
		return WorldMapActorPlacementsFromKalayTunnelEntranceSouth;
	case WORLD_MAP_FROM_FILE_SCREEN:
		return WorldMapActorPlacementsFromClear;
	case WORLD_MAP_BEFORE_SHEBAS_FALL:
		return WorldMapActorPlacementsBeforeShebasFall;
	case WORLD_MAP_FROM_SHIP_ROOMS_SAIL_TO_SECOND_ATTACK:
	case WORLD_MAP_FROM_SHIP_ROOMS_SAIL_TO_THIRD_ATTACK:
	case WORLD_MAP_FROM_SHIP_ROOMS_SAIL_TO_KRAKEN:
	case WORLD_MAP_FROM_SHIP_ROOMS_SAIL_TO_LAND:
	case WORLD_MAP_FROM_SHIP_ROOMS:
		return WorldMapActorPlacementsCrossing;
	case WORLD_MAP_JENNA_AND_KRADEN_ADRIFT:
		return WorldMapActorPlacementsJennaAndKradenAdrift;
	default:
		break;
	}

	gmFlagSet(FLAG_WORLD_MAP_ORDINARY_ACTORS);
	return WorldMapActorPlacements;
}

/* Takes the party into an area with two ways in by the side it walks up from:
   `exitBefore` if the leader is west of the area's picture, else `exitAfter`,
   with the sound of leaving the map. */
static void scWorldMapSelectExitByX(
	Sint32 cue, Sint32 exitBefore, Sint32 exitAfter)
{
	FIELD_STATE *state;
	ACTOR		*area;
	ACTOR		*leader;

	area = fdActorGet(cue - STEP_CUE_AREA_BASE);
	leader = fdActorGet(MainRoutineStateWork.leaderActorId);
	state = FieldStateWork;
	if(leader->x.fixed < area->x.fixed) {
		state->exit = exitBefore;
	} else {
		state->exit = exitAfter;
	}
	sySndPlay(SE_MAP_EXIT);
}

/* Takes the party into an area with two ways in by the side it walks up from:
   `exitBefore` if the leader is north of the area's picture, else `exitAfter`,
   with the sound of leaving the map. */
static void scWorldMapSelectExitByZ(
	Sint32 cue, Sint32 exitBefore, Sint32 exitAfter)
{
	FIELD_STATE *state;
	ACTOR		*area;
	ACTOR		*leader;

	area = fdActorGet(cue - STEP_CUE_AREA_BASE);
	leader = fdActorGet(MainRoutineStateWork.leaderActorId);
	state = FieldStateWork;
	if(leader->z.fixed < area->z.fixed) {
		state->exit = exitBefore;
	} else {
		state->exit = exitAfter;
	}
	sySndPlay(SE_MAP_EXIT);
}

/* Walking onto the Bilibin Barricade on the World Map enters it by the side the
   party comes from, west or east. */
static void scWorldMapEnterBilibinBarricade(void)
{
	scWorldMapSelectExitByX(STEP_CUE_AREA(ACTOR_TO_BILIBIN_BARRICADE),
		WORLD_MAP_TO_BILIBIN_BARRICADE_WEST,
		WORLD_MAP_TO_BILIBIN_BARRICADE_EAST);
}

/* Walking onto Kolima Bridge on the World Map enters it at its north end from
   the north, or at its south end from the south. */
static void scWorldMapEnterKolimaBridge(void)
{
	scWorldMapSelectExitByZ(STEP_CUE_AREA(ACTOR_TO_KOLIMA_BRIDGE),
		WORLD_MAP_TO_KOLIMA_BRIDGE_NORTH, WORLD_MAP_TO_KOLIMA_BRIDGE_SOUTH);
}

/* Walking onto Kalay Docks on the World Map enters them by their north side
   from the north, or by their south side from the south. */
static void scWorldMapEnterKalayDocks(void)
{
	scWorldMapSelectExitByZ(STEP_CUE_AREA(ACTOR_TO_KALAY_DOCKS),
		WORLD_MAP_TO_KALAY_DOCKS_NORTH, WORLD_MAP_TO_KALAY_DOCKS_SOUTH);
}

/* Walking onto Gondowan Passage on the World Map enters it at its north end
   from the north, or at its south end from the south. */
static void scWorldMapEnterGondowanPassage(void)
{
	scWorldMapSelectExitByZ(STEP_CUE_AREA(ACTOR_TO_GONDOWAN_PASSAGE),
		WORLD_MAP_TO_GONDOWAN_PASSAGE_NORTH,
		WORLD_MAP_TO_GONDOWAN_PASSAGE_SOUTH);
}

/* Walking onto Lalivero on the World Map enters the town by its west side from
   the north, or by its east side from the south. */
static void scWorldMapEnterLalivero(void)
{
	scWorldMapSelectExitByZ(STEP_CUE_AREA(ACTOR_TO_LALIVERO),
		WORLD_MAP_TO_LALIVERO_WEST, WORLD_MAP_TO_LALIVERO_EAST);
}

const CUE *scGetCues(void)
{
	return WorldCues;
}

/* Run each tick on the map, where scInit adds it: it shows only the area
   pictures near the view and, when the leader touches one, raises that area's
   cue. */
static void scWorldMapUpdateAreaContacts(void)
{
	ACTOR		*leader;
	FIELD_STATE *state;
	ACTOR		*actor;
	FIXED		 left;
	FIXED		 right;
	FIXED		 top;
	FIXED		 bottom;
	Sint32		 leaderReach;
	Uint32		 i;

	leader = fdActorGet(MainRoutineStateWork.leaderActorId);
	leaderReach = leader->spr->scale * leader->radius;
	state = FieldStateWork;
	actor = state->camera;
	left = actor->x.fixed - toFIXED(160);
	right = actor->x.fixed + toFIXED(160);
	top = actor->z.fixed - toFIXED(300);
	bottom = actor->z.fixed + toFIXED(200);

	for(i = ACTOR_FIRST_PLACED; i <= CONTACT_LAST_ACTOR; i++) {
		FIXED  x;
		FIXED  z;
		FIXED  dx;
		Sint32 reach;
		Sint32 scale;

		actor = fdActorLookup(i);
		if(actor == NULL) {
			continue;
		}
		x = actor->x.fixed;
		z = actor->z.fixed;
		if(x < left || x > right || z < top || z > bottom) {
			actor->sprStorageKind = ACTOR_SPRITE_NONE;
			continue;
		}
		actor->sprStorageKind = ACTOR_SPRITE_ONE;
		if(DebugMode != 0 && gmFlagGet(FLAG_DEBUG_STEPS_OFF) != 0) {
			continue;
		}
		scale = actor->spr->scale;
		dx = actor->x.fixed - leader->x.fixed;
		if(dx < 0) {
			dx = leader->x.fixed - actor->x.fixed;
		}
		reach = leaderReach + actor->radius * scale;
		if(dx +
					(actor->z.fixed - leader->z.fixed < 0
							? leader->z.fixed - actor->z.fixed
							: actor->z.fixed - leader->z.fixed) <
				reach &&
			gmFlagGet(FLAG_FIELD_HANDLING_REQUESTS) == 0) {
			state->stepCue = i + STEP_CUE_AREA_BASE;
		}
	}
}

/* Walking onto Vault on the World Map enters the town by its south side. The
   first time, Hammet's caravan is seen leaving first. */
static void scWorldMapEnterVault(void)
{
	if(gmFlagGet(FLAG_LUNPA_GUARDS_SUSPECT_KALAY) == 0) {
		syImportScriptSetExitRequest(WORLD_MAP_TO_HAMMETS_CARAVAN_LEAVES);
	} else {
		sySndPlay(SE_MAP_EXIT);
		syImportScriptSetExitRequest(WORLD_MAP_TO_VAULT_SOUTH);
	}
}

/* Walking east along Crossbone Isle's shore into the anchored Tolbi-bound Ship
   takes the party back aboard, onto its deck. */
static void scWorldMapReturnToTolbiShip(void)
{
	fdScriptStart();
	syImportScriptSetExitRequest(WORLD_MAP_TO_TOLBI_SHIP_DECK_CROSSBONE_ISLE);
}

/* Sets the map up for the entrance the party arrives by: the lighthouse
   transitions and the Black Orb defeat lead straight on to another scene; every
   other way in starts the area contacts and plays the story moment its entrance
   names, if any. */
Sint32 scInit(void)
{
	MAIN_ROUTINE_STATE *routine = &MainRoutineStateWork;

	if(routine->entrance == WORLD_MAP_FROM_DEBUG) {
		gmFlagSet(FLAG_NO_ENCOUNTERS);
		gmFlagSet(FLAG_NO_ENCOUNTERS_IN_AREA);
		gmFlagSet(FLAG_DEBUG_STEPS_OFF);
	}
	if(routine->entrance == WORLD_MAP_AFTER_MERCURY_LIGHTHOUSE_LIT) {
		scWorldMapPlayLighthouseTransition(WORLD_MAP_LIGHTHOUSE_MERCURY);
		fdScriptSetDestination(SCENE_MERCURY_LIGHTHOUSE_AERIE,
			MERCURY_LIGHTHOUSE_AERIE_AFTER_BEACON_LIT);
	} else if(routine->entrance == WORLD_MAP_AFTER_VENUS_LIGHTHOUSE_LIT) {
		scWorldMapPlayLighthouseTransition(WORLD_MAP_LIGHTHOUSE_VENUS);
		fdScriptSetDestination(SCENE_VENUS_LIGHTHOUSE_AERIE,
			VENUS_LIGHTHOUSE_AERIE_AFTER_BEACON_LIT);
	} else if(routine->entrance == WORLD_MAP_AFTER_BLACK_ORB_DEFEAT) {
		fdScriptStart();
		gmPartyInventoryDiscard(ITEM_BLACK_ORB);
		syImportScriptSetExitRequest(WORLD_MAP_TO_ROCK_ROOMS);
	} else {
		gmFlagSet(FLAG_RETREAT_BLOCKED);
		FieldStateWork->transition =
			SCENE_TRANSITION(TRANSITION_CENTER_LINE, CENTER_LINE_STEADY);
		FieldStateWork->transitionFrm = MAP_TRANSITION_TICKS;
		sySynch(1);
		fdCameraSetSpeed(8);
		gmFlagClear(FLAG_SHOW_AREA_NAME);
		sySetTickEvent(scWorldMapUpdateAreaContacts, EVENT_PASS_TICK);
		/* The bridge south of Vault, which the quake broke as Hammet's caravan
		   left, lies broken until the Manticore of the Lamakan Desert is
		   beaten. */
		if(gmFlagGet(FLAG_LAMAKAN_MANTICORE_BEATEN) == 0) {
			fdSetWorldMapGridTile(128, 256, 176, 56);
		}
		/* Each special entrance resumes its journey or story script. */
		switch(routine->entrance) {
		case WORLD_MAP_FROM_VALE_PLAZA:
			if(fdScriptGetFlag(FLAG_VALE_PARTY_LEFT) == 0) {
				gmFlagSet(FLAG_VALE_PARTY_LEFT);
				/* XXX (no evidence): what this flag records; no code in either
				   game reads it. */
				gmFlagSet(0x85c);
			}
			break;
		case WORLD_MAP_FROM_GATEWAY_CAVE:
			if(gmFlagGet(FLAG_KEEP_PARTY_POSITION) != 0) {
				if(gmFlagGet(FLAG_BLACK_ORB_RETURNED) == 0 &&
					gmFlagGet(FLAG_WORLD_MAP_BLACK_ORB_HANDED_OVER) != 0) {
					IodemActor = ACTOR_IODEM;
					fdActorSetPos(ACTOR_IODEM, 6036, 3400);
					fdActorGet(IodemActor)->facing = FACING_SOUTH_SOUTHEAST;
					scWorldMapBlockGatewayCaveExit(IodemActor);
				}
			} else if(gmFlagGet(FLAG_BLACK_ORB_RETURNED) == 0 &&
				gmFlagGet(FLAG_LALIVERO_WOUNDED_SOLDIERS_SEEN) == 0) {
				scWorldMapRunBlackOrbScript();
			}
			break;
		case WORLD_MAP_FROM_WEST_LUNPA_CAVE:
			if(gmFlagGet(FLAG_BUNZAS_WAGON_LEFT) == 0 &&
				gmFlagGet(FLAG_DODONPA_LOCKED_UP) != 0) {
				scWorldMapBoardBunzasWagon();
			}
			break;
		case WORLD_MAP_HAMMETS_CARAVAN_LEAVES:
			if(gmFlagGet(FLAG_LUNPA_GUARDS_SUSPECT_KALAY) == 0) {
				scWorldMapShowHammetsCaravan();
			}
			break;
		case WORLD_MAP_FROM_SHIP_ROOMS_SAIL_TO_FIRST_ATTACK:
			scWorldMapSailToFirstMonsterAttack();
			break;
		case WORLD_MAP_FROM_SHIP_ROOMS_SAIL_TO_SECOND_ATTACK:
			scWorldMapSailToSecondMonsterAttack();
			break;
		case WORLD_MAP_FROM_SHIP_ROOMS_SAIL_TO_THIRD_ATTACK:
			scWorldMapSailToThirdMonsterAttack();
			break;
		case WORLD_MAP_FROM_SHIP_ROOMS_SAIL_TO_KRAKEN:
			scWorldMapSailToKraken();
			break;
		case WORLD_MAP_FROM_SHIP_ROOMS_SAIL_TO_LAND:
			scWorldMapSailUntilLandSighted();
			break;
		case WORLD_MAP_FROM_TOLBI_SHIP_DECK_REACH_TOLBI:
			scWorldMapSailIntoTolbi();
			break;
		case WORLD_MAP_FROM_KALAY_TUNNEL_ENTRANCE_SOUTH:
			scWorldMapStepDownFromBunzasWagon();
			break;
		case WORLD_MAP_FROM_FILE_SCREEN:
			scWorldMapShowIslandDrifting();
			break;
		case WORLD_MAP_BEFORE_SHEBAS_FALL:
			scWorldMapShakeVenusLighthouse();
			break;
		case WORLD_MAP_FROM_TOLBI_SHIP_DECK_CROSSBONE_ISLE:
		case WORLD_MAP_FROM_CROSSBONE_ISLE:
		case WORLD_MAP_FROM_SUHALLA_DESERT_SANDSTORM:
			gmFlagSet(FLAG_WORLD_MAP_LEFT);
			if(syFieldGameFlagGetByte(FLAG_WORLD_MAP_STORM_TICKS_LEFT) != 0) {
				MainRoutineStateWork.movementMode = LEADER_MOVEMENT_AIRBORNE;
				sySetTickEvent(scWorldMapUpdateStormTimer, EVENT_PASS_TICK);
			}
			break;
		case WORLD_MAP_FROM_SHIP_ROOMS:
			scWorldMapSailTolbiShipToIsland();
			break;
		case WORLD_MAP_JENNA_AND_KRADEN_ADRIFT:
			scWorldMapShowEndingScript();
			break;
		default:
			fdActorGet(ACTOR_TOLBI_SHIP)->scaleX = toFIXED(5) / 4;
			fdActorGet(ACTOR_TOLBI_SHIP)->scaleY = toFIXED(5) / 4;
			break;
		}
	}
	return 0;
}

/* The actors placed at WORLD_MAP_FROM_WEST_LUNPA_CAVE
   (WorldMapActorPlacementsFromWestLunpaCave), numbered from ACTOR_FIRST_PLACED.

   Bunza's wagon comes first. Bunza and Hammet come after Lunpa's and West Lunpa
   Cave's area actors. */
enum {
	/* Bunza's wagon. */
	ACTOR_WAGON = ACTOR_FIRST_PLACED,
	ACTOR_BUNZA = ACTOR_FIRST_PLACED + 3,
	ACTOR_HAMMET
};

/* Out of West Lunpa Cave once Dodonpa is locked up, Bunza and then Hammet come
   out beside Isaac at the cave mouth, face each other and nod. A covered wagon
   pulls up beside them; Hammet, Bunza and Isaac climb aboard in turn, and it
   rolls off east and away south out of sight, and the screen closes for the
   Kalay Tunnel Entrance. */
static void scWorldMapBoardBunzasWagon(void)
{
	ACTOR *actor;

	fdScriptStart();
	fdScriptStartScreenTransition();
	fdScriptWaitTransitionTicks();
	fdWorldMapFreeze();
	gmFlagSet(FLAG_BUNZAS_WAGON_LEFT);
	fdActorSetPos(ACTOR_BUNZA, 5856, 1180);
	fdActorSetDestinationOffset(ACTOR_BUNZA, 24, 8);
	fdActorWaitForMoveById(ACTOR_BUNZA);
	fdScriptWait(60);
	fdActorSetPos(ACTOR_HAMMET, 5856, 1180);
	fdActorSetDestinationOffset(ACTOR_HAMMET, 12, 24);
	fdScriptWait(30);
	fdActorFaceDirection(ACTOR_BUNZA, FACING_SOUTH_SOUTHWEST, 0);
	fdActorFaceDirection(ACTOR_HAMMET, FACING_NORTH_NORTHEAST, 0);
	fdScriptWait(60);
	fdActorSetAnimation(ACTOR_BUNZA, ANIM_NOD);
	fdActorSetAnimation(ACTOR_HAMMET, ANIM_NOD);
	fdScriptWait(120);
	fdActorSetPos(ACTOR_WAGON, 5880, 1208);
	fdScriptWait(60);
	fdActorSetAnimation(ACTOR_HAMMET, ANIM_WALK);
	actor = fdActorGet(ACTOR_WAGON);
	if(actor != NULL) {
		fdActorSetDestination(
			ACTOR_HAMMET, actor->x.part[0].integer, actor->z.part[0].integer);
	}
	fdActorWaitForMoveById(ACTOR_HAMMET);
	fdActorSetPos(ACTOR_HAMMET, 0, 0);
	fdScriptWait(60);
	fdActorSetAnimation(ACTOR_BUNZA, ANIM_WALK);
	actor = fdActorGet(ACTOR_WAGON);
	if(actor != NULL) {
		fdActorSetDestination(
			ACTOR_BUNZA, actor->x.part[0].integer, actor->z.part[0].integer);
	}
	fdActorWaitForMoveById(ACTOR_BUNZA);
	fdActorSetPos(ACTOR_BUNZA, 0, 0);
	fdScriptWait(60);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
	actor = fdActorGet(ACTOR_WAGON);
	if(actor != NULL) {
		fdActorSetDestination(ACTOR_PARTY_LEADER, actor->x.part[0].integer,
			actor->z.part[0].integer);
	}
	fdActorWaitForMoveById(ACTOR_PARTY_LEADER);
	fdActorSetPos(ACTOR_PARTY_LEADER, 0, 0);
	fdScriptWait(60);
	fdActorSetSpeed(ACTOR_WAGON, 0.5);
	fdActorSetDestinationOffset(ACTOR_WAGON, 56, 8);
	fdActorWaitForMoveById(ACTOR_WAGON);
	fdActorSetDestinationOffset(ACTOR_WAGON, 40, 40);
	fdActorWaitForMoveById(ACTOR_WAGON);
	fdActorSetDestinationOffset(ACTOR_WAGON, 8, 88);
	fdActorWaitForMoveById(ACTOR_WAGON);
	fdScriptFinishScreenTransition();
	syImportScriptSetExitRequest(WORLD_MAP_TO_KALAY_TUNNEL_ENTRANCE_SOUTH);
	fdScriptEnd();
}

/* The first time the party walks onto Vault, Hammet's caravan comes out of the
   town: its wagons roll up beside the party, Hammet says they cannot stay in so
   dangerous a place, and the caravan heads south to the river, where his man
   finds the bridge is down.

   While Hammet and his man talk of leaving Ivan behind and of the rod Ivan will
   search for, the ground shakes and a rock falls beside them. The party is then
   taken into Vault by its south side. */
static void scWorldMapShowHammetsCaravan(void)
{
	ACTOR		 *actor;
	ACTOR		 *sprActor;
	const Sint32 *action;

	actor = fdActorGet(ACTOR_FALLING_ROCK);
	fdScriptStart();
	fdWorldMapStartZoom(toFIXED(5) / 4, 1);
	sySynch(4);
	fdScriptStartScreenTransition();
	fdScriptWaitTransitionTicks();
	fdWorldMapFreeze();
	fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
	sySynch(1);
	fdActorSetSpeed(ACTOR_PARTY_LEADER, 1.6);
	fdActorSetSpeed(ACTOR_GARET, 1.6);
	fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 5884, 1576);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
	fdActorSetPos(ACTOR_CARAVAN_HAMMET, 5848, 1576);
	sySynch(1);
	fdActorSetSprColById(ACTOR_CARAVAN_HAMMET, SPRITE_COLOR_HIDDEN);
	fdActorSetSprFlags(fdActorGet(ACTOR_CARAVAN_HAMMET), 0);
	fdActorSetSpeedAndAcceleration(ACTOR_CARAVAN_FIRST_WAGON, 1.6, 0.4);
	fdActorSetSpeedAndAcceleration(ACTOR_CARAVAN_FIRST_WAGON + 1, 1.6, 0.4);
	fdActorSetSpeedAndAcceleration(ACTOR_CARAVAN_FIRST_WAGON + 2, 1.6, 0.4);
	fdActorSetSpeedAndAcceleration(ACTOR_CARAVAN_FIRST_WAGON + 3, 1.6, 0.4);
	sySndPlay(SE_RUMBLE);
	fdActorSetScript(ACTOR_CARAVAN_FIRST_WAGON, CaravanFirstWagonArrives);
	fdScriptWait(20);
	fdActorSetScript(ACTOR_CARAVAN_FIRST_WAGON + 1, CaravanSecondWagonArrives);
	fdScriptWait(20);
	fdActorSetScript(ACTOR_CARAVAN_FIRST_WAGON + 2, CaravanThirdWagonArrives);
	fdScriptWait(20);
	fdActorRunScriptAndWait(
		ACTOR_CARAVAN_FIRST_WAGON + 3, CaravanFourthWagonArrives);
	fdScriptPlaySnd(SOUND_FADE_OUT_ACTION_SE);
	sprActor = fdActorGet(ACTOR_PARTY_LEADER);
	if(sprActor != NULL) {
		fdActorSetPosFX(ACTOR_GARET, sprActor->x.fixed, sprActor->z.fixed);
	}
	fdActorWalkToAndWait(ACTOR_GARET, 5892, 1600);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTHWEST, 20);
	fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
	fdActorSetPos(ACTOR_CARAVAN_HAMMET, 5848, 1592);
	sySynch(1);
	/* Hammet will not stay, and the wagons head south for the bridge. */
	fdScriptSetMessageId(MsgWorldMapWeCantStayAnotherMinute);
	/* "We can't stay another minute in a dangerous place like this...." */
	fdScriptShowMessageThenWait(ACTOR_CARAVAN_HAMMET, 0, 10);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_SOUTHWEST, 40);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTHWEST, 60);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_SOUTHWEST, 10);
	sySndPlay(SE_RUMBLE);
	fdActorSetScript(ACTOR_CARAVAN_FIRST_WAGON, CaravanFirstWagonDeparts);
	fdScriptWait(20);
	fdActorSetScript(ACTOR_CARAVAN_FIRST_WAGON + 1, CaravanSecondWagonDeparts);
	fdScriptWait(20);
	fdActorSetScript(ACTOR_CARAVAN_FIRST_WAGON + 2, CaravanThirdWagonDeparts);
	fdScriptWait(10);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_SOUTH, 10);
	fdActorRunScriptAndWait(
		ACTOR_CARAVAN_FIRST_WAGON + 3, CaravanFourthWagonDeparts);
	fdScriptPlaySnd(SOUND_FADE_OUT_ACTION_SE);
	fdScriptWait(20);
	fdWorldMapUnfreeze();
	fdActorSetAnimation(ACTOR_CARAVAN_FIRST_WAGON, ANIM_STAND);
	fdActorSetAnimation(ACTOR_CARAVAN_FIRST_WAGON + 1, ANIM_STAND);
	fdActorSetAnimation(ACTOR_CARAVAN_FIRST_WAGON + 2, ANIM_STAND);
	fdActorSetAnimation(ACTOR_CARAVAN_FIRST_WAGON + 3, ANIM_STAND);
	fdCameraMoveTo(5640, CAMERA_KEEP, 1784, 1);
	fdCameraWaitForMove();
	fdScriptWait(20);
	fdWorldMapFreeze();
	fdActorSetPos(ACTOR_CARAVAN_MAN, 5640, 1752);
	sySynch(1);
	fdActorSetSpeed(ACTOR_CARAVAN_MAN, 1.2);
	fdActorWalkToAndWait(ACTOR_CARAVAN_MAN, 5640, 1736);
	fdActorWalkToAndWait(ACTOR_CARAVAN_MAN, 5624, 1736);
	fdActorWalkToAndWait(ACTOR_CARAVAN_MAN, 5624, 1784);
	fdScriptWait(20);
	fdActorRunRepeatedMotion(ACTOR_CARAVAN_MAN, 2);
	fdScriptWait(20);
	fdActorSetAttachedEffect(
		ACTOR_CARAVAN_MAN, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdScriptWait(60);
	fdActorFaceDirection(ACTOR_CARAVAN_MAN, FACING_EAST, 20);
	fdActorStartRepeatedMotion(ACTOR_CARAVAN_MAN, 3);
	/* "Master Hammet! This is terrible! The bridge has been..." */
	fdScriptShowMessageThenWait(ACTOR_CARAVAN_MAN, 0, 20);
	fdActorSetPos(ACTOR_CARAVAN_HAMMET, 5656, 1784);
	sySynch(1);
	fdActorSetSprColById(ACTOR_CARAVAN_HAMMET, SPRITE_COLOR_NORMAL);
	fdActorSetSprFlags(fdActorGet(ACTOR_CARAVAN_HAMMET), SPRITE_FLAG_SHADOW);
	fdActorSetSpeed(ACTOR_CARAVAN_HAMMET, 0.8);
	fdActorWalkToAndWait(ACTOR_CARAVAN_HAMMET, 5640, 1784);
	fdScriptWait(20);
	fdActorStartRepeatedMotion(ACTOR_CARAVAN_HAMMET, 2);
	/* "What...? The bridge?" */
	syFieldScriptShowMessageThenWait(
		SPEAKER_PORTRAIT_RIGHT | ACTOR_CARAVAN_HAMMET, 0, 10);
	fdActorFaceDirection(ACTOR_CARAVAN_HAMMET, FACING_SOUTH_SOUTHEAST, 60);
	fdActorFaceDirection(ACTOR_CARAVAN_HAMMET, FACING_WEST, 10);
	fdActorSetAttachedEffect(
		ACTOR_CARAVAN_HAMMET, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdScriptWait(60);
	fdActorFaceDirection(ACTOR_CARAVAN_MAN, FACING_SOUTH_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_CARAVAN_HAMMET, FACING_SOUTH_SOUTHEAST, 40);
	fdActorSetAttachedEffect(
		ACTOR_CARAVAN_HAMMET, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdScriptWait(60);
	fdActorStartRepeatedMotion(ACTOR_CARAVAN_HAMMET, 2);
	/* "This is awful... How are we going to get to Kalay now?" */
	syFieldScriptShowMessageThenWait(
		SPEAKER_PORTRAIT_RIGHT | ACTOR_CARAVAN_HAMMET, 0, 40);
	fdActorRunRepeatedMotion(ACTOR_CARAVAN_MAN, 1);
	fdActorFaceDirection(ACTOR_CARAVAN_MAN, FACING_EAST, 10);
	/* "...Master Hammet, I'm worried about leaving Ivan behind..." */
	fdScriptShowMessageThenWait(ACTOR_CARAVAN_MAN, 0, 10);
	fdActorShowEmote(ACTOR_CARAVAN_HAMMET, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
	/* "As fond as I am of Ivan, it is his fault my rod was stolen..." */
	syFieldScriptShowMessageThenWait(
		SPEAKER_PORTRAIT_RIGHT | ACTOR_CARAVAN_HAMMET, 0, 10);
	fdActorRunRepeatedMotion(ACTOR_CARAVAN_HAMMET, 1);
	fdActorSetAnimationAndWait(ACTOR_CARAVAN_HAMMET, ANIM_NOD);
	/* "And his strange powers will aid him in search of the rod." */
	fdScriptShowMessageThenWait(
		SPEAKER_PORTRAIT_RIGHT | ACTOR_CARAVAN_HAMMET, 0, 10);
	fdActorShowEmote(ACTOR_CARAVAN_MAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
	/* "Ivan has strange powers?" */
	fdScriptShowMessageThenWait(ACTOR_CARAVAN_MAN, 0, 20);
	fdActorSetAnimationAndWait(ACTOR_CARAVAN_HAMMET, ANIM_NOD);
	fdWorldMapUnfreeze();
	sySndPlay(SE_TREMOR);
	fdCameraSetSpeedAndAcceleration(4, 4);
	/* The ground shakes, and a rock falls beside Hammet and his man. */
	scWorldMapShakeCameraOverCaravan();
	sySndPlay(SOUND_FADE_OUT_ACTION_SE);
	fdActorShowEmote(
		ACTOR_CARAVAN_HAMMET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdActorShowEmote(ACTOR_CARAVAN_MAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdActorFaceDirection(ACTOR_CARAVAN_HAMMET, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_CARAVAN_MAN, FACING_EAST, 40);
	fdActorFaceDirection(ACTOR_CARAVAN_HAMMET, FACING_NORTH_NORTHWEST, 0);
	fdActorFaceDirection(ACTOR_CARAVAN_MAN, FACING_NORTH_NORTHWEST, 0);
	fdCameraSetSpeed(1);
	fdCameraMoveTo(5608, CAMERA_KEEP, 1736, 1);
	fdCameraWaitForMove();
	fdActorSetPos(ACTOR_INVISIBLE_ROCK_GUIDE, 5544, 1704);
	sySynch(1);
	fdActorSetSpeed(ACTOR_INVISIBLE_ROCK_GUIDE, 0.3);
	fdActorSetScript(ACTOR_INVISIBLE_ROCK_GUIDE, CaravanRockGuideScript);
	fdScriptWait(160);
	/* The falling rock drops from 64 pixels up, and bounces as it lands. */
	actor->gravity = toFIXED(1) / 10;
	actor->bounce = toFIXED(1) / 10;
	actor->scaleX = toFIXED(3) / 2;
	actor->scaleY = toFIXED(3) / 2;
	actor->tmr = 0;
	actor->y.fixed = toFIXED(64);
	actor->spr->rotation = TRIG_TURN * 15 / 16;
	fdActorSetSprFlags(actor, 0);
	fdActorSelectAnimation(actor, FALLING_ROCK_ANIMATION_TUMBLE);
	sySynch(1);
	fdActorSetSprFlags(fdActorGet(ACTOR_FALLING_ROCK), 0);
	sySetTickEvent(scWorldMapUpdateFallingRock, EVENT_PASS_TICK);
	do {
		sySynch(1);
	} while(actor->tmr == 0);
	fdActorSetSprFlags(fdActorGet(ACTOR_FALLING_ROCK), 0);
	fdActorSetSprFlags(fdActorGet(ACTOR_INVISIBLE_ROCK_GUIDE), 0);
	fdScriptWait(10);
	fdActorGet(ACTOR_CARAVAN_MAN)->velY = toFIXED(6);
	fdActorGet(ACTOR_CARAVAN_HAMMET)->velY = toFIXED(6);
	sySndPlay(SE_CRASH);
	fdCameraSetSpeedAndAcceleration(4, 4);
	scWorldMapShakeCameraWhereRockFalls();
	scWorldMapShakeCameraWhereRockFalls();
	fdScriptWait(60);
	fdCameraSetSpeed(2);
	fdCameraMoveTo(5640, CAMERA_KEEP, 1784, 1);
	fdCameraWaitForMove();
	fdWorldMapFreeze();
	fdActorSetAttachedEffect(
		ACTOR_CARAVAN_MAN, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdActorSetAttachedEffect(
		ACTOR_CARAVAN_HAMMET, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdScriptWait(60);
	syCloseEvent(scWorldMapUpdateFallingRock);
	sySynch(1);
	fdActorSetPos(ACTOR_INVISIBLE_ROCK_GUIDE, 0, 0);
	fdActorSetPos(ACTOR_FALLING_ROCK, 0, 0);
	fdActorFaceDirection(ACTOR_CARAVAN_HAMMET, FACING_WEST, 10);
	fdActorJump(ACTOR_CARAVAN_HAMMET, 4, 40);
	/* "We must leave this treacherous place immediately!" */
	syFieldScriptShowMessageThenWait(
		SPEAKER_PORTRAIT_RIGHT | ACTOR_CARAVAN_HAMMET, 0, 10);
	fdActorFaceDirection(ACTOR_CARAVAN_MAN, FACING_EAST, 10);
	/* "But to where? Where can we go?" */
	fdScriptShowMessageThenWait(ACTOR_CARAVAN_MAN, 0, 20);
	fdActorFaceDirection(ACTOR_CARAVAN_HAMMET, FACING_NORTH, 40);
	/* "We have no choice. We're heading north." */
	syFieldScriptShowMessageThenWait(
		SPEAKER_PORTRAIT_RIGHT | ACTOR_CARAVAN_HAMMET, 0, 20);
	fdActorJump(ACTOR_CARAVAN_MAN, 4, 20);
	/* "We'll wind up in Lunpa if we go north..." */
	fdScriptShowMessageThenWait(ACTOR_CARAVAN_MAN, 0, 10);
	fdActorRunRepeatedMotion(ACTOR_CARAVAN_HAMMET, 1);
	fdActorFaceDirection(ACTOR_CARAVAN_HAMMET, FACING_WEST, 10);
	/* "Well, it's better than being smashed by falling rocks here, isn't
	   it?" */
	fdScriptShowMessageThenWait(
		SPEAKER_PORTRAIT_RIGHT | ACTOR_CARAVAN_HAMMET, 0, 10);
	fdActorSetAttachedEffect(
		ACTOR_CARAVAN_MAN, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdScriptWait(80);
	fdActorSetAnimationAndWait(ACTOR_CARAVAN_HAMMET, ANIM_NOD);
	fdScriptWait(20);
	fdActorRunRepeatedMotion(ACTOR_CARAVAN_MAN, 1);
	fdActorSetAnimationAndWait(ACTOR_CARAVAN_MAN, ANIM_NOD);
	fdActorWalkToAndWait(ACTOR_CARAVAN_HAMMET, 5656, 1784);
	fdActorSetPos(ACTOR_CARAVAN_HAMMET, 0, 0);
	fdActorWalkToAndWait(ACTOR_CARAVAN_MAN, 5624, 1736);
	fdActorWalkToAndWait(ACTOR_CARAVAN_MAN, 5640, 1736);
	fdActorWalkToAndWait(ACTOR_CARAVAN_MAN, 5640, 1752);
	fdActorSetPos(ACTOR_CARAVAN_MAN, 0, 0);
	sySndPlay(SE_RUMBLE);
	fdActorSetScript(ACTOR_CARAVAN_FIRST_WAGON, CaravanFirstWagonReturns);
	fdActorSetScript(ACTOR_CARAVAN_FIRST_WAGON + 1, CaravanSecondWagonReturns);
	fdScriptWait(40);
	fdActorSetScript(ACTOR_CARAVAN_FIRST_WAGON + 2, CaravanThirdWagonReturns);
	fdScriptWait(40);
	fdActorRunScriptAndWait(
		ACTOR_CARAVAN_FIRST_WAGON + 3, CaravanFourthWagonReturns);
	fdWorldMapUnfreeze();
	fdActorSetPos(ACTOR_PARTY_LEADER, 5900, 1576);
	fdActorSetPos(ACTOR_GARET, 5908, 1600);
	fdCameraSetSpeed(4);
	fdCameraMoveTo(5848, CAMERA_KEEP, 1608, 1);
	fdCameraWaitForMove();
	action = CaravanWagonHeadsNorth;
	fdActorSetScript(ACTOR_CARAVAN_FIRST_WAGON, action);
	fdScriptWait(20);
	fdCameraSetSpeed(0.4);
	fdCameraMoveTo(5848, CAMERA_KEEP, 1544, 1);
	fdActorSetScript(ACTOR_CARAVAN_FIRST_WAGON + 1, action);
	fdScriptWait(20);
	fdActorSetScript(ACTOR_CARAVAN_FIRST_WAGON + 2, action);
	fdScriptWait(20);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_WEST, 0);
	fdActorSetScript(ACTOR_CARAVAN_FIRST_WAGON + 3, action);
	fdScriptWait(40);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTH, 0);
	fdActorWaitForScriptEndById(ACTOR_CARAVAN_FIRST_WAGON + 3);
	sySndPlay(SOUND_FADE_OUT_ACTION_SE);
	fdCameraSetSpeed(4);
	fdCameraMoveTo(5880, CAMERA_KEEP, 1608, 1);
	fdCameraWaitForMove();
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTHWEST, 80);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
	fdScriptWait(20);
	scWalkIntoLeader(ACTOR_GARET);
	fdCameraSetSpeed(0.8);
	fdCameraMoveTo(5848, CAMERA_KEEP, 1608, 1);
	fdActorSetSpeed(ACTOR_PARTY_LEADER, 0.8);
	fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 5848, 1576);
	fdScriptFinishScreenTransition();
	fdScriptWaitTransitionTicks();
	gmFlagSet(FLAG_LUNPA_GUARDS_SUSPECT_KALAY);
	syImportScriptSetExitRequest(WORLD_MAP_TO_VAULT_SOUTH);
	fdScriptEnd();
}

/* Moves to the next camera-shake point, in whole pixels, and holds it for four
   ticks. */
static inline void scWorldMapMoveShakeCamera(Sint32 x, Sint32 z)
{
	fdCameraMoveToFX(toFIXED(x), CAMERA_KEEP, toFIXED(z), 1);
	sySynch(4);
}

/* Shakes the camera around Hammet and his man as the ground trembles. */
static void scWorldMapShakeCameraOverCaravan(void)
{
	scWorldMapMoveShakeCamera(5644, 1784);
	scWorldMapMoveShakeCamera(5636, 1788);
	scWorldMapMoveShakeCamera(5644, 1780);
	scWorldMapMoveShakeCamera(5644, 1788);
	scWorldMapMoveShakeCamera(5636, 1780);
	scWorldMapMoveShakeCamera(5640, 1784);
	scWorldMapMoveShakeCamera(5642, 1784);
	scWorldMapMoveShakeCamera(5638, 1786);
	scWorldMapMoveShakeCamera(5642, 1782);
	scWorldMapMoveShakeCamera(5642, 1786);
	scWorldMapMoveShakeCamera(5638, 1782);
	scWorldMapMoveShakeCamera(5640, 1784);
}

/* Shakes the camera around the spot where the falling rock lands. */
static void scWorldMapShakeCameraWhereRockFalls(void)
{
	scWorldMapMoveShakeCamera(5612, 1736);
	scWorldMapMoveShakeCamera(5604, 1740);
	scWorldMapMoveShakeCamera(5612, 1732);
	scWorldMapMoveShakeCamera(5612, 1740);
	scWorldMapMoveShakeCamera(5604, 1732);
	scWorldMapMoveShakeCamera(5608, 1736);
	scWorldMapMoveShakeCamera(5610, 1736);
	scWorldMapMoveShakeCamera(5606, 1738);
	scWorldMapMoveShakeCamera(5610, 1734);
	scWorldMapMoveShakeCamera(5610, 1738);
	scWorldMapMoveShakeCamera(5606, 1734);
	scWorldMapMoveShakeCamera(5608, 1736);
}

/* Keeps the falling rock above its guide as it drops, and lands it once. */
static void scWorldMapUpdateFallingRock(void)
{
	ACTOR *actor;
	ACTOR *guide;

	actor = fdActorGet(ACTOR_FALLING_ROCK);
	guide = fdActorGet(ACTOR_INVISIBLE_ROCK_GUIDE);
	actor->x.fixed = guide->x.fixed;
	actor->z.fixed = guide->z.fixed;
	if(actor->y.fixed < toFIXED(10)) {
		actor->y.fixed = toFIXED(10);
		if(fdScriptGetFlag(FLAG_WORLD_MAP_FALLING_ROCK_LANDED) == 0) {
			sySndPlay(SE_CRASH);
			fdActorSelectAnimation(actor, FALLING_ROCK_ANIMATION_SHATTER);
			gmFlagSet(FLAG_WORLD_MAP_FALLING_ROCK_LANDED);
			actor->tmr = 1;
		}
	}
}

/* The Tolbi-bound Ship, drawn a quarter larger, sails out of Kalay Docks on the
   World Map before the first attack at the mast. */
static void scWorldMapSailToFirstMonsterAttack(void)
{
	ACTOR *actor;

	actor = fdActorGet(ACTOR_CROSSING_SHIP);
	fdScriptStart();
	fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
	sySynch(1);
	fdActorSetPos(ACTOR_PARTY_LEADER, 0, 0);
	actor->scaleY = toFIXED(5) / 4;
	actor->scaleX = toFIXED(5) / 4;
	fdCameraFollowActor(ACTOR_CROSSING_SHIP, 1);
	fdScriptStartScreenTransition();
	fdActorSetSpeed(ACTOR_CROSSING_SHIP, 0.4);
	fdActorWalkToAndWait(ACTOR_CROSSING_SHIP, 5288, 2328);
	fdScriptFinishScreenTransition();
	fdScriptWaitTransitionTicks();
	gmFlagSet(FLAG_TOLBI_SHIP_ROWING);
	syImportScriptSetExitRequest(WORLD_MAP_TO_TOLBI_SHIP_MAST_FIRST_ATTACK);
	fdScriptEnd();
}

/* A middle leg of the Karagol Sea crossing: the ship, seen from above, sails
   west over open sea with the camera following it, until the screen closes on
   the scene `exit` leads to. */
static inline void scWorldMapSailCrossingLeg(Sint32 exit)
{
	ACTOR *actor;

	actor = fdActorGet(ACTOR_CROSSING_SHIP);
	fdScriptStart();
	fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
	sySynch(1);
	fdActorSetPos(ACTOR_PARTY_LEADER, 0, 0);
	fdActorSetPos(ACTOR_CROSSING_SHIP, 7944, 200);
	actor->scaleX = toFIXED(5) / 4;
	actor->scaleY = toFIXED(5) / 4;
	sySynch(1);
	fdCameraFollowActor(ACTOR_CROSSING_SHIP, 1);
	fdScriptStartScreenTransition();
	fdActorSetSpeed(ACTOR_CROSSING_SHIP, 0.6);
	actor->tmr = 0;
	fdActorSetScript(ACTOR_CROSSING_SHIP, CrossingShipSailsWest);
	do {
		sySynch(1);
	} while(actor->tmr == 0);
	fdScriptFinishScreenTransition();
	fdScriptWaitTransitionTicks();
	gmFlagSet(FLAG_TOLBI_SHIP_ROWING);
	syImportScriptSetExitRequest(exit);
	fdScriptEnd();
}

/* The ship sails west across the Karagol Sea after the first attack, and the
   screen closes for the second attack at the mast. */
static void scWorldMapSailToSecondMonsterAttack(void)
{
	scWorldMapSailCrossingLeg(WORLD_MAP_TO_TOLBI_SHIP_MAST_SECOND_ATTACK);
}

/* The ship sails on west after the second attack, and the screen closes for the
   third. */
static void scWorldMapSailToThirdMonsterAttack(void)
{
	scWorldMapSailCrossingLeg(WORLD_MAP_TO_TOLBI_SHIP_MAST_THIRD_ATTACK);
}

/* The ship sails on west after the third attack, and the screen closes on the
   ship's rooms for the fourth, the Kraken's. */
static void scWorldMapSailToKraken(void)
{
	scWorldMapSailCrossingLeg(WORLD_MAP_TO_SHIP_ROOMS);
}

/* After the Kraken, the ship sails west across open sea until the screen closes
   on the mast, where land is sighted. */
static void scWorldMapSailUntilLandSighted(void)
{
	ACTOR *actor;

	actor = fdActorGet(ACTOR_CROSSING_SHIP);
	fdScriptStart();
	fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
	sySynch(1);
	fdActorSetPos(ACTOR_PARTY_LEADER, 0, 0);
	fdActorSetPos(ACTOR_CROSSING_SHIP, 7944, 200);
	actor->scaleX = toFIXED(5) / 4;
	actor->scaleY = toFIXED(5) / 4;
	sySynch(1);
	fdCameraFollowActor(ACTOR_CROSSING_SHIP, 1);
	fdScriptStartScreenTransition();
	fdActorSetSpeed(ACTOR_CROSSING_SHIP, 0.6);
	actor->tmr = 0;
	if(scWorldMapGetRowingBalance() == ROWING_BALANCE_OFF_COURSE) {
		fdActorSetScript(ACTOR_CROSSING_SHIP, CrossingShipDriftsNorth);
	} else {
		fdActorSetScript(ACTOR_CROSSING_SHIP, CrossingShipSailsWest);
	}
	do {
		sySynch(1);
	} while(actor->tmr == 0);
	fdScriptFinishScreenTransition();
	fdScriptWaitTransitionTicks();
	gmFlagSet(FLAG_TOLBI_SHIP_ROWING);
	syImportScriptSetExitRequest(WORLD_MAP_TO_TOLBI_SHIP_MAST_LAND_HO);
	fdScriptEnd();
}

/* The ship sails the last stretch along the coast into Tolbi's harbour, and the
   scene changes to its deck. */
static void scWorldMapSailIntoTolbi(void)
{
	ACTOR *actor;

	actor = fdActorGet(ACTOR_CROSSING_SHIP);
	fdScriptStart();
	fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
	sySynch(1);
	fdActorSetPos(ACTOR_PARTY_LEADER, 0, 0);
	fdActorSetPos(ACTOR_CROSSING_SHIP, 5096, 2328);
	actor->scaleY = toFIXED(5) / 4;
	actor->scaleX = toFIXED(5) / 4;
	sySynch(1);
	fdCameraFollowActor(ACTOR_CROSSING_SHIP, 1);
	fdScriptStartScreenTransition();
	fdActorSetSpeed(ACTOR_CROSSING_SHIP, 0.4);
	fdActorWalkToAndWait(ACTOR_CROSSING_SHIP, 5064, 2328);
	fdScriptFinishScreenTransition();
	fdScriptWaitTransitionTicks();
	gmFlagSet(FLAG_TOLBI_SHIP_REACHED_TOLBI);
	gmFlagClear(FLAG_TOLBI_SHIP_ROWING);
	syImportScriptSetExitRequest(WORLD_MAP_TO_TOLBI_SHIP_DECK);
	fdScriptEnd();
}

/* Flint explains how a Djinni is set and put on standby, running the Djinn menu
   for each part of the lesson, then starts the lesson battle. "After bringing a
   Djinni into your party, you must set it to somebody." */
static void scWorldMapExplainSettingDjinn(void)
{
	fdWorldMapUnfreeze();
	fdWorldMapStartZoom(toFIXED(1), 6);
	fdScriptWaitForZoomTransition();
	fdWorldMapFreeze();
	fdActorRunRepeatedMotion(ACTOR_VENUS_DJINNI, 2);
	fdScriptSetMessageId(MsgWorldMapAfterBringingDjinniIntoYour);
	/* "After bringing a Djinni into your party, you must set it to..." */
	fdScriptShowMessage(ACTOR_VENUS_DJINNI, 0);
	fdScriptWait(30);
	sySndPlay(SE_MENU_CURSOR_MOVE);
	mnAnimateSelectionToEntry(0, 2);
	fdScriptClearFlag(FLAG_DJINN_LESSON_PART_A);
	fdScriptClearFlag(FLAG_DJINN_LESSON_PART_B);
	mnDjinnMenuRun();
	fdActorJump(ACTOR_VENUS_DJINNI, 4, 30);
	fdScriptSetMessageId(MsgWorldMapNextIllShowHowCan);
	/* "Next, I'll show you how you can use us in battle." */
	fdScriptShowMessage(ACTOR_VENUS_DJINNI, 0);
	gmFlagClear(FLAG_DJINN_LESSON_PART_A);
	gmFlagSet(FLAG_DJINN_LESSON_PART_B);
	mnDjinnMenuRun();
	fdScriptWait(30);
	fdRequestBattleFromEncounterTable(
		ENCOUNTER_ZONE_SCRIPTED_VALE_VAULT, ENCOUNTER_SLOT_DJINN_LESSON);
}

/* The ticks Flint takes to grow to full size as he comes down to the party. */
#define FLINT_GROW_TICKS 16

/* Walking up to Flint, the spark circling his spot just outside Vale: he cries
   "Oh!", drops down beside Isaac at full size, "A traveling warrior!", shimmers
   with sparkles, and asks to come along to find his comrades, telling how Djinn
   make an Adept's Psynergy stronger. Each "No" makes him plead again, and the
   seventh time he comes anyway; on "Yes", "Flint, the Venus Djinni, is now
   Isaac's ally!" Then he teaches the Djinn screen: it opens, and he shows how
   to give a Djinni to another Adept and set it, and that setting one changes
   attributes and even class. Then comes the lesson battle.

   Back from it, still on his ground (FLAG_DJINN_LESSON set), he leaps up again:
   "See! We Djinn can be very useful, can't we?" He shows the Djinn screen once
   more for standby and recovery, offers to explain it all again, and at last
   joins for good. */
static void scWorldMapMeetFlint(void)
{
	ACTOR *djinni;
	ACTOR *leader;
	FIXED  x;
	FIXED  z;
	Sint32 cnt;
	Sint32 i;

	cnt = 0;
	djinni = fdActorGet(ACTOR_VENUS_DJINNI);
	leader = fdActorGet(ACTOR_PARTY_LEADER);
	x = (leader->x.fixed - toFIXED(5584)) / 2 + toFIXED(5584);
	z = (leader->z.fixed - toFIXED(1328)) / 2 + toFIXED(1328);
	if(!fdScriptGetFlag(FLAG_DJINN_LESSON)) {
		gmPartyRestoreHpPp(1);
		gmFlagSet(FLAG_DJINN_LESSON);
		fdScriptStart();
		leader = fdActorGet(ACTOR_PARTY_LEADER);
		if(leader != NULL)
			fdActorSetPosFX(
				ACTOR_VENUS_DJINNI, leader->x.fixed, leader->z.fixed);
		gmDjinnGive(ISAAC, ELEMENT_VENUS, DJINNI_INDEX(DJINNI_FLINT));
		gmDjinnRecoveryAddStandby(
			ISAAC, ELEMENT_VENUS, DJINNI_INDEX(DJINNI_FLINT));
		fdWorldMapFreeze();
		fdActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_VENUS_DJINNI, 0);
		fdScriptWait(10);
		fdActorShowEmote(
			ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
		djinni->userValue = 1;
		fdActorFaceActor(ACTOR_VENUS_DJINNI, ACTOR_PARTY_LEADER, 0);
		sySynch(16);
		fdScriptSetMessageId(MsgWorldMapOh);
		/* "Oh!" */
		fdScriptShowMessage(ACTOR_VENUS_DJINNI, 0);
		fdWorldMapUnfreeze();
		fdWorldMapStartZoom(toFIXED(6) / 5, 6);
		fdScriptWaitForZoomTransition();
		fdWorldMapFreeze();
		djinni->motionFlg = ACTOR_FALLS;
		djinni->gravity = toFIXED(1) / 4;
		djinni->speedLimit = toFIXED(1);
		djinni->acceleration = toFIXED(1);
		djinni->velY = 0;
		djinni->groundHeight = 0;
		syFieldActorSetMoveTarget(djinni, 5584, 0, 1328);
		/* Flint grows from half size to full as he comes down. */
		for(i = 0; i < FLINT_GROW_TICKS; i++) {
			djinni->scaleX += toFIXED(1) / 2 / FLINT_GROW_TICKS;
			djinni->scaleY += toFIXED(1) / 2 / FLINT_GROW_TICKS;
			sySynch(1);
		}
		fdActorFaceActor(ACTOR_VENUS_DJINNI, ACTOR_PARTY_LEADER, 0);
		fdActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_VENUS_DJINNI, 0);
		sySynch(16);
		djinni->update = 0;
		fdActorSetColMode(djinni, SPRITE_COLOR_NORMAL);
		djinni->gravity = toFIXED(1);
		/* "A traveling warrior!" */
		fdScriptShowMessage(ACTOR_VENUS_DJINNI, 0);
		sySndPlay(SE_PSYNERGY_SPARKLE);
		fdPsynergyLoadResources(ABILITY_MOVE, 0);
		for(i = 0; i < 60; i++) {
			if(TickCount & 2)
				fdActorSetColMode(djinni, SPRITE_COLOR_WHITE);
			else
				fdActorSetColMode(djinni, SPRITE_COLOR_NORMAL);
			if((TickCount & 15) == 0)
				scSpawnGoldenRing(djinni);
			sySynch(1);
		}
		fdPsynergyEnd();
		fdActorSetColMode(djinni, SPRITE_COLOR_NORMAL);
		fdActorRunRepeatedMotion(ACTOR_VENUS_DJINNI, 2);
		/* "And I see you're an Adept, too!" */
		fdScriptShowMessage(ACTOR_VENUS_DJINNI, 0);
		fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
		/* "You are just the kind of fighter that I've been looking for." */
		fdScriptShowMessage(ACTOR_VENUS_DJINNI, 0);
		fdActorShowEmote(
			ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
		fdActorWalkToAndWait(ACTOR_VENUS_DJINNI, FIXEDtoINT(x), FIXEDtoINT(z));
		fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_LEADER_LEAN_BACK);
		/* "Won't you please take me along with you?" */
		fdScriptShowMessage(ACTOR_VENUS_DJINNI, 0);
		fdActorShowEmote(
			ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
		fdActorJump(ACTOR_VENUS_DJINNI, 4, 30);
		mnMessageArgumentPush(ABILITY_DJINNI_FIRST, MESSAGE_ARGUMENT_ABILITY);
		/* "I am a Venus Djinni. My name is Flint." */
		fdScriptShowMessage(ACTOR_VENUS_DJINNI, 0);
		fdActorShowEmote(
			ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
		/* "I must find my comrades. We were separated from each other when the
		   volcano erupted." */
		fdScriptShowMessage(ACTOR_VENUS_DJINNI, 0);
		fdActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
		/* "...Maybe you can help gather my friends together as you travel." */
		fdScriptShowMessage(ACTOR_VENUS_DJINNI, 0);
		fdActorJump(ACTOR_VENUS_DJINNI, 2, 30);
		/* "If you let me join you, I'll prove to be very beneficial to
		   you..." */
		fdScriptShowMessage(ACTOR_VENUS_DJINNI, 0);
		i = 0;
		djinni->motionFlg = 0;
		syFieldActorSetMoveTargetFX(djinni, x, toFIXED(16), z);
		/* A whole turn as he rises, then half a turn as he drops. */
		for(; i < TRIG_TURN / FACING_STEP; i++) {
			djinni->facing += FACING_STEP;
			sySynch(1);
		}
		fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
		/* "Your Psynergy will grow stronger, and your Adept skills will
		   also..." */
		fdScriptShowMessage(ACTOR_VENUS_DJINNI, 0);
		djinni->motionFlg = ACTOR_FALLS;
		djinni->velY = 0;
		djinni->groundHeight = 0;
		for(i = 0; i < TRIG_TURN / 2 / FACING_STEP; i++) {
			djinni->facing += FACING_STEP;
			sySynch(1);
		}
		fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_LEADER_LEAN_BACK);
		/* "You can call on us in battle or just use our power to boost your
		   own!" */
		fdScriptShowMessage(ACTOR_VENUS_DJINNI, 0);
		fdActorShowEmote(ACTOR_VENUS_DJINNI, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
		fdActorFaceActor(ACTOR_VENUS_DJINNI, ACTOR_PARTY_LEADER, 0);
		fdActorRunRepeatedMotion(ACTOR_VENUS_DJINNI, 2);
		/* "If you call on many of us at once, our powers will work
		   together!" */
		fdScriptShowMessage(ACTOR_VENUS_DJINNI, 0);
		fdActorJump(ACTOR_VENUS_DJINNI, 2, 30);
		/* "What do you think? Will you take me with you?" */
		fdScriptOpenMessage(ACTOR_VENUS_DJINNI, 0);
		/* Each "No" makes Flint plead again; the seventh time he gives up
		   asking. */
		cnt = 0;
	plead:
		if(fdScriptAskYesNoWithActorAnimation(0, 0) == 1) {
			fdActorJump(ACTOR_VENUS_DJINNI, 2, 20);
			fdActorJump(ACTOR_VENUS_DJINNI, 2, 20);
			if(cnt == 6) {
				fdScriptSetMessageId(MsgWorldMapMeanieDontCare);
				/* "Meanie! I don't care what you say! I'm coming along with
				   you!" */
				fdScriptShowMessage(ACTOR_VENUS_DJINNI, 0);
				goto listened;
			}
			/* "Come on! I promise you won't regret it! Please take me with
			   you!", and five more pleas, down to "Take me with you!" */
			fdScriptSetMessageId(cnt + MsgWorldMapComePromiseWont);
			fdScriptOpenMessage(ACTOR_VENUS_DJINNI, 0);
			cnt++;
			goto plead;
		}
		fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_LEADER_LEAN_BACK);
		fdActorJump(ACTOR_VENUS_DJINNI, 2, 20);
		fdActorJump(ACTOR_VENUS_DJINNI, 4, 20);
		fdScriptSetMessageId(MsgWorldMapSeeWontRegret);
		/* "Oh, you'll see! You won't regret it!" */
		fdScriptShowMessage(ACTOR_VENUS_DJINNI, 0);
	listened:
		mnMessageArgumentPush(ABILITY_DJINNI_FIRST, MESSAGE_ARGUMENT_ABILITY);
		sySndPlay(MUS_A_NEW_ALLY);
		cnt = MsgWorldMapAbilityVenusDjinni;
		/* "Flint, the Venus Djinni, is now Isaac's ally!" */
		mnMessageShowCentered(cnt++, 3);
		fdScriptSetMessageId(cnt);
		fdActorJump(ACTOR_VENUS_DJINNI, 2, 20);
		/* "Now, I'll briefly teach you how to use Djinn." */
		fdScriptShowMessage(ACTOR_VENUS_DJINNI, 0);
		sySndPlay(MUS_TRY_YOUR_LUCK);
		scWorldMapExplainSettingDjinn();
		fdWorldMapUnfreeze();
		return;
	}
	fdScriptStart();
	leader = fdActorGet(ACTOR_PARTY_LEADER);
	if(leader != NULL)
		fdActorSetPosFX(ACTOR_VENUS_DJINNI, leader->x.fixed, leader->z.fixed);
	djinni->velY = toFIXED(10);
	syFieldActorSetMoveTargetFX(djinni, x, 0, z);
	fdScriptWait(30);
	fdWorldMapFreeze();
	fdActorFaceActor(ACTOR_VENUS_DJINNI, ACTOR_PARTY_LEADER, 0);
	fdActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_VENUS_DJINNI, 0);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_LEADER_LEAN_BACK);
	/* Back from the lesson battle, Flint sums the lesson up. */
	fdScriptSetMessageId(MsgWorldMapSeeDjinnUseful);
	fdActorJump(ACTOR_VENUS_DJINNI, 2, 20);
	fdActorJump(ACTOR_VENUS_DJINNI, 2, 20);
	/* "See! We Djinn can be very useful, can't we?" */
	fdScriptShowMessage(ACTOR_VENUS_DJINNI, 0);
	fdActorRunRepeatedMotion(ACTOR_VENUS_DJINNI, 2);
	/* "And one more thing..." */
	fdScriptShowMessage(ACTOR_VENUS_DJINNI, 0);
	sySndPlay(SE_MENU_CURSOR_MOVE);
	mnAnimateSelectionToEntry(0, 2);
	gmFlagSet(FLAG_DJINN_LESSON_PART_A);
	gmFlagClear(FLAG_DJINN_LESSON_PART_B);
	mnDjinnMenuRun();
	fdScriptSetMessageId(MsgWorldMapGoSetStandby);
	syFieldActorSetMoveTarget(djinni, 5584, 0, 1328);
	fdScriptWait(30);
	/* "So, we go from set to standby to recovery, then back to set." */
	fdScriptShowMessage(ACTOR_VENUS_DJINNI, 0);
	fdActorFaceActor(ACTOR_VENUS_DJINNI, ACTOR_PARTY_LEADER, 0);
	/* "This is the simplest way to use us." */
	fdScriptShowMessage(ACTOR_VENUS_DJINNI, 0);
	/* "Did you get all that?" */
	fdScriptOpenMessage(ACTOR_VENUS_DJINNI, 0);
	if(fdScriptAskYesNoWithActorAnimation(0, 0) == 1) {
		fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_LEADER_LEAN_BACK);
		fdActorRunRepeatedMotion(ACTOR_VENUS_DJINNI, 2);
		fdScriptSetMessageId(MsgWorldMapHmmmmExplainAgain);
		/* "Hmmmm, shall I explain it all to you again?" */
		fdScriptOpenMessage(ACTOR_VENUS_DJINNI, 0);
		if(fdScriptAskYesNoWithActorAnimation(0, 0) != 1) {
			/* "Well, then, I guess I'll start from the beginning, OK?" */
			fdScriptShowMessage(ACTOR_VENUS_DJINNI, 0);
			fdActorWalkToAndWait(
				ACTOR_VENUS_DJINNI, FIXEDtoINT(x), FIXEDtoINT(z));
			scWorldMapExplainSettingDjinn();
			fdWorldMapUnfreeze();
			return;
		}
	}
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_LEADER_LEAN_BACK);
	fdScriptSetMessageId(MsgWorldMapYeahWantLearn);
	fdActorJump(ACTOR_VENUS_DJINNI, 2, 20);
	fdActorJump(ACTOR_VENUS_DJINNI, 2, 20);
	fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
	fdActorShowEmote(
		ACTOR_VENUS_DJINNI, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
	/* "Oh, yeah! If you want to learn more about Djinn, refer to..." */
	fdScriptShowMessage(ACTOR_VENUS_DJINNI, 0);
	fdScriptSetFlag(FLAG_DJINN_LESSON_PART_A);
	fdScriptSetFlag(FLAG_DJINN_LESSON_PART_B);
	mnDjinnMenuRun();
	fdActorJump(ACTOR_VENUS_DJINNI, 2, 20);
	/* "Okay, I'll join you now. Thank you very much! Please, use us..." */
	fdScriptShowMessage(ACTOR_VENUS_DJINNI, 0);
	fdWorldMapUnfreeze();
	fdDjinnJoinParty(
		ACTOR_VENUS_DJINNI, ELEMENT_VENUS, DJINNI_INDEX(DJINNI_FLINT));
	sySndPlay(MUS_A_LONG_JOURNEY);
	fdScriptEnd();
	gmFlagClear(FLAG_DJINN_LESSON);
	gmFlagClear(FLAG_DJINN_LESSON_PART_A);
	gmFlagClear(FLAG_DJINN_LESSON_PART_B);
}

/* Iodem's actor on the world map, set as he comes out to meet the party. This
   and WorldMapBlendWork have no starting value, so the linker places them,
   and it orders such variables by a hash of their names: under the names
   WorldMapIodem and WorldMapBlend it puts them the other way round from the
   ROM, so these two names were chosen to keep the ROM's order. */
Sint32 IodemActor;

/* Brings Bunza's wagon to a stop and lets Isaac step down, after Hammet's
   return home (kalay.c's scKalayHammetReturnsToPalace). */
static void scWorldMapStepDownFromBunzasWagon(void)
{
	ACTOR *actor;

	actor = fdActorGet(ACTOR_KALAY_WAGON);
	fdScriptStart();
	fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
	sySynch(1);
	fdActorSetPos(ACTOR_PARTY_LEADER, 0, 0);
	sySynch(1);
	actor->scaleX = toFIXED(3) / 2;
	actor->scaleY = toFIXED(3) / 2;
	actor->facing = FACING_SOUTH;
	fdScriptStartScreenTransition();
	fdScriptWaitTransitionTicks();
	fdScriptWait(20);
	fdActorSetPos(ACTOR_KALAY_WAGON, 5480, 2104);
	sySynch(1);
	sySndPlay(SE_RUMBLE);
	fdActorSetSpeedAndAcceleration(ACTOR_KALAY_WAGON, 1.6, 0.4);
	fdActorSetAnimation(ACTOR_KALAY_WAGON, ANIM_WALK);
	fdActorMoveToAndWait(ACTOR_KALAY_WAGON, 5485, 2136);
	fdCameraSetSpeed(0.4);
	fdCameraMoveTo(5560, CAMERA_KEEP, 2136, 1);
	fdActorMoveToAndWait(ACTOR_KALAY_WAGON, 5534, 2136);
	fdActorMoveToAndWait(ACTOR_KALAY_WAGON, 5544, 2158);
	fdActorMoveToAndWait(ACTOR_KALAY_WAGON, 5608, 2168);
	fdActorSetAnimation(ACTOR_KALAY_WAGON, ANIM_STAND);
	fdScriptPlaySnd(SOUND_FADE_OUT_ACTION_SE);
	fdScriptWait(20);
	fdActorSetPos(ACTOR_PARTY_LEADER, 5592, 2168);
	sySynch(1);
	fdActorJump(ACTOR_PARTY_LEADER, 6, 0);
	fdActorSetSpeed(ACTOR_PARTY_LEADER, 2);
	fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 5576, 2168);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 40);
	sySndPlay(SE_RUMBLE);
	fdActorSetAnimation(ACTOR_KALAY_WAGON, ANIM_WALK);
	fdActorMoveToAndWait(ACTOR_KALAY_WAGON, 5624, 2168);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHEAST, 0);
	fdActorMoveToAndWait(ACTOR_KALAY_WAGON, 5624, 2104);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
	fdActorMoveToAndWait(ACTOR_KALAY_WAGON, 5565, 2104);
	fdActorMoveToAndWait(ACTOR_KALAY_WAGON, 5560, 2131);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 0);
	fdActorMoveToAndWait(ACTOR_KALAY_WAGON, 5490, 2136);
	fdActorMoveToAndWait(ACTOR_KALAY_WAGON, 5480, 2104);
	fdActorSetPos(ACTOR_KALAY_WAGON, 0, 0);
	sySndPlay(SOUND_FADE_OUT_ACTION_SE);
	fdScriptWait(40);
	fdCameraMoveTo(5592, CAMERA_KEEP, 2136, 1);
	fdActorSetSpeed(ACTOR_PARTY_LEADER, 0.8);
	fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 5592, 2136);
	fdScriptFinishScreenTransition();
	fdScriptWaitTransitionTicks();
	syImportScriptSetExitRequest(WORLD_MAP_TO_KALAY_EAST);
	fdScriptEnd();
}

/* Out of Gateway Cave, Iodem sees Babi's ship scuttled off the coast, gives
   Isaac the Black Orb and prompts him to use it on the ship. */
static void scWorldMapRunBlackOrbScript(void)
{
	ACTOR *leader;

	fdScriptStart();
	IodemActor = ACTOR_IODEM;
	scWorldMapBlockGatewayCaveExit(ACTOR_IODEM);
	fdScriptStartScreenTransition();
	fdScriptWaitTransitionTicks();
	fdWorldMapFreeze();
	leader = fdActorGet(ACTOR_PARTY_LEADER);
	if(leader != NULL) {
		fdActorSetPosFX(IodemActor, leader->x.fixed, leader->z.fixed);
	}
	fdActorSetSpeed(IodemActor, 1.6);
	fdActorWalkToAndWait(IodemActor, 5992, 3448);
	fdActorFaceDirection(IodemActor, FACING_EAST, 20);
	fdActorShowEmote(IodemActor, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
	fdActorStartRepeatedMotion(IodemActor, 2);
	fdScriptSetMessageId(MsgWorldMapLook);
	/* "Look at this!" */
	fdScriptShowMessageThenWait(IodemActor | SPEAKER_PORTRAIT_LEFT, 0, 10);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
	fdWorldMapUnfreeze();
	fdWorldMapStartZoom(toFIXED(7) / 5, 10);
	fdCameraSetSpeed(8);
	fdCameraMoveTo(6024, CAMERA_KEEP, 3432, 1);
	fdCameraWaitForMove();
	fdScriptWait(40);
	fdWorldMapFreeze();
	fdActorSetAttachedEffect(
		IodemActor, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdScriptWait(60);
	fdActorRunRepeatedMotion(IodemActor, 1);
	/* "What happened?" */
	fdScriptShowMessageThenWait(
		IodemActor | SPEAKER_PORTRAIT_LEFT | SPEAKER_PORTRAIT_RIGHT, 0, 40);
	fdActorWalkToAndWait(IodemActor, 5992, 3400);
	fdActorWalkToAndWait(IodemActor, 6036, 3400);
	fdActorFaceDirection(IodemActor, FACING_SOUTH_SOUTHEAST, 20);
	fdActorSetSpeed(ACTOR_PARTY_LEADER, 0.8);
	fdActorSetScript(ACTOR_PARTY_LEADER, BlackOrbLeaderScript);
	fdScriptWait(20);
	fdActorRunRepeatedMotion(IodemActor, 1);
	/* "Babi's ship..." */
	fdScriptShowMessageThenWait(IodemActor, 0, 10);
	while(fdActorGet(ACTOR_PARTY_LEADER)->tmr == 0) {
		sySynch(1);
	}
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 20);
	fdActorShowEmote(IodemActor, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 0);
	fdActorRunRepeatedMotion(IodemActor, 2);
	/* "Hm... Perhaps it is time..." */
	fdScriptShowMessageThenWait(IodemActor, 0, 10);
	fdActorFaceDirection(IodemActor, FACING_WEST, 10);
	/* "Isaac, take this... Try to use it." */
	fdScriptShowMessage(IodemActor, 0);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 20);
	fdActorGet(IodemActor)->steeringFlg &= ~ACTOR_FACES_MOTION;
	fdActorWalkToAndWait(IodemActor, 6028, 3400);
	fdScriptWait(1);
	fdActorGet(IodemActor)->steeringFlg |= ACTOR_FACES_MOTION;
	fdScriptWait(20);
	fdActorGet(IodemActor)->steeringFlg &= ~ACTOR_FACES_MOTION;
	fdActorWalkToAndWait(IodemActor, 6036, 3400);
	fdScriptWait(1);
	fdActorGet(IodemActor)->steeringFlg |= ACTOR_FACES_MOTION;
	gmItemShowFound(ITEM_BLACK_ORB, 3);
	gmPartyGiveItem(ITEM_BLACK_ORB, 0);
	fdActorRunRepeatedMotion(IodemActor, 1);
	fdScriptSetMessageId(MsgWorldMapNowUseOnShip);
	/* "Now use it... on the ship." */
	fdScriptShowMessageThenWait(IodemActor, 0, 10);
	fdActorFaceDirection(IodemActor, FACING_SOUTH_SOUTHEAST, 0);
	fdWorldMapUnfreeze();
	fdWorldMapStartZoom(toFIXED(1), 10);
	fdScriptWait(20);
	gmFlagSet(FLAG_WORLD_MAP_BLACK_ORB_HANDED_OVER);
	gmFlagSet(FLAG_SAW_SHIP_AT_IDEJIMA);
	MainRoutineStateWork.restartScene = SCENE_WORLD_MAP;
	MainRoutineStateWork.restartEntrance = WORLD_MAP_AFTER_BLACK_ORB_DEFEAT;
	fdScriptEnd();
}

/* While Iodem waits with the Black Orb handed over, walking into Gateway Cave
   runs scWorldMapIodemCallsIsaacBack instead of entering it, and his row of the
   placements stands him on the coast. `actorId` goes unread. */
static void scWorldMapBlockGatewayCaveExit(Sint32 actorId)
{
	Sint32 i;

	i = 0;
	while(1) {
		if(WorldCues[i].kind == CUE_EXIT &&
			WorldCues[i].subject == STEP_CUE_AREA(ACTOR_TO_GATEWAY_CAVE)) {
			WorldCues[i].kind = CUE_STEP;
			WorldCues[i].value = (Uint32)scWorldMapIodemCallsIsaacBack;
		}
		if(WorldCues[i].kind == CUE_TABLE_END)
			break;
		i++;
	}
	for(i = 0;; i++) {
		if(WorldMapActorPlacements[i].spr == SPRITE_IODEM) {
			WorldMapActorPlacements[i].x = toFIXED(6036);
			WorldMapActorPlacements[i].z = toFIXED(3400);
			WorldMapActorPlacements[i].facing = FACING_SOUTH_SOUTHEAST;
			return;
		}
	}
}

/* Makes touching Gateway Cave an exit again, to WORLD_MAP_TO_GATEWAY_CAVE. */
static void scWorldMapRestoreGatewayCaveExit(void)
{
	Sint32 i;

	i = 0;
	while(1) {
		if(WorldCues[i].kind == CUE_STEP &&
			WorldCues[i].subject == STEP_CUE_AREA(ACTOR_TO_GATEWAY_CAVE)) {
			WorldCues[i].kind = CUE_EXIT;
			WorldCues[i].value = WORLD_MAP_TO_GATEWAY_CAVE;
			break;
		}
		if(WorldCues[i].kind == CUE_TABLE_END)
			break;
		i++;
	}
}

/* Talking to Iodem, who gave the Black Orb, or reading his mind, makes him urge
   its use. "Now use it... on the ship." */
static void scWorldMapTalkAboutBlackOrb(void)
{
	fdScriptStart();
	fdActorFaceActor(ACTOR_IODEM, ACTOR_PARTY_LEADER, 0);
	fdScriptSetMessageId(MsgWorldMapNowUseOnShip);
	/* "Now use it... on the ship." */
	fdScriptShowMessage(IodemActor, 0);
	fdActorFaceDirection(ACTOR_IODEM, FACING_SOUTH_SOUTHEAST, 0);
	fdScriptEnd();
}

/* Walking back into Gateway Cave with the Black Orb unused makes Iodem call the
   leader back to the coast. */
static void scWorldMapIodemCallsIsaacBack(void)
{
	fdScriptStart();
	fdWorldMapFreeze();
	fdScriptSetMessageId(MsgWorldMapIsaacWhereGoingSaidUse);
	/* "Isaac, where you going? I said to use the Orb on Babi's ship!" */
	fdScriptShowMessage(IodemActor, 0);
	fdWorldMapUnfreeze();
	fdActorSetSpeed(ACTOR_PARTY_LEADER, 1);
	fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 6008, 3400);
	fdScriptEnd();
}

/* Checking the wreck of Babi's ship off the coast by Gateway Cave reads "The
   wreckage of a ship scuttled off the coast." While Isaac holds the Black Orb
   Iodem gave him, the field menu then opens, so that he can use it. */
static void scWorldMapSearchWreckage(void)
{
	fdScriptStart();
#if !WRECK_SEARCH_WITHOUT_FREEZE
	fdWorldMapFreeze();
#endif
	mnFieldMessageShow(MsgWorldMapWreckageShipScuttledOffCoast, 1);
	if(gmFlagGet(FLAG_WORLD_MAP_BLACK_ORB_HANDED_OVER) != 0) {
		FieldStateWork->fieldMenu = 1;
	}
#if !WRECK_SEARCH_WITHOUT_FREEZE
	fdWorldMapUnfreeze();
#endif
	fdScriptEnd();
}

/* Using the Black Orb on the wreck off Gateway Cave, Isaac holds it up and it
   pulses and sparkles, but nothing happens. Iodem decides it is not yet time
   ("We must press on to Babi Lighthouse"), takes the orb back and leaves, and
   Gateway Cave can be entered again. */
static void scWorldMapUseBlackOrb(void)
{
	ACTOR		  *iodem;
	ACTOR		  *orb;
	ACTOR		  *leader;
	ACTOR		  *actor;
	Sint32		   msg;
	SPR			  *spr;
	ICON_TRANSFER *icon;

	leader = fdActorGet(ACTOR_PARTY_LEADER);
	fdScriptStart();
	fdWorldMapUnfreeze();
	fdWorldMapStartZoom(toFIXED(7) / 5, 6);
	fdCameraSetSpeed(3);
	fdCameraMoveTo(6024, CAMERA_KEEP, 3432, 1);
	fdActorSetSpeed(ACTOR_PARTY_LEADER, 0.8);
	iodem = NULL;
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
	leader->paused = 0;
	fdActorStartIdle(leader);
	if(leader->z.fixed > toFIXED(3432)) {
		if(leader->x.fixed > toFIXED(5998)) {
			syFieldActorSetMoveTargetFX(
				leader, toFIXED(5998), leader->y.fixed, toFIXED(3453));
			fdActorWaitForMove(leader);
		}
	} else if(leader->x.fixed > toFIXED(6010)) {
		/* BUG: iodem is NULL here; this branch reads its height through
		   null. */
		syFieldActorSetMoveTargetFX(
			leader, toFIXED(6010), iodem->y.fixed, toFIXED(3400));
		fdActorWaitForMove(leader);
	}
	syFieldActorSetMoveTarget(leader, 5993, 0, 3432);
	fdActorWaitForMove(leader);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 40);
	fdWorldMapFreeze();
	fdActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
	fdScriptWait(20);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_LEADER_ARMS_RAISED);
	orb = syFieldActorCreate(SPRITE_BLANK, leader->x.fixed + toFIXED(2),
		toFIXED(38), leader->z.fixed);
	if(orb != NULL) {
		orb->motionFlg = 0;
		spr = orb->spr;
		spr->flg = 0;
		spr->cnt = 0;
		spr->image.fullCol = 0;
		spr->image.pal = 0;
		/* LEFTOVER(template): the motion flags cleared again after the sprite
		 * resets, as scImilFuchinFallsPlaceDragonsEye clears them. */
		orb->motionFlg = 0;
		icon = syHeapSlotAlloc(HEAP_SLOT_ICON_TRANSFER, sizeof(ICON_TRANSFER));
		gmItemLoadIcon(ITEM_BLACK_ORB);
		mnVRAMLoad(spr->slot, ICON_TILE_BYTES, icon->tiles);
		syHeapSlotRelease(HEAP_SLOT_ICON_TRANSFER);
		fdScriptWait(20);
		orb->update = scWorldMapPulseOrbAndSpawnSparks;
		fdScriptWait(80);
	}
	fdActorGet(IodemActor)->facing = FACING_SOUTH_SOUTHEAST;
	fdActorShowEmote(IodemActor, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdActorRunRepeatedMotion(IodemActor, 2);
	msg = MsgWorldMapMatter;
	fdScriptSetMessageId(msg);
	/* "What's the matter?" */
	fdScriptShowMessageThenWait(IodemActor, 0, 80);
	if(orb != NULL) {
		fdActorRelease(orb);
	}
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
	fdScriptWait(40);
	fdActorJump(IodemActor, 6, 40);
	/* "Strange... Nothing happened. What does it mean?" */
	fdScriptShowMessageThenWait(IodemActor, 0, 20);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHEAST, 0);
	fdActorFaceDirection(IodemActor, FACING_NORTH_NORTHEAST, 20);
	/* "Could Babi have been wrong?" */
	fdScriptShowMessageThenWait(
		IodemActor | SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_LEFT, 0, 40);
	fdActorSetAnimationAndWait(IodemActor, ANIM_SHAKE_HEAD);
	/* "No. Not Babi..." */
	fdScriptShowMessageThenWait(
		IodemActor | SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_LEFT, 0, 20);
	fdActorFaceDirection(IodemActor, FACING_SOUTH_SOUTHEAST, 20);
	/* "I was wrong-it is not yet time. We must press on to Babi..." */
	fdScriptShowMessageThenWait(IodemActor, 0, 10);
	fdActorSetSpeed(IodemActor, 0.8);
	fdActorSetAnimation(IodemActor, ANIM_WALK);
	iodem = fdActorGet(ACTOR_IODEM);
	syFieldActorSetMoveTargetFX(
		iodem, toFIXED(6010), iodem->y.fixed, toFIXED(3400));
	fdActorWaitForMove(iodem);
	syFieldActorSetMoveTarget(iodem, 6001, 0, 3416);
	fdActorWaitForMove(iodem);
	fdActorSetAnimation(ACTOR_IODEM, ANIM_STAND);
	fdActorFaceDirection(IodemActor, FACING_SOUTH_SOUTHWEST, 10);
	fdActorRunRepeatedMotion(IodemActor, 1);
	/* "I will take that back, for now." */
	fdScriptShowMessageThenWait(IodemActor | SPEAKER_PORTRAIT_LEFT, 0, 20);
	fdActorSetSpeed(IodemActor, 1);
	fdActorGet(ACTOR_IODEM)->steeringFlg &= ~ACTOR_FACES_MOTION;
	fdActorSetAnimation(ACTOR_IODEM, ANIM_WALK);
	syFieldActorSetMoveTarget(iodem, 5997, 0, 3424);
	fdActorWaitForMove(iodem);
	fdActorSetAnimation(ACTOR_IODEM, ANIM_STAND);
	fdScriptWait(10);
	fdActorSetAnimation(ACTOR_IODEM, ANIM_WALK);
	syFieldActorSetMoveTarget(iodem, 6001, 0, 3416);
	fdActorWaitForMove(iodem);
	fdActorSetAnimation(ACTOR_IODEM, ANIM_STAND);
	/* "You returned the Black Orb." */
	mnMessageShowCentered(msg + 6, 1);
	FieldStateWork->msg++;
	gmPartyInventoryDiscard(ITEM_BLACK_ORB);
	fdScriptWait(20);
	fdActorSetAnimation(IodemActor, ANIM_SHAKE_HEAD);
	/* "We will return to the ship later." */
	fdScriptShowMessageThenWait(IodemActor, 0, 10);
	fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
	fdActorSetAnimationAndWait(IodemActor, ANIM_NOD);
	/* Iodem walks into Isaac and is taken off the map. */
	fdActorSetAnimation(IodemActor, ANIM_WALK);
	actor = fdActorGet(ACTOR_PARTY_LEADER);
	if(actor != NULL) {
		fdActorSetDestination(
			IodemActor, actor->x.part[0].integer, actor->z.part[0].integer);
	}
	fdActorWaitForMoveById(IodemActor);
	fdActorSetPos(IodemActor, 0, 0);
	fdWorldMapUnfreeze();
	fdWorldMapStartZoom(toFIXED(1), 6);
	fdScriptWait(20);
	scWorldMapRestoreGatewayCaveExit();
	fdActorReleaseById(IodemActor);
	gmFlagClear(FLAG_WORLD_MAP_BLACK_ORB_HANDED_OVER);
	gmFlagSet(FLAG_BLACK_ORB_RETURNED);
	fdScriptEnd();
}

/* The box around Venus Lighthouse, in pixels, where dust puffs up as it shakes
   (scWorldMapUpdateLighthouseQuake). */
enum {
	QUAKE_DUST_LEFT = 6064,
	QUAKE_DUST_TOP = 3148,
	QUAKE_DUST_WIDTH = 40,
	QUAKE_DUST_DEPTH = 30
};

/* The second weight of the lighthouse transition's blend (BLEND_ALPHA), which
   the picture's weight flickers over. */
Uint16 WorldMapBlendWork;

/* Queues one picture of the lighthouse transition, a whole tile map, into BG0's
   screen block. */
#define WORLD_MAP_QUEUE_TRANSITION_PICTURE(buffer, offset) \
	IO_WRITE_QUEUE_PUSH((Uint32)BG_SCREEN_BLOCK(4), \
		(Uint32)((buffer) + (offset)), DMA_COPY_WORDS(320))

/* Where picture `size` of the ball of light lies in the unpacked buffer: 0 is
   the ball at its largest, 6 at its smallest, and 7 the picture shown before it
   swells and after it shrinks. */
#define WORLD_MAP_LIGHT_BALL(size) (LIGHT_BALL_TILES_SIZE + 128 + (size) * 1536)

/* The ball of light's tiles (lighthouse_transition.png), first in the unpacked
   buffer: LIGHT_BALL_TILES 4-bit tiles, loaded into BG0's character block from
   tile LIGHT_BALL_FIRST_TILE, which its pictures count from, and shown in BG
   palette LIGHT_BALL_PALETTE. */
#define LIGHT_BALL_TILES	  128
#define LIGHT_BALL_TILES_SIZE (LIGHT_BALL_TILES * TILE_SIZE_4BPP)
#define LIGHT_BALL_FIRST_TILE 128
#define LIGHT_BALL_PALETTE	  14

/* The ending, on the island Jenna and Kraden are adrift on: they lie on its
   grass and talk of the Venus beacon, Alex comes to them, and he leads them to
   the shore, where Felix and Sheba lie washed up, alive. */
static void scWorldMapShowEndingScript(void)
{
	mnWindowColApply(
		MainRoutineStateWork.winHue, MainRoutineStateWork.winBrightness);
	fdScriptStart();
	fdWorldMapStartZoom(toFIXED(1), 300);
	fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
	fdActorSetAnimation(ACTOR_JENNA, ANIM_JENNA_LIE);
	fdActorSetAnimation(ACTOR_KRADEN, ANIM_KRADEN_LIE);
	fdActorSetPos(ACTOR_PARTY_LEADER, 0, 0);
	sySynch(1);
	fdWorldMapStartZoom(toFIXED(3) / 2, 16);
	FieldStateWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
	fdPaletteFadeApplyTarget(PALETTE_TINT_DIM, PALETTES_BG);
	FieldStateWork->transitionFrm = MAP_TRANSITION_TICKS;
	fdScriptStartScreenTransition();
	fdScriptWaitForZoomTransition();
	fdWorldMapFreeze();
	fdScriptWait(40);
	fdActorRunRepeatedMotion(ACTOR_JENNA, 1);
	fdScriptWait(20);
	/* Jenna and Kraden talk on the drifting island before Alex arrives. */
	fdScriptSetMessageId(MsgWorldMapKradenHowLongWillIsland);
	/* "Oh, Kraden... How long will this island continue to drift?" */
	fdScriptShowMessageThenWait(ACTOR_JENNA, 0, 20);
	fdActorRunRepeatedMotion(ACTOR_KRADEN, 2);
	fdScriptWait(20);
	/* "I don't know... I wish you could tell me." */
	fdScriptShowMessageThenWait(ACTOR_KRADEN, 0, 20);
	fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_SCOWL, 20);
	/* "Sheesh... You're no help in times like this." */
	fdScriptShowMessageThenWait(ACTOR_JENNA, 0, 20);
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 80);
	/* "Hey, I'm hungry..." */
	fdScriptShowMessageThenWait(ACTOR_KRADEN, 0, 10);
	fdActorRunRepeatedMotion(ACTOR_JENNA, 2);
	/* "How can you think about food?" */
	fdScriptShowMessageThenWait(ACTOR_JENNA, 0, 20);
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 100);
	fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
	/* "I wonder what happened to my brother..." */
	fdScriptShowMessageThenWait(ACTOR_JENNA, 0, 20);
	/* "I can't be certain, but it looks like they lit the Venus beacon." */
	fdScriptShowMessageThenWait(ACTOR_KRADEN, 0, 10);
	fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_WINCE, 20);
	/* "You don't have to tell me that. That's how we wound up..." */
	fdScriptShowMessageThenWait(ACTOR_JENNA, 0, 10);
	fdActorSetAttachedEffect(
		ACTOR_KRADEN, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdScriptWait(80);
	/* "You are so cranky today, Jenna!" */
	fdScriptShowMessageThenWait(ACTOR_KRADEN, 0, 20);
	fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 80);
	/* "Well, I suppose you must be worried about Felix." */
	fdScriptShowMessageThenWait(ACTOR_KRADEN, 0, 120);
	fdActorRunRepeatedMotion(ACTOR_JENNA, 1);
	/* "Isaac and his friends must have been nearby, too." */
	fdScriptShowMessageThenWait(ACTOR_JENNA, 0, 40);
	/* "They probably were..." */
	fdScriptShowMessageThenWait(ACTOR_KRADEN, 0, 20);
	fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
	/* "I miss them... Isaac... Felix..." */
	fdScriptShowMessageThenWait(ACTOR_JENNA, 0, 120);
	/* Alex comes to show them something and leads them on. */
	fdActorSetSpeed(ACTOR_ALEX, 0.4);
	fdActorSetPos(ACTOR_ALEX, 7644, 3460);
	fdActorWalkToAndWait(ACTOR_ALEX, 7572, 3468);
	fdActorWalkToAndWait(ACTOR_ALEX, 7560, 3488);
	fdScriptWait(20);
	/* "There is something I wish to show you." */
	syFieldScriptShowMessageThenWait(
		SPEAKER_WINDOW_ABOVE | SPEAKER_PORTRAIT_RIGHT | ACTOR_ALEX, 0, 20);
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
	fdActorSetAnimationAndWait(ACTOR_ALEX, ANIM_NOD);
	/* "Walk this way." */
	fdScriptShowMessage(
		SPEAKER_WINDOW_ABOVE | SPEAKER_PORTRAIT_RIGHT | ACTOR_ALEX, 0);
	fdWorldMapLoadDefaultMetatilesAndUpdateBlock();
	fdScriptWait(20);
	fdActorSetScript(ACTOR_ALEX, EndingAlexWalk);
	fdScriptWait(80);
	fdActorSetAnimation(ACTOR_KRADEN, ANIM_STAND);
	fdActorJump(ACTOR_KRADEN, 4, 40);
	fdActorSetAnimation(ACTOR_JENNA, ANIM_STAND);
	fdActorJump(ACTOR_JENNA, 4, 60);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_NORTH_NORTHWEST, 40);
	fdActorSetSpeed(ACTOR_KRADEN, 0.6);
	fdActorSetSpeed(ACTOR_JENNA, 0.6);
	fdActorSetScript(ACTOR_KRADEN, EndingKradenWalk);
	fdScriptWait(20);
	fdCameraSetSpeed(0.7);
	fdCameraMoveTo(7736, CAMERA_KEEP, 3528, 1);
	fdActorSetScript(ACTOR_JENNA, EndingJennaWalk);
	do {
		fdActorSetAnimation(ACTOR_FELIX, FELIX_ANIMATION_LIE);
		fdActorSetAnimation(ACTOR_SHEBA, SHEBA_ANIMATION_LIE);
		sySynch(1);
	} while(fdActorGet(ACTOR_JENNA)->tmr == 0);
	fdScriptWait(20);
	fdActorFaceDirection(ACTOR_ALEX, FACING_WEST, 20);
	fdActorSetAttachedEffect(
		ACTOR_KRADEN, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdActorSetAttachedEffect(
		ACTOR_JENNA, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdScriptWait(40);
	fdWorldMapFreeze();
	/* Jenna: "Felix... Sheba...", and Kraden: "Felix! Sheba!" */
	syFieldScriptShowTwoMessagesAndWait(
		ACTOR_JENNA, 7, 13, 2, 12, ACTOR_KRADEN, 9, 4, 4, 3, 0);
	fdScriptWait(20);
	fdWorldMapLoadDefaultMetatilesAndUpdateBlock();
	fdCameraSetSpeed(1);
	fdCameraMoveTo(7768, CAMERA_KEEP, 3528, 1);
	fdActorFaceDirection(ACTOR_ALEX, FACING_SOUTH_SOUTHEAST, 0);
	fdActorSetSpeed(ACTOR_KRADEN, 1.6);
	fdActorSetSpeed(ACTOR_JENNA, 1.6);
	fdActorWalkTo(ACTOR_KRADEN, 7804, 3512);
	fdActorWalkToAndWait(ACTOR_JENNA, 7788, 3544);
	fdActorSetAnimation(ACTOR_KRADEN, ANIM_STAND);
	fdWorldMapFreeze();
	fdScriptWait(80);
	fdActorRunRepeatedMotion(ACTOR_KRADEN, 1);
	/* "They... They're alive." */
	fdScriptShowMessageThenWait(ACTOR_KRADEN, 0, 20);
	fdActorRunRepeatedMotion(ACTOR_JENNA, 2);
	/* "They're alive!" */
	fdScriptShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_JENNA, 0, 40);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_WEST, 20);
	fdActorStartRepeatedMotion(ACTOR_KRADEN, 2);
	/* "What is this? Alex, what happened?" */
	fdScriptShowMessageThenWait(ACTOR_KRADEN, 0, 60);
	fdWorldMapLoadDefaultMetatilesAndUpdateBlock();
	sySndPlay(SOUND_FADE_OUT_BGM);
	fdPaletteFadeApplyTarget(RGB(0, 0, 0), PALETTES_ALL);
	fdPaletteFadeStartInterpolation(120);
	sySynch(120);
	fdScriptSetDestination(SCENE_TITLE, TITLE_FROM_WORLD_MAP);
}

/* After the beacon is lit, the view settles on Venus Lighthouse on the World
   Map, which shakes with a rumble as yellow clouds of dust puff up round it,
   before the scene returns to the aerie for Sheba's fall. */
static void scWorldMapShakeVenusLighthouse(void)
{
	fdScriptStart();
	sySndPlay(SE_RUMBLE);
	fdPaletteFadeApplySource(RGB(0, 0, 0), PALETTES_ALL);
	fdPaletteFadeApplyTarget(RGB(0, 0, 0), PALETTES_ALL);
	fdPaletteFadeStartInterpolation(1);
	sySynch(2);
	FieldStateWork->transitionFrm = 1;
	fdScriptStartScreenTransition();
	fdScriptWaitTransitionTicks();
	fdActorSetSprColById(ACTOR_PARTY_LEADER, SPRITE_COLOR_HIDDEN);
	fdActorSetSprFlags(fdActorGet(ACTOR_PARTY_LEADER), 0);
	sySynch(1);
	fdCameraSetSpeed(4);
	sySetTickEvent(scWorldMapUpdateLighthouseQuake, EVENT_PASS_TICK);
	fdPaletteFadeApplySource(RGB(0, 0, 0), PALETTES_ALL);
	fdPaletteFadeApplyTarget(PALETTE_TINT_MUTED, PALETTES_BG);
	fdPaletteFadeApplyTarget(PALETTE_TINT_NONE, PALETTES_SPRITES);
	fdPaletteFadeStartInterpolation(40);
	fdScriptWait(240);
	fdPaletteFadeApplyTarget(RGB(0, 0, 0), PALETTES_ALL);
	fdPaletteFadeStartInterpolation(80);
	sySynch(90);
	syImportScriptSetExitRequest(WORLD_MAP_TO_VENUS_LIGHTHOUSE_AERIE);
	gmFlagSet(FLAG_KEEP_MUSIC_ONCE);
	fdScriptEnd();
}

/* Run each tick while Venus Lighthouse shakes. */
static void scWorldMapUpdateLighthouseQuake(void)
{
	Uint32 x = syRandomBelow(QUAKE_DUST_WIDTH);
	Uint32 z = syRandomBelow(QUAKE_DUST_DEPTH);
	ACTOR *actor;

	actor = syFieldActorCreate(SPRITE_DUST_PUFF,
		toFIXED(x) + toFIXED(QUAKE_DUST_LEFT), 0,
		toFIXED(z) + toFIXED(QUAKE_DUST_TOP));
	if(actor != NULL) {
		SPR	 *spr = actor->spr;
		FIXED scale = syRandomBelow(toFIXED(1) / 2) + toFIXED(6) / 5;

		spr->flg = 0;
		spr->image.pri = 2;
		actor->motionFlg = 0;
		actor->scaleX = scale;
		actor->scaleY = scale;
		fdActorSelectAnimation(actor, 1);
		fdActorStartScript(actor, QuakeCloudScript);
	}
	if(TickCount % 3 == 0) {
		switch(syRandomBelow(4)) {
		case 0:
			fdCameraMoveTo(6087, CAMERA_KEEP, 3177, 1);
			break;
		case 1:
			fdCameraMoveTo(6089, CAMERA_KEEP, 3175, 1);
			break;
		case 2:
			fdCameraMoveTo(6089, CAMERA_KEEP, 3177, 1);
			break;
		case 3:
			fdCameraMoveTo(6087, CAMERA_KEEP, 3175, 1);
			break;
		}
	}
}

/* A leg of the Tolbi-bound Ship's crossing: from open sea the ship sails
   north-west with the camera on it until the green, wooded coast of an island
   comes into view, then turns north along the shore through the shallows and
   slows to a stop beside it. The screen closes for the rowing deck at the
   island. */
static void scWorldMapSailTolbiShipToIsland(void)
{
	fdScriptStart();
	fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
	sySynch(1);
	fdActorSetAnimation(ACTOR_CROSSING_SHIP, ANIM_WALK);
	fdActorSetPos(ACTOR_CROSSING_SHIP, 4872, 808);
	fdActorGet(ACTOR_CROSSING_SHIP)->facing = FACING_NORTHWEST;
	sySynch(1);
	fdWorldMapStartZoom(toFIXED(6) / 5, 1);
	fdActorSetPos(ACTOR_PARTY_LEADER, 0, 0);
	fdCameraFollowActor(ACTOR_CROSSING_SHIP, 1);
	sySynch(1);
	FieldStateWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
	fdScriptStartScreenTransition();
	fdActorSetSpeed(ACTOR_CROSSING_SHIP, 0.4);
	fdActorMoveToAndWait(ACTOR_CROSSING_SHIP, 4824, 712);
	fdActorMoveToAndWait(ACTOR_CROSSING_SHIP, 4776, 616);
	fdActorSetSpeed(ACTOR_CROSSING_SHIP, 0.3);
	fdActorMoveToAndWait(ACTOR_CROSSING_SHIP, 4776, 472);
	fdActorSetSpeed(ACTOR_CROSSING_SHIP, 0.2);
	fdActorMoveToAndWait(ACTOR_CROSSING_SHIP, 4760, 456);
	fdActorSetSpeed(ACTOR_CROSSING_SHIP, 0.1);
	fdActorMoveToAndWait(ACTOR_CROSSING_SHIP, 4760, 440);
	fdActorSetAnimation(ACTOR_CROSSING_SHIP, ANIM_STAND);
	fdScriptWait(40);
	fdScriptFinishScreenTransition();
	fdScriptWaitTransitionTicks();
	syImportScriptSetExitRequest(WORLD_MAP_TO_SHIP_ROOMS_ROWING_DECK_ISLAND);
}

/* The rowing balance of the chosen passengers, as
   scTolbiShipMastGetRowingBalance reckons it: the values of those at the west
   seats (the first and third) less those at the east seats (the second and
   fourth). */
static Sint32 scWorldMapGetRowingBalance(void)
{
	Sint32 westSeats;
	Sint32 eastSeats;

	westSeats = scWorldMapGetSeatRowValue(0) + scWorldMapGetSeatRowValue(2);
	eastSeats = scWorldMapGetSeatRowValue(1) + scWorldMapGetSeatRowValue(3);
	return westSeats - eastSeats;
}

/* The rowing value of the passenger chosen for a seat, or 0 while it is
   empty. */
static Sint32 scWorldMapGetSeatRowValue(Uint32 seat)
{
	Sint32 flgBase = 0;
	Uint32 ofs;

	switch(seat) {
	case 0:
		flgBase = FLAG_TOLBI_SHIP_FIRST_SEAT_ROWERS;
		break;
	case 1:
		flgBase = FLAG_TOLBI_SHIP_SECOND_SEAT_ROWERS;
		break;
	case 2:
		flgBase = FLAG_TOLBI_SHIP_THIRD_SEAT_ROWERS;
		break;
	case 3:
		flgBase = FLAG_TOLBI_SHIP_FOURTH_SEAT_ROWERS;
		break;
	}
	for(ofs = 0; ofs < 9; ofs++) {
		if(gmFlagGet(flgBase + ofs) != 0)
			return TolbiShipPassengerRowValues[ofs];
	}
	return 0;
}

/* Spawns a puff of the wake behind ACTOR_DRIFTING_ISLAND every 16 ticks. */
static void scWorldMapSpawnIslandWake(void)
{
	ACTOR *island;
	ACTOR *puff;
	SPR	  *spr;
	Sint32 distance;

	if((TickCount & 15) != 0)
		return;
	island = fdActorGet(ACTOR_DRIFTING_ISLAND);
	puff = syFieldActorCreate(SPRITE_DUST_PUFF, island->x.fixed - toFIXED(32),
		island->y.fixed, island->z.fixed - toFIXED(16));
	if(puff == NULL)
		return;
	puff->scaleX = toFIXED(1) / 2;
	puff->scaleY = toFIXED(1) / 2;
	spr = puff->spr;
	/* Up to 48 pixels behind the island, or up to 32 to the side. */
	if((syRandomBelow(2)) != 0) {
		FIXED behind = toFIXED(syRandomBelow(48));

		puff->x.fixed -= behind / 2;
		puff->z.fixed -= behind;
	} else {
		distance = toFIXED(syRandomBelow(32));
		puff->x.fixed += distance;
		distance /= 2;
		puff->z.fixed += distance;
	}
	spr->flg = 0;
	spr->image.pri = island->spr->image.pri;
	puff->priFlg |= ACTOR_PRIORITY_UNDERFOOT;
	puff->motionFlg = island->motionFlg;
	fdActorSetSprCol(puff, SPRITE_COLOR_BLUE_TINT);
	fdActorSelectAnimation(puff, 2);
	fdActorStartScript(puff, IslandWakeScript);
}

/* Before the ending, the island Jenna and Kraden are adrift on drifts east
   across a darkened sea, trailing a pale wake, as the view closes in on it,
   then the screen fades to white.

   The darkening is PALETTE_TINT_DIM, applied to the backgrounds only. */
static void scWorldMapShowIslandDrifting(void)
{
	ACTOR *actor;

	actor = fdActorGet(ACTOR_DRIFTING_ISLAND);
	fdScriptWait(60);
	fdScriptStart();
	fdWorldMapStartZoom(toFIXED(3) / 5, 1);
	actor->scaleX = toFIXED(6) / 5;
	actor->scaleY = toFIXED(6) / 5;
	fdCameraFollowActor(ACTOR_DRIFTING_ISLAND, 1);
	sySynch(1);
	fdActorSetSprColById(ACTOR_PARTY_LEADER, SPRITE_COLOR_HIDDEN);
	fdActorSetSprFlags(fdActorGet(ACTOR_PARTY_LEADER), 0);
	fdActorSetSprFlags(fdActorGet(ACTOR_DRIFTING_ISLAND), 0);
	fdActorSetSpeed(ACTOR_DRIFTING_ISLAND, 0.4);
	actor->tmr = 0;
	fdActorSetScript(ACTOR_DRIFTING_ISLAND, IslandDriftsEast);
	sySetTickEvent(scWorldMapSpawnIslandWake, EVENT_PASS_TICK);
	FieldStateWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
	fdPaletteFadeApplyTarget(PALETTE_TINT_DIM, PALETTES_BG);
	FieldStateWork->transitionFrm = 32;
	fdScriptStartScreenTransition();
	fdScriptWait(120);
	fdWorldMapStartZoom(toFIXED(7) / 5, 300);
	fdScriptWait(270);
	FieldStateWork->transitionFrm = MAP_TRANSITION_TICKS;
	PLTT_BACKDROP = RGB_WHITE;
	fdScriptFinishScreenTransition();
	fdScriptWaitTransitionTicks();
	syImportScriptSetExitRequest(WORLD_MAP_TO_JENNA_AND_KRADEN_ADRIFT);
}

/* Run each tick while a storm holds the party in the air: it spins the leader
   round and counts down the time left, which stands still while the menu is
   open. */
static void scWorldMapUpdateStormTimer(void)
{
	ACTOR		*actor;
	FIELD_STATE *state;
	Sint32		 progress;

	actor = fdActorGet(MainRoutineStateWork.leaderActorId);
	state = FieldStateWork;
	actor->facing = TickCount * FACING_STEP;

	progress = syFieldGameFlagGetByte(FLAG_WORLD_MAP_STORM_TICKS_LEFT);
	if(progress != 0) {
		if(progress == 1) {
			state->raisedCue = RAISED_STORM_TIME_UP;
		} else if(gmFlagGet(FLAG_MENU_OPEN) == 0) {
			progress -= 1;
		}
	}
	syFieldGameFlagSetByte(FLAG_WORLD_MAP_STORM_TICKS_LEFT, progress);
}

/* Draws the leader into the storm, lifts him and starts its timer. */
static void scWorldMapCatchInStorm(Sint32 tgtActorId)
{
	ACTOR *actor;
	ACTOR *storm;
	Sint32 id;

	id = MainRoutineStateWork.leaderActorId;
	actor = fdActorGet(id);
	if(fdScriptGetFlag(FLAG_WORLD_MAP_CAUGHT_IN_STORM) == 0) {
		fdScriptStart();
		fdActorSetAttachedEffect(
			id, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ATTACH);
		fdActorSetAnimation(id, ANIM_LEADER_LOOK_AROUND);
		storm = fdActorGet(tgtActorId);
		if(storm != NULL) {
			fdActorSetDestination(
				id, storm->x.part[0].integer, storm->z.part[0].integer);
		}
		fdActorWaitForMoveById(id);
		sySndPlay(SE_SANDSTORM);
		sySetTickEvent(scWorldMapUpdateStormTimer, EVENT_PASS_TICK);
		actor->motionFlg = 0;
		syFieldActorSetMoveTargetFX(actor, actor->x.fixed,
			actor->y.fixed + toFIXED(32), actor->z.fixed);
		fdActorWaitForMoveById(id);
		actor->velY = 0;
		actor->motionFlg = ACTOR_FLOATS;
		MainRoutineStateWork.movementMode = LEADER_MOVEMENT_AIRBORNE;
		gmFlagSet(FLAG_WORLD_MAP_CAUGHT_IN_STORM);
		syFieldGameFlagSetByte(
			FLAG_WORLD_MAP_STORM_TICKS_LEFT, STORM_TICKS_IN_AIR);
		fdScriptEnd();
		FieldStateWork->encounter = 0; /* drop any battle this step rolled */
	}
}

/* Walking into the whirlwind near Crossbone Isle draws the leader into it and
   lifts the party into the air for three seconds. */
static void scWorldMapWalkIntoStorm(void)
{
	scWorldMapCatchInStorm(ACTOR_STORM);
}

/* Casting Douse on the whirlwind that holds the party gathers the rain cloud
   high up, over the party where it floats, not on the ground below
   (script_commands_douse.c reads `sceneVariant`). */
static void scWorldMapStartDousingStorm(void)
{
	((FIELD_EFFECT_STATE *)FieldEffectStateWork)->sceneVariant = 1;
}

/* Douse on the whirlwind that holds the party stops it, and the party drops to
   fight the Tempest Lizard inside, as in Suhalla Desert's red storm. The battle
   is taken from the scripted encounter table as that storm's is. */
static void scWorldMapDouseMapStorm(void)
{
	if(FieldStateWork->raisedCue == RAISED_STORM_TIME_UP) {
		FieldStateWork->raisedCue = 0;
	}
	gmFlagClear(FLAG_WORLD_MAP_CAUGHT_IN_STORM);
	gmFlagSet(FLAG_WORLD_MAP_STORM_GONE);
	syFieldGameFlagSetByte(FLAG_WORLD_MAP_STORM_TICKS_LEFT, 0);
	/* As in Suhalla Desert, the Tornado Lizard is asked for and then replaced
	   by the Tempest Lizard. */
	fdRequestBattleFromEncounterTable(
		ENCOUNTER_ZONE_SCRIPTED, ENCOUNTER_SLOT_TORNADO_LIZARD);
	SET_BATTLE_TYPE(BATTLE_TYPE_SCRIPTED);
	fdRequestBattleFromEncounterTable(
		ENCOUNTER_ZONE_SCRIPTED, ENCOUNTER_SLOT_TEMPEST_LIZARD);
	fdActorGet(MainRoutineStateWork.leaderActorId)->motionFlg = ACTOR_FALLS;
}

/* If the party is not brought down within three seconds, the whirlwind rises
   with them faster and faster out of sight, the screen closes, and they drop
   onto the World Map outside Suhalla. */
static void scWorldMapStormDropsParty(void)
{
	ACTOR *leader = fdActorGet(MainRoutineStateWork.leaderActorId);
	ACTOR *storm = fdActorGet(ACTOR_STORM);
	Sint32 ticks;

	fdScriptStart();
	fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
	sySndPlay(SE_GUSH);
	fdActorSetSprFlags(leader, 0);
	storm->motionFlg = 0;
	leader->motionFlg = 0;
	leader->velY = 0;
	leader->frozen = 1;
	storm->frozen = 1;
	for(ticks = 0; ticks < 60; ticks++) {
		leader->velY += toFIXED(1) / 5;
		storm->velY += toFIXED(1) / 5;
		sySynch(1);
	}
	fdScriptFinishScreenTransition();
	fdScriptWaitTransitionTicks();
	fdScriptEnd();
	gmFlagSet(FLAG_ARRIVE_FALLING);
	fdScriptSetDestination(SCENE_WORLD_MAP, WORLD_MAP_FROM_SUHALLA);
}

/* Run each tick of the lighthouse transition: blends the ball of light over
   the map, its own weight flickering between 12 and 16 sixteenths every two
   ticks over WorldMapBlendWork. */
static void scWorldMapUpdateTransitionBlend(void)
{
	syIOWriteQueueStore16((Uint32)&REG_BLDCNT,
		BLDCNT_TGT1_BG0 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL);
	if((TickCount & 2) != 0) {
		syIOWriteQueueStore16(
			(Uint32)&REG_BLDALPHA, WorldMapBlendWork | BLEND_ALPHA(12, 0));
	} else {
		syIOWriteQueueStore16(
			(Uint32)&REG_BLDALPHA, WorldMapBlendWork | BLEND_ALPHA(16, 0));
	}
}

/* Run each tick at the end of the lighthouse transition, as the ground
   rumbles: every third tick it moves the camera a pixel diagonally from the
   leader. */
static void scWorldMapJitterTransitionCamera(void)
{
	ACTOR *actor;
	Sint32 x;
	Sint32 z;
	Uint32 mode;

	actor = fdActorGet(MainRoutineStateWork.leaderActorId);
	x = actor->x.part[0].integer;
	z = actor->z.part[0].integer;
	if(TickCount % 3 == 0) {
		mode = syRandomBelow(4);
		switch(mode) {
		case 0:
			fdCameraMoveToFX(toFIXED(x) - toFIXED(1), CAMERA_KEEP,
				toFIXED(z) + toFIXED(1), 1);
			break;
		case 1:
			fdCameraMoveWithinMapBounds(toFIXED(x) + toFIXED(1), CAMERA_KEEP,
				toFIXED(z) - toFIXED(1), 1);
			break;
		case 2:
			fdCameraMoveToFX(toFIXED(x) + toFIXED(1), CAMERA_KEEP,
				toFIXED(z) + toFIXED(1), 1);
			break;
		case 3:
			fdCameraMoveToFX(toFIXED(x) - toFIXED(1), CAMERA_KEEP,
				toFIXED(z) - toFIXED(1), 1);
			break;
		}
	}
}

/* Queues one picture of the lighthouse transition and waits ticks. */
static inline void scWorldMapShowTransitionPicture(
	Uint8 *buf, Sint32 ofs, Sint32 ticks)
{
	WORLD_MAP_QUEUE_TRANSITION_PICTURE(buf, ofs);
	fdScriptWait(ticks);
}

/* The lighthouse transition, after a beacon is lit: on the World Map, darkened
   to a deep blue, a ball of light in the lighthouse's colours (blue for Mercury
   Lighthouse) swells over the lighthouse's top, glows there for 140 ticks and
   shrinks away. The map brightens again as the camera shakes with a rumble,
   then the screen closes for the aerie, its beacon lit
   (https://alchemy.pascalpixel.com/story/venus-lighthouse-lit.gif).

   The ball is seven ticks, tile maps drawn on BG0 in the lighthouse's colours
   (WorldMapPalettes, by WORLD_MAP_LIGHTHOUSE).

   It clears FLAG_KEEP_PARTY_POSITION and sets FLAG_SKIP_SCENE_CHANGE_SOUND. */
static void scWorldMapPlayLighthouseTransition(WORLD_MAP_LIGHTHOUSE lighthouse)
{
	Uint8 *buf = syHeapAllocEWRAM(16384);

	sySynch(1);
	gmFlagClear(FLAG_KEEP_PARTY_POSITION);
	fdWorldMapStartStillImage();
	syFieldDecompressTaggedLZ(WorldMapPackedTiles, buf);
	syFieldDecompressTaggedLZ(
		WorldMapPackedPictures, buf + LIGHT_BALL_TILES_SIZE);
	IO_WRITE_QUEUE_PUSH((Uint32)&BG_PLTT_COLOR(LIGHT_BALL_PALETTE, 0),
		(Uint32)(WorldMapPalettes +
			lighthouse * PALETTE_COLORS * sizeof(Uint16)),
		DMA_COPY_HALFWORDS(PALETTE_COLORS));
	IO_WRITE_QUEUE_PUSH(
		(Uint32)BG_CHAR_BLOCK(0) + LIGHT_BALL_FIRST_TILE * TILE_SIZE_4BPP,
		(Uint32)buf, DMA_COPY_WORDS(LIGHT_BALL_TILES_SIZE / 4));
	sySetTickEvent(scWorldMapUpdateTransitionBlend, EVENT_PASS_TICK);
	fdScriptStart();
	WORLD_MAP_QUEUE_TRANSITION_PICTURE(buf, WORLD_MAP_LIGHT_BALL(7));
	fdActorGet(MainRoutineStateWork.leaderActorId)->sprStorageKind =
		ACTOR_SPRITE_NONE;
	FieldStateWork->transitionFrm = MAP_TRANSITION_TICKS;
	fdScriptStartScreenTransition();
	fdScriptWaitTransitionTicks();
	sySndPlay(SE_GLITTER);

	WorldMapBlendWork = BLEND_ALPHA(0, 14);
	scWorldMapShowTransitionPicture(buf, WORLD_MAP_LIGHT_BALL(6), 2);
	WorldMapBlendWork = BLEND_ALPHA(0, 13);
	scWorldMapShowTransitionPicture(buf, WORLD_MAP_LIGHT_BALL(5), 2);
	WorldMapBlendWork = BLEND_ALPHA(0, 12);
	scWorldMapShowTransitionPicture(buf, WORLD_MAP_LIGHT_BALL(4), 2);
	WorldMapBlendWork = BLEND_ALPHA(0, 11);
	scWorldMapShowTransitionPicture(buf, WORLD_MAP_LIGHT_BALL(3), 2);
	WorldMapBlendWork = BLEND_ALPHA(0, 10);
	scWorldMapShowTransitionPicture(buf, WORLD_MAP_LIGHT_BALL(2), 2);
	WorldMapBlendWork = BLEND_ALPHA(0, 9);
	scWorldMapShowTransitionPicture(buf, WORLD_MAP_LIGHT_BALL(1), 2);
	WorldMapBlendWork = BLEND_ALPHA(0, 8);
	scWorldMapShowTransitionPicture(buf, WORLD_MAP_LIGHT_BALL(0), 140);

	scWorldMapShowTransitionPicture(buf, WORLD_MAP_LIGHT_BALL(1), 4);
	scWorldMapShowTransitionPicture(buf, WORLD_MAP_LIGHT_BALL(2), 4);
	scWorldMapShowTransitionPicture(buf, WORLD_MAP_LIGHT_BALL(3), 4);
	WorldMapBlendWork = BLEND_ALPHA(0, 9);
	scWorldMapShowTransitionPicture(buf, WORLD_MAP_LIGHT_BALL(4), 4);
	WorldMapBlendWork = BLEND_ALPHA(0, 10);
	scWorldMapShowTransitionPicture(buf, WORLD_MAP_LIGHT_BALL(5), 4);
	WorldMapBlendWork = BLEND_ALPHA(0, 11);
	scWorldMapShowTransitionPicture(buf, WORLD_MAP_LIGHT_BALL(6), 4);
	WorldMapBlendWork = BLEND_ALPHA(0, 12);
	WORLD_MAP_QUEUE_TRANSITION_PICTURE(buf, WORLD_MAP_LIGHT_BALL(7));
	fdWorldMapLoadAreaGraphics();
	sySetTickEvent(scWorldMapJitterTransitionCamera, EVENT_PASS_TICK);
	sySndPlay(SE_RUMBLE);
	WorldMapBlendWork = BLEND_ALPHA(0, 13);
	fdScriptWait(4);
	WorldMapBlendWork = BLEND_ALPHA(0, 14);
	fdScriptWait(4);
	WorldMapBlendWork = BLEND_ALPHA(0, 15);
	fdScriptWait(4);
	WorldMapBlendWork = BLEND_ALPHA(0, 16);
	fdScriptWait(45);
	fdScriptFinishScreenTransition();
	fdScriptWaitTransitionTicks();
	syHeapRelease(buf);
	gmFlagSet(FLAG_SKIP_SCENE_CHANGE_SOUND);
}

/* Unused: an actor's update function that flashes it white, two ticks on and
   two off, and every sixteenth tick creates its attached effects. */
UNUSED static void scWorldMapUpdateFlashingLinkedEffects(ACTOR *actor)
{
	if((TickCount & 2) != 0) {
		fdActorSetColMode(actor, SPRITE_COLOR_WHITE);
	} else {
		fdActorSetColMode(actor, SPRITE_COLOR_NORMAL);
	}
	if((TickCount & 15) == 0) {
		scSpawnGoldenRing(actor);
	}
}

/* Unused: an actor's update function that cycles its colour modes and every
   sixteenth tick creates its attached effects. */
UNUSED static void scWorldMapUpdateCyclingLinkedEffects(ACTOR *actor)
{
	if((TickCount & 1) != 0) {
		fdActorSetColMode(actor, (TickCount >> 1) % 6);
	}
	if((TickCount & 15) == 0) {
		scSpawnGoldenRing(actor);
	}
}

#include "shared/color_mode_cycle.c"

#include "shared/field_effect_vertical.c"
#include "shared/golden_ring.c"

/* Hammet's caravan outside Vault (scWorldMapShowHammetsCaravan): each of the
   four wagons, at one and a half times its size, rolls up to the party and
   stops in its own place. */
Sint32 CaravanFirstWagonArrives[] = {
	SC_POSITION(5848, 0, 1576),
	SC_SET(SW_SCALE_X, toFIXED(3) / 2),
	SC_SET(SW_SCALE_Y, toFIXED(3) / 2),
	SC_WAIT(1),
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE(5848, 0, 1544),
	SC_SYNC,
	SC_MOVE(5795, 0, 1544),
	SC_SYNC,
	SC_MOVE(5800, 0, 1614),
	SC_SYNC,
	SC_MOVE(5864, 0, 1624),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_END,
};

/* Each wagon rolls south for the bridge and, out of sight, is put down where it
   waits at the broken bridge. */
Sint32 CaravanFirstWagonDeparts[] = {
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE(5868, 0, 1624),
	SC_SYNC,
	SC_MOVE(5868, 0, 1736),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_POSITION(5656, 0, 1800),
	SC_END,
};

/* Each wagon rolls back from the broken bridge and leaves the stage. */
Sint32 CaravanFirstWagonReturns[] = {
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE(5800, 0, 1800),
	SC_SYNC,
	SC_POSITION(0, 0, 0),
	SC_END,
};

/* Every wagon in turn: from the road south of Vault it rolls north for Lunpa
   and leaves the stage. */
Sint32 CaravanWagonHeadsNorth[] = {
	SC_POSITION(5868, 0, 1704),
	SC_WAIT(1),
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE(5872, 0, 1528),
	SC_SYNC,
	SC_MOVE(5880, 0, 1352),
	SC_SYNC,
	SC_POSITION(0, 0, 0),
	SC_END,
};

/* The second to fourth wagons stop in a column 32 pixels west of the first, the
   second southmost and each after it 16 pixels north. */
Sint32 CaravanSecondWagonArrives[] = {
	SC_POSITION(5848, 0, 1576),
	SC_SET(SW_SCALE_X, toFIXED(3) / 2),
	SC_SET(SW_SCALE_Y, toFIXED(3) / 2),
	SC_WAIT(1),
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE(5848, 0, 1544),
	SC_SYNC,
	SC_MOVE(5795, 0, 1544),
	SC_SYNC,
	SC_MOVE(5800, 0, 1630),
	SC_SYNC,
	SC_MOVE(5832, 0, 1640),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_END,
};

/* The second to fourth wagons wait side by side north of the first, west to
   east. */
Sint32 CaravanSecondWagonDeparts[] = {
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE(5863, 0, 1640),
	SC_SYNC,
	SC_MOVE(5868, 0, 1736),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_POSITION(5640, 0, 1768),
	SC_END,
};

/* The second to fourth wagons each roll south to the first's road and follow
   it. */
Sint32 CaravanSecondWagonReturns[] = {
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE(5640, 0, 1795),
	SC_SYNC,
	SC_MOVE(5800, 0, 1800),
	SC_SYNC,
	SC_POSITION(0, 0, 0),
	SC_END,
};

Sint32 CaravanThirdWagonArrives[] = {
	SC_POSITION(5848, 0, 1576),
	SC_SET(SW_SCALE_X, toFIXED(3) / 2),
	SC_SET(SW_SCALE_Y, toFIXED(3) / 2),
	SC_WAIT(1),
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE(5848, 0, 1544),
	SC_SYNC,
	SC_MOVE(5795, 0, 1544),
	SC_SYNC,
	SC_MOVE(5800, 0, 1614),
	SC_SYNC,
	SC_MOVE(5832, 0, 1624),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_END,
};

Sint32 CaravanThirdWagonDeparts[] = {
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE(5863, 0, 1624),
	SC_SYNC,
	SC_MOVE(5868, 0, 1736),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_POSITION(5656, 0, 1768),
	SC_END,
};

Sint32 CaravanThirdWagonReturns[] = {
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE(5656, 0, 1795),
	SC_SYNC,
	SC_MOVE(5800, 0, 1800),
	SC_SYNC,
	SC_POSITION(0, 0, 0),
	SC_END,
};

Sint32 CaravanFourthWagonArrives[] = {
	SC_POSITION(5848, 0, 1576),
	SC_SET(SW_SCALE_X, toFIXED(3) / 2),
	SC_SET(SW_SCALE_Y, toFIXED(3) / 2),
	SC_WAIT(1),
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE(5848, 0, 1544),
	SC_SYNC,
	SC_MOVE(5795, 0, 1544),
	SC_SYNC,
	SC_MOVE(5800, 0, 1598),
	SC_SYNC,
	SC_MOVE(5832, 0, 1608),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_END,
};

Sint32 CaravanFourthWagonDeparts[] = {
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE(5863, 0, 1608),
	SC_SYNC,
	SC_MOVE(5868, 0, 1736),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_POSITION(5672, 0, 1768),
	SC_END,
};

Sint32 CaravanFourthWagonReturns[] = {
	/* ANIM_NOD where the others take ANIM_WALK. The wagon's nod (wagon.png)
	   holds its standing picture, so this wagon glides back to the road without
	   the walk's rolling pictures. */
	SC_ANIMATION(ANIM_NOD),
	SC_MOVE(5672, 0, 1795),
	SC_SYNC,
	SC_MOVE(5800, 0, 1800),
	SC_SYNC,
	SC_POSITION(0, 0, 0),
	SC_END,
};

/* The invisible guide of the rock that falls beside the caravan: it grows from
   a tenth of its size over 160 ticks as it moves to where the rock lands. */
Sint32 CaravanRockGuideScript[] = {
	SC_SET(SW_SCALE_X, toFIXED(1) / 10),
	SC_SET(SW_SCALE_Y, toFIXED(1) / 10),
	SC_WAIT(1),
	SC_MOVE(5608, 0, 1720),
	SC_LABEL(1),
	SC_ADD(SW_SCALE_X, toFIXED(1) / 100),
	SC_ADD(SW_SCALE_Y, toFIXED(1) / 100),
	SC_WAIT(1),
	SC_LOOP(160, SC_TO(1)),
	SC_END,
};

/* The leader's walk in scWorldMapRunBlackOrbScript: SW_TIMER turns 1 when he
   has stopped. */
Sint32 BlackOrbLeaderScript[] = {
	SC_SET(SW_TIMER, 0),
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE(6008, 0, 3400),
	SC_SYNC,
	SC_MOVE(6020, 0, 3400),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_SET(SW_TIMER, 1),
	SC_WAIT(1),
	SC_END,
};

/* The walks of the ending (scWorldMapShowEndingScript): Alex, Kraden and Jenna
   follow one path and stop side by side; Jenna's turns SW_TIMER to 1 when she
   has stopped. */
Sint32 EndingAlexWalk[] = {
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE(7564, 0, 3476),
	SC_SYNC,
	SC_MOVE(7576, 0, 3464),
	SC_SYNC,
	SC_MOVE(7640, 0, 3464),
	SC_SYNC,
	SC_MOVE(7656, 0, 3480),
	SC_SYNC,
	SC_MOVE(7720, 0, 3480),
	SC_SYNC,
	SC_MOVE(7752, 0, 3496),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_SET(SW_FACING, FACING_SOUTH_SOUTHEAST),
	SC_WAIT(1),
	SC_END,
};

Sint32 EndingKradenWalk[] = {
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE(7564, 0, 3476),
	SC_SYNC,
	SC_MOVE(7576, 0, 3464),
	SC_SYNC,
	SC_MOVE(7640, 0, 3464),
	SC_SYNC,
	SC_MOVE(7656, 0, 3480),
	SC_SYNC,
	SC_MOVE(7704, 0, 3512),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_SET(SW_FACING, FACING_EAST),
	SC_WAIT(1),
	SC_END,
};

Sint32 EndingJennaWalk[] = {
	SC_SET(SW_TIMER, 0),
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE(7564, 0, 3476),
	SC_SYNC,
	SC_MOVE(7576, 0, 3464),
	SC_SYNC,
	SC_MOVE(7640, 0, 3464),
	SC_SYNC,
	SC_MOVE(7656, 0, 3480),
	SC_SYNC,
	SC_MOVE(7688, 0, 3528),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_SET(SW_FACING, FACING_EAST),
	SC_SET(SW_TIMER, 1),
	SC_WAIT(1),
	SC_END,
};

/* A cloud of dust in the lighthouse quake (scWorldMapUpdateLighthouseQuake): it
   lasts 16 ticks. */
Sint32 QuakeCloudScript[] = {
	SC_WAIT(16),
	SC_RELEASE,
};

/* The ship crossing the sea (scWorldMapSailCrossingLeg,
   scWorldMapSailUntilLandSighted): 8 pixels west a step for 40 steps, then
   SW_TIMER turns 1 while it sails on two more. */
Sint32 CrossingShipSailsWest[] = {
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE_BY(-8, 0, 0),
	SC_SYNC,
	SC_LOOP(40, SC_TOP),
	SC_SET(SW_TIMER, 1),
	SC_MOVE_BY(-8, 0, 0),
	SC_SYNC,
	SC_MOVE_BY(-8, 0, 0),
	SC_SYNC,
	SC_END,
};

/* As CrossingShipSailsWest, drifting 2 pixels north with each step. */
Sint32 CrossingShipDriftsNorth[] = {
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE_BY(-8, 0, -2),
	SC_SYNC,
	SC_LOOP(40, SC_TOP),
	SC_SET(SW_TIMER, 1),
	SC_MOVE_BY(-8, 0, -2),
	SC_SYNC,
	SC_MOVE_BY(-8, 0, -2),
	SC_SYNC,
	SC_END,
};

/* The island drifting east (scWorldMapShowIslandDrifting): 24 pixels a step for
   10 steps, then SW_TIMER turns 1 while it drifts on two more. */
Sint32 IslandDriftsEast[] = {
	SC_MOVE_BY(24, 0, 0),
	SC_SYNC,
	SC_LOOP(10, SC_TOP),
	SC_SET(SW_TIMER, 1),
	SC_MOVE_BY(24, 0, 0),
	SC_SYNC,
	SC_MOVE_BY(24, 0, 0),
	SC_SYNC,
	SC_END,
};

/* Flint's actor script on the map: it sets him up (scWorldMapSetUpFlint). */
Sint32 WorldMapFlintScript[] = {
	SC_CALL(scWorldMapSetUpFlint),
	SC_END,
};

SCENE_ENTRANCE WorldSceneEntrances[] = {
	{ENTRANCE_DEFAULT, CONDITION_ALWAYS, 5656, 0, 1256, FACING_SOUTH, 0, -1, -1,
		-1, -1, 0},
	{WORLD_MAP_FROM_VALE_PLAZA, CONDITION_ALWAYS, 5680, 0, 1226, FACING_SOUTH,
		0, -1, -1, -1, -1, 0},
	{WORLD_MAP_FROM_TITLE, CONDITION_ALWAYS, 5688, 0, 1224, FACING_SOUTH, 0, -1,
		-1, -1, -1, 0},
	{WORLD_MAP_FROM_VAULT_SOUTH, CONDITION_ALWAYS, 5848, 0, 1608, FACING_SOUTH,
		0, -1, -1, -1, -1, 0},
	{WORLD_MAP_FROM_GOMA_CAVE_ENTRANCE, CONDITION_ALWAYS, 6424, 0, 1320,
		FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{WORLD_MAP_FROM_BILIBIN, CONDITION_ALWAYS, 6584, 0, 1224, FACING_SOUTH, 0,
		-1, -1, -1, -1, 0},
	{WORLD_MAP_FROM_BILIBIN_BARRICADE_EAST, CONDITION_ALWAYS, 6928, 0, 1368,
		FACING_WEST, 0, -1, -1, -1, -1, 0},
	{WORLD_MAP_FROM_KOLIMA_WEST, CONDITION_ALWAYS, 7328, 0, 1240, FACING_WEST,
		0, -1, -1, -1, -1, 0},
	{WORLD_MAP_FROM_KOLIMA_FOREST_ENTRANCE, CONDITION_ALWAYS, 7136, 0, 1054,
		FACING_WEST, 0, -1, -1, -1, -1, 0},
	{WORLD_MAP_FROM_IMIL_SOUTH, CONDITION_ALWAYS, 5928, 0, 296, FACING_SOUTH, 0,
		-1, -1, -1, -1, 0},
	{WORLD_MAP_FROM_MERCURY_LIGHTHOUSE_EXTERIOR_EAST, CONDITION_ALWAYS, 6024, 0,
		200, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{WORLD_MAP_FROM_KOLIMA_BRIDGE_SOUTH, CONDITION_ALWAYS, 7568, 0, 1416,
		FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{WORLD_MAP_FROM_FUCHIN_TEMPLE_GROUNDS, CONDITION_ALWAYS, 7880, 0, 1592,
		FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{WORLD_MAP_FROM_MOGALL_FOREST_SIGN, CONDITION_ALWAYS, 7852, 0, 1688,
		FACING_NORTH, 0, -1, -1, -1, -1, 0},
	{WORLD_MAP_FROM_XIAN, CONDITION_ALWAYS, 7544, 0, 2008, FACING_SOUTH, 0, -1,
		-1, -1, -1, 0},
	{WORLD_MAP_FROM_ALPINE_CROSSING_EAST, CONDITION_ALWAYS, 7144, 0, 1736,
		FACING_EAST, 0, -1, -1, -1, -1, 0},
	{WORLD_MAP_FROM_ALTIN, CONDITION_ALWAYS, 7008, 0, 1624, FACING_EAST, 0, -1,
		-1, -1, -1, 0},
	{WORLD_MAP_FROM_LAMA_TEMPLE, CONDITION_ALWAYS, 6888, 0, 1736, FACING_SOUTH,
		0, -1, -1, -1, -1, 0},
	{WORLD_MAP_FROM_LAMAKAN_DESERT_EAST, CONDITION_ALWAYS, 6888, 0, 1828,
		FACING_EAST, 0, -1, -1, -1, -1, 0},
	{WORLD_MAP_FROM_KALAY_WEST, CONDITION_ALWAYS, 5560, 0, 2136, FACING_WEST, 0,
		-1, -1, -1, -1, 0},
	{WORLD_MAP_FROM_KALAY_EAST, CONDITION_ALWAYS, 5592, 0, 2168, FACING_SOUTH,
		0, -1, -1, -1, -1, 0},
	{WORLD_MAP_FROM_KALAY_DOCKS_SOUTH, CONDITION_ALWAYS, 5424, 0, 2264,
		FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{WORLD_MAP_FROM_TOLBI_DOCKS, CONDITION_ALWAYS, 5004, 0, 2328, FACING_WEST,
		0, -1, -1, -1, -1, 0},
	{WORLD_MAP_FROM_TOLBI, CONDITION_ALWAYS, 4904, 0, 2312, FACING_SOUTH, 0, -1,
		-1, -1, -1, 0},
	{WORLD_MAP_FROM_ALTMILLER_CAVE_ENTRANCE, CONDITION_ALWAYS, 4840, 0, 1992,
		FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{WORLD_MAP_FROM_GONDOWAN_PASSAGE_NORTH, CONDITION_ALWAYS, 4940, 0, 2648,
		FACING_NORTH, 0, -1, -1, -1, -1, 0},
	{WORLD_MAP_FROM_GONDOWAN_CAVE_NORTH, CONDITION_ALWAYS, 5656, 0, 2424,
		FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{WORLD_MAP_FROM_SUHALLA, CONDITION_ALWAYS, 5388, 0, 2840, FACING_WEST, 0,
		-1, -1, -1, -1, 0},
	{WORLD_MAP_FROM_SUHALLA_DESERT_NORTH, CONDITION_ALWAYS, 5448, 0, 2920,
		FACING_NORTH, 0, -1, -1, -1, -1, 0},
	{WORLD_MAP_FROM_SUHALLA_GATE_CHECKPOINT_SOUTH, CONDITION_ALWAYS, 5864, 0,
		3400, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{WORLD_MAP_FROM_LALIVERO_EAST, CONDITION_ALWAYS, 5996, 0, 2930,
		FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{WORLD_MAP_FROM_ROAD_TO_BABI_LIGHTHOUSE, CONDITION_ALWAYS, 5996, 0, 2840,
		FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{WORLD_MAP_FROM_VENUS_LIGHTHOUSE_ENTRY_EAST, CONDITION_ALWAYS, 6032, 0,
		3192, FACING_WEST, 0, -1, -1, -1, -1, 0},
	{WORLD_MAP_FROM_GATEWAY_CAVE, CONDITION_ALWAYS, 5976, 0, 3432, FACING_SOUTH,
		0, -1, -1, -1, -1, 0},
	{WORLD_MAP_FROM_LUNPA, CONDITION_ALWAYS, 5896, 0, 1160, FACING_SOUTH, 0, -1,
		-1, -1, -1, 0},
	{WORLD_MAP_FROM_BILIBIN_CAVE_ENTRANCE, CONDITION_ALWAYS, 5990, 0, 804,
		FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{WORLD_MAP_FROM_KOLIMA_EAST, CONDITION_ALWAYS, 7376, 0, 1240, FACING_EAST,
		0, -1, -1, -1, -1, 0},
	{WORLD_MAP_FROM_IMIL_NORTH, CONDITION_ALWAYS, 5956, 0, 266, FACING_EAST, 0,
		-1, -1, -1, -1, 0},
	{WORLD_MAP_FROM_GOMA_CAVE_BRIDGE, CONDITION_ALWAYS, 6484, 0, 1212,
		FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{WORLD_MAP_FROM_GONDOWAN_CAVE_SOUTH, CONDITION_ALWAYS, 5480, 0, 2632,
		FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{WORLD_MAP_FROM_SUHALLA_GATE_CHECKPOINT_EAST, CONDITION_ALWAYS, 5928, 0,
		3272, FACING_EAST, 0, -1, -1, -1, -1, 0},
	{WORLD_MAP_FROM_MOGALL_FOREST_KILLER_APE, CONDITION_ALWAYS, 7792, 0, 2104,
		FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{WORLD_MAP_FROM_VAULT_NORTH, CONDITION_ALWAYS, 5848, 0, 1544, FACING_NORTH,
		0, -1, -1, -1, -1, 0},
	/* Unused: no exit leads to entrance 45, 97 or 98. */
	{45, CONDITION_ALWAYS, 6248, 0, 520, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{WORLD_MAP_FROM_KOLIMA_BRIDGE_NORTH, CONDITION_ALWAYS, 7568, 0, 1352,
		FACING_NORTH, 0, -1, -1, -1, -1, 0},
	{WORLD_MAP_FROM_BILIBIN_BARRICADE_WEST, CONDITION_ALWAYS, 6976, 0, 1360,
		FACING_EAST, 0, -1, -1, -1, -1, 0},
	{WORLD_MAP_FROM_BILIBIN_CAVE_ICE, CONDITION_ALWAYS, 6228, 0, 552,
		FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{WORLD_MAP_FROM_WEST_LUNPA_CAVE, CONDITION_ALWAYS, 5860, 0, 1212,
		FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{WORLD_MAP_FROM_ALPINE_CROSSING_NORTH, CONDITION_ALWAYS, 7056, 0, 1656,
		FACING_NORTH, 0, -1, -1, -1, -1, 0},
	{WORLD_MAP_FROM_ALPINE_CROSSING_WEST, CONDITION_ALWAYS, 7000, 0, 1736,
		FACING_WEST, 0, -1, -1, -1, -1, 0},
	{WORLD_MAP_FROM_ALTIN_PEAK_WEST_EXIT, CONDITION_ALWAYS, 6920, 0, 1696,
		FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{WORLD_MAP_FROM_LAMAKAN_DESERT_CAVE, CONDITION_ALWAYS, 6090, 0, 2232,
		FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{WORLD_MAP_FROM_GONDOWAN_PASSAGE_SOUTH, CONDITION_ALWAYS, 4940, 0, 2728,
		FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{WORLD_MAP_FROM_SUHALLA_DESERT_SOUTH, CONDITION_ALWAYS, 5416, 0, 3272,
		FACING_EAST, 0, -1, -1, -1, -1, 0},
	{WORLD_MAP_FROM_SUHALLA_NORTH, CONDITION_ALWAYS, 5416, 0, 2808,
		FACING_NORTH, 0, -1, -1, -1, -1, 0},
	{WORLD_MAP_FROM_SUHALLA_EAST, CONDITION_ALWAYS, 5444, 0, 2840, FACING_EAST,
		0, -1, -1, -1, -1, 0},
	{WORLD_MAP_FROM_KALAY_DOCKS_NORTH, CONDITION_ALWAYS, 5424, 0, 2200,
		FACING_NORTH, 0, -1, -1, -1, -1, 0},
	{WORLD_MAP_FROM_LALIVERO_WEST, CONDITION_ALWAYS, 5996, 0, 2856,
		FACING_NORTH, 0, -1, -1, -1, -1, 0},
	{WORLD_MAP_FROM_KALAY_TUNNEL_ENTRANCE_NORTH, CONDITION_ALWAYS, 5480, 0,
		2120, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{WORLD_MAP_FROM_VENUS_LIGHTHOUSE_ENTRY_WEST, CONDITION_ALWAYS, 6152, 0,
		3212, FACING_EAST, 0, -1, -1, -1, -1, 0},
	{WORLD_MAP_HAMMETS_CARAVAN_LEAVES, CONDITION_ALWAYS, 5848, 0, 1608,
		FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{WORLD_MAP_FROM_SHIP_ROOMS_SAIL_TO_FIRST_ATTACK, CONDITION_ALWAYS, 5384, 0,
		2248, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{WORLD_MAP_FROM_SHIP_ROOMS_SAIL_TO_SECOND_ATTACK, CONDITION_ALWAYS, 7944, 0,
		200, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{WORLD_MAP_FROM_SHIP_ROOMS_SAIL_TO_THIRD_ATTACK, CONDITION_ALWAYS, 7944, 0,
		200, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{WORLD_MAP_FROM_SHIP_ROOMS_SAIL_TO_KRAKEN, CONDITION_ALWAYS, 7944, 0, 200,
		FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	/* The one row with a camera bound: the view goes no farther north than 128
	   pixels, where the ship blown off course drifts north
	   (CrossingShipDriftsNorth). */
	{WORLD_MAP_FROM_SHIP_ROOMS_SAIL_TO_LAND, CONDITION_ALWAYS, 7944, 0, 200,
		FACING_SOUTH, 0, -1, 128, -1, -1, 0},
	{WORLD_MAP_FROM_TOLBI_SHIP_DECK_REACH_TOLBI, CONDITION_ALWAYS, 5096, 0,
		2328, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{WORLD_MAP_FROM_KALAY_TUNNEL_ENTRANCE_SOUTH, CONDITION_ALWAYS, 5528, 0,
		2120, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{WORLD_MAP_FROM_FILE_SCREEN, CONDITION_ALWAYS, 7464, 0, 200, FACING_SOUTH,
		0, -1, -1, -1, -1, 0},
	{WORLD_MAP_BEFORE_SHEBAS_FALL, CONDITION_ALWAYS, 6088, 0, 3176, FACING_WEST,
		0, -1, -1, -1, -1, 0},
	{WORLD_MAP_FROM_TOLBI_SHIP_DECK_CROSSBONE_ISLE, CONDITION_ALWAYS, 4728, 0,
		424, FACING_WEST, 0, -1, -1, -1, -1, 0},
	{WORLD_MAP_FROM_SHIP_ROOMS, CONDITION_ALWAYS, 4872, 0, 808, FACING_WEST, 0,
		-1, -1, -1, -1, 0},
	{WORLD_MAP_FROM_CROSSBONE_ISLE, CONDITION_ALWAYS, 4576, 0, 508,
		FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{WORLD_MAP_FROM_SUHALLA_DESERT_SANDSTORM, CONDITION_ALWAYS, 4392, 0, 328,
		FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{WORLD_MAP_AFTER_BLACK_ORB_DEFEAT, CONDITION_ALWAYS, 8, 0, 8, FACING_SOUTH,
		0, -1, -1, -1, -1, 0},
	{WORLD_MAP_JENNA_AND_KRADEN_ADRIFT, CONDITION_ALWAYS, 7552, 0, 3504,
		FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{WORLD_MAP_AFTER_MERCURY_LIGHTHOUSE_LIT, CONDITION_ALWAYS, 6024, 0, 160,
		FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{WORLD_MAP_AFTER_VENUS_LIGHTHOUSE_LIT, CONDITION_ALWAYS, 6088, 0, 3168,
		FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	/* Unused. */
	{97, CONDITION_ALWAYS, 6248, 0, 872, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	/* Unused. */
	{98, CONDITION_ALWAYS, 4248, 0, 2424, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{WORLD_MAP_FROM_DEBUG, CONDITION_ALWAYS, 5576, 0, 2088, FACING_SOUTH, 0, -1,
		-1, -1, -1, 0},
	{SCENE_TABLE_END},
};

Uint32 WorldMapExits[] = {
	SCENE_EXITS(WorldMap),
	SCENE_EXIT(WORLD_MAP_TO_VALE_PLAZA, ValePlaza, VALE_PLAZA_FROM_WORLD_MAP),
	SCENE_EXIT(
		WORLD_MAP_TO_WORLD_MAP_FROM_TITLE, WorldMap, WORLD_MAP_FROM_TITLE),
	SCENE_EXIT(WORLD_MAP_TO_VAULT_SOUTH, Vault, VAULT_FROM_WORLD_MAP_SOUTH),
	SCENE_EXIT(WORLD_MAP_TO_GOMA_CAVE_ENTRANCE, GomaCaveEntrance,
		GOMA_CAVE_ENTRANCE_FROM_WORLD_MAP),
	SCENE_EXIT(WORLD_MAP_TO_BILIBIN, Bilibin, BILIBIN_FROM_WORLD_MAP),
	SCENE_EXIT(WORLD_MAP_TO_BILIBIN_BARRICADE_WEST, BilibinBarricade,
		BILIBIN_BARRICADE_FROM_WORLD_MAP_WEST),
	SCENE_EXIT(WORLD_MAP_TO_KOLIMA_WEST, Kolima, KOLIMA_FROM_WORLD_MAP),
	SCENE_EXIT(WORLD_MAP_TO_KOLIMA_FOREST_ENTRANCE, KolimaForestEntrance,
		KOLIMA_FOREST_ENTRANCE_FROM_WORLD_MAP),
	SCENE_EXIT(WORLD_MAP_TO_IMIL_SOUTH, Imil, IMIL_FROM_HOUSE_SOUTHWEST),
	SCENE_EXIT(WORLD_MAP_TO_MERCURY_LIGHTHOUSE_EXTERIOR_EAST,
		MercuryLighthouseExterior, MERCURY_LIGHTHOUSE_EXTERIOR_FROM_WORLD_MAP),
	SCENE_EXIT(WORLD_MAP_TO_KOLIMA_BRIDGE_SOUTH, KolimaBridge,
		KOLIMA_BRIDGE_FROM_WORLD_MAP_SOUTH),
	SCENE_EXIT(WORLD_MAP_TO_FUCHIN_TEMPLE_GROUNDS, FuchinTempleGrounds,
		FUCHIN_TEMPLE_GROUNDS_FROM_WORLD_MAP),
	SCENE_EXIT(WORLD_MAP_TO_MOGALL_FOREST_SIGN, MogallForestSign,
		MOGALL_FOREST_SIGN_FROM_WORLD_MAP),
	SCENE_EXIT(WORLD_MAP_TO_XIAN, Xian, XIAN_FROM_HOUSE),
	SCENE_EXIT(WORLD_MAP_TO_ALPINE_CROSSING_EAST, AlpineCrossing,
		ALPINE_CROSSING_FROM_WORLD_MAP_EAST),
	SCENE_EXIT(WORLD_MAP_TO_ALTIN, Altin, ALTIN_FROM_WORLD_MAP),
	SCENE_EXIT(
		WORLD_MAP_TO_LAMA_TEMPLE, LamaTemple, LAMA_TEMPLE_FROM_WORLD_MAP),
	SCENE_EXIT(WORLD_MAP_TO_LAMAKAN_DESERT_EAST, LamakanDesertEast,
		LAMAKAN_DESERT_EAST_FROM_WORLD_MAP),
	SCENE_EXIT(WORLD_MAP_TO_WORLD_MAP_FROM_KALAY_WEST, WorldMap,
		WORLD_MAP_FROM_KALAY_WEST),
	SCENE_EXIT(WORLD_MAP_TO_KALAY_EAST, Kalay, KALAY_FROM_WORLD_MAP),
	SCENE_EXIT(WORLD_MAP_TO_KALAY_DOCKS_SOUTH, KalayDocks,
		KALAY_DOCKS_FROM_WORLD_MAP_SOUTH),
	SCENE_EXIT(
		WORLD_MAP_TO_TOLBI_DOCKS, TolbiDocks, TOLBI_DOCKS_FROM_WORLD_MAP),
	SCENE_EXIT(WORLD_MAP_TO_TOLBI, Tolbi, TOLBI_FROM_WORLD_MAP),
	SCENE_EXIT(WORLD_MAP_TO_ALTMILLER_CAVE_ENTRANCE, AltmillerCaveEntrance,
		ALTMILLER_CAVE_ENTRANCE_FROM_WORLD_MAP),
	SCENE_EXIT(WORLD_MAP_TO_GONDOWAN_PASSAGE_NORTH, GondowanPassage,
		GONDOWAN_PASSAGE_FROM_WORLD_MAP_NORTH),
	SCENE_EXIT(WORLD_MAP_TO_GONDOWAN_CAVE_NORTH, GondowanCave,
		GONDOWAN_CAVE_FROM_WORLD_MAP_EAST),
	SCENE_EXIT(WORLD_MAP_TO_SUHALLA, Suhalla, SUHALLA_FROM_WORLD_MAP),
	SCENE_EXIT(WORLD_MAP_TO_SUHALLA_DESERT_NORTH, SuhallaDesertNorth,
		SUHALLA_DESERT_NORTH_FROM_WORLD_MAP),
	SCENE_EXIT(WORLD_MAP_TO_SUHALLA_GATE_CHECKPOINT_SOUTH,
		SuhallaGateCheckpoint, SUHALLA_GATE_CHECKPOINT_FROM_WORLD_MAP_SOUTH),
	SCENE_EXIT(
		WORLD_MAP_TO_LALIVERO_EAST, Lalivero, LALIVERO_FROM_WORLD_MAP_EAST),
	SCENE_EXIT(WORLD_MAP_TO_ROAD_TO_BABI_LIGHTHOUSE, RoadToBabiLighthouse,
		ROAD_TO_BABI_LIGHTHOUSE_FROM_WORLD_MAP),
	SCENE_EXIT(WORLD_MAP_TO_VENUS_LIGHTHOUSE_ENTRY_WEST, VenusLighthouseEntry,
		VENUS_LIGHTHOUSE_ENTRY_FROM_WORLD_MAP_WEST),
	SCENE_EXIT(
		WORLD_MAP_TO_GATEWAY_CAVE, GatewayCave, GATEWAY_CAVE_FROM_WORLD_MAP),
	SCENE_EXIT(WORLD_MAP_TO_LUNPA, Lunpa, LUNPA_FROM_WORLD_MAP),
	SCENE_EXIT(WORLD_MAP_TO_BILIBIN_CAVE_ENTRANCE, BilibinCaveEntrance,
		BILIBIN_CAVE_ENTRANCE_FROM_WORLD_MAP),
	SCENE_EXIT(WORLD_MAP_TO_GOMA_CAVE_BRIDGE, GomaCaveBridge,
		GOMA_CAVE_BRIDGE_FROM_WORLD_MAP),
	SCENE_EXIT(WORLD_MAP_TO_GONDOWAN_CAVE_SOUTH, GondowanCave,
		GONDOWAN_CAVE_FROM_WORLD_MAP_WEST),
	SCENE_EXIT(WORLD_MAP_TO_SUHALLA_GATE_CHECKPOINT_EAST, SuhallaGateCheckpoint,
		SUHALLA_GATE_CHECKPOINT_FROM_WORLD_MAP_EAST),
	SCENE_EXIT(WORLD_MAP_TO_MOGALL_FOREST_KILLER_APE, MogallForestKillerApe,
		MOGALL_FOREST_KILLER_APE_FROM_WORLD_MAP),
	SCENE_EXIT(WORLD_MAP_TO_VAULT_NORTH, Vault, VAULT_FROM_WORLD_MAP_NORTH),
	SCENE_EXIT(WORLD_MAP_TO_BILIBIN_CAVE_ENTRANCE_NORTHEAST,
		BilibinCaveEntrance, BILIBIN_CAVE_ENTRANCE_FROM_WORLD_MAP),
	SCENE_EXIT(WORLD_MAP_TO_KOLIMA_BRIDGE_NORTH, KolimaBridge,
		KOLIMA_BRIDGE_FROM_WORLD_MAP_NORTH),
	SCENE_EXIT(WORLD_MAP_TO_BILIBIN_BARRICADE_EAST, BilibinBarricade,
		BILIBIN_BARRICADE_FROM_WORLD_MAP_EAST),
	SCENE_EXIT(WORLD_MAP_TO_BILIBIN_CAVE_ICE, BilibinCaveIce,
		BILIBIN_CAVE_ICE_FROM_WORLD_MAP),
	SCENE_EXIT(WORLD_MAP_TO_WEST_LUNPA_CAVE, WestLunpaCave,
		WEST_LUNPA_CAVE_FROM_WORLD_MAP),
	SCENE_EXIT(WORLD_MAP_TO_ALPINE_CROSSING_NORTH, AlpineCrossing,
		ALPINE_CROSSING_FROM_WORLD_MAP_NORTH),
	SCENE_EXIT(WORLD_MAP_TO_ALPINE_CROSSING_WEST, AlpineCrossing,
		ALPINE_CROSSING_FROM_WORLD_MAP_WEST),
	SCENE_EXIT(WORLD_MAP_TO_ALTIN_PEAK_WEST_EXIT, AltinPeakWestExit,
		ALTIN_PEAK_WEST_EXIT_FROM_WORLD_MAP),
	SCENE_EXIT(WORLD_MAP_TO_LAMAKAN_DESERT_CAVE, LamakanDesertCave,
		LAMAKAN_DESERT_CAVE_FROM_WORLD_MAP),
	SCENE_EXIT(WORLD_MAP_TO_GONDOWAN_PASSAGE_SOUTH, GondowanPassage,
		GONDOWAN_PASSAGE_FROM_WORLD_MAP_SOUTH),
	SCENE_EXIT(WORLD_MAP_TO_SUHALLA_DESERT_SOUTH, SuhallaDesertSouth,
		SUHALLA_DESERT_SOUTH_FROM_WORLD_MAP),
	SCENE_EXIT(WORLD_MAP_TO_KALAY_DOCKS_NORTH, KalayDocks,
		KALAY_DOCKS_FROM_WORLD_MAP_NORTH),
	SCENE_EXIT(
		WORLD_MAP_TO_LALIVERO_WEST, Lalivero, LALIVERO_FROM_WORLD_MAP_WEST),
	SCENE_EXIT(WORLD_MAP_TO_KALAY_TUNNEL_ENTRANCE, KalayTunnelEntrance,
		KALAY_TUNNEL_ENTRANCE_FROM_WORLD_MAP_NORTH),
	SCENE_EXIT(WORLD_MAP_TO_VENUS_LIGHTHOUSE_ENTRY_EAST, VenusLighthouseEntry,
		VENUS_LIGHTHOUSE_ENTRY_FROM_WORLD_MAP_EAST),
	SCENE_EXIT(WORLD_MAP_TO_TOLBI_SHIP_DECK_CROSSBONE_ISLE, TolbiShipDeck,
		TOLBI_SHIP_DECK_FROM_TOLBI_DOCKS),
	SCENE_EXIT(WORLD_MAP_TO_CROSSBONE_ISLE, CrossboneIsle,
		CROSSBONE_ISLE_FROM_WORLD_MAP),
	SCENE_EXIT(WORLD_MAP_TO_WORLD_MAP_FROM_VALE_PLAZA, WorldMap,
		WORLD_MAP_FROM_VALE_PLAZA),
	SCENE_EXIT(WORLD_MAP_TO_HAMMETS_CARAVAN_LEAVES, WorldMap,
		WORLD_MAP_HAMMETS_CARAVAN_LEAVES),
	SCENE_EXIT(WORLD_MAP_TO_TOLBI_SHIP_MAST_FIRST_ATTACK, TolbiShipMast,
		TOLBI_SHIP_MAST_FROM_WORLD_MAP_FIRST_ATTACK),
	SCENE_EXIT(WORLD_MAP_TO_TOLBI_SHIP_MAST_SECOND_ATTACK, TolbiShipMast,
		TOLBI_SHIP_MAST_FROM_WORLD_MAP_SECOND_ATTACK),
	SCENE_EXIT(WORLD_MAP_TO_TOLBI_SHIP_MAST_THIRD_ATTACK, TolbiShipMast,
		TOLBI_SHIP_MAST_FROM_WORLD_MAP_THIRD_ATTACK),
	SCENE_EXIT(WORLD_MAP_TO_SHIP_ROOMS, ShipRooms, SHIP_ROOMS_FROM_WORLD_MAP),
	SCENE_EXIT(WORLD_MAP_TO_TOLBI_SHIP_MAST_LAND_HO, TolbiShipMast,
		TOLBI_SHIP_MAST_FROM_WORLD_MAP_LAND_HO),
	SCENE_EXIT(WORLD_MAP_TO_TOLBI_SHIP_DECK, TolbiShipDeck,
		TOLBI_SHIP_DECK_FROM_WORLD_MAP),
	SCENE_EXIT(WORLD_MAP_TO_KALAY_TUNNEL_ENTRANCE_SOUTH, KalayTunnelEntrance,
		KALAY_TUNNEL_ENTRANCE_FROM_WORLD_MAP_SOUTH),
	SCENE_EXIT(WORLD_MAP_TO_VENUS_LIGHTHOUSE_AERIE, VenusLighthouseAerie,
		VENUS_LIGHTHOUSE_AERIE_FROM_WORLD_MAP),
	SCENE_EXIT(WORLD_MAP_TO_SHIP_ROOMS_ROWING_DECK_ISLAND, ShipRooms,
		SHIP_ROOMS_ROWING_DECK_FROM_WORLD_MAP_ISLAND),
	SCENE_EXIT(WORLD_MAP_TO_JENNA_AND_KRADEN_ADRIFT, WorldMap,
		WORLD_MAP_JENNA_AND_KRADEN_ADRIFT),
	SCENE_EXIT(WORLD_MAP_TO_ROCK_ROOMS, RockRooms,
		ROCK_ROOMS_FROM_SUHALLA_GATE_CHECKPOINT),
	SCENE_EXITS_END,
};

Sint32 WorldMapKolimaBridgeScript[] = {
	SC_CALL(scWorldMapSetUpKolimaBridge),
	SC_END,
};

Sint32 WorldMapAlignToMapScript[] = {
	SC_CALL(scWorldMapAlignToMap),
	SC_END,
};

Sint32 WorldMapIodemScript[] = {
	SC_CALL(scWorldMapSetUpIodem),
	SC_END,
};

Sint32 WorldMapStormScript[] = {
	SC_CALL(scWorldMapSetUpStorm),
	SC_CALL(scWorldMapHideStormOnOddTicks),
	SC_END,
};

/* clang-format off */
ACTOR_PLACE WorldMapActorPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_VENUS_DJINNI, FLAG_DJINNI(DJINNI_VINE), (Sint32)WorldMapFlintScript,
		0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_MARS_DJINNI, FLAG_DJINNI(DJINNI_CORONA), MOVEMENT_TYPE_STAND,
		0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_MERCURY_DJINNI, FLAG_DJINNI(DJINNI_HAIL), MOVEMENT_TYPE_STAND,
		0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_WORLD_MAP_VILLAGE, CONDITION_ALWAYS, (Sint32)WorldMapAlignToMapScript,
		5678, 0, 1200, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_WORLD_MAP_VILLAGE, CONDITION_ALWAYS, (Sint32)WorldMapAlignToMapScript,
		5848, 0, 1576, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_WORLD_MAP_VILLAGE, CONDITION_ALWAYS, (Sint32)WorldMapAlignToMapScript,
		6976, 0, 1624, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_WORLD_MAP_VILLAGE, CONDITION_ALWAYS, (Sint32)WorldMapAlignToMapScript,
		5416, 0, 2840, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_WORLD_MAP_VILLAGE, CONDITION_ALWAYS, (Sint32)WorldMapAlignToMapScript,
		5892, 0, 1128, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_WORLD_MAP_DOCK, CONDITION_ALWAYS, (Sint32)WorldMapAlignToMapScript,
		5422, 0, 2232, FACING_EAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_WORLD_MAP_DOCK, CONDITION_ALWAYS, (Sint32)WorldMapAlignToMapScript,
		5036, 0, 2328, FACING_WEST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_WORLD_MAP_SNOWY_VILLAGE, CONDITION_ALWAYS, (Sint32)WorldMapAlignToMapScript,
		5928, 0, 270, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_WORLD_MAP_STUMP_VILLAGE, CONDITION_ALWAYS, (Sint32)WorldMapAlignToMapScript,
		7352, 0, 1240, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_WORLD_MAP_BLUE_ROOFED_TOWN, CONDITION_ALWAYS, (Sint32)WorldMapAlignToMapScript,
		7544, 0, 1976, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_WORLD_MAP_TEMPLE, CONDITION_ALWAYS, (Sint32)WorldMapAlignToMapScript,
		6888, 0, 1712, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_WORLD_MAP_TEMPLE, CONDITION_ALWAYS, (Sint32)WorldMapAlignToMapScript,
		7880, 0, 1560, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_WORLD_MAP_TOWN, CONDITION_ALWAYS, (Sint32)WorldMapAlignToMapScript,
		6584, 0, 1192, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_WORLD_MAP_TOWN, CONDITION_ALWAYS, (Sint32)WorldMapAlignToMapScript,
		5592, 0, 2136, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_WORLD_MAP_TOWN, CONDITION_ALWAYS, (Sint32)WorldMapAlignToMapScript,
		5996, 0, 2894, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_WORLD_MAP_TOWN, CONDITION_ALWAYS, (Sint32)WorldMapAlignToMapScript,
		4904, 0, 2280, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_WORLD_MAP_MERCURY_LIGHTHOUSE, CONDITION_ALWAYS, (Sint32)WorldMapAlignToMapScript,
		6024, 0, 168, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_WORLD_MAP_MOUNTAIN, CONDITION_ALWAYS, (Sint32)WorldMapAlignToMapScript,
		5656, 0, 1112, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_WORLD_MAP_VENUS_LIGHTHOUSE, CONDITION_ALWAYS, (Sint32)WorldMapAlignToMapScript,
		6088, 0, 3176, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_WORLD_MAP_BARRICADE, CONDITION_ALWAYS, (Sint32)WorldMapAlignToMapScript,
		6952, 0, 1362, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_WORLD_MAP_BABI_LIGHTHOUSE, CONDITION_ALWAYS, (Sint32)WorldMapAlignToMapScript,
		5996, 0, 2808, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_WORLD_MAP_CAVE, CONDITION_ALWAYS, (Sint32)WorldMapAlignToMapScript,
		6424, 0, 1302, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_WORLD_MAP_CAVE, CONDITION_ALWAYS, (Sint32)WorldMapAlignToMapScript,
		6484, 0, 1184, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_WORLD_MAP_CAVE, CONDITION_ALWAYS, (Sint32)WorldMapAlignToMapScript,
		4840, 0, 1960, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_WORLD_MAP_CAVE, CONDITION_ALWAYS, (Sint32)WorldMapAlignToMapScript,
		5656, 0, 2392, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_WORLD_MAP_CAVE, CONDITION_ALWAYS, (Sint32)WorldMapAlignToMapScript,
		5480, 0, 2600, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_WORLD_MAP_FOREST, CONDITION_ALWAYS, (Sint32)WorldMapAlignToMapScript,
		7790, 0, 2080, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_WORLD_MAP_CAVE, CONDITION_ALWAYS, (Sint32)WorldMapAlignToMapScript,
		5976, 0, 3400, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_WORLD_MAP_CAVE, CONDITION_ALWAYS, (Sint32)WorldMapAlignToMapScript,
		5988, 0, 774, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_WORLD_MAP_CAVE, CONDITION_ALWAYS, (Sint32)WorldMapAlignToMapScript,
		6224, 0, 520, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_WORLD_MAP_CAVE, CONDITION_ALWAYS, (Sint32)WorldMapAlignToMapScript,
		5856, 0, 1174, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_WORLD_MAP_CAVE, CONDITION_ALWAYS, (Sint32)WorldMapAlignToMapScript,
		6920, 0, 1664, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_WORLD_MAP_CAVE, CONDITION_ALWAYS, (Sint32)WorldMapAlignToMapScript,
		6092, 0, 2200, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_WORLD_MAP_FOREST, CONDITION_ALWAYS, (Sint32)WorldMapAlignToMapScript,
		7166, 0, 1054, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_WORLD_MAP_FOREST, CONDITION_ALWAYS, (Sint32)WorldMapAlignToMapScript,
		7856, 0, 1736, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_WORLD_MAP_CAVE, CONDITION_ALWAYS, (Sint32)WorldMapAlignToMapScript,
		5480, 0, 2088, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_SIGNPOST, CONDITION_ALWAYS, (Sint32)WorldMapAlignToMapScript,
		0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_SIGNPOST, CONDITION_ALWAYS, (Sint32)WorldMapAlignToMapScript,
		0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_SIGNPOST, CONDITION_ALWAYS, (Sint32)WorldMapAlignToMapScript,
		0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_WORLD_MAP_BRIDGE, CONDITION_ALWAYS, (Sint32)WorldMapKolimaBridgeScript,
		7568, 0, 1384, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_WORLD_MAP_GATEHOUSE, CONDITION_ALWAYS, (Sint32)WorldMapAlignToMapScript,
		4942, 0, 2690, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_SHIPWRECK, CONDITION_ALWAYS, (Sint32)WorldMapAlignToMapScript,
		6036, 0, 3458, FACING_WEST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_SHIP, IF_FLAG_SET(FLAG_TOLBI_SHIP_AT_ISLAND), (Sint32)WorldMapAlignToMapScript,
		4760, 0, 440, FACING_NORTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_SANDSTORM, FLAG_WORLD_MAP_STORM_GONE, (Sint32)WorldMapStormScript,
		4392, 0, 296, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_IODEM, CONDITION_ALWAYS, (Sint32)WorldMapIodemScript,
		0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
	{ SCENE_TABLE_END },
	/* Unused: a row for Garet past the table's end, which nothing reads. */
	ACTOR_PLACEMENT(ACTOR_GARET, CONDITION_ALWAYS, (Sint32)WorldMapAlignToMapScript,
		0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
	{ SCENE_TABLE_END },
};

ACTOR_PLACE WorldMapActorPlacementsHammetsCaravan[] = {
	ACTOR_PLACEMENT(ACTOR_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_HAMMET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_BALD_MUSCULAR_MAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_WAGON, CONDITION_ALWAYS, (Sint32)WorldMapAlignToMapScript,
		0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_WAGON, CONDITION_ALWAYS, (Sint32)WorldMapAlignToMapScript,
		0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_WAGON, CONDITION_ALWAYS, (Sint32)WorldMapAlignToMapScript,
		0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_WAGON, CONDITION_ALWAYS, (Sint32)WorldMapAlignToMapScript,
		0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_BLANK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_BLUE_CRYSTAL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_WORLD_MAP_VILLAGE, CONDITION_ALWAYS, (Sint32)WorldMapAlignToMapScript,
		5848, 0, 1576, FACING_SOUTH, TALK_FACING_KEEP, 0),
	{ SCENE_TABLE_END },
};

ACTOR_PLACE WorldMapActorPlacementsFromWestLunpaCave[] = {
	ACTOR_PLACEMENT(SPRITE_WAGON, CONDITION_ALWAYS, (Sint32)WorldMapAlignToMapScript,
		0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_WORLD_MAP_VILLAGE, CONDITION_ALWAYS, (Sint32)WorldMapAlignToMapScript,
		5892, 0, 1128, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_WORLD_MAP_CAVE, CONDITION_ALWAYS, (Sint32)WorldMapAlignToMapScript,
		5856, 0, 1174, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_BUNZA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_HAMMET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
	{ SCENE_TABLE_END },
};

ACTOR_PLACE WorldMapActorPlacementsCrossingEnds[] = {
	ACTOR_PLACEMENT(SPRITE_SHIP, CONDITION_ALWAYS, (Sint32)WorldMapAlignToMapScript,
		5384, 0, 2248, FACING_WEST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_WORLD_MAP_TOWN, CONDITION_ALWAYS, (Sint32)WorldMapAlignToMapScript,
		5592, 0, 2136, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_WORLD_MAP_DOCK, CONDITION_ALWAYS, (Sint32)WorldMapAlignToMapScript,
		5422, 0, 2232, FACING_EAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_WORLD_MAP_DOCK, CONDITION_ALWAYS, (Sint32)WorldMapAlignToMapScript,
		5028, 0, 2328, FACING_WEST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	{ SCENE_TABLE_END },
};

ACTOR_PLACE WorldMapActorPlacementsCrossing[] = {
	ACTOR_PLACEMENT(SPRITE_SHIP, CONDITION_ALWAYS, (Sint32)WorldMapAlignToMapScript,
		5432, 0, 2312, FACING_WEST, TALK_FACING_KEEP, 0),
	{ SCENE_TABLE_END },
};

ACTOR_PLACE WorldMapActorPlacementsFromKalayTunnelEntranceSouth[] = {
	ACTOR_PLACEMENT(SPRITE_WORLD_MAP_TOWN, CONDITION_ALWAYS, (Sint32)WorldMapAlignToMapScript,
		5592, 0, 2136, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_WORLD_MAP_CAVE, CONDITION_ALWAYS, (Sint32)WorldMapAlignToMapScript,
		5480, 0, 2088, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_WAGON, CONDITION_ALWAYS, (Sint32)WorldMapAlignToMapScript,
		0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0),
	{ SCENE_TABLE_END },
};

ACTOR_PLACE WorldMapActorPlacementsJennaAndKradenAdrift[] = {
	ACTOR_PLACEMENT(ACTOR_JENNA, CONDITION_ALWAYS, (Sint32)WorldMapAlignToMapScript,
		7560, 0, 3512, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(ACTOR_SHEBA, CONDITION_ALWAYS, (Sint32)WorldMapAlignToMapScript,
		7816, 0, 3528, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, (Sint32)WorldMapAlignToMapScript,
		7544, 0, 3496, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_ALEX, CONDITION_ALWAYS, (Sint32)WorldMapAlignToMapScript,
		7708, 0, 3456, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_FELIX, CONDITION_ALWAYS, (Sint32)WorldMapAlignToMapScript,
		7800, 0, 3544, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_SHIPWRECK, CONDITION_ALWAYS, (Sint32)WorldMapAlignToMapScript,
		7604, 0, 3522, FACING_WEST, TALK_FACING_KEEP, 0),
	{ SCENE_TABLE_END },
};

ACTOR_PLACE WorldMapActorPlacementsFromClear[] = {
	ACTOR_PLACEMENT(SPRITE_WORLD_MAP_ISLAND, CONDITION_ALWAYS, (Sint32)WorldMapAlignToMapScript,
		7464, 0, 200, FACING_SOUTH, TALK_FACING_KEEP, 0),
	{ SCENE_TABLE_END },
};

ACTOR_PLACE WorldMapActorPlacementsBeforeShebasFall[] = {
	ACTOR_PLACEMENT(SPRITE_WORLD_MAP_VENUS_LIGHTHOUSE, CONDITION_ALWAYS, (Sint32)WorldMapAlignToMapScript,
		6080, 0, 3168, FACING_SOUTH, TALK_FACING_KEEP, 0),
	{ SCENE_TABLE_END },
};

CUE WorldCues[] = {
	{ CUE_STEP, METATILE_BEHAVIOR_WORLD_MAP_FLINT, FLAG_DJINNI(DJINNI_FLINT), SCRIPT_REF(scWorldMapMeetFlint) },
	{ CUE_STEP, METATILE_BEHAVIOR_WORLD_MAP_STORM, FLAG_WORLD_MAP_STORM_GONE, SCRIPT_REF(scWorldMapWalkIntoStorm) },
	{ CUE_TALK, ACTOR_VENUS_DJINNI, CONDITION_ALWAYS, SCRIPT_REF(scWorldMapMeetVine) },
	{ CUE_TALK, ACTOR_CORONA, CONDITION_ALWAYS, SCRIPT_REF(scWorldMapMeetCorona) },
	{ CUE_TALK, ACTOR_HAIL, CONDITION_ALWAYS, SCRIPT_REF(scWorldMapMeetHail) },
	{ CUE_STEP, METATILE_BEHAVIOR_WORLD_MAP_VINE, FLAG_DJINNI(DJINNI_VINE), SCRIPT_REF(scWorldMapMeetVine) },
	{ CUE_STEP, METATILE_BEHAVIOR_WORLD_MAP_CORONA, FLAG_DJINNI(DJINNI_CORONA), SCRIPT_REF(scWorldMapMeetCorona) },
	{ CUE_STEP, METATILE_BEHAVIOR_WORLD_MAP_HAIL, FLAG_DJINNI(DJINNI_HAIL), SCRIPT_REF(scWorldMapMeetHail) },
	{ EXAMINE_CUE(EXAMINE_SILENT), METATILE_BEHAVIOR_WORLD_MAP_SHIPWRECK, CONDITION_ALWAYS, SCRIPT_REF(scWorldMapSearchWreckage) },
	{ USE_ITEM(ITEM_BLACK_ORB), METATILE_BEHAVIOR_WORLD_MAP_SHIPWRECK, FLAG_BLACK_ORB_RETURNED, SCRIPT_REF(scWorldMapUseBlackOrb) },
	{ EXAMINE_CUE(EXAMINE_GROUND), METATILE_BEHAVIOR_WORLD_MAP_TURTLE_BOOTS, FLAG_WORLD_MAP_TURTLE_BOOTS, FIND_ITEM(ITEM_TURTLE_BOOTS) },
	{ CUE_TALK, ACTOR_IODEM, CONDITION_ALWAYS, SCRIPT_REF(scWorldMapTalkAboutBlackOrb) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), CUE_SUBJECT_OWN_EFFECT | ACTOR_IODEM, CONDITION_ALWAYS, SCRIPT_REF(scWorldMapTalkAboutBlackOrb) },
	{ CUE_EXIT, WORLD_MAP_TO_MOGALL_FOREST_SIGN, CONDITION_ALWAYS, WORLD_MAP_TO_MOGALL_FOREST_SIGN },
	{ CUE_EXIT, WORLD_MAP_TO_ALPINE_CROSSING_EAST, CONDITION_ALWAYS, WORLD_MAP_TO_ALPINE_CROSSING_EAST },
	{ CUE_EXIT, WORLD_MAP_TO_ALPINE_CROSSING_NORTH, CONDITION_ALWAYS, WORLD_MAP_TO_ALPINE_CROSSING_NORTH },
	{ CUE_EXIT, WORLD_MAP_TO_ALPINE_CROSSING_WEST, CONDITION_ALWAYS, WORLD_MAP_TO_ALPINE_CROSSING_WEST },
	{ CUE_EXIT, WORLD_MAP_TO_LAMAKAN_DESERT_EAST, CONDITION_ALWAYS, WORLD_MAP_TO_LAMAKAN_DESERT_EAST },
	{ CUE_EXIT, WORLD_MAP_TO_SUHALLA_DESERT_NORTH, CONDITION_ALWAYS, WORLD_MAP_TO_SUHALLA_DESERT_NORTH },
	{ CUE_EXIT, WORLD_MAP_TO_SUHALLA_DESERT_SOUTH, CONDITION_ALWAYS, WORLD_MAP_TO_SUHALLA_DESERT_SOUTH },
	{ CUE_EXIT, WORLD_MAP_TO_SUHALLA_GATE_CHECKPOINT_SOUTH, CONDITION_ALWAYS, WORLD_MAP_TO_SUHALLA_GATE_CHECKPOINT_SOUTH },
	{ CUE_EXIT, WORLD_MAP_TO_SUHALLA_GATE_CHECKPOINT_EAST, CONDITION_ALWAYS, WORLD_MAP_TO_SUHALLA_GATE_CHECKPOINT_EAST },
	{ CUE_EXIT, WORLD_MAP_TO_VENUS_LIGHTHOUSE_ENTRY_WEST, CONDITION_ALWAYS, WORLD_MAP_TO_VENUS_LIGHTHOUSE_ENTRY_WEST },
	{ CUE_EXIT, WORLD_MAP_TO_VENUS_LIGHTHOUSE_ENTRY_EAST, CONDITION_ALWAYS, WORLD_MAP_TO_VENUS_LIGHTHOUSE_ENTRY_EAST },
	{ CUE_EXIT, WORLD_MAP_TO_BILIBIN_BARRICADE_EAST, CONDITION_ALWAYS, WORLD_MAP_TO_BILIBIN_BARRICADE_EAST },
	{ CUE_EXIT, STEP_CUE_AREA(ACTOR_TO_VALE_PLAZA), CONDITION_ALWAYS, WORLD_MAP_TO_VALE_PLAZA },
	{ CUE_STEP, STEP_CUE_AREA(ACTOR_TO_VAULT), CONDITION_ALWAYS, SCRIPT_REF(scWorldMapEnterVault) },
	{ CUE_EXIT, STEP_CUE_AREA(ACTOR_TO_GOMA_CAVE_ENTRANCE), CONDITION_ALWAYS, WORLD_MAP_TO_GOMA_CAVE_ENTRANCE },
	{ CUE_EXIT, STEP_CUE_AREA(ACTOR_TO_GOMA_CAVE_BRIDGE), CONDITION_ALWAYS, WORLD_MAP_TO_GOMA_CAVE_BRIDGE },
	{ CUE_EXIT, STEP_CUE_AREA(ACTOR_TO_BILIBIN), CONDITION_ALWAYS, WORLD_MAP_TO_BILIBIN },
	{ CUE_STEP, STEP_CUE_AREA(ACTOR_TO_BILIBIN_BARRICADE), CONDITION_ALWAYS, SCRIPT_REF(scWorldMapEnterBilibinBarricade) },
	{ CUE_EXIT, STEP_CUE_AREA(ACTOR_TO_KOLIMA), CONDITION_ALWAYS, WORLD_MAP_TO_KOLIMA_WEST },
	{ CUE_STEP, STEP_CUE_AREA(ACTOR_TO_KOLIMA_BRIDGE), CONDITION_ALWAYS, SCRIPT_REF(scWorldMapEnterKolimaBridge) },
	{ CUE_EXIT, STEP_CUE_AREA(ACTOR_TO_KOLIMA_FOREST), CONDITION_ALWAYS, WORLD_MAP_TO_KOLIMA_FOREST_ENTRANCE },
	{ CUE_EXIT, STEP_CUE_AREA(ACTOR_TO_BILIBIN_CAVE_ENTRANCE), CONDITION_ALWAYS, WORLD_MAP_TO_BILIBIN_CAVE_ENTRANCE },
	{ CUE_EXIT, STEP_CUE_AREA(ACTOR_TO_BILIBIN_CAVE), CONDITION_ALWAYS, WORLD_MAP_TO_BILIBIN_CAVE_ICE },
	{ CUE_EXIT, STEP_CUE_AREA(ACTOR_TO_IMIL), CONDITION_ALWAYS, WORLD_MAP_TO_IMIL_SOUTH },
	{ CUE_EXIT, STEP_CUE_AREA(ACTOR_TO_MERCURY_LIGHTHOUSE), CONDITION_ALWAYS, WORLD_MAP_TO_MERCURY_LIGHTHOUSE_EXTERIOR_EAST },
	{ CUE_EXIT, STEP_CUE_AREA(ACTOR_TO_FUCHIN_TEMPLE), CONDITION_ALWAYS, WORLD_MAP_TO_FUCHIN_TEMPLE_GROUNDS },
	{ CUE_EXIT, STEP_CUE_AREA(ACTOR_TO_MOGALL_FOREST_SIGN), CONDITION_ALWAYS, WORLD_MAP_TO_MOGALL_FOREST_SIGN },
	{ CUE_EXIT, STEP_CUE_AREA(ACTOR_TO_MOGALL_FOREST_KILLER_APE), CONDITION_ALWAYS, WORLD_MAP_TO_MOGALL_FOREST_KILLER_APE },
	{ CUE_EXIT, STEP_CUE_AREA(ACTOR_TO_XIAN), CONDITION_ALWAYS, WORLD_MAP_TO_XIAN },
	{ CUE_EXIT, STEP_CUE_AREA(ACTOR_TO_ALTIN), CONDITION_ALWAYS, WORLD_MAP_TO_ALTIN },
	{ CUE_EXIT, STEP_CUE_AREA(ACTOR_TO_ALTIN_PEAK), CONDITION_ALWAYS, WORLD_MAP_TO_ALTIN_PEAK_WEST_EXIT },
	{ CUE_EXIT, STEP_CUE_AREA(ACTOR_TO_LAMA_TEMPLE), CONDITION_ALWAYS, WORLD_MAP_TO_LAMA_TEMPLE },
	{ CUE_EXIT, STEP_CUE_AREA(ACTOR_TO_LAMAKAN_DESERT_CAVE), CONDITION_ALWAYS, WORLD_MAP_TO_LAMAKAN_DESERT_CAVE },
	{ CUE_EXIT, STEP_CUE_AREA(ACTOR_TO_KALAY), CONDITION_ALWAYS, WORLD_MAP_TO_KALAY_EAST },
	{ CUE_EXIT, STEP_CUE_AREA(ACTOR_TO_LUNPA), CONDITION_ALWAYS, WORLD_MAP_TO_LUNPA },
	{ CUE_EXIT, STEP_CUE_AREA(ACTOR_TO_WEST_LUNPA_CAVE), CONDITION_ALWAYS, WORLD_MAP_TO_WEST_LUNPA_CAVE },
	{ CUE_STEP, STEP_CUE_AREA(ACTOR_TO_KALAY_DOCKS), CONDITION_ALWAYS, SCRIPT_REF(scWorldMapEnterKalayDocks) },
	{ CUE_EXIT, STEP_CUE_AREA(ACTOR_TO_TOLBI_DOCKS), CONDITION_ALWAYS, WORLD_MAP_TO_TOLBI_DOCKS },
	{ CUE_EXIT, STEP_CUE_AREA(ACTOR_TO_TOLBI), CONDITION_ALWAYS, WORLD_MAP_TO_TOLBI },
	{ CUE_EXIT, STEP_CUE_AREA(ACTOR_TO_ALTMILLER_CAVE), CONDITION_ALWAYS, WORLD_MAP_TO_ALTMILLER_CAVE_ENTRANCE },
	{ CUE_STEP, STEP_CUE_AREA(ACTOR_TO_GONDOWAN_PASSAGE), CONDITION_ALWAYS, SCRIPT_REF(scWorldMapEnterGondowanPassage) },
	{ CUE_EXIT, STEP_CUE_AREA(ACTOR_TO_KALAY_TUNNEL_ENTRANCE), CONDITION_ALWAYS, WORLD_MAP_TO_KALAY_TUNNEL_ENTRANCE },
	{ CUE_EXIT, STEP_CUE_AREA(ACTOR_TO_GONDOWAN_CAVE_NORTH), CONDITION_ALWAYS, WORLD_MAP_TO_GONDOWAN_CAVE_NORTH },
	{ CUE_EXIT, STEP_CUE_AREA(ACTOR_TO_GONDOWAN_CAVE_SOUTH), CONDITION_ALWAYS, WORLD_MAP_TO_GONDOWAN_CAVE_SOUTH },
	{ CUE_EXIT, STEP_CUE_AREA(ACTOR_TO_SUHALLA), CONDITION_ALWAYS, WORLD_MAP_TO_SUHALLA },
	{ CUE_EXIT, STEP_CUE_AREA(ACTOR_TO_GATEWAY_CAVE), CONDITION_ALWAYS, WORLD_MAP_TO_GATEWAY_CAVE },
	{ CUE_EXIT, STEP_CUE_AREA(ACTOR_TO_ROAD_TO_BABI_LIGHTHOUSE), CONDITION_ALWAYS, WORLD_MAP_TO_ROAD_TO_BABI_LIGHTHOUSE },
	{ CUE_STEP, STEP_CUE_AREA(ACTOR_TO_LALIVERO), CONDITION_ALWAYS, SCRIPT_REF(scWorldMapEnterLalivero) },
	{ STEP_CUE_WALKING_INTO(FACING_EAST), METATILE_BEHAVIOR_WORLD_MAP_TOLBI_SHIP, IF_FLAG_SET(FLAG_TOLBI_SHIP_AT_ISLAND), SCRIPT_REF(scWorldMapReturnToTolbiShip) },
	{ CUE_EXIT | CUE_FLAG_FACING | FACING_NORTH, METATILE_BEHAVIOR_WORLD_MAP_CROSSBONE_ISLE, CONDITION_ALWAYS, WORLD_MAP_TO_CROSSBONE_ISLE },
	{ PSYNERGY_PHASE_ON_ACTOR(ABILITY_DOUSE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_STORM, IF_FLAG_SET(FLAG_WORLD_MAP_CAUGHT_IN_STORM), SCRIPT_REF(scWorldMapStartDousingStorm) },
	{ PSYNERGY_ON_ACTOR(ABILITY_DOUSE), ACTOR_STORM, IF_FLAG_SET(FLAG_WORLD_MAP_CAUGHT_IN_STORM), SCRIPT_REF(scWorldMapDouseMapStorm) },
	{ CUE_RAISED, RAISED_STORM_TIME_UP, CONDITION_ALWAYS, SCRIPT_REF(scWorldMapStormDropsParty) },
	{ CUE_TABLE_END },
};

#include "shared/tolbi_ship_rowing.c"
/* clang-format on */

Sint32 IslandWakeScript[] = {
	SC_ADD(SW_X, -toFIXED(1) / 10),
	SC_ADD(SW_Z, toFIXED(1) / 10),
	SC_ADD(SW_SCALE_X, toFIXED(7) / 128),
	SC_ADD(SW_SCALE_Y, -toFIXED(3) / 512),
	SC_WAIT(1),
	SC_LOOP(48, SC_TOP),
	SC_POSITION(0, 0, 0),
	SC_WAIT(1),
	SC_RELEASE,
};
