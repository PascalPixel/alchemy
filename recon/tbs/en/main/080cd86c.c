/* Draft, not exact (2026-09-24): 206 differing halfwords, same length. Hand-written
   from the assembly beside BattleFx_EndCanvasLayer. Residual: the queued-write
   pointers are allocated r5 (queue) and r7 (IME) where the reference has r6
   and r5 with the argument in r7; the tilemap loop stores its value through a
   sign-extended copy held in r9, which every spelling tried (s16 locals, s16
   array, inline store helper) folds away. */
/* 2026-09-26 bounded follow-up: 696/696 bytes, 206 differing halfwords
   (78 aligned edits). A shared u32 zero initialised after the first queued
   write and reused for work+0x77b8, state[3] and the three zero I/O writes
   gives 207/79, not a new allocation. Baseline allocator order is queue
   pseudo 49 before bg_control 32 before IME 48: queue has 13 uses over 292
   insns, IME 11 over 304, and bg_control 5 over 86. They become r5/r6/r7
   instead of the reference's r6/r7/r5. This is a concrete priority mismatch,
   not evidence for missing C or a reason to repeat zero spellings. */
/* Battle effect: open a canvas layer over the battle scene. */
#include "TYPES.H"
#include "CALLBACK_SCHEDULER.H"
#include "IO_WRITE_QUEUE.H"

void WaitFrames(s32);
void Runtime_ApplyValueToWork7818(s32);
void BattlePresentation_ConfigurePaletteFadeFar(s32, u16, s32);
void Func_080b5028(s32, s32, s32, s32);

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

typedef s32 (*FillWordsFn)(void *destination, s32 size);

static __inline__ s32 FillWords(void *destination, s32 size)
{
    return ((FillWordsFn)0x03000164)(destination, size);
}

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

void BattleFx_OpenCanvasLayer(s32 bg_control)
{
    u8 *work = *(u8 **)(gWorkSlot + 39 * 4);
    u8 *battle = *(u8 **)(gWorkSlot + 9 * 4);
    void *canvas = *(void **)(gWorkSlot + 40 * 4);
    s32 *state = *(s32 **)(gWorkSlot + 44 * 4);
    u16 *map;
    s32 y;
    s32 x;
    s32 n;

    Runtime_ApplyValueToWork7818(bg_control);
    state[3] = 1;
    WaitFrames(1);
    *(volatile u16 *)0x04000050 = 0;
    QUEUE_DISPLAY_CONTROL(0x1741);
    gBgScroll.unk06 = 32;
    WaitFrames(1);
    BattlePresentation_ConfigurePaletteFadeFar(1, *(u16 *)(battle + 0x648), 128);
    *(s32 *)(work + 0x77b4) = 24;
    *(s32 *)(work + 0x77b8) = 0;
    Scheduler_AddOrUpdateCallback(0x080cd4b5, 0xc80);
    QUEUE_DISPLAY_CONTROL(0x1341);
    WaitFrames(1);
    *(volatile u16 *)0x0400000c = bg_control | 0x784;
    QUEUE_DISPLAY_CONTROL(0x1341);
    Func_080b5028(0, 0, 0, 100);
    state[3] = 0;
    WaitFrames(1);
    *(volatile u16 *)0x04000050 = 0x3f44;
    *(volatile u16 *)0x04000052 = 0x100e;
    *(volatile s32 *)0x04000028 = 0;
    *(volatile s32 *)0x0400002c = -0x1000;
    *(volatile u16 *)0x04000020 = 0x80;
    *(volatile u16 *)0x04000022 = 0;
    *(volatile u16 *)0x04000024 = 0;
    *(volatile u16 *)0x04000026 = 0x100;
    *(volatile u16 *)0x04000040 = 0x00f0;
    *(volatile u16 *)0x04000044 = 0x1088;
    *(volatile u16 *)0x04000042 = 0x00f0;
    *(volatile u16 *)0x04000046 = 0x1088;
    *(volatile u16 *)0x04000048 = 0x3537;
    *(volatile u16 *)0x0400004a = 0x3f21;
    QUEUE_DISPLAY_CONTROL(0x7741);
    WaitFrames(1);
    map = (u16 *)0x06003800;
    n = 0;
    for (y = 0; y != 16; y++) {
        for (x = 0; x != 8; x++) {
            map[n++] = (s16)(((y << 12) + ((x * 2 + 1) << 8)) | ((y * 8 + x) * 2));
        }
    }
    FillWords(canvas, 0x4000);
    FillWords((void *)0x06004000, 0x4000);
    WaitFrames(1);
}
