#include "TYPES.H"
#include "CALLBACK_SCHEDULER.H"
#include "SCENE.H"

extern u8 DisplayScroll_ArmHBlankDma;
extern u8 DisplayScroll_BuildAndSwapHBlankPage;

void DisplayScroll_DisarmHBlankDma(void)
{
    void *dma0;

    Scheduler_RemoveCallback((u32)(&DisplayScroll_ArmHBlankDma));
    Scheduler_RemoveCallback((u32)(&DisplayScroll_BuildAndSwapHBlankPage));
    dma0 = (void *)0x040000B0;
    FIELD_AT_OFFSET(dma0, volatile u16 *, 0xA) = (u16)(0xC5FF & FIELD_AT_OFFSET(dma0, volatile u16 *, 0xA));
    FIELD_AT_OFFSET(dma0, volatile u16 *, 0xA) = (u16)(0x7FFF & FIELD_AT_OFFSET(dma0, volatile u16 *, 0xA));
    (void)FIELD_AT_OFFSET(dma0, volatile u16 *, 0xA);
}
