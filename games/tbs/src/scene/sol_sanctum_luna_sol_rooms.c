/* @file */
/* Sol Sanctum (SceneId_SolSanctumLunaSolRooms), the room of Sol and the room */
/* of Luna at the heart of the sanctum. */
/* */
/* The rooms lie between SceneId_SolSanctumReliefs (sol_sanctum_entrance.c), */
/* the statue room (SceneId_SolSanctumStatue, sol_sanctum_statue.c) and the way
 */
/* up to SceneId_SolSanctumElementalStarRoom. */
/* */
/* The way in from the second relief room is at the south-west (cell */
/* (3, 31)). Its left branch leads to a chest with a Herb (cell (3, 15)); the */
/* middle one runs north into the room of Sol, four winged statues round a */
/* golden sun on a purple and gold checkered floor, entered by its south */
/* doorway (cells (17, 15) to (18, 18)). A short passage east (cells (25, 10) */
/* to (28, 11)) joins it to the room of Luna, the same room bathed in blue, */
/* with a moon on its floor. South of the room of Luna a corridor leads to the
 */
/* arched stairs up to the statue room (cell (40, 19)). */
/* */
/* Kraden's events here follow the story flags in order: */
/* - scSolSanctumLunaSolRoomsKradenFindsHeartOfSanctum, on the first step onto
 */
/*   the symbol of the sun on the floor (FLAG_SOL_SANCTUM_FOUND). From then */
/*   until the way up opens, Retreat is blocked and */
/*   scSolSanctumLunaSolRoomsKradenStopsPartyLeaving turns the party back from
 */
/*   the way out. */
/* - scSolSanctumLunaSolRoomsIntroduceSolAndLunaRooms, the first time the party
 */
/*   crosses into the Luna room (FLAG_SOL_SANCTUM_LUNA_SOL_ROOMS_SEEN). Kraden
 */
/*   finds the picture of Luna and stays by it from then on. */
/* - Each statue pushed onto a seal in the statue room lights a quarter of the
 */
/*   pictures (FLAG_SOL_SANCTUM_NORTHWEST_SEAL_OPEN to _SOUTHEAST_SEAL_OPEN), */
/*   and the first one cuts here to Kraden. Before the trap is disarmed, lights
 */
/*   fall through the Luna room (scSolSanctumLunaSolRoomsSpringTrap, */
/*   FLAG_SOL_SANCTUM_FALLING_LIGHTS_SEEN) and every quarter goes dark again. */
/*   Afterwards the quarter only flashes and Kraden runs off to the statue room
 */
/*   (scSolSanctumLunaSolRoomsFlashLightsWithTrapDisarmed, */
/*   FLAG_SOL_SANCTUM_KRADEN_SAID_FOUND_SOMETHING). Both take the party back to
 */
/*   the statue room by the exit named for the event. */
/* - With all four quarters lit, Kraden sees that the picture of Luna has */
/*   become Sol (scSolSanctumLunaSolRoomsKradenSeesLunaPictureChange, */
/*   FLAG_SOL_SANCTUM_LUNA_PICTURE_CHANGED) and that the floor's symbol has */
/*   become Luna (scSolSanctumLunaSolRoomsDiscussFloorSymbolChangingToLuna, */
/*   FLAG_SOL_SANCTUM_FLOOR_SYMBOL_SEEN). Searching the symbol then opens the */
/*   floor, and Isaac and Kraden rise through it to the Elemental Star Room */
/*   (scSolSanctumLunaSolRoomsRiseToElementalStarRoom, */
/*   FLAG_SOL_SANCTUM_STAR_ROOM_PASSAGE_OPEN). */
/* - After the eruption, the Wise One sends Isaac and Garet here from the */
/*   collapsing Elemental Star Room (SceneId_SolSanctumEruption, */
/*   sol_sanctum_eruption.c), and they set off for the way out */
/*   (scSolSanctumLunaSolRoomsArriveAfterWiseOnesRescue, */
/*   FLAG_SOL_SANCTUM_RESCUE_ARRIVAL_SEEN). From then on the rooms rumble. */
/* */
/* The rooms' colours follow the party. Stepping into the room of Luna fades */
/* everything to blue, stepping back into the room of Sol or out into the */
/* corridors fades it back, and the passage between them is dimmed brown. */
/* Once all four quarters are lit the two rooms swap: the room of Sol turns */
/* blue and the room of Luna plain. Cells in the doorways do this (enum */
/* SOL_SANCTUM_LUNA_SOL_ROOMS_CELL_BEHAVIOR), and one of three scene flags */
/* remembers which colours show, so a return after a battle keeps them. */
/* */
/* On entry scInit copies each lit quarter into its */
/* FLAG_SOL_SANCTUM_*_LIGHT_SHOWN. It redraws the pictures, the floor and the */
/* open way up from the story flags, unless the entrance's own event draws */
/* them. */
/* */
/* The falling lights are four tasks, each with a random timer of its own, that
 */
/* the trap adds and removes. */
/* */
/* A third statue-room event is never reached. In it the quarters lit since the
 */
/* room last showed them flash for Kraden */
/* (scSolSanctumLunaSolRoomsUnusedAnimateSavedStatueLights). */

#include "flags.h"
#include "item_ids.h"
#include "macros.h"
#include "battle/battle.h"
#include "debug/debug.h"
#include "field/event_inline.h"
#include "graphics/graphics.h"
#include "scene/walk_into_leader.h"
#include "text/msg_ids.h"

u16 SolSanctumLunaSolRooms_Light2Timer BSS_DATA;
u16 SolSanctumLunaSolRooms_Light3Timer BSS_DATA ALIGNED(4);
u16 SolSanctumLunaSolRooms_Light1Timer BSS_DATA ALIGNED(4);
/* Set to 2 as a falling light lands: the rooms shake for a frame, then */
/* settle. */
s32 SolSanctumLunaSolRooms_LandingShake BSS_DATA;
u16 SolSanctumLunaSolRooms_Light4Timer BSS_DATA;

extern SCENE_ENTRANCE SolSanctumLunaSolRoomsEntrances[];
extern u32 SolSanctumLunaSolRoomsExits[];
extern ACTOR_PLACE SolSanctumLunaSolRoomsActorPlacements[];
extern TRIGGER SolSanctumLunaSolRoomsTriggers[];

/* The placed actors, numbered in the order of */
/* SolSanctumLunaSolRoomsActorPlacements; Jenna and Garet, its last rows, */
/* keep their characters' ids. */
typedef enum SOL_SANCTUM_LUNA_SOL_ROOMS_ACTOR_ID {
	/* Eight statues, four in each room, with the sprite of */
	/* SceneId_SolSanctumStatue's statues (sol_sanctum_statue.c). */
	ACTOR_FIRST_STATUE = ACTOR_FIRST_PLACED,
	ACTOR_KRADEN = ACTOR_FIRST_STATUE + 8
} SOL_SANCTUM_LUNA_SOL_ROOMS_ACTOR_ID;

/* The rooms of Sol and Luna show their plain colours (PALETTE_TINT_NONE), so a
 */
/* return after a battle keeps them. */
#define FLAG_SOL_SANCTUM_LUNA_SOL_ROOMS_NO_TINT FLAG_SCENE_TEMP_00
/* The rooms of Sol and Luna show SOL_SANCTUM_BLUE_TINT, so a return after a */
/* battle keeps it. */
#define FLAG_SOL_SANCTUM_LUNA_SOL_ROOMS_BLUE_TINTED FLAG_SCENE_TEMP_01
/* The rooms of Sol and Luna show SOL_SANCTUM_WARM_TINT, so a return after a */
/* battle keeps it. */
#define FLAG_SOL_SANCTUM_LUNA_SOL_ROOMS_WARM_TINTED FLAG_SCENE_TEMP_02

/* Area flags: scArrive clears 0x300 to 0x3ff when the */
/* party enters another area, and other areas use the numbers for */
/* their own. */
/* Set once Isaac and Garet's arrival after the Wise One's rescue has played. */
#define FLAG_SOL_SANCTUM_RESCUE_ARRIVAL_SEEN FLAG_AREA_TEMP_09

/* The blue tint the rooms fade to (PALETTE_TINT's TINT form). */
#define SOL_SANCTUM_BLUE_TINT (PALETTE_TINT_CUSTOM | RGB(12, 14, 20))
/* The dim red tint the rooms fade to. */
#define SOL_SANCTUM_WARM_TINT (PALETTE_TINT_CUSTOM | RGB(17, 13, 11))

/* A falling light in the Luna room is a column of five cells kept in a */
/* staging column on row 59, with the ten cells it falls through, unlit, ten */
/* rows above it on row 49. */
/* Draws a light with its top at (column, row). */
#define DRAW_LIGHT(staging, column, row)                                       \
	fdMapGridCopyCellsTo(staging, 59, column, row, 1, 5)
/* Moves a light a row down to (column, row). Its first cell is copied over the
 */
/* row it leaves, then it is drawn again. */
#define MOVE_LIGHT_DOWN(staging, column, row)                                  \
	(fdMapGridCopyCellsTo(staging, 59, column, (row) - 1, 1, 1),               \
	 DRAW_LIGHT(staging, column, row))
/* Puts back the ten unlit cells a light falls through, from (column, row). */
#define CLEAR_LIGHT_PATH(staging, column, row)                                 \
	fdMapGridCopyCellsTo(staging, 49, column, row, 1, 10)

/* The first of the two layout rows holding the Luna room's picture with its */
/* quarters lit, the picture of Sol, which fdMapGridCopyCellsTo draws into the
 */
/* room. */
#define STATUE_LIGHTS_LIT_ROW 28
/* The first of the two layout rows holding the same picture unlit, the */
/* picture of Luna. */
#define STATUE_LIGHTS_DARK_ROW 30

/* The first of the map's two tileset animations that stay paused until the */
/* floor opens the way up to the Elemental Star Room. */
#define WAY_UP_ANIMATION_A 9
/* The second of them. */
#define WAY_UP_ANIMATION_B 10

/* The cell behaviors of the floor cells the events answer. */
/* */
/* Behaviors 1 and 2 are the exits. */
typedef enum SOL_SANCTUM_LUNA_SOL_ROOMS_CELL_BEHAVIOR {
	/* The room of Luna's south doorway, where Kraden calls Isaac over once the
	 */
	/* picture has changed. */
	CELL_BEHAVIOR_SOL_SANCTUM_LUNA_SOL_ROOMS_LUNA_DOORWAY = 8,
	/* The way back out to SceneId_SolSanctumReliefs. */
	CELL_BEHAVIOR_SOL_SANCTUM_LUNA_SOL_ROOMS_WAY_OUT = 9,
	/* The symbol of the sun on the floor, first stepped on. */
	CELL_BEHAVIOR_SOL_SANCTUM_LUNA_SOL_ROOMS_HEART_OF_SANCTUM = 11,
	/* Fades to SOL_SANCTUM_WARM_TINT. */
	CELL_BEHAVIOR_SOL_SANCTUM_LUNA_SOL_ROOMS_WARM_TINT = 12,
	/* Fades to plain colours, or to SOL_SANCTUM_BLUE_TINT once all four */
	/* quarters are lit. */
	CELL_BEHAVIOR_SOL_SANCTUM_LUNA_SOL_ROOMS_TINT = 13,
	/* Fades to SOL_SANCTUM_BLUE_TINT, or to plain colours once all four */
	/* quarters are lit. */
	CELL_BEHAVIOR_SOL_SANCTUM_LUNA_SOL_ROOMS_TINT_INVERSE = 14,
	CELL_BEHAVIOR_SOL_SANCTUM_LUNA_SOL_ROOMS_PLAIN =
		15, /* fades to plain colours */
	CELL_BEHAVIOR_SOL_SANCTUM_LUNA_SOL_ROOMS_BLACK_BACKDROP =
		19, /* turns the backdrop black */
	/* The symbol on the floor, searched to open the way up. */
	CELL_BEHAVIOR_SOL_SANCTUM_LUNA_SOL_ROOMS_FLOOR_SYMBOL = 20,
	CELL_BEHAVIOR_SOL_SANCTUM_LUNA_SOL_ROOMS_CHEST = 100
} SOL_SANCTUM_LUNA_SOL_ROOMS_CELL_BEHAVIOR;

static void scSolSanctumLunaSolRoomsIntroduceSolAndLunaRooms(void);
static void scSolSanctumLunaSolRoomsDiscussFloorSymbolChangingToLuna(void);
static s32 scSolSanctumLunaSolRoomsAreAllStatueLightsLit(void);
static void scSolSanctumLunaSolRoomsShowMessageAndWait(s32 speaker,
													   s32 waitFrames);
