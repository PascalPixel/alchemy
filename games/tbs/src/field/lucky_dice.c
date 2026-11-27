/* Lucky Dice, the dice game in Tolbi, drawn with the battles' 3D camera. */

#include "field/lucky_dice.h"
#include "macros.h"
#include "resource_ids.h"
#include "battle/battle.h"
#include "battle/battle_camera.h"
#include "battle/battle_effect_state.h"
#include "field/map_grid_scroll.h"
#include "game/main_routine_state.h"
#include "game/system_call.h"
#include "text/msg_ids.h"

/* Starts the Lucky Dice minigame: leaves only the backdrop on screen, starts
   its music and plays until the player quits. Returns 0. */
Sint32 fdLuckyDiceStart(void)
{
	REG_DISPCNT = DISPCNT_OBJ_1D_MAP;
	sySndPlayStub(MUS_TRY_YOUR_LUCK);
	fdLuckyDiceRun();
	return 0;
}

/* Puts the 3D camera back at its starting view: at the origin, unturned,
   pitched to 152/256 of a turn and 510 pixels away, following nothing. */
void fdCameraResetViewDefaults(void)
{
	BATTLE_CAMERA *camera = BattleCameraWork;
	Sint32		   ofs[3];
	Uint32		   nearPlane;

	camera->pitch = TRIG_TURN * 152 / 256;
	camera->distance = toFIXED(510);
	SET_VECTOR(camera->focus, 0, 0, 0);
	camera->yaw = 0;
	camera->followPos = 0;
	ProjectionWork.centerX = 0;
	ProjectionWork.centerY = 0;
	camera->followEye = 0;

	/* The eye sits `distance` behind the target along the camera's view. */
	syInitMatrix();
	syMatrixTranslateVector(&camera->focus[0]);
	syRotY(camera->yaw);
	syRotX(camera->pitch);

	SET_VECTOR(ofs, 0, 0, camera->distance);
	syCalcPoint(ofs, camera->eye);

	nearPlane = syDivFX(toFIXED(250), toFIXED(3) / 4);
	sySetScreenDist(toFIXED(250), nearPlane, toFIXED(32767));
}

/* One in 8.8 fixed point: in the unused helpers below, and in a sprite's affine
   scale, which draws it full size at Q8_ONE (a die's half size below). */
#define Q8_ONE (1 << 8)

/* Unused: multiplies two 8.8 fixed-point values, rounding toward zero. */
UNUSED Sint32 fdFixedPointMulQ8(Sint16 left, Sint16 right)
{
	Sint32 product;
	Sint32 roundedProduct;

	/* Rounded toward zero, as a division by Q8_ONE rounds. */
	product = left;
	product *= right;
	if(product >= 0)
		roundedProduct = product;
	else
		roundedProduct = product + (Q8_ONE - 1);
	return (Sint16)(roundedProduct >> 8);
}

/* Unused: divides two 8.8 fixed-point values. */
UNUSED Sint16 fdFixedPointDivQ8(Sint16 numerator, Sint16 denominator)
{
	return (numerator * Q8_ONE) / denominator;
}

/* Unused: the reciprocal of an 8.8 fixed-point value. */
UNUSED Sint16 fdFixedPointReciprocal(Sint16 value)
{
	return toFIXED(1) / value;
}

/* Lucky Dice's layout, fitted to each language's text: the coins window at the
   top and its label, the help window (Throw / Quit) and the winnings window at
   the bottom with their text, in tiles for windows and pixels within them, and
   the centre of the result lettering in sprite x, which wraps at 512. */
#if LANGUAGE == LANGUAGE_GERMAN || LANGUAGE == LANGUAGE_SPANISH || \
	LANGUAGE == LANGUAGE_FRENCH
#define DICE_COINS_WINDOW_X		   17
#define DICE_COINS_WINDOW_WIDTH	   13
#define DICE_WINNINGS_WINDOW_WIDTH 13
#elif LANGUAGE != LANGUAGE_JAPANESE
#define DICE_COINS_WINDOW_X		   18
#define DICE_COINS_WINDOW_WIDTH	   12
#define DICE_WINNINGS_WINDOW_WIDTH 12
#else
#define DICE_COINS_WINDOW_X		   18
#define DICE_COINS_WINDOW_WIDTH	   11
#define DICE_WINNINGS_WINDOW_WIDTH 10
#endif

/* The x of the coins window's label, in pixels. */
#define DICE_COINS_LABEL_X BY_LANGUAGE(48, 48, 48, 56, 53, 48)

/* The help window (Throw / Quit), in tiles. */
#define DICE_HELP_WINDOW_X	   BY_LANGUAGE(23, 22, 20, 20, 21, 22)
#define DICE_HELP_WINDOW_WIDTH BY_LANGUAGE(7, 8, 10, 10, 9, 8)

/* The winnings window, in tiles, its number and coins label in pixels, and the
   centre of the result lettering in sprite x. */
#define DICE_WINNINGS_WINDOW_X	   JAPANESE_OR(10, 9)
#define DICE_WINNINGS_NUMBER_X	   JAPANESE_OR(40, 48)
#define DICE_WINNINGS_COINS_X	   JAPANESE_OR(40, DICE_COINS_LABEL_X)
#define DICE_RESULT_LABEL_CENTER_X JAPANESE_OR(640, 632)

/* The help window's labels, counted in messages from MsgLuckyDiceCoins:
   ":Throw" (MsgLuckyDiceThrow) and ":Quit" (MsgLuckyDiceQuit). */
enum { DICE_LABEL_THROW = 10, DICE_LABEL_QUIT = 11 };

/* Lucky Dice's state block (HEAP_SLOT_LUCKY_GAME): its sprites and windows,
   between two runs of bytes it allocates and never touches. */
typedef struct LUCKY_DICE_SPR {
	Uint8	  unused[200];
	OAM_WORDS oam[128];	   /* copied to OAM each tick */
	WINDOW	 *coinsWin;	   /* the player's coins */
	WINDOW	 *winningsWin; /* the winnings of the last throw */
	WINDOW	 *helpWin;	   /* the Throw and Quit buttons */
	Uint8	  unusedEnd[324];
} LUCKY_DICE_SPR;

/* sprites.tsv's seven sheets, from the rolling die to the lucky numbers: 496
   256-colour tiles, nearly all of sprite VRAM. */
#define DICE_SPRITE_TILES 496

/* What a throw's four numbers make, in the rules' words: "If two numbers match
   on either the dice or the table... it's called a pair, and you don't win or
   lose anything. Two pair doubles your money. Triple matches triple your money.
   Four of a kind is perfect, and you win five times your bet." Every result has
   its lettering (aim_hand_and_results.png: "Perfect!!!", "Triple!", "Two Pair!"
   and "One Pair!"), numbered from 0. */
