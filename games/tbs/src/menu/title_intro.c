/// @file
/// The title intro, the view rising up a peak towards the sun and its lens
/// flare until the flash into the title picture and its "Press Start" prompt;
/// the Nintendo and Camelot logos; and the palette fade the intro uses.

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

/// The most OAM entries the lens flare takes in a VBlank; the entry after
/// them is always hidden, and the sun is built there.
#define FLARE_OBJECT_COUNT 12

/// The title intro's state, in HEAP_SLOT_TITLE_INTRO.
struct IntroWork {
  s32 backRows;  ///< rows of the clouds copied in as they scroll
  s32 frontRows; ///< rows of the peak copied in
  s32 frame;
  s32 frameCount;
  s32 state; ///< an enum IntroState
  s32 rise;  ///< how far the lens flare's lower end has risen
  union {
    struct OamWords objects[INTRO_OBJECT_COUNT];
    struct {
      /// The lens flare's entries and the sun's, still the intro's.
      struct OamWords flareObjects[FLARE_OBJECT_COUNT + 1];
      struct IntroSprite sprites[8];
    } prompt;
  } area;
};

/// The intro's two pictures share character block 0: the clouds (BG1) in
/// 16-colour tiles, the peak (BG2) in 256-colour tiles after them.
#define INTRO_BACK_TILE(n) (BG_CHAR_BLOCK(0) + (n) * TILE_SIZE_4BPP)
#define INTRO_FRONT_TILE(n) (BG_CHAR_BLOCK(0) + (n) * 2 * TILE_SIZE_4BPP)

/// The screen blocks of the two pictures' maps
#define INTRO_FRONT_SCREEN 30
#define INTRO_BACK_SCREEN 31
/// Both pictures start scrolled this far down and scroll up into place.
#define INTRO_SCROLL_START 96

/// The intro's pictures unpack one after the other: the clouds, 33 rows of
/// tiles in 16 colours, then the peak, 28 rows in 256 colours, each row a
/// screen wide.
#define CLOUD_ROWS 33
#define CLOUD_ROW_BYTES (DISPLAY_TILE_WIDTH * TILE_SIZE_4BPP)
#define PEAK_ROW_BYTES (DISPLAY_TILE_WIDTH * TILE_SIZE_8BPP)
#define CLOUD_ROW(row) (Ram_MapGridCellBuffer + (row) * CLOUD_ROW_BYTES)
#define PEAK_ROW(row) (CLOUD_ROW(CLOUD_ROWS) + (row) * PEAK_ROW_BYTES)

/// Each picture keeps 21 rows of tiles in VRAM, the screen's 20 and the one
/// coming into view, and its map's 32 rows wrap round them: map row
/// INTRO_FIRST_MAP_ROW, the one above the screen at the start, shows the first.
#define INTRO_VRAM_ROWS (DISPLAY_TILE_HEIGHT + 1)
#define INTRO_FIRST_MAP_ROW (INTRO_SCROLL_START / 8 - 1)
/// The peak's first tile, after the clouds' in character block 0; it is blank.
#define PEAK_FIRST_TILE (INTRO_VRAM_ROWS * CLOUD_ROW_BYTES / TILE_SIZE_8BPP)
#define CLOUD_VRAM_ROW(row) INTRO_BACK_TILE((row) * DISPLAY_TILE_WIDTH)
#define PEAK_VRAM_ROW(row)                                                     \
  INTRO_FRONT_TILE(PEAK_FIRST_TILE + (row) * DISPLAY_TILE_WIDTH)
/// The peak's top stands this many rows down the map, below the clouds' top.
#define PEAK_MAP_ROW 6
/// The peak's rows above those in VRAM at the start, copied in as they come
/// into view; its top row is blank and never copied.
#define PEAK_ROWS_ABOVE 4

/// The x of each of the lens flare's six pieces
const u8 gTitle_FlareColumns[] = {96, 136, 140, 160, 168, 184};

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

/// Brings `v` into a sprite's y, 0 to 255, by whole turns
#define WRAP_OBJ_Y(v)                                                          \
  while ((v) > OBJ_Y_RANGE - 1)                                                \
    (v) -= OBJ_Y_RANGE;                                                        \
  while ((v) < 0)                                                              \
  (v) += OBJ_Y_RANGE

/// The lens flare's pieces lie on a line from lineLeftY at this x to lineRightY
/// at FLARE_LINE_RIGHT.
#define FLARE_LINE_LEFT 104
#define FLARE_LINE_RIGHT 184

