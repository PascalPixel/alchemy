#include "types.h"

/* Imports; the queried ones are typed for their return value. */
void Func_0200098c();
void Func_020009b8();
void Func_020009e2();
void Func_02000a08();
void Func_02001b7e();
void Func_02002b0e();
s32 Func_02002b16();
void Func_02002b3a();
void Func_02002b3a_a();
s32 Func_02002b42();
void Func_02002b48();
void Func_02002b66();
void Func_02002b66_a();
s32 Func_02002b6c();
void Func_02002b74();
void Func_02002b90();
void Func_02002b96();
void Func_02002b98();
void Func_02002ba4();
void Func_02002bb6();
void Func_02002bbe();
void Func_02002bc4();
void Func_02002bca();
void Func_02002c06();
void Func_02002cb8();

/*
 * Scene step for resource_39a.  Nothing is returned; the five pool words
 * after the return belong to the owner.  The tail call shared by the first
 * three arms is written out in each arm rather than adding control flow the
 * reference does not have.  Imports are named by the address their call site
 * computes, and are old-style because arity varies between sites.
 */
void FieldScene_RunFlag308DialogueBranch(void)
{
    Func_02002c06((s32)0xf1);
    /* 0x308 is built by shifting. */
    if (Func_02002b16((s32)0x308) != 0) {
        Func_0200098c(10, 0, 16);
        Func_02002b3a((s32)0x30b);
        Func_02002b3a_a((s32)0x30c);
        Func_02002b48((s32)0x30d);
        Func_02002ba4((s32)0x30e);
        /* 0x310 is built by shifting. */
    } else if (Func_02002b42((s32)0x310) != 0) {
        Func_020009b8(10, 0, 16);
        Func_02002b66((s32)0x30b);
        Func_02002b66_a((s32)0x30c);
        Func_02002b74((s32)0x30d);
        Func_02002ba4((s32)0x30e);
    } else if (Func_02002b6c((s32)0x311) != 0) {
        Func_020009e2(10, 0, 64);
        Func_02002b90((s32)0x30b);
        Func_02002b98((s32)0x30c);
        Func_02002b96((s32)0x30d);
        Func_02002ba4((s32)0x30e);
    } else {
        Func_02000a08(10, 0, 128);
        Func_02002bb6((s32)0x30b);
        Func_02002bbe((s32)0x30c);
        Func_02002bc4((s32)0x30d);
        Func_02002bca((s32)0x30e);
    }
    Func_02002cb8((s32)0x121);
    Func_02002b0e(2);
    Func_02001b7e();
}
