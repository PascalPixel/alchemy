// Vale on the stormy night (SceneIdValePrologue), the prologue, when Isaac and Garet are children.
//
// It is a scene of its own that stands in for SceneIdVale while FLAG_VALE_STORM is set, so that the
// doors that lead to Vale by day lead here. scInit sets the flag on every arrival, and vale_plaza.c
// clears it when the night ends.
//
// The night is a chain of story flags. Each is set by one script and read by scInit, which rebuilds
// the scene at that stage on the next arrival:
//
// - FLAG_VALE_STORM_SENT_TO_PLAZA: scValePrologueDoraSendsIsaacToThePlaza,
//       played on the first arrival by
//       VALE_PROLOGUE_FROM_COTTAGE_ROOMS_B_WITH_DORA. Dora and Kyle send Isaac
//       on to the plaza and go to help. Until the flag is set the lightning
//       waits for the script.
// - FLAG_VALE_STORM_FIRST_ROCK_FELL to FLAG_VALE_STORM_FOURTH_ROCK_FELL, with
//       the area flags FLAG_VALE_STORM_FIRST_ROCK_PASSED to
//       FLAG_VALE_STORM_FOURTH_ROCK_PASSED: scValePrologueFirstRockFalls to
//       scValePrologueFourthRockFalls drop a huge rock across a path at four
//       spots: south of Isaac's house, below the house north-west of it, on the
//       path along the west edge, and east of Jenna's grandparents' house.
// The _FELL flag puts back the rock and the metatile behaviors of the
// metatiles  it blocks. The _PASSED flag, cleared once the party leaves
// Vale, keeps  the script from running again.
// - FLAG_VALE_STORM_BUMPED_INTO_VILLAGER: scValePrologueBumpIntoShovingMan,
//       the first time Isaac tries the door of the house north-west of his own
//       (VALE_PROLOGUE_TO_COTTAGE_ROOMS_A_SOUTH).
// - FLAG_VALE_STORM_HEARD_GARET, then FLAG_VALE_STORM_GARET_JOINED: Garet is
//       trying to save his chest of valuables. Talking to him, or walking off
//       past him, brings him into the party (scValePrologueGaretLeavesHisChest)
//       and leaves the chest by his door. scValePrologueUnusedGaretJoinsIsaac
// is a third way that never plays. With Garet along, a fallen rock and the path
//       north to Mt. Aleph have him turn Isaac back.
// - FLAG_VALE_BOULDER_FELL: scValePrologueWatchBoulderFall, the elders
//       holding back the Boulder with their Psynergy as it slips. From then on
//       it rests where it slipped to.
// - FLAG_VALE_STORM_SAW_FELIX_HANGING_ON:
//       scValePrologueWatchAttemptToSaveFelix and
// scValePrologueDoraGoesForHelp, Felix clinging on in the river. The plaza (vale_plaza.c) then sets
// FLAG_VALE_STORM_SENT_TO_AID_ELDERS and sends the boys back.
// - FLAG_VALE_STORM_BOULDER_SWEPT_THEM_AWAY:
//       scValePrologueBoulderSweepsThemAway and
//       scValePrologueShowBoulderSweepingDownriver, the Boulder carrying Felix,
//       his parents and Kyle away down the river.
// - FLAG_VALE_STORM_GARET_LEFT: scValePrologueGaretFollowsIsaac, as Isaac
//       leaves for the plaza again. The night ends in vale_plaza.c.
//
// Every way out goes through scValeRequestBackdropExit (shared/vale.c), which stops the lightning
// first, and each door plays its opening metatiles before it. ValePrologueExits gives each exit's
// destination.
//
// Two monsters attack once each, at the west end of the rope bridge west of Isaac's house and past
// the east end of the bridge in the south (slots 2 and 3 of ENCOUNTER_ZONE_SCRIPTED_VALE_VAULT).

#include "battle/battle.h"
#include "debug/debug.h"
#include "field/script_inline.h"
#include "field/field_effect.h"
#include "field/field_psynergy_state.h"
#include "graphics/graphics.h"
#include "scene/squash_and_stretch.h"
#include "scene/script_psynergy.h"
#include "scene/vale.h"
#include "scene/walk_into_leader.h"
#include "text/msg_ids.h"

extern Uint8          ValePrologueFlashColorModes[];
extern Sint32         ValePrologueShovingManScript[];
extern Sint32         ValePrologueUnusedGaretRunsUpScript[];
extern Sint32         ValePrologueUnusedGaretPantingScript[];
extern Sint32         ValePrologueBoulderWaits[];
extern Sint32         ValePrologueBoulderFallLeaderPath[];
extern Sint32         ValePrologueBoulderFallGaretPath[];
extern Sint32         ValePrologueDoraRunsIntoRiversideHouse[];
extern Sint32         ValePrologueJennaRunsIntoRiversideHouse[];
extern Sint32         ValePrologueDoraComesOutOfRiversideHouse[];
extern Sint32         ValePrologueJennaComesOutOfRiversideHouse[];
extern Sint32         ValePrologueDoraHeadsNorth[];
extern Sint32         ValePrologueJennaHeadsForPlaza[];
extern Sint32         ValePrologueDoraLeavesForHelp[];
extern Sint32         ValePrologueDoraRunsToRiver[];
extern Sint32         ValePrologueToRiverbankPath[];
extern Sint32         ValePrologueExhaustedStrongmanGatherPath[];
extern Sint32         ValePrologueJennaGatherPath[];
extern Sint32         ValePrologueLeaderGatherPath[];
extern Sint32         ValePrologueGaretGatherPath[];
extern Sint32         ValePrologueLookAroundScript[];
extern Sint32         ValePrologueSquashAndStretchScript[];
extern Sint32         ValePrologueBoulderRollsDownriver[];
extern SCENE_ENTRANCE ValePrologueEntrances[];
extern Uint32         ValePrologueExits[];
extern ACTOR_PLACE    ValePrologueActorPlacements[];
extern ACTOR_PLACE    ValePrologueBoulderElders[];
extern CUE            ValePrologueCues[];
extern Uint16         ValePrologueSanctumDoorOpens[];
extern Uint16         ValePrologueIsaacsDoorOpens[];
extern Uint16         ValePrologueGrandparentsDoorOpens[];
extern Uint16         ValePrologueWestHouseDoorOpens[];
extern Uint16         ValePrologueIsaacsDoorCloses[];
extern Uint16         ValePrologueGrandparentsDoorCloses[];

static void           scValePrologueDoraSendsIsaacToThePlaza(void);
static void           scValePrologueApplyFirstRockCollision(void);
static void           scValePrologueApplySecondRockCollision(void);
static void           scValePrologueApplyThirdRockCollision(void);
static void           scValePrologueApplyFourthRockCollision(void);
static void           scValePrologueBumpIntoShovingMan(void);
static void           scValePrologueGaretLeavesHisChest(void);
static void           scValePrologueGaretTalksAboutBlockedPath(Sint32 x, Sint32 z);
static void           scValePrologueDoraGoesForHelp(void);
static void           scValePrologueShowBoulderSweepingDownriver(void);
static void           scValePrologueFlashLightning(void);
static void           scValePrologueUpdateBoulderShadow(void);
static void           scValePrologueFlickerBoulderShadow(void);
static void           scValePrologueGaretFollowsIsaac(void);
static void           scValePrologueRebuildRiverbank(void);
static void           scValePrologueFlashPaletteAndSpawnEffects(ACTOR *actor);
static void           scSpawnPsynergyRings(ACTOR *parent);
static void           scValePrologueUpdateBoulderMapEffects(void);
static void           scValePrologueCycleBoulderPalette(void);
static void           scValePrologueCopyLeaderSpritePriority(void);

// The placed actors, numbered in the order of ValePrologueActorPlacements. The elders holding back
// the Boulder (ValePrologueBoulderElders) follow them.
typedef enum STORM_NIGHT_ACTOR_ID
{
	// The strongman who comes back to the river with Jenna, his Psynergy restored at the Psynergy
	// Stone (vale_plaza.c). After the Boulder has passed he tries to go for help and cannot. "I...
	// I'll go!", then "It's no use. I can't move..."
	ACTOR_EXHAUSTED_STRONGMAN = ACTOR_FIRST_PLACED,
	ACTOR_DORA,
	ACTOR_KYLE,
	// Eight huge rocks that Isaac "can't even budge". ACTOR_FIRST_ROCK to ACTOR_FIRST_ROCK + 3 fall
	// across the paths (scValePrologueFirstRockFalls to scValePrologueFourthRockFalls,
	// FLAG_VALE_STORM_FIRST_ROCK_FELL on). ACTOR_FIRST_ROCK + 4 and + 5 already lie there.
	// ACTOR_FIRST_ROCK + 6 and
	// + 7 lie there only once the Boulder has passed down the river
	// (scValePrologueShowBoulderSweepingDownriver).
	ACTOR_FIRST_ROCK,
	ACTOR_BOULDER = ACTOR_FIRST_PLACED
	              + 11, // drawn at twice its size
	                    // A stout man who runs out of his house and shoves Isaac. "Huh? Isaac?"
	ACTOR_SHOVING_MAN,
	// The things Garet struggles to save. "If a rock hits me now, I'll lose both my chest and my
	// life."
	ACTOR_GARETS_CHEST,
	ACTOR_GARET_CHILD,
	ACTOR_FELIX_CHILD, // in the river
	                   // Jenna's father, a man with brown hair in a green shirt. Jenna calls him
	                   // and her mother "Mom! Dad!". "Hang on, Felix! Help is on the way!"
	ACTOR_JENNAS_FATHER,
	// Jenna's mother, a woman with pink hair in a white apron. "Don't let go, Felix!"
	ACTOR_JENNAS_MOTHER,
	ACTOR_JENNA_CHILD,
	// The Boulder's shadow (SPRITE_BLANK_LARGE, a black blob). It is drawn on alternate ticks under
	// the Boulder as it comes down the river, swelling and shrinking with its bounds
	// (scValePrologueUpdateBoulderShadow). During the frozen flashes it is twice its size
	// (scValePrologueFlickerBoulderShadow).
	ACTOR_BOULDER_SHADOW,
	// The elders holding back the Boulder with their Psynergy ("Go aid the elders!", vale_plaza.c),
	// from ValePrologueBoulderElders, which scValePrologueWatchBoulderFall places after the scene's
	// own actors and removes. Two invisible actors stand on the spots of ACTOR_ELDER_EAST and
	// ACTOR_ELDER_WEST, and some of the elders' lines come from them. The invisible actor on
	// ACTOR_ELDER_EAST's spot that speaks for that elder, the elders' first and third lines. "Ahhh!
	// The Boulder! It's falling!", then "If our Psynergy runs out, Vale will be destroyed!"
	ACTOR_ELDER_EAST_STAND_IN,
	// The westernmost elder, drawn with the sprite of the young man with brown hair (as is
	// ACTOR_ELDER_FAR_EAST), tells the boys to run. "Hurry, boys! Run!!! It's just a little
	// farther!"
	ACTOR_ELDER_FAR_WEST,
	// The easternmost elder, the same sprite drawn mirrored. "We have to hold on until the
	// villagers are safe!"
	ACTOR_ELDER_FAR_EAST,
	// The invisible actor on ACTOR_ELDER_WEST's spot that speaks for that elder, the elders' second
	// line. "It's too heavy! We can't hold it for long!"
	ACTOR_ELDER_WEST_STAND_IN,
	// The elders between the two are men with long brown hair and beards in long robes. East of
	// ACTOR_ELDER_WEST, and drawn as that elder is.
	ACTOR_ELDER_EAST,
	ACTOR_ELDER_WEST
} STORM_NIGHT_ACTOR_ID;

// What the night's metatiles do when Isaac steps on them, walks into them or searches them, by the
// cue rows that answer them. The day's Vale (vale.c) numbers the same metatiles the same way.
typedef enum VALE_PROLOGUE_METATILE_BEHAVIOR
{
	// The steps up Mt. Aleph in the north-west: metatiles (5, 16) to (8, 16).
	METATILE_BEHAVIOR_VALE_STEPS_UP_MT_ALEPH = 2,
	// The northern way west to Kraden's cottage: metatiles (0, 38) to (0, 41).
	METATILE_BEHAVIOR_VALE_NORTH_WAY_TO_KRADEN,
	// The southern way west: metatiles (0, 49) and (0, 50).
	METATILE_BEHAVIOR_VALE_SOUTH_WAY_TO_KRADEN,
	// The way east to the plaza, in the south: metatiles (29, 86) and (29, 87).
	METATILE_BEHAVIOR_VALE_WAY_TO_THE_PLAZA,
	// The door of Garet's house: metatile (25, 46).
	METATILE_BEHAVIOR_VALE_GARETS_DOOR,
	// The door of the house north-west of Isaac's: metatile (16, 51).
	METATILE_BEHAVIOR_VALE_NORTHWEST_HOUSE_DOOR,
	// The door of Isaac's house: metatile (21, 56).
	METATILE_BEHAVIOR_VALE_ISAACS_DOOR,
	// The door of Jenna's grandparents' house: metatile (20, 71).
	METATILE_BEHAVIOR_VALE_GRANDPARENTS_DOOR,
	// The door of the house south of theirs: metatile (23, 78).
	METATILE_BEHAVIOR_VALE_SOUTH_HOUSE_DOOR,
	// Unused: no metatile carries it.
	METATILE_BEHAVIOR_VALE_UNUSED_WRECKED_HOUSE_DOOR,
	// The west door of the house by the river, on its west bank: metatile (6, 75).
	METATILE_BEHAVIOR_VALE_RIVERSIDE_HOUSE_WEST_DOOR,
	// Its east door, onto the wooden deck: metatile (9, 74).
	METATILE_BEHAVIOR_VALE_RIVERSIDE_HOUSE_EAST_DOOR,
	// A metatile beyond each end of the rope bridges, a step from the river.
	METATILE_BEHAVIOR_VALE_OFF_BRIDGE,
	// The metatiles at each end of the rope bridges.
	METATILE_BEHAVIOR_VALE_ONTO_BRIDGE,
	// Where the night's two monsters attack: the west end of the rope bridge west of Isaac's house,
	// metatiles (3, 54) and (4, 54), and past the east end of the bridge in the south, metatiles
	// (21, 85) and (21, 86).
	METATILE_BEHAVIOR_VALE_THIRD_MONSTER,
	METATILE_BEHAVIOR_VALE_FOURTH_MONSTER,
	// Garet's chest of valuables, left on the path: metatiles (25, 38) to (27, 38).
	METATILE_BEHAVIOR_VALE_GARETS_CHEST       = 19,
	// The wrecked house's stuck door, once the Boulder has passed: metatile (6, 74).
	METATILE_BEHAVIOR_VALE_WRECKED_HOUSE_DOOR = 24,
	// Beside Garet on the path north of his house: metatiles (21, 38) to (21, 41).
	METATILE_BEHAVIOR_VALE_BESIDE_GARET       = 30,
	// West of Garet, walking on past him: metatiles (19, 39) to (19, 42).
	METATILE_BEHAVIOR_VALE_PAST_GARET,
	// Around the foot of the steps up to the Sanctum, metatiles (15, 29) to (16, 29) and (15, 35)
	// to (16, 35), where Garet would run up to Isaac (scValePrologueUnusedGaretJoinsIsaac).
	METATILE_BEHAVIOR_VALE_UNUSED_GARET_JOINS,
	// The foot of the steps up to the Sanctum's door: metatiles (15, 30) and (16, 30).
	METATILE_BEHAVIOR_VALE_FOOT_OF_SANCTUM_STEPS,
	// Where the Boulder comes into view: metatiles (4, 35) and (5, 35).
	METATILE_BEHAVIOR_VALE_BOULDER_IN_VIEW,
	// The river's west bank where Felix is seen: metatiles (9, 66), (8, 67) and (9, 67).
	METATILE_BEHAVIOR_VALE_FELIX_IN_VIEW,
	// The middle of the rope bridge in the south, once its planks are laid: metatiles (13, 85) and
	// (13, 86).
	METATILE_BEHAVIOR_VALE_MIDDLE_OF_BRIDGE,
	// Where each rock falls as Isaac walks past (scValePrologueFirstRockFalls to
	// scValePrologueFourthRockFalls).
	METATILE_BEHAVIOR_VALE_FIRST_ROCK = 41,
	METATILE_BEHAVIOR_VALE_SECOND_ROCK,
	METATILE_BEHAVIOR_VALE_THIRD_ROCK,
	METATILE_BEHAVIOR_VALE_FOURTH_ROCK
} VALE_PROLOGUE_METATILE_BEHAVIOR;

// Animations of the night's own sprites beyond ACTOR_ANIMATION, as each sprite's animation table
// and ticks under graphics/sprite draw them.
typedef enum STORM_NIGHT_ANIMATION
{
	// Garet leaning into his chest to drag it (garet_child.png).
	ANIM_GARET_DRAG              = 5,
	// Garet's chest, its rope pulled taut as he drags it (chest_of_valuables.png).
	ANIM_CHEST_DRAGGED           = 2,
	ANIM_CHEST_ROPE_DROPPED      = 3, // its rope let fall on the ground
	                             // Kyle crouched at the bank, reaching out to Felix (kyle.png).
	ANIM_KYLE_REACH_OUT          = 5,
	// Jenna's father and mother crouched at the bank, reaching out (man_in_green_shirt.png,
	// woman_with_pink_hair.png).
	ANIM_PARENT_REACH_OUT        = 6,
	// Only Felix's hands above the water as he goes under (felix_child.png).
	ANIM_FELIX_UNDER_WATER       = 6,
	// Felix clinging on in the river, one arm reaching out (felix_child.png).
	ANIM_FELIX_CLING             = 7,
	// A bearded elder seen from behind, both arms raised against the Boulder
	// (bearded_man_in_long_robe.png).
	ANIM_BEARDED_ELDER_HOLD_BACK = 5,
	// The poses frozen in the lightning as the Boulder comes down the river
	// (scValePrologueShowBoulderSweepingDownriver).
	ANIM_JENNA_RUN               = 5, // in mid-stride (jenna_child.png)
	ANIM_PARENT_RECOIL           = 5, // seen from behind, arms flung out
	ANIM_FELIX_CLING_STILL       = 8  // ANIM_FELIX_CLING's first frame held
} STORM_NIGHT_ANIMATION;

// Vale's area flags this scene keeps while the party is in Vale.
typedef enum STORM_NIGHT_AREA_FLAG
{
	// The strongman at the river tried to go for help and could not.
	FLAG_VALE_STORM_STRONGMAN_TRIED_TO_GO  = FLAG_AREA_TEMP_05,
	// Garet has said once that they cannot get around a fallen rock.
	FLAG_VALE_STORM_GARET_SAW_BLOCKED_PATH = FLAG_AREA_TEMP_0C,
	// Kyle has greeted Isaac at the river.
	FLAG_VALE_STORM_KYLE_GREETED_ISAAC     = FLAG_AREA_TEMP_0D,
	// The party has passed each rock's spot since coming to Vale, so its script does not play
	// again.
	FLAG_VALE_STORM_FIRST_ROCK_PASSED      = FLAG_AREA_TEMP_10,
	FLAG_VALE_STORM_SECOND_ROCK_PASSED     = FLAG_AREA_TEMP_11,
	FLAG_VALE_STORM_THIRD_ROCK_PASSED      = FLAG_AREA_TEMP_12,
	FLAG_VALE_STORM_FOURTH_ROCK_PASSED     = FLAG_AREA_TEMP_13
} STORM_NIGHT_AREA_FLAG;

// Scene flags, cleared on every ordinary arrival:
// - FLAG_VALE_ISAACS_DOOR_OPEN: the door of Isaac's house, from which Dora and
//       Isaac came, stands open.
// - FLAG_VALE_SHOVING_MANS_DOOR_OPEN: the stout man has run out of the door of
//       VALE_PROLOGUE_TO_COTTAGE_ROOMS_A_SOUTH, leaving it open.
// - FLAG_VALE_ON_BRIDGE (scene/vale.h, as Vale has it): the leader is on the
//       rope bridge over the river.

// 0 while the westernmost elder strains at the Boulder, 1 once he lets go.
static Sint32 ShakeDone;
// 0 to 2, raised and lowered as the Boulder lurches and settles: each step puts RINGS_INTERVAL_STEP
// more ticks between the elders' rings.
static Sint32 ShakeShift;

// How long the elders call up their Psynergy once the camera reaches the Boulder, in ticks.
#define ELDERS_FLASH_TICKS           40
// Ticks between an elder's rings: RINGS_INTERVAL, and RINGS_INTERVAL_STEP more for each step of
// ShakeShift.
#define RINGS_INTERVAL               16
#define RINGS_INTERVAL_STEP          8
// The colour modes an elder or the Boulder cycles through: plain colours and their five swaps,
// SPRITE_COLOR_NORMAL to SPRITE_COLOR_BRG.
#define SWAPPED_COLOR_MODES          (SPRITE_COLOR_BRG + 1)

// A random pause of 20 to 39 ticks between the glances of ValePrologueLookAroundScript.
#define VALE_PROLOGUE_GLANCE_PAUSE() (syMathRemainderUnsigned(syRandomNext(), 20) + 20)

// The step of ValePrologueLookAroundScript.
static Sint32 scValePrologueLookAround(ACTOR *actor)
{
	Sint32 timer     = actor->timer;
	Sint32 countdown = (Sint16)timer;

	if (timer == 0)
	{
		actor->facing = syRandomNext();
		countdown     = VALE_PROLOGUE_GLANCE_PAUSE();
		actor->timer  = countdown;
	}
	actor->timer = countdown - 1;
	return 1;
}

#include "shared/squash_and_stretch.c"

// The door of Isaac's house, from which Dora and Isaac came out into the storm, stands open.
#define FLAG_VALE_ISAACS_DOOR_OPEN       FLAG_SCENE_TEMP_05
// The stout man has run out of his door and shoved Isaac, leaving the door open.
#define FLAG_VALE_SHOVING_MANS_DOOR_OPEN FLAG_SCENE_TEMP_06

// Returns where the party appears.
const SCENE_ENTRANCE *scGetEntrances(void)
{
	return ValePrologueEntrances;
}

const SCENE_EXIT_REGION *scGetExitRegions(void)
{
	return NULL;
}

const Uint32 *scGetExits(void)
{
	return ValePrologueExits;
}

const ACTOR_PLACE *scGetActorPlacements(void)
{
	return ValePrologueActorPlacements;
}

const CUE *scGetCues(void)
{
	return ValePrologueCues;
}

// Nothing visible happens as Isaac steps up to either end of the rope bridge across the river in
// the south of the village, at metatiles (8, 85) to (8, 88) on the west bank or (18, 84) to (18,
// 87) on the east. The game lays the bridge's planks into the metatiles over the river, so that he
// can walk across.
//
// As by day (scValeStepOntoBridge in vale.c), the planks are copied from the bridge's own layer,
// metatiles (40, 84) to (46, 87) of the map, onto metatiles (10, 84) to (16, 87), and
// FLAG_VALE_ON_BRIDGE keeps them there after a battle. The ends of the rope bridge west of Isaac's
// house, metatiles (5, 56) to (5, 57) and (13, 56) to (14, 57), run this too, though that bridge is
// always open.
static void scValePrologueStepOntoBridge(void)
{
	gmFlagSet(FLAG_VALE_ON_BRIDGE);
	fdMapGridCopyMetatileAttributes(40, 84, 7, 4, 10, 84);
}

// Nothing visible happens as Isaac steps off either end of the rope bridge across the river in the
// south of the village, a metatile further from the river than scValePrologueStepOntoBridge's
// metatiles. The game puts the river's metatiles back under the bridge.
//
// The river's metatiles are kept below the bridge's own, at metatiles (40, 89) to (46, 92) of the
// map. The ends of the bridge west of Isaac's house run this too.
static void scValePrologueStepOffBridge(void)
{
	gmFlagClear(FLAG_VALE_ON_BRIDGE);
	fdMapGridCopyMetatileAttributes(40, 89, 7, 4, 10, 84);
}

#include "shared/vale.c"

