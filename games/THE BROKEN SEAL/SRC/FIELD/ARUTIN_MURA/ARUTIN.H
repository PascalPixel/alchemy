#ifndef ALCHEMY_ARUTIN_H
#define ALCHEMY_ARUTIN_H

#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"



struct Slot02000338 {
    u8 head[6];
    u16 angle;
};

struct SceneMotion {
    u8 unk_00[8];
    s32 x;
    s32 y;
    s32 z;
    s32 ground;
    s32 scale_x;
    s32 scale_y;
    u8 unk_20[8];
    s32 velocity;
    u8 unk_2c[47];
    u8 state;
    u8 unk_5c[8];
    s16 timer;
    s16 delay;
    s32 active;
    void (*callback)(struct SceneMotion *);
};

struct Actor {
    s32 f00;
    s32 f04;
    s32 f08;
};

struct Actor02000c0c {
    u8 head[18];
    u8 gap18[6];
    s32 span;
    s32 reach;
    u8 gap32[48];
    u8 *attached;
    u8 gap84;
    u8 state;
};

/* A cell pair for each of the village's exit triggers. */
extern u8 ArutinMura_TriggerTable[];

s32 Battle_GetWorkObject1e0();
void BattleFx_SetQueuedSoundAndPlay(s32 sound);
u32 Random16(void);
void SceneEffect_UpdateDriftingParticle(struct SceneMotion *work);
void SceneEffect_SpawnDriftingParticle(void);

/* Calls spelled through these wrappers pass their constants straight into the
 * argument registers. Spelling them as direct calls instead precomputes a
 * costly constant into a temporary that is then shared with later uses in the
 * same block. A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call1(void (*f)(), s32 a0)
{

    f(a0);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{

    return f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{

    f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{

    f(a0, a1, a2);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

static __inline__ void Call3_020006f0(void (*func)(), s32 a, s32 b, s32 c)
{
    func(a, b, c);
}

static __inline__ struct SceneMotion *CreateActor(
    struct SceneMotion *(*func)(s32, s32, s32, s32),
    s32 kind, s32 x, s32 y, s32 z)
{
    return func(kind, x, y, z);
}

static __inline__ s32 Value6(s32 (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    return f(a0, a1, a2, a3, a4, a5);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}


#endif
