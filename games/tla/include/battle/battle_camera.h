#ifndef ALCHEMY_BATTLE_CAMERA_H
#define ALCHEMY_BATTLE_CAMERA_H

/* The battle camera, the battle background's modes and scanline state, and the
   presentation state an action's staging hands its battle effect. The Broken
   Seal's presentation state holds eight targets; this one nine. */

#include "battle/battle_action.h"

#include "fixed_math.h"
#include "types.h"
#include "static_assert.h"
#include "edition.h"
#include "system.h"

/* The battle camera: its eye, the focus it turns about, and its pitch, yaw and
   distance. */
typedef struct BATTLE_CAMERA {
	/* The eye, 16.16 fixed-point pixels: written distance behind the focus
	   along the turned view. */
	FIXED	eye[3];
	FIXED	focus[3];  /* the focus, 16.16 fixed-point pixels */
	Sint32	followEye; /* cleared beside followPos; nothing reads it */
	Sint32 *followPos; /* when set, followed instead of focus */

	FIXED  distance;				/* eye to focus, 16.16 fixed-point pixels */
	Uint8  unusedAfterDistance[16]; /* no code names it */
	Sint16 pitch;					/* 1/65536 turns */
	Sint16 yaw;						/* 1/65536 turns */
	Uint8  unusedAfterYaw[20];		/* no code names it */
} BATTLE_CAMERA;

STATIC_ASSERT(sizeof(BATTLE_CAMERA) == 76);
STATIC_ASSERT(offsetof(BATTLE_CAMERA, focus) == 12);
STATIC_ASSERT(offsetof(BATTLE_CAMERA, followPos) == 28);
STATIC_ASSERT(offsetof(BATTLE_CAMERA, pitch) == 52);
STATIC_ASSERT(offsetof(BATTLE_CAMERA, yaw) == 54);

/* The HEAP_SLOT_BATTLE_BACKGROUND heap slot: the yaw the battle camera eases
   toward and the ticks left to ease it, and how the battle background is
   shown. */
typedef struct BATTLE_BACKGROUND_STATE {
	Sint32 tgtYaw; /* 1/65536 turns */
	Sint32 ticks;  /* ticks left to ease the yaw, a sixteenth a tick */
	Sint32 mode;   /* BATTLE_BACKGROUND_MODE */
	/* BATTLE_BACKGROUND_FIXED also sets BG1's scroll whatever the mode; the
	   battle effects' canvas layers set it. */
	Sint32 bg1ScrollMode;
	/* while set, an effect owns BG1's scroll and the camera leaves it */
	Sint32 busy;
	/* 0 lets the camera update set the background's range each tick. */
	Sint32 keepRange;
	/* cleared when a battle starts; nothing reads them */
	Sint32 unusedAfterKeepRange[2];
} BATTLE_BACKGROUND_STATE;

/* A presentation's effect command, laid out as BATTLE_EFFECT_PARM with a ninth
   actor, and the animation state it keeps. */
typedef struct BATTLE_PRESENTATION_STATE {
	Sint32 kind;		 /* the effect's index in the handler table */
	Sint32 side;		 /* 1 when the targets are the party, 0 for enemies */
	Sint32 actor;		 /* the user's unit id */
	Sint32 firstTgt;	 /* the action's first target */
	Sint32 range;		 /* as BATTLE_EFFECT_PARM's: the action's, or 1 */
	Sint32 cnt;			 /* targets in actors */
	Sint32 variant;		 /* the ability's size, from 0 */
	Sint32 prelude;		 /* as BATTLE_EFFECT_PARM's: 1 plays a prelude */
	Uint8  unused[4];	 /* no code names it */
	Sint16 actors[9];	 /* the targets' unit ids, count of them */
	Uint8  params[8][4]; /* each target's animation entries' param, kept */
} BATTLE_PRESENTATION_STATE;

STATIC_ASSERT(sizeof(BATTLE_BACKGROUND_STATE) == 32);
STATIC_ASSERT(offsetof(BATTLE_BACKGROUND_STATE, mode) == 8);
STATIC_ASSERT(offsetof(BATTLE_BACKGROUND_STATE, busy) == 16);
STATIC_ASSERT(sizeof(BATTLE_PRESENTATION_STATE) == 88);
STATIC_ASSERT(offsetof(BATTLE_PRESENTATION_STATE, actors) == 36);
STATIC_ASSERT(offsetof(BATTLE_PRESENTATION_STATE, params) == 54);

/* BATTLE_BACKGROUND_STATE.mode, how the battle background is shown, and its
   bg1ScrollMode. */
typedef enum BATTLE_BACKGROUND_MODE {
	/* Not loaded yet. */
	BATTLE_BACKGROUND_NONE,
	/* The picture at a fixed size on BG1, its scroll following the camera, BG2
	   left to a battle effect. */
	BATTLE_BACKGROUND_FIXED,
	/* The picture zoomed and panned on BG2, an affine layer; HBlank DMA sets
	   each line's BG2CNT to the half of the picture it reads. */
	BATTLE_BACKGROUND_ZOOMED
} BATTLE_BACKGROUND_MODE;

/* btBackgroundSetMode's fade that restores the saved palette instead of taking
   from each colour channel. */
#define BATTLE_BACKGROUND_FADE_RESTORE 128

/* The battle background's affine parameters and reference point, copied whole
   to the BG2PA registers. */
typedef struct BATTLE_AFFINE {
	Uint16 pa, pb, pc, pd;
	Sint32 x, y;
} BATTLE_AFFINE;

/* The battle background's scanline state: two pages of per-line values, swapped
   each tick, and the affine parameters. */
typedef struct BATTLE_AFFINE_HDMA {
	Uint32		  page;		  /* the page of lines shown */
	Uint8		  unused[12]; /* no code names it */
	BATTLE_AFFINE affine;
	Uint16		  lines[2][160];
} BATTLE_AFFINE_HDMA;

STATIC_ASSERT(sizeof(BATTLE_AFFINE_HDMA) == 672);
STATIC_ASSERT(offsetof(BATTLE_AFFINE_HDMA, affine) == 16);
STATIC_ASSERT(offsetof(BATTLE_AFFINE_HDMA, lines) == 32);

struct BATTLE_PLAN;
UNREAD_RESULT btStagingRunMessages(struct BATTLE_PLAN *plan, Sint32 unused);

void btStagingBuildTargetList(
	struct BATTLE_PLAN *plan, BATTLE_PRESENTATION_STATE *state);

void btCameraSetRange(FIXED cx, FIXED cy, Sint32 ox, Sint32 oy, FIXED scale);

Sint32 btObjectIsValidId(Uint32 objectId);
void   btHighlightSetSprMode(Sint32 actor, Sint32 mode);
/* Runs one of the large battle effects, the summons and the scenes of a few
   abilities. */
void btEffectRunSummonStub(Sint32 *context);
/* Copies the canvas to VRAM when a transfer is pending. */
void btEffectTransferCanvas(void);
void btEffectArmBg2AffineHBlankDMA(void);
/* Steps the background palette fade. */
void btEffectDarkenBackgroundStep(void);
struct BATTLE_EFFECT_PARM;

/* The battle camera's distance from its focus in its default view, and the
   unzoomed projection's focal length, in 16.16 pixels (btCameraSetView). */
#define BATTLE_CAMERA_DISTANCE toFIXED(510)

#endif