// Unused: on the stormy night, the Sanctum's door opens and the leader goes into a house of Vale.
//
// It takes VALE_PROLOGUE_TO_COTTAGE_ROOMS_A_WEST_SANCTUM_DOOR (SceneIdCottageRoomsA, at
// COTTAGE_ROOMS_A_FROM_VALE_WEST, the house VALE_PROLOGUE_TO_COTTAGE_ROOMS_A_WEST and
// VALE_PROLOGUE_TO_COTTAGE_ROOMS_A_WEST_BY_WEST_BANK_DOOR lead to as well). No cue names it, and
// nothing else takes that exit.
UNUSED static void scValePrologueUnusedOpenSanctumDoor(void)
{
	sySoundPlay(SE_DOOR_OPEN);
	fdMapGridPlayMapEdit(ValePrologueSanctumDoorOpens, 45, 11);
	fdActorWalkTo(ACTOR_PARTY_LEADER, 257, 420);
	fdScriptWait(3);
	scValeRequestBackdropExit(VALE_PROLOGUE_TO_COTTAGE_ROOMS_A_WEST_SANCTUM_DOOR);
}

// On the stormy night, climbing the steps up Mt. Aleph in the north-west of the village (metatiles
// (5..8, 16)) does not take Isaac up the mountain: he comes out in the plaza, outside the door of
// the house at its top.
//
// By day the steps lead to Sol Sanctum by exit 1 (VALE_TO_SOL_ENTRANCE). The script asks for the
// same exit 1, which ValePrologueExits does not list, and the running game brings Isaac out at the
// plaza's first entrance, VALE_PLAZA_FROM_COTTAGE_ROOMS_A_NORTH, instead.
static void scValePrologueLeaveNorthwestToValePlaza(void)
{
	sySoundPlay(SE_MAP_EXIT);
	scValeRequestBackdropExit(VALE_TO_SOL_ENTRANCE);
}

// On the stormy night, Isaac leaves the village by the northern of the two paths off its west edge
// (metatiles (0, 38..41)), toward Kraden's cottage.
static void scValePrologueLeaveToValeKradensCottageNorth(void)
{
	sySoundPlay(SE_MAP_EXIT);
	scValeRequestBackdropExit(VALE_PROLOGUE_TO_VALE_KRADENS_COTTAGE_NORTH);
}

// On the stormy night, Isaac leaves the village by the southern of the two paths off its west edge
// (metatiles (0, 49..50)), toward Kraden's cottage.
static void scValePrologueLeaveToValeKradensCottageSouth(void)
{
	sySoundPlay(SE_MAP_EXIT);
	scValeRequestBackdropExit(VALE_PROLOGUE_TO_VALE_KRADENS_COTTAGE_SOUTH);
}

// On the stormy night, Isaac leaves by the path off the village's south-east edge (metatiles
// (29, 86..87)) for the plaza, where the villagers have gathered (SceneIdValePlaza).
//
// Once the Boulder has swept Felix and the others away, Garet goes after him first
// (scValePrologueGaretFollowsIsaac).
static void scValePrologueLeaveToValePlaza(void)
{
	sySoundPlay(SE_MAP_EXIT);
	if (gmFlagGet(FLAG_VALE_STORM_BOULDER_SWEPT_THEM_AWAY) != 0
	    && gmFlagGet(FLAG_VALE_STORM_GARET_LEFT) == 0)
	{
		scValePrologueGaretFollowsIsaac();
	}
	scValeRequestBackdropExit(VALE_PROLOGUE_TO_VALE_PLAZA);
}

// On the stormy night, Isaac opens the door of Garet's house (metatile (25, 46)) and goes in.
static void scValePrologueLeaveToCottageRoomsAEast(void)
{
	sySoundPlay(SE_DOOR_OPEN);
	fdMapGridPlayMapEdit(ValePrologueIsaacsDoorOpens, 54, 32);
	fdActorWalkTo(ACTOR_PARTY_LEADER, 406, 727);
	fdScriptWait(3);
	scValeRequestBackdropExit(VALE_PROLOGUE_TO_COTTAGE_ROOMS_A_EAST);
}

// The first time Isaac walks up to the door of the house north-west of his own (metatile (16, 51))
// on the stormy night, a stout man runs out of it and shoves him
// (scValePrologueBumpIntoShovingMan). After that, or once the second rock has fallen, Isaac goes
// in.
static void scValePrologueLeaveToCottageRoomsA(void)
{
	if (fdScriptGetFlag(FLAG_VALE_SHOVING_MANS_DOOR_OPEN) == 0)
	{
		sySoundPlay(SE_DOOR_OPEN);
		fdMapGridAnimateMetatiles(ValePrologueGrandparentsDoorOpens, 45, 39);
	}
	if (gmFlagGet(FLAG_VALE_STORM_BUMPED_INTO_VILLAGER) == 0
	    && gmFlagGet(FLAG_VALE_STORM_SECOND_ROCK_FELL) == 0)
	{
		scValePrologueBumpIntoShovingMan();
		gmFlagSet(FLAG_VALE_SHOVING_MANS_DOOR_OPEN);
	}
	else
	{
		fdActorWalkTo(ACTOR_PARTY_LEADER, 262, 805);
		fdScriptWait(3);
		scValeRequestBackdropExit(VALE_PROLOGUE_TO_COTTAGE_ROOMS_A_SOUTH);
	}
}

// On the stormy night, Isaac goes back into his own house by its door (metatile (21, 56)), the one
// he and Dora came out of. It opens unless they left it open.
static void scValePrologueLeaveToIsaacsHouse(void)
{
	if (gmFlagGet(FLAG_VALE_ISAACS_DOOR_OPEN) == 0)
	{
		sySoundPlay(SE_DOOR_OPEN);
		fdMapGridPlayMapEdit(ValePrologueIsaacsDoorOpens, 50, 44);
	}
	fdActorWalkTo(ACTOR_PARTY_LEADER, 340, 888);
	fdScriptWait(3);
	scValeRequestBackdropExit(VALE_PROLOGUE_TO_COTTAGE_ROOMS_B_ISAACS_HOUSE);
}

// On the stormy night, Isaac opens the door of Jenna's grandparents' house, on the river's east
// bank (metatile (20, 71)), and goes in.
static void scValePrologueLeaveToCottageRoomsANorth(void)
{
	sySoundPlay(SE_DOOR_OPEN);
	fdMapGridPlayMapEdit(ValePrologueGrandparentsDoorOpens, 49, 69);
	fdActorWalkTo(ACTOR_PARTY_LEADER, 326, 1126);
	fdScriptWait(3);
	scValeRequestBackdropExit(VALE_PROLOGUE_TO_COTTAGE_ROOMS_A_NORTH);
}

// On the stormy night, Isaac opens the door of the house south of Jenna's grandparents', on the
// river's east bank (metatile (23, 78)), and goes in.
static void scValePrologueLeaveToCottageRoomsAWest(void)
{
	sySoundPlay(SE_DOOR_OPEN);
	fdMapGridPlayMapEdit(ValePrologueWestHouseDoorOpens, 52, 76);
	fdActorWalkTo(ACTOR_PARTY_LEADER, 374, 1238);
	fdScriptWait(3);
	scValeRequestBackdropExit(VALE_PROLOGUE_TO_COTTAGE_ROOMS_A_WEST);
}

// Unused: a door of a house on the river's west bank would lead into the house south of Jenna's
// grandparents', but no metatile of the stormy night's map carries its row's metatile behavior, so
// it never opens.
static void scValePrologueUnusedLeaveToCottageRoomsAWestByWestBankDoor(void)
{
	sySoundPlay(SE_DOOR_OPEN);
	fdMapGridAnimateMetatiles(ValePrologueIsaacsDoorOpens, 35, 74);
	fdActorWalkTo(ACTOR_PARTY_LEADER, 102, 1206);
	fdScriptWait(3);
	scValeRequestBackdropExit(VALE_PROLOGUE_TO_COTTAGE_ROOMS_A_WEST_BY_WEST_BANK_DOOR);
}

// On the stormy night, Isaac opens the west door of the house on the river's west bank (metatile
// (6, 75)) and goes in.
static void scValePrologueLeaveToCottageRoomsBWest(void)
{
	sySoundPlay(SE_DOOR_OPEN);
	fdMapGridAnimateMetatiles(ValePrologueIsaacsDoorOpens, 35, 73);
	fdActorWalkTo(ACTOR_PARTY_LEADER, 102, 1206);
	fdScriptWait(3);
	scValeRequestBackdropExit(VALE_PROLOGUE_TO_COTTAGE_ROOMS_B_WEST);
}

// On the stormy night, Isaac opens the east door of the house on the river's west bank, onto the
// wooden deck by the water (metatile (9, 74)), and goes in.
static void scValePrologueLeaveToCottageRoomsBEast(void)
{
	sySoundPlay(SE_DOOR_OPEN);
	fdMapGridAnimateMetatiles(ValePrologueGrandparentsDoorOpens, 38, 72);
	fdActorWalkTo(ACTOR_PARTY_LEADER, 146, 1182);
	fdScriptWait(3);
	scValeRequestBackdropExit(VALE_PROLOGUE_TO_COTTAGE_ROOMS_B_EAST);
}

// Arriving in Vale on the stormy night sets the scene up for its stage of the night.
Sint32 scInit(void)
{
	fdQueueMusic(SE_AMBIENT_WATER);
	fdActorSetPos(ACTOR_FELIX_CHILD, 0, 0);
	if (gmFlagGet(FLAG_KEEP_PARTY_POSITION) != 0)
	{
		gmFlagClear(FLAG_VALE_ISAACS_DOOR_OPEN);
		gmFlagClear(FLAG_VALE_SHOVING_MANS_DOOR_OPEN);
	}
	if (gmFlagGet(FLAG_VALE_STORM_FIRST_ROCK_FELL) != 0)
	{
		fdActorSetPos(ACTOR_FIRST_ROCK, 334, 932);
		scValePrologueApplyFirstRockCollision();
	}
	if (gmFlagGet(FLAG_VALE_STORM_SECOND_ROCK_FELL) != 0)
	{
		fdActorSetPos(ACTOR_FIRST_ROCK + 1, 224, 872);
		scValePrologueApplySecondRockCollision();
	}
	if (gmFlagGet(FLAG_VALE_STORM_THIRD_ROCK_FELL) != 0)
	{
		fdActorSetPos(ACTOR_FIRST_ROCK + 2, 64, 703);
		scValePrologueApplyThirdRockCollision();
	}
	if (gmFlagGet(FLAG_VALE_STORM_FOURTH_ROCK_FELL) != 0)
	{
		fdActorSetPos(ACTOR_FIRST_ROCK + 3, 432, 1147);
		scValePrologueApplyFourthRockCollision();
	}
	// The rocks are big, so the leader can talk to them from further off: the talk target search
	// counts their distance at 10/13 (ACTOR_NEAR_TALK).
	fdActorGet(ACTOR_FIRST_ROCK)->collisionFlags     |= ACTOR_NEAR_TALK;
	fdActorGet(ACTOR_FIRST_ROCK + 1)->collisionFlags |= ACTOR_NEAR_TALK;
	fdActorGet(ACTOR_FIRST_ROCK + 2)->collisionFlags |= ACTOR_NEAR_TALK;
	fdActorGet(ACTOR_FIRST_ROCK + 3)->collisionFlags |= ACTOR_NEAR_TALK;
	fdActorGet(ACTOR_FIRST_ROCK + 4)->collisionFlags |= ACTOR_NEAR_TALK;
	fdActorGet(ACTOR_FIRST_ROCK + 5)->collisionFlags |= ACTOR_NEAR_TALK;
	fdActorGet(ACTOR_FIRST_ROCK + 6)->collisionFlags |= ACTOR_NEAR_TALK;
	fdActorGet(ACTOR_FIRST_ROCK + 7)->collisionFlags |= ACTOR_NEAR_TALK;
	// Garet walks with the party once he has joined.
	if (gmFlagGet(FLAG_VALE_STORM_GARET_JOINED) != 0)
	{
		fdActorSetPos(ACTOR_GARET_CHILD, 0, 0);
	}
	{
		ACTOR *actor;

		actor         = fdActorGet(ACTOR_BOULDER);
		actor->scaleX = toFIXED(2);
		actor->scaleY = toFIXED(2);
	}
	// The Boulder lies where it fell, or waits on the mountainside.
	if (gmFlagGet(FLAG_VALE_BOULDER_FELL) != 0)
	{
		fdActorSetPos(ACTOR_BOULDER, 114, 333);
	}
	else
	{
		fdActorSetScript(ACTOR_BOULDER, ValePrologueBoulderWaits);
	}
	if (gmFlagGet(FLAG_VALE_STORM_BOULDER_SWEPT_THEM_AWAY) != 0)
	{
		const Sint32 *table;
		scValePrologueRebuildRiverbank();
		fdActorSetPos(ACTOR_DORA, 165, 1229);
		{
			ACTOR *actor;

			actor            = fdActorGet(ACTOR_DORA);
			actor->facing    = FACING_NORTHEAST;
			actor->timer     = SQUASH_AND_STRETCH_PAUSE();
			table            = ValePrologueSquashAndStretchScript;
			actor->userValue = 1;
			fdActorSetScript(ACTOR_DORA, table);
		}
		fdActorSetPos(ACTOR_JENNA_CHILD, 165, 1254);
		{
			ACTOR *actor;

			actor            = fdActorGet(ACTOR_JENNA_CHILD);
			actor->facing    = FACING_NORTHEAST;
			actor->timer     = SQUASH_AND_STRETCH_PAUSE();
			actor->userValue = 2;
			fdActorSetScript(ACTOR_JENNA_CHILD, table);
		}
		fdActorSetPos(ACTOR_GARET_CHILD, 152, 1285);
		{
			ACTOR *actor;

			actor            = fdActorGet(ACTOR_GARET_CHILD);
			actor->facing    = FACING_NORTHEAST;
			actor->timer     = SQUASH_AND_STRETCH_PAUSE();
			actor->userValue = 3;
			fdActorSetScript(ACTOR_GARET_CHILD, table);
		}
		fdActorSetPos(ACTOR_EXHAUSTED_STRONGMAN, 184, 1304);
		{
			ACTOR *actor;

			actor            = fdActorGet(ACTOR_EXHAUSTED_STRONGMAN);
			actor->facing    = FACING_NORTHEAST;
			actor->timer     = SQUASH_AND_STRETCH_PAUSE();
			actor->userValue = 4;
			fdActorSetScript(ACTOR_EXHAUSTED_STRONGMAN, table);
		}
		fdActorSetAnimation(ACTOR_EXHAUSTED_STRONGMAN, STRONGMAN_ANIMATION_SPRAWLED);
		fdActorGet(ACTOR_GARET_CHILD)->priorityFlags         &= ~ACTOR_PRIORITY_AUTOMATIC;
		fdActorGet(ACTOR_EXHAUSTED_STRONGMAN)->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
		sySetTickEvent(scValePrologueCopyLeaderSpritePriority, EVENT_PASS_TICK);
		fdActorSetPos(ACTOR_JENNAS_FATHER, 0, 0);
		fdActorSetPos(ACTOR_JENNAS_MOTHER, 0, 0);
		fdActorSetPos(ACTOR_FELIX_CHILD, 0, 0);
		fdActorSetPos(ACTOR_BOULDER, 0, 0);
		if (gmFlagGet(FLAG_VALE_STORM_GARET_LEFT) != 0)
		{
			fdActorSetPos(ACTOR_GARET_CHILD, 0, 0);
		}
		// While they try to save Felix: Kyle and Jenna's parents at the river, Felix in it.
	}
	else if (gmFlagGet(FLAG_VALE_STORM_SAW_FELIX_HANGING_ON) != 0)
	{
		const Sint32 *table;
		fdActorSetPos(ACTOR_KYLE, 192, 1214);
		fdActorFaceDirection(ACTOR_KYLE, FACING_SOUTHEAST, 0);
		fdActorSetAnimation(ACTOR_KYLE, ANIM_KYLE_REACH_OUT);
		{
			ACTOR *actor;

			actor        = fdActorGet(ACTOR_KYLE);
			actor->timer = SQUASH_AND_STRETCH_PAUSE();
		}
		table = ValePrologueSquashAndStretchScript;
		fdActorSetScript(ACTOR_KYLE, table);
		fdActorSetPos(ACTOR_JENNAS_FATHER, 227, 1214);
		fdActorFaceDirection(ACTOR_JENNAS_FATHER, FACING_SOUTH, 0);
		fdActorSetAnimation(ACTOR_JENNAS_FATHER, ANIM_PARENT_REACH_OUT);
		{
			ACTOR *actor;

			actor        = fdActorGet(ACTOR_JENNAS_FATHER);
			actor->timer = SQUASH_AND_STRETCH_PAUSE();
		}
		fdActorSetScript(ACTOR_JENNAS_FATHER, table);
		fdActorSetPos(ACTOR_JENNAS_MOTHER, 247, 1214);
		fdActorFaceDirection(ACTOR_JENNAS_MOTHER, FACING_SOUTH, 0);
		fdActorSetAnimation(ACTOR_JENNAS_MOTHER, ANIM_PARENT_REACH_OUT);
		{
			ACTOR *actor;

			actor        = fdActorGet(ACTOR_JENNAS_MOTHER);
			actor->timer = SQUASH_AND_STRETCH_PAUSE();
		}
		fdActorSetScript(ACTOR_JENNAS_MOTHER, table);
		fdActorSetPos(ACTOR_FELIX_CHILD, 243, 1277);
		fdActorFaceDirection(ACTOR_FELIX_CHILD, FACING_NORTH, 0);
		fdActorSetSpriteFlags(fdActorGet(ACTOR_FELIX_CHILD), 0);
		fdActorSetPos(ACTOR_FIRST_ROCK + 6, 0, 0);
		fdActorSetPos(ACTOR_FIRST_ROCK + 7, 0, 0);
	}
	else
	{
		fdActorSetPos(ACTOR_FIRST_ROCK + 6, 0, 0);
		fdActorSetPos(ACTOR_FIRST_ROCK + 7, 0, 0);
	}
	// The lightning starts at once, except before Dora and Kyle's script.
	if (MainRoutineStateWork.entrance != VALE_PROLOGUE_FROM_COTTAGE_ROOMS_B_WITH_DORA
	    || gmFlagGet(FLAG_VALE_STORM_SENT_TO_PLAZA) != 0)
	{
		fdWeatherStartRain();
		fdWeatherStartLightning();
	}
	if (gmFlagGet(FLAG_VALE_ON_BRIDGE) != 0)
	{
		scValePrologueStepOntoBridge();
	}
	gmFlagSet(FLAG_VALE_STORM);
	fdMapGridCopyMetatileAttributes(29, 24, 1, 2, 26, 46);
	fdMapGridCopyMetatileAttributes(29, 25, 1, 1, 27, 46);
	fdMapGridCopyMetatileAttributes(29, 25, 1, 1, 28, 46);
	fdMapGridCopyMetatileAttributes(19, 90, 1, 1, 20, 88);
	fdMapGridCopyMetatileAttributes(19, 90, 1, 1, 20, 89);
	{
		ACTOR *actor;

		actor                 = fdActorGet(ACTOR_GARETS_CHEST);
		actor->motionFlags    = 0;
		actor->y.fixed        = toFIXED(192);
		actor->collisionFlags = ACTOR_NOT_TARGETED;
		fdActorSetSpriteFlags(actor, 0);
	}
	sySynch(1);
	if (gmFlagGet(FLAG_VALE_STORM_SENT_TO_PLAZA) == 0
	    && MainRoutineStateWork.entrance == VALE_PROLOGUE_FROM_COTTAGE_ROOMS_B_WITH_DORA)
	{
		scValePrologueDoraSendsIsaacToThePlaza();
		return 0;
	}
	// Garet drags his chest out of his house until he joins, then the chest stands by the door.
	fdActorSetAnimation(ACTOR_FELIX_CHILD, ANIM_FELIX_CLING);
	if (gmFlagGet(FLAG_VALE_STORM_GARET_JOINED) == 0)
	{
		fdScriptStart();
		fdActorSetAttachedEffect(ACTOR_GARET_CHILD,
		                         ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ATTACH);
		fdActorSetPos(ACTOR_GARET_CHILD, 400, 611);
		fdActorSetPos(ACTOR_GARETS_CHEST, 424, 627);
		fdActorWalkTo(ACTOR_GARET_CHILD, 400, 619);
		fdActorWalkToAndWait(ACTOR_GARETS_CHEST, 424, 619);
		fdActorSetAnimation(ACTOR_GARETS_CHEST, ANIM_CHEST_DRAGGED);
		fdActorSetAnimation(ACTOR_GARET_CHILD, ANIM_GARET_DRAG);
		fdScriptEnd();
	}
	else
	{
		fdScriptStart();
		fdActorSetPos(ACTOR_GARETS_CHEST, 424, 627);
		fdActorWalkToAndWait(ACTOR_GARETS_CHEST, 424, 619);
		fdActorSetAnimation(ACTOR_GARETS_CHEST, ANIM_CHEST_ROPE_DROPPED);
		fdScriptEnd();
	}
	FieldStateWork->transition      = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
	FieldStateWork->transitionFrame = 24;
	fdScriptStartScreenTransition();
	fdScriptWaitTransitionTicks();
	fdWeatherResumeLightning();
	return 0;
}

// In Vale on the stormy night, Dora comes out into the storm with Isaac to Kyle, and the two send
// Isaac on to the plaza while they go to help.
static void scValePrologueDoraSendsIsaacToThePlaza(void)
{
	Sint32 msg;

	fdScriptStart();
	fdWeatherStartRain();
	fdWeatherStartLightning();
	fdWeatherLastLightning();
	sySynch(60);
	// The camera looks down on the door of Isaac's house; Kyle waits outside.
	fdCameraSetSpeed(0.25);
	fdCameraMoveTo(316, 160, 880, 1);
	fdActorSetPos(ACTOR_KYLE, 294, 868);
	fdActorSetPos(ACTOR_PARTY_LEADER, 0, 0);
	FieldStateWork->transition      = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
	FieldStateWork->transitionFrame = MAP_TRANSITION_TICKS;
	fdScriptStartScreenTransition();
	fdScriptWaitTransitionTicks();
	fdWeatherResumeLightning();
	// Dora and Isaac come out of the door.
	sySoundPlay(SE_DOOR_OPEN);
	fdMapGridAnimateMetatiles(ValePrologueIsaacsDoorOpens, 50, 44);
	fdActorSetAttachedEffect(ACTOR_GARET_CHILD, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ATTACH);
	fdActorSetSpeed(ACTOR_DORA, 0.8);
	fdActorSetSpeed(ACTOR_PARTY_LEADER, 0.8);
	fdActorSetSpeed(ACTOR_KYLE, 0.8);
	fdActorSetPos(ACTOR_DORA, 342, 890);
	fdActorWalkToAndWait(ACTOR_DORA, 342, 905);
	fdPlayQueuedMusic();
	fdActorWalkTo(ACTOR_DORA, 296, 905);
	fdActorSetPos(ACTOR_PARTY_LEADER, 342, 890);
	fdActorWalkTo(ACTOR_PARTY_LEADER, 342, 890);
	fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 342, 905);
	fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 318, 905);
	fdActorSetAnimation(ACTOR_DORA, ANIM_STAND);
	fdActorRunRepeatedMotion(ACTOR_DORA, 1);
	fdActorFaceDirection(ACTOR_DORA, FACING_NORTH, 60);
	// Kyle does not think the elders can hold the Boulder, and sends the two to take refuge in the
	// plaza.
	msg = MsgValeKyleAbleStop;
	fdScriptSetMessageId(msg);
	// "Kyle... Will they be able to stop the Boulder?"
	fdScriptShowMessage(ACTOR_DORA, 0);
	fdActorWalkToAndWait(ACTOR_KYLE, 294, 838);
	fdScriptWait(40);
	fdActorSetAnimationAndWait(ACTOR_KYLE, ANIM_SHAKE_HEAD);
	// "I don't think so... Not for long, anyway..."
	fdScriptShowMessage(ACTOR_KYLE, 0);
	fdActorFaceEachOther(ACTOR_PARTY_LEADER, ACTOR_DORA, 0);
	fdScriptWait(40);
	fdActorFaceDirection(ACTOR_KYLE, FACING_SOUTH, 0);
	// "You two go on ahead and take refuge in the plaza."
	fdScriptShowMessageThenWait(ACTOR_KYLE, 0, 20);
	fdActorShowEmote(ACTOR_DORA, EMOTE_IN_FRONT | EMOTE_QUESTION, 20);
	fdActorFaceDirection(ACTOR_DORA, FACING_NORTH, 10);
	// "Aren't you coming?"
	fdScriptShowMessageThenWait(ACTOR_DORA, 0, 10);
	fdActorSetAnimationAndWait(ACTOR_KYLE, ANIM_SHAKE_HEAD);
	// "I need to help evacuate the other villagers."
	fdScriptShowMessage(ACTOR_KYLE, 0);
	fdActorSetAttachedEffect(ACTOR_DORA, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdScriptWait(30);
	fdActorFaceDirection(ACTOR_DORA, FACING_EAST, 50);
	fdActorFaceDirection(ACTOR_DORA, FACING_NORTH, 10);
	// Dora runs to Kyle to help him, and he sends her back to Isaac.
	fdActorSetSpeed(ACTOR_DORA, 1.5);
	fdActorWalkToAndWait(ACTOR_DORA, 289, 883);
	fdActorFaceDirection(ACTOR_DORA, FACING_NORTHEAST, 0);
	// "Let me help you, Kyle!"
	fdScriptShowMessage(ACTOR_DORA, 0);
	fdActorRunRepeatedMotion(ACTOR_KYLE, 2);
	// "It's too dangerous, Dora. Please, just take good care of Isaac!"
	fdScriptShowMessage(ACTOR_KYLE, 0);
	fdActorSetAnimationAndWait(ACTOR_DORA, ANIM_SHAKE_HEAD);
	// "Isaac is old enough to get to the plaza on his own."
	fdScriptShowMessageThenWait(ACTOR_DORA, 0, 10);
	fdActorFaceDirection(ACTOR_DORA, FACING_SOUTHEAST, 10);
	// She asks again on every no.
	fdScriptSetMessageId(msg + 8);
	// "You can find your way, can't you, Isaac?"
	fdScriptOpenMessage(ACTOR_DORA, 0);
	fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 302, 905);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
	while (fdScriptAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 1)
	{
		fdActorRunRepeatedMotion(ACTOR_DORA, 1);
		fdScriptSetMessageId(MsgValeBigBoyWhy);
		// "You're a big boy now, Isaac. Why don't you go on your own?"
		fdScriptOpenMessage(ACTOR_DORA, 0);
	}
	fdActorSetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
	fdScriptSetMessageId(MsgValeKnowWayGo);
	// "You know the way-just go south to get to the plaza. Be careful!"
	fdScriptShowMessageThenWait(ACTOR_DORA, 0, 10);
	fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
	// Kyle and Dora run north toward the Boulder and out of sight.
	fdActorSetSpeed(ACTOR_KYLE, 1.5);
	fdActorWalkTo(ACTOR_KYLE, 297, 750);
	fdScriptWait(10);
	fdActorWalkToAndWait(ACTOR_DORA, 297, 750);
	fdActorSetPos(ACTOR_DORA, 0, 0);
	fdActorSetPos(ACTOR_KYLE, 0, 0);
	fdActorSetAnimation(ACTOR_KYLE, ANIM_STAND);
	fdActorSetAnimation(ACTOR_GARETS_CHEST, ANIM_CHEST_DRAGGED);
	fdActorSetAnimation(ACTOR_GARET_CHILD, ANIM_GARET_DRAG);
	gmFlagClear(FLAG_SHOW_AREA_NAME);
	gmFlagSet(FLAG_VALE_STORM_SENT_TO_PLAZA);
	gmFlagSet(FLAG_VALE_ISAACS_DOOR_OPEN);
	fdScriptEnd();
}

