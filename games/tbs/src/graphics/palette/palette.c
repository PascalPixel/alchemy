#include "battle_effect_runtime.h"
#include "unit/graphics/palette/palette.h"
#include "unit/runtime/runtime.h"
#include "battle.h"
#include "keystate.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "mem.h"
#include "types.h"
#include "callback_scheduler.h"
#include "dma.h"
#include "edition.h"
#include "io_reg.h"
#include "io_write_queue.h"
#include "map_scroll.h"
#include "ram_buffer.h"
#include "resource.h"
#include "resource_ids.h"
#include "palbuf.h"
#include "iwram_call.h"
#include "heap_state.h"
#include "scene.h"
#include "global_cells.h"
#include "frame.h"
#include "oambuf.h"
#include "macros.h"
#include "songs.h"

// A sprite of the "Press Start" prompt, linked into a render slot each frame:
// OAM attributes 0 to 2 with the position split out.
struct IntroSprite {
  u32 next; // the render slot link
  u8 y;
  u8 attr0High; // shape and mode bits of attribute 0
  u16 x : 9;
  u16 attr1High : 7; // flip and size bits of attribute 1
  s32 tile;
};

// The title intro's state, in heap slot 43.
struct IntroWork {
  s32 backRows;  // rows of the far background copied in as it scrolls
  s32 frontRows; // rows of the near background copied in
  s32 frame;
  s32 tick;
  s32 state;
  s32 rise; // how far the sprites have risen
  union {
    struct OamShadow objects[120];
    struct {
      u8 introObjects[0x68]; // objects[0] to [12], still the intro's
      struct IntroSprite sprites[8];
    } prompt;
  } area;
};

// The x of each of the six columns of intro sprites
const u8 gTitle_IntroSpriteColumns[] = {96, 136, 140, 160, 168, 184};

// The prompt's blend level over its 60-frame pulse
// clang-format off
const u8 gTitle_PromptBlendLevels[60] = {
    1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
    16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
    16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
    15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0
};
// clang-format on

// Entry `i` of the intro's OAM shadow
#define INTRO_OBJ(i) (work->area.objects[i])

// Brings `v` into 0..255 by whole turns of 256
#define WRAP_TO_BYTE(v)                                                        \
  while ((v) > 255)                                                            \
    (v) -= 256;                                                                \
  while ((v) < 0)                                                              \
  (v) += 256

// Sets x and y to the top left of a sprite of half-width `half` centred on
// column `column`, between slopeBaseY at x = 104 and slopeTopY at x = 184.
#define PLACE_ON_SLOPE(column, half)                                           \
  x = gTitle_IntroSpriteColumns[column] - (half);                              \
  y = (slopeTopY - slopeBaseY) * ((x + (half)) - 104) / 80 + slopeBaseY -      \
      (half);                                                                  \
  WRAP_TO_BYTE(y)

// Writes OAM entries i to i + 3: four sprites of `size` pixels, mirrored into
// the four quarters of one picture at (x, y)
#define MIRRORED_QUAD(i, size, shape, tile)                                    \
  INTRO_OBJ(i).attr01 = ((x + 4) << 16) | y | ((shape) + 0x00002400);          \
  INTRO_OBJ((i) + 1).attr01 =                                                  \
      ((x + 4 + (size)) << 16) | y | ((shape) + 0x10002400);                   \
  INTRO_OBJ((i) + 2).attr01 =                                                  \
      ((x + 4) << 16) | (u8)(y + (size)) | ((shape) + 0x20002400);             \
  INTRO_OBJ((i) + 3).attr01 =                                                  \
      ((x + 4 + (size)) << 16) | (u8)(y + (size)) | ((shape) + 0x30002400);    \
  INTRO_OBJ(i).attr2 = (tile);                                                 \
  INTRO_OBJ((i) + 1).attr2 = (tile);                                           \
  INTRO_OBJ((i) + 2).attr2 = (tile);                                           \
  INTRO_OBJ((i) + 3).attr2 = (tile)

