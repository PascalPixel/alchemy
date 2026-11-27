#ifndef ALCHEMY_FIELD_STATE_H
#define ALCHEMY_FIELD_STATE_H

/* FIELD_STATE, the running scene's state, with its treasure spots and the
   raised cues the field mode handles itself. */

#include "edition.h"
#include "fixed_math.h"
#include "static_assert.h"
#include "types.h"
#include "system/globals.h"

struct ACTOR_PLACE;
struct CUE;
struct ACTOR;
struct MAP_GRID_METATILE;
struct WINDOW;

/* The actor ids FIELD_STATE.actors maps (ACTOR_ID): the party's eight
   characters, then the actors a scene places. */
#define ACTOR_ID_COUNT 66

/* A treasure spot placed as an actor: a chest or a Psynergy Stone. */
typedef struct TREASURE_SPOT {
	struct ACTOR *actor; /* the actor placed for it */
	Uint8		  id; /* the subject of its cue (TREASURE_SPOT_FIRST_ID on) */
	Uint8		  unused; /* cleared with the slot */
	Uint8		  column; /* its 16-pixel metatile */
	Uint8		  row;	  /* its 16-pixel metatile */
} TREASURE_SPOT;

STATIC_ASSERT(sizeof(TREASURE_SPOT) == 8);
STATIC_ASSERT(offsetof(TREASURE_SPOT, id) == 4);
STATIC_ASSERT(offsetof(TREASURE_SPOT, column) == 6);
STATIC_ASSERT(offsetof(TREASURE_SPOT, row) == 7);

/* The values of FIELD_STATE.raisedCue that the field mode handles itself,
   rather than passing them to the scene (fdRunRaisedCue). */
typedef enum FIELD_RAISED_CUE {
	/* Party members fell to poison or hazardous ground this step. */
	FIELD_RAISED_CUE_PARTY_FELL = -1,
	/* R on the world map: the map screen (fdMapScreenShow). */
	FIELD_RAISED_CUE_MAP_SCREEN = -888,
	/* L on the world map: the view zooms out while L is held
	   (fdWorldMapZoomOutWhileLHeld). */
	FIELD_RAISED_CUE_WORLD_MAP_ZOOM = -889
} FIELD_RAISED_CUE;

/* How long a map's screen transition takes, in ticks, unless a script or scene
   sets another length. */
#define MAP_TRANSITION_TICKS 16
/* The ticks Reveal lasts once cast, ten seconds (FIELD_STATE.revealFrame). */
#define REVEAL_TICKS		 600

/* A field Psynergy for FIELD_STATE.psynergy: the unit who uses it, in bits 10
   to 13, and the Psynergy, in bits 0 to 9 (fdUsePsynergy). */
#define FIELD_PSYNERGY(unit, abilityId) ((unit) << 10 | (abilityId))

/* A field scene's state: fdModeRun allocates it in heap slot HEAP_SLOT_FIELD,
   zeroes it and frees it on the way out, and the field mode, the script
   commands and the scene overlays all reach it through FieldStateWork. */