// On the stormy night, a huge rock falls across the path just south of Isaac's house as he walks
// past (metatiles (19, 57..59)), and Garet, once with Isaac, says they cannot get around it.
static void scValePrologueFirstRockFalls(void)
{
	ACTOR *rock;
	FIXED  targetHeight;
	SPR   *rockSprite;

	if (fdScriptGetFlag(FLAG_VALE_STORM_FIRST_ROCK_PASSED) == 0)
	{
		fdScriptStart();
		if (fdScriptGetFlag(FLAG_VALE_STORM_FIRST_ROCK_FELL) == 0)
		{
			rock       = fdActorGet(ACTOR_FIRST_ROCK);
			rockSprite = rock->sprite;
			// A rumble, a crash, and the rock bounces down from the east.
			fdMapGridSetShake(4, 4, 1);
			sySoundPlay(SE_RUMBLE);
			sySynch(40);
			sySoundPlay(SE_CRASH);
			rock->priorityFlags        &= ~ACTOR_PRIORITY_AUTOMATIC;
			rockSprite->image.priority  = 1;
			fdActorSetPos(ACTOR_FIRST_ROCK, 473, 932);
			rock->speedLimit    = toFIXED(3) / 2;
			rock->acceleration  = toFIXED(3) / 2;
			targetHeight        = (rock->y.fixed + toFIXED(240));
			rock->y.fixed      += toFIXED(240);
			rock->targetY       = targetHeight;
			rock->bounce        = toFIXED(2) / 5;
			fdActorWalkToAndWait(ACTOR_FIRST_ROCK, 344, 932);
			rockSprite->image.priority  = 3;
			rock->priorityFlags        |= ACTOR_PRIORITY_AUTOMATIC;
			fdScriptWait(40);
			// It lands: the rumble fades and the shaking dies down.
			sySoundPlay(SOUND_FADE_OUT_ACTION_SE);
			fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
			fdMapGridWaitForShakeToSettle();
			fdPlayQueuedMusic();
			gmFlagSet(FLAG_VALE_STORM_FIRST_ROCK_FELL);
		}
		// The rock now blocks the path.
		scValePrologueApplyFirstRockCollision();
		gmFlagSet(FLAG_VALE_STORM_FIRST_ROCK_PASSED);
		if (gmFlagGet(FLAG_VALE_STORM_GARET_JOINED) != 0)
		{
			if (gmFlagGet(FLAG_VALE_STORM_BOULDER_SWEPT_THEM_AWAY) == 0)
			{
				if (gmFlagGet(FLAG_VALE_STORM_GARET_SAW_BLOCKED_PATH) == 0)
				{
					// Garet comes out beside Isaac on whichever side of the rock he stands.
					if (fdActorGet(ACTOR_PARTY_LEADER)->y.fixed > toFIXED(128))
					{
						scValePrologueGaretTalksAboutBlockedPath(326, 918);
						fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 291, 918);
					}
					else
					{
						scValePrologueGaretTalksAboutBlockedPath(335, 957);
					}
					gmFlagSet(FLAG_VALE_STORM_GARET_SAW_BLOCKED_PATH);
				}
			}
		}
		fdScriptEnd();
	}
}

// Applies the metatile behaviors left by the first rock (ACTOR_FIRST_ROCK).
static void scValePrologueApplyFirstRockCollision(void)
{
	fdMapGridCopyMetatileAttributes(29, 64, 1, 1, 21, 57);
	fdMapGridCopyMetatileAttributes(29, 64, 1, 1, 21, 58);
	fdMapGridCopyMetatileAttributes(29, 64, 1, 1, 22, 58);
	fdMapGridCopyMetatileAttributes(29, 64, 1, 1, 20, 58);
	fdMapGridCopyMetatileAttributes(28, 20, 1, 1, 20, 57);
}

// On the stormy night, a huge rock falls down onto the path below the house north-west of Isaac's,
// by the river, as he walks past (metatiles (12, 52) and (14, 55)).
static void scValePrologueSecondRockFalls(void)
{
	ACTOR *rock;
	FIXED  targetHeight;

	if (fdScriptGetFlag(FLAG_VALE_STORM_SECOND_ROCK_PASSED) == 0)
	{
		fdScriptStart();
		if (fdScriptGetFlag(FLAG_VALE_STORM_SECOND_ROCK_FELL) == 0)
		{
			rock = fdActorGet(ACTOR_FIRST_ROCK + 1);
			// A rumble, then the rock rolls down the slope from the north-east.
			fdMapGridSetShake(4, 4, 1);
			sySoundPlay(SE_RUMBLE);
			sySynch(40);
			sySoundPlay(SE_CRASH);
			fdActorSetPos(ACTOR_FIRST_ROCK + 1, 381, 808);
			rock->speedLimit    = toFIXED(3) / 2;
			rock->acceleration  = toFIXED(3) / 2;
			targetHeight        = (rock->y.fixed + toFIXED(256));
			rock->y.fixed      += toFIXED(256);
			rock->targetY       = targetHeight;
			rock->bounce        = toFIXED(1) / 2;
			fdActorWalkToAndWait(ACTOR_FIRST_ROCK + 1, 290, 833);
			fdActorSetSpritePriority(ACTOR_FIRST_ROCK + 1, 1);
			fdActorWalkToAndWait(ACTOR_FIRST_ROCK + 1, 258, 852);
			fdActorSetSpritePriority(ACTOR_FIRST_ROCK + 1, 2);
			fdActorWalkToAndWait(ACTOR_FIRST_ROCK + 1, 224, 872);
			fdScriptWait(40);
			// It lands: the rumble fades and the shaking dies down.
			sySoundPlay(SOUND_FADE_OUT_ACTION_SE);
			fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
			fdMapGridWaitForShakeToSettle();
			fdPlayQueuedMusic();
			gmFlagSet(FLAG_VALE_STORM_SECOND_ROCK_FELL);
		}
		scValePrologueApplySecondRockCollision();
		gmFlagSet(FLAG_VALE_STORM_SECOND_ROCK_PASSED);
		if (gmFlagGet(FLAG_VALE_STORM_GARET_JOINED) != 0)
		{
			if (gmFlagGet(FLAG_VALE_STORM_BOULDER_SWEPT_THEM_AWAY) == 0)
			{
				if (gmFlagGet(FLAG_VALE_STORM_GARET_SAW_BLOCKED_PATH) == 0)
				{
					// Garet comes out beside Isaac on whichever side of the rock he stands.
					if (fdActorGet(ACTOR_PARTY_LEADER)->y.fixed > toFIXED(128))
					{
						scValePrologueGaretTalksAboutBlockedPath(219, 843);
						fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 179, 829);
					}
					else
					{
						scValePrologueGaretTalksAboutBlockedPath(214, 908);
						fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 219, 911);
					}
					gmFlagSet(FLAG_VALE_STORM_GARET_SAW_BLOCKED_PATH);
				}
			}
		}
		fdScriptEnd();
	}
}

// Applies the metatile behaviors left by the second rock (ACTOR_FIRST_ROCK + 1).
static void scValePrologueApplySecondRockCollision(void)
{
	fdMapGridCopyMetatileAttributes(29, 23, 1, 1, 15, 53);
	fdMapGridCopyMetatileAttributes(29, 23, 1, 1, 14, 53);
	fdMapGridCopyMetatileAttributes(29, 23, 1, 1, 13, 53);
	fdMapGridCopyMetatileAttributes(26, 20, 2, 1, 14, 52);
	fdMapGridCopyMetatileAttributes(25, 21, 1, 1, 13, 54);
	fdMapGridCopyMetatileAttributes(25, 21, 1, 1, 15, 54);
	fdMapGridCopyMetatileAttributes(14, 53, 1, 1, 14, 54);
	fdMapGridCopyMetatileAttributes(13, 55, 1, 1, 15, 55);
}

// On the stormy night, a huge rock falls down onto the path along the village's west edge, between
// the two paths to Kraden's cottage, as Isaac walks past (metatiles (2..5, 40..45)).
static void scValePrologueThirdRockFalls(void)
{
	if (fdScriptGetFlag(FLAG_VALE_STORM_THIRD_ROCK_PASSED) == 0)
	{
		fdScriptStart();
		if (fdScriptGetFlag(FLAG_VALE_STORM_THIRD_ROCK_FELL) == 0)
		{
			ACTOR *actor    = fdActorGet(ACTOR_FIRST_ROCK + 2);
			ACTOR *leader   = fdActorGet(ACTOR_PARTY_LEADER);
			Uint16 priority = leader->sprite->image.priority;
			Uint8  flags    = leader->priorityFlags;

			// A rumble, then the rock drops in from the west while Isaac is drawn behind it.
			fdMapGridSetShake(4, 4, 1);
			sySoundPlay(SE_RUMBLE);
			sySynch(40);
			sySoundPlay(SE_CRASH);
			fdActorSetSpritePriority(ACTOR_PARTY_LEADER, 3);
			fdActorGet(ACTOR_PARTY_LEADER)->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
			fdActorSetPos(ACTOR_FIRST_ROCK + 2, 0, 703);
			actor->speedLimit    = toFIXED(3) / 2;
			actor->acceleration  = toFIXED(3) / 2;
			actor->y.fixed      += toFIXED(80);
			actor->targetY       = actor->y.fixed;
			actor->bounce        = toFIXED(1) / 2;
			fdActorWalkToAndWait(ACTOR_FIRST_ROCK + 2, 64, 703);
			fdScriptWait(40);
			// It lands: the rumble fades, and Isaac is drawn as before.
			sySoundPlay(SOUND_FADE_OUT_ACTION_SE);
			fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
			fdMapGridWaitForShakeToSettle();
			fdPlayQueuedMusic();
			gmFlagSet(FLAG_VALE_STORM_THIRD_ROCK_FELL);
			fdActorSetSpritePriority(ACTOR_PARTY_LEADER, priority);
			fdActorGet(ACTOR_PARTY_LEADER)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
			leader->priorityFlags                          = flags;
		}
		scValePrologueApplyThirdRockCollision();
		gmFlagSet(FLAG_VALE_STORM_THIRD_ROCK_PASSED);
		if (gmFlagGet(FLAG_VALE_STORM_GARET_JOINED) != 0)
		{
			if (gmFlagGet(FLAG_VALE_STORM_BOULDER_SWEPT_THEM_AWAY) == 0)
			{
				if (gmFlagGet(FLAG_VALE_STORM_GARET_SAW_BLOCKED_PATH) == 0)
				{
					// Garet comes out beside Isaac on whichever side of the rock he stands.
					if (fdActorGet(ACTOR_PARTY_LEADER)->z.fixed < toFIXED(693))
					{
						scValePrologueGaretTalksAboutBlockedPath(62, 669);
						fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 27, 627);
					}
					else
					{
						scValePrologueGaretTalksAboutBlockedPath(75, 715);
						fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 67, 757);
					}
					gmFlagSet(FLAG_VALE_STORM_GARET_SAW_BLOCKED_PATH);
				}
			}
		}
		fdScriptEnd();
	}
}

// Applies the metatile behaviors left by the third rock (ACTOR_FIRST_ROCK + 2).
static void scValePrologueApplyThirdRockCollision(void)
{
	fdMapGridCopyMetatileAttributes(29, 22, 1, 1, 3, 42);
	fdMapGridCopyMetatileAttributes(29, 21, 1, 1, 2, 42);
	fdMapGridCopyMetatileAttributes(29, 21, 1, 1, 4, 42);
	fdMapGridCopyMetatileAttributes(23, 20, 3, 1, 2, 43);
}

// On the stormy night, a huge rock falls down onto the path east of Jenna's grandparents' house, by
// the river, as Isaac walks past (metatiles (25..28, 68..74)).
static void scValePrologueFourthRockFalls(void)
{
	ACTOR *rock;
	FIXED  targetHeight;

	if (fdScriptGetFlag(FLAG_VALE_STORM_FOURTH_ROCK_PASSED) == 0)
	{
		fdScriptStart();
		if (fdScriptGetFlag(FLAG_VALE_STORM_FOURTH_ROCK_FELL) == 0)
		{
			rock = fdActorGet(ACTOR_FIRST_ROCK + 3);
			// A rumble, then the rock rolls in from the east.
			fdMapGridSetShake(4, 4, 1);
			sySoundPlay(SE_RUMBLE);
			sySynch(40);
			sySoundPlay(SE_CRASH);
			fdActorSetPos(ACTOR_FIRST_ROCK + 3, 474, 1147);
			rock->speedLimit    = toFIXED(1);
			rock->acceleration  = toFIXED(1);
			targetHeight        = (rock->y.fixed + toFIXED(72));
			rock->y.fixed      += toFIXED(72);
			rock->targetY       = targetHeight;
			rock->bounce        = toFIXED(1) / 2;
			fdActorWalkToAndWait(ACTOR_FIRST_ROCK + 3, 432, 1147);
			fdScriptWait(40);
			// It lands: the rumble fades and the shaking dies down.
			sySoundPlay(SOUND_FADE_OUT_ACTION_SE);
			fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
			fdMapGridWaitForShakeToSettle();
			fdPlayQueuedMusic();
			gmFlagSet(FLAG_VALE_STORM_FOURTH_ROCK_FELL);
		}
		scValePrologueApplyFourthRockCollision();
		gmFlagSet(FLAG_VALE_STORM_FOURTH_ROCK_PASSED);
		if (gmFlagGet(FLAG_VALE_STORM_GARET_JOINED) != 0)
		{
			if (gmFlagGet(FLAG_VALE_STORM_BOULDER_SWEPT_THEM_AWAY) == 0)
			{
				if (gmFlagGet(FLAG_VALE_STORM_GARET_SAW_BLOCKED_PATH) == 0)
				{
					// Garet comes out beside Isaac on whichever side of the rock he stands.
					if (fdActorGet(ACTOR_PARTY_LEADER)->z.fixed < toFIXED(1146))
					{
						scValePrologueGaretTalksAboutBlockedPath(412, 1120);
						fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 414, 1068);
					}
					else
					{
						scValePrologueGaretTalksAboutBlockedPath(445, 1172);
						fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 447, 1227);
					}
					gmFlagSet(FLAG_VALE_STORM_GARET_SAW_BLOCKED_PATH);
				}
			}
		}
		fdScriptEnd();
	}
}

// Applies the metatile behaviors left by the fourth rock (ACTOR_FIRST_ROCK + 3).
static void scValePrologueApplyFourthRockCollision(void)
{
	fdMapGridCopyMetatileAttributes(29, 20, 1, 1, 26, 71);
	fdMapGridCopyMetatileAttributes(29, 20, 1, 1, 26, 70);
	fdMapGridCopyMetatileAttributes(29, 20, 1, 1, 27, 70);
	fdMapGridCopyMetatileAttributes(28, 21, 1, 1, 28, 71);
	fdMapGridCopyMetatileAttributes(28, 22, 1, 1, 27, 72);
}

// A stout man runs out of the door of the house north-west of Isaac's and knocks him back. "Huh?
// Isaac? Did you come to get me?" He says sorry, says they had better get to the plaza before the
// Boulder hits, and runs off.
static void scValePrologueBumpIntoShovingMan(void)
{
	Sint32 msg;

	fdScriptStart();
	fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 262, 810);
	fdActorSetPos(ACTOR_SHOVING_MAN, 262, 805);
	fdActorWalkTo(ACTOR_SHOVING_MAN, 262, 825);
	fdActorSetSpeed(ACTOR_PARTY_LEADER, 2);
	fdActorJump(ACTOR_PARTY_LEADER, 2, 0);
	fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 282, 855);
	fdActorSetAnimation(ACTOR_SHOVING_MAN, ANIM_STAND);
	fdActorJump(ACTOR_PARTY_LEADER, 4, 0);
	fdActorFaceEachOther(ACTOR_PARTY_LEADER, ACTOR_SHOVING_MAN, 0);
	fdPlayQueuedMusic();
	fdScriptWait(30);
	fdActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
	fdActorShowEmote(ACTOR_SHOVING_MAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
	msg = MsgValeHuh;
	fdScriptSetMessageId(msg);
	// "Huh? Isaac?"
	fdScriptShowMessage(ACTOR_SHOVING_MAN, 0);
	fdScriptWait(20);
	// "Did you come to get me?" Yes: "Oh... Sorry to shove you like that." No: "This isn't the time
	// for games! Foolish boy!"
	fdScriptAskYesNo(ACTOR_SHOVING_MAN, 0);
	fdActorRunRepeatedMotion(ACTOR_SHOVING_MAN, 2);
	fdScriptSetMessageId(msg + 4);
	// "Well...we'd better get to the plaza before the Boulder hits."
	fdScriptShowMessageThenWait(ACTOR_SHOVING_MAN, 0, 20);
	fdActorRunScriptAndWait(ACTOR_SHOVING_MAN, ValePrologueShovingManScript);
	gmFlagSet(FLAG_VALE_STORM_BUMPED_INTO_VILLAGER);
	fdScriptEnd();
}

// On the stormy night, walking up beside Garet, who is dragging his chest of valuables along the
// path north of his house (metatiles (21, 38..41)), Isaac hears him struggling. "Ugh... Hrnghhh..."
static void scValePrologueHearGaretStruggling(void)
{
	if (fdScriptGetFlag(FLAG_VALE_STORM_HEARD_GARET) == 0)
	{
		if (gmFlagGet(FLAG_VALE_STORM_GARET_JOINED) == 0)
		{
			fdScriptStart();
			fdScriptSetMessageId(MsgValeUghHrnghhh);
			// "Ugh... Hrnghhh..."
			fdScriptShowMessageThenWait(ACTOR_GARET_CHILD, 0, 20);
			fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
			fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 382, 619);
			fdActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_GARET_CHILD, 0);
			fdActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
			fdScriptWait(30);
			// "Isaac, what do you want? I'm trying to save my things!"
			fdScriptShowMessage(ACTOR_GARET_CHILD, 0);
			gmFlagSet(FLAG_VALE_STORM_HEARD_GARET);
			fdScriptEnd();
		}
	}
}

// On the stormy night, if Isaac walks on west past Garet and his chest (metatiles (19, 39..42)),
// Garet calls after him, asking whether he will just leave him there.
static void scValePrologueGaretCallsAfterIsaac(void)
{
	if (gmFlagGet(FLAG_VALE_STORM_GARET_JOINED) == 0 && gmFlagGet(FLAG_VALE_STORM_HEARD_GARET) != 0)
	{
		fdScriptStart();
		fdActorRunRepeatedMotion(ACTOR_GARET_CHILD, 2);
		fdScriptWait(20);
		fdScriptSetMessageId(MsgValeDontLeaveMeHere);
		scValePrologueGaretLeavesHisChest();
		fdScriptEnd();
	}
}

// In Vale on the stormy night, talking to Garet is about whether to leave his things behind, or,
// after the river, about the Boulder.
static void scValePrologueTalkToGaret(void)
{
	if (gmFlagGet(FLAG_VALE_STORM_BOULDER_SWEPT_THEM_AWAY) != 0)
	{
		fdScriptStart();
		fdActorFaceActor(ACTOR_GARET_CHILD, ACTOR_PARTY_LEADER, 0);
		fdScriptWait(20);
		fdScriptSetMessageId(MsgValeBoulderNeedGet);
		// "Isaac... The Boulder... We need to get help, or everyone..."
		fdScriptShowMessage(ACTOR_GARET_CHILD, 0);
		fdActorFaceDirection(ACTOR_GARET_CHILD, FACING_NORTHEAST, 10);
		fdScriptEnd();
	}
	else
	{
		if (gmFlagGet(FLAG_VALE_STORM_GARET_JOINED) == 0)
		{
			fdScriptStart();
			fdScriptSetMessageId(MsgValeWantDumpStuff);
			scValePrologueGaretLeavesHisChest();
			fdScriptEnd();
		}
	}
}

// Garet asks the question the caller chose and, on either answer, drops his chest and joins the
// party.
static void scValePrologueGaretLeavesHisChest(void)
{
	Sint32 agreedToLeaveBelongings;

	fdScriptOpenMessage(ACTOR_GARET_CHILD, 0);
	fdActorFaceEachOther(ACTOR_PARTY_LEADER, ACTOR_GARET_CHILD, 0);
	agreedToLeaveBelongings = 0;
	if (fdScriptAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0)
	{
		fdScriptSetMessageId(MsgValeThinkForgetThings);
		agreedToLeaveBelongings = 1;
	}
	else
	{
		fdScriptSetMessageId(MsgValeRockHitsLose);
	}
	fdScriptWait(20);
	fdScriptShowMessageThenWait(ACTOR_GARET_CHILD, 0, 40);
	fdActorSetAttachedEffect(ACTOR_GARET_CHILD, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_REMOVE);
	fdActorSetAnimation(ACTOR_GARETS_CHEST, ANIM_CHEST_ROPE_DROPPED);
	fdActorSetAnimation(ACTOR_GARET_CHILD, ANIM_STAND);
	fdScriptWait(40);
	fdActorFaceActor(ACTOR_GARET_CHILD, ACTOR_PARTY_LEADER, 0);
	fdScriptWait(20);
	fdActorSetAnimationAndWait(ACTOR_GARET_CHILD, ANIM_NOD);
	if (agreedToLeaveBelongings != 0)
	{
		fdScriptSetMessageId(MsgValeKnowRightOk);
	}
	else
	{
		fdScriptSetMessageId(MsgValeRightDitchStuff);
	}
	fdScriptShowMessage(ACTOR_GARET_CHILD, 0);
	scWalkIntoLeader(ACTOR_GARET_CHILD);
	fdScriptAddPartyMember(GARET, 1);
	gmFlagSet(FLAG_VALE_STORM_GARET_JOINED);
}

