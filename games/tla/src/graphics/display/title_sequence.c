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
