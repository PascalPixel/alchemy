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

  /* FAKEMATCH(volatile): gData_0300121c read as volatile; as a plain u16, 61
     of 73 instructions differ in tla-en. */

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
