/* The sites of the three Colosso finals matches (Colosseum Final 1, 2 and 3):
   the large lettering a race starts with. It reads "The 1st Match", "3", "2",
   "1", "GO", from the pictures of games/tbs/src/graphics/colosso/lettering.tsv
   (COLOSSO_LETTERING_PICTURE). Each picture is COLOSSO_LETTERING_PICTURE_BYTES
   of tiles after the palettes. */

#include "system.h"

/* The lettering moves by a script of 16-bit words, a command and its values: */
typedef enum COLOSSO_LETTERING_COMMAND {
	/* x, speed: put the lettering at x pixels from the centre, moving speed /
	   COLOSSO_LETTERING_SUBPIXELS pixels a frame */
	COLOSSO_LETTERING_PLACE = 0x4000,
	/* speed, frames: change the speed over that many frames */
	COLOSSO_LETTERING_ACCELERATE = 0x3000,
	/* scale, frames: grow or shrink to scale (256 is its own size) */
	COLOSSO_LETTERING_SCALE = 0x1000,
	/* weight, frames: fade to weight sixteenths over the screen */
	COLOSSO_LETTERING_FADE = 0x2000,
	/* frames: run the moves for that many frames before the next command */
	COLOSSO_LETTERING_WAIT = 0x7fff,
	/* remove the lettering and free its VRAM block */
	COLOSSO_LETTERING_END = -1
} COLOSSO_LETTERING_COMMAND;

/* The lettering pictures, in lettering.tsv's order (scColossoLoadLettering). */
typedef enum COLOSSO_LETTERING_PICTURE {
	COLOSSO_PICTURE_LEND_A_HAND = 0,
	/* "The 1st Match", then the 2nd and 3rd. */
	COLOSSO_PICTURE_MATCH_TITLE_FIRST = 1,
	COLOSSO_PICTURE_WIN_LOSE = 4,
	COLOSSO_PICTURE_COUNTDOWN_START = 5, /* the countdown's 3 and 2 */
	COLOSSO_PICTURE_COUNTDOWN_END = 6,	 /* the countdown's 1 */
	COLOSSO_PICTURE_GO = 7,
	/* COLOSSO_PICTURE_WIN_LOSE in the blue palette a loss shows it in. */
	COLOSSO_PICTURE_WIN_LOSE_FOR_LOSS = 8
} COLOSSO_LETTERING_PICTURE;

/* The bytes of one lettering picture's tiles: 2,048 pixels at 4 bits each, as
   four 32x16 pieces or two 32x32 ones (lettering.tsv). */
#define COLOSSO_LETTERING_PICTURE_BYTES (4 * 32 * 16 / 2)

/* The lettering's x and speed are kept in 256ths of a pixel. */
#define COLOSSO_LETTERING_SUBPIXELS 256

/* The lettering's sprites are drawn just behind the windows. */
#define COLOSSO_LETTERING_DEPTH			(OAM_DEPTH_WINDOW - 4)
/* The sprite palette the lettering's colours are loaded into. */
#define COLOSSO_LETTERING_PALETTE		15
/* The lettering's pieces are 32 pixels wide: piece i of a row of n lies this
   far from the row's centre. */
#define COLOSSO_LETTERING_PIECE_X(i, n) ((i) * 32 - ((n) - 1) * 16)

/* How scColossoUpdateLettering lays a picture out (Colosso_LetteringMode);
   Colosso_LetteringParameter picks the second half of a picture. */
typedef enum COLOSSO_LETTERING_SHAPE {
	/* Four 32x16 pieces in a row: The 1st to 3rd Match, Lend a Hand! */
	COLOSSO_SHAPE_TITLE = 1,
	COLOSSO_SHAPE_DIGIT = 2,  /* one 32x32 piece: 3, 2, 1 */
	COLOSSO_SHAPE_RESULT = 3, /* two 32x16 pieces: WIN or LOSE */
	COLOSSO_SHAPE_GO = 4	  /* Go!: one 64x32 piece, or two 32x16 in French */
} COLOSSO_LETTERING_SHAPE;

