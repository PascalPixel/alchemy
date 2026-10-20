#include "unit/field/lucky_dice.h"
#include "unit/runtime/runtime.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "io_reg.h"
#include "battle_display.h"
#include "project.h"
#include "types.h"
#include "iwram_call.h"
#include "scene.h"
#include "edition.h"
#include "callback_scheduler.h"
#include "call.h"
#include "frame.h"
#include "oambuf.h"
#include "songs.h"
#include "macros.h"

/// Starts the Lucky Dice minigame: leaves only the backdrop on screen, starts
/// song MUS_009 and plays until the player quits. Returns 0. Reached through
/// Runtime_BlankDisplayAndRunFar from RunLuckyDice (object/object/scene.c),
/// which saves background palettes 14 and 15 around it and passes an
/// entrance this function does not take.
s32 Runtime_BlankDisplayAndRun(void) {
  REG_DISPCNT = DISPCNT_OBJ_1D_MAP;
  AudioCommand_PlayFar(MUS_009);
  LuckyDice_Run();
  return 0;
}

/// Puts the 3D camera back at its scene defaults: at the origin, unturned,
/// pitched to 152 and 510 units away, following nothing. Recomputes the eye
/// and sets the projection's focal length (250), near plane and far plane.
/// Called only by LuckyDice_Run, once the camera's heap block is allocated;
/// writes gCameraWork's fields and gProjection.
void Camera_ResetSceneDefaults(void) {
  struct BattleCamera *camera = gCameraWork;
  s32 offset[3];
  u32 nearPlane;
  u32 focalLength;

  camera->pitch = 152 << 8;
  camera->distance = 255 << 17;
  SET_VECTOR(camera->pos, 0, 0, 0);
  camera->yaw = 0;
  camera->followPos = 0;
  gProjection.centerX = 0;
  gProjection.centerY = 0;
  camera->unk18 = 0;

  // The eye sits `distance` behind the target along the camera's view.
  Render_ResetTransformState();
  SceneTransform_ApplyPosition(&camera->pos[0]);
  SceneTransform_ApplyYaw((s16)camera->yaw);
  SceneTransform_ApplyPitch((s16)camera->pitch);

  SET_VECTOR(offset, 0, 0, camera->distance);
  Iwram_TransformVector(offset, camera->eye);

  focalLength = 250;
  focalLength <<= 16;
  nearPlane = Iwram_RatioMulQ14(focalLength, 192 << 8);

  focalLength = 250;
  focalLength <<= 16;
  Projection_Set(focalLength, nearPlane, 0x7fff0000);
}

/// Multiplies two 8.8 fixed-point values, rounding toward zero.
UNUSED s32 FixedPoint_Multiply8_8(s32 leftValue, s32 rightValue) {
  /* FAKEMATCH(temporary): the product built in its own variable from a copy
   * of right, then rounded into another; as product = left * right adjusted
   * in place, 4 of 14 instructions differ in tbs-en. */
  s16 left = leftValue;
  s16 right = rightValue;
  s32 product;
  s32 roundedProduct;
  s32 multiplier;

  multiplier = right;
  product = left;
  product *= multiplier;
  if (product >= 0) {
    roundedProduct = product;
  } else {
    roundedProduct = product + 255;
  }
  return (s16)(roundedProduct >> 8);
}

/// Divides two 8.8 fixed-point values.
UNUSED s16 Math_ScaleByRatio(s16 numerator, s16 denominator) {
// UB: a signed numerator may be negative. All s16 values times 256 fit.
#ifdef UBFIX
  return (numerator * 256) / denominator;
#else
  return (numerator << 8) / denominator;
#endif
}

/// The reciprocal of an 8.8 fixed-point value.
UNUSED s16 FixedPoint_Reciprocal(s16 value) { return 0x10000 / value; }

#include "resource.h"
#include "mem.h"
#include "dma.h"
#include "ram_buffer.h"
#include "fixed_math.h"
#include "ui.h"
#include "game_state.h"
#include "map_scroll.h"
#include "battle_effect_work.h"
#include "transform.h"
#include "resource_ids.h"
#include "text/msg_ids.h"
#include "songs.h"
#include "heap_state.h"

// Lucky Dice window layout, in tiles (and pixels for text), by edition: the
// coins window at the top, the help window (Throw / Quit) and the winnings
// window at the bottom, and the result lettering's x in sprite coordinates.
#if (LANGUAGE == LANGUAGE_GERMAN) || (LANGUAGE == LANGUAGE_SPANISH) ||         \
    (LANGUAGE == LANGUAGE_FRENCH)
#define DICE_COIN_X 17
#define DICE_COIN_W 13
#define DICE_WIN_W 13
#elif (LANGUAGE != LANGUAGE_JAPANESE)
#define DICE_COIN_X 18
#define DICE_COIN_W 12
#define DICE_WIN_W 12
#else
#define DICE_COIN_X 18
#define DICE_COIN_W 11
#define DICE_WIN_W 10
#endif

