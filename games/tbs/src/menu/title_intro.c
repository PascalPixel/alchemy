/// @file
/// The title intro, the Nintendo and Camelot logos, and the palette fade the
/// intro uses.

#include "menu/title_intro.h"
#include "macros.h"
#include "resource_ids.h"
#include "battle/battle.h"
#include "field/field.h"
#include "field/map_grid_scroll.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "system/dma.h"
#include "system/frame.h"
#include "system/io_write_queue.h"
#include "system/iwram_call.h"
#include "system/oam_words.h"
#include "system/ram_buffer.h"
#include "system/system.h"

/// A sprite of the "Press Start" prompt, linked into a render slot each frame:
/// OAM attributes 0 to 2 with the position split out.
struct IntroSprite {
  u32 next; ///< the render slot link
  u8 y;
  u8 attr0High; ///< shape and mode bits of attribute 0
  u16 x : 9;
  u16 attr1High : 7; ///< flip and size bits of attribute 1
  s32 tile;
};

/// The intro's frames: A or START skips to the approach only after the first
/// INTRO_SKIP_AFTER_FRAME, and the screen flashes white into the title at
/// INTRO_FLASH_FRAME.
#define INTRO_SKIP_AFTER_FRAME 20
#define INTRO_APPROACH_FRAME 239
#define INTRO_FLASH_FRAME 280

/// IntroWork.state, which the intro writes and nothing reads.
enum IntroState {
  INTRO_STATE_SCROLLING = 0, ///< the pictures scroll in
  /// From frame 239, or a skip with A or START: the approach to the flash.
  INTRO_STATE_APPROACH = 1,
  /// Set from frame 281, which the loop never reaches: it ends at 280.
  INTRO_STATE_AFTER_FLASH = 2
};

/// OAM entries the intro builds each VBlank.
#define INTRO_OBJECT_COUNT 120

/// The title intro's state, in HEAP_SLOT_TITLE_INTRO.
struct IntroWork {
  s32 backRows;  ///< rows of the far background copied in as it scrolls
  s32 frontRows; ///< rows of the near background copied in
  s32 frame;
  s32 frameCount;
  s32 state; ///< an enum IntroState
  s32 rise;  ///< how far the sprites have risen
  union {
    struct OamWords objects[INTRO_OBJECT_COUNT];
    struct {
      u8 introObjects[104]; ///< objects[0] to [12], still the intro's
      struct IntroSprite sprites[8];
    } prompt;
  } area;
};

/// The intro's two pictures share character block 0: the far one (BG1) in
/// 16-colour tiles, the near one (BG2) in 256-colour tiles from tile 315.
#define INTRO_BACK_TILE(n) (BG_CHAR_BLOCK(0) + (n) * TILE_SIZE_4BPP)
#define INTRO_FRONT_TILE(n) (BG_CHAR_BLOCK(0) + (n) * 2 * TILE_SIZE_4BPP)

/// The screen blocks of the two pictures' maps
#define INTRO_FRONT_SCREEN 30
#define INTRO_BACK_SCREEN 31
/// Both pictures start scrolled this far down and scroll up into place.
#define INTRO_SCROLL_START 96

/// The x of each of the six columns of intro sprites
const u8 gTitle_IntroSpriteColumns[] = {96, 136, 140, 160, 168, 184};

/// The prompt pulses once a second.
#define PROMPT_PULSE_FRAMES FRAMES_PER_SECOND

/// The prompt's blend level over its pulse
// clang-format off
const u8 gTitle_PromptBlendLevels[PROMPT_PULSE_FRAMES] = {
    1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
    16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
    16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
    15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0
};
// clang-format on

#define INTRO_OBJ(i) (work->area.objects[i])

/// Brings `v` into 0..255 by whole turns of 256
#define WRAP_TO_BYTE(v)                                                        \
  while ((v) > 255)                                                            \
    (v) -= 256;                                                                \
  while ((v) < 0)                                                              \
  (v) += 256

/// The slope the sprites rise along runs from slopeBaseY at this x to slopeTopY
/// at INTRO_SLOPE_RIGHT.
#define INTRO_SLOPE_LEFT 104
#define INTRO_SLOPE_RIGHT 184

/// Sets x and y to the top left of a sprite of half-width `half` centred on
/// column `column`, on the slope.
#define PLACE_ON_SLOPE(column, half)                                           \
  x = gTitle_IntroSpriteColumns[column] - (half);                              \
  y = (slopeTopY - slopeBaseY) * ((x + (half)) - INTRO_SLOPE_LEFT) /           \
          (INTRO_SLOPE_RIGHT - INTRO_SLOPE_LEFT) +                             \
      slopeBaseY - (half);                                                     \
  WRAP_TO_BYTE(y)

/// Writes OAM entries i to i + 3: four sprites of `size` pixels, mirrored into
/// the four quarters of one picture at (x, y)
#define MIRRORED_QUAD(i, size, shape, tile)                                    \
  INTRO_OBJ(i).attr01 = OBJ_X(x + 4) | y | (shape) |                           \
                        OBJ_MODE(OBJ_MODE_SEMI_TRANSPARENT) | OBJ_256_COLORS;  \
  INTRO_OBJ((i) + 1).attr01 = OBJ_X(x + 4 + (size)) | y | (shape) |            \
                              OBJ_MODE(OBJ_MODE_SEMI_TRANSPARENT) |            \
                              OBJ_256_COLORS | OBJ_HFLIP;                      \
  INTRO_OBJ((i) + 2).attr01 = OBJ_X(x + 4) | (u8)(y + (size)) | (shape) |      \
                              OBJ_MODE(OBJ_MODE_SEMI_TRANSPARENT) |            \
                              OBJ_256_COLORS | OBJ_VFLIP;                      \
  INTRO_OBJ((i) + 3).attr01 = OBJ_X(x + 4 + (size)) | (u8)(y + (size)) |       \
                              (shape) | OBJ_MODE(OBJ_MODE_SEMI_TRANSPARENT) |  \
                              OBJ_256_COLORS | OBJ_HFLIP | OBJ_VFLIP;          \
  INTRO_OBJ(i).attr2 = (tile);                                                 \
  INTRO_OBJ((i) + 1).attr2 = (tile);                                           \
  INTRO_OBJ((i) + 2).attr2 = (tile);                                           \
  INTRO_OBJ((i) + 3).attr2 = (tile)

