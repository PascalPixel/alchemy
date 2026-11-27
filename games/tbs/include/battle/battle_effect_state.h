#ifndef ALCHEMY_BATTLE_EFFECT_STATE_H
#define ALCHEMY_BATTLE_EFFECT_STATE_H

/* BATTLE_EFFECT_STATE, the running battle effect's state, and its sprites. */

#include "static_assert.h"
#include "types.h"
#include "battle/battle_effect_params.h"
#include "field/particle.h"
#include "system.h"

struct SPR;

/* Two adjacent affine background coordinates, queued together for BG2X/BG2Y. */
typedef struct BATTLE_BG2_ORIGIN {
	Sint32 x; /* BG2X, 8 fractional bits */
	Sint32 y; /* BG2Y, 8 fractional bits */
	/* 1 once the coordinates are queued, 0 before the first; nothing in the
	   game reads it, and the queue copies only x and y. */
	Sint32 queued;
} BATTLE_BG2_ORIGIN;

STATIC_ASSERT(sizeof(BATTLE_BG2_ORIGIN) == 12);
STATIC_ASSERT(offsetof(BATTLE_BG2_ORIGIN, y) == 4);

/* How btEffectTransferCanvas moves the effect's 128x128 canvas into VRAM once a
   tick (state->transferMode); the canvas pixels are indices into a palette
   ramp, so scaling them fades a trail. For the clear, darken and brighten modes
   transferValue is a word, a fill or a step repeated in each of its four bytes
   (one per pixel, CANVAS_FILL_WORD); the fade mode only compares it with
   CANVAS_FADE_HALF. */
typedef enum CANVAS_TRANSFER_MODE {
	CANVAS_COPY, /* copy as drawn */
	/* copy, then fill the canvas with transferValue (0 clears it) */
	CANVAS_COPY_AND_CLEAR,
	/* copy, keeping half of each pixel's level for the next tick when
	   transferValue is CANVAS_FADE_HALF, and three quarters for any other value
	   (CANVAS_FADE_THREE_QUARTERS by convention) */
	CANVAS_COPY_AND_FADE,
	/* copy, then lower each level by a byte of transferValue, stopping at 0 */
	CANVAS_COPY_AND_DARKEN,
	/* copy, then raise each level by a byte of transferValue, stopping at
	   CANVAS_LEVEL_BRIGHTEST; no effect uses it */
	CANVAS_COPY_AND_BRIGHTEN
} CANVAS_TRANSFER_MODE;

/* CANVAS_COPY_AND_FADE's transferValue: the percentage of each pixel's level
   kept for the next tick. */
typedef enum CANVAS_FADE_SHARE {
	CANVAS_FADE_HALF = 50,
	CANVAS_FADE_THREE_QUARTERS = 75
} CANVAS_FADE_SHARE;

/* A canvas word holding `level` in each of its four bytes: a fill for
   CANVAS_COPY_AND_CLEAR or a step for CANVAS_COPY_AND_DARKEN, one byte per
   pixel. */
#define CANVAS_FILL_WORD(level) PIXELS_8BPP(level)

/* The brightest canvas level: the additive blitters and
   CANVAS_COPY_AND_BRIGHTEN stop at it. */
#define CANVAS_LEVEL_BRIGHTEST 63

/* The running battle effect's state: its canvas image, 64 particles, the
   display registers it stages and its sprites. It lives in the
   HEAP_SLOT_BATTLE_EFFECT heap slot. */
typedef struct BATTLE_EFFECT_STATE {
	/* Images and scanline tables reuse the storage before the particles. The
	   tile-effect mask extends beyond sprite sheet into the table area. */
	union {
		struct {
			/* the effect's decoded pictures, at fixed offsets */
			Uint8 sheet[27008];
			/* BG2X for each scanline, for an HBlank DMA */
			Sint32 bg2xLines[160];
			/* The rest of imageStorage, which only effects that skip the
			   scanline table reach through it. */
			Uint8 imageTail[1152];
		};

		Uint8 imageStorage[28800];

		/* The tracked elemental effect stages its origin inside the image
		   area. */
		struct {
			Uint8			  image[5060];
			BATTLE_BG2_ORIGIN origin;
		} trackedCanvas;
	};

	PARTICLE particles[64]; /* free while variant is -1 */
	/* How the canvas transfer events treat the canvas after each copy
	   (CANVAS_TRANSFER_MODE in battle_effect_internal.h). */
	Sint32 transferMode; /* an CANVAS_TRANSFER_MODE */
	/* The transfer mode's argument: the fill value, the share kept (50 or 75
	   percent) or the darken or brighten amount. */
	Sint32 transferValue;
	Sint32 unused;		   /* never read or written in either game */
	Sint32 frame;		   /* tick counter some effects keep, from 0 */
	Sint32 scrollTimer;	   /* ticks since the last interval scroll step */
	Sint32 scrollInterval; /* ticks between interval scroll steps */
	Sint32 scrollStepX;	   /* added to BG1 or BG2 x at each step */
	Sint32 scrollStepY;	   /* added to BG1 or BG2 y at each step */
	Sint32 savedBg1X;	   /* BG1 scroll restored when a shake ends */
	Sint32 savedBg1Y;	   /* BG1 scroll restored when a shake ends */
	Sint32 shakeFrame;	   /* shake ticks left; btCameraShake counts down */
	Sint32 cameraDelta;	   /* camera yaw turned per tick of the effect */
	/* The effect sets 1 after a tick; btCameraApplyPhasedDelta then turns the
	   whole cameraDelta and sets 0, else turns half and toggles 0 and 2. */
	Sint32 cameraPhase;
	Sint32 fadeFrame; /* battle background fade ticks left */
	/* (btEffectDarkenBackgroundStep) */
	Sint32 fadeStep; /* CANVAS_FADE_STEPs the background has darkened so far */
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
	Uint16 padding; /* aligns bg2x; never read or written */
	/* BG2's reference point, written to BG2X and BG2Y by
	   btEffectApplyBg2Reference and btEffectAdvanceBg2OnInterval. */
	Sint32		bg2x;
	Sint32		bg2y;
	struct SPR *sprites[16]; /* the effect's sprites */
	/* Each target's flash timer in ticks, for args->targets[n]: set by
	   btEffectTintBattler, counted down by btEffectCountDownBattlerTints. */
	Uint8 actorTimers[8];
	/* 1 on a canvas copy's VBlank, then counts up */
	Sint32 framesSinceTransfer;
	/* 1 once the effect has presented its canvas (EFFECT_PRESENT_CANVAS); the
	   canvas transfer event copies the canvas and clears it. */
	Sint32				transferPending;
	BATTLE_EFFECT_PARM *args; /* the command being run */
} BATTLE_EFFECT_STATE;

