/// @file
/// The Lucky Dice minigame, from its start to its tables, in the order it has
/// in the ROM.

#include "battle/battle.h"
#include "sounds.h"
#include "debug/debug.h"
#include "field/field.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "system/math_table.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "types.h"
#include "field/field_event.h"
#include "system/io_reg.h"
#include "system/ram_buffer.h"
#include "system/iwram_call.h"
#include "system/point.h"
#include "battle/battle_camera.h"
#include "macros.h"
#include "system/scene.h"
#include "edition.h"
#include "field/call.h"
#include "system/dma.h"
#include "system/input.h"
#include "system/main_routine_work.h"
#include "system/bg_scroll.h"
#include "field/actor.h"
#include "fixed_math.h"
#include "system/frame.h"
#include "gamepak/far_call/game.h"
#include "gamepak/far_call/window.h"
#include "system/heap_release.h"
#include "field/field_sequence.h"
#include "graphics/resource.h"
#include "resource_ids.h"
#include "system/task.h"
#include "text/msg_ids.h"
#include "system/matrix.h"

// The Lucky Dice minigame's tables, after LuckyDice_Run in both games.

// A table of signs nothing reads.
const s32 gLuckyDice_UnusedSigns[] = {-1, 1, -1, 1, -1, -1, 1, 1};

/// How many sprites each kind of result's lettering takes: 32 pixels each,
/// for the kinds of lucky_dice.c's resultKind, 0 all four numbers alike, 1
/// three alike, 2 two pairs and 3 one pair.
const u8 gLuckyDice_LabelCounts[] = {5, 4, 3, 3};

/// The first tile of each kind of result's lettering, in 16-pixel steps from
/// DICE_NUMBER_TILES.
const u8 gLuckyDice_LabelTiles[] = {
    28,  48,  64, 76,
#if LANGUAGE != LANGUAGE_JAPANESE
    28,  248, 64,
    248, // never read
#endif
};

/// The die face tile at each step of its roll: one for each 1024 of the roll
/// angle (variant), three to a face.
const u8 gLuckyDice_FaceTiles[] = {0, 12, 16, 1, 14, 8, 2, 15, 10,
                                   3, 9,  13, 4, 7,  6, 5, 17, 11};

/// The board's rows by depth: a die at rest is in the first row whose value its
/// z (16.16 pixels) exceeds, -32767 taking the rest.
const s16 gLuckyDice_FaceHeights[] = {147, 80, 15, -42, -95, -32767};

/// Starts the Lucky Dice minigame: leaves only the sprites on screen, starts
/// the music and plays until the player quits. Returns 0.
s32 LuckyDice_Start(void) {
  REG_DISPCNT = DISPCNT_OBJ_1D_MAP;
  Sound_PlayFar(MUS_ALHAFRA);
  LuckyDice_Run();
  return 0;
}

/// Puts the 3D camera back at its starting view: at the origin, unturned,
/// pitched to 152 and 510 units away, following nothing.
void Camera_ResetViewDefaults(void) {
  struct BattleCamera *camera = Ram_HeapSlots->cameraWork;
  s32 offset[3];
  u32 nearPlane;

  camera->pitch = 152 << 8;
  camera->distance = 255 << 17;
  SET_VECTOR(camera->focus, 0, 0, 0);
  camera->yaw = 0;
  camera->followPosition = 0;
  gProjection.centerX = 0;
  gProjection.centerY = 0;
  camera->followEye = 0;

  // The eye sits `distance` behind the target along the camera's view.
  Matrix_Initialize();
  Matrix_TranslateVector(&camera->focus[0]);
  Matrix_RotateY((s16)camera->yaw);
  Matrix_RotateX((s16)camera->pitch);

  SET_VECTOR(offset, 0, 0, camera->distance);
  Iwram_TransformVector(offset, camera->eye);

  nearPlane = Iwram_RatioMulQ14(toFIXED(250), 192 << 8);
  Projection_Set(toFIXED(250), nearPlane, 0x7fff0000);
}

/// Unused: multiplies two 8.8 fixed-point values, rounding toward zero.
s32 Math_MulQ8(s32 a, s32 b) {
  s16 left = a;
  s16 right = b;
  s32 product;
  s32 adjusted;
  s32 multiplier;

  multiplier = right;
  product = left;
  product *= multiplier;
  if (product >= 0) {
    adjusted = product;
  } else {
    adjusted = product + 255;
  }
  return (s16)(adjusted >> 8);
}

/// Unused: divides two 8.8 fixed-point values.
UNUSED s16 Math_ScaleByRatio(s16 numerator, s16 denominator) {
  return (numerator * 256) / denominator;
}

/// Unused: the reciprocal of an 8.8 fixed-point value.
UNUSED s16 FixedPoint_Reciprocal(s16 value) { return FIXED_ONE / value; }

