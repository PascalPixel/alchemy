#include "TYPES.H"
#include "STAGED_ACTOR.H"
#include "CALL.H"
#include "TBS_EDITION.H"
#if defined(TBS_EDITION_JA)
#define FIELD_STAGED_ACTOR_IMPORTS
#include "FIELD_EVENT.H"
#include "FIXED_POINT_POSITION.H"

s32 Object_CheckMovementCollision(struct FieldActor *object, struct FixedPointPosition *position);
void WaitFrames(s32 frames);
#endif

extern u8 GomaHashira_Extras[];
void Engine_EventBegin(void);
void Engine_EventEnd(void);
void Battle_WaitMode0(s32 frames);
void Engine_ActorSetSpriteFlags();
void Engine_ObjectSetBlendMode();
void Engine_ActorSetPosition(s32 actor, s32 fixed_x, s32 fixed_z);
void ObjectMotion_OffsetPositionAndResetMotion(s32 actor, s32 dx, s32 dz);
void ObjectMotion_CommitCurrentPositionAndActivate(s32 actor);
void ObjectMotion_SetSpeedParameters(s32 actor, s32 speed, s32 acceleration);
void Engine_ActorSetDestination(s32 actor, s32 x, s32 z);
s32 Engine_RandomNext(void);
void Engine_WorkSetValuesIfNonNegative(s32 first, s32 second, s32 third);
void Engine_MapRenderWaitForValues(void);
#if !defined(TBS_EDITION_JA)
s32 Engine_GameFlagSet(s32 flag);
#endif
void Map_CopyCellAttributeRect(s32 src_x, s32 src_y, s32 width, s32 height, s32 dest_x, s32 dest_y);
void Audio_PlayCue(s32 cue);
#if !defined(TBS_EDITION_JA)
u8 *Object_GetById();
#endif
void ObjectGroup_ConfigureChildValue();
#if !defined(TBS_EDITION_JA)
void Engine_TaskAddCallback();
void Engine_TaskRemoveCallback();
#endif
void Field_TryJumpForward(void);
void GomaHashira_SpawnPillarEffect();
void FieldScene_RunPrimarySequence(void);
void FieldScene_RunPillarBurst(void);

#if !defined(TBS_EDITION_JA)
static __inline__ void Work_SetValuesIfNonNegative(s32 first, s32 second, s32 third)
{
    /* FAKEMATCH: forwarding through this helper preserves measured instruction order in its callers; see the retained direct-call draft. */
    Engine_WorkSetValuesIfNonNegative(first, second, third);
}

static __inline__ void Map_CopyCellAttributes(s32 src_x, s32 src_y, s32 width, s32 height,
                                              s32 dest_x, s32 dest_y)
{
    /* FAKEMATCH: forwarding through this helper preserves measured instruction order in its callers; see the retained direct-call draft. */
    Map_CopyCellAttributeRect(src_x, src_y, width, height, dest_x, dest_y);
}
#endif

void Engine_ActorFaceDirection();
void Engine_ActorShowEmote();
void Engine_ActorStartRepeatedMotion();
void Engine_ActorJump();
void Object_SetModeById();
extern u8 *gWork;
extern s16 Data_02000240[];

/* Frames actor 13 has idled, in the overlay's own work past its image. */
s32 GomaHashira_Actor13Frames;

/*
 * The pillar at the square: pushing it over, its fall and the burst of
 * pillar effects, and the steps that run them.
 *
 * This file does not include FIELD_EVENT.H: the staged-actor module names
 * the audio import Audio_PlayCue, which FIELD_EVENT.H spells as an inline
 * wrapper. The wrappers these steps use are spelled here with the names
 * this overlay's imports carry.
 */
