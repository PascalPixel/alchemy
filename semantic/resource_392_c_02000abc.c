typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
/* Protected-window palette adjustment owner, 0x02000abc-0x02000b23. */
void Func_02000b8c(); u16 Func_02000b24(u16, s32); void Func_02000bcc();
void Func_02000bac(); void Func_0808a330();
void Func_02000abc(s32 adjustment)
{
    volatile u16 *palette = (volatile u16 *)0x05000000;
    u32 phase;
    Func_02000b8c();
    for (phase = 0; phase <= 0x00df0000; phase += 0x10000) {
        u32 index = phase >> 16;
        u32 secondWindow = (index + 0xff3f) << 16;
        if ((u32)(phase + 0xffef0000) <= 0x60000 || secondWindow <= 0x70000)
            continue;
        palette[index] = Func_02000b24(palette[index], adjustment);
    }
    Func_02000bcc(); Func_02000bac(); Func_0808a330(0x10000, 0);
}
