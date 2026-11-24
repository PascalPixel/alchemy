/// @file
/// Super Lucky Dice, Contigo's four-dice game with its hi-lo game to double
/// the winnings.

#include "battle/battle.h"
#include "resource_ids.h"
#include "sounds.h"
#include "debug/debug.h"
#include "field/field.h"
#include "field/field_sequence.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "system/math_table.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "system/io_reg.h"
#include "types.h"
#include "system/ram_buffer.h"
#include "system/iwram_call.h"
#include "battle/battle_camera.h"
#include "macros.h"
#include "fixed_math.h"
#include "system/dma.h"
#include "system/input.h"
#include "system/main_routine_state.h"
#include "system/bg_scroll.h"
#include "version.h"
#include "field/actor.h"
#include "system/frame.h"
#include "gamepak/far_call/game.h"
#include "gamepak/far_call/window.h"
#include "system/heap_release.h"
#include "graphics/resource.h"
#include "system/task.h"
#include "system/matrix.h"

// Where SuperLuckyDice_Run puts each picture in OBJ VRAM, as 16-colour tiles
// (the dice, 256-colour, then their shadow at DICE_SHADOW_TILE, fill the first
// DICE_SHADOW_TILE + 8).
#define SMALL_DOUBLE_UP_TILES 152 ///< "Double Up", for the rounds won
#define FINAL_GAME_TILES 168      ///< "Final Game", in 16 by 16 cells
/// Its "Game" alone, the sixth cell on.
#define GAME_TILES (FINAL_GAME_TILES + 5 * SPRITE_TILES(16, 16))
#define ORANGE_DIGIT_TILES 200 ///< 0 to 9, 16 by 32, the hi-lo game's number
#define TIMES_SIGN_TILES 280   ///< the times sign, then "or"
#define OR_TILES (TIMES_SIGN_TILES + SPRITE_TILES(16, 16))
#define DOUBLE_UP_TILES 292     ///< the large "Double Up" choice
#define CASH_OUT_TILES 372      ///< "Cash Out"
#define HIGHER_TILES 512        ///< "Higher", in a hi-lo game
#define RESULT_BANNER_TILES 512 ///< "Win!" to "Perfect!!!", over Higher
#define LOWER_TILES 560         ///< "Lower"
#define GREEN_DIGIT_TILES 640   ///< 0 to 9, 32 by 32, the total
#define PINK_DIGIT_TILES 816    ///< 0 to 9, 16 by 16

/// The dome's picture begins with its 224 colours, the dice faces' with 64,
/// each before its tiles.
#define DOME_PALETTE_COLORS 224
#define DICE_FACES_PALETTE_COLORS 64

/// A glowing picture's channels rise by up to twice this many levels.
#define GLOW_LEVELS 16

/// The x the choices, Higher and Lower or Double Up and Cash Out, rest at;
/// once A is pressed the one not chosen slides away 12 pixels a frame.
#define CHOICE_COLUMN_X 64
/// The chosen choice's number bobs by a sine shifted down this far: up to 4
/// pixels and back every 32 frames.
#define CHOICE_BOB_SHIFT 14

/// Where the total and the result's banner come to rest after sliding in from
/// the left; after a loss each sways about its own x and falls from there.
#define TOTAL_X 100
#define TOTAL_Y 48
#define RESULT_BANNER_X 60
#define RESULT_BANNER_Y 64
/// Offered the hi-lo game, the total arcs up and over to the left on an
/// ellipse this wide and high, a sixteenth of a turn a frame.
#define TOTAL_ARC_RADIUS_X 40
#define TOTAL_ARC_RADIUS_Y 48
/// Unused: the circle a won total would ride.
#define TOTAL_CIRCLE_RADIUS 48

/// How far left of and above its point a die's sprite, and a shadow's, has its
/// corner at DICE_NEAREST_DEPTH, in pixels; the further the die, the less.
#define DIE_CORNER_X 48
#define DIE_CORNER_Y 32
#define SHADOW_CORNER_X 48
#define SHADOW_CORNER_Y 24

/// The sprite palettes SuperLuckyDice_Run gives its pictures, after the six the
/// dice faces' copy fills; the last four are loaded again for each part of a
/// game.
enum SuperLuckyDicePalette {
  DICE_PALETTE_FINAL_GAME = 6,
  DICE_PALETTE_SMALL_DOUBLE_UP = 7,
  DICE_PALETTE_ORANGE_DIGITS = 8,
  DICE_PALETTE_TIMES_SIGN = 9,
  DICE_PALETTE_PINK_DIGITS = 10,
  DICE_PALETTE_PALE_DIGITS = 11, ///< the pink digits, paler
  DICE_PALETTE_RELOADED_A = 12,  ///< Higher, or the total's green digits
  DICE_PALETTE_RELOADED_B = 13,  ///< Lower, or the result's banner
  /// Double Up, or the glowing number above when choosing Higher or Lower.
  DICE_PALETTE_RELOADED_C = 14,
  DICE_PALETTE_RELOADED_D = 15 ///< Cash Out, or the glowing number below
};

/// How a throw ended, the banner it shows: in a hi-lo game a loss or a win, or
/// for the first throw what the four dice show (SuperLuckyDice_ScoreDice).
enum SuperLuckyDiceResult {
  SUPER_LUCKY_DICE_RESULT_LOSE = 0, ///< "Lose"
  SUPER_LUCKY_DICE_RESULT_WIN = 1,  ///< "Win!", a hi-lo game won
  SUPER_LUCKY_DICE_RESULT_ONE_PAIR = 2,
  SUPER_LUCKY_DICE_RESULT_TWO_PAIR = 3,
  SUPER_LUCKY_DICE_RESULT_TRIPLE = 4,
  SUPER_LUCKY_DICE_RESULT_PERFECT = 5 ///< all four alike
};

/// Super Lucky Dice's rules, as Contigo's sign gives them: four dice thrown at
/// once; two alike pay the bet back, three alike twice it, two pairs three
/// times and all four alike eight times; then up to five hi-lo games, each won
/// doubling the winnings.
#define SUPER_LUCKY_DICE_COUNT 4
#define SUPER_LUCKY_DICE_ONE_PAIR_PAYOUT 1
#define SUPER_LUCKY_DICE_TRIPLE_PAYOUT 2
#define SUPER_LUCKY_DICE_TWO_PAIR_PAYOUT 3
#define SUPER_LUCKY_DICE_PERFECT_PAYOUT 8
#define DICE_FINAL_GAME 5 ///< the fifth hi-lo game, the "Final Game"

/// The totals four dice can throw, four ones to four sixes, and the one between
/// them that the hi-lo game's target is reset to.
#define DICE_LOWEST_TOTAL SUPER_LUCKY_DICE_COUNT
#define DICE_HIGHEST_TOTAL (SUPER_LUCKY_DICE_COUNT * DICE_FACE_COUNT)
#define DICE_MIDDLE_TOTAL ((DICE_LOWEST_TOTAL + DICE_HIGHEST_TOTAL) / 2)

/// The pictures a state asks SuperLuckyDice_Run to load before the next frame.
enum SuperLuckyDicePictures {
  DICE_PICTURES_NONE = -1,
  DICE_PICTURES_HIGHER_LOWER = 1, ///< Higher and Lower, and the glowing digits
  DICE_PICTURES_RESULT = 3,   ///< the result's banner and the total's digits
  DICE_PICTURES_DOUBLE_UP = 5 ///< Double Up and Cash Out
};

/// Runs Super Lucky Dice, every layer off until it sets up its own display,
/// and returns 0.
s32 SuperLuckyDice_Start(void) {
  REG_DISPCNT = DISPCNT_OBJ_1D_MAP;
  SuperLuckyDice_Run();
  return 0;
}

/// Puts the 3D camera at its view of the dome: at the origin, unturned,
/// pitched to 147/256 of a turn and 510 pixels away, following nothing.
void SuperLuckyDice_ResetViewDefaults(void) {
  struct BattleCamera *camera = Ram_HeapSlots->cameraState;
  s32 offset[3];
  u32 nearPlane;

  camera->pitch = TRIG_TURN * 147 / 256;
  camera->distance = toFIXED(510);
  SET_VECTOR(camera->focus, 0, 0, 0);
  camera->yaw = 0;
  camera->followPosition = 0;
  gProjection.centerX = 0;
  gProjection.centerY = 0;
  camera->followEye = 0;

  // The eye sits `distance` behind the focus along the camera's view.
  Matrix_Initialize();
  Matrix_TranslateVector(&camera->focus[0]);
  Matrix_RotateY((s16)camera->yaw);
  Matrix_RotateX((s16)camera->pitch);

  SET_VECTOR(offset, 0, 0, camera->distance);
  Iwram_TransformVector(offset, camera->eye);

  nearPlane = Iwram_DivFX(toFIXED(250), FIXED_ONE * 3 / 4);
  Projection_Set(toFIXED(250), nearPlane, toFIXED(32767));
}

/// Copies count RGB555 colours from source to destination with each channel
/// multiplied by scale (16.16: 0 is black, FIXED_ONE unchanged), for fades.
/// Returns 0.
s32 SuperLuckyDice_ScaleRgb555Buffer(u16 *source, u16 *destination, FIXED scale,
                                     s32 count) {
  s32 remaining;
  u32 redMask;
  u32 greenMask;
  u32 blueMask;
  u32 pixel;
  u32 red;
  u32 green;
  u32 blue;

  if (count > 0) {
    redMask = RGB(31, 0, 0);
    greenMask = RGB(0, 31, 0);
    blueMask = RGB(0, 0, 31);
    remaining = count;
    do {
      pixel = *source;
      red = pixel & redMask;
      green = pixel & greenMask;
      blue = blueMask & pixel;
      red *= scale;
      green *= scale;
      blue *= scale;
      pixel = (FIXEDtoINT(red) & redMask) | (FIXEDtoINT(green) & greenMask);
      pixel |= FIXEDtoINT(blue) & blueMask;
      *destination = pixel;
      source++;
      destination++;
      remaining--;
    } while (remaining != 0);
  }
  return 0;
}

/// Copies count RGB555 colours from source to destination with each channel
/// raised by GLOW_LEVELS * (1 - cos(angle)) and clamped at the brightest:
/// unchanged at angle 0 and brightest at a half turn, so a picture glows and
/// fades as angle turns.
void SuperLuckyDice_BrightenColors(u16 *source, u16 *destination, u32 count,
                                   s32 angle) {
  s32 add;
  u32 i;

  add = FIXEDtoINT(-Trig_Cos(angle) * GLOW_LEVELS) + GLOW_LEVELS;
  for (i = 0; i != count; i++) {
    s32 r = RGB_RED(*source) + add;
    s32 g = RGB_GREEN(*source) + add;
    s32 b = RGB_BLUE(*source) + add;

    if (r > RGB_LEVEL_MAX)
      r = RGB_LEVEL_MAX;
    if (g > RGB_LEVEL_MAX)
      g = RGB_LEVEL_MAX;
    if (b > RGB_LEVEL_MAX)
      b = RGB_LEVEL_MAX;
    *destination = RGB(r, g, b);
    source++;
    destination++;
  }
}