typedef struct FIELD_STATE {
	/* The actor placement tables placed so far, recorded by
	   fdScriptSpawnActorPlacements. */
	struct ACTOR_PLACE *tables[4];
	const struct CUE   *cues; /* the scene's cue table */
	/* The actors by actor id: 0 to 7 the party's characters, 8 on the placed
	   actors. */
	struct ACTOR *actors[ACTOR_ID_COUNT];
	TREASURE_SPOT
	treasureSpots[10]; /* the map's treasure spots (TREASURE_SPOT_MAX) */
	/* The twelve requests: each nonzero value asks the field mode for one
	   thing; fdClearRequests clears them and fdCountRequests sums them. */
	Sint16 stepCue; /* a step cue (1 to 239) for fdRunStepCueOrExit */
	/* a metatile behavior (252 to 254) for fdRunPushMetatile */
	Sint16 pushMetatile;
	Sint16 exit;	  /* the exit taken, which ends the scene */
	Sint16 fieldMenu; /* open the field menu */
	/* The A button: a talk, an examine or the field menu, by what is ahead. */
	Sint16 aButton;
	Sint16 pauseMenu; /* the Start button: open the Pause menu */
	Uint16 talk;	  /* an actor's talk cue, as actor id | 0x1000 */
	/* a metatile's metatile behavior, for fdExamineMetatile */
	Sint16 examineMetatile;
	Sint16 encounter; /* the formation of a random encounter */
	Sint16 psynergy;  /* a field Psynergy to use, as FIELD_PSYNERGY makes it */
	Sint16 item;	  /* an item to use, packed as fdUseItem reads it */
	/* A raised cue's number for fdRunRaisedCue, or an enum */
	/* FIELD_RAISED_CUE. */
	Sint16 raisedCue;
	Sint16 fallenCount;		/* the characters who fell this step */
	Sint16 survivorCount;	/* 0 when the whole party fell */
	Sint16 fallenOwners[9]; /* the characters who fell, from the active party */
	Sint16 itemSlot;		/* the inventory slot fdUseItem receives */
	/* Ticks the leader has pushed against something, counted by the player
	   controls; step cues and push metatiles need more than 12. */
	Sint16 blockedSteps;
	Sint16 mode;			  /* an ACTOR_SYSTEM_MODE */
	Uint8  encounterZones[8]; /* encounter zone for each kind of ground */
	Sint32 encounterBias; /* randomized distance adjustment between battles */
	/* encounter steps before a battle; toFIXED(16) */
	FIXED encounterThreshold;
	/* Scales the leader's speed into walkingDistance, in 16.16 fixed point:
	   toFIXED(1) / 160 on entry, 0 in a town. */
	FIXED walkingDistanceScale;
	/* Distance walked toward the next toFIXED(1), at which the party regains
	   PP, Djinn recover and poison hurts (fdUpdateWalking). */
	FIXED walkingDistance;
	/* the metatile the leader last stepped onto */
	struct MAP_GRID_METATILE *currentMetatile;
	struct MAP_GRID_METATILE *prevMetatile; /* the metatile before it */
	Sint32 transition; /* SCENE_TRANSITION(style, variant); (1, 0) on entry */
	Uint16 unused;
	Uint16 transitionActive; /* set once the opening transition has started */
	/* ticks; MAP_TRANSITION_TICKS on entry and at each script */
	Sint32 transitionFrame;
	/* debug: skip timed script waits and actor messages */
	Sint32 fastForward;
	Uint8  unusedAfterFastForward[8];
	Sint16 msg; /* the next message, advanced as each window opens */
	/* Set to 0xffff by fdScriptStart at each script; nothing reads it. */
	Uint16 unusedAfterMsg;
	/* Both set to -1 by fdScriptStart at each script; nothing reads them. */
	Sint16 unusedBeforeCamera[2];
	/* the actor the map view follows, which has no actor id */
	struct ACTOR *camera;
	Uint8		  unusedAfterCamera[16];
	Sint32		  speaker;	 /* the actor speaking; the leader at each script */
	Sint32		  msgWindow; /* the open message window, 0 for none */
	Sint32		  sideWindow; /* the speaker's portrait window */
	/* The leader's two-row actor placement table (its row and an end row),
	   which fdActorResetForLeader builds through its offset. */
	Uint8 leaderPlacements[48];
	/* The area name notice's window (mnAreaNameWindowShow); it may since have
	   closed. */
	struct WINDOW *areaNameWindow;
	Uint16		   areaNameCountdown; /* ticks left to show the area name */
	/* The palette a field Psynergy's tint fades back to, as a palette fade's
	   target (BG half, then sprite half), saved as the tint starts. */
	Uint16 savedFadeTarget[448 * 3];
	/* Set while a raised or step cue runs; fdScriptStart then resets the effect
	   counter. */
	Uint16 busy;
	Sint16 revealActive;  /* nonzero while Reveal is active */
	Sint16 revealFrame;	  /* the ticks Reveal has left, REVEAL_TICKS as cast */
	Sint16 revealCenterX; /* where Reveal was cast, in pixels */
	Sint16 revealCenterY;
	/* Nonzero lets the Reveal countdown advance; menus and other effects pause
	   it. */
	Sint16 effectTimerRunning;
	/* The first of the two names the next yes/no choice shows in place of Yes
	   and No (mnYesNoMenuRun), 0 for those. */
	Sint16 choiceMsg;
	Sint16 choiceWidth;	   /* the choice window's width, 0 for the default */
	Sint8  psynergyActing; /* set while Psynergy takes effect */
	Sint8  castStarted;	   /* set once a cast has been announced */
	Sint16 queuedSound;	   /* a sound fdPlayQueuedMusic plays; -1 for none */
	/* Set while the field menu is open (mnFieldMenuRun), which pauses Reveal's
	   sparkles; only the German, Spanish, French and Italian editions keep
	   it. */
	Sint8 menuOpen;
} FIELD_STATE;