static void scSolSanctumLunaSolRoomsArriveAfterWiseOnesRescue(void);

const SCENE_ENTRANCE *scGetEntrances(void) {
	return SolSanctumLunaSolRoomsEntrances;
}

const SCENE_EXIT_REGION *scGetExitRegions(void) { return NULL; }

const u32 *scGetExits(void) { return SolSanctumLunaSolRoomsExits; }

/* Returns the eight statues, four in each room, Kraden, Jenna and Garet, all */
/* always placed. */
const ACTOR_PLACE *scGetActorPlacements(void) {
	return SolSanctumLunaSolRoomsActorPlacements;
}

/* Returns the floor's cell behaviors that tint the rooms, Kraden's talk */
/* and events, the floor symbol and the Herb's chest. */
const TRIGGER *scGetTriggers(void) { return SolSanctumLunaSolRoomsTriggers; }

/* Isaac steps into the room of Sol, from the passage at its east (cells */
/* (24, 10) and (24, 11)) or through its south doorway (cells (17, 16) and */
/* (18, 16)), and the colours fade back from blue to plain. Once all four */
/* quarters are lit it is the other way round: the room of Sol fades to blue, */
/* and the first time Kraden comes to point out that the symbol on the floor */
/* has changed from Sol to Luna. */
static void scSolSanctumLunaSolRoomsTintSolRoom(void) {
	s32 paletteActive;

	if(scSolSanctumLunaSolRoomsAreAllStatueLightsLit() != 0) {
		paletteActive =
			fdEventGetFlag(FLAG_SOL_SANCTUM_LUNA_SOL_ROOMS_BLUE_TINTED);
		if(paletteActive == 0) {
			fdEventStart();
			fdPaletteFadeApplyTarget(SOL_SANCTUM_BLUE_TINT, PALETTES_BG);
			fdPaletteFadeStartInterpolation(20);
			gmFlagSet(FLAG_SOL_SANCTUM_LUNA_SOL_ROOMS_BLUE_TINTED);
			gmFlagClear(FLAG_SOL_SANCTUM_LUNA_SOL_ROOMS_NO_TINT);
			gmFlagClear(FLAG_SOL_SANCTUM_LUNA_SOL_ROOMS_WARM_TINTED);
			if(gmFlagGet(FLAG_SOL_SANCTUM_LUNA_SOL_ROOMS_SEEN) == 0) {
				scSolSanctumLunaSolRoomsIntroduceSolAndLunaRooms();
			}
			if(scSolSanctumLunaSolRoomsAreAllStatueLightsLit() != 0) {
				if(gmFlagGet(FLAG_SOL_SANCTUM_FLOOR_SYMBOL_SEEN) == 0) {
					scSolSanctumLunaSolRoomsDiscussFloorSymbolChangingToLuna();
				}
			}
			fdEventEnd();
		}
	}
	else {
		if(fdEventGetFlag(FLAG_SOL_SANCTUM_LUNA_SOL_ROOMS_NO_TINT) == 0) {
			fdEventStart();
			fdPaletteFadeApplyTarget(PALETTE_TINT_NONE, PALETTES_BG);
			fdPaletteFadeStartInterpolation(20);
			gmFlagSet(FLAG_SOL_SANCTUM_LUNA_SOL_ROOMS_NO_TINT);
			gmFlagClear(FLAG_SOL_SANCTUM_LUNA_SOL_ROOMS_BLUE_TINTED);
			gmFlagClear(FLAG_SOL_SANCTUM_LUNA_SOL_ROOMS_WARM_TINTED);
			fdEventEnd();
		}
	}
}

/* Isaac walks south out of the room of Sol or of Luna into the corridor below
 */
/* (cells (17, 18) to (18, 18) or (35, 17) to (36, 17)), and the colours fade */
/* back to plain if they were tinted. Coming from a plain room, nothing */
/* visible happens. */
static void scSolSanctumLunaSolRoomsRestorePlainTint(void) {
	if(fdEventGetFlag(FLAG_SOL_SANCTUM_LUNA_SOL_ROOMS_NO_TINT) == 0) {
		fdEventStart();
		fdPaletteFadeApplyTarget(PALETTE_TINT_NONE, PALETTES_BG);
		fdPaletteFadeStartInterpolation(20);
		gmFlagSet(FLAG_SOL_SANCTUM_LUNA_SOL_ROOMS_NO_TINT);
		gmFlagClear(FLAG_SOL_SANCTUM_LUNA_SOL_ROOMS_BLUE_TINTED);
		gmFlagClear(FLAG_SOL_SANCTUM_LUNA_SOL_ROOMS_WARM_TINTED);
		fdEventEnd();
	}
}

/* Isaac steps into the room of Luna, from the passage at its west (cells */
/* (29, 10) and (29, 11)) or through its south doorway (cells (35, 15) and */
/* (36, 15)), and everything fades to blue. The first time, Kraden steps out: */
/* "Wh-What is this room?" He finds the picture of Luna, a moon in the centre,
 */
/* and asks Isaac to look farther up the passage. Once all four quarters are */
/* lit the room of Luna fades back to plain colours instead. */
static void scSolSanctumLunaSolRoomsTintLunaRoom(void) {
	s32 paletteActive;

	if(scSolSanctumLunaSolRoomsAreAllStatueLightsLit() != 0) {
		paletteActive = fdEventGetFlag(FLAG_SOL_SANCTUM_LUNA_SOL_ROOMS_NO_TINT);
		if(paletteActive == 0) {
			fdEventStart();
			fdPaletteFadeApplyTarget(PALETTE_TINT_NONE, PALETTES_BG);
			fdPaletteFadeStartInterpolation(20);
			gmFlagSet(FLAG_SOL_SANCTUM_LUNA_SOL_ROOMS_NO_TINT);
			gmFlagClear(FLAG_SOL_SANCTUM_LUNA_SOL_ROOMS_BLUE_TINTED);
			gmFlagClear(FLAG_SOL_SANCTUM_LUNA_SOL_ROOMS_WARM_TINTED);
			fdEventEnd();
		}
	}
	else {
		if(fdEventGetFlag(FLAG_SOL_SANCTUM_LUNA_SOL_ROOMS_BLUE_TINTED) == 0) {
			fdEventStart();
			fdPaletteFadeApplyTarget(SOL_SANCTUM_BLUE_TINT, PALETTES_BG);
			fdPaletteFadeStartInterpolation(20);
			gmFlagSet(FLAG_SOL_SANCTUM_LUNA_SOL_ROOMS_BLUE_TINTED);
			gmFlagClear(FLAG_SOL_SANCTUM_LUNA_SOL_ROOMS_NO_TINT);
			gmFlagClear(FLAG_SOL_SANCTUM_LUNA_SOL_ROOMS_WARM_TINTED);
			if(gmFlagGet(FLAG_SOL_SANCTUM_LUNA_SOL_ROOMS_SEEN) == 0) {
				scSolSanctumLunaSolRoomsIntroduceSolAndLunaRooms();
			}
			fdEventEnd();
		}
	}
}

/* Isaac walks along the short passage between the rooms of Sol and Luna */
/* (cells (25, 10) to (28, 11)), and the colours dim to a warm brown */
/* (SOL_SANCTUM_WARM_TINT). */
static void scSolSanctumLunaSolRoomsTintPassageBetweenRooms(void) {
	if(fdEventGetFlag(FLAG_SOL_SANCTUM_LUNA_SOL_ROOMS_WARM_TINTED) == 0) {
		fdPaletteFadeApplyTarget(SOL_SANCTUM_WARM_TINT, PALETTES_BG);
		fdPaletteFadeStartInterpolation(20);
		gmFlagSet(FLAG_SOL_SANCTUM_LUNA_SOL_ROOMS_WARM_TINTED);
		gmFlagClear(FLAG_SOL_SANCTUM_LUNA_SOL_ROOMS_NO_TINT);
		gmFlagClear(FLAG_SOL_SANCTUM_LUNA_SOL_ROOMS_BLUE_TINTED);
	}
}

/* Nothing visible happens when Isaac walks towards either way out, the arched
 */
/* stairs up to the statue room (cells (39, 20) to (41, 20)) or the way back */
/* to the relief rooms (cells (4, 29) and (4, 30)). The game sets the colour */
/* behind the map back to pure black, which the room tints also shade, so that
 */
/* the next room fades in from black. */
static void scSolSanctumLunaSolRoomsClearBackdropCol(void) {
	s32 black = RGB(0, 0, 0);
	u16 *backdropColor = (u16 *)&PLTT_BACKDROP;

	*backdropColor = black;
}

/* Draws the light of each lit quarter of the Luna room's picture, from four */
/* consecutive flags, with the tiles of the given source row. */
static inline void scSolSanctumLunaSolRoomsDrawStatueLightTiles(s32 firstFlag,
																s32 sourceRow) {
	if(gmFlagGet(firstFlag) != 0)
		fdMapGridCopyCellsTo(45, sourceRow, 34, 10, 2, 1);
	if(gmFlagGet(firstFlag + 1) != 0)
		fdMapGridCopyCellsTo(47, sourceRow, 36, 10, 2, 1);
	if(gmFlagGet(firstFlag + 2) != 0)
		fdMapGridCopyCellsTo(45, sourceRow + 1, 34, 11, 2, 1);
	if(gmFlagGet(firstFlag + 3) != 0)
		fdMapGridCopyCellsTo(47, sourceRow + 1, 36, 11, 2, 1);
}

/* Draws the lights of the quarters lit now (the _SEAL_OPEN flags). */
static void scSolSanctumLunaSolRoomsDrawCurrentStatueLights(void) {
	scSolSanctumLunaSolRoomsDrawStatueLightTiles(
		FLAG_SOL_SANCTUM_NORTHWEST_SEAL_OPEN, STATUE_LIGHTS_LIT_ROW);
}

/* Draws the lights the room last showed (the _LIGHT_SHOWN flags). */
static void scSolSanctumLunaSolRoomsDrawSavedStatueLights(void) {
	scSolSanctumLunaSolRoomsDrawStatueLightTiles(
		FLAG_SOL_SANCTUM_NORTHWEST_LIGHT_SHOWN, STATUE_LIGHTS_LIT_ROW);
}

/* Draws the quarters lit now unlit again. */
static void scSolSanctumLunaSolRoomsDarkenCurrentStatueLights(void) {
	scSolSanctumLunaSolRoomsDrawStatueLightTiles(
		FLAG_SOL_SANCTUM_NORTHWEST_SEAL_OPEN, STATUE_LIGHTS_DARK_ROW);
}

/* Draws the quarters the room last showed lit unlit again. */
static void scSolSanctumLunaSolRoomsDarkenSavedStatueLights(void) {
	scSolSanctumLunaSolRoomsDrawStatueLightTiles(
		FLAG_SOL_SANCTUM_NORTHWEST_LIGHT_SHOWN, STATUE_LIGHTS_DARK_ROW);
}

/* Flashes the lit quarters' lights on and off at this pace. */
static inline void
scSolSanctumLunaSolRoomsFlashCurrentStatueLights(s32 frames) {
	sySndPlay(SE_GLITTER);
	scSolSanctumLunaSolRoomsDrawCurrentStatueLights();
	fdEventWait(frames);
	sySndPlay(SE_GLITTER);
	scSolSanctumLunaSolRoomsDarkenCurrentStatueLights();
	fdEventWait(frames);
}

/* Flashes the lights last shown on and off at this pace. */
static inline void scSolSanctumLunaSolRoomsFlashSavedStatueLights(s32 frames) {
	sySndPlay(SE_GLITTER);
	scSolSanctumLunaSolRoomsDrawSavedStatueLights();
	fdEventWait(frames);
	sySndPlay(SE_GLITTER);
	scSolSanctumLunaSolRoomsDarkenSavedStatueLights();
	fdEventWait(frames);
}

/* The first time Isaac walks into the room of Sol (cells (17, 15) and */
/* (18, 15)), the music changes and Kraden hurries past him to the sun on the */
/* floor: "This is it! We've found it!" It is the mark of the sun, the heart of
 */
/* Sol Sanctum, so Saturos and Menardi told the truth and must be thieves. */
/* Jenna wants to hurry back to the village, but Kraden asks to go a little */
/* farther, and whatever Isaac answers, Garet sides with him. */
/* */
/* From then on Retreat does not work here. After the eruption this only fades
 */
