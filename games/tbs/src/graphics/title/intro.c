#include "speaker.h"
#include "unit/graphics/title/intro.h"
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
#include "task.h"
#include "dma.h"
#include "edition.h"
#include "io_reg.h"
#include "io_write_queue.h"
#include "map_grid_scroll.h"
#include "ram_buffer.h"
#include "resource.h"
#include "resource_ids.h"
#include "palbuf.h"
#include "iwram_call.h"
#include "heap.h"
#include "heap_slots.h"
#include "frame.h"
#include "oambuf.h"
#include "macros.h"
#include "songs.h"

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

/// IntroWork.state, which the intro writes and nothing reads.
enum IntroState {
  INTRO_STATE_SCROLLING = 0, ///< the pictures scroll in
  /// From frame 239, or a skip with A or START: the approach to the flash.
  INTRO_STATE_APPROACH = 1,
  /// Set from frame 281, which the loop never reaches: it ends at 280.
  INTRO_STATE_AFTER_FLASH = 2
};

/// The title intro's state, in HEAP_SLOT_TITLE_INTRO.
struct IntroWork {
  s32 backRows;  ///< rows of the far background copied in as it scrolls
  s32 frontRows; ///< rows of the near background copied in
  s32 frame;
  s32 tick;
  s32 state; ///< an enum IntroState
  s32 rise;  ///< how far the sprites have risen
  union {
    struct OamShadow objects[120];
    struct {
      u8 introObjects[0x68]; ///< objects[0] to [12], still the intro's
      struct IntroSprite sprites[8];
    } prompt;
  } area;
};

/// The x of each of the six columns of intro sprites
const u8 gTitle_IntroSpriteColumns[] = {96, 136, 140, 160, 168, 184};

/// The prompt's blend level over its 60-frame pulse
// clang-format off
const u8 gTitle_PromptBlendLevels[60] = {
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

/// Sets x and y to the top left of a sprite of half-width `half` centred on
/// column `column`, between slopeBaseY at x = 104 and slopeTopY at x = 184.
#define PLACE_ON_SLOPE(column, half)                                           \
  x = gTitle_IntroSpriteColumns[column] - (half);                              \
  y = (slopeTopY - slopeBaseY) * ((x + (half)) - 104) / 80 + slopeBaseY -      \
      (half);                                                                  \
  WRAP_TO_BYTE(y)

/// Writes OAM entries i to i + 3: four sprites of `size` pixels, mirrored into
/// the four quarters of one picture at (x, y)
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

/// A VBlank task of the title intro, added by Title_ShowIntro: places the
/// rising sprites along the slope of the scrolling background, alternating two
/// sets every other frame so twice as many show, hides the rest and copies the
/// list to OAM.
void Title_UpdateIntroSprites(void) {
  /* FAKEMATCH(volatile): the OAM shadow it builds is struct OamShadow, whose
   * words are volatile (oambuf.h); as plain u32 words, 157 of 496
   * instructions differ in tbs-en. */
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
    if ((++work->tick & 3) == 0)
      work->rise++;
  }
  slopeBaseY = 48 - gBgScroll[1].y;
  slopeTopY = 144 - work->rise;
  // Alternating the two groups fits six shapes into twelve OAM entries;
  // each group is visible every other frame.
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
  REG_BLDCNT = BLDCNT_TGT1_OBJ | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
  REG_BLDALPHA = BLEND_ALPHA(14, 14);
  Dma_Set(&INTRO_OBJ(0), OAM, DMA_COPY_WORDS((spriteCount * 8) >> 2), REG_DMA3);
  // The strip follows the back layer and occupies the first OAM entry
  // beyond the 120-entry list; it reuses a hidden shadow-buffer entry.
  stripY = 32 - gBgScroll[1].y;
  WRAP_TO_BYTE(stripY);
  INTRO_OBJ(12).attr01 = stripY | 0xc05c2000;
  INTRO_OBJ(12).attr2 = 0x800;
  Dma_Set(&INTRO_OBJ(12), (u8 *)OAM + spriteCount * 8, DMA_COPY_WORDS(2),
          REG_DMA3);
  Dma_Set(&INTRO_OBJ(0), OAM, DMA_COPY_WORDS(8), REG_DMA3);
}

