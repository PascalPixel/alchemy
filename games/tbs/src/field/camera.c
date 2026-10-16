#include "unit/field/camera.h"
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
#include "fixed_point_position.h"
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

// Starts the Lucky Dice minigame: leaves only the backdrop on screen, sends
// sound command 9 and plays until the player quits. Returns 0.
s32 Runtime_BlankDisplayAndRun(void) {
  REG_DISPCNT = DISPCNT_OBJ_1D_MAP;
  AudioCommand_PlayFar(9);
  LuckyDice_Run();
  return 0;
}

// Puts the 3D camera back at its scene defaults: at the origin, unturned,
// pitched to 152 and 510 units away, following nothing. Recomputes the eye
// and sets the projection's focal length (250), near plane and far plane.
void Camera_ResetSceneDefaults(void) {
  struct BattleCamera *camera = gCameraWork;
  struct FixedPointPosition offset;
  u32 nearPlane;
  u32 focalLength;

  camera->pitch = 152 << 8;
  camera->distance = 255 << 17;
  camera->pos[0] = 0;
  camera->pos[1] = 0;
  camera->pos[2] = 0;
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

  offset.x = 0;
  offset.y = 0;
  offset.z = camera->distance;
  Iwram_TransformVector((s32 *)&offset, (s32 *)camera);

  focalLength = 250;
  focalLength <<= 16;
  nearPlane = Iwram_RatioMulQ14(focalLength, 192 << 8);

  focalLength = 250;
  focalLength <<= 16;
  Projection_Set(focalLength, nearPlane, 0x7fff0000);
}

