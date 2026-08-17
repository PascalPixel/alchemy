#include "types.h"

/*
 * resource_3b1 owner at 0x02000728, 208 bytes including its nine-word pool.
 * Four ordered story-flag arms select the scene script; the 0x921 arm also
 * publishes a halfword state when 0x925 is clear and 0x924 is set. The sole
 * return is at 0x020007d2, and all 19 calls appear in machine order.
 *
 * Per-site call symbols (law 6): each callee below is named at the
 * bl-site-symbols-decoded address for its own call site, not the
 * generic main-image address the earlier draft used. Two sites (the
 * Func_0808a1b8(8,0xd000,60) call and the Func_0808a170(0x1e06) call)
 * decode to the same local trampoline, Func_02006d1e -- an old-style
 * declaration lets it be called with either arity.
 */

void Func_02006bf4(void);   /* site 0x72a, was Func_0808a018 */
int  Func_02006bd2();       /* site 0x730, was Func_080770c0(0x928) */
void Func_02006ce4();       /* site 0x73a, was Func_0808a170(0x1eb2) */
void Func_02004fae();       /* site 0x740, was Func_0200486c(8) [1st] */
void Func_02006d1e();       /* sites 0x74c and 0x774, shared trampoline */
void Func_02006cb6();       /* site 0x754, was Func_0808a110(8,4) */
void Func_02004fc8();       /* site 0x75a, was Func_0200486c(8) [2nd] */
void Func_02006cc4();       /* site 0x762, was Func_0808a110(8,3) */
int  Func_02006c0c();       /* site 0x76a, was Func_080770c0(0x925) [1st] */
void Func_02006d36();       /* site 0x77c, was Func_0808a180(8,0) [1st] */
int  Func_02006c26();       /* site 0x784, was Func_080770c0(0x921) */
void Func_02006d38();       /* site 0x78e, was Func_0808a170(0x1dcd) */
void Func_02006d50();       /* site 0x796, was Func_0808a180(8,0) [2nd] */
int  Func_02006c3e();       /* site 0x79c, was Func_080770c0(0x925) [2nd] */
int  Func_02006c48();       /* site 0x7a6, was Func_080770c0(0x924) */
void Func_02006d6a();       /* site 0x7c0, was Func_0808a170(0x1d30) */
void Func_02006d82();       /* site 0x7c8, was Func_0808a180(8,0) [3rd] */
void Func_02006c9e(void);   /* site 0x7cc, was Func_0808a020 */

extern u8 *Data_03001ebc;

void Func_02000728(void)
{
    Func_02006bf4();
    if (Func_02006bd2(0x928) != 0) {
        Func_02006ce4(0x1eb2);
        Func_02004fae(8);
        Func_02006d1e(8, 0xd000, 60);
        Func_02006cb6(8, 4);
        Func_02004fc8(8);
        Func_02006cc4(8, 3);
    } else if (Func_02006c0c(0x925) != 0) {
        Func_02006d1e(0x1e06);
        Func_02006d36(8, 0);
    } else if (Func_02006c26(0x921) != 0) {
        Func_02006d38(0x1dcd);
        Func_02006d50(8, 0);
        if (Func_02006c3e(0x925) == 0 && Func_02006c48(0x924) != 0) {
            u16 *dst = (u16 *)(Data_03001ebc + 370);
            u16 v = 1;
            *dst = v;
        }
    } else {
        Func_02006d6a(0x1d30);
        Func_02006d82(8, 0);
    }
    Func_02006c9e();
}