/* The lettering's script as it runs: the frames its wait has left, its glide's
   target and frames, its layout mode and parameter, its position (8.8 pixels)
   and the script itself. */
extern Sint16  Colosso_LetteringTimer;
extern Sint16  Colosso_LetteringMoveTarget;
extern Sint16  Colosso_LetteringMoveDuration;
extern Sint16  Colosso_LetteringMode;
extern Sint16  Colosso_LetteringParameter;
extern Sint32  Colosso_LetteringPosition;
extern Sint16 *Colosso_LetteringScript;

/* Loads lettering picture id into the lettering's VRAM block (taken once) and
   its palette into COLOSSO_LETTERING_PALETTE. */
static void scColossoLoadLettering(Sint32 id)
{
	Uint8 *buf;
	Sint32 off;

	/* Room for the palettes and seven pictures: Go!, the eighth, unpacks past
	   the block's end into the free EWRAM after it. */
	buf = syHeapAllocEWRAM(
		COLOSSO_PALETTE_BYTES + 7 * COLOSSO_LETTERING_PICTURE_BYTES);
	if(Colosso_LetteringSlot == -1)
		Colosso_LetteringSlot = syVRAMCacheFindFreeEntry();
	off = Colosso_LetteringPaletteOffsets[id];
	if(id == COLOSSO_PICTURE_WIN_LOSE_FOR_LOSS)
		id = COLOSSO_PICTURE_WIN_LOSE;
	syFieldDecompressTaggedLZ(syResourceGetTableEntry(RESOURCE_LETTERING), buf);
	syDMAXCopy(buf + off, (void *)&OBJ_PLTT_COLOR(COLOSSO_LETTERING_PALETTE, 0),
		DMA_COPY_WORDS(8), REG_DMA3);
	mnVRAMLoad(Colosso_LetteringSlot, COLOSSO_LETTERING_PICTURE_BYTES,
		(Uint8 *)((id * COLOSSO_LETTERING_PICTURE_BYTES) + (Sint32)buf +
			COLOSSO_PALETTE_BYTES));
	syDMAWait(REG_DMA3);
	syHeapRelease(buf);
}

/* The lettering's event: run its script, move, scale and fade it, and draw it
   as double-size affine sprites blended over the screen, in the shape of
   Colosso_LetteringMode. */
