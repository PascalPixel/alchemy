#include "resource.h"
#include "types.h"
#include "io_reg.h"
#include "system.h"
#include "ram_buffer.h"

struct BgScroll {
  s16 x;
  s16 y;
};

extern u8 gOamCopyEnabled;
extern struct BgScroll gBgScroll[4];
extern volatile u32 gKeyState;

void Audio_PlayCue(s32 cue);
void Scheduler_ResetTaskTable(void);
void Fade_ToBlack(s32 frames);
void Fade_FromBlack(s32 frames);
void Fade_Wait(void);
void DebugText_Clear(void);
void Ui_LoadWindowGraphics(void);

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
  gOamCopyEnabled = 1;
  resource = (s32)&ResourceId_NintendoLogo;
  blank = 0x1ff;
  Scheduler_ResetTaskTable();
  Fade_ToBlack(1);
  DebugText_Clear();
  WaitFrames(1);
  *(volatile u16 *)0x0400000c = 0x681;
  *(volatile u16 *)0x04000000 = 0x1440;
  result = 0;
  gBgScroll[2].y = result;
  data = Resource_GetTableEntry(resource);
  Dma_Set(data, (void *)0x05000000, 0x84000070, (volatile u32 *)0x040000d4);
  Resource_DecodeType01(data + 448, Ram_MapCellBuffer);
  Dma_Set(Ram_MapCellBuffer, (void *)0x06004000, 0x84002580,
          (volatile u32 *)0x040000d4);
  tile = 256;
  map = (u16 *)0x06003000;
  y = 0;
col:
  {
    x = 0;
  row:
    {
      s32 old = tile;

      /* FAKEMATCH: keep the signed tile wrap in the high half. */
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
  Dma_Set(gBgScroll, (void *)0x04000010, 0x84000004,
          (volatile u32 *)0x040000d4);
  Ui_LoadWindowGraphics();
  DebugText_Clear();
  *(volatile u16 *)0x04000000 = 0x1540;
  if (mode == 0) {
    Fade_FromBlack(1);
    Fade_Wait();
    for (y = 0; y < 120; y++) {
      if (gKeyState & (KEY_A | KEY_START)) {
        result = -1;
        break;
      }
      WaitFrames(1);
    }
    return result;
  }
  for (y = 0; y < 60; y++) {
    if (gKeyState & (KEY_A | KEY_START)) {
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
      if (gKeyState & (KEY_A | KEY_START)) {
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
