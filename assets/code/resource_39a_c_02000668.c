#include "types.h"

extern s32 Func_02002988(s32);
extern s32 Func_02002992(s32);
extern void Func_02002a94(s32);
extern void Func_020029ac(s32);
extern void Func_020029ba(s32);
extern void Func_0200081e(s32, s32, s32);
extern void Func_02002ab4(s32);
extern void Func_0200290a(s32);
extern void Func_0200197a(void);

void Func_02000668(void)
{
    /* movs r0,#0xc4 / lsls r0,#2 builds 0x310. */
    if (Func_02002988((s32) 0x310) != 0) {
        return;
    }
    if (Func_02002992((s32) 0x30D) != 0) {
        return;
    }

    Func_02002a94((s32) 0xF1);
    /* movs r0,#0xc2 / lsls r0,#2 builds 0x308. */
    Func_020029ac((s32) 0x308);
    Func_020029ba((s32) 0x309);
    /* movs r1,#0x30 / negs r1,r1 */
    Func_0200081e(8, -48, 0);
    Func_02002ab4((s32) 0x121);
    Func_0200290a(2);
    Func_0200197a();
}
