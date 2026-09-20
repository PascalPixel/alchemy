#include "DMA.H"

void *Engine_AllocateBlock(s32, s32);
s32 Engine_ScheduleCallback(void (*)(void), s32);
void Engine_BuildScrollPage(void);
void Engine_ArmScrollDma(void);

static __inline__ void WaitDma(volatile u32 *channel)
{
    while (channel[2] & 0x80000000) {
    }
}

struct ScrollWork {
    u8 data[0xf01];
    u8 mode;
    u8 unknown_f02[6];
    s32 field_f08;
    s32 field_f0c;
    s32 field_f10;
    s32 field_f14;
    s32 field_f18;
    s32 field_f1c;
};

void DisplayScroll_InitializeHBlankDma(s32 mode, s32 a, s32 b, s32 c, s32 d, s32 e, s32 f)
{
    struct ScrollWork *work;
    volatile u32 zero;

    work = Engine_AllocateBlock(34, 0xf20);
    zero = 0;
    Dma_Set(&zero, work, 0x850003c8, (volatile u32 *)0x040000d4);
    WaitDma((volatile u32 *)0x040000d4);
    work->mode = mode;
    work->field_f08 = a;
    work->field_f0c = d;
    work->field_f18 = c;
    work->field_f1c = f;
    work->field_f10 = b;
    work->field_f14 = e;
    Engine_ScheduleCallback(Engine_BuildScrollPage, 3200);
    Engine_ScheduleCallback(Engine_ArmScrollDma, 1152);
}
