typedef unsigned char u8;
typedef unsigned int u32;
typedef signed int s32;

extern s32 Func_08002f40(s32);
extern u8 Data_00000002[];

void Func_08002f10(void)
{
    u32 source;
    u32 *state;
    u32 *dma;

    source = Func_08002f40((s32)Data_00000002);
    state = (u32 *)0x03007804;
    dma = (u32 *)0x040000d4;
    dma[0] = source;
    dma[1] = (u32)state;
    dma[2] = 0x84000003;
    state[3] = 0;
}
