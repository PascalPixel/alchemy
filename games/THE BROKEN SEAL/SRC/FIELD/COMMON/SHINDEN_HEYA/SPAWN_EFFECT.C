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

/* Spawns the effect object above actor a0 with its script, zeroes its state and copies the owner's sprite mode. */
void ShindenHeya_SpawnOwnerEffect(s32 a0, s32 a1)
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
            { u16 v = 0; p5[38] = v; }
            { u8 m = ((struct Flags9 *)(*(s32 *)(rec + 80)))->mode; *(s32 *)((s32)rec8 + 104) = rec; ((struct Flags9 *)p5)->mode = m; } /* FAKEMATCH: the mode is read into a temporary so the owner store schedules first */
        }
    }
}
