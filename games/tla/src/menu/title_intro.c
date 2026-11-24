/// @file
/// The title intro, the Nintendo and Camelot logos, the title screen and the
/// palette fade they use. The intro opens on drifting clouds, shows the cards
/// `Camelot presents` and `Golden Sun` `Book Two` between them, drops through
/// the clouds into white, and scrolls down the lighthouse in its valley while a
/// gull flies off towards it and `The Lost Age` comes into focus.
///
/// The gull's frames and path are logo.c; the prompt's levels and the fade's
/// channel curves are title_intro_tables.c.

#include "types.h"
#include "sounds.h"
#include "graphics/graphics.h"
#include "system/system.h"
#include "edition.h"
#include "version.h"
#include "macros.h"
#include "system/dma.h"
#include "system/io_reg.h"
#include "system/io_write.h"
#include "window/window.h"
#include "system/io_write_queue.h"
#include "system/iwram_call.h"
#include "system/heap_release.h"
#include "graphics/resource.h"
#include "system/ram_buffer.h"
#include "system/frame.h"
#include "system/input.h"
#include "system/task.h"
#include "system/scene.h"
#include "system/bg_scroll.h"
#include "graphics/palette_buffers.h"
#include "field/actor.h"
#include "resource_ids.h"
#include "scene/title.h"

/// The screen blocks of the intro's maps: the near clouds on BG2 and the far
/// ones on BG1, which the lighthouse later takes; the cards' text on BG3 and
/// the line blended in over it on BG0.
#define INTRO_FRONT_SCREEN 30
#define INTRO_BACK_SCREEN 31
#define INTRO_CARD_SCREEN 29
#define INTRO_CARD_BLEND_SCREEN 28

/// One step of the clouds' scroll (Title_RunScrollStep, TITLE_STEP).
struct TitleScrollStep {
  s32 frames; ///< how long the step lasts
  /// Pixels a frame: BG1's y and x, then BG2's y and x.
  s32 speed[4];
  /// For each speed, the frame count bits that must be clear for it to move;
  /// 0 moves it every frame.
  u32 mask[4];
};

/// The scroll down the lighthouse picture, or back up it: where it is and where
/// it stops, the pixels a step and the frames each step waits, and the
/// picture's rows of tiles copied into VRAM as they come into view.
struct TitleScroll {
  s16 position;    ///< pixels down the picture
  s16 limit;       ///< the position this scroll stops at
  s16 step;        ///< pixels a step, negative going back up
  u8 *source;      ///< the next row of the picture to copy
  u8 *destination; ///< where in VRAM that row goes
  u16 *cursor;     ///< the map entry of the row's first tile
  u16 tile;        ///< that tile's number
  s16 scroll;      ///< pixels scrolled since the last row was copied
  /// Going down, the VRAM row the next copy fills; going back up, the rows
  /// left to pass before the next copy.
  u16 rows;
  u32 frames; ///< frames each step waits
};

/// An OAM entry's attributes 0 to 2, as the title's logo pieces are written.
struct OamEntry {
  u32 y : 8;
  u32 affine : 2; ///< 1 drawn through its matrix, 3 also at double size
  u32 mode : 2;   ///< an enum ObjMode
  u32 mosaic : 1;
  u32 fullColor : 1;
  u32 shape : 2;
  u32 x : 9;
  u32 matrix : 5;
  u32 size : 2;
  u16 tile : 10;
  u16 priority : 2;
  u16 palette : 4;
  u16 unused;
};

/// OamEntry.affine for a sprite drawn through its matrix in a box twice its
/// size, so that scaling it up never clips it.
#define OAM_AFFINE_DOUBLE_SIZE 3
/// OamEntry.shape and .size for a 64 by 64 sprite.
#define OAM_SHAPE_SQUARE 0
#define OAM_SIZE_64 3

/// TitleLogoState.phase: what TitleIntro_UpdateLogo draws.
enum TitleLogoPhase {
  /// A gull flies off towards the lighthouse along gTitleIntro_GullPath.
  TITLE_LOGO_GULL = 0,
  /// `The Lost Age` closes in, its three pieces scaling down and fading up.
  TITLE_LOGO_ZOOM = 1
};

/// The title intro's state while the lighthouse shows (HEAP_SLOT_TITLE_INTRO):
/// the gull or the logo, and the countdowns to the fade to white and back that
/// the scroll down starts.
struct TitleLogoState {
  s16 phase; ///< enum TitleLogoPhase
  /// Where the gull's frame starts in gTitleIntro_GullFrames; in the zoom, the
  /// logo's blend weight.
  s16 frame;
  /// Frames left of the gull's frame; in the zoom, frames since it began.
  s16 timer;
  /// Where the gull's step starts in gTitleIntro_GullPath; in the zoom, the
  /// logo's scale.
  s16 step;
  s16 delay;     ///< frames left of the gull's step
  s16 toWhite;   ///< scroll steps until the fade to white; 0 when none
  s16 fromWhite; ///< scroll steps until the fade back; 0 when none
};

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

/// The title screen's state (HEAP_SLOT_TITLE_INTRO): the prompt's sprites.
struct TitleScreenState {
  u8 unused[16]; ///< no code names it
  struct IntroSprite sprites[8];
};

void TitlePaletteFade_SetCurrent(s32 tint, s32 selectedPalettes);
/// Where each of `The Lost Age`'s pictures is unpacked before it is copied into
/// sprite tiles.
extern u8 gTitleIntro_LogoTileBuffer[];
extern const u8 gTitle_PromptBlendLevels[];
extern u16 gTitlePaletteFade_ChannelCurve[];
extern u16 gTitlePaletteFade_ChannelCurveLight[];
extern u16 gTitlePaletteFade_ChannelCurveContrast[];
s32 TitlePaletteFade_ClampChannel(s32 value);
s32 TitlePaletteFade_ClampShiftedChannel(s32 value);

/// The intro's tiles, in 16 colours: the far clouds in character block 0, the
/// near clouds in block 1, each 256 by 128 pixels, and the cards' text in block
/// 2. The text is `Camelot presents`, three rows in these editions and two in
/// the others, then `Golden Sun` on three and `Book Two` on two.
#define CLOUD_ROWS 16
#if LANGUAGE == LANGUAGE_GERMAN || LANGUAGE == LANGUAGE_SPANISH ||             \
    LANGUAGE == LANGUAGE_ITALIAN
#define PRESENTS_ROWS 3
#else
#define PRESENTS_ROWS 2
#endif
#define GOLDEN_SUN_ROWS 3
#define BOOK_TWO_ROWS 2
#define CARD_TEXT_ROWS (PRESENTS_ROWS + GOLDEN_SUN_ROWS + BOOK_TWO_ROWS)
#define GOLDEN_SUN_TILE (PRESENTS_ROWS * BG_SCREEN_WIDTH)
#define BOOK_TWO_TILE (GOLDEN_SUN_TILE + GOLDEN_SUN_ROWS * BG_SCREEN_WIDTH)
#define INTRO_TILE_BYTES                                                       \
  ((2 * CLOUD_ROWS + CARD_TEXT_ROWS) * BG_SCREEN_WIDTH * TILE_SIZE_4BPP)

/// Blanks the display, loads the intro's clouds and cards: their colours into
/// the BG palette with colour 0 black, and their tiles, unpacked through the
/// map cell buffer, from BG character block 0.
void TitleIntro_LoadPictureTiles(void) {
  u8 *source;
  u8 *cells;

  REG_DISPCNT = 0;
  source = Resource_GetTableEntry(RESOURCE_INTRO_CLOUDS);
  Dma_Set(source, (void *)BG_PLTT, DMA_COPY_WORDS(BG_PLTT_SIZE / 4), REG_DMA3);
  BG_PLTT[0] = 0;
  cells = Ram_MapCellBuffer;
  Decompress_ByteLz(source + BG_PLTT_SIZE, cells);
  Dma_Set(cells, (void *)BG_CHAR_BLOCK(0), DMA_COPY_WORDS(INTRO_TILE_BYTES / 4),
          REG_DMA3);
}

/// Waits frames frames, stirring the random seed each frame; returns 1 at once
/// when a key is pressed, 0 when the wait ends.
s32 Title_WaitOrPress(u32 frames) {
  u32 i;

  for (i = 0; i < frames; i++) {
    wait(1);
    Random16();
    if (gInput.pressed != 0)
      return 1;
  }
  return 0;
}

/// Runs one step of the clouds' scroll, moving BG1 and BG2 each frame by the
/// step's speeds on the frames its masks allow; returns 0 when a key cuts it
/// short, 1 when it ends.
s32 Title_RunScrollStep(struct TitleScrollStep *step) {
  s32 i;

  for (i = 0; i < step->frames; i++) {
    if (step->mask[0] == 0 || (gFrameCount & step->mask[0]) == 0)
      gBgScroll[1].y += step->speed[0];
    if (step->mask[1] == 0 || (gFrameCount & step->mask[1]) == 0)
      gBgScroll[1].x += step->speed[1];
    if (step->mask[2] == 0 || (gFrameCount & step->mask[2]) == 0)
      gBgScroll[2].y += step->speed[2];
    if (step->mask[3] == 0 || (gFrameCount & step->mask[3]) == 0)
      gBgScroll[2].x += step->speed[3];
    if (Title_WaitOrPress(1))
      return 0;
  }
  return 1;
}

