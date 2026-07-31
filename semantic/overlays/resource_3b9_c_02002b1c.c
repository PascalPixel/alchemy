typedef int s32;

/*
 * resource_3b9 owner at 0x02002b1c, 112 bytes: a handler-family run --
 * seven calls with sequential ids 12-18 and small tag values, then five
 * more calls reusing four of the same callees with different tag
 * values, plus one new callee.
 *
 * Complete owner: `push {lr}` at 0x02002b1c through `pop {r0} / bx r0`
 * at 0x02002b86-0x02002b88, followed by one alignment halfword; the
 * next owner's prologue is at 0x02002b8c (an import-veneer stub, not an
 * item-28 candidate). No incoming arguments are read before being
 * overwritten, so `void`.
 *
 * Not found by the structural inventory walk (unindexed): reached only
 * by `bl`, resolved with `bun tools/overlay_call_targets.ts`'s `+2`
 * rule.
 *
 * `Func_020057c0`, `Func_020057c8`, `Func_020057d8` and `Func_020057e0`
 * are each called twice (the second `Func_020057c8` call three times)
 * with different tag values at different sites -- ordinary repeat
 * calls, not an arity conflict (both sites take the same 2-argument
 * shape).
 *
 * Uncertainty: none of the eight callees are identified beyond call
 * shape; the id/tag pairs are recorded as passed values.
 */

extern void Func_020057b0(s32 id, s32 tag);
extern void Func_020057b8(s32 id, s32 tag);
extern void Func_020057c0(s32 id, s32 tag);
extern void Func_020057c8(s32 id, s32 tag);
extern void Func_020057d0(s32 id, s32 tag);
extern void Func_020057d8(s32 id, s32 tag);
extern void Func_020057e0(s32 id, s32 tag);
extern void Func_020057e8(s32 id, s32 tag);

void Func_02002b1c(void)
{
    Func_020057b0(12, 3);
    Func_020057b8(13, 0);
    Func_020057c0(14, 4);
    Func_020057c8(15, 1);
    Func_020057d0(16, 5);
    Func_020057d8(17, 2);
    Func_020057e0(18, 6);
    Func_020057c0(13, 10);
    Func_020057c8(14, 20);
    Func_020057c8(15, 0);
    Func_020057d8(16, 40);
    Func_020057e0(17, 50);
    Func_020057e8(18, 60);
}
