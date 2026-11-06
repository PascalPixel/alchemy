// Matches: differ 0 of 73 (tla-en, 2026-11-06).
// Waits for the next frame interrupt, running the once-a-frame sound step
// first unless unk02 asks for the wait alone. The state is volatile (each
// store reads the byte first), as Func_0801399c and f1c0cac.c declare it.
// The long path takes IME through IRQ_LOCK (irq_lock.h). The short path's
// wait is written for (;;) with return: as a while loop, the long path reuses
// the state's address from the first test (differ 22 of 73). sound is the
// tree's u8 view of gData_03007ff0: byte 4 is read twice in C and once in the
// code, as the native compares a copy of it. gData_0300121c is volatile here,
// as its wait loops reread it.
#include "types.h"
#include "io_reg.h"
#include "irq_lock.h"
#include "task.h"
#include "sound_driver.h"

struct SoundVBlankState {
  u8 lastVBlank;
  u8 unk01;
  u8 unk02;
  u8 unk03;
};
extern volatile struct SoundVBlankState gData_03001138;
extern volatile u16 gData_0300121c;
extern u32 gVBlankCount;
extern u8 *gData_03007ff0;
void Func_081c0088(void);
s32 SoundDriver_EnterVBlankUpdate(void);
void IwramSoundRenderVBlank(s32) __attribute__((long_call));

void System_WaitForFrameInterrupt(void) {
  u32 saved;
  u8 *sound;

  if (gData_03001138.unk02) {
    gData_0300121c &= ~1;
    for (;;)
      if (gData_0300121c & 1)
        return;
  } else {
    IRQ_LOCK(saved);
    gData_0300121c &= ~1;
    gData_03001138.lastVBlank = gVBlankCount;
    gData_03001138.unk03 = 1;
    sound = gData_03007ff0;
    if (sound[4] <= 1)
      gData_03001138.unk01 = sound[11];
    else
      gData_03001138.unk01 = sound[4] - 1;
    REG_IME = saved;
    Func_081c0088();
    if (SoundDriver_EnterVBlankUpdate() == 0) {
      void (*render)(s32) = IwramSoundRenderVBlank;

      render(8);
    }
    gData_03001138.unk03 = 0;
    while (!(gData_0300121c & 1))
      ;
  }
}
