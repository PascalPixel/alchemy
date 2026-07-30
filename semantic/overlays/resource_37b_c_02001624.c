typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

/*
 * Resource 37b scene step at 0x02001624.
 *
 * Complete owner: `push {r5, lr}` at 0x02001624 through the interworking
 * return `pop {r5} / pop {r0} / bx r0` at 0x02001666.  The popped register is
 * r0, so r0 holds the return address and nothing is returned: the owner is
 * `void`.  No literal pool sits inside the span.
 *
 * Call sites resolved with `tools/overlay_call_targets.ts`, which applies the
 * overlay rule `target_offset = stored_displacement + 2`; the disassembler's
 * own `bl` annotations are wrong for every overlay:
 *
 *   0x02001628, 0x02001632 -> veneer 0x0200246c -> Func_0808a080
 *   0x02001652, 0x02001662 -> veneer 0x02002424 -> Func_08009278
 *
 * 4 sites, 2 distinct imports, matching the assembly `bl` multiset for the
 * span (the inventory's calls=4 is only a lower bound in general).  The two
 * "printed" names in each pair are one import reached twice, so the resolved
 * main-image names are used here rather than the per-site spelling that the
 * byte-exact sources in this overlay carry.
 *
 * Func_0808a080(0) returns a pointer to the scene-state record; its words at
 * +8 and +16 are 16.16-style fixed-point values read as arithmetic `>> 20` to
 * give small integer selectors.  The byte-exact sibling
 * assets/code/resource_37b_c_0200166c.c is the same routine for a different
 * scene and reads the same two fields the same way, including the
 * `(u32)(v - base) <= 1` two-value range test that assembly spells as
 * `subs / cmp #1 / bhi`.  Func_0808a080 is also the record accessor used by
 * 0x02000c8c in this overlay, which independently confirms the resolution.
 */

/* Imports.  Old-style declarations: arity varies per site in this overlay. */
s32 *Func_0808a080();
void Func_08009278();

void Func_02001624(void)
{
    s32 selector;
    s32 stage;

    selector = Func_0808a080(0)[2] >> 20;
    stage = Func_0808a080(0)[4] >> 20;

    if (stage == 8 && (u32)(selector - 17) <= 1) {
        /* 0x88 << 17 and 0x90 << 17, and 0x80 << 16, built by movs/lsls. */
        Func_08009278(2, 0x1100000, 0x800000, 255);
        Func_08009278(2, 0x1200000, 0x800000, 255);
    }
}