/// Loads the title intro's two scrolling backgrounds and its sprite tiles,
/// builds their tilemaps and sets the blending and windows. The pictures are
/// decoded through Ram_MapGridCellBuffer, the map grid's cell buffer, which is
/// free while no map is loaded. Called by Title_ShowIntro.
void Title_LoadIntroBackgrounds(void) {
  struct IntroWork *work;
  u8 *resourceData;
  u16 *tilemap;
  s32 column;
  s32 row;
  u16 tileIndex;
  s32 blankTile;

  work = HEAP_SLOT_BLOCK(HEAP_SLOT_TITLE_INTRO);
  REG_DISPCNT = 0;
  resourceData = Resource_GetTableEntry((s32)&ResourceId_IntroGraphicsA);
  Dma_Set(resourceData, (void *)OBJ_PLTT, DMA_COPY_WORDS(0x80), REG_DMA3);
  OBJ_PLTT[0] = 0;
  resourceData += 0x200;
  Decompress_ByteLz(resourceData, Ram_MapGridCellBuffer);
  Dma_Set(Ram_MapGridCellBuffer, (void *)OBJ_VRAM0, DMA_ENABLE << 16 | 0xf00,
          REG_DMA3);
  resourceData = Resource_GetTableEntry((s32)&ResourceId_IntroGraphicsC);
  Dma_Set(resourceData, (void *)BG_PLTT, DMA_COPY_WORDS(0x80), REG_DMA3);
  BG_PLTT[0] = 0;
  resourceData += 0x200;
  Decompress_ByteLz(resourceData, Ram_MapGridCellBuffer);
  Dma_Set(Ram_MapGridCellBuffer + 0x2940, BG_CHAR_BLOCK(0),
          DMA_ENABLE << 16 | 0x2760, REG_DMA3);
  Dma_Set(Ram_MapGridCellBuffer + 0xa140, (void *)0x06004ec0,
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

  REG_BG1CNT = BGCNT_PRIORITY(3) | BGCNT_MOSAIC | BGCNT_SCREEN_BASE(31);
  REG_BG2CNT = BGCNT_PRIORITY(1) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(30);
  REG_WIN0H = WIN_RANGE(0, 240);
  REG_WIN0V = WIN_RANGE(0, 159);
  REG_WIN1H = WIN_RANGE(0, 240);
  REG_WIN1V = WIN_RANGE(0, 159);
  REG_WININ = WININ_WIN0_BG1 | WININ_WIN0_BG2 | WININ_WIN0_OBJ |
              WININ_WIN1_BG1 | WININ_WIN1_BG2 | WININ_WIN1_OBJ;
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
  work->state = INTRO_STATE_SCROLLING;
  Dma_Set(gBgScroll, REG_BG0HOFS, DMA_COPY_WORDS(4), REG_DMA3);
  REG_BLDCNT = BLDCNT_TGT1_ALL | BLDCNT_EFFECT_LIGHTEN | BLDCNT_TGT2_ALL;
  REG_BLDALPHA = BLEND_ALPHA(16, 16);
  REG_BLDCNT = BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
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

/// Plays the title intro: the backgrounds scroll down and the sprites rise
/// until frame 278 (A or Start skips ahead), then the screen flashes white
/// into the title picture. With `prompt` set the "Press Start" prompt pulses
/// for up to a minute, otherwise the picture stays five seconds. Reached
/// through the palette far-call table from the title screen. Returns 1 when A
/// or Start was pressed on the title picture.
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
  work = AllocHeapSlot(HEAP_SLOT_TITLE_INTRO, 224);
  DebugText_Clear();
  VramCache_Reset();
  WaitFrames(1);
  Task_ResetAll();
  gOamCopyEnabled = result;
  gCpuLoadDisplayEnabled = result;
  Title_LoadIntroBackgrounds();
  TitlePaletteFade_Initialize();
  TitlePaletteFade_SetCurrent(RGB(2, 0, 0), PALETTES_ALL);
  // This screen supplies its own OAM list in VBlank. Keep the normal OAM
  // copy disabled until its sprite callback is removed at the white flash.
  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT,
                      DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON | DISPCNT_BG1_ON |
                          DISPCNT_BG2_ON | DISPCNT_OBJ_ON | DISPCNT_WIN0_ON |
                          DISPCNT_WIN1_ON | DISPCNT_OBJWIN_ON,
                      IO_WRITE_16);
  TitlePaletteFade_Start(60);
  Task_Add(Title_UpdateIntroSprites, TASK_PRIORITY_VBLANK);

  for (;;) {
    s32 frame;
    s32 nextFrame;
    s32 tick;

    // A skip retains the final approach to the flash rather than replacing
    // the animated screen at an arbitrary scroll position.
    frame = work->frame;
    if (frame > 20 && frame < 239 &&
        (gKeysPressed & (KEY_A | KEY_START)) != 0) {
      work->state = INTRO_STATE_APPROACH;
      work->frame = 239;
      frame = 239;
    }
    nextFrame = frame + 1;
    work->frame = nextFrame;
    if (nextFrame <= 278) {
      // The back layer (BG1) scrolls a pixel every third tick, the front
      // (BG2) every other one; each new row of eight pixels copies the next
      // band of the picture from the decode buffer before it becomes visible.
      tick = work->tick;
      if (tick % 3 == 0) {
        s32 y;

        gBgScroll[1].y--;
        y = gBgScroll[1].y;
        if ((y & 7) == 0) {
          Dma_Set(Ram_MapGridCellBuffer + 0x2580 - work->backRows * 960,
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
            Dma_Set(Ram_MapGridCellBuffer + 0x99c0 - work->frontRows * 1920,
                    (u8 *)0x0600e4c0 - work->frontRows * 1920,
                    DMA_ENABLE << 16 | 0x3c0, REG_DMA3);
          } else {
            // Once the finite picture is exhausted, clear incoming bands
            // so wrapped tile memory cannot expose earlier picture rows.
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
        work->state = INTRO_STATE_APPROACH;
    } else if (nextFrame == 281) {
      // The frame-280 branch below exits before these later states can run.
      work->state = INTRO_STATE_AFTER_FLASH;
    } else if (nextFrame == 289) {
      work->state = INTRO_STATE_SCROLLING;
    } else if (nextFrame == 280) {
      // Hide the background replacement behind white, and restore the
      // normal OAM uploader before drawing the title prompt.
      ScreenFade_ToWhite(1);
      Task_Remove(Title_UpdateIntroSprites);
      gOamCopyEnabled = 1;
      WaitFrames(1);
      REG_BG2CNT = BGCNT_PRIORITY(1) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(6);
      REG_DISPCNT = DISPCNT_OBJ_1D_MAP | DISPCNT_BG2_ON | DISPCNT_OBJ_ON;
      gBgScroll[2].y = 0;
      resourceData = Resource_GetTableEntry((s32)&ResourceId_IntroGraphicsB);
      Dma_Set(resourceData, (void *)BG_PLTT, DMA_COPY_WORDS(0x78), REG_DMA3);
      BG_PLTT[0] = 0;
      resourceData += 0x200;
      Decompress_ByteLz(resourceData, Ram_MapGridCellBuffer);
      Dma_Set(Ram_MapGridCellBuffer, BG_CHAR_BLOCK(1),
              DMA_ENABLE << 16 | 0x4b00, REG_DMA3);
      tilemap = (u16 *)BG_SCREEN_BLOCK(6);
      tile = 0x100;
      i = 0;
      break;
    }
    WaitFrames(1);
  }
  // Blank the two off-screen columns so the 30-tile picture occupies a
  // complete 32-tile hardware map row.
  for (; i < 20; i++) {
    for (j = 0; j < 30; j++)
      *tilemap++ = tile++;
    *tilemap++ = blankTile;
    *tilemap++ = blankTile;
  }

  Window_LoadGraphics();
  DebugText_Clear();
  if (prompt != 0) {
    u8 *buffer = AllocHeapSlotEwram(HEAP_SLOT_TILE_SCRATCH, 0x400);
    struct IntroSprite *sprite = work->area.prompt.sprites;

    Decompress_ByteLz(gTitle_PromptTiles, buffer);
    for (i = 0; i < PROMPT_TILES; i++) {
      s32 vramBlock =
          VramCache_Load(VramCache_FindFreeEntry(), 128, buffer + (i << 8) / 2);
      u32 *spriteWords = (u32 *)sprite;

      *spriteWords++ = 0;
      sprite++;
      *spriteWords++ = 0x40004000;
      *spriteWords = vramBlock;
    }
    FreeHeapSlot(HEAP_SLOT_TILE_SCRATCH);
  }
  ScreenFade_FromWhite(30);
  ScreenFade_Wait();
  REG_DISPCNT =
      DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON | DISPCNT_BG2_ON | DISPCNT_OBJ_ON;
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
      // The prompt pulses once a second: its blend weight over the picture
      // (written out: as BLEND_ALPHA(level, 16 - level) the bytes differ).
      level = gTitle_PromptBlendLevels[i % 60];
      REG_BLDCNT = BLDCNT_TGT1_OBJ | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_BG0 |
                   BLDCNT_TGT2_BG1 | BLDCNT_TGT2_BG2 | BLDCNT_TGT2_BG3 |
                   BLDCNT_TGT2_BD;
      REG_BLDALPHA = (16 - level) * 256 + level;
    }
    if ((gKeysPressed & (KEY_A | KEY_START)) != 0) {
      result = 1;
      break;
    }
    WaitFrames(1);
  }
  gCpuLoadDisplayEnabled = savedCpuLoadDisplay;
  FreeHeapSlot(HEAP_SLOT_TITLE_INTRO);
  REG_BLDCNT = 0;
  REG_BLDALPHA = 0;
  WaitFrames(1);
  return result;
}

/// Unused: palette entry retained in the far-call table.
UNUSED void Title_NoOpPalette(void) {}

/// Shows the Nintendo logo with its fanfare. With `mode` 0 it fades in and
/// holds two seconds; otherwise it waits a second, fades in, holds three
/// seconds and fades out, faster once skipped. Called by Title_Run. Returns -1
/// when A or Start skipped it.
s32 Title_ShowNintendoLogo(s32 mode) {
  s32 result;
  u8 *resourceData;
  u16 *tilemap;
  u32 column;
  u32 rowOrHoldFrame;
  u16 tileIndex;
  s32 blankTile;
  s32 resourceId;

  Sound_PlaySongFar(SE_110);
  gOamCopyEnabled = 1;
  resourceId = (s32)&ResourceId_NintendoLogo;
  blankTile = 0x1ff;
  Task_ResetAll();
  ScreenFade_ToBlack(1);
  DebugText_Clear();
  WaitFrames(1);
  REG_BG2CNT = BGCNT_PRIORITY(1) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(6);
  REG_DISPCNT = DISPCNT_OBJ_1D_MAP | DISPCNT_BG2_ON | DISPCNT_OBJ_ON;
  result = 0;
  gBgScroll[2].y = result;
  // The resource begins with 224 colours; its packed tiles follow them.
  resourceData = Resource_GetTableEntry(resourceId);
  Dma_Set(resourceData, (void *)BG_PLTT, DMA_COPY_WORDS(0x70), REG_DMA3);
  Decompress_Type01(resourceData + 448, Ram_MapGridCellBuffer);
  Dma_Set(Ram_MapGridCellBuffer, BG_CHAR_BLOCK(1), DMA_COPY_WORDS(0x2580),
          REG_DMA3);
  // Pad the 30-tile picture to the hardware map's 32-tile row width.
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
  Dma_Set(gBgScroll, REG_BG0HOFS, DMA_COPY_WORDS(4), REG_DMA3);
  Window_LoadGraphics();
  DebugText_Clear();
  REG_DISPCNT =
      DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON | DISPCNT_BG2_ON | DISPCNT_OBJ_ON;
  if (mode == 0) {
    ScreenFade_FromBlack(1);
    ScreenFade_Wait();
    for (rowOrHoldFrame = 0; rowOrHoldFrame < 120; rowOrHoldFrame++) {
      if (gKeysPressed & (KEY_A | KEY_START)) {
        result = -1;
        break;
      }
      WaitFrames(1);
    }
    return result;
  }
  // Otherwise a second of black, a second's fade in, three seconds held and
  // a second's fade out; a skip shortens each fade to eight frames.
  for (rowOrHoldFrame = 0; rowOrHoldFrame < 60; rowOrHoldFrame++) {
    if (gKeysPressed & (KEY_A | KEY_START)) {
      result = -1;
      break;
    }
    WaitFrames(1);
  }
  if (result != 0)
    ScreenFade_FromBlack(8);
  else
    ScreenFade_FromBlack(60);
  ScreenFade_Wait();
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
    ScreenFade_ToBlack(8);
  else
    ScreenFade_ToBlack(60);
  ScreenFade_Wait();
  return result;
}

/// Shows the Camelot logo for two seconds, cycling its four frames of tiles.
/// A or Start ends it early. Reached through the palette far-call table from
/// the title sequence. Returns 0.
s32 Title_ShowCamelotLogo(void) {
  s32 result;
  u32 workIndex;
  u8 *resourceData;
  s32 resourceId;

  gOamCopyEnabled = 1;
  result = 0;
  resourceId = (s32)&ResourceId_CamelotLogo;
  Task_ResetAll();
  ScreenFade_ToBlack(1);
  DebugText_Clear();
  WaitFrames(1);
  REG_BG2CNT = BGCNT_PRIORITY(1) | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS |
               BGCNT_SCREEN_BASE(6);
  REG_DISPCNT = DISPCNT_OBJ_1D_MAP | DISPCNT_BG2_ON | DISPCNT_OBJ_ON;
  gBgScroll[2].y = result;
  /* FAKEMATCH(temporary): workIndex carries the decode destination before it
   * counts; as resourceData set before the decode, 13 of 119 instructions
   * differ in tbs-en. */
  workIndex = (u32)Ram_MapGridCellBuffer;
  Decompress_Type01(Resource_GetTableEntry(resourceId), (u8 *)workIndex);
  // The decoded resource: 224 colours, the map, the still tiles, then four
  // animation frames of 0x400 bytes each.
  resourceData = (u8 *)workIndex;
  Dma_Set(resourceData, (void *)BG_PLTT, DMA_COPY_WORDS(0x1c0 / 4), REG_DMA3);
  resourceData += 0x1c0;
  Dma_Set(resourceData, BG_SCREEN_BLOCK(6), DMA_COPY_WORDS(0x800 / 4),
          REG_DMA3);
  resourceData += 0x800;
  Dma_Set(resourceData, BG_CHAR_BLOCK(1), DMA_COPY_WORDS(0x4000 / 4), REG_DMA3);
  resourceData += 0x4000;
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
  // in VRAM while the four animation frames cycle.
  for (workIndex = 0; workIndex < 120; workIndex++) {
    Dma_Set(((gFrameCount >> 3) & 3) * 0x400 + resourceData,
            BG_CHAR_BLOCK(1) + 0x100, DMA_COPY_WORDS(0x340 / 4), REG_DMA3);
    if (gKeysPressed & (KEY_A | KEY_START))
      break;
    WaitFrames(1);
  }
  return result;
}

/// Unused: color entry retained in the far-call table.
UNUSED void Title_NoOpColor(void) {}

// The palette fade (struct TitlePaletteFade) keeps the saved palettes, the
// colour components shown now, the target components and each one's step.

/// Computes each of the 1536 colour components' change per frame from the
/// current components to the target (nothing when `frames` is not
/// positive). Called by TitlePaletteFade_Start.
void TitlePaletteFade_CalculateSteps(s16 *sourceComponents,
                                     s16 *targetComponents, s16 *componentSteps,
                                     s32 frames) {
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
  u16 *paletteBank;
  s32 blueMask;

  if (work->duration == 0)
    return;
  if ((s8)++work->step < work->duration) {
    currentComponents = work->current;
    for (remaining = 0; remaining <= 0x5ff; remaining++)
      *currentComponents++ += *componentSteps++;
  } else {
    Dma_Set(work->target, work->current, DMA_COPY_WORDS(0x300), REG_DMA3);
    work->duration = 0;
  }
  // The queued DMA keeps a pointer until VBlank. Build into the spare page
  // so packing this frame cannot overwrite the last frame's pending colours.
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

    IO_WRITE_QUEUE_PUSH((u32)BG_PLTT, (u32)backgroundColors,
                        DMA_COPY_WORDS(0x80));
  }
  IO_WRITE_QUEUE_PUSH((u32)OBJ_PLTT, (u32)(paletteBank + 256),
                      DMA_COPY_WORDS(0x80));
}

