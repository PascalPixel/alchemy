/// @file
/// Super Lucky Dice, Contigo's four-dice game with its Hi-Lo double-up, in the
/// order it has in the ROM.

#include "battle/battle.h"
#include "sounds.h"
#include "debug.h"
#include "field/field.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "lib.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "system/io_reg.h"
#include "types.h"
#include "system/ram_buffer.h"
#include "system/iwram_call.h"
#include "system/fixed_point_position.h"
#include "battle_display.h"
#include "macros.h"
#include "fixed_math.h"
#include "system/dma.h"
#include "system/input.h"
#include "system/main_routine_work.h"
#include "bg_scroll.h"
#include "version.h"
#include "actor.h"
#include "system/frame.h"
#include "gamepak/far_call/game.h"
#include "gamepak/far_call/window.h"
#include "system/heap_release.h"
#include "module/field/field.h"
#include "module/system/system.h"
#include "graphics/resource.h"
#include "system/task.h"
#include "system/transform.h"

s32 SuperLuckyDice_Start(void) {
  REG_DISPCNT = DISPCNT_OBJ_1D_MAP;
  SuperLuckyDice_Run();
  return 0;
}

/// Resets the scene camera and its projection, with the camera tilted to 147.
// The Broken Seal's Camera_ResetViewDefaults.
void Camera_ResetSceneDefaultsB(void) {
  struct BattleCamera *state = Ram_HeapSlots->cameraWork;
  struct FixedPointPosition local;
  s32 *result;
  u32 ratio;
  u32 param;

  state->pitch = 147 << 8;
  state->distance = 255 << 17;
  SET_VECTOR(state->pos, 0, 0, 0);
  state->yaw = 0;
  state->followPos = 0;
  // The projection's centre.
  gCameraSceneParameters[3] = 0;
  gCameraSceneParameters[4] = 0;
  state->unk18 = 0;

  Transform_Reset();
  SceneTransform_ApplyPosition(&state->pos[0]);
  Transform_ApplyYaw((s16)state->yaw);
  Transform_ApplyPitch((s16)state->pitch);

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

s32 Graphics_ScaleRgb555BufferB(u16 *source, u16 *destination, s32 scale,
                                s32 count) {
  s32 remaining;
  u32 mask_red;
  u32 mask_green;
  u32 mask_blue;
  u32 pixel;
  u32 red;
  u32 green;
  u32 blue;

  if (count > 0) {
    mask_red = 0x1f;
    mask_green = 0x3e0;
    mask_blue = 0x7c00;
    remaining = count;
    do {
      pixel = *source;
      red = pixel & mask_red;
      green = pixel & mask_green;
      blue = mask_blue & pixel;
      red *= scale;
      green *= scale;
      blue *= scale;
      pixel = ((red >> 16) & mask_red) | ((green >> 16) & mask_green);
      pixel |= (blue >> 16) & mask_blue;
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
    s32 r = (*source & 31) + add;
    s32 g = ((*source >> 5) & 31) + add;
    s32 b = ((*source >> 10) & 31) + add;

    if (r > 31)
      r = 31;
    if (g > 31)
      g = 31;
    if (b > 31)
      b = 31;
    *destination = (b << 10) | (g << 5) | r;
    source++;
    destination++;
  }
}

struct ReelWork {
  u8 unk000[0xc4];
  struct ReelSprite obj[128];
};

s32 SuperLuckyDice_DrawNumber(s32 sprite, s32 x, s32 y, s32 value, s32 palette,
                              s32 digits) {
  // The sprite words are volatile, as The Broken Seal's struct OamWords; as
  // plain u32 members, 84 of 119 instructions differ in tla-en.
  struct ReelWork *work;
  s32 started;
  s32 digit;
  s32 i;
  s32 k;

  work = (struct ReelWork *)Ram_HeapSlots->canvasMap;
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
    work->obj[sprite].attr01 =
        (((x + i * 10 + 522) & 0x1ff) << 16) | ((y + 256) & 255) | 0x40000000;
    work->obj[sprite].attr2 = (digit * 4 + 816) | (palette << 12);
    sprite++;
  }
  return sprite;
}

/// Draws value as digits large digit sprites from x, 22 pixels apart, leading
/// zeros skipped, tiles 640 + 16 * digit in palette, as
/// SuperLuckyDice_DrawNumber does. Returns the next free sprite.
s32 SuperLuckyDice_DrawLargeNumber(s32 sprite, s32 x, s32 y, s32 value,
                                   s32 palette, s32 digits) {
  struct ReelWork *work;
  s32 started;
  s32 digit;
  s32 i;
  s32 k;

  work = (struct ReelWork *)Ram_HeapSlots->canvasMap;
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
    work->obj[sprite].attr01 =
        (((x + i * 22 + 512) & 0x1ff) << 16) | ((y + 256) & 255) | 0x80000000;
    work->obj[sprite].attr2 = (digit * 16 + 640) | (palette << 12);
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

  score = 0;
  if (dice[0] == dice[1] && dice[0] == dice[2] && dice[0] == dice[3]) {
    score = 5;
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
      score = 2;
    if (pairs == 2)
      score = 3;
    if (pairs == 3)
      score = 4;
  }
  return score;
}

const u8 gData_081ba30c[] = {0, 12, 16, 1, 14, 8, 2, 15, 10,
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
#define ROUND_DIGIT_WORD 0x80748000
#elif LANGUAGE == LANGUAGE_SPANISH
#define ROUND_DIGIT_WORD 0x80488000
#elif LANGUAGE == LANGUAGE_FRENCH
#define ROUND_DIGIT_WORD 0x80708000
#elif LANGUAGE == LANGUAGE_ITALIAN
#define ROUND_DIGIT_WORD 0x80488000
#else
#define ROUND_DIGIT_WORD 0x80508000
#endif

#if LANGUAGE == LANGUAGE_FRENCH
#define BET_WINDOW_X 17
#define BET_WINDOW_WIDTH 13
#else
#define BET_WINDOW_X COIN_WINDOW_X
#define BET_WINDOW_WIDTH COIN_WINDOW_WIDTH
#endif

extern u8 gMapCellBuffer[];
extern char MsgLuckyDiceCoins, MsgLuckyDiceWinnings, MsgDiceGameThrow,
    MsgDiceGameHigher, MsgDiceGameLower, MsgDiceGameHigherAgain,
    MsgDiceGameLowerAgain, MsgDiceGameChoose, MsgDiceGameDoubleUp;
extern char ResourceId_FarCall00C, ResourceId_SlotsDomePicture,
    ResourceId_Data087, ResourceId_SlotsSmallDoubleUpBanner,
    ResourceId_SlotsFinalGameBanner, ResourceId_SlotsOrangeDigits,
    ResourceId_SlotsTimesSign, ResourceId_SlotsPinkDigits,
    ResourceId_SlotsHigherBanner, ResourceId_SlotsLowerBanner,
    ResourceId_SlotsYouLoseBanner, ResourceId_SlotsYouWinBanner,
    ResourceId_SlotsOnePairBanner, ResourceId_SlotsTwoPairBanner,
    ResourceId_SlotsTripleBanner, ResourceId_SlotsPerfectBanner,
    ResourceId_SlotsGreenDigits, ResourceId_SlotsDoubleUpBanner,
    ResourceId_SlotsCashOutBanner;

#define ABS(value) ((value) < 0 ? -(value) : (value))

#define DICE_GAME_THROW(dice, spinRate)                                        \
  {                                                                            \
    struct DiceParticle *die;                                                  \
    for (index = 0; index != 4; index++) {                                     \
      die = &(dice)->particles[index];                                         \
      die->z = 0;                                                              \
      die->y = 0x600000;                                                       \
      die->x = (index * 24 - 108) << 16;                                       \
      die->velocityX = ((Random16() & 127) - 64) << 12;                        \
      die->velocityY = ((Random16() & 15) + 48) << 13;                         \
      die->velocityZ = ((Random16() & 127) + 64) << 13;                        \
      die->variant = Random16() % 0x4800;                                      \
      (spinRate)[index] = 300;                                                 \
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
  // volatile: four halfwords stored to sp+138..132 and each reloaded with ldrh;
  // as plain u16 locals, 1094 of 3555 instructions differ in tla-en.
  volatile u16 affineA;
  volatile u16 affineB;
  volatile u16 affineC;
  volatile u16 affineD;
  u16 *spritePalette;
  u8 *cellBuffer;
  struct DiceWork *dice;
  // The OAM shadow it builds is struct DiceOam, whose words are volatile as
  // The Broken Seal's struct OamWords; as plain u32 members, 901 of 3555
  // instructions differ in tla-en.
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

  spritePalette = HeapSlot_Alloc(96, 0x200);
  dice = HeapSlot_AllocEwram(92, 0x7b7c);
  sprites = HeapSlot_AllocEwram(180, 0x614);
  camera = HeapSlot_AllocEwram(48, 76);
  Resource_ReservedNoOp((s32)&ResourceId_FarCall00C);
  Camera_ResetSceneDefaultsB();
  Task_ResetAll();
  gRenderOamEnabled = 0;
  cellBuffer = gMapCellBuffer;

  {
    s32 tileX;
    s32 tileY;
    index = 0;
    for (tileY = 0; tileY != 20; tileY++) {
      for (tileX = 0; tileX != 32; tileX++) {
        *(volatile u16 *)(0x06003000 + index) = tileX + tileY * 30;
        index += 2;
      }
    }
  }

  resourceAddress = Resource_GetTableEntry((s32)&ResourceId_SlotsDomePicture);
  Dma_Set((void *)0x05000000, bgPalette, (DMA_ENABLE | DMA_32BIT) << 16 | 0x80,
          REG_DMA3);
  Dma_Set(resourceAddress, bgPalette, (DMA_ENABLE | DMA_32BIT) << 16 | 0x70,
          REG_DMA3);
  resourceAddress += 0x1c0;
  Decompress_TaggedLz(resourceAddress, cellBuffer);
  Dma_Set(cellBuffer, (void *)0x06004000,
          (DMA_ENABLE | DMA_32BIT) << 16 | 0x2580, REG_DMA3);
  resourceAddress = Resource_GetTableEntry((s32)&ResourceId_Data087);
  Dma_Set(resourceAddress, spritePalette, (DMA_ENABLE | DMA_32BIT) << 16 | 0x30,
          REG_DMA3);
  resourceAddress += 128;
  Decompress_TaggedLz(resourceAddress, cellBuffer);
  Dma_Set(cellBuffer, (void *)0x06010000,
          (DMA_ENABLE | DMA_32BIT) << 16 | 0x4c0, REG_DMA3);
  resourceAddress =
      Resource_GetTableEntry((s32)&ResourceId_SlotsSmallDoubleUpBanner);
  Dma_Set(resourceAddress, spritePalette + 112,
          (DMA_ENABLE | DMA_32BIT) << 16 | 8, REG_DMA3);
  resourceAddress += 32;
  Decompress_TaggedLz(resourceAddress, cellBuffer);
  Dma_Set(cellBuffer, (void *)0x06011300, (DMA_ENABLE | DMA_32BIT) << 16 | 0x80,
          REG_DMA3);
  resourceAddress =
      Resource_GetTableEntry((s32)&ResourceId_SlotsFinalGameBanner);
  Dma_Set(resourceAddress, spritePalette + 96,
          (DMA_ENABLE | DMA_32BIT) << 16 | 8, REG_DMA3);
  resourceAddress += 32;
  Decompress_TaggedLz(resourceAddress, cellBuffer);
  Dma_Set(cellBuffer, (void *)0x06011500,
          (DMA_ENABLE | DMA_32BIT) << 16 | 0x100, REG_DMA3);
  resourceAddress = Resource_GetTableEntry((s32)&ResourceId_SlotsOrangeDigits);
  Dma_Set(resourceAddress, spritePalette + 128,
          (DMA_ENABLE | DMA_32BIT) << 16 | 8, REG_DMA3);
  resourceAddress += 32;
  Decompress_TaggedLz(resourceAddress, cellBuffer);
  Dma_Set(cellBuffer, (void *)0x06011900,
          (DMA_ENABLE | DMA_32BIT) << 16 | 0x280, REG_DMA3);
  resourceAddress = Resource_GetTableEntry((s32)&ResourceId_SlotsTimesSign);
  Dma_Set(resourceAddress, spritePalette + 144,
          (DMA_ENABLE | DMA_32BIT) << 16 | 8, REG_DMA3);
  resourceAddress += 32;
  Decompress_TaggedLz(resourceAddress, cellBuffer);
  Dma_Set(cellBuffer, (void *)0x06012300, (DMA_ENABLE | DMA_32BIT) << 16 | 0x60,
          REG_DMA3);
  resourceAddress = Resource_GetTableEntry((s32)&ResourceId_SlotsPinkDigits);
  Dma_Set(resourceAddress, spritePalette + 160,
          (DMA_ENABLE | DMA_32BIT) << 16 | 8, REG_DMA3);
  {
    u16 *src;
    u16 *dst;
    s32 red;
    s32 blue;

    dst = spritePalette + 176;
    src = (u16 *)resourceAddress;
    for (index = 0; index != 16; index++) {
      blue = ((*src >> 10) & 31) + 4;
      red = (*src & 31) + 4;
      if (red > 31)
        red = 31;
      if (blue > 31)
        blue = 31;
      *dst = (red << 10) | (red << 5) | blue;
      src++;
      dst++;
    }
  }
  Dma_Set(spritePalette + 176, (void *)0x05000360,
          (DMA_ENABLE | DMA_32BIT) << 16 | 8, REG_DMA3);
  resourceAddress += 32;
  Decompress_TaggedLz(resourceAddress, cellBuffer);
  Dma_Set(cellBuffer, (void *)0x06016600,
          (DMA_ENABLE | DMA_32BIT) << 16 | 0x140, REG_DMA3);
  Graphics_ScaleRgb555BufferB(spritePalette, (u16 *)0x05000200, 0, 256);
  Graphics_ScaleRgb555BufferB(bgPalette, (u16 *)0x05000000, 0, 256);

  REG_BG1CNT = 0x686;
  REG_WININ = 0x3737;
  REG_WINOUT = 0x2723;
  REG_DISPCNT = 0x3340;
  REG_BLDCNT = 0x3f44;
  REG_BLDALPHA = 0x810;
  gBgScroll[1].x = 0;
  gBgScroll[1].y = 0;
  REG_BG1HOFS = 0;
  REG_BG1VOFS = 0xff60;
  REG_BG2HOFS = 0;
  REG_BG2VOFS = 0xff60;
  REG_WIN0H = 240;
  REG_WIN0V = 160;
  REG_WIN1H = 240;
  REG_WIN1V = 160;

  total = 0;
  resultKind = 0;
  state = 0;
  round = 0;
  timer = 0;
  slide = 0;
  winnings = 0;
  lower = 0;
  cashOut = 0;
  target = 14;
  multiplier = 1;
  paletteRequest = -1;
  jingle = -1;
  {
    struct DiceParticle *die;

    die = dice->particles;
    for (index = 0; index != 4; index++) {
      die->x = ((Random16() & 127) - 64) << 16;
      die->y = 0;
      die->z = ((Random16() & 127) - 64) << 16;
      die->x = Trig_Sin(index << 14) * 100;
      die->y = 0;
      die->z = Trig_Cos(index << 14) * 100;
      die->velocityX = ((Random16() & 127) - 64) << 14;
      die->velocityY = ((Random16() & 31) + 32) << 12;
      die->velocityZ = ((Random16() & 127) - 64) << 14;
      die->variant = (Random16() % 6) * 0xc00;
      spinRate[index] = 300;
      numbers[index] = 0;
      die++;
    }
  }

  WindowSystem_InitFieldFar();
  window = Window_CreateFar(COIN_WINDOW_X, 0, COIN_WINDOW_WIDTH, 3, 6);
  sprites->coinsWindow = window;
  Text_DrawMessageAsTilesFar((s32)&MsgLuckyDiceCoins, window, COIN_TEXT_X, 0);
  Text_DrawNumberInWindowFar(gMainRoutineWork.coins, 6,
                             (s32)sprites->coinsWindow, 0, 0);
  window = Window_CreateFar(BET_WINDOW_X, 3, BET_WINDOW_WIDTH, 3, 6);
  sprites->betWindow = window;
  Text_DrawMessageAsTilesFar((s32)&MsgLuckyDiceCoins - 1, window, 0, 0);
  window = Window_CreateFar(HELP_WINDOW_X, 16, HELP_WINDOW_WIDTH, 4, 6);
  sprites->helpWindow = window;
  Text_DrawMessageAsTilesFar((s32)&MsgDiceGameThrow, window, 0, 0);
  Text_DrawMessageAsTilesFar((s32)&MsgDiceGameThrow + 1, sprites->helpWindow, 0,
                             8);
  bet = Party_GetAverageLevelFar() * 10;
#if !TLA_PATCH_2
  if ((u32)bet > (u32)gMainRoutineWork.coins)
    bet = gMainRoutineWork.coins;
#endif
  Text_DrawNumberInWindowFar(bet, 6, (s32)sprites->betWindow, BET_TEXT_X, 5);
  Sound_PlayFar(MUS_001);

  frame = 0;
  for (;;) {
    spriteCount = 0;
    if (frame <= 16) {
      Graphics_ScaleRgb555BufferB(spritePalette, (u16 *)0x05000200, frame << 12,
                                  256);
      Graphics_ScaleRgb555BufferB(bgPalette, (u16 *)0x05000000, frame << 12,
                                  256);
    }
    Transform_Reset();
    Transform_SetLookAt(camera->eye, camera->focus);

    if (state == 0) {
      if (frame > 17) {
        if ((u32)gMainRoutineWork.coins <
            (u32)(Party_GetAverageLevelFar() * 10)) {
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
          bet = Party_GetAverageLevelFar() * 10;
          if ((u32)bet > (u32)gMainRoutineWork.coins)
            bet = gMainRoutineWork.coins;
          Text_DrawNumberInWindowFar(bet, 6, (s32)sprites->betWindow,
                                     BET_TEXT_X, 5);
          Party_AdjustCoinsFar(-bet);
          Text_DrawNumberInWindowFar(gMainRoutineWork.coins, 6,
                                     (s32)sprites->coinsWindow, 0, 0);
          state = 2;
          timer = 0;
          slide = 0;
          bet = Party_GetAverageLevelFar() * 10;
          Text_DrawNumberInWindowFar(bet, 6, (s32)sprites->betWindow,
                                     BET_TEXT_X, 5);
        }
      }
    } else if (state == 1) {
      if (slide <= 7) {
        slide++;
      } else if (timer > 0) {
        if (++timer == 16) {
          Sound_PlayFar(SE_LUCKY_DICE_THROW);
          DICE_GAME_THROW(dice, spinRate);
          state = 2;
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
          state = 0;
        }
      } else if (gInput.repeat & state) {
        Sound_PlayFar(SE_MENU_CONFIRM);
        timer = 1;
        Window_CloseFar(sprites->helpWindow, 1);
        if (round == 0) {
          bet = Party_GetAverageLevelFar() * 10;
          if ((u32)bet > (u32)gMainRoutineWork.coins)
            bet = gMainRoutineWork.coins;
          Text_DrawNumberInWindowFar(bet, 6, (s32)sprites->betWindow,
                                     BET_TEXT_X, 5);
          Party_AdjustCoinsFar(-bet);
          Text_DrawNumberInWindowFar(gMainRoutineWork.coins, 6,
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
        SuperLuckyDice_BrightenColors(spritePalette + 192, (u16 *)0x05000380,
                                      16, timer << 12);
        Graphics_ScaleRgb555BufferB(spritePalette + 208, (u16 *)0x050003a0,
                                    0x8000, 16);
        SuperLuckyDice_BrightenColors(spritePalette + 224, (u16 *)0x050003c0,
                                      16, timer << 12);
        Graphics_ScaleRgb555BufferB(spritePalette + 240, (u16 *)0x050003e0,
                                    0x8000, 16);
      } else {
        SuperLuckyDice_BrightenColors(spritePalette + 208, (u16 *)0x050003a0,
                                      16, timer << 12);
        Graphics_ScaleRgb555BufferB(spritePalette + 192, (u16 *)0x05000380,
                                    0x8000, 16);
        SuperLuckyDice_BrightenColors(spritePalette + 240, (u16 *)0x050003e0,
                                      16, timer << 12);
        Graphics_ScaleRgb555BufferB(spritePalette + 224, (u16 *)0x050003c0,
                                    0x8000, 16);
      }
    } else if (state == 2) {
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

            separationX = (die->x - other->x) >> 16;
            separationY = (die->y - other->y) >> 16;
            separationZ = (die->z - other->z) >> 16;
            if (separationX * separationX + separationY * separationY +
                    separationZ * separationZ <=
                0x1323) {
              separation = Iwram_Sqrt(separationX * separationX +
                                      separationY * separationY +
                                      separationZ * separationZ);
              die->velocityX += (separationX << 15) / separation;
              die->velocityY += (separationY << 15) / separation;
              die->velocityZ += (separationZ << 15) / separation;
            }
          }
        }
        wallX = die->x >> 16;
        wallZ = die->z >> 16;
        if (Iwram_Sqrt(wallX * wallX + wallZ * wallZ) > 199) {
          if (die->x > 0)
            die->velocityX = -ABS(die->velocityX);
          else
            die->velocityX = ABS(die->velocityX);
          if (die->z > 0)
            die->velocityZ = -ABS(die->velocityZ);
          else
            die->velocityZ = ABS(die->velocityZ);
        }
        die->velocityY += -(px(1) / 2);
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
          if (spinRate[index] > 19)
            spinRate[index] -= 20;
          if (die->velocityY < 0x30000) {
            s32 spin;
            s32 facing;

            die->velocityX = 0;
            die->velocityY = 0;
            die->velocityZ = 0;
            spinRate[index] = 0;
            spin = die->variant;
            facing = spin / 1024 % 3;
            if (facing == 1)
              die->variant = spin + 0x800;
            else if (facing == 2)
              die->variant = spin + 0x400;
          }
        }
        die->velocityX = die->velocityX * 63 / 64;
        die->velocityY = die->velocityY * 63 / 64;
        die->velocityZ = die->velocityZ * 63 / 64;
        die->variant += spinRate[index];
        if (die->variant > 0x47ff)
          die->variant -= 0x4800;
        if (spinRate[index] > 0)
          spinRate[index]--;
        if (die->velocityX == 0 && die->velocityY == 0 && die->velocityZ == 0 &&
            die->y == 0) {
          numbers[index] = die->variant / 0xc00;
          stoppedDice++;
        }
      }
      if (stoppedDice == 4) {
        state = 3;
        timer = 0;
        total = 0;
        for (index = 0; index != 4; index++)
          total = total + numbers[index] + 1;
      }
    } else if (state == 3) {
      timer++;
      if (timer == 24)
        Sound_PlayFar(SE_JUMP);
      if (timer == 70)
        Sound_PlayFar(SE_CURSE);
      if (timer == 40) {
        if ((u32)resultKind > 1)
          Sound_PlayFar(MUS_055);
        else if (jingle != -1)
          Sound_PlayFar(jingle);
        jingle = -1;
      }
      if (timer == 1) {
        paletteRequest = 3;
        resultKind = 0;
        jingle = -1;
        if (round == 0) {
          resultKind = SuperLuckyDice_ScoreDice(numbers);
          switch (resultKind) {
          case 5:
            jingle = 93;
            multiplier = 8;
            break;
          case 2:
            jingle = 91;
            multiplier = 1;
            break;
          case 3:
            jingle = 92;
            multiplier = 3;
            break;
          case 4:
            jingle = 91;
            multiplier = 2;
            break;
          }
        } else if (lower == 0) {
          if (total > target)
            resultKind = 1;
        } else if (total < target) {
          resultKind = 1;
        }
        if (resultKind == 1) {
          if (round <= 3)
            jingle = 91;
          if (round == 4)
            jingle = 92;
          if (round == 5)
            jingle = 93;
        }
      }
      if (resultKind == 0 ? timer == 90 : timer == 68) {
        if (resultKind != 0) {
          if (round == 0)
            bet *= multiplier;
          else
            bet <<= 1;
          Text_DrawNumberInWindowFar(bet, 6, (s32)sprites->betWindow,
                                     BET_TEXT_X, 5);
          multiplier <<= 1;
          if (round == 5) {
            target = 14;
            round = 0;
            winnings = bet;
            state = 4;
          } else {
            state = 5;
            paletteRequest = 5;
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
          bet = Party_GetAverageLevelFar() * 10;
          Text_DrawNumberInWindowFar(bet, 6, (s32)sprites->betWindow,
                                     BET_TEXT_X, 5);
          multiplier = 0;
          Sound_PlayFar(MUS_001);
          state = 0;
          window = Window_CreateFar(HELP_WINDOW_X, 16, HELP_WINDOW_WIDTH, 4, 6);
          sprites->helpWindow = window;
          Text_DrawMessageAsTilesFar((s32)&MsgDiceGameThrow, window, 0, 0);
          Text_DrawMessageAsTilesFar((s32)&MsgDiceGameThrow + 1,
                                     sprites->helpWindow, 0, 8);
          slide = 0;
          timer = 0;
          paletteRequest = 1;
        }
      }
    } else if (state == 4) {
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
          Sound_PlayFar(MUS_001);
          Sound_PlayFar(SE_MENU_CONFIRM);
          state = 0;
          window = Window_CreateFar(HELP_WINDOW_X, 16, HELP_WINDOW_WIDTH, 4, 6);
          sprites->helpWindow = window;
          Text_DrawMessageAsTilesFar((s32)&MsgDiceGameThrow, window, 0, 0);
          Text_DrawMessageAsTilesFar((s32)&MsgDiceGameThrow + 1,
                                     sprites->helpWindow, 0, 8);
          slide = 0;
          timer = 0;
          paletteRequest = 1;
          bet = Party_GetAverageLevelFar() * 10;
          Text_DrawNumberInWindowFar(bet, 6, (s32)sprites->betWindow,
                                     BET_TEXT_X, 5);
          if (winnings > 0) {
            Sound_PlayFar(SE_105);
            Party_AdjustCoinsFar(winnings);
            Text_DrawNumberInWindowFar(gMainRoutineWork.coins, 6,
                                       (s32)sprites->coinsWindow, 0, state);
          }
        } else if (winnings > 0) {
          if (--winnings == 0)
            Sound_PlayFar(SE_105);
          Party_AdjustCoinsFar(1);
          Text_DrawNumberInWindowFar(gMainRoutineWork.coins, 6,
                                     (s32)sprites->coinsWindow, 0, 0);
          bet--;
          Text_DrawNumberInWindowFar(bet, 6, (s32)sprites->betWindow,
                                     BET_TEXT_X, 5);
        }
      }
    } else if (state == 5) {
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
            state = 1;
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
            paletteRequest = 1;
          } else {
            target = 14;
            round = 0;
            winnings = bet;
            state = 4;
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
        SuperLuckyDice_BrightenColors(spritePalette + 224, (u16 *)0x050003c0,
                                      16, timer << 12);
        Graphics_ScaleRgb555BufferB(spritePalette + 240, (u16 *)0x050003e0,
                                    0x8000, 16);
      } else {
        SuperLuckyDice_BrightenColors(spritePalette + 240, (u16 *)0x050003e0,
                                      16, timer << 12);
        Graphics_ScaleRgb555BufferB(spritePalette + 224, (u16 *)0x050003c0,
                                    0x8000, 16);
      }
    }

    // Load the pictures the state asked for.
    if (paletteRequest != -1) {
      if (paletteRequest == 1) {
        resourceAddress =
            Resource_GetTableEntry((s32)&ResourceId_SlotsHigherBanner);
        Dma_Set(resourceAddress, (void *)0x05000380,
                (DMA_ENABLE | DMA_32BIT) << 16 | 8, REG_DMA3);
        Dma_Set(resourceAddress, spritePalette + 192,
                (DMA_ENABLE | DMA_32BIT) << 16 | 8, REG_DMA3);
        resourceAddress += 32;
        Decompress_TaggedLz(resourceAddress, cellBuffer);
        Dma_Set(cellBuffer, (void *)0x06014000,
                (DMA_ENABLE | DMA_32BIT) << 16 | 0x180, REG_DMA3);
        resourceAddress =
            Resource_GetTableEntry((s32)&ResourceId_SlotsLowerBanner);
        Dma_Set(resourceAddress, (void *)0x050003a0,
                (DMA_ENABLE | DMA_32BIT) << 16 | 8, REG_DMA3);
        Dma_Set(resourceAddress, spritePalette + 208,
                (DMA_ENABLE | DMA_32BIT) << 16 | 8, REG_DMA3);
        resourceAddress += 32;
        Decompress_TaggedLz(resourceAddress, cellBuffer);
        Dma_Set(cellBuffer, (void *)0x06014600,
                (DMA_ENABLE | DMA_32BIT) << 16 | 0x180, REG_DMA3);
        resourceAddress =
            Resource_GetTableEntry((s32)&ResourceId_SlotsPinkDigits);
        {
          u16 *src;
          u16 *dst;
          s32 red;
          s32 blue;

          dst = spritePalette + 224;
          src = (u16 *)resourceAddress;
          for (index = 0; index != 16; index++) {
            blue = ((*src >> 10) & 31) + 4;
            red = (*src & 31) + 4;
            if (red > 31)
              red = 31;
            if (blue > 31)
              blue = 31;
            *dst = (blue << 10) | (red << 5) | blue;
            src++;
            dst++;
          }
        }
        Dma_Set(spritePalette + 224, (void *)0x050003c0,
                (DMA_ENABLE | DMA_32BIT) << 16 | 8, REG_DMA3);
        Dma_Set(spritePalette + 224, (void *)0x050003e0,
                (DMA_ENABLE | DMA_32BIT) << 16 | 8, REG_DMA3);
        Dma_Set(spritePalette + 224, spritePalette + 240,
                (DMA_ENABLE | DMA_32BIT) << 16 | 8, REG_DMA3);
      } else if (paletteRequest == 3) {
        switch (resultKind) {
        case 0:
          resourceAddress =
              Resource_GetTableEntry((s32)&ResourceId_SlotsYouLoseBanner);
          break;
        case 1:
          resourceAddress =
              Resource_GetTableEntry((s32)&ResourceId_SlotsYouWinBanner);
          break;
        case 2:
          resourceAddress =
              Resource_GetTableEntry((s32)&ResourceId_SlotsOnePairBanner);
          break;
        case 3:
          resourceAddress =
              Resource_GetTableEntry((s32)&ResourceId_SlotsTwoPairBanner);
          break;
        case 4:
          resourceAddress =
              Resource_GetTableEntry((s32)&ResourceId_SlotsTripleBanner);
          break;
        case 5:
          resourceAddress =
              Resource_GetTableEntry((s32)&ResourceId_SlotsPerfectBanner);
          break;
        }
        Dma_Set(resourceAddress, (void *)0x050003a0,
                (DMA_ENABLE | DMA_32BIT) << 16 | 8, REG_DMA3);
        resourceAddress += 32;
        Decompress_TaggedLz(resourceAddress, cellBuffer);
        Dma_Set(cellBuffer, (void *)0x06014000,
                (DMA_ENABLE | DMA_32BIT) << 16 | 0x400, REG_DMA3);
        resourceAddress =
            Resource_GetTableEntry((s32)&ResourceId_SlotsGreenDigits);
        Dma_Set(resourceAddress, (void *)0x05000380,
                (DMA_ENABLE | DMA_32BIT) << 16 | 8, REG_DMA3);
        resourceAddress += 32;
        Decompress_TaggedLz(resourceAddress, cellBuffer);
        Dma_Set(cellBuffer, (void *)0x06015000,
                (DMA_ENABLE | DMA_32BIT) << 16 | 0x500, REG_DMA3);
      } else if (paletteRequest == 5) {
        resourceAddress =
            Resource_GetTableEntry((s32)&ResourceId_SlotsDoubleUpBanner);
        Dma_Set(resourceAddress, (void *)0x050003c0,
                (DMA_ENABLE | DMA_32BIT) << 16 | 8, REG_DMA3);
        Dma_Set(resourceAddress, spritePalette + 224,
                (DMA_ENABLE | DMA_32BIT) << 16 | 8, REG_DMA3);
        resourceAddress += 32;
        Decompress_TaggedLz(resourceAddress, cellBuffer);
        Dma_Set(cellBuffer, (void *)0x06012480,
                (DMA_ENABLE | DMA_32BIT) << 16 | 0x280, REG_DMA3);
        resourceAddress =
            Resource_GetTableEntry((s32)&ResourceId_SlotsCashOutBanner);
        Dma_Set(resourceAddress, (void *)0x050003e0,
                (DMA_ENABLE | DMA_32BIT) << 16 | 8, REG_DMA3);
        Dma_Set(resourceAddress, spritePalette + 240,
                (DMA_ENABLE | DMA_32BIT) << 16 | 8, REG_DMA3);
        resourceAddress += 32;
        Decompress_TaggedLz(resourceAddress, cellBuffer);
        Dma_Set(cellBuffer, (void *)0x06012e80,
                (DMA_ENABLE | DMA_32BIT) << 16 | 0x280, REG_DMA3);
      }
      paletteRequest = -1;
    }

    // The double-up rounds won, the final-game or round lamps, and the
    // multiplier.
    if (round > 0) {
      s32 multiplierDigits;

      for (index = 0; index != 4; index++) {
        sprites->oam[spriteCount].attr01 =
            (((0x200 + index * 16) & 0x1ff) << 16) | 0x40000002;
        sprites->oam[spriteCount].attr2 = (152 + index * 4) | 0x7000;
        spriteCount++;
      }
      if (round == 5) {
        for (index = 0; index != 5; index++) {
          sprites->oam[spriteCount].attr01 =
              (((584 + index * 16) & 0x1ff) << 16) | 0x40000002;
          sprites->oam[spriteCount].attr2 = (168 + index * 4) | 0x6000;
          spriteCount++;
        }
      } else {
        for (index = 0; index != 3; index++) {
          sprites->oam[spriteCount].attr01 =
              (((ROUND_BANNER_X + index * 16) & 0x1ff) << 16) | 0x40000002;
          sprites->oam[spriteCount].attr2 = (188 + index * 4) | 0x6000;
          spriteCount++;
        }
        sprites->oam[spriteCount].attr01 = ROUND_DIGIT_WORD;
        sprites->oam[spriteCount].attr2 = (round * 8 + 200) | 0x8000;
        spriteCount++;
#if LANGUAGE == LANGUAGE_SPANISH
        if (round == 1) {
          sprites->oam[spriteCount].attr01 = 0x80508000;
          sprites->oam[spriteCount].attr2 = 200 | 0x8000;
        } else {
          sprites->oam[spriteCount].attr01 = 0x80548000;
          sprites->oam[spriteCount].attr2 = 200 | 0x8000;
        }
        spriteCount++;
#endif
      }
      for (index = 0; index != 2; index++) {
        sprites->oam[spriteCount].attr01 =
            (((576 + index * 16) & 0x1ff) << 16) | 0x40000018;
        sprites->oam[spriteCount].attr2 = (284 + index * 4) | 0x9000;
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
          (((560 - multiplierDigits * 10) & 0x1ff) << 16) | 0x40000018;
      sprites->oam[spriteCount].attr2 = 0x9118;
      spriteCount++;
      spriteCount =
          SuperLuckyDice_DrawNumber(spriteCount, 48, 20, multiplier, 10, 5);
      spriteCount = SuperLuckyDice_DrawNumber(spriteCount, MULTIPLIER_ZERO_X,
                                              20, 0, 11, 1);
    }

    // The higher and lower choices, with the target to beat.
    if (state == 1) {
      s32 x;
      s32 wobble;
      s32 left;

      x = (timer > 0 && lower == 1) ? timer * 12 + 64 : 64;
      wobble = lower == 0 ? Trig_Sin((frame << 10) & 0x7fff) >> 14 : 0;
      left = x + slide * 15;
      if (target <= 23)
        spriteCount = SuperLuckyDice_DrawNumber(spriteCount, left - 110,
                                                wobble + 60, target + 1, 14, 2);
      x = (timer > 0 && lower == 0) ? timer * 12 + 64 : 64;
      wobble = lower == 1 ? Trig_Sin((frame << 10) & 0x7fff) >> 14 : 0;
      left = x + slide * 15;
      if (target > 4)
        spriteCount = SuperLuckyDice_DrawNumber(spriteCount, left - 110,
                                                94 - wobble, target - 1, 15, 2);
      for (index = 0; index != 3; index++) {
        s32 column;

        column = 64;
        if (timer > 0 && lower == 1)
          column = timer * 12 + 64;
        column += slide * 15;
        column -= 120;
        sprites->oam[spriteCount].attr01 =
            (((column + index * 32 + 512) & 0x1ff) << 16) | 0x80000034;
        sprites->oam[spriteCount].attr2 = 0xc000 | (512 + index * 16);
        spriteCount++;
        column = 64;
        if (timer > 0 && lower == 0)
          column = timer * 12 + 64;
        column += slide * 15;
        column -= 120;
        sprites->oam[spriteCount].attr01 =
            (((column + index * 32 + 512) & 0x1ff) << 16) | 0x8000005c;
        sprites->oam[spriteCount].attr2 = (560 + index * 16) | 0xd000;
        spriteCount++;
      }
    }

    // Double up or cash out.
    if (state == 5) {
      for (index = 0; index != 5; index++) {
        s32 column;

        column = 64;
        if (timer > 0 && cashOut == 1)
          column = timer * 12 + 64;
        column += slide * 15;
        column -= 120;
        sprites->oam[spriteCount].attr01 =
            (((column + index * 32 + 512) & 0x1ff) << 16) | 0x80000034;
        sprites->oam[spriteCount].attr2 = (292 + index * 16) | 0xe000;
        spriteCount++;
        column = 64;
        if (timer > 0 && cashOut == 0)
          column = timer * 12 + 64;
        column += slide * 15;
        column -= 120;
        sprites->oam[spriteCount].attr01 =
            (((column + index * 32 + 512) & 0x1ff) << 16) | 0x80000054;
        sprites->oam[spriteCount].attr2 = (372 + index * 16) | 0xf000;
        spriteCount++;
      }
    }

    // The total and the result's banner.
    if (state == 3 || (state == 5 && resultKind != 0)) {
      s32 x;
      s32 y;
      s32 angle;
      s32 doubled;
      s32 wave;

      x = (timer << 4) - 376;
      y = 48;
      if (x > 100)
        x = 100;
      if (state == 5) {
        angle = slide << 12;
        angle += TRIG_TURN / 16;
        x = (Trig_Cos(angle) * 40 >> 16) - (timer << 4) + 56;
        y = 48 - (Trig_Sin(angle) * 48 >> 16);
      } else if (resultKind != 0) {
        if (timer > 67) {
          s32 elapsed = timer - 68;

          angle = elapsed * 1489;
          x = (Trig_Cos(angle) * 48 >> 16) + 52;
          y = 48 - (Trig_Sin(angle) * 48 >> 16);
        }
      } else if (timer > 67) {
        doubled = timer * 2;
        wave = doubled - 128;
        x = (Trig_Sin((timer << 12) - 0x40000) * wave >> 16) + 100;
        doubled += timer;
        doubled *= 2;
        y = doubled - 360;
      }
      if (x >= -31)
        spriteCount =
            SuperLuckyDice_DrawLargeNumber(spriteCount, x, y, total, 12, 2);
      if (timer > 30 || (state == 5 && resultKind != 0)) {
        s32 bannerX;
        s32 bannerY;

        bannerY = 64;
        bannerX = timer > 37 ? 60 : (timer << 4) - 620;
        if (state == 5) {
          bannerY = 64;
          bannerX = slide * 24 + 60;
        } else if (timer > 67) {
          doubled = timer * 2;
          wave = doubled - 136;
          bannerX = (Trig_Sin((timer << 12) - 0x44000) * wave >> 16) + 60;
          doubled += timer;
          doubled *= 2;
          bannerY = doubled - 344;
        }
        sprites->oam[spriteCount].attr01 = (((bannerX + 512) & 0x1ff) << 16) |
                                           ((bannerY + 256) & 255) | 0xc0000000;
        sprites->oam[spriteCount].attr2 = 0xd200;
        spriteCount++;
        sprites->oam[spriteCount].attr01 = (((bannerX + 576) & 0x1ff) << 16) |
                                           ((bannerY + 256) & 255) | 0xc0000000;
        sprites->oam[spriteCount].attr2 = 0xd240;
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
      if (screen[2] < 0x15e)
        screen[2] = 0x15e;
      if (screen[2] > 0x28a)
        screen[2] = 0x28a;
      spriteDepth[drawOrder[index]] = screen[2] - 0x15e;
      x = screen[0] - 0xc00 / (spriteDepth[drawOrder[index]] / 4 + 64);
      y = screen[1] - 0x800 / (spriteDepth[drawOrder[index]] / 4 + 64);
      if (y + 92 > -16) {
        sprites->oam[spriteCount].attr01 =
            ((drawOrder[index] << 25) | (((x + 632) & 0x1ff) << 16) |
             ((y + 348) & 255)) |
            0x40002300;
        sprites->oam[spriteCount].attr2 =
            (gData_081ba30c[die->variant / 1024] << 3) | 0x400;
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
        if (screen[2] < 0x15e)
          screen[2] = 0x15e;
        if (screen[2] > 0x28a)
          screen[2] = 0x28a;
        spriteDepth[4 + index] = screen[2] - 0x15e;
        spriteDepth[4 + index] += (s16)(die->y >> 16);
        half = spriteDepth[4 + index] / 2 + 128;
        x = screen[0] - 0x1800 / half;
        y = screen[1] - 0xc00 / half;
        if (y + 92 > -16) {
          sprites->oam[spriteCount].attr01 =
              (((index + 4) << 25) | (((x + 632) & 0x1ff) << 16)) |
              ((y + 348) & 255) | 0x40002300;
          sprites->oam[spriteCount].attr2 = 0x890;
          spriteCount++;
        }
      }
    }
    for (; spriteCount != 128; spriteCount++) {
      sprites->oam[spriteCount].attr01 = 0x40f02000;
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
    Dma_Set(sprites->oam, (void *)0x07000000,
            (DMA_ENABLE | DMA_32BIT) << 16 | 0x100, REG_DMA3);
    wait(1);
    frame++;
  }

  for (index = 0; index != 17; index++) {
    Graphics_ScaleRgb555BufferB(spritePalette, (u16 *)0x05000200,
                                0x10000 - (index << 12), 256);
    Graphics_ScaleRgb555BufferB(bgPalette, (u16 *)0x05000000,
                                0x10000 - (index << 12), 256);
    wait(1);
  }
  HeapSlot_Release(48);
  HeapSlot_Release(180);
  HeapSlot_Release(96);
  HeapSlot_Release(92);
  HeapSlot_Release(100);
}
