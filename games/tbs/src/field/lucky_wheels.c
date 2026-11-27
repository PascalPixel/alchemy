/* Lucky Wheels, Tolbi's game of chance: the player bets game tickets on
   paylines, pulls the lever up to five times and locks wheels between pulls to
   line up matching symbols. It draws its own wheels and paylines, and keeps an
   unused LZ packer. */

#include "field/lucky_wheels.h"
#include "item_ids.h"
#include "macros.h"
#include "resource_ids.h"
#include "battle/battle.h"
#include "battle/battle_effect_state.h"
#include "battle/blitter.h"
#include "field/lucky_wheels_lzpack.h"
#include "field/map_grid_scroll.h"
#include "game/main_routine_state.h"
#include "text/msg_ids.h"

/* The sparks of the jackpot, kept in the map metatile buffer. */
#define LUCKY_WHEELS_SPARKS ((PARTICLE *)RamMapGridMetatileBuffer)

/* Lucky Wheels' own state (LUCKY_WHEELS_STATE), in the heap slot Lucky */
/* Dice keeps its sprites in, beside the battle effect slots it borrows. */
#define LUCKY_WHEELS_HEAP_SLOT HEAP_SLOT_LUCKY_GAME

/* The paylines' canvas: 256 x 128 pixels of 8-bit colour, shown on BG2 from its
   tile 212 in the rows of its map from LUCKY_WHEELS_CANVAS_TOP_ROW to
   LUCKY_WHEELS_CANVAS_BOTTOM_ROW; the rows above and below show a clear tile,
   in the part of BG1's map below the screen that fdLuckyWheelsRun clears. */
#define LUCKY_WHEELS_CANVAS_SIZE	   (256 * 128)
#define LUCKY_WHEELS_CANVAS_FIRST_TILE 212
#define LUCKY_WHEELS_CANVAS_TOP_ROW	   2
#define LUCKY_WHEELS_CANVAS_BOTTOM_ROW 16
#define LUCKY_WHEELS_CANVAS_TILES \
	(BG_CHAR_BLOCK(0) + LUCKY_WHEELS_CANVAS_FIRST_TILE * TILE_SIZE_8BPP)
#define LUCKY_WHEELS_CANVAS_CLEAR_TILE 191
/* The canvas's first 64 colours, its sparks', from a Djinn sprite sheet's
   palette and cycled through others while a win shows. */
#define LUCKY_WHEELS_SPARK_COLORS	   64
/* The paylines' green on the canvas, colour 64, and its lighter centre. */
#define LUCKY_WHEELS_LINE_COLOR		   64
#define LUCKY_WHEELS_LINE_LIGHT		   (LUCKY_WHEELS_LINE_COLOR + 1)
/* The machine's picture: its 16 colours in BG palette 10, and its tiles from
   tile 424 of BG1's characters, all but those of the opening the wheels show
   through, which BG1's map fills with a clear tile. */
#define LUCKY_WHEELS_MACHINE_PALETTE   10
#define LUCKY_WHEELS_MACHINE_TILE	   424
#define LUCKY_WHEELS_CLEAR_TILE		   422
/* The machine's opening, in map metatiles: columns 5 to 24 and rows 3 to 13. */
#define LUCKY_WHEELS_IN_OPENING(column, row) \
	((column) >= 5 && (column) <= 24 && (row) > 2 && (row) <= 13)
/* The icons' picture begins with its 240 colours, the sprites' first 240. */
#define LUCKY_WHEELS_ICON_COLORS		240
/* The jackpot banner's and the title letters' sprite palette. */
#define LUCKY_WHEELS_JACKPOT_PALETTE	15
/* A palette's 16 colours in bytes, as each 16-colour picture's resource
   begins. */
#define PALETTE_BYTES					(PALETTE_COLORS * 2)
/* The Spanish title's first letter is its last letter's tile turned over both
   ways, drawn 16 pixels left and 8 down. */
#define LUCKY_WHEELS_TITLE_FIRST_TURNED (LANGUAGE == LANGUAGE_SPANISH)

#include "field/lucky_wheels_start.c"

#include "palette_scale.c"

/* A VBlank-pass event: on a VBlank after the canvas is ready (transferPending),
   sends the canvas to its tiles in VRAM (LUCKY_WHEELS_CANVAS_TILES), then
   clears it or fades it into trails as transferMode says. */
void fdLuckyWheelsTransferCanvas(void)
{
	BATTLE_EFFECT_STATE *effect;
	Uint8				*canvas;

	effect = BattleEffectStateWork[0];
	if(effect->transferPending == 1) {
		canvas = BattleEffectStateWork[1];
		switch(effect->transferMode) {
		case CANVAS_COPY_AND_CLEAR:
			syDMAXCopy(canvas, (void *)LUCKY_WHEELS_CANVAS_TILES,
				DMA_COPY_WORDS(LUCKY_WHEELS_CANVAS_SIZE / 4), REG_DMA3);
			syWordsFill(
				canvas, LUCKY_WHEELS_CANVAS_SIZE, effect->transferValue);
			break;
		case CANVAS_COPY_AND_FADE:
			/* Trails: nonzero colour numbers drop to 1/2 or 3/4. */
			if(effect->transferValue == CANVAS_FADE_HALF)
				syCanvasCopyAndHalveNonzero(canvas, LUCKY_WHEELS_CANVAS_TILES,
					LUCKY_WHEELS_CANVAS_SIZE);
			else
				syCanvasCopyAndScaleNonzeroThreeQuarters(canvas,
					LUCKY_WHEELS_CANVAS_TILES, LUCKY_WHEELS_CANVAS_SIZE);
			break;
		}
		effect->transferPending = 0;
		effect->framesSinceTransfer = 1;
	} else {
		(effect->framesSinceTransfer)++;
	}
}

#include "field/lucky_wheels_darken_and_sparks.c"

/* The byte of the canvas's pixel (x, y): its tiles run 32 to a row, 64 bytes
   each. */
static __inline__ Uint32 fdLuckyWheelsCanvasOffset(Uint32 x, Uint32 y)
{
	return ((y / 8 * 32 + x / 8) * 8 + y % 8) * 8 + x % 8;
}

/* A line's step along its shorter axis, in 256ths of a pixel. */
#define LINE_STEP_ONE (1 << 8)

/* Draws a line from (x0, y0) to (x1, y1) on the 256 x 128 8bpp canvas (32 tiles
   across), writing only over lower colour numbers. */
void fdLuckyWheelsDrawCanvasLine(
	Sint32 x0, Sint32 y0, Sint32 x1, Sint32 y1, Sint32 col)
{
	Sint32 dx = x1 - x0;
	Sint32 dy = y1 - y0;
	Sint32 frac = LINE_STEP_ONE / 2;
	Uint8 *canvas = ((Uint8 **)BattleEffectStateWork)[1];
	Sint32 step;
	Sint32 i;
	Sint32 x;
	Sint32 y;
	Sint32 tmp;
	Uint32 ofs;

	if(ABS(dx) < ABS(dy)) {
		/* Steep: step y one pixel at a time and follow x in fixed point. */
		if(dy < 0) {
			tmp = x0;
			x0 = x1;
			x1 = tmp;
			tmp = y0;
			y0 = y1;
			y1 = tmp;
			dx = x1 - x0;
			/* LEFTOVER(template): dy recomputed after the swap and never read
			 * again, as the shallow arm's swap below recomputes both deltas and
			 * reads dy for its y step; as the line removed. */
			dy = y1 - y0;
		}
		step = ABS(x1 - x0) * LINE_STEP_ONE / ABS(y1 - y0);
		x = x0;
		for(i = y0; i != y1; i++) {
			ofs = fdLuckyWheelsCanvasOffset(x, i);
			if(canvas[ofs] < col)
				canvas[ofs] = col;
			frac += step;
			if(frac & LINE_STEP_ONE) {
				if(dx > 0)
					x++;
				else
					x--;
				frac &= ~LINE_STEP_ONE;
			}
		}
	} else {
		/* Shallow: step x one pixel at a time and follow y in fixed point. */
		if(dx < 0) {
			tmp = x0;
			x0 = x1;
			x1 = tmp;
			tmp = y0;
			y0 = y1;
			y1 = tmp;
			/* LEFTOVER(template): dx recomputed after the swap and never read
			 * again, as the steep arm's swap above recomputes both deltas and
			 * reads dx for its x step; as the line removed. */
			dx = x1 - x0;
			dy = y1 - y0;
		}
		step = ABS(y1 - y0) * LINE_STEP_ONE / ABS(x1 - x0);
		y = y0;
		for(i = x0; i != x1; i++) {
			ofs = fdLuckyWheelsCanvasOffset(i, y);
			if(canvas[ofs] < col)
				canvas[ofs] = col;
			frac += step;
			if(frac & LINE_STEP_ONE) {
				if(dy > 0)
					y++;
				else
					y--;
				frac &= ~LINE_STEP_ONE;
			}
		}
	}
}

/* The machine's five wheels; the cursor's next place, past the last wheel, is
   the lever. */
#define LUCKY_WHEELS_REEL_COUNT			 5
#define LUCKY_WHEELS_LEVER				 LUCKY_WHEELS_REEL_COUNT
/* Pulling the lever: 48 frames down and back up, then 8 settling; only then can
   the wheels be stopped. */
#define LUCKY_WHEELS_LEVER_PULL_FRAMES	 48
#define LUCKY_WHEELS_LEVER_SETTLE_FRAMES 8
#define LUCKY_WHEELS_LEVER_FRAMES \
	(LUCKY_WHEELS_LEVER_PULL_FRAMES + LUCKY_WHEELS_LEVER_SETTLE_FRAMES)
/* The lever sprite's place at rest, in screen pixels, and how far it is pulled
   down. */
#define LUCKY_WHEELS_LEVER_X			208
#define LUCKY_WHEELS_LEVER_Y			40
#define LUCKY_WHEELS_LEVER_PULL			64
/* Left alone, the spinning wheels stop one every 32 frames; R stops one at
   once. */
#define LUCKY_WHEELS_AUTO_STOP_FRAMES	32
/* The pause after a winning spin before the win is shown, and after the last
   spin loses before the game ends. */
#define LUCKY_WHEELS_WIN_PAUSE			60
#define LUCKY_WHEELS_LOSS_PAUSE			45
/* The stop prompt's sound plays this many frames into a spin. */
#define LUCKY_WHEELS_STOP_PROMPT_FRAME	16
/* Where each picture of icons.png starts in sprite tiles, in OAM's 32-byte
   steps (a 256-colour tile is two): the sprite sheet is 32 pixels wide, so a
   picture starts at the tile numbered as its row. The six symbols are 16 apart
   from 0. */