#if (LANGUAGE == LANGUAGE_SPANISH)
#define DICE_LABEL_X 56
#elif (LANGUAGE == LANGUAGE_FRENCH)
#define DICE_LABEL_X 53
#else
#define DICE_LABEL_X 48
#endif

#if (LANGUAGE == LANGUAGE_GERMAN) || (LANGUAGE == LANGUAGE_SPANISH)
#define DICE_HELP_X 20
#define DICE_HELP_W 10
#elif (LANGUAGE == LANGUAGE_FRENCH)
#define DICE_HELP_X 21
#define DICE_HELP_W 9
#elif (LANGUAGE != LANGUAGE_JAPANESE)
#define DICE_HELP_X 22
#define DICE_HELP_W 8
#else
#define DICE_HELP_X 23
#define DICE_HELP_W 7
#endif

#if (LANGUAGE != LANGUAGE_JAPANESE)
#define DICE_WIN_X 9
#define DICE_WIN_NUM_X 48
#define DICE_WIN_LABEL_X DICE_LABEL_X
#define DICE_KIND_X 0x278
#else
#define DICE_WIN_X 10
#define DICE_WIN_NUM_X 40
#define DICE_WIN_LABEL_X 40
#define DICE_KIND_X 0x280
#endif

/// The used prefix of Lucky Dice's 0x618-byte work block (heap slot 45):
/// its sprites and windows. The allocation extends beyond these members.
struct LuckyDiceSprites {
  u8 filler[200];                  ///< Lucky Dice never touches these bytes
  struct OamShadow obj[128];       ///< copied to OAM each frame
  struct UiWindow *coinsWindow;    ///< the player's coins
  struct UiWindow *winningsWindow; ///< the winnings of the last throw
  struct UiWindow *helpWindow;     ///< the Throw and Quit buttons
};

#include "version.h"
#include "keystate.h"
#include "../graphics/lucky_dice/tables.h"

#define DICE_COUNT 2
#define DICE_FACE_COUNT 6
#define DICE_RESULT_COUNT 4
#define DICE_BET_PER_LEVEL 10

/// What the Lucky Dice screen is doing.
enum LuckyDiceState {
  LUCKY_DICE_AIM,    ///< the dice wait to be thrown; Up and Down raise them
  LUCKY_DICE_ROLL,   ///< the dice fly, bounce and settle
  LUCKY_DICE_RESULT, ///< the four numbers and the result are shown
  LUCKY_DICE_PAYOUT  ///< the winnings move into the coins
};

