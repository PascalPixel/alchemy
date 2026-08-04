#include "types.h"

extern u8 *Func_02003f14(s32);
extern void Func_02003f02(void);
extern void Func_02001c7a(s32, s32, s32, s32, s32, s32, s32, s32);
extern void Func_02003fe6(void);
/* 0x02003fb6 serves the two-argument presentation select and, twice later,
 * one-argument imports; the `_b` alias carries the two-argument one. */
extern void Func_0200200e(s32, s32);
extern void Func_02001f04(s32, s32, s32, s32);
extern void Func_02003fee(s32, s32);
extern void Func_02003fc0(s32, s32, s32);
extern void Func_02003f46(s32);
extern void Func_02003fb6_b(s32, s32);
extern void Func_0200400a(s32, s32);
extern void Func_02003f62(s32);
extern void Func_02001f62(s32, s32, s32, s32);
extern void Func_02003ff0(s32, s32, s32);
extern void Func_02003f7e(s32);
extern void Func_02001f7e(s32, s32, s32, s32);
extern void Func_0200400c(s32, s32, s32);
extern void Func_02003f9a(s32);
extern void Func_02001f9a(s32, s32, s32, s32);
extern void Func_02004028(s32, s32, s32);
extern void Func_02003fb6(s32);
extern void Func_02004020(s32, s32, s32);
extern void Func_0200402a(s32, s32, s32);
void Func_02001150(void)
{
    u8 *slot;

    slot = Func_02003f14(10);

    /* r0 still holds the record returned above. */
    Func_02003f02();

    Func_0200200e(10, 1);
    Func_02001f04(10, 88, 120, 0x60000);        /* 192 << 11 */

    Func_02001c7a(*(s32 *)(slot + 8), *(s32 *)(slot + 12),
                  *(s32 *)(slot + 16) + 0x180000,   /* 192 << 13 */
                  0, 0, 0, 1, 0);

    Func_02003fee(10, 1);
    Func_02003fc0(10, 0, 0);
    Func_02003f46(20);
    Func_02003fb6_b(10, 2);
    Func_0200400a(10, 258);                     /* 129 << 1 */
    Func_02003f62(60);

    /* Three waypoints, each at height 0x30000 (192 << 10). */
    Func_02001f62(10, 88, 152, 0x30000);
    Func_02003ff0(0, 10, 0);
    Func_02003f7e(10);

    Func_02001f7e(10, 120, 192, 0x30000);
    Func_0200400c(0, 10, 0);
    Func_02003f9a(10);

    Func_02001f9a(10, 120, 240, 0x30000);
    Func_02004028(0, 10, 0);
    Func_02003fb6(10);

    Func_02003fb6(768);                         /* 192 << 2 */
    Func_02004020(13, 0, 0);
    Func_0200402a(10, 0, 0);

    /* Common exit; no argument registers are set. */
    Func_02003fe6();
}
