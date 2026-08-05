#include "types.h"

/*
 * resource_39c owner at 0x0200251c, 228 bytes: check four fixed-point
 * object positions against small per-slot threshold pairs and dispatch one
 * of three handler pairs per slot, then a second, independent id check
 * that dispatches one of three further call sequences.
 *
 * Complete owner: `push {r5, r6, lr}` at 0x0200251c through `pop {r5, r6} /
 * pop {r0} / bx r0` at 0x20025f0-0x20025f4, followed by the two-word
 * literal pool 0x020025f8-0x020025ff; the next owner's prologue is at
 * 0x02002600. One argument (r0), forwarded unread to the first call (r1-r3
 * are never freshly set before it either, so it is not credited with more
 * than the one), so it is kept as a plain pass-through parameter; void
 * return.
 *
 * Not found by the structural inventory walk (unindexed): reached only by
 * `bl`, resolved with `bun tools/overlay_call_targets.ts`'s `+2` rule.
 *
 * `Fixed20()` is the same `if (raw < 0) raw += 0xfffff; raw >> 20`
 * truncate-toward-zero idiom resource_39c_c_020021cc.c already documents.
 *
 * The id-57 and id-59 paths each set one counterpart id, then converge on
 * one `Func_080770d0` call and the mode-10 `Func_080091c0` dispatch. The
 * unmatched path performs two clears and uses mode 11 before all paths
 * converge on `Func_0808a020`.
 *
 * Uncertainty: none of the six resolved callees are identified beyond
 * call shape; the four-slot loop's per-slot id pair (`slot*4+39`/`+41`)
 * and the second check's id constants (57, 59) are recorded as compared
 * values, not decoded further.
 */

extern void Func_0808a018(s32 arg0);
extern void *Func_0808a080(s32 arg0);
extern void Func_080770c8(s32 arg0);
extern void Func_080770d0(s32 arg0);
extern void Func_080091c0(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);
extern void Func_0808a020(void);

static s32 Fixed20(s32 raw)
{
    if (raw < 0) {
        raw += 0xfffff;
    }
    return raw >> 20;
}

void Func_0200251c(s32 arg0)
{
    s32 slot;
    s32 id;

    Func_0808a018(arg0);

    for (slot = 0; slot <= 3; slot++) {
        s32 id0 = slot * 4 + 39;
        s32 id1 = slot * 4 + 41;
        s32 a = 816 + slot * 2;

        id = Fixed20(*(s32 *)((u8 *)Func_0808a080(slot + 15) + 8));

        if (id == id0) {
            Func_080770c8(a);
            Func_080770d0(a + 1);
        } else if (id == id1) {
            Func_080770c8(a + 1);
            Func_080770d0(a);
        } else {
            Func_080770d0(a);
            Func_080770d0(a + 1);
        }
    }

    id = Fixed20(*(s32 *)((u8 *)Func_0808a080(19) + 8));

    if (id == 57) {
        Func_080770c8(824);
    } else if (id == 59) {
        id = 825;
        goto matched;
        Func_080770c8(825);
        id = 824;
        goto matched;
    }

    goto unmatched;

    Func_080770d0(id);
    Func_080091c0(53, 10, 1, 1, 58, 7);
    goto close;
matched:

    Func_080770d0(824);
    Func_080770d0(825);
unmatched:
    Func_080091c0(53, 11, 1, 1, 58, 7);

close:
    Func_0808a020();
}
