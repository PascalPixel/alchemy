/// @file
/// The Title's intro: the Nintendo and Camelot splash screens, the scroll down
/// the intro's tall picture, the logo coming together, the title screen and its
/// prompt, and the palette fades they share.
///
/// One object, as The Broken Seal's title_intro.c is. Its logo frames and
/// palettes are the data objects logo.c and title_intro_tables.s.

#include "types.h"
#include "graphics.h"
#include "system.h"
#include "edition.h"
#include "version.h"
#include "macros.h"
#include "dma.h"
#include "io_reg.h"
#include "io_write_queue.h"
#include "iwram_call.h"
#include "heap_release.h"
#include "resource.h"
#include "ram_buffer.h"
#include "frame.h"
#include "input.h"
#include "task.h"
#include "scene.h"
#include "bg_scroll.h"
#include "palette_buffers.h"
#include "actor.h"
#include "module/menu/menu.h"
#include "module/system/system.h"
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
void Func_081a6030(void) {
  u8 *source;
  u8 *cells;

  REG_DISPCNT = 0;
  source = Resource_GetTableEntry((s32)&ResourceId_Data018);
  Dma_Set(source, (void *)BG_PLTT, DMA_COPY_WORDS(0x200 / 4), REG_DMA3);
  BG_PLTT[0] = 0;
  cells = Ram_MapCellBuffer;
  Resource_DecodeByteLzInRam(source + 0x200, cells);
  Dma_Set(cells, (void *)0x06000000, DMA_COPY_WORDS(DATA018_TILE_BYTES / 4),
          REG_DMA3);
}

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

#define TITLE_STEP(f, s0, s1, s2, s3, m0, m1, m2, m3)                          \
  (step.frames = (f), step.speed[0] = (s0), step.speed[1] = (s1),              \
   step.speed[2] = (s2), step.speed[3] = (s3), step.mask[0] = (m0),            \
   step.mask[1] = (m1), step.mask[2] = (m2), step.mask[3] = (m3),              \
   Title_RunScrollStep(&step))