/* the room back to plain colours. */
static void scSolSanctumLunaSolRoomsKradenFindsHeartOfSanctum(void) {
	ACTOR *leader;

	if(gmFlagGet(FLAG_SOL_SANCTUM_ERUPTED) != 0) {
		scSolSanctumLunaSolRoomsTintSolRoom();
	}
	if(gmFlagGet(FLAG_SOL_SANCTUM_FOUND) == 0) {
		fdEventStart();
		/* The leader walks in as the music changes. */
		fdEventSetMessageId(MsgSolSanctumFound);
		sySndPlay(SOUND_FADE_OUT_BGM);
		fdActorSetSpeed(ACTOR_PARTY_LEADER, 1);
		fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 288, 232);
		fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STILL);
		fdEventWait(20);
		sySndPlay(MUS_FOREST_THEME);
		fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
		/* Kraden hurries out of the leader to the symbol on the floor. */
		leader = fdActorGet(ACTOR_PARTY_LEADER);
		if(leader != NULL) {
			fdActorSetPosFX(ACTOR_KRADEN, leader->x.fixed, leader->z.fixed);
		}
		fdActorSetSpeed(ACTOR_KRADEN, 1.4);
		fdActorWalkToAndWait(ACTOR_KRADEN, 288, 206);
		fdEventWait(40);
		fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
		fdActorJump(ACTOR_KRADEN, 4, 60);
		/* Kraden: "This is it! We've found it!" */
		scSolSanctumLunaSolRoomsShowMessageAndWait(ACTOR_KRADEN, 20);
		/* Garet and Jenna step out behind the leader. */
		leader = fdActorGet(ACTOR_PARTY_LEADER);
		if(leader != NULL) {
			fdActorSetPosFX(ACTOR_GARET, leader->x.fixed, leader->z.fixed);
		}
		leader = fdActorGet(ACTOR_PARTY_LEADER);
		if(leader != NULL) {
			fdActorSetPosFX(ACTOR_JENNA, leader->x.fixed, leader->z.fixed);
		}
		fdActorSetSpeed(ACTOR_GARET, 0.5);
		fdActorSetSpeed(ACTOR_JENNA, 0.5);
		fdActorWalkTo(ACTOR_GARET, 280, 248);
		fdActorWalkToAndWait(ACTOR_JENNA, 296, 248);
		fdActorSetAnimation(ACTOR_GARET, ANIM_STAND);
		fdActorFaceDirection(ACTOR_GARET, FACING_NORTH_NORTHEAST, 0);
		fdActorFaceDirection(ACTOR_JENNA, FACING_NORTH_NORTHWEST, 30);
		fdCameraSetSpeed(0.6);
		fdCameraMoveTo(288, CAMERA_KEEP, 213, 1);
		fdActorSetSpeed(ACTOR_KRADEN, 0.4);
		fdActorWalkToAndWait(ACTOR_KRADEN, 288, 176);
		fdEventWait(40);
		fdActorRunRepeatedMotion(ACTOR_KRADEN, 2);
		/* Kraden: the mark of the sun, the heart of Sol Sanctum; so those two
		 * told */
		/* the truth. */
		scSolSanctumLunaSolRoomsShowMessageAndWait(ACTOR_KRADEN, 6);
		fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH, 60);
		scSolSanctumLunaSolRoomsShowMessageAndWait(ACTOR_KRADEN, 20);
		fdActorFaceDirection(ACTOR_KRADEN, FACING_EAST, 40);
		fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
		fdEventWait(10);
		fdActorFaceDirection(ACTOR_KRADEN, FACING_WEST, 40);
		fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
		fdEventWait(20);
		fdActorRunRepeatedMotion(ACTOR_JENNA, 2);
		fdActorFaceDirection(ACTOR_JENNA, FACING_WEST_NORTHWEST, 10);
		/* Jenna: then they must have been... Garet: "They were thieves!" */
		scSolSanctumLunaSolRoomsShowMessageAndWait(ACTOR_JENNA, 10);
		fdActorRunRepeatedMotion(ACTOR_GARET, 2);
		fdActorFaceDirection(ACTOR_GARET, FACING_EAST_NORTHEAST, 10);
		scSolSanctumLunaSolRoomsShowMessageAndWait(ACTOR_GARET, 6);
		fdActorSetAttachedEffect(ACTOR_JENNA, ATTACHED_EFFECT_IN_FRONT |
												  ATTACHED_EFFECT_ONCE);
		fdEventWait(40);
		fdActorFaceDirection(ACTOR_JENNA, FACING_NORTHWEST, 10);
		/* Jenna: "Let's hurry back to the village and tell everybody!" */
		scSolSanctumLunaSolRoomsShowMessageAndWait(
			SPEAKER_PORTRAIT_RIGHT | ACTOR_JENNA, 10);
		fdActorRunRepeatedMotion(ACTOR_KRADEN, 2);
		fdEventWait(10);
		fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTHWEST, 20);
		fdActorSetAttachedEffect(ACTOR_KRADEN, ATTACHED_EFFECT_IN_FRONT |
												   ATTACHED_EFFECT_ONCE);
		fdEventWait(20);
		fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH_SOUTHWEST, 40);
		fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
		fdActorFaceDirection(ACTOR_GARET, FACING_NORTHEAST, 0);
		fdActorFaceDirection(ACTOR_JENNA, FACING_NORTHWEST, 40);
		fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 20);
		/* Garet: "Kraden? What's the matter?" Kraden: "We're so close..." The
		 */
		/* three wonder at turning back from the heart of the sanctum. */
		fdEventShowMessage(ACTOR_GARET, 0);
		fdEventWait(60);
		fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
		fdEventWait(40);
		scSolSanctumLunaSolRoomsShowMessageAndWait(ACTOR_KRADEN, 20);
		fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
		scSolSanctumLunaSolRoomsShowMessageAndWait(ACTOR_JENNA, 60);
		fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
		scSolSanctumLunaSolRoomsShowMessageAndWait(ACTOR_KRADEN, 10);
		fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS,
						 0);
		fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
		fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
		fdActorRunRepeatedMotion(ACTOR_GARET, 2);
		fdEventWait(20);
		scSolSanctumLunaSolRoomsShowMessageAndWait(ACTOR_GARET, 10);
		fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
		fdEventWait(20);
		fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
		fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
		fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_WINCE, 80);
		fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 80);
		scSolSanctumLunaSolRoomsShowMessageAndWait(ACTOR_KRADEN, 6);
		fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
		fdActorFaceDirection(ACTOR_GARET, FACING_EAST_NORTHEAST, 0);
		fdActorFaceDirection(ACTOR_JENNA, FACING_WEST_NORTHWEST, 60);
		fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH, 10);
		fdActorRunRepeatedMotion(ACTOR_KRADEN, 3);
		fdEventWait(6);
		/* Kraden: "Shall we go a little farther?" Yes: "Thank you!" No: then go
		 */
		/* back without him. */
		fdEventOpenMessage(ACTOR_KRADEN, 0);
		if(fdEventAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
			fdEventSetMessageId(MsgSolSanctumThank);
		}
		else {
			fdEventSetMessageId(MsgSolSanctumGoBackVillage);
			fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_SCOWL, 20);
		}
		fdActorJump(ACTOR_KRADEN, 4, 20);
		fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
		fdActorFaceDirection(ACTOR_GARET, FACING_NORTHEAST, 0);
		fdActorFaceDirection(ACTOR_JENNA, FACING_NORTHWEST, 0);
		scSolSanctumLunaSolRoomsShowMessageAndWait(ACTOR_KRADEN, 6);
		/* Either way Jenna gives in and Garet sides with Kraden, who is */
		/* delighted: "Good! It's decided. Let's move ahead!" */
		fdEventSetMessageId(MsgSolSanctumPutWayDont);
		fdEventWait(30);
		fdActorSetAnimationAndWait(ACTOR_JENNA, ANIM_SHAKE_HEAD);
		scSolSanctumLunaSolRoomsShowMessageAndWait(
			SPEAKER_PORTRAIT_RIGHT | ACTOR_JENNA, 6);
		fdActorSetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
		scSolSanctumLunaSolRoomsShowMessageAndWait(ACTOR_GARET, 20);
		fdActorJump(ACTOR_KRADEN, 6, 20);
		fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_HEART, 20);
		scSolSanctumLunaSolRoomsShowMessageAndWait(ACTOR_KRADEN, 30);
		fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
		fdActorSetAnimation(ACTOR_GARET, ANIM_NOD);
		fdActorSetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
		fdEventWait(20);
		fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
		scSolSanctumLunaSolRoomsShowMessageAndWait(ACTOR_KRADEN, 6);
		fdActorSetSpeed(ACTOR_GARET, 1);
		fdActorSetSpeed(ACTOR_JENNA, 1);
		fdActorSetSpeed(ACTOR_KRADEN, 2);
		scWalkIntoLeader(ACTOR_KRADEN);
		scWalkIntoLeader(ACTOR_GARET);
		scWalkIntoLeader(ACTOR_JENNA);
		gmFlagSet(FLAG_RETREAT_BLOCKED);
		gmFlagSet(FLAG_SOL_SANCTUM_FOUND);
		fdEventEnd();
	}
}

/* The first time the party steps into the room of Luna, Kraden finds the */
/* picture of Luna on its floor, "a moon in the center", wonders how the two */
/* rooms are connected, and asks Isaac to look farther up the passage. He */
/* stays by the picture from then on. */
static void scSolSanctumLunaSolRoomsIntroduceSolAndLunaRooms(void) {
	ACTOR *leader;
	s32 request;

	/* Kraden steps out beside Isaac: "What is this room?" */
	fdEventSetMessageId(MsgSolSanctumWhRoom);
	fdActorSetSpeed(ACTOR_PARTY_LEADER, 1);
	fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 488, 176);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STILL);
	leader = fdActorGet(ACTOR_PARTY_LEADER);
	if(leader != NULL) {
		fdActorSetPosFX(ACTOR_KRADEN, leader->x.fixed, leader->z.fixed);
	}
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 1);
	fdActorSetSpeed(ACTOR_KRADEN, 1);
	fdActorWalkToAndWait(ACTOR_KRADEN, 472, 168);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_EAST, 60);
	fdActorJump(ACTOR_KRADEN, 4, 40);
	scSolSanctumLunaSolRoomsShowMessageAndWait(ACTOR_KRADEN, 6);
	fdCameraSetSpeed(2.4);
	fdCameraMoveTo(575, CAMERA_KEEP, 181, 1);
	fdCameraWaitForMove();
	fdEventWait(120);
	scSolSanctumLunaSolRoomsShowMessageAndWait(
		SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN, 80);
	fdCameraMoveTo(492, CAMERA_KEEP, 168, 1);
	fdCameraWaitForMove();
	fdEventWait(20);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHEAST, 20);
	scSolSanctumLunaSolRoomsShowMessageAndWait(
		SPEAKER_WINDOW_ABOVE | ACTOR_KRADEN, 6);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_EAST, 60);
	fdActorRunRepeatedMotion(ACTOR_KRADEN, 2);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHEAST, 10);
	/* Kraden: "These rooms must be connected somehow, don't you think?" */
	fdEventOpenMessage(SPEAKER_WINDOW_ABOVE | ACTOR_KRADEN, 0);
	if(fdEventAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
		fdEventSetMessageId(MsgSolSanctumLunaSolRooms);
	}
	else {
		fdEventSetMessageId(MsgSolSanctumRoomLunaOne);
	}
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 10);
	scSolSanctumLunaSolRoomsShowMessageAndWait(
		SPEAKER_WINDOW_ABOVE | ACTOR_KRADEN, 10);
	/* Kraden: would Isaac look farther up the passage? Yes: "Let me know what
	 */
	/* you find." No: "Then just wait there." */
	request = MsgSolSanctumMeanLookFarther;
	fdEventSetMessageId(request);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_EAST, 40);
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
	fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHEAST, 10);
	fdActorSetAnimation(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
	/* "What does it all mean? Isaac, would you look farther up the..." */
	fdEventOpenMessage(SPEAKER_WINDOW_ABOVE | ACTOR_KRADEN, 0);
	if(fdEventAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
		fdEventSetMessageId(request + 1);
		gmFlagSet(FLAG_SOL_SANCTUM_ISAAC_SEARCHING_FOR_KRADEN);
	}
	else {
		fdEventSetMessageId(request + 2);
	}
	scSolSanctumLunaSolRoomsShowMessageAndWait(
		SPEAKER_WINDOW_ABOVE | ACTOR_KRADEN, 4);
	fdCameraFollowActor(ACTOR_KRADEN, 1);
	fdActorWalkToAndWait(ACTOR_KRADEN, 486, 131);
	fdActorWalkToAndWait(ACTOR_KRADEN, 576, 120);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTH, 2);
	fdCameraSetSpeed(4);
	gmFlagSet(FLAG_SOL_SANCTUM_LUNA_SOL_ROOMS_SEEN);
}

