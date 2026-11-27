/* @file */
/* The gangway out over the sea behind the shack in the north-west of */
/* Lalivero, where the Lemurian ship lies sunk (SceneId_LemurianShip, the area
 */
/* name Lemurian ship), and the game's last scene. */
/* */
/* The party comes in from the shack (timber_rooms_bc.c) once Iodem has given */
/* it the Black Orb and the guard lets it by. A long wooden gangway with rails
 */
/* runs north over the sea, and the ship lies beneath the water beside it, seen
 */
/* through the waves. Walking back off the gangway's south end goes back into */
/* the shack. */
/* */
/* Using the Black Orb at the gangway's north end raises the ship */
/* (scLemurianShipRaiseTheShip): the sea glows, spray rises, the screen shakes,
 */
/* and the ship comes up out of the water beside the gangway and rides the */
/* swell with a glow along its hull. The party then talks the journey over, */
/* with three yes-or-no questions, climbs aboard and sails away west as black */
/* bars close in from above and below (scLemurianShipSetSail). The game then */
/* fades to the staff credits and the clear screen (TITLE_FROM_LEMURIAN_SHIP),
 */
/* keeping the music (FLAG_KEEP_MUSIC_ONCE). */
/* */
/* The sunk ship is the map's two upper layers drawn 90 pixels low, under the */
/* sea's layer, which ripples by a line scroll (scRippleStart). Reveal stops */
/* the ripple while it shows. Arriving by LEMURIAN_SHIP_WITH_BLACK_ORB, which */
/* only the debug scene selector reaches, first gives the leader the Black Orb.
 */

#include "ability_ids.h"
#include "flags.h"
#include "item_ids.h"
#include "debug/debug.h"
#include "field/actor_script.h"
#include "field/event_inline.h"
#include "field/line_scroll.h"
#include "field/line_scroll_wave_sine.h"
#include "graphics/graphics.h"
#include "sdk/sound_driver_symbols.h"
#include "text/msg_ids.h"

s32 scInit(void);
extern u16 ShipGlowPalettes[];
extern s32 SprayFallScript[];
extern s16 RippleSine[LINE_SCROLL_WAVE_ENTRIES];
extern SCENE_ENTRANCE LemurianShipEntrances[];
extern u32 LemurianShipExits[];
extern ACTOR_PLACE LemurianShipActorPlacements[];
extern TRIGGER LemurianShipTriggers[];
extern s32 OrbGlowActive;
extern s32 OrbGlowPhase;
extern s32 OrbGlowStrength;
extern s32 OrbGlowFadeFrames;
extern s32 SprayActive;
extern s32 SwellActive;
extern s32 SwellPhase;
extern s32 ShipHullY;
extern s32 ShipDeckY;
extern s32 LeaderDeckY;
extern s32 GaretDeckY;
extern s32 MiaDeckY;
extern s32 IvanDeckY;
extern u16 ShipGlowStep;
extern s32 LeaderBoardsShip[];
extern s32 GaretBoardsShip[];
extern s32 IvanBoardsShip[];
extern s32 MiaBoardsShip[];

/* The ship's glow (scShipGlowCyclePalette): SHIP_GLOW_COLORS of */
/* ShipGlowPalettes at a time, from a start that moves one colour every */
/* SHIP_GLOW_STEP_FRAMES frames, through as many starts as colours shown. */
#define SHIP_GLOW_COLORS	  6
#define SHIP_GLOW_STEP_FRAMES 6

/* The curtains (scCurtainClose): three rows of CURTAIN_ROW_PARTS sprites, 32 */
/* by 16, one row sliding down from above the screen and two up from below, */
/* CURTAIN_START_GAP pixels in all, until they rest at the top and at */
/* CURTAIN_BOTTOM_Y. */
#define CURTAIN_PART_WIDTH	32
#define CURTAIN_PART_HEIGHT 16
#define CURTAIN_ROW_PARTS	8
#define CURTAIN_BOTTOM_Y	136
#define CURTAIN_START_GAP	48

/* The frames from the curtains starting to close to the ship sailing west: 30
 */
/* from patch 3, 80 before. */
#if TBS_PATCH_3
#define LEMURIAN_SHIP_DEPARTURE_DELAY_FRAMES 30
#else
#define LEMURIAN_SHIP_DEPARTURE_DELAY_FRAMES 80
#endif

static void scRippleStop(void);
static void scCurtainStart(void);

enum {
	/* The gangway, in two pieces (SPRITE_GANGWAY): its northern half. */
	ACTOR_GANGWAY_NORTH = ACTOR_FIRST_PLACED,
	ACTOR_GANGWAY_SOUTH
};

/* The deck height of a party member who has not yet boarded. */
/* The swell (scShipUpdateSwell) moves only those who have. */
#define SHIP_NOT_ON_DECK (-toFIXED(1))

/* The sprite priority of an actor's picture and of its shadow. */
#define SET_IMAGE_PRIORITY(actor, level)                                       \
	((void)(actor->spr->image.pri = (level)))
#define SET_SHADOW_PRIORITY(actor, level)                                      \
	((void)(actor->spr->shadow.pri = (level)))

const SCENE_ENTRANCE *scGetEntrances(void) { return LemurianShipEntrances; }

const SCENE_EXIT_REGION *scGetExitRegions(void) { return NULL; }

const u32 *scGetExits(void) { return LemurianShipExits; }

/* The gangway's two pieces. */
/* The party's own actors fill the first three rows. */
const ACTOR_PLACE *scGetActorPlacements(void) {
	return LemurianShipActorPlacements;
}

/* Walking off the south end of the gangway to the Lemurian ship (cells (18, */
/* 20) to (20, 20)) goes back into the shack in Lalivero. The backdrop colour */
/* is cleared to black for the way out. */
static void scLemurianShipLeaveTheShip(void) {
	syImportEventSetExitRequest(FieldStateWork->stepTrigger);
	scIoWrite16(0, &PLTT_BACKDROP);
}

/* Casting Reveal on the gangway to the Lemurian ship stops the sea rippling */
/* while the grey circle of Reveal shows; there is nothing hidden here. */
static void scRippleStopForReveal(void) { scRippleStop(); }

