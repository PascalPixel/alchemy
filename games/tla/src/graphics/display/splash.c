#include "resource.h"
#include "types.h"
#include "io_reg.h"
#include "system.h"
#include "ram_buffer.h"
#include "dma.h"
#include "frame.h"
#include "input.h"
#include "bg_scroll.h"

extern char ResourceId_NintendoLogo;

void Audio_PlayCue(s32 cue);
void Scheduler_ResetTaskTable(void);
void Fade_ToBlack(s32 frames);
void Fade_FromBlack(s32 frames);
void Fade_Wait(void);
void Func_08014b70(void);
void Ui_LoadWindowGraphics(void);

/// Shows the Nintendo logo with its jingle. With mode 0 it fades in and holds
/// two seconds; otherwise it waits a second, fades in, holds three seconds and
/// fades out, faster once skipped. Returns -1 when A or Start skipped it.
// As The Broken Seal's function of the same name.
s32 Title_ShowSplashScreen(s32 mode) {
  s32 result;
  u8 *data;
  u16 *map;
  u32 x;
  u32 y;
  s32 tile;
  s32 blank;
  s32 resource;

  Audio_PlayCue(110);
  gRenderOamEnabled = 1;
  resource = (s32)&ResourceId_NintendoLogo;
  blank = 0x1ff;
  Scheduler_ResetTaskTable();
  Fade_ToBlack(1);
  Func_08014b70();
  WaitTicks(1);
  {
    /* FAKEMATCH(pin): the native loads the control word from the pool before
     * it starts the register address; as REG_BG2CNT = 0x681, 2 of 192
     * instructions differ in tla-en. */
    register u16 control asm("r3") =
        BGCNT_PRIORITY(1) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(6);

    /* FAKEMATCH(barrier): holds r2 until the control word is loaded; as the
     * pin alone, 2 of 192 instructions differ in tla-en. */
    __asm__("" : "+l"(control) : : "r2");
    REG_BG2CNT = control;
  }
  REG_DISPCNT = DISPCNT_OBJ_1D_MAP | DISPCNT_BG2_ON | DISPCNT_OBJ_ON;
  result = 0;
  gBgScroll[2].y = result;
  data = Resource_GetTableEntry(resource);
  Dma_Set(data, (void *)BG_PLTT, (DMA_ENABLE | DMA_32BIT) << 16 | 0x70,
          REG_DMA3);
  {
    /* FAKEMATCH(pin): the native loads the cell buffer's address before it
     * starts the source offset; as Ram_MapCellBuffer at both uses, 2 of 192
     * instructions differ in tla-en. */
    register u8 *cells asm("r5") = Ram_MapCellBuffer;

    /* FAKEMATCH(barrier): holds the address's load ahead of the offset; as
     * the pin alone, 2 of 192 instructions differ in tla-en. */
    __asm__ __volatile__("" : "+l"(cells));
    Resource_DecodeType01(data + 448, cells);
    Dma_Set(cells, (void *)0x06004000, (DMA_ENABLE | DMA_32BIT) << 16 | 0x2580,
            REG_DMA3);
  }
  {
    /* FAKEMATCH(pin): the native loads the map's address before it starts
     * the first tile; as map = (u16 *)0x06003000, 2 of 192 instructions
     * differ in tla-en. */
    register u16 *start asm("r1") = (u16 *)0x06003000;

    /* FAKEMATCH(barrier): holds r3 until the address is loaded; as the pin
     * alone, 2 of 192 instructions differ in tla-en. */
    __asm__("" : "+l"(start) : : "r3");
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
  Dma_Set(gBgScroll, (void *)0x04000010, (DMA_ENABLE | DMA_32BIT) << 16 | 4,
          REG_DMA3);
  Ui_LoadWindowGraphics();
  Func_08014b70();
  REG_DISPCNT =
      DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON | DISPCNT_BG2_ON | DISPCNT_OBJ_ON;
  if (mode == 0) {
    Fade_FromBlack(1);
    Fade_Wait();
    for (y = 0; y < 120; y++) {
      if (gInput.pressed & (KEY_A | KEY_START)) {
        result = -1;
        break;
      }
      WaitTicks(1);
    }
    return result;
  }
  for (y = 0; y < 60; y++) {
    if (gInput.pressed & (KEY_A | KEY_START)) {
      result = -1;
      break;
    }
    WaitTicks(1);
  }
  if (result != 0)
    Fade_FromBlack(8);
  else
    Fade_FromBlack(60);
  Fade_Wait();
  if (result == 0) {
    for (y = 0; y < 180; y++) {
      if (gInput.pressed & (KEY_A | KEY_START)) {
        result = -1;
        break;
      }
      WaitTicks(1);
    }
  }
  if (result != 0)
    Fade_ToBlack(8);
  else
    Fade_ToBlack(60);
  Fade_Wait();
  return result;
}
