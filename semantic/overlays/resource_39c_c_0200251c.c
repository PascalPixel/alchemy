typedef unsigned char u8;
typedef int s32;

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
 * `Func_02008004` is called twice with genuinely different argument
 * counts: once with a single fresh register (the per-slot lookup, r1-r3
 * are stale leftovers from the earlier Func_02007fd6 call, not real
 * arguments) and once with six freshly-set arguments in the closing
 * dispatch. Declared old-style (no prototype) rather than picking one
 * shape and miscrediting the other -- the same "arities vary per site"
 * convention resource_3cb_c_02000580.c already documents for this
 * project.
 *
 * Uncertainty: none of the other thirteen callees are identified beyond
 * call shape; the four-slot loop's per-slot id pair (`slot*4+39`/`+41`)
 * and the second check's id constants (57, 59) are recorded as compared
 * values, not decoded further.
 */

extern void Func_02007fd6(s32 arg0);
extern void *Func_02008004();
extern void Func_02007fd8(s32 arg0);
extern void Func_02007fe6(s32 arg0);
extern void Func_02007fec(s32 arg0);
extern void Func_02007ffa(s32 arg0);
extern void Func_02008002(s32 arg0);
extern void Func_02008008(s32 arg0);
extern void *Func_02008056(s32 arg0);
extern void Func_02008026(s32 arg0);
extern void Func_02008034(s32 arg0);
extern void Func_02008062(s32 arg0);
extern void Func_02008068(s32 arg0);
extern void Func_02008044(s32 arg0);
extern void Func_02007fe0(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);
extern void Func_020080a8(void);

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

    Func_02007fd6(arg0);

    for (slot = 0; slot <= 3; slot++) {
        s32 id0 = slot * 4 + 39;
        s32 id1 = slot * 4 + 41;
        s32 a = 816 + slot * 2;

        id = Fixed20(*(s32 *)((u8 *)Func_02008004(slot + 15) + 8));

        if (id == id0) {
            Func_02007fd8(a);
            Func_02007fe6(a + 1);
        } else if (id == id1) {
            Func_02007fec(a + 1);
            Func_02007ffa(a);
        } else {
            Func_02008002(a);
            Func_02008008(a + 1);
        }
    }

    id = Fixed20(*(s32 *)((u8 *)Func_02008056(19) + 8));

    if (id == 57) {
        Func_02008026(824);
        Func_02008044(825);
    } else if (id == 59) {
        Func_02008034(825);
        Func_02008044(824);
    } else {
        Func_02008062(824);
        Func_02008068(825);
        Func_02008004(53, 11, 1, 1, 58, 7);
        Func_020080a8();
        return;
    }

    Func_02007fe0(53, 10, 1, 1, 58, 7);
    Func_020080a8();
}
