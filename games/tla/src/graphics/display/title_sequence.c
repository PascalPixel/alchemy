#include "types.h"
#include "io_reg.h"
#include "system.h"
#include "frame.h"
#include "bg_scroll.h"
#include "palbuf.h"
#include "callback_scheduler.h"
#include "dma.h"
#include "mem.h"
#include "resource.h"
#include "graphics.h"
#include "ram_buffer.h"
#include "input.h"
#include "edition.h"

// The title screen's "Press Start" prompt: five sprites further left in the
// French edition, three elsewhere.
#if (LANGUAGE == LANGUAGE_FRENCH)
#define PROMPT_SPRITES 5
#define PROMPT_X 56
#define PROMPT_Y 126
#else
#define PROMPT_SPRITES 3
#define PROMPT_X 80
#define PROMPT_Y 124
#endif

void Func_08014b70(void);
void VramCache_Reset(void);
void Func_081a6030(void);
s32 Func_081a6154(s32 step);
s32 Func_081a6638(void);
s32 Func_081a6724(void);
void Func_081a6ea0(void);
void Ui_LoadWindowGraphics(void);

// Runs the title sequence's opening pictures in turn (Func_081a6154's steps
// 0 to 2 between the pictures of Func_081a6638 and Func_081a6724), fading
// out early when a key cuts one short and running Func_081a6ea0 when all
// finish, then restores the display and the CPU-load flag it saved.
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
  Func_08014b70();
  VramCache_Reset();
  WaitFrames(1);
  Scheduler_ResetTaskTable();
  gRenderOamEnabled = 0;
  gCpuLoadDisplayEnabled = 0;
  Func_081a6030();
  if ((result = Func_081a6154(0)) == 0 || (result = Func_081a6638()) == 0 ||
      (result = Func_081a6154(1)) == 0 || (result = Func_081a6724()) == 0 ||
      (result = Func_081a6154(2)) == 0) {
    PaletteFade_SetTarget(0, 0);
    PaletteFade_Start(10);
    WaitFrames(10);
  }
  if (result)
    Func_081a6ea0();
  gRenderOamEnabled = 1;
  WaitFrames(1);
  // The result's register counts the backgrounds (a separate counter takes
  // other registers).
  for (result = 0; result < 4; result++)
    gBgScroll[result].x = gBgScroll[result].y = 0;
  REG_DISPCNT = 0;
  Ui_LoadWindowGraphics();
  Func_08014b70();
  PaletteFade_Init();
  gCpuLoadDisplayEnabled = cpuLoad;
  return 1;
}

#if (LANGUAGE != LANGUAGE_JAPANESE) && (LANGUAGE != LANGUAGE_GERMAN)
// The Japanese and German title screens differ and stay on their scaffolds.

struct TitleSprite {
  u32 next; // the render slot link
  u8 y;
  u8 attr0High; // shape and mode bits of attribute 0
  u16 x : 9;
  u16 attr1High : 7; // flip and size bits of attribute 1
  s32 tile;
};

// The title screen's state, in heap slot 172.
struct TitleScreenWork {
  u8 unk00[16];
  struct TitleSprite sprites[8];
};

extern char ResourceId_TitleScreenPicture;
extern const u8 Data_081a873e[];
extern const u8 gTitle_PromptTiles[];

void Fade_ToWhite(s32 frames);
void Fade_FromWhite(s32 frames);
void Fade_Wait(void);
void Func_080140d8(void *entry, s32 slot);

