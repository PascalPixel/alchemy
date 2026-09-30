#include "TYPES.H"
#include "STAGED_ACTOR.H"
#include "CALL.H"

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
s32 Engine_GameFlagSet(s32 flag);
void Map_CopyCellAttributeRect(s32 src_x, s32 src_y, s32 width, s32 height, s32 dest_x, s32 dest_y);
void Audio_PlayCue(s32 cue);
u8 *Object_GetById();
void ObjectGroup_ConfigureChildValue();
void Engine_TaskAddCallback();
void Engine_TaskRemoveCallback();
void Field_TryJumpForward(void);
void GomaHashira_SpawnPillarEffect();
void FieldScene_RunPrimarySequence(void);
void FieldScene_RunPillarBurst(void);

static __inline__ void Event_Begin(void)
{
    Engine_EventBegin();
}

static __inline__ void Event_End(void)
{
    Engine_EventEnd();
}

static __inline__ void Event_Wait(s32 frames)
{
    Battle_WaitMode0(frames);
}

static __inline__ void Actor_SetSpriteFlags(void *actor, s32 flags)
{
    Engine_ActorSetSpriteFlags(actor, flags);
}

static __inline__ void Object_SetBlendMode(void *object, s32 mode)
{
    Engine_ObjectSetBlendMode(object, mode);
}

static __inline__ void Actor_SetPosition(s32 actor, s32 fixed_x, s32 fixed_z)
{
    Engine_ActorSetPosition(actor, fixed_x, fixed_z);
}

static __inline__ void Actor_SetDestinationOffset(s32 actor, s32 dx, s32 dz)
{
    ObjectMotion_OffsetPositionAndResetMotion(actor, dx, dz);
}

static __inline__ void Actor_WaitForMove(s32 actor)
{
    ObjectMotion_CommitCurrentPositionAndActivate(actor);
}

static __inline__ void Actor_SetSpeed(s32 actor, s32 speed, s32 acceleration)
{
    ObjectMotion_SetSpeedParameters(actor, speed, acceleration);
}

static __inline__ void Actor_SetDestination(s32 actor, s32 x, s32 z)
{
    Engine_ActorSetDestination(actor, x, z);
}

static __inline__ s32 Random_Next(void)
{
    return Engine_RandomNext();
}

static __inline__ void Work_SetValuesIfNonNegative(s32 first, s32 second, s32 third)
{
    Engine_WorkSetValuesIfNonNegative(first, second, third);
}

static __inline__ void MapRender_WaitForValues(void)
{
    Engine_MapRenderWaitForValues();
}

static __inline__ s32 GameFlag_Set(s32 flag)
{
    return Engine_GameFlagSet(flag);
}

static __inline__ void Map_CopyCellAttributes(s32 src_x, s32 src_y, s32 width, s32 height,
                                              s32 dest_x, s32 dest_y)
{
    Map_CopyCellAttributeRect(src_x, src_y, width, height, dest_x, dest_y);
}

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
    ((void (*)())Engine_EventBegin)();
    if (kind == 25) {
        p0 = Object_GetById(11);
        zero = 0;
        p0[34] = 1;
        r2 = (s32)Object_GetById(11);
        Actor_SetSpriteFlags(r2, 0);
        ((void (*)())ObjectGroup_ConfigureChildValue)(11, 14);
        r3 = (s32)Object_GetById(11);
        Object_SetBlendMode(r3, 1);
        Actor_SetPosition(11, 0x19e0000, 0xf00000);
        Event_Wait(10);
        base = (s32)FieldScene_RunPrimarySequence;
        Engine_TaskAddCallback(base, 0xc80);
        Audio_PlayCue(141);
        Actor_SetDestinationOffset(9, 1, 0);
        ((void (*)())ObjectMotion_CommitCurrentPositionAndActivate)(9);
        Event_Wait(10);
        Actor_SetDestinationOffset(9, 2, 0);
        Actor_WaitForMove(9);
        r4 = (s32)Object_GetById(9);
        *(s32 *)(r4 + 68) = zero;
        r5 = (s32)Object_GetById(9);
        *(s32 *)(r5 + 72) = 0x9999;
        Event_Wait(3);
        Actor_SetSpeed(9, 0x28000, 0x4000);
        Audio_PlayCue(0x120);
        Actor_SetDestination(9, 0x1a0, 200);
        r6 = (s32)Object_GetById(9);
        Actor_SetSpriteFlags(r6, 0);
        Engine_TaskRemoveCallback(base);
        Event_Wait(12);
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
        v3 = Random_Next();
        n = -n;
        Call7(GomaHashira_SpawnPillarEffect, a0, *(s32 *)(rec4 + 12), a2, zero, n, (s32)((u32)(v3 << 1) >> 16), zero);
        Event_Wait(20);
        Audio_PlayCue(154);
        Work_SetValuesIfNonNegative(0x50000, 0x50000, 0x10000);
        Work_SetValuesIfNonNegative(-1, -1, 0xe666);
        MapRender_WaitForValues();
        Actor_SetPosition(9, 0, 0);
        Actor_SetPosition(11, 0, 0);
        GameFlag_Set(0x300);
        Map_CopyCellAttributes(21, 45, 4, 2, 21, 11);
    }
    Event_End();
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
            GameFlag_Set(0x301);
        }
    }
}

void FieldScene_RunThreeCallSequence(void)
{
    Event_Begin();
    FieldScene_RunPillarBurst();
    Event_End();
}

void FieldScene_RunFourStepSequence(void)
{
    Event_Begin();
    StagedActor_AdvancePair();
    FieldScene_RunPillarBurst();
    Event_End();
}

void FieldScene_TryJumpForward(void)
{
    Field_TryJumpForward();
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
