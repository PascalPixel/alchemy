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
  s32 backRows; // rows of the far background copied in as it scrolls
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
#define WRAP_TO_BYTE(v)                                                             \
  while ((v) > 255)                                                            \
    (v) -= 256;                                                                \
  while ((v) < 0)                                                              \
  (v) += 256

// Sets x and y to the top left of a sprite of half-width `half` centred on
// column `column`, on the slope that runs from `base` at x = 104 to `top` at
// x = 184
#define PLACE_ON_SLOPE(column, half)                                           \
  x = gTitle_IntroSpriteColumns[column] - (half);                              \
  y = (top - base) * ((x + (half)) - 104) / 80 + base - (half);                \
  WRAP_TO_BYTE(y)

// Writes OAM entries i to i + 3: four sprites of `size` pixels, mirrored into
// the four quarters of one picture at (x, y)
#define MIRRORED_QUAD(i, size, shape, tile)                                             \
  INTRO_OBJ(i).attr01 = ((x + 4) << 16) | y | ((shape) + 0x00002400);                \
  INTRO_OBJ((i) + 1).attr01 = ((x + 4 + (size)) << 16) | y | ((shape) + 0x10002400); \
  INTRO_OBJ((i) + 2).attr01 =                                                        \
      ((x + 4) << 16) | (u8)(y + (size)) | ((shape) + 0x20002400);             \
  INTRO_OBJ((i) + 3).attr01 =                                                        \
      ((x + 4 + (size)) << 16) | (u8)(y + (size)) | ((shape) + 0x30002400);    \
  INTRO_OBJ(i).attr2 = (tile);                                                       \
  INTRO_OBJ((i) + 1).attr2 = (tile);                                                 \
  INTRO_OBJ((i) + 2).attr2 = (tile);                                                 \
  INTRO_OBJ((i) + 3).attr2 = (tile)

// A VBlank task of the title intro: places the rising sprites along the
// slope of the scrolling background, alternating two sets every other frame
// so twice as many show, hides the rest and copies the list to OAM.
void Title_UpdateIntroSprites(void) {
  /* FAKEMATCH: the OAM shadow it builds is struct OamShadow, volatile words
   * (tag in oambuf.h): without the qualifier differ 205, 471 instructions for
   * 496. */
  struct IntroWork *work;
  u32 n;
  s32 base;
  s32 top;
  s32 x;
  s32 y;
  s32 line;

  work = Ram_WorkSlot[43];
  n = 0;
  if (gDebugPaused == 0) {
    if ((++work->tick & 3) == 0)
      work->rise++;
  }
  base = 48 - gBgScroll[1].y;
  top = 144 - work->rise;
  if (work->frame < 0x118) {
    if ((work->tick & 1) == 0) {
      PLACE_ON_SLOPE(0, 16);
      MIRRORED_QUAD(n, 16, 0x40000000, 232);
      n += 4;
      PLACE_ON_SLOPE(2, 16);
      INTRO_OBJ(n).attr01 = ((x + 4) << 16) | y | 0x80002400;
      INTRO_OBJ(n).attr2 = 128;
      n++;
      PLACE_ON_SLOPE(4, 32);
      MIRRORED_QUAD(n, 32, 0x80000000, 192);
      n += 4;
    } else {
      PLACE_ON_SLOPE(1, 16);
      MIRRORED_QUAD(n, 16, 0x40000000, 232);
      n += 4;
      PLACE_ON_SLOPE(3, 16);
      MIRRORED_QUAD(n, 16, 0x40000000, 224);
      n += 4;
      PLACE_ON_SLOPE(5, 32);
      MIRRORED_QUAD(n, 32, 0x80000000, 160);
      n += 4;
    }
  }
  for (; n <= 119; n++)
    INTRO_OBJ(n).attr01 = 0x400020a0;
  REG_BLDCNT = 0x3f50;
  REG_BLDALPHA = 0x0e0e;
  Dma_Set(&INTRO_OBJ(0), OAM, ((n * 8) >> 2) | 0x84000000, REG_DMA3);
  line = 32 - gBgScroll[1].y;
  WRAP_TO_BYTE(line);
  INTRO_OBJ(12).attr01 = line | 0xc05c2000;
  INTRO_OBJ(12).attr2 = 0x800;
  Dma_Set(&INTRO_OBJ(12), (u8 *)OAM + n * 8, 0x84000002, REG_DMA3);
  Dma_Set(&INTRO_OBJ(0), OAM, 0x84000008, REG_DMA3);
}