/// Tints 512 source colours into destination components (three
/// per colour, each in the high bits of a halfword). `mode` picks the
/// tint: a colour below PALETTE_TINT_FROM_SOURCE fills every colour
/// with it; PALETTE_TINT_FROM_SOURCE takes that colour from source[0];
/// PALETTE_TINT_NONE to PALETTE_TINT_WARM2 are the whole-palette
/// filters; the TINT, BLEND and SPLIT flags carry
/// a colour in their low bits; anything else is the address of components to
/// copy. `selectedPalettes` is an enum PaletteSelection.
/// Called by TitlePaletteFade_Initialize and the palette fade's setters.
void TitlePaletteFade_BuildColors(u32 mode, u16 *source, u16 *destination,
                                  s32 selectedPalettes) {
  u32 colorIndex;
  u32 colorCount = 512;
  s32 red;
  s32 blue;
  u32 colorWork;
  s32 green;

  if (mode == PALETTE_TINT_FROM_SOURCE)
    mode = *source;
  if (selectedPalettes == PALETTES_BG) {
    colorCount = 256;
  } else if (selectedPalettes == PALETTES_OBJECTS) {
    destination += 768;
    source += 256;
    colorCount = 256;
  }

  if (mode < PALETTE_TINT_FROM_SOURCE) {
    *destination++ = mode & 0x7c00;
    *destination++ = (mode & 0x03e0) << 5;
    *destination++ = (mode & 0x001f) << 10;
    Dma_Set(destination - 3, destination,
            DMA_ENABLE << 16 | (((colorCount - 1) * 6) >> 1), REG_DMA3);
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
        red = colorWork & 31;
        colorWork = Iwram_SignedDivide(
            red + ((colorWork >> 5) & 31) + ((colorWork >> 10) & 31), 10);
        red = colorWork * 4 + 5;
        blue = colorWork * 3 + 5;
        /* FAKEMATCH(dead): two XORs with red that cancel fold late, keeping
         * blue's add first; as green = blue, 3 of 803 instructions differ in
         * tbs-en. */
        green = blue ^ red;
        green ^= red;
        if (red < 8)
          red = 8;
        if (blue < 8) {
          green = 8;
          /* FAKEMATCH(dead): two XORs that cancel, green being 8, keep blue's
           * separate lower test; as no pair, 8 of 803 instructions differ in
           * tbs-en. */
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
        *destination++ = gPalette_ComponentCurveLight[blue];
        *destination++ = gPalette_ComponentCurveLight[green];
        *destination++ = gPalette_ComponentCurveLight[red];
      }
      break;
    case PALETTE_TINT_WARM:
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
        red = TitlePaletteFade_ClampComponent(red);
        green = TitlePaletteFade_ClampComponent(green);
        blue = TitlePaletteFade_ClampComponent(blue);
        SET_VECTOR(destination, gPalette_ComponentCurveContrast[blue],
                   gPalette_ComponentCurveLight[green],
                   gPalette_ComponentCurve[red]);
        destination += 3;
      }
      break;
    case PALETTE_TINT_MUTED:
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
        red = TitlePaletteFade_ClampComponent(red);
        green = TitlePaletteFade_ClampComponent(green);
        blue = TitlePaletteFade_ClampComponent(blue);
        *destination++ = gPalette_ComponentCurve[blue];
        *destination++ = gPalette_ComponentCurve[green];
        *destination++ = gPalette_ComponentCurve[red];
      }
      break;
    case PALETTE_TINT_PALE:
      for (colorIndex = 0; colorIndex < colorCount; colorIndex++) {
        s32 mean;

        colorWork = *source++;
        red = colorWork & 31;
        green = (colorWork >> 5) & 31;
        blue = (colorWork >> 10) & 31;
        mean = TitlePaletteFade_ClampComponent((red + green + blue) / 3);
        red = (red >> 1) + mean;
        green = (green >> 1) + mean;
        blue = (blue >> 1) + mean;
        red = TitlePaletteFade_ClampComponent(red);
        green = TitlePaletteFade_ClampComponent(green);
        blue = TitlePaletteFade_ClampComponent(blue);
        SET_VECTOR(destination, gPalette_ComponentCurveContrast[blue],
                   gPalette_ComponentCurveContrast[green],
                   gPalette_ComponentCurveContrast[red]);
        destination += 3;
      }
      break;
    case PALETTE_TINT_RED:
      for (colorIndex = 0; colorIndex < colorCount; colorIndex++) {
        colorWork = *source++;
        red = colorWork & 31;
        green = (colorWork >> 5) & 31;
        blue = (colorWork >> 10) & 31;
        red += ((green >> 3) + (blue >> 3));
        red = TitlePaletteFade_ClampComponent(red);
        green -= green / 3;
        blue -= blue / 3;
        SET_VECTOR(destination, gPalette_ComponentCurve[blue],
                   gPalette_ComponentCurve[green],
                   gPalette_ComponentCurveLight[red]);
        destination += 3;
      }
      break;
    case PALETTE_TINT_WARM2:
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
        red = TitlePaletteFade_ClampComponent(red);
        green = TitlePaletteFade_ClampComponent(green);
        blue = TitlePaletteFade_ClampComponent(blue);
        SET_VECTOR(destination, gPalette_ComponentCurveContrast[blue],
                   gPalette_ComponentCurveLight[green],
                   gPalette_ComponentCurve[red]);
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
      red = TitlePaletteFade_ClampComponent((u32)red >> 16);
      green = TitlePaletteFade_ClampComponent((u32)green >> 16);
      blue = TitlePaletteFade_ClampComponent((u32)blue >> 16);
      *destination++ = gPalette_ComponentCurve[blue];
      *destination++ = gPalette_ComponentCurve[green];
      *destination++ = gPalette_ComponentCurve[red];
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
      mode += 0x600;
    Dma_Set((const void *)mode, destination,
            DMA_COPY_WORDS((colorCount * 6) >> 2), REG_DMA3);
  }
}