// A VBlank task of the title intro: places the rising sprites along the
// slope of the scrolling background, alternating two sets every other frame
// so twice as many show, hides the rest and copies the list to OAM.
void Title_UpdateIntroSprites(void) {
  /* FAKEMATCH: the OAM shadow it builds is struct OamShadow, volatile words
   * (tag in oambuf.h): without the qualifier differ 205, 471 instructions for
   * 496. */
  struct IntroWork *work;
  u32 spriteCount;
  s32 slopeBaseY;
  s32 slopeTopY;
  s32 x;
  s32 y;
  s32 stripY;

  work = Ram_WorkSlot[43];
  spriteCount = 0;
  if (gDebugPaused == 0) {
    if ((++work->tick & 3) == 0)
      work->rise++;
  }
  slopeBaseY = 48 - gBgScroll[1].y;
  slopeTopY = 144 - work->rise;
  if (work->frame < 0x118) {
    if ((work->tick & 1) == 0) {
      PLACE_ON_SLOPE(0, 16);
      MIRRORED_QUAD(spriteCount, 16, 0x40000000, 232);
      spriteCount += 4;
      PLACE_ON_SLOPE(2, 16);
      INTRO_OBJ(spriteCount).attr01 = ((x + 4) << 16) | y | 0x80002400;
      INTRO_OBJ(spriteCount).attr2 = 128;
      spriteCount++;
      PLACE_ON_SLOPE(4, 32);
      MIRRORED_QUAD(spriteCount, 32, 0x80000000, 192);
      spriteCount += 4;
    } else {
      PLACE_ON_SLOPE(1, 16);
      MIRRORED_QUAD(spriteCount, 16, 0x40000000, 232);
      spriteCount += 4;
      PLACE_ON_SLOPE(3, 16);
      MIRRORED_QUAD(spriteCount, 16, 0x40000000, 224);
      spriteCount += 4;
      PLACE_ON_SLOPE(5, 32);
      MIRRORED_QUAD(spriteCount, 32, 0x80000000, 160);
      spriteCount += 4;
    }
  }
  for (; spriteCount <= 119; spriteCount++)
    INTRO_OBJ(spriteCount).attr01 = 0x400020a0;
  REG_BLDCNT = 0x3f50;
  REG_BLDALPHA = 0x0e0e;
  Dma_Set(&INTRO_OBJ(0), OAM,
          ((spriteCount * 8) >> 2) | (DMA_ENABLE | DMA_32BIT) << 16, REG_DMA3);
  stripY = 32 - gBgScroll[1].y;
  WRAP_TO_BYTE(stripY);
  INTRO_OBJ(12).attr01 = stripY | 0xc05c2000;
  INTRO_OBJ(12).attr2 = 0x800;
  Dma_Set(&INTRO_OBJ(12), (u8 *)OAM + spriteCount * 8,
          (DMA_ENABLE | DMA_32BIT) << 16 | 2, REG_DMA3);
  Dma_Set(&INTRO_OBJ(0), OAM, (DMA_ENABLE | DMA_32BIT) << 16 | 8, REG_DMA3);
}

// Loads the title intro's two scrolling backgrounds and its sprite tiles,
// builds their maps and sets the blending and windows.
void Title_LoadIntroBackgrounds(void) {
  struct IntroWork *work;
  u8 *resourceData;
  u16 *tilemap;
  s32 column;
  s32 row;
  u16 tileIndex;
  s32 blankTile;

  work = Ram_WorkSlot[43];
  REG_DISPCNT = 0;
  resourceData = Resource_GetTableEntry((s32)&ResourceId_IntroGraphicsA);
  Dma_Set(resourceData, (void *)OBJ_PLTT, (DMA_ENABLE | DMA_32BIT) << 16 | 0x80,
          REG_DMA3);
  OBJ_PLTT[0] = 0;
  resourceData += 0x200;
  Resource_DecodeByteLz(resourceData, Ram_MapCellBuffer);
  Dma_Set(Ram_MapCellBuffer, (void *)0x06010000, DMA_ENABLE << 16 | 0xf00,
          REG_DMA3);
  resourceData = Resource_GetTableEntry((s32)&ResourceId_IntroGraphicsC);
  Dma_Set(resourceData, (void *)BG_PLTT, (DMA_ENABLE | DMA_32BIT) << 16 | 0x80,
          REG_DMA3);
  BG_PLTT[0] = 0;
  resourceData += 0x200;
  Resource_DecodeByteLz(resourceData, Ram_MapCellBuffer);
  Dma_Set(Ram_MapCellBuffer + 0x2940, BG_CHAR_BLOCK(0),
          DMA_ENABLE << 16 | 0x2760, REG_DMA3);
  Dma_Set(Ram_MapCellBuffer + 0xa140, (void *)0x06004ec0,
          DMA_ENABLE << 16 | 0x4ec0, REG_DMA3);

  blankTile = 0x1ff;
  tilemap = (u16 *)BG_SCREEN_BLOCK(30);
  tileIndex = 0x267;
  for (row = 0; row <= 10; row++) {
    for (column = 0; column < 30; column++)
      *tilemap++ = tileIndex++;
    *tilemap++ = blankTile;
    *tilemap++ = blankTile;
  }
  tileIndex = 0x13b;
  for (row = 11; row <= 31; row++) {
    for (column = 0; column < 30; column++)
      *tilemap++ = tileIndex++;
    *tilemap++ = blankTile;
    *tilemap++ = blankTile;
  }
  tilemap = (u16 *)BG_SCREEN_BLOCK(31);
  tileIndex = 300;
  for (row = 0; row <= 10; row++) {
    for (column = 0; column < 30; column++)
      *tilemap++ = tileIndex++;
    *tilemap++ = blankTile;
    *tilemap++ = blankTile;
  }
  tileIndex = 0;
  for (row = 11; row <= 31; row++) {
    for (column = 0; column < 30; column++)
      *tilemap++ = tileIndex++;
    *tilemap++ = blankTile;
    *tilemap++ = blankTile;
  }

  REG_BG1CNT = 0x1f43;
  REG_BG2CNT = 0x1e81;
  REG_WIN0H = 0xf0;
  REG_WIN0V = 0x9f;
  REG_WIN1H = 0xf0;
  REG_WIN1V = 0x9f;
  REG_WININ = 0x1616;
  for (row = 0; row < 4; row++) {
    gBgScroll[row].y = 0;
    gBgScroll[row].x = 0;
  }
  gBgScroll[1].y = 96;
  gBgScroll[2].y = 96;
  work->frame = 0;
  work->backRows = 0;
  work->frontRows = 0;
  work->tick = 0;
  work->rise = 0;
  work->state = 0;
  Dma_Set(gBgScroll, REG_BG0HOFS, (DMA_ENABLE | DMA_32BIT) << 16 | 4, REG_DMA3);
  REG_BLDCNT = 0x3fbf;
  REG_BLDALPHA = 0x1010;
  REG_BLDCNT = 0x3f44;
}