/// Draws value as digits small digit sprites, from PINK_DIGIT_TILES in palette,
/// ending at x, 10 pixels apart, leading zeros skipped but for the last.
/// Returns the next free sprite.
s32 SuperLuckyDice_DrawNumber(s32 sprite, s32 x, s32 y, s32 value, s32 palette,
                              s32 digits) {
  struct DiceSprites *sprites;
  s32 started;
  s32 digit;
  s32 i;
  s32 k;

  sprites = (struct DiceSprites *)Ram_HeapSlots->canvasMap;
  started = 0;
  x -= digits * 10;
  for (i = 0; i != digits; i++) {
    digit = value;
    for (k = 0; k != digits - i - 1; k++)
      digit /= 10;
    digit %= 10;
    if (digit != 0)
      started = 1;
    else if (!started && i != digits - 1)
      continue;
    sprites->oam[sprite].attr01 =
        OBJ_POSITION(x + i * 10 + 522, y + 256) | OBJ_SIZE_16X16;
    sprites->oam[sprite].attr2 =
        (digit * SPRITE_TILES(16, 16) + PINK_DIGIT_TILES) |
        OBJ_PALETTE(palette);
    sprite++;
  }
  return sprite;
}

/// Draws value as digits large digit sprites, from GREEN_DIGIT_TILES in
/// palette, starting at x, 22 pixels apart, leading zeros skipped. Returns the
/// next free sprite.
s32 SuperLuckyDice_DrawLargeNumber(s32 sprite, s32 x, s32 y, s32 value,
                                   s32 palette, s32 digits) {
  struct DiceSprites *sprites;
  s32 started;
  s32 digit;
  s32 i;
  s32 k;

  sprites = (struct DiceSprites *)Ram_HeapSlots->canvasMap;
  started = 0;
  for (i = 0; i != digits; i++) {
    digit = value;
    for (k = 0; k != digits - i - 1; k++)
      digit /= 10;
    digit %= 10;
    if (digit != 0)
      started = 1;
    else if (!started)
      continue;
    sprites->oam[sprite].attr01 =
        OBJ_POSITION(x + i * 22 + 512, y + 256) | OBJ_SIZE_32X32;
    sprites->oam[sprite].attr2 =
        (digit * SPRITE_TILES(32, 32) + GREEN_DIGIT_TILES) |
        OBJ_PALETTE(palette);
    sprite++;
  }
  return sprite;
}

/// Scores the four dice's faces: SUPER_LUCKY_DICE_RESULT_PERFECT when all four
/// match, otherwise by the matching pairs among them, one pair, two pair or
/// three (three alike), or SUPER_LUCKY_DICE_RESULT_LOSE for none.
s32 SuperLuckyDice_ScoreDice(s32 *dice) {
// UB: three alike make three pairs, and the list of matched faces, which
// nothing reads, has room for two.
#ifdef UBFIX
  UNUSED s32 matches[3]; // written, never read
#else
  UNUSED s32 matches[2]; // written, never read
#endif
  s32 score;
  s32 pairs;
  s32 i;
  s32 j;

  score = SUPER_LUCKY_DICE_RESULT_LOSE;
  if (dice[0] == dice[1] && dice[0] == dice[2] && dice[0] == dice[3]) {
    score = SUPER_LUCKY_DICE_RESULT_PERFECT;
  } else {
    pairs = 0;
    for (i = 0; i != SUPER_LUCKY_DICE_COUNT - 1; i++) {
      for (j = i + 1; j != SUPER_LUCKY_DICE_COUNT; j++) {
        if (dice[i] == dice[j]) {
          matches[pairs] = dice[i];
          pairs++;
        }
      }
    }
    if (pairs == 1)
      score = SUPER_LUCKY_DICE_RESULT_ONE_PAIR;
    if (pairs == 2)
      score = SUPER_LUCKY_DICE_RESULT_TWO_PAIR;
    if (pairs == 3)
      score = SUPER_LUCKY_DICE_RESULT_TRIPLE;
  }
  return score;
}

/// The die face tile at each step of its roll, as Lucky Dice's
/// gLuckyDice_FaceTiles: one for each DICE_ROLL_PER_TILE of the roll angle,
/// three to a face.
const u8 gSuperLuckyDice_FaceTiles[] = {0, 12, 16, 1, 14, 8, 2, 15, 10,
                                        3, 9,  13, 4, 7,  6, 5, 17, 11};

/// What differs between the editions: the windows' sizes and text columns by
/// language; the hi-lo games B cannot leave (CANNOT_QUIT), only the first until
/// patch 1 and every one from it; and the first bet shown, clamped to the
/// player's coins until patch 2.
#if LANGUAGE == LANGUAGE_JAPANESE
#define COIN_WINDOW_X 19
#define COIN_WINDOW_WIDTH 11
#define COIN_TEXT_X 48
#define SUPER_LUCKY_DICE_HELP_WINDOW_X 22
#define SUPER_LUCKY_DICE_HELP_WINDOW_WIDTH 8
#define BET_TEXT_X 24
#define WINNINGS_WINDOW_X 10
#define WINNINGS_WINDOW_MARGIN 10
#define WINNINGS_LABEL_X 40
#define WINNINGS_TEXT_X 40
#define RESULT_WINDOW_X 4
#define RESULT_WINDOW_WIDTH 20
#define GAME_BANNER_X 608
#define MULTIPLIER_ZERO_X 80
#elif LANGUAGE == LANGUAGE_ENGLISH
#define COIN_WINDOW_X 18
#define COIN_WINDOW_WIDTH 12
#define COIN_TEXT_X 48
#define SUPER_LUCKY_DICE_HELP_WINDOW_X 22
#define SUPER_LUCKY_DICE_HELP_WINDOW_WIDTH 8
#define BET_TEXT_X 24
#define WINNINGS_WINDOW_X 9
#define WINNINGS_WINDOW_MARGIN 12
#define WINNINGS_LABEL_X 48
#define WINNINGS_TEXT_X 48
#define RESULT_WINDOW_X 4
#define RESULT_WINDOW_WIDTH 21
#define GAME_BANNER_X 608
#define MULTIPLIER_ZERO_X 80
#elif LANGUAGE == LANGUAGE_GERMAN
#define COIN_WINDOW_X 17
#define COIN_WINDOW_WIDTH 13
#define COIN_TEXT_X 48
#define SUPER_LUCKY_DICE_HELP_WINDOW_X 20
#define SUPER_LUCKY_DICE_HELP_WINDOW_WIDTH 10
#define BET_TEXT_X 32
#define WINNINGS_WINDOW_X 7
#define WINNINGS_WINDOW_MARGIN 14
#define WINNINGS_LABEL_X 48
#define WINNINGS_TEXT_X 48
#define RESULT_WINDOW_X 4
#define RESULT_WINDOW_WIDTH 21
#define GAME_BANNER_X 576
#define MULTIPLIER_ZERO_X 92
#elif LANGUAGE == LANGUAGE_SPANISH
#define COIN_WINDOW_X 18
#define COIN_WINDOW_WIDTH 12
#define COIN_TEXT_X 56
#define SUPER_LUCKY_DICE_HELP_WINDOW_X 20
#define SUPER_LUCKY_DICE_HELP_WINDOW_WIDTH 10
#define BET_TEXT_X 32
#define WINNINGS_WINDOW_X 9
#define WINNINGS_WINDOW_MARGIN 12
#define WINNINGS_LABEL_X 56
#define WINNINGS_TEXT_X 48
#define RESULT_WINDOW_X 4
#define RESULT_WINDOW_WIDTH 21
#define GAME_BANNER_X 608
#define MULTIPLIER_ZERO_X 80
#elif LANGUAGE == LANGUAGE_FRENCH
#define COIN_WINDOW_X 16
#define COIN_WINDOW_WIDTH 14
#define COIN_TEXT_X 56
#define SUPER_LUCKY_DICE_HELP_WINDOW_X 20
#define SUPER_LUCKY_DICE_HELP_WINDOW_WIDTH 10
#define BET_TEXT_X 24
#define WINNINGS_WINDOW_X 7
#define WINNINGS_WINDOW_MARGIN 14
#define WINNINGS_LABEL_X 56
#define WINNINGS_TEXT_X 48
#define RESULT_WINDOW_X 4
#define RESULT_WINDOW_WIDTH 21
#define GAME_BANNER_X 576
#define MULTIPLIER_ZERO_X 86
#elif LANGUAGE == LANGUAGE_ITALIAN
#define COIN_WINDOW_X 18
#define COIN_WINDOW_WIDTH 12
#define COIN_TEXT_X 56
#define SUPER_LUCKY_DICE_HELP_WINDOW_X 20
#define SUPER_LUCKY_DICE_HELP_WINDOW_WIDTH 10
#define BET_TEXT_X 24
#define WINNINGS_WINDOW_X 9
#define WINNINGS_WINDOW_MARGIN 12
#define WINNINGS_LABEL_X 56
#define WINNINGS_TEXT_X 48
#define RESULT_WINDOW_X 3
#define RESULT_WINDOW_WIDTH 23
#define GAME_BANNER_X 600
#define MULTIPLIER_ZERO_X 80
#endif

/// Whether B cannot leave hi-lo game `game`.
#if TLA_PATCH_1
#define CANNOT_QUIT(game) ((game) > 0)
#else
#define CANNOT_QUIT(game) ((game) == 1)
#endif

/// The OAM word of the game's number after the final game's banner: its x,
/// fitted to each language's banner, and its 16x32 shape.
#if LANGUAGE == LANGUAGE_GERMAN
#define GAME_NUMBER_WORD (OBJ_X(116) | OBJ_SIZE_16X32)
#elif LANGUAGE == LANGUAGE_SPANISH
#define GAME_NUMBER_WORD (OBJ_X(72) | OBJ_SIZE_16X32)
#elif LANGUAGE == LANGUAGE_FRENCH
#define GAME_NUMBER_WORD (OBJ_X(112) | OBJ_SIZE_16X32)
#elif LANGUAGE == LANGUAGE_ITALIAN
#define GAME_NUMBER_WORD (OBJ_X(72) | OBJ_SIZE_16X32)
#else
#define GAME_NUMBER_WORD (OBJ_X(80) | OBJ_SIZE_16X32)
#endif

/// The bet window: where the coin window is, but in French a tile further right
/// and narrower.
#if LANGUAGE == LANGUAGE_FRENCH
#define BET_WINDOW_X 17
#define BET_WINDOW_WIDTH 13
#else
#define BET_WINDOW_X COIN_WINDOW_X
#define BET_WINDOW_WIDTH COIN_WINDOW_WIDTH
#endif

extern u8 gMapGridCellBuffer[];
extern char MsgLuckyDiceCoins, MsgLuckyDiceWinnings, MsgDiceGameThrow,
    MsgDiceGameHigher, MsgDiceGameLower, MsgDiceGameHigherAgain,
    MsgDiceGameLowerAgain, MsgDiceGameChoose, MsgDiceGameDoubleUp;

/// What the Super Lucky Dice screen is doing.
enum SuperLuckyDiceState {
  SUPER_LUCKY_DICE_THROW,    ///< the dice wait to be thrown; A throws, B leaves
  SUPER_LUCKY_DICE_GUESS,    ///< a hi-lo game: higher or lower than this total
  SUPER_LUCKY_DICE_ROLL,     ///< the dice fly, bounce and settle
  SUPER_LUCKY_DICE_RESULT,   ///< the four dice are scored
  SUPER_LUCKY_DICE_PAYOUT,   ///< the winnings are counted into the coins
  SUPER_LUCKY_DICE_DOUBLE_UP ///< a win: double up or cash out
};