/// One step of the clouds' scroll: for `duration` frames, BG1 and BG2 move by
/// their dy and dx each frame whose count has none of its mask's bits.
#define TITLE_STEP(duration, bg1Dy, bg1Dx, bg2Dy, bg2Dx, bg1DyMask, bg1DxMask, \
                   bg2DyMask, bg2DxMask)                                       \
  (step.frames = (duration), step.speed[0] = (bg1Dy), step.speed[1] = (bg1Dx), \
   step.speed[2] = (bg2Dy), step.speed[3] = (bg2Dx),                           \
   step.mask[0] = (bg1DyMask), step.mask[1] = (bg1DxMask),                     \
   step.mask[2] = (bg2DyMask), step.mask[3] = (bg2DxMask),                     \
   Title_RunScrollStep(&step))

/// The near clouds' and the far clouds' blend weights over each other; the
/// plunge raises both from the near clouds' to full.
#define CLOUD_FRONT_WEIGHT 11
#define CLOUD_BACK_WEIGHT 10

/// The intro's three passes over the clouds.
enum CloudsPass {
  CLOUD_BEFORE_CARDS = 0,
  CLOUD_BETWEEN_CARDS = 1,
  /// After the last card, ending in a plunge through them into white.
  CLOUD_AFTER_CARDS = 2
};

/// Shows the two layers of clouds and runs one pass's scroll steps over them
/// (an enum CloudsPass), fading in from dark red, at once on the first pass;
/// returns 0 when a key cuts it short.
s32 TitleIntro_ScrollClouds(s32 pass) {
  struct TitleScrollStep step;
  u16 *map;
  u16 tile;
  s32 i;
  s32 j;

  REG_DISPCNT = 0;
  map = (u16 *)BG_SCREEN_BLOCK(INTRO_BACK_SCREEN);
  tile = 0;
  for (j = 0; j < CLOUD_ROWS; j++) {
    for (i = 0; i < BG_SCREEN_WIDTH; i++) {
      *map++ = tile++;
    }
  }
  tile = 0;
  for (; j < BG_SCREEN_WIDTH; j++) {
    for (i = 0; i < BG_SCREEN_WIDTH; i++) {
      *map++ = tile++;
    }
  }
  map = (u16 *)BG_SCREEN_BLOCK(INTRO_FRONT_SCREEN);
  tile = 0;
  for (j = 0; j < CLOUD_ROWS; j++) {
    for (i = 0; i < BG_SCREEN_WIDTH; i++) {
      *map++ = tile++ | BG_TILE_PALETTE(1);
    }
  }
  tile = 0;
  for (; j < BG_SCREEN_WIDTH; j++) {
    for (i = 0; i < BG_SCREEN_WIDTH; i++) {
      *map++ = tile++ | BG_TILE_PALETTE(1);
    }
  }
  REG_BG1CNT = BGCNT_PRIORITY(2) | BGCNT_SCREEN_BASE(INTRO_BACK_SCREEN);
  REG_BG2CNT = BGCNT_PRIORITY(1) | BGCNT_CHAR_BASE(1) |
               BGCNT_SCREEN_BASE(INTRO_FRONT_SCREEN);
  REG_BLDCNT = BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_BG1;
  REG_BLDALPHA = BLEND_ALPHA(CLOUD_FRONT_WEIGHT, CLOUD_BACK_WEIGHT);
  for (i = 0; i < 4; i++) {
    gBgScroll[i].y = 0;
    gBgScroll[i].x = 0;
  }
  if (pass == CLOUD_BEFORE_CARDS) {
    TitlePaletteFade_Initialize();
  }
  TitlePaletteFade_SetCurrent(RGB(2, 0, 0), PALETTES_ALL);
  TitlePaletteFade_SetTarget(PALETTE_TINT_NONE, PALETTES_ALL);
  REG_DISPCNT = DISPCNT_BG1_ON | DISPCNT_BG2_ON;
  if (pass == CLOUD_BEFORE_CARDS) {
    TitlePaletteFade_Start(1);
  } else {
    TitlePaletteFade_Start(10);
  }

  if (pass == CLOUD_BEFORE_CARDS) {
    if (!TITLE_STEP(30, 2, 0, -3, 0, 0, 0, 0, 0))
      return 0;
    if (!TITLE_STEP(10, 2, 0, -2, 0, 0, 0, 0, 0))
      return 0;
    if (!TITLE_STEP(10, 1, 0, -2, 0, 0, 0, 0, 0))
      return 0;
    if (!TITLE_STEP(10, 1, 0, -1, 0, 0, 0, 0, 0))
      return 0;
    if (!TITLE_STEP(10, 1, 0, -1, 0, 1, 0, 0, 0))
      return 0;
    if (!TITLE_STEP(10, 0, 0, -1, 0, 0, 0, 1, 0))
      return 0;
    if (!TITLE_STEP(20, -1, 0, 0, 0, 1, 0, 0, 0))
      return 0;
    if (!TITLE_STEP(20, -1, 0, 1, 0, 3, 0, 1, 0))
      return 0;
    if (!TITLE_STEP(20, -1, 0, 1, 0, 3, 0, 1, 0))
      return 0;
    TitlePaletteFade_SetTarget(RGB(2, 0, 0), PALETTES_ALL);
    TitlePaletteFade_Start(60);
    if (!TITLE_STEP(60, -1, 0, 1, 0, 3, 0, 1, 0))
      return 0;
  } else if (pass == CLOUD_BETWEEN_CARDS) {
    if (!TITLE_STEP(40, 1, 1, -1, -1, 1, 0, 1, 0))
      return 0;
    if (!TITLE_STEP(20, 1, 1, -1, -1, 3, 0, 1, 0))
      return 0;
    if (!TITLE_STEP(10, 1, 1, -1, -1, 7, 3, 1, 0))
      return 0;
    if (!TITLE_STEP(10, 0, 1, 0, -1, 0, 3, 0, 0))
      return 0;
    if (!TITLE_STEP(30, -1, 0, 0, -1, 15, 1, 0, 0))
      return 0;
    if (!TITLE_STEP(20, -1, -1, 0, -1, 15, 1, 0, 0))
      return 0;
    if (!TITLE_STEP(60, -1, -1, 0, -2, 15, 1, 0, 0))
      return 0;
    TitlePaletteFade_SetTarget(RGB(2, 0, 0), PALETTES_ALL);
    TitlePaletteFade_Start(60);
    if (!TITLE_STEP(60, -1, -1, 0, -2, 15, 1, 0, 0))
      return 0;
  } else {
    if (!TITLE_STEP(60, 0, -1, -1, 1, 0, 0, 0, 0))
      return 0;
    if (!TITLE_STEP(16, 8, -8, 8, 8, 0, 0, 0, 0))
      return 0;
    // The plunge: both layers speed up and brighten, two frames a weight.
    j = 2;
    for (i = CLOUD_FRONT_WEIGHT; i < BLEND_WEIGHT_MAX; i++) {
      REG_BLDALPHA = BLEND_ALPHA(i, i);
      gBgScroll[1].y += j + 16;
      gBgScroll[2].y += j + 8;
      j++;
      if (Title_WaitOrPress(1))
        return 0;
      gBgScroll[1].y += j + 16;
      gBgScroll[2].y += j + 8;
      j++;
      if (Title_WaitOrPress(1))
        return 0;
    }
    TitlePaletteFade_SetTarget(RGB_WHITE, PALETTES_ALL);
    TitlePaletteFade_Start(1);
    if (Title_WaitOrPress(2)) {
      ScreenFade_Wait();
      return 0;
    }
  }
  return 1;
}

/// The screen rows each card's text stands on.
#define PRESENTS_TOP_ROW 9
#define GOLDEN_SUN_TOP_ROW 7
#define BOOK_TWO_TOP_ROW (GOLDEN_SUN_TOP_ROW + GOLDEN_SUN_ROWS)
/// A tile every edition's card text leaves blank, the left end of its third
/// row: the `Golden Sun` card fills the screen around its text with it and with
/// the blank tile below it.
#define CARD_BLANK_TILE (2 * BG_SCREEN_WIDTH)

/// Fades in the card `Camelot presents` on BG3, holds it 150 frames and fades
/// it out to dark red; returns 0 when a key cuts it short.
s32 TitleIntro_ShowPresentsCard(void) {
  u16 *map;
  u16 tile;
  s32 i;
  s32 row;

  REG_DISPCNT = 0;
  map = (u16 *)BG_SCREEN_BLOCK(INTRO_CARD_SCREEN);
  tile = 0;
  for (row = 0; row < DISPLAY_TILE_HEIGHT; row++) {
    if (row >= PRESENTS_TOP_ROW && row < PRESENTS_TOP_ROW + PRESENTS_ROWS) {
      for (i = 0; i < BG_SCREEN_WIDTH; i++) {
        *map++ = tile++ | BG_TILE_PALETTE(2);
      }
    } else {
      for (i = 0; i < BG_SCREEN_WIDTH; i++)
        *map++ = BG_TILE_PALETTE(2);
    }
  }
  REG_BG3CNT = BGCNT_PRIORITY(2) | BGCNT_CHAR_BASE(2) |
               BGCNT_SCREEN_BASE(INTRO_CARD_SCREEN);
  REG_BLDCNT = 0;
  REG_BLDALPHA = 0;
  for (i = 0; i < 4; i++)
    gBgScroll[i].x = gBgScroll[i].y = 0;

  TitlePaletteFade_SetCurrent(RGB(0, 0, 0), PALETTES_ALL);
  TitlePaletteFade_SetTarget(PALETTE_TINT_NONE, PALETTES_ALL);
  REG_DISPCNT = DISPCNT_BG3_ON;
  TitlePaletteFade_Start(40);
  if (Title_WaitOrPress(150))
    return 0;
  TitlePaletteFade_SetTarget(RGB(2, 0, 0), PALETTES_ALL);
  TitlePaletteFade_Start(40);
  if (Title_WaitOrPress(60))
    return 0;
  return 1;
}