// The "Press Start" prompt: its tile count, rows and sprites, and where it
// stands, by edition.
#if (LANGUAGE == LANGUAGE_GERMAN)
#define PROMPT_TILES 8
#else
#define PROMPT_TILES 5
#endif
#if (LANGUAGE == LANGUAGE_GERMAN)
#define PROMPT_ROWS 2
#define PROMPT_SPRITES 3
#define PROMPT_X 80
#define PROMPT_Y 116
#elif (LANGUAGE == LANGUAGE_JAPANESE)
#define PROMPT_SPRITES 5
#define PROMPT_X 56
#define PROMPT_Y 126
#elif (LANGUAGE == LANGUAGE_FRENCH)
#define PROMPT_SPRITES 5
#define PROMPT_X 56
#define PROMPT_Y 124
#else
#define PROMPT_SPRITES 3
#define PROMPT_X 80
#define PROMPT_Y 124
#endif

// Plays the title intro: the backgrounds scroll down and the sprites rise
// until frame 278 (A or Start skips ahead), then the screen flashes white
// into the title picture. With `prompt` set the "Press Start" prompt pulses
// for up to a minute, otherwise the picture stays five seconds. Returns 1
// when A or Start was pressed on the title picture.
s32 Title_ShowIntro(s32 prompt) {
  struct IntroWork *work;
  s32 result;
  u8 savedCpuLoadDisplay;
  u16 clearValue;
  u8 *resourceData;
  u16 *tilemap;
  u32 i;
  u16 tile;
  u32 j;
  u32 holdFrames;
  s32 blankTile;

  result = 0;
  blankTile = 0x1ff;
  savedCpuLoadDisplay = gCpuLoadDisplayEnabled;
  work = Runtime_AllocateHeapBlock(43, 224);
  DebugText_Clear();
  VramCache_Reset();
  WaitFrames(1);
  Scheduler_ResetTaskTable();
  gOamCopyEnabled = result;
  gCpuLoadDisplayEnabled = result;
  Title_LoadIntroBackgrounds();
  PaletteFade_Init();
  PaletteFade_SetCurrent(2, 0);
  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT, 0xf740, 0x20000);
  PaletteFade_Start(60);
  Scheduler_AddOrUpdateCallback(Title_UpdateIntroSprites, 0x480);

  for (;;) {
    s32 frame;
    s32 nextFrame;
    s32 tick;

    frame = work->frame;
    if (frame > 20 && frame < 239 &&
        (gKeysPressed & (KEY_A | KEY_START)) != 0) {
      work->state = 1;
      work->frame = 239;
      frame = 239;
    }
    nextFrame = frame + 1;
    work->frame = nextFrame;
    if (nextFrame <= 278) {
      tick = work->tick;
      if (tick % 3 == 0) {
        s32 y;

        gBgScroll[1].y--;
        y = gBgScroll[1].y;
        if ((y & 7) == 0) {
          Dma_Set(Ram_MapCellBuffer + 0x2580 - work->backRows * 960,
                  (u8 *)0x06004b00 - work->backRows * 960,
                  DMA_ENABLE << 16 | 0x1e0, REG_DMA3);
          work->backRows++;
          tick = work->tick;
        }
      }
      if ((tick & 1) == 0) {
        s32 y;

        gBgScroll[2].y--;
        y = gBgScroll[2].y;
        if ((y & 7) == 0) {
          if (work->frontRows * 8 <= 24) {
            Dma_Set(Ram_MapCellBuffer + 0x99c0 - work->frontRows * 1920,
                    (u8 *)0x0600e4c0 - work->frontRows * 1920,
                    DMA_ENABLE << 16 | 0x3c0, REG_DMA3);
          } else {
            clearValue = 0;
            Dma_Set(&clearValue,
                    (u8 *)0x06004ec0 + (160 - work->frontRows * 8) % 160 * 240,
                    (DMA_ENABLE | DMA_SRC_FIXED) << 16 | 0x3c0, REG_DMA3);
            tilemap = (u16 *)0x0600f6c0;
            for (i = 0; i < 5; i++) {
              for (j = 0; j < 30; j++)
                *tilemap++ = 0x13b;
              *tilemap++ = 0x13b;
              *tilemap++ = 0x13b;
            }
          }
          work->frontRows++;
        }
      }
      if (work->frame == 239)
        work->state = 1;
    } else if (nextFrame == 281) {
      work->state = 2;
    } else if (nextFrame == 289) {
      work->state = 0;
    } else if (nextFrame == 280) {
      Fade_ToWhite(1);
      Scheduler_RemoveCallback(Title_UpdateIntroSprites);
      gOamCopyEnabled = 1;
      WaitFrames(1);
      REG_BG2CNT = 0x681;
      REG_DISPCNT = 0x1440;
      gBgScroll[2].y = 0;
      resourceData = Resource_GetTableEntry((s32)&ResourceId_IntroGraphicsB);
      Dma_Set(resourceData, (void *)BG_PLTT,
              (DMA_ENABLE | DMA_32BIT) << 16 | 0x78, REG_DMA3);
      BG_PLTT[0] = 0;
      resourceData += 0x200;
      Resource_DecodeByteLz(resourceData, Ram_MapCellBuffer);
      Dma_Set(Ram_MapCellBuffer, BG_CHAR_BLOCK(1), DMA_ENABLE << 16 | 0x4b00,
              REG_DMA3);
      tilemap = (u16 *)BG_SCREEN_BLOCK(6);
      tile = 0x100;
      i = 0;
      break;
    }
    WaitFrames(1);
  }
  for (; i < 20; i++) {
    for (j = 0; j < 30; j++)
      *tilemap++ = tile++;
    *tilemap++ = blankTile;
    *tilemap++ = blankTile;
  }

  Ui_LoadWindowGraphics();
  DebugText_Clear();
  if (prompt != 0) {
    u8 *buffer = Runtime_AllocateBlock(14, 0x400);
    struct IntroSprite *sprite = work->area.prompt.sprites;

    Resource_DecodeByteLz(gTitle_PromptTiles, buffer);
    for (i = 0; i < PROMPT_TILES; i++) {
      s32 vramBlock =
          VramCache_Load(Resource_FindFreeEntry(), 128, buffer + (i << 8) / 2);
      u32 *spriteWords = (u32 *)sprite;

      *spriteWords++ = 0;
      sprite++;
      *spriteWords++ = 0x40004000;
      *spriteWords = vramBlock;
    }
    Runtime_ReleaseHeapBlock(14);
  }
  Fade_FromWhite(30);
  Fade_Wait();
  REG_DISPCNT = 0x1540;
  if (prompt != 0)
    holdFrames = 3600;
  else
    holdFrames = 300;
  for (i = 0; i < holdFrames; i++) {
    if (prompt != 0) {
      struct IntroSprite *sprite = work->area.prompt.sprites;
      u32 level;
      s32 x;
#if (LANGUAGE == LANGUAGE_GERMAN)
      u32 row;
      s32 y;

      for (row = 0, y = PROMPT_Y; row < PROMPT_ROWS; y += 8, row++) {
        for (j = 0, x = PROMPT_X; j < PROMPT_SPRITES; j++) {
          sprite->x = x;
          sprite->y = y;
          Runtime_PushSlotEntry(sprite, 0);
          x += 32;
          sprite++;
        }
      }
#else

      for (j = 0, x = PROMPT_X; j < PROMPT_SPRITES; j++) {
        sprite->x = x;
        sprite->y = PROMPT_Y;
        Runtime_PushSlotEntry(sprite, 0);
        x += 32;
        sprite++;
      }
#endif
      level = gTitle_PromptBlendLevels[i % 60];
      REG_BLDCNT = 0x2f50;
      REG_BLDALPHA = (16 - level) * 256 + level;
    }
    if ((gKeysPressed & (KEY_A | KEY_START)) != 0) {
      result = 1;
      break;
    }
    WaitFrames(1);
  }
  gCpuLoadDisplayEnabled = savedCpuLoadDisplay;
  Runtime_ReleaseHeapBlock(43);
  REG_BLDCNT = 0;
  REG_BLDALPHA = 0;
  WaitFrames(1);
  return result;
}