/* Isaac walks south out of the room of Sol (cells (17, 17) and (18, 17)) */
/* after Kraden has found the heart of the sanctum, and Kraden hurries over: */
/* "That way leads out of the sanctum. It should be in these rooms." Isaac */
/* nods and steps back in. This stops once the way up is open, and after the */
/* eruption the cell only fades the colours back to plain. */
static void scSolSanctumLunaSolRoomsKradenStopsPartyLeaving(void) {
	ACTOR *kraden;
	ACTOR *leader;

	kraden = fdActorGet(ACTOR_KRADEN);
	if(gmFlagGet(FLAG_SOL_SANCTUM_FOUND) == 0) {
		return;
	}
	if(gmFlagGet(FLAG_SOL_SANCTUM_ERUPTED) != 0) {
		scSolSanctumLunaSolRoomsRestorePlainTint();
		return;
	}
	if(gmFlagGet(FLAG_SOL_SANCTUM_STAR_ROOM_PASSAGE_OPEN) != 0) {
		return;
	}
	fdEventStart();
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STILL);
	fdEventSetMessageId(MsgSolSanctumWayLeadsOutSanctum);
	if(fdEventGetFlag(FLAG_SOL_SANCTUM_LUNA_PICTURE_CHANGED) != 0 ||
	   gmFlagGet(FLAG_SOL_SANCTUM_LUNA_SOL_ROOMS_SEEN) == 0) {
		leader = fdActorGet(ACTOR_PARTY_LEADER);
		if(leader != NULL) {
			fdActorSetPosFX(ACTOR_KRADEN, leader->x.fixed, leader->z.fixed);
		}
		fdEventWait(4);
		fdActorSetSpeed(ACTOR_KRADEN, 1);
	}
	else if(gmFlagGet(FLAG_SOL_SANCTUM_LUNA_PICTURE_CHANGED) != 0 ||
			kraden->x.fixed > toFIXED(340)) {
		fdActorSetPos(ACTOR_KRADEN, 392, 168);
		fdEventWait(4);
		fdActorSetSpeed(ACTOR_KRADEN, 2);
	}
	if(gmFlagGet(FLAG_SOL_SANCTUM_LUNA_PICTURE_CHANGED) != 0 ||
	   kraden->x.fixed > toFIXED(340)) {
		fdActorWalkToAndWait(ACTOR_KRADEN, 288, 232);
	}
	else {
		/* DEAD: the flag's value is dropped; the ROM keeps its read. */
		gmFlagGet(FLAG_SOL_SANCTUM_LUNA_SOL_ROOMS_SEEN);
	}
	fdActorWalkToAndWait(ACTOR_KRADEN, 288, 232);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH, 10);
	/* "That way leads out of the sanctum. It should be in these rooms." */
	fdEventShowMessageThenWait(ACTOR_KRADEN, 0, 10);
	fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
	if(gmFlagGet(FLAG_SOL_SANCTUM_LUNA_PICTURE_CHANGED) != 0 ||
	   gmFlagGet(FLAG_SOL_SANCTUM_LUNA_SOL_ROOMS_SEEN) == 0) {
		scWalkIntoLeader(ACTOR_KRADEN);
		fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 288, 232);
	}
	else {
		fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 288, 248);
	}
	fdEventEnd();
}

/* One frame of a falling light in the Luna room, counted on `timer`. */
/* */
/* Three frames in four it advances its timer: the light appears at (column, */
/* top) from its staging column, falls four rows and flashes, and on frame */
/* `cleared` its path is put back. It starts again after `restart` frames and */
/* up to 40 more. */
#define STEP_FALLING_LIGHT(timer, staging, column, top, cleared, restart)      \
	if((syRandomNext() & 3) != 0) {                                            \
		switch(timer) {                                                        \
		case 0:                                                                \
			sySndPlay(SE_FALLING_LIGHT);                                       \
			DRAW_LIGHT(staging, column, top);                                  \
			break;                                                             \
		case 1:                                                                \
			MOVE_LIGHT_DOWN(staging, column, (top) + 1);                       \
			break;                                                             \
		case 2:                                                                \
			MOVE_LIGHT_DOWN(staging, column, (top) + 2);                       \
			break;                                                             \
		case 3:                                                                \
			MOVE_LIGHT_DOWN(staging, column, (top) + 3);                       \
			break;                                                             \
		case 4:                                                                \
			SolSanctumLunaSolRooms_LandingShake = 2;                           \
			MOVE_LIGHT_DOWN(staging, column, (top) + 4);                       \
			break;                                                             \
		case cleared:                                                          \
			CLEAR_LIGHT_PATH(staging, column, top);                            \
			break;                                                             \
		}                                                                      \
		timer++;                                                               \
		if(timer > (syRandomBelow(40)) + (restart)) {                          \
			timer = 0;                                                         \
		}                                                                      \
	}

/* The first light falling through the Luna room, which also shakes the rooms */
/* as any of the four lands. */
static void scSolSanctumLunaSolRoomsUpdateFirstFallingLight(void) {
	STEP_FALLING_LIGHT(SolSanctumLunaSolRooms_Light1Timer, 46, 30, 33, 80, 90)
	if(SolSanctumLunaSolRooms_LandingShake != 0) {
		if(SolSanctumLunaSolRooms_LandingShake == 2) {
			fdMapGridSetShake(1, 1, 1);
		}
		else if(SolSanctumLunaSolRooms_LandingShake == 1) {
			fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
		}
		SolSanctumLunaSolRooms_LandingShake--;
	}
}

/* Defines `name`, the task of one more falling light in the Luna room, with */
/* its own timer and random restart delay (STEP_FALLING_LIGHT). */
#define SOL_SANCTUM_FALLING_LIGHT(name, timer, staging, column, top, cleared,  \
								  restart)                                     \
	static void name(void) {                                                   \
		STEP_FALLING_LIGHT(timer, staging, column, top, cleared, restart)      \
	}

/* The Luna room's second falling light. */
SOL_SANCTUM_FALLING_LIGHT(SolSanctumLunaSolRooms_UpdateSecondFallingLight,
						  SolSanctumLunaSolRooms_Light2Timer, 47, 42, 33, 90,
						  100)
/* The Luna room's third falling light. */
SOL_SANCTUM_FALLING_LIGHT(SolSanctumLunaSolRooms_UpdateThirdFallingLight,
						  SolSanctumLunaSolRooms_Light3Timer, 48, 31, 36, 95,
						  105)
/* The Luna room's fourth falling light. */
SOL_SANCTUM_FALLING_LIGHT(SolSanctumLunaSolRooms_UpdateFourthFallingLight,
						  SolSanctumLunaSolRooms_Light4Timer, 46, 41, 36, 85,
						  95)

/* Opens on Kraden in the Luna room in SOL_SANCTUM_BLUE_TINT, as a flash */
/* crosses it. */
static void scSolSanctumLunaSolRoomsOpenOnKradenInLunaRoom(void) {
	fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
	fdMapGridCopyCellsTo(30, 43, 32, 40, 8, 3);
	fdMapGridCopyCellsTo(30, 43, 33, 39, 8, 1);
	fdMapGridCopyCellsTo(30, 43, 36, 38, 3, 3);
	fdMapGridCopyCellsTo(14, 41, 32, 41, 8, 4);
	fdCameraMoveTo(574, CAMERA_KEEP, 158, 0);
	fdMapGridCenterOnFollowedPoint();
	fdActorSetPos(ACTOR_KRADEN, 574, 120);
	fdActorSetPos(ACTOR_PARTY_LEADER, 0, 0);
	sySynch(1);
	fdPaletteFadeApplyTarget(SOL_SANCTUM_BLUE_TINT, PALETTES_BG);
	fdPaletteFadeStartInterpolation(20);
	gmFlagSet(FLAG_SOL_SANCTUM_LUNA_SOL_ROOMS_BLUE_TINTED);
	gmFlagClear(FLAG_SOL_SANCTUM_LUNA_SOL_ROOMS_NO_TINT);
	gmFlagClear(FLAG_SOL_SANCTUM_LUNA_SOL_ROOMS_WARM_TINTED);
	FieldStateWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
	FieldStateWork->transitionFrm = 32;
	fdEventStartScrTransition();
	fdEventWaitTransitionFrames();
	fdEventWait(40);
	sySndPlay(SE_THUNDER);
	fdPaletteFadeApplyTarget(PALETTE_TINT_PALE, PALETTES_BG);
	fdPaletteFadeStartInterpolation(8);
	fdEventWait(32);
	fdPaletteFadeApplyTarget(SOL_SANCTUM_BLUE_TINT, PALETTES_BG);
	fdPaletteFadeStartInterpolation(24);
}

/* The scene cuts to Kraden by the picture of Luna when Isaac sets the first */
/* statue on its seal after the sun-disc statue has dropped into the floor. A */
/* quarter of the picture lights up and flashes faster and faster; Kraden, */
/* startled, cries "What's happening?", decides Isaac must have found */
/* something, and runs off to the statue room. */
static void scSolSanctumLunaSolRoomsFlashLightsWithTrapDisarmed(void) {
	s32 twelveFrameFlashes;
	s32 eightFrameFlashes;
	s32 sixFrameFlashes;
	s32 fourFrameFlashes;
	s32 twoFrameFlashes;

	fdEventStart();
	scSolSanctumLunaSolRoomsOpenOnKradenInLunaRoom();
	/* Kraden turns, startled: "What's happening?" */
	fdEventSetMessageId(MsgSolSanctumWhatsHappeningAtTheTrap);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH, 20);
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdActorJump(ACTOR_KRADEN, 6, 30);
	fdCameraMoveTo(574, CAMERA_KEEP, 174, 1);
	fdCameraWaitForMove();
	fdEventWait(30);
	scSolSanctumLunaSolRoomsShowMessageAndWait(
		SPEAKER_WINDOW_BELOW | ACTOR_KRADEN, 20);
	/* Shorten each flash interval as the flashing accelerates. */
	for(twelveFrameFlashes = 0; twelveFrameFlashes != 4; twelveFrameFlashes++) {
		scSolSanctumLunaSolRoomsFlashCurrentStatueLights(12);
	}
	for(eightFrameFlashes = 0; eightFrameFlashes != 6; eightFrameFlashes++) {
		scSolSanctumLunaSolRoomsFlashCurrentStatueLights(8);
	}
	for(sixFrameFlashes = 0; sixFrameFlashes != 8; sixFrameFlashes++) {
		scSolSanctumLunaSolRoomsFlashCurrentStatueLights(6);
	}
	for(fourFrameFlashes = 0; fourFrameFlashes != 10; fourFrameFlashes++) {
		scSolSanctumLunaSolRoomsFlashCurrentStatueLights(4);
	}
	for(twoFrameFlashes = 0; twoFrameFlashes != 12; twoFrameFlashes++) {
		scSolSanctumLunaSolRoomsFlashCurrentStatueLights(2);
	}
	scSolSanctumLunaSolRoomsDrawCurrentStatueLights();
	fdEventWait(6);
	/* Kraden: "Isaac must have found something!" He runs off south. */
	scSolSanctumLunaSolRoomsShowMessageAndWait(
		SPEAKER_WINDOW_BELOW | ACTOR_KRADEN, 6);
	fdActorSetSpeed(ACTOR_KRADEN, 2);
	fdActorWalkToAndWait(ACTOR_KRADEN, 576, 280);
	FieldStateWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
	FieldStateWork->transitionFrm = 32;
	fdEventFinishScrTransition();
	fdEventWaitTransitionFrames();
	gmFlagSet(FLAG_SOL_SANCTUM_KRADEN_SAID_FOUND_SOMETHING);
	syImportEventSetExitRequest(
		SOL_SANCTUM_LUNA_SOL_ROOMS_TO_SOL_SANCTUM_STATUE_TRAP_DISARMED);
}

