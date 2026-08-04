typedef unsigned short u16;
typedef unsigned int u32;

struct DmaTransfer {
    u32 source;
    volatile u32 destination;
    volatile u32 control;
};

extern volatile u16 Data_04000008;

void Func_08004760(void)
{
    volatile u32 fill;
    u32 *source;
    u32 destination;
    u32 control;
    u16 display;
    volatile struct DmaTransfer *dma;

    destination = 0x06002000;
    source = &fill;
    *source = 0xf000f000;
    dma = (volatile struct DmaTransfer *)0x040000d4;
    dma->source = (u32)source;
    dma->destination = destination;
    control = 0x85000140;
    dma->control = control;
    destination = 0x03001cbc;
    *(volatile u32 *)destination = 0x06002000;
    display = 128;
    display <<= 3;
    Data_04000008 = display;
}
