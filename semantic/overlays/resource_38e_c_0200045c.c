typedef unsigned char u8;
typedef signed int s32;

/*
 * resource_38e owner at 0x0200045c, 96 bytes: a scene-script callback - open
 * the scene bracket, optionally run the shared 0x020008e8 preamble, shrink and
 * nudge entity 0, and close.
 *
 * Named before it was opened.  It is the middle word of the three-word
 * scene-script record
 *   0xffff0002, 0x0200845d, 0x00000000
 * in this overlay's data band; under the proven 0x02008000 link base
 * 0x0200845d is file offset 0x045c plus the Thumb bit.  Its neighbours in the
 * same table carry selectors 1, 8 and 9 in the same low-halfword position, and
 * `bun tools/overlay_call_targets.ts resource_38e` finds no `bl` anywhere in
 * the overlay targeting 0x045c - so the table is the only way in.
 *
 * Complete owner: `push {lr}` at 0x0200045c through the single epilogue at
 * 0x020004b2, ending `pop {r0} / bx r0` - void.
 *
 * Pool map from a control-flow walk: 0x0200045c-0x020004b5 is reached as
 * instructions, 0x020004b6-0x020004bb is not.  That pool is SIX bytes - one
 * alignment halfword plus the single word 0x03001ebc - the documented small
 * case, and assuming the customary 8 or 12 would have eaten the first
 * instruction of the next owner (0x020004bc, the overlay's entry-0 root).
 * 90 code + 6 pool = 96, exactly the inventory row's figures.
 *
 * Calls: 9 sites, matching the advertised 9, and 9 distinct targets:
 *   Func_0808a018  0x45e   Func_080770c0  0x466   Func_020008e8  0x46e
 *   Func_0808a090  0x47c   Func_0808a100  0x492   Func_0808a0d8  0x49e
 *   Func_0808a010  0x4a4   Func_0808a248  0x4aa   Func_0808a020  0x4ae
 * Func_020008e8 is this overlay's own owner and already has byte-exact C
 * (assets/code/resource_38e_c_020008e8.c); 0x020005dc calls it too.
 *
 * The workspace store is the CANONICAL form of the documented displacement/
 * value trap: r2 carries 448 as the offset, the address is formed from it, and
 * only then `subs r2, #192` turns the same register into the VALUE 32.  The
 * handover's worked example is this exact instruction pair.  Reading it as
 * workspace+256 receiving something, or as workspace+448 receiving 256, are
 * both natural and both wrong.
 *
 * This overlay's other owners write the same cell: the entry-0 root at
 * 0x020004bc stores 256 there and the cutscene at 0x020005dc stores 513, so
 * workspace+448 is a small scene-state code and 32 is one of its values.
 */

/* Pointer CELL holding the per-overlay workspace base, not the workspace. */
extern u8 *Data_03001ebc;

/* Old-style declarations: overlay imports vary in arity between call sites. */
s32 Func_080770c0();                 /* read an event flag */
void Func_0808a010();                /* wait this many frames */
void Func_0808a018();                /* scene bracket open */
void Func_0808a020();                /* scene bracket close */
void Func_0808a090();                /* set an x/y scale pair, 16.16 */
void Func_0808a0d8();                /* nudge an entity by a delta */
void Func_0808a100();                /* one-argument presentation call */
void Func_0808a248();                /* one-argument scene call */

void Func_020008e8(void);            /* this overlay, byte-exact source */

void Func_0200045c(void)
{
    u8 *workspace;

    Func_0808a018();

    if (Func_080770c0(512) == 0) {   /* 128 << 2 */
        Func_020008e8();
    }

    Func_0808a090(0, 0x00008000, 0x00004000);   /* 0.5, 0.25 in 16.16 */

    /* 448 is the OFFSET; 32 is the value stored there. */
    workspace = Data_03001ebc;
    *(s32 *)(workspace + 448) = 32;

    Func_0808a100(0, 2);
    Func_0808a0d8(0, 2, -16);
    Func_0808a010(16);
    Func_0808a248(2);
    Func_0808a020();
}