/* The scene cuts to Kraden by the picture of Luna when Isaac first sets a */
/* statue on its seal, before the trap is disarmed. The lights flash, then */
/* lights start falling through the room of Luna, and Kraden runs off to the */
/* statue room crying, "Isaac! Stop!!! You're about to make a big mistake!" */
static void scSolSanctumLunaSolRoomsSpringTrap(void) {
	s32 flash;
	fdEventStart();
	scSolSanctumLunaSolRoomsOpenOnKradenInLunaRoom();
	SolSanctumLunaSolRooms_Light1Timer = 0;
	SolSanctumLunaSolRooms_Light2Timer = 0;
	SolSanctumLunaSolRooms_Light3Timer = 0;
	SolSanctumLunaSolRooms_Light4Timer = 0;
	/* Kraden turns, startled: "What's happening?" The lights flash four times.
	 */
	fdEventSetMessageId(MsgSolSanctumWhatsHappening);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH, 20);
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdActorJump(ACTOR_KRADEN, 6, 30);
	fdCameraMoveTo(574, CAMERA_KEEP, 174, 1);
	fdCameraWaitForMove();
	fdEventWait(30);
	scSolSanctumLunaSolRoomsShowMessageAndWait(
		SPEAKER_WINDOW_BELOW | ACTOR_KRADEN, 20);
	for(flash = 0; flash != 4; flash++) {
		scSolSanctumLunaSolRoomsFlashCurrentStatueLights(12);
	}
	/* Kraden: "Isaac must have found something!" */
	scSolSanctumLunaSolRoomsShowMessageAndWait(
		SPEAKER_WINDOW_BELOW | ACTOR_KRADEN, 6);
	/* Each light starts falling after 20 to 79 frames. */
	SolSanctumLunaSolRooms_Light1Timer = (syRandomBelow(60)) + 20;
	SolSanctumLunaSolRooms_Light2Timer = (syRandomBelow(60)) + 20;
	SolSanctumLunaSolRooms_Light3Timer = (syRandomBelow(60)) + 20;
	SolSanctumLunaSolRooms_Light4Timer = (syRandomBelow(60)) + 20;
	SolSanctumLunaSolRooms_LandingShake = 0;
	syTaskAddFrame(scSolSanctumLunaSolRoomsUpdateFirstFallingLight,
				   TASK_PASS_FRAME);
	syTaskAddFrame(SolSanctumLunaSolRooms_UpdateSecondFallingLight,
				   TASK_PASS_FRAME);
	syTaskAddFrame(SolSanctumLunaSolRooms_UpdateThirdFallingLight,
				   TASK_PASS_FRAME);
	syTaskAddFrame(SolSanctumLunaSolRooms_UpdateFourthFallingLight,
				   TASK_PASS_FRAME);
	for(flash = 0; flash != 6; flash++) {
		scSolSanctumLunaSolRoomsFlashCurrentStatueLights(5);
	}
	for(flash = 0; flash != 8; flash++) {
		scSolSanctumLunaSolRoomsFlashCurrentStatueLights(4);
	}
	for(flash = 0; flash != 10; flash++) {
		scSolSanctumLunaSolRoomsFlashCurrentStatueLights(3);
	}
	for(flash = 0; flash != 12; flash++) {
		scSolSanctumLunaSolRoomsFlashCurrentStatueLights(2);
	}
	fdMapGridCopyCellsTo(45, STATUE_LIGHTS_DARK_ROW, 34, 10, 4, 2);
	fdActorJump(ACTOR_KRADEN, 6, 40);
	/* Kraden: "Stop!!! You're about to make a big mistake!" He runs south. */
	scSolSanctumLunaSolRoomsShowMessageAndWait(
		SPEAKER_WINDOW_BELOW | ACTOR_KRADEN, 6);
	fdActorSetSpeed(ACTOR_KRADEN, 2);
	fdActorWalkToAndWait(ACTOR_KRADEN, 576, 280);
	syCloseEvent(scSolSanctumLunaSolRoomsUpdateFirstFallingLight);
	syCloseEvent(SolSanctumLunaSolRooms_UpdateSecondFallingLight);
	syCloseEvent(SolSanctumLunaSolRooms_UpdateThirdFallingLight);
	syCloseEvent(SolSanctumLunaSolRooms_UpdateFourthFallingLight);
	FieldStateWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
	FieldStateWork->transitionFrm = 32;
	fdEventFinishScrTransition();
	fdEventWaitTransitionFrames();
	syImportEventSetExitRequest(
		SOL_SANCTUM_LUNA_SOL_ROOMS_TO_SOL_SANCTUM_STATUE_TRAP_TRIGGERED);
}

/* Unused: the only way here is a branch of scSolSanctumStatueOpenSeal that */
/* cannot run (SOL_SANCTUM_STATUE_TO_SOL_SANCTUM_LUNA_SOL_ROOMS_LIGHTS_SHOWN).
 */
static void scSolSanctumLunaSolRoomsUnusedAnimateSavedStatueLights(void) {
	s32 speaker;
	s32 twelveFrameFlashes;
	s32 eightFrameFlashes;
	s32 sixFrameFlashes;
	s32 fourFrameFlashes;
	s32 twoFrameFlashes;

	fdEventStart();
	/* Toggle the shown lights only where the quarters are lit now. */
	if(gmFlagGet(FLAG_SOL_SANCTUM_NORTHWEST_SEAL_OPEN) != 0 &&
	   fdEventGetFlag(FLAG_SOL_SANCTUM_NORTHWEST_LIGHT_SHOWN) != 0) {
		gmFlagClear(FLAG_SOL_SANCTUM_NORTHWEST_LIGHT_SHOWN);
		fdMapGridCopyCellsTo(45, STATUE_LIGHTS_LIT_ROW, 34, 10, 2, 1);
	}
	else if(gmFlagGet(FLAG_SOL_SANCTUM_NORTHWEST_SEAL_OPEN) != 0 &&
			fdEventGetFlag(FLAG_SOL_SANCTUM_NORTHWEST_LIGHT_SHOWN) == 0) {
		gmFlagSet(FLAG_SOL_SANCTUM_NORTHWEST_LIGHT_SHOWN);
	}
	if(gmFlagGet(FLAG_SOL_SANCTUM_NORTHEAST_SEAL_OPEN) != 0 &&
	   fdEventGetFlag(FLAG_SOL_SANCTUM_NORTHEAST_LIGHT_SHOWN) != 0) {
		gmFlagClear(FLAG_SOL_SANCTUM_NORTHEAST_LIGHT_SHOWN);
		fdMapGridCopyCellsTo(47, STATUE_LIGHTS_LIT_ROW, 36, 10, 2, 1);
	}
	else if(gmFlagGet(FLAG_SOL_SANCTUM_NORTHEAST_SEAL_OPEN) != 0 &&
			fdEventGetFlag(FLAG_SOL_SANCTUM_NORTHEAST_LIGHT_SHOWN) == 0) {
		gmFlagSet(FLAG_SOL_SANCTUM_NORTHEAST_LIGHT_SHOWN);
	}
	if(gmFlagGet(FLAG_SOL_SANCTUM_SOUTHWEST_SEAL_OPEN) != 0 &&
	   fdEventGetFlag(FLAG_SOL_SANCTUM_SOUTHWEST_LIGHT_SHOWN) != 0) {
		gmFlagClear(FLAG_SOL_SANCTUM_SOUTHWEST_LIGHT_SHOWN);
		fdMapGridCopyCellsTo(45, STATUE_LIGHTS_LIT_ROW + 1, 34, 11, 2, 1);
	}
	else if(gmFlagGet(FLAG_SOL_SANCTUM_SOUTHWEST_SEAL_OPEN) != 0 &&
			fdEventGetFlag(FLAG_SOL_SANCTUM_SOUTHWEST_LIGHT_SHOWN) == 0) {
		gmFlagSet(FLAG_SOL_SANCTUM_SOUTHWEST_LIGHT_SHOWN);
	}
	if(gmFlagGet(FLAG_SOL_SANCTUM_SOUTHEAST_SEAL_OPEN) != 0 &&
	   fdEventGetFlag(FLAG_SOL_SANCTUM_SOUTHEAST_LIGHT_SHOWN) != 0) {
		gmFlagClear(FLAG_SOL_SANCTUM_SOUTHEAST_LIGHT_SHOWN);
		fdMapGridCopyCellsTo(47, STATUE_LIGHTS_LIT_ROW + 1, 36, 11, 2, 1);
	}
	else if(gmFlagGet(FLAG_SOL_SANCTUM_SOUTHEAST_SEAL_OPEN) != 0 &&
			fdEventGetFlag(FLAG_SOL_SANCTUM_SOUTHEAST_LIGHT_SHOWN) == 0) {
		gmFlagSet(FLAG_SOL_SANCTUM_SOUTHEAST_LIGHT_SHOWN);
	}
	scSolSanctumLunaSolRoomsOpenOnKradenInLunaRoom();
	fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH, 20);
	fdActorJump(ACTOR_KRADEN, 6, 30);
	fdCameraMoveTo(574, CAMERA_KEEP, 174, 1);
	fdCameraWaitForMove();
	fdEventWait(30);
	for(twelveFrameFlashes = 0; twelveFrameFlashes != 4; twelveFrameFlashes++) {
		scSolSanctumLunaSolRoomsFlashSavedStatueLights(12);
	}
	for(eightFrameFlashes = 0; eightFrameFlashes != 6; eightFrameFlashes++) {
		scSolSanctumLunaSolRoomsFlashSavedStatueLights(8);
	}
	for(sixFrameFlashes = 0; sixFrameFlashes != 8; sixFrameFlashes++) {
		scSolSanctumLunaSolRoomsFlashSavedStatueLights(6);
	}
	for(fourFrameFlashes = 0; fourFrameFlashes != 10; fourFrameFlashes++) {
		scSolSanctumLunaSolRoomsFlashSavedStatueLights(4);
	}
	for(twoFrameFlashes = 0; twoFrameFlashes != 12; twoFrameFlashes++) {
		scSolSanctumLunaSolRoomsFlashSavedStatueLights(2);
	}
	sySndPlay(SE_GLITTER);
	scSolSanctumLunaSolRoomsDrawSavedStatueLights();
	fdEventWait(6);
	/* Kraden, once: "You found it, Isaac." and "Keep doing that until the light
	 */
	/* bathes all the remaining quarters." */
	if(gmFlagGet(FLAG_SOL_SANCTUM_KRADEN_SAID_YOU_FOUND_IT) == 0) {
		speaker = SPEAKER_WINDOW_BELOW | ACTOR_KRADEN;
		fdEventSetMessageId(MsgSolSanctumYouFoundIt);
		scSolSanctumLunaSolRoomsShowMessageAndWait(speaker, 6);
		fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
		scSolSanctumLunaSolRoomsShowMessageAndWait(speaker, 6);
	}
	FieldStateWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
	FieldStateWork->transitionFrm = 32;
	fdEventFinishScrTransition();
	fdEventWaitTransitionFrames();
	syImportEventSetExitRequest(
		SOL_SANCTUM_LUNA_SOL_ROOMS_TO_SOL_SANCTUM_STATUE_LIGHTS_SHOWN);
}

