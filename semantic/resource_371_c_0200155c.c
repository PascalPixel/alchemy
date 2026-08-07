/*
 * Resource 371 owner at 0x0200155c (292 bytes, 24 distinct call targets).
 *
 * Complete owner: `push {lr}` at 0x0200155c and the interworking return
 * `pop {r0} ; bx r0` at 0x02001656.  r0 holds the popped return
 * address, so the owner returns nothing.  Bytes
 * 0x0200165c-0x0200167f are the literal pool: nine 16.16
 * coordinate words, all reached only by `ldr rN, [pc, #imm]`.
 *
 * A flat placement carpet — twelve identical two-call pairs, 24 distinct
 * targets, matching the inventory count exactly and accounting for every
 * `bl`.  Each pair places one item at a 16.16 (X, Z) pair with the constant
 * second and fourth arguments -1 and 1, then issues the same follow-up with
 * the argument 4.  The recurring Z value `223 << 19` = 0x06f80000 is
 * built with
 * `movs`/`lsls` rather than pooled, which is why it appears inline below.
 *
 * This owner and 0x02001680 are the same body: they differ in
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

void Func_02005960();
void Func_020056f6();
void Func_02005974();
void Func_0200570a();
void Func_02005988();
void Func_0200571e();
void Func_0200599c();
void Func_02005732();
void Func_020059b0();
void Func_02005746();
void Func_020059c6();
void Func_0200575c();
void Func_020059dc();
void Func_02005772();
void Func_020059f0();
void Func_02005786();
void Func_02005a04();
void Func_0200579a();
void Func_02005a18();
void Func_020057ae();
void Func_02005a2c();
void Func_020057c2();
void Func_02005a42();
void Func_020057d8();

void Func_0200155c(void)
{
    Func_02005960(0x160c0000, -1, 0x06f80000, 1);
    Func_020056f6(4);
    Func_02005974(0x16040000, -1, 0x06fc0000, 1);
    Func_0200570a(4);
    Func_02005988(0x160c0000, -1, 0x06f40000, 1);
    Func_0200571e(4);
    Func_0200599c(0x160c0000, -1, 0x06fc0000, 1);
    Func_02005732(4);
    Func_020059b0(0x16040000, -1, 0x06f40000, 1);
    Func_02005746(4);
    Func_020059c6(0x16080000, -1, 0x06f80000, 1);
    Func_0200575c(4);
    Func_020059dc(0x160a0000, -1, 0x06f80000, 1);
    Func_02005772(4);
    Func_020059f0(0x16060000, -1, 0x06fa0000, 1);
    Func_02005786(4);
    Func_02005a04(0x160a0000, -1, 0x06f60000, 1);
    Func_0200579a(4);
    Func_02005a18(0x160a0000, -1, 0x06fa0000, 1);
    Func_020057ae(4);
    Func_02005a2c(0x16060000, -1, 0x06f60000, 1);
    Func_020057c2(4);
    Func_02005a42(0x16080000, -1, 0x06f80000, 1);
    Func_020057d8(4);
}
