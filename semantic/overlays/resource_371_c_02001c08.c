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
 * identity (see the note in resource_371_c_0200008c.c).  Old-style
 * declarations, because the interfaces are unknown and one import can be
 * reached with different argument counts.
 */

void Func_0808a3e0();
void Func_0808a5c0();
void Func_0808a250();
void Func_0808a5c8();
void Func_0808a3d8();
void Func_0808a138();
void Func_0808a170();
void Func_0808a180();
void Func_0808a010();
void Func_080f9010();
void Func_080153e8();
void Func_080770d0();
void Func_080a1040();
void Func_0808a128();
void Func_080770c8();

void Func_02001c08(void)
{
    Func_0808a3e0();
    Func_0808a5c0(0x10000, 6);
    Func_0808a5c8();
    Func_0808a3d8();

    Func_0808a138(8, 2);
    Func_0808a170(0xc66);
    Func_0808a180(8, 0);
    Func_0808a010(30);
    Func_080f9010(111);
    Func_080153e8(0, 2);
    Func_080770d0(0x16f);
    Func_080770d0(0x171);
    Func_080a1040();

    Func_0808a128(8, 4, 30);
    Func_0808a170(0xc67);
    Func_0808a180(8, 0);
    Func_080770d0(0x16f);
    Func_080770c8(0x171);
    Func_080a1040();

    Func_0808a010(30);
    Func_0808a250(12, 6);
}
