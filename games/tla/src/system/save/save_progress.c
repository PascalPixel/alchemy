#include "types.h"
#include "graphics.h"
#include "window.h"
#include "system.h"
#include "resource.h"
#include "mem.h"
#include "dma.h"
#include "io_reg.h"
#include "edition.h"

extern u8 MsgSaving;
extern u8 MsgErasing;
extern u8 MsgCopying;
extern u8 MsgDoNotTurnThePower;

extern const u8 gUi_ProgressBarTiles[];

struct UiWindow *UiWindow_Create(s32 x, s32 y, s32 width, s32 height,
                                 s32 style);
void UiText_DrawResource(s32 resource, struct UiWindow *window, s32 x, s32 y);
void Audio_SetMusicVolumeTargetAndStepFar(s32 target, s32 step);
void Audio_PlayCue(s32 cue);
#if (LANGUAGE == LANGUAGE_SPANISH) || (LANGUAGE == LANGUAGE_ITALIAN)
// Spanish and Italian far-call tables name the entry to the same routine
// differently.
void Func_080c8688(void);
#define Func_080c8698 Func_080c8688
#else
void Func_080c8698(void);
#endif

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

// Open the progress screen: 0 saves, 1 erases, 2 copies.
struct UiWindow *SaveProgress_Open(u32 kind) {
  struct UiWindow *window;
  void *tiles;
  s32 message;

  window = UiWindow_Create(0, 0, 30, 20, 0);
  tiles = Runtime_BumpAllocateAlternatePool(0x100);
  Resource_DecodeType01(gUi_ProgressBarTiles, tiles);
  Dma_Set(tiles, (void *)0x06001000, 0x84000040, REG_DMA3);
  switch (kind) {
  case 0:
    UiText_DrawResource((s32)&MsgSaving, window, SAVING_X, 40);
    break;
  case 1:
    UiText_DrawResource((s32)&MsgErasing, window, ERASING_X, 40);
    break;
  case 2:
    UiText_DrawResource((s32)&MsgCopying, window, COPYING_X, 40);
    break;
  }
  message = (s32)&MsgDoNotTurnThePower;
  UiText_DrawResource(message, window, 40, 88);
  UiText_DrawResource(message + 1, window, 40, 104);
  Audio_SetMusicVolumeTargetAndStepFar(0, 30);
  Audio_PlayCue(298);
  WaitFrames(10);
  MemFree(tiles);
  return window;
}

// Close the progress screen.
struct UiWindow *SaveProgress_Close(struct UiWindow *window) {
  WaitFrames(10);
  Func_080c8698();
  Audio_SetMusicVolumeTargetAndStepFar(0x100, 30);
  UiWork_Finalize(window, 0);
  return window;
}
