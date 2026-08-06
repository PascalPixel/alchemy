#include "types.h"

/*
 * resource_3bb owner at 0x02001df4, 288 bytes: run a short setup pair
 * on `arg0`, seed two fixed slot-table cells with fixed ids, then
 * branch on whether a lookup by the shared "current slot" index
 * succeeds -- on failure, either close out (if the index has already
 * passed 3) or recurse into the next index via a call into a
 * completely different part of the ROM; on success, close out through
 * a different id pair.
 *
 * Complete owner: `push {r5, r6, r7, lr}` + the r8/r9/sl/fp spill at
 * 0x02001df4 through `pop {r3, r5, r6, r7} / mov r8,r3 / mov r9,r5 /
 * mov sl,r6 / mov fp,r7 / pop {r5, r6, r7} / pop {r0} / bx r0` at
 * 0x02001eec-0x02001efa; the six-word literal pool 0x02001efc-0x02001f13
 * follows; the next owner's prologue is at 0x02001f14. One argument
 * (r0), void.
 *
 * Not found by the structural inventory walk (unindexed): reached only by
 * `bl`, resolved with `bun tools/overlay_call_targets.ts`'s `+2` rule.
 *
 * `slotsBase = Data_03001ebc` is dereferenced here exactly the same way
 * resource_39c_c_02000350.c's adopted `Func_02000350` and this
 * project's other `Data_03001ebc` sites do -- confirming it as a
 * shared, cross-overlay EWRAM global, not something private to
 * resource_39c. `idx` is read from the same `Data_02000240 + 0x1f4`
 * cell resource_39c's siblings and resource_3bb_c_020020b8.c's family
 * (a different, unrelated cell in that case) use for their own
 * per-overlay "current" concepts.
 *
 * Uncertainty: none of the fourteen callees are identified beyond call
 * shape. `resultObj`'s fields 8/16 and the
 * two fixed slot-table cells (slotsBase+0xcc2 halfword, slotsBase+0xcc4
 * halfword) are inferred only from this call shape.
 */

extern u8 *Data_03001ebc;
















extern u8 * Func_02005d5e(s32 arg0);
extern u8 * Func_02005d64(s32 arg0);
extern u8 * Func_02005d74(s32 arg0);
extern void Func_02005d62(void);
extern void Func_02005e04(s32 id);
extern void Func_02005e14(s32 arg0, s32 arg1);
extern s32 Func_02005d9e(s32 arg0, s32 arg1);
extern void Func_02005e34(s32 id);
extern void Func_02005e4e(s32 arg0, s32 arg1);
extern void Func_02005eee(void);
extern void Func_02005efa();
extern void Func_02005da4(s32 arg0, s32 arg1);
extern void Func_02005db6(s32 arg0, s32 arg1);
extern void Func_02005ee8(s32 arg0);
extern void Func_02005db0(void);
extern void Func_02003a70(void);
extern void Func_02005f2c(s32 arg0);
extern void Func_02005f40();
extern void Func_02005eae(s32 id);
extern void Func_02005ec6(s32 arg0, s32 arg1);
extern void Func_02005e2a(void);
void Func_02001df4(s32 arg0)
{
    u8 *slotsBase = Data_03001ebc;
    u32 idx = *(u32 *)0x02000434;
    u8 *resultObj;
    s32 lookupResult;

    Func_02005d5e(arg0);
    Func_02005d64(arg0);

    resultObj = Func_02005d74(idx);
    Func_02005d62();

    Func_02005e04(0x2086);
    Func_02005e14(arg0, 0);

    *(u16 *)(slotsBase + 0xcc2) = 0x2089;
    *(u16 *)(slotsBase + 0xcc4) = 4;

    lookupResult = Func_02005d9e(idx, 0);

    if (lookupResult == 0) {
        u8 *slotWord = slotsBase + 448;

        Func_02005e34(0x2087);
        Func_02005e4e(arg0, 0);

        *(u32 *)slotWord = 0x200;
        *(u32 *)(slotsBase + 456) = 15;

        Func_02005eee();
        Func_02005efa();

        Func_02005da4((s32)(idx * 16 + 0x370), *(s32 *)(resultObj + 8) >> 20);
        Func_02005db6((s32)(idx * 16 + 0x378), *(s32 *)(resultObj + 16) >> 20);

        if (idx + 1 <= 3) {
            Func_02005ee8((s32)(idx + 1));
            Func_02005db0();
            Func_02003a70();
        } else {
            *(u32 *)slotWord = lookupResult;
            Func_02005f2c(10);
            Func_02005f40(282);
        }
    } else {
        Func_02005eae(0x2088);
        Func_02005ec6(arg0, 0);
    }

    Func_02005e2a();
}
