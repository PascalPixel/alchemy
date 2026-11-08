#include "types.h"
#include "io_reg.h"
#include "io_write.h"
#include "system.h"
#include "frame.h"
#include "input.h"
#include "task.h"

extern s16 Data_02007500;
extern u8 gData_03001110;
extern s16 gDisplayScroll_Busy;
extern const void *gDisplayScroll_SlideResources[];

void DisplayScroll_StepPositionEveryFourVBlanks(void);
void DisplayScroll_BuildHblankWordTable(u32 *table);
void Graphics_ClearCharacterBlockAndPalette(s32 block);
void Func_081a0a70(void);
void Func_081a052c(const void *resource, s32 layer);
void Func_081a0e0c(s32 credits);
void Func_081a0fb8(void);
void Window_LoadGraphics(void);
void Func_08014b70(void);

/// The credits and slide show driver. It resets the task table, starts the
/// scroll task and sets up BG2, BG3 and blending; mode 1 cross-fades the slides
/// until START or the scroll ends, other modes start the scrolling credits
/// (mode - 2) and wait for their end.
s32 Func_081a06e0(s32 mode) {
  s32 slide;
  s32 step;
  s32 frame;

  gRenderOamEnabled = 0;
  gCpuLoadDisplayEnabled = 0;
  gData_03001110 = 0;
  Data_02007500 = 0;
  gAutoSleepEnabled = 0;
  Task_ResetAll();
  Task_Add(DisplayScroll_StepPositionEveryFourVBlanks, TASK_PASS_FRAME);
  Io_Write16(0x40, &REG_DISPCNT);
  DisplayScroll_BuildHblankWordTable((u32 *)0x06007800);
  DisplayScroll_BuildHblankWordTable((u32 *)0x0600f800);
  Graphics_ClearCharacterBlockAndPalette(0);
  Graphics_ClearCharacterBlockAndPalette(1);
  Io_Write16(0x1f8a, &REG_BG2CNT);
  Io_Write16(0x0f83, &REG_BG3CNT);
  Io_Write16(0x1c40, &REG_DISPCNT);
  Io_Write16(0x2844, &REG_BLDCNT);

  if (mode == 1) {

    Func_081a0a70();
    wait(60);
    for (slide = 0;; slide++) {
      s16 *count = &Data_02007500;

      Func_081a052c(gDisplayScroll_SlideResources[slide], (slide & 1) ^ 1);
      for (step = 1; step <= 16; step++) {
        if (slide & 1) {
          s32 t = 16;
          s32 hi = step << 8;
          s32 r = t - step;
          REG_BLDALPHA = hi | r;
        } else
          REG_BLDALPHA = ((16 - step) << 8) | step;
        for (frame = 0; frame <= 3; frame++) {
          if (gInput.held & 8)
            goto done;
          wait(1);
        }
      }
      if (*count == 0) {
        do {
          gInput.pressed;
          if (gInput.held & 8)
            goto done;
          if (gDisplayScroll_Busy == 2)
            goto done;
          wait(1);
        } while (Data_02007500 == 0);
      }
      Data_02007500--;
    }
  } else {
    Func_081a0e0c(mode - 2);
    while (gDisplayScroll_Busy <= 375)
      wait(1);
    Func_081a0fb8();
  }

done:
  Io_Write16(0, &REG_BLDCNT);
  Io_Write16(0x1040, &REG_DISPCNT);
  Window_LoadGraphics();
  Func_08014b70();
  gRenderOamEnabled = 1;
  return 0;
}