// Unused palette entry retained in the far-call table.
UNUSED void Title_NoOpPalette(void) {}

// Shows the Nintendo logo with its jingle. With `mode` 0 it fades in and
// holds two seconds; otherwise it waits a second, fades in, holds three
// seconds and fades out, faster once skipped. Returns -1 when A or Start
// skipped it.
s32 Title_ShowSplashScreen(s32 mode) {
  s32 result;
  u8 *resourceData;
  u16 *tilemap;
  u32 column;
  u32 rowOrHoldFrame;
  u16 tileIndex;
  s32 blankTile;
  s32 resourceId;

  AudioCommand_PlayFar(SE_110);
  gOamCopyEnabled = 1;
  resourceId = (s32)&ResourceId_NintendoLogo;
  blankTile = 0x1ff;
  Scheduler_ResetTaskTable();
  Fade_ToBlack(1);
  DebugText_Clear();
  WaitFrames(1);
  REG_BG2CNT = 0x681;
  REG_DISPCNT = 0x1440;
  result = 0;
  gBgScroll[2].y = result;
  resourceData = Resource_GetTableEntry(resourceId);
  Dma_Set(resourceData, (void *)BG_PLTT, (DMA_ENABLE | DMA_32BIT) << 16 | 0x70,
          REG_DMA3);
  Resource_DecodeType01(resourceData + 448, Ram_MapCellBuffer);
  Dma_Set(Ram_MapCellBuffer, BG_CHAR_BLOCK(1),
          (DMA_ENABLE | DMA_32BIT) << 16 | 0x2580, REG_DMA3);
  tileIndex = 256;
  tilemap = (u16 *)BG_SCREEN_BLOCK(6);
  for (rowOrHoldFrame = 0; rowOrHoldFrame < 20; rowOrHoldFrame++) {
    for (column = 0; column < 30; column++)
      *tilemap++ = tileIndex++;
    *tilemap++ = blankTile;
    *tilemap++ = blankTile;
  }
  for (rowOrHoldFrame = 0; rowOrHoldFrame < 4; rowOrHoldFrame++) {
    gBgScroll[rowOrHoldFrame].y = 0;
    gBgScroll[rowOrHoldFrame].x = 0;
  }
  Dma_Set(gBgScroll, REG_BG0HOFS, (DMA_ENABLE | DMA_32BIT) << 16 | 4, REG_DMA3);
  Ui_LoadWindowGraphics();
  DebugText_Clear();
  REG_DISPCNT = 0x1540;
  if (mode == 0) {
    Fade_FromBlack(1);
    Fade_Wait();
    for (rowOrHoldFrame = 0; rowOrHoldFrame < 120; rowOrHoldFrame++) {
      if (gKeysPressed & (KEY_A | KEY_START)) {
        result = -1;
        break;
      }
      WaitFrames(1);
    }
    return result;
  }
  for (rowOrHoldFrame = 0; rowOrHoldFrame < 60; rowOrHoldFrame++) {
    if (gKeysPressed & (KEY_A | KEY_START)) {
      result = -1;
      break;
    }
    WaitFrames(1);
  }
  if (result != 0)
    Fade_FromBlack(8);
  else
    Fade_FromBlack(60);
  Fade_Wait();
  if (result == 0) {
    for (rowOrHoldFrame = 0; rowOrHoldFrame < 180; rowOrHoldFrame++) {
      if (gKeysPressed & (KEY_A | KEY_START)) {
        result = -1;
        break;
      }
      WaitFrames(1);
    }
  }
  if (result != 0)
    Fade_ToBlack(8);
  else
    Fade_ToBlack(60);
  Fade_Wait();
  return result;
}

