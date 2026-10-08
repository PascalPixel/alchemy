#include "types.h"
#include "io_reg.h"
#include "callback_scheduler.h"
#include "audio_engine.h"

struct FrameSoundState {
    u8 tick;
    u8 transfer_countdown;
    u8 interrupt_only;
    u8 updating;
};
extern struct FrameSoundState Data_03001138;
extern volatile u16 Data_0300121c;
extern u32 gFrameTick;
extern struct SoundWork *Data_03007ff0;
void Func_081c0088(void);
s32 SoundDriver_EnterFrameUpdate(void);
void IwramSoundRenderFrame(s32 flags);

void System_WaitForFrameInterrupt(void)
{
    u32 saved;
    struct SoundWork *sound;
    u32 count;

    if (Data_03001138.interrupt_only) {
        Data_0300121c &= ~1;
        while (!(Data_0300121c & 1))
            ;
    } else {
        saved = REG_IME;
        REG_IME = (u32)&REG_IME;
        Data_0300121c &= ~1;
        Data_03001138.tick = gFrameTick;
        Data_03001138.updating = 1;
        sound = Data_03007ff0;
        count = sound->transfer_countdown;
        if (count <= 1)
            count = sound->transfer_period;
        else
            count--;
        Data_03001138.transfer_countdown = count;
        REG_IME = saved;
        Func_081c0088();
        if (SoundDriver_EnterFrameUpdate() == 0)
            IwramSoundRenderFrame(8);
        Data_03001138.updating = 0;
        while (!(Data_0300121c & 1))
            ;
    }
}