/// Fades in the card `Golden Sun` on BG3, blends `Book Two` in below it from
/// BG0 over 16 steps of 3 frames, holds the card 150 frames and fades it out to
/// dark red; returns 0 when a key cuts it short.
s32 TitleIntro_ShowBookTwoCard(void) {
  u16 *map;
  u16 tile;
  s32 i;
  s32 row;

  REG_DISPCNT = 0;
  map = (u16 *)BG_SCREEN_BLOCK(INTRO_CARD_SCREEN);
  tile = GOLDEN_SUN_TILE;
  for (row = 0; row < DISPLAY_TILE_HEIGHT; row++) {
    if (row >= GOLDEN_SUN_TOP_ROW && row < BOOK_TWO_TOP_ROW) {
      for (i = 0; i < BG_SCREEN_WIDTH; i++) {
        *map++ = tile++ | BG_TILE_PALETTE(3);
      }
    } else if (row >= BOOK_TWO_TOP_ROW &&
               row < BOOK_TWO_TOP_ROW + BOOK_TWO_ROWS) {
      for (i = 0; i < BG_SCREEN_WIDTH; i++)
        *map++ = BG_TILE_PALETTE(3) | (CARD_BLANK_TILE + BG_SCREEN_WIDTH);
    } else {
      for (i = 0; i < BG_SCREEN_WIDTH; i++)
        *map++ = BG_TILE_PALETTE(3) | CARD_BLANK_TILE;
    }
  }
  map = (u16 *)BG_SCREEN_BLOCK(INTRO_CARD_BLEND_SCREEN);
  tile = BOOK_TWO_TILE;
  for (row = 0; row < DISPLAY_TILE_HEIGHT; row++) {
    if (row >= BOOK_TWO_TOP_ROW && row < BOOK_TWO_TOP_ROW + BOOK_TWO_ROWS) {
      for (i = 0; i < BG_SCREEN_WIDTH; i++) {
        *map++ = tile++ | BG_TILE_PALETTE(3);
      }
    } else {
      for (i = 0; i < BG_SCREEN_WIDTH; i++)
        *map++ = BG_TILE_PALETTE(3) | CARD_BLANK_TILE;
    }
  }
  REG_BG3CNT = BGCNT_PRIORITY(2) | BGCNT_CHAR_BASE(2) |
               BGCNT_SCREEN_BASE(INTRO_CARD_SCREEN);
  REG_BG0CNT = BGCNT_PRIORITY(1) | BGCNT_CHAR_BASE(2) |
               BGCNT_SCREEN_BASE(INTRO_CARD_BLEND_SCREEN);
  REG_BLDCNT = BLDCNT_TGT1_BG0 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_BG3;
  REG_BLDALPHA = BLEND_ALPHA(0, BLEND_WEIGHT_MAX);
  for (i = 0; i < 4; i++)
    gBgScroll[i].x = gBgScroll[i].y = 0;

  TitlePaletteFade_SetCurrent(RGB(0, 0, 0), PALETTES_ALL);
  TitlePaletteFade_SetTarget(PALETTE_TINT_NONE, PALETTES_ALL);
  REG_DISPCNT = DISPCNT_BG0_ON | DISPCNT_BG3_ON;
  TitlePaletteFade_Start(40);
  if (Title_WaitOrPress(60))
    return 0;
  for (i = 1; i <= BLEND_WEIGHT_MAX; i++) {
    REG_BLDALPHA = BLEND_ALPHA(i, BLEND_WEIGHT_MAX);
    if (Title_WaitOrPress(3))
      return 0;
  }
  if (Title_WaitOrPress(150))
    return 0;
  TitlePaletteFade_SetTarget(RGB(2, 0, 0), PALETTES_ALL);
  TitlePaletteFade_Start(40);
  if (Title_WaitOrPress(60))
    return 0;
  return 1;
}

/// The lighthouse picture, 240 by 384 pixels in 256 colours: its first screen
/// is loaded before it scrolls and the rest of its rows of tiles are copied in
/// as they come into view, wrapping round the map's 32 rows.
#define LIGHTHOUSE_HEIGHT 384
#define LIGHTHOUSE_ROW_BYTES (DISPLAY_TILE_WIDTH * TILE_SIZE_8BPP)
/// The scroll that shows the picture's foot.
#define LIGHTHOUSE_BOTTOM (LIGHTHOUSE_HEIGHT - DISPLAY_HEIGHT)
/// A screen of tiles in 256 colours: the lighthouse picture's first screen, and
/// the Nintendo logo and the title screen.
#define SCREEN_TILE_BYTES (DISPLAY_TILE_HEIGHT * LIGHTHOUSE_ROW_BYTES)
/// The map entry of the two columns past the screen's right edge, which never
/// show.
#define OFFSCREEN_TILE 511

/// Scrolls the lighthouse picture from one position to another: each step moves
/// it by the scroll's step and waits its frames, copies in the next row of the
/// picture as one comes into view, and starts the fade to white or back when
/// its countdown reaches 1. Returns 1 when a key cuts the scroll short.
s32 TitleIntro_ScrollLighthouse(struct TitleScroll *s) {
  struct TitleLogoState *timers = Ram_HeapSlots->titleIntro;
  s32 i;
  s32 j;

  if (s->step > 0) {
    for (i = s->position; i < s->limit; i += s->step) {
      if (Title_WaitOrPress(s->frames))
        goto fail;
      if (s->scroll > 0) {
        Dma_Set(s->source, s->destination,
                DMA_COPY_WORDS(LIGHTHOUSE_ROW_BYTES / 4), REG_DMA3);
        s->source += LIGHTHOUSE_ROW_BYTES;
        s->destination += LIGHTHOUSE_ROW_BYTES;
        for (j = 0; j < DISPLAY_TILE_WIDTH; j++)
          *s->cursor++ = s->tile++;
        *s->cursor++ = OFFSCREEN_TILE;
        *s->cursor++ = OFFSCREEN_TILE;
        if (++s->rows == BG_SCREEN_WIDTH) {
          s->destination = (u8 *)BG_CHAR_BLOCK(0);
          s->rows = 0;
          s->tile = 0;
          s->cursor = (u16 *)BG_SCREEN_BLOCK(INTRO_BACK_SCREEN);
        }
        s->scroll -= 8;
      }
      s->scroll += s->step;
      gBgScroll[1].y += s->step;
      if (timers->toWhite != 0 && --timers->toWhite == 1) {
        TitlePaletteFade_SetTarget(RGB_WHITE, PALETTES_ALL);
        TitlePaletteFade_Start(10);
      }
      if (timers->fromWhite != 0 && --timers->fromWhite == 1) {
        TitlePaletteFade_SetTarget(PALETTE_TINT_NONE, PALETTES_ALL);
        TitlePaletteFade_Start(10);
      }
    }
    goto done;
  fail:
    return 1;
  } else {
    for (i = s->position; i > s->limit; i += s->step) {
      gBgScroll[1].y += s->step;
      s->scroll -= s->step;
      if (s->scroll > 8) {
        s->rows--;
        if (s->rows == 0) {
          s->rows = 1;
          Dma_Set(s->source, s->destination,
                  DMA_COPY_WORDS(LIGHTHOUSE_ROW_BYTES / 4), REG_DMA3);
          s->source -= LIGHTHOUSE_ROW_BYTES;
          s->destination -= LIGHTHOUSE_ROW_BYTES;
          for (j = 0; j < DISPLAY_TILE_WIDTH; j++)
            *s->cursor++ = s->tile++;
          s->cursor -= DISPLAY_TILE_WIDTH;
          s->tile -= DISPLAY_TILE_WIDTH;
        }
        s->scroll -= 8;
      }
      if (Title_WaitOrPress(s->frames))
        goto fail;
    }
  }
done:
  s->position = i;
  return 0;
}

/// A piece of `The Lost Age`: 64 by 64 pixels in 256 colours, numbered in the
/// sprites' tiles of 32 bytes.
#define LOGO_PIECE_BYTES (64 * 64)
#define LOGO_PIECE_TILES (LOGO_PIECE_BYTES / TILE_SIZE_4BPP)

