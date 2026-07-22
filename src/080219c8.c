typedef unsigned char u8;
typedef signed int s32;
typedef unsigned int u32;

extern u8 Data_08037280[];

void Func_08021950(void *, u8 *, s32, s32);

void Func_080219c8(s32 arg0)
{
    s32 phase = (*(u32 *)0x03001E40 >> 2) & 3;

    if (phase > 2) {
        phase = 2;
    }
    if (phase <= 0) {
        phase = 1;
    }
    phase = phase + 1;
    Func_08021950((void *)0x06000220, Data_08037280, arg0, -phase);
    Func_08021950((void *)0x06000240, Data_08037280 + 32, arg0 + 32, phase);
}
