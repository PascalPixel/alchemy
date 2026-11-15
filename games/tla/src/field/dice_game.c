/// @file
/// Super Lucky Dice, Contigo's four-dice game with its Hi-Lo double-up, in the
/// order it has in the ROM.

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
#include "system/point.h"
#include "battle/battle_camera.h"
#include "macros.h"
#include "fixed_math.h"
#include "system/dma.h"
#include "system/input.h"
#include "system/main_routine_work.h"
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
// (the dice, 256-colour, fill the first DICE_SHADOW_TILE + 8).
#define DICE_SHADOW_TILE 144      ///< the nineteenth dice frame, the shadow
#define SMALL_DOUBLE_UP_TILES 152 ///< "Double Up", for the rounds won
#define FINAL_GAME_TILES 168      ///< "Final Game", in 16 by 16 cells
#define GAME_TILES (FINAL_GAME_TILES + 20) ///< its "Game", the sixth cell on
#define ORANGE_DIGIT_TILES 200 ///< 0 to 9, 16 by 32, the round's number
#define TIMES_SIGN_TILES 280   ///< the times sign, then "or"
#define OR_TILES (TIMES_SIGN_TILES + 4)
#define DOUBLE_UP_TILES 292     ///< the large "Double Up" choice
#define CASH_OUT_TILES 372      ///< "Cash Out"
#define HIGHER_TILES 512        ///< "Higher", in a double-up round
#define RESULT_BANNER_TILES 512 ///< "You Win" to "Perfect", over Higher
#define LOWER_TILES 560         ///< "Lower"
#define GREEN_DIGIT_TILES 640   ///< 0 to 9, 32 by 32, the total
#define PINK_DIGIT_TILES 816    ///< 0 to 9, 16 by 16

/// The sprite palettes SuperLuckyDice_Run gives its pictures, after the dice's
/// six; the last four are loaded again for each part of a round.
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

/// How a round ended, the banner it shows: a loss or a win, or for the first
/// throw, what the four dice show (SuperLuckyDice_ScoreDice).
enum SuperLuckyDiceResult {
  DICE_RESULT_LOSE = 0, ///< "You Lose"
  DICE_RESULT_WIN = 1,  ///< "You Win", a double-up guessed
  DICE_RESULT_ONE_PAIR = 2,
  DICE_RESULT_TWO_PAIR = 3,
  DICE_RESULT_TRIPLE = 4,
  DICE_RESULT_PERFECT = 5 ///< all four alike
};

/// A palette's 16 colours in bytes, as each picture's resource begins.
#define PALETTE_BYTES (PALETTE_COLORS * 2)

/// The pictures a state asks SuperLuckyDice_Run to load before the next frame.
enum SuperLuckyDicePictures {
  DICE_PICTURES_NONE = -1,
  DICE_PICTURES_HIGHER_LOWER = 1, ///< Higher and Lower, and the glowing digits
  DICE_PICTURES_RESULT = 3,   ///< the result's banner and the total's digits
  DICE_PICTURES_DOUBLE_UP = 5 ///< Double Up and Cash Out
};

s32 SuperLuckyDice_Start(void) {
  REG_DISPCNT = DISPCNT_OBJ_1D_MAP;
  SuperLuckyDice_Run();
  return 0;
}

/// Resets the scene camera and its projection, with the camera tilted to 147.
void SuperLuckyDice_ResetViewDefaults(void) {
  struct BattleCamera *state = Ram_HeapSlots->cameraWork;
  struct Point local;
  s32 *result;
  u32 ratio;
  u32 param;

  state->pitch = 147 << 8;
  state->distance = 255 << 17;
  SET_VECTOR(state->focus, 0, 0, 0);
  state->yaw = 0;
  state->followPosition = 0;
  // The projection's centre.
  gProjection.centerX = 0;
  gProjection.centerY = 0;
  state->followEye = 0;

  Matrix_Initialize();
  Matrix_TranslateVector(&state->focus[0]);
  Matrix_RotateY((s16)state->yaw);
  Matrix_RotateX((s16)state->pitch);

  local.x = 0;
  local.y = 0;
  result = (s32 *)state;
  local.z = state->distance;
  Iwram_TransformVector(&local, result);

  param = 250;
  param <<= 16;
  ratio = Iwram_RatioMulQ14(param, 192 << 8);

  param = 250;
  param <<= 16;
  Projection_Set(param, ratio, 0x7fff0000);
}

s32 SuperLuckyDice_ScaleRgb555Buffer(u16 *source, u16 *destination, s32 scale,
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
      pixel = ((red >> 16) & redMask) | ((green >> 16) & greenMask);
      pixel |= (blue >> 16) & blueMask;
      *destination = pixel;
      source++;
      destination++;
      remaining--;
    } while (remaining != 0);
  }
  return 0;
}

/// Writes count RGB555 colours from source to destination brightened by
/// 16 - cos(angle) / 4096 on each channel, each clamped at 31.
void SuperLuckyDice_BrightenColors(u16 *source, u16 *destination, u32 count,
                                   s32 angle) {
  s32 add;
  u32 i;

  add = ((-Trig_Cos(angle)) * 16 >> 16) + 16;
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
        (digit * 4 + PINK_DIGIT_TILES) | OBJ_PALETTE(palette);
    sprite++;
  }
  return sprite;
}

/// Draws value as digits large digit sprites from x, 22 pixels apart, leading
/// zeros skipped, tiles 640 + 16 * digit in palette, as
/// SuperLuckyDice_DrawNumber does. Returns the next free sprite.
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
        (digit * 16 + GREEN_DIGIT_TILES) | OBJ_PALETTE(palette);
    sprite++;
  }
  return sprite;
}

// The casino's dice game's scoring (the game itself, SuperLuckyDice_Run,
// follows it). Its one table, after its code, is gLuckyDice_FaceTiles again:
// the die face tile at each step of its roll.

/// Scores four dice for the dice game: 5 when all four match, otherwise by
/// the number of matching pairs: 2 for one, 3 for two, 4 for three, 0 for
/// none. The matched values are collected in a local array the score never
/// reads.
// The native's stack frame and stores keep that array.
s32 SuperLuckyDice_ScoreDice(s32 *dice) {
  s32 matches[2];
  s32 score;
  s32 pairs;
  s32 i;
  s32 j;

  score = DICE_RESULT_LOSE;
  if (dice[0] == dice[1] && dice[0] == dice[2] && dice[0] == dice[3]) {
    score = DICE_RESULT_PERFECT;
  } else {
    pairs = 0;
    for (i = 0; i != 3; i++) {
      for (j = i + 1; j != 4; j++) {
        if (dice[i] == dice[j]) {
          matches[pairs] = dice[i];
          pairs++;
        }
      }
    }
    if (pairs == 1)
      score = DICE_RESULT_ONE_PAIR;
    if (pairs == 2)
      score = DICE_RESULT_TWO_PAIR;
    if (pairs == 3)
      score = DICE_RESULT_TRIPLE;
  }
  return score;
}

