typedef signed int s32;
typedef unsigned int u32;

void Func_080049e8(void)
{
    volatile s32 *counter = (volatile s32 *)0x03001cc4;

    if (*counter <= 0) {
        volatile s32 *source = (volatile s32 *)0x03001d2c;
        u32 *dma = (u32 *)0x040000d4;
        u32 descriptor_source = 0x03000350;
        u32 current_source = *source;
        u32 control = 0x8400000c;

        dma[0] = descriptor_source;
        dma[1] = current_source;
        dma[2] = control;
        *counter = *counter + 1;
        *source = *source + 48;
    }
}