/// A VBlank task of the title intro, added by Title_ShowIntro: places the
/// rising sprites along the slope of the scrolling background, alternating two
/// sets every other VBlank so twice as many show, hides the rest and copies the
/// list to OAM.
void Title_UpdateIntroSprites(void) {
  // The OAM shadow it builds is struct OamWords, whose words are volatile
  // (oam_words.h).
  struct IntroWork *work;
  u32 spriteCount;
  s32 slopeBaseY;
  s32 slopeTopY;
  s32 x;
  s32 y;
  s32 stripY;

  work = HEAP_SLOT_BLOCK(HEAP_SLOT_TITLE_INTRO);
  spriteCount = 0;
  if (gDebugPaused == 0) {
    if ((++work->frameCount & 3) == 0)
      work->rise++;
  }
  slopeBaseY = 48 - gBgScroll[1].y;
  slopeTopY = 144 - work->rise;
  // Alternating the two groups fits six shapes into twelve OAM entries;
  // each group is visible every other VBlank.
  if (work->frame < INTRO_FLASH_FRAME) {
    if ((work->frameCount & 1) == 0) {
      PLACE_ON_SLOPE(0, 16);
      MIRRORED_QUAD(spriteCount, 16, OBJ_SIZE_16X16, 232);
      spriteCount += 4;
      PLACE_ON_SLOPE(2, 16);
      INTRO_OBJ(spriteCount).attr01 = OBJ_X(x + 4) | y | OBJ_SIZE_32X32 |
                                      OBJ_MODE(OBJ_MODE_SEMI_TRANSPARENT) |
                                      OBJ_256_COLORS;
      INTRO_OBJ(spriteCount).attr2 = 128;
      spriteCount++;
      PLACE_ON_SLOPE(4, 32);
      MIRRORED_QUAD(spriteCount, 32, OBJ_SIZE_32X32, 192);
      spriteCount += 4;
    } else {
      PLACE_ON_SLOPE(1, 16);
      MIRRORED_QUAD(spriteCount, 16, OBJ_SIZE_16X16, 232);
      spriteCount += 4;
      PLACE_ON_SLOPE(3, 16);
      MIRRORED_QUAD(spriteCount, 16, OBJ_SIZE_16X16, 224);
      spriteCount += 4;
      PLACE_ON_SLOPE(5, 32);
      MIRRORED_QUAD(spriteCount, 32, OBJ_SIZE_32X32, 160);
      spriteCount += 4;
    }
  }
  for (; spriteCount < INTRO_OBJECT_COUNT; spriteCount++)
    INTRO_OBJ(spriteCount).attr01 =
        DISPLAY_HEIGHT | OBJ_SIZE_16X16 | OBJ_256_COLORS;
  REG_BLDCNT = BLDCNT_TGT1_OBJ | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
  REG_BLDALPHA = BLEND_ALPHA(14, 14);
  Dma_Set(&INTRO_OBJ(0), OAM, DMA_COPY_WORDS((spriteCount * 8) >> 2), REG_DMA3);
  // The strip follows the back layer and occupies the first OAM entry
  // beyond the 120-entry list; it reuses a hidden shadow-buffer entry.
  stripY = 32 - gBgScroll[1].y;
  WRAP_TO_BYTE(stripY);
  INTRO_OBJ(12).attr01 = stripY | OBJ_X(92) | OBJ_SIZE_64X64 | OBJ_256_COLORS;
  INTRO_OBJ(12).attr2 = OBJ_PRIORITY(2);
  Dma_Set(&INTRO_OBJ(12), (u8 *)OAM + spriteCount * 8, DMA_COPY_WORDS(2),
          REG_DMA3);
  Dma_Set(&INTRO_OBJ(0), OAM, DMA_COPY_WORDS(8), REG_DMA3);
}