/* The sea around the gangway to the Lemurian ship ripples, from the moment the
 */
/* party arrives and again as Reveal ends. The ripple sways the sea's layer */
/* across and up and down at once (scLineScrollInitWave). */
static void scRippleStart(void) {
	scLineScrollInitWave(0, FIXED_ONE * 4, FIXED_ONE, FIXED_ONE / 8, FIXED_ONE,
						 FIXED_ONE / 2, FIXED_ONE / 4);
}

/* The way back, Reveal, and the Black Orb at the gangway's north end. */
const TRIGGER *scGetTriggers(void) { return LemurianShipTriggers; }

/* Runs the glow along the risen ship's hull, a colour at a time. */
static void scShipGlowCyclePalette(void) {
	u16 paletteIndex;

	paletteIndex = syMathDivideUnsigned(ShipGlowStep, SHIP_GLOW_STEP_FRAMES);
	syDMAXCopy(&ShipGlowPalettes[paletteIndex], (void *)&BG_PLTT_COLOR(7, 4),
			   DMA_COPY_HALFWORDS(SHIP_GLOW_COLORS), REG_DMA3);
	ShipGlowStep++;
	if(ShipGlowStep > SHIP_GLOW_COLORS * SHIP_GLOW_STEP_FRAMES - 1)
		ShipGlowStep = 0;
}

/* The water drop's burst into a ring of spray (water_drop.png), beyond enum */
/* ACTOR_ANIMATION. */
enum { ANIM_DROP_BURST = 1 };

/* A drop of spray rises and wobbles for its timer, then bursts into a ring */
/* of spray and is released 20 frames later by SprayFallScript. */
static void scLemurianShipSprayUpdate(ACTOR *drop) {
	if(drop->tmr != 0) {
		s32 drift;

		drop->tmr--;
		drift = syRandomNext() - syRandomNext();
		drop->x.fixed += drift;
		drop->y.fixed += toFIXED(4) / 5;
	}
	else if(drop->userValue != 0) {
		drop->userValue = 0;
		fdActorSelectAnimation(drop, ANIM_DROP_BURST);
		drop->wait = 20;
		fdActorStartScript(drop, SprayFallScript);
	}
}

/* Every 40 frames, the Black Orb's glow fades a step, down to 4. */
static void scOrbGlowFade(void) {
	s32 frame = OrbGlowFadeFrames + 1;

	OrbGlowFadeFrames = frame;
	if(frame == 40) {
		if(OrbGlowStrength > 4) {
			OrbGlowStrength -= 1;
			OrbGlowFadeFrames = 0;
		}
	}
}

/* Keeps the raised ship alive with a glow, a swell and spray. */
static void scShipUpdateSwell(void) {
	MAP_GRID_SCROLL_STATE *grid;
	ACTOR *actor;
	FIXED swell;
	s32 x, z;

	grid = MapGridState[0];
	if(OrbGlowActive != 0) {
		/* The blend is a halfword in the frame, written through its address. */
		u16 blend;

		swell = syMulFX(sySin(OrbGlowPhase << 9), 3);
		*&blend = OrbGlowStrength + ((swell + 8) << 8);
		scIoWrite16(blend, &REG_BLDALPHA);
		OrbGlowPhase++;
	}
	if(SwellActive != 0) {
		swell = toFIXED(syMulFX(sySin(SwellPhase << 9), 2));
		grid->layers[MAPGRID_LAYER_MIDDLE].ofsY = ShipHullY + swell;
		grid->layers[MAPGRID_LAYER_OVERHEAD].ofsY = ShipDeckY + swell;
		if(LeaderDeckY != SHIP_NOT_ON_DECK) {
			actor = fdActorGet(ACTOR_PARTY_LEADER);
			actor->y.fixed = LeaderDeckY + swell;
			actor->groundHeight = LeaderDeckY + swell;
			actor->motionFlg = 0;
		}
		if(GaretDeckY != SHIP_NOT_ON_DECK) {
			actor = fdActorGet(ACTOR_GARET);
			actor->y.fixed = GaretDeckY + swell;
			actor->groundHeight = LeaderDeckY + swell;
			actor->motionFlg = 0;
		}
		if(MiaDeckY != SHIP_NOT_ON_DECK) {
			actor = fdActorGet(ACTOR_MIA);
			actor->y.fixed = MiaDeckY + swell;
			actor->groundHeight = LeaderDeckY + swell;
			actor->motionFlg = 0;
		}
		if(IvanDeckY != SHIP_NOT_ON_DECK) {
			actor = fdActorGet(ACTOR_IVAN);
			actor->y.fixed = IvanDeckY + swell;
			actor->groundHeight = LeaderDeckY + swell;
			actor->motionFlg = 0;
		}
		SwellPhase++;
	}
	if(SprayActive != 0 && (FrameCount & 1) != 0) {
		POS sprayPosition;

		x = grid->view[0] & FIELD_PIXEL_MASK;
		z = grid->view[1] & FIELD_PIXEL_MASK;
		x += syRandomNext() * DISPLAY_WIDTH;
		sprayPosition.x = x;
		sprayPosition.y = 0;
		z += syRandomNext() * DISPLAY_HEIGHT;
		z += toFIXED(30);
		sprayPosition.z = z;
		actor = syFieldActorCreate(SPRITE_WATER_DROP, sprayPosition.x,
								   sprayPosition.y, sprayPosition.z);
		if(actor != NULL) {
			actor->update = scLemurianShipSprayUpdate;
			actor->tmr = 60;
			actor->userValue = 1;
			actor->motionFlg = 0;
			actor->priFlg = ACTOR_PRIORITY_UNDERFOOT;
			actor->spr->image.pri = 2;
			fdActorSetSprFlags(actor, ACTOR_BLEND_NORMAL);
			fdActorSelectAnimation(actor, ANIM_STILL);
		}
	}
}

/* Remembers the leader's height on deck, so that from then on he bobs with */
/* the ship. */
static s32 scLemurianShipLeaderStepOnDeck(ACTOR *interpreter) {
	LeaderDeckY = fdActorGet(ACTOR_PARTY_LEADER)->y.fixed;
	return 0;
}