// Multiplies two 8.8 fixed-point values, rounding toward zero.
s32 FixedPoint_Multiply8_8(s32 a, s32 b) {
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

// Divides two 8.8 fixed-point values.
s16 Math_ScaleByRatio(s16 numerator, s16 denominator) {
  return (numerator << 8) / denominator;
}

// The reciprocal of an 8.8 fixed-point value.
s16 FixedPoint_Reciprocal(s16 value) { return 0x10000 / value; }

#if !GAME_TLA
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
#include "sound_ids.h"

// Lucky Dice window layout, in tiles (and pixels for text), by edition: the
// coins window at the top, the help window (Throw / Quit) and the winnings
// window at the bottom, and the result lettering's x in sprite coordinates.
#if defined(TBS_EDITION_DE) || defined(TBS_EDITION_ES) ||                      \
    defined(TBS_EDITION_FR)
#define DICE_COIN_X 17
#define DICE_COIN_W 13
#define DICE_WIN_W 13
#elif EDITION_INTERNATIONAL
#define DICE_COIN_X 18
#define DICE_COIN_W 12
#define DICE_WIN_W 12
#else
#define DICE_COIN_X 18
#define DICE_COIN_W 11
#define DICE_WIN_W 10
#endif

#if defined(TBS_EDITION_ES)
#define DICE_LABEL_X 56
#elif defined(TBS_EDITION_FR)
#define DICE_LABEL_X 53
#else
#define DICE_LABEL_X 48
#endif

#if defined(TBS_EDITION_DE) || defined(TBS_EDITION_ES)
#define DICE_HELP_X 20
#define DICE_HELP_W 10
#elif defined(TBS_EDITION_FR)
#define DICE_HELP_X 21
#define DICE_HELP_W 9
#elif EDITION_INTERNATIONAL
#define DICE_HELP_X 22
#define DICE_HELP_W 8
#else
#define DICE_HELP_X 23
#define DICE_HELP_W 7
#endif

#if EDITION_INTERNATIONAL
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

// The Lucky Dice screen's work block (heap slot 45): its sprites and windows.
struct LuckyDiceSprites {
  u8 filler[200];                  // Lucky Dice never touches these bytes
  struct OamShadow obj[128];       // copied to OAM each frame
  struct UiWindow *coinsWindow;    // the player's coins
  struct UiWindow *winningsWindow; // the winnings of the last throw
  struct UiWindow *helpWindow;     // the Throw and Quit buttons
};

#include "keystate.h"
#include "../../../common/src/graphics/lucky_dice/tables.h"

// What the Lucky Dice screen is doing.
enum LuckyDiceState {
  LUCKY_DICE_AIM,    // the dice wait to be thrown; Up and Down raise them
  LUCKY_DICE_ROLL,   // the dice fly, bounce and settle
  LUCKY_DICE_RESULT, // the four numbers and the result are shown
  LUCKY_DICE_PAYOUT  // the winnings move into the coins
};

#endif

// Copies count RGB555 colours from source to destination with each channel
// multiplied by scale (16.16: 0 is black, 0x10000 unchanged), for fades.
// Returns 0.
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

#if !GAME_TLA
// Runs the Lucky Dice minigame until the player quits or cannot afford a
// throw. Each throw costs ten coins per level of the party's average. Two
// dice are thrown onto a board of six rows; each die's face and the row it
// stops in give four numbers. Four of a kind pays 5 times the bet (15 or 25
// times when it matches one of the two lucky numbers shown at the top),
// three of a kind 3 times, two pairs twice and one pair the bet back.
void LuckyDice_Run(void) {
  u16 bgPalette[256];
  s32 spriteDepth[4]; // the two dice, then their shadows
  s32 numbers[4];     // the two faces, then the two rows the dice stopped in
  s32 groundPoint[3];
  s32 screen[3];
  /* FAKEMATCH: unused local; the native frame keeps 32 unused bytes at
   * sp+168..200, between screen and the affine halfwords (without it 42) */
  s32 unused[8];
  /* FAKEMATCH: volatile restores the form the bytes prove, four halfwords
   * stored to sp+166..160 and each reloaded with ldrh (without it 902) */
  volatile u16 affineA;
  volatile u16 affineB;
  volatile u16 affineC;
  volatile u16 affineD;
  s32 spinRate[2];
  s32 flashTimer[2];     // set on a win; nothing here reads it
  s32 luckyNumbers[2];   // the two numbers shown at the top, never equal
  s32 drawOrder[2];      // the dice, back to front
  s32 matchedNumbers[2]; // written, never read
  u16 *spritePalette;
  struct BattleEffectWork *diceWork;
  /* FAKEMATCH: the OAM shadow it builds is struct OamShadow, volatile words
   * (tag in oambuf.h): without the qualifier differ 1702. */
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
  struct EffectStep *die;
  struct EffectStep *other;
  s32 resetDice; // put the dice back at the throwing line
  struct UiWindow *window;
  s32 i;
  s32 j;
  s32 roll; // a die's roll angle: 0x400 per face tile, 0xc00 per face
  s32 x;
  s32 y;
  s32 halfSize;
  s32 faceStep; // which of a face's three tiles the roll shows
  s32 key;
  s32 tx;
  s32 ty;
  s32 resource;
  s32 dx;
  s32 dy;
  s32 dz;
  s32 dist;
  s32 speed;
  u32 angle;
  s32 scale;
  s32 pairs;
  s16 affineScale;

  // Allocate the screen's work blocks and reset the camera and tasks.
  Runtime_AllocateHeapBlock(41, 0x60e);
  spritePalette = Runtime_AllocateHeapBlock(40, 0x200);
  diceWork = Runtime_AllocateBlock(39, 0x782c);
  sprites = Runtime_AllocateBlock(45, 0x618);
  camera = Runtime_AllocateBlock(12, 76);
  RuntimeDispatch_NoOpHook((s32)&ResourceId_EffectFarCalls);
  Camera_ResetSceneDefaults();
  Scheduler_ResetTaskTable();
  gOamCopyEnabled = 0;

  // Fill BG1's map (screen block 6) with a 30-tile-wide picture, then load
  // the board and dice graphics with their palettes faded to black.
  i = 0;
  for (ty = 0; ty != 20; ty++) {
    for (tx = 0; tx != 32; tx++) {
      *(volatile u16 *)(0x06003000 + i) = tx + ty * 30;
      i += 2;
    }
  }

  resource = (s32)Resource_GetTableEntry((s32)&ResourceId_LuckyDiceBoard);
  Dma_Set((void *)resource, (void *)0x05000000, 0x84000070, REG_DMA3);
  Dma_Set((void *)0x05000000, bgPalette, 0x84000080, REG_DMA3);
  resource += 0x1c0;
  Value2(Resource_DecodeType01, resource, (s32)Ram_MapCellBuffer);
  Dma_Set(Ram_MapCellBuffer, (void *)0x06004000, 0x84002580, REG_DMA3);
  resource = (s32)Resource_GetTableEntry((s32)&ResourceId_LuckyDiceSprites);
  Dma_Set((void *)resource, spritePalette, 0x84000080, REG_DMA3);
  Dma_Set((void *)resource, (void *)0x05000200, 0x84000080, REG_DMA3);
  resource += 0x200;
  Value2(Resource_DecodeType01, resource, (s32)Ram_MapCellBuffer);
  Dma_Set(Ram_MapCellBuffer, (void *)0x06010000, 0x84001f00, REG_DMA3);
  Graphics_ScaleRgb555Buffer(spritePalette, (u16 *)0x05000200, 0, 256);
  Graphics_ScaleRgb555Buffer(bgPalette, (u16 *)0x05000000, 0, 256);

  // Show the board on BG1, sprites and windows, with the board blended.
  REG_BG1CNT = 0x0686;
  REG_WININ = 0x3737;
  REG_WINOUT = 0x2723;
  REG_DISPCNT = DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON | DISPCNT_BG1_ON |
                DISPCNT_OBJ_ON | DISPCNT_WIN0_ON;
  REG_BLDCNT = 0x3f44;
  REG_BLDALPHA = 0x0810;
  gBgScroll[1].x = 0;
  gBgScroll[1].y = 0;
  REG_BG1HOFS = 0;
  REG_BG1VOFS = 0xff60;
  REG_BG2HOFS = 0;
  REG_BG2VOFS = 0xff60;
  REG_WIN0H = 0xf0;
  REG_WIN0V = 0xa0;
  REG_WIN1H = 0xf0;
  REG_WIN1V = 0xa0;

  // Start aiming with both dice at the throwing line and two lucky numbers.
  state = LUCKY_DICE_AIM;
  resultTimer = 0;
  resultKind = -1;
  winnings = 0;
  payoutTimer = 0;
  for (i = 0; i != 2; i++) {
    diceWork->particles[i].x = ((Random16() & 127) - 64) << 16;
    diceWork->particles[i].y = (Random16() & 31) << 16;
    diceWork->particles[i].z = ((Random16() & 127) - 64) << 16;
    diceWork->particles[i].velocityX = ((Random16() & 127) - 64) << 14;
    diceWork->particles[i].velocityY = ((Random16() & 31) + 32) << 12;
    diceWork->particles[i].velocityZ = ((Random16() & 127) - 64) << 14;
    diceWork->particles[i].variant = 0;
    spinRate[i] = 300;
  }
  numbers[0] = numbers[1] = numbers[2] = numbers[3] = 0;
  flashTimer[0] = flashTimer[1] = 0;
  luckyNumbers[0] = Random16() % 6;
  do {
    luckyNumbers[1] = Random16() % 6;
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
      Graphics_ScaleRgb555Buffer(spritePalette, (u16 *)0x05000200, frame << 12,
                                 256);
      Graphics_ScaleRgb555Buffer(bgPalette, (u16 *)0x05000000, frame << 12,
                                 256);
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
    for (i = 0; i != 2; i++) {
      sprites->obj[spriteCount].attr01 = (32 + i * 16) | 0x40202000;
      sprites->obj[spriteCount].attr2 = luckyNumbers[i] * 8 + 0x3b0;
      spriteCount++;
    }

    // After a throw, reveal the four numbers five frames apart, then the
    // result's lettering.
    if (state == LUCKY_DICE_RESULT || state == LUCKY_DICE_PAYOUT) {
      for (i = 0; i != 4; i++) {
        if (resultTimer == i * 5) {
          AudioCommand_PlayFar(0x12f);
        }
        if (resultTimer >= i * 5) {
          sprites->obj[spriteCount].attr01 = 0x80002040 | ((56 + i * 32) << 16);
          sprites->obj[spriteCount].attr2 = numbers[i] * 32 + 152;
          spriteCount++;
        }
      }
      if (resultTimer > 19 && resultKind >= 0) {
        for (i = 0; i != gLuckyDice_LabelCounts[resultKind]; i++) {
          s32 kx;
          s32 ky;

          kx = DICE_KIND_X - gLuckyDice_LabelCounts[resultKind] * 16 + i * 32;
          ky = 96;
          sprites->obj[spriteCount].attr01 =
              (((kx & 0x1ff) << 16) | ky) | 0x80002000;
          sprites->obj[spriteCount].attr2 =
              gLuckyDice_LabelTiles[resultKind] * 8 + 152 + i * 32;
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
        AudioCommand_PlayFar(SOUND_MENU_CONFIRM);
        if (winnings > 0) {
          state = LUCKY_DICE_PAYOUT;
          payoutTimer = 0;
        } else {
          resetDice = 1;
          state = LUCKY_DICE_AIM;
#if EDITION_INTERNATIONAL
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
      /* FAKEMATCH: two discarded reads of gKeysRepeat, the form the bytes prove
       * (native loads it twice before the KEY_UP test; without them 189) */
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
      if (gGameState.coins < (u32)(Party_GetAverageLevelFar() * 10)) {
        UiWork_FinalizeFar(sprites->helpWindow, 1);
        break;
      }
      if (gKeysRepeat & KEY_B) {
        AudioCommand_PlayFar(SOUND_MENU_CANCEL);
        UiWork_FinalizeFar(sprites->helpWindow, 1);
        break;
      }
      if (gKeysRepeat & KEY_A) {
        AudioCommand_PlayFar(0x12e);
        UiWork_FinalizeFar(sprites->helpWindow, 1);
        state = LUCKY_DICE_ROLL;
        bet = Party_GetAverageLevelFar() * 10;
        if ((u32)bet > gGameState.coins) {
          bet = gGameState.coins;
        }
        Party_AdjustCoinsFar(-bet);
        UiText_DrawNumberInWindowFar(gGameState.coins, 6, sprites->coinsWindow,
                                     0, 0);
        for (i = 0; i != 2; i++) {
          die = &diceWork->particles[i];
          if (i > 1) { // never true for two dice
            die->z = -0xb40000;
            die->y = 0x600000;
          }
          die->velocityX = ((Random16() & 127) - 64) << 12;
          die->velocityY = ((Random16() & 15) + 48) << 13;
          die->velocityZ = (((Random16() & 63) + 140) << 12) + die->y / 6;
          die->variant = Random16() % 0x4800;
          spinRate[i] = 300;
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
      luckyNumbers[0] = Random16() % 6;
      do {
        luckyNumbers[1] = Random16() % 6;
      } while (luckyNumbers[0] == luckyNumbers[1]);
    }

    // Draw the dice back to front (an insertion sort on depth), each scaled
    // by its distance.
    for (i = 0; i != 2; i++) {
      drawOrder[i] = i;
    }
    for (i = 1; i < 2; i++) {
      key = drawOrder[i];
      for (j = i - 1; j >= 0 && diceWork->particles[drawOrder[j]].z >
                                    diceWork->particles[key].z;
           j--) {
        drawOrder[j + 1] = drawOrder[j];
      }
      drawOrder[j + 1] = key;
    }

    for (i = 0; i != 2; i++) {
      s32 px;
      s32 py;

      die = &diceWork->particles[drawOrder[i]];
      Render_ProjectPoint(die, screen);
      if (screen[2] < 0x15e) {
        screen[2] = 0x15e;
      }
      if (screen[2] > 0x28a) {
        screen[2] = 0x28a;
      }
      spriteDepth[drawOrder[i]] = screen[2] - 0x15e;
      halfSize = 0x800 / (spriteDepth[drawOrder[i]] / 2 + 128);
      px = screen[0] - halfSize;
      py = screen[1] - halfSize;
      if (py + 96 > -16) {
        sprites->obj[spriteCount].attr01 =
            ((drawOrder[i] << 25) | (((px + 0x278) & 0x1ff) << 16) |
             ((py + 0x160) & 255)) |
            0x40002300;
        sprites->obj[spriteCount].attr2 =
            (gLuckyDice_FaceTiles[die->variant / 1024] << 3) | 0x400;
        spriteCount++;
      }
    }

    // While aiming, a marker under each die.
    if (state == LUCKY_DICE_AIM) {
      for (i = 0; i != 2; i++) {
        Render_ProjectPoint(&diceWork->particles[i], screen);
        x = screen[0] - 0x800 / (spriteDepth[drawOrder[i]] / 2 + 128);
        y = screen[1] + 0x300 / (spriteDepth[drawOrder[i]] / 2 + 128);
        sprites->obj[spriteCount].attr01 =
            ((((x + 0x278) & 0x1ff) << 16) | ((y + 0x160) & 255)) | 0x80002000;
        if (i == 1) {
          sprites->obj[spriteCount].attr01 |= 0x10000000;
        }
        sprites->obj[spriteCount].attr2 = 0x958;
        spriteCount++;
      }
    }

    for (i = 0; i != 2; i++) {
      s32 sx;
      s32 sy;

      // Each die's shadow on the board below it.
      groundPoint[0] = diceWork->particles[i].x;
      groundPoint[1] = 0;
      groundPoint[2] = diceWork->particles[i].z;
      Render_ProjectPoint(groundPoint, screen);
      if (screen[2] < 0x15e) {
        screen[2] = 0x15e;
      }
      if (screen[2] > 0x28a) {
        screen[2] = 0x28a;
      }
      spriteDepth[2 + i] = screen[2] - 0x15e;
      spriteDepth[2 + i] += diceWork->particles[i].y >> 16;
      sx = screen[0];
      sy = screen[1];
      if (sy + 88 > -16) {
        sprites->obj[spriteCount].attr01 =
            (((i + 2) << 25) | (((sx + 0x270) & 0x1ff) << 16)) |
            ((sy + 0x158) & 255) | 0x40002300;
        sprites->obj[spriteCount].attr2 = 0x890;
        spriteCount++;
      }
    }

    // Rolling: move, collide and settle each die.
    if (state == LUCKY_DICE_ROLL) {
      s32 stopped;

      stopped = 0;
      for (i = 0; i != 2; i++) {
        die = &diceWork->particles[i];
        // Move, and push the dice apart when they touch (closer than 50).
        die->x += die->velocityX;
        die->y += die->velocityY;
        die->z += die->velocityZ;
        for (j = 0; j != 2; j++) {
          other = &diceWork->particles[j];
          if (i != j) {
            dx = (die->x - other->x) >> 16;
            dy = (die->y - other->y) >> 16;
            dz = (die->z - other->z) >> 16;
            if (dx * dx + dy * dy + dz * dz <= 0x9c3) {
              dist = Iwram_Sqrt(dx * dx + dy * dy + dz * dz);
              die->velocityX += (dx << 15) / dist;
              die->velocityY += (dy << 15) / dist;
              die->velocityZ += (dz << 15) / dist;
            }
          }
        }
        // Bounce off the board's sides and back wall.
        if (die->x < -0xc80000) {
          die->x = -0xc80000;
          die->velocityX = -die->velocityX * 48 / 64;
        }
        if (die->x > 0xc80000) {
          die->x = 0xc80000;
          die->velocityX = -die->velocityX * 48 / 64;
        }
        if (die->z < -0x960000) {
          die->z = -0x960000;
          die->velocityZ = -die->velocityZ * 48 / 64;
        }
        if (die->z > die->y + 0xb40000) {
          die->z = die->y + 0xb40000;
          if (die->velocityZ > 0) {
            die->velocityY += die->velocityZ / 2;
            die->velocityZ = -die->velocityZ * 48 / 64;
          }
        }
        // Gravity; on the board a die bounces, loses speed, sometimes kicks
        // off in a random direction, and spins less. Once it barely bounces
        // it stops, turned to the nearest face.
        die->velocityY += -0x8000;
        if (die->y <= 0) {
          die->y = 0;
          die->velocityY = -die->velocityY * 48 / 64;
          if ((Random16() & 3) == 0) {
            s32 vx;
            s32 vz;

            vx = die->velocityX >> 8;
            vz = die->velocityZ >> 8;
            speed = Iwram_Sqrt(vx * vx + vz * vz) * 56 / 64;
            angle = Random16() & 0xffff;
            die->velocityX = Trig_Sin(angle) * speed >> 8;
            die->velocityZ = Trig_Cos(angle) * speed >> 8;
          } else {
            die->velocityX = die->velocityX * 56 / 64;
            die->velocityZ = die->velocityZ * 56 / 64;
          }
          if (spinRate[i] > 19) {
            spinRate[i] -= 20;
          }
          if (die->velocityY < 0x30000) {
            die->velocityX = 0;
            die->velocityY = 0;
            die->velocityZ = 0;
            spinRate[i] = 0;
            roll = die->variant;
            faceStep = roll / 1024 % 3;
            if (faceStep == 1) {
              die->variant = roll + 0x800;
            } else if (faceStep == 2) {
              die->variant = roll + 0x400;
            }
          }
        }
        die->velocityX = die->velocityX * 63 / 64;
        die->velocityY = die->velocityY * 63 / 64;
        die->velocityZ = die->velocityZ * 63 / 64;
        die->variant += spinRate[i];
        if (die->variant > 0x47ff) {
          die->variant -= 0x4800;
        }
        if (spinRate[i] > 0) {
          spinRate[i]--;
        }
        // At rest: read the face from the roll and the row from the depth.
        if (die->velocityX == 0 && die->velocityY == 0 && die->velocityZ == 0 &&
            die->y == 0) {
          numbers[i] = die->variant / 0xc00;
          for (j = 0; j != 6; j++) {
            if (die->z > gLuckyDice_FaceHeights[j] << 16) {
              numbers[i + 2] = j;
              break;
            }
          }
          stopped++;
        }
      }

      // Both dice stopped: score the four numbers.
      if (stopped == 2) {
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
          AudioCommand_PlayFar(93);
        } else {
          pairs = 0;
          for (i = 0; i != 3; i++) {
            for (j = i + 1; j != 4; j++) {
              if (numbers[i] == numbers[j]) {
                matchedNumbers[pairs] = numbers[i];
                pairs++;
              }
            }
          }
          if (pairs == 1) {
            winnings = bet;
            UiText_DrawNumberInWindowFar(gGameState.coins, 6,
                                         sprites->coinsWindow, 0, 0);
            resultKind = 3;
          }
          if (pairs == 2) {
            winnings = bet * 2;
            UiText_DrawNumberInWindowFar(gGameState.coins, 6,
                                         sprites->coinsWindow, 0, 0);
            flashTimer[Random16() & 1] = 60;
            resultKind = 2;
            AudioCommand_PlayFar(91);
          }
          if (pairs == 3) {
            winnings = bet * 3;
            UiText_DrawNumberInWindowFar(gGameState.coins, 6,
                                         sprites->coinsWindow, 0, 0);
            flashTimer[Random16() & 1] = 60;
            resultKind = 1;
            AudioCommand_PlayFar(92);
          }
        }
      }
    }

    // Hide the unused sprites, write the four affine matrices (one per die
    // and shadow, scaled by depth) and copy the sprites to OAM.
    for (; spriteCount != 128; spriteCount++) {
      sprites->obj[spriteCount].attr01 = 0x40f02000;
      sprites->obj[spriteCount].attr2 = 0;
    }

    for (i = 0; i != 4; i++) {
      affineScale = spriteDepth[i] / 2 + 128;
      affineA = affineScale;
      affineB = 0;
      affineC = 0;
      affineD = affineScale;
      sprites->obj[i * 4 + 0].attr2 |= affineA << 16;
      sprites->obj[i * 4 + 1].attr2 |= affineB << 16;
      sprites->obj[i * 4 + 2].attr2 |= affineC << 16;
      sprites->obj[i * 4 + 3].attr2 |= affineD << 16;
    }

    Dma_Set(sprites->obj, (void *)0x07000000, 0x84000100, REG_DMA3);
    WaitFrames(1);
    frame++;
  }

  // Fade out over 17 frames and free the work blocks.
  for (i = 0; i != 17; i++) {
    scale = 0x10000 - (i << 12);
    Graphics_ScaleRgb555Buffer(spritePalette, (u16 *)0x05000200, scale, 256);
    Graphics_ScaleRgb555Buffer(bgPalette, (u16 *)0x05000000, scale, 256);
    WaitFrames(1);
  }
  Runtime_ReleaseHeapBlock(12);
  Runtime_ReleaseHeapBlock(45);
  Runtime_ReleaseHeapBlock(40);
  Runtime_ReleaseHeapBlock(39);
  Runtime_ReleaseHeapBlock(41);
}
#endif