/// Writes one piece of `The Lost Age` straight into OAM, in the entry its tile
/// picks: affine at double size, so that its top left is at (x, y) unscaled,
/// semi-transparent and in 256 colours.
void Title_PlaceLogoPiece(s32 tile, s32 x, u32 y) {
  struct OamEntry oam;

  Dma_Fill32(0, &oam, sizeof(oam));
  oam.shape = OAM_SHAPE_SQUARE;
  oam.size = OAM_SIZE_64;
  oam.fullColor = 1;
  oam.mode = OBJ_MODE_SEMI_TRANSPARENT;
  oam.affine = OAM_AFFINE_DOUBLE_SIZE;
  oam.tile = tile;
  oam.x = x - 32;
  oam.y = y - 32;
  Dma_Set(&oam, (void *)((u32)OAM + ((u32)tile >> 4)), DMA_COPY_WORDS(2),
          REG_DMA3);
}

/// Where the left, middle and right pieces of `The Lost Age` come to rest, and
/// how far down.
#if LANGUAGE == LANGUAGE_GERMAN || LANGUAGE == LANGUAGE_SPANISH ||             \
    LANGUAGE == LANGUAGE_ITALIAN
#define LOGO_LEFT_X 24
#define LOGO_MIDDLE_X 88
#define LOGO_RIGHT_X 152
#elif LANGUAGE == LANGUAGE_FRENCH
#define LOGO_LEFT_X 19
#define LOGO_MIDDLE_X 83
#define LOGO_RIGHT_X 147
#else
#define LOGO_LEFT_X 30
#define LOGO_MIDDLE_X 83
#define LOGO_RIGHT_X 140
#endif
#define LOGO_Y 50

/// The logo pieces' affine scale, in 8.8: they start drawn 1.6 times their size
/// and shrink to it.
#define LOGO_MATRIX_START Q_8_8(0.625)
#define LOGO_MATRIX_REST Q_8_8(1)

/// The fields of a gull frame in gTitleIntro_GullFrames: its first tile, its
/// rows of GULL_COLUMNS 8x8 sprites, the frames it shows for and its offset.
enum GullFrameField {
  GULL_FRAME_TILE,
  GULL_FRAME_ROWS,
  GULL_FRAME_DURATION,
  GULL_FRAME_OFFSET_X,
  GULL_FRAME_OFFSET_Y,
  GULL_FRAME_FIELDS
};
/// The fields of a step of gTitleIntro_GullPath: where the gull is and the
/// frames it stays; an x of -1 ends the path.
enum GullStepField {
  GULL_STEP_X,
  GULL_STEP_Y,
  GULL_STEP_DURATION,
  GULL_STEP_FIELDS
};
#define GULL_COLUMNS 8

extern s16 gTitleIntro_GullFrames[];
extern s16 gTitleIntro_GullPath[];

/// Unpacks a picture of `The Lost Age`, its three pieces, into sprite tiles.
#define LOAD_LOGO_TILES(id)                                                    \
  {                                                                            \
    u8 *buffer = gTitleIntro_LogoTileBuffer;                                   \
    Decompress_ByteLz(Resource_GetTableEntry(id), buffer);                     \
    Dma_Set(buffer, (void *)OBJ_VRAM0,                                         \
            DMA_COPY_WORDS(3 * LOGO_PIECE_BYTES / 4), REG_DMA3);               \
  }

/// The task that draws over the lighthouse each frame: the gull flying off
/// towards it along its path, then `The Lost Age` closing in, its pictures
/// coming into focus as it fades up.
void TitleIntro_UpdateLogo(void) {
  struct TitleLogoState *logo;
  struct OamEntry sprite;
  volatile u32 fill;
  s32 x;
  s32 tile;
  s32 left;
  s32 rows;
  s32 top;
  s32 offsetX;
  s32 offsetY;
  s32 row;
  s32 column;
  s32 y;
  s32 pieceX;
  volatile u16 *matrix;

  logo = Ram_HeapSlots->titleIntro;
  Dma_Fill32Cell(fill, 0, (void *)OAM, OAM_SIZE);
  if (logo->phase == TITLE_LOGO_GULL) {
    if (logo->timer == 0) {
      if (gTitleIntro_GullFrames[logo->frame + GULL_FRAME_FIELDS] != -1)
        logo->frame += GULL_FRAME_FIELDS;
      logo->timer = gTitleIntro_GullFrames[logo->frame + GULL_FRAME_DURATION];
    } else {
      logo->timer--;
    }
    if (logo->delay == 0) {
      logo->step += GULL_STEP_FIELDS;
      logo->delay = gTitleIntro_GullPath[logo->step + GULL_STEP_DURATION];
    } else {
      logo->delay--;
    }
    x = gTitleIntro_GullPath[logo->step + GULL_STEP_X];
    if (x == -1) {
      Dma_Set(Resource_GetTableEntry(RESOURCE_SUBTITLE_PALETTE),
              (void *)OBJ_PLTT, DMA_COPY_WORDS(OBJ_PLTT_SIZE / 4), REG_DMA3);
      LOAD_LOGO_TILES(RESOURCE_SUBTITLE_A);
      logo->phase = TITLE_LOGO_ZOOM;
      logo->frame = 1;
      logo->timer = 0;
      logo->step = LOGO_MATRIX_START;
      logo->delay = 0;
      REG_BLDCNT = BLDCNT_TGT1_OBJ | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_BG1;
      REG_BLDALPHA = BLEND_ALPHA(1, BLEND_WEIGHT_MAX);
      return;
    }
    tile = gTitleIntro_GullFrames[logo->frame + GULL_FRAME_TILE];
    rows = gTitleIntro_GullFrames[logo->frame + GULL_FRAME_ROWS];
    offsetX = gTitleIntro_GullFrames[logo->frame + GULL_FRAME_OFFSET_X];
    offsetY = gTitleIntro_GullFrames[logo->frame + GULL_FRAME_OFFSET_Y];
    left = x + offsetX - 28;
    top = gTitleIntro_GullPath[logo->step + GULL_STEP_Y] + offsetY;
    for (row = 0; row < rows; row++) {
      for (column = 0; column < GULL_COLUMNS; column++) {
        Dma_Fill32Cell(fill, 0, &sprite, sizeof(sprite));
        sprite.tile = tile + column + row * GULL_COLUMNS;
        sprite.x = left + column * 8;
        sprite.y = top + row * 8;
        Dma_Set(&sprite, (u8 *)OAM + (row * GULL_COLUMNS + column) * 8,
                DMA_COPY_WORDS(2), REG_DMA3);
      }
    }
  } else if (logo->phase == TITLE_LOGO_ZOOM) {
    // The side pieces close in from 24 pixels out as the scale reaches rest.
    pieceX = (logo->step - LOGO_MATRIX_REST) / 4 + LOGO_LEFT_X;
    y = (logo->step - LOGO_MATRIX_REST) / 32 + LOGO_Y;
    Title_PlaceLogoPiece(0, pieceX, y);
    Title_PlaceLogoPiece(LOGO_PIECE_TILES, LOGO_MIDDLE_X, y);
    pieceX = LOGO_RIGHT_X - (logo->step - LOGO_MATRIX_REST) / 4;
    Title_PlaceLogoPiece(2 * LOGO_PIECE_TILES, pieceX, y);
    // Affine matrix 0, in the fourth halfwords of the first four entries.
    matrix = (volatile u16 *)((u8 *)OAM + 6);
    *matrix = logo->step;
    matrix += 4;
    *matrix = 0;
    matrix += 4;
    *matrix = 0;
    matrix[4] = logo->step;
    if (logo->step < LOGO_MATRIX_REST) {
      logo->step += 2;
      if (logo->step > LOGO_MATRIX_REST)
        logo->step = LOGO_MATRIX_REST;
    }
    if (logo->frame < BLEND_WEIGHT_MAX) {
      logo->timer++;
      if (logo->timer % 3 == 0) {
        logo->frame++;
        REG_BLDALPHA = BLEND_ALPHA(logo->frame, BLEND_WEIGHT_MAX);
      }
      if (logo->timer == 8)
        LOAD_LOGO_TILES(RESOURCE_SUBTITLE_B)
      else if (logo->timer == 16)
        LOAD_LOGO_TILES(RESOURCE_SUBTITLE_C)
      else if (logo->timer == 24)
        LOAD_LOGO_TILES(RESOURCE_SUBTITLE_D)
    }
  }
}

/// The gull's resource begins with its 16 colours.
#define GULL_PALETTE_BYTES (16 * 2)