/* Remembers Garet's height on deck, so that from then on he bobs with the */
/* ship. */
static s32 scLemurianShipGaretStepOnDeck(ACTOR *interpreter) {
	GaretDeckY = fdActorGet(ACTOR_GARET)->y.fixed;
	return 0;
}

/* Remembers Mia's height on deck, so that from then on she bobs with the */
/* ship. */
static s32 scLemurianShipMiaStepOnDeck(ACTOR *interpreter) {
	MiaDeckY = fdActorGet(ACTOR_MIA)->y.fixed;
	return 0;
}

/* Remembers Ivan's height on deck, so that from then on he bobs with the */
/* ship. */
static s32 scLemurianShipIvanStepOnDeck(ACTOR *interpreter) {
	IvanDeckY = fdActorGet(ACTOR_IVAN)->y.fixed;
	return 0;
}

/* On the gangway beside the risen Lemurian ship, Garet, Mia and Ivan step out
 */
/* and talk the journey over with Isaac, who answers three questions: whether */
/* the journey ends when they find Jenna, whether to find her before Lemuria, */
/* and whether they will be all right on the rough sea. Each answer picks one */
/* of two replies. Then all four hop up onto the deck, Mia last after a look */
/* back at the shore, and the ship sails away west as black bars close in */
/* from above and below and the ending theme plays, until the screen fades to */
/* the staff credits. */
static void scLemurianShipSetSail(void) {
	MAP_GRID_SCROLL_STATE *grid;
	ACTOR *ship;
	FIELD_STATE *state;

	fdEventStart();
	fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
	fdMapGridCopyCellAttributes(18, 0, 3, 1, 18, 12);
	/* Garet, Mia and Ivan step out around the leader. Garet: "It looks like */
	/* it's finally time to leave Gondowan." Ivan and Mia say how much has */
	/* happened. */
	fdEventSetMessageId(MsgFieldLooksLikeFinally);
	syFieldActorLaunchFromLeader(ACTOR_GARET, -10, 16, FACING_NORTH);
	syFieldActorLaunchFromLeader(ACTOR_MIA, 0, 24, FACING_NORTH);
	fdActorLaunchFromLeader(ACTOR_IVAN, 10, 16, FACING_NORTH);
	fdActorWaitForMoveById(ACTOR_GARET);
	fdEventWait(50);
	fdActorRunRepeatedMotion(ACTOR_GARET, 2);
	fdEventWait(20);
	/* "It looks like it's finally time to leave Gondowan." */
	fdEventShowMessage(ACTOR_GARET, 0);
	fdEventWait(10);
	fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
	fdEventWait(20);
	/* "It's so strange... We've been traveling for so long, and yet..." */
	fdEventShowMessage(ACTOR_IVAN, 0);
	fdEventWait(10);
	fdActorFaceDirection(ACTOR_MIA, FACING_SOUTH, 0);
	fdEventWait(40);
	/* "Now that I think about it, so many things have happened." */
	fdEventShowMessage(ACTOR_MIA, 0);
	fdEventWait(10);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_SOUTH, 0);
	fdEventWait(60);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
	fdActorSetAnimation(ACTOR_GARET, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
	fdEventWait(60);
	/* The first question, Garet's: will the journey be over once they find */
	/* Jenna? The answer picks which of his two replies he says. */
	fdActorRunRepeatedMotion(ACTOR_GARET, 2);
	fdEventWait(30);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTHEAST, 0);
	fdEventWait(20);
	/* "But our journey will be over as soon as we find Jenna, right?" */
	fdEventOpenMessage(ACTOR_GARET, 0);
	if(fdEventAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
		fdEventWait(30);
		fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 50);
		/* "The end of one journey, but the beginning of another? No way!" */
		fdEventShowMessage(ACTOR_GARET, 0);
		FieldStateWork->msg++;
	}
	else {
		fdEventWait(30);
		fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 50);
		FieldStateWork->msg++;
		/* "What did you say? Our next journey? What journey is that?" */
		fdEventShowMessage(ACTOR_GARET, 0);
	}
	fdEventWait(10);
	/* Ivan and Mia remind Garet of their promise to Babi, and Lemuria comes up.
	 */
	fdActorFaceDirection(ACTOR_IVAN, FACING_WEST, 0);
	fdEventWait(30);
	fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 50);
	/* "You didn't forget, did you, Garet?" */
	fdEventShowMessage(ACTOR_IVAN, 0);
	fdEventWait(10);
	fdActorFaceDirection(ACTOR_GARET, FACING_EAST, 0);
	fdEventWait(30);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
	fdActorRunRepeatedMotion(ACTOR_MIA, 2);
	fdEventWait(30);
	fdActorFaceDirection(ACTOR_MIA, FACING_NORTHWEST, 0);
	fdEventWait(30);
	/* "Our promise to Babi." */
	fdEventShowMessage(ACTOR_MIA, 0);
	fdEventWait(10);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 50);
	/* "Oh... Lemuria!" */
	fdEventShowMessage(ACTOR_GARET, 0);
	fdEventWait(10);
	fdActorRunRepeatedMotion(ACTOR_IVAN, 2);
	fdEventWait(20);
	/* "I wonder what kind of place Lemuria is." */
	fdEventShowMessage(ACTOR_IVAN, 0);
	fdEventWait(10);
	fdActorFaceDirection(ACTOR_MIA, FACING_NORTH, 0);
	fdEventWait(20);
	fdActorSetAnimationAndWait(ACTOR_MIA, ANIM_SHAKE_HEAD);
	fdEventWait(20);
	/* The second question, Mia's: must they find Jenna before going to */
	/* Lemuria? Garet answers either way. */
	/* "We have to find Jenna before going to Lemuria, don't we?" */
	fdEventOpenMessage(ACTOR_MIA, 0);
	if(fdEventAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
		fdEventWait(20);
		fdActorRunRepeatedMotion(ACTOR_GARET, 2);
		fdEventWait(20);
		fdActorFaceDirection(ACTOR_GARET, FACING_NORTHEAST, 0);
		fdEventWait(20);
		/* "Yeah. That's important." */
		fdEventShowMessage(ACTOR_GARET, 0);
		FieldStateWork->msg++;
	}
	else {
		fdEventWait(20);
		fdActorRunRepeatedMotion(ACTOR_GARET, 2);
		fdEventWait(20);
		fdActorFaceDirection(ACTOR_GARET, FACING_NORTHEAST, 0);
		fdEventWait(20);
		FieldStateWork->msg++;
		/* "Huh? Jenna's going to be very sad when I tell her what you said!" */
		fdEventShowMessage(ACTOR_GARET, 0);
	}
	fdEventWait(10);
	/* Ivan: their first time on the ocean; Garet: the ocean Kraden wanted */
	/* him to see. */
	fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 50);
	fdEventWait(10);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTHWEST, 0);
	fdEventWait(20);
	fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
	fdEventWait(20);
	/* "At any rate, this will be our first time on the ocean." */
	fdEventShowMessage(ACTOR_IVAN, 0);
	fdEventWait(20);
	/* "So this is the ocean Kraden wanted me to see." */
	fdEventShowMessage(ACTOR_GARET, 0);
	fdEventWait(20);
	fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
	fdEventWait(30);
	fdEventWait(10);
	fdActorSetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
	fdEventWait(20);
	/* The third question, Mia's: the seas are rough; will they be all right? */
	/* "The seas are very rough. Do you think we will be okay?" */
	fdEventOpenMessage(ACTOR_MIA, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_MIA, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTHWEST, 0);
	fdEventWait(20);
	if(fdEventAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
		fdEventWait(30);
		fdActorSetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
		fdEventWait(20);
		/* "This is a Lemurian ship. We'll be fine." */
		fdEventShowMessage(ACTOR_MIA, 0);
		FieldStateWork->msg++;
	}
	else {
		fdEventWait(30);
		fdActorSetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
		fdEventWait(20);
		FieldStateWork->msg++;
		/* "I guess it's hard to say. Still, we must go, just the same..." */
		fdEventShowMessage(ACTOR_MIA, 0);
	}
	fdEventWait(10);
	/* Garet: "Well... Shall we, then?" Ivan: "To sea..." Mia: Jenna is waiting
	 */
	/* for them. The departure theme starts. */
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTH, 0);
	fdEventWait(30);
	/* "Well... Shall we, then?" */
	fdEventShowMessage(ACTOR_GARET, 0);
	fdEventWait(10);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
	fdEventWait(30);
	fdEventWait(10);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
	fdEventWait(30);
	fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
	fdEventWait(20);
	/* "To sea..." */
	fdEventShowMessage(ACTOR_IVAN, 0);
	fdEventWait(10);
	fdActorSetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
	fdEventWait(20);
	/* "Jenna is waiting for us!" */
	fdEventShowMessage(ACTOR_MIA, 0);
	sySndPlay(SOUND_FADE_OUT_BGM);
	fdEventWait(10);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
	fdActorSetAnimation(ACTOR_GARET, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
	fdEventWait(40);
	/* Everyone boards by their own scripts, hopping up onto the deck */
	/* (SE_HIGH_JUMP each); the camera follows up to the ship. */
	fdActorSetSpeed(ACTOR_PARTY_LEADER, 1.2);
	fdActorSetSpeed(ACTOR_GARET, 1.2);
	fdActorSetSpeed(ACTOR_IVAN, 1.2);
	fdActorSetSpeed(ACTOR_MIA, 1.2);
	fdActorSetScript(ACTOR_PARTY_LEADER, LeaderBoardsShip);
	fdEventWait(50);
	fdCameraMoveTo(264, CAMERA_KEEP, 200, 1);
	fdActorSetScript(ACTOR_GARET, GaretBoardsShip);
	fdEventWait(50);
	fdActorSetScript(ACTOR_IVAN, IvanBoardsShip);
	fdActorWaitForScriptEndById(ACTOR_IVAN);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_SOUTH, 0);
	/* Mia, last, looks back at the shore before she boards. */
	fdActorWalkByAndWait(ACTOR_MIA, 0, -32);
	fdEventWait(30);
	fdActorFaceDirection(ACTOR_MIA, FACING_SOUTH, 0);
	fdEventWait(60);
	fdActorFaceDirection(ACTOR_MIA, FACING_WEST, 0);
	fdEventWait(20);
	fdActorSetScript(ACTOR_MIA, MiaBoardsShip);
	fdActorWaitForScriptEndById(ACTOR_MIA);
	fdActorFaceDirection(ACTOR_MIA, FACING_NORTH, 0);
	fdEventWait(20);
	/* On deck, all four turn west together and nod. */
	fdCameraMoveTo(216, CAMERA_KEEP, 168, 1);
	fdCameraWaitForMove();
	fdEventWait(20);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_MIA, FACING_WEST, 0);
	fdEventWait(40);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
	fdActorSetAnimation(ACTOR_GARET, ANIM_NOD);
	fdActorSetAnimation(ACTOR_MIA, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
	fdEventWait(30);
	/* The ship sails west and the curtains close: the sea's layer and the */
	/* gangway's north piece slide away east past it, ever faster, and the
	 * camera */
	/* pans along the deck to the stern. */
	sySndPlay(MUS_THE_BROKEN_SEAL_ENDING);
	sySndSetBgmTempo(SOUND_TEMPO_SCALE_NORMAL * 15 / 16);
	scCurtainStart();
	fdEventWait(LEMURIAN_SHIP_DEPARTURE_DELAY_FRAMES);
	grid = MapGridState[0];
	ship = fdActorGet(ACTOR_GANGWAY_NORTH);
	ship->acceleration = toFIXED(0.002);
	ship->speedLimit = toFIXED(2);
	grid->layers[MAPGRID_LAYER_GROUND].speedX = -toFIXED(3) / 16;
	fdActorWalkByAndWait(ACTOR_GANGWAY_NORTH, 60, 0);
	grid->layers[MAPGRID_LAYER_GROUND].speedX = -toFIXED(3) / 8;
	fdActorWalkByAndWait(ACTOR_GANGWAY_NORTH, 60, 0);
	fdEventWait(80);
	fdEventWait(100);
	fdCameraSetSpeedAndAcceleration(2.2, 0.01);
	fdCameraMoveTo(808, CAMERA_KEEP, 168, 1);
	fdEventWait(300);
	state = FieldStateWork;
	state->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
	scIoWrite16(0, &PLTT_BACKDROP);
	state->transitionFrm = 96;
	fdEventFinishScrTransition();
	fdEventWaitTransitionFrames();
	fdEventWait(30);
	gmFlagSet(FLAG_KEEP_MUSIC_ONCE);
	fdEventSetDestination(SCENE_TITLE, TITLE_FROM_LEMURIAN_SHIP);
}

