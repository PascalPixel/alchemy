typedef unsigned char u8;

/*
 * Resource 3c3, flashing bracket at 0x020007ac (176 bytes, 22 call sites).
 *
 * Complete owner: `push {r5, lr}` at 0x020007ac, `pop {r5} / pop {r0} / bx r0`
 * at 0x0200084e.  The popped branch register IS r0, so it holds the return
 * address and the owner is `void`.  Bytes 0x02000854-0x0200085b are the
 * two-word literal pool (0x0000266d, a dialogue line id, and 0x00000897, a
 * story flag); 0x0200085c begins the overlay's import veneer table, so this is
 * the last owner in the image.
 *
 * The body is two `do`/`while` loops that alternate slot 10's palette level
 * between 0 and 15, each iteration pushing the record through
 * Func_080091e0 with phase 1 then phase 0.  They are genuine loops, not
 * repeated blocks: `bls.n` at 0x020007fc branches back to 0x020007c4 and
 * `bls.n` at 0x02000838 back to 0x02000800, so the six and twelve iterations
 * each contribute ONE call site per import, not six and twelve.  The two loops
 * differ only in the frame wait (4 then 2) and the trip count (r5 <= 5 then
 * r5 <= 11) — a faster flash for the second run.
 *
 * `Func_080091e0(Func_0808a080(10), phase)` is one expression in the assembly
 * too: r0 comes straight out of the Func_0808a080 call and r1 is set between
 * the two `bl`s.
 *
 * Call targets resolved with `tools/overlay_call_targets.ts`; all 22 sites are
 * import veneers.  Per-target multiset, reproduced exactly by the C below:
 *   Func_080000c0 x4, Func_080091e0 x4, Func_0808a080 x4, Func_0808a158 x4,
 *   and one each of Func_080770c8, Func_0808a018, Func_0808a020,
 *   Func_0808a0f0, Func_0808a170, Func_0808a188.
 */

/* Old-style declarations: overlay import arities vary per call site. */
void Func_080000c0();   /* wait n frames */
void Func_080091e0();   /* set presentation mode (record, phase) */
void Func_080770c8();   /* set a story flag */
void Func_0808a018();   /* open a scripted scene */
void Func_0808a020();   /* close a scripted scene */
u8 *Func_0808a080();    /* scene entity record by selector */
void Func_0808a0f0();   /* place a slot at a 16.16 position */
void Func_0808a158();   /* set a slot's palette level */
void Func_0808a170();   /* show a dialogue line by id */
void Func_0808a188();   /* move the slot along a heading */

void Func_020007ac(void)
{
    unsigned int beat;

    Func_0808a018();

    Func_0808a170(0x266d);
    Func_0808a188(10, 0, 10);

    beat = 0;
    do {
        Func_0808a158(10, 0);
        Func_080091e0(Func_0808a080(10), 1);
        Func_080000c0(4);

        Func_0808a158(10, 15);
        Func_080091e0(Func_0808a080(10), 0);
        beat++;
        Func_080000c0(4);
    } while (beat <= 5);

    beat = 0;
    do {
        Func_0808a158(10, 0);
        Func_080091e0(Func_0808a080(10), 1);
        Func_080000c0(2);

        Func_0808a158(10, 15);
        Func_080091e0(Func_0808a080(10), 0);
        beat++;
        Func_080000c0(2);
    } while (beat <= 11);

    Func_0808a0f0(10, 0, 0);

    Func_080770c8(0x897);

    Func_0808a020();
}
