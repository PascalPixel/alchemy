typedef unsigned char u8;
typedef unsigned int u32;

void Func_080a1090(void)
{
    u32 zero;
    u32 *source;
    u32 *dma;
    u8 *object;

    object = *(u8 **)0x03001f2c;
    source = &zero;
    *source = 0;
    dma = (u32 *)0x040000d4;
    dma[0] = (u32)source;
    dma[1] = (u32)object;
    dma[2] = 0x8500029c;
    object[28] = 255;
    object[30] = 1;
    object[31] = 1;
    object[274] = 1;
    object[275] = 1;
}
