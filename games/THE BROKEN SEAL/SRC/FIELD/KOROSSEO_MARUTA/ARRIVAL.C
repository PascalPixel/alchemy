#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "COLOSSO_LOG_ROLLING_STAGE.H"



void Korosseo_FinishSoloRound();
void FieldScene_RunMiddleSequence();
void Object_SetPosition();
void Object_CommitPosition();

static __inline__ void Call1(void (*func)(), s32 a0)
{
    func(a0);
}

static __inline__ void Call2(void (*func)(), s32 a0, s32 a1)
{
    func(a0, a1);
}

static __inline__ void Call3(void (*func)(), s32 a0, s32 a1, s32 a2)
{
    func(a0, a1, a2);
}

static __inline__ void Call4(void (*func)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    func(a0, a1, a2, a3);
}

static __inline__ s32 Value2(s32 (*func)(), s32 a0, s32 a1)
{
    return func(a0, a1);
}

static __inline__ s32 Value3(s32 (*func)(), s32 a0, s32 a1, s32 a2)
{
    return func(a0, a1, a2);
}

static __inline__ void *Pointer1(void *(*func)(), s32 a0)
{
    return func(a0);
}

void FieldScene_RunDualArrivalSequence(s32 scene)
{
    void *p17;
    void *p19;
    void *p23;
    void *p25;
    s32 state;

    if (gGameState.entrance == 2) {
        Korosseo_FinishSoloRound();
        return;
    }
    Engine_EventBegin();
    state = Value2(ColossoLogRollingStage_RunStateInteraction, scene, 1);
    if (state == 0) {
        Call1(Engine_EventSetMessage, 8370);
        Call2(Engine_CameraSetSpeed, 196608, 24576);
        Call4(Engine_CameraMoveTo, 9961472, -1, 13107200, 1);
        Engine_CameraWaitForMove();
        Call1(Engine_EventWait, 30);
        Call2(Engine_EventShowMessage, scene, 0);
        Call3(ColossoLogRollingStage_StartPaletteTask, 104, 68, 0);
        Call1(Engine_EventWait, 60);
        Call3(ColossoLogRollingStage_StartPaletteTaskFromState, 168, 96, 10);
        Call1(Engine_EventWait, 70);
        Call2(Engine_EventShowMessage, scene, 0);
        ColossoLogRollingStage_StopPaletteTask();
        Call1(Engine_TaskWait, 2);
        p17 = Pointer1(Engine_ActorGet, 10);
        *(u8 *)((u8 *)p17 + 85) = 0;
        *(s32 *)(p17 + 52) = 26214;
        *(s32 *)(p17 + 48) = 52428;
        Call4(Object_SetPosition, (s32)p17, *(s32 *)(p17 + 8), 262144, *(s32 *)(p17 + 16));
        p19 = Pointer1(Engine_ActorGet, 11);
        *(u8 *)((u8 *)p19 + 85) = 0;
        *(s32 *)(p19 + 52) = 26214;
        *(s32 *)(p19 + 48) = 52428;
        Call4(Object_SetPosition, (s32)p19, *(s32 *)(p19 + 8), 2097152, *(s32 *)(p19 + 16));
        Call1(Object_CommitPosition, p19);
        Call1(Engine_EventWait, 45);
        p23 = Pointer1(Engine_ActorGet, 10);
        *(u8 *)((u8 *)p23 + 85) = 0;
        *(s32 *)(p23 + 52) = 26214;
        *(s32 *)(p23 + 48) = 52428;
        Call4(Object_SetPosition, (s32)p23, *(s32 *)(p23 + 8), 2097152, *(s32 *)(p23 + 16));
        p25 = Pointer1(Engine_ActorGet, 11);
        *(u8 *)((u8 *)p25 + 85) = 0;
        *(s32 *)(p25 + 52) = 26214;
        *(s32 *)(p25 + 48) = 52428;
        Call4(Object_SetPosition, (s32)p25, *(s32 *)(p25 + 8), 262144, *(s32 *)(p25 + 16));
        Call1(Object_CommitPosition, p25);
        Call1((void (*)())Engine_EventWait, 15);
        Call2(Engine_EventShowMessage, scene, 0);
        Call3(ColossoLogRollingStage_StartPaletteTask, 104, 68, 0);
        Call1(Engine_EventWait, 30);
        Call3(ColossoLogRollingStage_StartPaletteTaskFromState, 168, 96, 10);
        Call1(Engine_EventWait, 40);
        Call3(ColossoLogRollingStage_StartPaletteTaskFromState, 104, 68, 10);
        Call1(Engine_EventWait, 70);
        Call2(Engine_EventShowMessage, scene, 0);
        ColossoLogRollingStage_StopPaletteTask();
        Call1(Engine_TaskWait, 2);
        Call2(Engine_CameraFollowActor, 0, 0);
        Call2(ColossoLogRollingStage_InitializeStateInteraction, scene, 1);
    } else if (state == 1) {
        Call1(Engine_EventSetMessage, 0x20b1);
        Call2(Engine_EventShowMessage, scene, 0);
    }
    Value3(FieldScene_RunMiddleSequence, state, scene, 1);
    Engine_EventEnd();
}