#define LUCKY_WHEELS_LAMP_TILES			96	/* a wheel's lamp, not held */
#define LUCKY_WHEELS_HELD_LAMP_TILES	128 /* a held wheel's lamp */
#define LUCKY_WHEELS_CORNER_LIGHT_TILES 224
#define LUCKY_WHEELS_FRAME_LIGHT_TILES	232
#define LUCKY_WHEELS_LEVER_TILES		256
#define LUCKY_WHEELS_LINE_TILES			272 /* a payline's gem, not bet on */
#define LUCKY_WHEELS_LIT_LINE_TILES		464 /* a payline's gem, bet on */
/* The slot the lever runs in, drawn in eight pieces 16 pixels tall down beside
   the wheels from (LUCKY_WHEELS_LEVER_SLOT_X, LUCKY_WHEELS_LEVER_SLOT_Y). */
#define LUCKY_WHEELS_LEVER_SLOT_TILES	320
#define LUCKY_WHEELS_LEVER_SLOT_PIECES	8
#define LUCKY_WHEELS_LEVER_SLOT_X		206
#define LUCKY_WHEELS_LEVER_SLOT_Y		12
#define LUCKY_WHEELS_HAND_TILES			496 /* the cursor's hand */
/* a pull's number, the current one large */
#define LUCKY_WHEELS_PULL_TILES			528
#define LUCKY_WHEELS_ARROW_TILES		688 /* the cursor's blinking arrows */
/* The symbols round a wheel, each 16 pixels tall: a wheel turns once in 336
   pixels, 8 a frame, and stops only with a symbol centred on a line. */
#define LUCKY_WHEELS_REEL_SYMBOLS		21
#define LUCKY_WHEELS_SYMBOL_HEIGHT		16
#define LUCKY_WHEELS_REEL_TURN \
	(LUCKY_WHEELS_REEL_SYMBOLS * LUCKY_WHEELS_SYMBOL_HEIGHT)
#define LUCKY_WHEELS_SPIN_SPEED	 8
/* The seven paylines: the two diagonals (0 and 6) and the five rows between. */
#define LUCKY_WHEELS_PAYLINES	 7
#define LUCKY_WHEELS_MIDDLE_LINE 3
#define LUCKY_WHEELS_BET_MAX	 4
/* The pulls of the lever a bet buys: "You have only 5 chances to pull the
   lever." */
#define LUCKY_WHEELS_PULLS		 5

/* The wheels' symbols, in icons.png's order. */
typedef enum LUCKY_WHEELS_SYMBOL {
	LUCKY_WHEELS_SHIRT,
	LUCKY_WHEELS_HEART,
	LUCKY_WHEELS_BOOT,
	LUCKY_WHEELS_STAR,
	LUCKY_WHEELS_RING,
	LUCKY_WHEELS_MOON
} LUCKY_WHEELS_SYMBOL;

/* The symbol that matches any other on a payline. */
#define LUCKY_WHEELS_WILD_SYMBOL LUCKY_WHEELS_MOON
/* The machine fades in and out over 16 ticks, a sixteenth of full brightness a
   tick; the win flashes the screen darker over as many. */
#define LUCKY_WHEELS_FADE_TICKS	 16

/* The win cycles the background through five palettes, 16 ticks each. */
#define LUCKY_WHEELS_CYCLE_TICKS	16
#define LUCKY_WHEELS_CYCLE_PALETTES 5

/* The jackpot sparks: four bursts of 256 in turn, one every eight ticks. */
#define LUCKY_WHEELS_BURST_SPARKS		256
#define LUCKY_WHEELS_SPARK_COUNT		(4 * LUCKY_WHEELS_BURST_SPARKS)
/* A spark's size steps down every 12 ticks of its life, from 2 or 3 to 1. */
#define LUCKY_WHEELS_SPARK_SHRINK_TICKS 12
/* A held direction moves the cursor once, waits 12 ticks, then moves it again
   with 4 ticks between each move. */
#define LUCKY_WHEELS_REPEAT_DELAY		12
#define LUCKY_WHEELS_REPEAT_INTERVAL	4

/* One of Lucky Wheels' five wheels: how far it has turned, its 21 symbols,
   whether it is held, and the countdown to its stop (-1 until one starts). */
typedef struct REEL {
	Sint32 pos; /* pixels, LUCKY_WHEELS_SYMBOL_HEIGHT a symbol */
	Uint8  symbols[LUCKY_WHEELS_REEL_SYMBOLS];
	Uint8  held;
	Sint8  stop; /* frames */
} REEL;

struct WINDOW;

/* Lucky Wheels' phases (LUCKY_WHEELS_STATE.state). */
typedef enum LUCKY_WHEELS_PHASE {
	/* Up and Down change the bet, A spins, B leaves */
	LUCKY_WHEELS_BETTING = 0,
	LUCKY_WHEELS_SPINNING = 1, /* the wheels spin until R or time stops each */
	LUCKY_WHEELS_WON = 2,	   /* a payline won: a pause */
	LUCKY_WHEELS_SHOWING_WIN = 3, /* the win is shown until A */
	/* choose wheels to hold, or pull the lever */
	LUCKY_WHEELS_CHOOSING_HOLDS = 5,
	LUCKY_WHEELS_DONE = 10,	  /* the game ends */
	LUCKY_WHEELS_NO_WIN = 11, /* no payline won: A to choose holds */
	/* The fifth and last pull lost: a pause, then the end. */
	LUCKY_WHEELS_OUT_OF_SPINS = 20
} LUCKY_WHEELS_PHASE;

/* The prompt the top window shows while wheels are locked
   (LUCKY_WHEELS_STATE.prompt), so that it is drawn again only when it
   changes. */
typedef enum LUCKY_WHEELS_PROMPT {
	LUCKY_WHEELS_PROMPT_NONE = 0,
	/* "If you lock all the slots,", its first line. */
	LUCKY_WHEELS_PROMPT_ALL_HELD = 1,
	/* Then "you can't pull the lever!" under it. */
	LUCKY_WHEELS_PROMPT_ALL_HELD_BOTH_LINES = 2,
	LUCKY_WHEELS_PROMPT_START = 3,	 /* ":Start machine", on the lever */
	LUCKY_WHEELS_PROMPT_HOLD = 4,	 /* ":Stop", on a spinning wheel */
	LUCKY_WHEELS_PROMPT_RELEASE = 5, /* ":Spin", on a locked wheel */
	/* After a pull that won nothing: "to lock, pull the lever!" is drawn. */
	LUCKY_WHEELS_PROMPT_PULL_LEVER = 1
} LUCKY_WHEELS_PROMPT;

/* Lucky Wheels' state in LUCKY_WHEELS_HEAP_SLOT: its wheels, state, bet and
   spins, keys, timer, winning lines, sprites, windows and the scanline offsets
   that shade the wheels. */
typedef struct LUCKY_WHEELS_STATE {
	REEL	  reels[LUCKY_WHEELS_REEL_COUNT];
	Sint32	  state;
	Sint32	  cursor;
	Sint32	  spins;
	Sint32	  bet;
	Uint16	  keys;
	Uint16	  dir;
	Uint16	  pressed;
	Uint16	  repeat;
	Uint8	  unused[4]; /* no code names it */
	Sint32	  tmr;
	Sint32	  winningLines[LUCKY_WHEELS_PAYLINES];
	OAM_WORDS oam[128];
	WINDOW	 *win;
	WINDOW	 *subWin;
	Uint8	  unusedAfterWins[8]; /* no code names it */
	Uint16	  scanlineOffsets[DISPLAY_HEIGHT];
#if LUCKY_WHEELS_PROMPT_DRAWN_ONCE
	/* The prompt shown (LUCKY_WHEELS_PROMPT). The Japanese release has none: it
	   closes and reopens its prompt window every tick. */
	Sint32 prompt;
#endif
} LUCKY_WHEELS_STATE;

/* Lucky Wheels' tables, the same in both games. */

/* Where each spark picture (a square 2 to 20 pixels on a side) starts: size s
   (1 to 10) is a square 2s pixels a side, after the smaller squares' pixels. */
const Uint16 LuckyWheelsSparkPictures[] = {
	0, 4, 20, 56, 120, 220, 364, 560, 816, 1140};

/* The prize of each wheel symbol (LUCKY_WHEELS_PRIZE), kept in
   MainRoutineStateWork.holdings.wonPrizes for each winning line. */
const Uint8 LuckyWheelsSymbolPrizes[] = {
	[LUCKY_WHEELS_SHIRT] = LUCKY_WHEELS_PRIZE_UNDERSHIRT,
	[LUCKY_WHEELS_HEART] = LUCKY_WHEELS_PRIZE_HEART_ITEM,
	[LUCKY_WHEELS_BOOT] = LUCKY_WHEELS_PRIZE_BOOTS,
	[LUCKY_WHEELS_STAR] = LUCKY_WHEELS_PRIZE_STAR_ITEM,
	[LUCKY_WHEELS_RING] = LUCKY_WHEELS_PRIZE_RING,
	[LUCKY_WHEELS_MOON] = LUCKY_WHEELS_PRIZE_ANYTHING,
};

/* The tile of each title letter, the letter's place in its sprite sheet:
   jackpot.png holds `Jackpot!` and jackpot_ja.png `ジャックポト!`. */
const Uint8 LuckyWheelsTitleLetterTiles[LUCKY_WHEELS_TITLE_LETTERS] = {
#if LANGUAGE == LANGUAGE_JAPANESE
	0, 1, 2, 3, 4, 2, 5, 6
#elif LANGUAGE == LANGUAGE_SPANISH
	7, 0, 1, 2, 3, 4, 5, 6, 7
#else
	0, 1, 2, 3, 4, 5, 6, 7
#endif
};

#define LUCKY_WHEELS_MACHINE_LIGHTS 14 /* the machine's lights */
#define LUCKY_WHEELS_CORNER_LIGHTS	4  /* the first of them, at its corners */
/* The positions of the machine's lights, in screen pixels: its four corners
   first, then the lights along its frame. */
const Uint8 LuckyWheelsMachineSpriteX[LUCKY_WHEELS_MACHINE_LIGHTS] = {
	2, 0, 232, 232, 32, 32, 200, 200, 28, 28, 208, 208, 219, 219};
const Uint8 LuckyWheelsMachineSpriteY[LUCKY_WHEELS_MACHINE_LIGHTS] = {
	2, 152, 0, 152, 8, 104, 8, 104, 118, 140, 118, 140, 6, 108};

/* The advance of each title letter, in pixels, from LUCKY_WHEELS_TITLE_X. The
   last letter's advance is never used; Japanese leaves it out (0). */
const Uint8 LuckyWheelsTitleLetterWidths[LUCKY_WHEELS_TITLE_LETTERS] = {
#if LANGUAGE == LANGUAGE_JAPANESE
	32, 24, 22, 28, 28, 22, 18
#elif LANGUAGE == LANGUAGE_SPANISH
	16, 28, 28, 26, 26, 28, 28, 20, 24
#else
	28, 28, 26, 26, 28, 28, 20, 24
#endif
};

/* Lucky Wheels' VBlank event: reads the keys, runs the game's state (betting,
   spinning and stopping the wheels, scoring the paylines, choosing wheels to
   hold, showing a win) and builds all 128 sprites: the cursor, the title
   letters of a win, the frame, the hold lamps, the spin counter, the paylines,
   the wheel symbols, the lever and the bottom bar. */
