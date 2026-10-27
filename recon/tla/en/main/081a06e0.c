// Draft, differ 108 of 195: the credits and slide show driver. It resets the
// scheduler, starts the scroll callback and sets up BG2, BG3 and blending;
// mode 1 cross-fades the slides until START or the scroll ends, other modes
// start the scrolling credits (mode - 2) and wait for their end. Left:
// register choice in the slide loop (the native keeps the slide counter in
// r10, the blend register in r9, the counter word in r11, the slide's parity
// in r5 then r1 and the START mask in r7; here parity takes r7 and the
// registers rotate), the native computing 16 - step with movs #16 where the
// draft pools it, and a dead load of gInput.pressed before the second
// wait's held test that no plain C here emits. The prologue, the register
// setup and the credits branch match. Tried: parity recomputed at each use
// (differ 113).
#include "types.h"
#include "io_reg.h"
#include "iowrite.h"
#include "system.h"
#include "frame.h"
#include "input.h"
#include "callback_scheduler.h"

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
void Ui_LoadWindowGraphics(void);
void Func_08014b70(void);

s32 Func_081a06e0(s32 mode) {
  s32 slide;
  s32 odd;
  s32 step;
  s32 wait;
  u32 blend;

  gRenderOamEnabled = 0;
  gCpuLoadDisplayEnabled = 0;
  gData_03001110 = 0;
  Data_02007500 = 0;
  gAutoSleepEnabled = 0;
  Scheduler_ResetTaskTable();
  Scheduler_AddOrUpdateCallback(DisplayScroll_StepPositionEveryFourVBlanks,
                                3200);
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
    WaitFrames(60);
    for (slide = 0;; slide++) {
      odd = slide & 1;
      Func_081a052c(gDisplayScroll_SlideResources[slide], odd ^ 1);
      blend = 0xf00;
      for (step = 1; step <= 16; step++) {
        if (odd)
          REG_BLDALPHA = (step << 8) | (16 - step);
        else
          REG_BLDALPHA = blend | step;
        for (wait = 0; wait <= 3; wait++) {
          if (gInput.held & 8)
            goto done;
          WaitFrames(1);
        }
        blend -= 0x100;
      }
      if (Data_02007500 == 0) {
        do {
          if (gInput.held & 8)
            goto done;
          if (gDisplayScroll_Busy == 2)
            goto done;
          WaitFrames(1);
        } while (Data_02007500 == 0);
      }
      Data_02007500--;
    }
  } else {
    Func_081a0e0c(mode - 2);
    while (gDisplayScroll_Busy <= 375)
      WaitFrames(1);
    Func_081a0fb8();
  }

done:
  Io_Write16(0, &REG_BLDCNT);
  Io_Write16(0x1040, &REG_DISPCNT);
  Ui_LoadWindowGraphics();
  Func_08014b70();
  gRenderOamEnabled = 1;
  return 0;
}
