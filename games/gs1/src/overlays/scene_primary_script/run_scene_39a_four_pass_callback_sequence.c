#include "types.h"

/* Imports, named by the address each call site computes. */
void Func_02004224();
void Func_02004224_a();
void Func_0200422a();
void Func_02004156();
void Func_0200421a();
void Func_020041e4();
void Func_020041fa();
void Func_020040a0();
void Func_0200412e();
void Func_0200412e_a();
void Func_0200413e();
void Func_0200421e();
void Func_02004234();
void Func_020040dc();
void Func_020040e6();
void Func_020040fa();
void Func_020040f8();
void Func_02004250();
void Func_02004266();
void Func_0200410c();
void Func_02004122();
void Func_02004118();
void Func_02004272();
void Func_02004288();
void Func_020041f6();
void Func_0200420c();
void Func_020042e0();

extern void Func_02009d78();
extern

/*
 * Field scene sequence for overlay resource_39a.  Imports are named by the
 * address their call site computes, not by a location in this image, and
 * their interfaces are left open.  The declarations are old-style because
 * Func_02004224 and Func_0200412e are each reached with two different
 * argument counts.  Func_02009d78 names a loader-relocated call word handed
 * to two imports as a callback, not a runtime address.
 */
void FieldScene_RunFourPassCallbackSequence(void)
{
    s32 pass;
    s32 step;
    s32 span;
    s32 one;

    Func_02004224(19);
    Func_0200422a(182);
    Func_02004156();
    Func_0200421a();

    /* 8, 7 and 1 are locals held across the loop, not literals: the first
     * call takes 8 as an immediate for argument 4 and from a register for
     * argument 5, which a literal cannot produce. */
    pass = 0;
    step = 8;
    span = 7;
    one = 1;
    do {
        Func_020041e4((s32)0x204318, 1);
        Func_020041fa(1);
        Func_020040a0(2);
        if (pass == 0) {
            Func_0200412e(30, 8, 12, 8, step, span);
            Func_0200413e(30, 57, 19, 57, one, one);
        }
        Func_0200421e((s32)0x203108, 1);
        Func_02004234(1);
        Func_020040dc(2);
        /* The increment belongs to the loop test, not the body: `pass++;` as
         * a statement would not place it after the last call.  The compare is
         * unsigned against 3, so the body runs for pass 0 to 3. */
    } while ((unsigned int)++pass <= 3);

    Func_020040e6(30);
    /* 0xc80 is built by shifting a small immediate, not loaded whole. */
    Func_020040fa((void *)Func_02009d78, (s32)0xc80);
    Func_020040f8(40);
    Func_02004250((s32)0x201090, 1);
    Func_02004266(40);
    Func_0200410c(80);
    Func_02004122((void *)Func_02009d78);
    Func_02004118(20);
    /* 0x10000 is built by shifting a small immediate, not loaded whole. */
    Func_02004272((s32)0x10000, 1);
    Func_02004288(80);
    /* Same import as in the loop, one argument here. */
    Func_0200412e_a(80);
    /* 0x820 is built by shifting a small immediate, not loaded whole. */
    Func_020041f6((s32)0x820);
    Func_0200420c(230);
    Func_020042e0();
    /* Same import as the first call, no argument register written here. */
    Func_02004224_a();
}
