// Draft, differ 40 of 78: runs the title sequence's opening pictures in turn
// (Func_081a6154's steps 0 to 2 between the pictures of Func_081a6638 and
// Func_081a6724), fading out early when a key cuts one short and running
// Func_081a6ea0 when all finish, then restores the display and the CPU-load
// flag it saved. Returns 1. Left: other: the native keeps the saved flag in
// a stack byte (sp+3, its address in r6) across the calls, as a volatile or
// address-taken local would be; the plain local here lives in r6, so the
// frame and its two byte moves are missing, and the scroll-clearing loop
// and DISPCNT store take other registers (r1/r2 against r2/r5).
#include "types.h"
#include "io_reg.h"
#include "system.h"
#include "frame.h"
#include "bg_scroll.h"
#include "palbuf.h"
#include "callback_scheduler.h"

void Func_08014b70(void);
void VramCache_Reset(void);
void Func_081a6030(void);
s32 Func_081a6154(s32 step);
s32 Func_081a6638(void);
s32 Func_081a6724(void);
void Func_081a6ea0(void);
void Ui_LoadWindowGraphics(void);

s32 Func_081a71c8(void) {
  u8 cpuLoad;
  s32 finished;
  s32 i;

  cpuLoad = gCpuLoadDisplayEnabled;
  Func_08014b70();
  VramCache_Reset();
  WaitFrames(1);
  Scheduler_ResetTaskTable();
  gRenderOamEnabled = 0;
  gCpuLoadDisplayEnabled = 0;
  Func_081a6030();
  if ((finished = Func_081a6154(0)) == 0 ||
      (finished = Func_081a6638()) == 0 ||
      (finished = Func_081a6154(1)) == 0 ||
      (finished = Func_081a6724()) == 0 ||
      (finished = Func_081a6154(2)) == 0) {
    PaletteFade_SetTarget(0, 0);
    PaletteFade_Start(10);
    WaitFrames(10);
  }
  if (finished)
    Func_081a6ea0();
  gRenderOamEnabled = 1;
  WaitFrames(1);
  for (i = 0; i < 4; i++)
    gBgScroll[i].x = gBgScroll[i].y = 0;
  REG_DISPCNT = 0;
  Ui_LoadWindowGraphics();
  Func_08014b70();
  PaletteFade_Init();
  gCpuLoadDisplayEnabled = cpuLoad;
  return 1;
}