/* Isaac comes back through the room of Luna's south doorway (cells (35, 16) */
/* and (36, 16)) once all four quarters are lit, and Kraden, by the picture, */
/* calls to him: "Look, Isaac! The picture of Luna changed to Sol!" But */
/* nothing else looks different, and he decides something else must have */
/* changed somewhere. */
static void scSolSanctumLunaSolRoomsKradenSeesLunaPictureChange(void) {
	if(fdEventGetFlag(FLAG_SOL_SANCTUM_LUNA_PICTURE_CHANGED) == 0) {
		if(scSolSanctumLunaSolRoomsAreAllStatueLightsLit() != 0) {
			fdEventStart();
			fdActorSetPos(ACTOR_KRADEN, 577, 147);
			fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH, 1);
			fdCameraMoveTo(574, CAMERA_KEEP, 184, 1);
			/* Isaac walks up to Kraden, who hops: "The picture of Luna changed
			 * to */
			/* Sol!" */
			fdEventSetMessageId(MsgSolSanctumThePictureOfLunaChanged);
			fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 576, 232);
			fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STILL);
			fdCameraWaitForMove();
			fdEventWait(10);
			fdActorSetSpeed(ACTOR_KRADEN, 1);
			fdActorWalkToAndWait(ACTOR_KRADEN, 576, 152);
			fdEventWait(6);
			fdActorJump(ACTOR_KRADEN, 6, 30);
			scSolSanctumLunaSolRoomsShowMessageAndWait(ACTOR_KRADEN, 6);
			fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
			fdEventWait(2);
			fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
			/* Kraden: "But other than that, nothing looks different..." */
			scSolSanctumLunaSolRoomsShowMessageAndWait(ACTOR_KRADEN, 6);
			fdActorSetAttachedEffect(ACTOR_PARTY_LEADER,
									 ATTACHED_EFFECT_IN_FRONT |
										 ATTACHED_EFFECT_ONCE);
			fdEventWait(40);
			fdActorRunRepeatedMotion(ACTOR_KRADEN, 2);
			fdEventWait(30);
			/* Kraden: a trap here, and this is all that happens? */
			scSolSanctumLunaSolRoomsShowMessageAndWait(ACTOR_KRADEN, 6);
			fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
			fdActorWalkToAndWait(ACTOR_KRADEN, 576, 184);
			fdEventWait(6);
			fdActorRunRepeatedMotion(ACTOR_KRADEN, 2);
			fdEventWait(40);
			/* Kraden: "Somewhere, something else must have changed." He walks
			 * back */
			/* into the leader. */
			scSolSanctumLunaSolRoomsShowMessageAndWait(
				SPEAKER_WINDOW_ABOVE | ACTOR_KRADEN, 6);
			fdActorWalkToAndWait(ACTOR_KRADEN, 576, 208);
			fdEventWait(40);
			fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
			fdEventWait(6);
			fdActorSetSpeed(ACTOR_KRADEN, 0.5);
			scWalkIntoLeader(ACTOR_KRADEN);
			gmFlagSet(FLAG_SOL_SANCTUM_LUNA_PICTURE_CHANGED);
			fdEventEnd();
		}
	}
}

/* Kraden comes out beside Isaac in the room of Sol once all four quarters are
 */
/* lit: "Look! The symbol on the floor! It changed from Sol to Luna!" Surely */
/* there must be something in this room. */
static void scSolSanctumLunaSolRoomsDiscussFloorSymbolChangingToLuna(void) {
	sySndPlay(MUS_FOREST_THEME);
	fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 376, 184);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STILL);
	fdActorSetPos(ACTOR_KRADEN, 376, 184);
	fdActorSetSpeed(ACTOR_KRADEN, 1);
	fdActorWalkToAndWait(ACTOR_KRADEN, 392, 168);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_WEST, 30);
	fdActorSetAnimation(ACTOR_KRADEN, ANIM_STAND);
	fdEventSetMessageId(MsgSolSanctumLookSymbolFloor);
	fdActorJump(ACTOR_KRADEN, 4, 30);
	scSolSanctumLunaSolRoomsShowMessageAndWait(ACTOR_KRADEN, 6);
	fdActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
	fdEventWait(6);
	fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
	scSolSanctumLunaSolRoomsShowMessageAndWait(ACTOR_KRADEN, 6);
	fdActorWalkToAndWait(ACTOR_KRADEN, 376, 184);
	/* Off the map, until he next comes out of the party. */
	fdActorSetPos(ACTOR_KRADEN, 1608, 1608);
	fdEventWait(4);
	gmFlagSet(FLAG_SOL_SANCTUM_FLOOR_SYMBOL_SEEN);
}

/* Isaac searches the north wall of the room of Sol, behind the symbol (cells */
/* (17, 6) and (18, 6)), once Kraden has seen the symbol change. Kraden comes */
/* out beside him, a portal opens with a shimmer ("Ah ha! This has to be it."),
 */
/* and Isaac rises slowly into the air, blinking, and vanishes upward; Kraden */
/* follows, "Wait for me, Isaac... I'm on my way!", and they arrive in the */
/* Elemental Star Room. */
/* */
/* Before Kraden has seen the symbol change, nothing happens. */
static void scSolSanctumLunaSolRoomsRiseToElementalStarRoom(void) {
	ACTOR *actor;
	s32 frame;
	SPR *sprite;

	if(gmFlagGet(FLAG_SOL_SANCTUM_FLOOR_SYMBOL_SEEN) != 0) {
		fdEventStart();
		/* Isaac steps onto the symbol and Kraden comes out beside him. */
		fdCameraSetSpeed(1);
		fdCameraMoveTo(287, CAMERA_KEEP, 148, 1);
		fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 288, 120);
		fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STILL);
		fdActorJump(ACTOR_PARTY_LEADER, 4, 30);
		fdActorSetPos(ACTOR_KRADEN, 288, 120);
		fdActorSetSpeed(ACTOR_KRADEN, 1);
		fdActorWalkToAndWait(ACTOR_KRADEN, 276, 136);
		fdActorWalkTo(ACTOR_KRADEN, 264, 136);
		fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 312, 136);
		fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
		fdActorSetAnimation(ACTOR_KRADEN, ANIM_STAND);
		fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH_NORTHWEST, 0);
		fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHEAST, 20);
		if(fdEventGetFlag(FLAG_SOL_SANCTUM_STAR_ROOM_PASSAGE_OPEN) == 0) {
			sySndPlay(SE_SHIMMER);
		}
		fdEventWait(40);
		/* Reveal the passage only once; later visits reuse its open state. */
		if(fdEventGetFlag(FLAG_SOL_SANCTUM_STAR_ROOM_PASSAGE_OPEN) == 0) {
			fdMapGridCopyCellsTo(36, 62, 17, 36, 2, 3);
			fdMapGridCopyCellsTo(44, 59, 17, 38, 2, 1);
			fdEventWait(10);
			fdMapGridCopyCellsTo(38, 62, 17, 36, 2, 3);
			fdMapGridCopyCellsTo(44, 59, 17, 39, 2, 1);
			fdEventWait(10);
			fdMapGridCopyCellsTo(40, 62, 17, 36, 2, 3);
			fdMapGridCopyCellsTo(0, 32, 17, 39, 2, 1);
			fdMapGridCopyCellsTo(44, 59, 17, 40, 2, 1);
			fdEventWait(10);
			fdActorShowEmote(ACTOR_PARTY_LEADER,
							 EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
			fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
							 0);
			fdMapGridCopyCellsTo(42, 62, 17, 36, 2, 3);
			fdMapGridCopyCellsTo(0, 32, 17, 40, 2, 1);
			fdMapGridCopyCellsTo(44, 59, 17, 41, 2, 1);
			fdEventWait(10);
			fdMapGridCopyCellsTo(0, 32, 17, 41, 2, 1);
			fdMapGridCopyCellsTo(44, 59, 17, 42, 2, 1);
			fdEventWait(10);
			fdMapGridCopyCellsTo(0, 32, 17, 42, 2, 3);
			fdEventWait(80);
			fdTilesetAnimResume(WAY_UP_ANIMATION_A);
			fdTilesetAnimResume(WAY_UP_ANIMATION_B);
			gmFlagSet(FLAG_SOL_SANCTUM_STAR_ROOM_PASSAGE_OPEN);
		}
		fdActorFaceEachOther(ACTOR_KRADEN, ACTOR_PARTY_LEADER, 30);
		fdActorSetAnimation(ACTOR_KRADEN, ANIM_NOD);
		fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
		fdActorSetAnimation(ACTOR_KRADEN, ANIM_STAND);
		fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STILL);
		/* Kraden: "Ah ha! This has to be it." and "Let's go, Isaac!" */
		fdEventSetMessageId(MsgSolSanctumHa);
		scSolSanctumLunaSolRoomsShowMessageAndWait(
			SPEAKER_WINDOW_BELOW | ACTOR_KRADEN, 6);
		fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
		fdActorSetAnimation(ACTOR_KRADEN, ANIM_STAND);
		fdEventShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_KRADEN, 0);
		fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
		fdEventWait(60);
		actor = fdActorGet(ACTOR_PARTY_LEADER);
		fdCameraSetSpeed(0.6);
		fdCameraMoveTo(287, CAMERA_KEEP, 114, 1);
		fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 288, 120);
		fdEventWait(20);
		fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 20);
		fdActorSetSpeed(ACTOR_PARTY_LEADER, 0.3);
		actor->steeringFlg &= ~ACTOR_FACES_MOTION;
		/* Default: NULL until it is pointed at the leader's sprite below. */
		sprite = NULL;
		actor->motionFlg = 0;
		sySndPlay(SE_ASCEND);
		fdActorSetSprColById(ACTOR_PARTY_LEADER, SPRITE_COLOR_BLINK);
		sprite = actor->spr;
		frame = 0;
		sprite->flg = 0;
		do {
			actor->y.fixed += toFIXED(1) / 5;
			sySynch(1);
			frame++;
		} while(frame != 120);
		sySndPlay(SE_WARP);
		for(frame = 0; frame != 60; frame++) {
			actor->y.fixed += toFIXED(1) / 10;
			sprite->scale -= FIXED_ONE / 64;
			sySynch(1);
		}
		fdActorSetPos(ACTOR_PARTY_LEADER, 0, 0);
		fdActorJump(ACTOR_KRADEN, 4, 20);
		/* Kraden: "Wait for me, Isaac... I'm on my way!" */
		scSolSanctumLunaSolRoomsShowMessageAndWait(ACTOR_KRADEN, 6);
		fdActorWalkToAndWait(ACTOR_KRADEN, 288, 120);
		fdActorRunRepeatedMotion(ACTOR_KRADEN, 2);
		fdEventWait(20);
		fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTH, 20);
		fdActorSetSpeed(ACTOR_KRADEN, 0.3);
		actor = fdActorGet(ACTOR_KRADEN);
		actor->steeringFlg &= ~ACTOR_FACES_MOTION;
		actor->motionFlg = 0;
		sySndPlay(SE_ASCEND);
		fdActorSetSprColById(ACTOR_KRADEN, SPRITE_COLOR_BLINK);
		sprite = actor->spr;
		sprite->flg = 0;
		frame = 0;
		do {
			actor->y.fixed += toFIXED(1) / 5;
			sySynch(1);
			frame++;
		} while(frame != 120);
		sySndPlay(SE_WARP);
		for(frame = 0; frame != 60; frame++) {
			actor->y.fixed += toFIXED(1) / 10;
			sprite->scale -= FIXED_ONE / 64;
			sySynch(1);
		}
		fdActorSetPos(ACTOR_KRADEN, 0, 0);
		fdEventWait(80);
		FieldStateWork->transition =
			SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_CIRCLE);
		FieldStateWork->transitionFrm = 24;
		fdEventFinishScrTransition();
		fdEventWaitTransitionFrames();
		fdPaletteFadeApplyTarget(RGB(0, 0, 0), PALETTES_ALL);
		fdPaletteFadeStartInterpolation(1);
		sySynch(1);
		syImportEventSetExitRequest(
			SOL_SANCTUM_LUNA_SOL_ROOMS_TO_SOL_SANCTUM_ELEMENTAL_STAR_ROOM);
		fdEventEnd();
	}
}