// Unused: another version of Garet joining Isaac on the stormy night, which the game never plays.
static void scValePrologueUnusedGaretJoinsIsaac(void)
{
	Sint32 msg;

	if (fdScriptGetFlag(FLAG_VALE_STORM_GARET_JOINED) == 0)
	{
		fdScriptStart();
		fdActorSetAttachedEffect(ACTOR_GARET_CHILD,
		                         ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_REMOVE);
		msg = MsgValeHey;
		fdScriptSetMessageId(msg);
		// "Hey! Isaac!"
		fdScriptShowMessage(ACTOR_GARET_CHILD, 0);
		fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
		fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
		fdCameraSetSpeed(0.4);
		fdCameraMoveTo(256, CAMERA_KEEP, 588, 1);
		fdActorSetSpeed(ACTOR_GARET_CHILD, 2);
		fdActorRunScriptAndWait(ACTOR_GARET_CHILD, ValePrologueUnusedGaretRunsUpScript);
		fdActorFaceEachOther(ACTOR_PARTY_LEADER, ACTOR_GARET_CHILD, 0);
		fdScriptWait(30);
		fdActorSetScript(ACTOR_GARET_CHILD, ValePrologueUnusedGaretPantingScript);
		// "*Hah* *Hah*"
		fdScriptShowMessage(ACTOR_GARET_CHILD, 0);
		fdActorGet(ACTOR_GARET_CHILD)->scaleY = toFIXED(1);
		fdActorRunRepeatedMotion(ACTOR_GARET_CHILD, 1);
		fdScriptWait(20);
		// "Are you trapped, too?" Yes: "Hmph! It's not like you to be caught unprepared, Isaac."
		// No: "Oh, you got held up seeing off the elders who went to help..."
		fdScriptAskYesNo(ACTOR_GARET_CHILD, 0);
		fdScriptWait(40);
		fdActorRunRepeatedMotion(ACTOR_GARET_CHILD, 1);
		fdScriptSetMessageId(msg + 5);
		// "You came just in time... We can go together."
		fdScriptShowMessageThenWait(ACTOR_GARET_CHILD, 0, 20);
		fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
		fdActorSetAnimationAndWait(ACTOR_GARET_CHILD, ANIM_NOD);
		// "Everyone's gathering down south, in the town plaza."
		fdScriptShowMessage(ACTOR_GARET_CHILD, 0);
		fdActorSetSpeed(ACTOR_GARET_CHILD, 1);
		scWalkIntoLeader(ACTOR_GARET_CHILD);
		fdScriptAddPartyMember(GARET, 1);
		fdActorSetAnimation(ACTOR_GARETS_CHEST, ANIM_CHEST_ROPE_DROPPED);
		gmFlagSet(FLAG_VALE_STORM_GARET_JOINED);
		fdScriptEnd();
	}
}

// On the stormy night, as Isaac walks up to the foot of the steps up to the Sanctum's door, beside
// the waterfall (metatiles (15..16, 30)), Garet stops him and turns him back. "We shouldn't go
// north... that'll take us toward Mt. Aleph."
//
// Garet steps out from behind Isaac, faces him, then turns south: "The plaza is over that way. We
// need to head south." He falls back in behind, and Isaac walks back from the steps.
static void scValePrologueWarnAboutMtAleph(void)
{
	ACTOR *initLeader;

	fdScriptStart();
	initLeader = fdActorGet(ACTOR_PARTY_LEADER);
	if (initLeader != NULL)
	{
		fdActorSetPosFX(ACTOR_GARET_CHILD, initLeader->x.fixed, initLeader->z.fixed);
	}
	fdActorSetSpeed(ACTOR_GARET_CHILD, 1);
	fdActorWalkToAndWait(ACTOR_GARET_CHILD, 281, 507);
	fdActorFaceEachOther(ACTOR_GARET_CHILD, ACTOR_PARTY_LEADER, 0);
	fdScriptWait(30);
	fdScriptSetMessageId(MsgValeNorthLeadsToMtAleph);
	// "We shouldn't go north... that'll take us toward Mt. Aleph."
	fdScriptShowMessage(ACTOR_GARET_CHILD, 0);
	fdActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_GARET_CHILD, 0);
	fdScriptWait(10);
	fdActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 1);
	fdScriptWait(20);
	fdActorFaceDirection(ACTOR_GARET_CHILD, FACING_SOUTH, 0);
	// "The plaza is over that way. We need to head south."
	fdScriptShowMessage(ACTOR_GARET_CHILD, 0);
	scWalkIntoLeader(ACTOR_GARET_CHILD);
	fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 256, 517);
	fdScriptEnd();
}

