typedef unsigned char u8;
typedef signed int s32;
typedef unsigned int u32;

extern u8 *Func_0808a080(s32);
extern u32 Func_080000f8(void);
extern s32 Func_08000120(s32);
extern s32 Func_08000118(s32);
extern void Func_0200013c(s32, s32, s32, s32, s32, s32, s32, s32);

void Func_020012a4(void)
{
    u8 *leader = Func_0808a080(0);

    if ((*(u32 *)0x03001e40 & 15) == 0) {
        s32 angle = (s32)((((52 * Func_080000f8()) >> 16) << 6) + 230);
        s32 x_velocity = Func_08000120(angle) / 4;
        s32 z_velocity = Func_08000118(angle) / 2;

        Func_0200013c(*(s32 *)(leader + 8), *(s32 *)(leader + 12),
                      *(s32 *)(leader + 16), x_velocity,
                      0, z_velocity, 0, 0);
    }
}
