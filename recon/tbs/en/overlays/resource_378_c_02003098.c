#include "TYPES.H"


s32 Func_02006614();
void Func_0200661a();
s32 Func_02006636();
void Func_0200664c();
void Func_0200665c();
void Func_0200665e();
void Func_02006698();
void Func_020066ae();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

/* NONMATCHING: 172 of 172 bytes, 12 halfword edits (2026-09-24). Same shape
 * as 380:02004260. Remaining: the -33 mask is folded to 0xdf (a word mask
 * variable grows the function), the sprite[28] load is scheduled late, and
 * the loop zero is not shared with the counter in r5. */
void Func_02003098(s32 item)
{
    u8 *obj;
    u8 *spr;
    s32 buf;
    s32 zero;
    u32 i;

    obj = Value1(Func_02006614, 22);
    zero = 0;
    if (obj != 0) {
        Call2(Func_0200661a, (s32)obj, 0x200b8f8);
        spr = *(u8 **)(obj + 80);
        spr[38] = zero;
        spr[39] = zero;
        spr[5] &= -33;
        spr[9] &= 15;
        *(s32 *)(obj + 40) = 0x20000;
        *(s32 *)(obj + 72) = 0x4000;
        buf = Value2(Func_02006636, 17, 0x608);
        Func_020066ae(item);
        Func_0200665e(spr[28], 128, buf + 0x400);
        Func_0200665c(17);
        for (i = 0; i <= 59; i++) {
            if ((u32)(*(s32 *)(obj + 40) + 255) <= 0x1fe)
                obj[85] = 0;
            Func_0200664c(1);
        }
        Func_02006698((s32)obj, 0x200ba9c);
    }
}
