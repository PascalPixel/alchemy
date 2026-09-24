/* Battle presentation: set up the effect display. Windows 0 and 1 cover the
   screen, blending starts from a clean slate, and the display control write
   (mode 1, BG0/BG1/BG2 and objects) is queued for the next frame; then wait
   one frame for it to land.

   FAKEMATCH: the queued write is QueueIoWriteDelay2 (SYSTEM/IO_WRITE_QUEUE.C)
   written out inline with that function's two odd constructs: the loop that
   runs once around the IME read and the count stored through an explicit u16
   pointer. */
#include "TYPES.H"
#include "IO_WRITE_QUEUE.H"

void WaitFrames(s32 frames);

extern volatile u16 Data_04000208;
#define REG_IME Data_04000208

void BattlePres_ConfigureEffectDisplay(void)
{
    volatile u16 *ime;
    struct IoWriteQueue *q;
    u32 saved;
    s32 count;

    *(volatile u16 *)0x04000050 = 0;
    *(volatile u16 *)0x04000052 = 0x100e;
    *(volatile u16 *)0x04000040 = 0x00f0;
    *(volatile u16 *)0x04000044 = 0x1088;
    *(volatile u16 *)0x04000042 = 0x00f0;
    *(volatile u16 *)0x04000046 = 0x1088;
    *(volatile u16 *)0x04000048 = 0x3537;
    *(volatile u16 *)0x0400004a = 0x3f21;

    q = &gIoWriteQueue;
    do {
        ime = &REG_IME;
        saved = *ime;
    } while (0);
    *ime = (u16)ime;
    count = q->count;
    if (count <= 31) {
        u32 *destination = (u32 *)((u8 *)q + count * 12 + 4);
        *(u16 *)&q->count = count + 1;
        *destination++ = 0x7741;
        *destination++ = 0x04000000;
        *destination = 0x20000;
    }
    *ime = saved;

    WaitFrames(1);
}
