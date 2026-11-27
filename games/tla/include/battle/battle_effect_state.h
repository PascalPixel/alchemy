#ifndef ALCHEMY_BATTLE_EFFECT_STATE_H
#define ALCHEMY_BATTLE_EFFECT_STATE_H

/* The Lost Age's copy of the battle effect state. The Broken Seal's block with
   the 64 particles in front of the sprite sheet, the two affine scales after
   the BG2 reference point, 64 object slots and one more word before the
   effect. */

#include "field/particle.h"
#include "battle/battle_effect_params.h"
#include "static_assert.h"
#include "system.h"
#include "types.h"

/* How the canvas transfer treats the canvas after each copy (a battle effect's
   BATTLE_EFFECT_STATE.transferMode, Lucky Wheels' REEL_EFFECT_STATE's);
   transferValue is its argument. */
typedef enum CANVAS_TRANSFER_MODE {
	CANVAS_COPY, /* copy as drawn */
	/* copy, then fill the canvas with transferValue (0 clears it) */
	CANVAS_COPY_AND_CLEAR,
	/* copy, keeping half of each pixel's level for the next frame when
	   transferValue is CANVAS_FADE_HALF, and three quarters for any other
	   value */
	CANVAS_COPY_AND_FADE,
	/* copy, then lower each level by a byte of transferValue, stopping at 0 */
	CANVAS_COPY_AND_DARKEN,
	/* copy, then raise each level by a byte of transferValue */
	CANVAS_COPY_AND_BRIGHTEN
} CANVAS_TRANSFER_MODE;

/* CANVAS_COPY_AND_FADE's transferValue: the percentage of each pixel's level
   kept for the next frame. */
typedef enum CANVAS_FADE_SHARE {
	CANVAS_FADE_HALF = 50,
	CANVAS_FADE_THREE_QUARTERS = 75
} CANVAS_FADE_SHARE;

/* The running battle effect's state: its canvas image, 64 particles, the
   display registers it stages and its sprites. It lives in the
   HEAP_SLOT_BATTLE_EFFECT heap slot. */
typedef struct BATTLE_EFFECT_STATE {
	PARTICLE particles[64];
	Uint8	 sheet[27008]; /* the effect's decoded pictures, at fixed offsets */
	Sint32	 bg2xLines[160]; /* BG2X for each scanline, for an HBlank DMA */
	/* The rest of the pictures' storage, which no code names. */
	Uint8 imageTail[832];
	/* The backdrop's 160 colours, one a line, that btEffectArmPaletteHBlankDMA
	   streams into colour 0. */
	Uint16 backdropLines[160];
	/* How the canvas transfer callback treats the canvas after each copy, an
	   CANVAS_TRANSFER_MODE. */
	Sint32 transferMode;
	/* The transfer mode's argument: the fill value, the share kept (an enum
	   CANVAS_FADE_SHARE) or the darken or brighten amount. */
	Sint32 transferValue;
	Sint32 unused;		   /* never read or written in either game */
	Sint32 frm;			   /* frame counter some effects keep, from 0 */
	Sint32 scrollTmr;	   /* frames since the last interval scroll step */
	Sint32 scrollInterval; /* frames between interval scroll steps */
	Sint32 scrollStepX;	   /* added to BG1 or BG2 x at each step */
	Sint32 scrollStepY;	   /* added to BG1 or BG2 y at each step */
	Sint32 savedBg1X;	   /* BG1 scroll restored when a shake ends */
	Sint32 savedBg1Y;	   /* BG1 scroll restored when a shake ends */
	Sint32 shakeFrm;	/* shake frames left; Camera_ApplyShake counts down */
	Sint32 cameraDelta; /* camera yaw turned per effect frame */
	/* The effect sets 1 after a frame; btCameraApplyPhasedDelta then turns the
	   whole cameraDelta and sets 0, else turns half and toggles 0 and 2. */
	Sint32 cameraPhase;
	Sint32 fadeFrm; /* backdrop fade frames left */
	/* (btEffectDarkenBackgroundStep) */
	Sint32 fadeStep; /* steps the backdrop has darkened so far */
	/* Window, display and blend registers, which
	   btEffectApplyWindowBlendRegisters writes all at once. */
	Uint16 win0h;
	Uint16 win0v;
	Uint16 win1h;
	Uint16 win1v;
	Uint16 winin;
	Uint16 winout;
	Uint16 dispcnt;
	Uint16 bldcnt;
	Uint16 bldalpha;
	Uint16 padding; /* aligns bg2x */
	/* BG2's reference point, written to BG2X and BG2Y by
	   btEffectApplyBg2Reference and btDispApplyBg2Affine. Volatile, as the
	   scale below. */
	volatile Sint32 bg2x;
	volatile Sint32 bg2y;
	/* BG2's scale, written to BG2PA and BG2PD by btDispApplyBg2Affine.
	   Volatile: btDispApplyBg2Affine copies them out from the VBlank handler
	   (EVENT_PASS_VBLANK). */
	volatile Uint16 bg2pa;
	volatile Uint16 bg2pd;
	void		   *sprs[64]; /* the effect's sprites */
	/* Each target's flash timer in frames, for args->tgts[n]: set by
	   btEffectTintBattler, counted down by btEffectCountDownBattlerTints. */
	Uint8 actorTmrs[8];
	/* 1 on a canvas copy's VBlank, then counts up. Volatile and unsigned: the
	   VBlank handler counts it while an effect waits on it, each wait reads it
	   afresh and compares it unsigned. */
	volatile Uint32 vblanksSinceTransfer;
	/* 1 once the effect has drawn a frame; the canvas transfer callback copies
	   the canvas and clears it. */
	Sint32 transferPending;
	/* one draft sets 1 for a frame range; no reader found */
	Sint32				stampPending;
	BATTLE_EFFECT_PARM *args; /* the command being run */
} BATTLE_EFFECT_STATE;

STATIC_ASSERT(offsetof(BATTLE_EFFECT_STATE, sheet) == 1792);
STATIC_ASSERT(offsetof(BATTLE_EFFECT_STATE, bg2xLines) == 28800);
STATIC_ASSERT(offsetof(BATTLE_EFFECT_STATE, backdropLines) == 30272);
STATIC_ASSERT(offsetof(BATTLE_EFFECT_STATE, transferMode) == 30592);
STATIC_ASSERT(offsetof(BATTLE_EFFECT_STATE, shakeFrm) == 30632);
STATIC_ASSERT(offsetof(BATTLE_EFFECT_STATE, fadeStep) == 30648);
STATIC_ASSERT(offsetof(BATTLE_EFFECT_STATE, win0h) == 30652);
STATIC_ASSERT(offsetof(BATTLE_EFFECT_STATE, bg2x) == 30672);
STATIC_ASSERT(offsetof(BATTLE_EFFECT_STATE, bg2pa) == 30680);
STATIC_ASSERT(offsetof(BATTLE_EFFECT_STATE, sprs) == 30684);
STATIC_ASSERT(offsetof(BATTLE_EFFECT_STATE, actorTmrs) == 30940);
STATIC_ASSERT(offsetof(BATTLE_EFFECT_STATE, transferPending) == 30952);
STATIC_ASSERT(offsetof(BATTLE_EFFECT_STATE, args) == 30960);

/* The battle canvas the effects draw into: 128 by 128 pixels, a byte each. */
#define CANVAS_BYTES			(128 * 128)
/* A word of four canvas pixels at one level, for filling the canvas. */
#define CANVAS_FILL_WORD(level) PIXELS_8BPP(level)

#endif