/// Loads the title intro's two scrolling backgrounds and its sprite tiles,
/// builds their tilemaps and sets the blending and windows.
void Title_LoadIntroBackgrounds(void) {
  struct IntroWork *work;
  u8 *resourceData;
  u16 *tilemap;
  s32 column;
  s32 row;
  u16 tileIndex;

  work = HEAP_SLOT_BLOCK(HEAP_SLOT_TITLE_INTRO);
  REG_DISPCNT = 0;
  resourceData = Resource_GetTableEntry(RESOURCE_INTRO_GRAPHICS_A);
  Dma_Set(resourceData, (void *)OBJ_PLTT, DMA_COPY_WORDS(BG_PLTT_SIZE / 4),
          REG_DMA3);
  OBJ_PLTT[0] = 0;
  resourceData += BG_PLTT_SIZE;
  Decompress_ByteLz(resourceData, Ram_MapGridCellBuffer);
  Dma_Set(Ram_MapGridCellBuffer, (void *)OBJ_VRAM0, DMA_COPY_HALFWORDS(3840),
          REG_DMA3);
  resourceData = Resource_GetTableEntry(RESOURCE_INTRO_GRAPHICS_C);
  Dma_Set(resourceData, (void *)BG_PLTT, DMA_COPY_WORDS(BG_PLTT_SIZE / 4),
          REG_DMA3);
  BG_PLTT[0] = 0;
  resourceData += BG_PLTT_SIZE;
  Decompress_ByteLz(resourceData, Ram_MapGridCellBuffer);
  Dma_Set(Ram_MapGridCellBuffer + 10560, BG_CHAR_BLOCK(0),
          DMA_COPY_HALFWORDS(10080), REG_DMA3);
  Dma_Set(Ram_MapGridCellBuffer + 41280, INTRO_FRONT_TILE(315),
          DMA_COPY_HALFWORDS(20160), REG_DMA3);

  {
    s32 blankTile = 511;

    tilemap = (u16 *)BG_SCREEN_BLOCK(INTRO_FRONT_SCREEN);
    tileIndex = 615;
    for (row = 0; row <= 10; row++) {
      for (column = 0; column < DISPLAY_TILE_WIDTH; column++)
        *tilemap++ = tileIndex++;
      *tilemap++ = blankTile;
      *tilemap++ = blankTile;
    }
    tileIndex = 315;
    for (row = 11; row <= 31; row++) {
      for (column = 0; column < DISPLAY_TILE_WIDTH; column++)
        *tilemap++ = tileIndex++;
      *tilemap++ = blankTile;
      *tilemap++ = blankTile;
    }
    tilemap = (u16 *)BG_SCREEN_BLOCK(INTRO_BACK_SCREEN);
    tileIndex = 300;
    for (row = 0; row <= 10; row++) {
      for (column = 0; column < DISPLAY_TILE_WIDTH; column++)
        *tilemap++ = tileIndex++;
      *tilemap++ = blankTile;
      *tilemap++ = blankTile;
    }
    tileIndex = 0;
    for (row = 11; row <= 31; row++) {
      for (column = 0; column < DISPLAY_TILE_WIDTH; column++)
        *tilemap++ = tileIndex++;
      *tilemap++ = blankTile;
      *tilemap++ = blankTile;
    }

    REG_BG1CNT =
        BGCNT_PRIORITY(3) | BGCNT_MOSAIC | BGCNT_SCREEN_BASE(INTRO_BACK_SCREEN);
    REG_BG2CNT = BGCNT_PRIORITY(1) | BGCNT_256_COLORS |
                 BGCNT_SCREEN_BASE(INTRO_FRONT_SCREEN);
    REG_WIN0H = WIN_RANGE(0, DISPLAY_WIDTH);
    REG_WIN0V = WIN_RANGE(0, DISPLAY_HEIGHT - 1);
    REG_WIN1H = WIN_RANGE(0, DISPLAY_WIDTH);
    REG_WIN1V = WIN_RANGE(0, DISPLAY_HEIGHT - 1);
    REG_WININ = WININ_WIN0_BG1 | WININ_WIN0_BG2 | WININ_WIN0_OBJ |
                WININ_WIN1_BG1 | WININ_WIN1_BG2 | WININ_WIN1_OBJ;
    for (row = 0; row < 4; row++) {
      gBgScroll[row].y = 0;
      gBgScroll[row].x = 0;
    }
    gBgScroll[1].y = INTRO_SCROLL_START;
    gBgScroll[2].y = INTRO_SCROLL_START;
    work->frame = 0;
    work->backRows = 0;
    work->frontRows = 0;
    work->frameCount = 0;
    work->rise = 0;
    work->state = INTRO_STATE_SCROLLING;
    Dma_Set(gBgScroll, REG_BG0HOFS, DMA_COPY_WORDS(4), REG_DMA3);
    REG_BLDCNT = BLDCNT_TGT1_ALL | BLDCNT_EFFECT_LIGHTEN | BLDCNT_TGT2_ALL;
    REG_BLDALPHA = BLEND_ALPHA(BLEND_WEIGHT_MAX, BLEND_WEIGHT_MAX);
    REG_BLDCNT = BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
  }
}

/// How long the title waits for A or START: a minute with the prompt shown,
/// five seconds without.
#define TITLE_PROMPT_HOLD_FRAMES (60 * FRAMES_PER_SECOND)
#define TITLE_HOLD_FRAMES (5 * FRAMES_PER_SECOND)

// The "Press Start" prompt: its tile count, rows and sprites, and where it
// stands, by edition.
#if LANGUAGE == LANGUAGE_GERMAN
#define PROMPT_TILES 8
#else
#define PROMPT_TILES 5
#endif
#if LANGUAGE == LANGUAGE_GERMAN
#define PROMPT_ROWS 2
#define PROMPT_SPRITES 3
#define PROMPT_X 80
#define PROMPT_Y 116
#elif LANGUAGE == LANGUAGE_JAPANESE
#define PROMPT_SPRITES 5
#define PROMPT_X 56
#define PROMPT_Y 126
#elif LANGUAGE == LANGUAGE_FRENCH
#define PROMPT_SPRITES 5
#define PROMPT_X 56
#define PROMPT_Y 124
#else
#define PROMPT_SPRITES 3
#define PROMPT_X 80
#define PROMPT_Y 124
#endif

