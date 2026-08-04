#include "types.h"

/*
 * resource_39e owner at 0x02001d50, 108 bytes: a scripted beat with a
 * SKIP-BEAT GATE, and the skip path is the one that costs the player the
 * story flag.
 *
 * Complete owner: `push {lr}` at 0x02001d50 through `pop {r0} / bx r0` at
 * 0x02001daa-0x02001dac, then the three-word literal pool
 * 0x02001db0-0x02001dbb (0x186e, 0x898, 0x03001ebc). The next owner's
 * prologue is exactly at 0x02001dbc -- the already-drafted
 * Func_02001dbc. **108 bytes**, measured to the epilogue.
 *
 * Published population (sweep B). All 10 `bl` sites resolved with
 * `bun tools/overlay_call_targets.ts resource_39e 1d50 1dbc`; sites=10
 * and ten bl lines were transcribed.
 *
 * A FOURTH SHAPE OF THE SKIP-BEAT COUNTER, and it is not the converging
 * one. In the resource_3c9 copies both arms bump the workspace+472 u16 so
 * it advances exactly once either way; in resource_3af's 0x02003f30 the
 * taken arm returns early and only the not-taken path bumps. Here:
 *   Func_0808a070(0, 0) == 0  -> wait 20 frames, pose slot 18, SET flag
 *                                0x898, close the bracket. NO bump.
 *   otherwise                 -> bump workspace+472 by 1, pose slot 18,
 *                                close the bracket. NO flag.
 * So the counter records skipped beats only, and skipping this beat also
 * skips setting story flag 0x898 -- the flag the neighbouring dispatcher
 * 0x020012e0 tests to choose between Func_02001160 and Func_0200102c.
 * That is a real gameplay consequence hanging off a skip, and it is the
 * reason this row is worth reading closely rather than pattern-matching
 * from a sibling. Branch sense is never inheritable.
 *
 * The two arms are NOT otherwise mirrored: both call
 * Func_0808a188(18, 0, 20) and Func_0808a020(), but only the ==0 arm
 * waits, and the bracket close appears TWICE in the assembly (once per
 * arm) rather than after the join.
 *
 * The workspace access is the standard shape: the u16 at
 * Data_03001ebc + 472, reached by `movs r3, #236 / lsls r3, #1`, cited
 * from resource_38b_c_02000584.c.
 *
 * SHARED IDIOMS, cited: scripted-scene bracket
 * Func_0808a018/Func_0808a020; dialogue-by-id Func_0808a170(0x186e);
 * wait-n-frames Func_0808a010(20); story-flag set Func_080770c8, which
 * is the SET and not the clear.
 *
 * Uncertainty: Func_0808a178(18, 0) and Func_0808a188(18, 0, 20) are
 * transcribed, not identified.
 *
 * STILL-OPEN residual, 4 bytes at 0x02001d5c: the reference sets the third
 * call's arguments as `movs r1,#0` before `movs r0,#18`; this source emits
 * them the other way.  Identical fingerprint to resource_378:574's open
 * residual (see that file's header for the full derivation): the pair ties
 * through every rank_for_schedule tier and falls to sched2's chain-order
 * tie-break; Thumb force-disables the first scheduler pass (arm.c
 * OVERRIDE_OPTIONS), and constant call arguments are expanded arg0-first
 * (calls.c load_register_parameters, ascending, LOAD_ARGS_REVERSED not
 * defined) -- so no scheduling flag and no statement reordering can flip
 * it; -fthumb-call-arg1-before-arg0's uid precondition provably keeps it
 * from firing.  The original source shape that made arg1 a non-deferred
 * value at this one site is not yet identified.
 */

extern void Func_02006130(void);
extern void Func_02006148();
extern s32 Func_0200615e(s32 arg0, s32 arg1);
extern void Func_0200616c(void);
extern void Func_0200618c(void);
extern void Func_020061f6(s32 dialogueId);
extern void Func_02006206(s32 slot, s32 arg1);
extern void Func_02006232(s32 slot, s32 arg1, s32 arg2);
extern void Func_02006258(s32 slot, s32 arg1, s32 arg2);

extern u8 *Data_03001ebc;

void Func_02001d50(void)
{
    u8 *workspace;

    Func_02006130();
    Func_020061f6(0x186e);
    Func_02006206(18, 0);

    if (Func_0200615e(0, 0) == 0) {
        Func_02006148(20);
        Func_02006232(18, 0, 20);
        Func_02006148(0x898);
        Func_0200616c();
    } else {
        workspace = Data_03001ebc;
        *(u16 *)(workspace + 472) += 1;
        Func_02006258(18, 0, 20);
        Func_0200618c();
    }
}
