// 2026-10-26 (round 5): differ 104 of 2055 (draft 2055); resetDice set before
// the state after a loss (a statement-move search): 108 to 104. Both decodes go
// through Call2 with Ram_MapCellBuffer as an integer, as The Broken Seal's
// LuckyDice_Run calls its decoder: the native then reloads the cell buffer's
// address from the pool for each transfer instead of keeping it in a register:
// 137 to 108. Left: scheduling rows only (pool loads one row early or late,
// argument stores).
// 2026-10-25, with the Makefile's options (-mthumb-slow-loads): differ 137
// of 2055 (draft 2056); not worked this pass.
// Draft, differ 231 of 2055: LuckyDice_Run, the Lucky Dice minigame, ported
// from ☀️'s LuckyDice_Run (games/tbs/src/field/lucky_dice.c) with this
// game's names: its heap blocks, the sprite block's windows at 0x4c4..0x4cc,
// the result lettering at 0x278, the winnings window 13 tiles plus the digits
// wide, no help window reopened after a loss, gInput for the keys, and the
// shadow's depth read from the die's height as a halfword. Left: scheduling
// and pool order in the setup (the native reloads gMapCellBuffer and the DMA
// constants from the pool at each use) and in the key tests; the resource
// and message names are not in directory.s and en.po yet.
#include "types.h"
#include "lib/call.h"
#include "ram_buffer.h"
#include "io_reg.h"
#include "dma.h"
#include "iwram_call.h"
#include "input.h"
#include "adventure.h"
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
  struct DiceParticle particles[2];
};

struct DiceOam {
  volatile u32 attr01;
  volatile u32 attr2;
};

struct RenderInput;

struct LuckyDiceSprites {
  u8 filler[196];
  struct DiceOam oam[128];
  struct RenderInput *coinsWindow;
  struct RenderInput *winningsWindow;
  struct RenderInput *helpWindow;
};

struct DiceCamera {
  s32 eye[3];
  s32 focus[3];
};

extern u8 gRenderOamEnabled;
extern const u8 gLuckyDice_LabelCounts[];
extern const u8 gLuckyDice_LabelTiles[];
extern const u8 gLuckyDice_FaceTiles[];
extern const s16 gLuckyDice_FaceHeights[];

void *Runtime_AllocateHeapBlock(s32 slot, s32 size);
void *Runtime_AllocateBlock(s32 slot, s32 size);
void Runtime_ReleaseHeapBlock(s32 slot);
void Resource_ReservedNoOp(s32 id);
void Camera_ResetSceneDefaults(void);
void Scheduler_ResetTaskTable(void);
void *Resource_GetTableEntry(s32 resourceId);
s32 Resource_DecodeType01(const void *source, void *destination);
s32 Graphics_ScaleRgb555Buffer(u16 *source, u16 *destination, s32 scale,
                               s32 count);
void Render_ResetTransformState(void);
void Transform_SetLookAt(s32 *eye, s32 *target);
s32 Render_ProjectPoint(const void *point, s32 *screen);
void UiWork_InitializeWithResourceCountersFar(void);
struct RenderInput *UiWindow_CreateFar(s32 x, s32 y, s32 width, s32 height,
                                       s32 mode);
void UiWork_FinalizeFar(struct RenderInput *window, s32 mode);
void UiText_DrawCharacterAtOffsetFar(s32 message, struct RenderInput *window,
                                     s32 x, s32 y);
void UiText_DrawNumberInWindowFar(s32 value, s32 width,
                                  struct RenderInput *window, s32 x, s32 y);
void Party_AdjustCoinsFar(s32 amount);
s32 Party_GetAverageLevelFar(void);
void Audio_PlayCue(s32 cue);
u32 Random16(void);
s32 Trig_Sin(s32 angle);
s32 Trig_Cos(s32 angle);
void WaitFrames(s32 frames);

extern char MsgLuckyDiceCoins;
extern char MsgLuckyDiceWinnings;
extern char MsgLuckyDiceThrow;
extern char ResourceId_FarCall00C;
extern char ResourceId_LuckyDiceBoard;
extern char ResourceId_LuckyDiceSprites;

