#include "TYPES.H"

s32 Engine_ActorGet();
void Engine_ActorSetPosition();
void Engine_ActorSetAnimation();



/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

struct Flags9 {
    u8 pad[9];
    u8 low : 2;
    u8 mode : 2;
};

struct Flags5 {
    u8 pad[5];
    u8 flags;
};

struct Flags37 {
    u8 pad[37];
    u8 flags;
};

struct Flags35 {
    u8 pad[35];
    u8 flags;
};

struct Flags39 {
    u8 pad[39];
    u8 count;
};

void ShindenHeya_CopyActorPose(void)
{
    u32 i;
    s32 rec7;
    s32 record;
    s32 v0;
    u8 *v2;
    u8 *p5;

    record = Value1(Engine_ActorGet, 8);
    if (record != 0) {
        Engine_ActorSetPosition(14, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    Engine_ActorSetAnimation(14, 0);
    rec7 = Value1(Engine_ActorGet, 14);
    record = Value1(Engine_ActorGet, 8);
    *(u16 *)(rec7 + 6) = *(u16 *)(record + 6);
    record = Engine_ActorGet(14);
    *(s32 *)(record + 108) = 0x200a67d;
    record = Value1(Engine_ActorGet, 14);
    p5 = *(s32 *)(record + 80);
    {
        for (i = 0; i < ((struct Flags39 *)p5)->count; i++) {
            u8 *e = ((u8 **)(p5 + 40))[i];

            if (e != 0 && *(s32 *)(e + 16) != 0) {
                ((struct Flags5 *)e)->flags = 10;
            }
        }
    }
    ((struct Flags37 *)p5)->flags = 1;
    ((struct Flags35 *)(Engine_ActorGet(14)))->flags &= 254;
    ((struct Flags9 *)p5)->mode = 2;
}