/// The lens flare's pictures in the sprite tiles, after the sun's: an orange
/// disc whole, and quarters, mirrored into the rest, of a rainbow ring, a
/// yellow-green ring, a green disc with a cyan heart and a white-hot disc.
#define SUN_TILE 0
#define FLARE_ORANGE_DISC_TILE 128
#define FLARE_RAINBOW_RING_TILE 160
#define FLARE_GREEN_RING_TILE 192
#define FLARE_GREEN_DISC_TILE 224
#define FLARE_WHITE_DISC_TILE 232
/// A disc is 32 pixels across, a ring 64.
#define FLARE_DISC_RADIUS 16
#define FLARE_RING_RADIUS 32

/// Sets x and y to the top left of a piece of `radius` centred on column
/// `column`, on the flare's line.
#define PLACE_ON_FLARE_LINE(column, radius)                                    \
  x = gTitle_FlareColumns[column] - (radius);                                  \
  y = (lineRightY - lineLeftY) * ((x + (radius)) - FLARE_LINE_LEFT) /          \
          (FLARE_LINE_RIGHT - FLARE_LINE_LEFT) +                               \
      lineLeftY - (radius);                                                    \
  WRAP_OBJ_Y(y)

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

/// Where the sun stands over the clouds, which it scrolls with.
#define SUN_X 92
#define SUN_Y 32

/// A VBlank task of the title intro, added by Title_ShowIntro: places the lens
/// flare's pieces along a line down from the sun, which swings up as the intro
/// goes on, alternating two sets of three every other VBlank so all six show,
/// hides the rest, adds the sun and copies the list to OAM.
void Title_UpdateIntroSprites(void) {
  struct IntroWork *work;
  u32 spriteCount;
  s32 lineLeftY;
  s32 lineRightY;
  s32 x;
  s32 y;
  s32 sunY;

  work = HEAP_SLOT_BLOCK(HEAP_SLOT_TITLE_INTRO);
  spriteCount = 0;
  if (gDebugPaused == 0) {
    if ((++work->frameCount & 3) == 0)
      work->rise++;
  }
  lineLeftY = 48 - gBgScroll[1].y;
  lineRightY = 144 - work->rise;
  if (work->frame < INTRO_FLASH_FRAME) {
    if ((work->frameCount & 1) == 0) {
      PLACE_ON_FLARE_LINE(0, FLARE_DISC_RADIUS);
      MIRRORED_QUAD(spriteCount, FLARE_DISC_RADIUS, OBJ_SIZE_16X16,
                    FLARE_WHITE_DISC_TILE);
      spriteCount += 4;
      PLACE_ON_FLARE_LINE(2, FLARE_DISC_RADIUS);
      INTRO_OBJ(spriteCount).attr01 = OBJ_X(x + 4) | y | OBJ_SIZE_32X32 |
                                      OBJ_MODE(OBJ_MODE_SEMI_TRANSPARENT) |
                                      OBJ_256_COLORS;
      INTRO_OBJ(spriteCount).attr2 = FLARE_ORANGE_DISC_TILE;
      spriteCount++;
      PLACE_ON_FLARE_LINE(4, FLARE_RING_RADIUS);
      MIRRORED_QUAD(spriteCount, FLARE_RING_RADIUS, OBJ_SIZE_32X32,
                    FLARE_GREEN_RING_TILE);
      spriteCount += 4;
    } else {
      PLACE_ON_FLARE_LINE(1, FLARE_DISC_RADIUS);
      MIRRORED_QUAD(spriteCount, FLARE_DISC_RADIUS, OBJ_SIZE_16X16,
                    FLARE_WHITE_DISC_TILE);
      spriteCount += 4;
      PLACE_ON_FLARE_LINE(3, FLARE_DISC_RADIUS);
      MIRRORED_QUAD(spriteCount, FLARE_DISC_RADIUS, OBJ_SIZE_16X16,
                    FLARE_GREEN_DISC_TILE);
      spriteCount += 4;
      PLACE_ON_FLARE_LINE(5, FLARE_RING_RADIUS);
      MIRRORED_QUAD(spriteCount, FLARE_RING_RADIUS, OBJ_SIZE_32X32,
                    FLARE_RAINBOW_RING_TILE);
      spriteCount += 4;
    }
  }
  for (; spriteCount < INTRO_OBJECT_COUNT; spriteCount++)
    INTRO_OBJ(spriteCount).attr01 =
        DISPLAY_HEIGHT | OBJ_SIZE_16X16 | OBJ_256_COLORS;
  REG_BLDCNT = BLDCNT_TGT1_OBJ | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
  REG_BLDALPHA = BLEND_ALPHA(14, 14);
  Dma_Set(&INTRO_OBJ(0), OAM, DMA_COPY_WORDS((spriteCount * 8) >> 2), REG_DMA3);
  // The sun goes in the OAM entry after the list.
  sunY = SUN_Y - gBgScroll[1].y;
  WRAP_OBJ_Y(sunY);
  INTRO_OBJ(FLARE_OBJECT_COUNT).attr01 =
      sunY | OBJ_X(SUN_X) | OBJ_SIZE_64X64 | OBJ_256_COLORS;
  INTRO_OBJ(FLARE_OBJECT_COUNT).attr2 = OBJ_PRIORITY(2) | SUN_TILE;
  Dma_Set(&INTRO_OBJ(FLARE_OBJECT_COUNT), (u8 *)OAM + spriteCount * 8,
          DMA_COPY_WORDS(2), REG_DMA3);
  Dma_Set(&INTRO_OBJ(0), OAM, DMA_COPY_WORDS(8), REG_DMA3);
}

