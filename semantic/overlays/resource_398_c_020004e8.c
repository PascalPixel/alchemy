typedef signed int s32;
typedef unsigned char u8;

/*
 * Resource 398 map-variant 0x30 setup at 0x020004e8.
 *
 * Complete owner: `push {lr}` at 0x020004e8 and the matching
 * `pop {r0} / bx r0` at 0x0200052a.  80-byte row: 70 bytes of code, an
 * alignment halfword at 0x0200052e, and the two pool words 0x03001ebc and
 * 0x00000882 filling 0x02000530-0x02000537.  Control-flow walk: the branches
 * target 0x0200051e and 0x0200052a only, so nothing reaches 0x0200052e.  The
 * return address is popped into r0, so the owner is `void`.
 *
 * Reached only from the root 0x0200046c, the entry-0 selector, when the
 * Data_02000240[224] variant reads 0x30.
 *
 * Workspace.  0x03001ebc is a pointer CELL, not the workspace itself — the
 * body is `ldr r3,[pc] / ldr r3,[r3]`, and the banked byte-exact
 * assets/code/resource_398_c_020007c4.c already spells it `u8 *state =
 * Data_03001ebc`.  The displacement/value pair is the additive form: r2 holds
 * 448 while the address is formed, and only then becomes 516, so the store is
 * "write 516 at workspace + 448", not "write at workspace + 516".
 *
 * Imports resolved with tools/overlay_call_targets.ts (an overlay `bl` stores
 * `target_offset - 2`).  Six call sites against the row's advertised five; the
 * manifest's `calls` field is a floor.  The two arms are kept separate: the
 * `Func_0808a080` lookup only exists on the else arm, and merging would
 * deflate the multiset.
 */

extern u8 *Data_03001ebc;

/* Old-style declarations: interfaces vary by call site across this overlay. */
void Func_080091e0();  /* attach an actor record to its display slot */
s32 Func_080770c0();   /* read event flag */
u8 *Func_0808a080();   /* actor record by slot id */
void Func_0808a0f0();  /* run script (id, a, b) */
void Func_0808a100();  /* set actor visibility/state (slot, state) */

void Func_020004e8(void)
{
    *(s32 *)(Data_03001ebc + 448) = 516;

    Func_0808a100(8, 1);
    Func_0808a100(10, 2);

    if (Func_080770c0(0x882) != 0) {
        Func_0808a0f0(9, 0, 0);
    } else {
        Func_080091e0(Func_0808a080(9), 0);
    }
}
