#include "TYPES.H"
#include "SCENE.H"
extern u8 Data_03001c94[];

/* menu/input/cancel_sound_tick.c */
/* menu/input/cancel_sound_tick.c */
s32 Scheduler_RemoveCallback(s32);
s32 GameFlag_SetBitFar(s32);
s32 Audio_PlayCue(s32);

void Menu_EnsureCancelSound(void)
{
    if (GameFlag_TestFar(0x150) == 0) {
        Scheduler_RemoveCallback((s32)Menu_CancelSoundTick);
    }
}
