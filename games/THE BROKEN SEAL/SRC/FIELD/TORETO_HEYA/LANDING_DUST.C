#include "TYPES.H"

void Engine_EventBegin(void);
void Engine_CameraMoveTo();
void Engine_MapRedraw(void);
void Engine_TaskWait(s32 frames);
u8 *Engine_ActorGet(s32 actor);
void Engine_EventOpenScreen(void);
void Engine_EventWaitForScreen(void);
void Engine_AudioPlayCue(s32 cue);
void Engine_EventWait(s32 frames);
s32 Engine_MathCos(s32 angle);
s32 Engine_MathSin(s32 angle);
void Effect_Spawn(s32 x, s32 y, s32 z, s32 dx, s32 dy, s32 dz, s32 lift, void *params);
void Engine_ActorSetAttachedEffect();
void Engine_ActorSetAnimation(s32 actor, s32 anim);
void Engine_WorkSetValuesIfNonNegative();
void Engine_MapWaitWorkValuesBelow256(void);
void Engine_EventEnd(void);

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

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

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
    u8 pad10[20];
    s32 script;
};

/* The leader lands in the room in a ring of dust, then the camera settles. */
void ToretoHeya_RunLandingDustScene(void)
{
    struct EffectParams params;
    struct Vec dir;
    struct EffectParams *p;
    struct Vec *v;
    u8 *leader;
    u32 i;
    s32 x;
    s32 z;

    Engine_EventBegin();
    Call4(Engine_CameraMoveTo, -1, -1, -1, 0);
    Engine_MapRedraw();
    Engine_TaskWait(1);
    *(s32 *)(Engine_ActorGet(0) + 12) = 0x820000;
    *(s32 *)(Engine_ActorGet(0) + 72) = 0x8000;
    *(s32 *)(Engine_ActorGet(0) + 68) = 0;
    Engine_ActorGet(0)[85] = 0;
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Engine_AudioPlayCue(204);
    Engine_ActorGet(0)[85] = 3;
    Engine_EventWait(24);
    leader = Engine_ActorGet(0);
    p = &params;
    p->kind = 7;
    p->script = 0x200985d;
    p->spread = 0xcccc;
    p->rise = 0xcccc;
    for (i = 0; i <= 16; i++) {
        v = &dir;
        v->x = Engine_MathCos(i << 12);
        v->y = 0;
        z = Engine_MathSin(i << 12);
        x = v->x;
        v->z = z;
        x += x / 2;
        v->x = x;
        Effect_Spawn(*(s32 *)(leader + 8), *(s32 *)(leader + 12), *(s32 *)(leader + 16), x, v->y, z, 0x1090001, p);
    }
    Engine_AudioPlayCue(188);
    Call2(Engine_ActorSetAttachedEffect, 0, 0x101);
    Engine_ActorSetAnimation(0, 22);
    Call3(Engine_WorkSetValuesIfNonNegative, 0x50000, 0x50000, 0x10000);
    Call3(Engine_WorkSetValuesIfNonNegative, -1, -1, 0xe666);
    Engine_MapWaitWorkValuesBelow256();
    Engine_ActorSetAttachedEffect(0, 0x100);
    *(s32 *)(Engine_ActorGet(0) + 72) = 0x10000;
    *(s32 *)(Engine_ActorGet(0) + 68) = 0x4000;
    Engine_EventEnd();
}
