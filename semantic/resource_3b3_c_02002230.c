#include "types.h"

extern volatile s32 Data_03001e40;




/*
 * Complete 168-byte frame-throttled particle owner through its six-word pool.
 * All three random draws and the local eight-argument spawn remain separate.
 */
extern void Func_02004e02(s32 soundId);
extern s32 Func_02004cb2(void);
extern s32 Func_02004cc0(void);
extern s32 Func_02004cd8(void);
extern void Func_020023f6(s32 x, s32 y, s32 z, s32 kind,
                          s32 arg4, s32 arg5, u32 flags, void *descriptor);
void Func_02002230(void)
{
    u8 descriptor[40];
    s32 spread;
    s32 secondary;
    u32 draw;

    if ((Data_03001e40 & 2) != 0)
        return;
    if ((Data_03001e40 & 7) == 0)
        Func_02004e02(136);

    *(s32 *)(descriptor + 4) = 10;
    *(s32 *)(descriptor + 8) = 0x8000;
    *(s32 *)(descriptor + 12) = 0x8000;
    *(s32 *)(descriptor + 20) = 0x19999;
    *(s32 *)(descriptor + 36) = 0x020093b1;
    *(u16 *)(descriptor + 32) = (u16)(Func_02004cb2() & 0x0ffff000);
    *(s32 *)(descriptor + 16) = 0x19999;

    draw = (u32)Func_02004cc0();
    spread = -((s32)((draw * 5) >> 16) * 0x10000 + 0x60000);
    secondary = -((s32)((draw * 5) >> 16) * 0x10000 + 0x50000);
    draw = (u32)Func_02004cd8();

    Func_020023f6(0x01440000, 0x00300000, 0x00e40000, spread / 2,
                  secondary, 0, 0x014d0000, descriptor);
}
