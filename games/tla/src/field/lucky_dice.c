/* The Lucky Dice minigame: its tables, its start, its helpers and its run. */

#include "field/field.h"
#include "field/dice_game.h"
#include "game/game.h"
#include "game/system_call.h"
#include "menu/menu.h"
#include "battle/battle_camera.h"
#include "macros.h"
#include "game/stubs.h"
#include "window/stubs.h"
#include "text/msg_ids.h"

/* The help window's two lines, as messages after MsgLuckyDiceCoins. */
typedef enum LUCKY_DICE_HELP_MESSAGE {
	DICE_HELP_THROW = 10, /* ":Throw" after its button */
	DICE_HELP_QUIT = 11	  /* ":Quit" after its button */
} LUCKY_DICE_HELP_MESSAGE;

/* What a throw's four numbers make, in the rules' words: "If two numbers match
   on either the dice or the table... it's called a pair, and you don't win or
   lose anything. Two pair doubles your money. Triple matches triple your money.
   Four of a kind is perfect, and you win five times your bet." Every result has
   its lettering (hand_titles.png: "Perfect!!!", "Triple!", "Two Pair!" and "One
   Pair!"), numbered from 0. */
typedef enum DICE_RESULT {
	DICE_RESULT_NONE = -1, /* no two numbers alike: the bet is lost */
	DICE_RESULT_PERFECT, /* all four alike: DICE_PERFECT_PAYOUT times the bet */
	DICE_RESULT_TRIPLE,	 /* three alike: three times the bet */
	DICE_RESULT_TWO_PAIR, /* two pairs: twice the bet */
	DICE_RESULT_ONE_PAIR  /* one pair: the bet back */
} DICE_RESULT;

/* Unused: a table of signs. */
const Sint32 LuckyDiceUnusedSigns[] = {-1, 1, -1, 1, -1, -1, 1, 1};

/* How many sprites each result's lettering takes, 32 pixels each. */
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

/* Starts the Lucky Dice minigame: leaves only the sprites on screen, starts the
   music and plays until the player quits. Returns 0. */
Sint32 fdLuckyDiceStart(void)
{
	REG_DISPCNT = DISPCNT_OBJ_1D_MAP;
	sySoundPlayStub(MUS_ALHAFRA);
	fdLuckyDiceRun();
	return 0;
}

/* Puts the 3D camera back at its starting view: at the origin, unturned,
   pitched to 152/256 of a turn and 510 pixels away, following nothing. */
void fdCameraResetViewDefaults(void)
{
	BATTLE_CAMERA *camera = RamHeapSlots->cameraState;
	Sint32		   offset[3];
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

	SET_VECTOR(offset, 0, 0, camera->distance);
	syCalcPoint(offset, camera->eye);

	nearPlane = syDivFX(toFIXED(250), toFIXED(3) / 4);
	sySetScreenDist(toFIXED(250), nearPlane, toFIXED(32767));
}

/* Unused: multiplies two 8.8 fixed-point values, rounding toward zero. */
UNUSED Sint32 fdFixedPointMultiplyQ8(Sint32 a, Sint32 b)
{
	Sint16 left = a;
	Sint16 right = b;
	Sint32 product;
	Sint32 adjusted;
	Sint32 multiplier;

	/* Rounded toward zero, as a division rounds. */
	multiplier = right;
	product = left;
	product *= multiplier;
	if(product >= 0) {
		adjusted = product;
	} else {
		adjusted = product + (Q_8_8(1) - 1);
	}
	return (Sint16)(adjusted >> 8);
}

/* Unused: divides two 8.8 fixed-point values. */
UNUSED Sint16 fdFixedPointDivQ8(Sint16 numerator, Sint16 denominator)
{
	return (numerator * Q_8_8(1)) / denominator;
}

/* Unused: the reciprocal of an 8.8 fixed-point value. */
UNUSED Sint16 fdFixedPointReciprocal(Sint16 value)
{
	return toFIXED(1) / value;
}

/* Copies count RGB555 colours from source to destination with each channel
   multiplied by scale (16.16: 0 is black, toFIXED(1) unchanged), for fades. */
Sint32 fdLuckyDiceScaleRGB555Buffer(
	Uint16 *src, Uint16 *dest, FIXED scale, Sint32 count)
{
	Sint32 remaining;
	Uint32 pixel;
	Uint32 red;
	Uint32 green;
	Uint32 blue;

	if(count > 0) {
		Uint32 redMask = RGB(31, 0, 0);
		Uint32 greenMask = RGB(0, 31, 0);
		Uint32 blueMask = RGB(0, 0, 31);

		remaining = count;
		do {
			pixel = *src;
			red = pixel & redMask;
			green = pixel & greenMask;
			blue = blueMask & pixel;
			red *= scale;
			green *= scale;
			blue *= scale;
			pixel =
				(FIXEDtoINT(red) & redMask) | (FIXEDtoINT(green) & greenMask);
			pixel |= FIXEDtoINT(blue) & blueMask;
			*dest = pixel;
			src++;
			dest++;
			remaining--;
		} while(remaining != 0);
	}
	return 0;
}

/* Lucky Dice's layout, fitted to each language's text: the coins window at the
   top and its label, the help window (Throw / Quit) and the winnings window at
   the bottom with their text, in tiles for windows and pixels within them, and
   the centre of the result lettering in sprite x, which wraps at 512. */