void FieldScene_RunPillarSequence(void)
{
    s32 kind;
    s32 zero;
    s32 base;
    s32 a0;
    s32 a2;
    s32 rec4;
    s32 r1, r2, r3, r4, r5, r6, r7;
    s32 rec7, v1, v2, v3, t, u, n;
    u8 *p0;

    r1 = ((s32 (*)())Object_GetById)(9);
    kind = *(s32 *)(r1 + 8) / 0x100000;
    Engine_EventBegin();
    if (kind == 25) {
        p0 = Object_GetById(11);
        zero = 0;
        p0[34] = 1;
        r2 = (s32)Object_GetById(11);
        Engine_ActorSetSpriteFlags(r2, 0);
        ObjectGroup_ConfigureChildValue(11, 14);
        r3 = (s32)Object_GetById(11);
        Engine_ObjectSetBlendMode(r3, 1);
        Engine_ActorSetPosition(11, 0x19e0000, 0xf00000);
        Battle_WaitMode0(10);
        base = (s32)FieldScene_RunPrimarySequence;
        Engine_TaskAddCallback(base, 0xc80);
        Audio_PlayCue(141);
        ObjectMotion_OffsetPositionAndResetMotion(9, 1, 0);
        ObjectMotion_CommitCurrentPositionAndActivate(9);
        Battle_WaitMode0(10);
        ObjectMotion_OffsetPositionAndResetMotion(9, 2, 0);
        ObjectMotion_CommitCurrentPositionAndActivate(9);
        r4 = (s32)Object_GetById(9);
        *(s32 *)(r4 + 68) = zero;
        r5 = (s32)Object_GetById(9);
        *(s32 *)(r5 + 72) = 0x9999;
        Battle_WaitMode0(3);
        ObjectMotion_SetSpeedParameters(9, 0x28000, 0x4000);
        Audio_PlayCue(0x120);
        Engine_ActorSetDestination(9, 0x1a0, 200);
        r6 = (s32)Object_GetById(9);
        Engine_ActorSetSpriteFlags(r6, 0);
        Engine_TaskRemoveCallback(base);
        Battle_WaitMode0(12);
        Audio_PlayCue(189);
        rec7 = ((s32 (*)())Object_GetById)(9);
        v1 = Engine_RandomNext();
        a0 = *(s32 *)(rec7 + 8);
        a0 = a0 + (s32)((((u32)(((v1 << 1) + v1) << 2)) >> 16) << 16);
        rec4 = (s32)Object_GetById(9);
        r7 = (s32)Object_GetById(9);
        a2 = *(s32 *)(r7 + 16);
        a2 = a2 + 0x60000;
        v2 = Engine_RandomNext();
        t = (s32)((u32)((v2 << 2) + v2) >> 16);
        u = (((t << 1) + t) << 2) + t;
        n = u << 6;
        n = n - u;
        n = n << 3;
        n = n + t;
        v3 = Engine_RandomNext();
        n = -n;
        Call7(GomaHashira_SpawnPillarEffect, a0, *(s32 *)(rec4 + 12), a2, zero, n, (s32)((u32)(v3 << 1) >> 16), zero);
        Battle_WaitMode0(20);
        Audio_PlayCue(154);
        Work_SetValuesIfNonNegative(0x50000, 0x50000, 0x10000);
        Work_SetValuesIfNonNegative(-1, -1, 0xe666);
        Engine_MapRenderWaitForValues();
        Engine_ActorSetPosition(9, 0, 0);
        Engine_ActorSetPosition(11, 0, 0);
        Engine_GameFlagSet(0x300);
        Map_CopyCellAttributes(21, 45, 4, 2, 21, 11);
    }
    Engine_EventEnd();
}