/// Shows the lighthouse picture from white and scrolls down it to its foot,
/// flashing white once on the way. There a gull flies off towards the
/// lighthouse and `The Lost Age` closes in as the picture drifts back up, and
/// it ends back at the top under a fade to white. A key cuts it short with a
/// fade to black. Returns 1.
s32 TitleIntro_ShowLighthouse(void) {
  struct TitleScroll scroll;
  volatile u32 fill;
  struct TitleLogoState *logo;
  u8 *source;
  u16 *map;
  u16 tile;
  s32 row;
  s32 i;
  u8 *buffer;

  logo = HeapSlot_Alloc(HEAP_SLOT_TITLE_INTRO, 112);
  REG_DISPCNT = 0;
  Dma_Fill32Cell(fill, 0, (void *)OAM, OAM_SIZE);
  source = Resource_GetTableEntry(RESOURCE_INTRO_GULL_SPRITES);
  buffer = Ram_MapCellBuffer;
  source += GULL_PALETTE_BYTES;
  Decompress_ByteLz(source, buffer);
  Dma_Set(buffer, (void *)OBJ_VRAM0, DMA_COPY_WORDS(6912), REG_DMA3);
  wait(1);
  source = Resource_GetTableEntry(RESOURCE_INTRO_LIGHTHOUSE_PICTURE);
  Dma_Set(source, (void *)BG_PLTT, DMA_COPY_WORDS(BG_PLTT_SIZE / 4), REG_DMA3);
  *(u16 *)BG_PLTT = RGB_WHITE;
  wait(1);
  source += BG_PLTT_SIZE;
  Decompress_ByteLz(source, buffer);
  Dma_Set(buffer, (void *)BG_CHAR_BLOCK(0),
          DMA_COPY_WORDS(SCREEN_TILE_BYTES / 4), REG_DMA3);
  wait(1);

  map = (u16 *)BG_SCREEN_BLOCK(INTRO_BACK_SCREEN);
  tile = 0;
  for (row = 0; row < DISPLAY_TILE_HEIGHT; row++) {
    for (i = 0; i < DISPLAY_TILE_WIDTH; i++) {
      *map++ = tile++;
    }
    *map++ = OFFSCREEN_TILE;
    *map++ = OFFSCREEN_TILE;
  }
  REG_BG1CNT = BGCNT_PRIORITY(3) | BGCNT_256_COLORS |
               BGCNT_SCREEN_BASE(INTRO_BACK_SCREEN);
  REG_BLDCNT = 0;
  REG_BLDALPHA = 0;
  for (i = 0; i < 4; i++) {
    gBgScroll[i].y = 0;
    gBgScroll[i].x = 0;
  }
  Dma_Set((void *)BG_PLTT, Ram_HeapSlots->paletteFade,
          DMA_COPY_WORDS(BG_PLTT_SIZE / 4), REG_DMA3);
  wait(1);
  TitlePaletteFade_SetTarget(PALETTE_TINT_NONE, PALETTES_ALL);
  REG_DISPCNT = DISPCNT_OBJ_1D_MAP | DISPCNT_BG1_ON | DISPCNT_OBJ_ON;
  TitlePaletteFade_Start(10);

  // Down the picture, slowing, with a flash of white near the top.
  scroll.position = 0;
  scroll.limit = 176;
  scroll.step = 4;
  scroll.source = Ram_MapCellBuffer + SCREEN_TILE_BYTES;
  scroll.destination = (u8 *)BG_CHAR_BLOCK(0) + SCREEN_TILE_BYTES;
  scroll.cursor = map;
  scroll.tile = tile;
  scroll.scroll = 0;
  scroll.rows = row;
  scroll.frames = 1;
  logo->toWhite = 10;
  logo->fromWhite = 25;
  if (TitleIntro_ScrollLighthouse(&scroll))
    goto stopped;
  scroll.limit = 200;
  scroll.step = 3;
  if (TitleIntro_ScrollLighthouse(&scroll))
    goto stopped;
  scroll.limit = 216;
  scroll.step = 2;
  if (TitleIntro_ScrollLighthouse(&scroll))
    goto stopped;
  source = Resource_GetTableEntry(RESOURCE_INTRO_GULL_SPRITES);
  Dma_Set(source, (void *)OBJ_PLTT, DMA_COPY_WORDS(GULL_PALETTE_BYTES / 4),
          REG_DMA3);
  logo->phase = TITLE_LOGO_GULL;
  logo->frame = -GULL_FRAME_FIELDS;
  logo->timer = 0;
  logo->step = -GULL_STEP_FIELDS;
  logo->delay = 0;
  scroll.step = 1;
  scroll.limit = LIGHTHOUSE_BOTTOM;
  if (TitleIntro_ScrollLighthouse(&scroll))
    goto stopped;
  Task_Add(TitleIntro_UpdateLogo, TASK_PASS_FRAME);
  // Back up, a pixel every eight frames, then fast under the fade to white,
  // copying the picture's rows back from its row 15, the last the scroll down
  // wrote over, once eleven rows have passed.
  scroll.limit = 160;
  scroll.step = -1;
  scroll.scroll = 0;
#ifdef BUGFIX
  // From row 16, still in place, so that each row lands in its own place and
  // the last copy is row 0.
  scroll.source = Ram_MapCellBuffer + 16 * LIGHTHOUSE_ROW_BYTES;
  scroll.destination = (u8 *)BG_CHAR_BLOCK(0) + 16 * LIGHTHOUSE_ROW_BYTES;
  scroll.cursor =
      (u16 *)BG_SCREEN_BLOCK(INTRO_BACK_SCREEN) + 16 * BG_SCREEN_WIDTH;
  scroll.tile = 16 * DISPLAY_TILE_WIDTH;
#else
  // BUG: each row lands one row low, from VRAM row 16, the map row the cursor
  // rewrites is row 7, one entry in, and the last copy reads the row above the
  // picture; unseen, as the rows are sky under the fade to white.
  scroll.source = Ram_MapCellBuffer + 15 * LIGHTHOUSE_ROW_BYTES;
  scroll.destination = (u8 *)BG_CHAR_BLOCK(0) + 16 * LIGHTHOUSE_ROW_BYTES;
  scroll.cursor =
      (u16 *)BG_SCREEN_BLOCK(INTRO_BACK_SCREEN) + 7 * BG_SCREEN_WIDTH + 1;
  scroll.tile = 15 * DISPLAY_TILE_WIDTH;
#endif
  scroll.rows = 11;
  scroll.frames = 8;
  if (TitleIntro_ScrollLighthouse(&scroll))
    goto stopped;
  TitlePaletteFade_SetTarget(RGB_WHITE, PALETTES_ALL);
  TitlePaletteFade_Start(20);
  scroll.limit = 0;
  scroll.step = -5;
  scroll.frames = 1;
  if (!TitleIntro_ScrollLighthouse(&scroll))
    Title_WaitOrPress(120);
  goto done;
stopped:
  TitlePaletteFade_SetTarget(RGB(0, 0, 0), PALETTES_ALL);
  TitlePaletteFade_Start(10);
  wait(10);
done:
  Dma_Fill32Cell(fill, 0, (void *)OAM, OAM_SIZE);
  Task_Remove(TitleIntro_UpdateLogo);
  HeapSlot_Release(HEAP_SLOT_TITLE_INTRO);
  wait(1);
  return 1;
}

/// Plays the title intro: the clouds, `Camelot presents`, the clouds again,
/// `Golden Sun` `Book Two`, the plunge through the clouds into white, then the
/// lighthouse in its valley. A key fades it out early, skipping the lighthouse.
/// Then it clears the screen for what follows and puts back the CPU-load
/// display it turned off.
s32 TitleIntro_Run(void) {
  u8 cpuLoad;
  s32 result;

  Io_Write8(gCpuLoadDisplayEnabled, &cpuLoad);
  DebugText_Clear();
  VramCache_Reset();
  wait(1);
  Task_ResetAll();
  gOamCopyEnabled = 0;
  gCpuLoadDisplayEnabled = 0;
  TitleIntro_LoadPictureTiles();
  if ((result = TitleIntro_ScrollClouds(CLOUD_BEFORE_CARDS)) == 0 ||
      (result = TitleIntro_ShowPresentsCard()) == 0 ||
      (result = TitleIntro_ScrollClouds(CLOUD_BETWEEN_CARDS)) == 0 ||
      (result = TitleIntro_ShowBookTwoCard()) == 0 ||
      (result = TitleIntro_ScrollClouds(CLOUD_AFTER_CARDS)) == 0) {
    TitlePaletteFade_SetTarget(RGB(0, 0, 0), PALETTES_ALL);
    TitlePaletteFade_Start(10);
    wait(10);
  }
  if (result)
    TitleIntro_ShowLighthouse();
  gOamCopyEnabled = 1;
  wait(1);
  // result is reused as the counter.
  for (result = 0; result < 4; result++)
    gBgScroll[result].x = gBgScroll[result].y = 0;
  REG_DISPCNT = 0;
  Window_LoadGraphics();
  DebugText_Clear();
  TitlePaletteFade_Initialize();
  gCpuLoadDisplayEnabled = cpuLoad;
  return 1;
}

/// The title screen's "Press Start" prompt: five sprites further left in the
/// French and Japanese editions; two rows of three, from eight tile blocks, in
/// German; one row of three elsewhere.
#if (LANGUAGE == LANGUAGE_FRENCH) || (LANGUAGE == LANGUAGE_JAPANESE)
#define PROMPT_SPRITES 5
#define PROMPT_X 56
#define PROMPT_Y 126
#elif LANGUAGE == LANGUAGE_GERMAN
#define PROMPT_SPRITES 3
#define PROMPT_ROWS 2
#define PROMPT_ROW_HEIGHT 8
#define PROMPT_X 80
#define PROMPT_Y 116
#else
#define PROMPT_SPRITES 3
#define PROMPT_X 80
#define PROMPT_Y 124
#endif
#if LANGUAGE == LANGUAGE_GERMAN
#define PROMPT_TILES 8
#else
#define PROMPT_TILES 5
#endif