/// The dice wait on a circle this far about the dome's centre, a quarter turn
/// apart, before the first throw.
#define DICE_START_RADIUS 100
/// Two dice whose centres come closer than this push each other apart.
#define SUPER_LUCKY_DICE_TOUCH_DISTANCE 70
/// The round dome's radius: a die beyond it is turned back inward.
#define DICE_DOME_RADIUS 200

/// Throws the four dice from a row left of the centre, 24 pixels apart and 96
/// up, each with a random push up and into the dome and a random roll; it
/// counts with SuperLuckyDice_Run's index.
#define DICE_GAME_THROW(dice, spinRate)                                        \
  {                                                                            \
    struct DiceParticle *die;                                                  \
    for (index = 0; index != SUPER_LUCKY_DICE_COUNT; index++) {                \
      die = &(dice)->particles[index];                                         \
      die->z = 0;                                                              \
      die->y = toFIXED(96);                                                    \
      die->x = toFIXED(index * 24 - 108);                                      \
      die->velocityX = ((Random16() & 127) - 64) << 12;                        \
      die->velocityY = ((Random16() & 15) + 48) << 13;                         \
      die->velocityZ = ((Random16() & 127) + 64) << 13;                        \
      die->variant = Random16() % DICE_ROLL_TURN;                              \
      (spinRate)[index] = DICE_THROW_SPIN;                                     \
    }                                                                          \
  }