/* Sets up Sol Sanctum's rooms of Sol and Luna and plays the event this */
/* entrance calls for. */
s32 scInit(void) {
	s32 playedEntranceEvent;
	s32 anyLightLit;

	sySynch(1);
	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
	playedEntranceEvent = 0;
	if(gmFlagGet(FLAG_SOL_SANCTUM_FOUND) != 0 &&
	   gmFlagGet(FLAG_SOL_SANCTUM_ERUPTED) == 0 &&
	   gmFlagGet(FLAG_SOL_SANCTUM_STAR_ROOM_PASSAGE_OPEN) == 0) {
		gmFlagSet(FLAG_RETREAT_BLOCKED);
	}
	fdPaletteFadeApplySource(PALETTE_TINT_NONE, PALETTES_ALL);
	if(gmFlagGet(FLAG_KEEP_PARTY_POSITION) != 0) {
		if(gmFlagGet(FLAG_SOL_SANCTUM_LUNA_SOL_ROOMS_BLUE_TINTED) != 0) {
			fdPaletteFadeApplyTarget(SOL_SANCTUM_BLUE_TINT, PALETTES_BG);
			fdPaletteFadeStartInterpolation(1);
			sySynch(1);
		}
		else if(gmFlagGet(FLAG_SOL_SANCTUM_LUNA_SOL_ROOMS_WARM_TINTED) != 0) {
			fdPaletteFadeApplyTarget(SOL_SANCTUM_WARM_TINT, PALETTES_BG);
			fdPaletteFadeStartInterpolation(1);
			sySynch(1);
		}
	}
	else {
		gmFlagSet(FLAG_SOL_SANCTUM_LUNA_SOL_ROOMS_NO_TINT);
		if(gmFlagGet(FLAG_SOL_SANCTUM_LUNA_SOL_ROOMS_SEEN) != 0) {
			fdActorSetPos(ACTOR_KRADEN, 576, 120);
		}
	}
	if(MainRoutineStateWork.entrance ==
	   SOL_SANCTUM_LUNA_SOL_ROOMS_FROM_SOL_SANCTUM_STATUE_TRAP_DISARMED) {
		if(fdEventGetFlag(FLAG_SOL_SANCTUM_KRADEN_SAID_FOUND_SOMETHING) == 0) {
			scSolSanctumLunaSolRoomsFlashLightsWithTrapDisarmed();
			gmFlagSet(FLAG_SOL_SANCTUM_KRADEN_SAID_FOUND_SOMETHING);
			playedEntranceEvent = 1;
		}
	}
	else if(MainRoutineStateWork.entrance ==
			SOL_SANCTUM_LUNA_SOL_ROOMS_FROM_SOL_SANCTUM_STATUE_TRAP_TRIGGERED) {
		if(fdEventGetFlag(FLAG_SOL_SANCTUM_FALLING_LIGHTS_SEEN) == 0) {
			scSolSanctumLunaSolRoomsSpringTrap();
			gmFlagSet(FLAG_SOL_SANCTUM_FALLING_LIGHTS_SEEN);
			gmFlagClear(FLAG_SOL_SANCTUM_NORTHWEST_SEAL_OPEN);
			gmFlagClear(FLAG_SOL_SANCTUM_NORTHEAST_SEAL_OPEN);
			gmFlagClear(FLAG_SOL_SANCTUM_SOUTHWEST_SEAL_OPEN);
			gmFlagClear(FLAG_SOL_SANCTUM_SOUTHEAST_SEAL_OPEN);
			playedEntranceEvent = 1;
		}
	}
	else if(MainRoutineStateWork.entrance ==
			SOL_SANCTUM_LUNA_SOL_ROOMS_FROM_SOL_SANCTUM_STATUE_LIGHTS_SHOWN) {
		if(gmFlagGet(FLAG_SOL_SANCTUM_FALLING_LIGHTS_SEEN) != 0) {
			scSolSanctumLunaSolRoomsUnusedAnimateSavedStatueLights();
			gmFlagSet(FLAG_SOL_SANCTUM_KRADEN_SAID_YOU_FOUND_IT);
		}
		playedEntranceEvent = 1;
	}
	if(fdEventGetFlag(FLAG_SOL_SANCTUM_NORTHWEST_SEAL_OPEN) != 0) {
		gmFlagSet(FLAG_SOL_SANCTUM_NORTHWEST_LIGHT_SHOWN);
	}
	if(fdEventGetFlag(FLAG_SOL_SANCTUM_NORTHEAST_SEAL_OPEN) != 0) {
		gmFlagSet(FLAG_SOL_SANCTUM_NORTHEAST_LIGHT_SHOWN);
	}
	if(fdEventGetFlag(FLAG_SOL_SANCTUM_SOUTHWEST_SEAL_OPEN) != 0) {
		gmFlagSet(FLAG_SOL_SANCTUM_SOUTHWEST_LIGHT_SHOWN);
	}
	if(fdEventGetFlag(FLAG_SOL_SANCTUM_SOUTHEAST_SEAL_OPEN) != 0) {
		gmFlagSet(FLAG_SOL_SANCTUM_SOUTHEAST_LIGHT_SHOWN);
	}
	sySynch(4);
	if(playedEntranceEvent == 0) {
		/* With all four seals open, the sun symbol glowing on the floor is */
		/* redrawn as a crescent moon under a falling beam of light, and the */
		/* room's glow is gone; the same is copied for the other room's symbol.
		 */
		if(scSolSanctumLunaSolRoomsAreAllStatueLightsLit() != 0) {
			fdMapGridCopyCellsTo(30, 44, 30, 38, 12, 5);
			fdMapGridCopyCellsTo(30, 44, 34, 37, 4, 1);
			fdMapGridCopyCellsTo(14, 41, 32, 41, 8, 4);
			fdMapGridCopyCellsTo(45, STATUE_LIGHTS_LIT_ROW, 34, 10, 4, 2);
			fdMapGridCopyCellsTo(45, STATUE_LIGHTS_DARK_ROW, 16, 10, 4, 2);
			fdMapGridCopyCellsTo(14, 45, 14, 41, 8, 4);
			if(MainRoutineStateWork.entrance !=
			   SOL_SANCTUM_LUNA_SOL_ROOMS_FROM_SOL_SANCTUM_ERUPTION) {
				if(gmFlagGet(FLAG_SOL_SANCTUM_ERUPTED) == 0) {
					/* The cells scSolSanctumLunaSolRoomsRiseToElementalStarRoom
					 * copies as */
					/* it sets the flag, redrawn on arrival. */
					if(gmFlagGet(FLAG_SOL_SANCTUM_STAR_ROOM_PASSAGE_OPEN) !=
					   0) {
						fdMapGridCopyCellsTo(0, 32, 17, 39, 2, 1);
						fdMapGridCopyCellsTo(42, 62, 17, 36, 2, 3);
						fdMapGridCopyCellsTo(0, 32, 17, 40, 2, 1);
						fdMapGridCopyCellsTo(0, 32, 17, 41, 2, 1);
						fdMapGridCopyCellsTo(0, 32, 17, 42, 2, 3);
					}
					else {
						fdMapGridCopyCellsTo(44, 59, 17, 37, 2, 6);
					}
					fdTilesetAnimResume(WAY_UP_ANIMATION_A);
					fdTilesetAnimResume(WAY_UP_ANIMATION_B);
				}
			}
			fdActorSetPos(ACTOR_KRADEN, 0, 0);
		}
		else {
			anyLightLit = 0;
			/* Short of all four, each open seal redraws its quarter of the
			 * symbol */
			/* on each room's floor: two cells of the four-by-two picture. */
			if(gmFlagGet(FLAG_SOL_SANCTUM_NORTHWEST_SEAL_OPEN) != 0) {
				fdMapGridCopyCellsTo(45, 28, 34, 10, 2, 1);
				fdMapGridCopyCellsTo(45, 30, 16, 10, 2, 1);
				anyLightLit = 1;
			}
			if(gmFlagGet(FLAG_SOL_SANCTUM_NORTHEAST_SEAL_OPEN) != 0) {
				fdMapGridCopyCellsTo(47, 28, 36, 10, 2, 1);
				fdMapGridCopyCellsTo(47, 30, 18, 10, 2, 1);
				anyLightLit = 1;
			}
			if(gmFlagGet(FLAG_SOL_SANCTUM_SOUTHWEST_SEAL_OPEN) != 0) {
				fdMapGridCopyCellsTo(45, 29, 34, 11, 2, 1);
				fdMapGridCopyCellsTo(45, 31, 16, 11, 2, 1);
				anyLightLit = 1;
			}
			if(gmFlagGet(FLAG_SOL_SANCTUM_SOUTHEAST_SEAL_OPEN) != 0) {
				fdMapGridCopyCellsTo(47, 29, 36, 11, 2, 1);
				fdMapGridCopyCellsTo(47, 31, 18, 11, 2, 1);
				anyLightLit = 1;
			}
			if(gmFlagGet(FLAG_SOL_SANCTUM_FALLING_LIGHTS_SEEN) != 0 ||
			   anyLightLit != 0) {
				fdMapGridCopyCellsTo(30, 43, 32, 40, 8, 3);
				fdMapGridCopyCellsTo(30, 43, 33, 39, 8, 1);
				fdMapGridCopyCellsTo(30, 43, 36, 38, 3, 3);
				fdMapGridCopyCellsTo(36, 58, 32, 41, 8, 4);
			}
			/* The wall behind the symbol takes the plain wall's attributes
			 * beside */
			/* it, so searching there does nothing. */
			fdMapGridCopyCellAttributes(15, 6, 2, 1, 17, 6);
		}
	}
	/* The arrival after the Wise One's rescue plays only once. */
	if(fdEventGetFlag(FLAG_SOL_SANCTUM_RESCUE_ARRIVAL_SEEN) == 0 &&
	   MainRoutineStateWork.entrance ==
		   SOL_SANCTUM_LUNA_SOL_ROOMS_FROM_SOL_SANCTUM_ERUPTION) {
		scSolSanctumLunaSolRoomsArriveAfterWiseOnesRescue();
		gmFlagSet(FLAG_SOL_SANCTUM_RESCUE_ARRIVAL_SEEN);
		fdMapGridCopyCellAttributes(15, 6, 2, 1, 17, 6);
	}
	else {
		if(gmFlagGet(FLAG_SOL_SANCTUM_ERUPTED) != 0) {
			scQueueMusic(SE_RUMBLE);
			fdMapGridSetShake(1, 1, 1);
			fdEffectInitSparkles();
			fdMapGridCopyCellAttributes(15, 6, 2, 1, 17, 6);
		}
	}
	return 0;
}

/* Returns 1 when all four quarters are lit. */
static s32 scSolSanctumLunaSolRoomsAreAllStatueLightsLit(void) {
	s32 allLit = 1;

	if(gmFlagGet(FLAG_SOL_SANCTUM_NORTHWEST_SEAL_OPEN) == 0)
		allLit = 0;
	if(gmFlagGet(FLAG_SOL_SANCTUM_NORTHEAST_SEAL_OPEN) == 0)
		allLit = 0;
	if(gmFlagGet(FLAG_SOL_SANCTUM_SOUTHWEST_SEAL_OPEN) == 0)
		allLit = 0;
	if(gmFlagGet(FLAG_SOL_SANCTUM_SOUTHEAST_SEAL_OPEN) == 0)
		allLit = 0;

	return allLit;
}

/* Shows the next message of the event, then waits. */
static void scSolSanctumLunaSolRoomsShowMessageAndWait(s32 speaker,
													   s32 waitFrames) {
	fdEventShowMessage(speaker, 0);
	fdEventWait(waitFrames);
}

/* Sol Sanctum after the eruption: Isaac and Garet appear here in a white */
/* flash. */
static void scSolSanctumLunaSolRoomsArriveAfterWiseOnesRescue(void) {
	PALETTE_FADE *palette;

	fdEventStart();
	/* The screen opens all white on Isaac and Garet, then fades to the tint. */
	fdCameraMoveTo(286, CAMERA_KEEP, 134, 0);
	fdMapGridCenterOnFollowedPoint();
	sySynch(1);
	fdPaletteFadeApplySource(RGB_WHITE, PALETTES_ALL);
	fdPaletteFadeApplyTarget(RGB_WHITE, PALETTES_ALL);
	fdPaletteFadeStartInterpolation(1);
	sySynch(1);
	fdActorSetPos(ACTOR_GARET, 280, 134);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTHWEST, 0);
	FieldStateWork->transitionFrm = 1;
	fdEventStartScrTransition();
	fdEventWaitTransitionFrames();
	fdPaletteFadeApplyTarget(SOL_SANCTUM_BLUE_TINT, PALETTES_BG);
	fdPaletteFadeStartInterpolation(120);
	sySynch(120);
	gmFlagSet(FLAG_SOL_SANCTUM_LUNA_SOL_ROOMS_BLUE_TINTED);
	gmFlagClear(FLAG_SOL_SANCTUM_LUNA_SOL_ROOMS_NO_TINT);
	gmFlagClear(FLAG_SOL_SANCTUM_LUNA_SOL_ROOMS_WARM_TINTED);
	fdPaletteFadeApplyTarget(PALETTE_TINT_NONE, PALETTES_SPRITES);
	fdPaletteFadeStartInterpolation(60);
	sySynch(100);
	/* Both look around, bewildered, as the rooms give a jolt. */
	fdActorSetAttachedEffect(ACTOR_PARTY_LEADER,
							 ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdActorSetAttachedEffect(ACTOR_GARET,
							 ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdEventWait(60);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHEAST, 20);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTHEAST, 40);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_WEST_NORTHWEST, 40);
	fdActorFaceDirection(ACTOR_GARET, FACING_SOUTH_SOUTHWEST, 80);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 10);
	fdActorFaceDirection(ACTOR_GARET, FACING_EAST_SOUTHEAST, 60);
	fdMapGridSetShake(3, 3, 1);
	fdEventWait(20);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTHEAST, 20);
	fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
	fdEventWait(20);
	fdActorRunRepeatedMotion(ACTOR_GARET, 2);
	/* Garet: "Did that floating eye thing save us?" A stronger tremor throws */
	/* them both aside. */
	fdEventSetMessageId(MsgSolSanctumFloatingEyeThing);
	/* "Did that floating eye thing save us?" */
	fdEventOpenMessage(ACTOR_GARET, 0);
	fdEventAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
	fdEventWait(40);
	sySndPlay(SE_TREMOR);
	fdMapGridSetShake(4, 4, 1);
	fdEventWait(40);
	fdEffectInitSparkles();
	fdActorGet(ACTOR_PARTY_LEADER)->steeringFlg &= ~ACTOR_FACES_MOTION;
	fdActorGet(ACTOR_GARET)->steeringFlg &= ~ACTOR_FACES_MOTION;
	fdActorJump(ACTOR_PARTY_LEADER, 4, 0);
	fdActorJump(ACTOR_GARET, 4, 0);
	fdActorSetDestination(ACTOR_PARTY_LEADER, 300, 130);
	fdActorSetDestination(ACTOR_GARET, 266, 144);
	fdActorWaitForMoveById(ACTOR_GARET);
	fdEventWait(40);
	fdActorGet(ACTOR_PARTY_LEADER)->steeringFlg |= ACTOR_FACES_MOTION;
	fdActorGet(ACTOR_GARET)->steeringFlg |= ACTOR_FACES_MOTION;
	/* Recover from the tremor and bring Garet back into the party. */
	scQueueMusic(SE_RUMBLE);
	fdMapGridSetShake(1, 1, 1);
	fdEventWait(40);
	fdActorRunRepeatedMotion(ACTOR_GARET, 2);
	/* Garet: "Yeah, right! Let's get out of here..." */
	scSolSanctumLunaSolRoomsShowMessageAndWait(ACTOR_GARET, 10);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
	scWalkIntoLeader(ACTOR_GARET);
	palette = PaletteFadeState;
	palette->tgt[0] = 0;
	palette->tgt[1] = 0;
	palette->tgt[2] = 0;
	palette->page = 0;
	palette->duration = 1;
	palette->step = 1;
	/* DEAD: nothing reads the fade's last byte; the ROM keeps its store. */
	palette->unusedAfterStep = 1;
	scIoWrite16(0, &PLTT_BACKDROP);
	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
	FieldStateWork->transitionFrm = MAP_TRANSITION_FRAMES;
	fdEventEnd();
}

