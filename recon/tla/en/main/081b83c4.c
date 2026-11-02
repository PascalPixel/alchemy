// Matches: differ 0 (tla-en, 2026-11-01): the casino's four-dice game, built
// like LuckyDice_Run (four dice in a round dome, Slots_DomePicture): A throws
// for a bet of ten coins a level. A pair, two pairs, a triple or four of a kind
// (Func_081b834c, banners Slots_OnePairBanner to Slots_PerfectBanner) pays 1,
// 2, 3 or 8 times the bet; a win offers a double-up round (state 5,
// Slots_DoubleUpBanner and Slots_CashOutBanner): guess whether the next total
// is higher or lower than this one (state 1, Slots_HigherBanner and
// Slots_LowerBanner) for twice the winnings, up to five rounds, the last the
// final game; a miss loses everything. Winnings are counted into the coins a
// coin a frame (state 4). The dice bounce off the dome's round wall, settle to
// a face and are drawn back to front with their shadows; palettes of the
// banners pulse with Func_081b810c. What closed it: the first throw uses a
// plain 0, with no zero local; each die loop sets die = &dice->particles[index]
// in its body; the affine loop computes its scale twice (scaleX, scaleY) and
// keeps its 0 in an s32 local, which removes the pooled halfword zero and with
// it the mid-loop pool; the first lamp row's tiles start at 152, not 608; and
// the banner's bannerX is a ?: so its second bannerY = 64 survives CSE. One
// device is left, the tagged volatile on the four affine halfwords: the twin's
// (void)&affineA form, a milder tag (dead), is byte-identical at differ 0. The
// message names stand for 0xd68 to 0xd6a and 0xd77 to 0xd7e; the resources are
// directory entries 0x85 to 0x97.
#include "types.h"
#include "io_reg.h"
#include "dma.h"
#include "iwram_call.h"
#include "input.h"
#include "main_routine_work.h"
#include "bg_scroll.h"

struct DiceParticle {
  s32 x;
  s32 y;
  s32 z;
  s32 velocityX;
  s32 velocityY;
  s32 velocityZ;
  s32 variant;
};

struct DiceWork {
  struct DiceParticle particles[4];
};

struct DiceOam {
  volatile u32 attr01;
  volatile u32 attr2;
};

struct RenderInput;

struct DiceSprites {
  u8 filler[196];
  struct DiceOam oam[128];
  struct RenderInput *coinsWindow;
  struct RenderInput *betWindow;
  struct RenderInput *helpWindow;
};

struct DiceCamera {
  s32 eye[3];
  s32 focus[3];
};

extern u8 gRenderOamEnabled;
extern u8 gMapCellBuffer[];
extern const u8 gData_081ba30c[];
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

void *HeapSlot_Alloc(s32 slot, s32 size);
void *HeapSlot_AllocEwram(s32 slot, s32 size);
void HeapSlot_Release(s32 slot);
void Resource_ReservedNoOp(s32 id);
void Camera_ResetSceneDefaultsB(void);
void Task_ResetAll(void);
void *Resource_GetTableEntry(s32 resourceId);
s32 Resource_DecodeType01(const void *source, void *destination);
s32 Graphics_ScaleRgb555BufferB(u16 *source, u16 *destination, s32 scale,
                                s32 count);
void Func_081b810c(u16 *source, u16 *destination, u32 count, s32 angle);
s32 Func_081b834c(s32 *dice);
s32 Func_081b8180(s32 sprite, s32 x, s32 y, s32 value, s32 palette, s32 digits);
s32 Func_081b8274(s32 sprite, s32 x, s32 y, s32 value, s32 palette, s32 digits);
void Render_ResetTransformState(void);
void Transform_SetLookAt(s32 *eye, s32 *target);
s32 Render_ProjectPoint(const void *point, s32 *screen);
void WindowSystem_InitFieldFar(void);
struct RenderInput *Window_CreateFar(s32 x, s32 y, s32 width, s32 height,
                                     s32 mode);
