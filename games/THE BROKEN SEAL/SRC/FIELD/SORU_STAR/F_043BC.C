#include "TYPES.H"

struct Actor *Engine_ActorGet(s32 actor);
void Engine_AudioPlayCue(s32 cue);
void Engine_ActorSetChildValue();
void Engine_ActorSetSpriteFlags(struct Actor *actor, s32 flags);
void Engine_EventWait(s32 frames);
u32 Engine_RandomNext(void);
void Effect_Spawn(s32 x, s32 y, s32 z, s32 dx, s32 dy, s32 dz, s32 lift, void *params);

/* Passing the constant through a wrapper loads it straight into the argument
 * register. */
static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

struct Actor {
    u8 pad[8];
    s32 x;
    s32 y;
    s32 z;
};

struct EffectParams {
    s32 count;
    s32 kind;
    s32 spread;
    s32 rise;
    u8 pad10[8];
    u16 tile;
    u16 pad1a;
    u8 pad1c[12];
};

/* Actor 14 rises back into view, throwing sparks every other frame. */
void SoruStar_Func020043bc(void)
{
    struct EffectParams params;
    struct EffectParams *p;
    struct Actor *actor;
    u32 i;
    s32 x;
    s32 r;

    actor = Engine_ActorGet(14);
    Engine_AudioPlayCue(190);
    Engine_ActorSetSpriteFlags(Engine_ActorGet(14), 0);
    p = &params;
    p->count = 1;
    p->kind = 5;
    p->tile = 0x11c;
    p->spread = 0x6666;
    p->rise = 0x30000;
    for (i = 0; i <= 31; i++) {
        Engine_EventWait(1);
        if (!(i & 1)) {
            r = ((Engine_RandomNext() * 24) >> 16) << 16;
            x = actor->x;
            x += r;
            x += -0xc0000;
            Effect_Spawn(x, actor->y + (((Engine_RandomNext() << 5) >> 16) << 16) + 0x200000, actor->z, 0, -0x40000, 0, 0x1b0000, p);
        }
        if (i == 20)
            Call2(Engine_ActorSetChildValue, 14, 0x100);
    }
    Engine_ActorSetChildValue(14, 0);
    Engine_ActorSetSpriteFlags(Engine_ActorGet(14), 1);
}
