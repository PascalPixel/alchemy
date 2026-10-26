// Draft, differ 42 of 73 (66 before, 2026-10-26): waits for the next frame
// interrupt, running the once-a-frame sound step first unless only the
// interrupt is wanted. The frame state is volatile (each store reads the byte
// first), IwramSoundRenderFrame is reached at its resident IWRAM address, and
// IME is written through a pointer to it, as the native stores the address
// register. Left: the native loads gData_03001138 afresh in the long path and
// forms the countdown in a copy (adds r3, r2, #0); here the address loaded for
// the first test is kept into that path.
#include "types.h"
#include "io_reg.h"
#include "callback_scheduler.h"
#include "audio_engine.h"

struct FrameSoundState {
  u8 tick;
  u8 transferCountdown;
  u8 interruptOnly;
  u8 updating;
};
extern volatile struct FrameSoundState gData_03001138;
extern volatile u16 gData_0300121c;
extern u32 gFrameTick;
extern struct SoundWork *gData_03007ff0;
void Func_081c0088(void);
s32 SoundDriver_EnterFrameUpdate(void);
#define IwramSoundRenderFrame FIXED_ADDRESS((void (*)(s32 flags)), 0x03000d40, IwramSoundRenderFrame)

void System_WaitForFrameInterrupt(void) {
  u32 saved;
  volatile u16 *ime;
  struct SoundWork *sound;
  u32 count;

  if (gData_03001138.interruptOnly) {
    gData_0300121c &= ~1;
    while (!(gData_0300121c & 1))
      ;
  } else {
    ime = &REG_IME;
    saved = *ime;
    *ime = (u32)ime;
    gData_0300121c &= ~1;
    gData_03001138.tick = gFrameTick;
    gData_03001138.updating = 1;
    sound = gData_03007ff0;
    count = sound->transferCountdown;
    gData_03001138.transferCountdown =
        count <= 1 ? sound->transferPeriod : sound->transferCountdown - 1;
    REG_IME = saved;
    Func_081c0088();
    if (SoundDriver_EnterFrameUpdate() == 0)
      IwramSoundRenderFrame(8);
    gData_03001138.updating = 0;
    while (!(gData_0300121c & 1))
      ;
  }
}