#define DICE_COINS_WINDOW_X		BY_LANGUAGE(18, 18, 16, 17, 16, 17)
#define DICE_COINS_WINDOW_WIDTH BY_LANGUAGE(11, 12, 14, 13, 14, 13)
#define DICE_COINS_LABEL_X		BY_LANGUAGE(48, 48, 53, 56, 53, 56)

/* The help window (Throw / Quit): its column and width, in tiles. */
#define DICE_HELP_WINDOW_X	   BY_LANGUAGE(23, 22, 20, 20, 21, 22)
#define DICE_HELP_WINDOW_WIDTH BY_LANGUAGE(7, 8, 10, 10, 9, 8)

/* The help window reopened after a payout; in Spanish it is a tile narrower
   than the one Lucky Dice opens with, for the same two lines. */
#define DICE_REOPENED_HELP_WINDOW_X		SPANISH_OR(21, DICE_HELP_WINDOW_X)
#define DICE_REOPENED_HELP_WINDOW_WIDTH SPANISH_OR(9, DICE_HELP_WINDOW_WIDTH)

/* The winnings window opens at this x less half its digits and is this width
   plus its digits; after the winnings label the number stands at
   DICE_WINNINGS_NUMBER_X, and the coins label at DICE_WINNINGS_COINS_X past the
   number's digits. */
#define DICE_WINNINGS_WINDOW_X	   BY_LANGUAGE(10, 8, 7, 9, 9, 9)
#define DICE_WINNINGS_WINDOW_WIDTH BY_LANGUAGE(10, 13, 14, 13, 13, 13)
#define DICE_WINNINGS_COINS_X	   BY_LANGUAGE(40, 56, 53, 64, 53, 64)
#define DICE_WINNINGS_NUMBER_X	   BY_LANGUAGE(40, 56, 53, 56, 53, 56)

/* The centre of the result lettering, in sprite x, which wraps at 512. */
#define DICE_RESULT_LABEL_CENTER_X JAPANESE_OR(640, 632)

/* The dice thrown, and the sprites drawn for them: the dice, then their
   shadows. */
#define DICE_COUNT				 2
#define DICE_RESULT_COUNT		 4
/* The result: one number every 5 ticks, then 20 ticks in the lettering, the
   winnings, and A to go on; the payout closes 15 ticks after it starts. */
#define DICE_NUMBER_REVEAL_TICKS 5
#define DICE_RESULT_TICKS		 20
#define DICE_PAYOUT_TICKS		 15

/* Two dice whose centres come closer than this push each other apart. */
#define DICE_TOUCH_DISTANCE			 50
/* Where each picture's sprite tiles start, in OAM's 32-byte steps (two to a
   256-colour tile): the rolling die's views from 0, its shadow at
   DICE_SHADOW_TILE (field/field.h), the big numbers (the result's lettering
   counted on from them), the aiming hand, the Perfect Bonus sign, its 3, 5 and
   times sign, and the lucky numbers' small faces. */
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

/* The board's picture begins with its 224 colours, the sprites' with all 256,
   each before its tiles. */
#define DICE_BOARD_PALETTE_COLORS	224
#define DICE_SPRITES_PALETTE_COLORS 256

/* What the Lucky Dice screen is doing. */
typedef enum LUCKY_DICE_STATE {
	LUCKY_DICE_AIM,	   /* the dice wait to be thrown; Up and Down raise them */
	LUCKY_DICE_ROLL,   /* the dice fly, bounce and settle */
	LUCKY_DICE_RESULT, /* the four numbers and the result are shown */
	LUCKY_DICE_PAYOUT  /* the winnings move into the coins */
} LUCKY_DICE_STATE;

/* Runs the Lucky Dice minigame until the player quits or cannot afford a throw.
   Each throw costs ten coins per level of the party's average. */