/// Copies count RGB555 colours from source to destination with each channel
/// multiplied by scale (16.16: 0 is black, FIXED_ONE unchanged), for fades.
s32 LuckyDice_ScaleRgb555Buffer(u16 *source, u16 *destination, s32 scale,
                                s32 count) {
  s32 remaining;
  u32 pixel;
  u32 red;
  u32 green;
  u32 blue;

  if (count > 0) {
    u32 redMask = RGB(31, 0, 0);
    u32 greenMask = RGB(0, 31, 0);
    u32 blueMask = RGB(0, 0, 31);

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

// Lucky Dice's layout, fitted to each language's text: the coins window at the
// top and its label, the help window (Throw / Quit) and the winnings window at
// the bottom with their text, in tiles for windows and pixels within them, and
// the centre of the result lettering in sprite x, which wraps at 512.
#if LANGUAGE == LANGUAGE_JAPANESE
#define DICE_COINS_WINDOW_X 18
#define DICE_COINS_WINDOW_WIDTH 11
#define DICE_COINS_LABEL_X 48
#elif LANGUAGE == LANGUAGE_GERMAN || LANGUAGE == LANGUAGE_FRENCH
#define DICE_COINS_WINDOW_X 16
#define DICE_COINS_WINDOW_WIDTH 14
#define DICE_COINS_LABEL_X 53
#elif LANGUAGE == LANGUAGE_SPANISH || LANGUAGE == LANGUAGE_ITALIAN
#define DICE_COINS_WINDOW_X 17
#define DICE_COINS_WINDOW_WIDTH 13
#define DICE_COINS_LABEL_X 56
#else
#define DICE_COINS_WINDOW_X 18
#define DICE_COINS_WINDOW_WIDTH 12
#define DICE_COINS_LABEL_X 48
#endif

#if LANGUAGE == LANGUAGE_JAPANESE
#define DICE_HELP_WINDOW_X 23
#define DICE_HELP_WINDOW_WIDTH 7
#elif LANGUAGE == LANGUAGE_GERMAN || LANGUAGE == LANGUAGE_SPANISH
#define DICE_HELP_WINDOW_X 20
#define DICE_HELP_WINDOW_WIDTH 10
#elif LANGUAGE == LANGUAGE_FRENCH
#define DICE_HELP_WINDOW_X 21
#define DICE_HELP_WINDOW_WIDTH 9
#else
#define DICE_HELP_WINDOW_X 22
#define DICE_HELP_WINDOW_WIDTH 8
#endif

// The help window reopened after a payout; in Spanish it is a tile narrower
// than the one Lucky Dice opens with, for the same two lines.
#if LANGUAGE == LANGUAGE_SPANISH
#define DICE_REOPENED_HELP_WINDOW_X 21
#define DICE_REOPENED_HELP_WINDOW_WIDTH 9
#else
#define DICE_REOPENED_HELP_WINDOW_X DICE_HELP_WINDOW_X
#define DICE_REOPENED_HELP_WINDOW_WIDTH DICE_HELP_WINDOW_WIDTH
#endif

// The winnings window opens at this x less half its digits and is this width
// plus its digits; after the winnings label the number stands at
// DICE_WINNINGS_NUMBER_X, and the coins label at DICE_WINNINGS_COINS_X past
// the number's digits.
#if LANGUAGE == LANGUAGE_JAPANESE
#define DICE_WINNINGS_WINDOW_X 10
#define DICE_WINNINGS_WINDOW_WIDTH 10
#define DICE_WINNINGS_COINS_X 40
#define DICE_WINNINGS_NUMBER_X 40
#elif LANGUAGE == LANGUAGE_GERMAN
#define DICE_WINNINGS_WINDOW_X 7
#define DICE_WINNINGS_WINDOW_WIDTH 14
#define DICE_WINNINGS_COINS_X 53
#define DICE_WINNINGS_NUMBER_X 53
#elif LANGUAGE == LANGUAGE_FRENCH
#define DICE_WINNINGS_WINDOW_X 9
#define DICE_WINNINGS_WINDOW_WIDTH 13
#define DICE_WINNINGS_COINS_X 53
#define DICE_WINNINGS_NUMBER_X 53
#elif LANGUAGE == LANGUAGE_SPANISH || LANGUAGE == LANGUAGE_ITALIAN
#define DICE_WINNINGS_WINDOW_X 9
#define DICE_WINNINGS_WINDOW_WIDTH 13
#define DICE_WINNINGS_COINS_X 64
#define DICE_WINNINGS_NUMBER_X 56
#else
#define DICE_WINNINGS_WINDOW_X 8
#define DICE_WINNINGS_WINDOW_WIDTH 13
#define DICE_WINNINGS_COINS_X 56
#define DICE_WINNINGS_NUMBER_X 56
#endif

#if LANGUAGE == LANGUAGE_JAPANESE
#define DICE_RESULT_LABEL_CENTER_X 640
#else
#define DICE_RESULT_LABEL_CENTER_X 632
#endif

#define DICE_COUNT 2
#define DICE_FACE_COUNT 6
#define DICE_RESULT_COUNT 4
#define DICE_BET_PER_LEVEL 10
/// The result: one number every 5 frames, then 20 frames in the lettering,
/// the winnings, and A to go on; the payout closes 15 frames after it starts.
#define DICE_NUMBER_REVEAL_FRAMES 5
#define DICE_RESULT_FRAMES 20
#define DICE_PAYOUT_FRAMES 15
/// The coins are drawn with six digits.
#define DICE_COIN_DIGITS 6

/// The board fades in and out over 16 frames, a sixteenth of full brightness a
/// frame.
#define DICE_FADE_FRAMES 16

/// The depths from the camera a die is drawn between: nearer or further is
/// drawn as at the limit, and the nearer, the larger.
#define DICE_NEAREST_DEPTH 350
#define DICE_FARTHEST_DEPTH 650

/// Two dice whose centres come closer than this push each other apart.
#define DICE_TOUCH_DISTANCE 50
/// Where each picture's sprite tiles start, in OAM's 32-byte steps (two to a
/// 256-colour tile): the rolling die's views from 0, its shadow, the big
/// numbers (the result's lettering counted on from them), the aiming hand,
/// the Perfect Bonus sign, its 3, 5 and times sign, and the lucky numbers'
/// small faces.
#define DICE_SHADOW_TILE 144
#define DICE_NUMBER_TILES 152
#define DICE_AIM_MARKER_TILE 344
#define DICE_BONUS_SIGN_TILE 856
#define DICE_THREE_TILE 920
#define DICE_FIVE_TILE 928
#define DICE_TIMES_TILE 936
#define DICE_LUCKY_NUMBER_TILES 944
/// Four alike pays five times the bet; four of a lucky number pays three or
/// five times that, the multipliers drawn beside the two lucky numbers.
#define DICE_PERFECT_PAYOUT 5
#define DICE_FIRST_LUCKY_MULTIPLIER 3
#define DICE_SECOND_LUCKY_MULTIPLIER 5
/// A win flashes a die (both for four alike) for this many frames.
#define DICE_FLASH_FRAMES 60
#define DICE_PERFECT_FLASH_FRAMES 120
/// A thrown die starts spinning at this rate, its roll angle a frame; each
/// bounce takes 20 off it.
#define DICE_THROW_SPIN 300
#define DICE_BOUNCE_SPIN_LOSS 20

/// A die's roll angle (its particle's variant) crosses a face in
/// DICE_TILES_PER_FACE tiles, and LuckyDice_Run reads the tile as angle /
/// DICE_ROLL_PER_TILE and the face as angle / DICE_ROLL_PER_FACE.
#define DICE_TILES_PER_FACE 3
#define DICE_ROLL_PER_TILE 1024
#define DICE_ROLL_PER_FACE (DICE_TILES_PER_FACE * DICE_ROLL_PER_TILE)
#define DICE_ROLL_TURN (DICE_FACE_COUNT * DICE_ROLL_PER_FACE) ///< six faces

/// What the Lucky Dice screen is doing.
enum LuckyDiceState {
  LUCKY_DICE_AIM,    ///< the dice wait to be thrown; Up and Down raise them
  LUCKY_DICE_ROLL,   ///< the dice fly, bounce and settle
  LUCKY_DICE_RESULT, ///< the four numbers and the result are shown
  LUCKY_DICE_PAYOUT  ///< the winnings move into the coins
};

/// Runs the Lucky Dice minigame until the player quits or cannot afford a
/// throw. Each throw costs ten coins per level of the party's average.
void LuckyDice_Run(void) {
  u16 bgPalette[256];
  s32 spriteDepth[DICE_RESULT_COUNT]; // the two dice, then their shadows
  // The two faces, then the two rows the dice stopped in.
  s32 numbers[DICE_RESULT_COUNT];
  s32 groundPoint[3];
  s32 screen[3];
  /* LEFTOVER(frame): an unused local the ROM's frame reserves and never
   * touches: of the 768 bytes it reserves (add sp, r5, r5 being -768), no
   * instruction addresses the 32 at sp+168 to sp+199, and The Broken Seal's
   * LuckyDice_Run declares the same unused[8]; as without it, 42 of 2055
   * instructions differ in tla-en. */
  UNUSED s32 unused[8];
  /* FAKEMATCH(volatile): four halfwords stored to sp+166..160 and each reloaded
   * with ldrh. */
  volatile u16 affineA;
  volatile u16 affineB;
  volatile u16 affineC;
  volatile u16 affineD;
  s32 spinRate[DICE_COUNT];
  s32 flashTimer[DICE_COUNT];   // set on a win; nothing here reads it
  s32 luckyNumbers[DICE_COUNT]; // the two numbers shown at the top, never equal
  s32 drawOrder[DICE_COUNT];    // the dice, back to front
  s32 matchedNumbers[2];
  u16 *spritePalette;
  struct DiceWork *diceWork;
  struct LuckyDiceSprites *sprites;
  struct BattleCamera *camera;
  s32 frame;
  s32 state;
  s32 bet;
  s32 resultTimer;
  s32 resultKind;
  s32 winnings;
  s32 payoutTimer;
  s32 digits;
  s32 spriteCount;
  struct Particle *rollingDie;
  struct Particle *otherDie;
  s32 resetDice; // put the dice back at the throwing line
  struct Window *window;
  s32 index;
  s32 otherIndex;
  s32 rollAngle; // a die's roll angle, in DICE_ROLL_PER_TILE steps
  s32 x;
  s32 y;
  s32 halfSize;
  s32 faceTile; // which of a face's three tiles the roll shows
  s32 sortedDie;
  s32 tileX;
  s32 tileY;
  s32 resourceAddress;
  s32 separationX;
  s32 separationY;
  s32 separationZ;
  s32 separation;
  s32 bounceSpeed;
  u32 bounceAngle;
  s32 scale;
  s32 pairCount;
  s16 affineScale;

  HeapSlot_Alloc(HEAP_SLOT_BATTLE_SHEET, 1550);
  spritePalette = HeapSlot_Alloc(HEAP_SLOT_BATTLE_CANVAS, 512);
  diceWork = HeapSlot_AllocEwram(HEAP_SLOT_BATTLE_EFFECT, 31612);
  sprites = HeapSlot_AllocEwram(HEAP_SLOT_CANVAS_MAP, 1556);
  camera = HeapSlot_AllocEwram(HEAP_SLOT_CAMERA, sizeof(*camera));
  Resource_ReservedNoOp((s32)&ResourceId_BattleEffectFarCalls);
  Camera_ResetViewDefaults();
  Task_ResetAll();
  gOamCopyEnabled = 0;

  index = 0;
  for (tileY = 0; tileY != DISPLAY_HEIGHT / 8; tileY++) {
    for (tileX = 0; tileX != 32; tileX++) {
      *(volatile u16 *)(BG_SCREEN_BLOCK(6) + index) =
          tileX + tileY * (DISPLAY_WIDTH / 8);
      index += 2;
    }
  }

  resourceAddress =
      (s32)Resource_GetTableEntry((s32)&ResourceId_LuckyDiceBoard);
  Dma_Set((void *)resourceAddress, (void *)BG_PLTT, DMA_COPY_WORDS(112),
          REG_DMA3);
  Dma_Set((void *)BG_PLTT, bgPalette, DMA_COPY_WORDS(128), REG_DMA3);
  resourceAddress += 112 * 4;
  // UB: Call2 passes integers to the decoder's two pointer parameters, as
  // The Broken Seal's LuckyDice_Run does; typed calls keep the cell buffer
  // in a register for the transfer after each decode (137 against 108).
#ifdef UBFIX
  Decompress_TaggedLz((const void *)resourceAddress, Ram_MapCellBuffer);
#else
  Call2((void (*)())Decompress_TaggedLz, resourceAddress,
        (s32)Ram_MapCellBuffer);
#endif
  Dma_Set(Ram_MapCellBuffer, (void *)BG_CHAR_BLOCK(1), DMA_COPY_WORDS(9600),
          REG_DMA3);
  resourceAddress =
      (s32)Resource_GetTableEntry((s32)&ResourceId_LuckyDiceSprites);
  Dma_Set((void *)resourceAddress, spritePalette, DMA_COPY_WORDS(128),
          REG_DMA3);
  Dma_Set((void *)resourceAddress, (void *)OBJ_PLTT, DMA_COPY_WORDS(128),
          REG_DMA3);
  resourceAddress += 128 * 4;
#ifdef UBFIX
  Decompress_TaggedLz((const void *)resourceAddress, Ram_MapCellBuffer);
#else
  Call2((void (*)())Decompress_TaggedLz, resourceAddress,
        (s32)Ram_MapCellBuffer);
#endif
  Dma_Set(Ram_MapCellBuffer, (void *)OBJ_VRAM0, DMA_COPY_WORDS(7936), REG_DMA3);
  LuckyDice_ScaleRgb555Buffer(spritePalette, (u16 *)OBJ_PLTT, 0, 256);
  LuckyDice_ScaleRgb555Buffer(bgPalette, (u16 *)BG_PLTT, 0, 256);

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

  // Default: set for real at the top of every frame.
  spriteCount = 0;
  state = LUCKY_DICE_AIM;
  resultTimer = 0;
  resultKind = -1;
  winnings = 0;
  payoutTimer = 0;
  for (index = 0; index != DICE_COUNT; index++) {
    diceWork->particles[index].x = ((Random16() & 127) - 64) << 16;
    diceWork->particles[index].y = (Random16() & 31) << 16;
    diceWork->particles[index].z = ((Random16() & 127) - 64) << 16;
    diceWork->particles[index].velocityX = ((Random16() & 127) - 64) << 14;
    diceWork->particles[index].velocityY = ((Random16() & 31) + 32) << 12;
    diceWork->particles[index].velocityZ = ((Random16() & 127) - 64) << 14;
    diceWork->particles[index].variant = 0;
    spinRate[index] = DICE_THROW_SPIN;
  }
  numbers[0] = numbers[1] = numbers[2] = numbers[3] = 0;
  flashTimer[0] = flashTimer[1] = 0;
  luckyNumbers[0] = Random16() % DICE_FACE_COUNT;
  do {
    luckyNumbers[1] = Random16() % DICE_FACE_COUNT;
  } while (luckyNumbers[0] == luckyNumbers[1]);
  diceWork->particles[0].x = toFIXED(40);
  diceWork->particles[0].y = toFIXED(96);
  diceWork->particles[0].z = -toFIXED(160);
  diceWork->particles[1].x = -toFIXED(40);
  diceWork->particles[1].y = toFIXED(96);
  diceWork->particles[1].z = -toFIXED(160);

  WindowSystem_InitFieldFar();
  window =
      Window_CreateFar(DICE_COINS_WINDOW_X, 0, DICE_COINS_WINDOW_WIDTH, 3, 6);
  sprites->coinsWindow = window;
  Text_DrawMessageAsTilesFar((s32)&MsgLuckyDiceCoins, window,
                             DICE_COINS_LABEL_X, 0);
  Text_DrawNumberInWindowFar(gMainRoutineWork.coins, DICE_COIN_DIGITS,
                             (s32)sprites->coinsWindow, 0, 0);
  window =
      Window_CreateFar(DICE_HELP_WINDOW_X, 16, DICE_HELP_WINDOW_WIDTH, 4, 6);
  sprites->helpWindow = window;
  // MsgLuckyDiceThrow and its Quit, ten and eleven after the coins label.
  Text_DrawMessageAsTilesFar((s32)&MsgLuckyDiceCoins + 10, window, 0, 0);
  Text_DrawMessageAsTilesFar((s32)&MsgLuckyDiceCoins + 11, sprites->helpWindow,
                             0, 8);

  frame = 0;
  for (;;) {
    resetDice = 0;
    if (frame <= DICE_FADE_FRAMES) {
      LuckyDice_ScaleRgb555Buffer(spritePalette, (u16 *)OBJ_PLTT,
                                  frame * (FIXED_ONE / DICE_FADE_FRAMES), 256);
      LuckyDice_ScaleRgb555Buffer(bgPalette, (u16 *)BG_PLTT,
                                  frame * (FIXED_ONE / DICE_FADE_FRAMES), 256);
    }
    Matrix_Initialize();
    Matrix_SetLookAt(camera->eye, camera->focus);

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
    for (index = 0; index != DICE_COUNT; index++) {
      sprites->oam[spriteCount].attr01 =
          OBJ_X(32) | (32 + index * 16) | OBJ_SIZE_16X16 | OBJ_256_COLORS;
      sprites->oam[spriteCount].attr2 =
          luckyNumbers[index] * 8 + DICE_LUCKY_NUMBER_TILES;
      spriteCount++;
    }

    if (state == LUCKY_DICE_RESULT || state == LUCKY_DICE_PAYOUT) {
      for (index = 0; index != DICE_RESULT_COUNT; index++) {
        if (resultTimer == index * DICE_NUMBER_REVEAL_FRAMES) {
          Sound_PlayFar(SE_LUCKY_DICE_NUMBER);
        }
        if (resultTimer >= index * DICE_NUMBER_REVEAL_FRAMES) {
          sprites->oam[spriteCount].attr01 =
              64 | OBJ_SIZE_32X32 | OBJ_256_COLORS | OBJ_X(56 + index * 32);
          sprites->oam[spriteCount].attr2 =
              numbers[index] * 32 + DICE_NUMBER_TILES;
          spriteCount++;
        }
      }
      if (resultTimer >= DICE_RESULT_FRAMES && resultKind >= 0) {
        for (index = 0; index != gLuckyDice_LabelCounts[resultKind]; index++) {
          s32 labelX;
          s32 labelY = 96;

          labelX = DICE_RESULT_LABEL_CENTER_X -
                   gLuckyDice_LabelCounts[resultKind] * 16 + index * 32;
          labelY = 96;
          sprites->oam[spriteCount].attr01 =
              OBJ_POSITION(labelX, labelY) | OBJ_SIZE_32X32 | OBJ_256_COLORS;
          sprites->oam[spriteCount].attr2 =
              gLuckyDice_LabelTiles[resultKind] * 8 + DICE_NUMBER_TILES +
              index * 32;
          spriteCount++;
        }
      }
    }

    if (state == LUCKY_DICE_RESULT) {
      if (resultTimer == DICE_RESULT_FRAMES && winnings > 0) {
        Party_AdjustCoinsFar(winnings);
        if (winnings <= 9) {
          digits = 1;
        } else if (winnings <= 99) {
          digits = 2;
        } else if (winnings <= 999) {
          digits = 3;
        } else if (winnings <= 9999) {
          digits = 4;
        } else {
          digits = 5;
        }
        window = Window_CreateFar(DICE_WINNINGS_WINDOW_X - digits / 2, 16,
                                  digits + DICE_WINNINGS_WINDOW_WIDTH, 3, 6);
        sprites->winningsWindow = window;
        Text_DrawMessageAsTilesFar((s32)&MsgLuckyDiceWinnings, window, 0, 0);
        Text_DrawMessageAsTilesFar((s32)&MsgLuckyDiceWinnings - 1,
                                   sprites->winningsWindow,
                                   digits * 8 + DICE_WINNINGS_COINS_X, 0);
        Text_DrawNumberInWindowFar(winnings, digits,
                                   (s32)sprites->winningsWindow,
                                   DICE_WINNINGS_NUMBER_X, 0);
      }
      if (payoutTimer > 0) {
        payoutTimer--;
      }
      if (resultTimer >= DICE_RESULT_FRAMES && (gInput.repeat & KEY_A)) {
        Sound_PlayFar(SE_MENU_CONFIRM);
        if (winnings > 0) {
          state = LUCKY_DICE_PAYOUT;
          payoutTimer = 0;
        } else {
          resetDice = 1;
          state = LUCKY_DICE_AIM;
        }
      }
      resultTimer++;
    } else if (state == LUCKY_DICE_PAYOUT) {
      if (payoutTimer == 0) {
        winnings = 0;
        Text_DrawNumberInWindowFar(0, digits, (s32)sprites->winningsWindow,
                                   DICE_WINNINGS_NUMBER_X, 0);
        Text_DrawNumberInWindowFar(gMainRoutineWork.coins, DICE_COIN_DIGITS,
                                   sprites->coinsWindow, 0, 0);
      }
      if (payoutTimer == DICE_PAYOUT_FRAMES) {
        state = LUCKY_DICE_AIM;
        Window_CloseFar(sprites->winningsWindow, 1);
        window = Window_CreateFar(DICE_REOPENED_HELP_WINDOW_X, 16,
                                  DICE_REOPENED_HELP_WINDOW_WIDTH, 4, 6);
        sprites->helpWindow = window;
        Text_DrawMessageAsTilesFar((s32)&MsgLuckyDiceCoins + 10, window, 0, 0);
        Text_DrawMessageAsTilesFar((s32)&MsgLuckyDiceCoins + 11,
                                   sprites->helpWindow, 0, 8);
        resetDice = 1;
      }
      payoutTimer++;
    } else if (state == LUCKY_DICE_AIM && frame > 17) {
      (void)gInput.repeat;
      (void)gInput.repeat;
      if (gInput.held & KEY_UP) {
        diceWork->particles[0].y += toFIXED(4);
        diceWork->particles[1].y += toFIXED(4);
      }
      if (gInput.held & KEY_DOWN) {
        diceWork->particles[0].y += -toFIXED(4);
        diceWork->particles[1].y += -toFIXED(4);
      }
      if (diceWork->particles[0].y < toFIXED(32)) {
        diceWork->particles[0].y = toFIXED(32);
      }
      if (diceWork->particles[0].y > toFIXED(160)) {
        diceWork->particles[0].y = toFIXED(160);
      }
      if (diceWork->particles[1].y < toFIXED(32)) {
        diceWork->particles[1].y = toFIXED(32);
      }
      if (diceWork->particles[1].y > toFIXED(160)) {
        diceWork->particles[1].y = toFIXED(160);
      }
      if (gMainRoutineWork.coins <
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
        state = LUCKY_DICE_ROLL;
        bet = Party_GetAverageLevelFar() * DICE_BET_PER_LEVEL;
        if ((u32)bet > gMainRoutineWork.coins) {
          bet = gMainRoutineWork.coins;
        }
        Party_AdjustCoinsFar(-bet);
        Text_DrawNumberInWindowFar(gMainRoutineWork.coins, DICE_COIN_DIGITS,
                                   sprites->coinsWindow, 0, 0);
        for (index = 0; index != DICE_COUNT; index++) {
          rollingDie = &diceWork->particles[index];
          if (index > 1) {
            rollingDie->z = -toFIXED(180);
            rollingDie->y = toFIXED(96);
          }
          rollingDie->velocityX = ((Random16() & 127) - 64) << 12;
          rollingDie->velocityY = ((Random16() & 15) + 48) << 13;
          rollingDie->velocityZ =
              (((Random16() & 63) + 140) << 12) + rollingDie->y / 6;
          rollingDie->variant = Random16() % DICE_ROLL_TURN;
          spinRate[index] = DICE_THROW_SPIN;
        }
      }
    }

    if (resetDice == 1) {
      diceWork->particles[0].x = toFIXED(40);
      diceWork->particles[0].y = toFIXED(96);
      diceWork->particles[0].z = -toFIXED(160);
      diceWork->particles[1].x = -toFIXED(40);
      diceWork->particles[1].y = toFIXED(96);
      diceWork->particles[1].z = -toFIXED(160);
      luckyNumbers[0] = Random16() % DICE_FACE_COUNT;
      do {
        luckyNumbers[1] = Random16() % DICE_FACE_COUNT;
      } while (luckyNumbers[0] == luckyNumbers[1]);
    }

    for (index = 0; index != DICE_COUNT; index++) {
      drawOrder[index] = index;
    }
    for (index = 1; index != DICE_COUNT; index++) {
      sortedDie = drawOrder[index];
      for (otherIndex = index - 1;
           otherIndex >= 0 && diceWork->particles[drawOrder[otherIndex]].z >
                                  diceWork->particles[sortedDie].z;
           otherIndex--) {
        drawOrder[otherIndex + 1] = drawOrder[otherIndex];
      }
      drawOrder[otherIndex + 1] = sortedDie;
    }

    for (index = 0; index != DICE_COUNT; index++) {
      s32 dieX;
      s32 dieY;

      rollingDie = &diceWork->particles[drawOrder[index]];
      Render_ProjectPoint(rollingDie, screen);
      if (screen[2] < DICE_NEAREST_DEPTH) {
        screen[2] = DICE_NEAREST_DEPTH;
      }
      if (screen[2] > DICE_FARTHEST_DEPTH) {
        screen[2] = DICE_FARTHEST_DEPTH;
      }
      spriteDepth[drawOrder[index]] = screen[2] - DICE_NEAREST_DEPTH;
      halfSize = 2048 / (spriteDepth[drawOrder[index]] / 2 + 128);
      dieX = screen[0] - halfSize;
      dieY = screen[1] - halfSize;
      if (dieY + 96 > -16) {
        sprites->oam[spriteCount].attr01 =
            (OBJ_AFFINE_MATRIX(drawOrder[index]) | OBJ_X((dieX + 632) & 0x1ff) |
             ((dieY + 352) & 255)) |
            OBJ_SIZE_16X16 | OBJ_256_COLORS | OBJ_DOUBLE_SIZE | OBJ_AFFINE;
        sprites->oam[spriteCount].attr2 =
            (gLuckyDice_FaceTiles[rollingDie->variant / DICE_ROLL_PER_TILE]
             << 3) |
            OBJ_PRIORITY(1);
        spriteCount++;
      }
    }

    if (state == LUCKY_DICE_AIM) {
      for (index = 0; index != DICE_COUNT; index++) {
        Render_ProjectPoint(&diceWork->particles[index], screen);
        x = screen[0] - 2048 / (spriteDepth[drawOrder[index]] / 2 + 128);
        y = screen[1] + 768 / (spriteDepth[drawOrder[index]] / 2 + 128);
        sprites->oam[spriteCount].attr01 =
            OBJ_POSITION(x + 632, y + 352) | OBJ_SIZE_32X32 | OBJ_256_COLORS;
        if (index == 1) {
          sprites->oam[spriteCount].attr01 |= OBJ_HFLIP;
        }
        sprites->oam[spriteCount].attr2 =
            OBJ_PRIORITY(2) | DICE_AIM_MARKER_TILE;
        spriteCount++;
      }
    }

    for (index = 0; index != DICE_COUNT; index++) {
      s32 shadowX;
      s32 shadowY;
      s32 depth;

      rollingDie = &diceWork->particles[index];
      groundPoint[0] = rollingDie->x;
      groundPoint[1] = 0;
      groundPoint[2] = rollingDie->z;
      Render_ProjectPoint(groundPoint, screen);
      depth = screen[2];
      if (depth < DICE_NEAREST_DEPTH) {
        screen[2] = depth = DICE_NEAREST_DEPTH;
      }
      if (depth > DICE_FARTHEST_DEPTH) {
        screen[2] = DICE_FARTHEST_DEPTH;
        depth = screen[2];
      }
      spriteDepth[2 + index] = depth - DICE_NEAREST_DEPTH;
      spriteDepth[2 + index] += FIXEDtoINT(diceWork->particles[index].y);
      shadowX = screen[0];
      shadowY = screen[1];
      if (shadowY + 88 > -16) {
        sprites->oam[spriteCount].attr01 =
            (OBJ_AFFINE_MATRIX(index + 2) | OBJ_X((shadowX + 624) & 0x1ff)) |
            ((shadowY + 344) & 255) | OBJ_SIZE_16X16 | OBJ_256_COLORS |
            OBJ_DOUBLE_SIZE | OBJ_AFFINE;
        sprites->oam[spriteCount].attr2 = OBJ_PRIORITY(2) | DICE_SHADOW_TILE;
        spriteCount++;
      }
    }

    if (state == LUCKY_DICE_ROLL) {
      s32 stoppedDice;

      stoppedDice = 0;
      for (index = 0; index != DICE_COUNT; index++) {
        rollingDie = &diceWork->particles[index];
        rollingDie->x += rollingDie->velocityX;
        rollingDie->y += rollingDie->velocityY;
        rollingDie->z += rollingDie->velocityZ;
        for (otherIndex = 0; otherIndex != DICE_COUNT; otherIndex++) {
          otherDie = &diceWork->particles[otherIndex];
          if (index != otherIndex) {
            separationX = FIXEDtoINT(rollingDie->x - otherDie->x);
            separationY = FIXEDtoINT(rollingDie->y - otherDie->y);
            separationZ = FIXEDtoINT(rollingDie->z - otherDie->z);
            if (separationX * separationX + separationY * separationY +
                    separationZ * separationZ <=
                DICE_TOUCH_DISTANCE * DICE_TOUCH_DISTANCE - 1) {
              separation = Iwram_Sqrt(separationX * separationX +
                                      separationY * separationY +
                                      separationZ * separationZ);
// UB: signed separation components may be negative; touching
// dice are at most 49 pixels apart, so multiplication fits.
// Coincident dice also need to avoid division by zero.
#ifdef UBFIX
              if (separation != 0) {
                rollingDie->velocityX +=
                    separationX * (FIXED_ONE / 2) / separation;
                rollingDie->velocityY +=
                    separationY * (FIXED_ONE / 2) / separation;
                rollingDie->velocityZ +=
                    separationZ * (FIXED_ONE / 2) / separation;
              }
#else
              rollingDie->velocityX += (separationX << 15) / separation;
              rollingDie->velocityY += (separationY << 15) / separation;
              rollingDie->velocityZ += (separationZ << 15) / separation;
#endif
            }
          }
        }
        if (rollingDie->x < -toFIXED(200)) {
          rollingDie->x = -toFIXED(200);
          rollingDie->velocityX = -rollingDie->velocityX * 48 / 64;
        }
        if (rollingDie->x > toFIXED(200)) {
          rollingDie->x = toFIXED(200);
          rollingDie->velocityX = -rollingDie->velocityX * 48 / 64;
        }
        if (rollingDie->z < -toFIXED(150)) {
          rollingDie->z = -toFIXED(150);
          rollingDie->velocityZ = -rollingDie->velocityZ * 48 / 64;
        }
        if (rollingDie->z > rollingDie->y + toFIXED(180)) {
          rollingDie->z = rollingDie->y + toFIXED(180);
          if (rollingDie->velocityZ > 0) {
            rollingDie->velocityY += rollingDie->velocityZ / 2;
            rollingDie->velocityZ = -rollingDie->velocityZ * 48 / 64;
          }
        }
        rollingDie->velocityY += -(toFIXED(1) / 2);
        if (rollingDie->y <= 0) {
          rollingDie->y = 0;
          rollingDie->velocityY = -rollingDie->velocityY * 48 / 64;
          if ((Random16() & 3) == 0) {
            s32 bounceVelocityX;
            s32 bounceVelocityZ;

            bounceVelocityX = rollingDie->velocityX >> 8;
            bounceVelocityZ = rollingDie->velocityZ >> 8;
            bounceSpeed = Iwram_Sqrt(bounceVelocityX * bounceVelocityX +
                                     bounceVelocityZ * bounceVelocityZ) *
                          56 / 64;
            bounceAngle = Random16() & 0xffff;
            rollingDie->velocityX = Trig_Sin(bounceAngle) * bounceSpeed >> 8;
            rollingDie->velocityZ = Trig_Cos(bounceAngle) * bounceSpeed >> 8;
          } else {
            rollingDie->velocityX = rollingDie->velocityX * 56 / 64;
            rollingDie->velocityZ = rollingDie->velocityZ * 56 / 64;
          }
          if (spinRate[index] >= DICE_BOUNCE_SPIN_LOSS) {
            spinRate[index] -= DICE_BOUNCE_SPIN_LOSS;
          }
          if (rollingDie->velocityY < toFIXED(3)) {
            rollingDie->velocityX = 0;
            rollingDie->velocityY = 0;
            rollingDie->velocityZ = 0;
            spinRate[index] = 0;
            rollAngle = rollingDie->variant;
            faceTile = rollAngle / DICE_ROLL_PER_TILE % DICE_TILES_PER_FACE;
            if (faceTile == 1) {
              rollingDie->variant = rollAngle + 2 * DICE_ROLL_PER_TILE;
            } else if (faceTile == 2) {
              rollingDie->variant = rollAngle + DICE_ROLL_PER_TILE;
            }
          }
        }
        rollingDie->velocityX = rollingDie->velocityX * 63 / 64;
        rollingDie->velocityY = rollingDie->velocityY * 63 / 64;
        rollingDie->velocityZ = rollingDie->velocityZ * 63 / 64;
        rollingDie->variant += spinRate[index];
        if (rollingDie->variant > DICE_ROLL_TURN - 1) {
          rollingDie->variant -= DICE_ROLL_TURN;
        }
        if (spinRate[index] > 0) {
          spinRate[index]--;
        }
        if (rollingDie->velocityX == 0 && rollingDie->velocityY == 0 &&
            rollingDie->velocityZ == 0 && rollingDie->y == 0) {
          numbers[index] = rollingDie->variant / DICE_ROLL_PER_FACE;
          for (otherIndex = 0; otherIndex != DICE_FACE_COUNT; otherIndex++) {
            s32 rowDepth;

            rowDepth = toFIXED(gLuckyDice_FaceHeights[otherIndex]);
            if (rollingDie->z > rowDepth) {
              numbers[index + 2] = otherIndex;
              break;
            }
          }
          stoppedDice++;
        }
      }

      if (stoppedDice == DICE_COUNT) {
        state = LUCKY_DICE_RESULT;
        resultTimer = 0;
        resultKind = -1;
        winnings = 0;
        payoutTimer = 60;
        if (numbers[0] == numbers[1] && numbers[0] == numbers[2] &&
            numbers[0] == numbers[3]) {
          flashTimer[0] = flashTimer[1] = DICE_PERFECT_FLASH_FRAMES;
          resultKind = 0;
          if (numbers[0] == luckyNumbers[0]) {
            winnings =
                bet * (DICE_PERFECT_PAYOUT * DICE_FIRST_LUCKY_MULTIPLIER);
          } else if (numbers[0] == luckyNumbers[1]) {
            winnings =
                bet * (DICE_PERFECT_PAYOUT * DICE_SECOND_LUCKY_MULTIPLIER);
          } else {
            winnings = bet * DICE_PERFECT_PAYOUT;
          }
          Text_DrawNumberInWindowFar(gMainRoutineWork.coins, DICE_COIN_DIGITS,
                                     sprites->coinsWindow, 0, 0);
          Sound_PlayFar(MUS_JINGLE_LARGE);
        } else {
          pairCount = 0;
          for (index = 0; index != 3; index++) {
            for (otherIndex = index + 1; otherIndex != DICE_RESULT_COUNT;
                 otherIndex++) {
              if (numbers[index] == numbers[otherIndex]) {
                matchedNumbers[pairCount] = numbers[index];
                pairCount++;
              }
            }
          }
          if (pairCount == 1) {
            winnings = bet;
            Text_DrawNumberInWindowFar(gMainRoutineWork.coins, DICE_COIN_DIGITS,
                                       sprites->coinsWindow, 0, 0);
            resultKind = 3;
          }
          if (pairCount == 2) {
            winnings = bet * 2;
            Text_DrawNumberInWindowFar(gMainRoutineWork.coins, DICE_COIN_DIGITS,
                                       sprites->coinsWindow, 0, 0);
            flashTimer[Random16() & 1] = DICE_FLASH_FRAMES;
            resultKind = 2;
            Sound_PlayFar(MUS_JINGLE_SMALL);
          }
          if (pairCount == 3) {
            winnings = bet * 3;
            Text_DrawNumberInWindowFar(gMainRoutineWork.coins, DICE_COIN_DIGITS,
                                       sprites->coinsWindow, 0, 0);
            flashTimer[Random16() & 1] = DICE_FLASH_FRAMES;
            resultKind = 1;
            Sound_PlayFar(MUS_JINGLE_MEDIUM);
          }
        }
      }
    }

    for (; spriteCount != OAM_SIZE / sizeof(struct OamWords); spriteCount++) {
      sprites->oam[spriteCount].attr01 =
          toFIXED(DISPLAY_WIDTH) | OBJ_SIZE_16X16 | OBJ_256_COLORS;
      sprites->oam[spriteCount].attr2 = 0;
    }

    for (index = 0; index != DICE_RESULT_COUNT; index++) {
      affineScale = spriteDepth[index] / 2 + 128;
      affineA = affineScale;
      affineB = 0;
      affineC = 0;
      affineD = affineScale;
      sprites->oam[index * 4 + 0].attr2 |= affineA << 16;
      sprites->oam[index * 4 + 1].attr2 |= affineB << 16;
      sprites->oam[index * 4 + 2].attr2 |= affineC << 16;
      sprites->oam[index * 4 + 3].attr2 |= affineD << 16;
    }

    Dma_Set(sprites->oam, OAM, DMA_COPY_WORDS(OAM_SIZE / 4), REG_DMA3);
    wait(1);
    frame++;
  }

  for (index = 0; index != DICE_FADE_FRAMES + 1; index++) {
    scale = FIXED_ONE - index * (FIXED_ONE / DICE_FADE_FRAMES);
    LuckyDice_ScaleRgb555Buffer(spritePalette, (u16 *)OBJ_PLTT, scale, 256);
    LuckyDice_ScaleRgb555Buffer(bgPalette, (u16 *)BG_PLTT, scale, 256);
    wait(1);
  }
  HeapSlot_Release(HEAP_SLOT_CAMERA);
  HeapSlot_Release(HEAP_SLOT_CANVAS_MAP);
  HeapSlot_Release(HEAP_SLOT_BATTLE_CANVAS);
  HeapSlot_Release(HEAP_SLOT_BATTLE_EFFECT);
  HeapSlot_Release(HEAP_SLOT_BATTLE_SHEET);
}

// The tables, after LuckyDice_Run in both games.
