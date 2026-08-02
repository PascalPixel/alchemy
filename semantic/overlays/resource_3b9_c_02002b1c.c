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
 * The first seven sites resolve to Func_0808a158. The remaining sites use
 * Func_0808a108 except for the id-15/tag-0 call, which resolves separately
 * to Func_0808a100; all retain the same two-argument shape.
 *
 * Uncertainty: none of the three resolved callees are identified beyond call
 * shape; the id/tag pairs are recorded as passed values.
 */

extern void Func_0808a158(s32 id, s32 tag);
extern void Func_0808a108(s32 id, s32 tag);
extern void Func_0808a100(s32 id, s32 tag);

void Func_02002b1c(void)
{
    Func_0808a158(12, 3);
    Func_0808a158(13, 0);
    Func_0808a158(14, 4);
    Func_0808a158(15, 1);
    Func_0808a158(16, 5);
    Func_0808a158(17, 2);
    Func_0808a158(18, 6);
    Func_0808a108(13, 10);
    Func_0808a108(14, 20);
    Func_0808a100(15, 0);
    Func_0808a108(16, 40);
    Func_0808a108(17, 50);
    Func_0808a108(18, 60);
}