/// Copies count RGB555 colours from source to destination with each channel
/// multiplied by scale (16.16: 0 is black, 0x10000 unchanged), for fades.
/// Returns 0. Called only by LuckyDice_Run, to fade both palettes in and out.
s32 Graphics_ScaleRgb555Buffer(u16 *source, u16 *destination, s32 scale,
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
    redMask = 0x1f;
    greenMask = 0x3e0;
    blueMask = 0x7c00;
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

/// Runs the Lucky Dice minigame until the player quits or cannot afford a
/// throw. Each throw costs ten coins per level of the party's average. Two
/// dice are thrown onto a board of six rows; each die's face and the row it
/// stops in give four numbers. Four of a kind pays 5 times the bet (15 or 25
/// times when it matches one of the two lucky numbers shown at the top),
/// three of a kind 3 times, two pairs twice and one pair the bet back.
/// Called by Runtime_BlankDisplayAndRun. Takes the bet from and pays the
/// winnings into gGameState.coins, resets the scheduler's task table, and
/// frees the five heap slots it allocates before it returns.
void LuckyDice_Run(void) {
  u16 bgPalette[256];
  s32 spriteDepth[DICE_RESULT_COUNT]; // the two dice, then their shadows
  // The two faces, then the two rows the dice stopped in.
  s32 numbers[DICE_RESULT_COUNT];
  s32 groundPoint[3];
  s32 screen[3];
  /* FAKEMATCH(dead): an unused local for the 32 bytes the native frame keeps
   * at sp+168..200, between screen and the affine halfwords; without it, 21
   * of 2046 instructions differ in tbs-en. */
  UNUSED s32 unused[8];
  /* FAKEMATCH(volatile): four halfwords stored to sp+166..160 and each
   * reloaded with ldrh, the form the bytes prove; as plain u16 locals, 463 of
   * 2046 instructions differ in tbs-en. */
  volatile u16 affineA;
  volatile u16 affineB;
  volatile u16 affineC;
  volatile u16 affineD;
  s32 spinRate[DICE_COUNT];
  s32 flashTimer[DICE_COUNT];   // set on a win; nothing here reads it
  s32 luckyNumbers[DICE_COUNT]; // the two numbers shown at the top, never equal
  s32 drawOrder[DICE_COUNT];    // the dice, back to front
// UB: three equal numbers produce three pairs, so the unused result list
// needs a third slot. Keep the original frame layout in the matching build.
#ifdef UBFIX
  UNUSED s32 matchedNumbers[3]; // written, never read
#else
  UNUSED s32 matchedNumbers[2]; // written, never read
#endif
  u16 *spritePalette;
  struct BattleEffectWork *diceWork;
  /* FAKEMATCH(volatile): the OAM shadow it builds is struct OamShadow, whose
   * words are volatile (oambuf.h); as plain words, 810 of 2046 instructions
   * differ in tbs-en. */
  struct LuckyDiceSprites *sprites;
  struct BattleCamera *camera;
  s32 frame;
  s32 state;
  s32 bet;
  s32 resultTimer;
  s32 resultKind;
  s32 winnings;
  s32 payoutTimer;
  // RESULT frame 20 sets the number's digit count before A can enter PAYOUT.
  // It persists across frames to clear the same numeric field afterward.
  s32 digits;
  s32 spriteCount;
  struct EffectStep *rollingDie;
  struct EffectStep *otherDie;
  s32 resetDice; // put the dice back at the throwing line
  struct UiWindow *window;
  s32 index;
  s32 otherIndex;
  s32 rollAngle; // a die's roll angle: 0x400 per face tile, 0xc00 per face
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

  // Allocate the screen's work blocks and reset the camera and tasks.
  Runtime_AllocateHeapBlock(HEAP_SLOT_BATTLE_SHEET, 0x60e);
  spritePalette = Runtime_AllocateHeapBlock(HEAP_SLOT_BATTLE_CANVAS, 0x200);
  diceWork = Runtime_AllocateBlock(HEAP_SLOT_BATTLE_EFFECT, sizeof(*diceWork));
  sprites = Runtime_AllocateBlock(45, 0x618);
  camera = Runtime_AllocateBlock(HEAP_SLOT_CAMERA, sizeof(*camera));
  RuntimeDispatch_NoOpHook((s32)&ResourceId_EffectFarCalls);
  Camera_ResetSceneDefaults();
  Scheduler_ResetTaskTable();
  gOamCopyEnabled = 0;

  // Fill BG1's map (screen block 6) with a 30-tile-wide picture, then load
  // the board and dice graphics with their palettes faded to black.
  index = 0;
  for (tileY = 0; tileY != 20; tileY++) {
    for (tileX = 0; tileX != 32; tileX++) {
      *(volatile u16 *)(BG_SCREEN_BLOCK(6) + index) = tileX + tileY * 30;
      index += 2;
    }
  }

  resourceAddress =
      (s32)Resource_GetTableEntry((s32)&ResourceId_LuckyDiceBoard);
  Dma_Set((void *)resourceAddress, (void *)BG_PLTT,
          (DMA_ENABLE | DMA_32BIT) << 16 | 0x70, REG_DMA3);
  Dma_Set((void *)BG_PLTT, bgPalette, (DMA_ENABLE | DMA_32BIT) << 16 | 0x80,
          REG_DMA3);
  resourceAddress += 0x1c0;
  // UB: Value2 passes integers to the decoder's two pointer parameters.
  // Plain typed calls add four bytes to LuckyDice_Run in tbs-en.
#ifdef UBFIX
  Resource_DecodeType01((const void *)resourceAddress, Ram_MapCellBuffer);
#else
  Value2(Resource_DecodeType01, resourceAddress, (s32)Ram_MapCellBuffer);
#endif
  Dma_Set(Ram_MapCellBuffer, BG_SCREEN_BLOCK(8),
          (DMA_ENABLE | DMA_32BIT) << 16 | 0x2580, REG_DMA3);
  resourceAddress =
      (s32)Resource_GetTableEntry((s32)&ResourceId_LuckyDiceSprites);
  Dma_Set((void *)resourceAddress, spritePalette,
          (DMA_ENABLE | DMA_32BIT) << 16 | 0x80, REG_DMA3);
  Dma_Set((void *)resourceAddress, (void *)OBJ_PLTT,
          (DMA_ENABLE | DMA_32BIT) << 16 | 0x80, REG_DMA3);
  resourceAddress += 0x200;
#ifdef UBFIX
  Resource_DecodeType01((const void *)resourceAddress, Ram_MapCellBuffer);
#else
  Value2(Resource_DecodeType01, resourceAddress, (s32)Ram_MapCellBuffer);
#endif
  Dma_Set(Ram_MapCellBuffer, (void *)OBJ_VRAM0,
          (DMA_ENABLE | DMA_32BIT) << 16 | 0x1f00, REG_DMA3);
  Graphics_ScaleRgb555Buffer(spritePalette, (u16 *)OBJ_PLTT, 0, 256);
  Graphics_ScaleRgb555Buffer(bgPalette, (u16 *)BG_PLTT, 0, 256);

  // Show the board on BG1, sprites and windows, with the board blended.
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
  REG_BG1VOFS = 0xff60;
  REG_BG2HOFS = 0;
  REG_BG2VOFS = 0xff60;
  REG_WIN0H = WIN_RANGE(0, 240);
  REG_WIN0V = WIN_RANGE(0, 160);
  REG_WIN1H = WIN_RANGE(0, 240);
  REG_WIN1V = WIN_RANGE(0, 160);

  // Start aiming with both dice at the throwing line and two lucky numbers.
  state = LUCKY_DICE_AIM;
  resultTimer = 0;
  resultKind = -1;
  winnings = 0;
  payoutTimer = 0;
  // Unsigned arithmetic wraps before storage as signed fixed point. Signed
  // subtraction grows the English routine from 0x1274 to 0x1280 bytes.
  for (index = 0; index != DICE_COUNT; index++) {
    diceWork->particles[index].x = ((Random16() & 127) - 64) << 16;
    diceWork->particles[index].y = (Random16() & 31) << 16;
    diceWork->particles[index].z = ((Random16() & 127) - 64) << 16;
    diceWork->particles[index].velocityX = ((Random16() & 127) - 64) << 14;
    diceWork->particles[index].velocityY = ((Random16() & 31) + 32) << 12;
    diceWork->particles[index].velocityZ = ((Random16() & 127) - 64) << 14;
    diceWork->particles[index].variant = 0;
    spinRate[index] = 300;
  }
  numbers[0] = numbers[1] = numbers[2] = numbers[3] = 0;
  flashTimer[0] = flashTimer[1] = 0;
  luckyNumbers[0] = Random16() % DICE_FACE_COUNT;
  do {
    luckyNumbers[1] = Random16() % DICE_FACE_COUNT;
  } while (luckyNumbers[0] == luckyNumbers[1]);
  diceWork->particles[0].x = 0x280000;
  diceWork->particles[0].y = 0x600000;
  diceWork->particles[0].z = -0xa00000;
  diceWork->particles[1].x = -0x280000;
  diceWork->particles[1].y = 0x600000;
  diceWork->particles[1].z = -0xa00000;

  // Open the coins window and the Throw / Quit help.
  UiWork_InitializeWithResourceCountersFar();
  window = UiWindow_CreateFar(DICE_COIN_X, 0, DICE_COIN_W, 3, 6);
  sprites->coinsWindow = window;
  UiText_DrawCharacterAtOffsetFar(MsgLuckyDiceCoins, window, DICE_LABEL_X, 0);
  UiText_DrawNumberInWindowFar(gGameState.coins, 6, sprites->coinsWindow, 0, 0);
  window = UiWindow_CreateFar(DICE_HELP_X, 16, DICE_HELP_W, 4, 6);
  sprites->helpWindow = window;
  // MsgLuckyDiceThrow and MsgLuckyDiceQuit, by their offset from the coins
  // label: written by name, they would share literals the native keeps apart.
  UiText_DrawCharacterAtOffsetFar(MsgLuckyDiceCoins + 10, window, 0, 0);
  UiText_DrawCharacterAtOffsetFar(MsgLuckyDiceCoins + 11, sprites->helpWindow,
                                  0, 8);

  frame = 0;
  for (;;) {
    resetDice = 0;
    // Fade in over the first 16 frames.
    if (frame <= 16) {
      Graphics_ScaleRgb555Buffer(spritePalette, (u16 *)OBJ_PLTT, frame << 12,
                                 256);
      Graphics_ScaleRgb555Buffer(bgPalette, (u16 *)BG_PLTT, frame << 12, 256);
    }
    Render_ResetTransformState();
    Transform_SetLookAt(camera->eye, camera->pos);

    // The fixed sprites: the board frame and the two lucky numbers.
    spriteCount = 0;
    sprites->obj[spriteCount].attr01 = 0xc0006000;
    sprites->obj[spriteCount].attr2 = 0x358;
    spriteCount++;
    sprites->obj[spriteCount].attr01 = 0x40102020;
    sprites->obj[spriteCount].attr2 = 0x398;
    spriteCount++;
    sprites->obj[spriteCount].attr01 = 0x40102030;
    sprites->obj[spriteCount].attr2 = 0x3a0;
    spriteCount++;
    sprites->obj[spriteCount].attr01 = 0x40042020;
    sprites->obj[spriteCount].attr2 = 0x3a8;
    spriteCount++;
    sprites->obj[spriteCount].attr01 = 0x40042030;
    sprites->obj[spriteCount].attr2 = 0x3a8;
    spriteCount++;
    for (index = 0; index != DICE_COUNT; index++) {
      sprites->obj[spriteCount].attr01 = (32 + index * 16) | 0x40202000;
      sprites->obj[spriteCount].attr2 = luckyNumbers[index] * 8 + 0x3b0;
      spriteCount++;
    }

    // After a throw, reveal the four numbers five frames apart, then the
    // result's lettering.
    if (state == LUCKY_DICE_RESULT || state == LUCKY_DICE_PAYOUT) {
      for (index = 0; index != DICE_RESULT_COUNT; index++) {
        if (resultTimer == index * 5) {
          AudioCommand_PlayFar(SE_303);
        }
        if (resultTimer >= index * 5) {
          sprites->obj[spriteCount].attr01 =
              0x80002040 | ((56 + index * 32) << 16);
          sprites->obj[spriteCount].attr2 = numbers[index] * 32 + 152;
          spriteCount++;
        }
      }
      if (resultTimer > 19 && resultKind >= 0) {
        for (index = 0; index != gLuckyDice_LabelCounts[resultKind]; index++) {
          s32 labelX;
          s32 labelY;

          labelX = DICE_KIND_X - gLuckyDice_LabelCounts[resultKind] * 16 +
                   index * 32;
          labelY = 96;
          sprites->obj[spriteCount].attr01 =
              (((labelX & 0x1ff) << 16) | labelY) | 0x80002000;
          sprites->obj[spriteCount].attr2 =
              gLuckyDice_LabelTiles[resultKind] * 8 + 152 + index * 32;
          spriteCount++;
        }
      }
    }

    // Showing the result: twenty frames in, pay the winnings and open their
    // window; A then moves on to the payout, or back to aiming after a loss.
    if (state == LUCKY_DICE_RESULT) {
      if (resultTimer == 20 && winnings > 0) {
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
        window = UiWindow_CreateFar(DICE_WIN_X - digits / 2, 16,
                                    digits + DICE_WIN_W, 3, 6);
        sprites->winningsWindow = window;
        UiText_DrawCharacterAtOffsetFar(MsgLuckyDiceWinnings, window, 0, 0);
        // "Coins" after the number (MsgLuckyDiceCoins, one before).
        UiText_DrawCharacterAtOffsetFar(MsgLuckyDiceWinnings - 1,
                                        sprites->winningsWindow,
                                        digits * 8 + DICE_WIN_LABEL_X, 0);
        UiText_DrawNumberInWindowFar(winnings, digits, sprites->winningsWindow,
                                     DICE_WIN_NUM_X, 0);
      }
      if (payoutTimer > 0) {
        payoutTimer--;
      }
      if (resultTimer > 19 && (gKeysRepeat & KEY_A)) {
        AudioCommand_PlayFar(SE_MENU_CONFIRM);
        if (winnings > 0) {
          state = LUCKY_DICE_PAYOUT;
          payoutTimer = 0;
        } else {
          resetDice = 1;
          state = LUCKY_DICE_AIM;
#if TBS_PATCH_1
          window = UiWindow_CreateFar(DICE_HELP_X, 16, DICE_HELP_W, 4, 6);
          sprites->helpWindow = window;
          UiText_DrawCharacterAtOffsetFar(MsgLuckyDiceThrow, window, 0, 0);
          UiText_DrawCharacterAtOffsetFar(MsgLuckyDiceThrow + 1,
                                          sprites->helpWindow, 0, 8);
#endif
        }
      }
      resultTimer++;
    } else if (state == LUCKY_DICE_PAYOUT) {
      // Clear the winnings into the coins, then after 15 frames close their
      // window and aim again.
      if (payoutTimer == 0) {
        winnings = 0;
        UiText_DrawNumberInWindowFar(0, digits, sprites->winningsWindow, 40, 0);
        UiText_DrawNumberInWindowFar(gGameState.coins, 6, sprites->coinsWindow,
                                     0, 0);
      }
      if (payoutTimer == 15) {
        state = LUCKY_DICE_AIM;
        UiWork_FinalizeFar(sprites->winningsWindow, 1);
        window = UiWindow_CreateFar(DICE_HELP_X, 16, DICE_HELP_W, 4, 6);
        sprites->helpWindow = window;
        UiText_DrawCharacterAtOffsetFar(MsgLuckyDiceThrow, window, 0, 0);
        UiText_DrawCharacterAtOffsetFar(MsgLuckyDiceThrow + 1,
                                        sprites->helpWindow, 0, 8);
        resetDice = 1;
      }
      payoutTimer++;
    } else if (state == LUCKY_DICE_AIM && frame > 17) {
      // Aiming: Up and Down raise and lower the dice between 32 and 160.
      // B quits, as does running out of coins for a bet; A throws.
      // DEAD: gKeysRepeat is read twice and discarded before the KEY_UP test;
      // both loads are in the ROM.
      (void)gKeysRepeat;
      (void)gKeysRepeat;
      if (gKeysHeld & KEY_UP) {
        diceWork->particles[0].y += 0x40000;
        diceWork->particles[1].y += 0x40000;
      }
      if (gKeysHeld & KEY_DOWN) {
        diceWork->particles[0].y += -0x40000;
        diceWork->particles[1].y += -0x40000;
      }
      if (diceWork->particles[0].y < 0x200000) {
        diceWork->particles[0].y = 0x200000;
      }
      if (diceWork->particles[0].y > 0xa00000) {
        diceWork->particles[0].y = 0xa00000;
      }
      if (diceWork->particles[1].y < 0x200000) {
        diceWork->particles[1].y = 0x200000;
      }
      if (diceWork->particles[1].y > 0xa00000) {
        diceWork->particles[1].y = 0xa00000;
      }
      if (gGameState.coins <
          (u32)(Party_GetAverageLevelFar() * DICE_BET_PER_LEVEL)) {
        UiWork_FinalizeFar(sprites->helpWindow, 1);
        break;
      }
      if (gKeysRepeat & KEY_B) {
        AudioCommand_PlayFar(SE_MENU_CANCEL);
        UiWork_FinalizeFar(sprites->helpWindow, 1);
        break;
      }
      if (gKeysRepeat & KEY_A) {
        AudioCommand_PlayFar(SE_302);
        UiWork_FinalizeFar(sprites->helpWindow, 1);
        state = LUCKY_DICE_ROLL;
        bet = Party_GetAverageLevelFar() * DICE_BET_PER_LEVEL;
        if ((u32)bet > gGameState.coins) {
          bet = gGameState.coins;
        }
        Party_AdjustCoinsFar(-bet);
        UiText_DrawNumberInWindowFar(gGameState.coins, 6, sprites->coinsWindow,
                                     0, 0);
        for (index = 0; index != DICE_COUNT; index++) {
          rollingDie = &diceWork->particles[index];
          // DEAD: never true with DICE_COUNT 2; a third die would start
          // further back and higher up.
          if (index > 1) {
            rollingDie->z = -0xb40000;
            rollingDie->y = 0x600000;
          }
          SET_VELOCITY(rollingDie, ((Random16() & 127) - 64) << 12,
                       ((Random16() & 15) + 48) << 13,
                       (((Random16() & 63) + 140) << 12) + rollingDie->y / 6);
          rollingDie->variant = Random16() % 0x4800;
          spinRate[index] = 300;
        }
      }
    }

    // Back to the throwing line, with two new lucky numbers.
    if (resetDice == 1) {
      diceWork->particles[0].x = 0x280000;
      diceWork->particles[0].y = 0x600000;
      diceWork->particles[0].z = -0xa00000;
      diceWork->particles[1].x = -0x280000;
      diceWork->particles[1].y = 0x600000;
      diceWork->particles[1].z = -0xa00000;
      luckyNumbers[0] = Random16() % DICE_FACE_COUNT;
      do {
        luckyNumbers[1] = Random16() % DICE_FACE_COUNT;
      } while (luckyNumbers[0] == luckyNumbers[1]);
    }

    // Draw the dice back to front (an insertion sort on depth), each scaled
    // by its distance.
    for (index = 0; index != DICE_COUNT; index++) {
      drawOrder[index] = index;
    }
    for (index = 1; index < DICE_COUNT; index++) {
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
      // Projection reads the record's three coordinate words directly.
      // Copying them to an array grows native EN from 0x1274 to 0x1294 bytes.
      Render_ProjectPoint(rollingDie, screen);
      // Depth is clamped to 350..650; the nearer, the larger the die.
      if (screen[2] < 0x15e) {
        screen[2] = 0x15e;
      }
      if (screen[2] > 0x28a) {
        screen[2] = 0x28a;
      }
      spriteDepth[drawOrder[index]] = screen[2] - 0x15e;
      halfSize = 0x800 / (spriteDepth[drawOrder[index]] / 2 + 128);
      dieX = screen[0] - halfSize;
      dieY = screen[1] - halfSize;
      // An affine, double-size, 256-colour sprite using the die's own
      // affine matrix (attr1 bits 9 to 13); 0x278 and 0x160 wrap in OAM's 9-
      // and 8-bit fields to 120 across and 96 down.
      if (dieY + 96 > -16) {
        sprites->obj[spriteCount].attr01 =
            ((drawOrder[index] << 25) | (((dieX + 0x278) & 0x1ff) << 16) |
             ((dieY + 0x160) & 255)) |
            0x40002300;
        sprites->obj[spriteCount].attr2 =
            (gLuckyDice_FaceTiles[rollingDie->variant / 1024] << 3) | 0x400;
        spriteCount++;
      }
    }

    // While aiming, a marker under each die.
    if (state == LUCKY_DICE_AIM) {
      for (index = 0; index != DICE_COUNT; index++) {
        Render_ProjectPoint(&diceWork->particles[index], screen);
        x = screen[0] - 0x800 / (spriteDepth[drawOrder[index]] / 2 + 128);
        y = screen[1] + 0x300 / (spriteDepth[drawOrder[index]] / 2 + 128);
        sprites->obj[spriteCount].attr01 =
            ((((x + 0x278) & 0x1ff) << 16) | ((y + 0x160) & 255)) | 0x80002000;
        if (index == 1) {
          sprites->obj[spriteCount].attr01 |= 0x10000000;
        }
        sprites->obj[spriteCount].attr2 = 0x958;
        spriteCount++;
      }
    }

    for (index = 0; index != DICE_COUNT; index++) {
      s32 shadowX;
      s32 shadowY;

      // Each die's shadow on the board below it.
      SET_VECTOR(groundPoint, diceWork->particles[index].x, 0,
                 diceWork->particles[index].z);
      Render_ProjectPoint(groundPoint, screen);
      // A shadow shrinks as its die rises.
      if (screen[2] < 0x15e) {
        screen[2] = 0x15e;
      }
      if (screen[2] > 0x28a) {
        screen[2] = 0x28a;
      }
      spriteDepth[2 + index] = screen[2] - 0x15e;
      spriteDepth[2 + index] += diceWork->particles[index].y >> 16;
      shadowX = screen[0];
      shadowY = screen[1];
      if (shadowY + 88 > -16) {
        sprites->obj[spriteCount].attr01 =
            (((index + 2) << 25) | (((shadowX + 0x270) & 0x1ff) << 16)) |
            ((shadowY + 0x158) & 255) | 0x40002300;
        sprites->obj[spriteCount].attr2 = 0x890;
        spriteCount++;
      }
    }

    // Rolling: move, collide and settle each die.
    if (state == LUCKY_DICE_ROLL) {
      s32 stoppedDice;

      stoppedDice = 0;
      for (index = 0; index != DICE_COUNT; index++) {
        rollingDie = &diceWork->particles[index];
        // Move, and push the dice apart when they touch (closer than 50).
        rollingDie->x += rollingDie->velocityX;
        rollingDie->y += rollingDie->velocityY;
        rollingDie->z += rollingDie->velocityZ;
        for (otherIndex = 0; otherIndex != DICE_COUNT; otherIndex++) {
          otherDie = &diceWork->particles[otherIndex];
          if (index != otherIndex) {
            separationX = (rollingDie->x - otherDie->x) >> 16;
            separationY = (rollingDie->y - otherDie->y) >> 16;
            separationZ = (rollingDie->z - otherDie->z) >> 16;
            if (separationX * separationX + separationY * separationY +
                    separationZ * separationZ <=
                0x9c3) {
              separation = Iwram_Sqrt(separationX * separationX +
                                      separationY * separationY +
                                      separationZ * separationZ);
// UB: signed separation components may be negative; touching
// dice are at most 49 pixels apart, so multiplication fits.
// Coincident dice also need to avoid division by zero.
#ifdef UBFIX
              if (separation != 0) {
                rollingDie->velocityX += separationX * 32768 / separation;
                rollingDie->velocityY += separationY * 32768 / separation;
                rollingDie->velocityZ += separationZ * 32768 / separation;
              }
#else
              rollingDie->velocityX += (separationX << 15) / separation;
              rollingDie->velocityY += (separationY << 15) / separation;
              rollingDie->velocityZ += (separationZ << 15) / separation;
#endif
            }
          }
        }
        // Bounce off the board's sides and back wall.
        if (rollingDie->x < -0xc80000) {
          rollingDie->x = -0xc80000;
          rollingDie->velocityX = -rollingDie->velocityX * 48 / 64;
        }
        if (rollingDie->x > 0xc80000) {
          rollingDie->x = 0xc80000;
          rollingDie->velocityX = -rollingDie->velocityX * 48 / 64;
        }
        if (rollingDie->z < -0x960000) {
          rollingDie->z = -0x960000;
          rollingDie->velocityZ = -rollingDie->velocityZ * 48 / 64;
        }
        if (rollingDie->z > rollingDie->y + 0xb40000) {
          rollingDie->z = rollingDie->y + 0xb40000;
          if (rollingDie->velocityZ > 0) {
            rollingDie->velocityY += rollingDie->velocityZ / 2;
            rollingDie->velocityZ = -rollingDie->velocityZ * 48 / 64;
          }
        }
        // Gravity; on the board a die bounces, loses speed, sometimes kicks
        // off in a random direction, and spins less. Once it barely bounces
        // it stops, turned to the nearest face.
        rollingDie->velocityY += -0x8000;
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
          if (spinRate[index] > 19) {
            spinRate[index] -= 20;
          }
          if (rollingDie->velocityY < 0x30000) {
            SET_VELOCITY(rollingDie, 0, 0, 0);
            spinRate[index] = 0;
            rollAngle = rollingDie->variant;
            faceTile = rollAngle / 1024 % 3;
            if (faceTile == 1) {
              rollingDie->variant = rollAngle + 0x800;
            } else if (faceTile == 2) {
              rollingDie->variant = rollAngle + 0x400;
            }
          }
        }
        SET_VELOCITY(rollingDie, rollingDie->velocityX * 63 / 64,
                     rollingDie->velocityY * 63 / 64,
                     rollingDie->velocityZ * 63 / 64);
        // The roll angle wraps at six faces of 0xc00.
        rollingDie->variant += spinRate[index];
        if (rollingDie->variant > 0x47ff) {
          rollingDie->variant -= 0x4800;
        }
        if (spinRate[index] > 0) {
          spinRate[index]--;
        }
        // At rest: read the face from the roll and the row from the depth.
        if (rollingDie->velocityX == 0 && rollingDie->velocityY == 0 &&
            rollingDie->velocityZ == 0 && rollingDie->y == 0) {
          numbers[index] = rollingDie->variant / 0xc00;
          for (otherIndex = 0; otherIndex != DICE_FACE_COUNT; otherIndex++) {
            // Row boundaries include negative signed halfwords.
            s32 rowDepth;

#ifdef UBFIX
            rowDepth = gLuckyDice_FaceHeights[otherIndex] * 65536;
#else
            rowDepth = gLuckyDice_FaceHeights[otherIndex] << 16;
#endif
            if (rollingDie->z > rowDepth) {
              numbers[index + 2] = otherIndex;
              break;
            }
          }
          stoppedDice++;
        }
      }

      // Both dice stopped: score the four numbers.
      if (stoppedDice == DICE_COUNT) {
        state = LUCKY_DICE_RESULT;
        resultTimer = 0;
        resultKind = -1;
        winnings = 0;
        payoutTimer = 60;
        if (numbers[0] == numbers[1] && numbers[0] == numbers[2] &&
            numbers[0] == numbers[3]) {
          flashTimer[0] = flashTimer[1] = 120;
          resultKind = 0;
          if (numbers[0] == luckyNumbers[0]) {
            winnings = bet * 15;
          } else if (numbers[0] == luckyNumbers[1]) {
            winnings = bet * 25;
          } else {
            winnings = bet * 5;
          }
          UiText_DrawNumberInWindowFar(gGameState.coins, 6,
                                       sprites->coinsWindow, 0, 0);
          AudioCommand_PlayFar(MUS_093);
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
            UiText_DrawNumberInWindowFar(gGameState.coins, 6,
                                         sprites->coinsWindow, 0, 0);
            resultKind = 3;
          }
          if (pairCount == 2) {
            winnings = bet * 2;
            UiText_DrawNumberInWindowFar(gGameState.coins, 6,
                                         sprites->coinsWindow, 0, 0);
            flashTimer[Random16() & 1] = 60;
            resultKind = 2;
            AudioCommand_PlayFar(MUS_091);
          }
          if (pairCount == 3) {
            winnings = bet * 3;
            UiText_DrawNumberInWindowFar(gGameState.coins, 6,
                                         sprites->coinsWindow, 0, 0);
            flashTimer[Random16() & 1] = 60;
            resultKind = 1;
            AudioCommand_PlayFar(MUS_092);
          }
        }
      }
    }

    // Hide the unused sprites, write the four affine matrices (one per die
    // and shadow, scaled by depth) and copy the sprites to OAM.
    // An unused sprite is parked at x 240, just past the right edge.
    for (; spriteCount != 128; spriteCount++) {
      sprites->obj[spriteCount].attr01 = 0x40f02000;
      sprites->obj[spriteCount].attr2 = 0;
    }

    // OAM keeps affine matrix n in the fourth halfwords of entries 4n to
    // 4n + 3, which attr2's high half covers here.
    for (index = 0; index != DICE_RESULT_COUNT; index++) {
      affineScale = spriteDepth[index] / 2 + 128;
      affineA = affineScale;
      affineB = 0;
      affineC = 0;
      affineD = affineScale;
      sprites->obj[index * 4 + 0].attr2 |= affineA << 16;
      sprites->obj[index * 4 + 1].attr2 |= affineB << 16;
      sprites->obj[index * 4 + 2].attr2 |= affineC << 16;
      sprites->obj[index * 4 + 3].attr2 |= affineD << 16;
    }

    Dma_Set(sprites->obj, OAM, (DMA_ENABLE | DMA_32BIT) << 16 | 0x100,
            REG_DMA3);
    WaitFrames(1);
    frame++;
  }

  // Fade out over 17 frames and free the work blocks.
  for (index = 0; index != 17; index++) {
    scale = 0x10000 - (index << 12);
    Graphics_ScaleRgb555Buffer(spritePalette, (u16 *)OBJ_PLTT, scale, 256);
    Graphics_ScaleRgb555Buffer(bgPalette, (u16 *)BG_PLTT, scale, 256);
    WaitFrames(1);
  }
  Runtime_ReleaseHeapBlock(HEAP_SLOT_CAMERA);
  Runtime_ReleaseHeapBlock(45);
  Runtime_ReleaseHeapBlock(HEAP_SLOT_BATTLE_CANVAS);
  Runtime_ReleaseHeapBlock(HEAP_SLOT_BATTLE_EFFECT);
  Runtime_ReleaseHeapBlock(HEAP_SLOT_BATTLE_SHEET);
}