typedef enum DICE_RESULT {
	DICE_RESULT_NONE = -1, /* no two numbers alike: the bet is lost */
	DICE_RESULT_PERFECT, /* all four alike: DICE_PERFECT_PAYOUT times the bet */
	DICE_RESULT_TRIPLE,	 /* three alike: three times the bet */
	DICE_RESULT_TWO_PAIR, /* two pairs: twice the bet */
	DICE_RESULT_ONE_PAIR  /* one pair: the bet back */
} DICE_RESULT;

/* Lucky Dice's tables, the same in both games. */

/* Unused: a table of signs. */
const Sint32 LuckyDiceUnusedSigns[] = {-1, 1, -1, 1, -1, -1, 1, 1};

/* How many sprites each result's lettering takes, DICE_LABEL_SPRITE_WIDTH
   pixels each. */
const Uint8 LuckyDiceLabelCounts[] = {
	[DICE_RESULT_PERFECT] = 5,
	[DICE_RESULT_TRIPLE] = 4,
	[DICE_RESULT_TWO_PAIR] = 3,
	[DICE_RESULT_ONE_PAIR] = 3,
};

/* The first tile of each result's lettering, in 16-pixel steps from
   DICE_NUMBER_TILES. */
const Uint8 LuckyDiceLabelTiles[] = {
	[DICE_RESULT_PERFECT] = 28,
	[DICE_RESULT_TRIPLE] = 48,
	[DICE_RESULT_TWO_PAIR] = 64,
	[DICE_RESULT_ONE_PAIR] = 76,
#if LANGUAGE != LANGUAGE_JAPANESE
	/* Four more, which nothing reads. */
	28,
	248,
	64,
	248,
#endif
};

/* The die face tile at each step of its roll: one for each DICE_ROLL_PER_TILE
   of the roll angle (variant), three to a face. */
const Uint8 LuckyDiceFaceTiles[] = {
	0, 12, 16, 1, 14, 8, 2, 15, 10, 3, 9, 13, 4, 7, 6, 5, 17, 11};

/* The board's rows by depth: a die at rest is in the first row whose value its
   z (16.16 pixels) exceeds, -32767 taking the rest. */
const Sint16 LuckyDiceFaceHeights[] = {147, 80, 15, -42, -95, -32767};

/* The dice, the faces of each, and the sprites of a throw: the two dice, then
   their shadows. */
#define DICE_COUNT				 2
#define DICE_FACE_COUNT			 6
#define DICE_RESULT_COUNT		 4
/* The result's lettering is a row of 32 by 32 sprites centred on
   DICE_RESULT_LABEL_CENTER_X. */
#define DICE_LABEL_SPRITE_WIDTH	 32
#define DICE_BET_PER_LEVEL		 10
/* The result: one number every 5 ticks, then 20 ticks in the lettering, the
   winnings, and A to go on; the payout closes 15 ticks after it starts. */
#define DICE_NUMBER_REVEAL_TICKS 5
#define DICE_RESULT_TICKS		 20
#define DICE_PAYOUT_TICKS		 15
/* The coins are drawn with six digits. */
#define DICE_COIN_DIGITS		 6

/* The board fades in and out over 16 ticks, a sixteenth of full brightness a
   tick. */
#define DICE_FADE_TICKS 16

/* The depths from the camera a die is drawn between: nearer or further is drawn
   as at the limit, and the nearer, the larger. */
#define DICE_NEAREST_DEPTH	350
#define DICE_FARTHEST_DEPTH 650

/* Two dice whose centres come closer than this push each other apart. */
#define DICE_TOUCH_DISTANCE			 50
/* Where each picture's sprite tiles start, in OAM's 32-byte steps (two to a
   256-colour tile): the rolling die's views from 0, its shadow, the big numbers
   (the result's lettering counted on from them), the aiming hand, the Perfect
   Bonus sign, its 3, 5 and times sign, and the lucky numbers' small faces. */
#define DICE_SHADOW_TILE			 144
#define DICE_NUMBER_TILES			 152
#define DICE_AIM_MARKER_TILE		 344
#define DICE_BONUS_SIGN_TILE		 856
#define DICE_THREE_TILE				 920
#define DICE_FIVE_TILE				 928
#define DICE_TIMES_TILE				 936
#define DICE_LUCKY_NUMBER_TILES		 944
/* Four alike pays five times the bet; four of a lucky number pays three or five
   times that, the multipliers drawn beside the two lucky numbers. */
#define DICE_PERFECT_PAYOUT			 5
#define DICE_FIRST_LUCKY_MULTIPLIER	 3
#define DICE_SECOND_LUCKY_MULTIPLIER 5
/* A win flashes a die (both for four alike) for this many ticks. */
#define DICE_FLASH_TICKS			 60
#define DICE_PERFECT_FLASH_TICKS	 120
/* A thrown die starts spinning at this rate, its roll angle a tick; each
   bounce takes 20 off it. */
#define DICE_THROW_SPIN				 300
#define DICE_BOUNCE_SPIN_LOSS		 20

/* A die's roll angle (its particle's variant) crosses a face in
   DICE_TILES_PER_FACE tiles, and fdLuckyDiceRun reads the tile as angle /
   DICE_ROLL_PER_TILE and the face as angle / DICE_ROLL_PER_FACE. */
#define DICE_TILES_PER_FACE 3
#define DICE_ROLL_PER_TILE	1024
#define DICE_ROLL_PER_FACE	(DICE_TILES_PER_FACE * DICE_ROLL_PER_TILE)
#define DICE_ROLL_TURN		(DICE_FACE_COUNT * DICE_ROLL_PER_FACE) /* six faces */

/* The board's picture begins with its 224 colours, the sprites' with all 256,
   each before its tiles. */
#define DICE_BOARD_PALETTE_COLORS	224
#define DICE_SPRITES_PALETTE_COLORS 256

/* A die's or a shadow's affine scale at DICE_NEAREST_DEPTH, in 256ths: drawn at
   twice its size, and smaller by half of its depth past it. */
#define DICE_NEAREST_SCALE (Q8_ONE / 2)

/* What the Lucky Dice screen is doing. */
typedef enum LUCKY_DICE_STATE {
	LUCKY_DICE_AIM,	   /* the dice wait to be thrown; Up and Down raise them */
	LUCKY_DICE_ROLL,   /* the dice fly, bounce and settle */
	LUCKY_DICE_RESULT, /* the four numbers and the result are shown */
	LUCKY_DICE_PAYOUT  /* the winnings move into the coins */
} LUCKY_DICE_STATE;

#include "palette_scale.c"

/* Runs the Lucky Dice minigame until the player quits or cannot afford a throw.
   Each throw costs ten coins per level of the party's average. */
