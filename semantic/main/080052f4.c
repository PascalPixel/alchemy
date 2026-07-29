typedef unsigned char u8;
typedef unsigned int u32;
typedef signed int s32;

void Func_08007318(s32, s32);

extern u8 Data_08002298;
extern u8 Value_00000054;

void Func_080052f4(s32 first, s32 second)
{
    u32 words = (u32)&Value_00000054 >> 2;

    {
        u32 buffer[words];
        u32 *dma = (u32 *)0x040000d4;
        u32 source = (u32)&Data_08002298;
        u32 destination = (u32)buffer;
        u32 control = 0x84000000 | words;

        dma[0] = source;
        dma[1] = destination;
        dma[2] = control;
        Func_08007318(first, second);
    }
}
