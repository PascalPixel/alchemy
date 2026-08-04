#include "types.h"

/* Two veneers each serve two imports: 0x020046fa is the three-argument setter
 * at site 8 and the one-argument wait at site 21, and 0x02004730 is the
 * three-argument step at site 14 and the one-argument wait at site 27. */
extern u8 *Func_02004644(s32);
extern void Func_02004632(void);
extern void Func_0200273e(s32, s32);
extern void Func_02002638(s32, s32, s32, s32);
extern void Func_0200464a(s32);
extern void Func_020023b4(s32, s32, s32, s32, s32, s32, s32, s32);
extern void Func_02004728(s32, s32);
extern void Func_020046fa(s32, s32, s32);
extern void Func_02004680(s32);
extern void Func_020046f0(s32, s32);
extern void Func_02004742(s32, s32);
extern void Func_0200469c(s32);
extern void Func_020026a2(s32, s32, s32, s32);
extern void Func_02004730(s32, s32, s32);
extern void Func_020046be(s32);
extern void Func_020026c0(s32, s32, s32, s32);
extern void Func_0200474e(s32, s32, s32);
extern void Func_020046dc(s32);
extern void Func_020026de(s32, s32, s32, s32);
extern void Func_0200476c(s32, s32, s32);
extern void Func_020046fa_b(s32);
extern void Func_020026fc(s32, s32, s32, s32);
extern void Func_0200478a(s32, s32, s32);
extern void Func_02004718(s32);
extern void Func_020047d8(s32, s32);
extern void Func_02004782(s32, s32, s32);
extern void Func_02004730_b(s32);
extern void Func_0200472e(s32);
extern void Func_02004798(s32, s32, s32);
extern void Func_02004754(void);
void Func_02001880(void)
{
    u8 *slot;

    slot = Func_02004644(14);

    /* r0 still holds the record returned above. */
    Func_02004632();

    Func_0200273e(14, 1);
    Func_02002638(14, 392, 504, 0x60000);       /* 196 << 1, 252 << 1, 192 << 11 */
    Func_0200464a(10);

    Func_020023b4(*(s32 *)(slot + 8), *(s32 *)(slot + 12),
                  *(s32 *)(slot + 16) + 0x40000,    /* 128 << 11 */
                  0, 0, 0, 1, 0);

    Func_02004728(14, 1);
    Func_020046fa(14, 0, 0);
    Func_02004680(20);
    Func_020046f0(14, 2);
    Func_02004742(14, 258);                     /* 129 << 1 */
    Func_0200469c(60);

    /* Four waypoints; Z is 132 << 2 and the height 192 << 10 throughout. */
    Func_020026a2(14, 360, 528, 0x30000);
    Func_02004730(0, 14, 0);
    Func_020046be(6);

    Func_020026c0(14, 328, 528, 0x30000);
    Func_0200474e(0, 14, 0);
    Func_020046dc(6);

    Func_020026de(14, 288, 528, 0x30000);
    Func_0200476c(0, 14, 0);
    Func_020046fa_b(6);

    Func_020026fc(14, 256, 528, 0x30000);
    Func_0200478a(0, 14, 0);
    Func_02004718(6);

    Func_020047d8(0, 1);
    Func_02004782(14, 0, 0);
    Func_02004730_b(30);
    Func_0200472e(0x306);
    Func_02004798(17, 0, 0);

    /* Common exit; no argument registers are set. */
    Func_02004754();
}