/// Plays the title intro: the backgrounds scroll down and the sprites rise
/// until frame 278 (A or Start skips ahead), then the screen flashes white into
/// the title picture.
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
  s32 blankTile = 511;

  result = 0;
  savedCpuLoadDisplay = gCpuLoadDisplayEnabled;
  work = HeapSlot_Alloc(HEAP_SLOT_TITLE_INTRO, 224);
  DebugText_Clear();
  VramCache_Reset();
  wait(1);
  Task_ResetAll();
  gOamCopyEnabled = result;
  gCpuLoadDisplayEnabled = result;
  Title_LoadIntroBackgrounds();
  TitlePaletteFade_Initialize();
  TitlePaletteFade_SetCurrent(RGB(2, 0, 0), PALETTES_ALL);
  // This screen supplies its own OAM list in VBlank.
  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT,
                      DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON | DISPCNT_BG1_ON |
                          DISPCNT_BG2_ON | DISPCNT_OBJ_ON | DISPCNT_WIN0_ON |
                          DISPCNT_WIN1_ON | DISPCNT_OBJWIN_ON,
                      IO_WRITE_HALFWORD);
  TitlePaletteFade_Start(60);
  Task_Add(Title_UpdateIntroSprites, TASK_PASS_VBLANK);

  for (;;) {
    s32 frame;
    s32 nextFrame;
    s32 frameCount;

    // A skip retains the final approach to the flash rather than replacing
    // the animated screen at an arbitrary scroll position.
    frame = work->frame;
    if (frame > INTRO_SKIP_AFTER_FRAME && frame < INTRO_APPROACH_FRAME &&
        (gKeysPressed & (KEY_A | KEY_START)) != 0) {
      work->state = INTRO_STATE_APPROACH;
      work->frame = INTRO_APPROACH_FRAME;
      frame = INTRO_APPROACH_FRAME;
    }
    nextFrame = frame + 1;
    work->frame = nextFrame;
    if (nextFrame <= INTRO_FLASH_FRAME - 2) {
      // The back layer (BG1) scrolls a pixel every third frame, the front
      // (BG2) every other one; each new row of eight pixels copies the next
      // band of the picture from the decode buffer before it becomes visible.
      frameCount = work->frameCount;
      if (frameCount % 3 == 0) {
        s32 y;

        gBgScroll[1].y--;
        y = gBgScroll[1].y;
        if ((y & 7) == 0) {
          Dma_Set(Ram_MapGridCellBuffer + 9600 - work->backRows * 960,
                  INTRO_BACK_TILE(600) - work->backRows * 960,
                  DMA_COPY_HALFWORDS(480), REG_DMA3);
          work->backRows++;
          frameCount = work->frameCount;
        }
      }
      if ((frameCount & 1) == 0) {
        s32 y;

        gBgScroll[2].y--;
        y = gBgScroll[2].y;
        if ((y & 7) == 0) {
          if (work->frontRows * 8 <= 24) {
            Dma_Set(Ram_MapGridCellBuffer + 39360 - work->frontRows * 1920,
                    INTRO_FRONT_TILE(915) - work->frontRows * 1920,
                    DMA_COPY_HALFWORDS(960), REG_DMA3);
          } else {
            // Once the finite picture is exhausted, clear incoming bands
            // so wrapped tile memory cannot expose earlier picture rows.
            clearValue = 0;
            Dma_Set(&clearValue,
                    INTRO_FRONT_TILE(315) +
                        (DISPLAY_HEIGHT - work->frontRows * 8) %
                            DISPLAY_HEIGHT * DISPLAY_WIDTH,
                    DMA_FILL_HALFWORDS(960), REG_DMA3);
            tilemap = (u16 *)BG_SCREEN_BLOCK(INTRO_FRONT_SCREEN) +
                      27 * BG_SCREEN_WIDTH;
            for (i = 0; i < 5; i++) {
              for (j = 0; j < DISPLAY_TILE_WIDTH; j++)
                *tilemap++ = 315;
              *tilemap++ = 315;
              *tilemap++ = 315;
            }
          }
          work->frontRows++;
        }
      }
      if (work->frame == INTRO_APPROACH_FRAME)
        work->state = INTRO_STATE_APPROACH;
    } else if (nextFrame == INTRO_FLASH_FRAME + 1) {
      // The frame-280 branch below exits before these later states can run.
      work->state = INTRO_STATE_AFTER_FLASH;
    } else if (nextFrame == INTRO_FLASH_FRAME + 9) {
      work->state = INTRO_STATE_SCROLLING;
    } else if (nextFrame == INTRO_FLASH_FRAME) {
      // Hide the background replacement behind white, and restore the
      // normal OAM uploader before drawing the title prompt.
      ScreenFade_ToWhite(1);
      Task_Remove(Title_UpdateIntroSprites);
      gOamCopyEnabled = 1;
      wait(1);
      REG_BG2CNT = BGCNT_PRIORITY(1) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(6);
      REG_DISPCNT = DISPCNT_OBJ_1D_MAP | DISPCNT_BG2_ON | DISPCNT_OBJ_ON;
      gBgScroll[2].y = 0;
      resourceData = Resource_GetTableEntry(RESOURCE_INTRO_GRAPHICS_B);
      Dma_Set(resourceData, (void *)BG_PLTT, DMA_COPY_WORDS(120), REG_DMA3);
      BG_PLTT[0] = 0;
      resourceData += BG_PLTT_SIZE;
      Decompress_ByteLz(resourceData, Ram_MapGridCellBuffer);
      Dma_Set(Ram_MapGridCellBuffer, BG_CHAR_BLOCK(1),
              DMA_COPY_HALFWORDS(19200), REG_DMA3);
      tilemap = (u16 *)BG_SCREEN_BLOCK(6);
      tile = 256;
      i = 0;
      break;
    }
    wait(1);
  }
  // Blank the two off-screen columns so the 30-tile picture occupies a
  // complete 32-tile hardware map row.
  for (; i < DISPLAY_TILE_HEIGHT; i++) {
    for (j = 0; j < DISPLAY_TILE_WIDTH; j++)
      *tilemap++ = tile++;
    *tilemap++ = blankTile;
    *tilemap++ = blankTile;
  }

  Window_LoadGraphics();
  DebugText_Clear();
  if (prompt != 0) {
    u8 *buffer = HeapSlot_AllocEwram(HEAP_SLOT_TILE_SCRATCH, 1024);
    struct IntroSprite *sprite = work->area.prompt.sprites;

    Decompress_ByteLz(gTitle_PromptTiles, buffer);
    for (i = 0; i < PROMPT_TILES; i++) {
      s32 vramBlock =
          VramCache_Load(VramCache_FindFreeEntry(), 128, buffer + (i << 8) / 2);
      u32 *spriteWords = (u32 *)sprite;

      *spriteWords++ = 0;
      sprite++;
      *spriteWords++ = OBJ_SIZE_32X8;
      *spriteWords = vramBlock;
    }
    HeapSlot_Release(HEAP_SLOT_TILE_SCRATCH);
  }
  ScreenFade_FromWhite(30);
  ScreenFade_Wait();
  REG_DISPCNT =
      DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON | DISPCNT_BG2_ON | DISPCNT_OBJ_ON;
  if (prompt != 0)
    holdFrames = TITLE_PROMPT_HOLD_FRAMES;
  else
    holdFrames = TITLE_HOLD_FRAMES;
  for (i = 0; i < holdFrames; i++) {
    if (prompt != 0) {
      struct IntroSprite *sprite = work->area.prompt.sprites;
      u32 level;
      s32 x;
#if LANGUAGE == LANGUAGE_GERMAN
      u32 row;
      s32 y;

      for (row = 0, y = PROMPT_Y; row < PROMPT_ROWS; y += 8, row++) {
        for (j = 0, x = PROMPT_X; j < PROMPT_SPRITES; j++) {
          sprite->x = x;
          sprite->y = y;
          Oam_AddSpriteAtDepth(sprite, 0);
          x += 32;
          sprite++;
        }
      }
#else

      for (j = 0, x = PROMPT_X; j < PROMPT_SPRITES; j++) {
        sprite->x = x;
        sprite->y = PROMPT_Y;
        Oam_AddSpriteAtDepth(sprite, 0);
        x += 32;
        sprite++;
      }
#endif
      // The prompt pulses once a second: its blend weight over the picture.
      level = gTitle_PromptBlendLevels[i % PROMPT_PULSE_FRAMES];
      REG_BLDCNT = BLDCNT_TGT1_OBJ | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_BG0 |
                   BLDCNT_TGT2_BG1 | BLDCNT_TGT2_BG2 | BLDCNT_TGT2_BG3 |
                   BLDCNT_TGT2_BD;
      REG_BLDALPHA = (BLEND_WEIGHT_MAX - level) * 256 + level;
    }
    if ((gKeysPressed & (KEY_A | KEY_START)) != 0) {
      result = 1;
      break;
    }
    wait(1);
  }
  gCpuLoadDisplayEnabled = savedCpuLoadDisplay;
  HeapSlot_Release(HEAP_SLOT_TITLE_INTRO);
  REG_BLDCNT = 0;
  REG_BLDALPHA = 0;
  wait(1);
  return result;
}

