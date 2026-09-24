/* NONMATCHING: 184 bytes, candidate 186 (2026-09-24). Unit symbol:
 * Local_030003e0 (IWRAM modulo) at its veneer 0x0200b520. Remaining: the
 * HImode zero for the spark frame is loaded after the delay store and lands in
 * r8 in the reference (actor in r6); here actor takes r8. */
#include "TYPES.H"

s32 Engine_ActorGet();
s32 Engine_RandomNext();
s32 Local_030003e0();
s32 Engine_ObjectCreate();


extern u8 Data_00000000[];

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ s32 Value0(s32 (*f)())
{
    return f();
}

static __inline__ s32 Value4(s32 (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    return f(a0, a1, a2, a3);
}

struct Sprite378 {
    u8 pad[9];
    u8 lo : 2;
    u8 layer : 2;
    u8 hi : 4;
};

/* Spawn a spark near the actor with a random delay and lifetime. */
void ShindenHeya_Func020031b8(s32 id)
{
    u8 *actor;
    u8 *obj;
    struct Sprite378 *spr;
    s32 x;
    s32 r;
    s32 zero;

    actor = (u8 *)Engine_ActorGet(id);
    if (actor == 0)
        return;
    r = Local_030003e0(Engine_RandomNext(), 20);
    x = *(s32 *)(actor + 8);
    x += r << 16;
    x += -0xa0000;
    obj = (u8 *)Engine_ObjectCreate(0x11e, x, *(s32 *)(actor + 12) + ((Engine_RandomNext() & 15) << 16) + -0x80000, *(s32 *)(actor + 16));
    if (obj == 0)
        return;
    spr = *(struct Sprite378 **)(obj + 80);
    obj[85] = 0;
    *(u16 *)(obj + 100) = Local_030003e0(Engine_RandomNext(), 10) + 5;
    zero = (u16)(u32)Data_00000000;
    *(u16 *)(obj + 102) = Local_030003e0(Engine_RandomNext(), 60) + 30;
    *(s32 *)(obj + 108) = 0x200b145;
    ((u8 *)spr)[38] = zero;
    spr->layer = (*(struct Sprite378 **)(actor + 80))->layer;
}
