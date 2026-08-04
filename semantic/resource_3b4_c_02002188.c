#include "types.h"

extern void Func_080000c0(s32 frames);
extern void Func_020020c4(s32 actor, s32 frame);
extern void Func_02000f40(s32 slot);
extern void Func_02000fdc(s32 slot);
extern s32 Func_080770c0(s32 flag);
extern void Func_0808a0f0(s32 actor, s32 x, s32 y);

/* Install actors 8..12, positioning only those whose story gates are clear. */
void Func_02002188(void)
{
    Func_080000c0(1);
    Func_020020c4(12, 243);
    Func_020020c4(11, 244);
    Func_020020c4(10, 244);
    Func_020020c4(9, 244);
    Func_020020c4(8, 244);
    if (Func_080770c0(0x0ee7) == 0) Func_0808a0f0(8, 0x00e80000, 0x03680000);
    if (Func_080770c0(0x0ee8) == 0) Func_0808a0f0(9, 0x01280000, 0x03380000);
    if (Func_080770c0(0x0ee9) == 0) Func_0808a0f0(10, 0x01480000, 0x02f80000);
    if (Func_080770c0(0x0eea) == 0) Func_0808a0f0(11, 0x01680000, 0x03680000);
    if (Func_080770c0(0x09c0) != 0) Func_02000f40(0);
    if (Func_080770c0(0x09c1) != 0) Func_02000f40(1);
    if (Func_080770c0(0x09c2) != 0) Func_02000f40(2);
    if (Func_080770c0(0x09c3) != 0) Func_02000f40(3);
    if (Func_080770c0(0x09c4) != 0) Func_02000fdc(0);
}
