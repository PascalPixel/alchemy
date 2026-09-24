/* NONMATCHING: 492 of 492 bytes, 3 differing halfwords (2026-09-24). Unit source
 * (evconv Engine_* veneers). Remaining: before the last
 * FieldScene_RunScene39f_02000d90 the reference moves px into r1 after the
 * third and fourth arguments; here it moves first. */
#include "TYPES.H"

u8 *Engine_ActorGet();
void Engine_EventBegin();
void Engine_ActorSetChildValue();
void Engine_ActorSetSpriteFlags();
void Engine_ActorSetPosition();
void Engine_CameraSetSpeed();
void Engine_CameraMoveTo();
void Engine_CameraWaitForMove();
void Engine_EventWait();
void Engine_WorkSetValuesIfNonNegative();
void MogoruMori_Func02000e18();
void Engine_ActorStartRepeatedMotion();
void Engine_MapWaitWorkValuesBelow256();
void Engine_ActorFaceDirection();
void FieldScene_RunScene39f_02000d90();
s32 Engine_MathCos();
s32 Engine_MathSin();
void Effect_Spawn(s32 x, s32 y, s32 z, s32 dx, s32 dy, s32 dz, s32 lift, s32 params);
void Engine_AudioPlayCue();
void Engine_ActorRunRepeatedMotion();
void Engine_GameStateSetReturn();
void Engine_Import0808a250();
void Engine_EventEnd();


extern u8 Data_00000046[];
extern u8 Data_02000240[];

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

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

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

static __inline__ void Call7(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6)
{
    f(a0, a1, a2, a3, a4, a5, a6);
}

struct Vec {
    s32 x;
    s32 y;
    s32 z;
};

void MogoruMori_Func020022c0(void)
{
    struct Vec dir;
    struct Vec *v;
    u8 *actor;
    u32 i;
    s32 x;
    s32 z;
    s32 zero;
    s32 base3_2000240;

    actor = (u8 *)Value1(Engine_ActorGet, 18);
    Engine_EventBegin();
    Engine_ActorSetChildValue(18, 15);
    Engine_ActorSetSpriteFlags(Engine_ActorGet(18), 0);
    Call3(Engine_ActorSetPosition, 18, 0x880000, 0x1680000);
    Call2(Engine_CameraSetSpeed, 0x8000, 0x1000);
    Call4(Engine_CameraMoveTo, 0x880000, -1, 0x1880000, 1);
    Engine_CameraWaitForMove();
    Engine_EventWait(60);
    Call3(Engine_WorkSetValuesIfNonNegative, 0x50000, 0x50000, 0x10000);
    MogoruMori_Func02000e18(18);
    Engine_ActorStartRepeatedMotion(0, 2);
    Call3(Engine_WorkSetValuesIfNonNegative, -1, -1, 0xe666);
    Engine_MapWaitWorkValuesBelow256();
    Call3(Engine_ActorFaceDirection, 0, 0xc000, 20);
    Engine_EventWait(40);
    Call3(Engine_WorkSetValuesIfNonNegative, 0x50000, 0x50000, 0x10000);
    MogoruMori_Func02000e18(18);
    Call3(Engine_WorkSetValuesIfNonNegative, -1, -1, 0xe666);
    Engine_MapWaitWorkValuesBelow256();
    Engine_EventWait(40);
    MogoruMori_Func02000e18(18);
    *(s32 *)(actor + 24) = 0x13333;
    *(s32 *)(actor + 28) = 0x13333;
    Engine_ActorSetChildValue(18, 5);
    FieldScene_RunScene39f_02000d90(18, 136, 0x188, 0xf0000);
    Engine_EventWait(15);
    Call3(Engine_WorkSetValuesIfNonNegative, 0x50000, 0x50000, 0x10000);
    for (i = 0; i <= 16; i++) {
        v = &dir;
        /* FAKEMATCH: zero is set inside the loop (always entered) so it is
         * materialized with the counter. */
        zero = 0;
        v->x = Engine_MathCos(i << 12);
        v->y = zero;
        z = Engine_MathSin(i << 12);
        x = v->x;
        v->z = z;
        x += x / 2;
        v->x = x;
        Effect_Spawn(*(s32 *)(actor + 8), 0, *(s32 *)(actor + 16), x, v->y, z, 1, zero);
    }
    Engine_EventWait(30);
    Call3(Engine_WorkSetValuesIfNonNegative, -1, -1, 0xe666);
    Engine_MapWaitWorkValuesBelow256();
    Engine_AudioPlayCue(148);
    Engine_ActorRunRepeatedMotion(18, 2);
    Engine_EventWait(20);
    {
        s32 px = *(s16 *)(Engine_ActorGet(0) + 10);

        FieldScene_RunScene39f_02000d90(18, px, *(s16 *)(Engine_ActorGet(0) + 18) - 16, 0x80000);
    }
    Engine_EventWait(10);
    base3_2000240 = (s32)Data_02000240;
    *(u8 *)((base3_2000240 + 0x22b)) = 3;
    Engine_GameStateSetReturn((s32)Data_00000046, 15);
    Engine_Import0808a250(53, 1);
    Engine_EventEnd();
}
