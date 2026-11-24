#ifndef ALCHEMY_DICE_GAME_H
#define ALCHEMY_DICE_GAME_H

/// @file
/// The dice games, Lucky Dice and Super Lucky Dice, and Lucky Wheels: their
/// states as their drawing reads them, the dice and the reels.

#include "fixed_math.h"
#include "types.h"
#include "tla_edition.h"
#include "field/particle.h"

struct Window;

/// One sprite's two OAM words, volatile because they are built in place for the
/// DMA to copy to OAM.
struct OamWords {
  volatile u32 attr01; ///< attr0 | attr1 << 16
  volatile u32 attr2;  ///< attr2 | affine value << 16
};

/// A die of the dice game: its place and velocity, 16.16, and its variant.
struct DiceParticle {
  FIXED x;
  FIXED y;
  FIXED z;
  FIXED velocityX;
  FIXED velocityY;
  FIXED velocityZ;
  s32 variant;
};

/// The dice game's four dice.
struct DiceState {
  struct DiceParticle particles[4];
};

/// The dice game's state as its drawing reads it: 128 OAM entries and its
/// coins, bet and help windows.
struct DiceSprites {
  u8 filler[196];
  struct OamWords oam[128];
  struct Window *coinsWindow;
  struct Window *betWindow;
  struct Window *helpWindow;
};

/// The dice games' camera: the eye and the point it looks at, 16.16.
struct DiceCamera {
  FIXED eye[3];
  FIXED focus[3];
};

/// The dice games' numbers, Lucky Dice's and Super Lucky Dice's alike: a die's
/// faces.
#define DICE_FACE_COUNT 6
/// A throw costs ten coins for each level of the party's average.
#define DICE_BET_PER_LEVEL 10
/// The coins, and Super Lucky Dice's bet, are drawn with six digits.
#define DICE_COIN_DIGITS 6
/// The dice screen fades in and out over 16 frames, a sixteenth of full
/// brightness a frame.
#define DICE_FADE_FRAMES 16
/// The depths from the camera a die or its shadow is drawn between: nearer or
/// further is drawn as at the limit, and the nearer, the larger.
#define DICE_NEAREST_DEPTH 350
#define DICE_FARTHEST_DEPTH 650
/// Where the dice's shadow starts in OBJ VRAM, in 16-colour tiles (32 bytes,
/// half a 256-colour tile): after the rolling die's eighteen 256-colour views.
#define DICE_SHADOW_TILE 144
/// A thrown die starts spinning at this rate, its roll angle a frame; each
/// bounce takes DICE_BOUNCE_SPIN_LOSS off it.
#define DICE_THROW_SPIN 300
#define DICE_BOUNCE_SPIN_LOSS 20
/// A die's roll angle (its particle's variant) crosses a face in
/// DICE_TILES_PER_FACE tiles: the tile is angle / DICE_ROLL_PER_TILE and the
/// face angle / DICE_ROLL_PER_FACE. It is the die's own unit, a whole roll
/// DICE_ROLL_TURN, not a TRIG_TURN angle.
#define DICE_TILES_PER_FACE 3
#define DICE_ROLL_PER_TILE 1024
#define DICE_ROLL_PER_FACE (DICE_TILES_PER_FACE * DICE_ROLL_PER_TILE)
#define DICE_ROLL_TURN (DICE_FACE_COUNT * DICE_ROLL_PER_FACE) ///< six faces
/// A die's or a shadow's affine scale at DICE_NEAREST_DEPTH, in 256ths: drawn
/// at twice its size, and smaller the further past it.
#define DICE_NEAREST_SCALE 128

/// Lucky Dice's state as its drawing reads it: 128 OAM entries and its coins,
/// winnings and help windows.
struct LuckyDiceSprites {
  u8 filler[196];
  struct OamWords oam[128];
  struct Window *coinsWindow;
  struct Window *winningsWindow;
  struct Window *helpWindow;
};

/// A Lucky Wheels reel: its turn, its 21 symbols, whether it is held and
/// where it stops.
struct Reel {
  s32 position;
  u8 symbols[21];
  u8 held;
  s8 stop;
};

/// Lucky Wheels' effect state: the title letters' particles, the BG and OBJ
/// shades, and its transfer to the screen.
struct ReelEffectState {
  struct Particle particles[LUCKY_WHEELS_TITLE_LETTERS];
  /// No code names it.
  u8 unusedAfterParticles[1792 -
                          LUCKY_WHEELS_TITLE_LETTERS * sizeof(struct Particle)];
  u16 bgShade[256];
  u16 spriteShade[256];
  u8 unusedAfterSpriteShade[27776]; ///< no code names it
  s32 transferMode;
  s32 transferValue;
  u8 unusedAfterTransferValue[4]; ///< no code names it
  s32 frame;
  u8 unusedAfterFrame[344]; ///< no code names it
  s32 transferPending;
};

/// The main routine's state from gPartyDjinnRecovery on, as Lucky Wheels reads
/// it: the prizes won, kept after the spring game's prize weights.
struct ReelSave {
  u8 unused[320]; ///< summons, coins, Djinn recovery, shops, prize weights
  s8 wonPrizes[16];
};

#endif