/// Unused: palette entry retained in the far-call table.
UNUSED void Title_NoOpPalette(void) {}

/// Each logo resource begins with its 224 colours.
#define LOGO_PALETTE_BYTES (224 * 2)

/// Shows the Nintendo logo with its jingle.
s32 Title_ShowNintendoLogo(s32 mode) {
  s32 result;
  u8 *resourceData;
  u16 *tilemap;
  u32 column;
  u32 rowOrHoldFrame;
  u16 tileIndex;
  s32 blankTile = 511;
  s32 resourceId;

  Sound_PlayFar(SE_CHIME);
  gOamCopyEnabled = 1;
  resourceId = RESOURCE_NINTENDO_LOGO;
  Task_ResetAll();
  ScreenFade_ToBlack(1);
  DebugText_Clear();
  wait(1);
  REG_BG2CNT = BGCNT_PRIORITY(1) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(6);
  REG_DISPCNT = DISPCNT_OBJ_1D_MAP | DISPCNT_BG2_ON | DISPCNT_OBJ_ON;
  result = 0;
  gBgScroll[2].y = result;
  // The resource begins with 224 colours; its packed tiles follow them.
  resourceData = Resource_GetTableEntry(resourceId);
  Dma_Set(resourceData, (void *)BG_PLTT, DMA_COPY_WORDS(LOGO_PALETTE_BYTES / 4),
          REG_DMA3);
  Decompress_TaggedLz(resourceData + LOGO_PALETTE_BYTES, Ram_MapGridCellBuffer);
  Dma_Set(Ram_MapGridCellBuffer, BG_CHAR_BLOCK(1), DMA_COPY_WORDS(9600),
          REG_DMA3);
  // Pad the 30-tile picture to the hardware map's 32-tile row width.
  tileIndex = 256;
  tilemap = (u16 *)BG_SCREEN_BLOCK(6);
  for (rowOrHoldFrame = 0; rowOrHoldFrame < DISPLAY_TILE_HEIGHT;
       rowOrHoldFrame++) {
    for (column = 0; column < DISPLAY_TILE_WIDTH; column++)
      *tilemap++ = tileIndex++;
    *tilemap++ = blankTile;
    *tilemap++ = blankTile;
  }
  for (rowOrHoldFrame = 0; rowOrHoldFrame < 4; rowOrHoldFrame++) {
    gBgScroll[rowOrHoldFrame].y = 0;
    gBgScroll[rowOrHoldFrame].x = 0;
  }
  Dma_Set(gBgScroll, REG_BG0HOFS, DMA_COPY_WORDS(4), REG_DMA3);
  Window_LoadGraphics();
  DebugText_Clear();
  REG_DISPCNT =
      DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON | DISPCNT_BG2_ON | DISPCNT_OBJ_ON;
  if (mode == 0) {
    ScreenFade_FromBlack(1);
    ScreenFade_Wait();
    for (rowOrHoldFrame = 0; rowOrHoldFrame < 2 * FRAMES_PER_SECOND;
         rowOrHoldFrame++) {
      if (gKeysPressed & (KEY_A | KEY_START)) {
        result = -1;
        break;
      }
      wait(1);
    }
    return result;
  }
  // Otherwise a second of black, a second's fade in, three seconds held and
  // a second's fade out; a skip shortens each fade to eight frames.
  for (rowOrHoldFrame = 0; rowOrHoldFrame < FRAMES_PER_SECOND;
       rowOrHoldFrame++) {
    if (gKeysPressed & (KEY_A | KEY_START)) {
      result = -1;
      break;
    }
    wait(1);
  }
  if (result != 0)
    ScreenFade_FromBlack(8);
  else
    ScreenFade_FromBlack(FRAMES_PER_SECOND);
  ScreenFade_Wait();
  if (result == 0) {
    for (rowOrHoldFrame = 0; rowOrHoldFrame < 3 * FRAMES_PER_SECOND;
         rowOrHoldFrame++) {
      if (gKeysPressed & (KEY_A | KEY_START)) {
        result = -1;
        break;
      }
      wait(1);
    }
  }
  if (result != 0)
    ScreenFade_ToBlack(8);
  else
    ScreenFade_ToBlack(FRAMES_PER_SECOND);
  ScreenFade_Wait();
  return result;
}

/// Shows the Camelot logo for two seconds, cycling its four frames of tiles. A
/// or Start ends it early.
s32 Title_ShowCamelotLogo(void) {
  u32 workIndex;
  u8 *resourceData;
  s32 resourceId;
  s32 result;

  gOamCopyEnabled = 1;
  result = 0;
  resourceId = RESOURCE_CAMELOT_LOGO;
  Task_ResetAll();
  ScreenFade_ToBlack(1);
  DebugText_Clear();
  wait(1);
  REG_BG2CNT = BGCNT_PRIORITY(1) | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS |
               BGCNT_SCREEN_BASE(6);
  REG_DISPCNT = DISPCNT_OBJ_1D_MAP | DISPCNT_BG2_ON | DISPCNT_OBJ_ON;
  gBgScroll[2].y = result;
  resourceData = Resource_GetTableEntry(resourceId);
  Decompress_TaggedLz(resourceData, Ram_MapGridCellBuffer);
  // The decoded resource: 224 colours, the map, the still tiles, then four
  // animation pictures of 1024 bytes each.
  resourceData = Ram_MapGridCellBuffer;
  Dma_Set(resourceData, (void *)BG_PLTT, DMA_COPY_WORDS(LOGO_PALETTE_BYTES / 4),
          REG_DMA3);
  resourceData += LOGO_PALETTE_BYTES;
  Dma_Set(resourceData, BG_SCREEN_BLOCK(6), DMA_COPY_WORDS(BG_SCREEN_SIZE / 4),
          REG_DMA3);
  resourceData += BG_SCREEN_SIZE;
  Dma_Set(resourceData, BG_CHAR_BLOCK(1), DMA_COPY_WORDS(BG_CHAR_SIZE / 4),
          REG_DMA3);
  resourceData += BG_CHAR_SIZE;
  for (workIndex = 0; workIndex < 4; workIndex++) {
    gBgScroll[workIndex].y = 0;
    gBgScroll[workIndex].x = 0;
  }
  Dma_Set(gBgScroll, REG_BG0HOFS, DMA_COPY_WORDS(4), REG_DMA3);
  Window_LoadGraphics();
  DebugText_Clear();
  ScreenFade_FromWhite(1);
  ScreenFade_Wait();
  REG_DISPCNT =
      DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON | DISPCNT_BG2_ON | DISPCNT_OBJ_ON;
  // Replace only the animated tiles; the rest of the decoded logo stays
  // in VRAM while the four animation pictures cycle.
  for (workIndex = 0; workIndex < 2 * FRAMES_PER_SECOND; workIndex++) {
    Dma_Set(((gFrameCount >> 3) & 3) * 1024 + resourceData,
            BG_CHAR_BLOCK(1) + 256, DMA_COPY_WORDS(832 / 4), REG_DMA3);
    if (gKeysPressed & (KEY_A | KEY_START))
      break;
    wait(1);
  }
  return result;
}