void FieldScene_RunPillarBurst(void)
{
    s32 a;
    s32 b;
    s32 x0, x1, x2;
    s32 c2;
    s32 q1, q2, q3, e1, e2;
    s32 g1a, g1b, g1c, g2a, g2b, g2c, g3a, g3b, g3c, g4a, g4b, g4c, g5a, g5b, g5c, g6a, g6b, g6c;

    q1 = ((s32 (*)())Object_GetById)(10);
    a = *(s32 *)(q1 + 8) / 0x100000;
    q2 = ((s32 (*)())Object_GetById)(10);
    b = *(s32 *)(q2 + 16) / 0x100000;
    if (a == 38) {
        if (b == 14) {
            q3 = (s32)Object_GetById(10);
            *(s32 *)(q3 + 12) = -0x20000;
            e1 = ((s32 (*)())Object_GetById)(10);
            e2 = ((s32 (*)())Object_GetById)(10);
            *(s32 *)(e1 + 60) = *(s32 *)(e2 + 12);
            Audio_PlayCue(188);
            g1a = ((s32 (*)())Object_GetById)(10);
            g1b = ((s32 (*)())Object_GetById)(10);
            g1c = (s32)Object_GetById(10);
            GomaHashira_SpawnPillarEffect(*(s32 *)(g1a + 8), *(s32 *)(g1b + 12),
                  *(s32 *)(g1c + 16), 0x8000, 0, 0, 1);
            g2a = ((s32 (*)())Object_GetById)(10);
            g2b = ((s32 (*)())Object_GetById)(10);
            g2c = (s32)Object_GetById(10);
            GomaHashira_SpawnPillarEffect(*(s32 *)(g2a + 8), *(s32 *)(g2b + 12),
                  *(s32 *)(g2c + 16), 0x6666, 0x6666, 0, 1);
            g3a = ((s32 (*)())Object_GetById)(10);
            g3b = ((s32 (*)())Object_GetById)(10);
            g3c = (s32)Object_GetById(10);
            x0 = *(s32 *)(g3a + 8);
            x1 = *(s32 *)(g3b + 12);
            x2 = *(s32 *)(g3c + 16);
            c2 = -0x6666;
            GomaHashira_SpawnPillarEffect(x0, x1, x2, c2, 0x6666, 0, 1);
            g4a = ((s32 (*)())Object_GetById)(10);
            g4b = ((s32 (*)())Object_GetById)(10);
            g4c = (s32)Object_GetById(10);
            GomaHashira_SpawnPillarEffect(*(s32 *)(g4a + 8), *(s32 *)(g4b + 12),
                  *(s32 *)(g4c + 16), -0x8000, 0, 0, 1);
            g5a = ((s32 (*)())Object_GetById)(10);
            g5b = ((s32 (*)())Object_GetById)(10);
            g5c = (s32)Object_GetById(10);
            GomaHashira_SpawnPillarEffect(*(s32 *)(g5a + 8), *(s32 *)(g5b + 12),
                  *(s32 *)(g5c + 16), 0x6666, c2, 0, 1);
            g6a = ((s32 (*)())Object_GetById)(10);
            g6b = ((s32 (*)())Object_GetById)(10);
            g6c = (s32)Object_GetById(10);
            GomaHashira_SpawnPillarEffect(*(s32 *)(g6a + 8), *(s32 *)(g6b + 12),
                  *(s32 *)(g6c + 16), c2, c2, 0, 1);
            Engine_GameFlagSet(0x301);
        }
    }
}

void FieldScene_RunThreeCallSequence(void)
{
    Engine_EventBegin();
    FieldScene_RunPillarBurst();
    Engine_EventEnd();
}

void FieldScene_RunFourStepSequence(void)
{
    Engine_EventBegin();
    StagedActor_AdvancePair();
    FieldScene_RunPillarBurst();
    Engine_EventEnd();
}

void FieldScene_TryJumpForward(void)
{
#if defined(TBS_EDITION_JA)
    struct FieldActor *obj;
    struct FixedPointPosition target;
    struct FixedPointPosition *pos;
    u8 *flags;
    u8 saved;

    obj = Object_GetById(0);
    flags = &obj->motion_flags;
    saved = *flags;
    target.x = Object_GetById(0)->x.fixed - 0x200000;
    pos = &target;
    pos->y = Object_GetById(0)->y.fixed;
    pos->z = Object_GetById(0)->z.fixed;
    if (Object_CheckMovementCollision(obj, pos) == 0) {
        Engine_EventBegin();
        Object_SetMode(obj, 6);
        WaitFrames(6);
        Audio_PlayCue(152);
        Object_SetMode(obj, 7);
        obj->speed = 0x30000;
        obj->acceleration = 0x20000;
        obj->velocity_y = 0x40000;
        *flags &= 0x7e;
        Engine_ActorSetSpriteFlags(obj, 0);
        Engine_ActorMoveToAndWait(0, *(s16 *)((u8 *)pos + 2), *(s16 *)((u8 *)pos + 10));
        Object_SetMode(obj, 6);
        Engine_ActorSetSpriteFlags(obj, 1);
        *flags = saved;
        Engine_EventEnd();
    }
#else
    Field_TryJumpForward();
#endif
}

u8 *SceneData_GetExtraTable(void)
{
    return GomaHashira_Extras;
}

/* Drive actor 13's idle routine from a frame counter. */
void GomaHashira_DriveActor13Idle(void)
{
    u8 *work;
    s32 t;

    work = gWork;
    t = ++GomaHashira_Actor13Frames;
    switch (t) {
    case 60:
        Call3(Engine_ActorFaceDirection, 13, 0x2000, 0);
        Engine_ActorShowEmote(13, 2, 0);
        break;
    case 180:
        Engine_ActorStartRepeatedMotion(13, 3);
        break;
    case 240:
    case 270:
        Engine_ActorJump(13, 4, 0);
        break;
    case 480:
        Object_SetModeById(13, 4);
        break;
    }
    /* FAKEMATCH: the 99 goes through the counter variable so it is built with
     * movs instead of a halfword pool constant. */
    if (Data_02000240[282] == 0)
        *(u16 *)(work + 0x182) = t = 99;
}