/// The die face tile at each step of its roll, as Lucky Dice's
/// gLuckyDice_FaceTiles: one for each DICE_ROLL_PER_TILE of the roll angle,
/// three to a face.
const u8 gSuperLuckyDice_FaceTiles[] = {0, 12, 16, 1, 14, 8, 2, 15, 10,
                                        3, 9,  13, 4, 7,  6, 5, 17, 11};

// What differs between the releases: the windows' sizes and text columns by
// language; a guess the Japanese release can refuse only in the first double-up
// round, until patch 1; and the bet's clamp to the player's coins, dropped from
// patch 2.
#if LANGUAGE == LANGUAGE_JAPANESE
#define COIN_WINDOW_X 19
#define COIN_WINDOW_WIDTH 11
#define COIN_TEXT_X 48
#define HELP_WINDOW_X 22
#define HELP_WINDOW_WIDTH 8
#define BET_TEXT_X 24
#define WINNINGS_WINDOW_X 10
#define WINNINGS_WINDOW_MARGIN 10
#define WINNINGS_LABEL_X 40
#define WINNINGS_TEXT_X 40
#define RESULT_WINDOW_X 4
#define RESULT_WINDOW_WIDTH 20
#define ROUND_BANNER_X 608
#define MULTIPLIER_ZERO_X 80
#elif LANGUAGE == LANGUAGE_ENGLISH
#define COIN_WINDOW_X 18
#define COIN_WINDOW_WIDTH 12
#define COIN_TEXT_X 48
#define HELP_WINDOW_X 22
#define HELP_WINDOW_WIDTH 8
#define BET_TEXT_X 24
#define WINNINGS_WINDOW_X 9
#define WINNINGS_WINDOW_MARGIN 12
#define WINNINGS_LABEL_X 48
#define WINNINGS_TEXT_X 48
#define RESULT_WINDOW_X 4
#define RESULT_WINDOW_WIDTH 21
#define ROUND_BANNER_X 608
#define MULTIPLIER_ZERO_X 80
#elif LANGUAGE == LANGUAGE_GERMAN
#define COIN_WINDOW_X 17
#define COIN_WINDOW_WIDTH 13
#define COIN_TEXT_X 48
#define HELP_WINDOW_X 20
#define HELP_WINDOW_WIDTH 10
#define BET_TEXT_X 32
#define WINNINGS_WINDOW_X 7
#define WINNINGS_WINDOW_MARGIN 14
#define WINNINGS_LABEL_X 48
#define WINNINGS_TEXT_X 48
#define RESULT_WINDOW_X 4
#define RESULT_WINDOW_WIDTH 21
#define ROUND_BANNER_X 576
#define MULTIPLIER_ZERO_X 92
#elif LANGUAGE == LANGUAGE_SPANISH
#define COIN_WINDOW_X 18
#define COIN_WINDOW_WIDTH 12
#define COIN_TEXT_X 56
#define HELP_WINDOW_X 20
#define HELP_WINDOW_WIDTH 10
#define BET_TEXT_X 32
#define WINNINGS_WINDOW_X 9
#define WINNINGS_WINDOW_MARGIN 12
#define WINNINGS_LABEL_X 56
#define WINNINGS_TEXT_X 48
#define RESULT_WINDOW_X 4
#define RESULT_WINDOW_WIDTH 21
#define ROUND_BANNER_X 608
#define MULTIPLIER_ZERO_X 80
#elif LANGUAGE == LANGUAGE_FRENCH
#define COIN_WINDOW_X 16
#define COIN_WINDOW_WIDTH 14
#define COIN_TEXT_X 56
#define HELP_WINDOW_X 20
#define HELP_WINDOW_WIDTH 10
#define BET_TEXT_X 24
#define WINNINGS_WINDOW_X 7
#define WINNINGS_WINDOW_MARGIN 14
#define WINNINGS_LABEL_X 56
#define WINNINGS_TEXT_X 48
#define RESULT_WINDOW_X 4
#define RESULT_WINDOW_WIDTH 21
#define ROUND_BANNER_X 576
#define MULTIPLIER_ZERO_X 86
#elif LANGUAGE == LANGUAGE_ITALIAN
#define COIN_WINDOW_X 18
#define COIN_WINDOW_WIDTH 12
#define COIN_TEXT_X 56
#define HELP_WINDOW_X 20
#define HELP_WINDOW_WIDTH 10
#define BET_TEXT_X 24
#define WINNINGS_WINDOW_X 9
#define WINNINGS_WINDOW_MARGIN 12
#define WINNINGS_LABEL_X 56
#define WINNINGS_TEXT_X 48
#define RESULT_WINDOW_X 3
#define RESULT_WINDOW_WIDTH 23
#define ROUND_BANNER_X 600
#define MULTIPLIER_ZERO_X 80
#endif

#if TLA_PATCH_1
#define CANNOT_QUIT(round) ((round) > 0)
#else
#define CANNOT_QUIT(round) ((round) == 1)
#endif

#if LANGUAGE == LANGUAGE_GERMAN
#define ROUND_DIGIT_WORD (OBJ_X(116) | OBJ_SIZE_16X32)
#elif LANGUAGE == LANGUAGE_SPANISH
#define ROUND_DIGIT_WORD (OBJ_X(72) | OBJ_SIZE_16X32)
#elif LANGUAGE == LANGUAGE_FRENCH
#define ROUND_DIGIT_WORD (OBJ_X(112) | OBJ_SIZE_16X32)
#elif LANGUAGE == LANGUAGE_ITALIAN
#define ROUND_DIGIT_WORD (OBJ_X(72) | OBJ_SIZE_16X32)
#else
#define ROUND_DIGIT_WORD (OBJ_X(80) | OBJ_SIZE_16X32)
#endif

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
  SUPER_LUCKY_DICE_GUESS,    ///< a double-up round: higher or lower than this
                             ///< total
  SUPER_LUCKY_DICE_ROLL,     ///< the dice fly, bounce and settle
  SUPER_LUCKY_DICE_RESULT,   ///< the four dice are scored
  SUPER_LUCKY_DICE_PAYOUT,   ///< the winnings are counted into the coins
  SUPER_LUCKY_DICE_DOUBLE_UP ///< a win: double up or cash out
};

#define DICE_BET_PER_LEVEL 10
#define DICE_COIN_DIGITS 6
#define DICE_FADE_FRAMES 16
#define DICE_FACE_COUNT 6
#define DICE_THROW_SPIN 300
/// Each bounce takes this off a die's spin, as in Lucky Dice.
#define DICE_BOUNCE_SPIN_LOSS 20
/// Two dice whose centres come closer than this push each other apart.
#define DICE_TOUCH_DISTANCE 70
/// The round dome's radius: a die beyond it is turned back inward.
#define DICE_DOME_RADIUS 200
#define DICE_NEAREST_DEPTH 350
#define DICE_FARTHEST_DEPTH 650
#define DICE_ROLL_PER_TILE 1024
/// A die's roll angle crosses a face in DICE_TILES_PER_FACE tiles, as Lucky
/// Dice's.
#define DICE_TILES_PER_FACE 3
#define DICE_ROLL_PER_FACE (DICE_TILES_PER_FACE * DICE_ROLL_PER_TILE)
#define DICE_ROLL_TURN (DICE_FACE_COUNT * DICE_ROLL_PER_FACE)

