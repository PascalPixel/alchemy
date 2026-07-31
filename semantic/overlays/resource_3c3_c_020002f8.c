typedef signed short s16;
typedef unsigned char u8;

/*
 * Resource 3c3, scene beat at 0x020002f8 (60 bytes, 4 call sites).
 *
 * Complete owner: `push {r5, lr}` at 0x020002f8, `pop {r5} / pop {r0} / bx r0`
 * at 0x02000320.  The popped branch register IS r0, so it holds the return
 * address and the owner is `void`.  A two-byte zero alignment word at
 * 0x02000326 is followed by the three-word literal pool
 * 0x02000328-0x02000333 (0x03001ebc, 0x8fb, 0x8fc).
 *
 * Same shape as 0x02000288: read the completion token out of the workspace
 * (`Data_03001ebc` is a pointer CELL, so `ldr r3,[pc] / ldr r3,[r3]` is one
 * dereference) at byte offset 364, hold it in r5 across the body, hand it to
 * Func_0808a248 last.  This beat's payload is the pair of story flags that
 * 0x02000124 tests on the way in: it clears both 0x8fb and 0x8fc.
 *
 * Call targets resolved with `tools/overlay_call_targets.ts`; all 4 sites are
 * import veneers.  Per-target multiset, reproduced exactly by the C below:
 *   Func_080770d0 x2, Func_0808a248 x1, Func_080f9010 x1.
 */

/* Old-style declarations: overlay import arities vary per call site. */
void Func_080770d0();   /* clear a story flag */
void Func_0808a248();   /* scene request keyed by a selector */
void Func_080f9010();   /* play a cue by id */

extern u8 *Data_03001ebc;

void Func_020002f8(void)
{
    u8 *workspace = Data_03001ebc;
    s16 token = *(s16 *)(workspace + 364);

    Func_080f9010(123);

    Func_080770d0(0x8fb);
    Func_080770d0(0x8fc);

    Func_0808a248(token);
}