void fdLuckyWheelsUpdate(void)
{
	LUCKY_WHEELS_STATE	*wheels;
	BATTLE_EFFECT_STATE *effect;
	volatile Uint16		*dma;
	Sint32				 spr;
	Sint32				 objMode;
	Uint16				 keys;
	Uint32				 dpad;
	Sint32				 tickets;
	Sint32				 allHeld;
	Sint32				 stoppedReels;
	Sint32				 line;
	Sint32				 arrowFrm;
	Sint32				 x;
	Sint32				 y;
	Sint32				 lever;
	Sint32				 k;
	Sint32				 i;

	wheels =
		((LUCKY_WHEELS_STATE **)BattleEffectStateWork)[LUCKY_WHEELS_HEAP_SLOT -
			HEAP_SLOT_BATTLE_EFFECT];
	effect = BattleEffectStateWork[0];
	spr = 0;
	objMode = OBJ_MODE(OBJ_MODE_SEMI_TRANSPARENT);

	syRandom();

	dma = REG_DMA0;
	dma[DMA_CNT_H_INDEX] &= ~DMA_START_TIMING_DRQ_REPEAT;
	dma[DMA_CNT_H_INDEX] &= ~DMA_ENABLE;
	(void)dma[DMA_CNT_H_INDEX];
	/* One halfword at each HBlank (repeating, destination reloaded). */
	syDMAXCopy(wheels->scanlineOffsets, (void *)&REG_BLDY,
		DMA_HBLANK_HALFWORD_PER_LINE, (volatile Uint32 *)dma);

	keys = PerData;
	dpad = PerData & KEYS_DPAD;
	wheels->pressed = keys & ~wheels->keys;
	wheels->dir = dpad;
	if((wheels->keys & KEYS_DPAD) == wheels->dir) {
		if(wheels->repeat > LUCKY_WHEELS_REPEAT_DELAY)
			wheels->repeat = LUCKY_WHEELS_REPEAT_DELAY;
		if(wheels->repeat == 0) {
			wheels->repeat = LUCKY_WHEELS_REPEAT_INTERVAL;
		} else {
			wheels->repeat--;
			wheels->dir = 0;
		}
	} else {
		wheels->repeat = LUCKY_WHEELS_REPEAT_DELAY;
	}
	wheels->keys = keys;

	if(DebugPaused == 0) {
		if(wheels->state == LUCKY_WHEELS_BETTING) {
			tickets = gmPartyInventoryCountItemStub(ITEM_GAME_TICKET);
			mnTextDrawNumInWindowStub(tickets - wheels->bet,
				LUCKY_WHEELS_TICKET_DIGITS, wheels->subWin,
				LUCKY_WHEELS_TICKET_X, 0);
			mnTextDrawNumInWindowStub(wheels->bet, 2, wheels->subWin, 64, 8);
			if(wheels->pressed & KEY_B) {
				PARTY_HOLDINGS *holdings = &MainRoutineStateWork.holdings;

				wheels->state = LUCKY_WHEELS_DONE;
				holdings->wonPrizes[0] = LUCKY_WHEELS_PRIZE_LIST_DONE;
				mnWindowCloseStub(wheels->win, 1);
			} else {
				if(wheels->pressed & KEY_UP) {
					if(wheels->bet < LUCKY_WHEELS_BET_MAX &&
						tickets > wheels->bet) {
						wheels->bet++;
						sySndPlayStub(SE_MENU_CURSOR_MOVE);
					} else {
						sySndPlayStub(SE_MENU_CANCEL);
					}
				}
				if(wheels->pressed & KEY_DOWN) {
					if(wheels->bet > 1) {
						wheels->bet--;
						sySndPlayStub(SE_MENU_CURSOR_MOVE);
					} else {
						sySndPlayStub(SE_MENU_CANCEL);
					}
				}
				REG_BLDCNT =
					BLDCNT_TGT1_OBJ | BLDCNT_EFFECT_DARKEN | BLDCNT_TGT2_ALL;
				REG_BLDALPHA = BLEND_ALPHA(16, 0);
				if(wheels->pressed & KEY_A) {
					wheels->state = LUCKY_WHEELS_SPINNING;
					effect->frm = 0;
					mnWindowCloseStub(wheels->win, 1);
					for(k = 0; k != wheels->bet; k++)
						gmPartyInventoryRemoveStub(ITEM_GAME_TICKET);
					mnWindowCloseStub(wheels->subWin, 1);
					sySndPlayStub(SE_LUCKY_WHEELS_SPIN);
				}
			}
		} else if(wheels->state == LUCKY_WHEELS_CHOOSING_HOLDS) {
			allHeld = 0;
			wheels->tmr++;
			for(k = 0;
				k != LUCKY_WHEELS_REEL_COUNT && wheels->reels[k].held != 0; k++)
				;
			if(k == LUCKY_WHEELS_REEL_COUNT)
				allHeld = 1;
			if(wheels->pressed & KEY_A) {
				wheels->tmr = 0;
				effect->frm = 0;
				if(wheels->spins == LUCKY_WHEELS_PULLS - 1) {
					wheels->spins = 0;
					wheels->cursor = 0;
					wheels->state = LUCKY_WHEELS_BETTING;
					for(i = 0; i != LUCKY_WHEELS_REEL_COUNT; i++) {
						wheels->reels[i].held = 0;
						wheels->reels[i].stop = -1;
					}
				} else if(wheels->cursor < LUCKY_WHEELS_LEVER) {
					sySndPlayStub(SE_LUCKY_WHEELS_HOLD);
					wheels->reels[wheels->cursor].held ^= 1;
				} else if(allHeld == 0) {
					sySndPlayStub(SE_LUCKY_WHEELS_SPIN);
					wheels->state = LUCKY_WHEELS_SPINNING;
					wheels->cursor = 0;
					for(i = 0; i != LUCKY_WHEELS_REEL_COUNT; i++)
						wheels->reels[i].stop = -1;
					wheels->spins++;
				} else {
					sySndPlayStub(SE_MENU_CANCEL);
				}
			} else {
				if(wheels->dir & KEY_RIGHT) {
					wheels->cursor =
						(wheels->cursor + 1) % (LUCKY_WHEELS_LEVER + 1);
					sySndPlayStub(SE_MENU_CURSOR_MOVE);
				}
				if(wheels->dir & KEY_LEFT) {
					wheels->cursor = (wheels->cursor + LUCKY_WHEELS_LEVER) %
						(LUCKY_WHEELS_LEVER + 1);
					sySndPlayStub(SE_MENU_CURSOR_MOVE);
				}
			}

			if(wheels->state == LUCKY_WHEELS_CHOOSING_HOLDS) {
#if LUCKY_WHEELS_PROMPT_DRAWN_ONCE
				if(wheels->cursor == LUCKY_WHEELS_LEVER) {
					if(allHeld != 0) {
						if(wheels->prompt != LUCKY_WHEELS_PROMPT_ALL_HELD &&
							wheels->prompt !=
								LUCKY_WHEELS_PROMPT_ALL_HELD_BOTH_LINES) {
							mnWindowCloseStub(wheels->win, 1);
							wheels->win =
								mnWindowCreateStub(LUCKY_WHEELS_ALL_HELD_X, 0,
									LUCKY_WHEELS_ALL_HELD_WIDTH, 4,
									WINDOW_FLAG_INSTANT | WINDOW_FLAG_UNREAD);
							mnTextDrawMessageAsTilesStub(
								MsgSlotsAllHeld, wheels->win, 0, 0);
							wheels->prompt = LUCKY_WHEELS_PROMPT_ALL_HELD;
						} else if(wheels->prompt ==
							LUCKY_WHEELS_PROMPT_ALL_HELD) {
							mnTextDrawMessageAsTilesStub(
								MsgSlotsCannotPull, wheels->win, 0, 8);
							wheels->prompt =
								LUCKY_WHEELS_PROMPT_ALL_HELD_BOTH_LINES;
						}
					} else {
						if(wheels->prompt != LUCKY_WHEELS_PROMPT_START) {
							mnWindowCloseStub(wheels->win, 1);
							wheels->win =
								mnWindowCreateStub(LUCKY_WHEELS_START_X, 0,
									LUCKY_WHEELS_START_WIDTH, 3,
									WINDOW_FLAG_INSTANT | WINDOW_FLAG_UNREAD);
							mnTextDrawMessageAsTilesStub(
								MsgSlotsStart, wheels->win, 0, 0);
						}
						wheels->prompt = LUCKY_WHEELS_PROMPT_START;
					}
				} else if(wheels->reels[wheels->cursor].held == 0) {
					if(wheels->prompt != LUCKY_WHEELS_PROMPT_HOLD) {
						mnWindowCloseStub(wheels->win, 1);
						wheels->win = mnWindowCreateStub(LUCKY_WHEELS_HOLD_X, 0,
							LUCKY_WHEELS_HOLD_WIDTH, 3,
							WINDOW_FLAG_INSTANT | WINDOW_FLAG_UNREAD);
						mnTextDrawMessageAsTilesStub(
							MsgSlotsHold, wheels->win, 0, 0);
					}
					wheels->prompt = LUCKY_WHEELS_PROMPT_HOLD;
				} else {
					if(wheels->prompt != LUCKY_WHEELS_PROMPT_RELEASE) {
						mnWindowCloseStub(wheels->win, 1);
						wheels->win = mnWindowCreateStub(LUCKY_WHEELS_RELEASE_X,
							0, LUCKY_WHEELS_RELEASE_WIDTH, 3,
							WINDOW_FLAG_INSTANT | WINDOW_FLAG_UNREAD);
						mnTextDrawMessageAsTilesStub(
							MsgSlotsRelease, wheels->win, 0, 0);
					}
					wheels->prompt = LUCKY_WHEELS_PROMPT_RELEASE;
				}
#else
				mnWindowCloseStub(wheels->win, 1);
				if(wheels->cursor == LUCKY_WHEELS_LEVER) {
					if(allHeld != 0) {
						wheels->win = mnWindowCreateStub(12, 0, 18, 3,
							WINDOW_FLAG_INSTANT | WINDOW_FLAG_UNREAD);
						mnTextDrawMessageAsTilesStub(
							MsgSlotsAllHeld, wheels->win, 0, 0);
					} else {
						wheels->win = mnWindowCreateStub(18, 0, 12, 3,
							WINDOW_FLAG_INSTANT | WINDOW_FLAG_UNREAD);
						mnTextDrawMessageAsTilesStub(
							MsgSlotsStart, wheels->win, 0, 0);
					}
				} else {
					wheels->win = mnWindowCreateStub(
						18, 0, 12, 3, WINDOW_FLAG_INSTANT | WINDOW_FLAG_UNREAD);
					if(wheels->reels[wheels->cursor].held == 0)
						mnTextDrawMessageAsTilesStub(
							MsgSlotsHold, wheels->win, 0, 0);
					else
						mnTextDrawMessageAsTilesStub(
							MsgSlotsRelease, wheels->win, 0, 0);
				}
#endif
			} else {
				mnWindowCloseStub(wheels->win, 1);
			}
		} else if(wheels->state == LUCKY_WHEELS_WON) {
			wheels->tmr++;
			objMode = 0;
			if(wheels->tmr == LUCKY_WHEELS_WIN_PAUSE) {
				wheels->state = LUCKY_WHEELS_SHOWING_WIN;
				sySndPlayStub(MUS_SUPER_LUCKY);
				wheels->tmr = 0;
				REG_BLDCNT =
					BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
				REG_BLDALPHA = BLEND_ALPHA(16, 16);
				effect->transferMode = CANVAS_COPY_AND_FADE;
				effect->transferValue = CANVAS_FADE_THREE_QUARTERS;
			}
		} else if(wheels->state == LUCKY_WHEELS_SHOWING_WIN) {
			wheels->tmr++;
			objMode = 0;
			if(wheels->pressed & KEY_A) {
				wheels->state = LUCKY_WHEELS_DONE;
				sySndPlayStub(SE_MENU_CONFIRM);
			}
		} else if(wheels->state == LUCKY_WHEELS_NO_WIN) {
#if LUCKY_WHEELS_PROMPT_DRAWN_ONCE
			if(wheels->prompt == LUCKY_WHEELS_PROMPT_NONE) {
				wheels->prompt = LUCKY_WHEELS_PROMPT_PULL_LEVER;
				mnTextDrawMessageAsTilesStub(
					MsgSlotsPullLever, wheels->win, 0, 8);
			}
#endif
			if(wheels->pressed & KEY_A) {
				wheels->state = LUCKY_WHEELS_CHOOSING_HOLDS;
#if LUCKY_WHEELS_PROMPT_DRAWN_ONCE
				wheels->prompt = LUCKY_WHEELS_PROMPT_NONE;
#endif
				sySndPlayStub(SE_MENU_CONFIRM);
				mnWindowCloseStub(wheels->win, 1);
			}
		} else if(wheels->state == LUCKY_WHEELS_OUT_OF_SPINS) {
			wheels->tmr++;
			if(wheels->tmr == LUCKY_WHEELS_LOSS_PAUSE)
				wheels->state = LUCKY_WHEELS_DONE;
		} else if(wheels->state != LUCKY_WHEELS_DONE) {
			if(wheels->tmr == 4) {
				wheels->win = mnWindowCreateStub(LUCKY_WHEELS_STOP_X, 17,
					LUCKY_WHEELS_STOP_WIDTH, 3,
					WINDOW_FLAG_INSTANT | WINDOW_FLAG_UNREAD);
				mnTextDrawMessageAsTilesStub(
					MsgSlotsStopReels, wheels->win, 0, 0);
			}
			if(wheels->tmr == LUCKY_WHEELS_STOP_PROMPT_FRAME)
				sySndPlayStub(SE_LUCKY_WHEELS_STOP_PROMPT);
			if(wheels->tmr > LUCKY_WHEELS_LEVER_FRAMES) {
				if(effect->frm >= LUCKY_WHEELS_AUTO_STOP_FRAMES ||
					(wheels->pressed & KEY_R)) {
					effect->frm = 0;
					for(i = 0; i != LUCKY_WHEELS_REEL_COUNT; i++) {
						if(wheels->reels[i].held == 0 &&
							wheels->reels[i].stop == -1) {
							wheels->reels[i].stop = (syRandom() & 3) + 4;
							sySndPlayStub(SE_LUCKY_WHEELS_STOP);
							break;
						}
					}
				}
			}

			for(i = 0; i != LUCKY_WHEELS_REEL_COUNT; i++) {
				if(wheels->reels[i].stop > 0)
					wheels->reels[i].stop--;
			}

			stoppedReels = 0;
			for(i = 0; i != LUCKY_WHEELS_REEL_COUNT; i++) {
				if(wheels->reels[i].held == 1)
					stoppedReels++;
				else if(wheels->reels[i].stop == 0 &&
					(wheels->reels[i].pos & (LUCKY_WHEELS_SYMBOL_HEIGHT - 1)) ==
						LUCKY_WHEELS_SYMBOL_HEIGHT / 2)
					stoppedReels++;
			}

			if(stoppedReels == LUCKY_WHEELS_REEL_COUNT) {
				PARTY_HOLDINGS *holdings = &MainRoutineStateWork.holdings;
				Sint32			hits = 0;

				for(line = 0; line != LUCKY_WHEELS_PAYLINES; line++) {
					Sint32 mixed;
					Sint32 found;

					found = -1;
					mixed = 0;
					wheels->winningLines[line] = 0;
					if(line > LUCKY_WHEELS_MIDDLE_LINE - wheels->bet &&
						line < wheels->bet + LUCKY_WHEELS_MIDDLE_LINE) {
						for(i = 0; i != LUCKY_WHEELS_REEL_COUNT; i++) {
							Sint32 symbol;

							if(line == 0)
								symbol =
									wheels->reels[i].symbols
										[(i -
											 wheels->reels[i].pos /
												 LUCKY_WHEELS_SYMBOL_HEIGHT +
											 LUCKY_WHEELS_REEL_SYMBOLS + 1) %
											LUCKY_WHEELS_REEL_SYMBOLS];
							else if(line == LUCKY_WHEELS_PAYLINES - 1)
								symbol =
									wheels->reels[i].symbols
										[(-i -
											 wheels->reels[i].pos /
												 LUCKY_WHEELS_SYMBOL_HEIGHT +
											 LUCKY_WHEELS_REEL_SYMBOLS + 5) %
											LUCKY_WHEELS_REEL_SYMBOLS];
							else
								symbol =
									wheels->reels[i].symbols
										[(line -
											 wheels->reels[i].pos /
												 LUCKY_WHEELS_SYMBOL_HEIGHT +
											 LUCKY_WHEELS_REEL_SYMBOLS) %
											LUCKY_WHEELS_REEL_SYMBOLS];
							if(symbol != LUCKY_WHEELS_WILD_SYMBOL) {
								if(found == -1)
									found = symbol;
								else if(found != symbol)
									mixed = 1;
							}
						}
						if(mixed == 0) {
							wheels->winningLines[line] = 1;
							holdings->wonPrizes[hits] =
								LuckyWheelsSymbolPrizes[found];
							hits++;
						}
					}
				}
				wheels->tmr = 0;
				if(hits != 0) {
					holdings->wonPrizes[hits] = LUCKY_WHEELS_PRIZE_LIST_END;
					wheels->state = LUCKY_WHEELS_WON;
					sySndPlayStub(SE_THUNDER);
					effect->transferMode = CANVAS_COPY_AND_CLEAR;
					effect->transferValue = 0;
					REG_BLDCNT = 0;
					mnWindowCloseStub(wheels->win, 1);
				} else {
					wheels->state = LUCKY_WHEELS_NO_WIN;
#if LUCKY_WHEELS_PROMPT_DRAWN_ONCE
					wheels->prompt = LUCKY_WHEELS_PROMPT_NONE;
					mnWindowCloseStub(wheels->win, 1);
#if LUCKY_WHEELS_HINT_BEFORE_LAST_SPIN
					if(wheels->spins < LUCKY_WHEELS_PULLS - 1)
#endif
					{
						wheels->win = mnWindowCreateStub(3, 16, 24, 4,
							WINDOW_FLAG_INSTANT | WINDOW_FLAG_UNREAD);
						mnTextDrawMessageAsTilesStub(
							MsgSlotsChooseHeld, wheels->win, 0, 0);
					}
#else
					mnWindowCloseStub(wheels->win, 1);
					{
						Sint32 msg;

						wheels->win = mnWindowCreateStub(7, 16, 16, 4,
							WINDOW_FLAG_INSTANT | WINDOW_FLAG_UNREAD);
						/* "When you've chosen which slots" above "to lock, pull
						   the lever!". */
						msg = MsgSlotsChooseHeld;
						mnTextDrawMessageAsTilesStub(msg, wheels->win, 0, 0);
						mnTextDrawMessageAsTilesStub(
							msg + 1, wheels->win, 0, 8);
					}
#endif
					if(wheels->spins == LUCKY_WHEELS_PULLS - 1) {
						Sint32 left =
							gmPartyInventoryCountItemStub(ITEM_GAME_TICKET);
						WINDOW *win;
						Sint32	msg;

						/* DEAD: both arms set the out-of-spins state, so the
						   ticket count cannot change it; the ROM keeps the
						   compare and both stores. */
						if(left > 0)
							wheels->state = LUCKY_WHEELS_OUT_OF_SPINS;
						else
							wheels->state = LUCKY_WHEELS_OUT_OF_SPINS;
						if(wheels->bet > left)
							wheels->bet = left;
						wheels->spins = 0;
						wheels->cursor = 0;
						for(i = 0; i != LUCKY_WHEELS_REEL_COUNT; i++) {
							wheels->reels[i].held = 0;
							wheels->reels[i].stop = -1;
						}
						effect->transferMode = CANVAS_COPY_AND_CLEAR;
						effect->transferValue = 0;
						REG_BLDCNT = 0;
						win = mnWindowCreateStub(18, 0, 12, 4,
							WINDOW_FLAG_INSTANT | WINDOW_FLAG_UNREAD);
						wheels->subWin = win;
						/* "Game ticket" above "Bet". */
						msg = MsgSlotsBet;
						mnTextDrawMessageAsTilesStub(msg, win, 0, 8);
						mnTextDrawMessageAsTilesStub(
							msg - 1, wheels->subWin, 0, 0);
					}
				}
			}

			if(wheels->state == LUCKY_WHEELS_SPINNING) {
				for(i = 0; i != LUCKY_WHEELS_REEL_COUNT; i++) {
					REEL *reel = &wheels->reels[i];

					if(reel->held == 0) {
						if(reel->stop != 0)
							reel->pos += LUCKY_WHEELS_SPIN_SPEED;
						else if((reel->pos &
									(LUCKY_WHEELS_SYMBOL_HEIGHT - 1)) !=
							LUCKY_WHEELS_SYMBOL_HEIGHT / 2)
							reel->pos += LUCKY_WHEELS_SPIN_SPEED;
						if(reel->pos == LUCKY_WHEELS_REEL_TURN)
							reel->pos = 0;
					}
				}
			}
			(effect->frm)++;
			wheels->tmr++;
		}
	}

	if(wheels->state == LUCKY_WHEELS_CHOOSING_HOLDS) {
		x = wheels->cursor * 36 + 36;
		y = 128;
		arrowFrm = 0;
		if((wheels->tmr & 15) <= 7)
			arrowFrm = 1;
		if(wheels->cursor == LUCKY_WHEELS_LEVER) {
			x = LUCKY_WHEELS_LEVER_X;
			y = 32;
		}
		wheels->oam[spr].attr01 =
			OBJ_X(x - 12) | objMode | (y + 8) | OBJ_SIZE_32X16 | OBJ_256_COLORS;
		wheels->oam[spr].attr2 =
			arrowFrm * SPRITE_TILES_8BPP(32, 16) + LUCKY_WHEELS_ARROW_TILES;
		spr++;
		wheels->oam[spr].attr01 = OBJ_X(x + 12) | objMode | (y + 8) |
			OBJ_SIZE_32X16 | OBJ_256_COLORS | OBJ_HFLIP;
		wheels->oam[spr].attr2 =
			arrowFrm * SPRITE_TILES_8BPP(32, 16) + LUCKY_WHEELS_ARROW_TILES;
		spr++;
		wheels->oam[spr].attr01 =
			OBJ_X(x) | objMode | y | OBJ_SIZE_32X32 | OBJ_256_COLORS;
		wheels->oam[spr].attr2 = LUCKY_WHEELS_HAND_TILES;
		spr++;
	}

	if(wheels->state == LUCKY_WHEELS_SHOWING_WIN) {
		for(i = 0; i != LUCKY_WHEELS_TITLE_LETTERS; i++) {
			PARTICLE *p = &effect->particles[i];

			if(LUCKY_WHEELS_TITLE_FIRST_TURNED && i == 0)
				wheels->oam[spr].attr01 =
					OBJ_X(((Sint16)FIXEDtoINT(p->x) + 512 - 16) & OBJ_X_MASK) |
					objMode |
					(((Sint16)FIXEDtoINT(p->y) + OBJ_Y_RANGE + 8) &
						(OBJ_Y_RANGE - 1)) |
					OBJ_SIZE_32X32 | OBJ_HFLIP | OBJ_VFLIP;
			else
				wheels->oam[spr].attr01 = OBJ_X((Sint16)FIXEDtoINT(p->x)) |
					objMode |
					(((Sint16)FIXEDtoINT(p->y) + OBJ_Y_RANGE) &
						(OBJ_Y_RANGE - 1)) |
					OBJ_SIZE_32X32;
			wheels->oam[spr].attr2 =
				(LuckyWheelsTitleLetterTiles[i] * SPRITE_TILES(32, 32) +
					LUCKY_WHEELS_TITLE_TILES) |
				OBJ_PALETTE(LUCKY_WHEELS_JACKPOT_PALETTE);
			p->y += p->velY;
			p->velY += toFIXED(1) / 4;
			if(wheels->tmr % 256 == i * 4 + 200) {
				p->velY = toFIXED(6);
				p->variant = 0;
			}
			if(p->y > toFIXED(64)) {
				p->y = toFIXED(64);
				if(p->variant <= 1)
					p->velY = -p->velY / 2;
				p->variant++;
			}
			spr++;
		}
	}

	for(k = 0; k != LUCKY_WHEELS_MACHINE_LIGHTS; k++) {
		wheels->oam[spr].attr01 = OBJ_X(LuckyWheelsMachineSpriteX[k]) |
			objMode | LuckyWheelsMachineSpriteY[k] | OBJ_SIZE_32X16 |
			OBJ_256_COLORS;
		if(k < LUCKY_WHEELS_CORNER_LIGHTS)
			wheels->oam[spr].attr2 =
				OBJ_PRIORITY(1) | LUCKY_WHEELS_CORNER_LIGHT_TILES;
		else
			wheels->oam[spr].attr2 =
				OBJ_PRIORITY(1) | LUCKY_WHEELS_FRAME_LIGHT_TILES;
		spr++;
	}

	for(i = 0; i != LUCKY_WHEELS_REEL_COUNT; i++) {
		if(wheels->reels[i].held == 0) {
			wheels->oam[spr].attr01 = objMode | OBJ_X(i * 36 + 32) | 124 |
				OBJ_SIZE_32X32 | OBJ_256_COLORS;
			wheels->oam[spr].attr2 = OBJ_PRIORITY(1) | LUCKY_WHEELS_LAMP_TILES;
		} else {
			wheels->oam[spr].attr01 = objMode | OBJ_X(i * 36 + 32) | 124 |
				OBJ_SIZE_32X32 | OBJ_256_COLORS;
			wheels->oam[spr].attr2 =
				OBJ_PRIORITY(1) | LUCKY_WHEELS_HELD_LAMP_TILES;
		}
		spr++;
	}

	/* One sprite for each pull of the lever, the first and its re-pulls; the
	   current pull's takes other tiles. */
	for(i = 0; i != LUCKY_WHEELS_PULLS; i++) {
		wheels->oam[spr].attr01 =
			objMode | OBJ_X(i * 16 + 32) | 3 | OBJ_SIZE_32X16 | OBJ_256_COLORS;
		if(i == wheels->spins)
			wheels->oam[spr].attr2 =
				(i * 2 * SPRITE_TILES_8BPP(32, 16) + LUCKY_WHEELS_PULL_TILES) |
				OBJ_PRIORITY(1);
		else
			wheels->oam[spr].attr2 =
				(i * 2 * SPRITE_TILES_8BPP(32, 16) + LUCKY_WHEELS_PULL_TILES +
					SPRITE_TILES_8BPP(32, 16)) |
				OBJ_PRIORITY(1);
		spr++;
	}

	for(line = 0; line != LUCKY_WHEELS_PAYLINES; line++) {
		wheels->oam[spr].attr01 =
			OBJ_X((516 - ((line & 1) << 3)) & OBJ_X_MASK) | objMode |
			(line * LUCKY_WHEELS_SYMBOL_HEIGHT + 5) | OBJ_SIZE_32X32 |
			OBJ_256_COLORS;
		if(line > LUCKY_WHEELS_MIDDLE_LINE - wheels->bet &&
			line < wheels->bet + LUCKY_WHEELS_MIDDLE_LINE)
			wheels->oam[spr].attr2 =
				OBJ_PRIORITY(1) | LUCKY_WHEELS_LIT_LINE_TILES;
		else
			wheels->oam[spr].attr2 = OBJ_PRIORITY(1) | LUCKY_WHEELS_LINE_TILES;
		spr++;
	}

	for(i = 0; i != LUCKY_WHEELS_REEL_COUNT; i++) {
		REEL *reel = &wheels->reels[i];

		for(line = 0; line != LUCKY_WHEELS_PAYLINES; line++) {
			wheels->oam[spr].attr01 =
				(line * LUCKY_WHEELS_SYMBOL_HEIGHT +
					reel->pos % LUCKY_WHEELS_SYMBOL_HEIGHT + 4) |
				OBJ_X(i * 32 + 40) | OBJ_SIZE_32X16 | OBJ_256_COLORS;
			wheels->oam[spr].attr2 =
				reel->symbols[(line - reel->pos / LUCKY_WHEELS_SYMBOL_HEIGHT +
								  LUCKY_WHEELS_REEL_SYMBOLS) %
					LUCKY_WHEELS_REEL_SYMBOLS] *
					SPRITE_TILES_8BPP(32, 16) |
				OBJ_PRIORITY(2);
			spr++;
		}
	}

	/* The lever, pulled down and let back up, then bouncing as it settles. */
	lever = LUCKY_WHEELS_LEVER_Y;
	if(wheels->state == LUCKY_WHEELS_SPINNING) {
		if(wheels->tmr < LUCKY_WHEELS_LEVER_PULL_FRAMES)
			lever = FIXEDtoINT(
						sySin(wheels->tmr *
							(DEGtoANG(180) / LUCKY_WHEELS_LEVER_PULL_FRAMES)) *
						LUCKY_WHEELS_LEVER_PULL) +
				LUCKY_WHEELS_LEVER_Y;
		else if(wheels->tmr < LUCKY_WHEELS_LEVER_FRAMES)
			lever =
				FIXEDtoINT(
					sySin(wheels->tmr * (TRIG_TURN / 16) - TRIG_TURN * 3) * 4) +
				LUCKY_WHEELS_LEVER_Y;
	}
	wheels->oam[spr].attr01 = lever | objMode | OBJ_X(LUCKY_WHEELS_LEVER_X) |
		OBJ_SIZE_32X16 | OBJ_256_COLORS;
	wheels->oam[spr].attr2 = OBJ_PRIORITY(1) | LUCKY_WHEELS_LEVER_TILES;
	spr++;

	/* The lever's slot, in eight pieces from its top: two of its own, four
	   alike and two for its bottom. */
	for(k = 0; k != LUCKY_WHEELS_LEVER_SLOT_PIECES; k++) {
		wheels->oam[spr].attr01 = objMode |
			(k * 16 + LUCKY_WHEELS_LEVER_SLOT_Y) |
			OBJ_X(LUCKY_WHEELS_LEVER_SLOT_X) | OBJ_SIZE_32X16 | OBJ_256_COLORS;
		if(k == 0)
			wheels->oam[spr].attr2 =
				OBJ_PRIORITY(1) | LUCKY_WHEELS_LEVER_SLOT_TILES;
		else if(k == 1)
			wheels->oam[spr].attr2 = OBJ_PRIORITY(1) |
				(LUCKY_WHEELS_LEVER_SLOT_TILES + SPRITE_TILES_8BPP(32, 16));
		else if(k == 6)
			wheels->oam[spr].attr2 = OBJ_PRIORITY(1) |
				(LUCKY_WHEELS_LEVER_SLOT_TILES + 3 * SPRITE_TILES_8BPP(32, 16));
		else if(k == 7)
			wheels->oam[spr].attr2 = OBJ_PRIORITY(1) |
				(LUCKY_WHEELS_LEVER_SLOT_TILES + 4 * SPRITE_TILES_8BPP(32, 16));
		else
			wheels->oam[spr].attr2 = OBJ_PRIORITY(1) |
				(LUCKY_WHEELS_LEVER_SLOT_TILES + 2 * SPRITE_TILES_8BPP(32, 16));
		spr++;
	}

	for(; spr != OAM_SIZE / sizeof(OAM_WORDS); spr++) {
		wheels->oam[spr].attr01 =
			OBJ_X(DISPLAY_WIDTH) | OBJ_SIZE_16X16 | OBJ_256_COLORS;
		wheels->oam[spr].attr2 = 0;
	}

	syDMAXCopy(wheels->oam, OAM, DMA_COPY_WORDS(spr * 2), REG_DMA3);
}