/* The water's ripple, this scene's view of the line scroll's state */
/* (LineScroll, LINE_SCROLL_STATE). */
/* */
/* It holds two pages of per-line scroll offsets, swapped each frame and */
/* built from a sine table. Here a line is one word, BG3's x and y, where */
/* LINE_SCROLL_STATE has three. */
typedef struct RIPPLE_SCROLL_STATE {
	u16 pages[2][960]; /* per page: 160 lines of BG3 x, y; the rest unused */
	u8 page;		   /* the page the HBlank DMA shows; the other is built */
	u8 mode;
	u16 phase;			/* frames built so far, added to BG3's y */
	u8 unused[4];		/* zeroed with the block; nothing reads or writes it */
	FIXED frequency[2]; /* x, y: 16.16 sine entries per frame and per row */
	FIXED step[2];		/* x, y: 16.16 sine entries a line */
	FIXED amplitude[2]; /* x, y: 16.16 scale on the sine, then divided by 256 */
} RIPPLE_SCROLL_STATE;

/* One line's ripple offset. */
static inline s32 scLemurianShipWaveOffset(s32 angle, FIXED amplitude,
										   u16 base) {
	return (u16)(syMulFX(RippleSine[FIXEDtoINT(angle) &
									(LINE_SCROLL_WAVE_ENTRIES - 1)],
						 amplitude) /
				 LINE_SCROLL_WAVE_SCALE) +
		   base;
}

