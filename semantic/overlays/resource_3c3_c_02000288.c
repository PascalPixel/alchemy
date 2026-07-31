typedef signed short s16;
typedef unsigned char u8;
typedef signed int s32;

/*
 * Resource 3c3, scene beat at 0x02000288 (112 bytes, 8 call sites).
 *
 * Complete owner: `push {r5, r6, lr}` at 0x02000288 with `sub sp, #8` at
 * 0x02000296, and `add sp, #8 / pop {r5, r6} / pop {r0} / bx r0` at
 * 0x020002ec.  The popped branch register IS r0, so it holds the return
 * address and the owner is `void`.  Bytes 0x020002f4-0x020002f7 are a
 * one-word literal pool (0x03001ebc).
 *
 * `Data_03001ebc` is a pointer CELL: the code does `ldr r3,[pc] / ldr r3,[r3]`,
 * so one dereference gets the workspace base.  The signed halfword at
 * workspace + 364 (built as `movs r2,#182 / lsls r2,#1`) is the completion
 * token this beat hands back to Func_0808a248 at the end — it is read BEFORE
 * any call, then held in r6 across the whole body.
 *
 * `actor[85] = 0` is a byte store into the record returned by
 * Func_0808a080(0), performed with `adds r0,#85 / strb r3,[r0,#0]` and with no
 * null check, unlike the guarded Func_0808a080 sites elsewhere in this
 * overlay.  The bytes are not in doubt; noted rather than "fixed".
 *
 * The two Func_08009180 blocks are the same six-argument request with only the
 * first argument changing (0x42 then 0x44); r5 holds the constant 2 that fills
 * both stack slots for both of them.  -16 is a `movs r2,#16 / negs r2,r2`
 * pair.
 *
 * Call targets resolved with `tools/overlay_call_targets.ts`; all 8 sites are
 * import veneers.  Per-target multiset, reproduced exactly by the C below:
 *   Func_080000c0 x2, Func_08009180 x2, Func_0808a080 x1, Func_0808a0d8 x1,
 *   Func_0808a248 x1, Func_080f9010 x1.
 */

/* Old-style declarations: overlay import arities vary per call site. */
void Func_080000c0();   /* wait n frames */
void Func_08009180();   /* six-argument presentation request, last two on the stack */
u8 *Func_0808a080();    /* scene entity record by selector */
void Func_0808a0d8();   /* camera/offset adjustment */
void Func_0808a248();   /* scene request keyed by a selector */
void Func_080f9010();   /* play a cue by id */

extern u8 *Data_03001ebc;

void Func_02000288(void)
{
    u8 *workspace = Data_03001ebc;
    s16 token = *(s16 *)(workspace + 364);
    u8 *actor;

    actor = Func_0808a080(0);
    actor[85] = 0;

    Func_080f9010(158);

    Func_08009180(0x42, 0x24, 0x47, 8, 2, 2);
    Func_080000c0(4);

    Func_08009180(0x44, 0x24, 0x47, 8, 2, 2);
    Func_080000c0(4);

    Func_0808a0d8(0, 3, -16);

    Func_0808a248(token);
}
