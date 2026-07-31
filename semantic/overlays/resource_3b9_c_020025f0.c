typedef int s32;

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
 * Uncertainty: none of the twelve callees are identified beyond call
 * shape; the flag id (0x90f) and the two closing constants (32, 12) are
 * recorded as passed values.
 */

extern void Func_020051e8(void);
extern void Func_02005212(s32 arg0, s32 arg1, s32 arg2);
extern void Func_0200531e(void);
extern void Func_02005266(s32 arg0, s32 arg1);
extern void Func_02005254(s32 arg0, s32 arg1, s32 arg2);
extern void Func_02005262(s32 arg0, s32 arg1, s32 arg2);
extern void Func_02005268(s32 arg0, s32 arg1, s32 arg2);
extern void Func_0200535c(void);
extern void Func_02005368(void);
extern s32 Func_020051fe(s32 arg0);
extern void Func_02005348(s32 arg0);
extern void Func_02005350(s32 arg0);

void Func_020025f0(void)
{
    Func_020051e8();
    Func_02005212(0, 0x19999, 0xcccc);
    Func_0200531e();
    Func_02005266(0, 2);
    Func_02005254(0, 780, 428);
    Func_02005262(0, 880, 428);
    Func_02005268(0, 980, 428);
    Func_0200535c();
    Func_02005368();

    if (Func_020051fe(0x90f) != 0) {
        Func_02005348(32);
    } else {
        Func_02005350(12);
    }
}