// Shows the Camelot logo for two seconds, cycling its four frames of tiles.
// A or Start ends it early. Returns 0.
s32 Title_ShowAnimatedSplash(void) {
  s32 result;
  u32 workIndex;
  u8 *resourceData;
  s32 resourceId;

  gOamCopyEnabled = 1;
  result = 0;
  resourceId = (s32)&ResourceId_CamelotLogo;
  Scheduler_ResetTaskTable();
  Fade_ToBlack(1);
  DebugText_Clear();
  WaitFrames(1);
  REG_BG2CNT = 0x685;
  REG_DISPCNT = 0x1440;
  gBgScroll[2].y = result;
  /* FAKEMATCH: needed to match: workIndex carries the decode destination
   * before counting; a separate pointer is coalesced with resourceData
   * (31 differ). */
  workIndex = (u32)Ram_MapCellBuffer;
  Resource_DecodeType01(Resource_GetTableEntry(resourceId), (u8 *)workIndex);
  resourceData = (u8 *)workIndex;
  Dma_Set(resourceData, (void *)BG_PLTT,
          (DMA_ENABLE | DMA_32BIT) << 16 | 0x1c0 / 4, REG_DMA3);
  resourceData += 0x1c0;
  Dma_Set(resourceData, BG_SCREEN_BLOCK(6),
          (DMA_ENABLE | DMA_32BIT) << 16 | 0x800 / 4, REG_DMA3);
  resourceData += 0x800;
  Dma_Set(resourceData, BG_CHAR_BLOCK(1),
          (DMA_ENABLE | DMA_32BIT) << 16 | 0x4000 / 4, REG_DMA3);
  resourceData += 0x4000;
  for (workIndex = 0; workIndex < 4; workIndex++) {
    gBgScroll[workIndex].y = 0;
    gBgScroll[workIndex].x = 0;
  }
  Dma_Set(gBgScroll, REG_BG0HOFS, (DMA_ENABLE | DMA_32BIT) << 16 | 4, REG_DMA3);
  Ui_LoadWindowGraphics();
  DebugText_Clear();
  Fade_FromWhite(1);
  Fade_Wait();
  REG_DISPCNT = 0x1540;
  for (workIndex = 0; workIndex < 120; workIndex++) {
    Dma_Set(((gFrameCount >> 3) & 3) * 0x400 + resourceData,
            BG_CHAR_BLOCK(1) + 0x100,
            (DMA_ENABLE | DMA_32BIT) << 16 | 0x340 / 4, REG_DMA3);
    if (gKeysPressed & (KEY_A | KEY_START))
      break;
    WaitFrames(1);
  }
  return result;
}

// Unused color entry retained in the far-call table.
UNUSED void Graphics_NoOpColor(void) {}

// The palette fade (struct PaletteFade) keeps the saved palettes, the
// colour components shown now, the target components and each one's step.

// Computes each of the 1536 colour components' change per frame from the
// current components to the target (nothing when `frames` is not
// positive).
void PaletteFade_ComputeSteps(s16 *sourceComponents, s16 *targetComponents,
                              s16 *componentSteps, s32 frames) {
  s32 remaining;
  s32 sourceValue;
  s32 targetValue;

  if (frames > 0) {
    remaining = 3 * 512 - 1;
    do {
      sourceValue = *sourceComponents;
      targetValue = *targetComponents;
      *componentSteps = Iwram_SignedDivide(targetValue - sourceValue, frames);
      remaining--;
      sourceComponents++;
      targetComponents++;
      componentSteps++;
    } while (remaining >= 0);
  }
}

// A task that steps the palette fade each frame: adds the step to every
// colour component until the fade is done, packs the components into the
// spare RGB555 copy of the palettes and queues both halves for VBlank.
void PaletteFade_Update(void) {
  struct PaletteFade *work = (struct PaletteFade *)gPaletteBufferWork;
  u16 *componentSteps = work->delta;
  u16 *currentComponents;
  u16 *packedColors;
  s32 remaining;
  u16 *paletteBank;
  s32 blueMask;

  if (work->duration == 0)
    return;
  if ((s8)++work->step < work->duration) {
    currentComponents = work->current;
    for (remaining = 0; remaining <= 0x5ff; remaining++)
      *currentComponents++ += *componentSteps++;
  } else {
    Dma_Set(work->target, work->current, (DMA_ENABLE | DMA_32BIT) << 16 | 0x300,
            REG_DMA3);
    work->duration = 0;
  }
  packedColors = work->packed[work->page ^ 1];
  blueMask = 0x7c00;
  remaining = 512;
  currentComponents = work->current;
  for (; remaining != 0; remaining--) {
    *packedColors++ = (currentComponents[0] & blueMask) |
                      (((s16)currentComponents[1] >> 5) & 0x3e0) |
                      (((s16)currentComponents[2] >> 10) & 0x1f);
    currentComponents += 3;
  }
  work->page ^= 1;
  paletteBank = work->packed[work->page];
  {
    u16 *backgroundColors = paletteBank;

    IO_WRITE_QUEUE_PUSH(0x05000000, (u32)backgroundColors,
                        (DMA_ENABLE | DMA_32BIT) << 16 | 0x80);
  }
  IO_WRITE_QUEUE_PUSH(0x05000200, (u32)(paletteBank + 256),
                      (DMA_ENABLE | DMA_32BIT) << 16 | 0x80);
}