/// The title screen holds this long with its prompt, and this long without.
#define TITLE_PROMPT_HOLD_FRAMES (90 * FRAMES_PER_SECOND)
#define TITLE_HOLD_FRAMES (11 * FRAMES_PER_SECOND)
/// The prompt pulses once a second, from two seconds in.
#define PROMPT_PULSE_FRAMES FRAMES_PER_SECOND
#define PROMPT_DELAY_FRAMES (2 * FRAMES_PER_SECOND)

/// The Nintendo and Camelot logos are shown as the Golden Sun logo is
/// (scene/title.h), on BG2, each resource beginning with its 224 colours.
#define LOGO_PALETTE_BYTES (LOGO_PALETTE_COLORS * 2)

/// Shows the title screen from white: the Golden Sun logo over the valley and
/// its lighthouse, with, when `prompt` is set, the "Press Start" prompt
/// pulsing. It holds 90 seconds with the prompt and 11 without, and A or Start
/// ends it early, returning 1 (else 0); except in the Japanese edition, the
/// prompt and the keys wait two seconds.
s32 Title_ShowTitleScreen(s32 prompt) {
  s32 blankTile = OFFSCREEN_TILE;
  s32 result = 0;
  struct TitleScreenState *state = HeapSlot_AllocEwram(
      HEAP_SLOT_TITLE_INTRO, sizeof(struct TitleScreenState));
  u8 *data;
  u16 *tilemap;
  u16 tile;
  u32 i;
  u32 j;
  u32 holdFrames;

  Task_ResetAll();
  ScreenFade_ToWhite(1);
  DebugText_Clear();
  VramCache_Reset();
  wait(1);
  REG_BG2CNT = LOGO_BGCNT;
  REG_DISPCNT = DISPCNT_OBJ_1D_MAP | DISPCNT_BG2_ON | DISPCNT_OBJ_ON;
  gBgScroll[2].y = result;
  data = Resource_GetTableEntry(RESOURCE_TITLE_SCREEN_PICTURE);
  Dma_Set(data, (void *)BG_PLTT, DMA_COPY_WORDS(LOGO_PALETTE_BYTES / 4),
          REG_DMA3);
  Decompress_TaggedLz(data + LOGO_PALETTE_BYTES, Ram_MapCellBuffer);
  Dma_Set(Ram_MapCellBuffer, (void *)LOGO_TILES,
          DMA_COPY_WORDS(SCREEN_TILE_BYTES / 4), REG_DMA3);
  tile = LOGO_FIRST_TILE;
  tilemap = LOGO_MAP;
  for (i = 0; i < DISPLAY_TILE_HEIGHT; i++) {
    for (j = 0; j < DISPLAY_TILE_WIDTH; j++)
      *tilemap++ = tile++;
    *tilemap++ = blankTile;
    *tilemap++ = blankTile;
  }
  for (i = 0; i < 4; i++)
    gBgScroll[i].x = gBgScroll[i].y = 0;
  Dma_Set(gBgScroll, REG_BG0HOFS, DMA_COPY_WORDS(4), REG_DMA3);
  Window_LoadGraphics();
  DebugText_Clear();
  if (prompt != 0) {
    u8 *buffer = HeapSlot_AllocEwram(HEAP_SLOT_TILE_SCRATCH, 1024);
    struct IntroSprite *sprite = state->sprites;

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
    if (!TLA_PATCH_1 || i > PROMPT_DELAY_FRAMES) {
      if (prompt != 0) {
        struct IntroSprite *sprite = state->sprites;
        s32 x;
        u32 level;
#ifdef PROMPT_ROWS
        u32 row;
        s32 y;

        for (row = 0, y = PROMPT_Y; row < PROMPT_ROWS;
             y += PROMPT_ROW_HEIGHT, row++) {
          for (j = 0, x = PROMPT_X; j < PROMPT_SPRITES; j++) {
            sprite->x = x;
            sprite->y = y;
            Oam_AddSpriteAtDepthClamped(sprite, 0);
            x += 32;
            sprite++;
          }
        }
#else
        for (j = 0, x = PROMPT_X; j < PROMPT_SPRITES; j++) {
          sprite->x = x;
          sprite->y = PROMPT_Y;
          Oam_AddSpriteAtDepthClamped(sprite, 0);
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
      if (gInput.pressed & (KEY_A | KEY_START)) {
        result = 1;
        break;
      }
    }
    Random16();
    wait(1);
  }
  HeapSlot_Release(HEAP_SLOT_TITLE_INTRO);
  REG_BLDCNT = 0;
  REG_BLDALPHA = 0;
  wait(1);
  return result;
}

/// Unused: an empty entry the far-call table keeps.
void Title_BeforeLogosNoOp(void) {}

/// The Nintendo logo's first tile: character block 1's, as BG2 numbers its
/// 256-colour tiles from block 0.
#define NINTENDO_LOGO_TILE (BG_CHAR_SIZE / TILE_SIZE_8BPP)

/// Shows the Nintendo logo with its chime. With mode 0, which is all the title
/// passes, it appears at once and holds two seconds; any other mode, unused,
/// waits a second, fades in, holds three seconds and fades out, faster once
/// skipped. Returns -1 when A or Start skipped it.
s32 Title_ShowNintendoLogo(s32 mode) {
  s32 result;
  u8 *data;
  u16 *map;
  u32 x;
  u32 y;
  u16 tile;
  s32 blank = OFFSCREEN_TILE;
  s32 resource;

  Sound_PlayFar(SE_CHIME);
  gOamCopyEnabled = 1;
  resource = RESOURCE_NINTENDO_LOGO;
  Task_ResetAll();
  ScreenFade_ToBlack(1);
  DebugText_Clear();
  wait(1);
  REG_BG2CNT = LOGO_BGCNT;
  REG_DISPCNT = DISPCNT_OBJ_1D_MAP | DISPCNT_BG2_ON | DISPCNT_OBJ_ON;
  result = 0;
  gBgScroll[2].y = result;
  data = Resource_GetTableEntry(resource);
  Dma_Set(data, (void *)BG_PLTT, DMA_COPY_WORDS(LOGO_PALETTE_BYTES / 4),
          REG_DMA3);
  Decompress_TaggedLz(data + LOGO_PALETTE_BYTES, Ram_MapCellBuffer);
  Dma_Set(Ram_MapCellBuffer, (void *)BG_CHAR_BLOCK(1),
          DMA_COPY_WORDS(SCREEN_TILE_BYTES / 4), REG_DMA3);
  // Pad the 30-tile picture to the hardware map's 32-tile row width.
  tile = NINTENDO_LOGO_TILE;
  map = LOGO_MAP;
  for (y = 0; y < DISPLAY_TILE_HEIGHT; y++) {
    for (x = 0; x < DISPLAY_TILE_WIDTH; x++)
      *map++ = tile++;
    *map++ = blank;
    *map++ = blank;
  }
  for (y = 0; y < 4; y++) {
    gBgScroll[y].y = 0;
    gBgScroll[y].x = 0;
  }
  Dma_Set(gBgScroll, REG_BG0HOFS, DMA_COPY_WORDS(4), REG_DMA3);
  Window_LoadGraphics();
  DebugText_Clear();
  REG_DISPCNT =
      DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON | DISPCNT_BG2_ON | DISPCNT_OBJ_ON;
  if (mode == 0) {
    ScreenFade_FromBlack(1);
    ScreenFade_Wait();
    for (y = 0; y < 2 * FRAMES_PER_SECOND; y++) {
      if (gInput.pressed & (KEY_A | KEY_START)) {
        result = -1;
        break;
      }
      wait(1);
    }
    return result;
  }
  for (y = 0; y < FRAMES_PER_SECOND; y++) {
    if (gInput.pressed & (KEY_A | KEY_START)) {
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
    for (y = 0; y < 3 * FRAMES_PER_SECOND; y++) {
      if (gInput.pressed & (KEY_A | KEY_START)) {
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

/// Shows the Camelot logo for two seconds, cycling its four frames of tiles. A
/// or Start ends it early.
s32 Title_ShowCamelotLogo(void) {
  s32 result;
  u32 i;
  u8 *buffer;
  s32 resource;

  gOamCopyEnabled = 1;
  result = 0;
  resource = RESOURCE_CAMELOT_LOGO;
  Task_ResetAll();
  ScreenFade_ToBlack(1);
  DebugText_Clear();
  wait(1);
  REG_BG2CNT = LOGO_BGCNT | BGCNT_CHAR_BASE(1);
  REG_DISPCNT = DISPCNT_OBJ_1D_MAP | DISPCNT_BG2_ON | DISPCNT_OBJ_ON;
  gBgScroll[2].y = result;
  buffer = Resource_GetTableEntry(resource);
  Decompress_TaggedLz(buffer, Ram_MapCellBuffer);
  // The unpacked resource: 224 colours, the map, the still tiles, then the
  // flag's four pictures.
  buffer = Ram_MapCellBuffer;
  Dma_Set(buffer, (void *)BG_PLTT, DMA_COPY_WORDS(LOGO_PALETTE_BYTES / 4),
          REG_DMA3);
  buffer += LOGO_PALETTE_BYTES;
  Dma_Set(buffer, LOGO_MAP, DMA_COPY_WORDS(BG_SCREEN_SIZE / 4), REG_DMA3);
  buffer += BG_SCREEN_SIZE;
  Dma_Set(buffer, BG_CHAR_BLOCK(1), DMA_COPY_WORDS(BG_CHAR_SIZE / 4), REG_DMA3);
  buffer += BG_CHAR_SIZE;
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
    Dma_Set(((gFrameCount >> 3) & 3) * CAMELOT_FLAG_PICTURE_BYTES + buffer,
            BG_CHAR_BLOCK(1) + CAMELOT_FLAG_TILE * TILE_SIZE_8BPP,
            DMA_COPY_WORDS(CAMELOT_FLAG_BYTES / 4), REG_DMA3);
    if (gInput.pressed & (KEY_A | KEY_START))
      break;
    wait(1);
  }
  return result;
}

/// Unused: an empty entry the far-call table keeps.
void Title_AfterIntroNoOp(void) {}

// The palette fade (struct TitlePaletteFade) keeps the saved palettes, the
// colour components shown now, the target components and each one's step.

/// A component is a colour's five-bit channel shifted into the high bits of a
/// halfword, with ten bits of fraction below it.
#define COMPONENT_SHIFT 10

/// Computes the change per frame of each of the TITLE_PALETTE_FADE_CHANNELS
/// colour components, from the current components to the target (nothing when
/// `frames` is not positive).
void TitlePaletteFade_CalculateSteps(s16 *current, s16 *target, s16 *step,
                                     s32 frames) {
  s32 n;
  s32 from;
  s32 to;

  if (frames > 0) {
    for (n = 0; n < TITLE_PALETTE_FADE_CHANNELS; n++) {
      from = *current;
      to = *target;
      *step = Iwram_SignedDivide(to - from, frames);
      current++;
      target++;
      step++;
    }
  }
}

/// A task added by TitlePaletteFade_Initialize: steps the palette fade each
/// frame, adding the step to every colour component until the fade is done,
/// packs the components into the spare RGB555 copy of the palettes and queues
/// both palettes for VBlank.
void TitlePaletteFade_Update(void) {
  struct TitlePaletteFade *state = Ram_HeapSlots->paletteFade;
  u16 *delta = state->delta;
  u16 *current;
  u16 *packed;
  s32 i;
  u16 *bank;
  s32 blueMask;

  if (state->duration == 0)
    return;
  if ((s8)++state->step < state->duration) {
    current = state->current;
    for (i = 0; i <= TITLE_PALETTE_FADE_CHANNELS - 1; i++)
      *current++ += *delta++;
  } else {
    Dma_Set(state->target, state->current,
            DMA_COPY_WORDS(sizeof(state->current) / 4), REG_DMA3);
    state->duration = 0;
  }
  // The queued DMA keeps a pointer until VBlank.
  packed = state->packed[state->page ^ 1];
  blueMask = BLUE_BITS;
  i = PLTT_COLORS;
  current = state->current;
  for (; i != 0; i--) {
    *packed++ = (current[0] & blueMask) |
                (((s16)current[1] >> (COMPONENT_SHIFT - RGB_GREEN_SHIFT)) &
                 GREEN_BITS) |
                (((s16)current[2] >> COMPONENT_SHIFT) & RED_BITS);
    current += 3;
  }
  state->page ^= 1;
  bank = state->packed[state->page];
  IO_WRITE_QUEUE_PUSH((u32)BG_PLTT, (u32)bank,
                      DMA_COPY_WORDS(BG_PLTT_SIZE / 4));
  IO_WRITE_QUEUE_PUSH((u32)OBJ_PLTT, (u32)(bank + BG_PLTT_COLORS),
                      DMA_COPY_WORDS(BG_PLTT_SIZE / 4));
}

/// Tints 512 source colours into destination components (three per colour, each
/// in the high bits of a halfword). An enum PaletteTint picks the tint: a
/// colour below PALETTE_TINT_FROM_SOURCE fills every colour with it;
/// PALETTE_TINT_FROM_SOURCE takes that colour from source[0]; PALETTE_TINT_NONE
/// to PALETTE_TINT_WARM_DARK are the whole-palette filters; the CUSTOM, BLEND
/// and SPLIT flags carry a colour in their low bits; anything else is the
/// address of components to copy. `selectedPalettes` is an enum
/// PaletteSelection.
void TitlePaletteFade_BuildColors(u32 tint, u16 *source, u16 *destination,
                                  s32 selectedPalettes) {
  u32 colorIndex;
  u32 colorCount = PLTT_COLORS;
  s32 red;
  s32 blue;
  u32 color;
  s32 green;
  s32 tintedRed;

  if (tint == PALETTE_TINT_FROM_SOURCE)
    tint = *source;
  if (selectedPalettes == PALETTES_BG) {
    colorCount = BG_PLTT_COLORS;
  } else if (selectedPalettes == PALETTES_OBJECTS) {
    destination += 3 * BG_PLTT_COLORS;
    source += BG_PLTT_COLORS;
    colorCount = BG_PLTT_COLORS;
  }

  if (tint < PALETTE_TINT_FROM_SOURCE) {
    *destination++ = tint & BLUE_BITS;
    *destination++ = (tint & GREEN_BITS) << (COMPONENT_SHIFT - RGB_GREEN_SHIFT);
    *destination++ = (tint & RED_BITS) << COMPONENT_SHIFT;
    Dma_Set(destination - 3, destination,
            DMA_COPY_HALFWORDS(((colorCount - 1) * 6) >> 1), REG_DMA3);
  } else if (tint < PALETTE_TINT_FLAGS) {
    switch (tint) {
    case PALETTE_TINT_GRAYSCALE:
      for (colorIndex = 0; colorIndex < colorCount; colorIndex++) {
        color = *source++;
        red = ((color << BRIGHTNESS_RED_SHIFT) &
               (RED_BITS << BRIGHTNESS_RED_SHIFT));
        green = ((color << BRIGHTNESS_GREEN_SHIFT) &
                 (GREEN_BITS << BRIGHTNESS_GREEN_SHIFT));
        blue = (color & BLUE_BITS);
        color = Iwram_SignedDivide(red + green + blue, 2 + 4 + 1);
        *destination++ = color;
        *destination++ = color;
        *destination++ = color;
      }
      break;
    case PALETTE_TINT_SEPIA:
      for (colorIndex = 0; colorIndex < colorCount; colorIndex++) {
        color = *source++;
        red = RGB_RED(color);
        green = RGB_GREEN(color);
        blue = RGB_BLUE(color);
        color = Iwram_SignedDivide(red + green + blue, SEPIA_SHADE_DIVISOR);
        red = color * 4 + 5;
        blue = color * 3 + 5;
        green = color * 3 + 5;
        if (red < SEPIA_LEVEL_MIN)
          red = SEPIA_LEVEL_MIN;
        if (green < SEPIA_LEVEL_MIN)
          green = SEPIA_LEVEL_MIN;
        if (blue < SEPIA_LEVEL_MIN)
          blue = SEPIA_LEVEL_MIN;
        if (red > SEPIA_LEVEL_MAX)
          red = SEPIA_LEVEL_MAX;
        if (green > SEPIA_LEVEL_MAX)
          green = SEPIA_LEVEL_MAX;
        if (blue > SEPIA_LEVEL_MAX)
          blue = SEPIA_LEVEL_MAX;
        *destination++ = gTitlePaletteFade_ChannelCurveLight[blue];
        *destination++ = gTitlePaletteFade_ChannelCurveLight[green];
        *destination++ = gTitlePaletteFade_ChannelCurveLight[red];
      }
      break;
    // The same as PALETTE_TINT_WARM_DARK below, where the field's fade warms
    // more.
    case PALETTE_TINT_WARM:
      for (colorIndex = 0; colorIndex < colorCount; colorIndex++) {
        color = *source++;
        red = RGB_RED(color);
        green = RGB_GREEN(color);
        blue = RGB_BLUE(color);
        red -= (u32)red >> 1;
        green -= green / 3;
        red += 6;
        green += 4;
        blue -= 6;
        red = TitlePaletteFade_ClampChannel(red);
        green = TitlePaletteFade_ClampChannel(green);
        blue = TitlePaletteFade_ClampChannel(blue);
        SET_VECTOR(destination, gTitlePaletteFade_ChannelCurveContrast[blue],
                   gTitlePaletteFade_ChannelCurveLight[green],
                   gTitlePaletteFade_ChannelCurve[red]);
        destination += 3;
      }
      break;
    case PALETTE_TINT_MUTED:
      for (colorIndex = 0; colorIndex < colorCount; colorIndex++) {
        color = *source++;
        red = RGB_RED(color);
        green = RGB_GREEN(color);
        blue = RGB_BLUE(color);
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
        red = TitlePaletteFade_ClampChannel(red);
        green = TitlePaletteFade_ClampChannel(green);
        blue = TitlePaletteFade_ClampChannel(blue);
        *destination++ = gTitlePaletteFade_ChannelCurve[blue];
        *destination++ = gTitlePaletteFade_ChannelCurve[green];
        *destination++ = gTitlePaletteFade_ChannelCurve[red];
      }
      break;
    case PALETTE_TINT_PALE:
      for (colorIndex = 0; colorIndex < colorCount; colorIndex++) {
        s32 mean;

        color = *source++;
        red = RGB_RED(color);
        green = RGB_GREEN(color);
        blue = RGB_BLUE(color);
        mean = TitlePaletteFade_ClampChannel((red + green + blue) / 3);
        red = (red >> 1) + mean;
        green = (green >> 1) + mean;
        blue = (blue >> 1) + mean;
        red = TitlePaletteFade_ClampChannel(red);
        green = TitlePaletteFade_ClampChannel(green);
        blue = TitlePaletteFade_ClampChannel(blue);
        SET_VECTOR(destination, gTitlePaletteFade_ChannelCurveContrast[blue],
                   gTitlePaletteFade_ChannelCurveContrast[green],
                   gTitlePaletteFade_ChannelCurveContrast[red]);
        destination += 3;
      }
      break;
    case PALETTE_TINT_RED:
      for (colorIndex = 0; colorIndex < colorCount; colorIndex++) {
        color = *source++;
        green = RGB_GREEN(color);
        red = RGB_RED(color);
        blue = RGB_BLUE(color);
        red += ((green >> 3) + (blue >> 3));
        red = TitlePaletteFade_ClampChannel(red);
        green -= green / 3;
        blue -= blue / 3;
        SET_VECTOR(destination, gTitlePaletteFade_ChannelCurve[blue],
                   gTitlePaletteFade_ChannelCurve[green],
                   gTitlePaletteFade_ChannelCurveLight[red]);
        destination += 3;
      }
      break;
    case PALETTE_TINT_WARM_DARK:
      for (colorIndex = 0; colorIndex < colorCount; colorIndex++) {
        color = *source++;
        red = RGB_RED(color);
        green = RGB_GREEN(color);
        blue = RGB_BLUE(color);
        red -= (u32)red >> 1;
        green -= green / 3;
        red += 6;
        green += 4;
        blue -= 6;
        red = TitlePaletteFade_ClampChannel(red);
        green = TitlePaletteFade_ClampChannel(green);
        blue = TitlePaletteFade_ClampChannel(blue);
        SET_VECTOR(destination, gTitlePaletteFade_ChannelCurveContrast[blue],
                   gTitlePaletteFade_ChannelCurveLight[green],
                   gTitlePaletteFade_ChannelCurve[red]);
        destination += 3;
      }
      break;
    default:
      for (colorIndex = 0; colorIndex < colorCount; colorIndex++) {
        color = *source++;
        destination[colorIndex * 3 + 0] = color & BLUE_BITS;
        destination[colorIndex * 3 + 1] =
            (color & GREEN_BITS) << (COMPONENT_SHIFT - RGB_GREEN_SHIFT);
        destination[colorIndex * 3 + 2] = (color & RED_BITS) << COMPONENT_SHIFT;
      }
      break;
    }
  } else if (tint & PALETTE_TINT_CUSTOM) {
    u32 tintRed;
    u32 tintGreen;
    u32 tintBlue;

    tintRed = RGB_RED(tint);
    tintGreen = RGB_GREEN(tint);
    tintBlue = RGB_BLUE(tint);
    for (colorIndex = 0; colorIndex < colorCount; colorIndex++) {
      color = *source++;
      red = ((color << BRIGHTNESS_RED_SHIFT) &
             (RED_BITS << BRIGHTNESS_RED_SHIFT));
      green = ((color << BRIGHTNESS_GREEN_SHIFT) &
               (GREEN_BITS << BRIGHTNESS_GREEN_SHIFT));
      blue = (color & BLUE_BITS);
      color = Iwram_SignedDivide(red + green + blue, CUSTOM_TINT_DIVISOR);
      tintedRed = color * tintRed;
      green = color * tintGreen;
      blue = color * tintBlue;
      tintedRed = TitlePaletteFade_ClampShiftedChannel(tintedRed);
      green = TitlePaletteFade_ClampShiftedChannel(green);
      blue = TitlePaletteFade_ClampShiftedChannel(blue);
      destination[colorIndex * 3 + 0] = blue;
      destination[colorIndex * 3 + 1] = green;
      destination[colorIndex * 3 + 2] = tintedRed;
    }
  } else if (tint & PALETTE_TINT_BLEND) {
    u32 tintRed;
    u32 tintGreen;
    u32 tintBlue;
    s32 scaledGreen;
    s32 scaledBlue;
    s32 clampedRed;
    s32 clampedGreen;
    s32 clampedBlue;

    tintRed = RGB_RED(tint);
    tintGreen = RGB_GREEN(tint);
    tintBlue = RGB_BLUE(tint);
    for (colorIndex = 0; colorIndex < colorCount; colorIndex++) {
      color = *source++;
      red = RGB_RED(color);
      green = RGB_GREEN(color);
      blue = RGB_BLUE(color);
      color = Iwram_SignedDivide((red + green + blue) << 4,
                                 tintRed + tintGreen + tintBlue);
      tintedRed = Iwram_MulFX(INTtoFIXED((color * tintRed) >> 4),
                              (s32)INTtoFIXED(tintRed) >> 4);
      scaledGreen = Iwram_MulFX(INTtoFIXED((color * tintGreen) >> 4),
                                (s32)INTtoFIXED(tintGreen) >> 4);
      scaledBlue = Iwram_MulFX(INTtoFIXED((color * tintBlue) >> 4),
                               (s32)INTtoFIXED(tintBlue) >> 4);
      tintedRed = FIXEDtoINT((u32)tintedRed);
      clampedRed = TitlePaletteFade_ClampChannel(tintedRed);
      scaledGreen = FIXEDtoINT((u32)scaledGreen);
      clampedGreen = TitlePaletteFade_ClampChannel(scaledGreen);
      scaledBlue = FIXEDtoINT((u32)scaledBlue);
      clampedBlue = TitlePaletteFade_ClampChannel(scaledBlue);
      destination[colorIndex * 3 + 0] =
          gTitlePaletteFade_ChannelCurve[clampedBlue];
      destination[colorIndex * 3 + 1] =
          gTitlePaletteFade_ChannelCurve[clampedGreen];
      destination[colorIndex * 3 + 2] =
          gTitlePaletteFade_ChannelCurve[clampedRed];
    }
  } else if (tint & PALETTE_TINT_SPLIT) {
    for (colorIndex = 0; colorIndex < colorCount; colorIndex++) {
      color = *source++;
      destination[colorIndex * 3 + 0] = color & BLUE_BITS;
      destination[colorIndex * 3 + 1] = (color & GREEN_BITS)
                                        << (COMPONENT_SHIFT - RGB_GREEN_SHIFT);
      destination[colorIndex * 3 + 2] = (color & RED_BITS) << COMPONENT_SHIFT;
    }
  } else {
    if (selectedPalettes == PALETTES_OBJECTS)
      tint += 3 * BG_PLTT_SIZE; // past the background colours' components
    Dma_Set((const void *)tint, destination,
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

/// Sets the fade target by tinting the saved background and/or sprite palettes
/// with the requested tint.
void TitlePaletteFade_SetTarget(s32 tint, s32 selectedPalettes) {
  struct TitlePaletteFade *fade = Ram_HeapSlots->paletteFade;

  if (fade != NULL)
    TitlePaletteFade_BuildColors(tint, fade->palette, fade->target,
                                 selectedPalettes);
}

/// Sets the colours shown now by tinting the saved palettes.
void TitlePaletteFade_SetCurrent(s32 tint, s32 selectedPalettes) {
  struct TitlePaletteFade *fade = Ram_HeapSlots->paletteFade;

  if (fade != NULL)
    TitlePaletteFade_BuildColors(tint, fade->palette, fade->current,
                                 selectedPalettes);
}

/// Unused: replaces the first saved colour, the backdrop.
UNUSED void TitlePaletteFade_SetBackdrop(s32 color) {
  struct TitlePaletteFade *fade = Ram_HeapSlots->paletteFade;

  if (fade != NULL)
    fade->palette[0] = color;
}

/// Starts fading the shown colours to the target over the requested frames:
/// each of the components (three for each of the 512 colours) steps by
/// its distance divided by the frames, the remainder kept in the ten bits below
/// it, so every colour arrives together.
void TitlePaletteFade_Start(s32 frames) {
  struct TitlePaletteFade *fade = Ram_HeapSlots->paletteFade;

  if (fade != NULL) {
    fade->duration = frames;
    fade->step = 0;
    TitlePaletteFade_CalculateSteps((s16 *)fade->current, (s16 *)fade->target,
                                    (s16 *)fade->delta, frames);
  }
}

/// Holds a colour channel's level to 0 to 31.
s32 TitlePaletteFade_ClampChannel(s32 value) {
  if (value > RGB_LEVEL_MAX)
    return RGB_LEVEL_MAX;
  if (value < 0)
    value = 0;
  return value;
}

/// Caps a colour component at a channel's brightest level shifted into the high
/// bits. There is no lower bound.
s32 TitlePaletteFade_ClampShiftedChannel(s32 value) {
  if (value > RGB_LEVEL_MAX << COMPONENT_SHIFT)
    value = RGB_LEVEL_MAX << COMPONENT_SHIFT;
  return value;
}
