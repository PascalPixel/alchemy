#include "types.h"

extern u8 Data_02000240[];

/*
 * The message/event dispatcher for resource_3b9: read the selector from the
 * shared table, subtract 5, and dispatch through a 66-entry jump table. Only
 * twelve selectors act; the rest fall through. The 508-byte owner includes the
 * alignment halfword and the five-word literal pool after the return.
 */

/* A Func_ name here is a loader-relocated call word, not a runtime address.
 * Func_020047ec and Func_020047ec_a share an encoded word but reach different
 * veneers, so the two declarations must stay distinct; the suffix marks the
 * second call word, not a second entry point. */

extern void Func_020045de(s32 arg0);
extern void Func_020047da(s32 arg0, s32 arg1);
extern void Func_020047e2(s32 arg0, s32 arg1);
extern void Func_020047ec(s32 arg0, s32 arg1);
extern void Func_020047ec_a(s32 arg0);
extern void Func_020047f4(s32 arg0, s32 arg1);
extern s32 Func_0200475a(s32 arg0);
extern void Func_020031b2(void);
extern void Func_02002e44(void);
extern void Func_0200323a(void);
extern void Func_0200388c(void);
extern void Func_020047a0(void);
extern void Func_02003fa2(void);
extern void Func_020041b8(void);
extern void Func_02004796(s32 arg0);
extern void Func_020046f2(void);
extern s32 Func_02004798(s32 arg0);
extern void Func_020040bc(void);
extern void Func_020047c0(s32 arg0);
extern void Func_020047c6(s32 arg0);
extern void Func_020047cc(s32 arg0);
extern void Func_020047c2(s32 arg0);
extern void Func_0200426a(void);
extern void Func_02004428(void);
extern void Func_02004512(void);
extern void Func_020047ea(s32 arg0);
extern void Func_020047f0(s32 arg0);
extern void Func_020047f6(s32 arg0);
extern void Func_02004590(void);

/*
 * The case order is load-bearing and it is not the selector order: the arms
 * are laid out in the order the reference places their bodies, and the jump
 * table stores their absolute addresses. Each callee name is keyed to the
 * address of the instruction that calls it, so an arm moved out of place
 * emits the wrong call word.
 */
void FieldScene_DispatchBySelector(void)
{
    s32 no;

    Func_020045de(1);
    {
        s32 off = 450;

        no = *(s16 *)(Data_02000240 + off);
    }
    switch (no) {
    case 5:
        Func_020047da(8, 2);
        Func_020047e2(9, 2);
        break;
    case 69:
        Func_020047ec(8, 2);
        Func_020047f4(9, 2);
        if (Func_0200475a(0x109) != 0)
            break;
        Func_020031b2();
        break;
    case 7:
        Func_02002e44();
        break;
    case 70:
        Func_0200323a();
        break;
    case 64:
        Func_0200388c();
        Func_020047a0();
        break;
    case 65:
        Func_02003fa2();
        break;
    case 66:
        Func_020041b8();
        break;
    case 12:
        Func_02004796(324);
        Func_020046f2();
        if (Func_02004798(0x109) != 0)
            break;
        Func_020040bc();
        break;
    case 21:
        Func_020047c0(1);
        Func_020047c6(2);
        Func_020047cc(3);
        Func_020047c2(0x90e);
        Func_0200426a();
        break;
    case 67:
        Func_02004428();
        break;
    case 68:
        Func_02004512();
        break;
    case 31:
        Func_020047ea(1);
        Func_020047f0(2);
        Func_020047f6(3);
        Func_020047ec_a(0x90f);
        Func_02004590();
        break;
    default:
        break;
    }
}