/// Allocates the palette fade, saves both hardware palettes in it as the
/// starting colours and adds the task TitlePaletteFade_Update. Called by
/// Title_ShowIntro.
void TitlePaletteFade_Initialize(void) {
  struct TitlePaletteFade *buffer;
  s32 callbackPriority;

  buffer = AllocHeapSlotEwram(HEAP_SLOT_PALETTE_FADE, sizeof(*buffer));
  Dma_Fill32(0, buffer, sizeof(*buffer));
  Dma_Set((void *)BG_PLTT, buffer, DMA_COPY_WORDS(0x80), REG_DMA3);
  Dma_Set((void *)OBJ_PLTT, &buffer->palette[256], DMA_COPY_WORDS(0x80),
          REG_DMA3);
  TitlePaletteFade_BuildColors(PALETTE_TINT_NONE, buffer->palette,
                               buffer->target, PALETTES_ALL);
  callbackPriority = TASK_PRIORITY_MAIN_LOOP;
  Task_Add(TitlePaletteFade_Update, callbackPriority);
}

/// Unused: removes the palette fade's task and frees its buffers. Nothing in ☀️
/// calls it.
UNUSED void TitlePaletteFade_Release(void) {
  Task_Remove(&TitlePaletteFade_Update);
  FreeHeapSlot(HEAP_SLOT_PALETTE_FADE);
}