// Transforms 512 source colours into destination components (three
// per colour, each in the high bits of a halfword). `mode` picks the
// transform: a colour below 0x8000 fills every colour with it; 0x8000 takes
// that colour from source[0]; 0x10000 to 0x10007 are the whole-palette filters
// (plain, grey, sepia and the others); the TINT, BLEND and SPLIT flags carry
// a colour in their low bits; anything else is the address of components to
// copy. paletteHalf 1 does only the background palettes, 2 only the sprites'.
void Palette_Transform(u32 mode, u16 *source, u16 *destination,
                       s32 paletteHalf) {
  u32 colorIndex;
  u32 colorCount = 512;
  s32 red;
  s32 blue;
  u32 colorWork;
  s32 green;

  if (mode == PALETTE_TRANSFORM_FROM_SOURCE)
    mode = *source;
  if (paletteHalf == 1) {
    colorCount = 256;
  } else if (paletteHalf == 2) {
    destination += 768;
    source += 256;
    colorCount = 256;
  }

  if (mode < PALETTE_TRANSFORM_FROM_SOURCE) {
    *destination++ = mode & 0x7c00;
    *destination++ = (mode & 0x03e0) << 5;
    *destination++ = (mode & 0x001f) << 10;
    Dma_Set(destination - 3, destination,
            (((colorCount - 1) * 6) >> 1) | (DMA_ENABLE << 16), REG_DMA3);
  } else if (mode < PALETTE_TRANSFORM_FLAGS) {
    switch (mode) {
    case PALETTE_TRANSFORM_GREY:
      for (colorIndex = 0; colorIndex < colorCount; colorIndex++) {
        colorWork = *source++;
        red = ((colorWork << 11) & 0xf800);
        green = ((colorWork << 7) & 0x1f000);
        blue = (colorWork & 0x7c00);
        colorWork = Iwram_SignedDivide(red + green + blue, 7);
        *destination++ = colorWork;
        *destination++ = colorWork;
        *destination++ = colorWork;
      }
      break;
    case PALETTE_TRANSFORM_SEPIA:
      for (colorIndex = 0; colorIndex < colorCount; colorIndex++) {
        colorWork = *source++;
        red = colorWork & 31;
        colorWork = Iwram_SignedDivide(
            red + ((colorWork >> 5) & 31) + ((colorWork >> 10) & 31), 10);
        red = colorWork * 4 + 5;
        blue = colorWork * 3 + 5;
        /* FAKEMATCH: the reversible XOR initialization of green folds late,
         * keeping the native blue-first add/copy and separate lower test; green
         * = blue before the two XORs exchanges the add/copy registers (four
         * aligned differences), plain clamps with green = blue, or both set to
         * colorWork * 3 + 5, 202 (green takes the computed register, blue the
         * copy, green's test not threaded into blue's). */
        green = blue ^ red;
        green ^= red;
        if (red < 8)
          red = 8;
        if (blue < 8) {
          green = 8;
          /* FAKEMATCH: needed to match: green is 8, so this pair changes no
           * bits. Its late fold retains the separate blue lower test; an
           * ordinary copy threads past that test and loses two instructions. */
          blue ^= green;
          blue ^= 8;
        }
        if (blue < 8)
          blue = 8;
        if (red > 28)
          red = 28;
        if (green > 28)
          green = 28;
        if (blue > 28)
          blue = 28;
        *destination++ = gPalette_ChannelCurveLight[blue];
        *destination++ = gPalette_ChannelCurveLight[green];
        *destination++ = gPalette_ChannelCurveLight[red];
      }
      break;
    case PALETTE_TRANSFORM_WARM:
      for (colorIndex = 0; colorIndex < colorCount; colorIndex++) {
        colorWork = *source++;
        red = colorWork & 31;
        green = (colorWork >> 5) & 31;
        blue = (colorWork >> 10) & 31;
        red -= (u32)red >> 1;
        green -= green / 3;
        red += 6;
        green += 4;
        blue -= 6;
        red = Graphics_ClampRgb555Channel(red);
        green = Graphics_ClampRgb555Channel(green);
        blue = Graphics_ClampRgb555Channel(blue);
        SET_VECTOR(destination, gPalette_ChannelCurveContrast[blue],
                   gPalette_ChannelCurveLight[green],
                   gPalette_ChannelCurve[red]);
        destination += 3;
      }
      break;
    case PALETTE_TRANSFORM_MUTED:
      for (colorIndex = 0; colorIndex < colorCount; colorIndex++) {
        colorWork = *source++;
        red = colorWork & 31;
        green = (colorWork >> 5) & 31;
        blue = (colorWork >> 10) & 31;
        if (red < 10)
          red = 10;
        if (green < 16)
          green = 16;
        if (blue < 16)
          blue = 16;
        if (red > 28)
          red = 28;
        if (green > 24)
          green = 24;
        if (blue > 26)
          blue = 26;
        green += 2;
        blue += 2;
        red = Graphics_ClampRgb555Channel(red);
        green = Graphics_ClampRgb555Channel(green);
        blue = Graphics_ClampRgb555Channel(blue);
        *destination++ = gPalette_ChannelCurve[blue];
        *destination++ = gPalette_ChannelCurve[green];
        *destination++ = gPalette_ChannelCurve[red];
      }
      break;
    case PALETTE_TRANSFORM_PALE:
      for (colorIndex = 0; colorIndex < colorCount; colorIndex++) {
        s32 mean;

        colorWork = *source++;
        red = colorWork & 31;
        green = (colorWork >> 5) & 31;
        blue = (colorWork >> 10) & 31;
        mean = Graphics_ClampRgb555Channel((red + green + blue) / 3);
        red = (red >> 1) + mean;
        green = (green >> 1) + mean;
        blue = (blue >> 1) + mean;
        red = Graphics_ClampRgb555Channel(red);
        green = Graphics_ClampRgb555Channel(green);
        blue = Graphics_ClampRgb555Channel(blue);
        SET_VECTOR(destination, gPalette_ChannelCurveContrast[blue],
                   gPalette_ChannelCurveContrast[green],
                   gPalette_ChannelCurveContrast[red]);
        destination += 3;
      }
      break;
    case PALETTE_TRANSFORM_RED:
      for (colorIndex = 0; colorIndex < colorCount; colorIndex++) {
        colorWork = *source++;
        red = colorWork & 31;
        green = (colorWork >> 5) & 31;
        blue = (colorWork >> 10) & 31;
        red += ((green >> 3) + (blue >> 3));
        red = Graphics_ClampRgb555Channel(red);
        green -= green / 3;
        blue -= blue / 3;
        SET_VECTOR(destination, gPalette_ChannelCurve[blue],
                   gPalette_ChannelCurve[green],
                   gPalette_ChannelCurveLight[red]);
        destination += 3;
      }
      break;
    case PALETTE_TRANSFORM_WARM2:
      for (colorIndex = 0; colorIndex < colorCount; colorIndex++) {
        colorWork = *source++;
        red = colorWork & 31;
        green = (colorWork >> 5) & 31;
        blue = (colorWork >> 10) & 31;
        red -= (u32)red >> 1;
        green -= green / 3;
        red += 6;
        green += 4;
        blue -= 6;
        red = Graphics_ClampRgb555Channel(red);
        green = Graphics_ClampRgb555Channel(green);
        blue = Graphics_ClampRgb555Channel(blue);
        SET_VECTOR(destination, gPalette_ChannelCurveContrast[blue],
                   gPalette_ChannelCurveLight[green],
                   gPalette_ChannelCurve[red]);
        destination += 3;
      }
      break;
    default:
      for (colorIndex = 0; colorIndex < colorCount; colorIndex++) {
        colorWork = *source++;
        destination[colorIndex * 3 + 0] = colorWork & 0x7c00;
        destination[colorIndex * 3 + 1] = (colorWork & 0x03e0) << 5;
        destination[colorIndex * 3 + 2] = (colorWork & 0x001f) << 10;
      }
      break;
    }
  } else if (mode & PALETTE_TRANSFORM_TINT) {
    u32 tintRed;
    u32 tintGreen;
    u32 tintBlue;

    tintRed = mode & 31;
    tintGreen = (mode >> 5) & 31;
    tintBlue = (mode >> 10) & 31;
    for (colorIndex = 0; colorIndex < colorCount; colorIndex++) {
      colorWork = *source++;
      red = ((colorWork << 11) & 0xf800);
      green = ((colorWork << 7) & 0x1f000);
      blue = (colorWork & 0x7c00);
      colorWork = Iwram_SignedDivide(red + green + blue, 96);
      red = colorWork * tintRed;
      green = colorWork * tintGreen;
      blue = colorWork * tintBlue;
      red = Graphics_ClampRgb555Component(red);
      green = Graphics_ClampRgb555Component(green);
      blue = Graphics_ClampRgb555Component(blue);
      SET_VECTOR(destination, blue, green, red);
      destination += 3;
    }
  } else if (mode & PALETTE_TRANSFORM_BLEND) {
    u32 tintRed;
    u32 tintGreen;
    u32 tintBlue;

    tintRed = mode & 31;
    tintGreen = (mode >> 5) & 31;
    tintBlue = (mode >> 10) & 31;
    for (colorIndex = 0; colorIndex < colorCount; colorIndex++) {
      colorWork = *source++;
      red = colorWork & 31;
      colorWork = Iwram_SignedDivide(
          (red + ((colorWork >> 5) & 31) + ((colorWork >> 10) & 31)) << 4,
          tintRed + tintGreen + tintBlue);
      red = Iwram_MulQ16(((colorWork * tintRed) >> 4) << 16,
                         (s32)(tintRed << 16) >> 4);
      green = Iwram_MulQ16(((colorWork * tintGreen) >> 4) << 16,
                           (s32)(tintGreen << 16) >> 4);
      blue = Iwram_MulQ16(((colorWork * tintBlue) >> 4) << 16,
                          (s32)(tintBlue << 16) >> 4);
      red = Graphics_ClampRgb555Channel((u32)red >> 16);
      green = Graphics_ClampRgb555Channel((u32)green >> 16);
      blue = Graphics_ClampRgb555Channel((u32)blue >> 16);
      *destination++ = gPalette_ChannelCurve[blue];
      *destination++ = gPalette_ChannelCurve[green];
      *destination++ = gPalette_ChannelCurve[red];
    }
  } else if (mode & PALETTE_TRANSFORM_SPLIT) {
    for (colorIndex = 0; colorIndex < colorCount; colorIndex++) {
      colorWork = *source++;
      destination[colorIndex * 3 + 0] = colorWork & 0x7c00;
      destination[colorIndex * 3 + 1] = (colorWork & 0x03e0) << 5;
      destination[colorIndex * 3 + 2] = (colorWork & 0x001f) << 10;
    }
  } else {
    if (paletteHalf == 2)
      mode += 0x600;
    Dma_Set((const void *)mode, destination,
            ((colorCount * 6) >> 2) | (DMA_ENABLE | DMA_32BIT) << 16, REG_DMA3);
  }
}