static void scColossoUpdateLettering(void)
{
	Uint32 *write = Colosso_LetteringSprites;
	/* Each lettering piece is queued as a sprite part written as three words:
	   the queue link, attributes 0 and 1, and attribute 2. */
	ANIMATION_SPR_PART *sprite = (ANIMATION_SPR_PART *)write;
	Sint32 tile = VramCacheEntries[Colosso_LetteringSlot].ofs / TILE_SIZE_4BPP;
	Uint32 flags;
	Sint32 scale, blend, speed, centerX;
	Sint32 matrix, i, x, y, left;
	AFFINE_TRANSFORM work;

commands:
	if(Colosso_LetteringTimer == 0) {
		{
			switch(*Colosso_LetteringScript++) {
			case COLOSSO_LETTERING_PLACE:
				Colosso_LetteringPosition = *Colosso_LetteringScript++ << 8;
				Colosso_LetteringMoveTarget = *Colosso_LetteringScript++;
				Colosso_LetteringMoveDuration = 0;
				break;
			case COLOSSO_LETTERING_ACCELERATE:
				Colosso_LetteringMoveStart = Colosso_LetteringMoveTarget;
				Colosso_LetteringMoveTarget = *Colosso_LetteringScript++;
				Colosso_LetteringMoveDuration = *Colosso_LetteringScript++;
				Colosso_LetteringMoveStep = 0;
				break;
			case COLOSSO_LETTERING_SCALE:
				Colosso_LetteringScaleStart = Colosso_LetteringScaleTarget;
				Colosso_LetteringScaleTarget = *Colosso_LetteringScript++;
				Colosso_LetteringScaleDuration = *Colosso_LetteringScript++;
				Colosso_LetteringScaleStep = 0;
				break;
			case COLOSSO_LETTERING_FADE:
				Colosso_LetteringFadeStart = Colosso_LetteringFadeTarget;
				Colosso_LetteringFadeTarget = *Colosso_LetteringScript++;
				Colosso_LetteringFadeDuration = *Colosso_LetteringScript++;
				Colosso_LetteringFadeStep = 0;
				break;
			case COLOSSO_LETTERING_WAIT:
				Colosso_LetteringTimer = *Colosso_LetteringScript++;
				break;
			case COLOSSO_LETTERING_END:
				syCloseEvent(scColossoUpdateLettering);
				syVRAMCacheFreeEntry(Colosso_LetteringSlot);
				return;
			}
		}
		goto commands;
	}
	Colosso_LetteringTimer--;
	if(Colosso_LetteringScaleDuration == 0) {
		scale = Colosso_LetteringScaleTarget;
	} else {
		Sint32 duration, start, progress, target;
		duration = Colosso_LetteringScaleDuration;
		start = Colosso_LetteringScaleStart;
		target = Colosso_LetteringScaleTarget;
		progress = ++Colosso_LetteringScaleStep;
		scale = start + (target - start) * progress / duration;
		if(progress >= duration)
			Colosso_LetteringScaleDuration = 0;
	}
	if(Colosso_LetteringFadeDuration == 0) {
		blend = Colosso_LetteringFadeTarget;
	} else {
		Sint32 duration, start, progress, target;
		duration = Colosso_LetteringFadeDuration;
		start = Colosso_LetteringFadeStart;
		target = Colosso_LetteringFadeTarget;
		progress = ++Colosso_LetteringFadeStep;
		blend = start + (target - start) * progress / duration;
		if(progress >= duration)
			Colosso_LetteringFadeDuration = 0;
	}
	if(Colosso_LetteringMoveDuration == 0) {
		speed = Colosso_LetteringMoveTarget;
	} else {
		Sint32 duration, start, progress, target;
		duration = Colosso_LetteringMoveDuration;
		start = Colosso_LetteringMoveStart;
		target = Colosso_LetteringMoveTarget;
		progress = ++Colosso_LetteringMoveStep;
		speed = start + (target - start) * progress / duration;
		if(progress >= duration)
			Colosso_LetteringMoveDuration = 0;
	}
	work.ang = 0;
	work.scaleX = scale;
	work.scaleY = scale;
	matrix = (Sint16)syOAMMatrixAllocFromTransform(&work);
	Colosso_LetteringPosition += speed;
	centerX = Colosso_LetteringPosition / COLOSSO_LETTERING_SUBPIXELS;
	switch(Colosso_LetteringMode) {
	case COLOSSO_SHAPE_TITLE: {
		Uint32 attr;
		i = 0;
		y = 56;
		flags = OBJ_SIZE_32X16;
		attr = OBJ_MATRIX(matrix);
		for(; i < 4; i++) {
			x = centerX + scale * COLOSSO_LETTERING_PIECE_X(i, 4) / 256;
			left = x + 88;
			if(x >= -152 && x < 152) {
				x = left & OBJ_X_MASK;
				*write++ = 0;
				*write++ = OBJ_X(x) | y | flags | attr |
					OBJ_MODE(OBJ_MODE_SEMI_TRANSPARENT) | OBJ_DOUBLE_SIZE |
					OBJ_AFFINE;
				*write++ = tile | OBJ_PRIORITY(1) |
					OBJ_PALETTE(COLOSSO_LETTERING_PALETTE);
				syOAMAddSprAtDepth(sprite++, COLOSSO_LETTERING_DEPTH);
			}
			tile += 8;
		}
		break;
	}
	case COLOSSO_SHAPE_RESULT: {
		Uint32 attr;
		i = 0;
		y = 48;
		flags = OBJ_SIZE_32X16;
		attr = OBJ_MATRIX(matrix);
		for(; i < 2; i++) {
			x = centerX + scale * COLOSSO_LETTERING_PIECE_X(i, 2) / 256;
			left = x + 88;
			if(x >= -152 && x < 152) {
				x = left & OBJ_X_MASK;
				*write++ = 0;
				*write++ = OBJ_X(x) | y | flags | attr |
					OBJ_MODE(OBJ_MODE_SEMI_TRANSPARENT) | OBJ_DOUBLE_SIZE |
					OBJ_AFFINE;
				*write++ = (tile + Colosso_LetteringParameter) |
					OBJ_PRIORITY(1) | OBJ_PALETTE(COLOSSO_LETTERING_PALETTE);
				syOAMAddSprAtDepth(sprite++, COLOSSO_LETTERING_DEPTH);
			}
			tile += 8;
		}
		break;
	}
#if LANGUAGE == LANGUAGE_FRENCH
	case COLOSSO_SHAPE_GO: {
		Uint32 attr;
		i = 0;
		y = 64;
		flags = OBJ_SIZE_32X16;
		attr = OBJ_MATRIX(matrix);
		for(; i < 2; i++) {
			x = centerX + scale * COLOSSO_LETTERING_PIECE_X(i, 2) / 256;
			left = x + 88;
			if(x >= -152 && x < 152) {
				x = left & OBJ_X_MASK;
				*write++ = 0;
				*write++ = OBJ_X(x) | y | flags | attr |
					OBJ_MODE(OBJ_MODE_SEMI_TRANSPARENT) | OBJ_DOUBLE_SIZE |
					OBJ_AFFINE;
				*write++ = (tile + Colosso_LetteringParameter) |
					OBJ_PRIORITY(1) | OBJ_PALETTE(COLOSSO_LETTERING_PALETTE);
				syOAMAddSprAtDepth(sprite++, COLOSSO_LETTERING_DEPTH);
			}
			tile += 8;
		}
		break;
	}
#else
	case COLOSSO_SHAPE_GO:
		y = 48;
		flags = OBJ_SIZE_64X32;
		left = centerX + 56;
		if(centerX >= -120 && centerX < 184) {
			x = left & OBJ_X_MASK;
			*write++ = 0;
			*write++ = OBJ_X(x) | y | flags | OBJ_MATRIX(matrix) |
				OBJ_MODE(OBJ_MODE_SEMI_TRANSPARENT) | OBJ_DOUBLE_SIZE |
				OBJ_AFFINE;
			*write++ = (tile + Colosso_LetteringParameter) | OBJ_PRIORITY(1) |
				OBJ_PALETTE(COLOSSO_LETTERING_PALETTE);
			syOAMAddSprAtDepth(sprite, COLOSSO_LETTERING_DEPTH);
		}
		break;
#endif
	case COLOSSO_SHAPE_DIGIT:
		y = 48;
		flags = OBJ_SIZE_32X32;
		left = centerX + 88;
		if(centerX >= -152 && centerX < 152) {
			x = left & OBJ_X_MASK;
			*write++ = 0;
			*write++ = OBJ_X(x) | y | flags | OBJ_MATRIX(matrix) |
				OBJ_MODE(OBJ_MODE_SEMI_TRANSPARENT) | OBJ_DOUBLE_SIZE |
				OBJ_AFFINE;
			*write++ = (tile + Colosso_LetteringParameter) | OBJ_PRIORITY(1) |
				OBJ_PALETTE(COLOSSO_LETTERING_PALETTE);
			syOAMAddSprAtDepth(sprite, COLOSSO_LETTERING_DEPTH);
		}
		break;
	}
	IO_WRITE_QUEUE_PUSH(
		(Uint32)&REG_BLDCNT, BLDCNT_TGT2_ALL, IO_WRITE_HALFWORD);
	IO_WRITE_QUEUE_PUSH((Uint32)&REG_BLDALPHA,
		((BLEND_WEIGHT_MAX - blend) << 8) | blend, IO_WRITE_HALFWORD);
}
