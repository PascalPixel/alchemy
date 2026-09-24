/* NONMATCHING: 130 of 132 bytes, 7 differing halfwords (2026-09-24). Written as
 * a single-overlay unit source. The zero byte comes from a HImode pool constant
 * as in the reference; remaining: the script-pointer and flag stores are
 * scheduled differently and the function is 2 bytes short. */
#include "TYPES.H"

s32 Engine_ActorGet();
s32 Engine_ObjectCreate();
void Engine_ObjectSetScript();


extern u8 Data_00000000[];

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ s32 Value0(s32 (*f)())
{
    return f();
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ s32 Value4(s32 (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    return f(a0, a1, a2, a3);
}

struct Flags9 {
    u8 pad[9];
    u8 low : 2;
    u8 mode : 2;
};

void Local_02002750(s32 a0, s32 a1)
{
    u32 i;
    s32 p8;
    s32 p8b;
    s32 rec;
    u8 *rec8;
    s32 record;
    u8 *p5;

    p8 = a1;
    rec = Value0(Engine_ActorGet);
    if (rec != 0) {
        rec8 = Value4(Engine_ObjectCreate, 0x11d, *(s32 *)(rec + 8), (*(s32 *)(rec + 12) + 0x2d0000), *(s32 *)(rec + 16));
        if ((s32)rec8 != 0) {
            p5 = *(s32 *)((s32)rec8 + 80);
            Call2(Engine_ObjectSetScript, (s32)rec8, 0x200c15c);
            {
                s32 zero = 0;

                rec8[85] = zero;
                *(u16 *)(rec8 + 100) = zero;
            }
            *(u16 *)(rec8 + 102) = p8;
            *(s32 *)((s32)rec8 + 108) = 0x200a6f1;
            { s32 v = (u16)(u32)Data_00000000; p5[38] = v; }
            ((struct Flags9 *)p5)->mode = ((struct Flags9 *)(*(s32 *)(rec + 80)))->mode;
            *(s32 *)((s32)rec8 + 104) = rec;
        }
    }
}
