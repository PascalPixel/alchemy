#ifndef ALCHEMY_DICE_GAME_H
#define ALCHEMY_DICE_GAME_H

/* The dice games, Lucky Dice and Super Lucky Dice, and Lucky Wheels: their
   states as their drawing reads them, the dice and the reels. */

#include "fixed_math.h"
#include "types.h"
#include "tla_edition.h"
#include "field/particle.h"

struct WINDOW;

/* One sprite's two OAM words, volatile because they are built in place for the
   DMA to copy to OAM. */
typedef struct OAM_WORDS {
	volatile Uint32 attr01; /* attr0 | attr1 << 16 */
	volatile Uint32 attr2;	/* attr2 | affine value << 16 */
} OAM_WORDS;

/* A die of the dice game: its place and velocity, 16.16, and its variant. */
typedef struct DICE_PARTICLE {
	FIXED  x;
	FIXED  y;
	FIXED  z;
	FIXED  velX;
	FIXED  velY;
	FIXED  velZ;
	Sint32 variant;
} DICE_PARTICLE;

/* The dice game's four dice. */
typedef struct DICE_STATE {
	DICE_PARTICLE particles[4];
} DICE_STATE;

/* The dice game's state as its drawing reads it: 128 OAM entries and its coins,
   bet and help windows. */
typedef struct DICE_SPR {
	Uint8		   filler[196];
	OAM_WORDS	   oam[128];
	struct WINDOW *coinsWin;
	struct WINDOW *betWin;
	struct WINDOW *helpWin;
} DICE_SPR;

/* The dice games' camera: the eye and the point it looks at, 16.16. */
typedef struct DICE_CAMERA {
	FIXED eye[3];
	FIXED focus[3];
} DICE_CAMERA;

/* The dice games' numbers, Lucky Dice's and Super Lucky Dice's alike: a die's
   faces. */
#define DICE_FACE_COUNT		  6
/* A throw costs ten coins for each level of the party's average. */
#define DICE_BET_PER_LEVEL	  10
/* The coins, and Super Lucky Dice's bet, are drawn with six digits. */
#define DICE_COIN_DIGITS	  6
/* The dice screen fades in and out over 16 ticks, a sixteenth of full
   brightness a tick. */
#define DICE_FADE_TICKS		  16
/* The depths from the camera a die or its shadow is drawn between: nearer or
   further is drawn as at the limit, and the nearer, the larger. */
#define DICE_NEAREST_DEPTH	  350
#define DICE_FARTHEST_DEPTH	  650
/* Where the dice's shadow starts in OBJ VRAM, in 16-colour tiles (32 bytes,
   half a 256-colour tile): after the rolling die's eighteen 256-colour
   views. */
#define DICE_SHADOW_TILE	  144
/* A thrown die starts spinning at this rate, its roll angle a tick; each
   bounce takes DICE_BOUNCE_SPIN_LOSS off it. */
#define DICE_THROW_SPIN		  300
#define DICE_BOUNCE_SPIN_LOSS 20
/* A die's roll angle (its particle's variant) crosses a face in
   DICE_TILES_PER_FACE tiles: the tile is angle / DICE_ROLL_PER_TILE and the
   face angle / DICE_ROLL_PER_FACE. It is the die's own unit, a whole roll
   DICE_ROLL_TURN, not a TRIG_TURN angle. */
#define DICE_TILES_PER_FACE	  3
#define DICE_ROLL_PER_TILE	  1024
#define DICE_ROLL_PER_FACE	  (DICE_TILES_PER_FACE * DICE_ROLL_PER_TILE)
#define DICE_ROLL_TURN		  (DICE_FACE_COUNT * DICE_ROLL_PER_FACE) /* six faces */
/* A die's or a shadow's affine scale at DICE_NEAREST_DEPTH, in 256ths: drawn at
   twice its size, and smaller the further past it. */
#define DICE_NEAREST_SCALE	  128

/* Lucky Dice's state as its drawing reads it: 128 OAM entries and its coins,
   winnings and help windows. */
typedef struct LUCKY_DICE_SPR {
	Uint8		   filler[196];
	OAM_WORDS	   oam[128];
	struct WINDOW *coinsWin;
	struct WINDOW *winningsWin;
	struct WINDOW *helpWin;
} LUCKY_DICE_SPR;

/* A Lucky Wheels reel: its turn, its 21 symbols, whether it is held and where
   it stops. */
typedef struct REEL {
	Sint32 pos;
	Uint8  symbols[21];
	Uint8  held;
	Sint8  stop;
} REEL;

/* Lucky Wheels' effect state: the title letters' particles, the BG and OBJ
   shades, and its transfer to the screen. */
typedef struct REEL_EFFECT_STATE {
	PARTICLE particles[LUCKY_WHEELS_TITLE_LETTERS];
	/* No code names it. */
	Uint8  unusedAfterParticles[1792 -
		 LUCKY_WHEELS_TITLE_LETTERS * sizeof(PARTICLE)];
	Uint16 bgShade[256];
	Uint16 sprShade[256];
	Uint8  unusedAfterSprShade[27776]; /* no code names it */
	Sint32 transferMode;
	Sint32 transferValue;
	Uint8  unusedAfterTransferValue[4]; /* no code names it */
	Sint32 frm;
	Uint8  unusedAfterFrm[344]; /* no code names it */
	Sint32 transferPending;
} REEL_EFFECT_STATE;

/* The main routine's state from PartyDjinnRecovery on, as Lucky Wheels reads
   it: the prizes won, kept after the spring game's prize weights. */
typedef struct REEL_SAVE {
	/* summons, coins, Djinn recovery, shops, prize weights */
	Uint8 unused[320];
	Sint8 wonPrizes[16];
} REEL_SAVE;

/* The Lucky Dice module's functions in the order of its stub table
   (src/field/lucky_dice_stubs.s). Starts the Lucky Dice minigame: leaves only
   the sprites on screen, starts the music and plays until the player quits.
   Returns 0. */
Sint32 fdLuckyDiceStart(void);

/* The Lucky Wheels module's functions in the order of its stub table
   (src/field/lucky_wheels_stubs.s). Plays Lucky Wheels: blanks the screen,
   seeds the random numbers with the play time and runs the game. Returns 0. */
Sint32 fdLuckyWheelsStart(void);

/* The Super Lucky Dice module's functions in the order of its stub table
   (src/field/super_lucky_dice_stubs.s). Runs Super Lucky Dice, every layer off
   until it sets up its own display, and returns 0. */
Sint32 fdSuperLuckyDiceStart(void);

#endif