/// Unused: sets the fade target by tinting the saved background and/or
/// sprite palettes with the requested tint. Nothing in ☀️ calls it.
UNUSED void TitlePaletteFade_SetTarget(s32 mode, s32 selectedPalettes) {
  struct TitlePaletteFade *fade = (struct TitlePaletteFade *)gPaletteFadeWork;

  if (fade != NULL)
    TitlePaletteFade_BuildColors(mode, fade->palette, fade->target,
                                 selectedPalettes);
}

/// Sets the colours shown now by tinting the saved palettes. Called by
/// Title_ShowIntro.
void TitlePaletteFade_SetCurrent(s32 mode, s32 selectedPalettes) {
  struct TitlePaletteFade *fade = (struct TitlePaletteFade *)gPaletteFadeWork;

  if (fade != NULL)
    TitlePaletteFade_BuildColors(mode, fade->palette, fade->current,
                                 selectedPalettes);
}

/// Unused: replaces the first saved colour, the backdrop. Nothing in ☀️ calls
/// it.
UNUSED void TitlePaletteFade_SetBackdrop(s32 color) {
  struct TitlePaletteFade *fade = (struct TitlePaletteFade *)gPaletteFadeWork;

  if (fade != NULL)
    fade->palette[0] = color;
}

/// Starts fading the shown colours to the target over the requested frames.
/// Called by Title_ShowIntro for the opening fade from black.
void TitlePaletteFade_Start(s32 frames) {
  struct TitlePaletteFade *fade = (struct TitlePaletteFade *)gPaletteFadeWork;

  if (fade != NULL) {
    fade->duration = frames;
    fade->step = 0;
    TitlePaletteFade_CalculateSteps((s16 *)fade->current, (s16 *)fade->target,
                                    (s16 *)fade->delta, frames);
  }
}