/* Using the Black Orb at the north end of the gangway (cells (18, 12) to */
/* (20, 14)) raises the Lemurian ship. Isaac walks to the end and faces the */
/* sea; the water glows brighter and brighter, flashing three times, spray */
/* rises all over it and the screen shakes harder, and the ship slowly comes */
/* up out of the sea beside the gangway. A white flash, the puzzle-solved */
/* jingle, and the departure follows (scLemurianShipSetSail). */
static void scLemurianShipRaiseTheShip(void) {
	MAP_GRID_SCROLL_STATE *grid;
	s32 i;
	s32 flash;
	s32 white;
	s32 dark;
	FIXED risen;
	u16 cnt; /* each control, read into the frame by scIoRead16 */

	grid = MapGridState[0];

	fdEventStart();
	fdEndBusyAndReveal();
	/* The leader steps to the water's edge; the backdrop brightens 16 steps. */
	fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 312, 232);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
	fdEventWait(40);
	sySndPlay(SE_SPIN);
	for(i = 0; i <= RGB_LEVEL_MAX / 2; i++) {
		scIoWrite16(RGB(0, i, i * 2), &PLTT_BACKDROP);
		fdEventWait(10);
	}
	scIoWrite16(RGB(0, 16, 31), &PLTT_BACKDROP);
	{
		u16 bright = BLEND_ALPHA(16, 16);

		for(flash = 0; flash < 3; flash++) {
			sySndPlay(SE_POWER_UP);
			REG_BLDALPHA = bright;
			fdEventWait(3);
			scIoWrite16(BLEND_ALPHA(16, 8), &REG_BLDALPHA);
			fdEventWait(65);
		}
	}
	/* The orb's glow and the spray begin, and the water lightens in three */
	/* steps. */
	OrbGlowActive = 1;
	OrbGlowPhase = 0;
	syTaskAddFrame(scShipUpdateSwell, TASK_PASS_FRAME);
	SprayActive = 1;
	fdEventWait(20);
	sySndPlay(SE_RUSH);
	fdMapGridSetShake(1, 1, 1);
	fdEventWait(60);
	SprayActive = 1;
	fdMapGridSetShake(2, 2, 1);
	fdEventWait(60);
	fdMapGridSetShake(3, 3, 1);
	OrbGlowFadeFrames = 0;
	syTaskAddFrame(scOrbGlowFade, TASK_PASS_FRAME);
	/* The ship rises 90 pixels, 0.2 pixels a frame. */
	risen = 0;
	do {
		grid->layers[MAPGRID_LAYER_MIDDLE].ofsY += toFIXED(1) / 5;
		grid->layers[MAPGRID_LAYER_OVERHEAD].ofsY += toFIXED(1) / 5;
		risen += toFIXED(1) / 5;
		sySynch(1);
	} while(risen < toFIXED(90));
	syCloseEvent(scOrbGlowFade);
	SprayActive = 0;
	scIoRead16(&REG_BG3CNT, &cnt);
	cnt = (cnt & ~BGCNT_PRIORITY(3)) | BGCNT_PRIORITY(3);
	REG_BG3CNT = cnt;
	scIoRead16(&REG_BG2CNT, &cnt);
	cnt = (cnt & ~BGCNT_PRIORITY(3)) | BGCNT_PRIORITY(3);
	REG_BG2CNT = cnt;
	scIoRead16(&REG_BG1CNT, &cnt);
	cnt = (cnt & ~BGCNT_PRIORITY(3)) | BGCNT_PRIORITY(2);
	REG_BG1CNT = cnt;
	OrbGlowActive = 0;
	sySndPlay(SE_STOP_ACTION_SE);
	sySynch(1);
	/* A flash of white; the swell starts as it clears. */
	sySndPlay(SE_CRASH);
	scIoWrite16(BLDCNT_TGT1_ALL | BLDCNT_EFFECT_LIGHTEN, &REG_BLDCNT);
	for(white = 0; white <= BLEND_FADE_MAX; white++) {
		REG_BLDY = white;
		fdEventWait(1);
	}
	fdEventWait(40);
	fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
	ShipHullY = grid->layers[MAPGRID_LAYER_MIDDLE].ofsY;
	ShipDeckY = grid->layers[MAPGRID_LAYER_OVERHEAD].ofsY;
	SwellActive = 1;
	for(dark = BLEND_FADE_MAX; dark >= 0; dark--) {
		REG_BLDY = dark;
		fdEventWait(8);
	}
	syTaskAddFrame(scShipGlowCyclePalette, TASK_PASS_FRAME);
	sySndPlay(MUS_PUZZLE_SOLVED);
	mnSndWaitForJingle();
	fdEventWait(20);
	fdEventEnd();
	scLemurianShipSetSail();
}