// Allocates the palette fade, saves both hardware palettes in it as the
// starting colours and schedules PaletteFade_Update.
void PaletteFade_Init(void) {
  struct PaletteFade *buffer;
  s32 callbackPriority;

  buffer = Runtime_AllocateBlock(32, sizeof(*buffer));
  Dma_Fill32(0, buffer, sizeof(*buffer));
  Dma_Set((void *)0x05000000, buffer, (DMA_ENABLE | DMA_32BIT) << 16 | 0x80,
          REG_DMA3);
  Dma_Set((void *)0x05000200, &buffer->palette[256],
          (DMA_ENABLE | DMA_32BIT) << 16 | 0x80, REG_DMA3);
  Palette_Transform(PALETTE_TRANSFORM_PLAIN, buffer->palette, buffer->target,
                    0);
  callbackPriority = 3200;
  Scheduler_AddOrUpdateCallback(PaletteFade_Update, callbackPriority);
}

// Stops the palette fade and frees its buffers.
void PaletteFade_Release(void) {
  Scheduler_RemoveCallback(&PaletteFade_Update);
  Runtime_ReleaseHeapBlock(0x20);
}

// Sets the fade target by transforming the saved background and/or sprite
// palettes through the requested mode.
void PaletteFade_SetTarget(s32 mode, s32 paletteHalf) {
  struct PaletteFade *fade = (struct PaletteFade *)gPaletteBufferWork;

  if (fade != NULL)
    Palette_Transform(mode, fade->palette, fade->target, paletteHalf);
}

