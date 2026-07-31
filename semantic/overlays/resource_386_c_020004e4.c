typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;

/*
 * Resource 386 overlay initialiser at 0x020004e4.
 *
 * This is the overlay's ROOT: entry 0 of the exported-entry veneer table at
 * file offset 0 is `ldr r4,[pc,#0] / bx r4 / .word 0x020084e5`, and
 * 0x020084e5 - 0x8000 = 0x4e5 = `Func_020004e4 + 1`.  Everything else in the
 * overlay is reached from here or from the handler pool this owner's mode
 * selects.
 *
 * Complete owner: `push {r5, lr}` at 0x020004e4 with `sub sp, #8` at
 * 0x020004fe for the two stacked arguments of Func_08009180; interworking
 * return `add sp, #8 / pop {r5} / pop {r1} / bx r1` at 0x0200055c-0x02000562.
 * The control-flow walk reaches every halfword to 0x02000562;
 * 0x02000564-0x0200056f is the literal pool (0x03001ebc, 0x02000240,
 * 0x02008031).  Next owner at 0x02000570.
 *
 * Signature.  The return address is popped into r1, NOT r0, so r0 survives
 * and IS the result (HANDOVER section 0).  r0 is set to 0 immediately before
 * the epilogue on every path, so the owner returns a constant 0 status.
 *
 * LINK-BASE PROOF, and the strongest one available here.  The pool word
 * 0x02008031 is odd; under `offset = value - 0x8000` it is `0x30 + 1`, that
 * is `Func_02000030` plus the Thumb bit — and assets/code/resource_386_c_
 * 02000030.c is a banked byte-exact source for exactly that address.  It is
 * passed to Func_080000d0, the per-frame task installer, so this word is a
 * task callback and not data.  That single word proves the base against
 * already-banked material and names 0x02000030's role at the same time.
 *
 * The displacement/value trap, additive form (HANDOVER).  r2 is built as
 * 224 << 1 = 448 and used as the workspace displacement; `adds r2, #73`
 * turns the SAME register into the stored value 521; `subs r2, #71` then
 * turns it into the displacement 450 of the following load.  Reading it as
 * 448 - 71 or as a single running offset is the natural mistake, and nothing
 * else in the owner would catch it.
 *
 * Data_02000240 is the cross-overlay RAM global block, not an in-image
 * address: 0x02000240 lies BELOW the 0x02008000 link band, so it cannot be a
 * file offset here.  The signed halfword read out of it selects the scene.
 */

/* Old-style declarations; Func_02000570 is called with three further
 * registers set that it does not read — see that file. */
void Func_08009180();
void Func_080000d0();
u8  *Func_0808a080();  /* veneer 0x664 — actor record accessor */
void Func_02000570();

#define WORKSPACE (*(u8 **)0x03001ebc)

s32 Func_020004e4(void)
{
    s32 scene;

    *(s32 *)(WORKSPACE + 448) = 521;
    scene = *(s16 *)((u8 *)0x02000240 + 450);

    if (scene == 5) {
        /* Two arguments are passed on the stack: 4 then 3. */
        Func_08009180(0, 120, 8, 67, 4, 3);
        Func_0808a080(8)[0x55] = 0;
        *(s32 *)(Func_0808a080(8) + 12) = 0;
        *(s32 *)(Func_0808a080(8) + 20) = 0;
    } else if (scene == 7 || scene == 11) {
        /* Built by shifts: 142 << 18, 128 << 13, 168 << 18. */
        Func_02000570(0xe7, 0x02380000, 0x00100000, 0x02a00000);
        /* 0x02008031 is Func_02000030 + 1; 200 << 4 is the period. */
        Func_080000d0(0x02008031, 0xc80);
    }

    return 0;
}