/// Runs Super Lucky Dice until the player quits or cannot afford a throw. Each
/// throw costs ten coins per level of the party's average.
void SuperLuckyDice_Run(void) {
  u16 bgPalette[BG_PLTT_COLORS];
  s32 spinRate[SUPER_LUCKY_DICE_COUNT];
  s32 spriteDepth[SUPER_LUCKY_DICE_COUNT * 2]; // the dice, then their shadows
  s32 numbers[SUPER_LUCKY_DICE_COUNT];         // each die's face, 0 for a one
  s32 groundPoint[3];
  s32 screen[3];
  s32 drawOrder[SUPER_LUCKY_DICE_COUNT];
  u16 *spritePalette;
  u8 *cellBuffer;
  struct DiceState *dice;
  struct DiceSprites *sprites;
  struct DiceCamera *camera;
  u8 *resourceAddress;
  s32 frame;
  s32 total;
  s32 resultKind;
  s32 state;
  s32 bet;
  s32 hiLoGame; // 0 for the first throw, then 1 to DICE_FINAL_GAME
  s32 timer;
  s32 slide;
  s32 winnings;
  s32 lower;
  s32 cashOut;
  s32 target;
  s32 multiplier;
  s32 loadPictures;
  s32 jingle;
  s32 spriteCount;
  s32 stoppedDice;
  struct Window *window;
  s32 index;
  s32 otherIndex;
  s32 digits;

  spritePalette = HeapSlot_Alloc(HEAP_SLOT_BATTLE_CANVAS, 512);
  dice = HeapSlot_AllocEwram(HEAP_SLOT_BATTLE_EFFECT, 31612);
  sprites = HeapSlot_AllocEwram(HEAP_SLOT_CANVAS_MAP, 1556);
  camera = HeapSlot_AllocEwram(HEAP_SLOT_CAMERA, 76);
  Resource_ReservedNoOp(RESOURCE_BATTLE_EFFECT_FAR_CALLS);
  SuperLuckyDice_ResetViewDefaults();
  Task_ResetAll();
  gOamCopyEnabled = 0;
  cellBuffer = gMapGridCellBuffer;

  // Fill BG1's map (screen block 6) with a 30-tile-wide picture, then load
  // the dome, the dice and the lettering with their palettes faded to black.
  {
    s32 tileX;
    s32 tileY;
    index = 0;
    for (tileY = 0; tileY != DISPLAY_TILE_HEIGHT; tileY++) {
      for (tileX = 0; tileX != BG_SCREEN_WIDTH; tileX++) {
        *(volatile u16 *)(BG_SCREEN_BLOCK(6) + index) =
            tileX + tileY * DISPLAY_TILE_WIDTH;
        index += 2;
      }
    }
  }

  resourceAddress = Resource_GetTableEntry(RESOURCE_SLOTS_DOME_PICTURE);
  Dma_Set((void *)BG_PLTT, bgPalette, DMA_COPY_WORDS(128), REG_DMA3);
  Dma_Set(resourceAddress, bgPalette, DMA_COPY_WORDS(DOME_PALETTE_COLORS / 2),
          REG_DMA3);
  resourceAddress += DOME_PALETTE_COLORS * 2;
  Decompress_TaggedLz(resourceAddress, cellBuffer);
  Dma_Set(cellBuffer, (void *)BG_CHAR_BLOCK(1), DMA_COPY_WORDS(9600), REG_DMA3);
  resourceAddress = Resource_GetTableEntry(RESOURCE_DICE_FACES);
  // Six palettes: the faces' DICE_FACES_PALETTE_COLORS colours, then 32 read
  // from the start of their tiles into palettes 4 and 5.
  Dma_Set(resourceAddress, spritePalette,
          DMA_COPY_WORDS(6 * PALETTE_COLORS / 2), REG_DMA3);
  resourceAddress += DICE_FACES_PALETTE_COLORS * 2;
  Decompress_TaggedLz(resourceAddress, cellBuffer);
  Dma_Set(cellBuffer, (void *)OBJ_VRAM0, DMA_COPY_WORDS(1216), REG_DMA3);
  resourceAddress =
      Resource_GetTableEntry(RESOURCE_SLOTS_SMALL_DOUBLE_UP_BANNER);
  Dma_Set(resourceAddress,
          spritePalette + DICE_PALETTE_SMALL_DOUBLE_UP * PALETTE_COLORS,
          DMA_COPY_WORDS(8), REG_DMA3);
  resourceAddress += PALETTE_BYTES;
  Decompress_TaggedLz(resourceAddress, cellBuffer);
  Dma_Set(cellBuffer,
          (void *)(OBJ_VRAM0 + SMALL_DOUBLE_UP_TILES * TILE_SIZE_4BPP),
          DMA_COPY_WORDS(128), REG_DMA3);
  resourceAddress = Resource_GetTableEntry(RESOURCE_SLOTS_FINAL_GAME_BANNER);
  Dma_Set(resourceAddress,
          spritePalette + DICE_PALETTE_FINAL_GAME * PALETTE_COLORS,
          DMA_COPY_WORDS(8), REG_DMA3);
  resourceAddress += PALETTE_BYTES;
  Decompress_TaggedLz(resourceAddress, cellBuffer);
  Dma_Set(cellBuffer, (void *)(OBJ_VRAM0 + FINAL_GAME_TILES * TILE_SIZE_4BPP),
          DMA_COPY_WORDS(256), REG_DMA3);
  resourceAddress = Resource_GetTableEntry(RESOURCE_SLOTS_ORANGE_DIGITS);
  Dma_Set(resourceAddress,
          spritePalette + DICE_PALETTE_ORANGE_DIGITS * PALETTE_COLORS,
          DMA_COPY_WORDS(8), REG_DMA3);
  resourceAddress += PALETTE_BYTES;
  Decompress_TaggedLz(resourceAddress, cellBuffer);
  Dma_Set(cellBuffer, (void *)(OBJ_VRAM0 + ORANGE_DIGIT_TILES * TILE_SIZE_4BPP),
          DMA_COPY_WORDS(640), REG_DMA3);
  resourceAddress = Resource_GetTableEntry(RESOURCE_SLOTS_TIMES_SIGN);
  Dma_Set(resourceAddress,
          spritePalette + DICE_PALETTE_TIMES_SIGN * PALETTE_COLORS,
          DMA_COPY_WORDS(8), REG_DMA3);
  resourceAddress += PALETTE_BYTES;
  Decompress_TaggedLz(resourceAddress, cellBuffer);
  Dma_Set(cellBuffer, (void *)(OBJ_VRAM0 + TIMES_SIGN_TILES * TILE_SIZE_4BPP),
          DMA_COPY_WORDS(96), REG_DMA3);
  resourceAddress = Resource_GetTableEntry(RESOURCE_SLOTS_PINK_DIGITS);
  Dma_Set(resourceAddress,
          spritePalette + DICE_PALETTE_PINK_DIGITS * PALETTE_COLORS,
          DMA_COPY_WORDS(8), REG_DMA3);
  {
    u16 *src;
    u16 *dst;
    s32 red;
    s32 blue;

    // The pink digits' blue and red raised by four: red takes the blue, and
    // green and blue the red.
    dst = spritePalette + DICE_PALETTE_PALE_DIGITS * PALETTE_COLORS;
    src = (u16 *)resourceAddress;
    for (index = 0; index != PALETTE_COLORS; index++) {
      blue = RGB_BLUE(*src) + 4;
      red = RGB_RED(*src) + 4;
      if (red > RGB_LEVEL_MAX)
        red = RGB_LEVEL_MAX;
      if (blue > RGB_LEVEL_MAX)
        blue = RGB_LEVEL_MAX;
      *dst = RGB(blue, red, red);
      src++;
      dst++;
    }
  }
  Dma_Set(spritePalette + DICE_PALETTE_PALE_DIGITS * PALETTE_COLORS,
          (void *)&OBJ_PLTT_COLOR(DICE_PALETTE_PALE_DIGITS, 0),
          DMA_COPY_WORDS(8), REG_DMA3);
  resourceAddress += PALETTE_BYTES;
  Decompress_TaggedLz(resourceAddress, cellBuffer);
  Dma_Set(cellBuffer, (void *)(OBJ_VRAM0 + PINK_DIGIT_TILES * TILE_SIZE_4BPP),
          DMA_COPY_WORDS(320), REG_DMA3);
  SuperLuckyDice_ScaleRgb555Buffer(spritePalette, (u16 *)OBJ_PLTT, 0, 256);
  SuperLuckyDice_ScaleRgb555Buffer(bgPalette, (u16 *)BG_PLTT, 0, 256);

  // Show the dome on BG1, sprites and windows, with the dome blended.
  REG_BG1CNT = BGCNT_PRIORITY(2) | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS |
               BGCNT_SCREEN_BASE(6);
  REG_WININ = WININ_WIN0_BG0 | WININ_WIN0_BG1 | WININ_WIN0_BG2 |
              WININ_WIN0_OBJ | WININ_WIN0_CLR | WININ_WIN1_BG0 |
              WININ_WIN1_BG1 | WININ_WIN1_BG2 | WININ_WIN1_OBJ | WININ_WIN1_CLR;
  REG_WINOUT = WINOUT_WIN01_BG0 | WINOUT_WIN01_BG1 | WINOUT_WIN01_CLR |
               WINOUT_WINOBJ_BG0 | WINOUT_WINOBJ_BG1 | WINOUT_WINOBJ_BG2 |
               WINOUT_WINOBJ_CLR;
  REG_DISPCNT = DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON | DISPCNT_BG1_ON |
                DISPCNT_OBJ_ON | DISPCNT_WIN0_ON;
  REG_BLDCNT = BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
  REG_BLDALPHA = BLEND_ALPHA(16, 8);
  gBgScroll[1].x = 0;
  gBgScroll[1].y = 0;
  REG_BG1HOFS = 0;
  REG_BG1VOFS = -DISPLAY_HEIGHT;
  REG_BG2HOFS = 0;
  REG_BG2VOFS = -DISPLAY_HEIGHT;
  REG_WIN0H = WIN_RANGE(0, DISPLAY_WIDTH);
  REG_WIN0V = WIN_RANGE(0, DISPLAY_HEIGHT);
  REG_WIN1H = WIN_RANGE(0, DISPLAY_WIDTH);
  REG_WIN1V = WIN_RANGE(0, DISPLAY_HEIGHT);

  total = 0;
  resultKind = SUPER_LUCKY_DICE_RESULT_LOSE;
  state = SUPER_LUCKY_DICE_THROW;
  hiLoGame = 0;
  timer = 0;
  slide = 0;
  winnings = 0;
  lower = 0;
  cashOut = 0;
  target = DICE_MIDDLE_TOTAL;
  multiplier = 1;
  loadPictures = DICE_PICTURES_NONE;
  jingle = -1;
  // The dice rest on a circle about the dome's centre, each on a random face,
  // until the first throw.
  {
    struct DiceParticle *die;

    die = dice->particles;
    for (index = 0; index != SUPER_LUCKY_DICE_COUNT; index++) {
      // DEAD: a random place, replaced at once by one on the circle (its two
      // rolls still advance the random numbers); the ROM keeps its stores.
      SET_POSITION(die, ((Random16() & 127) - 64) << 16, 0,
                   ((Random16() & 127) - 64) << 16);
      SET_POSITION(die, Trig_Sin(index * DEGtoANG(90)) * DICE_START_RADIUS, 0,
                   Trig_Cos(index * DEGtoANG(90)) * DICE_START_RADIUS);
      die->velocityX = ((Random16() & 127) - 64) << 14;
      die->velocityY = ((Random16() & 31) + 32) << 12;
      die->velocityZ = ((Random16() & 127) - 64) << 14;
      die->variant = (Random16() % DICE_FACE_COUNT) * DICE_ROLL_PER_FACE;
      spinRate[index] = DICE_THROW_SPIN;
      numbers[index] = 0;
      die++;
    }
  }

  WindowSystem_InitFieldFar();
  window = Window_CreateFar(COIN_WINDOW_X, 0, COIN_WINDOW_WIDTH, 3, 6);
  sprites->coinsWindow = window;
  Text_DrawMessageAsTilesFar((s32)&MsgLuckyDiceCoins, window, COIN_TEXT_X, 0);
  Text_DrawNumberInWindowFar(gMainRoutineState.coins, DICE_COIN_DIGITS,
                             (s32)sprites->coinsWindow, 0, 0);
  window = Window_CreateFar(BET_WINDOW_X, 3, BET_WINDOW_WIDTH, 3, 6);
  sprites->betWindow = window;
  // "Bet", the message before.
  Text_DrawMessageAsTilesFar((s32)&MsgLuckyDiceCoins - 1, window, 0, 0);
  window = Window_CreateFar(SUPER_LUCKY_DICE_HELP_WINDOW_X, 16,
                            SUPER_LUCKY_DICE_HELP_WINDOW_WIDTH, 4, 6);
  sprites->helpWindow = window;
  // The Throw and Quit buttons.
  Text_DrawMessageAsTilesFar((s32)&MsgDiceGameThrow, window, 0, 0);
  Text_DrawMessageAsTilesFar((s32)&MsgDiceGameThrow + 1, sprites->helpWindow, 0,
                             8);
  bet = Party_GetAverageLevelFar() * DICE_BET_PER_LEVEL;
#if !TLA_PATCH_2
  if ((u32)bet > (u32)gMainRoutineState.coins)
    bet = gMainRoutineState.coins;
#endif
  Text_DrawNumberInWindowFar(bet, DICE_COIN_DIGITS, (s32)sprites->betWindow,
                             BET_TEXT_X, 5);
  Sound_PlayFar(MUS_SUPER_LUCKY_DICE);

  frame = 0;
  for (;;) {
    spriteCount = 0;
    if (frame <= DICE_FADE_FRAMES) {
      SuperLuckyDice_ScaleRgb555Buffer(spritePalette, (u16 *)OBJ_PLTT,
                                       frame * (FIXED_ONE / DICE_FADE_FRAMES),
                                       256);
      SuperLuckyDice_ScaleRgb555Buffer(bgPalette, (u16 *)BG_PLTT,
                                       frame * (FIXED_ONE / DICE_FADE_FRAMES),
                                       256);
    }
    Matrix_Initialize();
    Matrix_SetLookAt(camera->eye, camera->focus);

    if (state == SUPER_LUCKY_DICE_THROW) {
      if (frame > 17) {
        if ((u32)gMainRoutineState.coins <
            (u32)(Party_GetAverageLevelFar() * DICE_BET_PER_LEVEL)) {
          Window_CloseFar(sprites->helpWindow, 1);
          break;
        }
        if (gInput.repeat & KEY_B) {
          Sound_PlayFar(SE_MENU_CANCEL);
          Window_CloseFar(sprites->helpWindow, 1);
          break;
        }
        if (gInput.repeat & KEY_A) {
          Sound_PlayFar(SE_LUCKY_DICE_THROW);
          Window_CloseFar(sprites->helpWindow, 1);
          Sound_PlayFar(SE_LUCKY_DICE_THROW);
          DICE_GAME_THROW(dice, spinRate);
          bet = Party_GetAverageLevelFar() * DICE_BET_PER_LEVEL;
          if ((u32)bet > (u32)gMainRoutineState.coins)
            bet = gMainRoutineState.coins;
          Text_DrawNumberInWindowFar(bet, DICE_COIN_DIGITS,
                                     (s32)sprites->betWindow, BET_TEXT_X, 5);
          Party_AdjustCoinsFar(-bet);
          Text_DrawNumberInWindowFar(gMainRoutineState.coins, DICE_COIN_DIGITS,
                                     (s32)sprites->coinsWindow, 0, 0);
          state = SUPER_LUCKY_DICE_ROLL;
          timer = 0;
          slide = 0;
          bet = Party_GetAverageLevelFar() * DICE_BET_PER_LEVEL;
          Text_DrawNumberInWindowFar(bet, DICE_COIN_DIGITS,
                                     (s32)sprites->betWindow, BET_TEXT_X, 5);
        }
      }
    } else if (state == SUPER_LUCKY_DICE_GUESS) {
      // Higher and Lower slide in; A throws once the one not chosen has slid
      // away, and B, where CANNOT_QUIT allows it, goes back to throwing for a
      // new bet, the winnings lost.
      if (slide <= 7) {
        slide++;
      } else if (timer > 0) {
        if (++timer == 16) {
          Sound_PlayFar(SE_LUCKY_DICE_THROW);
          DICE_GAME_THROW(dice, spinRate);
          state = SUPER_LUCKY_DICE_ROLL;
        }
      } else if (gInput.repeat & KEY_B) {
        if (CANNOT_QUIT(hiLoGame)) {
          Sound_PlayFar(SE_MENU_ERROR);
        } else {
          Window_CloseFar(sprites->helpWindow, 1);
          window = Window_CreateFar(SUPER_LUCKY_DICE_HELP_WINDOW_X, 16,
                                    SUPER_LUCKY_DICE_HELP_WINDOW_WIDTH, 4, 6);
          sprites->helpWindow = window;
          Text_DrawMessageAsTilesFar((s32)&MsgDiceGameThrow, window, 0, 0);
          Text_DrawMessageAsTilesFar((s32)&MsgDiceGameThrow + 1,
                                     sprites->helpWindow, 0, 8);
          Sound_PlayFar(SE_MENU_CANCEL);
          state = SUPER_LUCKY_DICE_THROW;
          // BUG: the hi-lo game and its total are kept, so the next throw,
          // its bet paid anew, is judged as this hi-lo game rather than
          // scored. Only editions before patch 1 reach here.
#ifdef BUGFIX
          hiLoGame = 0;
          target = DICE_MIDDLE_TOTAL;
#endif
        }
      } else if (gInput.repeat & KEY_A) {
        Sound_PlayFar(SE_MENU_CONFIRM);
        timer = 1;
        Window_CloseFar(sprites->helpWindow, 1);
        // Unused: a guess before the first throw would pay its bet here, and
        // be told "If the dice total more than 15," (MsgDiceGameHigher); the
        // first hi-lo game comes only after a win.
        if (hiLoGame == 0) {
          bet = Party_GetAverageLevelFar() * DICE_BET_PER_LEVEL;
          if ((u32)bet > (u32)gMainRoutineState.coins)
            bet = gMainRoutineState.coins;
          Text_DrawNumberInWindowFar(bet, DICE_COIN_DIGITS,
                                     (s32)sprites->betWindow, BET_TEXT_X, 5);
          Party_AdjustCoinsFar(-bet);
          Text_DrawNumberInWindowFar(gMainRoutineState.coins, DICE_COIN_DIGITS,
                                     (s32)sprites->coinsWindow, 0, 0);
        }
      } else {
        s32 message = -1;

        if ((gInput.repeat & KEY_UP) && lower == 1) {
          if (target == DICE_HIGHEST_TOTAL) {
            Sound_PlayFar(SE_MENU_CANCEL);
          } else {
            if (hiLoGame == 0)
              message = (s32)&MsgDiceGameHigher;
            else
              message = (s32)&MsgDiceGameHigherAgain;
            Sound_PlayFar(SE_MENU_CURSOR_MOVE);
            lower = 0;
          }
        } else if ((gInput.repeat & KEY_DOWN) && lower == 0) {
          if (target == DICE_LOWEST_TOTAL) {
            Sound_PlayFar(SE_MENU_CANCEL);
          } else {
            if (hiLoGame == 0)
              message = (s32)&MsgDiceGameLower;
            else
              message = (s32)&MsgDiceGameLowerAgain;
            Sound_PlayFar(SE_MENU_CURSOR_MOVE);
            lower = 1;
          }
        }
        if (message != -1) {
#if LANGUAGE == LANGUAGE_GERMAN || LANGUAGE == LANGUAGE_SPANISH ||             \
    LANGUAGE == LANGUAGE_ITALIAN
          WindowSprite_PrepareForRedrawFar(sprites->helpWindow);
#endif
          Text_DrawMessageAsTilesFar(message, sprites->helpWindow, 0, 0);
          Text_DrawMessageAsTilesFar((s32)&MsgDiceGameChoose,
                                     sprites->helpWindow, 0, 8);
        }
      }
      if (lower == 0) {
        SuperLuckyDice_BrightenColors(
            spritePalette + DICE_PALETTE_RELOADED_A * PALETTE_COLORS,
            (u16 *)&OBJ_PLTT_COLOR(DICE_PALETTE_RELOADED_A, 0), PALETTE_COLORS,
            timer * (TRIG_TURN / 16));
        SuperLuckyDice_ScaleRgb555Buffer(
            spritePalette + DICE_PALETTE_RELOADED_B * PALETTE_COLORS,
            (u16 *)&OBJ_PLTT_COLOR(DICE_PALETTE_RELOADED_B, 0), FIXED_ONE / 2,
            PALETTE_COLORS);
        SuperLuckyDice_BrightenColors(
            spritePalette + DICE_PALETTE_RELOADED_C * PALETTE_COLORS,
            (u16 *)&OBJ_PLTT_COLOR(DICE_PALETTE_RELOADED_C, 0), PALETTE_COLORS,
            timer * (TRIG_TURN / 16));
        SuperLuckyDice_ScaleRgb555Buffer(
            spritePalette + DICE_PALETTE_RELOADED_D * PALETTE_COLORS,
            (u16 *)&OBJ_PLTT_COLOR(DICE_PALETTE_RELOADED_D, 0), FIXED_ONE / 2,
            PALETTE_COLORS);
      } else {
        SuperLuckyDice_BrightenColors(
            spritePalette + DICE_PALETTE_RELOADED_B * PALETTE_COLORS,
            (u16 *)&OBJ_PLTT_COLOR(DICE_PALETTE_RELOADED_B, 0), PALETTE_COLORS,
            timer * (TRIG_TURN / 16));
        SuperLuckyDice_ScaleRgb555Buffer(
            spritePalette + DICE_PALETTE_RELOADED_A * PALETTE_COLORS,
            (u16 *)&OBJ_PLTT_COLOR(DICE_PALETTE_RELOADED_A, 0), FIXED_ONE / 2,
            PALETTE_COLORS);
        SuperLuckyDice_BrightenColors(
            spritePalette + DICE_PALETTE_RELOADED_D * PALETTE_COLORS,
            (u16 *)&OBJ_PLTT_COLOR(DICE_PALETTE_RELOADED_D, 0), PALETTE_COLORS,
            timer * (TRIG_TURN / 16));
        SuperLuckyDice_ScaleRgb555Buffer(
            spritePalette + DICE_PALETTE_RELOADED_C * PALETTE_COLORS,
            (u16 *)&OBJ_PLTT_COLOR(DICE_PALETTE_RELOADED_C, 0), FIXED_ONE / 2,
            PALETTE_COLORS);
      }
    } else if (state == SUPER_LUCKY_DICE_ROLL) {
      struct DiceParticle *die;
      struct DiceParticle *other;

      stoppedDice = 0;
      for (index = 0; index != SUPER_LUCKY_DICE_COUNT; index++) {
        s32 wallX, wallZ;

        die = &dice->particles[index];

        die->x += die->velocityX;
        die->y += die->velocityY;
        die->z += die->velocityZ;
        for (otherIndex = 0; otherIndex != SUPER_LUCKY_DICE_COUNT;
             otherIndex++) {
          other = &dice->particles[otherIndex];
          if (index != otherIndex) {
            s32 separationX, separationY, separationZ, separation;

            separationX = FIXEDtoINT(die->x - other->x);
            separationY = FIXEDtoINT(die->y - other->y);
            separationZ = FIXEDtoINT(die->z - other->z);
            if (separationX * separationX + separationY * separationY +
                    separationZ * separationZ <=
                SUPER_LUCKY_DICE_TOUCH_DISTANCE *
                        SUPER_LUCKY_DICE_TOUCH_DISTANCE -
                    1) {
              separation = Iwram_Sqrt(separationX * separationX +
                                      separationY * separationY +
                                      separationZ * separationZ);
              // UB: coincident dice divide by zero.
#ifdef UBFIX
              if (separation != 0)
#endif
              {
                die->velocityX += separationX * (FIXED_ONE / 2) / separation;
                die->velocityY += separationY * (FIXED_ONE / 2) / separation;
                die->velocityZ += separationZ * (FIXED_ONE / 2) / separation;
              }
            }
          }
        }
        wallX = FIXEDtoINT(die->x);
        wallZ = FIXEDtoINT(die->z);
        if (Iwram_Sqrt(wallX * wallX + wallZ * wallZ) > DICE_DOME_RADIUS - 1) {
          if (die->x > 0)
            die->velocityX = -ABS(die->velocityX);
          else
            die->velocityX = ABS(die->velocityX);
          if (die->z > 0)
            die->velocityZ = -ABS(die->velocityZ);
          else
            die->velocityZ = ABS(die->velocityZ);
        }
        die->velocityY += -(toFIXED(1) / 2);
        if (die->y <= 0) {
          die->y = 0;
          die->velocityY = -die->velocityY * 48 / DAMPING_SCALE;
          if ((Random16() & 3) == 0) {
            s32 speed;
            s32 angle;
            s32 speedX = die->velocityX >> 8;
            s32 speedZ = die->velocityZ >> 8;

            speed = Iwram_Sqrt(speedX * speedX + speedZ * speedZ) * 56 /
                    DAMPING_SCALE;
            angle = Random16() & TRIG_ANGLE_MASK;
            die->velocityX = Trig_Sin(angle) * speed >> 8;
            die->velocityZ = Trig_Cos(angle) * speed >> 8;
          } else {
            die->velocityX = die->velocityX * 56 / DAMPING_SCALE;
            die->velocityZ = die->velocityZ * 56 / DAMPING_SCALE;
          }
          if (spinRate[index] >= DICE_BOUNCE_SPIN_LOSS)
            spinRate[index] -= DICE_BOUNCE_SPIN_LOSS;
          if (die->velocityY < toFIXED(3)) {
            s32 spin;
            s32 facing;

            die->velocityX = 0;
            die->velocityY = 0;
            die->velocityZ = 0;
            spinRate[index] = 0;
            spin = die->variant;
            facing = spin / DICE_ROLL_PER_TILE % DICE_TILES_PER_FACE;
            if (facing == 1)
              die->variant = spin + 2 * DICE_ROLL_PER_TILE;
            else if (facing == 2)
              die->variant = spin + DICE_ROLL_PER_TILE;
          }
        }
        die->velocityX = die->velocityX * 63 / DAMPING_SCALE;
        die->velocityY = die->velocityY * 63 / DAMPING_SCALE;
        die->velocityZ = die->velocityZ * 63 / DAMPING_SCALE;
        die->variant += spinRate[index];
        if (die->variant > DICE_ROLL_TURN - 1)
          die->variant -= DICE_ROLL_TURN;
        if (spinRate[index] > 0)
          spinRate[index]--;
        if (die->velocityX == 0 && die->velocityY == 0 && die->velocityZ == 0 &&
            die->y == 0) {
          numbers[index] = die->variant / DICE_ROLL_PER_FACE;
          stoppedDice++;
        }
      }
      if (stoppedDice == SUPER_LUCKY_DICE_COUNT) {
        state = SUPER_LUCKY_DICE_RESULT;
        timer = 0;
        total = 0;
        for (index = 0; index != SUPER_LUCKY_DICE_COUNT; index++)
          total = total + numbers[index] + 1;
      }
    } else if (state == SUPER_LUCKY_DICE_RESULT) {
      // The total and the banner fly in, with the result's music at frame 40;
      // a win moves on at frame 68, a loss at 90.
      timer++;
      if (timer == 24)
        Sound_PlayFar(SE_JUMP);
      if (timer == 70)
        Sound_PlayFar(SE_CURSE);
      if (timer == 40) {
        if ((u32)resultKind > SUPER_LUCKY_DICE_RESULT_WIN)
          Sound_PlayFar(MUS_TEST_YOUR_SKILLS);
        else if (jingle != -1)
          Sound_PlayFar(jingle);
        jingle = -1;
      }
      if (timer == 1) {
        loadPictures = DICE_PICTURES_RESULT;
        resultKind = SUPER_LUCKY_DICE_RESULT_LOSE;
        jingle = -1;
        if (hiLoGame == 0) {
          resultKind = SuperLuckyDice_ScoreDice(numbers);
          switch (resultKind) {
          case SUPER_LUCKY_DICE_RESULT_PERFECT:
            jingle = MUS_SUPER_LUCKY;
            multiplier = SUPER_LUCKY_DICE_PERFECT_PAYOUT;
            break;
          case SUPER_LUCKY_DICE_RESULT_ONE_PAIR:
            jingle = MUS_LUCKY;
            multiplier = SUPER_LUCKY_DICE_ONE_PAIR_PAYOUT;
            break;
          case SUPER_LUCKY_DICE_RESULT_TWO_PAIR:
            jingle = MUS_VERY_LUCKY;
            multiplier = SUPER_LUCKY_DICE_TWO_PAIR_PAYOUT;
            break;
          case SUPER_LUCKY_DICE_RESULT_TRIPLE:
            jingle = MUS_LUCKY;
            multiplier = SUPER_LUCKY_DICE_TRIPLE_PAYOUT;
            break;
          }
        } else if (lower == 0) {
          if (total > target)
            resultKind = SUPER_LUCKY_DICE_RESULT_WIN;
        } else if (total < target) {
          resultKind = SUPER_LUCKY_DICE_RESULT_WIN;
        }
        if (resultKind == SUPER_LUCKY_DICE_RESULT_WIN) {
          if (hiLoGame <= 3)
            jingle = MUS_LUCKY;
          if (hiLoGame == 4)
            jingle = MUS_VERY_LUCKY;
          if (hiLoGame == DICE_FINAL_GAME)
            jingle = MUS_SUPER_LUCKY;
        }
      }
      if (resultKind == SUPER_LUCKY_DICE_RESULT_LOSE ? timer == 90
                                                     : timer == 68) {
        if (resultKind != SUPER_LUCKY_DICE_RESULT_LOSE) {
          if (hiLoGame == 0)
            bet *= multiplier;
          else
            bet <<= 1;
          Text_DrawNumberInWindowFar(bet, DICE_COIN_DIGITS,
                                     (s32)sprites->betWindow, BET_TEXT_X, 5);
          multiplier <<= 1;
          if (hiLoGame == DICE_FINAL_GAME) {
            target = DICE_MIDDLE_TOTAL;
            hiLoGame = 0;
            winnings = bet;
            state = SUPER_LUCKY_DICE_PAYOUT;
          } else {
            state = SUPER_LUCKY_DICE_DOUBLE_UP;
            loadPictures = DICE_PICTURES_DOUBLE_UP;
            cashOut = 0;
            window = Window_CreateFar(4, 16, 20, 4, 6);
            sprites->helpWindow = window;
            Text_DrawMessageAsTilesFar((s32)&MsgDiceGameDoubleUp, window, 0, 0);
          }
          timer = 0;
          slide = 0;
        } else {
          target = DICE_MIDDLE_TOTAL;
          hiLoGame = 0;
          bet = Party_GetAverageLevelFar() * DICE_BET_PER_LEVEL;
          Text_DrawNumberInWindowFar(bet, DICE_COIN_DIGITS,
                                     (s32)sprites->betWindow, BET_TEXT_X, 5);
          multiplier = 0;
          Sound_PlayFar(MUS_SUPER_LUCKY_DICE);
          state = SUPER_LUCKY_DICE_THROW;
          window = Window_CreateFar(SUPER_LUCKY_DICE_HELP_WINDOW_X, 16,
                                    SUPER_LUCKY_DICE_HELP_WINDOW_WIDTH, 4, 6);
          sprites->helpWindow = window;
          Text_DrawMessageAsTilesFar((s32)&MsgDiceGameThrow, window, 0, 0);
          Text_DrawMessageAsTilesFar((s32)&MsgDiceGameThrow + 1,
                                     sprites->helpWindow, 0, 8);
          slide = 0;
          timer = 0;
          loadPictures = DICE_PICTURES_HIGHER_LOWER;
        }
      }
    } else if (state == SUPER_LUCKY_DICE_PAYOUT) {
      // The winnings window opens at frame 20 and the winnings count into the
      // coins a coin a frame; A pays the rest at once and goes back to
      // throwing.
      if (++timer == 20) {
        if (winnings < 10)
          digits = 1;
        else if (winnings < 100)
          digits = 2;
        else if (winnings < 1000)
          digits = 3;
        else if (winnings < 10000)
          digits = 4;
        else
          digits = 5;
        window = Window_CreateFar(WINNINGS_WINDOW_X - ((u32)digits >> 1), 16,
                                  digits + WINNINGS_WINDOW_MARGIN, 3, 6);
        sprites->helpWindow = window;
        Text_DrawMessageAsTilesFar((s32)&MsgLuckyDiceWinnings, window, 0, 0);
        Text_DrawMessageAsTilesFar((s32)&MsgLuckyDiceWinnings - 1,
                                   sprites->helpWindow,
                                   digits * 8 + WINNINGS_LABEL_X, 0);
        Text_DrawNumberInWindowFar(winnings, digits, (s32)sprites->helpWindow,
                                   WINNINGS_TEXT_X, 0);
      }
      if (timer > 19) {
        if (gInput.repeat & KEY_A) {
          Window_CloseFar(sprites->helpWindow, 1);
          Sound_PlayFar(MUS_SUPER_LUCKY_DICE);
          Sound_PlayFar(SE_MENU_CONFIRM);
          state = SUPER_LUCKY_DICE_THROW;
          window = Window_CreateFar(SUPER_LUCKY_DICE_HELP_WINDOW_X, 16,
                                    SUPER_LUCKY_DICE_HELP_WINDOW_WIDTH, 4, 6);
          sprites->helpWindow = window;
          Text_DrawMessageAsTilesFar((s32)&MsgDiceGameThrow, window, 0, 0);
          Text_DrawMessageAsTilesFar((s32)&MsgDiceGameThrow + 1,
                                     sprites->helpWindow, 0, 8);
          slide = 0;
          timer = 0;
          loadPictures = DICE_PICTURES_HIGHER_LOWER;
          bet = Party_GetAverageLevelFar() * DICE_BET_PER_LEVEL;
          Text_DrawNumberInWindowFar(bet, DICE_COIN_DIGITS,
                                     (s32)sprites->betWindow, BET_TEXT_X, 5);
          if (winnings > 0) {
            Sound_PlayFar(SE_COIN_PAYOUT);
            Party_AdjustCoinsFar(winnings);
            Text_DrawNumberInWindowFar(gMainRoutineState.coins,
                                       DICE_COIN_DIGITS,
                                       (s32)sprites->coinsWindow, 0, 0);
          }
        } else if (winnings > 0) {
          if (--winnings == 0)
            Sound_PlayFar(SE_COIN_PAYOUT);
          Party_AdjustCoinsFar(1);
          Text_DrawNumberInWindowFar(gMainRoutineState.coins, DICE_COIN_DIGITS,
                                     (s32)sprites->coinsWindow, 0, 0);
          bet--;
          Text_DrawNumberInWindowFar(bet, DICE_COIN_DIGITS,
                                     (s32)sprites->betWindow, BET_TEXT_X, 5);
        }
      }
    } else if (state == SUPER_LUCKY_DICE_DOUBLE_UP) {
      // Double Up and Cash Out slide in; A chooses, and Double Up plays the
      // next hi-lo game against this total.
      if (slide == 4)
        Sound_PlayFar(SE_NOTE_HIGH);
      if (slide <= 7) {
        slide++;
      } else if (timer > 0) {
        if (++timer == 16) {
          Window_CloseFar(sprites->helpWindow, 1);
          if (cashOut == 0) {
            target = total;
            hiLoGame++;
            state = SUPER_LUCKY_DICE_GUESS;
            window = Window_CreateFar(RESULT_WINDOW_X, 16, RESULT_WINDOW_WIDTH,
                                      4, 6);
            sprites->helpWindow = window;
            if (total == DICE_HIGHEST_TOTAL) {
              lower = 1;
              Text_DrawMessageAsTilesFar((s32)&MsgDiceGameLowerAgain, window, 0,
                                         0);
            } else {
              lower = 0;
              Text_DrawMessageAsTilesFar((s32)&MsgDiceGameHigherAgain, window,
                                         0, 0);
            }
            Text_DrawMessageAsTilesFar((s32)&MsgDiceGameChoose,
                                       sprites->helpWindow, 0, 8);
            slide = 0;
            loadPictures = DICE_PICTURES_HIGHER_LOWER;
          } else {
            target = DICE_MIDDLE_TOTAL;
            hiLoGame = 0;
            winnings = bet;
            state = SUPER_LUCKY_DICE_PAYOUT;
          }
          timer = 0;
        }
      } else if (gInput.repeat & KEY_A) {
        timer = 1;
        Sound_PlayFar(SE_MENU_CONFIRM);
      } else if ((gInput.repeat & KEY_UP) && cashOut == 1) {
        Sound_PlayFar(SE_MENU_CURSOR_MOVE);
        cashOut = 0;
      } else if ((gInput.repeat & KEY_DOWN) && cashOut == 0) {
        Sound_PlayFar(SE_MENU_CURSOR_MOVE);
        cashOut = 1;
      }
      if (cashOut == 0) {
        SuperLuckyDice_BrightenColors(
            spritePalette + DICE_PALETTE_RELOADED_C * PALETTE_COLORS,
            (u16 *)&OBJ_PLTT_COLOR(DICE_PALETTE_RELOADED_C, 0), PALETTE_COLORS,
            timer * (TRIG_TURN / 16));
        SuperLuckyDice_ScaleRgb555Buffer(
            spritePalette + DICE_PALETTE_RELOADED_D * PALETTE_COLORS,
            (u16 *)&OBJ_PLTT_COLOR(DICE_PALETTE_RELOADED_D, 0), FIXED_ONE / 2,
            PALETTE_COLORS);
      } else {
        SuperLuckyDice_BrightenColors(
            spritePalette + DICE_PALETTE_RELOADED_D * PALETTE_COLORS,
            (u16 *)&OBJ_PLTT_COLOR(DICE_PALETTE_RELOADED_D, 0), PALETTE_COLORS,
            timer * (TRIG_TURN / 16));
        SuperLuckyDice_ScaleRgb555Buffer(
            spritePalette + DICE_PALETTE_RELOADED_C * PALETTE_COLORS,
            (u16 *)&OBJ_PLTT_COLOR(DICE_PALETTE_RELOADED_C, 0), FIXED_ONE / 2,
            PALETTE_COLORS);
      }
    }

    // Load the pictures the state asked for.
    if (loadPictures != DICE_PICTURES_NONE) {
      if (loadPictures == DICE_PICTURES_HIGHER_LOWER) {
        resourceAddress = Resource_GetTableEntry(RESOURCE_SLOTS_HIGHER_BANNER);
        Dma_Set(resourceAddress,
                (void *)&OBJ_PLTT_COLOR(DICE_PALETTE_RELOADED_A, 0),
                DMA_COPY_WORDS(8), REG_DMA3);
        Dma_Set(resourceAddress,
                spritePalette + DICE_PALETTE_RELOADED_A * PALETTE_COLORS,
                DMA_COPY_WORDS(8), REG_DMA3);
        resourceAddress += PALETTE_BYTES;
        Decompress_TaggedLz(resourceAddress, cellBuffer);
        Dma_Set(cellBuffer, (void *)(OBJ_VRAM0 + HIGHER_TILES * TILE_SIZE_4BPP),
                DMA_COPY_WORDS(384), REG_DMA3);
        resourceAddress = Resource_GetTableEntry(RESOURCE_SLOTS_LOWER_BANNER);
        Dma_Set(resourceAddress,
                (void *)&OBJ_PLTT_COLOR(DICE_PALETTE_RELOADED_B, 0),
                DMA_COPY_WORDS(8), REG_DMA3);
        Dma_Set(resourceAddress,
                spritePalette + DICE_PALETTE_RELOADED_B * PALETTE_COLORS,
                DMA_COPY_WORDS(8), REG_DMA3);
        resourceAddress += PALETTE_BYTES;
        Decompress_TaggedLz(resourceAddress, cellBuffer);
        Dma_Set(cellBuffer, (void *)(OBJ_VRAM0 + LOWER_TILES * TILE_SIZE_4BPP),
                DMA_COPY_WORDS(384), REG_DMA3);
        resourceAddress = Resource_GetTableEntry(RESOURCE_SLOTS_PINK_DIGITS);
        {
          u16 *src;
          u16 *dst;
          s32 red;
          s32 blue;

          // The glowing digits: the pink digits' blue and red raised by four,
          // red and blue taking the blue and green the red.
          dst = spritePalette + DICE_PALETTE_RELOADED_C * PALETTE_COLORS;
          src = (u16 *)resourceAddress;
          for (index = 0; index != PALETTE_COLORS; index++) {
            blue = RGB_BLUE(*src) + 4;
            red = RGB_RED(*src) + 4;
            if (red > RGB_LEVEL_MAX)
              red = RGB_LEVEL_MAX;
            if (blue > RGB_LEVEL_MAX)
              blue = RGB_LEVEL_MAX;
            *dst = RGB(blue, red, blue);
            src++;
            dst++;
          }
        }
        Dma_Set(spritePalette + DICE_PALETTE_RELOADED_C * PALETTE_COLORS,
                (void *)&OBJ_PLTT_COLOR(DICE_PALETTE_RELOADED_C, 0),
                DMA_COPY_WORDS(8), REG_DMA3);
        Dma_Set(spritePalette + DICE_PALETTE_RELOADED_C * PALETTE_COLORS,
                (void *)&OBJ_PLTT_COLOR(DICE_PALETTE_RELOADED_D, 0),
                DMA_COPY_WORDS(8), REG_DMA3);
        Dma_Set(spritePalette + DICE_PALETTE_RELOADED_C * PALETTE_COLORS,
                spritePalette + DICE_PALETTE_RELOADED_D * PALETTE_COLORS,
                DMA_COPY_WORDS(8), REG_DMA3);
      } else if (loadPictures == DICE_PICTURES_RESULT) {
        switch (resultKind) {
        case SUPER_LUCKY_DICE_RESULT_LOSE:
          resourceAddress =
              Resource_GetTableEntry(RESOURCE_SLOTS_YOU_LOSE_BANNER);
          break;
        case SUPER_LUCKY_DICE_RESULT_WIN:
          resourceAddress =
              Resource_GetTableEntry(RESOURCE_SLOTS_YOU_WIN_BANNER);
          break;
        case SUPER_LUCKY_DICE_RESULT_ONE_PAIR:
          resourceAddress =
              Resource_GetTableEntry(RESOURCE_SLOTS_ONE_PAIR_BANNER);
          break;
        case SUPER_LUCKY_DICE_RESULT_TWO_PAIR:
          resourceAddress =
              Resource_GetTableEntry(RESOURCE_SLOTS_TWO_PAIR_BANNER);
          break;
        case SUPER_LUCKY_DICE_RESULT_TRIPLE:
          resourceAddress =
              Resource_GetTableEntry(RESOURCE_SLOTS_TRIPLE_BANNER);
          break;
        case SUPER_LUCKY_DICE_RESULT_PERFECT:
          resourceAddress =
              Resource_GetTableEntry(RESOURCE_SLOTS_PERFECT_BANNER);
          break;
        }
        Dma_Set(resourceAddress,
                (void *)&OBJ_PLTT_COLOR(DICE_PALETTE_RELOADED_B, 0),
                DMA_COPY_WORDS(8), REG_DMA3);
        resourceAddress += PALETTE_BYTES;
        Decompress_TaggedLz(resourceAddress, cellBuffer);
        Dma_Set(cellBuffer,
                (void *)(OBJ_VRAM0 + RESULT_BANNER_TILES * TILE_SIZE_4BPP),
                DMA_COPY_WORDS(1024), REG_DMA3);
        resourceAddress = Resource_GetTableEntry(RESOURCE_SLOTS_GREEN_DIGITS);
        Dma_Set(resourceAddress,
                (void *)&OBJ_PLTT_COLOR(DICE_PALETTE_RELOADED_A, 0),
                DMA_COPY_WORDS(8), REG_DMA3);
        resourceAddress += PALETTE_BYTES;
        Decompress_TaggedLz(resourceAddress, cellBuffer);
        Dma_Set(cellBuffer,
                (void *)(OBJ_VRAM0 + GREEN_DIGIT_TILES * TILE_SIZE_4BPP),
                DMA_COPY_WORDS(1280), REG_DMA3);
      } else if (loadPictures == DICE_PICTURES_DOUBLE_UP) {
        resourceAddress =
            Resource_GetTableEntry(RESOURCE_SLOTS_DOUBLE_UP_BANNER);
        Dma_Set(resourceAddress,
                (void *)&OBJ_PLTT_COLOR(DICE_PALETTE_RELOADED_C, 0),
                DMA_COPY_WORDS(8), REG_DMA3);
        Dma_Set(resourceAddress,
                spritePalette + DICE_PALETTE_RELOADED_C * PALETTE_COLORS,
                DMA_COPY_WORDS(8), REG_DMA3);
        resourceAddress += PALETTE_BYTES;
        Decompress_TaggedLz(resourceAddress, cellBuffer);
        Dma_Set(cellBuffer,
                (void *)(OBJ_VRAM0 + DOUBLE_UP_TILES * TILE_SIZE_4BPP),
                DMA_COPY_WORDS(640), REG_DMA3);
        resourceAddress =
            Resource_GetTableEntry(RESOURCE_SLOTS_CASH_OUT_BANNER);
        Dma_Set(resourceAddress,
                (void *)&OBJ_PLTT_COLOR(DICE_PALETTE_RELOADED_D, 0),
                DMA_COPY_WORDS(8), REG_DMA3);
        Dma_Set(resourceAddress,
                spritePalette + DICE_PALETTE_RELOADED_D * PALETTE_COLORS,
                DMA_COPY_WORDS(8), REG_DMA3);
        resourceAddress += PALETTE_BYTES;
        Decompress_TaggedLz(resourceAddress, cellBuffer);
        Dma_Set(cellBuffer,
                (void *)(OBJ_VRAM0 + CASH_OUT_TILES * TILE_SIZE_4BPP),
                DMA_COPY_WORDS(640), REG_DMA3);
      }
      loadPictures = DICE_PICTURES_NONE;
    }

    // In a hi-lo game, which one at the top ("Double Up", then "Game" and its
    // number, or "Final Game") and the multiplier below.
    if (hiLoGame > 0) {
      s32 multiplierDigits;

      for (index = 0; index != 4; index++) {
        sprites->oam[spriteCount].attr01 =
            OBJ_POSITION(512 + index * 16, 2) | OBJ_SIZE_16X16;
        sprites->oam[spriteCount].attr2 =
            (SMALL_DOUBLE_UP_TILES + index * SPRITE_TILES(16, 16)) |
            OBJ_PALETTE(DICE_PALETTE_SMALL_DOUBLE_UP);
        spriteCount++;
      }
      if (hiLoGame == DICE_FINAL_GAME) {
        for (index = 0; index != 5; index++) {
          sprites->oam[spriteCount].attr01 =
              OBJ_POSITION(584 + index * 16, 2) | OBJ_SIZE_16X16;
          sprites->oam[spriteCount].attr2 =
              (FINAL_GAME_TILES + index * SPRITE_TILES(16, 16)) |
              OBJ_PALETTE(DICE_PALETTE_FINAL_GAME);
          spriteCount++;
        }
      } else {
        for (index = 0; index != 3; index++) {
          sprites->oam[spriteCount].attr01 =
              OBJ_POSITION(GAME_BANNER_X + index * 16, 2) | OBJ_SIZE_16X16;
          sprites->oam[spriteCount].attr2 =
              (GAME_TILES + index * SPRITE_TILES(16, 16)) |
              OBJ_PALETTE(DICE_PALETTE_FINAL_GAME);
          spriteCount++;
        }
        sprites->oam[spriteCount].attr01 = GAME_NUMBER_WORD;
        sprites->oam[spriteCount].attr2 =
            (hiLoGame * SPRITE_TILES(16, 32) + ORANGE_DIGIT_TILES) |
            OBJ_PALETTE(DICE_PALETTE_ORANGE_DIGITS);
        spriteCount++;
#if LANGUAGE == LANGUAGE_SPANISH
        if (hiLoGame == 1) {
          sprites->oam[spriteCount].attr01 = OBJ_X(80) | OBJ_SIZE_16X32;
          sprites->oam[spriteCount].attr2 =
              ORANGE_DIGIT_TILES | OBJ_PALETTE(DICE_PALETTE_ORANGE_DIGITS);
        } else {
          sprites->oam[spriteCount].attr01 = OBJ_X(84) | OBJ_SIZE_16X32;
          sprites->oam[spriteCount].attr2 =
              ORANGE_DIGIT_TILES | OBJ_PALETTE(DICE_PALETTE_ORANGE_DIGITS);
        }
        spriteCount++;
#endif
      }
      for (index = 0; index != 2; index++) {
        sprites->oam[spriteCount].attr01 =
            OBJ_POSITION(576 + index * 16, 24) | OBJ_SIZE_16X16;
        sprites->oam[spriteCount].attr2 =
            (OR_TILES + index * SPRITE_TILES(16, 16)) |
            OBJ_PALETTE(DICE_PALETTE_TIMES_SIGN);
        spriteCount++;
      }
      if (multiplier < 10)
        multiplierDigits = 1;
      else if (multiplier < 100)
        multiplierDigits = 2;
      else if (multiplier < 1000)
        multiplierDigits = 3;
      else if (multiplier < 10000)
        multiplierDigits = 4;
      else
        multiplierDigits = 5;
      sprites->oam[spriteCount].attr01 =
          OBJ_POSITION(560 - multiplierDigits * 10, 24) | OBJ_SIZE_16X16;
      sprites->oam[spriteCount].attr2 =
          TIMES_SIGN_TILES | OBJ_PALETTE(DICE_PALETTE_TIMES_SIGN);
      spriteCount++;
      spriteCount = SuperLuckyDice_DrawNumber(spriteCount, 48, 20, multiplier,
                                              DICE_PALETTE_PINK_DIGITS, 5);
      spriteCount = SuperLuckyDice_DrawNumber(
          spriteCount, MULTIPLIER_ZERO_X, 20, 0, DICE_PALETTE_PALE_DIGITS, 1);
    }

    // Higher and Lower, each beside the nearest total it wins with; the chosen
    // one's number bobs four pixels, and once A is pressed the other slides
    // away.
    if (state == SUPER_LUCKY_DICE_GUESS) {
      s32 x;
      s32 wobble;
      s32 left;

      x = (timer > 0 && lower == 1) ? timer * 12 + CHOICE_COLUMN_X
                                    : CHOICE_COLUMN_X;
      wobble = lower == 0
                   ? Trig_Sin(frame * (TRIG_TURN / 64) & (TRIG_TURN / 2 - 1)) >>
                         CHOICE_BOB_SHIFT
                   : 0;
      left = x + slide * 15;
      if (target < DICE_HIGHEST_TOTAL)
        spriteCount =
            SuperLuckyDice_DrawNumber(spriteCount, left - 110, wobble + 60,
                                      target + 1, DICE_PALETTE_RELOADED_C, 2);
      x = (timer > 0 && lower == 0) ? timer * 12 + CHOICE_COLUMN_X
                                    : CHOICE_COLUMN_X;
      wobble = lower == 1
                   ? Trig_Sin(frame * (TRIG_TURN / 64) & (TRIG_TURN / 2 - 1)) >>
                         CHOICE_BOB_SHIFT
                   : 0;
      left = x + slide * 15;
      if (target > DICE_LOWEST_TOTAL)
        spriteCount =
            SuperLuckyDice_DrawNumber(spriteCount, left - 110, 94 - wobble,
                                      target - 1, DICE_PALETTE_RELOADED_D, 2);
      for (index = 0; index != 3; index++) {
        s32 column;

        column = CHOICE_COLUMN_X;
        if (timer > 0 && lower == 1)
          column = timer * 12 + CHOICE_COLUMN_X;
        column += slide * 15;
        column -= 120;
        sprites->oam[spriteCount].attr01 =
            OBJ_POSITION(column + index * 32 + 512, 52) | OBJ_SIZE_32X32;
        sprites->oam[spriteCount].attr2 =
            OBJ_PALETTE(DICE_PALETTE_RELOADED_A) |
            (HIGHER_TILES + index * SPRITE_TILES(32, 32));
        spriteCount++;
        column = CHOICE_COLUMN_X;
        if (timer > 0 && lower == 0)
          column = timer * 12 + CHOICE_COLUMN_X;
        column += slide * 15;
        column -= 120;
        sprites->oam[spriteCount].attr01 =
            OBJ_POSITION(column + index * 32 + 512, 92) | OBJ_SIZE_32X32;
        sprites->oam[spriteCount].attr2 =
            (LOWER_TILES + index * SPRITE_TILES(32, 32)) |
            OBJ_PALETTE(DICE_PALETTE_RELOADED_B);
        spriteCount++;
      }
    }

    // Double Up and Cash Out; once A is pressed the one not chosen slides away.
    if (state == SUPER_LUCKY_DICE_DOUBLE_UP) {
      for (index = 0; index != 5; index++) {
        s32 column;

        column = CHOICE_COLUMN_X;
        if (timer > 0 && cashOut == 1)
          column = timer * 12 + CHOICE_COLUMN_X;
        column += slide * 15;
        column -= 120;
        sprites->oam[spriteCount].attr01 =
            OBJ_POSITION(column + index * 32 + 512, 52) | OBJ_SIZE_32X32;
        sprites->oam[spriteCount].attr2 =
            (DOUBLE_UP_TILES + index * SPRITE_TILES(32, 32)) |
            OBJ_PALETTE(DICE_PALETTE_RELOADED_C);
        spriteCount++;
        column = CHOICE_COLUMN_X;
        if (timer > 0 && cashOut == 0)
          column = timer * 12 + CHOICE_COLUMN_X;
        column += slide * 15;
        column -= 120;
        sprites->oam[spriteCount].attr01 =
            OBJ_POSITION(column + index * 32 + 512, 84) | OBJ_SIZE_32X32;
        sprites->oam[spriteCount].attr2 =
            (CASH_OUT_TILES + index * SPRITE_TILES(32, 32)) |
            OBJ_PALETTE(DICE_PALETTE_RELOADED_D);
        spriteCount++;
      }
    }

    // The total and the result's banner, each sliding in from the left 16
    // pixels a frame. Offered the hi-lo game, the total arcs over to the left
    // and the banner slides off to the right; after a loss both sway ever wider
    // and fall away.
    if (state == SUPER_LUCKY_DICE_RESULT ||
        (state == SUPER_LUCKY_DICE_DOUBLE_UP &&
         resultKind != SUPER_LUCKY_DICE_RESULT_LOSE)) {
      s32 x;
      s32 y;
      s32 angle;
      s32 doubled;
      s32 swayWidth;

      x = (timer << 4) - 376;
      y = TOTAL_Y;
      if (x > TOTAL_X)
        x = TOTAL_X;
      if (state == SUPER_LUCKY_DICE_DOUBLE_UP) {
        angle = slide * (TRIG_TURN / 16);
        angle += TRIG_TURN / 16;
        x = FIXEDtoINT(Trig_Cos(angle) * TOTAL_ARC_RADIUS_X) - (timer << 4) +
            56;
        y = TOTAL_Y - FIXEDtoINT(Trig_Sin(angle) * TOTAL_ARC_RADIUS_Y);
      } else if (resultKind != SUPER_LUCKY_DICE_RESULT_LOSE) {
        // Unused: a win would circle from frame 68, a turn in 44 frames, but
        // it moves on at frame 68.
        if (timer > 67) {
          s32 elapsed = timer - 68;

          angle = elapsed * (TRIG_TURN / 44);
          x = FIXEDtoINT(Trig_Cos(angle) * TOTAL_CIRCLE_RADIUS) + 52;
          y = TOTAL_Y - FIXEDtoINT(Trig_Sin(angle) * TOTAL_CIRCLE_RADIUS);
        }
      } else if (timer > 67) {
        doubled = timer * 2;
        swayWidth = doubled - 128;
        x = FIXEDtoINT(Trig_Sin(timer * (TRIG_TURN / 16) - TRIG_TURN * 4) *
                       swayWidth) +
            TOTAL_X;
        doubled += timer;
        doubled *= 2;
        y = doubled - 360;
      }
      if (x >= -31)
        spriteCount = SuperLuckyDice_DrawLargeNumber(
            spriteCount, x, y, total, DICE_PALETTE_RELOADED_A, 2);
      if (timer > 30 || (state == SUPER_LUCKY_DICE_DOUBLE_UP &&
                         resultKind != SUPER_LUCKY_DICE_RESULT_LOSE)) {
        s32 bannerX;
        s32 bannerY;

        bannerY = RESULT_BANNER_Y;
        bannerX = timer > 37 ? RESULT_BANNER_X : (timer << 4) - 620;
        if (state == SUPER_LUCKY_DICE_DOUBLE_UP) {
          bannerY = RESULT_BANNER_Y;
          bannerX = slide * 24 + RESULT_BANNER_X;
        } else if (timer > 67) {
          doubled = timer * 2;
          swayWidth = doubled - 136;
          bannerX = FIXEDtoINT(Trig_Sin(timer * (TRIG_TURN / 16) -
                                        TRIG_TURN * 17 / 4) *
                               swayWidth) +
                    RESULT_BANNER_X;
          doubled += timer;
          doubled *= 2;
          bannerY = doubled - 344;
        }
        sprites->oam[spriteCount].attr01 =
            OBJ_POSITION(bannerX + 512, bannerY + 256) | OBJ_SIZE_64X64;
        sprites->oam[spriteCount].attr2 =
            RESULT_BANNER_TILES | OBJ_PALETTE(DICE_PALETTE_RELOADED_B);
        spriteCount++;
        sprites->oam[spriteCount].attr01 =
            OBJ_POSITION(bannerX + 576, bannerY + 256) | OBJ_SIZE_64X64;
        sprites->oam[spriteCount].attr2 =
            (RESULT_BANNER_TILES + SPRITE_TILES(64, 64)) |
            OBJ_PALETTE(DICE_PALETTE_RELOADED_B);
        spriteCount++;
      }
    }

    // The dice in order of depth (an insertion sort), each scaled by its
    // distance.
    for (index = 0; index != SUPER_LUCKY_DICE_COUNT; index++)
      drawOrder[index] = index;
    for (index = 1; index != SUPER_LUCKY_DICE_COUNT; index++) {
      s32 sortedDie;

      sortedDie = drawOrder[index];
      for (otherIndex = index - 1;
           otherIndex >= 0 && dice->particles[drawOrder[otherIndex]].z >
                                  dice->particles[sortedDie].z;
           otherIndex--)
        drawOrder[otherIndex + 1] = drawOrder[otherIndex];
      drawOrder[otherIndex + 1] = sortedDie;
    }
    for (index = 0; index != SUPER_LUCKY_DICE_COUNT; index++) {
      struct DiceParticle *die;
      s32 x;
      s32 y;

      die = &dice->particles[drawOrder[index]];
      Matrix_ProjectPoint(die, screen);
      if (screen[2] < DICE_NEAREST_DEPTH)
        screen[2] = DICE_NEAREST_DEPTH;
      if (screen[2] > DICE_FARTHEST_DEPTH)
        screen[2] = DICE_FARTHEST_DEPTH;
      spriteDepth[drawOrder[index]] = screen[2] - DICE_NEAREST_DEPTH;
      x = screen[0] -
          DIE_CORNER_X * (DICE_NEAREST_SCALE / 2) /
              (spriteDepth[drawOrder[index]] / 4 + DICE_NEAREST_SCALE / 2);
      y = screen[1] -
          DIE_CORNER_Y * (DICE_NEAREST_SCALE / 2) /
              (spriteDepth[drawOrder[index]] / 4 + DICE_NEAREST_SCALE / 2);
      if (y + 92 > -16) {
        sprites->oam[spriteCount].attr01 =
            (OBJ_AFFINE_MATRIX(drawOrder[index]) |
             OBJ_X((x + 632) & OBJ_X_MASK) | ((y + 348) & 255)) |
            OBJ_SIZE_16X16 | OBJ_256_COLORS | OBJ_DOUBLE_SIZE | OBJ_AFFINE;
        sprites->oam[spriteCount].attr2 =
            (gSuperLuckyDice_FaceTiles[die->variant / DICE_ROLL_PER_TILE]
             << 3) |
            OBJ_PRIORITY(1);
        spriteCount++;
      }
    }
    // Each die's shadow on the dome below it, the smaller the higher the die.
    {
      struct DiceParticle *die;

      for (index = 0; index != SUPER_LUCKY_DICE_COUNT; index++) {
        s32 x;
        s32 y;
        s32 scale;

        die = &dice->particles[index];

        groundPoint[0] = die->x;
        groundPoint[1] = 0;
        groundPoint[2] = die->z;
        Matrix_ProjectPoint(groundPoint, screen);
        if (screen[2] < DICE_NEAREST_DEPTH)
          screen[2] = DICE_NEAREST_DEPTH;
        if (screen[2] > DICE_FARTHEST_DEPTH)
          screen[2] = DICE_FARTHEST_DEPTH;
        spriteDepth[SUPER_LUCKY_DICE_COUNT + index] =
            screen[2] - DICE_NEAREST_DEPTH;
        spriteDepth[SUPER_LUCKY_DICE_COUNT + index] += (s16)FIXEDtoINT(die->y);
        scale = spriteDepth[SUPER_LUCKY_DICE_COUNT + index] / 2 +
                DICE_NEAREST_SCALE;
        x = screen[0] - SHADOW_CORNER_X * DICE_NEAREST_SCALE / scale;
        y = screen[1] - SHADOW_CORNER_Y * DICE_NEAREST_SCALE / scale;
        if (y + 92 > -16) {
          sprites->oam[spriteCount].attr01 =
              (OBJ_AFFINE_MATRIX(SUPER_LUCKY_DICE_COUNT + index) |
               OBJ_X((x + 632) & OBJ_X_MASK)) |
              ((y + 348) & 255) | OBJ_SIZE_16X16 | OBJ_256_COLORS |
              OBJ_DOUBLE_SIZE | OBJ_AFFINE;
          sprites->oam[spriteCount].attr2 = DICE_SHADOW_TILE | OBJ_PRIORITY(2);
          spriteCount++;
        }
      }
    }
    // Hide the unused sprites, write the eight affine matrices (one for each
    // die and each shadow, scaled by depth) and copy the sprites to OAM.
    for (; spriteCount != OAM_SIZE / sizeof(struct OamWords); spriteCount++) {
      sprites->oam[spriteCount].attr01 =
          OBJ_X(DISPLAY_WIDTH) | OBJ_SIZE_16X16 | OBJ_256_COLORS;
      sprites->oam[spriteCount].attr2 = 0;
    }
    for (index = 0; index != SUPER_LUCKY_DICE_COUNT * 2; index++) {
      // The sprite's matrix: its scale on the diagonal and no shear across,
      // each element stored through its address.
      u16 affineA;
      u16 affineB;
      u16 affineC;
      u16 affineD;
      s16 scaleX;
      s16 scaleY;
      s32 shear = 0;

      scaleX = spriteDepth[index] / 4 + DICE_NEAREST_SCALE;
      *&affineA = scaleX;
      *&affineB = shear;
      *&affineC = shear;
      scaleY = spriteDepth[index] / 4 + DICE_NEAREST_SCALE;
      *&affineD = scaleY;
      sprites->oam[index * 4 + 0].attr2 |= OBJ_AFFINE_PARAMETER(affineA);
      sprites->oam[index * 4 + 1].attr2 |= OBJ_AFFINE_PARAMETER(affineB);
      sprites->oam[index * 4 + 2].attr2 |= OBJ_AFFINE_PARAMETER(affineC);
      sprites->oam[index * 4 + 3].attr2 |= OBJ_AFFINE_PARAMETER(affineD);
    }
    Dma_Set(sprites->oam, (void *)OAM, DMA_COPY_WORDS(OAM_SIZE / 4), REG_DMA3);
    wait(1);
    frame++;
  }

  // Fade out and free the game's state.
  for (index = 0; index != DICE_FADE_FRAMES + 1; index++) {
    SuperLuckyDice_ScaleRgb555Buffer(
        spritePalette, (u16 *)OBJ_PLTT,
        FIXED_ONE - index * (FIXED_ONE / DICE_FADE_FRAMES), 256);
    SuperLuckyDice_ScaleRgb555Buffer(
        bgPalette, (u16 *)BG_PLTT,
        FIXED_ONE - index * (FIXED_ONE / DICE_FADE_FRAMES), 256);
    wait(1);
  }
  HeapSlot_Release(HEAP_SLOT_CAMERA);
  HeapSlot_Release(HEAP_SLOT_CANVAS_MAP);
  HeapSlot_Release(HEAP_SLOT_BATTLE_CANVAS);
  HeapSlot_Release(HEAP_SLOT_BATTLE_EFFECT);
  HeapSlot_Release(HEAP_SLOT_BATTLE_SHEET);
}