// Garet steps out of Isaac to (`x`, `z`), in pixels, beside a fallen rock, says they cannot get
// around it, and steps back in. The four rock scripts play it once between them.
static void scValePrologueGaretTalksAboutBlockedPath(Sint32 x, Sint32 z)
{
	ACTOR *leaderAtStart;
	FIXED  acceleration = (toFIXED(1) / 2);

	leaderAtStart       = fdActorGet(ACTOR_PARTY_LEADER);
	if (leaderAtStart != NULL)
	{
		fdActorSetPosFX(ACTOR_GARET_CHILD, leaderAtStart->x.fixed, leaderAtStart->z.fixed);
	}
	fdActorSetSpeedFX(ACTOR_GARET_CHILD, toFIXED(1), acceleration);
	fdActorWalkToAndWait(ACTOR_GARET_CHILD, x, z);
	fdActorFaceEachOther(ACTOR_PARTY_LEADER, ACTOR_GARET_CHILD, 0);
	fdScriptWait(20);
	fdActorSetAttachedEffect(ACTOR_PARTY_LEADER, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdScriptWait(40);
	fdScriptSetMessageId(MsgValeCantGetAroundThisRock);
	// "We can't get around this rock!"
	fdScriptShowMessage(ACTOR_GARET_CHILD, 0);
	fdActorRunRepeatedMotion(ACTOR_GARET_CHILD, 2);
	// "We'll have to find another way."
	fdScriptShowMessage(ACTOR_GARET_CHILD, 0);
	fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
	scWalkIntoLeader(ACTOR_GARET_CHILD);
}

// On the stormy night, walking along the path west of Garet's house (metatiles (4..5, 35)), Isaac
// and Garet see the elders holding the Boulder up on the mountainside with their Psynergy. It slips
// toward the village, and the elders tell the boys to run.
static void scValePrologueWatchBoulderFall(void)
{
	Uint32     i;
	ACTOR     *movingActor;
	Sint32    *shakeShift;
	ACTOR     *actor;
	EVENT_FUNC updateEffects;
	FIXED      boulderHeight;

	if (gmFlagGet(FLAG_VALE_BOULDER_FELL) == 0)
	{
		fdScriptStart();
		fdScriptAppendActorPlacements(ValePrologueBoulderElders);
		scStartScriptPsynergy();
		sySynch(1);
		sySoundPlay(SE_RUMBLE);
		fdMapGridSetShake(2, 2, 1);
		fdScriptWait(30);
		fdMapGridSetShake(3, 3, 1);
		sySoundPlay(SE_CRASH);
		fdScriptWait(30);
		actor = fdActorGet(ACTOR_PARTY_LEADER);
		if (actor != NULL)
		{
			fdActorSetPosFX(ACTOR_GARET_CHILD, actor->x.fixed, actor->z.fixed);
		}
		fdActorSetSpeed(ACTOR_PARTY_LEADER, 2);
		fdActorSetSpeed(ACTOR_GARET_CHILD, 2);
		fdActorSetScript(ACTOR_PARTY_LEADER, ValePrologueBoulderFallLeaderPath);
		fdActorRunScriptAndWait(ACTOR_GARET_CHILD, ValePrologueBoulderFallGaretPath);
		fdActorWaitForScriptEndById(ACTOR_PARTY_LEADER);
		fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
		fdActorShowEmote(ACTOR_GARET_CHILD, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
		fdMapGridSetShake(4, 4, 1);
		sySoundPlay(SE_CRASH);
		fdScriptWait(40);
		fdMapGridSetShake(5, 5, 1);
		sySoundPlay(SE_CRASH);
		fdScriptWait(20);
		fdActorSetAttachedEffect(ACTOR_PARTY_LEADER,
		                         ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
		fdActorSetAttachedEffect(ACTOR_GARET_CHILD,
		                         ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
		fdScriptWait(40);
		fdActorSetAnimation(ACTOR_ELDER_EAST, ANIM_BEARDED_ELDER_HOLD_BACK);
		fdActorSetAnimation(ACTOR_ELDER_WEST, ANIM_BEARDED_ELDER_HOLD_BACK);
		fdActorSetAnimation(ACTOR_ELDER_FAR_EAST, YOUNG_MAN_WITH_BROWN_HAIR_ANIMATION_HOLD_BACK);
		fdActorSetAnimation(ACTOR_ELDER_FAR_WEST, YOUNG_MAN_WITH_BROWN_HAIR_ANIMATION_HOLD_BACK);
		fdActorGet(ACTOR_ELDER_FAR_EAST)->scaleX = -toFIXED(1);
		fdActorSetSpritePriority(ACTOR_ELDER_EAST, 2);
		fdActorSetSpritePriority(ACTOR_ELDER_WEST, 2);
		fdActorSetSpritePriority(ACTOR_ELDER_FAR_EAST, 3);
		fdActorSetSpritePriority(ACTOR_ELDER_FAR_WEST, 3);
		fdScriptSetMessageId(MsgValeTheBoulderIsFalling);
		// "Ahhh! The Boulder! It's falling!"
		fdScriptShowMessageThenWait(ACTOR_ELDER_EAST_STAND_IN, 0, 20);
		fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
		fdActorFaceDirection(ACTOR_GARET_CHILD, FACING_NORTH, 20);
		fdCameraSetSpeed(4);
		fdCameraMoveTo(112, CAMERA_KEEP, 331, 1);
		fdCameraWaitForMove();
		for (i = 0; i < ELDERS_FLASH_TICKS; i++)
		{
			scValePrologueFlashPaletteAndSpawnEffects(fdActorGet(ACTOR_ELDER_EAST));
			scValePrologueFlashPaletteAndSpawnEffects(fdActorGet(ACTOR_ELDER_WEST));
			scValePrologueFlashPaletteAndSpawnEffects(fdActorGet(ACTOR_ELDER_FAR_EAST));
			scValePrologueFlashPaletteAndSpawnEffects(fdActorGet(ACTOR_ELDER_FAR_WEST));
			sySynch(1);
		}
		shakeShift    = &ShakeShift;
		updateEffects = scValePrologueUpdateBoulderMapEffects;
		ShakeDone     = 0;
		*shakeShift   = 0;
		sySetOrUpdateTickEvent(updateEffects, EVENT_PASS_TICK);
		sySetTickEvent(scValePrologueCycleBoulderPalette, EVENT_PASS_TICK);
		fdScriptWait(40);
		*shakeShift = 1;
		fdScriptWait(30);
		fdActorSetPos(ACTOR_BOULDER, 114, 290);
		movingActor          = fdActorGet(ACTOR_BOULDER);
		boulderHeight        = movingActor->y.fixed + toFIXED(64);
		movingActor->y.fixed = boulderHeight;
		movingActor->targetY = boulderHeight;
		fdActorSetSpeed(ACTOR_BOULDER, 0.8);
		sySoundPlay(SE_CRASH);
		fdActorMoveToAndWait(ACTOR_BOULDER, 114, 333);
		fdActorSetAnimation(ACTOR_BOULDER, ANIM_WALK);
		fdMapGridSetShake(2, 2, 1);
		sySoundPlay(SE_CRASH);
		*shakeShift = 0;
		fdActorSetSpeed(ACTOR_BOULDER, 0.4);
		fdActorMoveToAndWait(ACTOR_BOULDER, 114, 300);
		fdActorSetAnimation(ACTOR_BOULDER, ANIM_WALK);
		fdMapGridSetShake(5, 5, 1);
		sySoundPlay(SE_CRASH);
		*shakeShift = 2;
		fdActorSetSpeed(ACTOR_BOULDER, 0.8);
		fdActorMoveToAndWait(ACTOR_BOULDER, 114, 333);
		fdActorSetAnimation(ACTOR_BOULDER, ANIM_WALK);
		fdMapGridSetShake(2, 2, 1);
		sySoundPlay(SE_CRASH);
		*shakeShift = 0;
		fdActorSetSpeed(ACTOR_BOULDER, 0.4);
		fdActorMoveToAndWait(ACTOR_BOULDER, 114, 300);
		fdActorSetAnimation(ACTOR_BOULDER, ANIM_WALK);
		fdMapGridSetShake(4, 4, 1);
		sySoundPlay(SE_CRASH);
		*shakeShift = 2;
		fdActorSetSpeed(ACTOR_BOULDER, 0.8);
		fdActorMoveToAndWait(ACTOR_BOULDER, 114, 333);
		fdActorSetAnimation(ACTOR_BOULDER, ANIM_WALK);
		fdMapGridSetShake(2, 2, 1);
		sySoundPlay(SE_CRASH);
		*shakeShift = 1;
		fdScriptWait(20);
		fdActorSetAttachedEffect(ACTOR_ELDER_EAST, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
		fdActorRunRepeatedMotion(ACTOR_ELDER_EAST, 2);
		// "Ugh... Ughnn... It's too heavy! We can't hold it for long!"
		fdScriptShowMessage(ACTOR_ELDER_WEST_STAND_IN, 0);
		fdActorShowEmote(ACTOR_ELDER_WEST, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
		fdActorRunRepeatedMotion(ACTOR_ELDER_WEST, 2);
		// "If our Psynergy runs out, Vale will be destroyed!"
		fdScriptShowMessageThenWait(ACTOR_ELDER_EAST_STAND_IN, 0, 40);
		fdActorSetAttachedEffect(ACTOR_ELDER_FAR_EAST,
		                         ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
		fdActorRunRepeatedMotion(ACTOR_ELDER_FAR_EAST, 2);
		// "We have to hold on until the villagers are safe!"
		fdScriptShowMessage(ACTOR_ELDER_FAR_EAST, 0);
		// The westernmost elder stops straining, looks about and calls to the boys, and the camera
		// returns to Isaac.
		ShakeDone = 1;
		fdActorSetAnimation(ACTOR_ELDER_FAR_WEST, ANIM_STAND);
		sySynch(1);
		fdActorSetSpriteColorById(ACTOR_ELDER_FAR_WEST, SPRITE_COLOR_NORMAL);
		fdActorShowEmote(ACTOR_ELDER_FAR_WEST, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 20);
		fdActorFaceDirection(ACTOR_ELDER_FAR_WEST, FACING_WEST, 40);
		fdActorFaceDirection(ACTOR_ELDER_FAR_WEST, FACING_EAST, 20);
		fdActorFaceDirection(ACTOR_ELDER_FAR_WEST, FACING_WEST, 20);
		fdActorFaceDirection(ACTOR_ELDER_FAR_WEST, FACING_SOUTH, 40);
		fdActorShowEmote(ACTOR_ELDER_FAR_WEST, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
		fdActorRunRepeatedMotion(ACTOR_ELDER_FAR_WEST, 2);
		fdActorJump(ACTOR_ELDER_FAR_WEST, 4, 40);
		fdActorSetAnimation(ACTOR_ELDER_FAR_WEST, YOUNG_MAN_WITH_BROWN_HAIR_ANIMATION_CALL_OUT);
		fdScriptWait(10);
		// "Hurry, boys! Run!!! It's just a little farther!"
		fdScriptShowMessageThenWait(ACTOR_ELDER_FAR_WEST, 0, 20);
		sySoundPlay(SOUND_FADE_OUT_ACTION_SE);
		fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
		fdCameraSetSpeed(6);
		fdCameraMoveTo(84, CAMERA_KEEP, 564, 1);
		fdCameraWaitForMove();
		fdPlayQueuedMusic();
		fdActorFaceActor(ACTOR_GARET_CHILD, ACTOR_PARTY_LEADER, 0);
		fdScriptWait(20);
		fdActorSetAttachedEffect(ACTOR_GARET_CHILD,
		                         ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
		fdScriptWait(30);
		syCloseEvent(updateEffects);
		syCloseEvent(scValePrologueCycleBoulderPalette);
		// "Come on, Isaac, run! The Boulder is coming!"
		fdScriptShowMessage(ACTOR_GARET_CHILD, 0);
		fdActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_GARET_CHILD, 0);
		fdScriptWait(20);
		scEndScriptPsynergy();
		fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
		fdActorSetAnimationAndWait(ACTOR_GARET_CHILD, ANIM_NOD);
		fdScriptWait(20);
		scWalkIntoLeader(ACTOR_GARET_CHILD);
		fdActorReleaseById(ACTOR_ELDER_WEST_STAND_IN);
		fdActorReleaseById(ACTOR_ELDER_EAST_STAND_IN);
		fdActorReleaseById(ACTOR_ELDER_FAR_EAST);
		fdActorReleaseById(ACTOR_ELDER_FAR_WEST);
		fdActorReleaseById(ACTOR_ELDER_EAST);
		fdActorReleaseById(ACTOR_ELDER_WEST);
		gmFlagSet(FLAG_VALE_BOULDER_FELL);
		fdScriptEnd();
	}
}

// Sets the colour mode of every part of a one-sprite actor to `mode` - 1, or with mode 0 flashes it
// white and plain in turn, two ticks each (ValePrologueFlashColorModes).
static void scValePrologueSetActorColorMode(ACTOR *actor, Sint32 mode)
{
	if ((actor->spriteStorageKind & 0xf) == ACTOR_SPRITE_ONE)
	{
		SPR   *animation          = actor->sprite;
		Sint32 animationParameter = mode - 1;
		Uint8  count;
		if (mode == 0)
		{
			animationParameter =
				ValePrologueFlashColorModes[(TickCount >> 1) & (actor->spriteStorageKind & 0xf)];
		}
		count = animation->count;
		if (count != 0)
		{
			SPR_ANIMATION **entries          = animation->entries;
			Sint32          remainingEntries = count;
			do
			{
				SPR_ANIMATION *entry = *entries++;
				if (entry != NULL && entry->script != NULL)
				{
					entry->colorMode = animationParameter;
				}
				remainingEntries--;
			} while (remainingEntries != 0);
		}
		animation->dirty = 1;
	}
}

// Unused: flashes the four elders white one beat in four, a beat lasting 2 to the power ShakeShift
// ticks, as the Boulder slips.
UNUSED static void scValePrologueFlashBoulderMapAnimationEntries(void)
{
	if (((TickCount >> ShakeShift) & 3) != 0)
	{
		scValePrologueSetActorColorMode(fdActorGet(ACTOR_ELDER_EAST), 1);
		scValePrologueSetActorColorMode(fdActorGet(ACTOR_ELDER_WEST), 1);
		scValePrologueSetActorColorMode(fdActorGet(ACTOR_ELDER_FAR_EAST), 1);
		scValePrologueSetActorColorMode(fdActorGet(ACTOR_ELDER_FAR_WEST), 1);
	}
	else
	{
		scValePrologueSetActorColorMode(fdActorGet(ACTOR_ELDER_EAST), 8);
		scValePrologueSetActorColorMode(fdActorGet(ACTOR_ELDER_WEST), 8);
		scValePrologueSetActorColorMode(fdActorGet(ACTOR_ELDER_FAR_EAST), 8);
		scValePrologueSetActorColorMode(fdActorGet(ACTOR_ELDER_FAR_WEST), 8);
	}
}

// On the stormy night, coming down to the river's west bank north of the house by the river
// (metatiles (8..9, 66..67)), Isaac sees Felix clinging on in the current, and Jenna cries out as
// her family tries to save him.
static void scValePrologueWatchAttemptToSaveFelix(void)
{
	const Sint32 *pulseScript;

	if (fdScriptGetFlag(FLAG_VALE_STORM_SAW_FELIX_HANGING_ON) != 0)
	{
		return;
	}
	fdScriptStart();
	fdActorSetPos(ACTOR_KYLE, 192, 1214);
	fdActorFaceDirection(ACTOR_KYLE, FACING_SOUTHEAST, 0);
	fdActorSetAnimation(ACTOR_KYLE, ANIM_KYLE_REACH_OUT);
	{
		ACTOR *actor;
		Sint32 pulseDelay;
		actor        = fdActorGet(ACTOR_KYLE);
		pulseDelay   = SQUASH_AND_STRETCH_PAUSE();
		pulseScript  = ValePrologueSquashAndStretchScript;
		actor->timer = pulseDelay;
		fdActorSetScript(ACTOR_KYLE, pulseScript);
	}
	fdActorSetPos(ACTOR_DORA, 192, 1189);
	fdActorFaceDirection(ACTOR_DORA, FACING_SOUTHEAST, 0);
	fdActorSetPos(ACTOR_JENNAS_FATHER, 227, 1214);
	fdActorFaceDirection(ACTOR_JENNAS_FATHER, FACING_SOUTH, 0);
	fdActorSetAnimation(ACTOR_JENNAS_FATHER, ANIM_PARENT_REACH_OUT);
	{
		ACTOR *actor;
		Sint32 pulseDelay;
		actor        = fdActorGet(ACTOR_JENNAS_FATHER);
		pulseDelay   = SQUASH_AND_STRETCH_PAUSE();
		actor->timer = pulseDelay;
		fdActorSetScript(ACTOR_JENNAS_FATHER, pulseScript);
	}
	fdActorSetPos(ACTOR_JENNAS_MOTHER, 250, 1214);
	fdActorFaceDirection(ACTOR_JENNAS_MOTHER, FACING_SOUTH, 0);
	fdActorSetAnimation(ACTOR_JENNAS_MOTHER, ANIM_PARENT_REACH_OUT);
	{
		ACTOR *actor;
		Sint32 pulseDelay;
		actor        = fdActorGet(ACTOR_JENNAS_MOTHER);
		pulseDelay   = SQUASH_AND_STRETCH_PAUSE();
		actor->timer = pulseDelay;
		fdActorSetScript(ACTOR_JENNAS_MOTHER, pulseScript);
	}
	fdActorSetPos(ACTOR_JENNA_CHILD, 227, 1189);
	fdActorFaceDirection(ACTOR_JENNA_CHILD, FACING_SOUTH_SOUTHEAST, 0);
	fdActorSetPos(ACTOR_FELIX_CHILD, 243, 1277);
	fdActorFaceDirection(ACTOR_FELIX_CHILD, FACING_NORTH, 0);
	fdActorSetSpriteFlags(fdActorGet(ACTOR_FELIX_CHILD), 0);
	sySynch(3);
	fdScriptSetMessageId(MsgValeNoBrother);
	// "Oh no! My brother!"
	fdScriptShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_JENNA_CHILD, 0);
	fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
	fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 150, 1094);
	{
		ACTOR *leader;
		leader = fdActorGet(ACTOR_PARTY_LEADER);
		if (leader != NULL)
		{
			fdActorSetPosFX(ACTOR_GARET_CHILD, leader->x.fixed, leader->z.fixed);
		}
	}
	fdActorWalkToAndWait(ACTOR_GARET_CHILD, 132, 1094);
	fdActorFaceEachOther(ACTOR_PARTY_LEADER, ACTOR_GARET_CHILD, 0);
	fdScriptWait(40);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_GARET_CHILD, FACING_SOUTH, 20);
	fdCameraSetSpeed(4);
	fdCameraMoveTo(216, CAMERA_KEEP, 1232, 1);
	fdCameraWaitForMove();
	fdScriptWait(40);
	fdActorRunRepeatedMotion(ACTOR_KYLE, 2);
	// "Hang on, Felix!"
	fdScriptShowMessageThenWait(ACTOR_KYLE, 0, 10);
	fdActorRunRepeatedMotion(ACTOR_FELIX_CHILD, 3);
	fdActorFaceDirection(ACTOR_DORA, FACING_EAST, 10);
	fdActorSetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
	// "Felix will be all right. We'll find a way to save him..."
	fdScriptShowMessageThenWait(ACTOR_DORA, 0, 10);
	fdActorFaceDirection(ACTOR_DORA, FACING_SOUTH_SOUTHEAST, 10);
	fdCameraSetSpeed(3);
	fdCameraMoveTo(232, CAMERA_KEEP, 1253, 1);
	fdCameraWaitForMove();
	fdScriptWait(20);
	sySoundPlay(SE_HEAVY_IMPACT);
	fdActorJump(ACTOR_FELIX_CHILD, 4, 0);
	fdActorSetAnimation(ACTOR_FELIX_CHILD, ANIM_FELIX_UNDER_WATER);
	fdScriptWait(10);
	fdActorSetPos(ACTOR_FELIX_CHILD, 0, 0);
	fdScriptWait(60);
	fdPlayQueuedMusic();
	fdActorSetAnimation(ACTOR_KYLE, ANIM_STAND);
	{
		ACTOR *actor;
		actor         = fdActorGet(ACTOR_KYLE);
		actor->scaleX = toFIXED(1);
		actor->scaleY = toFIXED(1);
	}
	fdActorSetAnimation(ACTOR_JENNAS_FATHER, ANIM_STAND);
	{
		ACTOR *actor;
		actor         = fdActorGet(ACTOR_JENNAS_FATHER);
		actor->scaleX = toFIXED(1);
		actor->scaleY = toFIXED(1);
	}
	fdActorSetAnimation(ACTOR_JENNAS_MOTHER, ANIM_STAND);
	{
		ACTOR *actor;
		actor         = fdActorGet(ACTOR_JENNAS_MOTHER);
		actor->scaleX = toFIXED(1);
		actor->scaleY = toFIXED(1);
	}
	fdActorStartRepeatedMotion(ACTOR_KYLE, 2);
	fdActorStartRepeatedMotion(ACTOR_DORA, 2);
	fdActorStartRepeatedMotion(ACTOR_JENNAS_FATHER, 2);
	fdActorStartRepeatedMotion(ACTOR_JENNAS_MOTHER, 2);
	fdActorRunRepeatedMotion(ACTOR_JENNA_CHILD, 2);
	fdCameraSetSpeed(0.6);
	fdCameraMoveTo(216, CAMERA_KEEP, 1232, 1);
	fdCameraWaitForMove();
	fdActorSetAttachedEffect(ACTOR_JENNA_CHILD, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdActorSetAttachedEffect(ACTOR_DORA, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdScriptWait(60);
	fdActorRunRepeatedMotion(ACTOR_JENNA_CHILD, 2);
	fdActorStartRepeatedMotion(ACTOR_JENNA_CHILD, 3);
	// "Nooooo! Felix!"
	fdScriptShowMessage(ACTOR_JENNA_CHILD, 0);
	fdActorJump(ACTOR_JENNAS_MOTHER, 2, 0);
	fdActorSetDestination(ACTOR_JENNAS_MOTHER, 234, 1205);
	fdActorJump(ACTOR_JENNA_CHILD, 2, 0);
	fdActorSetDestination(ACTOR_JENNA_CHILD, 227, 1201);
	fdScriptWait(90);
	fdCameraMoveTo(232, CAMERA_KEEP, 1253, 1);
	fdCameraWaitForMove();
	fdActorSetPos(ACTOR_FELIX_CHILD, 243, 1277);
	sySynch(1);
	sySoundPlay(SE_DRIP);
	{
		ACTOR *actor;
		actor            = fdActorGet(ACTOR_FELIX_CHILD);
		actor->velocityY = toFIXED(2);
	}
	fdScriptWait(6);
	fdActorSetAnimation(ACTOR_FELIX_CHILD, ANIM_FELIX_CLING);
	fdScriptWait(20);
	fdPlayQueuedMusic();
	fdScriptWait(20);
	fdCameraSetSpeed(1.6);
	fdCameraMoveTo(216, CAMERA_KEEP, 1232, 1);
	fdCameraWaitForMove();
	fdActorRunRepeatedMotion(ACTOR_JENNAS_FATHER, 2);
	fdScriptWait(20);
	fdActorShowEmote(ACTOR_JENNAS_FATHER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
	fdActorFaceEachOther(ACTOR_JENNAS_FATHER, ACTOR_KYLE, 0);
	fdScriptWait(10);
	fdActorRunRepeatedMotion(ACTOR_KYLE, 2);
	// "The rope won't reach... We should use Psynergy. Do you have..."
	fdScriptShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_KYLE, 0);
	{
		ACTOR *actor;
		actor                 = fdActorGet(ACTOR_JENNAS_MOTHER);
		actor->steeringFlags &= ~ACTOR_FACES_MOTION;
	}
	{
		ACTOR *actor;
		actor                 = fdActorGet(ACTOR_JENNA_CHILD);
		actor->steeringFlags &= ~ACTOR_FACES_MOTION;
	}
	fdActorSetSpeed(ACTOR_JENNAS_MOTHER, 0.6);
	fdActorSetSpeed(ACTOR_JENNA_CHILD, 0.6);
	fdActorSetDestination(ACTOR_JENNAS_MOTHER, 247, 1210);
	fdActorMoveToAndWait(ACTOR_JENNA_CHILD, 227, 1189);
	fdActorGet(ACTOR_JENNAS_MOTHER)->steeringFlags |= ACTOR_FACES_MOTION;
	fdActorGet(ACTOR_JENNA_CHILD)->steeringFlags   |= ACTOR_FACES_MOTION;
	fdActorFaceDirection(ACTOR_JENNA_CHILD, FACING_SOUTHWEST, 0);
	fdActorFaceDirection(ACTOR_JENNAS_MOTHER, FACING_WEST, 10);
	fdActorSetAnimation(ACTOR_JENNAS_FATHER, ANIM_SHAKE_HEAD);
	// "I've used all mine up. What about you, Kyle?"
	fdScriptShowMessageThenWait(SPEAKER_WINDOW_BELOW | ACTOR_JENNAS_FATHER, 0, 10);
	fdActorFaceDirection(ACTOR_KYLE, FACING_NORTH, 20);
	fdActorFaceDirection(ACTOR_KYLE, FACING_EAST, 10);
	fdActorSetAnimation(ACTOR_KYLE, ANIM_SHAKE_HEAD);
	// "I'm drained from helping the other villagers..."
	syFieldScriptShowMessageThenWait(SPEAKER_WINDOW_BELOW | ACTOR_KYLE, 0, 10);
	fdActorShowEmote(ACTOR_JENNAS_FATHER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
	fdActorShowEmote(ACTOR_KYLE, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
	fdActorShowEmote(ACTOR_DORA, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 20);
	fdActorFaceDirection(ACTOR_DORA, FACING_WEST, 40);
	fdActorFaceDirection(ACTOR_DORA, FACING_NORTH, 20);
	fdActorFaceDirection(ACTOR_DORA, FACING_EAST, 30);
	fdActorFaceDirection(ACTOR_DORA, FACING_SOUTH, 10);
	// "We have to get help."
	fdScriptShowMessageThenWait(ACTOR_DORA, 0, 10);
	fdActorFaceDirection(ACTOR_KYLE, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_JENNAS_MOTHER, FACING_WEST_NORTHWEST, 0);
	fdActorFaceDirection(ACTOR_JENNAS_FATHER, FACING_NORTHWEST, 0);
	fdActorFaceDirection(ACTOR_JENNA_CHILD, FACING_WEST, 10);
	fdActorRunRepeatedMotion(ACTOR_KYLE, 1);
	// "Can Felix hang on long enough for us to find help?"
	syFieldScriptShowMessageThenWait(SPEAKER_WINDOW_BELOW | ACTOR_KYLE, 0, 10);
	fdActorSetAnimation(ACTOR_DORA, ANIM_SHAKE_HEAD);
	// "There's nothing else we can do for him here, is there?"
	fdScriptShowMessageThenWait(ACTOR_DORA, 0, 10);
	fdActorShowEmote(ACTOR_KYLE, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
	fdActorShowEmote(ACTOR_JENNAS_FATHER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
	fdActorShowEmote(ACTOR_JENNAS_MOTHER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
	fdActorShowEmote(ACTOR_JENNA_CHILD, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
	fdActorFaceDirection(ACTOR_DORA, FACING_EAST, 10);
	fdActorFaceEachOther(ACTOR_JENNAS_FATHER, ACTOR_JENNAS_MOTHER, 0);
	fdScriptWait(20);
	fdActorFaceDirection(ACTOR_DORA, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_KYLE, FACING_EAST, 10);
	fdActorFaceDirection(ACTOR_JENNAS_FATHER, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_JENNAS_MOTHER, FACING_WEST, 10);
	fdActorSetAnimation(ACTOR_JENNAS_FATHER, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_JENNAS_MOTHER, ANIM_NOD);
	fdActorFaceEachOther(ACTOR_KYLE, ACTOR_DORA, 0);
	fdScriptWait(20);
	fdActorRunRepeatedMotion(ACTOR_KYLE, 1);
	// "Then go get help, but be as quick as possible!"
	fdScriptShowMessageThenWait(SPEAKER_WINDOW_BELOW | ACTOR_KYLE, 0, 10);
	fdActorSetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
	fdActorFaceDirection(ACTOR_JENNAS_FATHER, FACING_NORTH_NORTHEAST, 10);
	fdActorStartRepeatedMotion(ACTOR_JENNAS_FATHER, 1);
	// "Jenna, can you go too?"
	fdScriptShowMessageThenWait(ACTOR_JENNAS_FATHER, 0, 10);
	fdActorFaceDirection(ACTOR_KYLE, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_DORA, FACING_EAST, 0);
	fdActorRunRepeatedMotion(ACTOR_JENNA_CHILD, 1);
	fdActorFaceDirection(ACTOR_JENNA_CHILD, FACING_SOUTHEAST, 20);
	fdActorFaceDirection(ACTOR_JENNAS_MOTHER, FACING_NORTHWEST, 20);
	fdActorSetAnimationAndWait(ACTOR_JENNAS_MOTHER, ANIM_NOD);
	// "You'll go find help, right?"
	fdScriptShowMessageThenWait(ACTOR_JENNAS_MOTHER, 0, 10);
	fdActorSetAnimationAndWait(ACTOR_JENNA_CHILD, ANIM_NOD);
	fdScriptWait(20);
	fdActorRunRepeatedMotion(ACTOR_DORA, 2);
	fdActorSetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
	// "Your brother's counting on you, Jenna."
	fdScriptShowMessageThenWait(ACTOR_DORA, 0, 10);
	fdActorFaceDirection(ACTOR_JENNA_CHILD, FACING_WEST, 10);
	fdActorSetAnimationAndWait(ACTOR_JENNA_CHILD, ANIM_NOD);
	fdScriptWait(20);
	fdActorRunRepeatedMotion(ACTOR_DORA, 1);
	// "All right, let's get going!"
	fdScriptShowMessageThenWait(ACTOR_DORA, 0, 10);
	scValePrologueDoraGoesForHelp();
	gmFlagSet(FLAG_VALE_STORM_SAW_FELIX_HANGING_ON);
	fdScriptEnd();
}

// Dora and Jenna run through the house by the river for help. Dora finds Isaac and Garet, asks
// whether they saw what happened and whether they will help, and sends them on to Jenna at the
// plaza.
static void scValePrologueDoraGoesForHelp(void)
{
	ACTOR        *villager;
	const Sint32 *script;
	Sint32        northMsg;
	Sint32        helpMsg;

	fdActorFaceDirection(ACTOR_JENNA_CHILD, FACING_SOUTH_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_JENNAS_FATHER, FACING_NORTH_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_JENNAS_MOTHER, FACING_NORTH_NORTHWEST, 0);
	fdActorFaceDirection(ACTOR_DORA, FACING_SOUTH_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_KYLE, FACING_NORTH_NORTHEAST, 20);
	fdActorSetAnimation(ACTOR_JENNA_CHILD, ANIM_NOD);
	fdActorSetAnimation(ACTOR_JENNAS_FATHER, ANIM_NOD);
	fdActorSetAnimation(ACTOR_JENNAS_MOTHER, ANIM_NOD);
	fdActorSetAnimation(ACTOR_DORA, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_JENNAS_MOTHER, ANIM_NOD);
	fdScriptWait(20);
	fdCameraSetSpeed(1);
	fdCameraMoveTo(134, CAMERA_KEEP, 1195, 1);
	fdActorSetSpeed(ACTOR_JENNA_CHILD, 1.6);
	fdActorSetSpeed(ACTOR_DORA, 1.6);
	fdActorSetScript(ACTOR_JENNA_CHILD, ValePrologueJennaRunsIntoRiversideHouse);
	fdActorRunScriptAndWait(ACTOR_DORA, ValePrologueDoraRunsIntoRiversideHouse);
	sySoundPlay(SE_DOOR_OPEN);
	fdMapGridAnimateMetatiles(ValePrologueGrandparentsDoorOpens, 38, 72);
	fdScriptWait(10);
	fdActorWalkToAndWait(ACTOR_DORA, 149, 1175);
	fdActorSetPos(ACTOR_DORA, 0, 0);
	fdActorWalkToAndWait(ACTOR_JENNAS_MOTHER, 250, 1214);
	fdPlayQueuedMusic();
	fdActorFaceDirection(ACTOR_KYLE, FACING_SOUTH_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_JENNAS_FATHER, FACING_SOUTH_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_JENNAS_MOTHER, FACING_SOUTH_SOUTHEAST, 0);
	fdActorSetAnimation(ACTOR_KYLE, ANIM_KYLE_REACH_OUT);
	fdActorSetAnimation(ACTOR_JENNAS_FATHER, ANIM_PARENT_REACH_OUT);
	fdActorSetAnimation(ACTOR_JENNAS_MOTHER, ANIM_PARENT_REACH_OUT);
	villager        = fdActorGet(ACTOR_KYLE);
	villager->timer = SQUASH_AND_STRETCH_PAUSE();
	villager        = fdActorGet(ACTOR_JENNAS_FATHER);
	villager->timer = SQUASH_AND_STRETCH_PAUSE();
	villager        = fdActorGet(ACTOR_JENNAS_MOTHER);
	villager->timer = SQUASH_AND_STRETCH_PAUSE();
	script          = ValePrologueSquashAndStretchScript;
	fdActorSetScript(ACTOR_KYLE, script);
	fdActorSetScript(ACTOR_JENNAS_FATHER, script);
	fdActorSetScript(ACTOR_JENNAS_MOTHER, script);
	fdActorWaitForScriptEndById(ACTOR_JENNA_CHILD);
	fdScriptWait(10);
	sySoundPlay(SE_DOOR_CLOSE);
	fdMapGridAnimateMetatiles(ValePrologueGrandparentsDoorCloses, 38, 72);
	fdScriptWait(30);
	fdPlayQueuedMusic();
	fdCameraMoveTo(112, CAMERA_KEEP, 1225, 1);
	sySoundPlay(SE_DOOR_OPEN);
	fdMapGridAnimateMetatiles(ValePrologueIsaacsDoorOpens, 35, 73);
	fdScriptWait(20);
	fdPlayQueuedMusic();
	fdActorSetScript(ACTOR_DORA, ValePrologueDoraComesOutOfRiversideHouse);
	fdScriptWait(20);
	fdActorSetScript(ACTOR_JENNA_CHILD, ValePrologueJennaComesOutOfRiversideHouse);
	fdScriptWait(40);
	sySoundPlay(SE_DOOR_CLOSE);
	fdMapGridAnimateMetatiles(ValePrologueIsaacsDoorCloses, 35, 73);
	fdActorWaitForScriptEndById(ACTOR_JENNA_CHILD);
	fdPlayQueuedMusic();
	fdScriptWait(40);
	northMsg = MsgValeGoLookNorth;
	fdScriptSetMessageId(northMsg);
	// "I'll go look up north... You head for the plaza, all right?"
	fdScriptShowMessageThenWait(ACTOR_DORA, 0, 20);
	fdActorSetAnimationAndWait(ACTOR_JENNA_CHILD, ANIM_NOD);
	// "Yes, ma'am. We're looking for anyone with any Psynergy left,..."
	fdScriptShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_JENNA_CHILD, 0, 40);
	fdActorSetAnimation(ACTOR_DORA, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_JENNA_CHILD, ANIM_NOD);
	fdScriptWait(30);
	fdActorSetScript(ACTOR_DORA, ValePrologueDoraHeadsNorth);
	fdActorSetScript(ACTOR_JENNA_CHILD, ValePrologueJennaHeadsForPlaza);
	fdScriptWait(40);
	fdCameraSetSpeed(2);
	fdCameraMoveTo(105, CAMERA_KEEP, 1086, 1);
	fdActorWaitForScriptEndById(ACTOR_DORA);
	fdActorFaceDirection(ACTOR_DORA, FACING_EAST, 0);
	fdActorShowEmote(ACTOR_DORA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
	// "Isaac!"
	fdScriptShowMessageThenWait(ACTOR_DORA, 0, 10);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_GARET_CHILD, FACING_WEST, 10);
	fdActorWalkToAndWait(ACTOR_DORA, 105, 1086);
	fdActorRunRepeatedMotion(ACTOR_DORA, 2);
	// "You saw what happened?"
	fdScriptOpenMessage(SPEAKER_WINDOW_BELOW | ACTOR_DORA, 0);
	fdActorFaceDirection(ACTOR_GARET_CHILD, FACING_EAST, 0);
	if (fdScriptAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0)
	{
		fdActorSetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
		fdScriptSetMessageId(northMsg + 4);
	}
	else
	{
		fdActorRunRepeatedMotion(ACTOR_DORA, 2);
		fdScriptSetMessageId(northMsg + 5);
	}
	fdScriptShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_DORA, 0);
	fdActorFaceDirection(ACTOR_GARET_CHILD, FACING_WEST, 40);
	fdActorShowEmote(ACTOR_DORA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
	helpMsg = MsgValeDontSupposeTwo;
	fdScriptSetMessageId(helpMsg);
	// "I don't suppose... Could the two of you help me?"
	fdScriptOpenMessage(SPEAKER_WINDOW_BELOW | ACTOR_DORA, 0);
	if (fdScriptAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0)
	{
		fdActorSetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
		fdScriptSetMessageId(helpMsg + 1);
		// "Jenna has gone to the plaza to get help. Go join her."
		fdScriptShowMessageThenWait(SPEAKER_WINDOW_BELOW | ACTOR_DORA, 0, 30);
		fdActorFaceDirection(ACTOR_GARET_CHILD, FACING_WEST, 20);
		fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
		fdActorSetAnimation(ACTOR_GARET_CHILD, ANIM_NOD);
		fdActorSetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
		fdScriptWait(40);
	}
	else
	{
		fdActorShowEmote(ACTOR_DORA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 90);
		fdActorShowEmote(ACTOR_DORA, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 40);
		fdActorSetAnimation(ACTOR_DORA, ANIM_SHAKE_HEAD);
		fdScriptSetMessageId(helpMsg + 2);
		// "You don't have to if you don't want to... Just get..."
		fdScriptShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_DORA, 0);
	}
	fdActorSetScript(ACTOR_DORA, ValePrologueDoraLeavesForHelp);
	fdScriptWait(90);
	fdActorFaceEachOther(ACTOR_PARTY_LEADER, ACTOR_GARET_CHILD, 0);
	fdScriptWait(40);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_GARET_CHILD, ANIM_NOD);
	fdScriptWait(20);
	scWalkIntoLeader(ACTOR_GARET_CHILD);
}

// Kyle tells Isaac and Garet to find help for Felix.
static void scValePrologueTalkAboutSavingFelix(void)
{
	const Sint32 *pulseScript;

	fdScriptStart();
	fdActorSetAnimation(ACTOR_KYLE, ANIM_STAND);
	fdScriptWait(10);
	fdActorFaceEachOther(ACTOR_KYLE, ACTOR_PARTY_LEADER, 20);
	if (gmFlagGet(FLAG_VALE_STORM_KYLE_GREETED_ISAAC) != 0)
	{
		fdScriptSetMessageId(MsgValeTwoDontEnough);
		// "You two don't have enough Psynergy to save Felix. Go find help."
		fdScriptShowMessageThenWait(ACTOR_KYLE, 0, 10);
	}
	else
	{
		fdScriptSetMessageId(MsgValeOh);
		fdActorStartRepeatedMotion(ACTOR_KYLE, 1);
		// "Oh, Isaac..."
		fdScriptShowMessageThenWait(ACTOR_KYLE, 0, 10);
		fdActorStartRepeatedMotion(ACTOR_KYLE, 2);
		// "You two don't have enough Psynergy to save Felix. Go find help."
		fdScriptShowMessageThenWait(ACTOR_KYLE, 0, 10);
	}
	fdActorFaceDirection(ACTOR_KYLE, FACING_SOUTHEAST, 20);
	fdActorSetAnimation(ACTOR_KYLE, ANIM_KYLE_REACH_OUT);
	fdScriptWait(10);
	{
		ACTOR *villager;
		Sint32 pulseDelay;
		villager        = fdActorGet(ACTOR_KYLE);
		pulseDelay      = SQUASH_AND_STRETCH_PAUSE();
		pulseScript     = ValePrologueSquashAndStretchScript;
		villager->timer = pulseDelay;
		fdActorSetScript(ACTOR_KYLE, pulseScript);
	}
	fdScriptWait(20);
	gmFlagSet(FLAG_VALE_STORM_KYLE_GREETED_ISAAC);
	fdScriptEnd();
}

// On the stormy night, back at the river with Jenna and the strongman, as Isaac walks out onto the
// rope bridge across the river in the south (metatiles (13, 85..86)), the Boulder comes down the
// river and sweeps Felix, his parents and Kyle away.
static void scValePrologueBoulderSweepsThemAway(void)
{
	ACTOR *leader;
	ACTOR *actor;
	SPR   *sprite;

	if (gmFlagGet(FLAG_VALE_STORM_SENT_TO_AID_ELDERS) != 0)
	{
		if (fdScriptGetFlag(FLAG_VALE_STORM_BOULDER_SWEPT_THEM_AWAY) == 0)
		{
			fdScriptStart();
			fdActorSetSpeed(ACTOR_PARTY_LEADER, 1);
			fdActorSetSpeed(ACTOR_GARET_CHILD, 1);
			fdActorSetSpeed(ACTOR_JENNA_CHILD, 1);
			fdActorSetSpeed(ACTOR_EXHAUSTED_STRONGMAN, 1);
			fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 217, 1367);
			actor = fdActorGet(ACTOR_PARTY_LEADER);
			if (actor != NULL)
			{
				fdActorSetPosFX(ACTOR_GARET_CHILD, actor->x.fixed, actor->z.fixed);
			}
			fdActorWalkToAndWait(ACTOR_GARET_CHILD, 235, 1367);
			fdActorFaceDirection(ACTOR_GARET_CHILD, FACING_NORTH_NORTHWEST, 0);
			actor = fdActorGet(ACTOR_PARTY_LEADER);
			if (actor != NULL)
			{
				fdActorSetPosFX(ACTOR_JENNA_CHILD, actor->x.fixed, actor->z.fixed);
			}
			fdActorWalkToAndWait(ACTOR_JENNA_CHILD, 199, 1367);
			fdActorFaceDirection(ACTOR_JENNA_CHILD, FACING_NORTH_NORTHEAST, 0);
			fdActorSetPos(ACTOR_JENNAS_MOTHER, 247, 1210);
			fdActorFaceDirection(ACTOR_JENNAS_MOTHER, FACING_SOUTHWEST, 0);
			actor                   = fdActorGet(ACTOR_EXHAUSTED_STRONGMAN);
			sprite                  = actor->sprite;
			actor->priorityFlags   &= ~ACTOR_PRIORITY_AUTOMATIC;
			sprite->image.priority  = 1;
			leader                  = fdActorGet(ACTOR_PARTY_LEADER);
			sprite                  = leader->sprite;
			leader->priorityFlags  &= ~ACTOR_PRIORITY_AUTOMATIC;
			sprite->image.priority  = 2;
			actor                   = fdActorGet(ACTOR_PARTY_LEADER);
			if (actor != NULL)
			{
				fdActorSetPosFX(ACTOR_EXHAUSTED_STRONGMAN, actor->x.fixed, actor->z.fixed);
			}
			fdActorWalkToAndWait(ACTOR_EXHAUSTED_STRONGMAN, 221, 1385);
			fdActorFaceDirection(ACTOR_EXHAUSTED_STRONGMAN, FACING_NORTH_NORTHWEST, 60);
			fdActorRunRepeatedMotion(ACTOR_JENNA_CHILD, 2);
			fdScriptSetMessageId(MsgValeMomDadBack);
			// "Mom! Dad! We're back!"
			fdScriptShowMessageThenWait(ACTOR_JENNA_CHILD, 0, 40);
			fdActorSetPos(ACTOR_DORA, 101, 1197);
			fdActorFaceDirection(ACTOR_DORA, FACING_SOUTHEAST, 0);
			// "Is that you, Jenna?"
			fdScriptShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_DORA, 0, 10);
			fdActorFaceDirection(ACTOR_JENNA_CHILD, FACING_NORTHWEST, 0);
			fdCameraSetSpeed(1.2);
			fdCameraMoveTo(101, CAMERA_KEEP, 1197, 1);
			fdActorSetSpeed(ACTOR_DORA, 1.4);
			fdActorSetScript(ACTOR_DORA, ValePrologueDoraRunsToRiver);
			fdScriptWait(60);
			fdCameraSetSpeed(0.6);
			fdCameraMoveTo(187, CAMERA_KEEP, 1328, 1);
			fdCameraWaitForMove();
			fdScriptWait(40);
			fdActorRunRepeatedMotion(ACTOR_JENNA_CHILD, 2);
			// "Yes, ma'am..."
			fdScriptShowMessageThenWait(ACTOR_JENNA_CHILD, 0, 20);
			fdActorRunRepeatedMotion(ACTOR_DORA, 2);
			// "I couldn't find anyone who could help... How did you do?"
			fdScriptShowMessageThenWait(SPEAKER_WINDOW_ABOVE | ACTOR_DORA, 0, 20);
			fdCameraSetSpeed(2);
			fdCameraMoveTo(221, CAMERA_KEEP, 1385, 1);
			fdActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_EXHAUSTED_STRONGMAN, 0);
			fdActorFaceActor(ACTOR_GARET_CHILD, ACTOR_EXHAUSTED_STRONGMAN, 0);
			fdActorFaceDirection(ACTOR_JENNA_CHILD, FACING_SOUTH_SOUTHEAST, 80);
			fdCameraMoveTo(182, CAMERA_KEEP, 1360, 1);
			fdActorWalkToAndWait(ACTOR_EXHAUSTED_STRONGMAN, 182, 1384);
			fdActorFaceActor(ACTOR_EXHAUSTED_STRONGMAN, ACTOR_DORA, 0);
			fdScriptWait(30);
			fdActorSetAnimationAndWait(ACTOR_EXHAUSTED_STRONGMAN, ANIM_NOD);
			fdScriptWait(10);
			fdActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_DORA, 0);
			fdActorFaceActor(ACTOR_GARET_CHILD, ACTOR_DORA, 0);
			fdActorFaceActor(ACTOR_JENNA_CHILD, ACTOR_DORA, 0);
			fdActorSetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
			// "You're just in time."
			fdScriptShowMessage(ACTOR_DORA, 0);
			fdActorRunRepeatedMotion(ACTOR_JENNA_CHILD, 2);
			// "Then my brother is still..."
			fdScriptShowMessageThenWait(ACTOR_JENNA_CHILD, 0, 10);
			fdActorFaceDirection(ACTOR_DORA, FACING_NORTHEAST, 40);
			fdActorFaceDirection(ACTOR_DORA, FACING_SOUTH_SOUTHEAST, 20);
			fdActorSetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
			// "Felix is all right. He's hanging in there."
			fdScriptShowMessage(ACTOR_DORA, 0);
			fdActorFaceEachOther(ACTOR_JENNA_CHILD, ACTOR_EXHAUSTED_STRONGMAN, 0);
			fdActorFaceEachOther(ACTOR_GARET_CHILD, ACTOR_PARTY_LEADER, 0);
			fdScriptWait(40);
			fdActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_DORA, 0);
			fdActorFaceActor(ACTOR_GARET_CHILD, ACTOR_DORA, 0);
			fdActorFaceActor(ACTOR_JENNA_CHILD, ACTOR_DORA, 0);
			fdActorFaceActor(ACTOR_EXHAUSTED_STRONGMAN, ACTOR_DORA, 0);
			fdActorRunRepeatedMotion(ACTOR_DORA, 2);
			fdScriptWait(20);
			// "Come down here, so we can save Felix!"
			fdScriptShowMessageThenWait(ACTOR_DORA, 0, 10);
			fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
			fdActorSetAnimation(ACTOR_JENNA_CHILD, ANIM_NOD);
			fdActorSetAnimation(ACTOR_GARET_CHILD, ANIM_NOD);
			fdActorSetAnimationAndWait(ACTOR_EXHAUSTED_STRONGMAN, ANIM_NOD);
			leader->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
			actor                  = fdActorGet(ACTOR_EXHAUSTED_STRONGMAN);
			actor->priorityFlags  |= ACTOR_PRIORITY_AUTOMATIC;
			scValePrologueShowBoulderSweepingDownriver();
			gmFlagSet(FLAG_VALE_STORM_BOULDER_SWEPT_THEM_AWAY);
			fdScriptEnd();
		}
	}
}

// Places an actor and its targets at once, x doubling as its height.
static inline void scValeProloguePlaceActor(ACTOR *actor, FIXED horizontalPos, FIXED groundPos)
{
	actor->x.fixed = horizontalPos;
	actor->y.fixed = horizontalPos;
	actor->targetX = horizontalPos;
	actor->targetY = horizontalPos;
	actor->z.fixed = groundPos;
	actor->targetZ = groundPos;
}

// One crash of the Boulder coming down, made of a shake of shakeAmplitude pixels, SE_CRASH and a
// wait.
static inline void scValeProloguePlayImpactBeat(Sint32 shakeAmplitude, Sint32 ticks)
{
	fdMapGridSetShakeFX(toFIXED(shakeAmplitude), toFIXED(shakeAmplitude), toFIXED(1));
	sySoundPlay(SE_CRASH);
	fdScriptWait(ticks);
}

// The Boulder comes down the river and carries Felix, his parents and Kyle away.
static void scValePrologueShowBoulderSweepingDownriver(void)
{
	SPR          *boulderSprite;
	// The actor being set up and its sprite: the Boulder's shadow, and while the lightning flashes
	// Kyle and each of Jenna's parents in turn.
	SPR          *figureSprite;
	ACTOR        *actor;
	ACTOR        *lookingActor;
	ACTOR        *figure;
	const Sint32 *entryActions;
	const Sint32 *lookAroundActions;
	const Sint32 *departureActions;

	actor         = fdActorGet(ACTOR_BOULDER);
	figure        = fdActorGet(ACTOR_BOULDER_SHADOW);
	boulderSprite = actor->sprite;
	figureSprite  = figure->sprite;
	fdCameraSetSpeed(1);
	fdCameraMoveTo(110, CAMERA_KEEP, 1419, 1);
	fdActorSetSpeed(ACTOR_EXHAUSTED_STRONGMAN, 1.2);
	fdActorSetSpeed(ACTOR_JENNA_CHILD, 1.2);
	fdActorSetSpeed(ACTOR_PARTY_LEADER, 1.2);
	fdActorSetSpeed(ACTOR_GARET_CHILD, 1.2);
	entryActions = ValePrologueToRiverbankPath;
	fdActorSetScript(ACTOR_EXHAUSTED_STRONGMAN, entryActions);
	fdScriptWait(10);
	fdActorSetScript(ACTOR_JENNA_CHILD, entryActions);
	fdWeatherLastLightning();
	fdScriptWait(10);
	fdActorSetScript(ACTOR_PARTY_LEADER, entryActions);
	fdScriptWait(10);
	fdWeatherStopLightning();
	fdActorSetScript(ACTOR_GARET_CHILD, entryActions);
	fdScriptWait(128);
	scValePrologueStepOffBridge();
	fdCameraMoveTo(174, CAMERA_KEEP, 1428, 1);
	fdScriptWait(104);
	fdCameraMoveTo(153, CAMERA_KEEP, 1325, 1);
	fdActorWalkToAndWait(ACTOR_DORA, 158, 1272);
	fdActorFaceDirection(ACTOR_DORA, FACING_SOUTHEAST, 0);
	fdActorWaitForScriptEndById(ACTOR_EXHAUSTED_STRONGMAN);
	fdActorSetScript(ACTOR_EXHAUSTED_STRONGMAN, ValePrologueExhaustedStrongmanGatherPath);
	fdActorSetScript(ACTOR_JENNA_CHILD, ValePrologueJennaGatherPath);
	fdActorSetScript(ACTOR_PARTY_LEADER, ValePrologueLeaderGatherPath);
	fdActorRunScriptAndWait(ACTOR_GARET_CHILD, ValePrologueGaretGatherPath);
	scValeProloguePlayImpactBeat(4, 20);
	fdMapGridSetShake(1, 1, 1);
	fdScriptWait(60);
	fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdActorShowEmote(ACTOR_JENNA_CHILD, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdActorShowEmote(ACTOR_GARET_CHILD, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdActorShowEmote(ACTOR_EXHAUSTED_STRONGMAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdActorShowEmote(ACTOR_DORA, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
	fdActorFaceEachOther(ACTOR_JENNA_CHILD, ACTOR_EXHAUSTED_STRONGMAN, 0);
	fdActorFaceEachOther(ACTOR_GARET_CHILD, ACTOR_PARTY_LEADER, 0);
	fdScriptWait(20);
	lookingActor        = fdActorGet(ACTOR_PARTY_LEADER);
	lookingActor->timer = VALE_PROLOGUE_GLANCE_PAUSE();
	lookingActor        = fdActorGet(ACTOR_GARET_CHILD);
	lookingActor->timer = VALE_PROLOGUE_GLANCE_PAUSE();
	lookingActor        = fdActorGet(ACTOR_JENNA_CHILD);
	lookingActor->timer = VALE_PROLOGUE_GLANCE_PAUSE();
	lookingActor        = fdActorGet(ACTOR_EXHAUSTED_STRONGMAN);
	lookingActor->timer = VALE_PROLOGUE_GLANCE_PAUSE();
	lookingActor        = fdActorGet(ACTOR_DORA);
	lookingActor->timer = VALE_PROLOGUE_GLANCE_PAUSE();
	lookAroundActions   = ValePrologueLookAroundScript;
	fdActorSetScript(ACTOR_DORA, lookAroundActions);
	fdScriptWait(30);
	fdActorSetScript(ACTOR_PARTY_LEADER, lookAroundActions);
	fdActorSetScript(ACTOR_JENNA_CHILD, lookAroundActions);
	fdActorSetScript(ACTOR_GARET_CHILD, lookAroundActions);
	fdActorSetScript(ACTOR_EXHAUSTED_STRONGMAN, lookAroundActions);
	fdScriptWait(10);
	sySoundPlay(SOUND_FADE_OUT_BGM);
	scValeProloguePlayImpactBeat(3, 30);
	fdMapGridSetShake(1, 1, 1);
	fdScriptWait(120);
	scValeProloguePlayImpactBeat(3, 40);
	fdMapGridSetShake(2, 2, 1);
	fdScriptWait(60);
	scValeProloguePlayImpactBeat(4, 20);
	fdMapGridSetShake(1, 1, 1);
	fdScriptWait(60);
	scValeProloguePlayImpactBeat(3, 40);
	fdMapGridSetShake(1, 1, 1);
	fdScriptWait(60);
	fdWeatherStopLightning();
	fdMapGridSetShake(1, 1, 1);
	fdScriptWait(1);
	fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
	fdCameraSetSpeedAndAcceleration(8, 8);
	fdCameraMoveTo(217, CAMERA_KEEP, 1084, 1);
	fdPaletteFadeApplyTarget(RGB(0, 0, 0), PALETTES_ALL);
	fdPaletteFadeStartInterpolation(40);
	sySynch(40);
	fdActorSetSpriteColorById(ACTOR_BOULDER, SPRITE_COLOR_NORMAL);
	fdActorSetSpriteFlags(fdActorGet(ACTOR_BOULDER), 0);
	fdActorSetSpriteFlags(fdActorGet(ACTOR_BOULDER_SHADOW), 0);
	figure->scaleX                = toFIXED(4) / 5;
	figure->scaleY                = toFIXED(4) / 5;
	figure->priorityFlags        &= ~ACTOR_PRIORITY_AUTOMATIC;
	figureSprite->image.priority  = 1;
	scValeProloguePlaceActor(actor, toFIXED(200), toFIXED(898));
	actor->motionFlags             = 0;
	actor->priorityFlags          &= ~ACTOR_PRIORITY_AUTOMATIC;
	boulderSprite->image.priority  = 0;
	fdCameraGetActor()->targetX    = ACTOR_NO_TARGET;
	fdCameraGetActor()->targetY    = ACTOR_NO_TARGET;
	fdCameraGetActor()->targetZ    = ACTOR_NO_TARGET;
	fdCameraGetActor()->velocityX  = 0;
	fdCameraGetActor()->velocityY  = 0;
	fdCameraGetActor()->velocityZ  = 0;
	sySynch(1);
	fdCameraMoveTo(247, 128, 917, 0);
	fdMapGridCenterOnFollowedPoint();
	sySynch(1);
	fdPaletteFadeApplyTarget(PALETTE_TINT_DIM, PALETTES_BG);
	fdPaletteFadeApplyTarget(PALETTE_TINT_NONE, PALETTES_SPRITES);
	fdPaletteFadeStartInterpolation(30);
	sySynch(30);
	sySetTickEvent(scValePrologueUpdateBoulderShadow, EVENT_PASS_TICK);
	fdActorSetScript(ACTOR_BOULDER, ValePrologueBoulderRollsDownriver);
	fdCameraSetSpeedAndAcceleration(2, 0.03);
	fdCameraMoveTo(175, 96, 1086, 1);
	do
	{
		sySynch(1);
	} while (actor->userValue != 8);
	fdPaletteFadeApplyTarget(RGB(0, 0, 0), PALETTES_ALL);
	fdPaletteFadeStartInterpolation(60);
	sySynch(60);
	fdMapGridWaitForShakeToSettle();
	fdCameraGetActor()->targetX   = ACTOR_NO_TARGET;
	fdCameraGetActor()->targetY   = ACTOR_NO_TARGET;
	fdCameraGetActor()->targetZ   = ACTOR_NO_TARGET;
	fdCameraGetActor()->velocityX = 0;
	fdCameraGetActor()->velocityY = 0;
	fdCameraGetActor()->velocityZ = 0;
	syCloseEvent(scValePrologueUpdateBoulderShadow);
	fdActorStop(ACTOR_BOULDER);
	sySynch(1);
	fdActorSetAnimation(ACTOR_BOULDER, ANIM_STILL);
	figure->scaleX        = toFIXED(5) / 4;
	figure->scaleY        = toFIXED(5) / 4;
	figureSprite->offsetY = 2;
	figureSprite->scale   = toFIXED(5) / 4;
	actor->scaleX         = toFIXED(2);
	actor->scaleY         = toFIXED(2);
	actor->x.fixed        = 0;
	actor->z.fixed        = 0;
	actor->targetX        = 0;
	actor->targetZ        = 0;
	sySynch(1);
	fdActorSetAnimation(ACTOR_FELIX_CHILD, ANIM_FELIX_CLING_STILL);
	fdActorSetPos(ACTOR_DORA, 169, 1264);
	fdActorFaceDirection(ACTOR_DORA, FACING_NORTH, 0);
	fdActorSetAnimation(ACTOR_DORA, DORA_ANIMATION_RUN);
	fdActorSetPos(ACTOR_JENNA_CHILD, 151, 1292);
	fdActorFaceDirection(ACTOR_JENNA_CHILD, FACING_WEST, 0);
	fdActorSetAnimation(ACTOR_JENNA_CHILD, ANIM_JENNA_RUN);
	fdActorSetPos(ACTOR_EXHAUSTED_STRONGMAN, 170, 1313);
	fdActorFaceDirection(ACTOR_EXHAUSTED_STRONGMAN, FACING_SOUTHWEST, 0);
	fdActorSetAnimation(ACTOR_EXHAUSTED_STRONGMAN, STRONGMAN_ANIMATION_RUN);
	fdActorSetPos(ACTOR_PARTY_LEADER, 185, 1333);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHEAST, 0);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ISAAC_CHILD_ANIMATION_RECOIL);
	fdActorSetPos(ACTOR_GARET_CHILD, 169, 1384);
	fdActorFaceDirection(ACTOR_GARET_CHILD, FACING_SOUTH, 0);
	fdActorSetAnimation(ACTOR_GARET_CHILD, ANIM_STILL);
	fdCameraMoveTo(166, 0, 1337, 0);
	fdMapGridCenterOnFollowedPoint();
	actor->motionFlags = 0;
	actor->targetX     = ACTOR_NO_TARGET;
	actor->targetY     = ACTOR_NO_TARGET;
	actor->targetZ     = ACTOR_NO_TARGET;
	scValePrologueFlashLightning();
	fdActorSetPos(ACTOR_BOULDER_SHADOW, 218, 1176);
	fdCameraMoveTo(210, 0, 1196, 0);
	fdMapGridCenterOnFollowedPoint();
	figure->scaleX = toFIXED(2);
	figure->scaleY = toFIXED(2);
	sySetTickEvent(scValePrologueFlickerBoulderShadow, EVENT_PASS_TICK);
	fdActorStop(ACTOR_KYLE);
	fdActorStop(ACTOR_JENNAS_FATHER);
	fdActorStop(ACTOR_JENNAS_MOTHER);
	sySynch(1);
	// Kyle and Jenna's parents freeze in the flash, drawn in front of all else.
	figure                        = fdActorGet(ACTOR_KYLE);
	figureSprite                  = figure->sprite;
	figure->priorityFlags        &= ~ACTOR_PRIORITY_AUTOMATIC;
	figure->scaleX                = toFIXED(1);
	figure->scaleY                = toFIXED(1);
	figure->facing                = FACING_NORTH_NORTHEAST;
	figureSprite->image.priority  = 0;
	fdActorSetAnimation(ACTOR_KYLE, ANIM_STILL);
	figure                        = fdActorGet(ACTOR_JENNAS_FATHER);
	figureSprite                  = figure->sprite;
	figure->priorityFlags        &= ~ACTOR_PRIORITY_AUTOMATIC;
	figure->scaleX                = toFIXED(1);
	figure->scaleY                = toFIXED(1);
	figure->facing                = FACING_NORTH_NORTHWEST;
	figureSprite->image.priority  = 0;
	fdActorSetAnimation(ACTOR_JENNAS_FATHER, ANIM_PARENT_RECOIL);
	figure                        = fdActorGet(ACTOR_JENNAS_MOTHER);
	figureSprite                  = figure->sprite;
	figure->priorityFlags        &= ~ACTOR_PRIORITY_AUTOMATIC;
	figure->scaleX                = toFIXED(1);
	figure->scaleY                = toFIXED(1);
	figure->facing                = FACING_NORTH_NORTHWEST;
	figureSprite->image.priority  = 0;
	fdActorSetAnimation(ACTOR_JENNAS_MOTHER, ANIM_PARENT_RECOIL);
	figure       = fdActorGet(ACTOR_BOULDER_SHADOW);
	figureSprite = figure->sprite;
	scValePrologueFlashLightning();
	actor->y.fixed               = toFIXED(48);
	actor->x.fixed               = toFIXED(214);
	actor->z.fixed               = toFIXED(1216);
	actor->targetX               = ACTOR_NO_TARGET;
	actor->targetY               = ACTOR_NO_TARGET;
	actor->targetZ               = ACTOR_NO_TARGET;
	figureSprite->image.priority = 1;
	fdActorSetPos(ACTOR_BOULDER_SHADOW, 214, 1216);
	fdActorFaceDirection(ACTOR_JENNAS_FATHER, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_JENNAS_MOTHER, FACING_NORTH, 20);
	fdScriptSetFlag(FLAG_WEATHER_HELD);
	fdTilesetAnimationPause(0);
	fdTilesetAnimationPause(1);
	fdTilesetAnimationPause(2);
	fdTilesetAnimationPause(3);
	fdTilesetAnimationPause(4);
	fdTilesetAnimationPause(5);
	fdPaletteFadeApplyTarget(PALETTE_TINT_DIM, PALETTES_BG);
	fdPaletteFadeApplyTarget(PALETTE_TINT_NONE, PALETTES_SPRITES);
	fdPaletteFadeStartInterpolation(120);
	sySynch(160);
	fdPaletteFadeApplyTarget(RGB_WHITE, PALETTES_BG);
	fdPaletteFadeApplyTarget(RGB_WHITE, PALETTES_SPRITES);
	fdPaletteFadeStartInterpolation(80);
	fdScriptWait(80);
	fdScriptWait(100);
	syCloseEvent(scValePrologueFlickerBoulderShadow);
	figureSprite->scale = figure->scaleX;
	gmFlagClear(FLAG_WEATHER_HELD);
	fdTilesetAnimationResume(0);
	fdTilesetAnimationResume(1);
	fdTilesetAnimationResume(2);
	fdTilesetAnimationResume(3);
	fdTilesetAnimationResume(4);
	fdTilesetAnimationResume(5);
	scValePrologueRebuildRiverbank();
	fdActorSetPos(ACTOR_DORA, 165, 1229);
	fdActorSetAnimation(ACTOR_DORA, ANIM_STAND);
	actor            = fdActorGet(ACTOR_DORA);
	actor->facing    = FACING_NORTHEAST;
	actor->timer     = SQUASH_AND_STRETCH_PAUSE();
	departureActions = ValePrologueSquashAndStretchScript;
	actor->userValue = 1;
	fdActorSetScript(ACTOR_DORA, departureActions);
	fdActorSetPos(ACTOR_JENNA_CHILD, 165, 1254);
	fdActorSetAnimation(ACTOR_JENNA_CHILD, ANIM_STAND);
	actor            = fdActorGet(ACTOR_JENNA_CHILD);
	actor->facing    = FACING_NORTHEAST;
	actor->timer     = SQUASH_AND_STRETCH_PAUSE();
	actor->userValue = 2;
	fdActorSetScript(ACTOR_JENNA_CHILD, departureActions);
	fdActorSetPos(ACTOR_GARET_CHILD, 152, 1285);
	fdActorSetAnimation(ACTOR_GARET_CHILD, ANIM_STAND);
	actor            = fdActorGet(ACTOR_GARET_CHILD);
	actor->facing    = FACING_NORTHEAST;
	actor->timer     = SQUASH_AND_STRETCH_PAUSE();
	actor->userValue = 3;
	fdActorSetScript(ACTOR_GARET_CHILD, departureActions);
	fdActorSetPos(ACTOR_EXHAUSTED_STRONGMAN, 180, 1311);
	actor            = fdActorGet(ACTOR_EXHAUSTED_STRONGMAN);
	actor->facing    = FACING_NORTHEAST;
	actor->timer     = SQUASH_AND_STRETCH_PAUSE();
	actor->userValue = 4;
	fdActorSetScript(ACTOR_EXHAUSTED_STRONGMAN, departureActions);
	fdActorSetAnimation(ACTOR_EXHAUSTED_STRONGMAN, STRONGMAN_ANIMATION_SPRAWLED);
	fdActorGet(ACTOR_GARET_CHILD)->priorityFlags         &= ~ACTOR_PRIORITY_AUTOMATIC;
	fdActorGet(ACTOR_EXHAUSTED_STRONGMAN)->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
	sySetTickEvent(scValePrologueCopyLeaderSpritePriority, EVENT_PASS_TICK);
	fdActorSetPos(ACTOR_PARTY_LEADER, 181, 1273);
	fdActorGet(ACTOR_PARTY_LEADER)->facing = FACING_NORTHEAST;
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
	fdCameraMoveTo(181, 0, 1273, 0);
	fdMapGridCenterOnFollowedPoint();
	fdActorSetPos(ACTOR_KYLE, 0, 0);
	fdActorSetPos(ACTOR_BOULDER, 0, 0);
	fdActorSetPos(ACTOR_JENNAS_FATHER, 0, 0);
	fdActorSetPos(ACTOR_JENNAS_MOTHER, 0, 0);
	fdActorSetPos(ACTOR_FELIX_CHILD, 0, 0);
	fdActorSetPos(ACTOR_BOULDER_SHADOW, 0, 0);
	fdActorSetPos(ACTOR_FIRST_ROCK + 6, 144, 1070);
	fdActorSetPos(ACTOR_FIRST_ROCK + 7, 276, 1270);
	sySynch(60);
	fdPaletteFadeApplyTarget(PALETTE_TINT_DIM, PALETTES_BG);
	fdPaletteFadeApplyTarget(PALETTE_TINT_NONE, PALETTES_SPRITES);
	fdPaletteFadeStartInterpolation(80);
	fdScriptWait(60);
	fdPlayQueuedMusic();
	fdScriptWait(60);
	fdPartyRemoveMemberRestored(GARET);
	fdWeatherResumeLightning();
}

// A lightning flash that freezes the picture.
static void scValePrologueFlashLightning(void)
{
	sySynch(20);
	fdScriptSetFlag(FLAG_WEATHER_HELD);
	fdTilesetAnimationPause(0);
	fdTilesetAnimationPause(1);
	fdTilesetAnimationPause(2);
	fdTilesetAnimationPause(3);
	fdTilesetAnimationPause(4);
	fdTilesetAnimationPause(5);
	fdPaletteFadeApplyTarget(PALETTE_TINT_DIM, PALETTES_BG);
	fdPaletteFadeApplyTarget(PALETTE_TINT_NONE, PALETTES_SPRITES);
	fdPaletteFadeStartInterpolation(1);
	sySynch(120);
	fdPaletteFadeApplyTarget(RGB(0, 0, 0), PALETTES_ALL);
	fdPaletteFadeStartInterpolation(60);
	sySynch(60);
	gmFlagClear(FLAG_WEATHER_HELD);
	fdTilesetAnimationResume(0);
	fdTilesetAnimationResume(1);
	fdTilesetAnimationResume(2);
	fdTilesetAnimationResume(3);
	fdTilesetAnimationResume(4);
	fdTilesetAnimationResume(5);
}

// The event that keeps ACTOR_BOULDER_SHADOW on the Boulder as it comes down the river.
static void scValePrologueUpdateBoulderShadow(void)
{
	ACTOR  *src;
	ACTOR  *shadow;
	SPR    *sprite;
	Sint16 *timer;

	src    = fdActorGet(ACTOR_BOULDER);
	shadow = fdActorGet(ACTOR_BOULDER_SHADOW);
	sprite = shadow->sprite;
	timer  = &src->timer;
	if (*timer != 0)
	{
		if (*timer == 60)
		{
			fdMapGridSetShake(3, 3, 1);
		}
		if (*timer == 40)
		{
			fdMapGridSetShake(4, 4, 1);
		}
		if (*timer == 30)
		{
			fdMapGridSetShake(2, 2, 1);
		}
		if (*timer == 20)
		{
			fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
		}
		(*timer)--;
	}
	shadow->x.fixed = src->x.fixed;
	shadow->z.fixed = src->z.fixed;
	shadow->targetX = shadow->x.fixed;
	sprite->offsetY = 10;
	if (TickCount & 1)
	{
		switch (src->userValue)
		{
			case 1:
			case 5:
				shadow->scaleX += toFIXED(1) / 25;
				shadow->scaleY += toFIXED(1) / 25;
				break;
			case 4:
				shadow->scaleX += toFIXED(1) / 50;
				shadow->scaleY += toFIXED(1) / 50;
				break;
			case 2:
			case 3:
			case 6:
			case 7:
			case 8:
			case 9:
				shadow->scaleX -= toFIXED(3) / 100;
				shadow->scaleY -= toFIXED(3) / 100;
				break;
		}
		sprite->scale = shadow->scaleX;
	}
	else
	{
		sprite->scale = 0;
	}
}

// The event that moves ACTOR_BOULDER_SHADOW between two heights on alternate ticks, during the
// frozen flashes.
static void scValePrologueFlickerBoulderShadow(void)
{
	ACTOR *actor  = fdActorGet(ACTOR_BOULDER_SHADOW);
	Uint32 frame  = TickCount;
	SPR   *sprite = actor->sprite;

	if (frame & 1)
	{
		sprite->offsetY = 2;
	}
	else
	{
		sprite->offsetY = 64;
	}
}

// An exhausted strongman tries to go for help and can't move.
static void scValePrologueStrongmanTriesToGoForHelp(void)
{
	ACTOR *villager;
	ACTOR *leader;

	leader   = fdActorGet(ACTOR_PARTY_LEADER);
	villager = fdActorGet(ACTOR_EXHAUSTED_STRONGMAN);
	fdScriptStart();
	if (gmFlagGet(FLAG_VALE_STORM_STRONGMAN_TRIED_TO_GO) != 0)
	{
		fdActorStop(ACTOR_EXHAUSTED_STRONGMAN);
		fdScriptWait(10);
		fdActorRunRepeatedMotion(ACTOR_EXHAUSTED_STRONGMAN, 2);
		fdScriptWait(40);
		if ((Sint16)leader->facing >= 0)
		{
			fdActorSetAnimation(ACTOR_EXHAUSTED_STRONGMAN, STRONGMAN_ANIMATION_PROPPED_UP);
		}
		else
		{
			fdActorSetAnimation(ACTOR_EXHAUSTED_STRONGMAN,
			                    STRONGMAN_ANIMATION_PROPPED_UP_OTHER_WAY);
		}
		fdActorRunRepeatedMotion(ACTOR_EXHAUSTED_STRONGMAN, 2);
		fdScriptWait(20);
		fdScriptSetMessageId(MsgValeICantMoveGetHelp);
		// "It's no use. I can't move... Isaac, please get help!"
		fdScriptShowMessage(ACTOR_EXHAUSTED_STRONGMAN, 0);
		fdActorSetScript(ACTOR_EXHAUSTED_STRONGMAN, ValePrologueSquashAndStretchScript);
		fdActorSetAnimation(ACTOR_EXHAUSTED_STRONGMAN, STRONGMAN_ANIMATION_SPRAWLED);
	}
	else
	{
		fdActorStop(ACTOR_EXHAUSTED_STRONGMAN);
		villager->scaleX = toFIXED(1);
		villager->scaleY = toFIXED(1);
		fdActorFaceDirection(ACTOR_EXHAUSTED_STRONGMAN, FACING_EAST_SOUTHEAST, 0);
		if ((Sint16)leader->facing >= 0)
		{
			fdActorSetAnimation(ACTOR_EXHAUSTED_STRONGMAN, STRONGMAN_ANIMATION_PROPPED_UP);
		}
		else
		{
			fdActorSetAnimation(ACTOR_EXHAUSTED_STRONGMAN,
			                    STRONGMAN_ANIMATION_PROPPED_UP_OTHER_WAY);
		}
		fdScriptWait(20);
		fdScriptSetMessageId(MsgValeIllGo);
		// "Oh, ohhh... I... I'll go!"
		fdScriptShowMessageThenWait(ACTOR_EXHAUSTED_STRONGMAN, 0, 20);
		fdActorSetAnimation(ACTOR_EXHAUSTED_STRONGMAN, ANIM_STAND);
		fdActorJump(ACTOR_EXHAUSTED_STRONGMAN, 4, 0);
		fdScriptWait(80);
		fdActorRunRepeatedMotion(ACTOR_EXHAUSTED_STRONGMAN, 2);
		fdScriptWait(40);
		if ((Sint16)leader->facing >= 0)
		{
			fdActorSetAnimation(ACTOR_EXHAUSTED_STRONGMAN, STRONGMAN_ANIMATION_PROPPED_UP);
		}
		else
		{
			fdActorSetAnimation(ACTOR_EXHAUSTED_STRONGMAN,
			                    STRONGMAN_ANIMATION_PROPPED_UP_OTHER_WAY);
		}
		fdScriptWait(2);
		fdActorJump(ACTOR_EXHAUSTED_STRONGMAN, 2, 0);
		fdScriptWait(60);
		fdActorRunRepeatedMotion(ACTOR_EXHAUSTED_STRONGMAN, 2);
		fdScriptWait(20);
		// "It's no use. I can't move... Isaac, please get help!"
		fdScriptShowMessage(ACTOR_EXHAUSTED_STRONGMAN, 0);
		fdActorSetScript(ACTOR_EXHAUSTED_STRONGMAN, ValePrologueSquashAndStretchScript);
		fdActorSetAnimation(ACTOR_EXHAUSTED_STRONGMAN, STRONGMAN_ANIMATION_SPRAWLED);
		gmFlagSet(FLAG_VALE_STORM_STRONGMAN_TRIED_TO_GO);
	}
	fdScriptEnd();
}

// As Isaac leaves the river to get help, Garet will not let him go alone and goes after him. "I
// can't let him go alone..."
static void scValePrologueGaretFollowsIsaac(void)
{
	fdScriptStart();
	fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
	fdActorStop(ACTOR_GARET_CHILD);
	syCloseEvent(scValePrologueCopyLeaderSpritePriority);
	fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 480, 1392);
	fdActorSetPos(ACTOR_PARTY_LEADER, 0, 0);
	fdActorFaceDirection(ACTOR_GARET_CHILD, FACING_SOUTH_SOUTHEAST, 20);
	fdActorGet(ACTOR_GARET_CHILD)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
	fdActorSetPos(ACTOR_GARET_CHILD, 249, 1240);
	sySynch(1);
	fdScriptSetMessageId(MsgValeIsaac);
	// "Isaac..."
	fdScriptShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_GARET_CHILD, 0);
	fdActorSetPos(ACTOR_GARET_CHILD, 172, 1278);
	sySynch(1);
	fdCameraSetSpeed(4);
	fdCameraMoveTo(162, 0, 1285, 1);
	fdCameraWaitForMove();
	fdScriptWait(40);
	fdActorSetAnimationAndWait(ACTOR_GARET_CHILD, ANIM_SHAKE_HEAD);
	// "I can't let him go alone..."
	syFieldScriptShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_GARET_CHILD, 0, 10);
	fdActorFaceDirection(ACTOR_GARET_CHILD, FACING_NORTH, 20);
	fdActorRunRepeatedMotion(ACTOR_GARET_CHILD, 2);
	// "It's times like these that we men have to stand together!"
	fdScriptShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_GARET_CHILD, 0, 10);
	fdActorFaceDirection(ACTOR_GARET_CHILD, FACING_EAST_SOUTHEAST, 20);
	fdActorSetAnimationAndWait(ACTOR_GARET_CHILD, ANIM_NOD);
	fdActorSetSpeed(ACTOR_GARET_CHILD, 2);
	fdActorWalkToAndWait(ACTOR_GARET_CHILD, 165, 1300);
	fdActorWalkToAndWait(ACTOR_GARET_CHILD, 195, 1432);
	gmFlagSet(FLAG_VALE_STORM_GARET_LEFT);
}

// Redraws the riverbank as the Boulder left it, pictures and metatile behaviors.
static void scValePrologueRebuildRiverbank(void)
{
	fdMapGridCopyMetatilesTo(16, 96, 11, 73, 6, 3);
	fdMapGridCopyMetatilesTo(16, 96, 34, 68, 14, 10);
	fdMapGridCopyMetatilesTo(16, 96, 64, 68, 7, 7);
	fdMapGridCopyMetatilesTo(9, 95, 11, 73, 6, 3);
	fdMapGridCopyMetatilesTo(40, 94, 34, 68, 14, 10);
	fdMapGridCopyMetatilesTo(54, 94, 64, 68, 8, 7);
	fdMapGridCopyMetatilesTo(72, 75, 72, 76, 1, 1);
	fdMapGridCopyMetatilesTo(72, 75, 74, 76, 1, 1);
	fdMapGridCopyMetatileAttributes(7, 75, 1, 1, 6, 75);
	fdMapGridCopyMetatileAttributes(8, 70, 3, 1, 8, 71);
	fdMapGridCopyMetatileAttributes(8, 70, 2, 1, 9, 72);
	fdMapGridCopyMetatileAttributes(8, 70, 2, 1, 9, 73);
	fdMapGridCopyMetatileAttributes(11, 66, 1, 1, 8, 73);
	fdMapGridCopyMetatileAttributes(12, 66, 1, 4, 11, 73);
	fdMapGridCopyMetatileAttributes(25, 0, 1, 1, 6, 74);
	fdMapGridCenterOnFollowedPoint();
}

// On the stormy night, a monster attacks as Isaac comes to the west end of the rope bridge west of
// his house (metatiles (3..4, 54)). It attacks once (slot 2 of ENCOUNTER_ZONE_SCRIPTED_VALE_VAULT).
static void scValePrologueFightThirdMonster(void)
{
	FIELD_STATE *state;

	fdScriptStart();
	state                  = FieldStateWork;
	state->transition      = SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_BARS);
	state->transitionFrame = 64;
	gmFlagSet(FLAG_VALE_STORM_THIRD_MONSTER_FOUGHT);
	fdRequestBattleFromEncounterTable(ENCOUNTER_ZONE_SCRIPTED_VALE_VAULT, 2);
	gmFlagSet(FLAG_VALE_STORM_MONSTER_FOUGHT);
	fdScriptEnd();
}

// On the stormy night, a monster attacks as Isaac steps off the east end of the rope bridge across
// the river in the south of the village (metatiles (21, 85..86)). It attacks once (slot 3 of
// ENCOUNTER_ZONE_SCRIPTED_VALE_VAULT).
static void scValePrologueFightFourthMonster(void)
{
	FIELD_STATE *state;

	fdScriptStart();
	state                  = FieldStateWork;
	state->transition      = SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_BARS);
	state->transitionFrame = 64;
	gmFlagSet(FLAG_VALE_STORM_FOURTH_MONSTER_FOUGHT);
	fdRequestBattleFromEncounterTable(ENCOUNTER_ZONE_SCRIPTED_VALE_VAULT, 3);
	gmFlagSet(FLAG_VALE_STORM_MONSTER_FOUGHT);
	fdScriptEnd();
}

// An elder calling up Psynergy, for ELDERS_FLASH_TICKS once the camera reaches the Boulder: white
// two ticks in four, sending up rings.
static void scValePrologueFlashPaletteAndSpawnEffects(ACTOR *actor)
{
	Sint32 spawnInterval;

	if ((TickCount & 2) != 0)
	{
		fdActorSetColorMode(actor, SPRITE_COLOR_WHITE);
	}
	else
	{
		fdActorSetColorMode(actor, SPRITE_COLOR_NORMAL);
	}
	{
		volatile Uint32 *frame = &TickCount;

		spawnInterval          = ShakeShift * RINGS_INTERVAL_STEP + RINGS_INTERVAL;
		if (syMathRemainderUnsigned(*frame, spawnInterval) == 0)
		{
			scSpawnPsynergyRings(actor);
		}
	}
}

// An elder holding the Boulder. The sprite cycles through the SWAPPED_COLOR_MODES, and rings rise
// and fall as in scValePrologueFlashPaletteAndSpawnEffects.
static void scValePrologueCyclePaletteAndSpawnEffects(ACTOR *actor)
{
	Sint32 paletteFrame;
	Sint32 spawnInterval;

	if ((TickCount & 1) != 0)
	{
		paletteFrame = (TickCount >> 1);
		fdActorSetColorMode(actor, syMathRemainderUnsigned(paletteFrame, SWAPPED_COLOR_MODES));
	}
	spawnInterval = ShakeShift * RINGS_INTERVAL_STEP + RINGS_INTERVAL;
	if (syMathRemainderUnsigned(TickCount, spawnInterval) == 0)
	{
		scSpawnPsynergyRings(actor);
	}
}

// Cycles the sprite through the SWAPPED_COLOR_MODES, a step every second tick.
static void scValePrologueCycleActorPalettes(ACTOR *actor)
{
	Sint32 paletteFrame;

	if ((TickCount & 1) != 0)
	{
		paletteFrame = (TickCount >> 1);
		fdActorSetColorMode(actor, syMathRemainderUnsigned(paletteFrame, SWAPPED_COLOR_MODES));
	}
}

// The sound the rings of light play here (psynergy_rings.c).
#define PSYNERGY_RINGS_SOUND SE_JUMP
#include "shared/psynergy_rings.c"

#include "shared/script_psynergy.c"

// Cycles the four elders' palettes and effects while they hold the Boulder. The westernmost elder's
// stop once that elder lets go (ShakeDone).
static void scValePrologueUpdateBoulderMapEffects(void)
{
	scValePrologueCyclePaletteAndSpawnEffects(fdActorGet(ACTOR_ELDER_EAST));
	scValePrologueCyclePaletteAndSpawnEffects(fdActorGet(ACTOR_ELDER_WEST));
	scValePrologueCyclePaletteAndSpawnEffects(fdActorGet(ACTOR_ELDER_FAR_EAST));
	if (ShakeDone == 0)
	{
		scValePrologueCyclePaletteAndSpawnEffects(fdActorGet(ACTOR_ELDER_FAR_WEST));
	}
}

// The event that cycles the Boulder's palettes while the elders hold it.
static void scValePrologueCycleBoulderPalette(void)
{
	scValePrologueCycleActorPalettes(fdActorGet(ACTOR_BOULDER));
}

// Keeps Garet and the man who came to help (ACTOR_EXHAUSTED_STRONGMAN) at the leader's sprite
// priority while they stand by the river.
static void scValePrologueCopyLeaderSpritePriority(void)
{
	SPR *srcSprite;
#if !VALE_PROLOGUE_SHADOWS_TAKE_LEADER_PRIORITY
	SPR *targetSprite;
	SPR *secondTargetSprite;
#endif

	srcSprite = fdActorGet(ACTOR_PARTY_LEADER)->sprite;
#if VALE_PROLOGUE_SHADOWS_TAKE_LEADER_PRIORITY
	fdActorSetSpritePriority(ACTOR_GARET_CHILD, srcSprite->image.priority);
	fdActorSetSpritePriority(ACTOR_EXHAUSTED_STRONGMAN, srcSprite->image.priority);
#else
	targetSprite                       = fdActorGet(ACTOR_GARET_CHILD)->sprite;
	targetSprite->image.priority       = srcSprite->image.priority;
	secondTargetSprite                 = fdActorGet(ACTOR_EXHAUSTED_STRONGMAN)->sprite;
	secondTargetSprite->image.priority = srcSprite->image.priority;
#endif
}

// Once the Boulder has passed, searching the west door of the house on the river's west bank
// (metatile (6, 74)) finds it will not open. "The force of the impact must have knocked it off its
// hinges."
static void scValePrologueSearchBrokenDoor(void)
{
	fdScriptStart();
	mnFieldMessageShow(MsgValeDoorWontOpen, 1);
	fdScriptEnd();
}

// On the stormy night, searching the chest Garet was trying to save, left on the path north of his
// house once he joins (metatiles (25..27, 38)). "It's Garet's chest of valuables."
static void scValePrologueSearchValuablesChest(void)
{
	fdScriptStart();
	mnFieldMessageShow(MsgValeSChestValuables, 1);
	fdScriptEnd();
}

// The colour modes scValePrologueSetActorColorMode's mode 0 steps through, a step every two ticks.
Uint8 ValePrologueFlashColorModes[] = {
	SPRITE_COLOR_WHITE, SPRITE_COLOR_NORMAL, SPRITE_COLOR_NORMAL, SPRITE_COLOR_NORMAL};

// The actors' scripts, positions in pixels.

// The stout man runs off toward the plaza after shoving Isaac.
Sint32 ValePrologueShovingManScript[] = {
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE(185, 0, 824),
	SC_SYNC,
	SC_MOVE(179, 0, 836),
	SC_SYNC,
	SC_MOVE(226, 0, 840),
	SC_SYNC,
	SC_MOVE(226, 0, 944),
	SC_SYNC,
	SC_POSITION(0, 0, 0),
	SC_ANIMATION(ANIM_STAND),
	SC_END,
};

// Garet runs up to Isaac (scValePrologueUnusedGaretJoinsIsaac).
Sint32 ValePrologueUnusedGaretRunsUpScript[] = {
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE(345, 0, 612),
	SC_SYNC,
	SC_MOVE(311, 0, 638),
	SC_SYNC,
	SC_MOVE(256, 0, 608),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_END,
};

// Garet heaves for breath. His sprite stretches over 24 ticks and settles over 12, for ever.
Sint32 ValePrologueUnusedGaretPantingScript[] = {
	SC_LABEL(1),
	SC_ADD(SW_SCALE_X, toFIXED(3) / 1024),
	SC_ADD(SW_SCALE_Y, toFIXED(3) / 512),
	SC_WAIT(1),
	SC_LOOP(24, SC_TO(1)),
	SC_LABEL(2),
	SC_ADD(SW_SCALE_X, -toFIXED(3) / 512),
	SC_ADD(SW_SCALE_Y, -toFIXED(3) / 256),
	SC_WAIT(1),
	SC_LOOP(12, SC_TO(2)),
	SC_LOOP(SC_FOREVER, SC_TOP),
	SC_END,
};

// The Boulder on the mountainside, in animation 2 until it slips.
Sint32 ValePrologueBoulderWaits[] = {
	SC_ANIMATION(ANIM_WALK),
	SC_END,
};

// Isaac and Garet run to where they see the elders hold the Boulder
// (scValePrologueWatchBoulderFall), and face each other.
Sint32 ValePrologueBoulderFallLeaderPath[] = {
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE(94, 0, 564),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_SET(SW_FACING, FACING_WEST),
	SC_WAIT(30),
	SC_END,
};

Sint32 ValePrologueBoulderFallGaretPath[] = {
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE(76, 0, 564),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_SET(SW_FACING, FACING_EAST),
	SC_WAIT(30),
	SC_END,
};

// scValePrologueDoraGoesForHelp: Jenna and Dora run into the house by the river by its east door
// and come out by its west door; Jenna then heads for the plaza and Dora north.

Sint32 ValePrologueDoraRunsIntoRiversideHouse[] = {
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE(178, 0, 1190),
	SC_SYNC,
	SC_MOVE(149, 0, 1194),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_END,
};

Sint32 ValePrologueJennaRunsIntoRiversideHouse[] = {
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE(198, 0, 1176),
	SC_SYNC,
	SC_MOVE(170, 0, 1195),
	SC_SYNC,
	SC_MOVE(149, 0, 1194),
	SC_SYNC,
	SC_MOVE(149, 0, 1175),
	SC_SYNC,
	SC_POSITION(0, 0, 0),
	SC_ANIMATION(ANIM_STAND),
	SC_END,
};

Sint32 ValePrologueDoraComesOutOfRiversideHouse[] = {
	SC_ANIMATION(ANIM_WALK),
	SC_SET(SW_FACING, FACING_SOUTH),
	SC_WAIT(30),
	SC_POSITION(102, 0, 1189),
	SC_WAIT(1),
	SC_MOVE(102, 0, 1218),
	SC_SYNC,
	SC_MOVE(84, 0, 1228),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_SET(SW_FACING, FACING_EAST),
	SC_END,
};

Sint32 ValePrologueJennaComesOutOfRiversideHouse[] = {
	SC_ANIMATION(ANIM_WALK),
	SC_SET(SW_FACING, FACING_SOUTH),
	SC_WAIT(30),
	SC_POSITION(102, 0, 1189),
	SC_SYNC,
	SC_MOVE(102, 0, 1218),
	SC_SYNC,
	SC_MOVE(120, 0, 1228),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_SET(SW_FACING, FACING_WEST),
	SC_END,
};

Sint32 ValePrologueDoraHeadsNorth[] = {
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE(50, 0, 1214),
	SC_SYNC,
	SC_MOVE(46, 0, 1130),
	SC_SYNC,
	SC_MOVE(67, 0, 1100),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_END,
};

Sint32 ValePrologueJennaHeadsForPlaza[] = {
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE(153, 0, 1251),
	SC_SYNC,
	SC_MOVE(177, 0, 1357),
	SC_SYNC,
	SC_POSITION(0, 0, 0),
	SC_ANIMATION(ANIM_STAND),
	SC_END,
};

// Dora leaves to the west once she has asked the boys to help.
Sint32 ValePrologueDoraLeavesForHelp[] = {
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE(123, 0, 1078),
	SC_SYNC,
	SC_MOVE(148, 0, 1078),
	SC_SYNC,
	SC_MOVE(148, 0, 1064),
	SC_SYNC,
	SC_MOVE(35, 0, 1064),
	SC_SYNC,
	SC_POSITION(0, 0, 0),
	SC_ANIMATION(ANIM_STAND),
	SC_END,
};

// Dora runs down to the river when Jenna comes back (scValePrologueBoulderSweepsThemAway).
Sint32 ValePrologueDoraRunsToRiver[] = {
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE(102, 0, 1236),
	SC_SYNC,
	SC_MOVE(148, 0, 1256),
	SC_SYNC,
	SC_MOVE(177, 0, 1303),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_END,
};

// The way down to the riverbank that each of the party and the strongman takes in turn as the
// Boulder comes, then each one's place there.
Sint32 ValePrologueToRiverbankPath[] = {
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE(137, 0, 1379),
	SC_SYNC,
	SC_MOVE(114, 0, 1416),
	SC_SYNC,
	SC_MOVE(95, 0, 1416),
	SC_SYNC,
	SC_MOVE(83, 0, 1427),
	SC_SYNC,
	SC_MOVE(154, 0, 1430),
	SC_SYNC,
	SC_MOVE(179, 0, 1398),
	SC_SYNC,
	SC_MOVE(179, 0, 1361),
	SC_SYNC,
	SC_END,
};

// Where each one stops on the bank to hear the Boulder coming.

Sint32 ValePrologueExhaustedStrongmanGatherPath[] = {
	SC_MOVE(184, 0, 1309),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_SET(SW_FACING, FACING_NORTH_NORTHWEST),
	SC_END,
};

Sint32 ValePrologueJennaGatherPath[] = {
	SC_MOVE(165, 0, 1306),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_SET(SW_FACING, FACING_NORTHWEST),
	SC_END,
};

Sint32 ValePrologueLeaderGatherPath[] = {
	SC_MOVE(183, 0, 1325),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_SET(SW_FACING, FACING_NORTHWEST),
	SC_END,
};

Sint32 ValePrologueGaretGatherPath[] = {
	SC_MOVE(166, 0, 1325),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_SET(SW_FACING, FACING_NORTH_NORTHWEST),
	SC_END,
};

// Everyone on the bank looks about (scValePrologueLookAround).
Sint32 ValePrologueLookAroundScript[] = {
	SC_CALL(scValePrologueLookAround),
	SC_LOOP(SC_FOREVER, SC_TOP),
};

// A squash and stretch every 60 to 149 ticks (scSquashAndStretch), for the people straining at the
// river and, after the Boulder, those on the bank.
Sint32 ValePrologueSquashAndStretchScript[] = {
	SC_CALL(scSquashAndStretch),
	SC_LOOP(SC_FOREVER, SC_TOP),
};

// The Boulder bounds down the river in nine steps. Each sets its step in SW_USER_VALUE, which
// scValePrologueUpdateBoulderShadow reads and scValePrologueShowBoulderSweepingDownriver waits on,
// and two of its landings start a shake countdown in SW_TIMER.
Sint32 ValePrologueBoulderRollsDownriver[] = {
	SC_ANIMATION(ANIM_WALK),
	SC_SPEED(3),
	SC_ACCELERATION(1),
	SC_SET(SW_TIMER, 0),
	SC_SET(SW_USER_VALUE, 1),
	SC_SET(SW_SCALE_X, toFIXED(2)),
	SC_SET(SW_SCALE_Y, toFIXED(2)),
	SC_MOVE(256, 128, 931),
	SC_SYNC,
	SC_SE(SE_CRASH),
	SC_SET(SW_TIMER, 60),
	SC_SET(SW_USER_VALUE, 2),
	SC_MOVE(228, 149, 957),
	SC_SYNC,
	SC_SET(SW_USER_VALUE, 3),
	SC_MOVE(201, 160, 984),
	SC_SYNC,
	SC_SET(SW_USER_VALUE, 4),
	SC_MOVE(173, 138, 1011),
	SC_SYNC,
	SC_SET(SW_USER_VALUE, 5),
	SC_MOVE(146, 96, 1038),
	SC_SYNC,
	SC_SE(SE_CRASH),
	SC_SPEED(1.5),
	SC_ACCELERATION(0.5),
	SC_SET(SW_TIMER, 60),
	SC_SET(SW_USER_VALUE, 6),
	SC_MOVE(161, 107, 1050),
	SC_SYNC,
	SC_SPEED(2),
	SC_ACCELERATION(0.6),
	SC_SET(SW_USER_VALUE, 7),
	SC_MOVE(177, 96, 1062),
	SC_SYNC,
	SC_SPEED(2.5),
	SC_ACCELERATION(0.8),
	SC_SET(SW_USER_VALUE, 8),
	SC_MOVE(201, 74, 1074),
	SC_SYNC,
	SC_SPEED(3),
	SC_ACCELERATION(1),
	SC_SET(SW_USER_VALUE, 9),
	SC_MOVE(225, -64, 1086),
	SC_SYNC,
	SC_SET(SW_TIMER, 0),
	SC_SET(SW_USER_VALUE, 0),
	SC_END,
};

SCENE_ENTRANCE ValePrologueEntrances[] = {
	{ENTRANCE_DEFAULT, CONDITION_ALWAYS, 167, 0, 1281, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{VALE_PROLOGUE_DEBUG_BY_STONE_BUILDING,
     CONDITION_ALWAYS, 257,
     0, 456,
     FACING_SOUTH, 0,
     -1,
     -1,
     -1,
     -1,
     0},
	{VALE_PROLOGUE_DEBUG_BY_HILLSIDE_STEPS,
     CONDITION_ALWAYS, 113,
     0, 303,
     FACING_SOUTH, 0,
     -1,
     -1,
     -1,
     -1,
     0},
	{VALE_PROLOGUE_FROM_VALE_KRADENS_COTTAGE_NORTH,
     CONDITION_ALWAYS, 27,
     0, 621,
     FACING_EAST, 0,
     -1,
     -1,
     -1,
     -1,
     0},
	{VALE_PROLOGUE_FROM_VALE_KRADENS_COTTAGE_SOUTH,
     CONDITION_ALWAYS, 29,
     0, 792,
     FACING_EAST, 0,
     -1,
     -1,
     -1,
     -1,
     0},
	{VALE_PROLOGUE_FROM_VALE_PLAZA,
     CONDITION_ALWAYS, 458,
     0, 1393,
     FACING_WEST, 0,
     -1,
     -1,
     -1,
     -1,
     0},
	{VALE_PROLOGUE_FROM_COTTAGE_ROOMS_A_EAST,
     CONDITION_ALWAYS, 406,
     0, 743,
     FACING_SOUTH, 0,
     -1,
     -1,
     -1,
     -1,
     0},
	{VALE_PROLOGUE_FROM_COTTAGE_ROOMS_A_SOUTH,
     CONDITION_ALWAYS, 262,
     0, 821,
     FACING_SOUTH, 0,
     -1,
     -1,
     -1,
     -1,
     0},
	{VALE_PROLOGUE_FROM_COTTAGE_ROOMS_B_ISAACS_HOUSE,
     CONDITION_ALWAYS, 340,
     0, 904,
     FACING_SOUTH, 0,
     -1,
     -1,
     -1,
     -1,
     0},
	{VALE_PROLOGUE_FROM_COTTAGE_ROOMS_A_NORTH,
     CONDITION_ALWAYS, 326,
     0, 1142,
     FACING_SOUTH, 0,
     -1,
     -1,
     -1,
     -1,
     0},
	{VALE_PROLOGUE_FROM_COTTAGE_ROOMS_A_WEST,
     CONDITION_ALWAYS, 374,
     0, 1254,
     FACING_SOUTH, 0,
     -1,
     -1,
     -1,
     -1,
     0},
	{VALE_PROLOGUE_DEBUG_BY_WEST_COTTAGE,
     CONDITION_ALWAYS, 102,
     0, 1222,
     FACING_SOUTH, 0,
     -1,
     -1,
     -1,
     -1,
     0},
	{VALE_PROLOGUE_FROM_COTTAGE_ROOMS_B_WEST,
     CONDITION_ALWAYS, 101,
     0, 1216,
     FACING_SOUTH, 0,
     -1,
     -1,
     -1,
     -1,
     0},
	{VALE_PROLOGUE_FROM_COTTAGE_ROOMS_B_EAST,
     CONDITION_ALWAYS, 146,
     0, 1193,
     FACING_SOUTH, 0,
     -1,
     -1,
     -1,
     -1,
     0},
	{VALE_PROLOGUE_FROM_COTTAGE_ROOMS_B_WITH_DORA,
     CONDITION_ALWAYS, 335,
     0, 908,
     FACING_NORTH, 0,
     -1,
     -1,
     -1,
     -1,
     0},
	{VALE_PROLOGUE_FROM_BOULDER_PASSED,
     CONDITION_ALWAYS, 181,
     0, 1273,
     FACING_NORTHEAST, 0,
     -1,
     -1,
     -1,
     -1,
     0},
	{SCENE_TABLE_END},
};

Uint32 ValePrologueExits[] = {
	SCENE_EXITS(ValePrologue),
	SCENE_EXIT(VALE_PROLOGUE_TO_VALE_PLAZA, ValePlaza, VALE_PLAZA_FROM_VALE),
	SCENE_EXIT(VALE_PROLOGUE_TO_VALE_KRADENS_COTTAGE_NORTH,
               ValeKradensCottage,
               VALE_KRADENS_COTTAGE_FROM_VALE_NORTH),
	SCENE_EXIT(VALE_PROLOGUE_TO_VALE_KRADENS_COTTAGE_SOUTH,
               ValeKradensCottage,
               VALE_KRADENS_COTTAGE_FROM_VALE_SOUTH),
	SCENE_EXIT(
		VALE_PROLOGUE_TO_COTTAGE_ROOMS_A_EAST, CottageRoomsA, COTTAGE_ROOMS_A_FROM_VALE_EAST),
	SCENE_EXIT(
		VALE_PROLOGUE_TO_COTTAGE_ROOMS_A_SOUTH, CottageRoomsA, COTTAGE_ROOMS_A_FROM_VALE_SOUTH),
	SCENE_EXIT(
		VALE_PROLOGUE_TO_COTTAGE_ROOMS_B_ISAACS_HOUSE, CottageRoomsB, COTTAGE_ROOMS_B_FROM_VALE),
	SCENE_EXIT(
		VALE_PROLOGUE_TO_COTTAGE_ROOMS_A_NORTH, CottageRoomsA, COTTAGE_ROOMS_A_FROM_VALE_NORTH),
	SCENE_EXIT(
		VALE_PROLOGUE_TO_COTTAGE_ROOMS_A_WEST, CottageRoomsA, COTTAGE_ROOMS_A_FROM_VALE_WEST),
	SCENE_EXIT(VALE_PROLOGUE_TO_COTTAGE_ROOMS_A_WEST_BY_WEST_BANK_DOOR,
               CottageRoomsA,
               COTTAGE_ROOMS_A_FROM_VALE_WEST),
	SCENE_EXIT(VALE_PROLOGUE_TO_COTTAGE_ROOMS_A_WEST_SANCTUM_DOOR,
               CottageRoomsA,
               COTTAGE_ROOMS_A_FROM_VALE_WEST),
	SCENE_EXIT(VALE_PROLOGUE_TO_COTTAGE_ROOMS_B_WEST,
               CottageRoomsB,
               COTTAGE_ROOMS_B_FROM_VALE_PROLOGUE_WEST),
	SCENE_EXIT(VALE_PROLOGUE_TO_COTTAGE_ROOMS_B_EAST,
               CottageRoomsB,
               COTTAGE_ROOMS_B_FROM_VALE_PROLOGUE_EAST),
	SCENE_EXIT(VALE_PROLOGUE_TO_BOULDER_PASSED, ValePrologue, VALE_PROLOGUE_FROM_BOULDER_PASSED),
	SCENE_EXITS_END,
};

// clang-format off
ACTOR_PLACE ValePrologueActorPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_STRONGMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		0, 0, 0, FACING_NORTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_DORA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_KYLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_ROCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_ROCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_ROCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_ROCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_ROCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		437, 0, 762, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_ROCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		322, 0, 1433, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_ROCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		144, 0, 1070, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_ROCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		276, 0, 1270, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_ROCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_STOUT_MAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_CHEST_OF_VALUABLES, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		420, 0, 619, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_GARET_CHILD, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		396, 0, 619, FACING_EAST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_FELIX_CHILD, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_MAN_IN_GREEN_SHIRT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_PINK_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_JENNA_CHILD, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_BLANK_LARGE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
	{ SCENE_TABLE_END },
};

ACTOR_PLACE ValePrologueBoulderElders[] = {
	ACTOR_PLACEMENT(SPRITE_BLANK, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
		124, 0, 375, FACING_NORTH_NORTHWEST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		85, 0, 361, FACING_NORTH_NORTHEAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		135, 0, 363, FACING_NORTH_NORTHWEST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_BLANK, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
		100, 0, 375, FACING_NORTH_NORTHEAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_BEARDED_MAN_IN_LONG_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		124, 0, 375, FACING_NORTH_NORTHWEST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_BEARDED_MAN_IN_LONG_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		100, 0, 375, FACING_NORTH_NORTHEAST, TALK_FACING_KEEP, 0),
	{ SCENE_TABLE_END },
};
// clang-format on

CUE ValePrologueCues[] = {
	{CUE_STEP,
     METATILE_BEHAVIOR_VALE_STEPS_UP_MT_ALEPH, CONDITION_ALWAYS,
     SCRIPT_REF(scValePrologueLeaveNorthwestToValePlaza)},
	{CUE_STEP,
     METATILE_BEHAVIOR_VALE_NORTH_WAY_TO_KRADEN, CONDITION_ALWAYS,
     SCRIPT_REF(scValePrologueLeaveToValeKradensCottageNorth)},
	{CUE_STEP,
     METATILE_BEHAVIOR_VALE_SOUTH_WAY_TO_KRADEN, CONDITION_ALWAYS,
     SCRIPT_REF(scValePrologueLeaveToValeKradensCottageSouth)},
	{CUE_STEP,
     METATILE_BEHAVIOR_VALE_WAY_TO_THE_PLAZA, CONDITION_ALWAYS,
     SCRIPT_REF(scValePrologueLeaveToValePlaza)},
	{STEP_CUE_WALKING_INTO(FACING_NORTH),
     METATILE_BEHAVIOR_VALE_GARETS_DOOR, CONDITION_ALWAYS,
     SCRIPT_REF(scValePrologueLeaveToCottageRoomsAEast)},
	{STEP_CUE_WALKING_INTO(FACING_NORTH),
     METATILE_BEHAVIOR_VALE_NORTHWEST_HOUSE_DOOR, CONDITION_ALWAYS,
     SCRIPT_REF(scValePrologueLeaveToCottageRoomsA)},
	{STEP_CUE_WALKING_INTO(FACING_NORTH),
     METATILE_BEHAVIOR_VALE_ISAACS_DOOR, CONDITION_ALWAYS,
     SCRIPT_REF(scValePrologueLeaveToIsaacsHouse)},
	{STEP_CUE_WALKING_INTO(FACING_NORTH),
     METATILE_BEHAVIOR_VALE_GRANDPARENTS_DOOR, CONDITION_ALWAYS,
     SCRIPT_REF(scValePrologueLeaveToCottageRoomsANorth)},
	{STEP_CUE_WALKING_INTO(FACING_NORTH),
     METATILE_BEHAVIOR_VALE_SOUTH_HOUSE_DOOR, CONDITION_ALWAYS,
     SCRIPT_REF(scValePrologueLeaveToCottageRoomsAWest)},
	{STEP_CUE_WALKING_INTO(FACING_NORTH),
     METATILE_BEHAVIOR_VALE_UNUSED_WRECKED_HOUSE_DOOR, CONDITION_ALWAYS,
     SCRIPT_REF(scValePrologueUnusedLeaveToCottageRoomsAWestByWestBankDoor)},
	{STEP_CUE_WALKING_INTO(FACING_NORTH),
     METATILE_BEHAVIOR_VALE_RIVERSIDE_HOUSE_WEST_DOOR, CONDITION_ALWAYS,
     SCRIPT_REF(scValePrologueLeaveToCottageRoomsBWest)},
	{STEP_CUE_WALKING_INTO(FACING_NORTH),
     METATILE_BEHAVIOR_VALE_RIVERSIDE_HOUSE_EAST_DOOR, CONDITION_ALWAYS,
     SCRIPT_REF(scValePrologueLeaveToCottageRoomsBEast)},
	{CUE_TALK, ACTOR_GARET_CHILD, CONDITION_ALWAYS, SCRIPT_REF(scValePrologueTalkToGaret)},
	{CUE_TALK, ACTOR_KYLE, CONDITION_ALWAYS, SCRIPT_REF(scValePrologueTalkAboutSavingFelix)},
	{CUE_TALK, ACTOR_JENNAS_FATHER, CONDITION_ALWAYS, MSGNO(MsgValeHangOnFelixHelpIsOn)},
	{CUE_TALK, ACTOR_JENNAS_MOTHER, CONDITION_ALWAYS, MSGNO(MsgValeDontLetGoFelix)},
	{CUE_TALK, ACTOR_DORA, CONDITION_ALWAYS, MSGNO(MsgValeNoItCantBeKyleIt)},
	{CUE_TALK, ACTOR_JENNA_CHILD, CONDITION_ALWAYS, MSGNO(MsgValeMomDadFelixPleaseDontLeave)},
	{CUE_TALK,
     ACTOR_EXHAUSTED_STRONGMAN, CONDITION_ALWAYS,
     SCRIPT_REF(scValePrologueStrongmanTriesToGoForHelp)},
	{CUE_TALK, ACTOR_FIRST_ROCK, CONDITION_ALWAYS, MSGNO(MsgValeRockCannotBeMoved)},
	{CUE_TALK, ACTOR_FIRST_ROCK + 1, CONDITION_ALWAYS, MSGNO(MsgValeRockCannotBeMoved)},
	{CUE_TALK, ACTOR_FIRST_ROCK + 2, CONDITION_ALWAYS, MSGNO(MsgValeRockCannotBeMoved)},
	{CUE_TALK, ACTOR_FIRST_ROCK + 3, CONDITION_ALWAYS, MSGNO(MsgValeRockCannotBeMoved)},
	{CUE_TALK, ACTOR_FIRST_ROCK + 4, CONDITION_ALWAYS, MSGNO(MsgValeRockCannotBeMoved)},
	{CUE_TALK, ACTOR_FIRST_ROCK + 5, CONDITION_ALWAYS, MSGNO(MsgValeRockCannotBeMoved)},
	{CUE_TALK, ACTOR_FIRST_ROCK + 6, CONDITION_ALWAYS, MSGNO(MsgValeRockCannotBeMoved)},
	{CUE_TALK, ACTOR_FIRST_ROCK + 7, CONDITION_ALWAYS, MSGNO(MsgValeRockCannotBeMoved)},
	{CUE_STEP,
     METATILE_BEHAVIOR_VALE_ONTO_BRIDGE, CONDITION_ALWAYS,
     SCRIPT_REF(scValePrologueStepOntoBridge)},
	{CUE_STEP,
     METATILE_BEHAVIOR_VALE_OFF_BRIDGE, CONDITION_ALWAYS,
     SCRIPT_REF(scValePrologueStepOffBridge)},
	{CUE_STEP,
     METATILE_BEHAVIOR_VALE_FIRST_ROCK, CONDITION_ALWAYS,
     SCRIPT_REF(scValePrologueFirstRockFalls)},
	{CUE_STEP,
     METATILE_BEHAVIOR_VALE_SECOND_ROCK, CONDITION_ALWAYS,
     SCRIPT_REF(scValePrologueSecondRockFalls)},
	{CUE_STEP,
     METATILE_BEHAVIOR_VALE_THIRD_ROCK, CONDITION_ALWAYS,
     SCRIPT_REF(scValePrologueThirdRockFalls)},
	{CUE_STEP,
     METATILE_BEHAVIOR_VALE_FOURTH_ROCK, CONDITION_ALWAYS,
     SCRIPT_REF(scValePrologueFourthRockFalls)},
	{CUE_STEP,
     METATILE_BEHAVIOR_VALE_BESIDE_GARET, CONDITION_ALWAYS,
     SCRIPT_REF(scValePrologueHearGaretStruggling)},
	{CUE_STEP,
     METATILE_BEHAVIOR_VALE_PAST_GARET, CONDITION_ALWAYS,
     SCRIPT_REF(scValePrologueGaretCallsAfterIsaac)},
	{CUE_STEP,
     METATILE_BEHAVIOR_VALE_UNUSED_GARET_JOINS, CONDITION_ALWAYS,
     SCRIPT_REF(scValePrologueUnusedGaretJoinsIsaac)},
	{CUE_STEP,
     METATILE_BEHAVIOR_VALE_FOOT_OF_SANCTUM_STEPS, CONDITION_ALWAYS,
     SCRIPT_REF(scValePrologueWarnAboutMtAleph)},
	{CUE_STEP,
     METATILE_BEHAVIOR_VALE_BOULDER_IN_VIEW, CONDITION_ALWAYS,
     SCRIPT_REF(scValePrologueWatchBoulderFall)},
	{CUE_STEP,
     METATILE_BEHAVIOR_VALE_FELIX_IN_VIEW, CONDITION_ALWAYS,
     SCRIPT_REF(scValePrologueWatchAttemptToSaveFelix)},
	{CUE_STEP,
     METATILE_BEHAVIOR_VALE_MIDDLE_OF_BRIDGE, CONDITION_ALWAYS,
     SCRIPT_REF(scValePrologueBoulderSweepsThemAway)},
	{CUE_STEP,
     METATILE_BEHAVIOR_VALE_THIRD_MONSTER, FLAG_VALE_STORM_THIRD_MONSTER_FOUGHT,
     SCRIPT_REF(scValePrologueFightThirdMonster)},
	{CUE_STEP,
     METATILE_BEHAVIOR_VALE_FOURTH_MONSTER, FLAG_VALE_STORM_FOURTH_MONSTER_FOUGHT,
     SCRIPT_REF(scValePrologueFightFourthMonster)},
	{EXAMINE_CUE(EXAMINE_SILENT),
     METATILE_BEHAVIOR_VALE_WRECKED_HOUSE_DOOR, CONDITION_ALWAYS,
     SCRIPT_REF(scValePrologueSearchBrokenDoor)},
	{EXAMINE_CUE(EXAMINE_SILENT),
     METATILE_BEHAVIOR_VALE_GARETS_CHEST, CONDITION_ALWAYS,
     SCRIPT_REF(scValePrologueSearchValuablesChest)},
	{CUE_TABLE_END},
};

// The doors' metatile animations: two pictures of a door opening, from the map's metatiles below
// the village, each shown for two ticks. The four kinds of door are the day map's (vale.c), and
// each table takes the name of a door it opens.

// The Sanctum's door (scValePrologueUnusedOpenSanctumDoor).
Uint16 ValePrologueSanctumDoorOpens[] = {
	MAPGRID_ANIMATION_STEP(0, 98, 2, 2, 2),
	MAPGRID_ANIMATION_STEP(2, 98, 2, 2, 2),
	MAPGRID_ANIMATION_STEPS_END,
};

// The door of Isaac's house, and of Garet's and the house by the river.
Uint16 ValePrologueIsaacsDoorOpens[] = {
	MAPGRID_ANIMATION_STEP(0, 96, 2, 2, 2),
	MAPGRID_ANIMATION_STEP(2, 96, 2, 2, 2),
	MAPGRID_ANIMATION_STEPS_END,
};

// The door of the house of Jenna's grandparents, and two more of the kind.
Uint16 ValePrologueGrandparentsDoorOpens[] = {
	MAPGRID_ANIMATION_STEP(4, 98, 2, 2, 2),
	MAPGRID_ANIMATION_STEP(6, 98, 2, 2, 2),
	MAPGRID_ANIMATION_STEPS_END,
};

// The door VALE_PROLOGUE_TO_COTTAGE_ROOMS_A_WEST goes through.
Uint16 ValePrologueWestHouseDoorOpens[] = {
	MAPGRID_ANIMATION_STEP(4, 96, 2, 2, 2),
	MAPGRID_ANIMATION_STEP(6, 96, 2, 2, 2),
	MAPGRID_ANIMATION_STEPS_END,
};

// Shuts a door of ValePrologueIsaacsDoorOpens's kind, with the shut picture of Isaac's door, after
// Dora and Jenna come out of the house by the river.
Uint16 ValePrologueIsaacsDoorCloses[] = {
	MAPGRID_ANIMATION_STEP(0, 96, 2, 2, 2),
	MAPGRID_ANIMATION_STEP(50, 44, 2, 2, 2),
	MAPGRID_ANIMATION_STEPS_END,
};

// Shuts a door of ValePrologueGrandparentsDoorOpens's kind behind Dora and Jenna as they go into
// the house by the river.
Uint16 ValePrologueGrandparentsDoorCloses[] = {
	MAPGRID_ANIMATION_STEP(4, 98, 2, 2, 2),
	MAPGRID_ANIMATION_STEP(44, 108, 2, 2, 2),
	MAPGRID_ANIMATION_STEPS_END,
};
