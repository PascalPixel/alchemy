#include "TYPES.H"
#include "SCENE.H"

void *Runtime_AllocateBlock(s32 arg0, s32 arg1);
s32 WaitFrames(s32 frames);

/* Wait (at most 300 frames) for the display work's pending flag at +0x358 to clear. */
void Event_WaitForDisplayField358Clear(void)
{
    s32 frames;
    u8 *work = *(u8 **)ADDR_03001E70;

    if (*(s16 *)((u8 *)Runtime_AllocateBlock(0x1b, 0xccc) + 0x19e) == 3) {
        frames = 0;
        if (*(s16 *)(work + 0x358) != 0) {
            do {
                WaitFrames(1);
                frames++;
            } while (frames <= 0x12b && *(s16 *)(work + 0x358) != 0);
        }
    }
}