/* The screen lines of the wheels' top and bottom edges, and the lines from each
   that fdLuckyWheelsSetUpBetting shades. */
#define LUCKY_WHEELS_WHEELS_TOP	   23
#define LUCKY_WHEELS_WHEELS_BOTTOM 110
#define LUCKY_WHEELS_SHADE_LINES   40
/* The darkening at the wheels' very edges, in BLDY's sixteenths. */
#define LUCKY_WHEELS_SHADE_LEVEL   6

/* Sets up the bouncing title letters of a win, the brightness wave of the
   screen's lines and the betting window, and starts at betting. */
void fdLuckyWheelsSetUpBetting(void)
{
	LUCKY_WHEELS_STATE	*wheels;
	BATTLE_EFFECT_STATE *effect;
	PARTICLE			*letter;
	const Uint8			*width;
	Sint32				 left;
	Sint32				 step;
	FIXED				 y;
	Sint32				 i;
	Sint32				 ang;
	WINDOW				*win;
	Sint32				 msg;

	wheels =
		((LUCKY_WHEELS_STATE **)BattleEffectStateWork)[LUCKY_WHEELS_HEAP_SLOT -
			HEAP_SLOT_BATTLE_EFFECT];
	effect = BattleEffectStateWork[0];
	left = 0;
	/* Twice the sparks the win uses are cleared. */
	for(i = 0; i != 2 * LUCKY_WHEELS_SPARK_COUNT; i++)
		LUCKY_WHEELS_SPARKS[i].variant = 0;

	letter = effect->particles;
	y = -toFIXED(32);
	width = LuckyWheelsTitleLetterWidths;
	for(i = 0; i != LUCKY_WHEELS_TITLE_LETTERS; i++) {
		letter[i].x = INTtoFIXED(left + LUCKY_WHEELS_TITLE_X);
		step = *width;
		width++;
		left += step;
		letter[i].y = y;
		letter[i].velY = 0;
		letter[i].variant = 0;
		y += -toFIXED(8);
	}

	for(i = 0; i != DISPLAY_HEIGHT; i++)
		wheels->scanlineOffsets[i] = 0;

	/* The wheels darken toward their top and bottom edges, as a drum turns
	   away:
	   BLDY falls from LUCKY_WHEELS_SHADE_LEVEL at each edge to 0 over a quarter
	   turn of the cosine. */
	for(i = 0; i != LUCKY_WHEELS_SHADE_LINES; i++) {
		ang = i * (DEGtoANG(90) / LUCKY_WHEELS_SHADE_LINES);
		wheels->scanlineOffsets[LUCKY_WHEELS_WHEELS_TOP + i] =
			FIXEDtoINT((Uint32)(syCos(ang) * LUCKY_WHEELS_SHADE_LEVEL));
		wheels->scanlineOffsets[LUCKY_WHEELS_WHEELS_BOTTOM - i] =
			FIXEDtoINT((Uint32)(syCos(ang) * LUCKY_WHEELS_SHADE_LEVEL));
	}

	wheels->spins = 0;
	wheels->state = LUCKY_WHEELS_BETTING;
	wheels->cursor = 0;
	effect->transferMode = CANVAS_COPY_AND_CLEAR;
	effect->transferValue = 0;
	REG_BLDCNT = 0;
	win = mnWindowCreateStub(
		18, 0, 12, 4, WINDOW_FLAG_INSTANT | WINDOW_FLAG_UNREAD);
	wheels->subWin = win;
	/* "Game ticket" above "Bet". */
	msg = MsgSlotsBet;
	mnTextDrawMessageAsTilesStub(msg, win, 0, 8);
	mnTextDrawMessageAsTilesStub(msg - 1, wheels->subWin, 0, 0);
}

