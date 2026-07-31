typedef unsigned char u8;
typedef signed int s32;

/*
 * Resource 371 owner at 0x02001b5c (172 bytes, 16 distinct call targets).
 *
 * Complete owner: `push {r5, lr}` at 0x02001b5c and the interworking return
 * `pop {r5} ; pop {r0} ; bx r0` at 0x02001be2.  r0 holds the popped return
 * address, so the owner returns nothing.  Bytes 0x02001be8-0x02001c07 are
 * the literal pool (0x09180000, 0x13e80000, 0x6666, 0x3333, 0x13c8, 0x918,
 * 0x93e, 0x927).
 *
 * The same cutscene set-up shape as 0x020017fc against a different import
 * set.  Seventeen `bl`s reach sixteen distinct targets — 0x02005ec4 is
 * called twice, at 0x02001bb6 and at 0x02001bde — which matches the
 * inventory's distinct-target count of 16 and accounts for every call.
 *
 * Argument recovery: `bl 0x02005e44` at 0x02001b66 sets no register, but r0
 * still holds the object returned by 0x02005e5e, so that dataflow is
 * preserved.  0x02005fea, 0x0200600a, 0x02006016 and the second 0x02005ec4
 * follow another `bl` with nothing set, so no arguments are asserted.
 *
 * The two 16.16 pool constants 0x13e80000 and 0x09180000 are a coordinate
 * pair (5096.0, 2328.0); 0x6666/0x3333 and 0x13c8/0x918 are the plain
 * integer pairs the corresponding imports take in the sibling at 0x020017fc.
 *
 * Call-target convention: each `bl` is named by the address its call site
 * computes; that address is a per-call-site label for a
 * load-time-relocated import, not a place to disassemble and not a global
 * identity (see the note in resource_371_c_0200008c.c).  Old-style
 * declarations, because the interfaces are unknown and one import can be
 * reached with different argument counts.
 */

u8 *Func_0808a080();
void Func_0808a018();
void Func_0808a210();
void Func_080000c0();
void Func_0808a0f0();
void Func_0808a200();
void Func_0808a360();
void Func_0808a090();
void Func_0808a020();
void Func_0808a0d0();
void Func_0808a368();
void Func_0808a370();
void Func_080770c8();
void Func_080770d0();
void Func_0808a248();

void Func_02001b5c(void)
{
    u8 *actor = Func_0808a080(8);

    Func_0808a018(actor);
    Func_0808a210(-1, -1, -1, 0);
    Func_080000c0(1);
    Func_0808a0f0(0, 0, 0);
    Func_0808a0f0(8, 0x13e80000, 0x09180000);

    *(s32 *)(actor + 0x1c) = 0x14000;
    *(s32 *)(actor + 0x18) = 0x14000;

    Func_080000c0(1);
    Func_0808a200(8, 1);
    Func_0808a360();
    Func_0808a090(8, 0x6666, 0x3333);
    Func_0808a0d0(8, 0x13c8, 0x918);
    Func_0808a368();
    Func_0808a370();
    Func_080770c8(0x93e);
    Func_080770d0(0x927);
    Func_0808a248(107);
    Func_0808a020();
}
