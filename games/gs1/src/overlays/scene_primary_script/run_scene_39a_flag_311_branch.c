#include "types.h"

/* Imports; the queried ones are typed for their return value. */
void Func_02000aba();
void Func_02000ae6();
void Func_02000b0c();
void Func_02001c82();
void Func_02002c12();
s32 Func_02002c44();
void Func_02002c68();
void Func_02002c6e();
s32 Func_02002c70();
s32 Func_02002c70_a();
void Func_02002c94();
void Func_02002c94_a();
void Func_02002ca2();
void Func_02002ca8();
void Func_02002cba();
void Func_02002cc2();
void Func_02002cc8();
void Func_02002cce();
void Func_02002d36();
void Func_02002dbc();

/*
 * Scene step for resource_39a.  Nothing is returned; the five pool words
 * after the return belong to the owner.  Func_02002c70 serves both a query
 * and a setter, so its result is dropped at the setter site.  The tail call
 * shared by the first two arms is written out in each arm rather than adding
 * a flag the reference does not have.  Imports are named by the address their
 * call site computes, and are old-style because arity varies between sites.
 */
void FieldScene_RunFlag311DialogueBranch(void)
{
    Func_02002d36((s32)0xf1);
    if (Func_02002c44((s32)0x311) != 0) {
        Func_02000aba(10, 0, 48);
        Func_02002c68((s32)0x30b);
        /* 0x30c is built by shifting; the result is unused. */
        Func_02002c70((s32)0x30c);
        Func_02002c6e((s32)0x30d);
        Func_02002ca8((s32)0x30e);
        /* 0x310 is built by shifting. */
    } else if (Func_02002c70_a((s32)0x310) != 0) {
        Func_02000ae6(10, 0, 32);
        Func_02002c94((s32)0x30b);
        Func_02002c94_a((s32)0x30c);
        Func_02002ca2((s32)0x30d);
        Func_02002ca8((s32)0x30e);
    } else {
        Func_02000b0c(10, 0, 112);
        Func_02002cba((s32)0x30b);
        Func_02002cc2((s32)0x30c);
        Func_02002cc8((s32)0x30d);
        Func_02002cce((s32)0x30e);
    }
    Func_02002dbc((s32)0x121);
    Func_02002c12(2);
    Func_02001c82();
}