void fdLuckyDiceRun(void)
{
	Uint16 bgPalette[BG_PLTT_COLORS];
	/* the two dice, then their shadows */
	Sint32 sprDepth[DICE_RESULT_COUNT];
	/* The two faces, then the two rows the dice stopped in. */
	Sint32 numbers[DICE_RESULT_COUNT];
	Sint32 groundPoint[3];
	Sint32 screen[3];
	/* LEFTOVER(frame): an unused local the ROM's frame reserves and never
	 * touches. */
	UNUSED Sint32 unused[8];
	Sint32		  spinRate[DICE_COUNT];
	Sint32 flashTmr[DICE_COUNT]; /* set on a win; nothing here reads it */
	/* the two numbers shown at the top, never equal */
	Sint32 luckyNumbers[DICE_COUNT];
	Sint32 drawOrder[DICE_COUNT]; /* the dice, the nearest first */
/* UB: three equal numbers produce three pairs, so the unused result list needs
   a third slot. */
#if FIX_DICE_MATCH_LIST_OVERFLOW
	UNUSED Sint32 matchedNumbers[3]; /* written, never read */
#else
	UNUSED Sint32 matchedNumbers[2]; /* written, never read */
#endif
	Uint16				*sprPalette;
	BATTLE_EFFECT_STATE *diceState;
	LUCKY_DICE_SPR		*sprs;
	BATTLE_CAMERA		*camera;
	Sint32				 frm;
	Sint32				 state;
	Sint32				 bet;
	Sint32				 resultTmr;
	Sint32				 resultKind;
	Sint32				 winnings;
	Sint32				 payoutTmr;
	/* The winnings' digit count, kept to clear the same field in the payout. */
	Sint32	  digits;
	Sint32	  sprCnt;
	PARTICLE *rollingDie;
	PARTICLE *otherDie;
	Sint32	  resetDice; /* put the dice back at the throwing line */
	WINDOW	 *win;
	Sint32	  idx;
	Sint32	  otherIdx;
	Sint32	  rollAng; /* a die's roll angle, in DICE_ROLL_PER_TILE steps */
	Sint32	  x;
	Sint32	  y;
	Sint32	  halfSize;
	Sint32	  faceTile; /* which of a face's three tiles the roll shows */
	Sint32	  sortedDie;
	Sint32	  tileX;
	Sint32	  tileY;
	Sint32	  resourceAdr;
	Sint32	  separationX;
	Sint32	  separationY;
	Sint32	  separationZ;
	Sint32	  separation;
	Sint32	  bounceSpeed;
	Uint32	  bounceAng;
	FIXED	  scale;
	Sint32	  pairCnt;
	Sint16	  affineScale;

	/* Allocate the screen's state blocks and reset the camera and events. */
	syHeapSlotAlloc(HEAP_SLOT_BATTLE_SHEET, GLOW_SHEET_ROOM);
	sprPalette = syHeapSlotAlloc(
		HEAP_SLOT_BATTLE_CANVAS, DICE_SPRITES_PALETTE_COLORS * sizeof(Uint16));
	diceState =
		syHeapSlotAllocEWRAM(HEAP_SLOT_BATTLE_EFFECT, sizeof(*diceState));
	sprs = syHeapSlotAllocEWRAM(HEAP_SLOT_LUCKY_GAME, sizeof(*sprs));
	camera = syHeapSlotAllocEWRAM(HEAP_SLOT_CAMERA, sizeof(*camera));
	syDummyNoOperationHook(RESOURCE_BATTLE_EFFECT_STUBS);
	fdCameraResetViewDefaults();
	syInitEvent();
	OamCopyEnabled = 0;

	/* Fill BG1's map (screen block 6) with a 30-tile-wide picture, then load
	   the board and dice graphics with their palettes faded to black. */
	idx = 0;
	for(tileY = 0; tileY != DISPLAY_TILE_HEIGHT; tileY++) {
		for(tileX = 0; tileX != BG_SCREEN_WIDTH; tileX++) {
			*(volatile Uint16 *)(BG_SCREEN_BLOCK(6) + idx) =
				tileX + tileY * DISPLAY_TILE_WIDTH;
			idx += 2;
		}
	}

	resourceAdr = (Sint32)syResourceGetTableEntry(RESOURCE_LUCKY_DICE_BOARD);
	syDMAXCopy((void *)resourceAdr, (void *)BG_PLTT,
		DMA_COPY_WORDS(DICE_BOARD_PALETTE_COLORS / 2), REG_DMA3);
	syDMAXCopy((void *)BG_PLTT, bgPalette, DMA_COPY_WORDS(sizeof bgPalette / 4),
		REG_DMA3);
	resourceAdr += DICE_BOARD_PALETTE_COLORS * 2;
	syFieldDecompressTaggedLZ(
		(const void *)resourceAdr, RamMapGridMetatileBuffer);
	/* One 256-colour tile for each tile of the screen. */
	syDMAXCopy(RamMapGridMetatileBuffer, BG_CHAR_BLOCK(1),
		DMA_COPY_WORDS(
			DISPLAY_TILE_WIDTH * DISPLAY_TILE_HEIGHT * TILE_SIZE_8BPP / 4),
		REG_DMA3);
	resourceAdr = (Sint32)syResourceGetTableEntry(RESOURCE_LUCKY_DICE_SPRITES);
	syDMAXCopy((void *)resourceAdr, sprPalette,
		DMA_COPY_WORDS(DICE_SPRITES_PALETTE_COLORS / 2), REG_DMA3);
	syDMAXCopy((void *)resourceAdr, (void *)OBJ_PLTT,
		DMA_COPY_WORDS(DICE_SPRITES_PALETTE_COLORS / 2), REG_DMA3);
	resourceAdr += DICE_SPRITES_PALETTE_COLORS * 2;
	syFieldDecompressTaggedLZ(
		(const void *)resourceAdr, RamMapGridMetatileBuffer);
	syDMAXCopy(RamMapGridMetatileBuffer, (void *)OBJ_VRAM0,
		DMA_COPY_WORDS(DICE_SPRITE_TILES * TILE_SIZE_8BPP / 4), REG_DMA3);
	fdPaletteScaleRGB555(sprPalette, (Uint16 *)OBJ_PLTT, 0, BG_PLTT_COLORS);
	fdPaletteScaleRGB555(bgPalette, (Uint16 *)BG_PLTT, 0, BG_PLTT_COLORS);

	/* Show the board on BG1, sprites and windows, with the board blended. */
	REG_BG1CNT = BGCNT_PRIORITY(2) | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS |
		BGCNT_SCREEN_BASE(6);
	REG_WININ = WININ_WIN0_BG0 | WININ_WIN0_BG1 | WININ_WIN0_BG2 |
		WININ_WIN0_OBJ | WININ_WIN0_CLR | WININ_WIN1_BG0 | WININ_WIN1_BG1 |
		WININ_WIN1_BG2 | WININ_WIN1_OBJ | WININ_WIN1_CLR;
	REG_WINOUT = WINOUT_WIN01_BG0 | WINOUT_WIN01_BG1 | WINOUT_WIN01_CLR |
		WINOUT_WINOBJ_BG0 | WINOUT_WINOBJ_BG1 | WINOUT_WINOBJ_BG2 |
		WINOUT_WINOBJ_CLR;
	REG_DISPCNT = DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON | DISPCNT_BG1_ON |
		DISPCNT_OBJ_ON | DISPCNT_WIN0_ON;
	REG_BLDCNT = BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
	REG_BLDALPHA = BLEND_ALPHA(16, 8);
	BgScrollWork[1].x = 0;
	BgScrollWork[1].y = 0;
	REG_BG1HOFS = 0;
	REG_BG1VOFS = -DISPLAY_HEIGHT;
	REG_BG2HOFS = 0;
	REG_BG2VOFS = -DISPLAY_HEIGHT;
	REG_WIN0H = WIN_RANGE(0, DISPLAY_WIDTH);
	REG_WIN0V = WIN_RANGE(0, DISPLAY_HEIGHT);
	REG_WIN1H = WIN_RANGE(0, DISPLAY_WIDTH);
	REG_WIN1V = WIN_RANGE(0, DISPLAY_HEIGHT);

	/* Start aiming with both dice at the throwing line and two lucky
	   numbers. */
	state = LUCKY_DICE_AIM;
	resultTmr = 0;
	resultKind = DICE_RESULT_NONE;
	winnings = 0;
	payoutTmr = 0;
	/* A random place and speed for each die: the throwing line below takes the
	   places, and a throw the speeds. */
	for(idx = 0; idx != DICE_COUNT; idx++) {
		diceState->particles[idx].x = toFIXED((syRandom() & 127) - 64);
		diceState->particles[idx].y = toFIXED(syRandom() & 31);
		diceState->particles[idx].z = toFIXED((syRandom() & 127) - 64);
		diceState->particles[idx].velX = ((syRandom() & 127) - 64) << 14;
		diceState->particles[idx].velY = ((syRandom() & 31) + 32) << 12;
		diceState->particles[idx].velZ = ((syRandom() & 127) - 64) << 14;
		diceState->particles[idx].variant = 0;
		spinRate[idx] = DICE_THROW_SPIN;
	}
	numbers[0] = numbers[1] = numbers[2] = numbers[3] = 0;
	flashTmr[0] = flashTmr[1] = 0;
	luckyNumbers[0] = syRandom() % DICE_FACE_COUNT;
	do {
		luckyNumbers[1] = syRandom() % DICE_FACE_COUNT;
	} while(luckyNumbers[0] == luckyNumbers[1]);
	diceState->particles[0].x = toFIXED(40);
	diceState->particles[0].y = toFIXED(96);
	diceState->particles[0].z = -toFIXED(160);
	diceState->particles[1].x = -toFIXED(40);
	diceState->particles[1].y = toFIXED(96);
	diceState->particles[1].z = -toFIXED(160);

	/* Open the coins window and the Throw / Quit help. */
	mnWindowSystemInitFieldStub();
	win = mnWindowCreateStub(DICE_COINS_WINDOW_X, 0, DICE_COINS_WINDOW_WIDTH, 3,
		WINDOW_FLAG_INSTANT | WINDOW_FLAG_UNREAD);
	sprs->coinsWin = win;
	mnTextDrawMessageAsTilesStub(MsgLuckyDiceCoins, win, DICE_COINS_LABEL_X, 0);
	mnTextDrawNumInWindowStub(MainRoutineStateWork.holdings.coins,
		DICE_COIN_DIGITS, sprs->coinsWin, 0, 0);
	win = mnWindowCreateStub(DICE_HELP_WINDOW_X, 16, DICE_HELP_WINDOW_WIDTH, 4,
		WINDOW_FLAG_INSTANT | WINDOW_FLAG_UNREAD);
	sprs->helpWin = win;
	/* ":Throw" and ":Quit" after their buttons. */
	mnTextDrawMessageAsTilesStub(
		MsgLuckyDiceCoins + DICE_LABEL_THROW, win, 0, 0);
	mnTextDrawMessageAsTilesStub(
		MsgLuckyDiceCoins + DICE_LABEL_QUIT, sprs->helpWin, 0, 8);

	frm = 0;
	for(;;) {
		resetDice = 0;
		/* Fade in. */
		if(frm <= DICE_FADE_TICKS) {
			fdPaletteScaleRGB555(sprPalette, (Uint16 *)OBJ_PLTT,
				frm * (toFIXED(1) / DICE_FADE_TICKS), BG_PLTT_COLORS);
			fdPaletteScaleRGB555(bgPalette, (Uint16 *)BG_PLTT,
				frm * (toFIXED(1) / DICE_FADE_TICKS), BG_PLTT_COLORS);
		}
		syInitMatrix();
		syLookAtLoad(camera->eye, camera->focus);

		/* The fixed sprites: the Perfect Bonus sign with its times 3 and times
		   5, and the two lucky numbers beside them. */
		sprCnt = 0;
		sprs->oam[sprCnt].attr01 = OBJ_SIZE_64X32 | OBJ_256_COLORS;
		sprs->oam[sprCnt].attr2 = DICE_BONUS_SIGN_TILE;
		sprCnt++;
		sprs->oam[sprCnt].attr01 =
			OBJ_POSITION(16, 32) | OBJ_SIZE_16X16 | OBJ_256_COLORS;
		sprs->oam[sprCnt].attr2 = DICE_THREE_TILE;
		sprCnt++;
		sprs->oam[sprCnt].attr01 =
			OBJ_POSITION(16, 48) | OBJ_SIZE_16X16 | OBJ_256_COLORS;
		sprs->oam[sprCnt].attr2 = DICE_FIVE_TILE;
		sprCnt++;
		sprs->oam[sprCnt].attr01 =
			OBJ_POSITION(4, 32) | OBJ_SIZE_16X16 | OBJ_256_COLORS;
		sprs->oam[sprCnt].attr2 = DICE_TIMES_TILE;
		sprCnt++;
		sprs->oam[sprCnt].attr01 =
			OBJ_POSITION(4, 48) | OBJ_SIZE_16X16 | OBJ_256_COLORS;
		sprs->oam[sprCnt].attr2 = DICE_TIMES_TILE;
		sprCnt++;
		for(idx = 0; idx != DICE_COUNT; idx++) {
			sprs->oam[sprCnt].attr01 =
				(32 + idx * 16) | OBJ_X(32) | OBJ_SIZE_16X16 | OBJ_256_COLORS;
			sprs->oam[sprCnt].attr2 =
				luckyNumbers[idx] * SPRITE_TILES_8BPP(16, 16) +
				DICE_LUCKY_NUMBER_TILES;
			sprCnt++;
		}

		/* After a throw, reveal the four numbers five ticks apart, then the
		   result's lettering. */
		if(state == LUCKY_DICE_RESULT || state == LUCKY_DICE_PAYOUT) {
			for(idx = 0; idx != DICE_RESULT_COUNT; idx++) {
				if(resultTmr == idx * DICE_NUMBER_REVEAL_TICKS) {
					sySndPlayStub(SE_LUCKY_DICE_NUMBER);
				}
				if(resultTmr >= idx * DICE_NUMBER_REVEAL_TICKS) {
					sprs->oam[sprCnt].attr01 = 64 | OBJ_SIZE_32X32 |
						OBJ_256_COLORS | OBJ_X(56 + idx * 32);
					sprs->oam[sprCnt].attr2 =
						numbers[idx] * SPRITE_TILES_8BPP(32, 32) +
						DICE_NUMBER_TILES;
					sprCnt++;
				}
			}
			if(resultTmr >= DICE_RESULT_TICKS && resultKind >= 0) {
				for(idx = 0; idx != LuckyDiceLabelCounts[resultKind]; idx++) {
					Sint32 labelX;
					Sint32 labelY = 96;

					labelX = DICE_RESULT_LABEL_CENTER_X -
						LuckyDiceLabelCounts[resultKind] *
							(DICE_LABEL_SPRITE_WIDTH / 2) +
						idx * DICE_LABEL_SPRITE_WIDTH;
					sprs->oam[sprCnt].attr01 = OBJ_POSITION(labelX, labelY) |
						OBJ_SIZE_32X32 | OBJ_256_COLORS;
					sprs->oam[sprCnt].attr2 = LuckyDiceLabelTiles[resultKind] *
							SPRITE_TILES_8BPP(16, 16) +
						DICE_NUMBER_TILES + idx * SPRITE_TILES_8BPP(32, 32);
					sprCnt++;
				}
			}
		}

		/* Showing the result: twenty ticks in, pay the winnings and open their
		   window; A then moves on to the payout, or back to aiming after a
		   loss. */
		if(state == LUCKY_DICE_RESULT) {
			if(resultTmr == DICE_RESULT_TICKS && winnings > 0) {
				gmPartyAdjustCoinsStub(winnings);
				if(winnings < 10) {
					digits = 1;
				} else if(winnings < 100) {
					digits = 2;
				} else if(winnings < 1000) {
					digits = 3;
				} else if(winnings < 10000) {
					digits = 4;
				} else {
					digits = 5;
				}
				win = mnWindowCreateStub(DICE_WINNINGS_WINDOW_X - digits / 2,
					16, digits + DICE_WINNINGS_WINDOW_WIDTH, 3,
					WINDOW_FLAG_INSTANT | WINDOW_FLAG_UNREAD);
				sprs->winningsWin = win;
				mnTextDrawMessageAsTilesStub(MsgLuckyDiceWinnings, win, 0, 0);
				/* "Coins" after the number (MsgLuckyDiceCoins, one before). */
				mnTextDrawMessageAsTilesStub(MsgLuckyDiceWinnings - 1,
					sprs->winningsWin, digits * 8 + DICE_WINNINGS_COINS_X, 0);
				mnTextDrawNumInWindowStub(winnings, digits, sprs->winningsWin,
					DICE_WINNINGS_NUMBER_X, 0);
			}
			/* DEAD: the count down of the 60 ticks a throw's end sets, which
			   nothing reads (the payout starts its own count at 0); the ROM
			   keeps the test and the decrement. */
			if(payoutTmr > 0) {
				payoutTmr--;
			}
			if(resultTmr >= DICE_RESULT_TICKS && (PerRepeat & KEY_A)) {
				sySndPlayStub(SE_MENU_CONFIRM);
				if(winnings > 0) {
					state = LUCKY_DICE_PAYOUT;
					payoutTmr = 0;
				} else {
					resetDice = 1;
					state = LUCKY_DICE_AIM;
#if LUCKY_DICE_HELP_AFTER_LOSS
					/* In every edition but the Japanese a lost throw reopens
					   the Throw / Quit help. */
					win = mnWindowCreateStub(DICE_HELP_WINDOW_X, 16,
						DICE_HELP_WINDOW_WIDTH, 4,
						WINDOW_FLAG_INSTANT | WINDOW_FLAG_UNREAD);
					sprs->helpWin = win;
					mnTextDrawMessageAsTilesStub(MsgLuckyDiceThrow, win, 0, 0);
					mnTextDrawMessageAsTilesStub(
						MsgLuckyDiceThrow + 1, sprs->helpWin, 0, 8);
#endif
				}
			}
			resultTmr++;
		} else if(state == LUCKY_DICE_PAYOUT) {
			/* Clear the winnings into the coins, then DICE_PAYOUT_TICKS later
			   close their window and aim again. */
			if(payoutTmr == 0) {
				winnings = 0;
				/* The 0 is drawn at Japanese's DICE_WINNINGS_NUMBER_X in every
				   edition, 8 pixels left of where the others drew the winnings;
				   The Lost Age draws it where it drew them. */
				mnTextDrawNumInWindowStub(0, digits, sprs->winningsWin, 40, 0);
				mnTextDrawNumInWindowStub(MainRoutineStateWork.holdings.coins,
					DICE_COIN_DIGITS, sprs->coinsWin, 0, 0);
			}
			if(payoutTmr == DICE_PAYOUT_TICKS) {
				state = LUCKY_DICE_AIM;
				mnWindowCloseStub(sprs->winningsWin, 1);
				win = mnWindowCreateStub(DICE_HELP_WINDOW_X, 16,
					DICE_HELP_WINDOW_WIDTH, 4,
					WINDOW_FLAG_INSTANT | WINDOW_FLAG_UNREAD);
				sprs->helpWin = win;
				mnTextDrawMessageAsTilesStub(MsgLuckyDiceThrow, win, 0, 0);
				mnTextDrawMessageAsTilesStub(
					MsgLuckyDiceThrow + 1, sprs->helpWin, 0, 8);
				resetDice = 1;
			}
			payoutTmr++;
		} else if(state == LUCKY_DICE_AIM && frm > DICE_FADE_TICKS + 1) {
			/* Aiming: Up and Down raise and lower the dice between 32 and 160.
			   B quits, as does running out of coins for a bet; A throws. */
			/* DEAD: PerRepeat is read twice and discarded before the KEY_UP
			   test; the ROM keeps both loads. */
			(void)PerRepeat;
			(void)PerRepeat;
			if(PerData & KEY_UP) {
				diceState->particles[0].y += toFIXED(4);
				diceState->particles[1].y += toFIXED(4);
			}
			if(PerData & KEY_DOWN) {
				diceState->particles[0].y += -toFIXED(4);
				diceState->particles[1].y += -toFIXED(4);
			}
			if(diceState->particles[0].y < toFIXED(32)) {
				diceState->particles[0].y = toFIXED(32);
			}
			if(diceState->particles[0].y > toFIXED(160)) {
				diceState->particles[0].y = toFIXED(160);
			}
			if(diceState->particles[1].y < toFIXED(32)) {
				diceState->particles[1].y = toFIXED(32);
			}
			if(diceState->particles[1].y > toFIXED(160)) {
				diceState->particles[1].y = toFIXED(160);
			}
			if(MainRoutineStateWork.holdings.coins <
				(Uint32)(gmPartyGetAverageLevelStub() * DICE_BET_PER_LEVEL)) {
				mnWindowCloseStub(sprs->helpWin, 1);
				break;
			}
			if(PerRepeat & KEY_B) {
				sySndPlayStub(SE_MENU_CANCEL);
				mnWindowCloseStub(sprs->helpWin, 1);
				break;
			}
			if(PerRepeat & KEY_A) {
				sySndPlayStub(SE_LUCKY_DICE_THROW);
				mnWindowCloseStub(sprs->helpWin, 1);
				state = LUCKY_DICE_ROLL;
				bet = gmPartyGetAverageLevelStub() * DICE_BET_PER_LEVEL;
				if((Uint32)bet > MainRoutineStateWork.holdings.coins) {
					bet = MainRoutineStateWork.holdings.coins;
				}
				gmPartyAdjustCoinsStub(-bet);
				mnTextDrawNumInWindowStub(MainRoutineStateWork.holdings.coins,
					DICE_COIN_DIGITS, sprs->coinsWin, 0, 0);
				for(idx = 0; idx != DICE_COUNT; idx++) {
					rollingDie = &diceState->particles[idx];
					/* DEAD: never true with DICE_COUNT 2 (a third die would
					   start further back and higher up); the ROM keeps the test
					   and its stores. */
					if(idx > 1) {
						rollingDie->z = -toFIXED(180);
						rollingDie->y = toFIXED(96);
					}
					SET_VELOCITY(rollingDie, ((syRandom() & 127) - 64) << 12,
						((syRandom() & 15) + 48) << 13,
						(((syRandom() & 63) + 140) << 12) + rollingDie->y / 6);
					rollingDie->variant = syRandom() % DICE_ROLL_TURN;
					spinRate[idx] = DICE_THROW_SPIN;
				}
			}
		}

		/* Back to the throwing line, with two new lucky numbers. */
		if(resetDice == 1) {
			diceState->particles[0].x = toFIXED(40);
			diceState->particles[0].y = toFIXED(96);
			diceState->particles[0].z = -toFIXED(160);
			diceState->particles[1].x = -toFIXED(40);
			diceState->particles[1].y = toFIXED(96);
			diceState->particles[1].z = -toFIXED(160);
			luckyNumbers[0] = syRandom() % DICE_FACE_COUNT;
			do {
				luckyNumbers[1] = syRandom() % DICE_FACE_COUNT;
			} while(luckyNumbers[0] == luckyNumbers[1]);
		}

		/* Draw the dice nearest first, so that the nearer is in front (an
		   insertion sort on depth), each scaled by its distance. */
		for(idx = 0; idx != DICE_COUNT; idx++) {
			drawOrder[idx] = idx;
		}
		for(idx = 1; idx < DICE_COUNT; idx++) {
			sortedDie = drawOrder[idx];
			for(otherIdx = idx - 1; otherIdx >= 0 &&
				diceState->particles[drawOrder[otherIdx]].z >
					diceState->particles[sortedDie].z;
				otherIdx--) {
				drawOrder[otherIdx + 1] = drawOrder[otherIdx];
			}
			drawOrder[otherIdx + 1] = sortedDie;
		}

		for(idx = 0; idx != DICE_COUNT; idx++) {
			Sint32 dieX;
			Sint32 dieY;

			rollingDie = &diceState->particles[drawOrder[idx]];
			/* Projection reads the die's three coordinate words directly. */
			syConvert3Dto2D((const FIXED *)rollingDie, screen);
			if(screen[2] < DICE_NEAREST_DEPTH) {
				screen[2] = DICE_NEAREST_DEPTH;
			}
			if(screen[2] > DICE_FARTHEST_DEPTH) {
				screen[2] = DICE_FARTHEST_DEPTH;
			}
			sprDepth[drawOrder[idx]] = screen[2] - DICE_NEAREST_DEPTH;
			/* Half a die's 16 pixels, at its scale. */
			halfSize = 8 * Q8_ONE /
				(sprDepth[drawOrder[idx]] / 2 + DICE_NEAREST_SCALE);
			dieX = screen[0] - halfSize;
			dieY = screen[1] - halfSize;
			/* Each die has its own affine matrix (attr1 bits 9 to 13); 632 and
			   352 wrap in OAM's 9- and 8-bit fields to 120 across and 96
			   down. */
			if(dieY + 96 > -16) {
				sprs->oam[sprCnt].attr01 =
					(OBJ_MATRIX(drawOrder[idx]) |
						OBJ_X((dieX + 632) & OBJ_X_MASK) |
						((dieY + 352) & 255)) |
					OBJ_SIZE_16X16 | OBJ_256_COLORS | OBJ_DOUBLE_SIZE |
					OBJ_AFFINE;
				sprs->oam[sprCnt].attr2 =
					(LuckyDiceFaceTiles[rollingDie->variant /
						 DICE_ROLL_PER_TILE]
						<< 3) |
					OBJ_PRIORITY(1);
				sprCnt++;
			}
		}

		/* While aiming, a marker under each die. */
		if(state == LUCKY_DICE_AIM) {
			for(idx = 0; idx != DICE_COUNT; idx++) {
				syConvert3Dto2D(
					(const FIXED *)&diceState->particles[idx], screen);
				x = screen[0] -
					8 * Q8_ONE /
						(sprDepth[drawOrder[idx]] / 2 + DICE_NEAREST_SCALE);
				y = screen[1] +
					3 * Q8_ONE /
						(sprDepth[drawOrder[idx]] / 2 + DICE_NEAREST_SCALE);
				sprs->oam[sprCnt].attr01 = OBJ_POSITION(x + 632, y + 352) |
					OBJ_SIZE_32X32 | OBJ_256_COLORS;
				if(idx == 1) {
					sprs->oam[sprCnt].attr01 |= OBJ_HFLIP;
				}
				sprs->oam[sprCnt].attr2 =
					OBJ_PRIORITY(2) | DICE_AIM_MARKER_TILE;
				sprCnt++;
			}
		}

		for(idx = 0; idx != DICE_COUNT; idx++) {
			Sint32 shadowX;
			Sint32 shadowY;

			/* Each die's shadow on the board below it. */
			SET_VECTOR(groundPoint, diceState->particles[idx].x, 0,
				diceState->particles[idx].z);
			syConvert3Dto2D(groundPoint, screen);
			/* A shadow shrinks as its die rises. */
			if(screen[2] < DICE_NEAREST_DEPTH) {
				screen[2] = DICE_NEAREST_DEPTH;
			}
			if(screen[2] > DICE_FARTHEST_DEPTH) {
				screen[2] = DICE_FARTHEST_DEPTH;
			}
			sprDepth[2 + idx] = screen[2] - DICE_NEAREST_DEPTH;
			sprDepth[2 + idx] += FIXEDtoINT(diceState->particles[idx].y);
			shadowX = screen[0];
			shadowY = screen[1];
			if(shadowY + 88 > -16) {
				sprs->oam[sprCnt].attr01 =
					(OBJ_MATRIX(idx + 2) |
						OBJ_X((shadowX + 624) & OBJ_X_MASK)) |
					((shadowY + 344) & 255) | OBJ_SIZE_16X16 | OBJ_256_COLORS |
					OBJ_DOUBLE_SIZE | OBJ_AFFINE;
				sprs->oam[sprCnt].attr2 = OBJ_PRIORITY(2) | DICE_SHADOW_TILE;
				sprCnt++;
			}
		}

		/* Rolling: move, collide and settle each die. */
		if(state == LUCKY_DICE_ROLL) {
			Sint32 stoppedDice;

			stoppedDice = 0;
			for(idx = 0; idx != DICE_COUNT; idx++) {
				rollingDie = &diceState->particles[idx];
				/* Move, and push the dice apart when they touch (closer than
				   50). */
				rollingDie->x += rollingDie->velX;
				rollingDie->y += rollingDie->velY;
				rollingDie->z += rollingDie->velZ;
				for(otherIdx = 0; otherIdx != DICE_COUNT; otherIdx++) {
					otherDie = &diceState->particles[otherIdx];
					if(idx != otherIdx) {
						separationX = FIXEDtoINT(rollingDie->x - otherDie->x);
						separationY = FIXEDtoINT(rollingDie->y - otherDie->y);
						separationZ = FIXEDtoINT(rollingDie->z - otherDie->z);
						if(separationX * separationX +
								separationY * separationY +
								separationZ * separationZ <=
							DICE_TOUCH_DISTANCE * DICE_TOUCH_DISTANCE - 1) {
							separation =
								sySquartIWRAM(separationX * separationX +
									separationY * separationY +
									separationZ * separationZ);
							/* Touching dice push each other apart, half a pixel
							   a tick along the line between them. */
							/* UB: coincident dice divide by zero. */
#if FIX_DICE_DIVIDE_BY_ZERO
							if(separation != 0)
#endif
							{
								rollingDie->velX +=
									separationX * (toFIXED(1) / 2) / separation;
								rollingDie->velY +=
									separationY * (toFIXED(1) / 2) / separation;
								rollingDie->velZ +=
									separationZ * (toFIXED(1) / 2) / separation;
							}
						}
					}
				}
				/* Bounce off the board's sides and back wall. */
				if(rollingDie->x < -toFIXED(200)) {
					rollingDie->x = -toFIXED(200);
					rollingDie->velX = -rollingDie->velX * 48 / DAMPING_SCALE;
				}
				if(rollingDie->x > toFIXED(200)) {
					rollingDie->x = toFIXED(200);
					rollingDie->velX = -rollingDie->velX * 48 / DAMPING_SCALE;
				}
				if(rollingDie->z < -toFIXED(150)) {
					rollingDie->z = -toFIXED(150);
					rollingDie->velZ = -rollingDie->velZ * 48 / DAMPING_SCALE;
				}
				if(rollingDie->z > rollingDie->y + toFIXED(180)) {
					rollingDie->z = rollingDie->y + toFIXED(180);
					if(rollingDie->velZ > 0) {
						rollingDie->velY += rollingDie->velZ / 2;
						rollingDie->velZ =
							-rollingDie->velZ * 48 / DAMPING_SCALE;
					}
				}
				/* Gravity; on the board a die bounces, loses speed, sometimes
				   kicks off in a random direction, and spins less. */
				rollingDie->velY += -toFIXED(1) / 2;
				if(rollingDie->y <= 0) {
					rollingDie->y = 0;
					rollingDie->velY = -rollingDie->velY * 48 / DAMPING_SCALE;
					if((syRandom() & 3) == 0) {
						Sint32 bounceVelX;
						Sint32 bounceVelZ;

						bounceVelX = rollingDie->velX >> 8;
						bounceVelZ = rollingDie->velZ >> 8;
						bounceSpeed = sySquartIWRAM(bounceVelX * bounceVelX +
										  bounceVelZ * bounceVelZ) *
							56 / DAMPING_SCALE;
						bounceAng = syRandom() & TRIG_ANGLE_MASK;
						rollingDie->velX = sySin(bounceAng) * bounceSpeed >> 8;
						rollingDie->velZ = syCos(bounceAng) * bounceSpeed >> 8;
					} else {
						rollingDie->velX =
							rollingDie->velX * 56 / DAMPING_SCALE;
						rollingDie->velZ =
							rollingDie->velZ * 56 / DAMPING_SCALE;
					}
					if(spinRate[idx] >= DICE_BOUNCE_SPIN_LOSS) {
						spinRate[idx] -= DICE_BOUNCE_SPIN_LOSS;
					}
					if(rollingDie->velY < toFIXED(3)) {
						SET_VELOCITY(rollingDie, 0, 0, 0);
						spinRate[idx] = 0;
						rollAng = rollingDie->variant;
						faceTile =
							rollAng / DICE_ROLL_PER_TILE % DICE_TILES_PER_FACE;
						if(faceTile == 1) {
							rollingDie->variant =
								rollAng + 2 * DICE_ROLL_PER_TILE;
						} else if(faceTile == 2) {
							rollingDie->variant = rollAng + DICE_ROLL_PER_TILE;
						}
					}
				}
				SET_VELOCITY(rollingDie, rollingDie->velX * 63 / DAMPING_SCALE,
					rollingDie->velY * 63 / DAMPING_SCALE,
					rollingDie->velZ * 63 / DAMPING_SCALE);
				/* The roll angle wraps after six faces, at DICE_ROLL_TURN. */
				rollingDie->variant += spinRate[idx];
				if(rollingDie->variant > DICE_ROLL_TURN - 1) {
					rollingDie->variant -= DICE_ROLL_TURN;
				}
				if(spinRate[idx] > 0) {
					spinRate[idx]--;
				}
				/* At rest: read the face from the roll and the row from the
				   depth. */
				if(rollingDie->velX == 0 && rollingDie->velY == 0 &&
					rollingDie->velZ == 0 && rollingDie->y == 0) {
					numbers[idx] = rollingDie->variant / DICE_ROLL_PER_FACE;
					for(otherIdx = 0; otherIdx != DICE_FACE_COUNT; otherIdx++) {
						/* Row boundaries include negative signed halfwords. */
						Sint32 rowDepth;

						rowDepth = toFIXED(LuckyDiceFaceHeights[otherIdx]);
						if(rollingDie->z > rowDepth) {
							numbers[idx + 2] = otherIdx;
							break;
						}
					}
					stoppedDice++;
				}
			}

			/* Both dice stopped: score the four numbers. */
			if(stoppedDice == DICE_COUNT) {
				state = LUCKY_DICE_RESULT;
				resultTmr = 0;
				resultKind = DICE_RESULT_NONE;
				winnings = 0;
				/* DEAD: a count nothing reads, run down above; the ROM keeps
				   the store. */
				payoutTmr = 60;
				if(numbers[0] == numbers[1] && numbers[0] == numbers[2] &&
					numbers[0] == numbers[3]) {
					flashTmr[0] = flashTmr[1] = DICE_PERFECT_FLASH_TICKS;
					resultKind = DICE_RESULT_PERFECT;
					if(numbers[0] == luckyNumbers[0]) {
						winnings = bet *
							(DICE_PERFECT_PAYOUT * DICE_FIRST_LUCKY_MULTIPLIER);
					} else if(numbers[0] == luckyNumbers[1]) {
						winnings = bet *
							(DICE_PERFECT_PAYOUT *
								DICE_SECOND_LUCKY_MULTIPLIER);
					} else {
						winnings = bet * DICE_PERFECT_PAYOUT;
					}
					mnTextDrawNumInWindowStub(
						MainRoutineStateWork.holdings.coins, DICE_COIN_DIGITS,
						sprs->coinsWin, 0, 0);
					sySndPlayStub(MUS_SUPER_LUCKY);
				} else {
					pairCnt = 0;
					for(idx = 0; idx != DICE_RESULT_COUNT - 1; idx++) {
						for(otherIdx = idx + 1; otherIdx != DICE_RESULT_COUNT;
							otherIdx++) {
							if(numbers[idx] == numbers[otherIdx]) {
								matchedNumbers[pairCnt] = numbers[idx];
								pairCnt++;
							}
						}
					}
					if(pairCnt == 1) {
						winnings = bet;
						mnTextDrawNumInWindowStub(
							MainRoutineStateWork.holdings.coins,
							DICE_COIN_DIGITS, sprs->coinsWin, 0, 0);
						resultKind = DICE_RESULT_ONE_PAIR;
					}
					if(pairCnt == 2) {
						winnings = bet * 2;
						mnTextDrawNumInWindowStub(
							MainRoutineStateWork.holdings.coins,
							DICE_COIN_DIGITS, sprs->coinsWin, 0, 0);
						flashTmr[syRandom() & 1] = DICE_FLASH_TICKS;
						resultKind = DICE_RESULT_TWO_PAIR;
						sySndPlayStub(MUS_LUCKY);
					}
					if(pairCnt == 3) {
						winnings = bet * 3;
						mnTextDrawNumInWindowStub(
							MainRoutineStateWork.holdings.coins,
							DICE_COIN_DIGITS, sprs->coinsWin, 0, 0);
						flashTmr[syRandom() & 1] = DICE_FLASH_TICKS;
						resultKind = DICE_RESULT_TRIPLE;
						sySndPlayStub(MUS_VERY_LUCKY);
					}
				}
			}
		}

		/* Hide the unused sprites, write the four affine matrices (one per die
		   and shadow, scaled by depth) and copy the sprites to OAM. */
		for(; sprCnt != OAM_SIZE / sizeof(OAM_WORDS); sprCnt++) {
			sprs->oam[sprCnt].attr01 =
				OBJ_X(DISPLAY_WIDTH) | OBJ_SIZE_16X16 | OBJ_256_COLORS;
			sprs->oam[sprCnt].attr2 = 0;
		}

		/* OAM keeps affine matrix n in the fourth halfwords of entries 4n to 4n
		   + 3, which attr2's high half covers here. */
		for(idx = 0; idx != DICE_RESULT_COUNT; idx++) {
			/* The sprite's matrix: its scale on the diagonal and none across,
			   each element stored through its address. */
			Uint16 affineA;
			Uint16 affineB;
			Uint16 affineC;
			Uint16 affineD;

			affineScale = sprDepth[idx] / 2 + DICE_NEAREST_SCALE;
			*&affineA = affineScale;
			*&affineB = 0;
			*&affineC = 0;
			*&affineD = affineScale;
			sprs->oam[idx * 4 + 0].attr2 |= OBJ_AFFINE_PARAMETER(affineA);
			sprs->oam[idx * 4 + 1].attr2 |= OBJ_AFFINE_PARAMETER(affineB);
			sprs->oam[idx * 4 + 2].attr2 |= OBJ_AFFINE_PARAMETER(affineC);
			sprs->oam[idx * 4 + 3].attr2 |= OBJ_AFFINE_PARAMETER(affineD);
		}

		syDMAXCopy(sprs->oam, OAM, DMA_COPY_WORDS(OAM_SIZE / 4), REG_DMA3);
		sySynch(1);
		frm++;
	}

	/* Fade out and free the state blocks. */
	for(idx = 0; idx != DICE_FADE_TICKS + 1; idx++) {
		scale = toFIXED(1) - idx * (toFIXED(1) / DICE_FADE_TICKS);
		fdPaletteScaleRGB555(
			sprPalette, (Uint16 *)OBJ_PLTT, scale, BG_PLTT_COLORS);
		fdPaletteScaleRGB555(
			bgPalette, (Uint16 *)BG_PLTT, scale, BG_PLTT_COLORS);
		sySynch(1);
	}
	syHeapSlotRelease(HEAP_SLOT_CAMERA);
	syHeapSlotRelease(HEAP_SLOT_LUCKY_GAME);
	syHeapSlotRelease(HEAP_SLOT_BATTLE_CANVAS);
	syHeapSlotRelease(HEAP_SLOT_BATTLE_EFFECT);
	syHeapSlotRelease(HEAP_SLOT_BATTLE_SHEET);
}
