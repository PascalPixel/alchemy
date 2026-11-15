/// @file
/// The Title's intro: the Nintendo and Camelot splash screens, the scroll down
/// the intro's tall picture, the logo coming together, the title screen and its
/// prompt, and the palette fades they share.
///
/// One object, as The Broken Seal's title_intro.c is. Its logo frames and
/// palettes are the data objects logo.c and title_intro_tables.s.

#include "types.h"
#include "sounds.h"
#include "graphics/graphics.h"
#include "system/system.h"
#include "edition.h"
#include "version.h"
#include "macros.h"
#include "system/dma.h"
#include "system/io_reg.h"
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

/// One step of the intro picture's scroll: how many frames it lasts, and for
/// each of BG1's and BG2's y and x the pixels it moves a frame and the mask of
/// frame counts it moves on (0 for every frame).
struct TitleScrollStep {
  s32 frames;
  s32 speed[4];
  u32 mask[4];
};

/// The scroll down or up the intro's tall lighthouse picture: where it is and
/// where it stops, the pixels a step, the picture's rows streamed into VRAM
/// behind it and the frames each step waits.
struct TitleScroll {
  s16 pos;
  s16 limit;
  u16 step;
  u8 *src;
  u8 *dst;
  u16 *cursor;
  u16 tile;
  s16 scroll;
  u16 rows;
  u32 frames;
};