STATIC_ASSERT(offsetof(BATTLE_EFFECT_STATE, bg2xLines) == 27008);
STATIC_ASSERT(offsetof(BATTLE_EFFECT_STATE, imageStorage) == 0);
STATIC_ASSERT(offsetof(BATTLE_EFFECT_STATE, trackedCanvas.origin) == 5060);
STATIC_ASSERT(offsetof(BATTLE_EFFECT_STATE, particles) == 28800);
STATIC_ASSERT(offsetof(BATTLE_EFFECT_STATE, transferMode) == 30592);
STATIC_ASSERT(offsetof(BATTLE_EFFECT_STATE, transferValue) == 30596);
STATIC_ASSERT(offsetof(BATTLE_EFFECT_STATE, frame) == 30604);
STATIC_ASSERT(offsetof(BATTLE_EFFECT_STATE, scrollTimer) == 30608);
STATIC_ASSERT(offsetof(BATTLE_EFFECT_STATE, scrollStepX) == 30616);
STATIC_ASSERT(offsetof(BATTLE_EFFECT_STATE, savedBg1X) == 30624);
STATIC_ASSERT(offsetof(BATTLE_EFFECT_STATE, shakeFrame) == 30632);
STATIC_ASSERT(offsetof(BATTLE_EFFECT_STATE, cameraDelta) == 30636);
STATIC_ASSERT(offsetof(BATTLE_EFFECT_STATE, fadeFrame) == 30644);
STATIC_ASSERT(offsetof(BATTLE_EFFECT_STATE, fadeStep) == 30648);
STATIC_ASSERT(offsetof(BATTLE_EFFECT_STATE, win0h) == 30652);
STATIC_ASSERT(offsetof(BATTLE_EFFECT_STATE, bg2x) == 30672);
STATIC_ASSERT(offsetof(BATTLE_EFFECT_STATE, sprites) == 30680);
STATIC_ASSERT(offsetof(BATTLE_EFFECT_STATE, actorTimers) == 30744);
STATIC_ASSERT(offsetof(BATTLE_EFFECT_STATE, framesSinceTransfer) == 30752);
STATIC_ASSERT(offsetof(BATTLE_EFFECT_STATE, transferPending) == 30756);
STATIC_ASSERT(offsetof(BATTLE_EFFECT_STATE, args) == 30760);
STATIC_ASSERT(sizeof(BATTLE_EFFECT_STATE) == 30764);

/* The pixel fragments btEffectBuildBlitter copies into its blitters, from
   blitter_fragments.s: Forward reads the picture left to right and Reverse
   right to left. None of them runs where it is. Max, four pixels (one canvas
   word), right to left; a full cell's row takes it twice. */
extern const Uint32 BattleEffectMax4Reverse[];
/* Max, four pixels (one canvas word), left to right; a full cell's row takes it
   twice. */
extern const Uint32 BattleEffectMax4Forward[];
/* Max, one pixel, right to left. */
extern const Uint32 BattleEffectMaxReverse[];
/* Max, one pixel, left to right. */
extern const Uint32 BattleEffectMaxForward[];
/* Additive, four pixels added a word at a time, right to left; a full cell's
   row takes it twice. */
extern const Uint32 BattleEffectColorReverse[];
/* Additive, four pixels added a word at a time, left to right; a full cell's
   row takes it twice. */
extern const Uint32 BattleEffectColorForward[];
/* Additive, one pixel, right to left. */
extern const Uint32 BattleEffectAdditiveReverse[];
/* Additive, one pixel, left to right. */
extern const Uint32 BattleEffectAdditiveForward[];
/* Builds the mask word, 64 in every byte, with which an additive word's pixels
   are capped at CANVAS_LEVEL_BRIGHTEST. */
extern const Uint32 BattleEffectMask[];
/* Transparent, a full cell's eight pixels, right to left. */
extern const Uint32 BattleEffectTransparent8Reverse[];
/* Transparent, a full cell's eight pixels, left to right. */
extern const Uint32 BattleEffectTransparent8Forward[];
/* Transparent, one pixel, right to left. */
extern const Uint32 BattleEffectTransparentReverse[];
/* Transparent, one pixel, left to right. */
extern const Uint32 BattleEffectTransparentForward[];
/* Copy, one pixel, right to left. */
extern const Uint32 BattleEffectCopyReverse[];
/* Copy, one pixel, left to right. */
extern const Uint32 BattleEffectCopyForward[];

#endif
