typedef signed int s32;
typedef unsigned int u32;

extern s32 Data_03001cc4;
extern s32 Data_03001d2c;

void Func_08004a5c(void)
{
    s32 value;
    u32 *dma;
    u32 destination = 0x03000350;
    u32 control = 0x8400000c;

    if (Data_03001cc4 > 0) {
        Data_03001cc4--;
        value = Data_03001d2c - 48;
        Data_03001d2c = value;
        dma = (u32 *)0x040000d4;
        dma[0] = value;
        dma[1] = destination;
        dma[2] = control;
    }
}
