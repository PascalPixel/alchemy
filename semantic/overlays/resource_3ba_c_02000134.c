typedef signed int s32;

/*
 * Resource 3ba, cutscene teardown at 0x02000134.
 *
 * The owner is complete: `push {r5, lr}` at 0x02000134 and the matching
 * `pop {r5} / pop {r0} / bx r0` at 0x02000148..0x0200014c, an alignment
 * halfword, then the two-word literal pool 0x02000150/0x02000154 that closes
 * the 36-byte row.  `pop {r0} ; bx r0` — r0 is the popped return address, so
 * the owner returns nothing.
 *
 * LINK BASE (0x02008000, proven).  The pool word 0x0200804d is
 * Func_0200004c + the Thumb bit, and 0x0200c41c is the in-image mode word at
 * file offset 0x441c.  Both spellings are corroborated by the byte-exact
 * sibling `assets/code/resource_3ba_c_02000158.c`, which is this function's
 * counterpart: it writes 66 to the same word and *installs* the same callback
 * with `Func_080000d0(0x0200804d, 0xc80)`.
 *
 * THE SECOND `bl`.  Site 0x02000144 targets 0x02003e1c, which is this
 * overlay's OWN `call_via` bank (`bx r0 / nop`, `bx r1 / nop`, ... from
 * 0x02003e04); 0x02003e1c is the `bx r5` entry.  r5 is loaded from the pool
 * at 0x02000138 with 0x0200804d and is not written again -- `pop {r5}` in the
 * epilogue confirms it is a callee-saved live value, not scratch -- so this
 * is an indirect call to Func_0200004c itself.  The overlay therefore
 * unregisters the per-frame task and then runs its body once more directly,
 * with the mode word freshly zeroed so the mode-0 arm executes.
 *
 * CALL ACCOUNTING.  Two `bl` sites in the row: Func_080000d8 (task removal,
 * one argument -- the same shape as the semantic `resource_371` sources) and
 * the `call_via r5` above.  The inventory reports `calls=2`.
 *
 * UNCERTAINTY.  Func_080000d8 clobbers r0; the indirect call inherits
 * whatever it left there.  Func_0200004c takes no arguments (it reads the
 * mode word), so nothing is asserted about that register.
 */

/* Old-style declaration: the task-removal import's interface is not fully
 * known and the same name is called with other argument counts elsewhere. */
void Func_080000d8();

/* The per-frame task this owner tears down; also the callback it re-enters. */
void Func_0200004c(void);

/* The shared mode/countdown word.  In-image data at file offset 0x441c. */
extern s32 Data_0200c41c;

void Func_02000134(void)
{
    void (*task)(void);

    task = Func_0200004c;

    Data_0200c41c = 0;

    /* r0 = the Thumb-tagged task pointer, exactly as the installer passes it. */
    Func_080000d8((void (*)(void))((s32)task | 1));

    /* bl 0x02003e1c = this overlay's `bx r5` thunk, with r5 still holding the
     * same pointer: a direct re-entry of the task body. */
    task();
}
