typedef signed int s32;

/*
 * resource_3a4 owner at 0x02002bb0, 48 bytes: stash the caller's four
 * arguments into two RAM globals, run an unresolved cutscene primitive
 * with a fixed argument, then install a per-frame task at the standard
 * 200-frame rate.
 *
 * Complete owner: `push {lr}` at 0x02002bb0 through the matching
 * interworking return `pop {r0} / bx r0` at 0x2002bce-0x02002bd0, followed
 * by the three-word literal pool 0x02002bd2-0x02002bdd that closes the
 * 48-byte span. Four arguments (r0-r3), void.
 *
 * Not found by the structural inventory walk (unindexed); four incoming
 * call sites per overlay_unindexed.ts. This owner makes two outgoing
 * calls, both resolved with the `+2` rule against the raw image (this
 * owner starts outside a recorded row, so overlay_call_targets.ts's own
 * table does not cover it -- resolved with `bun tools/overlay_show.ts
 * resource_3a4 2bb0 | bun tools/overlay_call_targets.ts resource_3a4 2bb0
 * 2bd0 --annotate`):
 *   0x2002bc0 -> veneer -> Func_0808a5e0(170)          old-style import, arity varies per site across the codebase (semantic/overlays/resource_3c8_c_02003068.c calls it with this same literal 170 among other arities); not resolved to a specific role here
 *   0x2002bca -> veneer -> Func_080000d0(0x0200ab59, 0xc80)   install a per-frame task, established, the standard 200-frame rate (0xc80 = 200 << 4)
 *
 * Data_0200d240 (a three-word block, arg0/arg1/arg2) and Data_0200d23c (a
 * separate word immediately before it, arg3) are not otherwise
 * cross-referenced from this owner alone.
 */

extern s32 Data_0200d240[3];
extern s32 Data_0200d23c;

void Func_0808a5e0();          /* old-style import, arity varies per site */
void Func_080000d0();          /* install a per-frame task, established */

void Func_02002bb0(s32 arg0, s32 arg1, s32 arg2, s32 arg3)
{
    Data_0200d240[0] = arg0;
    Data_0200d240[1] = arg1;
    Data_0200d240[2] = arg2;
    Data_0200d23c = arg3;

    Func_0808a5e0(170);
    Func_080000d0(0x0200ab59, 0xc80);
}
