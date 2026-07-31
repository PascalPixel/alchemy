typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef signed int s32;

/*
 * Resource 386 approach guard at 0x02000458 — member 0 of a four-member
 * family (0x0200_0380, 0x0200_03c8, 0x0200_0410, 0x0200_0458).
 *
 * Complete owner: `push {r5, lr}` at 0x02000458, interworking return
 * `pop {r5} / pop {r0} / bx r0` 56 bytes later.  The control-flow walk
 * reaches every halfword of the body; the last 14 bytes of the 72-byte span
 * are the literal pool (0xffff5fff, 0x00003ffe, 0x00001cec) plus one
 * alignment halfword, reached only by the three `ldr rN,[pc,#imm]` loads.
 *
 * Signature: `pop {r0} / bx r0` puts the return address in r0 — `void`.
 *
 * Link base 0x02008000; 0x0200 80458 in the overlay's handler pool is
 * `Func_02000458 + 1`, which named this row as a scene handler in advance.
 *
 * FAMILY PROOF (byte diff, not inference).  Over all 72 bytes this row and
 * 0x02000380 differ in exactly the halfwords listed in that file's comment:
 * three immediates and one pool word here, and for 0x02000458 additionally
 * the `bl` halfword that selects Func_080b0018 over Func_080b0008.  The
 * bodies are otherwise bit-identical, so the four files are transpositions of
 * one another with the diff as their correctness proof.
 *
 * The guard: the u16 at +6 of actor record 0 is a heading/position word.
 * Adding 0xffff5fff (that is, subtracting 0xa001) and comparing unsigned
 * against 0x3ffe is a half-open band test on the wrapped value — the
 * documented "in range" idiom, and the reason the constant looks like a mask.
 * Inside the band the scene runs its transition; outside it the actor simply
 * says the refusal line.
 */

void Func_0808a018();  /* veneer 0x63c — scene begin */
void Func_0808a020();  /* veneer 0x644 — scene end */
u8  *Func_0808a080();  /* veneer 0x664 — actor record accessor */
void Func_0808a170();  /* veneer 0x694 — message */
void Func_0808a180();  /* veneer 0x6a4 — act */
void Func_080b0018();  /* veneer 0x6d4 */

void Func_02000458(void)
{
    u32 place;

    place = *(u16 *)(Func_0808a080(0) + 6);
    Func_0808a018();

    if ((u32) (place + 0xffff5fff) <= 0x3ffe) {
        Func_080b0018(1, 22);
    } else {
        Func_0808a170(0x1cec);
        Func_0808a180(22, 0);
    }

    Func_0808a020();
}