/* Lays the Lemurian ship under the sea: its two layers are lowered 90 pixels */
/* and drawn behind the sea's, which is blended over them, while the gangway */
/* and the party are drawn above. The sea starts rippling. */
s32 scInit(void) {
	MAP_GRID_SCROLL_STATE *grid;
	u16 cnt; /* each control, read into the frame by scIoRead16 */

	grid = MapGridState[0];
	if(MainRoutineStateWork.entrance == LEMURIAN_SHIP_WITH_BLACK_ORB) {
		gmInventoryAddItem(ACTOR_PARTY_LEADER, ITEM_BLACK_ORB);
	}
	FieldStateWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
	fdActorGet(ACTOR_GANGWAY_NORTH)->collisionFlg = 0;
	fdActorGet(ACTOR_GANGWAY_NORTH)->priFlg = ACTOR_PRIORITY_UNDERFOOT;
	fdActorGet(ACTOR_GANGWAY_SOUTH)->collisionFlg = 0;
	fdActorGet(ACTOR_GANGWAY_SOUTH)->priFlg = ACTOR_PRIORITY_UNDERFOOT;
	{
		ACTOR *actor = fdActorGet(ACTOR_GANGWAY_NORTH);

		SET_IMAGE_PRIORITY(actor, 1);
	}
	{
		ACTOR *actor = fdActorGet(ACTOR_GANGWAY_SOUTH);

		SET_IMAGE_PRIORITY(actor, 1);
	}
	{
		ACTOR *actor = fdActorGet(ACTOR_PARTY_LEADER);

		SET_IMAGE_PRIORITY(actor, 1);
		SET_SHADOW_PRIORITY(actor, 1);
	}
	{
		ACTOR *actor = fdActorGet(ACTOR_GARET);

		SET_IMAGE_PRIORITY(actor, 1);
		SET_SHADOW_PRIORITY(actor, 1);
	}
	{
		ACTOR *actor = fdActorGet(ACTOR_IVAN);

		SET_IMAGE_PRIORITY(actor, 1);
		SET_SHADOW_PRIORITY(actor, 1);
	}
	{
		ACTOR *actor = fdActorGet(ACTOR_MIA);

		SET_IMAGE_PRIORITY(actor, 1);
		SET_SHADOW_PRIORITY(actor, 1);
	}
	scIoRead16(&REG_BG3CNT, &cnt);
	cnt = (cnt & ~BGCNT_PRIORITY(3)) | BGCNT_PRIORITY(2);
	REG_BG3CNT = cnt;
	scIoRead16(&REG_BG2CNT, &cnt);
	cnt = (cnt & ~BGCNT_PRIORITY(3)) | BGCNT_PRIORITY(3);
	REG_BG2CNT = cnt;
	scIoRead16(&REG_BG1CNT, &cnt);
	cnt = (cnt & ~BGCNT_PRIORITY(3)) | BGCNT_PRIORITY(3);
	REG_BG1CNT = cnt;
	scIoWrite16(BLDCNT_TGT1_BG3 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_BG1 |
					BLDCNT_TGT2_BG2 | BLDCNT_TGT2_BD,
				&REG_BLDCNT);
	scIoWrite16(BLEND_ALPHA(16, 8), &REG_BLDALPHA);
	{
		MAP_GRID_LAYER_SCROLL *layer = grid->layers + 1;

		layer->ofsY -= toFIXED(90);
		layer = grid->layers + 2;
		layer->ofsY -= toFIXED(90);
	}
	fdMapGridCenterOnFollowedPoint();
	scRippleStart();
	return 0;
}

/* Arms DMA 0 to copy the shown ripple page into background 3's scroll */
/* registers. */
void fdLineScrollArmDma(void) {
	RIPPLE_SCROLL_STATE *state;
	u32 *source;
	volatile u16 *dma0;
	volatile u32 *destination;

	state = (RIPPLE_SCROLL_STATE *)LineScroll;
	source = (u32 *)state->pages[state->page];

	dma0 = REG_DMA0;
	destination = (volatile u32 *)&REG_BG3HOFS;
	dma0[DMA_CNT_H_INDEX] &= ~DMA_START_TIMING_DRQ_REPEAT;
	dma0[DMA_CNT_H_INDEX] &= ~DMA_ENABLE;
	dma0[DMA_CNT_H_INDEX];

	*destination = *source++;
	syDMAXCopy(source, (void *)destination, DMA_HBLANK_WORD_PER_LINE,
			   (volatile u32 *)dma0);
}

/* Builds the other ripple page and swaps it in. */
void fdLineScrollBuildPage(void) {
	RIPPLE_SCROLL_STATE *state;
	u16 *line;
	s32 angle;
	u16 base;
	s32 amplitude;
	s32 step;
	u16 y;
	s32 i;

	state = (RIPPLE_SCROLL_STATE *)LineScroll;
	y = BgScrollWork[3].y;
	line = state->pages[state->page ^ 1];
	step = state->step[0];
	angle = (state->phase + y) * state->frequency[0];
	amplitude = state->amplitude[0];
	base = BgScrollWork[3].x;
	for(i = 0; i != DISPLAY_HEIGHT; i++) {
		*line = scLemurianShipWaveOffset(angle, amplitude, base);
		angle += step;
		line += 2;
	}
	line = state->pages[state->page ^ 1] + 1;
	step = state->step[1];
	angle = (state->phase + y) * state->frequency[1];
	amplitude = state->amplitude[1];
	for(i = 0; i != DISPLAY_HEIGHT; i++) {
		*line = scLemurianShipWaveOffset(angle, amplitude, y);
		angle += step;
		line += 2;
	}
	state->phase++;
	state->page ^= 1;
}