STATIC_ASSERT(sizeof(FIELD_STATE) == 3276);
STATIC_ASSERT(offsetof(FIELD_STATE, tables) == 0);
STATIC_ASSERT(offsetof(FIELD_STATE, cues) == 16);
STATIC_ASSERT(offsetof(FIELD_STATE, actors) == 20);
STATIC_ASSERT(offsetof(FIELD_STATE, treasureSpots) == 284);
STATIC_ASSERT(offsetof(FIELD_STATE, stepCue) == 364);
STATIC_ASSERT(offsetof(FIELD_STATE, talk) == 376);
STATIC_ASSERT(offsetof(FIELD_STATE, encounter) == 380);
STATIC_ASSERT(offsetof(FIELD_STATE, raisedCue) == 386);
STATIC_ASSERT(offsetof(FIELD_STATE, blockedSteps) == 412);
STATIC_ASSERT(offsetof(FIELD_STATE, mode) == 414);
STATIC_ASSERT(offsetof(FIELD_STATE, encounterZones) == 416);
STATIC_ASSERT(offsetof(FIELD_STATE, encounterBias) == 424);
STATIC_ASSERT(offsetof(FIELD_STATE, encounterThreshold) == 428);
STATIC_ASSERT(offsetof(FIELD_STATE, walkingDistanceScale) == 432);
STATIC_ASSERT(offsetof(FIELD_STATE, walkingDistance) == 436);
STATIC_ASSERT(offsetof(FIELD_STATE, currentMetatile) == 440);
STATIC_ASSERT(offsetof(FIELD_STATE, prevMetatile) == 444);
STATIC_ASSERT(offsetof(FIELD_STATE, transition) == 448);
STATIC_ASSERT(offsetof(FIELD_STATE, transitionActive) == 454);
STATIC_ASSERT(offsetof(FIELD_STATE, transitionFrame) == 456);
STATIC_ASSERT(offsetof(FIELD_STATE, fastForward) == 460);
STATIC_ASSERT(offsetof(FIELD_STATE, msg) == 472);
STATIC_ASSERT(offsetof(FIELD_STATE, camera) == 480);
STATIC_ASSERT(offsetof(FIELD_STATE, unusedAfterMsg) == 474);
STATIC_ASSERT(offsetof(FIELD_STATE, unusedBeforeCamera) == 476);
STATIC_ASSERT(offsetof(FIELD_STATE, speaker) == 500);
STATIC_ASSERT(offsetof(FIELD_STATE, msgWindow) == 504);
STATIC_ASSERT(offsetof(FIELD_STATE, sideWindow) == 508);
STATIC_ASSERT(offsetof(FIELD_STATE, areaNameWindow) == 560);
STATIC_ASSERT(offsetof(FIELD_STATE, savedFadeTarget) == 566);
STATIC_ASSERT(offsetof(FIELD_STATE, busy) == 3254);
STATIC_ASSERT(offsetof(FIELD_STATE, revealActive) == 3256);
STATIC_ASSERT(offsetof(FIELD_STATE, revealFrame) == 3258);
STATIC_ASSERT(offsetof(FIELD_STATE, revealCenterX) == 3260);
STATIC_ASSERT(offsetof(FIELD_STATE, revealCenterY) == 3262);
STATIC_ASSERT(offsetof(FIELD_STATE, effectTimerRunning) == 3264);
STATIC_ASSERT(offsetof(FIELD_STATE, choiceMsg) == 3266);
STATIC_ASSERT(offsetof(FIELD_STATE, choiceWidth) == 3268);
STATIC_ASSERT(offsetof(FIELD_STATE, psynergyActing) == 3270);
STATIC_ASSERT(offsetof(FIELD_STATE, castStarted) == 3271);
STATIC_ASSERT(offsetof(FIELD_STATE, queuedSound) == 3272);

/* The running field scene's state, heap slot HEAP_SLOT_FIELD; fdModeRun takes
   it and frees it. */
#define FieldStateWork (*(FIELD_STATE **)&HeapSlots[HEAP_SLOT_FIELD])

#endif
