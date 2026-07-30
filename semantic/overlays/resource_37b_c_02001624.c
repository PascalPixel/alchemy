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
 * Call sites (resolved with `tools/overlay_call_targets.ts`, which applies the
 * overlay `bl` rule `target = stored_displacement + 2`; the disassembler's own
 * annotations are wrong for overlays):
 *
 *   0x02001628, 0x02001632 -> import veneer 0x02002424 -> Func_08009278
 *   0x02001652, 0x02001662 -> import veneer 0x0200246c -> Func_0808a080
 *
 * So the two "printed" names in each pair are one import reached twice.  The
 * per-site printed spelling is kept below because the byte-exact sources in
 * this overlay (e.g. assets/code/resource_37b_c_0200166c.c, which is the same
 * routine for a different scene) already use it; the identity is recorded here
 * rather than by renaming anything.
 *
 * The state accessor returns a pointer to a record whose words at +8 and +16
 * are 16.16-style fixed-point quantities; both are read as `>> 20` (arithmetic)
 * to yield a small integer selector.  This matches the byte-exact sibling at
 * 0x0200166c exactly, including the `(u32)(v - base) <= 1` two-value range test
 * spelled in assembly as `subs / cmp #1 / bhi`.
 */

/* Imports.  Old-style declarations: arities vary per site across this overlay
 * and two names can be one import. */
s32 *Func_02003a96();
s32 *Func_02003aa0();
void Func_02003a78();
void Func_02003a88();

void Func_02001624(void)
{
    s32 selector;
    s32 stage;

    selector = Func_02003a96(0)[2] >> 20;
    stage = Func_02003aa0(0)[4] >> 20;

    if (stage == 8 && (u32)(selector - 17) <= 1) {
        /* 0x88 << 17 and 0x90 << 17, 0x80 << 16, built by movs/lsls pairs. */
        Func_02003a78(2, 0x1100000, 0x800000, 255);
        Func_02003a88(2, 0x1200000, 0x800000, 255);
    }
}
