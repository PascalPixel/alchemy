/* Battle effect: open a canvas layer over the battle scene. The variant of
   BattleFx_BeginCanvasLayer that fades the battle palette only to level 128,
   starts the palette fade transfer at 24 frames and queues the display
   without the windows until the canvas is set up; it lays out the same 16 by
   16 canvas tile map (two tile numbers per halfword) and clears the canvas
   buffers.

   FAKEMATCH: each queued display-control write is QueueIoWriteDelay2
   (SYSTEM/IO_WRITE_QUEUE.C) written out inline with that function's odd
   constructs, the one-pass loop around the IME read and the count stored
   through an explicit u16 pointer; the queue and IME pointers are held for
   the whole function as the ROM keeps them. */
#include "TYPES.H"
#include "IWRAM_CALL.H"
#include "SYSTEM.H"
#include "CALLBACK_SCHEDULER.H"
#include "IO_WRITE_QUEUE.H"
#include "IO_REG.H"

void Runtime_ApplyValueToWork7818(void);
void BattlePresentation_ConfigurePaletteFadeFar(s32, u16, s32);
void Func_080b5028(s32, s32, s32, s32);
void Palette_StepFadeTransfer(void);

extern u8 gWorkSlot[];

struct Cells03001ad0 {
    u16 unk00;
    u16 unk02;
    u16 unk04;
    u16 unk06;
};
extern struct Cells03001ad0 gBgScroll;

typedef s32 (*ClearWordsFn)(void *destination, s32 size);

static __inline__ void ClearWords(ClearWordsFn clear, void *destination, s32 size)
{
    clear(destination, size);
}

#define QUEUE_DISPLAY_CONTROL(control) do {                                 \
        u32 saved;                                                          \
        s32 count;                                                          \
                                                                            \
        do {                                                                \
            saved = *ime;                                                   \
        } while (0);                                                        \
        *ime = (u16)ime;                                                    \
        count = q->count;                                                   \
        if (count <= 31) {                                                  \
            u32 *destination = (u32 *)((u8 *)q + count * 12 + 4);           \
            *(u16 *)&q->count = count + 1;                                  \
            *destination++ = (control);                                     \
            *destination++ = 0x04000000;                                    \
            *destination = 0x20000;                                         \
        }                                                                   \
        *ime = saved;                                                       \
    } while (0)

void BattleFx_OpenCanvasLayer(s32 bg_control)
{
    void **cache = (void **)(gWorkSlot + 39 * 4);
    u8 *work = cache[0];
    u8 *battle = *(u8 **)(gWorkSlot + 9 * 4);
    void *canvas = cache[1];
    u8 *display = cache[5];
    volatile u16 *ime;
    struct IoWriteQueue *q;
    s32 row;
    s32 col;
    s32 offset;

    Runtime_ApplyValueToWork7818();
    *(s32 *)(display + 12) = 1;
    WaitFrames(1);
    *(volatile u16 *)0x04000050 = 0;
    q = &gIoWriteQueue;
    ime = &REG_IME;
    QUEUE_DISPLAY_CONTROL(0x1741);
    gBgScroll.unk06 = 32;
    WaitFrames(1);
    BattlePresentation_ConfigurePaletteFadeFar(1, *(u16 *)(battle + 0x648), 128);
    *(s32 *)(work + 0x77b4) = 24;
    *(s32 *)(work + 0x77b8) = 0;
    Scheduler_AddOrUpdateCallback((s32)Palette_StepFadeTransfer, 0xc80);
    QUEUE_DISPLAY_CONTROL(0x1341);
    WaitFrames(1);
    *(volatile u16 *)0x0400000c = bg_control | 0x784;
    QUEUE_DISPLAY_CONTROL(0x1341);
    Func_080b5028(0, 0, 0, 100);
    *(s32 *)(display + 12) = 0;
    WaitFrames(1);

    *(volatile u16 *)0x04000050 = 0x3f44;
    *(volatile u16 *)0x04000052 = 0x100e;
    *(volatile u32 *)0x04000028 = 0;
    *(volatile s32 *)0x0400002c = -0x1000;
    *(volatile u16 *)0x04000020 = 0x80;
    *(volatile u16 *)0x04000022 = 0;
    *(volatile u16 *)0x04000024 = 0;
    *(volatile u16 *)0x04000026 = 0x100;
    *(volatile u16 *)0x04000040 = 0xf0;
    *(volatile u16 *)0x04000044 = 0x1088;
    *(volatile u16 *)0x04000042 = 0xf0;
    *(volatile u16 *)0x04000046 = 0x1088;
    *(volatile u16 *)0x04000048 = 0x3537;
    *(volatile u16 *)0x0400004a = 0x3f21;
    QUEUE_DISPLAY_CONTROL(0x7741);
    WaitFrames(1);

    offset = 0;
    for (row = 0; row != 16; row++) {
        for (col = 0; col != 8; col++, offset += 2) {
            s32 tile;
            s16 pair;

            tile = row * 16 + col * 2;
            pair = ((tile + 1) << 8) | tile;
            *(volatile s16 *)(0x06003800 + offset) = pair;
        }
    }

    ClearWords(Iwram_ClearWords, canvas, 0x4000);
    ClearWords(Iwram_ClearWords, (void *)0x06004000, 0x4000);
    WaitFrames(1);
}
