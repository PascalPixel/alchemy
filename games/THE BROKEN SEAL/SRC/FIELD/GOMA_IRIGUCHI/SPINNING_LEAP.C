#include "TYPES.H"

struct Actor *Engine_ActorGet(s32 actor);
void Engine_TaskWait(s32 frames);
s32 Engine_MathCos(s32 angle);
void Engine_ActorSetSpeed();
void Engine_ActorSetDestination(s32 actor, s32 x, s32 y);
void Engine_ActorWaitForMove(s32 actor);
void OverlayObject_WaitForHeight();
void Engine_AudioPlayCue(s32 cue);
void Engine_WorkSetValuesIfNonNegative();
s32 Engine_MathSin(s32 angle);
void Effect_Spawn(s32 x, s32 y, s32 z, s32 dx, s32 dy, s32 dz, s32 lift, void *params);
void Engine_ActorSetAnimation(s32 actor, s32 anim);
void Engine_MapWaitWorkValuesBelow256(void);

/* Passing constants through these wrappers loads them straight into the
 * argument registers. */
static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

struct Actor {
    u8 pad00[8];
    s32 x;
    s32 y;
    s32 z;
    u8 pad14[10];
    s16 angle;
    u8 pad20[2];
    u8 flags34;
    u8 pad23[5];
    s32 speed;
    u8 pad2c[12];
    s32 hover;
    u8 pad3c[12];
    s32 accel;
    u8 pad4c[4];
    struct Actor *sprite;
    u8 pad54;
    u8 layer;
    u8 pad56[22];
    s32 callback;
};

struct Vec {
    s32 x;
    s32 y;
    s32 z;
};

struct EffectParams {
    s32 count;
    s32 kind;
    s32 spread;
    s32 rise;
    s32 rise2;
    s32 spin;
    u16 tile;
    u16 pad1a;
    u8 pad1c[12];
};

/* The actor spins up out of the ground, lands with a ring of dust and a
 * final burst. */
void GomaIriguchi_RunSpinningLeap(s32 id)
{
    struct Vec dir;
    struct EffectParams params;
    struct Vec *v;
    struct EffectParams *p;
    struct Actor *actor;
    u32 i;
    s32 x;
    s32 z;
    s32 zero;

    actor = Engine_ActorGet(id);
    actor->layer = 0;
    for (i = 0; i <= 17; i++) {
        Engine_TaskWait(1);
        actor->sprite->angle += -256;
        actor->x -= Engine_MathCos((u16)actor->sprite->angle) / 2;
        actor->hover = 0x80000000;
    }
    actor->callback = 0x2008da9;
    Call3(Engine_ActorSetSpeed, id, 0x30000, 0x18000);
    Engine_ActorSetDestination(id, 160, 192);
    actor->accel = 0xcccc;
    actor->layer = 3;
    actor->flags34 = 0;
    Engine_ActorWaitForMove(id);
    Call2(OverlayObject_WaitForHeight, (s32)actor, 0x200000);
    Engine_AudioPlayCue(188);
    Call3(Engine_WorkSetValuesIfNonNegative, 0x50000, 0x50000, 0x10000);
    Engine_AudioPlayCue(141);
    Call3(Engine_WorkSetValuesIfNonNegative, -1, -1, 0xe666);
    for (i = 0; i <= 16; i++) {
        v = &dir;
        /* FAKEMATCH: zero is set inside the loop (always entered) so it is
         * materialized after the counter and the direction pointer. */
        zero = 0;
        v->x = Engine_MathCos(i << 12);
        v->y = zero;
        v->z = Engine_MathSin(i << 12);
        v->x -= v->x / 4;
        v->z -= v->z / 2;
        Effect_Spawn(actor->x, actor->y, actor->z, v->x, v->y, v->z, zero, (void *)zero);
    }
    actor->speed = 0x50000;
    Engine_ActorSetDestination(id, 139, 196);
    Engine_ActorWaitForMove(id);
    Call2(OverlayObject_WaitForHeight, (s32)actor, 0x200000);
    actor->callback = zero;
    actor->sprite->angle = 0x1000;
    p = &params;
    p->tile = 214;
    p->spread = 0x8000;
    p->rise = 0xcccc;
    p->rise2 = 0x18000;
    p->spin = 0x13333;
    Effect_Spawn(actor->x, actor->y, actor->z, 0, zero, zero, 0x1c0000, p);
    Engine_AudioPlayCue(154);
    Engine_ActorSetAnimation(id, 3);
    Engine_MapWaitWorkValuesBelow256();
}