void fdLuckyDiceRun(void)
{
	Uint16 bgPalette[256];
	/* the two dice, then their shadows */
	Sint32 spriteDepth[DICE_RESULT_COUNT];
	/* The two faces, then the two rows the dice stopped in. */
	Sint32 numbers[DICE_RESULT_COUNT];
	Sint32 groundPoint[3];
	Sint32 screen[3];
	/* LEFTOVER(frame): an unused local the ROM's frame reserves and never
	 * touches; The Broken Seal's fdLuckyDiceRun declares the same unused[8]. */
	UNUSED Sint32 unused[8];
	Sint32		  spinRate[DICE_COUNT];
	Sint32 flashTimer[DICE_COUNT]; /* set on a win; nothing here reads it */
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
	Uint16		   *spritePalette;
	DICE_STATE	   *diceState;
	LUCKY_DICE_SPR *sprites;
	BATTLE_CAMERA  *camera;
	Sint32			frame;
	Sint32			state;
	Sint32			bet;
	Sint32			resultTimer;
	Sint32			resultKind;
	Sint32			winnings;
	Sint32			payoutTimer;
	/* The winnings' digit count, kept to clear the same field in the payout. */
	Sint32		   digits;
	Sint32		   spriteCount;
	DICE_PARTICLE *rollingDie;
	DICE_PARTICLE *otherDie;
	Sint32		   resetDice; /* put the dice back at the throwing line */
	WINDOW		  *window;
	Sint32		   index;
	Sint32		   otherIndex;
	Sint32 rollAngle; /* a die's roll angle, in DICE_ROLL_PER_TILE steps */
	Sint32 x;
	Sint32 y;
	Sint32 halfSize;
	Sint32 faceTile; /* which of a face's three tiles the roll shows */
	Sint32 sortedDie;
	Sint32 tileX;
	Sint32 tileY;
	Sint32 resourceAddr;
	Sint32 separationX;
	Sint32 separationY;
	Sint32 separationZ;
	Sint32 separation;
	Sint32 bounceSpeed;
	Uint32 bounceAngle;
	FIXED  scale;
	Sint32 pairCount;
	Sint16 affineScale;

	syHeapSlotAllocate(HEAP_SLOT_BATTLE_SHEET, 1550);
	spritePalette = syHeapSlotAllocate(
		HEAP_SLOT_BATTLE_CANVAS, DICE_SPRITES_PALETTE_COLORS * sizeof(Uint16));
	diceState = syHeapSlotAllocateEWRAM(HEAP_SLOT_BATTLE_EFFECT, 31612);
	sprites = syHeapSlotAllocateEWRAM(HEAP_SLOT_CANVAS_MAP, 1556);
	camera = syHeapSlotAllocateEWRAM(HEAP_SLOT_CAMERA, sizeof(*camera));
	syResourceReservedNoOperation(RESOURCE_BATTLE_EFFECT_STUBS);
	fdCameraResetViewDefaults();
	syInitEvent();
	OamCopyEnabled = 0;

	/* Fill BG1's map (screen block 6) with a 30-tile-wide picture, then load
	   the board and dice graphics with their palettes faded to black. */
	index = 0;
	for(tileY = 0; tileY != DISPLAY_HEIGHT / 8; tileY++) {
		for(tileX = 0; tileX != 32; tileX++) {
			*(volatile Uint16 *)(BG_SCREEN_BLOCK(6) + index) =
				tileX + tileY * (DISPLAY_WIDTH / 8);
			index += 2;
		}
	}

	resourceAddr = (Sint32)syResourceGetTableEntry(RESOURCE_LUCKY_DICE_BOARD);
	syDMAXCopy((void *)resourceAddr, (void *)BG_PLTT,
		DMA_COPY_WORDS(DICE_BOARD_PALETTE_COLORS / 2), REG_DMA3);
	syDMAXCopy((void *)BG_PLTT, bgPalette, DMA_COPY_WORDS(sizeof bgPalette / 4),
		REG_DMA3);
	resourceAddr += DICE_BOARD_PALETTE_COLORS * 2;
	syFieldDecompressTaggedLZ((const void *)resourceAddr, RamMapMetatileBuffer);
	syDMAXCopy(RamMapMetatileBuffer, (void *)BG_CHAR_BLOCK(1),
		DMA_COPY_WORDS(9600), REG_DMA3);
	resourceAddr = (Sint32)syResourceGetTableEntry(RESOURCE_LUCKY_DICE_SPRITES);
	syDMAXCopy((void *)resourceAddr, spritePalette,
		DMA_COPY_WORDS(DICE_SPRITES_PALETTE_COLORS / 2), REG_DMA3);
	syDMAXCopy((void *)resourceAddr, (void *)OBJ_PLTT,
		DMA_COPY_WORDS(DICE_SPRITES_PALETTE_COLORS / 2), REG_DMA3);
	resourceAddr += DICE_SPRITES_PALETTE_COLORS * 2;
	syFieldDecompressTaggedLZ((const void *)resourceAddr, RamMapMetatileBuffer);
	syDMAXCopy(RamMapMetatileBuffer, (void *)OBJ_VRAM0, DMA_COPY_WORDS(7936),
		REG_DMA3);
	fdLuckyDiceScaleRGB555Buffer(
		spritePalette, (Uint16 *)OBJ_PLTT, 0, OBJ_PLTT_COLORS);
	fdLuckyDiceScaleRGB555Buffer(
		bgPalette, (Uint16 *)BG_PLTT, 0, BG_PLTT_COLORS);

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

	spriteCount = 0;
	/* Start aiming with both dice at the throwing line and two lucky
	   numbers. */
	state = LUCKY_DICE_AIM;
	resultTimer = 0;
	resultKind = DICE_RESULT_NONE;
	winnings = 0;
	payoutTimer = 0;
	/* A random place and speed for each die: the throwing line below takes the
	   places, and a throw the speeds. */
	for(index = 0; index != DICE_COUNT; index++) {
		diceState->particles[index].x = ((syRandom() & 127) - 64) << 16;
		diceState->particles[index].y = (syRandom() & 31) << 16;
		diceState->particles[index].z = ((syRandom() & 127) - 64) << 16;
		diceState->particles[index].velocityX = ((syRandom() & 127) - 64) << 14;
		diceState->particles[index].velocityY = ((syRandom() & 31) + 32) << 12;
		diceState->particles[index].velocityZ = ((syRandom() & 127) - 64) << 14;
		diceState->particles[index].variant = 0;
		spinRate[index] = DICE_THROW_SPIN;
	}
	numbers[0] = numbers[1] = numbers[2] = numbers[3] = 0;
	flashTimer[0] = flashTimer[1] = 0;
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
	window = mnWindowCreateStub(
		DICE_COINS_WINDOW_X, 0, DICE_COINS_WINDOW_WIDTH, 3, 6);
	sprites->coinsWindow = window;
	mnTextDrawMessageAsTilesStub(
		(Sint32)&MsgLuckyDiceCoins, window, DICE_COINS_LABEL_X, 0);
	mnTextDrawNumberInWindowStub(MainRoutineStateWork.coins, DICE_COIN_DIGITS,
		(Sint32)sprites->coinsWindow, 0, 0);
	window = mnWindowCreateStub(
		DICE_HELP_WINDOW_X, 16, DICE_HELP_WINDOW_WIDTH, 4, 6);
	sprites->helpWindow = window;
	mnTextDrawMessageAsTilesStub(
		(Sint32)&MsgLuckyDiceCoins + DICE_HELP_THROW, window, 0, 0);
	mnTextDrawMessageAsTilesStub(
		(Sint32)&MsgLuckyDiceCoins + DICE_HELP_QUIT, sprites->helpWindow, 0, 8);

	frame = 0;
	for(;;) {
		resetDice = 0;
		if(frame <= DICE_FADE_TICKS) {
			fdLuckyDiceScaleRGB555Buffer(spritePalette, (Uint16 *)OBJ_PLTT,
				frame * (toFIXED(1) / DICE_FADE_TICKS), OBJ_PLTT_COLORS);
			fdLuckyDiceScaleRGB555Buffer(bgPalette, (Uint16 *)BG_PLTT,
				frame * (toFIXED(1) / DICE_FADE_TICKS), BG_PLTT_COLORS);
		}
		syInitMatrix();
		syLookAtLoad(camera->eye, camera->focus);

		/* The fixed sprites: the Perfect Bonus sign with its times 3 and times
		   5, and the two lucky numbers beside them. */
		spriteCount = 0;
		sprites->oam[spriteCount].attr01 = OBJ_SIZE_64X32 | OBJ_256_COLORS;
		sprites->oam[spriteCount].attr2 = DICE_BONUS_SIGN_TILE;
		spriteCount++;
		sprites->oam[spriteCount].attr01 =
			OBJ_POSITION(16, 32) | OBJ_SIZE_16X16 | OBJ_256_COLORS;
		sprites->oam[spriteCount].attr2 = DICE_THREE_TILE;
		spriteCount++;
		sprites->oam[spriteCount].attr01 =
			OBJ_POSITION(16, 48) | OBJ_SIZE_16X16 | OBJ_256_COLORS;
		sprites->oam[spriteCount].attr2 = DICE_FIVE_TILE;
		spriteCount++;
		sprites->oam[spriteCount].attr01 =
			OBJ_POSITION(4, 32) | OBJ_SIZE_16X16 | OBJ_256_COLORS;
		sprites->oam[spriteCount].attr2 = DICE_TIMES_TILE;
		spriteCount++;
		sprites->oam[spriteCount].attr01 =
			OBJ_POSITION(4, 48) | OBJ_SIZE_16X16 | OBJ_256_COLORS;
		sprites->oam[spriteCount].attr2 = DICE_TIMES_TILE;
		spriteCount++;
		for(index = 0; index != DICE_COUNT; index++) {
			sprites->oam[spriteCount].attr01 =
				OBJ_X(32) | (32 + index * 16) | OBJ_SIZE_16X16 | OBJ_256_COLORS;
			sprites->oam[spriteCount].attr2 =
				luckyNumbers[index] * SPRITE_TILES_8BPP(16, 16) +
				DICE_LUCKY_NUMBER_TILES;
			spriteCount++;
		}

		/* After a throw, reveal the four numbers five ticks apart, then the
		   result's lettering. */
		if(state == LUCKY_DICE_RESULT || state == LUCKY_DICE_PAYOUT) {
			for(index = 0; index != DICE_RESULT_COUNT; index++) {
				if(resultTimer == index * DICE_NUMBER_REVEAL_TICKS) {
					sySoundPlayStub(SE_LUCKY_DICE_NUMBER);
				}
				if(resultTimer >= index * DICE_NUMBER_REVEAL_TICKS) {
					sprites->oam[spriteCount].attr01 = 64 | OBJ_SIZE_32X32 |
						OBJ_256_COLORS | OBJ_X(56 + index * 32);
					sprites->oam[spriteCount].attr2 =
						numbers[index] * SPRITE_TILES_8BPP(32, 32) +
						DICE_NUMBER_TILES;
					spriteCount++;
				}
			}
			if(resultTimer >= DICE_RESULT_TICKS && resultKind >= 0) {
				for(index = 0; index != LuckyDiceLabelCounts[resultKind];
					index++) {
					Sint32 labelX;
					Sint32 labelY = 96;

					labelX = DICE_RESULT_LABEL_CENTER_X -
						LuckyDiceLabelCounts[resultKind] * 16 + index * 32;
					labelY = 96;
					sprites->oam[spriteCount].attr01 =
						OBJ_POSITION(labelX, labelY) | OBJ_SIZE_32X32 |
						OBJ_256_COLORS;
					sprites->oam[spriteCount].attr2 =
						LuckyDiceLabelTiles[resultKind] *
							SPRITE_TILES_8BPP(16, 16) +
						DICE_NUMBER_TILES + index * SPRITE_TILES_8BPP(32, 32);
					spriteCount++;
				}
			}
		}

		/* Showing the result: twenty ticks in, pay the winnings and open their
		   window; A then moves on to the payout, or back to aiming after a
		   loss. */
		if(state == LUCKY_DICE_RESULT) {
			if(resultTimer == DICE_RESULT_TICKS && winnings > 0) {
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
				window = mnWindowCreateStub(DICE_WINNINGS_WINDOW_X - digits / 2,
					16, digits + DICE_WINNINGS_WINDOW_WIDTH, 3, 6);
				sprites->winningsWindow = window;
				mnTextDrawMessageAsTilesStub(
					(Sint32)&MsgLuckyDiceWinnings, window, 0, 0);
				/* "Coins" after the number (MsgLuckyDiceCoins, one before). */
				mnTextDrawMessageAsTilesStub((Sint32)&MsgLuckyDiceWinnings - 1,
					sprites->winningsWindow, digits * 8 + DICE_WINNINGS_COINS_X,
					0);
				mnTextDrawNumberInWindowStub(winnings, digits,
					(Sint32)sprites->winningsWindow, DICE_WINNINGS_NUMBER_X, 0);
			}
			if(payoutTimer > 0) {
				payoutTimer--;
			}
			if(resultTimer >= DICE_RESULT_TICKS && (PerWork.repeat & KEY_A)) {
				sySoundPlayStub(SE_MENU_CONFIRM);
				if(winnings > 0) {
					state = LUCKY_DICE_PAYOUT;
					payoutTimer = 0;
				} else {
					resetDice = 1;
					state = LUCKY_DICE_AIM;
				}
			}
			resultTimer++;
		} else if(state == LUCKY_DICE_PAYOUT) {
			/* Clear the winnings into the coins, then DICE_PAYOUT_TICKS later
			   close their window and aim again. */
			if(payoutTimer == 0) {
				winnings = 0;
				mnTextDrawNumberInWindowStub(0, digits,
					(Sint32)sprites->winningsWindow, DICE_WINNINGS_NUMBER_X, 0);
				mnTextDrawNumberInWindowStub(MainRoutineStateWork.coins,
					DICE_COIN_DIGITS, (Sint32)sprites->coinsWindow, 0, 0);
			}
			if(payoutTimer == DICE_PAYOUT_TICKS) {
				state = LUCKY_DICE_AIM;
				mnWindowCloseStub(sprites->winningsWindow, 1);
				window = mnWindowCreateStub(DICE_REOPENED_HELP_WINDOW_X, 16,
					DICE_REOPENED_HELP_WINDOW_WIDTH, 4, 6);
				sprites->helpWindow = window;
				mnTextDrawMessageAsTilesStub(
					(Sint32)&MsgLuckyDiceCoins + DICE_HELP_THROW, window, 0, 0);
				mnTextDrawMessageAsTilesStub(
					(Sint32)&MsgLuckyDiceCoins + DICE_HELP_QUIT,
					sprites->helpWindow, 0, 8);
				resetDice = 1;
			}
			payoutTimer++;
		} else if(state == LUCKY_DICE_AIM && frame > DICE_FADE_TICKS + 1) {
			/* Aiming, a tick after the fade in: Up and Down raise and lower
			   the dice between 32 and 160. B quits, as does running out of
			   coins for a bet; A throws. */
			/* DEAD: PerWork.repeat is read twice and discarded before the
			   KEY_UP test; the ROM keeps both loads. */
			(void)PerWork.repeat;
			(void)PerWork.repeat;
			if(PerWork.data & KEY_UP) {
				diceState->particles[0].y += toFIXED(4);
				diceState->particles[1].y += toFIXED(4);
			}
			if(PerWork.data & KEY_DOWN) {
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
			if(MainRoutineStateWork.coins <
				(Uint32)(gmPartyGetAverageLevelStub() * DICE_BET_PER_LEVEL)) {
				mnWindowCloseStub(sprites->helpWindow, 1);
				break;
			}
			if(PerWork.repeat & KEY_B) {
				sySoundPlayStub(SE_MENU_CANCEL);
				mnWindowCloseStub(sprites->helpWindow, 1);
				break;
			}
			if(PerWork.repeat & KEY_A) {
				sySoundPlayStub(SE_LUCKY_DICE_THROW);
				mnWindowCloseStub(sprites->helpWindow, 1);
				state = LUCKY_DICE_ROLL;
				bet = gmPartyGetAverageLevelStub() * DICE_BET_PER_LEVEL;
				if((Uint32)bet > MainRoutineStateWork.coins) {
					bet = MainRoutineStateWork.coins;
				}
				gmPartyAdjustCoinsStub(-bet);
				mnTextDrawNumberInWindowStub(MainRoutineStateWork.coins,
					DICE_COIN_DIGITS, (Sint32)sprites->coinsWindow, 0, 0);
				for(index = 0; index != DICE_COUNT; index++) {
					rollingDie = &diceState->particles[index];
					/* DEAD: never true with DICE_COUNT 2 (a third die would
					   start further back and higher up); the ROM keeps the test
					   and its stores. */
					if(index > 1) {
						rollingDie->z = -toFIXED(180);
						rollingDie->y = toFIXED(96);
					}
					rollingDie->velocityX = ((syRandom() & 127) - 64) << 12;
					rollingDie->velocityY = ((syRandom() & 15) + 48) << 13;
					rollingDie->velocityZ =
						(((syRandom() & 63) + 140) << 12) + rollingDie->y / 6;
					rollingDie->variant = syRandom() % DICE_ROLL_TURN;
					spinRate[index] = DICE_THROW_SPIN;
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
		for(index = 0; index != DICE_COUNT; index++) {
			drawOrder[index] = index;
		}
		for(index = 1; index != DICE_COUNT; index++) {
			sortedDie = drawOrder[index];
			for(otherIndex = index - 1; otherIndex >= 0 &&
				diceState->particles[drawOrder[otherIndex]].z >
					diceState->particles[sortedDie].z;
				otherIndex--) {
				drawOrder[otherIndex + 1] = drawOrder[otherIndex];
			}
			drawOrder[otherIndex + 1] = sortedDie;
		}

		for(index = 0; index != DICE_COUNT; index++) {
			Sint32 dieX;
			Sint32 dieY;

			rollingDie = &diceState->particles[drawOrder[index]];
			syConvert3Dto2D((const FIXED *)rollingDie, screen);
			if(screen[2] < DICE_NEAREST_DEPTH) {
				screen[2] = DICE_NEAREST_DEPTH;
			}
			if(screen[2] > DICE_FARTHEST_DEPTH) {
				screen[2] = DICE_FARTHEST_DEPTH;
			}
			spriteDepth[drawOrder[index]] = screen[2] - DICE_NEAREST_DEPTH;
			/* Half a die's 16 pixels, at its scale. */
			halfSize = 8 * AFFINE_SCALE_ONE /
				(spriteDepth[drawOrder[index]] / 2 + DICE_NEAREST_SCALE);
			dieX = screen[0] - halfSize;
			dieY = screen[1] - halfSize;
			/* Each die has its own affine matrix (attr1 bits 9 to 13); 632 and
			   352 wrap in OAM's 9- and 8-bit fields to 120 across and 96
			   down. */
			if(dieY + 96 > -16) {
				sprites->oam[spriteCount].attr01 =
					(OBJ_AFFINE_MATRIX(drawOrder[index]) |
						OBJ_X((dieX + 632) & OBJ_X_MASK) |
						((dieY + 352) & OBJ_Y_MASK)) |
					OBJ_SIZE_16X16 | OBJ_256_COLORS | OBJ_DOUBLE_SIZE |
					OBJ_AFFINE;
				sprites->oam[spriteCount].attr2 =
					(LuckyDiceFaceTiles[rollingDie->variant /
						 DICE_ROLL_PER_TILE]
						<< 3) |
					OBJ_PRIORITY(1);
				spriteCount++;
			}
		}

		/* While aiming, a marker under each die. */
		if(state == LUCKY_DICE_AIM) {
			for(index = 0; index != DICE_COUNT; index++) {
				syConvert3Dto2D(
					(const FIXED *)&diceState->particles[index], screen);
				x = screen[0] -
					8 * AFFINE_SCALE_ONE /
						(spriteDepth[drawOrder[index]] / 2 +
							DICE_NEAREST_SCALE);
				y = screen[1] +
					3 * AFFINE_SCALE_ONE /
						(spriteDepth[drawOrder[index]] / 2 +
							DICE_NEAREST_SCALE);
				sprites->oam[spriteCount].attr01 =
					OBJ_POSITION(x + 632, y + 352) | OBJ_SIZE_32X32 |
					OBJ_256_COLORS;
				if(index == 1) {
					sprites->oam[spriteCount].attr01 |= OBJ_HFLIP;
				}
				sprites->oam[spriteCount].attr2 =
					OBJ_PRIORITY(2) | DICE_AIM_MARKER_TILE;
				spriteCount++;
			}
		}

		for(index = 0; index != DICE_COUNT; index++) {
			Sint32 shadowX;
			Sint32 shadowY;
			Sint32 depth;

			/* Each die's shadow on the board below it. */
			rollingDie = &diceState->particles[index];
			groundPoint[0] = rollingDie->x;
			groundPoint[1] = 0;
			groundPoint[2] = rollingDie->z;
			syConvert3Dto2D(groundPoint, screen);
			/* A shadow shrinks as its die rises. */
			depth = screen[2];
			if(depth < DICE_NEAREST_DEPTH) {
				screen[2] = depth = DICE_NEAREST_DEPTH;
			}
			if(depth > DICE_FARTHEST_DEPTH) {
				screen[2] = DICE_FARTHEST_DEPTH;
				depth = screen[2];
			}
			spriteDepth[2 + index] = depth - DICE_NEAREST_DEPTH;
			spriteDepth[2 + index] += FIXEDtoINT(diceState->particles[index].y);
			shadowX = screen[0];
			shadowY = screen[1];
			if(shadowY + 88 > -16) {
				sprites->oam[spriteCount].attr01 =
					(OBJ_AFFINE_MATRIX(index + 2) |
						OBJ_X((shadowX + 624) & OBJ_X_MASK)) |
					((shadowY + 344) & OBJ_Y_MASK) | OBJ_SIZE_16X16 |
					OBJ_256_COLORS | OBJ_DOUBLE_SIZE | OBJ_AFFINE;
				sprites->oam[spriteCount].attr2 =
					OBJ_PRIORITY(2) | DICE_SHADOW_TILE;
				spriteCount++;
			}
		}

		/* Rolling: move, collide and settle each die. */
		if(state == LUCKY_DICE_ROLL) {
			Sint32 stoppedDice;

			stoppedDice = 0;
			for(index = 0; index != DICE_COUNT; index++) {
				/* Move, and push the dice apart when they touch (closer than
				   DICE_TOUCH_DISTANCE). */
				rollingDie = &diceState->particles[index];
				rollingDie->x += rollingDie->velocityX;
				rollingDie->y += rollingDie->velocityY;
				rollingDie->z += rollingDie->velocityZ;
				for(otherIndex = 0; otherIndex != DICE_COUNT; otherIndex++) {
					otherDie = &diceState->particles[otherIndex];
					if(index != otherIndex) {
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
								rollingDie->velocityX +=
									separationX * (toFIXED(1) / 2) / separation;
								rollingDie->velocityY +=
									separationY * (toFIXED(1) / 2) / separation;
								rollingDie->velocityZ +=
									separationZ * (toFIXED(1) / 2) / separation;
							}
						}
					}
				}
				/* Bounce off the board's sides and back wall. */
				if(rollingDie->x < -toFIXED(200)) {
					rollingDie->x = -toFIXED(200);
					rollingDie->velocityX =
						-rollingDie->velocityX * 48 / DAMPING_SCALE;
				}
				if(rollingDie->x > toFIXED(200)) {
					rollingDie->x = toFIXED(200);
					rollingDie->velocityX =
						-rollingDie->velocityX * 48 / DAMPING_SCALE;
				}
				if(rollingDie->z < -toFIXED(150)) {
					rollingDie->z = -toFIXED(150);
					rollingDie->velocityZ =
						-rollingDie->velocityZ * 48 / DAMPING_SCALE;
				}
				if(rollingDie->z > rollingDie->y + toFIXED(180)) {
					rollingDie->z = rollingDie->y + toFIXED(180);
					if(rollingDie->velocityZ > 0) {
						rollingDie->velocityY += rollingDie->velocityZ / 2;
						rollingDie->velocityZ =
							-rollingDie->velocityZ * 48 / DAMPING_SCALE;
					}
				}
				/* Gravity; on the board a die bounces, loses speed, sometimes
				   kicks off in a random direction, and spins less. */
				rollingDie->velocityY += -(toFIXED(1) / 2);
				if(rollingDie->y <= 0) {
					rollingDie->y = 0;
					rollingDie->velocityY =
						-rollingDie->velocityY * 48 / DAMPING_SCALE;
					if((syRandom() & 3) == 0) {
						Sint32 bounceVelocityX;
						Sint32 bounceVelocityZ;

						bounceVelocityX = rollingDie->velocityX >> 8;
						bounceVelocityZ = rollingDie->velocityZ >> 8;
						bounceSpeed =
							sySquartIWRAM(bounceVelocityX * bounceVelocityX +
								bounceVelocityZ * bounceVelocityZ) *
							56 / DAMPING_SCALE;
						bounceAngle = syRandom() & TRIG_ANGLE_MASK;
						rollingDie->velocityX =
							sySin(bounceAngle) * bounceSpeed >> 8;
						rollingDie->velocityZ =
							syCos(bounceAngle) * bounceSpeed >> 8;
					} else {
						rollingDie->velocityX =
							rollingDie->velocityX * 56 / DAMPING_SCALE;
						rollingDie->velocityZ =
							rollingDie->velocityZ * 56 / DAMPING_SCALE;
					}
					if(spinRate[index] >= DICE_BOUNCE_SPIN_LOSS) {
						spinRate[index] -= DICE_BOUNCE_SPIN_LOSS;
					}
					if(rollingDie->velocityY < toFIXED(3)) {
						rollingDie->velocityX = 0;
						rollingDie->velocityY = 0;
						rollingDie->velocityZ = 0;
						spinRate[index] = 0;
						rollAngle = rollingDie->variant;
						faceTile = rollAngle / DICE_ROLL_PER_TILE %
							DICE_TILES_PER_FACE;
						if(faceTile == 1) {
							rollingDie->variant =
								rollAngle + 2 * DICE_ROLL_PER_TILE;
						} else if(faceTile == 2) {
							rollingDie->variant =
								rollAngle + DICE_ROLL_PER_TILE;
						}
					}
				}
				rollingDie->velocityX =
					rollingDie->velocityX * 63 / DAMPING_SCALE;
				rollingDie->velocityY =
					rollingDie->velocityY * 63 / DAMPING_SCALE;
				rollingDie->velocityZ =
					rollingDie->velocityZ * 63 / DAMPING_SCALE;
				/* The roll angle wraps after six faces, at DICE_ROLL_TURN. */
				rollingDie->variant += spinRate[index];
				if(rollingDie->variant > DICE_ROLL_TURN - 1) {
					rollingDie->variant -= DICE_ROLL_TURN;
				}
				if(spinRate[index] > 0) {
					spinRate[index]--;
				}
				/* At rest: read the face from the roll and the row from the
				   depth. */
				if(rollingDie->velocityX == 0 && rollingDie->velocityY == 0 &&
					rollingDie->velocityZ == 0 && rollingDie->y == 0) {
					numbers[index] = rollingDie->variant / DICE_ROLL_PER_FACE;
					for(otherIndex = 0; otherIndex != DICE_FACE_COUNT;
						otherIndex++) {
						Sint32 rowDepth;

						rowDepth = toFIXED(LuckyDiceFaceHeights[otherIndex]);
						if(rollingDie->z > rowDepth) {
							numbers[index + 2] = otherIndex;
							break;
						}
					}
					stoppedDice++;
				}
			}

			/* Both dice stopped: score the four numbers. */
			if(stoppedDice == DICE_COUNT) {
				state = LUCKY_DICE_RESULT;
				resultTimer = 0;
				resultKind = DICE_RESULT_NONE;
				winnings = 0;
				payoutTimer = 60;
				if(numbers[0] == numbers[1] && numbers[0] == numbers[2] &&
					numbers[0] == numbers[3]) {
					flashTimer[0] = flashTimer[1] = DICE_PERFECT_FLASH_TICKS;
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
					mnTextDrawNumberInWindowStub(MainRoutineStateWork.coins,
						DICE_COIN_DIGITS, (Sint32)sprites->coinsWindow, 0, 0);
					sySoundPlayStub(MUS_SUPER_LUCKY);
				} else {
					pairCount = 0;
					for(index = 0; index != DICE_RESULT_COUNT - 1; index++) {
						for(otherIndex = index + 1;
							otherIndex != DICE_RESULT_COUNT; otherIndex++) {
							if(numbers[index] == numbers[otherIndex]) {
								matchedNumbers[pairCount] = numbers[index];
								pairCount++;
							}
						}
					}
					if(pairCount == 1) {
						winnings = bet;
						mnTextDrawNumberInWindowStub(MainRoutineStateWork.coins,
							DICE_COIN_DIGITS, (Sint32)sprites->coinsWindow, 0,
							0);
						resultKind = DICE_RESULT_ONE_PAIR;
					}
					if(pairCount == 2) {
						winnings = bet * 2;
						mnTextDrawNumberInWindowStub(MainRoutineStateWork.coins,
							DICE_COIN_DIGITS, (Sint32)sprites->coinsWindow, 0,
							0);
						flashTimer[syRandom() & 1] = DICE_FLASH_TICKS;
						resultKind = DICE_RESULT_TWO_PAIR;
						sySoundPlayStub(MUS_LUCKY);
					}
					if(pairCount == 3) {
						winnings = bet * 3;
						mnTextDrawNumberInWindowStub(MainRoutineStateWork.coins,
							DICE_COIN_DIGITS, (Sint32)sprites->coinsWindow, 0,
							0);
						flashTimer[syRandom() & 1] = DICE_FLASH_TICKS;
						resultKind = DICE_RESULT_TRIPLE;
						sySoundPlayStub(MUS_VERY_LUCKY);
					}
				}
			}
		}

		/* Hide the unused sprites, write the four affine matrices (one per die
		   and shadow, scaled by depth) and copy the sprites to OAM. */
		for(; spriteCount != OAM_SIZE / sizeof(OAM_WORDS); spriteCount++) {
			sprites->oam[spriteCount].attr01 =
				OBJ_X(DISPLAY_WIDTH) | OBJ_SIZE_16X16 | OBJ_256_COLORS;
			sprites->oam[spriteCount].attr2 = 0;
		}

		for(index = 0; index != DICE_RESULT_COUNT; index++) {
			/* The sprite's matrix: its scale on the diagonal and none across,
			   each element stored through its address. */
			Uint16 affineA;
			Uint16 affineB;
			Uint16 affineC;
			Uint16 affineD;

			affineScale = spriteDepth[index] / 2 + DICE_NEAREST_SCALE;
			*&affineA = affineScale;
			*&affineB = 0;
			*&affineC = 0;
			*&affineD = affineScale;
			sprites->oam[index * 4 + 0].attr2 |= OBJ_AFFINE_PARAMETER(affineA);
			sprites->oam[index * 4 + 1].attr2 |= OBJ_AFFINE_PARAMETER(affineB);
			sprites->oam[index * 4 + 2].attr2 |= OBJ_AFFINE_PARAMETER(affineC);
			sprites->oam[index * 4 + 3].attr2 |= OBJ_AFFINE_PARAMETER(affineD);
		}

		syDMAXCopy(sprites->oam, OAM, DMA_COPY_WORDS(OAM_SIZE / 4), REG_DMA3);
		sySynch(1);
		frame++;
	}

	/* Fade out and free the game's state. */
	for(index = 0; index != DICE_FADE_TICKS + 1; index++) {
		scale = toFIXED(1) - index * (toFIXED(1) / DICE_FADE_TICKS);
		fdLuckyDiceScaleRGB555Buffer(
			spritePalette, (Uint16 *)OBJ_PLTT, scale, OBJ_PLTT_COLORS);
		fdLuckyDiceScaleRGB555Buffer(
			bgPalette, (Uint16 *)BG_PLTT, scale, BG_PLTT_COLORS);
		sySynch(1);
	}
	syHeapSlotRelease(HEAP_SLOT_CAMERA);
	syHeapSlotRelease(HEAP_SLOT_CANVAS_MAP);
	syHeapSlotRelease(HEAP_SLOT_BATTLE_CANVAS);
	syHeapSlotRelease(HEAP_SLOT_BATTLE_EFFECT);
	syHeapSlotRelease(HEAP_SLOT_BATTLE_SHEET);
}