#include "shared/lemurian_ship_ripple.c"

/* Takes the ripple's tasks off and stops DMA 0. */
static void scRippleStop(void) {
	volatile u16 *reg;

	syCloseEvent(fdLineScrollArmDma);
	syCloseEvent(fdLineScrollBuildPage);
	reg = REG_DMA0;
	reg[DMA_CNT_H_INDEX] = ~DMA_START_TIMING_DRQ_REPEAT & reg[DMA_CNT_H_INDEX];
	reg[DMA_CNT_H_INDEX] = ~DMA_ENABLE & reg[DMA_CNT_H_INDEX];
	reg[DMA_CNT_H_INDEX];
}

/* The curtains' sprites, which close in from above and below as the ship */
/* sails. */
static MENU_SPR sCurtainSprites[3 * CURTAIN_ROW_PARTS];
static s16 sCurtainGap;
static s16 sCurtainVramSlot;

/* Writes one curtain sprite: its queue link, its attributes 0 and 1, and its */
/* attribute 2. */
#ifdef UBFIX
#define CURTAIN_PART(attributes, tile)                                         \
	(part->next = NULL, part->oam.word.attr01 = (attributes),                  \
	 part->oam.word.attr23 = (tile), part++)
#else
/* UB: the parts are written through a word cursor, the link among them. */
#define CURTAIN_PART(attributes, tile)                                         \
	(*word++ = 0, *word++ = (attributes), *word++ = (tile))
#endif

/* Each frame from scCurtainStart to the end of the game: closes the curtains a
 */
/* pixel more, until they rest, and queues their 24 sprites. */
static void scCurtainClose(void) {
#ifdef UBFIX
	MENU_SPR *part;
#else
	u32 *word;
#endif
	u32 tile;
	s32 gap;
	s32 edge;
	u32 i;

	tile = VramCacheEntries[sCurtainVramSlot].ofs >> 5;
#ifdef UBFIX
	part = sCurtainSprites;
#else
	word = (u32 *)sCurtainSprites;
#endif
	if(sCurtainGap != 0)
		sCurtainGap--;
	/* The top row, then the bottom two. */
	for(i = 0; i < CURTAIN_ROW_PARTS; i++) {
		s32 topEdge;

		gap = sCurtainGap;
		topEdge = -gap / 2;
		CURTAIN_PART((topEdge & (OBJ_Y_RANGE - 1)) |
						 OBJ_X(i * CURTAIN_PART_WIDTH) | OBJ_SIZE_32X16,
					 tile);
	}
	edge = (gap / 2 + CURTAIN_BOTTOM_Y) & (OBJ_Y_RANGE - 1);
	for(i = 0; i < CURTAIN_ROW_PARTS; i++)
		CURTAIN_PART(OBJ_X(i * CURTAIN_PART_WIDTH) | edge | OBJ_SIZE_32X16,
					 tile);
	i = 0;
	edge = (sCurtainGap / 2 + CURTAIN_BOTTOM_Y + CURTAIN_PART_HEIGHT) &
		   (OBJ_Y_RANGE - 1);
	for(; i < CURTAIN_ROW_PARTS; i++)
		CURTAIN_PART(OBJ_X(i * CURTAIN_PART_WIDTH) | edge | OBJ_SIZE_32X16,
					 tile);
	for(i = 0; i < 3 * CURTAIN_ROW_PARTS; i++)
		syOamAddSprAtDepth(&sCurtainSprites[i], OAM_DEPTH_FRONT);
}

/* Fills a curtain sprite's tiles with colour 1 and starts the curtains */
/* CURTAIN_START_GAP pixels apart from where they rest. */
static void scCurtainStart(void) {
	u8 *tiles;

	tiles = syHeapCallAlternatePool(
		syHeapAllocEwram, CURTAIN_PART_WIDTH * CURTAIN_PART_HEIGHT / 2);
	sCurtainVramSlot = syVramCacheFindFreeEntry();
	btDmaFill32(PIXELS_4BPP(1), tiles,
				CURTAIN_PART_WIDTH * CURTAIN_PART_HEIGHT / 2);
	syVramCacheLoad(sCurtainVramSlot,
					CURTAIN_PART_WIDTH * CURTAIN_PART_HEIGHT / 2, tiles);

	sCurtainGap = CURTAIN_START_GAP;
	syTaskAddFrame(scCurtainClose, TASK_PASS_FRAME);
}

/* The ship's glow. */
/* */
/* scShipGlowCyclePalette shows six of these colours at a time, from a start */
/* that moves one colour every 6 frames. */
u16 ShipGlowPalettes[] = {
	RGB(0, 3, 22),	RGB(0, 14, 26), RGB(0, 25, 31), RGB(0, 14, 26),
	RGB(0, 3, 22),	RGB(0, 0, 18),	RGB(0, 3, 22),	RGB(0, 14, 26),
	RGB(0, 25, 31), RGB(0, 14, 26), RGB(0, 3, 22),	RGB(0, 0, 0),
};

/* A drop of spray, at the end of its rise: released. */
s32 SprayFallScript[] = {
	SC_RELEASE,
};

/* The ripple's sine, the line scroll's LineScroll_WaveSine kept again in */
/* this overlay. */
s16 RippleSine[LINE_SCROLL_WAVE_ENTRIES] = {LINE_SCROLL_WAVE_SINE};

SCENE_ENTRANCE LemurianShipEntrances[] = {
	{ENTRANCE_DEFAULT, CONDITION_ALWAYS, 408, 0, 200, FACING_NORTH, 0, -1, -1,
	 -1, -1, 0},
	{LEMURIAN_SHIP_FROM_TIMBER_ROOMS_C, CONDITION_ALWAYS, 312, 0, 296,
	 FACING_NORTH, 0, -1, -1, -1, 328, 0},
	{SCENE_TABLE_END},
};