/// An OAM entry's attributes 0 to 2, as the title's logo pieces are written.
struct OamEntry {
  u32 y : 8;
  u32 affine : 2;
  u32 mode : 2;
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

/// The title intro's state (HEAP_SLOT_TITLE_INTRO): the logo's phase, frame,
/// timer, step and delay, and the countdowns to the fade to white and back that
/// the lighthouse scroll starts.
struct TitleLogoState {
  s16 phase;
  s16 frame; ///< index into gTitleIntro_GullFrames (5 halfwords a frame)
  s16 timer;
  s16 step; ///< index into gTitleIntro_GullPath (3 halfwords a step); the scale
            ///< in phase 1
  s16 delay;
  s16 white;
  s16 back;
};

/// A sprite of the "Press Start" prompt, linked into a render slot each frame:
/// OAM attributes 0 to 2 with the position split out.
struct TitleSprite {
  u32 next; ///< the render slot link
  u8 y;
  u8 attr0High; ///< shape and mode bits of attribute 0
  u16 x : 9;
  u16 attr1High : 7; ///< flip and size bits of attribute 1
  s32 tile;
};

/// The title screen's state, in heap slot 172.
struct TitleScreenWork {
  u8 unused[16]; ///< no code names it
  struct TitleSprite sprites[8];
};

void TitlePaletteFade_SetCurrent(s32 index, s32 transform);
extern u8 Data_02026800[];
extern u8 Data_02019600[];
extern u8 Data_02017080[];
extern const u8 gTitle_PromptBlendLevels[];
extern u16 gTitleIntro_ChannelCurve[];
extern u16 gTitleIntro_ChannelCurveLight[];
extern u16 gTitleIntro_ChannelCurveContrast[];
s32 TitlePaletteFade_ClampComponent(s32 value);
s32 TitlePaletteFade_ClampShiftedComponent(s32 value);

#include "system/system_bank.h"
#include "resource_ids.h"

#if (LANGUAGE == LANGUAGE_GERMAN) || (LANGUAGE == LANGUAGE_SPANISH) ||         \
    (LANGUAGE == LANGUAGE_ITALIAN)
#define DATA018_TILE_BYTES 0xa000
#else
#define DATA018_TILE_BYTES 0x9c00
#endif

/// Blanks the display, loads resource 0x18's palette into the BG palette with
/// colour 0 black, and unpacks its tiles through the map cell buffer into BG
/// character block 0.
void TitleIntro_LoadPictureTiles(void) {
  u8 *source;
  u8 *cells;

  REG_DISPCNT = 0;
  source = Resource_GetTableEntry((s32)&ResourceId_Data018);
  Dma_Set(source, (void *)BG_PLTT, DMA_COPY_WORDS(BG_PLTT_SIZE / 4), REG_DMA3);
  BG_PLTT[0] = 0;
  cells = Ram_MapCellBuffer;
  Resource_DecodeByteLzInRam(source + BG_PLTT_SIZE, cells);
  Dma_Set(cells, (void *)BG_CHAR_BLOCK(0),
          DMA_COPY_WORDS(DATA018_TILE_BYTES / 4), REG_DMA3);
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

/// Runs one step of the intro picture's scroll, moving BG1 and BG2 each frame
/// by the step's speeds on the frames its masks allow; returns 0 when a key
/// cuts it short, 1 when it ends.
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

/// One step of the intro picture's scroll: for `duration` frames, BG1 and BG2
/// move by their dy and dx each frame whose count has none of its mask's bits.
#define TITLE_STEP(duration, bg1Dy, bg1Dx, bg2Dy, bg2Dx, bg1DyMask, bg1DxMask, \
                   bg2DyMask, bg2DxMask)                                       \
  (step.frames = (duration), step.speed[0] = (bg1Dy), step.speed[1] = (bg1Dx), \
   step.speed[2] = (bg2Dy), step.speed[3] = (bg2Dx),                           \
   step.mask[0] = (bg1DyMask), step.mask[1] = (bg1DxMask),                     \
   step.mask[2] = (bg2DyMask), step.mask[3] = (bg2DxMask),                     \
   Title_RunScrollStep(&step))

/// Shows the intro's picture of step mode (0 to 2) and runs its scroll steps;
/// returns 0 when a key cuts it short.
s32 TitleIntro_ScrollPicture(s32 mode) {
  struct TitleScrollStep step;
  u16 *map;
  u16 tile;
  s32 i;
  s32 j;

  REG_DISPCNT = 0;
  map = (u16 *)BG_SCREEN_BLOCK(31);
  tile = 0;
  for (j = 0; j < BG_SCREEN_WIDTH / 2; j++) {
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
  map = (u16 *)BG_SCREEN_BLOCK(30);
  tile = 0;
  for (j = 0; j < BG_SCREEN_WIDTH / 2; j++) {
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
  REG_BG1CNT = BGCNT_PRIORITY(2) | BGCNT_SCREEN_BASE(31);
  REG_BG2CNT = BGCNT_PRIORITY(1) | BGCNT_CHAR_BASE(1) | BGCNT_SCREEN_BASE(30);
  REG_BLDCNT = BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_BG1;
  REG_BLDALPHA = BLEND_ALPHA(11, 10);
  for (i = 0; i < 4; i++) {
    gBgScroll[i].y = 0;
    gBgScroll[i].x = 0;
  }
  if (mode == 0) {
    TitlePaletteFade_Initialize();
  }
  TitlePaletteFade_SetCurrent(RGB(2, 0, 0), PALETTES_ALL);
  TitlePaletteFade_SetTarget(PALETTE_TINT_NONE, PALETTES_ALL);
  REG_DISPCNT = DISPCNT_BG1_ON | DISPCNT_BG2_ON;
  if (mode == 0) {
    TitlePaletteFade_Start(1);
  } else {
    TitlePaletteFade_Start(10);
  }

  if (mode == 0) {
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
  } else if (mode == 1) {
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
    j = 2;
    for (i = 11; i < 16; i++) {
      REG_BLDALPHA = (i << 8) | i;
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

// The picture numbers three rows in these editions, two in the others.
#if LANGUAGE == LANGUAGE_GERMAN || LANGUAGE == LANGUAGE_SPANISH ||             \
    LANGUAGE == LANGUAGE_ITALIAN
#define NUMBERED_ROWS 3
#else
#define NUMBERED_ROWS 2
#endif

/// Fades in a card of the picture's numbered rows on BG3, holds it 150 frames
/// and fades it out; returns 0 when a key cuts it short.
s32 TitleIntro_ShowCard(void) {
  u16 *map;
  u16 tile;
  s32 i;
  s32 row;

  REG_DISPCNT = 0;
  map = (u16 *)BG_SCREEN_BLOCK(29);
  tile = 0;
  for (row = 0; row < DISPLAY_TILE_HEIGHT; row++) {
    if (row >= 9 && row < 9 + NUMBERED_ROWS) {
      for (i = 0; i < BG_SCREEN_WIDTH; i++) {
        *map++ = tile++ | BG_TILE_PALETTE(2);
      }
    } else {
      for (i = 0; i < BG_SCREEN_WIDTH; i++)
        *map++ = BG_TILE_PALETTE(2);
    }
  }
  REG_BG3CNT = BGCNT_PRIORITY(2) | BGCNT_CHAR_BASE(2) | BGCNT_SCREEN_BASE(29);
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

// This picture's tiles follow TitleIntro_ShowCard's, 32 for each row it
// numbers.
#if LANGUAGE == LANGUAGE_GERMAN || LANGUAGE == LANGUAGE_SPANISH ||             \
    LANGUAGE == LANGUAGE_ITALIAN
#define FIRST_TILE 96
#else
#define FIRST_TILE 64
#endif

/// As TitleIntro_ShowCard, with a second layer of rows blended in over 16
/// steps of 3 frames.
s32 TitleIntro_ShowBlendedCard(void) {
  u16 *map;
  u16 tile;
  s32 i;
  s32 row;

  REG_DISPCNT = 0;
  map = (u16 *)BG_SCREEN_BLOCK(29);
  tile = FIRST_TILE;
  for (row = 0; row < DISPLAY_TILE_HEIGHT; row++) {
    if (row >= 7 && row <= 9) {
      for (i = 0; i < BG_SCREEN_WIDTH; i++) {
        *map++ = tile++ | BG_TILE_PALETTE(3);
      }
    } else if (row == 10 || row == 11) {
      for (i = 0; i < BG_SCREEN_WIDTH; i++)
        *map++ = BG_TILE_PALETTE(3) | 96;
    } else {
      for (i = 0; i < BG_SCREEN_WIDTH; i++)
        *map++ = BG_TILE_PALETTE(3) | 64;
    }
  }
  map = (u16 *)BG_SCREEN_BLOCK(28);
  tile = FIRST_TILE + 96;
  for (row = 0; row < DISPLAY_TILE_HEIGHT; row++) {
    if (row == 10 || row == 11) {
      for (i = 0; i < BG_SCREEN_WIDTH; i++) {
        *map++ = tile++ | BG_TILE_PALETTE(3);
      }
    } else {
      for (i = 0; i < BG_SCREEN_WIDTH; i++)
        *map++ = BG_TILE_PALETTE(3) | 64;
    }
  }
  REG_BG3CNT = BGCNT_PRIORITY(2) | BGCNT_CHAR_BASE(2) | BGCNT_SCREEN_BASE(29);
  REG_BG0CNT = BGCNT_PRIORITY(1) | BGCNT_CHAR_BASE(2) | BGCNT_SCREEN_BASE(28);
  REG_BLDCNT = BLDCNT_TGT1_BG0 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_BG3;
  REG_BLDALPHA = BLEND_ALPHA(0, 16);
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

/// A row of the tall picture's 256-colour tiles, streamed in as it scrolls:
/// the screen's width in tiles.
#define LIGHTHOUSE_ROW_BYTES (DISPLAY_TILE_WIDTH * 64)
/// The map entry of the blank tile beside each streamed row.
#define LIGHTHOUSE_BLANK_TILE 511

/// Scrolls the intro's tall picture from one position to another: each step
/// moves it by the scroll's step and waits its frames, streams in the next map
/// rows' tiles when scrolling down and takes them back when scrolling up, and
/// starts the fade to white or back when its countdown reaches 1. A key press
/// cuts the scroll short.
s32 TitleIntro_ScrollLighthouse(struct TitleScroll *s) {
  struct TitleLogoState *timers = Ram_HeapSlots->titleIntro;
  s32 i;
  s32 j;
  s32 step;

  step = s->step;
  if ((s16)step > 0) {
    for (i = s->pos; i < s->limit; i += (s16)s->step) {
      if (Title_WaitOrPress(s->frames))
        goto fail;
      if (s->scroll > 0) {
        Dma_Set(s->src, s->dst, DMA_COPY_WORDS(LIGHTHOUSE_ROW_BYTES / 4),
                REG_DMA3);
        s->src += LIGHTHOUSE_ROW_BYTES;
        s->dst += LIGHTHOUSE_ROW_BYTES;
        for (j = 0; j < DISPLAY_TILE_WIDTH; j++)
          *s->cursor++ = s->tile++;
        *s->cursor++ = LIGHTHOUSE_BLANK_TILE;
        *s->cursor++ = LIGHTHOUSE_BLANK_TILE;
        if (++s->rows == BG_SCREEN_WIDTH) {
          s->dst = (u8 *)BG_CHAR_BLOCK(0);
          s->rows = 0;
          s->tile = 0;
          s->cursor = (u16 *)BG_SCREEN_BLOCK(31);
        }
        s->scroll -= 8;
      }
      s->scroll += s->step;
      gBgScroll[1].y += s->step;
      if (timers->white != 0 && --timers->white == 1) {
        TitlePaletteFade_SetTarget(RGB_WHITE, PALETTES_ALL);
        TitlePaletteFade_Start(10);
      }
      if (timers->back != 0 && --timers->back == 1) {
        TitlePaletteFade_SetTarget(PALETTE_TINT_NONE, PALETTES_ALL);
        TitlePaletteFade_Start(10);
      }
    }
    goto done;
  fail:
    return 1;
  } else {
    for (i = s->pos; i > s->limit; i += (s16)step) {
      gBgScroll[1].y += step;
      s->scroll -= step;
      if (s->scroll > 8) {
        s->rows--;
        if (s->rows == 0) {
          s->rows = 1;
          Dma_Set(s->src, s->dst, DMA_COPY_WORDS(LIGHTHOUSE_ROW_BYTES / 4),
                  REG_DMA3);
          s->src -= LIGHTHOUSE_ROW_BYTES;
          s->dst -= LIGHTHOUSE_ROW_BYTES;
          for (j = 0; j < DISPLAY_TILE_WIDTH; j++)
            *s->cursor++ = s->tile++;
          s->cursor -= DISPLAY_TILE_WIDTH;
          s->tile -= DISPLAY_TILE_WIDTH;
        }
        s->scroll -= 8;
      }
      if (Title_WaitOrPress(s->frames))
        goto fail;
      step = *(volatile u16 *)&s->step;
    }
  }
done:
  s->pos = i;
  return 0;
}

/// Writes one 64x64 piece of the title logo straight into OAM: double-size
/// affine, semi-transparent, 256 colours, at x - 32 and y + 224, in the OAM
/// entry its tile picks.
void Title_PlaceLogoPiece(s32 tile, s32 x, u32 y) {
  struct OamEntry oam;

  Dma_Fill32(0, &oam, sizeof(oam));
  oam.shape = 0;
  oam.size = 3;
  oam.fullColor = 1;
  oam.mode = 1;
  oam.affine = 3;
  oam.tile = tile;
  oam.x = x - 32;
  oam.y = y + 224;
  Dma_Set(&oam, (void *)((u32)OAM + ((u32)tile >> 4)), DMA_COPY_WORDS(2),
          REG_DMA3);
}

// Where the left, middle and right logo pieces come to rest.
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

extern s16 gTitleIntro_GullFrames[];
extern s16 gTitleIntro_GullPath[];

#define LOAD_LOGO_TILES(id)                                                    \
  {                                                                            \
    u8 *buffer = Data_02026800;                                                \
    Resource_DecodeByteLzInRam(Resource_GetTableEntry((s32)&(id)), buffer);    \
    Dma_Set(buffer, (void *)OBJ_VRAM0, DMA_COPY_WORDS(3072), REG_DMA3);        \
  }

/// The logo's per-frame task while the lighthouse scrolls: places its pieces
/// and loads their next tiles as its timer runs.
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
  if (logo->phase == 0) {
    if (logo->timer == 0) {
      if (gTitleIntro_GullFrames[logo->frame + 5] != -1)
        logo->frame += 5;
      logo->timer = gTitleIntro_GullFrames[logo->frame + 2];
    } else {
      logo->timer--;
    }
    if (logo->delay == 0) {
      logo->step += 3;
      logo->delay = gTitleIntro_GullPath[logo->step + 2];
    } else {
      logo->delay--;
    }
    x = gTitleIntro_GullPath[logo->step];
    if (x == -1) {
      Dma_Set(Resource_GetTableEntry((s32)&ResourceId_Data01B),
              (void *)OBJ_PLTT,
              (DMA_ENABLE | DMA_32BIT) << 16 | (BG_PLTT_SIZE / 4), REG_DMA3);
      LOAD_LOGO_TILES(ResourceId_Data01C);
      logo->phase = 1;
      logo->frame = 1;
      logo->timer = 0;
      logo->step = 160;
      logo->delay = 0;
      REG_BLDCNT = BLDCNT_TGT1_OBJ | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_BG1;
      REG_BLDALPHA = BLEND_ALPHA(1, BLEND_WEIGHT_MAX);
      return;
    }
    tile = gTitleIntro_GullFrames[logo->frame];
    rows = gTitleIntro_GullFrames[logo->frame + 1];
    offsetX = gTitleIntro_GullFrames[logo->frame + 3];
    offsetY = gTitleIntro_GullFrames[logo->frame + 4];
    left = x + offsetX - 28;
    top = gTitleIntro_GullPath[logo->step + 1] + offsetY;
    for (row = 0; row < rows; row++) {
      for (column = 0; column < 8; column++) {
        Dma_Fill32Cell(fill, 0, &sprite, sizeof(sprite));
        sprite.tile = tile + column + row * 8;
        sprite.x = left + column * 8;
        sprite.y = top + row * 8;
        Dma_Set(&sprite, (u8 *)OAM + (row * 8 + column) * 8, DMA_COPY_WORDS(2),
                REG_DMA3);
      }
    }
  } else if (logo->phase == 1) {
    pieceX = (logo->step - 256) / 4 + LOGO_LEFT_X;
    y = (logo->step - 256) / 32 + 50;
    Title_PlaceLogoPiece(0, pieceX, y);
    Title_PlaceLogoPiece(128, LOGO_MIDDLE_X, y);
    pieceX = LOGO_RIGHT_X - (logo->step - 256) / 4;
    Title_PlaceLogoPiece(256, pieceX, y);
    matrix = (volatile u16 *)((u8 *)OAM + 6);
    *matrix = logo->step;
    matrix += 4;
    *matrix = 0;
    matrix += 4;
    *matrix = 0;
    matrix[4] = logo->step;
    if (logo->step < 256) {
      logo->step += 2;
      if (logo->step > 256)
        logo->step = 256;
    }
    if (logo->frame < BLEND_WEIGHT_MAX) {
      logo->timer++;
      if (logo->timer % 3 == 0) {
        logo->frame++;
        REG_BLDALPHA = BLEND_ALPHA(logo->frame, BLEND_WEIGHT_MAX);
      }
      if (logo->timer == 8)
        LOAD_LOGO_TILES(ResourceId_Data01D)
      else if (logo->timer == 16)
        LOAD_LOGO_TILES(ResourceId_Data01E)
      else if (logo->timer == 24)
        LOAD_LOGO_TILES(ResourceId_Data01F)
    }
  }
}

extern u8 gMapGridCellBuffer[];

/// Shows the lighthouse picture with its gulls
/// (ResourceId_IntroLighthousePicture, ResourceId_IntroGullSprites), scrolls it
/// with TitleIntro_ScrollLighthouse while TitleIntro_UpdateLogo runs, and
/// returns 0 when a key cuts it short.
s32 TitleIntro_ShowLighthouse(void) {
  struct TitleScroll scroll;
  volatile u32 fill;
  struct TitleLogoState *logo;
  u8 *source;
  u16 *map;
  s16 tile;
  s32 q16One;
  s32 number;
  s32 row;
  s32 i;
  u8 *buffer;

  logo = HeapSlot_Alloc(HEAP_SLOT_TITLE_INTRO, 112);
  REG_DISPCNT = 0;
  Dma_Fill32Cell(fill, 0, (void *)OAM, OAM_SIZE);
  source = Resource_GetTableEntry((s32)&ResourceId_IntroGullSprites);
  buffer = gMapGridCellBuffer;
  source += 32;
  Resource_DecodeByteLzInRam(source, buffer);
  Dma_Set(buffer, (void *)OBJ_VRAM0, DMA_COPY_WORDS(6912), REG_DMA3);
  wait(1);
  source = Resource_GetTableEntry((s32)&ResourceId_IntroLighthousePicture);
  Dma_Set(source, (void *)BG_PLTT,
          (DMA_ENABLE | DMA_32BIT) << 16 | (BG_PLTT_SIZE / 4), REG_DMA3);
  *(u16 *)BG_PLTT = RGB_WHITE;
  wait(1);
  source += BG_PLTT_SIZE;
  Resource_DecodeByteLzInRam(source, buffer);
  Dma_Set(buffer, (void *)BG_CHAR_BLOCK(0), DMA_COPY_WORDS(9600), REG_DMA3);
  wait(1);

  map = (u16 *)BG_SCREEN_BLOCK(31);
  tile = 0;
  q16One = 0x10000;
  for (row = 0; row < DISPLAY_TILE_HEIGHT; row++) {
    for (i = DISPLAY_TILE_WIDTH - 1; i >= 0; i--) {
      /* FAKEMATCH(temporary): the old tile kept in number, the step spelt on
       * the high halfword. */
      number = tile;
      tile = ((tile << 16) + q16One) >> 16;
      *map = number;
      map++;
    }
    *map++ = LIGHTHOUSE_BLANK_TILE;
    *map++ = LIGHTHOUSE_BLANK_TILE;
  }
  REG_BG1CNT = BGCNT_PRIORITY(3) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(31);
  REG_BLDCNT = 0;
  REG_BLDALPHA = 0;
  for (i = 0; i < 4; i++) {
    gBgScroll[i].y = 0;
    gBgScroll[i].x = 0;
  }
  Dma_Set((void *)BG_PLTT, Ram_HeapSlots->paletteFade,
          (DMA_ENABLE | DMA_32BIT) << 16 | (BG_PLTT_SIZE / 4), REG_DMA3);
  wait(1);
  TitlePaletteFade_SetTarget(q16One, PALETTES_ALL);
  REG_DISPCNT = DISPCNT_OBJ_1D_MAP | DISPCNT_BG1_ON | DISPCNT_OBJ_ON;
  TitlePaletteFade_Start(10);

  scroll.pos = 0;
  scroll.limit = 176;
  scroll.step = i;
  scroll.src = Data_02019600;
  scroll.dst = (u8 *)0x06009600;
  scroll.cursor = map;
  scroll.tile = tile;
  scroll.scroll = 0;
  scroll.rows = row;
  scroll.frames = 1;
  logo->white = 10;
  logo->back = 25;
  if (TitleIntro_ScrollLighthouse(&scroll))
    goto stopped;
  scroll.limit = 200;
  scroll.step = 3;
  if (TitleIntro_ScrollLighthouse(&scroll))
    goto stopped;
  scroll.limit = 216;
  scroll.step = 2;
  map = (u16 *)TitleIntro_ScrollLighthouse(&scroll);
  if (map)
    goto stopped;
  Dma_Set(Resource_GetTableEntry((s32)&ResourceId_IntroGullSprites),
          (void *)OBJ_PLTT, DMA_COPY_WORDS(8), REG_DMA3);
  logo->phase = 0;
  logo->frame = -5;
  logo->timer = 0;
  logo->step = -3;
  logo->delay = 0;
  scroll.step = 1;
  scroll.limit = 224;
  i = TitleIntro_ScrollLighthouse(&scroll);
  if (i)
    goto stopped;
  Task_Add(TitleIntro_UpdateLogo, 0x480);
  scroll.limit = 160;
  scroll.step = -1;
  scroll.scroll = 0;
  scroll.src = Data_02017080;
  scroll.dst = (u8 *)BG_SCREEN_BLOCK(15);
  scroll.cursor = (u16 *)0x0600f9c2;
  scroll.tile = 450;
  scroll.rows = 11;
  scroll.frames = 8;
  i = TitleIntro_ScrollLighthouse(&scroll);
  if (i)
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

// The title screen's "Press Start" prompt: five sprites further left in the
// French and Japanese editions; two rows of three, from eight tile blocks, in
// German; one row of three elsewhere.
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
#define PROMPT_TILE_BLOCKS 8
#else
#define PROMPT_TILE_BLOCKS 5
#endif

/// Runs the intro's pictures in turn (TitleIntro_ScrollPicture's steps
/// 0 to 2 between the pictures of TitleIntro_ShowCard and
/// TitleIntro_ShowBlendedCard), fading out early when a key cuts one short and
/// running TitleIntro_ShowLighthouse when all finish, then restores the display
/// and the CPU-load flag it saved.
s32 TitleIntro_Run(void) {
  /* FAKEMATCH(volatile): the saved flag lives in a stack byte across the calls.
   */
  volatile u8 cpuLoad;
  s32 result;
  /* FAKEMATCH(temporary): the flag is read before the stack byte's address is
   * formed. */
  u8 saved = gCpuLoadDisplayEnabled;

  cpuLoad = saved;
  DebugText_Clear();
  VramCache_Reset();
  wait(1);
  Task_ResetAll();
  gOamCopyEnabled = 0;
  gCpuLoadDisplayEnabled = 0;
  TitleIntro_LoadPictureTiles();
  if ((result = TitleIntro_ScrollPicture(0)) == 0 ||
      (result = TitleIntro_ShowCard()) == 0 ||
      (result = TitleIntro_ScrollPicture(1)) == 0 ||
      (result = TitleIntro_ShowBlendedCard()) == 0 ||
      (result = TitleIntro_ScrollPicture(2)) == 0) {
    TitlePaletteFade_SetTarget(RGB(0, 0, 0), PALETTES_ALL);
    TitlePaletteFade_Start(10);
    wait(10);
  }
  if (result)
    TitleIntro_ShowLighthouse();
  gOamCopyEnabled = 1;
  wait(1);
  // The result's register counts the backgrounds (a separate counter takes
  // other registers).
  for (result = 0; result < 4; result++)
    gBgScroll[result].x = gBgScroll[result].y = 0;
  REG_DISPCNT = 0;
  Window_LoadGraphics();
  DebugText_Clear();
  TitlePaletteFade_Initialize();
  gCpuLoadDisplayEnabled = cpuLoad;
  return 1;
}

// The Japanese title screen, the older revision, blinks a five-sprite prompt
// (at the French position) from the first frame, without the 120-frame wait;
// the German one, laid out for its longer prompt, loads eight prompt tiles and
// draws two rows of three sprites from y 116.

/// Each logo and title resource begins with its 224 colours.
#define LOGO_PALETTE_BYTES (224 * 2)

/// The title screen: fades to white, shows Title_ScreenPicture on BG2 (its
/// map numbered from tile 0x1a0, two blank columns a row) and, with prompt
/// set, loads the five prompt sprites. After the fade from white it holds 660
/// frames, or 5400 with the prompt; after the first 120 the prompt's three
/// sprites blink by gTitle_PromptBlendLevels's levels, and A or Start ends it
/// early, returning 1 (else 0).
s32 Title_ShowIntro(s32 prompt) {
  s32 blankTile = 511;
  s32 result = 0;
  struct TitleScreenWork *work = HeapSlot_AllocEwram(
      HEAP_SLOT_TITLE_INTRO, sizeof(struct TitleScreenWork));
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
  REG_BG2CNT = BGCNT_PRIORITY(1) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(6);
  REG_DISPCNT = DISPCNT_OBJ_1D_MAP | DISPCNT_BG2_ON | DISPCNT_OBJ_ON;
  gBgScroll[2].y = result;
  data = Resource_GetTableEntry((s32)&ResourceId_TitleScreenPicture);
  Dma_Set(data, (void *)BG_PLTT, DMA_COPY_WORDS(LOGO_PALETTE_BYTES / 4),
          REG_DMA3);
  Decompress_TaggedLz(data + LOGO_PALETTE_BYTES, Ram_MapCellBuffer);
  Dma_Set(Ram_MapCellBuffer, (void *)BG_SCREEN_BLOCK(13), DMA_COPY_WORDS(9600),
          REG_DMA3);
  tile = 416;
  tilemap = (u16 *)BG_SCREEN_BLOCK(6);
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
    u8 *buffer = HeapSlot_AllocEwram(HEAP_SLOT_TILE_SCRATCH, 0x400);
    struct TitleSprite *sprite = work->sprites;

    Resource_DecodeByteLzInRam(gTitle_PromptTiles, buffer);
    for (i = 0; i < PROMPT_TILE_BLOCKS; i++) {
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
    holdFrames = 90 * FRAMES_PER_SECOND;
  else
    holdFrames = 11 * FRAMES_PER_SECOND;
  for (i = 0; i < holdFrames; i++) {
    // The Japanese edition blinks the prompt from the first frame.
    if (!TLA_PATCH_1 || i > 120) {
      if (prompt != 0) {
        struct TitleSprite *sprite = work->sprites;
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
        level = gTitle_PromptBlendLevels[i % 60];
        REG_BLDCNT = BLDCNT_TGT1_OBJ | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_BG0 |
                     BLDCNT_TGT2_BG1 | BLDCNT_TGT2_BG2 | BLDCNT_TGT2_BG3 |
                     BLDCNT_TGT2_BD;
        REG_BLDALPHA = (BLEND_WEIGHT_MAX - level) * 256 + level;
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

/// An empty entry the far-call table keeps.
void Title_NoOpPalette(void) {}

/// Shows the Nintendo logo with its jingle. With mode 0 it fades in and holds
/// two seconds; otherwise it waits a second, fades in, holds three seconds and
/// fades out, faster once skipped. Returns -1 when A or Start skipped it.
s32 Title_ShowNintendoLogo(s32 mode) {
  s32 result;
  u8 *data;
  u16 *map;
  u32 x;
  u32 y;
  u16 tile;
  s32 blank = 511;
  s32 resource;

  Sound_PlayFar(SE_CHIME);
  gOamCopyEnabled = 1;
  resource = (s32)&ResourceId_NintendoLogo;
  Task_ResetAll();
  ScreenFade_ToBlack(1);
  DebugText_Clear();
  wait(1);
  REG_BG2CNT = BGCNT_PRIORITY(1) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(6);
  REG_DISPCNT = DISPCNT_OBJ_1D_MAP | DISPCNT_BG2_ON | DISPCNT_OBJ_ON;
  result = 0;
  gBgScroll[2].y = result;
  data = Resource_GetTableEntry(resource);
  Dma_Set(data, (void *)BG_PLTT, DMA_COPY_WORDS(LOGO_PALETTE_BYTES / 4),
          REG_DMA3);
  Decompress_TaggedLz(data + LOGO_PALETTE_BYTES, Ram_MapCellBuffer);
  Dma_Set(Ram_MapCellBuffer, (void *)BG_CHAR_BLOCK(1), DMA_COPY_WORDS(9600),
          REG_DMA3);
  // Pad the 30-tile picture to the hardware map's 32-tile row width.
  tile = 256;
  map = (u16 *)BG_SCREEN_BLOCK(6);
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

/// As The Broken Seal's: the same resource, BG2 set-up, DMA copies, 120-frame
/// tile cycle and A or Start exit.
s32 Title_ShowCamelotLogo(void) {
  s32 result;
  u32 i;
  u8 *buffer;
  s32 resource;

  gOamCopyEnabled = 1;
  result = 0;
  resource = (s32)&ResourceId_CamelotLogo;
  Task_ResetAll();
  ScreenFade_ToBlack(1);
  DebugText_Clear();
  wait(1);
  REG_BG2CNT = BGCNT_PRIORITY(1) | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS |
               BGCNT_SCREEN_BASE(6);
  REG_DISPCNT = DISPCNT_OBJ_1D_MAP | DISPCNT_BG2_ON | DISPCNT_OBJ_ON;
  gBgScroll[2].y = result;
  buffer = Resource_GetTableEntry(resource);
  Decompress_TaggedLz(buffer, Ram_MapCellBuffer);
  buffer = Ram_MapCellBuffer;
  Dma_Set(buffer, (void *)BG_PLTT, DMA_COPY_WORDS(LOGO_PALETTE_BYTES / 4),
          REG_DMA3);
  buffer += LOGO_PALETTE_BYTES;
  Dma_Set(buffer, (void *)BG_SCREEN_BLOCK(6),
          DMA_COPY_WORDS(BG_SCREEN_SIZE / 4), REG_DMA3);
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
    Dma_Set(((gFrameCount >> 3) & 3) * 1024 + buffer, BG_CHAR_BLOCK(1) + 256,
            DMA_COPY_WORDS(832 / 4), REG_DMA3);
    if (gInput.pressed & (KEY_A | KEY_START))
      break;
    wait(1);
  }
  return result;
}

/// Unused: an empty routine in the far-call table.
void Data_081a786c(void) {}

/// Writes to step, for each of the 1536 RGB components of 512 colours, the
/// difference between current and target values divided by frames.
/// src: current components.
/// dst: target components.
/// step: destination for component increments.
/// frames: fade duration; nothing is written if zero or negative.
void TitlePaletteFade_CalculateSteps(s16 *src, s16 *dst, s16 *step,
                                     s32 frames) {
  s32 n;
  s32 from;
  s32 to;

  if (frames > 0) {
    for (n = 0; n < 3 * 512; n++) {
      from = *src;
      to = *dst;
      *step = Iwram_SignedDivide(to - from, frames);
      src++;
      dst++;
      step++;
    }
  }
}

/// Steps the palette fade on heap slot 0x80 by one frame: adds each channel's
/// step until the last frame, which copies the target instead, packs the
/// channels into the hidden page of RGB555 colours and queues that page for the
/// BG and OBJ palettes.
void TitlePaletteFade_Update(void) {
  struct TitlePaletteFade *work = Ram_HeapSlots->paletteFade;
  u16 *delta = work->delta;
  u16 *current;
  u16 *packed;
  s32 i;
  u16 *bank;
  s32 blue;

  if (work->duration == 0)
    return;
  if ((s8)++work->step < work->duration) {
    current = work->current;
    for (i = 0; i <= TITLE_PALETTE_FADE_CHANNELS - 1; i++)
      *current++ += *delta++;
  } else {
    Dma_Set(work->target, work->current,
            DMA_COPY_WORDS(sizeof(work->current) / 4), REG_DMA3);
    work->duration = 0;
  }
  // The queued DMA keeps a pointer until VBlank.
  packed = work->packed[work->page ^ 1];
  blue = 0x7c00;
  i = 512;
  current = work->current;
  for (; i != 0; i--) {
    *packed++ = (current[0] & blue) | (((s16)current[1] >> 5) & 0x3e0) |
                (((s16)current[2] >> 10) & 0x1f);
    current += 3;
  }
  work->page ^= 1;
  bank = work->packed[work->page];
  IO_WRITE_QUEUE_PUSH((u32)BG_PLTT, (u32)bank,
                      DMA_COPY_WORDS(BG_PLTT_SIZE / 4));
  IO_WRITE_QUEUE_PUSH((u32)OBJ_PLTT, (u32)(bank + BG_PLTT_COLORS),
                      DMA_COPY_WORDS(BG_PLTT_SIZE / 4));
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
  u32 colorCount = 512;
  s32 red;
  s32 blue;
  u32 colorWork;
  s32 green;
  s32 tintedRed;

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
        red = RGB_RED(colorWork);
        green = RGB_GREEN(colorWork);
        blue = RGB_BLUE(colorWork);
        colorWork = Iwram_SignedDivide(red + green + blue, 10);
        red = colorWork * 4 + 5;
        blue = colorWork * 3 + 5;
        green = colorWork * 3 + 5;
        if (red < 8)
          red = 8;
        if (green < 8)
          green = 8;
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
        green = RGB_GREEN(colorWork);
        red = RGB_RED(colorWork);
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
      tintedRed = colorWork * tintRed;
      green = colorWork * tintGreen;
      blue = colorWork * tintBlue;
      tintedRed = TitlePaletteFade_ClampShiftedComponent(tintedRed);
      green = TitlePaletteFade_ClampShiftedComponent(green);
      blue = TitlePaletteFade_ClampShiftedComponent(blue);
      destination[colorIndex * 3 + 0] = blue;
      destination[colorIndex * 3 + 1] = green;
      destination[colorIndex * 3 + 2] = tintedRed;
    }
  } else if (mode & PALETTE_TINT_BLEND) {
    u32 tintRed;
    u32 tintGreen;
    u32 tintBlue;
    s32 scaledGreen;
    s32 scaledBlue;
    s32 clampedRed;
    s32 clampedGreen;
    s32 clampedBlue;

    tintRed = RGB_RED(mode);
    tintGreen = RGB_GREEN(mode);
    tintBlue = RGB_BLUE(mode);
    for (colorIndex = 0; colorIndex < colorCount; colorIndex++) {
      colorWork = *source++;
      red = RGB_RED(colorWork);
      green = RGB_GREEN(colorWork);
      blue = RGB_BLUE(colorWork);
      colorWork = Iwram_SignedDivide((red + green + blue) << 4,
                                     tintRed + tintGreen + tintBlue);
      tintedRed = Iwram_MulQ16(((colorWork * tintRed) >> 4) << 16,
                               (s32)(tintRed << 16) >> 4);
      scaledGreen = Iwram_MulQ16(((colorWork * tintGreen) >> 4) << 16,
                                 (s32)(tintGreen << 16) >> 4);
      scaledBlue = Iwram_MulQ16(((colorWork * tintBlue) >> 4) << 16,
                                (s32)(tintBlue << 16) >> 4);
      tintedRed = (u32)tintedRed >> 16;
      clampedRed = TitlePaletteFade_ClampComponent(tintedRed);
      scaledGreen = (u32)scaledGreen >> 16;
      clampedGreen = TitlePaletteFade_ClampComponent(scaledGreen);
      scaledBlue = (u32)scaledBlue >> 16;
      clampedBlue = TitlePaletteFade_ClampComponent(scaledBlue);
      destination[colorIndex * 3 + 0] = gTitleIntro_ChannelCurve[clampedBlue];
      destination[colorIndex * 3 + 1] = gTitleIntro_ChannelCurve[clampedGreen];
      destination[colorIndex * 3 + 2] = gTitleIntro_ChannelCurve[clampedRed];
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
      mode += 256 * 3 * sizeof(u16); // the sprite colours' components
    Dma_Set((const void *)mode, destination,
            DMA_COPY_WORDS((colorCount * 6) >> 2), REG_DMA3);
  }
}

/// Allocates the palette fade, saves both hardware palettes in it as the
/// starting colours and adds the task TitlePaletteFade_Update.
void TitlePaletteFade_Initialize(void) {
  struct TitlePaletteFade *buffer;
  s32 operation;

  buffer = HeapSlot_AllocEwram(HEAP_SLOT_PALETTE_FADE, sizeof(*buffer));
  Dma_Fill32(0, buffer, sizeof(*buffer));
  Dma_Set((void *)BG_PLTT, buffer, DMA_COPY_WORDS(128), REG_DMA3);
  Dma_Set((void *)OBJ_PLTT, &buffer->palette[256], DMA_COPY_WORDS(128),
          REG_DMA3);
  TitlePaletteFade_BuildColors(PALETTE_TINT_NONE, buffer->palette,
                               buffer->target, PALETTES_ALL);
  operation = TASK_PASS_VBLANK;
  Task_Add((s32)(TitlePaletteFade_Update), operation);
}

/// Removes the palette fade's task and frees its buffers.
void TitlePaletteFade_Release(void) {
  Task_Remove((u32)((s32)&TitlePaletteFade_Update));
  HeapSlot_Release(HEAP_SLOT_PALETTE_FADE);
}

/// Sets the fade target by tinting the saved background and/or sprite palettes
/// with the requested tint.
void TitlePaletteFade_SetTarget(s32 mode, s32 selectedPalettes) {
  struct TitlePaletteFade *fade = Ram_HeapSlots->paletteFade;

  if (fade != NULL)
    TitlePaletteFade_BuildColors(mode, fade->palette, fade->target,
                                 selectedPalettes);
}

/// Sets the colours shown now by tinting the saved palettes.
void TitlePaletteFade_SetCurrent(s32 mode, s32 selectedPalettes) {
  struct TitlePaletteFade *fade = Ram_HeapSlots->paletteFade;

  if (fade != NULL)
    TitlePaletteFade_BuildColors(mode, (u16 *)fade, fade->current,
                                 selectedPalettes);
}

/// Replaces the first saved colour, the backdrop.
void TitlePaletteFade_SetBackdrop(s32 color) {
  struct TitlePaletteFade *fade = Ram_HeapSlots->paletteFade;

  if (fade != NULL)
    fade->palette[0] = color;
}

/// Starts fading the shown colours to the target over the requested frames:
/// each of the 1,536 components (three for each of the 512 colours) steps by
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