/* The places on each wheel given a set symbol after the random fill: one of
   each plain symbol, then three moons. */
#define LUCKY_WHEELS_PLACED_SYMBOLS 8

/* Lucky Wheels: loads its backgrounds, sprites and palettes, fills each of the
   five wheels with 21 random symbols, among them each plain symbol and three
   moons, then runs until the player leaves: fading in, drawing the paylines on
   the canvas while betting or winning, and the jackpot's sparks and colour
   cycle while showing a win. Then fades out and frees its memory. */
void fdLuckyWheelsRun(void)
{
	Sint32				 picks[LUCKY_WHEELS_PLACED_SYMBOLS];
	Sint32				 litLines[LUCKY_WHEELS_PAYLINES];
	DRAW_RECTANGLE		 draw[2];
	Uint8				*sheet;
	void				*canvas;
	BATTLE_EFFECT_STATE *effect;
	LUCKY_WHEELS_STATE	*wheels;
	PARTY_HOLDINGS		*holdings;
	Uint8				*entry;
	Uint8				*tiles;
	Sint32				 frm;
	Uint32				 cnt;
	Sint32				 i;
	Sint32				 j;
	Sint32				 k;

	sheet = syHeapSlotAlloc(HEAP_SLOT_BATTLE_SHEET, GLOW_SHEET_ROOM);
	canvas = syHeapSlotAlloc(HEAP_SLOT_BATTLE_CANVAS, LUCKY_WHEELS_CANVAS_SIZE);
	effect = syHeapSlotAllocEWRAM(
		HEAP_SLOT_BATTLE_EFFECT, sizeof(BATTLE_EFFECT_STATE));
	wheels = syHeapSlotAllocEWRAM(
		LUCKY_WHEELS_HEAP_SLOT, sizeof(LUCKY_WHEELS_STATE));
	tiles = RamMapGridMetatileBuffer;
	holdings = &MainRoutineStateWork.holdings;
	syDummyNoOperationHook(RESOURCE_BATTLE_EFFECT_STUBS);
	holdings->wonPrizes[0] = LUCKY_WHEELS_PRIZE_LIST_END;
	wheels->repeat = 0;
	wheels->bet = 1;
	syInitEvent();
	OamCopyEnabled = 0;

	/* BG1 is the machine, its tiles in order but for the opening's. */
	cnt = 0;
	j = 0;
	for(k = 0; k != DISPLAY_TILE_HEIGHT; k++) {
		for(i = 0; i != BG_SCREEN_WIDTH; i++, j += 2) {
			if(LUCKY_WHEELS_IN_OPENING(i, k)) {
				*(volatile Uint16 *)(BG_SCREEN_BLOCK(5) + j) =
					BG_TILE_PALETTE(LUCKY_WHEELS_MACHINE_PALETTE) |
					LUCKY_WHEELS_CLEAR_TILE;
			} else if(i >= DISPLAY_TILE_WIDTH) {
				*(volatile Uint16 *)(BG_SCREEN_BLOCK(5) + j) = 0;
			} else {
				*(volatile Uint16 *)(BG_SCREEN_BLOCK(5) + j) =
					(BG_TILE_PALETTE(LUCKY_WHEELS_MACHINE_PALETTE) |
						LUCKY_WHEELS_MACHINE_TILE) +
					cnt;
				cnt++;
			}
		}
	}
	syDecompressTaggedLZ(
		syResourceGetTableEntry(RESOURCE_BLUE_GLOW_SHEET), sheet);
	entry = syResourceGetTableEntry(RESOURCE_LUCKY_WHEELS_MACHINE_PICTURE);
	syDMAXCopy(entry, (void *)&BG_PLTT_COLOR(LUCKY_WHEELS_MACHINE_PALETTE, 0),
		DMA_COPY_WORDS(PALETTE_BYTES / 4), REG_DMA3);
	entry += PALETTE_BYTES;
	syDecompressTaggedLZ(entry, tiles);
	j = 0;
	for(k = 0; k != DISPLAY_TILE_HEIGHT; k++) {
		for(i = 0; i != DISPLAY_TILE_WIDTH; i++) {
			if(!LUCKY_WHEELS_IN_OPENING(i, k)) {
				syDMAXCopy(
					tiles + (k * DISPLAY_TILE_WIDTH + i) * TILE_SIZE_4BPP,
					(void *)(BG_CHAR_BLOCK(2) +
						LUCKY_WHEELS_MACHINE_TILE * TILE_SIZE_4BPP + j),
					DMA_COPY_WORDS(TILE_SIZE_4BPP / 4), REG_DMA3);
				j += TILE_SIZE_4BPP;
			}
		}
	}
	/* Clear BG1's map below the screen, then show the canvas on BG2. */
	syWordsClear(BG_SCREEN_BLOCK(5) +
			DISPLAY_TILE_HEIGHT * BG_SCREEN_WIDTH * sizeof(Uint16),
		BG_SCREEN_SIZE -
			DISPLAY_TILE_HEIGHT * BG_SCREEN_WIDTH * sizeof(Uint16));
	j = 0;
	for(k = 0; k != DISPLAY_TILE_HEIGHT; k++) {
		for(i = 0; i != BG_SCREEN_WIDTH; i++, j += 2) {
			if(k < LUCKY_WHEELS_CANVAS_TOP_ROW ||
				k > LUCKY_WHEELS_CANVAS_BOTTOM_ROW)
				*(volatile Uint16 *)(BG_SCREEN_BLOCK(6) + j) =
					LUCKY_WHEELS_CANVAS_CLEAR_TILE;
			else
				*(volatile Uint16 *)(BG_SCREEN_BLOCK(6) + j) =
					(k - LUCKY_WHEELS_CANVAS_TOP_ROW) * BG_SCREEN_WIDTH + i +
					LUCKY_WHEELS_CANVAS_FIRST_TILE;
		}
	}
	REG_BG1CNT = BGCNT_SCREEN_BASE(5) | BGCNT_CHAR_BASE(2) | BGCNT_PRIORITY(1);
	REG_BG2CNT = BGCNT_256_COLORS | BGCNT_SCREEN_BASE(6);
	BgScrollWork[0].x = 0;
	BgScrollWork[0].y = 0;
	BgScrollWork[1].x = 0;
	BgScrollWork[1].y = 0;
	BgScrollWork[2].x = 0;
	BgScrollWork[2].y = 0;
	REG_WININ = WININ_WIN0_BG0 | WININ_WIN0_BG1 | WININ_WIN0_BG2 |
		WININ_WIN0_OBJ | WININ_WIN0_CLR | WININ_WIN1_BG0 | WININ_WIN1_BG1 |
		WININ_WIN1_BG2 | WININ_WIN1_OBJ | WININ_WIN1_CLR;
	REG_WINOUT = WINOUT_WIN01_BG0 | WINOUT_WIN01_BG1 | WINOUT_WIN01_BG2 |
		WINOUT_WIN01_CLR | WINOUT_WINOBJ_BG0 | WINOUT_WINOBJ_BG1 |
		WINOUT_WINOBJ_BG2 | WINOUT_WINOBJ_CLR;
	REG_BLDCNT = BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
	REG_BLDALPHA = BLEND_ALPHA(16, 16);
	REG_BG1HOFS = 0;
	REG_BG2HOFS = 0;
	REG_BG1VOFS = -DISPLAY_HEIGHT;
	REG_BG2VOFS = -DISPLAY_HEIGHT;
	/* DEAD: a window round the opening, replaced at once by the whole screen;
	   the ROM keeps both stores. */
	REG_WIN0H = WIN_RANGE(40, 200);
	REG_WIN0V = WIN_RANGE(24, 120);
	REG_WIN0H = WIN_RANGE(0, DISPLAY_WIDTH);
	REG_WIN0V = WIN_RANGE(0, DISPLAY_HEIGHT);
	REG_WIN1H = WIN_RANGE(0, DISPLAY_WIDTH);
	REG_WIN1V = WIN_RANGE(0, DISPLAY_HEIGHT);

	{
		/* The palettes as loaded, kept at the start of the effect's sheet to
		   fade from: the background's, then the sprites'. */
		Uint16 *bgShade = (Uint16 *)effect->sheet;
		Uint16 *shade = (Uint16 *)(effect->sheet + BG_PLTT_SIZE);
		Sint32	size = LUCKY_WHEELS_CANVAS_SIZE;

		wheels->state = LUCKY_WHEELS_BETTING;
		wheels->cursor = 0;
		wheels->spins = 0;
		effect->frm = 0;
		wheels->tmr = 0;
		syDMAXCopy(syResourceGetTableEntry(RESOURCE_JUPITER_DJINN_SHEET),
			(void *)BG_PLTT, DMA_COPY_WORDS(LUCKY_WHEELS_SPARK_COLORS / 2),
			REG_DMA3);
		BG_PLTT[LUCKY_WHEELS_LINE_COLOR] = RGB(11, 28, 11);
		BG_PLTT[LUCKY_WHEELS_LINE_LIGHT] = RGB(22, 31, 22);
		entry = syResourceGetTableEntry(RESOURCE_LUCKY_WHEELS_ICONS);
		syDMAXCopy(entry, (void *)OBJ_PLTT,
			DMA_COPY_WORDS(LUCKY_WHEELS_ICON_COLORS / 2), REG_DMA3);
		entry += LUCKY_WHEELS_ICON_COLORS * 2;
		syDecompressTaggedLZ(entry, tiles);
		syDMAXCopy(tiles, (void *)OBJ_VRAM0, DMA_COPY_WORDS(6960), REG_DMA3);
		entry = syResourceGetTableEntry(RESOURCE_LUCKY_WHEELS_JACKPOT);
		syDMAXCopy(entry,
			(void *)&OBJ_PLTT_COLOR(LUCKY_WHEELS_JACKPOT_PALETTE, 0),
			DMA_COPY_WORDS(PALETTE_BYTES / 4), REG_DMA3);
		entry += PALETTE_BYTES;
		syDecompressTaggedLZ(entry, tiles);
		syDMAXCopy(tiles, (void *)LUCKY_WHEELS_JACKPOT_TILES,
			LUCKY_WHEELS_JACKPOT_COPY, REG_DMA3);
		mnWindowSystemInitFieldStub();
		fdLuckyWheelsSetUpBetting();

		for(i = 0; i != LUCKY_WHEELS_REEL_COUNT; i++) {
			REEL *reel = &wheels->reels[i];

			reel->pos = LUCKY_WHEELS_SYMBOL_HEIGHT / 2;
			reel->held = 0;
			reel->stop = -1;
			for(k = 0; k != LUCKY_WHEELS_REEL_SYMBOLS; k++)
				/* One of the plain symbols, all numbered below the wild one. */
				reel->symbols[k] = syRandom() % LUCKY_WHEELS_WILD_SYMBOL;
		}
		/* Then eight different places on each wheel take one of each plain
		   symbol and three moons, so every symbol is on every wheel. */
		for(i = 0; i != LUCKY_WHEELS_REEL_COUNT; i++) {
			REEL *reel = &wheels->reels[i];

			for(j = 0; j != LUCKY_WHEELS_PLACED_SYMBOLS; j++) {
				picks[j] = syRandom() % LUCKY_WHEELS_REEL_SYMBOLS;
				for(k = 0; k != j; k++) {
					if(picks[j] == picks[k]) {
						j--;
						break;
					}
				}
			}
			for(k = 0; k != LUCKY_WHEELS_PLACED_SYMBOLS; k++) {
				Sint32 symbol = k;

				if(symbol > LUCKY_WHEELS_WILD_SYMBOL)
					symbol = LUCKY_WHEELS_WILD_SYMBOL;
				reel->symbols[picks[k]] = symbol;
			}
		}

		btEffectBuildBlitterStub(HEAP_SLOT_RAM_CODE, 8, 7, 3, 2);
		draw[0] = HeapSlots[HEAP_SLOT_RAM_CODE];
		btEffectBuildBlitterStub(HEAP_SLOT_RAM_CODE_PAIRED, 8, 7, 3, 3);
		draw[1] = HeapSlots[HEAP_SLOT_RAM_CODE_PAIRED];
		syWordsFill(canvas, size, 0);
		syDMAXCopy(canvas, (void *)LUCKY_WHEELS_CANVAS_TILES,
			DMA_COPY_WORDS(LUCKY_WHEELS_CANVAS_SIZE / 4), REG_DMA3);
		syDMAXCopy((void *)BG_PLTT, bgShade, DMA_COPY_WORDS(BG_PLTT_SIZE / 4),
			REG_DMA3);
		syDMAXCopy((void *)OBJ_PLTT, shade, DMA_COPY_WORDS(BG_PLTT_SIZE / 4),
			REG_DMA3);
		fdPaletteScaleRGB555(shade, (Uint16 *)OBJ_PLTT, 0, BG_PLTT_COLORS);
		fdPaletteScaleRGB555(bgShade, (Uint16 *)BG_PLTT, 0, BG_PLTT_COLORS);
		REG_DISPCNT = DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON | DISPCNT_BG1_ON |
			DISPCNT_BG2_ON | DISPCNT_OBJ_ON | DISPCNT_WIN0_ON;
		if(gmPartyInventoryCountItemStub(ITEM_GAME_TICKET) == 1) {
			WINDOW *win = mnWindowCreateStub(
				6, 16, 18, 3, WINDOW_FLAG_INSTANT | WINDOW_FLAG_UNREAD);

			wheels->win = win;
			mnTextDrawMessageAsTilesStub(MsgSlotsControls, win, 0, 0);
		} else {
			WINDOW *win = mnWindowCreateStub(LUCKY_WHEELS_HELP_WINDOW_X, 16,
				LUCKY_WHEELS_HELP_WINDOW_WIDTH, 4,
				WINDOW_FLAG_INSTANT | WINDOW_FLAG_UNREAD);
			Sint32	msg;

			wheels->win = win;
			/* "Press up or down to change value" above the controls. */
			msg = MsgSlotsChangeBet;
			mnTextDrawMessageAsTilesStub(msg, win, 0, 0);
			mnTextDrawMessageAsTilesStub(msg + 1, wheels->win, 0, 8);
		}
		effect->transferPending = 0;
		sySetEvent(fdLuckyWheelsUpdate, EVENT_PASS_VBLANK);
		sySetEvent(fdLuckyWheelsTransferCanvas, EVENT_PASS_VBLANK);

		for(frm = 0; wheels->state != LUCKY_WHEELS_DONE; frm++) {
			if(frm <= LUCKY_WHEELS_FADE_TICKS) {
				fdPaletteScaleRGB555(shade, (Uint16 *)OBJ_PLTT,
					frm * (toFIXED(1) / LUCKY_WHEELS_FADE_TICKS),
					BG_PLTT_COLORS);
				fdPaletteScaleRGB555(bgShade, (Uint16 *)BG_PLTT,
					frm * (toFIXED(1) / LUCKY_WHEELS_FADE_TICKS),
					BG_PLTT_COLORS);
			}
			if(wheels->state == LUCKY_WHEELS_SHOWING_WIN) {
				Sint32 phase = frm %
					(LUCKY_WHEELS_CYCLE_PALETTES * LUCKY_WHEELS_CYCLE_TICKS);

				if(phase < LUCKY_WHEELS_CYCLE_TICKS)
					fdLuckyWheelsStepSparkCols(RESOURCE_MERCURY_DJINN_SHEET);
				else if(phase < LUCKY_WHEELS_CYCLE_TICKS * 2)
					fdLuckyWheelsStepSparkCols(RESOURCE_VENUS_DJINN_SHEET);
				else if(phase < LUCKY_WHEELS_CYCLE_TICKS * 3)
					fdLuckyWheelsStepSparkCols(RESOURCE_EMBER_STREAK_SHEET);
				else if(phase < LUCKY_WHEELS_CYCLE_TICKS * 4)
					fdLuckyWheelsStepSparkCols(RESOURCE_LIME_PALETTE);
				else
					fdLuckyWheelsStepSparkCols(RESOURCE_JUPITER_DJINN_SHEET);
				if(wheels->tmr < LUCKY_WHEELS_FADE_TICKS)
					fdLuckyWheelsDarkenStep();
				if(wheels->tmr > LUCKY_WHEELS_FADE_TICKS && (frm & 7) == 0) {
					Sint32	  x = (syRandom() & 127) + 56;
					Sint32	  y = (syRandom() & 31) + 48;
					PARTICLE *spark = &LUCKY_WHEELS_SPARKS[(frm / 8 & 3) *
						LUCKY_WHEELS_BURST_SPARKS];

					for(j = 0; j != LUCKY_WHEELS_BURST_SPARKS; j++) {
						Sint32 speed;
						Sint32 ang;

						speed = (syRandom() & 255) + 64;
						ang = syRandom() & TRIG_ANGLE_MASK;
						spark->x = toFIXED(x);
						spark->y = toFIXED(y);
						spark->velX = sySin(ang) * speed >> 6;
						spark->velY = -(syCos(ang) * speed) >> 6;
						spark->variant = (syRandom() & 15) + 16;
						spark++;
					}
				}
				for(j = 0; j != LUCKY_WHEELS_SPARK_COUNT; j++) {
					PARTICLE *spark = &LUCKY_WHEELS_SPARKS[j];

					if(spark->variant > 0) {
						Sint32 x;

						spark->variant--;
						x = spark->x;
						if(x >= 0 && x <= toFIXED(256) - 1) {
							Sint32 y = spark->y;

							if(y <= toFIXED(128) - 1) {
								if(y >= 0) {
									Sint32 drawX = FIXEDtoINT(x);
									Sint32 drawY = FIXEDtoINT(y);
									Sint32 size = spark->variant /
											LUCKY_WHEELS_SPARK_SHRINK_TICKS +
										1;

									draw[j & 1](canvas,
										sheet +
											LuckyWheelsSparkPictures[size - 1],
										drawX - size, drawY - size, size * 2,
										size * 2);
								}
							}
						}
						spark->x += spark->velX;
						spark->y += spark->velY;
						spark->velX = spark->velX * 60 / DAMPING_SCALE;
						spark->velY = spark->velY * 60 / DAMPING_SCALE;
					}
				}
			}
			/* The paylines bet on while betting, or those won, blinking. */
			if(wheels->state == LUCKY_WHEELS_BETTING ||
				wheels->state == LUCKY_WHEELS_WON) {
				for(j = 0; j != LUCKY_WHEELS_PAYLINES; j++)
					litLines[j] = 0;
				if(wheels->state == LUCKY_WHEELS_BETTING) {
					litLines[LUCKY_WHEELS_MIDDLE_LINE] = 1;
					if(wheels->bet > 1) {
						litLines[LUCKY_WHEELS_MIDDLE_LINE + 1] = 1;
						litLines[LUCKY_WHEELS_MIDDLE_LINE - 1] = 1;
					}
					if(wheels->bet > 2) {
						litLines[LUCKY_WHEELS_MIDDLE_LINE + 2] = 1;
						litLines[LUCKY_WHEELS_MIDDLE_LINE - 2] = 1;
					}
					if(wheels->bet > 3) {
						litLines[LUCKY_WHEELS_MIDDLE_LINE + 3] = 1;
						litLines[LUCKY_WHEELS_MIDDLE_LINE - 3] = 1;
					}
				} else if((frm & 7) <= 3) {
					for(j = 0; j != LUCKY_WHEELS_PAYLINES; j++)
						litLines[j] = wheels->winningLines[j];
				}
				/* Each three pixels thick, its middle the lighter green. */
				for(j = 0; j != 3; j++) {
					Sint32 col = j != 1;

					col = LUCKY_WHEELS_LINE_LIGHT - col;
					if(litLines[1])
						fdLuckyWheelsDrawCanvasLine(
							20, j + 19, 200, j + 19, col);
					if(litLines[2])
						fdLuckyWheelsDrawCanvasLine(
							28, j + 35, 200, j + 35, col);
					if(litLines[3])
						fdLuckyWheelsDrawCanvasLine(
							20, j + 51, 200, j + 51, col);
					if(litLines[4])
						fdLuckyWheelsDrawCanvasLine(
							28, j + 67, 200, j + 67, col);
					if(litLines[5])
						fdLuckyWheelsDrawCanvasLine(
							20, j + 83, 200, j + 83, col);
					if(litLines[0])
						fdLuckyWheelsDrawCanvasLine(
							28, j + 5, 200, j + 91, col);
					if(litLines[6])
						fdLuckyWheelsDrawCanvasLine(
							28, j + 97, 200, j + 11, col);
				}
			}
			effect->transferPending = 1;
			sySynch(1);
		}

		for(j = 0; j != LUCKY_WHEELS_FADE_TICKS + 1; j++) {
			fdPaletteScaleRGB555(shade, (Uint16 *)OBJ_PLTT,
				toFIXED(1) - j * (toFIXED(1) / LUCKY_WHEELS_FADE_TICKS),
				BG_PLTT_COLORS);
			fdPaletteScaleRGB555(bgShade, (Uint16 *)BG_PLTT,
				toFIXED(1) - j * (toFIXED(1) / LUCKY_WHEELS_FADE_TICKS),
				BG_PLTT_COLORS);
			sySynch(1);
		}
		syHeapSlotRelease(HEAP_SLOT_RAM_CODE_PAIRED);
		syHeapSlotRelease(HEAP_SLOT_RAM_CODE);
		syCloseEvent(fdLuckyWheelsTransferCanvas);
		syCloseEvent(fdLuckyWheelsUpdate);
		syHeapSlotRelease(LUCKY_WHEELS_HEAP_SLOT);
		syHeapSlotRelease(HEAP_SLOT_BATTLE_CANVAS);
		syHeapSlotRelease(HEAP_SLOT_BATTLE_EFFECT);
		syHeapSlotRelease(HEAP_SLOT_BATTLE_SHEET);
	}
}