void LuckyDice_Run(void) {
  u16 bgPalette[256];
  s32 spriteDepth[4];
  s32 numbers[4];
  s32 groundPoint[3];
  s32 screen[3];
  s32 unused[8];
  volatile u16 affineA;
  volatile u16 affineB;
  volatile u16 affineC;
  volatile u16 affineD;
  s32 spinRate[2];
  s32 flashTimer[2];
  s32 luckyNumbers[2];
  s32 drawOrder[2];
  s32 matchedNumbers[2];
  u16 *spritePalette;
  struct DiceWork *diceWork;
  struct LuckyDiceSprites *sprites;
  struct DiceCamera *camera;
  s32 frame;
  s32 state;
  s32 bet;
  s32 resultTimer;
  s32 resultKind;
  s32 winnings;
  s32 payoutTimer;
  s32 digits;
  s32 spriteCount;
  struct DiceParticle *rollingDie;
  struct DiceParticle *otherDie;
  s32 resetDice;
  struct RenderInput *window;
  s32 index;
  s32 otherIndex;
  s32 rollAngle;
  s32 x;
  s32 y;
  s32 halfSize;
  s32 faceTile;
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

  Runtime_AllocateHeapBlock(100, 0x60e);
  spritePalette = Runtime_AllocateHeapBlock(96, 0x200);
  diceWork = Runtime_AllocateBlock(92, 0x7b7c);
  sprites = Runtime_AllocateBlock(180, 0x614);
  camera = Runtime_AllocateBlock(48, 76);
  Resource_ReservedNoOp((s32)&ResourceId_FarCall00C);
  Camera_ResetSceneDefaults();
  Scheduler_ResetTaskTable();
  gRenderOamEnabled = 0;

  index = 0;
  for (tileY = 0; tileY != 20; tileY++) {
    for (tileX = 0; tileX != 32; tileX++) {
      *(volatile u16 *)(0x06003000 + index) = tileX + tileY * 30;
      index += 2;
    }
  }

  resourceAddress =
      (s32)Resource_GetTableEntry((s32)&ResourceId_LuckyDiceBoard);
  Dma_Set((void *)resourceAddress, (void *)0x05000000,
          (DMA_ENABLE | DMA_32BIT) << 16 | 0x70, REG_DMA3);
  Dma_Set((void *)0x05000000, bgPalette, (DMA_ENABLE | DMA_32BIT) << 16 | 0x80,
          REG_DMA3);
  resourceAddress += 0x1c0;
  // UB: Call2 passes integers to the decoder's two pointer parameters, as
  // The Broken Seal's LuckyDice_Run does; typed calls keep the cell buffer
  // in a register for the transfer after each decode (137 against 108).
#ifdef UBFIX
  Resource_DecodeType01((const void *)resourceAddress, Ram_MapCellBuffer);
#else
  Call2((void (*)())Resource_DecodeType01, resourceAddress,
        (s32)Ram_MapCellBuffer);
#endif
  Dma_Set(Ram_MapCellBuffer, (void *)0x06004000,
          (DMA_ENABLE | DMA_32BIT) << 16 | 0x2580, REG_DMA3);
  resourceAddress =
      (s32)Resource_GetTableEntry((s32)&ResourceId_LuckyDiceSprites);
  Dma_Set((void *)resourceAddress, spritePalette,
          (DMA_ENABLE | DMA_32BIT) << 16 | 0x80, REG_DMA3);
  Dma_Set((void *)resourceAddress, (void *)0x05000200,
          (DMA_ENABLE | DMA_32BIT) << 16 | 0x80, REG_DMA3);
  resourceAddress += 0x200;
#ifdef UBFIX
  Resource_DecodeType01((const void *)resourceAddress, Ram_MapCellBuffer);
#else
  Call2((void (*)())Resource_DecodeType01, resourceAddress,
        (s32)Ram_MapCellBuffer);
#endif
  Dma_Set(Ram_MapCellBuffer, (void *)0x06010000,
          (DMA_ENABLE | DMA_32BIT) << 16 | 0x1f00, REG_DMA3);
  Graphics_ScaleRgb555Buffer(spritePalette, (u16 *)0x05000200, 0, 256);
  Graphics_ScaleRgb555Buffer(bgPalette, (u16 *)0x05000000, 0, 256);

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

  state = 0;
  resultTimer = 0;
  resultKind = -1;
  winnings = 0;
  payoutTimer = 0;
  for (index = 0; index != 2; index++) {
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

  UiWork_InitializeWithResourceCountersFar();
  window = UiWindow_CreateFar(18, 0, 12, 3, 6);
  sprites->coinsWindow = window;
  UiText_DrawCharacterAtOffsetFar((s32)&MsgLuckyDiceCoins, window, 48, 0);
  UiText_DrawNumberInWindowFar(gAdventure.coins, 6, sprites->coinsWindow, 0,
                               0);
  window = UiWindow_CreateFar(22, 16, 8, 4, 6);
  sprites->helpWindow = window;
  UiText_DrawCharacterAtOffsetFar((s32)&MsgLuckyDiceCoins + 10, window, 0, 0);
  UiText_DrawCharacterAtOffsetFar((s32)&MsgLuckyDiceCoins + 11,
                                  sprites->helpWindow, 0, 8);

  frame = 0;
  for (;;) {
    resetDice = 0;
    if (frame <= 16) {
      Graphics_ScaleRgb555Buffer(spritePalette, (u16 *)0x05000200, frame << 12,
                                 256);
      Graphics_ScaleRgb555Buffer(bgPalette, (u16 *)0x05000000, frame << 12,
                                 256);
    }
    Render_ResetTransformState();
    Transform_SetLookAt(camera->eye, camera->focus);

    spriteCount = 0;
    sprites->oam[spriteCount].attr01 = 0xc0006000;
    sprites->oam[spriteCount].attr2 = 0x358;
    spriteCount++;
    sprites->oam[spriteCount].attr01 = 0x40102020;
    sprites->oam[spriteCount].attr2 = 0x398;
    spriteCount++;
    sprites->oam[spriteCount].attr01 = 0x40102030;
    sprites->oam[spriteCount].attr2 = 0x3a0;
    spriteCount++;
    sprites->oam[spriteCount].attr01 = 0x40042020;
    sprites->oam[spriteCount].attr2 = 0x3a8;
    spriteCount++;
    sprites->oam[spriteCount].attr01 = 0x40042030;
    sprites->oam[spriteCount].attr2 = 0x3a8;
    spriteCount++;
    for (index = 0; index != 2; index++) {
      sprites->oam[spriteCount].attr01 = (32 + index * 16) | 0x40202000;
      sprites->oam[spriteCount].attr2 = luckyNumbers[index] * 8 + 0x3b0;
      spriteCount++;
    }

    if (state == 2 || state == 3) {
      for (index = 0; index != 4; index++) {
        if (resultTimer == index * 5) {
          Audio_PlayCue(303);
        }
        if (resultTimer >= index * 5) {
          sprites->oam[spriteCount].attr01 =
              0x80002040 | ((56 + index * 32) << 16);
          sprites->oam[spriteCount].attr2 = numbers[index] * 32 + 152;
          spriteCount++;
        }
      }
      if (resultTimer > 19 && resultKind >= 0) {
        for (index = 0; index != gLuckyDice_LabelCounts[resultKind]; index++) {
          s32 labelX;
          s32 labelY;

          labelX = 0x278 - gLuckyDice_LabelCounts[resultKind] * 16 + index * 32;
          labelY = 96;
          sprites->oam[spriteCount].attr01 =
              (((labelX & 0x1ff) << 16) | labelY) | 0x80002000;
          sprites->oam[spriteCount].attr2 =
              gLuckyDice_LabelTiles[resultKind] * 8 + 152 + index * 32;
          spriteCount++;
        }
      }
    }

    if (state == 2) {
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
        window = UiWindow_CreateFar(8 - digits / 2, 16, digits + 13, 3, 6);
        sprites->winningsWindow = window;
        UiText_DrawCharacterAtOffsetFar((s32)&MsgLuckyDiceWinnings, window, 0,
                                        0);
        UiText_DrawCharacterAtOffsetFar((s32)&MsgLuckyDiceWinnings - 1,
                                        sprites->winningsWindow,
                                        digits * 8 + 56, 0);
        UiText_DrawNumberInWindowFar(winnings, digits, sprites->winningsWindow,
                                     56, 0);
      }
      if (payoutTimer > 0) {
        payoutTimer--;
      }
      if (resultTimer > 19 && (gInput.repeat & 1)) {
        Audio_PlayCue(112);
        if (winnings > 0) {
          state = 3;
          payoutTimer = 0;
        } else {
          resetDice = 1;
          state = 0;
        }
      }
      resultTimer++;
    } else if (state == 3) {
      if (payoutTimer == 0) {
        winnings = 0;
        UiText_DrawNumberInWindowFar(0, digits, sprites->winningsWindow, 56, 0);
        UiText_DrawNumberInWindowFar(gAdventure.coins, 6, sprites->coinsWindow,
                                     0, 0);
      }
      if (payoutTimer == 15) {
        state = 0;
        UiWork_FinalizeFar(sprites->winningsWindow, 1);
        window = UiWindow_CreateFar(22, 16, 8, 4, 6);
        sprites->helpWindow = window;
        UiText_DrawCharacterAtOffsetFar((s32)&MsgLuckyDiceThrow, window, 0, 0);
        UiText_DrawCharacterAtOffsetFar((s32)&MsgLuckyDiceThrow + 1,
                                        sprites->helpWindow, 0, 8);
        resetDice = 1;
      }
      payoutTimer++;
    } else if (state == 0 && frame > 17) {
      (void)gInput.repeat;
      (void)gInput.repeat;
      if (gInput.held & 0x40) {
        diceWork->particles[0].y += 0x40000;
        diceWork->particles[1].y += 0x40000;
      }
      if (gInput.held & 0x80) {
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
      if ((u32)gAdventure.coins < (u32)(Party_GetAverageLevelFar() * 10)) {
        UiWork_FinalizeFar(sprites->helpWindow, 1);
        break;
      }
      if (gInput.repeat & 2) {
        Audio_PlayCue(113);
        UiWork_FinalizeFar(sprites->helpWindow, 1);
        break;
      }
      if (gInput.repeat & 1) {
        Audio_PlayCue(302);
        UiWork_FinalizeFar(sprites->helpWindow, 1);
        state = 1;
        bet = Party_GetAverageLevelFar() * 10;
        if ((u32)bet > (u32)gAdventure.coins) {
          bet = gAdventure.coins;
        }
        Party_AdjustCoinsFar(-bet);
        UiText_DrawNumberInWindowFar(gAdventure.coins, 6, sprites->coinsWindow,
                                     0, 0);
        for (index = 0; index != 2; index++) {
          rollingDie = &diceWork->particles[index];
          if (index > 1) {
            rollingDie->z = -0xb40000;
            rollingDie->y = 0x600000;
          }
          rollingDie->velocityX = ((Random16() & 127) - 64) << 12;
          rollingDie->velocityY = ((Random16() & 15) + 48) << 13;
          rollingDie->velocityZ =
              (((Random16() & 63) + 140) << 12) + rollingDie->y / 6;
          rollingDie->variant = Random16() % 0x4800;
          spinRate[index] = 300;
        }
      }
    }

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

    for (index = 0; index != 2; index++) {
      drawOrder[index] = index;
    }
    for (index = 1; index < 2; index++) {
      sortedDie = drawOrder[index];
      for (otherIndex = index - 1;
           otherIndex >= 0 && diceWork->particles[drawOrder[otherIndex]].z >
                                  diceWork->particles[sortedDie].z;
           otherIndex--) {
        drawOrder[otherIndex + 1] = drawOrder[otherIndex];
      }
      drawOrder[otherIndex + 1] = sortedDie;
    }

    for (index = 0; index != 2; index++) {
      s32 dieX;
      s32 dieY;

      rollingDie = &diceWork->particles[drawOrder[index]];
      Render_ProjectPoint(rollingDie, screen);
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
      if (dieY + 96 > -16) {
        sprites->oam[spriteCount].attr01 =
            ((drawOrder[index] << 25) | (((dieX + 0x278) & 0x1ff) << 16) |
             ((dieY + 0x160) & 255)) |
            0x40002300;
        sprites->oam[spriteCount].attr2 =
            (gLuckyDice_FaceTiles[rollingDie->variant / 1024] << 3) | 0x400;
        spriteCount++;
      }
    }

    if (state == 0) {
      for (index = 0; index != 2; index++) {
        Render_ProjectPoint(&diceWork->particles[index], screen);
        x = screen[0] - 0x800 / (spriteDepth[drawOrder[index]] / 2 + 128);
        y = screen[1] + 0x300 / (spriteDepth[drawOrder[index]] / 2 + 128);
        sprites->oam[spriteCount].attr01 =
            ((((x + 0x278) & 0x1ff) << 16) | ((y + 0x160) & 255)) | 0x80002000;
        if (index == 1) {
          sprites->oam[spriteCount].attr01 |= 0x10000000;
        }
        sprites->oam[spriteCount].attr2 = 0x958;
        spriteCount++;
      }
    }

    for (index = 0; index != 2; index++) {
      s32 shadowX;
      s32 shadowY;

      groundPoint[0] = diceWork->particles[index].x;
      groundPoint[1] = 0;
      groundPoint[2] = diceWork->particles[index].z;
      Render_ProjectPoint(groundPoint, screen);
      if (screen[2] < 0x15e) {
        screen[2] = 0x15e;
      }
      if (screen[2] > 0x28a) {
        screen[2] = 0x28a;
      }
      spriteDepth[2 + index] = screen[2] - 0x15e;
      spriteDepth[2 + index] += (s16)(diceWork->particles[index].y >> 16);
      shadowX = screen[0];
      shadowY = screen[1];
      if (shadowY + 88 > -16) {
        sprites->oam[spriteCount].attr01 =
            (((index + 2) << 25) | (((shadowX + 0x270) & 0x1ff) << 16)) |
            ((shadowY + 0x158) & 255) | 0x40002300;
        sprites->oam[spriteCount].attr2 = 0x890;
        spriteCount++;
      }
    }

    if (state == 1) {
      s32 stoppedDice;

      stoppedDice = 0;
      for (index = 0; index != 2; index++) {
        rollingDie = &diceWork->particles[index];
        rollingDie->x += rollingDie->velocityX;
        rollingDie->y += rollingDie->velocityY;
        rollingDie->z += rollingDie->velocityZ;
        for (otherIndex = 0; otherIndex != 2; otherIndex++) {
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
              rollingDie->velocityX += (separationX << 15) / separation;
              rollingDie->velocityY += (separationY << 15) / separation;
              rollingDie->velocityZ += (separationZ << 15) / separation;
            }
          }
        }
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
            rollingDie->velocityX = 0;
            rollingDie->velocityY = 0;
            rollingDie->velocityZ = 0;
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
        rollingDie->velocityX = rollingDie->velocityX * 63 / 64;
        rollingDie->velocityY = rollingDie->velocityY * 63 / 64;
        rollingDie->velocityZ = rollingDie->velocityZ * 63 / 64;
        rollingDie->variant += spinRate[index];
        if (rollingDie->variant > 0x47ff) {
          rollingDie->variant -= 0x4800;
        }
        if (spinRate[index] > 0) {
          spinRate[index]--;
        }
        if (rollingDie->velocityX == 0 && rollingDie->velocityY == 0 &&
            rollingDie->velocityZ == 0 && rollingDie->y == 0) {
          numbers[index] = rollingDie->variant / 0xc00;
          for (otherIndex = 0; otherIndex != 6; otherIndex++) {
            s32 rowDepth;

            rowDepth = gLuckyDice_FaceHeights[otherIndex] << 16;
            if (rollingDie->z > rowDepth) {
              numbers[index + 2] = otherIndex;
              break;
            }
          }
          stoppedDice++;
        }
      }

      if (stoppedDice == 2) {
        state = 2;
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
          UiText_DrawNumberInWindowFar(gAdventure.coins, 6,
                                       sprites->coinsWindow, 0, 0);
          Audio_PlayCue(93);
        } else {
          pairCount = 0;
          for (index = 0; index != 3; index++) {
            for (otherIndex = index + 1; otherIndex != 4; otherIndex++) {
              if (numbers[index] == numbers[otherIndex]) {
                matchedNumbers[pairCount] = numbers[index];
                pairCount++;
              }
            }
          }
          if (pairCount == 1) {
            winnings = bet;
            UiText_DrawNumberInWindowFar(gAdventure.coins, 6,
                                         sprites->coinsWindow, 0, 0);
            resultKind = 3;
          }
          if (pairCount == 2) {
            winnings = bet * 2;
            UiText_DrawNumberInWindowFar(gAdventure.coins, 6,
                                         sprites->coinsWindow, 0, 0);
            flashTimer[Random16() & 1] = 60;
            resultKind = 2;
            Audio_PlayCue(91);
          }
          if (pairCount == 3) {
            winnings = bet * 3;
            UiText_DrawNumberInWindowFar(gAdventure.coins, 6,
                                         sprites->coinsWindow, 0, 0);
            flashTimer[Random16() & 1] = 60;
            resultKind = 1;
            Audio_PlayCue(92);
          }
        }
      }
    }

    for (; spriteCount != 128; spriteCount++) {
      sprites->oam[spriteCount].attr01 = 0x40f02000;
      sprites->oam[spriteCount].attr2 = 0;
    }

    for (index = 0; index != 4; index++) {
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

    Dma_Set(sprites->oam, (void *)0x07000000,
            (DMA_ENABLE | DMA_32BIT) << 16 | 0x100, REG_DMA3);
    WaitFrames(1);
    frame++;
  }

  for (index = 0; index != 17; index++) {
    scale = 0x10000 - (index << 12);
    Graphics_ScaleRgb555Buffer(spritePalette, (u16 *)0x05000200, scale, 256);
    Graphics_ScaleRgb555Buffer(bgPalette, (u16 *)0x05000000, scale, 256);
    WaitFrames(1);
  }
  Runtime_ReleaseHeapBlock(48);
  Runtime_ReleaseHeapBlock(180);
  Runtime_ReleaseHeapBlock(96);
  Runtime_ReleaseHeapBlock(92);
  Runtime_ReleaseHeapBlock(100);
}
