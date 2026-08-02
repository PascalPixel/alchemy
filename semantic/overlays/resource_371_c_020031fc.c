typedef unsigned char u8;
typedef unsigned int u32;
typedef signed int s32;

extern u8 Data_0200d14c[];
extern u32 Data_03001e40;
extern u32 Func_080000f8(void);
extern u8 *Func_080090c8(s32, s32, s32, s32);
extern void Func_08009080(u8 *, s32);
extern void Func_08009098(u8 *, u8 *);
extern s32 Func_030003e0(s32, s32);
extern void Func_0808a210(s32, s32, s32, s32);

/* Emit one randomized particle and periodically nudge the camera quadrant. */
void Func_020031fc(void)
{
    s32 x = 0x17b00000 + (s32)(((Func_080000f8() * 40) & 0xffff) << 16);
    s32 z = 0x0c4c0000 + (s32)(((Func_080000f8() * 30) & 0xffff) << 16);
    u8 *particle = Func_080090c8(222, x, 0, z);

    if (particle != 0) {
        u8 *sprite = *(u8 **)(particle + 80);
        s32 scale = 0x13333 + (s32)((Func_080000f8() << 15) >> 16);

        sprite[38] = 0;
        sprite[9] = (u8)((sprite[9] & ~12) | 8);
        particle[85] = 0;
        *(s32 *)(particle + 24) = scale;
        *(s32 *)(particle + 28) = scale;
        Func_08009080(particle, 1);
        Func_08009098(particle, Data_0200d14c);
    }

    if (Func_030003e0(Data_03001e40, 3) == 0) {
        u32 quadrant = (Func_080000f8() << 2) >> 16;

        if (quadrant <= 2) {
            s32 cameraX = quadrant == 0 ? 0x17c70000 : 0x17c90000;
            s32 cameraZ = quadrant == 1 ? 0x0c670000 : 0x0c690000;

            Func_0808a210(cameraX, -1, cameraZ, 1);
        } else if (quadrant == 3) {
            Func_0808a210(0x17c70000, -1, 0x0c670000, 1);
        }
    }
}