/* The LZ packer: a window of PACK_WINDOW_SIZE bytes with a hash chain for each
   byte value, PACK_LOOKAHEAD bytes read ahead of the one being packed and the
   PACK_HISTORY behind it kept in the chains, and matches of up to
   PACK_MATCH_MAX bytes at most PACK_DISTANCE_MAX back. */
#define PACK_WINDOW_SIZE		1024
#define PACK_WINDOW_MASK		(PACK_WINDOW_SIZE - 1)
#define PACK_BYTE_VALUES		256
#define PACK_LOOKAHEAD			672
#define PACK_HISTORY			60
#define PACK_DISTANCE_MAX		63
/* A match of up to 16 bytes takes two bytes; a longer one three, its length
   less 17 in the third, up to PACK_MATCH_MAX. */
#define PACK_SHORT_MATCH_MAX	16
#define PACK_MATCH_MAX			272
/* A group's flag byte is read from its top bit down, a bit an item. */
#define PACK_FIRST_FLAG			(1 << 7)
/* A match's code, a halfword written high byte first: its distance's bits from
   8 up in bits 12 to 15, its length less one in bits 8 to 11 for a short match,
   and its distance's low byte. */
#define PACK_CODE_DISTANCE_HIGH (~0xfff)
#define PACK_CODE_LENGTH		0xf00
#define PACK_CODE_DISTANCE_LOW	0xff