// Loads the title intro's two scrolling backgrounds and its sprite tiles,
// builds their maps and sets the blending and windows.
void Title_LoadIntroBackgrounds(void) {
  struct IntroWork *work;
  u8 *data;
  u16 *map;
  s32 x;
  s32 y;
  u16 tile;
  s32 blank;

  work = Ram_WorkSlot[43];
  REG_DISPCNT = 0;
  data = Resource_GetTableEntry((s32)&ResourceId_IntroGraphicsA);
  Dma_Set(data, (void *)OBJ_PLTT, 0x84000080, REG_DMA3);
  OBJ_PLTT[0] = 0;
  data += 0x200;
  Resource_DecodeByteLz(data, Ram_MapCellBuffer);
  Dma_Set(Ram_MapCellBuffer, (void *)0x06010000, 0x80000f00, REG_DMA3);
  data = Resource_GetTableEntry((s32)&ResourceId_IntroGraphicsC);
  Dma_Set(data, (void *)BG_PLTT, 0x84000080, REG_DMA3);
  BG_PLTT[0] = 0;
  data += 0x200;
  Resource_DecodeByteLz(data, Ram_MapCellBuffer);
  Dma_Set(Ram_MapCellBuffer + 0x2940, BG_CHAR_BLOCK(0), 0x80002760, REG_DMA3);
  Dma_Set(Ram_MapCellBuffer + 0xa140, (void *)0x06004ec0, 0x80004ec0, REG_DMA3);

  blank = 0x1ff;
  map = (u16 *)BG_SCREEN_BLOCK(30);
  tile = 0x267;
  for (y = 0; y <= 10; y++) {
    for (x = 0; x < 30; x++)
      *map++ = tile++;
    *map++ = blank;
    *map++ = blank;
  }
  tile = 0x13b;
  for (y = 11; y <= 31; y++) {
    for (x = 0; x < 30; x++)
      *map++ = tile++;
    *map++ = blank;
    *map++ = blank;
  }
  map = (u16 *)BG_SCREEN_BLOCK(31);
  tile = 300;
  for (y = 0; y <= 10; y++) {
    for (x = 0; x < 30; x++)
      *map++ = tile++;
    *map++ = blank;
    *map++ = blank;
  }
  tile = 0;
  for (y = 11; y <= 31; y++) {
    for (x = 0; x < 30; x++)
      *map++ = tile++;
    *map++ = blank;
    *map++ = blank;
  }

  REG_BG1CNT = 0x1f43;
  REG_BG2CNT = 0x1e81;
  REG_WIN0H = 0xf0;
  REG_WIN0V = 0x9f;
  REG_WIN1H = 0xf0;
  REG_WIN1V = 0x9f;
  REG_WININ = 0x1616;
  for (y = 0; y < 4; y++) {
    gBgScroll[y].y = 0;
    gBgScroll[y].x = 0;
  }
  gBgScroll[1].y = 96;
  gBgScroll[2].y = 96;
  work->frame = 0;
  work->backRows = 0;
  work->frontRows = 0;
  work->tick = 0;
  work->rise = 0;
  work->state = 0;
  Dma_Set(gBgScroll, REG_BG0HOFS, 0x84000004, REG_DMA3);
  REG_BLDCNT = 0x3fbf;
  REG_BLDALPHA = 0x1010;
  REG_BLDCNT = 0x3f44;
}

