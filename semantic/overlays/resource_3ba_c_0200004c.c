typedef signed int s32;

/*
 * Resource 3ba, per-frame cutscene task at 0x0200004c.
 *
 * The owner is complete: `push {r5, r6, r7, lr}` plus `sub sp, #8` at
 * 0x0200004c, and the matching `add sp, #8 / pop {r5, r6, r7} / pop {r0} /
 * bx r0` at 0x02000128..0x0200012e.  A one-word literal pool at 0x02000130
 * (0x0200c41c) closes the 232-byte row.  Nothing live escapes the row.
 *
 * `pop {r0} ; bx r0` — r0 holds the popped return address, so the owner
 * returns nothing.
 *
 * LINK BASE.  This overlay is linked at 0x02008000, proven from the byte-exact
 * sibling `assets/code/resource_3ba_c_02000158.c`, whose task pool word is
 * 0x0200804d = (this function's address 0x0200004c) + the Thumb bit.  So this
 * function IS the task that 0x02000158 installs through Func_080000d0 and that
 * 0x02000134 removes through Func_080000d8; the same sibling shows the shared
 * mode word 0x0200c41c, spelled `Data_0200c41c` and typed `u32` there.
 *
 * SHAPE.  Every frame the task looks at the shared mode word, presents one
 * camera/scene arrangement for that mode, and then decrements the word.  So
 * 0x02000158 seeds the word with a frame budget (it writes 66) and each frame
 * counts down through the arrangements below.  Modes 6 and 66 share one arm --
 * one `bl` site each, reached from two `cmp`s -- which is why the switch below
 * groups those cases rather than duplicating the calls.
 *
 * CALL ACCOUNTING.  Eleven `bl` sites in the row, resolved with
 * `tools/overlay_call_targets.ts`: Func_080091c8 x6, Func_0808a100 x3,
 * Func_080091c0 x2.  The C below has exactly the same multiset (the shared
 * 6/66 arm is written once, as in the assembly).  The inventory's `calls=11`
 * agrees, though that field is only a lower bound in general.
 *
 * UNCERTAINTY.  Func_080091c8 and Func_080091c0 are the established
 * six-argument presentation/renderer ABI (r0-r3 plus [sp,#0] and [sp,#4]);
 * the meaning of the individual numbers is not asserted here.  In the mode-0
 * arm the assembly writes [sp,#0] once and reuses it for both Func_080091c8
 * calls; that is preserved by passing the same value twice.
 */

/* Old-style declarations: these imports' interfaces are not fully known and
 * the same name takes different argument counts elsewhere in the tree. */
void Func_080091c0();           /* six-argument renderer, last two on stack */
void Func_080091c8();           /* six-argument scene presentation request */
void Func_0808a100();           /* two-argument cue */

/* The shared mode/countdown word.  In-image data at file offset 0x441c. */
extern s32 Data_0200c41c;

void Func_0200004c(void)
{
    s32 mode;

    mode = Data_0200c41c;

    if (mode == 60) {
        Func_080091c8(92, 33, 2, 2, 50, 38);
        Func_080091c8(92, 33, 2, 2, 54, 38);
        Func_080091c0(50, 25, 6, 1, 50, 12);
        Func_0808a100(16, 11);
    } else if (mode == 6 || mode == 66) {
        Func_080091c8(92, 31, 2, 2, 50, 38);
        Func_080091c8(92, 31, 2, 2, 54, 38);
        Func_0808a100(16, 10);
    } else if (mode == 0) {
        Func_080091c8(92, 29, 2, 2, 50, 38);
        Func_080091c8(92, 29, 2, 2, 54, 38);
        Func_0808a100(16, 12);
        Func_080091c0(50, 24, 6, 1, 50, 12);
        Data_0200c41c = 120;
    }

    Data_0200c41c = Data_0200c41c - 1;
}
