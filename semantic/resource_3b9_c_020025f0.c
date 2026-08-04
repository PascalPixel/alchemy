#include "types.h"

/*
 * resource_3b9 owner at 0x020025f0, 106 bytes: the same linear setup
 * shape resource_3b9_c_02002904.c documents (one call with the two
 * large constants 0xcccc/0x19999, three near-identical 428-second-
 * argument calls, a short closing run), plus a trailing flag check that
 * picks one of two final calls.
 *
 * Complete owner: `push {lr}` at 0x020025f0 through `pop {r0} / bx r0`
 * at 0x02002656-0x02002658, followed by the three-word literal pool
 * 0x0200265c-0x02002667; the next owner's prologue is at 0x02002668
 * (this overlay's own resource_3b9_c_02002668.c candidate, not yet
 * drafted). No incoming arguments are read before being overwritten, so
 * `void`.
 *
 * Not found by the structural inventory walk (unindexed): reached only
 * by `bl`, resolved with `bun tools/overlay_call_targets.ts`'s `+2`
 * rule.
 *
 * Uncertainty: none of the ten resolved callees are identified beyond call
 * shape; the flag id (0x90f) and the two closing constants (32, 12) are
 * recorded as passed values.
 */

extern void Func_0808a018(void);
extern void Func_0808a090(s32 arg0, s32 arg1, s32 arg2);
extern void Func_0808a360(void);
extern void Func_0808a100(s32 arg0, s32 arg1);
extern void Func_0808a0c0(s32 arg0, s32 arg1, s32 arg2);
extern void Func_0808a0b8(s32 arg0, s32 arg1, s32 arg2);
extern void Func_0808a368(void);
extern void Func_0808a370(void);
extern s32 Func_080770c0(s32 arg0);
extern void Func_0808a248(s32 arg0);

void Func_020025f0(void)
{
    Func_0808a018();
    Func_0808a090(0, 0x19999, 0xcccc);
    Func_0808a360();
    Func_0808a100(0, 2);
    Func_0808a0c0(0, 780, 428);
    Func_0808a0c0(0, 880, 428);
    Func_0808a0b8(0, 980, 428);
    Func_0808a368();
    Func_0808a370();

    if (Func_080770c0(0x90f) != 0) {
        Func_0808a248(32);
    } else {
        Func_0808a248(12);
    }
}
