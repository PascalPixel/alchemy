#include "types.h"

#define M2C_FIELD(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

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
    M2C_FIELD(dma0, volatile u16 *, 0xA) = (u16) (0xC5FF & M2C_FIELD(dma0, volatile u16 *, 0xA));
    M2C_FIELD(dma0, volatile u16 *, 0xA) = (u16) (0x7FFF & M2C_FIELD(dma0, volatile u16 *, 0xA));
    (void) M2C_FIELD(dma0, volatile u16 *, 0xA);
}