// The "Press Start" prompt: its tile count, rows and sprites, and where it
// stands, by edition.
#if defined(TBS_EDITION_DE)
#define PROMPT_TILES 8
#else
#define PROMPT_TILES 5
#endif
#if defined(TBS_EDITION_DE)
#define PROMPT_ROWS 2
#define PROMPT_SPRITES 3
#define PROMPT_X 80
#define PROMPT_Y 116
#elif !EDITION_INTERNATIONAL
#define PROMPT_SPRITES 5
#define PROMPT_X 56
#define PROMPT_Y 126
#elif defined(TBS_EDITION_FR)
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
  u8 saved;
  u16 zero;
  u8 *data;
  u16 *map;
  u32 i;
  u16 tile;
  u32 j;
  u32 limit;
  s32 blank;

  result = 0;
  blank = 0x1ff;
  saved = gCpuLoadDisplayEnabled;
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
    s32 next;
    s32 tick;

    frame = work->frame;
    if (frame > 20 && frame < 239 &&
        (gKeysPressed & (KEY_A | KEY_START)) != 0) {
      work->state = 1;
      work->frame = 239;
      frame = 239;
    }
    next = frame + 1;
    work->frame = next;
    if (next <= 278) {
      tick = work->tick;
      if (tick % 3 == 0) {
        s32 y;

        gBgScroll[1].y--;
        y = gBgScroll[1].y;
        if ((y & 7) == 0) {
          Dma_Set(Ram_MapCellBuffer + 0x2580 - work->backRows * 960,
                  (u8 *)0x06004b00 - work->backRows * 960, 0x800001e0,
                  REG_DMA3);
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
                    (u8 *)0x0600e4c0 - work->frontRows * 1920, 0x800003c0,
                    REG_DMA3);
          } else {
            zero = 0;
            Dma_Set(&zero,
                    (u8 *)0x06004ec0 + (160 - work->frontRows * 8) % 160 * 240,
                    0x810003c0, REG_DMA3);
            map = (u16 *)0x0600f6c0;
            for (i = 0; i < 5; i++) {
              for (j = 0; j < 30; j++)
                *map++ = 0x13b;
              *map++ = 0x13b;
              *map++ = 0x13b;
            }
          }
          work->frontRows++;
        }
      }
      if (work->frame == 239)
        work->state = 1;
    } else if (next == 281) {
      work->state = 2;
    } else if (next == 289) {
      work->state = 0;
    } else if (next == 280) {
      Fade_ToWhite(1);
      Scheduler_RemoveCallback(Title_UpdateIntroSprites);
      gOamCopyEnabled = 1;
      WaitFrames(1);
      REG_BG2CNT = 0x681;
      REG_DISPCNT = 0x1440;
      gBgScroll[2].y = 0;
      data = Resource_GetTableEntry((s32)&ResourceId_IntroGraphicsB);
      Dma_Set(data, (void *)BG_PLTT, 0x84000078, REG_DMA3);
      BG_PLTT[0] = 0;
      data += 0x200;
      Resource_DecodeByteLz(data, Ram_MapCellBuffer);
      Dma_Set(Ram_MapCellBuffer, BG_CHAR_BLOCK(1), 0x80004b00, REG_DMA3);
      map = (u16 *)BG_SCREEN_BLOCK(6);
      tile = 0x100;
      i = 0;
      break;
    }
    WaitFrames(1);
  }
  for (; i < 20; i++) {
    for (j = 0; j < 30; j++)
      *map++ = tile++;
    *map++ = blank;
    *map++ = blank;
  }

  Ui_LoadWindowGraphics();
  DebugText_Clear();
  if (prompt != 0) {
    u8 *buffer = Runtime_AllocateBlock(14, 0x400);
    struct IntroSprite *sprite = work->area.prompt.sprites;

    Resource_DecodeByteLz(gTitle_PromptTiles, buffer);
    for (i = 0; i < PROMPT_TILES; i++) {
      s32 block =
          VramCache_Load(Resource_FindFreeEntry(), 128, buffer + (i << 8) / 2);
      u32 *out = (u32 *)sprite;

      *out++ = 0;
      sprite++;
      *out++ = 0x40004000;
      *out = block;
    }
    Runtime_ReleaseHeapBlock(14);
  }
  Fade_FromWhite(30);
  Fade_Wait();
  REG_DISPCNT = 0x1540;
  if (prompt != 0)
    limit = 3600;
  else
    limit = 300;
  for (i = 0; i < limit; i++) {
    if (prompt != 0) {
      struct IntroSprite *sprite = work->area.prompt.sprites;
      u32 level;
      s32 x;
#if defined(TBS_EDITION_DE)
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
  gCpuLoadDisplayEnabled = saved;
  Runtime_ReleaseHeapBlock(43);
  REG_BLDCNT = 0;
  REG_BLDALPHA = 0;
  WaitFrames(1);
  return result;
}

// An empty routine.
void Title_NoOpPalette(void) {}

// Shows the Nintendo logo with its jingle. With `mode` 0 it fades in and
// holds two seconds; otherwise it waits a second, fades in, holds three
// seconds and fades out, faster once skipped. Returns -1 when A or Start
// skipped it.
s32 Title_ShowSplashScreen(s32 mode) {
  s32 result;
  u8 *data;
  u16 *map;
  u32 x;
  u32 y;
  u16 tile;
  s32 blank;
  s32 resource;

  AudioCommand_PlayFar(110);
  gOamCopyEnabled = 1;
  resource = (s32)&ResourceId_NintendoLogo;
  blank = 0x1ff;
  Scheduler_ResetTaskTable();
  Fade_ToBlack(1);
  DebugText_Clear();
  WaitFrames(1);
  REG_BG2CNT = 0x681;
  REG_DISPCNT = 0x1440;
  result = 0;
  gBgScroll[2].y = result;
  data = Resource_GetTableEntry(resource);
  Dma_Set(data, (void *)BG_PLTT, 0x84000070, REG_DMA3);
  Resource_DecodeType01(data + 448, Ram_MapCellBuffer);
  Dma_Set(Ram_MapCellBuffer, BG_CHAR_BLOCK(1), 0x84002580, REG_DMA3);
  tile = 256;
  map = (u16 *)BG_SCREEN_BLOCK(6);
  for (y = 0; y < 20; y++) {
    for (x = 0; x < 30; x++)
      *map++ = tile++;
    *map++ = blank;
    *map++ = blank;
  }
  for (y = 0; y < 4; y++) {
    gBgScroll[y].y = 0;
    gBgScroll[y].x = 0;
  }
  Dma_Set(gBgScroll, REG_BG0HOFS, 0x84000004, REG_DMA3);
  Ui_LoadWindowGraphics();
  DebugText_Clear();
  REG_DISPCNT = 0x1540;
  if (mode == 0) {
    Fade_FromBlack(1);
    Fade_Wait();
    for (y = 0; y < 120; y++) {
      if (gKeysPressed & (KEY_A | KEY_START)) {
        result = -1;
        break;
      }
      WaitFrames(1);
    }
    return result;
  }
  for (y = 0; y < 60; y++) {
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
    for (y = 0; y < 180; y++) {
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
  u32 i;
  u8 *buffer;
  s32 resource;

  gOamCopyEnabled = 1;
  result = 0;
  resource = (s32)&ResourceId_CamelotLogo;
  Scheduler_ResetTaskTable();
  Fade_ToBlack(1);
  DebugText_Clear();
  WaitFrames(1);
  REG_BG2CNT = 0x685;
  REG_DISPCNT = 0x1440;
  gBgScroll[2].y = result;
  /* FAKEMATCH: needed to match: i carries the decode destination before it
   * counts; a pointer of its own is coalesced with buffer (31 differ). */
  i = (u32)Ram_MapCellBuffer;
  Resource_DecodeType01(Resource_GetTableEntry(resource), (u8 *)i);
  buffer = (u8 *)i;
  Dma_Set(buffer, (void *)BG_PLTT, DMA_ENABLE32 | DMA_32BIT | 0x1c0 / 4,
          REG_DMA3);
  buffer += 0x1c0;
  Dma_Set(buffer, BG_SCREEN_BLOCK(6), DMA_ENABLE32 | DMA_32BIT | 0x800 / 4,
          REG_DMA3);
  buffer += 0x800;
  Dma_Set(buffer, BG_CHAR_BLOCK(1), DMA_ENABLE32 | DMA_32BIT | 0x4000 / 4,
          REG_DMA3);
  buffer += 0x4000;
  for (i = 0; i < 4; i++) {
    gBgScroll[i].y = 0;
    gBgScroll[i].x = 0;
  }
  Dma_Set(gBgScroll, REG_BG0HOFS, DMA_ENABLE32 | DMA_32BIT | 4, REG_DMA3);
  Ui_LoadWindowGraphics();
  DebugText_Clear();
  Fade_FromWhite(1);
  Fade_Wait();
  REG_DISPCNT = 0x1540;
  for (i = 0; i < 120; i++) {
    Dma_Set(((gFrameCount >> 3) & 3) * 0x400 + buffer, BG_CHAR_BLOCK(1) + 0x100,
            DMA_ENABLE32 | DMA_32BIT | 0x340 / 4, REG_DMA3);
    if (gKeysPressed & (KEY_A | KEY_START))
      break;
    WaitFrames(1);
  }
  return result;
}

// An empty routine.
void Graphics_NoOpColor(void) {}

// The palette fade (struct PaletteFade) keeps the saved palettes, the
// colour components shown now, the target components and each one's step.

// Writes to `step` each of the 1536 colour components' change per frame,
// from `src` now to `dst` over `frames` frames (nothing when `frames` is not
// positive).
void PaletteFade_ComputeSteps(s16 *src, s16 *dst, s16 *step,
                                        s32 frames) {
  s32 n;
  s32 from;
  s32 to;

  if (frames > 0) {
    n = 3 * 512 - 1;
    do {
      from = *src;
      to = *dst;
      *step = Iwram_SignedDivide(to - from, frames);
      n--;
      src++;
      dst++;
      step++;
    } while (n >= 0);
  }
}

// A task that steps the palette fade each frame: adds the step to every
// colour component until the fade is done, packs the components into the
// spare RGB555 copy of the palettes and queues both halves for VBlank.
void PaletteFade_Update(void) {
  struct PaletteFade *work = gPaletteBufferWork;
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
    Dma_Set(work->target, work->current, DMA_ENABLE32 | DMA_32BIT | 0x300,
            REG_DMA3);
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
  {
    u16 *front = bank;

    IO_WRITE_QUEUE_PUSH(0x05000000, (u32)front, 0x84000080);
  }
  IO_WRITE_QUEUE_PUSH(0x05000200, (u32)(bank + 256), 0x84000080);
}

// Transforms the 512 colours at `src` into colour components at `dst` (three
// per colour, each in the high bits of a halfword). `mode` picks the
// transform: a colour below 0x8000 fills every colour with it; 0x8000 takes
// that colour from src[0]; 0x10000 to 0x10007 are the whole-palette filters
// (plain, grey, sepia and the others); the TINT, BLEND and SPLIT flags carry
// a colour in their low bits; anything else is the address of components to
// copy. `half` 1 does only the background palettes, 2 only the sprites'.
void Palette_Transform(u32 mode, u16 *src, u16 *dst, s32 half) {
  u32 i;
  u32 cnt = 512;
  s32 r;
  s32 b;
  u32 c;
  s32 g;

  if (mode == PALETTE_TRANSFORM_FROM_SOURCE)
    mode = *src;
  if (half == 1) {
    cnt = 256;
  } else if (half == 2) {
    dst += 768;
    src += 256;
    cnt = 256;
  }

  if (mode < PALETTE_TRANSFORM_FROM_SOURCE) {
    *dst++ = mode & 0x7c00;
    *dst++ = (mode & 0x03e0) << 5;
    *dst++ = (mode & 0x001f) << 10;
    Dma_Set(dst - 3, dst, (((cnt - 1) * 6) >> 1) | DMA_ENABLE32, REG_DMA3);
  } else if (mode < PALETTE_TRANSFORM_FLAGS) {
    switch (mode) {
    case PALETTE_TRANSFORM_GREY:
      for (i = 0; i < cnt; i++) {
        c = *src++;
        r = ((c << 11) & 0xf800);
        g = ((c << 7) & 0x1f000);
        b = (c & 0x7c00);
        c = Iwram_SignedDivide(r + g + b, 7);
        *dst++ = c;
        *dst++ = c;
        *dst++ = c;
      }
      break;
    case PALETTE_TRANSFORM_SEPIA:
      for (i = 0; i < cnt; i++) {
        c = *src++;
        r = c & 31;
        c = Iwram_SignedDivide(r + ((c >> 5) & 31) + ((c >> 10) & 31), 10);
        r = c * 4 + 5;
        b = c * 3 + 5;
        /* FAKEMATCH: the reversible XOR initialization of g folds late, keeping
         * the native blue-first add/copy and separate lower test; g = b before
         * the two XORs exchanges the add/copy registers (four aligned
         * differences), plain clamps with g = b, or g and b each c * 3 + 5, 202
         * (g takes the computed register, b the copy, g's test not threaded
         * into b's). */
        g = b ^ r;
        g ^= r;
        if (r < 8)
          r = 8;
        if (b < 8) {
          g = 8;
          /* FAKEMATCH: needed to match: g is 8, so this pair changes no bits.
           * Its late fold retains the separate blue lower test; an ordinary
           * copy threads past that test and loses two instructions. */
          b ^= g;
          b ^= 8;
        }
        if (b < 8)
          b = 8;
        if (r > 28)
          r = 28;
        if (g > 28)
          g = 28;
        if (b > 28)
          b = 28;
        *dst++ = gPalette_ChannelCurveLight[b];
        *dst++ = gPalette_ChannelCurveLight[g];
        *dst++ = gPalette_ChannelCurveLight[r];
      }
      break;
    case PALETTE_TRANSFORM_WARM:
      for (i = 0; i < cnt; i++) {
        c = *src++;
        r = c & 31;
        g = (c >> 5) & 31;
        b = (c >> 10) & 31;
        r -= (u32)r >> 1;
        g -= g / 3;
        r += 6;
        g += 4;
        b -= 6;
        r = Graphics_ClampRgb555Channel(r);
        g = Graphics_ClampRgb555Channel(g);
        b = Graphics_ClampRgb555Channel(b);
        dst[0] = gPalette_ChannelCurveContrast[b];
        dst[1] = gPalette_ChannelCurveLight[g];
        dst[2] = gPalette_ChannelCurve[r];
        dst += 3;
      }
      break;
    case PALETTE_TRANSFORM_MUTED:
      for (i = 0; i < cnt; i++) {
        c = *src++;
        r = c & 31;
        g = (c >> 5) & 31;
        b = (c >> 10) & 31;
        if (r < 10)
          r = 10;
        if (g < 16)
          g = 16;
        if (b < 16)
          b = 16;
        if (r > 28)
          r = 28;
        if (g > 24)
          g = 24;
        if (b > 26)
          b = 26;
        g += 2;
        b += 2;
        r = Graphics_ClampRgb555Channel(r);
        g = Graphics_ClampRgb555Channel(g);
        b = Graphics_ClampRgb555Channel(b);
        *dst++ = gPalette_ChannelCurve[b];
        *dst++ = gPalette_ChannelCurve[g];
        *dst++ = gPalette_ChannelCurve[r];
      }
      break;
    case PALETTE_TRANSFORM_PALE:
      for (i = 0; i < cnt; i++) {
        s32 mean;

        c = *src++;
        r = c & 31;
        g = (c >> 5) & 31;
        b = (c >> 10) & 31;
        mean = Graphics_ClampRgb555Channel((r + g + b) / 3);
        r = (r >> 1) + mean;
        g = (g >> 1) + mean;
        b = (b >> 1) + mean;
        r = Graphics_ClampRgb555Channel(r);
        g = Graphics_ClampRgb555Channel(g);
        b = Graphics_ClampRgb555Channel(b);
        dst[0] = gPalette_ChannelCurveContrast[b];
        dst[1] = gPalette_ChannelCurveContrast[g];
        dst[2] = gPalette_ChannelCurveContrast[r];
        dst += 3;
      }
      break;
    case PALETTE_TRANSFORM_RED:
      for (i = 0; i < cnt; i++) {
        c = *src++;
        r = c & 31;
        g = (c >> 5) & 31;
        b = (c >> 10) & 31;
        r += ((g >> 3) + (b >> 3));
        r = Graphics_ClampRgb555Channel(r);
        g -= g / 3;
        b -= b / 3;
        dst[0] = gPalette_ChannelCurve[b];
        dst[1] = gPalette_ChannelCurve[g];
        dst[2] = gPalette_ChannelCurveLight[r];
        dst += 3;
      }
      break;
    case PALETTE_TRANSFORM_WARM2:
      for (i = 0; i < cnt; i++) {
        c = *src++;
        r = c & 31;
        g = (c >> 5) & 31;
        b = (c >> 10) & 31;
        r -= (u32)r >> 1;
        g -= g / 3;
        r += 6;
        g += 4;
        b -= 6;
        r = Graphics_ClampRgb555Channel(r);
        g = Graphics_ClampRgb555Channel(g);
        b = Graphics_ClampRgb555Channel(b);
        dst[0] = gPalette_ChannelCurveContrast[b];
        dst[1] = gPalette_ChannelCurveLight[g];
        dst[2] = gPalette_ChannelCurve[r];
        dst += 3;
      }
      break;
    default:
      for (i = 0; i < cnt; i++) {
        c = *src++;
        dst[i * 3 + 0] = c & 0x7c00;
        dst[i * 3 + 1] = (c & 0x03e0) << 5;
        dst[i * 3 + 2] = (c & 0x001f) << 10;
      }
      break;
    }
  } else if (mode & PALETTE_TRANSFORM_TINT) {
    u32 tr;
    u32 tg;
    u32 tb;

    tr = mode & 31;
    tg = (mode >> 5) & 31;
    tb = (mode >> 10) & 31;
    for (i = 0; i < cnt; i++) {
      c = *src++;
      r = ((c << 11) & 0xf800);
      g = ((c << 7) & 0x1f000);
      b = (c & 0x7c00);
      c = Iwram_SignedDivide(r + g + b, 96);
      r = c * tr;
      g = c * tg;
      b = c * tb;
      r = Graphics_ClampRgb555Component(r);
      g = Graphics_ClampRgb555Component(g);
      b = Graphics_ClampRgb555Component(b);
      dst[0] = b;
      dst[1] = g;
      dst[2] = r;
      dst += 3;
    }
  } else if (mode & PALETTE_TRANSFORM_BLEND) {
    u32 tr;
    u32 tg;
    u32 tb;

    tr = mode & 31;
    tg = (mode >> 5) & 31;
    tb = (mode >> 10) & 31;
    for (i = 0; i < cnt; i++) {
      c = *src++;
      r = c & 31;
      c = Iwram_SignedDivide((r + ((c >> 5) & 31) + ((c >> 10) & 31)) << 4,
                             tr + tg + tb);
      r = Iwram_MulQ16(((c * tr) >> 4) << 16, (s32)(tr << 16) >> 4);
      g = Iwram_MulQ16(((c * tg) >> 4) << 16, (s32)(tg << 16) >> 4);
      b = Iwram_MulQ16(((c * tb) >> 4) << 16, (s32)(tb << 16) >> 4);
      r = Graphics_ClampRgb555Channel((u32)r >> 16);
      g = Graphics_ClampRgb555Channel((u32)g >> 16);
      b = Graphics_ClampRgb555Channel((u32)b >> 16);
      *dst++ = gPalette_ChannelCurve[b];
      *dst++ = gPalette_ChannelCurve[g];
      *dst++ = gPalette_ChannelCurve[r];
    }
  } else if (mode & PALETTE_TRANSFORM_SPLIT) {
    for (i = 0; i < cnt; i++) {
      c = *src++;
      dst[i * 3 + 0] = c & 0x7c00;
      dst[i * 3 + 1] = (c & 0x03e0) << 5;
      dst[i * 3 + 2] = (c & 0x001f) << 10;
    }
  } else {
    if (half == 2)
      mode += 0x600;
    Dma_Set((const void *)mode, dst,
            ((cnt * 6) >> 2) | DMA_ENABLE32 | DMA_32BIT, REG_DMA3);
  }
}

// Allocates the palette fade, saves both hardware palettes in it as the
// starting colours and schedules PaletteFade_Update.
void PaletteFade_Init(void) {
  struct PaletteFade *buffer;
  s32 operation;

  buffer = Runtime_AllocateBlock(32, sizeof(*buffer));
  Dma_Fill32(0, buffer, sizeof(*buffer));
  Dma_Set((void *)0x05000000, buffer, 0x84000080, REG_DMA3);
  Dma_Set((void *)0x05000200, &buffer->palette[256], 0x84000080, REG_DMA3);
  Palette_Transform(PALETTE_TRANSFORM_PLAIN, buffer->palette,
                                  buffer->target, 0);
  operation = 3200;
  Scheduler_AddOrUpdateCallback(PaletteFade_Update, operation);
}

// Stops the palette fade and frees its buffers.
void PaletteFade_Release(void) {
  Scheduler_RemoveCallback(&PaletteFade_Update);
  Runtime_ReleaseHeapBlock(0x20);
}

// Sets the palette fade's target: the saved palettes through `index`, a
// Palette_Transform mode, for half `transform`.
void PaletteFade_SetTarget(s32 index, s32 transform) {
  struct PaletteFade *target = gPaletteBufferWork;

  if (target != NULL)
    Palette_Transform(index, target->palette, target->target,
                                    transform);
}

// Sets the colours shown now: the saved palettes through mode `index`.
void PaletteFade_SetCurrent(s32 index, s32 transform) {
  struct PaletteFade *target = gPaletteBufferWork;

  if (target != NULL)
    Palette_Transform(index, target, target->current, transform);
}

// Replaces the first saved colour, the backdrop.
void PaletteFade_SetBackdrop(s32 value) {
  struct PaletteFade *target = gPaletteBufferWork;

  if (target != NULL)
    target->palette[0] = value;
}

// Starts fading the shown colours to the target over `value` frames.
void PaletteFade_Start(s32 value) {
  struct PaletteFade *state = gPaletteBufferWork;

  if (state != NULL) {
    state->duration = value;
    state->step = 0;
    PaletteFade_ComputeSteps(state->current, state->target,
                                       state->delta, value);
  }
}

// Clamps a five-bit colour channel to 0..31.
s32 Graphics_ClampRgb555Channel(s32 val) {
  if (val > 31)
    return 31;
  if (val < 0)
    val = 0;
  return val;
}

// Caps a colour component at 0x7c00, the largest a five-bit channel shifted
// into the high bits reaches. There is no lower bound.
s32 Graphics_ClampRgb555Component(s32 val) {
  if (val > 0x7c00)
    val = 0x7c00;
  return val;
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
