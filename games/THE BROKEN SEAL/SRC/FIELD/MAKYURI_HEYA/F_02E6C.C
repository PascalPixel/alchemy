#include "TYPES.H"

u8 *Engine_ActorGet(s32 actor);
void Engine_ActorSetSpriteFlags(u8 *actor, s32 flags);
void Engine_ActorSetChildValue(s32 actor, s32 value);
void Engine_AudioPlayCue(s32 cue);
void SceneEffect_SpawnWithRandomOffset(s32 x, s32 y, s32 z);
u32 Engine_RandomNext(void);
void Effect_Spawn(s32 x, s32 y, s32 z, s32 dx, s32 dy, s32 dz, s32 lift, void *params);
void Engine_TaskWait(s32 frames);

struct EffectParams {
    s32 count;
    s32 kind;
    u8 pad08[16];
    u16 tile;
    u16 pad1a;
    s32 script;
    u8 pad20[8];
};

/* Passing the constant through a wrapper loads it straight into the argument
 * register. */
static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

/* The actor sinks away in a burst of sparks over 48 frames. */
void MakyuriHeya_Func02002e6c(s32 actor)
{
    struct EffectParams params;
    struct EffectParams *p;
    u8 *rec;
    u32 i;
    s32 x;
    s32 y;
    s32 speed;

    rec = Engine_ActorGet(actor);
    rec[85] = 0;
    Engine_ActorSetSpriteFlags(rec, 0);
    Call2(Engine_ActorSetChildValue, actor, 0x100);
    Engine_AudioPlayCue(221);
    p = &params;
    p->count = 1;
    p->kind = 5;
    p->tile = 0x11e;
    p->script = 0x200de70;
    for (i = 0; i <= 47; i++) {
        if (i <= 31)
            SceneEffect_SpawnWithRandomOffset(*(s32 *)(rec + 8), *(s32 *)(rec + 12), *(s32 *)(rec + 16));
        if (i & 1) {
            Engine_AudioPlayCue(246);
            x = *(s32 *)(rec + 8) + (((Engine_RandomNext() * 24) >> 16) << 16) + -0xc0000;
            y = *(s32 *)(rec + 12) + (((Engine_RandomNext() << 5) >> 16) << 16) + -0x100000;
            speed = (((Engine_RandomNext() << 2) >> 16) << 15) + 0x8000;
            Effect_Spawn(x, y, *(s32 *)(rec + 16), 0, speed, 0, 0x330000, p);
        }
        *(s32 *)(rec + 12) += i * 0x1999;
        *(s32 *)(rec + 60) = *(s32 *)(rec + 12);
        Engine_TaskWait(2);
    }
}
