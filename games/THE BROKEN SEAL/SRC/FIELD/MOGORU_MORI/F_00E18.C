#include "TYPES.H"

struct Actor *Engine_ActorGet(s32 actor);
void Engine_AudioPlayCue(s32 cue);
s32 Engine_MathCos(s32 angle);
s32 Engine_MathSin(s32 angle);
/* The IWRAM divide, reached through this overlay's import veneer. */
s32 Local_03000380(s32 num, s32 den);
void Effect_Spawn(s32 x, s32 y, s32 z, s32 dx, s32 dy, s32 dz, s32 lift, void *params);

struct Actor {
    u8 pad[8];
    s32 x;
    s32 y;
    s32 z;
};

struct Vec {
    s32 x;
    s32 y;
    s32 z;
};

struct EffectParams {
    s32 count;
    u8 pad04[36];
};

/* A ring of seventeen puffs bursts out around the actor. */
void MogoruMori_Func02000e18(s32 id)
{
    struct Vec dir;
    struct EffectParams params;
    struct EffectParams *p;
    struct Vec *v;
    struct Actor *actor;
    u32 i;
    s32 x;
    s32 z;

    actor = Engine_ActorGet(id);
    Engine_AudioPlayCue(188);
    p = &params;
    p->count = 1;
    for (i = 0; i <= 16; i++) {
        v = &dir;
        v->x = Engine_MathCos(i << 12);
        v->y = 0;
        z = Engine_MathSin(i << 12);
        x = v->x;
        v->z = z;
        x += Local_03000380(x, 3);
        v->x = x;
        Effect_Spawn(actor->x, 0x100000, actor->z, x, v->y + 0x1999, z, 0x20000, p);
    }
}