/// Unused: color entry retained in the far-call table.
UNUSED void Title_NoOpColor(void) {}

// The palette fade (struct TitlePaletteFade) keeps the saved palettes, the
// colour components shown now, the target components and each one's step.

/// Computes each of the 1536 colour components' change per frame from the
/// current components to the target (nothing when `frames` is not positive).
void TitlePaletteFade_CalculateSteps(s16 *sourceComponents,
                                     s16 *targetComponents, s16 *componentSteps,
                                     s32 frames) {
  s32 remaining;
  s32 sourceValue;
  s32 targetValue;

  if (frames > 0) {
    remaining = TITLE_PALETTE_FADE_CHANNELS - 1;
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

/// A task added by TitlePaletteFade_Initialize: steps the palette fade each
/// frame, adding the step to every colour component until the fade is done,
/// packs the components into the spare RGB555 copy of the palettes and queues
/// both palettes for VBlank.
void TitlePaletteFade_Update(void) {
  struct TitlePaletteFade *work = (struct TitlePaletteFade *)gPaletteFadeWork;
  u16 *componentSteps = work->delta;
  u16 *currentComponents;
  u16 *packedColors;
  s32 remaining;
  u16 *pagePalettes;

  if (work->duration == 0)
    return;
  if ((s8)++work->step < work->duration) {
    currentComponents = work->current;
    for (remaining = 0; remaining <= TITLE_PALETTE_FADE_CHANNELS - 1;
         remaining++)
      *currentComponents++ += *componentSteps++;
  } else {
    Dma_Set(work->target, work->current,
            DMA_COPY_WORDS(sizeof(work->current) / 4), REG_DMA3);
    work->duration = 0;
  }
  // The queued DMA keeps a pointer until VBlank.
  packedColors = work->packed[work->page ^ 1];
  {
    s32 blueMask = 0x7c00;

    remaining = PLTT_COLORS;
    currentComponents = work->current;
    for (; remaining != 0; remaining--) {
      *packedColors++ = (currentComponents[0] & blueMask) |
                        (((s16)currentComponents[1] >> 5) & 0x3e0) |
                        (((s16)currentComponents[2] >> 10) & 0x1f);
      currentComponents += 3;
    }
    work->page ^= 1;
    pagePalettes = work->packed[work->page];
    {
      u16 *backgroundColors = pagePalettes;

      IO_WRITE_QUEUE_PUSH((u32)BG_PLTT, (u32)backgroundColors,
                          DMA_COPY_WORDS(BG_PLTT_SIZE / 4));
    }
    IO_WRITE_QUEUE_PUSH((u32)OBJ_PLTT, (u32)(pagePalettes + BG_PLTT_COLORS),
                        DMA_COPY_WORDS(BG_PLTT_SIZE / 4));
  }
}

/// Tints 512 source colours into destination components (three per colour, each
/// in the high bits of a halfword). `mode` picks the tint: a colour below
/// PALETTE_TINT_FROM_SOURCE fills every colour with it;
/// PALETTE_TINT_FROM_SOURCE takes that colour from source[0]; PALETTE_TINT_NONE
/// to PALETTE_TINT_WARM_DARK are the whole-palette filters; the TINT, BLEND and
/// SPLIT flags carry a colour in their low bits; anything else is the address
/// of components to copy. `selectedPalettes` is an enum PaletteSelection.
void TitlePaletteFade_BuildColors(u32 mode, u16 *source, u16 *destination,
                                  s32 selectedPalettes) {
  u32 colorIndex;
  u32 colorCount = PLTT_COLORS;
  s32 red;
  s32 blue;
  u32 colorWork;
  s32 green;

  if (mode == PALETTE_TINT_FROM_SOURCE)
    mode = *source;
  if (selectedPalettes == PALETTES_BG) {
    colorCount = BG_PLTT_COLORS;
  } else if (selectedPalettes == PALETTES_OBJECTS) {
    destination += 3 * BG_PLTT_COLORS;
    source += BG_PLTT_COLORS;
    colorCount = BG_PLTT_COLORS;
  }

  if (mode < PALETTE_TINT_FROM_SOURCE) {
    *destination++ = mode & 0x7c00;
    *destination++ = (mode & 0x03e0) << 5;
    *destination++ = (mode & 0x001f) << 10;
    Dma_Set(destination - 3, destination,
            DMA_COPY_HALFWORDS(((colorCount - 1) * 6) >> 1), REG_DMA3);
  } else if (mode < PALETTE_TINT_FLAGS) {
    switch (mode) {
    case PALETTE_TINT_GRAYSCALE:
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
    case PALETTE_TINT_SEPIA:
      for (colorIndex = 0; colorIndex < colorCount; colorIndex++) {
        colorWork = *source++;
        red = RGB_RED(colorWork);
        colorWork = Iwram_SignedDivide(
            red + (RGB_GREEN(colorWork)) + (RGB_BLUE(colorWork)), 10);
        red = colorWork * 4 + 5;
        blue = colorWork * 3 + 5;
        /* FAKEMATCH(dead): two XORs with red that cancel fold late. */
        green = blue ^ red;
        green ^= red;
        if (red < 8)
          red = 8;
        if (blue < 8) {
          green = 8;
          /* FAKEMATCH(dead): two XORs that cancel, green being 8. */
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
        *destination++ = gTitleIntro_ChannelCurveLight[blue];
        *destination++ = gTitleIntro_ChannelCurveLight[green];
        *destination++ = gTitleIntro_ChannelCurveLight[red];
      }
      break;
    case PALETTE_TINT_WARM:
      for (colorIndex = 0; colorIndex < colorCount; colorIndex++) {
        colorWork = *source++;
        red = RGB_RED(colorWork);
        green = RGB_GREEN(colorWork);
        blue = RGB_BLUE(colorWork);
        red -= (u32)red >> 1;
        green -= green / 3;
        red += 6;
        green += 4;
        blue -= 6;
        red = TitlePaletteFade_ClampComponent(red);
        green = TitlePaletteFade_ClampComponent(green);
        blue = TitlePaletteFade_ClampComponent(blue);
        SET_VECTOR(destination, gTitleIntro_ChannelCurveContrast[blue],
                   gTitleIntro_ChannelCurveLight[green],
                   gTitleIntro_ChannelCurve[red]);
        destination += 3;
      }
      break;
    case PALETTE_TINT_MUTED:
      for (colorIndex = 0; colorIndex < colorCount; colorIndex++) {
        colorWork = *source++;
        red = RGB_RED(colorWork);
        green = RGB_GREEN(colorWork);
        blue = RGB_BLUE(colorWork);
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
        red = TitlePaletteFade_ClampComponent(red);
        green = TitlePaletteFade_ClampComponent(green);
        blue = TitlePaletteFade_ClampComponent(blue);
        *destination++ = gTitleIntro_ChannelCurve[blue];
        *destination++ = gTitleIntro_ChannelCurve[green];
        *destination++ = gTitleIntro_ChannelCurve[red];
      }
      break;
    case PALETTE_TINT_PALE:
      for (colorIndex = 0; colorIndex < colorCount; colorIndex++) {
        s32 mean;

        colorWork = *source++;
        red = RGB_RED(colorWork);
        green = RGB_GREEN(colorWork);
        blue = RGB_BLUE(colorWork);
        mean = TitlePaletteFade_ClampComponent((red + green + blue) / 3);
        red = (red >> 1) + mean;
        green = (green >> 1) + mean;
        blue = (blue >> 1) + mean;
        red = TitlePaletteFade_ClampComponent(red);
        green = TitlePaletteFade_ClampComponent(green);
        blue = TitlePaletteFade_ClampComponent(blue);
        SET_VECTOR(destination, gTitleIntro_ChannelCurveContrast[blue],
                   gTitleIntro_ChannelCurveContrast[green],
                   gTitleIntro_ChannelCurveContrast[red]);
        destination += 3;
      }
      break;
    case PALETTE_TINT_RED:
      for (colorIndex = 0; colorIndex < colorCount; colorIndex++) {
        colorWork = *source++;
        red = RGB_RED(colorWork);
        green = RGB_GREEN(colorWork);
        blue = RGB_BLUE(colorWork);
        red += ((green >> 3) + (blue >> 3));
        red = TitlePaletteFade_ClampComponent(red);
        green -= green / 3;
        blue -= blue / 3;
        SET_VECTOR(destination, gTitleIntro_ChannelCurve[blue],
                   gTitleIntro_ChannelCurve[green],
                   gTitleIntro_ChannelCurveLight[red]);
        destination += 3;
      }
      break;
    case PALETTE_TINT_WARM_DARK:
      for (colorIndex = 0; colorIndex < colorCount; colorIndex++) {
        colorWork = *source++;
        red = RGB_RED(colorWork);
        green = RGB_GREEN(colorWork);
        blue = RGB_BLUE(colorWork);
        red -= (u32)red >> 1;
        green -= green / 3;
        red += 6;
        green += 4;
        blue -= 6;
        red = TitlePaletteFade_ClampComponent(red);
        green = TitlePaletteFade_ClampComponent(green);
        blue = TitlePaletteFade_ClampComponent(blue);
        SET_VECTOR(destination, gTitleIntro_ChannelCurveContrast[blue],
                   gTitleIntro_ChannelCurveLight[green],
                   gTitleIntro_ChannelCurve[red]);
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
  } else if (mode & PALETTE_TINT_CUSTOM) {
    u32 tintRed;
    u32 tintGreen;
    u32 tintBlue;

    tintRed = RGB_RED(mode);
    tintGreen = RGB_GREEN(mode);
    tintBlue = RGB_BLUE(mode);
    for (colorIndex = 0; colorIndex < colorCount; colorIndex++) {
      colorWork = *source++;
      red = ((colorWork << 11) & 0xf800);
      green = ((colorWork << 7) & 0x1f000);
      blue = (colorWork & 0x7c00);
      colorWork = Iwram_SignedDivide(red + green + blue, 96);
      red = colorWork * tintRed;
      green = colorWork * tintGreen;
      blue = colorWork * tintBlue;
      red = TitlePaletteFade_ClampShiftedComponent(red);
      green = TitlePaletteFade_ClampShiftedComponent(green);
      blue = TitlePaletteFade_ClampShiftedComponent(blue);
      SET_VECTOR(destination, blue, green, red);
      destination += 3;
    }
  } else if (mode & PALETTE_TINT_BLEND) {
    u32 tintRed;
    u32 tintGreen;
    u32 tintBlue;

    tintRed = RGB_RED(mode);
    tintGreen = RGB_GREEN(mode);
    tintBlue = RGB_BLUE(mode);
    for (colorIndex = 0; colorIndex < colorCount; colorIndex++) {
      colorWork = *source++;
      red = RGB_RED(colorWork);
      colorWork = Iwram_SignedDivide(
          (red + (RGB_GREEN(colorWork)) + (RGB_BLUE(colorWork))) << 4,
          tintRed + tintGreen + tintBlue);
      red = Iwram_MulFX(((colorWork * tintRed) >> 4) << 16,
                        (s32)(tintRed << 16) >> 4);
      green = Iwram_MulFX(((colorWork * tintGreen) >> 4) << 16,
                          (s32)(tintGreen << 16) >> 4);
      blue = Iwram_MulFX(((colorWork * tintBlue) >> 4) << 16,
                         (s32)(tintBlue << 16) >> 4);
      red = TitlePaletteFade_ClampComponent((u32)red >> 16);
      green = TitlePaletteFade_ClampComponent((u32)green >> 16);
      blue = TitlePaletteFade_ClampComponent((u32)blue >> 16);
      *destination++ = gTitleIntro_ChannelCurve[blue];
      *destination++ = gTitleIntro_ChannelCurve[green];
      *destination++ = gTitleIntro_ChannelCurve[red];
    }
  } else if (mode & PALETTE_TINT_SPLIT) {
    for (colorIndex = 0; colorIndex < colorCount; colorIndex++) {
      colorWork = *source++;
      destination[colorIndex * 3 + 0] = colorWork & 0x7c00;
      destination[colorIndex * 3 + 1] = (colorWork & 0x03e0) << 5;
      destination[colorIndex * 3 + 2] = (colorWork & 0x001f) << 10;
    }
  } else {
    if (selectedPalettes == PALETTES_OBJECTS)
      mode += 3 * BG_PLTT_SIZE;
    Dma_Set((const void *)mode, destination,
            DMA_COPY_WORDS((colorCount * 6) >> 2), REG_DMA3);
  }
}

/// Allocates the palette fade, saves both hardware palettes in it as the
/// starting colours and adds the task TitlePaletteFade_Update.
void TitlePaletteFade_Initialize(void) {
  struct TitlePaletteFade *buffer;

  buffer = HeapSlot_AllocEwram(HEAP_SLOT_PALETTE_FADE, sizeof(*buffer));
  Dma_Fill32(0, buffer, sizeof(*buffer));
  Dma_Set((void *)BG_PLTT, buffer, DMA_COPY_WORDS(BG_PLTT_SIZE / 4), REG_DMA3);
  Dma_Set((void *)OBJ_PLTT, &buffer->palette[BG_PLTT_COLORS],
          DMA_COPY_WORDS(BG_PLTT_SIZE / 4), REG_DMA3);
  TitlePaletteFade_BuildColors(PALETTE_TINT_NONE, buffer->palette,
                               buffer->target, PALETTES_ALL);
  Task_Add(TitlePaletteFade_Update, TASK_PASS_FRAME);
}

/// Unused: removes the palette fade's task and frees its buffers.
UNUSED void TitlePaletteFade_Release(void) {
  Task_Remove(&TitlePaletteFade_Update);
  HeapSlot_Release(HEAP_SLOT_PALETTE_FADE);
}

/// Unused: sets the fade target by tinting the saved background and/or
/// sprite palettes with the requested tint.
UNUSED void TitlePaletteFade_SetTarget(s32 mode, s32 selectedPalettes) {
  struct TitlePaletteFade *fade = (struct TitlePaletteFade *)gPaletteFadeWork;

  if (fade != NULL)
    TitlePaletteFade_BuildColors(mode, fade->palette, fade->target,
                                 selectedPalettes);
}

/// Sets the colours shown now by tinting the saved palettes.
void TitlePaletteFade_SetCurrent(s32 mode, s32 selectedPalettes) {
  struct TitlePaletteFade *fade = (struct TitlePaletteFade *)gPaletteFadeWork;

  if (fade != NULL)
    TitlePaletteFade_BuildColors(mode, fade->palette, fade->current,
                                 selectedPalettes);
}

/// Unused: replaces the first saved colour, the backdrop.
UNUSED void TitlePaletteFade_SetBackdrop(s32 color) {
  struct TitlePaletteFade *fade = (struct TitlePaletteFade *)gPaletteFadeWork;

  if (fade != NULL)
    fade->palette[0] = color;
}

/// Starts fading the shown colours to the target over the requested frames:
/// each of the 1,536 components (three for each of the 512 colours) steps by
/// its distance divided by the frames, the remainder kept in the ten bits below
/// it, so every colour arrives together.
void TitlePaletteFade_Start(s32 frames) {
  struct TitlePaletteFade *fade = (struct TitlePaletteFade *)gPaletteFadeWork;

  if (fade != NULL) {
    fade->duration = frames;
    fade->step = 0;
    TitlePaletteFade_CalculateSteps((s16 *)fade->current, (s16 *)fade->target,
                                    (s16 *)fade->delta, frames);
  }
}

/// Clamps a five-bit colour channel to 0..31.
s32 TitlePaletteFade_ClampComponent(s32 value) {
  if (value > RGB_LEVEL_MAX)
    return RGB_LEVEL_MAX;
  if (value < 0)
    value = 0;
  return value;
}

/// Caps a colour component at 0x7c00, the largest a five-bit channel shifted
/// into the high bits reaches. There is no lower bound.
s32 TitlePaletteFade_ClampShiftedComponent(s32 value) {
  if (value > 0x7c00)
    value = 0x7c00;
  return value;
}

// The response curves of TitlePaletteFade_BuildColors's filters.

/// Maps a channel (0..31) to a component (0..31 << 10).
// clang-format off
u16 gTitleIntro_ChannelCurve[32] = {
    0, 524, 1071, 1643, 2240, 2863, 3513, 4191,
    4898, 5635, 6403, 7202, 8035, 8901, 9803, 10740,
    11715, 12729, 13783, 14877, 16014, 17195, 18422, 19694,
    21015, 22386, 23808, 25283, 26812, 28397, 30041, 31744
};

/// A brighter curve
u16 gTitleIntro_ChannelCurveLight[32] = {
    0, 613, 1247, 1902, 2580, 3281, 4005, 4753,
    5526, 6324, 7148, 7999, 8876, 9782, 10716, 11680,
    12674, 13699, 14755, 15843, 16965, 18121, 19312, 20538,
    21801, 23101, 24440, 25818, 27237, 28697, 30199, 31744
};

/// A high-contrast curve (25 and above are full)
u16 gTitleIntro_ChannelCurveContrast[32] = {
    0, 360, 759, 1201, 1689, 2226, 2817, 3466,
    4178, 4957, 5808, 6738, 7752, 8856, 10058, 11365,
    12784, 14325, 15996, 17806, 19767, 21888, 24183, 26662,
    29340, 31744, 31744, 31744, 31744, 31744, 31744, 31744
};
// clang-format on