// The title screen, as the end of The Broken Seal's Title_RunIntro: fades to
// white, shows Title_ScreenPicture on BG2 (its map numbered from tile 0x1a0,
// two blank columns a row) and, with prompt set, loads the five prompt
// sprites. After the fade from white it holds 660 frames, or 5400 with the
// prompt; after the first 120 the prompt's three sprites blink by
// Data_081a873e's levels, and A or Start ends it early, returning 1 (else 0).
s32 Func_081a729c(s32 prompt) {
  s32 blankTile = 0x1ff;
  s32 result = 0;
  struct TitleScreenWork *work = Runtime_AllocateBlock(172, 0x70);
  u8 *data;
  u16 *tilemap;
  s16 tile;
  u32 i;
  u32 j;
  u32 holdFrames;

  Scheduler_ResetTaskTable();
  Fade_ToWhite(1);
  Func_08014b70();
  VramCache_Reset();
  WaitFrames(1);
  {
    /* FAKEMATCH(pin): the native loads the control word from the pool before
     * it starts the register address; as REG_BG2CNT = 0x681, 2 of 254
     * instructions differ in tla-en. */
    register u16 control asm("r3") =
        BGCNT_PRIORITY(1) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(6);

    /* FAKEMATCH(barrier): holds r2 until the control word is loaded; as the
     * pin alone, 2 of 254 instructions differ in tla-en. */
    __asm__("" : "+l"(control) : : "r2");
    REG_BG2CNT = control;
  }
  REG_DISPCNT = DISPCNT_OBJ_1D_MAP | DISPCNT_BG2_ON | DISPCNT_OBJ_ON;
  gBgScroll[2].y = result;
  data = Resource_GetTableEntry((s32)&ResourceId_TitleScreenPicture);
  Dma_Set(data, (void *)BG_PLTT, (DMA_ENABLE | DMA_32BIT) << 16 | 0x70,
          REG_DMA3);
  {
    /* FAKEMATCH(pin): the native loads the cell buffer's address before it
     * starts the source offset; as Ram_MapCellBuffer at both uses, 2 of 254
     * instructions differ in tla-en. */
    register u8 *cells asm("r5") = Ram_MapCellBuffer;

    /* FAKEMATCH(barrier): holds the address's load ahead of the offset; as
     * the pin alone, 2 of 254 instructions differ in tla-en. */
    __asm__ __volatile__("" : "+l"(cells));
    Resource_DecodeType01(data + 0x1c0, cells);
    Dma_Set(cells, (void *)0x06006800, (DMA_ENABLE | DMA_32BIT) << 16 | 0x2580,
            REG_DMA3);
  }
  {
    /* FAKEMATCH(pin): the native loads the map's address before it starts
     * the first tile; as tilemap = (u16 *)0x06003000, 2 of 254 instructions
     * differ in tla-en. */
    register u16 *start asm("r1") = (u16 *)0x06003000;

    /* FAKEMATCH(barrier): holds r3 until the address is loaded; as the pin
     * alone, 2 of 254 instructions differ in tla-en. */
    __asm__("" : "+l"(start) : : "r3");
    tilemap = start;
  }
  tile = 0x1a0;
  i = 0;
row:
  j = 0;
column:
  {
    /* FAKEMATCH(temporary): the s16 step written as its high-half wrap, as
     * in Title_ShowSplashScreen (splash.c), with the loops as gotos, so that
     * loop.c neither hoists the step nor strength-reduces it; as
     * *tilemap++ = tile; tile++, 94 of 254 instructions differ in tla-en. */
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
  Dma_Set(gBgScroll, (void *)0x04000010, (DMA_ENABLE | DMA_32BIT) << 16 | 4,
          REG_DMA3);
  Ui_LoadWindowGraphics();
  Func_08014b70();
  if (prompt != 0) {
    u8 *buffer = Runtime_AllocateBlock(56, 0x400);
    struct TitleSprite *sprite = work->sprites;

    Resource_DecodeByteLzInRam(gTitle_PromptTiles, buffer);
    for (i = 0; i < 5; i++) {
      s32 vramBlock =
          VramCache_Load(Resource_FindFreeEntry(), 128, buffer + (i << 8) / 2);
      u32 *spriteWords = (u32 *)sprite;

      *spriteWords++ = 0;
      sprite++;
      *spriteWords++ = 0x40004000;
      *spriteWords = vramBlock;
    }
    Runtime_ReleaseHeapBlock(56);
  }
  Fade_FromWhite(30);
  Fade_Wait();
  REG_DISPCNT =
      DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON | DISPCNT_BG2_ON | DISPCNT_OBJ_ON;
  if (prompt != 0)
    holdFrames = 5400;
  else
    holdFrames = 660;
  for (i = 0; i < holdFrames; i++) {
    if (i > 120) {
      if (prompt != 0) {
        struct TitleSprite *sprite = work->sprites;
        s32 x;
        u32 level;

        for (j = 0, x = PROMPT_X; j < PROMPT_SPRITES; j++) {
          sprite->x = x;
          sprite->y = PROMPT_Y;
          Func_080140d8(sprite, 0);
          x += 32;
          sprite++;
        }
        level = Data_081a873e[i % 60];
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
    WaitFrames(1);
  }
  Runtime_ReleaseHeapBlock(172);
  {
    /* FAKEMATCH(pin): the native loads the zero from the pool before it
     * starts the register address; as REG_BLDCNT = 0 and REG_BLDALPHA = 0, 2
     * of 254 instructions differ in tla-en. */
    register u16 zero asm("r2") = 0;

    /* FAKEMATCH(barrier): holds r3 until the zero is loaded; as the pin
     * alone, 2 of 254 instructions differ in tla-en. */
    __asm__("" : "+l"(zero) : : "r3");
    REG_BLDCNT = zero;
    REG_BLDALPHA = zero;
  }
  WaitFrames(1);
  return result;
}

// An empty entry the far-call table keeps, as The Broken Seal's.
void Title_NoOpPalette(void) {}
#endif
