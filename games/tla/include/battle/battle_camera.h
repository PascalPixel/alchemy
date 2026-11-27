#ifndef ALCHEMY_BATTLE_CAMERA_H
#define ALCHEMY_BATTLE_CAMERA_H

/* @file */
/* The battle camera, the battle background's modes and scanline state, and the
 */
/* presentation state an action's staging hands its battle effect. */
/* The Broken Seal's presentation state holds eight targets; this one nine. */

#include "battle/battle_action.h"

#include "fixed_math.h"
#include "types.h"
#include "static_assert.h"
#include "edition.h"
#include "system.h"

/* The battle camera: its eye, the focus it turns about, and its pitch, yaw */
/* and distance. */
typedef struct BATTLE_CAMERA {
	/* The eye, 16.16 fixed-point pixels: written distance behind the focus */
	/* along the turned view. */
	FIXED eye[3];
	FIXED focus[3]; /* the focus, 16.16 fixed-point pixels */
	s32 followEye;	/* cleared beside followPos; nothing reads it */
	s32 *followPos; /* when set, followed instead of focus */

	FIXED distance;				/* eye to focus, 16.16 fixed-point pixels */
	u8 unusedAfterDistance[16]; /* no code names it */
	s16 pitch;					/* 1/65536 turns */
	s16 yaw;					/* 1/65536 turns */
	u8 unusedAfterYaw[20];		/* no code names it */
} BATTLE_CAMERA;

STATIC_ASSERT(sizeof(BATTLE_CAMERA) == 76);
STATIC_ASSERT(offsetof(BATTLE_CAMERA, focus) == 12);
STATIC_ASSERT(offsetof(BATTLE_CAMERA, followPos) == 28);
STATIC_ASSERT(offsetof(BATTLE_CAMERA, pitch) == 52);
STATIC_ASSERT(offsetof(BATTLE_CAMERA, yaw) == 54);

/* The HEAP_SLOT_BATTLE_BACKGROUND heap slot: the yaw the battle camera eases */
/* toward and the frames left to ease it, and how the battle background is */
/* shown. */
typedef struct BATTLE_BACKGROUND_STATE {
	s32 tgtYaw; /* 1/65536 turns */
	s32 frames; /* frames left to ease the yaw, a sixteenth a frame */
	s32 mode;	/* BATTLE_BACKGROUND_MODE */
	/* BATTLE_BACKGROUND_FIXED also sets BG1's scroll whatever the mode; the */
	/* battle effects' canvas layers set it. */
	s32 bg1ScrollMode;
	s32 busy; /* while set, an effect owns BG1's scroll and the camera leaves it
			   */
	/* 0 lets the camera update set the background's range each frame. */
	s32 keepRange;
	s32 unusedAfterKeepRange[2]; /* cleared when a battle starts; nothing reads
								  */
								 /* them */
} BATTLE_BACKGROUND_STATE;

/* A presentation's effect command, laid out as BATTLE_EFFECT_PARM with a */
/* ninth actor, and the animation state it keeps. */
typedef struct BATTLE_PRESENTATION_STATE {
	s32 kind;		 /* the effect's index in the handler table */
	s32 side;		 /* 1 when the targets are the party, 0 for enemies */
	s32 actor;		 /* the user's unit id */
	s32 firstTgt;	 /* the action's first target */
	s32 range;		 /* as BATTLE_EFFECT_PARM's: the action's, or 1 */
	s32 cnt;		 /* targets in actors */
	s32 variant;	 /* the ability's size, from 0 */
	s32 prelude;	 /* as BATTLE_EFFECT_PARM's: 1 plays a prelude */
	u8 unused[4];	 /* no code names it */
	s16 actors[9];	 /* the targets' unit ids, count of them */
	u8 params[8][4]; /* each target's animation entries' param, kept */
} BATTLE_PRESENTATION_STATE;

STATIC_ASSERT(sizeof(BATTLE_BACKGROUND_STATE) == 32);
STATIC_ASSERT(offsetof(BATTLE_BACKGROUND_STATE, mode) == 8);
STATIC_ASSERT(offsetof(BATTLE_BACKGROUND_STATE, busy) == 16);
STATIC_ASSERT(sizeof(BATTLE_PRESENTATION_STATE) == 88);
STATIC_ASSERT(offsetof(BATTLE_PRESENTATION_STATE, actors) == 36);
STATIC_ASSERT(offsetof(BATTLE_PRESENTATION_STATE, params) == 54);

/* BATTLE_BACKGROUND_STATE.mode, how the battle background is shown, and its */
/* bg1ScrollMode. */
typedef enum BATTLE_BACKGROUND_MODE {
	/* Not loaded yet. */
	BATTLE_BACKGROUND_NONE,
	/* The picture at a fixed size on BG1, its scroll following the camera, BG2
	 */
	/* left to a battle effect. */
	BATTLE_BACKGROUND_FIXED,
	/* The picture zoomed and panned on BG2, an affine layer; HBlank DMA sets */
	/* each line's BG2CNT to the half of the picture it reads. */
	BATTLE_BACKGROUND_ZOOMED
} BATTLE_BACKGROUND_MODE;

/* btBackgroundSetMode's fade that restores the saved palette instead of */
/* taking from each colour channel. */
#define BATTLE_BACKGROUND_FADE_RESTORE 128

/* The battle background's affine parameters and reference point, copied */
/* whole to the BG2PA registers. */
typedef struct BATTLE_AFFINE {
	u16 pa, pb, pc, pd;
	s32 x, y;
} BATTLE_AFFINE;

/* The battle background's scanline state: two pages of per-line values, */
/* swapped each frame, and the affine parameters. */
typedef struct BATTLE_AFFINE_HDMA {
	u32 page;	   /* the page of lines shown */
	u8 unused[12]; /* no code names it */
	BATTLE_AFFINE affine;
	u16 lines[2][160];
} BATTLE_AFFINE_HDMA;

STATIC_ASSERT(sizeof(BATTLE_AFFINE_HDMA) == 672);
STATIC_ASSERT(offsetof(BATTLE_AFFINE_HDMA, affine) == 16);
STATIC_ASSERT(offsetof(BATTLE_AFFINE_HDMA, lines) == 32);

struct BATTLE_PLAN;
UNREAD_RESULT btStagingRunMessages(struct BATTLE_PLAN *plan, s32 unused);

void btStagingBuildTargetList(struct BATTLE_PLAN *plan,
							  BATTLE_PRESENTATION_STATE *state);

void btCameraSetRange(FIXED cx, FIXED cy, s32 ox, s32 oy, FIXED scale);

s32 btObjectIsValidId(u32 objectId);
void btHighlightSetSprMode(s32 actor, s32 mode);
/* Runs one of the large battle effects, the summons and the scenes of a few */
/* abilities. */
void btEffectRunSummonStub(s32 *context);
/* Copies the canvas to VRAM when a transfer is pending. */
void btEffectTransferCanvas(void);
void btEffectArmBg2AffineHBlankDma(void);
/* Steps the background palette fade. */
void btEffectDarkenBackgroundStep(void);
struct BATTLE_EFFECT_PARM;

/* The battle camera's distance from its focus in its default view, and the */
/* unzoomed projection's focal length, in 16.16 pixels (btCameraSetView). */
#define BATTLE_CAMERA_DISTANCE toFIXED(510)

#endif
