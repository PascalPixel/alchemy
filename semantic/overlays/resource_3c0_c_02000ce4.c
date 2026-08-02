typedef unsigned short u16;
typedef unsigned int u32;

/* Pulse the encounter palette entry through a symmetric 64-frame ramp. */
void Func_02000ce4(void)
{
    u32 phase = *(volatile u32 *)0x03001e40 & 63;
    u32 shade;

    if (phase > 31)
        phase = 64 - phase;
    shade = (phase >> 1) + 7;
    *(volatile u16 *)0x0500019e =
        (u16)(shade | (shade << 5) | (shade << 10));
}