void Window_CloseFar(struct RenderInput *window, s32 mode);
void Text_DrawCharacterAtOffsetFar(s32 message, struct RenderInput *window,
                                   s32 x, s32 y);
void Text_DrawNumberInWindowFar(s32 value, s32 width,
                                struct RenderInput *window, s32 x, s32 y);
void Party_AdjustCoinsFar(s32 amount);
s32 Party_GetAverageLevelFar(void);
void Sound_PlayFar(s32 cue);
u32 Random16(void);
s32 Trig_Sin(s32 angle);
s32 Trig_Cos(s32 angle);
void wait(s32 frames);

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

void Func_081b83c4(void) {
  u16 bgPalette[256];
  s32 spinRate[4];
  s32 spriteDepth[8];
  s32 numbers[4];
  s32 groundPoint[3];
  s32 screen[3];
  s32 drawOrder[4];
  /* FAKEMATCH(volatile): the four affine halfwords stored to sp+138..132 and
   * reloaded with ldrh; as plain u16 locals, 1094 of 3555 instructions differ
   * in tla-en. */
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
  Resource_DecodeType01(resourceAddress, cellBuffer);
  Dma_Set(cellBuffer, (void *)0x06004000,
          (DMA_ENABLE | DMA_32BIT) << 16 | 0x2580, REG_DMA3);
  resourceAddress = Resource_GetTableEntry((s32)&ResourceId_Data087);
  Dma_Set(resourceAddress, spritePalette, (DMA_ENABLE | DMA_32BIT) << 16 | 0x30,
          REG_DMA3);
  resourceAddress += 128;
  Resource_DecodeType01(resourceAddress, cellBuffer);
  Dma_Set(cellBuffer, (void *)0x06010000,
          (DMA_ENABLE | DMA_32BIT) << 16 | 0x4c0, REG_DMA3);
  resourceAddress =
      Resource_GetTableEntry((s32)&ResourceId_SlotsSmallDoubleUpBanner);
  Dma_Set(resourceAddress, spritePalette + 112,
          (DMA_ENABLE | DMA_32BIT) << 16 | 8, REG_DMA3);
  resourceAddress += 32;
  Resource_DecodeType01(resourceAddress, cellBuffer);
  Dma_Set(cellBuffer, (void *)0x06011300, (DMA_ENABLE | DMA_32BIT) << 16 | 0x80,
          REG_DMA3);
  resourceAddress =
      Resource_GetTableEntry((s32)&ResourceId_SlotsFinalGameBanner);
  Dma_Set(resourceAddress, spritePalette + 96,
          (DMA_ENABLE | DMA_32BIT) << 16 | 8, REG_DMA3);
  resourceAddress += 32;
  Resource_DecodeType01(resourceAddress, cellBuffer);
  Dma_Set(cellBuffer, (void *)0x06011500,
          (DMA_ENABLE | DMA_32BIT) << 16 | 0x100, REG_DMA3);
  resourceAddress = Resource_GetTableEntry((s32)&ResourceId_SlotsOrangeDigits);
  Dma_Set(resourceAddress, spritePalette + 128,
          (DMA_ENABLE | DMA_32BIT) << 16 | 8, REG_DMA3);
  resourceAddress += 32;
  Resource_DecodeType01(resourceAddress, cellBuffer);
  Dma_Set(cellBuffer, (void *)0x06011900,
          (DMA_ENABLE | DMA_32BIT) << 16 | 0x280, REG_DMA3);
  resourceAddress = Resource_GetTableEntry((s32)&ResourceId_SlotsTimesSign);
  Dma_Set(resourceAddress, spritePalette + 144,
          (DMA_ENABLE | DMA_32BIT) << 16 | 8, REG_DMA3);
  resourceAddress += 32;
  Resource_DecodeType01(resourceAddress, cellBuffer);
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
  Resource_DecodeType01(resourceAddress, cellBuffer);
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
  window = Window_CreateFar(18, 0, 12, 3, 6);
  sprites->coinsWindow = window;
  Text_DrawCharacterAtOffsetFar((s32)&MsgLuckyDiceCoins, window, 48, 0);
  Text_DrawNumberInWindowFar(gMainRoutineWork.coins, 6, sprites->coinsWindow, 0,
                             0);
  window = Window_CreateFar(18, 3, 12, 3, 6);
  sprites->betWindow = window;
  Text_DrawCharacterAtOffsetFar((s32)&MsgLuckyDiceCoins - 1, window, 0, 0);
  window = Window_CreateFar(22, 16, 8, 4, 6);
  sprites->helpWindow = window;
  Text_DrawCharacterAtOffsetFar((s32)&MsgDiceGameThrow, window, 0, 0);
  Text_DrawCharacterAtOffsetFar((s32)&MsgDiceGameThrow + 1, sprites->helpWindow,
                                0, 8);
  bet = Party_GetAverageLevelFar() * 10;
  if ((u32)bet > (u32)gMainRoutineWork.coins)
    bet = gMainRoutineWork.coins;
  Text_DrawNumberInWindowFar(bet, 6, sprites->betWindow, 24, 5);
  Sound_PlayFar(1);

  frame = 0;
  for (;;) {
    spriteCount = 0;
    if (frame <= 16) {
      Graphics_ScaleRgb555BufferB(spritePalette, (u16 *)0x05000200, frame << 12,
                                  256);
      Graphics_ScaleRgb555BufferB(bgPalette, (u16 *)0x05000000, frame << 12,
                                  256);
    }
    Render_ResetTransformState();
    Transform_SetLookAt(camera->eye, camera->focus);

    if (state == 0) {
      if (frame > 17) {
        if ((u32)gMainRoutineWork.coins <
            (u32)(Party_GetAverageLevelFar() * 10)) {
          Window_CloseFar(sprites->helpWindow, 1);
          break;
        }
        if (gInput.repeat & 2) {
          Sound_PlayFar(113);
          Window_CloseFar(sprites->helpWindow, 1);
          break;
        }
        if (gInput.repeat & 1) {
          Sound_PlayFar(302);
          Window_CloseFar(sprites->helpWindow, 1);
          Sound_PlayFar(302);
          DICE_GAME_THROW(dice, spinRate);
          bet = Party_GetAverageLevelFar() * 10;
          if ((u32)bet > (u32)gMainRoutineWork.coins)
            bet = gMainRoutineWork.coins;
          Text_DrawNumberInWindowFar(bet, 6, sprites->betWindow, 24, 5);
          Party_AdjustCoinsFar(-bet);
          Text_DrawNumberInWindowFar(gMainRoutineWork.coins, 6,
                                     sprites->coinsWindow, 0, 0);
          state = 2;
          timer = 0;
          slide = 0;
          bet = Party_GetAverageLevelFar() * 10;
          Text_DrawNumberInWindowFar(bet, 6, sprites->betWindow, 24, 5);
        }
      }
    } else if (state == 1) {
      if (slide <= 7) {
        slide++;
      } else if (timer > 0) {
        if (++timer == 16) {
          Sound_PlayFar(302);
          DICE_GAME_THROW(dice, spinRate);
          state = 2;
        }
      } else if (gInput.repeat & 2) {
        if (round > 0) {
          Sound_PlayFar(114);
        } else {
          Window_CloseFar(sprites->helpWindow, 1);
          window = Window_CreateFar(22, 16, 8, 4, 6);
          sprites->helpWindow = window;
          Text_DrawCharacterAtOffsetFar((s32)&MsgDiceGameThrow, window, 0, 0);
          Text_DrawCharacterAtOffsetFar((s32)&MsgDiceGameThrow + 1,
                                        sprites->helpWindow, 0, 8);
          Sound_PlayFar(113);
          state = 0;
        }
      } else if (gInput.repeat & state) {
        Sound_PlayFar(112);
        timer = 1;
        Window_CloseFar(sprites->helpWindow, 1);
        if (round == 0) {
          bet = Party_GetAverageLevelFar() * 10;
          if ((u32)bet > (u32)gMainRoutineWork.coins)
            bet = gMainRoutineWork.coins;
          Text_DrawNumberInWindowFar(bet, 6, sprites->betWindow, 24, 5);
          Party_AdjustCoinsFar(-bet);
          Text_DrawNumberInWindowFar(gMainRoutineWork.coins, 6,
                                     sprites->coinsWindow, 0, round);
        }
      } else {
        s32 message = -1;

        if ((gInput.repeat & 0x40) && lower == 1) {
          if (target == 24)
            goto refuse;
          if (round == 0)
            message = (s32)&MsgDiceGameHigher;
          else
            message = (s32)&MsgDiceGameHigherAgain;
          Sound_PlayFar(111);
          lower = 0;
        } else if ((gInput.repeat & 0x80) && lower == 0) {
          if (target == 4) {
          refuse:
            Sound_PlayFar(113);
          } else {
            if (round == 0)
              message = (s32)&MsgDiceGameLower;
            else
              message = (s32)&MsgDiceGameLowerAgain;
            Sound_PlayFar(111);
            lower = 1;
          }
        }
        if (message != -1) {
          Text_DrawCharacterAtOffsetFar(message, sprites->helpWindow, 0, 0);
          Text_DrawCharacterAtOffsetFar((s32)&MsgDiceGameChoose,
                                        sprites->helpWindow, 0, 8);
        }
      }
      if (lower == 0) {
        Func_081b810c(spritePalette + 192, (u16 *)0x05000380, 16, timer << 12);
        Graphics_ScaleRgb555BufferB(spritePalette + 208, (u16 *)0x050003a0,
                                    0x8000, 16);
        Func_081b810c(spritePalette + 224, (u16 *)0x050003c0, 16, timer << 12);
        Graphics_ScaleRgb555BufferB(spritePalette + 240, (u16 *)0x050003e0,
                                    0x8000, 16);
      } else {
        Func_081b810c(spritePalette + 208, (u16 *)0x050003a0, 16, timer << 12);
        Graphics_ScaleRgb555BufferB(spritePalette + 192, (u16 *)0x05000380,
                                    0x8000, 16);
        Func_081b810c(spritePalette + 240, (u16 *)0x050003e0, 16, timer << 12);
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
        die->velocityY += -0x8000;
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
        Sound_PlayFar(152);
      if (timer == 70)
        Sound_PlayFar(103);
      if (timer == 40) {
        if ((u32)resultKind > 1)
          Sound_PlayFar(55);
        else if (jingle != -1)
          Sound_PlayFar(jingle);
        jingle = -1;
      }
      if (timer == 1) {
        paletteRequest = 3;
        resultKind = 0;
        jingle = -1;
        if (round == 0) {
          resultKind = Func_081b834c(numbers);
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
          Text_DrawNumberInWindowFar(bet, 6, sprites->betWindow, 24, 5);
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
            Text_DrawCharacterAtOffsetFar((s32)&MsgDiceGameDoubleUp, window, 0,
                                          0);
          }
          timer = 0;
          slide = 0;
        } else {
          target = 14;
          round = 0;
          bet = Party_GetAverageLevelFar() * 10;
          Text_DrawNumberInWindowFar(bet, 6, sprites->betWindow, 24, 5);
          multiplier = 0;
          Sound_PlayFar(1);
          state = 0;
          window = Window_CreateFar(22, 16, 8, 4, 6);
          sprites->helpWindow = window;
          Text_DrawCharacterAtOffsetFar((s32)&MsgDiceGameThrow, window, 0, 0);
          Text_DrawCharacterAtOffsetFar((s32)&MsgDiceGameThrow + 1,
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
        window =
            Window_CreateFar(9 - ((u32)digits >> 1), 16, digits + 12, 3, 6);
        sprites->helpWindow = window;
        Text_DrawCharacterAtOffsetFar((s32)&MsgLuckyDiceWinnings, window, 0, 0);
        Text_DrawCharacterAtOffsetFar((s32)&MsgLuckyDiceWinnings - 1,
                                      sprites->helpWindow, digits * 8 + 48, 0);
        Text_DrawNumberInWindowFar(winnings, digits, sprites->helpWindow, 48,
                                   0);
      }
      if (timer > 19) {
        if (gInput.repeat & 1) {
          Window_CloseFar(sprites->helpWindow, 1);
          Sound_PlayFar(1);
          Sound_PlayFar(112);
          state = 0;
          window = Window_CreateFar(22, 16, 8, 4, 6);
          sprites->helpWindow = window;
          Text_DrawCharacterAtOffsetFar((s32)&MsgDiceGameThrow, window, 0, 0);
          Text_DrawCharacterAtOffsetFar((s32)&MsgDiceGameThrow + 1,
                                        sprites->helpWindow, 0, 8);
          slide = 0;
          timer = 0;
          paletteRequest = 1;
          bet = Party_GetAverageLevelFar() * 10;
          Text_DrawNumberInWindowFar(bet, 6, sprites->betWindow, 24, 5);
          if (winnings > 0) {
            Sound_PlayFar(105);
            Party_AdjustCoinsFar(winnings);
            Text_DrawNumberInWindowFar(gMainRoutineWork.coins, 6,
                                       sprites->coinsWindow, 0, state);
          }
        } else if (winnings > 0) {
          if (--winnings == 0)
            Sound_PlayFar(105);
          Party_AdjustCoinsFar(1);
          Text_DrawNumberInWindowFar(gMainRoutineWork.coins, 6,
                                     sprites->coinsWindow, 0, 0);
          bet--;
          Text_DrawNumberInWindowFar(bet, 6, sprites->betWindow, 24, 5);
        }
      }
    } else if (state == 5) {
      if (slide == 4)
        Sound_PlayFar(175);
      if (slide <= 7) {
        slide++;
      } else if (timer > 0) {
        if (++timer == 16) {
          Window_CloseFar(sprites->helpWindow, 1);
          if (cashOut == 0) {
            target = total;
            round++;
            state = 1;
            window = Window_CreateFar(4, 16, 21, 4, 6);
            sprites->helpWindow = window;
            if (total == 24) {
              lower = 1;
              Text_DrawCharacterAtOffsetFar((s32)&MsgDiceGameLowerAgain, window,
                                            0, 0);
            } else {
              lower = 0;
              Text_DrawCharacterAtOffsetFar((s32)&MsgDiceGameHigherAgain,
                                            window, 0, 0);
            }
            Text_DrawCharacterAtOffsetFar((s32)&MsgDiceGameChoose,
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
      } else if (gInput.repeat & 1) {
        timer = 1;
        Sound_PlayFar(112);
      } else if ((gInput.repeat & 0x40) && cashOut == 1) {
        Sound_PlayFar(111);
        cashOut = 0;
      } else if ((gInput.repeat & 0x80) && cashOut == 0) {
        Sound_PlayFar(111);
        cashOut = 1;
      }
      if (cashOut == 0) {
        Func_081b810c(spritePalette + 224, (u16 *)0x050003c0, 16, timer << 12);
        Graphics_ScaleRgb555BufferB(spritePalette + 240, (u16 *)0x050003e0,
                                    0x8000, 16);
      } else {
        Func_081b810c(spritePalette + 240, (u16 *)0x050003e0, 16, timer << 12);
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
        Resource_DecodeType01(resourceAddress, cellBuffer);
        Dma_Set(cellBuffer, (void *)0x06014000,
                (DMA_ENABLE | DMA_32BIT) << 16 | 0x180, REG_DMA3);
        resourceAddress =
            Resource_GetTableEntry((s32)&ResourceId_SlotsLowerBanner);
        Dma_Set(resourceAddress, (void *)0x050003a0,
                (DMA_ENABLE | DMA_32BIT) << 16 | 8, REG_DMA3);
        Dma_Set(resourceAddress, spritePalette + 208,
                (DMA_ENABLE | DMA_32BIT) << 16 | 8, REG_DMA3);
        resourceAddress += 32;
        Resource_DecodeType01(resourceAddress, cellBuffer);
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
        Resource_DecodeType01(resourceAddress, cellBuffer);
        Dma_Set(cellBuffer, (void *)0x06014000,
                (DMA_ENABLE | DMA_32BIT) << 16 | 0x400, REG_DMA3);
        resourceAddress =
            Resource_GetTableEntry((s32)&ResourceId_SlotsGreenDigits);
        Dma_Set(resourceAddress, (void *)0x05000380,
                (DMA_ENABLE | DMA_32BIT) << 16 | 8, REG_DMA3);
        resourceAddress += 32;
        Resource_DecodeType01(resourceAddress, cellBuffer);
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
        Resource_DecodeType01(resourceAddress, cellBuffer);
        Dma_Set(cellBuffer, (void *)0x06012480,
                (DMA_ENABLE | DMA_32BIT) << 16 | 0x280, REG_DMA3);
        resourceAddress =
            Resource_GetTableEntry((s32)&ResourceId_SlotsCashOutBanner);
        Dma_Set(resourceAddress, (void *)0x050003e0,
                (DMA_ENABLE | DMA_32BIT) << 16 | 8, REG_DMA3);
        Dma_Set(resourceAddress, spritePalette + 240,
                (DMA_ENABLE | DMA_32BIT) << 16 | 8, REG_DMA3);
        resourceAddress += 32;
        Resource_DecodeType01(resourceAddress, cellBuffer);
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
              (((608 + index * 16) & 0x1ff) << 16) | 0x40000002;
          sprites->oam[spriteCount].attr2 = (188 + index * 4) | 0x6000;
          spriteCount++;
        }
        sprites->oam[spriteCount].attr01 = 0x80508000;
        sprites->oam[spriteCount].attr2 = (round * 8 + 200) | 0x8000;
        spriteCount++;
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
      spriteCount = Func_081b8180(spriteCount, 48, 20, multiplier, 10, 5);
      spriteCount = Func_081b8180(spriteCount, 80, 20, 0, 11, 1);
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
        spriteCount = Func_081b8180(spriteCount, left - 110, wobble + 60,
                                    target + 1, 14, 2);
      x = (timer > 0 && lower == 0) ? timer * 12 + 64 : 64;
      wobble = lower == 1 ? Trig_Sin((frame << 10) & 0x7fff) >> 14 : 0;
      left = x + slide * 15;
      if (target > 4)
        spriteCount = Func_081b8180(spriteCount, left - 110, 94 - wobble,
                                    target - 1, 15, 2);
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
        angle += 0x1000;
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
        spriteCount = Func_081b8274(spriteCount, x, y, total, 12, 2);
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
        sprites->oam[spriteCount].attr01 =
            (((bannerX + 512) & 0x1ff) << 16) | ((bannerY + 256) & 255) |
            0xc0000000;
        sprites->oam[spriteCount].attr2 = 0xd200;
        spriteCount++;
        sprites->oam[spriteCount].attr01 =
            (((bannerX + 576) & 0x1ff) << 16) | ((bannerY + 256) & 255) |
            0xc0000000;
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
           otherIndex >= 0 &&
           dice->particles[drawOrder[otherIndex]].z > dice->particles[sortedDie].z;
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
