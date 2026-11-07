#include "types.h"
#include "graphics.h"
#include "window.h"
#include "system.h"
#include "resource.h"
#include "heap_release.h"
#include "dma.h"
#include "io_reg.h"
#include "edition.h"

extern u8 MsgSaving;
extern u8 MsgErasing;
extern u8 MsgCopying;
extern u8 MsgDoNotTurnThePower;

extern const u8 gWindow_ProgressBarTiles[];

struct Window *Window_Create(s32 x, s32 y, s32 width, s32 height, s32 style);
void Text_DrawMessage(s32 resource, struct Window *window, s32 x, s32 y);
void Sound_SetBgmVolumeTargetFar(s32 target, s32 step);
void Sound_PlayFar(s32 cue);
void Func_080c8698(void);

// Heading x coordinate: only French differs.
#if (LANGUAGE == LANGUAGE_FRENCH)
#define SAVING_X 40
#define ERASING_X 48
#define COPYING_X 48
#else
#define SAVING_X 76
#define ERASING_X 92
#define COPYING_X 84
#endif

/// Opens the progress screen: 0 saves, 1 erases, 2 copies.
struct Window *SaveProgress_Open(u32 kind) {
  struct Window *window;
  void *tiles;
  s32 message;

  window = Window_Create(0, 0, 30, 20, 0);
  tiles = Heap_AllocEwram(0x100);
  Resource_DecodeType01(gWindow_ProgressBarTiles, tiles);
  Dma_Set(tiles, (void *)0x06001000, DMA_COPY_WORDS(64), REG_DMA3);
  switch (kind) {
  case 0:
    Text_DrawMessage((s32)&MsgSaving, window, SAVING_X, 40);
    break;
  case 1:
    Text_DrawMessage((s32)&MsgErasing, window, ERASING_X, 40);
    break;
  case 2:
    Text_DrawMessage((s32)&MsgCopying, window, COPYING_X, 40);
    break;
  }
  message = (s32)&MsgDoNotTurnThePower;
  Text_DrawMessage(message, window, 40, 88);
  Text_DrawMessage(message + 1, window, 40, 104);
  Sound_SetBgmVolumeTargetFar(0, 30);
  Sound_PlayFar(298);
  wait(10);
  Heap_Release(tiles);
  return window;
}

/// Closes the progress screen.
struct Window *SaveProgress_Close(struct Window *window) {
  wait(10);
  Func_080c8698();
  Sound_SetBgmVolumeTargetFar(0x100, 30);
  Window_Close(window, 0);
  return window;
}
