#include "types.h"

extern unsigned char *Data_03001ebc;

extern void Func_020012fc(void);
extern void Func_0200137a(s32);
/* s32, not u8.  The reference loads the pool word 0x00000301 at 0x02000280 into
 * r0 for this call; a u8 parameter truncates 0x301 to 1 at compile time and emits
 * `movs r0, #1`, so the callee would receive a different value.  The narrow type
 * is not a near-miss, it is wrong.
 *
 * Widening does NOT close the owner and makes the count worse, 7 differing
 * halfwords to 36, for a reason that is not this declaration: with both sites
 * passing 0x301, gcc shares the constant in r5 and pays `push {r5, lr}` for it,
 * where the reference re-loads the same pool word at each site. Proved by
 * changing the second site's constant as a diagnostic -- sharing then cannot
 * happen and the residual falls to 3 halfwords, the remaining 4 bytes being the
 * second pool word that diagnostic introduces. So what is left here is gcc's
 * cross-block constant CSE, not the interface. */
extern s32 Func_020012e8(s32);
extern void Func_020013a4(s32, s32);
extern void Func_02001312(s32);
extern void Func_02001336(void);

void Func_02000240(void)
{
    Func_020012fc();
    Func_0200137a(0x13ae);
    if (Func_020012e8(0x301)) {
        ((u16 *)Data_03001ebc)[236]++;
    }
    Func_020013a4(9, 0);
    Func_02001312(0x301);
    Func_02001336();
}