// Sets the colours shown now by transforming the saved palettes.
void PaletteFade_SetCurrent(s32 mode, s32 paletteHalf) {
  struct PaletteFade *fade = (struct PaletteFade *)gPaletteBufferWork;

  if (fade != NULL)
    Palette_Transform(mode, fade->palette, fade->current, paletteHalf);
}

// Replaces the first saved colour, the backdrop.
void PaletteFade_SetBackdrop(s32 color) {
  struct PaletteFade *fade = (struct PaletteFade *)gPaletteBufferWork;

  if (fade != NULL)
    fade->palette[0] = color;
}

// Starts fading the shown colours to the target over the requested frames.
void PaletteFade_Start(s32 frames) {
  struct PaletteFade *fade = (struct PaletteFade *)gPaletteBufferWork;

  if (fade != NULL) {
    fade->duration = frames;
    fade->step = 0;
    PaletteFade_ComputeSteps((s16 *)fade->current, (s16 *)fade->target,
                             (s16 *)fade->delta, frames);
  }
}

// Clamps a five-bit colour channel to 0..31.
s32 Graphics_ClampRgb555Channel(s32 value) {
  if (value > 31)
    return 31;
  if (value < 0)
    value = 0;
  return value;
}

// Caps a colour component at 0x7c00, the largest a five-bit channel shifted
// into the high bits reaches. There is no lower bound.
s32 Graphics_ClampRgb555Component(s32 value) {
  if (value > 0x7c00)
    value = 0x7c00;
  return value;
}

// The response curves of Palette_Transform's filters. Like the
// declarations in palbuf.h they are not const: as const the code differs.

// Maps a channel (0..31) to a component (0..31 << 10)
// clang-format off
u16 gPalette_ChannelCurve[32] = {
    0, 524, 1071, 1643, 2240, 2863, 3513, 4191,
    4898, 5635, 6403, 7202, 8035, 8901, 9803, 10740,
    11715, 12729, 13783, 14877, 16014, 17195, 18422, 19694,
    21015, 22386, 23808, 25283, 26812, 28397, 30041, 31744
};

// A brighter curve
u16 gPalette_ChannelCurveLight[32] = {
    0, 613, 1247, 1902, 2580, 3281, 4005, 4753,
    5526, 6324, 7148, 7999, 8876, 9782, 10716, 11680,
    12674, 13699, 14755, 15843, 16965, 18121, 19312, 20538,
    21801, 23101, 24440, 25818, 27237, 28697, 30199, 31744
};

// A high-contrast curve (25 and above are full)
u16 gPalette_ChannelCurveContrast[32] = {
    0, 360, 759, 1201, 1689, 2226, 2817, 3466,
    4178, 4957, 5808, 6738, 7752, 8856, 10058, 11365,
    12784, 14325, 15996, 17806, 19767, 21888, 24183, 26662,
    29340, 31744, 31744, 31744, 31744, 31744, 31744, 31744
};
// clang-format on
