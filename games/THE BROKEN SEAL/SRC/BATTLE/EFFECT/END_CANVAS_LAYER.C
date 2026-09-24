/* Battle effect: close the canvas layer an effect opened. Restore the scroll
   pair saved in the work block, recentre the projection, clear the canvas
   tiles, stop the canvas transfer callback, switch the display back through
   the next-frame queue, and fade the battle palette back in over eight
   frames.

   FAKEMATCH: each queued display-control write is QueueIoWriteDelay2
   (SYSTEM/IO_WRITE_QUEUE.C) written out inline with that function's odd
   constructs, the one-pass loop around the IME read and the count stored
   through an explicit u16 pointer, plus a one-pass loop around everything
   after the queue pointer so the queue literal loads ahead of the store
   before it. */
#include "TYPES.H"
#include "CALLBACK_SCHEDULER.H"
#include "IO_WRITE_QUEUE.H"

void Audio_PlayCue(s32);
void WaitFrames(s32);
void BattlePresentation_ConfigurePaletteFadeFar(s32, u16, s32);
void Func_080b5048(u16, s32);

extern u8 gWorkSlot[];
extern volatile u16 Data_04000208;
#define REG_IME Data_04000208

struct Cells03001ad0 {
    u16 unk00;
    u16 unk02;
    u16 unk04;
    u16 unk06;
};
extern struct Cells03001ad0 gBgScroll;

struct Cells03001ce0 {
    s32 unk00[4];
    s32 unk10;
};
extern struct Cells03001ce0 gProjection;

typedef s32 (*FillWordsFn)(void *destination, s32 size);

#define QUEUE_DISPLAY_CONTROL(control) {                                    \
        volatile u16 *ime;                                                  \
        struct IoWriteQueue *q;                                             \
        u32 saved;                                                          \
        s32 count;                                                          \
                                                                            \
        q = &gIoWriteQueue;                                                 \
        do {                                                                \
            do {                                                            \
                ime = &REG_IME;                                             \
                saved = *ime;                                               \
            } while (0);                                                    \
            *ime = (u16)ime;                                                \
            count = q->count;                                               \
            if (count <= 31) {                                              \
                u32 *destination = (u32 *)((u8 *)q + count * 12 + 4);       \
                *(u16 *)&q->count = count + 1;                              \
                *destination++ = (control);                                 \
                *destination++ = 0x04000000;                                \
                *destination = 0x20000;                                     \
            }                                                               \
            *ime = saved;                                                   \
        } while (0);                                                        \
    }

void BattleFx_EndCanvasLayer(void)
{
    struct Cells03001ad0 *scroll;
    u8 *work = *(u8 **)(gWorkSlot + 39 * 4);
    u8 *battle = *(u8 **)(gWorkSlot + 9 * 4);
    s32 i;

    Audio_PlayCue(0x121);
    scroll = &gBgScroll;
    scroll->unk04 = *(s32 *)(work + 0x77a0);
    scroll->unk06 = *(s32 *)(work + 0x77a4);
    gProjection.unk00[3] = 120;
    gProjection.unk10 = 120;
    *(volatile u16 *)0x0400000c = 0x787;
    ((FillWordsFn)0x03000164)((void *)0x06004000, 0x4000);
    Scheduler_RemoveCallback(0x080cd4b5);
    scroll->unk06 = 32;
    QUEUE_DISPLAY_CONTROL(0x7341);
    *(volatile u16 *)0x04000050 = 0;
    WaitFrames(1);

    BattlePresentation_ConfigurePaletteFadeFar(2, *(u16 *)(battle + 0x648), 7);
    WaitFrames(1);
    for (i = 0; i != 8; i++) {
        Func_080b5048(*(u16 *)(battle + 0x648), 21 - i * 3);
        WaitFrames(1);
    }

    QUEUE_DISPLAY_CONTROL(0x7541);
    WaitFrames(1);
}