u32 LemurianShipExits[] = {
	SCENE_EXITS(LemurianShip),
	SCENE_EXIT(LEMURIAN_SHIP_TO_TIMBER_ROOMS_C, TimberRoomsC,
			   TIMBER_ROOMS_C_FROM_LEMURIAN_SHIP),
	SCENE_EXITS_END,
};

ACTOR_PLACE LemurianShipActorPlacements[] = {
	ACTOR_PLACEMENT(ACTOR_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0,
					FACING_EAST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(ACTOR_IVAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0,
					FACING_EAST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(ACTOR_MIA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0,
					FACING_EAST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_GANGWAY, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 312,
					0, 248, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_GANGWAY, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 312,
					0, 312, FACING_SOUTH, TALK_FACING_KEEP, 0),
	{SCENE_TABLE_END},
};

TRIGGER LemurianShipTriggers[] = {
	{TRIGGER_STEP, 1, CONDITION_ALWAYS, EVENT(scLemurianShipLeaveTheShip)},
	{PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0,
	 CONDITION_ALWAYS, EVENT(scRippleStopForReveal)},
	{PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0,
	 CONDITION_ALWAYS, EVENT(scRippleStart)},
	{USE_ITEM(ITEM_BLACK_ORB), 10, CONDITION_ALWAYS,
	 EVENT(scLemurianShipRaiseTheShip)},
	{TRIGGER_TABLE_END},
};

/* The Black Orb's glow: whether it shows, its phase in frames, its strength */
/* (BLDALPHA's first weight, a step weaker every 40 frames down to 4) and the */
/* frames into that step. */
s32 OrbGlowActive = 0;
s32 OrbGlowPhase = 0;
s32 OrbGlowStrength = 16;
s32 OrbGlowFadeFrames = 0;

/* Set while spray flies about the rising ship. */
s32 SprayActive = 0;

/* The raised ship's swell: whether it runs, its phase in frames, the hull's */
/* and deck's layer heights it rocks about, and the height on the deck of each
 */
/* of the party (16.16 pixels), SHIP_NOT_ON_DECK for one not on it. */
s32 SwellActive = 0;
s32 SwellPhase = 0;
s32 ShipHullY = 0;
s32 ShipDeckY = 0;
s32 LeaderDeckY = SHIP_NOT_ON_DECK;
s32 GaretDeckY = SHIP_NOT_ON_DECK;
s32 MiaDeckY = SHIP_NOT_ON_DECK;
s32 IvanDeckY = SHIP_NOT_ON_DECK;

/* The frames into the glow's run along the risen ship's hull, */
/* SHIP_GLOW_STEP_FRAMES for each of its SHIP_GLOW_COLORS colours. */
u16 ShipGlowStep = 0;

s32 LeaderBoardsShip[] = {
	SC_ANIMATION(2),
	SC_MOVE_BY(-8, 0, 0),
	SC_SYNC,
	SC_MOVE_BY(0, 0, -16),
	SC_SYNC,
	SC_SET(SW_VELOCITY_Y, toFIXED(8)),
	SC_SE(SE_HIGH_JUMP),
	SC_MOVE_BY(0, 0, -24),
	SC_SYNC,
	SC_ANIMATION(1),
	SC_WAIT(20),
	SC_ANIMATION(2),
	SC_MOVE_BY(-8, 0, 0),
	SC_SYNC,
	SC_MOVE_BY(0, 0, -56),
	SC_SYNC,
	SC_ANIMATION(1),
	SC_SET(SW_FACING, FACING_SOUTH),
	SC_WAIT(1),
	SC_CALL(scLemurianShipLeaderStepOnDeck),
	SC_END,
};

s32 GaretBoardsShip[] = {
	SC_ANIMATION(2),
	SC_MOVE_BY(0, 0, -32),
	SC_SYNC,
	SC_SET(SW_VELOCITY_Y, toFIXED(8)),
	SC_SE(SE_HIGH_JUMP),
	SC_MOVE_BY(0, 0, -24),
	SC_SYNC,
	SC_ANIMATION(1),
	SC_WAIT(20),
	SC_ANIMATION(2),
	SC_MOVE_BY(-8, 0, 0),
	SC_SYNC,
	SC_MOVE_BY(0, 0, -40),
	SC_SYNC,
	SC_ANIMATION(1),
	SC_SET(SW_FACING, FACING_SOUTH),
	SC_WAIT(1),
	SC_CALL(scLemurianShipGaretStepOnDeck),
	SC_END,
};

s32 IvanBoardsShip[] = {
	SC_ANIMATION(2),
	SC_MOVE_BY(-20, 0, 0),
	SC_SYNC,
	SC_MOVE_BY(0, 0, -32),
	SC_SYNC,
	SC_SET(SW_VELOCITY_Y, toFIXED(8)),
	SC_SE(SE_HIGH_JUMP),
	SC_MOVE_BY(0, 0, -24),
	SC_SYNC,
	SC_ANIMATION(1),
	SC_WAIT(20),
	SC_ANIMATION(2),
	SC_MOVE_BY(-8, 0, 0),
	SC_SYNC,
	SC_MOVE_BY(0, 0, -24),
	SC_SYNC,
	SC_ANIMATION(1),
	SC_SET(SW_FACING, FACING_SOUTH),
	SC_WAIT(1),
	SC_CALL(scLemurianShipIvanStepOnDeck),
	SC_END,
};

s32 MiaBoardsShip[] = {
	SC_ANIMATION(2),
	SC_MOVE_BY(-8, 0, 0),
	SC_SYNC,
	SC_MOVE_BY(0, 0, -8),
	SC_SYNC,
	SC_SET(SW_VELOCITY_Y, toFIXED(8)),
	SC_SE(SE_HIGH_JUMP),
	SC_MOVE_BY(0, 0, -24),
	SC_SYNC,
	SC_ANIMATION(1),
	SC_WAIT(20),
	SC_ANIMATION(2),
	SC_MOVE_BY(-8, 0, 0),
	SC_SYNC,
	SC_MOVE_BY(0, 0, -8),
	SC_SYNC,
	SC_ANIMATION(1),
	SC_CALL(scLemurianShipMiaStepOnDeck),
	SC_END,
};
