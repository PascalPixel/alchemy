/*
 * Resource 371 owner at 0x02001680 (292 bytes, 24 distinct call targets).
 *
 * Complete owner: `push {lr}` at 0x02001680 and the interworking return
 * `pop {r0} ; bx r0` at 0x0200177a.  r0 holds the popped return
 * address, so the owner returns nothing.  Bytes
 * 0x02001780-0x020017a3 are the literal pool: nine 16.16
 * coordinate words, all reached only by `ldr rN, [pc, #imm]`.
 *
 * A flat placement carpet — twelve identical two-call pairs, 24 distinct
 * targets, matching the inventory count exactly and accounting for every
 * `bl`.  Each pair places one item at a 16.16 (X, Z) pair with the constant
 * second and fourth arguments -1 and 1, then issues the same follow-up with
 * the argument 4.  The recurring Z value `217 << 19` = 0x06c80000 is
 * built with
 * `movs`/`lsls` rather than pooled, which is why it appears inline below.
 *
 * This owner and 0x0200155c are the same body: they differ in
 * thirteen halfwords only — the inline Z immediate and the nine pool words —
 * and their `bl` encodings are bit-identical, so their printed targets differ
 * by exactly the 0x124 spacing between the two owners.  The 24 imports named
 * here and the 24 named there are the same 24 callees.  See
 * resource_371_c_0200008c.c for the same proof on a smaller pair.
 *
 * Call-target convention: each `bl` is named by the address its call site
 * computes, which is a per-call-site label for a load-time-relocated import.
 * Old-style declarations, because the interfaces are unknown.
 */

void Func_0808a210();
void Func_080000c0();

void Func_02001680(void)
{
    Func_0808a210(0x15ec0000, -1, 0x06c80000, 1);
    Func_080000c0(4);
    Func_0808a210(0x15e40000, -1, 0x06cc0000, 1);
    Func_080000c0(4);
    Func_0808a210(0x15ec0000, -1, 0x06c40000, 1);
    Func_080000c0(4);
    Func_0808a210(0x15ec0000, -1, 0x06cc0000, 1);
    Func_080000c0(4);
    Func_0808a210(0x15e40000, -1, 0x06c40000, 1);
    Func_080000c0(4);
    Func_0808a210(0x15e80000, -1, 0x06c80000, 1);
    Func_080000c0(4);
    Func_0808a210(0x15ea0000, -1, 0x06c80000, 1);
    Func_080000c0(4);
    Func_0808a210(0x15e60000, -1, 0x06ca0000, 1);
    Func_080000c0(4);
    Func_0808a210(0x15ea0000, -1, 0x06c60000, 1);
    Func_080000c0(4);
    Func_0808a210(0x15ea0000, -1, 0x06ca0000, 1);
    Func_080000c0(4);
    Func_0808a210(0x15e60000, -1, 0x06c60000, 1);
    Func_080000c0(4);
    Func_0808a210(0x15e80000, -1, 0x06c80000, 1);
    Func_080000c0(4);
}
