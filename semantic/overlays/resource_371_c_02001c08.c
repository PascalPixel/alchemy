/*
 * Resource 371 owner at 0x02001c08 (156 bytes, 20 distinct call targets).
 *
 * Complete owner: `push {lr}` at 0x02001c08 and the interworking return
 * `pop {r0} ; bx r0` at 0x02001c90.  r0 holds the popped return address, so
 * the owner returns nothing.  Bytes 0x02001c94-0x02001ca3 are the literal
 * pool (0xc66, 0x16f, 0x171, 0xc67).
 *
 * A cutscene step built from two near-identical halves — one per participant
 * — that differ only in their import addresses and in the 0xc66/0xc67 and
 * 0x16f/0x171 pool identifiers.  Twenty-one `bl`s reach twenty distinct
 * targets (0x020060a2 is called at both 0x02001c14 and 0x02001c8c), which
 * matches the inventory's distinct-target count of 20 and accounts for every
 * call.
 *
 * Argument recovery: the calls that set no register and follow another `bl`
 * — 0x020060ae, 0x0200607a, 0x020060f4, 0x0200611c — receive only
 * call-clobbered values, so no arguments are asserted for them.  The leading
 * 0x02006070 is likewise written with no arguments: nothing sets r0 before
 * it and this owner has no evidence of taking one (it is not installed as a
 * callback anywhere in the image).
 *
 * UNCERTAINTY: if this owner is in fact reached with a live r0, the first
 * call would forward it.  No call site was found to settle that, so the
 * conservative reading is recorded here rather than an invented parameter.
 *
 * Call-target convention: each `bl` is named by the address its call site
 * computes; that address is a per-call-site label for a
 * load-time-relocated import, not a place to disassemble and not a global
 * identity (see the note in resource_371_c_0200008c.c).  Old-style declarations, because the interfaces are unknown and
 * one import can be reached with different argument counts.
 */

void Func_02006070();
void Func_020060a2();
void Func_020060ae();
void Func_0200607a();
void Func_02005faa();
void Func_02005fd0();
void Func_02005fe8();
void Func_02005f0e();
void Func_020060e4();
void Func_02005ecc();
void Func_02005eea();
void Func_02005ef0();
void Func_020060f4();
void Func_02005fd6();
void Func_0200600c();
void Func_02006024();
void Func_02005f12();
void Func_02005f10();
void Func_0200611c();
void Func_02005f5a();

void Func_02001c08(void)
{
    Func_02006070();
    Func_020060a2(0x10000, 6);
    Func_020060ae();
    Func_0200607a();

    Func_02005faa(8, 2);
    Func_02005fd0(0xc66);
    Func_02005fe8(8, 0);
    Func_02005f0e(30);
    Func_020060e4(111);
    Func_02005ecc(0, 2);
    Func_02005eea(0x16f);
    Func_02005ef0(0x171);
    Func_020060f4();

    Func_02005fd6(8, 4, 30);
    Func_0200600c(0xc67);
    Func_02006024(8, 0);
    Func_02005f12(0x16f);
    Func_02005f10(0x171);
    Func_0200611c();

    Func_02005f5a(30);
    Func_020060a2(12, 6);
}