/// Loads the title intro's two scrolling pictures, the clouds and the peak, and
/// the sun's and lens flare's sprite tiles, builds the pictures' maps and sets
/// the blending and windows.
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
  Dma_Set(CLOUD_ROW(INTRO_FIRST_MAP_ROW), CLOUD_VRAM_ROW(0),
          DMA_COPY_HALFWORDS(INTRO_VRAM_ROWS * CLOUD_ROW_BYTES / 2), REG_DMA3);
  Dma_Set(PEAK_ROW(INTRO_FIRST_MAP_ROW - PEAK_MAP_ROW), PEAK_VRAM_ROW(0),
          DMA_COPY_HALFWORDS(INTRO_VRAM_ROWS * PEAK_ROW_BYTES / 2), REG_DMA3);

  {
    s32 offscreenTile = OFFSCREEN_TILE;

    // Map rows from INTRO_FIRST_MAP_ROW show the VRAM rows from the first;
    // the rows above it show the last ones.
    tilemap = (u16 *)BG_SCREEN_BLOCK(INTRO_FRONT_SCREEN);
    tileIndex = PEAK_FIRST_TILE +
                (INTRO_VRAM_ROWS - INTRO_FIRST_MAP_ROW) * DISPLAY_TILE_WIDTH;
    for (row = 0; row < INTRO_FIRST_MAP_ROW; row++) {
      for (column = 0; column < DISPLAY_TILE_WIDTH; column++)
        *tilemap++ = tileIndex++;
      *tilemap++ = offscreenTile;
      *tilemap++ = offscreenTile;
    }
    tileIndex = PEAK_FIRST_TILE;
    for (row = INTRO_FIRST_MAP_ROW; row <= 31; row++) {
      for (column = 0; column < DISPLAY_TILE_WIDTH; column++)
        *tilemap++ = tileIndex++;
      *tilemap++ = offscreenTile;
      *tilemap++ = offscreenTile;
    }
    tilemap = (u16 *)BG_SCREEN_BLOCK(INTRO_BACK_SCREEN);
    tileIndex = (INTRO_VRAM_ROWS - INTRO_FIRST_MAP_ROW) * DISPLAY_TILE_WIDTH;
    for (row = 0; row < INTRO_FIRST_MAP_ROW; row++) {
      for (column = 0; column < DISPLAY_TILE_WIDTH; column++)
        *tilemap++ = tileIndex++;
      *tilemap++ = offscreenTile;
      *tilemap++ = offscreenTile;
    }
    tileIndex = 0;
    for (row = INTRO_FIRST_MAP_ROW; row <= 31; row++) {
      for (column = 0; column < DISPLAY_TILE_WIDTH; column++)
        *tilemap++ = tileIndex++;
      *tilemap++ = offscreenTile;
      *tilemap++ = offscreenTile;
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
#define PROMPT_ROW_HEIGHT 8
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

/// The title picture's first tile: character block 1's, as BG2 numbers its
/// 256-colour tiles from block 0.
#define TITLE_PICTURE_TILE (BG_CHAR_SIZE / TILE_SIZE_8BPP)

/// Plays the title intro: the view rises up the peak towards the sun, the
/// clouds slower behind it, while the sun's lens flare swings up, until just
/// before INTRO_FLASH_FRAME (A or Start skips ahead); then the screen flashes
/// white into the title picture, the Golden Sun logo over the peak. That holds
/// a minute with the "Press Start" prompt pulsing when `prompt` is set, five
/// seconds without, and returns 1 when A or Start ends it, else 0.
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
  s32 offscreenTile = OFFSCREEN_TILE;

  result = 0;
  savedCpuLoadDisplay = gCpuLoadDisplayEnabled;
  // BUG: the block is sized for the prompt's sprites, but the intro's OAM list
  // runs on past its end.
#ifdef BUGFIX
  work = HeapSlot_Alloc(HEAP_SLOT_TITLE_INTRO, sizeof(struct IntroWork));
#else
  work =
      HeapSlot_Alloc(HEAP_SLOT_TITLE_INTRO, offsetof(struct IntroWork, area) +
                                                sizeof(work->area.prompt));
#endif
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

    // A or Start skips to the approach, which still plays out to the flash.
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
      // The clouds (BG1) scroll a pixel every third frame and the peak (BG2)
      // every other one; as a new row of tiles comes into view, its row of
      // the picture is copied over the VRAM row leaving at the bottom.
      frameCount = work->frameCount;
      if (frameCount % 3 == 0) {
        s32 y;

        gBgScroll[1].y--;
        y = gBgScroll[1].y;
        if ((y & 7) == 0) {
          Dma_Set(CLOUD_ROW(INTRO_FIRST_MAP_ROW - 1) -
                      work->backRows * CLOUD_ROW_BYTES,
                  CLOUD_VRAM_ROW(INTRO_VRAM_ROWS - 1) -
                      work->backRows * CLOUD_ROW_BYTES,
                  DMA_COPY_HALFWORDS(CLOUD_ROW_BYTES / 2), REG_DMA3);
          work->backRows++;
          frameCount = work->frameCount;
        }
      }
      if ((frameCount & 1) == 0) {
        s32 y;

        gBgScroll[2].y--;
        y = gBgScroll[2].y;
        if ((y & 7) == 0) {
          if (work->frontRows * 8 <= (PEAK_ROWS_ABOVE - 1) * 8) {
            Dma_Set(PEAK_ROW(INTRO_FIRST_MAP_ROW - PEAK_MAP_ROW - 1) -
                        work->frontRows * PEAK_ROW_BYTES,
                    PEAK_VRAM_ROW(INTRO_VRAM_ROWS - 1) -
                        work->frontRows * PEAK_ROW_BYTES,
                    DMA_COPY_HALFWORDS(PEAK_ROW_BYTES / 2), REG_DMA3);
          } else {
            // Above the peak: each new row is cleared, and the map's bottom
            // five rows, whose VRAM rows held the peak's top and which the
            // scroll brings round above the screen, show its blank first tile.
            clearValue = 0;
            Dma_Set(&clearValue,
                    INTRO_FRONT_TILE(PEAK_FIRST_TILE) +
                        (DISPLAY_HEIGHT - work->frontRows * 8) %
                            DISPLAY_HEIGHT * DISPLAY_WIDTH,
                    DMA_FILL_HALFWORDS(PEAK_ROW_BYTES / 2), REG_DMA3);
            tilemap = (u16 *)BG_SCREEN_BLOCK(INTRO_FRONT_SCREEN) +
                      27 * BG_SCREEN_WIDTH;
            for (i = 0; i < 5; i++) {
              for (j = 0; j < DISPLAY_TILE_WIDTH; j++)
                *tilemap++ = PEAK_FIRST_TILE;
              *tilemap++ = PEAK_FIRST_TILE;
              *tilemap++ = PEAK_FIRST_TILE;
            }
          }
          work->frontRows++;
        }
      }
      if (work->frame == INTRO_APPROACH_FRAME)
        work->state = INTRO_STATE_APPROACH;
    } else if (nextFrame == INTRO_FLASH_FRAME + 1) {
      // Unused: the loop ends at the flash, before these frames.
      work->state = INTRO_STATE_AFTER_FLASH;
    } else if (nextFrame == INTRO_FLASH_FRAME + 9) {
      work->state = INTRO_STATE_SCROLLING;
    } else if (nextFrame == INTRO_FLASH_FRAME) {
      // The flash: behind the white the intro's sprites stop, OAM goes back
      // to the frame's own sprite list and the title picture is put up.
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
              DMA_COPY_HALFWORDS(SCREEN_TILE_BYTES / 2), REG_DMA3);
      tilemap = (u16 *)BG_SCREEN_BLOCK(6);
      tile = TITLE_PICTURE_TILE;
      i = 0;
      break;
    }
    wait(1);
  }
  // The title picture's map, each row's two columns past the screen's edge,
  // which never show.
  for (; i < DISPLAY_TILE_HEIGHT; i++) {
    for (j = 0; j < DISPLAY_TILE_WIDTH; j++)
      *tilemap++ = tile++;
    *tilemap++ = offscreenTile;
    *tilemap++ = offscreenTile;
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

      for (row = 0, y = PROMPT_Y; row < PROMPT_ROWS;
           y += PROMPT_ROW_HEIGHT, row++) {
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
      REG_BLDALPHA = (BLEND_WEIGHT_MAX - level) * BLEND_ALPHA(0, 1) + level;
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

/// Unused: an empty entry the far-call table keeps.
UNUSED void Title_NoOpPalette(void) {}

#define LOGO_PALETTE_BYTES (LOGO_PALETTE_COLORS * 2)

/// The Nintendo logo's first tile: character block 1's, as BG2 numbers its
/// 256-colour tiles from block 0.
#define NINTENDO_LOGO_TILE (BG_CHAR_SIZE / TILE_SIZE_8BPP)

/// Shows the Nintendo logo with its jingle. With mode 0 it fades in and holds
/// two seconds; otherwise it waits a second, fades in, holds three seconds and
/// fades out, faster once skipped. Returns -1 when A or Start skipped it.
s32 Title_ShowNintendoLogo(s32 mode) {
  s32 result;
  u8 *resourceData;
  u16 *tilemap;
  u32 column;
  u32 i;
  u16 tileIndex;
  s32 offscreenTile = OFFSCREEN_TILE;
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
  // The packed tiles follow the colours.
  resourceData = Resource_GetTableEntry(resourceId);
  Dma_Set(resourceData, (void *)BG_PLTT, DMA_COPY_WORDS(LOGO_PALETTE_BYTES / 4),
          REG_DMA3);
  Decompress_TaggedLz(resourceData + LOGO_PALETTE_BYTES, Ram_MapGridCellBuffer);
  Dma_Set(Ram_MapGridCellBuffer, BG_CHAR_BLOCK(1),
          DMA_COPY_WORDS(SCREEN_TILE_BYTES / 4), REG_DMA3);
  // Pad the 30-tile picture to the hardware map's 32-tile row width.
  tileIndex = NINTENDO_LOGO_TILE;
  tilemap = (u16 *)BG_SCREEN_BLOCK(6);
  for (i = 0; i < DISPLAY_TILE_HEIGHT; i++) {
    for (column = 0; column < DISPLAY_TILE_WIDTH; column++)
      *tilemap++ = tileIndex++;
    *tilemap++ = offscreenTile;
    *tilemap++ = offscreenTile;
  }
  for (i = 0; i < 4; i++) {
    gBgScroll[i].y = 0;
    gBgScroll[i].x = 0;
  }
  Dma_Set(gBgScroll, REG_BG0HOFS, DMA_COPY_WORDS(4), REG_DMA3);
  Window_LoadGraphics();
  DebugText_Clear();
  REG_DISPCNT =
      DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON | DISPCNT_BG2_ON | DISPCNT_OBJ_ON;
  if (mode == 0) {
    ScreenFade_FromBlack(1);
    ScreenFade_Wait();
    for (i = 0; i < 2 * FRAMES_PER_SECOND; i++) {
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
  for (i = 0; i < FRAMES_PER_SECOND; i++) {
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
    for (i = 0; i < 3 * FRAMES_PER_SECOND; i++) {
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

/// The Camelot logo's flag: four pictures of 16 tiles after the logo's own,
/// each shown 8 frames, whose first 13 tiles are copied over the logo's from
/// its fifth tile.
#define CAMELOT_FLAG_TILE 4
#define CAMELOT_FLAG_PICTURE_BYTES (16 * TILE_SIZE_8BPP)
#define CAMELOT_FLAG_BYTES (13 * TILE_SIZE_8BPP)

/// Shows the Camelot logo for two seconds, its flag waving. A or Start ends it
/// early.
s32 Title_ShowCamelotLogo(void) {
  u32 i;
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
  // The unpacked resource: the colours, the map, the still tiles, then the
  // flag's four pictures.
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
  for (i = 0; i < 4; i++) {
    gBgScroll[i].y = 0;
    gBgScroll[i].x = 0;
  }
  Dma_Set(gBgScroll, REG_BG0HOFS, DMA_COPY_WORDS(4), REG_DMA3);
  Window_LoadGraphics();
  DebugText_Clear();
  ScreenFade_FromWhite(1);
  ScreenFade_Wait();
  REG_DISPCNT =
      DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON | DISPCNT_BG2_ON | DISPCNT_OBJ_ON;
  for (i = 0; i < 2 * FRAMES_PER_SECOND; i++) {
    Dma_Set(((gFrameCount >> 3) & 3) * CAMELOT_FLAG_PICTURE_BYTES +
                resourceData,
            BG_CHAR_BLOCK(1) + CAMELOT_FLAG_TILE * TILE_SIZE_8BPP,
            DMA_COPY_WORDS(CAMELOT_FLAG_BYTES / 4), REG_DMA3);
    if (gKeysPressed & (KEY_A | KEY_START))
      break;
    wait(1);
  }
  return result;
}

/// Unused: an empty entry the far-call table keeps.
UNUSED void Title_NoOpColor(void) {}

// The palette fade (struct TitlePaletteFade) keeps the saved palettes, the
// colour components shown now, the target components and each one's step.

// The fade holds each channel as a component, moved up to blue's place (bits
// 14 to 10) with a fraction in the ten bits below.

/// Computes each of the 1536 colour components' change per frame from the
/// current components to the target (nothing when `frames` is not positive).
void TitlePaletteFade_CalculateSteps(s16 *sourceComponents,
                                     s16 *targetComponents, s16 *componentSteps,
                                     s32 frames) {
  s32 i;
  s32 sourceValue;
  s32 targetValue;

  if (frames > 0) {
    for (i = 0; i < TITLE_PALETTE_FADE_CHANNELS; i++) {
      sourceValue = *sourceComponents;
      targetValue = *targetComponents;
      *componentSteps = Iwram_SignedDivide(targetValue - sourceValue, frames);
      sourceComponents++;
      targetComponents++;
      componentSteps++;
    }
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
    s32 blueMask = BLUE_BITS;

    remaining = PLTT_COLORS;
    currentComponents = work->current;
    for (; remaining != 0; remaining--) {
      *packedColors++ =
          (currentComponents[0] & blueMask) |
          (((s16)currentComponents[1] >> (BLUE_SHIFT - GREEN_SHIFT)) &
           GREEN_BITS) |
          (((s16)currentComponents[2] >> BLUE_SHIFT) & RED_BITS);
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
    *destination++ = mode & BLUE_BITS;
    *destination++ = (mode & GREEN_BITS) << (BLUE_SHIFT - GREEN_SHIFT);
    *destination++ = (mode & RED_BITS) << BLUE_SHIFT;
    Dma_Set(destination - 3, destination,
            DMA_COPY_HALFWORDS(((colorCount - 1) * 6) >> 1), REG_DMA3);
  } else if (mode < PALETTE_TINT_FLAGS) {
    switch (mode) {
    case PALETTE_TINT_GRAYSCALE:
      for (colorIndex = 0; colorIndex < colorCount; colorIndex++) {
        colorWork = *source++;
        red = ((colorWork << BRIGHTNESS_RED_SHIFT) &
               (RED_BITS << BRIGHTNESS_RED_SHIFT));
        green = ((colorWork << BRIGHTNESS_GREEN_SHIFT) &
                 (GREEN_BITS << BRIGHTNESS_GREEN_SHIFT));
        blue = (colorWork & BLUE_BITS);
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
        colorWork =
            Iwram_SignedDivide(red + RGB_GREEN(colorWork) + RGB_BLUE(colorWork),
                               SEPIA_SHADE_DIVISOR);
        red = colorWork * 4 + 5;
        blue = colorWork * 3 + 5;
        /* FAKEMATCH(dead): two XORs with red that cancel. */
        green = blue ^ red;
        green ^= red;
        if (red < SEPIA_LEVEL_MIN)
          red = SEPIA_LEVEL_MIN;
        if (blue < SEPIA_LEVEL_MIN) {
          green = SEPIA_LEVEL_MIN;
          /* FAKEMATCH(dead): two XORs that cancel, green being the minimum. */
          blue ^= green;
          blue ^= SEPIA_LEVEL_MIN;
        }
        if (blue < SEPIA_LEVEL_MIN)
          blue = SEPIA_LEVEL_MIN;
        if (red > SEPIA_LEVEL_MAX)
          red = SEPIA_LEVEL_MAX;
        if (green > SEPIA_LEVEL_MAX)
          green = SEPIA_LEVEL_MAX;
        if (blue > SEPIA_LEVEL_MAX)
          blue = SEPIA_LEVEL_MAX;
        *destination++ = gTitleIntro_ChannelCurveLight[blue];
        *destination++ = gTitleIntro_ChannelCurveLight[green];
        *destination++ = gTitleIntro_ChannelCurveLight[red];
      }
      break;
    case PALETTE_TINT_WARM: // here PALETTE_TINT_WARM_DARK's recipe
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
        if (red < MUTED_RED_MIN)
          red = MUTED_RED_MIN;
        if (green < MUTED_GREEN_MIN)
          green = MUTED_GREEN_MIN;
        if (blue < MUTED_BLUE_MIN)
          blue = MUTED_BLUE_MIN;
        if (red > MUTED_RED_MAX)
          red = MUTED_RED_MAX;
        if (green > MUTED_GREEN_MAX)
          green = MUTED_GREEN_MAX;
        if (blue > MUTED_BLUE_MAX)
          blue = MUTED_BLUE_MAX;
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
        destination[colorIndex * 3 + 0] = colorWork & BLUE_BITS;
        destination[colorIndex * 3 + 1] = (colorWork & GREEN_BITS)
                                          << (BLUE_SHIFT - GREEN_SHIFT);
        destination[colorIndex * 3 + 2] = (colorWork & RED_BITS) << BLUE_SHIFT;
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
      red = ((colorWork << BRIGHTNESS_RED_SHIFT) &
             (RED_BITS << BRIGHTNESS_RED_SHIFT));
      green = ((colorWork << BRIGHTNESS_GREEN_SHIFT) &
               (GREEN_BITS << BRIGHTNESS_GREEN_SHIFT));
      blue = (colorWork & BLUE_BITS);
      colorWork = Iwram_SignedDivide(red + green + blue, CUSTOM_TINT_DIVISOR);
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
          (red + RGB_GREEN(colorWork) + RGB_BLUE(colorWork)) << 4,
          tintRed + tintGreen + tintBlue);
      red = Iwram_MulFX(INTtoFIXED((colorWork * tintRed) >> 4),
                        (s32)INTtoFIXED(tintRed) >> 4);
      green = Iwram_MulFX(INTtoFIXED((colorWork * tintGreen) >> 4),
                          (s32)INTtoFIXED(tintGreen) >> 4);
      blue = Iwram_MulFX(INTtoFIXED((colorWork * tintBlue) >> 4),
                         (s32)INTtoFIXED(tintBlue) >> 4);
      red = TitlePaletteFade_ClampComponent(FIXEDtoINT((u32)red));
      green = TitlePaletteFade_ClampComponent(FIXEDtoINT((u32)green));
      blue = TitlePaletteFade_ClampComponent(FIXEDtoINT((u32)blue));
      *destination++ = gTitleIntro_ChannelCurve[blue];
      *destination++ = gTitleIntro_ChannelCurve[green];
      *destination++ = gTitleIntro_ChannelCurve[red];
    }
  } else if (mode & PALETTE_TINT_SPLIT) {
    for (colorIndex = 0; colorIndex < colorCount; colorIndex++) {
      colorWork = *source++;
      destination[colorIndex * 3 + 0] = colorWork & BLUE_BITS;
      destination[colorIndex * 3 + 1] = (colorWork & GREEN_BITS)
                                        << (BLUE_SHIFT - GREEN_SHIFT);
      destination[colorIndex * 3 + 2] = (colorWork & RED_BITS) << BLUE_SHIFT;
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

/// Caps a colour component at a channel's brightest level moved up to the
/// components' place. There is no lower bound.
s32 TitlePaletteFade_ClampShiftedComponent(s32 value) {
  if (value > BLUE_BITS)
    value = BLUE_BITS;
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