/* Empties the LZ packer's hash chains and numbers its window nodes. */
void fdLZPackResetChains(void)
{
	Sint32		   idx;
	LZ_PACK_NODE **bucket;

	for(idx = 0; idx < PACK_WINDOW_SIZE; idx++) {
		LzPackStateWork->nodes[idx].slot = idx;
		LzPackStateWork->nodes[idx].prev = NULL;
	}
	bucket = LzPackStateWork->bucket;
	for(idx = 0; idx < PACK_BYTE_VALUES; idx++) {
		*bucket++ = NULL;
	}
}

#include "field/lz_pack_slot_nodes.c"

/* Reads count more input bytes into the window from position start, dropping
   the bytes they replace from their chains; past the end of the input the
   positions are marked empty (-1). */
void fdLZPackReadInput(Sint32 start, Sint32 cnt, const Uint8 *input)
{
	Sint32 cur = 0;

	if(cur < cnt) {
		Uint32 mask = PACK_WINDOW_MASK;
		/* The byte PACK_HISTORY behind the one being packed leaves its
		   chain. */
		Uint32 removal =
			start + PACK_WINDOW_SIZE - PACK_LOOKAHEAD - PACK_HISTORY;

		do {
			Uint32		   slot = start + cur;
			LZ_PACK_STATE *state;
			Uint32		   readOfs;
			Uint32		   nextOfs;
			Uint8		   value;

			fdLZPackRemoveSlotNode(removal & mask);
			state = LzPackStateWork;
			readOfs = state->inputCursor;
			value = input[readOfs];
			nextOfs = readOfs + 1;
			state->inputCursor = nextOfs;
			if(nextOfs == state->inputLimit) {
				state->bucketBySlot[slot & mask] = -1;
				break;
			}

			state->bucketBySlot[slot & mask] = value;
			cur++;
			fdLZPackInsertSlotNode(slot & mask);
			removal++;
		} while(cur < cnt);
	}

	cur++;
	if(cur < cnt) {
		Uint32 mask = PACK_WINDOW_MASK;

		do {
			Uint32		   slot = (start + cur) & mask;
			LZ_PACK_STATE *state;

			fdLZPackRemoveSlotNode(slot);
			state = LzPackStateWork;
			cur++;
			state->bucketBySlot[slot] = -1;
		} while(cur < cnt);
	}
}

