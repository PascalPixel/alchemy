typedef signed int s32;

/*
 * resource_3bb owner at 0x020009b0, 108 bytes (0x020009b0-0x02000a1b):
 * 96 bytes of code plus the three-word literal pool at 0x02000a10.
 *
 * Prologue `push {r5, lr}` / `sub sp, #8` at 0x020009b0, epilogue
 * `add sp, #8 / pop {r5} / pop {r0} / bx r0`: r0 holds the popped return
 * address, so the owner is void.  r5 holds the constant 58 that both
 * Func_080091c0 calls place in their fifth argument slot.
 *
 * All seven `bl` sites are placed and reach five distinct callees, matching
 * the inventory row's calls=7.  Targets come from
 * tools/overlay_call_targets.ts (target offset = stored displacement + 2),
 * never from the disassembler's annotations - it prints the two identical
 * Func_080000c0 halfwords as different callees.  In call order: 0x40a0 ->
 * Func_0808a590, 0x3ef0 -> Func_080770c8, 0x3da0 -> Func_080000c0 (twice),
 * 0x3db0 -> Func_080000d8, 0x3e88 -> Func_080091c0 (twice).
 *
 * The pool word 0x02008715 is `Func_02000714 + 1`, this overlay's own routine
 * at file offset 0x0714 carrying the Thumb bit - the same task pointer the
 * byte-exact sibling assets/code/resource_3bb_c_02000950.c passes, and the
 * cheapest witness for the 0x02008000 link base.  Under that base
 * 0x0200c834 and 0x0200c838 are in-image words at file offsets 0x4834 and
 * 0x4838: the same pair that 0x020008ec clears and 0x02000970 polls.
 *
 * Shape: reset, then if the first status word is still non-zero clear the
 * second one, wait, reinstall the 0x0714 task, and draw two lines.
 *
 * Uncertainty: what the two status words mean is not established here, only
 * that this owner conditionally clears the second when the first is set.
 */

/* Import veneers, named by the main-image function each one reaches.
 * Old-style declarations: arities vary between call sites in this overlay. */
void Func_0808a590();
void Func_080770c8();
void Func_080000c0();
void Func_080000d8();
void Func_080091c0();

/* This overlay's own task routine at file offset 0x0714. */
void Func_02000714();

/* In-image status words at file offsets 0x4834 and 0x4838. */
extern volatile s32 Data_0200c834;
extern volatile s32 Data_0200c838;

void Func_020009b0(void)
{
    Func_0808a590(31);
    Func_080770c8(820);                 /* 205 << 2 */

    if (Data_0200c834 != 0) {
        Data_0200c838 = 0;
    }

    Func_080000c0(30);
    Func_080000c0(1);

    /* The task is published as its entry address plus the Thumb bit. */
    Func_080000d8((s32)Func_02000714 + 1);

    Func_080091c0(58, 28, 7, 1, 58, 13);
    Func_080091c0(57, 11, 1, 1, 58, 11);
}
