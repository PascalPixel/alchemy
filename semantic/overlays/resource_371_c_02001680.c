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
 * the argument 4.  The recurring Z value `217 << 19` = 0x06c80000 is built with
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

void Func_02005a84();
void Func_0200581a();
void Func_02005a98();
void Func_0200582e();
void Func_02005aac();
void Func_02005842();
void Func_02005ac0();
void Func_02005856();
void Func_02005ad4();
void Func_0200586a();
void Func_02005aea();
void Func_02005880();
void Func_02005b00();
void Func_02005896();
void Func_02005b14();
void Func_020058aa();
void Func_02005b28();
void Func_020058be();
void Func_02005b3c();
void Func_020058d2();
void Func_02005b50();
void Func_020058e6();
void Func_02005b66();
void Func_020058fc();

void Func_02001680(void)
{
    Func_02005a84(0x15ec0000, -1, 0x06c80000, 1);
    Func_0200581a(4);
    Func_02005a98(0x15e40000, -1, 0x06cc0000, 1);
    Func_0200582e(4);
    Func_02005aac(0x15ec0000, -1, 0x06c40000, 1);
    Func_02005842(4);
    Func_02005ac0(0x15ec0000, -1, 0x06cc0000, 1);
    Func_02005856(4);
    Func_02005ad4(0x15e40000, -1, 0x06c40000, 1);
    Func_0200586a(4);
    Func_02005aea(0x15e80000, -1, 0x06c80000, 1);
    Func_02005880(4);
    Func_02005b00(0x15ea0000, -1, 0x06c80000, 1);
    Func_02005896(4);
    Func_02005b14(0x15e60000, -1, 0x06ca0000, 1);
    Func_020058aa(4);
    Func_02005b28(0x15ea0000, -1, 0x06c60000, 1);
    Func_020058be(4);
    Func_02005b3c(0x15ea0000, -1, 0x06ca0000, 1);
    Func_020058d2(4);
    Func_02005b50(0x15e60000, -1, 0x06c60000, 1);
    Func_020058e6(4);
    Func_02005b66(0x15e80000, -1, 0x06c80000, 1);
    Func_020058fc(4);
}
