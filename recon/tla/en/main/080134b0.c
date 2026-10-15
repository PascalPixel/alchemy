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
extern struct FrameSoundState gData_03001138;
extern volatile u16 gData_0300121c;
extern u32 gFrameTick;
extern struct SoundWork *gData_03007ff0;
void Func_081c0088(void);
s32 SoundDriver_EnterFrameUpdate(void);
void IwramSoundRenderFrame(s32 flags);

void System_WaitForFrameInterrupt(void)
{
    u32 saved;
    struct SoundWork *sound;
    u32 count;

    if (gData_03001138.interruptOnly) {
        gData_0300121c &= ~1;
        while (!(gData_0300121c & 1))
            ;
    } else {
        saved = REG_IME;
        REG_IME = (u32)&REG_IME;
        gData_0300121c &= ~1;
        gData_03001138.tick = gFrameTick;
        gData_03001138.updating = 1;
        sound = gData_03007ff0;
        count = sound->transferCountdown;
        if (count <= 1)
            count = sound->transferPeriod;
        else
            count--;
        gData_03001138.transferCountdown = count;
        REG_IME = saved;
        Func_081c0088();
        if (SoundDriver_EnterFrameUpdate() == 0)
            IwramSoundRenderFrame(8);
        gData_03001138.updating = 0;
        while (!(gData_0300121c & 1))
            ;
    }
}