/// Clamps a five-bit colour channel to 0..31. Called by
/// TitlePaletteFade_BuildColors.
s32 TitlePaletteFade_ClampComponent(s32 value) {
  if (value > 31)
    return 31;
  if (value < 0)
    value = 0;
  return value;
}

/// Caps a colour component at 0x7c00, the largest a five-bit channel shifted
/// into the high bits reaches. There is no lower bound. Called by
/// TitlePaletteFade_BuildColors.
s32 TitlePaletteFade_ClampShiftedComponent(s32 value) {
  if (value > 0x7c00)
    value = 0x7c00;
  return value;
}

// The response curves of TitlePaletteFade_BuildColors's filters. Like the
// declarations in palbuf.h they are not const: as const the code differs.

/// Maps a channel (0..31) to a component (0..31 << 10)
// clang-format off
u16 gPalette_ComponentCurve[32] = {
    0, 524, 1071, 1643, 2240, 2863, 3513, 4191,
    4898, 5635, 6403, 7202, 8035, 8901, 9803, 10740,
    11715, 12729, 13783, 14877, 16014, 17195, 18422, 19694,
    21015, 22386, 23808, 25283, 26812, 28397, 30041, 31744
};

/// A brighter curve
u16 gPalette_ComponentCurveLight[32] = {
    0, 613, 1247, 1902, 2580, 3281, 4005, 4753,
    5526, 6324, 7148, 7999, 8876, 9782, 10716, 11680,
    12674, 13699, 14755, 15843, 16965, 18121, 19312, 20538,
    21801, 23101, 24440, 25818, 27237, 28697, 30199, 31744
};

/// A high-contrast curve (25 and above are full)
u16 gPalette_ComponentCurveContrast[32] = {
    0, 360, 759, 1201, 1689, 2226, 2817, 3466,
    4178, 4957, 5808, 6738, 7752, 8856, 10058, 11365,
    12784, 14325, 15996, 17806, 19767, 21888, 24183, 26662,
    29340, 31744, 31744, 31744, 31744, 31744, 31744, 31744
};
// clang-format on