/* Isaac talks to Kraden by the picture of Luna: "Let me know what you find" if
 */
/* Isaac agreed to look farther up the passage, or "Just wait over there." */
static void scSolSanctumLunaSolRoomsTalkToKraden(void) {
	fdEventStart();
	if(gmFlagGet(FLAG_SOL_SANCTUM_ISAAC_SEARCHING_FOR_KRADEN) != 0) {
		fdEventSetMessageId(MsgSolSanctumKradenLetMeKnowWhat);
	}
	else {
		fdEventSetMessageId(MsgSolSanctumKradenJustWaitOverThere);
	}
	fdEventShowMessageThenWait(ACTOR_KRADEN, 0, 10);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTH, 10);
	fdEventEnd();
}

SCENE_ENTRANCE SolSanctumLunaSolRoomsEntrances[] = {
	{ENTRANCE_DEFAULT, CONDITION_ALWAYS, 288, 0, 157, FACING_SOUTH, 0, 0, 0,
	 752, 496, 0},
	{SOL_SANCTUM_LUNA_SOL_ROOMS_FROM_SOL_SANCTUM_RELIEFS, CONDITION_ALWAYS, 55,
	 0, 477, FACING_NORTH, 0, 0, 0, 752, 496, 0},
	{SOL_SANCTUM_LUNA_SOL_ROOMS_FROM_SOL_SANCTUM_STATUE, CONDITION_ALWAYS, 647,
	 0, 327, FACING_SOUTH, 0, 0, 0, 752, 496, 0},
	{SOL_SANCTUM_LUNA_SOL_ROOMS_UNUSED, CONDITION_ALWAYS, 287, 0, 218,
	 FACING_SOUTH, 0, 0, 0, 752, 496, 0},
	{SOL_SANCTUM_LUNA_SOL_ROOMS_FROM_SOL_SANCTUM_STATUE_TRAP_DISARMED,
	 CONDITION_ALWAYS, 576, 0, 8, FACING_SOUTH, 0, 0, 0, 752, 496, 0},
	{SOL_SANCTUM_LUNA_SOL_ROOMS_FROM_SOL_SANCTUM_STATUE_TRAP_TRIGGERED,
	 CONDITION_ALWAYS, 576, 0, 8, FACING_SOUTH, 0, 0, 0, 752, 496, 0},
	{SOL_SANCTUM_LUNA_SOL_ROOMS_FROM_SOL_SANCTUM_STATUE_LIGHTS_SHOWN,
	 CONDITION_ALWAYS, 576, 0, 8, FACING_SOUTH, 0, 0, 0, 752, 496, 0},
	{SOL_SANCTUM_LUNA_SOL_ROOMS_FROM_SOL_SANCTUM_STATUE_UNUSED,
	 CONDITION_ALWAYS, 576, 0, 8, FACING_SOUTH, 0, 0, 0, 752, 496, 0},
	{SOL_SANCTUM_LUNA_SOL_ROOMS_FROM_SOL_SANCTUM_ERUPTION, CONDITION_ALWAYS,
	 297, 0, 119, FACING_NORTHWEST, 0, 0, 0, 752, 496, 0},
	{SCENE_TABLE_END},
};

u32 SolSanctumLunaSolRoomsExits[] = {
	SCENE_EXITS(SolSanctumLunaSolRooms),
	SCENE_EXIT(SOL_SANCTUM_LUNA_SOL_ROOMS_TO_SOL_SANCTUM_RELIEFS,
			   SolSanctumReliefs,
			   SOL_SANCTUM_RELIEFS_FROM_SOL_SANCTUM_LUNA_SOL_ROOMS),
	SCENE_EXIT(SOL_SANCTUM_LUNA_SOL_ROOMS_TO_SOL_SANCTUM_STATUE,
			   SolSanctumStatue,
			   SOL_SANCTUM_STATUE_FROM_SOL_SANCTUM_LUNA_SOL_ROOMS),
	SCENE_EXIT(
		SOL_SANCTUM_LUNA_SOL_ROOMS_TO_SOL_SANCTUM_STATUE_TRAP_DISARMED,
		SolSanctumStatue,
		SOL_SANCTUM_STATUE_FROM_SOL_SANCTUM_LUNA_SOL_ROOMS_TRAP_DISARMED),
	SCENE_EXIT(
		SOL_SANCTUM_LUNA_SOL_ROOMS_TO_SOL_SANCTUM_STATUE_TRAP_TRIGGERED,
		SolSanctumStatue,
		SOL_SANCTUM_STATUE_FROM_SOL_SANCTUM_LUNA_SOL_ROOMS_TRAP_TRIGGERED),
	SCENE_EXIT(SOL_SANCTUM_LUNA_SOL_ROOMS_TO_SOL_SANCTUM_STATUE_LIGHTS_SHOWN,
			   SolSanctumStatue,
			   SOL_SANCTUM_STATUE_FROM_SOL_SANCTUM_LUNA_SOL_ROOMS_LIGHTS_SHOWN),
	SCENE_EXIT(SOL_SANCTUM_LUNA_SOL_ROOMS_TO_SOL_SANCTUM_STATUE_UNUSED,
			   SolSanctumStatue,
			   SOL_SANCTUM_STATUE_FROM_SOL_SANCTUM_LUNA_SOL_ROOMS_UNUSED),
	SCENE_EXIT(SOL_SANCTUM_LUNA_SOL_ROOMS_TO_SOL_SANCTUM_ELEMENTAL_STAR_ROOM,
			   SolSanctumElementalStarRoom,
			   SOL_SANCTUM_ELEMENTAL_STAR_ROOM_FROM_SOL_SANCTUM_LUNA_SOL_ROOMS),
	EXIT_TO_BATTLE(SOL_SANCTUM_LUNA_SOL_ROOMS_TO_BATTLE, 1),
	SCENE_EXITS_END,
};

/* clang-format off */
ACTOR_PLACE SolSanctumLunaSolRoomsActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_WINGED_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        360, 0, 152, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WINGED_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        360, 0, 200, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WINGED_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        216, 0, 200, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WINGED_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        216, 0, 152, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WINGED_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        648, 0, 152, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WINGED_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        648, 0, 200, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WINGED_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        504, 0, 200, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WINGED_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        504, 0, 152, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        1608, 0, 1608, FACING_NORTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_JENNA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        1608, 0, 1608, FACING_NORTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        1608, 0, 1608, FACING_NORTH, TALK_FACING_PARTY, 0),
    { SCENE_TABLE_END },
};
/* clang-format on */

TRIGGER SolSanctumLunaSolRoomsTriggers[] = {
	{TRIGGER_TALK, ACTOR_KRADEN, CONDITION_ALWAYS,
	 EVENT(scSolSanctumLunaSolRoomsTalkToKraden)},
	{TRIGGER_STEP, CELL_BEHAVIOR_SOL_SANCTUM_LUNA_SOL_ROOMS_TINT_INVERSE,
	 CONDITION_ALWAYS, EVENT(scSolSanctumLunaSolRoomsTintLunaRoom)},
	{TRIGGER_STEP, CELL_BEHAVIOR_SOL_SANCTUM_LUNA_SOL_ROOMS_TINT,
	 CONDITION_ALWAYS, EVENT(scSolSanctumLunaSolRoomsTintSolRoom)},
	{TRIGGER_STEP, CELL_BEHAVIOR_SOL_SANCTUM_LUNA_SOL_ROOMS_PLAIN,
	 CONDITION_ALWAYS, EVENT(scSolSanctumLunaSolRoomsRestorePlainTint)},
	{TRIGGER_STEP, CELL_BEHAVIOR_SOL_SANCTUM_LUNA_SOL_ROOMS_WARM_TINT,
	 CONDITION_ALWAYS, EVENT(scSolSanctumLunaSolRoomsTintPassageBetweenRooms)},
	{TRIGGER_STEP, CELL_BEHAVIOR_SOL_SANCTUM_LUNA_SOL_ROOMS_BLACK_BACKDROP,
	 CONDITION_ALWAYS, EVENT(scSolSanctumLunaSolRoomsClearBackdropCol)},
	{EXAMINE_TRIGGER(EXAMINE_SILENT),
	 CELL_BEHAVIOR_SOL_SANCTUM_LUNA_SOL_ROOMS_FLOOR_SYMBOL, CONDITION_ALWAYS,
	 EVENT(scSolSanctumLunaSolRoomsRiseToElementalStarRoom)},
	{TRIGGER_STEP, CELL_BEHAVIOR_SOL_SANCTUM_LUNA_SOL_ROOMS_LUNA_DOORWAY,
	 CONDITION_ALWAYS,
	 EVENT(scSolSanctumLunaSolRoomsKradenSeesLunaPictureChange)},
	{TRIGGER_EXIT, SOL_SANCTUM_LUNA_SOL_ROOMS_TO_SOL_SANCTUM_RELIEFS,
	 CONDITION_ALWAYS, SOL_SANCTUM_LUNA_SOL_ROOMS_TO_SOL_SANCTUM_RELIEFS},
	{TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP,
	 SOL_SANCTUM_LUNA_SOL_ROOMS_TO_SOL_SANCTUM_STATUE, CONDITION_ALWAYS,
	 SOL_SANCTUM_LUNA_SOL_ROOMS_TO_SOL_SANCTUM_STATUE},
	{TRIGGER_STEP, CELL_BEHAVIOR_SOL_SANCTUM_LUNA_SOL_ROOMS_HEART_OF_SANCTUM,
	 CONDITION_ALWAYS,
	 EVENT(scSolSanctumLunaSolRoomsKradenFindsHeartOfSanctum)},
	{TRIGGER_STEP, CELL_BEHAVIOR_SOL_SANCTUM_LUNA_SOL_ROOMS_WAY_OUT,
	 CONDITION_ALWAYS, EVENT(scSolSanctumLunaSolRoomsKradenStopsPartyLeaving)},
	{EXAMINE_TRIGGER(EXAMINE_CHEST),
	 CELL_BEHAVIOR_SOL_SANCTUM_LUNA_SOL_ROOMS_CHEST,
	 FLAG_SOL_SANCTUM_HERB_FOUND, FIND_ITEM(ITEM_HERB)},
	{TRIGGER_TABLE_END},
};
