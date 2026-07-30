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

u8 *Func_02005e5e();
void Func_02005e44();
void Func_02005f6e();
void Func_02005d04();
void Func_02005ee6();
void Func_02005ef0();
void Func_02005d26();
void Func_02005f8e();
void Func_02005fea();
void Func_02005ec4();
void Func_02005f06();
void Func_0200600a();
void Func_02006016();
void Func_02005e64();
void Func_02005e72();
void Func_02005fe8();

void Func_02001b5c(void)
{
    u8 *actor = Func_02005e5e(8);

    Func_02005e44(actor);
    Func_02005f6e(-1, -1, -1, 0);
    Func_02005d04(1);
    Func_02005ee6(0, 0, 0);
    Func_02005ef0(8, 0x13e80000, 0x09180000);

    *(s32 *)(actor + 0x1c) = 0x14000;
    *(s32 *)(actor + 0x18) = 0x14000;

    Func_02005d26(1);
    Func_02005f8e(8, 1);
    Func_02005fea();
    Func_02005ec4(8, 0x6666, 0x3333);
    Func_02005f06(8, 0x13c8, 0x918);
    Func_0200600a();
    Func_02006016();
    Func_02005e64(0x93e);
    Func_02005e72(0x927);
    Func_02005fe8(107);
    Func_02005ec4();
}
