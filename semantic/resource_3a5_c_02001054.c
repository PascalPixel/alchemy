typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef signed short s16;
typedef signed int s32;

extern u8 *Func_0808a080();
extern u32 Func_080000f8(void);
extern void Func_0200013c();

/* Emit an intermittent particle on either side of the active actor. */
void Func_02001054(void)
{
    s32 slot = *(s32 *)(0x02000240 + 500);
    u8 *actor = Func_0808a080(slot);
    u8 *workspace = *(u8 **)0x03001ebc;
    s32 descriptor[10] = { 0 };
    s32 lateral = 0;
    s32 random_value;

    if (*(s32 *)(actor + 56) == (s32)0x80000000)
        return;
    (*(s16 *)(0x02000240 + 0x232))++;
    if (*(s16 *)(workspace + 364) == 30)
        return;

    if (*(s32 *)(actor + 48) <= 0x10000) {
        if ((*(u32 *)0x03001e40 & 15) != 0)
            return;
        descriptor[2] = 0x20000;
        descriptor[3] = 0x20000;
        descriptor[8] = (s32)(((Func_080000f8() << 12) >> 16) + 0x3e0000);
        if (*(u16 *)(actor + 6) != 0 && *(u16 *)(actor + 6) != 0x2000)
            lateral = 0x400000 - (((*(s32 *)(actor + 16) >> 20) & 1) * 0x50000);
        Func_0200013c(*(s32 *)(actor + 8) + lateral, *(s32 *)(actor + 12),
                      *(s32 *)(actor + 16), 0, 0, 0x00880001, descriptor);
    } else {
        if ((*(u32 *)0x03001e40 & 7) != 0)
            return;
        descriptor[2] = 0x0000cccc;
        descriptor[3] = 0x0000cccc;
        descriptor[8] = (s32)(((Func_080000f8() << 12) >> 16) + 0x3e0000);
        random_value = (s32)(((Func_080000f8() * 5) >> 16) * 0x1999);
        Func_0200013c(*(s32 *)(actor + 8), *(s32 *)(actor + 12) + 0x400000,
                      *(s32 *)(actor + 16), random_value, 0, 0x00880001,
                      descriptor);
    }
}
