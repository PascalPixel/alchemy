typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

extern volatile s32 Data_03001e40;
extern s32 Func_080000f8(void);
extern void Func_080f9010(s32 soundId);
extern void Func_0200013c(s32 x, s32 y, s32 z, s32 kind,
                          s32 arg4, s32 arg5, u32 flags, void *descriptor);

/*
 * Complete 168-byte frame-throttled particle owner through its six-word pool.
 * All three random draws and the local eight-argument spawn remain separate.
 */
void Func_02002230(void)
{
    u8 descriptor[40];
    s32 spread;
    s32 secondary;
    u32 draw;

    if ((Data_03001e40 & 2) != 0)
        return;
    if ((Data_03001e40 & 7) == 0)
        Func_080f9010(136);

    *(s32 *)(descriptor + 4) = 10;
    *(s32 *)(descriptor + 8) = 0x8000;
    *(s32 *)(descriptor + 12) = 0x8000;
    *(s32 *)(descriptor + 16) = 0x19999;
    *(s32 *)(descriptor + 20) = 0x19999;
    *(u16 *)(descriptor + 32) = (u16)(Func_080000f8() & 0x0ffff000);
    *(s32 *)(descriptor + 36) = 0x020093b1;

    draw = (u32)Func_080000f8();
    spread = -((s32)((draw * 5) >> 16) * 0x10000 + 0x60000);
    draw = (u32)Func_080000f8();
    secondary = -((s32)((draw * 5) >> 16) * 0x10000 + 0x50000);

    Func_0200013c(0x01440000, 0x00300000, 0x00e40000, spread / 2,
                  secondary, 0, 0x014d0000, descriptor);
}
