#include "types.h"
#include "scene.h"
#include "abi/shop/draw/item_page.h"

/* Main-image symbols: every pool word inside the ROM or the work RAM. */
extern u8 gVal[];
extern u8 gVal2[];
extern u8 gIw[];

s32 Sys_Run2(s32 a0, s32 a1, s32 a2)
{
    u32 i;
    s32 p10;
    s32 p10b;
    s32 p11;
    s32 p8;
    s32 p9;
    s32 p9b;
    s32 rec5;
    s32 record;
    s32 r10;
    s32 v10;
    s32 v5;
    s32 base6_0;
    s32 v6;
    s32 base5_af7;

    p9 = *(s32 *)gIw;
    rec5 = Sys_Check(*(u8 *)((0x21a + p9)));
    Sys_Run3(a0, 128, 8, 224, 96);
    p8 = ((*(s32 *)(a2 + 8) << 2) + *(s32 *)(a2 + 8));
    p10 = ((u32)(s32)((s32)(*(s32 *)(a2 + 20) - (s32)p8) << 24) >> 24);
    v10 = p10;
    if ((u32)p10 > 5) {
        v10 = 5;
    }
    Sys_Run4(5, p8, a0, 119, 52);
    Sys_Run5(a0, *(s32 *)(a2 + 20), 5, *(s32 *)(a2 + 8), 28);
    if (*(u8 *)((0x218 + p9)) == 0) {
        Sys_Run(0xad7, a0, 120, 8);
        v6 = r10;
    } else {
        v6 = 0;
        base6_0 = 0;
        /* The entry guard compares the count variable, not a literal zero:
           folded to a literal it becomes `!= 0` and a `beq`, where the
           original keeps the unsigned `bls`. */
        if ((u32)v10 > base6_0) {
            v5 = ((s32)(((s32)p8 << 1) + p9) + 0x1c8);
            do {
                Sys_Run6(((0x1ff & *(u16 *)(v5)) + (s32)gVal), a0, 128, ((base6_0 << 4) + 8));
                base6_0 = ((u32)((base6_0 + 1) << 24) >> 24);
                v5 = (v5 + 2);
            } while ((u32)v10 > base6_0);
            v6 = base6_0;
        }
    }
    Sys_Run7(rec5, a0, 40, 0);
    base5_af7 = (s32)gVal2;
    Sys_Run6(base5_af7, a0, 32, 16);
    Sys_Run6((base5_af7 + 1), a0, 32, 24);
    Sys_Run8(*(u16 *)(rec5 + 60), 3, a0, 80, 16);
    Sys_Run8(*(u16 *)(rec5 + 62), 3, a0, 80, 24);
    p9b = base5_af7 + 1;
    p10b = v6;
    p11 = a0;
    return 1;
}