#include "field/lz_pack_window_match.c"

/* Unused: packs size bytes of input into dst with a small LZ scheme and returns
   the packed size. Each group starts with a flag byte, a set bit marking a
   match: two bytes of distance and length (lengths 2 to 16), or three for
   lengths up to 272; a match is put off by one byte when the next position
   matches further. A zero match ends the stream. */
UNUSED Sint32 fdLZPackPackData(const Uint8 *input, Uint8 *dst, Sint32 size)
{
	Sint32 defer;
	Sint32 len;
	Sint32 saveOfs;
	Sint32 saveLen;
	Sint32 reach[2];
	Sint16 code;

	defer = 0;
	LzPackStateWork = (LZ_PACK_STATE *)MapGridMetatileBuffer;
	fdLZPackResetChains();
	LzPackStateWork->inputLimit = size;
	LzPackStateWork->pos = 0;
	LzPackStateWork->inputCursor = 0;
	LzPackStateWork->outTotal = 0;
	LzPackStateWork->flgMask = PACK_FIRST_FLAG;
	LzPackStateWork->outBuf[0] = 0;
	LzPackStateWork->outCnt = 1;
	fdLZPackReadInput(0, PACK_LOOKAHEAD, input);

	while(LzPackStateWork->bucketBySlot[LzPackStateWork->pos] != -1) {
		fdLZPackFindWindowMatch(LzPackStateWork->pos);
		if(defer == 0) {
			len = LzPackStateWork->matchLen;
			if(LzPackStateWork->matchLen > 1) {
				saveOfs = LzPackStateWork->matchOfs;
				saveLen = len;
				fdLZPackFindWindowMatch(
					(LzPackStateWork->pos + 1) & PACK_WINDOW_MASK);
				if(LzPackStateWork->matchLen > 2) {
					reach[0] = LzPackStateWork->matchLen + 1;
					fdLZPackFindWindowMatch(
						(LzPackStateWork->pos + len) & PACK_WINDOW_MASK);
					reach[1] = LzPackStateWork->matchLen + len;
					if(reach[0] >= reach[1]) {
						saveLen = 1;
						defer = 1;
					}
				}
				LzPackStateWork->matchOfs = saveOfs;
				LzPackStateWork->matchLen = saveLen;
			}
		}

		if(LzPackStateWork->matchLen > 1) {
			defer = 0;
			LzPackStateWork->outBuf[0] |= LzPackStateWork->flgMask;
			if(LzPackStateWork->matchLen <= PACK_SHORT_MATCH_MAX) {
				code = ((LzPackStateWork->matchOfs << 4) &
						   PACK_CODE_DISTANCE_HIGH) |
					(LzPackStateWork->matchOfs & PACK_CODE_DISTANCE_LOW) |
					(((LzPackStateWork->matchLen - 1) << 8) & PACK_CODE_LENGTH);
				LzPackStateWork->outBuf[LzPackStateWork->outCnt++] =
					(Uint16)code >> 8;
				LzPackStateWork->outBuf[LzPackStateWork->outCnt++] = code;
			} else {
				code = ((LzPackStateWork->matchOfs << 4) &
						   PACK_CODE_DISTANCE_HIGH) |
					(LzPackStateWork->matchOfs & PACK_CODE_DISTANCE_LOW);
				LzPackStateWork->outBuf[LzPackStateWork->outCnt++] =
					(Uint16)code >> 8;
				LzPackStateWork->outBuf[LzPackStateWork->outCnt++] = code;
				LzPackStateWork->outBuf[LzPackStateWork->outCnt++] =
					LzPackStateWork->matchLen - (PACK_SHORT_MATCH_MAX + 1);
			}
		} else {
			LzPackStateWork->outBuf[LzPackStateWork->outCnt++] =
				LzPackStateWork->bucketBySlot[LzPackStateWork->pos];
			LzPackStateWork->matchLen = 1;
		}

		fdLZPackReadInput(LzPackStateWork->pos + PACK_LOOKAHEAD,
			LzPackStateWork->matchLen, input);
		LzPackStateWork->pos =
			(LzPackStateWork->pos + LzPackStateWork->matchLen) &
			PACK_WINDOW_MASK;
		LzPackStateWork->flgMask >>= 1;
		if(LzPackStateWork->flgMask == 0) {
			fdLZPackCopyBufferedBytes(dst);
			LzPackStateWork->flgMask = PACK_FIRST_FLAG;
			LzPackStateWork->outBuf[0] = 0;
			LzPackStateWork->outCnt = 1;
		}
	}

	LzPackStateWork->outBuf[0] |= LzPackStateWork->flgMask;
	LzPackStateWork->outBuf[LzPackStateWork->outCnt++] = 0;
	LzPackStateWork->outBuf[LzPackStateWork->outCnt++] = 0;
	fdLZPackCopyBufferedBytes(dst);
	return LzPackStateWork->outTotal;
}
