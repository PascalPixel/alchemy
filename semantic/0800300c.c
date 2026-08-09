#include "types.h"

extern u16 Value_0000c3ff;
extern u16 Value_00001001;
extern u32 Data_03000000;
extern u32 Data_03007ffc;

struct DmaTransfer_0800300c {
    u32 source;
    u32 destination;
    u32 control;
};

void Func_0800300c(void)
{
    volatile u16 *interruptMaster = (volatile u16 *)0x04000208;
    u32 destination;
    u16 disabled;
    u16 enabled;

    disabled = 0;
    *interruptMaster = disabled;

    destination = (u32)&Data_03000000;
    {
        volatile struct DmaTransfer_0800300c *dma =
            (volatile struct DmaTransfer_0800300c *)0x040000d4;
        u32 source = 0x08000770;
        u32 control = 0x84000500;

        *dma = (struct DmaTransfer_0800300c) {
            source,
            destination,
            control,
        };
    }

    {
        volatile struct DmaTransfer_0800300c *dma =
            (volatile struct DmaTransfer_0800300c *)0x040000d4;
        u32 source = 0x08007320;
        u32 secondDestination = 0x030000e0;
        u32 control = 0x8400000e;

        Data_03007ffc = destination;
        *dma = (struct DmaTransfer_0800300c) {
            source,
            secondDestination,
            control,
        };

        *(volatile u16 *)((u8 *)dma - 208) = disabled;
    }
    {
        volatile u16 *interruptEnable = (volatile u16 *)0x04000132;

        *interruptEnable = (u16)(u32)&Value_0000c3ff;
        interruptEnable += 103;
        *interruptEnable = (u16)(u32)&Value_00001001;
    }
    enabled = 1;
    *interruptMaster = enabled;
}
