typedef signed int s32;

/*
 * resource_3bb owner at 0x020008ec, 100 bytes (0x020008ec-0x0200094f):
 * 84 bytes of code, two alignment bytes at 0x02000942, and the three-word
 * literal pool at 0x02000944.
 *
 * Prologue `push {lr}`, epilogue `pop {r0} / bx r0`: r0 holds the popped
 * return address, so the owner is void.
 *
 * Scene entry.  It clears the two in-image status words at file offsets
 * 0x4838 and 0x4834, installs this overlay's own routine at file offset
 * 0x0714 as a task, places four participants and starts one of them.  The
 * neighbouring owner 0x02000970 is the matching spin-wait: it polls those
 * same two words until the first is 0 and the second is 75, so the pair is
 * only interpretable together.
 *
 * The pool word 0x02008715 is `Func_02000714 + 1` - a task pointer with the
 * Thumb bit, and the byte-exact sibling assets/code/resource_3bb_c_02000950.c
 * passes the identical constant.  That is the cheapest witness that this
 * overlay is linked at 0x02008000, which is also why 0x0200c834/0x0200c838
 * are in-image words and not RAM globals.
 *
 * All six `bl` sites are placed and reach three distinct callees, matching the
 * inventory row's calls=6.  Targets come from tools/overlay_call_targets.ts
 * (target offset = stored displacement + 2), never from the disassembler's
 * annotations: 0x3db0 -> Func_080000d8, 0x3f90 -> Func_0808a0f0 (four sites),
 * 0x3f98 -> Func_0808a100.
 *
 * The coordinates are built by shifting small immediates, which is how the
 * original spells the 18.14-style fixed-point constants below; they are
 * written out in full here.
 */

/* Import veneers, named by the main-image function each one reaches.
 * Old-style declarations: arities vary between call sites in this overlay. */
void Func_080000d8();
void Func_0808a0f0();
void Func_0808a100();

/* This overlay's own task routine at file offset 0x0714. */
void Func_02000714();

/* In-image status words at file offsets 0x4834 and 0x4838, cleared here and
 * polled by Func_02000970. */
extern volatile s32 Data_0200c834;
extern volatile s32 Data_0200c838;

void Func_020008ec(void)
{
    Data_0200c838 = 0;
    Data_0200c834 = 0;

    /* The task is published as its entry address plus the Thumb bit. */
    Func_080000d8((s32)Func_02000714 + 1);

    Func_0808a0f0(22, 0x3a80000, 0xd80000);      /* 234 << 18, 216 << 16 */
    Func_0808a0f0(23, 0x3c80000, 0xd80000);      /* 242 << 18 */
    Func_0808a0f0(24, 0x3e80000, 0xd80000);      /* 250 << 18 */
    Func_0808a0f0(25, 0x4080000, 0xd80000);      /* 129 << 19 */

    Func_0808a100(31, 10);
}