s32 Func_081a6154(s32 mode) {
  struct TitleScrollStep step;
  u16 *map;
  u16 tile;
  s32 i;
  s32 j;

  REG_DISPCNT = 0;
  map = (u16 *)0x0600f800;
  tile = 0;
  for (j = 0; j < 16; j++) {
    for (i = 0; i < 32; i++) {
      *map++ = tile++;
    }
  }
  tile = 0;
  for (; j < 32; j++) {
    for (i = 0; i < 32; i++) {
      *map++ = tile++;
    }
  }
  map = (u16 *)0x0600f000;
  tile = 0;
  for (j = 0; j < 16; j++) {
    for (i = 0; i < 32; i++) {
      *map++ = tile++ | 0x1000;
    }
  }
  tile = 0;
  for (; j < 32; j++) {
    for (i = 0; i < 32; i++) {
      *map++ = tile++ | 0x1000;
    }
  }
  REG_BG1CNT = 0x1f02;
  REG_BG2CNT = 0x1e05;
  REG_BLDCNT = 0x244;
  REG_BLDALPHA = 0xa0b;
  for (i = 0; i < 4; i++) {
    gBgScroll[i].y = 0;
    gBgScroll[i].x = 0;
  }
  if (mode == 0) {
    PaletteFade_Initialize();
  }
  PaletteFade_SetSource(2, 0);
  PaletteFade_SetTarget(0x10000, 0);
  REG_DISPCNT = 0x600;
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
    PaletteFade_SetTarget(2, 0);
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
    PaletteFade_SetTarget(2, 0);
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
    PaletteFade_SetTarget(0x7fff, 0);
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

s32 Func_081a6638(void) {
  u16 *map;
  u16 tile;
  s32 i;
  s32 row;

  REG_DISPCNT = 0;
  map = (u16 *)0x0600e800;
  tile = 0;
  for (row = 0; row < 20; row++) {
    if (row >= 9 && row < 9 + NUMBERED_ROWS) {
      for (i = 0; i < 32; i++) {
        *map++ = tile++ | 0x2000;
      }
    } else {
      for (i = 0; i < 32; i++)
        *map++ = 0x2000;
    }
  }
  REG_BG3CNT = 0x1d0a;
  REG_BLDCNT = 0;
  REG_BLDALPHA = 0;
  for (i = 0; i < 4; i++)
    gBgScroll[i].x = gBgScroll[i].y = 0;

  PaletteFade_SetSource(0, 0);
  PaletteFade_SetTarget(0x10000, 0);
  REG_DISPCNT = 0x800;
  TitlePaletteFade_Start(40);
  if (Title_WaitOrPress(150))
    return 0;
  PaletteFade_SetTarget(2, 0);
  TitlePaletteFade_Start(40);
  if (Title_WaitOrPress(60))
    return 0;
  return 1;
}

// This picture's tiles follow Func_081a6638's, 32 for each row it numbers.
#if LANGUAGE == LANGUAGE_GERMAN || LANGUAGE == LANGUAGE_SPANISH ||             \
    LANGUAGE == LANGUAGE_ITALIAN
#define FIRST_TILE 96
#else
#define FIRST_TILE 64
#endif

s32 Func_081a6724(void) {
  u16 *map;
  u16 tile;
  s32 i;
  s32 row;

  REG_DISPCNT = 0;
  map = (u16 *)0x0600e800;
  tile = FIRST_TILE;
  for (row = 0; row < 20; row++) {
    if (row >= 7 && row <= 9) {
      for (i = 0; i < 32; i++) {
        *map++ = tile++ | 0x3000;
      }
    } else if (row == 10 || row == 11) {
      for (i = 0; i < 32; i++)
        *map++ = 0x3060;
    } else {
      for (i = 0; i < 32; i++)
        *map++ = 0x3040;
    }
  }
  map = (u16 *)0x0600e000;
  tile = FIRST_TILE + 96;
  for (row = 0; row < 20; row++) {
    if (row == 10 || row == 11) {
      for (i = 0; i < 32; i++) {
        *map++ = tile++ | 0x3000;
      }
    } else {
      for (i = 0; i < 32; i++)
        *map++ = 0x3040;
    }
  }
  REG_BG3CNT = 0x1d0a;
  REG_BG0CNT = 0x1c09;
  REG_BLDCNT = 0x0841;
  REG_BLDALPHA = 0x1000;
  for (i = 0; i < 4; i++)
    gBgScroll[i].x = gBgScroll[i].y = 0;

  PaletteFade_SetSource(0, 0);
  PaletteFade_SetTarget(0x10000, 0);
  REG_DISPCNT = 0x900;
  TitlePaletteFade_Start(40);
  if (Title_WaitOrPress(60))
    return 0;
  for (i = 1; i <= 16; i++) {
    REG_BLDALPHA = i | 0x1000;
    if (Title_WaitOrPress(3))
      return 0;
  }
  if (Title_WaitOrPress(150))
    return 0;
  PaletteFade_SetTarget(2, 0);
  TitlePaletteFade_Start(40);
  if (Title_WaitOrPress(60))
    return 0;
  return 1;
}

/// Scrolls the intro's tall picture from one position to another: each step
/// moves it by the scroll's step and waits its frames, streams in the next map
/// rows' tiles when scrolling down and takes them back when scrolling up, and
/// starts the fade to white or back when its countdown reaches 1. A key press
/// cuts the scroll short.
s32 Func_081a68b4(struct TitleScroll *s) {
  struct TitleFadeTimers *timers = Ram_HeapSlots->unk84[10];
  s32 i;
  s32 j;
  s32 step;

  step = s->step;
  if ((s16)step > 0) {
    for (i = s->pos; i < s->limit; i += (s16)s->step) {
      if (Title_WaitOrPress(s->frames))
        goto fail;
      if (s->scroll > 0) {
        Dma_Set(s->src, s->dst, 0x840001e0, REG_DMA3);
        s->src += 0x780;
        s->dst += 0x780;
        for (j = 0; j < 30; j++)
          *s->cursor++ = s->tile++;
        *s->cursor++ = 0x1ff;
        *s->cursor++ = 0x1ff;
        if (++s->rows == 32) {
          s->dst = (u8 *)0x06000000;
          s->rows = 0;
          s->tile = 0;
          s->cursor = (u16 *)0x0600f800;
        }
        s->scroll -= 8;
      }
      s->scroll += s->step;
      gBgScroll[1].y += s->step;
      if (timers->white != 0 && --timers->white == 1) {
        PaletteFade_SetTarget(0x7fff, 0);
        TitlePaletteFade_Start(10);
      }
      if (timers->back != 0 && --timers->back == 1) {
        PaletteFade_SetTarget(0x10000, 0);
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
          Dma_Set(s->src, s->dst, 0x840001e0, REG_DMA3);
          s->src -= 0x780;
          s->dst -= 0x780;
          for (j = 0; j < 30; j++)
            *s->cursor++ = s->tile++;
          s->cursor -= 30;
          s->tile -= 30;
        }
        s->scroll -= 8;
      }
      if (Title_WaitOrPress(s->frames))
        goto fail;
      // Read through a volatile cast: keeps the ldrh, its copy and the sign
      // extension apart, where a plain read folds them into one ldrsh.
      step = *(volatile u16 *)&s->step;
    }
  }
done:
  s->pos = i;
  return 0;
}

