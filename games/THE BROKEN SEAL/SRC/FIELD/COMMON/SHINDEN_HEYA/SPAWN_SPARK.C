#include "TYPES.H"

s32 Engine_ActorGet();
s32 Engine_RandomNext();
s32 IwramUnsignedRemainder();
s32 Engine_ObjectCreate();
void ShindenHeya_UpdateRisingSpark();

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
void ShindenHeya_SpawnActorSpark(s32 id)
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
    r = IwramUnsignedRemainder(Engine_RandomNext(), 20);
    x = *(s32 *)(actor + 8);
    x += r << 16;
    x += -0xa0000;
    obj = (u8 *)Engine_ObjectCreate(0x11e, x, *(s32 *)(actor + 12) + ((Engine_RandomNext() & 15) << 16) + -0x80000, *(s32 *)(actor + 16));
    if (obj == 0)
        return;
    spr = *(struct Sprite378 **)(obj + 80);
    obj[85] = 0;
    *(u16 *)(obj + 100) = IwramUnsignedRemainder(Engine_RandomNext(), 10) + 5;
    /* FAKEMATCH: the spark frame zero held in a halfword struct. */
    zero.v = 0;
    *(u16 *)(obj + 102) = IwramUnsignedRemainder(Engine_RandomNext(), 60) + 30;
    *(s32 *)(obj + 108) = (s32)ShindenHeya_UpdateRisingSpark;
    ((u8 *)spr)[38] = zero.v;
    spr->layer = (*(struct Sprite378 **)(actor + 80))->layer;
}
