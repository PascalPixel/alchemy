#include "types.h"

#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))
#define Scheduler_RemoveCallback Func_08004278

#define DisplayScroll_ArmHBlankDma_Entry Data_080944ed
#define DisplayScroll_BuildAndSwapHBlankPage_Entry Data_08094545
#define DisplayScroll_DisarmHBlankDma Func_080947e4

void Scheduler_RemoveCallback(void *callback);
extern u8 DisplayScroll_ArmHBlankDma_Entry;
extern u8 DisplayScroll_BuildAndSwapHBlankPage_Entry;

void DisplayScroll_DisarmHBlankDma(void) {
    void *dma0;

    Scheduler_RemoveCallback(&DisplayScroll_ArmHBlankDma_Entry);
    Scheduler_RemoveCallback(&DisplayScroll_BuildAndSwapHBlankPage_Entry);
    dma0 = (void *)0x040000B0;
    FIELD_AT_OFFSET(dma0, volatile u16 *, 0xA) = (u16) (0xC5FF & FIELD_AT_OFFSET(dma0, volatile u16 *, 0xA));
    FIELD_AT_OFFSET(dma0, volatile u16 *, 0xA) = (u16) (0x7FFF & FIELD_AT_OFFSET(dma0, volatile u16 *, 0xA));
    (void) FIELD_AT_OFFSET(dma0, volatile u16 *, 0xA);
}