#define ABS(value) ((value) < 0 ? -(value) : (value))

#define DICE_GAME_THROW(dice, spinRate)                                        \
  {                                                                            \
    struct DiceParticle *die;                                                  \
    for (index = 0; index != 4; index++) {                                     \
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

/// Runs a game of Super Lucky Dice.
void SuperLuckyDice_Run(void) {
  u16 bgPalette[256];
  s32 spinRate[4];
  s32 spriteDepth[8];
  s32 numbers[4];
  s32 groundPoint[3];
  s32 screen[3];
  s32 drawOrder[4];
  /* FAKEMATCH(volatile): four halfwords stored to sp+138..132 and each reloaded
   * with ldrh. */
  volatile u16 affineA;
  volatile u16 affineB;
  volatile u16 affineC;
  volatile u16 affineD;
  u16 *spritePalette;
  u8 *cellBuffer;
  struct DiceWork *dice;
  struct DiceSprites *sprites;
  struct DiceCamera *camera;
  u8 *resourceAddress;
  s32 frame;
  s32 total;
  s32 resultKind;
  s32 state;
  s32 bet;
  s32 round;
  s32 timer;
  s32 slide;
  s32 winnings;
  s32 lower;
  s32 cashOut;
  s32 target;
  s32 multiplier;
  s32 paletteRequest;
  s32 jingle;
  s32 spriteCount;
  s32 stoppedDice;
  struct RenderInput *window;
  s32 index;
  s32 otherIndex;
  s32 digits;

  spritePalette = HeapSlot_Alloc(HEAP_SLOT_BATTLE_CANVAS, 0x200);
  dice = HeapSlot_AllocEwram(HEAP_SLOT_BATTLE_EFFECT, 0x7b7c);
  sprites = HeapSlot_AllocEwram(HEAP_SLOT_CANVAS_MAP, 0x614);
  camera = HeapSlot_AllocEwram(HEAP_SLOT_CAMERA, 76);
  Resource_ReservedNoOp((s32)&ResourceId_BattleEffectFarCalls);
  SuperLuckyDice_ResetViewDefaults();
  Task_ResetAll();
  gOamCopyEnabled = 0;
  cellBuffer = gMapGridCellBuffer;

  {
    s32 tileX;
    s32 tileY;
    index = 0;
    for (tileY = 0; tileY != DISPLAY_HEIGHT / 8; tileY++) {
      for (tileX = 0; tileX != 32; tileX++) {
        *(volatile u16 *)(BG_SCREEN_BLOCK(6) + index) =
            tileX + tileY * (DISPLAY_WIDTH / 8);
        index += 2;
      }
    }
  }

  resourceAddress = Resource_GetTableEntry((s32)&ResourceId_SlotsDomePicture);
  Dma_Set((void *)BG_PLTT, bgPalette, DMA_COPY_WORDS(128), REG_DMA3);
  Dma_Set(resourceAddress, bgPalette, DMA_COPY_WORDS(112), REG_DMA3);
  resourceAddress += 0x1c0;
  Decompress_TaggedLz(resourceAddress, cellBuffer);
  Dma_Set(cellBuffer, (void *)BG_CHAR_BLOCK(1), DMA_COPY_WORDS(9600), REG_DMA3);
  resourceAddress = Resource_GetTableEntry((s32)&ResourceId_DiceFaces);
  Dma_Set(resourceAddress, spritePalette, DMA_COPY_WORDS(48), REG_DMA3);
  resourceAddress += 128;
  Decompress_TaggedLz(resourceAddress, cellBuffer);
  Dma_Set(cellBuffer, (void *)OBJ_VRAM0, DMA_COPY_WORDS(1216), REG_DMA3);
  resourceAddress =
      Resource_GetTableEntry((s32)&ResourceId_SlotsSmallDoubleUpBanner);
  Dma_Set(resourceAddress,
          spritePalette + DICE_PALETTE_SMALL_DOUBLE_UP * PALETTE_COLORS,
          DMA_COPY_WORDS(8), REG_DMA3);
  resourceAddress += PALETTE_BYTES;
  Decompress_TaggedLz(resourceAddress, cellBuffer);
  Dma_Set(cellBuffer,
          (void *)(OBJ_VRAM0 + SMALL_DOUBLE_UP_TILES * TILE_SIZE_4BPP),
          DMA_COPY_WORDS(128), REG_DMA3);
  resourceAddress =
      Resource_GetTableEntry((s32)&ResourceId_SlotsFinalGameBanner);
  Dma_Set(resourceAddress,
          spritePalette + DICE_PALETTE_FINAL_GAME * PALETTE_COLORS,
          DMA_COPY_WORDS(8), REG_DMA3);
  resourceAddress += PALETTE_BYTES;
  Decompress_TaggedLz(resourceAddress, cellBuffer);
  Dma_Set(cellBuffer, (void *)(OBJ_VRAM0 + FINAL_GAME_TILES * TILE_SIZE_4BPP),
          DMA_COPY_WORDS(256), REG_DMA3);
  resourceAddress = Resource_GetTableEntry((s32)&ResourceId_SlotsOrangeDigits);
  Dma_Set(resourceAddress,
          spritePalette + DICE_PALETTE_ORANGE_DIGITS * PALETTE_COLORS,
          DMA_COPY_WORDS(8), REG_DMA3);
  resourceAddress += PALETTE_BYTES;
  Decompress_TaggedLz(resourceAddress, cellBuffer);
  Dma_Set(cellBuffer, (void *)(OBJ_VRAM0 + ORANGE_DIGIT_TILES * TILE_SIZE_4BPP),
          DMA_COPY_WORDS(640), REG_DMA3);
  resourceAddress = Resource_GetTableEntry((s32)&ResourceId_SlotsTimesSign);
  Dma_Set(resourceAddress,
          spritePalette + DICE_PALETTE_TIMES_SIGN * PALETTE_COLORS,
          DMA_COPY_WORDS(8), REG_DMA3);
  resourceAddress += PALETTE_BYTES;
  Decompress_TaggedLz(resourceAddress, cellBuffer);
  Dma_Set(cellBuffer, (void *)(OBJ_VRAM0 + TIMES_SIGN_TILES * TILE_SIZE_4BPP),
          DMA_COPY_WORDS(96), REG_DMA3);
  resourceAddress = Resource_GetTableEntry((s32)&ResourceId_SlotsPinkDigits);
  Dma_Set(resourceAddress,
          spritePalette + DICE_PALETTE_PINK_DIGITS * PALETTE_COLORS,
          DMA_COPY_WORDS(8), REG_DMA3);
  {
    u16 *src;
    u16 *dst;
    s32 red;
    s32 blue;

    dst = spritePalette + DICE_PALETTE_PALE_DIGITS * PALETTE_COLORS;
    src = (u16 *)resourceAddress;
    for (index = 0; index != 16; index++) {
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
  resultKind = DICE_RESULT_LOSE;
  state = SUPER_LUCKY_DICE_THROW;
  round = 0;
  timer = 0;
  slide = 0;
  winnings = 0;
  lower = 0;
  cashOut = 0;
  target = 14;
  multiplier = 1;
  paletteRequest = DICE_PICTURES_NONE;
  jingle = -1;
  {
    struct DiceParticle *die;

    die = dice->particles;
    for (index = 0; index != 4; index++) {
      SET_POSITION(die, ((Random16() & 127) - 64) << 16, 0,
                   ((Random16() & 127) - 64) << 16);
      SET_POSITION(die, Trig_Sin(index << 14) * 100, 0,
                   Trig_Cos(index << 14) * 100);
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
  Text_DrawNumberInWindowFar(gMainRoutineWork.coins, DICE_COIN_DIGITS,
                             (s32)sprites->coinsWindow, 0, 0);
  window = Window_CreateFar(BET_WINDOW_X, 3, BET_WINDOW_WIDTH, 3, 6);
  sprites->betWindow = window;
  Text_DrawMessageAsTilesFar((s32)&MsgLuckyDiceCoins - 1, window, 0, 0);
  window = Window_CreateFar(HELP_WINDOW_X, 16, HELP_WINDOW_WIDTH, 4, 6);
  sprites->helpWindow = window;
  Text_DrawMessageAsTilesFar((s32)&MsgDiceGameThrow, window, 0, 0);
  Text_DrawMessageAsTilesFar((s32)&MsgDiceGameThrow + 1, sprites->helpWindow, 0,
                             8);
  bet = Party_GetAverageLevelFar() * DICE_BET_PER_LEVEL;
#if !TLA_PATCH_2
  if ((u32)bet > (u32)gMainRoutineWork.coins)
    bet = gMainRoutineWork.coins;
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
        if ((u32)gMainRoutineWork.coins <
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
          if ((u32)bet > (u32)gMainRoutineWork.coins)
            bet = gMainRoutineWork.coins;
          Text_DrawNumberInWindowFar(bet, DICE_COIN_DIGITS,
                                     (s32)sprites->betWindow, BET_TEXT_X, 5);
          Party_AdjustCoinsFar(-bet);
          Text_DrawNumberInWindowFar(gMainRoutineWork.coins, DICE_COIN_DIGITS,
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
      if (slide <= 7) {
        slide++;
      } else if (timer > 0) {
        if (++timer == 16) {
          Sound_PlayFar(SE_LUCKY_DICE_THROW);
          DICE_GAME_THROW(dice, spinRate);
          state = SUPER_LUCKY_DICE_ROLL;
        }
      } else if (gInput.repeat & KEY_B) {
        if (CANNOT_QUIT(round)) {
          Sound_PlayFar(SE_MENU_ERROR);
        } else {
          Window_CloseFar(sprites->helpWindow, 1);
          window = Window_CreateFar(HELP_WINDOW_X, 16, HELP_WINDOW_WIDTH, 4, 6);
          sprites->helpWindow = window;
          Text_DrawMessageAsTilesFar((s32)&MsgDiceGameThrow, window, 0, 0);
          Text_DrawMessageAsTilesFar((s32)&MsgDiceGameThrow + 1,
                                     sprites->helpWindow, 0, 8);
          Sound_PlayFar(SE_MENU_CANCEL);
          state = SUPER_LUCKY_DICE_THROW;
        }
      } else if (gInput.repeat & state) {
        Sound_PlayFar(SE_MENU_CONFIRM);
        timer = 1;
        Window_CloseFar(sprites->helpWindow, 1);
        if (round == 0) {
          bet = Party_GetAverageLevelFar() * DICE_BET_PER_LEVEL;
          if ((u32)bet > (u32)gMainRoutineWork.coins)
            bet = gMainRoutineWork.coins;
          Text_DrawNumberInWindowFar(bet, DICE_COIN_DIGITS,
                                     (s32)sprites->betWindow, BET_TEXT_X, 5);
          Party_AdjustCoinsFar(-bet);
          Text_DrawNumberInWindowFar(gMainRoutineWork.coins, DICE_COIN_DIGITS,
                                     (s32)sprites->coinsWindow, 0, round);
        }
      } else {
        s32 message = -1;

        if ((gInput.repeat & KEY_UP) && lower == 1) {
          if (target == 24)
            goto refuse;
          if (round == 0)
            message = (s32)&MsgDiceGameHigher;
          else
            message = (s32)&MsgDiceGameHigherAgain;
          Sound_PlayFar(SE_MENU_CURSOR_MOVE);
          lower = 0;
        } else if ((gInput.repeat & KEY_DOWN) && lower == 0) {
          if (target == 4) {
          refuse:
            Sound_PlayFar(SE_MENU_CANCEL);
          } else {
            if (round == 0)
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
            (u16 *)&OBJ_PLTT_COLOR(DICE_PALETTE_RELOADED_A, 0), 16,
            timer * (TRIG_TURN / 16));
        SuperLuckyDice_ScaleRgb555Buffer(
            spritePalette + DICE_PALETTE_RELOADED_B * PALETTE_COLORS,
            (u16 *)&OBJ_PLTT_COLOR(DICE_PALETTE_RELOADED_B, 0), FIXED_ONE / 2,
            16);
        SuperLuckyDice_BrightenColors(
            spritePalette + DICE_PALETTE_RELOADED_C * PALETTE_COLORS,
            (u16 *)&OBJ_PLTT_COLOR(DICE_PALETTE_RELOADED_C, 0), 16,
            timer * (TRIG_TURN / 16));
        SuperLuckyDice_ScaleRgb555Buffer(
            spritePalette + DICE_PALETTE_RELOADED_D * PALETTE_COLORS,
            (u16 *)&OBJ_PLTT_COLOR(DICE_PALETTE_RELOADED_D, 0), FIXED_ONE / 2,
            16);
      } else {
        SuperLuckyDice_BrightenColors(
            spritePalette + DICE_PALETTE_RELOADED_B * PALETTE_COLORS,
            (u16 *)&OBJ_PLTT_COLOR(DICE_PALETTE_RELOADED_B, 0), 16,
            timer * (TRIG_TURN / 16));
        SuperLuckyDice_ScaleRgb555Buffer(
            spritePalette + DICE_PALETTE_RELOADED_A * PALETTE_COLORS,
            (u16 *)&OBJ_PLTT_COLOR(DICE_PALETTE_RELOADED_A, 0), FIXED_ONE / 2,
            16);
        SuperLuckyDice_BrightenColors(
            spritePalette + DICE_PALETTE_RELOADED_D * PALETTE_COLORS,
            (u16 *)&OBJ_PLTT_COLOR(DICE_PALETTE_RELOADED_D, 0), 16,
            timer * (TRIG_TURN / 16));
        SuperLuckyDice_ScaleRgb555Buffer(
            spritePalette + DICE_PALETTE_RELOADED_C * PALETTE_COLORS,
            (u16 *)&OBJ_PLTT_COLOR(DICE_PALETTE_RELOADED_C, 0), FIXED_ONE / 2,
            16);
      }
    } else if (state == SUPER_LUCKY_DICE_ROLL) {
      struct DiceParticle *die;
      struct DiceParticle *other;

      stoppedDice = 0;
      for (index = 0; index != 4; index++) {
        s32 wallX, wallZ;

        die = &dice->particles[index];

        die->x += die->velocityX;
        die->y += die->velocityY;
        die->z += die->velocityZ;
        for (otherIndex = 0; otherIndex != 4; otherIndex++) {
          other = &dice->particles[otherIndex];
          if (index != otherIndex) {
            s32 separationX, separationY, separationZ, separation;

            separationX = FIXEDtoINT(die->x - other->x);
            separationY = FIXEDtoINT(die->y - other->y);
            separationZ = FIXEDtoINT(die->z - other->z);
            if (separationX * separationX + separationY * separationY +
                    separationZ * separationZ <=
                DICE_TOUCH_DISTANCE * DICE_TOUCH_DISTANCE - 1) {
              separation = Iwram_Sqrt(separationX * separationX +
                                      separationY * separationY +
                                      separationZ * separationZ);
// UB: a negative separation is shifted, and two dice at one place divide by
// zero, as in Lucky Dice.
#ifdef UBFIX
              if (separation != 0) {
                die->velocityX += separationX * (FIXED_ONE / 2) / separation;
                die->velocityY += separationY * (FIXED_ONE / 2) / separation;
                die->velocityZ += separationZ * (FIXED_ONE / 2) / separation;
              }
#else
              die->velocityX += (separationX << 15) / separation;
              die->velocityY += (separationY << 15) / separation;
              die->velocityZ += (separationZ << 15) / separation;
#endif
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
          die->velocityY = -die->velocityY * 48 / 64;
          if ((Random16() & 3) == 0) {
            s32 speed;
            s32 angle;
            s32 speedX = die->velocityX >> 8;
            s32 speedZ = die->velocityZ >> 8;

            speed = Iwram_Sqrt(speedX * speedX + speedZ * speedZ) * 56 / 64;
            angle = Random16() & 0xffff;
            die->velocityX = Trig_Sin(angle) * speed >> 8;
            die->velocityZ = Trig_Cos(angle) * speed >> 8;
          } else {
            die->velocityX = die->velocityX * 56 / 64;
            die->velocityZ = die->velocityZ * 56 / 64;
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
        die->velocityX = die->velocityX * 63 / 64;
        die->velocityY = die->velocityY * 63 / 64;
        die->velocityZ = die->velocityZ * 63 / 64;
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
      if (stoppedDice == 4) {
        state = SUPER_LUCKY_DICE_RESULT;
        timer = 0;
        total = 0;
        for (index = 0; index != 4; index++)
          total = total + numbers[index] + 1;
      }
    } else if (state == SUPER_LUCKY_DICE_RESULT) {
      timer++;
      if (timer == 24)
        Sound_PlayFar(SE_JUMP);
      if (timer == 70)
        Sound_PlayFar(SE_CURSE);
      if (timer == 40) {
        if ((u32)resultKind > DICE_RESULT_WIN)
          Sound_PlayFar(MUS_BOULDER_CHASE);
        else if (jingle != -1)
          Sound_PlayFar(jingle);
        jingle = -1;
      }
      if (timer == 1) {
        paletteRequest = DICE_PICTURES_RESULT;
        resultKind = DICE_RESULT_LOSE;
        jingle = -1;
        if (round == 0) {
          resultKind = SuperLuckyDice_ScoreDice(numbers);
          switch (resultKind) {
          case DICE_RESULT_PERFECT:
            jingle = MUS_JINGLE_LARGE;
            multiplier = 8;
            break;
          case DICE_RESULT_ONE_PAIR:
            jingle = MUS_JINGLE_SMALL;
            multiplier = 1;
            break;
          case DICE_RESULT_TWO_PAIR:
            jingle = MUS_JINGLE_MEDIUM;
            multiplier = 3;
            break;
          case DICE_RESULT_TRIPLE:
            jingle = MUS_JINGLE_SMALL;
            multiplier = 2;
            break;
          }
        } else if (lower == 0) {
          if (total > target)
            resultKind = DICE_RESULT_WIN;
        } else if (total < target) {
          resultKind = DICE_RESULT_WIN;
        }
        if (resultKind == DICE_RESULT_WIN) {
          if (round <= 3)
            jingle = MUS_JINGLE_SMALL;
          if (round == 4)
            jingle = MUS_JINGLE_MEDIUM;
          if (round == 5)
            jingle = MUS_JINGLE_LARGE;
        }
      }
      if (resultKind == DICE_RESULT_LOSE ? timer == 90 : timer == 68) {
        if (resultKind != DICE_RESULT_LOSE) {
          if (round == 0)
            bet *= multiplier;
          else
            bet <<= 1;
          Text_DrawNumberInWindowFar(bet, DICE_COIN_DIGITS,
                                     (s32)sprites->betWindow, BET_TEXT_X, 5);
          multiplier <<= 1;
          if (round == 5) {
            target = 14;
            round = 0;
            winnings = bet;
            state = SUPER_LUCKY_DICE_PAYOUT;
          } else {
            state = SUPER_LUCKY_DICE_DOUBLE_UP;
            paletteRequest = DICE_PICTURES_DOUBLE_UP;
            cashOut = 0;
            window = Window_CreateFar(4, 16, 20, 4, 6);
            sprites->helpWindow = window;
            Text_DrawMessageAsTilesFar((s32)&MsgDiceGameDoubleUp, window, 0, 0);
          }
          timer = 0;
          slide = 0;
        } else {
          target = 14;
          round = 0;
          bet = Party_GetAverageLevelFar() * DICE_BET_PER_LEVEL;
          Text_DrawNumberInWindowFar(bet, DICE_COIN_DIGITS,
                                     (s32)sprites->betWindow, BET_TEXT_X, 5);
          multiplier = 0;
          Sound_PlayFar(MUS_SUPER_LUCKY_DICE);
          state = SUPER_LUCKY_DICE_THROW;
          window = Window_CreateFar(HELP_WINDOW_X, 16, HELP_WINDOW_WIDTH, 4, 6);
          sprites->helpWindow = window;
          Text_DrawMessageAsTilesFar((s32)&MsgDiceGameThrow, window, 0, 0);
          Text_DrawMessageAsTilesFar((s32)&MsgDiceGameThrow + 1,
                                     sprites->helpWindow, 0, 8);
          slide = 0;
          timer = 0;
          paletteRequest = DICE_PICTURES_HIGHER_LOWER;
        }
      }
    } else if (state == SUPER_LUCKY_DICE_PAYOUT) {
      if (++timer == 20) {
        if (winnings <= 9)
          digits = 1;
        else if (winnings <= 99)
          digits = 2;
        else if (winnings < 1000)
          digits = 3;
        else if (winnings <= 9999)
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
          window = Window_CreateFar(HELP_WINDOW_X, 16, HELP_WINDOW_WIDTH, 4, 6);
          sprites->helpWindow = window;
          Text_DrawMessageAsTilesFar((s32)&MsgDiceGameThrow, window, 0, 0);
          Text_DrawMessageAsTilesFar((s32)&MsgDiceGameThrow + 1,
                                     sprites->helpWindow, 0, 8);
          slide = 0;
          timer = 0;
          paletteRequest = DICE_PICTURES_HIGHER_LOWER;
          bet = Party_GetAverageLevelFar() * DICE_BET_PER_LEVEL;
          Text_DrawNumberInWindowFar(bet, DICE_COIN_DIGITS,
                                     (s32)sprites->betWindow, BET_TEXT_X, 5);
          if (winnings > 0) {
            Sound_PlayFar(SE_COIN_PAYOUT);
            Party_AdjustCoinsFar(winnings);
            Text_DrawNumberInWindowFar(gMainRoutineWork.coins, DICE_COIN_DIGITS,
                                       (s32)sprites->coinsWindow, 0, state);
          }
        } else if (winnings > 0) {
          if (--winnings == 0)
            Sound_PlayFar(SE_COIN_PAYOUT);
          Party_AdjustCoinsFar(1);
          Text_DrawNumberInWindowFar(gMainRoutineWork.coins, DICE_COIN_DIGITS,
                                     (s32)sprites->coinsWindow, 0, 0);
          bet--;
          Text_DrawNumberInWindowFar(bet, DICE_COIN_DIGITS,
                                     (s32)sprites->betWindow, BET_TEXT_X, 5);
        }
      }
    } else if (state == SUPER_LUCKY_DICE_DOUBLE_UP) {
      if (slide == 4)
        Sound_PlayFar(SE_NOTE_HIGH);
      if (slide <= 7) {
        slide++;
      } else if (timer > 0) {
        if (++timer == 16) {
          Window_CloseFar(sprites->helpWindow, 1);
          if (cashOut == 0) {
            target = total;
            round++;
            state = SUPER_LUCKY_DICE_GUESS;
            window = Window_CreateFar(RESULT_WINDOW_X, 16, RESULT_WINDOW_WIDTH,
                                      4, 6);
            sprites->helpWindow = window;
            if (total == 24) {
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
            paletteRequest = DICE_PICTURES_HIGHER_LOWER;
          } else {
            target = 14;
            round = 0;
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
            (u16 *)&OBJ_PLTT_COLOR(DICE_PALETTE_RELOADED_C, 0), 16,
            timer * (TRIG_TURN / 16));
        SuperLuckyDice_ScaleRgb555Buffer(
            spritePalette + DICE_PALETTE_RELOADED_D * PALETTE_COLORS,
            (u16 *)&OBJ_PLTT_COLOR(DICE_PALETTE_RELOADED_D, 0), FIXED_ONE / 2,
            16);
      } else {
        SuperLuckyDice_BrightenColors(
            spritePalette + DICE_PALETTE_RELOADED_D * PALETTE_COLORS,
            (u16 *)&OBJ_PLTT_COLOR(DICE_PALETTE_RELOADED_D, 0), 16,
            timer * (TRIG_TURN / 16));
        SuperLuckyDice_ScaleRgb555Buffer(
            spritePalette + DICE_PALETTE_RELOADED_C * PALETTE_COLORS,
            (u16 *)&OBJ_PLTT_COLOR(DICE_PALETTE_RELOADED_C, 0), FIXED_ONE / 2,
            16);
      }
    }

    // Load the pictures the state asked for.
    if (paletteRequest != DICE_PICTURES_NONE) {
      if (paletteRequest == DICE_PICTURES_HIGHER_LOWER) {
        resourceAddress =
            Resource_GetTableEntry((s32)&ResourceId_SlotsHigherBanner);
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
        resourceAddress =
            Resource_GetTableEntry((s32)&ResourceId_SlotsLowerBanner);
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
        resourceAddress =
            Resource_GetTableEntry((s32)&ResourceId_SlotsPinkDigits);
        {
          u16 *src;
          u16 *dst;
          s32 red;
          s32 blue;

          dst = spritePalette + DICE_PALETTE_RELOADED_C * PALETTE_COLORS;
          src = (u16 *)resourceAddress;
          for (index = 0; index != 16; index++) {
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
      } else if (paletteRequest == DICE_PICTURES_RESULT) {
        switch (resultKind) {
        case DICE_RESULT_LOSE:
          resourceAddress =
              Resource_GetTableEntry((s32)&ResourceId_SlotsYouLoseBanner);
          break;
        case DICE_RESULT_WIN:
          resourceAddress =
              Resource_GetTableEntry((s32)&ResourceId_SlotsYouWinBanner);
          break;
        case DICE_RESULT_ONE_PAIR:
          resourceAddress =
              Resource_GetTableEntry((s32)&ResourceId_SlotsOnePairBanner);
          break;
        case DICE_RESULT_TWO_PAIR:
          resourceAddress =
              Resource_GetTableEntry((s32)&ResourceId_SlotsTwoPairBanner);
          break;
        case DICE_RESULT_TRIPLE:
          resourceAddress =
              Resource_GetTableEntry((s32)&ResourceId_SlotsTripleBanner);
          break;
        case DICE_RESULT_PERFECT:
          resourceAddress =
              Resource_GetTableEntry((s32)&ResourceId_SlotsPerfectBanner);
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
        resourceAddress =
            Resource_GetTableEntry((s32)&ResourceId_SlotsGreenDigits);
        Dma_Set(resourceAddress,
                (void *)&OBJ_PLTT_COLOR(DICE_PALETTE_RELOADED_A, 0),
                DMA_COPY_WORDS(8), REG_DMA3);
        resourceAddress += PALETTE_BYTES;
        Decompress_TaggedLz(resourceAddress, cellBuffer);
        Dma_Set(cellBuffer,
                (void *)(OBJ_VRAM0 + GREEN_DIGIT_TILES * TILE_SIZE_4BPP),
                DMA_COPY_WORDS(1280), REG_DMA3);
      } else if (paletteRequest == DICE_PICTURES_DOUBLE_UP) {
        resourceAddress =
            Resource_GetTableEntry((s32)&ResourceId_SlotsDoubleUpBanner);
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
            Resource_GetTableEntry((s32)&ResourceId_SlotsCashOutBanner);
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
      paletteRequest = DICE_PICTURES_NONE;
    }

    // The double-up rounds won, the final-game or round lamps, and the
    // multiplier.
    if (round > 0) {
      s32 multiplierDigits;

      for (index = 0; index != 4; index++) {
        sprites->oam[spriteCount].attr01 =
            OBJ_POSITION(512 + index * 16, 2) | OBJ_SIZE_16X16;
        sprites->oam[spriteCount].attr2 =
            (SMALL_DOUBLE_UP_TILES + index * 4) |
            OBJ_PALETTE(DICE_PALETTE_SMALL_DOUBLE_UP);
        spriteCount++;
      }
      if (round == 5) {
        for (index = 0; index != 5; index++) {
          sprites->oam[spriteCount].attr01 =
              OBJ_POSITION(584 + index * 16, 2) | OBJ_SIZE_16X16;
          sprites->oam[spriteCount].attr2 =
              (FINAL_GAME_TILES + index * 4) |
              OBJ_PALETTE(DICE_PALETTE_FINAL_GAME);
          spriteCount++;
        }
      } else {
        for (index = 0; index != 3; index++) {
          sprites->oam[spriteCount].attr01 =
              OBJ_POSITION(ROUND_BANNER_X + index * 16, 2) | OBJ_SIZE_16X16;
          sprites->oam[spriteCount].attr2 =
              (GAME_TILES + index * 4) | OBJ_PALETTE(DICE_PALETTE_FINAL_GAME);
          spriteCount++;
        }
        sprites->oam[spriteCount].attr01 = ROUND_DIGIT_WORD;
        sprites->oam[spriteCount].attr2 =
            (round * 8 + ORANGE_DIGIT_TILES) |
            OBJ_PALETTE(DICE_PALETTE_ORANGE_DIGITS);
        spriteCount++;
#if LANGUAGE == LANGUAGE_SPANISH
        if (round == 1) {
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
            (OR_TILES + index * 4) | OBJ_PALETTE(DICE_PALETTE_TIMES_SIGN);
        spriteCount++;
      }
      if (multiplier <= 9)
        multiplierDigits = 1;
      else if (multiplier <= 99)
        multiplierDigits = 2;
      else if (multiplier < 1000)
        multiplierDigits = 3;
      else if (multiplier <= 9999)
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

    // The higher and lower choices, with the target to beat.
    if (state == SUPER_LUCKY_DICE_GUESS) {
      s32 x;
      s32 wobble;
      s32 left;

      x = (timer > 0 && lower == 1) ? timer * 12 + 64 : 64;
      wobble = lower == 0 ? Trig_Sin((frame << 10) & 0x7fff) >> 14 : 0;
      left = x + slide * 15;
      if (target <= 23)
        spriteCount =
            SuperLuckyDice_DrawNumber(spriteCount, left - 110, wobble + 60,
                                      target + 1, DICE_PALETTE_RELOADED_C, 2);
      x = (timer > 0 && lower == 0) ? timer * 12 + 64 : 64;
      wobble = lower == 1 ? Trig_Sin((frame << 10) & 0x7fff) >> 14 : 0;
      left = x + slide * 15;
      if (target > 4)
        spriteCount =
            SuperLuckyDice_DrawNumber(spriteCount, left - 110, 94 - wobble,
                                      target - 1, DICE_PALETTE_RELOADED_D, 2);
      for (index = 0; index != 3; index++) {
        s32 column;

        column = 64;
        if (timer > 0 && lower == 1)
          column = timer * 12 + 64;
        column += slide * 15;
        column -= 120;
        sprites->oam[spriteCount].attr01 =
            OBJ_POSITION(column + index * 32 + 512, 52) | OBJ_SIZE_32X32;
        sprites->oam[spriteCount].attr2 =
            OBJ_PALETTE(DICE_PALETTE_RELOADED_A) | (HIGHER_TILES + index * 16);
        spriteCount++;
        column = 64;
        if (timer > 0 && lower == 0)
          column = timer * 12 + 64;
        column += slide * 15;
        column -= 120;
        sprites->oam[spriteCount].attr01 =
            OBJ_POSITION(column + index * 32 + 512, 92) | OBJ_SIZE_32X32;
        sprites->oam[spriteCount].attr2 =
            (LOWER_TILES + index * 16) | OBJ_PALETTE(DICE_PALETTE_RELOADED_B);
        spriteCount++;
      }
    }

    // Double up or cash out.
    if (state == SUPER_LUCKY_DICE_DOUBLE_UP) {
      for (index = 0; index != 5; index++) {
        s32 column;

        column = 64;
        if (timer > 0 && cashOut == 1)
          column = timer * 12 + 64;
        column += slide * 15;
        column -= 120;
        sprites->oam[spriteCount].attr01 =
            OBJ_POSITION(column + index * 32 + 512, 52) | OBJ_SIZE_32X32;
        sprites->oam[spriteCount].attr2 = (DOUBLE_UP_TILES + index * 16) |
                                          OBJ_PALETTE(DICE_PALETTE_RELOADED_C);
        spriteCount++;
        column = 64;
        if (timer > 0 && cashOut == 0)
          column = timer * 12 + 64;
        column += slide * 15;
        column -= 120;
        sprites->oam[spriteCount].attr01 =
            OBJ_POSITION(column + index * 32 + 512, 84) | OBJ_SIZE_32X32;
        sprites->oam[spriteCount].attr2 = (CASH_OUT_TILES + index * 16) |
                                          OBJ_PALETTE(DICE_PALETTE_RELOADED_D);
        spriteCount++;
      }
    }

    // The total and the result's banner.
    if (state == SUPER_LUCKY_DICE_RESULT ||
        (state == SUPER_LUCKY_DICE_DOUBLE_UP &&
         resultKind != DICE_RESULT_LOSE)) {
      s32 x;
      s32 y;
      s32 angle;
      s32 doubled;
      s32 wave;

      x = (timer << 4) - 376;
      y = 48;
      if (x > 100)
        x = 100;
      if (state == SUPER_LUCKY_DICE_DOUBLE_UP) {
        angle = slide * (TRIG_TURN / 16);
        angle += TRIG_TURN / 16;
        x = FIXEDtoINT(Trig_Cos(angle) * 40) - (timer << 4) + 56;
        y = 48 - FIXEDtoINT(Trig_Sin(angle) * 48);
      } else if (resultKind != DICE_RESULT_LOSE) {
        if (timer > 67) {
          s32 elapsed = timer - 68;

          angle = elapsed * 1489;
          x = FIXEDtoINT(Trig_Cos(angle) * 48) + 52;
          y = 48 - FIXEDtoINT(Trig_Sin(angle) * 48);
        }
      } else if (timer > 67) {
        doubled = timer * 2;
        wave = doubled - 128;
        x = FIXEDtoINT(Trig_Sin(timer * (TRIG_TURN / 16) - TRIG_TURN * 4) *
                       wave) +
            100;
        doubled += timer;
        doubled *= 2;
        y = doubled - 360;
      }
      if (x >= -31)
        spriteCount = SuperLuckyDice_DrawLargeNumber(
            spriteCount, x, y, total, DICE_PALETTE_RELOADED_A, 2);
      if (timer > 30 || (state == SUPER_LUCKY_DICE_DOUBLE_UP &&
                         resultKind != DICE_RESULT_LOSE)) {
        s32 bannerX;
        s32 bannerY;

        bannerY = 64;
        bannerX = timer > 37 ? 60 : (timer << 4) - 620;
        if (state == SUPER_LUCKY_DICE_DOUBLE_UP) {
          bannerY = 64;
          bannerX = slide * 24 + 60;
        } else if (timer > 67) {
          doubled = timer * 2;
          wave = doubled - 136;
          bannerX = FIXEDtoINT(Trig_Sin(timer * (TRIG_TURN / 16) -
                                        TRIG_TURN * 17 / 4) *
                               wave) +
                    60;
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
            (RESULT_BANNER_TILES + 64) | OBJ_PALETTE(DICE_PALETTE_RELOADED_B);
        spriteCount++;
      }
    }

    // The dice, back to front.
    for (index = 0; index != 4; index++)
      drawOrder[index] = index;
    for (index = 1; index != 4; index++) {
      s32 sortedDie;

      sortedDie = drawOrder[index];
      for (otherIndex = index - 1;
           otherIndex >= 0 && dice->particles[drawOrder[otherIndex]].z >
                                  dice->particles[sortedDie].z;
           otherIndex--)
        drawOrder[otherIndex + 1] = drawOrder[otherIndex];
      drawOrder[otherIndex + 1] = sortedDie;
    }
    for (index = 0; index != 4; index++) {
      struct DiceParticle *die;
      s32 x;
      s32 y;

      die = &dice->particles[drawOrder[index]];
      Render_ProjectPoint(die, screen);
      if (screen[2] < DICE_NEAREST_DEPTH)
        screen[2] = DICE_NEAREST_DEPTH;
      if (screen[2] > DICE_FARTHEST_DEPTH)
        screen[2] = DICE_FARTHEST_DEPTH;
      spriteDepth[drawOrder[index]] = screen[2] - DICE_NEAREST_DEPTH;
      x = screen[0] - 0xc00 / (spriteDepth[drawOrder[index]] / 4 + 64);
      y = screen[1] - 0x800 / (spriteDepth[drawOrder[index]] / 4 + 64);
      if (y + 92 > -16) {
        sprites->oam[spriteCount].attr01 =
            (OBJ_AFFINE_MATRIX(drawOrder[index]) | OBJ_X((x + 632) & 0x1ff) |
             ((y + 348) & 255)) |
            OBJ_SIZE_16X16 | OBJ_256_COLORS | OBJ_DOUBLE_SIZE | OBJ_AFFINE;
        sprites->oam[spriteCount].attr2 =
            (gSuperLuckyDice_FaceTiles[die->variant / DICE_ROLL_PER_TILE]
             << 3) |
            OBJ_PRIORITY(1);
        spriteCount++;
      }
    }
    {
      struct DiceParticle *die;

      for (index = 0; index != 4; index++) {
        s32 x;
        s32 y;
        s32 half;

        die = &dice->particles[index];

        groundPoint[0] = die->x;
        groundPoint[1] = 0;
        groundPoint[2] = die->z;
        Render_ProjectPoint(groundPoint, screen);
        if (screen[2] < DICE_NEAREST_DEPTH)
          screen[2] = DICE_NEAREST_DEPTH;
        if (screen[2] > DICE_FARTHEST_DEPTH)
          screen[2] = DICE_FARTHEST_DEPTH;
        spriteDepth[4 + index] = screen[2] - DICE_NEAREST_DEPTH;
        spriteDepth[4 + index] += (s16)FIXEDtoINT(die->y);
        half = spriteDepth[4 + index] / 2 + 128;
        x = screen[0] - 0x1800 / half;
        y = screen[1] - 0xc00 / half;
        if (y + 92 > -16) {
          sprites->oam[spriteCount].attr01 =
              (OBJ_AFFINE_MATRIX(index + 4) | OBJ_X((x + 632) & 0x1ff)) |
              ((y + 348) & 255) | OBJ_SIZE_16X16 | OBJ_256_COLORS |
              OBJ_DOUBLE_SIZE | OBJ_AFFINE;
          sprites->oam[spriteCount].attr2 = DICE_SHADOW_TILE | OBJ_PRIORITY(2);
          spriteCount++;
        }
      }
    }
    for (; spriteCount != OAM_SIZE / sizeof(struct OamWords); spriteCount++) {
      sprites->oam[spriteCount].attr01 =
          OBJ_X(DISPLAY_WIDTH) | OBJ_SIZE_16X16 | OBJ_256_COLORS;
      sprites->oam[spriteCount].attr2 = 0;
    }
    for (index = 0; index != 8; index++) {
      s16 scaleX;
      s16 scaleY;
      s32 zero = 0;

      scaleX = spriteDepth[index] / 4 + 128;
      affineA = scaleX;
      affineB = zero;
      affineC = zero;
      scaleY = spriteDepth[index] / 4 + 128;
      affineD = scaleY;
      sprites->oam[index * 4 + 0].attr2 |= affineA << 16;
      sprites->oam[index * 4 + 1].attr2 |= affineB << 16;
      sprites->oam[index * 4 + 2].attr2 |= affineC << 16;
      sprites->oam[index * 4 + 3].attr2 |= affineD << 16;
    }
    Dma_Set(sprites->oam, (void *)OAM, DMA_COPY_WORDS(256), REG_DMA3);
    wait(1);
    frame++;
  }

  for (index = 0; index != DICE_FADE_FRAMES + 1; index++) {
    SuperLuckyDice_ScaleRgb555Buffer(spritePalette, (u16 *)OBJ_PLTT,
                                     0x10000 - (index << 12), 256);
    SuperLuckyDice_ScaleRgb555Buffer(bgPalette, (u16 *)BG_PLTT,
                                     0x10000 - (index << 12), 256);
    wait(1);
  }
  HeapSlot_Release(HEAP_SLOT_CAMERA);
  HeapSlot_Release(HEAP_SLOT_CANVAS_MAP);
  HeapSlot_Release(HEAP_SLOT_BATTLE_CANVAS);
  HeapSlot_Release(HEAP_SLOT_BATTLE_EFFECT);
  HeapSlot_Release(HEAP_SLOT_BATTLE_SHEET);
}
