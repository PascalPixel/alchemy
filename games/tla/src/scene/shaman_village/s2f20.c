#include "shaman_village.h"

extern u16 gData_020064b0;
extern u16 gData_02006494;
extern u16 gData_020064bc;
extern s16 *gData_020064c0;
extern u16 gData_02006504;
extern u16 gData_0200649c;
extern s32 gData_020064a0;
extern s16 gData_0200515a[];
extern s16 gData_02004b58[];
extern s16 gData_02005186[];
extern s16 gData_02004b80[];
extern s16 gData_02005204[];

void Func_02002a20(void);

/// Starts the mode task: keeps the mode and its parameter, schedules
/// Func_02002a20 and picks the script the mode runs, then rewinds the
/// task's timer and position.
void Func_02002f20(s32 modeArg, s32 paramArg) {
  /* FAKEMATCH: the parameter pointer and the two pins copy the arguments into
   * r5 and r6 after both store addresses load, as the native does; as plain
   * parameters the copies tie with the second address load and come first
   * (differ 2). */
  u16 *paramPtr = &gData_02006494;
  register s32 mode asm("r5") = modeArg;
  register s32 param asm("r6") = paramArg;
  s32 handler;

  gData_020064b0 = mode;
  *paramPtr = (param << 4);

  Scheduler_AddOrUpdateCallback(Func_02002a20, 0x480);

  handler = (s32)gData_0200515a;
  if (mode == 2) {
    handler = (s32)gData_02004b58;
  } else if (mode == 4) {
    handler = (s32)gData_02005186;
  } else if (mode == 3) {
    if (param != 0) {
      handler = (s32)gData_02004b80;
    } else {
      handler = (s32)gData_02005204;
    }
  }

  gData_020064bc = 0;
  gData_020064c0 = (s16 *)handler;
  gData_02006504 = 0;
  gData_0200649c = 0;
  gData_020064a0 = 0;
}
