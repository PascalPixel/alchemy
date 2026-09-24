/* NONMATCHING: 184 bytes, candidate 182, 1 differing halfword (2026-09-24).
 * Unit symbol: Local_030003e0 (IWRAM modulo) at its veneer 0x0200b520. The
 * body is exact: the spark frame zero is a HImode value in a one-halfword
 * struct, which puts it in r8 through a pool load as the reference does. The
 * one halfword is the trailing alignment pad at 0x0200326e inside the 184-byte
 * reviewed extent: the first adoption check passes, but the repeatability
 * check (recovery/adopt.rs repeatable, score --align) counts the pad and
 * refuses ("exact in 1 of 30 compiles"). Adopt once that check tolerates a
 * trailing alignment halfword. */
#include "TYPES.H"

s32 Engine_ActorGet();
s32 Engine_RandomNext();
s32 Local_030003e0();
s32 Engine_ObjectCreate();



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
/* A zero kept in a one-halfword struct, so it is a HImode value the compiler
 * holds in a high register and reloads from the pool. */
struct Half {
    u16 v;
};

/* Shrine room: spawns a spark effect at a random offset above an actor, with random delay and lifetime, on the actor's sprite layer. */
void Local_020031b8(s32 id)
{
    u8 *actor;
    u8 *obj;
    struct Sprite378 *spr;
    s32 x;
    s32 r;
    struct Half zero;

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
    /* FAKEMATCH: the spark frame zero held in a halfword struct. */
    zero.v = 0;
    *(u16 *)(obj + 102) = Local_030003e0(Engine_RandomNext(), 60) + 30;
    *(s32 *)(obj + 108) = 0x200b145;
    ((u8 *)spr)[38] = zero.v;
    spr->layer = (*(struct Sprite378 **)(actor + 80))->layer;
}