void Title_PlaceLogoPiece(s32 tile, s32 x, u32 y) {
  struct OamEntry oam;

  Dma_Fill32(0, &oam, sizeof(oam));
  oam.shape = 0;
  oam.size = 3;
  oam.colors256 = 1;
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

// Declared here without const, as Camelot's code read them: a const table's
// loads are scheduled across the stores before them (4 of 360 differ with
// either const, 6 with both). menu/logo.c defines both, const, in
// ROM.
extern s16 gData_081a8378[];
extern s16 gData_081a84e6[];

#define LOAD_LOGO_TILES(id)                                                    \
  {                                                                            \
    u8 *buffer = Data_02026800;                                                \
    Resource_DecodeByteLzInRam(Resource_GetTableEntry((s32)&(id)), buffer);    \
    Dma_Set(buffer, (void *)0x06010000,                                        \
            (DMA_ENABLE | DMA_32BIT) << 16 | 0xc00, REG_DMA3);                 \
  }

void Func_081a6b50(void) {
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

  logo = Ram_HeapSlots->unk84[10];
  Dma_Fill32Cell(fill, 0, (void *)0x07000000, 0x400);
  if (logo->phase == 0) {
    if (logo->timer == 0) {
      if (gData_081a8378[logo->frame + 5] != -1)
        logo->frame += 5;
      logo->timer = gData_081a8378[logo->frame + 2];
    } else {
      logo->timer--;
    }
    if (logo->delay == 0) {
      logo->step += 3;
      logo->delay = gData_081a84e6[logo->step + 2];
    } else {
      logo->delay--;
    }
    x = gData_081a84e6[logo->step];
    if (x == -1) {
      Dma_Set(Resource_GetTableEntry((s32)&ResourceId_Data01B),
              (void *)0x05000200, (DMA_ENABLE | DMA_32BIT) << 16 | 0x80,
              REG_DMA3);
      LOAD_LOGO_TILES(ResourceId_Data01C);
      logo->phase = 1;
      logo->frame = 1;
      logo->timer = 0;
      logo->step = 160;
      logo->delay = 0;
      REG_BLDCNT = 0x250;
      REG_BLDALPHA = 0x1001;
      return;
    }
    tile = gData_081a8378[logo->frame];
    rows = gData_081a8378[logo->frame + 1];
    offsetX = gData_081a8378[logo->frame + 3];
    offsetY = gData_081a8378[logo->frame + 4];
    left = x + offsetX - 28;
    top = gData_081a84e6[logo->step + 1] + offsetY;
    for (row = 0; row < rows; row++) {
      for (column = 0; column < 8; column++) {
        Dma_Fill32Cell(fill, 0, &sprite, sizeof(sprite));
        sprite.tile = tile + column + row * 8;
        sprite.x = left + column * 8;
        sprite.y = top + row * 8;
        Dma_Set(&sprite, (u8 *)0x07000000 + (row * 8 + column) * 8,
                (DMA_ENABLE | DMA_32BIT) << 16 | 2, REG_DMA3);
      }
    }
  } else if (logo->phase == 1) {
    pieceX = (logo->step - 256) / 4 + LOGO_LEFT_X;
    y = (logo->step - 256) / 32 + 50;
    Title_PlaceLogoPiece(0, pieceX, y);
    Title_PlaceLogoPiece(128, LOGO_MIDDLE_X, y);
    pieceX = LOGO_RIGHT_X - (logo->step - 256) / 4;
    Title_PlaceLogoPiece(256, pieceX, y);
    // Pointer steps and one index, as the listing schedules them: plain
    // indexes give 34 of 360, steps throughout 9.
    matrix = (volatile u16 *)0x07000006;
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
    if (logo->frame < 16) {
      logo->timer++;
      if (logo->timer % 3 == 0) {
        logo->frame++;
        REG_BLDALPHA = logo->frame | 0x1000;
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

extern u8 gMapCellBuffer[];

s32 Func_081a6ea0(void) {
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

  logo = HeapSlot_Alloc(172, 112);
  REG_DISPCNT = 0;
  Dma_Fill32Cell(fill, 0, (void *)0x07000000, 0x400);
  source = Resource_GetTableEntry((s32)&ResourceId_IntroGullSprites);
  buffer = gMapCellBuffer;
  source += 32;
  Resource_DecodeByteLzInRam(source, buffer);
  Dma_Set(buffer, (void *)0x06010000, (DMA_ENABLE | DMA_32BIT) << 16 | 0x1b00,
          REG_DMA3);
  wait(1);
  source = Resource_GetTableEntry((s32)&ResourceId_IntroLighthousePicture);
  Dma_Set(source, (void *)0x05000000, (DMA_ENABLE | DMA_32BIT) << 16 | 0x80,
          REG_DMA3);
  *(u16 *)0x05000000 = 0x7fff;
  wait(1);
  source += 0x200;
  Resource_DecodeByteLzInRam(source, buffer);
  Dma_Set(buffer, (void *)0x06000000, (DMA_ENABLE | DMA_32BIT) << 16 | 0x2580,
          REG_DMA3);
  wait(1);

  map = (u16 *)0x0600f800;
  tile = 0;
  q16One = 0x10000;
  for (row = 0; row < 20; row++) {
    for (i = 29; i >= 0; i--) {
      /* FAKEMATCH(temporary): the old tile kept in number, the step spelt on
       * the high halfword; as *map = tile; tile++, 97 of 325 instructions
       * differ in tla-en. */
      number = tile;
      tile = ((tile << 16) + q16One) >> 16;
      *map = number;
      map++;
    }
    *map++ = 0x1ff;
    *map++ = 0x1ff;
  }
  REG_BG1CNT = 0x1f83;
  REG_BLDCNT = 0;
  REG_BLDALPHA = 0;
  for (i = 0; i < 4; i++) {
    gBgScroll[i].y = 0;
    gBgScroll[i].x = 0;
  }
  Dma_Set((void *)0x05000000, Ram_HeapSlots->block32,
          (DMA_ENABLE | DMA_32BIT) << 16 | 0x80, REG_DMA3);
  wait(1);
  PaletteFade_SetTarget(q16One, 0);
  REG_DISPCNT = 0x1240;
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
  if (Func_081a68b4(&scroll))
    goto stopped;
  scroll.limit = 200;
  scroll.step = 3;
  if (Func_081a68b4(&scroll))
    goto stopped;
  scroll.limit = 216;
  scroll.step = 2;
  map = (u16 *)Func_081a68b4(&scroll);
  if (map)
    goto stopped;
  Dma_Set(Resource_GetTableEntry((s32)&ResourceId_IntroGullSprites),
          (void *)0x05000200, (DMA_ENABLE | DMA_32BIT) << 16 | 8, REG_DMA3);
  logo->phase = 0;
  logo->frame = -5;
  logo->timer = 0;
  logo->step = -3;
  logo->delay = 0;
  scroll.step = 1;
  scroll.limit = 224;
  i = Func_081a68b4(&scroll);
  if (i)
    goto stopped;
  Task_Add(Func_081a6b50, 0x480);
  scroll.limit = 160;
  scroll.step = -1;
  scroll.scroll = 0;
  scroll.src = Data_02017080;
  scroll.dst = (u8 *)0x06007800;
  scroll.cursor = (u16 *)0x0600f9c2;
  scroll.tile = 450;
  scroll.rows = 11;
  scroll.frames = 8;
  i = Func_081a68b4(&scroll);
  if (i)
    goto stopped;
  PaletteFade_SetTarget(0x7fff, 0);
  TitlePaletteFade_Start(20);
  scroll.limit = 0;
  scroll.step = -5;
  scroll.frames = 1;
  if (!Func_081a68b4(&scroll))
    Title_WaitOrPress(120);
  goto done;
stopped:
  PaletteFade_SetTarget(0, 0);
  TitlePaletteFade_Start(10);
  wait(10);
done:
  Dma_Fill32Cell(fill, 0, (void *)0x07000000, 0x400);
  Task_Remove(Func_081a6b50);
  HeapSlot_Release(172);
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

/// Runs the intro's pictures in turn (Func_081a6154's steps
/// 0 to 2 between the pictures of Func_081a6638 and Func_081a6724), fading
/// out early when a key cuts one short and running Func_081a6ea0 when all
/// finish, then restores the display and the CPU-load flag it saved.
s32 Func_081a71c8(void) {
  /* FAKEMATCH(volatile): the saved flag lives in a stack byte across the
   * calls; as a plain u8 local, 28 of 78 instructions differ in tla-en. */
  volatile u8 cpuLoad;
  s32 result;
  /* FAKEMATCH(temporary): the flag is read before the stack byte's address
   * is formed; as cpuLoad = gCpuLoadDisplayEnabled, 2 of 78 instructions
   * differ in tla-en. */
  u8 saved = gCpuLoadDisplayEnabled;

  cpuLoad = saved;
  DebugText_Clear();
  VramCache_Reset();
  wait(1);
  Task_ResetAll();
  gRenderOamEnabled = 0;
  gCpuLoadDisplayEnabled = 0;
  Func_081a6030();
  if ((result = Func_081a6154(0)) == 0 || (result = Func_081a6638()) == 0 ||
      (result = Func_081a6154(1)) == 0 || (result = Func_081a6724()) == 0 ||
      (result = Func_081a6154(2)) == 0) {
    PaletteFade_SetTarget(0, 0);
    TitlePaletteFade_Start(10);
    wait(10);
  }
  if (result)
    Func_081a6ea0();
  gRenderOamEnabled = 1;
  wait(1);
  // The result's register counts the backgrounds (a separate counter takes
  // other registers).
  for (result = 0; result < 4; result++)
    gBgScroll[result].x = gBgScroll[result].y = 0;
  REG_DISPCNT = 0;
  Window_LoadGraphics();
  DebugText_Clear();
  PaletteFade_Initialize();
  gCpuLoadDisplayEnabled = cpuLoad;
  return 1;
}

// The Japanese title screen, the older revision, blinks a five-sprite prompt
// (at the French position) from the first frame, without the 120-frame wait;
// the German one, laid out for its longer prompt, loads eight prompt tiles and
// draws two rows of three sprites from y 116.

/// The title screen: fades to white, shows Title_ScreenPicture on BG2 (its
/// map numbered from tile 0x1a0, two blank columns a row) and, with prompt
/// set, loads the five prompt sprites. After the fade from white it holds 660
/// frames, or 5400 with the prompt; after the first 120 the prompt's three
/// sprites blink by gTitle_PromptBlendLevels's levels, and A or Start ends it
/// early, returning 1 (else 0).
// As the end of The Broken Seal's Title_RunIntro.
s32 Title_ShowIntro(s32 prompt) {
  s32 blankTile = 0x1ff;
  s32 result = 0;
  struct TitleScreenWork *work = HeapSlot_AllocEwram(172, 0x70);
  u8 *data;
  u16 *tilemap;
  s16 tile;
  u32 i;
  u32 j;
  u32 holdFrames;

  Task_ResetAll();
  ScreenFade_ToWhite(1);
  DebugText_Clear();
  VramCache_Reset();
  wait(1);
  {
    /* FAKEMATCH(pin): the native loads the control word from the pool before
     * it starts the register address; as REG_BG2CNT = 0x681, 2 of 254
     * instructions differ in tla-en. */
    register u16 control asm("r3") =
        BGCNT_PRIORITY(1) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(6);

    /* FAKEMATCH(barrier): an asm holding r2 until the control word is loaded;
     * as the pin alone, 2 of 254 instructions differ in tla-en (the Japanese
     * and German editions place the first literal pool one instruction later
     * and match with the pin alone). */
#if TLA_PATCH_1 && !(TLA_PATCH_2 && !TLA_PATCH_3)
    __asm__("" : "+l"(control) : : "r2");
#endif
    REG_BG2CNT = control;
  }
  REG_DISPCNT = DISPCNT_OBJ_1D_MAP | DISPCNT_BG2_ON | DISPCNT_OBJ_ON;
  gBgScroll[2].y = result;
  data = Resource_GetTableEntry((s32)&ResourceId_TitleScreenPicture);
  Dma_Set(data, (void *)BG_PLTT, DMA_COPY_WORDS(112), REG_DMA3);
  {
    u8 *cells = Ram_MapCellBuffer;

    Decompress_TaggedLz(data + 0x1c0, cells);
    Dma_Set(cells, (void *)0x06006800, DMA_COPY_WORDS(0x9600 / 4), REG_DMA3);
  }
  {
    u16 *start = (u16 *)0x06003000;

    tilemap = start;
  }
  tile = 0x1a0;
  i = 0;
row:
  j = 0;
column:
  {
    /* FAKEMATCH(temporary): the s16 step as its high-half wrap, with goto
     * loops, so loop.c leaves it alone; as *tilemap++ = tile; tile++, 94 of
     * 254 instructions differ in tla-en. */
    s32 old = tile;

    tile = ((old << 16) + 0x10000) >> 16;
    *tilemap++ = old;
  }
  if (++j <= 29)
    goto column;
  *tilemap++ = blankTile;
  *tilemap++ = blankTile;
  if (++i <= 19)
    goto row;
  for (i = 0; i < 4; i++)
    gBgScroll[i].x = gBgScroll[i].y = 0;
  Dma_Set(gBgScroll, REG_BG0HOFS, DMA_COPY_WORDS(4), REG_DMA3);
  Window_LoadGraphics();
  DebugText_Clear();
  if (prompt != 0) {
    u8 *buffer = HeapSlot_AllocEwram(56, 0x400);
    struct TitleSprite *sprite = work->sprites;

    Resource_DecodeByteLzInRam(gTitle_PromptTiles, buffer);
    for (i = 0; i < PROMPT_TILE_BLOCKS; i++) {
      s32 vramBlock =
          VramCache_Load(VramCache_FindFreeEntry(), 128, buffer + (i << 8) / 2);
      u32 *spriteWords = (u32 *)sprite;

      *spriteWords++ = 0;
      sprite++;
      *spriteWords++ = 0x40004000;
      *spriteWords = vramBlock;
    }
    HeapSlot_Release(56);
  }
  ScreenFade_FromWhite(30);
  ScreenFade_Wait();
  REG_DISPCNT =
      DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON | DISPCNT_BG2_ON | DISPCNT_OBJ_ON;
  if (prompt != 0)
    holdFrames = 5400;
  else
    holdFrames = 660;
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
            Func_080140d8(sprite, 0);
            x += 32;
            sprite++;
          }
        }
#else
        for (j = 0, x = PROMPT_X; j < PROMPT_SPRITES; j++) {
          sprite->x = x;
          sprite->y = PROMPT_Y;
          Func_080140d8(sprite, 0);
          x += 32;
          sprite++;
        }
#endif
        level = gTitle_PromptBlendLevels[i % 60];
        REG_BLDCNT = BLDCNT_TGT1_OBJ | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_BG0 |
                     BLDCNT_TGT2_BG1 | BLDCNT_TGT2_BG2 | BLDCNT_TGT2_BG3 |
                     BLDCNT_TGT2_BD;
        REG_BLDALPHA = (16 - level) * 256 + level;
      }
      if (gInput.pressed & (KEY_A | KEY_START)) {
        result = 1;
        break;
      }
    }
    Random16();
    wait(1);
  }
  HeapSlot_Release(172);
  {
    /* FAKEMATCH(pin): the native loads the zero from the pool before it
     * starts the register address; as REG_BLDCNT = 0 and REG_BLDALPHA = 0, 2
     * of 254 instructions differ in tla-en. */
    register u16 zero asm("r2") = 0;

    REG_BLDCNT = zero;
    REG_BLDALPHA = zero;
  }
  wait(1);
  return result;
}

/// An empty entry the far-call table keeps.
// As The Broken Seal's.
void Title_NoOpPalette(void) {}

/// Shows the Nintendo logo with its jingle. With mode 0 it fades in and holds
/// two seconds; otherwise it waits a second, fades in, holds three seconds and
/// fades out, faster once skipped. Returns -1 when A or Start skipped it.
// As The Broken Seal's function of the same name.
s32 Title_ShowNintendoLogo(s32 mode) {
  s32 result;
  u8 *data;
  u16 *map;
  u32 x;
  u32 y;
  s32 tile;
  s32 blank;
  s32 resource;

  Sound_PlayFar(110);
  gRenderOamEnabled = 1;
  resource = (s32)&ResourceId_NintendoLogo;
  blank = 0x1ff;
  Task_ResetAll();
  ScreenFade_ToBlack(1);
  DebugText_Clear();
  wait(1);
  {
    /* FAKEMATCH(pin): the native loads the control word from the pool before
     * it starts the register address; as REG_BG2CNT = 0x681, 2 of 192
     * instructions differ in tla-en. */
    register u16 control asm("r3") =
        BGCNT_PRIORITY(1) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(6);

    REG_BG2CNT = control;
  }
  REG_DISPCNT = DISPCNT_OBJ_1D_MAP | DISPCNT_BG2_ON | DISPCNT_OBJ_ON;
  result = 0;
  gBgScroll[2].y = result;
  data = Resource_GetTableEntry(resource);
  Dma_Set(data, (void *)BG_PLTT, DMA_COPY_WORDS(112), REG_DMA3);
  {
    u8 *cells = Ram_MapCellBuffer;

    Decompress_TaggedLz(data + 448, cells);
    Dma_Set(cells, (void *)0x06004000, DMA_COPY_WORDS(0x9600 / 4), REG_DMA3);
  }
  {
    u16 *start = (u16 *)0x06003000;

    map = start;
  }
  tile = 256;
  y = 0;
col:
  {
    x = 0;
  row:
    {
      /* FAKEMATCH(temporary): the s16 step as its high-half wrap keeps loop.c
       * from hoisting it; as an s16 tile stored and stepped, 37 of 192
       * instructions differ in tla-en. */
      s32 old = tile;

      tile = ((old << 16) + 0x10000) >> 16;
      *map++ = old;
    }
    if (++x <= 29)
      goto row;
    *map++ = blank;
    *map++ = blank;
  }
  if (++y <= 19)
    goto col;
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
    for (y = 0; y < 120; y++) {
      if (gInput.pressed & (KEY_A | KEY_START)) {
        result = -1;
        break;
      }
      wait(1);
    }
    return result;
  }
  for (y = 0; y < 60; y++) {
    if (gInput.pressed & (KEY_A | KEY_START)) {
      result = -1;
      break;
    }
    wait(1);
  }
  if (result != 0)
    ScreenFade_FromBlack(8);
  else
    ScreenFade_FromBlack(60);
  ScreenFade_Wait();
  if (result == 0) {
    for (y = 0; y < 180; y++) {
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
    ScreenFade_ToBlack(60);
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

  gRenderOamEnabled = 1;
  result = 0;
  resource = (s32)&ResourceId_CamelotLogo;
  Task_ResetAll();
  ScreenFade_ToBlack(1);
  DebugText_Clear();
  wait(1);
  REG_BG2CNT = 0x685;
  REG_DISPCNT = 0x1440;
  gBgScroll[2].y = result;
  buffer = Resource_GetTableEntry(resource);
  Decompress_TaggedLz(buffer, Ram_MapCellBuffer);
  buffer = Ram_MapCellBuffer;
  Dma_Set(buffer, (void *)BG_PLTT, (DMA_ENABLE | DMA_32BIT) << 16 | 0x1c0 / 4,
          REG_DMA3);
  buffer += 0x1c0;
  Dma_Set(buffer, (void *)0x06003000,
          (DMA_ENABLE | DMA_32BIT) << 16 | 0x800 / 4, REG_DMA3);
  buffer += 0x800;
  Dma_Set(buffer, BG_CHAR_BLOCK(1), (DMA_ENABLE | DMA_32BIT) << 16 | 0x4000 / 4,
          REG_DMA3);
  buffer += 0x4000;
  for (i = 0; i < 4; i++) {
    gBgScroll[i].y = 0;
    gBgScroll[i].x = 0;
  }
  Dma_Set(gBgScroll, REG_BG0HOFS, (DMA_ENABLE | DMA_32BIT) << 16 | 4, REG_DMA3);
  Window_LoadGraphics();
  DebugText_Clear();
  ScreenFade_FromWhite(1);
  ScreenFade_Wait();
  REG_DISPCNT = 0x1540;
  for (i = 0; i < 120; i++) {
    Dma_Set(((gFrameCount >> 3) & 3) * 1024 + buffer, BG_CHAR_BLOCK(1) + 0x100,
            (DMA_ENABLE | DMA_32BIT) << 16 | 0x340 / 4, REG_DMA3);
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
void PaletteFade_CalculateSteps(s16 *src, s16 *dst, s16 *step, s32 frames) {
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

/// Steps the palette fade on heap slot 0x80 by one frame, as The Broken Seal's
/// PaletteFade_Update does: adds each channel's step until the last frame,
/// which copies the target instead, packs the channels into the hidden page
/// of RGB555 colours and queues that page for the BG and OBJ palettes.
void PaletteFade_Update(void) {
  struct PaletteFade *work = Ram_HeapSlots->block32;
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
    for (i = 0; i <= 0x5ff; i++)
      *current++ += *delta++;
  } else {
    Dma_Set(work->target, work->current, DMA_COPY_WORDS(768), REG_DMA3);
    work->duration = 0;
  }
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
  IO_WRITE_QUEUE_PUSH((u32)BG_PLTT, (u32)bank, DMA_COPY_WORDS(128));
  IO_WRITE_QUEUE_PUSH((u32)OBJ_PLTT, (u32)(bank + 256), DMA_COPY_WORDS(128));
}

void Palette_Transform(u32 mode, u16 *source, u16 *destination,
                       s32 selectedPalettes) {
  u32 colorIndex;
  u32 colorCount = 512;
  s32 red;
  s32 blue;
  u32 colorWork;
  s32 green;
  s32 tintedRed;

  if (mode == 0x8000)
    mode = *source;
  if (selectedPalettes == 1) {
    colorCount = 256;
  } else if (selectedPalettes == 2) {
    destination += 768;
    source += 256;
    colorCount = 256;
  }

  if (mode < 0x8000) {
    *destination++ = mode & 0x7c00;
    *destination++ = (mode & 0x03e0) << 5;
    *destination++ = (mode & 0x001f) << 10;
    Dma_Set(destination - 3, destination,
            DMA_ENABLE << 16 | (((colorCount - 1) * 6) >> 1), REG_DMA3);
  } else if (mode < 0x100000) {
    switch (mode) {
    case 0x10001:
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
    case 0x10002:
      for (colorIndex = 0; colorIndex < colorCount; colorIndex++) {
        colorWork = *source++;
        red = colorWork & 31;
        green = (colorWork >> 5) & 31;
        blue = (colorWork >> 10) & 31;
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
        *destination++ = Data_081a87ba[blue];
        *destination++ = Data_081a87ba[green];
        *destination++ = Data_081a87ba[red];
      }
      break;
    case 0x10003:
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
        red = BattleEffect_ClampRgb555Channel(red);
        green = BattleEffect_ClampRgb555Channel(green);
        blue = BattleEffect_ClampRgb555Channel(blue);
        SET_VECTOR(destination, Data_081a87fa[blue], Data_081a87ba[green],
                   Data_081a877a[red]);
        destination += 3;
      }
      break;
    case 0x10004:
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
        red = BattleEffect_ClampRgb555Channel(red);
        green = BattleEffect_ClampRgb555Channel(green);
        blue = BattleEffect_ClampRgb555Channel(blue);
        *destination++ = Data_081a877a[blue];
        *destination++ = Data_081a877a[green];
        *destination++ = Data_081a877a[red];
      }
      break;
    case 0x10005:
      for (colorIndex = 0; colorIndex < colorCount; colorIndex++) {
        s32 mean;

        colorWork = *source++;
        red = colorWork & 31;
        green = (colorWork >> 5) & 31;
        blue = (colorWork >> 10) & 31;
        mean = BattleEffect_ClampRgb555Channel((red + green + blue) / 3);
        red = (red >> 1) + mean;
        green = (green >> 1) + mean;
        blue = (blue >> 1) + mean;
        red = BattleEffect_ClampRgb555Channel(red);
        green = BattleEffect_ClampRgb555Channel(green);
        blue = BattleEffect_ClampRgb555Channel(blue);
        SET_VECTOR(destination, Data_081a87fa[blue], Data_081a87fa[green],
                   Data_081a87fa[red]);
        destination += 3;
      }
      break;
    case 0x10006:
      for (colorIndex = 0; colorIndex < colorCount; colorIndex++) {
        colorWork = *source++;
        green = (colorWork >> 5) & 31;
        red = colorWork & 31;
        blue = (colorWork >> 10) & 31;
        red += ((green >> 3) + (blue >> 3));
        red = BattleEffect_ClampRgb555Channel(red);
        green -= green / 3;
        blue -= blue / 3;
        SET_VECTOR(destination, Data_081a877a[blue], Data_081a877a[green],
                   Data_081a87ba[red]);
        destination += 3;
      }
      break;
    case 0x10007:
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
        red = BattleEffect_ClampRgb555Channel(red);
        green = BattleEffect_ClampRgb555Channel(green);
        blue = BattleEffect_ClampRgb555Channel(blue);
        SET_VECTOR(destination, Data_081a87fa[blue], Data_081a87ba[green],
                   Data_081a877a[red]);
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
  } else if (mode & 0x200000) {
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
  } else if (mode & 0x400000) {
    u32 tintRed;
    u32 tintGreen;
    u32 tintBlue;
    s32 scaledGreen;
    s32 scaledBlue;
    s32 clampedRed;
    s32 clampedGreen;
    s32 clampedBlue;

    tintRed = mode & 31;
    tintGreen = (mode >> 5) & 31;
    tintBlue = (mode >> 10) & 31;
    for (colorIndex = 0; colorIndex < colorCount; colorIndex++) {
      colorWork = *source++;
      red = colorWork & 31;
      green = (colorWork >> 5) & 31;
      blue = (colorWork >> 10) & 31;
      colorWork = Iwram_SignedDivide((red + green + blue) << 4,
                                     tintRed + tintGreen + tintBlue);
      tintedRed = Iwram_MulQ16(((colorWork * tintRed) >> 4) << 16,
                               (s32)(tintRed << 16) >> 4);
      scaledGreen = Iwram_MulQ16(((colorWork * tintGreen) >> 4) << 16,
                                 (s32)(tintGreen << 16) >> 4);
      scaledBlue = Iwram_MulQ16(((colorWork * tintBlue) >> 4) << 16,
                                (s32)(tintBlue << 16) >> 4);
      tintedRed = (u32)tintedRed >> 16;
      clampedRed = BattleEffect_ClampRgb555Channel(tintedRed);
      scaledGreen = (u32)scaledGreen >> 16;
      clampedGreen = BattleEffect_ClampRgb555Channel(scaledGreen);
      scaledBlue = (u32)scaledBlue >> 16;
      clampedBlue = BattleEffect_ClampRgb555Channel(scaledBlue);
      destination[colorIndex * 3 + 0] = Data_081a877a[clampedBlue];
      destination[colorIndex * 3 + 1] = Data_081a877a[clampedGreen];
      destination[colorIndex * 3 + 2] = Data_081a877a[clampedRed];
    }
  } else if (mode & 0x800000) {
    for (colorIndex = 0; colorIndex < colorCount; colorIndex++) {
      colorWork = *source++;
      destination[colorIndex * 3 + 0] = colorWork & 0x7c00;
      destination[colorIndex * 3 + 1] = (colorWork & 0x03e0) << 5;
      destination[colorIndex * 3 + 2] = (colorWork & 0x001f) << 10;
    }
  } else {
    if (selectedPalettes == 2)
      mode += 0x600;
    Dma_Set((const void *)mode, destination,
            DMA_COPY_WORDS((colorCount * 6) >> 2), REG_DMA3);
  }
}

void PaletteFade_Initialize(void) {
  struct PaletteFade *buffer;
  s32 operation;

  buffer = HeapSlot_AllocEwram(128, sizeof(*buffer));
  Dma_Fill32(0, buffer, sizeof(*buffer));
  Dma_Set((void *)BG_PLTT, buffer, DMA_COPY_WORDS(128), REG_DMA3);
  Dma_Set((void *)OBJ_PLTT, &buffer->palette[256], DMA_COPY_WORDS(128),
          REG_DMA3);
  Palette_Transform(0x10000, buffer->palette, buffer->target, 0);
  operation = 1152;
  Task_Add((s32)(PaletteFade_Update), operation);
}

void PaletteFade_Release(void) {
  Task_Remove((u32)((s32)&PaletteFade_Update));
  HeapSlot_Release(0x80);
}

void PaletteFade_SetTarget(s32 index, s32 transform) {
  struct PaletteFade *target = Ram_HeapSlots->block32;

  if (target != NULL)
    Palette_Transform(index, target->palette, target->target, transform);
}

void PaletteFade_SetSource(s32 index, s32 transform) {
  struct PaletteFade *target = Ram_HeapSlots->block32;

  if (target != NULL)
    Palette_Transform(index, (u16 *)target, target->current, transform);
}

void PaletteFade_SetBackdrop(s32 value) {
  struct PaletteFade *target = Ram_HeapSlots->block32;

  if (target != NULL)
    target->palette[0] = value;
}

void TitlePaletteFade_Start(s32 value) {
  struct PaletteFade *state = Ram_HeapSlots->block32;

  if (state != NULL) {
    state->duration = value;
    state->step = 0;
    PaletteFade_CalculateSteps((s16 *)state->current, (s16 *)state->target,
                               (s16 *)state->delta, value);
  }
}

s32 BattleEffect_ClampRgb555Channel(s32 value) {
  if (value > 31)
    return 31;
  if (value < 0)
    value = 0;
  return value;
}

s32 TitlePaletteFade_ClampShiftedComponent(s32 value) {
  if (value > 31744)
    value = 31744;
  return value;
}
