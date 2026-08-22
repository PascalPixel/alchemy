#include "types.h"

extern u8 *Func_020040ec(s32);
extern void Func_020040dc(void);
extern void Func_020021e8(s32, s32);
extern void Func_020020e2(s32, s32, s32, s32);
extern void Func_02001e58(s32, s32, s32, s32, s32, s32, s32, s32);
extern void Func_020041cc(s32, s32);
extern void Func_0200419e(s32, s32, s32);
extern void Func_02004124(s32);
extern void Func_02004194(s32, s32);
extern void Func_020041e6(s32, s32);
extern void Func_0200413e(s32);
extern void Func_02002142(s32, s32, s32, s32);
extern void Func_020041d0(s32, s32, s32);
extern void Func_0200415e(s32);
extern void Func_02002160(s32, s32, s32, s32);
extern void Func_020041ee(s32, s32, s32);
extern void Func_0200417c(s32);
extern void Func_0200217e(s32, s32, s32, s32);
extern void Func_0200420c(s32, s32, s32);
extern void Func_0200419a(s32);
extern void Func_02004198(s32);
extern void Func_02004202(s32, s32, s32);
extern void Func_020041be(void);
void Func_02001328(void)
{
    u8 *slot;

    slot = Func_020040ec(12);

    /* r0 still holds the record returned above. */
    Func_020040dc();

    Func_020021e8(12, 1);
    Func_020020e2(12, 536, 344, 0x70000);       /* 134 << 2, 172 << 1, 224 << 11 */

    Func_02001e58(*(s32 *)(slot + 8), *(s32 *)(slot + 12),
                  *(s32 *)(slot + 16) + 0x100000,   /* 128 << 13 */
                  0, 0, 0, 1, 0);

    Func_020041cc(12, 1);
    Func_0200419e(12, 0, 0);
    Func_02004124(20);
    Func_02004194(12, 2);
    Func_020041e6(12, 258);                     /* 129 << 1 */
    Func_0200413e(60);

    /* Three waypoints, each at height 0x30000 (192 << 10); the X literals are
     * 146 << 2, 158 << 2 and 170 << 2 and the Z is the same 172 << 1. */
    Func_02002142(12, 584, 344, 0x30000);
    Func_020041d0(0, 12, 0);
    Func_0200415e(6);

    Func_02002160(12, 632, 344, 0x30000);
    Func_020041ee(0, 12, 0);
    Func_0200417c(6);

    Func_0200217e(12, 680, 344, 0x30000);
    Func_0200420c(0, 12, 0);
    Func_0200419a(6);

    Func_02004198(0x302);
    Func_02004202(15, 0, 0);

    /* Common exit; no argument registers are set. */
    Func_020041be();
}
