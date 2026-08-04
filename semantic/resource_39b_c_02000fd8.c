typedef unsigned char u8;
typedef unsigned int u32;
typedef signed int s32;

extern u8 *Func_0808a080();
extern void Func_0808a018();
extern void Func_0808a330();
extern void Func_0808a348();
extern void Func_0808a010();
extern void Func_080f9010();
extern u32 Func_080000f8(void);
extern s32 Func_03000380();
extern void Func_0200013c();
extern void Func_080770c8();
extern void Func_080000d0();
extern void Func_08009180();
extern void Func_080091c0();
extern void Func_0808a020();

/* Trigger the tile-(12,32) celebration and emit its fifteen-piece burst. */
void Func_02000fd8(void)
{
    u8 *actor = Func_0808a080(0);
    s32 x = *(s32 *)(actor + 8);
    s32 z = *(s32 *)(Func_0808a080(0) + 16);
    s32 descriptor[10] = { 0 };
    s32 index;

    if (((x < 0 ? x + 0x000fffff : x) >> 20) != 12)
        return;
    if (((z < 0 ? z + 0x000fffff : z) >> 20) != 32)
        return;

    Func_0808a018();
    Func_0808a330(0x10000, 0);
    Func_0808a348(60);
    Func_0808a010(120);
    Func_0808a330(0x10005, 1);
    Func_0808a348(60);
    Func_0808a010(40);

    descriptor[0] = 1;
    descriptor[6] = 286;
    descriptor[7] = 0x0200af4c;
    for (index = 0; index <= 14; index++) {
        s32 burst_x;
        s32 burst_z;
        s32 height;

        Func_080f9010(246);
        burst_x = (208 - (s32)((Func_080000f8() << 4) >> 16)) << 16;
        burst_z = (560 - (s32)((Func_080000f8() << 4) >> 16)) << 16;
        height = Func_03000380(
            (s32)((((Func_080000f8() * 4) >> 16) * 15) << 16) + 0x3c0000,
            100);
        Func_0200013c(burst_x, 0, burst_z, 0, height, 0x00320001,
                      descriptor);
        Func_0808a010(4);
    }

    Func_080f9010(220);
    Func_0808a010(60);
    Func_080770c8(0x875);
    Func_080000d0((void *)0x02008d99, 3200);
    Func_08009180(37, 98, 10, 97, 5, 3);
    Func_080091c0(70, 32, 13, 7, 6, 32);
    Func_0808a330(0x10000, 0);
    Func_0808a348(60);
    Func_0808a010(120);
    Func_0808a020();
}
