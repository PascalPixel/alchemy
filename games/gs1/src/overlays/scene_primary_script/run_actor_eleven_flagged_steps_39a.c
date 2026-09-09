#include "types.h"

/* Imports; the queried ones are typed for their return value. */
void Func_02000cac();
void Func_02000cda();
void Func_02000d02();
void Func_02001e78();
s32 Func_02002e2a();
s32 Func_02002e34();
void Func_02002e5a();
void Func_02002e60();
s32 Func_02002e62();
s32 Func_02002e62_a();
void Func_02002e88();
void Func_02002e88_a();
void Func_02002e96();
void Func_02002e9c();
void Func_02002ea8();
void Func_02002eb8();
void Func_02002ebe();
void Func_02002ec4();
void Func_02002e08();
void Func_02002f1a();
void Func_02002fb2();

/*
 * Field scene step for overlay resource_39a.  Imports are named by the address
 * their call site computes, not by a location in this image, and their
 * interfaces are left open.  Func_02002e62 is reached both as a setter and as
 * a query, so its result is dropped at the setter site.  The first two arms
 * share one tail call, which is why Func_02002e9c is spelled out in each arm.
 */
void FieldScene_RunActorElevenFlaggedSteps(void)
{
    Func_02002f1a((s32)0xf1);
    /* 0x308 is built by shifting a small immediate, not loaded whole. */
    if (Func_02002e2a((s32)0x308) != 0 || Func_02002e34((s32)0x30d) != 0) {
        Func_02000cac(11, 0, -64);
        Func_02002e5a((s32)0x30f);
        Func_02002e62((s32)0x310);
        Func_02002e60((s32)0x311);
        Func_02002e9c((s32)0x312);
        /* 0x30c is built by shifting a small immediate, not loaded whole. */
    } else if (Func_02002e62_a((s32)0x30c) != 0) {
        Func_02000cda(11, 0, -112);
        Func_02002e88((s32)0x30f);
        Func_02002e88_a((s32)0x310);
        Func_02002e96((s32)0x311);
        Func_02002e9c((s32)0x312);
    } else {
        Func_02000d02(11, 0, -128);
        Func_02002ea8((s32)0x30f);
        Func_02002eb8((s32)0x310);
        Func_02002ebe((s32)0x311);
        Func_02002ec4((s32)0x312);
    }
    Func_02002fb2((s32)0x121);
    Func_02002e08(2);
    Func_02001e78();
}
